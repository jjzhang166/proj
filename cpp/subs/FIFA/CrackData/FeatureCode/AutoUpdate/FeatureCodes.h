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
	void ScanCode_PT_Update( Cmn_FC_OFstream& ofs );
	void ScanCode_PT_UpdateAssert( Cmn_FC_OFstream& ofs );
	void ScanCode_DataAnalyseUpdate( Cmn_FC_OFstream& ofs );

private:
	Cmn_FeatureCodeScan_Direct	scan_exe_;
	Cmn_FeatureCodeScan_Direct	scan_fifazfbase_;
};
