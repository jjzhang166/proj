stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点  新手训练
pos_info:AddVertex(780,42.130089,32.524040,1.166345)
pos_info:AddVertex(779,39.729450,38.060108,1.166345)
pos_info:AddVertex(778,45.578568,39.839817,1.166345)
pos_info:AddVertex(777,52.052475,39.553856,1.166345)
pos_info:AddVertex(776,49.567612,33.864071,1.166345)
pos_info:AddVertex(775,47.613041,27.758923,1.166345)
pos_info:AddVertex(774,45.281750,22.180183,1.166344)

--坐标链接
pos_info:AddEdge(780,779)
pos_info:AddEdge(779,778)
pos_info:AddEdge(778,777)
pos_info:AddEdge(777,776)
pos_info:AddEdge(776,775)
pos_info:AddEdge(775,774)

--房间
room_info:AddPosInfo(0,pos_info)
room_info:AddPosInfo(-1,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x0000057A,room_info)
