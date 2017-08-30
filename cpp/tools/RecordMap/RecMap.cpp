#include "stdafx.h"
#include "RecMap.h"
#include "RECM_Lua.h"
#include <Common/LogOut.h>
#include <wx/wxprec.h>

//////////////////////////////////////////////////////////////////////////
const UINT_PTR kTimerUpdate = WM_USER + 1;
const UINT_PTR kTimerRecording = WM_USER + 2;
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(RecordMap, CDialog)

RecordMap::RecordMap(CWnd* pParent /*=NULL*/)
: CDialog(RecordMap::IDD, pParent)
{
	h_accel_ = nullptr;
	last_hover_pos_id_ = -1;
	ofstm_ = nullptr;
	fn_get_role_pos_ = nullptr;
	fn_get_map_id_ = nullptr;
	fn_get_room_id_ = nullptr;
	room_id_ = -1;
	timer_update_ = false;
	timer_update_elapse_ = 100;
	timer_record_elapse_ = 900;
	timer_record_ = false;
	add_pos_mode_ = kAPM_ByRole;
}

RecordMap::~RecordMap()
{
}

void RecordMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAP_ID, edit_map_id_);
	DDX_Control(pDX, IDC_EDIT_ROOM_ID, edit_room_id_);
	DDX_Control(pDX, IDC_EDIT_LAST_OPER, edit_last_oper_);
	DDX_Text(pDX, IDC_EDIT_ZOOM_RATE, stage_mgr_.GetZoomRate());
	DDX_Control(pDX, IDC_BT_TIMER_UPDATE, bt_timer_update_);
	DDX_Text(pDX, IDC_EDIT_TIMER_UPDATE, timer_update_elapse_);
	DDX_Text(pDX, IDC_EDIT_TIMER_RECORD, timer_record_elapse_);
	DDX_Control(pDX, IDC_COMBO_ADD_POS_MODE, com_add_pos_mode_);
	auto& pos = stage_mgr_.GetRelativePos();
	DDX_Text(pDX, IDC_EDIT_RELATIVE_X, pos.x);
	DDX_Text(pDX, IDC_EDIT_RELATIVE_Y, pos.y);
}


BEGIN_MESSAGE_MAP(RecordMap, CDialog)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BT_IMPORT, &RecordMap::OnBnClickedBtImport)
	ON_BN_CLICKED(IDC_BT_EXPORT, &RecordMap::OnBnClickedBtExport)
	ON_COMMAND(ID_A_RECORD, &RecordMap::OnAccelRecord)
	ON_COMMAND(ID_A_ADD_POS, &RecordMap::OnAccelAddPos)
	ON_COMMAND(ID_A_ADD_SELECT_POS, &RecordMap::OnAddSelectPos)
	ON_COMMAND(ID_A_ADD_SELECT_EDGE, &RecordMap::OnAddSelectEdge)
	ON_COMMAND(ID_A_ERASE_SELECT_EDGE, &RecordMap::OnEraseSelectEdge)
	ON_COMMAND(ID_A_ERASE_SELECT_POS_INFO, &RecordMap::OnEraseSelectPosInfo)
	ON_COMMAND(ID_A_CLEAR_SELECT_POS, &RecordMap::ClearSelectPos)
	ON_BN_CLICKED(IDC_BT_SET_ZOOM_RATE, &RecordMap::OnBnClickedBtSetZoomRate)
	ON_BN_CLICKED(IDC_BT_SELECT_DLL, &RecordMap::OnBnClickedBtSelectDll)
	ON_BN_CLICKED(IDC_BT_TIMER_UPDATE, &RecordMap::OnBnClickedBtTimerUpdate)
	ON_BN_CLICKED(IDC_BT_CLEAR_DATA, &RecordMap::OnBnClickedBtClearData)
	ON_CBN_SELCHANGE(IDC_COMBO_ADD_POS_MODE, &RecordMap::OnSelchangeAddPosMode)
	ON_BN_CLICKED(IDC_BT_SET_RELATIVE_POS, &RecordMap::OnBtSetRelativePos)
END_MESSAGE_MAP()

void RecordMap::OnPaint()
{
	CDialog::OnPaint();
	
	CClientDC dc(this);
	static bool exec_once = false;
	if (!exec_once)
	{
		exec_once = true;
		CRect rc;
		GetClientRect(rc);
		rc.right -= 150;
		stage_mgr_.Init(dc, rc, this);
	}
	stage_mgr_.Paint(dc);
}

