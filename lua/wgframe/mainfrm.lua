require("consolecfg")
require("frmfactory")
require("FrmDlg")
mainfrm = {
    consoleCfg = nil,
	frame = nil,
	menuBar = nil,
	statusBar = nil,
	acList = nil,
	middleArea = nil,
	headerCtrl = nil,
    myApp = GetMyApp(),
    uiOperHandler = GetMyApp():GetAppUiOper(),
    factory = nil,
    curSelectedCfg = nil,
    curSelectedItem = nil,
    areaSerInfo = nil,
	---------------------------------------
	CreateFrm = nil,
	CreateWorkArea = nil,
    GetAcListCfg = nil,
    GetSharedCfgs = nil,
    UpdateSelectedCfgInfo = nil,
    UpdateAcListItem = nil,
    -----------------------------------
    OnDoAddAc = nil,
    -----------------------------------
    OnAddAc = nil,
    OnCreateAcCfg = nil,
    OnSelectAcCfg = nil,
    OnImportAcCfg = nil,
    OnExportAcCfg = nil,
    OnP_ChangeAcCfg = nil,
    OnP_ChangeScript = nil,
    OnP_DeleteAc = nil,
    OnP_Login = nil,
    OnP_CancelLogin = nil,
    OnP_Run = nil,
    OnP_Stop = nil,
    OnP_AcCate = nil,
    OnP_ResetAcState = nil,
    OnP_CloseGame = nil,
    OnShowGameWnd = nil,
    OnRunFirstConnection = nil,
    OnStopFirstConnection = nil,
    -----------------test
    OnTestDoFile = nil,
    -----------------------------------
    --sinks
    OnS_GetItemData = nil,
    OnS_UpdateItem = nil,
    OnS_Login = nil,
    OnS_CancelLogin = nil,
    OnS_AttachLoginSession = nil,
    OnS_GetAcItemInfo = nil,
    OnS_LoginDisconnect = nil,
    OnS_AttachGameSession = nil,
    OnS_GameDisconnect = nil,
    OnS_SetEndDateTime = nil,
    OnS_SetVersion = nil,
    OnS_OutputScriptInfo = nil,
    OnS_ScriptState = nil,
    OnS_RunScript = nil,
    OnS_TimerDo = nil,
    OnS_RunTheScript = nil,
    -----------------------------------
    NewFrmFactory_ = nil,
}

function mainfrm:CreateFrm(title)
    assert(self.factory == nil)
    self.factory = self:NewFrmFactory_()
    local factory = self.factory
    assert(factory)
    self.areaSerInfo = factory:GetAreaSerInfo_()
    assert(consolecfg)
    consolecfg.mainCfg:ResolveCfgs()
    self.consoleCfg = consolecfg
    self.curSelectedCfg = consolecfg.acGlobalCfgName
	self.frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, title, wx.wxDefaultPosition, wx.wxDefaultSize
				--, style
					)
	local size = self.frame:GetSize()
	size:Scale(3, 2.5)
	self.frame:SetSize(size)
	----------------------------------------------------
	self.statusBar = factory:NewFrmStatusBar_():Create(self.frame:CreateStatusBar())
	
	self.menuBar = factory:NewMenuBar_():CreateMenuBar(self)
	if self.menuBar then
		self.frame:SetMenuBar(self.menuBar:GetLuaBase())
	end
	
	self:CreateWorkArea()
	----------------------------------------------------
	self.frame:CenterOnScreen()
	self.frame:Show(true)
    self:UpdateSelectedCfgInfo()
    ShellM("mainfrm.myApp:OnFrameCreated()")
	return CmnBase.ExtCObj(self, self.frame)
end

function mainfrm:CreateWorkArea()
    local factory = self.factory
	local splitterMain = wx.wxSplitterWindow(self.frame, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)
	self.acList = factory:NewAcList_():Create(splitterMain, self)
	assert(self.acList)
	local splitterLower = wx.wxSplitterWindow(splitterMain, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)
	self.middleArea = factory:NewMidArea_():Create(splitterLower)
	assert(self.middleArea)
	self.headerCtrl = factory:NewHeaderCtrl_():Create(splitterLower)
	assert(self.headerCtrl)
	splitterLower:SetSashGravity(0.005)
	splitterLower:SplitHorizontally(self.middleArea:GetLuaBase(), self.headerCtrl:GetLuaBase())
	
	splitterMain:SetSashGravity(0.6)
    splitterMain:SplitHorizontally(self.acList:GetLuaBase(), splitterLower)
    --------------------------------------------------
    for _, v in self:GetAcListCfg():Iterator() do
        self.acList:AddItem(v)
    end
