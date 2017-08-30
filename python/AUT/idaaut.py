#coding=gbk
"""
idaaut : ida auto update
"""
import idaapi
import idc
import idautils
import idaex
import uuts
import traceback
import Queue
import greenlet
import os

#----------------------------------------------------------------
#-------------------------------说明---------------------------------------
#------------以下是建议
#全局自定义名称一律以'@'打头，函数内局部名称无所谓。
#全局变量以"g"前缀。
#全局函数以"f"前缀。
#本地名称以"l"前缀。
#类型说明，字符串(默认字节大小为1)：s，数组：a，类型/结构体：c，指针：p，无符号：u，函数：f，浮点：d，指令(insn)：i，偏移(默认为有符号)：o，布尔(默认字节大小为1)：b。
#数据大小按字节为单位的数字，（1、2、4、8），分别对应（char、short、int、longlong）。
#真正的名字之前要加'_'
#---以下数据格式示例
# 成员函数的格式：@f_ClassName_FunctionName。
# 全局多字节字符串：@gs_StrName。
# 全局utf16字符串：@gs2_StrName。
# 全局uint：@gu4_Name。
# 全局int*：@gp4_Name。
# 全局结构体指针：@gpc_Name。
# 全局结构体变量：@gc_Name。
# uint的本地变量：lu4_Name。
#------------以下是约定
#lineA里的代码是由源idb数据库来执行的，是用来生成定位与操作的对象的。
#lineB里的代码是由目标idb执行的，主要用来更新后，细微之处的断言之类的，用于判断该地址或该地址附近的内容是否符合你的心意。
#lineB当然也可以做到，根据该地址指示的偏移，来更新某个结构体成员的偏移，或来更新enum常量的数值。
#OpFn能够自动识别出函数内部引用的全局名称（全局变量或函数），在更新的时候，自动把这些引用给设置成相应的名称。
#OpFn根据内部某地址处是否有注释（任意注释，lineA、lineB、cmt或repeat cmt），(如果有注释)来自动设置相应的操作数信息（o_imm、o_mem、o_phrase、o_displ）。
#操作数信息，o_imm：Immediate Value. o_mem：Direct Memory Reference  (DATA). o_phrase：Memory Ref [Base Reg + Index Reg]. o_displ：Memory Reg [Base Reg + Index Reg + Displacement].
#-------------以下是配置相关
#在使用本文件功能前，必须要先设置DefGlobal，像这样：uuts.DefGlobal = lambda : globals()
#uuts.SetLogFile("f:/tmp/tmp/log.txt")，是用来设置log文件的，如果没有设置，则不会输出log到文件中。
#设置出错对象保存的文件，idaaut.SetErrorObjFile("f:/tmp/tmp/errorObj.txt", "f:/tmp/tmp/continueErrorObj.txt")。
#-------------功能与用法
#EnvObjMgr：它的成员函数得到的对象保存了idb文件要更新的信息，你可以把这些信息保存在文件中，像这样：obj.WriteToFile("f:/tmp/tmp/obj.txt")
#EnvObjMgr在name list中搜索名字，根据指定的前缀和指定的名字，来决定生成哪些函数或全局变量的对象，前缀和指定的名字可修改与配置，详情去看EnvObjMgr的实现。
#Interface中的成员函数用来执行EnvObjMgr生成的对象，也就是进行更新的操作，它支持xmlrpc的网络调用，也支持通过读取文件来获取对象，再调用。
#EaEnvGen，环境对象，作用于lineA，lineA里全局变量self就指向了一个EaEnvGen对象。
#EaEnvRun，环境对象，作用于lineB，lineB里全局变量self就指向了一个EaEnvRun对象。
#-------------核心算法相关概念
#对一个地址进行更新，需要两部分：定位和操作。
#定位(Finder)，需要查找定位的数据与方式，由前缀为Loc的一组类表示。
#另外，Finder若要定位到正确的数据，还需要前置断言、转换器与后置断言，前置断言和后置断言由前缀为Assert的一组类表示，转换器由前缀为Tr的一组类表示。
#前置断言，Loc得到一个地址，先交由前置断言处理，若不通过则Loc再去查找下一地址，若通过由交由Tr来处理。
#转换器，根据前置断言传递过来的地址进行判断与转换，如果成功则返回调整后的地址给后置断言，否则Loc再去查找下一地址。
#后置断言，根据前置断言或转换器传过来的地址进行最后的判断，若通过则Finder就找到了最终的地址，否则Loc再去查找下一地址。
#Finder的Loc不能为空，其他均可为空。
#操作，Op开关的一组类，用于对Finder得到的地址进行操作。
#----------------------------------------------------------------------------------
#--------------常量定义---------------------------------------------
#更新操作的优先级定义，数值越小优化级越高。UPL -> Update Priority Level
UPL_Enum = 1
UPL_Struct = 1000
UPL_Func = 2000
UPL_Chunk = 3000
UPL_GloData = 4000
UPL_Last = 10000
#----------------------------------------------------------------------------------
#----------------------------------------------------------------
#ea环境
class _EaEnvBase:
    # 当前的地址
    curEa = idc.BADADDR

#ea环境，用于生成对象
class EaEnvGen(_EaEnvBase):
    """
    GF: Gen Finder
    GOF: Gen Operate Function
    GOD: Gen Operate Data
    """
    #当前生成的loc(定位器)
    curLoc = None
    #当前的前置断言
    curPa = None
    #当前的转换器
    curTr = None
    #当前的后置断言
    curAa = None
    #当前生成的操作
    curOp = None

    def _Reset(self):
        self.curLoc = None
        self.curPa = None
        self.curTr = None
        self.curAa = None
        self.curOp = None

    def GF_FnBiOrd(self, offsetBegin = 0, offsetSize = 20, fnCheckSum = True, occurCnt = None):
        """
        简单常用的二进制搜索
        Bi : Binary
        """
        seg = idaapi.getseg(self.curEa)
        if not seg:
            assert False, "FnBiOrd之没找到区段"
            return
        fnRange = idaex.EaRangeFn(self.curEa)
        subRange = fnRange.SubRange(offsetBegin, offsetSize, True)
        bvalue = idaex.GenBinary(subRange.eaBegin, subRange.eaEnd)
        findRange = idaex.EaRangeSegName(idaapi.get_true_segm_name(seg))
        self.curLoc = LocBinary(bvalue, findRange)
        self.curPa = AssertAllOf(AssertFuncSize(fnRange.Size()))
        if fnCheckSum:
            sumSrc = idaex.FuncFullCheck(subRange.eaBegin, fnRange.eaEnd)
            sumDst = AssertFuncFullCheck(sumSrc)
            self.curPa.Add(sumDst)
        if occurCnt is not None:
            self.curPa.Add(AssertAsCnt(occurCnt))
        self.curTr = TrFnOffsetStart(-offsetBegin)

    def GF_FnCrefByName(self, fnSubName, isFnOffset=None, fnCheckSum=True):
        """
        简单常用的code ref by name
        GF : Gen Finder
        """
        frm, to = idaex.FindNameOfFuncCallsFrom(fnSubName, self.curEa)
        self.curLoc = LocCodeRefTo(to, isFnOffset)
        fnRange = idaex.EaRangeFn(self.curEa)
        if fnCheckSum:
            # AssertFuncFullCheck若是当作前置断言的话，验证的是从frm到函数结尾的check sum。
            sumSrc = idaex.FuncFullCheck(fnRange.eaBegin, fnRange.eaEnd)
            self.curAa = AssertFuncFullCheck(sumSrc)
        self.curPa = AssertFuncSize(fnRange.Size())
        self.curTr = TrFnOffsetStart(self.curEa - frm)

    def GF_FnDrefByName(self, subName, fnCheckSum=True):
        """
        简单常用的data ref by name
        GF : Gen Finder
        """
        frm, to = idaex.FindNameOfFuncDrefFrom(subName, self.curEa)
        self.curLoc = LocDataRefTo(to)
        fnRange = idaex.EaRangeFn(self.curEa)
        if fnCheckSum:
            sumSrc = idaex.FuncFullCheck(fnRange.eaBegin, fnRange.eaEnd)
            # AssertFuncFullCheck若是当作前置断言的话，验证的是从frm到函数结尾的check sum。
            self.curAa = AssertFuncFullCheck(sumSrc)
        self.curPa = AssertFuncSize(fnRange.Size())
        self.curTr = TrFnOffsetStart(self.curEa - frm)

    def GF_FnDrefByText(self, subStartText, fnCheckSum=True):
        """
        简单常用的data ref by text
        GF : Gen Finder
        """
        frm, to = idaex.FindTextOfFuncTextFrom(subStartText, self.curEa)
        # 若to在目标数据库中未找到名字，则用subStartText寻找
        self.curLoc = LocDataRefTo(idaex.NetnodeText(to, subStartText))
        fnRange = idaex.EaRangeFn(self.curEa)
        if fnCheckSum:
            sumSrc = idaex.FuncFullCheck(fnRange.eaBegin, fnRange.eaEnd)
            # AssertFuncFullCheck若是当作前置断言的话，验证的是从frm到函数结尾的check sum。
            self.curAa = AssertFuncFullCheck(sumSrc)
        self.curPa = AssertFuncSize(fnRange.Size())
        self.curTr = TrFnOffsetStart(self.curEa - frm)

    def GF_FnXrefAuto(self, startswidth=None, fnCheckSum=True):
        """
        智能得到函数中的已存在的全局引用
        GF : Gen Finder
        """
        xref = idaex.FindAnyFnXrefFrom(self.curEa, startswidth)
        self.curLoc = LocXrefTo(xref.to, xref.type, xref.iscode, xref.user)
        fnRange = idaex.EaRangeFn(self.curEa)
        if fnCheckSum:
            sumSrc = idaex.FuncFullCheck(fnRange.eaBegin, fnRange.eaEnd)
            # AssertFuncFullCheck若是当作前置断言的话，验证的是从frm到函数结尾的check sum。
            self.curAa = AssertFuncFullCheck(sumSrc)
        self.curPa = AssertFuncSize(fnRange.Size())
        self.curTr = TrFnOffsetStart(self.curEa - xref.frm)

    def GF_ByNameNormal(self):
        self.curLoc = LocByGloName(self.curEa)

    def GOF_Normal(self):
        self.curOp = OpFn(self.curEa)

    def GOD_Normal(self, doUknFlag=idc.DOUNK_SIMPLE):
        """
        DOUNK_SIMPLE有时会失败，DOUNK_DELNAMES能够解决这种失败的情况。当这段内存已有命名的时候，DOUNK_SIMPLE会失败。
        """
        self.curOp = OpData(self.curEa, doUknFlag)


