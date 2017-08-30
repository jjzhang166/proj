require("CmnBase")
FrmStatusBar = {
	Create = nil,
    SetCurSelectedCfg = nil,
	SetEndDateTime = nil,
	SetVersion = nil,
}

function FrmStatusBar:Create(statusBar)
	assert(statusBar)
	local version = 13
    local selectedCfg = version + 5
	local endDateTime = version + 5
	local left = 100 - version - endDateTime - selectedCfg
	statusBar:SetFieldsCount({-left, -selectedCfg, -endDateTime, -version})
	return CmnBase.ExtCObj(self, statusBar)
end

function FrmStatusBar:SetCurSelectedCfg(cfgName)
    self:SetStatusText(cfgName, 1)
end

function FrmStatusBar:SetEndDateTime(time)
	self:SetStatusText("到期时间：" .. time, 2)
end

function FrmStatusBar:SetVersion(ver)
	self:SetStatusText("版本：" .. ver, 3)
end