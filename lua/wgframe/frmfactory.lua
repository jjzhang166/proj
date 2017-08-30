require("WxCmn")
require("FrmStatusBar")
require("FrmMenuBar")
require("AcList")
require("FrmMidArea")
require("FrmHeaderCtrl")
require("BasicSetting")

frmfactory = {
    NewFrmStatusBar_ = nil,
    NewMenuBar_ = nil,
    NewAcList_ = nil,
    NewMidArea_ = nil,
    NewHeaderCtrl_ = nil,
    GetAreaSerInfo_ = nil,
    GetMenuBarMenuInfo_ = nil,
    GetAcListMenuInfo_ = nil,
    GetAcListColumnInfo = nil,
    GetAcListColumnIdxInfo_ = nil,
    GetAcListColumnNameInfo_ = nil,
    GetFrmHeaderCtrlColumnInfo_ = nil,
	NewFrmHeaderCtrlPages_ = nil,
    NewBasicSetting = nil,
}

function frmfactory:NewFrmStatusBar_()
    return FrmStatusBar
end

function frmfactory:NewMenuBar_()
    return FrmMenuBar
end

function frmfactory:NewAcList_()
	return AcList
end

function frmfactory:NewMidArea_()
	return FrmMidArea
end

function frmfactory:NewHeaderCtrl_()
	return FrmHeaderCtrl
end

function frmfactory:GetAreaSerInfo_()
    return {
        ["area1"] = {
            "area1_ser1", "area1_ser2", "area1_ser3",
        },
        ["area2"] = {
            "area2_ser1", "area2_ser2",
        },
    }
end

function frmfactory:GetMenuBarMenuInfo_()
	local tblMenus = 
	{
		{ "帐号", 
				{
                    --A->Account
					{ WxCmn.NewID(), "添加帐号\tAlt+A", "", function () mainfrm:OnAddAc() end },
                    { WxCmn.NewID(), "清空帐号", "删除所有帐号", function () mainfrm:OnP_DeleteAc() end },
                    {},
                    --F->File
                    { WxCmn.NewID(), "创建配置\tAlt+F", "定制专有的帐号配置，可使某些帐号继承自该配置，以便这些帐号可以共享配置", function () mainfrm:OnCreateAcCfg() end },
                    --Q->Query
                    { WxCmn.NewID(), "选择配置\tAlt+Q", "选择想要查看的配置，并且可对当前选择的配置进行修改", function () mainfrm:OnSelectAcCfg() end },
                    {},
                    { WxCmn.NewID(), "导入帐号", "文件内容的格式与导出帐号文件的格式一致", function () mainfrm:OnImportAcCfg() end },
                    { WxCmn.NewID(), "导出帐号", "保存的文件名是->帐号.txt", function () mainfrm:OnExportAcCfg() end },
                    {},
                    --L->Login
                    { WxCmn.NewID(), "批量上号\tAlt+L", "开始登陆所有帐号", function () mainfrm:OnP_Login() end },
                    --C->Cancel
                    { WxCmn.NewID(), "取消批量上号\tAlt+C", "取消登陆所有还未成功登陆的帐号", function () mainfrm:OnP_CancelLogin() end },
                    {},
                    --R->Run
                    { WxCmn.NewID(), "批量运行\tAlt+R", "运行所有帐号", function () mainfrm:OnP_Run() end },
                    --S->Stop
                    { WxCmn.NewID(), "批量停止\tAlt+S", "停止所有帐号", function () mainfrm:OnP_Stop() end },
                    {},
                    --K->Kill
                    { WxCmn.NewID(), "关闭所有游戏\tAlt+K", "关闭所有打开的游戏", function () mainfrm:OnP_CloseGame() end },
				},
		},
	}
    if not GetMyApp():IsDebug() then return tblMenus end
    table.insert(tblMenus,
        { "Test", 
				{
					{ WxCmn.NewID(), "test", "", function () mainfrm:OnTest() end },
                    { WxCmn.NewID(), "关闭登陆连接", "", function () mainfrm:OnCloseLoginSession() end },
                    { WxCmn.NewID(), "运行第一个连接\tR", "", function () mainfrm:OnRunFirstConnection() end },
                    { WxCmn.NewID(), "停止第一个连接\tS", "", function () mainfrm:OnStopFirstConnection() end },
                    { WxCmn.NewID(), "更改脚本", "", function () mainfrm:OnP_ChangeScript(false) end },
                    { WxCmn.NewID(), "执行lua文件", "", function () mainfrm:OnTestDoFile() end },
				},
		})
    return tblMenus
end

function frmfactory:GetAcListMenuInfo_()
    return {
        { WxCmn.NewID(), "添加帐号", "", function () mainfrm:OnAddAc() end },
        { WxCmn.NewID(), "删除帐号", "删除一个或多个已选帐号", function () mainfrm:OnP_DeleteAc(false) end },
        {},
        { WxCmn.NewID(), "更改配置", "更改一个或多个已选帐号的配置", function () mainfrm:OnP_ChangeAcCfg(false) end },
        { WxCmn.NewID(), "更换脚本", "更换一个或多个已选帐号的脚本", function () mainfrm:OnP_ChangeScript(false) end },
        { WxCmn.NewID(), "切换种类", "切换帐号的种类：普通号或仓库号", function () mainfrm:OnP_AcCate(false) end },
        { WxCmn.NewID(), "重置帐号状态", "恢复成正常的帐号状态", function () mainfrm:OnP_ResetAcState(false) end },
        {},
        { WxCmn.NewID(), "上号", "开始登陆一个或多个已选帐号", function () mainfrm:OnP_Login(false) end },
        { WxCmn.NewID(), "取消上号", "取消登陆一个或多个已选帐号", function () mainfrm:OnP_CancelLogin(false) end },
        {},
        { WxCmn.NewID(), "运行", "运行一个或多个已选帐号", function () mainfrm:OnP_Run(false) end },
        { WxCmn.NewID(), "停止", "停止运行一个或多个已选帐号", function () mainfrm:OnP_Stop(false) end },
        {},
        { WxCmn.NewID(), "关闭游戏", "关闭一个或多个游戏", function () mainfrm:OnP_CloseGame(false) end },
        { WxCmn.NewID(), "显示游戏", "显示一个游戏窗口", function () mainfrm:OnShowGameWnd() end },
    }
end

function frmfactory:GetAcListColumnInfo()
    local idxInfo = self:GetAcListColumnIdxInfo_()
    local nameInfo = self:GetAcListColumnNameInfo_()
    local tblIdx = {}
    for k, v in pairs(nameInfo) do
        local name = idxInfo[k + 1]
        assert(name)
        tblIdx[name] = k
    end
    return tblIdx, nameInfo
end

function frmfactory:GetAcListColumnIdxInfo_()
    return {
        "ac",
        "pwd",
        "area",
        "ser",
        "cate",
        "cfg",
        "script",
        "state",
        "scriptInfo",
        "lastTime",
    }
end

function frmfactory:GetAcListColumnNameInfo_()
    return {
        [0] = "帐号",
        "密码",
        "大区",
        "服务器",
        "种类",
        "配置",
        "脚本",
        "状态",      
        "输出信息",
        "有效时间",
    }
end

function frmfactory:GetFrmHeaderCtrlColumnInfo_()
	return 
		{
			"基本设置",
		}
end

function frmfactory:NewFrmHeaderCtrlPages_(parent)
    local basicSetting = self:NewBasicSetting()
	return {
		basicSetting:Create(parent),		
	}
end

function frmfactory:NewBasicSetting()
    return BasicSetting
end
