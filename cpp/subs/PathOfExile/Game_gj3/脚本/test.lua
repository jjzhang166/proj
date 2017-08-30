----[[
加载配置脚本("通用函数")
加载配置脚本("通用配置")
--]]
--对话NPC("Lilly Rot","Sail to Wraeclast")
--打开目标("The Black Core")
local c = scmd
local cdi = CD_Interface
local gpm = GPlayerMe()
定义函数 移动到目标打开()
    移动到目标()
结束
定义函数 杀到目标打开()

结束
定义函数 做_地图试炼ww()
    如果 当前地图名字() == "The Crypt" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        杀到打开目标("Trial of Ascendancy Plaque")
    否则如果 当前地图名字() == "The Lower Prison" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        杀到打开目标("Lever")
        杀到打开目标("Trial of Ascendancy Plaque")
    否则如果 当前地图名字() == "The Crypt Level 1" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        移动到目标()("Lever")
        杀到打开目标("Trial of Ascendancy Plaque")
    否则如果 当前地图名字() == "The Chamber of Sins Level 2" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        杀到打开目标("Trial of Ascendancy Plaque")
    否则如果 当前地图名字() == "The Crematorium" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        杀到打开目标("Lever") --自动打开该对象
        杀到打开目标("Trial of Ascendancy Plaque")
    否则如果 当前地图名字() == "The Catacombs" 那么
        杀到指定位置("Trial of Ascendancy")
        打开附近对象("Trial of Ascendancy")
        杀到打开目标("Lever") --自动打开该对象 就一个
        杀到打开目标("Trial of Ascendancy Plaque")
    结束
结束
--cdi.DebugTheDbWorldArea("Aspirant's Trial");
--做_地图试炼ww()
--cdi.DebugPlayerEntityInfo()
--cdi.DebugMapModels(30,true)
--杀到指定地图("Ascendancy Chamber")
--杀到指定位置("Trial of Ascendancy")
--杀到打开目标("Trial of Ascendancy")
--杀到指定位置("Lever")
--杀到打开目标("Aspirant's Trial")
--杀到打开目标("Trial of Ascendancy Plaque")
--全图杀怪()
--杀到目标("Nessa")
--开启地图装置()
--DbgTestPrint(1)
--cdi.DebugPlayerPosInfo()
--c.KillMoveTo(stCD_VecInt(2702, 494))
--LogE("------------------------------------------------------")
--cdi.DebugPlayerPosInfo()
----c.KillMoveTo("The Warehouse Sewerss")
--TestAutoDetectTerrainObjs({"Aspirant's Trial"})
--cdi.DebugUiFullInfo(0, 1, 2)
--DbgObjs(false,{0,1,2,3,4,5,6,7,8,10})  --传送门
--DbgObjs(false,{10}) --可传送对象
--DbgObjs(false,{0}) --npc
--DbgObjs(false,{1}) --怪物
--DbgObjs(false,{11}) --召唤物
--DbgObjs(false,{10}) --召唤物
--打开目标("The Causeway111")
--全图杀怪(true)
local start = G_AStarMe(); 
start:DebugPosInfo(true,"f://tmp//tmp1.log")
--start:DebugPosRectInfo("f:/tmp/tmp1.log", 100, false)
--start:DebugPosRectInfo("f:/tmp/tmp1.log", stCD_VecInt(1741, 1764), 50, false)
local path = GPathMarkMe()
--path:DebugPosInfo(true,"f://tmp//tmp1.log")
--自动穿戴装备()
--c.AutoWearSomeEquips()
--DbgGSP_Resolver(false)
--gpm:SmartMove(stCD_VecInt(1541, 483))
--gpm:SmartMove(stCD_VecInt(1264, 552))

--cdi.DebugDbInfo("Data/WorldAreas.dat")
--cdi.DebugDbWorldArea()

