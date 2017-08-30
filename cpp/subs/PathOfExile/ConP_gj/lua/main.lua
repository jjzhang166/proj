
wxApp = wx.wxGetApp()
myApp = GetMyApp()

local loadFiles = {
    "loginui_p", "MyCfg", "MyFrm",
}

for _, v in ipairs(loadFiles) do
	require(v)
end

consolecfg:LoadMainCfg(consolecfg.MakeCfgInfo("配置"))
----------------------------------------------------------------------------
--mainfrm:CreateFrm("流放之路")
----[[
loginui_p.ui.ShowLogin("POE", function()
		mainfrm:CreateFrm("流放之路")
	end)
--]]