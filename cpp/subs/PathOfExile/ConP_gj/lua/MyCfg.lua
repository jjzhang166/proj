require("consolecfg")
require("cfgfactory_p")
MyCfg = {
    __index = consolecfg,
    ------------------
    NewCfgFactory_ = nil,
}
setmetatable(MyCfg, MyCfg)
consolecfg = MyCfg

MyCfgFactory = {
    __index = ConsoleCfgFactory_p,
    ------------------
    NewDftAcGlobalTbl_ = nil,
}
local ConsoleCfgFactory = ConsoleCfgFactory
setmetatable(MyCfgFactory, MyCfgFactory)

function MyCfgFactory:NewDftAcGlobalTbl_()
	local res = ConsoleCfgFactory_p:NewDftAcGlobalTbl_()
    res.script = "run"
    res.dstPosX = 0
    res.dstPosY = 0
    return res
end

function MyCfg:NewCfgFactory_()
    return MyCfgFactory
end