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
    GetAcListColumnIdxInfo_ = nil,
    GetAcListColumnNameInfo_ = nil,
    NewAcList_ = nil,
}
setmetatable(MyFrmFactory, MyFrmFactory)
local frmFactoryBase = frmfactory
local frmfactory = frmfactory

MyAcList = {
    __index = AcList,

    UpdateItem = nil,
}
setmetatable(MyAcList, MyAcList)

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

function MyFrmFactory:GetAcListColumnIdxInfo_()
    local res = frmFactoryBase.GetAcListColumnIdxInfo_(self)
    table.insert(res, "roleName")
    table.insert(res, "roleLvl")
    table.insert(res, "mapName")
    return res
end

function MyFrmFactory:GetAcListColumnNameInfo_()
    local res = frmFactoryBase.GetAcListColumnNameInfo_(self)
    table.insert(res, "角色")
    table.insert(res, "等级")
    table.insert(res, "地图名")
    return res
end

function MyFrmFactory:NewAcList_()
	return MyAcList
end

function MyAcList:UpdateItem(item)
    local itemData = AcList.UpdateItem(self, item)
    assert(itemData, "itemData")
    local tmp = itemData.roleName
    if tmp ~= nil then self:SetItem(item, self.listColumnIdx.roleName, tmp) end
    tmp = itemData.roleLvl
    if tmp ~= nil then self:SetItem(item, self.listColumnIdx.roleLvl, tmp) end
    tmp = itemData.mapName
    if tmp ~= nil then self:SetItem(item, self.listColumnIdx.mapName, tmp) end
    return itemData
end