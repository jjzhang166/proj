#include "stdafx.h"
#include "dm.h"
#include "src/obj.h"
#include <cassert>
#include <luabind/raw_policy.hpp>


static dmsoft& GetImpl_(void* impl){
	assert(impl);
	return *static_cast<dmsoft*>(impl);
}
static std::string t2a_(const CString& str){
	if (str.IsEmpty())
		return std::string();
	std::string res = CT2CA(str);
	return res;
}
static CString a2t_(const std::string& str){
	if (str.empty())
		return CString();
	CString res = CA2CT(str.c_str());
	return res;
}

DmSoft::DmSoft()
{
	impl_ = nullptr;
	impl_ = new dmsoft;
}

DmSoft::~DmSoft()
{
	auto impl = (dmsoft*)impl_;
	delete impl;
	impl_ = nullptr;
}

void DmSoft::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<DmSoft>("DmSoft")
		.def(constructor<>())
		.def("Ver", &DmSoft::Ver)
		.def("SetPath", &DmSoft::SetPath)
		.def("Ocr", &DmSoft::Ocr)
		.def("FindStr", &DmSoft::_FindStr_, raw(_2))
		.def("GetResultCount", &DmSoft::GetResultCount)
		.def("GetResultPos", &DmSoft::_GetResultPos_, raw(_2))
		.def("StrStr", &DmSoft::StrStr)
		.def("SendCommand", &DmSoft::SendCommand)
		.def("UseDict", &DmSoft::UseDict)
		.def("GetBasePath", &DmSoft::GetBasePath)
		.def("SetDictPwd", &DmSoft::SetDictPwd)
		.def("OcrInFile", &DmSoft::OcrInFile)
		.def("Capture", &DmSoft::Capture)
		.def("KeyPress", &DmSoft::KeyPress)
		.def("KeyDown", &DmSoft::KeyDown)
		.def("KeyUp", &DmSoft::KeyUp)
		.def("LeftClick", &DmSoft::LeftClick)
		.def("RightClick", &DmSoft::RightClick)
		.def("MiddleClick", &DmSoft::MiddleClick)
		.def("LeftDoubleClick", &DmSoft::LeftDoubleClick)
		.def("LeftDown", &DmSoft::LeftDown)
		.def("LeftUp", &DmSoft::LeftUp)
		.def("RightDown", &DmSoft::RightDown)
		.def("RightUp", &DmSoft::RightUp)
		.def("MoveTo", &DmSoft::MoveTo)
		.def("MoveR", &DmSoft::MoveR)
		.def("GetColor", &DmSoft::GetColor)
		.def("GetColorBGR", &DmSoft::GetColorBGR)
		.def("RGB2BGR", &DmSoft::RGB2BGR)
		.def("BGR2RGB", &DmSoft::BGR2RGB)
		.def("UnBindWindow", &DmSoft::UnBindWindow)
		.def("CmpColor", &DmSoft::CmpColor)
		.def("ClientToScreen", &DmSoft::_ClientToScreen_, raw(_2))
		.def("ScreenToClient", &DmSoft::_ScreenToClient_, raw(_2))
		.def("ShowScrMsg", &DmSoft::ShowScrMsg)
		.def("SetMinRowGap", &DmSoft::SetMinRowGap)
		.def("SetMinColGap", &DmSoft::SetMinColGap)
		.def("FindColor", &DmSoft::_FindColor_, raw(_2))
		.def("FindColorEx", &DmSoft::FindColorEx)
		.def("SetWordLineHeight", &DmSoft::SetWordLineHeight)
		.def("SetWordGap", &DmSoft::SetWordGap)
		.def("SetRowGapNoDict", &DmSoft::SetRowGapNoDict)
		.def("SetColGapNoDict", &DmSoft::SetColGapNoDict)
		.def("SetWordLineHeightNoDict", &DmSoft::SetWordLineHeightNoDict)
		.def("SetWordGapNoDict", &DmSoft::SetWordGapNoDict)
		.def("GetWordResultCount", &DmSoft::GetWordResultCount)
		.def("GetWordResultPos", &DmSoft::_GetWordResultPos_, raw(_2))
		.def("GetWordResultStr", &DmSoft::GetWordResultStr)
		.def("GetWords", &DmSoft::GetWords)
		.def("GetWordsNoDict", &DmSoft::GetWordsNoDict)
		.def("SetShowErrorMsg", &DmSoft::SetShowErrorMsg)
		.def("GetClientSize", &DmSoft::_GetClientSize_, raw(_2))
		.def("MoveWindow", &DmSoft::MoveWindow)
		.def("GetColorHSV", &DmSoft::GetColorHSV)
		.def("GetAveRGB", &DmSoft::GetAveRGB)
		.def("GetAveHSV", &DmSoft::GetAveHSV)
		.def("GetForegroundWindow", &DmSoft::GetForegroundWindow)
		.def("GetForegroundFocus", &DmSoft::GetForegroundFocus)
		.def("GetMousePointWindow", &DmSoft::GetMousePointWindow)
		.def("GetPointWindow", &DmSoft::GetPointWindow)
		.def("EnumWindow", &DmSoft::EnumWindow)
		.def("GetWindowState", &DmSoft::GetWindowState)
		.def("GetWindow", &DmSoft::GetWindow)
		.def("GetSpecialWindow", &DmSoft::GetSpecialWindow)
		.def("SetWindowText", &DmSoft::SetWindowText)
		.def("SetWindowSize", &DmSoft::SetWindowSize)
		.def("GetWindowRect", &DmSoft::_GetWindowRect_, raw(_2))
		.def("GetWindowTitle", &DmSoft::GetWindowTitle)
		.def("GetWindowClass", &DmSoft::GetWindowClass)
		.def("SetWindowState", &DmSoft::SetWindowState)
		.def("CreateFoobarRect", &DmSoft::CreateFoobarRect)
		.def("CreateFoobarRoundRect", &DmSoft::CreateFoobarRoundRect)
		.def("CreateFoobarEllipse", &DmSoft::CreateFoobarEllipse)
		.def("CreateFoobarCustom", &DmSoft::CreateFoobarCustom)
		.def("FoobarFillRect", &DmSoft::FoobarFillRect)
		.def("FoobarDrawText", &DmSoft::FoobarDrawText)
		.def("FoobarDrawPic", &DmSoft::FoobarDrawPic)
		.def("FoobarUpdate", &DmSoft::FoobarUpdate)
		.def("FoobarLock", &DmSoft::FoobarLock)
		.def("FoobarUnlock", &DmSoft::FoobarUnlock)
		.def("FoobarSetFont", &DmSoft::FoobarSetFont)
		.def("FoobarTextRect", &DmSoft::FoobarTextRect)
		.def("FoobarPrintText", &DmSoft::FoobarPrintText)
		.def("FoobarClearText", &DmSoft::FoobarClearText)
		.def("FoobarTextLineGap", &DmSoft::FoobarTextLineGap)
		.def("Play", &DmSoft::Play)
		.def("FaqCapture", &DmSoft::FaqCapture)
		.def("FaqRelease", &DmSoft::FaqRelease)
		.def("FaqSend", &DmSoft::FaqSend)
		.def("Beep", &DmSoft::Beep)
		.def("FoobarClose", &DmSoft::FoobarClose)
		.def("MoveDD", &DmSoft::MoveDD)
		.def("FaqGetSize", &DmSoft::FaqGetSize)
		.def("LoadPic", &DmSoft::LoadPic)
		.def("FreePic", &DmSoft::FreePic)
		.def("GetScreenData", &DmSoft::GetScreenData)
		.def("FreeScreenData", &DmSoft::FreeScreenData)
		.def("WheelUp", &DmSoft::WheelUp)
		.def("WheelDown", &DmSoft::WheelDown)
		.def("SetMouseDelay", &DmSoft::SetMouseDelay)
		.def("SetKeypadDelay", &DmSoft::SetKeypadDelay)
		.def("GetEnv", &DmSoft::GetEnv)
		.def("SetEnv", &DmSoft::SetEnv)
		.def("SendString", &DmSoft::SendString)
		.def("DelEnv", &DmSoft::DelEnv)
		.def("GetPath", &DmSoft::GetPath)
		.def("SetDict", &DmSoft::SetDict)
		.def("FindPic", &DmSoft::_FindPic_, raw(_2))
		.def("FindPicEx", &DmSoft::FindPicEx)
		.def("SetClientSize", &DmSoft::SetClientSize)
		.def("ReadInt", &DmSoft::ReadInt)
		.def("ReadFloat", &DmSoft::ReadFloat)
		.def("ReadDouble", &DmSoft::ReadDouble)
		.def("FindInt", &DmSoft::FindInt)
		.def("FindFloat", &DmSoft::FindFloat)
		.def("FindDouble", &DmSoft::FindDouble)
		.def("FindString", &DmSoft::FindString)
		.def("GetModuleBaseAddr", &DmSoft::GetModuleBaseAddr)
		.def("MoveToEx", &DmSoft::MoveToEx)
		.def("MatchPicName", &DmSoft::MatchPicName)
		.def("AddDict", &DmSoft::AddDict)
		.def("EnterCri", &DmSoft::EnterCri)
		.def("LeaveCri", &DmSoft::LeaveCri)
		.def("WriteInt", &DmSoft::WriteInt)
		.def("WriteFloat", &DmSoft::WriteFloat)
		.def("WriteDouble", &DmSoft::WriteDouble)
		.def("WriteString", &DmSoft::WriteString)
		.def("AsmAdd", &DmSoft::AsmAdd)
		.def("AsmClear", &DmSoft::AsmClear)
		.def("AsmCall", &DmSoft::AsmCall)
		.def("FindMultiColor", &DmSoft::_FindMultiColor_, raw(_2))
		.def("FindMultiColorEx", &DmSoft::FindMultiColorEx)
		.def("AsmCode", &DmSoft::AsmCode)
		.def("Assemble", &DmSoft::Assemble)
		.def("SetWindowTransparent", &DmSoft::SetWindowTransparent)
		.def("ReadData", &DmSoft::ReadData)
		.def("WriteData", &DmSoft::WriteData)
		.def("FindData", &DmSoft::FindData)
		.def("SetPicPwd", &DmSoft::SetPicPwd)
		.def("Log", &DmSoft::Log)
		.def("FindStrE", &DmSoft::FindStrE)
		.def("FindColorE", &DmSoft::FindColorE)
		.def("FindPicE", &DmSoft::FindPicE)
		.def("FindMultiColorE", &DmSoft::FindMultiColorE)
		.def("SetExactOcr", &DmSoft::SetExactOcr)
		.def("ReadString", &DmSoft::ReadString)
		.def("FoobarTextPrintDir", &DmSoft::FoobarTextPrintDir)
		.def("OcrEx", &DmSoft::OcrEx)
		.def("SetDisplayInput", &DmSoft::SetDisplayInput)
		.def("GetTime", &DmSoft::GetTime)
		.def("GetScreenWidth", &DmSoft::GetScreenWidth)
		.def("GetScreenHeight", &DmSoft::GetScreenHeight)
		.def("BindWindowEx", &DmSoft::BindWindowEx)
		.def("GetDiskSerial", &DmSoft::GetDiskSerial)
		.def("Md5", &DmSoft::Md5)
		.def("GetMac", &DmSoft::GetMac)
		.def("ActiveInputMethod", &DmSoft::ActiveInputMethod)
		.def("CheckInputMethod", &DmSoft::CheckInputMethod)
		.def("FindInputMethod", &DmSoft::FindInputMethod)
		.def("GetCursorPos", &DmSoft::_GetCursorPos_, raw(_2))
		.def("BindWindow", &DmSoft::BindWindow)
		.def("FindWindow", &DmSoft::FindWindow)
		.def("GetScreenDepth", &DmSoft::GetScreenDepth)
		.def("SetScreen", &DmSoft::SetScreen)
		.def("ExitOs", &DmSoft::ExitOs)
		.def("GetDir", &DmSoft::GetDir)
		.def("GetOsType", &DmSoft::GetOsType)
		.def("FindWindowEx", &DmSoft::FindWindowEx)
		.def("SetExportDict", &DmSoft::SetExportDict)
		.def("GetCursorShape", &DmSoft::GetCursorShape)
		.def("DownCpu", &DmSoft::DownCpu)
		.def("GetCursorSpot", &DmSoft::GetCursorSpot)
		.def("SendString2", &DmSoft::SendString2)
		.def("FaqPost", &DmSoft::FaqPost)
		.def("FaqFetch", &DmSoft::FaqFetch)
		.def("FetchWord", &DmSoft::FetchWord)
		.def("CaptureJpg", &DmSoft::CaptureJpg)
		.def("FindStrWithFont", &DmSoft::_FindStrWithFont_)
		.def("FindStrWithFontE", &DmSoft::FindStrWithFontE)
		.def("FindStrWithFontEx", &DmSoft::FindStrWithFontEx)
		.def("GetDictInfo", &DmSoft::GetDictInfo)
		.def("SaveDict", &DmSoft::SaveDict)
		.def("GetWindowProcessId", &DmSoft::GetWindowProcessId)
		.def("GetWindowProcessPath", &DmSoft::GetWindowProcessPath)
		.def("LockInput", &DmSoft::LockInput)
		.def("GetPicSize", &DmSoft::GetPicSize)
		.def("GetID", &DmSoft::GetID)
		.def("CapturePng", &DmSoft::CapturePng)
		.def("CaptureGif", &DmSoft::CaptureGif)
		.def("ImageToBmp", &DmSoft::ImageToBmp)
		.def("FindStrFast", &DmSoft::_FindStrFast_, raw(_2))
		.def("FindStrFastEx", &DmSoft::FindStrFastEx)
		.def("FindStrFastE", &DmSoft::FindStrFastE)
		.def("EnableDisplayDebug", &DmSoft::EnableDisplayDebug)
		.def("CapturePre", &DmSoft::CapturePre)
		.def("RegEx", &DmSoft::RegEx)
		.def("GetMachineCode", &DmSoft::GetMachineCode)
		.def("SetClipboard", &DmSoft::SetClipboard)
		.def("GetClipboard", &DmSoft::GetClipboard)
		.def("GetNowDict", &DmSoft::GetNowDict)
		.def("Is64Bit", &DmSoft::Is64Bit)
		.def("GetColorNum", &DmSoft::GetColorNum)
		.def("EnumWindowByProcess", &DmSoft::EnumWindowByProcess)
		.def("GetDictCount", &DmSoft::GetDictCount)
		.def("GetLastError", &DmSoft::GetLastError)
		.def("GetNetTime", &DmSoft::GetNetTime)
		.def("EnableGetColorByCapture", &DmSoft::EnableGetColorByCapture)
		.def("CheckUAC", &DmSoft::CheckUAC)
		.def("SetUAC", &DmSoft::SetUAC)
		.def("DisableFontSmooth", &DmSoft::DisableFontSmooth)
		.def("CheckFontSmooth", &DmSoft::CheckFontSmooth)
		.def("SetDisplayAcceler", &DmSoft::SetDisplayAcceler)
		.def("FindWindowByProcess", &DmSoft::FindWindowByProcess)
		.def("FindWindowByProcessId", &DmSoft::FindWindowByProcessId)
		.def("ReadIni", &DmSoft::ReadIni)
		.def("WriteIni", &DmSoft::WriteIni)
		.def("RunApp", &DmSoft::RunApp)
		.def("delay", &DmSoft::delay)
		.def("FindWindowSuper", &DmSoft::FindWindowSuper)
		.def("ExcludePos", &DmSoft::ExcludePos)
		.def("FindNearestPos", &DmSoft::FindNearestPos)
		.def("SortPosDistance", &DmSoft::SortPosDistance)
		.def("FindPicMem", &DmSoft::_FindPicMem_, raw(_2))
		.def("FindPicMemEx", &DmSoft::FindPicMemEx)
		.def("FindPicMemE", &DmSoft::FindPicMemE)
		.def("AppendPicAddr", &DmSoft::AppendPicAddr)
		.def("WriteFile", &DmSoft::WriteFile)
		.def("Stop", &DmSoft::Stop)
		.def("SetDictMem", &DmSoft::SetDictMem)
		.def("GetNetTimeSafe", &DmSoft::GetNetTimeSafe)
		.def("ForceUnBindWindow", &DmSoft::ForceUnBindWindow)
		.def("ReadIniPwd", &DmSoft::ReadIniPwd)
		.def("WriteIniPwd", &DmSoft::WriteIniPwd)
		.def("DecodeFile", &DmSoft::DecodeFile)
		.def("KeyDownChar", &DmSoft::KeyDownChar)
		.def("KeyUpChar", &DmSoft::KeyUpChar)
		.def("KeyPressChar", &DmSoft::KeyPressChar)
		.def("KeyPressStr", &DmSoft::KeyPressStr)
		.def("EnableKeypadPatch", &DmSoft::EnableKeypadPatch)
		.def("EnableKeypadSync", &DmSoft::EnableKeypadSync)
		.def("EnableMouseSync", &DmSoft::EnableMouseSync)
		.def("DmGuard", &DmSoft::DmGuard)
		.def("FaqCaptureFromFile", &DmSoft::FaqCaptureFromFile)
		.def("FindIntEx", &DmSoft::FindIntEx)
		.def("FindFloatEx", &DmSoft::FindFloatEx)
		.def("FindDoubleEx", &DmSoft::FindDoubleEx)
		.def("FindStringEx", &DmSoft::FindStringEx)
		.def("FindDataEx", &DmSoft::FindDataEx)
		.def("EnableRealMouse", &DmSoft::EnableRealMouse)
		.def("EnableRealKeypad", &DmSoft::EnableRealKeypad)
		.def("SendStringIme", &DmSoft::SendStringIme)
		.def("FoobarDrawLine", &DmSoft::FoobarDrawLine)
		.def("FindStrEx", &DmSoft::FindStrEx)
		.def("IsBind", &DmSoft::IsBind)
		.def("SetDisplayDelay", &DmSoft::SetDisplayDelay)
		.def("GetDmCount", &DmSoft::GetDmCount)
		.def("DisableScreenSave", &DmSoft::DisableScreenSave)
		.def("DisablePowerSave", &DmSoft::DisablePowerSave)
		.def("SetMemoryHwndAsProcessId", &DmSoft::SetMemoryHwndAsProcessId)
		.def("FindShape", &DmSoft::_FindShape_, raw(_2))
		.def("FindShapeE", &DmSoft::FindShapeE)
		.def("FindShapeEx", &DmSoft::FindShapeEx)
		.def("FindStrS", &DmSoft::_FindStrS_, raw(_2))
		.def("FindStrExS", &DmSoft::FindStrExS)
		.def("FindStrFastS", &DmSoft::_FindStrFastS_, raw(_2))
		.def("FindStrFastExS", &DmSoft::FindStrFastExS)
		.def("FindPicS", &DmSoft::_FindPicS_, raw(_2))
		.def("FindPicExS", &DmSoft::FindPicExS)
		.def("ClearDict", &DmSoft::ClearDict)
		.def("GetMachineCodeNoMac", &DmSoft::GetMachineCodeNoMac)
		.def("GetClientRect", &DmSoft::_GetClientRect_)
		.def("EnableFakeActive", &DmSoft::EnableFakeActive)
		.def("GetScreenDataBmp", &DmSoft::_GetScreenDataBmp_)
		.def("EncodeFile", &DmSoft::EncodeFile)
		.def("GetCursorShapeEx", &DmSoft::GetCursorShapeEx)
		.def("FaqCancel", &DmSoft::FaqCancel)
		.def("IntToData", &DmSoft::IntToData)
		.def("FloatToData", &DmSoft::FloatToData)
		.def("DoubleToData", &DmSoft::DoubleToData)
		.def("StringToData", &DmSoft::StringToData)
		.def("SetMemoryFindResultToFile", &DmSoft::SetMemoryFindResultToFile)
		.def("EnableBind", &DmSoft::EnableBind)
		.def("SetSimMode", &DmSoft::SetSimMode)
		.def("LockMouseRect", &DmSoft::LockMouseRect)
		.def("SendPaste", &DmSoft::SendPaste)
		.def("IsDisplayDead", &DmSoft::IsDisplayDead)
		.def("GetKeyState", &DmSoft::GetKeyState)
		.def("CopyFile", &DmSoft::CopyFile)
		.def("IsFileExist", &DmSoft::IsFileExist)
		.def("DeleteFile", &DmSoft::DeleteFile)
		.def("MoveFile", &DmSoft::MoveFile)
		.def("CreateFolder", &DmSoft::CreateFolder)
		.def("DeleteFolder", &DmSoft::DeleteFolder)
		.def("GetFileLength", &DmSoft::GetFileLength)
		.def("ReadFile", &DmSoft::ReadFile)
		.def("WaitKey", &DmSoft::WaitKey)
		.def("DeleteIni", &DmSoft::DeleteIni)
		.def("DeleteIniPwd", &DmSoft::DeleteIniPwd)
		.def("EnableSpeedDx", &DmSoft::EnableSpeedDx)
		.def("EnableIme", &DmSoft::EnableIme)
		.def("Reg", &DmSoft::Reg)
		.def("SelectFile", &DmSoft::SelectFile)
		.def("SelectDirectory", &DmSoft::SelectDirectory)
		.def("LockDisplay", &DmSoft::LockDisplay)
		.def("FoobarSetSave", &DmSoft::FoobarSetSave)
		.def("EnumWindowSuper", &DmSoft::EnumWindowSuper)
		.def("DownloadFile", &DmSoft::DownloadFile)
		.def("EnableKeypadMsg", &DmSoft::EnableKeypadMsg)
		.def("EnableMouseMsg", &DmSoft::EnableMouseMsg)
		.def("RegNoMac", &DmSoft::RegNoMac)
		.def("RegExNoMac", &DmSoft::RegExNoMac)
		.def("SetEnumWindowDelay", &DmSoft::SetEnumWindowDelay)
		.def("FindMulColor", &DmSoft::FindMulColor)
		.def("GetDict", &DmSoft::GetDict)
		.def("GetBindWindow", &DmSoft::GetBindWindow)
		.def("FoobarStartGif", &DmSoft::FoobarStartGif)
		.def("FoobarStopGif", &DmSoft::FoobarStopGif)
		.def("FreeProcessMemory", &DmSoft::FreeProcessMemory)
		.def("ReadFileData", &DmSoft::ReadFileData)
		.def("VirtualAllocEx", &DmSoft::VirtualAllocEx)
		.def("VirtualFreeEx", &DmSoft::VirtualFreeEx)
		.def("GetCommandLine", &DmSoft::GetCommandLine)
		.def("TerminateProcess", &DmSoft::TerminateProcess)
		.def("GetNetTimeByIp", &DmSoft::GetNetTimeByIp)
		.def("EnumProcess", &DmSoft::EnumProcess)
		.def("GetProcessInfo", &DmSoft::GetProcessInfo)
		.def("ReadIntAddr", &DmSoft::ReadIntAddr)
		.def("ReadDataAddr", &DmSoft::ReadDataAddr)
		.def("ReadDoubleAddr", &DmSoft::ReadDoubleAddr)
		.def("ReadFloatAddr", &DmSoft::ReadFloatAddr)
		.def("ReadStringAddr", &DmSoft::ReadStringAddr)
		.def("WriteDataAddr", &DmSoft::WriteDataAddr)
		.def("WriteDoubleAddr", &DmSoft::WriteDoubleAddr)
		.def("WriteFloatAddr", &DmSoft::WriteFloatAddr)
		.def("WriteIntAddr", &DmSoft::WriteIntAddr)
		.def("WriteStringAddr", &DmSoft::WriteStringAddr)
		.def("Delays", &DmSoft::Delays)
		.def("FindColorBlock", &DmSoft::_FindColorBlock_, raw(_2))
		.def("FindColorBlockEx", &DmSoft::FindColorBlockEx)
		.def("OpenProcess", &DmSoft::OpenProcess)
		.def("EnumIniSection", &DmSoft::EnumIniSection)
		.def("EnumIniSectionPwd", &DmSoft::EnumIniSectionPwd)
		.def("EnumIniKey", &DmSoft::EnumIniKey)
		.def("EnumIniKeyPwd", &DmSoft::EnumIniKeyPwd)
		.def("SwitchBindWindow", &DmSoft::SwitchBindWindow)
		.def("InitCri", &DmSoft::InitCri)
		.def("SendStringIme2", &DmSoft::SendStringIme2)
		.def("EnumWindowByProcessId", &DmSoft::EnumWindowByProcessId)
		.def("GetDisplayInfo", &DmSoft::GetDisplayInfo)
		.def("EnableFontSmooth", &DmSoft::EnableFontSmooth)
		.def("OcrExOne", &DmSoft::OcrExOne)
		.def("SetAero", &DmSoft::SetAero)
		.def("FoobarSetTrans", &DmSoft::FoobarSetTrans)
		.def("EnablePicCache", &DmSoft::EnablePicCache)
		.def("GetInfo", &DmSoft::GetInfo)
		.def("FaqIsPosted", &DmSoft::FaqIsPosted)
		.def("LoadPicByte", &DmSoft::LoadPicByte)
		.def("MiddleDown", &DmSoft::MiddleDown)
		.def("MiddleUp", &DmSoft::MiddleUp)
		.def("FaqCaptureString", &DmSoft::FaqCaptureString)
		.def("VirtualProtectEx", &DmSoft::VirtualProtectEx)
		.def("SetMouseSpeed", &DmSoft::SetMouseSpeed)
		.def("GetMouseSpeed", &DmSoft::GetMouseSpeed)
		.def("EnableMouseAccuracy", &DmSoft::EnableMouseAccuracy)
	];
}

