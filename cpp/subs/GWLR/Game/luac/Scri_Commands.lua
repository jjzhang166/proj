
function 延迟( 毫秒 )
	if type(毫秒) ~= "number" then
		LuaLogE("延迟之参数必须是数字类型")
		return false
	end
	RunInfo("延迟，时间是：" .. tostring(毫秒))
	return Delay(毫秒);
end

--[[
作用：添加一项任务处理
参数1：任务的名称，必须是字符串类型，不能为空
参数2：接任务的处理函数，去接可以接受的任务。必须是一个函数或可调用的类型，可为空
参数3：做任务的处理函数，去做已经接到的任务。必须是一个函数或可调用的类型，可为空
参数4：完成任务的处理函数，去完成或提交一个已经达成的任务。必须是一个函数或可调用的类型，可为空
参数5：自定义的参数，用来传递给接任务的处理函数、做任务的处理函数、完成任务的处理函数，可用于实现某些特殊需求。任意类型，可为空
参数6：任务ID，有的任务存在同名的任务，所以只能用任务ID来唯一确定一个任务。数字类型，可为空。
注意事项：接任务的处理函数、做任务的处理函数、完成任务的处理函数，三者不能同时为空，有同样的参数：参数1是任务ID，参数2是任务名，参数3是自定义的参数
]]
function 添加任务处理( task_name, accept_handler, do_handler, done_handler, unknown_param, task_id )
	g_is_out_to_console = false
	RunInfo("添加任务处理,任务名是:" .. task_name)
	g_is_out_to_console = true
	return TH_Mgr:AddItem(task_name, accept_handler, do_handler, done_handler, unknown_param, task_id)
end

function 接一个可接的任务()
	RunInfo("接一个可接的任务")
	return 安全执行之跳过异常(SafeSkipRun_AutoAcceptOne, false);
end

function 做一个未完成的任务()
	RunInfo("做一个未完成的任务")
	return 安全执行之跳过异常(SafeSkipRun_AutoDoOne, false);
end

function 提交一个已完成的任务()
	RunInfo("提交一个已完成的任务")
	return 安全执行之跳过异常(SafeSkipRun_AutoDoneOne, false);
end

function 任务处理中的任务都处理完了()
	RunInfo("任务处理中的任务都处理完了")
	return not HasUnHandledAtTaskHandles()
end

function 清空任务处理()
	RunInfo("清空任务处理")
	return TH_Mgr:ClearAll();
end

--[[
第一个参数是对象名，字符串类型，不可为空
第二个参数是对象标题，字符串类型，可以为空
]]
function 查找NPC( obj_name, obj_title )
	local the_str = ""
	if type(obj_title) == "string" then
		the_str = " 对象标题：" .. obj_title
	end
	RunInfo("查找NPC，npc：" .. obj_name .. the_str)
	return scmd():FindNpcByName(obj_name, obj_title)
end

function 查找指定名字的可打的对象( obj_name )
	RunInfo("查找指定名字的可打的对象，游戏对象名：" .. obj_name)
	return scmd():FindBeatableMonsterByName(obj_name)
end

function 查找BOSS()
	RunInfo("查找BOSS")
	return scmd():FindTheBoss()
end

