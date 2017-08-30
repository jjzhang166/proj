
局部变量 单次任务 = 单次处理任务
局部变量 任务 = 处理任务
局部变量 任务状态 = 处理任务状态
局部变量 接 = 接任务处理
局部变量 做 = 做任务处理
局部变量 提交 = 提交任务处理
局部变量 完成 = 提交任务处理
 
--Tarkleigh:塔格拉(武器交易)
--Nessa:奈莎(技能杂货)
--Bestel:毕斯特
--The Coast:暮光海滩
--Silk:斯克
--Yeena:伊娜
--Eramir: 艾米尔
--Greust:古斯特(武器交易)
--Yeena : 伊娜(技能杂货)
局部变量 所有的任务 = {
    [任务状态("初始任务")] = {
        做(做_初始任务),
    },

    任务("The Lord's Labyrinth",{
        [任务状态("Enter the Labyrinth")] = { 
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Trial of Ascendancy Plaque"}},"当前地图"}
        },
        [任务状态("Solve the Trials of Ascendancy")] = {    --The Crypt
             做(做_前往试炼)
        },
        [任务状态("Solve the Trial of Ascendancy in this Area")] = { 
             做(做_地图试炼)
        },
        [任务状态("Complete the Labyrinth")] = { 
             做(做_完成迷宫)
        },
        [任务状态("Travel to The Sarn Encampment")] = { 
             [做(通用任务_杀到任务地图操作)] = {{{}},"The Sarn Encampment"}
        },
        [任务状态("Enter the Labyrinth in the Sarn Encampment")] = { 
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Ascendancy Plaque"}},"The Sarn Encampment"}
        },
        [任务状态("Activate the Altar of Ascendancy")] = { 
              做(做_完成迷宫)
        },
    }),

    --大门口的敌人
    任务("Enemy at the Gate", {
        [任务状态("Kill Hillock")] = { --击败西拉克
             [做(通用任务_杀到任务地图操作)] = {{{"杀死","Hillock"}}}
        },
       [任务状态("Enter town for your reward")] = { --进入城镇领取奖励
            做(通用任务_杀到任务地图操作)
        },
       [提交(通用任务_提交任务)]= {"Tarkleigh","Hillock Reward",3},
    }),

    --医者之心
    任务("Mercy Mission", {
        [任务状态("Talk to Bestel")] = {    --对话毕斯特
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Bestel"}},空}
        },
        [任务状态("Travel to The Tidal Island")] = { -- 前往 海潮孤岛  The Tidal Island(海潮孤岛)
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","The Mud Flats"},{"打开","The Tidal Island"}},"The Coast"}  --1
        },
        [任务状态("Find the Medicine Chest")] = { --找到医药箱
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Medicine Chest"}},空}
        },
        [任务状态("Travel to Lioneye's Watch")] = { -- 前往 狮眼守望
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Nessa for your reward")] = { --跟奈莎交谈以获得奖励  Medicine Chest Reward(医药箱奖励)
            [做(通用任务_提交任务)]= {"Nessa","Medicine Chest Reward",3},
        },
        [提交(通用任务_提交任务)]= {"Nessa","Medicine Chest Reward",3},
    }),
    
    --打破鸟蛋
    任务("Breaking Some Eggs",{
        [任务状态("Travel to The Mud Flats")] = { --前往 炙热盐沼
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Mud Flats"}
        },
        [任务状态("Take the Glyphs to the Glyph Wall")] = { --拿着雕纹到石雕之壁去
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Strange Glyph Wall"}}} --刻着雕纹的奇怪墙壁
        },
        [任务状态("Explore the Mud Flats")] = { --探索炙热盐沼
            做(做_探索炙热盐沼)
        },
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Tarkleigh for your reward")] = { --跟塔格拉交谈来获得奖励  Glyph Reward(开启石雕之壁的奖励)
            [做(通用任务_提交任务)]= {"Tarkleigh","Glyph Reward",3},
        },
        [提交(通用任务_提交任务)]= {"Tarkleigh","Glyph Reward",3},
    }), 

    --腐水之息
    单次任务("A Dirty Job", {
        [任务状态("Travel to The Fetid Pool")] = { --前往 恶水池
            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","Breaking Some Eggs"}}} 
        },
        [任务状态("Kill all the monsters in the Fetid Pool")] = { --清空恶水池
             通用做任务_全图杀怪
        },
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
             做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Tarkleigh for your reward")] = { --与塔格拉交谈来获得奖励 Necromancer Reward(清空恶水池的奖励)
             [做(通用任务_提交任务)]= {"Tarkleigh","Necromancer Reward",1},
        },
    }),

    --黑渊危机
    任务("The Dweller of the Deep",{
        [任务状态("Travel to The Flooded Depths")] = { --前往 水声之渊
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Kill the Dweller of the Deep")] = { --击败深渊巨蟹
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","The Deep Dweller"}}}
        },
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Tarkleigh for your reward")] = { --与塔格拉交谈来获得奖励 Dweller Reward(击败深渊巨蟹的奖励)
            [做(通用任务_提交任务)]= {"Tarkleigh","Dweller Reward",1},
        },
    }),

    --冲出监牢
    单次任务("The Caged Brute",{
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望  Waypoint(传送点) Lioneye's Watch(狮眼守望)  The Lower Prison(禁灵之狱下层)
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Waypoint"},{"杀到地图","Lioneye's Watch"}},"The Lower Prison"}
        },
        [任务状态("Talk to Nessa")] = {  --对话奈沙  Prison Reward(监狱奖励)
             [做(通用任务_提交任务)]= {"Nessa","Prison Reward",1,"Tarkleigh"},
        },--{"杀到房间","The Warden's Chambers"},
        [任务状态("Explore the Prison and kill Brutus")] = { --探索监狱并击败布鲁特斯  The Warden's Chambers(典狱长的房间)  Brutus, Lord Incarcerator(典狱长布鲁特斯)
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Warden's Chambers"},{"杀到","Brutus, Lord Incarcerator"}},"The Upper Prison"}
        },
        [任务状态("Travel to The Prison")] = { --前往 禁灵之狱  The Upper Prison(禁灵之狱上层)
             做(通用任务_杀到任务地图操作)
        },
        [任务状态("Kill Brutus")] = { --击败布鲁特斯
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Warden's Chambers"},{"杀到地图","Prisoner's Gate"}},"The Upper Prison"}
        },
        [任务状态("Find the exit from the Prison")] = { --找出离开监狱的出口
             做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Tarkleigh for your reward")] = { --与塔格拉交谈以获得奖励  Brutus Reward(击败屠夫的奖赏)
             [做(通用任务_提交任务)]= {"Tarkleigh","Prison Reward",1}
        },
        [提交(通用任务_提交任务)]= {"Tarkleigh","Prison Reward",1} 
    }),



   --海妖之歌
   任务("The Siren's Cadence",{
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Waypoint"},{"杀到地图","Lioneye's Watch"}},"The Cavern of Wrath"}
        },
        [任务状态("Travel to Merveil's Caverns")] = { --击败海妖.莫薇儿
            做(做_击败海妖)
        },
        [任务状态("Find the exit to Act 2")] = { --找出前往第二章的入口  The Southern Forest(南部森林)
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Southern Forest"}
        },
        [任务状态("Kill Merveil")] = { --与奈莎交谈  Merveil Reward(击败海妖的奖励)
            做(做_击败海妖)
        },
        [任务状态("Talk to Nessa")] = { --与奈莎交谈  Merveil Reward(击败海妖的奖励)
            [做(通用任务_提交任务)]= {"Nessa","Merveil Reward",4},
        },
    }),

    --开路先锋
    单次任务("The Way Forward", { 
        [任务状态("Travel to Prisoner's Gate")] = { --前往 监狱大门  The Lower Prison(禁灵之狱下层) Prisoner's Gate(监狱大门)
            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","The Caged Brute"},{"杀到地图","Prisoner's Gate"}},"The Lower Prison"}  --做完去做冲出监牢
       },
       [任务状态("Follow the Emperor's Road")] = { --找到帝王之路 Thaumetic Seal(托麦迪克之印)
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Thaumetic Seal"}},"Prisoner's Gate"}
       },
       [任务状态("Find another way to the forest")] = { --找到其他通往森林的路 --转做海妖之歌  The Cavern of Wrath(怨忿之窟)  The Cavern of Anger(怨忿之窟深处)
            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","The Siren's Cadence"},{"杀到地图","The Cavern of Anger"},{"杀死","魔语之灵莫薇儿"}},"The Cavern of Wrath"}
       },
       [任务状态("Find the Forest Encampment")] = { --找到森林营地
            做(通用任务_杀到任务地图操作)
       },
       [任务状态("Talk to Silk")] = { --与斯克交谈
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Silk"}}}
       },
       [任务状态("Travel to The Western Forest")] = { --前往 西部密林
            做(通用任务_杀到任务地图操作)
       },
       [任务状态("Open the blocked pass")] = { --打通被阻断的道路
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Thaumetic Seal"}}}
       },
       [任务状态("Kill the Blackguards")] = { --击败乌旗守卫  Thaumetic Emblem(奇术徽章)
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Thaumetic Emblem"}}}
       },
       [任务状态("Activate the Thaumetic Seal")] = { --开启托麦迪克封印
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Thaumetic Seal"}}}
       },
       [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
             做(通用任务_杀到任务地图操作)
       },
       [任务状态("Talk to Bestel for your reward")] = { --与毕斯特交谈来获得奖励 Reopened Passage(开路奖赏)
            [做(通用任务_提交任务)]= {"Bestel","Reopened Passage",1},
        },
   }),

   --白色巨兽
   任务("The Great White Beast",{
        [任务状态("Travel to The Den")] = { --前往 兽穴
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Great White Beast")] = { --狩猎白色巨兽 The Great White Beast(白色巨兽)
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","The Great White Beast"}}}
        },
        [任务状态("Kill the Great White Beast")] = { --击败白色巨兽
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","The Great White Beast"}}}
        },
        [任务状态("Travel to The Forest Encampment")] = { --前往 森林营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Yeena for your reward")] = { --与伊娜交谈来获得奖励
            [做(通用任务_提交任务)]= {"Yeena","Great White Beast Reward",3},
        },          --Great White Beast Reward(击败白色巨兽的奖励)
        [提交(通用任务_提交任务)]= {"Yeena","Great White Beast Reward",3},
   }),

   --与盗贼们打交道
   任务("Deal with the Bandits",{
        [任务状态("Travel to The Broken Bridge, The Western Forest or The Wetlands")] = {  --前往 河畔断桥, 西部密林 或是 湿地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Eramir wants you to kill Kraityn")] = { --艾米尔要你干掉克雷顿 --Kraityn's Amulet(阿莉亚的古灵之记)
            [做(通用任务_杀到任务地图操作)] = {{{"击败","Kraityn"},{"打开","Kraityn's Amulet"}},空},
        },
        [任务状态("Travel to The Western Forest or The Wetlands")] = { --前往 西部密林 或是 湿地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Eramir wants you to kill Alira")] = {  --艾米尔要你干掉阿莉亚与欧克
            [做(通用任务_杀到任务地图操作)] = {{{"击败","Alira"},{"打开","Alira's Amulet"}},空},
        },
        [任务状态("Travel to The Wetlands")] = { --前往 湿地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Eramir wants you to kill Oak")] = { --艾米尔要你干掉欧克
            [做(通用任务_杀到任务地图操作)] = {{{"击败","Oak"},{"打开","Oak's Amulet"}},空},
        },
        [任务状态("Travel to The Forest Encampment")] = { --前往 森林营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Eramir for your reward")] = { --与艾米尔交谈来获得奖励
            [做(通用任务_提交任务)]= {"Eramir","Take the Apex",1},
            --[做(通用任务_领取奖励)] = {"Eramir",{{"Take the Apex",1}}}
        },
   }),

   --瓦尔之影
   任务("Shadow of the Vaal",{	
        [任务状态("Talk to Eramir")] = { --与艾米尔交谈
           [做(通用任务_杀到任务地图操作)] = {{{"对话","Eramir"}}}
        },
        [任务状态("Find the Ancient Pyramid")] = {--找到古金字塔
            [做(通用任务_杀到任务地图操作)] = {{},"The Ancient Pyramid"}
        },
        [任务状态("Ascend the Ancient Pyramid")] = {--登上-杀到房间 金字塔顶端 打开 黑暗祭坛
           [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Pyramid Apex"},{"打开","Dark Altar"}},"The Ancient Pyramid"}
        },
        [任务状态("Travel to The Caverns")] = {--前往 瀑布洞穴
            [做(通用任务_杀到任务地图操作)] = {{},"The Ancient Pyramid"}
        },
        [任务状态("Activate the Dark Altar")] = {--启动黑暗祭坛
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Pyramid Apex"},{"打开","Dark Altar"}},"The Ancient Pyramid"}
        },
        [任务状态("Kill the Vaal Oversoul")] = {--击败瓦尔超灵
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Vaal Oversoul"}},"The Ancient Pyramid"}
        },
        [任务状态("Find the exit to Act 3")] = {--找到前往第三章的出口--萨恩城废墟
            [做(通用任务_杀到任务地图操作)] = {{},"The City of Sarn"}
        },
        [任务状态("Find the Sarn encampment")] = {--找到萨恩营地  --卡尔莉萨  --找到萨恩营地   --萨恩城废墟
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Clarissa"},{"打开","The Sarn Encampment"}},"The City of Sarn"}
        },
   }),

   --黑色入侵者
   任务("Intruders in Black",{
        [任务状态("Travel to The Chamber of Sins")] = { --前往 罪孽之殿
            做(通用任务_杀到任务地图操作)
        },  
        [任务状态("Explore the Chamber of Sins")] = { --调查罪孽之殿.   Fidelitas, the Mourning(悼灵者费德利塔斯)
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Fidelitas, the Mourning"}},"The Chamber of Sins Level 2"}
        },
        [任务状态("Kill Fidelitas")] = { --击败费德斯塔斯
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Fidelitas, the Mourning"}}}
        },
        [任务状态("Get the Baleful Gem")] = { --取得邪影宝石   Baleful Gem(邪影宝石) Strange Device(奇怪的装置)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Strange Device"},{"打开","Baleful Gem"}}}
        },
        [任务状态("Travel to The Forest Encampment")] = { --前往 森林营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Helena")] = { --与赫莲娜交谈 --这个有问题
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Helena"}},"当前地图"}
        },      --Blackguard Reward(取回邪影宝石的奖励)
        [提交(通用任务_提交任务)]= {"Greust","Blackguard Reward",3},
   }),

   --清理蜘蛛
   任务("Sharp and Cruel",{
        [任务状态("Talk to Silk")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Silk"}}}
        },
        [任务状态("Travel to The Weaver's Chambers")] = { --前往 织网者巢穴
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Maligaro's Spike")] = { --找出【马雷格罗的尖刺】{"杀到房间","The Weaver's Nest"},
             [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Maligaro's Spike"},{"打开","Maligaro's Spike"}}}
        },
        [任务状态("Get Maligaro's Spike")] = { --取得【马雷格罗的尖刺】
             [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Maligaro's Spike"},{"打开","Maligaro's Spike"}}}
        },
        [任务状态("Travel to The Forest Encampment")] = { --前往 森林营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Silk for your reward")] = { --与斯克交谈来获得奖励 Maligaro's Spike Reward(马雷格罗的尖刺奖励)
            [做(通用任务_提交任务)]= {"Silk","Maligaro's Spike Reward",1},
        },
   }),

   --梦中圣地
   任务("Through Sacred Ground",{
        [任务状态("Travel to The Crypt")] = { --前往 寂静陵墓
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Golden Hand")] = { --找到黄金之手  {"杀到","Altar"},   Altar(祭坛) Golden Hand(黄金之手),The Crypt Level 2(寂静陵墓 2 楼)
            [做(通用任务_杀到任务地图操作)] ={{{"打开","Golden Hand"}},"The Crypt Level 2"}
        },
        [任务状态("Travel to The Forest Encampment")] = {  --前往 森林营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Yeena for your reward")] = { --与伊娜交谈来获得奖励  Jewel Reward(珠宝奖励) Fellshrine Reward(黄金之手的奖励)
           [做(通用任务_领取奖励)] = {"Yeena",{{"Jewel Reward",1},{"Fellshrine Reward",1}}}
        },
   }),

   --盘根错节
   任务("The Root of the Problem",{
        [任务状态("Travel to The Wetlands")] = { --前往 湿地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Poison the ancient tree")] = { --将古树毒化 Tree Roots(树根)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Tree Roots"}}}
        },
        [任务状态("Travel to The Vaal Ruins")] = { --前往 瓦尔废墟
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find a way through the Vaal Ruins")] = {  --找到进入瓦尔遗迹的方法 The Northern Forest(北部密林)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Waypoint"}},"The Northern Forest"}
        },
   }),

   --迷失的爱情
   任务("Lost in Love",{ 
        [任务状态("Find the Sarn encampment")] = { --前往 萨恩城废墟
             [做(通用任务_杀到任务地图操作)] = {{{"杀到","Clarissa"},{"打开","The Sarn Encampment"}},"The City of Sarn"}
        },
        [任务状态("Travel to The City of Sarn")] = { --前往 萨恩城废墟
             做(通用任务_杀到任务地图操作)
        },
        [任务状态("Defeat the guards and rescue the girl")] = {--击败守卫并拯救女孩
            做(做_击败守卫并挽救女孩)
        },
        [任务状态("Kill Clarissa's guards")] = {-- 注意这个状态名
            做(做_击败守卫并挽救女孩)
        },
        [任务状态("Find Tolman in the Crematorium")] = {--在火葬场找到托尔曼--派蒂--托尔曼
            --[做(通用任务_杀到任务地图操作)] = { { {"杀到","Tolman"},{"打开","Tolman"},{"拾取","任务物品"} },空},
            [做(通用任务_杀到任务地图操作)] = { { {"杀到","Tolman"},{"打开","Tolman's Bracelet"} },空},
        },
        [任务状态("Find Clarissa and Tolman")] = {--在火葬场找到托尔曼--派蒂--托尔曼
            [做(通用任务_杀到任务地图操作)] = { { {"杀到","Tolman"},{"打开","Tolman's Bracelet"} },空},
        },  
        [任务状态("Get Tolman's bracelet")] = { --取得托尔曼的手镯
            --[做(通用任务_杀到任务地图操作)] = { { {"杀死","Piety"},{"打开","Tolman"},{"拾取","任务物品"} },空},
            [做(通用任务_杀到任务地图操作)] = { { {"打开","Tolman's Bracelet"} },空},
        },
        [任务状态("Travel to The Sarn Encampment")] = { --前往 萨恩营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Clarissa")] = {--与卡尔莉萨交谈--取得下水道钥匙
            [做(通用任务_提交任务)] = {"Clarissa","Take Sewer Keys",1},
        },
		--添加一个新的项，防止掉线断档。
		[任务状态("Talk to Maramoa,Talk to Clarissa")] = {--与马拉莫交谈来获得奖励   --交完任务去接宝石皇后任务
           [做(通用任务_提交任务)] = {"Clarissa","Take Sewer Keys",1},
        },
        [任务状态("Talk to Maramoa for your reward")] = {--与马拉莫交谈来获得奖励   --交完任务去接宝石皇后任务
            [做(通用任务_提交任务)] = {"Maramoa","Clarissa Reward",4,"Grigor"},
        },
   }),

   --宝石皇后
   任务("The Gemling Queen",{
        [任务状态("Travel to The Solaris Temple")] = {--前往 日耀神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Solaris Temple Level 2")] = {--前往 日耀神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the tomb of the Gemling Queen")] = { --找出宝石皇后的陵墓  --达拉夫人
             [做(通用任务_杀到任务地图操作) ] = {{{"杀到","Lady Dialla"},{"对话","Lady Dialla"}},空}
        },
   }),

   --丝带线轴
   任务("The Ribbon Spool",{
        [任务状态("Travel to The Battlefront")] = { --前往 激战广场
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Ribbon Spool")] = {--找出丝带线轴 --缎带之轴
            [做(通用任务_杀到任务地图操作) ] = {{{"打开","Ribbon Spool"}},空}
        },
        [任务状态("Travel to The Solaris Temple")] = {--前往 日耀神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Lady Dialla for your reward")] = {--与达拉夫人交谈来获得奖励--取回缎带之轴的奖励
            [做(通用任务_提交任务)]= {"Lady Dialla","Ribbon Spool Reward",1},
        },
        [任务状态("Travel to The Solaris Temple Level 2")] = {--与达拉夫人交谈来获得奖励--取回缎带之轴的奖励
            做(通用任务_杀到任务地图操作)
        },
   }),

   --余烬之尘
   任务("Fiery Dust",{
        [任务状态("Travel to The Docks")] = {--前往 不朽海港
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Thaumetic Sulphite")] = {--找出托麦迪克亚硫酸--托麦迪克亚硫酸
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Supply Container"},{"打开","Thaumetic Sulphite"}},空}
        },
        [任务状态("Travel to The Solaris Temple Level 2")] = {--前往 日耀神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Lady Dialla")] = {--与达拉夫人交谈--取得炼狱之粉
            [做(通用任务_提交任务)]= {"Lady Dialla","Take Infernal Talc",1},
        },
        [任务状态("Travel to The Warehouse Sewers")] = {--前往 仓库地带下水道
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Sewers")] = {--前往 仓库地带下水道
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Destroy the Undying Blockage")] = {--摧毁不朽屏障--不朽屏障
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Undying Blockage"}},空}
        },
   }),

   --维多利奥的秘宝
   任务("Victario's Secrets",{ 
        [任务状态("Talk to Hargan")] = {--与贺根交谈
             [做(通用任务_杀到任务地图操作)] = {{{"对话","Hargan"}},空}
        },
        [任务状态("Find the platinum bust")] = { --找出半身像 --藏物处(Stash)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Stash"},{"拾取","任务物品"}},"当前地图"}
        },
        [任务状态("Find the third Platinum Bust")] = { --找出半身像 --藏物处(Stash)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Stash"},{"拾取","任务物品"}},"当前地图"}
        },
        [任务状态("Find the two remaining Platinum Busts")] = { --找出半身像 --藏物处(Stash)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Stash"},{"拾取","任务物品"}},"当前地图"}
        },
        [任务状态("Travel to The Sarn Encampment")] = { --前往 萨恩营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Sewers")] = { 
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Market Sewers")] = { --前往 市集地带下水道
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Market Sewers"}
        },
        [任务状态("Travel to The Warehouse Sewers")] = { --前往 仓库地带
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Warehouse Sewers"}
        },
        [任务状态("Travel to The Slums Sewers or The Warehouse Sewers")] = { --前往 前往 贫民窟下水道 或是 市集地带下水道
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Slums Sewers"}
        },
        [任务状态("Talk to Hargan for your reward")] = {--与贺根交谈来获得奖励--维多里奥的奖赏
            [做(通用任务_提交任务)]= {"Hargan","Victario Reward",1},
        },
   }),

   --铲除左右手
   任务("Sever the Right Hand",{ 
        [任务状态("Travel to The Ebony Barracks")] = {--前往 乌旗守卫兵营
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Kill General Gravicius")] = { --击败格拉维奇将军--格拉维奇将军
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","General Gravicius"}}}
        },  
        [任务状态("Travel to The Sarn Encampment")] = {--前往 萨恩营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Maramoa for your reward")] = {--与马拉莫交谈来获得奖励--击败将军的奖励
            [做(通用任务_提交任务)]= {"Maramoa","Gravicius Reward",4},
        },
   }),

   --派蒂的宠物
   任务("Piety's Pets",{
        [任务状态("Travel to The Lunaris Temple")] = {--前往 月影神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Piety")] = {--找出派蒂--传送门--神塔之钥--月影神殿:第 2 层
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Tower Key"}},"The Lunaris Temple Level 2"},
        },
        [任务状态("Kill Piety")] = {--击败派蒂q
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Tower Key"}},"The Lunaris Temple Level 2"},
        },
        [任务状态("Travel to The Sarn Encampment")] = {--前往 萨恩营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Grigor for your reward")] = {--与葛里戈交谈来获得奖励--击败派蒂的奖励
            [做(通用任务_提交任务)]= {"Grigor","Piety Reward",1},
        },
   }),

   --神权之塔 
   任务("The Sceptre of God",{
        [任务状态("Travel to The Imperial Gardens")] = {--前往 皇家花园(子地图:树篱迷宫)
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Lunaris Temple")] = {--前往 月影神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Piety")] = {--取得派蒂的钥匙--神塔之钥
        [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Tower Key"}},"The Lunaris Temple Level 2"},
        },
        [任务状态("Unlock the Sceptre of God")] = {--打开神权之塔的大门--上锁的门
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Locked Door"}},空}
        },
        [任务状态("Travel to The Sceptre of God")] = {--前往神权之塔的顶端--"上层神权之塔--奇术之灵神主
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Dominus"}},"The Upper Sceptre of God"}
        },
		[任务状态("Ascend the Sceptre of God")] = {--登上神权之塔的顶端 {"杀到房间","Portal"},
            [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","BOOS"},{"杀死","Dominus Ascendant"}},"The Upper Sceptre of God"}
        },
        [任务状态("Fight your way to Dominus")] = {--击败神主
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Tower Rooftop"},{"杀死","Dominus Ascendant"}},"The Upper Sceptre of God"}
        },
		[任务状态("Kill Dominus")] = {--击败神主
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Tower Rooftop"},{"杀死","Dominus Ascendant"}},"The Upper Sceptre of God"}
        },
        [任务状态("Talk to Dialla")] = {--与达拉夫人交谈
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Lady Dialla"}},"The Upper Sceptre of God"}
        },
        [任务状态("Find the exit to Act 4")] = {--找到前往第四章的出口--水道遗迹
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Aqueduct"}},"The Upper Sceptre of God"}
        },
   }),

      --重生的渴望
   任务("A Swig of Hope",{
        [任务状态("Return to The Ship Graveyard in Act One")] = {  --The Ship Graveyard(魅影船墓)
            [做(通用任务_杀到任务地图操作)] = {{},"The Ship Graveyard"}
        },
        [任务状态("Travel to The Docks")] = { --前往 不朽海港
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Search the Docks")] = { --搜索不朽海港
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Captain Fairgraves"},{"对话","Captain Fairgraves"}}}
        },
        [任务状态("Travel to The Imperial Gardens")] = { --前往 皇家花园(子地图:树篱迷宫)
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Chitus' Plum")] = { --找出【切特斯的梅花果】  Chitus' Plum（切特斯的梅果） The Hedge Maze(树篱迷宫)
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Plum"},{"打开","Chitus' Plum"}},"The Hedge Maze"}
        },
        [任务状态("Talk to Fairgraves")] = { --与费尔船长交谈 Swig of Hope Reward(取得聚魂美酒的奖励)
            [做(通用任务_提交任务)] = {"Captain Fairgraves","Swig of Hope Reward",1}
        }
   }),

   --被放逐的水手
   任务("The Marooned Mariner",{
        [任务状态("Find Fairgraves")] = { --找到费尔船长
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Captain Fairgraves"},{"对话","Captain Fairgraves"}}}
        },  
        [任务状态("Find the Allflame")] = { --找到不灭之火
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Allflame"}}}
        },
        [任务状态("Travel to The Ship Graveyard Cave")] = { --前往 魅影船墓
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Ship Graveyard")] = { --前往 魅影船墓
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Bring the Allflame to Fairgraves")] = { --将不灭之火交给费尔船长
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Captain Fairgraves"},{"对话","Captain Fairgraves"}}}
        },
        [任务状态("Kill Fairgraves")] = { --找到不灭之火
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Captain Fairgraves"}}}
        },
        [任务状态("Travel to Lioneye's Watch")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Bestel for your reward")] = { --与毕斯特交谈来获得奖励 Fairgraves Reward(击败费尔船长的奖赏)
            [做(通用任务_提交任务)]= {"Bestel","Fairgraves Reward",1},
        },
        [提交(通用任务_提交任务)]= {"Bestel","Fairgraves Reward",1},
   }),

   --突破封印
   任务("Breaking the Seal",{
        [任务状态("Find Deshret's Banner in the Dried Lake")] = {--找到迪虚瑞特的旗帜
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Deshret's Banner"}}}
        },
        [任务状态("前往 干涸湖岸")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Take Deshret's Banner")] = {--取回迪虚瑞特的旗帜
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Deshret's Banner"}}}
        },
        [任务状态("Travel to Highgate")] = {--前往 统治者之殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Oyun")] = {--对话 欧忧
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Oyun"}}}
        },
        [任务状态("Use Deshret's Banner on the Seal to open the Mines")] = {--使用旗帜破除迪虚瑞特的封印
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Deshret's Seal"}}}
        },
        [任务状态("Kill Voll and take Deshret's Banner")] = { --击败福尔并取回迪虚瑞特的旗帜
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Deshret's Banner"}}}
        },
        [任务状态("Talk to Oyun for your reward")] = {--对话Oyun--红旗奖赏
            [做(通用任务_提交任务)]= {"Oyun","Red Banner Reward",4},
        },
    }),

    --不屈意志 
    任务("An Indomitable Spirit",{ 
        [任务状态("Find Deshret's spirit")] = { --释放迪虚瑞特的灵魂  The Mines Level 2(漆黑矿坑:第 2 层)
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Deshret's Spiriteee"}},"The Mines Level 2"}
        },
--        [任务状态("前往 漆黑矿坑")] = {
--            [做(通用任务_杀到任务地图操作)] = {{},"漆黑矿坑:第 2 层"}
--        },  
        [任务状态("Talk to Tasuni in town for your reward")] = { --与营地的达苏尼交谈以获取奖励 --Deshret Reward(迪虚瑞特奖励)
            [做(通用任务_提交任务)]= {"Tasuni","Deshret Reward",1},
        },

   }),

   --暴怒之王
   任务("The King of Fury",{
        [任务状态("Find Kaom and the Eye of Fury")] = {--找到冈姆与愤怒之眼-- --愤怒之眼
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Caldera of The King"},{"打开","The Eye of Fury"}},"Kaom's Stronghold"}
        },
        [任务状态("Kill Kaom")] = {--击败冈姆--冈姆的堡垒--Caldera of The King熔岩之环
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Caldera of The King"},{"打开","The Eye of Fury"}},"Kaom's Stronghold"}
        },
        [任务状态("Travel to The Crystal Veins")] = {--前往 水晶矿脉  --统治者之殿
            [做(通用任务_杀到任务地图操作)] =  {{{"杀到地图","The Crystal Veins"}},"Highgate"}
        },
        [任务状态("Travel to Kaom's Stronghold")] = {--前往 冈姆的堡垒
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Lady Dialla"}}}
        },
        [任务状态("Deliver the Eye of Fury to Dialla")] = {--把愤怒之眼交给达拉
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"对话","Lady Dialla"}}}
        },
        [任务状态("Take the Eye of Fury")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Eye of Fury"}},"Kaom's Stronghold"}
        },
        [任务状态("Talk to Dialla in town for a reward")] = {--与营地的达拉交谈以获取奖励
            [做(通用任务_提交任务)]= {"Dialla","Rapture Reward",1},
        },
   }),

   --欲望之王
   任务("The King of Desire",{
        [任务状态("Find Daresso and the Eye of Desire")] = {-- 找到德瑞索与欲望之眼--剑刃之环--欲望之眼
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Ring of Blades"},{"打开","The Eye of Desire"}}}
        },
        [任务状态("Kill Daresso")] = {--击败德瑞索
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Ring of Blades"},{"打开","The Eye of Desire"}}}
        },
        [任务状态("Take the Eye of Desire")] = {--取回欲望之眼
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Eye of Desire"}}}
        },
        [任务状态("Travel to The Crystal Veins")] = {--前往 水晶矿脉--统治者之殿
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Crystal Veins"}},"Highgate"}
        },
        [任务状态("Deliver the Eye of Desire to Dialla")] = {--把欲望之眼交给达拉
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"访问","Lady Dialla"},{"对话","Lady Dialla"}}}
        },
   }),

   --永恒梦魇
   任务("The Eternal Nightmare",{
        [任务状态("Travel to Highgate")] = {--前往 统治者之殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Dialla")] = {--与达拉交谈
            [做(通用任务_杀到任务地图操作)] = { {{"对话","Lady Dialla"}},空}
        },
        [任务状态("Talk to Oyun")] = {--与欧优恩交谈
            [做(通用任务_杀到任务地图操作)] = { {{"对话","Oyun"}},空}
        },
        [任务状态("Talk to Tasuni")] = {
            [做(通用任务_杀到任务地图操作)] = { {{"对话","Tasuni"}},空}
        },
        [任务状态("Find the Rapture Device")] = {--找到裂界装置--冈姆的幻境
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"对话","Lady Dialla"}}}
        },
        [任务状态("Travel to The Belly of the Beast")] = {--巨兽之腹:第 1 层
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Belly of the Beast Level 1"}},"The Crystal Veins"}  
        },
        [任务状态("Destroy the Beast within")] = {--进入巨兽之腹
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Belly of the Beast Level 2"},{"打开","The Bowels of the Beast"},{"杀死","Piety, the Abomination"}},"The Crystal Veins"}
        },
        [任务状态("Kill Piety")] = {  --在小地图房间   
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Piety, the Abomination"}},"The Belly of the Beast Level 2"}
        },
        [任务状态("Talk to Piety")] = {--与派蒂交谈  --在小地图房间  这个怪物名字下次注意下
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Piety"}},"The Belly of the Beast Level 2"}
        },
        [任务状态("Travel to The Harvest")] = {       --前往 育灵之室
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find a way into the Black Core")] = { --找到进入黑灵核心的路  --这个需要自动打开 一些传送门
            做(做_找到进入黑灵核心的路)
        },
        [任务状态("Kill Doedre")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Doedre Darktongue"}}}
        },
        [任务状态("Kill Maligaro")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Maligaro Darktongue"}}}
        },
        [任务状态("Kill Shavronne")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Shavronne of Umbra"}}}
        },
        [任务状态("Kill Shavronne to access the Black Core")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Shavronne of Umbra"}}}
        },
        [任务状态("Enter the Black Core")] = {--进入黑灵核心--Malachai, The Nightmare梦魇玛拉凯
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Piety"},{"杀到房间","The Black Hear"}}}
        },
        [任务状态("Kill Malachai")] = {  --击败玛拉凯的守卫以进入黑灵核心     -- 这个需要杀三个怪才行 而且状态还不会改变
            --[做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Malachai, The Nightmare"}}}
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Black Hear"},{"杀死","Malachai, The Nightmare"}}}
        },
        [任务状态("Kill Malachai's guardians to access the Black Core")] = { --名字没看清请比对   --杀死后 打开黑灵核心
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Piety"}}}
           -- 做(做_击败玛拉凯)
        },
    }),

   任务("Corpus Malachus",{
        [任务状态("Deliver the organ to Piety")] = { 
            [做(通用任务_杀到任务地图操作)] ={"对话","Piety"}
        },
   }),

    --3.0新增任务 -----------------------------------------------
   任务("Return to Oriath",{
        [任务状态("Travel to The Ascent")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Access the Ascent from the eastern side of Highgate")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Climb the Ascent to find the portal back to Oriath")] = {
            [做(通用任务_杀到任务地图操作)] = { {{"杀到","Lever"}}}
        },
        [任务状态("Activate the Resonator and travel back to Oriath")] = {
            [做(通用任务_杀到任务地图操作)] = { {{"打开","Lever"}}}
        },
        [任务状态("Enter the portal to Oriath")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Oriath Portal"}}}
        },
        [任务状态("Escape the Slave Pens")] = {
             [做(通用任务_杀到任务地图操作)] = {{{"杀死","Overseer Krow"}}}
        },
        [任务状态("Enter Overseer's Tower")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Kill Overseer Krow to access town")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Overseer Krow"}}}
        },
        [任务状态("Talk to Lani for a reward")] = {  
            [做(通用任务_提交任务)]= {"Lani","Overseer Reward",1},
        },
    }),

    任务("The Key to Freedom",{
        [任务状态("Kill Justicar Casticus")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Justicar Casticus"}}}
        },
        [任务状态("Climb the Ascent to find the portal back to Oriath")] = {
            --做(通用任务_杀到任务地图操作)--] = { {{"对话","Lady Dialla"}},空}
            通用做任务_全图杀怪
        },
        [任务状态("Activate the Resonator and travel back to Oriath")] = {
            做(通用任务_杀到任务地图操作)--] = { {{"杀到","Oriath Portal"}}}
        },
        [任务状态("Enter the portal to Oriath")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Oriath Portal"}}}
        },
        [任务状态("Enter Overseer's Tower")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Lani for your reward")] = {  
            [做(通用任务_提交任务)]= {"Lani","Casticus Reward",1},
        },
    }),

    任务("In Service to Science",{
        [任务状态("Return the Miasmeter to Vilenta")] = {
            [做(通用任务_提交任务)]= {"Vilenta","Miasmeter Reward",1},
        },
        [任务状态("Talk to Vilenta for your reward")] = {
            [做(通用任务_提交任务)]= {"Vilenta","Miasmeter Reward",1},
        },
    }),

    任务("Death to Purity",{
        [任务状态("Travel to The Control Blocks")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Take the Eyes of Zeal")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Eyes of Zeal"}}}
        },
        [任务状态("Gain access to the Templar Courts")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Justicar Casticus"}}}
        },
        [任务状态("Activate the Resonator and travel back to Oriath")] = {
            做(通用任务_杀到任务地图操作)--] = { {{"杀到","Oriath Portal"}}}
        },
        [任务状态("Enter the portal to Oriath")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Oriath Portal"}}}
        },
        [任务状态("Use the Eyes of Zeal to access the Templar Courts")] = {  
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Chamber of Innocence")] = {  
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find High Templar Avarius and kill him")] = {  
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Sanctum of Innocence"},{"杀死","Innocence, God-Emperor of Eternity"}}}
        },
        [任务状态("Kill High Templar Avarius")] = {  
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Sanctum of Innocence"},{"杀死","Innocence, God-Emperor of Eternity"}}}
        },
        [任务状态("Talk to Sin")] = {  
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Sanctum of Innocence"},{"对话","Sin"}}}
        },
        [任务状态("Talk to Lani for your reward")] = {  
            [做(通用任务_提交任务)]= {"Lani","Avarius Reward",2},
        },
    }),

    任务("The Ravenous God",{
        [任务状态("Travel to The Chamber of Innocence")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Cathedral Rooftop")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Search the Ossuary for the Sign of Purity")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Sign of Purity"}},"The Ossuary"}
        },
        [任务状态("Use the Sign of Purity to defeat Kitava")] = {       --可以做Kitava's Torments任务 {"检测任务","Kitava's Torments"},
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"打开","Cradle of Purity"}}}
        },
        [任务状态("Kill Kitava")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"打开附近","Cradle of Purity"},{"杀死","Kitava, the Insatiable"}}}
        },
        [任务状态("Talk to Sin")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Take the ship back to Wraeclast")] = {
            [做(通用任务_对话NPC)] = {"Lilly Roth","Sail to Wraeclast"}
        },
    }),
    
    任务("Kitava's Torments",{
        [任务状态("Travel to the Reliquary")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Kitava's Torments")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Tukohama's Tooth"}}}
        },
        [任务状态("Find the rest of Kitava's Torments")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Hinekor's Hair"}}}
        },
        [任务状态("Find the last of Kitava's Torments")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Valako's Jaw"}}}
        },
        [任务状态("Deliver Kitava's Torments to Lani")] = {
            [做(通用任务_提交任务)]= {"Lani","Torments Reward",1},
        },
        [任务状态("Talk to Lani for your reward")] = {
            [做(通用任务_提交任务)]= {"Lani","Torments Reward",1},
        },
    }),

    任务("The King's Feast",{
        [任务状态("Talk to Bannon in the Chamber of Innocence")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Bannon"}}}
        },
        [任务状态("Talk to Bannon in town for your reward")] = {
            [做(通用任务_提交任务)]= {"Bannon","Utula Reward",1},
        },
        [任务状态("Kill Utula")] = {        --这个怪物名字下次注意看下
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Utula"}}}
        },
    }),



    -------第六章任务--------------------

    任务("Bestel's Epic",{
        [任务状态("Find Bestel's Manuscript at the Tidal Island")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Bestel's Manuscript"}}}
        },
        [任务状态("Deliver the Manuscript to Bestel")] = {
           [做(通用任务_提交任务)]= {"Bestel","Bestel's Epic Reward",1},
        },
        [任务状态("Talk to Li")] = {
           [做(通用任务_提交任务)]= {"Bestel","Bestel's Epic Reward",1},
        },
    }),
    
    任务("The Father of War",{
        [任务状态("Search the area for whoever has the missing eye")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Eye of Conquest"}}}
        },
        [任务状态("Use the eye to access the Karui Fortress")] = {
            [做(通用任务_杀到任务地图操作) ] = {{{"打开","Fortress Gate"}}}
        },
        [任务状态("Travel to The Mub Flats")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Access the Karui Fortress")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Karui Fortress"}}}
        },
        [任务状态("Find the exit to the Karui Fortress")] = {
            [做(通用任务_杀到任务地图操作)] =    {{{"杀到房间", "Tukohama's Keep"},{"杀死", "Tukohama, karui God of War"}}}
        },
        [任务状态("Defeat Tukohama to unlock the exit")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间", "Tukohama's Keep"},{"杀死", "Tukohama, karui God of War"}}}
        },
        [任务状态("Talk to Tarkleigh for a reward")] = {
            [做(通用任务_提交任务)]= {"Tarkleigh","Tukohama Reward",1},
        }
    }),

    单次任务("Essence of Umbra",{
        [任务状态("Talk to Sin")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Kill Shavronne")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","The Father of War"},{"杀到房间","Prison Rooftop"},{"杀死","Shavronne the Returned"}},"Shavronne's Tower"}
        },
