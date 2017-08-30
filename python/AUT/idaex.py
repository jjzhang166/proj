#coding=gbk
"""
ida extention
"""
import idaapi
import idc
import idautils
import itertools
import uuts
import ctypes
import win32api
import os

#--------------------------------------------------------------------------------
#----------------------------------常量定义----------------------------------------------
#DUR : Do Update Result
DUR_Error = 0
DUR_Succeed = 1
DUR_Continue = 2
#--------------------------------------------------------------------------------
#----------------------------------_idaex----------------------------------------------
_idaex = ctypes.cdll.LoadLibrary(os.path.join(uuts.CurFileDir(), "_idaex.pyd"))

def _FreeLibIdaex():
    try:
        while True:
            m = win32api.GetModuleHandle("_idaex.pyd")
            if not m:
                break
            win32api.FreeLibrary(m)
    except Exception, e:
        print e
        pass


def GetNetnodeId(name):
    return ctypes.c_uint(_idaex.GetNetnodeId(name)).value

def GetStringEx(ea, length = -1):
    type = idc.GetStringType(ea)
    if type is None:
        return
    return idc.GetString(ea, length, type)
#---------------------------------------------------------------------------------------------
##-----------------------------------IDC脚本中有，python中没有的东西--------------------------
FF_3BYTE    = idaapi.FF_3BYTE & 0xFFFFFFFF         #// 3-byte data
FF_CUSTOM   = idaapi.FF_CUSTOM         #// custom data type
FF_YWRD     = idaapi.FF_YWRD             #// ymm word (32 bytes/256 bits)

STRUC_ERROR_MEMBER_OK = idaapi.STRUC_ERROR_MEMBER_OK

def is3byte(F):    return (idc.isData(F) and (F & idc.DT_TYPE) == FF_3BYTE)
def isCustom(F):    return (idc.isData(F) and (F & idc.DT_TYPE) == FF_CUSTOM)

def isYwrd(F):     return (idc.isData(F) and (F & idc.DT_TYPE) == FF_YWRD)
#---------------------------------------------------------------------------------------------
#-----------------------------------------自定义常量------------------------------------
MAXULONG = ctypes.c_ulong(-1).value
#---------------------------------------------------------------------------------------------
E_PREV = idaapi.E_PREV
E_NEXT = idaapi.E_NEXT
def GetLineCnt(ea, lineA = True):
    if lineA:
        idx = E_PREV
    else:
        idx = E_NEXT
    res = idaapi.get_first_free_extra_cmtidx(ea, idx)
    if res == 0:
        assert False
        return
    return res - idx

def GetLineCmt(ea, lineA = True):
    cnt = GetLineCnt(ea, lineA)
    if cnt <= 0:
        return
    str = ""
    if lineA:
        idx = E_PREV
    else:
        idx = E_NEXT
    # last = cnt - 1
    for i in xrange(cnt):
        res = idaapi.get_extra_cmt(ea, idx + i)
        if res:
            str += res
        # if last == i:
        #     continue
        str += "\n"
    return str

def SetLineCmt(ea, str, lineA = True):
    if lineA:
        idx = E_PREV
    else:
        idx = E_NEXT
    cnt = 0
    for line in str.splitlines(True):
        c = line.find('\n')
        if c >= 0:
            line = line[0:c]
        idaapi.update_extra_cmt(ea, idx + cnt, line)
        cnt += 1
    maxIdx = idaapi.get_first_free_extra_cmtidx(ea, idx)
    leftCnt = maxIdx - idx - cnt
    if leftCnt > 0:
        for i in xrange(1, leftCnt + 1):
            idaapi.del_extra_cmt(ea, maxIdx - i)
    idaapi.doExtra(ea)


def GetFuncCmt(ea, repeatable):
    func = idaapi.get_func(ea)
    if not func:
        return
    else:
        return idaapi.get_func_cmt(func, repeatable)


#这个函数好像没必要了，因为不管是结构体还是其他，计算出来的结果与idc.ItemSize一样的。
# def ItemSizeEx(ea):
#     flags = idaapi.getFlags(ea)
#     if idaapi.isStruct(flags):
#         ti = idaapi.opinfo_t()
#         assert idaapi.get_opinfo(ea, 0, flags, ti), "ItemSizeEx.get_opinfo() failed"
#         return idaapi.get_data_elsize(ea, flags, ti)
#     else:
#         return idaapi.get_item_size(ea)


def IsFunctionStart(ea):
    fn = idaapi.get_func(ea)
    if fn is None:
        return False
    return fn.startEA == ea


def SubItems(start, end):
    start = idc.ItemHead(start)
    while start < end and start != idc.BADADDR:
        yield start
        start = idc.NextHead(start, end)


def FuncSubItems(start, end = idc.BADADDR):
    """
    Get a list of function sub items

    @param start: address of the function

    @return: ea of each item in the function
    """
    func = idaapi.get_func(start)
    if not func:
        return
    if end == idc.BADADDR or end > func.endEA:
        end = func.endEA
    return SubItems(start, end)


def GenBinary(eaBegin, eaEnd):
    """
    firt byte of every instruction, other part of instruction, use double '?'
    """
    eaBegin = idc.ItemHead(eaBegin)
    strResult = ""
    while eaBegin < eaEnd:
        itemSize = idc.ItemSize(eaBegin)
        if itemSize <= 0:
            assert False
            return ""
        strResult += " %x " % idc.Byte(eaBegin)
        eaBegin = idc.NextHead(eaBegin, eaEnd)
        if eaBegin != idc.BADADDR:
            strResult += ' '
            strResult += '?? ' * (itemSize - 1)
    return strResult


def GenMnem(eaBegin, eaEnd):
    """
    generate operand code names
    """
    eaBegin = idc.ItemHead(eaBegin)
    strResult = []
    while eaBegin < eaEnd and eaBegin != idc.BADADDR:
        strResult.append(idc.GetMnem(eaBegin))
        eaBegin = idc.NextHead(eaBegin, eaEnd)
    return strResult


def FindTheBinary(str, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW):
    """
    return ea or idc.BADADDR
    """
    return idaapi.find_binary(eaBegin, eaEnd, str, 16, flag)


def FindMnem(mnem, eaBegin, eaEnd=idc.BADADDR):
    """
    return ea or idc.BADADDR
    """
    eaBegin = idc.ItemHead(eaBegin)
    while eaBegin < eaEnd and eaBegin != idc.BADADDR:
        tmp = eaBegin
        for i in mnem:
            if tmp == idc.BADADDR:
                return idc.BADADDR
            if idc.GetMnem(tmp) != i:
                break
            tmp = idc.NextHead(tmp, eaEnd)
        else:
            return eaBegin
        eaBegin = idc.NextHead(eaBegin, eaEnd)
    return idc.BADADDR


def FindTheText(str, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW | idc.SEARCH_REGEX):
    """
    return ea or idc.BADADDR
    """
    res = idc.FindText(eaBegin, flag, 0, 0, str)
    if res == idc.BADADDR or res >= eaEnd or res < eaBegin:
        return idc.BADADDR
    return res


def FindImme(value, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW):
    """
    return ea or idc.BADADDR
    """
    res = idc.FindImmediate(eaBegin, flag, value)
    if res == idc.BADADDR or res[0] >= eaEnd or res[0] < eaBegin:
        return idc.BADADDR
    return res[0]


