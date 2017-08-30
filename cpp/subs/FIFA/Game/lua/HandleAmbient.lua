
--点击继续
function HandlePressOnClick_()
	LuaLog("进入到点击继续场景");
	local normal = FuncObj_NormalC_Instance();
	if normal ~= nil and normal.game_state == enGameState.enGameState_Invalid then
		normal.game_state = enGameState.enGameState_PressOnClick;
		normal:LuaContinueOnClick();
	end
end

--notice dialog
function HandleNoticeDlgScene_()
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end

	if normal.game_state == enGameState.enGameState_PressOnClick then
		LuaLog("进入到notice dialog场景");
		normal.game_state = enGameState.enGameState_NoticeDlg;
		normal:LuaCloseNoticeDialog();
	end
end

function 解决教练前的转移数据()
	LeftBtClick(282,346);
	Delay(3000)
	LeftBtClick(520,309);

	--点击决定
	LeftBtClick(964,737);
end

--关闭公告对话框
function CloseNoticeDlg()
	解决教练前的转移数据()
	LeftBtClick(770, 88);
end

--教练选择
function HandleTrainerSelect_( _, coach_list )
	if coach_list ~= nil then
		if coach_list:IsNull() or not coach_list:IsArray() then
			LuaLogE("教练选择时收到不符合条件判定");
			return;
		end

		local normal = FuncObj_NormalC_Instance();
		if normal == nil then
			return;
		end

		if coach_list:Size() == 0 then
			LuaLog("没有教练，需要创建教练");
			normal.game_state = enGameState.enGameState_CreateCoach;
		else
			LuaLog("进入到教练选择场景");
			normal.game_state = enGameState.enGameState_TrainerSelect;
		end
	end
end

--处理第一次进入教练选定场景
function HandleFirstTrainerSelected( recv_handler_obj )
	--[[
	--处理选定场景
	HandleTrainerSelected_();

	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end

	--关闭每日任务dlg
	normal:LuaCloseTaskDlg();
	--]]
end

--教练选定
function HandleTrainerSelected_()
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal.game_state = enGameState.enGameState_TrainerSelected;
	if normal.game_state == enGameState.enGameState_TrainerSelected then
		LuaLog("进入到教练选定场景");
	end
	--执行这个函数，是为了能够进行下一步的任务，但是这个函数貌似是没必要的了，以后再说吧
	--normal:LuaTrainerSelectedScene();
end

--处理点击友谊赛
function HandlePressFriendshipMatch()
	--点击友谊赛
	LeftBtClick(368, 588);
	--test，以后下面的执行代码是由其他函数或事件来触发的
	--选择经理人模式
	local match_mgr = GMatchMgr_Instance();
	match_mgr:ChooseMatch(enMatchMode.enMatchMode_Managers);
end

--处理点击经理人模式
function HandlePressManagersMode()
	--点击经理人
	LeftBtClick(914, 562);
	Delay(2000)
end

--处理进入比赛房间
function HandleEnterMatchRoom( template_id )
	--重置匹配
	MultiStrMatchOnTime_Me():ResetMatch(template_id);

	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	--[[
	if normal.game_state ~= enGameState.enGameState_TrainerSelected then
		return;
	end
	--]]

	--进入指定的比赛房间
	local match_mgr = GMatchMgr_Instance();

	--test，以后下面的执行代码是由其他函数或事件来触发的
	--选择经理人模式
	match_mgr:ChooseMatch(enMatchMode.enMatchMode_Managers);

	local match_mode = match_mgr:MatchMode();
	match_mode:EnteredRoom();

	--test，以后下面的执行代码是由其他函数或事件来触发的
	--选择对阵电脑模式
	match_mode:ChooseFightMode(enFightMode.enFightMode_Computer);
end

--处理点击经理人模式之对阵电脑
function HandlePressMangers_Computer()
	LeftBtClick(584, 476);
end

--处理点击经理人模式之对阵好友
function HandlePressMangers_Friend()
	LeftBtClick(591, 454);
end

