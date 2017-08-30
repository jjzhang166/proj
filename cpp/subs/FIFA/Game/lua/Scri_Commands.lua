--此文件提供命令

--得到游戏状态
function 游戏状态()
	RunInfo("游戏状态")
	return GetGameState();
end

--是否教练选择状态
function 可以选择教练()
	RunInfo("可以选择教练")
	HandleElapseTimers()
	return GetGameState() == enGameState.enGameState_TrainerSelect;
end

--在教练选择场景中，进入游戏
function 进入游戏()
	RunInfo("进入游戏")
	--弹框
	--LeftBtClick(400, 349);
	--进入游戏
	LeftBtClick(754, 281);
	--控制设定 选择确认
	L_HandlePressControlSetting()
end

--是否教练选定状态
function 进入到游戏()
	RunInfo("进入到游戏")
	return GetGameState() == enGameState.enGameState_TrainerSelected;
end

--处理关闭每日任务dlg
function 关闭每日任务框()
	RunInfo("关闭每日任务框")
	LeftBtClick(511, 653);
end

--点击友谊赛
function 选择友谊赛()
	RunInfo("选择友谊赛")
	--如果不加延迟，就会进入到道具页面中，不知为何
	Delay(2000)
	--点击多人
	LeftBtClick(275, 751);
	Delay(2000)
	--点击友谊赛
	LeftBtClick(694, 698);
	Delay(2000)
end

--点击经理人模式
function 选择经理人模式()
	RunInfo("选择经理人模式")
	--选择经理人模式
	local match_mgr = GMatchMgr_Instance();
	match_mgr:ChooseMatch(enMatchMode.enMatchMode_Managers);
	--点击经理人模式
	HandlePressManagersMode();
	Delay(1500);
	HandlePressManagersMode();
end

--是否在比赛房间状态
function 在比赛房间()
	RunInfo("在比赛房间")
	return IsInMatchRoom();
end

function 只是在比赛房间()
	RunInfo("只是在比赛房间")
	return enGameState.enGameState_Managers == GetGameState();
end

--是否选择了经理人模式的对阵电脑
function 选择了对阵电脑()
	RunInfo("选择了对阵电脑")
	return GetGameState() == enGameState.enGameState_Managers_Computer;
end

--点击经理人模式之对阵电脑
function 选择对阵电脑()
	RunInfo("选择对阵电脑")
	LeftBtClick(796, 615);
end

--是否选择了经理人模式的对阵好友
function 选择了对阵好友()
	RunInfo("选择了对阵好友")
	return GetGameState() == enGameState.enGameState_Managers_Friend;
end

--点击经理人模式之对阵好友
function 选择对阵好友()
	RunInfo("选择对阵好友")
	LeftBtClick(799, 581);
end

--是否进入到了比赛之选择球员状态
function 可以选择球员进行比赛()
	RunInfo("可以选择球员进行比赛")
	return GetGameState() == enGameState.enGameState_Match_Choose_Player;
end

--是否进入到了比赛之就绪
function 已经比赛就绪()
	RunInfo("已经比赛就绪")
	return GetGameState() == enGameState.enGameState_Match_Ready;
end

--点击右下角按钮
function 点击右下角按钮()
	RunInfo("点击右下角按钮")
	LeftBtClick(981, 750);
end

--是否要开始踢球状态(马上就要进入动画)
function 有比赛动画()
	RunInfo("有比赛动画")
	return GetGameState() == enGameState.enGameState_Match_BeginFirtAnimate;
end

--是否比赛结束状态
function 比赛已经结束()
	RunInfo("比赛已经结束")
	return GetGameState() == enGameState.enGameState_MatchEnd;
end

--是否比赛奖励状态
function 可以选择比赛奖励()
	RunInfo("可以选择比赛奖励")
	return GetGameState() == enGameState.enGameState_MatchEnd_Prize;
end

--[[
--是否比赛结束的返回状态
function IsMatchEnd_BackState()
	return 游戏状态() == enGameState.enGameState_MatchEnd_Back;
end
--]]

