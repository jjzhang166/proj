require("androidcfg")
MyCfg = {
    __index = androidcfg
}
setmetatable(MyCfg, MyCfg)
consolecfg = MyCfg