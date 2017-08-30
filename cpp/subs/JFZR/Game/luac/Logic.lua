--这个文件里的函数必须要在注入线程里执行

--更新所有游戏对象
function UpdateAllGameObjs()
	local obj_mgr = GameObjMgr_Me();
	if not obj_mgr:RebuildUpdateAll() then
		LuaLog("所有游戏对象更新失败");
	end
	return obj_mgr;
end

--更新角色
function UpdateRole()
	local gp = GPlayer_Me();
	if not gp:Update() then
		LuaLog("角色信息更新失败");
	end
	return gp;
end

--更新地图或房间
function UpdateMapRoom()
	local gm = GMap_Me();
	if not gm:Update() then
		LuaLog("地图更新失败");
		return;
	end
	if not gm:IsTown() then
		local gr = GCloneMap_Me();
		if not gr:Update() then
			LuaLog("房间更新失败");
			return;
		end
	end
	return gm;
end

--更新房间信息
function UpdateRoom()
	local gr = GCloneMap_Me();
	if not gr:Update() then
		LuaLog("房间信息更新失败");
		return;
	end
	return gr;
end

------------------------------------------------
--杀死所有的怪物
function KillAllMonster()
	UpdateMapRoom();
	UpdateAllGameObjs();
	UpdateRole();
	local gp = GPlayer_Me();
	if not gp:KillAllMonster() then
		LuaLogE("杀死所有的怪物失败");
	end
	return true;
end

--过房间门
function StepRoomDoor( door_idx )
	local gr = UpdateRoom();
	if gr == nil then
		return;
	end
	local room_door = gr:GetTheDoor(door_idx);
	local gp = GPlayer_Me();
	gp:EnterAnotherRoom(room_door);
end

--拾取所有物品
function PickUpAllItems()
	UpdateAllGameObjs();
	local gp = GPlayer_Me();
	if not gp:PickUpAllItems() then
		LuaLog("拾取所有物品失败");
	end
end

--拾取所有金币
function PickUpAllGold()
	UpdateAllGameObjs();
	local gp = GPlayer_Me();
	if not gp:PickUpAllGold() then
		LuaLog("拾取所有物品失败");
	end
	return true;
end

--自动升级所有技能
function AutoLvlUpSkills()
	local gps = GPlayer_Me();
	if not gps:Update() then
		LuaLog("gps update failed");
		return false;
	end
	if not gps:AutoLvlUpSkills() then
		LuaLogE("learn skill error");
		return false;
	end
	return true;
end

function AutoLearnLvlUpSkills()
	local gps = GPlayer_Me();
	if not gps:Update() then
		LuaLog("gps update failed");
		return false;
	end
	if not gps:AutoLearnLvlUpSkills() then
		LuaLogE("learn skill error");
		return false;
	end
	return true;
end

--直到进入城镇
function UntileEnteredCity( delay_time )
	if type(delay_time) ~= "number" then
		delay_time = 0;
	end
	local ambient = GAmbient_Me();
	if not ambient:UntilCityUiNow(delay_time) then
		LuaLogE("直到进入城镇返回false");
	end
end

--直到进入副本
function UntileEnteredFb( delay_time )
	if type(delay_time) ~= "number" then
		delay_time = 0;
	end
	local ambient = GAmbient_Me();
	if not ambient:UntilEnteredFb(delay_time) then
		LuaLogE("直到进入副本返回false");
	end
end

--直到可以返回城镇
function UntileCanRetCity( delay_time )
	if type(delay_time) ~= "number" then
		delay_time = 0;
	end
	local ambient = GAmbient_Me();
	if not ambient:UntilCanRetCity(delay_time) then
		LuaLogE("直到可以返回城镇返回false");
		return false;
	end
	return true;
end

--打完所有房间
function FightThroughAllRooms( func )
	if func == nil then
		LuaLogE("打完所有房间之参数不能为空");
		return;
	end
	if type(func) ~= "function" then
		LuaLogE("打完所有房间之参数必须是函数类型");
	end
	local gps = UpdateRole();
	if gps == nil then
		LuaLog("角色更新失败");
		return;
	end
	if not gps:FightThroughAllRooms(func) then
		LuaLog("打完所有房间出错");
		return;
	end
	--AutoLvlUpSkills();
	--自动更换装备();
	--修理已穿装备();
