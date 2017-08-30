
wxApp = wx.wxGetApp()
myApp = GetMyApp()
------------------------------------------------------------------------------------------------------------
local inFileCnt = myApp:FileCount(true)
if inFileCnt > 0 then
	print("启动命令行模式")
--	require("CmnBase")
	require("cmdimpl")
	H_DoCmdLineFiles()
	print("所有文件已处理完毕，退出消息循环，正常结束进程")
	ShellM([[
	wxApp:ExitMainLoop()
	]])
	return
end
------------------------------------------------------------------------------------------------------------
require("mainui")
------------------------------------------------------
frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "AssetExplore")
statusBar = frame:CreateStatusBar()
menuBar = wx.wxMenuBar()
frame:SetMenuBar(menuBar)
toolBar = frame:CreateToolBar(wx.wxNO_BORDER + wx.wxTB_FLAT + wx.wxTB_DOCKABLE)
config = WxCmn.FileConfig()
treeCtrl = nil
listCtrl = nil


CreateMenuBar()
CreateToolBar()
CreateCmdMap()
CreateWorkArea()
AutoEnableMenu()

toolBar:Realize()
------------------------------------------------------
frame:SetStatusText("若是允许Disk2Memory，则该节点标签是粗体状态，否则是正常状态")

config:RestoreWndPos(frame, "MainFrame")
local function CloseWindow(event)
    --print("close frame")
    config:SaveWndPos(frame, "MainFrame")
    config:delete() -- always delete the config
    event:Skip(true)   --let fame call frame:Destroy()
end
frame:Connect(wx.wxEVT_CLOSE_WINDOW, CloseWindow)
frame:Show(true)
------------------------------------------------------
------------------------------------------------------------------------------------------------------------
