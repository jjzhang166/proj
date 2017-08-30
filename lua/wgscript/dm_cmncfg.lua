require("s_cmncfg")
dm_cmncfg = {
    __index = s_cmncfg,
    -----------------------
    OnS_GameConnected = nil,
}
setmetatable(dm_cmncfg, dm_cmncfg)
local s_cmncfg = s_cmncfg

function dm_cmncfg:OnS_GameConnected()
    s_cmncfg.OnS_GameConnected(self)
    dm_command:InitDm()
    local app = GetMyApp()
    local dm = dm_command.dm
    assert(dm:SetPath(app:GetAssetDirString()) == 1)

    local hwnd = app:GetGameTopHwnd()
    assert(hwnd > 0)
    dm:BindWindowEx(hwnd, "dx.graphic.opengl", "windows", "windows", "dx.public.graphic.protect|dx.public.anti.api", 0)
end
