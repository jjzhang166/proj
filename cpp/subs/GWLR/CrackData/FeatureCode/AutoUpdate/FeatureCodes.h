#pragma once
/*
@author zhp
@date 2016/4/12 19:26
@purpose for feature code
*/
#include <CmnMix/Cmn_FeatureCode.h>

class ScanCodeHelper{
public:
	ScanCodeHelper();

public:
	void ScanCode_Updates_Old( Cmn_FC_OFstream& ofs );
	bool ScanCode_Test();
	void ScanCode_PT_Update( Cmn_FC_OFstream& ofs );
	void ScanCode_PT_UpdateAssert( Cmn_FC_OFstream& ofs );
	void ScanCode_DataAnalyseUpdate( Cmn_FC_OFstream& ofs );

private:
	//hook
	void ScanCode_PT_UpdateHooks(Cmn_FC_OFstream& ofs);
	//patch
	void ScanCode_PT_UpdatePatch(Cmn_FC_OFstream& ofs);
	//µ÷ÓÃ
	void ScanCode_PT_UpdateCall(Cmn_FC_OFstream& ofs);
	//µØÖ·
	void ScanCode_PT_UpdateAddr(Cmn_FC_OFstream& ofs);
	//Æ«ÒÆ
	void ScanCode_PT_UpdateOffset(Cmn_FC_OFstream& ofs);
	//Ðé±íÆ«ÒÆ
	void ScanCode_PT_UpdateVOffset(Cmn_FC_OFstream& ofs);
	//ºê
	void ScanCode_PT_UpdateMacro(Cmn_FC_OFstream& ofs);
	//lpka
	void ScanCode_PT_UpdateLpka(Cmn_FC_OFstream& ofs);
	//lpkb
	void ScanCode_PT_UpdateLpkb(Cmn_FC_OFstream& ofs);

private:
	Cmn_FeatureCodeScan_Direct		scan_CryRenderD3D9_;
	Cmn_FeatureCodeScan_Direct		scan_game_;
	Cmn_FeatureCodeScan_Direct		scan_cry_action_;
	Cmn_FeatureCodeScan_Direct		scan_mho_base_;
	Cmn_FeatureCodeScan_Direct		scan_ter_safe_;
	Cmn_FeatureCodeScan_Direct		scan_tmp_;
	Cmn_FeatureCodeScan_Direct		scan_exe_;
};
