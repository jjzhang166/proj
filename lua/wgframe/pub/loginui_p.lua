require("frmcmn")
require("WxCmn")
require("consolecfg")
loginui_p = {
	oper = frmcmn.MakeProxy("loginio", true),
	ui = {
		ShowLogin = nil,
	},
	EndDayTime = nil,
}

local oper = loginui_p.oper
local ui = loginui_p.ui

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

function loginui_p.EndDayTime()
	return endDayTime
end

local function CheckAcPwd(ac, pwd)
	local acLen = #ac
	if acLen < acPwdMinLen then
		wx.wxMessageBox(tostring("’ ∫≈≥§∂»–°”⁄ ", acPwdMinLen, " "), "’À∫≈¥ÌŒÛ", wx.wxOK)
		return 
	end
	if acLen > acPwdMaxLen then
		wx.wxMessageBox(tostring("’ ∫≈≥§∂»¥Û”⁄ ", acPwdMaxLen, ",≤ªƒ‹”–÷–Œƒ"), "’À∫≈¥ÌŒÛ", wx.wxOK)
		return 
	end
	if pwd == nil then
		return true
	end
	local pwdLen = #pwd
	if pwdLen < acPwdMinLen then
		wx.wxMessageBox(tostring("√‹¬Î≥§∂»–°”⁄ ", acPwdMinLen, " "), "√‹¬Î¥ÌŒÛ", wx.wxOK)
		return 
	end
	if pwdLen > acPwdMaxLen then
		wx.wxMessageBox(tostring("√‹¬Î≥§∂»¥Û”⁄ ", acPwdMaxLen, " ,≤ªƒ‹∞¸∫¨÷–Œƒ"), "√‹¬Î¥ÌŒÛ", wx.wxOK)
		return 
	end
	return true
end

local function CheckQq(qq)
	local qqLen = #qq
	if qqLen < qqMinLen or qqLen > qqMaxLen then
		wx.wxMessageBox("QQ∫≈ÃÓ–¥¥ÌŒÛ", "¥ÌŒÛ", wx.wxOK)
		return
	end
	return true
end

local function CheckKey(str, key)
	local keyLen = #key
	if keyLen < cardIdMinLen or keyLen > cardIdMaxLen then
    wx.wxMessageBox(str .. " ÃÓ–¥¥ÌŒÛ", str .. "¥ÌŒÛ", wx.wxOK)
		--wx.wxMessageBox("«ÎÃÓ–¥’˝»∑µƒ" .. str, str .. "¥ÌŒÛ", wx.wxOK)
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
	statusBar:PushStatusText("’˝‘⁄µ«¬Ω...")
	local res = oper.Login(true, ac, pwd)
	statusBar:PopStatusText()
	if res == nil then
		statusBar:SetStatusText("Õ¯¬Á‘≠“Ú,µ«¬Ω ß∞‹")
	else
		if res[1] == true then
			statusBar:SetStatusText(tostring("µ«¬Ω≥…π¶£¨µΩ∆⁄ ±º‰:>>>>", res[2]))
			endDayTime = res[2]
			frameLogin:Close()
            local loginAcInfo = consolecfg.mainCfg.loginAcInfo
            loginAcInfo.ac = ac
            loginAcInfo.pwd = pwd
		else
			statusBar:SetStatusText(tostring("µ«¬Ω ß∞‹>>>>>", res[2]))
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
	statusBar:PushStatusText("’˝‘⁄Ω‚∞Û...")
	local res = oper.RemoveBind(ac, pwd)
	statusBar:PopStatusText()
	if res == nil then
		statusBar:SetStatusText("Õ¯¬Á‘≠“Ú,Ω‚∞Û ß∞‹")
	else
		if res[1] == true then
			statusBar:SetStatusText(res[2])
		else
			statusBar:SetStatusText(tostring("Ω‚∞Û ß∞‹>>>>", res[2]))
		end
	end
	Sleep(1500)
end

local function AcPromptStr()
	return tostring("’ ∫≈:(≤ªƒ‹∞¸∫¨÷–Œƒ,’À∫≈≥§∂»«Îøÿ÷∆‘⁄ ", acPwdMinLen, "  -  ", acPwdMaxLen, " ÷Æº‰)")
end

local function PwdPromptStr()
	return tostring("√‹¬Î:(≤ªƒ‹∞¸∫¨÷–Œƒ,√‹¬Î≥§∂»«Îøÿ÷∆‘⁄ ", acPwdMinLen, " -  ", acPwdMaxLen, " ÷Æº‰)")
