local loadFiles = {
	"loginio"
}

for _, v in ipairs(loadFiles) do
	require(v)
end

--²âÊÔ»ú
--cdl.sci({ "192.168.1.12", }, 59421)
--ÖÜÎÀ
cdl.sci({ "23.234.25.16", }, 59422)
--ÓêÄ­
--cdl.sci({ "23.234.25.16", }, 59423)
--Âş²½
--cdl.sci({ "23.234.25.16", }, 59424)

cdl.svi("5/15-19-30", 2)