--        [任务状态("Travel to Axiom Prison")] = {
--            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","The Father of War"},{"杀到","Fortress Gate"},{"检测任务","The Father of War"}},"The Mud Flats"}
--        },
        [任务状态("Kill Shavronne and Brutus")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"检测任务","The Father of War"},{"杀到房间","Prison Rooftop"},{"杀死","Shavronne the Returned"}},"当前地图"}
        },
        [任务状态("Talk to Tarkleigh for your reward")] = {
             [做(通用任务_提交任务)]= {"Tarkleigh","Shavronne Reward",1},
        },
    }),

   任务("The Puppet Mistress",{
        [任务状态("Defeat the Puppet Mistress")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Spawning Ground"},{"杀死","Ryslatha, the Puppet Mistress"}}}
        },
        [任务状态("Talk to Tarkleigh for your reward")] = {
            [做(通用任务_提交任务)]= {"Tarkleigh","Puppet Mistress Reward",1},
        },
    }),

    单次任务("The Brine King",{
        [任务状态("Search the Mud Flats for Nessa")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Nessa"}}}
        },
        [任务状态("Travel through Prisoner's Gate to the Ship Graveyard")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Black Flag"}}}
        },
        [任务状态("Use the Black Flag at the Beacon to summon the Black Crest")] = {    --杀到柱子附近触发下个任务状态
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","可移动柱子"}}}
        },
        [任务状态("Push the fuel carts into position to refuel the Beacon")] = {    --跟着柱子走
            做(做_跟着柱子走)
        },
        [任务状态("Travel to The Beacon")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Light the Beacon and throw the Black Flag into the flames")] = { --杀到打开火把
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Ignition Switch"}}}
        },
        [任务状态("Talk to Weylam Roth at the Beacon")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Weylam Roth"}}}
        },
        [任务状态("Board the Black Crest to sail to the Brine King's Reef")] = { 
            [做(通用任务_对话NPC)] = {"Weylam Roth","Sail to the Brine King's Reef"}
        },
        [任务状态("Search the Brine King's Reef for Nessa")] = { 
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Brine King's Throne"},{"杀死","Tsoagoth, The Brine King"}}}
        },
        [任务状态("Defeat the Brine King")] = { 
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Brine King's Throne"},{"杀死","Tsoagoth, The Brine King"}}}
        },
        [任务状态("The exit to Act 7 is now open")] = {
             [做(通用任务_对话NPC)]= {"Weylam Roth now","Sail to Act 7"},
        },
    }),

    任务("Fallen from Grace",{
        [任务状态("Clear the Twilight Strand")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"全图杀","Fallen from Grace"}}}
        },
        [任务状态("Talk to Lilly for your reward")] = {
           [做(通用任务_提交任务)]= {"Lilly Roth","Twilight Strand Reward",1},
        },
    }),



    任务("The Cloven One",{
        [任务状态("Defeat Abberath")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Valley of the Fire Drinker"},{"杀死","Abberath, the Cloven One"}}}
        },
        [任务状态("Talk to Bestel for your reward")] = {
            [做(通用任务_提交任务)]= {"Bestel","Aberrath Reward",1},
        },
    }),
    --------------------第七章---------------------------------
    任务("Essence of the Artist",{
        [任务状态("Talk to Sin")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Talk to Helena to find out how to reach Maligaro")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Helena"}}}
        },
        [任务状态("Find Maligaro's Map")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Maligaro's Map"}}}
        },
        [任务状态("Travel to The Chamber of Sins")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Crypt")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find Maligaro")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Chamber of Sins Level 1"},{"打开","Map Device"}},"The Bridge Encampment"}
        },
        [任务状态("Use Maligaro's Map in his Map Device")] = { --开启异界装置
            [做(通用任务_杀到任务地图操作)] = {{{"开启装置","Maligaro's Map"}}}
        },
        [任务状态("Enter Maligaro's Sanctum")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Maligaro's Workshop"},{"打开","Black Venom"}}}
        },
        [任务状态("Find Maligaro and kill him")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Maligaro's Workshop"},{"打开","Black Venom"}}}
        },
        [任务状态("Kill Maligaro")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Maligaro's Workshop"},{"打开","Black Venom"}},"Maligaro's Sanctum"}
        },
        [任务状态("Talk to Helena for your reward")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Bridge Encampment"},{"对话","Helena"}},"The Bridge Encampment"}
        },
        [任务状态("Travel to Maligaro's Sanctum")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"开启装置","Maligaro's Map"},{"杀到地图","Maligaro's Sanctum"}},"The Chamber of Sins Level 1"}
        },
        [提交(通用任务_提交任务)]= {"Helena","Maligaro Reward",1},
    }),

    任务("Web of Secrets",{
        [任务状态("Bring the Black Venom to Silk")] = {
            [做(通用任务_提交任务)]= {"Silk","Black Death Reward",1},
        },
        [任务状态("Talk to Silk for your reward")] = {
            [做(通用任务_提交任务)]= {"Silk","Black Death Reward",1},
        },
    }),

    任务("The Master of a Million Faces",{
        [任务状态("Find a way info the Den")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Use the Obsidian Key to access the Den")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Secret Passage"}},"The Chamber of Sins Level 2"}
        },
        [任务状态("Find the old Forest Encampment")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Fortress Encampment"},{"杀死","Greust, Lord of the Forest"}}}
        },
        [任务状态("Kill Greust")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Fortress Encampment"},{"杀死","Greust, Lord of the Forest"}}}
        },
        [任务状态("Talk to Eramir for your reward")] = {
            [做(通用任务_提交任务)]= {"Eramir","Ralakesh Reward",1},
        },
        [提交(通用任务_提交任务)]= {"Eramir","Ralakesh Reward",1},
    }),
    
    任务("In Memory of Greust",{
        [任务状态("Take Greust's Necklace from Helena")] = {
            [做(通用任务_提交任务)]= {"Helena","Take Greust's Necklace",1},
        },
        [任务状态("Ask Helena about the Azmeri Shrine")] = {
            [做(通用任务_提交任务)]= {"Helena","Take Greust's Necklace",1},
        },
        [任务状态("Take Greust's Necklace to the shrine in the Northern Forest")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Azmeri Shirine"}}}
        },
        [任务状态("Place Greust's Necklace on the Azmeri shrine")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Azmeri Shirine"}}}
        },
        [任务状态("Travel to The NorThern Forest")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Helena for your reward")] = {
            [做(通用任务_提交任务)]= {"Helena","Greust's Necklace Reward",1},
        },

        [提交(通用任务_提交任务)]= {"Helena","Greust's Necklace Reward",1},
    }),

    任务("The Mother of Spiders",{
        [任务状态("Find Arakaali's temple and save Silk")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","The Temple of Decay Level 1"},{"对话","Yeena"}},"The Vaal City"}
        },
        [任务状态("Enter Arakaali's temple and save Silk")] = {
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Temple of Decay Level 2"}
        },
        [任务状态("Travel to The Temple of Decay")] = {
            [做(通用任务_杀到任务地图操作)] = {{{}},"The Temple of Decay Level 2"}
        },
        [任务状态("Find Silk and Arakaali")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Arakaali's Web"},{"杀死","Arakaali, Spinner of Shadows"}},"The Temple of Decay Level 2"}
        },
        [任务状态("Kill Arakaali")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Arakaali's Web"},{"杀死","Arakaali, Spinner of Shadows"}},"The Temple of Decay Level 2"}
        },
        [任务状态("The exit to Act 8 is now open")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Sarn Ramparts"}},"The Bridge Encampment"}
        },
    }),

    任务("Lighting the Way",{
        [任务状态("Search the Dread Thicket for Fireflies")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Firefly"}}}
        },
        [任务状态("Search the Dread Thicket for the remaining Fireflies")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Firefly"}}}
        },
        [任务状态("Deliver the Fireflies to Yeena at Arakaali's temple")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Yeena"}},"The Vaal City"}
        },
    }),

    任务("Kishara's Star",{
        [任务状态("Deliver Kishara's Star to Weylam")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Weylam Roth"}}}
        },
        [任务状态("Talk to Weylam for your reward")] = {
            [做(通用任务_提交任务)]= {"Weylam Roth","Kishara's Star Reward",1},
        },
    }),

    任务("The Silver Locket",{
        [任务状态("Find Weylam's Silver Locket")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Silver Locket"}}}
        },
        [任务状态("Deliver the Silver Locket to Weylam")] = {
            [做(通用任务_提交任务)] = {"Weylam Roth","Silver Locket Reward",1}
        },
        [提交(通用任务_提交任务)] = {"Weylam Roth","Silver Locket Reward",1}
    }),



    -------------------------------第八章----------------------------
    任务("Essence of the Hag",{
        [任务状态("Meet Sin at the Sarn Encampment")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Find and kill Doedre")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Doedre"}},"The Toxic Conduits"}
        },
        [任务状态("Travel to The Toxic Conduits")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Kill Doedre")] = {       --有问题 会一直杀怪
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Cauldron"},{"杀死","Doedre the Vile"}}}
        },
        [任务状态("Talk to Hargan for your reward")] = {     
             [做(通用任务_提交任务)] = {"Hargan","Doedre Reward",1}
        },
        [提交(通用任务_提交任务)] = {"Hargan","Doedre Reward",1}
    }),

    任务("The Wings of Vastiri",{
        [任务状态("Travel to The Bath House")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Wings of Vastiri")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Hector Titucius, Eternal Servan"}}}
        },
        [任务状态("Kill Hector Titucius")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Hector Titucius, Eternal Servant"}}}
        },
        [任务状态("Deliver the Wings of Vastiri to Hargan")] = {
            [做(通用任务_提交任务)] = {"Hargan","Wings of Vastiri Reward",1}
        },
        [提交(通用任务_提交任务)] = {"Hargan","Wings of Vastiri Reward",1}
    }),

    任务("Lunar Eclipse",{
        [任务状态("Talk to Maramoa")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Maramoa"}}}
        },
        [任务状态("Travel to The Lunaris Temple")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Travel to The Harbour Bridge")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Moon Orb")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Moon Orb"}},"The Lunaris Temple Level 2"}
        },
        [任务状态("Kill Dusk")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Moon Orb"}},"The Lunaris Temple Level 2"}
        },
        [任务状态("Find the Sun Orb")] = {
            --[做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Solaris Temple Level 2"},{"打开","Sun Orb"}},"The Sarn Encampment"}
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Sun Orb"}},"The Solaris Temple Level 2"}
        },
        [任务状态("Take the Moon Orb to the Harbour Bridge")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Sky Shrine"},{"打开","Statue of The Sisters"}}}
        },
        [任务状态("Kill Solaris and Lunaris")] = { --Solaris,Eternal Sun
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Sky Shrine"},{"杀死","Lunaris, Eternal Moon"}}}
        },
        [任务状态("Take the Moon Orb")] = { 
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Portal"},{"打开","Moon Orb"}},"The Lunaris Temple Level 2"}
        },
        [任务状态("The exit to Act 9 is now open")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Blood Aqueduct"}},"The Sarn Encampment"}
        },
    }),

    任务("Love is Dead",{  
        [任务状态("Travel to The Quay")] = {
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Ankh of Eternity")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Ankh of Eternity"}}}
        },
        [任务状态("Give her the Ankh")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Arena"},{"对话","Clarissa"}}}
        },
        [任务状态("Talk to Clarissa")] = {
            [做(通用任务_提交任务)] = {"Clarissa","Tolman Reward",1}
        },
    }),

    任务("The Gemling Legion",{  
        [任务状态("Talk to Maramoa for your reward")] = {
            [做(通用任务_提交任务)] = {"Maramoa","Gemling Legion Reward",1}
        },
        [任务状态("Travel to The Grain Gate")] = {
            做(通用任务_杀到任务地图操作)
        },
    }),

    任务("Reflection of Terror",{  
        [任务状态("Kill Yugul")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Pools of Terror"},{"杀死","Yugul, Reflection of Terror"}}}
        },
        [任务状态("Talk to Hargan for your reward")] = {
            [做(通用任务_提交任务)] = {"Hargan","Yugul Reward",1}
        },
    }),