--是否比赛房间之续约状态
function 比赛完需要续约()
	RunInfo("比赛完需要续约")
	return GetGameState() == enGameState.enGameState_Managers_Contract;
end

function 进行续约()
	RunInfo("进行续约")
	ContractBySendPackage();
	PressEsc();
end

function 延迟( 毫秒 )
	RunInfo("延迟")
	Delay(毫秒);
end

--开始比赛
function 开始比赛()
	RunInfo("开始比赛")
	if 需要重置数据() or 在比赛房间超时() then
		SetScriptOutputInfo("要回到教练选择画面了");
		回到选择教练场景();
	else
		GMatchMgr_Instance():MatchMode():FightMode():BeginMatch();
		点击右下角按钮();
	end
end

--在比赛房间超时
function 在比赛房间超时()
	RunInfo("在比赛房间超时")
	return GPlayerMe():IsTimeoutAtMatchRoom();
end

--按ESC键
function 按ESC键()
	RunInfo("按ESC键")
	PressEsc();
end

--交换球员
function 交换球员()
	RunInfo("交换球员")
	local stplayer_tbl = {};
	for player in Closure_Iterator(GPlayerMe():CreateExchangePlayerIter()) do
		table.insert(stplayer_tbl, player);
		if table.maxn(stplayer_tbl) == 2 then
			stplayer_tbl[1]:ExchangePlayer(stplayer_tbl[2]);
			return;
		end
	end
	LuaLog("并不能交换球员");
end

--是否有可以交换的球员
function 有可以交换的球员()
	RunInfo("有可以交换的球员")
	return GPlayerMe():IsCanExchangePlayer();
end

--是否需要自动选拔球员
function 需要自动选拔球员()
	RunInfo("需要自动选拔球员")
	return GPlayerMe():IsNeedAutoSelectPlayer();
end

--自动选拔球员
function 自动选拔球员()
	RunInfo("自动选拔球员")
	--AutoSelectPlayersBySend();
	return GPlayerMe():AutoSelectPlayerBySend();
end

--执行自动选拔球员
function 执行自动选拔球员()
	RunInfo("执行自动选拔球员")
	local gp = GPlayerMe();
	if gp:IsNeedAutoSelectPlayer() then
		gp:AutoSelectPlayerBySend();
	end
end

--开球员卡
function 开球员卡( 球员卡类型 )
	RunInfo("开球员卡")
	if 球员卡类型 == "绿色" then
		return UseItem(ItemId_GreenCard());
	elseif 球员卡类型 == "红色" then
		return UseItem(ItemId_RedCard());
	elseif 球员卡类型 == "紫色" then
		return UseItem(ItemId_PurpleCard());
	elseif 球员卡类型 == "蓝色" then
		return UseItem(ItemId_BlueCard());
	else
		LuaLogE("不存在这样的球员卡类型：" .. 球员卡类型);
		return false;
	end
end

--开默认的球员卡
--[[
function 开默认的球员卡()
	RunInfo("开默认的球员卡")
	if 有球员卡("绿色") and 开球员卡("绿色") then
		return;
	elseif 有球员卡("红色") and 开球员卡("红色") then
		return;
	elseif 有球员卡("紫色") and 开球员卡("紫色") then
		return;
	elseif 有球员卡("蓝色") and 开球员卡("蓝色") then
		return;
	end
end
--]]

--是否有球员卡
function 有球员卡( 球员卡类型 )
	RunInfo("有球员卡")
	if 球员卡类型 == "绿色" then
		return HasItem(ItemId_GreenCard());
	elseif 球员卡类型 == "红色" then
		return HasItem(ItemId_RedCard());
	elseif 球员卡类型 == "紫色" then
		return HasItem(ItemId_PurpleCard());
	elseif 球员卡类型 == "蓝色" then
		return HasItem(ItemId_BlueCard());
	else
		return false;
	end
end