end

local function QqPromptStr()
	return tostring("QQ∫≈≥§∂»«Îøÿ÷∆‘⁄ ", qqMinLen, " - ", qqMaxLen, " ÷Æº‰")
end

local function OnCmdReg()
	local dlgReg = wx.wxDialog(frameLogin, wx.wxID_ANY, "◊¢≤·’À∫≈",wx.wxDefaultPosition, wx.wxSize(1000,105))
	local regStatusBar = wx.wxStatusBar(dlgReg, wx.wxID_ANY)
--	local staticAc = wx.wxStaticText(dlgReg, wx.wxID_ANY, "’ ∫≈£∫")
--	local staticPwd = wx.wxStaticText(dlgReg, wx.wxID_ANY, "√‹¬Î£∫")
--	local staticRegId = wx.wxStaticText(dlgReg, wx.wxID_ANY, "◊¢≤·¬Î£∫")
--	local staticQQ = wx.wxStaticText(dlgReg, wx.wxID_ANY, "ø€ø€£∫")
	local ctrlAc = WxCmn.CreateTextCtrl(dlgReg, "ÃÓ–¥’À∫≈", wx.wxTE_LEFT, acPwdMaxLen, regStatusBar, AcPromptStr())
	local ctrlPwd = WxCmn.CreateTextCtrl(dlgReg, "", bit.bor(wx.wxTE_LEFT, wx.wxTE_PASSWORD), acPwdMaxLen, regStatusBar, PwdPromptStr())
	local ctrlQQ = WxCmn.CreateTextCtrl(dlgReg, "ÃÓ–¥QQ", wx.wxTE_LEFT, qqMaxLen, regStatusBar, QqPromptStr())
	local ctrlRegId = WxCmn.CreateTextCtrl(dlgReg, "ÃÓ–¥◊¢≤·¬Î", wx.wxTE_LEFT, cardIdMaxLen)
	local btReg = wx.wxButton(dlgReg, cmdIdDoReg, "ø™ º◊¢≤·")
	
	dlgReg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlgReg:GetClientSize()
		WxCmn.SetStatusBarSize(regStatusBar, clientSize)
		local centerSize = dlgReg:GetClientSize():Scale(0, 0)
		local staticAcPos = wx.wxPoint(centerSize:GetWidth() + 4, centerSize:GetHeight() + 4)
		ctrlAc:Move(staticAcPos)
        WxCmn.SetHorizonPos(ctrlAc, ctrlPwd, nil, 0.1)
        WxCmn.SetHorizonPos(ctrlPwd, ctrlQQ, nil, 0.1)
        WxCmn.SetHorizonPos(ctrlQQ, ctrlRegId, nil, 0.1)
        WxCmn.SetHorizonPos(ctrlRegId, btReg, nil, 0.1)

--		local width = (staticAc:GetSize():GetWidth() + staticRegId:GetSize():GetWidth()) * 0.5
--		WxCmn.SetVerticalPos(staticAc, staticPwd)
--		WxCmn.SetHorizonPos(staticAc, ctrlAc, nil, nil, width, width)
--		WxCmn.SetHorizonPos(staticPwd, ctrlPwd, nil, nil, width, width)
--		WxCmn.SetVerticalPos(staticPwd, staticQQ)
--		WxCmn.SetHorizonPos(staticQQ, ctrlQQ, nil, nil, width, width)
--		WxCmn.SetVerticalPos(staticQQ, staticRegId)
--		WxCmn.SetHorizonPos(staticRegId, ctrlRegId, nil, nil, width, width)
--		btReg:Move(wx.wxPoint(centerSize:GetWidth() - btReg:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.73))
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
		if CheckKey("◊¢≤·¬Î", regKey) ~= true then
			return
		end
		
		regStatusBar:PushStatusText("’˝‘⁄◊¢≤·...")
		local res = oper.Reg(ac, pwd, qq, regKey)
		regStatusBar:PopStatusText()
		if res == nil then
			regStatusBar:SetStatusText("◊¢≤· ß∞‹£¨Õ¯¬Á‘≠“Ú")
		else
			if res[1] == true then
				regStatusBar:SetStatusText(res[2])
			else
				regStatusBar:SetStatusText(tostring("◊¢≤· ß∞‹>>>>", res[2]))
			end
		end
	end)
	
	dlgReg:ShowModal()
	dlgReg:Destroy()
