
require("mycmd")
require("中文角色名支持")
s_cmncfg = s_cmncfg

-----------------------------
AddMapRoomRate("河道", 1000)
AddMapRoomRate("水道遗迹", 1100)
AddMapRoomRate("上层神权之塔", 1100)
AddMapRoomRate("马雷格罗的藏身处", 1100)
AddMapRoomRate("堤道", 1300)
AddMapRoomRate("育灵之室", 1300)
AddMapRoomRate("巨兽沼泽 1 层", 1300)
AddMapRoomRate("巨兽沼泽 2 层", 1300)
AddMapRoomRate("薛朗之塔", 1200)

-----------------------------
SetMapAreaInfo("禁灵之狱上层", { "监狱宿舍", "禁灵之狱上层", "典狱长的房间" })
SetMapAreaInfo("怨忿之窟深处", { "莫薇儿的巢穴","楼梯","通道"})
SetMapAreaInfo("大竞技场", { "竞技场", "通道", "剑刃之环" })
SetMapAreaInfo("古金字塔", { "楼梯", "金字塔顶端" })
SetMapAreaInfo("月影神殿 2 层", {"传送门"})
SetMapAreaInfo("育灵之室", {"黑灵之核","黑灵之心"})
SetMapAreaInfo("神权之塔", {"楼梯"})
SetMapAreaInfo("上层神权之塔", {"楼梯","神权之塔(顶层)"})
SetMapAreaInfo("织网者巢穴", {"织网者巢穴"})
SetMapAreaInfo("冈姆的堡垒", {"熔岩之环","冈姆的堡垒"})
--
SetMapAreaInfo("纯净圣殿", {"纯净圣地"})
--
SetMapAreaInfo("大教堂顶楼", {"教堂之尖"})
--
SetMapAreaInfo("卡鲁要塞", {"卡鲁要塞"})
--
SetMapAreaInfo("薛朗之塔", {"楼梯","禁灵之狱(顶层)"}) --"典狱长的房间"
--
SetMapAreaInfo("监狱大门", {"引火者山谷","割裂者之径","噬魂者山谷"})
--
SetMapAreaInfo("湿地", {"典狱长的房间"})
--
SetMapAreaInfo("惊海之王的海礁", {"海王皇座"})
--
SetMapAreaInfo("寂静陵墓", {"楼梯"})
--
SetMapAreaInfo("马雷格罗的藏身处", {"马雷格罗的工作间"})
--
SetMapAreaInfo("灰原", {"旧森林营地"})
--
SetMapAreaInfo("坠欲之殿 1 层", {"楼梯"})
--  
SetMapAreaInfo("坠欲之殿 2 层", {"楼梯","阿拉卡力的巨网"})
--  
SetMapAreaInfo("奇迹之墙", {"楼梯"})
--  
SetMapAreaInfo("港湾大桥", {"天空神殿"})
--  
SetMapAreaInfo("月影神殿 2 层", {"传送门"})
--  
SetMapAreaInfo("日耀神殿 2 层", {"传送门"})
--  
SetMapAreaInfo("斜坡", {"抬升通道"})
--  
SetMapAreaInfo("精炼厂", {"竞技场"})
--  
SetMapAreaInfo("圣堂教团实验室", {"楼梯"})
--  
SetMapAreaInfo("德瑞的污水坑", {"巨型锅炉"})
--  
SetMapAreaInfo("中转码头", {"竞技场"})
--
SetMapAreaInfo("贵族花园", {"恐惧之池"})
--SetMapAreaInfo("教堂顶楼", {"教堂之尖"})

-----------------------------

local tp = GetDefTerrainParser()
--传送点
tp:AddAny(enTerrainParserFlag.kTPF_ObjDbPath, "传送点", "Metadata/Effects/Environment/waypoint/new/")

--绝望岩滩
--->狮眼守望
tp:AddMap("绝望岩滩", "狮眼守望", "Art/Models/Terrain/Beach/areatransitions/southentrance/beachtown_south_entrance_c2r2.tgt")

--暮光海滩
--->炙热盐沼
tp:AddMap("暮光海滩", "炙热盐沼", "Art/Models/Terrain/Beach/areatransitions/toswamp/act6_area2_transition_v01_01_c3r1.tgt")
--NPC奈莎
tp:AddMap("暮光海滩", "奈莎", "Art/Models/Terrain/Beach/areatransitions/beachtowatercave_v2/Beach_to_watercave_v2_c3r5.tgt")

--炙热盐沼
--恶水池
tp:AddMap("炙热盐沼", "恶水池", "Art/Models/Terrain/Beach/areatransitions/swamp2/beach_toswamp2_v01_01_c6r4.tgt")
--海潮地穴
tp:AddMap("炙热盐沼", "海潮地穴", "Art/Models/Terrain/Beach/areatransitions/beachtowatercave_v2/Beach_to_watercave_v2_c3r2.tgt")
-->刻着雕纹的奇怪墙壁
tp:AddMap("炙热盐沼", "刻着雕纹的奇怪墙壁", "Art/Models/Terrain/Beach/areatransitions/beachtowateracave/Beach_to_watercave_c2r6.tgt")
-->奈莎
tp:AddMap("炙热盐沼", "奈莎", "Art/Models/Terrain/Beach/areatransitions/beachtowatercave_v2/Beach_to_watercave_v2_c3r5.tgt")
-->卡鲁要塞
tp:AddMap("炙热盐沼", "卡鲁要塞", "Art/Models/Terrain/Beach/areatransitions/swamp_radiantpool/beach_karuipools_v01_01_c6r3.tgt")
-->Fortress Gate
tp:AddMap("炙热盐沼", "The Karui Fortress", "Art/Models/Terrain/Beach/areatransitions/swamp_radiantpool/beach_karuipools_v01_01_c6r3.tgt")
-->Fortress Gate
tp:AddMap("炙热盐沼", "Fortress Gate", "Art/Models/Terrain/Beach/areatransitions/swamp_radiantpool/beach_karuipools_v01_01_c6r3.tgt")