--是否有默认的球员卡
function 有默认的球员卡()
	RunInfo("有默认的球员卡")
	if 有球员卡("绿色") then
		return true;
	elseif 有球员卡("红色") then
		return true;
	elseif 有球员卡("紫色") then
		return true;
	elseif 有球员卡("蓝色") then
		return true;
	else
		return false;
	end
end

--开EP卡
function 开EP卡( 数量 )
	RunInfo("开EP卡")
	OpenCard_EP(数量);
end

--EP卡的数量
function EP卡的数量()
	RunInfo("EP卡的数量")
	return EPCardCnt();
end

--新EP卡的数量
function 新EP卡的数量()
	RunInfo("新EP卡的数量")
	return NewEPCardCnt();
end

--开新EP卡
function 开新EP卡( 数量 )
	RunInfo("开新EP卡")
	OpenCardNew_EP(数量);
end

--是否有自动比赛券
function 有自动比赛券()
	RunInfo("有自动比赛券")
	return IsHasAutoMatchTicket();
end

--使用自动比赛券跳过比赛过程
function 使用自动比赛券跳过比赛过程()
	RunInfo("使用自动比赛券跳过比赛过程")
	HandleAutoMatch();
end

--自动比赛券的数量
function 自动比赛券的数量()
	RunInfo("自动比赛券的数量")
	return GPlayerMe():AutoMatchTicketCnt();
end

--使用一些自动比赛券
function 使用一些自动比赛券( cnt )
	RunInfo("使用一些自动比赛券")
	GPlayerMe():UseSomeAutoMatchTicket(cnt);
end

--输出脚本信息
function 输出脚本信息( 信息 )
	RunInfo("输出脚本信息, 信息：" .. 信息)
	--SetScriptOutputInfo(信息);
end

--已拥有的球员的数量
function 已拥有的球员的数量()
	RunInfo("已拥有的球员的数量")
	return GetAllPlayersCnt();
end

--需要解雇球员
function 需要解雇球员()
	RunInfo("需要解雇球员")
	return GPlayerMe():IsNeedFirePlayer();
end

--自动解雇球员
function 自动解雇球员()
	RunInfo("自动解雇球员")
	GPlayerMe():AutoFirePlayers();
end

--是否选择了弱队
function 选择了弱队()
	RunInfo("选择了弱队")
	return GPlayerMe():IsSelectedSickEnemy();
end

--自动选择弱队
function 自动选择弱队()
	RunInfo("自动选择弱队")
	GPlayerMe():AutoSelectSickEnemy();
	选择随机对手();
end

--点击随机对手
function 选择随机对手()
	RunInfo("选择随机对手")
	LeftBtClick(802, 643);
end

--选择了随机对手
function 选择了随机对手()
	RunInfo("选择了随机对手")
	return GetGameState() == enGameState.enGameState_Managers_Random;
end

--取消动画
function 取消动画()
	RunInfo("取消动画")
	assert(false, "不能执行取消动画")
	PressDxinputKey(1);
end

--需要重置数据
function 需要重置数据()
	RunInfo("需要重置数据")
	return GPlayerMe():CanJoinedPlayersCnt() <= 18;
end

function 回到选择教练场景()
	RunInfo("回到选择教练场景")
	--从比赛房间退出到选定状态
	PressEsc();
	Delay(500);
	PressEsc();
	PressManagers_Back();
	Delay(500);

	while not 可以选择教练() do
		PressRightTopButton();
		关闭每日任务框();
		LuaLog("尝试进入选择教练的状态");
		Delay(3000);
	end
end

--是否需要补充候补球员
function 需要补充候补球员()
	RunInfo("需要补充候补球员")
	return GPlayerMe():NeedSupplyThirdPlayers();
end

--补充候补球员
function 补充候补球员()
	RunInfo("补充候补球员")
	GPlayerMe():DegradeAllThirdRubbishPlayers();
	GPlayerMe():FillFullThirdPlayers();
end

--开始创建教练
function 开始创建教练()
	RunInfo("开始创建教练")
	PressCreateCoach();
