require("WxCmn")
require("consolecfg")
FrmDlg = {
    AddAc = nil,
    CreateAcCfg = nil,
    SelectAcCfg = nil,
    SelectScript = nil,
    SetDmt = nil,
}

function FrmDlg.AddAc(sink, hasArea)
    if hasArea == nil then hasArea = false end
    assert(hasArea, "若是为false，那就得修改下面的算法")
    local dlg = wx.wxDialog(sink.frame, wx.wxID_ANY, "添加游戏帐号")
    dlg:Center()
    local statusBar = wx.wxStatusBar(dlg, wx.wxID_ANY)
    local staticAc = wx.wxStaticText(dlg, wx.wxID_ANY, "帐号：")
	local staticPwd = wx.wxStaticText(dlg, wx.wxID_ANY, "密码：")
    local staticSer = wx.wxStaticText(dlg, wx.wxID_ANY, "服务器：")
    local ctrlAc = WxCmn.CreateTextCtrl(dlg, "", wx.wxTE_LEFT)
	local ctrlPwd = WxCmn.CreateTextCtrl(dlg, "", wx.wxTE_LEFT)
    local comboSer = wx.wxComboBox(dlg, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, { "" }, wx.wxCB_READONLY)
    local comboWidth = comboSer:GetSize():Scale(4.8, 1):GetWidth()
	comboSer:SetSize(-1, -1, comboWidth, -1)
    local comboArea, staticArea, scaleY
    local areaSerInfo = sink.areaSerInfo
    if hasArea then
        scaleY = 1
        staticArea = wx.wxStaticText(dlg, wx.wxID_ANY, "大区：")
        local area = {}
        for k, _ in pairs(areaSerInfo) do
            table.insert(area, k)
        end
        table.sort(area)
        comboArea = wx.wxComboBox(dlg, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, area, wx.wxCB_READONLY)
        comboArea:SetSize(-1, -1, comboWidth, -1)
        local fnHandleSer = function ()
            local serInfo = areaSerInfo[comboArea:GetStringSelection()]
            if serInfo == nil then return end
            comboSer:Clear()
            for _, v in ipairs(serInfo) do
                comboSer:Append(v)
            end
            if #serInfo > 0 then comboSer:SetSelection(0) end
        end
        comboArea:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_TEXT_UPDATED, function(event)
            event:Skip(true)
		    fnHandleSer()
	    end)
        if #area > 0 then
            comboArea:SetSelection(0)
            fnHandleSer()
        end
    else
        scaleY = 1.6
    end
    local btReg = wx.wxButton(dlg, wx.wxID_ANY, "添加")

    dlg:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		local ac = ctrlAc:GetValue()
		local pwd = ctrlPwd:GetValue()
		local ser = comboSer:GetStringSelection()
        local area
		if comboArea then
            area = comboArea:GetStringSelection()
        end
        local info, res = sink:OnDoAddAc(ac, pwd, area, ser)
		if res == false then
            statusBar:SetStatusText(info)
        else
            statusBar:SetStatusText("添加成功")
        end
	end)

    dlg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlg:GetClientSize()
        WxCmn.SetStatusBarSize(statusBar, clientSize)
		local centerSize = dlg:GetClientSize():Scale(0.5, 0.5)
		local staticAcPos = wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.2)
		staticAc:Move(staticAcPos)
		local width = (staticAc:GetSize():GetWidth() + staticSer:GetSize():GetWidth()) * 0.5

		WxCmn.SetVerticalPos(staticAc, staticPwd, nil, scaleY)
		WxCmn.SetHorizonPos(staticAc, ctrlAc, nil, nil, width, width)
		WxCmn.SetHorizonPos(staticPwd, ctrlPwd, nil, nil, width, width)
        if staticArea then
            WxCmn.SetVerticalPos(staticPwd, staticArea, nil, scaleY)
            WxCmn.SetVerticalPos(staticArea, staticSer, nil, scaleY)
            WxCmn.SetHorizonPos(staticArea, comboArea, nil, nil, width, width)
        else
            WxCmn.SetVerticalPos(staticPwd, staticSer, nil, scaleY)
        end
        WxCmn.SetHorizonPos(staticSer, comboSer, nil, nil, width, width)
		btReg:Move(wx.wxPoint(centerSize:GetWidth() - btReg:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.73))
	end)

    dlg:ShowModal()
    dlg:Destroy()
