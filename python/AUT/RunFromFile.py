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


uuts.SetLogFile("f:/tmp/tmp/RunLog.txt")
idaaut.SetErrorObjFile("f:/tmp/tmp/RunErrObj.txt", "f:/tmp/tmp/RunConErrObj.txt")
uuts.DefGlobal = lambda : globals()


ince = Interface(False)
EnumMgr().DelAllEnums()
StructsMgr().DelAllStructs()
ince.ExecObjFromFile("f:/tmp/tmp/ObjFile.txt")
