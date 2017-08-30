require("androidfrm")
require("MyPages")
MyFrm = {
    __index = androidfrm,
    -------------------------
    ---------test
    OnTest = nil,
    OnCloseLoginSession = nil,
    -------------------------
    NewFrmFactory_ = nil,
}
setmetatable(MyFrm, MyFrm)
mainfrm = MyFrm

MyFrmFactory = {
    __index = androidfrmfactory,
    ----------------
    GetMenuBarMenuInfo_ = nil,
    GetFrmHeaderCtrlColumnInfo_ = nil,
	NewFrmHeaderCtrlPages_ = nil,
    GetAreaSerInfo_ = nil,
}
setmetatable(MyFrmFactory, MyFrmFactory)


function MyFrm:OnTest()
--    --[[
	local app = GetMyApp()
    app:Test("")
    --]]
end

function MyFrm:OnCloseLoginSession()
    local acList = self:GetAcListCfg()
    local stateLogining = acList.stateLogining
    for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(item + 1)
        if itemData:GetState() == stateLogining then
            self.uiOperHandler:CloseLoginSession(itemData:GetItemKey())
        end
    end
end

function MyFrm:NewFrmFactory_()
	return MyFrmFactory
end

function MyFrmFactory:GetMenuBarMenuInfo_()
    local res = CmnBase.Super(self).GetMenuBarMenuInfo_(self)
    local my = 
	{
		{ "Test", 
					{
						{ WxCmn.NewID(), "test", "", function () mainfrm:OnTest() end },
                        { WxCmn.NewID(), "关闭登陆连接", "", function () mainfrm:OnCloseLoginSession() end },
                        { WxCmn.NewID(), "运行第一个连接\tR", "", function () mainfrm:OnRunFirstConnection() end },
                        { WxCmn.NewID(), "停止第一个连接\tS", "", function () mainfrm:OnStopFirstConnection() end },
                        { WxCmn.NewID(), "更改脚本", "", function () mainfrm:OnP_ChangeScript(false) end },
                        { WxCmn.NewID(), "执行lua文件", "", function () mainfrm:OnTestDoFile([[F:\proj\cpp\subs\android\jyzj\Console\lua\test.lua]]) end },
					},
		},
	}
    for _, v in ipairs(my) do
        table.insert(res, v)
    end
    return res
end

function MyFrmFactory:GetFrmHeaderCtrlColumnInfo_()
	return 
		{
			"基本设置",
		}
end

function MyFrmFactory:NewFrmHeaderCtrlPages_(parent)
	return {
		MyPages.basic:Create(parent),
	}
end

function MyFrmFactory:GetAreaSerInfo_()
    return {
        ["时间服"] = {
            "一帆风顺", "锦绣年华", "温文尔雅",
        },
        ["地界"] = {
            "百鸟朝凤", "旧貌新颜", "缘定三生",
        },
    }
end
