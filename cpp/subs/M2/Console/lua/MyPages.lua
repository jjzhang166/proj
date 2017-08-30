require("BasicSetting")
MyPages = {
    MyBasicSetting = {
        __index = BasicSetting,
        -------------
        GetSelectPathTitleInfo = nil,
    },
}
local MyBasicSetting = MyPages.MyBasicSetting
setmetatable(MyBasicSetting, MyBasicSetting)

function MyBasicSetting:GetSelectPathTitleInfo()
    return "选择流放之路程序", "PathOfExile.exe|PathOfExile.exe"
end
