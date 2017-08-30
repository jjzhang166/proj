require("frmcmn")
require("WxCmn")
require("consolecfg")
loginui = {
	oper = frmcmn.MakeProxy("loginio", true),
	ui = {
		ShowLogin = nil,
	},
	EndDayTime = nil,
}

local oper = loginui.oper
local ui = loginui.ui

local frameLogin = nil
local statusBar = nil
local toolBar = nil
local textCtrlAc = nil
local textCtrlPwd = nil
local acPwdMinLen = 5
local acPwdMaxLen = 20
local cardIdMinLen = 32
local cardIdMaxLen = 36
local qqMinLen = 4
local qqMaxLen = 12
local endDayTime = nil

local cmdIdLogin = WxCmn.NewID()
local cmdIdRemoveBind = WxCmn.NewID()
local cmdIdReg = WxCmn.NewID()
local cmdIdDoReg = WxCmn.NewID()
local cmdIdPay = WxCmn.NewID()
local cmdIdDoPay = WxCmn.NewID()

function loginui.EndDayTime()
	return endDayTime
end

local function CheckAcPwd(ac, pwd)
	local acLen = #ac
	if acLen < acPwdMinLen then
		wx.wxMessageBox(tostring("帐号长度小于[", acPwdMinLen, "]"), "帐号长度错误", wx.wxOK)
		return 
	end
	if acLen > acPwdMaxLen then
		wx.wxMessageBox(tostring("帐号长度大于[", acPwdMaxLen, "]，请不要包含中文"), "帐号长度错误", wx.wxOK)
		return 
	end
	if pwd == nil then
		return true
	end
	local pwdLen = #pwd
	if pwdLen < acPwdMinLen then
		wx.wxMessageBox(tostring("密码长度小于[", acPwdMinLen, "]"), "密码长度错误", wx.wxOK)
		return 
	end
	if pwdLen > acPwdMaxLen then
		wx.wxMessageBox(tostring("密码长度大于[", acPwdMaxLen, "]，请不要包含中文"), "密码长度错误", wx.wxOK)
		return 
	end
	return true
end

local function CheckQq(qq)
	local qqLen = #qq
	if qqLen < qqMinLen or qqLen > qqMaxLen then
		wx.wxMessageBox("请填写正确的QQ", "错误", wx.wxOK)
		return
	end
	return true
end

local function CheckKey(str, key)
	local keyLen = #key
	if keyLen < cardIdMinLen or keyLen > cardIdMaxLen then
		wx.wxMessageBox("请填写正确的" .. str, str .. "错误", wx.wxOK)
		return
	end
	return true
end

local function OnCmdLogin()
	local ac = textCtrlAc:GetValue()
	local pwd = textCtrlPwd:GetValue()
	if CheckAcPwd(ac, pwd) ~= true then
		return
	end
	statusBar:PushStatusText("登陆中...")
	local res = oper.Login(true, ac, pwd)
	statusBar:PopStatusText()
	if res == nil then
		statusBar:SetStatusText("登陆失败，网络原因")
	else
		if res[1] == true then
			statusBar:SetStatusText(tostring("登陆成功，到期时间->", res[2]))
			endDayTime = res[2]
			frameLogin:Close()
            local loginAcInfo = consolecfg.mainCfg.loginAcInfo
            loginAcInfo.ac = ac
            loginAcInfo.pwd = pwd
		else
			statusBar:SetStatusText(tostring("登陆失败->", res[2]))
		end
	end
	Sleep(1500)
end

local function OnCmdRemoveBind()
	local ac = textCtrlAc:GetValue()
	local pwd = textCtrlPwd:GetValue()
	if CheckAcPwd(ac, pwd) ~= true then
		return
	end
	statusBar:PushStatusText("解绑中...")
	local res = oper.RemoveBind(ac, pwd)
	statusBar:PopStatusText()
	if res == nil then
		statusBar:SetStatusText("解绑失败，网络原因")
	else
		if res[1] == true then
			statusBar:SetStatusText(res[2])
		else
			statusBar:SetStatusText(tostring("解绑失败->", res[2]))
		end
	end
	Sleep(1500)
end

local function AcPromptStr()
	return tostring("帐号不能包含中文字符；长度必须在[", acPwdMinLen, "]到[", acPwdMaxLen, "]之间")
end

local function PwdPromptStr()
	return tostring("密码不能包含中文字符；长度必须在[", acPwdMinLen, "]到[", acPwdMaxLen, "]之间")
end

local function QqPromptStr()
	return tostring("qq长度必须在[", qqMinLen, "]到[", qqMaxLen, "]之间")
end