--海潮地穴
-->炙热盐沼
tp:AddMap("海潮地穴", "炙热盐沼", "Art/Models/Terrain/CaveDungeon/cave_exit_v01_01_c2r2.tgt")
-->水声之渊
tp:AddMap("海潮地穴", "水声之渊", "Art/Models/Terrain/CaveDungeon/watery_depth_entrance_v01_01_c1r1.tgt")
-->沉寂海崖
tp:AddMap("海潮地穴", "沉寂海崖", "Art/Models/Terrain/CaveDungeon/caveup_exit_v01_01_c2r3.tgt")

--水声之渊
-->海潮地穴
tp:AddMap("水声之渊", "海潮地穴", "Art/Models/Terrain/CaveDungeon/caveup_exit_v01_01_c2r3.tgt")

--沉寂海崖
-->海潮地穴
tp:AddMap("沉寂海崖", "海潮地穴", "Art/Models/Terrain/Beach/CaveEntrances/beachcaveentrance_v01_01_c2r4.tgt")
-->碎岩山坡
tp:AddMap("沉寂海崖", "碎岩山坡", "Art/Models/Terrain/Beach/areatransitions/passageway/beach_passageway_v01_01_c3r3.tgt")

--碎岩山坡
-->沉寂海崖
tp:AddMap("碎岩山坡", "沉寂海崖", "Art/Models/Terrain/Beach/areatransitions/passageway/beach_passageway_v01_01_c3r3.tgt")
-->禁灵之狱下层
tp:AddMap("碎岩山坡", "禁灵之狱下层", "Art/Models/Terrain/Beach/Prison/beach_prisonback_c6r4.tgt")

