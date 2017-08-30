#pragma once
/*
@author zhp
@date 2016/6/19 15:52
@purpose 录制地图相关组件
RECM->record map
*/
#include <afxwin.h>
#include <vector>
#include "../DependSelfLib.h"
#include <Common/UsefulClass.h>
#include <CommonX/CmnX_Math.h>

struct stRC_Vector3 : stVec3Float{
	stRC_Vector3();
	stRC_Vector3(const stVec3Float& rhs) : stVec3Float(rhs){}
	stRC_Vector3(float the_x, float the_y, float the_z);

	friend bool operator == (const stRC_Vector3& lhs, const stRC_Vector3& rhs);
	friend std::ostream& operator << (std::ostream& ostm, const stRC_Vector3& v);
};

struct stRC_Config{
	stRC_Config(){
		is_vector_2d_ = true;
		has_room_id_ = false;
		is_float_ = false;
	}

	bool	is_vector_2d_;
	bool	has_room_id_;
	bool	is_float_;
};

enum class enRECM_PosState{
	kNormal,
	kSelected,
	kMouseHover,
};

struct RECM_PosBase{
	stRC_Vector3		pos_;
	enRECM_PosState		old_state_;
	enRECM_PosState		state_;
	int					id_;
};

class RECM_StageSink{
public:
	//virtual void MakePaintMsg() = 0;
	virtual bool OnAddPos(int id, const stRC_Vector3& pos) = 0;
	virtual bool OnAddEdge(int id_lhs, int id_rhs) = 0;
};

class RECM_StageMgr{
public:
	typedef std::vector<RECM_PosBase> PosInfo;
	struct EdgeT{
		int	first;
		int	second;

		EdgeT(int lhs, int rhs);
		friend bool operator == (const EdgeT& lhs, const EdgeT& rhs);
	};
	typedef NameList<EdgeT, std::vector<EdgeT> > EdgeInfo;
	typedef NameList<int, std::vector<int> > SelectInfo;

public:
	RECM_StageMgr();
	bool Init(CDC& wnd_dc, const CRect& rect, RECM_StageSink* sink);
	bool Paint(CDC& wnd_dc);

public:
	bool AddPos(const stRC_Vector3& pos);
	bool AddPos(int pos_id, const stRC_Vector3& pos);
	bool ErasePos(const stRC_Vector3& pos);
	bool ErasePos(int pos_id);
	bool AddEdge(int id_lhs, int id_rhs);
	bool AddEdge(const stRC_Vector3& pos_lhs, const stRC_Vector3& pos_rhs);
	bool EraseEdge(int id_lhs, int id_rhs);
	bool EraseEdge(const stRC_Vector3& pos_lhs, const stRC_Vector3& pos_rhs);
	int EraseEdges(int pos_id);

public:
	bool AddSelectPos(int pos_id);
	int AddSelectPosEdges();
	int EraseSelectPosEdges();
	int	EraseSelectPosInfo();
	int ClearSelectPos();	

public:
	bool DoSaveData() const;

public:
	RECM_PosBase* PosInfoFromMouse(int x, int y);
	stRC_Vector3 GetPosFromMouse(int x, int y);
	//bool SetPosState(int pos_id, enRECM_PosState state);
	float GetMostPosX(bool is_max) const;
	float GetMostPosY(bool is_max) const;
	void SetRolePos(const stRC_Vector3& pos);
	void SetZoomRate(float zoom_rate);
	float& GetZoomRate();
	CPoint& GetRelativePos();
	void ClearAll();
	RECM_PosBase* FindByPosId(int pos_id);
	int Pos2Id(const stRC_Vector3& pos) const;
	const CRect& GetRect() const;

private:
	int GenRandomId() const;
	bool HasThePosId(int pos_id) const;
	
	RECM_PosBase* FindNearestPos(float x, float y, const float* max_distance);

private:
	CDC			dc_;
	CBitmap		bit_map_;
	CRect		rect_;
	CPen		pen_normal_;
	CPen		pen_selected_;
	CPen		pen_hover_;
	CPen		pen_line_;
	CPen		pen_role_;
	CBrush		brush_normal_;
	CBrush		brush_selected_;
	CBrush		brush_hover_;
	CBrush		brush_role_;

private:
	RECM_StageSink*		sink_;

private:
	PosInfo		pos_info_;
	EdgeInfo	edge_info_;
	SelectInfo	select_info_;
	stRC_Vector3	role_pos_;
	float		zoom_rate_;
	CPoint		relative_pos_;
};