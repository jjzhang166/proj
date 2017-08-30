
wxApp = wx.wxGetApp()
myApp = GetMyApp()

local loadFiles = {
    "loginui", "MyCfg", "MyFrm", "VersionCfg"
}

for _, v in ipairs(loadFiles) do
	require(v)
end

consolecfg:LoadMainCfg(consolecfg.MakeCfgInfo("≈‰÷√"))
----------------------------------------------------------------------------
loginui.ui.ShowLogin(VersionCfg.loginTitle, function()
		mainfrm:CreateFrm(VersionCfg.frmTitle)
	end)