local function OnCmdReg()
	local dlgReg = wx.wxDialog(frameLogin, wx.wxID_ANY, "注册")
	local regStatusBar = wx.wxStatusBar(dlgReg, wx.wxID_ANY)
	local staticAc = wx.wxStaticText(dlgReg, wx.wxID_ANY, "帐号：")
	local staticPwd = wx.wxStaticText(dlgReg, wx.wxID_ANY, "密码：")
	local ctrlAc = WxCmn.CreateTextCtrl(dlgReg, "", wx.wxTE_LEFT, acPwdMaxLen, regStatusBar, AcPromptStr())
	local ctrlPwd = WxCmn.CreateTextCtrl(dlgReg, "", bit.bor(wx.wxTE_LEFT, wx.wxTE_PASSWORD), acPwdMaxLen, regStatusBar, PwdPromptStr())
	local staticQQ = wx.wxStaticText(dlgReg, wx.wxID_ANY, "扣扣：")
	local ctrlQQ = WxCmn.CreateTextCtrl(dlgReg, "", wx.wxTE_LEFT, qqMaxLen, regStatusBar, QqPromptStr())
	local staticRegId = wx.wxStaticText(dlgReg, wx.wxID_ANY, "注册码：")
	local ctrlRegId = WxCmn.CreateTextCtrl(dlgReg, "", wx.wxTE_LEFT, cardIdMaxLen)
	local btReg = wx.wxButton(dlgReg, cmdIdDoReg, "注册")
	
	dlgReg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlgReg:GetClientSize()
		WxCmn.SetStatusBarSize(regStatusBar, clientSize)
		local centerSize = dlgReg:GetClientSize():Scale(0.5, 0.5)
		local staticAcPos = wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.2)
		staticAc:Move(staticAcPos)
		local width = (staticAc:GetSize():GetWidth() + staticRegId:GetSize():GetWidth()) * 0.5
		WxCmn.SetVerticalPos(staticAc, staticPwd)
		WxCmn.SetHorizonPos(staticAc, ctrlAc, nil, nil, width, width)
		WxCmn.SetHorizonPos(staticPwd, ctrlPwd, nil, nil, width, width)
		WxCmn.SetVerticalPos(staticPwd, staticQQ)
		WxCmn.SetHorizonPos(staticQQ, ctrlQQ, nil, nil, width, width)
		WxCmn.SetVerticalPos(staticQQ, staticRegId)
		WxCmn.SetHorizonPos(staticRegId, ctrlRegId, nil, nil, width, width)
		btReg:Move(wx.wxPoint(centerSize:GetWidth() - btReg:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.73))
	end)
	
	dlgReg:Connect(cmdIdDoReg, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		local ac = ctrlAc:GetValue()
		local pwd = ctrlPwd:GetValue()
		if CheckAcPwd(ac, pwd) ~= true then
			return
		end
		local qq = ctrlQQ:GetValue()
		if CheckQq(qq) ~= true then
			return 
		end
		local regKey = ctrlRegId:GetValue()
		if CheckKey("注册码", regKey) ~= true then
			return
		end
		
		regStatusBar:PushStatusText("注册中...")
		local res = oper.Reg(ac, pwd, qq, regKey)
		regStatusBar:PopStatusText()
		if res == nil then
			regStatusBar:SetStatusText("注册失败，网络原因")
		else
			if res[1] == true then
				regStatusBar:SetStatusText(res[2])
			else
				regStatusBar:SetStatusText(tostring("注册失败->", res[2]))
			end
		end
	end)
	
	dlgReg:ShowModal()
	dlgReg:Destroy()
end

local function OnCmdPay()
	local dlgPay = wx.wxDialog(frameLogin, wx.wxID_ANY, "充值")
	local payStatusBar = wx.wxStatusBar(dlgPay, wx.wxID_ANY)
	local staticCardId = wx.wxStaticText(dlgPay, wx.wxID_ANY, "卡号：")
	local staticCardPwd = wx.wxStaticText(dlgPay, wx.wxID_ANY, "卡密：")
	local staticAc = wx.wxStaticText(dlgPay, wx.wxID_ANY, "帐号：")
	local ctrlCardId = WxCmn.CreateTextCtrl(dlgPay, "", wx.wxTE_LEFT, cardIdMaxLen, payStatusBar, "充值卡号")
	local ctrlCardPwd = WxCmn.CreateTextCtrl(dlgPay, "", wx.wxTE_LEFT, cardIdMaxLen, payStatusBar, "充值卡密")
	local ctrlAc = WxCmn.CreateTextCtrl(dlgPay, "", wx.wxTE_LEFT, acPwdMaxLen, payStatusBar, "被充值的帐号；" .. AcPromptStr())
	local btPay = wx.wxButton(dlgPay, cmdIdDoPay, "充值")
	
	dlgPay:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlgPay:GetClientSize()
		WxCmn.SetStatusBarSize(payStatusBar, clientSize)
		local centerSize = dlgPay:GetClientSize():Scale(0.5, 0.5)
		staticCardId:Move(wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.4))
		WxCmn.SetVerticalPos(staticCardId, staticCardPwd)
		WxCmn.SetHorizonPos(staticCardId, ctrlCardId)
		WxCmn.SetHorizonPos(staticCardPwd, ctrlCardPwd)
		WxCmn.SetVerticalPos(staticCardPwd, staticAc)
		WxCmn.SetHorizonPos(staticAc, ctrlAc)
		btPay:Move(wx.wxPoint(centerSize:GetWidth() - btPay:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.7))
	end)
	
	dlgPay:Connect(cmdIdDoPay, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		local cardId = ctrlCardId:GetValue()
		if CheckKey("卡号", cardId) ~= true then
			return
		end
		local cardPwd = ctrlCardPwd:GetValue()
		if CheckKey("卡密", cardPwd) ~= true then
			return
		end
		local ac = ctrlAc:GetValue()
		if CheckAcPwd(ac) ~= true then
			return
		end
		
		payStatusBar:PushStatusText("充值中...")
		local res = oper.Pay(ac, cardId, cardPwd)
		payStatusBar:PopStatusText()
		if res == nil then
			payStatusBar:SetStatusText("充值失败，网络原因")
		else
			if res[1] == true then
				payStatusBar:SetStatusText(res[2])
			else
				payStatusBar:SetStatusText(tostring("充值失败->", res[2]))
			end
		end
	end)
	
	dlgPay:ShowModal()
	dlgPay:Destroy()
