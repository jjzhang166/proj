--[[
游戏状态设置器
]]
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--[[
过滤收到的消息，判断环境
第一个参数是json的路径(path)
第二个参数是处理函数的名字
函数是所属的recv msg handle元素(recv_handler_obj, last_json_value)。
第三个参数是可选的，是个表，分别对应第一个参数里的值，如果没有，表示只是效验path是否存在
]]
g_tblRecvMsgHandle = {
	----[[
	--以后做优化的时候，可以考虑先判断游戏状态，再去判断以下寻路径、判断值
	--{ RetTrue, HandleFirstTrainerSelected, { "channelid" } },
	{ RetTrue, HandleSelectManagers_Computer, { ".resources.[0].body" }, { "^pve_sim$" } },
	{ RetTrue, HandleSelectManagers_Friend, { ".resources.[0].body" }, { "^custom_sim$" } },
	{ RetTrue, HandleSelectManagers_Random, { ".resources.[0].body" }, { "^pvp_sim$" } },
	{ RetTrue, HandleTrainerSelect_, { ".coachlist" } },
	--设置对手信息
	{ RetTrue, HandleSetEnemyInfo, { ".resources.[3].body.index.team", ".resources.[3].body.index" } },
	--返回房间后".resources.[0].body.stage"=="room"
	--{ RetTrue, HandleEnterMatchWaiting, { ".resources.[0].body.type", ".resources.[0].body.method", ".resources.[0].body.stage" }, { "^pve_sim$", "^createRoom$", "^matched$" } },
	{ RetTrue, HandleEnterMatchChoosePlayer, { ".resources.[0].path", ".resources.[0].body" }, { "^stage$", "^squad$" } },
	--{ RetTrue, HandleEnterMatchReady, { ".resources.[0].path", "result" }, { "^side.left.squad$", true } },
	--{
		--RetTrue, HandleMatch_AllReady,
		--{ "resources.[0].path", "resources.[1].path", "resources.[2].path", "resources.[3].path" },
		--{ "^side.left.ready$", "^side.right.ready$", "^side.observer.ready$", "^side.referee.ready$" }
	--},
	{ RetTrue, HandleGainRoomId, { ".resources.[0].body.roomid" } },
	{ RetTrue, HandleGainMatchId, { ".resources.[0].body.matchmaking.matchid" } },
	----------------
	--游戏初始化接收到的数据
	{ RetTrue, Handle_GainEpFromServer, { ".resources.[0].uri", ".resources.[0].body.money.ep" }, { "^/coach/me$" } },
	{ RetTrue, Handle_GainBagItems, { ".resources.[0].uri", ".resources.[0].body.items" }, { "^/coach/me$" } },
	{ RetTrue, HandleInit_GainName, { ".resources.[0].uri", ".resources.[0].body.name" }, { "^/coach/me$" } },
	{ RetTrue, Handle_GainLevelInfo, { ".resources.[0].uri", ".resources.[0].body.levelinfo" }, { "^/coach/me$" } },
	{ RetTrue, HandleInit_GainPlayers, { ".resources.[0].uri", ".resources.[0].body.team.players" }, { "^/coach/me$" } },
	{ RetTrue, HandleUpdatePlayersPositions, { ".resources.[0].uri", ".resources.[0].body.team.squad.positions" }, { "^/coach/me$" } },	--初始化职位信息
	{ RetTrue, HandleUpdateTotalPlayersCnt, { ".resources.[0].uri", ".resources.[0].body.limits.players" }, { "^/coach/me$" } },	--俱乐部球员管理数量
	--比赛房间中的续约相关
	{ RetTrue, HandleMatchRoom_ContractDlg,
		{ ".boost.event", ".boost.igr", ".boost.item", "detail.items", "discount", "players.[0].availablematchcount", "total" }
	},
	--比赛结束后接收到的数据
	{ RetTrue, Handle_GainEpFromServer, { ".resources.[2].path", ".resources.[2].body.ep" }, { "^money$" } },
	{ RetTrue, Handle_GainBagItems, { ".resources.[0].path", ".resources.[0].body" }, { "^items$" } },
	{ RetTrue, Handle_GainLevelInfo, { ".resources.[1].path", ".resources.[1].body" }, { "^levelinfo$" } },
	{ RetTrue, HandleMatchEnd_GainPlayers, { ".resources.[5].path", ".resources.[5].body" }, { "^team%.players%.%$$" } },
	{ RetTrue, HandleSetFirstPlayers, { ".resources.[3].path", ".resources.[3].body.home.players" }, { "^result$" } },
	{ RetTrue, HandleSetSecondPlayers, { ".resources.[3].path", ".resources.[3].body.home.reserved" }, { "^result$" } },
	{ CanHandleInputCoachName, HandleInputCoachNameSucceed, { ".result" }, { true } },
	{ CanHandleInputCoachName, HandleInputCoachNameFail, { ".result" }, { false } },
	--成就
	{ RetTrue, HandleAchievementInfo, { ".summary", ".achievement" } },
	--gp
	{ RetTrue, Handle_GainGpFromServer, { ".resources.[0].uri", ".resources.[0].body.gp" }, { "^/coach/me$" } },
	--gp礼包
	{ IsGettingGpData, HandleRecvGpData, { ".[0].shopid", "." } },
	--疲劳
	{ RetTrue, HandleMatchAddict, { ".resources.[0].path", ".matchaddicted" }, { "^items$" } },
	--封号
	{ RetTrue, HandleBanAccount, { ".errorCode", ".result" }, { 5, false } },
	--{ RetTrue, HandleBanAccount, { ".errorCode", ".result", ".reason" }, { 5, false, "^1$" } },
	--]]
	-----------------------------------------
	--[[
	--get ep
	{ "HandleGainEpFromServer", { ".resources.[0].path", ".resources.[0].body.ep" }, { "^money$" } },
	--get 背包物品数据
	{ "HandleGainBagItems", { ".resources.[0].path", ".resources.[0].body" }, { "^items$" } },
	--get 人物等级数据
	{ "HandleGainLevelInfo", { ".resources.[0].path", ".resources.[0].body" }, { "^levelinfo$" } },
	--get 比赛完球员变化的数据(这个先不搞)
	--]]
};
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--by send msg
g_tblMsgCataloghandler = {
	--格式为{ http URI, handler_obj }
	{ RetTrue, HandlePressOnClick_, "^/game/ads$" },
	--{ RetTrue, HandleNoticeDlgScene_, "^/testcbt/externalAssets/notice/notice.png$" },
	{ RetTrue, HandleNoticeDlgScene_, "^/game/inweburl/notice$" },
	{ RetTrue, HandleTrainerSelected_, "^/chat/blacklist$" },
	{ RetTrue, HandleMatchEnd, "^/match/%d+/end$" },
	--{ RetTrue, HandleMatchEnd_Prize, "^/shop/list$" },
	{ RetTrue, HandleMatchEnd_Back, "^/room/%d+/back$" },
	{ RetTrue, HandleLevelUp, "^/coach/me/level/reward$" }, --升级处理
	{ RetTrue, HandleEnterClubManage, "^/stadium/%d+$" }, --俱乐部管理状态
	{ RetTrue, HandleEnterAutoSelectPlayer, "^/coach/me/squad$" }, --自动选拔球员成功
	{ RetTrue, HandleGetGiftPackageData, "^/shop/list/gp$" }, --GP礼包
	{ RetTrue, HandleEnterMatchReadyOrAnimate, "^/match/%d+/side/left/ready$" }, --准备比赛场景或比赛动画状态
	{ RetTrue, HandleEnterMatchWaiting, "^/match/%d+/player/ready$" },
	{ RetTrue, HandleControlSetting, "^/ob/externalAssets/beginnersguide/mascot4.png$" }, --设置 控制设定
	{ RetTrue, HandleScanSoldPlayer, "^/transfer/search/%d+$" }, --查找拍卖球员卡
};

