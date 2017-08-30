stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(1173,806.339294,3186.647949,93.687500)
pos_info:AddVertex(8,822.274597,3141.256592,95.735123)
pos_info:AddVertex(7,822.757202,3147.438477,95.812500)
pos_info:AddVertex(6,822.427063,3153.728516,95.875000)
pos_info:AddVertex(5,821.512268,3160.043701,95.750000)
pos_info:AddVertex(4,818.349182,3165.434814,95.418648)
pos_info:AddVertex(3,814.694946,3170.548828,94.283501)
pos_info:AddVertex(2,810.976685,3175.591064,93.687500)
pos_info:AddVertex(1,806.944275,3180.677002,93.687500)
pos_info:AddVertex(438,802.432861,3178.999023,93.652054)

--坐标链接
pos_info:AddEdge(1173,1)
pos_info:AddEdge(8,7)
pos_info:AddEdge(7,6)
pos_info:AddEdge(6,5)
pos_info:AddEdge(5,4)
pos_info:AddEdge(4,3)
pos_info:AddEdge(3,2)
pos_info:AddEdge(2,1)
pos_info:AddEdge(438,1)

--房间
room_info:AddPosInfo(0,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003EB,room_info)
