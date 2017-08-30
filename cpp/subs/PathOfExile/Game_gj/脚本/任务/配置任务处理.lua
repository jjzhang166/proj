
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
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Take the glyphs to the glyph wall")] = { --拿着雕纹到石雕之壁去
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
    任务("The Caged Brute",{
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望  Waypoint(传送点) Lioneye's Watch(狮眼守望)  The Lower Prison(禁灵之狱下层)
             [做(通用任务_杀到任务地图操作)] = {{{"打开","Waypoint"},{"杀到地图","Lioneye's Watch"}},"The Lower Prison"}
        },
        [任务状态("Talk to Nessa")] = {  --对话奈沙  Prison Reward(监狱奖励)
             [做(通用任务_提交任务)]= {"Nessa","Prison Reward",1,"Tarkleigh"},
        },--{"杀到房间","The Warden's Chambers"},
        [任务状态("Explore the prison and Kill Brutus")] = { --探索监狱并击败布鲁特斯  The Warden's Chambers(典狱长的房间)  Brutus, Lord Incarcerator(典狱长布鲁特斯)
             [做(通用任务_杀到任务地图操作)] = {{{"杀到","Brutus, Lord Incarcerator"}},"The Upper Prison"}
        },
        [任务状态("Travel to The Prison")] = { --前往 禁灵之狱  The Upper Prison(禁灵之狱上层)
             [做(通用任务_杀到任务地图操作)] = {{},"The Upper Prison"}
        },
        [任务状态("Kill Brutus")] = { --击败布鲁特斯
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Warden's Chambers"},{"杀死","Brutus, Lord Incarcerator"}},"The Upper Prison"}
        },
        [任务状态("Find the exit from the prison")] = { --找出离开监狱的出口
             做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Tarkleigh for your reward")] = { --与塔格拉交谈以获得奖励  Brutus Reward(击败屠夫的奖赏)
             [做(通用任务_提交任务)]= {"Tarkleigh","Brutus Reward",1}
        },
        [提交(通用任务_提交任务)]= {"Tarkleigh","Brutus Reward",1}
    }),

   --海妖之歌
   任务("The Siren's Cadence",{
        [任务状态("Travel to Lioneye's Watch")] = { --前往 狮眼守望
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Waypoint"},{"杀到地图","Lioneye's Watch"}},"The Cavern of Wrath"}
        },
        [任务状态("Travel to Merveil's Caverns")] = { --击败海妖.莫薇儿
            做(做_击败海妖)
        },
        [任务状态("Find the exit to Act Two")] = { --找出前往第二章的入口  The Southern Forest(南部森林)
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
       [任务状态("Find the forest encampment")] = { --找到森林营地
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
        [任务状态("Find the exit to Act Three")] = {--找到前往第三章的出口--萨恩城废墟
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
            [做(通用任务_杀到任务地图操作)] = {{{"杀死","Fidelitas, the Mourning"}}}
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
        [任务状态("Talk to Helena")] = { --与赫莲娜交谈
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Helena"}}}
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
        [任务状态("Find Maligaro's Spike")] = { --找出【马雷格罗的尖刺】
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Weaver's Nest"},{"打开","Maligaro's Spike"}}}
        },
        [任务状态("Get Maligaro's Spike")] = { --取得【马雷格罗的尖刺】
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Weaver's Nest"},{"打开","Maligaro's Spike"}}}
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
        [任务状态("Find Clarissa and Tolman")] = {-- 注意这个状态名
            做(做_击败守卫并挽救女孩)
        },
        [任务状态("Find Tolman in the Crematorium")] = {--在火葬场找到托尔曼--派蒂--托尔曼
            --[做(通用任务_杀到任务地图操作)] = { { {"杀到","Tolman"},{"打开","Tolman"},{"拾取","任务物品"} },空},
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
   }),

   --余烬之尘
   任务("Fiery Dust",{
        [任务状态("Travel to The Docks")] = {--前往 不朽海港
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Find the Thaumetic Sulphite")] = {--找出托麦迪克亚硫酸--托麦迪克亚硫酸
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Supply Container"},{"打开","Thaumetic Sulphite"}},空}
        },
        [任务状态("Travel to The Solaris Temple")] = {--前往 日耀神殿
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Lady Dialla")] = {--与达拉夫人交谈--取得炼狱之粉
            [做(通用任务_提交任务)]= {"Lady Dialla","Take Infernal Talc",1},
        },
        [任务状态("Travel to The Warehouse Sewers")] = {--前往 仓库地带下水道
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
        [任务状态("Find the platinum bust")] = { --找出半身像 --藏物处(Stash) 这个有问题
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Stash"},{"拾取","任务物品"}},"当前地图"}
        },
        [任务状态("Travel to The Sarn Encampment")] = { --前往 萨恩营地
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
        [任务状态("Travel to The Slums Sewers or The Market Sewers")] = { --前往 前往 贫民窟下水道 或是 市集地带下水道
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
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Tower Key"}},"The Lunaris Temple Level 2"},
        },
        [任务状态("Kill Piety")] = {--击败派蒂q {"杀到房间","Portal"}
            [做(通用任务_杀到任务地图操作)] = {{{"打开","Tower Key"}},"The Lunaris Temple Level 2"},
        },
        [任务状态("Travel to The Sarn Encampment")] = {--前往 萨恩营地
            做(通用任务_杀到任务地图操作)
        },
        [任务状态("Talk to Grigor for your reward")] = {--与葛里戈交谈来获得奖励--击败派蒂的奖励
            [做(通用任务_提交任务)]= {"Grigor","Piety Reward",1},
        },
   }),

   --神权之塔
   任务("Sceptre of God",{
        [任务状态("Travel to The Imperial Gardens")] = {--前往 皇家花园(子地图:树篱迷宫)
            做(通用任务_杀到任务地图操作)
        },
--        [任务状态("Unlock the Sceptre of God")] = {--前往 神权之塔
--            做(通用任务_杀到任务地图操作)
--        },
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
            --[做(通用任务_杀到任务地图操作)] = {{{"杀死","Dominus"}},"The Upper Sceptre of God"}
            [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Boos Pos"}},"The Upper Sceptre of God"}
        },
	    [任务状态("Ascend the Sceptre of God")] = {--登上神权之塔的顶端 {"杀到房间","Portal"}, {"杀到","Dominus, High Templar"}
            [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Boos Pos"},{"杀死","Dominus, Ascendant"}},"The Upper Sceptre of God"}
        },
        [任务状态("Fight your way to Dominus")] = {--击败神主
            [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Boos Pos"},{"杀死","Dominus, Ascendant"}},"The Upper Sceptre of God"}
        },
		[任务状态("Kill Dominus")] = {--击败神主
            [做(通用任务_杀到任务地图操作)] = {{{"杀到位置","Boos Pos"},{"杀死","Dominus, Ascendant"}},"The Upper Sceptre of God"}
        },
        [任务状态("Talk to Dialla")] = {--与达拉夫人交谈
            [做(通用任务_杀到任务地图操作)] = {{{"对话","Lady Dialla"}},"The Upper Sceptre of God"}
        },
        [任务状态("Find the exit to Act Four")] = {--找到前往第四章的出口--水道遗迹
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
        [任务状态("Find Kaom and the Eye of Fury")] = {--找到冈姆与愤怒之眼-- --愤怒之眼 {"杀到房间","Caldera of The King"},
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Eye of Fury"}},"Kaom's Stronghold"}
        },
        [任务状态("Kill Kaom")] = {--击败冈姆--冈姆的堡垒--Caldera of The King熔岩之环 {"杀到房间","Caldera of The King"},
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Eye of Fury"}},"Kaom's Stronghold"}
        },
        [任务状态("Travel to The Crystal Veins")] = {--前往 水晶矿脉  --统治者之殿
            [做(通用任务_杀到任务地图操作)] =  {{{"杀到地图","The Crystal Veins"}},"Highgate"}
        },
        [任务状态("Travel to Kaom's Stronghold")] = {--前往 冈姆的堡垒
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"对话","Lady Dialla"}}}
        },
        [任务状态("Deliver the Eye of Fury to Dialla")] = {--把愤怒之眼交给达拉
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"对话","Lady Dialla"}}}
        },
        [任务状态("Talk to Dialla in town for a reward")] = {--与营地的达拉交谈以获取奖励
            [做(通用任务_提交任务)]= {"Dialla","Rapture Reward",1},
        },
   }),

   --欲望之王
   任务("The King of Desire",{
        [任务状态("Find Daresso and the Eye of Desire")] = {-- 找到德瑞索与欲望之眼--剑刃之环--欲望之眼
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Ring Blades"},{"打开","The Eye of Desire"}}}
        },
        [任务状态("Kill Daresso")] = {--击败德瑞索
            [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Ring Blades"},{"打开","The Eye of Desire"}}}
        },
        [任务状态("Take the Eye of Desire")] = {--取回欲望之眼
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Eye of Desire"}}}
        },
        [任务状态("Travel to The Crystal Veins")] = {--前往 水晶矿脉--统治者之殿
            [做(通用任务_杀到任务地图操作)] = {{{"杀到地图","The Crystal Veins"}},"Highgate"}
        },
        [任务状态("Deliver the Eye of Desire to Dialla")] = {--把欲望之眼交给达拉
            [做(通用任务_杀到任务地图操作)] = {{{"访问","Lady Dialla"},{"对话","Lady Dialla"}}}
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
        [任务状态("Find the Rapture Device")] = {--找到裂界装置--冈姆的幻境
            [做(通用任务_杀到任务地图操作)] = {{{"杀到","Kaom's Dream"},{"对话","Lady Dialla"}}}
        },
        [任务状态("Travel to The Belly of the Beast")] = {--巨兽之腹:第 1 层
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Belly of the Beast Level 1"}},"The Crystal Veins"}  
        },
        [任务状态("Destroy the Beast within")] = {--进入巨兽之腹--兽腹The Bowels of the Beast--派蒂,憎恨梦魇--巨兽之腹:第 2 层
            [做(通用任务_杀到任务地图操作)] = {{{"打开","The Bowels of the Beast"},{"杀死","Piety, the Abomination"}},"The Belly of the Beast Level 2"}
        },
        [任务状态("Kill Piety")] = {  --在小地图房间  这个怪物名字下次注意下 --巨兽之腹:第 2 层
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
        [任务状态("Enter the Black Core")] = {--进入黑灵核心--Malachai, The Nightmare梦魇玛拉凯
             [做(通用任务_杀到任务地图操作)] = {{{"杀到房间","The Black Core"},{"杀死","Malachai, The Nightmare"}}}
        },
        [任务状态("Kill Malachai")] = {  --击败玛拉凯的守卫以进入黑灵核心     -- 这个需要杀三个怪才行 而且状态还不会改变
            做(做_找到进入黑灵核心的路)
            --[做(通用任务_杀到任务地图操作)] = {{{"打开","黑灵核心"}}}
        },
        [任务状态("Kill The Nightmare")] = { --名字没看清请比对   --杀死后 打开黑灵核心
            做(做_击败玛拉凯)
        },
    }),



   ---------------一般 难度  任务结束-------------------
   ----城镇会出现一个 传送门  绝望岩滩(残酷)
}

清空任务处理()
解析任务处理(所有的任务)