end

function mainfrm:GetAcListCfg()
    return self.consoleCfg:GetAcList()
end

function mainfrm:GetSharedCfgs(butCfg)
    local res = {}
    local iterCfg = self.consoleCfg:IteratorSharedCfgName()
    if butCfg == nil then
        for cfg in iterCfg do
            table.insert(res, cfg)
        end
    else
        for cfg in iterCfg do
            if butCfg ~= cfg then
                table.insert(res, cfg)
            end
        end
    end
    table.sort(res)
    return res
end

function mainfrm:UpdateSelectedCfgInfo()
    if self.curSelectedItem ~= nil then
        self.statusBar:SetCurSelectedCfg("帐号列表项[" .. self.curSelectedItem + 1 .. "]独有的配置")
        local itemData = self.acList:GetItemData(self.curSelectedItem)
        self.headerCtrl:UpdatePagesFromCurSelected(itemData)
        return
    end
    self.statusBar:SetCurSelectedCfg("配置：" .. self.curSelectedCfg)
    local consoleCfg = self.consoleCfg
    local curCfg = consoleCfg:FindCfg(consoleCfg:GetAcCfgFullName(self.curSelectedCfg))
    assert(curCfg)
    --curCfg就是itemData，只不过没有ac、pwd、ser等属性而已
    self.headerCtrl:UpdatePagesFromCurSelected(curCfg)
end

function mainfrm:UpdateAcListItem(item, itemData)
    self.headerCtrl:UpdatePagesFromItemData(item, itemData)
    self.myApp:OnUpdateAcItem(item, itemData:GetItemKey(), itemData)
end

function mainfrm:OnDoAddAc(ac, pwd, area, ser)
    local info, res
    if area == nil then
        info, res = self:GetAcListCfg():AddItem(ac, pwd, nil, ser)
    else
        info, res = self:GetAcListCfg():AddItem(ac, pwd, nil, area, ser)
    end
    if res ~= false then
        self.acList:AddItem(info)
    end
    return info, res
end

function mainfrm:OnAddAc()
    FrmDlg.AddAc(self, true)
end

function mainfrm:OnCreateAcCfg()
    local cfgName, parentCfgName = FrmDlg.CreateAcCfg(self, "创建帐号配置")
    if cfgName == nil or #cfgName < 1 then return end
    self.consoleCfg:LoadAcCfg(cfgName, parentCfgName, true)
end

function mainfrm:OnSelectAcCfg()
    local dstCfg = FrmDlg.SelectAcCfg(self, "选择当前要查看和修改的配置")
    if #dstCfg < 1 then return end
    if dstCfg == self.curSelectedCfg then
        if self.curSelectedItem == nil then return end
        self.acList:SelectItem(-1, false)
        self:UpdateSelectedCfgInfo()
        return
    end
    self.curSelectedCfg = dstCfg
    self.acList:SelectItem(-1, false)
    self:UpdateSelectedCfgInfo()
end

function mainfrm:OnImportAcCfg()
    local fileName = WxCmn.OpenFile(self.frame, "导入帐号配置", "(帐号.txt)|*.txt")
    if fileName == nil then return end
    local mgr = ConsoleAcCfgMgr()
    if not mgr:Load(fileName) then return end
    local acList = self:GetAcListCfg()
    local areaSerInfo = self.areaSerInfo
    for item in Iterate(mgr:Iterator()) do
        if acList:FindItem(item.ac_, item.area_, item.ser_) == nil then
           local serInfo = areaSerInfo[item.area_]
           if serInfo then
                if CmnBase.TableFind(serInfo, item.ser_) then
                    self:OnDoAddAc(item.ac_, item.pwd_, item.area_, item.ser_)
                end
           end
        end
    end
end

function mainfrm:OnExportAcCfg()
    local dir = WxCmn.OpenDir(self.frame, "选择帐号保存的目录")
    if dir == nil then return end
    local mgr = ConsoleAcCfgMgr()
    for k, v in self:GetAcListCfg():Iterator() do
        mgr:Add(ConsoleAcCfgItemInfo(v.ac, v.pwd, v.area, v.ser))
    end
    mgr:Save(dir .. "/帐号.txt")
end

