require("CmnBase")
require("cfgfactory")
cmncfg = {
	mainCfg = nil,
	mainCfgName = nil,
	autoSave = true,
    myApp = GetMyApp(),
	resolver = GetMyApp():GetLuaCfgResolver(),
    factory = nil,
	-------------------------------
	CfgWrapperTbl = {
		GetCfgTbl = nil,	--真正的配置表
		GetSmartCfg = nil,
		DoIndex = nil,
		DoNewIndex = nil,
	},
	-------------------------------
	SmartCfg = {
		GetSinks = nil,
		GetSinkItems = nil,
		AddSink = nil,
		RemoveSink = nil,
		NotifySink = nil,
		GetCfgObj = nil,
		SetCfgObj = nil,
		GetCfgTbl = nil,	--真正的配置表
		FindWrapperTbl = nil,
		NewSmartCfg = nil,      --不要重写此函数
		GetCfgMgr = nil,
	},
	-------------------------------
	MakeCfgInfo = nil,
	LoadMainCfg = nil,
	LoadCfg = nil,
	LoadSomeCfg = nil,
	GetAllFiles = nil,			--除了主配置文件
	LoadAllFiles = nil,
    FindDependeceCfgName = nil,
	ResolveCfg = nil,
	DeleteCfg = nil,
	Clear = nil,
	FindCfg = nil,
	Iterator = nil,
	SaveAllCfg = nil,
	PersistAutoSave = nil,
	UnpersistAutoSave = nil,
	GetGlobalSinks = nil,
	AddGlobalSink = nil,
	RemoveGlobalSink = nil,
    NewCfgFactory_ = nil,
}
assert(cmncfg.resolver)
local CfgWrapperTbl = cmncfg.CfgWrapperTbl
local SmartCfg = cmncfg.SmartCfg

function CfgWrapperTbl:GetCfgTbl()
	return self.__RealSubCfgTbl__
end

function CfgWrapperTbl:GetSmartCfg()
	return self.__SmartCfgOfWrapper__
end

function CfgWrapperTbl:DoIndex(k)
end

function CfgWrapperTbl:DoNewIndex(k, v)
end

function SmartCfg:GetSinks(isNew)
	local sinks = rawget(self, "__sinks__")
	if sinks == nil and isNew == true then
		sinks = {}
		rawset(self, "__sinks__", sinks)
	end
	return sinks
end

function SmartCfg:GetSinkItems(strCfgDataItem, isNew)
	local sinks = self:GetSinks(isNew)
	if sinks == nil then return end
	local items = rawget(sinks, strCfgDataItem)
	if items ~= nil or isNew ~= true then return items end
	items = {}
	rawset(sinks, strCfgDataItem, items)
	return items
end

function SmartCfg:AddSink(strCfgDataItem, sink)
	assert(strCfgDataItem)
	assert(sink)
	table.insert(self:GetSinkItems(strCfgDataItem, true), sink)
end

function SmartCfg:RemoveSink(strCfgDataItem, sink)
	local sinks = self:GetSinkItems(strCfgDataItem)
	if sinks == nil then return end
	if sink then
		for k, v in pairs(sinks) do
			if v == sink then
				table.remove(sinks, k)
				break
			end
		end
	else
		rawset(self:GetSinks(), strCfgDataItem, nil)
	end
end

function SmartCfg:NotifySink(strCfgDataItem, old, new)
	LogT("cfg data changed, key:", strCfgDataItem, "\told:", old, " \tnew:", new)
	local sinks = self:GetSinkItems(strCfgDataItem)
	if sinks == nil then return end
	for _, v in ipairs(sinks) do
		v(self, old, new)
	end	
end

function SmartCfg:GetCfgObj()
	return rawget(self, "__cfgobj__")
end

function SmartCfg:SetCfgObj(cfgObj)
	assert(cfgObj)
	return rawset(self, "__cfgobj__", cfgObj)
end

function SmartCfg:GetCfgTbl()
	local cfgObj = self:GetCfgObj()
	if cfgObj == nil then return end
	return cfgObj:GetTable()
end

local function GetSmartCfgWrapperTbl(smart)
	local tbl = rawget(smart, "__wrapperTbls__")
	if tbl == nil then
		tbl = {}
		rawset(smart, "__wrapperTbls__", tbl)
	end
	return tbl
end

function SmartCfg:FindWrapperTbl(cfgTbl)
	return GetSmartCfgWrapperTbl(self)[cfgTbl]
end