--处理选择了经理人模式之对阵电脑
function HandleSelectManagers_Computer()
	LuaLog("选择了经理人模式之对阵电脑");
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal.game_state = enGameState.enGameState_Managers_Computer;

	--[[
	local match_mgr = GMatchMgr_Instance();
	local match_mode = match_mgr:MatchMode();
	match_mode:ChooseFightMode(enFightMode.enFightMode_Computer);

	--test，以后下面的执行代码是由其他函数或事件来触发的
	--开始比赛
	match_mode:FightMode():BeginMatch();
	--]]
end

--处理选择了经理人模式对阵好友
function HandleSelectManagers_Friend( recv_handler_obj )
	LuaLog("选择了经理人模式之对阵好友");
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal.game_state = enGameState.enGameState_Managers_Friend;

	--[[
	local match_mgr = GMatchMgr_Instance();
	local match_mode = match_mgr:MatchMode();
	match_mode:ChooseFightMode(enFightMode.enFightMode_Friend);
	--]]
end

--处理选择了经理人模式随机对手
function HandleSelectManagers_Random()
	LuaLog("选择了经理人模式之对阵随机好友");
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal.game_state = enGameState.enGameState_Managers_Random;
end

--处理进入比赛之等待
function HandleEnterMatchWaiting(url, send_json_msg)
	if send_json_msg == nil or send_json_msg:IsNull() then
		return;
	end
	local json_stage =  send_json_msg:Key("stage")
	if json_stage:IsNull() or not json_stage:IsString() then
		return
	end
	local stage_value = json_stage:AsString()
	if stage_value == "created" then
		local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
		fight_mode:EnterWaiting()
	end
end

--处理进入比赛之选择球员
function HandleEnterMatchChoosePlayer( recv_handler_obj )
	local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
	fight_mode:EnterChoosePlayer();
end

--处理进入比赛之就绪或比赛动画状态
function HandleEnterMatchReadyOrAnimate( url, send_json_msg )
	if send_json_msg == nil or send_json_msg:IsNull() then
		return;
	end
	local json_stage =  send_json_msg:Key("stage")
	if json_stage:IsNull() or not json_stage:IsString() then
		return
	end
	local stage_value = json_stage:AsString()
	if stage_value == "squad" then
		local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
		fight_mode:EnterMatchReady();
	elseif stage_value == "setting" then
		local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
		fight_mode:EnterMatchAllReady();
	end
end

--选择设置 控制设定 ，按确认后发的包
function HandleControlSetting( uri, send_json_msg )
	L_HandleControlSetting()
end

function HandleScanSoldPlayer( _, send_json_msg, http_request )
	L_HandleScanSoldPlayer(http_request)
end

--处理右下角按钮
function HandlePressRightBottomBt()
	LeftBtClick(755, 580);
end

--处理进入比赛之所有人都已就绪，即进入到了游戏开始的动画
function HandleMatch_AllReady()
	local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
	fight_mode:EnterMatchAllReady();
end

--处理获得房间ID
function HandleGainRoomId( recv_handler_obj, last_json_value )
	if last_json_value == nil or not last_json_value:IsInt() then
		--LuaLogE("LUA中获得房间ID错误啦");
		return;
	end
	--设置房间ID
	GMatchMgr_Instance():MatchMode():GetMatchInfo().room_id = last_json_value:AsInt();
end

--处理获得比赛ID
function HandleGainMatchId( recv_handler_obj, last_json_value )
	if last_json_value == nil or not last_json_value:IsInt() then
		--LuaLogE("LUA中获得比赛ID错误啦");
		return;
	end

	--设置比赛ID
	GMatchMgr_Instance():MatchMode():GetMatchInfo().match_id = last_json_value:AsInt();
end

--处理比赛结束
function HandleMatchEnd()
	local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
	fight_mode:MatchEnd();
end

--处理比赛结束之比赛奖励
function HandleMatchEnd_Prize()
	local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
	fight_mode:MatchEnd_Prize();
end

--处理比赛结束之返回比赛房间
function HandleMatchEnd_Back( handler_obj )
	local fight_mode = GMatchMgr_Instance():MatchMode():FightMode();
	fight_mode:MatchEnd_Back();

	--循环踢比赛
	--LuaLogT("循环踢比赛");
	--GMatchMgr_Instance():MatchMode():FightMode():BeginMatch();
end

