-- $Id$

require "pluto"


permtable = 1234

rootobj = {
	testfalse = false,
	testseven = 7,
	testfoobar = "foobar",
	testnil = 1,
	permtable = permtable,
}

--permanents table
perms = { [permtable] = 1, [coroutine.yield] = 2, }
buf = pluto.persist(perms, rootobj)

outfile = io.open("f:/test.plh", "wb")
outfile:write(buf)
outfile:close()