function 等待出现BOSS( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待出现BOSS之等待时间必须是数字类型")
		return false
	end
	RunInfo("等待出现BOSS，等待时间：" .. tostring(wait_time))
	return scmd():WaitAppearBoss(wait_time)
end

function 等待出现NPC( obj_name, wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待出现NPC之等待时间必须是数字类型")
		return false
	end
	if type(obj_name) ~= "string" then
		LuaLogE("等待出现NPC之对象名必须是字符串类型")
		return false
	end
	RunInfo("等待出现NPC，对象名：" .. obj_name .. " 等待时间：" .. tostring(wait_time))
	return scmd():WaitAppearTheNpc(obj_name, wait_time)
end

function 等待出现可打的怪物( obj_name, wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待出现可打的怪物之等待时间必须是数字类型")
		return false
	end
	if type(obj_name) ~= "string" then
		LuaLogE("等待出现可打的怪物之对象名必须是字符串类型")
		return false
	end
	RunInfo("等待出现可打的怪物，对象名：" .. obj_name .. " 等待时间：" .. tostring(wait_time))
	return scmd():WaitAppearBeatableMonster(obj_name, wait_time)
end

function 获取角色当前房间号()
	RunInfo("获取角色当前房间号")
	return scmd():GetCurRoleRoom()
end

--参数是房间号，数字类型，可以为空，表示角色所在房间号，也可为-1，表示所有房间
function 优先查找最近的普通怪物对象( the_room_num )
	if type(the_room_num) ~= "number" then
		the_room_num = scmd():GetCurRoleRoom()
	end
	RunInfo("优先查找最近的普通怪物对象，房间号：" .. tostring(the_room_num))
	return scmd():FindNearestNormalMonsterFirst(the_room_num)
end

--只有一个参数，是游戏对象，不能为空
function 是有效的对象( obj )
	RunInfo("是有效的对象")
	return scmd():IsValidGameObj(obj)
end

--只有一个参数，参数是游戏对象的类型，该类型可通过 查找指定名字的对象 等函数获取得到
function 移动到指定的对象身后( obj, steps )
	if obj == nil then
		LuaLogE("移动到指定的对象身后之参数必须不能为空");
		return false;
	end
	if type(steps) ~= "number" then
		steps = 1
	end
	RunInfo("移动到指定的对象身后")
	return scmd():SetRoleToBack(obj, steps)
end

function 移动到指定的对象身前( obj, steps )
	if obj == nil then
		LuaLogE("移动到指定的对象身前之参数必须不能为空");
		return false;
	end
	if type(steps) ~= "number" then
		steps = -1
	end
	RunInfo("移动到指定的对象身前")
	return scmd():SetRoleToBack(obj, steps)
end

--只有一个参数，参数是游戏对象的类型，该类型可通过 查找指定名字的对象 等函数获取得到
function 打开NPC面板( obj )
	if obj == nil then
		LuaLogE("打开NPC面板之参数必须不能为空");
		return false;
	end
	RunInfo("打开NPC面板")
	return scmd():OpenNpc(obj)
end

function 打开NPC格子面板( obj )
	if obj == nil then
		LuaLogE("打开NPC格子面板之参数必须不能为空");
		return false;
	end
	RunInfo("打开NPC格子面板")
	return scmd():OpenNpc_Grid(obj)
end

--只有一个参数，参数是数字类型，为任务ID
function 接受指定ID的任务( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("接受指定ID的任务之参数必须是数字类型")
		return false
	end
	RunInfo("接受指定ID的任务，任务ID：" .. tostring(task_id))
	return scmd():AcceptTaskById(task_id)
end

function 放弃指定ID的任务( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("放弃指定ID的任务之参数必须是数字类型")
		return false
	end
	RunInfo("放弃指定ID的任务，任务ID：" .. tostring(task_id))
	return scmd():GiveUpTask(task_id)
end

--只有一个参数，参数是数字类型，为任务ID
function 提交指定ID的任务( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("提交指定ID的任务之参数必须是数字类型")
		return false
	end
	RunInfo("提交指定ID的任务，任务ID：" .. tostring(task_id))
	return scmd():CommitTaskById(task_id)
end

function 进行NPC对话( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("进行NPC对话之参数必须是数字类型")
		return false
	end
	RunInfo("进行NPC对话，任务ID：" .. tostring(task_id))
	return scmd():DoNpcChat(task_id)
end

--只有一个参数，参数是字符串类型，为村庄名
function 移动到指定的村庄( village_name, room_num )
	if type(village_name) ~= "string" then
		LuaLogE("移动到指定的村庄之参数必须是字符串类型")
		return false
	end
	if type(room_num) ~= "number" then
		room_num = 0
	end
	RunInfo("移动到指定的村庄，村庄名：" .. village_name .. " 房间号：" .. tostring(room_num))
	return scmd():MoveToTheVillage(village_name, room_num);
end

--第一个参数是副本名，必须是字符串类型
function 移动到指定的副本( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("移动到指定的副本之副本名参数必须是字符串类型")
		return false
	end
	RunInfo("移动到指定的副本，副本名：" .. fb_name)
	return scmd():MoveToTheFb(fb_name);
end

function 移动到指定的副本_ID( fb_id )
	if type(fb_id) ~= "number" then
		LuaLogE("移动到指定的副本_ID之副本ID参数必须是数字类型")
		return false
	end
	RunInfo("移动到指定的副本_ID，副本ID：" .. fb_id)
	return scmd():MoveToTheFbById(fb_id);
end

function 移动到指定的喵喵副本( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("移动到指定的喵喵副本之副本名参数必须是字符串类型")
		return false
	end
	RunInfo("移动到指定的喵喵副本，副本名：" .. fb_name)
	return scmd():MoveToTheFb_Cat(fb_name);
end

function 移动到指定的王立副本( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("移动到指定的王立副本之副本名参数必须是字符串类型")
		return false
	end
	RunInfo("移动到指定的王立副本，副本名：" .. fb_name)
	return scmd():MoveToTheFb_King(fb_name);
end

function 移动到指定的王立精英副本( fb_name, unlock_item_name )
	if type(fb_name) ~= "string" then
		LuaLogE("移动到指定的王立精英副本之副本名参数必须是字符串类型")
		return -1
	end
	if type(unlock_item_name) ~= "string" then
		unlock_item_name = ""
	end
	RunInfo("移动到指定的王立精英副本，副本名：" .. fb_name .. " 解锁物品名：" .. unlock_item_name)
	return scmd():MoveToTheFb_King_Cream(fb_name, unlock_item_name)
end

function 可做指定的喵喵副本( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("可做指定的喵喵副本之副本名参数必须是字符串类型")
		return false
	end
	RunInfo("可做指定的喵喵副本，副本名：" .. fb_name)
	return scmd():CanDoTheFb_Cat(fb_name);
end

function 添加喵喵副本黑名单( fb_id )
	if type(fb_id) ~= "number" then
		LuaLogE("添加喵喵副本黑名单之副本ID参数必须是数字类型")
		return
	end
	RunInfo("添加喵喵副本黑名单，副本ID:" .. tostring(fb_id))
	return scmd():AddFbBlackList_Cat(fb_id)
end

function 清空喵喵副本黑名单()
	RunInfo("清空喵喵副本黑名单")
	return scmd():ClrFbBlackList_Cat()
end

function 添加副本黑名单( fb_id )
	if type(fb_id) ~= "number" then
		LuaLogE("添加副本黑名单之副本ID参数必须是数字类型")
		return
	end
	RunInfo("添加副本黑名单，副本ID:" .. tostring(fb_id))
	return scmd():AddFbBlackList(fb_id)
end

function 清空副本黑名单()
	RunInfo("清空副本黑名单")
	return scmd():ClrFbBlackList()
end


function 放弃关卡()
	RunInfo("放弃关卡")
	return scmd():GiveUpFb();
end

function 退出新手训练()
	RunInfo("退出新手训练")
	return scmd():GiveUpXinShouXunLian();
end

function 移动到指定的房间( room_num )
	if type(room_num) ~= "number" then
		LuaLogE("移动到指定的房间之房间序号参数必须是数字类型")
		return false
	end
	RunInfo("移动到指定的房间，房间序号是：" .. tostring(room_num))
	return scmd():MoveToTheRoom(room_num);
end

--[[
参数是物品名，字符串类型，不能为空
]]
function 获取补给箱物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("获取补给箱物品之物品名参数必须是字符串类型")
		return 0
	end
	RunInfo("获取补给箱物品，物品名：" .. item_name)
	return scmd():GainSupplyItem(item_name);
end

--[[
第一个参数是怪物对象，可从 查找指定名字的对象 等函数返回，不能为空
第二个参数是攻击的间隔，单位是毫秒，可以为0，表示没有间隔，数字类型，不能为空
返回攻击次数
]]
function 攻击指定的怪物直到死亡( obj, sleep_time, is_any_direction )
	if type(sleep_time) ~= "number" then
		LuaLogE("攻击指定的怪物直到死亡之间隔时间参数必须是数字类型")
		return 0
	end
	if type(is_any_direction) ~= "boolean" then
		is_any_direction = false
	end
	RunInfo("攻击指定的怪物直到死亡，间隔时间：" .. tostring(sleep_time) .. "毫秒")
	return scmd():HitTheMonsterToDie(obj, sleep_time, is_any_direction, true)
end

function 设置攻击模式_使用背包物品( item_name, steps )
	if type(item_name) ~= "string" then
		LuaLogE("设置攻击模式_使用背包物品之物品名必须是字符串类型");
		return false
	end
	if type(steps) ~= "number" then
		steps = 1
	end
	RunInfo("设置攻击模式_使用背包物品，物品名：" .. item_name)
	return scmd():SetHitMode_UsePacketItem(item_name, steps)
end

--设置的攻击模式只是此次有效，当执行完某个攻击类的脚本命令后，便会恢复到默认的攻击模式
--参数是攻击模式名，支持：右键攻击
function 设置此次攻击模式( mode_name )
	if type(mode_name) ~= "string" then
		LuaLogE("设置此次攻击模式之参数必须是字符串类型")
		return false
	end
	RunInfo("设置此次攻击模式，模式名：" .. mode_name)
	return scmd():SetCurTimeHitMode(mode_name)
end

--第一个参数是攻击的间隔，单位是毫秒，可以为0，表示没有间隔，数字类型，不能为空
--第二个参数是 是否剥皮，如果为真，则剥皮每个打死的怪物
--返回杀死的怪物的数量
function 杀光本房间怪物( sleep_time, is_collect )
	if type(sleep_time) ~= "number" then
		LuaLogE("杀光本房间怪物之间隔时间参数必须是数字类型")
		return 0
	end
	if type(is_collect) ~= "boolean" then
		is_collect = false
	end
	RunInfo("杀光本房间怪物，每次打击怪物的时间间隔是：" .. tostring(sleep_time) .. "  是否要剥皮：" .. tostring(is_collect))
	return scmd():KillOverTheRoomMonsters(sleep_time, is_collect, scmd():GetCurRoleRoom())
end

--第一个参数是攻击的间隔，单位是毫秒，可以为0，表示没有间隔，数字类型，不能为空
--第二个参数是 是否剥皮，如果为真，则剥皮每个打死的怪物
--第三个参数是 房间号，如果为空，表示角色所在房间，-1表示所有房间
--返回杀死的怪物的数量
function 杀光指定房间中所有怪物( sleep_time, is_collect, room_num )
	if type(sleep_time) ~= "number" then
		LuaLogE("杀光指定房间中所有怪物之间隔时间参数必须是数字类型")
		return 0
	end
	if type(is_collect) ~= "boolean" then
		is_collect = false
	end
	if type(room_num) ~= "number" then
		room_num = scmd():GetCurRoleRoom()
	end
	RunInfo("杀光指定房间中所有怪物，每次打击怪物的时间间隔是：" .. tostring(sleep_time) .. "  是否要剥皮：" .. tostring(is_collect) .. "  房间号：" .. tostring(room_num))
	return scmd():KillOverTheRoomMonsters(sleep_time, is_collect, room_num)
end

function 开启当前执行信息()
	g_is_start_run_info = true
end

function 关闭当前执行信息()
	g_is_start_run_info = false
end

function 开启输出信息到控制台()
	g_is_out_to_console = true
end

function 关闭输出信息到控制台()
	g_is_out_to_console = false
end

--只有一个参数，采集等待时间，数字类型，可为0，表示不等待，不能为空
function 自动采集一次最近的对象( collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("自动采集一次最近的对象之采集等待时间必须是数字类型");
		return false
	end
	RunInfo("自动采集一次最近的对象，采集等待时间：" .. tostring(collect_time))
	return scmd():AutoCollectNearestCollectableObjAsSameRoom(collect_time)
end

function 自动采集完最近的对象( collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("自动采集完最近的对象之采集等待时间必须是数字类型");
		return false
	end
	RunInfo("自动采集完最近的对象，采集等待时间：" .. tostring(collect_time))
	return scmd():AutoCollectOverNearestCollectableObjAsSameRoom(collect_time)
end

--只有一个参数，采集等待时间，数字类型，可为0，表示不等待，不能为空
function 自动采集一次房间所有对象( collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("自动采集一次房间所有对象之采集等待时间必须是数字类型");
		return false
	end
	RunInfo("自动采集一次房间所有对象，采集等待时间：" .. tostring(collect_time))
	return scmd():AutoCollectOnceAllRoomCollectableObj(collect_time)
end

--只有一个参数，采集等待时间，数字类型，可为0，表示不等待，不能为空
function 自动采集完房间所有对象( collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("自动采集完房间所有对象之采集等待时间必须是数字类型");
		return 0
	end
	RunInfo("自动采集完房间所有对象，采集等待时间：" .. tostring(collect_time))
	return scmd():AutoCollectOverAllObj(collect_time)
end

--[[
这个函数允许采集指定的房间或所有的房间；允许设置采集的对象的最大数量，和对每个对象采集的最大次数；允许只采集特定名字的对象
第一个参数是每次采集等待时间，数字类型，不可为空
第二个参数是房间号，表示要在哪个房间进行采集；数字类型，可以为空，为空表示角色所在的房间号，为-1表示在所有房间进行采集
第三个参数是采集对象的最大数量，表示最多采集指定数量的采集对象，数字类型，可以为空，表示没有数量限制；
第四个参数是对每个采集对象采集的最大次数，数字类型，可以为空，表示没有次数限制
第五个参数是采集物的名字，表示只采集具有该名字的对象，数字类型，可以为空，表示不管什么名字的对象都会被采集
第六个参数每次采集后所做的事，函数类型，可为空
]]
function 自动详细采集( collect_time, room_num, obj_cnt, every_collect_cnt, collect_obj_name, gained_item, do_after_every_collect )
	if type(collect_time) ~= "number" then
		LuaLogE("自动详细采集之采集等待时间必须是数字类型");
		return 0
	end
	if room_num == nil then
		room_num = -2
	end
	if type(room_num) ~= "number" then
		LuaLogE("自动详细采集之房间号参数必须是数字类型");
		return 0
	end
	if obj_cnt == nil then
		obj_cnt = -1
	end
	if type(obj_cnt) ~= "number" then
		LuaLogE("自动详细采集之要采集的对象的最大数量参数必须是数字类型");
		return 0
	end
	if every_collect_cnt == nil then
		every_collect_cnt = -1
	end
	if type(every_collect_cnt) ~= "number" then
		LuaLogE("自动详细采集之对每个对象采集最大次数的参数必须是数字类型");
		return 0
	end
	if collect_obj_name == nil then
		collect_obj_name = ""
	end
	if type(collect_obj_name) ~= "string" then
		LuaLogE("自动详细采集之对指定的采集对象的名字的参数必须是字符串类型");
		return 0
	end
	if type(gained_item) ~= "string" then
		gained_item = ""
	end
	if type(do_after_every_collect) ~= "function" then
		do_after_every_collect = nil
	end
	RunInfo("自动详细采集，采集等待时间：" .. tostring(collect_time) .. " 房间号：" .. tostring(room_num) .. "  采集对象最大数量：" .. tostring(obj_cnt) .. " 每个对象的最大采集次数：" .. tostring(every_collect_cnt) .. " 采集对象名：" .. collect_obj_name
		.. " 想要采集得到的物品名：" .. gained_item)
	return scmd():AutoCollectSpecifc(collect_time, room_num, obj_cnt, every_collect_cnt, collect_obj_name, gained_item, do_after_every_collect)
end

--[[
这个函数允许采集指定的房间或所有的房间；允许设置采集的对象的最大数量，和对每个对象采集的最大次数；允许只采集特定名字的对象
第一个参数是每次采集等待时间，数字类型，不可为空
第二个参数是采集对象的最大数量，表示最多采集指定数量的采集对象，数字类型，可以为空，表示没有数量限制；
第三个参数是对每个采集对象采集的最大次数，数字类型，可以为空，表示没有次数限制
第四个参数是采集物的名字，支持的对象名有：矿场、鱼塘、虫栖地、育菇木床、蜂房、草药田1、草药田2、草药田3
]]
function 自动庄园采集( collect_time, obj_cnt, every_collect_cnt, collect_obj_name )
	if type(collect_time) ~= "number" then
		LuaLogE("自动庄园采集之采集等待时间必须是数字类型");
		return 0
	end
	if obj_cnt == nil then
		obj_cnt = -1
	end
	if type(obj_cnt) ~= "number" then
		LuaLogE("自动庄园采集之要采集的对象的最大数量参数必须是数字类型");
		return 0
	end
	if every_collect_cnt == nil then
		every_collect_cnt = -1
	end
	if type(every_collect_cnt) ~= "number" then
		LuaLogE("自动庄园采集之对每个对象采集最大次数的参数必须是数字类型");
		return 0
	end
	if collect_obj_name == nil then
		collect_obj_name = ""
	end
	if type(collect_obj_name) ~= "string" then
		LuaLogE("自动庄园采集之对指定的采集对象的名字的参数必须是字符串类型");
		return 0
	end
	RunInfo("自动庄园采集，采集等待时间：" .. tostring(collect_time) .. "  采集对象最大数量：" .. tostring(obj_cnt) .. " 每个对象的最大采集次数：" .. tostring(every_collect_cnt) .. " 采集对象名：" .. collect_obj_name)
	return scmd():AutoCollectSpecifc_Farm(collect_time, obj_cnt, every_collect_cnt, collect_obj_name)
end

function 自动采集完房间指定的对象( obj_name, collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("自动采集完房间指定的对象之采集等待时间必须是数字类型");
		return 0
	end
	if type(obj_name) ~= "string" then
		LuaLogE("自动采集完房间指定的对象之采集物名称必须是字符串类型");
		return 0
	end
	RunInfo("自动采集完房间指定的对象，采集名：" .. obj_name .. "  采集等待时间：" .. tostring(collect_time))
	return scmd():AutoCollectOverTheObj(obj_name, collect_time)
end

--[[
第一个参数是等待时间，数字类型，不可为空
第二个参数是对象名，字符串类型，不可为空
第三个参数是房间号，数字类型，可为空，表示角色所在房间号
]]
function 等待出现采集对象( wait_time, obj_name, room_num )
	if type(wait_time) ~= "number" then
		LuaLogE("等待出现采集对象之等待时间必须是数字类型");
		return false
	end
	if type(obj_name) ~= "string" then
		LuaLogE("等待出现采集对象之采集物名称必须是字符串类型");
		return false
	end
	if room_num == nil then
		room_num = -2
	end
	if type(room_num) ~= "number" then
		LuaLogE("等待出现采集对象之房间号必须是数字类型");
		return false
	end
	RunInfo("等待出现采集对象，等待时间：" .. tostring(wait_time) .. "  采集对象名：" .. obj_name .. " 房间号：" .. tostring(room_num))
	return scmd():WaitTheCollectableObj(wait_time, obj_name, room_num)
end

--无参数，返回采集对象
function 获得当前房间离角色最近的采集对象()
	RunInfo("获得当前房间离角色最近的采集对象")
	return FindNearestCollectObjAsSameRoom()
end

function 获得指定名字的可采集对象( obj_name )
	RunInfo("获得指定名字的可采集对象，对象名：" .. obj_name)
	return scmd():FindCollectObjByName(obj_name)
end

--[[
第一个参数是采集对象，可由 获得当前房间离角色最近的采集对象 获得
第二个参数是采集等待时间，数字类型，可为0，表示不等待，不能为空
]]
function 采集一次指定的对象( obj, collect_time )
	if type(collect_time) ~= "number" then
		LuaLogE("采集一次指定的对象之采集等待时间必须是数字类型");
		return false
	end
	RunInfo("采集一次指定的对象，采集等待时间：" .. tostring(collect_time))
	return scmd():CollectTheCollectableObj(obj, collect_time)
end

function 等待可剥皮( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待可剥皮之最大等待时间必须是数字类型");
		return false
	end
	RunInfo("等待可剥皮，最大等待时间：" .. tostring(wait_time))
	return scmd():WaitForCutSkin(wait_time)
end

function 得到副本所有房间号()
	RunInfo(得到副本所有房间号)
	return scmd():GetCurFbAllRoomNum()
end

--无参数
function 收刀()
	RunInfo("收刀")
	return scmd():TakeInKnife();
end

--只有一个参数，物品名，字符串类型，不为空
function 使用背包物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("使用背包物品之物品名必须是字符串类型");
		return false
	end
	RunInfo("使用背包物品，物品名：" .. item_name)
	return scmd():UsePackageItem(item_name)
end

--参数是物品名，支持模糊匹配
function 批量使用物品( item_name, use_cnt )
	if type(item_name) ~= "string" then
		LuaLogE("批量使用物品之物品名必须是字符串类型");
		return 0
	end
	if type(use_cnt) ~= "number" or use_cnt <= 0 then
		use_cnt = 0xfffffff
	end
	RunInfo("批量使用物品，物品名：" .. item_name .. " 数量：" .. tostring(use_cnt))
	return scmd():UseSomeItems(item_name, use_cnt)
end

--参数是物品名，支持模糊匹配
function 使用任务物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("使用任务物品之物品名必须是字符串类型");
		return 0
	end
	RunInfo("使用任务物品，物品名：" .. item_name)
	return scmd():UseTaskItem(item_name)
end

function 制作调和物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("制作调和物品之物品名必须是字符串类型");
		return false
	end
	RunInfo("制作调和物品，物品名：" .. item_name)
	return scmd():ManuFactureItem(item_name)
end

--有一个参数，数字类型，表示最大等待时间，单位毫秒，不能为空
function 等待过图( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待过图之等待时间必须是数字类型")
		return false
	end
	RunInfo("等待过图，等待时间：" .. tostring(wait_time))
	return scmd():WaitCrossedGraph(wait_time)
end

--有一个参数，数字类型，表示最大等待时间，单位毫秒，不能为空
function 等待返回城镇( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待返回城镇之等待时间必须是数字类型")
		return false
	end
	RunInfo("等待返回城镇，等待时间：" .. tostring(wait_time))
	return scmd():WaitRetCity(wait_time)
end

--第一个参数是等待时间，数字类型，不可为空
--第二个参数是要房间号，数字类型，可为空，表示当前角色所在房间，也可为-1，表示所有房间
function 剥一次皮( wait_time, room_num )
	if type(wait_time) ~= "number" then
		LuaLogE("剥一次皮之等待时间必须是数字类型")
		return false
	end
	if type(room_num) ~= "number" then
		room_num = scmd():GetCurRoleRoom()
	end
	RunInfo("剥一次皮，等待时间：" .. tostring(wait_time) .. "  房间号：" .. tostring(room_num))
	return scmd():CutNearestSkinOnce(wait_time, room_num)
end

--第一个参数是等待时间，数字类型，不可为空
--第二个参数是要房间号，数字类型，可为空，表示当前角色所在房间，也可为-1，表示所有房间
function 剥完皮( wait_time, room_num )
	if type(wait_time) ~= "number" then
		LuaLogE("剥完皮之等待时间必须是数字类型")
		return 0
	end
	if type(room_num) ~= "number" then
		room_num = scmd():GetCurRoleRoom()
	end
	RunInfo("剥完皮，等待时间：" .. tostring(wait_time) .. "  房间号：" .. tostring(room_num))
	return scmd():CutOverNearestSkin(wait_time, room_num)
end

--一个参数，表示等待时间，数字类型，可以为空，可以为0，默认等待2分钟
function 等待到空闲状态( wait_time )
	if wait_time == nil then
		wait_time = 0
	end
	if type(wait_time) ~= "number" then
		LuaLogE("等待到空闲状态之等待时间必须是数字类型")
		return false
	end
	RunInfo("等待到空闲状态，等待时间：" .. tostring(wait_time))
	return scmd():WaitToFree(wait_time)
end

function 按Z键()
	RunInfo("按Z键")
	return scmd():PressKeyZ()
end

function 按Q键()
	RunInfo("按Q键")
	return scmd():PressKeyQ()
end

function 按W键()
	RunInfo("按W键")
	return scmd():PressKeyW()
end

function 按S键()
	RunInfo("按S键")
	return scmd():PressKeyS()
end

function 按A键()
	RunInfo("按A键")
	return scmd():PressKeyA()
end

function 按D键()
	RunInfo("按D键")
	return scmd():PressKeyD()
end

function 按空格键()
	RunInfo("按空格键")
	return scmd():PressKeySpace()
end

function 按ESC键()
	RunInfo("按ESC键")
	return scmd():PressKeyEsc()
end

function 左键点击( x, y )
	RunInfo("左键点击,x:" .. tostring(x) .. " y:" .. tostring(y))
	return scmd():LeftButtonClick(x, y)
end

function 关闭所有UI()
	RunInfo("关闭所有UI")
	return scmd():CloseAllUi()
end

function 输出所有怪物信息()
	RunInfo("输出所有怪物信息")
	return scmd():DebugInfoMonsterInfo()
end

function 输出所有可接任务信息()
	RunInfo("输出所有可接任务信息")
	return scmd():DebugInfoAccetableTask()
end

function 输出背包物品信息()
	RunInfo("输出背包物品信息")
	return scmd():DebugInfoPackageItemInfo()
end

function 输出可采集物信息()
	RunInfo("输出可采集物信息")
	return scmd():DebugInfoCollectableObjs()
end

--只有一个参数，物品对象，不能为空
function 得到物品名( item )
	if item == nil then
		LuaLogE("得到物品名之物品参数不能为空")
		return "";
	end
	local name = scmd():GetItemName(item)
	RunInfo("得到物品名，物品名：" .. name)
	return name
end

--只有一个参数，物品对象，不能为空
function 得到物品数量( item )
	if item == nil then
		LuaLogE("得到物品数量之物品参数不能为空")
		return -1;
	end
	RunInfo("得到物品数量")
	return scmd():GetItemCnt(item)
end

function 得到指定物品的所有数量( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("得到指定物品的所有数量之物品名参数必须是字符串类型")
		return -1;
	end
	local res = scmd():GetTheItemTotalCnt(item_name);
	RunInfo("得到指定物品的所有数量，物品名：" .. item_name .. " 数量:" .. tostring(res))
	return res
end

--只有一个参数，物品对象，不能为空
function 得到物品等级( item )
	if item == nil then
		LuaLogE("得到物品等级之物品参数不能为空")
		return -1;
	end
	local res = scmd():GetItemLvl(item)
	RunInfo("得到物品等级,结果：" .. tostring(res))
	return res
end

--只有一个参数，物品对象，不能为空
function 得到物品角色等级( item )
	if item == nil then
		LuaLogE("得到物品角色等级之物品参数不能为空")
		return -1;
	end
	local res = scmd():GetItemRoleLvl(item)
	RunInfo("得到物品角色等级,结果：" .. tostring(res))
	return res
end

--只有一个参数，物品对象，不能为空
function 物品是太刀( item )
	if item == nil then
		LuaLogE("物品是太刀之物品参数不能为空")
		return false
	end
	local res = scmd():IsTaiDaoType(item)
	RunInfo("物品是太刀,结果：" .. tostring(res))
	return res
end

--只有一个参数，物品对象，不能为空
function 是有效物品对象( item )
	RunInfo("是有效物品对象")
	if item == nil then
		return false;
	end
	return scmd():IsValidItem(item)
end

--第一个参数为物品名，字符串类型，不能为空
function 查找背包物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("查找背包物品之物品名参数必须是字符串类型")
		return;
	end
	RunInfo("查找背包物品，物品名：" .. item_name)
	return scmd():FindPakcetItem(item_name)
end

function 查找仓库物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("查找仓库物品之物品名参数必须是字符串类型")
		return;
	end
	RunInfo("查找仓库物品，物品名：" .. item_name)
	return scmd():FindStoreItem(item_name)
end

function 查找任务物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("查找任务物品之物品名参数必须是字符串类型")
		return;
	end
	RunInfo("查找任务物品，物品名：" .. item_name)
	return scmd():FindTaskItem(item_name)
end

--支持模糊匹配
function 得到指定任务物品总数量( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("得到指定任务物品总数量之物品名参数必须是字符串类型")
		return;
	end
	RunInfo("得到指定任务物品总数量，物品名：" .. item_name)
	return scmd():GetTaskTotalItemCnt(item_name)
end

function 查找背包和仓库物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("查找背包和仓库物品之物品名参数必须是字符串类型")
		return;
	end
	RunInfo("查找背包和仓库物品，物品名：" .. item_name)
	return scmd():FindItemFromPacketAndStore(item_name)
end

function 锻造装备( 装备名 )
	if type(装备名) ~= "string" then
		LuaLogE("锻造装备之装备名参数必须是字符串类型")
		return false;
	end
	RunInfo("锻造装备，装备名是：" .. 装备名)
	return scmd():DoCraft(装备名)
end

--装备名支持模糊查找
--装备数量上限可为空，表示要锻造无数个这样的装备
function 自动打造装备( 装备名, 装备数量上限 )
	if type(装备名) ~= "string" then
		LuaLogE("自动打造装备之装备名参数必须是字符串类型")
		return false;
	end
	if type(装备数量上限) ~= "number" then
		装备数量上限 = -1
	end
	RunInfo("自动打造装备，装备名：" .. 装备名 .. " 装备数量上限：" .. tostring(装备数量上限))
	return scmd():AutoMakeEquip(装备名, 装备数量上限)
end

--[[
第一个参数是商品各类，字符串类型，不可为空；支持的商品各类有：猎守补给品、新手猎守装备、弹药补给、弓箭瓶特卖、猎守武器入门许可、武器技能、神秘的商人、艾路猫雇佣、艾露农业特供、庄园必须品
第二个参数是物品名
第三个参数是购买数量，数字类型，不可为空
第四个参数是最大拥有数量，若身上的该物品已经具有这样的数量了，就不会再买了；数字类型，不可为空，可为-1，表示没有拥有上限
第五个参数是要移往的城镇名
第六个参数是NPC名
]]
function 保持购买一定数量的物品( shop_category_name, item_name, buy_cnt, max_cnt, village_name, npc_name, gold_limit )
	if type(shop_category_name) ~= "string" then
		LuaLogE("保持购买一定数量的物品之商品种类参数必须是字符串类型")
		return false;
	end
	if type(item_name) ~= "string" then
		LuaLogE("保持购买一定数量的物品之物品名参数必须是字符串类型")
		return false;
	end
	if type(buy_cnt) ~= "number" then
		LuaLogE("保持购买一定数量的物品之购买数量参数必须是数字类型")
		return false;
	end
	if type(max_cnt) ~= "number" then
		LuaLogE("保持购买一定数量的物品之最大拥有数量参数必须是数字类型")
		return false;
	end
	if village_name == nil then
		village_name = ""
	end
	if type(village_name) ~= "string" then
		LuaLogE("保持购买一定数量的物品之城镇名参数必须是字符串类型")
		return false;
	end
	if type(npc_name) ~= "string" then
		LuaLogE("保持购买一定数量的物品之NPC名参数必须是字符串类型")
		return false;
	end
	if type(gold_limit) ~= "boolean" then
		gold_limit = true
	end
	RunInfo("保持购买一定数量的物品，商品种类：" .. shop_category_name .. " 物品名是：" .. item_name .. " 购买数量:" .. tostring(buy_cnt) .. " 最大拥有数量:" ..
		tostring(max_cnt) .. " 城镇名：" .. village_name .. " NPC名：" .. npc_name .. " 是否检查金币限制：" .. tostring(gold_limit))
	return scmd():BuyTheItemAsMaxCnt(shop_category_name, item_name, buy_cnt, max_cnt, village_name, npc_name, gold_limit)
end

--[[
第一个参数是物品名，字符串类型，不可为空
第二个参数是目标装备索引，数字类型，不可为空。表示这个装备想要升级后成为的装备的索引，从1开始计数
]]
function 升级装备( equip_name, lvl_up_equip_idx )
	if type(equip_name) ~= "string" then
		LuaLogE("升级装备之物品名参数必须是字符串类型")
		return false;
	end
	if type(lvl_up_equip_idx) ~= "number" then
		LuaLogE("升级装备之目标装备索引参数必须是数字类型")
		return false;
	end
	RunInfo("升级装备，物品名：" .. equip_name .. " 目标装备索引：" .. tostring(lvl_up_equip_idx))
	return scmd():LvlUpEquip(equip_name, lvl_up_equip_idx)
end

--只是去自动升级身上装备而已，若材料不足，并不会自动去获取材料
function 自动升级身上装备( village_npc )
	if type(village_npc) ~= "table" then
		LuaLogE("自动升级身上装备之城镇NPC信息参数必须是表类型")
		return false;
	end
	RunInfo("自动升级身上装备")
	return scmd():AutoLvlUpBodyEquips(village_npc)
end

--只会去获取 添加自动穿戴装备白名单 命令添加的装备所需的材料。即当前武器不在白名单中，是不去会获取当前武器所需材料的。
function 自动去获取当前武器所需材料()
	RunInfo("自动去获取当前武器所需材料")
	return scmd():AutoDoGainCurWeaponMaterial()
end

--[[
用来影响 自动升级身上装备 命令，即指定物品不会自动升级的
参数是物品名，字符串类型，不可为空
]]
function 添加自动升级身上装备黑名单( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("添加自动升级身上装备黑名单之物品名参数必须是字符串类型")
		return false;
	end
	RunInfo("添加自动升级身上装备黑名单，物品名：" .. item_name)
	return scmd():AddAutoLvlUpBlackList(item_name)
end

function 清空自动升级装备黑名单()
	RunInfo("清空自动升级装备黑名单")
	return scmd():ClrAutoLvlUpEquipBlackList()
end

function 是有效的任务ID( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("是有效的任务ID之任务ID参数必须是数字类型")
		return false;
	end
	RunInfo("是有效的任务ID")
	return scmd():IsValidTaskId(task_id)
end

function 今日剩余可接赏金任务数量()
	local res = scmd():GetLeftAcceptableShangJinCntToday()
	RunInfo("今日剩余可接赏金任务数量，数量：" .. tostring(res))
	return res
end

function 获取一个可接的赏金任务ID()
	RunInfo("获取一个可接的赏金任务ID")
	local res = scmd():GetAnyAcceptableShangJinTaskId(0)
	return unpack(res)
end

function 获取一个可做的赏金任务ID()
	RunInfo("获取一个可做的赏金任务ID")
	local res = scmd():GetAnyCanDoShangJinTaskId(0)
	return unpack(res)
end

function 获取一个可提交的赏金任务ID()
	RunInfo("获取一个可提交的赏金任务ID")
	return scmd():GetAnyCommitableShangJinTaskId()
end

function 获取任务所做副本名( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("获取任务所做副本名之任务ID参数必须是数字类型")
		return;
	end
	RunInfo("获取任务所做副本名")
	return scmd():GetTaskToDstFbName(task_id)
end

function 获取任务所做副本ID( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("获取任务所做副本ID之任务ID参数必须是数字类型")
		return;
	end
	RunInfo("获取任务所做副本ID, 任务ID：" .. tostring(task_id))
	return scmd():GetTaskToDstFbId(task_id)
end

function 根据ID获取副本名( fb_id )
	if type(fb_id) ~= "number" then
		LuaLogE("根据ID获取副本名之副本ID参数必须是数字类型")
		return;
	end
	RunInfo("根据ID获取副本名，副本ID：" .. tostring(fb_id))
	return scmd():GetTheFbNameById(fb_id)
end

function 根据副本名获取副本ID( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("根据副本名获取副本ID之副本名参数必须是字符串类型")
		return -1;
	end
	RunInfo("根据副本名获取副本ID，副本名：" .. fb_name)
	return scmd():GetTheFbIdByName(fb_name)
end

function 获取任务名( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("获取任务名之任务ID参数必须是数字类型")
		return;
	end
	RunInfo("获取任务名")
	return scmd():GetTheTaskName(task_id)
end

--[[
第一个参数是物品所属物品栏，数字类型，不可为空。1为背包栏，2为仓库栏，3为装备栏，4为素材栏
第二个参数是物品名，字符串类型，不可为空
第三个参数是物品等级，数字类型，不可为空
第四个参数是物品要移往的物品栏，数字类型，不可为空。1为背包栏，2为仓库栏，3为装备栏，4为素材栏
第五个参数是要移往的物品栏的格子索引，数字类型，不可为空，1是第一个索引
]]
function 移动物品( tab_category_src, item_name, tab_category_dst, dst_slot_idx )
	if type(tab_category_src) ~= "number" then
		LuaLogE("移动物品之物品所属物品栏参数必须是数字类型")
		return false
	end
	if type(item_name) ~= "string" then
		LuaLogE("移动物品之要移动的物品名参数必须是字符串类型")
		return false
	end
	if type(tab_category_dst) ~= "number" then
		LuaLogE("移动物品之移往的物品栏参数必须是数字类型")
		return false
	end
	if type(dst_slot_idx) ~= "number" then
		LuaLogE("移动物品之移往的物品索引参数必须是数字类型")
		return false
	end
	RunInfo("移动物品，物品名：" .. item_name)
	return scmd():MoveItem(tab_category_src, item_name, tab_category_dst, dst_slot_idx)
end

--[[
参数是保留的空格子的数量，数字类型，不可为空
函数会使背包空出起码这个数量的格子，并把多余的物品放到仓库中去
]]
function 存放一些背包物品到仓库( reserve_cnt )
	if type(reserve_cnt) ~= "number" then
		LuaLogE("存放一些背包物品到仓库之保留的空格子的数量参数必须是数字类型")
		return false
	end
	RunInfo("存放一些背包物品到仓库，保留的空格子的数量：" .. tostring(reserve_cnt))
	return scmd():StoreSomePackageItemsToStore(reserve_cnt)
end

function 从仓库中取物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("从仓库中取物品之物品名参数必须是字符串类型")
		return false
	end
	RunInfo("从仓库中取物品，物品名：" .. item_name)
	return scmd():GainItemFromStore(item_name)
end

function 存物品到仓库( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("存物品到仓库之物品名参数必须是字符串类型")
		return false
	end
	RunInfo("存物品到仓库，物品名：" .. item_name)
	return scmd():PackageItemToStore(item_name)
end

--参数是物品名，支持模糊查找
function 从仓库中取一些物品( item_name, cnt )
	if type(item_name) ~= "string" then
		LuaLogE("从仓库中取一些物品之物品名参数必须是字符串类型")
		return false
	end
	if type(cnt) ~= "number" then
		cnt = -1
	end
	RunInfo("从仓库中取一些物品，物品名：" .. item_name .. " 数量：" .. tostring(cnt))
	return scmd():GainSomeItemsFromStore(item_name, cnt)
end

function 添加存仓库物品白名单( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("添加存仓库物品白名单之物品名必须是字符串类型")
		return
	end
	RunInfo("添加存仓库物品白名单，物品名：" .. item_name)
	return scmd():AddStoreWhiteList(item_name)
end

function 清空存仓库物品白名单()
	RunInfo("清空存仓库物品白名单")
	return scmd():ClrStoreWhiteList()
end

--[[
参数是物品名，字符串类型，不能为空
]]
function 穿戴物品( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("穿戴物品之要移动的物品名参数必须是字符串类型")
		return false
	end
	RunInfo("穿戴物品，物品名：" .. item_name)
	return scmd():WearEquip(item_name)
end

--[[
只穿戴小于等于参数指定的品质的装备
参数是物品品质上限，数字类型，不可为空，必须是从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色
]]
function 自动穿戴一些装备( equip_quality_limit, is_auto_gian )
	if type(equip_quality_limit) ~= "number" then
		LuaLogE("穿戴物品之装备品质上限参数必须是数字类型")
		return false
	end
	if type(is_auto_gian) ~= "boolean" then
		is_auto_gian = false
	end
	RunInfo("自动穿戴一些装备，装备品质上限：" .. tostring(equip_quality_limit) .. " 是否先自动去获取装备材料：" .. tostring(is_auto_gian))
	return scmd():AutoWearSomeEquips(equip_quality_limit, is_auto_gian)
end

function 添加自动穿戴装备黑名单( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("添加自动穿戴装备黑名单之物品名参数必须是字符串类型")
		return false
	end
	RunInfo("添加自动穿戴装备黑名单，物品名：" .. item_name)
	return scmd():AddAutoWearBlackList(item_name)
end

function 清空自动穿戴装备黑名单()
	RunInfo("清空自动穿戴装备黑名单")
	return scmd():ClrAutoWearBlackList()
end

function 添加自动穿戴装备白名单( item_name, item_type )
	if type(item_name) ~= "string" then
		LuaLogE("添加自动穿戴装备白名单之装备名参数必须是字符串类型")
		return false
	end
	if type(item_type) ~= "string" then
		LuaLogE("添加自动穿戴装备白名单之装备类型参数必须是字符串类型")
		return false
	end
	RunInfo("添加自动穿戴装备白名单，装备名：" .. item_name .. " 装备类型：" .. item_type)
	return scmd():AddAutoWearWhiteList(item_name, item_type)
end

function 清空自动穿戴装备白名单()
	RunInfo("清空自动穿戴装备白名单")
	return scmd():ClrAutoWearWhiteList()
end

--[[
参数是装备的索引号，数字类型，不能为空，第一个武器索引号是1，手部的索引号是2，依此类推
]]
function 脱下装备( equip_part_idx )
	if type(equip_part_idx) ~= "number" then
		LuaLogE("脱下装备之装备索引参数必须是数字类型")
		return false
	end
	RunInfo("脱下装备，装备索引：" .. tostring(equip_part_idx))
	return scmd():GetOffEquipByIdx(equip_part_idx)
end

function 进入到自己的庄园()
	RunInfo("进入到自己的庄园")
	return scmd():EnterSelfFarm()
end

function 进入新手训练()
	RunInfo("进入新手训练")
	return scmd():EnterXinShou()
end

function 离开庄园()
	RunInfo("离开庄园")
	return scmd():LeaveFarm()
end

function 获得一个已有的种子名()
	local the_name = scmd():GetExistFarmSeedName()
	RunInfo("获得一个已有的种子名，种子名：" .. the_name)
	return the_name
end

--[[
第一个参数是种子名，也即物品名
第二个参数是土地名，支持草药田1、草药田2、草药田3。可为空，默认为草药田1
]]
function 种植种子( seed_name, land_name )
	if type(seed_name) ~= "string" then
		LuaLogE("种植种子之种子名参数必须是字符串类型")
		return false
	end
	if land_name == nil then
		land_name = ""
	end
	if type(land_name) ~= "string" then
		LuaLogE("种植种子之土地索引参数必须是字符串类型")
		return false
	end
	RunInfo("种植种子，种子名：" .. seed_name .. " 土地名：" .. land_name)
	return scmd():PlantFarmSeed(seed_name, land_name)
end

function 清空打怪时的处理()
	RunInfo("清空打怪时的处理")
	return scmd():ClrHandleAtHitting()
end

--[[
第一个参数是判断条件的函数，返回真，不能为这空。如果函数返回真，才会去执行第二个函数
第二个参数是做事的函数，不返回任何值，不能为空
]]
function 添加打怪时的处理( judge_func, do_func )
	RunInfo("添加打怪时的处理")
	return scmd():AddHandleAtHitting(judge_func, do_func)
end

function 清空脚本停止时的处理()
	RunInfo("清空脚本停止时的处理")
	return scmd():ClrHandlersAtScriptStoped()
end

function 添加脚本停止时的处理( func )
	RunInfo("添加脚本停止时的处理")
	return scmd():AddHandlersAtScriptStoped(func)
end

function 获取角色当前血值()
	local res = scmd():GetRoleCurHp()
	RunInfo("获取角色当前血值：" .. tostring(res))
	return res
end

--第一个参数是村庄名，第二个参数是邮箱的NPC名
function 处理所有邮件( village_name, npc_name )
	if type(village_name) ~= "string" then
		LuaLogE("处理所有邮件之村庄名参数必须是字符串类型")
		return 0
	end
	if type(npc_name) ~= "string" then
		LuaLogE("处理所有邮件之NPC名参数必须是字符串类型")
		return 0
	end
	RunInfo("处理所有邮件, 村庄名：" .. village_name .. " NPC名：" .. npc_name)
	return scmd():HandleAllMails(village_name, npc_name)
end

--第一个参数是个表，类似这样：{ { "怪鸟之翼", 1 }, { "护石", 10, 真 }, { "一角龙之翼", -1 } }，
--其中数字1表示邮寄怪鸟之翼的数量为1，-1表示邮寄所有一角龙之翼。其中护石那一项，第三个参数是个布尔类型，可不填，不填即为假，为真表示护石达到了10个才会去邮寄。
--第二个参数是要邮寄的金币，数字类型，不可为空，为0表示不邮寄金币，如果是-100，表示保留100金币，其余全都邮寄
--第三个参数是接受邮件的角色名
--第四个参数是村庄名
--第五个参数是NPC名，即邮件NPC的名字
function 发送邮件( tbl_items, gold, target_role_name, village_name, npc_name )
	if type(gold) ~= "number" then
		LuaLogE("发送邮件之金币参数必须是数字类型")
		return -1
	end
	if type(target_role_name) ~= "string" then
		LuaLogE("发送邮件之角色名参数必须是字符串类型")
		return -1
	end
	if type(village_name) ~= "string" then
		LuaLogE("发送邮件之村庄名参数必须是字符串类型")
		return -1
	end
	if type(npc_name) ~= "string" then
		LuaLogE("发送邮件之NPC名参数必须是字符串类型")
		return -1
	end
	RunInfo("发送邮件, 金币：" .. tostring(gold) .. " 角色名：" .. target_role_name)
	return scmd():SendMail(tbl_items, gold, target_role_name, village_name, npc_name)
end

--邮寄任意物品 与 发送邮件 命令类似，都有五个参数，后四个参数含义一致
--第一个参数是指定邮寄数量，数字类型，不可为空。当物品的数量达到指定邮寄数量，便会去邮寄。
function 邮寄任意物品(item_reach_cnt, gold, target_role_name, village_name, npc_name)
	if type(item_reach_cnt) ~= "number" then
		LuaLogE("邮寄任意物品之指定邮寄数量参数必须是数字类型")
		return -1
	end
	if type(gold) ~= "number" then
		LuaLogE("邮寄任意物品之金币参数必须是数字类型")
		return -1
	end
	if type(target_role_name) ~= "string" then
		LuaLogE("邮寄任意物品之角色名参数必须是字符串类型")
		return -1
	end
	if type(village_name) ~= "string" then
		LuaLogE("邮寄任意物品之村庄名参数必须是字符串类型")
		return -1
	end
	if type(npc_name) ~= "string" then
		LuaLogE("邮寄任意物品之NPC名参数必须是字符串类型")
		return -1
	end
	RunInfo("邮寄任意物品, 指定邮寄数量：" .. tostring(item_reach_cnt) .. " 金币：" .. tostring(gold) .. " 角色名：" .. target_role_name)
	return scmd():SendAnyMail(item_reach_cnt, gold, target_role_name, village_name, npc_name)
end

--[[
第一个参数是采集点索引，从1到6依次是矿场、鱼塘、虫栖地、育菇木床、蜂房、草药田
第二个参数是等级上限，可为空，表示无上限
]]
function 升级庄园采集( collect_obj_name, lvl_limit, is_open_npc )
	if type(collect_obj_name) ~= "string" then
		LuaLogE("升级庄园采集之采集名必须是字符串类型")
		return 0
	end
	if lvl_limit == nil then
		lvl_limit = -1;
	end
	if type(lvl_limit) ~= "number" then
		LuaLogE("升级庄园采集之等级上限必须是数字类型")
		return 0
	end
	if type(is_open_npc) ~= "boolean" then
		is_open_npc = true
	end
	RunInfo("升级庄园采集，采集名：" .. collect_obj_name .. " 等级上限:" .. tostring(lvl_limit))
	return scmd():LvlUpCollectOfFarm(collect_obj_name, lvl_limit, is_open_npc)
end

--参数是采集名，字符串类型，可为空，表示最短的采集时间，支持的名称：矿场、鱼塘、虫栖地、育菇木床、蜂房、草药田1、草药田2、草药田3
function 到了庄园采集的时间( collect_name )
	if type(collect_name) ~= "string" then
		collect_name = ""
	end
	RunInfo("到了庄园采集的时间，采集名：" .. collect_name)
	return scmd():IsTimeToCollectFarm(collect_name)
end

function 选择指定名字的角色( role_name )
	if type(role_name) ~= "string" then
		LuaLogE("选择指定名字的角色之角色名参数必须是字符串类型")
		return false
	end
	RunInfo("选择指定名字的角色，角色名：" .. role_name)
	return scmd():SelectRoleByName(role_name)
end

function 选择指定的角色( role_idx )
	if type(role_idx) ~= "number" then
		LuaLogE("选择指定的角色之角色索引参数必须是数字类型")
		return false
	end
	RunInfo("选择指定的角色，角色索引：" .. tostring(role_idx))
	return scmd():SeletRoleByIdx(role_idx)
end

function 等待可选择角色( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待可选择角色之等待时间必须是数字类型")
		return false
	end
	RunInfo("等待可选择角色，等待时间：" .. tostring(wait_time))
	return scmd():WaitForSelectableRole(wait_time)
end

function 自动创建角色()
	RunInfo("自动创建角色")
	return scmd():AutoCreateRole()
end

function 自动创建或选择角色()
	RunInfo("自动创建或选择角色")
	return scmd():AutoCreateOrSelectRole()
end

function 设置当前要删除的角色索引( role_idx )
	if type(role_idx) ~= "number" then
		LuaLogE("设置当前要删除的角色索引之角色索引参数必须是数字类型")
		return false
	end
	RunInfo("设置当前要删除的角色索引，角色索引：" .. tostring(role_idx))
	return scmd():SetCurDeleteRoleIdx(role_idx)
end

function 得到当前角色的角色列表索引()
	local res = scmd():GetCurRoleIdxAtRoleList()
	RunInfo("得到当前角色的角色列表索引:" .. tostring(res))
	return res
end

function 得到当前要删除的角色索引()
	local res = scmd():GetCurDeleteRoleIdx()
	RunInfo("得到当前要删除的角色索引:" .. tostring(res))
	return res
end

function 进行删除角色( change_ac_at_delete_one_role )
	if type(change_ac_at_delete_one_role) ~= "boolean" then
		change_ac_at_delete_one_role = true
	end
	RunInfo("进行删除角色，是否在删除每个角色的时候换号：" .. tostring(change_ac_at_delete_one_role))
	return scmd():DoDeleteRole(change_ac_at_delete_one_role)
end

function 在选择角色界面()
	RunInfo("在选择角色界面")
	return scmd():IsAtSelectRole()
end

function 已进入到游戏()
	RunInfo("已进入到游戏")
	return scmd():IsAtInGame()
end

function 换角色()
	RunInfo("换角色")
	return scmd():ChangeRole()
end

function 换号( str )
	if type(str) ~= "string" then
		str = ""
	end
	RunInfo("换号：" .. str)
	return scmd():ChangeGameAc(str)
end

function 需要创建角色()
	RunInfo("需要创建角色")
	return scmd():NeedCreateRole()
end

function 获取角色数量()
	RunInfo("获取角色数量")
	return scmd():GetRoleCnt()
end

--[[
第一个参数是剧情名，字符串类型，不能为空
第二个参数是等待时间，数字类型，不能为空
]]

function 等待指定的剧情( str_ju_qing, wait_time )
	if type(str_ju_qing) ~= "string" then
		LuaLogE("等待指定的剧情之剧情名参数必须是字符串类型")
		return false
	end
	if type(wait_time) ~= "number" then
		LuaLogE("等待指定的剧情之等待时间参数必须是数字类型")
		return false
	end
	RunInfo("等待指定的剧情，剧情名：" .. str_ju_qing .. "  等待时间：" .. tostring(wait_time))
	return scmd():WaitForTheJuQing(str_ju_qing, wait_time)
end

function 步过指定名字的静止对象( obj_name )
	if type(obj_name) ~= "string" then
		LuaLogE("步过指定名字的静止对象之对象名必须是字符串类型")
		return 0
	end
	return scmd():StepOverTheStaticObjs(obj_name)
end

function 步过闪光的东西()
	RunInfo("步过闪光的东西")
	return 步过指定名字的静止对象("Bug_SFX_")
end

function 输出静止对象信息( obj_name )
	if type(obj_name) ~= "string" then
		LuaLogE("输出静止对象信息之对象名必须是字符串类型")
		return 0
	end
	RunInfo("输出静止对象信息，对象名：" .. obj_name)
	return scmd():DebugInfoStaticObjs(obj_name)
end

function 输出静止可打的对象信息()
	RunInfo("输出静止可打的对象信息")
	return scmd():DebugInfoStaticBeatableObjs()
end

function 自动去做指定的公会委托( task_lvl, task_name, do_func )
	if type(task_lvl) ~= "string" then
		LuaLogE("自动去做指定的公会委托之任务等级必须是字符串类型")
		return 0
	end
	if type(task_name) ~= "string" then
		LuaLogE("自动去做指定的公会委托之任务名必须是字符串类型")
		return 0
	end
	RunInfo("自动去做指定的公会委托，任务等级：" .. task_lvl .. "  任务名：" .. task_name)
	if scmd():AutoToDoTheSociatyTask(task_lvl, task_name, do_func) then
		return 1
	end
	return 0
end

--[[
第一个参数是物品名，字符串类型，不可为空
第二个参数是要拍卖的物品数量，数字类型，不可为空
第三个参数是起拍价，数字类型，不可为空
第四个参数是一口价，数字类型，不可为空
第五个参数相对一口价，数字类型，不可为空。参考前5(第七个参数)个最低一口价的平均价，若此参数为正数，表示在平均价的基础上加，否则表示在平均价的基础上减。
第六个参数最低一口价，数字类型，不可为空。通过第5个参数计算出来的一口价，不能小于这个最低一口价，否则一口价按第4个参数设定。
第七个参数是平均数取值，默认为5，表示取前5个最低价的平均值，可为空，最多不超过一个拍卖页面的数量。
第八个参数是直接起拍价，布尔类型，可为空，默认为真，表示第三个参数(起拍价)填多少数值，挂拍卖成功后的起拍价就为多少。若该参数为假，第三个参数的含义为间接起拍价，它是一个从1到100之间的数值，表示相对于一口价的百分比。比如第三个参数为80，表示起拍价按一口价的百分之八十来计算。
若是该物品并没有人在拍卖，那么第5、6、7个参数不起作用。
]]
function 挂拍卖( item_name, item_cnt, begin_price, smart_price, relative_smart_price, min_smart_price, average_cnt, begin_price_direct )
	if type(item_name) ~= "string" then
		LuaLogE("挂拍卖之物品名参数必须是字符串类型");
		return false
	end
	if type(item_cnt) ~= "number" then
		LuaLogE("挂拍卖之要拍卖的物品数量参数必须是数字类型");
		return false
	end
	if type(begin_price) ~= "number" then
		LuaLogE("挂拍卖之起拍价参数必须是数字类型");
		return false
	end
	if type(smart_price) ~= "number" then
		LuaLogE("挂拍卖之一口价参数必须是数字类型");
		return false
	end
	if type(relative_smart_price) ~= "number" then
		LuaLogE("挂拍卖之相对一口价参数必须是数字类型");
		return false
	end
	if type(min_smart_price) ~= "number" then
		LuaLogE("挂拍卖之最低一口价参数必须是数字类型");
		return false
	end
	if type(average_cnt) ~= "number" then
		average_cnt = 5
	end
	if type(begin_price_direct) ~= "boolean" then
		begin_price_direct = true
	end
	RunInfo("挂拍卖，物品名：" .. item_name .. " 物品数量：" .. tostring(item_cnt) .. " 起拍价：" .. tostring(begin_price) ..
		" 一口价：" .. tostring(smart_price) .. " 相对一口价：" .. tostring(relative_smart_price) .. " 最低一口价：" .. tostring(min_smart_price)
		.. " 平均数取值：" .. tostring(average_cnt) .. " 是否为直接起拍价：" .. tostring(begin_price_direct))
	return scmd():SellToAuction(item_name, item_cnt, begin_price, smart_price, relative_smart_price, min_smart_price, average_cnt, begin_price_direct)
end

--[[
第一个参数是物品名，字符串类型，不可为空
第二个参数是要拍卖的物品数量，数字类型，不可为空
第三个参数是起拍价，数字类型，不可为空
第四个参数是一口价，数字类型，不可为空
]]
function 挂拍卖给对方号( item_name, item_cnt, begin_price, smart_price )
	if type(item_name) ~= "string" then
		LuaLogE("挂拍卖给对方号之物品名参数必须是字符串类型");
		return false
	end
	if type(item_cnt) ~= "number" then
		LuaLogE("挂拍卖给对方号之要拍卖的物品数量参数必须是数字类型");
		return false
	end
	if type(begin_price) ~= "number" then
		LuaLogE("挂拍卖给对方号之起拍价参数必须是数字类型");
		return false
	end
	if type(smart_price) ~= "number" then
		LuaLogE("挂拍卖给对方号之一口价参数必须是数字类型");
		return false
	end
	RunInfo("挂拍卖给对方号，物品名：" .. item_name .. " 物品数量：" .. tostring(item_cnt) .. " 起拍价：" .. tostring(begin_price) .. " 一口价：" .. tostring(smart_price))
	return scmd():SellToOtherSide(item_name, item_cnt, begin_price, smart_price)
end

--[[
第一个参数是物品名，表示要购买的物品，字符串类型，不可为空
第二个参数是查询页数，数字类型，不可为空。如果物品的总页数为10，查询页数为3，表示在1到3的页面物品中查找该物品；如果查询页数为-3，表示在7到10的页面物品中查找该物品。
第三个参数是拍卖人，字符串类型，可为空，为空时表示任何售卖者；不为空时，表示只买指定名字的拍卖人的物品
第四个参数是价格上限，数字类型，不可为空，表示若查找到的物品的拍卖价格小于等级该价格上限，才会购买该物品
第五个参数是购买数量，表示购买次数的数量，而不是物品数量。如果购买数量为2，表示要买两次这样名字的物品
]]
function 一口价购买( item_name, search_page_cnt, seller, price_limit, buy_cnt )
	if type(item_name) ~= "string" then
		LuaLogE("一口价购买之物品名参数必须是字符串类型");
		return 0
	end
	if type(search_page_cnt) ~= "number" then
		LuaLogE("一口价购买之查询页数参数必须是数字类型");
		return 0
	end
	if seller == nil then
		seller = ""
	end
	if type(seller) ~= "string" then
		LuaLogE("一口价购买之拍卖人参数必须是字符串类型");
		return 0
	end
	if type(price_limit) ~= "number" then
		LuaLogE("一口价购买之价格上限参数必须是数字类型");
		return 0
	end
	if type(buy_cnt) ~= "number" then
		LuaLogE("一口价购买之购买数量参数必须是数字类型");
		return 0
	end
	RunInfo("一口价购买，物品名：" .. item_name .. " 查询页数：" .. tostring(search_page_cnt) .. " 售卖者：" .. seller .. " 价格上限：" .. tostring(price_limit) .. " 购买数量：" .. tostring(buy_cnt))
	return scmd():SmartBuy(item_name, search_page_cnt, seller, price_limit, buy_cnt)
end

function 购买一些对方阵营的拍卖物品()
	RunInfo("购买一些对方阵营的拍卖物品")
	return scmd():SmartBuyOtherSide()
end

--[[
参数为物品名，字符串类型，不可为空
]]
function 查询已拍卖条目数( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("查询已拍卖条目数之物品名必须为字符串类型")
		return 0;
	end
	local cnt = scmd():QuerySoldCnt(item_name)
	RunInfo("查询已拍卖条目数，物品名：" .. item_name .. " 返回结果：" .. tostring(cnt))
	return cnt;
end

--[[
凡是在黑名单中的，就一定会被卖掉；并且卖掉在不白名单中的物品
第一个参数是背包的空格子的保留数量，数字类型，不能为空
第二个参数是仓库的空格子的保留数量，数字类型，不能为空
第三个参数是城镇名，字符串类型，不能为空
第四个参数是NPC名，字符串类型，不能为空。垃圾物品就会卖给指定城镇中指定NPC处
]]
function 卖掉仓库和背包的垃圾物品( packet_reserve_cnt, stored_reserve_cnt, village_name, npc_name )
	if type(packet_reserve_cnt) ~= "number" then
		LuaLogE("卖掉仓库和背包的垃圾物品之背包的空闲保留个数参数必须是数字类型")
		return 0;
	end
	if type(stored_reserve_cnt) ~= "number" then
		LuaLogE("卖掉仓库和背包的垃圾物品之仓库的空闲保留个数参数必须是数字类型")
		return 0;
	end
	if village_name == nil then
		village_name = ""
	end
	if type(village_name) ~= "string" then
		LuaLogE("卖掉仓库和背包的垃圾物品之城镇名参数必须是字符串类型")
		return 0;
	end
	if type(npc_name) ~= "string" then
		LuaLogE("卖掉仓库和背包的垃圾物品之NPC名参数必须是字符串类型")
		return 0;
	end
	RunInfo("卖掉仓库和背包的垃圾物品，背包空闲保留个数：" .. tostring(packet_reserve_cnt) .. " 仓库空闲保留个数：" .. tostring(stored_reserve_cnt))
	return scmd():SellStored_PacketRubbishItems(packet_reserve_cnt, stored_reserve_cnt, village_name, npc_name);
end

--[[
参数是函数类型
]]
function 设置卖商店物品过滤黑名单( item_func )
	if type(item_func) ~= "function" then
		LuaLogE("设置卖商店物品过滤黑名单之参数必须为函数类型")
		return;
	end
	RunInfo("设置卖商店物品过滤黑名单")
	return scmd():SetItemFilterBlackList(item_func);
end

--[[
参数是函数类型
]]
function 设置卖商店物品过滤白名单( item_func )
	if type(item_func) ~= "function" then
		LuaLogE("设置卖商店物品过滤白名单之参数必须为函数类型")
		return;
	end
	RunInfo("设置卖商店物品过滤白名单")
	return scmd():SetItemFilterWhiteList(item_func);
end

--[[
参数是物品名，字符串类型，不可为空
]]
function 添加卖商店物品名黑名单( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("添加卖商店物品名黑名单之参数必须为函数类型")
		return;
	end
	RunInfo("添加卖商店物品名黑名单，物品名：" .. item_name)
	return scmd():AddItemNameBlackList(item_name);
end

function 清空卖商店物品名黑名单()
	RunInfo("清空卖商店物品名黑名单")
	return scmd():ClrItemNameBlackList()
end

--[[
参数是物品名，字符串类型，不可为空
]]
function 添加卖商店物品名白名单( item_name )
	if type(item_name) ~= "string" then
		LuaLogE("添加卖商店物品名白名单之参数必须为函数类型")
		return;
	end
	RunInfo("添加卖商店物品名白名单，物品名：" .. item_name)
	return scmd():AddItemNameWhiteList(item_name);
end

function 清空卖商店物品名白名单()
	RunInfo("清空卖商店物品名白名单")
	return scmd():ClrItemNameWhiteList()
end

--[[
除了黑名单中的物品，凡是物品等级和物品品质能够同时符合的，都会添加到白名单中
假设有一个物品，物品等级=20，物品品质为3(即中级物品)。如果 20大于等级第一个参数，且3大于等级第二个参数，则把该物品添加到白名单中
第一个参数是物品等级，数字类型，不可为空
第二个参数是物品品质，数字类型，不可为空，必须是从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色
]]
function 设置卖商店物品信息白名单( item_lvl, item_quality )
	if type(item_lvl) ~= "number" then
		LuaLogE("设置卖商店物品信息白名单之物品等级参数必须为数字类型")
		return;
	end
	if type(item_quality) ~= "number" then
		LuaLogE("设置卖商店物品信息白名单之物品品质参数必须为数字类型")
		return;
	end
	RunInfo("设置卖商店物品信息白名单，物品等级：" .. tostring(item_lvl) .. " 物品品质:" .. tostring(item_quality))
	return scmd():SetItemInfoWhiteList(item_lvl, item_quality);
end

--[[
当前物品为材料，假如存在这么一个锻造后的物品，其物品等级和物品品质能够同时符合对应参数的，就会添加到白名单中
第一个参数是物品等级，数字类型，不可为空
第二个参数是物品品质，数字类型，不可为空，必须是从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色
]]
function 设置卖商店锻造物品白名单( item_lvl, item_quality )
	if type(item_lvl) ~= "number" then
		LuaLogE("设置卖商店锻造物品白名单之物品等级参数必须为数字类型")
		return;
	end
	if type(item_quality) ~= "number" then
		LuaLogE("设置卖商店锻造物品白名单之物品品质参数必须为数字类型")
		return;
	end
	RunInfo("设置卖商店锻造物品白名单，物品等级：" .. tostring(item_lvl) .. " 物品品质:" .. tostring(item_quality))
	return scmd():SetCraftItemWhiteList(item_lvl, item_quality);
end

--[[
当前物品为材料，假如存在这么一个升级后的物品，其物品等级和物品品质能够同时符合对应参数的，就会添加到白名单中
第一个参数是物品等级，数字类型，不可为空
第二个参数是物品品质，数字类型，不可为空，必须是从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色
]]
function 设置卖商店升级装备白名单( item_lvl, item_quality )
	if type(item_lvl) ~= "number" then
		LuaLogE("设置卖商店升级装备白名单之物品等级参数必须为数字类型")
		return;
	end
	if type(item_quality) ~= "number" then
		LuaLogE("设置卖商店升级装备白名单之物品品质参数必须为数字类型")
		return;
	end
	RunInfo("设置卖商店升级装备白名单，物品等级：" .. tostring(item_lvl) .. " 物品品质:" .. tostring(item_quality))
	return scmd():SetLvlUpEquipWhiteList(item_lvl, item_quality);
end

--[[
当前物品为材料，假如存在这么一个调和后的物品，其物品等级和物品品质能够同时符合对应参数的，就会添加到白名单中
第一个参数是物品等级，数字类型，不可为空
第二个参数是物品品质，数字类型，不可为空，必须是从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色
]]
function 设置卖商店调和白名单( item_lvl, item_quality )
	if type(item_lvl) ~= "number" then
		LuaLogE("设置卖商店调和白名单之物品等级参数必须为数字类型")
		return;
	end
	if type(item_quality) ~= "number" then
		LuaLogE("设置卖商店调和白名单之物品品质参数必须为数字类型")
		return;
	end
	RunInfo("设置卖商店调和白名单，物品等级：" .. tostring(item_lvl) .. " 物品品质:" .. tostring(item_quality))
	return scmd():SetFactureWhiteList(item_lvl, item_quality);
end

function 等待角色可以移动( wait_time )
	if type(wait_time) ~= "number" then
		LuaLogE("等待角色可以移动之等待时间参数必须为数字类型")
		return false;
	end
	RunInfo("等待角色可以移动，等待时间：" .. tostring(wait_time))
	return scmd():WaitRoleCanMove(wait_time)
end

function 设置角色坐标( x, y, z )
	if type(x) ~= "number" then
		LuaLogE("设置角色坐标之第一个参数即X坐标必须为数字类型")
		return;
	end
	if type(y) ~= "number" then
		LuaLogE("设置角色坐标之第二个参数即Y坐标必须为数字类型")
		return;
	end
	if type(z) ~= "number" then
		LuaLogE("设置角色坐标之第三个参数即Z坐标必须为数字类型")
		return;
	end
	RunInfo("设置角色坐标，x：" .. tostring(x) .. " y:" .. tostring(y) .. " z:" .. tostring(z))
	return scmd():SetRoleToPos(x, y, z)
end

function 设置到目标对象的朝向( x, y, z )
	if type(x) ~= "number" then
		LuaLogE("设置到目标对象的朝向之第一个参数即X坐标必须为数字类型")
		return;
	end
	if type(y) ~= "number" then
		LuaLogE("设置到目标对象的朝向之第二个参数即Y坐标必须为数字类型")
		return;
	end
	if type(z) ~= "number" then
		LuaLogE("设置到目标对象的朝向之第三个参数即Z坐标必须为数字类型")
		return;
	end
	RunInfo("设置到目标对象的朝向，x：" .. tostring(x) .. " y:" .. tostring(y) .. " z:" .. tostring(z))
	return scmd():SetDirFromDstObj(x, y, z)
end

function 是新创建的角色()
	RunInfo("是新创建的角色")
	return scmd():IsTheNewRole()
end

function 剩余单倍经验()
	local res = scmd():G_GetLeftSingleExp();
	RunInfo("剩余单倍经验，返回：" .. tostring(res))
	return res
end

function 剩余双倍经验()
	local res = scmd():G_GetLeftDoubleExp();
	RunInfo("剩余双倍经验，返回：" .. tostring(res))
	return res;
end

function 任务_做热气球()
	RunInfo("任务_做热气球")
	return scmd():ZuoReQiQiu();
end

function 获取合适等级的副本名()
	local fb_name = scmd():FindSuitableFbName()
	RunInfo("获取合适等级的副本名，副本名：" .. fb_name)
	return fb_name
end

function 在炎热状态()
	RunInfo("在炎热状态")
	return scmd():IsHotStatus()
end

function 在寒冷状态()
	RunInfo("在寒冷状态")
	return scmd():IsColdStatus()
end

function 在中毒状态()
	RunInfo("在中毒状态")
	return scmd():IsPoisoningStatus()
end

function 设置进入副本后的处理( func )
	RunInfo("设置进入副本后的处理")
	return scmd():SetAfterEnteredFb(func)
end

function 需要磨刀()
	RunInfo("需要磨刀")
	return scmd():NeedMoDao()
end

function 角色等级()
	local res = scmd():GetRoleLvl()
	RunInfo("角色等级,返回：" .. tostring(res))
	return res
end

--第一个参数是任务名（支持模糊匹配），字符串类型，不可为空
--第二个参数是赏金任务的难度等级，可为空，表示任何难度等级。支持：试炼、挑战
function 添加赏金任务白名单( task_name, 难度等级 )
	if type(task_name) ~= "string" then
		LuaLogE("添加赏金任务白名单之任务名参数必须为字符串类型")
		return;
	end
	if type(难度等级) ~= "string" then
		难度等级 = ""
	end
	RunInfo("添加赏金任务白名单，任务名：" .. task_name .. " 难度等级：" .. 难度等级)
	return scmd():AddShangJinWhiteList(task_name, 难度等级)
end

function 清空赏金任务白名单()
	RunInfo("清空赏金任务白名单")
	return scmd():ClrShangJinWhiteList()
end

function 添加赏金目标副本黑名单( fb_id )
	if type(fb_id) ~= "number" then
		LuaLogE("添加赏金目标副本黑名单之副本ID参数必须为数字类型")
		return;
	end
	RunInfo("添加赏金目标副本黑名单，副本ID：" .. tostring(fb_id))
	return scmd():AddShangJinDstFbBlackList(fb_id)
end

function 清空赏金目标副本黑名单()
	RunInfo("清空赏金目标副本黑名单")
	return scmd():ClrShangJinDstFbBlackList()
end

function 添加赏金目标副本固定黑名单( fb_name )
	if type(fb_name) ~= "string" then
		LuaLogE("添加赏金目标副本固定黑名单之副本名参数必须为字符串类型")
		return;
	end
	RunInfo("添加赏金目标副本固定黑名单，副本名：" .. fb_name)
	return scmd():AddShangJinDstFbStableBlackList(fb_name)
end

function 清空赏金目标副本固定黑名单()
	RunInfo("清空赏金目标副本固定黑名单")
	return scmd():ClrShangJinDstFbStableBlackList()
end

function 剩余狩猎券()
	local res = scmd():LeftShouLieQuanCnt()
	RunInfo("剩余狩猎券，返回：" .. tostring(res))
	return res
end

--若是返回-1，表示当前的副本消费模式已无效，原因可能是已无狩猎券或没有足够的银币
--此函数用来判断换角色
function 得到当前有效的副本消费模式()
	local res = scmd():GetCurFbValidConsumeMode()
	RunInfo("得到当前有效的副本消费模式，返回：" .. tostring(res))
	return res
end

function 设置疲劳值( data )
	if type(data) ~= "number" then
		data = 0
	end
	RunInfo("设置疲劳值：" .. tostring(data))
	return scmd():SetFatigueData(data)
end

function 字符串为空( str )
	if type(str) ~= "string" then
		RunInfo("字符串为空")
		return true
	end
	RunInfo("字符串为空，字符串：" .. str)
	return IsStringEmpty(str)
end

function 摧毁静态可打的怪物( obj_name, every_hit_cnt, obj_cnt, delay_time )
	if type(obj_name) ~= "string" then
		LuaLogE("摧毁静态可打的怪物之怪物名必须是字符串类型")
		return 0
	end
	if type(every_hit_cnt) ~= "number" then
		every_hit_cnt = 1
	end
	if type(obj_cnt) ~= "number" then
		obj_cnt = -1
	end
	if type(delay_time) ~= "number" then
		obj_cnt = 700
	end
	RunInfo("摧毁静态可打的怪物，怪物名：" .. obj_name .. " 每次打击次数：" .. tostring(every_hit_cnt) .. " 数量：" .. tostring(obj_cnt))
	return scmd():DestoryStaticBeatableMonster(obj_name, every_hit_cnt, obj_cnt, delay_time)
end

function 自动领取礼包()
	RunInfo("自动领取礼包")
	return scmd():AutoGainGiftBag()
end

function 设置当前BOSS的攻击部位( part_name )
	if type(part_name) ~= "string" then
		LuaLogE("设置当前BOSS的攻击部位之部位名参数必须为字符串类型")
		return;
	end
	RunInfo("设置当前BOSS的攻击部位，部位名：" .. part_name)
	return scmd():SetCurBossHitBodyPart(part_name)
end

function 输出支持的怪物部位信息()
	RunInfo("输出支持的怪物部位信息")
	return scmd():DebuInfo_AllBodyPart()
end

----------------------------------------------
--第一个参数是材料名，字符串类型，不能为空
--第二个参数是材料获取处理函数，函数类型，不能为空
--后面可以添加任意参数，用来传递给获取处理函数
function 添加材料获取映射( material_name, gain_func, ... )
	if type(material_name) ~= "string" then
		LuaLogE("添加材料获取映射之材料名参数必须是字符串类型")
		return false
	end
	if type(gain_func) ~= "function" then
		LuaLogE("添加材料获取映射之获取函数参数必须是函数类型")
		return false
	end
	RunInfo("添加材料获取映射，材料名：" .. material_name)
	return MM_AddItem(material_name, gain_func, ...)
end

function 清空材料获取映射()
	RunInfo("清空材料获取映射")
	return MM_Clear()
end

function 存在指定的材料获取映射( material_name )
	if type(material_name) ~= "string" then
		LuaLogE("存在指定的材料获取映射之材料名参数必须是字符串类型")
		return false
	end
	RunInfo("存在指定的材料获取映射")
	return MM_ExistTheItem(material_name)
end

--第一个参数是材料名
--第二个参数是材料数量上限，如果已拥有的材料数量大于等于该上限则不去获取该材料了，可为空，表示无上限
function 去获取指定的材料( material_name, cnt_limit )
	if type(material_name) ~= "string" then
		LuaLogE("去获取指定的材料之材料名参数必须是字符串类型")
		return false
	end
	if type(cnt_limit) ~= "number" then
		cnt_limit = -1
	end
	RunInfo("去获取指定的材料，材料名：" .. material_name .. " 材料数量上限：" .. tostring(cnt_limit))
	return scmd():MM_DoGainTheMaterial(material_name, cnt_limit)
end
----------------------------------------------
function 领取周狩猎券()
	RunInfo("领取周狩猎券")
	return scmd():GainWeekShouLieQuan()
end

function 添加采集黑名单( collect_name )
	if type(collect_name) ~= "string" then
		LuaLogE("添加采集黑名单之采集名参数必须是字符串类型")
		return
	end
	RunInfo("添加采集黑名单，名字：" .. collect_name)
	return scmd():AddCollectBlackList(collect_name)
end

function 清空采集黑名单()
	RunInfo("清空采集黑名单")
	return scmd():ClrCollectBlackList()
end

function 遍历执行_灭火玉( func, cnt )
	RunInfo("遍历执行_灭火玉")
	return scmd():DoEveryObj_Bucket(func, cnt)
end

function 在城镇中()
	RunInfo("在城镇中")
	return scmd():IsInVillage()
end

function 在副本中()
	RunInfo("在副本中")
	return scmd():IsInFb()
end

function 获取当前所穿武器()
	RunInfo("获取当前所穿武器")
	return scmd():GetSelfWeapon()
end

function 角色已进入游戏后( func, ... )
	if type(func) ~= "function" then
		LuaLogE("角色已进入游戏后之处理函数参数必须是函数类型")
		return false
	end
	RunInfo("角色已进入游戏后")
	return LuaHandleRoleEnteredGame(func, ...)
end

function 处理游戏场景( func, ... )
	if type(func) ~= "function" then
		LuaLogE("处理游戏场景之处理函数参数必须是函数类型")
		return false
	end
	RunInfo("处理游戏场景")
	return LuaHandleGameScene(func, ...)
end

function 是BOSS对象( obj )
	RunInfo("是BOSS对象")
	return scmd():IsTheBoss(obj)
end

function 设置不去保持购买之财富限制( min_gold, min_silver )
	if type(min_gold) ~= "number" then
		LuaLogE("设置不去保持购买之财富限制之金币限制参数必须是数字类型")
		return
	end
	if type(min_silver) ~= "number" then
		LuaLogE("设置不去保持购买之财富限制之银币限制参数必须是数字类型")
		return
	end
	RunInfo("设置不去保持购买之财富限制，金币限制：" .. tostring(min_gold) .. " 银币限制：" .. tostring(min_silver))
	return scmd():SetMinMoneyToKeepBuy(min_gold, min_silver)
end

function 打开某个城镇的某个NPC( village_name, npc_name, npc_title, room_num, cur_village_first )
	if type(village_name) ~= "string" then
		LuaLogE("打开某个城镇的某个NPC之城镇名参数必须是字符串类型")
		return false
	end
	if type(npc_name) ~= "string" then
		LuaLogE("打开某个城镇的某个NPC之NPC参数必须是字符串类型")
		return false
	end
	if type(npc_title) ~= "string" then
		npc_title = ""
	end
	if type(room_num) ~= "number" then
		room_num = 0
	end
	if type(cur_village_first) ~= "boolean" then
		cur_village_first = false
	end
	RunInfo("打开某个城镇的某个NPC，城镇：" .. village_name .. " NPC：" .. npc_name .. " 房间号：" .. tostring(room_num) .. " 是否优先在本城镇查找NPC:" .. tostring(cur_village_first))
	return scmd():OpenTheNpcOfVillage(village_name, room_num, npc_name, npc_title, cur_village_first)
end

function 打开某个城镇的指定NPC( village_name, npc_id, room_num, cur_village_first )
	if type(village_name) ~= "string" then
		LuaLogE("打开某个城镇的指定NPC之城镇名参数必须是字符串类型")
		return false
	end
	if type(npc_id) ~= "number" then
		LuaLogE("打开某个城镇的指定NPC之NpcId参数必须是数字类型")
		return false
	end
	if type(room_num) ~= "number" then
		room_num = 0
	end
	if type(cur_village_first) ~= "boolean" then
		cur_village_first = false
	end
	RunInfo("打开某个城镇的指定NPC，城镇：" .. village_name .. " NpcId：" .. tostring(npc_id) .. " 房间号：" .. tostring(room_num) .. " 是否优先在本城镇查找NPC:" .. tostring(cur_village_first))
	return scmd():OpenVillageNpcById(village_name, room_num, npc_id, cur_village_first)
end

function 得到发布任务的NpcId( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("得到发布任务的NpcId之任务ID参数必须是数字类型")
		return -1
	end
	RunInfo("得到发布任务的NpcId,任务ID：" .. tostring(task_id))
	return scmd():GetTaskNpcId(true, task_id)
end

function 得到提交任务的NpcId( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("得到提交任务的NpcId之任务ID参数必须是数字类型")
		return -1
	end
	RunInfo("得到提交任务的NpcId,任务ID：" .. tostring(task_id))
	return scmd():GetTaskNpcId(false, task_id)
end

--第一个参数是怪物名，字符串类型，不可为空
--第二个参数要杀的数量，数字类型，可为空，为空表示杀光该名字的怪物
function 杀死指定名字和数量的怪物( obj_name, kill_cnt, use_item_name, steps_at_use_item )
	if type(obj_name) ~= "string" then
		LuaLogE("杀死指定名字和数量的怪物之怪物名参数必须是字符串类型")
		return 0
	end
	if type(kill_cnt) ~= "number" then
		kill_cnt = -1
	end
	if type(use_item_name) ~= "string" then
		use_item_name = ""
	end
	if type(steps_at_use_item) ~= "number" then
		steps_at_use_item = 1
	end
	RunInfo("杀死指定名字和数量的怪物,怪物名：" .. obj_name .. " 要杀死的数量：" .. tostring(kill_cnt))
	return scmd():KillTheMonsterAsCnt(obj_name, kill_cnt, use_item_name, steps_at_use_item)
end

function 根据当前任务自动设置当前BOSS的攻击部位( task_id )
	if type(task_id) ~= "number" then
		LuaLogE("根据当前任务自动设置当前BOSS的攻击部位之任务ID参数必须是数字类型")
		return
	end
	RunInfo("根据当前任务自动设置当前BOSS的攻击部位,任务ID：" .. tostring(task_id))
	return scmd():AutoSetCurBossHitBodyPartAsTheTask(task_id)
end

function 添加好友( role_name )
	if type(role_name) ~= "string" then
		LuaLogE("添加好友之角色名参数必须是字符串类型")
		return
	end
	RunInfo("添加好友,角色名：" .. role_name)
	return scmd():AddNewFriend(role_name)
end

function 获取错误_副本服务器已满次数()
	local res = scmd():GetError_FbSerIsFull()
	RunInfo("获取错误_副本服务器已满次数,次数：" .. tostring(res))
	return res
end

--参数是顶级脚本名，不能为空，必须是字符串类型
function 进行更换指定的脚本( script_name )
	if type(script_name) ~= "string" then
		LuaLogE("进行更换指定的脚本之顶级脚本名参数必须是字符串类型")
		return
	end
	RunInfo("进行更换指定的脚本：" .. script_name)
	return scmd():DoChangeRunScriptLevel0(script_name)
end

function 得到将要更换的顶级脚本名()
	local res = scmd():GetNextChangeScriptNameLevel0()
	RunInfo("得到将要更换的顶级脚本名：" .. res)
	return res
end

function 得到拥有的金币()
	local res = scmd():GetRoleCurGold()
	RunInfo("得到拥有的金币：" .. tostring(res))
	return res
end

function 得到拥有的银币()
	local res = scmd():GetRoleCurSilver()
	RunInfo("得到拥有的银币：" .. tostring(res))
	return res
end

function 设置要选择的第几个服务器线路( ser_line )
	if type(ser_line) ~= "number" then
		LuaLogE("设置要选择的第几个服务器线路之服务器线路参数必须是数字类型")
		return
	end
	RunInfo("设置要选择的第几个服务器线路，服务器线路：" .. tostring(ser_line))
	return scmd():SetCurSerLine(ser_line)
end

function 进行面对面交易( func_pre_deal, func_do_deal, func_after_deal )
	RunInfo("进行面对面交易")
	return scmd():DoDealFaceToFace(func_pre_deal, func_do_deal, func_after_deal)
end

function 面对面交易_请求交易( player_name, wait_time )
	if type(player_name) ~= "string" then
		LuaLogE("面对面交易_请求交易之目标交易对象参数必须是字符串类型")
		return false
	end
	if type(wait_time) ~= "number" then
		wait_time = 1000 * 60 * 2
	end
	RunInfo("面对面交易_请求交易，目标玩家：" .. player_name .. " 等待时间：" .. tostring(wait_time))
	return scmd():Deal_RequestDeal(player_name, wait_time)
end

function 面对面交易_等待并接受一个交易请求( time_out )
	if type(time_out) ~= "number" then
		time_out = 1000 * 60
	end
	RunInfo("面对面交易_等待并接受一个交易请求，超时时间：" .. tostring(time_out))
	return scmd():Deal_UntilAcceptOneDealRequest(time_out)
end

--第一个参数是个表，类似这样：{ { "怪鸟之翼", 1 }, { "一角龙之翼", -1 } }，其中数字1表示交易怪鸟之翼的数量为1，-1表示交易所有一角龙之翼
--第二个参数是要交易的金币，数字类型，不可为空，为0表示不交易金币，如果是-100，表示保留100金币，其余全都交易
function 面对面交易_移到物品和金币到交易栏( tbl_items, gold )
	if type(gold) ~= "number" then
		LuaLogE("面对面交易_移到物品和金币到交易栏之金币参数必须是数字类型")
		return false
	end
	RunInfo("面对面交易_等待并接受一个交易请求，金币:" .. tostring(gold))
	return scmd():Deal_MoveSomeItems_Gold(tbl_items, gold)
end

function 面对面交易_锁定交易栏()
	RunInfo("面对面交易_锁定交易栏")
	return scmd():Deal_LockDealTab()
end

function 面对面交易_确认交易()
	RunInfo("面对面交易_确认交易")
	return scmd():Deal_EnterDeal()
end

function 是泥人状态()
	local res = scmd():IsNiRenState()
	RunInfo("是泥人状态：" .. tostring(res))
	return res
end

function 处理泥人状态( time_out )
	if type(time_out) ~= "number" then
		LuaLogE("处理泥人状态之超时时间必须是数字类型")
		return false
	end
	RunInfo("处理泥人状态，超时时间：" .. tostring(time_out))
	return scmd():HandleNiRenState(time_out)
end

function 是燃烧状态()
	local res = scmd():IsRanShaoStatus()
	RunInfo("是燃烧状态：" .. tostring(res))
	return res
end

function 处理燃烧状态( time_out )
	if type(time_out) ~= "number" then
		LuaLogE("处理燃烧状态之超时时间必须是数字类型")
		return false
	end
	RunInfo("处理燃烧状态，超时时间：" .. tostring(time_out))
	return scmd():HandleRanShaoStatus(time_out)
end

function 是恶臭状态()
	local res = scmd():IsErChouStatus()
	RunInfo("是恶臭状态：" .. tostring(res))
	return res
end

function 是雪人状态()
	local res = scmd():IsSnowManStatus()
	RunInfo("是雪人状态：" .. tostring(res))
	return res
end

function 处理雪人状态( time_out )
	if type(time_out) ~= "number" then
		LuaLogE("处理雪人状态之超时时间必须是数字类型")
		return false
	end
	RunInfo("处理雪人状态，超时时间：" .. tostring(time_out))
	return scmd():HandleSnowManState(time_out)
end

--[[
可以控制打一次副本消耗的是什么
第一个参数是消费模式，0表示只消耗狩猎券，1表示只消耗银币，2表示先消耗狩猎券，狩猎券用完后再用银币
第二个参数是最低银币，只有当前拥有的银币大于等于这个参数，才会使用银币进入副本
]]
function 设置副本消费模式( mode, min_silver )
	if type(mode) ~= "number" then
		LuaLogE("设置副本消费模式之消费模式参数必须是数字类型")
		return
	end
	if type(min_silver) ~= "number" then
		LuaLogE("设置副本消费模式之最低银币参数必须是数字类型")
		return
	end
	RunInfo("设置副本消费模式，模式：" .. tostring(mode) .. " 最低银币：" .. tostring(min_silver))
	return scmd():SetFbConsumeMode(mode, min_silver)
end

function 是缠绕状态()
	local res = scmd():IsChanRaoStatus()
	RunInfo("是雪人状态：" .. tostring(res))
	return res
end

function 处理缠绕状态( time_out )
	if type(time_out) ~= "number" then
		LuaLogE("处理缠绕状态之超时时间必须是数字类型")
		return false
	end
	RunInfo("处理缠绕状态，超时时间：" .. tostring(time_out))
	return scmd():HandleChanRaoStatus(time_out)
end

function 测试输出装备的材料出处信息()
	RunInfo("测试输出装备的材料出处信息")
	return scmd():TestOutEquipMaterialFrom()
end
