stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点
pos_info:AddVertex(1053,308.345306,848.115540,17.834743)
pos_info:AddVertex(118,313.269043,847.558533,17.819397)
pos_info:AddVertex(119,314.196716,841.528992,17.880499)
pos_info:AddVertex(120,313.649170,835.158630,18.121683)
pos_info:AddVertex(121,310.248230,830.186768,18.745346)
pos_info:AddVertex(122,305.019806,827.246338,19.447647)
pos_info:AddVertex(123,299.387726,824.991638,20.111509)
pos_info:AddVertex(124,298.119354,831.156311,19.345097)
pos_info:AddVertex(125,296.325745,836.917908,18.515865)
pos_info:AddVertex(126,296.538574,843.030579,18.053972)
pos_info:AddVertex(127,300.848969,847.314209,17.922031)
pos_info:AddVertex(128,305.470245,851.325256,17.810614)
pos_info:AddVertex(129,309.765381,855.677246,17.725508)

--坐标链接
pos_info:AddEdge(1053,128)
pos_info:AddEdge(1053,118)
pos_info:AddEdge(118,129)
pos_info:AddEdge(119,118)
pos_info:AddEdge(120,119)
pos_info:AddEdge(121,120)
pos_info:AddEdge(122,124)
pos_info:AddEdge(122,121)
pos_info:AddEdge(123,122)
pos_info:AddEdge(124,123)
pos_info:AddEdge(125,124)
pos_info:AddEdge(126,125)
pos_info:AddEdge(127,126)
pos_info:AddEdge(128,127)
pos_info:AddEdge(129,128)

--房间
room_info:AddPosInfo(7,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x000003EC,room_info)
