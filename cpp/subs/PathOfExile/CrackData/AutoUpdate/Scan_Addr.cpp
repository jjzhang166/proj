#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Addr(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	//pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-80     007062B0     83EC 10                          SUB ESP, 0x10
	$-7D     007062B3     803D 6801FF00 00                 CMP BYTE PTR DS:[0xFF0168], 0x0
	$-76     007062BA     53                               PUSH EBX
	$-75     007062BB     56                               PUSH ESI
	$-74     007062BC     57                               PUSH EDI                                              ; PathOfEx.00AC802D
	$-73     007062BD     74 16                            JE SHORT PathOfEx.007062D5
	$-71     007062BF     68 6C01FF00                      PUSH PathOfEx.00FF016C
	$-6C     007062C4     B9 5C01FF00                      MOV ECX, PathOfEx.00FF015C
	$-67     007062C9     E8 1217DDFF                      CALL PathOfEx.004D79E0
	$-62     007062CE     C605 6801FF00 00                 MOV BYTE PTR DS:[0xFF0168], 0x0
	$-5B     007062D5     8B35 DC52D800                    MOV ESI, DWORD PTR DS:[0xD852DC]
	$-55     007062DB     8D4424 10                        LEA EAX, DWORD PTR SS:[ESP+0x10]
	$-51     007062DF     50                               PUSH EAX
	$-50     007062E0     FFD6                             CALL NEAR ESI
	$-4E     007062E2     FF35 34670001                    PUSH DWORD PTR DS:[0x1006734]
	$-48     007062E8     FF35 30670001                    PUSH DWORD PTR DS:[0x1006730]
	$-42     007062EE     FF7424 1C                        PUSH DWORD PTR SS:[ESP+0x1C]                          ; PathOfEx.00473DCC
	$-3E     007062F2     FF7424 1C                        PUSH DWORD PTR SS:[ESP+0x1C]                          ; PathOfEx.00473DCC
	$-3A     007062F6     E8 75CF5600                      CALL PathOfEx.00C73270
	$-35     007062FB     833D 4801FF00 00                 CMP DWORD PTR DS:[0xFF0148], 0x0
	$-2E     00706302     8BF8                             MOV EDI, EAX
	$-2C     00706304     8BDA                             MOV EBX, EDX
	$-2A     00706306     75 08                            JNZ SHORT PathOfEx.00706310
	$-28     00706308     8B0D 5801FF00                    MOV ECX, DWORD PTR DS:[0xFF0158]
	$-22     0070630E     EB 18                            JMP SHORT PathOfEx.00706328
	$-20     00706310     8B15 5001FF00                    MOV EDX, DWORD PTR DS:[0xFF0150]                      ; PathOfEx.0096C219
	$-1A     00706316     8BC2                             MOV EAX, EDX
	$-18     00706318     0B05 5401FF00                    OR EAX, DWORD PTR DS:[0xFF0154]
	$-12     0070631E     74 06                            JE SHORT PathOfEx.00706326
	$-10     00706320     8BCF                             MOV ECX, EDI                                          ; PathOfEx.00AC802D
	$-E      00706322     2BCA                             SUB ECX, EDX
	$-C      00706324     EB 02                            JMP SHORT PathOfEx.00706328
	$-A      00706326     33C9                             XOR ECX, ECX
	$-8      00706328     A1 4001FF00                      MOV EAX, DWORD PTR DS:[0xFF0140]
	$-3      0070632D     51                               PUSH ECX
	$-2      0070632E     53                               PUSH EBX
	$-1      0070632F     57                               PUSH EDI                                              ; PathOfEx.00AC802D
	$ ==>    00706330     B9 4001FF00                      MOV ECX, PathOfEx.00FF0140                            ; 游戏基址
	$+5      00706335     FF10                             CALL NEAR DWORD PTR DS:[EAX]
	$+7      00706337     833D 4801FF00 00                 CMP DWORD PTR DS:[0xFF0148], 0x0
	$+E      0070633E     75 43                            JNZ SHORT PathOfEx.00706383
	$+10     00706340     8D4424 10                        LEA EAX, DWORD PTR SS:[ESP+0x10]
	$+14     00706344     50                               PUSH EAX
	$+15     00706345     FFD6                             CALL NEAR ESI
	$+17     00706347     FF35 34670001                    PUSH DWORD PTR DS:[0x1006734]
	$+1D     0070634D     FF35 30670001                    PUSH DWORD PTR DS:[0x1006730]
	$+23     00706353     FF7424 1C                        PUSH DWORD PTR SS:[ESP+0x1C]                          ; PathOfEx.00473DCC
	$+27     00706357     FF7424 1C                        PUSH DWORD PTR SS:[ESP+0x1C]                          ; PathOfEx.00473DCC
	$+2B     0070635B     E8 10CF5600                      CALL PathOfEx.00C73270
	$+30     00706360     8B35 5801FF00                    MOV ESI, DWORD PTR DS:[0xFF0158]
	$+36     00706366     33C9                             XOR ECX, ECX
	$+38     00706368     2BF0                             SUB ESI, EAX
	$+3A     0070636A     1BCA                             SBB ECX, EDX
	$+3C     0070636C     03F7                             ADD ESI, EDI                                          ; PathOfEx.00AC802D
	$+3E     0070636E     13CB                             ADC ECX, EBX
	$+40     00706370     78 11                            JS SHORT PathOfEx.00706383
	$+42     00706372     7F 04                            JG SHORT PathOfEx.00706378
	$+44     00706374     85F6                             TEST ESI, ESI
	$+46     00706376     72 0B                            JB SHORT PathOfEx.00706383
	$+48     00706378     85F6                             TEST ESI, ESI
	$+4A     0070637A     74 07                            JE SHORT PathOfEx.00706383
	$+4C     0070637C     56                               PUSH ESI
	$+4D     0070637D     FF15 B050D800                    CALL NEAR DWORD PTR DS:[0xD850B0]
	$+53     00706383     893D 5001FF00                    MOV DWORD PTR DS:[0xFF0150], EDI                      ; PathOfEx.00AC802D
	$+59     00706389     5F                               POP EDI                                               ; PathOfEx.00AC802D
	$+5A     0070638A     5E                               POP ESI
	$+5B     0070638B     891D 5401FF00                    MOV DWORD PTR DS:[0xFF0154], EBX
	$+61     00706391     5B                               POP EBX
	$+62     00706392     83C4 10                          ADD ESP, 0x10
	$+65     00706395     C3                               RETN
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("b9????????ff??83????????????75??8d??????50ff??ff??????????ff??????????ff??????ff??????e8", 1);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_a_game_base_ = " << ExeAddr2Offset(*op_dw_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-53     00A124D0     55                    PUSH EBP
	$-52     00A124D1     8BEC                  MOV EBP, ESP
	$-50     00A124D3     83E4 F8               AND ESP, -0x8
	$-4D     00A124D6     83EC 4C               SUB ESP, 0x4C
	$-4A     00A124D9     817D 08 00020000      CMP DWORD PTR SS:[EBP+0x8], 0x200
	$-43     00A124E0     53                    PUSH EBX
	$-42     00A124E1     56                    PUSH ESI
	$-41     00A124E2     57                    PUSH EDI
	$-40     00A124E3     8BF9                  MOV EDI, ECX
	$-3E     00A124E5     8B87 B80A0000         MOV EAX, DWORD PTR DS:[EDI+0xAB8]
	$-38     00A124EB     8DB7 B80A0000         LEA ESI, DWORD PTR DS:[EDI+0xAB8]
	$-32     00A124F1     894424 18             MOV DWORD PTR SS:[ESP+0x18], EAX
	$-2E     00A124F5     8B87 D80A0000         MOV EAX, DWORD PTR DS:[EDI+0xAD8]
	$-28     00A124FB     894424 1C             MOV DWORD PTR SS:[ESP+0x1C], EAX
	$-24     00A124FF     8B87 DC0A0000         MOV EAX, DWORD PTR DS:[EDI+0xADC]
	$-1E     00A12505     894424 20             MOV DWORD PTR SS:[ESP+0x20], EAX
	$-1A     00A12509     8B87 BC0A0000         MOV EAX, DWORD PTR DS:[EDI+0xABC]
	$-14     00A1250F     894424 30             MOV DWORD PTR SS:[ESP+0x30], EAX
	$-10     00A12513     8B87 C00A0000         MOV EAX, DWORD PTR DS:[EDI+0xAC0]
	$-A      00A12519     894424 34             MOV DWORD PTR SS:[ESP+0x34], EAX
	$-6      00A1251D     0F85 98000000         JNZ PathOfEx.00A125BB
	$ ==>    00A12523     A1 3CFCFE00           MOV EAX, DWORD PTR DS:[0xFEFC3C]         ; 获取窗口句柄
	$+5      00A12528     8B18                  MOV EBX, DWORD PTR DS:[EAX]
	$+7      00A1252A     8D4424 28             LEA EAX, DWORD PTR SS:[ESP+0x28]
	$+B      00A1252E     50                    PUSH EAX
	$+C      00A1252F     FF15 3454D800         CALL NEAR DWORD PTR DS:[0xD85434]        ; GetCursorPos
	$+12     00A12535     85C0                  TEST EAX, EAX
	$+14     00A12537     74 24                 JE SHORT PathOfEx.00A1255D
	$+16     00A12539     8D4424 28             LEA EAX, DWORD PTR SS:[ESP+0x28]
	$+1A     00A1253D     50                    PUSH EAX
	$+1B     00A1253E     53                    PUSH EBX
	$+1C     00A1253F     FF15 A053D800         CALL NEAR DWORD PTR DS:[0xD853A0]        ; ScreenToClient
	$+22     00A12545     85C0                  TEST EAX, EAX
	$+24     00A12547     74 14                 JE SHORT PathOfEx.00A1255D
	$+26     00A12549     8B4424 28             MOV EAX, DWORD PTR SS:[ESP+0x28]
	$+2A     00A1254D     8B4C24 2C             MOV ECX, DWORD PTR SS:[ESP+0x2C]
	$+2E     00A12551     8987 D80A0000         MOV DWORD PTR DS:[EDI+0xAD8], EAX
	$+34     00A12557     898F DC0A0000         MOV DWORD PTR DS:[EDI+0xADC], ECX
	$+3A     00A1255D     807D 10 00            CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+3E     00A12561     74 07                 JE SHORT PathOfEx.00A1256A
	$+40     00A12563     8BCF                  MOV ECX, EDI
	$+42     00A12565     E8 E6DFFFFF           CALL PathOfEx.00A10550
	$+47     00A1256A     8B4424 1C             MOV EAX, DWORD PTR SS:[ESP+0x1C]
	$+4B     00A1256E     3B87 D80A0000         CMP EAX, DWORD PTR DS:[EDI+0xAD8]
	$+51     00A12574     75 45                 JNZ SHORT PathOfEx.00A125BB
	$+53     00A12576     8B4424 20             MOV EAX, DWORD PTR SS:[ESP+0x20]         ; gdi32.#1005
	$+57     00A1257A     3B87 DC0A0000         CMP EAX, DWORD PTR DS:[EDI+0xADC]
	$+5D     00A12580     75 39                 JNZ SHORT PathOfEx.00A125BB
	$+5F     00A12582     8B4424 18             MOV EAX, DWORD PTR SS:[ESP+0x18]
	$+63     00A12586     3B06                  CMP EAX, DWORD PTR DS:[ESI]
	$+65     00A12588     75 31                 JNZ SHORT PathOfEx.00A125BB
	$+67     00A1258A     F3:0F104424 30        MOVSS XMM0, DWORD PTR SS:[ESP+0x30]
	$+6D     00A12590     0F2E87 BC0A0000       UCOMISS XMM0, DWORD PTR DS:[EDI+0xABC]
	$+74     00A12597     9F                    LAHF
	$+75     00A12598     F6C4 44               TEST AH, 0x44
	$+78     00A1259B     7A 1E                 JPE SHORT PathOfEx.00A125BB
	$+7A     00A1259D     F3:0F104424 34        MOVSS XMM0, DWORD PTR SS:[ESP+0x34]
	$+80     00A125A3     0F2E87 C00A0000       UCOMISS XMM0, DWORD PTR DS:[EDI+0xAC0]
	$+87     00A125AA     9F                    LAHF
	$+88     00A125AB     F6C4 44               TEST AH, 0x44
	$+8B     00A125AE     7A 0B                 JPE SHORT PathOfEx.00A125BB
	$+8D     00A125B0     B0 01                 MOV AL, 0x1
	$+8F     00A125B2     5F                    POP EDI
	$+90     00A125B3     5E                    POP ESI
	$+91     00A125B4     5B                    POP EBX
	$+92     00A125B5     8BE5                  MOV ESP, EBP
	$+94     00A125B7     5D                    POP EBP                                  ; PathOfEx.00953FF5
	$+95     00A125B8     C2 0C00               RETN 0xC
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("a1????????8b??8d??????50ff??????????85??74??8d", 1);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_a_game_base_wnd_info_ = " << ExeAddr2Offset(*op_dw_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-14B    005E6DEF     CC                   INT3
	$-14A    005E6DF0     55                   PUSH EBP
	$-149    005E6DF1     8BEC                 MOV EBP,ESP
	$-147    005E6DF3     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-141    005E6DF9     6A FF                PUSH -0x1
	$-13F    005E6DFB     68 3C94C200          PUSH PathOfEx.00C2943C
	$-13A    005E6E00     50                   PUSH EAX
	$-139    005E6E01     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-132    005E6E08     56                   PUSH ESI
	$-131    005E6E09     57                   PUSH EDI
	$-130    005E6E0A     8BF9                 MOV EDI,ECX
	$-12E    005E6E0C     8D77 5C              LEA ESI,DWORD PTR DS:[EDI+0x5C]
	$-12B    005E6E0F     56                   PUSH ESI
	$-12A    005E6E10     E8 55A85800          CALL PathOfEx.00B7166A
	$-125    005E6E15     83C4 04              ADD ESP,0x4
	$-122    005E6E18     85C0                 TEST EAX,EAX
	$-120    005E6E1A     74 09                JE SHORT PathOfEx.005E6E25
	$-11E    005E6E1C     50                   PUSH EAX
	$-11D    005E6E1D     E8 FDA35800          CALL PathOfEx.00B7121F
	$-118    005E6E22     83C4 04              ADD ESP,0x4
	$-115    005E6E25     8D87 8C000000        LEA EAX,DWORD PTR DS:[EDI+0x8C]
	$-10F    005E6E2B     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-108    005E6E32     50                   PUSH EAX
	$-107    005E6E33     E8 06AB5800          CALL PathOfEx.00B7193E
	$-102    005E6E38     83C4 04              ADD ESP,0x4
	$-FF     005E6E3B     85C0                 TEST EAX,EAX
	$-FD     005E6E3D     74 09                JE SHORT PathOfEx.005E6E48
	$-FB     005E6E3F     50                   PUSH EAX
	$-FA     005E6E40     E8 DAA35800          CALL PathOfEx.00B7121F
	$-F5     005E6E45     83C4 04              ADD ESP,0x4
	$-F2     005E6E48     56                   PUSH ESI
	$-F1     005E6E49     C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
	$-EA     005E6E50     E8 3AA85800          CALL PathOfEx.00B7168F
	$-E5     005E6E55     83C4 04              ADD ESP,0x4
	$-E2     005E6E58     85C0                 TEST EAX,EAX
	$-E0     005E6E5A     74 09                JE SHORT PathOfEx.005E6E65
	$-DE     005E6E5C     50                   PUSH EAX
	$-DD     005E6E5D     E8 BDA35800          CALL PathOfEx.00B7121F
	$-D8     005E6E62     83C4 04              ADD ESP,0x4
	$-D5     005E6E65     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-D2     005E6E68     5F                   POP EDI
	$-D1     005E6E69     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-CA     005E6E70     5E                   POP ESI
	$-C9     005E6E71     8BE5                 MOV ESP,EBP
	$-C7     005E6E73     5D                   POP EBP
	$-C6     005E6E74     C3                   RETN
	$-C5     005E6E75     CC                   INT3
	$-C4     005E6E76     CC                   INT3
	$-C3     005E6E77     CC                   INT3
	$-C2     005E6E78     CC                   INT3
	$-C1     005E6E79     CC                   INT3
	$-C0     005E6E7A     CC                   INT3
	$-BF     005E6E7B     CC                   INT3
	$-BE     005E6E7C     CC                   INT3
	$-BD     005E6E7D     CC                   INT3
	$-BC     005E6E7E     CC                   INT3
	$-BB     005E6E7F     CC                   INT3
	$-BA     005E6E80     55                   PUSH EBP
	$-B9     005E6E81     8BEC                 MOV EBP,ESP
	$-B7     005E6E83     6A FF                PUSH -0x1
	$-B5     005E6E85     68 3E07C500          PUSH PathOfEx.00C5073E
	$-B0     005E6E8A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-AA     005E6E90     50                   PUSH EAX
	$-A9     005E6E91     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-A2     005E6E98     83EC 10              SUB ESP,0x10
	$-9F     005E6E9B     56                   PUSH ESI
	$-9E     005E6E9C     57                   PUSH EDI
	$-9D     005E6E9D     8BF9                 MOV EDI,ECX
	$-9B     005E6E9F     8D77 5C              LEA ESI,DWORD PTR DS:[EDI+0x5C]
	$-98     005E6EA2     56                   PUSH ESI
	$-97     005E6EA3     8975 E4              MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-94     005E6EA6     E8 BFA75800          CALL PathOfEx.00B7166A
	$-8F     005E6EAB     83C4 04              ADD ESP,0x4
	$-8C     005E6EAE     85C0                 TEST EAX,EAX
	$-8A     005E6EB0     74 09                JE SHORT PathOfEx.005E6EBB
	$-88     005E6EB2     50                   PUSH EAX
	$-87     005E6EB3     E8 67A35800          CALL PathOfEx.00B7121F
	$-82     005E6EB8     83C4 04              ADD ESP,0x4
	$-7F     005E6EBB     C645 E8 01           MOV BYTE PTR SS:[EBP-0x18],0x1
	$-7B     005E6EBF     8D45 EC              LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-78     005E6EC2     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-71     005E6EC9     50                   PUSH EAX
	$-70     005E6ECA     8D45 E4              LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-6D     005E6ECD     C745 EC 01000000     MOV DWORD PTR SS:[EBP-0x14],0x1
	$-66     005E6ED4     50                   PUSH EAX
	$-65     005E6ED5     8D8F 8C000000        LEA ECX,DWORD PTR DS:[EDI+0x8C]
	$-5F     005E6EDB     C745 F0 00000000     MOV DWORD PTR SS:[EBP-0x10],0x0
	$-58     005E6EE2     E8 A9180000          CALL PathOfEx.005E8790
	$-53     005E6EE7     56                   PUSH ESI
	$-52     005E6EE8     C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
	$-4B     005E6EEF     E8 9BA75800          CALL PathOfEx.00B7168F
	$-46     005E6EF4     83C4 04              ADD ESP,0x4
	$-43     005E6EF7     5F                   POP EDI
	$-42     005E6EF8     5E                   POP ESI
	$-41     005E6EF9     85C0                 TEST EAX,EAX
	$-3F     005E6EFB     74 09                JE SHORT PathOfEx.005E6F06
	$-3D     005E6EFD     50                   PUSH EAX
	$-3C     005E6EFE     E8 1CA35800          CALL PathOfEx.00B7121F
	$-37     005E6F03     83C4 04              ADD ESP,0x4
	$-34     005E6F06     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-31     005E6F09     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-2A     005E6F10     8BE5                 MOV ESP,EBP
	$-28     005E6F12     5D                   POP EBP
	$-27     005E6F13     C3                   RETN
	$-26     005E6F14     CC                   INT3
	$-25     005E6F15     CC                   INT3
	$-24     005E6F16     CC                   INT3
	$-23     005E6F17     CC                   INT3
	$-22     005E6F18     CC                   INT3
	$-21     005E6F19     CC                   INT3
	$-20     005E6F1A     CC                   INT3
	$-1F     005E6F1B     CC                   INT3
	$-1E     005E6F1C     CC                   INT3
	$-1D     005E6F1D     CC                   INT3
	$-1C     005E6F1E     CC                   INT3
	$-1B     005E6F1F     CC                   INT3
	$-1A     005E6F20     55                   PUSH EBP                                               ; stCD_ThrdParam::ThrdRun
	$-19     005E6F21     8BEC                 MOV EBP,ESP
	$-17     005E6F23     6A FF                PUSH -0x1
	$-15     005E6F25     68 5807C500          PUSH PathOfEx.00C50758
	$-10     005E6F2A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-A      005E6F30     50                   PUSH EAX
	$-9      005E6F31     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-2      005E6F38     51                   PUSH ECX
	$-1      005E6F39     56                   PUSH ESI
	$ ==>    005E6F3A     C745 F0 00000000     MOV DWORD PTR SS:[EBP-0x10],0x0
	$+7      005E6F41     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+A      005E6F44     8D4D F0              LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+D      005E6F47     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+14     005E6F4E     8B56 04              MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+17     005E6F51     E8 9A090000          CALL PathOfEx.005E78F0
	$+1C     005E6F56     FF15 FC70C900        CALL DWORD PTR DS:[<&KERNEL32.GetCurrentThreadId>]     ; kernel32.GetCurrentThreadId
	$+22     005E6F5C     64:8B15 2C000000     MOV EDX,DWORD PTR FS:[0x2C]
	$+29     005E6F63     8BCE                 MOV ECX,ESI
	$+2B     005E6F65     8B12                 MOV EDX,DWORD PTR DS:[EDX]
	$+2D     005E6F67     8942 0C              MOV DWORD PTR DS:[EDX+0xC],EAX
	$+30     005E6F6A     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+32     005E6F6C     8942 10              MOV DWORD PTR DS:[EDX+0x10],EAX
	$+35     005E6F6F     8B46 04              MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+38     005E6F72     8942 14              MOV DWORD PTR DS:[EDX+0x14],EAX
	$+3B     005E6F75     E8 36000000          CALL PathOfEx.005E6FB0
	$+40     005E6F7A     8B75 F0              MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+43     005E6F7D     85F6                 TEST ESI,ESI
	$+45     005E6F7F     74 15                JE SHORT PathOfEx.005E6F96
	$+47     005E6F81     8BCE                 MOV ECX,ESI
	$+49     005E6F83     E8 E81BFDFF          CALL PathOfEx.005B8B70
	$+4E     005E6F88     68 F0000000          PUSH 0xF0
	$+53     005E6F8D     56                   PUSH ESI
	$+54     005E6F8E     E8 44A95900          CALL PathOfEx.00B818D7
	$+59     005E6F93     83C4 08              ADD ESP,0x8
	$+5C     005E6F96     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+5F     005E6F99     5E                   POP ESI
	$+60     005E6F9A     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+67     005E6FA1     8BE5                 MOV ESP,EBP
	$+69     005E6FA3     5D                   POP EBP
	$+6A     005E6FA4     C3                   RETN
	$+6B     005E6FA5     CC                   INT3
	$+6C     005E6FA6     CC                   INT3
	$+6D     005E6FA7     CC                   INT3
	$+6E     005E6FA8     CC                   INT3
	$+6F     005E6FA9     CC                   INT3
	$+70     005E6FAA     CC                   INT3
	$+71     005E6FAB     CC                   INT3
	$+72     005E6FAC     CC                   INT3
	$+73     005E6FAD     CC                   INT3
	$+74     005E6FAE     CC                   INT3
	$+75     005E6FAF     CC                   INT3
	$+76     005E6FB0     A0 EEDFF000          MOV AL,BYTE PTR DS:[0xF0DFEE]
	$+7B     005E6FB5     56                   PUSH ESI
	$+7C     005E6FB6     8BF1                 MOV ESI,ECX
	$+7E     005E6FB8     84C0                 TEST AL,AL
	$+80     005E6FBA     74 6B                JE SHORT PathOfEx.005E7027
	$+82     005E6FBC     0F1F40 00            NOP DWORD PTR DS:[EAX]
	$+86     005E6FC0     84C0                 TEST AL,AL
	$+88     005E6FC2     74 63                JE SHORT PathOfEx.005E7027
	$+8A     005E6FC4     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+8C     005E6FC6     8B0D 9007F300        MOV ECX,DWORD PTR DS:[0xF30790]
	$+92     005E6FCC     6A 01                PUSH 0x1
	$+94     005E6FCE     8B0C81               MOV ECX,DWORD PTR DS:[ECX+EAX*4]
	$+97     005E6FD1     E8 7AFDFFFF          CALL PathOfEx.005E6D50
	$+9C     005E6FD6     8BC8                 MOV ECX,EAX
	$+9E     005E6FD8     E8 93010000          CALL PathOfEx.005E7170
	$+A3     005E6FDD     84C0                 TEST AL,AL
	$+A5     005E6FDF     74 07                JE SHORT PathOfEx.005E6FE8
	$+A7     005E6FE1     A0 EEDFF000          MOV AL,BYTE PTR DS:[0xF0DFEE]
	$+AC     005E6FE6   ^ EB D8                JMP SHORT PathOfEx.005E6FC0
	$+AE     005E6FE8     803D EEDFF000 00     CMP BYTE PTR DS:[0xF0DFEE],0x0
	$+B5     005E6FEF     74 36                JE SHORT PathOfEx.005E7027
	$+B7     005E6FF1     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+B9     005E6FF3     8B0D 9007F300        MOV ECX,DWORD PTR DS:[0xF30790]
	$+BF     005E6FF9     6A 00                PUSH 0x0
	$+C1     005E6FFB     8B0C81               MOV ECX,DWORD PTR DS:[ECX+EAX*4]
	$+C4     005E6FFE     E8 4DFDFFFF          CALL PathOfEx.005E6D50
	$+C9     005E7003     8BC8                 MOV ECX,EAX
	$+CB     005E7005     E8 66010000          CALL PathOfEx.005E7170
	$+D0     005E700A     84C0                 TEST AL,AL
	$+D2     005E700C     75 10                JNZ SHORT PathOfEx.005E701E
	$+D4     005E700E     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+D6     005E7010     8B0D 9007F300        MOV ECX,DWORD PTR DS:[0xF30790]
	$+DC     005E7016     8B0C81               MOV ECX,DWORD PTR DS:[ECX+EAX*4]
	$+DF     005E7019     E8 62FEFFFF          CALL PathOfEx.005E6E80
	$+E4     005E701E     A0 EEDFF000          MOV AL,BYTE PTR DS:[0xF0DFEE]
	$+E9     005E7023     84C0                 TEST AL,AL
	$+EB     005E7025   ^ 75 9D                JNZ SHORT PathOfEx.005E6FC4
	$+ED     005E7027     5E                   POP ESI
	$+EE     005E7028     C3                   RETN
	$+EF     005E7029     CC                   INT3
	$+F0     005E702A     CC                   INT3
	$+F1     005E702B     CC                   INT3
	$+F2     005E702C     CC                   INT3
	$+F3     005E702D     CC                   INT3
	$+F4     005E702E     CC                   INT3
	$+F5     005E702F     CC                   INT3
	$+F6     005E7030     53                   PUSH EBX
	$+F7     005E7031     8AD9                 MOV BL,CL
	$+F9     005E7033     E8 319F5800          CALL PathOfEx.00B70F69
	$+FE     005E7038     83F8 02              CMP EAX,0x2
	$+101    005E703B     72 0B                JB SHORT PathOfEx.005E7048
	$+103    005E703D     C605 00E0F000 01     MOV BYTE PTR DS:[0xF0E000],0x1
	$+10A    005E7044     84DB                 TEST BL,BL
	$+10C    005E7046     75 07                JNZ SHORT PathOfEx.005E704F
	$+10E    005E7048     C605 00E0F000 00     MOV BYTE PTR DS:[0xF0E000],0x0
	$+115    005E704F     5B                   POP EBX
	$+116    005E7050     C3                   RETN
	$+117    005E7051     CC                   INT3
	$+118    005E7052     CC                   INT3
	$+119    005E7053     CC                   INT3
	$+11A    005E7054     CC                   INT3
	$+11B    005E7055     CC                   INT3
	$+11C    005E7056     CC                   INT3
	$+11D    005E7057     CC                   INT3
	$+11E    005E7058     CC                   INT3
	$+11F    005E7059     CC                   INT3
	$+120    005E705A     CC                   INT3
	$+121    005E705B     CC                   INT3
	$+122    005E705C     CC                   INT3
	$+123    005E705D     CC                   INT3
	$+124    005E705E     CC                   INT3
	$+125    005E705F     CC                   INT3
	$+126    005E7060     55                   PUSH EBP
	$+127    005E7061     8BEC                 MOV EBP,ESP
	$+129    005E7063     6A FF                PUSH -0x1
	$+12B    005E7065     68 8007C500          PUSH PathOfEx.00C50780
	$+130    005E706A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+136    005E7070     50                   PUSH EAX
	$+137    005E7071     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+13E    005E7078     83EC 10              SUB ESP,0x10
	$+141    005E707B     53                   PUSH EBX
	$+142    005E707C     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+149    005E7083     8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+14B    005E7085     56                   PUSH ESI
	$+14C    005E7086     57                   PUSH EDI
	$+14D    005E7087     8965 F0              MOV DWORD PTR SS:[EBP-0x10],ESP
	$+150    005E708A     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
	$+153    005E708D     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+156    005E7090     5F                   POP EDI
	$+157    005E7091     5E                   POP ESI
	$+158    005E7092     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+15F    005E7099     5B                   POP EBX
	$+160    005E709A     8BE5                 MOV ESP,EBP
	$+162    005E709C     5D                   POP EBP
	$+163    005E709D     C3                   RETN
	$+164    005E709E     64:A1 2C000000       MOV EAX,DWORD PTR FS:[0x2C]
	$+16A    005E70A4     8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$+16C    005E70A6     8B40 0C              MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+16F    005E70A9     3B05 04E0F000        CMP EAX,DWORD PTR DS:[0xF0E004]
	$+175    005E70AF     75 09                JNZ SHORT PathOfEx.005E70BA
	$+177    005E70B1     6A 00                PUSH 0x0
	$+179    005E70B3     6A 00                PUSH 0x0
	$+17B    005E70B5     E8 E9435B00          CALL PathOfEx.00B9B4A3
	$+180    005E70BA     68 A807F300          PUSH PathOfEx.00F307A8
	$+185    005E70BF     C745 EC A807F300     MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00F307A8
	$+18C    005E70C6     FF15 3C71C900        CALL DWORD PTR DS:[<&KERNEL32.EnterCriticalSection>]   ; ntdll_1i.RtlEnterCriticalSection
	$+192    005E70CC     8D45 E4              LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+195    005E70CF     50                   PUSH EAX
	$+196    005E70D0     E8 1B190000          CALL PathOfEx.005E89F0
	$+19B    005E70D5     83C4 04              ADD ESP,0x4
	$+19E    005E70D8     50                   PUSH EAX
	$+19F    005E70D9     C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$+1A3    005E70DD     E8 7E120000          CALL PathOfEx.005E8360
	$+1A8    005E70E2     8D45 E4              LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+1AB    005E70E5     C645 FC 02           MOV BYTE PTR SS:[EBP-0x4],0x2
	$+1AF    005E70E9     50                   PUSH EAX
	$+1B0    005E70EA     E8 ACB05800          CALL PathOfEx.00B7219B
	$+1B5    005E70EF     83C4 04              ADD ESP,0x4
	$+1B8    005E70F2     68 A807F300          PUSH PathOfEx.00F307A8
	$+1BD    005E70F7     FF15 4071C900        CALL DWORD PTR DS:[<&KERNEL32.LeaveCriticalSection>]   ; ntdll_1i.RtlLeaveCriticalSection
	$+1C3    005E70FD     B8 8D705E00          MOV EAX,PathOfEx.005E708D
	$+1C8    005E7102     C3                   RETN
	$+1C9    005E7103     CC                   INT3
	$+1CA    005E7104     CC                   INT3
	$+1CB    005E7105     CC                   INT3
	$+1CC    005E7106     CC                   INT3
	$+1CD    005E7107     CC                   INT3
	$+1CE    005E7108     CC                   INT3
	$+1CF    005E7109     CC                   INT3
	$+1D0    005E710A     CC                   INT3
	$+1D1    005E710B     CC                   INT3
	$+1D2    005E710C     CC                   INT3
	$+1D3    005E710D     CC                   INT3
	$+1D4    005E710E     CC                   INT3
	$+1D5    005E710F     CC                   INT3
	$+1D6    005E7110     64:A1 2C000000       MOV EAX,DWORD PTR FS:[0x2C]
	$+1DC    005E7116     56                   PUSH ESI
	$+1DD    005E7117     8BF1                 MOV ESI,ECX
	$+1DF    005E7119     57                   PUSH EDI
	$+1E0    005E711A     8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$+1E2    005E711C     BF FCDFF000          MOV EDI,PathOfEx.00F0DFFC
	$+1E7    005E7121     8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+1EA    005E7124     8B42 18              MOV EAX,DWORD PTR DS:[EDX+0x18]
	$+1ED    005E7127     8942 08              MOV DWORD PTR DS:[EDX+0x8],EAX
	$+1F0    005E712A     BA 01000000          MOV EDX,0x1
	$+1F5    005E712F     D3E2                 SHL EDX,CL
	$+1F7    005E7131     83C9 FF              OR ECX,0xFFFFFFFF
	$+1FA    005E7134     8BC1                 MOV EAX,ECX
	$+1FC    005E7136     F0:0FC107            LOCK XADD DWORD PTR DS:[EDI],EAX
	$+200    005E713A     F6C2 0F              TEST DL,0xF
	$+203    005E713D     74 0B                JE SHORT PathOfEx.005E714A
	$+205    005E713F     8BC1                 MOV EAX,ECX
	$+207    005E7141     BF F8DFF000          MOV EDI,PathOfEx.00F0DFF8
	$+20C    005E7146     F0:0FC107            LOCK XADD DWORD PTR DS:[EDI],EAX
	$+210    005E714A     F6C2 04              TEST DL,0x4
	$+213    005E714D     74 09                JE SHORT PathOfEx.005E7158
	$+215    005E714F     B8 F4DFF000          MOV EAX,PathOfEx.00F0DFF4
	$+21A    005E7154     F0:0FC108            LOCK XADD DWORD PTR DS:[EAX],ECX
	$+21E    005E7158     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+220    005E715A     8BCE                 MOV ECX,ESI
	$+222    005E715C     6A 01                PUSH 0x1
	$+224    005E715E     FF10                 CALL DWORD PTR DS:[EAX]
	$+226    005E7160     5F                   POP EDI
	$+227    005E7161     5E                   POP ESI
	$+228    005E7162     C3                   RETN
	$+229    005E7163     CC                   INT3
	$+22A    005E7164     CC                   INT3
	$+22B    005E7165     CC                   INT3
	$+22C    005E7166     CC                   INT3
	$+22D    005E7167     CC                   INT3
	$+22E    005E7168     CC                   INT3
	$+22F    005E7169     CC                   INT3
	$+230    005E716A     CC                   INT3
	$+231    005E716B     CC                   INT3
	$+232    005E716C     CC                   INT3
	$+233    005E716D     CC                   INT3
	$+234    005E716E     CC                   INT3
	$+235    005E716F     CC                   INT3
	$+236    005E7170     55                   PUSH EBP
	$+237    005E7171     8BEC                 MOV EBP,ESP
	$+239    005E7173     6A FF                PUSH -0x1
	$+23B    005E7175     68 9007C500          PUSH PathOfEx.00C50790
	$+240    005E717A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+246    005E7180     50                   PUSH EAX
	$+247    005E7181     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+24E    005E7188     83EC 08              SUB ESP,0x8
	$+251    005E718B     53                   PUSH EBX
	$+252    005E718C     56                   PUSH ESI
	$+253    005E718D     8BF1                 MOV ESI,ECX
	$+255    005E718F     8975 EC              MOV DWORD PTR SS:[EBP-0x14],ESI
	$+258    005E7192     57                   PUSH EDI
	$+259    005E7193     8965 F0              MOV DWORD PTR SS:[EBP-0x10],ESP
	$+25C    005E7196     85F6                 TEST ESI,ESI
	$+25E    005E7198     75 13                JNZ SHORT PathOfEx.005E71AD
	$+260    005E719A     32C0                 XOR AL,AL
	$+262    005E719C     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+265    005E719F     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+26C    005E71A6     5F                   POP EDI
	$+26D    005E71A7     5E                   POP ESI
	$+26E    005E71A8     5B                   POP EBX
	$+26F    005E71A9     8BE5                 MOV ESP,EBP
	$+271    005E71AB     5D                   POP EBP
	$+272    005E71AC     C3                   RETN
	$+273    005E71AD     64:A1 2C000000       MOV EAX,DWORD PTR FS:[0x2C]
	$+279    005E71B3     8B56 04              MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+27C    005E71B6     8B08                 MOV ECX,DWORD PTR DS:[EAX]
	$+27E    005E71B8     8B41 08              MOV EAX,DWORD PTR DS:[ECX+0x8]
	$+281    005E71BB     8941 18              MOV DWORD PTR DS:[ECX+0x18],EAX
	$+284    005E71BE     8951 08              MOV DWORD PTR DS:[ECX+0x8],EDX
	$+287    005E71C1     8BCE                 MOV ECX,ESI
	$+289    005E71C3     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+290    005E71CA     E8 91FEFFFF          CALL PathOfEx.005E7060
	$+295    005E71CF     8BCE                 MOV ECX,ESI
	$+297    005E71D1     E8 3AFFFFFF          CALL PathOfEx.005E7110
	$+29C    005E71D6     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+29F    005E71D9     B0 01                MOV AL,0x1
	$+2A1    005E71DB     5F                   POP EDI
	$+2A2    005E71DC     5E                   POP ESI
	$+2A3    005E71DD     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+2AA    005E71E4     5B                   POP EBX
	$+2AB    005E71E5     8BE5                 MOV ESP,EBP
	$+2AD    005E71E7     5D                   POP EBP
	$+2AE    005E71E8     C3                   RETN
	$+2AF    005E71E9     8B4D EC              MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+2B2    005E71EC     E8 1FFFFFFF          CALL PathOfEx.005E7110
	$+2B7    005E71F1     6A 00                PUSH 0x0
	$+2B9    005E71F3     6A 00                PUSH 0x0
	$+2BB    005E71F5     E8 A9425B00          CALL PathOfEx.00B9B4A3
	$+2C0    005E71FA     CC                   INT3
	$+2C1    005E71FB     CC                   INT3
	$+2C2    005E71FC     CC                   INT3
	$+2C3    005E71FD     CC                   INT3
	$+2C4    005E71FE     CC                   INT3
	$+2C5    005E71FF     CC                   INT3
	$+2C6    005E7200     55                   PUSH EBP
	$+2C7    005E7201     8BEC                 MOV EBP,ESP
	$+2C9    005E7203     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+2CF    005E7209     6A FF                PUSH -0x1
	$+2D1    005E720B     68 48B3C200          PUSH PathOfEx.00C2B348
	$+2D6    005E7210     50                   PUSH EAX
	$+2D7    005E7211     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+2DE    005E7218     83EC 08              SUB ESP,0x8
	$+2E1    005E721B     53                   PUSH EBX
	$+2E2    005E721C     56                   PUSH ESI
	$+2E3    005E721D     57                   PUSH EDI
	$+2E4    005E721E     8BF9                 MOV EDI,ECX
	$+2E6    005E7220     BE 01000000          MOV ESI,0x1
	$+2EB    005E7225     8BD6                 MOV EDX,ESI
	$+2ED    005E7227     8BC6                 MOV EAX,ESI
	$+2EF    005E7229     BB FCDFF000          MOV EBX,PathOfEx.00F0DFFC
	$+2F4    005E722E     8B4F 04              MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+2F7    005E7231     D3E2                 SHL EDX,CL
	$+2F9    005E7233     F0:0FC103            LOCK XADD DWORD PTR DS:[EBX],EAX
	$+2FD    005E7237     F6C2 0F              TEST DL,0xF
	$+300    005E723A     74 0B                JE SHORT PathOfEx.005E7247
	$+302    005E723C     8BC6                 MOV EAX,ESI
	$+304    005E723E     BB F8DFF000          MOV EBX,PathOfEx.00F0DFF8
	$+309    005E7243     F0:0FC103            LOCK XADD DWORD PTR DS:[EBX],EAX
	$+30D    005E7247     F6C2 04              TEST DL,0x4
	$+310    005E724A     74 09                JE SHORT PathOfEx.005E7255
	$+312    005E724C     B8 F4DFF000          MOV EAX,PathOfEx.00F0DFF4
	$+317    005E7251     F0:0FC130            LOCK XADD DWORD PTR DS:[EAX],ESI
	$+31B    005E7255     803D EFDFF000 00     CMP BYTE PTR DS:[0xF0DFEF],0x0
	$+322    005E725C     75 1A                JNZ SHORT PathOfEx.005E7278
	$+324    005E725E     8BCF                 MOV ECX,EDI
	$+326    005E7260     E8 0BFFFFFF          CALL PathOfEx.005E7170
	$+32B    005E7265     32C0                 XOR AL,AL
	$+32D    005E7267     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+330    005E726A     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+337    005E7271     5F                   POP EDI
	$+338    005E7272     5E                   POP ESI
	$+339    005E7273     5B                   POP EBX
	$+33A    005E7274     8BE5                 MOV ESP,EBP
	$+33C    005E7276     5D                   POP EBP
	$+33D    005E7277     C3                   RETN
	$+33E    005E7278     33F6                 XOR ESI,ESI
	$+340    005E727A     F6C2 03              TEST DL,0x3
	$+343    005E727D     74 09                JE SHORT PathOfEx.005E7288
	$+345    005E727F     A1 9007F300          MOV EAX,DWORD PTR DS:[0xF30790]
	$+34A    005E7284     8B30                 MOV ESI,DWORD PTR DS:[EAX]
	$+34C    005E7286     EB 1C                JMP SHORT PathOfEx.005E72A4
	$+34E    005E7288     F6C2 0C              TEST DL,0xC
	$+351    005E728B     74 0A                JE SHORT PathOfEx.005E7297
	$+353    005E728D     A1 9007F300          MOV EAX,DWORD PTR DS:[0xF30790]
	$+358    005E7292     8B70 04              MOV ESI,DWORD PTR DS:[EAX+0x4]
	$+35B    005E7295     EB 0D                JMP SHORT PathOfEx.005E72A4
	$+35D    005E7297     F6C2 30              TEST DL,0x30
	$+360    005E729A     74 08                JE SHORT PathOfEx.005E72A4
	$+362    005E729C     A1 9007F300          MOV EAX,DWORD PTR DS:[0xF30790]
	$+367    005E72A1     8B70 08              MOV ESI,DWORD PTR DS:[EAX+0x8]
	$+36A    005E72A4     85C9                 TEST ECX,ECX
	$+36C    005E72A6     74 14                JE SHORT PathOfEx.005E72BC
	$+36E    005E72A8     83F9 02              CMP ECX,0x2
	$+371    005E72AB     74 0F                JE SHORT PathOfEx.005E72BC
	$+373    005E72AD     83F9 04              CMP ECX,0x4
	$+376    005E72B0     74 0A                JE SHORT PathOfEx.005E72BC
	$+378    005E72B2     8D5E 48              LEA EBX,DWORD PTR DS:[ESI+0x48]
	$+37B    005E72B5     B8 30000000          MOV EAX,0x30
	$+380    005E72BA     EB 08                JMP SHORT PathOfEx.005E72C4
	$+382    005E72BC     8D5E 1C              LEA EBX,DWORD PTR DS:[ESI+0x1C]
	$+385    005E72BF     B8 04000000          MOV EAX,0x4
	$+38A    005E72C4     897D F0              MOV DWORD PTR SS:[EBP-0x10],EDI
	$+38D    005E72C7     8D3C30               LEA EDI,DWORD PTR DS:[EAX+ESI]
	$+390    005E72CA     57                   PUSH EDI
	$+391    005E72CB     897D EC              MOV DWORD PTR SS:[EBP-0x14],EDI
	$+394    005E72CE     FF15 3C71C900        CALL DWORD PTR DS:[<&KERNEL32.EnterCriticalSection>]   ; ntdll_1i.RtlEnterCriticalSection
	$+39A    005E72D4     8D45 F0              LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+39D    005E72D7     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+3A4    005E72DE     50                   PUSH EAX
	$+3A5    005E72DF     8BCB                 MOV ECX,EBX
	$+3A7    005E72E1     E8 6A86EDFF          CALL PathOfEx.004BF950
	$+3AC    005E72E6     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+3B3    005E72ED     85FF                 TEST EDI,EDI
	$+3B5    005E72EF     74 07                JE SHORT PathOfEx.005E72F8
	$+3B7    005E72F1     57                   PUSH EDI
	$+3B8    005E72F2     FF15 4071C900        CALL DWORD PTR DS:[<&KERNEL32.LeaveCriticalSection>]   ; ntdll_1i.RtlLeaveCriticalSection
	$+3BE    005E72F8     8BCE                 MOV ECX,ESI
	$+3C0    005E72FA     E8 F1FAFFFF          CALL PathOfEx.005E6DF0
	$+3C5    005E72FF     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3C8    005E7302     B0 01                MOV AL,0x1
	$+3CA    005E7304     5F                   POP EDI
	$+3CB    005E7305     5E                   POP ESI
	$+3CC    005E7306     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+3D3    005E730D     5B                   POP EBX
	$+3D4    005E730E     8BE5                 MOV ESP,EBP
	$+3D6    005E7310     5D                   POP EBP
	$+3D7    005E7311     C3                   RETN
	$+3D8    005E7312     CC                   INT3
	*/
	addr_tmp = 0;
/*
#ifndef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("c7????????????8b????8d????c7????????????8b????e8????????ff??????????64", -0x1a);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_a_fn_switch_pre_login_scene_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
#endif*/
	return true;
}
