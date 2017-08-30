--登陆服务器坐标
g_login_service_pos = {
	--x坐标，y坐标，服务器ID
	华东电信 = { 626, 359, 16777473 },
	华南电信 = { 833, 353, 33554689 },
	华北网通 = { 1001, 354, 16777985 },
	西南电信 = { 1001, 354, 50331905 },
};
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

function MixLua()
	JsonValue = 1;
end
