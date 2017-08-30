require("s_command")
mycmd = {

}
local scmd = scmd
local RunInfo = s_command.RunInfo
local RunInfoP = s_command.RunInfoP
local RunInfoF = s_command.RunInfoF
local RunInfoS = s_command.RunInfoS

---------------------------------
---------------配置相关
function 自动开箱子(enable)
    if enable == nil then enable = true end
    RunInfoP(enable)
    return scmd.EnableAutoOpenChest(enable)
end

function 自动捡物品(enable)
    if enable == nil then enable = true end
    RunInfoP(enable)
    return scmd.EnableAutoPickupItem(enable)
end

function 自动开传送点(enable)
    if enable == nil then enable = true end
    RunInfoP(enable)
    return scmd.EnableAutoOpenWaypoint(enable)
end

function 自动开门(enable)
    if enable == nil then enable = true end
    RunInfoP(enable)
    return scmd.EnableAutoOpenTransitionable(enable)
end

function 配置天赋树(jobSkillNode)
    RunInfoP()
    return scmd.SetPassiveSkillTree(jobSkillNode)
end

function 默认升级技能宝石(enable)
    if enable == nil then enable = true end
    RunInfoP(enable)
    return scmd.EnableAutoLvlUpSkillGemByDef(enable)
end

--参数1：血量低于百分比 0.6 =  60%;参数2：魔力低于百分比0.6 = 60%
function 设置自动补血补蓝(hpRate, mpRate)
    RunInfoP(hpRate, mpRate)
    return scmd.SetAutoUseFlask(hpRate, mpRate)
end

function 设置全局危险距离(screenRate)
    RunInfoP(screenRate)
    return scmd.SetGlobalDangerDistance(screenRate)
end

function 创建技能(skillName)
    RunInfoP(skillName)
    return scmd.MakeSkill(skillName)
end

function 创建尸体技能(skillName)
    RunInfoP(skillName)
    return scmd.MakeCorpseSkill(skillName)
end

function 添加技能(skill)
    RunInfoP()
    return scmd.AddSkill(skill)
end

function 清空技能()
    RunInfoP()
    return scmd.ClearSkills()
end
---------------------------------
function 等待可移动(timeout)
    if type(timeout) ~= "number" then
        timeout = 60 * 3
    end
    RunInfoP(timeout .. "秒")
    return scmd.WaitRoleCanMove(timeout * 1000)
end

--包括拾取物品，开箱子，传送门等，只传入要处理的名字即可
function 打开目标(obj)
    if type(obj) ~= "userdata" then
        RunInfoP(obj)
    else
        RunInfoP()
    end
    return scmd.OpenObj(obj)
end

--传入查找到的目标对象
function 移动到目标(obj)
    if type(obj) ~= "userdata" then
        RunInfoP(obj)
    else
        RunInfoP()
    end
    return scmd.MoveToTheObj(obj)
end

function 杀到房间(dstName)
    RunInfoP(dstName)
    return scmd.KillToTheRoom(dstName)
end

--全图杀怪
function 全图杀怪(clearMarkPosFirst)
    if clearMarkPosFirst == nil then clearMarkPosFirst = true end
    RunInfoP(clearMarkPosFirst)
    return scmd.KillAllMonster(clearMarkPosFirst)
end

--参数: 目标名字(限当前地图)
function 杀到打开目标(objName)
     if type(objName) ~= "string" then 
        LogE("杀到打开目标之参数必须是技能名字")
        return false
    end
    RunInfoP(objName)
    return scmd.KillToOpenObj(objName)
end

--参数：怪物名字(限当前地图)
function 杀到杀死目标(obj)
    if type(obj) == "string" then 
       RunInfoP(obj)
    else
       RunInfoP()
    end
    return scmd.KillToKillTheObj(obj)
end

--参数:目标地图名字(当前难度等级)
function 杀到指定地图(mapName,diffLvl,useWay)
    if type(mapName) ~= "string" then
        LogE("杀到指定地图之参数必须是目标地图名字")
        return false;
    end
    if diffLvl == nil then diffLvl = 0 end
    if useWay == nil then useWay = true end
    RunInfoP(mapName)
    return scmd.KillToTheMap(mapName,diffLvl,useWay)
end

--参数:目标名字(限当前地图)
function 杀到目标(objName)
    if type(objName) ~= "string" then
        LogE("杀到目标之参数必须是目标名字")
        return false
    end
    RunInfoP(objName)
    return scmd.KillToTheObj(objName)
end

--参数：目标npc名字(访问后会变成怪的，例如盗贼)
function 杀到杀死NPC怪(npcName)
    if type(npcName) ~= "string" then
        LogE("杀到杀死NPC怪之参数必须是npc名字")
        return false
    end
    RunInfoP(npcName)
    return scmd.KillTokillNpc(npcName)
end

