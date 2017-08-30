require("s_cmncfg")
s_StateResolver = {
    resolver = nil,
    ----------
    GetResolver = nil,
    ParseStates = nil,
    SetStateRoot = nil,
    ParseSetStateRoot = nil,
    Resolve = nil,
    StateContNode = nil,
    状态容器结点 = nil,
    StateUnionNode = nil,
    状态联合结点 = nil,
    StateFindPicNode = nil,
    状态找图结点 = nil,
}

function s_StateResolver:GetResolver()
    local resolver = self.resolver
    if resolver ~= nil then return resolver end
    resolver = StateResolver(s_cmncfg.nextStateGloTimeout)
    self.resolver = resolver
    return resolver
end

function s_StateResolver:ParseStates(tbl)
    local resolver = self:GetResolver()
    return resolver:ParseStates(tbl)
end

function s_StateResolver:SetStateRoot(stateRoot)
    local resolver = self:GetResolver()
    return resolver:SetRoot(stateRoot)
end

function s_StateResolver:ParseSetStateRoot(tbl)
    local resolver = self:GetResolver()
    if resolver:GetRoot() then return end
    local stateRoot = self:ParseStates(tbl)
    if stateRoot == nil then return end
    resolver:SetRoot(stateRoot)
    return stateRoot
end

function s_StateResolver:Resolve()
    local resolver = self:GetResolver()
    while true do
        local res = resolver:Resolve()
        if res > 0 then
            LogT("s_StateResolver:Resolve正常结束，处理的状态数：", res)
            break
        elseif res < 0 then
            LogD("s_StateResolver:Resolve结果小于0，表示出错了，怀疑没有判断到游戏现在的状态，结果：", res)
            break
        else
            LogT("s_StateResolver:Resolve结果为0，表示没有判断到游戏现在的状态，延迟后再次循环")
        end
        Delay(2000)
    end    
end

function s_StateResolver.StateContNode(...)
    return s_StateResolver:GetResolver():StateContNode(...)
end
s_StateResolver.状态容器结点 = s_StateResolver.StateContNode

function s_StateResolver.StateUnionNode(...)
    return s_StateResolver:GetResolver():StateUnionNode(...)
end
s_StateResolver.状态联合结点 = s_StateResolver.StateUnionNode

function s_StateResolver.StateFindPicNode(...)
    return s_StateResolver:GetResolver():StateFindPicNode(...)
end
s_StateResolver.状态找图结点 = s_StateResolver.StateFindPicNode