function mainfrm:OnP_ChangeAcCfg(isAll)
    local dstCfg = FrmDlg.SelectAcCfg(self, "选择配置")
    if #dstCfg < 1 then return end
    local acList = self:GetAcListCfg()
    local stateNil = acList.stateNil
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData:GetState() == stateNil and itemData:GetItemCfgName() ~= dstCfg then
            itemData:SetItemCfgName(dstCfg)
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnP_ChangeScript(isAll)
    local script = FrmDlg.SelectScript(self, "选择脚本")
    if script == nil then return end
    local acList = self:GetAcListCfg()
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData.script ~= script then
            itemData.script = script
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnP_DeleteAc(isAll)
    if isAll == nil then isAll = true end
    local acList = self:GetAcListCfg()
    if isAll then
        for item = self.acList:GetItemCount() - 1, 0, -1 do
            local itemData = acList:FindItemByIdx(item + 1)
            local key = itemData:GetItemKey()
            if not self.uiOperHandler:IsLogined(key) then
                self.acList:DeleteItem(item)
                acList:DeleteItemByIdx(item + 1)
                self.myApp:OnLuaDeleteAc(key)
            end
        end
    else
        local items = {}
        for item in self.acList:IteratorSelected() do
            table.insert(items, item)
        end
        if #items < 1 then return end
        items = CmnBase.Reverse(items)
        for _, item in ipairs(items) do
            local itemData = acList:FindItemByIdx(item + 1)
            local key = itemData:GetItemKey()
            if not self.uiOperHandler:IsLogined(key) then
                self.acList:DeleteItem(item)
                acList:DeleteItemByIdx(item + 1)
                self.myApp:OnLuaDeleteAc(key)
            end
        end
    end
end

function mainfrm:OnP_Login(isAll)
    if not self.myApp:CanLoginGameAc() then return end
    local gamePath = self.consoleCfg.mainCfg.gamePath
    if gamePath == nil then
        assert(false)
		return
    end

    local acList = self:GetAcListCfg()
    local stateNil = acList.stateNil
    local stateWaitLogin = acList.stateWaitLogin
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData.checked == true and itemData:GetState() == stateNil then
            local itemKey = itemData:GetItemKey()
            self:OnS_GetAcItemInfo(itemKey)
            if self.uiOperHandler:Login(itemKey, gamePath, false) then
                itemData:SetState(stateWaitLogin)
                self.acList:UpdateItem(item)
            end
        end
    end
end

function mainfrm:OnP_CancelLogin(isAll)
    local acList = self:GetAcListCfg()
    local stateNil = acList.stateNil
    local stateWaitLogin = acList.stateWaitLogin
    local stateLogining = acList.stateLogining
    local stateWaitLogined = acList.stateWaitLogined
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        local state = itemData:GetState()
        if state == stateLogining or state == stateWaitLogin or state == stateWaitLogined then
            if self.uiOperHandler:CancelLogin(itemData:GetItemKey()) then
                itemData:SetState(stateNil)
                self.acList:UpdateItem(item)
            end
        end
    end
end

local function DoRunScript(self, itemData, itemKey)
    if itemKey == nil then itemKey = itemData:GetItemKey() end
    local cfg = itemData:GetItemSmartCfg()
    cfg = cfg:GetCfgTbl()
    return self.uiOperHandler:Run(itemKey, itemData.script, pluto.persist({}, cfg))
end

function mainfrm:OnP_Run(isAll)
    local acList = self:GetAcListCfg()
    local stateLogined = acList.stateLogined
    local stateStopped = acList.stateStopped
    local stateWaitRun = acList.stateWaitRun
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        local curState = itemData:GetState()
        if curState == stateLogined or curState == stateStopped then
            if DoRunScript(self, itemData) then
                itemData:SetState(stateWaitRun)
                self.acList:UpdateItem(item)
            end
        end
    end
end

function mainfrm:OnP_Stop(isAll)
    local acList = self:GetAcListCfg()
    local stateRunning = acList.stateRunning
    local stateWaitStop = acList.stateWaitStop
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData:GetState() == stateRunning then
            if self.uiOperHandler:Stop(itemData:GetItemKey()) then
                itemData:SetState(stateWaitStop)
                self.acList:UpdateItem(item)
            end
        end
    end
end

