stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点		此问题已修复，坐标在门附近
pos_info:AddVertex(1, 2760.93, 1571.35, 86.0616)
pos_info:AddVertex(2, 2761.93, 1572.35, 86.0616)

--坐标链接
pos_info:AddEdge(1,2)

--房间
room_info:AddPosInfo(10,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003F2,room_info)