-----------------第九章任务---------------------
    任务("Recurring Nightmare",{  
        [任务状态("Meet Sin in Highgate")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Find the Basilisk Acid")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Basilisk Acid"}}}
        },
        [任务状态("Find the Trarthan Powder")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Trarthan Powder"}}}
        },
        [任务状态("Deliver the Trarthan Powder to Sin")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Meet Sin at the Quarry")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Meet Sin at the Black Core")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Defeat the spirit of Doedre")] = {
            [做(通用任务_杀到任务地图操作重复)] = {{{"杀到房间","Doedre's Despair"},{"重新打开","The Black Heart"}}}
        },
        [任务状态("Defeat the spirit of Shavronne")] = {
            [做(通用任务_杀到任务地图操作重复)] = {{{"杀到房间","Shavronne's Sorrow"},{"重新打开","The Black Heart"}}}
        },
        [任务状态("Defeat the spirit of Maligaro")] = {
            [做(通用任务_杀到任务地图操作重复)] = {{{"杀到房间","Maligaro's Misery"},{"重新打开","The Black Heart"}}}
        },
        [任务状态("Talk to Sin")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Sin"}}}
        },
        [任务状态("Kill the Depraved Trinity")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"重新打开","The Black Heart"}}}
        },
        [任务状态("Talk to Lilly")] = {  --转到第十章
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Lilly Roth"}}}
        },

    }),
        