def IterBinary(str, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW):
    """
    iterator
    """
    # print "IterBinary  %x ,  %x  , str:%s" % (eaBegin, eaEnd, str)
    res = eaBegin
    while res != idc.BADADDR:
        res = FindTheBinary(str, res, eaEnd, flag)
        if res == idc.BADADDR:
            break
        yield res
        res = idc.NextAddr(res)


def IterMnem(mnem, eaBegin, eaEnd=idc.BADADDR):
    """
    iterator
    """
    res = eaBegin
    while res != idc.BADADDR:
        res = FindMnem(mnem, res, eaEnd)
        if res == idc.BADADDR:
            break
        yield res
        res = idc.NextHead(res, eaEnd)


def IterText(str, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW | idc.SEARCH_REGEX):
    """
    iterator
    """
    while eaBegin != idc.BADADDR:
        eaBegin = FindTheText(str, eaBegin, eaEnd, flag)
        if eaBegin == idc.BADADDR:
            break
        yield eaBegin
        eaBegin = idc.NextHead(eaBegin, eaEnd)


def IterImme(value, eaBegin, eaEnd=idc.BADADDR, flag=idc.SEARCH_DOWN | idc.SEARCH_NOSHOW):
    """
    iterator
    """
    while eaBegin != idc.BADADDR:
        eaBegin = FindImme(value, eaBegin, eaEnd, flag)
        if eaBegin == idc.BADADDR:
            break
        yield eaBegin
        eaBegin = idc.NextHead(eaBegin, eaEnd)


def IterFirstInsn(eaBegin, eaEnd):
    eaBegin = idc.ItemHead(eaBegin)
    while eaBegin < eaEnd and eaBegin != idc.BADADDR:
        yield idc.Byte(eaBegin)
        eaBegin = idc.NextHead(eaBegin, eaEnd)


def RelativeInsnOffset(insnDelta, ea, eaBound=idc.BADADDR):
    """
    relative instruction and offset258
    return (ea, offset), error:(idc.BADADDR, ?)
    """
    offset = 0
    if insnDelta < 0:
        eaBound = 0 if eaBound == idc.BADADDR else eaBound
        while insnDelta < 0 and ea != idc.BADADDR:
            offset -= idc.ItemSize(ea)
            ea = idc.PrevHead(ea, eaBound)
            insnDelta += 1
    else:
        while insnDelta > 0 and ea != idc.BADADDR:
            offset += idc.ItemSize(ea)
            ea = idc.NextHead(ea, eaBound)
            insnDelta -= 1
    return ea, offset


def IsSameInsnOffset(offsetDelta, insnDelta, ea, eaBound=idc.BADADDR):
    """
    相对ea的偏移与指令个数相同与否
    """
    if insnDelta < 0:
        eaBound = 0 if eaBound == idc.BADADDR else eaBound
        while insnDelta < 0 and ea != idc.BADADDR:
            offsetDelta += idc.ItemSize(ea)
            if offsetDelta > 0:
                return False
            ea = idc.PrevHead(ea, eaBound)
            insnDelta += 1
    else:
        while insnDelta > 0 and ea != idc.BADADDR:
            offsetDelta -= idc.ItemSize(ea)
            if offsetDelta < 0:
                return False
            ea = idc.NextHead(ea, eaBound)
            insnDelta -= 1
    return offsetDelta == 0 and insnDelta == 0


def InsnAndSize(eaBegin, eaEnd):
    """
    size of function
    return (instruction count, byte count)
    """
    eaBegin = idc.ItemHead(eaBegin)
    if eaBegin >= eaEnd:
        assert False
        return
    inCnt = 0
    byCnt = 0
    while eaBegin != idc.BADADDR:
        inCnt += 1
        byCnt += idc.ItemSize(eaBegin)
        eaBegin = idc.NextHead(eaBegin, eaEnd)
    return inCnt, byCnt


def FuncInsnAndSize(eaFunc, eaEnd = idc.BADADDR):
    fn = idaapi.get_func(eaFunc)
    if not fn:
        assert False
        return
    if eaEnd == idc.BADADDR:
        eaEnd = fn.endEA
    return InsnAndSize(eaFunc, eaEnd)


def CheckSum(data):
    ''''
    按字节计算校验和。每个字节被翻译为无符号整数
    @param data: 字节串
    @param byteorder: 大/小端
    '''
    checksum = 0 & 0xffffffff
    for i in data:
        checksum += (i & 0xffffffff)
        checksum &= 0xFFFFFFFF  # 强制截断
    return checksum


def InsnCheckSum(eaBegin, eaEnd):
    return CheckSum(IterFirstInsn(eaBegin, eaEnd))


def FuncCheckSum(eaFunc, eaEnd = idc.BADADDR):
    fn = idaapi.get_func(eaFunc)
    if not fn:
        assert False
        return
    if eaEnd == idc.BADADDR:
        eaEnd = fn.endEA
    return InsnCheckSum(eaFunc, eaEnd)


def InsnFullCheck(eaBegin, eaEnd):
    """
    return (instruction count, byte count, check sum)
    """
    eaBegin = idc.ItemHead(eaBegin)
    if eaBegin >= eaEnd:
        assert False
        return
    inCnt = 0
    byCnt = 0
    checkSum = 0 & 0xffffffff
    while eaBegin != idc.BADADDR:
        inCnt += 1
        byCnt += idc.ItemSize(eaBegin)
        checkSum += (idc.Byte(eaBegin) & 0xffffffff)
        checkSum &= 0xFFFFFFFF  # 强制截断
        eaBegin = idc.NextHead(eaBegin, eaEnd)
    return inCnt, byCnt, checkSum


def FuncFullCheck(eaFunc, eaEnd = idc.BADADDR):
    fn = idaapi.get_func(eaFunc)
    if not fn:
        # assert False, " %x  不是函数地址"%eaFunc
        return
    if eaEnd == idc.BADADDR:
        eaEnd = fn.endEA
    res = InsnFullCheck(eaFunc, eaEnd)
    # print res
    return res


def FuncCallsTo(eaFunc, fnCaller=True):
    """
    fnCaller : caller must be function
    """
    # get begin of function
    fn = idaapi.get_func(eaFunc)
    if fn is None:
        assert False
        return
    eaFunc = fn.startEA
    if fnCaller:
        return itertools.ifilter(lambda x: idaapi.get_func(x) is not None, idautils.CodeRefsTo(eaFunc, 0))
    else:
        return idautils.CodeRefsTo(eaFunc, 0)


def FuncCallsFrom(eaFunc, fnCalled=False):
    """
    fnCalled : called must be function
    return : (srcEA, dstEA)
    """
    # get begin of function
    fn = idaapi.get_func(eaFunc)
    if not fn:
        assert False
        return
    fnStart = fn.startEA
    for frm in FuncSubItems(eaFunc):
        for to in idautils.CodeRefsFrom(frm, 0):
            fn = idaapi.get_func(to)
            if fn and fn.startEA == fnStart:
                continue
            if fnCalled:
                if fn:
                    yield (frm, to)
            else:
                yield (frm, to)


def FindNameOfFuncCallsFrom(subName, eaFunc, fnCalled = False):
    for frm, to in  FuncCallsFrom(eaFunc, fnCalled):
        if idaapi.netnode(to).name().find(subName) >= 0:
            return frm, to


def TextFrom(eaBegin, eaEnd):
    """
     return : (srcEA, dstEA)
    """
    for i in SubItems(eaBegin, eaEnd):
        for x in idautils.DataRefsFrom(i):
            if idc.isASCII(idc.GetFlags(x)):
                yield (i, x)