end

function FrmDlg.CreateAcCfg(sink, title)
    local dlg = wx.wxDialog(sink.frame, wx.wxID_ANY, title)
    dlg:Center()
    local statusBar = wx.wxStatusBar(dlg, wx.wxID_ANY)
    local staticCfgName = wx.wxStaticText(dlg, wx.wxID_ANY, "配置名：")
    local ctrlCfgName = WxCmn.CreateTextCtrl(dlg, "", wx.wxTE_LEFT, 25)
    local staticChoose = wx.wxStaticText(dlg, wx.wxID_ANY, "继承自：")
    local comboChoose = wx.wxComboBox(dlg, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, sink:GetSharedCfgs(), wx.wxCB_READONLY)
    local comboWidth = comboChoose:GetSize():Scale(2.8, 1):GetWidth()
	comboChoose:SetSize(-1, -1, comboWidth, -1)
    local cfg = sink.consoleCfg
    comboChoose:SetValue(cfg.acGlobalCfgName)
    local btCreate = wx.wxButton(dlg, wx.wxID_ANY, "创建")

    local cfgName, parentCfgName
    dlg:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		cfgName = ctrlCfgName:GetValue()
        if #cfgName < 1 then
            statusBar:SetStatusText("请输入配置的名称")
            return
        end
		parentCfgName = comboChoose:GetStringSelection()
        if #parentCfgName < 1 then
            statusBar:SetStatusText("请选择继承的配置")
            return
        end
        if not cfg:ValidAcCfgName(cfgName) then
            statusBar:SetStatusText("无效的配置名")
            return
        end
        statusBar:SetStatusText("")
        dlg:Close()
	end)
    dlg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlg:GetClientSize()
        WxCmn.SetStatusBarSize(statusBar, clientSize)
		local centerSize = dlg:GetClientSize():Scale(0.5, 0.5)
		local staticCfgNamePos = wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.2)
		staticCfgName:Move(staticCfgNamePos)

		WxCmn.SetVerticalPos(staticCfgName, staticChoose)
		WxCmn.SetHorizonPos(staticCfgName, ctrlCfgName)
		WxCmn.SetHorizonPos(staticChoose, comboChoose)
		btCreate:Move(wx.wxPoint(centerSize:GetWidth() - btCreate:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.73))
	end)

    dlg:ShowModal()
    dlg:Destroy()
    return cfgName, parentCfgName
end

function FrmDlg.SelectAcCfg(sink, title, butCfg)
    local dlg = wx.wxDialog(sink.frame, wx.wxID_ANY, title)
    dlg:Center()
    local comboCfg = wx.wxComboBox(dlg, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, sink:GetSharedCfgs(), wx.wxCB_READONLY)
    local comboWidth = comboCfg:GetSize():Scale(2.8, 1):GetWidth()
	comboCfg:SetSize(-1, -1, comboWidth, -1)
    local cfg = sink.consoleCfg
    comboCfg:SetValue(cfg.acGlobalCfgName)
    local btChoose = wx.wxButton(dlg, wx.wxID_ANY, "选择")

    local selectName = ""

    dlg:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		selectName = comboCfg:GetStringSelection()
        dlg:Close()
	end)

    dlg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlg:GetClientSize()
		local centerSize = dlg:GetClientSize():Scale(0.5, 0.5)
        local comboSize = comboCfg:GetSize()
        local centerX = centerSize:GetWidth()
        local centerY = centerSize:GetHeight()
		local comboSerPos = wx.wxPoint(centerX - comboSize:GetWidth() / 2, centerY - comboSize:GetHeight() * 3)
		comboCfg:Move(comboSerPos)
        local btSize = btChoose:GetSize()
        local btPos = wx.wxPoint(centerX - btSize:GetWidth() / 2, clientSize:GetHeight() - btSize:GetHeight() * 2)
        btChoose:Move(btPos)
	end)

    dlg:ShowModal()
    dlg:Destroy()
    return selectName