function mainfrm:OnP_AcCate(isAll)
    local acList = self:GetAcListCfg()
    local cateNormal = acList.cateNormal
    local cateStore = acList.cateStore
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if not self.uiOperHandler:IsLogined(itemData:GetItemKey()) then
            if itemData:GetAcCate() == cateNormal then
                itemData:SetAcCate(cateStore)
            else
                itemData:SetAcCate(cateNormal)
            end
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnP_ResetAcState(isAll)
    local acList = self:GetAcListCfg()
    local stateNormalMax = acList.stateNormalMax
    local stateNil = acList.stateNil
    local stateWaitStop = acList.stateWaitStop
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        local curState = itemData:GetState()
        if curState >= stateNormalMax then
            itemData:SetState(stateNil)
            self.acList:UpdateItem(item)
        elseif curState == stateWaitStop then
            itemData:SetState(acList.stateStopped)
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnP_CloseGame(isAll)
    local acList = self:GetAcListCfg()
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        local key = itemData:GetItemKey()
        if self.uiOperHandler:IsLogined(key) then
            self.uiOperHandler:CloseGame(key)
        end
    end
end

function mainfrm:OnShowGameWnd()
    local item = self.acList:GetLastSelectedItem()
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItemByIdx(item + 1)
    local key = itemData:GetItemKey()
    if self.uiOperHandler:IsLogined(key) then
        self.uiOperHandler:ShowGameWnd(key)
    end
end

function mainfrm:OnRunFirstConnection()
    local acList = self:GetAcListCfg()
    local stateLogined = acList.stateLogined
    local stateStopped = acList.stateStopped
    for item = 0, self.acList:GetItemCount() - 1 do
        local itemData = acList:FindItemByIdx(item + 1)
        local curState = itemData:GetState()
        if curState == stateLogined or curState == stateStopped then
            if DoRunScript(self, itemData, itemKey) then
                itemData:SetState(acList.stateWaitRun)
                self.acList:UpdateItem(item)
                break
            end
        end
    end    
end

function mainfrm:OnStopFirstConnection()
    local acList = self:GetAcListCfg()
    local stateRunning = acList.stateRunning
    for item = 0, self.acList:GetItemCount() - 1 do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData:GetState() == stateRunning then
            if self.uiOperHandler:Stop(itemData:GetItemKey()) then
                itemData:SetState(acList.stateWaitStop)
                self.acList:UpdateItem(item)
                break
            end
        end
    end    
end

function mainfrm:OnTestDoFile()
    dofile([[F:\tmp\test.lua]])
end

function mainfrm:OnS_GetItemData(itemKey)
    return self:GetAcListCfg():FindItem(itemKey)
end

function mainfrm:OnS_UpdateItem(itemKey)
    local item = self.acList:FindItemByAc(itemKey)
    self.acList:UpdateItem(item)
end

function mainfrm:OnS_Login(itemKey, addToFirst)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    if addToFirst == nil then addToFirst = false end
    local gamePath = self.consoleCfg.mainCfg.gamePath
    local stateNil = acList.stateNil
    local stateWaitLogin = acList.stateWaitLogin
    if itemData.checked == true and itemData:GetState() == stateNil and self.uiOperHandler:Login(itemKey, gamePath, addToFirst) then
        itemData:SetState(stateWaitLogin)
        local item = self.acList:FindItemByData(itemData)
        self.acList:UpdateItem(item)
    end
end

