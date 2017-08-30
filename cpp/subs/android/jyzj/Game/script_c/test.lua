
local dm = dm

--安装游戏apk()
--卸载游戏apk()
--启动游戏app()
--关闭游戏app()
-------------------------------------------------------

--dm:FreePic("systemnoticebt.bmp")

--TestReloadGameLua("jyzj", "mycmd", true)
--[[
TestReloadGameLua("jyzj", "StatesInfo", true)
LoadStatesInfo()
OutputScriptInfo("Resolve")
s_StateResolver:Resolve()
--]]

--选择大区()

dm:EnablePicCache(0)
print(dm:FindPicEx(500, 100, 800, 200, "gametask.bmp", "", 0.9, 0))