def FuncTextFrom(eaFunc):
    """
     return : (srcEA, dstEA)
    """
    for i in FuncSubItems(eaFunc):
        for x in idautils.DataRefsFrom(i):
            if idc.isASCII(idc.GetFlags(x)):
                yield (i, x)


def FindTextOfFuncTextFrom(text, eaFunc):
    for frm, to in FuncTextFrom(eaFunc):
        if GetStringEx(to).find(text) >= 0:
            return frm, to


def FindNameOfFuncDrefFrom(subName, eaFunc):
    for frm in FuncSubItems(eaFunc):
        for to in idautils.DataRefsFrom(frm):
            name = idaapi.netnode(to).name()
            if name and name.find(subName) >= 0:
                return frm, to

def XrefsFilterFrom(filter, ea, flags=0):
    xref = idaapi.xrefblk_t()
    if xref.first_from(ea, flags):
        if filter(xref):
            yield idautils._copy_xref(xref)
        while xref.next_from():
            if filter(xref):
                yield idautils._copy_xref(xref)


def XrefsFilterTo(filter, ea, flags=0):
    xref = idaapi.xrefblk_t()
    if xref.first_to(ea, flags):
        if filter(xref):
            yield idautils._copy_xref(xref)
        while xref.next_to():
            if filter(xref):
                yield idautils._copy_xref(xref)


def FilterXrefType(*types):
    return lambda xref: xref.type in types


def FilterXrefData():
    return FilterXrefType(0, 1, 2, 3, 4, 5)


fl_F = idaapi.fl_F
#flags ==0, flags == idaapi.XREF_ALL
def XrefsFromEx(ea, flags = 0):
    global fl_F
    xref = idaapi.xrefblk_t()
    if xref.first_from(ea, flags):
        if xref.type != fl_F:
            yield xref
        while xref.next_from():
            if xref.type != fl_F:
                yield xref


#flags ==0, flags == idaapi.XREF_ALL
def XrefsToEx(ea, flags = 0):
    global fl_F
    xref = idaapi.xrefblk_t()
    if xref.first_to(ea, flags):
        if xref.type != fl_F:
            yield xref
        while xref.next_to():
            if xref.type != fl_F:
                yield xref


def FindAnyFnXrefFrom(eaFunc, startswidth = None):
    fn = idaapi.get_func(eaFunc)
    if not fn:
        assert False
        return
    fnStart = fn.startEA
    for frm in FuncSubItems(eaFunc):
        for xref in XrefsFromEx(frm, 0):
            fn = idaapi.get_func(xref.to)
            if fn and fn.startEA == fnStart:
                continue
            #for test
            # assert frm == xref.frm
            #必须得有名字
            name = idaapi.netnode(xref.to).name()
            if not name:
                continue
            if startswidth:
                if name.startswith(startswidth):
                    return xref
            else:
                return xref


class ImportFns:
    """
    fi : function info, [0]->ea, [1]->function name
    fis : function infos, [fi0, fi1, fi2, ...]
    mi : module info, [module_name, fis]
    mis : module infos,[mi0, mi1, mi2, ...]
    """
    mis = [];
    def __init__(self):
        qty = idaapi.get_import_module_qty()
        assert qty >= 1, "must >= 1"
        for i in xrange(qty):
            self.mis.append([idaapi.get_import_module_name(i), self._GenFis(i)])

    def GetModuleQty(self):
        """
        Get number of import modules
        """
        return len(self.mis)

    def GetFis(self, module_name):
        #get function infos
        for i in self.mis:
            if i[0] == module_name:
                return i[1]

    def GetFunctionAddr(self, funcName):
        for m in self.mis:
            for i in m[1]:
                if (i[1] == funcName):
                    return i[0], m[0]

    def GetFunctionName(self, ea):
        for m in self.mis:
            for i in m[1]:
                if (i[0] == ea):
                    return i[1], m[0]

    def _GenFis(self, moduleIndex):
        """
        Generate imports from specific module.
        """
        fis = []
        def CallBack(ea, name, ord):
            """
            ea     import address
            name   import name (NULL if imported by ordinal)
            ord    import ordinal (0 for imports by name)
            """
            fis.append([ea, name])
            return 1
        assert 1 == idaapi.enum_import_names(moduleIndex, CallBack)
        return fis


def MakeNameStrict(ea, name, globalName = True):
    flag = idc.SN_CHECK | idc.SN_NOWARN
    if not globalName:
        flag |= idc.SN_LOCAL
    res = idc.MakeNameEx(ea, name, flag)
    assert 1 == res, "MakeNameStrict失败，ea: %x ,name:%s, flag: %x " %(ea, name, flag)
    return res


#ordinary
class EaRange(uuts.Serialize):
    eaBegin = idc.BADADDR
    eaEnd = idc.BADADDR

    def _InitDict(self, d):
        self.eaBegin = d["begin"]
        self.eaEnd = d["end"]

    def _InitOrd(self, begin = None, end = None):
        if begin is not None:
            self.eaBegin = begin
        if end is not None:
            self.eaEnd = end

    def _BuildDict(self, d):
        d["begin"] = self.eaBegin
        d["end"] = self.eaEnd

    def Size(self):
        return self.eaEnd - self.eaBegin

    def Offset(self, offset, insnAlign = False):
        ea = self.eaBegin + offset
        if ea < self.eaBegin or ea >= self.eaEnd:
            return idc.BADADDR
        if insnAlign:
            ea = idc.ItemHead(ea)
        return ea

    def SubRange(self, offsetBegin, offsetSize = idc.BADADDR, insnAlign = True):
        ea = self.Offset(offsetBegin, insnAlign)
        if ea == idc.BADADDR:
            assert False, "EaRange.SubRange之offsetBegin无效"
            return
        size = self.eaEnd - ea
        if size > offsetSize:
            size = offsetSize
        return EaRange(ea, ea + size)


#by segment name
class EaRangeSegName(EaRange):
    def _InitDict(self, d):
        self._InitOrd(d["segname"], d["occurCnt"])

    def _InitOrd(self, segname, occurCnt = 1):
        res = SegMgr().FindSegByName(segname, occurCnt)
        if not res:
            assert False, "EaRangeSegName之没有这样的段名：%s,occurCnt:%d" % (segname, occurCnt)
            return
        self.segname = segname
        self.occurCnt = occurCnt
        EaRange._InitOrd(self, res[0].startEA, res[0].endEA)

    def _BuildDict(self, d):
        EaRange._BuildDict(self, d)
        d["segname"] = self.segname
        d["occurCnt"] = self.occurCnt

    def PrintDbgInfo(self):
        uuts.LogDebug("EaRangeSegName  segname:", self.segname)


class EaRangeFn(EaRange):
    def _InitDict(self, d):
        self.fnName = d["fnName"]
        ea = NamesMgr().DelayGetNetnodeId(self.fnName)
        assert ea != idc.BADADDR, "EaRangeFn之不存在这个函数，地址： %x , 函数名：%s" % (ea, self.fnName)
        EaRange._InitOrd(self, ea)

    def _InitOrd(self, eaFunc):
        fn = idaapi.get_func(eaFunc)
        if not fn:
            return
        EaRange._InitOrd(self, fn.startEA, fn.endEA)
        self.fnName = idc.GetFunctionName(self.eaBegin)

    def _BuildDict(self, d):
        EaRange._BuildDict(self, d)
        d["fnName"] = self.fnName

    def PrintDbgInfo(self):
        uuts.LogDebug("EaRangeFn  fnName:", self.fnName)