end

local function OnCmdPay()
	local dlgPay = wx.wxDialog(frameLogin, wx.wxID_ANY, "’À∫≈≥‰÷µøÚ",wx.wxDefaultPosition, wx.wxSize(830,105))
	local payStatusBar = wx.wxStatusBar(dlgPay, wx.wxID_ANY)
--	local staticCardId = wx.wxStaticText(dlgPay, wx.wxID_ANY, "ø®∫≈£∫")
--	local staticCardPwd = wx.wxStaticText(dlgPay, wx.wxID_ANY, "ø®√‹£∫")
--	local staticAc = wx.wxStaticText(dlgPay, wx.wxID_ANY, "’ ∫≈£∫")
	local ctrlCardId = WxCmn.CreateTextCtrl(dlgPay, "ÃÓ–¥ø®∫≈", wx.wxTE_LEFT, cardIdMaxLen, payStatusBar, "≥‰÷µø®∫≈")
	local ctrlCardPwd = WxCmn.CreateTextCtrl(dlgPay, "ÃÓ–¥ø®√‹", wx.wxTE_LEFT, cardIdMaxLen, payStatusBar, "≥‰÷µø®√‹")
	local ctrlAc = WxCmn.CreateTextCtrl(dlgPay, "ÃÓ–¥≥‰÷µ’ ∫≈", wx.wxTE_LEFT, acPwdMaxLen, payStatusBar, "–Ë“™≥‰÷µµƒ’ ∫≈:" .. AcPromptStr())
	local btPay = wx.wxButton(dlgPay, cmdIdDoPay, "ø™ º≥‰÷µ")
	
	dlgPay:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlgPay:GetClientSize()
		WxCmn.SetStatusBarSize(payStatusBar, clientSize)
		local centerSize = dlgPay:GetClientSize():Scale(0, 0)
		ctrlCardId:Move(wx.wxPoint(centerSize:GetWidth() + 3, centerSize:GetHeight() + 4))
        WxCmn.SetHorizonPos(ctrlCardId, ctrlCardPwd, nil, 0.1)
        WxCmn.SetHorizonPos(ctrlCardPwd, ctrlAc, nil, 0.1)
        WxCmn.SetHorizonPos(ctrlAc, btPay, nil, 0.1)

--		WxCmn.SetVerticalPos(staticCardId, staticCardPwd)
--		WxCmn.SetHorizonPos(staticCardId, ctrlCardId)
--		WxCmn.SetHorizonPos(staticCardPwd, ctrlCardPwd)
--		WxCmn.SetVerticalPos(staticCardPwd, staticAc)
--		WxCmn.SetHorizonPos(staticAc, ctrlAc)
--		btPay:Move(wx.wxPoint(centerSize:GetWidth() - btPay:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.7))
	end)
	
	dlgPay:Connect(cmdIdDoPay, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		local cardId = ctrlCardId:GetValue()
		if CheckKey("≥‰÷µø®∫≈", cardId) ~= true then
			return
		end
		local cardPwd = ctrlCardPwd:GetValue()
		if CheckKey("≥‰÷µø®√‹", cardPwd) ~= true then
			return
		end
		local ac = ctrlAc:GetValue()
		if CheckAcPwd(ac) ~= true then
			return
		end
		
		payStatusBar:PushStatusText("’˝‘⁄≥‰÷µ...")
		local res = oper.Pay(ac, cardId, cardPwd)
		payStatusBar:PopStatusText()
		if res == nil then
			payStatusBar:SetStatusText("Õ¯¬Á‘≠“Ú ≥‰÷µ ß∞‹")
		else
			if res[1] == true then
				payStatusBar:SetStatusText(res[2])
			else
				payStatusBar:SetStatusText(tostring("≥‰÷µ ß∞‹ ¥ÌŒÛ¬Î->", res[2]))
			end
		end
	end)
	
	dlgPay:ShowModal()
	dlgPay:Destroy()
end

local function CreateLoginUi()
	local wndClient = wx.wxWindow(frameLogin, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)	
	--local staticAc = wx.wxStaticText(wndClient, wx.wxID_ANY, "’ ∫≈£∫")
