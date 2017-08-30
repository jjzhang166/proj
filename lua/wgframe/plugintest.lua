plugintest = {
    RunWithMsgBox = nil,
    DofileWithMsgBox = nil,
}

function plugintest.RunWithMsgBox(fn, msg)
    msg = msg or ""
    local theFn = function()
        GetMyApp():RunWithMsgBox(fn, msg)
    end
    Shell(theFn)
end

function plugintest.DofileWithMsgBox(fileName)
    assert(fileName)
    assert(#fileName > 0)
    local fn = function()
        dofile(fileName)
    end
    local theFn = function()
        GetMyApp():RunWithMsgBox(fn, fileName)
    end
    Shell(theFn)
end
