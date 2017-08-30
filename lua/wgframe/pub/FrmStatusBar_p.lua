require("FrmStatusBar")
FrmStatusBar_p = {
    __index = FrmStatusBar,

	Create = nil,
    SetCurSelectedCfg = nil,
	SetEndDateTime = nil,
	SetVersion = nil,
}
setmetatable(FrmStatusBar_p, FrmStatusBar_p)

function FrmStatusBar_p:Create(statusBar)
	assert(statusBar)
	return CmnBase.ExtCObj(self, statusBar)
end

function FrmStatusBar_p:SetCurSelectedCfg(cfgName)
end

function FrmStatusBar_p:SetEndDateTime(time)
end

function FrmStatusBar_p:SetVersion(ver)
end