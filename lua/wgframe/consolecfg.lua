require("cmncfg")
assert(cmncfg)
consolecfg = {
	__index = cmncfg,	--derived from cmncfg
	-----------------------------------
	acGlobalCfg = nil,
	acGlobalCfgName = "全局",
	acCfgNamePrefix = "_",
	-----------------------------------
	--[[
	有的属性是可继承的，那么就写在AcGlobalCfgOper(帐号全局配置)里面；有的是不可继承的，那么就写在AcListItemOper里面。
	有的属性是需要保存的，那么就用self.xxx的方式；有的属性不需要保存的，那么就在AcListItemOper里定义个默认值。
	]]
	AcListItemOper = {
--		stateName = "",
		scriptInfo = "",
		lastTime = "",
		state_ = 0,		--不要直接去访问
		------------------------------------
		GetItemSmartCfg = nil,
		GetState = nil,
		SetState = nil,
		GetStateName = nil,
        GetAcCate = nil,
        SetAcCate = nil,
        GetAcCateName = nil,
		GetListOper = nil,
		GetItemKey = nil,
		GetItemCfgName = nil,
		SetItemCfgName = nil,
        GetExcludeSelfCfgName = nil,
		DoIndex = nil,
		DoNewIndex = nil,
	},
	-----------------------------------
	AcListOper = {
		cateNormal = 1,		--普通号
		cateStore = 2,		--仓库号
		---------------------------
		stateNil = 0,		--空状态，未知状态
        stateWaitLogin = 1, --等待登陆
		stateLogining = 2,	--登陆中
        stateWaitLogined = 3,   --等待连接
        stateLoginedMin = 5,
        stateLogined = 5,   --已登陆，这个数值不要随便变动
        stateWaitRun = 6,   --等待运行
        stateRunning = 7,	--运行中
        stateWaitStop = 8,  --等待停止
        stateStopped = 9,   --已停止，从stateLogined到stateStopped之间的数值不要随便改变
        stateLoginedMax = 9,
		stateNormalMax = 99,	--占位符，小于此数值的状态，说明帐号是正常的
		--大于以下数值的，表示一段时间内帐号不能登陆了
		stateTaskDone = 100,	--已做完
        stateRolesUseout = 101, --角色刷完
		stateDoneMax = 199,		--占位符
		--大于以下数值的，表示帐号不能用了
		stateAcPwdErr = 200,	--帐号密码错误
		stateBan = 201,		--已封
		stateFreeze = 202,	--冻结
		---------------------------
		Cate2Name = nil,
		State2Name = nil,
		GetStateNameMap = nil,
		GenItemKey = nil,
		AddItem = nil,			--不能修改帐号、密码、大区、服务器的属性
		DeleteItem = nil,
        DeleteItemByIdx = nil,
		FindItem = nil,
        FindItemByIdx = nil,
		Iterator = nil,
		CheckPwd = nil,
	},
	-----------------------------------
	MainCfgOper = {
		ResolveCfgs = nil,
        GetAcCfgInfo = nil,     --帐号配置信息
	},
	-----------------------------------
	AcGlobalCfgOper = {
	},
	-----------------------------------
    DeleteCfg = nil,
    GetAcList = nil,
	GetAcCfgFullName = nil,
    GetCfgNameFromFullName = nil,
	IsAcCfgFullName = nil,
	ValidAcCfgName = nil,
    LoadAcCfg = nil,
    IsSharedCfgName = nil,
    IteratorCfgName = nil,
    IteratorSharedCfgName = nil,
    NewCfgFactory_ = nil,
}
setmetatable(consolecfg, consolecfg)
local AcListItemOper = consolecfg.AcListItemOper
local AcListOper = consolecfg.AcListOper
local MainCfgOper = consolecfg.MainCfgOper
local AcGlobalCfgOper = consolecfg.AcGlobalCfgOper


