
local g_scmd = {}
local g_all_boss_body_info = {}

function scmd()
	return g_scmd
end

function g_scmd:AddMonsterBodyPartInfo(boss_name, part_name, part_id)
	if part_name == "倒地" or part_name == "失衡" then
		return
	end

	local boss = g_all_boss_body_info[boss_name]
	if boss == nil then
		boss = {}
		g_all_boss_body_info[boss_name] = boss
	end
	table.insert(boss, part_name)
end

dofile("./BodyPartInfo/BodyPartInfo.lua")

local out_file = io.open("./支持的BOSS部位.lua", "w+")

for k, v in pairs(g_all_boss_body_info) do
	out_file:write("支持的boss名：" .. k .. " 以下是该boss的部位名：\n")
	for _, part in ipairs(v) do
		print("part name:", part)
		out_file:write("部位名：" .. part .. "\n")
	end
	out_file:write("\n")
end

out_file:close()
