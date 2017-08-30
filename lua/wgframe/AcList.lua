require("CmnBase")
require("WxCmn")
AcList = {
	listCtrl = nil,
    listColumnIdx = nil,
    popuMenu_ = nil,
    mainFrm = nil,
    itemDatas = {},
    lastSelectedItem = -1,
    -------------------------------------

	Create = nil,
    AddItem = nil,
    DeleteItem = nil,
    UpdateItemLastTime = nil,
    UpdateItem = nil,
    DoUpdateItem = nil,
    FindItemByAc = nil,
    FindItemByData = nil,
    GetItemData = nil,
    OutputScriptInfo = nil,
    SelectItem = nil,
    Iterator = nil,
    IteratorData = nil,
    IteratorSelected = nil,
    IteratorP_Selected = nil,
    ShowPopMenu = nil,
    GetLastSelectedItem = nil,
    --------------------------------------
    OnSelectedItemChange = nil,
}

function AcList:Create(parent, mainFrm)
    assert(mainFrm)
    self.mainFrm = mainFrm
	self.listCtrl = wx.wxListCtrl(parent, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxLC_REPORT)
    self.listCtrl:EnableCheckboxes(true)
	
    local factory = mainfrm.factory
    local columnNameInfo
    self.listColumnIdx, columnNameInfo = factory:GetAcListColumnInfo()
    for k, v in pairs(columnNameInfo) do
        self.listCtrl:InsertColumn(k, v, wx.wxLIST_FORMAT_LEFT, wx.wxLIST_AUTOSIZE)
    end
    local columnLastTime = self.listColumnIdx.lastTime
    if columnLastTime then
        local lastTimeWidth = self.listCtrl:GetColumnWidth(columnLastTime)
        self.listCtrl:SetColumnWidth(columnLastTime, lastTimeWidth * 1.25)
    end
	
    local res = CmnBase.ExtCObj(self, self.listCtrl)

    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, function(event)
		event:Skip(true)
        res:ShowPopMenu(event:GetIndex(), event:GetPoint())
	end)
    local isPushed = false
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_MENU_HIGHLIGHT, function(event)
        if isPushed then
            isPushed = false
            res.mainFrm.statusBar:PopStatusText()
        end
        local menuId = event:GetMenuId()
        assert(menuId)
        local menuItem = res.popuMenu_:FindItem(menuId)
        if menuItem == nil then return end
        isPushed = true
        local str = menuItem:GetHelp()
--        print(str)
        res.mainFrm.statusBar:PushStatusText(str)
	end)
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_MENU_CLOSE, function(event)
		event:Skip(true)
--        print("isPushed:", isPushed)
        if isPushed == false then return end
        isPushed = false
        res.mainFrm.statusBar:PopStatusText()
	end)
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_LIST_ITEM_CHECKED, function(event)
		event:Skip(true)
        local itemData = res:GetItemData(event:GetIndex())
        itemData.checked = true
	end)
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_LIST_ITEM_UNCHECKED, function(event)
		event:Skip(true)
        local itemData = res:GetItemData(event:GetIndex())
        itemData.checked = false
	end)
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_LIST_ITEM_SELECTED, function(event)
		event:Skip(true)
        res:OnSelectedItemChange()
	end)
    self.listCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_LIST_ITEM_DESELECTED, function(event)
		event:Skip(true)
        res:OnSelectedItemChange()
	end)

	return res
end

function AcList:AddItem(itemData)
    local item = self:GetItemCount()
    self:InsertItem(item, itemData.ac)
    self:SetItem(item, self.listColumnIdx.pwd, itemData.pwd)
    self:SetItem(item, self.listColumnIdx.ser, itemData.ser)
    local area = itemData.area
    if area ~= nil and #area > 0 then
        self:SetItem(item, self.listColumnIdx.area, area)
    else
        area = ""
    end
    self.itemDatas[item + 1] = itemData
    self:UpdateItem(item)
    GetMyApp():OnLuaAddAc(itemData.ac, itemData.pwd, area, itemData.ser)
end

function AcList:DeleteItem(item)
    table.remove(self.itemDatas, item + 1)
    self.listCtrl:DeleteItem(item)
end

function AcList:UpdateItemLastTime(item, itemData)
    local lastTime = os.date("%m-%d %H:%M:%S")
    itemData.lastTime = lastTime
    self:SetItem(item, self.listColumnIdx.lastTime, lastTime)
