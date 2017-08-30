require("mainfrm_p")
require("MyPages")
MyFrm = {
    __index = mainfrm_p,
    -------------------------
    ---------test
    OnTest = nil,
    OnCloseLoginSession = nil,
    -------------------------
    NewFrmFactory_ = nil,
}
setmetatable(MyFrm, MyFrm)
mainfrm = MyFrm

MyFrmFactory = {
    __index = frmfactory_p,
    ----------------
    GetMenuBarMenuInfo_ = nil,
    GetFrmHeaderCtrlColumnInfo_ = nil,
	NewFrmHeaderCtrlPages_ = nil,
    GetAreaSerInfo_ = nil,
    NewBasicSetting = nil,
}
setmetatable(MyFrmFactory, MyFrmFactory)
local frmfactory = frmfactory

GetMyApp():ParseAreaSer2Id({
    [""] = {
        ["Auto-select Gateway"] = 1,
        ["Texas (US)"] = 2,
        ["Washington, D.C. (US)"] = 3,
        ["California (US)"] = 4,
        ["Amsterdam (EU)"] = 5,
        ["London (EU)"] = 6,
        ["Frankfurt (EU)"] = 7,
        ["Milan (EU)"] = 8,
        ["Singapore"] = 9,
        ["Australia"] = 10,
        ["Sao Paulo (BR)"] = 11,
        ["Paris (EU)"] = 12,
        ["Miami (US)"] = 13,
        ["Moscow (RU)"] = 14,
        ["Auckland (NZ)"] = 15,
    },
})

function MyFrmFactory:GetAreaSerInfo_()
    return {
        [""] = {
            "Auto-select Gateway",
            "Texas (US)",
            "Washington, D.C. (US)",
            "California (US)",
            "Amsterdam (EU)",
            "London (EU)",
            "Frankfurt (EU)",
            "Milan (EU)",
            "Singapore",
            "Australia",
            "Sao Paulo (BR)",
            "Paris (EU)",
            "Miami (US)",
            "Moscow (RU)",
            "Auckland (NZ)",
        },
    }
end

function MyFrm:OnTest()
--    --[[
	local app = GetMyApp()
    app:Test("")
    --]]
end

function MyFrm:OnCloseLoginSession()
    local acList = self:GetAcListCfg()
    local stateLogining = acList.stateLogining
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData:GetState() == stateLogining then
            self.uiOperHandler:CloseLoginSession(itemData:GetItemKey())
        end
    end
end

function MyFrm:NewFrmFactory_()
	return MyFrmFactory
end

function MyFrmFactory:GetMenuBarMenuInfo_()
    return CmnBase.Super(self).GetMenuBarMenuInfo_(self)
end

function MyFrmFactory:GetFrmHeaderCtrlColumnInfo_()
	return frmfactory:GetFrmHeaderCtrlColumnInfo_()
end

function MyFrmFactory:NewFrmHeaderCtrlPages_(parent)
	local basicSetting = self:NewBasicSetting()
	return {
		basicSetting:Create(parent),		
	}
end

function MyFrmFactory:NewBasicSetting()
    return MyPages.MyBasicSetting
end