@uuts.singleton
class SegMgr:
    def IterSegBy(self, fn):
        for n in xrange(idaapi.get_segm_qty()):
            seg = idaapi.getnseg(n)
            if seg:
                if fn(seg, n):
                    return True
        return False

    def IterSegByType(self, fn, type):
        for n in xrange(idaapi.get_segm_qty()):
            seg = idaapi.getnseg(n)
            if seg:
                if type == idc.GetSegmentAttr(seg.startEA, idc.SEGATTR_TYPE) and fn(seg, n):
                    return True
        return False

    def IterSegByName(self, fn, name):
        for n in xrange(idaapi.get_segm_qty()):
            seg = idaapi.getnseg(n)
            if seg:
                if name == idaapi.get_true_segm_name(seg) and fn(seg, n):
                    return True
        return False

    def FindSegByName(self, name, occurCnt = 1):
        for n in xrange(idaapi.get_segm_qty()):
            seg = idaapi.getnseg(n)
            if seg:
                if name == idaapi.get_true_segm_name(seg):
                    occurCnt -= 1
                    if occurCnt <= 0:
                        return seg, n

class StrucOp(uuts.Serialize):
    """
     struct operator
    """
    class MemOp(uuts.Serialize):
        """
        member operator
        """
        def _InitDict(self, d, parent):
            self.parent = parent
            self.name = d["name"]
            self.offset_ = d["offset_"]
            self.offset = d["offset"]
            self.size = d["size"]
            self.flags = uuts.BuildObj(d["flags"])
            self.cmt = d["cmt"]
            self.rcmt = d["rcmt"]
            self.type = uuts.BuildObj(d["type"])

        def _InitOrd(self, t, parent):
            self.parent = parent
            self.name = t[1]
            # union的时候用这个
            self.offset_ = t[0]
            # struct的时候用这个或offset_
            self.offset = idc.GetMemberOffset(parent.sid, self.name)
            self.size = t[2]
            sptr = idaapi.get_struc(parent.sid)
            assert sptr
            mptr = idaapi.get_member(sptr, self.offset_)
            assert mptr
            self.flags = FlagsOp0(mptr.id, mptr.flag)
            self.cmt = idc.GetMemberComment(parent.sid, self.offset_, 0)
            self.rcmt = idc.GetMemberComment(parent.sid, self.offset_, 1)
            self.type = OpType(mptr.id)

        def _BuildDict(self, d):
            d["offset_"] = self.offset_
            d["offset"] = self.offset
            d["name"] = self.name
            d["size"] = self.size
            d["flags"] = self.flags
            d["cmt"] = self.cmt
            d["rcmt"] = self.rcmt
            d["type"] = self.type

        def AddMem(self):
            #名字中凡是带空格的，都是系统命名的
            if not idaapi.is_valid_typename(self.name):
                return True
            # print "开始添加成员:%s.%s" % (self.parent.name, self.name)
            if not self.flags.Refresh():
                return False
            #-------------------------废弃掉下面的-----------------------------------------------
            # res = None
            # typeid = None
            # if idc.isStruct(self.flag):
            #     typeid = self.opinfo.tid
            # elif idc.isOff0(self.flag):
            #     res = idc.AddStrucMember(self.parent.sid, self.name, self.offset, self.flag, self.opinfo.ri.base,
            #                              self.size,
            #                              self.opinfo.ri.target, self.opinfo.ri.tdelta, self.opinfo.ri.flags)
            # elif idc.isASCII(self.flag):  # (\ref ASCSTR_)
            #     typeid = self.opinfo.strtype
            # elif idc.isStroff0(self.flag):
            #     assert self.opinfo.path.len == 1
            #     typeid = self.opinfo.path.ids[0]
            # elif idc.isEnum0(self.flag):
            #     typeid = self.opinfo.ec.tid
            # elif isCustom(self.flag):
            #     assert False, "尚不支持自定义数据2"
            # if res is None:
            #     if typeid is None:
            #         res = AddStrucMemberEx(self.parent.sid, self.name, self.offset, self.flag, self.size, self.opinfo._impl_)
            #     else:
            #         res = idc.AddStrucMember(self.parent.sid, self.name, self.offset, self.flag, typeid, self.size)
            #------------------------------------------------------------------------------------------------------
            res = AddStrucMemberEx(self.parent.sid, self.name, self.offset, self.flags.flag, self.size, self.flags.GetOpinfo())
            if STRUC_ERROR_MEMBER_OK != res:
                assert False, "%s.%s添加成员失败，错误码：%d" % (self.parent.name, self.name, res)
                return False
            if self.cmt:
                idc.SetMemberComment(self.parent.sid, self.offset_, self.cmt, 0)
            if self.rcmt:
                idc.SetMemberComment(self.parent.sid, self.offset_, self.rcmt, 1)
            sptr = idaapi.get_struc(self.parent.sid)
            assert sptr
            mptr = idaapi.get_member(sptr, self.offset_)
            assert mptr
            if not self.type(mptr.id):
                assert False
                return False
            return True

        def Undefine(self):
            if not idaapi.is_valid_typename(self.name):
                return False
            sptr = idaapi.get_struc(self.parent.sid)
            if not sptr:
                assert False, "StrucOp.MemOp之Undefine失败，没找到结构体,结构体名：%s，成员名：%s"%(self.parent.name, self.name)
                return False
            if idaapi.del_struc_member(sptr, self.offset_) != 1:
                return False
            return True

        def Update(self, other):
            if not other.flags.Refresh():
                return False
            if self.offset_ != other.offset_ or self.offset != other.offset or self.name != other.name:
                assert False
                return False
            res = SetStrucMemberEx(self.parent.sid, self.offset_, other.flags.flag, other.size, other.flags.GetOpinfo())
            if True != res:
                assert False, "%s.%s更新成员失败" % (self.parent.name, self.name)
                return False
            if self.cmt != other.cmt and other.cmt:
                idc.SetMemberComment(self.parent.sid, self.offset_, other.cmt, 0)
            if self.rcmt != other.rcmt and other.rcmt:
                idc.SetMemberComment(self.parent.sid, self.offset_, other.rcmt, 1)
            sptr = idaapi.get_struc(self.parent.sid)
            assert sptr
            mptr = idaapi.get_member(sptr, self.offset_)
            assert mptr
            if not self.type(mptr.id):
                assert False
                return False
            return True

    def _InitDict(self, d):
        self.idx = d["idx"]
        self.sid = d["sid"]
        self.name = d["name"]
        self.union = d["union"]
        self.size = d["size"]
        self.memQty = d["memQty"]
        self.cmt = d["cmt"]
        self.rcmt = d["pcmt"]
        self.mems = []
        for i in d["mems"]:
            m = self.MemOp(i, self)
            self.mems.append(m)
        assert self.memQty == len(self.mems)

    def _InitOrd(self, sid):
        self.idx = idc.GetStrucIdx(sid)
        self.sid = sid
        self.name = idc.GetStrucName(sid)
        self.union = idc.IsUnion(sid)
        self.size = idc.GetStrucSize(sid)
        #print "size:", self.size
        self.memQty = idc.GetMemberQty(sid)
        self.cmt = idc.GetStrucComment(sid, 0)
        self.rcmt = idc.GetStrucComment(sid, 1)
        self.mems = []
        for i in idautils.StructMembers(sid):
            m = self.MemOp(i, self)
            self.mems.append(m)
        assert self.memQty == len(self.mems)

    def _BuildDict(self, d):
        d["idx"] = self.idx
        d["sid"] = self.sid
        d["name"] = self.name
        d["union"] = self.union
        d["size"] = self.size
        d["memQty"] = self.memQty
        d["cmt"] = self.cmt
        d["pcmt"] = self.rcmt
        d["mems"] = self.mems

    def AddStruct(self, isAddMems = False):
        if idc.GetStrucId(self.idx) == idc.BADADDR:
            idx = self.idx
        else:
            idx = -1
        sid = idc.AddStrucEx(idx, self.name, self.union)
        if sid == idaapi.BADNODE:
            assert False, "%s已经存在了" % self.name
            return False
        self.sid = sid
        self.idx = idc.GetStrucIdx(sid)
        self._AddOtherInfo()
        if isAddMems:
            return self.AddMembers()
        return True

    #这个函数若要更改，需要同时关注Update
    def AddMembers(self):
        for i in self.mems:
            if not i.AddMem():
                return False
        if idc.GetMemberQty(self.sid) != self.memQty:
            assert False, "结构体成员数量不一样,self.memQty:%d, 现在：%d"%(self.memQty, idc.GetMemberQty(self.sid))
            return False
        #GetStrucSize有时候不刷新成正确的值，不知道为啥
        # if idc.GetStrucSize(self.sid) != self.size:
        #     assert False, "结构体大小不一样，size:%d, sid size:%d, sid: %x " % (self.size, idc.GetStrucSize(self.sid), self.sid)
        #     return False
        return True

    def _AddOtherInfo(self):
        if self.cmt:
            idc.SetStrucComment(self.sid, self.cmt, 0)
        if self.rcmt:
            idc.SetStrucComment(self.sid, self.rcmt, 1)

    def UndefineAll(self):
        sptr = idaapi.get_struc(self.sid)
        if not sptr:
            assert False, "StrucOp之UndefineAll失败，没找到结构体,结构体名：%s" % self.name
            return -1
        #这个函数不行，会把系统命名的都给删除了
        # return idaapi.del_struc_members(sptr, 0, self.size)
        for i in self.mems:
            i.Undefine()

    def Update(self, other):
        if self.union != other.union:
            assert False, "必须是相同的类型"
            return False
        #先删除所有旧成员
        self.UndefineAll()
        self._InitOrd(self.sid)
        if self.cmt != other.cmt and other.cmt:
            idc.SetStrucComment(self.sid, other.cmt, 0)
        if self.rcmt != other.rcmt and other.rcmt:
            idc.SetStrucComment(self.sid, other.rcmt, 1)
        #接着进行成员的更新
        for i in self.mems:
            m = other.FindMemByName(i.name)
            if not m:
                continue
            if not i.Update(m):
                return False
        oldSid = other.sid
        oldSize = other.size
        #最后进行成员的添加
        try:
            for i in other.mems:
                i.parent = self
            other.sid = self.sid
            other.size = self.size
            return other.AddMembers()
        finally:
            for i in other.mems:
                i.parent = other
            other.sid = oldSid
            other.size = oldSize

    def FindMemByName(self, memName):
        for i in self.mems:
            if i.name == memName:
                return i


