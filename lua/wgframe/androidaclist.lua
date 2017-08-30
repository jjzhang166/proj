require("AcList")
androidaclist = {
    __index = AcList,
    -----------------------
    DoUpdateItem = nil,
}
setmetatable(androidaclist, androidaclist)
local AcList = AcList

function androidaclist:DoUpdateItem(item, itemData)
    assert(itemData.vmName)
    self:SetItem(item, self.listColumnIdx.vmName, itemData.vmName)
    return AcList.DoUpdateItem(self, item, itemData)
end
