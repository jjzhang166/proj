VersionCfg = {}

local proxyId = 1

if proxyId == 0 then    --测试机
    VersionCfg.ip = "192.168.1.12"
    VersionCfg.port = 59421
    VersionCfg.loginTitle = "登陆"
    VersionCfg.frmTitle = "流放之路"
elseif proxyId == 1 then --周卫
    VersionCfg.ip = "23.234.25.16"
    VersionCfg.port = 59422
    VersionCfg.loginTitle = "流放之路(内)"
    VersionCfg.frmTitle = "DEMON(内)"
elseif proxyId == 2 then --雨沫
    VersionCfg.ip = "23.234.25.16"
    VersionCfg.port = 59423
    VersionCfg.loginTitle = "登陆"
    VersionCfg.frmTitle = "流放之路"
elseif proxyId == 3 then --漫步
    VersionCfg.ip = "23.234.25.16"
    VersionCfg.port = 59424
    VersionCfg.loginTitle = "登陆-魔"
    VersionCfg.frmTitle = "流放之路-魔"
end