local function MakeSmartCfgWrapperTblItem(smart, cfgTbl, parentName, name)
	local wrapper = smart:GetCfgMgr().factory:NewCfgWrapper_(cfgTbl, parentName, name)	
	if parentName ~= nil then
		name = parentName .. '.' .. name
	end
	local wrapperImpl = {
		__RealSubCfgTbl__ = cfgTbl,
		__SmartCfgOfWrapper__ = smart,
	}
	if wrapper then
		CmnBase.SelfMetaForwardSet(wrapper, "__index", CfgWrapperTbl)
		wrapperImpl.__index = function(t, k)
			local res = wrapper[k]
			if res ~= nil then return res end
			res = cfgTbl[k]
			if res == nil then return t:DoIndex(k) end
			if type(res) == "table" then
				local tbl = smart:FindWrapperTbl(res)
				if tbl ~= nil then return tbl end
				return MakeSmartCfgWrapperTblItem(smart, res, name, k)
			end
			return res
		end
		wrapperImpl.__newindex = function(t, k, v)
			local old = wrapper[k]
			if old ~= nil then wrapper[k] = v; return end
			old = cfgTbl[k]
			if old == v then
				if v == nil then t:DoNewIndex(k, v) end
				return
			end
			if old == nil and t:DoNewIndex(k, v) ~= nil then return end
			if type(old) == "table" then
				GetSmartCfgWrapperTbl(smart)[old] = nil
			end
			cfgTbl[k] = v
			local cfgMgr = smart:GetCfgMgr()
			if cfgMgr.autoSave == true and smart:Save() ~= true then
				assert(false)
			end
			local key = name .. '.' .. k
			smart:NotifySink(key, old, v)
			local gloSink = cfgMgr:GetGlobalSinks()
			if gloSink then
				gloSink(smart, key, old, v)
			end
		end
	else
		wrapperImpl.__index = function(t, k)
			local res = CfgWrapperTbl[k]
			if res ~= nil then return res end
			res = cfgTbl[k]
			if res == nil then return res end
			if type(res) == "table" then
				local tbl = smart:FindWrapperTbl(res)
				if tbl ~= nil then return tbl end
				return MakeSmartCfgWrapperTblItem(smart, res, name, k)
			end
			return res
		end
		wrapperImpl.__newindex = function(t, k, v)
			local old = CfgWrapperTbl[k]
			if old ~= nil then t[k] = v; return end
			old = cfgTbl[k]
			if old == v then return end
			if type(old) == "table" then
				GetSmartCfgWrapperTbl(smart)[old] = nil
			end
			cfgTbl[k] = v
			local cfgMgr = smart:GetCfgMgr()
			if cfgMgr.autoSave == true and smart:Save() ~= true then
				assert(false)
			end
			local key = name .. '.' .. k
			smart:NotifySink(key, old, v)
			local gloSink = cfgMgr:GetGlobalSinks()
			if gloSink then
				gloSink(smart, key, old, v)
			end
		end
	end
	GetSmartCfgWrapperTbl(smart)[cfgTbl] = wrapperImpl
	setmetatable(wrapperImpl, wrapperImpl)
	return wrapperImpl
end

function SmartCfg:NewSmartCfg(cfgObj, cfgOper)
	assert(cfgObj)
	cfgOper = cfgOper or {}
	assert(cfgOper.__index == nil)
	assert(cfgOper.__newindex == nil)
	assert(getmetatable(cfgOper) == nil)
	cfgOper.__index = function(t, k)
		local res = self[k]
		if res ~= nil then return res end
		local theCfgObj = self.GetCfgObj(t)
		if theCfgObj == nil then return end
		res = theCfgObj[k]
		if res ~= nil then
			if type(res) == "function" then
				return CmnBase.ChooseCall(res, t, theCfgObj)
			end
			return res
		end
		local cfgTbl = theCfgObj:GetTable()
		if cfgTbl == nil then return end
		res = t:FindWrapperTbl(cfgTbl)
		if res ~= nil then return res[k] end
		return MakeSmartCfgWrapperTblItem(t, cfgTbl, nil, theCfgObj:GetFileName())[k]
	end
	cfgOper.__newindex = function(t, k, v)
		local res = self[k]
		if res ~= nil then self[k] = v; return end
		local theCfgObj = self.GetCfgObj(t)
		if theCfgObj ~= nil then
			res = theCfgObj[k]
			if res ~= nil then theCfgObj[k] = v; return end
			local cfgTbl = theCfgObj:GetTable()
			if cfgTbl ~= nil then
				res = t:FindWrapperTbl(cfgTbl)
				if res ~= nil then res[k] = v; return end
				MakeSmartCfgWrapperTblItem(t, cfgTbl, nil, theCfgObj:GetFileName())[k] = v
				return
			end
		end
		t[k] = v
	end
	setmetatable(cfgOper, cfgOper)
	cfgOper:SetCfgObj(cfgObj)
	return cfgOper
