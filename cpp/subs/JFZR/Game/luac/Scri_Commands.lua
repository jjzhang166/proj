
function 延迟( 毫秒 )
	return Delay(毫秒);
end

--是否结束脚本
function 可以结束脚本()
	assert(false, "不能再用可以结束脚本指定了");
end

--加载脚本文件
function 加载脚本文件( file_name )
	return LoadScriptFile(file_name);
end

function 空函数()
end

function 加载任务目录脚本( 脚本名 )
	return LoadScriptFile("任务\\" .. 脚本名);
end
-------------------------------------------------------
--杀死所有怪物
function 杀死所有怪物()
	return KillAllMonster();
end

function 过房间门( room_idx )
	return StepRoomDoor(room_idx);
end

function 拾取所有物品()
	return PickUpAllItems();
end

function 拾取所有金币()
	return PickUpAllGold();
end

function 打完所有房间( func )
	return FightThroughAllRooms(func);
end

function 全图打副本( func )
	return FightAllGraph(func);
end

function 修理已穿装备()
	UpdateAllGameObjs();
	local gp = GPlayer_Me();
	gp:RepairAllWearEquip();
end

function 自动更换装备()
	local gps = UpdateRole();
	if gps == nil then
		LuaLog("更新角色失败");
		return;
	end
	local packet = gps:GetPacket();
	if not packet:AutoReplaceEquip() then
		LuaLog("自动更换装备失败");
	end
end

function 自动升级所有技能()
	return AutoLvlUpSkills();
end

function 自动学习和升级所有技能()
	return AutoLearnLvlUpSkills();
end

function 返回城镇()
	UpdateMapRoom();
	local gps = GPlayer_Me();
	gps:ReturnCity();
end

function 自动提交完成任务()
	return AutoCommitCompleteTasks();
end

function 接受指定任务( quest_ch_name )
	return AcceptTheTask(quest_ch_name);
end

function 存在指定的已接任务( quest_ch_name )
	return IsAcceptedTheTask(quest_ch_name);
end

function 指定的已接任务已完成( quest_ch_name )
	return TheAcceptedTaskCompletable(quest_ch_name);
end

function 移动到指定的城镇( village_ch_name )
	return MoveToTheVillage(village_ch_name);
end

function 移动到指定的副本( fb_ch_name )
	return MoveToTheCloneMap(fb_ch_name);
end

function 卖掉普通栏所有装备()
	UpdateMapRoom();
	UpdateAllGameObjs();
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("role update failed");
		return;
	end
	if not gps:SellNormalTabAllEquips() then
		LuaLogE("卖掉普通栏所有装备失败");
		return;
	end
end

function 直到进入城镇( delay_time )
	return UntileEnteredCity(delay_time)
end

function 直到进入副本( delay_time )
	return UntileEnteredFb(delay_time)
end

function 学习指定技能( skill_ch_name )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("update role failed");
		return;
	end
	if not gps:LearnTheSkill(skill_ch_name) then
		LuaLogE("学习技能失败");
	end
end

function 直到可以返回城镇( delay_time )
	return UntileCanRetCity(delay_time)
end

function 重置任务处理映射( func )
	return RestTaskHandlerMap(func);
end

--第一个参数是任务名，第二个是做任务的函数
--第三个是角色等级限制，角色等级小于这个角色等级限制，这个任务才会接，才能够得到处理
--第三个参数可以不填或为0，表示不管角色多少等级，都要处理这个相应的任务
function 添加任务处理( quest_ch_name, func, role_lvl_limt )
	return AddTaskHandler(quest_ch_name, func, role_lvl_limt);
end

--第四个参数表示应该如何去完成任务
function 添加任务处理和完成( quest_ch_name, func, role_lvl_limt, complete_func )
	return AddTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func);
end

--第五个参数是任务的关键内容
function 添加详细的职业任务处理( quest_ch_name, func, role_lvl_limt, complete_func, key_sub )
	return AddSpecifyJobTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_sub);
end

--第五个参数是任务的关键内容
--第六个参数表示匹配任务id
function 添加详细的任务处理( quest_ch_name, func, role_lvl_limt, complete_func, key_sub, match_quest_id )
	return AddSpecifyTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_sub, match_quest_id);
end

function 添加详细的分支职业任务处理( quest_ch_name, func, role_lvl_limt, complete_func, key_sub, match_quest_id )
	return AddSpecifySubJobTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_sub, match_quest_id);
end

function 自动做任务处理任务()
	return AutoRunTaskHandlerTasks();
end

function 角色在副本中()
	return RoleInCloneMap();
end

function 角色在城镇中()
	return RoleInTown();
end

function 接受本地图任意一个任务()
	return AcceptLocalMapAnyTask();
end

function 接受本地图任意所有任务()
	return AcceptLocalMapAnyAllTasks();
end

function 索引任务内容的关键部分( sub_contents )
	return IdxTaskKeyContents(sub_contents);
end

function 索引任务ID的关键部分( sub_quest_id )
	return IdxTaskIdKey(sub_quest_id);
end

function 破坏所有固态物品()
	return DestroyAllMisc();
end

function 在选择频道界面()
	return IsSelectChannelScene();
end

function 在选择角色界面()
	return IsSelectRoleScene();
end

function 角色已经进入游戏中()
	return IsRoleInGame();
end

function 随机选择频道()
	return RandomChooseChannel();
end

function 自动创建或选择角色()
	return AutoCreateOrSelectRole();
end

function 得到角色疲劳值()
	return GetRoleFatigueData();
end

function 返回到选择角色界面()
	return RetChooseRoleScene();
end

function 默认转职()
	local gps = GPlayer_Me();
	return gps:ChangeJobAsDlt();
end

function 需要转职()
	local gps = GPlayer_Me();
	return gps:NeedChangeJob();
end

function 添加装备的交易物品过滤器( tbl )
	return AddTradeItemFilter(tbl, 1);
end

function 添加材料的交易物品过滤器( tbl )
	return AddTradeItemFilter(tbl, 2);
end

function 添加消耗品的交易物品过滤器( tbl )
	return AddTradeItemFilter(tbl, 3);
end

function 以指定的价格拍卖( price, cnt, product_cnt, ac_type )
	return SaleProductAsPrice(price, cnt, product_cnt, ac_type);
end

function 按市场价的某个范围拍卖( price_delta, dlt_price, cnt, product_cnt, ac_type )
	return SaleProductAsPriceBase(price_delta, dlt_price, cnt, product_cnt, ac_type);
end

function 保留并提示物品()
	return KeepPromptDiscoverItem();
end

function 邮寄物品( role_name )
	return SendMailItem(role_name);
end

function 卖给商店()
	return SellTheItemToShop();
end

function 分解物品()
	return DisassemblyTheItem;
end