end

function AcList:UpdateItem(item)
    local itemData = self.itemDatas[item + 1]
    if itemData == nil then
        print(item)
        assert(false, "AcList:UpdateItem Error")
        return
    end
    self:SetItem(item, self.listColumnIdx.cfg, itemData:GetExcludeSelfCfgName())
    self:CheckItem(item, itemData.checked)
    self:SetItem(item, self.listColumnIdx.state, itemData:GetStateName())
    self:SetItem(item, self.listColumnIdx.cate, itemData:GetAcCateName())
    self:SetItem(item, self.listColumnIdx.lastTime, itemData.lastTime)
    self:SetItem(item, self.listColumnIdx.script, itemData.script)
    self:DoUpdateItem(item, itemData)
    return itemData
end

function AcList:DoUpdateItem(item, itemData)
    self.mainFrm:UpdateAcListItem(item, itemData)
end

function AcList:FindItemByAc(acOrKey, serOrArea, ser)
    local itemData = self.mainFrm:GetAcListCfg():FindItem(acOrKey, serOrArea, ser)
    if itemData == nil then return end
    return self:FindItemByData(itemData)
end

function AcList:FindItemByData(itemData)
    assert(itemData)
    for k, v in ipairs(self.itemDatas) do
        if v == itemData then return k - 1 end
    end
end

function AcList:GetItemData(item)
    return self.itemDatas[item + 1]
end

function AcList:OutputScriptInfo(item, info)
    self:SetItem(item, self.listColumnIdx.scriptInfo, info)
end

function AcList:SelectItem(item, isSelect)
    local cnt = self:GetItemCount()
    if item >= 0 then
        if item >= cnt then assert(false); return end
        local mask = wx.wxLIST_STATE_SELECTED
        local oldIsSelect = self:GetItemState(item, mask) == mask
        if isSelect == oldIsSelect then return end
        if isSelect then self:SetItemState(item, mask, mask); return end
        self:SetItemState(item, 0, mask)
    else
        if cnt < 1 then return end
        local mask = wx.wxLIST_STATE_SELECTED
        for item = 0, cnt - 1 do
            local oldIsSelect = self:GetItemState(item, mask) == mask
            if isSelect ~= oldIsSelect then 
                if isSelect then 
                    self:SetItemState(item, mask, mask)
                else
                    self:SetItemState(item, 0, mask)
                end
            end
        end
    end
end

function AcList:Iterator()
    local idx = 0
    local cnt = self:GetItemCount()
    return function ()
        if idx >= cnt then return end
        local old = idx
        idx = idx + 1
        return old
    end
end

function AcList:IteratorData()
    local impl, t, k = ipairs(self.itemDatas)
	return function()
        local v
		k, v = impl(t, k)
		if k ~= nil then return k - 1, v end
	end
end

function AcList:IteratorSelected()
    local idx = 0
    local cnt = self:GetItemCount()
    local mask = wx.wxLIST_STATE_SELECTED
    return function ()
        while idx < cnt do
            if mask == self:GetItemState(idx, mask) then
                local old = idx
                idx = idx + 1
                return old
            end
            idx = idx + 1
        end
    end
end

function AcList:IteratorP_Selected(isAll)
    if isAll == nil or isAll == true then return self:Iterator() end
    return self:IteratorSelected()
end

function AcList:ShowPopMenu(item, pos)
    if self.popuMenu_ == nil then
        local menuInfo = self.mainFrm.factory:GetAcListMenuInfo_()
        self.popuMenu_ = wx.wxMenu(menuInfo)
        for _, v in ipairs(menuInfo) do
            local itemId = v[1]
            if itemId ~= nil then
                self:Connect(itemId, wx.wxEVT_COMMAND_MENU_SELECTED, v[4])
            end
        end
    end
    self.lastSelectedItem = item
    self:PopupMenu(self.popuMenu_, pos)
end

function AcList:GetLastSelectedItem()
    return self.lastSelectedItem
end

function AcList:OnSelectedItemChange()
    local item
    local frm = mainfrm
    for i in self:IteratorSelected() do
        if item ~= nil then
            if frm.curSelectedItem == nil then return end
            frm.curSelectedItem = nil
            frm:UpdateSelectedCfgInfo()
            return
        end
        item = i
    end
    if frm.curSelectedItem == item then return end
    frm.curSelectedItem = item
    frm:UpdateSelectedCfgInfo()
end