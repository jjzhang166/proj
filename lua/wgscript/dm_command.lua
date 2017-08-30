require("s_cmncfg")
require("s_StateResolver")
dm_command = {
--    dm = luacom.CreateObject("dm.dmsoft"),
    dm = nil,
    ----------------------
    InitDm = nil,
    FindPic = nil,
    ----------------------
    --DmS->DmState
    StateFindPicNode = nil,
    ×´Ì¬ÕÒÍ¼½áµã = nil,
    DmS_FindPicInfo = nil,
    DmS_HandleClick = nil,
    DmS_HandlePassScene = nil,
    DmS_HandleNothing = nil,
    DmS_FindPic = nil,
    DmS_FindPicClick = nil,
    DmS_FindPicPassScene = nil,
    ----------------------
    DmInputBindWndString = nil,
    DmClickLBt = nil,
}
GetMyApp():AddScriptExecEnv("dm_command", dm_command)

function dm_command:InitDm()
    assert(self.dm == nil)
    self.dm = DmOper()
    local dmRegKey = s_cmncfg.dmRegKey
    if #dmRegKey > 0 then
        local regRes = self.dm:Reg(dmRegKey, "")
        if regRes ~= 1 then
            LogF("´óÄ®×¢²áÊ§°Ü£¬´íÎóÂë£º", regRes)
            assert(false, "dm:Reg")
        end
    end
end

function dm_command.FindPic(picName, deltaColor, sim, dir)
    assert(picName)
    picName = picName .. ".bmp"
    deltaColor = deltaColor or ""
    sim = sim or 0.9
    dir = dir or 0
--    return dm_command.dm:FindPic(0, 0, 2000, 2000, picName, deltaColor, sim, dir)
--    --[[
    local res, x, y = dm_command.dm:FindPic(0, 0, 2000, 2000, picName, deltaColor, sim, dir)
    if x >= 0 and y >= 0 then
        LogT("ÕÒµ½Í¼Æ¬£º", picName, "\t×ø±êx:", x, "\t×ø±êy:", y)
    end
    return res, x, y
    --]]
end
local DmFindPic = dm_command.FindPic

local function GetDmObjImpl_()
    local dm = dm_command.dm
    assert(dm)
    return dm
end

local StateFindPicNodeImpl = s_StateResolver.StateFindPicNode
function dm_command.StateFindPicNode(stateName, ...)
    return StateFindPicNodeImpl(stateName, GetDmObjImpl_, ...)
end
dm_command.×´Ì¬ÕÒÍ¼½áµã = dm_command.StateFindPicNode

function dm_command.DmS_FindPicInfo(picName, ...)
    if type(picName) == "string" then
        picName = picName .. ".bmp"
        return { picName, ... }
    end
    local names = {}
    for _, v in ipairs(picName) do
        table.insert(names, v .. ".bmp")
    end
    return { names, ... }
end

function dm_command.DmS_HandleClick(stateNode, picName, posX, posY)
    local dm = dm_command.dm
    dm:MoveTo(posX, posY)
    dm:LeftClick()
end

function dm_command.DmS_HandlePassScene(stateNode, picName, posX, posY)
    return false
end

function dm_command.DmS_HandleNothing(stateNode, picName, posX, posY)
    return true
end

function dm_command.DmS_FindPic(picName, fnHandle, ...)
    local fnPrepare = function ()
        local res, x, y = DmFindPic(picName)
        return x >= 0 and y >= 0
    end
    return { picName, fnPrepare, fnHandle, ... }
end

function dm_command.DmS_FindPicClick(picName, clickX, clickY, ...)
    local fnPrepare
    if clickX == nil or clickY == nil then
        fnPrepare = function ()
            local res, x, y = DmFindPic(picName)
            if x >= 0 and y >= 0 then
                clickX = x
                clickY = y
                return true
            else
                return false
            end
        end
    else
        fnPrepare = function ()
            local res, x, y = DmFindPic(picName)
            return x >= 0 and y >= 0
        end
    end
    local fnHandle = function ()
        local dm = dm_command.dm
        dm:MoveTo(clickX, clickY)
        dm:LeftClick()
    end
    return { picName, fnPrepare, fnHandle, ... }
end

local function RetFalse()
    return false
end

function dm_command.DmS_FindPicPassScene(picName)
    return dm_command.DmS_FindPic(picName, RetFalse)
end

function dm_command.DmInputBindWndString(str, posX, posY)
    return dm_command.dm:InputBindWndString(str, posX, posY)
end

function dm_command.DmClickLBt(posX, posY)
    local dm = dm_command.dm
    dm:MoveTo(posX, posY)
    dm:LeftClick()
end