--初始化之处理获得EP
function Handle_GainEpFromServer( recv_handler_obj, exp_json_int )
	if  exp_json_int == nil or not exp_json_int:IsInt() then
		LuaLogW("获得EP，不符合条件判定");
		return;
	end

	GPlayerMe().ep = exp_json_int:AsInt();
end

--初始化之处理获得背包物品数据
function Handle_GainBagItems( recv_handler_obj, bag_items_json_array )
	if bag_items_json_array == nil or not bag_items_json_array:IsArray() then
		LuaLogW("获得背包物品，不符合条件判定");
		return;
	end

	GPlayerMe():SetBagItems(bag_items_json_array);
end

--初始化之处理获得教练名称
function HandleInit_GainName( recv_handler_obj, trainer_name_json_str )
	if trainer_name_json_str == nil or not trainer_name_json_str:IsString() then
		LuaLogW("获得教练名称，不符合条件判定");
		return;
	end

	GPlayerMe().name = trainer_name_json_str:AsString();
end

--初始化之处理人物等级数据
function Handle_GainLevelInfo( recv_handler_obj, level_info_json_obj )
	GPlayerMe():SetJsonLevelInfo(level_info_json_obj);
end

--处理点击dialog之续约
function HandlePressDlg_Contract()
	LeftBtClick(365, 461);
end

--处理点击dialog之完成续约
function HandlePressDlg_DoneContract()
	LeftBtClick(400, 348);
end

--处理比赛房间之续约dialog
function HandleMatchRoom_ContractDlg( recv_handler_obj )
	--LuaLogT("弹出续约对话框");
	--GMatchMgr_Instance():MatchMode():MatchRoom_CloseContractDlg();
	----[[
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal.game_state = enGameState.enGameState_Managers_Contract;
	--]]
end

--关闭比赛房间之完成续约DLG
function CloseMatchRomm_DoneContractDlg()
	LeftBtClick(400, 348);
end

--处理升级
function HandleLevelUp()
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal:CloseLevelUpDlg(500, 2);
end

--获得球员信息
function HandleInit_GainPlayers( _, player_json_array )
	GPlayerMe():Init_SetAllPlayers(player_json_array);
end

--比赛结束后获得球员的信息
function HandleMatchEnd_GainPlayers( _, player_json_array )
	GPlayerMe():MatchEnd_UpdatePlayers(player_json_array);
end

--返回到教练选定场景
function ReturnSelectedScene()
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		LuaLogE("返回到教练选定场景，func obj normal没有创建");
		return;
	end
	if normal.game_state == enGameState.enGameState_Managers then
		PressManagers_Back();
	end

	PressRightTop_ReturnSelected();
end

--点击经理人房间之后退
function PressManagers_Back()
	LeftBtClick(791, 748);
end

--点击右上角按钮
function PressRightTopButton()
	LeftBtClick(971, 68);
end

--点击右上角之返回选定场景
function PressRightTop_ReturnSelected()
	PressRightTopButton();
end

--点击俱乐部管理
function PressClubManage()
	--[[
	local game_state = GetGameState();
	if game_state ~= enGameState.enGameState_TrainerSelected then
		LuaLogE("当前状态是：" .. tostring(game_state) .. "不能点击俱乐部管理");
		return;
	end
	--]]
	LeftBtClick(460, 587);
end

--得到游戏状态
function GetGameState()
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return enGameState.enGameState_Invalid;
	end
	return normal.game_state;
end

--设置游戏状态
function SetGameState( new_game_state )
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		LuaLogE("LUA设置游戏状态，并没有找到func ob normal");
		return;
	end
	normal.game_state = new_game_state;
end

--点击球队管理
function PressTeamManage()
	LeftBtClick(220, 59);
end

--点击自动选拔球员
function PressAutoSelectPlayers()
	LeftBtClick(188, 208);
end

--处理进入俱乐部管理场景
function HandleEnterClubManage()
	LuaLogT("进入到俱乐部管理状态");
	SetGameState(enGameState.enGameState_ClubManage);
end

--处理进入自动选拔球员状态
function HandleEnterAutoSelectPlayer()
	LuaLogT("进入到自动选择球员状态");
	SetGameState(enGameState.enGameState_AutoSelectPlayer);
end

