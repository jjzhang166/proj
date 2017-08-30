local loadFiles = {
	"loginio", "VersionCfg"
}

for _, v in ipairs(loadFiles) do
	require(v)
end

cdl.sci({ VersionCfg.ip, }, VersionCfg.port)

cdl.svi("8-22-12", 2)
