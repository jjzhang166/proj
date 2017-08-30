stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(12,634.855957,896.017395,30.006687)
pos_info:AddVertex(11,635.008362,889.525208,30.008177)
pos_info:AddVertex(10,627.642700,860.757629,30.485962)
pos_info:AddVertex(9,647.972473,847.457397,30.291056)
pos_info:AddVertex(8,641.623657,850.981262,30.000328)
pos_info:AddVertex(7,634.795959,882.303894,30.000000)
pos_info:AddVertex(6,634.760559,875.892883,30.002510)
pos_info:AddVertex(5,634.725159,869.481873,30.107758)
pos_info:AddVertex(4,634.690369,863.176453,30.143867)
pos_info:AddVertex(3,634.655579,856.836487,30.040611)
pos_info:AddVertex(2,634.622253,850.830444,30.000000)
pos_info:AddVertex(1,634.585022,844.833313,30.479145)

--坐标链接
pos_info:AddEdge(12,11)
pos_info:AddEdge(8,9)
pos_info:AddEdge(7,11)
pos_info:AddEdge(7,6)
pos_info:AddEdge(6,5)
pos_info:AddEdge(5,4)
pos_info:AddEdge(4,3)
pos_info:AddEdge(3,8)
pos_info:AddEdge(3,10)
pos_info:AddEdge(3,2)
pos_info:AddEdge(2,1)

--房间
room_info:AddPosInfo(0,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x00000579,room_info)