@uuts.singleton
class StructsMgr:
    def GenStructsStr(self):
        l = []
        for s in idautils.Structs():
            v = StrucOp(s[1])
            l.append(v)
        return repr(l)

    def AddStructsFromStr(self, l):
        l = eval(l)
        ss = []
        for i in l:
            s = StrucOp(i)
            if not s.AddStruct(False):
                return False, "添加%s结构体失败" % s.name
            ss.append(s)

        for s in ss:
            if not s.AddMembers():
                return False, "添加%s结构体成员失败" % s.name
        return True

    def CopyStructTo(self, strucName, toName):
        id = GetNetnodeId(strucName)
        if id == idaapi.BADNODE:
            assert False, "StructsMgr.CopyStructTo之没有这样的结构体名：%s" % strucName
            return False
        ptr = idaapi.get_struc(id)
        if not ptr:
            assert False, "StructsMgr.CopyStructTo之%s不是结构体" % strucName
            return False
        s = StrucOp(id)
        s.name = toName
        str = repr(s)
        # print str
        s = uuts.BuildObj(str)
        if not s.AddStruct(False):
            assert False, "StructsMgr.CopyStructTo之添加%s结构体失败" % s.name
            return False
        if not s.AddMembers():
            assert False, "StructsMgr.CopyStructTo之添加%s结构体成员失败" % s.name
        return True

    def DelAllStructs(self):
        ss = []
        for i in idautils.Structs():
            ss.append(i[1])

        for i in ss:
            if 1 != idc.DelStruc(i):
                assert False
                return -1
        return len(ss)


