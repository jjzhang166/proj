stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(46,560.825745,1996.941162,27.085236)
pos_info:AddVertex(45,566.897034,1997.851440,26.075123)
pos_info:AddVertex(44,573.108826,1997.363037,25.286102)
pos_info:AddVertex(43,579.351379,1996.674194,25.208664)
pos_info:AddVertex(42,585.608276,1996.006836,25.121403)

--坐标链接
pos_info:AddEdge(46,45)
pos_info:AddEdge(45,44)
pos_info:AddEdge(44,43)
pos_info:AddEdge(43,42)

--房间
room_info:AddPosInfo(2,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003EB,room_info)
