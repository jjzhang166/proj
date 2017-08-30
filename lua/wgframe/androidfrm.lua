require("mainfrm")
require("CmnBase")
require("androidaclist")
androidfrm = {
    __index = mainfrm,
    --------------------------
    SetAcListItemVmName = nil,
    --------------------------
    --sinks
    OnS_AttachLoginSession = nil,
    OnS_LoginDisconnect = nil,
    OnS_AttachGameSession = nil,
    OnS_GameDisconnect = nil,
    --------------------------
    NewFrmFactory_ = nil,
}
setmetatable(androidfrm, androidfrm)
local mainfrm = mainfrm
--mainfrm = androidfrm

androidfrmfactory = {
    __index = frmfactory,
    ------------------------
    NewAcList_ = nil,
    GetAcListColumnIdxInfo_ = nil,
    GetAcListColumnNameInfo_ = nil,
}
setmetatable(androidfrmfactory, androidfrmfactory)
local frmfactory = frmfactory

function frmfactory:NewAcList_()
	return androidaclist
end

function androidfrmfactory:GetAcListColumnIdxInfo_()
    local res = frmfactory.GetAcListColumnIdxInfo_(self)
    local new = {
        "vmName",
    }
    return CmnBase.TableAppend(res, new)
    --[[
    local res = CmnBase.TableAppend(res, new)
    for k, v in pairs(res) do
        print(k, " ", v)
    end
    return res
    --]]
end

function androidfrmfactory:GetAcListColumnNameInfo_()
    local res = frmfactory.GetAcListColumnNameInfo_(self)
    local new = {
        "ÐéÄâ»ú",
    }
    return CmnBase.TableAppend(res, new)
end

function androidfrm:SetAcListItemVmName(itemKey, vmName)
    local acList = self:GetAcListCfg()
    local itemData = acList:FindItem(itemKey)
    if itemData == nil then
        assert(false, itemKey)
        return
    end
    assert(vmName)
    itemData.vmName = vmName
end

function androidfrm:OnS_AttachLoginSession(itemKey, vmName)
    self:SetAcListItemVmName(itemKey, vmName)
    return mainfrm.OnS_AttachLoginSession(self, itemKey)
end

function androidfrm:OnS_LoginDisconnect(itemKey, stateKey, vmName)
    self:SetAcListItemVmName(itemKey, vmName)
    return mainfrm.OnS_LoginDisconnect(self, stateKey, itemKey)
end

function androidfrm:OnS_AttachGameSession(itemKey, vmName)
    self:SetAcListItemVmName(itemKey, vmName)
    return mainfrm.OnS_AttachGameSession(self, itemKey)
end

function androidfrm:OnS_GameDisconnect(itemKey, stateKey, vmName)
    self:SetAcListItemVmName(itemKey, vmName)
    return mainfrm.OnS_GameDisconnect(self, itemKey, stateKey)
end

function androidfrm:NewFrmFactory_()
    return androidfrmfactory
end