--处理初始化之球员职位信息，和比赛结束后更新的信息
function HandleUpdatePlayersPositions( _, player_json_array )
	if player_json_array == nil then
		LuaLogE("player_json_array不能为空");
		return;
	end
	GPlayerMe():SetFirstSecondPlayers(player_json_array);
end

function HandleUpdateTotalPlayersCnt( _, players_cnt )
	if players_cnt == nil then
		LuaLogE("players_cnt不能为空");
		return;
	end
	GPlayerMe():SetTotalPlayersCnt(players_cnt);
end

--使用物品
function UseItemByItemId( item_id )
	local item = GPlayerMe():FindItemByItemId(item_id);
	if item == nil then
		LuaLogE("UseItemByItemId,没有item_id为：" .. item_id .. "的物品");
		return;
	end
	item:Use();
end

--是否可以使用物品
function CanUseItemByItemId( item_id )
	local item = GPlayerMe():FindItemByItemId(item_id);
	if item_id == nil then
		LuaLogE("CanUseItemByItemId,没有item_id为：" .. item_id .. "的物品");
		return false;
	end
	return item:CanUse();
end

--发包续约
function ContractBySendPackage()
	if not GPlayerMe():ContractFirstAndSecondPlayers() then
		LuaLogE("发包续约失败");
	end
end

--按键续约
function ContractByPress()
	LeftBtClick(375, 471);
	LeftBtClick(375, 455);
	CloseMatchRomm_DoneContractDlg();
	Delay(500);
	PressEsc();
end

----[[
--自动选拔球员
function AutoSelectPlayersByPress()
	--从比赛房间退出到选定状态
	PressEsc();
	Delay(500);
	PressManagers_Back();
	Delay(500);
	PressRightTop_ReturnSelected();

	AutoSelectPlayersFromSelected();
	--return selected
	PressRightTop_ReturnSelected();

	EnterManagersRoomFromSelected();
end
--]]
--是否在比赛房间
function IsInMatchRoom()
	local game_state = GetGameState();
	return game_state == enGameState.enGameState_Managers or game_state == enGameState.enGameState_Managers_Contract or
	game_state == enGameState.enGameState_Managers_Computer or game_state == enGameState.enGameState_Managers_Friend
	or game_state == enGameState.enGameState_Managers_Random;
end

--从选定状态进入到经理人房间
function EnterManagersRoomFromSelected()
	while not IsInMatchRoom() do
		Delay(500);
		选择友谊赛();
		Delay(500);
		选择经理人模式();
	end
end

--从选定状态进入到俱乐部管理，然后自动选拔球员
function AutoSelectPlayersFromSelected()
	while GetGameState() ~= enGameState.enGameState_ClubManage do
		PressClubManage();
		Delay(500);
	end

	PressTeamManage();
	Delay(500);
	PressAutoSelectPlayers();
	Delay(500);
end

--使用物品
function UseItem( item_id )
	local item = GPlayerMe():FindItemByItemId(item_id);
	if item == nil then
		return false;
	end
	return item:Use();
end

--是否有指定的物品
function HasItem( item_id )
	local item = GPlayerMe():FindItemByItemId(item_id);
	if item == nil then
		return false;
	else
		return item.remains > 0;
	end
end

--EP卡数量
function EPCardCnt()
	local item = GPlayerMe():FindItemByItemId(ItemId_EpCard());
	if item == nil then
		return 0;
	else
		return item.remains;
	end
end

--新幸运EP卡数量
function NewEPCardCnt()
	local item = GPlayerMe():FindItemByItemId(ItemId_EpCardNew());
	if item == nil then
		return 0;
	else
		return item.remains;
	end
end

--开EP卡
function OpenCard_EP( cnt )
	if cnt <= 0 then
		LuaLogE("OpenCard_EP之参数为" .. cnt);
		return;
	end
	local item_cnt = EPCardCnt();
	if item_cnt < cnt then
		cnt = item_cnt;
	end
	local gp = GPlayerMe();

	for i = 1, cnt do
		local item = gp:FindItemByItemId(ItemId_EpCard());
		if item == nil then
			LuaLogE("item竟然为空，应该执行不到才是");
			return;
		end
		item:Use();
	end
end

