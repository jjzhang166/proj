require("cmdid")

local treeExt_ = {}
local listExt_ = {}
local menuBarExt_ = {}
local toolBarExt_ = {}


function AutoEnableMenu()
    for _, v in ipairs(Id) do
        if v.id ~= nil then
            if v.enable == nil then
                menuBar:Enable(v.id, true)
                toolBar:EnableTool(v.id, true)
            else
                menuBar:Enable(v.id, v.enable)
                toolBar:EnableTool(v.id, v.enable)
            end
        end
    end
end

local function CreateMenu(ids, byEnable)
    byEnable = byEnable or false
    local param = {}
    local isCreate = true
	for i, v in ipairs(ids) do
        if byEnable then
            if v.id == nil then
                isCreate = true
            elseif v.enable == nil then
                isCreate = true
            else
                isCreate = v.enable
            end
        end
        if isCreate then
            if v.id == nil then
                table.insert(param, v)
            else
                local p = { v.id }
                local str = "&" .. v.name
                if v.shortcut ~= nil then
                    str = str .. "\t" .. v.shortcut
                end
                p[2] = str
                p[3] = v.prompt
                table.insert(param, p)
            end
        end
	end
	return wx.wxMenu(param)
end

local function CreateMenuBarItem(menu)
	menuBar:Append(CreateMenu(Id[menu]), "&" .. menu)
end

local function CreateTool(menu)
    local toolBmpSize = toolBar:GetToolBitmapSize()
    for _, v in ipairs(menu) do
        if v.id == nil then
            toolBar:AddSeparator()
        else
            if v.artId ~= nil then
                toolBar:AddTool(v.id, v.name, wx.wxArtProvider.GetBitmap(v.artId, wx.wxART_MENU, toolBmpSize), v.prompt)
            end
        end
    end
end

function CreateMenuBar()
    local menuBar_ = menuBar
    menuBar = CmnBase.ExtCObj(menuBarExt_, menuBar_)
	CreateMenuBarItem("File")
    CreateMenuBarItem("Edit")
end

function CreateToolBar()
    local toolBar_ = toolBar
    toolBar = CmnBase.ExtCObj(toolBarExt_, toolBar_)
	CreateTool(Id.File)
    toolBar:AddSeparator()
    CreateTool(Id.Edit)
end

function CreateCmdMap()
	local event = wx.wxEVT_COMMAND_MENU_SELECTED
	for _, v in ipairs(Id) do
        if v.id ~= nil then
            frame:Connect(v.id, event, v.handler)
        end
	end
end

function CreateWorkArea()
    local splitter = wx.wxSplitterWindow(frame, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)
    
    treeCtrl_ = wx.wxTreeCtrl(splitter, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize)
    treeCtrl = CmnBase.ExtCObj(treeExt_, treeCtrl_)
    --------------------------------------------------------------
    treeCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_TREE_SEL_CHANGED, function (event) treeCtrl:OnSelectedChanged(event:GetItem()) end)
    treeCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, function (event) treeCtrl:ShowPopMenu(event:GetItem(), event:GetPoint()) end)
    treeCtrl:Connect(wx.wxID_ANY, wx.wxEVT_COMMAND_TREE_ITEM_EXPANDED, function (event) treeCtrl:OnNodeExpanded(event:GetItem()) end)
    --------------------------------------------------------------
    listCtrl_ = wx.wxListCtrl(splitter, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxLC_REPORT)
    listCtrl = CmnBase.ExtCObj(listExt_, listCtrl_)
    listCtrl:InsertColumn(0, "属性", wx.wxLIST_FORMAT_LEFT, 60)
    listCtrl:InsertColumn(1, "值", wx.wxLIST_FORMAT_LEFT, wx.wxLIST_AUTOSIZE_USEHEADER)
    listCtrl:InsertItem(0, "绝对路径")
    listCtrl:InsertItem(1, "类型")
    listCtrl:InsertItem(2, "寄存于")
    listCtrl:InsertItem(3, "类名")
    listCtrl:InsertItem(4, "说明")
    --------------------------------------------------------------
    --------------------------------------------------------------
    splitter:SetSashGravity(0.7)
    splitter:SplitVertically(treeCtrl_, listCtrl_)
end

function treeExt_:SetItemNode(itemId, node)
    local treeData = wx.wxLuaTreeItemData()
    treeData:SetData(node)
    self:SetItemData(itemId, treeData)
end

function treeExt_:AllAncestorYesD2M(item)
    local node = self:GetItemNode(item):Parent(false)
    while node do
        if not node:IsD2M() then
            return false
        end
        node = node:Parent(false)
    end
    return true
end

function treeExt_:UpdateD2M(item)
	local isBold = false
	if self:AllAncestorYesD2M(item) then
		isBold = self:GetItemNode(item):IsD2M()
	end    
    self:SetItemBold(item, isBold)
end

function treeExt_:GenTreeByRoot(root)
    self:DeleteAllItems()
    root:GetPriDir()
    local itemRoot = self:AddRoot(root:GetPath())
    self:SetItemNode(itemRoot, root)
    self:GenSubNode(itemRoot)
    return itemRoot
end

function treeExt_:GenSubNode(parent)
	self:UpdateD2M(parent)
    if self:ItemHasChildren(parent) then
        --LogT("has child")
        return
    end
    for v in Iterate(self:GetItemNode(parent):Iterator()) do
        local treeData = wx.wxLuaTreeItemData()
        treeData:SetData(v)
        local item = self:AppendItem(parent, v:GetPath(), -1, -1, treeData)
        self:UpdateD2M(item)
    end
end

function treeExt_:GetItemNode(itemId)
    if not itemId:IsOk() then
        assert(false)
        return
    end
    return self:GetItemData(itemId):GetData()
