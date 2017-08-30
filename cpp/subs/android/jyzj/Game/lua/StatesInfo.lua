require("mycmd")

function LoadStatesInfo()
    local dm_command = dm_command
    local DmS_FindPicClick = dm_command.DmS_FindPicClick
    local DmS_FindPicPassScene = dm_command.DmS_FindPicPassScene
    local DmS_FindPic = dm_command.DmS_FindPic
    local StateFindPicNode = dm_command.StateFindPicNode
    local DmS_HandleClick = dm_command.DmS_HandleClick
    local DmS_HandlePassScene = dm_command.DmS_HandlePassScene
    local DmS_HandleNothing = dm_command.DmS_HandleNothing
    local DmS_FindPicInfo = dm_command.DmS_FindPicInfo
    local resolver = s_StateResolver:GetResolver()
    local StateContNode = s_StateResolver.StateContNode
    local StateUnionNode = s_StateResolver.StateUnionNode
    local states = {
        [StateFindPicNode("login", { DmS_HandleClick, DmS_HandlePassScene, DmS_HandleNothing })] = 
        {
            DmS_FindPicInfo("loginacceptxieyi"),
            DmS_FindPicInfo("loginmail"),
            DmS_FindPicInfo("loginmailac", mycmd. ‰»Î’ ∫≈√‹¬Î),
        },
    }
    --s_StateResolver:ParseSetStateRoot(states)
    local rootState = s_StateResolver:ParseStates(states)
    s_StateResolver:SetStateRoot(rootState)
    return rootState
end