--登陆服务器坐标
g_login_service_pos = {
	--ser x坐标, ser y坐标，服务器ID, ser region x, ser region y
	山城兰特 = { 633, 420, 257, 452, 367 },
	蛛网森林 = { 603, 462, 513, 452, 367 },
	骨鸣悬崖 = { 575, 502, 769, 452, 367 },
	红色剧场 = { 537, 541, 1025, 452, 367 },
	寒风营寨 = { 511, 585, 1281, 452, 367 },
	废墟哨塔 = { 482, 630, 1537, 452, 367 },
	苏尔之城 = { 451, 667, 1793, 452, 367 },
	迷雾峡谷 = { 482, 699, 2049, 452, 367 },
	幽眠峡谷 = { 650, 595, 2305, 452, 367 },

	迈尔矿山 = { 633, 420, 258, 405, 419 },
	魔法圣城 = { 603, 462, 514, 405, 419 },
	刃弯峡谷 = { 575, 502, 770, 405, 419 },
	冰刺山坡 = { 537, 541, 1026, 405, 419 },
	风暴峡谷 = { 511, 585, 1282, 405, 419 },
	莱温黑市 = { 482, 630, 1538, 405, 419 },
	鲁利广场 = { 451, 667, 1794, 405, 419 },

	灰烬回廊 = { 633, 420, 259, 361, 478 },
	晨曦祭坛 = { 603, 462, 515, 361, 478 },
	试炼缝隙 = { 575, 502, 771, 361, 478 },
	自由海港 = { 537, 541, 1027, 361, 478 },

	黄金码头 = { 633, 420, 260, 313, 534 },
	冰霜部落 = { 603, 462, 516, 313, 534 },
	沙漠之都 = { 575, 502, 722, 313, 534 },
	记忆战场 = { 537, 541, 1028, 313, 534 },
	魔女宅邸 = { 511, 585, 1284, 313, 534 },
	火焰祭坛 = { 482, 630, 1540, 313, 534 },
	巨人废矿 = { 451, 667, 1796, 313, 534 },
	魔像遗址 = { 482, 699, 2052, 313, 534 },
};
function Login_GetSerRegionPosX( service_name )
	return g_login_service_pos[service_name][4];
end
function Login_GetSerRegionPosY( service_name )
	return g_login_service_pos[service_name][5];
end
function Login_GetServicePosX( service_name )
	return g_login_service_pos[service_name][1];
end
function Login_GetServicePosY( service_name )
	return g_login_service_pos[service_name][2];
end
function Login_GetServiceNames( vec )
	for k, _ in pairs(g_login_service_pos) do
		vec:PushBack(k);
	end
end
--得到服务器ID
function Login_GetServiceId( service_name )
	return g_login_service_pos[service_name][3];
end