bool RecordMap::OnAddPos(int id, const stRC_Vector3& pos)
{
	if (!ofstm_)
		return false;
	if (config_info_.is_vector_2d_)
	{
		*ofstm_ << "pos_info:AddVertex(" << id << ", " << pos.x << ", " << pos.y << ")\n";
	}
	else
	{
		*ofstm_ << "pos_info:AddVertex(" << id << ", " << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
	}
	return true;
}

bool RecordMap::OnAddEdge(int id_lhs, int id_rhs)
{
	if (!ofstm_)
		return false;
	*ofstm_ << "pos_info:AddEdge(" << id_lhs << ", " << id_rhs << ")\n";
	return true;
}

void RecordMap::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
	bool need_update = false;	
	auto old_id = -1;
	auto pos = stage_mgr_.PosInfoFromMouse(point.x, point.y);
	if (pos)
	{
		if (last_hover_pos_id_ == pos->id_)
			return;
		old_id = last_hover_pos_id_;
		last_hover_pos_id_ = pos->id_;
		if (enRECM_PosState::kMouseHover != pos->state_)
		{
			pos->old_state_ = pos->state_;
			pos->state_ = enRECM_PosState::kMouseHover;
		}
		need_update = true;
	}
	else
	{
		old_id = last_hover_pos_id_;
		last_hover_pos_id_ = -1;
	}
	if (old_id >= 0)
	{
		auto pos = stage_mgr_.FindByPosId(old_id);
		if (pos)
		{
			if (enRECM_PosState::kMouseHover == pos->state_)
			{
				std::swap(pos->state_, pos->old_state_);
				need_update = true;
			}
		}
	}

	if (need_update)
		InvalidateRect(stage_mgr_.GetRect(), FALSE);
}


// RecordMap message handlers

void RecordMap::GSP__PosInfo::AddVertex(int id, float x, float y, float z)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		ptr->stage_mgr_.AddPos(id, stRC_Vector3(x, y, z));
	}
}

void RecordMap::GSP__PosInfo::AddEdge(int id_lhs, int id_rhs)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		ptr->stage_mgr_.AddEdge(id_lhs, id_rhs);
	}
}

void RecordMap::GSP__RoomInfo::AddPosInfo(int room_id, const GSP__PosInfo& pos_info)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		CString str;
		str.Format(_T("%x"), room_id);
		ptr->edit_room_id_.SetWindowText(str);
	}
}

void RecordMap::GSP_StagePosPath::AddStageInfo(int stage_id, const GSP__RoomInfo& room_info)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		CString str;
		str.Format(_T("%x"), stage_id);
		ptr->edit_map_id_.SetWindowText(str);
	}
}


void RecordMap::OnBnClickedBtImport()
{
	CFileDialog file_dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY,
		 _T("*.lua|*.lua||"));
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		if (PathFileExists(file_path))
		{
			stage_mgr_.ClearAll();
			stage_mgr_.SetZoomRate(5.5);

			std::string the_file_name = CT2CA(file_path);
			RECM_LuaVM::GetMe().DoFile(the_file_name.c_str());

			OnBnClickedBtSetZoomRate();

			/*CPath path_file(file_path);
			auto offset_file_name = path_file.FindFileName();
			if (offset_file_name < 0)
			{
				assert(false);
				return;
			}
			auto ptr_file_name = (LPCTSTR)file_path + offset_file_name;
			std::string the_file_name = CT2CA(ptr_file_name);
			SeparateString(the_file_name, "")*/
		}
	}
}

