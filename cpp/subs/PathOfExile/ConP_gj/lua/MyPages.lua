require("BasicSetting_p")
MyPages = {
    MyBasicSetting = {
        __index = BasicSetting_p,
        -------------
        GetSelectPathTitleInfo = nil,
        Create = nil,
    },
}
local MyBasicSetting = MyPages.MyBasicSetting
setmetatable(MyBasicSetting, MyBasicSetting)

function MyBasicSetting:GetSelectPathTitleInfo()
    return "选择流放之路程序", "PathOfExile.exe|PathOfExile.exe"
end

function MyBasicSetting:Create(parent)
    self.wnd = wx.wxWindow(parent, wx.wxID_ANY)
    local selfEx = CmnBase.ExtCObj(self, self.wnd)
    --local staticGamePathDir = wx.wxStaticText(self.wnd, wx.wxID_ANY, "游戏目录：")
    local textGamePathDir = wx.wxTextCtrl(self.wnd, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxTE_READONLY)
    local btChooseGamePath = wx.wxButton(self.wnd, wx.wxID_ANY, "选择目录")

    local textGameAc = WxCmn.CreateTextCtrl(self.wnd, "游戏账号", wx.wxTE_LEFT)
    local textGamePw = WxCmn.CreateTextCtrl(self.wnd, "游戏密码", wx.wxTE_LEFT)
    local comboArea,btGameLogin
    ---------------------------------------------------------------------------
    local comboSer = wx.wxComboBox(self.wnd, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, { "" }, wx.wxCB_READONLY)
    local comboWidth = comboSer:GetSize():Scale(4.8, 1):GetWidth()
	comboSer:SetSize(-1, -1, comboWidth, -1)
     staticArea = wx.wxStaticText(self.wnd, wx.wxID_ANY, "服务器:")
        local area = {}
        local areaSerInfo = MyFrmFactory:GetAreaSerInfo_()
        for k, _ in pairs(areaSerInfo) do
            table.insert(area, k)
        end
        table.sort(area)
--        comboArea = wx.wxComboBox(self.wnd, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, area, wx.wxCB_READONLY)
--        comboArea:SetSize(-1, -1, comboWidth, -1)
        local fnHandleSer = function ()
            local serInfo = areaSerInfo[comboSer:GetStringSelection()]
            if serInfo == nil then return end
            comboSer:Clear()
            for _, v in ipairs(serInfo) do
                comboSer:Append(v)
            end
            if #serInfo > 0 then comboSer:SetSelection(0) end
        end
        comboSer:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_TEXT_UPDATED, function(event)
            event:Skip(true)
		    fnHandleSer()
	    end)
        if #area > 0 then
            comboSer:SetSelection(0)
            fnHandleSer()
        end
    ----------------------------------------------------------------------
    btGameLogin = wx.wxButton(self.wnd, wx.wxID_ANY, "登陆游戏")
    btGameLogin:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		local ac = textGameAc:GetValue()
		local pwd = textGamePw:GetValue()
		local ser = comboSer:GetStringSelection()
        local area
		if comboArea then
            area = comboArea:GetStringSelection()
        end
        local info, res = mainfrm:OnDoAddAc(ac, pwd, area, ser)
		if res == false then
            wx.wxMessageBox(tostring("登陆游戏失败:",info), "登陆错误", wx.wxOK)
            return selfEx
        end
        mainfrm:OnP_Login(false)
	end)
    ----------------------------------------------------------------------

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
    local btGameOper = wx.wxButton(self.wnd, wx.wxID_ANY, "游戏操作")
    btGameOper:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
        event:Skip(true)
        GetMyApp():OnOpenMfcDlg();
	end)
   -- self.uiBtAutoRun = btAutoRun
    ----------------------------------------------
    self.wnd:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
        WxCmn.SetHorizonPos(btChooseGamePath, textGamePathDir, nil, 0.1)
        WxCmn.SetVerticalPos(btChooseGamePath, textGameAc)
        WxCmn.SetHorizonPos(textGameAc, textGamePw, nil, 0.1)
        WxCmn.SetHorizonPos(textGamePw, staticArea, nil, 0.1)
        WxCmn.SetHorizonPos(staticArea, comboSer, nil, 0.1)
        WxCmn.SetHorizonPos(comboSer, btGameLogin, nil, 0.1)
        WxCmn.SetVerticalPos(textGameAc,btGameOper)

--      WxCmn.SetHorizonPos(textGamePathDir, btChooseGamePath, nil, 0)
--      WxCmn.SetHorizonPos(btChooseGamePath, btAutoRun, nil, 0)
        --self.uiVerticalLast = staticGamePathDir
        --self.uiHorizonLast = btAutoRun
        --self:OnCrackChaptchaSize()
        self:OnOtherSize()
	end)
    ----------------------------------------------
    --self:CreateCrackChaptchaUi()
    self:CreateOtherUi()
    return selfEx
end
