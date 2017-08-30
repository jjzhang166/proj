require("BasicSetting")
BasicSetting_p = {
    __index = BasicSetting,
    DoUpdateAutoRunText_ = nil,
}
setmetatable(BasicSetting_p, BasicSetting_p)

function BasicSetting_p:DoUpdateAutoRunText_()
end
