require("cfgfactory")
ConsoleCfgFactory_p = {
    __index = ConsoleCfgFactory,

    NewDftAcGlobalTbl_ = nil,
}
setmetatable(ConsoleCfgFactory_p, ConsoleCfgFactory_p)

function ConsoleCfgFactory_p:NewDftAcGlobalTbl_()
	local res = ConsoleCfgFactory:NewDftAcGlobalTbl_()
    
    return res
end