end

function FightAllGraph( func )
	if func == nil then
		LuaLogE("全图打副本之参数不能为空");
		return false;
	end
	if type(func) ~= "function" then
		LuaLogE("全图打副本之参数必须是函数类型");
	end
	local gps = UpdateRole();
	if gps == nil then
		LuaLog("角色更新失败");
		return false;
	end
	if not gps:FightAllGraph(func) then
		LuaLog("全图打副本出错");
		return false;
	end
	return true;
end

--角色在副本中
function RoleInCloneMap()
	local gm = GMap_Me();
	if not gm:Update() then
		LuaLog("地图更新失败");
		return false;
	end
	return not gm:IsTown();
end

function RoleInTown()
	local gm = GMap_Me();
	if not gm:Update() then
		LuaLog("地图更新失败");
		return false;
	end
	return gm:IsTown();
end

--破坏所有固态物品
function DestroyAllMisc()
	UpdateMapRoom();
	UpdateAllGameObjs();
	UpdateRole();
	local gp = GPlayer_Me();
	if not gp:DestroyAllMisc() then
		LuaLog("破坏所有固态物品失败");
	end
end

function GetGameState()
	local nor = NormalC_Inst();
	if nor == nil then
		LuaLogE("func obj nor is nil");
		return enGameState.kGS_Invalid;
	end
	return nor.game_state;
end

function IsSelectChannelScene()
	return enGameState.kGS_SelectChannel == GetGameState();
end

function IsSelectRoleScene()
	return enGameState.kGS_SelectRole == GetGameState();
end

function IsRoleInGame()
	return enGameState.kGS_InGame == GetGameState();
end

--随机选择频道
function RandomChooseChannel()
	local nor = NormalC_Inst();
	if nor == nil then
		LuaLogE("func obj nor is nil");
		return enGameState.kGS_Invalid;
	end
	if not nor:ChooseChannel() then
		LuaLogE("选择频道返回失败");
	end
end

function AutoCreateOrSelectRole()
	local nor = NormalC_Inst();
	if nor == nil then
		LuaLogE("func obj nor is nil");
		return false;
	end
	if not nor:AutoCreateOrSelectRole() then
		LuaLogE("自动创建或选择角色返回失败");
		return false;
	end
	return true;
end

--移动到指定的副本
function MoveToTheCloneMap( fb_ch_name )
	local gps = GPlayer_Me();
	if not gps:MoveToTheCloneMap(fb_ch_name) then
		LuaLogE("移动到指定的副本失败");
		return false;
	end
	return true;
end

function MoveToTheVillage( village_ch_name )
	local gps = GPlayer_Me();
	if not gps:MoveToTheVillage(village_ch_name) then
		LuaLogE("移动到指定的城镇失败");
		return false;
	end
	return true;
end

function GetRoleFatigueData()
	local gps = GPlayer_Me();
	return gps:GetFatigueData();
end

function RetChooseRoleScene()
	local nor = NormalC_Inst();
	if nor == nil then
		LuaLogE("func obj nor is nil");
		return false;
	end
	return nor:RetChooseRoleScene();
end

