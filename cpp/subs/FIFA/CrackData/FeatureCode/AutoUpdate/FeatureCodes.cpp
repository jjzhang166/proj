#include "stdafx.h"
#include "FeatureCodes.h"
#include "GloFns.h"
#include <CmnMix/Cmn_LogOut.h>
#include <ProtocolX/P_DefinesX.h>

void __stdcall DummySleep(unsigned long){
	assert(false);
}
ScanCodeHelper::ScanCodeHelper()
{
	auto dw_value_tmp = GetModuleFifazfbase();
	assert(dw_value_tmp);
	scan_fifazfbase_.SetModuleBegin((const void*)dw_value_tmp);
	scan_fifazfbase_.SetCodeRange((const char*)dw_value_tmp + 0x1000, (const char*)dw_value_tmp + 0x000B36000);

	scan_exe_.SetCodeRange((void*)0x00400000, (void*)0x7FFFFFFF);
	scan_exe_.SetModuleBegin((const void*)0x00400000);
}

void ScanCodeHelper::ScanCode_DataAnalyseUpdate( Cmn_FC_OFstream& ofs )
{
	LOG_O(Log_trace) << "ScanCode_DataAnalyse开始更新";
	ofs << "#pragma once" << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	ofs << "/**************************************特征码更新结束*******************************************/";
	LOG_O(Log_trace) << "ScanCode_DataAnalyse更新完毕";
}

void ScanCodeHelper::ScanCode_PT_Update( Cmn_FC_OFstream& ofs )
{
	LOG_O(Log_trace) << "ScanCode_PT_Update开始更新";
	ofs << "#pragma once" << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ofs << "/**************************************特征码更新结束*******************************************/";
	LOG_O(Log_trace) << "ScanCode_PT_Update更新完毕";
}

void ScanCodeHelper::ScanCode_PT_UpdateAssert( Cmn_FC_OFstream& ofs )
{
	LOG_O(Log_trace) << "ScanCode_PT_UpdateAssert开始更新";
	ofs << "#pragma once" << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ofs << "/**************************************特征码更新结束*******************************************/";
	LOG_O(Log_trace) << "ScanCode_PT_UpdateAssert更新完毕";
}

void ScanCodeHelper::ScanCode_Updates_Old( Cmn_FC_OFstream& ofs )
{
	LOG_O(Log_trace) << "ScanCode_Updates_Old开始更新";
	ofs << "#pragma once" << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////
	/*++
	游戏基址
	00B4F20B    E8 901E0A00     CALL fifazf.00BF10A0
	00B4F210    8BC8            MOV ECX,EAX
	00B4F212    E8 99FF0900     CALL fifazf.00BEF1B0
	00B4F217    8947 20         MOV DWORD PTR DS:[EDI+0x20],EAX
	00B4F21A    5F              POP EDI
	00B4F21B    5E              POP ESI
	00B4F21C    5D              POP EBP
	00B4F21D    C2 0800         RETN 0x8
	00B4F220    A1 DCF05701     MOV EAX,DWORD PTR DS:[0x157F0DC]
	00B4F225    C3              RETN

	C20800A1????????C3
	E8????????8BC8E8????????8947??5F5E5DC20800A1
	--*/

	// 	DWORD	dwGameBase = ScanImme((HANDLE)-1,"E8????????8B??E8????????89????5F5E5DC2",0x156);
	// 	wsprintf(szPrint,"[FIFA] 游戏基址:0x%p \r\n",dwGameBase);
	// 	OutputDebugString(szPrint);
	// 	wsprintf(szPrint,"[FIFA] const MemOper<int> g_pGameBase(0x%p);",dwGameBase);
	// 	OutputDebugString(szPrint);
	// 	
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_GAME_BASE_ADDR  ";
	// 	LOG << dwGameBase;
	// 	LOG_END;



	/*++
	发包CALL
	00F17760    83EC 08         SUB ESP,0x8
	00F17763    8B4424 1C       MOV EAX,DWORD PTR SS:[ESP+0x1C]
	00F17767    53              PUSH EBX
	00F17768    55              PUSH EBP
	00F17769    56              PUSH ESI
	00F1776A    57              PUSH EDI
	00F1776B    8B7C24 20       MOV EDI,DWORD PTR SS:[ESP+0x20]
	00F1776F    8B9487 34010000 MOV EDX,DWORD PTR DS:[EDI+EAX*4+0x134]
	00F17776    8B4424 1C       MOV EAX,DWORD PTR SS:[ESP+0x1C]
	00F1777A    8B18            MOV EBX,DWORD PTR DS:[EAX]

	zhp 2016/8/9 13:58 防止特征码找不到
	$-10     00439980       6A 0C                          PUSH 0C
	$-E      00439982       6A 01                          PUSH 1
	$-C      00439984       FF15 C42F7801                  CALL DWORD PTR DS:[1782FC4]              ; MSVCR120.calloc
	$-6      0043998A       83C4 08                        ADD ESP, 8
	$-3      0043998D       C3                             RETN
	$-2      0043998E       CC                             INT3
	$-1      0043998F       CC                             INT3
	$ ==>    00439990       83EC 08                        SUB ESP, 8
	$+3      00439993       8B4424 1C                      MOV EAX, DWORD PTR SS:[ESP+1C]
	$+7      00439997       53                             PUSH EBX
	$+8      00439998       55                             PUSH EBP
	$+9      00439999       56                             PUSH ESI
	$+A      0043999A       57                             PUSH EDI
	$+B      0043999B       8B7C24 20                      MOV EDI, DWORD PTR SS:[ESP+20]
	$+F      0043999F       8B9487 34010000                MOV EDX, DWORD PTR DS:[EDI+EAX*4+134]
	$+16     004399A6       8B4424 1C                      MOV EAX, DWORD PTR SS:[ESP+1C]
	$+1A     004399AA       8B18                           MOV EBX, DWORD PTR DS:[EAX]
	$+1C     004399AC       8B40 08                        MOV EAX, DWORD PTR DS:[EAX+8]
	$+1F     004399AF       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+21     004399B1       8BB1 84860000                  MOV ESI, DWORD PTR DS:[ECX+8684]
	$+27     004399B7       895424 14                      MOV DWORD PTR SS:[ESP+14], EDX
	$+2B     004399BB       8B97 38020000                  MOV EDX, DWORD PTR DS:[EDI+238]
	$+31     004399C1       8BE8                           MOV EBP, EAX
	$+33     004399C3       2B6C24 28                      SUB EBP, DWORD PTR SS:[ESP+28]
	$+37     004399C7       F642 40 01                     TEST BYTE PTR DS:[EDX+40], 1
	$+3B     004399CB       894424 10                      MOV DWORD PTR SS:[ESP+10], EAX
	$+3F     004399CF       74 34                          JE SHORT fifazf.00439A05
	$+41     004399D1       C74424 2C 00400000             MOV DWORD PTR SS:[ESP+2C], 4000
	$+49     004399D9       3D 00400000                    CMP EAX, 4000
	$+4E     004399DE       77 04                          JA SHORT fifazf.004399E4
	$+50     004399E0       894424 2C                      MOV DWORD PTR SS:[ESP+2C], EAX
	$+54     004399E4       8B4424 2C                      MOV EAX, DWORD PTR SS:[ESP+2C]
	$+58     004399E8       50                             PUSH EAX
	$+59     004399E9       81C1 65450000                  ADD ECX, 4565
	$+5F     004399EF       53                             PUSH EBX
	$+60     004399F0       51                             PUSH ECX
	$+61     004399F1       E8 50411E00                    CALL fifazf.0061DB46                     ; JMP 到 MSVCR120.memcpy
	$+66     004399F6       8B1F                           MOV EBX, DWORD PTR DS:[EDI]
	$+68     004399F8       8B4424 38                      MOV EAX, DWORD PTR SS:[ESP+38]
	$+6C     004399FC       83C4 0C                        ADD ESP, 0C
	$+6F     004399FF       81C3 65450000                  ADD EBX, 4565
	$+75     00439A05       8B5424 14                      MOV EDX, DWORD PTR SS:[ESP+14]
	$+79     00439A09       8D4C24 20                      LEA ECX, DWORD PTR SS:[ESP+20]
	$+7D     00439A0D       51                             PUSH ECX
	$+7E     00439A0E       50                             PUSH EAX
	$+7F     00439A0F       53                             PUSH EBX
	$+80     00439A10       52                             PUSH EDX
	$+81     00439A11       57                             PUSH EDI
	$+82     00439A12       E8 C96BFFFF                    CALL fifazf.004305E0
	$+87     00439A17       83C4 14                        ADD ESP, 14
	$+8A     00439A1A       894424 28                      MOV DWORD PTR SS:[ESP+28], EAX
	$+8E     00439A1E       85C0                           TEST EAX, EAX
	$+90     00439A20       0F85 F7000000                  JNZ fifazf.00439B1D
	$+96     00439A26       8B4424 20                      MOV EAX, DWORD PTR SS:[ESP+20]
	$+9A     00439A2A       3BC5                           CMP EAX, EBP
	$+9C     00439A2C       76 08                          JBE SHORT fifazf.00439A36
	$+9E     00439A2E       8BD5                           MOV EDX, EBP
	$+A0     00439A30       895424 2C                      MOV DWORD PTR SS:[ESP+2C], EDX
	$+A4     00439A34       EB 06                          JMP SHORT fifazf.00439A3C
	$+A6     00439A36       894424 2C                      MOV DWORD PTR SS:[ESP+2C], EAX
	$+AA     00439A3A       8BD0                           MOV EDX, EAX
	$+AC     00439A3C       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+AE     00439A3E       8BE8                           MOV EBP, EAX
	$+B0     00439A40       2BEA                           SUB EBP, EDX
	$+B2     00439A42       83B9 E4020000 00               CMP DWORD PTR DS:[ECX+2E4], 0
	$+B9     00439A49       74 2C                          JE SHORT fifazf.00439A77
	$+BB     00439A4B       57                             PUSH EDI
	$+BC     00439A4C       52                             PUSH EDX
	$+BD     00439A4D       53                             PUSH EBX
	$+BE     00439A4E       6A 02                          PUSH 2
	$+C0     00439A50       51                             PUSH ECX
	$+C1     00439A51       E8 AA6DFFFF                    CALL fifazf.00430800
	$+C6     00439A56       83C4 14                        ADD ESP, 14
	$+C9     00439A59       85ED                           TEST EBP, EBP
	$+CB     00439A5B       74 25                          JE SHORT fifazf.00439A82
	$+CD     00439A5D       8B4424 2C                      MOV EAX, DWORD PTR SS:[ESP+2C]
	$+D1     00439A61       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+D3     00439A63       57                             PUSH EDI
	$+D4     00439A64       55                             PUSH EBP
	$+D5     00439A65       03C3                           ADD EAX, EBX
	$+D7     00439A67       50                             PUSH EAX
	$+D8     00439A68       6A 04                          PUSH 4
	$+DA     00439A6A       51                             PUSH ECX
	$+DB     00439A6B       E8 906DFFFF                    CALL fifazf.00430800
	$+E0     00439A70       8B4424 34                      MOV EAX, DWORD PTR SS:[ESP+34]
	$+E4     00439A74       83C4 14                        ADD ESP, 14
	$+E7     00439A77       85ED                           TEST EBP, EBP
	$+E9     00439A79       74 0B                          JE SHORT fifazf.00439A86
	$+EB     00439A7B       016E 28                        ADD DWORD PTR DS:[ESI+28], EBP
	$+EE     00439A7E       8356 2C 00                     ADC DWORD PTR DS:[ESI+2C], 0
	$+F2     00439A82       8B4424 20                      MOV EAX, DWORD PTR SS:[ESP+20]
	$+F6     00439A86       8B4C24 24                      MOV ECX, DWORD PTR SS:[ESP+24]
	$+FA     00439A8A       0101                           ADD DWORD PTR DS:[ECX], EAX
	$+FC     00439A8C       85F6                           TEST ESI, ESI
	$+FE     00439A8E       74 70                          JE SHORT fifazf.00439B00
	$+100    00439A90       8B5424 10                      MOV EDX, DWORD PTR SS:[ESP+10]
	$+104    00439A94       3BC2                           CMP EAX, EDX
	$+106    00439A96       74 5F                          JE SHORT fifazf.00439AF7
	$+108    00439A98       8B5C24 1C                      MOV EBX, DWORD PTR SS:[ESP+1C]
	$+10C    00439A9C       8B0B                           MOV ECX, DWORD PTR DS:[EBX]              ; fifazf.014EFED8
	$+10E    00439A9E       2BD0                           SUB EDX, EAX
	$+110    00439AA0       03C8                           ADD ECX, EAX
	$+112    00439AA2       8B87 9C020000                  MOV EAX, DWORD PTR DS:[EDI+29C]
	$+118    00439AA8       8946 40                        MOV DWORD PTR DS:[ESI+40], EAX
	$+11B    00439AAB       8B87 A0020000                  MOV EAX, DWORD PTR DS:[EDI+2A0]
	$+121    00439AB1       8946 44                        MOV DWORD PTR DS:[ESI+44], EAX
	$+124    00439AB4       8B46 10                        MOV EAX, DWORD PTR DS:[ESI+10]
	$+127    00439AB7       8946 48                        MOV DWORD PTR DS:[ESI+48], EAX
	$+12A    00439ABA       8B46 08                        MOV EAX, DWORD PTR DS:[ESI+8]
	$+12D    00439ABD       8946 50                        MOV DWORD PTR DS:[ESI+50], EAX
	$+130    00439AC0       8B46 0C                        MOV EAX, DWORD PTR DS:[ESI+C]
	$+133    00439AC3       8946 54                        MOV DWORD PTR DS:[ESI+54], EAX
	$+136    00439AC6       C787 9C020000 B0984300         MOV DWORD PTR DS:[EDI+29C], fifazf.00439>
	$+140    00439AD0       89BF A0020000                  MOV DWORD PTR DS:[EDI+2A0], EDI
	$+146    00439AD6       5F                             POP EDI                                  ; ntdll.770DFD91
	$+147    00439AD7       895E 5C                        MOV DWORD PTR DS:[ESI+5C], EBX
	$+14A    00439ADA       894E 10                        MOV DWORD PTR DS:[ESI+10], ECX
	$+14D    00439ADD       8956 08                        MOV DWORD PTR DS:[ESI+8], EDX
	$+150    00439AE0       C746 0C 00000000               MOV DWORD PTR DS:[ESI+C], 0
	$+157    00439AE7       C746 58 01000000               MOV DWORD PTR DS:[ESI+58], 1
	$+15E    00439AEE       5E                             POP ESI                                  ; ntdll.770DFD91
	$+15F    00439AEF       5D                             POP EBP                                  ; ntdll.770DFD91
	$+160    00439AF0       33C0                           XOR EAX, EAX
	$+162    00439AF2       5B                             POP EBX                                  ; ntdll.770DFD91
	$+163    00439AF3       83C4 08                        ADD ESP, 8
	$+166    00439AF6       C3                             RETN
	$+167    00439AF7       C746 58 02000000               MOV DWORD PTR DS:[ESI+58], 2
	$+16E    00439AFE       EB 1D                          JMP SHORT fifazf.00439B1D
	$+170    00439B00       3B4424 10                      CMP EAX, DWORD PTR SS:[ESP+10]
	$+174    00439B04       74 0D                          JE SHORT fifazf.00439B13
	$+176    00439B06       5F                             POP EDI                                  ; ntdll.770DFD91
	$+177    00439B07       5E                             POP ESI                                  ; ntdll.770DFD91
	$+178    00439B08       5D                             POP EBP                                  ; ntdll.770DFD91
	$+179    00439B09       B8 37000000                    MOV EAX, 37
	$+17E    00439B0E       5B                             POP EBX                                  ; ntdll.770DFD91
	$+17F    00439B0F       83C4 08                        ADD ESP, 8
	$+182    00439B12       C3                             RETN
	$+183    00439B13       C787 7C020000 00000000         MOV DWORD PTR DS:[EDI+27C], 0
	$+18D    00439B1D       8B7424 1C                      MOV ESI, DWORD PTR SS:[ESP+1C]
	$+191    00439B21       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$+193    00439B23       85C0                           TEST EAX, EAX
	$+195    00439B25       74 0A                          JE SHORT fifazf.00439B31
	$+197    00439B27       50                             PUSH EAX
	$+198    00439B28       FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$+19E    00439B2E       83C4 04                        ADD ESP, 4
	$+1A1    00439B31       56                             PUSH ESI
	$+1A2    00439B32       FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$+1A8    00439B38       8B4424 2C                      MOV EAX, DWORD PTR SS:[ESP+2C]
	$+1AC    00439B3C       83C4 04                        ADD ESP, 4
	$+1AF    00439B3F       5F                             POP EDI                                  ; ntdll.770DFD91
	$+1B0    00439B40       5E                             POP ESI                                  ; ntdll.770DFD91
	$+1B1    00439B41       5D                             POP EBP                                  ; ntdll.770DFD91
	$+1B2    00439B42       5B                             POP EBX                                  ; 1FBDD018
	$+1B3    00439B43       83C4 08                        ADD ESP, 8
	$+1B6    00439B46       C3                             RETN

	2016/8/25 18:16
	$-30     00D22D50            56                     PUSH ESI
	$-2F     00D22D51            8B7424 08              MOV ESI, DWORD PTR SS:[ESP+8]
	$-2B     00D22D55            85F6                   TEST ESI, ESI
	$-29     00D22D57            74 0B                  JE SHORT fifazf.00D22D64
	$-27     00D22D59            FF36                   PUSH DWORD PTR DS:[ESI]
	$-25     00D22D5B            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$-1F     00D22D61            83C4 04                ADD ESP, 4
	$-1C     00D22D64            56                     PUSH ESI
	$-1B     00D22D65            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$-15     00D22D6B            83C4 04                ADD ESP, 4
	$-12     00D22D6E            5E                     POP ESI                                  ; 42937020
	$-11     00D22D6F            C3                     RETN
	$-10     00D22D70            6A 0C                  PUSH 0C
	$-E      00D22D72            6A 01                  PUSH 1
	$-C      00D22D74            FF15 44E07801          CALL DWORD PTR DS:[178E044]              ; MSVCR120.calloc
	$-6      00D22D7A            83C4 08                ADD ESP, 8
	$-3      00D22D7D            C3                     RETN
	$-2      00D22D7E            CC                     INT3
	$-1      00D22D7F            CC                     INT3
	$ ==>    00D22D80            51                     PUSH ECX
	$+1      00D22D81            8B4424 18              MOV EAX, DWORD PTR SS:[ESP+18]
	$+5      00D22D85            53                     PUSH EBX
	$+6      00D22D86            55                     PUSH EBP
	$+7      00D22D87            56                     PUSH ESI
	$+8      00D22D88            8B7424 18              MOV ESI, DWORD PTR SS:[ESP+18]
	$+C      00D22D8C            57                     PUSH EDI
	$+D      00D22D8D            8B8486 54010000        MOV EAX, DWORD PTR DS:[ESI+EAX*4+154]
	$+14     00D22D94            8B16                   MOV EDX, DWORD PTR DS:[ESI]
	$+16     00D22D96            894424 10              MOV DWORD PTR SS:[ESP+10], EAX
	$+1A     00D22D9A            8B4424 18              MOV EAX, DWORD PTR SS:[ESP+18]
	$+1E     00D22D9E            8BBA 14010000          MOV EDI, DWORD PTR DS:[EDX+114]
	$+24     00D22DA4            8B08                   MOV ECX, DWORD PTR DS:[EAX]
	$+26     00D22DA6            8B58 08                MOV EBX, DWORD PTR DS:[EAX+8]
	$+29     00D22DA9            8BC3                   MOV EAX, EBX
	$+2B     00D22DAB            2B4424 24              SUB EAX, DWORD PTR SS:[ESP+24]
	$+2F     00D22DAF            894424 24              MOV DWORD PTR SS:[ESP+24], EAX
	$+33     00D22DB3            8B86 0C020000          MOV EAX, DWORD PTR DS:[ESI+20C]
	$+39     00D22DB9            894C24 28              MOV DWORD PTR SS:[ESP+28], ECX
	$+3D     00D22DBD            F640 40 01             TEST BYTE PTR DS:[EAX+40], 1
	$+41     00D22DC1            74 34                  JE SHORT fifazf.00D22DF7
	$+43     00D22DC3            83BE 3C010000 14       CMP DWORD PTR DS:[ESI+13C], 14
	$+4A     00D22DCA            74 2B                  JE SHORT fifazf.00D22DF7
	$+4C     00D22DCC            B8 00400000            MOV EAX, 4000
	$+51     00D22DD1            8BEB                   MOV EBP, EBX
	$+53     00D22DD3            3BD8                   CMP EBX, EAX
	$+55     00D22DD5            0F47E8                 CMOVA EBP, EAX
	$+58     00D22DD8            8D82 55450000          LEA EAX, DWORD PTR DS:[EDX+4555]
	$+5E     00D22DDE            55                     PUSH EBP
	$+5F     00D22DDF            51                     PUSH ECX
	$+60     00D22DE0            50                     PUSH EAX
	$+61     00D22DE1            E8 F0B78CFF            CALL fifazf.005EE5D6                     ; JMP 到 MSVCR120.memcpy
	$+66     00D22DE6            8B0E                   MOV ECX, DWORD PTR DS:[ESI]
	$+68     00D22DE8            83C4 0C                ADD ESP, 0C
	$+6B     00D22DEB            81C1 55450000          ADD ECX, 4555
	$+71     00D22DF1            894C24 28              MOV DWORD PTR SS:[ESP+28], ECX
	$+75     00D22DF5            EB 02                  JMP SHORT fifazf.00D22DF9
	$+77     00D22DF7            8BEB                   MOV EBP, EBX
	$+79     00D22DF9            8D4424 1C              LEA EAX, DWORD PTR SS:[ESP+1C]
	$+7D     00D22DFD            50                     PUSH EAX
	$+7E     00D22DFE            55                     PUSH EBP
	$+7F     00D22DFF            51                     PUSH ECX
	$+80     00D22E00            FF7424 1C              PUSH DWORD PTR SS:[ESP+1C]
	$+84     00D22E04            56                     PUSH ESI
	$+85     00D22E05            E8 6690FFFF            CALL fifazf.00D1BE70
	$+8A     00D22E0A            83C4 14                ADD ESP, 14
	$+8D     00D22E0D            894424 10              MOV DWORD PTR SS:[ESP+10], EAX
	$+91     00D22E11            85C0                   TEST EAX, EAX
	$+93     00D22E13            0F85 F6000000          JNZ fifazf.00D22F0F
	$+99     00D22E19            8B5424 1C              MOV EDX, DWORD PTR SS:[ESP+1C]
	$+9D     00D22E1D            8BC2                   MOV EAX, EDX
	$+9F     00D22E1F            3B5424 24              CMP EDX, DWORD PTR SS:[ESP+24]
	$+A3     00D22E23            8BEA                   MOV EBP, EDX
	$+A5     00D22E25            8B0E                   MOV ECX, DWORD PTR DS:[ESI]
	$+A7     00D22E27            0F474424 24            CMOVA EAX, DWORD PTR SS:[ESP+24]
	$+AC     00D22E2C            2BE8                   SUB EBP, EAX
	$+AE     00D22E2E            894424 24              MOV DWORD PTR SS:[ESP+24], EAX
	$+B2     00D22E32            80B9 D0020000 00       CMP BYTE PTR DS:[ECX+2D0], 0
	$+B9     00D22E39            74 30                  JE SHORT fifazf.00D22E6B
	$+BB     00D22E3B            56                     PUSH ESI
	$+BC     00D22E3C            50                     PUSH EAX
	$+BD     00D22E3D            FF7424 30              PUSH DWORD PTR SS:[ESP+30]
	$+C1     00D22E41            6A 02                  PUSH 2
	$+C3     00D22E43            51                     PUSH ECX
	$+C4     00D22E44            E8 5789FFFF            CALL fifazf.00D1B7A0
	$+C9     00D22E49            83C4 14                ADD ESP, 14
	$+CC     00D22E4C            85ED                   TEST EBP, EBP
	$+CE     00D22E4E            74 26                  JE SHORT fifazf.00D22E76
	$+D0     00D22E50            8B4424 28              MOV EAX, DWORD PTR SS:[ESP+28]
	$+D4     00D22E54            034424 24              ADD EAX, DWORD PTR SS:[ESP+24]
	$+D8     00D22E58            56                     PUSH ESI
	$+D9     00D22E59            55                     PUSH EBP
	$+DA     00D22E5A            50                     PUSH EAX
	$+DB     00D22E5B            6A 04                  PUSH 4
	$+DD     00D22E5D            FF36                   PUSH DWORD PTR DS:[ESI]
	$+DF     00D22E5F            E8 3C89FFFF            CALL fifazf.00D1B7A0
	$+E4     00D22E64            8B5424 30              MOV EDX, DWORD PTR SS:[ESP+30]
	$+E8     00D22E68            83C4 14                ADD ESP, 14
	$+EB     00D22E6B            85ED                   TEST EBP, EBP
	$+ED     00D22E6D            74 0B                  JE SHORT fifazf.00D22E7A
	$+EF     00D22E6F            016F 28                ADD DWORD PTR DS:[EDI+28], EBP
	$+F2     00D22E72            8357 2C 00             ADC DWORD PTR DS:[EDI+2C], 0
	$+F6     00D22E76            8B5424 1C              MOV EDX, DWORD PTR SS:[ESP+1C]
	$+FA     00D22E7A            8B4424 20              MOV EAX, DWORD PTR SS:[ESP+20]
	$+FE     00D22E7E            0110                   ADD DWORD PTR DS:[EAX], EDX
	$+100    00D22E80            85FF                   TEST EDI, EDI
	$+102    00D22E82            74 73                  JE SHORT fifazf.00D22EF7
	$+104    00D22E84            3BD3                   CMP EDX, EBX
	$+106    00D22E86            74 66                  JE SHORT fifazf.00D22EEE
	$+108    00D22E88            8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$+10A    00D22E8A            2BDA                   SUB EBX, EDX
	$+10C    00D22E8C            8B6C24 18              MOV EBP, DWORD PTR SS:[ESP+18]
	$+110    00D22E90            8B80 68860000          MOV EAX, DWORD PTR DS:[EAX+8668]
	$+116    00D22E96            8B4D 00                MOV ECX, DWORD PTR SS:[EBP]              ; nvd3dum.67616D69
	$+119    00D22E99            8947 40                MOV DWORD PTR DS:[EDI+40], EAX
	$+11C    00D22E9C            03CA                   ADD ECX, EDX
	$+11E    00D22E9E            8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$+120    00D22EA0            8B80 6C860000          MOV EAX, DWORD PTR DS:[EAX+866C]
	$+126    00D22EA6            8947 44                MOV DWORD PTR DS:[EDI+44], EAX
	$+129    00D22EA9            8B47 10                MOV EAX, DWORD PTR DS:[EDI+10]
	$+12C    00D22EAC            8947 48                MOV DWORD PTR DS:[EDI+48], EAX
	$+12F    00D22EAF            8B47 08                MOV EAX, DWORD PTR DS:[EDI+8]
	$+132    00D22EB2            8947 50                MOV DWORD PTR DS:[EDI+50], EAX
	$+135    00D22EB5            8B47 0C                MOV EAX, DWORD PTR DS:[EDI+C]
	$+138    00D22EB8            8947 54                MOV DWORD PTR DS:[EDI+54], EAX
	$+13B    00D22EBB            8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$+13D    00D22EBD            C780 68860000 C04FD200 MOV DWORD PTR DS:[EAX+8668], fifazf.00D2>
	$+147    00D22EC7            8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$+149    00D22EC9            89B0 6C860000          MOV DWORD PTR DS:[EAX+866C], ESI
	$+14F    00D22ECF            33C0                   XOR EAX, EAX
	$+151    00D22ED1            895F 08                MOV DWORD PTR DS:[EDI+8], EBX
	$+154    00D22ED4            896F 5C                MOV DWORD PTR DS:[EDI+5C], EBP
	$+157    00D22ED7            894F 10                MOV DWORD PTR DS:[EDI+10], ECX
	$+15A    00D22EDA            C747 0C 00000000       MOV DWORD PTR DS:[EDI+C], 0
	$+161    00D22EE1            C747 58 01000000       MOV DWORD PTR DS:[EDI+58], 1
	$+168    00D22EE8            5F                     POP EDI
	$+169    00D22EE9            5E                     POP ESI
	$+16A    00D22EEA            5D                     POP EBP
	$+16B    00D22EEB            5B                     POP EBX
	$+16C    00D22EEC            59                     POP ECX
	$+16D    00D22EED            C3                     RETN
	$+16E    00D22EEE            C747 58 02000000       MOV DWORD PTR DS:[EDI+58], 2
	$+175    00D22EF5            EB 18                  JMP SHORT fifazf.00D22F0F
	$+177    00D22EF7            3BD3                   CMP EDX, EBX
	$+179    00D22EF9            74 0B                  JE SHORT fifazf.00D22F06
	$+17B    00D22EFB            5F                     POP EDI
	$+17C    00D22EFC            5E                     POP ESI
	$+17D    00D22EFD            5D                     POP EBP
	$+17E    00D22EFE            B8 37000000            MOV EAX, 37
	$+183    00D22F03            5B                     POP EBX
	$+184    00D22F04            59                     POP ECX
	$+185    00D22F05            C3                     RETN
	$+186    00D22F06            56                     PUSH ESI
	$+187    00D22F07            E8 24380000            CALL fifazf.00D26730
	$+18C    00D22F0C            83C4 04                ADD ESP, 4
	$+18F    00D22F0F            8B6C24 18              MOV EBP, DWORD PTR SS:[ESP+18]
	$+193    00D22F13            FF75 00                PUSH DWORD PTR SS:[EBP]                  ; nvd3dum.67616D69
	$+196    00D22F16            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$+19C    00D22F1C            55                     PUSH EBP
	$+19D    00D22F1D            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$+1A3    00D22F23            8B4424 18              MOV EAX, DWORD PTR SS:[ESP+18]
	$+1A7    00D22F27            83C4 08                ADD ESP, 8
	$+1AA    00D22F2A            5F                     POP EDI
	$+1AB    00D22F2B            5E                     POP ESI
	$+1AC    00D22F2C            5D                     POP EBP
	$+1AD    00D22F2D            5B                     POP EBX
	$+1AE    00D22F2E            59                     POP ECX
	$+1AF    00D22F2F            C3                     RETN
	--*/
	auto addr_tmp = scan_exe_.ScanAddr("518b??????5355568b??????578b????????????8b??89", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL  " << addr_tmp << FC_OFstream_EndlLog;

	/*++

	// HOOK ssl_write 调用的上一层，获取发包时的socket对象地址
	$ ==>    00486BBA        3D FFFFFF7F          CMP EAX,0x7FFFFFFF
	$+5      00486BBF        76 05                JBE SHORT fifazf.00486BC6
	$+7      00486BC1        B8 FFFFFF7F          MOV EAX,0x7FFFFFFF
	$+C      00486BC6        8D0C76               LEA ECX,DWORD PTR DS:[ESI+ESI*2]
	$+F      00486BC9        8B94CF 60010000      MOV EDX,DWORD PTR DS:[EDI+ECX*8+0x160]
	$+16     00486BD0        50                   PUSH EAX
	$+17     00486BD1        8DB4CF 60010000      LEA ESI,DWORD PTR DS:[EDI+ECX*8+0x160]
	$+1E     00486BD8        53                   PUSH EBX
	$+1F     00486BD9        52                   PUSH EDX
	$+20     00486BDA        E8 31820100          CALL fifazf.0049EE10                     ; ssl_write
	$+25     00486BDF        83C4 0C              ADD ESP,0xC
	$+28     00486BE2        85C0                 TEST EAX,EAX
	$+2A     00486BE4        7F 70                JG SHORT fifazf.00486C56
	$+2C     00486BE6        50                   PUSH EAX
	$+2D     00486BE7        8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+2F     00486BE9        50                   PUSH EAX
	$+30     00486BEA        E8 519D0100          CALL fifazf.004A0940


	zhp 2016/8/9/14:01 防止特征码找不到
	$-6A     00444BE0       8B4424 08                      MOV EAX, DWORD PTR SS:[ESP+8]
	$-66     00444BE4       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$-62     00444BE8       8D0440                         LEA EAX, DWORD PTR DS:[EAX+EAX*2]
	$-5F     00444BEB       8B84C1 60010000                MOV EAX, DWORD PTR DS:[ECX+EAX*8+160]
	$-58     00444BF2       85C0                           TEST EAX, EAX
	$-56     00444BF4       74 10                          JE SHORT fifazf.00444C06
	$-54     00444BF6       50                             PUSH EAX
	$-53     00444BF7       E8 74800100                    CALL fifazf.0045CC70
	$-4E     00444BFC       83C4 04                        ADD ESP, 4
	$-4B     00444BFF       F7D8                           NEG EAX
	$-49     00444C01       1BC0                           SBB EAX, EAX
	$-47     00444C03       F7D8                           NEG EAX
	$-45     00444C05       C3                             RETN
	$-44     00444C06       33C0                           XOR EAX, EAX
	$-42     00444C08       C3                             RETN
	$-41     00444C09       CC                             INT3
	$-40     00444C0A       CC                             INT3
	$-3F     00444C0B       CC                             INT3
	$-3E     00444C0C       CC                             INT3
	$-3D     00444C0D       CC                             INT3
	$-3C     00444C0E       CC                             INT3
	$-3B     00444C0F       CC                             INT3
	$-3A     00444C10       83EC 7C                        SUB ESP, 7C
	$-37     00444C13       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$-32     00444C18       33C4                           XOR EAX, ESP
	$-30     00444C1A       894424 78                      MOV DWORD PTR SS:[ESP+78], EAX
	$-2C     00444C1E       53                             PUSH EBX
	$-2B     00444C1F       8B9C24 8C000000                MOV EBX, DWORD PTR SS:[ESP+8C]
	$-24     00444C26       55                             PUSH EBP
	$-23     00444C27       8BAC24 98000000                MOV EBP, DWORD PTR SS:[ESP+98]
	$-1C     00444C2E       56                             PUSH ESI
	$-1B     00444C2F       8BB424 90000000                MOV ESI, DWORD PTR SS:[ESP+90]
	$-14     00444C36       57                             PUSH EDI
	$-13     00444C37       8BBC24 90000000                MOV EDI, DWORD PTR SS:[ESP+90]
	$-C      00444C3E       E8 DD34FEFF                    CALL fifazf.00428120
	$-7      00444C43       8B8424 9C000000                MOV EAX, DWORD PTR SS:[ESP+9C]
	$ ==>    00444C4A       3D FFFFFF7F                    CMP EAX, 7FFFFFFF
	$+5      00444C4F       76 05                          JBE SHORT fifazf.00444C56
	$+7      00444C51       B8 FFFFFF7F                    MOV EAX, 7FFFFFFF
	$+C      00444C56       8D0C76                         LEA ECX, DWORD PTR DS:[ESI+ESI*2]
	$+F      00444C59       8B94CF 60010000                MOV EDX, DWORD PTR DS:[EDI+ECX*8+160]
	$+16     00444C60       50                             PUSH EAX			;UPDATES_FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK == edx
	$+17     00444C61       8DB4CF 60010000                LEA ESI, DWORD PTR DS:[EDI+ECX*8+160]
	$+1E     00444C68       53                             PUSH EBX
	$+1F     00444C69       52                             PUSH EDX
	$+20     00444C6A       E8 B1810100                    CALL fifazf.0045CE20
	$+25     00444C6F       83C4 0C                        ADD ESP, 0C
	$+28     00444C72       85C0                           TEST EAX, EAX
	$+2A     00444C74       7F 70                          JG SHORT fifazf.00444CE6
	$+2C     00444C76       50                             PUSH EAX
	$+2D     00444C77       8B06                           MOV EAX, DWORD PTR DS:[ESI]              ; fifazf.01542FAC
	$+2F     00444C79       50                             PUSH EAX
	$+30     00444C7A       E8 C19C0100                    CALL fifazf.0045E940
	$+35     00444C7F       8D48 FF                        LEA ECX, DWORD PTR DS:[EAX-1]
	$+38     00444C82       83C4 08                        ADD ESP, 8
	$+3B     00444C85       83F9 04                        CMP ECX, 4
	$+3E     00444C88       77 41                          JA SHORT fifazf.00444CCB
	$+40     00444C8A       FF248D FC4C4400                JMP DWORD PTR DS:[ECX*4+444CFC]
	$+47     00444C91       C745 00 51000000               MOV DWORD PTR SS:[EBP], 51
	$+4E     00444C98       EB 49                          JMP SHORT fifazf.00444CE3
	$+50     00444C9A       FF15 889B4E01                  CALL DWORD PTR DS:[14E9B88]              ; WS2_32.WSAGetLastError
	$+56     00444CA0       50                             PUSH EAX
	$+57     00444CA1       68 B4CC6101                    PUSH fifazf.0161CCB4                     ; ASCII "SSL_write() returned SYSCALL, errno = %d"
	$+5C     00444CA6       EB 29                          JMP SHORT fifazf.00444CD1
	$+5E     00444CA8       E8 A336FEFF                    CALL fifazf.00428350
	$+63     00444CAD       8D5424 10                      LEA EDX, DWORD PTR SS:[ESP+10]
	$+67     00444CB1       52                             PUSH EDX
	$+68     00444CB2       50                             PUSH EAX
	$+69     00444CB3       E8 4836FEFF                    CALL fifazf.00428300
	$+6E     00444CB8       50                             PUSH EAX
	$+6F     00444CB9       8B07                           MOV EAX, DWORD PTR DS:[EDI]              ; fifazf.0171DF3C
	$+71     00444CBB       68 9CCC6101                    PUSH fifazf.0161CC9C                     ; ASCII "SSL_write() error: %s"
	$+76     00444CC0       50                             PUSH EAX
	$+77     00444CC1       E8 5ABDFEFF                    CALL fifazf.00430A20
	$+7C     00444CC6       83C4 14                        ADD ESP, 14
	$+7F     00444CC9       EB 11                          JMP SHORT fifazf.00444CDC
	$+81     00444CCB       50                             PUSH EAX
	$+82     00444CCC       68 80CC6101                    PUSH fifazf.0161CC80                     ; ASCII "SSL_write() return error %d"
	$+87     00444CD1       8B0F                           MOV ECX, DWORD PTR DS:[EDI]              ; fifazf.0171DF3C
	$+89     00444CD3       51                             PUSH ECX
	$+8A     00444CD4       E8 47BDFEFF                    CALL fifazf.00430A20
	$+8F     00444CD9       83C4 0C                        ADD ESP, 0C
	$+92     00444CDC       C745 00 37000000               MOV DWORD PTR SS:[EBP], 37
	$+99     00444CE3       83C8 FF                        OR EAX, FFFFFFFF
	$+9C     00444CE6       8B8C24 88000000                MOV ECX, DWORD PTR SS:[ESP+88]           ; fifazf.008E5861
	$+A3     00444CED       5F                             POP EDI
	$+A4     00444CEE       5E                             POP ESI
	$+A5     00444CEF       5D                             POP EBP
	$+A6     00444CF0       5B                             POP EBX
	$+A7     00444CF1       33CC                           XOR ECX, ESP
	$+A9     00444CF3       E8 64891D00                    CALL fifazf.0061D65C
	$+AE     00444CF8       83C4 7C                        ADD ESP, 7C
	$+B1     00444CFB       C3                             RETN
	$+B2     00444CFC       A8 4C                          TEST AL, 4C
	$+B4     00444CFE       44                             INC ESP
	$+B5     00444CFF       0091 4C440091                  ADD BYTE PTR DS:[ECX+9100444C], DL
	$+BB     00444D05       4C                             DEC ESP
	$+BC     00444D06       44                             INC ESP
	$+BD     00444D07       00CB                           ADD BL, CL
	$+BF     00444D09       4C                             DEC ESP
	$+C0     00444D0A       44                             INC ESP
	$+C1     00444D0B       009A 4C440081                  ADD BYTE PTR DS:[EDX+8100444C], BL
	$+C7     00444D11       EC                             IN AL, DX                                ; I/O 命令
	$+C8     00444D12       8000 00                        ADD BYTE PTR DS:[EAX], 0
	$+CB     00444D15       00A1 60C37101                  ADD BYTE PTR DS:[ECX+171C360], AH
	$+D1     00444D1B       33C4                           XOR EAX, ESP
	$+D3     00444D1D       894424 7C                      MOV DWORD PTR SS:[ESP+7C], EAX
	$+D7     00444D21       8B8424 94000000                MOV EAX, DWORD PTR SS:[ESP+94]           ; fifazf.00618D75
	$+DE     00444D28       53                             PUSH EBX
	$+DF     00444D29       8B9C24 88000000                MOV EBX, DWORD PTR SS:[ESP+88]           ; fifazf.008E5861
	$+E6     00444D30       55                             PUSH EBP
	$+E7     00444D31       8BAC24 94000000                MOV EBP, DWORD PTR SS:[ESP+94]           ; fifazf.00618D75
	$+EE     00444D38       56                             PUSH ESI
	$+EF     00444D39       8BB424 94000000                MOV ESI, DWORD PTR SS:[ESP+94]           ; fifazf.00618D75
	$+F6     00444D40       57                             PUSH EDI
	$+F7     00444D41       894424 10                      MOV DWORD PTR SS:[ESP+10], EAX
	$+FB     00444D45       E8 D633FEFF                    CALL fifazf.00428120
	$+100    00444D4A       8B8424 A0000000                MOV EAX, DWORD PTR SS:[ESP+A0]
	$+107    00444D51       3D FFFFFF7F                    CMP EAX, 7FFFFFFF
	$+10C    00444D56       76 05                          JBE SHORT fifazf.00444D5D
	$+10E    00444D58       B8 FFFFFF7F                    MOV EAX, 7FFFFFFF
	$+113    00444D5D       8D0C76                         LEA ECX, DWORD PTR DS:[ESI+ESI*2]
	$+116    00444D60       8B94CB 60010000                MOV EDX, DWORD PTR DS:[EBX+ECX*8+160]
	$+11D    00444D67       50                             PUSH EAX
	$+11E    00444D68       8DBCCB 60010000                LEA EDI, DWORD PTR DS:[EBX+ECX*8+160]
	$+125    00444D6F       55                             PUSH EBP
	$+126    00444D70       52                             PUSH EDX
	$+127    00444D71       E8 0A800100                    CALL fifazf.0045CD80
	$+12C    00444D76       8BF0                           MOV ESI, EAX
	$+12E    00444D78       83C4 0C                        ADD ESP, 0C
	$+131    00444D7B       85F6                           TEST ESI, ESI
	$+133    00444D7D       7F 6A                          JG SHORT fifazf.00444DE9
	$+135    00444D7F       8B07                           MOV EAX, DWORD PTR DS:[EDI]              ; fifazf.0171DF3C
	$+137    00444D81       56                             PUSH ESI
	$+138    00444D82       50                             PUSH EAX
	$+139    00444D83       E8 B89B0100                    CALL fifazf.0045E940
	$+13E    00444D88       83C4 08                        ADD ESP, 8
	$+141    00444D8B       83F8 06                        CMP EAX, 6
	$+144    00444D8E       77 1D                          JA SHORT fifazf.00444DAD
	$+146    00444D90       0FB688 104E4400                MOVZX ECX, BYTE PTR DS:[EAX+444E10]
	$+14D    00444D97       FF248D 044E4400                JMP DWORD PTR DS:[ECX*4+444E04]
	$+154    00444D9E       8B5424 10                      MOV EDX, DWORD PTR SS:[ESP+10]
	$+158    00444DA2       C702 51000000                  MOV DWORD PTR DS:[EDX], 51
	$+15E    00444DA8       83C8 FF                        OR EAX, FFFFFFFF
	$+161    00444DAB       EB 3E                          JMP SHORT fifazf.00444DEB
	$+163    00444DAD       E8 9E35FEFF                    CALL fifazf.00428350
	$+168    00444DB2       8BF0                           MOV ESI, EAX
	$+16A    00444DB4       FF15 889B4E01                  CALL DWORD PTR DS:[14E9B88]              ; WS2_32.WSAGetLastError
	$+170    00444DBA       50                             PUSH EAX
	$+171    00444DBB       8D4424 18                      LEA EAX, DWORD PTR SS:[ESP+18]
	$+175    00444DBF       50                             PUSH EAX
	$+176    00444DC0       56                             PUSH ESI
	$+177    00444DC1       E8 3A35FEFF                    CALL fifazf.00428300
	$+17C    00444DC6       8B0B                           MOV ECX, DWORD PTR DS:[EBX]
	$+17E    00444DC8       83C4 08                        ADD ESP, 8
	$+181    00444DCB       50                             PUSH EAX
	$+182    00444DCC       68 FCC16101                    PUSH fifazf.0161C1FC                     ; ASCII "SSL read: %s, errno %d"
	$+187    00444DD1       51                             PUSH ECX
	$+188    00444DD2       E8 49BCFEFF                    CALL fifazf.00430A20
	$+18D    00444DD7       8B5424 20                      MOV EDX, DWORD PTR SS:[ESP+20]
	$+191    00444DDB       83C4 10                        ADD ESP, 10
	$+194    00444DDE       C702 38000000                  MOV DWORD PTR DS:[EDX], 38
	$+19A    00444DE4       83C8 FF                        OR EAX, FFFFFFFF
	$+19D    00444DE7       EB 02                          JMP SHORT fifazf.00444DEB
	$+19F    00444DE9       8BC6                           MOV EAX, ESI
	$+1A1    00444DEB       8B8C24 8C000000                MOV ECX, DWORD PTR SS:[ESP+8C]
	$+1A8    00444DF2       5F                             POP EDI
	$+1A9    00444DF3       5E                             POP ESI
	$+1AA    00444DF4       5D                             POP EBP
	$+1AB    00444DF5       5B                             POP EBX
	$+1AC    00444DF6       33CC                           XOR ECX, ESP
	$+1AE    00444DF8       E8 5F881D00                    CALL fifazf.0061D65C
	$+1B3    00444DFD       81C4 80000000                  ADD ESP, 80
	$+1B9    00444E03       C3                             RETN
	$+1BA    00444E04     - E9 4D44009E                    JMP 9E449256
	$+1BF    00444E09       4D                             DEC EBP
	$+1C0    00444E0A       44                             INC ESP
	$+1C1    00444E0B       00AD 4D440000                  ADD BYTE PTR SS:[EBP+444D], CH
	$+1C7    00444E11       0201                           ADD AL, BYTE PTR DS:[ECX]
	$+1C9    00444E13       0102                           ADD DWORD PTR DS:[EDX], EAX
	$+1CB    00444E15       0200                           ADD AL, BYTE PTR DS:[EAX]

	2016/8/25 18:29
	$-57     00D2A320            83EC 7C                SUB ESP, 7C
	$-54     00D2A323            A1 60737201            MOV EAX, DWORD PTR DS:[1727360]
	$-4F     00D2A328            33C4                   XOR EAX, ESP
	$-4D     00D2A32A            894424 78              MOV DWORD PTR SS:[ESP+78], EAX
	$-49     00D2A32E            53                     PUSH EBX
	$-48     00D2A32F            8B9C24 94000000        MOV EBX, DWORD PTR SS:[ESP+94]
	$-41     00D2A336            55                     PUSH EBP
	$-40     00D2A337            8BAC24 88000000        MOV EBP, DWORD PTR SS:[ESP+88]
	$-39     00D2A33E            56                     PUSH ESI
	$-38     00D2A33F            8BB424 90000000        MOV ESI, DWORD PTR SS:[ESP+90]
	$-31     00D2A346            57                     PUSH EDI
	$-30     00D2A347            8BBC24 98000000        MOV EDI, DWORD PTR SS:[ESP+98]
	$-29     00D2A34E            E8 1DAB6FFF            CALL fifazf.00424E70
	$-24     00D2A353            8D0476                 LEA EAX, DWORD PTR DS:[ESI+ESI*2]
	$-21     00D2A356            B9 FFFFFF7F            MOV ECX, 7FFFFFFF
	$-1C     00D2A35B            8D34C5 00000000        LEA ESI, DWORD PTR DS:[EAX*8]
	$-15     00D2A362            8B8424 9C000000        MOV EAX, DWORD PTR SS:[ESP+9C]
	$-E      00D2A369            3BC1                   CMP EAX, ECX
	$-C      00D2A36B            0F47C1                 CMOVA EAX, ECX
	$-9      00D2A36E            50                     PUSH EAX
	$-8      00D2A36F            57                     PUSH EDI
	$-7      00D2A370            FFB42E 88010000        PUSH DWORD PTR DS:[ESI+EBP+188]          ; fifazf.005EE5AD
	$ ==>    00D2A377            E8 94A370FF            CALL fifazf.00434710			;UPDATES_FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK == [esp]
	$+5      00D2A37C            83C4 0C                ADD ESP, 0C
	$+8      00D2A37F            85C0                   TEST EAX, EAX
	$+A      00D2A381            0F8F DC000000          JG fifazf.00D2A463
	$+10     00D2A387            50                     PUSH EAX
	$+11     00D2A388            FFB42E 88010000        PUSH DWORD PTR DS:[ESI+EBP+188]          ; fifazf.005EE5AD
	$+18     00D2A38F            E8 9CBE70FF            CALL fifazf.00436230
	$+1D     00D2A394            83C4 08                ADD ESP, 8
	$+20     00D2A397            8D48 FF                LEA ECX, DWORD PTR DS:[EAX-1]
	$+23     00D2A39A            83F9 04                CMP ECX, 4
	$+26     00D2A39D            0F87 93000000          JA fifazf.00D2A436
	$+2C     00D2A3A3            FF248D 80A4D200        JMP DWORD PTR DS:[ECX*4+D2A480]          ; fifazf.00D2A3F9
	$+33     00D2A3AA            5F                     POP EDI                                  ; fifazf.009DC2A6
	$+34     00D2A3AB            5E                     POP ESI                                  ; fifazf.009DC2A6
	$+35     00D2A3AC            5D                     POP EBP                                  ; fifazf.009DC2A6
	$+36     00D2A3AD            C703 51000000          MOV DWORD PTR DS:[EBX], 51
	$+3C     00D2A3B3            83C8 FF                OR EAX, FFFFFFFF
	$+3F     00D2A3B6            5B                     POP EBX                                  ; fifazf.009DC2A6
	$+40     00D2A3B7            8B4C24 78              MOV ECX, DWORD PTR SS:[ESP+78]
	$+44     00D2A3BB            33CC                   XOR ECX, ESP
	$+46     00D2A3BD            E8 223D8CFF            CALL fifazf.005EE0E4
	$+4B     00D2A3C2            83C4 7C                ADD ESP, 7C
	$+4E     00D2A3C5            C3                     RETN
	$+4F     00D2A3C6            FF15 6C3B4F01          CALL DWORD PTR DS:[14F3B6C]              ; WS2_32.WSAGetLastError
	$+55     00D2A3CC            50                     PUSH EAX
	$+56     00D2A3CD            68 D87A6201            PUSH fifazf.01627AD8                     ; ASCII "SSL_write() returned SYSCALL, errno = %d"
	$+5B     00D2A3D2            FF75 00                PUSH DWORD PTR SS:[EBP]
	$+5E     00D2A3D5            E8 B614FFFF            CALL fifazf.00D1B890
	$+63     00D2A3DA            83C4 0C                ADD ESP, 0C
	$+66     00D2A3DD            C703 37000000          MOV DWORD PTR DS:[EBX], 37
	$+6C     00D2A3E3            83C8 FF                OR EAX, FFFFFFFF
	$+6F     00D2A3E6            5F                     POP EDI                                  ; fifazf.009DC2A6
	$+70     00D2A3E7            5E                     POP ESI                                  ; fifazf.009DC2A6
	$+71     00D2A3E8            5D                     POP EBP                                  ; fifazf.009DC2A6
	$+72     00D2A3E9            5B                     POP EBX                                  ; fifazf.009DC2A6
	$+73     00D2A3EA            8B4C24 78              MOV ECX, DWORD PTR SS:[ESP+78]
	$+77     00D2A3EE            33CC                   XOR ECX, ESP
	$+79     00D2A3F0            E8 EF3C8CFF            CALL fifazf.005EE0E4
	$+7E     00D2A3F5            83C4 7C                ADD ESP, 7C
	$+81     00D2A3F8            C3                     RETN
	$+82     00D2A3F9            E8 A2AC6FFF            CALL fifazf.004250A0
	$+87     00D2A3FE            8D4C24 10              LEA ECX, DWORD PTR SS:[ESP+10]
	$+8B     00D2A402            51                     PUSH ECX
	$+8C     00D2A403            50                     PUSH EAX
	$+8D     00D2A404            E8 47AC6FFF            CALL fifazf.00425050
	$+92     00D2A409            50                     PUSH EAX
	$+93     00D2A40A            68 047B6201            PUSH fifazf.01627B04                     ; ASCII "SSL_write() error: %s"
	$+98     00D2A40F            FF75 00                PUSH DWORD PTR SS:[EBP]
	$+9B     00D2A412            E8 7914FFFF            CALL fifazf.00D1B890
	$+A0     00D2A417            83C4 14                ADD ESP, 14
	$+A3     00D2A41A            C703 37000000          MOV DWORD PTR DS:[EBX], 37
	$+A9     00D2A420            83C8 FF                OR EAX, FFFFFFFF
	$+AC     00D2A423            5F                     POP EDI                                  ; fifazf.009DC2A6
	$+AD     00D2A424            5E                     POP ESI                                  ; fifazf.009DC2A6
	$+AE     00D2A425            5D                     POP EBP                                  ; fifazf.009DC2A6
	$+AF     00D2A426            5B                     POP EBX                                  ; fifazf.009DC2A6
	$+B0     00D2A427            8B4C24 78              MOV ECX, DWORD PTR SS:[ESP+78]
	$+B4     00D2A42B            33CC                   XOR ECX, ESP
	$+B6     00D2A42D            E8 B23C8CFF            CALL fifazf.005EE0E4
	$+BB     00D2A432            83C4 7C                ADD ESP, 7C
	$+BE     00D2A435            C3                     RETN
	$+BF     00D2A436            50                     PUSH EAX
	$+C0     00D2A437            68 1C7B6201            PUSH fifazf.01627B1C                     ; ASCII "SSL_write() return error %d"
	$+C5     00D2A43C            FF75 00                PUSH DWORD PTR SS:[EBP]
	$+C8     00D2A43F            E8 4C14FFFF            CALL fifazf.00D1B890
	$+CD     00D2A444            83C4 0C                ADD ESP, 0C
	$+D0     00D2A447            C703 37000000          MOV DWORD PTR DS:[EBX], 37
	$+D6     00D2A44D            83C8 FF                OR EAX, FFFFFFFF
	$+D9     00D2A450            5F                     POP EDI                                  ; fifazf.009DC2A6
	$+DA     00D2A451            5E                     POP ESI                                  ; fifazf.009DC2A6
	$+DB     00D2A452            5D                     POP EBP                                  ; fifazf.009DC2A6
	$+DC     00D2A453            5B                     POP EBX                                  ; fifazf.009DC2A6
	$+DD     00D2A454            8B4C24 78              MOV ECX, DWORD PTR SS:[ESP+78]
	$+E1     00D2A458            33CC                   XOR ECX, ESP
	$+E3     00D2A45A            E8 853C8CFF            CALL fifazf.005EE0E4
	$+E8     00D2A45F            83C4 7C                ADD ESP, 7C
	$+EB     00D2A462            C3                     RETN
	$+EC     00D2A463            8B8C24 88000000        MOV ECX, DWORD PTR SS:[ESP+88]
	$+F3     00D2A46A            5F                     POP EDI                                  ; fifazf.009DC2A6
	$+F4     00D2A46B            5E                     POP ESI                                  ; fifazf.009DC2A6
	$+F5     00D2A46C            5D                     POP EBP                                  ; fifazf.009DC2A6
	$+F6     00D2A46D            C703 00000000          MOV DWORD PTR DS:[EBX], 0
	$+FC     00D2A473            5B                     POP EBX                                  ; fifazf.009DC2A6
	$+FD     00D2A474            33CC                   XOR ECX, ESP
	$+FF     00D2A476            E8 693C8CFF            CALL fifazf.005EE0E4
	$+104    00D2A47B            83C4 7C                ADD ESP, 7C
	$+107    00D2A47E            C3                     RETN
	$+108    00D2A47F            90                     NOP
	$+109    00D2A480            F9                     STC
	$+10A    00D2A481            A3 D200AAA3            MOV DWORD PTR DS:[A3AA00D2], EAX
	$+10F    00D2A486            D200                   ROL BYTE PTR DS:[EAX], CL
	$+111    00D2A488            AA                     STOS BYTE PTR ES:[EDI]
	$+112    00D2A489            A3 D20036A4            MOV DWORD PTR DS:[A43600D2], EAX
	$+117    00D2A48E            D200                   ROL BYTE PTR DS:[EAX], CL
	$+119    00D2A490            C6                     ???                                      ; 未知命令
	$+11A    00D2A491            A3 D200CCCC            MOV DWORD PTR DS:[CCCC00D2], EAX
	--*/
	addr_tmp = scan_exe_.ScanAddr("e8????????83????85??0f??????????50ff????????????e8", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	SSL_write
	00F45F90    8B4424 04       MOV EAX,DWORD PTR SS:[ESP+0x4]
	00F45F94    8378 20 00      CMP DWORD PTR DS:[EAX+0x20],0x0
	00F45F98    75 22           JNZ SHORT fifazf.00F45FBC
	00F45F9A    68 DE030000     PUSH 0x3DE
	00F45F9F    68 7C433501     PUSH fifazf.0135437C                     ; ASCII ".\\ssl\\ssl_lib.c"
	00F45FA4    68 14010000     PUSH 0x114
	00F45FA9    68 D0000000     PUSH 0xD0
	00F45FAE    6A 14           PUSH 0x14
	00F45FB0    E8 3BA4FCFF     CALL fifazf.00F103F0
	
	zhp 2016/8/9 14:02 防止特征码找不到
	$ ==>    0045CE20       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$+4      0045CE24       8378 20 00                     CMP DWORD PTR DS:[EAX+20], 0
	$+8      0045CE28       75 22                          JNZ SHORT fifazf.0045CE4C
	$+A      0045CE2A       68 DE030000                    PUSH 3DE
	$+F      0045CE2F       68 00F76201                    PUSH fifazf.0162F700                     ; ASCII ".\ssl\ssl_lib.c"
	$+14     0045CE34       68 14010000                    PUSH 114
	$+19     0045CE39       68 D0000000                    PUSH 0D0
	$+1E     0045CE3E       6A 14                          PUSH 14
	$+20     0045CE40       E8 EBB1FCFF                    CALL fifazf.00428030
	$+25     0045CE45       83C4 14                        ADD ESP, 14
	$+28     0045CE48       83C8 FF                        OR EAX, FFFFFFFF
	$+2B     0045CE4B       C3                             RETN
	$+2C     0045CE4C       B9 01000000                    MOV ECX, 1
	$+31     0045CE51       8448 30                        TEST BYTE PTR DS:[EAX+30], CL
	$+34     0045CE54       74 25                          JE SHORT fifazf.0045CE7B
	$+36     0045CE56       68 E5030000                    PUSH 3E5
	$+3B     0045CE5B       68 00F76201                    PUSH fifazf.0162F700                     ; ASCII ".\ssl\ssl_lib.c"
	$+40     0045CE60       68 CF000000                    PUSH 0CF
	$+45     0045CE65       68 D0000000                    PUSH 0D0
	$+4A     0045CE6A       6A 14                          PUSH 14
	$+4C     0045CE6C       8948 18                        MOV DWORD PTR DS:[EAX+18], ECX
	$+4F     0045CE6F       E8 BCB1FCFF                    CALL fifazf.00428030
	$+54     0045CE74       83C4 14                        ADD ESP, 14
	$+57     0045CE77       83C8 FF                        OR EAX, FFFFFFFF
	$+5A     0045CE7A       C3                             RETN
	$+5B     0045CE7B       8B48 08                        MOV ECX, DWORD PTR DS:[EAX+8]
	$+5E     0045CE7E       894424 04                      MOV DWORD PTR SS:[ESP+4], EAX
	$+62     0045CE82       8B41 20                        MOV EAX, DWORD PTR DS:[ECX+20]
	$+65     0045CE85     - FFE0                           JMP EAX
	$+67     0045CE87       CC                             INT3
	$+68     0045CE88       CC                             INT3
	$+69     0045CE89       CC                             INT3
	$+6A     0045CE8A       CC                             INT3
	$+6B     0045CE8B       CC                             INT3
	$+6C     0045CE8C       CC                             INT3
	$+6D     0045CE8D       CC                             INT3
	$+6E     0045CE8E       CC                             INT3
	$+6F     0045CE8F       CC                             INT3
	$+70     0045CE90       8B4C24 08                      MOV ECX, DWORD PTR SS:[ESP+8]
	$+74     0045CE94       8D41 F0                        LEA EAX, DWORD PTR DS:[ECX-10]
	$+77     0045CE97       83F8 3E                        CMP EAX, 3E
	$+7A     0045CE9A       0F87 76010000                  JA fifazf.0045D016
	$+80     0045CEA0       0FB680 A0D04500                MOVZX EAX, BYTE PTR DS:[EAX+45D0A0]
	$+87     0045CEA7     - FF2485 34D04500                JMP DWORD PTR DS:[EAX*4+45D034]
	$+8E     0045CEAE       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+92     0045CEB2       8B81 B4000000                  MOV EAX, DWORD PTR DS:[ECX+B4]
	$+98     0045CEB8       C3                             RETN
	$+99     0045CEB9       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+9D     0045CEBD       8B5424 0C                      MOV EDX, DWORD PTR SS:[ESP+C]
	$+A1     0045CEC1       8B81 B4000000                  MOV EAX, DWORD PTR DS:[ECX+B4]
	$+A7     0045CEC7       8991 B4000000                  MOV DWORD PTR DS:[ECX+B4], EDX
	$+AD     0045CECD       C3                             RETN
	$+AE     0045CECE       8B4424 10                      MOV EAX, DWORD PTR SS:[ESP+10]
	$+B2     0045CED2       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+B6     0045CED6       8981 BC000000                  MOV DWORD PTR DS:[ECX+BC], EAX
	$+BC     0045CEDC       B8 01000000                    MOV EAX, 1
	$+C1     0045CEE1       C3                             RETN
	$+C2     0045CEE2       8B5424 04                      MOV EDX, DWORD PTR SS:[ESP+4]
	$+C6     0045CEE6       8B82 AC000000                  MOV EAX, DWORD PTR DS:[EDX+AC]
	$+CC     0045CEEC       C3                             RETN
	$+CD     0045CEED       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+D1     0045CEF1       8B5424 0C                      MOV EDX, DWORD PTR SS:[ESP+C]
	$+D5     0045CEF5       8B81 AC000000                  MOV EAX, DWORD PTR DS:[ECX+AC]
	$+DB     0045CEFB       8991 AC000000                  MOV DWORD PTR DS:[ECX+AC], EDX
	$+E1     0045CF01       C3                             RETN
	$+E2     0045CF02       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+E6     0045CF06       8B5424 0C                      MOV EDX, DWORD PTR SS:[ESP+C]
	$+EA     0045CF0A       8B41 14                        MOV EAX, DWORD PTR DS:[ECX+14]
	$+ED     0045CF0D       8951 14                        MOV DWORD PTR DS:[ECX+14], EDX
	$+F0     0045CF10       C3                             RETN
	$+F1     0045CF11       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$+F5     0045CF15       8B40 14                        MOV EAX, DWORD PTR DS:[EAX+14]
	$+F8     0045CF18       C3                             RETN
	$+F9     0045CF19       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+FD     0045CF1D       8B5424 0C                      MOV EDX, DWORD PTR SS:[ESP+C]
	$+101    0045CF21       8B41 20                        MOV EAX, DWORD PTR DS:[ECX+20]
	$+104    0045CF24       8951 20                        MOV DWORD PTR DS:[ECX+20], EDX
	$+107    0045CF27       C3                             RETN
	$+108    0045CF28       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$+10C    0045CF2C       8B40 20                        MOV EAX, DWORD PTR DS:[EAX+20]
	$+10F    0045CF2F       C3                             RETN
	$+110    0045CF30       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]
	$+114    0045CF34       8B51 10                        MOV EDX, DWORD PTR DS:[ECX+10]
	$+117    0045CF37       52                             PUSH EDX
	$+118    0045CF38       E8 232CFEFF                    CALL fifazf.0043FB60
	$+11D    0045CF3D       83C4 04                        ADD ESP, 4
	$+120    0045CF40       C3                             RETN
	--*/

	addr_tmp = scan_exe_.ScanAddr("8B4424??8378????75??68DE??000068????????68????????68????????6A??E8", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_SSL_WRITE  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	SSL_read
	00F45EF0    8B4424 04       MOV EAX,DWORD PTR SS:[ESP+0x4]           ; 收包
	00F45EF4    8378 20 00      CMP DWORD PTR DS:[EAX+0x20],0x0
	00F45EF8    75 22           JNZ SHORT fifazf.00F45F1C
	00F45EFA    68 BF030000     PUSH 0x3BF
	00F45EFF    68 7C433501     PUSH fifazf.0135437C                     ; ASCII ".\\ssl\\ssl_lib.c"
	00F45F04    68 14010000     PUSH 0x114
	00F45F09    68 DF000000     PUSH 0xDF
	00F45F0E    6A 14           PUSH 0x14
	00F45F10    E8 DBA4FCFF     CALL fifazf.00F103F0

	zhp 2016/8/9 14:04 防止特征码找不到
	$-10     0045CD70       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$-C      0045CD74       8B48 08                        MOV ECX, DWORD PTR DS:[EAX+8]
	$-9      0045CD77       8B51 60                        MOV EDX, DWORD PTR DS:[ECX+60]           ; fifazf.004D9BA1
	$-6      0045CD7A     - FFE2                           JMP EDX
	$-4      0045CD7C       CC                             INT3
	$-3      0045CD7D       CC                             INT3
	$-2      0045CD7E       CC                             INT3
	$-1      0045CD7F       CC                             INT3
	$ ==>    0045CD80       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$+4      0045CD84       8378 20 00                     CMP DWORD PTR DS:[EAX+20], 0
	$+8      0045CD88       75 22                          JNZ SHORT fifazf.0045CDAC
	$+A      0045CD8A       68 BF030000                    PUSH 3BF
	$+F      0045CD8F       68 00F76201                    PUSH fifazf.0162F700                     ; ASCII ".\ssl\ssl_lib.c"
	$+14     0045CD94       68 14010000                    PUSH 114
	$+19     0045CD99       68 DF000000                    PUSH 0DF
	$+1E     0045CD9E       6A 14                          PUSH 14
	$+20     0045CDA0       E8 8BB2FCFF                    CALL fifazf.00428030
	$+25     0045CDA5       83C4 14                        ADD ESP, 14
	$+28     0045CDA8       83C8 FF                        OR EAX, FFFFFFFF
	$+2B     0045CDAB       C3                             RETN
	$+2C     0045CDAC       F640 30 02                     TEST BYTE PTR DS:[EAX+30], 2
	$+30     0045CDB0       74 0A                          JE SHORT fifazf.0045CDBC
	$+32     0045CDB2       C740 18 01000000               MOV DWORD PTR DS:[EAX+18], 1
	$+39     0045CDB9       33C0                           XOR EAX, EAX
	$+3B     0045CDBB       C3                             RETN
	$+3C     0045CDBC       8B48 08                        MOV ECX, DWORD PTR DS:[EAX+8]
	$+3F     0045CDBF       894424 04                      MOV DWORD PTR SS:[ESP+4], EAX
	$+43     0045CDC3       8B41 18                        MOV EAX, DWORD PTR DS:[ECX+18]
	$+46     0045CDC6     - FFE0                           JMP EAX
	$+48     0045CDC8       CC                             INT3
	$+49     0045CDC9       CC                             INT3
	$+4A     0045CDCA       CC                             INT3
	$+4B     0045CDCB       CC                             INT3
	$+4C     0045CDCC       CC                             INT3
	$+4D     0045CDCD       CC                             INT3
	$+4E     0045CDCE       CC                             INT3
	$+4F     0045CDCF       CC                             INT3
	$+50     0045CDD0       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]
	$+54     0045CDD4       8378 20 00                     CMP DWORD PTR DS:[EAX+20], 0
	$+58     0045CDD8       75 22                          JNZ SHORT fifazf.0045CDFC
	$+5A     0045CDDA       68 CF030000                    PUSH 3CF
	$+5F     0045CDDF       68 00F76201                    PUSH fifazf.0162F700                     ; ASCII ".\ssl\ssl_lib.c"
	$+64     0045CDE4       68 14010000                    PUSH 114
	$+69     0045CDE9       68 0E010000                    PUSH 10E
	$+6E     0045CDEE       6A 14                          PUSH 14
	$+70     0045CDF0       E8 3BB2FCFF                    CALL fifazf.00428030
	$+75     0045CDF5       83C4 14                        ADD ESP, 14
	$+78     0045CDF8       83C8 FF                        OR EAX, FFFFFFFF
	$+7B     0045CDFB       C3                             RETN
	$+7C     0045CDFC       F640 30 02                     TEST BYTE PTR DS:[EAX+30], 2
	$+80     0045CE00       74 03                          JE SHORT fifazf.0045CE05
	$+82     0045CE02       33C0                           XOR EAX, EAX
	$+84     0045CE04       C3                             RETN
	$+85     0045CE05       8B48 08                        MOV ECX, DWORD PTR DS:[EAX+8]
	$+88     0045CE08       894424 04                      MOV DWORD PTR SS:[ESP+4], EAX
	$+8C     0045CE0C       8B41 1C                        MOV EAX, DWORD PTR DS:[ECX+1C]
	$+8F     0045CE0F     - FFE0                           JMP EAX
	--*/
	addr_tmp = scan_exe_.ScanAddr("8B4424??8378????75??68BF??000068????????68????????68????????6A??E8", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_SSL_READ  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	SSL_get_error
	00F47AB0      55              PUSH EBP
	00F47AB1      8B6C24 0C       MOV EBP,DWORD PTR SS:[ESP+0xC]
	00F47AB5      85ED            TEST EBP,EBP
	00F47AB7      7E 04           JLE SHORT fifazf.00F47ABD
	00F47AB9      33C0            XOR EAX,EAX
	00F47ABB      5D              POP EBP
	00F47ABC      C3              RETN
	00F47ABD      E8 BE8DFCFF     CALL fifazf.00F10880
	00F47AC2      85C0            TEST EAX,EAX
	00F47AC4      74 1A           JE SHORT fifazf.00F47AE0

	zhp 2016/8/9 14:05 防止特征码找不到
	$ ==>    0045E940       55                             PUSH EBP
	$+1      0045E941       8B6C24 0C                      MOV EBP, DWORD PTR SS:[ESP+C]
	$+5      0045E945       85ED                           TEST EBP, EBP
	$+7      0045E947       7E 04                          JLE SHORT fifazf.0045E94D
	$+9      0045E949       33C0                           XOR EAX, EAX
	$+B      0045E94B       5D                             POP EBP                                  ; ntdll.770DFD91
	$+C      0045E94C       C3                             RETN
	$+D      0045E94D       E8 6E9BFCFF                    CALL fifazf.004284C0
	$+12     0045E952       85C0                           TEST EAX, EAX
	$+14     0045E954       74 1A                          JE SHORT fifazf.0045E970
	$+16     0045E956       25 000000FF                    AND EAX, FF000000
	$+1B     0045E95B       33C9                           XOR ECX, ECX
	$+1D     0045E95D       3D 00000002                    CMP EAX, fifazf.02000000
	$+22     0045E962       0F94C1                         SETE CL
	$+25     0045E965       5D                             POP EBP                                  ; ntdll.770DFD91
	$+26     0045E966       8D0C8D 01000000                LEA ECX, DWORD PTR DS:[ECX*4+1]
	$+2D     0045E96D       8BC1                           MOV EAX, ECX
	$+2F     0045E96F       C3                             RETN
	$+30     0045E970       56                             PUSH ESI
	$+31     0045E971       8B7424 0C                      MOV ESI, DWORD PTR SS:[ESP+C]
	$+35     0045E975       57                             PUSH EDI
	$+36     0045E976       85ED                           TEST EBP, EBP
	$+38     0045E978       0F8D B2000000                  JGE fifazf.0045EA30
	$+3E     0045E97E       837E 18 03                     CMP DWORD PTR DS:[ESI+18], 3
	$+42     0045E982       75 30                          JNZ SHORT fifazf.0045E9B4
	$+44     0045E984       8B7E 0C                        MOV EDI, DWORD PTR DS:[ESI+C]
	$+47     0045E987       6A 01                          PUSH 1
	$+49     0045E989       57                             PUSH EDI
	$+4A     0045E98A       E8 E1D1FFFF                    CALL fifazf.0045BB70
	$+4F     0045E98F       83C4 08                        ADD ESP, 8
	$+52     0045E992       85C0                           TEST EAX, EAX
	$+54     0045E994       75 4E                          JNZ SHORT fifazf.0045E9E4
	$+56     0045E996       6A 02                          PUSH 2
	$+58     0045E998       57                             PUSH EDI
	$+59     0045E999       E8 D2D1FFFF                    CALL fifazf.0045BB70
	$+5E     0045E99E       83C4 08                        ADD ESP, 8
	$+61     0045E9A1       85C0                           TEST EAX, EAX
	$+63     0045E9A3       75 27                          JNZ SHORT fifazf.0045E9CC
	$+65     0045E9A5       6A 04                          PUSH 4
	$+67     0045E9A7       57                             PUSH EDI
	$+68     0045E9A8       E8 C3D1FFFF                    CALL fifazf.0045BB70
	$+6D     0045E9AD       83C4 08                        ADD ESP, 8
	$+70     0045E9B0       85C0                           TEST EAX, EAX
	$+72     0045E9B2       75 48                          JNZ SHORT fifazf.0045E9FC
	$+74     0045E9B4       837E 18 02                     CMP DWORD PTR DS:[ESI+18], 2
	$+78     0045E9B8       75 6A                          JNZ SHORT fifazf.0045EA24
	$+7A     0045E9BA       8B7E 10                        MOV EDI, DWORD PTR DS:[ESI+10]
	$+7D     0045E9BD       6A 02                          PUSH 2
	$+7F     0045E9BF       57                             PUSH EDI
	$+80     0045E9C0       E8 ABD1FFFF                    CALL fifazf.0045BB70
	$+85     0045E9C5       83C4 08                        ADD ESP, 8
	$+88     0045E9C8       85C0                           TEST EAX, EAX
	$+8A     0045E9CA       74 09                          JE SHORT fifazf.0045E9D5
	$+8C     0045E9CC       5F                             POP EDI                                  ; 1FBDD018
	$+8D     0045E9CD       5E                             POP ESI                                  ; fifazf.00BF646D
	$+8E     0045E9CE       B8 03000000                    MOV EAX, 3
	$+93     0045E9D3       5D                             POP EBP                                  ; ntdll.770DFD91
	$+94     0045E9D4       C3                             RETN
	$+95     0045E9D5       6A 01                          PUSH 1
	$+97     0045E9D7       57                             PUSH EDI
	$+98     0045E9D8       E8 93D1FFFF                    CALL fifazf.0045BB70
	$+9D     0045E9DD       83C4 08                        ADD ESP, 8
	$+A0     0045E9E0       85C0                           TEST EAX, EAX
	$+A2     0045E9E2       74 09                          JE SHORT fifazf.0045E9ED
	$+A4     0045E9E4       5F                             POP EDI                                  ; ntdll.770DFD91
	$+A5     0045E9E5       5E                             POP ESI                                  ; ntdll.770DFD91
	$+A6     0045E9E6       B8 02000000                    MOV EAX, 2
	$+AB     0045E9EB       5D                             POP EBP                                  ; ntdll.770DFD91
	$+AC     0045E9EC       C3                             RETN
	$+AD     0045E9ED       6A 04                          PUSH 4
	$+AF     0045E9EF       57                             PUSH EDI
	$+B0     0045E9F0       E8 7BD1FFFF                    CALL fifazf.0045BB70
	$+B5     0045E9F5       83C4 08                        ADD ESP, 8
	$+B8     0045E9F8       85C0                           TEST EAX, EAX
	$+BA     0045E9FA       74 28                          JE SHORT fifazf.0045EA24
	$+BC     0045E9FC       57                             PUSH EDI
	$+BD     0045E9FD       E8 DE200800                    CALL fifazf.004E0AE0
	$+C2     0045EA02       83C4 04                        ADD ESP, 4
	$+C5     0045EA05       83F8 02                        CMP EAX, 2
	$+C8     0045EA08       75 09                          JNZ SHORT fifazf.0045EA13
	$+CA     0045EA0A       5F                             POP EDI                                  ; ntdll.770DFD91
	$+CB     0045EA0B       5E                             POP ESI                                  ; ntdll.770DFD91
	$+CC     0045EA0C       B8 07000000                    MOV EAX, 7
	$+D1     0045EA11       5D                             POP EBP                                  ; ntdll.770DFD91
	$+D2     0045EA12       C3                             RETN
	$+D3     0045EA13       83E8 03                        SUB EAX, 3
	$+D6     0045EA16       F7D8                           NEG EAX
	$+D8     0045EA18       1BC0                           SBB EAX, EAX
	$+DA     0045EA1A       5F                             POP EDI                                  ; ntdll.770DFD91
	$+DB     0045EA1B       83E0 FD                        AND EAX, FFFFFFFD
	$+DE     0045EA1E       5E                             POP ESI                                  ; ntdll.770DFD91
	$+DF     0045EA1F       83C0 08                        ADD EAX, 8
	$+E2     0045EA22       5D                             POP EBP                                  ; ntdll.770DFD91
	$+E3     0045EA23       C3                             RETN
	$+E4     0045EA24       B8 04000000                    MOV EAX, 4
	$+E9     0045EA29       3946 18                        CMP DWORD PTR DS:[ESI+18], EAX
	$+EC     0045EA2C       74 2E                          JE SHORT fifazf.0045EA5C
	$+EE     0045EA2E       85ED                           TEST EBP, EBP
	$+F0     0045EA30       75 25                          JNZ SHORT fifazf.0045EA57
	$+F2     0045EA32       833E 02                        CMP DWORD PTR DS:[ESI], 2
	$+F5     0045EA35       75 09                          JNZ SHORT fifazf.0045EA40
	$+F7     0045EA37       5F                             POP EDI                                  ; ntdll.770DFD91
	$+F8     0045EA38       5E                             POP ESI                                  ; ntdll.770DFD91
	$+F9     0045EA39       B8 06000000                    MOV EAX, 6
	$+FE     0045EA3E       5D                             POP EBP                                  ; ntdll.770DFD91
	$+FF     0045EA3F       C3                             RETN
	$+100    0045EA40       F646 30 02                     TEST BYTE PTR DS:[ESI+30], 2
	$+104    0045EA44       74 11                          JE SHORT fifazf.0045EA57
	$+106    0045EA46       8B56 58                        MOV EDX, DWORD PTR DS:[ESI+58]           ; fifazf.009F7944
	$+109    0045EA49       83BA 84010000 00               CMP DWORD PTR DS:[EDX+184], 0
	$+110    0045EA50       B8 06000000                    MOV EAX, 6
	$+115    0045EA55       74 05                          JE SHORT fifazf.0045EA5C
	$+117    0045EA57       B8 05000000                    MOV EAX, 5
	$+11C    0045EA5C       5F                             POP EDI                                  ; ntdll.770DFD91
	$+11D    0045EA5D       5E                             POP ESI                                  ; ntdll.770DFD91
	$+11E    0045EA5E       5D                             POP EBP                                  ; ntdll.770DFD91
	$+11F    0045EA5F       C3                             RETN
	$+120    0045EA60       56                             PUSH ESI
	$+121    0045EA61       8B7424 08                      MOV ESI, DWORD PTR SS:[ESP+8]
	$+125    0045EA65       8B46 08                        MOV EAX, DWORD PTR DS:[ESI+8]
	$+128    0045EA68       57                             PUSH EDI
	$+129    0045EA69       33FF                           XOR EDI, EDI
	$+12B    0045EA6B       897E 24                        MOV DWORD PTR DS:[ESI+24], EDI
	$+12E    0045EA6E       897E 30                        MOV DWORD PTR DS:[ESI+30], EDI
	$+131    0045EA71       C746 34 00500000               MOV DWORD PTR DS:[ESI+34], 5000
	$+138    0045EA78       8B48 14                        MOV ECX, DWORD PTR DS:[EAX+14]
	$+13B    0045EA7B       56                             PUSH ESI
	$+13C    0045EA7C       894E 20                        MOV DWORD PTR DS:[ESI+20], ECX
	$+13F    0045EA7F       E8 ECEFFFFF                    CALL fifazf.0045DA70
	$+144    0045EA84       8B86 84000000                  MOV EAX, DWORD PTR DS:[ESI+84]
	$+14A    0045EA8A       83C4 04                        ADD ESP, 4
	$+14D    0045EA8D       3BC7                           CMP EAX, EDI
	$+14F    0045EA8F       74 09                          JE SHORT fifazf.0045EA9A
	$+151    0045EA91       50                             PUSH EAX
	$+152    0045EA92       E8 893E0100                    CALL fifazf.00472920
	$+157    0045EA97       83C4 04                        ADD ESP, 4
	$+15A    0045EA9A       89BE 84000000                  MOV DWORD PTR DS:[ESI+84], EDI
	$+160    0045EAA0       8B86 90000000                  MOV EAX, DWORD PTR DS:[ESI+90]
	$+166    0045EAA6       3BC7                           CMP EAX, EDI
	$+168    0045EAA8       74 09                          JE SHORT fifazf.0045EAB3
	$+16A    0045EAAA       50                             PUSH EAX
	$+16B    0045EAAB       E8 703E0100                    CALL fifazf.00472920
	$+170    0045EAB0       83C4 04                        ADD ESP, 4
	$+173    0045EAB3       89BE 90000000                  MOV DWORD PTR DS:[ESI+90], EDI
	$+179    0045EAB9       5F                             POP EDI                                  ; ntdll.770DFD91
	$+17A    0045EABA       5E                             POP ESI                                  ; ntdll.770DFD91
	$+17B    0045EABB       C3                             RETN
	--*/
	addr_tmp = scan_exe_.ScanAddr("558B6C24??85ED7E??33C05DC3E8????????85C074", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_SSL_GET_ERROR  " << addr_tmp << FC_OFstream_EndlLog;

	/*
	UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_1 UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_2 UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_3
	这4个收包的hook经过这次更新已无用了，2016/8/25 20:52
	$-5F     00D2A130      81EC 80000000             SUB ESP, 80
	$-59     00D2A136      A1 60737201               MOV EAX, DWORD PTR DS:[1727360]
	$-54     00D2A13B      33C4                      XOR EAX, ESP
	$-52     00D2A13D      894424 7C                 MOV DWORD PTR SS:[ESP+7C], EAX
	$-4E     00D2A141      53                        PUSH EBX
	$-4D     00D2A142      8B9C24 98000000           MOV EBX, DWORD PTR SS:[ESP+98]                             ; fifazf.017E3454
	$-46     00D2A149      55                        PUSH EBP
	$-45     00D2A14A      8BAC24 8C000000           MOV EBP, DWORD PTR SS:[ESP+8C]
	$-3E     00D2A151      56                        PUSH ESI
	$-3D     00D2A152      8BB424 94000000           MOV ESI, DWORD PTR SS:[ESP+94]                             ; fifazf.01F69FA9
	$-36     00D2A159      57                        PUSH EDI
	$-35     00D2A15A      8BBC24 9C000000           MOV EDI, DWORD PTR SS:[ESP+9C]                             ; fifazf.01F6BB32
	$-2E     00D2A161      896C24 10                 MOV DWORD PTR SS:[ESP+10], EBP
	$-2A     00D2A165      E8 06AD6FFF               CALL fifazf.00424E70
	$-25     00D2A16A      8D0476                    LEA EAX, DWORD PTR DS:[ESI+ESI*2]
	$-22     00D2A16D      B9 FFFFFF7F               MOV ECX, 7FFFFFFF
	$-1D     00D2A172      8D6CC5 00                 LEA EBP, DWORD PTR SS:[EBP+EAX*8]
	$-19     00D2A176      8B8424 A0000000           MOV EAX, DWORD PTR SS:[ESP+A0]                             ; fifazf.008B62E9
	$-12     00D2A17D      3BC1                      CMP EAX, ECX
	$-10     00D2A17F      0F47C1                    CMOVA EAX, ECX
	$-D      00D2A182      50                        PUSH EAX
	$-C      00D2A183      57                        PUSH EDI
	$-B      00D2A184      FFB5 88010000             PUSH DWORD PTR SS:[EBP+188]
	$-5      00D2A18A      E8 E1A470FF               CALL fifazf.00434670                                       ; call ssl_read
	$ ==>    00D2A18F      8BF0                      MOV ESI, EAX                                               ; call ssl_read ret
	$+2      00D2A191      83C4 0C                   ADD ESP, 0C
	$+5      00D2A194      85F6                      TEST ESI, ESI
	$+7      00D2A196      7F 70                     JG SHORT fifazf.00D2A208
	$+9      00D2A198      56                        PUSH ESI
	$+A      00D2A199      FFB5 88010000             PUSH DWORD PTR SS:[EBP+188]
	$+10     00D2A19F      E8 8CC070FF               CALL fifazf.00436230
	$+15     00D2A1A4      83C4 08                   ADD ESP, 8
	$+18     00D2A1A7      83F8 06                   CMP EAX, 6
	$+1B     00D2A1AA      77 19                     JA SHORT fifazf.00D2A1C5
	$+1D     00D2A1AC      0FB680 30A2D200           MOVZX EAX, BYTE PTR DS:[EAX+D2A230]
	$+24     00D2A1B3      FF2485 24A2D200           JMP DWORD PTR DS:[EAX*4+D2A224]                            ; fifazf.00D2A208
	$+2B     00D2A1BA      C703 51000000             MOV DWORD PTR DS:[EBX], 51
	$+31     00D2A1C0      83C8 FF                   OR EAX, FFFFFFFF
	$+34     00D2A1C3      EB 45                     JMP SHORT fifazf.00D2A20A
	$+36     00D2A1C5      E8 D6AE6FFF               CALL fifazf.004250A0
	$+3B     00D2A1CA      8BF8                      MOV EDI, EAX
	$+3D     00D2A1CC      85F6                      TEST ESI, ESI
	$+3F     00D2A1CE      78 04                     JS SHORT fifazf.00D2A1D4
	$+41     00D2A1D0      85FF                      TEST EDI, EDI
	$+43     00D2A1D2      74 34                     JE SHORT fifazf.00D2A208
	$+45     00D2A1D4      FF15 6C3B4F01             CALL DWORD PTR DS:[14F3B6C]                                ; WS2_32.WSAGetLastError
	$+4B     00D2A1DA      50                        PUSH EAX
	$+4C     00D2A1DB      8D4424 18                 LEA EAX, DWORD PTR SS:[ESP+18]
	$+50     00D2A1DF      50                        PUSH EAX
	$+51     00D2A1E0      57                        PUSH EDI
	$+52     00D2A1E1      E8 6AAE6FFF               CALL fifazf.00425050
	$+57     00D2A1E6      83C4 08                   ADD ESP, 8
	$+5A     00D2A1E9      50                        PUSH EAX
	$+5B     00D2A1EA      8B4424 18                 MOV EAX, DWORD PTR SS:[ESP+18]
	$+5F     00D2A1EE      68 387B6201               PUSH fifazf.01627B38                                       ; ASCII "SSL read: %s, errno %d"
	$+64     00D2A1F3      FF30                      PUSH DWORD PTR DS:[EAX]
	$+66     00D2A1F5      E8 9616FFFF               CALL fifazf.00D1B890
	$+6B     00D2A1FA      83C4 10                   ADD ESP, 10
	$+6E     00D2A1FD      C703 38000000             MOV DWORD PTR DS:[EBX], 38
	$+74     00D2A203      83C8 FF                   OR EAX, FFFFFFFF
	$+77     00D2A206      EB 02                     JMP SHORT fifazf.00D2A20A
	$+79     00D2A208      8BC6                      MOV EAX, ESI
	$+7B     00D2A20A      8B8C24 8C000000           MOV ECX, DWORD PTR SS:[ESP+8C]
	$+82     00D2A211      5F                        POP EDI                                                    ; ntdll.7788FD91
	$+83     00D2A212      5E                        POP ESI                                                    ; ntdll.7788FD91
	$+84     00D2A213      5D                        POP EBP                                                    ; ntdll.7788FD91
	$+85     00D2A214      5B                        POP EBX                                                    ; ntdll.7788FD91
	$+86     00D2A215      33CC                      XOR ECX, ESP
	$+88     00D2A217      E8 C83E8CFF               CALL fifazf.005EE0E4
	$+8D     00D2A21C      81C4 80000000             ADD ESP, 80
	$+93     00D2A222      C3                        RETN
	$+94     00D2A223      90                        NOP
	$+95     00D2A224      08A2 D200BAA1             OR BYTE PTR DS:[EDX+A1BA00D2], AH
	$+9B     00D2A22A      D200                      ROL BYTE PTR DS:[EAX], CL
	$+9D     00D2A22C      C5A1 D2000002             LDS ESP, FWORD PTR DS:[ECX+20000D2]                        ; 段寄存器更改
	$+A3     00D2A232      0101                      ADD DWORD PTR DS:[ECX], EAX
	$+A5     00D2A234      0202                      ADD AL, BYTE PTR DS:[EDX]
	$+A7     00D2A236      00CC                      ADD AH, CL
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??83????85??7f??56ff??????????e8????????83", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR_ZHP  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	收包CALL
	00F8AF30    0F291424        MOVAPS DQWORD PTR SS:[ESP],XMM2
	00F8AF34    B9 10000000     MOV ECX,0x10
	00F8AF39    89E6            MOV ESI,ESP
	00F8AF3B    29C1            SUB ECX,EAX
	00F8AF3D    89F6            MOV ESI,ESI
	00F8AF3F    F3:A4           REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[>
	00F8AF41    8B6424 10       MOV ESP,DWORD PTR SS:[ESP+0x10]
	00F8AF45    8B6C24 24       MOV EBP,DWORD PTR SS:[ESP+0x24]
	00F8AF49    0F117D 00       MOVUPS DQWORD PTR SS:[EBP],XMM7
	00F8AF4D    5F              POP EDI
	00F8AF4E    5E              POP ESI
	00F8AF4F    5B              POP EBX
	00F8AF50    5D              POP EBP
	00F8AF51    C3              RETN

	zhp 2016/8/9 14:06 防止特征码找不到
	$-50     004ABE60       E8 3BE6FFFF                    CALL fifazf.004AA4A0
	$-4B     004ABE65       0F104E 10                      MOVUPS XMM1, DQWORD PTR DS:[ESI+10]
	$-47     004ABE69       0F1046 20                      MOVUPS XMM0, DQWORD PTR DS:[ESI+20]
	$-43     004ABE6D       0F57D7                         XORPS XMM2, XMM7
	$-40     004ABE70       0F107E 30                      MOVUPS XMM7, DQWORD PTR DS:[ESI+30]
	$-3C     004ABE74       0F57DE                         XORPS XMM3, XMM6
	$-39     004ABE77       0F1117                         MOVUPS DQWORD PTR DS:[EDI], XMM2
	$-36     004ABE7A       0F57E1                         XORPS XMM4, XMM1
	$-33     004ABE7D       0F115F 10                      MOVUPS DQWORD PTR DS:[EDI+10], XMM3
	$-2F     004ABE81       0F57E8                         XORPS XMM5, XMM0
	$-2C     004ABE84       0F1167 20                      MOVUPS DQWORD PTR DS:[EDI+20], XMM4
	$-28     004ABE88       8D7F 30                        LEA EDI, DWORD PTR DS:[EDI+30]
	$-25     004ABE8B       0F28D5                         MOVAPS XMM2, XMM5
	$-22     004ABE8E       2D 40000000                    SUB EAX, 40
	$-1D     004ABE93       25 0F000000                    AND EAX, 0F
	$-18     004ABE98       0F85 12000000                  JNZ fifazf.004ABEB0
	$-12     004ABE9E       0F1117                         MOVUPS DQWORD PTR DS:[EDI], XMM2
	$-F      004ABEA1       E9 1B000000                    JMP fifazf.004ABEC1
	$-A      004ABEA6       90                             NOP
	$-9      004ABEA7       90                             NOP
	$-8      004ABEA8       90                             NOP
	$-7      004ABEA9       90                             NOP
	$-6      004ABEAA       90                             NOP
	$-5      004ABEAB       90                             NOP
	$-4      004ABEAC       90                             NOP
	$-3      004ABEAD       90                             NOP
	$-2      004ABEAE       90                             NOP
	$-1      004ABEAF       90                             NOP
	$ ==>    004ABEB0       0F291424                       MOVAPS DQWORD PTR SS:[ESP], XMM2
	$+4      004ABEB4       B9 10000000                    MOV ECX, 10
	$+9      004ABEB9       89E6                           MOV ESI, ESP
	$+B      004ABEBB       29C1                           SUB ECX, EAX
	$+D      004ABEBD       89F6                           MOV ESI, ESI
	$+F      004ABEBF       F3:A4                          REP MOVS BYTE PTR ES:[EDI], BYTE PTR DS:>
	$+11     004ABEC1       8B6424 10                      MOV ESP, DWORD PTR SS:[ESP+10]
	$+15     004ABEC5       8B6C24 24                      MOV EBP, DWORD PTR SS:[ESP+24]
	$+19     004ABEC9       0F117D 00                      MOVUPS DQWORD PTR SS:[EBP], XMM7
	$+1D     004ABECD       5F                             POP EDI                                  ; ntdll.770DFD91
	$+1E     004ABECE       5E                             POP ESI                                  ; ntdll.770DFD91
	$+1F     004ABECF       5B                             POP EBX                                  ; ntdll.770DFD91
	$+20     004ABED0       5D                             POP EBP                                  ; ntdll.770DFD91
	$+21     004ABED1       C3                             RETN
	$+22     004ABED2       90                             NOP
	$+23     004ABED3       90                             NOP
	$+24     004ABED4       90                             NOP
	$+25     004ABED5       90                             NOP
	$+26     004ABED6       90                             NOP
	$+27     004ABED7       90                             NOP
	$+28     004ABED8       90                             NOP
	$+29     004ABED9       90                             NOP
	$+2A     004ABEDA       90                             NOP
	$+2B     004ABEDB       90                             NOP
	$+2C     004ABEDC       90                             NOP
	$+2D     004ABEDD       90                             NOP
	$+2E     004ABEDE       90                             NOP
	$+2F     004ABEDF       90                             NOP
	$+30     004ABEE0       85C0                           TEST EAX, EAX
	$+32     004ABEE2       0F84 A4020000                  JE fifazf.004AC18C
	$+38     004ABEE8       85D2                           TEST EDX, EDX
	$+3A     004ABEEA       0F84 9C020000                  JE fifazf.004AC18C
	$+40     004ABEF0       0F1000                         MOVUPS XMM0, DQWORD PTR DS:[EAX]
	$+43     004ABEF3       0F57E4                         XORPS XMM4, XMM4
	$+46     004ABEF6       8D52 10                        LEA EDX, DWORD PTR DS:[EDX+10]
	$+49     004ABEF9       81F9 00010000                  CMP ECX, 100
	$+4F     004ABEFF       0F84 9B010000                  JE fifazf.004AC0A0
	$+55     004ABF05       81F9 C0000000                  CMP ECX, 0C0
	$+5B     004ABF0B       0F84 AF000000                  JE fifazf.004ABFC0
	$+61     004ABF11       81F9 80000000                  CMP ECX, 80
	$+67     004ABF17       0F85 77020000                  JNZ fifazf.004AC194
	$+6D     004ABF1D       90                             NOP
	$+6E     004ABF1E       90                             NOP
	$+6F     004ABF1F       90                             NOP
	$+70     004ABF20       B9 09000000                    MOV ECX, 9
	$+75     004ABF25       0F1142 F0                      MOVUPS DQWORD PTR DS:[EDX-10], XMM0
	$+79     004ABF29       66:0F3A                        ???                                      ; 未知命令
	$+7C     004ABF2C       DF                             ???                                      ; 未知命令
	$+7D     004ABF2D       C8 01E872                      ENTER 0E801, 72
	$+81     004ABF31       0000                           ADD BYTE PTR DS:[EAX], AL
	$+83     004ABF33       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+86     004ABF36       3ADF                           CMP BL, BH
	$+88     004ABF38       C8 02E861                      ENTER 0E802, 61
	$+8C     004ABF3C       0000                           ADD BYTE PTR DS:[EAX], AL
	$+8E     004ABF3E       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+91     004ABF41       3ADF                           CMP BL, BH
	$+93     004ABF43       C8 04E856                      ENTER 0E804, 56
	$+97     004ABF47       0000                           ADD BYTE PTR DS:[EAX], AL
	$+99     004ABF49       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+9C     004ABF4C       3ADF                           CMP BL, BH
	$+9E     004ABF4E       C8 08E84B                      ENTER 0E808, 4B
	$+A2     004ABF52       0000                           ADD BYTE PTR DS:[EAX], AL
	$+A4     004ABF54       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+A7     004ABF57       3ADF                           CMP BL, BH
	$+A9     004ABF59       C8 10E840                      ENTER 0E810, 40
	$+AD     004ABF5D       0000                           ADD BYTE PTR DS:[EAX], AL
	$+AF     004ABF5F       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+B2     004ABF62       3ADF                           CMP BL, BH
	$+B4     004ABF64       C8 20E835                      ENTER 0E820, 35
	$+B8     004ABF68       0000                           ADD BYTE PTR DS:[EAX], AL
	$+BA     004ABF6A       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+BD     004ABF6D       3ADF                           CMP BL, BH
	$+BF     004ABF6F       C8 40E82A                      ENTER 0E840, 2A
	$+C3     004ABF73       0000                           ADD BYTE PTR DS:[EAX], AL
	$+C5     004ABF75       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	$+C8     004ABF78       3ADF                           CMP BL, BH
	$+CA     004ABF7A       C8 80E81F                      ENTER 0E880, 1F
	$+CE     004ABF7E       0000                           ADD BYTE PTR DS:[EAX], AL
	$+D0     004ABF80       0066 0F                        ADD BYTE PTR DS:[ESI+F], AH
	--*/

	/*addr_tmp = scan_exe_.ScanAddr("0F291424B9????????89E629C189F6F3A48B6424", 0x1d);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR  " << addr_tmp << FC_OFstream_EndlLog;*/

	/*++
	收包CALL1
	00F99CC8    9D                     POPFD
	00F99CC9    5F                     POP EDI
	00F99CCA    5E                     POP ESI
	00F99CCB    5B                     POP EBX
	00F99CCC    5D                     POP EBP
	00F99CCD    C3                     RETN
	00F99CCE    9C                     PUSHFD
	00F99CCF    90                     NOP
	00F99CD0    8B00                   MOV EAX,DWORD PTR DS:[EAX]
	00F99CD2    8B7C24 24              MOV EDI,DWORD PTR SS:[ESP+0x24]
	00F99CD6    8D7424 B0              LEA ESI,DWORD PTR SS:[ESP-0x50]
	00F99CDA    81E6 C0FFFFFF          AND ESI,0xFFFFFFC0
	00F99CE0    8D9F 71FFFFFF          LEA EBX,DWORD PTR DS:[EDI-0x8F]
	00F99CE6    29F3                   SUB EBX,ESI
	00F99CE8    F7DB                   NEG EBX
	00F99CEA    81E3 C0030000          AND EBX,0x3C0
	00F99CF0    29DE                   SUB ESI,EBX
	00F99CF2    8D9E 00030000          LEA EBX,DWORD PTR DS:[ESI+0x300]
	00F99CF8    29EB                   SUB EBX,EBP
	00F99CFA    81E3 00030000          AND EBX,0x300
	00F99D00    8DAC1D 80080000        LEA EBP,DWORD PTR SS:[EBP+EBX+0x880]
	00F99D07    8D5424 18              LEA EDX,DWORD PTR SS:[ESP+0x18]
	00F99D0B    87E6                   XCHG ESI,ESP
	00F99D0D    81C4 04000000          ADD ESP,0x4
	00F99D13    896C24 18              MOV DWORD PTR SS:[ESP+0x18],EBP
	00F99D17    897424 1C              MOV DWORD PTR SS:[ESP+0x1C],ESI
	00F99D1B    894424 34              MOV DWORD PTR SS:[ESP+0x34],EAX
	00F99D1F    8B02                   MOV EAX,DWORD PTR DS:[EDX]
	00F99D21    8B5A 04                MOV EBX,DWORD PTR DS:[EDX+0x4]
	00F99D24    8B72 10                MOV ESI,DWORD PTR DS:[EDX+0x10]
	00F99D27    8B52 14                MOV EDX,DWORD PTR DS:[EDX+0x14]
	00F99D2A    894424 20              MOV DWORD PTR SS:[ESP+0x20],EAX
	00F99D2E    895C24 24              MOV DWORD PTR SS:[ESP+0x24],EBX
	00F99D32    894C24 28              MOV DWORD PTR SS:[ESP+0x28],ECX
	00F99D36    897C24 2C              MOV DWORD PTR SS:[ESP+0x2C],EDI
	00F99D3A    897424 30              MOV DWORD PTR SS:[ESP+0x30],ESI
	00F99D3E    89F7                   MOV EDI,ESI
	00F99D40    89C6                   MOV ESI,EAX
	00F99D42    81FA 00000000          CMP EDX,0x0
	00F99D48    0F84 62010000          JE fifazf.00F99EB0
	00F99D4E    81F9 10000000          CMP ECX,0x10
	00F99D54    89DA                   MOV EDX,EBX
	00F99D56    0F82 14010000          JB fifazf.00F99E70
	00F99D5C    0FBA6424 34 19         BT DWORD PTR SS:[ESP+0x34],0x19
	00F99D62    0F83 78000000          JNB fifazf.00F99DE0
	00F99D68    0F6F07                 MOVQ MM0,QWORD PTR DS:[EDI]
	00F99D6B    0F6F67 08              MOVQ MM4,QWORD PTR DS:[EDI+0x8]
	00F99D6F    90                     NOP
	00F99D70    0FEF06                 PXOR MM0,QWORD PTR DS:[ESI]
	00F99D73    0FEF66 08              PXOR MM4,QWORD PTR DS:[ESI+0x8]
	00F99D77    8B7C24 2C              MOV EDI,DWORD PTR SS:[ESP+0x2C]
	00F99D7B    E8 20D4FFFF            CALL fifazf.00F971A0

	zhp 2016/8/9 14:07 防止特征码找不到
	$-148    004B0BA0       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$-146    004B0BA2       8B5E 04                        MOV EBX, DWORD PTR DS:[ESI+4]
	$-143    004B0BA5       8B4E 08                        MOV ECX, DWORD PTR DS:[ESI+8]
	$-140    004B0BA8       8B56 0C                        MOV EDX, DWORD PTR DS:[ESI+C]
	$-13D    004B0BAB       8D7C24 3C                      LEA EDI, DWORD PTR SS:[ESP+3C]
	$-139    004B0BAF       8907                           MOV DWORD PTR DS:[EDI], EAX
	$-137    004B0BB1       895F 04                        MOV DWORD PTR DS:[EDI+4], EBX
	$-134    004B0BB4       894F 08                        MOV DWORD PTR DS:[EDI+8], ECX
	$-131    004B0BB7       8957 0C                        MOV DWORD PTR DS:[EDI+C], EDX
	$-12E    004B0BBA       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]
	$-12A    004B0BBE       E8 1DEEFFFF                    CALL fifazf.004AF9E0
	$-125    004B0BC3       8B7C24 30                      MOV EDI, DWORD PTR SS:[ESP+30]
	$-121    004B0BC7       8B7424 24                      MOV ESI, DWORD PTR SS:[ESP+24]
	$-11D    004B0BCB       3307                           XOR EAX, DWORD PTR DS:[EDI]
	$-11B    004B0BCD       335F 04                        XOR EBX, DWORD PTR DS:[EDI+4]
	$-118    004B0BD0       334F 08                        XOR ECX, DWORD PTR DS:[EDI+8]
	$-115    004B0BD3       3357 0C                        XOR EDX, DWORD PTR DS:[EDI+C]
	$-112    004B0BD6       8906                           MOV DWORD PTR DS:[ESI], EAX
	$-110    004B0BD8       895E 04                        MOV DWORD PTR DS:[ESI+4], EBX
	$-10D    004B0BDB       894E 08                        MOV DWORD PTR DS:[ESI+8], ECX
	$-10A    004B0BDE       8956 0C                        MOV DWORD PTR DS:[ESI+C], EDX
	$-107    004B0BE1       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$-104    004B0BE4       897424 24                      MOV DWORD PTR SS:[ESP+24], ESI
	$-100    004B0BE8       8D7424 3C                      LEA ESI, DWORD PTR SS:[ESP+3C]
	$-FC     004B0BEC       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$-FA     004B0BEE       8B5E 04                        MOV EBX, DWORD PTR DS:[ESI+4]
	$-F7     004B0BF1       8B4E 08                        MOV ECX, DWORD PTR DS:[ESI+8]
	$-F4     004B0BF4       8B56 0C                        MOV EDX, DWORD PTR DS:[ESI+C]
	$-F1     004B0BF7       8907                           MOV DWORD PTR DS:[EDI], EAX
	$-EF     004B0BF9       895F 04                        MOV DWORD PTR DS:[EDI+4], EBX
	$-EC     004B0BFC       894F 08                        MOV DWORD PTR DS:[EDI+8], ECX
	$-E9     004B0BFF       8957 0C                        MOV DWORD PTR DS:[EDI+C], EDX
	$-E6     004B0C02       8B7424 20                      MOV ESI, DWORD PTR SS:[ESP+20]
	$-E2     004B0C06       8B4C24 28                      MOV ECX, DWORD PTR SS:[ESP+28]
	$-DE     004B0C0A       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$-DB     004B0C0D       897424 20                      MOV DWORD PTR SS:[ESP+20], ESI
	$-D7     004B0C11       81E9 10000000                  SUB ECX, 10
	$-D1     004B0C17       894C24 28                      MOV DWORD PTR SS:[ESP+28], ECX
	$-CD     004B0C1B     ^ 0F85 7FFFFFFF                  JNZ fifazf.004B0BA0
	$-C7     004B0C21       90                             NOP
	$-C6     004B0C22       90                             NOP
	$-C5     004B0C23       90                             NOP
	$-C4     004B0C24       81BC24 3C010000 00000000       CMP DWORD PTR SS:[ESP+13C], 0
	$-B9     004B0C2F       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]
	$-B5     004B0C33       0F84 0B000000                  JE fifazf.004B0C44
	$-AF     004B0C39       B9 3C000000                    MOV ECX, 3C
	$-AA     004B0C3E       31C0                           XOR EAX, EAX
	$-A8     004B0C40       89F6                           MOV ESI, ESI
	$-A6     004B0C42       F3:AB                          REP STOS DWORD PTR ES:[EDI]
	$-A4     004B0C44       8B6424 1C                      MOV ESP, DWORD PTR SS:[ESP+1C]
	$-A0     004B0C48       9D                             POPFD
	$-9F     004B0C49       5F                             POP EDI                                  ; ntdll.770DFD91
	$-9E     004B0C4A       5E                             POP ESI                                  ; ntdll.770DFD91
	$-9D     004B0C4B       5B                             POP EBX                                  ; ntdll.770DFD91
	$-9C     004B0C4C       5D                             POP EBP                                  ; ntdll.770DFD91
	$-9B     004B0C4D       C3                             RETN
	$-9A     004B0C4E       9C                             PUSHFD
	$-99     004B0C4F       90                             NOP
	$-98     004B0C50       8B00                           MOV EAX, DWORD PTR DS:[EAX]
	$-96     004B0C52       8B7C24 24                      MOV EDI, DWORD PTR SS:[ESP+24]
	$-92     004B0C56       8D7424 B0                      LEA ESI, DWORD PTR SS:[ESP-50]
	$-8E     004B0C5A       81E6 C0FFFFFF                  AND ESI, FFFFFFC0
	$-88     004B0C60       8D9F 71FFFFFF                  LEA EBX, DWORD PTR DS:[EDI-8F]
	$-82     004B0C66       29F3                           SUB EBX, ESI
	$-80     004B0C68       F7DB                           NEG EBX
	$-7E     004B0C6A       81E3 C0030000                  AND EBX, 3C0
	$-78     004B0C70       29DE                           SUB ESI, EBX
	$-76     004B0C72       8D9E 00030000                  LEA EBX, DWORD PTR DS:[ESI+300]
	$-70     004B0C78       29EB                           SUB EBX, EBP
	$-6E     004B0C7A       81E3 00030000                  AND EBX, 300
	$-68     004B0C80       8DAC1D 80080000                LEA EBP, DWORD PTR SS:[EBP+EBX+880]
	$-61     004B0C87       8D5424 18                      LEA EDX, DWORD PTR SS:[ESP+18]
	$-5D     004B0C8B       87E6                           XCHG ESI, ESP
	$-5B     004B0C8D       81C4 04000000                  ADD ESP, 4
	$-55     004B0C93       896C24 18                      MOV DWORD PTR SS:[ESP+18], EBP
	$-51     004B0C97       897424 1C                      MOV DWORD PTR SS:[ESP+1C], ESI
	$-4D     004B0C9B       894424 34                      MOV DWORD PTR SS:[ESP+34], EAX
	$-49     004B0C9F       8B02                           MOV EAX, DWORD PTR DS:[EDX]
	$-47     004B0CA1       8B5A 04                        MOV EBX, DWORD PTR DS:[EDX+4]
	$-44     004B0CA4       8B72 10                        MOV ESI, DWORD PTR DS:[EDX+10]
	$-41     004B0CA7       8B52 14                        MOV EDX, DWORD PTR DS:[EDX+14]
	$-3E     004B0CAA       894424 20                      MOV DWORD PTR SS:[ESP+20], EAX
	$-3A     004B0CAE       895C24 24                      MOV DWORD PTR SS:[ESP+24], EBX
	$-36     004B0CB2       894C24 28                      MOV DWORD PTR SS:[ESP+28], ECX
	$-32     004B0CB6       897C24 2C                      MOV DWORD PTR SS:[ESP+2C], EDI
	$-2E     004B0CBA       897424 30                      MOV DWORD PTR SS:[ESP+30], ESI
	$-2A     004B0CBE       89F7                           MOV EDI, ESI
	$-28     004B0CC0       89C6                           MOV ESI, EAX
	$-26     004B0CC2       81FA 00000000                  CMP EDX, 0
	$-20     004B0CC8       0F84 62010000                  JE fifazf.004B0E30
	$-1A     004B0CCE       81F9 10000000                  CMP ECX, 10
	$-14     004B0CD4       89DA                           MOV EDX, EBX
	$-12     004B0CD6       0F82 14010000                  JB fifazf.004B0DF0
	$-C      004B0CDC       0FBA6424 34 19                 BT DWORD PTR SS:[ESP+34], 19
	$-6      004B0CE2       0F83 78000000                  JNB fifazf.004B0D60
	$ ==>    004B0CE8       0F6F07                         MOVQ MM0, QWORD PTR DS:[EDI]
	$+3      004B0CEB       0F6F67 08                      MOVQ MM4, QWORD PTR DS:[EDI+8]
	$+7      004B0CEF       90                             NOP
	$+8      004B0CF0       0FEF06                         PXOR MM0, QWORD PTR DS:[ESI]
	$+B      004B0CF3       0FEF66 08                      PXOR MM4, QWORD PTR DS:[ESI+8]
	$+F      004B0CF7       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]
	$+13     004B0CFB       E8 20D4FFFF                    CALL fifazf.004AE120
	$+18     004B0D00       8B7424 20                      MOV ESI, DWORD PTR SS:[ESP+20]
	$+1C     004B0D04       8B7C24 24                      MOV EDI, DWORD PTR SS:[ESP+24]
	$+20     004B0D08       8B4C24 28                      MOV ECX, DWORD PTR SS:[ESP+28]
	$+24     004B0D0C       0F7F07                         MOVQ QWORD PTR DS:[EDI], MM0
	$+27     004B0D0F       0F7F67 08                      MOVQ QWORD PTR DS:[EDI+8], MM4
	$+2B     004B0D13       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$+2E     004B0D16       897424 20                      MOV DWORD PTR SS:[ESP+20], ESI
	$+32     004B0D1A       8D57 10                        LEA EDX, DWORD PTR DS:[EDI+10]
	$+35     004B0D1D       895424 24                      MOV DWORD PTR SS:[ESP+24], EDX
	$+39     004B0D21       81E9 10000000                  SUB ECX, 10
	$+3F     004B0D27       81F9 10000000                  CMP ECX, 10
	$+45     004B0D2D       894C24 28                      MOV DWORD PTR SS:[ESP+28], ECX
	$+49     004B0D31     ^ 0F83 B9FFFFFF                  JNB fifazf.004B0CF0
	$+4F     004B0D37       F7C1 0F000000                  TEST ECX, 0F
	$+55     004B0D3D       0F85 AD000000                  JNZ fifazf.004B0DF0
	$+5B     004B0D43       8B7424 30                      MOV ESI, DWORD PTR SS:[ESP+30]
	$+5F     004B0D47       0F7F06                         MOVQ QWORD PTR DS:[ESI], MM0
	$+62     004B0D4A       0F7F66 08                      MOVQ QWORD PTR DS:[ESI+8], MM4
	$+66     004B0D4E       0F77                           EMMS
	$+68     004B0D50       8B6424 1C                      MOV ESP, DWORD PTR SS:[ESP+1C]
	$+6C     004B0D54       9D                             POPFD
	$+6D     004B0D55       5F                             POP EDI                                  ; ntdll.770DFD91
	$+6E     004B0D56       5E                             POP ESI                                  ; ntdll.770DFD91
	$+6F     004B0D57       5B                             POP EBX                                  ; ntdll.770DFD91
	$+70     004B0D58       5D                             POP EBP                                  ; ntdll.770DFD91
	$+71     004B0D59       C3                             RETN
	$+72     004B0D5A       9C                             PUSHFD
	$+73     004B0D5B       90                             NOP
	$+74     004B0D5C       90                             NOP
	$+75     004B0D5D       90                             NOP
	$+76     004B0D5E       90                             NOP
	$+77     004B0D5F       90                             NOP
	$+78     004B0D60       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+7A     004B0D62       8B5F 04                        MOV EBX, DWORD PTR DS:[EDI+4]
	$+7D     004B0D65       90                             NOP
	$+7E     004B0D66       90                             NOP
	$+7F     004B0D67       90                             NOP
	$+80     004B0D68       8B4F 08                        MOV ECX, DWORD PTR DS:[EDI+8]
	$+83     004B0D6B       8B57 0C                        MOV EDX, DWORD PTR DS:[EDI+C]
	$+86     004B0D6E       3306                           XOR EAX, DWORD PTR DS:[ESI]
	$+88     004B0D70       335E 04                        XOR EBX, DWORD PTR DS:[ESI+4]
	$+8B     004B0D73       334E 08                        XOR ECX, DWORD PTR DS:[ESI+8]
	$+8E     004B0D76       3356 0C                        XOR EDX, DWORD PTR DS:[ESI+C]            ; fifazf.008F3DA1
	$+91     004B0D79       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]           ; fifazf.0061E7AD
	$+95     004B0D7D       E8 3ED0FFFF                    CALL fifazf.004ADDC0
	$+9A     004B0D82       8B7424 20                      MOV ESI, DWORD PTR SS:[ESP+20]
	$+9E     004B0D86       8B7C24 24                      MOV EDI, DWORD PTR SS:[ESP+24]
	$+A2     004B0D8A       8907                           MOV DWORD PTR DS:[EDI], EAX
	$+A4     004B0D8C       895F 04                        MOV DWORD PTR DS:[EDI+4], EBX
	$+A7     004B0D8F       894F 08                        MOV DWORD PTR DS:[EDI+8], ECX
	$+AA     004B0D92       8957 0C                        MOV DWORD PTR DS:[EDI+C], EDX
	$+AD     004B0D95       8B4C24 28                      MOV ECX, DWORD PTR SS:[ESP+28]
	$+B1     004B0D99       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$+B4     004B0D9C       897424 20                      MOV DWORD PTR SS:[ESP+20], ESI
	$+B8     004B0DA0       8D57 10                        LEA EDX, DWORD PTR DS:[EDI+10]
	$+BB     004B0DA3       895424 24                      MOV DWORD PTR SS:[ESP+24], EDX
	$+BF     004B0DA7       81E9 10000000                  SUB ECX, 10
	$+C5     004B0DAD       81F9 10000000                  CMP ECX, 10
	$+CB     004B0DB3       894C24 28                      MOV DWORD PTR SS:[ESP+28], ECX
	$+CF     004B0DB7     ^ 0F83 ABFFFFFF                  JNB fifazf.004B0D68
	$+D5     004B0DBD       F7C1 0F000000                  TEST ECX, 0F
	$+DB     004B0DC3       0F85 27000000                  JNZ fifazf.004B0DF0
	$+E1     004B0DC9       8B7424 30                      MOV ESI, DWORD PTR SS:[ESP+30]
	$+E5     004B0DCD       8B4F 08                        MOV ECX, DWORD PTR DS:[EDI+8]
	$+E8     004B0DD0       8B57 0C                        MOV EDX, DWORD PTR DS:[EDI+C]
	$+EB     004B0DD3       8906                           MOV DWORD PTR DS:[ESI], EAX
	$+ED     004B0DD5       895E 04                        MOV DWORD PTR DS:[ESI+4], EBX
	$+F0     004B0DD8       894E 08                        MOV DWORD PTR DS:[ESI+8], ECX
	$+F3     004B0DDB       8956 0C                        MOV DWORD PTR DS:[ESI+C], EDX
	$+F6     004B0DDE       8B6424 1C                      MOV ESP, DWORD PTR SS:[ESP+1C]
	$+FA     004B0DE2       9D                             POPFD
	$+FB     004B0DE3       5F                             POP EDI                                  ; ntdll.770DFD91
	$+FC     004B0DE4       5E                             POP ESI                                  ; 1FBDE48C
	$+FD     004B0DE5       5B                             POP EBX                                  ; fifazf.00BF646D
	$+FE     004B0DE6       5D                             POP EBP
	$+FF     004B0DE7       C3                             RETN
	$+100    004B0DE8       9C                             PUSHFD
	--*/
	/*addr_tmp = scan_exe_.ScanAddr("0F6F070F6F67??900FEF060FEF66??8B7C24??E8", -0x9F);
	assert(addr_tmp);
	assert(*(pt_byte*)addr_tmp == 0x5f);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_1  " << addr_tmp << FC_OFstream_EndlLog;*/

	/*++
	收包CALL2
	00F99EE0    0F6F0E                 MOVQ MM1,QWORD PTR DS:[ESI]
	00F99EE3    0F6F6E 08              MOVQ MM5,QWORD PTR DS:[ESI+0x8]
	00F99EE7    0FEF07                 PXOR MM0,QWORD PTR DS:[EDI]
	00F99EEA    0FEF67 08              PXOR MM4,QWORD PTR DS:[EDI+0x8]
	00F99EEE    0F7F0F                 MOVQ QWORD PTR DS:[EDI],MM1
	00F99EF1    0F7F6F 08              MOVQ QWORD PTR DS:[EDI+0x8],MM5
	00F99EF5    81E9 10000000          SUB ECX,0x10
	00F99EFB    0F82 2F000000          JB fifazf.00F99F30
	00F99F01    0F7F03                 MOVQ QWORD PTR DS:[EBX],MM0
	00F99F04    0F7F63 08              MOVQ QWORD PTR DS:[EBX+0x8],MM4
	00F99F08    8D5B 10                LEA EBX,DWORD PTR DS:[EBX+0x10]
	00F99F0B    895C24 24              MOV DWORD PTR SS:[ESP+0x24],EBX
	00F99F0F    8D76 10                LEA ESI,DWORD PTR DS:[ESI+0x10]
	00F99F12    897424 20              MOV DWORD PTR SS:[ESP+0x20],ESI
	00F99F16    894C24 28              MOV DWORD PTR SS:[ESP+0x28],ECX
	00F99F1A  ^ 0F85 9CFFFFFF          JNZ fifazf.00F99EBC
	00F99F20    0F77                   EMMS
	00F99F22    8B6424 1C              MOV ESP,DWORD PTR SS:[ESP+0x1C]
	00F99F26    9D                     POPFD
	00F99F27    5F                     POP EDI
	00F99F28    5E                     POP ESI
	00F99F29    5B                     POP EBX
	00F99F2A    5D                     POP EBP
	00F99F2B    C3                     RETN

	zhp 2016/8/9 14:22 防止特征码找不到
	$-70     004B0DF0       0F77                           EMMS
	$-6E     004B0DF2       89D7                           MOV EDI, EDX
	$-6C     004B0DF4       BB 10000000                    MOV EBX, 10
	$-67     004B0DF9       29CB                           SUB EBX, ECX
	$-65     004B0DFB       39F7                           CMP EDI, ESI
	$-63     004B0DFD       0F84 0A000000                  JE fifazf.004B0E0D
	$-5D     004B0E03       90                             NOP
	$-5C     004B0E04       89F6                           MOV ESI, ESI
	$-5A     004B0E06       F3:A4                          REP MOVS BYTE PTR ES:[EDI], BYTE PTR DS:>
	$-58     004B0E08       E9 03000000                    JMP fifazf.004B0E10
	$-53     004B0E0D       8D3C0F                         LEA EDI, DWORD PTR DS:[EDI+ECX]
	$-50     004B0E10       89D9                           MOV ECX, EBX
	$-4E     004B0E12       31C0                           XOR EAX, EAX
	$-4C     004B0E14       89F6                           MOV ESI, ESI
	$-4A     004B0E16       F3:AA                          REP STOS BYTE PTR ES:[EDI]
	$-48     004B0E18       8B7C24 30                      MOV EDI, DWORD PTR SS:[ESP+30]
	$-44     004B0E1C       89D6                           MOV ESI, EDX
	$-42     004B0E1E       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$-40     004B0E20       8B5F 04                        MOV EBX, DWORD PTR DS:[EDI+4]
	$-3D     004B0E23       C74424 28 10000000             MOV DWORD PTR SS:[ESP+28], 10
	$-35     004B0E2B     ^ E9 38FFFFFF                    JMP fifazf.004B0D68
	$-30     004B0E30       0FBA6424 34 19                 BT DWORD PTR SS:[ESP+34], 19
	$-2A     004B0E36       0F83 A4000000                  JNB fifazf.004B0EE0
	$-24     004B0E3C       0F6F06                         MOVQ MM0, QWORD PTR DS:[ESI]
	$-21     004B0E3F       0F6F66 08                      MOVQ MM4, QWORD PTR DS:[ESI+8]
	$-1D     004B0E43       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]
	$-19     004B0E47       E8 C4E8FFFF                    CALL fifazf.004AF710
	$-14     004B0E4C       8B7424 20                      MOV ESI, DWORD PTR SS:[ESP+20]
	$-10     004B0E50       8D4424 3C                      LEA EAX, DWORD PTR SS:[ESP+3C]
	$-C      004B0E54       8B5C24 24                      MOV EBX, DWORD PTR SS:[ESP+24]
	$-8      004B0E58       8B4C24 28                      MOV ECX, DWORD PTR SS:[ESP+28]
	$-4      004B0E5C       8B7C24 30                      MOV EDI, DWORD PTR SS:[ESP+30]
	$ ==>    004B0E60       0F6F0E                         MOVQ MM1, QWORD PTR DS:[ESI]
	$+3      004B0E63       0F6F6E 08                      MOVQ MM5, QWORD PTR DS:[ESI+8]
	$+7      004B0E67       0FEF07                         PXOR MM0, QWORD PTR DS:[EDI]
	$+A      004B0E6A       0FEF67 08                      PXOR MM4, QWORD PTR DS:[EDI+8]
	$+E      004B0E6E       0F7F0F                         MOVQ QWORD PTR DS:[EDI], MM1
	$+11     004B0E71       0F7F6F 08                      MOVQ QWORD PTR DS:[EDI+8], MM5
	$+15     004B0E75       81E9 10000000                  SUB ECX, 10
	$+1B     004B0E7B       0F82 2F000000                  JB fifazf.004B0EB0
	$+21     004B0E81       0F7F03                         MOVQ QWORD PTR DS:[EBX], MM0
	$+24     004B0E84       0F7F63 08                      MOVQ QWORD PTR DS:[EBX+8], MM4
	$+28     004B0E88       8D5B 10                        LEA EBX, DWORD PTR DS:[EBX+10]
	$+2B     004B0E8B       895C24 24                      MOV DWORD PTR SS:[ESP+24], EBX
	$+2F     004B0E8F       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$+32     004B0E92       897424 20                      MOV DWORD PTR SS:[ESP+20], ESI
	$+36     004B0E96       894C24 28                      MOV DWORD PTR SS:[ESP+28], ECX
	$+3A     004B0E9A     ^ 0F85 9CFFFFFF                  JNZ fifazf.004B0E3C
	$+40     004B0EA0       0F77                           EMMS
	$+42     004B0EA2       8B6424 1C                      MOV ESP, DWORD PTR SS:[ESP+1C]
	$+46     004B0EA6       9D                             POPFD
	$+47     004B0EA7       5F                             POP EDI                                  ; ntdll.770DFD91
	$+48     004B0EA8       5E                             POP ESI                                  ; ntdll.770DFD91
	$+49     004B0EA9       5B                             POP EBX                                  ; ntdll.770DFD91
	$+4A     004B0EAA       5D                             POP EBP                                  ; ntdll.770DFD91
	$+4B     004B0EAB       C3                             RETN
	$+4C     004B0EAC       9C                             PUSHFD
	$+4D     004B0EAD       90                             NOP
	$+4E     004B0EAE       90                             NOP
	$+4F     004B0EAF       90                             NOP
	$+50     004B0EB0       0F7F00                         MOVQ QWORD PTR DS:[EAX], MM0
	$+53     004B0EB3       0F7F60 08                      MOVQ QWORD PTR DS:[EAX+8], MM4
	$+57     004B0EB7       0F77                           EMMS
	$+59     004B0EB9       81C1 10000000                  ADD ECX, 10
	$+5F     004B0EBF       89DF                           MOV EDI, EBX
	$+61     004B0EC1       89C6                           MOV ESI, EAX
	$+63     004B0EC3       90                             NOP
	$+64     004B0EC4       89F6                           MOV ESI, ESI
	$+66     004B0EC6       F3:A4                          REP MOVS BYTE PTR ES:[EDI], BYTE PTR DS:>
	$+68     004B0EC8       8B6424 1C                      MOV ESP, DWORD PTR SS:[ESP+1C]
	$+6C     004B0ECC       9D                             POPFD
	$+6D     004B0ECD       5F                             POP EDI                                  ; ntdll.770DFD91
	$+6E     004B0ECE       5E                             POP ESI                                  ; ntdll.770DFD91
	$+6F     004B0ECF       5B                             POP EBX                                  ; ntdll.770DFD91
	$+70     004B0ED0       5D                             POP EBP                                  ; ntdll.770DFD91
	$+71     004B0ED1       C3                             RETN
	$+72     004B0ED2       9C                             PUSHFD
	--*/

	/*addr_tmp = scan_exe_.ScanAddr("0F6F0E0F6F6E??0FEF070FEF67??0F7F0F0F7F6F", 0x47);
	assert(addr_tmp);
	assert(*(pt_byte*)addr_tmp == 0x5f);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_2  " << addr_tmp << FC_OFstream_EndlLog;*/

	/*++
	收包CALL3
	00F9B281    0F6F06          MOVQ MM0,QWORD PTR DS:[ESI]
	00F9B284    66:0F7F4C24 10  MOVQ QWORD PTR SS:[ESP+0x10],MM1
	00F9B28A    66:0F7F4424 20  MOVQ QWORD PTR SS:[ESP+0x20],MM0
	00F9B290    E8 8BF8FFFF     CALL fifazf.00F9AB20
	00F9B295    8B1C24          MOV EBX,DWORD PTR SS:[ESP]
	00F9B298    8B5424 04       MOV EDX,DWORD PTR SS:[ESP+0x4]
	00F9B29C    66:0FEF4424 10  PXOR MM0,QWORD PTR SS:[ESP+0x10]
	00F9B2A2    66:0F6F4C24 20  MOVQ MM1,QWORD PTR SS:[ESP+0x20]
	00F9B2A8    F3:             PREFIX REP:
	00F9B2A9    0F7F0433        MOVQ QWORD PTR DS:[EBX+ESI],MM0
	00F9B2AD    8D76 10         LEA ESI,DWORD PTR DS:[ESI+0x10]
	00F9B2B0    81EF 10000000   SUB EDI,0x10
	00F9B2B6  ^ 0F83 C4FFFFFF   JNB fifazf.00F9B280
	00F9B2BC    8B5C24 08       MOV EBX,DWORD PTR SS:[ESP+0x8]
	00F9B2C0    8B6424 30       MOV ESP,DWORD PTR SS:[ESP+0x30]
	00F9B2C4    F3:             PREFIX REP:
	00F9B2C5    0F7F0B          MOVQ QWORD PTR DS:[EBX],MM1
	00F9B2C8    5F              POP EDI
	00F9B2C9    5E              POP ESI
	00F9B2CA    5B              POP EBX
	00F9B2CB    5D              POP EBP
	00F9B2CC    C3              RETN

	zhp 2016/8/9 14:27 防止特征码找不到
	$-1      004B2200       F3:                            PREFIX REP:                              ; 多余的前缀
	$ ==>    004B2201       0F6F06                         MOVQ MM0, QWORD PTR DS:[ESI]
	$+3      004B2204       66:0F7F4C24 10                 MOVQ QWORD PTR SS:[ESP+10], MM1
	$+9      004B220A       66:0F7F4424 20                 MOVQ QWORD PTR SS:[ESP+20], MM0
	$+F      004B2210       E8 8BF8FFFF                    CALL fifazf.004B1AA0
	$+14     004B2215       8B1C24                         MOV EBX, DWORD PTR SS:[ESP]              ; ntdll.770DFD91
	$+17     004B2218       8B5424 04                      MOV EDX, DWORD PTR SS:[ESP+4]            ; KERNELBA.76B13BC8
	$+1B     004B221C       66:0FEF4424 10                 PXOR MM0, QWORD PTR SS:[ESP+10]
	$+21     004B2222       66:0F6F4C24 20                 MOVQ MM1, QWORD PTR SS:[ESP+20]
	$+27     004B2228       F3:                            PREFIX REP:                              ; 多余的前缀
	$+28     004B2229       0F7F0433                       MOVQ QWORD PTR DS:[EBX+ESI], MM0
	$+2C     004B222D       8D76 10                        LEA ESI, DWORD PTR DS:[ESI+10]
	$+2F     004B2230       81EF 10000000                  SUB EDI, 10
	$+35     004B2236     ^ 0F83 C4FFFFFF                  JNB fifazf.004B2200
	$+3B     004B223C       8B5C24 08                      MOV EBX, DWORD PTR SS:[ESP+8]
	$+3F     004B2240       8B6424 30                      MOV ESP, DWORD PTR SS:[ESP+30]
	$+43     004B2244       F3:                            PREFIX REP:                              ; 多余的前缀
	$+44     004B2245       0F7F0B                         MOVQ QWORD PTR DS:[EBX], MM1
	$+47     004B2248       5F                             POP EDI                                  ; ntdll.770DFD91
	$+48     004B2249       5E                             POP ESI                                  ; ntdll.770DFD91
	$+49     004B224A       5B                             POP EBX                                  ; ntdll.770DFD91
	$+4A     004B224B       5D                             POP EBP                                  ; ntdll.770DFD91
	$+4B     004B224C       C3                             RETN
	$+4C     004B224D       CC                             INT3
	$+4D     004B224E       CC                             INT3
	$+4E     004B224F       CC                             INT3
	$+4F     004B2250       B8 14000000                    MOV EAX, 14
	$+54     004B2255       E8 C6B41600                    CALL fifazf.0061D720
	$+59     004B225A       8B4424 28                      MOV EAX, DWORD PTR SS:[ESP+28]
	$+5D     004B225E       53                             PUSH EBX
	$+5E     004B225F       8B5C24 24                      MOV EBX, DWORD PTR SS:[ESP+24]
	$+62     004B2263       55                             PUSH EBP
	$+63     004B2264       56                             PUSH ESI
	$+64     004B2265       8B7424 24                      MOV ESI, DWORD PTR SS:[ESP+24]
	$+68     004B2269       57                             PUSH EDI
	$+69     004B226A       8B7C24 2C                      MOV EDI, DWORD PTR SS:[ESP+2C]
	$+6D     004B226E       8BE8                           MOV EBP, EAX
	$+6F     004B2270       83FB 10                        CMP EBX, 10
	$+72     004B2273       72 58                          JB SHORT fifazf.004B22CD
	$+74     004B2275       C1EB 04                        SHR EBX, 4
	$+77     004B2278       895C24 10                      MOV DWORD PTR SS:[ESP+10], EBX
	$+7B     004B227C       8D6424 00                      LEA ESP, DWORD PTR SS:[ESP]
	$+7F     004B2280       8BCF                           MOV ECX, EDI
	$+81     004B2282       2BEE                           SUB EBP, ESI
	$+83     004B2284       8BC6                           MOV EAX, ESI
	$+85     004B2286       2BCE                           SUB ECX, ESI
	$+87     004B2288       BA 04000000                    MOV EDX, 4
	$+8C     004B228D       8D49 00                        LEA ECX, DWORD PTR DS:[ECX]
	$+8F     004B2290       8B1C28                         MOV EBX, DWORD PTR DS:[EAX+EBP]
	$+92     004B2293       3318                           XOR EBX, DWORD PTR DS:[EAX]
	$+94     004B2295       83C0 04                        ADD EAX, 4
	$+97     004B2298       83EA 01                        SUB EDX, 1
	$+9A     004B229B       895C01 FC                      MOV DWORD PTR DS:[ECX+EAX-4], EBX
	$+9E     004B229F     ^ 75 EF                          JNZ SHORT fifazf.004B2290
	$+A0     004B22A1       8B4424 34                      MOV EAX, DWORD PTR SS:[ESP+34]
	$+A4     004B22A5       50                             PUSH EAX
	$+A5     004B22A6       57                             PUSH EDI
	$+A6     004B22A7       57                             PUSH EDI
	$+A7     004B22A8       FF5424 48                      CALL DWORD PTR SS:[ESP+48]
	$+AB     004B22AC       8B5C24 3C                      MOV EBX, DWORD PTR SS:[ESP+3C]           ; KERNELBA.76B13B9C
	$+AF     004B22B0       8BEF                           MOV EBP, EDI
	$+B1     004B22B2       83EB 10                        SUB EBX, 10
	$+B4     004B22B5       83C4 0C                        ADD ESP, 0C
	$+B7     004B22B8       83C6 10                        ADD ESI, 10
	$+BA     004B22BB       83C7 10                        ADD EDI, 10
	$+BD     004B22BE       836C24 10 01                   SUB DWORD PTR SS:[ESP+10], 1
	$+C2     004B22C3       895C24 30                      MOV DWORD PTR SS:[ESP+30], EBX
	$+C6     004B22C7     ^ 75 B7                          JNZ SHORT fifazf.004B2280
	$+C8     004B22C9       8B4424 38                      MOV EAX, DWORD PTR SS:[ESP+38]
	$+CC     004B22CD       85DB                           TEST EBX, EBX
	$+CE     004B22CF       0F84 F7000000                  JE fifazf.004B23CC
	$+D4     004B22D5       8BCF                           MOV ECX, EDI
	$+D6     004B22D7       2BCE                           SUB ECX, ESI
	$+D8     004B22D9       894C24 14                      MOV DWORD PTR SS:[ESP+14], ECX
	$+DC     004B22DD       B9 01000000                    MOV ECX, 1
	$+E1     004B22E2       2BCE                           SUB ECX, ESI
	$+E3     004B22E4       894C24 18                      MOV DWORD PTR SS:[ESP+18], ECX
	$+E7     004B22E8       B9 02000000                    MOV ECX, 2
	$+EC     004B22ED       2BCE                           SUB ECX, ESI
	$+EE     004B22EF       894C24 1C                      MOV DWORD PTR SS:[ESP+1C], ECX
	$+F2     004B22F3       8BD5                           MOV EDX, EBP
	$+F4     004B22F5       2BD6                           SUB EDX, ESI
	$+F6     004B22F7       B9 03000000                    MOV ECX, 3
	$+FB     004B22FC       33C0                           XOR EAX, EAX
	$+FD     004B22FE       2BCE                           SUB ECX, ESI
	$+FF     004B2300       895424 10                      MOV DWORD PTR SS:[ESP+10], EDX
	$+103    004B2304       894C24 20                      MOV DWORD PTR SS:[ESP+20], ECX
	$+107    004B2308       EB 0A                          JMP SHORT fifazf.004B2314
	$+109    004B230A       8D9B 00000000                  LEA EBX, DWORD PTR DS:[EBX]
	$+10F    004B2310       8B5424 10                      MOV EDX, DWORD PTR SS:[ESP+10]
	$+113    004B2314       3BC3                           CMP EAX, EBX
	$+115    004B2316       73 6A                          JNB SHORT fifazf.004B2382
	$+117    004B2318       8B5C24 14                      MOV EBX, DWORD PTR SS:[ESP+14]
	$+11B    004B231C       8D0C30                         LEA ECX, DWORD PTR DS:[EAX+ESI]
	$+11E    004B231F       8A140A                         MOV DL, BYTE PTR DS:[EDX+ECX]
	$+121    004B2322       3211                           XOR DL, BYTE PTR DS:[ECX]
	$+123    004B2324       88140B                         MOV BYTE PTR DS:[EBX+ECX], DL
	$+126    004B2327       8B5424 18                      MOV EDX, DWORD PTR SS:[ESP+18]
	$+12A    004B232B       8B5C24 30                      MOV EBX, DWORD PTR SS:[ESP+30]
	$+12E    004B232F       03D1                           ADD EDX, ECX
	$+130    004B2331       3BD3                           CMP EDX, EBX
	$+132    004B2333       73 42                          JNB SHORT fifazf.004B2377
	$+134    004B2335       8A5428 01                      MOV DL, BYTE PTR DS:[EAX+EBP+1]
	$+138    004B2339       325430 01                      XOR DL, BYTE PTR DS:[EAX+ESI+1]
	$+13C    004B233D       885438 01                      MOV BYTE PTR DS:[EAX+EDI+1], DL
	$+140    004B2341       8B5424 1C                      MOV EDX, DWORD PTR SS:[ESP+1C]
	$+144    004B2345       03D1                           ADD EDX, ECX
	$+146    004B2347       3BD3                           CMP EDX, EBX
	$+148    004B2349       73 2F                          JNB SHORT fifazf.004B237A
	$+14A    004B234B       8A5430 02                      MOV DL, BYTE PTR DS:[EAX+ESI+2]
	$+14E    004B234F       325428 02                      XOR DL, BYTE PTR DS:[EAX+EBP+2]
	$+152    004B2353       885438 02                      MOV BYTE PTR DS:[EAX+EDI+2], DL
	$+156    004B2357       8B5424 20                      MOV EDX, DWORD PTR SS:[ESP+20]
	$+15A    004B235B       03D1                           ADD EDX, ECX
	$+15C    004B235D       3BD3                           CMP EDX, EBX
	$+15E    004B235F       73 1E                          JNB SHORT fifazf.004B237F
	$+160    004B2361       8A4C28 03                      MOV CL, BYTE PTR DS:[EAX+EBP+3]
	$+164    004B2365       324C30 03                      XOR CL, BYTE PTR DS:[EAX+ESI+3]
	$+168    004B2369       83C0 04                        ADD EAX, 4
	$+16B    004B236C       884C38 FF                      MOV BYTE PTR DS:[EAX+EDI-1], CL
	$+16F    004B2370       83F8 10                        CMP EAX, 10
	$+172    004B2373     ^ 72 9B                          JB SHORT fifazf.004B2310
	$+174    004B2375       EB 27                          JMP SHORT fifazf.004B239E
	$+176    004B2377       40                             INC EAX
	$+177    004B2378       EB 08                          JMP SHORT fifazf.004B2382
	$+179    004B237A       83C0 02                        ADD EAX, 2
	$+17C    004B237D       EB 03                          JMP SHORT fifazf.004B2382
	$+17E    004B237F       83C0 03                        ADD EAX, 3
	$+181    004B2382       83F8 10                        CMP EAX, 10
	$+184    004B2385       73 17                          JNB SHORT fifazf.004B239E
	$+186    004B2387       BA 10000000                    MOV EDX, 10
	$+18B    004B238C       2BEF                           SUB EBP, EDI
	$+18D    004B238E       8D0C38                         LEA ECX, DWORD PTR DS:[EAX+EDI]
	$+190    004B2391       2BD0                           SUB EDX, EAX
	$+192    004B2393       8A0429                         MOV AL, BYTE PTR DS:[ECX+EBP]
	$+195    004B2396       8801                           MOV BYTE PTR DS:[ECX], AL
	$+197    004B2398       41                             INC ECX
	$+198    004B2399       83EA 01                        SUB EDX, 1
	$+19B    004B239C     ^ 75 F5                          JNZ SHORT fifazf.004B2393
	$+19D    004B239E       8B4C24 34                      MOV ECX, DWORD PTR SS:[ESP+34]
	$+1A1    004B23A2       51                             PUSH ECX
	$+1A2    004B23A3       57                             PUSH EDI
	$+1A3    004B23A4       57                             PUSH EDI
	$+1A4    004B23A5       FF5424 48                      CALL DWORD PTR SS:[ESP+48]
	$+1A8    004B23A9       83C4 0C                        ADD ESP, 0C
	$+1AB    004B23AC       8BEF                           MOV EBP, EDI
	$+1AD    004B23AE       83FB 10                        CMP EBX, 10
	$+1B0    004B23B1       76 15                          JBE SHORT fifazf.004B23C8
	$+1B2    004B23B3       83EB 10                        SUB EBX, 10
	$+1B5    004B23B6       83C6 10                        ADD ESI, 10
	$+1B8    004B23B9       83C7 10                        ADD EDI, 10
	$+1BB    004B23BC       895C24 30                      MOV DWORD PTR SS:[ESP+30], EBX
	$+1BF    004B23C0       85DB                           TEST EBX, EBX
	$+1C1    004B23C2     ^ 0F85 0DFFFFFF                  JNZ fifazf.004B22D5
	$+1C7    004B23C8       8B4424 38                      MOV EAX, DWORD PTR SS:[ESP+38]
	$+1CB    004B23CC       8B55 00                        MOV EDX, DWORD PTR SS:[EBP]
	$+1CE    004B23CF       8910                           MOV DWORD PTR DS:[EAX], EDX
	$+1D0    004B23D1       8B4D 04                        MOV ECX, DWORD PTR SS:[EBP+4]            ; fifazf.0063AB39
	$+1D3    004B23D4       8948 04                        MOV DWORD PTR DS:[EAX+4], ECX
	$+1D6    004B23D7       8B55 08                        MOV EDX, DWORD PTR SS:[EBP+8]
	$+1D9    004B23DA       5F                             POP EDI                                  ; ntdll.770DFD91
	$+1DA    004B23DB       8950 08                        MOV DWORD PTR DS:[EAX+8], EDX
	$+1DD    004B23DE       8B4D 0C                        MOV ECX, DWORD PTR SS:[EBP+C]
	$+1E0    004B23E1       5E                             POP ESI                                  ; ntdll.770DFD91
	$+1E1    004B23E2       5D                             POP EBP                                  ; ntdll.770DFD91
	$+1E2    004B23E3       8948 0C                        MOV DWORD PTR DS:[EAX+C], ECX
	$+1E5    004B23E6       5B                             POP EBX                                  ; ntdll.770DFD91
	$+1E6    004B23E7       83C4 14                        ADD ESP, 14
	$+1E9    004B23EA       C3                             RETN
	--*/
	/*addr_tmp = scan_exe_.ScanAddr("0F6F06660F7F4C24??660F7F4424??E8", 0x47);
	assert(addr_tmp);
	assert(*(pt_byte*)addr_tmp == 0x5f);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_3  " << addr_tmp << FC_OFstream_EndlLog;*/
	
	/*++
	动画特征
	00B89B1F    8B00            MOV EAX,DWORD PTR DS:[EAX]
	00B89B21    3977 14         CMP DWORD PTR DS:[EDI+0x14],ESI
	00B89B24    72 04           JB SHORT fifazf.00B89B2A
	00B89B26    8B0F            MOV ECX,DWORD PTR DS:[EDI]
	00B89B28    EB 02           JMP SHORT fifazf.00B89B2C
	00B89B2A    8BCF            MOV ECX,EDI
	00B89B2C    50              PUSH EAX
	00B89B2D    51              PUSH ECX
	00B89B2E    68 E0492A01     PUSH fifazf.012A49E0                     ; ASCII "[GfxManager] ASFunc::Invoke : %s, %s\n"
	00B89B33    E8 B864C0FF     CALL fifazf.0078FFF0

	8B003977??72??8B0FEB??8BCF505168????????E8

	$-C      00985A05        8378 14 10      CMP DWORD PTR DS:[EAX+0x14],0x10
	$-8      00985A09        72 02           JB SHORT fifazf.00985A0D
	$-6      00985A0B        8B00            MOV EAX,DWORD PTR DS:[EAX]
	$-4      00985A0D        837F 14 10      CMP DWORD PTR DS:[EDI+0x14],0x10
	$ ==>    00985A11        72 04           JB SHORT fifazf.00985A17
	$+2      00985A13        8B0F            MOV ECX,DWORD PTR DS:[EDI]
	$+4      00985A15        EB 02           JMP SHORT fifazf.00985A19
	$+6      00985A17        8BCF            MOV ECX,EDI
	$+8      00985A19        50              PUSH EAX
	$+9      00985A1A        51              PUSH ECX
	$+A      00985A1B        68 285B5301     PUSH fifazf.01535B28                     ; ASCII "[GfxManager] ASFunc::Invoke : %s, %s\n"
	$+F      00985A20        E8 CBFC2300     CALL fifazf.00BC56F0
	$+14     00985A25        83C4 0C         ADD ESP,0xC
	$+17     00985A28        83BD 04FFFFFF 1>CMP DWORD PTR SS:[EBP-0xFC],0x10
	$+1E     00985A2F        72 0E           JB SHORT fifazf.00985A3F
	$+20     00985A31        FFB5 F0FEFFFF   PUSH DWORD PTR SS:[EBP-0x110]


	72??8B??EB??8B??????68????????E8

	//by zhp 2016/5/2 14:17
	0096D380               55				PUSH EBP
	0096D381               8BEC				MOV EBP, ESP
	0096D383               83EC 1C			SUB ESP, 0x1C
	0096D386               A1 60737001		MOV EAX, DWORD PTR DS:[0x1707360]
	0096D38B               33C5				XOR EAX, EBP
	0096D38D               8945 FC			MOV DWORD PTR SS:[EBP-0x4], EAX
	0096D390               8B01				MOV EAX, DWORD PTR DS:[ECX]              ; fifazf.01538BF0
	0096D392               8B55 08			MOV EDX, DWORD PTR SS:[EBP+0x8]
	0096D395               56				PUSH ESI
	0096D396               8D75 E4			LEA ESI, DWORD PTR SS:[EBP-0x1C]
	0096D399               C745 F4 00000000	MOV DWORD PTR SS:[EBP-0xC], 0x0
	0096D3A0               56				PUSH ESI
	0096D3A1               6A 00			PUSH 0x0
	0096D3A3               68 60908301		PUSH fifazf.01839060
	0096D3A8               52				PUSH EDX                                 ; fifazf.01538BF0
	0096D3A9               FF50 78			CALL NEAR DWORD PTR DS:[EAX+0x78]        ; fifazf.0153E870
	0096D3AC               8B4D F4			MOV ECX, DWORD PTR SS:[EBP-0xC]
	0096D3AF               5E				POP ESI
	0096D3B0               85C9				TEST ECX, ECX
	0096D3B2               74 11			JE SHORT fifazf.0096D3C5
	0096D3B4               8B11				MOV EDX, DWORD PTR DS:[ECX]              ; fifazf.01538BF0
	0096D3B6               8D45 E4			LEA EAX, DWORD PTR SS:[EBP-0x1C]
	0096D3B9               3BC8				CMP ECX, EAX
	0096D3BB               0F95C0			SETNE AL
	0096D3BE               0FB6C0			MOVZX EAX, AL
	0096D3C1               50				PUSH EAX
	0096D3C2               FF52 10			CALL NEAR DWORD PTR DS:[EDX+0x10]        ; fifazf.01F4E7B8
	0096D3C5               8B4D FC			MOV ECX, DWORD PTR SS:[EBP-0x4]
	0096D3C8               33CD				XOR ECX, EBP
	0096D3CA               E8 BD2CD0FF		CALL fifazf.0067008C
	0096D3CF               8BE5				MOV ESP, EBP
	0096D3D1               5D				POP EBP
	0096D3D2               C2 0400			RETN 0x4

	zhp 2016/8/9 14:29 防止特征码找不到
	$ ==>    009617A0       55                             PUSH EBP
	$+1      009617A1       8BEC                           MOV EBP, ESP
	$+3      009617A3       83EC 1C                        SUB ESP, 1C
	$+6      009617A6       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$+B      009617AB       33C5                           XOR EAX, EBP
	$+D      009617AD       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+10     009617B0       8B01                           MOV EAX, DWORD PTR DS:[ECX]
	$+12     009617B2       8B55 08                        MOV EDX, DWORD PTR SS:[EBP+8]
	$+15     009617B5       56                             PUSH ESI
	$+16     009617B6       8D75 E4                        LEA ESI, DWORD PTR SS:[EBP-1C]
	$+19     009617B9       C745 F4 00000000               MOV DWORD PTR SS:[EBP-C], 0
	$+20     009617C0       56                             PUSH ESI
	$+21     009617C1       6A 00                          PUSH 0
	$+23     009617C3       68 F0E28401                    PUSH fifazf.0184E2F0
	$+28     009617C8       52                             PUSH EDX
	$+29     009617C9       FF50 78                        CALL DWORD PTR DS:[EAX+78]
	$+2C     009617CC       8B4D F4                        MOV ECX, DWORD PTR SS:[EBP-C]            ; KERNELBA.76B36FF0
	$+2F     009617CF       5E                             POP ESI                                  ; ntdll.770DFD91
	$+30     009617D0       85C9                           TEST ECX, ECX
	$+32     009617D2       74 11                          JE SHORT fifazf.009617E5
	$+34     009617D4       8B11                           MOV EDX, DWORD PTR DS:[ECX]
	$+36     009617D6       8D45 E4                        LEA EAX, DWORD PTR SS:[EBP-1C]
	$+39     009617D9       3BC8                           CMP ECX, EAX
	$+3B     009617DB       0F95C0                         SETNE AL
	$+3E     009617DE       0FB6C0                         MOVZX EAX, AL
	$+41     009617E1       50                             PUSH EAX
	$+42     009617E2       FF52 10                        CALL DWORD PTR DS:[EDX+10]
	$+45     009617E5       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+48     009617E8       33CD                           XOR ECX, EBP
	$+4A     009617EA       E8 6DBECBFF                    CALL fifazf.0061D65C
	$+4F     009617EF       8BE5                           MOV ESP, EBP
	$+51     009617F1       5D                             POP EBP                                  ; ntdll.770DFD91
	$+52     009617F2       C2 0400                        RETN 4
	$+55     009617F5       CC                             INT3
	$+56     009617F6       CC                             INT3
	$+57     009617F7       CC                             INT3
	$+58     009617F8       CC                             INT3
	$+59     009617F9       CC                             INT3
	$+5A     009617FA       CC                             INT3
	$+5B     009617FB       CC                             INT3
	$+5C     009617FC       CC                             INT3
	$+5D     009617FD       CC                             INT3
	$+5E     009617FE       CC                             INT3
	$+5F     009617FF       CC                             INT3
	$+60     00961800       55                             PUSH EBP
	$+61     00961801       8BEC                           MOV EBP, ESP
	$+63     00961803       83EC 54                        SUB ESP, 54
	$+66     00961806       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$+6B     0096180B       33C5                           XOR EAX, EBP
	$+6D     0096180D       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+70     00961810       8B45 14                        MOV EAX, DWORD PTR SS:[EBP+14]
	$+73     00961813       8B55 10                        MOV EDX, DWORD PTR SS:[EBP+10]           ; fifazf.01F62FC2
	$+76     00961816       53                             PUSH EBX
	$+77     00961817       56                             PUSH ESI
	$+78     00961818       8B75 0C                        MOV ESI, DWORD PTR SS:[EBP+C]
	$+7B     0096181B       8BD9                           MOV EBX, ECX
	$+7D     0096181D       57                             PUSH EDI
	$+7E     0096181E       8B7D 08                        MOV EDI, DWORD PTR SS:[EBP+8]
	$+81     00961821       8D4D AC                        LEA ECX, DWORD PTR SS:[EBP-54]
	$+84     00961824       50                             PUSH EAX
	$+85     00961825       52                             PUSH EDX
	$+86     00961826       56                             PUSH ESI
	$+87     00961827       57                             PUSH EDI
	$+88     00961828       E8 E33DFEFF                    CALL fifazf.00945610
	$+8D     0096182D       8DBB 04010000                  LEA EDI, DWORD PTR DS:[EBX+104]
	$+93     00961833       8BF0                           MOV ESI, EAX
	$+95     00961835       57                             PUSH EDI
	$+96     00961836       FF15 D4914E01                  CALL DWORD PTR DS:[14E91D4]              ; ntdll.RtlEnterCriticalSection
	$+9C     0096183C       56                             PUSH ESI
	$+9D     0096183D       8D4F 18                        LEA ECX, DWORD PTR DS:[EDI+18]
	$+A0     00961840       E8 4B270500                    CALL fifazf.009B3F90
	$+A5     00961845       57                             PUSH EDI
	$+A6     00961846       FF15 D8914E01                  CALL DWORD PTR DS:[14E91D8]              ; ntdll.RtlLeaveCriticalSection
	$+AC     0096184C       8D4D AC                        LEA ECX, DWORD PTR SS:[EBP-54]
	$+AF     0096184F       E8 FC7EFEFF                    CALL fifazf.00949750
	$+B4     00961854       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+B7     00961857       5F                             POP EDI                                  ; ntdll.770DFD91
	$+B8     00961858       5E                             POP ESI                                  ; ntdll.770DFD91
	$+B9     00961859       33CD                           XOR ECX, EBP
	$+BB     0096185B       5B                             POP EBX                                  ; ntdll.770DFD91
	$+BC     0096185C       E8 FBBDCBFF                    CALL fifazf.0061D65C
	$+C1     00961861       8BE5                           MOV ESP, EBP
	$+C3     00961863       5D                             POP EBP                                  ; ntdll.770DFD91
	$+C4     00961864       C2 1000                        RETN 10
	--*/
	addr_tmp = scan_exe_.ScanAddr("558b??83????a1????????33??89????8b??8b????568d????c7????????????566a??68????????52ff????8b????5e", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_ANIMATION_FEATURE  " << addr_tmp << FC_OFstream_EndlLog;

	/*这个函数是一个顶层命令处理函数，游戏中的功能都是以命令的形式组织的，通过字符串来代表消息ID。
	类似窗口的消息机制，顶层窗口处理所有消息，把感兴趣的自己处理掉，不感兴趣的转发给子窗口的消息过程去处理。只不过这里的消息ID是以字符串的形式呈现的。
	这里的命令函数处理所有事件，包括鼠标离开(onMouseLeave)、离开踢球状态(exitPlayState)、进入踢球状态(enterPlayState)、按ESC设置游戏选项(ShowOption)等。
	这应该就是一个万能断点了。
	命令函数是this call，第一个参数就是命令字符串。
	*/
	/*
	2016/5/2 19:58。
	$ ==>    0096D3E0      55                    PUSH EBP                                 ; 顶层命令函数
	$+1      0096D3E1      8BEC                  MOV EBP, ESP
	$+3      0096D3E3      83EC 54               SUB ESP, 0x54
	$+6      0096D3E6      A1 60737001           MOV EAX, DWORD PTR DS:[0x1707360]
	$+B      0096D3EB      33C5                  XOR EAX, EBP
	$+D      0096D3ED      8945 FC               MOV DWORD PTR SS:[EBP-0x4], EAX
	$+10     0096D3F0      8B45 14               MOV EAX, DWORD PTR SS:[EBP+0x14]
	$+13     0096D3F3      8B55 10               MOV EDX, DWORD PTR SS:[EBP+0x10]         ; fifazf.00992294
	$+16     0096D3F6      53                    PUSH EBX
	$+17     0096D3F7      56                    PUSH ESI
	$+18     0096D3F8      8B75 0C               MOV ESI, DWORD PTR SS:[EBP+0xC]
	$+1B     0096D3FB      8BD9                  MOV EBX, ECX
	$+1D     0096D3FD      57                    PUSH EDI
	$+1E     0096D3FE      8B7D 08               MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+21     0096D401      8D4D AC               LEA ECX, DWORD PTR SS:[EBP-0x54]
	$+24     0096D404      50                    PUSH EAX
	$+25     0096D405      52                    PUSH EDX
	$+26     0096D406      56                    PUSH ESI
	$+27     0096D407      57                    PUSH EDI
	$+28     0096D408      E8 7352FEFF           CALL fifazf.00952680
	$+2D     0096D40D      8DBB DC000000         LEA EDI, DWORD PTR DS:[EBX+0xDC]
	$+33     0096D413      8BF0                  MOV ESI, EAX
	$+35     0096D415      57                    PUSH EDI
	$+36     0096D416      FF15 60534D01         CALL NEAR DWORD PTR DS:[0x14D5360]       ; ntdll_1.RtlEnterCriticalSection
	$+3C     0096D41C      56                    PUSH ESI
	$+3D     0096D41D      8D4F 18               LEA ECX, DWORD PTR DS:[EDI+0x18]
	$+40     0096D420      E8 2BFF0400           CALL fifazf.009BD350
	$+45     0096D425      57                    PUSH EDI
	$+46     0096D426      FF15 D4514D01         CALL NEAR DWORD PTR DS:[0x14D51D4]       ; ntdll_1.RtlLeaveCriticalSection
	$+4C     0096D42C      8D4D AC               LEA ECX, DWORD PTR SS:[EBP-0x54]
	$+4F     0096D42F      E8 1C96FEFF           CALL fifazf.00956A50
	$+54     0096D434      8B4D FC               MOV ECX, DWORD PTR SS:[EBP-0x4]
	$+57     0096D437      5F                    POP EDI                                  ; 05615580
	$+58     0096D438      5E                    POP ESI                                  ; 05615580
	$+59     0096D439      33CD                  XOR ECX, EBP
	$+5B     0096D43B      5B                    POP EBX                                  ; ntdll_1.775CFD91
	$+5C     0096D43C      E8 4B2CD0FF           CALL fifazf.0067008C
	$+61     0096D441      8BE5                  MOV ESP, EBP
	$+63     0096D443      5D                    POP EBP                                  ; ntdll_1.775CFD91
	$+64     0096D444      C2 1000               RETN 0x10

	zhp 2016/8/9 14:30 防止特征码找不到
	$ ==>    00961800       55                             PUSH EBP
	$+1      00961801       8BEC                           MOV EBP, ESP
	$+3      00961803       83EC 54                        SUB ESP, 54
	$+6      00961806       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$+B      0096180B       33C5                           XOR EAX, EBP
	$+D      0096180D       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+10     00961810       8B45 14                        MOV EAX, DWORD PTR SS:[EBP+14]
	$+13     00961813       8B55 10                        MOV EDX, DWORD PTR SS:[EBP+10]           ; fifazf.01F62FC2
	$+16     00961816       53                             PUSH EBX
	$+17     00961817       56                             PUSH ESI
	$+18     00961818       8B75 0C                        MOV ESI, DWORD PTR SS:[EBP+C]
	$+1B     0096181B       8BD9                           MOV EBX, ECX
	$+1D     0096181D       57                             PUSH EDI
	$+1E     0096181E       8B7D 08                        MOV EDI, DWORD PTR SS:[EBP+8]
	$+21     00961821       8D4D AC                        LEA ECX, DWORD PTR SS:[EBP-54]
	$+24     00961824       50                             PUSH EAX
	$+25     00961825       52                             PUSH EDX
	$+26     00961826       56                             PUSH ESI
	$+27     00961827       57                             PUSH EDI
	$+28     00961828       E8 E33DFEFF                    CALL fifazf.00945610
	$+2D     0096182D       8DBB 04010000                  LEA EDI, DWORD PTR DS:[EBX+104]
	$+33     00961833       8BF0                           MOV ESI, EAX
	$+35     00961835       57                             PUSH EDI
	$+36     00961836       FF15 D4914E01                  CALL DWORD PTR DS:[14E91D4]              ; ntdll.RtlEnterCriticalSection
	$+3C     0096183C       56                             PUSH ESI
	$+3D     0096183D       8D4F 18                        LEA ECX, DWORD PTR DS:[EDI+18]
	$+40     00961840       E8 4B270500                    CALL fifazf.009B3F90
	$+45     00961845       57                             PUSH EDI
	$+46     00961846       FF15 D8914E01                  CALL DWORD PTR DS:[14E91D8]              ; ntdll.RtlLeaveCriticalSection
	$+4C     0096184C       8D4D AC                        LEA ECX, DWORD PTR SS:[EBP-54]
	$+4F     0096184F       E8 FC7EFEFF                    CALL fifazf.00949750
	$+54     00961854       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+57     00961857       5F                             POP EDI                                  ; ntdll.770DFD91
	$+58     00961858       5E                             POP ESI                                  ; ntdll.770DFD91
	$+59     00961859       33CD                           XOR ECX, EBP
	$+5B     0096185B       5B                             POP EBX                                  ; ntdll.770DFD91
	$+5C     0096185C       E8 FBBDCBFF                    CALL fifazf.0061D65C
	$+61     00961861       8BE5                           MOV ESP, EBP
	$+63     00961863       5D                             POP EBP                                  ; ntdll.770DFD91
	$+64     00961864       C2 1000                        RETN 10
	$+67     00961867       CC                             INT3
	$+68     00961868       CC                             INT3
	$+69     00961869       CC                             INT3
	$+6A     0096186A       CC                             INT3
	$+6B     0096186B       CC                             INT3
	$+6C     0096186C       CC                             INT3
	$+6D     0096186D       CC                             INT3
	$+6E     0096186E       CC                             INT3
	$+6F     0096186F       CC                             INT3
	$+70     00961870       55                             PUSH EBP
	$+71     00961871       8BEC                           MOV EBP, ESP
	$+73     00961873       83EC 3C                        SUB ESP, 3C
	$+76     00961876       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$+7B     0096187B       33C5                           XOR EAX, EBP
	$+7D     0096187D       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+80     00961880       8B45 08                        MOV EAX, DWORD PTR SS:[EBP+8]
	$+83     00961883       8D4D CC                        LEA ECX, DWORD PTR SS:[EBP-34]
	$+86     00961886       53                             PUSH EBX
	$+87     00961887       56                             PUSH ESI
	$+88     00961888       8B75 0C                        MOV ESI, DWORD PTR SS:[EBP+C]
	$+8B     0096188B       57                             PUSH EDI
	$+8C     0096188C       6A FF                          PUSH -1
	$+8E     0096188E       6A 00                          PUSH 0
	$+90     00961890       50                             PUSH EAX
	$+91     00961891       C745 E0 0F000000               MOV DWORD PTR SS:[EBP-20], 0F
	$+98     00961898       C745 DC 00000000               MOV DWORD PTR SS:[EBP-24], 0
	$+9F     0096189F       C645 CC 00                     MOV BYTE PTR SS:[EBP-34], 0
	$+A3     009618A3       E8 A8F2C2FF                    CALL fifazf.00590B50
	$+A8     009618A8       8B4E 10                        MOV ECX, DWORD PTR DS:[ESI+10]
	$+AB     009618AB       C745 F4 00000000               MOV DWORD PTR SS:[EBP-C], 0
	$+B2     009618B2       85C9                           TEST ECX, ECX
	$+B4     009618B4       75 05                          JNZ SHORT fifazf.009618BB
	$+B6     009618B6       894D F4                        MOV DWORD PTR SS:[EBP-C], ECX
	$+B9     009618B9       EB 13                          JMP SHORT fifazf.009618CE
	$+BB     009618BB       8B01                           MOV EAX, DWORD PTR DS:[ECX]
	$+BD     009618BD       3BCE                           CMP ECX, ESI
	$+BF     009618BF       75 06                          JNZ SHORT fifazf.009618C7
	$+C1     009618C1       8D55 E4                        LEA EDX, DWORD PTR SS:[EBP-1C]
	$+C4     009618C4       52                             PUSH EDX
	$+C5     009618C5       EB 02                          JMP SHORT fifazf.009618C9
	$+C7     009618C7       6A 00                          PUSH 0
	$+C9     009618C9       FF10                           CALL DWORD PTR DS:[EAX]
	$+CB     009618CB       8945 F4                        MOV DWORD PTR SS:[EBP-C], EAX
	$+CE     009618CE       8B3D 7CDF7E01                  MOV EDI, DWORD PTR DS:[17EDF7C]
	$+D4     009618D4       6A 40                          PUSH 40
	$+D6     009618D6       E8 8528CAFF                    CALL fifazf.00604160
	$+DB     009618DB       8BF0                           MOV ESI, EAX
	$+DD     009618DD       83C4 04                        ADD ESP, 4
	$+E0     009618E0       85F6                           TEST ESI, ESI
	$+E2     009618E2       75 06                          JNZ SHORT fifazf.009618EA
	$+E4     009618E4       FF15 68944E01                  CALL DWORD PTR DS:[14E9468]              ; msvcp120.?_Xbad_alloc@std@@YAXXZ
	$+EA     009618EA       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+EC     009618EC       8D4E 04                        LEA ECX, DWORD PTR DS:[ESI+4]
	$+EF     009618EF       8906                           MOV DWORD PTR DS:[ESI], EAX
	$+F1     009618F1       85C9                           TEST ECX, ECX
	$+F3     009618F3       74 04                          JE SHORT fifazf.009618F9
	$+F5     009618F5       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+F7     009618F7       8901                           MOV DWORD PTR DS:[ECX], EAX
	$+F9     009618F9       8D4E 08                        LEA ECX, DWORD PTR DS:[ESI+8]
	$+FC     009618FC       85C9                           TEST ECX, ECX
	$+FE     009618FE       74 04                          JE SHORT fifazf.00961904
	$+100    00961900       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+102    00961902       8901                           MOV DWORD PTR DS:[ECX], EAX
	$+104    00961904       8D5E 10                        LEA EBX, DWORD PTR DS:[ESI+10]
	$+107    00961907       66:C746 0C 0000                MOV WORD PTR DS:[ESI+C], 0
	$+10D    0096190D       85DB                           TEST EBX, EBX
	$+10F    0096190F       74 0B                          JE SHORT fifazf.0096191C
	$+111    00961911       8D45 CC                        LEA EAX, DWORD PTR SS:[EBP-34]
	$+114    00961914       8BCB                           MOV ECX, EBX
	$+116    00961916       50                             PUSH EAX
	$+117    00961917       E8 447FFDFF                    CALL fifazf.00939860
	$+11C    0096191C       56                             PUSH ESI
	$+11D    0096191D       53                             PUSH EBX
	$+11E    0096191E       6A 00                          PUSH 0
	$+120    00961920       8D45 C4                        LEA EAX, DWORD PTR SS:[EBP-3C]
	$+123    00961923       8BCF                           MOV ECX, EDI
	$+125    00961925       50                             PUSH EAX
	$+126    00961926       E8 55F4FDFF                    CALL fifazf.00940D80
	$+12B    0096192B       8D4D CC                        LEA ECX, DWORD PTR SS:[EBP-34]
	$+12E    0096192E       E8 6D7CFEFF                    CALL fifazf.009495A0
	$+133    00961933       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+136    00961936       5F                             POP EDI                                  ; ntdll.770DFD91
	$+137    00961937       5E                             POP ESI                                  ; ntdll.770DFD91
	$+138    00961938       33CD                           XOR ECX, EBP
	$+13A    0096193A       5B                             POP EBX                                  ; ntdll.770DFD91
	$+13B    0096193B       E8 1CBDCBFF                    CALL fifazf.0061D65C
	$+140    00961940       8BE5                           MOV ESP, EBP
	$+142    00961942       5D                             POP EBP                                  ; ntdll.770DFD91
	$+143    00961943       C3                             RETN
	*/
	addr_tmp = scan_exe_.ScanAddr("558b??83????a1????????33??89????8b????8b????53568b????8b??578b????8d????50525657e8", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_TOP_COMMAND_FN  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	DX按键

	$-1B     >  8D85 D8FEFFFF   LEA EAX,DWORD PTR SS:[EBP-0x128]
	$-15     >  68 00010000     PUSH 0x100
	$-10     >  50              PUSH EAX
	$-F      >  E8 630746FF     CALL fifazf.005F6580
	$-A      >  8B3F            MOV EDI,DWORD PTR DS:[EDI]
	$-8      >  8B0F            MOV ECX,DWORD PTR DS:[EDI]
	$-6      >  8B41 24         MOV EAX,DWORD PTR DS:[ECX+0x24]
	$-3      >  83C4 08         ADD ESP,0x8
	$ ==>    >  8D95 D8FEFFFF   LEA EDX,DWORD PTR SS:[EBP-0x128]
	$+6      >  52              PUSH EDX
	$+7      >  68 00010000     PUSH 0x100
	$+C      >  57              PUSH EDI
	$+D      >  FFD0            CALL EAX
	$+F      >  85C0            TEST EAX,EAX
	$+11     >  0F85 23030000   JNZ fifazf.01196161

	8D????????????6800010000??????85C00F85

	$-20     00B829AD      68 00010000         PUSH 0x100
	$-1B     00B829B2      50                  PUSH EAX
	$-1A     00B829B3      8D85 FCFEFFFF       LEA EAX,DWORD PTR SS:[EBP-0x104]
	$-14     00B829B9      50                  PUSH EAX
	$-13     00B829BA      E8 5DDAAEFF         CALL fifazf.0067041C                     ; JMP 到 msvcr120.memset
	$-E      00B829BF      8B07                MOV EAX,DWORD PTR DS:[EDI]
	$-C      00B829C1      8D95 FCFEFFFF       LEA EDX,DWORD PTR SS:[EBP-0x104]
	$-6      00B829C7      83C4 0C             ADD ESP,0xC
	$-3      00B829CA      8B08                MOV ECX,DWORD PTR DS:[EAX]
	$-1      00B829CC      52                  PUSH EDX
	$ ==>    00B829CD      68 00010000         PUSH 0x100
	$+5      00B829D2      50                  PUSH EAX
	$+6      00B829D3      FF51 24             CALL DWORD PTR DS:[ECX+0x24]
	$+9      00B829D6      85C0                TEST EAX,EAX
	$+B      00B829D8      0F85 45030000       JNZ fifazf.00B82D23
	$+11     00B829DE      33FF                XOR EDI,EDI
	$+13     00B829E0      397B 08             CMP DWORD PTR DS:[EBX+0x8],EDI
	$+16     00B829E3      0F86 C1020000       JBE fifazf.00B82CAA

	zhp 2016/8/9 14:31 防止特征码找不到
	$-CD     00B81940       6A 00                          PUSH 0
	$-CB     00B81942       50                             PUSH EAX
	$-CA     00B81943       E8 04C2A9FF                    CALL fifazf.0061DB4C                     ; JMP 到 MSVCR120.memset
	$-C5     00B81948       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$-C3     00B8194A       83C4 0C                        ADD ESP, 0C
	$-C0     00B8194D       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$-BE     00B8194F       50                             PUSH EAX
	$-BD     00B81950       FF51 20                        CALL DWORD PTR DS:[ECX+20]
	$-BA     00B81953       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$-B8     00B81955       50                             PUSH EAX
	$-B7     00B81956       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$-B5     00B81958       FF51 08                        CALL DWORD PTR DS:[ECX+8]
	$-B2     00B8195B       C706 00000000                  MOV DWORD PTR DS:[ESI], 0
	$-AC     00B81961       8BB5 D0FDFFFF                  MOV ESI, DWORD PTR SS:[EBP-230]
	$-A6     00B81967       83C7 04                        ADD EDI, 4
	$-A3     00B8196A       46                             INC ESI
	$-A2     00B8196B       89B5 D0FDFFFF                  MOV DWORD PTR SS:[EBP-230], ESI
	$-9C     00B81971       3B73 30                        CMP ESI, DWORD PTR DS:[EBX+30]
	$-99     00B81974     ^ 0F8C 36FFFFFF                  JL fifazf.00B818B0
	$-93     00B8197A       B9 04000000                    MOV ECX, 4
	$-8E     00B8197F       33C0                           XOR EAX, EAX
	$-8C     00B81981       3843 2C                        CMP BYTE PTR DS:[EBX+2C], AL
	$-89     00B81984       0F45C1                         CMOVNE EAX, ECX
	$-86     00B81987       8985 D0FDFFFF                  MOV DWORD PTR SS:[EBP-230], EAX
	$-80     00B8198D       3B43 30                        CMP EAX, DWORD PTR DS:[EBX+30]
	$-7D     00B81990       0F8D CD030000                  JGE fifazf.00B81D63
	$-77     00B81996       83C0 1F                        ADD EAX, 1F
	$-74     00B81999       8D3483                         LEA ESI, DWORD PTR DS:[EBX+EAX*4]
	$-71     00B8199C       8BC3                           MOV EAX, EBX
	$-6F     00B8199E       89B5 CCFDFFFF                  MOV DWORD PTR SS:[EBP-234], ESI
	$-69     00B819A4       8B5E C0                        MOV EBX, DWORD PTR DS:[ESI-40]           ; KERNELBA.76B13BC8
	$-66     00B819A7       8B3E                           MOV EDI, DWORD PTR DS:[ESI]
	$-64     00B819A9       8B76 E0                        MOV ESI, DWORD PTR DS:[ESI-20]
	$-61     00B819AC       837E 04 01                     CMP DWORD PTR DS:[ESI+4], 1
	$-5D     00B819B0       0F85 3A030000                  JNZ fifazf.00B81CF0
	$-57     00B819B6       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$-55     00B819B8       50                             PUSH EAX
	$-54     00B819B9       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$-52     00B819BB       FF51 64                        CALL DWORD PTR DS:[ECX+64]
	$-4F     00B819BE       85C0                           TEST EAX, EAX
	$-4D     00B819C0       0F88 60030000                  JS fifazf.00B81D26
	$-47     00B819C6       8B85 D4FDFFFF                  MOV EAX, DWORD PTR SS:[EBP-22C]
	$-41     00B819CC       8B00                           MOV EAX, DWORD PTR DS:[EAX]
	$-3F     00B819CE       8946 0C                        MOV DWORD PTR DS:[ESI+C], EAX
	$-3C     00B819D1       8B43 04                        MOV EAX, DWORD PTR DS:[EBX+4]
	$-39     00B819D4       83E8 00                        SUB EAX, 0
	$-36     00B819D7       0F84 5F010000                  JE fifazf.00B81B3C
	$-30     00B819DD       83E8 02                        SUB EAX, 2
	$-2D     00B819E0       0F84 9B000000                  JE fifazf.00B81A81
	$-27     00B819E6       48                             DEC EAX
	$-26     00B819E7       0F85 FD020000                  JNZ fifazf.00B81CEA
	$-20     00B819ED       68 00010000                    PUSH 100
	$-1B     00B819F2       50                             PUSH EAX
	$-1A     00B819F3       8D85 FCFEFFFF                  LEA EAX, DWORD PTR SS:[EBP-104]
	$-14     00B819F9       50                             PUSH EAX
	$-13     00B819FA       E8 4DC1A9FF                    CALL fifazf.0061DB4C                     ; JMP 到 MSVCR120.memset
	$-E      00B819FF       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$-C      00B81A01       8D95 FCFEFFFF                  LEA EDX, DWORD PTR SS:[EBP-104]
	$-6      00B81A07       83C4 0C                        ADD ESP, 0C
	$-3      00B81A0A       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$-1      00B81A0C       52                             PUSH EDX
	$ ==>    00B81A0D       68 00010000                    PUSH 100
	$+5      00B81A12       50                             PUSH EAX
	$+6      00B81A13       FF51 24                        CALL DWORD PTR DS:[ECX+24]
	$+9      00B81A16       85C0                           TEST EAX, EAX
	$+B      00B81A18       0F85 45030000                  JNZ fifazf.00B81D63
	$+11     00B81A1E       33FF                           XOR EDI, EDI
	$+13     00B81A20       397B 08                        CMP DWORD PTR DS:[EBX+8], EDI
	$+16     00B81A23       0F86 C1020000                  JBE fifazf.00B81CEA
	$+1C     00B81A29       8DA424 00000000                LEA ESP, DWORD PTR SS:[ESP]
	$+23     00B81A30       8A8C3D FCFEFFFF                MOV CL, BYTE PTR SS:[EBP+EDI-104]
	$+2A     00B81A37       8B56 10                        MOV EDX, DWORD PTR DS:[ESI+10]
	$+2D     00B81A3A       80E1 80                        AND CL, 80
	$+30     00B81A3D       3BFA                           CMP EDI, EDX
	$+32     00B81A3F       73 24                          JNB SHORT fifazf.00B81A65
	$+34     00B81A41       8BD7                           MOV EDX, EDI
	$+36     00B81A43       B0 01                          MOV AL, 1
	$+38     00B81A45       C1EA 03                        SHR EDX, 3
	$+3B     00B81A48       0356 1C                        ADD EDX, DWORD PTR DS:[ESI+1C]
	$+3E     00B81A4B       84C9                           TEST CL, CL
	$+40     00B81A4D       8BCF                           MOV ECX, EDI
	$+42     00B81A4F       75 0B                          JNZ SHORT fifazf.00B81A5C
	$+44     00B81A51       83E1 07                        AND ECX, 7
	$+47     00B81A54       D2E0                           SHL AL, CL
	$+49     00B81A56       F6D0                           NOT AL
	$+4B     00B81A58       2002                           AND BYTE PTR DS:[EDX], AL
	$+4D     00B81A5A       EB 1A                          JMP SHORT fifazf.00B81A76
	$+4F     00B81A5C       83E1 07                        AND ECX, 7
	$+52     00B81A5F       D2E0                           SHL AL, CL
	$+54     00B81A61       0802                           OR BYTE PTR DS:[EDX], AL
	$+56     00B81A63       EB 11                          JMP SHORT fifazf.00B81A76
	$+58     00B81A65       8B46 14                        MOV EAX, DWORD PTR DS:[ESI+14]
	$+5B     00B81A68       03C2                           ADD EAX, EDX
	$+5D     00B81A6A       3BF8                           CMP EDI, EAX
	$+5F     00B81A6C       73 08                          JNB SHORT fifazf.00B81A76
	$+61     00B81A6E       8B46 20                        MOV EAX, DWORD PTR DS:[ESI+20]
	$+64     00B81A71       2BC2                           SUB EAX, EDX
	$+66     00B81A73       880C38                         MOV BYTE PTR DS:[EAX+EDI], CL
	$+69     00B81A76       47                             INC EDI
	$+6A     00B81A77       3B7B 08                        CMP EDI, DWORD PTR DS:[EBX+8]
	$+6D     00B81A7A     ^ 72 B4                          JB SHORT fifazf.00B81A30
	$+6F     00B81A7C       E9 69020000                    JMP fifazf.00B81CEA
	$+74     00B81A81       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+76     00B81A83       8D95 E8FEFFFF                  LEA EDX, DWORD PTR SS:[EBP-118]
	$+7C     00B81A89       0F57C0                         XORPS XMM0, XMM0
	$+7F     00B81A8C       C785 F8FEFFFF 00000000         MOV DWORD PTR SS:[EBP-108], 0
	$+89     00B81A96       52                             PUSH EDX
	$+8A     00B81A97       F3:                            PREFIX REP:                              ; 多余的前缀
	$+8B     00B81A98       0F7F85 E8FEFFFF                MOVQ QWORD PTR SS:[EBP-118], MM0
	$+92     00B81A9F       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$+94     00B81AA1       6A 14                          PUSH 14
	$+96     00B81AA3       50                             PUSH EAX
	$+97     00B81AA4       FF51 24                        CALL DWORD PTR DS:[ECX+24]
	$+9A     00B81AA7       85C0                           TEST EAX, EAX
	$+9C     00B81AA9       0F85 80000000                  JNZ fifazf.00B81B2F
	$+A2     00B81AAF       8B4E 24                        MOV ECX, DWORD PTR DS:[ESI+24]
	$+A5     00B81AB2       33FF                           XOR EDI, EDI
	$+A7     00B81AB4       8B85 E8FEFFFF                  MOV EAX, DWORD PTR SS:[EBP-118]
	$+AD     00B81ABA       8901                           MOV DWORD PTR DS:[ECX], EAX
	$+AF     00B81ABC       8B4E 24                        MOV ECX, DWORD PTR DS:[ESI+24]
	$+B2     00B81ABF       8B85 ECFEFFFF                  MOV EAX, DWORD PTR SS:[EBP-114]
	$+B8     00B81AC5       8941 04                        MOV DWORD PTR DS:[ECX+4], EAX
	$+BB     00B81AC8       8B4E 24                        MOV ECX, DWORD PTR DS:[ESI+24]
	$+BE     00B81ACB       8B85 F0FEFFFF                  MOV EAX, DWORD PTR SS:[EBP-110]
	$+C4     00B81AD1       8941 08                        MOV DWORD PTR DS:[ECX+8], EAX
	$+C7     00B81AD4       397B 08                        CMP DWORD PTR DS:[EBX+8], EDI
	$+CA     00B81AD7       0F86 0D020000                  JBE fifazf.00B81CEA
	$+D0     00B81ADD       8D49 00                        LEA ECX, DWORD PTR DS:[ECX]
	$+D3     00B81AE0       8B4E 10                        MOV ECX, DWORD PTR DS:[ESI+10]
	$+D6     00B81AE3       8A943D F4FEFFFF                MOV DL, BYTE PTR SS:[EBP+EDI-10C]
	$+DD     00B81AEA       3BF9                           CMP EDI, ECX
	$+DF     00B81AEC       73 25                          JNB SHORT fifazf.00B81B13
	$+E1     00B81AEE       8BCF                           MOV ECX, EDI
	$+E3     00B81AF0       B0 01                          MOV AL, 1
	$+E5     00B81AF2       83E1 07                        AND ECX, 7
	$+E8     00B81AF5       D2E0                           SHL AL, CL
	$+EA     00B81AF7       84D2                           TEST DL, DL
	$+EC     00B81AF9       8BD7                           MOV EDX, EDI
	$+EE     00B81AFB       75 0C                          JNZ SHORT fifazf.00B81B09
	$+F0     00B81AFD       C1EA 03                        SHR EDX, 3
	$+F3     00B81B00       F6D0                           NOT AL
	$+F5     00B81B02       0356 1C                        ADD EDX, DWORD PTR DS:[ESI+1C]
	$+F8     00B81B05       2002                           AND BYTE PTR DS:[EDX], AL
	$+FA     00B81B07       EB 1B                          JMP SHORT fifazf.00B81B24
	$+FC     00B81B09       C1EA 03                        SHR EDX, 3
	$+FF     00B81B0C       0356 1C                        ADD EDX, DWORD PTR DS:[ESI+1C]
	$+102    00B81B0F       0802                           OR BYTE PTR DS:[EDX], AL
	$+104    00B81B11       EB 11                          JMP SHORT fifazf.00B81B24
	$+106    00B81B13       8B46 14                        MOV EAX, DWORD PTR DS:[ESI+14]
	$+109    00B81B16       03C1                           ADD EAX, ECX
	$+10B    00B81B18       3BF8                           CMP EDI, EAX
	$+10D    00B81B1A       73 08                          JNB SHORT fifazf.00B81B24
	$+10F    00B81B1C       8B46 20                        MOV EAX, DWORD PTR DS:[ESI+20]
	$+112    00B81B1F       2BC1                           SUB EAX, ECX
	$+114    00B81B21       881438                         MOV BYTE PTR DS:[EAX+EDI], DL
	$+117    00B81B24       47                             INC EDI
	$+118    00B81B25       3B7B 08                        CMP EDI, DWORD PTR DS:[EBX+8]
	$+11B    00B81B28     ^ 72 B6                          JB SHORT fifazf.00B81AE0
	$+11D    00B81B2A       E9 BB010000                    JMP fifazf.00B81CEA
	$+122    00B81B2F       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+124    00B81B31       50                             PUSH EAX
	$+125    00B81B32       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$+127    00B81B34       FF51 1C                        CALL DWORD PTR DS:[ECX+1C]
	$+12A    00B81B37       E9 AE010000                    JMP fifazf.00B81CEA
	$+12F    00B81B3C       8B07                           MOV EAX, DWORD PTR DS:[EDI]
	$+131    00B81B3E       8D95 D8FDFFFF                  LEA EDX, DWORD PTR SS:[EBP-228]
	$+137    00B81B44       52                             PUSH EDX
	$+138    00B81B45       68 10010000                    PUSH 110
	$+13D    00B81B4A       50                             PUSH EAX
	$+13E    00B81B4B       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$+140    00B81B4D       FF51 24                        CALL DWORD PTR DS:[ECX+24]
	$+143    00B81B50       85C0                           TEST EAX, EAX
	$+145    00B81B52       0F85 0B020000                  JNZ fifazf.00B81D63
	$+14B    00B81B58       698D D8FDFFFF D0070000         IMUL ECX, DWORD PTR SS:[EBP-228], 7D0
	$+155    00B81B62       B8 01800080                    MOV EAX, 80008001
	$+15A    00B81B67       F7E9                           IMUL ECX
	$+15C    00B81B69       8B46 24                        MOV EAX, DWORD PTR DS:[ESI+24]
	$+15F    00B81B6C       03D1                           ADD EDX, ECX
	$+161    00B81B6E       C1FA 0F                        SAR EDX, 0F
	$+164    00B81B71       8BCA                           MOV ECX, EDX
	$+166    00B81B73       81C2 18FCFFFF                  ADD EDX, -3E8
	$+16C    00B81B79       C1E9 1F                        SHR ECX, 1F
	$+16F    00B81B7C       03CA                           ADD ECX, EDX
	$+171    00B81B7E       8908                           MOV DWORD PTR DS:[EAX], ECX
	$+173    00B81B80       B8 01800080                    MOV EAX, 80008001
	$+178    00B81B85       698D DCFDFFFF D0070000         IMUL ECX, DWORD PTR SS:[EBP-224], 7D0
	$+182    00B81B8F       F7E9                           IMUL ECX
	$+184    00B81B91       03D1                           ADD EDX, ECX
	$+186    00B81B93       B9 E8030000                    MOV ECX, 3E8
	$+18B    00B81B98       C1FA 0F                        SAR EDX, 0F
	$+18E    00B81B9B       8BC2                           MOV EAX, EDX
	$+190    00B81B9D       C1E8 1F                        SHR EAX, 1F
	$+193    00B81BA0       03C2                           ADD EAX, EDX
	--*/
	addr_tmp = scan_exe_.ScanAddr("6800010000??FF????85C00F85", 9);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	DX模式
	01167436    8B76 0C         MOV ESI,DWORD PTR DS:[ESI+0xC]
	01167439    85F6            TEST ESI,ESI
	0116743B    74 11           JE SHORT fifazf.0116744E
	0116743D    8B03            MOV EAX,DWORD PTR DS:[EBX]
	0116743F    8B08            MOV ECX,DWORD PTR DS:[EAX]
	01167441    8B51 34         MOV EDX,DWORD PTR DS:[ECX+0x34]
	01167444    6A 06           PUSH 0x6
	01167446    56              PUSH ESI
	01167447    50              PUSH EAX
	01167448    FFD2            CALL EDX
	0116744A    85C0            TEST EAX,EAX

	8B76??85F6????8B038B088B51??6A065650FFD285C0

	$ ==>    0119D8C6        83CA 02                   OR EDX,0x2
	$+3      0119D8C9        52                        PUSH EDX
	$+4      0119D8CA        8B56 0C                   MOV EDX,DWORD PTR DS:[ESI+0xC]
	$+7      0119D8CD        52                        PUSH EDX
	$+8      0119D8CE        50                        PUSH EAX
	$+9      0119D8CF        8B41 34                   MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+C      0119D8D2        FFD0                      CALL EAX
	$+E      0119D8D4        85C0                      TEST EAX,EAX
	$+10     0119D8D6      ^ 75 AD                     JNZ SHORT fifazf.0119D885
	$+12     0119D8D8        8D8D BCFDFFFF             LEA ECX,DWORD PTR SS:[EBP-0x244]
	$+18     0119D8DE        68 44020000               PUSH 0x244
	$+1D     0119D8E3        51                        PUSH ECX
	$+1E     0119D8E4        E8 27C745FF               CALL fifazf.005FA010
	$+23     0119D8E9        C785 BCFDFFFF 44020000    MOV DWORD PTR SS:[EBP-0x244],0x244
	$+2D     0119D8F3        8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+2F     0119D8F5        8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+31     0119D8F7        8B52 3C                   MOV EDX,DWORD PTR DS:[EDX+0x3C]
	$+34     0119D8FA        83C4 08                   ADD ESP,0x8
	$+37     0119D8FD        8D8D BCFDFFFF             LEA ECX,DWORD PTR SS:[EBP-0x244]
	$+3D     0119D903        51                        PUSH ECX
	$+3E     0119D904        50                        PUSH EAX
	$+3F     0119D905        FFD2                      CALL EDX

	83CA02??8B????????8B????FF??85C075??8D8D

	$-2C     00B8113E              837D 18 00           CMP DWORD PTR SS:[EBP+0x18],0x0
	$-28     00B81142              8B85 B4FDFFFF        MOV EAX,DWORD PTR SS:[EBP-0x24C]
	$-22     00B81148              F3:                  PREFIX REP:
	$-21     00B81149              0F6F47 14            MOVQ MM0,QWORD PTR DS:[EDI+0x14]
	$-1D     00B8114D              F3:                  PREFIX REP:
	$-1C     00B8114E              0F7F80 20030000      MOVQ QWORD PTR DS:[EAX+0x320],MM0
	$-15     00B81155              74 22                JE SHORT fifazf.00B81179
	$-13     00B81157              8B0B                 MOV ECX,DWORD PTR DS:[EBX]
	$-11     00B81159              33C0                 XOR EAX,EAX
	$-F      00B8115B              3845 14              CMP BYTE PTR SS:[EBP+0x14],AL
	$-C      00B8115E              0F94C0               SETE AL
	$-9      00B81161              8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$-7      00B81163              8D0485 04000000      LEA EAX,DWORD PTR DS:[EAX*4+0x4]
	$ ==>    00B8116A              83C8 02              OR EAX,0x2
	$+3      00B8116D              50                   PUSH EAX
	$+4      00B8116E              FF76 0C              PUSH DWORD PTR DS:[ESI+0xC]
	$+7      00B81171              51                   PUSH ECX
	$+8      00B81172              FF52 34              CALL DWORD PTR DS:[EDX+0x34]
	$+B      00B81175              85C0                 TEST EAX,EAX
	$+D      00B81177            ^ 75 B0                JNZ SHORT fifazf.00B81129
	$+F      00B81179              68 44020000          PUSH 0x244
	$+14     00B8117E              8D85 B8FDFFFF        LEA EAX,DWORD PTR SS:[EBP-0x248]
	$+1A     00B81184              6A 00                PUSH 0x0
	$+1C     00B81186              50                   PUSH EAX
	$+1D     00B81187              E8 90F2AEFF          CALL fifazf.0067041C                     ; JMP 到 msvcr120.memset

	zhp 2016/8/9 14:34 防止特征码找不到
	$-8A     00B80120       55                             PUSH EBP
	$-89     00B80121       8BEC                           MOV EBP, ESP
	$-87     00B80123       81EC 50020000                  SUB ESP, 250
	$-81     00B80129       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$-7C     00B8012E       33C5                           XOR EAX, EBP
	$-7A     00B80130       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$-77     00B80133       8B45 08                        MOV EAX, DWORD PTR SS:[EBP+8]
	$-74     00B80136       53                             PUSH EBX
	$-73     00B80137       56                             PUSH ESI
	$-72     00B80138       8BF1                           MOV ESI, ECX
	$-70     00B8013A       8B4D 10                        MOV ECX, DWORD PTR SS:[EBP+10]           ; fifazf.01F62FC2
	$-6D     00B8013D       57                             PUSH EDI
	$-6C     00B8013E       8B7D 0C                        MOV EDI, DWORD PTR SS:[EBP+C]
	$-69     00B80141       898D B0FDFFFF                  MOV DWORD PTR SS:[EBP-250], ECX
	$-63     00B80147       8B5C86 7C                      MOV EBX, DWORD PTR DS:[ESI+EAX*4+7C]
	$-5F     00B8014B       8B4E 04                        MOV ECX, DWORD PTR DS:[ESI+4]
	$-5C     00B8014E       8B4486 3C                      MOV EAX, DWORD PTR DS:[ESI+EAX*4+3C]
	$-58     00B80152       6A 00                          PUSH 0
	$-56     00B80154       8985 B4FDFFFF                  MOV DWORD PTR SS:[EBP-24C], EAX
	$-50     00B8015A       8D47 04                        LEA EAX, DWORD PTR DS:[EDI+4]
	$-4D     00B8015D       8B11                           MOV EDX, DWORD PTR DS:[ECX]
	$-4B     00B8015F       53                             PUSH EBX
	$-4A     00B80160       50                             PUSH EAX
	$-49     00B80161       51                             PUSH ECX
	$-48     00B80162       FF52 0C                        CALL DWORD PTR DS:[EDX+C]
	$-45     00B80165       85C0                           TEST EAX, EAX
	$-43     00B80167       74 15                          JE SHORT fifazf.00B8017E
	$-41     00B80169       5F                             POP EDI
	$-40     00B8016A       5E                             POP ESI                                  ; fifazf.00BF646D
	$-3F     00B8016B       32C0                           XOR AL, AL
	$-3D     00B8016D       5B                             POP EBX                                  ; ntdll.770DFD91
	$-3C     00B8016E       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$-39     00B80171       33CD                           XOR ECX, EBP
	$-37     00B80173       E8 E4D4A9FF                    CALL fifazf.0061D65C
	$-32     00B80178       8BE5                           MOV ESP, EBP
	$-30     00B8017A       5D                             POP EBP                                  ; ntdll.770DFD91
	$-2F     00B8017B       C2 1400                        RETN 14
	$-2C     00B8017E       837D 18 00                     CMP DWORD PTR SS:[EBP+18], 0
	$-28     00B80182       8B85 B4FDFFFF                  MOV EAX, DWORD PTR SS:[EBP-24C]          ; ntdll.770E015D
	$-22     00B80188       F3:                            PREFIX REP:                              ; 多余的前缀
	$-21     00B80189       0F6F47 14                      MOVQ MM0, QWORD PTR DS:[EDI+14]
	$-1D     00B8018D       F3:                            PREFIX REP:                              ; 多余的前缀
	$-1C     00B8018E       0F7F80 20030000                MOVQ QWORD PTR DS:[EAX+320], MM0
	$-15     00B80195       74 22                          JE SHORT fifazf.00B801B9
	$-13     00B80197       8B0B                           MOV ECX, DWORD PTR DS:[EBX]              ; fifazf.014EFED8
	$-11     00B80199       33C0                           XOR EAX, EAX
	$-F      00B8019B       3845 14                        CMP BYTE PTR SS:[EBP+14], AL
	$-C      00B8019E       0F94C0                         SETE AL
	$-9      00B801A1       8B11                           MOV EDX, DWORD PTR DS:[ECX]
	$-7      00B801A3       8D0485 04000000                LEA EAX, DWORD PTR DS:[EAX*4+4]
	$ ==>    00B801AA       83C8 02                        OR EAX, 2
	$+3      00B801AD       50                             PUSH EAX
	$+4      00B801AE       FF76 0C                        PUSH DWORD PTR DS:[ESI+C]
	$+7      00B801B1       51                             PUSH ECX
	$+8      00B801B2       FF52 34                        CALL DWORD PTR DS:[EDX+34]
	$+B      00B801B5       85C0                           TEST EAX, EAX
	$+D      00B801B7     ^ 75 B0                          JNZ SHORT fifazf.00B80169
	$+F      00B801B9       68 44020000                    PUSH 244
	$+14     00B801BE       8D85 B8FDFFFF                  LEA EAX, DWORD PTR SS:[EBP-248]
	$+1A     00B801C4       6A 00                          PUSH 0
	$+1C     00B801C6       50                             PUSH EAX
	$+1D     00B801C7       E8 80D9A9FF                    CALL fifazf.0061DB4C                     ; JMP 到 MSVCR120.memset
	$+22     00B801CC       C785 B8FDFFFF 44020000         MOV DWORD PTR SS:[EBP-248], 244
	$+2C     00B801D6       8D95 B8FDFFFF                  LEA EDX, DWORD PTR SS:[EBP-248]
	$+32     00B801DC       8B03                           MOV EAX, DWORD PTR DS:[EBX]              ; fifazf.014EFED8
	$+34     00B801DE       83C4 0C                        ADD ESP, 0C
	$+37     00B801E1       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$+39     00B801E3       52                             PUSH EDX
	$+3A     00B801E4       50                             PUSH EAX
	$+3B     00B801E5       FF51 3C                        CALL DWORD PTR DS:[ECX+3C]
	$+3E     00B801E8       85C0                           TEST EAX, EAX
	$+40     00B801EA       75 45                          JNZ SHORT fifazf.00B80231
	$+42     00B801EC       8B85 B4FDFFFF                  MOV EAX, DWORD PTR SS:[EBP-24C]          ; ntdll.770E015D
	$+48     00B801F2       8DB5 E0FDFFFF                  LEA ESI, DWORD PTR SS:[EBP-220]
	$+4E     00B801F8       B9 40000000                    MOV ECX, 40
	$+53     00B801FD       8D78 14                        LEA EDI, DWORD PTR DS:[EAX+14]
	$+56     00B80200       F3:A5                          REP MOVS DWORD PTR ES:[EDI], DWORD PTR D>
	$+58     00B80202       B9 40000000                    MOV ECX, 40
	$+5D     00B80207       66:A5                          MOVS WORD PTR ES:[EDI], WORD PTR DS:[ESI>
	$+5F     00B80209       A4                             MOVS BYTE PTR ES:[EDI], BYTE PTR DS:[ESI>
	$+60     00B8020A       8DB8 18010000                  LEA EDI, DWORD PTR DS:[EAX+118]
	$+66     00B80210       8DB5 E4FEFFFF                  LEA ESI, DWORD PTR SS:[EBP-11C]
	$+6C     00B80216       F3:A5                          REP MOVS DWORD PTR ES:[EDI], DWORD PTR D>
	$+6E     00B80218       B9 40000000                    MOV ECX, 40
	$+73     00B8021D       66:A5                          MOVS WORD PTR ES:[EDI], WORD PTR DS:[ESI>
	$+75     00B8021F       A4                             MOVS BYTE PTR ES:[EDI], BYTE PTR DS:[ESI>
	$+76     00B80220       8DB8 1C020000                  LEA EDI, DWORD PTR DS:[EAX+21C]
	$+7C     00B80226       8DB5 E4FEFFFF                  LEA ESI, DWORD PTR SS:[EBP-11C]
	$+82     00B8022C       F3:A5                          REP MOVS DWORD PTR ES:[EDI], DWORD PTR D>
	$+84     00B8022E       66:A5                          MOVS WORD PTR ES:[EDI], WORD PTR DS:[ESI>
	$+86     00B80230       A4                             MOVS BYTE PTR ES:[EDI], BYTE PTR DS:[ESI>
	$+87     00B80231       8B03                           MOV EAX, DWORD PTR DS:[EBX]              ; fifazf.014EFED8
	$+89     00B80233       FFB5 B0FDFFFF                  PUSH DWORD PTR SS:[EBP-250]
	$+8F     00B80239       50                             PUSH EAX
	$+90     00B8023A       8B08                           MOV ECX, DWORD PTR DS:[EAX]
	$+92     00B8023C       FF51 2C                        CALL DWORD PTR DS:[ECX+2C]
	$+95     00B8023F       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+98     00B80242       85C0                           TEST EAX, EAX
	$+9A     00B80244       5F                             POP EDI                                  ; ntdll.770DFD91
	$+9B     00B80245       0F94C0                         SETE AL
	$+9E     00B80248       33CD                           XOR ECX, EBP
	$+A0     00B8024A       5E                             POP ESI                                  ; ntdll.770DFD91
	$+A1     00B8024B       5B                             POP EBX                                  ; ntdll.770DFD91
	$+A2     00B8024C       E8 0BD4A9FF                    CALL fifazf.0061D65C
	$+A7     00B80251       8BE5                           MOV ESP, EBP
	$+A9     00B80253       5D                             POP EBP                                  ; ntdll.770DFD91
	$+AA     00B80254       C2 1400                        RETN 14
	--*/
	addr_tmp = scan_exe_.ScanAddr("83??02??FF??????FF????85??75", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_DX_INPUT_MODEL  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	窗口句柄
	007932CF    6A 00           PUSH 0x0
	007932D1    6A 00           PUSH 0x0
	007932D3    57              PUSH EDI
	007932D4    56              PUSH ESI
	007932D5    68 00000080     PUSH 0x80000000
	007932DA    68 00000080     PUSH 0x80000000
	007932DF    53              PUSH EBX
	007932E0    8D8D BCFEFFFF   LEA ECX,DWORD PTR SS:[EBP-0x144]
	007932E6    51              PUSH ECX
	007932E7    52              PUSH EDX
	007932E8    50              PUSH EAX
	007932E9    FF15 18492201   CALL DWORD PTR DS:[0x1224918]                    ; user32.CreateWindowExA
	007932EF    6A 01           PUSH 0x1

	6A006A0057566800000080680000008053

	$ ==>    00BC87EF        6A 00                PUSH 0x0
	$+2      00BC87F1        6A 00                PUSH 0x0
	$+4      00BC87F3        56                   PUSH ESI
	$+5      00BC87F4        57                   PUSH EDI
	$+6      00BC87F5        68 00000080          PUSH 0x80000000
	$+B      00BC87FA        68 00000080          PUSH 0x80000000
	$+10     00BC87FF        53                   PUSH EBX
	$+11     00BC8800        50                   PUSH EAX
	$+12     00BC8801        FF35 44EA7401        PUSH DWORD PTR DS:[0x174EA44]            ; fifazf.01571070
	$+18     00BC8807        FFB5 E4FEFFFF        PUSH DWORD PTR SS:[EBP-0x11C]
	$+1E     00BC880D        FF15 44294D01        CALL DWORD PTR DS:[0x14D2944]
	$+24     00BC8813        6A 01                PUSH 0x1
	$+26     00BC8815        50                   PUSH EAX
	$+27     00BC8816        A3 9CCD8801          MOV DWORD PTR DS:[0x188CD9C],EAX
	$+2C     00BC881B        FF15 B8294D01        CALL DWORD PTR DS:[0x14D29B8]

	zhp 2016/8/9 14:37 防止特征码找不到
	$-122    00BC74E4       5F                             POP EDI                                  ; ntdll.770DFD91
	$-121    00BC74E5       5E                             POP ESI                                  ; ntdll.770DFD91
	$-120    00BC74E6       32C0                           XOR AL, AL
	$-11E    00BC74E8       5B                             POP EBX                                  ; ntdll.770DFD91
	$-11D    00BC74E9       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$-11A    00BC74EC       33CD                           XOR ECX, EBP
	$-118    00BC74EE       E8 6961A5FF                    CALL fifazf.0061D65C
	$-113    00BC74F3       8BE5                           MOV ESP, EBP
	$-111    00BC74F5       5D                             POP EBP                                  ; ntdll.770DFD91
	$-110    00BC74F6       C3                             RETN
	$-10F    00BC74F7       68 44765101                    PUSH fifazf.01517644                     ; ASCII "FULLSCREEN"
	$-10A    00BC74FC       6A 01                          PUSH 1
	$-108    00BC74FE       6A 01                          PUSH 1
	$-106    00BC7500       68 787A200A                    PUSH 0A207A78
	$-101    00BC7505       C705 A4508A01 10000000         MOV DWORD PTR DS:[18A50A4], 10
	$-F7     00BC750F       C705 A8508A01 03000000         MOV DWORD PTR DS:[18A50A8], 3
	$-ED     00BC7519       C705 B0508A01 01000000         MOV DWORD PTR DS:[18A50B0], 1
	$-E3     00BC7523       E8 3872A5FF                    CALL fifazf.0061E760
	$-DE     00BC7528       8BC8                           MOV ECX, EAX
	$-DC     00BC752A       E8 4172A5FF                    CALL fifazf.0061E770
	$-D7     00BC752F       8BF0                           MOV ESI, EAX
	$-D5     00BC7531       C785 E0FEFFFF 00000000         MOV DWORD PTR SS:[EBP-120], 0
	$-CB     00BC753B       8D85 A8FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-158]
	$-C5     00BC7541       C785 E4FEFFFF 00000000         MOV DWORD PTR SS:[EBP-11C], 0
	$-BB     00BC754B       50                             PUSH EAX
	$-BA     00BC754C       8D85 E4FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-11C]
	$-B4     00BC7552       50                             PUSH EAX
	$-B3     00BC7553       8D85 E0FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-120]
	$-AD     00BC7559       50                             PUSH EAX
	$-AC     00BC755A       56                             PUSH ESI
	$-AB     00BC755B       E8 C0EBFFFF                    CALL fifazf.00BC6120
	$-A6     00BC7560       83C4 10                        ADD ESP, 10
	$-A3     00BC7563       68 DC0E4F01                    PUSH fifazf.014F0EDC                     ; ASCII "USE_FAKE_FULLSCREEN"
	$-9E     00BC7568       6A 01                          PUSH 1
	$-9C     00BC756A       6A 00                          PUSH 0
	$-9A     00BC756C       68 1A1D9145                    PUSH 45911D1A
	$-95     00BC7571       E8 EA71A5FF                    CALL fifazf.0061E760
	$-90     00BC7576       8BC8                           MOV ECX, EAX
	$-8E     00BC7578       E8 F371A5FF                    CALL fifazf.0061E770
	$-89     00BC757D       85C0                           TEST EAX, EAX
	$-87     00BC757F       75 16                          JNZ SHORT fifazf.00BC7597
	$-85     00BC7581       83FE 02                        CMP ESI, 2
	$-82     00BC7584       75 11                          JNZ SHORT fifazf.00BC7597
	$-80     00BC7586       8B35 2C9A4E01                  MOV ESI, DWORD PTR DS:[14E9A2C]
	$-7A     00BC758C       50                             PUSH EAX
	$-79     00BC758D       FFD6                           CALL ESI
	$-77     00BC758F       6A 01                          PUSH 1
	$-75     00BC7591       8BD8                           MOV EBX, EAX
	$-73     00BC7593       FFD6                           CALL ESI
	$-71     00BC7595       EB 06                          JMP SHORT fifazf.00BC759D
	$-6F     00BC7597       8B85 DCFEFFFF                  MOV EAX, DWORD PTR SS:[EBP-124]
	$-69     00BC759D       50                             PUSH EAX
	$-68     00BC759E       53                             PUSH EBX
	$-67     00BC759F       6A 00                          PUSH 0
	$-65     00BC75A1       6A 00                          PUSH 0
	$-63     00BC75A3       8D85 E8FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-118]
	$-5D     00BC75A9       50                             PUSH EAX
	$-5C     00BC75AA       FF15 D0994E01                  CALL DWORD PTR DS:[14E99D0]
	$-56     00BC75B0       8B9D E0FEFFFF                  MOV EBX, DWORD PTR SS:[EBP-120]
	$-50     00BC75B6       8D85 E8FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-118]
	$-4A     00BC75BC       6A 00                          PUSH 0
	$-48     00BC75BE       53                             PUSH EBX
	$-47     00BC75BF       50                             PUSH EAX
	$-46     00BC75C0       FF15 14994E01                  CALL DWORD PTR DS:[14E9914]
	$-40     00BC75C6       8BBD F0FEFFFF                  MOV EDI, DWORD PTR SS:[EBP-110]
	$-3A     00BC75CC       8D85 F8FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-108]
	$-34     00BC75D2       8BB5 F4FEFFFF                  MOV ESI, DWORD PTR SS:[EBP-10C]
	$-2E     00BC75D8       2BBD E8FEFFFF                  SUB EDI, DWORD PTR SS:[EBP-118]
	$-28     00BC75DE       2BB5 ECFEFFFF                  SUB ESI, DWORD PTR SS:[EBP-114]
	$-22     00BC75E4       68 748F5801                    PUSH fifazf.01588F74                     ; ASCII "FIFA ONLINE3 - Developed by SPEARHEAD"
	$-1D     00BC75E9       68 04010000                    PUSH 104
	$-18     00BC75EE       50                             PUSH EAX
	$-17     00BC75EF       FF15 50984E01                  CALL DWORD PTR DS:[14E9850]              ; MSVCR120.strcpy_s
	$-11     00BC75F5       83C4 0C                        ADD ESP, 0C
	$-E      00BC75F8       8D85 F8FEFFFF                  LEA EAX, DWORD PTR SS:[EBP-108]
	$-8      00BC75FE       6A 00                          PUSH 0
	$-6      00BC7600       FF35 94508A01                  PUSH DWORD PTR DS:[18A5094]              ; fifazf.00400000
	$ ==>    00BC7606       6A 00                          PUSH 0
	$+2      00BC7608       6A 00                          PUSH 0
	$+4      00BC760A       56                             PUSH ESI
	$+5      00BC760B       57                             PUSH EDI
	$+6      00BC760C       68 00000080                    PUSH 80000000
	$+B      00BC7611       68 00000080                    PUSH 80000000
	$+10     00BC7616       53                             PUSH EBX
	$+11     00BC7617       50                             PUSH EAX
	$+12     00BC7618       FF35 846B7601                  PUSH DWORD PTR DS:[1766B84]              ; fifazf.01588DA8
	$+18     00BC761E       FFB5 E4FEFFFF                  PUSH DWORD PTR SS:[EBP-11C]              ; fifazf.0063F156
	$+1E     00BC7624       FF15 E8994E01                  CALL DWORD PTR DS:[14E99E8]
	$+24     00BC762A       6A 01                          PUSH 1
	$+26     00BC762C       50                             PUSH EAX
	$+27     00BC762D       A3 98508A01                    MOV DWORD PTR DS:[18A5098], EAX
	$+2C     00BC7632       FF15 409A4E01                  CALL DWORD PTR DS:[14E9A40]
	$+32     00BC7638       FF35 98508A01                  PUSH DWORD PTR DS:[18A5098]
	$+38     00BC763E       FF15 A8994E01                  CALL DWORD PTR DS:[14E99A8]
	$+3E     00BC7644       68 007F0000                    PUSH 7F00
	$+43     00BC7649       FF35 94508A01                  PUSH DWORD PTR DS:[18A5094]              ; fifazf.00400000
	$+49     00BC764F       C705 84508A01 08000000         MOV DWORD PTR DS:[18A5084], 8
	$+53     00BC7659       C705 88508A01 00000000         MOV DWORD PTR DS:[18A5088], 0
	$+5D     00BC7663       C705 7C508A01 00000000         MOV DWORD PTR DS:[18A507C], 0
	$+67     00BC766D       FF15 D4994E01                  CALL DWORD PTR DS:[14E99D4]
	$+6D     00BC7673       A3 A0508A01                    MOV DWORD PTR DS:[18A50A0], EAX
	$+72     00BC7678       A1 8C508A01                    MOV EAX, DWORD PTR DS:[18A508C]
	$+77     00BC767D       85C0                           TEST EAX, EAX
	$+79     00BC767F       75 24                          JNZ SHORT fifazf.00BC76A5
	$+7B     00BC7681       6A 08                          PUSH 8
	$+7D     00BC7683       E8 D8CAA3FF                    CALL fifazf.00604160
	$+82     00BC7688       83C4 04                        ADD ESP, 4
	$+85     00BC768B       85C0                           TEST EAX, EAX
	$+87     00BC768D       74 0F                          JE SHORT fifazf.00BC769E
	$+89     00BC768F       C700 00000000                  MOV DWORD PTR DS:[EAX], 0
	$+8F     00BC7695       C740 04 00000000               MOV DWORD PTR DS:[EAX+4], 0
	$+96     00BC769C       EB 02                          JMP SHORT fifazf.00BC76A0
	$+98     00BC769E       33C0                           XOR EAX, EAX
	$+9A     00BC76A0       A3 8C508A01                    MOV DWORD PTR DS:[18A508C], EAX
	$+9F     00BC76A5       8BC8                           MOV ECX, EAX
	$+A1     00BC76A7       E8 34FAFFFF                    CALL fifazf.00BC70E0
	$+A6     00BC76AC       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+A9     00BC76AF       B0 01                          MOV AL, 1
	$+AB     00BC76B1       5F                             POP EDI                                  ; ntdll.770DFD91
	$+AC     00BC76B2       5E                             POP ESI                                  ; ntdll.770DFD91
	$+AD     00BC76B3       33CD                           XOR ECX, EBP
	$+AF     00BC76B5       C705 A4508A01 10000000         MOV DWORD PTR DS:[18A50A4], 10
	$+B9     00BC76BF       C705 A8508A01 03000000         MOV DWORD PTR DS:[18A50A8], 3
	$+C3     00BC76C9       C705 B0508A01 01000000         MOV DWORD PTR DS:[18A50B0], 1
	$+CD     00BC76D3       5B                             POP EBX                                  ; ntdll.770DFD91
	$+CE     00BC76D4       E8 835FA5FF                    CALL fifazf.0061D65C
	$+D3     00BC76D9       8BE5                           MOV ESP, EBP
	$+D5     00BC76DB       5D                             POP EBP                                  ; ntdll.770DFD91
	$+D6     00BC76DC       C3                             RETN
	$+D7     00BC76DD       CC                             INT3
	$+D8     00BC76DE       CC                             INT3
	$+D9     00BC76DF       CC                             INT3
	$+DA     00BC76E0       55                             PUSH EBP
	$+DB     00BC76E1       8BEC                           MOV EBP, ESP
	$+DD     00BC76E3       81EC 24010000                  SUB ESP, 124
	$+E3     00BC76E9       A1 60C37101                    MOV EAX, DWORD PTR DS:[171C360]
	$+E8     00BC76EE       33C5                           XOR EAX, EBP
	$+EA     00BC76F0       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+ED     00BC76F3       8B45 08                        MOV EAX, DWORD PTR SS:[EBP+8]
	$+F0     00BC76F6       53                             PUSH EBX
	$+F1     00BC76F7       8B5D 10                        MOV EBX, DWORD PTR SS:[EBP+10]           ; fifazf.01F62FC2
	$+F4     00BC76FA       56                             PUSH ESI
	$+F5     00BC76FB       8B75 14                        MOV ESI, DWORD PTR SS:[EBP+14]
	$+F8     00BC76FE       57                             PUSH EDI
	$+F9     00BC76FF       8B7D 0C                        MOV EDI, DWORD PTR SS:[EBP+C]
	$+FC     00BC7702       56                             PUSH ESI
	$+FD     00BC7703       53                             PUSH EBX
	$+FE     00BC7704       57                             PUSH EDI
	$+FF     00BC7705       50                             PUSH EAX
	$+100    00BC7706       8985 4CFFFFFF                  MOV DWORD PTR SS:[EBP-B4], EAX
	$+106    00BC770C       E8 DF0B0000                    CALL fifazf.00BC82F0
	$+10B    00BC7711       83C4 10                        ADD ESP, 10
	$+10E    00BC7714       85C0                           TEST EAX, EAX
	$+110    00BC7716       0F85 080A0000                  JNZ fifazf.00BC8124
	$+116    00BC771C       83FF 0F                        CMP EDI, 0F
	$+119    00BC771F       0F84 E3090000                  JE fifazf.00BC8108
	$+11F    00BC7725       83FF 14                        CMP EDI, 14
	$+122    00BC7728       0F84 C2090000                  JE fifazf.00BC80F0
	$+128    00BC772E       83FF 7B                        CMP EDI, 7B
	$+12B    00BC7731       0F84 B9090000                  JE fifazf.00BC80F0
	$+131    00BC7737       83FF 18                        CMP EDI, 18
	$+134    00BC773A       0F87 E0020000                  JA fifazf.00BC7A20
	$+13A    00BC7740       0F84 AB020000                  JE fifazf.00BC79F1
	$+140    00BC7746       8D47 FF                        LEA EAX, DWORD PTR DS:[EDI-1]
	$+143    00BC7749       83F8 05                        CMP EAX, 5
	$+146    00BC774C       0F87 7C090000                  JA fifazf.00BC80CE
	$+14C    00BC7752       FF2485 3C81BC00                JMP DWORD PTR DS:[EAX*4+BC813C]          ; fifazf.00BC7759
	$+153    00BC7759       FF15 10924E01                  CALL DWORD PTR DS:[14E9210]              ; kernel32.GetTickCount
	$+159    00BC775F       56                             PUSH ESI
	$+15A    00BC7760       53                             PUSH EBX
	$+15B    00BC7761       57                             PUSH EDI
	$+15C    00BC7762       FFB5 4CFFFFFF                  PUSH DWORD PTR SS:[EBP-B4]               ; fifazf.0063E47D
	$+162    00BC7768       A3 74508A01                    MOV DWORD PTR DS:[18A5074], EAX
	$+167    00BC776D       FF15 F0994E01                  CALL DWORD PTR DS:[14E99F0]              ; ntdll.NtdllDefWindowProc_A
	$+16D    00BC7773       5F                             POP EDI                                  ; ntdll.770DFD91
	$+16E    00BC7774       5E                             POP ESI                                  ; ntdll.770DFD91
	$+16F    00BC7775       5B                             POP EBX                                  ; ntdll.770DFD91
	$+170    00BC7776       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+173    00BC7779       33CD                           XOR ECX, EBP
	$+175    00BC777B       E8 DC5EA5FF                    CALL fifazf.0061D65C
	$+17A    00BC7780       8BE5                           MOV ESP, EBP
	$+17C    00BC7782       5D                             POP EBP                                  ; ntdll.770DFD91
	$+17D    00BC7783       C2 1000                        RETN 10
	$+180    00BC7786       6A 00                          PUSH 0
	$+182    00BC7788       E9 39090000                    JMP fifazf.00BC80C6
	$+187    00BC778D       83FB 01                        CMP EBX, 1
	$+18A    00BC7790       75 27                          JNZ SHORT fifazf.00BC77B9
	$+18C    00BC7792       E8 A91D0000                    CALL fifazf.00BC9540
	$+191    00BC7797       56                             PUSH ESI
	$+192    00BC7798       53                             PUSH EBX
	$+193    00BC7799       57                             PUSH EDI
	$+194    00BC779A       FFB5 4CFFFFFF                  PUSH DWORD PTR SS:[EBP-B4]               ; fifazf.0063E47D
	$+19A    00BC77A0       FF15 F0994E01                  CALL DWORD PTR DS:[14E99F0]              ; ntdll.NtdllDefWindowProc_A
	$+1A0    00BC77A6       5F                             POP EDI                                  ; ntdll.770DFD91
	$+1A1    00BC77A7       5E                             POP ESI                                  ; ntdll.770DFD91
	$+1A2    00BC77A8       5B                             POP EBX                                  ; ntdll.770DFD91
	$+1A3    00BC77A9       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+1A6    00BC77AC       33CD                           XOR ECX, EBP
	$+1A8    00BC77AE       E8 A95EA5FF                    CALL fifazf.0061D65C
	$+1AD    00BC77B3       8BE5                           MOV ESP, EBP
	$+1AF    00BC77B5       5D                             POP EBP                                  ; ntdll.770DFD91
	$+1B0    00BC77B6       C2 1000                        RETN 10
	$+1B3    00BC77B9       E8 F2210000                    CALL fifazf.00BC99B0
	$+1B8    00BC77BE       56                             PUSH ESI
	$+1B9    00BC77BF       53                             PUSH EBX
	$+1BA    00BC77C0       57                             PUSH EDI
	$+1BB    00BC77C1       FFB5 4CFFFFFF                  PUSH DWORD PTR SS:[EBP-B4]               ; fifazf.0063E47D
	$+1C1    00BC77C7       FF15 F0994E01                  CALL DWORD PTR DS:[14E99F0]              ; ntdll.NtdllDefWindowProc_A
	$+1C7    00BC77CD       5F                             POP EDI                                  ; ntdll.770DFD91
	$+1C8    00BC77CE       5E                             POP ESI                                  ; ntdll.770DFD91
	$+1C9    00BC77CF       5B                             POP EBX                                  ; ntdll.770DFD91
	$+1CA    00BC77D0       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+1CD    00BC77D3       33CD                           XOR ECX, EBP
	$+1CF    00BC77D5       E8 825EA5FF                    CALL fifazf.0061D65C
	$+1D4    00BC77DA       8BE5                           MOV ESP, EBP
	$+1D6    00BC77DC       5D                             POP EBP                                  ; ntdll.770DFD91
	$+1D7    00BC77DD       C2 1000                        RETN 10
	$+1DA    00BC77E0       68 44765101                    PUSH fifazf.01517644                     ; ASCII "FULLSCREEN"
	$+1DF    00BC77E5       6A 01                          PUSH 1
	$+1E1    00BC77E7       6A 01                          PUSH 1
	$+1E3    00BC77E9       68 787A200A                    PUSH 0A207A78
	$+1E8    00BC77EE       E8 6D6FA5FF                    CALL fifazf.0061E760
	$+1ED    00BC77F3       8BC8                           MOV ECX, EAX
	$+1EF    00BC77F5       E8 766FA5FF                    CALL fifazf.0061E770
	$+1F4    00BC77FA       8985 48FFFFFF                  MOV DWORD PTR SS:[EBP-B8], EAX
	$+1FA    00BC7800       85DB                           TEST EBX, EBX
	$+1FC    00BC7802       0F85 EC000000                  JNZ fifazf.00BC78F4
	$+202    00BC7808       E8 331D0000                    CALL fifazf.00BC9540
	$+207    00BC780D       68 DC0E4F01                    PUSH fifazf.014F0EDC                     ; ASCII "USE_FAKE_FULLSCREEN"
	$+20C    00BC7812       6A 01                          PUSH 1
	$+20E    00BC7814       53                             PUSH EBX
	$+20F    00BC7815       68 1A1D9145                    PUSH 45911D1A
	$+214    00BC781A       E8 416FA5FF                    CALL fifazf.0061E760
	--*/

	addr_tmp = scan_exe_.ScanAddr("6A006A00????68000000806800000080", 0x24);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_FOR_GAIN_MAIN_WND_HANDLE  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	calloc
	00F17750    6A 0C           PUSH 0xC
	00F17752    6A 01           PUSH 0x1
	00F17754    FF15 B0034501   CALL DWORD PTR DS:[0x14503B0]                    ; msvcr100.calloc
	00F1775A    83C4 08         ADD ESP,0x8
	00F1775D    C3              RETN

	zhp 2016/8/9 14:38 防止特征码找不到
	$ ==>    0043997F       CC                             INT3
	$+1      00439980       6A 0C                          PUSH 0C
	$+3      00439982       6A 01                          PUSH 1
	$+5      00439984       FF15 C42F7801                  CALL DWORD PTR DS:[1782FC4]              ; MSVCR120.calloc UPDATES_FIFA_REVERSAL_MEMOPER_G_CALLOC
	$+B      0043998A       83C4 08                        ADD ESP, 8
	$+E      0043998D       C3                             RETN
	$+F      0043998E       CC                             INT3
	$+10     0043998F       CC                             INT3
	$+11     00439990       83EC 08                        SUB ESP, 8
	$+14     00439993       8B4424 1C                      MOV EAX, DWORD PTR SS:[ESP+1C]
	$+18     00439997       53                             PUSH EBX
	$+19     00439998       55                             PUSH EBP
	$+1A     00439999       56                             PUSH ESI
	$+1B     0043999A       57                             PUSH EDI
	$+1C     0043999B       8B7C24 20                      MOV EDI, DWORD PTR SS:[ESP+20]
	$+20     0043999F       8B9487 34010000                MOV EDX, DWORD PTR DS:[EDI+EAX*4+134]
	$+27     004399A6       8B4424 1C                      MOV EAX, DWORD PTR SS:[ESP+1C]
	$+2B     004399AA       8B18                           MOV EBX, DWORD PTR DS:[EAX]
	$+2D     004399AC       8B40 08                        MOV EAX, DWORD PTR DS:[EAX+8]
	$+30     004399AF       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+32     004399B1       8BB1 84860000                  MOV ESI, DWORD PTR DS:[ECX+8684]
	$+38     004399B7       895424 14                      MOV DWORD PTR SS:[ESP+14], EDX
	$+3C     004399BB       8B97 38020000                  MOV EDX, DWORD PTR DS:[EDI+238]
	$+42     004399C1       8BE8                           MOV EBP, EAX
	$+44     004399C3       2B6C24 28                      SUB EBP, DWORD PTR SS:[ESP+28]
	$+48     004399C7       F642 40 01                     TEST BYTE PTR DS:[EDX+40], 1
	$+4C     004399CB       894424 10                      MOV DWORD PTR SS:[ESP+10], EAX
	$+50     004399CF       74 34                          JE SHORT fifazf.00439A05
	$+52     004399D1       C74424 2C 00400000             MOV DWORD PTR SS:[ESP+2C], 4000
	$+5A     004399D9       3D 00400000                    CMP EAX, 4000
	$+5F     004399DE       77 04                          JA SHORT fifazf.004399E4
	$+61     004399E0       894424 2C                      MOV DWORD PTR SS:[ESP+2C], EAX
	$+65     004399E4       8B4424 2C                      MOV EAX, DWORD PTR SS:[ESP+2C]
	$+69     004399E8       50                             PUSH EAX
	$+6A     004399E9       81C1 65450000                  ADD ECX, 4565
	$+70     004399EF       53                             PUSH EBX
	$+71     004399F0       51                             PUSH ECX
	$+72     004399F1       E8 50411E00                    CALL fifazf.0061DB46                     ; JMP 到 MSVCR120.memcpy
	$+77     004399F6       8B1F                           MOV EBX, DWORD PTR DS:[EDI]
	$+79     004399F8       8B4424 38                      MOV EAX, DWORD PTR SS:[ESP+38]
	$+7D     004399FC       83C4 0C                        ADD ESP, 0C
	$+80     004399FF       81C3 65450000                  ADD EBX, 4565
	$+86     00439A05       8B5424 14                      MOV EDX, DWORD PTR SS:[ESP+14]
	$+8A     00439A09       8D4C24 20                      LEA ECX, DWORD PTR SS:[ESP+20]
	$+8E     00439A0D       51                             PUSH ECX
	$+8F     00439A0E       50                             PUSH EAX
	$+90     00439A0F       53                             PUSH EBX
	$+91     00439A10       52                             PUSH EDX
	$+92     00439A11       57                             PUSH EDI
	$+93     00439A12       E8 C96BFFFF                    CALL fifazf.004305E0
	$+98     00439A17       83C4 14                        ADD ESP, 14
	$+9B     00439A1A       894424 28                      MOV DWORD PTR SS:[ESP+28], EAX
	$+9F     00439A1E       85C0                           TEST EAX, EAX
	$+A1     00439A20       0F85 F7000000                  JNZ fifazf.00439B1D
	$+A7     00439A26       8B4424 20                      MOV EAX, DWORD PTR SS:[ESP+20]
	$+AB     00439A2A       3BC5                           CMP EAX, EBP
	$+AD     00439A2C       76 08                          JBE SHORT fifazf.00439A36
	$+AF     00439A2E       8BD5                           MOV EDX, EBP
	$+B1     00439A30       895424 2C                      MOV DWORD PTR SS:[ESP+2C], EDX
	$+B5     00439A34       EB 06                          JMP SHORT fifazf.00439A3C
	$+B7     00439A36       894424 2C                      MOV DWORD PTR SS:[ESP+2C], EAX
	$+BB     00439A3A       8BD0                           MOV EDX, EAX
	$+BD     00439A3C       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+BF     00439A3E       8BE8                           MOV EBP, EAX
	$+C1     00439A40       2BEA                           SUB EBP, EDX
	$+C3     00439A42       83B9 E4020000 00               CMP DWORD PTR DS:[ECX+2E4], 0
	$+CA     00439A49       74 2C                          JE SHORT fifazf.00439A77
	$+CC     00439A4B       57                             PUSH EDI
	$+CD     00439A4C       52                             PUSH EDX
	$+CE     00439A4D       53                             PUSH EBX
	$+CF     00439A4E       6A 02                          PUSH 2
	$+D1     00439A50       51                             PUSH ECX
	$+D2     00439A51       E8 AA6DFFFF                    CALL fifazf.00430800
	$+D7     00439A56       83C4 14                        ADD ESP, 14
	$+DA     00439A59       85ED                           TEST EBP, EBP
	$+DC     00439A5B       74 25                          JE SHORT fifazf.00439A82
	$+DE     00439A5D       8B4424 2C                      MOV EAX, DWORD PTR SS:[ESP+2C]
	$+E2     00439A61       8B0F                           MOV ECX, DWORD PTR DS:[EDI]
	$+E4     00439A63       57                             PUSH EDI
	$+E5     00439A64       55                             PUSH EBP
	$+E6     00439A65       03C3                           ADD EAX, EBX
	$+E8     00439A67       50                             PUSH EAX
	$+E9     00439A68       6A 04                          PUSH 4
	$+EB     00439A6A       51                             PUSH ECX
	$+EC     00439A6B       E8 906DFFFF                    CALL fifazf.00430800

	2016/8/25 18:41 这个特征码与UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL的特征码是紧挨着的
	$-20     00D22D50            56                     PUSH ESI
	$-1F     00D22D51            8B7424 08              MOV ESI, DWORD PTR SS:[ESP+8]
	$-1B     00D22D55            85F6                   TEST ESI, ESI
	$-19     00D22D57            74 0B                  JE SHORT fifazf.00D22D64
	$-17     00D22D59            FF36                   PUSH DWORD PTR DS:[ESI]                  ; fifazf.0158F380
	$-15     00D22D5B            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$-F      00D22D61            83C4 04                ADD ESP, 4
	$-C      00D22D64            56                     PUSH ESI
	$-B      00D22D65            FF15 38E07801          CALL DWORD PTR DS:[178E038]              ; MSVCR120.free
	$-5      00D22D6B            83C4 04                ADD ESP, 4
	$-2      00D22D6E            5E                     POP ESI                                  ; 0213C828
	$-1      00D22D6F            C3                     RETN
	$ ==>    00D22D70            6A 0C                  PUSH 0C
	$+2      00D22D72            6A 01                  PUSH 1
	$+4      00D22D74            FF15 44E07801          CALL DWORD PTR DS:[178E044]              ; MSVCR120.calloc
	$+A      00D22D7A            83C4 08                ADD ESP, 8
	$+D      00D22D7D            C3                     RETN
	$+E      00D22D7E            CC                     INT3
	$+F      00D22D7F            CC                     INT3
	$+10     00D22D80            51                     PUSH ECX
	$+11     00D22D81            8B4424 18              MOV EAX, DWORD PTR SS:[ESP+18]
	$+15     00D22D85            53                     PUSH EBX
	$+16     00D22D86            55                     PUSH EBP
	$+17     00D22D87            56                     PUSH ESI
	$+18     00D22D88            8B7424 18              MOV ESI, DWORD PTR SS:[ESP+18]
	$+1C     00D22D8C            57                     PUSH EDI
	$+1D     00D22D8D            8B8486 54010000        MOV EAX, DWORD PTR DS:[ESI+EAX*4+154]
	$+24     00D22D94            8B16                   MOV EDX, DWORD PTR DS:[ESI]              ; fifazf.0158F380
	$+26     00D22D96            894424 10              MOV DWORD PTR SS:[ESP+10], EAX
	$+2A     00D22D9A            8B4424 18              MOV EAX, DWORD PTR SS:[ESP+18]
	$+2E     00D22D9E            8BBA 14010000          MOV EDI, DWORD PTR DS:[EDX+114]          ; d3dx9_43.51B7B9D3
	$+34     00D22DA4            8B08                   MOV ECX, DWORD PTR DS:[EAX]              ; d3dx9_43.51A8CC68
	$+36     00D22DA6            8B58 08                MOV EBX, DWORD PTR DS:[EAX+8]            ; d3dx9_43.51A8F210
	$+39     00D22DA9            8BC3                   MOV EAX, EBX
	$+3B     00D22DAB            2B4424 24              SUB EAX, DWORD PTR SS:[ESP+24]
	$+3F     00D22DAF            894424 24              MOV DWORD PTR SS:[ESP+24], EAX
	$+43     00D22DB3            8B86 0C020000          MOV EAX, DWORD PTR DS:[ESI+20C]
	$+49     00D22DB9            894C24 28              MOV DWORD PTR SS:[ESP+28], ECX
	$+4D     00D22DBD            F640 40 01             TEST BYTE PTR DS:[EAX+40], 1
	$+51     00D22DC1            74 34                  JE SHORT fifazf.00D22DF7
	$+53     00D22DC3            83BE 3C010000 14       CMP DWORD PTR DS:[ESI+13C], 14
	$+5A     00D22DCA            74 2B                  JE SHORT fifazf.00D22DF7
	$+5C     00D22DCC            B8 00400000            MOV EAX, 4000
	$+61     00D22DD1            8BEB                   MOV EBP, EBX
	$+63     00D22DD3            3BD8                   CMP EBX, EAX
	$+65     00D22DD5            0F47E8                 CMOVA EBP, EAX
	$+68     00D22DD8            8D82 55450000          LEA EAX, DWORD PTR DS:[EDX+4555]
	$+6E     00D22DDE            55                     PUSH EBP
	$+6F     00D22DDF            51                     PUSH ECX
	$+70     00D22DE0            50                     PUSH EAX
	$+71     00D22DE1            E8 F0B78CFF            CALL fifazf.005EE5D6                     ; JMP 到 MSVCR120.memcpy
	$+76     00D22DE6            8B0E                   MOV ECX, DWORD PTR DS:[ESI]              ; fifazf.0158F380
	$+78     00D22DE8            83C4 0C                ADD ESP, 0C
	$+7B     00D22DEB            81C1 55450000          ADD ECX, 4555
	$+81     00D22DF1            894C24 28              MOV DWORD PTR SS:[ESP+28], ECX
	$+85     00D22DF5            EB 02                  JMP SHORT fifazf.00D22DF9
	$+87     00D22DF7            8BEB                   MOV EBP, EBX
	$+89     00D22DF9            8D4424 1C              LEA EAX, DWORD PTR SS:[ESP+1C]
	$+8D     00D22DFD            50                     PUSH EAX
	$+8E     00D22DFE            55                     PUSH EBP
	$+8F     00D22DFF            51                     PUSH ECX
	$+90     00D22E00            FF7424 1C              PUSH DWORD PTR SS:[ESP+1C]
	$+94     00D22E04            56                     PUSH ESI
	$+95     00D22E05            E8 6690FFFF            CALL fifazf.00D1BE70
	$+9A     00D22E0A            83C4 14                ADD ESP, 14
	$+9D     00D22E0D            894424 10              MOV DWORD PTR SS:[ESP+10], EAX
	$+A1     00D22E11            85C0                   TEST EAX, EAX
	$+A3     00D22E13            0F85 F6000000          JNZ fifazf.00D22F0F
	$+A9     00D22E19            8B5424 1C              MOV EDX, DWORD PTR SS:[ESP+1C]
	$+AD     00D22E1D            8BC2                   MOV EAX, EDX                             ; d3dx9_43.51A8CC68
	$+AF     00D22E1F            3B5424 24              CMP EDX, DWORD PTR SS:[ESP+24]           ; fifazf.0077F514
	$+B3     00D22E23            8BEA                   MOV EBP, EDX                             ; d3dx9_43.51A8CC68
	$+B5     00D22E25            8B0E                   MOV ECX, DWORD PTR DS:[ESI]              ; fifazf.0158F380
	$+B7     00D22E27            0F474424 24            CMOVA EAX, DWORD PTR SS:[ESP+24]         ; fifazf.0077F514
	$+BC     00D22E2C            2BE8                   SUB EBP, EAX
	$+BE     00D22E2E            894424 24              MOV DWORD PTR SS:[ESP+24], EAX
	$+C2     00D22E32            80B9 D0020000 00       CMP BYTE PTR DS:[ECX+2D0], 0
	$+C9     00D22E39            74 30                  JE SHORT fifazf.00D22E6B
	$+CB     00D22E3B            56                     PUSH ESI
	$+CC     00D22E3C            50                     PUSH EAX
	$+CD     00D22E3D            FF7424 30              PUSH DWORD PTR SS:[ESP+30]
	$+D1     00D22E41            6A 02                  PUSH 2
	$+D3     00D22E43            51                     PUSH ECX
	$+D4     00D22E44            E8 5789FFFF            CALL fifazf.00D1B7A0
	$+D9     00D22E49            83C4 14                ADD ESP, 14
	$+DC     00D22E4C            85ED                   TEST EBP, EBP
	$+DE     00D22E4E            74 26                  JE SHORT fifazf.00D22E76
	$+E0     00D22E50            8B4424 28              MOV EAX, DWORD PTR SS:[ESP+28]
	$+E4     00D22E54            034424 24              ADD EAX, DWORD PTR SS:[ESP+24]
	$+E8     00D22E58            56                     PUSH ESI
	$+E9     00D22E59            55                     PUSH EBP
	$+EA     00D22E5A            50                     PUSH EAX
	$+EB     00D22E5B            6A 04                  PUSH 4
	$+ED     00D22E5D            FF36                   PUSH DWORD PTR DS:[ESI]                  ; fifazf.0158F380
	$+EF     00D22E5F            E8 3C89FFFF            CALL fifazf.00D1B7A0
	$+F4     00D22E64            8B5424 30              MOV EDX, DWORD PTR SS:[ESP+30]
	$+F8     00D22E68            83C4 14                ADD ESP, 14
	$+FB     00D22E6B            85ED                   TEST EBP, EBP
	$+FD     00D22E6D            74 0B                  JE SHORT fifazf.00D22E7A
	$+FF     00D22E6F            016F 28                ADD DWORD PTR DS:[EDI+28], EBP
	$+102    00D22E72            8357 2C 00             ADC DWORD PTR DS:[EDI+2C], 0
	$+106    00D22E76            8B5424 1C              MOV EDX, DWORD PTR SS:[ESP+1C]
	$+10A    00D22E7A            8B4424 20              MOV EAX, DWORD PTR SS:[ESP+20]           ; d3d9.69B9B774
	$+10E    00D22E7E            0110                   ADD DWORD PTR DS:[EAX], EDX              ; d3dx9_43.51A8CC68
	$+110    00D22E80            85FF                   TEST EDI, EDI
	$+112    00D22E82            74 73                  JE SHORT fifazf.00D22EF7
	$+114    00D22E84            3BD3                   CMP EDX, EBX
	$+116    00D22E86            74 66                  JE SHORT fifazf.00D22EEE
	--*/

	addr_tmp = *scan_exe_.ScanImme<void*>("6A0C6A01FF15????????83C408", 6);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_CALLOC  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	malloc
	00F0EC67    2BC2            SUB EAX,EDX
	00F0EC69    8BD8            MOV EBX,EAX
	00F0EC6B    8D43 02         LEA EAX,DWORD PTR DS:[EBX+0x2]
	00F0EC6E    50              PUSH EAX
	00F0EC6F    FF15 A0034501   CALL DWORD PTR DS:[0x14503A0]            ; msvcr100.malloc
	00F0EC75    8BE8            MOV EBP,EAX
	00F0EC77    83C4 04         ADD ESP,0x4
	00F0EC7A    85ED            TEST EBP,EBP

	zhp 2016/8/9 14:39 防止特征码找不到
	$-95     0042FB42       50                             PUSH EAX
	$-94     0042FB43       8D542E 01                      LEA EDX, DWORD PTR DS:[ESI+EBP+1]
	$-90     0042FB47       56                             PUSH ESI
	$-8F     0042FB48       52                             PUSH EDX
	$-8E     0042FB49       FF15 48984E01                  CALL DWORD PTR DS:[14E9848]              ; MSVCR120.memmove
	$-88     0042FB4F       55                             PUSH EBP
	$-87     0042FB50       8D46 01                        LEA EAX, DWORD PTR DS:[ESI+1]
	$-84     0042FB53       53                             PUSH EBX
	$-83     0042FB54       50                             PUSH EAX
	$-82     0042FB55       E8 ECDF1E00                    CALL fifazf.0061DB46                     ; JMP 到 MSVCR120.memcpy
	$-7D     0042FB5A       8B6C24 24                      MOV EBP, DWORD PTR SS:[ESP+24]
	$-79     0042FB5E       C606 2F                        MOV BYTE PTR DS:[ESI], 2F
	$-76     0042FB61       83C4 18                        ADD ESP, 18
	$-73     0042FB64       C603 00                        MOV BYTE PTR DS:[EBX], 0
	$-70     0042FB67       EB 0D                          JMP SHORT fifazf.0042FB76
	$-6E     0042FB69       803E 00                        CMP BYTE PTR DS:[ESI], 0
	$-6B     0042FB6C       75 10                          JNZ SHORT fifazf.0042FB7E
	$-69     0042FB6E       B9 2F000000                    MOV ECX, 2F
	$-64     0042FB73       66:890E                        MOV WORD PTR DS:[ESI], CX
	$-61     0042FB76       C74424 14 01000000             MOV DWORD PTR SS:[ESP+14], 1
	$-59     0042FB7E       803E 3F                        CMP BYTE PTR DS:[ESI], 3F
	$-56     0042FB81       75 23                          JNZ SHORT fifazf.0042FBA6
	$-54     0042FB83       8BC6                           MOV EAX, ESI
	$-52     0042FB85       8D50 01                        LEA EDX, DWORD PTR DS:[EAX+1]
	$-4F     0042FB88       8A08                           MOV CL, BYTE PTR DS:[EAX]
	$-4D     0042FB8A       40                             INC EAX
	$-4C     0042FB8B       84C9                           TEST CL, CL
	$-4A     0042FB8D     ^ 75 F9                          JNZ SHORT fifazf.0042FB88
	$-48     0042FB8F       2BC2                           SUB EAX, EDX
	$-46     0042FB91       40                             INC EAX
	$-45     0042FB92       50                             PUSH EAX
	$-44     0042FB93       8D56 01                        LEA EDX, DWORD PTR DS:[ESI+1]
	$-41     0042FB96       56                             PUSH ESI
	$-40     0042FB97       52                             PUSH EDX
	$-3F     0042FB98       FF15 48984E01                  CALL DWORD PTR DS:[14E9848]              ; MSVCR120.memmove
	$-39     0042FB9E       83C4 0C                        ADD ESP, 0C
	$-36     0042FBA1       C606 2F                        MOV BYTE PTR DS:[ESI], 2F
	$-33     0042FBA4       EB 0B                          JMP SHORT fifazf.0042FBB1
	$-31     0042FBA6       837C24 14 00                   CMP DWORD PTR SS:[ESP+14], 0
	$-2C     0042FBAB       0F84 BF000000                  JE fifazf.0042FC70
	$-26     0042FBB1       8BC6                           MOV EAX, ESI
	$-24     0042FBB3       8D50 01                        LEA EDX, DWORD PTR DS:[EAX+1]
	$-21     0042FBB6       8A08                           MOV CL, BYTE PTR DS:[EAX]
	$-1F     0042FBB8       40                             INC EAX
	$-1E     0042FBB9       84C9                           TEST CL, CL
	$-1C     0042FBBB     ^ 75 F9                          JNZ SHORT fifazf.0042FBB6
	$-1A     0042FBBD       2BC2                           SUB EAX, EDX
	$-18     0042FBBF       894424 14                      MOV DWORD PTR SS:[ESP+14], EAX
	$-14     0042FBC3       8B87 20040000                  MOV EAX, DWORD PTR DS:[EDI+420]
	$-E      0042FBC9       8D50 01                        LEA EDX, DWORD PTR DS:[EAX+1]
	$-B      0042FBCC       8D6424 00                      LEA ESP, DWORD PTR SS:[ESP]
	$-7      0042FBD0       8A08                           MOV CL, BYTE PTR DS:[EAX]
	$-5      0042FBD2       40                             INC EAX
	$-4      0042FBD3       84C9                           TEST CL, CL
	$-2      0042FBD5     ^ 75 F9                          JNZ SHORT fifazf.0042FBD0
	$ ==>    0042FBD7       2BC2                           SUB EAX, EDX
	$+2      0042FBD9       8BD8                           MOV EBX, EAX
	$+4      0042FBDB       8D43 02                        LEA EAX, DWORD PTR DS:[EBX+2]
	$+7      0042FBDE       50                             PUSH EAX
	$+8      0042FBDF       FF15 B42F7801                  CALL DWORD PTR DS:[1782FB4]              ; MSVCR120.malloc  UPDATES_FIFA_REVERSAL_MEMOPER_G_MALLOC
	$+E      0042FBE5       8BE8                           MOV EBP, EAX
	$+10     0042FBE7       83C4 04                        ADD ESP, 4
	$+13     0042FBEA       85ED                           TEST EBP, EBP
	$+15     0042FBEC       75 15                          JNZ SHORT fifazf.0042FC03
	$+17     0042FBEE       5E                             POP ESI                                  ; ntdll.770DFD91
	$+18     0042FBEF       8D45 1B                        LEA EAX, DWORD PTR SS:[EBP+1B]
	$+1B     0042FBF2       5D                             POP EBP                                  ; ntdll.770DFD91
	$+1C     0042FBF3       5B                             POP EBX                                  ; ntdll.770DFD91
	$+1D     0042FBF4       8B4C24 28                      MOV ECX, DWORD PTR SS:[ESP+28]
	$+21     0042FBF8       33CC                           XOR ECX, ESP
	$+23     0042FBFA       E8 5DDA1E00                    CALL fifazf.0061D65C
	$+28     0042FBFF       83C4 2C                        ADD ESP, 2C
	$+2B     0042FC02       C3                             RETN
	$+2C     0042FC03       8B97 20040000                  MOV EDX, DWORD PTR DS:[EDI+420]
	$+32     0042FC09       8BCB                           MOV ECX, EBX
	$+34     0042FC0B       2B4C24 14                      SUB ECX, DWORD PTR SS:[ESP+14]
	$+38     0042FC0F       41                             INC ECX
	$+39     0042FC10       51                             PUSH ECX
	$+3A     0042FC11       52                             PUSH EDX
	$+3B     0042FC12       55                             PUSH EBP
	$+3C     0042FC13       E8 2EDF1E00                    CALL fifazf.0061DB46                     ; JMP 到 MSVCR120.memcpy
	$+41     0042FC18       8B4424 20                      MOV EAX, DWORD PTR SS:[ESP+20]
	$+45     0042FC1C       8D48 01                        LEA ECX, DWORD PTR DS:[EAX+1]
	$+48     0042FC1F       51                             PUSH ECX
	$+49     0042FC20       8BD5                           MOV EDX, EBP
	$+4B     0042FC22       2BD0                           SUB EDX, EAX
	$+4D     0042FC24       8D441A 01                      LEA EAX, DWORD PTR DS:[EDX+EBX+1]
	$+51     0042FC28       56                             PUSH ESI
	$+52     0042FC29       50                             PUSH EAX
	$+53     0042FC2A       E8 17DF1E00                    CALL fifazf.0061DB46                     ; JMP 到 MSVCR120.memcpy
	$+58     0042FC2F       33DB                           XOR EBX, EBX
	$+5A     0042FC31       83C4 18                        ADD ESP, 18
	$+5D     0042FC34       399F 24040000                  CMP DWORD PTR DS:[EDI+424], EBX
	$+63     0042FC3A       74 20                          JE SHORT fifazf.0042FC5C
	$+65     0042FC3C       8B87 20040000                  MOV EAX, DWORD PTR DS:[EDI+420]
	$+6B     0042FC42       3BC3                           CMP EAX, EBX
	$+6D     0042FC44       74 10                          JE SHORT fifazf.0042FC56
	$+6F     0042FC46       50                             PUSH EAX
	$+70     0042FC47       FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$+76     0042FC4D       83C4 04                        ADD ESP, 4
	$+79     0042FC50       899F 20040000                  MOV DWORD PTR DS:[EDI+420], EBX
	$+7F     0042FC56       899F 24040000                  MOV DWORD PTR DS:[EDI+424], EBX
	$+85     0042FC5C       89AF 20040000                  MOV DWORD PTR DS:[EDI+420], EBP
	$+8B     0042FC62       8B6C24 0C                      MOV EBP, DWORD PTR SS:[ESP+C]
	$+8F     0042FC66       C787 24040000 01000000         MOV DWORD PTR DS:[EDI+424], 1
	$+99     0042FC70       8B4C24 20                      MOV ECX, DWORD PTR SS:[ESP+20]
	$+9D     0042FC74       8B5424 1C                      MOV EDX, DWORD PTR SS:[ESP+1C]
	$+A1     0042FC78       8B4424 18                      MOV EAX, DWORD PTR SS:[ESP+18]
	$+A5     0042FC7C       51                             PUSH ECX
	$+A6     0042FC7D       52                             PUSH EDX
	$+A7     0042FC7E       57                             PUSH EDI
	$+A8     0042FC7F       8BCD                           MOV ECX, EBP
	$+AA     0042FC81       E8 6AF8FFFF                    CALL fifazf.0042F4F0
	$+AF     0042FC86       83C4 0C                        ADD ESP, 0C
	$+B2     0042FC89       85C0                           TEST EAX, EAX
	$+B4     0042FC8B       0F85 D8000000                  JNZ fifazf.0042FD69
	$+BA     0042FC91       8B85 80000000                  MOV EAX, DWORD PTR SS:[EBP+80]
	$+C0     0042FC97       8038 5B                        CMP BYTE PTR DS:[EAX], 5B
	$+C3     0042FC9A       0F85 82000000                  JNZ fifazf.0042FD22
	$+C9     0042FCA0       68 CC026101                    PUSH fifazf.016102CC                     ; ASCII "%25"
	$+CE     0042FCA5       50                             PUSH EAX
	$+CF     0042FCA6       FF15 2C984E01                  CALL DWORD PTR DS:[14E982C]              ; MSVCR120.strstr
	$+D5     0042FCAC       8BD8                           MOV EBX, EAX
	$+D7     0042FCAE       83C4 08                        ADD ESP, 8
	$+DA     0042FCB1       85DB                           TEST EBX, EBX
	$+DC     0042FCB3       74 6D                          JE SHORT fifazf.0042FD22
	$+DE     0042FCB5       6A 0A                          PUSH 0A
	$+E0     0042FCB7       8D4424 1C                      LEA EAX, DWORD PTR SS:[ESP+1C]
	$+E4     0042FCBB       50                             PUSH EAX
	$+E5     0042FCBC       8D4B 03                        LEA ECX, DWORD PTR DS:[EBX+3]
	$+E8     0042FCBF       51                             PUSH ECX
	$+E9     0042FCC0       FF15 5C954E01                  CALL DWORD PTR DS:[14E955C]              ; MSVCR120.strtoul
	$+EF     0042FCC6       8B5424 24                      MOV EDX, DWORD PTR SS:[ESP+24]
	$+F3     0042FCCA       83C4 0C                        ADD ESP, 0C
	$+F6     0042FCCD       803A 5D                        CMP BYTE PTR DS:[EDX], 5D
	$+F9     0042FCD0       894424 1C                      MOV DWORD PTR SS:[ESP+1C], EAX
	$+FD     0042FCD4       75 38                          JNZ SHORT fifazf.0042FD0E
	$+FF     0042FCD6       8BC2                           MOV EAX, EDX
	$+101    0042FCD8       8D68 01                        LEA EBP, DWORD PTR DS:[EAX+1]
	$+104    0042FCDB       EB 03                          JMP SHORT fifazf.0042FCE0
	$+106    0042FCDD       8D49 00                        LEA ECX, DWORD PTR DS:[ECX]
	$+109    0042FCE0       8A08                           MOV CL, BYTE PTR DS:[EAX]
	$+10B    0042FCE2       40                             INC EAX
	$+10C    0042FCE3       84C9                           TEST CL, CL
	$+10E    0042FCE5     ^ 75 F9                          JNZ SHORT fifazf.0042FCE0
	$+110    0042FCE7       2BC5                           SUB EAX, EBP
	$+112    0042FCE9       40                             INC EAX
	$+113    0042FCEA       50                             PUSH EAX
	$+114    0042FCEB       52                             PUSH EDX
	$+115    0042FCEC       53                             PUSH EBX
	$+116    0042FCED       FF15 48984E01                  CALL DWORD PTR DS:[14E9848]              ; MSVCR120.memmove
	$+11C    0042FCF3       83C4 0C                        ADD ESP, 0C
	$+11F    0042FCF6       83BF 70850000 00               CMP DWORD PTR DS:[EDI+8570], 0
	$+126    0042FCFD       75 1F                          JNZ SHORT fifazf.0042FD1E
	$+128    0042FCFF       8B4424 0C                      MOV EAX, DWORD PTR SS:[ESP+C]
	$+12C    0042FD03       8B5424 1C                      MOV EDX, DWORD PTR SS:[ESP+1C]
	$+130    0042FD07       8950 70                        MOV DWORD PTR DS:[EAX+70], EDX
	$+133    0042FD0A       8BE8                           MOV EBP, EAX
	$+135    0042FD0C       EB 14                          JMP SHORT fifazf.0042FD22
	$+137    0042FD0E       68 5C026101                    PUSH fifazf.0161025C                     ; ASCII "Invalid IPv6 address format\n"
	$+13C    0042FD13       57                             PUSH EDI
	$+13D    0042FD14       E8 770C0000                    CALL fifazf.00430990
	$+142    0042FD19       83C4 08                        ADD ESP, 8

	zhp 2016/8/25 18:48
	$-D      00D19DB5       ^\75 F9                     JNZ SHORT fifazf.00D19DB0
	$-B      00D19DB7         2BCD                      SUB ECX, EBP
	$-9      00D19DB9         83C6 03                   ADD ESI, 3
	$-6      00D19DBC         03F1                      ADD ESI, ECX
	$-4      00D19DBE         8D42 02                   LEA EAX, DWORD PTR DS:[EDX+2]
	$-1      00D19DC1         50                        PUSH EAX
	$ ==>    00D19DC2         FF15 34E07801             CALL DWORD PTR DS:[178E034]              ; MSVCR120.malloc
	$+6      00D19DC8         8BE8                      MOV EBP, EAX
	$+8      00D19DCA         83C4 04                   ADD ESP, 4
	$+B      00D19DCD         85ED                      TEST EBP, EBP
	$+D      00D19DCF         75 18                     JNZ SHORT fifazf.00D19DE9
	$+F      00D19DD1         5E                        POP ESI                                  ; ntdll.7788FD91
	$+10     00D19DD2         5F                        POP EDI                                  ; ntdll.7788FD91
	$+11     00D19DD3         5D                        POP EBP                                  ; ntdll.7788FD91
	$+12     00D19DD4         B8 1B000000               MOV EAX, 1B
	$+17     00D19DD9         5B                        POP EBX                                  ; ntdll.7788FD91
	$+18     00D19DDA         8B4C24 30                 MOV ECX, DWORD PTR SS:[ESP+30]
	$+1C     00D19DDE         33CC                      XOR ECX, ESP
	$+1E     00D19DE0         E8 FF428DFF               CALL fifazf.005EE0E4
	$+23     00D19DE5         83C4 34                   ADD ESP, 34
	$+26     00D19DE8         C3                        RETN
	$+27     00D19DE9         56                        PUSH ESI
	$+28     00D19DEA         FFB7 08040000             PUSH DWORD PTR DS:[EDI+408]
	$+2E     00D19DF0         55                        PUSH EBP
	$+2F     00D19DF1         E8 E0478DFF               CALL fifazf.005EE5D6                     ; JMP 到 MSVCR120.memcpy
	$+34     00D19DF6         8D43 01                   LEA EAX, DWORD PTR DS:[EBX+1]
	$+37     00D19DF9         8B5C24 30                 MOV EBX, DWORD PTR SS:[ESP+30]
	$+3B     00D19DFD         50                        PUSH EAX
	$+3C     00D19DFE         8D042E                    LEA EAX, DWORD PTR DS:[ESI+EBP]
	$+3F     00D19E01         53                        PUSH EBX
	$+40     00D19E02         50                        PUSH EAX
	$+41     00D19E03         E8 CE478DFF               CALL fifazf.005EE5D6                     ; JMP 到 MSVCR120.memcpy
	$+46     00D19E08         83C4 18                   ADD ESP, 18
	$+49     00D19E0B         80BF 0C040000 00          CMP BYTE PTR DS:[EDI+40C], 0
	$+50     00D19E12         74 20                     JE SHORT fifazf.00D19E34
	$+52     00D19E14         FFB7 08040000             PUSH DWORD PTR DS:[EDI+408]
	$+58     00D19E1A         FF15 38E07801             CALL DWORD PTR DS:[178E038]              ; MSVCR120.free  UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE
	$+5E     00D19E20         83C4 04                   ADD ESP, 4
	$+61     00D19E23         C787 08040000 00000000    MOV DWORD PTR DS:[EDI+408], 0
	$+6B     00D19E2D         C687 0C040000 00          MOV BYTE PTR DS:[EDI+40C], 0
	$+72     00D19E34         55                        PUSH EBP
	$+73     00D19E35         68 B8A96101               PUSH fifazf.0161A9B8                     ; ASCII "Rebuilt URL to: %s\n"
	$+78     00D19E3A         57                        PUSH EDI
	$+79     00D19E3B         E8 201B0000               CALL fifazf.00D1B960
	$+7E     00D19E40         83C4 0C                   ADD ESP, 0C
	$+81     00D19E43         89AF 08040000             MOV DWORD PTR DS:[EDI+408], EBP
	$+87     00D19E49         C687 0C040000 01          MOV BYTE PTR DS:[EDI+40C], 1
	$+8E     00D19E50         FF7424 2C                 PUSH DWORD PTR SS:[ESP+2C]
	$+92     00D19E54         8B6C24 1C                 MOV EBP, DWORD PTR SS:[ESP+1C]
	$+96     00D19E58         FF7424 24                 PUSH DWORD PTR SS:[ESP+24]
	$+9A     00D19E5C         FF7424 24                 PUSH DWORD PTR SS:[ESP+24]
	$+9E     00D19E60         55                        PUSH EBP
	$+9F     00D19E61         57                        PUSH EDI
	$+A0     00D19E62         E8 79F9FFFF               CALL fifazf.00D197E0
	$+A5     00D19E67         83C4 14                   ADD ESP, 14
	$+A8     00D19E6A         85C0                      TEST EAX, EAX
	$+AA     00D19E6C         0F85 FF000000             JNZ fifazf.00D19F71
	$+B0     00D19E72         8B85 98000000             MOV EAX, DWORD PTR SS:[EBP+98]
	$+B6     00D19E78         8038 5B                   CMP BYTE PTR DS:[EAX], 5B
	$+B9     00D19E7B         0F85 A9000000             JNZ fifazf.00D19F2A
	$+BF     00D19E81         6A 25                     PUSH 25
	$+C1     00D19E83         50                        PUSH EAX
	$+C2     00D19E84         A1 50384F01               MOV EAX, DWORD PTR DS:[14F3850]
	$+C7     00D19E89         FFD0                      CALL EAX
	$+C9     00D19E8B         8BF0                      MOV ESI, EAX
	$+CB     00D19E8D         83C4 08                   ADD ESP, 8
	$+CE     00D19E90         85F6                      TEST ESI, ESI
	$+D0     00D19E92         0F84 92000000             JE fifazf.00D19F2A
	$+D6     00D19E98         BD 03000000               MOV EBP, 3
	$+DB     00D19E9D         55                        PUSH EBP
	$+DC     00D19E9E         56                        PUSH ESI
	$+DD     00D19E9F         68 CCA96101               PUSH fifazf.0161A9CC                     ; ASCII "%25"
	$+E2     00D19EA4         FF15 D4344F01             CALL DWORD PTR DS:[14F34D4]              ; MSVCR120.strncmp
	$+E8     00D19EAA         83C4 0C                   ADD ESP, 0C
	$+EB     00D19EAD         85C0                      TEST EAX, EAX
	$+ED     00D19EAF         74 13                     JE SHORT fifazf.00D19EC4
	$+EF     00D19EB1         68 D0A96101               PUSH fifazf.0161A9D0                     ; ASCII "Please URL encode %% as %%25, see RFC 6874.\n"
	$+F4     00D19EB6         57                        PUSH EDI
	$+F5     00D19EB7         E8 A41A0000               CALL fifazf.00D1B960
	$+FA     00D19EBC         83C4 08                   ADD ESP, 8
	$+FD     00D19EBF         BD 01000000               MOV EBP, 1
	$+102    00D19EC4         6A 0A                     PUSH 0A
	$+104    00D19EC6         8D4424 20                 LEA EAX, DWORD PTR SS:[ESP+20]
	$+108    00D19ECA         50                        PUSH EAX
	$+109    00D19ECB         8D042E                    LEA EAX, DWORD PTR DS:[ESI+EBP]
	$+10C    00D19ECE         50                        PUSH EAX
	$+10D    00D19ECF         FF15 5C354F01             CALL DWORD PTR DS:[14F355C]              ; MSVCR120.strtoul
	$+113    00D19ED5         8B5424 28                 MOV EDX, DWORD PTR SS:[ESP+28]
	$+117    00D19ED9         83C4 0C                   ADD ESP, 0C
	$+11A    00D19EDC         894424 20                 MOV DWORD PTR SS:[ESP+20], EAX
	$+11E    00D19EE0         803A 5D                   CMP BYTE PTR DS:[EDX], 5D
	$+121    00D19EE3         75 33                     JNZ SHORT fifazf.00D19F18
	$+123    00D19EE5         8BCA                      MOV ECX, EDX
	$+125    00D19EE7         8D69 01                   LEA EBP, DWORD PTR DS:[ECX+1]
	$+128    00D19EEA         8D9B 00000000             LEA EBX, DWORD PTR DS:[EBX]
	$+12E    00D19EF0         8A01                      MOV AL, BYTE PTR DS:[ECX]
	$+130    00D19EF2         41                        INC ECX
	$+131    00D19EF3         84C0                      TEST AL, AL
	$+133    00D19EF5       ^ 75 F9                     JNZ SHORT fifazf.00D19EF0
	$+135    00D19EF7         2BCD                      SUB ECX, EBP
	$+137    00D19EF9         8D41 01                   LEA EAX, DWORD PTR DS:[ECX+1]
	$+13A    00D19EFC         50                        PUSH EAX
	$+13B    00D19EFD         52                        PUSH EDX
	$+13C    00D19EFE         56                        PUSH ESI
	$+13D    00D19EFF         FF15 58384F01             CALL DWORD PTR DS:[14F3858]              ; MSVCR120.memmove
	$+143    00D19F05         8B6C24 24                 MOV EBP, DWORD PTR SS:[ESP+24]
	$+147    00D19F09         83C4 0C                   ADD ESP, 0C
	$+14A    00D19F0C         8B4424 20                 MOV EAX, DWORD PTR SS:[ESP+20]
	$+14E    00D19F10         8985 88000000             MOV DWORD PTR SS:[EBP+88], EAX
	$+154    00D19F16         EB 12                     JMP SHORT fifazf.00D19F2A
	$+156    00D19F18         68 00AA6101               PUSH fifazf.0161AA00                     ; ASCII "Invalid IPv6 address format\n"
	$+15B    00D19F1D         57                        PUSH EDI
	$+15C    00D19F1E         E8 3D1A0000               CALL fifazf.00D1B960
	$+161    00D19F23         8B6C24 20                 MOV EBP, DWORD PTR SS:[ESP+20]
	$+165    00D19F27         83C4 08                   ADD ESP, 8
	$+168    00D19F2A         8B87 AC030000             MOV EAX, DWORD PTR DS:[EDI+3AC]
	$+16E    00D19F30         85C0                      TEST EAX, EAX
	$+170    00D19F32         74 06                     JE SHORT fifazf.00D19F3A
	$+172    00D19F34         8985 88000000             MOV DWORD PTR SS:[EBP+88], EAX
	$+178    00D19F3A         6A 23                     PUSH 23
	$+17A    00D19F3C         53                        PUSH EBX
	$+17B    00D19F3D         8B1D 50384F01             MOV EBX, DWORD PTR DS:[14F3850]          ; MSVCR120.strchr
	$+181    00D19F43         FFD3                      CALL EBX
	$+183    00D19F45         83C4 08                   ADD ESP, 8
	--*/
	addr_tmp = *scan_exe_.ScanImme<void*>("ff??????????8b??83????85??75??5e5f5db8", 2);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_MALLOC  " << addr_tmp << FC_OFstream_EndlLog;

	//UPDATES_FIFA_REVERSAL_MEMOPER_G_MALLOC与UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE的特征码是在一起的
	addr_tmp = *scan_exe_.ScanImme<void*>("ff??????????83????c7??????????????????c6????????????55", 2);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	free
	00F021CA    8D9B 00000000   LEA EBX,DWORD PTR DS:[EBX]
	00F021D0    8B06            MOV EAX,DWORD PTR DS:[ESI]
	00F021D2    8B7E 04         MOV EDI,DWORD PTR DS:[ESI+0x4]
	00F021D5    85C0            TEST EAX,EAX
	00F021D7    74 10           JE SHORT fifazf.00F021E9
	00F021D9    50              PUSH EAX
	00F021DA    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F021E0    83C4 04         ADD ESP,0x4
	00F021E3    C706 00000000   MOV DWORD PTR DS:[ESI],0x0
	00F021E9    56              PUSH ESI
	00F021EA    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free

	zhp 2016/8/9 14:40 防止特征码找不到
	$-5A     00423510      /74 4A                          JE SHORT fifazf.0042355C
	$-58     00423512      |8B4424 0C                      MOV EAX, DWORD PTR SS:[ESP+C]
	$-54     00423516      |50                             PUSH EAX
	$-53     00423517      |FF15 C02F7801                  CALL DWORD PTR DS:[1782FC0]              ; MSVCR120._strdup
	$-4D     0042351D      |83C4 04                        ADD ESP, 4
	$-4A     00423520      |85C0                           TEST EAX, EAX
	$-48     00423522      |74 2E                          JE SHORT fifazf.00423552
	$-46     00423524      |8906                           MOV DWORD PTR DS:[ESI], EAX
	$-44     00423526      |8B4424 08                      MOV EAX, DWORD PTR SS:[ESP+8]
	$-40     0042352A      |C746 04 00000000               MOV DWORD PTR DS:[ESI+4], 0
	$-39     00423531      |85C0                           TEST EAX, EAX
	$-37     00423533      |74 19                          JE SHORT fifazf.0042354E
	$-35     00423535      |8B48 04                        MOV ECX, DWORD PTR DS:[EAX+4]
	$-32     00423538      |8BD0                           MOV EDX, EAX
	$-30     0042353A      |85C9                           TEST ECX, ECX
	$-2E     0042353C      |74 0B                          JE SHORT fifazf.00423549
	$-2C     0042353E      |8BFF                           MOV EDI, EDI
	$-2A     00423540      |8BD1                           MOV EDX, ECX
	$-28     00423542      |8B4A 04                        MOV ECX, DWORD PTR DS:[EDX+4]
	$-25     00423545      |85C9                           TEST ECX, ECX
	$-23     00423547     ^|75 F7                          JNZ SHORT fifazf.00423540
	$-21     00423549      |8972 04                        MOV DWORD PTR DS:[EDX+4], ESI
	$-1E     0042354C      |5E                             POP ESI                                  ; ntdll.770DFD91
	$-1D     0042354D      |C3                             RETN
	$-1C     0042354E      |8BC6                           MOV EAX, ESI
	$-1A     00423550      |5E                             POP ESI                                  ; ntdll.770DFD91
	$-19     00423551      |C3                             RETN
	$-18     00423552      |56                             PUSH ESI
	$-17     00423553      |FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$-11     00423559      |83C4 04                        ADD ESP, 4
	$-E      0042355C      \33C0                           XOR EAX, EAX
	$-C      0042355E       5E                             POP ESI                                  ; ntdll.770DFD91
	$-B      0042355F       C3                             RETN
	$-A      00423560       56                             PUSH ESI
	$-9      00423561       8B7424 08                      MOV ESI, DWORD PTR SS:[ESP+8]
	$-5      00423565       85F6                           TEST ESI, ESI
	$-3      00423567       74 31                          JE SHORT fifazf.0042359A
	$-1      00423569       57                             PUSH EDI
	$ ==>    0042356A       8D9B 00000000                  LEA EBX, DWORD PTR DS:[EBX]
	$+6      00423570       8B06                           MOV EAX, DWORD PTR DS:[ESI]
	$+8      00423572       8B7E 04                        MOV EDI, DWORD PTR DS:[ESI+4]
	$+B      00423575       85C0                           TEST EAX, EAX
	$+D      00423577       74 10                          JE SHORT fifazf.00423589
	$+F      00423579       50                             PUSH EAX
	$+10     0042357A       FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$+16     00423580       83C4 04                        ADD ESP, 4
	$+19     00423583       C706 00000000                  MOV DWORD PTR DS:[ESI], 0
	$+1F     00423589       56                             PUSH ESI
	$+20     0042358A       FF15 B82F7801                  CALL DWORD PTR DS:[1782FB8]              ; MSVCR120.free
	$+26     00423590       83C4 04                        ADD ESP, 4
	$+29     00423593       8BF7                           MOV ESI, EDI
	$+2B     00423595       85FF                           TEST EDI, EDI
	$+2D     00423597     ^ 75 D7                          JNZ SHORT fifazf.00423570
	$+2F     00423599       5F                             POP EDI                                  ; ntdll.770DFD91
	$+30     0042359A       5E                             POP ESI                                  ; ntdll.770DFD91
	$+31     0042359B       C3                             RETN
	$+32     0042359C       CC                             INT3
	$+33     0042359D       CC                             INT3
	$+34     0042359E       CC                             INT3
	$+35     0042359F       CC                             INT3
	$+36     004235A0       8B4424 04                      MOV EAX, DWORD PTR SS:[ESP+4]            ; KERNELBA.76B13BC8
	$+3A     004235A4       83F8 59                        CMP EAX, 59
	$+3D     004235A7       0F87 E7010000                  JA fifazf.00423794
	$+43     004235AD       FF2485 9C374200                JMP DWORD PTR DS:[EAX*4+42379C]          ; fifazf.004235B4
	$+4A     004235B4       B8 C4EF6001                    MOV EAX, fifazf.0160EFC4                 ; ASCII "No error"
	$+4F     004235B9       C3                             RETN
	$+50     004235BA       B8 ACEF6001                    MOV EAX, fifazf.0160EFAC                 ; ASCII "Unsupported protocol"
	$+55     004235BF       C3                             RETN
	$+56     004235C0       B8 94EF6001                    MOV EAX, fifazf.0160EF94                 ; ASCII "Failed initialization"
	$+5B     004235C5       C3                             RETN
	$+5C     004235C6       B8 68EF6001                    MOV EAX, fifazf.0160EF68                 ; ASCII "URL using bad/illegal format or missing URL"
	$+61     004235CB       C3                             RETN
	$+62     004235CC       B8 F8EE6001                    MOV EAX, fifazf.0160EEF8                 ; ASCII "A requested feature, protocol or option was not found built-in in this libcurl due to a build-time d"
	$+67     004235D1       C3                             RETN
	$+68     004235D2       B8 DCEE6001                    MOV EAX, fifazf.0160EEDC                 ; ASCII "Couldn't resolve proxy name"
	$+6D     004235D7       C3                             RETN
	$+6E     004235D8       B8 C0EE6001                    MOV EAX, fifazf.0160EEC0                 ; ASCII "Couldn't resolve host name"
	$+73     004235DD       C3                             RETN
	$+74     004235DE       B8 A4EE6001                    MOV EAX, fifazf.0160EEA4                 ; ASCII "Couldn't connect to server"
	$+79     004235E3       C3                             RETN
	$+7A     004235E4       B8 8CEE6001                    MOV EAX, fifazf.0160EE8C                 ; ASCII "FTP: weird server reply"
	$+7F     004235E9       C3                             RETN
	$+80     004235EA       B8 68EE6001                    MOV EAX, fifazf.0160EE68                 ; ASCII "Access denied to remote resource"
	$+85     004235EF       C3                             RETN
	$+86     004235F0       B8 38EE6001                    MOV EAX, fifazf.0160EE38                 ; ASCII "FTP: The server failed to connect to data port"
	$+8B     004235F5       C3                             RETN
	$+8C     004235F6       B8 0CEE6001                    MOV EAX, fifazf.0160EE0C                 ; ASCII "FTP: Accepting server connect has timed out"
	$+91     004235FB       C3                             RETN
	$+92     004235FC       B8 D8ED6001                    MOV EAX, fifazf.0160EDD8                 ; ASCII "FTP: The server did not accept the PRET command."
	$+97     00423601       C3                             RETN
	$+98     00423602       B8 C0ED6001                    MOV EAX, fifazf.0160EDC0                 ; ASCII "FTP: unknown PASS reply"
	$+9D     00423607       C3                             RETN
	$+9E     00423608       B8 A8ED6001                    MOV EAX, fifazf.0160EDA8                 ; ASCII "FTP: unknown PASV reply"
	$+A3     0042360D       C3                             RETN
	$+A4     0042360E       B8 84ED6001                    MOV EAX, fifazf.0160ED84                 ; ASCII "FTP: unknown 227 response format"
	$+A9     00423613       C3                             RETN
	$+AA     00423614       B8 50ED6001                    MOV EAX, fifazf.0160ED50                 ; ASCII "FTP: can't figure out the host in the PASV response"
	$+AF     00423619       C3                             RETN
	--*/
	/*addr_tmp = *scan_exe_.ScanImme<void*>("8D9B000000008B068B7E??85C0????50FF15", 0x12);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE  " << addr_tmp << FC_OFstream_EndlLog;*/

	/*++
	按键对象
	00792D27    E8 14883B00     CALL fifazf.00B4B540
	00792D2C    8B10            MOV EDX,DWORD PTR DS:[EAX]
	00792D2E    8B52 40         MOV EDX,DWORD PTR DS:[EDX+0x40]
	00792D31    8BCE            MOV ECX,ESI
	00792D33    C1E9 10         SHR ECX,0x10
	00792D36    0FBFC9          MOVSX ECX,CX
	00792D39    51              PUSH ECX
	00792D3A    0FBFCE          MOVSX ECX,SI
	00792D3D    51              PUSH ECX
	00792D3E    6A 00           PUSH 0x0
	00792D40    6A 01           PUSH 0x1
	00792D42    8BC8            MOV ECX,EAX
	00792D44    FFD2            CALL EDX

	E8????????8B108B52??8BCEC1E9??0FBFC9

	$ ==>    00BC8CD9        E8 12CBDBFF     CALL fifazf.009857F0
	$+5      00BC8CDE        8BCE            MOV ECX,ESI
	$+7      00BC8CE0        C1E9 10         SHR ECX,0x10
	$+A      00BC8CE3        0FBFC9          MOVSX ECX,CX
	$+D      00BC8CE6        8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+F      00BC8CE8        51              PUSH ECX
	$+10     00BC8CE9        0FBFCE          MOVSX ECX,SI
	$+13     00BC8CEC        51              PUSH ECX
	$+14     00BC8CED        6A 00           PUSH 0x0
	$+16     00BC8CEF        6A 01           PUSH 0x1
	$+18     00BC8CF1        8BC8            MOV ECX,EAX
	$+1A     00BC8CF3        FF52 50         CALL DWORD PTR DS:[EDX+0x50]

	zhp 2016/8/9 14:41 防止特征码找不到
	$-57     00BC7B1D       57                             PUSH EDI
	$-56     00BC7B1E       E8 4DD4A3FF                    CALL fifazf.00604F70
	$-51     00BC7B23       83C4 04                        ADD ESP, 4
	$-4E     00BC7B26       85DB                           TEST EBX, EBX
	$-4C     00BC7B28       75 2F                          JNZ SHORT fifazf.00BC7B59
	$-4A     00BC7B2A       6A 64                          PUSH 64
	$-48     00BC7B2C       8D45 98                        LEA EAX, DWORD PTR SS:[EBP-68]
	$-45     00BC7B2F       53                             PUSH EBX
	$-44     00BC7B30       50                             PUSH EAX
	$-43     00BC7B31       E8 1660A5FF                    CALL fifazf.0061DB4C                     ; JMP 到 MSVCR120.memset
	$-3E     00BC7B36       56                             PUSH ESI
	$-3D     00BC7B37       8D45 98                        LEA EAX, DWORD PTR SS:[EBP-68]
	$-3A     00BC7B3A       68 508F5801                    PUSH fifazf.01588F50                     ; ASCII "%08x"
	$-35     00BC7B3F       50                             PUSH EAX
	$-34     00BC7B40       E8 EBC2D7FF                    CALL fifazf.00943E30
	$-2F     00BC7B45       83C4 18                        ADD ESP, 18
	$-2C     00BC7B48       8D45 98                        LEA EAX, DWORD PTR SS:[EBP-68]
	$-29     00BC7B4B       6A 01                          PUSH 1
	$-27     00BC7B4D       50                             PUSH EAX
	$-26     00BC7B4E       FF15 64994E01                  CALL DWORD PTR DS:[14E9964]
	$-20     00BC7B54       E9 CB050000                    JMP fifazf.00BC8124
	$-1B     00BC7B59       6A 08                          PUSH 8
	$-19     00BC7B5B       56                             PUSH ESI
	$-18     00BC7B5C       FF15 68994E01                  CALL DWORD PTR DS:[14E9968]
	$-12     00BC7B62       E9 BD050000                    JMP fifazf.00BC8124
	$-D      00BC7B67       803D 92DF7E01 00               CMP BYTE PTR DS:[17EDF92], 0
	$-6      00BC7B6E       0F84 5A050000                  JE fifazf.00BC80CE
	$ ==>    00BC7B74       E8 D745DBFF                    CALL fifazf.0097C150
	$+5      00BC7B79       8BCE                           MOV ECX, ESI
	$+7      00BC7B7B       C1E9 10                        SHR ECX, 10
	$+A      00BC7B7E       0FBFC9                         MOVSX ECX, CX
	$+D      00BC7B81       8B10                           MOV EDX, DWORD PTR DS:[EAX]
	$+F      00BC7B83       51                             PUSH ECX
	$+10     00BC7B84       0FBFCE                         MOVSX ECX, SI
	$+13     00BC7B87       51                             PUSH ECX
	$+14     00BC7B88       8BC8                           MOV ECX, EAX
	$+16     00BC7B8A       FF52 54                        CALL DWORD PTR DS:[EDX+54]
	$+19     00BC7B8D       56                             PUSH ESI
	$+1A     00BC7B8E       53                             PUSH EBX
	$+1B     00BC7B8F       57                             PUSH EDI
	$+1C     00BC7B90       FFB5 4CFFFFFF                  PUSH DWORD PTR SS:[EBP-B4]               ; fifazf.0063E47D
	$+22     00BC7B96       FF15 F0994E01                  CALL DWORD PTR DS:[14E99F0]              ; ntdll.NtdllDefWindowProc_A
	$+28     00BC7B9C       5F                             POP EDI                                  ; ntdll.770DFD91
	$+29     00BC7B9D       5E                             POP ESI                                  ; ntdll.770DFD91
	$+2A     00BC7B9E       5B                             POP EBX                                  ; ntdll.770DFD91
	$+2B     00BC7B9F       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+2E     00BC7BA2       33CD                           XOR ECX, EBP
	$+30     00BC7BA4       E8 B35AA5FF                    CALL fifazf.0061D65C
	$+35     00BC7BA9       8BE5                           MOV ESP, EBP
	$+37     00BC7BAB       5D                             POP EBP                                  ; ntdll.770DFD91
	$+38     00BC7BAC       C2 1000                        RETN 10
	$+3B     00BC7BAF       81FF 18040000                  CMP EDI, 418
	$+41     00BC7BB5       0F87 13050000                  JA fifazf.00BC80CE
	$+47     00BC7BBB       0F84 03050000                  JE fifazf.00BC80C4
	$+4D     00BC7BC1       8D87 FFFDFFFF                  LEA EAX, DWORD PTR DS:[EDI-201]
	$+53     00BC7BC7       3D 8F000000                    CMP EAX, 8F
	$+58     00BC7BCC       0F87 FC040000                  JA fifazf.00BC80CE
	$+5E     00BC7BD2       0FB680 5482BC00                MOVZX EAX, BYTE PTR DS:[EAX+BC8254]
	$+65     00BC7BD9       FF2485 2882BC00                JMP DWORD PTR DS:[EAX*4+BC8228]          ; fifazf.00BC7BE0
	$+6C     00BC7BE0       803D 92DF7E01 00               CMP BYTE PTR DS:[17EDF92], 0
	$+73     00BC7BE7       0F84 E1040000                  JE fifazf.00BC80CE
	$+79     00BC7BED       E8 5E45DBFF                    CALL fifazf.0097C150
	$+7E     00BC7BF2       8BCE                           MOV ECX, ESI
	$+80     00BC7BF4       C1E9 10                        SHR ECX, 10
	$+83     00BC7BF7       0FBFC9                         MOVSX ECX, CX
	$+86     00BC7BFA       8B10                           MOV EDX, DWORD PTR DS:[EAX]
	$+88     00BC7BFC       51                             PUSH ECX
	$+89     00BC7BFD       0FBFCE                         MOVSX ECX, SI
	$+8C     00BC7C00       51                             PUSH ECX
	$+8D     00BC7C01       6A 00                          PUSH 0
	$+8F     00BC7C03       6A 01                          PUSH 1
	$+91     00BC7C05       8BC8                           MOV ECX, EAX
	$+93     00BC7C07       FF52 50                        CALL DWORD PTR DS:[EDX+50]
	$+96     00BC7C0A       56                             PUSH ESI
	$+97     00BC7C0B       53                             PUSH EBX
	$+98     00BC7C0C       57                             PUSH EDI
	$+99     00BC7C0D       FFB5 4CFFFFFF                  PUSH DWORD PTR SS:[EBP-B4]               ; fifazf.0063E47D
	$+9F     00BC7C13       FF15 F0994E01                  CALL DWORD PTR DS:[14E99F0]              ; ntdll.NtdllDefWindowProc_A
	$+A5     00BC7C19       5F                             POP EDI                                  ; ntdll.770DFD91
	$+A6     00BC7C1A       5E                             POP ESI                                  ; ntdll.770DFD91
	$+A7     00BC7C1B       5B                             POP EBX                                  ; ntdll.770DFD91
	$+A8     00BC7C1C       8B4D FC                        MOV ECX, DWORD PTR SS:[EBP-4]
	$+AB     00BC7C1F       33CD                           XOR ECX, EBP
	$+AD     00BC7C21       E8 365AA5FF                    CALL fifazf.0061D65C
	$+B2     00BC7C26       8BE5                           MOV ESP, EBP
	$+B4     00BC7C28       5D                             POP EBP                                  ; ntdll.770DFD91
	$+B5     00BC7C29       C2 1000                        RETN 10
	$+B8     00BC7C2C       803D 92DF7E01 00               CMP BYTE PTR DS:[17EDF92], 0
	$+BF     00BC7C33       0F84 95040000                  JE fifazf.00BC80CE
	$+C5     00BC7C39       E8 1245DBFF                    CALL fifazf.0097C150
	--*/
	addr_tmp = scan_exe_.ScanCall("E8????????8B??C1????0F????8B????0F", 1);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER  " << addr_tmp << FC_OFstream_EndlLog;

	/*++
	对象基址偏移1
	00BF064F    8BD9            MOV EBX,ECX
	00BF0651    8B73 24         MOV ESI,DWORD PTR DS:[EBX+0x24]
	00BF0654    E8 886EF9FF     CALL fifazf.00B874E1
	00BF0659  ^ 75 8B           JNZ SHORT fifazf.00BF05E6

	8BD98B73??E8
	--*/

	/*addr_tmp = (void*)(int)*scan_exe_.ScanImme<char>("8BD98B73??E8", 4);
	sprintf_s(szPrint, "[FIFA] 对象基址偏移1:0x%p\r\n", dwOffset1);
	OutputDebugStringA(szPrint);*/


	/*++
	对象基址偏移2
	00B80831    8BEC            MOV EBP,ESP
	00B80833    8B49 34         MOV ECX,DWORD PTR DS:[ECX+0x34]
	00B80836    83EC 24         SUB ESP,0x24
	00B80839    56              PUSH ESI
	00B8083A    57              PUSH EDI
	00B8083B    33FF            XOR EDI,EDI
	00B8083D    3BCF            CMP ECX,EDI
	00B8083F    75 27           JNZ SHORT fifazf.00B80868
	00B80841    8B75 08         MOV ESI,DWORD PTR SS:[EBP+0x8]
	00B80844    57              PUSH EDI
	00B80845    C746 14 0F00000>MOV DWORD PTR DS:[ESI+0x14],0xF

	8BEC8B49??83EC??565733FF3BCF75??8B75??57C746
	--*/

	/*DWORD	dwOffset2 = ScanImme((HANDLE)-1, "8BEC8B49??83EC??565733FF3BCF75??8B75??57C746", 4, 1);
	sprintf_s(szPrint, "[FIFA] 对象基址偏移2:0x%p\r\n", dwOffset2);
	OutputDebugStringA(szPrint);*/


	/*++
	对象1偏移3
	00F100B1    83C4 04         ADD ESP,0x4
	00F100B4    899E D4860000   MOV DWORD PTR DS:[ESI+0x86D4],EBX
	00F100BA    899E C0860000   MOV DWORD PTR DS:[ESI+0x86C0],EBX
	00F100C0    899E C4860000   MOV DWORD PTR DS:[ESI+0x86C4],EBX
	00F100C6    899E D0860000   MOV DWORD PTR DS:[ESI+0x86D0],EBX


	83C404899E????????899E????????899E????????899E
	--*/

	// 	DWORD	dwHttpSendCall_NeededObj1 = ScanImme((HANDLE)-1,"83C404899E????????899E????????899E????????899E",0x11);
	// 	wsprintf(szPrint,"[FIFA] 对象1偏移3:0x%p\r\n",dwHttpSendCall_NeededObj1);
	// 	OutputDebugString(szPrint);
	// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj1[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj1);
	// 	OutputDebugString(szPrint);
	// 
	// 	dwOffset1 = 0x0000000C;	// 此特征码是不确定的，存在于VMP中
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_OFFSET1  ";
	// 	LOG << dwOffset1;
	// 	LOG_END;
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_OFFSET2  ";
	// 	LOG << dwOffset2;
	// 	LOG_END;
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_1_OFFSET  ";
	// 	LOG << dwHttpSendCall_NeededObj1;
	// 	LOG_END;

	/*++
	0x8688
	00F0B612    39BE 88860000   CMP DWORD PTR DS:[ESI+0x8688],EDI
	00F0B618    74 14           JE SHORT fifazf.00F0B62E
	00F0B61A    50              PUSH EAX
	00F0B61B    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F0B621    83C4 04         ADD ESP,0x4
	00F0B624    C786 84860000 0>MOV DWORD PTR DS:[ESI+0x8684],0x0
	00F0B62E    89BE 88860000   MOV DWORD PTR DS:[ESI+0x8688],EDI

	39BE????????74??50FF15????????83C404C786
	--*/

	// 	DWORD	dwHttpSendCall_NeededObj2 = ScanImme((HANDLE)-1,"39BE????????74??50FF15????????83C404C786",2);
	// 	wsprintf(szPrint,"[FIFA] 对象2偏移3:0x%p\r\n",dwHttpSendCall_NeededObj2);
	// 	OutputDebugString(szPrint);
	// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj2[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj2);
	// 	OutputDebugString(szPrint);
	// 	
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_2_OFFSET  ";
	// 	LOG << dwHttpSendCall_NeededObj2;
	// 	LOG_END;


	/*++
	0x8684
	00F0B612    39BE 88860000   CMP DWORD PTR DS:[ESI+0x8688],EDI
	00F0B618    74 14           JE SHORT fifazf.00F0B62E
	00F0B61A    50              PUSH EAX
	00F0B61B    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F0B621    83C4 04         ADD ESP,0x4
	00F0B624    C786 84860000 0>MOV DWORD PTR DS:[ESI+0x8684],0x0
	00F0B62E    89BE 88860000   MOV DWORD PTR DS:[ESI+0x8688],EDI

	39BE????????74??50FF15????????83C404C786
	--*/
	// 	DWORD	dwHttpSendCall_NeededObj3 = ScanImme((HANDLE)-1,"39BE????????74??50FF15????????83C404C786",0x14);
	// 	wsprintf(szPrint,"[FIFA] 对象3偏移3:0x%p\r\n",dwHttpSendCall_NeededObj3);
	// 	OutputDebugString(szPrint);
	// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj3[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj3);
	// 	OutputDebugString(szPrint);
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_3_OFFSET  ";
	// 	LOG << dwHttpSendCall_NeededObj3;
	// 	LOG_END;

	/*++
	SSL_read 偏移4
	00F2DE68    B8 FFFFFF7F            MOV EAX,0x7FFFFFFF
	00F2DE6D    8D0C76                 LEA ECX,DWORD PTR DS:[ESI+ESI*2]
	00F2DE70    8B94CB 60010000        MOV EDX,DWORD PTR DS:[EBX+ECX*8+0x160]
	00F2DE77    50                     PUSH EAX
	00F2DE78    8DBCCB 60010000        LEA EDI,DWORD PTR DS:[EBX+ECX*8+0x160]
	00F2DE7F    55                     PUSH EBP
	00F2DE80    52                     PUSH EDX
	00F2DE81    E8 6A800100            CALL fifazf.00F45EF0                     ; SSL_read
	00F2DE86    8BF0                   MOV ESI,EAX
	00F2DE88    83C4 0C                ADD ESP,0xC

	B8FFFFFF7F8D0C768B94CB????????50
	--*/

	//DWORD	dwHttp_SSL_read_Addr4 = ScanImme((HANDLE)-1, "B8FFFFFF7F8D0C768B94CB????????50", 11, 2);

	/*++
	SSL_read 偏移3
	00F188C5   /75 71                  JNZ SHORT fifazf.00F18938
	00F188C7   |83BB C4010000 00       CMP DWORD PTR DS:[EBX+0x1C4],0x0
	00F188CE   |75 68                  JNZ SHORT fifazf.00F18938
	00F188D0   |85ED                   TEST EBP,EBP
	00F188D2   |75 64                  JNZ SHORT fifazf.00F18938
	00F188D4   |E8 A7F9FFFF            CALL fifazf.00F18280
	00F188D9   |85C0                   TEST EAX,EAX
	00F188DB   |74 40                  JE SHORT fifazf.00F1891D
	00F188DD   |899F 58050000          MOV DWORD PTR DS:[EDI+0x558],EBX
	00F188E3   |39AB D0010000          CMP DWORD PTR DS:[EBX+0x1D0],EBP
	00F188E9   |74 08                  JE SHORT fifazf.00F188F3
	00F188EB   |8B83 94000000          MOV EAX,DWORD PTR DS:[EBX+0x94]
	00F188F1   |EB 06                  JMP SHORT fifazf.00F188F9
	00F188F3   |8B83 84000000          MOV EAX,DWORD PTR DS:[EBX+0x84]
	00F188F9   |50                     PUSH EAX
	00F188FA   |8B43 34                MOV EAX,DWORD PTR DS:[EBX+0x34]

	E8????????85C074??899F????????39AB????????74

	--*/
	/*DWORD	dwHttp_SSL_read_Addr3 = ScanImme((HANDLE)-1, "E8????????85C074??899F????????39AB????????74", 0x0b);
	sprintf_s(szPrint, "[FIFA] SSL_read 偏移1:0x%p,偏移2:0x%p,偏移3:0x%p,偏移4:0x%p\r\n", dwOffset1, dwOffset2, dwHttp_SSL_read_Addr3, dwHttp_SSL_read_Addr4);
	OutputDebugStringA(szPrint);*/


	/*++
	用于完全收包后，离开互斥体
	00F1B165   /74 38                  JE SHORT fifazf.00F1B19F
	00F1B167   |8B53 38                MOV EDX,DWORD PTR DS:[EBX+0x38]
	00F1B16A   |8B73 3C                MOV ESI,DWORD PTR DS:[EBX+0x3C]
	00F1B16D   |3BD0                   CMP EDX,EAX
	00F1B16F   |75 04                  JNZ SHORT fifazf.00F1B175
	00F1B171   |3BF1                   CMP ESI,ECX
	00F1B173   |74 2A                  JE SHORT fifazf.00F1B19F
	00F1B175   |83BB D4000000 00       CMP DWORD PTR DS:[EBX+0xD4],0x0
	00F1B17C   |75 21                  JNZ SHORT fifazf.00F1B19F
	...
	00F1B19F    83BB D4020000 00       CMP DWORD PTR DS:[EBX+0x2D4],0x0		// HOOK点
	00F1B1A6    75 2A                  JNZ SHORT fifazf.00F1B1D2
	00F1B1A8    83BB E0000000 00       CMP DWORD PTR DS:[EBX+0xE0],0x0
	00F1B1AF    74 21                  JE SHORT fifazf.00F1B1D2
	00F1B1B1    837F 1C 08             CMP DWORD PTR DS:[EDI+0x1C],0x8
	00F1B1B5    74 1B                  JE SHORT fifazf.00F1B1D2

	zhp 2016/8/9 14:44 防止特征码找不到
	$-90     00436DB5      /74 3D                          JE SHORT fifazf.00436DF4
	$-8E     00436DB7      |51                             PUSH ECX
	$-8D     00436DB8      |8B4B 38                        MOV ECX, DWORD PTR DS:[EBX+38]
	$-8A     00436DBB      |50                             PUSH EAX
	$-89     00436DBC      |8B43 3C                        MOV EAX, DWORD PTR DS:[EBX+3C]
	$-86     00436DBF      |50                             PUSH EAX
	$-85     00436DC0      |8B83 D0040000                  MOV EAX, DWORD PTR DS:[EBX+4D0]
	$-7F     00436DC6      |51                             PUSH ECX
	$-7E     00436DC7      |8B4E 04                        MOV ECX, DWORD PTR DS:[ESI+4]
	$-7B     00436DCA      |52                             PUSH EDX
	$-7A     00436DCB      |8B16                           MOV EDX, DWORD PTR DS:[ESI]
	$-78     00436DCD      |50                             PUSH EAX
	$-77     00436DCE      |51                             PUSH ECX
	$-76     00436DCF      |52                             PUSH EDX
	$-75     00436DD0      |E8 8B97FFFF                    CALL fifazf.00430560
	$-70     00436DD5      |83C4 10                        ADD ESP, 10
	$-6D     00436DD8      |50                             PUSH EAX
	$-6C     00436DD9      |68 08F56001                    PUSH fifazf.0160F508                     ; ASCII "Operation timed out after %ld milliseconds with %lld out of %lld bytes received"
	$-67     00436DDE      |53                             PUSH EBX
	$-66     00436DDF      |E8 3C9CFFFF                    CALL fifazf.00430A20
	$-61     00436DE4      |83C4 1C                        ADD ESP, 1C
	$-5E     00436DE7      |5F                             POP EDI                                  ; ntdll.770DFD91
	$-5D     00436DE8      |5E                             POP ESI                                  ; ntdll.770DFD91
	$-5C     00436DE9      |5D                             POP EBP                                  ; ntdll.770DFD91
	$-5B     00436DEA      |B8 1C000000                    MOV EAX, 1C
	$-56     00436DEF      |5B                             POP EBX                                  ; ntdll.770DFD91
	$-55     00436DF0      |83C4 08                        ADD ESP, 8
	$-52     00436DF3      |C3                             RETN
	$-51     00436DF4      \8B43 3C                        MOV EAX, DWORD PTR DS:[EBX+3C]
	$-4E     00436DF7       8B4B 38                        MOV ECX, DWORD PTR DS:[EBX+38]
	$-4B     00436DFA       50                             PUSH EAX
	$-4A     00436DFB       8B83 D0040000                  MOV EAX, DWORD PTR DS:[EBX+4D0]
	$-44     00436E01       51                             PUSH ECX
	$-43     00436E02       8B4E 04                        MOV ECX, DWORD PTR DS:[ESI+4]
	$-40     00436E05       52                             PUSH EDX
	$-3F     00436E06       8B16                           MOV EDX, DWORD PTR DS:[ESI]
	$-3D     00436E08       50                             PUSH EAX
	$-3C     00436E09       51                             PUSH ECX
	$-3B     00436E0A       52                             PUSH EDX
	$-3A     00436E0B       E8 5097FFFF                    CALL fifazf.00430560
	$-35     00436E10       83C4 10                        ADD ESP, 10
	$-32     00436E13       50                             PUSH EAX
	$-31     00436E14       68 980E6101                    PUSH fifazf.01610E98                     ; ASCII "Operation timed out after %ld milliseconds with %lld bytes received"
	$-2C     00436E19       53                             PUSH EBX
	$-2B     00436E1A       E8 019CFFFF                    CALL fifazf.00430A20
	$-26     00436E1F       83C4 14                        ADD ESP, 14
	$-23     00436E22       5F                             POP EDI                                  ; ntdll.770DFD91
	$-22     00436E23       5E                             POP ESI                                  ; ntdll.770DFD91
	$-21     00436E24       5D                             POP EBP                                  ; ntdll.770DFD91
	$-20     00436E25       B8 1C000000                    MOV EAX, 1C
	$-1B     00436E2A       5B                             POP EBX                                  ; ntdll.770DFD91
	$-1A     00436E2B       83C4 08                        ADD ESP, 8
	$-17     00436E2E       C3                             RETN
	$-16     00436E2F       83BB D4020000 00               CMP DWORD PTR DS:[EBX+2D4], 0
	$-F      00436E36       75 7A                          JNZ SHORT fifazf.00436EB2
	$-D      00436E38       8B43 18                        MOV EAX, DWORD PTR DS:[EBX+18]
	$-A      00436E3B       8B4B 1C                        MOV ECX, DWORD PTR DS:[EBX+1C]
	$-7      00436E3E       8BD0                           MOV EDX, EAX
	$-5      00436E40       23D1                           AND EDX, ECX
	$-3      00436E42       83FA FF                        CMP EDX, -1
	$ ==>    00436E45       74 38                          JE SHORT fifazf.00436E7F
	$+2      00436E47       8B53 38                        MOV EDX, DWORD PTR DS:[EBX+38]
	$+5      00436E4A       8B73 3C                        MOV ESI, DWORD PTR DS:[EBX+3C]
	$+8      00436E4D       3BD0                           CMP EDX, EAX
	$+A      00436E4F       75 04                          JNZ SHORT fifazf.00436E55
	$+C      00436E51       3BF1                           CMP ESI, ECX
	$+E      00436E53       74 2A                          JE SHORT fifazf.00436E7F
	$+10     00436E55       83BB D4000000 00               CMP DWORD PTR DS:[EBX+D4], 0
	$+17     00436E5C       75 21                          JNZ SHORT fifazf.00436E7F
	$+19     00436E5E       2BC2                           SUB EAX, EDX
	$+1B     00436E60       1BCE                           SBB ECX, ESI
	$+1D     00436E62       51                             PUSH ECX
	$+1E     00436E63       50                             PUSH EAX
	$+1F     00436E64       68 600E6101                    PUSH fifazf.01610E60                     ; ASCII "transfer closed with %lld bytes remaining to read"
	$+24     00436E69       53                             PUSH EBX
	$+25     00436E6A       E8 B19BFFFF                    CALL fifazf.00430A20
	$+2A     00436E6F       83C4 10                        ADD ESP, 10
	$+2D     00436E72       5F                             POP EDI                                  ; ntdll.770DFD91
	$+2E     00436E73       5E                             POP ESI                                  ; ntdll.770DFD91
	$+2F     00436E74       5D                             POP EBP                                  ; ntdll.770DFD91
	$+30     00436E75       B8 12000000                    MOV EAX, 12
	$+35     00436E7A       5B                             POP EBX                                  ; ntdll.770DFD91
	$+36     00436E7B       83C4 08                        ADD ESP, 8
	$+39     00436E7E       C3                             RETN
	$+3A     00436E7F       83BB D4020000 00               CMP DWORD PTR DS:[EBX+2D4], 0			;UPDATES_FIFA_REVERSAL_HOOK_AFTER_GAME_READ_FULL_HTTP_PACKAGE
	$+41     00436E86       75 2A                          JNZ SHORT fifazf.00436EB2
	$+43     00436E88       83BB E0000000 00               CMP DWORD PTR DS:[EBX+E0], 0
	$+4A     00436E8F       74 21                          JE SHORT fifazf.00436EB2
	$+4C     00436E91       837F 1C 08                     CMP DWORD PTR DS:[EDI+1C], 8
	$+50     00436E95       74 1B                          JE SHORT fifazf.00436EB2
	$+52     00436E97       68 280E6101                    PUSH fifazf.01610E28                     ; ASCII "transfer closed with outstanding read data remaining"
	$+57     00436E9C       53                             PUSH EBX
	$+58     00436E9D       E8 7E9BFFFF                    CALL fifazf.00430A20
	$+5D     00436EA2       83C4 08                        ADD ESP, 8
	$+60     00436EA5       5F                             POP EDI                                  ; ntdll.770DFD91
	$+61     00436EA6       5E                             POP ESI                                  ; ntdll.770DFD91
	$+62     00436EA7       5D                             POP EBP                                  ; ntdll.770DFD91
	$+63     00436EA8       B8 12000000                    MOV EAX, 12
	$+68     00436EAD       5B                             POP EBX                                  ; 1F98DE90
	$+69     00436EAE       83C4 08                        ADD ESP, 8
	$+6C     00436EB1       C3                             RETN
	$+6D     00436EB2       57                             PUSH EDI
	$+6E     00436EB3       E8 E8190000                    CALL fifazf.004388A0
	$+73     00436EB8       83C4 04                        ADD ESP, 4
	$+76     00436EBB       85C0                           TEST EAX, EAX
	$+78     00436EBD       74 0D                          JE SHORT fifazf.00436ECC
	$+7A     00436EBF       5F                             POP EDI                                  ; ntdll.770DFD91
	$+7B     00436EC0       5E                             POP ESI                                  ; ntdll.770DFD91
	$+7C     00436EC1       5D                             POP EBP                                  ; ntdll.770DFD91
	$+7D     00436EC2       B8 2A000000                    MOV EAX, 2A
	$+82     00436EC7       5B                             POP EBX                                  ; ntdll.770DFD91
	$+83     00436EC8       83C4 08                        ADD ESP, 8
	$+86     00436ECB       C3                             RETN
	$+87     00436ECC       F783 C0000000 33000000         TEST DWORD PTR DS:[EBX+C0], 33
	$+91     00436ED6       8B4C24 20                      MOV ECX, DWORD PTR SS:[ESP+20]
	$+95     00436EDA       B8 00000000                    MOV EAX, 0
	$+9A     00436EDF       0F94C0                         SETE AL
	$+9D     00436EE2       8901                           MOV DWORD PTR DS:[ECX], EAX
	$+9F     00436EE4       33C0                           XOR EAX, EAX
	$+A1     00436EE6       5F                             POP EDI                                  ; ntdll.770DFD91
	$+A2     00436EE7       5E                             POP ESI                                  ; ntdll.770DFD91
	$+A3     00436EE8       5D                             POP EBP                                  ; ntdll.770DFD91
	$+A4     00436EE9       5B                             POP EBX                                  ; ntdll.770DFD91
	$+A5     00436EEA       83C4 08                        ADD ESP, 8
	$+A8     00436EED       C3                             RETN
	$+A9     00436EEE       CC                             INT3
	$+AA     00436EEF       CC                             INT3
	$+AB     00436EF0       8B4C24 04                      MOV ECX, DWORD PTR SS:[ESP+4]            ; KERNELBA.76B13BC8
	$+AF     00436EF4       8B81 38020000                  MOV EAX, DWORD PTR DS:[ECX+238]
	$+B5     00436EFA       8B50 2C                        MOV EDX, DWORD PTR DS:[EAX+2C]
	$+B8     00436EFD       55                             PUSH EBP
	$+B9     00436EFE       56                             PUSH ESI
	$+BA     00436EFF       8B31                           MOV ESI, DWORD PTR DS:[ECX]
	$+BC     00436F01       57                             PUSH EDI
	$+BD     00436F02       33ED                           XOR EBP, EBP
	$+BF     00436F04       33FF                           XOR EDI, EDI
	$+C1     00436F06       85D2                           TEST EDX, EDX
	$+C3     00436F08       74 09                          JE SHORT fifazf.00436F13
	$+C5     00436F0A       5F                             POP EDI                                  ; ntdll.770DFD91
	$+C6     00436F0B       5E                             POP ESI                                  ; ntdll.770DFD91
	$+C7     00436F0C       5D                             POP EBP                                  ; ntdll.770DFD91
	$+C8     00436F0D       894C24 04                      MOV DWORD PTR SS:[ESP+4], ECX
	$+CC     00436F11       FFE2                           JMP EDX
	$+CE     00436F13       837C24 18 02                   CMP DWORD PTR SS:[ESP+18], 2
	$+D3     00436F18       7D 06                          JGE SHORT fifazf.00436F20
	$+D5     00436F1A       5F                             POP EDI                                  ; ntdll.770DFD91
	$+D6     00436F1B       5E                             POP ESI                                  ; ntdll.770DFD91
	$+D7     00436F1C       33C0                           XOR EAX, EAX
	$+D9     00436F1E       5D                             POP EBP                                  ; ntdll.770DFD91
	$+DA     00436F1F       C3                             RETN
	$+DB     00436F20       8B96 C0000000                  MOV EDX, DWORD PTR DS:[ESI+C0]
	$+E1     00436F26       80E2 15                        AND DL, 15
	$+E4     00436F29       53                             PUSH EBX
	$+E5     00436F2A       8B5C24 18                      MOV EBX, DWORD PTR SS:[ESP+18]
	$+E9     00436F2E       80FA 01                        CMP DL, 1
	$+EC     00436F31       75 0D                          JNZ SHORT fifazf.00436F40
	$+EE     00436F33       8B81 44020000                  MOV EAX, DWORD PTR DS:[ECX+244]
	$+F4     00436F39       BD 01000000                    MOV EBP, 1
	$+F9     00436F3E       8903                           MOV DWORD PTR DS:[EBX], EAX
	$+FB     00436F40       8B96 C0000000                  MOV EDX, DWORD PTR DS:[ESI+C0]
	$+101    00436F46       8BC2                           MOV EAX, EDX
	$+103    00436F48       24 2A                          AND AL, 2A
	$+105    00436F4A       3C 02                          CMP AL, 2
	$+107    00436F4C       75 2C                          JNZ SHORT fifazf.00436F7A
	$+109    00436F4E       8B81 48020000                  MOV EAX, DWORD PTR DS:[ECX+248]
	$+10F    00436F54       3981 44020000                  CMP DWORD PTR DS:[ECX+244], EAX
	$+115    00436F5A       75 05                          JNZ SHORT fifazf.00436F61
	$+117    00436F5C       F6C2 01                        TEST DL, 1
	$+11A    00436F5F       75 0D                          JNZ SHORT fifazf.00436F6E
	$+11C    00436F61       F6C2 01                        TEST DL, 1
	$+11F    00436F64       74 05                          JE SHORT fifazf.00436F6B
	$+121    00436F66       BF 01000000                    MOV EDI, 1
	$+126    00436F6B       8904BB                         MOV DWORD PTR DS:[EBX+EDI*4], EAX
	$+129    00436F6E       8D4F 10                        LEA ECX, DWORD PTR DS:[EDI+10]
	$+12C    00436F71       BA 01000000                    MOV EDX, 1
	$+131    00436F76       D3E2                           SHL EDX, CL
	$+133    00436F78       0BEA                           OR EBP, EDX
	$+135    00436F7A       5B                             POP EBX                                  ; ntdll.770DFD91
	$+136    00436F7B       5F                             POP EDI                                  ; ntdll.770DFD91
	$+137    00436F7C       5E                             POP ESI                                  ; ntdll.770DFD91
	$+138    00436F7D       8BC5                           MOV EAX, EBP
	$+13A    00436F7F       5D                             POP EBP                                  ; ntdll.770DFD91
	$+13B    00436F80       C3                             RETN

	2016/8/25 18:58
	$-4E     00D1F5AF         8A87 C7020000             MOV AL, BYTE PTR DS:[EDI+2C7]
	$-48     00D1F5B5         84C0                      TEST AL, AL
	$-46     00D1F5B7         75 75                     JNZ SHORT fifazf.00D1F62E
	$-44     00D1F5B9         8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$-42     00D1F5BB         8BC2                      MOV EAX, EDX
	$-40     00D1F5BD         8B4E 04                   MOV ECX, DWORD PTR DS:[ESI+4]
	$-3D     00D1F5C0         23C1                      AND EAX, ECX
	$-3B     00D1F5C2         83F8 FF                   CMP EAX, -1
	$-38     00D1F5C5         74 36                     JE SHORT fifazf.00D1F5FD
	$-36     00D1F5C7         8B6E 20                   MOV EBP, DWORD PTR DS:[ESI+20]
	$-33     00D1F5CA         8B46 24                   MOV EAX, DWORD PTR DS:[ESI+24]
	$-30     00D1F5CD         3BEA                      CMP EBP, EDX
	$-2E     00D1F5CF         75 04                     JNZ SHORT fifazf.00D1F5D5
	$-2C     00D1F5D1         3BC1                      CMP EAX, ECX
	$-2A     00D1F5D3         74 28                     JE SHORT fifazf.00D1F5FD
	$-28     00D1F5D5         83BF 04010000 00          CMP DWORD PTR DS:[EDI+104], 0
	$-21     00D1F5DC         75 1F                     JNZ SHORT fifazf.00D1F5FD
	$-1F     00D1F5DE         2BD5                      SUB EDX, EBP
	$-1D     00D1F5E0         1BC8                      SBB ECX, EAX
	$-1B     00D1F5E2         51                        PUSH ECX
	$-1A     00D1F5E3         52                        PUSH EDX
	$-19     00D1F5E4         68 A4B76101               PUSH fifazf.0161B7A4                     ; ASCII "transfer closed with %I64d bytes remaining to read"
	$-14     00D1F5E9         57                        PUSH EDI
	$-13     00D1F5EA         E8 A1C2FFFF               CALL fifazf.00D1B890
	$-E      00D1F5EF         83C4 10                   ADD ESP, 10
	$-B      00D1F5F2         B8 12000000               MOV EAX, 12
	$-6      00D1F5F7         5F                        POP EDI                                  ; ntdll.7788FD91
	$-5      00D1F5F8         5E                        POP ESI                                  ; ntdll.7788FD91
	$-4      00D1F5F9         5D                        POP EBP                                  ; ntdll.7788FD91
	$-3      00D1F5FA         5B                        POP EBX                                  ; ntdll.7788FD91
	$-2      00D1F5FB         59                        POP ECX                                  ; ntdll.7788FD91
	$-1      00D1F5FC         C3                        RETN
	$ ==>    00D1F5FD         8A87 C7020000             MOV AL, BYTE PTR DS:[EDI+2C7]
	$+6      00D1F603         84C0                      TEST AL, AL
	$+8      00D1F605         75 27                     JNZ SHORT fifazf.00D1F62E
	$+A      00D1F607         3886 C0000000             CMP BYTE PTR DS:[ESI+C0], AL
	$+10     00D1F60D         74 1F                     JE SHORT fifazf.00D1F62E
	$+12     00D1F60F         837B 20 04                CMP DWORD PTR DS:[EBX+20], 4
	$+16     00D1F613         74 19                     JE SHORT fifazf.00D1F62E
	$+18     00D1F615         68 D8B76101               PUSH fifazf.0161B7D8                     ; ASCII "transfer closed with outstanding read data remaining"
	$+1D     00D1F61A         57                        PUSH EDI
	$+1E     00D1F61B         E8 70C2FFFF               CALL fifazf.00D1B890
	$+23     00D1F620         83C4 08                   ADD ESP, 8
	$+26     00D1F623         B8 12000000               MOV EAX, 12
	$+2B     00D1F628         5F                        POP EDI                                  ; ntdll.7788FD91
	$+2C     00D1F629         5E                        POP ESI                                  ; ntdll.7788FD91
	$+2D     00D1F62A         5D                        POP EBP                                  ; ntdll.7788FD91
	$+2E     00D1F62B         5B                        POP EBX                                  ; ntdll.7788FD91
	$+2F     00D1F62C         59                        POP ECX                                  ; ntdll.7788FD91
	$+30     00D1F62D         C3                        RETN
	$+31     00D1F62E         53                        PUSH EBX
	$+32     00D1F62F         E8 BC620000               CALL fifazf.00D258F0
	$+37     00D1F634         83C4 04                   ADD ESP, 4
	$+3A     00D1F637         85C0                      TEST EAX, EAX
	$+3C     00D1F639         74 0B                     JE SHORT fifazf.00D1F646
	$+3E     00D1F63B         5F                        POP EDI                                  ; ntdll.7788FD91
	$+3F     00D1F63C         5E                        POP ESI                                  ; ntdll.7788FD91
	$+40     00D1F63D         5D                        POP EBP                                  ; ntdll.7788FD91
	$+41     00D1F63E         B8 2A000000               MOV EAX, 2A
	$+46     00D1F643         5B                        POP EBX                                  ; ntdll.7788FD91
	$+47     00D1F644         59                        POP ECX                                  ; ntdll.7788FD91
	$+48     00D1F645         C3                        RETN
	$+49     00D1F646         F786 A8000000 33000000    TEST DWORD PTR DS:[ESI+A8], 33
	$+53     00D1F650         8B4C24 20                 MOV ECX, DWORD PTR SS:[ESP+20]
	$+57     00D1F654         0F94C0                    SETE AL
	$+5A     00D1F657         8801                      MOV BYTE PTR DS:[ECX], AL
	$+5C     00D1F659         33C0                      XOR EAX, EAX
	$+5E     00D1F65B         5F                        POP EDI                                  ; ntdll.7788FD91
	$+5F     00D1F65C         5E                        POP ESI                                  ; ntdll.7788FD91
	$+60     00D1F65D         5D                        POP EBP                                  ; ntdll.7788FD91
	$+61     00D1F65E         5B                        POP EBX                                  ; ntdll.7788FD91
	$+62     00D1F65F         59                        POP ECX                                  ; ntdll.7788FD91
	$+63     00D1F660         C3                        RETN
	--*/
	addr_tmp = scan_exe_.ScanAddr("8a??????????84??75??38??????????74??83", 0);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_HOOK_AFTER_GAME_READ_FULL_HTTP_PACKAGE  " << addr_tmp << FC_OFstream_EndlLog;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 优化代码 特征码

	//OutputDebugStringA("[FIFA] **************游戏优化代码*****************************************************************");
	/*++
	//1. SLEEP 做事线程,让出时间片
	0047824E   /74 14           JE SHORT fifazf.00478264
	00478250   |E8 EB503100     CALL fifazf.0078D340
	00478255   |E8 F64D3100     CALL fifazf.0078D050
	0047825A   |E8 C1533100     CALL fifazf.0078D620
	0047825F   |E8 4C7DB900     CALL fifazf.0100FFB0

	7414E8????????E8????????E8????????E8
	--*/
	// 	DWORD	g_dwHookDoDownCpu = ScanCall((HANDLE)-1,"7414E8????????E8????????E8????????E8",18);
	// 	wsprintf(szPrint,"[FIFA] 做事线程,让出时间片:0x%p\r\n",g_dwHookDoDownCpu);
	// 	OutputDebugString(szPrint);


	/*++
	// 降低主线程的CPU
	0078C0CD    8B81 E8BB5001   MOV EAX,DWORD PTR DS:[ECX+0x150BBE8]
	0078C0D3    40              INC EAX
	0078C0D4    33D2            XOR EDX,EDX
	0078C0D6    BE E8030000     MOV ESI,0x3E8
	0078C0DB    F7F6            DIV ESI
	0078C0DD    8991 E8BB5001   MOV DWORD PTR DS:[ECX+0x150BBE8],EDX
	0078C0E3    5E              POP ESI
	0078C0E4    8BE5            MOV ESP,EBP
	0078C0E6    5D              POP EBP
	0078C0E7    C3              RETN

	8B81????????4033D2BE????????F7F68991????????5E8BE5
	--*/

	// 	DWORD	g_dwHookDoDownCpu1 = ScanAddr((HANDLE)-1,"8B81????????4033D2BE????????F7F68991????????5E8BE5",0x1a);
	// 	wsprintf(szPrint,"[FIFA] 降低主线程的CPU:0x%p\r\n",g_dwHookDoDownCpu1);
	// 	OutputDebugString(szPrint);

	/*++
	2. 优化渲染
	01115050    8B41 0C         MOV EAX,DWORD PTR DS:[ECX+0xC]
	01115053    8B08            MOV ECX,DWORD PTR DS:[EAX]
	01115055    8B51 44         MOV EDX,DWORD PTR DS:[ECX+0x44]
	01115058    6A 00           PUSH 0x0
	0111505A    6A 00           PUSH 0x0
	0111505C    6A 00           PUSH 0x0
	0111505E    6A 00           PUSH 0x0
	01115060    50              PUSH EAX
	01115061    FFD2            CALL EDX
	01115063    85C0            TEST EAX,EAX
	01115065    0F99C0          SETNS AL
	01115068    C2 0400         RETN 0x4
	0111506B    CC              INT3

	50FFD285C00F99C0C20400CC
	--*/
	// 	DWORD g_dwHookPresent = ScanAddr((HANDLE)-1,"50FFD285C00F99C0C20400CC",11);
	// 	wsprintf(szPrint,"[FIFA] 优化渲染:0x%p\r\n",g_dwHookPresent);
	// 	OutputDebugString(szPrint);

	/*++
	// 降低渲染线程的CPU
	005705A9    FFD0            CALL EAX
	005705AB    8B16            MOV EDX,DWORD PTR DS:[ESI]
	005705AD    8B82 E4000000   MOV EAX,DWORD PTR DS:[EDX+0xE4]
	005705B3    8BCE            MOV ECX,ESI
	005705B5    FFD0            CALL EAX
	005705B7    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705B9    8BC8            MOV ECX,EAX
	005705BB    8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	005705BE    FFD0            CALL EAX
	005705C0    5F              POP EDI
	005705C1    5E              POP ESI
	005705C2    B0 01           MOV AL,0x1
	005705C4    5B              POP EBX
	005705C5    C3              RETN

	FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3
	--*/
	// 	DWORD g_dwHookPresent1 = ScanAddr((HANDLE)-1,"FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3",0x17);
	// 	wsprintf(szPrint,"[FIFA] 降低渲染线程的CPU:0x%p\r\n",g_dwHookPresent1);
	// 	OutputDebugString(szPrint);


	/*++
	黑屏优化
	01120E50    8B4E 1C         MOV ECX,DWORD PTR DS:[ESI+0x1C]
	01120E53    8B0499          MOV EAX,DWORD PTR DS:[ECX+EBX*4]
	01120E56    8B48 14         MOV ECX,DWORD PTR DS:[EAX+0x14]
	01120E59    8B40 24         MOV EAX,DWORD PTR DS:[EAX+0x24]
	01120E5C    8B17            MOV EDX,DWORD PTR DS:[EDI]
	01120E5E    51              PUSH ECX
	01120E5F    8B8A 90010000   MOV ECX,DWORD PTR DS:[EDX+0x190]
	01120E65    6A 00           PUSH 0x0
	01120E67    50              PUSH EAX
	01120E68    53              PUSH EBX
	01120E69    57              PUSH EDI
	01120E6A    FFD1            CALL ECX

	8B4E??8B??998B48??8B40??8B17518B8A

	8B????8B????8B????8B????8B????8B??????????6A

	8B????8B??????????6A00??????FF
	--*/
	//这个特征码貌似无用了，没必要了
	//addr_tmp = scan_exe_.ScanAddr("8B4E??8B??998B48??8B40??8B17518B8A",/*14*/-0x70);
	addr_tmp = nullptr;
	//ofs << "#define UPDATES_FIFA_REVERSAL_SET_STREAM_SOURCE  " << addr_tmp << "\t//已无用" << FC_OFstream_EndlLog;

	/*++
	给恒平那边的优化地址
	005705A2    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705A4    8BC8            MOV ECX,EAX
	005705A6    8B42 0C         MOV EAX,DWORD PTR DS:[EDX+0xC]
	005705A9    FFD0            CALL EAX
	005705AB    8B16            MOV EDX,DWORD PTR DS:[ESI]
	005705AD    8B82 E4000000   MOV EAX,DWORD PTR DS:[EDX+0xE4]
	005705B3    8BCE            MOV ECX,ESI
	005705B5    FFD0            CALL EAX
	005705B7    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705B9    8BC8            MOV ECX,EAX
	005705BB    8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	005705BE    FFD0            CALL EAX                                 ; 004C7E80
	005705C0    5F              POP EDI
	005705C1    5E              POP ESI
	005705C2    B0 01           MOV AL,0x1
	005705C4    5B              POP EBX
	005705C5    C3              RETN

	8B108BC88B42??FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3

	0055B13F                    8B16            MOV EDX,DWORD PTR DS:[ESI]
	0055B141                    8B42 68         MOV EAX,DWORD PTR DS:[EDX+0x68]
	0055B144                    8BCE            MOV ECX,ESI
	0055B146                    FFD0            CALL EAX
	0055B148                    8B10            MOV EDX,DWORD PTR DS:[EAX]
	0055B14A                    8BC8            MOV ECX,EAX
	0055B14C                    8B42 04         MOV EAX,DWORD PTR DS:[EDX+0x4]
	0055B14F                    FFD0            CALL EAX
	0055B151                    B0 01           MOV AL,0x1
	0055B153                    5E              POP ESI
	0055B154                    C3              RETN

	8B??8B????8B??FFD08B??8B??8B????FFD0B001??C3
	--*/
	//已无用
	//addr_tmp = scan_exe_.ScanAddr("8B??8B????8B??FFD08B??8B??8B????FFD0B001??C3", -0x9F);		// 改到函数头，函数尾部zhp不知道为何HOOK不成功
	addr_tmp = nullptr;
	//ofs << "#define UPDATES_FIFA_REVERSAL_SET_STREAM_SOURCE_ZHP  " << addr_tmp << "\t//已无用" << FC_OFstream_EndlLog;


	/*++
	主线程优化
	0078C0D3    40              INC EAX
	0078C0D4    33D2            XOR EDX,EDX
	0078C0D6    BE E8030000     MOV ESI,0x3E8
	0078C0DB    F7F6            DIV ESI
	0078C0DD    8991 E8BB5001   MOV DWORD PTR DS:[ECX+0x150BBE8],EDX
	0078C0E3    5E              POP ESI
	0078C0E4    8BE5            MOV ESP,EBP
	0078C0E6    5D              POP EBP
	0078C0E7    C3              RETN

	4033D2BE????????F7F68991????????5E8BE55DC3

	--*/
	//已无用
	//addr_tmp = scan_exe_.ScanAddr("4033D2BE????????F7F68991????????5E8BE55DC3", 0x14);
	addr_tmp = nullptr;
	//ofs << "#define UPDATES_FIFA_REVERSAL_MAIL_THREAD_OPTIMIZE  " << addr_tmp << "\t//已无用" << FC_OFstream_EndlLog;

	/*++

	62A9EA32    8BFF            MOV EDI,EDI
	...
	62A9EA50    8A03            MOV AL,BYTE PTR DS:[EBX]
	62A9EA52    FF4D 08         DEC DWORD PTR SS:[EBP+0x8]
	62A9EA55    8BCF            MOV ECX,EDI
	62A9EA57    E8 7DFFFFFF     CALL fifazfBa.62A9E9D9
	62A9EA5C    43              INC EBX
	62A9EA5D    833E FF         CMP DWORD PTR DS:[ESI],-0x1
	62A9EA60    75 13           JNZ SHORT fifazfBa.62A9EA75

	zhp 2016/8/9 14:47 防止特征码找不到
	$-91     5F1C5C1C       F7C2 01000000                  TEST EDX, 1
	$-8B     5F1C5C22       74 18                          JE SHORT fifazfBa.5F1C5C3C
	$-89     5F1C5C24       8A02                           MOV AL, BYTE PTR DS:[EDX]
	$-87     5F1C5C26       83C2 01                        ADD EDX, 1
	$-84     5F1C5C29       3A01                           CMP AL, BYTE PTR DS:[ECX]
	$-82     5F1C5C2B     ^ 75 E7                          JNZ SHORT fifazfBa.5F1C5C14
	$-80     5F1C5C2D       83C1 01                        ADD ECX, 1
	$-7D     5F1C5C30       0AC0                           OR AL, AL
	$-7B     5F1C5C32     ^ 74 DC                          JE SHORT fifazfBa.5F1C5C10
	$-79     5F1C5C34       F7C2 02000000                  TEST EDX, 2
	$-73     5F1C5C3A     ^ 74 A4                          JE SHORT fifazfBa.5F1C5BE0
	$-71     5F1C5C3C       66:8B02                        MOV AX, WORD PTR DS:[EDX]
	$-6E     5F1C5C3F       83C2 02                        ADD EDX, 2
	$-6B     5F1C5C42       3A01                           CMP AL, BYTE PTR DS:[ECX]
	$-69     5F1C5C44     ^ 75 CE                          JNZ SHORT fifazfBa.5F1C5C14
	$-67     5F1C5C46       0AC0                           OR AL, AL
	$-65     5F1C5C48     ^ 74 C6                          JE SHORT fifazfBa.5F1C5C10
	$-63     5F1C5C4A       3A61 01                        CMP AH, BYTE PTR DS:[ECX+1]
	$-60     5F1C5C4D     ^ 75 C5                          JNZ SHORT fifazfBa.5F1C5C14
	$-5E     5F1C5C4F       0AE4                           OR AH, AH
	$-5C     5F1C5C51     ^ 74 BD                          JE SHORT fifazfBa.5F1C5C10
	$-5A     5F1C5C53       83C1 02                        ADD ECX, 2
	$-57     5F1C5C56     ^ EB 88                          JMP SHORT fifazfBa.5F1C5BE0
	$-55     5F1C5C58       8BFF                           MOV EDI, EDI
	$-53     5F1C5C5A       55                             PUSH EBP
	$-52     5F1C5C5B       8BEC                           MOV EBP, ESP
	$-50     5F1C5C5D       8B45 08                        MOV EAX, DWORD PTR SS:[EBP+8]
	$-4D     5F1C5C60       A3 C4C6265F                    MOV DWORD PTR DS:[5F26C6C4], EAX
	$-48     5F1C5C65       5D                             POP EBP                                  ; ntdll.770DFD91
	$-47     5F1C5C66       C3                             RETN
	$-46     5F1C5C67       8BFF                           MOV EDI, EDI
	$-44     5F1C5C69       55                             PUSH EBP
	$-43     5F1C5C6A       8BEC                           MOV EBP, ESP
	$-41     5F1C5C6C       FF35 C4C6265F                  PUSH DWORD PTR DS:[5F26C6C4]             ; SHELL32.76359D28
	$-3B     5F1C5C72       E8 800C0000                    CALL fifazfBa.5F1C68F7
	$-36     5F1C5C77       59                             POP ECX                                  ; ntdll.770DFD91
	$-35     5F1C5C78       85C0                           TEST EAX, EAX
	$-33     5F1C5C7A       74 0F                          JE SHORT fifazfBa.5F1C5C8B
	$-31     5F1C5C7C       FF75 08                        PUSH DWORD PTR SS:[EBP+8]
	$-2E     5F1C5C7F       FFD0                           CALL EAX
	$-2C     5F1C5C81       59                             POP ECX                                  ; ntdll.770DFD91
	$-2B     5F1C5C82       85C0                           TEST EAX, EAX
	$-29     5F1C5C84       74 05                          JE SHORT fifazfBa.5F1C5C8B
	$-27     5F1C5C86       33C0                           XOR EAX, EAX
	$-25     5F1C5C88       40                             INC EAX
	$-24     5F1C5C89       5D                             POP EBP                                  ; ntdll.770DFD91
	$-23     5F1C5C8A       C3                             RETN
	$-22     5F1C5C8B       33C0                           XOR EAX, EAX
	$-20     5F1C5C8D       5D                             POP EBP                                  ; ntdll.770DFD91
	$-1F     5F1C5C8E       C3                             RETN
	$-1E     5F1C5C8F       8BFF                           MOV EDI, EDI
	$-1C     5F1C5C91       55                             PUSH EBP
	$-1B     5F1C5C92       8BEC                           MOV EBP, ESP
	$-19     5F1C5C94       F647 0C 40                     TEST BYTE PTR DS:[EDI+C], 40
	$-15     5F1C5C98       53                             PUSH EBX
	$-14     5F1C5C99       56                             PUSH ESI
	$-13     5F1C5C9A       8BF0                           MOV ESI, EAX
	$-11     5F1C5C9C       8BD9                           MOV EBX, ECX
	$-F      5F1C5C9E       74 32                          JE SHORT fifazfBa.5F1C5CD2
	$-D      5F1C5CA0       837F 08 00                     CMP DWORD PTR DS:[EDI+8], 0
	$-9      5F1C5CA4       75 2C                          JNZ SHORT fifazfBa.5F1C5CD2
	$-7      5F1C5CA6       8B45 08                        MOV EAX, DWORD PTR SS:[EBP+8]            ; MSVCR120._control87
	$-4      5F1C5CA9       0106                           ADD DWORD PTR DS:[ESI], EAX
	$-2      5F1C5CAB       EB 2B                          JMP SHORT fifazfBa.5F1C5CD8
	$ ==>    5F1C5CAD       8A03                           MOV AL, BYTE PTR DS:[EBX]
	$+2      5F1C5CAF       FF4D 08                        DEC DWORD PTR SS:[EBP+8]
	$+5      5F1C5CB2       8BCF                           MOV ECX, EDI
	$+7      5F1C5CB4       E8 431B0000                    CALL fifazfBa.5F1C77FC
	$+C      5F1C5CB9       43                             INC EBX
	$+D      5F1C5CBA       833E FF                        CMP DWORD PTR DS:[ESI], -1
	$+10     5F1C5CBD       75 13                          JNZ SHORT fifazfBa.5F1C5CD2
	$+12     5F1C5CBF       E8 5093FFFF                    CALL fifazfBa.5F1BF014
	$+17     5F1C5CC4       8338 2A                        CMP DWORD PTR DS:[EAX], 2A
	$+1A     5F1C5CC7       75 0F                          JNZ SHORT fifazfBa.5F1C5CD8
	$+1C     5F1C5CC9       8BCF                           MOV ECX, EDI
	$+1E     5F1C5CCB       B0 3F                          MOV AL, 3F
	$+20     5F1C5CCD       E8 2A1B0000                    CALL fifazfBa.5F1C77FC
	$+25     5F1C5CD2       837D 08 00                     CMP DWORD PTR SS:[EBP+8], 0
	$+29     5F1C5CD6     ^ 7F D5                          JG SHORT fifazfBa.5F1C5CAD
	$+2B     5F1C5CD8       5E                             POP ESI                                  ; ntdll.770DFD91
	$+2C     5F1C5CD9       5B                             POP EBX                                  ; ntdll.770DFD91
	$+2D     5F1C5CDA       5D                             POP EBP                                  ; ntdll.770DFD91
	$+2E     5F1C5CDB       C3                             RETN
	$+2F     5F1C5CDC       8BFF                           MOV EDI, EDI
	$+31     5F1C5CDE       55                             PUSH EBP
	$+32     5F1C5CDF       8BEC                           MOV EBP, ESP
	$+34     5F1C5CE1       81EC 78020000                  SUB ESP, 278
	$+3A     5F1C5CE7       A1 30C6235F                    MOV EAX, DWORD PTR DS:[5F23C630]
	$+3F     5F1C5CEC       33C5                           XOR EAX, EBP
	$+41     5F1C5CEE       8945 FC                        MOV DWORD PTR SS:[EBP-4], EAX
	$+44     5F1C5CF1       53                             PUSH EBX
	$+45     5F1C5CF2       8B5D 0C                        MOV EBX, DWORD PTR SS:[EBP+C]
	$+48     5F1C5CF5       56                             PUSH ESI
	$+49     5F1C5CF6       8B75 08                        MOV ESI, DWORD PTR SS:[EBP+8]
	$+4C     5F1C5CF9       33C0                           XOR EAX, EAX
	$+4E     5F1C5CFB       57                             PUSH EDI
	$+4F     5F1C5CFC       8B7D 14                        MOV EDI, DWORD PTR SS:[EBP+14]
	$+52     5F1C5CFF       FF75 10                        PUSH DWORD PTR SS:[EBP+10]               ; fifazf.01F62FC2
	$+55     5F1C5D02       8D8D A4FDFFFF                  LEA ECX, DWORD PTR SS:[EBP-25C]
	$+5B     5F1C5D08       89B5 B4FDFFFF                  MOV DWORD PTR SS:[EBP-24C], ESI
	$+61     5F1C5D0E       89BD DCFDFFFF                  MOV DWORD PTR SS:[EBP-224], EDI
	$+67     5F1C5D14       8985 B8FDFFFF                  MOV DWORD PTR SS:[EBP-248], EAX
	$+6D     5F1C5D1A       8985 F0FDFFFF                  MOV DWORD PTR SS:[EBP-210], EAX
	$+73     5F1C5D20       8985 CCFDFFFF                  MOV DWORD PTR SS:[EBP-234], EAX
	$+79     5F1C5D26       8985 E8FDFFFF                  MOV DWORD PTR SS:[EBP-218], EAX
	$+7F     5F1C5D2C       8985 D0FDFFFF                  MOV DWORD PTR SS:[EBP-230], EAX
	$+85     5F1C5D32       8985 C0FDFFFF                  MOV DWORD PTR SS:[EBP-240], EAX
	$+8B     5F1C5D38       8985 C8FDFFFF                  MOV DWORD PTR SS:[EBP-238], EAX
	$+91     5F1C5D3E       E8 3A2DFFFF                    CALL fifazfBa.5F1B8A7D
	$+96     5F1C5D43       85F6                           TEST ESI, ESI
	$+98     5F1C5D45       75 35                          JNZ SHORT fifazfBa.5F1C5D7C
	$+9A     5F1C5D47       E8 C892FFFF                    CALL fifazfBa.5F1BF014
	$+9F     5F1C5D4C       C700 16000000                  MOV DWORD PTR DS:[EAX], 16
	$+A5     5F1C5D52       33C0                           XOR EAX, EAX
	$+A7     5F1C5D54       50                             PUSH EAX
	$+A8     5F1C5D55       50                             PUSH EAX
	$+A9     5F1C5D56       50                             PUSH EAX
	$+AA     5F1C5D57       50                             PUSH EAX
	$+AB     5F1C5D58       50                             PUSH EAX
	$+AC     5F1C5D59       E8 B72CFFFF                    CALL fifazfBa.5F1B8A15
	$+B1     5F1C5D5E       83C4 14                        ADD ESP, 14
	$+B4     5F1C5D61       80BD B0FDFFFF 00               CMP BYTE PTR SS:[EBP-250], 0
	$+BB     5F1C5D68       74 0A                          JE SHORT fifazfBa.5F1C5D74
	$+BD     5F1C5D6A       8B85 ACFDFFFF                  MOV EAX, DWORD PTR SS:[EBP-254]
	$+C3     5F1C5D70       8360 70 FD                     AND DWORD PTR DS:[EAX+70], FFFFFFFD
	$+C7     5F1C5D74       83C8 FF                        OR EAX, FFFFFFFF
	$+CA     5F1C5D77       E9 C80A0000                    JMP fifazfBa.5F1C6844
	$+CF     5F1C5D7C       F646 0C 40                     TEST BYTE PTR DS:[ESI+C], 40
	$+D3     5F1C5D80       75 5E                          JNZ SHORT fifazfBa.5F1C5DE0
	$+D5     5F1C5D82       56                             PUSH ESI
	$+D6     5F1C5D83       E8 25460000                    CALL fifazfBa.5F1CA3AD
	$+DB     5F1C5D88       59                             POP ECX                                  ; ntdll.770DFD91
	$+DC     5F1C5D89       BA A0CF235F                    MOV EDX, fifazfBa.5F23CFA0
	$+E1     5F1C5D8E       83F8 FF                        CMP EAX, -1
	$+E4     5F1C5D91       74 1B                          JE SHORT fifazfBa.5F1C5DAE
	$+E6     5F1C5D93       83F8 FE                        CMP EAX, -2
	$+E9     5F1C5D96       74 16                          JE SHORT fifazfBa.5F1C5DAE
	$+EB     5F1C5D98       8BC8                           MOV ECX, EAX
	$+ED     5F1C5D9A       83E1 1F                        AND ECX, 1F
	$+F0     5F1C5D9D       8BF0                           MOV ESI, EAX
	$+F2     5F1C5D9F       C1FE 05                        SAR ESI, 5
	$+F5     5F1C5DA2       C1E1 06                        SHL ECX, 6
	$+F8     5F1C5DA5       030CB5 20242E5F                ADD ECX, DWORD PTR DS:[ESI*4+5F2E2420]
	$+FF     5F1C5DAC       EB 02                          JMP SHORT fifazfBa.5F1C5DB0
	$+101    5F1C5DAE       8BCA                           MOV ECX, EDX
	$+103    5F1C5DB0       F641 24 7F                     TEST BYTE PTR DS:[ECX+24], 7F
	$+107    5F1C5DB4     ^ 75 91                          JNZ SHORT fifazfBa.5F1C5D47
	$+109    5F1C5DB6       83F8 FF                        CMP EAX, -1
	$+10C    5F1C5DB9       74 19                          JE SHORT fifazfBa.5F1C5DD4
	$+10E    5F1C5DBB       83F8 FE                        CMP EAX, -2
	$+111    5F1C5DBE       74 14                          JE SHORT fifazfBa.5F1C5DD4
	$+113    5F1C5DC0       8BC8                           MOV ECX, EAX
	$+115    5F1C5DC2       83E0 1F                        AND EAX, 1F
	$+118    5F1C5DC5       C1F9 05                        SAR ECX, 5
	$+11B    5F1C5DC8       C1E0 06                        SHL EAX, 6
	$+11E    5F1C5DCB       03048D 20242E5F                ADD EAX, DWORD PTR DS:[ECX*4+5F2E2420]
	$+125    5F1C5DD2       EB 02                          JMP SHORT fifazfBa.5F1C5DD6
	$+127    5F1C5DD4       8BC2                           MOV EAX, EDX
	$+129    5F1C5DD6       F640 24 80                     TEST BYTE PTR DS:[EAX+24], 80
	$+12D    5F1C5DDA     ^ 0F85 67FFFFFF                  JNZ fifazfBa.5F1C5D47
	$+133    5F1C5DE0       33C9                           XOR ECX, ECX
	$+135    5F1C5DE2       3BD9                           CMP EBX, ECX
	$+137    5F1C5DE4     ^ 0F84 5DFFFFFF                  JE fifazfBa.5F1C5D47
	$+13D    5F1C5DEA       8A13                           MOV DL, BYTE PTR DS:[EBX]
	$+13F    5F1C5DEC       898D D8FDFFFF                  MOV DWORD PTR SS:[EBP-228], ECX
	$+145    5F1C5DF2       898D E0FDFFFF                  MOV DWORD PTR SS:[EBP-220], ECX
	$+14B    5F1C5DF8       898D BCFDFFFF                  MOV DWORD PTR SS:[EBP-244], ECX
	$+151    5F1C5DFE       8895 EFFDFFFF                  MOV BYTE PTR SS:[EBP-211], DL
	$+157    5F1C5E04       84D2                           TEST DL, DL
	$+159    5F1C5E06       0F84 1F0A0000                  JE fifazfBa.5F1C682B
	$+15F    5F1C5E0C       43                             INC EBX
	$+160    5F1C5E0D       83BD D8FDFFFF 00               CMP DWORD PTR SS:[EBP-228], 0
	$+167    5F1C5E14       899D C4FDFFFF                  MOV DWORD PTR SS:[EBP-23C], EBX
	$+16D    5F1C5E1A       0F8C 0B0A0000                  JL fifazfBa.5F1C682B
	$+173    5F1C5E20       8AC2                           MOV AL, DL
	$+175    5F1C5E22       2C 20                          SUB AL, 20
	$+177    5F1C5E24       3C 58                          CMP AL, 58
	$+179    5F1C5E26       77 11                          JA SHORT fifazfBa.5F1C5E39
	$+17B    5F1C5E28       0FBEC2                         MOVSX EAX, DL
	$+17E    5F1C5E2B       0FBE80 185E215F                MOVSX EAX, BYTE PTR DS:[EAX+5F215E18]
	$+185    5F1C5E32       83E0 0F                        AND EAX, 0F
	$+188    5F1C5E35       33F6                           XOR ESI, ESI
	$+18A    5F1C5E37       EB 04                          JMP SHORT fifazfBa.5F1C5E3D
	$+18C    5F1C5E39       33F6                           XOR ESI, ESI
	$+18E    5F1C5E3B       33C0                           XOR EAX, EAX
	$+190    5F1C5E3D       0FBE84C1 385E215F              MOVSX EAX, BYTE PTR DS:[ECX+EAX*8+5F215E>
	$+198    5F1C5E45       6A 07                          PUSH 7
	$+19A    5F1C5E47       C1F8 04                        SAR EAX, 4
	$+19D    5F1C5E4A       59                             POP ECX                                  ; ntdll.770DFD91
	$+19E    5F1C5E4B       8985 94FDFFFF                  MOV DWORD PTR SS:[EBP-26C], EAX
	--*/
	addr_tmp = scan_fifazfbase_.ScanAddr("8A03FF4D??8BCFE8????????43833E", -0x1E);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_FIFAZFBASE_SPRINTF_ADDR  " << scan_fifazfbase_.OffsetToModuleBegin(addr_tmp) << FC_OFstream_EndlLog;


	/*++
	$ ==>    00830E27        8B40 04         MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+3      00830E2A        8B73 08         MOV ESI,DWORD PTR DS:[EBX+0x8]
	$+6      00830E2D        8BCF            MOV ECX,EDI
	$+8      00830E2F        8946 04         MOV DWORD PTR DS:[ESI+0x4],EAX
	$+B      00830E32        E8 B9056500     CALL fifazf.00E813F0
	$+10     00830E37        8B48 04         MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+13     00830E3A        894E 1C         MOV DWORD PTR DS:[ESI+0x1C],ECX
	$+16     00830E3D        D940 08         FLD DWORD PTR DS:[EAX+0x8]
	$+19     00830E40        8BCF            MOV ECX,EDI                              ; hook 取ecx
	$+1B     00830E42        D95E 20         FSTP DWORD PTR DS:[ESI+0x20]
	$+1E     00830E45        E8 C6C10D00     CALL fifazf.0090D010
	$+23     00830E4A        E8 01BDDCFF     CALL fifazf.005FCB50

	8B????8B????8B??89????E8????????8B????89????D9
	--*/
	// 	DWORD dwFifaRevampMatchTimeHook = ScanAddr((HANDLE)-1,"8B????8B????8B??89????E8????????8B????89????D9",0x19);
	// 	assert(dwFifaRevampMatchTimeHook);
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK  ";
	// 	LOG << dwFifaRevampMatchTimeHook;
	// 	LOG_END;
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET1___  ";
	// 	LOG << 0x44;
	// 	LOG_END;
	// 
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET2___  ";
	// 	LOG << 0x50;
	// 	LOG_END;
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET3___  ";
	// 	LOG << 0x5C;
	// 	LOG_END;
	// 
	// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET4___  ";
	// 	LOG << 0x68;
	// 	LOG_END;

	/*++
	$-A      >  53                   PUSH EBX
	$-9      >  56                   PUSH ESI
	$-8      >  57                   PUSH EDI
	$-7      >  8BF9                 MOV EDI,ECX
	$-5      >  B9 B0047C01          MOV ECX,fifazf.017C04B0
	$ ==>    >  E8 913D0000          CALL fifazf.00669D60
	$+5      >  68 707B4D01          PUSH fifazf.014D7B70
	$+A      >  8D8F 980E0000        LEA ECX,DWORD PTR DS:[EDI+0xE98]
	$+10     >  E8 F1510200          CALL fifazf.0068B1D0
	$+15     >  8A9F 200F0000        MOV BL,BYTE PTR DS:[EDI+0xF20]
	$+1B     >  8D8F 980E0000        LEA ECX,DWORD PTR DS:[EDI+0xE98]
	$+21     >  E8 B05B0200          CALL fifazf.0068BBA0
	$+26     >  84DB                 TEST BL,BL
	$+28     >  74 10                JE SHORT fifazf.00666004
	$+2A     >  80BF 710D0000 00     CMP BYTE PTR DS:[EDI+0xD71],0x0
	$+31     >  75 07                JNZ SHORT fifazf.00666004
	$+33     >  C687 710D0000 01     MOV BYTE PTR DS:[EDI+0xD71],0x1
	$+3A     >  6A 00                PUSH 0x0
	$+3C     >  E8 75A7F7FF          CALL fifazf.005E0780
	$+41     >  83C4 04              ADD ESP,0x4
	$+44     >  E8 CD435600          CALL fifazf.00BCA3E0
	$+49     >  8BC8                 MOV ECX,EAX
	$+4B     >  E8 76485600          CALL fifazf.00BCA890
	$+50     >  68 707B4D01          PUSH fifazf.014D7B70
	$+55     >  B9 C0047C01          MOV ECX,fifazf.017C04C0
	$+5A     >  E8 A7510200          CALL fifazf.0068B1D0
	$+5F     >  8A1D 48057C01        MOV BL,BYTE PTR DS:[0x17C0548]
	$+65     >  B9 C0047C01          MOV ECX,fifazf.017C04C0
	$+6A     >  E8 675B0200          CALL fifazf.0068BBA0

	zhp 2016/8/9 14:48 防止特征码找不到
	$-A      006136C0       53                             PUSH EBX
	$-9      006136C1       56                             PUSH ESI
	$-8      006136C2       57                             PUSH EDI
	$-7      006136C3       8BF9                           MOV EDI, ECX
	$-5      006136C5       B9 B8877D01                    MOV ECX, fifazf.017D87B8
	$ ==>    006136CA       E8 113E0000                    CALL fifazf.006174E0
	$+5      006136CF       68 4CEF4E01                    PUSH fifazf.014EEF4C
	$+A      006136D4       8D8F 980E0000                  LEA ECX, DWORD PTR DS:[EDI+E98]
	$+10     006136DA       E8 916A0200                    CALL fifazf.0063A170
	$+15     006136DF       8A9F 200F0000                  MOV BL, BYTE PTR DS:[EDI+F20]
	$+1B     006136E5       8D8F 980E0000                  LEA ECX, DWORD PTR DS:[EDI+E98]
	$+21     006136EB       E8 50740200                    CALL fifazf.0063AB40
	$+26     006136F0       84DB                           TEST BL, BL
	$+28     006136F2       74 10                          JE SHORT fifazf.00613704
	$+2A     006136F4       80BF 710D0000 00               CMP BYTE PTR DS:[EDI+D71], 0
	$+31     006136FB       75 07                          JNZ SHORT fifazf.00613704
	$+33     006136FD       C687 710D0000 01               MOV BYTE PTR DS:[EDI+D71], 1
	$+3A     00613704       6A 00                          PUSH 0
	$+3C     00613706       E8 25A6F7FF                    CALL fifazf.0058DD30
	$+41     0061370B       83C4 04                        ADD ESP, 4
	$+44     0061370E       E8 1D5E5B00                    CALL fifazf.00BC9530
	$+49     00613713       8BC8                           MOV ECX, EAX
	$+4B     00613715       E8 C6625B00                    CALL fifazf.00BC99E0
	$+50     0061371A       68 4CEF4E01                    PUSH fifazf.014EEF4C
	$+55     0061371F       B9 C8877D01                    MOV ECX, fifazf.017D87C8
	$+5A     00613724       E8 476A0200                    CALL fifazf.0063A170
	$+5F     00613729       8A1D 50887D01                  MOV BL, BYTE PTR DS:[17D8850]
	$+65     0061372F       B9 C8877D01                    MOV ECX, fifazf.017D87C8
	$+6A     00613734       E8 07740200                    CALL fifazf.0063AB40
	$+6F     00613739       84DB                           TEST BL, BL
	$+71     0061373B       74 6D                          JE SHORT fifazf.006137AA
	$+73     0061373D       E8 BEEC3100                    CALL fifazf.00932400
	$+78     00613742       68 4CEF4E01                    PUSH fifazf.014EEF4C
	$+7D     00613747       8D8F 080E0000                  LEA ECX, DWORD PTR DS:[EDI+E08]
	$+83     0061374D       E8 1E6A0200                    CALL fifazf.0063A170
	$+88     00613752       8A9F 900E0000                  MOV BL, BYTE PTR DS:[EDI+E90]
	$+8E     00613758       8D8F 080E0000                  LEA ECX, DWORD PTR DS:[EDI+E08]
	$+94     0061375E       E8 DD730200                    CALL fifazf.0063AB40
	$+99     00613763       84DB                           TEST BL, BL
	$+9B     00613765       74 0A                          JE SHORT fifazf.00613771
	$+9D     00613767       8B4F 10                        MOV ECX, DWORD PTR DS:[EDI+10]
	$+A0     0061376A       E8 118F0000                    CALL fifazf.0061C680
	$+A5     0061376F       EB 52                          JMP SHORT fifazf.006137C3
	$+A7     00613771       68 50144F01                    PUSH fifazf.014F1450                     ; ASCII "RenderSplashScreen"
	$+AC     00613776       E8 858C5500                    CALL fifazf.00B6C400
	$+B1     0061377B       6A 01                          PUSH 1
	$+B3     0061377D       E8 BE8C5500                    CALL fifazf.00B6C440
	$+B8     00613782       A1 B8877D01                    MOV EAX, DWORD PTR DS:[17D87B8]
	$+BD     00613787       83C4 08                        ADD ESP, 8
	$+C0     0061378A       48                             DEC EAX
	$+C1     0061378B       75 11                          JNZ SHORT fifazf.0061379E
	$+C3     0061378D       8B0D 1C807D01                  MOV ECX, DWORD PTR DS:[17D801C]
	$+C9     00613793       85C9                           TEST ECX, ECX
	$+CB     00613795       74 07                          JE SHORT fifazf.0061379E
	$+CD     00613797       8B01                           MOV EAX, DWORD PTR DS:[ECX]
	$+CF     00613799       6A 00                          PUSH 0
	$+D1     0061379B       FF50 0C                        CALL DWORD PTR DS:[EAX+C]
	$+D4     0061379E       E8 8D8E5500                    CALL fifazf.00B6C630
	$+D9     006137A3       E8 58695700                    CALL fifazf.00B8A100
	$+DE     006137A8       EB 19                          JMP SHORT fifazf.006137C3
	$+E0     006137AA       A1 B8877D01                    MOV EAX, DWORD PTR DS:[17D87B8]
	$+E5     006137AF       48                             DEC EAX
	$+E6     006137B0       75 11                          JNZ SHORT fifazf.006137C3
	$+E8     006137B2       8B0D 1C807D01                  MOV ECX, DWORD PTR DS:[17D801C]
	$+EE     006137B8       85C9                           TEST ECX, ECX
	$+F0     006137BA       74 07                          JE SHORT fifazf.006137C3
	$+F2     006137BC       8B01                           MOV EAX, DWORD PTR DS:[ECX]
	$+F4     006137BE       6A 00                          PUSH 0
	$+F6     006137C0       FF50 0C                        CALL DWORD PTR DS:[EAX+C]
	$+F9     006137C3       68 64144F01                    PUSH fifazf.014F1464                     ; ASCII "BMPCAPTURE"
	$+FE     006137C8       6A 01                          PUSH 1
	$+100    006137CA       6A 00                          PUSH 0
	$+102    006137CC       68 585CE20C                    PUSH 0CE25C58
	$+107    006137D1       E8 8AAF0000                    CALL fifazf.0061E760
	$+10C    006137D6       8BC8                           MOV ECX, EAX
	$+10E    006137D8       E8 93AF0000                    CALL fifazf.0061E770
	$+113    006137DD       83F8 01                        CMP EAX, 1
	$+116    006137E0       74 0E                          JE SHORT fifazf.006137F0
	$+118    006137E2       803D 976E7D01 00               CMP BYTE PTR DS:[17D6E97], 0
	$+11F    006137E9       74 05                          JE SHORT fifazf.006137F0
	$+121    006137EB       E8 80934C00                    CALL fifazf.00ADCB70
	$+126    006137F0       68 08084F01                    PUSH fifazf.014F0808                     ; ASCII "DEBUG/LOG"
	$+12B    006137F5       6A 01                          PUSH 1
	$+12D    006137F7       6A 00                          PUSH 0
	$+12F    006137F9       68 BDB931CF                    PUSH CF31B9BD
	$+134    006137FE       E8 5DAF0000                    CALL fifazf.0061E760
	$+139    00613803       8BC8                           MOV ECX, EAX
	$+13B    00613805       E8 66AF0000                    CALL fifazf.0061E770
	$+140    0061380A       8BF0                           MOV ESI, EAX
	$+142    0061380C       85F6                           TEST ESI, ESI
	$+144    0061380E       74 27                          JE SHORT fifazf.00613837
	$+146    00613810       FF15 B09A4E01                  CALL DWORD PTR DS:[14E9AB0]              ; winmm.timeGetTime
	$+14C    00613816       69CE E8030000                  IMUL ECX, ESI, 3E8
	$+152    0061381C       8BD0                           MOV EDX, EAX
	$+154    0061381E       2B15 04967D01                  SUB EDX, DWORD PTR DS:[17D9604]
	$+15A    00613824       3BD1                           CMP EDX, ECX
	$+15C    00613826       72 0F                          JB SHORT fifazf.00613837
	$+15E    00613828       8BCF                           MOV ECX, EDI
	$+160    0061382A       A3 04967D01                    MOV DWORD PTR DS:[17D9604], EAX
	$+165    0061382F       5F                             POP EDI                                  ; ntdll.770DFD91
	$+166    00613830       5E                             POP ESI                                  ; ntdll.770DFD91
	$+167    00613831       5B                             POP EBX                                  ; ntdll.770DFD91
	$+168    00613832     ^ E9 A9DEFFFF                    JMP fifazf.006116E0
	$+16D    00613837       5F                             POP EDI                                  ; ntdll.770DFD91
	$+16E    00613838       5E                             POP ESI                                  ; ntdll.770DFD91
	$+16F    00613839       5B                             POP EBX                                  ; ntdll.770DFD91
	$+170    0061383A       C3                             RETN
	--*/
	addr_tmp = scan_exe_.ScanAddr("E8????????68????????8D??????????E8????????8A??????????8D", -0xA);
	assert(addr_tmp);
	ofs << "#define UPDATES_FIFA_REVERSAL_STOP_RENDER_AND_SLEEP_HOOK  " << addr_tmp << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////
	ofs << "/**************************************特征码更新结束*******************************************/";
	LOG_O(Log_trace) << "ScanCode_Updates_Old更新完毕";
}
