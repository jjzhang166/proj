#coding=gbk
# import idaapi, idc, idautils
import uuts, idaex, idaaut
import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCServer
from idaex import *
from idaaut import *
# from idc import *
# from idautils import *


uuts.SetLogFile("f:/tmp/tmp/RunLogS.txt")
idaaut.SetErrorObjFile("f:/tmp/tmp/RunErrObjS.txt", "f:/tmp/tmp/RunConErrObjS.txt")
uuts.DefGlobal = lambda : globals()


def TestFn(a):
    print a
    return 1


server = SimpleXMLRPCServer(("localhost", 21111))

print "Listening on port 21111..."
server.register_multicall_functions()
server.register_function(TestFn)
server.register_instance(Interface(True))


EnumMgr().DelAllEnums()
StructsMgr().DelAllStructs()


server.serve_forever()