class EnumOp(uuts.Serialize):
    class Const(uuts.Serialize):
        def _InitDict(self, d, parent):
            self.parent = parent
            self.cid = d["cid"]
            self.name = d["name"]
            # for test
            # self.name += '_'
            self.value = d["value"]
            self.bmask = d["bmask"]
            self.cmt = d["cmt"]
            self.rcmt = d["rcmt"]

        def _InitOrd(self, t, parent):
            self.parent = parent
            self.cid = t[0]
            self.name = idc.GetConstName(self.cid)
            assert self.name, "Const竟然没有名字"
            self.value = idc.GetConstValue(self.cid)
            self.bmask = t[1]
            self.cmt = idc.GetConstCmt(self.cid, 0)
            self.rcmt = idc.GetConstCmt(self.cid, 1)

        def _BuildDict(self, d):
            d["cid"] = self.cid
            d["name"] = self.name
            d["value"] = self.value
            d["bmask"] = self.bmask
            d["cmt"] = self.cmt
            d["rcmt"] = self.rcmt

        def AddConst(self):
            global MAXULONG
            if idc.GetConstByName(self.name) != MAXULONG:
                assert False, "%s:已经存在常量名%s，常量名是全局唯一的" % (self.parent.name, self.name)
                return False
            res = idc.AddConstEx(self.parent.eid, self.name, self.value, self.bmask)
            if 0 != res:
                assert False, "%s.%s添加失败，错误码：%d" % (self.parent.name, self.name, res)
                return False
            self.cid = idc.GetConstByName(self.name)
            assert self.cid != MAXULONG, "GetConstByName必须要成功"
            if self.cmt:
                idc.SetConstCmt(self.cid, self.cmt, 0)
            if self.rcmt:
                idc.SetConstCmt(self.cid, self.rcmt, 1)
            return True

    def _InitDict(self, d):
        self.eid = d["eid"]
        self.name = d["name"]
        self.idx = d["idx"]
        self.size = d["size"]
        self.width = d["width"]
        self.flag = d["flag"]
        self.bitfield = d["bitfield"]
        self.cmt = d["cmt"]
        self.rcmt = d["rcmt"]
        self.consts = []
        for i in d["consts"]:
            self.consts.append(self.Const(i, self))

    def _InitOrd(self, eid):
        self.eid = eid
        self.name = idc.GetEnumName(eid)
        assert self.name, "EnumOp之名字不能为空"
        self.idx = idc.GetEnumIdx(eid)
        self.size = idc.GetEnumSize(eid)
        self.width = idc.GetEnumWidth(eid)
        self.flag = idc.GetEnumFlag(eid)
        self.bitfield = idc.IsBitfield(eid)
        self.cmt = idc.GetEnumCmt(eid, 0)
        self.rcmt = idc.GetEnumCmt(eid, 1)
        self.consts = []
        for i in self.Consts():
            self.consts.append(self.Const(i, self))

    def _BuildDict(self, d):
        d["eid"] = self.eid
        d["name"] = self.name
        d["idx"] = self.idx
        d["size"] = self.size
        d["width"] = self.width
        d["flag"] = self.flag
        d["bitfield"] = self.bitfield
        d["cmt"] = self.cmt
        d["rcmt"] = self.rcmt
        d["consts"] = self.consts

    def Consts(self):
        """
        return (const id, bmask)
        """
        global MAXULONG
        if self.bitfield == 0:
            return self._ConstsImpl(MAXULONG)
        else:
            return self._ConstsBmask()

    def AddEnum(self):
        if idc.GetnEnum(self.idx) == idaapi.BADNODE:
            idx = self.idx
        else:
            idx = -1
        eid = idc.AddEnum(idx, self.name, self.flag)
        if eid == idc.BADADDR:
            assert False, "enum(%s)添加失败" % self.name
            return False
        self.eid = eid
        self.idx = idc.GetEnumIdx(eid)

        if self.cmt:
            idc.SetEnumCmt(eid, self.cmt, 0)
        if self.rcmt:
            idc.SetEnumCmt(eid, self.rcmt, 1)

        if 1 != idc.SetEnumWidth(eid, self.width):
            assert False, "enum(%s)设置宽度失败" % self.name
            return False
        if 1 != idc.SetEnumBf(eid, self.bitfield):
            assert False, "enum(%s)设置bifield失败" % self.name
            return False
        for i in self.consts:
            if not i.AddConst():
                assert False, "添加%s.%s时失败" % (self.name, i.name)
                return False
        assert self.size == len(self.consts)
        return True

    def _ConstsImpl(self, bmask):
        def _SubValueConst(eid, v):
            global MAXULONG
            for i in xrange(idaapi.MAX_ENUM_SERIAL):
                res = idc.GetConstEx(eid, v, i, bmask)
                if res == MAXULONG:
                    break
                yield res

        v = idc.GetFirstConst(self.eid, bmask)
        while True:
            cnt = 0
            for cid in _SubValueConst(self.eid, v):
                cnt += 1
                yield cid, bmask
            if cnt <= 0:
                break
            old = v
            v = idc.GetNextConst(self.eid, v, bmask)
            if v == old:
                break

    def _ConstsBmask(self):
        assert self.bitfield == 1, "_ConstsOrdinal bitfield must == 1"
        b = idc.GetFirstBmask(self.eid)
        while True:
            cnt = 0
            for res in self._ConstsImpl(b):
                cnt += 1
                yield res
            if cnt <= 0:
                break
            old = b
            b = idc.GetNextBmask(self.eid, b)
            if b == old:
                break


@uuts.singleton
class EnumMgr:
    def Enums(self):
        maxCnt = idc.GetEnumQty()
        cnt = 0
        idx = 0
        while cnt < maxCnt:
            eid = idc.GetnEnum(idx)
            if eid != idaapi.BADNODE:
                yield eid
                cnt += 1
            idx += 1

    def GenEnumsStr(self):
        l = []
        for i in self.Enums():
            v = EnumOp(i)
            l.append(v)
        return repr(l)

    def AddEnumsFromStr(self, l):
        l = eval(l)
        for i in l:
            s = EnumOp(i)
            if not s.AddEnum():
                return False, "添加%s枚举失败" % s.name
        return True

    def CopyEnumTo(self, enumName, toName):
        id = idaapi.get_enum(enumName)
        if id == idaapi.BADNODE:
            assert False, "EnumMgr.CopyEnumTo之没有这样的enum名：%s" % enumName
            return False
        s = EnumOp(id)
        s.name = toName
        str = repr(s)
        s = uuts.BuildObj(str)
        #名字后面全部加下划线
        for i in s.consts:
            i.name += "_"

        if not s.AddEnum():
            assert False, "EnumMgr.CopyEnumTo之添加%s enum失败" % s.name
            return False
        return True

    def DelAllEnums(self):
        ss = []
        for i in self.Enums():
            ss.append(i)

        for i in ss:
            idc.DelEnum(i)
        return len(ss)


class Netnode(uuts.Serialize):
    def _InitDict(self, d):
        self.name = d["name"]
        self.id = NamesMgr().DelayGetNetnodeId(self.name)
        assert self.id != idaapi.BADNODE, "Netnode之通过名字没有找到有效id，名字:%s" % d["name"]

    def _InitOrd(self, id, name = None):
        self.id = id
        assert self.id != idaapi.BADNODE, "Netnode之ID是无效的"
        if name:
            self.name = name
        else:
            self.name = idaapi.netnode(self.id).name()
        assert self.name, "Netnode之该ID必须得有名字,id: %x " % self.id

    def _BuildDict(self, d):
        d["id"] = self.id
        d["name"] = self.name


class NetnodeMember(Netnode):
    def _InitDict(self, d):
        Netnode._InitDict(self, d)
        self.parent = uuts.BuildObj(d["parent"])

    def _InitOrd(self, id, name = None, parent = None):
        Netnode._InitOrd(self, id, name)
        if parent:
            self.parent = parent
        else:
            sptr = idaapi.get_member_struc(self.name)
            assert sptr, "NetnodeMember之parent为空，name:%s" % self.name
            id = sptr.id
            self.parent = Netnode(id)

    def _BuildDict(self, d):
        Netnode._BuildDict(self, d)
        d["parent"] = self.parent


class NetnodeFrameMember(NetnodeMember):
    def _InitDict(self, d):
        NetnodeMember._InitDict(self, d)
        self.fn = uuts.BuildObj(d["fn"])

    def _InitOrd(self, id, name = None, parent = None, fn = None):
        NetnodeMember._InitOrd(self, id, name, parent)
        if fn:
            self.fn = fn
        else:
            ea = idaapi.get_func_by_frame(self.parent.id)
            assert ea != idc.BADADDR, "NetnodeFrameMember之函数未找到，name:%s,frameName:%s" % (self.name, self.parent.name)
            self.fn = Netnode(ea)

    def _BuildDict(self, d):
        NetnodeMember._BuildDict(self, d)
        d["fn"] = self.fn


class NetnodeFnOffset(Netnode):
    def _InitDict(self, d):
        self.fn = uuts.BuildObj(d["fn"])
        self.offset = d["offset"]
        self.id = self.fn.id + self.offset
        fnPtr = idaapi.get_func(self.fn.id)
        assert fnPtr
        assert fnPtr.contains(self.id), "NetnodeFnOffset之函数(%s)的偏移( %x )处已不属于该函数" % (self.fn.name, self.offset)

    def _InitOrd(self, id):
        fnPtr = idaapi.get_func(id)
        assert fnPtr, "NetnodeFnOffset之 %x 所在必须是个函数" % id
        self.fn = Netnode(fnPtr.startEA)
        self.offset = id - fnPtr.startEA
        self.id = id

    def _BuildDict(self, d):
        d["id"] = self.id
        d["fn"] = self.fn
        d["offset"] = self.offset


