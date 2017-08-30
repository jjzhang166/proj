#pragma once
/*
@author zhp
@date 2017/2/3 21:06
@purpose od info
*/
//发包内容过滤
//word[eax] != 0f00 && word[eax] != 0100
//存放移动技能ID的全局变量
//d 100D70C
//InGameState对象
//dd [[[[00FF7168+38]+4]+10]]
//PreGameState对象
//dd [[[[00FF7168+38]+4]+10]]
//InGameState的发包对象
//dd [[[[[[00FF7168+38]+4]+10]]+13c]+3A98]
//InGameState的发包对象的data
//dd [[[[[[[00FF7168+38]+4]+10]]+13c]+3A98]+0c8]
//PreGameState的发包对象
//dd [[[[[[00FF7168+38]+4]+10]]+28]+828]
//PreGameState的发包对象的data
//dd [[[[[[[00FF7168+38]+4]+10]]+28]+828]+0c8]
//stCD_EntityArray
//dd [[[[[[[[00FF7168+38]+4]+10]]+13c]+3A94]+1c4]+4]
//Player
//dd [[[[[[[[[00FF7168+38]+4]+10]]+13c]+3A94]+1c4]+4]+5*4]
//stCD_BagMgr
//d [[[[[00FF7168+38]+4]+10]]+13c]+3CEC
//stCD_PassiveSkill
//d [[[[[00FF7168+38]+4]+10]]+13c]+3AC4
//stCD_GameLogic
//d [[[[[00FF7168+38]+4]+10]]+13c]
//stCD_World
//d [[[[[[00FF7168+38]+4]+10]]+13c]+3A94]