function AcListItemOper:GetItemSmartCfg()
	local cfg = rawget(self, "__ItemSmartCfg__")
	if cfg ~= nil then return cfg end
	local cfgMgr = self:GetSmartCfg():GetCfgMgr()
	local cfgName = self.itemCfgName
	cfg = cfgMgr:FindCfg(cfgMgr:GetAcCfgFullName(cfgName))
	if cfg ~= nil then 
		rawset(self, "__ItemSmartCfg__", cfg)
		return cfg
	end
	cfg = cfgMgr.acGlobalCfg
	assert(cfg)
	rawset(self, "__ItemSmartCfg__", cfg)
	self.itemCfgName = cfgMgr.acGlobalCfg
	return cfg
end

function AcListItemOper:GetState()
	local cfgState = self.state
	if cfgState == nil then return self.state_ end
	local list = self:GetListOper()
	if cfgState >= list.stateNormalMax then return cfgState end
	return self.state_
end

function AcListItemOper:SetState(state)
	assert(state ~= nil)
	local cfgState = self.state
	local list = self:GetListOper()
	if cfgState ~= nil and cfgState >= list.stateNormalMax then
		self.state_ = state
		if state >= list.stateNormalMax then
			self.state = state
		else
			self.state = nil
		end
		return
	end
	if state >= list.stateNormalMax then
		self.state_ = state
		self.state = state
		return
	end
	if cfgState ~= nil then
		self.state = nil
	end
	self.state_ = state
end

function AcListItemOper:GetStateName()
	local state = self:GetState()
	return self:GetListOper():State2Name(state)
end

function AcListItemOper:GetAcCate()
    local res = self.cate
    if res ~= nil then return res end
    return self:GetListOper().cateNormal
end

function AcListItemOper:SetAcCate(cate)
    local res = self.cate
    if res == cate then return end
    local listOper = self:GetListOper()
    if listOper.cateNormal == cate and res == nil then return end
    if listOper.cateNormal == res and cate == nil then return end
    self.cate = cate
end

function AcListItemOper:GetAcCateName()
    return self:GetListOper():Cate2Name(self.cate)
end

function AcListItemOper:GetListOper()
	return self:GetSmartCfg().acList
end

function AcListItemOper:GetItemKey()
	return self:GetListOper():GenItemKey(self.ac, self.area, self.ser)
end

function AcListItemOper:GetItemCfgName()
	return self.itemCfgName
end

function AcListItemOper:SetItemCfgName(cfgName)
	assert(cfgName)
	if self.itemCfgName == cfgName then return end
	rawset(self, "__ItemSmartCfg__", nil)
	local key = self:GetItemKey()
	if key == cfgName then
		local cfgMgr = self:GetSmartCfg():GetCfgMgr()
		cfgMgr:LoadAcCfg(key, self.itemCfgName, false)
		rawset(self, "__IsSelfCfg__", true)
	else
		rawset(self, "__IsSelfCfg__", nil)
		if self.itemCfgName == key then
			local cfgMgr = self:GetSmartCfg():GetCfgMgr()
			cfgMgr:DeleteCfg(cfgMgr:GetAcCfgFullName(key))
		end
	end
	self.itemCfgName = cfgName
end

function AcListItemOper:GetExcludeSelfCfgName()
    local cfgName
    if self.itemCfgName == self:GetItemKey() then
        local cfgMgr = self:GetSmartCfg():GetCfgMgr()
        cfgName = cfgMgr:GetCfgNameFromFullName(cfgMgr:FindDependeceCfgName(cfgMgr:GetAcCfgFullName(self.itemCfgName)))
    else
        cfgName = self.itemCfgName
    end
    return cfgName
end

function AcListItemOper:DoIndex(k)
	return self:GetItemSmartCfg()[k]
end

function AcListItemOper:DoNewIndex(k, v)
    local smartCfg = self:GetItemSmartCfg()
    if smartCfg[k] == v then return true end
	local selfCfg = rawget(self, "__IsSelfCfg__")
	if selfCfg == true then smartCfg[k] = v; return true end
	local key = self:GetItemKey()
	if self:GetItemCfgName() == key then
		rawset(self, "__IsSelfCfg__", true)
		smartCfg[k] = v
		return true
	end
	self:SetItemCfgName(key)
	self:GetItemSmartCfg()[k] = v
	return true
end

function AcListOper:Cate2Name(cate)
	if cate == nil or self.cateNormal == cate then return "普通" end
	return "仓库"
end