class NetnodeFnInsn(Netnode):
    def _InitDict(self, d):
        self.fn = uuts.BuildObj(d["fn"])
        self.insnCnt = d["insnCnt"]
        fnPtr = idaapi.get_func(self.fn.id)
        assert fnPtr
        self.id, self.byCnt = RelativeInsnOffset(self.insnCnt, self.fn.id, fnPtr.endEA)
        assert self.id != idc.BADADDR and self.byCnt == d["byCnt"], "NetnodeFnInsn之函数(%s)的偏移( %x )处已不属于该函数" % (self.fn.name, d["byCnt"])

    def _InitOrd(self, id):
        fnPtr = idaapi.get_func(id)
        assert fnPtr, "NetnodeFnInsn之 %x 所在必须是个函数" % id
        self.insnCnt, self.byCnt = InsnAndSize(fnPtr.startEA, id)
        self.fn = Netnode(fnPtr.startEA)
        self.id = id

    def _BuildDict(self, d):
        d["id"] = self.id
        d["fn"] = self.fn
        d["insnCnt"] = self.insnCnt
        d["byCnt"] = self.byCnt


class NetnodeText(Netnode):
    def _InitDict(self, d):
        self.name = d["name"]
        self.id = GetNetnodeId(self.name)
        # for test
        # self.id = idaapi.BADNODE
        self.text = d["text"]
        # 若在目标数据库中未找到名字，则用self.text寻找
        if self.id == idaapi.BADNODE:
            self._InitByText()
        else:
            str = GetStringEx(self.id)
            if not str or not str.startswith(self.text):
                self._InitByText()

    def _InitOrd(self, id, text = None):
        self.id = id
        self.name = idaapi.netnode(self.id).name()
        # for test
        # self.name = "aHeVariable"
        if text:
            self.text = text
        else:
            self.text = GetStringEx(id)

    def _InitByText(self):
        mgr = StringsExMgr()
        res = mgr.FindStartswith(self.text)
        if res is None:
            assert False, "NetnodeText没找到text，id: %x , name:%s, text:%s" % (self.id, self.name, self.text)
            return
        self.id = res.ea
        self.name = idaapi.netnode(self.id).name()

    def _BuildDict(self, d):
        d["text"] = self.text
        d["name"] = self.name


def ParseNetnode(id, isFnInsn = True):
    m = idaapi.get_member_by_id(id)
    if m:
        name = idaapi.netnode(id).name()
        parent = idaapi.get_member_struc(name)
        assert parent
        ea = idaapi.get_func_by_frame(parent.id)
        if ea == idc.BADADDR:
            return NetnodeMember(id, name, Netnode(parent.id))
        else:
            return NetnodeFrameMember(id, name, Netnode(parent.id), Netnode(ea))

    fnPtr = idaapi.get_func(id)
    if fnPtr:
        if id != fnPtr.startEA:
            if isFnInsn:
                return NetnodeFnInsn(id)
            else:
                return NetnodeFnOffset(id)

    return Netnode(id)


@uuts.singleton
class NamesMgr:
    def DelayGetNetnodeId(self, name):
        while True:
            id = GetNetnodeId(name)
            if id != idaapi.BADNODE:
                return id
            res = uuts.GletParentSwitch(DUR_Continue)
            if res[0] == False:
                return id


class StringsEx(idautils.Strings):
    def Iter(self):
        for i in xrange(0, self.size):
            yield self._get_item(i)

    def FindStartswith(self, text):
        for i in self.Iter():
            if str(i).startswith(text):
                return i


@uuts.singleton
class StringsExMgr(StringsEx):
    pass


#操作，封装的是算法
class _OpBase(uuts.Serialize):
    #接受ea(或从Translator得到的结果)，成功返回0，否则返回非0
    def __call__(self, ea, *args, **kwargs):
        assert False, "必须得自己实现"

    def IsExistName(self, name):
        return False

    def Valid(self):
        return True


class OpType(_OpBase):
    def _InitDict(self, d):
        self.type = d["type"]

    def _InitOrd(self, ea):
        #GetType得到的只是类型声明，没有名字，而print_type有名字
        # self.type = idc.GetType(ea)
        self.type = idaapi.print_type(ea, True)

    def _BuildDict(self, d):
        d["type"] = self.type

    def Valid(self):
        return self.type is not None

    def __call__(self, ea, *args, **kwargs):
        if not self.type:
            if idc.GetType(ea):
                return idc.SetType(ea, "")
            return True
        if idc.SetType(ea, self.type) == 0:
            assert False, "OpType设置type失败，地址： %x , type:%s" % (ea, self.type)
            return False
        return True


class refinfo_tProxy(uuts.ProxyBase):
    def _InitDict(self, d):
        self._impl_ = idaapi.refinfo_t()
        self.target = d["target"]
        self.base = d["base"]
        self.tdelta = d["tdelta"]
        self.flags = d["flags"]

    def _BuildDict(self, d):
        d["target"] = self.target
        d["base"] = self.base
        d["tdelta"] = self.tdelta
        d["flags"] = self.flags


class enum_const_tProxy(uuts.ProxyBase):
    def _InitDict(self, d):
        self._impl_ = idaapi.enum_const_t()
        self.tid_ = tid_tProxy(d["tid"])
        self.serial = d["serial"]

    def _InitOrd(self, impl):
        self._impl_ = impl
        self.tid_ = tid_tProxy(self.tid, False)

    def _BuildDict(self, d):
        d["tid"] = self.tid_
        d["serial"] = self.serial

    def Refresh(self):
        if not self.tid_.Refresh():
            return False
        self.tid = self.tid_.tid
        return True


class strpath_tProxy(uuts.ProxyBase):
    def _InitDict(self, d):
        self._impl_ = idaapi.strpath_t()
        self.len = d["len"]
        #此时就让ids_引用d["ids"]，不要变成自以为是的数组，否则去关注下strpath_tProxy.Refresh
        self.ids_ = d["ids"]
        assert self.len == len(self.ids_)
        self.delta = d["delta"]

    def _InitOrd(self, impl):
        self._impl_ = impl
        self.ids_ = [tid_tProxy(self.ids[x], True) for x in xrange(self.len)]

    def Refresh(self):
        idx = 0
        for i in self.ids_:
            v = tid_tProxy(i)
            if not v.Refresh():
                assert False
                return False
            self.ids[idx] = v.tid
            idx += 1
        return True

    def _BuildDict(self, d):
        d["len"] = self.len
        d["ids"] = self.ids_
        d["delta"] = self.delta


class tid_tProxy(uuts.Serialize):
    isStruc = None
    def _InitDict(self, d):
        self.tid = d["tid"]
        self.isStruc = d["isStruc"]
        if self.isStruc is None:
            self.sname = d["sname"]
            self.ename = d["ename"]
        elif self.isStruc:
            self.sname = d["sname"]
        else:
            self.ename = d["ename"]

    def _InitOrd(self, tid, isStruct = None):
        self.tid = tid
        self.isStruc = isStruct
        if self.isStruc is None:
            self.sname = idc.GetStrucName(self.tid)
            self.ename = idc.GetEnumName(self.tid)
        elif self.isStruc:
            self.sname = idc.GetStrucName(self.tid)
        else:
            self.ename = idc.GetEnumName(self.tid)

    def Refresh(self):
        if self.isStruc is None:
            self.tid = idc.GetStrucIdByName(self.sname)
            if self.tid == idc.BADADDR:
                self.tid = idc.GetEnum(self.ename)
                if self.tid == idaapi.BADNODE:
                    assert False
                    return False
        elif self.isStruc:
            self.tid = idc.GetStrucIdByName(self.sname)
            if self.tid == idc.BADADDR:
                assert False
                return False
        else:
            self.tid = idc.GetEnum(self.ename)
            # assert isinstance(self.tid, long)
            if self.tid == idaapi.BADNODE:
                assert False
                return False
        return True

    def _BuildDict(self, d):
        d["tid"] = self.tid
        d["isStruc"] = self.isStruc
        if self.isStruc is None:
            d["sname"] = self.sname
            d["ename"] = self.ename
        elif self.isStruc:
            d["sname"] = self.sname
        else:
            d["ename"] = self.ename