--开新幸运EP卡
function OpenCardNew_EP( cnt )
	if cnt <= 0 then
		LuaLogE("OpenCardNew_EP之参数为" .. cnt);
		return;
	end
	local item_cnt = NewEPCardCnt();
	if item_cnt < cnt then
		cnt = item_cnt;
	end
	local gp = GPlayerMe();

	for i = 1, cnt do
		local item = gp:FindItemByItemId(ItemId_EpCardNew());
		if item == nil then
			LuaLogE("item竟然为空，应该执行不到才是");
			return;
		end
		item:Use();
	end
end

--是否有自动比赛券
function IsHasAutoMatchTicket()
	return GPlayerMe():HasAutoMatchTicket();
end

--自动赛程
function HandleAutoMatch()
	GPlayerMe():SendAutoMatch();
end

--得到所有球员的数量
function GetAllPlayersCnt()
	return GPlayerMe():GetAllPlayersCnt();
end

--设置对手信息
function HandleSetEnemyInfo( _, json_obj )
	LuaLog("选择弱队");
	if json_obj == nil or not json_obj:IsObject() then
		LuaLogE("设置对手信息时，不符合条件判定");
		return;
	end
	GPlayerMe():SetEnemyInfo(json_obj);
end

--点击DX按键
function PressDxinputKey( di_key )
	local normal_c = FuncObj_NormalC_Instance();
	if normal_c ~= nil then
		normal_c:PressDxinputKey(di_key);
	end
end

--设置首发球员
function HandleSetFirstPlayers( _, json_players )
	GPlayerMe():SetFirstPlayers(json_players);
end

--设置替补球员
function HandleSetSecondPlayers( _, json_players )
	GPlayerMe():SetSecondPlayers(json_players);
end

--点击创建教练
function PressCreateCoach()
	LeftBtClick(416, 377);
end

--点击创建教练dlg之focus input edit control
function PressCreateCoach_FocusInput()
	LeftBtClick(597, 357);
end

--得到创建教练子状态
function GetCC_SubState()
	return GPlayerMe().cc_state;
end

--是否可以接收输入教练名字后返回信息
function CanHandleInputCoachName()
	return GetCC_SubState() == enCreateCoach_SubState.enCC_SubState_InputName;
end

--输入教练名成功
function HandleInputCoachNameSucceed()
	LuaLog("输入教练名成功");
	local gplayer = GPlayerMe();
	gplayer.cc_state = enCreateCoach_SubState.enCC_SubState_SelectTeam;
	gplayer.create_coach_succeed = true;
end

--输入教练名失败
function HandleInputCoachNameFail()
	LuaLog("输入教练名失败");
	local gplayer = GPlayerMe();
	gplayer.cc_state = enCreateCoach_SubState.enCC_SubState_Invalid;
	gplayer.create_coach_succeed = false;
end

--设置成就信息
function HandleAchievementInfo(_, achievement )
	GPlayerMe():SetAchievementInfo(achievement);
end

--初始化之处理获得GP
function Handle_GainGpFromServer( _, gp_json_int )
	if  gp_json_int == nil or not gp_json_int:IsInt() then
		LuaLogW("获得GP，不符合条件判定");
		return;
	end

	GPlayerMe().gp = gp_json_int:AsInt();
end

--处理请求获得GP礼包数据
function HandleGetGiftPackageData()
	LuaLogT("请求获得GP礼包数据");
	GPlayerMe().getting_gp_data = true;
end

--是否正在获得GP礼包数据
function IsGettingGpData()
	return GPlayerMe().getting_gp_data;
end

--处理接收到的GP数据
function HandleRecvGpData( _, gp_data )
	local gplayer = GPlayerMe();
	if gplayer:SetGiftPackageInfo(gp_data) then
		gplayer.getting_gp_data = false;
	end
end

--处理疲劳
function HandleMatchAddict( _, is_addict )
	if  is_addict == nil or not is_addict:IsBool() then
		LuaLogW("处理疲劳，不符合条件判定");
		return;
	end

	GPlayerMe():SetMatchAddict(is_addict:AsBool());
end

--封号
function HandleBanAccount( _, _ )
	local normal = FuncObj_NormalC_Instance();
	if normal ~= nil then
		normal:BanAccount();
	end
end
