require("FrmMenuBar")
FrmMenuBar_p = {
    __index = FrmMenuBar,

	CreateMenuBar = nil,
}
setmetatable(FrmMenuBar_p, FrmMenuBar_p)

--[[
function FrmMenuBar_p:CreateMenuBar(parent)
end
--]]