require("frmfactory")
require("FrmStatusBar_p")
require("FrmMenuBar_p")
require("BasicSetting_p")

frmfactory_p = {
    __index = frmfactory,

    NewFrmStatusBar_ = nil,
    NewMenuBar_ = nil,
    NewBasicSetting = nil,
}
setmetatable(frmfactory_p, frmfactory_p)

function frmfactory_p:NewFrmStatusBar_()
    return FrmStatusBar_p
end

function frmfactory_p:NewMenuBar_()
    return FrmMenuBar_p
end

function frmfactory_p:NewBasicSetting()
    return BasicSetting_p
end
