MyPages = {
    basic = {
        wnd = nil,
        -------------
        Create = nil,
    },
}
local basic = MyPages.basic

function basic:Create(parent)
    self.wnd = wx.wxWindow(parent, wx.wxID_ANY)
    local staticDir = wx.wxStaticText(self.wnd, wx.wxID_ANY, "模拟器目录：")
    local textDir = wx.wxTextCtrl(self.wnd, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxTE_READONLY)
    local btChoose = wx.wxButton(self.wnd, wx.wxID_ANY, "选择")

    btChoose:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
        event:Skip(true)
		local filePath = WxCmn.OpenFile(self.wnd, "逍遥安卓执行程序", "MEmuConsole.exe|MEmuConsole.exe")
        if filePath == nil then return end
        mainfrm.consoleCfg.mainCfg.gamePath = filePath
        textDir:SetValue(filePath)
        GetMyApp():OnSetGamePath(filePath, true)
	end)
    textDir:Connect(wx.wxID_ANY, wx.wxEVT_SET_FOCUS, function(event)
        event:Skip(true)
		mainfrm.statusBar:PushStatusText(textDir:GetValue())
	end)
    textDir:Connect(wx.wxID_ANY, wx.wxEVT_KILL_FOCUS, function(event)
        event:Skip(true)
		mainfrm.statusBar:PopStatusText()
	end)
    self.wnd:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		WxCmn.SetHorizonPos(staticDir, textDir, nil, 0)
        WxCmn.SetHorizonPos(textDir, btChoose, nil, 0)
	end)
    local thePath = mainfrm.consoleCfg.mainCfg.gamePath
    if thePath ~= nil then 
        textDir:SetValue(thePath)
        GetMyApp():OnSetGamePath(thePath, false)
    end
    return CmnBase.ExtCObj(self, self.wnd)
end