void RecordMap::OnBnClickedBtExport()
{
	auto room_id = GetRoomId();
	auto map_id = GetMapId();
	auto str_file_name = map_id;
	if (!config_info_.is_vector_2d_)
		str_file_name += "-" + room_id;
	str_file_name += ".lua";
	CString cstr_file_name = CA2CT(str_file_name.c_str());
	CFileDialog file_dlg(TRUE, nullptr, cstr_file_name, OFN_HIDEREADONLY,
		_T("*.lua|*.lua||"));
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		std::string the_file = CT2CA(file_path);
		std::ofstream ofstm(the_file, std::ios_base::out | std::ios_base::trunc);
		if (!ofstm)
		{
			LOG_O(Log_error) << "文件打开失败：" << the_file;
			return;
		}
		ofstm_ = &ofstm;
		BOOST_SCOPE_EXIT_ALL(&){
			ofstm_ = nullptr;
		};
		if (!config_info_.is_vector_2d_)
		{
			ofstm << R"(stage_pos_mgr = GSP_StagePosPath_Me()
pos_info = GSP__PosInfo()
room_info = GSP__RoomInfo()

)";
		}
		else
		{
		ofstm << R"(stage_pos_mgr = GSP_StagePosPath2d_Me()
pos_info = GSP__PosInfo2d()

)";
		}
		stage_mgr_.DoSaveData();

		if (!config_info_.is_vector_2d_)
		{
			ofstm << "\nroom_info:AddPosInfo(" << room_id << ", pos_info)\n";
			ofstm << "stage_pos_mgr:AddStageInfo(" << map_id << ", room_info)\n";
		}
		else
		{
			ofstm << "stage_pos_mgr:AddStageInfo(\"" << map_id << "\", pos_info)\n";
		}

		LOG_O(Log_trace) << "导出文件成功：" << the_file;
	}
}

