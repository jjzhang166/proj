require("s_command")
require("sa_cmncfg")
sa_command = {
    CopyAssetFileToShareDir = nil,
    ---------------------------------
    安装apk = nil,
    卸载apk = nil,
    启动app = nil,
    关闭app = nil,
    安装游戏apk = nil,
    卸载游戏apk = nil,
    启动游戏app = nil,
    关闭游戏app = nil,
}
GetMyApp():AddScriptExecEnv("sa_command", sa_command)
local RunInfoP = s_command.RunInfoP
local RunInfoF = s_command.RunInfoF
local RunInfoS = s_command.RunInfoS

function sa_command.CopyAssetFileToShareDir(fileName, newFileName)
    newFileName = newFileName or fileName
    return scmd.CopyAssetFileToShareDir(fileName, newFileName)
end

function sa_command.安装apk(apkName, packageName, isPmInstall)
    if isPmInstall == nil then isPmInstall = false end
    RunInfoP(apkName, packageName)
    if isPmInstall then
        return scmd.PmInstallApk(apkName, packageName)
    else
        return scmd.InstallApk(apkName, packageName)
    end
end

function sa_command.卸载apk(packageName)
    RunInfoP(packageName)
    return scmd.UnInstallApk(packageName)
end

function sa_command.启动app(packageName, activityName)
    RunInfoP(packageName, activityName)
    return scmd.StartApp(packageName, activityName)
end

function sa_command.关闭app(packageName)
    RunInfoP(packageName)
    return scmd.StopApp(packageName)
end

function sa_command.安装游戏apk()
    RunInfoP()
    return scmd.PmInstallApk(sa_cmncfg.gameApkName, sa_cmncfg.gamePackageName)
end

function sa_command.卸载游戏apk()
    RunInfoP()
    return scmd.UnInstallApk(sa_cmncfg.gamePackageName)
end

function sa_command.启动游戏app()
    RunInfoP()
    return scmd.StartApp(sa_cmncfg.gamePackageName, sa_cmncfg.gameMainActivityName)
end

function sa_command.关闭游戏app()
    RunInfoP()
    return scmd.StopApp(sa_cmncfg.gamePackageName)
end