--移动到指定npc 参数1: 城镇名字 参数2: npc名字
function 移动到指定NPC(townName,npcName)
    if type(townName) ~= "string" then
        LogE("移动到指定NPC之目标地图不能为空")
        return false
    end
    if type(npcName) ~= "string" then 
        LogE(移动到指定NPC之目标npc名称不能为空)
        return false;
    end
    RunInfoP(townName, npcName)
    return scmd.MoveToTheNpc(townName,npcName)
end

--移动到指定副本，参数1：地图名字 参数2：地图等级(1:一般  2:残酷 3:无情)
function 移动到指定地图(mapName,diffLvl)
    if type(mapName) ~= "string" then
        LogE("移动到指定地图之目标地图名称不能为空")
        return false
    end
    if type(diffLvl) ~= "number" then 
        LogE("移动到指定地图之目标地图等级必须为数字 1- 3之间")
        return false
    end
    RunInfoP(mapName)
    return scmd.SmartMoveMap(mapName,diffLvl)
end

--此函数 可以领取奖励和从npc处购买物品（物品名字或者索引）
function 获取奖励物品(itemNameOrIndex)
    if type(itemNameOrIndex) ~= "string" and type(itemNameOrIndex) ~= "number" then
        LogE("获取奖励物品之参数必须是物品名字或者物品所在索引")
        return false
    end
    RunInfoP(itemNameOrIndex)
    return scmd.TakeNpcItem(itemNameOrIndex)
end

--参数:npc对话内容(列表框中的一条内容)
function 对话NPC(talkName)
    if type(talkName) ~= "string" then
         LogE("对话NPC之对话内容必须是字符串，及npc对话列表中的内容")
         return false
    end
    RunInfoP(talkName)
    return scmd.TalkToNpc(talkName)
end

function 选择击败怪物()
    RunInfoP()
    return scmd.ChooseKillTheMonster()
end

--参数:真或假
function 关闭所有面板(isCloseNpcPanel)
    if isCloseNpcPanel == nil then isCloseNpcPanel = true end
    RunInfoP()
    return scmd.CloseAllPanel(isCloseNpcPanel)
end

--参数:使用物品名字
function 使用物品(itemName)
    if type(itemName) ~= "string" then 
        LogE("使用物品之物品名必须是字符串")
        return false
    end
    RunInfoP(itemName)
    return scmd.UseItem(itemName)
end

function 查找最近的掉落物品()
    RunInfoP()
    return scmd.FindNearestObj(GameObjType.kGOT_WorldItem)
end

--参数:怪物名字
function 查找最近的目标(name)
    RunInfoP(name)
    return scmd.FindNearestObj(name)
end

function 查找最近怪物(monName)
    if type(monName) ~= "string" then
        LogE("查找最近怪物之参数必须是怪物名字")
        return
    end
    RunInfoP(monName)
    return scmd.FindNearestMonsterObj(monName)
end

--创建组合过滤
function 创建组合过滤(tbl)
    if type(tbl) ~= "table" then
        LogE("创建组合过滤之菜蔬必须是表类型")
        return
    end
    RunInfoP()
    return scmd.CreateGroupFilter(tbl)
end

--创建过滤
function 创建过滤(filterType,filterTab)
    if type(filterType) ~= "number" then
        LogE("设置存仓过滤之过滤类型必须是数字")
        return;
    end
    RunInfoP(filterType)
    return scmd.CreateFilterObj(filterType,filterTab)
end

--第二个参数是个tab表
function 添加拾取过滤(filterTab)
    RunInfoP()
    return scmd.AddPickUpFilter(filterTab)
end

function 添加存仓过滤(filter)
    RunInfoP()
    return scmd.AddStoreFilter(filter)
end

function 添加出售过滤(filter)
    RunInfoP()
    return scmd.AddSellFilter(filter)
end

--查找背包物品
function 查找背包物品(itemName)
    if type(itemName) ~= "string" then
        LogE("查找背包物品之参数必须是字符串")
        return nil
    end
    RunInfoP(itemName)
    return scmd.FindPakcetItem(itemName)
end
--获得背包某个物品数量 参数:物品名字
function 获得物品数量(itemName)
    if type(itemName) ~= "string" then
        LogE("获得物品数量之参数必须是物品名")
        return nil
    end
    RunInfoP(itemName)
    return scmd.GetItemCnt(itemName)
end

--参数:地图名字 （限当前难度等级）
function 是否开启传送点(mapName)
    if type(mapName) ~= "string" then
        LogE("是否开启传送点之参数必须是地图名字")
        return false;
    end
    RunInfoP(mapName)
    return scmd.IsOpenWayPointByMap(mapName)
end

--参数1：保留格子宽度数量 参数2:保留格子高度数量
function 设置背包保留大小(wight,high)
    if type(wight) ~= "number" then
        LogE("设置背包保留大小之宽度必须是小于背包宽度的值")
        return false;
    end
    if type(high) ~= "number" then
        LogE("设置背包保留大小之高度必须是小于背包高度的值")
        return false
    end
    RunInfoP(wight,high)
    return scmd.SetMainBagMinFree(wight,high)
end

