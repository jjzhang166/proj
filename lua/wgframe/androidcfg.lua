require("consolecfg")
androidcfg = {
    __index = consolecfg,
    ------------------------
    AndroidAcListItemOper = {
        __index = consolecfg.AcListItemOper,
        ---------------------
        vmName = "",
    },
    -----------------
    LoadMainCfg = nil,
    NewCfgFactory_ = nil,
}
setmetatable(androidcfg, androidcfg)
--consolecfg = androidcfg
local consolecfgOld = consolecfg
local AndroidAcListItemOper = androidcfg.AndroidAcListItemOper
setmetatable(AndroidAcListItemOper, AndroidAcListItemOper)

androidcfgFactory = {
    __index = ConsoleCfgFactory,
    -----------------
   NewAcListItemOper_ = nil,
}
setmetatable(androidcfgFactory, androidcfgFactory)
local ConsoleCfgFactory = ConsoleCfgFactory

function androidcfgFactory:NewAcListItemOper_(cfgMgr)
	return cfgMgr.AndroidAcListItemOper
end

function androidcfg:LoadMainCfg(...)
    local res = { consolecfgOld.LoadMainCfg(self, ...) }
    local gamePath = self.mainCfg.gamePath
    if gamePath == nil or #gamePath < 1 then
        self.mainCfg.gamePath = GetMyApp():DetectGamePath()
    end
    return unpack(res)
end

function androidcfg:NewCfgFactory_()
    return androidcfgFactory
end
