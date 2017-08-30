require("mainfrm")
require("MyPages")
MyFrm = {
    __index = mainfrm,
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
    __index = frmfactory,
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
        ["体验服"] = 257,
    },
})
function MyFrmFactory:GetAreaSerInfo_()
    return {
        [""] = {
            "体验服",
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
