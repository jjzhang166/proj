#coding=gbk

# import idaapi, idc, idautils
import uuts, idaex, idaaut
import xmlrpclib
from idaex import *
from idaaut import *
# from idc import *
# from idautils import *


uuts.SetLogFile("f:/tmp/tmp/GenLogC.txt")
idaaut.SetErrorObjFile("f:/tmp/tmp/GenErrObjC.txt", "f:/tmp/tmp/GenConErrObjC.txt")
uuts.DefGlobal = lambda : globals()


envMgr = EnvObjMgr()


# que = envMgr.GenByEa(ea)
# que = envMgr.GenAllStructEnum()
que = envMgr.GenAll()


proxy = xmlrpclib.ServerProxy("http://localhost:21111/")


# mc = xmlrpclib.MultiCall(proxy)
# mc.ExecObj(uuts.ObjToUStr(que))
# res = mc()
# print uuts.ObjFromUStr(res[0])


res = proxy.ExecObj(uuts.ObjToUStr(que))
print uuts.StrFromUStr(res)
