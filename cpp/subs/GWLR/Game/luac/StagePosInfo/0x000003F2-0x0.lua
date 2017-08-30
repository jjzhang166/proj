stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(19,244.447845,3895.577393,81.956245)
pos_info:AddVertex(18,245.847549,3878.296387,80.370796)
pos_info:AddVertex(17,247.204987,3884.225342,80.975975)
pos_info:AddVertex(16,248.601852,3890.291016,81.361618)
pos_info:AddVertex(15,253.774246,3904.487061,80.846085)
pos_info:AddVertex(14,252.087097,3898.720703,81.210052)
pos_info:AddVertex(13,250.036346,3892.720703,81.400314)

--坐标链接
pos_info:AddEdge(19,13)
pos_info:AddEdge(18,17)
pos_info:AddEdge(17,16)
pos_info:AddEdge(16,13)
pos_info:AddEdge(16,19)
pos_info:AddEdge(15,14)
pos_info:AddEdge(14,13)

--房间
room_info:AddPosInfo(0,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003F2,room_info)