end

function SmartCfg:GetCfgMgr()
	return self.__CfgMgr__
end

function cmncfg.MakeCfgInfo(cfgName, mapLvl, dependece, cfgOper)
	assert(cfgName)
	mapLvl = mapLvl or 0
	return { ["cfgName"] = cfgName, ["mapLvl"] = mapLvl, ["dependece"] = dependece, ["cfgOper"] = cfgOper }
end

local function _GetCfgObjHolder_(cmncfg_)
	local cfgs = rawget(cmncfg_, "__cfgs__")
	if cfgs ~= nil then return cfgs end
	cfgs = {}
	rawset(cmncfg_, "__cfgs__", cfgs)
	return cfgs
end

local function _ClearCfgObjHolder_(cmncfg_)
	if rawget(cmncfg_, "__cfgs__") == nil then return end
	rawset(cmncfg_, "__cfgs__", nil)
end

local function NewSmartCfgImpl_(cfgMgr, newImpl, ...)
	local cfg = newImpl(cfgMgr.factory, ...)
	rawset(cfg, "__CfgMgr__", cfgMgr)
	CmnBase.SelfMetaForwardSet(cfg, "__index", cmncfg.SmartCfg)
	return cfg
end

function cmncfg:LoadMainCfg(cfgInfo, ...)
    assert(self.factory == nil)
    self.factory = self:NewCfgFactory_()
    assert(self.factory)
    self.factory.cfg = self
	assert(cfgInfo ~= nil)
	assert(cfgInfo.dependece == nil)
	local dftCfg = self.factory:NewDftMainCfgTbl_(cfgInfo, ...)
	assert(dftCfg)
	assert(dftCfg.version ~= nil)
	local version = dftCfg.version
	self:Clear()
	local cfgObj = self.resolver:AddFile(cfgInfo.cfgName, cfgInfo.mapLvl)
	assert(cfgObj)
	cfgObj:SetTable(dftCfg)
	self:ResolveCfg("")
	local resTbl = cfgObj:GetTable()
	if version > resTbl.version then
		self.resolver:DeleteAllDiskFile(true)
		dftCfg = self.factory:NewDftMainCfgTbl_(cfgInfo, ...)
		cfgObj:SetTable(dftCfg)
		self:ResolveCfg("")
		resTbl = cfgObj:GetTable()
		assert(version == resTbl.version)
	end
	self.mainCfgName = cfgInfo.cfgName
	self.mainCfg = NewSmartCfgImpl_(self, self.factory.NewMainSmartCfg_, cfgInfo, ...):NewSmartCfg(cfgObj, cfgInfo.cfgOper)
	_GetCfgObjHolder_(self)[cfgInfo.cfgName] = self.mainCfg
    self.myApp:ParseMainCfg(self.mainCfg)
	return self.mainCfg
end

function cmncfg:LoadCfg(cfgInfo, isLoad, ...)
    if self.resolver:FindFile(cfgInfo.cfgName) then return end
	if isLoad == nil then isLoad = true end
	assert(cfgInfo ~= nil)
	local dftCfg = self.factory:NewDftCfgTbl_(cfgInfo, ...)
	assert(dftCfg)
	local cfgObj = self.resolver:AddFile(cfgInfo.cfgName, cfgInfo.mapLvl)
	assert(cfgObj)
	if cfgInfo.dependece ~= nil and #cfgInfo.dependece > 0 then cfgObj:AddDependence(cfgInfo.dependece) end
	cfgObj:SetTable(dftCfg)
	if isLoad then
		if cfgObj:LoadCfg() ~= true then
			assert(false)
			return
		end
	end
	local smart = NewSmartCfgImpl_(self, self.factory.NewSmartCfg_, cfgInfo, ...):NewSmartCfg(cfgObj, cfgInfo.cfgOper)
    assert(smart)
	_GetCfgObjHolder_(self)[cfgInfo.cfgName] = smart
	
	cfgInfo.dependece = cfgInfo.dependece or ""
	local fileInfo = self:GetAllFiles()[cfgInfo.cfgName]
	if fileInfo == nil or fileInfo[1] ~= cfgInfo.mapLvl or fileInfo[2] ~= cfgInfo.dependece then
		self:GetAllFiles()[cfgInfo.cfgName] = { cfgInfo.mapLvl, cfgInfo.dependece or "" }
	end
	return smart