--禁灵之狱下层
-->碎岩山坡
tp:AddMap("禁灵之狱下层", "碎岩山坡", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_wall_straight_hole_v01_01_c1r1.tgt")
-->禁灵之狱上层
tp:AddMap("禁灵之狱下层", "禁灵之狱上层", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_exit_up_v01_01_c1r1.tgt")
-->升华试炼门
tp:AddMap("禁灵之狱下层", "升华试炼", "Art/Models/Terrain/PrisonDungeon/prison_trial_door_v01_01_c1r1.tgt")
-->Trial of Ascendancy Plaque
--tp:AddMapArea("The Lower Prison", "Trial of Ascendancy Plaque", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_stairs_convex_v01_01.tgt", {{"Art/Models/Terrain/PrisonDungeon/dungeon_prison_floor_height_lower_v01_03.tgt", 47}})
-->把手
tp:AddMap("禁灵之狱下层", "把手", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_fence_crevice_thickwall_v01_01.tgt")
-->薛朗之塔
tp:AddMap("禁灵之狱下层", "薛朗之塔", "Art/Models/Terrain/PrisonDungeon/areatransitions/shavronne_prison_door_up_v01_01_c2r2.tgt")

--薛朗之塔
tp:AddMap("薛朗之塔", "复生者薛朗", "Art/Models/Terrain/PrisonDungeon/areatransitions/tower_spiral_stair_v01_01_c3r1.tgt")


--禁灵之狱上层  (有小地图)
-->禁灵之狱下层
tp:AddMap("禁灵之狱上层", "禁灵之狱下层", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_entrance_down_v01_01_c1r3.tgt")

--监狱大门
-->魅影船墓
tp:AddMap("监狱大门", "魅影船墓", "Art/Models/Terrain/Beach/areatransitions/passagewaytoshipgraveyard/shipgraveyard_passageway_v01_01_c4r3.tgt")
--托麦迪克之印
tp:AddMap("监狱大门", "托麦迪克之印", "Art/Models/Terrain/Beach/areatransitions/spikepassage/beach_passageblock_v01_01_c2r2.tgt")
--西部密林
tp:AddMap("监狱大门", "西部密林", "Art/Models/Terrain/Beach/areatransitions/spikepassage/beach_passageblock_v01_01_c5r3.tgt")

--魅影船墓
-->魅影船墓洞穴
tp:AddMap("魅影船墓", "魅影船墓洞穴e", "Art/Models/Terrain/Beach/areatransitions/shipgraveyard_dungeon/ship_entrance_v01_01_c3r5.tgt")
-->怨忿之窟
--tp:AddMap("魅影船墓", "怨忿之窟", "Art/Models/Terrain/Beach/areatransitions/beachtoendcave/beach_caveentranceskeleton_v01_01_c7r6.tgt")
--费尔船长
tp:AddMap("魅影船墓", "费尔船长", "Art/Models/Terrain/Beach/QuestLocations/shipwreck_quest_v01_01_c7r8.tgt")

--怨忿之窟
-->怨忿之窟深处
--tp:AddMap("怨忿之窟", "怨忿之窟深处", "Art/Models/Terrain/CaveDungeon/caveup_exit_v01_01_c2r3.tgt")

--南部森林
-->森林营地iiiiiiiiiiiiiiiiii
tp:AddMap("南部森林", "森林营地", "Art/Models/Terrain/ForestTown/forestcamp_dock_v01_01_c2r4.tgt")
--怨忿之窟深处
tp:AddMap("南部森林", "怨忿之窟深处", "Art/Models/Terrain/Forest/areatransitions/forest_caveentrance_v01_01_c2r4.tgt")

--河道
-->湿地
tp:AddMap("河道", "湿地", "Art/Models/Terrain/Forest/areatransitions/bridgeconnection_v01_01_c2r2.tgt")
-->南部森林
tp:AddMap("河道", "南部森林", "Art/Models/Terrain/ForestTown/forest_to_river_v01_01_c5r2.tgt")


--前哨原野
-->森林营地
tp:AddMap("前哨原野", "森林营地t", "Art/Models/Terrain/ForestTown/forestcamp_right_v01_01_c3r3.tgt")
-->兽穴
tp:AddMap("前哨原野", "兽穴", "Art/Models/Terrain/ForestCave/forestcave_entrance_hole_v01_01_c1r1.tgt")
-->危机叉路
tp:AddMap("前哨原野", "危机叉路", "Art/Models/Terrain/Forest/areatransitions/wall_gate_v01_01_c2r2.tgt")

--危机叉路
-->前哨原野
tp:AddMap("危机叉路", "前哨原野", "Art/Models/Terrain/Forest/areatransitions/wall_gate_v01_01_c2r2.tgt")
-->罪孽之殿 1 层
tp:AddMap("危机叉路", "罪孽之殿 1 层", "Art/Models/Terrain/Forest/areatransitions/temple_entrance_v01_01_c2r5.tgt")
-->堕道遗迹 
tp:AddMap("危机叉路", "堕道遗迹", "Art/Models/Terrain/Forest/areatransitions/wall_gate_v01_01_c2r2.tgt")
-->河畔断桥
tp:AddMap("危机叉路", "河畔断桥", "Art/Models/Terrain/Forest/areatransitions/bridgeconnection_v01_01_c2r2.tgt")

--罪孽之殿 1 层
-->危机叉路
tp:AddMap("罪孽之殿 1 层", "危机叉路", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_entrance_v01_01_c1r1.tgt")
-->罪孽之殿:第 2 层
tp:AddMap("罪孽之殿 1 层", "罪孽之殿 2 层", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_exit_down_v01_01_c2r3.tgt")
-->地图装置
tp:AddMap("罪孽之殿 1 层", "地图装置", "Art/Models/Terrain/Doodads/TempleDungeon/temple_maporrery4_c1r2.tgt")
-->马雷格罗的藏身处
tp:AddMap("罪孽之殿 1 层", "马雷格罗的藏身处", "Art/Models/Terrain/Doodads/TempleDungeon/temple_maporrery4_c1r2.tgt")


--罪孽之殿:第 2 层
-->罪孽之殿:第 1 层
tp:AddMap("罪孽之殿 2 层", "罪孽之殿 1 层", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_exit_up_v01_01_c1r2.tgt")
-->Trial of Ascendancy
tp:AddMap("罪孽之殿:第 2 层", "Trial of Ascendancy", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_labyrinth_door_thin_corridor_v01_03_c1r1.tgt")
--Trial of Ascendancy Plaque
tp:AddMapArea("罪孽之殿:第 2 层", "Trial of Ascendancy Plaque", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_stairs_straight_v02_02.tgt", {{"Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_crevice_corner_concave_v01_01.tgt", 53}})
-->秘密通道
tp:AddMap("罪孽之殿 2 层", "秘密通道", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_maligaro_passage_c1r1.tgt")
-->兽穴
tp:AddMap("罪孽之殿 2 层", "兽穴", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_maligaro_passage_c1r1.tgt")

--兽穴
tp:AddMap("兽穴", "灰原", "Art/Models/Terrain/ForestCave/forestcaveup_exit_v01_01_c2r3.tgt")

--灰原
tp:AddMap("灰原", "旧森林营地", "Art/Models/Terrain/ForestTown/oldfields_campboss_v01_01_c8r5.tgt")

--堕道遗迹
-->危机叉路
tp:AddMap("堕道遗迹", "危机叉路", "Art/Models/Terrain/Forest/areatransitions/wall_gate_v01_01_c2r2.tgt")
-->寂静陵墓 1 楼
tp:AddMap("堕道遗迹", "寂静陵墓 1 楼", "Art/Models/Terrain/Forest/ChurchRuin/church_dungeon_entrance_v01_01_c2r2.tgt")
-->寂静陵墓
tp:AddMap("堕道遗迹", "寂静陵墓", "Art/Models/Terrain/Forest/ChurchRuin/church_dungeon_entrance_v01_01_c2r2.tgt")

--寂静陵墓 1 楼
-->寂静陵墓 2 楼
tp:AddMap("寂静陵墓 1 楼", "寂静陵墓 2 楼", "Art/Models/Terrain/ChurchDungeon/dungeon_church_exit_down_v01_01_c1r1.tgt")
-->Trial of Ascendancy
tp:AddMap("寂静陵墓 1 楼", "Trial of Ascendancy", "Art/Models/Terrain/ChurchDungeon/labyrinth_church_door_v01_01_c2r1.tgt")
-->Lever
tp:AddMap("寂静陵墓 1 楼", "Lever", "Art/Models/Terrain/ChurchDungeon/dungeon_church_wall_corner_convex_crevice_v01_01.tgt")
-->Trial of Ascendancy Plaque
tp:AddMap("寂静陵墓 1 楼", "Trial of Ascendancy Plaque", "Art/Models/Terrain/ChurchDungeon/dungeon_church_stairs_straight_feature_v01_01.tgt")


--寂静陵墓 2 楼
-->寂静陵墓 1 楼
tp:AddMap("寂静陵墓 2 楼", "寂静陵墓 1 楼", "Art/Models/Terrain/ChurchDungeon/dungeon_church_exit_up_v01_01_c1r1.tgt")
--祭坛
tp:AddMap("寂静陵墓 2 楼", "黄金之手", "Art/Models/Terrain/ChurchDungeon/dungeon_church_relic_altar_v01_01_c1r2.tgt")

--河畔断桥
-->危机叉路
tp:AddMap("河畔断桥", "危机叉路", "Art/Models/Terrain/Forest/areatransitions/bridgeconnection_v01_01_c2r2.tgt")

--西部密林
-->河道
tp:AddMap("西部密林", "河道", "Art/Models/Terrain/Forest/areatransitions/roadtothickforest_entrance_v01_01_c6r2.tgt")
-->织网者巢穴
tp:AddMap("西部密林", "织网者巢穴", "Art/Models/Terrain/Forest/areatransitions/spidergrove_entrance_v01_01_c4r2.tgt")
-->阿莉亚
tp:AddMap("西部密林", "阿莉亚", "Art/Models/Terrain/Forest/Trees/treewitch_camp_v01_02_c7r6.tgt")

--瓦尔废墟
-->湿地
tp:AddMap("瓦尔废墟", "湿地", "Art/Models/Terrain/IncaDungeon/dungeon_inca_exit_v01_01_c1r2.tgt")
-->北部密林(有待验证)
--tp:AddMap("瓦尔废墟", "北部密林", "Art/Models/Terrain/IncaDungeon/dungeon_bossrelease_v01_01_c2r1.tgt")

--北部密林
-->瓦尔废墟
--tp:AddMap("北部密林", "瓦尔废墟", "Art/Models/Terrain/Forest/areatransitions/forest_caveexit_inca_v01_01_c2r5.tgt")
--惊魂树洞
tp:AddMap("北部密林", "惊魂树洞", "Art/Models/Terrain/Forest/areatransitions/grovewall_entrance_v01_01_c1r2.tgt")
--瀑布洞穴
tp:AddMap("北部密林", "瀑布洞穴", "Art/Models/Terrain/Forest/areatransitions/waterfall_cave_entrance_v01_01_c2r4.tgt")
--堤道
tp:AddMap("北部密林", "堤道", "Art/Models/Terrain/Forest/areatransitions/city/forestriver_plinthtransition_v01_01_c6r2.tgt")

--湿地
-->瓦尔废墟
tp:AddMap("湿地", "瓦尔废墟", "Art/Models/Terrain/Forest/areatransitions/forest_caveentrance_inca_v01_01_c2r5.tgt")
-->河道
tp:AddMap("湿地", "河道", "Art/Models/Terrain/Forest/areatransitions/bridgeconnection_v01_01_c2r2.tgt")
-->欧克 
tp:AddMap("湿地", "欧克", "Art/Models/Terrain/Forest/OaksCamp/cliffpathconnection_gate_v01_01_c1r1.tgt")
--树根
tp:AddMap("湿地", "树根", "Art/Models/Terrain/Forest/areatransitions/forest_caveentrance_inca_v01_01_c2r5.tgt")


--惊魂树洞
-->北部密林
tp:AddMap("惊魂树洞", "北部密林", "Art/Models/Terrain/Grove/grove_wall_exit_v01_01_c2r2.tgt")

--瀑布洞穴
-->北部密林
tp:AddMap("瀑布洞穴", "北部密林", "Art/Models/Terrain/ForestCave/forestcavedown_exit_v01_01_c2r3.tgt")
-->古金字塔
tp:AddMap("瀑布洞穴", "古金字塔", "Art/Models/Terrain/IncaDungeon/dungeon_stairs_up_v01_01_c1r2.tgt")

--古金字塔
-->瀑布洞穴
tp:AddMap("古金字塔", "瀑布洞穴", "Art/Models/Terrain/IncaDungeon/dungeon_stairs_down_v01_01_c1r2.tgt")
-->萨恩城废墟
tp:AddMap("古金字塔", "萨恩城废墟", "Art/Models/Terrain/IncaDungeon/dungeon_huangdoor_v01_01_c3r2.tgt")

--萨恩城废墟
-->萨恩营地
tp:AddMap("萨恩城废墟", "萨恩营地", "Art/Models/Terrain/Act3/AreaTransitions/act3_docks_to_town_lower_01_01_c3r3.tgt")

--萨恩营地
-->萨恩城废墟 
tp:AddMap("萨恩营地", "萨恩城废墟 ", "Art/Models/Terrain/Act3Town/Act3_town_01_01_c12r21.tgt")
-->贫民窟
tp:AddMap("萨恩营地", "贫民窟", "Art/Models/Terrain/Act3Town/Act3_town_01_01_c21r7.tgt")
-->The Lord's Labyrinth
tp:AddMap("萨恩营地", "The Lord's Labyrinth", "Art/Models/Terrain/Act3Town/Act3_town_01_01_c8r8.tgt")

--The Crypt
tp:AddMap("The Crypt", "Trial of Ascendancy Plaque", "Art/Models/Terrain/ChurchDungeon/dungeonchurch_fence_crevice_wallthick_v02_01.tgt")
--Trial of Ascendancy
tp:AddMap("The Crypt", "Trial of Ascendancy", "Art/Models/Terrain/ChurchDungeon/labyrinth_church_door_v01_01_c1r1.tgt")


--贫民窟
-->萨恩营地
tp:AddMap("贫民窟", "萨恩营地", "Art/Models/Terrain/Act3/AreaTransitions/act3_to_slums_area_transition_c5r4.tgt")
-->下水道
tp:AddMap("贫民窟", "下水道", "Art/Models/Terrain/Act3/AreaTransitions/slum_sewer_entrance_v02_01.tgt")
-->火葬场
tp:AddMap("贫民窟", "火葬场", "Art/Models/Terrain/Act3/AreaTransitions/act3_prison_entrance_01_01_c2r3.tgt")

--火葬场
-->贫民窟
tp:AddMap("火葬场", "贫民窟", "Art/Models/Terrain/PrisonDungeon/dungeon_prison_exit_up_v01_01_c1r1.tgt")
-->托尔曼(目标)
--tp:AddMap("火葬场", "托尔曼", "Art/Models/Terrain/quest_marker.tgt")
-->Trial of Ascendancy
tp:AddMap("火葬场", "Trial of Ascendancy", "Art/Models/Terrain/PrisonDungeon/prison_trial_door_v01_01_c2r1.tgt")
-->Trial of Ascendancy Plaque
tp:AddMap("火葬场", "Trial of Ascendancy Plaque", "Art/Models/Terrain/PrisonDungeon/dugeon_prison_lava_fence_concave_v01_01.tgt")

--下水道
-->贫民窟
tp:AddMap("下水道", "贫民窟", "Art/Models/Terrain/Act3/Sewers/sewerwall_exit_v01_01_c1r1.tgt")
-->市集地带
tp:AddMap("下水道", "市集地带", "Art/Models/Terrain/Act3/Sewers/sewerwall_exit_v01_01_c1r1.tgt")
-->乌旗守卫兵营u
tp:AddMap("下水道", "乌旗守卫兵营", "Art/Models/Terrain/Act3/AreaTransitions/sewerexit_v01_01_c3r1.tgt")

--市集地带
-->仓库地带
tp:AddMap("市集地带", "仓库地带", "Art/Models/Terrain/Act3/Sewers/sewerwall_exit_v01_01_c1r1.tgt")
-->黑石陵墓()
tp:AddMap("市集地带", "黑石陵墓", "Art/Models/Terrain/Act3/AreaTransitions/markettochurchdungeon_v01_01_c3r2.tgt")
-->下水道
tp:AddMap("市集地带", "下水道", "Art/Models/Terrain/Act3/AreaTransitions/slum_sewer_entrance_v02_01.tgt")
-->激战广场()
tp:AddMap("市集地带", "激战广场", "Art/Models/Terrain/Act3/AreaTransitions/market_to_battlefront_v01_01_c5r6.tgt")

--黑石陵墓
-->市集地带
tp:AddMap("黑石陵墓", "市集地带", "Art/Models/Terrain/ChurchDungeon/dungeon_church_exit_up_v01_01_c1r1.tgt")
-->Trial of Ascendancy
tp:AddMap("黑石陵墓", "Trial of Ascendancy", "Art/Models/Terrain/ChurchDungeon/labyrinth_church_door_v01_01_c2r1.tgt")
-->Trial of Ascendancy
tp:AddMap("黑石陵墓", "Trial of Ascendancy Plaque", "Art/Models/Terrain/ChurchDungeon/dungeon_church_floor_height_convex_stairs_v01_02.tgt")

--激战广场
-->市集地带
tp:AddMap("激战广场", "市集地带", "Art/Models/Terrain/Act3/AreaTransitions/market_to_battlefront_v01_01_c5r6.tgt")
-->日耀神殿：第 1 层(The Solaris Temple Level 1)
tp:AddMap("激战广场", "日耀神殿 1 层", "Art/Models/Terrain/Act3/IvoryTemple/act3_temple_entrance_v01_01_c3r2.tgt")
-->不朽海港()
tp:AddMap("激战广场", "不朽海港", "Art/Models/Terrain/Act3/Battlefield/battlefield_arch_v01_03_c2r4.tgt")
-->The Harbour Bridge
tp:AddMap("激战广场", "The Harbour Bridge", "Art/Models/Terrain/Act3/AreaTransitions/battlefronttobarracks/act3_battlefront_to_harbor_v01_01_c2r3.tgt")

--日耀神殿：第 1 层
-->激战广场
tp:AddMap("日耀神殿 1 层", "激战广场", "Art/Models/Terrain/Act3/AreaTransitions/temple_to_battlefront_v01_01_c3r3.tgt")
-->日耀神殿：第 2 层
tp:AddMap("日耀神殿 1 层", "日耀神殿 2 层", "Art/Models/Terrain/TempleDungeon/templeclean_exit_down_v01_01_c1r3.tgt")
---->The Battlefront(门The Imperial Fields一样)
--tp:AddMap("The Solaris Temple Level 1", "The Battlefront", "Art/Models/Terrain/Act3/AreaTransitions/temple_to_battlefront_v01_01_c3r3.tgt")

--不朽海港()
-->激战广场
tp:AddMap("不朽海港", "激战广场", "Art/Models/Terrain/Act3/Battlefield/battlefield_arch_v01_02_c2r4.tgt")
--装硫磺的箱子
tp:AddMap("不朽海港", "托麦迪克亚硫酸", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_questcart.tgt")
--费尔船长
tp:AddMap("不朽海港", "费尔船长", "Art/Models/Terrain/Act3/citywaypoint_v01_01_c2r1.tgt")

--日耀神殿：第 2 层
-->日耀神殿：第 1 层
tp:AddMap("日耀神殿 2 层", "日耀神殿 1 层", "Art/Models/Terrain/TempleDungeon/templeclean_exit_up_v01_02_c3r1.tgt")
-->达拉夫人
tp:AddMap("日耀神殿 2 层", "达拉夫人", "Art/Models/Terrain/TempleDungeon/gemling_queen_throne_v01_01_c2r3.tgt")
-->The Sun Orb (物品位置)
--tp:AddMap("日耀神殿 2 层", "Sun Orb", "Art/Models/Terrain/TempleDungeon/gemling_queen_throne_v01_01_c3r3.tgt")
-->传送门
tp:AddMap("日耀神殿 2 层", "传送门", "Art/Models/Terrain/TempleDungeon/gemling_queen_throne_v01_01_c2r3.tgt")

--乌旗守卫兵营
-->下水道
tp:AddMap("乌旗守卫兵营", "下水道", "Art/Models/Terrain/Act3/AreaTransitions/battlefield_to_sewer_stairs_v01_01_c3r2.tgt")
-->月影神殿 1 层
tp:AddMap("乌旗守卫兵营", "月影神殿 1 层", "Art/Models/Terrain/Act3/IvoryTemple/act3_temple_entrance_v01_01_c3r2.tgt")
-->皇家花园
tp:AddMap("乌旗守卫兵营", "皇家花园", "Art/Models/Terrain/Act3/Gardens/garden_arch_v01_01_c2r5.tgt")
-->The Harbour Bridge
tp:AddMap("乌旗守卫兵营", "The Harbour Bridger", "Art/Models/Terrain/Act3/AreaTransitions/battlefronttobarracks/act3_riverbridge_transition_v01_01_c9r7.tgt")

-->月影神殿 1 层
tp:AddMap("月影神殿 1 层", "月影神殿 2 层", "Art/Models/Terrain/TempleDungeon/templeclean_exit_down_v01_01_c1r3.tgt")

--皇家花园
-->乌旗守卫兵营
tp:AddMap("皇家花园", "乌旗守卫兵营", "Art/Models/Terrain/Act3/Gardens/garden_arch_v01_01_c2r5.tgt")
-->树篱迷宫(The Hedge Maze)
tp:AddMap("皇家花园", "树篱迷宫", "Art/Models/Terrain/Act3/Gardens/walls/garden_wall_entrance_v01_01_c2r2.tgt")
-->图书馆
tp:AddMap("皇家花园", "图书馆", "Art/Models/Terrain/Act3/Library/Library_LargeBuilding_entrance_v01_01_c2r3.tgt")
-->神权之塔
tp:AddMap("皇家花园", "神权之塔", "Art/Models/Terrain/Act3/AreaTransitions/Act3_EpicDoor_v02_01_c7r7.tgt")
-->上锁的门
tp:AddMap("皇家花园", "上锁的门", "Art/Models/Terrain/Act3/AreaTransitions/Act3_EpicDoor_v02_01_c7r7.tgt")

--树篱迷宫
-->皇家花园
tp:AddMap("树篱迷宫", "皇家花园", "Art/Models/Terrain/Act3/Gardens/walls/garden_wall_entrance_v01_01_c2r2.tgt")
-->梅果
--tp:AddMap("树篱迷宫", "梅果", "Art/Models/Terrain/Act3/Gardens/fruittree_c2r2.tgt")
-->Trial of Ascendancy
tp:AddMap("The Hedge Maze", "Trial of Ascendancy", "Art/Models/Terrain/Act3/Gardens/walls/garden_labyrinth_gate_v02_02_c1r2.tgt")
--Trial of Ascendancy Plaque(试炼很小！ 一下就能遍历到)

--图书馆
-->皇家花园
tp:AddMap("图书馆", "皇家花园", "Art/Models/Terrain/Act3/Library/library_entrance_v01_01_c2r3.tgt")
-->档案室
tp:AddMap("图书馆", "档案室", "Art/Models/Terrain/Act3/Library/library_entrance_v02_01_c2r3.tgt")

--档案室
-->图书馆
tp:AddMap("档案室", "图书馆", "Art/Models/Terrain/Act3/Library/library_entrance_v03_01_c2r3.tgt")

--水道遗迹
-->统治者之殿
tp:AddMap("水道遗迹", "统治者之殿", "Art/Models/Terrain/Act4/Town/AqueductConnection/mountiantown_connection_c7r13.tgt")

--漆黑矿坑:第 1 层
-->统治者之殿
tp:AddMap("漆黑矿坑:第 1 层", "统治者之殿", "Art/Models/Terrain/Act4/mine/mine_areatransition_entrance_v01_01_c3r4.tgt")
-->漆黑矿坑:第 2 层
tp:AddMap("漆黑矿坑:第 1 层", "漆黑矿坑:第 2 层", "Art/Models/Terrain/Act4/mine/mine_areatransition_v01_01_c1r2.tgt")

--漆黑矿坑：第 2 层
-->水晶矿脉
tp:AddMap("漆黑矿坑:第 2 层", "水晶矿脉", "Art/Models/Terrain/Act4/mine/mine_areatransition_v03_01_c1r2.tgt")
-->漆黑矿坑：第 1 层
tp:AddMap("漆黑矿坑:第 2 层", "漆黑矿坑:第 1 层", "Art/Models/Terrain/Act4/mine/mine_areatransition_v02_02_c1r2.tgt")

--水晶矿脉
-->漆黑矿坑：第 2 层
tp:AddMap("水晶矿脉", "漆黑矿坑:第 2 层", "Art/Models/Terrain/Act4/mine/mine_areatransition_v04_01_c1r2.tgt")
-->冈姆的幻境
tp:AddMap("水晶矿脉", "冈姆的幻境", "Art/Models/Terrain/Act4/mine/crystals_openAnimation_v01_01_c1r3.tgt")
-->德瑞索的幻境
tp:AddMap("水晶矿脉", "德瑞索的幻境", "Art/Models/Terrain/Act4/mine/crystals_openAnimation_v01_01_c5r3.tgt")
-->巨兽之腹：第 1 层
tp:AddMap("水晶矿脉", "巨兽之腹:第 1 层", "Art/Models/Terrain/Act4/mine/crystals_openAnimation_v01_01_c3r2.tgt")

--冈姆的幻境
-->水晶矿脉 -14
--tp:AddMap("冈姆的幻境", "The Crystal Veins", "Art/Models/Terrain/Act4/LavaPlateau/KaomTiles/lava_abyss_v02_01_c3r1.tgt")
-->冈姆的堡垒
tp:AddMap("冈姆的幻境", "冈姆的堡垒", "Art/Models/Terrain/Act4/LavaPlateau/AreaTransition/lava_abyss_transition_entrance_v01_01_c3r1.tgt")

--冈姆的堡垒
-->冈姆的幻境
tp:AddMap("冈姆的堡垒", "冈姆的幻境", "Art/Models/Terrain/Act4/LavaPlateau/AreaTransition/lava_kaom3_transition_entrance_v02_01_c2r1.tgt")
--熔岩之环
tp:AddMap("冈姆的堡垒", "熔岩之环", "Art/Models/Terrain/Act4/LavaPlateau/AreaTransition/lava_lake_throne_room_v01_01_c2r2.tgt")

--德瑞索的幻境
-->水晶矿脉 -22
--tp:AddMap("德瑞索的幻境", "The Crystal Veins", "Art/Models/Terrain/Act4/Arena/arena_wall_v02_02.tgt")
-->大竞技场
tp:AddMap("德瑞索的幻境", "大竞技场", "Art/Models/Terrain/Act4/Arena/AreaTransition/arena_areatransition_v01_01_c1r2.tgt")

-->北部密林
tp:AddMap("北部密林", "阿兹莫里的神殿", "Art/Models/Terrain/Forest/Feature/forest_azmeri_shrine_c2r1.tgt")

--育灵之室(The Harvest)
-->巨兽之腹：第 2 层
tp:AddMap("育灵之室", "巨兽之腹 2 层", "Art/Models/Terrain/Act4/Belly/Harvest_tunnel_v01_01_c2r3.tgt")
-->黑灵之核
tp:AddMap("育灵之室", "黑灵之核", "Art/Models/Terrain/Act4/Belly/Taster/TasterMouth_openAnimation_v01_01_c2r2.tgt")

--奥瑞亚之道
tp:AddMap("奥瑞亚之道", "传送开关", "Art/Models/Terrain/Act4/Ascent/Feature/ascent_summit_v01_01_c4r3.tgt")

--奴隶深坑
tp:AddMap("奴隶深坑", "狱卒之塔", "Art/Models/Terrain/Act5/Tower/tower_v01_01_c9r4.tgt")
--监工头克劳
tp:AddMap("奴隶深坑", "监工头克劳", "Art/Models/Terrain/Act5/Tower/tower_v01_01_c9r4.tgt")

--控制区
tp:AddMap("控制区", "奥瑞亚广场", "Art/Models/Terrain/Act5/Area Transitions/security_exit_v01_01_c1r5.tgt")
tp:AddMap("控制区", "审判长卡斯提克斯", "Art/Models/Terrain/Act5/SlavePen/slave_crater_v01_01_c2r1.tgt")
tp:AddMap("控制区", "奥瑞亚广场", "Art/Models/Terrain/Act5/Area Transitions/security_exit_v01_01_c1r5.tgt")

--奥瑞亚广场
tp:AddMap("奥瑞亚广场", "圣堂高庭", "Art/Models/Terrain/Act5/Area Transitions/Oriath_AreaTransition_v01_03_c2r2.tgt")

--圣堂高庭
tp:AddMap("圣堂高庭", "纯净圣殿", "Art/Models/Terrain/Act5/Templar Court/templar_to_innocents_v01_01_c2r1.tgt")

--纯净圣殿
-->纯净圣地(小房间)
tp:AddMap("纯净圣殿", "纯净圣地", "Art/Models/Terrain/Act5/Innocence/transition_chamber_to_boss_v01_01_c4r4.tgt")
-->化为火海的高庭
tp:AddMap("纯净圣殿", "化作火海的高庭", "Art/Models/Terrain/Act5/Innocence/transition_chamber_to_courts_v01_01_c3r4.tgt")

--化为火海的高庭
tp:AddMap("化作火海的高庭", "广场遗迹", "Art/Models/Terrain/Act5/Templar Court/templar_oriath_transition_v01_01_c2r1.tgt")

--藏古堂
tp:AddMap("藏古堂", "广场遗迹", "Art/Models/Terrain/Act5/Area Transitions/Ossuary_abyss_transition_v01_01_c1r2.tgt")

--广场遗迹
tp:AddMap("广场遗迹", "遗忘圣地", "Art/Models/Terrain/Act5/OriathSquare/Oriath_AreaTransition_v01_04_c2r2.tgt")
tp:AddMap("广场遗迹", "遗物圣所", "Art/Models/Terrain/Act5/OriathSquare/Oriath_AreaTransition_v01_02_c2r2.tgt")
tp:AddMap("广场遗迹", "大教堂顶楼", "Art/Models/Terrain/Act5/OriathSquare/ChitusTiles/chitus_statuewall_transition_v01_01_c1r3.tgt")

--教堂顶楼
tp:AddMap("教堂顶楼", "教堂之尖", "Art/Models/Terrain/Act5/CathedralRoof/boss/cathedralroof_boss_area_v02_01_c5r10.tgt")

--卡鲁要塞
tp:AddMap("卡鲁要塞", "寂默山岭", "Art/Models/Terrain/Beach/areatransitions/toridge/swamp_to_ridge_v01_01_c5r3.tgt")

--寂默山岭
tp:AddMap("寂默山岭", "禁灵之狱下层", "Art/Models/Terrain/PrisonDungeon/areatransitions/ledge_prisonback_c5r3.tgt")

--孤岛灯塔 --Art/Models/Terrain/Act3/Beacon/beacon_stairs_v01_01_c2r1.tgt
tp:AddMap("孤岛灯塔", "可移动柱子", "Art/Models/Terrain/Act3/Beacon/beacon_nostairs_v01_01_c5r6.tgt")
--tp:AddMap("孤岛灯塔", "可移动柱子", "Art/Models/Terrain/Act3/Battlefield/mosaic_doubleledge_connection_v01_02_c1r1.tgt")
tp:AddMap("孤岛灯塔", "威勒姆·罗斯", "Art/Models/Terrain/Act3/Beacon/beach_walkablelargecliffwater_weylamwalk_v01_01_c7r3.tgt")

--惊海之王的海礁
tp:AddMap("惊海之王的海礁", "海王皇座", "Art/Models/Terrain/Beach/areatransitions/beachisland/beach_island_transition_v01_02_c10r3.tgt")

-->堤道 --Art/Models/Terrain/Forest/areatransitions/city/vaal_stairs_bottom_v01_01_c4r7.tgt
tp:AddMap("堤道", "瓦尔古城", "Art/Models/Terrain/Forest/areatransitions/city/vaal_stairs_bottom_v01_01_c5r9.tgt")

-->瓦尔古城
tp:AddMap("瓦尔古城", "伊娜(技能杂货)", "Art/Models/Terrain/IncaDungeon/BanteaySrei_Web_c2r4.tgt")

-->瓦尔古城
tp:AddMap("坠欲之殿 2 层", "BOOS位置", "Art/Models/Terrain/IncaDungeon/Arakaali_Balcony_Arch/dungeon_web_arakaali_balcony_v01_03_c1r3.tgt")

-->月影广场
tp:AddMap("月影广场", "港湾大桥", "Art/Models/Terrain/Act3/AreaTransitions/battlefronttobarracks/act3_riverbridge_transition_v01_01_c9r7.tgt")

-->巨兽沼泽
tp:AddMap("巨兽沼泽", "育灵之室", "Art/Models/Terrain/Act3/AreaTransitions/battlefronttobarracks/act3_riverbridge_transition_v01_01_c9r7.tgt")

-->剧毒管道
tp:AddMap("剧毒管道", "德瑞的污水坑", "Art/Models/Terrain/Act3/Sewers/sewerwall_end_tunnel_v01_04_c1r2.tgt")

-->德瑞的污水坑
tp:AddMap("德瑞的污水坑", "巨型锅炉", "Art/Models/Terrain/Act3/AreaTransitions/slum_sewer_entrance_v03_01.tgt")
--月色回廊
tp:AddMap("德瑞的污水坑", "月色回廊", "Art/Models/Terrain/Act3/AreaTransitions/sewers/sewer_ladder_up_v01_01_c1r1.tgt")
--月色回廊
tp:AddMap("德瑞的污水坑", "中转码头", "Art/Models/Terrain/Act3/Sewers/sewerwall_exit_v01_01_c1r1.tgt")

--月色回廊
tp:AddMap("月色回廊", "古兵工厂", "Art/Models/Terrain/Act3/Battlefield/arch_promenade_to_arsenal_v01_01_c2r3.tgt")

----古兵工厂(有三个一样的门  不好定位)
--tp:AddMap("古兵工厂", "古兵工厂", "Art/Models/Terrain/Act3/Battlefield/arch_promenade_to_arsenal_v01_01_c2r3.tgt")

--月影广场
tp:AddMap("月影广场", "港湾大桥", "Art/Models/Terrain/Act3/AreaTransitions/battlefronttobarracks/act3_riverbridge_transition_v01_01_c9r7.tgt")

--月影神殿 1 层
tp:AddMap("月影广场", "月影神殿 1 层", "Art/Models/Terrain/TempleDungeon/lunariswater/act3_temple_entrance_v01_01_c3r2.tgt")

--月影神殿 2 层
tp:AddMapArea("月影神殿 2 层", "传送门", "Art/Models/Terrain/TempleDungeon/lunariswater/templeclean_prepiety_sidehigh_01_c1r2.tgt", {{"Art/Models/Terrain/TempleDungeon/lunariswater/templeclean_bloodpool_ledge_straight_v01_01.tgt", 84}})

--中转码头(这个不准确)
--tp:AddMap("中转码头", "稻穗之门", "Art/Models/Terrain/Act3/LargeCliff/Act3_Largecliff_v01_01_victario_c3r1.tgt")

--稻穗之门
tp:AddMap("稻穗之门", "帝国平原", "Art/Models/Terrain/Act3/AreaTransitions/act8_grain_gate_transition_v01_01_c8r5.tgt")

--帝国平原
tp:AddMap("帝国平原", "日耀神殿 1 层", "Art/Models/Terrain/Act8/act8_temple_entrance_v01_01_c3r2.tgt")

--帝国平原
tp:AddMap("帝国平原", "日耀神殿 1 层", "Art/Models/Terrain/Act8/act8_temple_entrance_v01_01_c3r2.tgt")

-----------------------------迷宫地图

-->Estate Path  
tp:AddMap("Estate Path", "Aspirant's Trial", "Art/Models/Terrain/Act3/ElegantHouse/eleganthouse_transition_v01_01_c1r1.tgt")

-->Basilica Halls(杀到这个门有机关不太好过！ 但是路径近,有其他路径可以通过)
--tp:AddMap("Basilica Halls", "Aspirant's Trial", "ArtArt/Models/Terrain/Act3/Gardens/walls/garden_labyrinth_gate_v02_02_c1r2.tgt")

--Basilica Hallsh
tp:AddMapArea("Basilica Hallsh", "Sanitorium Passage", "Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_exit_up_v01_01_c1r2.tgt", {{"Art/Models/Terrain/RuinedTempleForestDungeon/templeruinforest_wall_corner_concave_crevice_v01_01.tgt", 150}})

--Sepulchre Passage
tp:AddMapArea("Sepulchre Passage", "Aspirant's Trial", "Art/Models/Terrain/ChurchDungeon/dungeon_church_wall_straight_v01_02.tgt", {{"Art/Models/Terrain/ChurchDungeon/dungeon_church_wall_straight_v01_02.tgt", 25}})

--Domain Walkways
--Sepulchre Halls
tp:AddMapArea("Domain Walkways", "Sepulchre Halls", "Art/Models/Terrain/Doodads/labyrinth/Doorway/labyrinth_dry_garden_door_v01_01_c2r5.tgt", {{"Art/Models/Terrain/Blank.tgt", 96}})

--Sepulchre Halls
--Aspirant's Trial
tp:AddMapArea("Sepulchre Halls", "Aspirant's Trial", "Art/Models/Terrain/ChurchDungeon/dungeon_church_exit_up_v01_01_c1r1.tgt", {{"Art/Models/Terrain/ChurchDungeon/dungeon_church_wall_straight_v01_03.tgt", 96}})