end

--输入随机教练名
function 输入随机教练名()
	RunInfo("输入随机教练名")
	local normal_c = FuncObj_NormalC_Instance();
	if normal_c == nil then
		return;
	end
	local name = GPlayerMe():GenCoachName();
	if name == nil then
		return;
	end
	local str_len = string.len(name);
	LuaLog("coach name: " .. name);
	for i = 1, str_len do
		normal_c:InputCoachName(string.byte(name, i));
	end
	Delay(500);

	GPlayerMe().cc_state = enCreateCoach_SubState.enCC_SubState_InputName;
	PressReturn();
	Delay(500);
end

--点击创建教练的输入框
function 点击创建教练的输入框()
	RunInfo("点击创建教练的输入框")
	PressCreateCoach_FocusInput();
end

--按回车
function 按回车()
	RunInfo("按回车")
	PressReturn();
end

--是否需要走创建教练流程
function 需要走创建教练流程()
	RunInfo("需要走创建教练流程")
	return GPlayerMe():IsCreateCoachProcedure();
end

--创建教练是否成功
function 创建教练成功()
	RunInfo("创建教练成功")
	return GPlayerMe().create_coach_succeed;
end

--按指定次数的退格键
function 按退格键( cnt )
	RunInfo("按退格键")
	for i = 1, cnt do
		PressBackSpace();
	end
end

function 点击创建俱乐部()
	RunInfo("点击创建俱乐部")
	--点击快速创建
	LeftBtClick(473, 674)
end

function 关闭新手引导()
	RunInfo("关闭新手引导")
	while not 可以选择教练() do
		PressRightTopButton();
		Delay(500);
		--关闭新手引导结束的dlg
		LeftBtClick(362, 431);

		Delay(500);
		PressRightTopButton();
		LuaLog("创建完角色，尝试进入选择教练的状态");
		Delay(3000);
	end
end

function 等待进入到新手引导画面()
	RunInfo("等待进入到新手引导画面")
	local 次数 = 0
	while not 进入到游戏() do
		Delay(2000);
		--点击决定
		LeftBtClick(965, 734);
		--点击确认
		LeftBtClick(890, 545);
		次数 = 次数 + 1
		if 次数 >= 20 then
			local normal = FuncObj_NormalC_Instance();
			if normal == nil then
				return;
			end
			normal:Disconnect();
		end
	end
	LuaLogT("进入游戏成功");
end

function 拍卖球员( ability, price )
	RunInfo("拍卖球员")
	GPlayerMe():SaleSpecifyPlayers(ability, price);
end

--设置不会被交换的最低能力值
function 设置不会被交换的最低能力值( ability )
	RunInfo("设置不会被交换的最低能力值")
	GPlayerMe().ability_filter = ability;
end

--设置不会被交换的最低价值
function 设置不会被交换的最低价值( price )
	RunInfo("设置不会被交换的最低价值")
	GPlayerMe().exch_price_filter = price;
end

--设置高价值球员提示
function 设置高价值球员提示( price )
	RunInfo("设置高价值球员提示")
	GPlayerMe().high_price_filter = price;
end

--是否可以买第几个GP礼包
function 可以买GP礼包( idx )
	RunInfo("可以买GP礼包")
	GPlayerMe():CanBuyGiftPackage(idx);
end

--买第几个GP礼包
function 买GP礼包( idx )
	RunInfo("买GP礼包")
	GPlayerMe():BuyGiftPackageByIdx(idx);
end

function 加载脚本文件( file_name )
	RunInfo("加载脚本文件")
	LoadScriptFile(file_name);
end

--接收保管箱物品
function 接收保管箱物品()
	RunInfo("接收保管箱物品")
	--SendGet_BagBox();
	GPlayerMe():UnpackAllBags();
end

--开卡补足一些球员
function 开卡补足一些球员( cnt )
	RunInfo("开卡补足一些球员")
	GPlayerMe():OpenSomePlayersAs(cnt);
end

