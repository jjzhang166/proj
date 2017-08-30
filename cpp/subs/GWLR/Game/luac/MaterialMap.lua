
local g_material_map_ = {}

function MM_AddItem( material_name, gain_func, ... )
	if g_material_map_[material_name] ~= nil then
		LuaLogW("添加材料获取映射之已存在该映射了，所以忽略本次添加，材料名：" .. material_name)
		return false
	end
	local item_mgr = ItemDbMgr_Me()
	item_mgr:RebuildAll()
	if item_mgr:FindByName(material_name) == nil then
		LuaLogW("添加材料获取映射之不存在这样的物品信息，材料名：" .. material_name)
		return false
	end
	g_material_map_[material_name] = { gain_func, { ... } }
	return true
end

function MM_Clear()
	g_material_map_ = {}
end

function MM_ExistTheItem( material_name )
	return g_material_map_[material_name] ~= nil
end

function MM_DoGainTheMaterial_( material_name, cnt_limit )
	local delay_func = function ()
		local func_info = g_material_map_[material_name]
		if func_info == nil then
			return false
		end
		if cnt_limit >= 0 then
			if scmd():GetTheItemTotalCnt(material_name) >= cnt_limit then
				return false
			end
		end
		return RetBoolDef(func_info[1](unpack(func_info[2])), true)
	end
	return SafeRun_SkipExcept(delay_func, false)
end
