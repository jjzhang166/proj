stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(13,509.501221,3700.145996,55.005104)
pos_info:AddVertex(11,525.056396,3704.177246,55.005104)
pos_info:AddVertex(10,530.984924,3702.712646,55.005104)
pos_info:AddVertex(9,537.378418,3701.698730,55.431576)
pos_info:AddVertex(8,520.017334,3680.687988,55.314247)
pos_info:AddVertex(7,518.927856,3686.689941,55.005108)
pos_info:AddVertex(6,518.182861,3692.945068,55.005108)
pos_info:AddVertex(5,517.627808,3699.223633,55.005100)
pos_info:AddVertex(4,517.812927,3705.509521,55.005100)
pos_info:AddVertex(3,518.108337,3711.573730,56.564846)
pos_info:AddVertex(2,518.509766,3717.918945,57.776478)
pos_info:AddVertex(1,520.632996,3723.825195,57.962990)

--坐标链接
pos_info:AddEdge(13,5)
pos_info:AddEdge(11,5)
pos_info:AddEdge(11,4)
pos_info:AddEdge(11,10)
pos_info:AddEdge(10,9)
pos_info:AddEdge(8,7)
pos_info:AddEdge(7,6)
pos_info:AddEdge(6,5)
pos_info:AddEdge(5,4)
pos_info:AddEdge(4,3)
pos_info:AddEdge(3,2)
pos_info:AddEdge(2,1)

--房间
room_info:AddPosInfo(0,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003ED,room_info)