BOOL RecordMap::PreTranslateMessage(MSG* pMsg)
{
	if (h_accel_)
	{
		if (::TranslateAccelerator(m_hWnd, h_accel_, pMsg))
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL RecordMap::OnInitDialog()
{
	if (!__super::OnInitDialog())
		return FALSE;
	h_accel_ = ::LoadAccelerators(wxGetInstance(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	com_add_pos_mode_.InsertString(kAPM_ByRole, _T("角色坐标模式"));
	com_add_pos_mode_.InsertString(kAPM_ByMouse, _T("鼠标模式"));
	com_add_pos_mode_.InsertString(kAPM_Mouse_Role, _T("鼠标设置角色坐标"));
	com_add_pos_mode_.SetCurSel(add_pos_mode_);
	return TRUE;
}

void RecordMap::OnAccelRecord()
{
	if (timer_record_)
	{
		edit_last_oper_.SetWindowText(_T("停止录制"));
		KillTimer(kTimerRecording);
		timer_record_ = false;
		return;
	}
	edit_last_oper_.SetWindowText(_T("开始录制"));
	if (!timer_update_)
	{
		LOG_O(Log_error) << "你没有启动更新计时，无法录制";
		return;
	}

	UpdateData(TRUE);

	if (!SetTimer(kTimerRecording, timer_record_elapse_, TimerRecording))
	{
		assert(false);
		return;
	}
	timer_record_ = true;
	LOG_O(Log_trace) << "开始录制了";
}

void RecordMap::OnAccelAddPos()
{
	edit_last_oper_.SetWindowText(_T("增加坐标点"));
	if (kAPM_ByRole == add_pos_mode_)
	{
		if (!fn_get_role_pos_)
		{
			LOG_O(Log_error) << "获取坐标函数为空，请点击更新计时来获取坐标函数";
			return;
		}
		if (!fn_get_role_pos_(role_pos_))
		{
			assert(false);
			return;
		}
		if (stage_mgr_.AddPos(role_pos_))
			InvalidateRect(stage_mgr_.GetRect(), FALSE);
	}
	else if (kAPM_ByMouse == add_pos_mode_)
	{
		CPoint pos;
		GetCursorPos(&pos);
		ScreenToClient(&pos);

		const auto& the_pos = stage_mgr_.GetPosFromMouse(pos.x, pos.y);
		if (stage_mgr_.AddPos(the_pos))
			InvalidateRect(stage_mgr_.GetRect(), FALSE);
	}
	else if (kAPM_Mouse_Role == add_pos_mode_)
	{
		if (!fn_get_role_pos_)
		{
			LOG_O(Log_error) << "获取坐标函数为空，请点击更新计时来获取坐标函数";
			return;
		}
		if (!fn_get_role_pos_(role_pos_))
		{
			assert(false);
			return;
		}
		CPoint pos;
		GetCursorPos(&pos);
		ScreenToClient(&pos);

		const auto& the_pos = stage_mgr_.GetPosFromMouse(pos.x, pos.y);
		/*auto role_pos_ptr = const_cast<stRC_Vector3*>(role_pos_);
		role_pos_ptr->x = the_pos.x;
		role_pos_ptr->y = the_pos.y;
		role_pos_ptr->z = the_pos.z + 2;*/
		::Sleep(100);

		if (stage_mgr_.AddPos(role_pos_))
			InvalidateRect(stage_mgr_.GetRect(), FALSE);
	}
}

void RecordMap::OnAddSelectPos()
{
	edit_last_oper_.SetWindowText(_T("增加选择坐标"));
	if (last_hover_pos_id_ >= 0)
	{
		if (stage_mgr_.AddSelectPos(last_hover_pos_id_))
			InvalidateRect(stage_mgr_.GetRect(), FALSE);
	}
}

void RecordMap::OnAddSelectEdge()
{
	edit_last_oper_.SetWindowText(_T("增加选择边"));
	if (stage_mgr_.AddSelectPosEdges() > 0)
		InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

void RecordMap::OnEraseSelectEdge()
{
	edit_last_oper_.SetWindowText(_T("删除选择边"));
	if (stage_mgr_.EraseSelectPosEdges() > 0)
		InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

void RecordMap::OnEraseSelectPosInfo()
{
	edit_last_oper_.SetWindowText(_T("删除选择坐标"));
	if (stage_mgr_.EraseSelectPosInfo() > 0)
		InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

void RecordMap::ClearSelectPos()
{
	edit_last_oper_.SetWindowText(_T("清空选择坐标"));
	if (stage_mgr_.ClearSelectPos() > 0)
		InvalidateRect(stage_mgr_.GetRect(), FALSE);
}


void RecordMap::OnBnClickedBtSetZoomRate()
{
	UpdateData(TRUE);
	InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

std::string RecordMap::GetRoomId() const
{
	CString cstr;
	edit_room_id_.GetWindowText(cstr);
	auto num = _tcstol(cstr, nullptr, 16);
	char buffer[40];
	sprintf_s(buffer, "0x%x", num);
	return buffer;
}

std::string RecordMap::GetMapId() const
{
	CString cstr;
	edit_map_id_.GetWindowText(cstr);
	std::string map_id = CT2CA(cstr);
	return map_id;
	/*auto num = _tcstol(cstr, nullptr, 16);
	char buffer[40];
	sprintf_s(buffer, "0x%x", num);
	return buffer;*/
}

void RecordMap::OnBnClickedBtSelectDll()
{
	CFileDialog file_dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY,
		_T("*.dll|*.dll||"));
	if (file_dlg.DoModal())
	{
		dll_file_full_name_ = file_dlg.GetPathName();
		if (PathFileExists(dll_file_full_name_))
		{
			LOG_O(Log_info) << "选择了dll:" << dll_file_full_name_;
		}
		else
		{
			LOG_O(Log_error) << "该dll不存在：" << dll_file_full_name_;
		}
	}
}

void RecordMap::OnBnClickedBtTimerUpdate()
{
	if (timer_update_)
	{
		timer_update_ = false;
		KillTimer(kTimerUpdate);
		bt_timer_update_.SetWindowText(_T("开始计时"));

		if (timer_record_)
		{
			edit_last_oper_.SetWindowText(_T("停止录制"));
			KillTimer(kTimerRecording);
			timer_record_ = false;
		}
		return;
	}
	if (dll_file_full_name_.IsEmpty())
	{
		LOG_O(Log_error) << "你还没有选择dll";
		return;
	}
	if (!PathFileExists(dll_file_full_name_))
	{
		LOG_O(Log_error) << "该dll不是有效的：" << dll_file_full_name_;
		return;
	}
	auto h_dll = LoadLibrary(dll_file_full_name_);
	if (!h_dll)
	{
		LOG_O(Log_error) << "dll加载失败" << dll_file_full_name_;
		return;
	}
	auto fn_cfg_info = (DllGetConfigInfo)GetProcAddress(h_dll, "DllGetConfigInfo");
	if (fn_cfg_info)
		fn_cfg_info(config_info_);
	fn_get_role_pos_ = (DllGetRolePosPtr)GetProcAddress(h_dll, "DllGetRolePosPtr");
	if (!fn_get_role_pos_)
	{
		LOG_O(Log_error) << "该函数中没有DllGetRolePosPtr函数，dll名：" << dll_file_full_name_;
		return;
	}
	fn_get_map_id_ = (DllGetMapId)GetProcAddress(h_dll, "DllGetMapId");
	if (!fn_get_map_id_)
	{
		LOG_O(Log_error) << "该函数中没有DllGetMapId函数，dll名：" << dll_file_full_name_;
		return;
	}
	if (config_info_.has_room_id_)
	{
		fn_get_room_id_ = (DllGetNumT)GetProcAddress(h_dll, "DllGetRoomId");
		if (!fn_get_room_id_)
		{
			LOG_O(Log_error) << "该函数中没有DllGetRoomId函数，dll名：" << dll_file_full_name_;
			return;
		}
	}
	if (!fn_get_role_pos_(role_pos_))
	{
		LOG_O(Log_error) << "坐标竟然获取不到";
		return;
	}
	fn_get_map_id_(map_id_);
	if (fn_get_room_id_)
		room_id_ = fn_get_room_id_();

	CString str = CA2CT(map_id_.c_str());
	edit_map_id_.SetWindowText(str);

	str.Format(_T("%x"), room_id_);
	edit_room_id_.SetWindowText(str);

	UpdateData(TRUE);

	if (!SetTimer(kTimerUpdate, timer_update_elapse_, TimerUpdate))
	{
		assert(false);
		return;
	}
	timer_update_ = true;
	bt_timer_update_.SetWindowText(_T("停止更新"));
}

void CALLBACK RecordMap::TimerUpdate(_In_ HWND hwnd, _In_ UINT uMsg, _In_ UINT_PTR idEvent, _In_ DWORD dwTime)
{
	auto ptr = RecordMap::GetInstance();
	if (!ptr)
	{
		assert(false);
		return;
	}
	if (!ptr->fn_get_role_pos_)
	{
		LOG_O(Log_error) << "获取坐标函数为空，请点击更新计时来获取坐标函数";
		return;
	}
	if (!ptr->fn_get_role_pos_(ptr->role_pos_))
	{
		assert(false);
		return;
	}
	ptr->stage_mgr_.SetRolePos(ptr->role_pos_);
	ptr->InvalidateRect(ptr->stage_mgr_.GetRect(), FALSE);
}

void CALLBACK RecordMap::TimerRecording(_In_ HWND hwnd, _In_ UINT uMsg, _In_ UINT_PTR idEvent, _In_ DWORD dwTime)
{
	auto ptr = RecordMap::GetInstance();
	if (!ptr)
	{
		assert(false);
		return;
	}
	if (!ptr->fn_get_role_pos_)
	{
		LOG_O(Log_error) << "获取坐标函数为空，请点击更新计时来获取坐标函数";
		return;
	}
	if (!ptr->fn_get_role_pos_(ptr->role_pos_))
	{
		assert(false);
		return;
	}
	if (ptr->stage_mgr_.AddPos(ptr->role_pos_))
		ptr->InvalidateRect(ptr->stage_mgr_.GetRect(), FALSE);
}

void RecordMap::OnBnClickedBtClearData()
{
	stage_mgr_.ClearAll();
	if (timer_update_)
		KillTimer(kTimerUpdate);
	if (timer_record_)
		KillTimer(kTimerRecording);

	InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

void RecordMap::OnSelchangeAddPosMode()
{
	add_pos_mode_ = (enAddPosMode)com_add_pos_mode_.GetCurSel();
}

void RecordMap::OnBtSetRelativePos()
{
	UpdateData();
	InvalidateRect(stage_mgr_.GetRect(), FALSE);
}

void RecordMap::GSP__PosInfo2d::AddVertex(int id, int x, int y)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		ptr->stage_mgr_.AddPos(id, stRC_Vector3(x, y, 0));
	}
}

void RecordMap::GSP__PosInfo2d::AddEdge(int id_lhs, int id_rhs)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		ptr->stage_mgr_.AddEdge(id_lhs, id_rhs);
	}
}

void RecordMap::GSP_StagePosPath2d::AddStageInfo(const std::string& stage_id, const GSP__PosInfo2d& pos_info)
{
	auto ptr = RecordMap::GetInstance();
	if (ptr)
	{
		CString str = CA2CT(stage_id.c_str());
		ptr->edit_map_id_.SetWindowText(str);
	}
}