end

function treeExt_:OnSelectedChanged(itemId)
    listCtrl:ShowNodeAttr(self:GetItemNode(itemId))
    self:AutoEnable(itemId)
end

function treeExt_:GetRootNode()
    local item = treeCtrl:GetRootItem()
    if item:IsOk() then
        return self:GetItemNode(item)
    end
end

function treeExt_:GetSelectNode()
    local item = self:GetSelection()
    return self:GetItemNode(item)
end

function treeExt_:AutoEnable(itemId)
    local edit = Id.Edit
    for _, v in ipairs(edit) do
        if v.id ~= nil then
            v.enable = false
        end
    end
    if not self:GetSelection():IsOk() then
        AutoEnableMenu()
        return
    end
    local node = self:GetItemNode(itemId)
    
    local nodeSys = node:GetSysDir()
    local is_sys_dir = node:Equal(nodeSys)
    if not is_sys_dir then
        local rootNode = node:GetRoot(true)
        rootNode = rootNode:Parent(false)
        if not rootNode or not rootNode:Equal(nodeSys) then
            edit.D2M.enable = true
        end
    end
    if edit.D2M.enable == true then
        if self:AllAncestorYesD2M(itemId) then
			if node:IsD2M() then
				Id.NoD2M.enable = true
			else
				Id.YesD2M.enable = true
				edit.D2M.enable = false
			end
		else
			edit.D2M.enable = false
		end
    end
	
    if is_sys_dir or node:Equal(node:GetPriDir()) or node:Equal(treeCtrl:GetRootNode()) then
        edit.AddNode.enable = true
        edit.Refresh.enable = true
        edit.Find.enable = true
    else
        local ntype = node:GetType()
        local isDlt = true
        if enAS_NodeType.kDir == ntype then
            edit.AddNode.enable = true
            edit.Refresh.enable = true
            edit.Find.enable = true
            local p = node:Parent(false)
            if p and p:Equal(p:GetSysDir()) then
                isDlt = false
            end
        elseif enAS_NodeType.kRef == ntype then
            edit.SetRefAsSrc.enable = true
        elseif enAS_NodeType.kFile == ntype then
            edit.SaveTo.enable = true
        end
        if isDlt then
            edit.DeleteNode.enable = true
            edit.ReplacePath.enable = true
        end
    end
    if ValidRefAsSrc() then
        edit.SetRefAsDst.enable = true
    end
    AutoEnableMenu()
end

function treeExt_:ShowPopMenu(itemId, pos)
    if not self:GetSelection():IsOk() then
        LogW("treeExt_:ShowPopMenu之还未选择节点")
        return
    end
    self:AutoEnable(itemId)
    local popu = CreateMenu(Id.Edit, true)
    self:PopupMenu(popu, pos)
end

function treeExt_:OnNodeExpanded(itemId)
    self:DeleteChildren(itemId)
    self:GenSubNode(itemId)
    
    for item in WxCmn.TreeItemIterator(self, itemId) do
        self:GenSubNode(item)
    end
end

function treeExt_:RefreshItem(itemId, newNode, d2m)
    if newNode ~= nil then
        self:SetItemNode(itemId, newNode)
    end
	local hasChild = self:ItemHasChildren(itemId)
	if d2m == true and not hasChild then
		self:UpdateD2M(itemId)
	end
	if not hasChild then
		self:GenSubNode(itemId)		
	end
    self:Collapse(itemId)
end

function treeExt_:FindChildItemId(itemId, node)
    for item in WxCmn.TreeItemIterator(self, itemId) do
        if node:Equal(self:GetItemNode(item)) then
            return item
        end
    end
end

function treeExt_:GenItemIdsByNode(node)
    local ids = {}
    while node do
        table.insert(ids, node)
        node = node:Parent(false)
    end
    table.remove(ids)
    ids = CmnBase.Reverse(ids)
    local item = self:GetRootItem()
    local res = { item }    
    for _, v in ipairs(ids) do
        --Expand不能删
        self:Expand(item)
        item = self:FindChildItemId(item, v)
        assert(item)
        table.insert(res, item)
    end
    return res
end

function treeExt_:FocuseItemByNode(node)
    local ids = self:GenItemIdsByNode(node)
    self:SelectItem(ids[#ids], true)
end

local function GetNodeTypeName(nodeType)
    if nodeType == enAS_NodeType.kDir then
        return "目录"
    elseif nodeType == enAS_NodeType.kFile then
        return "文件"
    elseif nodeType == enAS_NodeType.kRef then
        return "引用"
    else
        assert(false)
    end
end

local function GetNodeCateName(nodeCate)
    if nodeCate == enAS_NodeCate.kDisk then
        return "硬盘"
    elseif nodeCate == enAS_NodeCate.kMemory then
        return "内存"
    else
        assert(false)
    end
end

local function GetNodeDescription(node)
    if node:Equal(node:GetSysDir()) then
        return "系统目录（应用程序所在目录，不可更改）"
    elseif node:Equal(node:GetPriDir()) then
        return "私有目录"
    elseif node:Equal(treeCtrl:GetRootNode()) then
        return "根目录"
    elseif node:Equal(node:GetRoot(true)) then
        return "顶级节点"
    else
        return "普通子节点"
    end    
end

function listExt_:ShowNodeAttr(node)    
    self:SetItem(0, 1, node:GetAbsolutePath(true, false, false))    
    self:SetItem(1, 1, GetNodeTypeName(node:GetType()))    
    self:SetItem(2, 1, GetNodeCateName(node:GetCate()))
    self:SetItem(3, 1, node:ClsName())
    self:SetItem(4, 1, GetNodeDescription(node))
end
