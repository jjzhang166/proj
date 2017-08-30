require("WxCmn")
require("cmdhandler")
local function MakeCmd(name, id, handler, artId, shortcut, prompt, enable)
	local cmd = {}
    if name == nil then
        return cmd
    end
	cmd[name] = cmd
	cmd.name = name
	cmd.id = id
	cmd.handler = handler
    cmd.artId = artId
	cmd.prompt = prompt
	cmd.shortcut = shortcut
    cmd.enable = enable
	return cmd
end


Id = {
	-- File menu
	File = {
		MakeCmd("New", wx.wxID_NEW, H_New, wx.wxART_NORMAL_FILE, "Ctrl-N", "Create a Asset System file"),
		MakeCmd("Open", wx.wxID_OPEN, H_Open, wx.wxART_FILE_OPEN, "Ctrl-O", "Open a exist Asset System file"),
		MakeCmd("Close", WxCmn.NewID(), H_Close, nil, "Ctrl+W", "Close current Asset System file"),
        MakeCmd(),
		MakeCmd("Save", wx.wxID_SAVE, H_Save, wx.wxART_FILE_SAVE, "Ctrl-S", "Save current Asset System file"),
		MakeCmd("SaveAs", wx.wxID_SAVEAS, H_SaveAs, wx.wxART_FILE_SAVE_AS, "Ctrl-Shift-S", "Save Asset System to another file"),
        MakeCmd(),
		},
        
    -- Edit menu
    Edit = {
        MakeCmd("AddNode", wx.wxID_ADD, H_AddNode, wx.wxART_NEW, "Ctrl-A", "Add a sub node to the selected item", false),
        MakeCmd("DeleteNode", wx.wxID_DELETE, H_DeleteNode, wx.wxART_DELETE, "Ctrl-D", "Delete the selected item", false),
        MakeCmd("ReplacePath", wx.wxID_REPLACE, H_ReplacePath, wx.wxART_EDIT, "Ctrl-R", "Replace the path of the selected item", false),
        MakeCmd("Refresh", wx.wxID_REFRESH, H_Refresh, nil, "F5", "Refresh the selected item", false),
        MakeCmd("SaveTo", WxCmn.NewID(), H_SaveTo, nil, "Alt+T", "保存到", false),
        MakeCmd(),
        MakeCmd("YesD2M", WxCmn.NewID(), H_YesD2M, nil, "Alt+Y", "允许D2M", false),
        MakeCmd("NoD2M", WxCmn.NewID(), H_NoD2M, nil, "Alt+N", "不允许D2M", false),
        MakeCmd("D2M", WxCmn.NewID(), H_D2M, nil, "Alt+D", "文件转换到内存", false),
        MakeCmd(),
        MakeCmd("SetRefAsSrc", wx.wxID_FORWARD, H_SetRefAsSrc, wx.wxART_GO_FORWARD, "Ctrl-[", "引用源", false),
        MakeCmd("SetRefAsDst", wx.wxID_BACKWARD, H_SetRefAsDst, wx.wxART_GO_BACK, "Ctrl-]", "引用目标", false),
        MakeCmd(),
        MakeCmd("Find", wx.wxID_FIND, H_Find, wx.wxART_FIND, "Ctrl-F", "首字符若为':'，则表示只查找子节点；首字符若为?，则表示查找后代节点；否则是普通查找（支持像../、./等的相对路径）。", false),
    }
}
Id = WxCmn.RectifyMenuIds(Id)