--开卡补足到
function 开卡补足到( cnt )
	RunInfo("开卡补足到")
	local gp = GPlayerMe();
	local now_cnt = gp:GetAllPlayersCnt();
	if cnt <= now_cnt then
		return;
	end
	local need_cnt = cnt - now_cnt;
	gp:OpenSomePlayersAs(need_cnt);
end

--自动刷新我的出售列表
function 自动刷新我的出售列表()
	RunInfo("自动刷新我的出售列表")
	local gp = GPlayerMe();
	if gp:NeedRefreshSelfOffserList() then
		gp:RefreshSelfOfferList();
	end
end

--保持拍卖一些球员
function 保持拍卖一些球员( price, cnt )
	RunInfo("保持拍卖一些球员")
	GPlayerMe():KeepSellingPlayerAs(price, cnt);
end

--保持拍卖一个垃圾球员
function 保持拍卖一个垃圾球员( price_min, price_max )
	RunInfo("保持拍卖一个垃圾球员")
	GPlayerMe():KeepSellingRubbishPlayer(price_min, price_max);
end

--仓库号保持拍卖一些球员
function 仓库号保持拍卖一些球员( cnt, price_limit )
	RunInfo("仓库号保持拍卖一些球员")
	GPlayerMe():KeepSellingGoodPlayers( cnt, price_limit );
end

--保持一定的球员数量
function 保持一定的球员数量( cnt, price_limit )
	RunInfo("保持一定的球员数量")
	GPlayerMe():KeepPlayersCntMoreThan(cnt, price_limit);
end

--购买仓库号球员
function 购买仓库号球员( ep )
	RunInfo("购买仓库号球员")
	local gp = GPlayerMe();
	if gp.ep >= ep then
		gp:BuyOtherPlayers();
	end
end

--可以执行普通号的交易流程
function 可以执行普通号的交易流程()
	RunInfo("可以执行普通号的交易流程")
	local normal = FuncObj_NormalC_Instance();
	if ( normal == nil ) then
		return false;
	end
	return normal:CanNormalTradeProcedure();
end

--可以执行普通号的交易流程
function 可以执行仓库号的交易流程()
	RunInfo("可以执行仓库号的交易流程")
	local normal = FuncObj_NormalC_Instance();
	if ( normal == nil ) then
		return false;
	end
	return normal:CanStoredTradeProcedure();
end

--按照默认的价格拍卖一些球员
function 按照默认的价格拍卖一些球员( price_lower, price_upper )
	RunInfo("按照默认的价格拍卖一些球员")
	GPlayerMe():SalePlayersAsDefault(price_lower, price_upper);
end

--确认创建俱乐部
function 确认创建俱乐部()
	RunInfo("确认创建俱乐部")
	LeftBtClick(522, 619);
end

--当前EP
function 当前EP()
	RunInfo("当前EP")
	return GPlayerMe().ep;
end

--执行换号
function 执行换号()
	RunInfo("执行换号")
	ExchangeGameAc();
end

--无职位球员中交换一些球员
function 无职位球员中交换一些球员( price, cnt )
	RunInfo("无职位球员中交换一些球员")
	GPlayerMe():ExchPlayersInPosNo(price, cnt);
end

--二线球员人数
function 二线球员人数()
	RunInfo("二线球员人数")
	return GPlayerMe():GetSecondTeamCnt();
end

--领取交换后的球员
function 领取交换后的球员()
	RunInfo("领取交换后的球员")
	GPlayerMe():ChooseExchPlayer();
end

--设置对方阵营不会接收到拍卖信息的价格
function 设置对方阵营不会接收到拍卖信息的价格( price )
	RunInfo("设置对方阵营不会接收到拍卖信息的价格")
	GPlayerMe().against_offer_price = price;
end

--自动刷新教练信息
function 自动刷新教练信息()
	RunInfo("自动刷新教练信息")
	GPlayerMe():RefreshCoachMe();
end

--自动处理下架
function 自动处理下架()
	RunInfo("自动处理下架")
	GPlayerMe():HandleTimeCancelSale();
