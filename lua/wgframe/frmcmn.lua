require("CmnBase")
frmcmn = {
MakeProxy = nil
}

function frmcmn.MakeProxy(name, isSync)
	if isSync == nil then
			isSync = false
		end
	return CmnBase.IndexCallProxy(nil, name, function(...)
		return Shell(..., isSync)
	end)
end