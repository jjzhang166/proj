loginio = {
	Login = nil,
	RemoveBind = nil,
	Reg = nil,
	Pay = nil,
	LeaveMsg = nil,
}

function loginio.Login(...)
	assert(cdl)
	return cdl.l(...)
end

function loginio.RemoveBind(...)
	assert(cdl)
	return cdl.rb(...)
end

function loginio.Reg(...)
	assert(cdl)
	return cdl.ru(...)
end

function loginio.Pay(...)
	assert(cdl)
	return cdl.up(...)
end