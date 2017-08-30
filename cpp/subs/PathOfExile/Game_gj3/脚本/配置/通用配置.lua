
自动开箱子(开箱子标志.开启 + 开箱子标志.只开显示名字的)
自动捡物品(真)
自动开传送点(真)
自动开门(真)
自动换装(真)
默认升级技能宝石(真)
--设置使用武器类型(武器类型.单手)
设置背包保留大小(2, 3)
设置自动补血补蓝(0.8, 0.3)
设置背包药品数量({{药品类型.生命,3},{药品类型.魔力,1},{药品类型.水银,1}})  --总的数量不能超过5个
--设置重置地图(真)
-------------------------------------------------------------------------------
--注释:
--物品类型: 主动宝石、辅助宝石、暴力药剂、可堆叠通货、地图碎片、异界地图、珠宝、命运卡 ->
--头部、项链、腰带、戒指、鞋子、手套、胸甲、盾、爪、匕首、法杖、单手剑、细剑、单手斧、单手锤、弓、箭袋、长杖、双手剑、双手斧、双手锤、短杖;
--物品颜色: 白色 -> 蓝色 -> 金色 -> 暗金 
--过滤类型: 白名单、 黑名单、 颜色、 物品类型、 品质、 物品数量、 三色连、 宝石槽连接、 宝石槽连接、 绑定状态、 属性、装备、等级;  
--评分类型: 装备颜色、槽子连接、槽子颜色、品质、武器、属性
-------------------------------------------------------------------------
--在白名单拾取  在黑名单不拾取
清除拾取过滤()   
添加拾取过滤(创建过滤(过滤类型.白名单,{
"Lightning Tendrils", --电能释放
"Fireball", --火球
"Arcane Surge Support",
"Binding Shard",
"Engineer's Shard",
"Albino Rhoa Feather","Alchemy Shard","Alteration Shard","Transmutation Shard","Scroll Fragment","Blessed Orb","Divine Orb","Orb of Augmentation","Exalted Orb","Orb of Scouring","Orb of Alteration","Orb of Transmutation","Orb of Chance","Orb of Alchemy","Mirror of Kalandra","Chaos Orb","Orb of Regret","Silver Coin","Perandus Coin","Eternal Orb","Enchant","Prophecy","Blacksmith's Whetstone","Armourer's Scrap","Glassblower's Bauble","Gemcutter's Prism","Cartographer's Chisel","Chromatic Orb","Orb of Fusing","Jeweller's Orb"}))
添加拾取过滤(创建过滤(过滤类型.黑名单,{"毒蛇打击","劈砍","遥控","督军印记","闪现射击","魅影射击","陷阱","霸气之击","横扫","箭矢","遥控地雷","裂地之击","双持打击","重击","双重打击"}))
添加拾取过滤(创建过滤(过滤类型.物品类型,{物品类型.命运卡,物品类型.地图碎片,物品类型.异界地图}))
添加拾取过滤(创建过滤(过滤类型.物品数量,{{"Portal Scroll",40},{"Scroll of Wisdom",40}}))
添加拾取过滤(创建组合过滤({创建过滤(过滤类型.颜色,{物品颜色.暗金}),创建过滤(过滤类型.绑定状态,假)}))   
添加拾取过滤(创建组合过滤({创建过滤(过滤类型.宝石槽连接,{5,6}),创建过滤(过滤类型.宝石槽数量,{5,6}),创建过滤(过滤类型.绑定状态,假)})) 
添加拾取过滤(创建组合过滤({创建过滤(过滤类型.三色连,{}), 创建过滤(过滤类型.绑定状态,假)}))  
添加拾取过滤(创建组合过滤({ 创建过滤(过滤类型.物品类型,{物品类型.主动宝石,物品类型.辅助宝石}),创建过滤(过滤类型.品质,{5,100})}))