class _opinfo_tProxyBase(uuts.ProxyBase):
    def _InitDict(self, d):
        self._impl_ = idaapi.opinfo_t()
        self.flag = d["flag"]
        self.isRefreshed = False
        if idc.isStruct(self.flag):
            self.tid_ = tid_tProxy(d["tid"])
        elif self.isOff(self.flag):
            self.ri_ = refinfo_tProxy(d["ri"])
            self.ri = self.ri_._impl_
        elif idc.isASCII(self.flag):  # (\ref ASCSTR_)
            self.strtype = d["strtype"]
        elif self.isStroff(self.flag):
            self.path_ = strpath_tProxy(d["path"])
        elif self.isEnum(self.flag):
            self.ec_ = enum_const_tProxy(d["ec"])
        elif isCustom(self.flag):
            assert False, "尚不支持自定义数据1"
        else:
            assert False

    def _InitOrd(self, impl, flag):
        self.flag = flag
        self.isRefreshed = True
        self._impl_ = impl
        assert impl
        if idc.isStruct(self.flag):
            self.tid_ = tid_tProxy(self.tid, True)
        elif self.isOff(self.flag):
            self.ri_ = refinfo_tProxy(self.ri)
        elif idc.isASCII(self.flag):  # (\ref ASCSTR_)，这里还不能注释掉呢
            pass
        elif self.isStroff(self.flag):
            self.path_ = strpath_tProxy(self.path)
        elif self.isEnum(self.flag):
            self.ec_ = enum_const_tProxy(self.ec)
        elif isCustom(self.flag):
            assert False, "尚不支持自定义数据1"
        else:
            assert False

    def Refresh(self):
        if self.isRefreshed:
            return True
        self.isRefreshed = True
        if idc.isStruct(self.flag):
            if not self.tid_.Refresh():
                return False
            self.tid = self.tid_.tid
        # elif self.isOff(self.flag):
        # elif idc.isASCII(self.flag):  # (\ref ASCSTR_)
        elif self.isStroff(self.flag):
            if not self.path_.Refresh():
                return False
            self.path = self.path_._impl_
            # assert self.path.ids[0] == self.path_._impl_.ids[0]
        elif self.isEnum(self.flag):
            if not self.ec_.Refresh():
                return False
            self.ec = self.ec_._impl_
            # assert self.ec.tid == self.ec_._impl_.tid
        elif isCustom(self.flag):
            assert False, "尚不支持自定义数据1"
        return True

    def _BuildDict(self, d):
        assert self.flag
        if not self._impl_:
            return repr(None)
        d["flag"] = self.flag
        if idc.isStruct(self.flag):
            d["tid"] = self.tid_
        elif self.isOff(self.flag):
            d["ri"] = self.ri_
        elif idc.isASCII(self.flag):  # (\ref ASCSTR_)
            d["strtype"] = self.strtype
        elif self.isStroff(self.flag):
            d["path"] = self.path_
        elif self.isEnum(self.flag):
            d["ec"] = self.ec_
        elif isCustom(self.flag):
            assert False, "尚不支持自定义数据"

class opinfo_tProxy0(_opinfo_tProxyBase):
    isOff = lambda self, f : idc.isOff0(f)
    isStroff = lambda self, f : idc.isStroff0(f)
    isEnum = lambda self, f : idc.isEnum0(f)

class opinfo_tProxy1(_opinfo_tProxyBase):
    isOff = lambda self, f : idc.isOff1(f)
    isStroff = lambda self, f : idc.isStroff1(f)
    isEnum = lambda self, f : idc.isEnum1(f)


#opinfo成员不要直接访问，请用GetOpinfo()
class _FlagsOpBase(uuts.Serialize):
    def _InitDict(self, d):
        self.n = d["n"]
        self.flag = d["flag"]
        self.opinfo = uuts.BuildObj(d["opinfo"])

    def _InitOrd(self, n, flag, opinfo):
        self.n = n
        self.flag = flag
        self.opinfo = opinfo

    def _BuildDict(self, d):
        d["n"] = self.n
        d["flag"] = self.flag
        d["opinfo"] = self.opinfo

    def GetOpinfo(self):
        if not self.opinfo:
            return
        if not self.Refresh():
            assert False
            return
        return self.opinfo._impl_

    def Refresh(self):
        if not self.opinfo:
            return True
        return self.opinfo.Refresh()

    def SetFlag(self, ea, checkFlag = False):
        if not self.Refresh():
            assert False
            return False
        if checkFlag and idc.GetFlags(ea) == self.flag:
            return True
        type = idc.GetOpType(ea, self.n)
        if type == idc.o_void:
            return False
        if not self.SetOpinfo(ea):
            return False
        idaapi.setFlags(ea, self.flag)
        return True

    def SetOpinfo(self, ea):
        if not self.Refresh():
            assert False
            return False
        if self.opinfo and not idaapi.set_opinfo(ea, self.n, self.flag, self.GetOpinfo()):
            assert False, "_FlagsOpBase之SetOpinfo失败，地址： %x " % ea
            return False
        return True


class FlagsOp0(_FlagsOpBase):
    def _InitOrd(self, ea, flag):
        op = GetOpInfo(ea, 0, flag)
        if op:
            _FlagsOpBase._InitOrd(self, 0, flag, opinfo_tProxy0(op, flag))
        else:
            _FlagsOpBase._InitOrd(self, 0, flag, None)


class FlagsOp1(_FlagsOpBase):
    def _InitOrd(self, ea, flag):
        op = GetOpInfo(ea, 1, flag)
        if op:
            _FlagsOpBase._InitOrd(self, 1, flag, opinfo_tProxy1(op, flag))
        else:
            _FlagsOpBase._InitOrd(self, 1, flag, None)


def GetMember(sid, member_offset):
    s = idaapi.get_struc(sid)
    if not s:
        return
    return idaapi.get_member(s, member_offset)


def RetrieveMemberInfo(mptr):
    # idaapi.retrieve_member_info这类函数返回的指针，是只读的，引用它并不会增加引用计数
    op = idaapi.opinfo_t()
    if idaapi.retrieve_member_info(mptr, op):
        return op
    else:
        return None

def GetOpInfo(ea, n, flags):
    op = idaapi.opinfo_t()
    # idaapi.get_opinfo这类函数返回的指针，是只读的，引用它并不会增加引用计数
    if idaapi.get_opinfo(ea, n, flags, op):
        return op
    return None

def AddStrucMemberEx(sid, name, offset, flag, nbytes, opinfo = None):
    s = idaapi.get_struc(sid)
    if s:
        return idaapi.add_struc_member(s, name, offset, flag, opinfo, nbytes)
    else:
        assert False, "添加成员%s失败，没找到spid" % name
        return idc.STRUC_ERROR_MEMBER_STRUCT

def SetStrucMemberEx(sid, offset, flag, nbytes, opinfo = None):
    s = idaapi.get_struc(sid)
    if s:
        return idaapi.set_member_type(s, offset, flag, opinfo, nbytes)
    else:
        assert False, "SetStrucMemberEx失败，没找到spid, sid: %x " % sid
        return False