function AddPerItemFilter( tbl, i, v )
	if type(v) ~= "table" then
		LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项，不是表类型");
		return;
	end
	local tbl_tmp = {};
	local item_name = v[1];
	if item_name ~= nil then
		if type(item_name) ~= "string" then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品名称必须是字符串类型");
			return;
		end
		--[[
		local item_id = GameEn2ChNodeMgr_Me():GetItemIndex():Ch2EnA(item_name);
		if item_id:empty() then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品名称不存在->" .. item_name);
			return;
		end
		tbl_tmp[1] = item_id;
		--]]
		tbl_tmp[1] = GbkToWideChar(item_name);
		if tbl_tmp[1]:empty() then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品名称不能为空");
			return;
		end
	end
	local item_quality = v[2];
	if item_quality ~= nil then
		if type(item_quality) ~= "number" then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品品质必须是数字类型");
			return;
		end
		if item_quality < enItemQuality.kIQ_Low or item_quality > enItemQuality.kIQ_Legendary then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品品质数值无效");
			return;
		end
		tbl_tmp[2] = item_quality;
	end
	local item_lvl = v[3];
	if item_lvl ~= nil then
		if type(item_lvl) ~= "number" then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品等级必须是数字类型");
			return;
		end
		if item_lvl <= 0 or item_lvl > 200 then
			LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品等级数值无效");
			return;
		end
		tbl_tmp[3] = item_lvl;
	end
	local handler = v[4];
	if type(handler) ~= "function" then
		LuaLogE("添加物品过滤之第[" .. tostring(i) .. "]项的物品过滤处理必须是函数类型");
		return;
	end
	if table.maxn(tbl_tmp) > 0 then
		tbl_tmp[4] = handler;
		table.insert(tbl, tbl_tmp);
	end
end

function CmnCmpFunc( lhs, rhs )
	if rhs == nil then
		return true;
	end
	return lhs >= rhs;
end
function WStrCmpFunc( lhs, rhs )
	if rhs == nil then
		return true;
	end
	return 0 == lhs:compare(rhs);
end

function WStrBlurCmpFunc( lhs, rhs )
	if rhs == nil then
		return true;
	end
	if lhs:empty() then
		return true;
	end
	return FindSubWStr(lhs, rhs);
end

function QualityCmpFunc( lhs, rhs )
	if rhs == nil then
		return true;
	end
	return lhs == rhs;
end

function AddTradeItemFilter( filter_tbl, tab_cate )
	if type(filter_tbl) ~= "table" then
		LuaLogE("添加交易物品过滤器之过滤表的参数类型必须是表类型");
		return false;
	end
	if type(tab_cate) ~= "number" then
		LuaLogE("添加交易物品过滤器之物品种类的参数类型不对，必须是数字类型");
		return false;
	end
	if tab_cate < 1 or tab_cate > 3 then
		LuaLogE("添加交易物品过滤器之物品种类的参数数值不对，请详看函数说明");
		return false;
	end
	local tbl_tmp = {}
	for i, v in ipairs(filter_tbl) do
		AddPerItemFilter(tbl_tmp, i, v);
	end
	if table.maxn(tbl_tmp) <= 0 then
		return;
	end
	local tmp = GPlayer_Me():GetPacket();
	if tab_cate == 1 then
		tmp = tmp:GetTabNormal();
	elseif tab_cate == 2 then
		tmp = tmp:GetTabMaterial();
	else
		tmp = tmp:GetTabConsume();
	end
	local tbl_item;
	return tmp:AddTradeFilterHandler(function( item )
		local lvl = item:GetItemLevel();
		local item_qua = item:GetItemQualityValue();
		--[[
		local item_id = item:GetItemId();
		for _, v in ipairs(tbl_tmp) do
			if WStrCmpFunc(item_id, v[1]) and CmnCmpFunc(item_qua, v[2]) and CmnCmpFunc(lvl, v[3]) then
				tbl_item = v;
				return true;
			end
		end
		--]]
		local item_cn_name = item:GetItemCnName();
		for _, v in ipairs(tbl_tmp) do
			if WStrBlurCmpFunc(item_cn_name, v[1]) and QualityCmpFunc(item_qua, v[2]) and CmnCmpFunc(lvl, v[3]) then
				tbl_item = v;
				return true;
			end
		end
		return false;
	end, function( item )
		return tbl_item[4](item);
	end);
end

local g_ac_type_normal_ = 1;
local g_ac_type_storehouse_ = 2;
local g_ac_type_trans_station = 3;
local g_ac_type_invalid = 4;

function SaleProductAsPriceImpl( item, price, cnt, product_cnt, ac_type )
	local item_cnt = item:GetItemCnt();
	if item_cnt < cnt then
		cnt = item_cnt;
	end
	GPlayer_Me():SaleProductAsPrice(item, price, cnt, product_cnt, ac_type);
end