--杀到打开目标("The Upper Prison")
--杀到打开目标("The Warden's Quarters")
--杀到打开目标("The Cavern of Anger")
--cdi.DebugMySkillInfo()
--全图杀怪()
--cdi.DebugMySkillInfo()
--打开目标("Nessa")
--打开目标("Dying Exile")

--cdi.DebugEntityObjs(9)
--cdi.DebugEntityObjs(1)
--等待可移动(60 * 3)
--cdi.DebugUiFullInfo(0, 1, 2)
--cdi.DebugTheParentUiInfo(0x537C9A00)
--cdi.TestShowUi(0x4A845158)

--关闭所有面板()

--cdi.DebugWorldInfo()
--cdi.DebugEntityObjs(-1)
--cdi.DebugMapModels(50, false)

local tp = GetDefTerrainParser()

--TestAutoDetectTerrainObjs({ "The Western Forest" })
TestDetectUniqueTerrain(100, 1)
TestDetectOneTerrainArea(100, 2)
--[[
TestDetectUniqueTerrainByPos({stCD_VecInt(368, 759), stCD_VecInt(437, 736)
, stCD_VecInt(391, 713) 
, stCD_VecInt(368, 644), stCD_VecInt(414, 736), stCD_VecInt(414, 736)
})
--]]
--cdi.DbgPlayerPosModel()

--cdi.DebugInGameState()
--杀到打开目标("Nessa")
--杀到打开目标("The Upper Prison")
--杀到打开目标("The Warden's Quarters")
--杀到打开目标("The Warden's Chambers")
--杀到杀死目标("Brutus, Lord Incarcerator")
--杀到目标("Prisoner's Gate")


--cdi.DebugStateInfo()
--cdi.DebugUiFullInfo(0, 1, 2)
--cdi.DebugUiFullInfo(0x57B9B088, 3, 4)
--cdi.DebugUiFullInfo(, 1, 4)

--cdi.TestShowUi(0x538F8578)

--cdi.TestShowUi(0x5914CA98)

--cdi.TestShowUi(0x14CB49E8)

--帐号
--cdi.TestShowUi(0x15753B28)
--密码
--cdi.TestShowUi(0x15754A30)
--网关
--cdi.TestShowUi(0x15742920)
--登陆按钮
--cdi.TestShowUi(0x158922F8)

--TestLoginAccount("2181249973@qq.com", "poe123456", "Singapore")

--cdi.DebugLoginState()
--cdi.DebugPreGameState()
--自动登陆帐号()
--cdi.DebugSelectRoleTop()
--DbgSortedObjs(false, {4})
--自动创建选择角色()
--cdi.DebugPlayerEntityInfo()
--cdi.DebugPlayerPosInfo()
--TestActorUseSkill("Fireball", stCD_VecInt(322, 246))
--gpm:SmartMove(stCD_VecInt(428, 229))

--自动存仓()
--TestTerminateCurSkill()
--cdi.DebugWorldInfo()
--cdi.DebugWorldInfo()
--cdi.DebugCurQuestInfo()
--cdi.DebugUiQuestItem(3, 1, 1)
--cdi.DebugUiQuestStateItem()
--cdi.DebugPlayerInfo()
--cdi.DebugPlayerEntityInfo()
--cdi.DebugWorldInfo()
--cdi.DebugEntityObjs(1)
--cdi.DebugInventoryItems(1)
--cdi.DebugFindGridPanelFromDlgProp(1)
--获取奖励物品("Lightning Tendrils")
--自动穿戴装备()
--cdi.DebugDbWorldArea()
--购买物品("Nessa","Rustic Sash",1)
--cdi.DebugPassiveSkill()
--TestAutoLvlUpPassiveSkills(0)
--移动到指定地图("Prisoner's Gate", 1)
--cdi.DebugTheDbWorldArea("Lioneye's Watch")
--cdi.DebugTheDbWorldArea("Prisoner's Gate")