--参数1：主宝石 参数2：与主宝石连接的宝石(辅助宝石也可以是其他宝石)
function 设置宝石连接(mainGemName,otherGemNameTbl)
    if type(mainGemName)  ~= "string" then
        LogE("设置宝石连接之第一个参数必须是宝石名")
        return false;
    end
    if type(otherGemNameTbl)  ~= "table" then
        LogE("设置宝石连接之第二个参数必须是宝石名表")
        return false
    end
    RunInfoP(mainGemName)
    return scmd.SetGemLink(mainGemName,otherGemNameTbl)
end

function 清理宝石连接()
    RunInfoP()
    return scmd.ClearAllGemLink()
end

--参数:武器类型.单手 武器类型.双手 武器类型.限定位置的 
function 设置使用武器类型(weaponType)
    if type(weaponType) ~= "number" then
        LogE("设置使用武器类型之参数不对")
        return false;
    end
    RunInfoP(weaponType)
    return scmd.SetWearWeaponType(weaponType)
end

--参数:任务名字
function 查找任务(questName)
    if type(questName) ~= "string" then
        LogE("查找任务之任务名字必须是字符串")
        return false
    end
    RunInfoP(questName)
    return scmd.HasTheQuestByName(questName)
end

function 在城镇()
    RunInfoP()
    return scmd.InTown()
end

function 自动存仓()
    RunInfoP()
    return scmd.AutoStoreBagItemToStore()
end

function 自动出售()
    RunInfoP()
    return scmd.AutoSellBagItem()
end

--参数1:NPC名字  参数2:购买的物品名字
function 购买物品(npcName,itemName)
    if type(npcName) ~= "string" then
        LogE("自动购买物品之npc名字必须是字符串")
        return false
    end
    if type(itemName) ~= "string" then
        LogE("自动购买物品之物品必须是字符串")
        return false
    end
    RunInfoP(itemName)
    return scmd.BuyItemByNpc(npcName,itemName)
end

function 拾取最近任务物品()
    RunInfoP()
    return scmd.PickUpQuestItem()
end

--参数:职业名字 ("女巫"...等)
function 设置默认创建角色的职业(professionName)
    RunInfoP(professionName)
    return scmd.SetDefCreateProfessionType(professionName)
end

--参数：大区名字
function 设置默认创建角色的区(gameArea)
    RunInfoP(gameArea)
    return scmd.SetDefCreateGameArea(gameArea)
end

--参数：角色名字(选择进入游戏的角色名字)
function 选择进入游戏(roleName)
    RunInfoP(roleName)
    return scmd.SelectEnterGame(roleName)
end

function 选择角色场景(fn)
    --RunInfoP()
    return scmd.GScene_PreGameState(fn)
end

function 游戏内场景(fn)
    --RunInfoP()
    return scmd.GScene_InGameState(fn)
end

--设置自动换装
function 自动换装(enable)
    RunInfoP(enable)
    return scmd.EnableAutoWearEquip(enable)
end

--设置换装评分
function 设置换装评分(scoreType,tab)
    if type(scoreType) ~= "number" then
        LogE("设置换装评分之第一个参数必须是设定类型")
        return 
    end
    if type(tab) ~=  "table" then
        LogE("设置换装评分之第二个参数必须是表")
        return
    end
    RunInfoP(scoreType)
    return scmd.SetItemScore(scoreType,tab)
end

function 清空物品属性解析()
    RunInfoP()
    return scmd.ClearItemAttrParser()
end

function 物品属性解析(fn)
    RunInfoP()
    return scmd.CreateItemAttrParser(fn)
end

function 当前难度()
    return scmd.GetCurDiffLvl()
end

function 设置背包药品数量(tbl)
    if type(tbl) ~= "table" then
        LogE("设置药品数量之参数必须是表类型！请参看本函数")
        return
    end
    RunInfoP()
    return scmd.SetFlaskCnt(tbl)
end

--第一个参数 提取的物品类型，第二个参数提取的数量()
function 仓库提取异界地图(itemCnt)
    RunInfoP()
    return scmd.TakeOutYiJieMapFromStorage(itemCnt)
end

function 开启地图装置() 
    RunInfoP()
    return scmd.OpenMapDevice()
end

function 寻找异界传送门()
    RunInfoP()
    return scmd.FindYiJieTransitonable()
end

function 根据类型获得物品数量(type)
    RunInfoP(type)
    return scmd.GetItemCntByType(type)
end

function 当前地图名字()
    RunInfoP()
    return scmd.GetCurMapName()
end

function 传送卷轴回城()
    RunInfoP()
    return scmd.UseWayToTown()
end

function 设置地图过滤(type,tbl)
    RunInfoP(type)
    return scmd.SetYiJieFilter(type,tbl);
end

function 设置套装数量(cnt)
    if type(cnt) ~= "number" then
        LogE("设置套装数量之参数必须是整数")
        return
    end
    RunInfoP(cnt)
    return scmd.SetEmboitementCnt(cnt)
end

--检测仓库套装并提取到背包(提取成功返回真 反之返回假)
function 检测仓库套装并提取()
    RunInfoP()
    scmd.CheckStoreEmboitementToBag()
end