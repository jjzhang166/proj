require("CmnBase")
FrmMidArea = {
	middleArea = nil,
	
	Create = nil,
}

function FrmMidArea:Create(parent)
	assert(parent)
	self.middleArea = wx.wxWindow(parent, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)
	return CmnBase.ExtCObj(self, self.middleArea)
end