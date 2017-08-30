require("mainfrm")
require("frmfactory_p")
mainfrm_p = {
    __index = mainfrm,

    CreateWorkArea = nil,
    -----------------------------------
    NewFrmFactory_ = nil,
}
setmetatable(mainfrm_p, mainfrm_p)

function mainfrm_p:NewFrmFactory_()
    return frmfactory_p
end

--[[
function mainfrm:CreateWorkArea()
   local factory = self.factory
	local splitterMain = wx.wxSplitterWindow(self.frame, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxDefaultSize, wx.wxSP_3DSASH)
	self.acList = factory:NewAcList_():Create(splitterMain, self)
	assert(self.acList)
	self.headerCtrl = factory:NewHeaderCtrl_():Create(splitterMain)
	assert(self.headerCtrl)
	splitterMain:Initialize(self.headerCtrl:GetLuaBase())
    --------------------------------------------------
    for _, v in self:GetAcListCfg():Iterator() do
        self.acList:AddItem(v)
    end
    --------------------------------------------------
    self.acList:Show(false)
end

function mainfrm:OnDoAddAc(ac, pwd, area, ser)
    if self:GetAcListCfg().FindItem(ac, area, ser) == nil and self:GetAcListCfg().GetItemData(0) then
		local first = self:GetAcListCfg().GetItemData(0);
        if first ~= nil then
            self.DeleteItem(0)
        end
	end
    local info, res
    if area == nil then
        info, res = self:GetAcListCfg():AddItem(ac, pwd, nil, ser)
    else
        info, res = self:GetAcListCfg():AddItem(ac, pwd, nil, area, ser)
    end
    if res ~= false then
       self.acList:AddItem(info)
    end
    return info, res
end

function mainfrm:OnP_Login(isAll)
    if not self.myApp:CanLoginGameAc() then return end
    local gamePath = self.consoleCfg.mainCfg.gamePath
    if gamePath == nil then
        assert(false)
		return
    end

    local acList = self:GetAcListCfg()
    local stateNil = acList.stateNil
    local stateWaitLogin = acList.stateWaitLogin
   -- for item in self.acList:IteratorP_Selected(isAll) do
        local itemData = acList:FindItemByIdx(0 + 1)
        if itemData.checked == true and itemData:GetState() == stateNil then
            local itemKey = itemData:GetItemKey()
            self:OnS_GetAcItemInfo(itemKey)
            if self.uiOperHandler:Login(itemKey, gamePath, false) then
                itemData:SetState(stateWaitLogin)
                self.acList:UpdateItem(item)
            end
        end
    
end
--]]