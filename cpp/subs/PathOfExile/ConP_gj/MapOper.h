#pragma once
/*
@author zhp
@date 2017/7/20 14:31
@purpose for map operate
*/
#include <mfc/MfcMap.h>
#include "resource.h"
#include <Data/public/CG_Public.h>

class MyAcOper;
class DlgMapOper;

class MyMfcMap : public MfcMap{
public:
	MyMfcMap(DlgMapOper* dlg_map_oper);
	~MyMfcMap();
	void SetDlgMapOper(DlgMapOper* dlg_map_oper);
	void SetMapData(const CG_MapBitset& bset, const stSizeInt& map_size, const std::string& map_name);
	void SetPlayerPos(const stVec2Int& player_pos);

protected:
	stMfcMapData GetMapData() const override;
	void OnDbClick(const stVec2Int& map_pos, const stVec2Int& wnd_pos) override;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

private:
	DlgMapOper*					dlg_map_oper_;
	stMfcMapData::MapDataT		map_data_;
	stSizeInt					map_size_;
	std::string					map_name_;
	UINT_PTR					timer_update_pos_;
	DECLARE_MESSAGE_MAP()
};

class DlgMapOper : public CDialog{
public:
	DlgMapOper(UINT nIDTemplate, MyAcOper* pParentWnd);
	DlgMapOper(MyAcOper* pParentWnd);
	MyAcOper* GetAcOper() const{ return ac_oper_; }
	MyMfcMap* GetMfcMap();
	void SetMapName(const std::string& map_name);
	template<typename Fn>
	void PostToSessionOper(const Fn& fn){
		auto ac_oper = ac_oper_;
		if (!ac_oper)
		{
			assert(false);
			return;
		}
		auto io = GetMyApp().GetIoThrd();
		if (!io)
		{
			assert(false);
			return;
		}
		io->GetIoService().post([ac_oper, fn](){
			fn(ac_oper);
		});
	}

protected:
	BOOL OnInitDialog() override;
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtUpdateMap();

private:
	MyMfcMap		dlg_map_;
	MyAcOper*		ac_oper_;
	CString			my_wnd_text_;

	DECLARE_MESSAGE_MAP()
};