#ea环境，用于执行，用于更新后的执行
class EaEnvRun(_EaEnvBase):
    pass

#--------------------------通信接口------------------------------
@uuts.singleton
class Interface:
    """
     更新接口
    """
    def __init__(self, changeCoding):
        self.changeCoding = changeCoding

    #执行一段命令
    def Exec(self, execStr, dictLocal, dictGlobal):
        try:
            if self.changeCoding:
                execStr = uuts.StrFromUStr(execStr)
            res = uuts.Execs(execStr, dictLocal, dictGlobal, uuts.DefGlobal())
            if self.changeCoding:
                res = uuts.ObjToUStr(res)
            return res
        except:
            # traceback.print_exc()
            uuts.LogError(traceback.format_exc())
            # return False, traceback.format_exc()

    def ExecObj(self, strObj):
        if self.changeCoding:
            strObj = uuts.StrFromUStr(strObj)
        obj = uuts.BuildObj(strObj)
        res = obj()
        if self.changeCoding:
            res = uuts.ObjToUStr(res)
        return res

    def ExecObjFromFile(self, strFilePath):
        if self.changeCoding:
            strFilePath = uuts.StrFromUStr(strFilePath)
        obj = uuts.ObjFromFile(strFilePath)
        res = obj()
        if self.changeCoding:
            res = uuts.ObjToUStr(res)
        return res

    def ExecObjFromFiles(self, strMainFile, subFileName):
        if self.changeCoding:
            strMainFile = uuts.StrFromUStr(strMainFile)
            subFileName = uuts.StrFromUStr(subFileName)
        strMainFile = uuts.NormalPathCase(strMainFile)
        obj = uuts.ObjFromFile(strMainFile)
        dir = os.path.dirname(strMainFile)
        for f in uuts.IterDirFiles(dir, subFileName):
            f = uuts.NormalPathCase(f)
            if f != strMainFile:
                obj.AddFromFile(f)
        res = obj()
        if self.changeCoding:
            res = uuts.ObjToUStr(res)
        return res


#----------------------------------------------------------------
#---------------------global variable-------------------------
def _GenDefCodeRange():
    res = uuts.ValueRange()
    for n in xrange(idaapi.get_segm_qty()):
        seg = idaapi.getnseg(n)
        if seg:
           if idc.SEG_CODE == idc.GetSegmentAttr(seg.startEA, idc.SEGATTR_TYPE):
               if res.begin_ == 0:
                    res.begin_ = seg.startEA
                    res.end_ = seg.endEA
               else:
                    v = uuts.ValueRange(seg.startEA, seg.endEA)
                    if res.IsProximity(v):
                        res = res.CalcUnion(v)
                    else:
                        break
    return res

defCodeAddr = _GenDefCodeRange()
#----------------------------------------------------------------
#----------------------------------------------------------------
class Finder(uuts.Serialize):
    """
     loc_ : locator,定位方式、定位方法、定位器。
     pa_ : pre assert,前置断言
     tr_ : translator,转换器
     aa_ : after assert,后置断言
     ea_ : result address
    """
    ea_ = idc.BADADDR
    errorPos = -2

    #init ordinary
    def _InitOrd(self, loc, pa = None, tr = None, aa = None):
        self.loc_ = loc
        assert self.loc_
        self.pa_ = pa
        self.tr_ = tr
        self.aa_ = aa

    def _InitDict(self, d):
        self.loc_ = uuts.BuildObj(d["loc"])
        assert self.loc_
        self.pa_ = uuts.BuildObj(d["pa"])
        self.tr_ = uuts.BuildObj(d["tr"])
        self.aa_ = uuts.BuildObj(d["aa"])

    @uuts.LogStack
    def Do(self, isDbg = False):
        """
         算法核心，进行查找定位，并断言过滤
        """
        cnt = 0
        self.errorPos = -1
        for ea in self.loc_():
            cnt += 1
            self.errorPos = 0
            self.ea_ = ea
            if self.pa_ and self.pa_(ea, cnt) <= 0:
                if isDbg:
                    uuts.LogDebug("Finder.DebugDo之前置断言失败，地址: %x " % ea)
                    self.pa_.PrintDbgInfo()
                continue
            if self.tr_:
                self.errorPos = 1
                ea = self.tr_(ea)
                self.ea_ = ea
                self.errorPos = 2
                if ea is None or ea == idc.BADADDR or ea == -1 or ea == 0:
                    if isDbg:
                        uuts.LogDebug("Finder.DebugDo之转换器返回失败，转换后的地址: %x " % ea)
                        self.tr_.PrintDbgInfo()
                    continue
            self.errorPos = 3
            if self.aa_ and self.aa_(ea, cnt) <= 0:
                if isDbg:
                    uuts.LogDebug("Finder.DebugDo之后置断言失败，地址: %x " % ea)
                    self.aa_.PrintDbgInfo()
                continue
            self.errorPos = -2
            return ea
        if cnt == 0:
            self.errorPos = 4
            if isDbg:
                uuts.LogDebug("Finder.DebugDo之没有定位到任何地址")
                self.loc_.PrintDbgInfo()
        return 0

    @uuts.LogStack
    def PrintDbgInfo(self):
        if self.errorPos == -2:
            return
        uuts.LogDebug("-----以下是Finder最后查找到的地址的错误信息")
        if self.errorPos == -1:
            uuts.LogDebug("finder估计进入了循环引用")
        elif self.errorPos == 0:
            uuts.LogDebug("前置断言失败，地址: %x " % self.ea_)
            self.pa_.PrintDbgInfo()
        elif self.errorPos == 1:
            uuts.LogDebug("转换器失败，地址: %x " % self.ea_)
            self.tr_.PrintDbgInfo()
        elif self.errorPos == 2:
            uuts.LogDebug("找到了地址，但是转换器返回失败，转换后的地址：%x" % self.ea_)
            self.tr_.PrintDbgInfo()
        elif self.errorPos == 3:
            uuts.LogDebug("后置断言失败，地址: %x "%self.ea_)
            self.aa_.PrintDbgInfo()
        else:
            uuts.LogDebug("没有定位到任何地址")
            self.loc_.PrintDbgInfo()
        uuts.LogDebug("-----以下是Finder查找的每个地址的详细出错信息")

        try:
            self.Do(True)
        except:
            uuts.LogError("Finder之DebugDo捕获到异常\n", traceback.format_exc())
        uuts.LogDebug("-----以上是Finder的出错信息，到此结束---------------------")

    def _BuildDict(self, d):
        d["loc"] = self.loc_
        d["pa"] = self.pa_
        d["tr"] = self.tr_
        d["aa"] = self.aa_


class LocMulti(uuts.MultiIterCall):
    """
    loc : locator
    """
    pass

class _LocBase(uuts.Serialize):
    def _InitOrd(self, value, eaRange, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW):
        self.value = value
        self.eaRange = eaRange
        self.flag = flag

    def _InitDict(self, d):
        self.value = d["value"]
        self.eaRange = uuts.BuildObj(d["eaRange"])
        self.flag = d["flag"]

    def __call__(self, *args, **kwargs):
        return self.iterFn(self.value, self.eaRange.eaBegin, self.eaRange.eaEnd, self.flag)

    def _BuildDict(self, d):
        d["value"] = self.value
        d["eaRange"] = self.eaRange
        d["flag"] = self.flag


class LocByGloName(uuts.Serialize):
    """
    by global name
    """
    def _InitOrd(self, netnodeId):
        self.nnode = idaex.Netnode(netnodeId)

    def _InitDict(self, d):
        self.nnode_ = d["nnode"]
        self.nnode = None

    def _BuildDict(self, d):
        if self.nnode:
            d["nnode"] = self.nnode
        else:
            d["nnode"] = self.nnode_

    def __call__(self, *args, **kwargs):
        if not self.nnode:
            self.nnode = uuts.BuildObj(self.nnode_)
        yield self.nnode.id


class LocBinary(_LocBase):
    iterFn = lambda self, *args: idaex.IterBinary(*args)

    def PrintDbgInfo(self):
        uuts.LogDebug("LocBinary,eaRange")
        self.eaRange.PrintDbgInfo()