end

function FrmDlg.SelectScript(sink, title)
    local app = GetMyApp()
    local tbl = {}
    if app:GetDirFileNames(tbl, app:GetScriptDirName(), ".lua", false) < 0 then return end

    local dlg = wx.wxDialog(sink.frame, wx.wxID_ANY, title)
    dlg:Center()
    local comboCfg = wx.wxComboBox(dlg, wx.wxID_ANY, "", wx.wxDefaultPosition, wx.wxDefaultSize, tbl, wx.wxCB_READONLY)
    local comboWidth = comboCfg:GetSize():Scale(2.8, 1):GetWidth()
	comboCfg:SetSize(-1, -1, comboWidth, -1)
    local cfg = sink.consoleCfg
    comboCfg:SetValue(cfg.acGlobalCfgName)
    local btChoose = wx.wxButton(dlg, wx.wxID_ANY, "选择")

    local selectName

    dlg:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		selectName = comboCfg:GetStringSelection()
        dlg:Close()
	end)

    dlg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlg:GetClientSize()
		local centerSize = dlg:GetClientSize():Scale(0.5, 0.5)
        local comboSize = comboCfg:GetSize()
        local centerX = centerSize:GetWidth()
        local centerY = centerSize:GetHeight()
		local comboSerPos = wx.wxPoint(centerX - comboSize:GetWidth() / 2, centerY - comboSize:GetHeight() * 3)
		comboCfg:Move(comboSerPos)
        local btSize = btChoose:GetSize()
        local btPos = wx.wxPoint(centerX - btSize:GetWidth() / 2, clientSize:GetHeight() - btSize:GetHeight() * 2)
        btChoose:Move(btPos)
	end)

    dlg:ShowModal()
    dlg:Destroy()
    return selectName
end

function FrmDlg.SetDmt(wndParent, oldAc, oldPwd)
    local dlg = wx.wxDialog(wndParent, wx.wxID_ANY, "设置打码兔帐号信息")
    dlg:Center()
    local staticAc = wx.wxStaticText(dlg, wx.wxID_ANY, "帐号：")
	local staticPwd = wx.wxStaticText(dlg, wx.wxID_ANY, "密码：")
    local ctrlAc = WxCmn.CreateTextCtrl(dlg, "", wx.wxTE_LEFT)
	local ctrlPwd = WxCmn.CreateTextCtrl(dlg, "", wx.wxTE_LEFT)
    if oldAc then ctrlAc:SetValue(oldAc) end
    if oldPwd then ctrlPwd:SetValue(oldPwd) end

    local btSet = wx.wxButton(dlg, wx.wxID_ANY, "设置")

    local ac, pwd
    dlg:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_BUTTON_CLICKED, function()
		ac = ctrlAc:GetValue()
		pwd = ctrlPwd:GetValue()
        dlg:Close()
	end)

    dlg:Connect(wx.wxEVT_SIZE, function(event)
		event:Skip(true)
		local clientSize = dlg:GetClientSize()
		local centerSize = dlg:GetClientSize():Scale(0.5, 0.5)
		local staticAcPos = wx.wxPoint(centerSize:GetWidth() * 0.35, centerSize:GetHeight() * 0.2)
		staticAc:Move(staticAcPos)
		local width = staticAc:GetSize():GetWidth()

		WxCmn.SetVerticalPos(staticAc, staticPwd, nil, 1.5)
		WxCmn.SetHorizonPos(staticAc, ctrlAc, nil, nil, width, width)
		WxCmn.SetHorizonPos(staticPwd, ctrlPwd, nil, nil, width, width)
        
		btSet:Move(wx.wxPoint(centerSize:GetWidth() - btSet:GetSize():GetWidth() * 0.5, clientSize:GetHeight() * 0.63))
	end)

    dlg:ShowModal()
    dlg:Destroy()

    return ac, pwd
end
