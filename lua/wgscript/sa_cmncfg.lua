require("dm_cmncfg")
sa_cmncfg = {
    __index = dm_cmncfg,
    ----------------------
    gameApkName = "",
    gamePackageName = "",
    gameMainActivityName = "",
    ----------------------
    OnS_GameConnected = nil,
}
setmetatable(sa_cmncfg, sa_cmncfg)
local dm_cmncfg = dm_cmncfg
--s_cmncfg = sa_cmncfg

function sa_cmncfg:OnS_GameConnected()
    dm_cmncfg.OnS_GameConnected(self)
    local gameApkName = self.gameApkName
    assert(#gameApkName > 0)
    sa_command.CopyAssetFileToShareDir(gameApkName)
end
