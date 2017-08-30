
wxApp = wx.wxGetApp()
myApp = GetMyApp()

local loadFiles = {
    "loginui", "MyCfg", "MyFrm",
}

for _, v in ipairs(loadFiles) do
	require(v)
end

consolecfg:LoadMainCfg(consolecfg.MakeCfgInfo("配置"))
----------------------------------------------------------------------------

loginui.ui.ShowLogin("登陆", function()
		mainfrm:CreateFrm("流放之路")
	end)