class LocMnem(_LocBase):
    iterFn = lambda self, *args: idaex.IterMnem(*args)

    def PrintDbgInfo(self):
        uuts.LogDebug("LocMnem,eaRange")
        self.eaRange.PrintDbgInfo()


class LocText(_LocBase):
    iterFn = lambda self, *args : idaex.IterText(*args)

    def PrintDbgInfo(self):
        uuts.LogDebug("LocText,eaRange")
        self.eaRange.PrintDbgInfo()


class LocImme(_LocBase):
    iterFn = lambda self, *args : idaex.IterImme(*args)

    def PrintDbgInfo(self):
        uuts.LogDebug("LocImme,eaRange")
        self.eaRange.PrintDbgInfo()


class _LocCrossRefBase(uuts.Serialize):
    def _InitOrd(self, netnode, isTo, isCode):
        self.netnode = netnode
        self.isTo = isTo
        self.isCode = isCode
        if isCode == True:
            if isTo == True:
                self.iterFn = lambda ea: idautils.CodeRefsTo(ea, 0)
            else:
                self.iterFn = lambda ea: idautils.CodeRefsFrom(ea, 0)
        else:
            if isTo == True:
                self.iterFn = idautils.DataRefsTo
            else:
                self.iterFn = idautils.DataRefsFrom

    def _InitDict(self, d):
        self.nnode_ = d["nnode"]
        _LocCrossRefBase._InitOrd(self, None, d["isTo"], d["isCode"])

    def _BuildDict(self, d):
        if self.netnode:
            d["nnode"] = self.netnode
        else:
            d["nnode"] = self.nnode_
        d["isTo"] = self.isTo
        d["isCode"] = self.isCode

    def __call__(self, *args, **kwargs):
        if not self.netnode:
            self.netnode = uuts.BuildObj(self.nnode_)
        return self.iterFn(self.netnode.id)


class _LocCrossRef(_LocCrossRefBase):
    def _InitOrd(self, nnode, isFnOffset, isTo, isCode):
        if not isinstance(nnode, idaex.Netnode):
            if isFnOffset == None:
                nnode = idaex.Netnode(nnode)
            elif isFnOffset == True:
                nnode = idaex.NetnodeFnOffset(nnode)
            else:
                nnode = idaex.NetnodeFnInsn(nnode)
        _LocCrossRefBase._InitOrd(self, nnode, isTo, isCode)


class LocCodeRefTo(_LocCrossRef):
    def _InitOrd(self, nnode, isFnOffset = None):
        _LocCrossRef._InitOrd(self, nnode, isFnOffset, True, True)


class LocCodeRefFrom(_LocCrossRef):
    def _InitOrd(self, nnode, isFnOffset = None):
        _LocCrossRef._InitOrd(self, nnode, isFnOffset, False, True)


class LocDataRefTo(_LocCrossRef):
    def _InitOrd(self, nnode):
        #DataRefTo针对的是数据地址
        _LocCrossRef._InitOrd(self, nnode, None, True, False)


class LocDataRefFrom(_LocCrossRef):
    def _InitOrd(self, nnode, isFnOffset = None):
        #DataRefFrom针对既可以是代码地址，也可以是数据地址，数据地址时isFnOffset等于None
        _LocCrossRef._InitOrd(self, nnode, isFnOffset, False, False)


class _LocXrefBase(uuts.Serialize):
    def _InitDict(self, d):
        self.type = d["type"]
        self.iscode = d["iscode"]
        self.user = d["user"]

    def _InitOrd(self, type = None, iscode = None, user = None):
        self.type = type
        self.iscode = iscode
        self.user = user

    def _BuildDict(self, d):
        d["type"] = self.type
        d["iscode"] = self.iscode
        d["user"] = self.user

    def __call__(self, *args, **kwargs):
        assert False


class _LocXref(_LocXrefBase):
    def _InitDict(self, d):
        _LocXrefBase._InitDict(self, d)
        self.nnode_ = d["nnode"]
        self.nnode = None

    def _InitOrd(self, ea, type = None, iscode = None, user = None):
        _LocXrefBase._InitOrd(self, type, iscode, user)
        self.nnode = idaex.ParseNetnode(ea)

    def _BuildDict(self, d):
        _LocXrefBase._BuildDict(self, d)
        if self.nnode:
            d["nnode"] = self.nnode
        else:
            d["nnode"] = self.nnode_

class LocXrefTo(_LocXref):
    def __call__(self, *args, **kwargs):
        if not self.nnode:
            self.nnode = uuts.BuildObj(self.nnode_)
        for xref in idaex.XrefsToEx(self.nnode.id):
            if self.type is not None and self.type != xref.type:
                continue
            if self.iscode is not None and self.iscode != xref.iscode:
                continue
            if self.user is not None and self.user != xref.user:
                continue
            yield xref.frm


class LocXrefFrom(_LocXref):
    def __call__(self, *args, **kwargs):
        if not self.nnode:
            self.nnode = uuts.BuildObj(self.nnode_)
        for xref in idaex.XrefsFromEx(self.nnode.id):
            if self.type is not None and self.type != xref.type:
                continue
            if self.iscode is not None and self.iscode != xref.iscode:
                continue
            if self.user is not None and self.user != xref.user:
                continue
            yield xref.to


#translator,转换器
class _TrBase(uuts.Serialize):
    pass

class TrHolder(_TrBase):
    def _InitDict(self, d):
        self.data = d["data"]

    def _InitOrd(self, data):
        self.data = data

    def __call__(self, ea, *args, **kwargs):
        return self.data

    def _BuildDict(self, v):
        v["data"] = self.data


class TrFnEnd(_TrBase):
    _res = idc.BADADDR

    def __call__(self, ea, *args, **kwargs):
        self._res = idc.FindFuncEnd(ea)
        return self._res

    def PrintDbgInfo(self):
        if self._res != idc.BADADDR:
            return
        uuts.LogDebug("TrFnEnd之没找到函数结尾")


class TrFnStart(_TrBase):
    notFn = False

    def __call__(self, ea, *args, **kwargs):
        ptr = idaapi.get_func(ea)
        # assert False, "TrFnStart，地址( %x )不是函数内的地址" % ea
        if not ptr:
            self.notFn = True
            return
        self.notFn = False
        return ptr.startEA

    def PrintDbgInfo(self):
        if self.notFn == False:
            return
        uuts.LogDebug("TrFnStart之该地址不属于函数")


class TrEaOffset(_TrBase):
    def _InitDict(self, d):
        self.offset = d["offset"]

    def _InitOrd(self, offset):
        self.offset = offset

    def __call__(self, ea, *args, **kwargs):
        return ea + self.offset

    def _BuildDict(self, v):
        v["offset"] = self.offset


class TrFnOffset(TrEaOffset):
    errorCode = -1

    def __call__(self, ea, *args, **kwargs):
        res = ea + self.offset
        ptr = idaapi.get_func(res)
        if not ptr:
            self.errorCode = 0
            return
        srcPtr = idaapi.get_func(ea)
        if not srcPtr:
            self.errorCode = 1
            return
        if srcPtr.startEA != ptr.startEA:
            self.errorCode = 2
            return
        self.errorCode = -1
        return res

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("TrFnOffset之该地址加上偏移就不属于函数了，偏移：%d"%self.offset)
        elif self.errorCode == 1:
            uuts.LogDebug("TrFnOffset之该地址不属于函数")
        else:
            uuts.LogDebug("TrFnOffset之该地址加上偏移得到的地址与该地址不属于同一个函数了，偏移：%d" % self.offset)


class TrFnOffsetStart(TrFnOffset):
    errorCode = -1

    def __call__(self, ea, *args, **kwargs):
        res = ea + self.offset
        ptr = idaapi.get_func(res)
        if not ptr:
            self.errorCode = 0
            return
        if ptr.startEA != res:
            self.errorCode = 1
            return
        srcPtr = idaapi.get_func(ea)
        if not srcPtr:
            self.errorCode = 2
            return
        if srcPtr.startEA != res:
            self.errorCode = 3
            return
        self.errorCode = -1
        return res

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("TrFnOffsetStart之该地址加上偏移就不属于函数了，偏移：%d"%self.offset)
        elif self.errorCode == 1:
            uuts.LogDebug("TrFnOffsetStart之该地址加上偏移得到地址假设为a,a不是a所在函数的首地址，偏移： %x " % self.offset)
        elif self.errorCode == 2:
            uuts.LogDebug("TrFnOffsetStart之该地址不属于函数")
        else:
            uuts.LogDebug("TrFnOffsetStart之该地址加上偏移得到地址假设为a,a所在函数与该地址所在函数不是同一个函数，偏移： %x " % self.offset)


class _AssertMultiBase(uuts.MultiIterCall):
    pass


class AssertAllOf(_AssertMultiBase):
    def __call__(self, *args, **kwargs):
        for i in self._fns_:
            if i(*args, **kwargs) <= 0:
                return 0
        return len(self._fns_)

class AssertAnyOf(_AssertMultiBase):
    def __call__(self, *args, **kwargs):
        for i in self._fns_:
            res = i(*args, **kwargs)
            if res > 0:
                return res
        return 0

class AssertInRange(uuts.Serialize):
    _res = 1
    def _InitDict(self, d):
        self.eaRange = uuts.BuildObj(d)

    def _InitOrd(self, eaRange):
        self.eaRange = eaRange

    def __call__(self, ea, *args, **kwargs):
        if ea >= self.eaRange.eaBegin and ea < self.eaRange.eaEnd:
            self._res = 1
        else:
            self._res = 0
        return self._res

    def _BuildDict(self, d):
        d["eaRange"] = self.eaRange

    def PrintDbgInfo(self):
        if self._res == 1:
            return
        uuts.LogDebug("AssertInRange之该地址不在有效范围内")