end

--设置拍卖超时下架时间
function 设置拍卖超时下架时间( time_hour )
	RunInfo("设置拍卖超时下架时间")
	GPlayerMe():SetTimeWaitSalePlayer(time_hour);
end

--调整发包计数
function 解决有时不能交易问题()
	RunInfo("解决有时不能交易问题")
	HttpPackageSend_Me():SendPackageHit();
end

--解决创建教练时的弹框问题
function 解决创建教练时的弹框问题()
	RunInfo("解决创建教练时的弹框问题")
	LeftBtClick(522, 619);
end

function 解决卡续约问题()
	RunInfo("解决卡续约问题")
	--1个球员时Y=120
	--[20,150]
	for i = 150, 20, -10 do
		LeftBtClick(602, i)
	end
end

function 解决新教练练习赛问题()
	RunInfo("解决新教练练习赛问题")
	local cnt = 0;
	while true do
		LeftBtClick(695, 424);
		Delay(1000);
		if cnt > 10 then
			break;
		end
		cnt = cnt + 1;
	end
	local normal = FuncObj_NormalC_Instance();
	if normal == nil then
		return;
	end
	normal:Disconnect();
end

function 设置比赛次数上限( cnt )
	RunInfo("设置比赛次数上限")
	GPlayerMe().match_limit_cnt = cnt;
end

function 以最高价拍卖一些球员( player_cnt )
	RunInfo("以最高价拍卖一些球员")
	return GPlayerMe():SaleSomePlayersAsMaxPrice(player_cnt);
end

function 以最高价拍卖指定的球员( player_id, player_cnt )
	RunInfo("以最高价拍卖指定的球员")
	local the_cnt = 1;
	if type(player_cnt) == "number" then
		the_cnt = player_cnt;
	end
	return GPlayerMe():SaleThePlayersAsMaxPrice(the_cnt, player_id);
end

function 以最低价拍卖一些球员( player_cnt )
	RunInfo("以最低价拍卖一些球员")
	return GPlayerMe():SaleSomePlayersAsMinPrice(player_cnt);
end

function 以最低价拍卖指定的球员( player_id, player_cnt )
	RunInfo("以最低价拍卖指定的球员")
	local the_cnt = 1;
	if type(player_cnt) == "number" then
		the_cnt = player_cnt;
	end
	return GPlayerMe():SaleThePlayersAsMinPrice(the_cnt, player_id);
end

function 是第一次开始计时双点()
	RunInfo("是第一次开始计时双点")
	return GPlayerMe():IsFirstCalcDoubleTime();
end

function 到了双点()
	RunInfo("到了双点")
	return GPlayerMe():IsAtDoubleTime();
end

function 更新双点计时()
	RunInfo("更新双点计时")
	GPlayerMe():UpdateDoubleTime();
end

function 到了指定的时间间隔( time_minustes )
	RunInfo("到了指定的时间间隔")
	return GPlayerMe():IsAtTheTimeSpan(time_minustes);
end

function 以指定的价格拍卖一些指定的球员( player_id, player_cnt, price )
	RunInfo("以指定的价格拍卖一些指定的球员")
	return GPlayerMe():SaleThePlayersAsPrice_Cnt(player_id, player_cnt, price);
end

--[[
第一个参数是球员ID
第二个参数是强化等级下限
第三个参数是强化等级上限
第四个参数是要拍卖的数量，如果这样的球员已经拍卖了这样的个数了，则不会再拍卖了
第五个参数是拍卖价格
]]
function 以指定的价格拍卖指定的球员( player_id, grade_min, grade_max, player_cnt, price )
	RunInfo("以指定的价格拍卖指定的球员")
	return GPlayerMe():SaleThePlayersSpecific(player_id, grade_min, grade_max, player_cnt, price);
end

function 到了截卡指定的时间间隔( time_minustes )
	RunInfo("到了截卡指定的时间间隔")
	return GPlayerMe():IsJieCardTheTimeSpan(time_minustes);