------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--发包与收包的映射,post
g_tblMapPost = {
	--handle,uri
	--handle ->参数是json msg
};
g_tblRequestMapPost = {
	--handle, uri
	--handle ->@p1=quest,@p2=json msg
	--拍卖球员
	{ Post_SellPlayer ,"/transfer/offer" },
};
--发包与收包的映射,get
g_tblMapGet = {
	--handle,uri
	{ Get_BagBox, "/bag" },
};
g_tblRequestMapGet = {

};
-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
--通过hook发包call，来进行字符串匹配的表
--格式：第一个元素是数值，表示template_id,int
--第二个元素是：是否按照初始化的顺序进行匹配，bool
--第四个元素是个表，表示要匹配的字符串,支持lua string里的模式匹配
--第三个元素是个全都匹配后的处理函实数的字符串, void( int template_id )
--{ template_id, is_order, { matched_strings }, handle_function_name
g_tblMultiMatch_ByHookSendPackage = {
	---[[
	--经理人模式
	{ 1, true, "HandleEnterMatchRoom", { "^/room$", "^/contact/list$" } },
	--]]
};

--通过一个给定的表，来进行字符串匹配的初始化工作
function InitMultiStrMatchByTbl( tbl )
	local multi_match = MultiStrMatchOnTime_Me();
	if multi_match ~= nil then
		for _, v in ipairs(tbl) do
			multi_match:InitTemplateIdObj(v[1], multi_match:GetLifecycleForever(), v[2]);
			if v[1] ~= nil and v[2] ~= nil and v[3] ~= nil and v[4] ~= nil then
				for _, matched_str in ipairs(v[4]) do
					multi_match:AddMatchedStr(v[1], matched_str);
				end
			end
		end
	end
end

--通过ID匹配指定字符串
function MatchSpecifyStrById( template_id, specify_str )
	if specify_str == nil then
		return false;
	end
	local multi_match = MultiStrMatchOnTime_Me();
	if multi_match ~= nil then
		return multi_match:AllMatchSpecifyStr(template_id, specify_str);
	end
	return false;
end

--通过一个表匹配指定字符串
function MatchSpecifyStrByTbl( tbl, specify_str )
	for _, v in ipairs(tbl) do
		if MatchSpecifyStrById(v[1], specify_str) == true then
			--全部匹配成功，调用处理函数
			_G[v[3]](v[1]);
		end
	end
end

--初始化hook send packat multi match table
InitMultiStrMatchByTbl(g_tblMultiMatch_ByHookSendPackage);
-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