function mainfrm:OnS_CancelLogin(itemKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local stateNil = acList.stateNil
    local stateWaitLogin = acList.stateWaitLogin
    local stateLogining = acList.stateLogining
    local stateWaitLogined = acList.stateWaitLogined
    local state = itemData:GetState()
    if state == stateLogining or state == stateWaitLogin or state == stateWaitLogined then
        if self.uiOperHandler:CancelLogin(itemKey) then
            itemData:SetState(stateNil)
            local item = self.acList:FindItemByData(itemData)
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnS_AttachLoginSession(itemKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    if itemData:GetState() ~= acList.stateWaitLogin then
        assert(false)
        return
    end
    itemData:SetState(acList.stateLogining)
    local item = self.acList:FindItemByData(itemData)
    self.acList:UpdateItem(item)
end

function mainfrm:OnS_GetAcItemInfo(itemKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    self.myApp:SetAcItemInfo(itemKey, itemData.ac, itemData.pwd, itemData.area, itemData.ser)
end

function mainfrm:OnS_LoginDisconnect(itemKey, stateKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end

    local stateNew = acList[stateKey]
    assert(stateNew)
    local state = itemData:GetState()
    if state >= acList.stateLogining and state < acList.stateLoginedMin then
        itemData:SetState(stateNew)
        local item = self.acList:FindItemByData(itemData)
        self.acList:UpdateItem(item)
    end
end

function mainfrm:OnS_AttachGameSession(itemKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local stateWaitLogin = acList.stateWaitLogin
    local stateLogining = acList.stateLogining
    local stateWaitLogined = acList.stateWaitLogined
    local state = itemData:GetState()
    if state == stateLogining or state == stateWaitLogin or state == stateWaitLogined then
        itemData:SetState(acList.stateLogined)
        
        local cfg = itemData:GetItemSmartCfg()
        cfg = cfg:GetCfgTbl()
        self.myApp:SetGameSessionCfg(itemKey, pluto.persist({}, cfg))

        local item = self.acList:FindItemByData(itemData)
        self.acList:UpdateItem(item)
    end
end

function mainfrm:OnS_GameDisconnect(itemKey, stateKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local stateNew = acList[stateKey]
    assert(stateNew)
    local stateLoginedMin = acList.stateLoginedMin
    local stateLoginedMax = acList.stateLoginedMax    
    local state = itemData:GetState()
    if state >= stateLoginedMin and state <= stateLoginedMax then
        itemData:SetState(stateNew)
        local item = self.acList:FindItemByData(itemData)
        self.acList:UpdateItem(item)
    end
end

function mainfrm:OnS_SetEndDateTime(time)
    self.statusBar:SetEndDateTime(time)
end

function mainfrm:OnS_SetVersion(version)
    self.statusBar:SetVersion(version)
end

function mainfrm:OnS_OutputScriptInfo(itemKey, info)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local item = self.acList:FindItemByData(itemData)
    local acListCtr = self.acList
    acListCtr:OutputScriptInfo(item, info)
    acListCtr:UpdateItemLastTime(item, itemData)
end

function mainfrm:OnS_ScriptState(itemKey, isStartScript)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local stateLogined = acList.stateLogined
    local stateStopped = acList.stateStopped
    local state = itemData:GetState()
    if state < stateLogined or state > stateStopped then return end
    if isStartScript then
        local stateRunning = acList.stateRunning
        if state ~= stateRunning then
            itemData:SetState(stateRunning)
            local item = self.acList:FindItemByData(itemData)
            self.acList:UpdateItem(item)
        end
    else
        local stateStopped = acList.stateStopped
        if state ~= stateStopped then
            itemData:SetState(stateStopped)
            local item = self.acList:FindItemByData(itemData)
            self.acList:UpdateItem(item)
        end
    end    
end

function mainfrm:OnS_RunScript(itemKey)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    local curState = itemData:GetState()
    if curState == acList.stateLogined or curState == acList.stateStopped then
        if DoRunScript(self, itemData, itemKey) then
            itemData:SetState(acList.stateWaitRun)
            local item = self.acList:FindItemByData(itemData)
            self.acList:UpdateItem(item)
        end
    end
end

function mainfrm:OnS_TimerDo()
    local acList = self:GetAcListCfg()
    local stateNormalMax = acList.stateNormalMax
    local stateDoneMax = acList.stateDoneMax
    local stateNil = acList.stateNil
    local timeNow = DateTimeLocal()
    for item in self.acList:Iterator() do
        local itemData = acList:FindItemByIdx(item + 1)
        local curState = itemData:GetState()
        if curState > stateNormalMax and curState <= stateDoneMax then
            local timeTmp = DateTimeLocal()
            timeTmp:FromEpochTime(int64(itemData.timeRestricted))
            if timeTmp < timeNow then
                itemData:SetState(stateNil)
                self.acList:UpdateItem(item)
            end
        end
    end
    -------------------------------
end

function mainfrm:OnS_RunTheScript(itemKey, scriptName)
    local acList = self:GetAcListCfg()
    local itemData, item = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false)
        return
    end
    local curState = itemData:GetState()
    if curState == acList.stateLogined or curState == acList.stateStopped then
        local cfg = itemData:GetItemSmartCfg()
        cfg = cfg:GetCfgTbl()
        if self.uiOperHandler:Run(itemKey, scriptName, pluto.persist({}, cfg)) then
            itemData:SetState(acList.stateWaitRun)
            self.acList:UpdateItem(item - 1)
        end
    end
end

function mainfrm:NewFrmFactory_()
    return frmfactory
end