end

function 到了截卡指定的时间间隔_秒( time_minustes )
	RunInfo("到了截卡指定的时间间隔_秒")
	return GPlayerMe():IsJieCardTheTime_Sec(time_minustes);
end

function 对指定的球员进行截卡( player_id, grade, price )
	RunInfo("对指定的球员进行截卡")
	return GPlayerMe():JieCardThePlayer(player_id, grade, price);
end

function 以指定的价格买一个指定的球员( player_id, grade, price_lower, price_upper )
	RunInfo("以指定的价格买一个指定的球员")
	return GPlayerMe():BuyThePlayerAsPrice(player_id, grade, price_lower, price_upper);
end

function 自动使用一些物品( item_cnt )
	RunInfo("自动使用一些物品")
	if type(item_cnt) ~= "number" then
		LuaLogE("自动使用一些物品之物品数量参数必须是数字类型")
		return false
	end
	return UseSomeItem(item_cnt)
end

--[[
第一个参数是球员ID，球员ID的获取方式请参考 以指定的价格拍卖一些指定的球员 命令
第二个参数是被强化球员的强化等级下限
第三个参数是被强化球员的强化等级上限
第四个参数是强化球员的强化等级上限
强化球员功能需要两个相同球员才可以，被强化球员指的是较高强化等级的那个球员，强化球员是用来提高被强化球员等级的那个球员。
对于被强化球员，其强化等级在 强化等级下限 和 强化等级上限 之间的才会被强化。
对于强化球员，其强化等级小于等级 强化球员的强化等级上限 才会强化。
]]
function 强化指定的球员(player_id, dst_grade_min, dst_grade_max, src_grade_max)
	if type(player_id) ~= "number" then
		LuaLogE("强化指定的球员之球员ID参数必须是数字类型")
		return
	end
	if type(dst_grade_min) ~= "number" then
		LuaLogE("强化指定的球员之被强化球员的强化等级下限参数必须是数字类型")
		return
	end
	if type(dst_grade_max) ~= "number" then
		LuaLogE("强化指定的球员之被强化球员的强化等级上限参数必须是数字类型")
		return
	end
	if type(src_grade_max) ~= "number" then
		LuaLogE("强化指定的球员之强化球员的强化等级上限参数必须是数字类型")
		return
	end
	RunInfo("强化指定的球员")
	return L_EnhanceThePlayer(player_id, dst_grade_min, dst_grade_max, 0, src_grade_max)
end

--[[
第一个参数是球员ID，球员ID的获取方式请参考 以指定的价格拍卖一些指定的球员 命令
第二个参数是被强化球员的强化等级下限
第三个参数是被强化球员的强化等级上限
第四个参数是强化球员的强化等级下限
第五个参数是强化球员的强化等级上限
强化球员功能需要两个相同球员才可以，被强化球员指的是较高强化等级的那个球员，强化球员是用来提高被强化球员等级的那个球员。
对于被强化球员，其强化等级在 强化等级下限 和 强化等级上限 之间的才会被强化。
对于强化球员，其强化等级小于等级 强化球员的强化等级上限 才会强化。
]]
function 指定强化指定的球员(player_id, dst_grade_min, dst_grade_max, src_grade_min, src_grade_max)
	if type(player_id) ~= "number" then
		LuaLogE("指定强化指定的球员之球员ID参数必须是数字类型")
		return
	end
	if type(dst_grade_min) ~= "number" then
		LuaLogE("指定强化指定的球员之被强化球员的强化等级下限参数必须是数字类型")
		return
	end
	if type(dst_grade_max) ~= "number" then
		LuaLogE("指定强化指定的球员之被强化球员的强化等级上限参数必须是数字类型")
		return
	end
	if type(src_grade_min) ~= "number" then
		LuaLogE("指定强化指定的球员之强化球员的强化等级下限参数必须是数字类型")
		return
	end
    if type(src_grade_max) ~= "number" then
		LuaLogE("指定强化指定的球员之强化球员的强化等级上限参数必须是数字类型")
		return
	end
	RunInfo("指定强化指定的球员")
	return L_EnhanceThePlayer(player_id, dst_grade_min, dst_grade_max, src_grade_min, src_grade_max)
