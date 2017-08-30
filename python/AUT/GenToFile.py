#coding=gbk
def Reload():
    try:
        reload(uuts)
        reload(idaex)
        reload(idaaut)
    except:
        pass

Reload()

# import idaapi, idc, idautils
import uuts, idaex, idaaut
from idaex import *
from idaaut import *
# from idc import *
# from idautils import *


uuts.SetLogFile("f:/tmp/tmp/GenLog.txt")
idaaut.SetErrorObjFile("f:/tmp/tmp/GenErrObj.txt", "f:/tmp/tmp/GenConErrObj.txt")
uuts.DefGlobal = lambda : globals()


envMgr = EnvObjMgr()

# que = envMgr.GenByEa(ea)
# que = envMgr.GenAllStructEnum()
que = envMgr.GenAll()
que.WriteToFile("f:/tmp/tmp/ObjFile.txt")
