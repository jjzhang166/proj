local loadFiles = {
	"loginio"
}

for _, v in ipairs(loadFiles) do
	require(v)
end

cdl.sci({ "192.168.1.12", }, 59421)
cdl.svi("2016/12/26 19:57", 1)
