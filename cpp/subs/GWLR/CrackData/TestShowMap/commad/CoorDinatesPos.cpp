#include "StdAfx.h"
#include "CoorDinatesPos.h"
#include "../commad/Log.h"

int CoorDinatesPos::m_nIndex = 0;	//同类对象共享  不占用对象空间
int CoorDinatesPos::m_nBigShowCount = 3;

LOG_VALUE;

CoorDinatesPos::CoorDinatesPos(void)
{
	m_fx = 0.0f;
	m_fy = 0.0f;
	m_fz = 0.0f;
	m_x = 0;
	m_y = 0;
	m_dwMapId = 0;
	m_dwRoomId = 0;
	m_dwPosId = 0;
}

CoorDinatesPos::~CoorDinatesPos(void)
{
	m_theConnectPosList.clear();
}


int CoorDinatesPos::AddPos(PCOORDINATES_POS_DATA pthePosData)
{
	try
	{
		m_fx = pthePosData->fx;
		m_fy = pthePosData->fy;
		m_fz = pthePosData->fz;

		m_x = (int)pthePosData->fx;
		m_y = (int)pthePosData->fy;

		assert(pthePosData->dwMapId);
		m_dwMapId = pthePosData->dwMapId;

		m_dwRoomId = pthePosData->dwRoomId;

		m_dwPosId = GetRandPosId();
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

int CoorDinatesPos::AddLuaPos(PCOORDINATES_POS_DATA pthePosData,DWORD dwPosId)
{
	try
	{
		m_fx = pthePosData->fx;
		m_fy = pthePosData->fy;
		m_fz = pthePosData->fz;

		m_x = (int)pthePosData->fx;
		m_y = (int)pthePosData->fy;

		assert(pthePosData->dwMapId);
		m_dwMapId = pthePosData->dwMapId;

		m_dwRoomId = pthePosData->dwRoomId;

		m_dwPosId = dwPosId;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

int CoorDinatesPos::InitRole(PCOORDINATES_POS_DATA pthePosData)
{
	try
	{
		m_fx = pthePosData->fx;
		m_fy = pthePosData->fy;
		m_fz = pthePosData->fz;

		m_x = (int)pthePosData->fx;
		m_y = (int)pthePosData->fy;

		assert(pthePosData->dwMapId);
		m_dwMapId = pthePosData->dwMapId;

		m_dwRoomId = pthePosData->dwRoomId;

	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

int CoorDinatesPos::AddConnectPos(CoorDinatesPos* ptheCoorDinatesPos)
{
	try
	{
		if( FindConnectPos(ptheCoorDinatesPos->GetPosId()) == NULL )
		{
			m_theConnectPosList.push_back(ptheCoorDinatesPos);
			return 1;
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	return 0;
}

int CoorDinatesPos::DelConnectPos(CoorDinatesPos* ptheCoorDinatesPos)
{
	try
	{
		// 删除符合的坐标对象
		m_theConnectPosList.remove(ptheCoorDinatesPos);
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

CoorDinatesPos* CoorDinatesPos::FindConnectPos(DWORD dwPosId)
{
	try
	{
		std::list<CoorDinatesPos*>::reverse_iterator it = m_theConnectPosList.rbegin();
		for (; it != m_theConnectPosList.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos->GetPosId() == dwPosId )
			{
				return ptheCoorDinatesPos;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	return NULL;
}

void CoorDinatesPos::Save()
{
	try
	{
		
		char szPrint[256] = {0};
		std::list<CoorDinatesPos*>::reverse_iterator it = m_theConnectPosList.rbegin();
		for (; it != m_theConnectPosList.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			sprintf_s(szPrint,"pos_info:AddEdge(%d,%d)",m_dwPosId,ptheCoorDinatesPos->GetPosId());
			LOG << szPrint; LOG_END;
		}
	}
	catch (...)
	{
		LOG << "保存异常!";
		assert(false);
	}
	
	
}


//////////////////////////////////////////////////////////////////////////
// 管理地址对象类
CmdCoorDinatesPos	theCmdCoorDinatesPos;



CmdCoorDinatesPos::CmdCoorDinatesPos(void)
{
	m_pTempShape = NULL;
	m_lpBaseFactory = new CMyLineFactory;
}

CmdCoorDinatesPos::~CmdCoorDinatesPos(void)
{
	Remove();
}


void CmdCoorDinatesPos::Init(PCOORDINATES_POS_DATA ptheCoorDinatesPosData)
{
	try
	{

		CoorDinatesPos* ptheCoorDinatesPos = new CoorDinatesPos;

		if( ptheCoorDinatesPos != NULL )
		{
			ptheCoorDinatesPos->AddPos(ptheCoorDinatesPosData);
			
			m_theCmdLock.Lock("[LOG] Init Lock");
			// 自动生成与上一个节点连接
			if( m_CoorDinatesPosList_cmd.size() > 0 )
			{
				CoorDinatesPos* ptheCoorDinatesPos2 = m_CoorDinatesPosList_cmd.back();
				if( ptheCoorDinatesPos2 != NULL )
					ptheCoorDinatesPos->AddConnectPos(ptheCoorDinatesPos2);
			}
			m_CoorDinatesPosList_cmd.push_back(ptheCoorDinatesPos);
			m_theCmdLock.UnLock("[LOG] Init UnLock");

		}
	}
	catch (...)
	{
		assert(false);
	}
}

void CmdCoorDinatesPos::ReadLuaInit(PCOORDINATES_POS_DATA ptheCoorDinatesPosData,DWORD dwPosId)
{
	try
	{
		// 如果不存在id，则录入
		if( NULL == GetCoorDinatesPosAddr(dwPosId) ) 
		{
			CoorDinatesPos* ptheCoorDinatesPos = new CoorDinatesPos;

			if( ptheCoorDinatesPos != NULL )
			{
				ptheCoorDinatesPos->AddLuaPos(ptheCoorDinatesPosData,dwPosId);

				m_theCmdLock.Lock("[LOG] Init Lock");
				m_CoorDinatesPosList_cmd.push_back(ptheCoorDinatesPos);
				m_theCmdLock.UnLock("[LOG] Init UnLock");

			}
		}
	}
	catch (...)
	{
		assert(false);
	}
}

void CmdCoorDinatesPos::AddRoleNode()
{
	try
	{

		CoorDinatesPos* ptheCoorDinatesPos = new CoorDinatesPos;

		if( ptheCoorDinatesPos != NULL )
		{
			COORDINATES_POS_DATA theCoorDinatesPosData;

			theCoorDinatesPosData.fx = m_theRoleCoorDinatesPosData.m_fx;
			theCoorDinatesPosData.fy = m_theRoleCoorDinatesPosData.m_fy;
			theCoorDinatesPosData.fz = m_theRoleCoorDinatesPosData.m_fz;

			theCoorDinatesPosData.dwRoomId = m_theRoleCoorDinatesPosData.m_dwRoomId;
			theCoorDinatesPosData.dwMapId = m_theRoleCoorDinatesPosData.m_dwMapId;

			ptheCoorDinatesPos->AddPos(&theCoorDinatesPosData);

			
			m_theCmdLock.Lock("[LOG] AddRoleNode Lock");
			m_CoorDinatesPosList_cmd.push_back(ptheCoorDinatesPos);
			m_theCmdLock.UnLock("[LOG] AddRoleNode UnLock");
		}

	}
	catch (...)
	{
		assert(false);
	}
}




void CmdCoorDinatesPos::InitRole(PCOORDINATES_POS_DATA ptheCoorDinatesPosData)
{

	m_theRoleCoorDinatesPosData.InitRole(ptheCoorDinatesPosData);

}


// 函数功能： 显示坐标点和线
// 参数一,二： 优化显示靠左上角开始显示(坐标对象中坐标最小的x和y)
void CmdCoorDinatesPos::Show(int nAlignX,int nAlignY)
{
	CDC dcMemory;

	m_theCmdLock.Lock("[LOG] Show Lock");

	nAlignX -= SHOW_MAP_ALIGN_X;
	nAlignY -= SHOW_MAP_ALIGN_Y;
	try
	{
		//// CreateCompatibleDC()为屏幕DC创建兼容的内存DC
		if( !dcMemory.CreateCompatibleDC(m_pdc) )
		{
			::PostQuitMessage(0);
		}
		// 创建位图
		CBitmap Bmp;
		//CreateCompatibleBitmap()该函数创建与指定的设备环境相关的设备兼容的位图
		Bmp.CreateCompatibleBitmap(m_pdc,theCmdCoorDinatesPos.m_WndRect.Width()-130,theCmdCoorDinatesPos.m_WndRect.Height());

		// 相当于选择画布
		dcMemory.SelectObject(Bmp);

		// 白色填充, 在内存设备环境上
		dcMemory.FillSolidRect(&theCmdCoorDinatesPos.m_WndRect,RGB(255,255,255));
	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		// 画出所有的坐标点
		std::list<CoorDinatesPos*>::reverse_iterator it = theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rbegin();
		for (; it != theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			CMyRect theRect;

			theRect.m_thePen.m_crColor = RGB(255,0,0);

			theRect.m_theBegin.x = ptheCoorDinatesPos->GetX()-nAlignX;
			theRect.m_theBegin.y = ptheCoorDinatesPos->GetY()-nAlignY;

			theRect.m_theEnd.x = theRect.m_theBegin.x+5;
			theRect.m_theEnd.y = theRect.m_theBegin.y+5;

			theRect.OnDraw(&dcMemory);	
		}
	}
	catch (...)
	{
		assert(false);
	}

	

	try
	{
		std::list<CoorDinatesPos*>::reverse_iterator it1 = theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rbegin();
		for (; it1 != theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rend(); ++it1)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it1;

			CMyLine theLine;
			theLine.m_theBegin.x = ptheCoorDinatesPos->GetX()-nAlignX;
			theLine.m_theBegin.y = ptheCoorDinatesPos->GetY()-nAlignY;


			// 遍历并画坐标点和相连的点的线
			std::list<CoorDinatesPos*>::reverse_iterator it2 = ptheCoorDinatesPos->m_theConnectPosList.rbegin();
			for (; it2 != ptheCoorDinatesPos->m_theConnectPosList.rend(); ++it2)
			{
				CoorDinatesPos* ptheCoorDinatesPos2 = *it2;

				theLine.m_theEnd.x = ptheCoorDinatesPos2->GetX()-nAlignX;
				theLine.m_theEnd.y = ptheCoorDinatesPos2->GetY()-nAlignY;

				theLine.OnDraw(&dcMemory);	
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	try
	{

		CMyRect theRect;

		theRect.m_thePen.m_crColor = RGB(161,233,0);

		theRect.m_theBegin.x = m_theRoleCoorDinatesPosData.GetX()-nAlignX;
		theRect.m_theBegin.y = m_theRoleCoorDinatesPosData.GetY()-nAlignY;

		theRect.m_theEnd.x = theRect.m_theBegin.x+5;
		theRect.m_theEnd.y = theRect.m_theBegin.y+5;

		theRect.OnDraw(&dcMemory);	
	}
	catch (...)
	{
		assert(false);
	}


	try
	{
		if( theCmdCoorDinatesPos.m_pTempShape == NULL )
		{
			theCmdCoorDinatesPos.m_pTempShape = theCmdCoorDinatesPos.m_lpBaseFactory->CreateShape();
		}

		if( theCmdCoorDinatesPos.m_pTempShape != NULL )
		{
			theCmdCoorDinatesPos.m_pTempShape->OnDraw(&dcMemory);
		}

		// 一次性的将内存设备环境上绘制完毕的图形"贴"到屏幕上
		m_pdc->BitBlt(0, 0, theCmdCoorDinatesPos.m_WndRect.Width(), theCmdCoorDinatesPos.m_WndRect.Height(), &dcMemory, 0, 0, SRCCOPY);
	}
	catch (...)
	{
		assert(false);
	}
	
	m_theCmdLock.UnLock("[LOG] Show UnLock");
	
}

int CmdCoorDinatesPos::GetMinX()
{
	int x = 1000000;

	try
	{
		m_theCmdLock.Lock("[LOG] GetMinX Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rbegin();


		for (; it != theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos->GetX() < x )
			{
				x = ptheCoorDinatesPos->GetX();
			}
		}

		m_theCmdLock.UnLock("[LOG] GetMinX UnLock");
	}
	catch (...)
	{
		assert(false);
	}

	return x;
}

int CmdCoorDinatesPos::GetMinY()
{
	int y = 1000000;

	try
	{
		m_theCmdLock.Lock("[LOG] GetMinY Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rbegin();

		for (; it != theCmdCoorDinatesPos.m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos->GetY() < y )
			{
				y = ptheCoorDinatesPos->GetY();
			}
		}
		m_theCmdLock.UnLock("[LOG] GetMinY UnLock");
	}
	catch (...)
	{
		assert(false);
	}

	return y;
}

int CmdCoorDinatesPos::OnMouseMove( CPoint point)
{
	point.x += GetMinX();
	point.y += GetMinY();

	try
	{
		m_theCmdLock.Lock("[LOG] OnMouseMove Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos->GetX() >= (point.x-5) && ptheCoorDinatesPos->GetX() <= (point.x+5) &&
				ptheCoorDinatesPos->GetY() >= (point.y-5) && ptheCoorDinatesPos->GetY() <= (point.y+5) )
			{
				m_theCmdLock.UnLock("[LOG] OnMouseMove UnLock");
				return ptheCoorDinatesPos->GetPosId();
			}
		}

		m_theCmdLock.UnLock("[LOG] OnMouseMove UnLock");
	}
	catch (...)
	{
		assert(false);
	}
	
	return 0;
}

int CmdCoorDinatesPos::OnLButtonDown(UINT nFlags, CPoint point)
{
	try
	{
		if( nFlags == 1 )
		{
			point.x += GetMinX();
			point.y += GetMinY();

			m_theCmdLock.Lock("[LOG] OnLButtonDown Lock");
			std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

			for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
			{
				CoorDinatesPos* ptheCoorDinatesPos = *it;

				if( ptheCoorDinatesPos->GetX() >= (point.x-5) && ptheCoorDinatesPos->GetX() <= (point.x+5) &&
					ptheCoorDinatesPos->GetY() >= (point.y-5) && ptheCoorDinatesPos->GetY() <= (point.y+5) )
				{
					m_theCmdLock.UnLock("[LOG] OnLButtonDown UnLock");
					return ptheCoorDinatesPos->GetPosId();
				}
			}

			m_theCmdLock.UnLock("[LOG] OnLButtonDown UnLock");
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	return 0;
}

void CmdCoorDinatesPos::Remove()
{
	try
	{
		m_theCmdLock.Lock("[LOG] Remove Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos != NULL )
			{
				delete ptheCoorDinatesPos;
				ptheCoorDinatesPos = NULL;
			}
		}

		m_CoorDinatesPosList_cmd.clear();

		m_theCmdLock.UnLock("[LOG] Remove UnLock");
	}
	catch (...)
	{
		assert(false);
	}
}

CoorDinatesPos* CmdCoorDinatesPos::GetCoorDinatesPosAddr(DWORD dwPosId)
{
	try
	{
		m_theCmdLock.Lock("[LOG] GetCoorDinatesPosAddr Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos = *it;

			if( ptheCoorDinatesPos->GetPosId() == dwPosId )
			{
				m_theCmdLock.UnLock("[LOG] GetCoorDinatesPosAddr UnLock");
				return ptheCoorDinatesPos;
			}
		}
		m_theCmdLock.UnLock("[LOG] GetCoorDinatesPosAddr UnLock");
	}
	catch (...)
	{
		assert(false);
	}

	return NULL;
}

int CmdCoorDinatesPos::AddConnectPos(DWORD dwPosId1, DWORD dwPosId2)
{
	try
	{
		CoorDinatesPos* ptheCoorDinatesPos1 = GetCoorDinatesPosAddr(dwPosId1);
		if( ptheCoorDinatesPos1 == NULL )
		{
			return 0;
		}

		CoorDinatesPos* ptheCoorDinatesPos2 = GetCoorDinatesPosAddr(dwPosId2);
		if( ptheCoorDinatesPos2 == NULL )
		{
			return 0;
		}

		ptheCoorDinatesPos1->AddConnectPos(ptheCoorDinatesPos2);
	}
	catch (...)
	{
		assert(false);
	}

	return 1;
}

int CmdCoorDinatesPos::DelConnectPos(DWORD dwPosId1, DWORD dwPosId2)
{
	try
	{
		CoorDinatesPos* ptheCoorDinatesPos1 = GetCoorDinatesPosAddr(dwPosId1);
		if( ptheCoorDinatesPos1 == NULL )
		{
			return 0;
		}

		CoorDinatesPos* ptheCoorDinatesPos2 = GetCoorDinatesPosAddr(dwPosId2);
		if( ptheCoorDinatesPos2 == NULL )
		{
			return 0;
		}

		ptheCoorDinatesPos1->DelConnectPos(ptheCoorDinatesPos2);
		ptheCoorDinatesPos2->DelConnectPos(ptheCoorDinatesPos1);
	}
	catch (...)
	{
		assert(false);
	}

	return 1;
}

int CmdCoorDinatesPos::DelPos(DWORD dwPosId)
{
	try
	{
		CoorDinatesPos* ptheCoorDinatesPos = GetCoorDinatesPosAddr(dwPosId);
		if( ptheCoorDinatesPos == NULL )
		{
			return 0;
		}

		m_theCmdLock.Lock("[LOG] DelPos Lock");
		// 删除与节点有关系的节点连接
		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos_cmd = *it;

			std::list<CoorDinatesPos*>::reverse_iterator it1 = ptheCoorDinatesPos_cmd->m_theConnectPosList.rbegin();

			ptheCoorDinatesPos_cmd->DelConnectPos(ptheCoorDinatesPos);
		}

		// 删除节点
		m_CoorDinatesPosList_cmd.remove(ptheCoorDinatesPos);
		delete ptheCoorDinatesPos;
		ptheCoorDinatesPos = NULL;

		m_theCmdLock.UnLock("[LOG] DelPos UnLock");
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

void CmdCoorDinatesPos::Save()
{
	TCHAR szPrint[1024] = {0};
	_stprintf_s(szPrint, _T("0x%p-0x%d.lua"), m_theRoleCoorDinatesPosData.m_dwMapId, m_theRoleCoorDinatesPosData.m_dwRoomId);

	
	LOG_INIT(szPrint);

	LOG << "stage_pos_mgr = GSP_StagePosPath_Me()";LOG_END;
	LOG << "pos_info = GSP__PosInfo()";LOG_END;
	LOG << "room_info = GSP__RoomInfo()";LOG_END;
	char buffer[MAX_PATH];
	try
	{
		LOG << "\r\n-- 坐标节点";LOG_END;

		m_theCmdLock.Lock("[LOG] Save Lock");

		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos_cmd = *it;
		
			sprintf_s(buffer, "pos_info:AddVertex(%d,%f,%f,%f)", ptheCoorDinatesPos_cmd->GetPosId(), ptheCoorDinatesPos_cmd->m_fx, ptheCoorDinatesPos_cmd->m_fy, ptheCoorDinatesPos_cmd->m_fz);
			LOG << buffer; LOG_END;
		}
		m_theCmdLock.UnLock("[LOG] Save UnLock");
	}
	catch (...)
	{
		LOG << "保存异常!!";
		assert(false);
	}

	try
	{
		LOG << "\r\n--坐标链接";LOG_END;

		m_theCmdLock.Lock("[LOG] Save Lock");
		std::list<CoorDinatesPos*>::reverse_iterator it = m_CoorDinatesPosList_cmd.rbegin();

		for (; it != m_CoorDinatesPosList_cmd.rend(); ++it)
		{
			CoorDinatesPos* ptheCoorDinatesPos_cmd = *it;

			ptheCoorDinatesPos_cmd->Save();
		}
		m_theCmdLock.UnLock("[LOG] Save UnLock");
	}
	catch (...)
	{
		LOG << "保存异常!!";
		assert(false);
	}
	
	LOG << "\r\n--房间";LOG_END;
	// 写房间id
	sprintf_s(buffer, "room_info:AddPosInfo(%d,pos_info)", m_theRoleCoorDinatesPosData.m_dwRoomId);
	LOG << buffer; LOG_END;

	LOG << "\r\n--副本模板";LOG_END;
	// 写入副本id
	sprintf_s(buffer, "stage_pos_mgr:AddStageInfo(0x%p,room_info)", m_theRoleCoorDinatesPosData.m_dwMapId);
	LOG << buffer; LOG_END;

	LOG_CLOSE;
}

void CmdCoorDinatesPos::Read()
{
	char szPrint[1024] = {0};
	sprintf_s(szPrint,"0x%p-0x%d.lua",m_theRoleCoorDinatesPosData.m_dwMapId,m_theRoleCoorDinatesPosData.m_dwRoomId);




}