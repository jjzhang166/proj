
--test
--[[
function RecvGet( json_msg )
	LuaLog("recv get succeed");
end
--]]

--查询保管箱物品
function Get_BagBox( bag_items )
	--[[
	if bag_items:Size() > 0 then
		--全部接收保管箱物品
		SendNormalMsgAsync("/bag/unpackall", "{}");
	end
	--]]
end

--发送查询保管箱物品
function SendGet_BagBox()
	--SendGetMsgAsync("/bag");
end

--拍卖球员
function Post_SellPlayer( quest, json_msg )
	local quest_msg = JsonValue();
	if not quest:DeContentToJson(quest_msg) then
		LuaLogE("can not convert to json");
		return;
	end
	if not RequestMsgSucceed(json_msg) then
		LuaLogW("拍卖球员没有成功");
	end
	GPlayerMe():OnSellPlayerSucceed(quest_msg:Key("playerobjectid"):AsString());
end
