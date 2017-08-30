-- $Id$

require "pluto"

permtable = 4321

--permanents table
perms = { [1] = permtable, [2] = coroutine.yield, }

infile, err = io.open("f:/test.plh", "rb")
if infile == nil then
	error("While opening: " .. (err or "no error"))
end

buf, err = infile:read("*a")
if buf == nil then
	error("While reading: " .. (err or "no error"))
end

infile:close()

rootobj = pluto.unpersist(perms, buf)

----[[
for k, v in pairs(rootobj) do
	print(k, "  ", v)
end
--]]