class AssertAsCnt(uuts.Serialize):
    asCnt = True
    def _InitDict(self, d):
        self.cnt_ = d["cnt"]

    def _InitOrd(self, cnt):
        self.cnt_ = cnt

    def __call__(self, ea, cnt, *args, **kwargs):
        self.asCnt = cnt == self.cnt_
        return self.asCnt

    def _BuildDict(self, d):
        d["cnt"] = self.cnt_

    def PrintDbgInfo(self):
        if self.asCnt:
            return
        uuts.LogDebug("AssertAsCnt之出现次数不对，有效次数：%d" % self.cnt_)


class AssertEqual(uuts.Serialize):
    valid = True
    def _InitDict(self, d):
        self.what = uuts.BuildObj(d["what"])
        self.value = d["value"]

    def _InitOrd(self, what, value):
        self.what = what
        self.value = value

    def __call__(self, ea, *args, **kwargs):
        self.valid = self.what(ea) == self.value
        return self.valid

    def _BuildDict(self, d):
        d["what"] = self.what
        d["value"] = self.value

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertEqual之value不相同，有效value：", self.value)


class AssertIsFunc(uuts.Serialize):
    valid = True

    def __call__(self, ea, *args, **kwargs):
        self.valid = idaapi.get_func(ea) is not None
        return self.valid

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertIsFunc之该地址不是函数")


class AssertFuncStart(uuts.Serialize):
    valid = True
    def __call__(self, ea, *args, **kwargs):
        self.valid = idaex.IsFunctionStart(ea)
        return self.valid

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertFuncStart之该地址不是函数或不是函数首地址")


class _AssertWithTrBase(uuts.Serialize):
    def _InitDict(self, d):
        self.value = d["value"]
        self.tr = uuts.BuildObj(d["tr"])
        if not self.tr:
            self.tr = TrHolder(idc.BADADDR)

    def _InitOrd(self, value, tr = None):
        self.value = value
        self.tr = tr
        if not self.tr:
            self.tr = TrHolder(idc.BADADDR)

    def _BuildDict(self, d):
        d["value"] = self.value
        if isinstance(self.tr, TrHolder) and self.tr.data == idc.BADADDR:
            d["tr"] = None
        else:
            d["tr"] = self.tr

    def __call__(self, ea, *args, **kwargs):
        assert False, "必须得定义一个call"


class AssertSameInsnOffset(_AssertWithTrBase):
    valid = True

    def _InitOrd(self, offsetDelta, insnDelta, ea2Bound = None):
        _AssertWithTrBase._InitOrd(self, (offsetDelta, insnDelta), ea2Bound)

    def __call__(self, ea, *args, **kwargs):
        self.valid = idaex.IsSameInsnOffset(self.value[0], self.value[1], ea, self.tr(ea))
        return self.valid

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertSameInsnOffset之该地址与所在函数的相对指令个数或偏移不对")


class _AssertWithTrBase_Equal(_AssertWithTrBase):
    valid = True

    def __call__(self, ea, *args, **kwargs):
        self.valid = self.fn(ea, self.tr(ea)) == self.value
        return self.valid


class AssertFuncSize(_AssertWithTrBase_Equal):
    fn = lambda self, ea, *args: idaex.EaRangeFn(ea).Size()

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertFuncSize之该地址所在函数的size不对")