--    任务("The Ruler of Highgate",{  
--        [任务状态("Find Oyun and save her from Kira")] = {
--            做(通用任务_杀到任务地图操作)
--        },
--    }),

---------------------第十章------------------
    任务("Safe Passage",{  
        [任务状态("Find Bannon on the Cathedral Rooftop and save him")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"杀死","Plaguewing"}}}
        },
        [任务状态("Fight off Kitava's cultists to rescue Bannon")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"杀死","Plaguewing"}}}
        },
        [任务状态("Defeat Plaguewing to rescue Bannon")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"杀死","Plaguewing"}}}
        },
        [任务状态("Talk to Bannon")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"对话","Bannon"}}}
        },
        [任务状态("Meet Bannon back at the docks")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Bannon"}}}
        },
        [任务状态("Talk to Lani for your reward")] = {
            [做(通用任务_提交任务)] = {"Lani","Bannon Reward",1}
        },
    }),

    任务("Death and Rebirth",{  
        [任务状态("Find the Staff of Purity")] = {
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","Cathedral Apex"},{"杀死","Plaguewing"}}}
        },
        [任务状态("Talk to Lani for your reward")] = {
            [做(通用任务_提交任务)] = {"Lani","Bannon Reward",1}
        },
    }),
}



清空任务处理()
解析任务处理(所有的任务)