function SaleProductAsPrice( price, cnt, product_cnt, ac_type )
	if type(price) ~= "number" then
		LuaLogE("以指定的价格拍卖之价格参数必须是数字类型");
		return;
	end
	if ac_type ~= nil then
		if type(ac_type) ~= "number" then
			LuaLogE("以指定的价格拍卖之帐号种类的参数必须是数字类型");
			return;
		end
		if ac_type < g_ac_type_normal_ or ac_type > g_ac_type_trans_station then
			LuaLogE("以指定的价格拍卖之帐号种类数值不正确");
			return;
		end
	else
		ac_type = g_ac_type_invalid;
	end
	ac_type = ac_type - 1;
	if product_cnt ~= nil then
		if type(product_cnt) ~= "number" then
			LuaLogE("以指定的价格拍卖之拍卖条数限制的参数必须是数字类型");
			return;
		end
		if product_cnt <= 0 then
			product_cnt = -1;
		end
	else
		product_cnt = 1;
	end
	if cnt == nil then
		cnt = 1;
	elseif type(cnt) ~= "number" then
		LuaLogE("以指定的价格拍卖之数量参数必须是数字类型");
		return;
	elseif cnt <= 0 then
		cnt = 1;
	end
	if price <= 0 then
		price = 10;
	end
	return function( item )
		return SaleProductAsPriceImpl(item, price, cnt, product_cnt, ac_type);
	end
end

function SaleProductAsPriceBaseImpl( item, price_delta, dlt_price, cnt, product_cnt, ac_type )
	local item_cnt = item:GetItemCnt();
	if item_cnt < cnt then
		cnt = item_cnt;
	end
	GPlayer_Me():SaleProductAsPriceBase(item, price_delta, dlt_price, cnt, product_cnt, ac_type);
end

function SaleProductAsPriceBase( price_delta, dlt_price, cnt, product_cnt, ac_type )
	if type(price_delta) ~= "number" then
		LuaLogE("按市场价的某个范围拍卖之价格范围参数必须是数字类型");
		return;
	end
	if ac_type ~= nil then
		if type(ac_type) ~= "number" then
			LuaLogE("按市场价的某个范围拍卖之帐号种类的参数必须是数字类型");
			return;
		end
		if ac_type < g_ac_type_normal_ or ac_type > g_ac_type_trans_station then
			LuaLogE("按市场价的某个范围拍卖之帐号种类数值不正确");
			return;
		end
	else
		ac_type = g_ac_type_invalid;
	end
	ac_type = ac_type - 1;
	if product_cnt ~= nil then
		if type(product_cnt) ~= "number" then
			LuaLogE("按市场价的某个范围拍卖之拍卖条数限制的参数必须是数字类型");
			return;
		end
		if product_cnt <= 0 then
			product_cnt = -1;
		end
	else
		product_cnt = 1;
	end
	if type(dlt_price) ~= "number" then
		LuaLogE("按市场价的某个范围拍卖之默认价格类型必须是数字类型");
		return;
	end
	if dlt_price < 1 then
		LuaLogE("按市场价的某个范围拍卖之默认价格必须必须大于等于1");
		return;
	end
	if cnt == nil then
		cnt = 1;
	elseif type(cnt) ~= "number" then
		LuaLogE("按市场价的某个范围拍卖之数量参数必须是数字类型");
		return;
	elseif cnt <= 0 then
		cnt = 1;
	end
	return function( item )
		return SaleProductAsPriceBaseImpl(item, price_delta, dlt_price, cnt, product_cnt, ac_type);
	end
end

function KeepPromptDiscoverItemImpl( item )
end

function KeepPromptDiscoverItem()
	return function( item )
		return KeepPromptDiscoverItemImpl(item);
	end
end

function SendMailItemImpl( item, role_name )
	GPlayer_Me():SendMailItem(role_name, item);
end

function SendMailItem( role_name )
	if type(role_name) ~= "string" then
		LuaLogE("邮寄物品之角色名参数必须是字符串类型");
		return;
	end
	return function( item )
		return SendMailItemImpl(item, GbkToWideChar(role_name));
	end
end

function SellTheItemToShopImpl( item )
	GPlayer_Me():SellTheItemToShop(item);
end

function SellTheItemToShop()
	return SellTheItemToShopImpl;
end