--	local staticPwd = wx.wxStaticText(wndClient, wx.wxID_ANY, "√‹¬Î£∫")
    local loginAcInfo = consolecfg.mainCfg.loginAcInfo
	textCtrlAc = WxCmn.CreateTextCtrl(wndClient, loginAcInfo.ac, wx.wxTE_LEFT, acPwdMaxLen, statusBar, AcPromptStr())
	textCtrlPwd = WxCmn.CreateTextCtrl(wndClient, loginAcInfo.pwd, bit.bor(wx.wxTE_LEFT, wx.wxTE_PASSWORD), acPwdMaxLen, statusBar, PwdPromptStr())
	local btLogin = wx.wxButton(wndClient, cmdIdLogin, "µ«¬Ω")
	local btRemoveBind = wx.wxButton(wndClient, cmdIdRemoveBind, "Ω‚∞Û")
    local btRegister = wx.wxButton(wndClient, cmdIdReg, "◊¢≤·’À∫≈")
	local btPay = wx.wxButton(wndClient, cmdIdPay, "≥‰÷µ’À∫≈")
	
	wndClient:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local centerSize = event:GetSize():Scale(0,0)
		local TextAcPos = wx.wxPoint(centerSize:GetWidth()+3 , centerSize:GetHeight()+4 )
		textCtrlAc:Move(TextAcPos)
        WxCmn.SetHorizonPos(textCtrlAc, textCtrlPwd, nil, 0.1)
        WxCmn.SetHorizonPos(textCtrlPwd, btLogin, nil,0.1)
        WxCmn.SetHorizonPos(btLogin, btRemoveBind, nil,0.1)
        WxCmn.SetHorizonPos(btRemoveBind, btRegister,nil, 0.1)
        WxCmn.SetHorizonPos(btRegister, btPay, nil,0.1)
--		WxCmn.SetVerticalPos(textCtrlAc, textCtrlPwd)
--      WxCmn.SetVerticalPos(staticPwd, btLogin, 1.3, 2)
--		WxCmn.SetHorizonPos(staticPwd, textCtrlPwd)
--		WxCmn.SetHorizonPos(btLogin, btRemoveBind)
	end)
end

local function CreateToolBar()
	toolBar = frameLogin:CreateToolBar(wx.wxNO_BORDER + wx.wxTB_FLAT + wx.wxTB_DOCKABLE)
	local toolBmpSize = toolBar:GetToolBitmapSize()
	toolBar:AddTool(cmdIdReg, "◊¢≤·", wx.wxArtProvider.GetBitmap(wx.wxART_ADD_BOOKMARK, wx.wxART_TOOLBAR, toolBmpSize), "◊¢≤·“ª∏ˆ’ ∫≈")
	toolBar:AddTool(cmdIdPay, "≥‰÷µ", wx.wxArtProvider.GetBitmap(wx.wxART_REPORT_VIEW, wx.wxART_TOOLBAR, toolBmpSize), "≥‰÷µ’ ∫≈")
	toolBar:Realize()
end

local function CreateCmdMap()
	frameLogin:Connect(cmdIdLogin, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdLogin)
	frameLogin:Connect(cmdIdRemoveBind, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdRemoveBind)
	frameLogin:Connect(cmdIdReg, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdReg)
	frameLogin:Connect(cmdIdPay, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnCmdPay)

--	frameLogin:Connect(cmdIdReg, wx.wxEVT_COMMAND_MENU_SELECTED, OnCmdReg)
--	frameLogin:Connect(cmdIdPay, wx.wxEVT_COMMAND_MENU_SELECTED, OnCmdPay)
end

function ui.ShowLogin(title, onLoginSucceed)
    local maxmin = wx.wxSize()
    maxmin.x = 100
    maxmin.y = 200
	local style = CmnBase.BitExcept(wx.wxDEFAULT_FRAME_STYLE, bit.bor(wx.wxMINIMIZE_BOX, wx.wxMAXIMIZE_BOX, wx.wxRESIZE_BORDER))
	frameLogin = wx.wxFrame(wx.NULL, wx.wxID_ANY, title, wx.wxDefaultPosition, wx.wxSize(850,105)
				, style
					)
	frameLogin:CenterOnScreen()
	frameLogin:Connect(wx.wxEVT_DESTROY, function (event) 
		event:Skip(true)
		if endDayTime ~= nil then onLoginSucceed() end
	end)
	statusBar = frameLogin:CreateStatusBar()
	--CreateToolBar()
	CreateLoginUi()
	CreateCmdMap()
	frameLogin:Show(true)
end
