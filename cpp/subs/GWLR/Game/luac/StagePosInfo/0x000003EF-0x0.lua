stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(8,606.162231,3201.756592,28.593750)
pos_info:AddVertex(7,606.936768,3207.825439,28.593750)
pos_info:AddVertex(6,607.199341,3213.870850,28.593750)
pos_info:AddVertex(5,619.711365,3236.698486,28.599529)
pos_info:AddVertex(4,614.479065,3233.402832,28.594498)
pos_info:AddVertex(3,609.947449,3229.363281,28.594458)
pos_info:AddVertex(2,607.392944,3223.808594,28.594458)
pos_info:AddVertex(1,604.732971,3218.314941,28.598431)

--坐标链接
pos_info:AddEdge(8,7)
pos_info:AddEdge(7,6)
pos_info:AddEdge(6,1)
pos_info:AddEdge(5,4)
pos_info:AddEdge(4,3)
pos_info:AddEdge(3,2)
pos_info:AddEdge(2,1)

--房间
room_info:AddPosInfo(0,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003EF,room_info)