function AcListOper:State2Name(state)
	local res = self:GetStateNameMap()[state]
	if res ~= nil then return res end
	if state == nil then return "" end
	assert(false)
end

function AcListOper:GetStateNameMap()
	local map = rawget(self, "__StateNameMap__")
	if map ~= nil then return map end
	map = self:GetSmartCfg():GetCfgMgr().factory:NewStateNameMap_(self)
    assert(map)
	rawset(self, "__StateNameMap__", map)
	return map
end

function AcListOper:GenItemKey(ac, serOrArea, ser)
	assert(ac)
	assert(serOrArea)
	ser = ser or ""
    return GetMyApp():GenAcItemKey(ac, serOrArea, ser)
end

function AcListOper:AddItem(ac, pwd, itemCfgName, serOrArea, ser)
	if type(ac) ~= "string" then
		return "帐号不能为空", false
	end
	if type(pwd) ~= "string" then
		return "密码不能为空", false
	end
	local area = ""
	if ser == nil then
		ser = serOrArea
	else
		area = serOrArea
	end
	if type(area) ~= "string" then
		return "大区不能为空", false
	end
	if type(ser) ~= "string" then
		return "服务器不能为空", false
	end
	if #ac < 1 then
		return "帐号长度太短", false
	end
	if #pwd < 1 then
		return "密码长度太短", false
	end
    --[[
	if area == serOrArea and #area < 1 then
		return "大区长度太短", false
	end
    ]]
	if #ser < 1 then
		return "服务器长度太短", false
	end
	if self:FindItem(ac, area, ser) then
		return "帐号和服务器已存在", false
	end
	if not self:CheckPwd(ac, pwd) then
		return "密码与已存在项的密码不符", false
	end
	
	local cfgMgr = self:GetSmartCfg():GetCfgMgr()
	if itemCfgName == nil then
		itemCfgName = cfgMgr.acGlobalCfgName
	else
		assert(cfgMgr:FindCfg(cfgMgr:GetAcCfgFullName(itemCfgName)))
	end
	local newIdx = #self:GetCfgTbl() + 1
	local cfg = {}
	cfg.ac = ac
	cfg.pwd = pwd
	cfg.area = area
	cfg.ser = ser
	cfg.itemCfgName = itemCfgName
	self[newIdx] = cfg
	return self[newIdx]
end

function AcListOper:DeleteItem(...)
	local v, k = self:FindItem(...)
	if v == nil then return end
	local smart = self:GetSmartCfg()
	local mgr = smart:GetCfgMgr()
	local isAuto = mgr:PersistAutoSave(false)
    local fullCfgName = mgr:GetAcCfgFullName(v:GetItemKey())
    mgr:DeleteCfg(fullCfgName)
--	self[k] = nil
	table.remove(self:GetCfgTbl(), k)
	mgr:UnpersistAutoSave(isAuto)
	smart:Save()
end

function AcListOper:DeleteItemByIdx(idx)
    local item = self[idx]
    if item == nil then return end
    local smart = self:GetSmartCfg()
	local mgr = smart:GetCfgMgr()
	local isAuto = mgr:PersistAutoSave(false)
    local fullCfgName = mgr:GetAcCfgFullName(item:GetItemKey())
    mgr:DeleteCfg(fullCfgName)
    --不能加上这句，否则会有问题,idx + 1后面的元素的key值就不是依次排序下来的了。
--	self[idx] = nil
	table.remove(self:GetCfgTbl(), idx)
	mgr:UnpersistAutoSave(isAuto)
	smart:Save()
end

function AcListOper:FindItem(acOrKey, serOrArea, ser)
	assert(acOrKey)
	local key
	if serOrArea == nil then
		assert(ser == nil)
		key = acOrKey
	else
		key = self:GenItemKey(acOrKey, serOrArea, ser)
	end
	for k, v in ipairs(self:GetCfgTbl()) do
		if self:GenItemKey(v.ac, v.area, v.ser) == key then
			return self[k], k
		end
	end
end

function AcListOper:FindItemByIdx(idx)
    return self[idx]
end

function AcListOper:Iterator()
	local impl, t, k = ipairs(self:GetCfgTbl())
	return function()
		k = impl(t, k)
		if k ~= nil then return k, self[k] end
	end