----------------------------------------------------------------------------------
--在白名单的存仓, 在黑名单的不存  
清除存仓过滤()
添加存仓过滤(创建过滤(过滤类型.白名单,{"Engineer's Shard","Binding Shard","Albino Rhoa Feather","Alchemy Shard","Alteration Shard","Transmutation Shard","Scroll Fragment","Blessed Orb","Divine Orb","Orb of Augmentation","Exalted Orb","Orb of Scouring","Orb of Alteration","Orb of Transmutation","Orb of Chance","Orb of Alchemy","Mirror of Kalandra","Chaos Orb","Orb of Regret","Silver Coin","Perandus Coin","Eternal Orb","Enchant","Prophecy","Blacksmith's Whetstone","Armourer's Scrap","Glassblower's Bauble","Gemcutter's Prism","Cartographer's Chisel","Chromatic Orb","Orb of Fusing","Jeweller's Orb"}))
添加存仓过滤(创建过滤(过滤类型.黑名单,{"召唤愤怒狂灵","传送卷轴","知识卷轴","近战物理伤害(辅)","近战伤害扩散(辅)","召唤生物速度(辅)","快速施法(辅)","增加暴击几率(辅)","召唤生物速度(辅)","附加冰霜伤害(辅)","附加闪电伤害(辅)"}))
添加存仓过滤(创建组合过滤({创建过滤(过滤类型.宝石槽数量,{6,7}),创建过滤(过滤类型.宝石槽连接,{5,6})} ))
添加存仓过滤(创建过滤(过滤类型.物品类型,{物品类型.地图碎片,物品类型.命运卡,物品类型.异界地图}))
添加存仓过滤(创建组合过滤({创建过滤(过滤类型.颜色,{物品颜色.暗金}),创建过滤(过滤类型.宝石槽连接,{5,6}),创建过滤(过滤类型.绑定状态,假)}))
添加存仓过滤(创建过滤(过滤类型.属性,通用_存仓属性过滤))
添加存仓过滤(创建组合过滤({ 创建过滤(过滤类型.物品类型,{物品类型.主动宝石,物品类型.辅助宝石}),创建过滤(过滤类型.品质,{5,100})}))

---------------------------------------------------------------------------------
--在白名单的出售 在黑名单的不卖 
清除出售过滤() 
添加出售过滤(创建过滤(过滤类型.白名单, {"Fireball","Arcane Surge Support"}))
添加出售过滤(创建过滤(过滤类型.黑名单, {"Albino Rhoa Feather","Alchemy Shard","Alteration Shard","Transmutation Shard","Scroll Fragment","Blessed Orb","Divine Orb","Orb of Augmentation","Exalted Orb","Orb of Scouring","Orb of Alteration","Orb of Transmutation","Orb of Chance","Orb of Alchemy","Mirror of Kalandra","Chaos Orb","Orb of Regret","Silver Coin","Perandus Coin","Eternal Orb","Enchant","Prophecy","Blacksmith's Whetstone","Armourer's Scrap","Glassblower's Bauble","Gemcutter's Prism","Cartographer's Chisel","Chromatic Orb","Orb of Fusing","Jeweller's Orb"}))
添加出售过滤(创建过滤(过滤类型.三色连,{}))
添加出售过滤(创建过滤(过滤类型.装备,{}))

---------------------------------------------------------------------------------------------
设置换装评分(评分类型.装备颜色,{{物品颜色.白色,0},{物品颜色.蓝色,0},{物品颜色.金色,10},{物品颜色.暗金,20}})
设置换装评分(评分类型.槽子连接,{{1,0},{2,2},{3,3},{4,4},{5,30},{6,100}})
设置换装评分(评分类型.槽子颜色,{{插槽颜色.红色,2},{插槽颜色.绿色,0},{插槽颜色.蓝色,2},{插槽颜色.白色,10}})
设置换装评分(评分类型.武器,{{物品类型.单手剑,0},{物品类型.法杖,10},{物品类型.匕首,5}})
设置换装评分(评分类型.属性,通用_物品属性评分)

