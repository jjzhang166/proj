stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

-- 坐标节点   梅杰波尔坦-喵喵
pos_info:AddVertex(280,1332.548706,1058.200195,13.270680)
pos_info:AddVertex(279,1338.555298,1056.477539,12.864448)
pos_info:AddVertex(278,1341.496704,1050.724121,12.863737)
pos_info:AddVertex(277,1356.174438,1043.917603,12.859019)
pos_info:AddVertex(276,1349.843384,1044.571655,12.861126)
pos_info:AddVertex(275,1343.524414,1045.356079,12.863224)
pos_info:AddVertex(274,1336.853149,1045.692383,12.864320)
pos_info:AddVertex(273,1327.709106,1026.786987,12.180199)
pos_info:AddVertex(272,1330.392090,1032.255127,12.220323)
pos_info:AddVertex(271,1333.552856,1037.633911,12.703113)
pos_info:AddVertex(270,1333.645386,1043.739502,13.270018)
pos_info:AddVertex(269,1331.372070,1049.411987,13.911783)
pos_info:AddVertex(268,1326.044800,1052.129761,15.443522)
pos_info:AddVertex(267,1324.391357,1058.284912,15.443522)
pos_info:AddVertex(266,1325.678833,1064.167358,15.457356)

--坐标链接
pos_info:AddEdge(280,279)
pos_info:AddEdge(279,278)
pos_info:AddEdge(278,275)
pos_info:AddEdge(277,276)
pos_info:AddEdge(276,275)
pos_info:AddEdge(275,274)
pos_info:AddEdge(273,272)
pos_info:AddEdge(272,271)
pos_info:AddEdge(271,270)
pos_info:AddEdge(270,274)
pos_info:AddEdge(270,269)
pos_info:AddEdge(269,268)
pos_info:AddEdge(268,267)
pos_info:AddEdge(267,266)

--房间
room_info:AddPosInfo(2,pos_info)

--副本模板
stage_pos_mgr:AddStageInfo(0x0000070A,room_info)
