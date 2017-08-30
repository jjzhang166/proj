require("CmnBase")
require("WxCmn")
FrmMenuBar = {
	menuBar = nil,
    parent_ = nil,
	
	CreateMenuBar = nil,
}

function FrmMenuBar:CreateMenuBar(parent)
    self.parent_ = parent
	self.menuBar = wx.wxMenuBar()
	local menus = mainfrm.factory:GetMenuBarMenuInfo_()
	if menus then
		for _, v in ipairs(menus) do
            local info = v[2]
			self.menuBar:Append(wx.wxMenu(info), v[1])
            for _, item in ipairs(info) do
                local itemId = item[1]
                if itemId ~= nil then
                    self.menuBar:Connect(itemId, wx.wxEVT_COMMAND_MENU_SELECTED, item[4])
                end
            end
		end
	end
	return CmnBase.ExtCObj(self, self.menuBar)
end
