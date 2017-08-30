
CmnBase = {
EmptyTbl = {},
NewTbl = nil,
Object = {
    New = nil,  --构造类或对象
},
Super = nil,
MetaSetAs = nil,
Inherit = nil,
CreateClass = nil,  --构造类，多重继承，返回的class是继承自CmnBase.Object的
ReadBaseOnly = nil, --不能改写父类的成员
ReadOnly = nil,  --只读表
ChooseCall = nil,
ExtCObj = nil,
GrandsonAsSon = nil, --假如存在：t.a.b，那么ChildAsAttri(t).b == t.a.b and ChildAsAttri(t) == t。
UnionTable = nil, --求表的并集，合并表
CopyTableValue = nil, --复制值，但不复制key
CopyTable = nil,	--浅拷贝
OptionCopy = nil,   --如果存在key，则不copy
TableFind = nil,
TableAppend = nil,
UnpackValue = nil, --Unpack表的值，不包括key
Reverse = nil,
Startwith = nil,
MtTostring = nil,
IndexCall2Str = nil,
IndexCallProxy = nil,
BitExcept = nil,
ToPlutoStr = nil,
FromPlutoStr = nil,
MetaForwardSet = nil,
SelfMetaForwardSet = nil,
ForwardIndexBut = nil,
}

Object = CmnBase.Object

function CmnBase.NewTbl()
	return {}
end

function CmnBase.MetaSetAs(tbl, k, v)
	local mt = getmetatable(tbl)
	if mt then
		assert(rawget(mt, k) == nil)
	else
		assert(rawget(tbl, k) == nil)
		setmetatable(tbl, tbl)
		mt = tbl
	end
	rawset(mt, k, v)
    return tbl
end

function CmnBase.Inherit(derived, base)
	return CmnBase.MetaSetAs(derived, "__index", base)
end

function CmnBase.Object:New(o)
	return CmnBase.Inherit(o or {}, self)
end

function CmnBase.Super(derived)
	local mt = getmetatable(derived)
	if mt ~= nil then return rawget(mt, "__index") end
end

local function Search(k, plist)
    for i = 1, #plist do
        local v = plist[i][k]
        if v ~= nil then return v end
	end
end

function CmnBase.CreateClass(...)
    local c = CmnBase.Object:New()
    local firstIdx = nil
    local omt = getmetatable(c)
    if omt ~= nil then
        firstIdx = omt.__index
	else
		assert(rawget(c, "__index") == nil)
		omt = c
		setmetatable(c, c)
    end
    local parents = {...}
    if firstIdx == nil then
		rawset(omt, "__index", function(t, k)
                return Search(k, parents)
            end)
    elseif type(firstIdx) == "function" then
        rawset(omt, "__index", function(t, k)
                local v = firstIdx(c, k)
                if v ~= nil then return v end
				return Search(k, parents)
            end)
    else
        rawset(omt, "__index", function(t, k)
                local v = firstIdx[k]
                if v ~= nil then return v end
				return Search(k, parents)
            end)
    end
    return c
end

function CmnBase.ReadBaseOnly(o)
    local oIndex = nil
    local omt = getmetatable(o)
    if omt ~= nil then
        oIndex = omt.__index
    end
	local proxy = { __index = o }
    if oIndex == nil then
        proxy.__newindex = o
    elseif type(oIndex) == "function" then
        proxy.__newindex = function (t, k, v)
            if oIndex(o, k) ~= nil then
                assert(false, "can not set value of parent class member, tag is function")
                return
            end
            o[k] = v
        end
    else
        proxy.__newindex = function (t, k, v)
            if oIndex[k] ~= nil then
                assert(false, "can not set value of parent class member, tag is table")
                return
            end
            o[k] = v
        end
    end
    setmetatable(proxy, proxy)
    return proxy
end

function CmnBase.ReadOnly(o)
    local proxy = {
        __index = o,
        __newindex = function(t, k, v)
            assert(false, "can not set value of parent class member")
        end
    }
    setmetatable(proxy, proxy)
    return proxy
end

function CmnBase.ChooseCall(f, exclude, include)
    return function (frist_p, ...)
		if frist_p == exclude then
			frist_p = include
		end
		return f(frist_p, ...)
	end
end

function CmnBase.ExtCObj(derived, base)
	local obj = { __CObj_Derived__ = derived, __CObj_Base__ = base }
	function obj:GetDerived()
		return rawget(self, "__CObj_Derived__")
	end
	function obj:GetLuaBase()
		return rawget(self, "__CObj_Base__")
	end
	return CmnBase.MetaSetAs(obj, "__index", function (t, k)
		local res = derived[k]
		if res ~= nil then return res end
		res = base[k]
		if res == nil then return end
		if type(res) == "function" then
			return CmnBase.ChooseCall(res, t, base)
		end
		return res
	end)