end

function AcListOper:CheckPwd(ac, pwd)
	for _, v in ipairs(self:GetCfgTbl()) do
		if v.ac == ac then return v.pwd == pwd end
	end
	return true
end

function MainCfgOper:ResolveCfgs()
	local cfgMgr = self:GetCfgMgr()
	assert(cfgMgr.acGlobalCfg == nil)
	assert(cfgMgr.mainCfg)
	local acGloCfgName = cfgMgr:GetAcCfgFullName(cfgMgr.acGlobalCfgName)
	cfgMgr:LoadAllFiles(function(cfgName)
		return cfgMgr:IsAcCfgFullName(cfgName)
	end)
	cfgMgr.acGlobalCfg = cfgMgr:FindCfg(acGloCfgName)
	if cfgMgr.acGlobalCfg == nil then
--        assert(false)
		cfgMgr.acGlobalCfg = cfgMgr:LoadCfg(cfgMgr.MakeCfgInfo(acGloCfgName, -1), true)
	end
end

function MainCfgOper:GetAcCfgInfo()
    local res = self._acCfgInfo__
    if res then return res end
    self._acCfgInfo__ = {}
    return self._acCfgInfo__
end

function consolecfg:DeleteCfg(fullCfgName)
	cmncfg.DeleteCfg(self, fullCfgName)
    self.mainCfg:GetAcCfgInfo()[fullCfgName] = nil
end

function consolecfg:GetAcList()
    return self.mainCfg.acList
end

function consolecfg:GetAcCfgFullName(cfgName)
    if self:IsAcCfgFullName(cfgName) then return cfgName end
	return self.acCfgNamePrefix .. cfgName
end

function consolecfg:GetCfgNameFromFullName(cfgFullName)
    local pos, pos_end = string.find(cfgFullName, self.acCfgNamePrefix)
    if pos ~= 1 then return cfgFullName end
    return string.sub(cfgFullName, pos_end + 1)
end

function consolecfg:IsAcCfgFullName(cfgFullName)
	local pos = string.find(cfgFullName, self.acCfgNamePrefix)
	return pos == 1
end

function consolecfg:ValidAcCfgName(cfgName)
	return not self:IsAcCfgFullName(cfgName)
end

function consolecfg:LoadAcCfg(cfgName, oldCfgName, isShared)
    if oldCfgName == nil then oldCfgName = self.acGlobalCfgName end
    if isShared == nil then isShared = true end
	cfgName = self:GetAcCfgFullName(cfgName)
	oldCfgName = self:GetAcCfgFullName(oldCfgName)
	local res = self:LoadCfg(self.MakeCfgInfo(cfgName, -1, oldCfgName), true)
    if isShared == true then return res end
    if res == nil then return res end
    local acCfgInfo = self.mainCfg:GetAcCfgInfo()
    local item = acCfgInfo[cfgName]
    if item == nil then
        acCfgInfo[cfgName] = {}
        item = acCfgInfo[cfgName]
    end
    item.isShared = isShared
    return res
end

function consolecfg:IsSharedCfgName(cfgName)
    local cfgName = self:GetAcCfgFullName(cfgName)
    local acCfgInfo = self.mainCfg:GetAcCfgInfo()
    local item = acCfgInfo[cfgName]
    if item == nil then return true end
    local res = item.isShared
    if res == nil then return true end
    return res
end

function consolecfg:IteratorCfgName()
    local allFiles = self:GetAllFiles():GetCfgTbl()
    assert(allFiles)
    local impl, t, k = pairs(allFiles)
	return function()
        while true do
            k = impl(t, k)
            if k == nil then break end
            if self:IsAcCfgFullName(k) then return self:GetCfgNameFromFullName(k) end
        end		
	end
end

function consolecfg:IteratorSharedCfgName()
    local allFiles = self:GetAllFiles():GetCfgTbl()
    assert(allFiles)
    local impl, t, k = pairs(allFiles)
	return function()
		while true do
            k = impl(t, k)
            if k == nil then break end
            if self:IsSharedCfgName(k) then return self:GetCfgNameFromFullName(k) end
        end
	end
end

function consolecfg:NewCfgFactory_()
    return ConsoleCfgFactory
end