std::string DmSoft::Ver()
{
	return t2a_(GetImpl_(impl_).Ver());
}

long DmSoft::SetPath(const std::string& path)
{
	return GetImpl_(impl_).SetPath(a2t_(path));
}

std::string DmSoft::Ocr(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).Ocr(x1, y1, x2, y2, a2t_(color), sim));
}

long DmSoft::FindStr(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, long * x, long * y)
{
	return GetImpl_(impl_).FindStr(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, x, y);
}

void DmSoft::_FindStr_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	long x = -1, y = -1;
	auto res = FindStr(x1, y1, x2, y2, str, color, sim, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::GetResultCount(const std::string& str)
{
	return GetImpl_(impl_).GetResultCount(a2t_(str));
}

long DmSoft::GetResultPos(const std::string& str, long index, long * x, long * y)
{
	return GetImpl_(impl_).GetResultPos(a2t_(str), index, x, y);
}

void DmSoft::_GetResultPos_(lua_State* l, const std::string& str, long index)
{
	long x = -1, y = -1;
	auto res = GetResultPos(str, index, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::StrStr(const std::string& s, const std::string& str)
{
	return GetImpl_(impl_).StrStr(a2t_(s), a2t_(str));
}

long DmSoft::SendCommand(const std::string& cmd)
{
	return GetImpl_(impl_).SendCommand(a2t_(cmd));
}

long DmSoft::UseDict(long index)
{
	return GetImpl_(impl_).UseDict(index);
}

std::string DmSoft::GetBasePath()
{
	return t2a_(GetImpl_(impl_).GetBasePath());
}

long DmSoft::SetDictPwd(const std::string& pwd)
{
	return GetImpl_(impl_).SetDictPwd(a2t_(pwd));
}

std::string DmSoft::OcrInFile(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).OcrInFile(x1, y1, x2, y2, a2t_(pic_name), a2t_(color), sim));
}

long DmSoft::Capture(long x1, long y1, long x2, long y2, const std::string& file_name)
{
	return GetImpl_(impl_).Capture(x1, y1, x2, y2, a2t_(file_name));
}

long DmSoft::KeyPress(long vk)
{
	return GetImpl_(impl_).KeyPress(vk);
}

long DmSoft::KeyDown(long vk)
{
	return GetImpl_(impl_).KeyDown(vk);
}

long DmSoft::KeyUp(long vk)
{
	return GetImpl_(impl_).KeyUp(vk);
}

long DmSoft::LeftClick()
{
	return GetImpl_(impl_).LeftClick();
}

long DmSoft::RightClick()
{
	return GetImpl_(impl_).RightClick();
}

long DmSoft::MiddleClick()
{
	return GetImpl_(impl_).MiddleClick();
}

long DmSoft::LeftDoubleClick()
{
	return GetImpl_(impl_).LeftDoubleClick();
}

long DmSoft::LeftDown()
{
	return GetImpl_(impl_).LeftDown();
}

long DmSoft::LeftUp()
{
	return GetImpl_(impl_).LeftUp();
}

long DmSoft::RightDown()
{
	return GetImpl_(impl_).RightDown();
}

long DmSoft::RightUp()
{
	return GetImpl_(impl_).RightUp();
}

long DmSoft::MoveTo(long x, long y)
{
	return GetImpl_(impl_).MoveTo(x, y);
}

long DmSoft::MoveR(long rx, long ry)
{
	return GetImpl_(impl_).MoveR(rx, ry);
}

std::string DmSoft::GetColor(long x, long y)
{
	return t2a_(GetImpl_(impl_).GetColor(x, y));
}

std::string DmSoft::GetColorBGR(long x, long y)
{
	return t2a_(GetImpl_(impl_).GetColorBGR(x, y));
}

std::string DmSoft::RGB2BGR(const std::string& rgb_color)
{
	return t2a_(GetImpl_(impl_).RGB2BGR(a2t_(rgb_color)));
}

std::string DmSoft::BGR2RGB(const std::string& bgr_color)
{
	return t2a_(GetImpl_(impl_).BGR2RGB(a2t_(bgr_color)));
}

long DmSoft::UnBindWindow()
{
	return GetImpl_(impl_).UnBindWindow();
}

long DmSoft::CmpColor(long x, long y, const std::string& color, double sim)
{
	return GetImpl_(impl_).CmpColor(x, y, a2t_(color), sim);
}

long DmSoft::ClientToScreen(long hwnd, long * x, long * y)
{
	return GetImpl_(impl_).ClientToScreen(hwnd, x, y);
}

void DmSoft::_ClientToScreen_(lua_State* l, long hwnd)
{
	long x = -1, y = -1;
	auto res = ClientToScreen(hwnd, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::ScreenToClient(long hwnd, long * x, long * y)
{
	return GetImpl_(impl_).ScreenToClient(hwnd, x, y);
}

void DmSoft::_ScreenToClient_(lua_State* l, long hwnd)
{
	long x = -1, y = -1;
	auto res = ScreenToClient(hwnd, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::ShowScrMsg(long x1, long y1, long x2, long y2, const std::string& msg, const std::string& color)
{
	return GetImpl_(impl_).ShowScrMsg(x1, y1, x2, y2, a2t_(msg), a2t_(color));
}

long DmSoft::SetMinRowGap(long row_gap)
{
	return GetImpl_(impl_).SetMinRowGap(row_gap);
}

long DmSoft::SetMinColGap(long col_gap)
{
	return GetImpl_(impl_).SetMinColGap(col_gap);
}

long DmSoft::FindColor(long x1, long y1, long x2, long y2, const std::string& color, double sim, long dir, long * x, long * y)
{
	return GetImpl_(impl_).FindColor(x1, y1, x2, y2, a2t_(color), sim, dir, x, y);
}

void DmSoft::_FindColor_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& color, double sim, long dir)
{
	long x = -1, y = -1;
	auto res = FindColor(x1, y1, x2, y2, color, sim, dir, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindColorEx(long x1, long y1, long x2, long y2, const std::string& color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindColorEx(x1, y1, x2, y2, a2t_(color), sim, dir));
}

long DmSoft::SetWordLineHeight(long line_height)
{
	return GetImpl_(impl_).SetWordLineHeight(line_height);
}

long DmSoft::SetWordGap(long word_gap)
{
	return GetImpl_(impl_).SetWordLineHeight(word_gap);
}

long DmSoft::SetRowGapNoDict(long row_gap)
{
	return GetImpl_(impl_).SetRowGapNoDict(row_gap);
}

long DmSoft::SetColGapNoDict(long col_gap)
{
	return GetImpl_(impl_).SetColGapNoDict(col_gap);
}

long DmSoft::SetWordLineHeightNoDict(long line_height)
{
	return GetImpl_(impl_).SetWordLineHeightNoDict(line_height);
}

long DmSoft::SetWordGapNoDict(long word_gap)
{
	return GetImpl_(impl_).SetWordGapNoDict(word_gap);
}

long DmSoft::GetWordResultCount(const std::string& str)
{
	return GetImpl_(impl_).GetWordResultCount(a2t_(str));
}

long DmSoft::GetWordResultPos(const std::string& str, long index, long * x, long * y)
{
	return GetImpl_(impl_).GetWordResultPos(a2t_(str), index, x, y);
}

void DmSoft::_GetWordResultPos_(lua_State* l, const std::string& str, long index)
{
	long x = -1, y = -1;
	auto res = GetWordResultPos(str, index, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::GetWordResultStr(const std::string& str, long index)
{
	return t2a_(GetImpl_(impl_).GetWordResultStr(a2t_(str), index));
}

std::string DmSoft::GetWords(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).GetWords(x1, y1, x2, y2, a2t_(color), sim));
}

std::string DmSoft::GetWordsNoDict(long x1, long y1, long x2, long y2, const std::string& color)
{
	return t2a_(GetImpl_(impl_).GetWordsNoDict(x1, y1, x2, y2, a2t_(color)));
}

long DmSoft::SetShowErrorMsg(long show)
{
	return GetImpl_(impl_).SetShowErrorMsg(show);
}

long DmSoft::GetClientSize(long hwnd, long * width, long * height)
{
	return GetImpl_(impl_).GetClientSize(hwnd, width, height);
}

void DmSoft::_GetClientSize_(lua_State* l, long hwnd)
{
	long x = -1, y = -1;
	auto res = GetClientSize(hwnd, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::MoveWindow(long hwnd, long x, long y)
{
	return GetImpl_(impl_).MoveWindow(hwnd, x, y);
}

std::string DmSoft::GetColorHSV(long x, long y)
{
	return t2a_(GetImpl_(impl_).GetColorHSV(x, y));
}

std::string DmSoft::GetAveRGB(long x1, long y1, long x2, long y2)
{
	return t2a_(GetImpl_(impl_).GetAveRGB(x1, y1, x2, y2));
}

std::string DmSoft::GetAveHSV(long x1, long y1, long x2, long y2)
{
	return t2a_(GetImpl_(impl_).GetAveHSV(x1, y1, x2, y2));
}

long DmSoft::GetForegroundWindow()
{
	return GetImpl_(impl_).GetForegroundWindow();
}

long DmSoft::GetForegroundFocus()
{
	return GetImpl_(impl_).GetForegroundFocus();
}

long DmSoft::GetMousePointWindow()
{
	return GetImpl_(impl_).GetMousePointWindow();
}

long DmSoft::GetPointWindow(long x, long y)
{
	return GetImpl_(impl_).GetPointWindow(x, y);
}

std::string DmSoft::EnumWindow(long parent, const std::string& title, const std::string& class_name, long filter)
{
	return t2a_(GetImpl_(impl_).EnumWindow(parent, a2t_(title), a2t_(class_name), filter));
}

long DmSoft::GetWindowState(long hwnd, long flag)
{
	return GetImpl_(impl_).GetWindowState(hwnd, flag);
}

long DmSoft::GetWindow(long hwnd, long flag)
{
	return GetImpl_(impl_).GetWindow(hwnd, flag);
}

long DmSoft::GetSpecialWindow(long flag)
{
	return GetImpl_(impl_).GetSpecialWindow(flag);
}

long DmSoft::SetWindowText(long hwnd, const std::string& text)
{
	return GetImpl_(impl_).SetWindowText(hwnd, a2t_(text));
}

long DmSoft::SetWindowSize(long hwnd, long width, long height)
{
	return GetImpl_(impl_).SetWindowSize(hwnd, width, height);
}

long DmSoft::GetWindowRect(long hwnd, long * x1, long * y1, long * x2, long * y2)
{
	return GetImpl_(impl_).GetWindowRect(hwnd, x1, y1, x2, y2);
}

void DmSoft::_GetWindowRect_(lua_State* l, long hwnd)
{
	long x1 = -1, y1 = -1, x2 = -1, y2 = -1;
	auto res = GetWindowRect(hwnd, &x1, &y1, &x2, &y2);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x1);
	lua_pushinteger(l, y1);
	lua_pushinteger(l, x2);
	lua_pushinteger(l, y2);
}

std::string DmSoft::GetWindowTitle(long hwnd)
{
	return t2a_(GetImpl_(impl_).GetWindowTitle(hwnd));
}

std::string DmSoft::GetWindowClass(long hwnd)
{
	return t2a_(GetImpl_(impl_).GetWindowClass(hwnd));
}

long DmSoft::SetWindowState(long hwnd, long flag)
{
	return GetImpl_(impl_).SetWindowState(hwnd, flag);
}

long DmSoft::CreateFoobarRect(long hwnd, long x, long y, long w, long h)
{
	return GetImpl_(impl_).CreateFoobarRect(hwnd, x, y, w, h);
}

long DmSoft::CreateFoobarRoundRect(long hwnd, long x, long y, long w, long h, long rw, long rh)
{
	return GetImpl_(impl_).CreateFoobarRoundRect(hwnd, x, y, w, h, rw, rh);
}

long DmSoft::CreateFoobarEllipse(long hwnd, long x, long y, long w, long h)
{
	return GetImpl_(impl_).CreateFoobarEllipse(hwnd, x, y, w, h);
}

long DmSoft::CreateFoobarCustom(long hwnd, long x, long y, const std::string& pic, const std::string& trans_color, double sim)
{
	return GetImpl_(impl_).CreateFoobarCustom(hwnd, x, y, a2t_(pic), a2t_(trans_color), sim);
}

long DmSoft::FoobarFillRect(long hwnd, long x1, long y1, long x2, long y2, const std::string& color)
{
	return GetImpl_(impl_).FoobarFillRect(hwnd, x1, y1, x2, y2, a2t_(color));
}

long DmSoft::FoobarDrawText(long hwnd, long x, long y, long w, long h, const std::string& text, const std::string& color, long align)
{
	return GetImpl_(impl_).FoobarDrawText(hwnd, x, y, w, h, a2t_(text), a2t_(color), align);
}

long DmSoft::FoobarDrawPic(long hwnd, long x, long y, const std::string& pic, const std::string& trans_color)
{
	return GetImpl_(impl_).FoobarDrawPic(hwnd, x, y, a2t_(pic), a2t_(trans_color));
}

long DmSoft::FoobarUpdate(long hwnd)
{
	return GetImpl_(impl_).FoobarUpdate(hwnd);
}

long DmSoft::FoobarLock(long hwnd)
{
	return GetImpl_(impl_).FoobarLock(hwnd);
}

long DmSoft::FoobarUnlock(long hwnd)
{
	return GetImpl_(impl_).FoobarUnlock(hwnd);
}

long DmSoft::FoobarSetFont(long hwnd, const std::string& font_name, long size, long flag)
{
	return GetImpl_(impl_).FoobarSetFont(hwnd, a2t_(font_name), size, flag);
}

long DmSoft::FoobarTextRect(long hwnd, long x, long y, long w, long h)
{
	return GetImpl_(impl_).FoobarTextRect(hwnd, x, y, w, h);
}

long DmSoft::FoobarPrintText(long hwnd, const std::string& text, const std::string& color)
{
	return GetImpl_(impl_).FoobarPrintText(hwnd, a2t_(text), a2t_(color));
}

long DmSoft::FoobarClearText(long hwnd)
{
	return GetImpl_(impl_).FoobarClearText(hwnd);
}

long DmSoft::FoobarTextLineGap(long hwnd, long gap)
{
	return GetImpl_(impl_).FoobarTextLineGap(hwnd, gap);
}

long DmSoft::Play(const std::string& file_name)
{
	return GetImpl_(impl_).Play(a2t_(file_name));
}

long DmSoft::FaqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time)
{
	return GetImpl_(impl_).FaqCapture(x1, y1, x2, y2, quality, delay, time);
}

long DmSoft::FaqRelease(long handle)
{
	return GetImpl_(impl_).FaqRelease(handle);
}

std::string DmSoft::FaqSend(const std::string& server, long handle, long request_type, long time_out)
{
	return t2a_(GetImpl_(impl_).FaqSend(a2t_(server), handle, request_type, time_out));
}

long DmSoft::Beep(long fre, long delay)
{
	return GetImpl_(impl_).Beep(fre, delay);
}

long DmSoft::FoobarClose(long hwnd)
{
	return GetImpl_(impl_).FoobarClose(hwnd);
}

long DmSoft::MoveDD(long dx, long dy)
{
	return GetImpl_(impl_).MoveDD(dx, dy);
}

long DmSoft::FaqGetSize(long handle)
{
	return GetImpl_(impl_).FaqGetSize(handle);
}

long DmSoft::LoadPic(const std::string& pic_name)
{
	return GetImpl_(impl_).LoadPic(a2t_(pic_name));
}

long DmSoft::FreePic(const std::string& pic_name)
{
	return GetImpl_(impl_).FreePic(a2t_(pic_name));
}

long DmSoft::GetScreenData(long x1, long y1, long x2, long y2)
{
	return GetImpl_(impl_).GetScreenData(x1, y1, x2, y2);
}

long DmSoft::FreeScreenData(long handle)
{
	return GetImpl_(impl_).FreeScreenData(handle);
}

long DmSoft::WheelUp()
{
	return GetImpl_(impl_).WheelUp();
}

long DmSoft::WheelDown()
{
	return GetImpl_(impl_).WheelDown();
}

long DmSoft::SetMouseDelay(const std::string& tpe, long delay)
{
	return GetImpl_(impl_).SetMouseDelay(a2t_(tpe), delay);
}

long DmSoft::SetKeypadDelay(const std::string& tpe, long delay)
{
	return GetImpl_(impl_).SetKeypadDelay(a2t_(tpe), delay);
}

std::string DmSoft::GetEnv(long index, const std::string& name)
{
	return t2a_(GetImpl_(impl_).GetEnv(index, a2t_(name)));
}

long DmSoft::SetEnv(long index, const std::string& name, const std::string& value)
{
	return GetImpl_(impl_).SetEnv(index, a2t_(name), a2t_(value));
}

long DmSoft::SendString(long hwnd, const std::string& str)
{
	return GetImpl_(impl_).SendString(hwnd, a2t_(str));
}

long DmSoft::DelEnv(long index, const std::string& name)
{
	return GetImpl_(impl_).DelEnv(index, a2t_(name));
}

std::string DmSoft::GetPath()
{
	return t2a_(GetImpl_(impl_).GetPath());
}

long DmSoft::SetDict(long index, const std::string& dict_name)
{
	return GetImpl_(impl_).SetDict(index, a2t_(dict_name));
}

long DmSoft::FindPic(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir, long * x, long * y)
{
	return GetImpl_(impl_).FindPic(x1, y1, x2, y2, a2t_(pic_name), a2t_(delta_color), sim, dir, x, y);
}

void DmSoft::_FindPic_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir)
{
	long x = -1, y = -1;
	auto res = FindPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindPicEx(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindPicEx(x1, y1, x2, y2, a2t_(pic_name), a2t_(delta_color), sim, dir));
}

long DmSoft::SetClientSize(long hwnd, long width, long height)
{
	return GetImpl_(impl_).SetClientSize(hwnd, width, height);
}

long DmSoft::ReadInt(long hwnd, const std::string& addr, long tpe)
{
	return GetImpl_(impl_).ReadInt(hwnd, a2t_(addr), tpe);
}

float DmSoft::ReadFloat(long hwnd, const std::string& addr)
{
	return GetImpl_(impl_).ReadFloat(hwnd, a2t_(addr));
}

double DmSoft::ReadDouble(long hwnd, const std::string& addr)
{
	return GetImpl_(impl_).ReadDouble(hwnd, a2t_(addr));
}

std::string DmSoft::FindInt(long hwnd, const std::string& addr_range, long int_value_min, long int_value_max, long tpe)
{
	return t2a_(GetImpl_(impl_).FindInt(hwnd, a2t_(addr_range), int_value_min, int_value_max, tpe));
}

std::string DmSoft::FindFloat(long hwnd, const std::string& addr_range, float float_value_min, float float_value_max)
{
	return t2a_(GetImpl_(impl_).FindFloat(hwnd, a2t_(addr_range), float_value_min, float_value_max));
}

std::string DmSoft::FindDouble(long hwnd, const std::string& addr_range, double double_value_min, double double_value_max)
{
	return t2a_(GetImpl_(impl_).FindDouble(hwnd, a2t_(addr_range), double_value_min, double_value_max));
}

std::string DmSoft::FindString(long hwnd, const std::string& addr_range, const std::string& string_value, long tpe)
{
	return t2a_(GetImpl_(impl_).FindString(hwnd, a2t_(addr_range), a2t_(string_value), tpe));
}

long DmSoft::GetModuleBaseAddr(long hwnd, const std::string& module_name)
{
	return GetImpl_(impl_).GetModuleBaseAddr(hwnd, a2t_(module_name));
}

std::string DmSoft::MoveToEx(long x, long y, long w, long h)
{
	return t2a_(GetImpl_(impl_).MoveToEx(x, y, w, h));
}

std::string DmSoft::MatchPicName(const std::string& pic_name)
{
	return t2a_(GetImpl_(impl_).MatchPicName(a2t_(pic_name)));
}

long DmSoft::AddDict(long index, const std::string& dict_info)
{
	return GetImpl_(impl_).AddDict(index, a2t_(dict_info));
}

long DmSoft::EnterCri()
{
	return GetImpl_(impl_).EnterCri();
}

long DmSoft::LeaveCri()
{
	return GetImpl_(impl_).EnterCri();
}

long DmSoft::WriteInt(long hwnd, const std::string& addr, long tpe, long v)
{
	return GetImpl_(impl_).WriteInt(hwnd, a2t_(addr), tpe, v);
}

long DmSoft::WriteFloat(long hwnd, const std::string& addr, float v)
{
	return GetImpl_(impl_).WriteFloat(hwnd, a2t_(addr), v);
}

long DmSoft::WriteDouble(long hwnd, const std::string& addr, double v)
{
	return GetImpl_(impl_).WriteDouble(hwnd, a2t_(addr), v);
}

long DmSoft::WriteString(long hwnd, const std::string& addr, long tpe, const std::string& v)
{
	return GetImpl_(impl_).WriteString(hwnd, a2t_(addr), tpe, a2t_(v));
}

long DmSoft::AsmAdd(const std::string& asm_ins)
{
	return GetImpl_(impl_).AsmAdd(a2t_(asm_ins));
}

long DmSoft::AsmClear()
{
	return GetImpl_(impl_).AsmClear();
}

long DmSoft::AsmCall(long hwnd, long mode)
{
	return GetImpl_(impl_).AsmCall(hwnd, mode);
}

long DmSoft::FindMultiColor(long x1, long y1, long x2, long y2, const std::string& first_color, const std::string& offset_color, double sim, long dir, long * x, long * y)
{
	return GetImpl_(impl_).FindMultiColor(x1, y1, x2, y2, a2t_(first_color), a2t_(offset_color), sim, dir, x, y);
}

void DmSoft::_FindMultiColor_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& first_color, const std::string& offset_color, double sim, long dir)
{
	long x = -1, y = -1;
	auto res = FindMultiColor(x1, y1, x2, y2, first_color, offset_color, sim, dir, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindMultiColorEx(long x1, long y1, long x2, long y2, const std::string& first_color, const std::string& offset_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindMultiColorEx(x1, y1, x2, y2, a2t_(first_color), a2t_(offset_color), sim, dir));
}

std::string DmSoft::AsmCode(long base_addr)
{
	return t2a_(GetImpl_(impl_).AsmCode(base_addr));
}

std::string DmSoft::Assemble(const std::string& asm_code, long base_addr, long is_upper)
{
	return t2a_(GetImpl_(impl_).Assemble(a2t_(asm_code), base_addr, is_upper));
}

long DmSoft::SetWindowTransparent(long hwnd, long v)
{
	return GetImpl_(impl_).SetWindowTransparent(hwnd, v);
}

std::string DmSoft::ReadData(long hwnd, const std::string& addr, long length)
{
	return t2a_(GetImpl_(impl_).ReadData(hwnd, a2t_(addr), length));
}

long DmSoft::WriteData(long hwnd, const std::string& addr, const std::string& data)
{
	return GetImpl_(impl_).WriteData(hwnd, a2t_(addr), a2t_(data));
}

std::string DmSoft::FindData(long hwnd, const std::string& addr_range, const std::string& data)
{
	return t2a_(GetImpl_(impl_).FindData(hwnd, a2t_(addr_range), a2t_(data)));
}

long DmSoft::SetPicPwd(const std::string& pwd)
{
	return GetImpl_(impl_).SetPicPwd(a2t_(pwd));
}

long DmSoft::Log(const std::string& info)
{
	return GetImpl_(impl_).Log(a2t_(info));
}

std::string DmSoft::FindStrE(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrE(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

std::string DmSoft::FindColorE(long x1, long y1, long x2, long y2, const std::string& color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindColorE(x1, y1, x2, y2, a2t_(color), sim, dir));
}

std::string DmSoft::FindPicE(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindPicE(x1, y1, x2, y2, a2t_(pic_name), a2t_(delta_color), sim, dir));
}

std::string DmSoft::FindMultiColorE(long x1, long y1, long x2, long y2, const std::string& first_color, const std::string& offset_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindMultiColorE(x1, y1, x2, y2, a2t_(first_color), a2t_(offset_color), sim, dir));
}

long DmSoft::SetExactOcr(long exact_ocr)
{
	return GetImpl_(impl_).SetExactOcr(exact_ocr);
}

std::string DmSoft::ReadString(long hwnd, const std::string& addr, long tpe, long length)
{
	return t2a_(GetImpl_(impl_).ReadString(hwnd, a2t_(addr), tpe, length));
}

long DmSoft::FoobarTextPrintDir(long hwnd, long dir)
{
	return GetImpl_(impl_).FoobarTextPrintDir(hwnd, dir);
}

std::string DmSoft::OcrEx(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).OcrEx(x1, y1, x2, y2, a2t_(color), sim));
}

long DmSoft::SetDisplayInput(const std::string& mode)
{
	return GetImpl_(impl_).SetDisplayInput(a2t_(mode));
}

long DmSoft::GetTime()
{
	return GetImpl_(impl_).GetTime();
}

long DmSoft::GetScreenWidth()
{
	return GetImpl_(impl_).GetScreenWidth();
}

long DmSoft::GetScreenHeight()
{
	return GetImpl_(impl_).GetScreenHeight();
}

long DmSoft::BindWindowEx(long hwnd, const std::string& display, const std::string& mouse, const std::string& keypad, const std::string& public_desc, long mode)
{
	return GetImpl_(impl_).BindWindowEx(hwnd, a2t_(display), a2t_(mouse), a2t_(keypad), a2t_(public_desc), mode);
}

std::string DmSoft::GetDiskSerial()
{
	return t2a_(GetImpl_(impl_).GetDiskSerial());
}

std::string DmSoft::Md5(const std::string& str)
{
	return t2a_(GetImpl_(impl_).Md5(a2t_(str)));
}

std::string DmSoft::GetMac()
{
	return t2a_(GetImpl_(impl_).GetMac());
}

long DmSoft::ActiveInputMethod(long hwnd, const std::string& id)
{
	return GetImpl_(impl_).ActiveInputMethod(hwnd, a2t_(id));
}

long DmSoft::CheckInputMethod(long hwnd, const std::string& id)
{
	return GetImpl_(impl_).CheckInputMethod(hwnd, a2t_(id));
}

long DmSoft::FindInputMethod(const std::string& id)
{
	return GetImpl_(impl_).FindInputMethod(a2t_(id));
}

long DmSoft::GetCursorPos(long * x, long * y)
{
	return GetImpl_(impl_).GetCursorPos(x, y);
}

void DmSoft::_GetCursorPos_(lua_State* l)
{
	long x = -1, y = -1;
	auto res = GetCursorPos(&x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

long DmSoft::BindWindow(long hwnd, const std::string& display, const std::string& mouse, const std::string& keypad, long mode)
{
	return GetImpl_(impl_).BindWindow(hwnd, a2t_(display), a2t_(mouse), a2t_(keypad), mode);
}

long DmSoft::FindWindow(const std::string& class_name, const std::string& title_name)
{
	return GetImpl_(impl_).FindWindow(a2t_(class_name), a2t_(title_name));
}

long DmSoft::GetScreenDepth()
{
	return GetImpl_(impl_).GetScreenDepth();
}

long DmSoft::SetScreen(long width, long height, long depth)
{
	return GetImpl_(impl_).SetScreen(width, height, depth);
}

long DmSoft::ExitOs(long tpe)
{
	return GetImpl_(impl_).ExitOs(tpe);
}

std::string DmSoft::GetDir(long tpe)
{
	return t2a_(GetImpl_(impl_).GetDir(tpe));
}

long DmSoft::GetOsType()
{
	return GetImpl_(impl_).GetOsType();
}

long DmSoft::FindWindowEx(long parent, const std::string& class_name, const std::string& title_name)
{
	return GetImpl_(impl_).FindWindowEx(parent, a2t_(class_name), a2t_(title_name));
}

long DmSoft::SetExportDict(long index, const std::string& dict_name)
{
	return GetImpl_(impl_).SetExportDict(index, a2t_(dict_name));
}

std::string DmSoft::GetCursorShape()
{
	return t2a_(GetImpl_(impl_).GetCursorShape());
}

long DmSoft::DownCpu(long rate)
{
	return GetImpl_(impl_).DownCpu(rate);
}

std::string DmSoft::GetCursorSpot()
{
	return t2a_(GetImpl_(impl_).GetCursorSpot());
}

long DmSoft::SendString2(long hwnd, const std::string& str)
{
	return GetImpl_(impl_).SendString2(hwnd, a2t_(str));
}

long DmSoft::FaqPost(const std::string& server, long handle, long request_type, long time_out)
{
	return GetImpl_(impl_).FaqPost(a2t_(server), handle, request_type, time_out);
}

std::string DmSoft::FaqFetch()
{
	return t2a_(GetImpl_(impl_).FaqFetch());
}

std::string DmSoft::FetchWord(long x1, long y1, long x2, long y2, const std::string& color, const std::string& word)
{
	return t2a_(GetImpl_(impl_).FetchWord(x1, y1, x2, y2, a2t_(color), a2t_(word)));
}

long DmSoft::CaptureJpg(long x1, long y1, long x2, long y2, const std::string& file_name, long quality)
{
	return GetImpl_(impl_).CaptureJpg(x1, y1, x2, y2, a2t_(file_name), quality);
}

long DmSoft::FindStrWithFont(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, const std::string& font_name, long font_size, long flag, long * x, long * y)
{
	return GetImpl_(impl_).FindStrWithFont(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, a2t_(font_name), font_size, flag, x, y);
}

void DmSoft::_FindStrWithFont_(const luabind::object& x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, const std::string& font_name, long font_size, long flag)
{
	long x = -1, y = -1;
	auto l = x1.interpreter();
	auto res = FindStrWithFont(luabind::object_cast<long>(x1), y1, x2, y2, str, color, sim, font_name, font_size, flag, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindStrWithFontE(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, const std::string& font_name, long font_size, long flag)
{
	return t2a_(GetImpl_(impl_).FindStrWithFontE(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, a2t_(font_name), font_size, flag));
}

std::string DmSoft::FindStrWithFontEx(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, const std::string& font_name, long font_size, long flag)
{
	return t2a_(GetImpl_(impl_).FindStrWithFontEx(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, a2t_(font_name), font_size, flag));
}

std::string DmSoft::GetDictInfo(const std::string& str, const std::string& font_name, long font_size, long flag)
{
	return t2a_(GetImpl_(impl_).GetDictInfo(a2t_(str), a2t_(font_name), font_size, flag));
}

long DmSoft::SaveDict(long index, const std::string& file_name)
{
	return GetImpl_(impl_).SaveDict(index, a2t_(file_name));
}

long DmSoft::GetWindowProcessId(long hwnd)
{
	return GetImpl_(impl_).GetWindowProcessId(hwnd);
}

std::string DmSoft::GetWindowProcessPath(long hwnd)
{
	return t2a_(GetImpl_(impl_).GetWindowProcessPath(hwnd));
}

long DmSoft::LockInput(long locks)
{
	return GetImpl_(impl_).LockInput(locks);
}

std::string DmSoft::GetPicSize(const std::string& pic_name)
{
	return t2a_(GetImpl_(impl_).GetPicSize(a2t_(pic_name)));
}

long DmSoft::GetID()
{
	return GetImpl_(impl_).GetID();
}

long DmSoft::CapturePng(long x1, long y1, long x2, long y2, const std::string& file_name)
{
	return GetImpl_(impl_).CapturePng(x1, y1, x2, y2, a2t_(file_name));
}

long DmSoft::CaptureGif(long x1, long y1, long x2, long y2, const std::string& file_name, long delay, long time)
{
	return GetImpl_(impl_).CaptureGif(x1, y1, x2, y2, a2t_(file_name), delay, time);
}

long DmSoft::ImageToBmp(const std::string& pic_name, const std::string& bmp_name)
{
	return GetImpl_(impl_).ImageToBmp(a2t_(pic_name), a2t_(bmp_name));
}

long DmSoft::FindStrFast(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, long * x, long * y)
{
	return GetImpl_(impl_).FindStrFast(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, x, y);
}

void DmSoft::_FindStrFast_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	long x = -1, y = -1;
	auto res = FindStrFast(x1, y1, x2, y2, str, color, sim, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindStrFastEx(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrFastEx(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

std::string DmSoft::FindStrFastE(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrFastE(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

long DmSoft::EnableDisplayDebug(long enable_debug)
{
	return GetImpl_(impl_).EnableDisplayDebug(enable_debug);
}

long DmSoft::CapturePre(const std::string& file_name)
{
	return GetImpl_(impl_).CapturePre(a2t_(file_name));
}

long DmSoft::RegEx(const std::string& code, const std::string& Ver, const std::string& ip)
{
	return GetImpl_(impl_).RegEx(a2t_(code), a2t_(Ver), a2t_(ip));
}

std::string DmSoft::GetMachineCode()
{
	return t2a_(GetImpl_(impl_).GetMachineCode());
}

long DmSoft::SetClipboard(const std::string& data)
{
	return GetImpl_(impl_).SetClipboard(a2t_(data));
}

std::string DmSoft::GetClipboard()
{
	return t2a_(GetImpl_(impl_).GetClipboard());
}

long DmSoft::GetNowDict()
{
	return GetImpl_(impl_).GetNowDict();
}

long DmSoft::Is64Bit()
{
	return GetImpl_(impl_).Is64Bit();
}

long DmSoft::GetColorNum(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return GetImpl_(impl_).GetColorNum(x1, y1, x2, y2, a2t_(color), sim);
}

std::string DmSoft::EnumWindowByProcess(const std::string& process_name, const std::string& title, const std::string& class_name, long filter)
{
	return t2a_(GetImpl_(impl_).EnumWindowByProcess(a2t_(process_name), a2t_(title), a2t_(class_name), filter));
}

long DmSoft::GetDictCount(long index)
{
	return GetImpl_(impl_).GetDictCount(index);
}

long DmSoft::GetLastError()
{
	return GetImpl_(impl_).GetLastError();
}

std::string DmSoft::GetNetTime()
{
	return t2a_(GetImpl_(impl_).GetNetTime());
}

long DmSoft::EnableGetColorByCapture(long en)
{
	return GetImpl_(impl_).EnableGetColorByCapture(en);
}

long DmSoft::CheckUAC()
{
	return GetImpl_(impl_).CheckUAC();
}

long DmSoft::SetUAC(long uac)
{
	return GetImpl_(impl_).SetUAC(uac);
}

long DmSoft::DisableFontSmooth()
{
	return GetImpl_(impl_).DisableFontSmooth();
}

long DmSoft::CheckFontSmooth()
{
	return GetImpl_(impl_).CheckFontSmooth();
}

long DmSoft::SetDisplayAcceler(long level)
{
	return GetImpl_(impl_).SetDisplayAcceler(level);
}

long DmSoft::FindWindowByProcess(const std::string& process_name, const std::string& class_name, const std::string& title_name)
{
	return GetImpl_(impl_).FindWindowByProcess(a2t_(process_name), a2t_(class_name), a2t_(title_name));
}

long DmSoft::FindWindowByProcessId(long process_id, const std::string& class_name, const std::string& title_name)
{
	return GetImpl_(impl_).FindWindowByProcessId(process_id, a2t_(class_name), a2t_(title_name));
}

std::string DmSoft::ReadIni(const std::string& section, const std::string& key, const std::string& file_name)
{
	return t2a_(GetImpl_(impl_).ReadIni(a2t_(section), a2t_(key), a2t_(file_name)));
}

long DmSoft::WriteIni(const std::string& section, const std::string& key, const std::string& v, const std::string& file_name)
{
	return GetImpl_(impl_).WriteIni(a2t_(section), a2t_(key), a2t_(v), a2t_(file_name));
}

long DmSoft::RunApp(const std::string& path, long mode)
{
	return GetImpl_(impl_).RunApp(a2t_(path), mode);
}

long DmSoft::delay(long mis)
{
	return GetImpl_(impl_).delay(mis);
}

long DmSoft::FindWindowSuper(const std::string& spec1, long flag1, long type1, const std::string& spec2, long flag2, long type2)
{
	return GetImpl_(impl_).FindWindowSuper(a2t_(spec1), flag1, type1, a2t_(spec2), flag2, type2);
}

std::string DmSoft::ExcludePos(const std::string& all_pos, long tpe, long x1, long y1, long x2, long y2)
{
	return t2a_(GetImpl_(impl_).ExcludePos(a2t_(all_pos), tpe, x1, y1, x2, y2));
}

std::string DmSoft::FindNearestPos(const std::string& all_pos, long tpe, long x, long y)
{
	return t2a_(GetImpl_(impl_).FindNearestPos(a2t_(all_pos), tpe, x, y));
}

std::string DmSoft::SortPosDistance(const std::string& all_pos, long tpe, long x, long y)
{
	return t2a_(GetImpl_(impl_).SortPosDistance(a2t_(all_pos), tpe, x, y));
}

long DmSoft::FindPicMem(long x1, long y1, long x2, long y2, const std::string& pic_info, const std::string& delta_color, double sim, long dir, long * x, long * y)
{
	return GetImpl_(impl_).FindPicMem(x1, y1, x2, y2, a2t_(pic_info), a2t_(delta_color), sim, dir, x, y);
}

void DmSoft::_FindPicMem_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& pic_info, const std::string& delta_color, double sim, long dir)
{
	long x = -1, y = -1;
	auto res = FindPicMem(x1, y1, x2, y2, pic_info, delta_color, sim, dir, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindPicMemEx(long x1, long y1, long x2, long y2, const std::string& pic_info, const std::string& delta_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindPicMemEx(x1, y1, x2, y2, a2t_(pic_info), a2t_(delta_color), sim, dir));
}

std::string DmSoft::FindPicMemE(long x1, long y1, long x2, long y2, const std::string& pic_info, const std::string& delta_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindPicMemE(x1, y1, x2, y2, a2t_(pic_info), a2t_(delta_color), sim, dir));
}

std::string DmSoft::AppendPicAddr(const std::string& pic_info, long addr, long size)
{
	return t2a_(GetImpl_(impl_).AppendPicAddr(a2t_(pic_info), addr, size));
}

long DmSoft::WriteFile(const std::string& file_name, const std::string& content)
{
	return GetImpl_(impl_).WriteFile(a2t_(file_name), a2t_(content));
}

long DmSoft::Stop(long id)
{
	return GetImpl_(impl_).Stop(id);
}

long DmSoft::SetDictMem(long index, long addr, long size)
{
	return GetImpl_(impl_).SetDictMem(index, addr, size);
}

std::string DmSoft::GetNetTimeSafe()
{
	return t2a_(GetImpl_(impl_).GetNetTimeSafe());
}

long DmSoft::ForceUnBindWindow(long hwnd)
{
	return GetImpl_(impl_).ForceUnBindWindow(hwnd);
}

std::string DmSoft::ReadIniPwd(const std::string& section, const std::string& key, const std::string& file_name, const std::string& pwd)
{
	return t2a_(GetImpl_(impl_).ReadIniPwd(a2t_(section), a2t_(key), a2t_(file_name), a2t_(pwd)));
}

long DmSoft::WriteIniPwd(const std::string& section, const std::string& key, const std::string& v, const std::string& file_name, const std::string& pwd)
{
	return GetImpl_(impl_).WriteIniPwd(a2t_(section), a2t_(key), a2t_(v), a2t_(file_name), a2t_(pwd));
}

long DmSoft::DecodeFile(const std::string& file_name, const std::string& pwd)
{
	return GetImpl_(impl_).DecodeFile(a2t_(file_name), a2t_(pwd));
}

long DmSoft::KeyDownChar(const std::string& key_str)
{
	return GetImpl_(impl_).KeyDownChar(a2t_(key_str));
}

long DmSoft::KeyUpChar(const std::string& key_str)
{
	return GetImpl_(impl_).KeyUpChar(a2t_(key_str));
}

long DmSoft::KeyPressChar(const std::string& key_str)
{
	return GetImpl_(impl_).KeyPressChar(a2t_(key_str));
}

long DmSoft::KeyPressStr(const std::string& key_str, long delay)
{
	return GetImpl_(impl_).KeyPressStr(a2t_(key_str), delay);
}

long DmSoft::EnableKeypadPatch(long en)
{
	return GetImpl_(impl_).EnableKeypadPatch(en);
}

long DmSoft::EnableKeypadSync(long en, long time_out)
{
	return GetImpl_(impl_).EnableKeypadSync(en, time_out);
}

long DmSoft::EnableMouseSync(long en, long time_out)
{
	return GetImpl_(impl_).EnableMouseSync(en, time_out);
}

long DmSoft::DmGuard(long en, const std::string& tpe)
{
	return GetImpl_(impl_).DmGuard(en, a2t_(tpe));
}

long DmSoft::FaqCaptureFromFile(long x1, long y1, long x2, long y2, const std::string& file_name, long quality)
{
	return GetImpl_(impl_).FaqCaptureFromFile(x1, y1, x2, y2, a2t_(file_name), quality);
}

std::string DmSoft::FindIntEx(long hwnd, const std::string& addr_range, long int_value_min, long int_value_max, long tpe, long steps, long multi_thread, long mode)
{
	return t2a_(GetImpl_(impl_).FindIntEx(hwnd, a2t_(addr_range), int_value_min, int_value_max, tpe, steps, multi_thread, mode));
}

std::string DmSoft::FindFloatEx(long hwnd, const std::string& addr_range, float float_value_min, float float_value_max, long steps, long multi_thread, long mode)
{
	return t2a_(GetImpl_(impl_).FindFloatEx(hwnd, a2t_(addr_range), float_value_min, float_value_max, steps, multi_thread, mode));
}

std::string DmSoft::FindDoubleEx(long hwnd, const std::string& addr_range, double double_value_min, double double_value_max, long steps, long multi_thread, long mode)
{
	return t2a_(GetImpl_(impl_).FindDoubleEx(hwnd, a2t_(addr_range), double_value_min, double_value_max, steps, multi_thread, mode));
}

std::string DmSoft::FindStringEx(long hwnd, const std::string& addr_range, const std::string& string_value, long tpe, long steps, long multi_thread, long mode)
{
	return t2a_(GetImpl_(impl_).FindStringEx(hwnd, a2t_(addr_range), a2t_(string_value), tpe, steps, multi_thread, mode));
}

std::string DmSoft::FindDataEx(long hwnd, const std::string& addr_range, const std::string& data, long steps, long multi_thread, long mode)
{
	return t2a_(GetImpl_(impl_).FindDataEx(hwnd, a2t_(addr_range), a2t_(data), steps, multi_thread, mode));
}

long DmSoft::EnableRealMouse(long en, long mousedelay, long mousestep)
{
	return GetImpl_(impl_).EnableRealMouse(en, mousedelay, mousestep);
}

long DmSoft::EnableRealKeypad(long en)
{
	return GetImpl_(impl_).EnableRealKeypad(en);
}

long DmSoft::SendStringIme(const std::string& str)
{
	return GetImpl_(impl_).SendStringIme(a2t_(str));
}

long DmSoft::FoobarDrawLine(long hwnd, long x1, long y1, long x2, long y2, const std::string& color, long style, long width)
{
	return GetImpl_(impl_).FoobarDrawLine(hwnd, x1, y1, x2, y2, a2t_(color), style, width);
}

std::string DmSoft::FindStrEx(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrEx(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

long DmSoft::IsBind(long hwnd)
{
	return GetImpl_(impl_).IsBind(hwnd);
}

long DmSoft::SetDisplayDelay(long t)
{
	return GetImpl_(impl_).SetDisplayDelay(t);
}

long DmSoft::GetDmCount()
{
	return GetImpl_(impl_).GetDmCount();
}

long DmSoft::DisableScreenSave()
{
	return GetImpl_(impl_).DisableScreenSave();
}

long DmSoft::DisablePowerSave()
{
	return GetImpl_(impl_).DisablePowerSave();
}

long DmSoft::SetMemoryHwndAsProcessId(long en)
{
	return GetImpl_(impl_).SetMemoryHwndAsProcessId(en);
}

long DmSoft::FindShape(long x1, long y1, long x2, long y2, const std::string& offset_color, double sim, long dir, long * x, long * y)
{
	return GetImpl_(impl_).FindShape(x1, y1, x2, y2, a2t_(offset_color), sim, dir, x, y);
}

void DmSoft::_FindShape_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& offset_color, double sim, long dir)
{
	long x = -1, y = -1;
	auto res = FindShape(x1, y1, x2, y2, offset_color, sim, dir, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindShapeE(long x1, long y1, long x2, long y2, const std::string& offset_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindShapeE(x1, y1, x2, y2, a2t_(offset_color), sim, dir));
}

std::string DmSoft::FindShapeEx(long x1, long y1, long x2, long y2, const std::string& offset_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindShapeEx(x1, y1, x2, y2, a2t_(offset_color), sim, dir));
}

std::string DmSoft::FindStrS(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, long * x, long * y)
{
	return t2a_(GetImpl_(impl_).FindStrS(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, x, y));
}

void DmSoft::_FindStrS_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	long x = -1, y = -1;
	const auto& res = FindStrS(x1, y1, x2, y2, str, color, sim, &x, &y);
	lua_pushlstring(l, res.c_str(), res.size());
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindStrExS(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrExS(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

std::string DmSoft::FindStrFastS(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim, long * x, long * y)
{
	return t2a_(GetImpl_(impl_).FindStrFastS(x1, y1, x2, y2, a2t_(str), a2t_(color), sim, x, y));
}

void DmSoft::_FindStrFastS_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	long x = -1, y = -1;
	const auto& res = FindStrFastS(x1, y1, x2, y2, str, color, sim, &x, &y);
	lua_pushlstring(l, res.c_str(), res.size());
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindStrFastExS(long x1, long y1, long x2, long y2, const std::string& str, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).FindStrFastExS(x1, y1, x2, y2, a2t_(str), a2t_(color), sim));
}

std::string DmSoft::FindPicS(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir, long * x, long * y)
{
	return t2a_(GetImpl_(impl_).FindPicS(x1, y1, x2, y2, a2t_(pic_name), a2t_(delta_color), sim, dir, x, y));
}

void DmSoft::_FindPicS_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir)
{
	long x = -1, y = -1;
	const auto& res = FindPicS(x1, y1, x2, y2, pic_name, delta_color, sim, dir, &x, &y);
	lua_pushlstring(l, res.c_str(), res.size());
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindPicExS(long x1, long y1, long x2, long y2, const std::string& pic_name, const std::string& delta_color, double sim, long dir)
{
	return t2a_(GetImpl_(impl_).FindPicExS(x1, y1, x2, y2, a2t_(pic_name), a2t_(delta_color), sim, dir));
}

long DmSoft::ClearDict(long index)
{
	return GetImpl_(impl_).ClearDict(index);
}

std::string DmSoft::GetMachineCodeNoMac()
{
	return t2a_(GetImpl_(impl_).GetMachineCodeNoMac());
}

long DmSoft::GetClientRect(long hwnd, long * x1, long * y1, long * x2, long * y2)
{
	return GetImpl_(impl_).GetClientRect(hwnd, x1, y1, x2, y2);
}

void DmSoft::_GetClientRect_(lua_State* l, long hwnd)
{
	long x1 = -1, y1 = -1, x2 = -1, y2 = -1;
	auto res = GetClientRect(hwnd, &x1, &y1, &x2, &y2);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x1);
	lua_pushinteger(l, y1);
	lua_pushinteger(l, x2);
	lua_pushinteger(l, y2);
}

long DmSoft::EnableFakeActive(long en)
{
	return GetImpl_(impl_).EnableFakeActive(en);
}

long DmSoft::GetScreenDataBmp(long x1, long y1, long x2, long y2, long * data, long * size)
{
	return GetImpl_(impl_).GetScreenDataBmp(x1, y1, x2, y2, data, size);
}

void DmSoft::_GetScreenDataBmp_(lua_State* l, long x1, long y1, long x2, long y2)
{
	long data = -1, size = -1;
	auto res = GetScreenDataBmp(x1, y1, x2, y2, &data, &size);
	lua_pushinteger(l, res);
	lua_pushinteger(l, data);
	lua_pushinteger(l, size);
}

long DmSoft::EncodeFile(const std::string& file_name, const std::string& pwd)
{
	return GetImpl_(impl_).EncodeFile(a2t_(file_name), a2t_(pwd));
}

std::string DmSoft::GetCursorShapeEx(long tpe)
{
	return t2a_(GetImpl_(impl_).GetCursorShapeEx(tpe));
}

long DmSoft::FaqCancel()
{
	return GetImpl_(impl_).FaqCancel();
}

std::string DmSoft::IntToData(long int_value, long tpe)
{
	return t2a_(GetImpl_(impl_).IntToData(int_value, tpe));
}

std::string DmSoft::FloatToData(float float_value)
{
	return t2a_(GetImpl_(impl_).FloatToData(float_value));
}

std::string DmSoft::DoubleToData(double double_value)
{
	return t2a_(GetImpl_(impl_).DoubleToData(double_value));
}

std::string DmSoft::StringToData(const std::string& string_value, long tpe)
{
	return t2a_(GetImpl_(impl_).StringToData(a2t_(string_value), tpe));
}

long DmSoft::SetMemoryFindResultToFile(const std::string& file_name)
{
	return GetImpl_(impl_).SetMemoryFindResultToFile(a2t_(file_name));
}

long DmSoft::EnableBind(long en)
{
	return GetImpl_(impl_).EnableBind(en);
}

long DmSoft::SetSimMode(long mode)
{
	return GetImpl_(impl_).SetSimMode(mode);
}

long DmSoft::LockMouseRect(long x1, long y1, long x2, long y2)
{
	return GetImpl_(impl_).LockMouseRect(x1, y1, x2, y2);
}

long DmSoft::SendPaste(long hwnd)
{
	return GetImpl_(impl_).SendPaste(hwnd);
}

long DmSoft::IsDisplayDead(long x1, long y1, long x2, long y2, long t)
{
	return GetImpl_(impl_).IsDisplayDead(x1, y1, x2, y2, t);
}

long DmSoft::GetKeyState(long vk)
{
	return GetImpl_(impl_).GetKeyState(vk);
}

long DmSoft::CopyFile(const std::string& src_file, const std::string& dst_file, long over)
{
	return GetImpl_(impl_).CopyFile(a2t_(src_file), a2t_(dst_file), over);
}

long DmSoft::IsFileExist(const std::string& file_name)
{
	return GetImpl_(impl_).IsFileExist(a2t_(file_name));
}

long DmSoft::DeleteFile(const std::string& file_name)
{
	return GetImpl_(impl_).DeleteFile(a2t_(file_name));
}

long DmSoft::MoveFile(const std::string& src_file, const std::string& dst_file)
{
	return GetImpl_(impl_).MoveFile(a2t_(src_file), a2t_(dst_file));
}

long DmSoft::CreateFolder(const std::string& folder_name)
{
	return GetImpl_(impl_).CreateFolder(a2t_(folder_name));
}

long DmSoft::DeleteFolder(const std::string& folder_name)
{
	return GetImpl_(impl_).DeleteFolder(a2t_(folder_name));
}

long DmSoft::GetFileLength(const std::string& file_name)
{
	return GetImpl_(impl_).GetFileLength(a2t_(file_name));
}

std::string DmSoft::ReadFile(const std::string& file_name)
{
	return t2a_(GetImpl_(impl_).ReadFile(a2t_(file_name)));
}

long DmSoft::WaitKey(long key_code, long time_out)
{
	return GetImpl_(impl_).WaitKey(key_code, time_out);
}

long DmSoft::DeleteIni(const std::string& section, const std::string& key, const std::string& file_name)
{
	return GetImpl_(impl_).DeleteIni(a2t_(section), a2t_(key), a2t_(file_name));
}

long DmSoft::DeleteIniPwd(const std::string& section, const std::string& key, const std::string& file_name, const std::string& pwd)
{
	return GetImpl_(impl_).DeleteIniPwd(a2t_(section), a2t_(key), a2t_(file_name), a2t_(pwd));
}

long DmSoft::EnableSpeedDx(long en)
{
	return GetImpl_(impl_).EnableSpeedDx(en);
}

long DmSoft::EnableIme(long en)
{
	return GetImpl_(impl_).EnableIme(en);
}

long DmSoft::Reg(const std::string& code, const std::string& Ver)
{
	return GetImpl_(impl_).Reg(a2t_(code), a2t_(Ver));
}

std::string DmSoft::SelectFile()
{
	return t2a_(GetImpl_(impl_).SelectFile());
}

std::string DmSoft::SelectDirectory()
{
	return t2a_(GetImpl_(impl_).SelectDirectory());
}

long DmSoft::LockDisplay(long locks)
{
	return GetImpl_(impl_).LockDisplay(locks);
}

long DmSoft::FoobarSetSave(long hwnd, const std::string& file_name, long en, const std::string& header)
{
	return GetImpl_(impl_).FoobarSetSave(hwnd, a2t_(file_name), en, a2t_(header));
}

std::string DmSoft::EnumWindowSuper(const std::string& spec1, long flag1, long type1, const std::string& spec2, long flag2, long type2, long sort)
{
	return t2a_(GetImpl_(impl_).EnumWindowSuper(a2t_(spec1), flag1, type1, a2t_(spec2), flag2, type2, sort));
}

long DmSoft::DownloadFile(const std::string& url, const std::string& save_file, long timeout)
{
	return GetImpl_(impl_).DownloadFile(a2t_(url), a2t_(save_file), timeout);
}

long DmSoft::EnableKeypadMsg(long en)
{
	return GetImpl_(impl_).EnableKeypadMsg(en);
}

long DmSoft::EnableMouseMsg(long en)
{
	return GetImpl_(impl_).EnableMouseMsg(en);
}

long DmSoft::RegNoMac(const std::string& code, const std::string& Ver)
{
	return GetImpl_(impl_).RegNoMac(a2t_(code), a2t_(Ver));
}

long DmSoft::RegExNoMac(const std::string& code, const std::string& Ver, const std::string& ip)
{
	return GetImpl_(impl_).RegExNoMac(a2t_(code), a2t_(Ver), a2t_(ip));
}

long DmSoft::SetEnumWindowDelay(long delay)
{
	return GetImpl_(impl_).SetEnumWindowDelay(delay);
}

long DmSoft::FindMulColor(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return GetImpl_(impl_).FindMulColor(x1, y1, x2, y2, a2t_(color), sim);
}

std::string DmSoft::GetDict(long index, long font_index)
{
	return t2a_(GetImpl_(impl_).GetDict(index, font_index));
}

long DmSoft::GetBindWindow()
{
	return GetImpl_(impl_).GetBindWindow();
}

long DmSoft::FoobarStartGif(long hwnd, long x, long y, const std::string& pic_name, long repeat_limit, long delay)
{
	return GetImpl_(impl_).FoobarStartGif(hwnd, x, y, a2t_(pic_name), repeat_limit, delay);
}

long DmSoft::FoobarStopGif(long hwnd, long x, long y, const std::string& pic_name)
{
	return GetImpl_(impl_).FoobarStopGif(hwnd, x, y, a2t_(pic_name));
}

long DmSoft::FreeProcessMemory(long hwnd)
{
	return GetImpl_(impl_).FreeProcessMemory(hwnd);
}

std::string DmSoft::ReadFileData(const std::string& file_name, long start_pos, long end_pos)
{
	return t2a_(GetImpl_(impl_).ReadFileData(a2t_(file_name), start_pos, end_pos));
}

long DmSoft::VirtualAllocEx(long hwnd, long addr, long size, long tpe)
{
	return GetImpl_(impl_).VirtualAllocEx(hwnd, addr, size, tpe);
}

long DmSoft::VirtualFreeEx(long hwnd, long addr)
{
	return GetImpl_(impl_).VirtualFreeEx(hwnd, addr);
}

std::string DmSoft::GetCommandLine(long hwnd)
{
	return t2a_(GetImpl_(impl_).GetCommandLine(hwnd));
}

long DmSoft::TerminateProcess(long pid)
{
	return GetImpl_(impl_).TerminateProcess(pid);
}

std::string DmSoft::GetNetTimeByIp(const std::string& ip)
{
	return t2a_(GetImpl_(impl_).GetNetTimeByIp(a2t_(ip)));
}

std::string DmSoft::EnumProcess(const std::string& name)
{
	return t2a_(GetImpl_(impl_).EnumProcess(a2t_(name)));
}

std::string DmSoft::GetProcessInfo(long pid)
{
	return t2a_(GetImpl_(impl_).GetProcessInfo(pid));
}

long DmSoft::ReadIntAddr(long hwnd, long addr, long tpe)
{
	return GetImpl_(impl_).ReadIntAddr(hwnd, addr, tpe);
}

std::string DmSoft::ReadDataAddr(long hwnd, long addr, long length)
{
	return t2a_(GetImpl_(impl_).ReadDataAddr(hwnd, addr, length));
}

double DmSoft::ReadDoubleAddr(long hwnd, long addr)
{
	return GetImpl_(impl_).ReadDoubleAddr(hwnd, addr);
}

float DmSoft::ReadFloatAddr(long hwnd, long addr)
{
	return GetImpl_(impl_).ReadFloatAddr(hwnd, addr);
}

std::string DmSoft::ReadStringAddr(long hwnd, long addr, long tpe, long length)
{
	return t2a_(GetImpl_(impl_).ReadStringAddr(hwnd, addr, tpe, length));
}

long DmSoft::WriteDataAddr(long hwnd, long addr, const std::string& data)
{
	return GetImpl_(impl_).WriteDataAddr(hwnd, addr, a2t_(data));
}

long DmSoft::WriteDoubleAddr(long hwnd, long addr, double v)
{
	return GetImpl_(impl_).WriteDoubleAddr(hwnd, addr, v);
}

long DmSoft::WriteFloatAddr(long hwnd, long addr, float v)
{
	return GetImpl_(impl_).WriteFloatAddr(hwnd, addr, v);
}

long DmSoft::WriteIntAddr(long hwnd, long addr, long tpe, long v)
{
	return GetImpl_(impl_).WriteIntAddr(hwnd, addr, tpe, v);
}

long DmSoft::WriteStringAddr(long hwnd, long addr, long tpe, const std::string& v)
{
	return GetImpl_(impl_).WriteStringAddr(hwnd, addr, tpe, a2t_(v));
}

long DmSoft::Delays(long min_s, long max_s)
{
	return GetImpl_(impl_).Delays(min_s, max_s);
}

long DmSoft::FindColorBlock(long x1, long y1, long x2, long y2, const std::string& color, double sim, long count, long width, long height, long * x, long * y)
{
	return GetImpl_(impl_).FindColorBlock(x1, y1, x2, y2, a2t_(color), sim, count, width, height, x, y);
}

void DmSoft::_FindColorBlock_(lua_State* l, long x1, long y1, long x2, long y2, const std::string& color, double sim, long count, long width, long height)
{
	long x = -1, y = -1;
	auto res = FindColorBlock(x1, y1, x2, y2, color, sim, count, width, height, &x, &y);
	lua_pushinteger(l, res);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
}

std::string DmSoft::FindColorBlockEx(long x1, long y1, long x2, long y2, const std::string& color, double sim, long count, long width, long height)
{
	return t2a_(GetImpl_(impl_).FindColorBlockEx(x1, y1, x2, y2, a2t_(color), sim, count, width, height));
}

long DmSoft::OpenProcess(long pid)
{
	return GetImpl_(impl_).OpenProcess(pid);
}

std::string DmSoft::EnumIniSection(const std::string& file_name)
{
	return t2a_(GetImpl_(impl_).EnumIniSection(a2t_(file_name)));
}

std::string DmSoft::EnumIniSectionPwd(const std::string& file_name, const std::string& pwd)
{
	return t2a_(GetImpl_(impl_).EnumIniSectionPwd(a2t_(file_name), a2t_(pwd)));
}

std::string DmSoft::EnumIniKey(const std::string& section, const std::string& file_name)
{
	return t2a_(GetImpl_(impl_).EnumIniKey(a2t_(section), a2t_(file_name)));
}

std::string DmSoft::EnumIniKeyPwd(const std::string& section, const std::string& file_name, const std::string& pwd)
{
	return t2a_(GetImpl_(impl_).EnumIniKeyPwd(a2t_(section), a2t_(file_name), a2t_(pwd)));
}

long DmSoft::SwitchBindWindow(long hwnd)
{
	return GetImpl_(impl_).SwitchBindWindow(hwnd);
}

long DmSoft::InitCri()
{
	return GetImpl_(impl_).InitCri();
}

long DmSoft::SendStringIme2(long hwnd, const std::string& str, long mode)
{
	return GetImpl_(impl_).SendStringIme2(hwnd, a2t_(str), mode);
}

std::string DmSoft::EnumWindowByProcessId(long pid, const std::string& title, const std::string& class_name, long filter)
{
	return t2a_(GetImpl_(impl_).EnumWindowByProcessId(pid, a2t_(title), a2t_(class_name), filter));
}

std::string DmSoft::GetDisplayInfo()
{
	return t2a_(GetImpl_(impl_).GetDisplayInfo());
}

long DmSoft::EnableFontSmooth()
{
	return GetImpl_(impl_).EnableFontSmooth();
}

std::string DmSoft::OcrExOne(long x1, long y1, long x2, long y2, const std::string& color, double sim)
{
	return t2a_(GetImpl_(impl_).OcrExOne(x1, y1, x2, y2, a2t_(color), sim));
}

long DmSoft::SetAero(long en)
{
	return GetImpl_(impl_).SetAero(en);
}

long DmSoft::FoobarSetTrans(long hwnd, long trans, const std::string& color, double sim)
{
	return GetImpl_(impl_).FoobarSetTrans(hwnd, trans, a2t_(color), sim);
}

long DmSoft::EnablePicCache(long en)
{
	return GetImpl_(impl_).EnablePicCache(en);
}

std::string DmSoft::GetInfo(const std::string& cmd, const std::string& param)
{
	return t2a_(GetImpl_(impl_).GetInfo(a2t_(cmd), a2t_(param)));
}

long DmSoft::FaqIsPosted()
{
	return GetImpl_(impl_).FaqIsPosted();
}

long DmSoft::LoadPicByte(long addr, long size, const std::string& name)
{
	return GetImpl_(impl_).LoadPicByte(addr, size, a2t_(name));
}

long DmSoft::MiddleDown()
{
	return GetImpl_(impl_).MiddleDown();
}

long DmSoft::MiddleUp()
{
	return GetImpl_(impl_).MiddleUp();
}

long DmSoft::FaqCaptureString(const std::string& str)
{
	return GetImpl_(impl_).FaqCaptureString(a2t_(str));
}

long DmSoft::VirtualProtectEx(long hwnd, long addr, long size, long tpe, long old_protect)
{
	return GetImpl_(impl_).VirtualProtectEx(hwnd, addr, size, tpe, old_protect);
}

long DmSoft::SetMouseSpeed(long speed)
{
	return GetImpl_(impl_).SetMouseSpeed(speed);
}

long DmSoft::GetMouseSpeed()
{
	return GetImpl_(impl_).GetMouseSpeed();
}

long DmSoft::EnableMouseAccuracy(long en)
{
	return GetImpl_(impl_).EnableMouseAccuracy(en);
}
