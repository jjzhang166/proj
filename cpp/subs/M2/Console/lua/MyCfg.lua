require("consolecfg")
MyCfg = {
    __index = consolecfg,
    ------------------
    NewCfgFactory_ = nil,
}
setmetatable(MyCfg, MyCfg)
consolecfg = MyCfg

MyCfgFactory = {
    __index = ConsoleCfgFactory,
    ------------------
    NewDftAcGlobalTbl_ = nil,
}
local ConsoleCfgFactory = ConsoleCfgFactory
setmetatable(MyCfgFactory, MyCfgFactory)

function MyCfgFactory:NewDftAcGlobalTbl_()
	local res = ConsoleCfgFactory:NewDftAcGlobalTbl_()
    res.script = "run"
    return res
end

function MyCfg:NewCfgFactory_()
    return MyCfgFactory
end