end

function CmnBase.GrandsonAsSon(o)
	assert(getmetatable(o) == nil)
	setmetatable(o, { __index = function (t, k)
		for _, v in pairs(o) do
			local res = v[k]
			if res ~= nil then
				return res
			end
		end
	end })
	return o
end

function CmnBase.UnionTable(...)
	local res = {}
	for _, t in ipairs({...}) do
		for k, v in pairs(t) do
            if res[k] == nil then
                res[k] = v
            else
                table.insert(res, v)
            end
		end
	end
	return res
end

function CmnBase.CopyTableValue(t)
	local res = {}
	for _, v in pairs(t) do
		table.insert(res, v)
	end
	return res
end

function CmnBase.CopyTable(t)
	local res = {}
	for k, v in pairs(t) do
		res[k] = v
	end
	return res
end

function CmnBase.OptionCopy(src, dst)
    for k, v in pairs(src) do
        if dst[k] == nil then
            dst[k] = v
        end
    end
    return dst
end

function CmnBase.TableFind(tbl, value)
    for _, v in pairs(tbl) do
        if v == value then return true end
    end
    return false
end

function CmnBase.TableAppend(lhs, rhs)
    for _, v in pairs(rhs) do
        table.insert(lhs, v)
    end
    return lhs
end

function CmnBase.UnpackValue(t)
	return unpack(CmnBase.CopyTableValue(t))
end

function CmnBase.Reverse(t)
    local tmp = {}
    for i = #t, 1, -1 do
        table.insert(tmp, t[i])
    end
    return tmp
end

function CmnBase.Startwith(str, beginStr)
    return string.find(str, beginStr) == 1
end

function CmnBase.MtTostring(o, str)
	o = o or {}
	return CmnBase.MetaSetAs(o, "__tostring", function() return str end)
end

local function IndexCall2Str_IndexImpl(t, k)
	return CmnBase.IndexCall2Str(nil, tostring(t) .. "." .. k)
end

local function IndexCall2Str_CallImpl(t, ...)
	local param = {...}
	local res = tostring(t) .. "("
	local size = #param
	for i, v in ipairs(param) do
		if type(v) == "string" then
			res = res .. "[[" .. tostring(v) .. "]]"
		else
			res = res .. tostring(v)
		end
		if i < size then
			res = res .. ","
		end
	end
	res = res .. ")"
	return res
end

function CmnBase.IndexCall2Str(o, str)
	o = CmnBase.MtTostring(o, str)
	local mt = getmetatable(o)
	assert(mt)
	mt.__index = IndexCall2Str_IndexImpl
	mt.__call = IndexCall2Str_CallImpl
	return o
end

function CmnBase.IndexCallProxy(o, str, callImpl)
	o = CmnBase.MtTostring(o, str)
	local mt = getmetatable(o)
	assert(mt)
	mt.__index = function(t, k)
		return CmnBase.IndexCallProxy(nil, tostring(t) .. "." .. k, callImpl)
	end
	mt.__call = function(t, ...)
		return callImpl("return " .. IndexCall2Str_CallImpl(t, ...), tostring(t))
	end
	return o
end

function CmnBase.BitExcept(org, except)
	return bit.band(org, bit.bnot(except))
end

function CmnBase.ToPlutoStr(perms, anyObj)
	assert(anyObj ~= nil)
	perms = perms or CmnBase.EmptyTbl
	return pluto.persist(perms, anyObj)
end

function CmnBase.FromPlutoStr(perms, str)
	assert(str)
	perms = perms or CmnBase.EmptyTbl
	return pluto.unpersist(perms, str)
end

function CmnBase.MetaForwardSet(tbl, metaKey, value)
	local mt = getmetatable(tbl)
	if not mt then
		if value == nil then return end
		assert(rawget(tbl, metaKey) == nil)
		rawset(tbl, metaKey, value)
		setmetatable(tbl, tbl)
		return
	end
	local v = rawget(mt, metaKey)
	if v then
		if v == value then return end
		if type(v) == "function" then return end
		return CmnBase.MetaForwardSet(v, metaKey, value)
	end
	rawset(mt, metaKey, value)
end

function CmnBase.SelfMetaForwardSet(tbl, metaKey, value)
	if getmetatable(tbl) == nil then setmetatable(tbl, tbl) end
	CmnBase.MetaForwardSet(tbl, metaKey, value)
end

function CmnBase.ForwardIndexBut(tbl, key, butIdx)
	local v = rawget(tbl, key)
	if v ~= nil then return v end
	local mt = getmetatable(tbl)
	if mt == nil then return end
	local h = mt.__index
	if h == nil then return end
	if h == butIdx then return end
	if type(h) == "function" then return h(tbl, key) end
	return CmnBase.ForwardIndexBut(h, key, butIdx)
end