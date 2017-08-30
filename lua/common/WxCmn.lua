require("CmnBase")
WxCmn = {
NewID = nil,	--Generate a unique new wxWindowID
RectifyMenuIds = nil, --使菜单栏和命令都能被顶层表索引到
FileConfig = nil,
--对话框
OpenFile = nil,
OpenDir = nil,
GetTextFromDlg = nil,
SingleChoiceDlg = nil,
CreateTextCtrl = nil,
-------------------------
TreeItemIterator = nil,
SetStatusBarSize = nil,
SetHorizonPos = nil,	--水平
SetVerticalPos = nil,	--垂直
}

local ID_IDCOUNTER = wx.wxID_HIGHEST + 1
function WxCmn.NewID()
    ID_IDCOUNTER = ID_IDCOUNTER + 1
    return ID_IDCOUNTER
end

function WxCmn.RectifyMenuIds(ids)
	local tbls = {}
	for _, v in pairs(ids) do
		table.insert(tbls, CmnBase.GrandsonAsSon(v))
	end
	table.insert(tbls, ids)
	tbls = CmnBase.UnionTable(unpack(tbls))
	return CmnBase.GrandsonAsSon(tbls)
end

local _fileConfig = {}
function _fileConfig:SaveWndPos(window, windowName)
	local path = self:GetPath()
    self:SetPath("/"..windowName)

    local s    = 0
    local w, h = window:GetSizeWH()
    local x, y = window:GetPositionXY()

    if window:IsMaximized() then
        s = 1
    elseif window:IsIconized() then
        s = 2
    end

    self:Write("s", s)

    if s == 0 then
        self:Write("x", x)
        self:Write("y", y)
        self:Write("w", w)
        self:Write("h", h)
    end

    self:SetPath(path)
end

function _fileConfig:RestoreWndPos(window, windowName)
	local path = self:GetPath()
    self:SetPath("/"..windowName)

    local _, s = self:Read("s", -1)
    local _, x = self:Read("x", 0)
    local _, y = self:Read("y", 0)
    local _, w = self:Read("w", 0)
    local _, h = self:Read("h", 0)

    if s == 0 then
        local clientX, clientY, clientWidth, clientHeight
        clientX, clientY, clientWidth, clientHeight = wx.wxClientDisplayRect()

        if x < clientX then x = clientX end
        if y < clientY then y = clientY end

        if w > clientWidth  then w = clientWidth end
        if h > clientHeight then h = clientHeight end

        window:SetSize(x, y, w, h)
    elseif s ~= -1 then
        window:Maximize(true)
    end

    self:SetPath(path)
end

function WxCmn.FileConfig(...)
	local impl = wx.wxFileConfig(...)
	assert(impl)
	return CmnBase.ExtCObj(_fileConfig, impl)
end

function WxCmn.OpenFile(wndParent, title, selectorStr, flag)
    title = title or "Open file"
    selectorStr = selectorStr or "Lua files (*.lua)|*.lua|Text files (*.txt)|*.txt|All files (*)|*"
    flag = flag or wx.wxFD_OPEN + wx.wxFD_FILE_MUST_EXIST
    local fileDialog = wx.wxFileDialog(wndParent, title, "", "", selectorStr, flag)
    local res
    if fileDialog:ShowModal() == wx.wxID_OK then
        res = fileDialog:GetPath()
    end
    fileDialog:Destroy()
    return res
end

function WxCmn.OpenDir(wndParent, title)
    title = title or "Open dir"
    local dlg = wx.wxDirDialog(wndParent, title, "", wx.wxDD_DEFAULT_STYLE + wx.wxDD_DIR_MUST_EXIST)
    local res
    if dlg:ShowModal() == wx.wxID_OK then
        res = dlg:GetPath()
    end
    dlg:Destroy()
    return res
end

function WxCmn.GetTextFromDlg(wndParent, caption, defText, msg)
    caption = caption or ""
    msg = msg or ""
    defText = defText or ""
    local dlg = wx.wxTextEntryDialog(wndParent, msg, caption, defText)
    local res
    if dlg:ShowModal() == wx.wxID_OK then
        res = dlg:GetValue()
    end
    dlg:Destroy()
    return res
end

function WxCmn.SingleChoiceDlg(wndParent, title, arrayStr)
    title = title or "请选择下面的一项"
    local dlg = wx.wxSingleChoiceDialog(wndParent, "", title, arrayStr)
    local res
    if dlg:ShowModal() == wx.wxID_OK then
        res = dlg:GetStringSelection()
    end
    dlg:Destroy()
    return res
end

function WxCmn.CreateTextCtrl(parent, title, style, maxLen, statusBar, statusStr)
	title = title or ""
	local ctrl = wx.wxTextCtrl(parent, wx.wxID_ANY, title, wx.wxDefaultPosition, wx.wxDefaultSize, style)
    if maxLen ~= nil then
	    ctrl:SetMaxLength(maxLen)
    end
	local size = ctrl:GetSize():Scale(1.8, 1)
	ctrl:SetSize(-1, -1, size:GetWidth(), -1)
	if statusStr then
		ctrl:Connect(wx.wxEVT_SET_FOCUS, function(event) 
		statusBar:PushStatusText(statusStr)
		event:Skip(true)
		end)
		ctrl:Connect(wx.wxEVT_KILL_FOCUS, function(event) 
			statusBar:PopStatusText()
			event:Skip(true)
		end)
	end
	return ctrl
end

function WxCmn.TreeItemIterator(tree, itemId)
    local item, cookie = tree:GetFirstChild(itemId)
    return function()
        if item:IsOk() then
            local res = item
            item, cookie = tree:GetNextChild(itemId, cookie)
            return res
        end
    end
end

function WxCmn.SetStatusBarSize(statusBar, clientSize)
	local size = statusBar:GetSize()
	statusBar:SetSize(0, clientSize:GetHeight() - size:GetHeight(), clientSize:GetWidth(), wx.wxDefaultCoord, wx.wxSIZE_AUTO_HEIGHT)
end

function WxCmn.SetHorizonPos(preCtrl, curCtrl, yScale, distanceScale, minWidth, maxWidth)
	local prePos = preCtrl:GetPosition()
	local preWidth = preCtrl:GetSize():GetWidth()
	if minWidth ~= nil and preWidth < minWidth then preWidth = minWidth end
	if maxWidth ~= nil and preWidth > maxWidth then preWidth = maxWidth end
	yScale = yScale or 1
	distanceScale = distanceScale or 0.3
	curCtrl:Move(wx.wxPoint(prePos.x + preWidth + preWidth * distanceScale, prePos.y * yScale))
end

function WxCmn.SetVerticalPos(preCtrl, curCtrl, xScale, distanceScale, minHeight, maxHeight)
	local prePos = preCtrl:GetPosition()
	local preHeight = preCtrl:GetSize():GetHeight()
	if minHeight ~= nil and preHeight < minHeight then preHeight = minHeight end
	if maxHeight ~= nil and preHeight > maxHeight then preHeight = maxHeight end
	xScale = xScale or 1
	distanceScale = distanceScale or 1
	curCtrl:Move(wx.wxPoint(prePos.x * xScale, prePos.y + preHeight + preHeight * distanceScale))
end