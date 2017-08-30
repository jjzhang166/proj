#pragma once
/*
@author zhp
@date 2016/6/19 21:07
*/
#include "Resource.h"
#include <Common/CmnRecordMap.h>

typedef void(*DllGetConfigInfo)(stRC_Config&);
typedef bool(*DllGetRolePosPtr)(stVec3Float&);
typedef int(*DllGetNumT)();
typedef void(*DllGetMapId)(std::string&);

class RecordMap : public CDialog, public RECM_StageSink, public Singleton<RecordMap, Singleton_Instance>{
	DECLARE_DYNAMIC(RecordMap)
public:
	//////////////////////////////////////////////////////////////////////////
	struct GSP__PosInfo : Singleton<GSP__PosInfo, Singleton_MakeMe>{
		void AddVertex(int id, float x, float y, float z);
		void AddEdge(int id_lhs, int id_rhs);
	};
	struct GSP__RoomInfo : Singleton<GSP__RoomInfo, Singleton_MakeMe>{
		void AddPosInfo(int room_id, const GSP__PosInfo& pos_info);
	};
	struct GSP_StagePosPath : Singleton<GSP_StagePosPath, Singleton_MakeMe>{
		void AddStageInfo(int stage_id, const GSP__RoomInfo& room_info);
	};

	struct GSP__PosInfo2d : Singleton<GSP__PosInfo2d, Singleton_MakeMe>{
		void AddVertex(int id, int x, int y);
		void AddEdge(int id_lhs, int id_rhs);
	};
	struct GSP_StagePosPath2d : Singleton<GSP_StagePosPath2d, Singleton_MakeMe>{
		void AddStageInfo(const std::string& stage_id, const GSP__PosInfo2d& pos_info);
	};
	//////////////////////////////////////////////////////////////////////////
	enum enAddPosMode{
		kAPM_ByRole = 0,
		kAPM_ByMouse,
		kAPM_Mouse_Role,	//先鼠标获取坐标，然后稳定角色
	};

public:
	RecordMap(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecordMap();

	// Dialog Data
	enum { IDD = IDD_DLG_RECM };

protected:
	bool OnAddPos(int id, const stRC_Vector3& pos) override;
	bool OnAddEdge(int id_lhs, int id_rhs) override;
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL PreTranslateMessage(MSG* pMsg) override;
	BOOL OnInitDialog() override;
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnSelchangeAddPosMode();
	afx_msg void OnBnClickedBtImport();
	afx_msg void OnBnClickedBtExport();
	afx_msg void OnBnClickedBtSetZoomRate();
	afx_msg void OnBnClickedBtSelectDll();
	afx_msg void OnBnClickedBtTimerUpdate();
	afx_msg void OnBnClickedBtClearData();
	afx_msg void OnBtSetRelativePos();
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnAccelRecord();
	afx_msg void OnAccelAddPos();
	afx_msg void OnAddSelectPos();
	afx_msg void OnAddSelectEdge();
	afx_msg void OnEraseSelectEdge();
	afx_msg void OnEraseSelectPosInfo();
	afx_msg void ClearSelectPos();
	//////////////////////////////////////////////////////////////////////////
	DECLARE_MESSAGE_MAP()

private:
	std::string GetRoomId() const;
	std::string GetMapId() const;
	static void CALLBACK TimerUpdate(_In_  HWND hwnd, _In_  UINT uMsg, _In_  UINT_PTR idEvent, _In_  DWORD dwTime);
	static void CALLBACK TimerRecording(_In_  HWND hwnd, _In_  UINT uMsg, _In_  UINT_PTR idEvent, _In_  DWORD dwTime);

public:
	RECM_StageMgr		stage_mgr_;

private:
	HACCEL		h_accel_;
	int			last_hover_pos_id_;
	std::ofstream*		ofstm_;
	CString		dll_file_full_name_;
	bool		timer_update_;
	UINT		timer_update_elapse_;
	UINT		timer_record_elapse_;
	bool		timer_record_;
	enAddPosMode	add_pos_mode_;

private:
	stRC_Config			config_info_;
	DllGetRolePosPtr	fn_get_role_pos_;
	DllGetMapId			fn_get_map_id_;
	DllGetNumT			fn_get_room_id_;
	stVec3Float			role_pos_;
	std::string			map_id_;
	int					room_id_;

public:
	CEdit		edit_map_id_;
	CEdit		edit_room_id_;
	CEdit		edit_last_oper_;
	CButton		bt_timer_update_;
	CComboBox	com_add_pos_mode_;
};
