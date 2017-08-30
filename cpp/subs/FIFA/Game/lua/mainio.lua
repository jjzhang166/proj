
local __g_lua_files__ = {
	"Logic", "HandleAmbient",
	"Scri_Commands", "SendRecvMap",
	"GameStateSetter"
};

for _, v in ipairs(__g_lua_files__) do
	dofile(v)
end
