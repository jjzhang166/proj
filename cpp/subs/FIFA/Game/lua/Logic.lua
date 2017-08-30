
--发送聊天信息
function SendChatMsg( msg )
	local local_msg = [[{"message":"]];
	local_msg = local_msg .. msg;
	local_msg = local_msg .. [[","hint":{"linkinfo":[]}}]];
	SendNormalMsgAsync("/chat/my/channel/message", local_msg);
	return "success";
end

--处理发送的包
function HandleSendHttpPackage( http_request, json_msg )
	--URI
	local http_uri = http_request:GetURI();
	--匹配多个字符串
	MatchSpecifyStrByTbl(g_tblMultiMatch_ByHookSendPackage, http_uri);

	for _, v in ipairs(g_tblMsgCataloghandler) do
		if v[1]() then
			if string.find(http_uri, v[3]) ~= nil then
				v[2](v, json_msg, http_request);
				break;
			end
		end
	end

	--有时会不发送消息内容的，所以json_root就为空
	-- if json_root ~= nil then
end

--处理收到的包
function HandleRecvHttpPackage( msg_head, json_root )
	if json_root == nil then
		return;
	end

	for _, v in ipairs(g_tblRecvMsgHandle) do
		if v[1]() then

		--是否匹配成功
		local is_matched_all = false;
		local resolved_value = nil;
		for idx, path in ipairs(v[3]) do
			local json_path = JsonPath(path);
			--resolved json value
			resolved_value = json_path:Resolve(json_root);
			if resolved_value:IsNull() then	--没有找到
				is_matched_all = false;
				break;
			else
				if v[4] ~= nil and v[4][idx] ~= nil then
					local value_option = v[4][idx];
					if resolved_value:IsString() then
						if string.find(resolved_value:AsString(), value_option) == nil then
							is_matched_all = false;
							break;
						end
					elseif resolved_value:IsBool() then
						if resolved_value:AsBool() ~= value_option then
							is_matched_all = false;
							break;
						end
					elseif resolved_value:IsUInt() then
						if resolved_value:AsUInt() ~= value_option then
							is_matched_all = false;
							break;
						end
					elseif resolved_value:IsInt() then
						if resolved_value:AsInt() ~= value_option then
							is_matched_all = false;
							break;
						end
					elseif resolved_value:IsNumeric() then
						if resolved_value:AsDouble() ~= value_option then
							is_matched_all = false;
							break;
						end
					else
						is_matched_all = false;
						break;
					end
				end
			end
			is_matched_all = true;
		end

		if is_matched_all then --找到了
			v[2](v, resolved_value);
		end

		end
	end
end

--return true
function RetTrue()
	return true;
end

--处理具体的消息表
function HandleSRMap_Tbl( map_tbl, http_uri, json_msg )
	for _, tbl_item in ipairs(map_tbl) do
		if tbl_item[2] == http_uri then
			tbl_item[1](json_msg);
			return;
		end
	end
end

--发包与收包的映射
function HandleSendRecvMap( quest_info, json_msg )
	if quest_info == nil then
		LuaLogE("quest info can not be nil");
		return;
	end
	if json_msg == nil then
		LuaLogE("json_msg can not be nil");
		return;
	end

	--post消息
	if quest_info.method == "POST" then
		HandleSRMap_Tbl(g_tblMapPost, quest_info.uri, json_msg);
		return;
	end
	--get消息
	if quest_info.method == "GET" then
		HandleSRMap_Tbl(g_tblMapGet, quest_info.uri, json_msg);
		return;
	end
	LuaLogE("HandleSendRecvMap之不是post消息，也不是get消息");
end

function HandleRequestMap_Tbl( map_tbl, quest, json_msg )
	local http_uri = quest:GetURI();
	for _, tbl_item in ipairs(map_tbl) do
		if tbl_item[2] == http_uri then
			tbl_item[1](quest, json_msg);
			return;
		end
	end
end
function HandleRequestRecvMap( quest, json_msg )
	if quest == nil then
		LuaLogE("quest can not be nil");
		return;
	end
	if json_msg == nil then
		LuaLogE("json_msg can not be nil");
		return;
	end

	--post消息
	local str_method = quest:GetMethod();
	if str_method == "POST" then
		HandleRequestMap_Tbl(g_tblRequestMapPost, quest, json_msg);
		return;
	end
	--get消息
	if str_method == "GET" then
		HandleRequestMap_Tbl(g_tblRequestMapGet, quest, json_msg);
		return;
	end
	LuaLogE("HandleRequestRecvMap之不是post消息，也不是get消息");
end

--收包的消息中，是否执行成功了
function RequestMsgSucceed( recv_msg )
	if recv_msg == nil then
		return false;
	end
	local res = recv_msg:Key("result");
	if res:IsNull() then
		return false;
	end
	if not res:IsBool() then
		LuaLogW("msg result is not bool");
		return false;
	end
	return res:AsBool();
end

--混乱LUA
function MixLua()
	GPlayerMe = 1;
end

g_is_start_run_info = true
g_is_out_to_console = true
function RunInfo( str )
	if g_is_start_run_info then
		LuaLog("当前执行信息：" .. str);
		if g_is_out_to_console then
			OutputScriptInfo(str)
		end
	end
end