添加物品获取方式("Portal Scroll","Lioneye's Watch","Nessa")
--添加物品获取方式("僵尸复苏","狮眼守望","奈莎(技能杂货)")
添加物品获取方式("Raise Zombie","Lioneye's Watch","Nessa")
--添加物品获取方式("近战伤害扩散","狮眼守望","奈莎(技能杂货)")
添加物品获取方式("Melee Splash Support","Lioneye's Watch","Nessa")
--添加物品获取方式("召唤愤怒狂灵","狮眼守望","奈莎(技能杂货)")
添加物品获取方式("Summon Raging Spirit","Lioneye's Watch","Nessa")
--添加物品获取方式("血肉奉献","狮眼守望","奈莎(技能杂货)")
添加物品获取方式("Flesh Offering","Lioneye's Watch","Nessa")
--添加物品获取方式("召唤生物伤害(辅)","狮眼守望","奈莎(技能杂货)")
添加物品获取方式("Minion Damage Support","Lioneye's Watch","Nessa")
--添加物品获取方式("近战物理伤害(辅)","XXX","伊娜(技能杂货)")
添加物品获取方式("Melee Physical Damage Support","The Forest Encampment","Yeena")
--添加物品获取方式("召唤生物生命(辅)","xxx","伊娜(技能杂货)")
添加物品获取方式("Minion Life Support","The Forest Encampment","Yeena")
--添加物品获取方式("精准破坏(辅)","xxx","伊娜(技能杂货)")
添加物品获取方式("Controlled Destruction Support","The Forest Encampment","Yeena")
--添加物品获取方式("憎恨","XXX","卡尔莉萨")
添加物品获取方式("Hatred","The Sarn Encampment","Clarissa")
--添加物品获取方式("和善(辅)","XXX","卡尔莉萨")
添加物品获取方式("Generosity Support","The Sarn Encampment","Clarissa")
--添加物品获取方式("衰弱","XXX","卡尔莉萨")    
添加物品获取方式("Enfeeble","The Sarn Encampment","Clarissa")
--添加物品获取方式("施法回响(辅)","XXX","佩塔卢斯和芙安珈(技能杂货)")        
添加物品获取方式("Spell Echo Support","Highgate","Petarus and Vanja")
--添加物品获取方式("受伤时施放(辅)","XXX","佩塔卢斯和芙安珈(技能杂货)")     
添加物品获取方式("Cast when Damage Taken Support","Highgate","Petarus and Vanja")
--添加物品获取方式("召唤烈焰魔像","XXX","佩塔卢斯和芙安珈(技能杂货)) 
添加物品获取方式("Summon Flame Golem","Highgate","Petarus and Vanja")
--添加物品获取方式("高阶多重投射(辅)","XXX","佩塔卢斯和芙安珈(技能杂货)) 
添加物品获取方式("Greater Multiple Projectiles Support","Highgate","Petarus and Vanja")

----------------------------------------------------------------------------
清空技能宝石升级信息()
添加技能宝石升级信息("Cast when Damage Taken Support", 5) --受伤时施放
添加技能宝石升级信息("Enfeeble", 5)                       --衰弱
添加技能宝石升级信息("Flesh Offering", 7)                 --血肉奉献
--添加技能宝石升级信息("Elemental Weakness", 5)           --元素要害

------------------------------------------------------------------------------------------------
--参数是屏幕比率，危险距离=屏幕比率*整个屏幕所占距离
--全局危险距离影响打开目标等命令，比如开箱子的时候，有怪进入到危险距离，便会去杀怪。
设置全局危险距离(0.5)
清空技能()
--施法距离，第1个参数是危险距离屏幕比率，第2个参数是技能距离屏幕比率，危险距离必须小于等于技能距离。
--憎恨
添加技能(创建技能("Hatred"):释放间隔(4):技能CD(0.05):直接施放():优先级(11))
--纪律
添加技能(创建技能("Discipline"):释放间隔(3):技能CD(0.05):直接施放():优先级(11))
--召唤愤怒狂灵
添加技能(创建技能("Summon Raging Spirit"):释放间隔(0):技能CD(3):血值区间(0.1, 1):蓝值区间(0.1, 1):施法距离(0.1, 0.6):优先级(10):边逃边杀()
    :增加前置技能({"Summon Flame Golem","Raise Zombie", "Flesh Offering"})
    :增加后置技能({"Discipline","Raise Zombie", "Hatred","Clarity"})
)
--电能释放
添加技能(创建技能("Lightning Tendrils"):释放间隔(0):技能CD(0.3):施法距离(0.15, 0.2):边逃边杀():优先级(900))
--火球
添加技能(创建技能("Fireball"):释放间隔(0):技能CD(0.3):施法距离(0.15, 0.2):边逃边杀():优先级(900))
--普通攻击
添加技能(创建技能("Default Attack"):释放间隔(0):技能CD(0.3):血值区间(0, 1):蓝值区间(0, 1):施法距离(0.1, 0.15):边逃边杀():优先级(1000))
--僵尸复苏
添加技能(创建尸体技能("Raise Zombie"):释放间隔(3):技能CD(0.05):施法距离(0.3, 0.5):优先级(2000):边逃边杀())
--血肉奉献
添加技能(创建尸体技能("Flesh Offering"):释放间隔(5):技能CD(0.05):施法距离(0.3, 0.5):优先级(2000):边逃边杀())
--召唤烈焰魔像
添加技能(创建技能("Summon Flame Golem"):释放间隔(3):技能CD(0.05):施法距离(0.3, 0.5):优先级(2000):边逃边杀())
--清晰
添加技能(创建技能("Clarity"):释放间隔(3):技能CD(0.05):直接施放():优先级(11))

----------------------------------------------------------------------------------------------
----假如匹配类型有重复字节，描述长的放在描述短的前边边。例如 法术暴击率 与 攻击和法术暴击率  后置都为 法术暴击率 ，注意物品属性解析的前后顺序
----属性解析
清空物品属性解析()
物品属性解析("全抗性"):匹配("+"):捕获(浮点类型()):匹配("to all Elemental Resistances")
物品属性解析("火焰抗性"):匹配("+"):捕获(浮点类型()):匹配("Fire Resistance")
物品属性解析("冰霜抗性"):匹配("+"):捕获(浮点类型()):匹配("Cold Resistance")
物品属性解析("闪电抗性"):匹配("+"):捕获(浮点类型()):匹配("Lightning Resistance")
物品属性解析("混沌抗性"):匹配("+"):捕获(浮点类型()):匹配(" to Chaos Resistance ")
物品属性解析("生命"):匹配("+"):捕获(浮点类型()):匹配("to maximum Life")
物品属性解析("生命百分比"):捕获(浮点类型()):匹配("increased maximum Life")  
物品属性解析("移动速度"):捕获(浮点类型()):匹配("increased Movement Speed")
物品属性解析("护盾"):匹配("+"):捕获(浮点类型()):匹配("to maximum Energy Shield")
物品属性解析("护盾百分比"):捕获(浮点类型()):匹配("increased Energy Shield")
物品属性解析("攻击速度1"):捕获(浮点类型()):匹配("increased Attack Speed")
物品属性解析("攻击速度2"):捕获(浮点类型()):匹配("increased Attack Speed")
物品属性解析("法术附加闪电伤害"):匹配("Adds"):捕获(浮点类型()):匹配("to"):捕获(浮点类型()):匹配("Lightning Damage  to Spells")
物品属性解析("法术附加火焰伤害"):匹配("Adds"):捕获(浮点类型()):匹配("to"):捕获(浮点类型()):匹配("Fire Damage  to Spells")
物品属性解析("法术附加冰霜伤害"):匹配("Adds"):捕获(浮点类型()):匹配("to"):捕获(浮点类型()):匹配("Cold Damage  to Spells")
物品属性解析("法术附加混沌伤害"):匹配("Adds"):捕获(浮点类型()):匹配("to"):捕获(浮点类型()):匹配(" Chaos Damage to Spells")
物品属性解析("施法速度"):匹配("增加"):捕获(浮点类型()):匹配("increased Cast Speed")
物品属性解析("增加闪电伤害"):捕获(浮点类型()):匹配("increased Lightning Damage")
物品属性解析("增加火焰伤害"):匹配("增加"):捕获(浮点类型()):匹配("increased Fire Damage ")
物品属性解析("增加冰霜伤害"):匹配("增加"):捕获(浮点类型()):匹配("increased Cold Damage")
物品属性解析("攻击和法术暴击率"):捕获(浮点类型()):匹配("increased Global Critical Strike Chance")
物品属性解析("命中值"):匹配("+"):捕获(浮点类型()):匹配("to Accuracy Rating")
物品属性解析("增加法术伤害"):匹配("+"):捕获(浮点类型()):匹配("increased Spell Damage")
物品属性解析("法术暴击率"):捕获(浮点类型()):匹配("increased Critical Strike Chance for Spells")
物品属性解析("增加武器暴击率"):捕获(浮点类型()):匹配("increased Critical Strike Chance")
物品属性解析("宝石等级"):匹配("+"):捕获(浮点类型()):匹配("to Level of Socketed Gems ")
物品属性解析("火焰宝石等级"):匹配("+"):捕获(浮点类型()):匹配("to Level of Socketed Fire Gems")
物品属性解析("攻击和施法速度"):捕获(浮点类型()):匹配("increased Attack and Cast Speed")
物品属性解析("增加全属性伤害"):捕获(浮点类型()):匹配("(increased Elemental Damage ")
物品属性解析("物品稀有度"):捕获(浮点类型()):匹配("increased Rarity of Items found")
物品属性解析("物品掉落数量"):捕获(浮点类型()):匹配("increased Quantity of Items found")
物品属性解析("全能力"):匹配("+"):捕获(浮点类型()):匹配("to all Attributes")

----------------------------------------------------------------------------------

配置天赋树({
 --女巫
    ["Witch"] = 
    {
        --此处是 大天赋节点，如果不配置相应的 大天赋节点，那么下面的 小天赋节点 配置再多，也都没有用。"Jewel Socket"为珠宝插槽我放置在“大点”里，出问题请删除
        { "Deep Wisdom","Heart and soul","Lord of the Dead","Occultist's Dominion","Quick Recovery","Practical Application","Elemental Equilibrium","Gravepact","Potency of Will","Jewel Socket","Deep Thoughts","Cruel Preparation","Death Attunement","Retribution","Precision","Discipline and Training","Sanctity","Righteous Decree","Shaper","Constitution","Sentinel","Exceptional Performance","Dynamo","Tireless","Born to Fight","Heart of the Warrior","Versatility","Warrior's Blood","Profane Chemistry","Unwavering Stance"},
        --此处是 小天赋节点。控制连接到 大天赋节点 的走向。此处可为空。
        { "Energy shield and mana Regeneration","Mana and Mana Regeneration","Mana and Life" ,"Minion Life","Mnion Damage","Intelligence","Strength","Intelligence","Avoid Interruption while Casting","Intelligence","Intelligence","Minion Damage","Minion Attack and Cast SPeed","Minion Attack and Cast SPeed","Skill Duration","Skill Duration","Intelligence","Intelligence","Mana","Life","Life","Intelligence","Intelligence","Ninion Life","Minion Damage and Life Leech","Minion Damage","Dexterity","Intelligence","Attack and Cast Speed,Accuracy","Life","Life","Mana and Reduced Mana Cost","Strength","Strength","Life","Life","Strength","Strength","Dexterity","Skill Duration","Skill Duration","Mana and Reduced Mana Cost","Strength","Strength","Strength","Strength","Strength","Life and Reduced Mana Cost","Life and Reduced Mana Cost","Life and Reduced Mana Cost","Intelligence","Strength","Life","Accuracy and Reduced Mana Cost","Life","Dexterity","Life and Flasks","Life and Flasks","Strength","Strength","Strength","Strength"} 
    },
--贵族
    ["Scion"] = 
    {
        { "Shaper" },
        { "Mana Regeneration and Intelligence" } 
    },
--野蛮人
	["Manrauder"] = 
    {
        { "Born to Fight", },
        { "Strength" } 
    },
--游侠
	["Ranger"] = 
    {
        { "Ballistic Mastery", },
        { "Projectile Damage" } 
    },
--决斗者
	["Duelist"] = 
    {
        { "Art of the Gladiator", },
        { "Dexterity" } 
    },
--圣堂武僧
	["Templar"] = 
    {
        { "Sanctity", },
        { "Mana Regeneration and Life" } 
    },
--暗影刺客
	["Shadow"] = 
    {
        { "Trickery", },
        { "Cast Speed" } 
    },
})

--参数1:参数2 等级区间需要连接的宝石
清理宝石连接()
--召唤愤怒狂灵，近战物理伤害(辅)，近战伤害扩散(辅)，施法回响(辅)，多重打击(辅)，召唤生物伤害(辅) ,快速施法(辅)
设置宝石连接(4,100,"Summon Raging Spirit",{"Melee Physical Damage Support","Melee Splash Support","Spell Echo Support","Minion Damage Support","Faster Casting Support","Added Lightning Damage Support"})
--僵尸复苏，近战物理伤害(辅)，近战伤害扩散(辅),召唤生物生命(辅)
设置宝石连接(4,100,"Raise Zombie",{"Melee Splash Support","Minion Life Support","Melee Physical Damage Support"})
--血肉奉献,受伤时施放(辅),衰弱,
设置宝石连接(4,100,"Flesh Offering",{"Cast when Damage Taken Support","Enfeeble",})
--召唤烈焰魔像，高阶多重投射(辅)，精准破坏(辅)
设置宝石连接(38,100,"Summon Flame Golem",{"Greater Multiple Projectiles Support","Controlled Destruction Support","Minion Damage Support"})
--电能释放
设置宝石连接(0,20,"Lightning Tendrils",{})
--火球
设置宝石连接(0,15,"Fireball",{"Arcane Surge Support"})
--憎恨--和善
设置宝石连接(24,100,"Hatred",{"Generosity Support"})
--清晰
设置宝石连接(0,100,"Clarity",{})

-------------------------------------
清除洗练过滤()
添加洗练过滤(创建过滤(过滤类型.物品类型,{物品类型.胸甲}))
添加洗练过滤(创建组合过滤({创建过滤(过滤类型.物品类型,{物品类型.胸甲}),创建过滤(过滤类型.品质,{5,100})}))
添加洗练过滤(创建组合过滤({创建过滤(过滤类型.物品类型,{物品类型.胸甲}),创建过滤(过滤类型.品质,{5,100}),创建过滤(过滤类型.等级,{30,100})}))

--注释:
--满足一个条件即成功！例如:装备颜色({物品颜色.金色}) 但达到金色或者暗金颜色该项成功，其他项类同；要洗练的装备材料都足够的时候才会洗练；材料务必填写正确，可能会出现未知错误。如果在城镇会从仓库中寻找材料洗练！如果在副本中只会寻找背包中的材料。
--设置洗练装备:参数1:物品类型、参数2和参数2:物品等级区间
--装备颜色:参数1:物品颜色、参数2:消耗材料、参数3:消耗材料上限(材料足够才会洗练)
--槽子数量:参数1:槽子数量、参数2:消耗材料、参数3:消耗材料上限(材料足够才会洗练)
--槽子连接:参数1:槽子连接数、参数2:消耗材料、参数3:消耗材料上限(材料足够才会洗练)
--插槽颜色:参数1:连接的插槽颜色(可以多个，最好不要大于槽子连接数量，否则会消耗材料至上限)  参数2:消耗材料、参数3:消耗材料上限(材料足够才会洗练)
设置洗练装备(物品类型.胸甲,30, 50):装备颜色({物品颜色.金色,"Orb of Alchemy"},2):槽子数量({4,"Jeweller's Orb"},1):槽子连接({4,"Orb of Fusing"},2):插槽颜色({{插槽颜色.红色,插槽颜色.红色,插槽颜色.蓝色,插槽颜色.蓝色},"Chromatic Orb"},5)
设置洗练装备(物品类型.胸甲,51, 60):装备颜色({物品颜色.金色,"Orb of Alchemy"},2):槽子数量({3,"Jeweller's Orb"},1):槽子连接({3,"Orb of Fusing"},2):插槽颜色({{插槽颜色.红色,插槽颜色.红色,插槽颜色.红色},"Chromatic Orb"},5)
设置洗练装备(物品类型.鞋子,40, 60):装备颜色({物品颜色.金色,"Orb of Alchemy"},2):槽子数量({3,"Jeweller's Orb"},1):槽子连接({3,"Orb of Fusing"},2):插槽颜色({{插槽颜色.红色,插槽颜色.红色,插槽颜色.红色},"Chromatic Orb"},5)

-----------------------------------
设置默认创建角色的职业("女巫")
设置默认创建角色的区("Legacy")
设置最大角色数量(5)
-----------------------------------