class AssertInsnAndSize(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.InsnAndSize(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertInsnAndSize之地址范围的size不对或指令个数不对")


class AssertFuncInsnAndSize(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.FuncInsnAndSize(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertFuncInsnAndSize之该地址到函数结尾的size不对或指令个数不对")


class AssertInsnCheckSum(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.InsnCheckSum(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertInsnCheckSum之地址范围的checksum不对")


class AssertFuncCheckSum(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.FuncCheckSum(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertFuncCheckSum之该地址到函数结尾的的checksum不对")


class AssertInsnFullCheck(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.InsnFullCheck(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertInsnFullCheck之地址范围的指令个数、字节个数或checksum不对")


class AssertFuncFullCheck(_AssertWithTrBase_Equal):
    fn = lambda self, *args : idaex.FuncFullCheck(*args)

    def PrintDbgInfo(self):
        if self.valid:
            return
        uuts.LogDebug("AssertFuncFullCheck之该地址到函数结尾的指令个数、字节个数或checksum不对")


class _OpMultiBase(uuts.MultiIterCall):
    def __call__(self, ea, *args, **kwargs):
        assert False, "必须得自己实现"

    def Valid(self):
        if not self._fns_:
            return False
        return len(self._fns_) > 0

    def IsExistName(self, name):
        for i in self._fns_:
            if i.IsExistName(name):
                return True
        return False


class OpAllOf(_OpMultiBase):
    def __call__(self, ea, *args, **kwargs):
        for impl in self._fns_:
            res = impl(ea, *args, **kwargs)
            if res == 0:
                return res
        return len(self._fns_)

class OpSetName(idaex._OpBase):
    _res = True

    def _InitOrd(self, name, globalName = True):
        assert name
        self.name = name
        self.globalName = globalName

    def _InitDict(self, d):
        self.name = d["name"]
        self.globalName = d["globalName"]

    def _BuildDict(self, d):
        d["name"] = self.name
        d["globalName"] = self.globalName

    def __call__(self, ea, *args, **kwargs):
        name = idaapi.netnode(ea).name()
        self._res = True
        if not name or name != self.name:
            self._res = idaex.MakeNameStrict(ea, self.name, self.globalName)
        return self._res

    def IsExistName(self, name):
        return self.name == name

    def PrintDbgInfo(self):
        if self._res:
            return
        uuts.LogDebug("OpSetName之设置名字失败，名字：", self.name)


class OpEaCmt(idaex._OpBase):
    errorCode = -1

    def _InitDict(self, d):
        self.lineA = d["lineA"]
        self.lineB = d["lineB"]
        self.cmt = d["cmt"]
        self.rcmt = d["rcmt"]

    def _InitOrd(self, ea):
        self.lineA = idaex.GetLineCmt(ea, True)
        self.lineB = idaex.GetLineCmt(ea, False)
        self.cmt = idaapi.get_cmt(ea, False)
        self.rcmt = idaapi.get_cmt(ea, True)

    def _BuildDict(self, d):
        d["lineA"] = self.lineA
        d["lineB"] = self.lineB
        d["cmt"] = self.cmt
        d["rcmt"] = self.rcmt

    def __call__(self, ea, *args, **kwargs):
        self.errorCode = 1
        if self.lineB:
            self.errorEa = ea
            idaex.SetLineCmt(ea, self.lineB, False)
            env = EnvObjMgr()
            env.RunByEa(ea, self.lineB)
        self.errorCode = 0
        if self.lineA:
            idaex.SetLineCmt(ea, self.lineA, True)
        if self.cmt:
            idc.MakeComm(ea, self.cmt)
        if self.rcmt:
            idc.MakeRptCmt(ea, self.rcmt)
        self.errorCode = -1
        return True

    @uuts.LogStack
    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpEaCmt之设置注释失败")
        else:
            uuts.LogDebug("OpEaCmt之运行LineB失败，地址： %x ，以下是OpEaCmt运行LineB的详细出错信息" % self.errorEa)
            try:
                self(self.errorEa)
            except:
                uuts.LogError(traceback.format_exc())
        uuts.LogDebug("-----以上是OpEaCmt的出错信息，到此结束---------------------")

    def Valid(self):
        return self.lineA or self.lineB or self.cmt or self.rcmt


class OpEaOffset(OpAllOf):
    def _InitDict(self, d):
        OpAllOf._InitDict(self, d)
        self.offset = d["offset"]

    def _InitOrd(self, offset, *fns):
        # assert len(fns) > 0
        OpAllOf._InitOrd(self, *fns)
        self.offset = offset

    def _BuildDict(self, d):
        OpAllOf._BuildDict(self, d)
        d["offset"] = self.offset

    def __call__(self, ea, *args, **kwargs):
        return OpAllOf.__call__(self, ea + self.offset, *args, **kwargs)


class OpEaXrefInfo(_LocXrefBase):
    errorCode = -1

    def _InitDict(self, d):
        _LocXrefBase._InitDict(self, d)
        self.nameInfo = OpSetName(d["nameInfo"])

    def _InitOrd(self, name, globalName = True, type = None, iscode = None, user = None):
        _LocXrefBase._InitOrd(self, type, iscode, user)
        self.nameInfo = OpSetName(name, globalName)

    def _BuildDict(self, d):
        _LocXrefBase._BuildDict(self, d)
        d["nameInfo"] = self.nameInfo

    def __call__(self, ea, xref, *args, **kwargs):
        if self.type and self.type != xref.type:
            self.errorCode = 0
            assert False, "OpEaXrefInfo之地址： %x ，xref type不同了。" % ea
            return False
        if self.iscode and self.iscode != xref.iscode:
            self.errorCode = 1
            assert False, "OpEaXrefInfo之地址： %x ，xref iscode不同了。" % ea
            return False
        if self.user and self.user != xref.user:
            self.errorCode = 2
            assert False, "OpEaXrefInfo之地址： %x ，xref user不同了。" % ea
            return False
        self.errorCode = 3
        res = self.nameInfo(ea)
        if res != 0:
            self.errorCode = -1
        return res

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        if self.errorCode == 0:
            uuts.LogDebug("OpEaXrefInfo之xref type不同了")
        elif self.errorCode == 1:
            uuts.LogDebug("OpEaXrefInfo之xref iscode不同了")
        elif self.errorCode == 2:
            uuts.LogDebug("OpEaXrefInfo之xref user不同了")
        else:
            uuts.LogDebug("OpEaXrefInfo之设置nameInfo失败")
            self.nameInfo.PrintDbgInfo()

    def Valid(self):
        return self.nameInfo.Valid()

    def IsExistName(self, name):
        return self.nameInfo.IsExistName(name)


class OpEaFromXrefName(idaex._OpBase):
    errorCode = -1
    errorIdx = -1

    def _InitDict(self, d):
        tos = d["tos"]
        if tos:
            self.tos = [uuts.BuildObj(x) for x in tos]
        else:
            self.tos = None

    def _InitOrd(self, ea, top = None):
        self.tos = None
        self.top = top
        fn = idaapi.get_func(ea)
        if fn:
            fnStart = fn.startEA
        else:
            fnStart = idc.BADADDR
        for xref in idaex.XrefsFromEx(ea, 0):
            #若该地址没有手动命名，name便为空
            name = idaapi.netnode(xref.to).name()
            globalName = True
            if not name:
                if not xref.iscode:
                    continue
                # 判断是否函数内名字
                name = idaapi.get_true_name(ea, xref.to)    #idc.GetTrueNameEx
                if name and idaapi.is_uname(name):
                    globalName = False
                else:
                    continue
            #过滤掉结构体与成员的引用
            if not xref.iscode:
                if idaapi.get_member_by_id(xref.to) or idaapi.get_struc(xref.to):
                    continue
            #去重复
            if self.FindToEa(name) or self.top and self.top.IsExistName(name):
                continue
            # print name
            if not self.tos:
                self.tos = []
            self.tos.append(OpEaXrefInfo(name, globalName, xref.type, xref.iscode, xref.user))

    def _BuildDict(self, d):
        d["tos"] = self.tos

    def FindToEa(self, name, idxLimit = -1):
        if not self.tos:
            return
        if idxLimit < 0:
            idxLimit = len(self.tos)
        idx = 0
        for i in self.tos:
            if idx >= idxLimit:
                return
            if i.nameInfo.name == name:
                return i
            idx += 1

    def IsExistName(self, name):
        return self.FindToEa(name) is not None

    def Valid(self):
        if not self.tos:
            return False
        return len(self.tos) > 0

    def __call__(self, ea, *args, **kwargs):
        if not self.tos:
            return True
        size = len(self.tos)
        idx = 0
        for xref in idaex.XrefsFromEx(ea, 0):
            if idaapi.get_member_by_id(xref.to) or idaapi.get_struc(xref.to):
                continue
            name = idaapi.netnode(xref.to).name()
            #只在自己的成员里搜索即可
            if name and self.FindToEa(name, idx):
                continue
            if idx >= size:
                self.errorCode = 0
                assert False, "OpEaFromXrefName之索引错误，idx:%d, size:%d, ea: %x " % (idx, size, ea)
                return False
            self.errorCode = 1
            self.errorIdx = idx
            if self.tos[idx](xref.to, xref, *args, **kwargs) == 0:
                return False
            idx += 1
        if idx != size:
            self.errorCode = 2
            assert False, "OpEaFromXrefName之xref信息个数不匹配，idx:%d, size:%d, ea: %x " % (idx, size, ea)
            return False
        self.errorCode = -1
        return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpEaFromXrefName之索引错误，可能是现在的xref数量大于原先的xref数量了，原先xref数量：", len(self.tos))
        elif self.errorCode == 1:
            uuts.LogDebug("OpEaFromXrefName之tos执行错误")
            self.tos[self.errorIdx].PrintDbgInfo()
        else:
            uuts.LogDebug("OpEaFromXrefName之xref数量不匹配，原先xref数量：", len(self.tos))


class OpDataFlags(idaex._OpBase):
    errorCode = -1

    def _InitDict(self, d):
        self.flag0 = uuts.BuildObj(d["flag0"])
        self.size = d["size"]
        self.duf = d["duf"]

    #DOUNK_SIMPLE有时会失败，DOUNK_DELNAMES能够解决这种失败的情况。当这段内存已有命名的时候，DOUNK_SIMPLE会失败。
    def _InitOrd(self, ea, doUknFlag = idc.DOUNK_SIMPLE):
        self.flag0 = idaex.FlagsOp0(ea, idc.GetFlags(ea))
        self.size = idc.ItemSize(ea)
        # self.size = idaex.ItemSizeEx(ea)
        self.duf = doUknFlag

    def _BuildDict(self, d):
        d["flag0"] = self.flag0
        d["size"] = self.size
        d["duf"] = self.duf

    def __call__(self, ea, *args, **kwargs):
        flag = self.flag0.flag & ~idc.MS_VAL
        #还是把下面给注释掉吧，因为有可能是结构体的情况，flag是相同的，而opinfo却是不同的。
        # if idc.GetFlags(ea) & ~idc.MS_VAL == flag and self.size == idc.ItemSize(ea):    #判断大小是防止是数组而且flag又相同的情况
        #     return True
        size = self.size
        if idaapi.isStruct(flag):
            nid = self.flag0.GetOpinfo().tid
            setOpinfo = False
        elif idc.isASCII(flag):
            size = idaapi.get_max_ascii_length(ea, self.flag0.GetOpinfo().strtype)
            assert size >= 0
            nid = idaapi.BADNODE
            setOpinfo = False
        else:
            nid = idaapi.BADNODE
            setOpinfo = True
        if idaapi.do_data_ex(ea, flag, size, nid) == 0:
            # 先取消定义再进行设置，否则地址范围内有不符合该数据格式的，do_data_ex就会出错
            idaapi.do_unknown_range(ea, size, self.duf)
            self.errorCode = 0
            if idaapi.do_data_ex(ea, flag, size, nid) == 0:
                assert False, "OpDataFlags之do_data_ex失败，ea: %x , flag: %x " % (ea, self.flag0.flag)
                return False
        self.errorCode = 1
        if setOpinfo and not self.flag0.SetOpinfo(ea):
            assert False, "_FlagsOpBase之SetOpInfo失败，地址： %x " % ea
            return False
        self.errorCode = 2
        if idc.GetFlags(ea) & ~idc.MS_VAL != flag:
            assert False, "OpDataFlags之设置flag失败，ea: %x , flag: %x " % (ea, self.flag0.flag)
            return False
        self.errorCode = -1
        return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        if self.errorCode == 0:
            uuts.LogDebug("OpDataFlags之do_data_ex失败")
        elif self.errorCode == 1:
            uuts.LogDebug("OpDataFlags之SetOpInfo失败")
            self.flag0.PrintDbgInfo()
        else:
            uuts.LogDebug("OpDataFlags之虽然成功设置了flag，但后面的验证flag却失败了")


class OpData(OpSetName):
    errorCode = -1

    def _InitDict(self, d):
        OpSetName._InitDict(self, d)
        self.flag = uuts.BuildObj(d["flag"])
        self.type = uuts.BuildObj(d["type"])
        self.cmt = uuts.BuildObj(d["cmt"])

    # DOUNK_SIMPLE有时会失败，DOUNK_DELNAMES能够解决这种失败的情况。当这段内存已有命名的时候，DOUNK_SIMPLE会失败。
    def _InitOrd(self, ea, doUknFlag = idc.DOUNK_SIMPLE):
        name = idaapi.netnode(ea).name()
        assert name, "OpData名字不能为空, ea: %x " % ea
        OpSetName._InitOrd(self, name, True)
        self.flag = OpDataFlags(ea, doUknFlag)
        tmp = OpEaType(ea)
        if tmp.Valid():
            self.type = tmp
        else:
            self.type = None
        tmp = OpEaCmt(ea)
        if tmp.Valid():
            self.cmt = tmp
        else:
            self.cmt = None

    def _BuildDict(self, d):
        OpSetName._BuildDict(self, d)
        d["flag"] = self.flag
        d["type"] = self.type
        d["cmt"] = self.cmt

    def __call__(self, ea, *args, **kwargs):
        self.errorCode = 0
        if OpSetName.__call__(self, ea, *args, **kwargs) == False:
            return False
        self.errorCode = 1
        if self.type and self.type(ea, *args, **kwargs) == False:
            return False
        self.errorCode = 2
        # cmt最好放到后面，因为有可能会运行cmt
        if self.cmt and self.cmt(ea, *args, **kwargs) == False:
            return False
        self.errorCode = 3
        #flag最好最后再设置，因为注释等会改变flag的。
        if self.flag(ea, *args, **kwargs) == False:
            return False
        self.errorCode = -1
        return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpData之设置名字失败")
            OpSetName.PrintDbgInfo(self)
        elif self.errorCode == 1:
            uuts.LogDebug("OpData之设置type失败")
            self.type.PrintDbgInfo()
        elif self.errorCode == 2:
            uuts.LogDebug("OpData之注释方面出了问题")
            self.cmt.PrintDbgInfo()
        else:
            uuts.LogDebug("OpData之设置flag失败")
            self.flag.PrintDbgInfo()


class OpCodeFlags(idaex._OpBase):
    errorCode = -1

    def _InitDict(self, d):
        self.flag0 = uuts.BuildObj(d["flag0"])
        self.flag1 = uuts.BuildObj(d["flag1"])
        self.type0 = d["type0"]
        self.type1 = d["type1"]

    def _InitOrd(self, ea):
        self.type0 = idc.GetOpType(ea, 0)
        if self.type0 == idc.o_void:
            self.flag0 = None
        else:
            self.flag0 = idaex.FlagsOp0(ea, idc.GetFlags(ea))
        self.type1 = idc.GetOpType(ea, 1)
        if self.type1 == idc.o_void:
            self.flag1 = None
        else:
            self.flag1 = idaex.FlagsOp1(ea, idc.GetFlags(ea))

    def _BuildDict(self, d):
        d["flag0"] = self.flag0
        d["flag1"] = self.flag1
        d["type0"] = self.type0
        d["type1"] = self.type1

    def Valid(self):
        return self.flag1 is not None or self.flag0 is not None

    #有时候操作数是o_mem（偏移），而有的时候是o_imm，所以当有注释的时候才允许添加格式信息，这也是为了减少更新信息
    def HasImme(self):
        return self.type0 == idc.o_imm or self.type1 == idc.o_imm

    #是否需要更新信息
    def NeedInfo(self):
        if self.type0 != idc.o_void:
            if self.type0 == idc.o_mem or self.type0 == idc.o_phrase or self.type0 == idc.o_displ or self.type0 == idc.o_imm:
                return True
        if self.type1 != idc.o_void:
            if self.type1 == idc.o_mem or self.type1 == idc.o_phrase or self.type1 == idc.o_displ or self.type1 == idc.o_imm:
                return True
        return False

    def __call__(self, ea, *args, **kwargs):
        self.errorCode = 0
        if self.flag0 and not self.flag0.SetFlag(ea, False):
            return False
        self.errorCode = 1
        if self.flag1 and not self.flag1.SetFlag(ea, False):
            return False
        if self.flag0:
            flag = self.flag0.flag
        elif self.flag1:
            flag = self.flag1.flag
        self.errorCode = 2
        if idc.GetFlags(ea) != flag:
            assert False, "OpCodeFlags之设置flag失败，ea: %x , flag: %x " %(ea, flag)
            return False
        self.errorCode = -1
        return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpCodeFlags之设置flag0失败")
            self.flag0.PrintDbgInfo()
        elif self.errorCode == 1:
            uuts.LogDebug("OpCodeFlags之设置flag1失败")
            self.flag1.PrintDbgInfo()
        else:
            uuts.LogDebug("OpCodeFlags之虽然成功设置了flag，但后面的校验却失败了")


class OpFnInfo(idaex._OpBase):
    errorCode = -1

    def _InitDict(self, d):
        self.size = d["size"]
        self.frsize = d["frsize"]
        self.frregs = d["frregs"]
        self.argsize = d["argsize"]
        self.fpd = d["fpd"]
        self.color = d["color"]
        self.flag = d["flag"]

    def _InitOrd(self, ea):
        end = idc.GetFunctionAttr(ea, idc.FUNCATTR_END)
        assert end != idc.BADADDR
        self.size = end - ea
        self.frsize = idc.GetFunctionAttr(ea, idc.FUNCATTR_FRSIZE)
        self.frregs = idc.GetFunctionAttr(ea, idc.FUNCATTR_FRREGS)
        self.argsize = idc.GetFunctionAttr(ea, idc.FUNCATTR_ARGSIZE)
        self.fpd = idc.GetFunctionAttr(ea, idc.FUNCATTR_FPD)
        self.color = idc.GetFunctionAttr(ea, idc.FUNCATTR_COLOR)
        self.flag = idc.GetFunctionFlags(ea)
        #FUNCATTR_OWNER  FUNCATTR_REFQTY这两个尚不知何用


    def _BuildDict(self, d):
        d["size"] = self.size
        d["frsize"] = self.frsize
        d["frregs"] = self.frregs
        d["argsize"] = self.argsize
        d["fpd"] = self.fpd
        d["color"] = self.color
        d["flag"] = self.flag

    def __call__(self, ea, *args, **kwargs):
        dst = OpFnInfo(ea)
        self.errorCode = 0
        if self.size != dst.size:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_END, ea + self.size):
                assert False, "设置函数结束地址失败，函数地址： %x ，函数大小：%d" % (ea, self.size)
                return False
        self.errorCode = 1
        if self.frsize != dst.frsize:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_FRSIZE, self.frsize):
                assert False, "设置函数frsize失败，函数地址： %x ，frsize：%d" % (ea, self.frsize)
                return False
        self.errorCode = 2
        if self.frregs != dst.frregs:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_FRREGS, self.frregs):
                assert False, "设置函数frregs失败，函数地址： %x ，frregs：%d" % (ea, self.frregs)
                return False
        self.errorCode = 3
        if self.argsize != dst.argsize:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_ARGSIZE, self.argsize):
                assert False, "设置函数argsize失败，函数地址： %x ，argsize：%d" % (ea, self.argsize)
                return False
        self.errorCode = 4
        if self.fpd != dst.fpd:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_FPD, self.fpd):
                assert False, "设置函数fpd失败，函数地址： %x ，fpd：%d" % (ea, self.fpd)
                return False
        self.errorCode = 5
        if self.color != dst.color:
            if 0 == idc.SetFunctionAttr(ea, idc.FUNCATTR_COLOR, self.color):
                assert False, "设置函数color失败，函数地址： %x ，color：%d" % (ea, self.color)
                return False
        self.errorCode = 6
        if self.flag != dst.flag:
            if 0 == idc.SetFunctionFlags(ea, self.flag):
                assert False, "设置函数flag失败，函数地址： %x ，flag：%d" % (ea, self.flag)
                return False
        self.errorCode = -1
        return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpFnInfo之函数结尾设置失败")
        elif self.errorCode == 1:
            uuts.LogDebug("OpFnInfo之frsize设置失败")
        elif self.errorCode == 2:
            uuts.LogDebug("OpFnInfo之frregs设置失败")
        elif self.errorCode == 3:
            uuts.LogDebug("OpFnInfo之argsize设置失败")
        elif self.errorCode == 4:
            uuts.LogDebug("OpFnInfo之fpd设置失败")
        elif self.errorCode == 5:
            uuts.LogDebug("OpFnInfo之color设置失败")
        elif self.errorCode == 1:
            uuts.LogDebug("OpFnInfo之flag设置失败")


class OpFrame(idaex._OpBase):
    errorCode = -1

    def _InitDict(self, d):
        self.s = uuts.BuildObj(d["s"])

    def _InitOrd(self, ea):
        fnPtr = idaapi.get_func(ea)
        assert fnPtr
        sPtr = idaapi.get_frame(fnPtr)
        if sPtr:
            self.s = idaex.StrucOp(sPtr.id)
        else:
            self.s = None

    def _BuildDict(self, d):
        d["s"] = self.s

    def __call__(self, ea, *args, **kwargs):
        fnPtr = idaapi.get_func(ea)
        assert fnPtr
        sPtr = idaapi.get_frame(fnPtr)
        if sPtr:
            if self.s:
                self.errorCode = 0
                dst = idaex.StrucOp(sPtr.id)
                if dst.size != self.s.size:
                    assert False, "地址： %x 的堆栈大小已变,原大小： %x ，现大小： %x "%(ea, self.s.size, dst.size)
                    return False
                self.errorCode = 1
                res = dst.Update(self.s)
                if res != 0:
                    self.errorCode = -1
                return res
            else:
                self.errorCode = 2
                assert False, "地址： %x ，原先没堆栈，现已有堆栈了" % ea
                return False
        else:
            if self.s:
                self.errorCode = 3
                assert False, "地址： %x ，原先有堆栈，现在没堆栈了" % ea
                return False
            else:
                self.errorCode = -1
                return True

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("OpFrame之StrucOp失败")
        elif self.errorCode == 1:
            uuts.LogDebug("OpFrame之StrucOp的更新操作失败")
        elif self.errorCode == 2:
            uuts.LogDebug("OpFrame之原先没堆栈，现已有堆栈了")
        else:
            uuts.LogDebug("OpFrame之原先有堆栈，现在没堆栈了")


class OpEaType(idaex.OpType):
    pass


class OpFn(OpSetName):
    errorCode = -1
    errorSub = None

    def _InitDict(self, d):
        OpSetName._InitDict(self, d)
        self.cmt = d["cmt"]
        self.rcmt = d["rcmt"]
        self.info = uuts.BuildObj(d["info"])
        self.frame = uuts.BuildObj(d["frame"])
        self.subs = [uuts.BuildObj(x) for x in d["subs"]]
        self.type = uuts.BuildObj(d["type"])

    def _InitOrd(self, eaFunc):
        OpSetName._InitOrd(self, idaapi.netnode(eaFunc).name(), True)
        self.cmt = idaex.GetFuncCmt(eaFunc, False)
        self.rcmt = idaex.GetFuncCmt(eaFunc, True)
        self.info = OpFnInfo(eaFunc)
        self.frame = OpFrame(eaFunc)
        self._InitSubs(eaFunc)
        self.type = OpEaType(eaFunc)

    def _InitSubs(self, eaFunc):
        self.subs = []
        for ea in idautils.FuncItems(eaFunc):
            eao = OpEaOffset(ea - eaFunc)
            cmt = OpEaCmt(ea)
            if cmt.Valid():
                #cmt最好添加在后面，因为有可能会运行cmt
                eao.Add(cmt)
                #flag最好在cmt后面，因为注释会影响flag的。
                flags = OpCodeFlags(ea)
                if flags.NeedInfo():
                    eao.Add(flags)  # 当有注释，才添加操作数信息
                    # print "OpCodeFlags,ea: %x " % ea
            #要放在OpCodeFlags后面添加才行
            xrefFrom = OpEaFromXrefName(ea, self)
            if xrefFrom.Valid():
                eao.Add(xrefFrom)

            if eao.Valid():
                self.subs.append(eao)

    def _BuildDict(self, d):
        OpSetName._BuildDict(self, d)
        d["cmt"] = self.cmt
        d["rcmt"] = self.rcmt
        d["info"] = self.info
        d["frame"] = self.frame
        d["subs"] = self.subs
        d["type"] = self.type

    def IsExistName(self, name):
        if OpSetName.IsExistName(self, name) == True:
            return True
        for sub in self.subs:
            if sub.IsExistName(name):
                return True
        return False

    def __call__(self, ea, *args, **kwargs):
        self.errorCode = 0
        res = OpSetName.__call__(self, ea, *args, **kwargs)
        if res == 0:
            return 0
        if self.cmt:
            self.errorCode = 1
            idc.SetFunctionCmt(ea, self.cmt, 0)
        if self.rcmt:
            self.errorCode = 2
            idc.SetFunctionCmt(ea, self.rcmt, 1)
        self.errorCode = 3
        if self.info(ea) == False:
            return 0
        self.errorCode = 4
        if self.type(ea, *args, **kwargs) == 0:
            return 0
        self.errorCode = 5
        if self.frame(ea) == False:
            return 0
        self.errorCode = 6
        #subs最好添加在最后，因为subs有可能包含cmt，而又有可能会运行cmt
        for sub in self.subs:
            self.errorSub = sub
            if sub(ea) == 0:
                return 0
        self.errorCode = -1
        return 1

    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        uuts.LogDebug("OpFn之函数中的某项操作失败，函数名：", self.name)
        if self.errorCode == 0:
            uuts.LogDebug("OpFn之函数名设置失败")
            OpSetName.PrintDbgInfo(self)
        elif self.errorCode == 1:
            uuts.LogDebug("OpFn之函数注释设置失败")
            self.cmt.PrintDbgInfo()
        elif self.errorCode == 2:
            uuts.LogDebug("OpFn之函数的可重复注释设置失败")
            self.rcmt.PrintDbgInfo()
        elif self.errorCode == 3:
            uuts.LogDebug("OpFn之函数的信息设置失败")
            self.info.PrintDbgInfo()
        elif self.errorCode == 4:
            uuts.LogDebug("OpFn之函数的类型设置失败")
            self.type.PrintDbgInfo()
        elif self.errorCode == 5:
            uuts.LogDebug("OpFn之函数的堆栈设置失败")
            self.frame.PrintDbgInfo()
        else:
            if self.errorSub:
                uuts.LogDebug("OpFn之函数的其他信息设置失败")
                self.errorSub.PrintDbgInfo()


class _DoUpdateBase(uuts.Serialize):
    """
    进行更新的操作
    DU : Do Update
    """
    def _InitDict(self, d):
        self.upl = d["upl"]

    def _InitOrd(self, upl):
        self.upl = upl

    def _BuildDict(self, d):
        d["upl"] = self.upl

    def SetUpl(self, upl):
        self.upl = upl

    def __cmp__(self, other):
        return cmp(self.upl, other.upl)

    def __call__(self):
        assert False


_errorFile_ = None
_continueErrorFile_ = None
def SetErrorObjFile(errorFileName, continueErrorFileName = None):
    """
    若continueErrorFileName为空，则把所有错误信息都输出到_errorFile_
    """
    global _errorFile_, _continueErrorFile_
    _errorFile_ = errorFileName
    _continueErrorFile_ = continueErrorFileName

class DU_Multi(uuts.MultiIterCall):
    def Add(self, item):
        if item is None:
            return
        if isinstance(item, DU_Multi):
            for i in item._fns_:
                self.Add(i)
        else:
            assert isinstance(item, _DoUpdateBase)
            self._fns_.append(item)

    def __call__(self):
        for item in self._fns_:
            if item() == idaex.DUR_Error:
                return idaex.DUR_Error
        return idaex.DUR_Succeed


class DU_Queue(DU_Multi):
    queue = None

    #greenlet.greenlet.run在执行以后，好像就被取消掉这个属性了
    class GletWapper(greenlet.greenlet):
        def __init__(self, run=None, parent=None):
            greenlet.greenlet.__init__(self, run, parent)
            self._MyRun_ = run

        def __cmp__(self, other):
            return cmp(self._MyRun_, other._MyRun_)

    def __call__(self):
        if not self.queue:
            self.queue = Queue.PriorityQueue()
        #clear
        while not self.queue.empty():
            self.queue.get_nowait()
        #add
        for item in self._fns_:
            self.queue.put_nowait(self.GletWapper(item))

        errors = DU_Queue()
        succeedCnt = 0
        eqCnt = 0
        while not self.queue.empty():
            queCnt = self.queue._qsize()
            glet = self.queue.get_nowait()
            gletItem = glet._MyRun_
            try:
                res = glet.switch()
                if res == idaex.DUR_Error:
                    uuts.LogError("DU_Queue  出错了一个")
                    errors.Add(gletItem)
                elif res == idaex.DUR_Succeed:
                    succeedCnt += 1
                    uuts.LogTrace("DU_Queue  成功了一个")
                elif res == idaex.DUR_Continue:
                    uuts.LogStackClr()
                    gletItem.SetUpl(UPL_Last)
                    self.queue.put_nowait(glet)
                    uuts.LogInfo("DU_Queue  继续了一个")
                else:
                    assert False
            except:
                uuts.LogError("DU_Queue捕获到异常\n", traceback.format_exc())
                errors.Add(gletItem)
            if queCnt == self.queue._qsize():
                eqCnt += 1
            else:
                eqCnt = 0
            if eqCnt > queCnt:
                uuts.LogError("队列数量老是没有变化,queue size:%d, equal cnt:%d" % (queCnt, eqCnt))
                break

        continus = None
        if not self.queue.empty():
            continus = DU_Queue()
            while not self.queue.empty():
                continus.Add(self.queue.get_nowait()._MyRun_)

        errorCnt = errors.ItemSize()
        conCnt = continus.ItemSize() if continus else 0
        if errorCnt == 0 and conCnt == 0:
            uuts.LogInfo("所有数据更新成功，要更新的总个数：%d，总成功个数：%d" % (self.ItemSize(), succeedCnt))
            return idaex.DUR_Succeed

        uuts.LogInfo("更新完毕，但有出错的，要更新的总个数：%d，总成功个数：%d" % (self.ItemSize(), succeedCnt))

        isAdded = False
        if errorCnt > 0:
            uuts.LogDebug("----------------以下是更新的出错详细信息----------------------")
            errors.PrintDbgInfo()
            if _errorFile_:
                uuts.LogInfo("输出错误信息到这个文件中：", _errorFile_)
                if not _continueErrorFile_:
                    isAdded = True
                    errors.Add(continus)
                errors.WriteToFile(_errorFile_)
            uuts.LogDebug("----------------更新的出错详细信息结束----------------------")
        if conCnt > 0:
            uuts.LogDebug("----------------以下是循环引用出错的详细信息----------------------")
            continus.PrintDbgInfo()
            if isAdded == False:
                if _continueErrorFile_:
                    errorFile = _continueErrorFile_
                else:
                    errorFile = _errorFile_
                if errorFile:
                    uuts.LogInfo("输出循环引用错误信息到这个文件中：", errorFile)
                    continus.WriteToFile(errorFile)
            uuts.LogDebug("----------------循环引用出错的详细信息结束----------------------")
        return idaex.DUR_Error


#枚举常量
class DU_SetEnums(_DoUpdateBase):
    errorE = None

    def _InitOrd(self, upl = UPL_Enum):
        _DoUpdateBase._InitOrd(self, upl)
        self.enums = [idaex.EnumOp(x) for x in idaex.EnumMgr().Enums()]

    def _InitDict(self, d):
        _DoUpdateBase._InitDict(self, d)
        self.enums = [uuts.BuildObj(x) for x in d["enums"]]

    def _BuildDict(self, d):
        _DoUpdateBase._BuildDict(self, d)
        d["enums"] = self.enums

    @uuts.LogStack
    def __call__(self):
        uuts.LogTrace("开始更新枚举")
        for e in self.enums:
            self.errorE = e
            if not e.AddEnum():
                assert False, "DU_SetEnums添加%s枚举失败" % e.name
                return idaex.DUR_Error
        self.errorE = None
        uuts.LogTrace("枚举类型更新成功")
        return idaex.DUR_Succeed

    @uuts.LogStack
    def PrintDbgInfo(self):
        if not self.errorE:
            return
        uuts.LogDebug("枚举有失败的，请看上面的详细信息")
        # uuts.LogDebug("#--------------枚举DbgInfo begin--------------------------")
        # self.errorE.PrintDbgInfo()
        # uuts.LogDebug("#--------------枚举DbgInfo end--------------------------")


#结构体
class DU_SetStructs(_DoUpdateBase):
    errorCode = -1
    errorS = None

    def _InitOrd(self, upl = UPL_Struct):
        _DoUpdateBase._InitOrd(self, upl)
        self.structs = [idaex.StrucOp(x[1]) for x in idautils.Structs()]

    def _InitDict(self, d):
        _DoUpdateBase._InitDict(self, d)
        self.structs = [idaex.StrucOp(x) for x in d["structs"]]

    def _BuildDict(self, d):
        _DoUpdateBase._BuildDict(self, d)
        d["structs"] = self.structs

    @uuts.LogStack
    def __call__(self):
        uuts.LogTrace("开始更新结构体")
        self.errorCode = 0
        for s in self.structs:
            self.errorS = s
            if not s.AddStruct(False):
                assert False, "DU_SetStructs添加%s结构体失败" % s.name
                return idaex.DUR_Error

        self.errorCode = 1
        for s in self.structs:
            self.errorS = s
            if not s.AddMembers():
                assert False, "DU_SetStructs添加%s结构体成员失败" % s.name
                return idaex.DUR_Error
        self.errorCode = -1
        uuts.LogTrace("结构体更新成功")
        return idaex.DUR_Succeed

    @uuts.LogStack
    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        uuts.LogDebug("结构体有失败的，请看上面的详细信息")
        # uuts.LogDebug("#--------------结构体DbgInfo begin--------------------------")
        # if self.errorCode == 0:
        #     uuts.LogDebug("添加结构体失败")
        # else:
        #     uuts.LogDebug("添加结构体成员失败")
        # self.errorE.PrintDbgInfo()
        # uuts.LogDebug("#--------------结构体DbgInfo end--------------------------")


#设置指令地址，还未搞定
class DU_SetChunk(_DoUpdateBase):
    def _InitOrd(self, upl = UPL_Chunk):
        _DoUpdateBase._InitOrd(self, upl)

    def _InitDict(self, d):
        _DoUpdateBase._InitDict(self, d)

    def _BuildDict(self, d):
        _DoUpdateBase._BuildDict(self, d)


#设置函数
class DU_SetFunc(_DoUpdateBase):
    errorCode = -1

    def _InitOrd(self, finder, toDo, upl = UPL_Func):
        _DoUpdateBase._InitOrd(self, upl)
        assert isinstance(finder, Finder)
        assert isinstance(toDo, (_OpMultiBase, idaex._OpBase))
        self.finder = finder
        self.toDo = toDo

    def _InitDict(self, d):
        _DoUpdateBase._InitDict(self, d)
        self.finder = uuts.BuildObj(d["finder"])
        self.toDo = uuts.BuildObj(d["toDo"])

    def _BuildDict(self, d):
        _DoUpdateBase._BuildDict(self, d)
        d["finder"] = self.finder
        d["toDo"] = self.toDo

    def _DoCall(self):
        self.errorCode = 0
        addr = self.finder.Do()
        if addr == idc.BADADDR:
            return idaex.DUR_Error
        self.errorCode = 1
        if self.toDo(addr) == 0:
            return idaex.DUR_Error
        self.errorCode = -1
        return idaex.DUR_Succeed

    @uuts.LogStack
    def __call__(self):
        if hasattr(self.toDo, "name"):
            name = self.toDo.name
        else:
            name = None
        if name is not None:
            uuts.LogTrace("开始更新这个函数：", name)
        else:
            uuts.LogTrace("开始更新一个函数")
        res = self._DoCall()
        if idaex.DUR_Succeed == res:
            if name is not None:
                uuts.LogTrace("这个函数更新成功：", name)
            else:
                uuts.LogTrace("那个函数更新成功")
        else:
            if name is not None:
                uuts.LogTrace("这个函数更新失败：", name)
            else:
                uuts.LogTrace("那个函数更新失败")
        return res

    @uuts.LogStack
    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("DU_SetFunc之finder失败")
            self.finder.PrintDbgInfo()
        else:
            uuts.LogDebug("DU_SetFunc之toDo失败")
            self.toDo.PrintDbgInfo()
            uuts.LogDebug("DU_SetFunc之虽然toDo失败了，但有可能是finder造成的，下面查看一下finder失败的信息")
            self.finder.PrintDbgInfo()


class DU_SetGloData(DU_SetFunc):
    """
    全局变量，包括字符串常量、虚函数表
    """
    def _InitOrd(self, finder, toDo, upl=UPL_GloData):
        DU_SetFunc._InitOrd(self, finder, toDo, upl)

    @uuts.LogStack
    def __call__(self):
        if hasattr(self.toDo, "name"):
            name = self.toDo.name
        else:
            name = None
        if name is not None:
            uuts.LogTrace("开始更新这个全局变量：", name)
        else:
            uuts.LogTrace("开始更新一个全局变量")
        res = self._DoCall()
        if idaex.DUR_Succeed == res:
            if name is not None:
                uuts.LogTrace("这个全局变量更新成功：", name)
            else:
                uuts.LogTrace("那个全局变量更新成功")
        else:
            if name is not None:
                uuts.LogTrace("这个全局变量更新失败：", name)
            else:
                uuts.LogTrace("那个函数更新失败")
        return res

    @uuts.LogStack
    def PrintDbgInfo(self):
        if self.errorCode == -1:
            return
        elif self.errorCode == 0:
            uuts.LogDebug("DU_SetGloData之finder失败")
            self.finder.PrintDbgInfo()
        else:
            uuts.LogDebug("DU_SetGloData之toDo失败")
            self.toDo.PrintDbgInfo()
            uuts.LogDebug("DU_SetGloData之虽然toDo失败了，但有可能是finder造成的，下面查看一下finder失败的信息")
            self.finder.PrintDbgInfo()


#根据头文件中的结构体声明顺序来调整结构体的顺序
# def SetStrucOrderBy
# def SetEnumOrderBy
# def UpdateStructs(self):    #断言结构体的变化
# def UpdateEnums(self):      #断言常量的变化
# def XrefEnums(self):
# def XrefStructs(self):
# def XrefOthers(self):
# def Check         #校验所有名字、xref等信息


#管理环境对象的
@uuts.singleton
class EnvObjMgr:
    #name list凡是以此为前缀的都会更新
    sysPrefixes = ("@",)
    #name list中指定名字（也可以是名字中的一部分）的会进行更新
    subNames = []

    def __init__(self, clsEnvGen = None, clsEnvRun = None):
        class Wrapper:
            def __init__(self, env):
                self.self = env

        if not clsEnvGen:
            clsEnvGen = EaEnvGen
        if not clsEnvRun:
            clsEnvRun = EaEnvRun
        self.genEnv = clsEnvGen()
        self.genExec = uuts.ExecEnv(Wrapper(self.genEnv), None, uuts.DefGlobal())
        self.runEnv = clsEnvRun()
        self.runExec = uuts.ExecEnv(Wrapper(self.runEnv), None, uuts.DefGlobal())

    def GenByEa(self, ea):
        return DU_Queue(self._GenByEa(ea))

    def GenBySubName(self, subName):
        for ea, name in idautils.Names():
            if name.find(subName) >= 0:
                return self.GenByEa(ea)

    def GenAll(self, addStructEnum = True):
        que = DU_Queue()
        for ea, name in idautils.Names():
            if name.startswith(self.sysPrefixes):
                que.Add(self._GenByEa(ea))
            else:
                for i in self.subNames:
                    if name.find(i) >= 0:
                        que.Add(self._GenByEa(ea))
        if addStructEnum:
            que.Add(DU_SetEnums())
            que.Add(DU_SetStructs())
        uuts.LogTrace("生了%d个Do Update对象" % que.ItemSize())
        return que

    def GenAllStructEnum(self):
        que = DU_Queue()
        que.Add(DU_SetEnums())
        que.Add(DU_SetStructs())
        return que

    def RunByEa(self, ea, cmt = None):
        if cmt is None:
            cmt = idaex.GetLineCmt(ea, False)
        if not cmt:
            assert False, "根本没LineB注释，地址： %x " % ea
            return
        self.runEnv.curEa = ea
        return self.runExec(cmt)

    def RunBySubName(self, subName):
        for ea, name in idautils.Names():
            if name.find(subName) >= 0:
                return self.RunByEa(ea)

    def _GenByEa(self, ea):
        flag = idc.GetFlags(ea)
        cmt = idaex.GetLineCmt(ea, True)
        if not cmt:
            if idc.isCode(flag):
                assert False, "没有LineA，无法执行生成Do Update对象，ea: %x " % ea
            else:
                return

        self.genEnv.curEa = ea
        self.genEnv._Reset()
        try:
            self.genExec(cmt)
        except:
            uuts.LogError("执行LineA失败，地址：  %x  ，代码：\n%s\n以下是堆栈输出：\n" % (ea, cmt), traceback.format_exc())
            return

        assert self.genEnv.curLoc is not None, "执行LineA代码后，没有得到生成器对象，是不是忘记民添加了？name:%s, ea: %x " % (idaapi.netnode(ea).name(), ea)
        assert self.genEnv.curOp is not None, "执行LineA代码后，没有得到要操作的对象，是不是忘记民添加了？name:%s, ea: %x " % (idaapi.netnode(ea).name(), ea)
        finder = Finder(self.genEnv.curLoc, self.genEnv.curPa, self.genEnv.curTr, self.genEnv.curAa)
        if idc.isCode(flag):
            if idaapi.isFunc(flag):
                return DU_SetFunc(finder, self.genEnv.curOp)
            else:
                assert False, "该地址不是函数首地址，可能是chunk之类的，目前不支持，地址： %x " % ea
        else:
            return DU_SetGloData(finder, self.genEnv.curOp)