end

function cmncfg:LoadSomeCfg(cfgInfos, isSolve, ...)
	assert(cfgInfos)
	if isSolve == nil then isSolve = true end
	local cnt = 0
	for _, v in ipairs(cfgInfos) do
		self:LoadCfg(v, false, ...)
		cnt = cnt + 1
	end
	if cnt > 0 and isSolve then
		self:ResolveCfg(self.mainCfgName)
	end
	return cnt
end

function cmncfg:GetAllFiles()
	local tbl = self.mainCfg.allFiles
	if tbl ~= nil then return tbl end
	self.mainCfg.allFiles = {}
	return self.mainCfg.allFiles
end

function cmncfg:LoadAllFiles(fn)
	local cfgTbl = self:GetAllFiles():GetCfgTbl()
	local tblTmp = {}
	if fn == nil then
		for k, v in pairs(cfgTbl) do
			table.insert(tblTmp, self.MakeCfgInfo(k, v[1], v[2]))
		end
	else
		for k, v in pairs(cfgTbl) do
			local mapLvl = v[1]
			local dependece = v[2]
			if fn(k, mapLvl, dependece) == true then
				table.insert(tblTmp, self.MakeCfgInfo(k, mapLvl, dependece))
			end
		end
	end
	return self:LoadSomeCfg(tblTmp, true)
end

function cmncfg:FindDependeceCfgName(cfgName)
    local cfgInfo = self:GetAllFiles()[cfgName]
    if cfgInfo == nil then return end
    return cfgInfo[2]
end

function cmncfg:ResolveCfg(butCfg)
	butCfg = butCfg or self.mainCfgName
	self.resolver:Resolve(butCfg)
end

function cmncfg:DeleteCfg(cfgName)
	assert(cfgName)
	local cfg = self.resolver:FindFile(cfgName)
	if not cfg then
--		assert(false)
		return
	end
	self:GetAllFiles()[cfgName] = nil
	_GetCfgObjHolder_(self)[cfgName] = nil
	cfg:DeleteDiskFile()
	self.resolver:RemoveFile(cfg)
	cfg = nil
end

function cmncfg:Clear()
	if self.mainCfg then
		self.mainCfg.allFiles = nil
	end
	_ClearCfgObjHolder_(self)
	self.resolver:Clear()
	self.mainCfgName = nil
	self.mainCfg = nil
end

function cmncfg:FindCfg(cfgName)
	assert(cfgName)
--    print("FindCfg:", cfgName)
	return _GetCfgObjHolder_(self)[cfgName]
end

function cmncfg:Iterator()
	return pairs(_GetCfgObjHolder_(self))
end

function cmncfg:SaveAllCfg()
	self.resolver:SaveAll()
end

function cmncfg:PersistAutoSave(isAutoSave)
	if isAutoSave == self.autoSave then return end
	assert(isAutoSave ~= nil)
	self.autoSave = isAutoSave
	return not isAutoSave
end

function cmncfg:UnpersistAutoSave(isAutoSave)
	if isAutoSave == nil then return end
	assert(self.autoSave ~= isAutoSave)
	self.autoSave = isAutoSave
end

function cmncfg:GetGlobalSinks(isNew)
	local sinks = rawget(self, "__gsinks__")
	if sinks ~= nil or isNew ~= true then return sinks end
	sinks = {}
	local mt = {
		__call = function(t, smart, key, ...)
			for k, v in pairs(sinks) do
				if k == smart then
					v(smart, key, ...)
				elseif type(k) == "string" and string.find(key, k) then
					v(smart, key, ...)
				end
			end
		end
	}
	setmetatable(sinks, mt)
	rawset(self, "__gsinks__", sinks)
	return sinks
end

function cmncfg:AddGlobalSink(key, sink)
	assert(key)
	assert(sink)
	self:GetGlobalSinks(true)[key] = sink
end

function cmncfg:RemoveGlobalSink(key)
	assert(key)
	local sinks = self:GetGlobalSinks()
	if sinks == nil then return end
	sinks[key] = nil
end

function cmncfg:NewCfgFactory_()
    assert(cfgfactory)
    return cfgfactory
end