end

local function CreateLoginUi()
	local wndClient = wx.wxWindow(frameLogin, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)	
	local staticAc = wx.wxStaticText(wndClient, wx.wxID_ANY, "帐号：")
	local staticPwd = wx.wxStaticText(wndClient, wx.wxID_ANY, "密码：")
    local loginAcInfo = consolecfg.mainCfg.loginAcInfo
	textCtrlAc = WxCmn.CreateTextCtrl(wndClient, loginAcInfo.ac, wx.wxTE_LEFT, acPwdMaxLen, statusBar, AcPromptStr())
	textCtrlPwd = WxCmn.CreateTextCtrl(wndClient, loginAcInfo.pwd, bit.bor(wx.wxTE_LEFT, wx.wxTE_PASSWORD), acPwdMaxLen, statusBar, PwdPromptStr())
	local btLogin = wx.wxButton(wndClient, cmdIdLogin, "登陆")
	local btRemoveBind = wx.wxButton(wndClient, cmdIdRemoveBind, "解绑")
	
	wndClient:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local centerSize = event:GetSize():Scale(0.5, 0.5)
		local staticAcPos = wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.3)
		staticAc:Move(staticAcPos)
		WxCmn.SetVerticalPos(staticAc, staticPwd)
		WxCmn.SetHorizonPos(staticAc, textCtrlAc)
		WxCmn.SetHorizonPos(staticPwd, textCtrlPwd)
		WxCmn.SetVerticalPos(staticPwd, btLogin, 1.3, 2)
		WxCmn.SetHorizonPos(btLogin, btRemoveBind)
	end)
end

local function CreateToolBar()
	toolBar = frameLogin:CreateToolBar(wx.wxNO_BORDER + wx.wxTB_FLAT + wx.wxTB_DOCKABLE)
	local toolBmpSize = toolBar:GetToolBitmapSize()
	toolBar:AddTool(cmdIdReg, "注册", wx.wxArtProvider.GetBitmap(wx.wxART_ADD_BOOKMARK, wx.wxART_TOOLBAR, toolBmpSize), "注册一个帐号")
	toolBar:AddTool(cmdIdPay, "充值", wx.wxArtProvider.GetBitmap(wx.wxART_REPORT_VIEW, wx.wxART_TOOLBAR, toolBmpSize), "充值帐号")
	toolBar:Realize()
end

local function CreateCmdMap()
	frameLogin:Connect(cmdIdLogin, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdLogin)
	frameLogin:Connect(cmdIdRemoveBind, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdRemoveBind)
	frameLogin:Connect(cmdIdReg, wx.wxEVT_COMMAND_MENU_SELECTED, OnCmdReg)
	frameLogin:Connect(cmdIdPay, wx.wxEVT_COMMAND_MENU_SELECTED, OnCmdPay)
end

function ui.ShowLogin(title, onLoginSucceed)
	local style = CmnBase.BitExcept(wx.wxDEFAULT_FRAME_STYLE, bit.bor(wx.wxMINIMIZE_BOX, wx.wxMAXIMIZE_BOX, wx.wxRESIZE_BORDER))
	frameLogin = wx.wxFrame(wx.NULL, wx.wxID_ANY, title, wx.wxDefaultPosition, wx.wxDefaultSize
				, style
					)
	frameLogin:CenterOnScreen()
	frameLogin:Connect(wx.wxEVT_DESTROY, function (event) 
		event:Skip(true)
		if endDayTime ~= nil then onLoginSucceed() end
	end)
	statusBar = frameLogin:CreateStatusBar()
	CreateToolBar()
	CreateLoginUi()
	CreateCmdMap()
	frameLogin:Show(true)
end
