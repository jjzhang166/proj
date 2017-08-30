#pragma once
/*
@author zhp
@date 2014/8/26 2:25
@purpose for village path, move to village or room, GSP->game stage path
*/
#include <deque>
#include <string>
#include <memory>
#include "GMap.h"

class GSP_Stage;
class GSP_Gate{
public:
	const GSP_Stage*	next_stage_;		//进入门后的城镇或副本房间
	std::wstring		wrap_gate_id_;
};

struct GSP_Step{
	const GSP_Stage*			step_stage_;
	const GSP_Gate*				step_gate_;
};

class GSP_Stage{
public:
	typedef std::deque<GSP_Step> StepsT;
	typedef std::shared_ptr<GSP_Stage> StagePtr;

public:
	bool MoveTo( const GSP_Stage* dst_stage, StepsT& steps_out ) const;

private:
	void MoveToNext( StepsT& cur_steps, const GSP_Stage* dst_stage, StepsT& steps_out ) const;

public:
	std::wstring				stage_id_;
	std::vector<GSP_Gate>		wrap_gates_;
};

class GSP_Resolver{
public:
	typedef GSP_Stage::StagePtr StagePtr;
	typedef std::vector<StagePtr> StagesT;

public:
	bool VillageMove( const std::wstring& village_id_src, const std::wstring& village_id_dst, GSP_Stage::StepsT& steps_out );
	const GSP_Stage* FindStage( const std::wstring& stage_id );

private:
	bool FillVillages();

private:
	StagesT			stages_;
};

class GSP_StageEx;
struct GSP_GateEx{
	GSP_StageEx*		next_stage_;
	std::wstring		wrap_gate_id_;
};

struct GSP_StepEx{
	const GSP_StageEx*	step_stage_;
	const GSP_GateEx*	step_gate_;
	bool				is_pass_;
};

class GSP_StageEx{
public:
	typedef std::deque<GSP_StepEx> StepsT;

public:
	GSP_StageEx();
	bool MoveTo( StepsT& steps_out );

private:
	bool MoveToImpl( StepsT& steps_out );
	bool MoveToDst( StepsT& steps_out ) const;
	bool MoveToDstImpl( StepsT& cur_steps, StepsT& min_steps ) const;
	bool GenRecall( const GSP_StageEx& dest_stage, GSP_StepEx& recall_step ) const;

public:
	std::wstring				stage_id_;
	std::vector<GSP_GateEx>		wrap_gates_;
	int							visited_cnt_;
	bool						is_dst_stage_;
};

class GSP_ResolverEx{
public:
	typedef std::vector<GSP_StageEx> StagesT;

public:
	bool StageMove( const GStageTemplate& stage_template, const std::wstring& sector_src, GSP_StageEx::StepsT& steps_out );
	GSP_StageEx* FindStage( const std::wstring& stage_id );

private:
	bool FillStages( const GStageTemplate& stage_template );

private:
	StagesT			stages_;
};