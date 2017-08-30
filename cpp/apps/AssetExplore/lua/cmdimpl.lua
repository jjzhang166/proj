function H_DoNew()
    return AS_FactoryMe():CreateNode("AS_Root")
end

function H_DoOpen(filePath)
    return AS_FactoryMe():ReadFromFile(filePath)
end

function H_DoSaveAs(node, filePath)
    if AS_FactoryMe():WriteToFile(node, filePath) == false then
        return false
    end
end

function H_DoAddNode(nodeParent, clsName, pathName)
	local node = AS_FactoryMe():CreateNode(clsName)
    pathName = pathName or "*dummpy name"
    node:SetPath(pathName, nodeParent)
    if nodeParent:Add(node, false) == false then
        return
    end
    return node
end

function H_DoDeleteNode(node)
	local p = node:Parent(false)
    p:Erase(node)
end

function H_DoReplacePath(node, path)
    local p = node:Parent(true)
    if p == nil then
        node:SetPath(path)
    else
        node:SetPath(path, p)
    end
end

function H_DoD2M(node)
    GetMyApp():AutoAddCfgFiles(node)
    local nodeParent = node:Parent(false)
    node = node:D2M()
	if node == nil then
		return
	end
    if nodeParent ~= nil then
        nodeParent:Add(node, true)
    end
    return node
end

function H_DoSetRef(nodeSrc, nodeDst)
    nodeSrc:SetRefTo(nodeDst)
    nodeSrc:Parent(false):Refresh(false)
end

local function HandlePerFile(inFile, outFile)
	Log("开始处理该文件：", inFile)
	local root = H_DoOpen(inFile)
	if not root then
		LogE(inFile, " 打开失败")
		return
	end
	root = H_DoD2M(root)
	if root == nil then
		LogE(inFile, " 转换成内存失败")
		return
	end
	if H_DoSaveAs(root, outFile) == false then
		LogE("保存到文件失败：", outFile)
		return
	end
	Log(inFile, " 处理成功，输出到：", outFile)
end

function H_DoCmdLineFiles()
	for i = 0, myApp:FileCount(true) - 1 do
		if HandlePerFile(myApp:GetFileName(true, i), myApp:GetFileName(false, i)) == false then
			break
		end
	end
end