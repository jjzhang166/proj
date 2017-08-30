require("CmnBase")
FrmHeaderCtrl = {
	wndContainer = nil,
	headerCtrl = nil,
	wndPages = {},
	lastPage = 1,
	
    -------------------------------
    PageBase = {
        UpdateFromCurSelected = nil,
        UpdateFromItemData = nil,
    },
    -------------------------------
	Create = nil,
	CreatePages = nil,
    UpdatePagesFromCurSelected = nil,
    UpdatePagesFromItemData = nil,
}
local PageBase = FrmHeaderCtrl.PageBase

function PageBase:UpdateFromCurSelected(itemData)
end

function PageBase:UpdateFromItemData(item, itemData)
end

function FrmHeaderCtrl:Create(parent)
	assert(parent)
	self.wndContainer = wx.wxSplitterWindow(parent, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxFULL_REPAINT_ON_RESIZE)
	self.headerCtrl = wx.wxHeaderCtrlSimple(self.wndContainer)
	self:CreatePages()
	return CmnBase.ExtCObj(self, self.wndContainer)
end

local function GetRawWnd(wnd)
    if type(wnd) == "table" then 
        return wnd:GetLuaBase()
    end
    return wnd
end

local function FrmHeaderCtrlCreatePages(self, factory)
    local pages = factory:NewFrmHeaderCtrlPages_(self.wndContainer)
    local wndPages = self.wndPages
    local pageBase = self.PageBase
    for k, v in pairs(pages) do
        wndPages[k] = CmnBase.OptionCopy(pageBase, v)
    end
end

function FrmHeaderCtrl:CreatePages()
	local headerCtrl = self.headerCtrl
	local align = wx.wxAlignment.wxALIGN_CENTER
	local width = 100
    local factory = mainfrm.factory
	local columnInfo = factory:GetFrmHeaderCtrlColumnInfo_()
	for _, v in ipairs(columnInfo) do
		headerCtrl:AppendColumn(wx.wxHeaderColumnSimple(v, width, align))
	end
	
    FrmHeaderCtrlCreatePages(self, factory)
--    self.wndPages[1]:SetLabel("123")
--    self.wndPages[2]:SetLabel("abc")
	assert(#self.wndPages == #columnInfo)
	for _, v in ipairs(self.wndPages) do
		v:Show(false)
	end
	
    self.wndContainer:SetSashGravity(0.15)
    self.wndContainer:SplitHorizontally(self.headerCtrl:GetLuaBase(), GetRawWnd(self.wndPages[self.lastPage]))
    self.headerCtrl:ShowSortIndicator(self.lastPage - 1, false)
    
	headerCtrl:Connect(wx.wxEVT_HEADER_CLICK, function (event) 
		event:Skip(true)
        event = wx.wxHeaderCtrlEvent(event)
        local newIdx = event:GetColumn() + 1
        if newIdx == self.lastPage then return end
        self.wndPages[self.lastPage]:Show(false)
        self.wndPages[newIdx]:Show(true)
        self.wndContainer:ReplaceWindow(GetRawWnd(self.wndPages[self.lastPage]), GetRawWnd(self.wndPages[newIdx]))
        self.lastPage = newIdx
        self.headerCtrl:ShowSortIndicator(newIdx - 1, false)
	end)
end

function FrmHeaderCtrl:UpdatePagesFromCurSelected(itemData)
    local page = self.wndPages[self.lastPage]
    if not page:IsShown() then return end
    page:UpdateFromCurSelected(itemData)
end

function FrmHeaderCtrl:UpdatePagesFromItemData(item, itemData)
    local page = self.wndPages[self.lastPage]
    if not page:IsShown() then return end
    page:UpdateFromItemData(item, itemData)
end