end

--[[
这个命令根据 会扫描 购买最低价格 到 购买最高价格 之间球员，每次购买的价格依次是上一次购买价格+递增价格
比如 扫隐藏卡(1, 1, 10, 30, 10)，表示扫 球员ID为1，强化等级为1的球员，购买价格依次是10、20、30。若购买价格为20能够购买成功，则命令返回20，并输出价格信息到控制台的 输出结果 栏目下。
第一个参数是球员ID，球员ID的获取方式请参考 以指定的价格拍卖一些指定的球员 命令
第二个参数是球员强化等级
第三个参数是购买最低价格
第四个参数是购买最高价格
第五个参数是递增价格
]]
function 扫隐藏卡(player_id, grade, price_lower, price_high, price_addition)
	if type(player_id) ~= "number" then
		LuaLogE("扫隐藏卡之球员ID参数必须是数字类型")
		return -1
	end
	if type(grade) ~= "number" then
		LuaLogE("扫隐藏卡之球员强化等级参数必须是数字类型")
		return -1
	end
	if type(price_lower) ~= "number" then
		LuaLogE("扫隐藏卡之购买最低价格参数必须是数字类型")
		return -1
	end
	if type(price_high) ~= "number" then
		LuaLogE("扫隐藏卡之购买最高价格参数必须是数字类型")
		return -1
	end
	if type(price_addition) ~= "number" then
		LuaLogE("扫隐藏卡之递增价格参数必须是数字类型")
		return -1
	end
	RunInfo("扫隐藏卡")
	return L_ScanHideCard(player_id, grade, price_lower, price_high, price_addition)
end

--[[
按指定的价格扫隐藏卡(1, 2, 30, 10) 表示 球员ID为1，球员强化等级为2的球员，以30的价格购买10个。
第一个参数是球员ID，球员ID的获取方式请参考 以指定的价格拍卖一些指定的球员 命令
第二个参数是球员强化等级
第三个参数是购买价格
第四个参数是购买个数
]]
function 按指定的价格扫隐藏卡(player_id, grade, price, cnt)
	if type(player_id) ~= "number" then
		LuaLogE("按指定的价格扫隐藏卡之球员ID参数必须是数字类型")
		return -1
	end
	if type(grade) ~= "number" then
		LuaLogE("按指定的价格扫隐藏卡之球员强化等级参数必须是数字类型")
		return -1
	end
	if type(price) ~= "number" then
		LuaLogE("按指定的价格扫隐藏卡之价格参数必须是数字类型")
		return -1
	end
	if type(cnt) ~= "number" then
		LuaLogE("按指定的价格扫隐藏卡之购买个数参数必须是数字类型")
		return -1
	end
	RunInfo("按指定的价格扫隐藏卡")
	return L_ScanHideCardAsPrice(player_id, grade, price, cnt)
end

--[[
第一个参数是球员ID，球员ID的获取方式请参考 以指定的价格拍卖一些指定的球员 命令
第二个参数是球员最低强化等级
第三个参数是球员最高强化等级
指定球员的数量(1, 1, 1)，表示球员id为1，强化等级为1的球员的数量
]]
function 指定球员的数量(player_id, grade_min, grade_max)
	if type(player_id) ~= "number" then
		LuaLogE("指定球员的数量之球员ID参数必须是数字类型")
		return -1
	end
	if type(grade_min) ~= "number" then
		LuaLogE("指定球员的数量之最低强化等级参数必须是数字类型")
		return -1
	end
	if type(grade_max) ~= "number" then
		LuaLogE("指定球员的数量之最高强化等级必须是数字类型")
		return -1
	end
	RunInfo("指定球员的数量")
	return L_GetThePlayerCnt(player_id, grade_min, grade_max)
end
