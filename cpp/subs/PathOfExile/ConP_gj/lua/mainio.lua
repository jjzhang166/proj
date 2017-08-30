local loadFiles = {
	"loginio"
}

for _, v in ipairs(loadFiles) do
	require(v)
end

cdl.sci({ "23.234.25.16", }, 59422)
--cdl.sci({ "192.168.1.12", }, 59421)
cdl.svi("5/15-19-30", 2)
