require("FrmDlg")

BasicSetting = {
    wnd = nil,
    curItemData = nil,
    uiVerticalLast = nil,
    uiHorizonLast = nil,
    uiBtAutoRun = nil,
    uiBtCrackCaptcha = nil,
    -------------
    UpdateFromCurSelected = nil,
    UpdateFromItemData = nil,
    -------------
    Create = nil,
    GetSelectPathTitleInfo = nil,
    CreateCrackChaptchaUi = nil,
    CreateOtherUi = nil,
    OnCrackChaptchaSize = nil,
    OnOtherSize = nil,
    -------------
    DoUpdateAutoRunText_ = nil,
}

function BasicSetting:UpdateFromCurSelected(itemData)
    self.curItemData = itemData
    self:DoUpdateAutoRunText_()
end

function BasicSetting:UpdateFromItemData(item, itemData)
end

function BasicSetting:Create(parent)
    self.wnd = wx.wxWindow(parent, wx.wxID_ANY)
    local selfEx = CmnBase.ExtCObj(self, self.wnd)
    local staticGamePathDir = wx.wxStaticText(self.wnd, wx.wxID_ANY, "游戏目录：")
    local textGamePathDir = wx.wxTextCtrl(self.wnd, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxTE_READONLY)
    local btChooseGamePath = wx.wxButton(self.wnd, wx.wxID_ANY, "选择游戏目录")

    btChooseGamePath:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
        event:Skip(true)
		local filePath = WxCmn.OpenFile(self.wnd, self:GetSelectPathTitleInfo())
        if filePath == nil then return end
        mainfrm.consoleCfg.mainCfg.gamePath = filePath
        textGamePathDir:SetValue(filePath)
        GetMyApp():OnSetGamePath(filePath, true)
	end)
    textGamePathDir:Connect(wx.wxID_ANY, wx.wxEVT_SET_FOCUS, function(event)
        event:Skip(true)
		mainfrm.statusBar:PushStatusText(textGamePathDir:GetValue())
	end)
    textGamePathDir:Connect(wx.wxID_ANY, wx.wxEVT_KILL_FOCUS, function(event)
        event:Skip(true)
		mainfrm.statusBar:PopStatusText()
	end)
    local theGamePath = mainfrm.consoleCfg.mainCfg.gamePath
    if theGamePath ~= nil then
        textGamePathDir:SetValue(theGamePath)
    end
    ----------------------------------------------
    local btAutoRun = wx.wxButton(self.wnd, wx.wxID_ANY, "自动运行")
    btAutoRun:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
        event:Skip(true)
		if selfEx.curItemData == nil then return end
        selfEx.curItemData.autoRun = not selfEx.curItemData.autoRun
        selfEx:DoUpdateAutoRunText_()
	end)
    self.uiBtAutoRun = btAutoRun
    ----------------------------------------------
    self.wnd:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		WxCmn.SetHorizonPos(staticGamePathDir, textGamePathDir, nil, 0)
        WxCmn.SetHorizonPos(textGamePathDir, btChooseGamePath, nil, 0)
        WxCmn.SetHorizonPos(btChooseGamePath, btAutoRun, nil, 0)

        self.uiVerticalLast = staticGamePathDir
        self.uiHorizonLast = btAutoRun
        self:OnCrackChaptchaSize()
        self:OnOtherSize()
	end)
    ----------------------------------------------
    self:CreateCrackChaptchaUi()
    self:CreateOtherUi()
    return selfEx
end

function BasicSetting:GetSelectPathTitleInfo()
    return "选择游戏程序", "*.exe|*.exe"
end

function BasicSetting:CreateCrackChaptchaUi()
    local mainCfg = mainfrm.consoleCfg.mainCfg
    mainCfg.enCrackCaptchaMode = enCrackCaptchaMode.kCCMode_DaMaTu
    local btCrackCaptcha = wx.wxButton(self.wnd, wx.wxID_ANY, "打码兔")
    btCrackCaptcha:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
        event:Skip(true)
        local ac, pwd = FrmDlg.SetDmt(self.wnd, mainCfg.ccDmtAc, mainCfg.ccDmtPwd)
		if ac == nil or pwd == nil then return end
        mainCfg.ccDmtAc = ac
        mainCfg.ccDmtPwd = pwd
        GetMyApp():SetCrackCaptchaDmt(ac, pwd)
	end)
    self.uiBtCrackCaptcha = btCrackCaptcha
end

function BasicSetting:CreateOtherUi()
end

function BasicSetting:OnCrackChaptchaSize()
    if self.uiBtCrackCaptcha == nil then return end
    WxCmn.SetHorizonPos(self.uiHorizonLast, self.uiBtCrackCaptcha, nil, 0)
    self.uiHorizonLast = self.uiBtCrackCaptcha
end

function BasicSetting:OnOtherSize()
end

function BasicSetting:DoUpdateAutoRunText_()
    if self.curItemData.autoRun then
        self.uiBtAutoRun:SetLabel("自动运行")
    else
        self.uiBtAutoRun:SetLabel("不自动运行")
    end
end
