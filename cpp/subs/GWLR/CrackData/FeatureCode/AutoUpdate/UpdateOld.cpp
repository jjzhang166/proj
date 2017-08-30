#include "stdafx.h"
#include "FeatureCodes.h"
#include "GloFns.h"
#include <CmnMix/Cmn_LogOut.h>
#include <ProtocolX/P_DefinesX.h>

void ScanCodeHelper::ScanCode_Updates_Old(Cmn_FC_OFstream& ofs)
{
	LOG_O(Log_trace) << "ScanCode_Updates_Old开始更新";
	ofs << "#pragma once" << FC_OFstream_EndlLog;
	//////////////////////////////////////////////////////////////////////////

	/*++
	$-9      149A12A0      55                  PUSH EBP
	$-8      149A12A1      8BEC                MOV EBP,ESP
	$-6      149A12A3      83EC 0C             SUB ESP,0xC
	$-3      149A12A6      894D F4             MOV DWORD PTR SS:[EBP-0xC],ECX
	$ ==>    149A12A9      8B49 24             MOV ECX,DWORD PTR DS:[ECX+0x24]
	$+3      149A12AC      85C9                TEST ECX,ECX
	$+5      149A12AE      0F84 24010000       JE CryRende.149A13D8
	$+B      149A12B4      8B81 F4020000       MOV EAX,DWORD PTR DS:[ECX+0x2F4]
	$+11     149A12BA      85C0                TEST EAX,EAX
	$+13     149A12BC      74 05               JE SHORT CryRende.149A12C3
	$+15     149A12BE      8945 FC             MOV DWORD PTR SS:[EBP-0x4],EAX
	$+18     149A12C1      EB 05               JMP SHORT CryRende.149A12C8
	$+1A     149A12C3      894D FC             MOV DWORD PTR SS:[EBP-0x4],ECX
	$+1D     149A12C6      8BC1                MOV EAX,ECX
	$+1F     149A12C8      8378 0C 00          CMP DWORD PTR DS:[EAX+0xC],0x0
	$+23     149A12CC      0F8C 06010000       JL CryRende.149A13D8
	$+29     149A12D2      83B9 40010000 00    CMP DWORD PTR DS:[ECX+0x140],0x0
	$+30     149A12D9      0F8C F9000000       JL CryRende.149A13D8
	$+36     149A12DF      8B0D A095CF14       MOV ECX,DWORD PTR DS:[0x14CF95A0]
	$+3C     149A12E5      8B90 C4020000       MOV EDX,DWORD PTR DS:[EAX+0x2C4]
	$+42     149A12EB      53                  PUSH EBX
	$+43     149A12EC      56                  PUSH ESI
	$+44     149A12ED      57                  PUSH EDI
	$+45     149A12EE      51                  PUSH ECX
	$+46     149A12EF      8B0C95 68EBCD14     MOV ECX,DWORD PTR DS:[EDX*4+0x14CDEB68]
	$+4D     149A12F6      8B50 08             MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+50     149A12F9      8B40 04             MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+53     149A12FC      51                  PUSH ECX
	$+54     149A12FD      52                  PUSH EDX
	$+55     149A12FE      50                  PUSH EAX
	$+56     149A12FF      6A 00               PUSH 0x0
	$+58     149A1301      B9 F078CF14         MOV ECX,CryRende.14CF78F0
	$+5D     149A1306      E8 3214F3FF         CALL CryRende.148D273D

	8B????85??0F??????????8B??????????85??74
	--*/
	auto dwMMOClientBlackScreenRenderHook = scan_CryRenderD3D9_.ScanAddr("8B????85??0F??????????8B??????????85??74", -9);
	assert(dwMMOClientBlackScreenRenderHook);

	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_BLACK_SCREEN_RENDER_HOOK  "
		<< scan_CryRenderD3D9_.OffsetToModuleBegin(dwMMOClientBlackScreenRenderHook) << "  // " << dwMMOClientBlackScreenRenderHook << "  CryRenderD3D11.dll" << FC_OFstream_EndlLog;

	/*++
	发包CALL

	568BF183BE????????0074??8B8E????????8B018B50??FFD284C074??32C0

	// 先遣测试版本
	$-6      >  55                  PUSH EBP
	$-5      >  8BEC                MOV EBP,ESP
	$-3      >  56                  PUSH ESI
	$-2      >  8BF1                MOV ESI,ECX
	$ ==>    >  83BE 30010000 00    CMP DWORD PTR DS:[ESI+0x130],0x0
	$+7      >  74 18               JE SHORT CryGame.0D02E277
	$+9      >  8B8E 30010000       MOV ECX,DWORD PTR DS:[ESI+0x130]
	$+F      >  8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+11     >  8B50 20             MOV EDX,DWORD PTR DS:[EAX+0x20]
	$+14     >  FFD2                CALL EDX
	$+16     >  84C0                TEST AL,AL
	$+18     >  74 07               JE SHORT CryGame.0D02E277
	$+1A     >  32C0                XOR AL,AL
	$+1C     >  5E                  POP ESI
	$+1D     >  5D                  POP EBP
	$+1E     >  C2 0400             RETN 0x4
	$+21     >  837E 1C 00          CMP DWORD PTR DS:[ESI+0x1C],0x0
	$+25     >^ 74 F3               JE SHORT CryGame.0D02E270
	$+27     >  8B4E 1C             MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$+2A     >  8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+2C     >  8B55 08             MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+2F     >  8B40 10             MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+32     >  52                  PUSH EDX
	$+33     >  FFD0                CALL EAX
	$+35     >  85C0                TEST EAX,EAX
	$+37     >  0F94C0              SETE AL
	$+3A     >  5E                  POP ESI
	$+3B     >  5D                  POP EBP
	$+3C     >  C2 0400             RETN 0x4

	83BE????????0074??8B8E????????8B018B50??FFD284C074??32C0
	--*/
	auto	dwMMOGameSendCall = scan_game_.ScanAddr("83BE????????0074??8B8E????????8B018B50??FFD284C074??32C0", -0x06);
	assert(dwMMOGameSendCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SEND_CALL  " << scan_game_.OffsetToModuleBegin(dwMMOGameSendCall) << "  // " << dwMMOGameSendCall << " CryGame.dll" << FC_OFstream_EndlLog;

	auto dwMMOGameSocektOffst3 = *scan_game_.ScanImme<unsigned char>("83BE????????0074??8B8E????????8B018B50??FFD284C074??32C0", 0x23);
	assert(dwMMOGameSocektOffst3);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SOCKET_OFFSET3  " << dwMMOGameSocektOffst3 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	发包CALL基址
	$-9      >  A1 485B2802          MOV EAX,DWORD PTR DS:[0x2285B48]	// 基址
	$-4      >  8B7424 18            MOV ESI,DWORD PTR SS:[ESP+0x18]
	$ ==>    >  85C0                 TEST EAX,EAX
	$+2      >  74 26                JE SHORT MHOClien.0051BA10
	$+4      >  83B8 74010000 00     CMP DWORD PTR DS:[EAX+0x174],0x0	// 偏移
	$+B      >  74 1D                JE SHORT MHOClien.0051BA10
	$+D      >  E8 18DBE700          CALL MHOClien.01399510                   ; 取基址
	{
	$ ==>    >  A1 485B2802          MOV EAX,DWORD PTR DS:[0x2285B48]	// 基址
	$+5      >  85C0                 TEST EAX,EAX
	$+7      >  74 07                JE SHORT MHOClien.01399520
	$+9      >  8B80 74010000        MOV EAX,DWORD PTR DS:[EAX+0x174]	// 偏移
	$+F      >  C3                   RETN
	$+10     >  33C0                 XOR EAX,EAX
	$+12     >  C3                   RETN
	}
	$+12     >  8B48 04              MOV ECX,DWORD PTR DS:[EAX+0x4]           ; 取socket addr
	$+15     >  8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+17     >  8B42 28              MOV EAX,DWORD PTR DS:[EDX+0x28]
	$+1A     >  FFD0                 CALL EAX
	$+1C     >  85C0                 TEST EAX,EAX
	$+1E     >  74 0A                JE SHORT MHOClien.0051BA10
	$+20     >  8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$+22     >  8BC8                 MOV ECX,EAX
	$+24     >  8B42 24              MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+27     >  56                   PUSH ESI
	$+28     >  FFD0                 CALL EAX

	85??74??83????????????74??E8????????8B????8B??8B????FF

	// 先遣测试版本
	$-9      0C8BDB12      A1 7CCAAC0D         MOV EAX,DWORD PTR DS:[0xDACCA7C]                      ; 发包CALL基址
	$-4      0C8BDB17      8B7424 18           MOV ESI,DWORD PTR SS:[ESP+0x18]
	$ ==>    0C8BDB1B      85C0                TEST EAX,EAX
	$+2      0C8BDB1D      74 26               JE SHORT CryGame.0C8BDB45
	$+4      0C8BDB1F      83B8 74010000 00    CMP DWORD PTR DS:[EAX+0x174],0x0
	$+B      0C8BDB26      74 1D               JE SHORT CryGame.0C8BDB45
	$+D      0C8BDB28      E8 83C63E00         CALL CryGame.0CCAA1B0
	$+12     0C8BDB2D      8B48 04             MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+15     0C8BDB30      8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+17     0C8BDB32      8B42 28             MOV EAX,DWORD PTR DS:[EDX+0x28]
	$+1A     0C8BDB35      FFD0                CALL EAX
	$+1C     0C8BDB37      85C0                TEST EAX,EAX
	$+1E     0C8BDB39      74 0A               JE SHORT CryGame.0C8BDB45
	$+20     0C8BDB3B      8B10                MOV EDX,DWORD PTR DS:[EAX]
	$+22     0C8BDB3D      8BC8                MOV ECX,EAX
	$+24     0C8BDB3F      8B42 24             MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+27     0C8BDB42      56                  PUSH ESI
	$+28     0C8BDB43      FFD0                CALL EAX

	//zhp 2016/5/16 16:27
	$-AB     0B2BE0E0      55                    PUSH EBP
	$-AA     0B2BE0E1      8BEC                  MOV EBP, ESP
	$-A8     0B2BE0E3      83E4 F8               AND ESP, FFFFFFF8
	$-A5     0B2BE0E6      83EC 20               SUB ESP, 20
	$-A2     0B2BE0E9      56                    PUSH ESI
	$-A1     0B2BE0EA      57                    PUSH EDI
	$-A0     0B2BE0EB      8BF9                  MOV EDI, ECX                             ; ntdll.779301E4
	$-9E     0B2BE0ED      897C24 0C             MOV DWORD PTR SS:[ESP+C], EDI
	$-9A     0B2BE0F1      C74424 08 00000000    MOV DWORD PTR SS:[ESP+8], 0
	$-92     0B2BE0F9      E8 E28B4300           CALL CryGame.0B6F6CE0
	$-8D     0B2BE0FE      85C0                  TEST EAX, EAX
	$-8B     0B2BE100      74 2F                 JE SHORT CryGame.0B2BE131
	$-89     0B2BE102      E8 D98B4300           CALL CryGame.0B6F6CE0
	$-84     0B2BE107      8BC8                  MOV ECX, EAX
	$-82     0B2BE109      E8 D28E4300           CALL CryGame.0B6F6FE0
	$-7D     0B2BE10E      85C0                  TEST EAX, EAX
	$-7B     0B2BE110      74 1F                 JE SHORT CryGame.0B2BE131
	$-79     0B2BE112      E8 C98B4300           CALL CryGame.0B6F6CE0
	$-74     0B2BE117      8BC8                  MOV ECX, EAX
	$-72     0B2BE119      E8 C28E4300           CALL CryGame.0B6F6FE0
	$-6D     0B2BE11E      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$-6B     0B2BE120      8B52 04               MOV EDX, DWORD PTR DS:[EDX+4]
	$-68     0B2BE123      8D4C24 08             LEA ECX, DWORD PTR SS:[ESP+8]
	$-64     0B2BE127      51                    PUSH ECX                                 ; ntdll.779301E4
	$-63     0B2BE128      68 B8000000           PUSH 0B8
	$-5E     0B2BE12D      8BC8                  MOV ECX, EAX
	$-5C     0B2BE12F      FFD2                  CALL EDX
	$-5A     0B2BE131      8B45 14               MOV EAX, DWORD PTR SS:[EBP+14]           ; mhoclien.0040E245
	$-57     0B2BE134      8B48 04               MOV ECX, DWORD PTR DS:[EAX+4]
	$-54     0B2BE137      8B50 0C               MOV EDX, DWORD PTR DS:[EAX+C]
	$-51     0B2BE13A      8B70 08               MOV ESI, DWORD PTR DS:[EAX+8]
	$-4E     0B2BE13D      894C24 14             MOV DWORD PTR SS:[ESP+14], ECX           ; ntdll.779301E4
	$-4A     0B2BE141      895424 1C             MOV DWORD PTR SS:[ESP+1C], EDX
	$-46     0B2BE145      8B50 10               MOV EDX, DWORD PTR DS:[EAX+10]
	$-43     0B2BE148      C1E9 06               SHR ECX, 6
	$-40     0B2BE14B      C74424 10 00000000    MOV DWORD PTR SS:[ESP+10], 0
	$-38     0B2BE153      897424 18             MOV DWORD PTR SS:[ESP+18], ESI
	$-34     0B2BE157      895424 20             MOV DWORD PTR SS:[ESP+20], EDX
	$-30     0B2BE15B      F6C1 01               TEST CL, 1
	$-2D     0B2BE15E      74 15                 JE SHORT CryGame.0B2BE175
	$-2B     0B2BE160      8B00                  MOV EAX, DWORD PTR DS:[EAX]
	$-29     0B2BE162      894424 10             MOV DWORD PTR SS:[ESP+10], EAX
	$-25     0B2BE166      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$-23     0B2BE168      8B52 04               MOV EDX, DWORD PTR DS:[EDX+4]
	$-20     0B2BE16B      56                    PUSH ESI
	$-1F     0B2BE16C      8D4C24 14             LEA ECX, DWORD PTR SS:[ESP+14]
	$-1B     0B2BE170      51                    PUSH ECX                                 ; ntdll.779301E4
	$-1A     0B2BE171      8BC8                  MOV ECX, EAX
	$-18     0B2BE173      FFD2                  CALL EDX
	$-16     0B2BE175      8B4424 14             MOV EAX, DWORD PTR SS:[ESP+14]
	$-12     0B2BE179      25 8F000000           AND EAX, 8F
	$-D      0B2BE17E      3C 03                 CMP AL, 3
	$-B      0B2BE180      75 36                 JNZ SHORT CryGame.0B2BE1B8
	$-9      0B2BE182      A1 94B6780C           MOV EAX, DWORD PTR DS:[C78B694]
	$-4      0B2BE187      8B7424 18             MOV ESI, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$ ==>    0B2BE18B      85C0                  TEST EAX, EAX
	$+2      0B2BE18D      74 26                 JE SHORT CryGame.0B2BE1B5
	$+4      0B2BE18F      83B8 7C010000 00      CMP DWORD PTR DS:[EAX+17C], 0
	$+B      0B2BE196      74 1D                 JE SHORT CryGame.0B2BE1B5
	$+D      0B2BE198      E8 438B4300           CALL CryGame.0B6F6CE0
	$+12     0B2BE19D      8B48 04               MOV ECX, DWORD PTR DS:[EAX+4]
	$+15     0B2BE1A0      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+17     0B2BE1A2      8B42 28               MOV EAX, DWORD PTR DS:[EDX+28]
	$+1A     0B2BE1A5      FFD0                  CALL EAX
	$+1C     0B2BE1A7      85C0                  TEST EAX, EAX
	$+1E     0B2BE1A9      74 0A                 JE SHORT CryGame.0B2BE1B5
	$+20     0B2BE1AB      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+22     0B2BE1AD      8BC8                  MOV ECX, EAX
	$+24     0B2BE1AF      8B42 24               MOV EAX, DWORD PTR DS:[EDX+24]
	$+27     0B2BE1B2      56                    PUSH ESI
	$+28     0B2BE1B3      FFD0                  CALL EAX
	$+2A     0B2BE1B5      8977 20               MOV DWORD PTR DS:[EDI+20], ESI
	$+2D     0B2BE1B8      A1 8879780C           MOV EAX, DWORD PTR DS:[C787988]
	$+32     0B2BE1BD      85C0                  TEST EAX, EAX
	$+34     0B2BE1BF      0F84 A9000000         JE CryGame.0B2BE26E
	$+3A     0B2BE1C5      8378 2C 00            CMP DWORD PTR DS:[EAX+2C], 0
	$+3E     0B2BE1C9      74 5C                 JE SHORT CryGame.0B2BE227
	$+40     0B2BE1CB      8378 28 00            CMP DWORD PTR DS:[EAX+28], 0
	$+44     0B2BE1CF      74 56                 JE SHORT CryGame.0B2BE227
	$+46     0B2BE1D1      8B40 2C               MOV EAX, DWORD PTR DS:[EAX+2C]
	$+49     0B2BE1D4      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+4B     0B2BE1D6      8BC8                  MOV ECX, EAX
	$+4D     0B2BE1D8      8B42 54               MOV EAX, DWORD PTR DS:[EDX+54]
	$+50     0B2BE1DB      68 0C445B0C           PUSH CryGame.0C5B440C                    ; ASCII "sys_last_config_time"
	$+55     0B2BE1E0      FFD0                  CALL EAX
	$+57     0B2BE1E2      85C0                  TEST EAX, EAX
	$+59     0B2BE1E4      74 3C                 JE SHORT CryGame.0B2BE222
	$+5B     0B2BE1E6      8B0D 8879780C         MOV ECX, DWORD PTR DS:[C787988]          ; mhoclien.01820E10
	$+61     0B2BE1EC      8B49 2C               MOV ECX, DWORD PTR DS:[ECX+2C]
	$+64     0B2BE1EF      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+66     0B2BE1F1      8B42 54               MOV EAX, DWORD PTR DS:[EDX+54]
	$+69     0B2BE1F4      68 0C445B0C           PUSH CryGame.0C5B440C                    ; ASCII "sys_last_config_time"
	$+6E     0B2BE1F9      FFD0                  CALL EAX
	$+70     0B2BE1FB      8B0D 8879780C         MOV ECX, DWORD PTR DS:[C787988]          ; mhoclien.01820E10
	$+76     0B2BE201      8B49 28               MOV ECX, DWORD PTR DS:[ECX+28]
	$+79     0B2BE204      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+7B     0B2BE206      8BF8                  MOV EDI, EAX
	$+7D     0B2BE208      8B37                  MOV ESI, DWORD PTR DS:[EDI]
	$+7F     0B2BE20A      8B42 0C               MOV EAX, DWORD PTR DS:[EDX+C]
	$+82     0B2BE20D      6A 00                 PUSH 0
	$+84     0B2BE20F      83C6 18               ADD ESI, 18
	$+87     0B2BE212      FFD0                  CALL EAX
	$+89     0B2BE214      8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$+8B     0B2BE216      51                    PUSH ECX                                 ; ntdll.779301E4
	$+8C     0B2BE217      8BCF                  MOV ECX, EDI
	$+8E     0B2BE219      D91C24                FSTP DWORD PTR SS:[ESP]
	$+91     0B2BE21C      FFD2                  CALL EDX
	$+93     0B2BE21E      8B7C24 0C             MOV EDI, DWORD PTR SS:[ESP+C]            ; KERNELBA.771C3BF4
	$+97     0B2BE222      A1 8879780C           MOV EAX, DWORD PTR DS:[C787988]
	$+9C     0B2BE227      85C0                  TEST EAX, EAX
	$+9E     0B2BE229      74 43                 JE SHORT CryGame.0B2BE26E
	$+A0     0B2BE22B      8378 2C 00            CMP DWORD PTR DS:[EAX+2C], 0
	$+A4     0B2BE22F      74 3D                 JE SHORT CryGame.0B2BE26E
	$+A6     0B2BE231      8B40 2C               MOV EAX, DWORD PTR DS:[EAX+2C]
	$+A9     0B2BE234      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+AB     0B2BE236      8BC8                  MOV ECX, EAX
	$+AD     0B2BE238      8B42 54               MOV EAX, DWORD PTR DS:[EDX+54]
	$+B0     0B2BE23B      68 14585B0C           PUSH CryGame.0C5B5814                    ; ASCII "sys_expected_fps"
	$+B5     0B2BE240      FFD0                  CALL EAX
	$+B7     0B2BE242      85C0                  TEST EAX, EAX
	$+B9     0B2BE244      74 28                 JE SHORT CryGame.0B2BE26E
	$+BB     0B2BE246      8B0D 8879780C         MOV ECX, DWORD PTR DS:[C787988]          ; mhoclien.01820E10
	$+C1     0B2BE24C      8B49 2C               MOV ECX, DWORD PTR DS:[ECX+2C]
	$+C4     0B2BE24F      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+C6     0B2BE251      8B42 54               MOV EAX, DWORD PTR DS:[EDX+54]
	$+C9     0B2BE254      68 14585B0C           PUSH CryGame.0C5B5814                    ; ASCII "sys_expected_fps"
	$+CE     0B2BE259      FFD0                  CALL EAX
	$+D0     0B2BE25B      D905 C8935C0C         FLD DWORD PTR DS:[C5C93C8]
	$+D6     0B2BE261      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+D8     0B2BE263      51                    PUSH ECX                                 ; ntdll.779301E4
	$+D9     0B2BE264      8BC8                  MOV ECX, EAX
	$+DB     0B2BE266      D91C24                FSTP DWORD PTR SS:[ESP]
	$+DE     0B2BE269      8B42 18               MOV EAX, DWORD PTR DS:[EDX+18]
	$+E1     0B2BE26C      FFD0                  CALL EAX
	$+E3     0B2BE26E      8B4C24 14             MOV ECX, DWORD PTR SS:[ESP+14]
	$+E7     0B2BE272      C1E9 06               SHR ECX, 6
	$+EA     0B2BE275      C747 20 00000000      MOV DWORD PTR DS:[EDI+20], 0
	$+F1     0B2BE27C      F6C1 01               TEST CL, 1
	$+F4     0B2BE27F      74 15                 JE SHORT CryGame.0B2BE296
	$+F6     0B2BE281      8B4424 18             MOV EAX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+FA     0B2BE285      8B4C24 10             MOV ECX, DWORD PTR SS:[ESP+10]
	$+FE     0B2BE289      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+100    0B2BE28B      8B52 08               MOV EDX, DWORD PTR DS:[EDX+8]
	$+103    0B2BE28E      50                    PUSH EAX
	$+104    0B2BE28F      8D4424 14             LEA EAX, DWORD PTR SS:[ESP+14]
	$+108    0B2BE293      50                    PUSH EAX
	$+109    0B2BE294      FFD2                  CALL EDX
	$+10B    0B2BE296      5F                    POP EDI                                  ; ntdll.7793FD91
	$+10C    0B2BE297      5E                    POP ESI                                  ; ntdll.7793FD91
	$+10D    0B2BE298      8BE5                  MOV ESP, EBP
	$+10F    0B2BE29A      5D                    POP EBP                                  ; ntdll.7793FD91
	$+110    0B2BE29B      C2 1800               RETN 18
	$+113    0B2BE29E      CC                    INT3
	$+114    0B2BE29F      CC                    INT3
	$+115    0B2BE2A0      C2 1800               RETN 18
	--*/
	//feature code:85??74??83????????????74??E8????????8B????8B??8B????FF
	DWORD	dwMMOClientSocketBaseAddr = *scan_game_.ScanImme<DWORD>("85??74??83????????????74??E8????????8B????8B??8B????FF", -0x8);
	assert(dwMMOClientSocketBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SOCKET_BASE_ADDR  " << dwMMOClientSocketBaseAddr - GetModuleCryGameBase() << "  // " << dwMMOClientSocketBaseAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientSocketOffset1 = *scan_game_.ScanImme<DWORD>("85??74??83????????????74??E8????????8B????8B??8B????FF", 6);
	assert(dwMMOClientSocketOffset1);
	assert(dwMMOClientSocketOffset1 == 0x17c);
	ofs << "#define UPDATES_MMOLCINET_REVERSAL_SOCKET_OFFSET1  " << dwMMOClientSocketOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientSocketOffset2_New = *scan_game_.ScanImme<byte>("85??74??83????????????74??E8????????8B????8B??8B????FF", 0x14);
	assert(dwMMOClientSocketOffset2_New);
	assert(dwMMOClientSocketOffset2_New == 0x4);
	ofs << "#define UPDATES_MMOLCINET_REVERSAL_SOCKET_OFFSET2  " << dwMMOClientSocketOffset2_New << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	游戏基址
	$ ==>    >  833D 0C495802 00      cmp dword ptr ds:[0x258490C],0x0
	$+7      >  0F85 85000000         jnz MHOClien.00404262
	$+D      >  56                    push esi
	$+E      >  68 30414000           push MHOClien.00404130
	$+13     >  E8 44F68E01           call MHOClien.01CF382C                   ; jmp 到 msvcr90._set_invalid_parameter_handler
	$+18     >  8B7424 0C             mov esi,dword ptr ss:[esp+0xC]
	$+1C     >  83C4 04               add esp,0x4
	$+1F     >  85F6                  test esi,esi
	$+21     >  74 6E                 je short MHOClien.00404261
	$+23     >  8B06                  mov eax,dword ptr ds:[esi]
	$+25     >  8B50 08               mov edx,dword ptr ds:[eax+0x8]
	$+28     >  57                    push edi
	$+29     >  8BCE                  mov ecx,esi
	$+2B     >  FFD2                  call edx
	$+2D     >  A3 0C495802           mov dword ptr ds:[0x258490C],eax
	$+32     >  8B06                  mov eax,dword ptr ds:[esi]
	$+34     >  8B90 80020000         mov edx,dword ptr ds:[eax+0x280]
	$+3A     >  8BCE                  mov ecx,esi
	$+3C     >  FFD2                  call edx

	833D????????000F85????????5668????????E8????????8B7424??83C4??85F6

	$ ==>    >  8B0D 0C495802         mov ecx,dword ptr ds:[0x258490C]         ; MHOClien.0209D0A0
	$+6      >  8B49 04               mov ecx,dword ptr ds:[ecx+0x4]
	$+9      >  8B11                  mov edx,dword ptr ds:[ecx]
	$+B      >  8B42 38               mov eax,dword ptr ds:[edx+0x38]
	$+E      >  FFD0                  call eax
	$+10     >  8B10                  mov edx,dword ptr ds:[eax]
	$+12     >  8BC8                  mov ecx,eax
	$+14     >  8B82 48020000         mov eax,dword ptr ds:[edx+0x248]
	$+1A     >  57                    push edi
	$+1B     >  FFD0                  call eax
	$+1D     >  8B4E 14               mov ecx,dword ptr ds:[esi+0x14]
	$+20     >  85C9                  test ecx,ecx

	8B0D????????8B49??8B118B42??FFD08B108BC88B82????????57FFD08B4E??85C9

	0083EE00             8B41 0C                        mov eax,dword ptr ds:[ecx+0xC]                      ; MHOClien.0257DB70
	0083EE03             C3                             retn

	8B41??C3

	// 先遣测试版本
	$ ==>    0CCAA4F9      8B0D F08EAC0D       MOV ECX,DWORD PTR DS:[0xDAC8EF0]                      ; 游戏基址
	$+6      0CCAA4FF      8B49 04             MOV ECX,DWORD PTR DS:[ECX+0x4]
	$+9      0CCAA502      8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+B      0CCAA504      8B42 38             MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+E      0CCAA507      FFD0                CALL EAX
	$+10     0CCAA509      8B10                MOV EDX,DWORD PTR DS:[EAX]
	$+12     0CCAA50B      8BC8                MOV ECX,EAX                                           ; CryActio.0A76F400
	$+14     0CCAA50D      8B82 4C020000       MOV EAX,DWORD PTR DS:[EDX+0x24C]
	$+1A     0CCAA513      57                  PUSH EDI
	$+1B     0CCAA514      FFD0                CALL EAX

	--*/
	DWORD	dwMMOClientSocketAddr = *scan_game_.ScanImme<DWORD>("8B0D????????8B49??8B118B42??FFD08B108BC88B82????????57FFD08B4E??85C9", 0x02);
	assert(dwMMOClientSocketAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_ADDR  " << dwMMOClientSocketAddr - GetModuleCryGameBase() << "  // " << dwMMOClientSocketAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientGameOffset1 = *scan_game_.ScanImme<unsigned char>("8B0D????????8B49??8B118B42??FFD08B108BC88B82????????57FFD08B4E??85C9", 0x08);
	assert(dwMMOClientGameOffset1);
	assert(dwMMOClientGameOffset1 == 0x00000004);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OFFSET1  " << dwMMOClientGameOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientSocketOffset2 = *scan_game_.ScanImme<unsigned char>("8B0D????????8B49??8B118B42??FFD08B108BC88B82????????57FFD08B4E??85C9", 0x0D);
	assert(dwMMOClientSocketOffset2);

	DWORD	dwMMOClientSocketOffset2_temp = *(DWORD*)dwMMOClientSocketAddr;		// mov ecx,dword ptr ds:[0x258490C] 
	assert(dwMMOClientSocketOffset2_temp);

	dwMMOClientSocketOffset2_temp = *(DWORD*)(dwMMOClientSocketOffset2_temp + dwMMOClientGameOffset1);		// mov ecx,dword ptr ds:[ecx+0x4]
	assert(dwMMOClientSocketOffset2_temp);

	dwMMOClientSocketOffset2_temp = *(DWORD*)dwMMOClientSocketOffset2_temp;	// mov edx,dword ptr ds:[ecx]  
	assert(dwMMOClientSocketOffset2_temp);

	dwMMOClientSocketOffset2_temp = *(DWORD*)(dwMMOClientSocketOffset2_temp + dwMMOClientSocketOffset2);	// mov eax,dword ptr ds:[edx+0x38] 
	assert(dwMMOClientSocketOffset2_temp);

	scan_tmp_.SetCodeRange((char*)dwMMOClientSocketOffset2_temp - 2, (char*)dwMMOClientSocketOffset2_temp + 0x20);
	dwMMOClientSocketOffset2 = *scan_tmp_.ScanImme<unsigned char>("8B41", 0x02);
	assert(dwMMOClientSocketOffset2);

	assert(dwMMOClientSocketOffset2 == 0x0000000C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OFFSET2  " << dwMMOClientSocketOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	rand 随机种子
	$ ==>    >  68 34A00602       PUSH MHOClien.0206A034                   ; ASCII "pet"
	$+5      >  8D4C24 10         LEA ECX,DWORD PTR SS:[ESP+0x10]
	$+9      >  E8 65D6F9FF       CALL MHOClien.0040F760
	$+E      >  8B0D E0E43102     MOV ECX,DWORD PTR DS:[0x231E4E0]         ; MHOClien.02320BF8
	$+14     >  8B89 F4000000     MOV ECX,DWORD PTR DS:[ECX+0xF4]
	$+1A     >  8B11              MOV EDX,DWORD PTR DS:[ECX]
	$+1C     >  8B52 08           MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+1F     >  6A 00             PUSH 0x0
	$+21     >  8D4424 10         LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+25     >  50                PUSH EAX
	$+26     >  6A 02             PUSH 0x2
	$+28     >  6A 01             PUSH 0x1
	$+2A     >  FFD2              CALL EDX

	68????????8D??????E8????????8B??????????8B??????????8B??8B????6A

	zhp 2016/5/17 11:44
	$-30     0B0925C0      55                    PUSH EBP
	$-2F     0B0925C1      8BEC                  MOV EBP, ESP
	$-2D     0B0925C3      81EC 98000000         SUB ESP, 98
	$-27     0B0925C9      A1 4066620C           MOV EAX, DWORD PTR DS:[C626640]
	$-22     0B0925CE      33C5                  XOR EAX, EBP
	$-20     0B0925D0      8945 FC               MOV DWORD PTR SS:[EBP-4], EAX
	$-1D     0B0925D3      837E 30 00            CMP DWORD PTR DS:[ESI+30], 0
	$-19     0B0925D7      0F84 57010000         JE CryGame.0B092734
	$-13     0B0925DD      8B86 28010000         MOV EAX, DWORD PTR DS:[ESI+128]
	$-D      0B0925E3      3B86 2C010000         CMP EAX, DWORD PTR DS:[ESI+12C]
	$-7      0B0925E9      0F84 45010000         JE CryGame.0B092734
	$-1      0B0925EF      53                    PUSH EBX
	$ ==>    0B0925F0      68 6CF85D0C           PUSH CryGame.0C5DF86C                    ; ASCII "pet"
	$+5      0B0925F5      8D8D 6CFFFFFF         LEA ECX, DWORD PTR SS:[EBP-94]
	$+B      0B0925FB      E8 B09D0000           CALL CryGame.0B09C3B0
	$+10     0B092600      8B0D 04C77C0C         MOV ECX, DWORD PTR DS:[C7CC704]          ; CryGame.0C7C8E50
	$+16     0B092606      8B89 F4000000         MOV ECX, DWORD PTR DS:[ECX+F4]
	$+1C     0B09260C      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+1E     0B09260E      8B52 08               MOV EDX, DWORD PTR DS:[EDX+8]
	$+21     0B092611      6A 00                 PUSH 0
	$+23     0B092613      8D85 6CFFFFFF         LEA EAX, DWORD PTR SS:[EBP-94]
	$+29     0B092619      50                    PUSH EAX
	$+2A     0B09261A      6A 02                 PUSH 2
	$+2C     0B09261C      6A 01                 PUSH 1
	$+2E     0B09261E      FFD2                  CALL EDX
	$+30     0B092620      8D8D 6CFFFFFF         LEA ECX, DWORD PTR SS:[EBP-94]
	$+36     0B092626      8BD8                  MOV EBX, EAX
	$+38     0B092628      E8 739F0000           CALL CryGame.0B09C5A0
	$+3D     0B09262D      85DB                  TEST EBX, EBX
	$+3F     0B09262F      0F84 FE000000         JE CryGame.0B092733
	$+45     0B092635      8B4E 30               MOV ECX, DWORD PTR DS:[ESI+30]
	$+48     0B092638      8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$+4A     0B09263A      8B50 4C               MOV EDX, DWORD PTR DS:[EAX+4C]
	$+4D     0B09263D      57                    PUSH EDI
	$+4E     0B09263E      FFD2                  CALL EDX
	$+50     0B092640      8D8D 70FFFFFF         LEA ECX, DWORD PTR SS:[EBP-90]
	$+56     0B092646      E8 C59C0000           CALL CryGame.0B09C310
	$+5B     0B09264B      A1 F8897C0C           MOV EAX, DWORD PTR DS:[C7C89F8]
	$+60     0B092650      C686 7D010000 01      MOV BYTE PTR DS:[ESI+17D], 1
	$+67     0B092657      8B48 34               MOV ECX, DWORD PTR DS:[EAX+34]
	$+6A     0B09265A      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+6C     0B09265C      8B82 E8030000         MOV EAX, DWORD PTR DS:[EDX+3E8]
	$+72     0B092662      FFD0                  CALL EAX
	$+74     0B092664      8BF8                  MOV EDI, EAX
	$+76     0B092666      85FF                  TEST EDI, EDI
	$+78     0B092668      0F84 B2000000         JE CryGame.0B092720
	$+7E     0B09266E      8B17                  MOV EDX, DWORD PTR DS:[EDI]
	$+80     0B092670      8B86 64010000         MOV EAX, DWORD PTR DS:[ESI+164]
	$+86     0B092676      8B52 10               MOV EDX, DWORD PTR DS:[EDX+10]
	$+89     0B092679      50                    PUSH EAX
	$+8A     0B09267A      8BCF                  MOV ECX, EDI
	$+8C     0B09267C      FFD2                  CALL EDX
	$+8E     0B09267E      8986 60010000         MOV DWORD PTR DS:[ESI+160], EAX
	$+94     0B092684      85C0                  TEST EAX, EAX
	$+96     0B092686      74 13                 JE SHORT CryGame.0B09269B
	$+98     0B092688      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+9A     0B09268A      8BC8                  MOV ECX, EAX
	$+9C     0B09268C      8B42 18               MOV EAX, DWORD PTR DS:[EDX+18]
	$+9F     0B09268F      FFD0                  CALL EAX
	$+A1     0B092691      C786 60010000 0000000>MOV DWORD PTR DS:[ESI+160], 0
	$+AB     0B09269B      8B0F                  MOV ECX, DWORD PTR DS:[EDI]
	$+AD     0B09269D      8B13                  MOV EDX, DWORD PTR DS:[EBX]
	$+AF     0B09269F      8B42 20               MOV EAX, DWORD PTR DS:[EDX+20]
	$+B2     0B0926A2      898D 68FFFFFF         MOV DWORD PTR SS:[EBP-98], ECX           ; ntdll.778701BF
	$+B8     0B0926A8      6A 00                 PUSH 0
	$+BA     0B0926AA      8BCB                  MOV ECX, EBX
	$+BC     0B0926AC      FFD0                  CALL EAX
	$+BE     0B0926AE      8B95 68FFFFFF         MOV EDX, DWORD PTR SS:[EBP-98]
	$+C4     0B0926B4      50                    PUSH EAX
	$+C5     0B0926B5      8B42 04               MOV EAX, DWORD PTR DS:[EDX+4]
	$+C8     0B0926B8      8BCF                  MOV ECX, EDI
	$+CA     0B0926BA      FFD0                  CALL EAX
	$+CC     0B0926BC      8986 64010000         MOV DWORD PTR DS:[ESI+164], EAX
	$+D2     0B0926C2      8B17                  MOV EDX, DWORD PTR DS:[EDI]
	$+D4     0B0926C4      50                    PUSH EAX
	$+D5     0B0926C5      8B42 10               MOV EAX, DWORD PTR DS:[EDX+10]
	$+D8     0B0926C8      8BCF                  MOV ECX, EDI
	$+DA     0B0926CA      FFD0                  CALL EAX
	$+DC     0B0926CC      8986 60010000         MOV DWORD PTR DS:[ESI+160], EAX
	$+E2     0B0926D2      85C0                  TEST EAX, EAX
	$+E4     0B0926D4      74 4A                 JE SHORT CryGame.0B092720
	$+E6     0B0926D6      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+E8     0B0926D8      8BC8                  MOV ECX, EAX
	$+EA     0B0926DA      8B42 48               MOV EAX, DWORD PTR DS:[EDX+48]
	$+ED     0B0926DD      6A 01                 PUSH 1
	$+EF     0B0926DF      FFD0                  CALL EAX
	$+F1     0B0926E1      8B8E 60010000         MOV ECX, DWORD PTR DS:[ESI+160]
	$+F7     0B0926E7      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+F9     0B0926E9      8B42 08               MOV EAX, DWORD PTR DS:[EDX+8]
	$+FC     0B0926EC      FFD0                  CALL EAX
	$+FE     0B0926EE      85C0                  TEST EAX, EAX
	$+100    0B0926F0      75 2E                 JNZ SHORT CryGame.0B092720
	$+102    0B0926F2      8B8E 60010000         MOV ECX, DWORD PTR DS:[ESI+160]
	$+108    0B0926F8      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+10A    0B0926FA      8B42 14               MOV EAX, DWORD PTR DS:[EDX+14]
	$+10D    0B0926FD      68 989A5E0C           PUSH CryGame.0C5E9A98                    ; ASCII "Shaders/EngineAssets/Textures/avatarbg/avatarimage.dds"
	$+112    0B092702      FFD0                  CALL EAX
	$+114    0B092704      8B8E 60010000         MOV ECX, DWORD PTR DS:[ESI+160]
	$+11A    0B09270A      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+11C    0B09270C      8B42 70               MOV EAX, DWORD PTR DS:[EDX+70]
	$+11F    0B09270F      FFD0                  CALL EAX
	$+121    0B092711      8B8E 60010000         MOV ECX, DWORD PTR DS:[ESI+160]
	$+127    0B092717      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+129    0B092719      8B42 28               MOV EAX, DWORD PTR DS:[EDX+28]
	$+12C    0B09271C      6A 00                 PUSH 0
	$+12E    0B09271E      FFD0                  CALL EAX
	$+130    0B092720      8D8D 70FFFFFF         LEA ECX, DWORD PTR SS:[EBP-90]
	$+136    0B092726      C686 7C010000 00      MOV BYTE PTR DS:[ESI+17C], 0
	$+13D    0B09272D      E8 1E000000           CALL CryGame.0B092750
	$+142    0B092732      5F                    POP EDI                                  ; ntdll.7787FD91
	$+143    0B092733      5B                    POP EBX                                  ; ntdll.7787FD91
	$+144    0B092734      8B4D FC               MOV ECX, DWORD PTR SS:[EBP-4]
	$+147    0B092737      33CD                  XOR ECX, EBP
	$+149    0B092739      E8 E7981E01           CALL CryGame.0C27C025
	$+14E    0B09273E      8BE5                  MOV ESP, EBP
	$+150    0B092740      5D                    POP EBP                                  ; ntdll.7787FD91
	$+151    0B092741      C3                    RETN
	--*/
	//68????????8d??????????e8????????8b??????????8b??????????8b??8b????6a??8d??????????506a??6a??ff
	DWORD	dwMMOClientRandBaseAddr = *scan_game_.ScanImme<DWORD>("68????????8d??????????e8????????8b??????????8b??????????8b??8b????6a??8d??????????506a??6a??ff", 0x10 + 2);
	assert(dwMMOClientRandBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_RAND_BASE_ADDR  " << dwMMOClientRandBaseAddr - GetModuleCryGameBase() << "  // " << dwMMOClientRandBaseAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	发包CALL2
	$-3      >  57                     PUSH EDI
	$-2      >  8BF9                   MOV EDI,ECX
	$ ==>    >  837F 24 00             CMP DWORD PTR DS:[EDI+0x24],0x0
	$+4      >  74 56                  JE SHORT MHOClien.0077475F
	$+6      >  56                     PUSH ESI
	$+7      >  8B7424 0C              MOV ESI,DWORD PTR SS:[ESP+0xC]
	$+B      >  B8 04050000            MOV EAX,0x504
	$+10     >  C747 2C 02000000       MOV DWORD PTR DS:[EDI+0x2C],0x2
	$+17     >  66:3B06                CMP AX,WORD PTR DS:[ESI]
	$+1A     >  75 0F                  JNZ SHORT MHOClien.0077472E
	$+1C     >  E8 1CFCFFFF            CALL MHOClien.00774340
	$+21     >  8946 18                MOV DWORD PTR DS:[ESI+0x18],EAX
	$+24     >  C746 1C 00000000       MOV DWORD PTR DS:[ESI+0x1C],0x0
	$+2B     >  56                     PUSH ESI
	$+2C     >  8B77 24                MOV ESI,DWORD PTR DS:[EDI+0x24]
	$+2F     >  E8 79190000            CALL MHOClien.007760B0
	$+34     >  837F 2C 03             CMP DWORD PTR DS:[EDI+0x2C],0x3
	$+38     >  8BF0                   MOV ESI,EAX
	$+3A     >  75 12                  JNZ SHORT MHOClien.00774751
	$+3C     >  8B17                   MOV EDX,DWORD PTR DS:[EDI]
	$+3E     >  8B42 24                MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+41     >  6A 00                  PUSH 0x0
	$+43     >  8BCF                   MOV ECX,EDI
	$+45     >  C747 2C 00000000       MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+4C     >  FFD0                   CALL EAX
	$+4E     >  8BC6                   MOV EAX,ESI
	$+50     >  5E                     POP ESI
	$+51     >  C747 2C 00000000       MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+58     >  5F                     POP EDI
	$+59     >  C2 0400                RETN 0x4
	$+5C     >  83C8 FF                OR EAX,0xFFFFFFFF
	$+5F     >  5F                     POP EDI
	$+60     >  C2 0400                RETN 0x4

	837F????74??568B7424??B8????????C747

	$-10     0069A870       55                      PUSH EBP
	$-F      0069A871       8BEC                    MOV EBP,ESP
	$-D      0069A873       57                      PUSH EDI
	$-C      0069A874       8BF9                    MOV EDI,ECX
	$-A      0069A876       837F 24 00              CMP DWORD PTR DS:[EDI+0x24],0x0
	$-6      0069A87A       74 56                   JE SHORT MHOClien.0069A8D2
	$-4      0069A87C       56                      PUSH ESI
	$-3      0069A87D       8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$ ==>    0069A880       B8 04050000             MOV EAX,0x504
	$+5      0069A885       C747 2C 02000000        MOV DWORD PTR DS:[EDI+0x2C],0x2
	$+C      0069A88C       66:3B06                 CMP AX,WORD PTR DS:[ESI]
	$+F      0069A88F       75 0F                   JNZ SHORT MHOClien.0069A8A0
	$+11     0069A891       E8 DAFBFFFF             CALL MHOClien.0069A470
	$+16     0069A896       8946 18                 MOV DWORD PTR DS:[ESI+0x18],EAX
	$+19     0069A899       C746 1C 00000000        MOV DWORD PTR DS:[ESI+0x1C],0x0
	$+20     0069A8A0       56                      PUSH ESI
	$+21     0069A8A1       8B77 24                 MOV ESI,DWORD PTR DS:[EDI+0x24]
	$+24     0069A8A4       E8 E7843B02             CALL MHOClien.02A52D90

	B804050000C747??02000000

	B8????????C7????????????66????75??E8????????89????C7??????????????8B

	$-E      0069E690          57                    PUSH EDI
	$-D      0069E691          8BF9                  MOV EDI,ECX
	$-B      0069E693          837F 24 00            CMP DWORD PTR DS:[EDI+0x24],0x0
	$-7      0069E697          74 56                 JE SHORT MHOClien.0069E6EF
	$-5      0069E699          56                    PUSH ESI
	$-4      0069E69A          8B7424 0C             MOV ESI,DWORD PTR SS:[ESP+0xC]
	$ ==>    0069E69E          B8 04050000           MOV EAX,0x504
	$+5      0069E6A3          C747 2C 02000000      MOV DWORD PTR DS:[EDI+0x2C],0x2
	$+C      0069E6AA          66:3B06               CMP AX,WORD PTR DS:[ESI]
	$+F      0069E6AD          75 0F                 JNZ SHORT MHOClien.0069E6BE
	$+11     0069E6AF          E8 DCFBFFFF           CALL MHOClien.0069E290
	$+16     0069E6B4          8946 18               MOV DWORD PTR DS:[ESI+0x18],EAX
	$+19     0069E6B7          C746 1C 00000000      MOV DWORD PTR DS:[ESI+0x1C],0x0
	$+20     0069E6BE          56                    PUSH ESI
	$+21     0069E6BF          8B77 24               MOV ESI,DWORD PTR DS:[EDI+0x24]
	$+24     0069E6C2          E8 169FAC00           CALL MHOClien.011685DD
	$+29     0069E6C7          837F 2C 03            CMP DWORD PTR DS:[EDI+0x2C],0x3
	$+2D     0069E6CB          8BF0                  MOV ESI,EAX
	$+2F     0069E6CD          75 12                 JNZ SHORT MHOClien.0069E6E1
	$+31     0069E6CF          8B17                  MOV EDX,DWORD PTR DS:[EDI]
	$+33     0069E6D1          8B42 24               MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+36     0069E6D4          6A 00                 PUSH 0x0
	$+38     0069E6D6          8BCF                  MOV ECX,EDI
	$+3A     0069E6D8          C747 2C 00000000      MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+41     0069E6DF          FFD0                  CALL EAX
	$+43     0069E6E1          8BC6                  MOV EAX,ESI
	$+45     0069E6E3          5E                    POP ESI
	$+46     0069E6E4          C747 2C 00000000      MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+4D     0069E6EB          5F                    POP EDI
	$+4E     0069E6EC          C2 0400               RETN 0x4
	$+51     0069E6EF          83C8 FF               OR EAX,0xFFFFFFFF
	$+54     0069E6F2          5F                    POP EDI
	$+55     0069E6F3          C2 0400               RETN 0x4

	zhp:2016/3/22 19:47

	$-10     >  55              PUSH EBP                                            ; 发包函数
	$-F      >  8BEC            MOV EBP,ESP
	$-D      >  57              PUSH EDI
	$-C      >  8BF9            MOV EDI,ECX
	$-A      >  837F 24 00      CMP DWORD PTR DS:[EDI+0x24],0x0
	$-6      >  74 56           JE SHORT CryGame.0AEC6EC2
	$-4      >  56              PUSH ESI
	$-3      >  8B75 08         MOV ESI,DWORD PTR SS:[EBP+0x8]
	$ ==>    >  B8 04050000     MOV EAX,0x504
	$+5      >  C747 2C 0200000>MOV DWORD PTR DS:[EDI+0x2C],0x2
	$+C      >  66:3B06         CMP AX,WORD PTR DS:[ESI]
	$+F      >  75 0F           JNZ SHORT CryGame.0AEC6E90
	$+11     >  E8 2AFBFFFF     CALL CryGame.0AEC69B0
	$+16     >  8946 18         MOV DWORD PTR DS:[ESI+0x18],EAX
	$+19     >  C746 1C 0000000>MOV DWORD PTR DS:[ESI+0x1C],0x0
	$+20     >  56              PUSH ESI
	$+21     >  8B77 24         MOV ESI,DWORD PTR DS:[EDI+0x24]
	$+24     >  E8 467BA301     CALL CryGame.0C8FE9DF
	$+29     >  837F 2C 03      CMP DWORD PTR DS:[EDI+0x2C],0x3
	$+2D     >  8BF0            MOV ESI,EAX
	$+2F     >  75 12           JNZ SHORT CryGame.0AEC6EB3
	$+31     >  8B17            MOV EDX,DWORD PTR DS:[EDI]
	$+33     >  8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+36     >  6A 00           PUSH 0x0
	$+38     >  8BCF            MOV ECX,EDI
	$+3A     >  C747 2C 0000000>MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+41     >  FFD0            CALL EAX
	$+43     >  8BC6            MOV EAX,ESI
	$+45     >  5E              POP ESI
	$+46     >  C747 2C 0000000>MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+4D     >  5F              POP EDI
	$+4E     >  5D              POP EBP
	$+4F     >  C2 0400         RETN 0x4
	$+52     >  83C8 FF         OR EAX,0xFFFFFFFF
	$+55     >  5F              POP EDI
	$+56     >  5D              POP EBP
	$+57     >  C2 0400         RETN 0x4

	B8????????C7????????????66????75??E8????????89????C7??????????????8B
	--*/
	DWORD	dwMMOClientSendcall2 = (DWORD)scan_game_.ScanAddr("B8????????C7????????????66????75??E8????????89????C7??????????????8B", -0xE);
	if (dwMMOClientSendcall2)
	{
		byte bCode = *(byte*)dwMMOClientSendcall2;
		if (bCode != 0x57)
		{
			dwMMOClientSendcall2 = (DWORD)scan_game_.ScanAddr("B8????????C7????????????66????75??E8????????89????C7??????????????8B", -0x10);
			if (dwMMOClientSendcall2)
			{
				byte bCode = *(byte*)dwMMOClientSendcall2;
				if (bCode != 0x55)
				{
					assert(false);
				}
			}
		}
	}
	else
	{
		//zhp,这个特殊码才有效
		dwMMOClientSendcall2 = (DWORD)scan_game_.ScanAddr("B8????????C7????????????66????75??E8????????89????C7??????????????8B", -0x10);
		if (dwMMOClientSendcall2)
		{
			byte bCode = *(byte*)dwMMOClientSendcall2;
			if (bCode != 0x55)
			{
				assert(false);
			}
		}
	}

	assert(dwMMOClientSendcall2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SEND_CALL2  " << dwMMOClientSendcall2 - GetModuleCryGameBase() << "  // " << dwMMOClientSendcall2 << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    >  8B41 04                MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+3      >  8BD1                   MOV EDX,ECX
	$+5      >  85C0                   TEST EAX,EAX
	$+7      >  74 33                  JE SHORT MHOClien.015B1BDC
	$+9      >  56                     PUSH ESI
	$+A      >  8B7424 0C              MOV ESI,DWORD PTR SS:[ESP+0xC]
	$+E      >  8B36                   MOV ESI,DWORD PTR DS:[ESI]
	$+10     >  3970 10                CMP DWORD PTR DS:[EAX+0x10],ESI
	$+13     >  72 07                  JB SHORT MHOClien.015B1BBC
	$+15     >  8BD0                   MOV EDX,EAX
	$+17     >  8B40 08                MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+1A     >  EB 03                  JMP SHORT MHOClien.015B1BBF
	$+1C     >  8B40 0C                MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+1F     >  85C0                   TEST EAX,EAX
	$+21     >^ 75 ED                  JNZ SHORT MHOClien.015B1BB0
	$+23     >  8B4424 08              MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+27     >  3BD1                   CMP EDX,ECX
	$+29     >  74 0B                  JE SHORT MHOClien.015B1BD6
	$+2B     >  3B72 10                CMP ESI,DWORD PTR DS:[EDX+0x10]
	$+2E     >  73 06                  JNB SHORT MHOClien.015B1BD6
	$+30     >  8908                   MOV DWORD PTR DS:[EAX],ECX
	$+32     >  5E                     POP ESI
	$+33     >  C2 0800                RETN 0x8
	$+36     >  8910                   MOV DWORD PTR DS:[EAX],EDX
	$+38     >  5E                     POP ESI
	$+39     >  C2 0800                RETN 0x8
	$+3C     >  8B4424 04              MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+40     >  8910                   MOV DWORD PTR DS:[EAX],EDX
	$+42     >  C2 0800                RETN 0x8

	8B41??8BD185C074??568B??????8B??39????72

	$-10     >  8B41 04         MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-D      >  8BD1            MOV EDX,ECX
	$-B      >  85C0            TEST EAX,EAX
	$-9      >  74 33           JE SHORT MHOClien.00A315BC
	$-7      >  56              PUSH ESI
	$-6      >  8B7424 0C       MOV ESI,DWORD PTR SS:[ESP+0xC]
	$-2      >  8B36            MOV ESI,DWORD PTR DS:[ESI]
	$ ==>    >  3970 10         CMP DWORD PTR DS:[EAX+0x10],ESI
	$+3      >  72 07           JB SHORT MHOClien.00A3159C
	$+5      >  8BD0            MOV EDX,EAX
	$+7      >  8B40 08         MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+A      >  EB 03           JMP SHORT MHOClien.00A3159F
	$+C      >  8B40 0C         MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+F      >  85C0            TEST EAX,EAX
	$+11     >^ 75 ED           JNZ SHORT MHOClien.00A31590
	$+13     >  8B4424 08       MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+17     >  3BD1            CMP EDX,ECX
	$+19     >  74 0B           JE SHORT MHOClien.00A315B6
	$+1B     >  3B72 10         CMP ESI,DWORD PTR DS:[EDX+0x10]
	$+1E     >  73 06           JNB SHORT MHOClien.00A315B6
	$+20     >  8908            MOV DWORD PTR DS:[EAX],ECX
	$+22     >  5E              POP ESI
	$+23     >  C2 0800         RETN 0x8
	$+26     >  8910            MOV DWORD PTR DS:[EAX],EDX
	$+28     >  5E              POP ESI
	$+29     >  C2 0800         RETN 0x8
	$+2C     >  8B4424 04       MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+30     >  8910            MOV DWORD PTR DS:[EAX],EDX
	$+32     >  C2 0800         RETN 0x8

	39????72??8B??8B????EB??8B????85??75??8B??????3B??74??3B

	$-12     010BF340       55                      PUSH EBP
	$-11     010BF341       8BEC                    MOV EBP,ESP
	$-F      010BF343       8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-C      010BF346       8BD1                    MOV EDX,ECX
	$-A      010BF348       85C0                    TEST EAX,EAX
	$-8      010BF34A       74 33                   JE SHORT MHOClien.010BF37F
	$-6      010BF34C       56                      PUSH ESI
	$-5      010BF34D       8B75 0C                 MOV ESI,DWORD PTR SS:[EBP+0xC]
	$-2      010BF350       8B36                    MOV ESI,DWORD PTR DS:[ESI]
	$ ==>    010BF352       3970 10                 CMP DWORD PTR DS:[EAX+0x10],ESI
	$+3      010BF355       72 07                   JB SHORT MHOClien.010BF35E
	$+5      010BF357       8BD0                    MOV EDX,EAX
	$+7      010BF359       8B40 08                 MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+A      010BF35C       EB 03                   JMP SHORT MHOClien.010BF361
	$+C      010BF35E       8B40 0C                 MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+F      010BF361       85C0                    TEST EAX,EAX
	$+11     010BF363     ^ 75 ED                   JNZ SHORT MHOClien.010BF352
	$+13     010BF365       8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+16     010BF368       3BD1                    CMP EDX,ECX
	$+18     010BF36A       74 0C                   JE SHORT MHOClien.010BF378
	$+1A     010BF36C       3B72 10                 CMP ESI,DWORD PTR DS:[EDX+0x10]
	$+1D     010BF36F       73 07                   JNB SHORT MHOClien.010BF378

	39????72??8B??8B????EB??8B????85??75??8B????3B??74??3B
	--*/
	DWORD	dwMMOClientGetActorObjAddrCall = (DWORD)scan_game_.ScanAddr("39????72??8B??8B????EB??8B????85??75??8B????3B??74??3B", -0x12);
	assert(dwMMOClientGetActorObjAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_ACTOR_OBJADDR_CALL  " << dwMMOClientGetActorObjAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetActorObjAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	// 技能CALL
	$ ==>    >  8B07            MOV EAX,DWORD PTR DS:[EDI]
	$+2      >  8B8E 1C030000   MOV ECX,DWORD PTR DS:[ESI+0x31C]
	$+8      >  8D1480          LEA EDX,DWORD PTR DS:[EAX+EAX*4]
	$+B      >  8D04D1          LEA EAX,DWORD PTR DS:[ECX+EDX*8]
	$+E      >  8B8E 34030000   MOV ECX,DWORD PTR DS:[ESI+0x334]         ; 2F104F98
	$+14     >  50              PUSH EAX                                 ; 535F0C00
	$+15     >  E8 8FE64901     CALL MHOClien.02B02289                   ; 3862EDF0
	$+1A     >  84C0            TEST AL,AL
	$+1C     >  75 0B           JNZ SHORT MHOClien.01663C09
	$+1E     >  83C7 04         ADD EDI,0x4
	$+21     >  3BBE 2C030000   CMP EDI,DWORD PTR DS:[ESI+0x32C]
	$+27     >^ 75 D7           JNZ SHORT MHOClien.01663BE0
	$+29     >  8B4D 04         MOV ECX,DWORD PTR SS:[EBP+0x4]
	$+2C     >  8B45 00         MOV EAX,DWORD PTR SS:[EBP]
	$+2F     >  3BC1            CMP EAX,ECX
	$+31     >  74 16           JE SHORT MHOClien.01663C29

	8B??8B??????????8D????8D????8B??????????50E8

	$ ==>    0CB1FF53       8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$+2      0CB1FF55       8B8B 44040000           MOV ECX,DWORD PTR DS:[EBX+0x444]
	$+8      0CB1FF5B       8BD0                    MOV EDX,EAX		// LEA EAX,DWORD PTR DS:[ECX+EDX*8] 这段汇编指令另一种表现形式
	$+A      0CB1FF5D       C1E2 04                 SHL EDX,0x4
	$+D      0CB1FF60       03D0                    ADD EDX,EAX
	$+F      0CB1FF62       8D0491                  LEA EAX,DWORD PTR DS:[ECX+EDX*4]
	$+12     0CB1FF65       8B8B 5C040000           MOV ECX,DWORD PTR DS:[EBX+0x45C]
	$+18     0CB1FF6B       50                      PUSH EAX
	$+19     0CB1FF6C       E8 7F800A00             CALL CryGame.0CBC7FF0                    ; 技能CALL
	$+1E     0CB1FF71       84C0                    TEST AL,AL
	$+20     0CB1FF73       75 0B                   JNZ SHORT CryGame.0CB1FF80
	$+22     0CB1FF75       83C6 04                 ADD ESI,0x4
	$+25     0CB1FF78       3BB3 54040000           CMP ESI,DWORD PTR DS:[EBX+0x454]
	$+2B     0CB1FF7E     ^ 75 D3                   JNZ SHORT CryGame.0CB1FF53
	$+2D     0CB1FF80       8B55 F8                 MOV EDX,DWORD PTR SS:[EBP-0x8]
	$+30     0CB1FF83       8B4A 04                 MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+33     0CB1FF86       8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$+35     0CB1FF88       3BC1                    CMP EAX,ECX
	$+37     0CB1FF8A       74 19                   JE SHORT CryGame.0CB1FFA5
	$+39     0CB1FF8C       8BF1                    MOV ESI,ECX
	$+3B     0CB1FF8E       2BF1                    SUB ESI,ECX
	$+3D     0CB1FF90       74 10                   JE SHORT CryGame.0CB1FFA2

	8B??8B??????????8B??C1????03??8D????8B????????????E8

	--*/
	DWORD	dwMMOClientSkillCall = (DWORD)scan_game_.ScanCall("8B??8B??????????8D????8D????8B??????????50E8", 0x16);

	if (dwMMOClientSkillCall == 0)
	{
		dwMMOClientSkillCall = (DWORD)scan_game_.ScanCall("8B??8B??????????8B??C1????03??8D????8B????????????E8", 0x19 + 1);
	}
	assert(dwMMOClientSkillCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SKILL_CALL  " << dwMMOClientSkillCall - GetModuleCryGameBase() << "  // " << dwMMOClientSkillCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    014E870A     8B8E 901D0000       MOV ECX,DWORD PTR DS:[ESI+0x1D90]
	$+6      014E8710     85C9                TEST ECX,ECX
	$+8      014E8712     74 14               JE SHORT MHOClien.014E8728
	$+A      014E8714     E8 477F35FF         CALL MHOClien.00840660
	$+F      014E8719     85C0                TEST EAX,EAX
	$+11     014E871B     74 0B               JE SHORT MHOClien.014E8728
	$+13     014E871D     8B8E 901D0000       MOV ECX,DWORD PTR DS:[ESI+0x1D90]
	$+19     014E8723     E8 12AE85FF         CALL MHOClien.00D4353A
	$+1E     014E8728     8B8E 801B0000       MOV ECX,DWORD PTR DS:[ESI+0x1B80]
	$+24     014E872E     85C9                TEST ECX,ECX
	$+26     014E8730     74 32               JE SHORT MHOClien.014E8764

	8B8E????????85C9????E8????????85C074??8B8E

	//zhp
	$-6E     0B4DBB20      85C0                  TEST EAX, EAX
	$-6C     0B4DBB22    ^ 0F84 2CFCFFFF         JE CryGame.0B4DB754
	$-66     0B4DBB28      833D A03B7A0C 00      CMP DWORD PTR DS:[C7A3BA0], 0
	$-5F     0B4DBB2F    ^ 0F84 1FFCFFFF         JE CryGame.0B4DB754
	$-59     0B4DBB35      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$-57     0B4DBB37      8BC8                  MOV ECX, EAX
	$-55     0B4DBB39      8B42 54               MOV EAX, DWORD PTR DS:[EDX+54]
	$-52     0B4DBB3C      FFD0                  CALL EAX
	$-50     0B4DBB3E      D900                  FLD DWORD PTR DS:[EAX]
	$-4E     0B4DBB40      8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$-4C     0B4DBB42      D95D E4               FSTP DWORD PTR SS:[EBP-1C]
	$-49     0B4DBB45      D940 04               FLD DWORD PTR DS:[EAX+4]
	$-46     0B4DBB48      8BCE                  MOV ECX, ESI
	$-44     0B4DBB4A      D95D E8               FSTP DWORD PTR SS:[EBP-18]
	$-41     0B4DBB4D      D940 08               FLD DWORD PTR DS:[EAX+8]
	$-3E     0B4DBB50      8B82 08030000         MOV EAX, DWORD PTR DS:[EDX+308]
	$-38     0B4DBB56      D95D EC               FSTP DWORD PTR SS:[EBP-14]
	$-35     0B4DBB59      FFD0                  CALL EAX
	$-33     0B4DBB5B      8B55 E4               MOV EDX, DWORD PTR SS:[EBP-1C]
	$-30     0B4DBB5E      8B40 04               MOV EAX, DWORD PTR DS:[EAX+4]
	$-2D     0B4DBB61      83EC 0C               SUB ESP, 0C
	$-2A     0B4DBB64      8BCC                  MOV ECX, ESP
	$-28     0B4DBB66      8911                  MOV DWORD PTR DS:[ECX], EDX
	$-26     0B4DBB68      8B55 E8               MOV EDX, DWORD PTR SS:[EBP-18]
	$-23     0B4DBB6B      8951 04               MOV DWORD PTR DS:[ECX+4], EDX
	$-20     0B4DBB6E      8B55 EC               MOV EDX, DWORD PTR SS:[EBP-14]           ; MHOCli_1.0FA6E466
	$-1D     0B4DBB71      8951 08               MOV DWORD PTR DS:[ECX+8], EDX
	$-1A     0B4DBB74      8B0D A03B7A0C         MOV ECX, DWORD PTR DS:[C7A3BA0]
	$-14     0B4DBB7A      50                    PUSH EAX
	$-13     0B4DBB7B      E8 40884C00           CALL CryGame.0B9A43C0
	$-E      0B4DBB80    ^ E9 CFFBFFFF           JMP CryGame.0B4DB754
	$-9      0B4DBB85      83F9 1E               CMP ECX, 1E
	$-6      0B4DBB88    ^ 0F85 C9FBFFFF         JNZ CryGame.0B4DB757
	$ ==>    0B4DBB8E      8B8E 141E0000         MOV ECX, DWORD PTR DS:[ESI+1E14]
	$+6      0B4DBB94      85C9                  TEST ECX, ECX                            ; ntdll.779301E4
	$+8      0B4DBB96      74 14                 JE SHORT CryGame.0B4DBBAC
	$+A      0B4DBB98      E8 73AF0D00           CALL CryGame.0B5B6B10
	$+F      0B4DBB9D      85C0                  TEST EAX, EAX
	$+11     0B4DBB9F      74 0B                 JE SHORT CryGame.0B4DBBAC
	$+13     0B4DBBA1      8B8E 141E0000         MOV ECX, DWORD PTR DS:[ESI+1E14]
	$+19     0B4DBBA7      E8 74B20D00           CALL CryGame.0B5B6E20
	$+1E     0B4DBBAC      8B8E 041C0000         MOV ECX, DWORD PTR DS:[ESI+1C04]         ; mhoclien.006E0069
	$+24     0B4DBBB2      85C9                  TEST ECX, ECX                            ; ntdll.779301E4
	$+26     0B4DBBB4      74 25                 JE SHORT CryGame.0B4DBBDB
	$+28     0B4DBBB6      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+2A     0B4DBBB8      8B42 60               MOV EAX, DWORD PTR DS:[EDX+60]
	$+2D     0B4DBBBB      FFD0                  CALL EAX
	$+2F     0B4DBBBD      84C0                  TEST AL, AL
	$+31     0B4DBBBF      74 0D                 JE SHORT CryGame.0B4DBBCE
	$+33     0B4DBBC1      8B8E 041C0000         MOV ECX, DWORD PTR DS:[ESI+1C04]         ; mhoclien.006E0069
	$+39     0B4DBBC7      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+3B     0B4DBBC9      8B42 64               MOV EAX, DWORD PTR DS:[EDX+64]
	$+3E     0B4DBBCC      FFD0                  CALL EAX
	$+40     0B4DBBCE      8B8E 041C0000         MOV ECX, DWORD PTR DS:[ESI+1C04]         ; mhoclien.006E0069
	$+46     0B4DBBD4      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+48     0B4DBBD6      8B42 04               MOV EAX, DWORD PTR DS:[EDX+4]
	$+4B     0B4DBBD9      FFD0                  CALL EAX
	$+4D     0B4DBBDB      8B8E 041C0000         MOV ECX, DWORD PTR DS:[ESI+1C04]         ; mhoclien.006E0069
	$+53     0B4DBBE1      85C9                  TEST ECX, ECX                            ; ntdll.779301E4
	$+55     0B4DBBE3      74 32                 JE SHORT CryGame.0B4DBC17
	$+57     0B4DBBE5      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+59     0B4DBBE7      8B42 60               MOV EAX, DWORD PTR DS:[EDX+60]
	$+5C     0B4DBBEA      FFD0                  CALL EAX
	$+5E     0B4DBBEC      84C0                  TEST AL, AL
	$+60     0B4DBBEE      74 27                 JE SHORT CryGame.0B4DBC17
	$+62     0B4DBBF0      8B8E 041C0000         MOV ECX, DWORD PTR DS:[ESI+1C04]         ; mhoclien.006E0069
	$+68     0B4DBBF6      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+6A     0B4DBBF8      8B42 5C               MOV EAX, DWORD PTR DS:[EDX+5C]
	$+6D     0B4DBBFB      6A 00                 PUSH 0
	$+6F     0B4DBBFD      FFD0                  CALL EAX
	$+71     0B4DBBFF      8B0D 8879780C         MOV ECX, DWORD PTR DS:[C787988]          ; mhoclien.01820E10
	$+77     0B4DBC05      8B89 BC000000         MOV ECX, DWORD PTR DS:[ECX+BC]
	$+7D     0B4DBC0B      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+7F     0B4DBC0D      8B82 C4000000         MOV EAX, DWORD PTR DS:[EDX+C4]
	$+85     0B4DBC13      6A 01                 PUSH 1
	$+87     0B4DBC15      FFD0                  CALL EAX
	$+89     0B4DBC17      8B4E 0C               MOV ECX, DWORD PTR DS:[ESI+C]
	$+8C     0B4DBC1A      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$+8E     0B4DBC1C      8B82 4C010000         MOV EAX, DWORD PTR DS:[EDX+14C]
	$+94     0B4DBC22      6A 00                 PUSH 0
	$+96     0B4DBC24      FFD0                  CALL EAX
	$+98     0B4DBC26      85C0                  TEST EAX, EAX
	$+9A     0B4DBC28    ^ 0F84 29FBFFFF         JE CryGame.0B4DB757
	$+A0     0B4DBC2E      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+A2     0B4DBC30      8BC8                  MOV ECX, EAX
	$+A4     0B4DBC32      8B42 64               MOV EAX, DWORD PTR DS:[EDX+64]
	$+A7     0B4DBC35      FFD0                  CALL EAX
	$+A9     0B4DBC37      84C0                  TEST AL, AL
	$+AB     0B4DBC39    ^ 0F84 18FBFFFF         JE CryGame.0B4DB757
	$+B1     0B4DBC3F      8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$+B3     0B4DBC41      8B82 04070000         MOV EAX, DWORD PTR DS:[EDX+704]
	$+B9     0B4DBC47      8BCE                  MOV ECX, ESI
	$+BB     0B4DBC49      FFD0                  CALL EAX
	$+BD     0B4DBC4B    ^ E9 07FBFFFF           JMP CryGame.0B4DB757
	$+C2     0B4DBC50      8378 04 00            CMP DWORD PTR DS:[EAX+4], 0
	$+C6     0B4DBC54    ^ 0F84 FDFAFFFF         JE CryGame.0B4DB757
	$+CC     0B4DBC5A      8B40 04               MOV EAX, DWORD PTR DS:[EAX+4]
	$+CF     0B4DBC5D      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+D1     0B4DBC5F      8BC8                  MOV ECX, EAX
	$+D3     0B4DBC61      8B42 6C               MOV EAX, DWORD PTR DS:[EDX+6C]
	$+D6     0B4DBC64      FFD0                  CALL EAX
	$+D8     0B4DBC66      8B0B                  MOV ECX, DWORD PTR DS:[EBX]
	$+DA     0B4DBC68      83F9 0C               CMP ECX, 0C
	$+DD     0B4DBC6B      75 18                 JNZ SHORT CryGame.0B4DBC85
	$+DF     0B4DBC6D      85C0                  TEST EAX, EAX
	$+E1     0B4DBC6F      74 14                 JE SHORT CryGame.0B4DBC85
	$+E3     0B4DBC71      8B4E 0C               MOV ECX, DWORD PTR DS:[ESI+C]
	$+E6     0B4DBC74      8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+E8     0B4DBC76      8B52 28               MOV EDX, DWORD PTR DS:[EDX+28]
	$+EB     0B4DBC79      51                    PUSH ECX                                 ; ntdll.779301E4
	$+EC     0B4DBC7A      6A 0E                 PUSH 0E
	$+EE     0B4DBC7C      8BC8                  MOV ECX, EAX
	$+F0     0B4DBC7E      FFD2                  CALL EDX
	$+F2     0B4DBC80    ^ E9 D2FAFFFF           JMP CryGame.0B4DB757
	--*/
	//feature code:8B8E????????85C9????E8????????85C074??8B8E
	DWORD	dwMMOClientSkillOffset = *scan_game_.ScanImme<DWORD>("8B8E????????85C9????E8????????85C074??8B8E", 0x02);
	/*ASSERT_GWLR(dwMMOClientSkillOffset==0x1D90);*/
	assert(dwMMOClientSkillOffset == 0x1e14);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SKILL_OFFSET  " << dwMMOClientSkillOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    >  51                 PUSH ECX                                 ; 加密数据为16位 用 12345678来表示每个字节
	$+1      >  8B01               MOV EAX,DWORD PTR DS:[ECX]               ; 取加密字段 1234
	$+3      >  56                 PUSH ESI
	$+4      >  894424 04          MOV DWORD PTR SS:[ESP+0x4],EAX
	$+8      >  BA 01000000        MOV EDX,0x1
	$+D      >  8D7424 04          LEA ESI,DWORD PTR SS:[ESP+0x4]
	$+11     >  33C0               XOR EAX,EAX
	$+13     >  2BD6               SUB EDX,ESI                              ; 1-高8位
	$+15     >  EB 09              JMP SHORT MHOClien.00467370
	$+17     >  8DA424 00000000    LEA ESP,DWORD PTR SS:[ESP]
	$+1E     >  8BFF               MOV EDI,EDI
	$+20     >  66:8B7401 04       MOV SI,WORD PTR DS:[ECX+EAX+0x4]         ; 取 56
	$+25     >  66:317404 04       XOR WORD PTR SS:[ESP+EAX+0x4],SI
	$+2A     >  83C0 02            ADD EAX,0x2
	$+2D     >  8D3402             LEA ESI,DWORD PTR DS:[EDX+EAX]
	$+30     >  8D7434 04          LEA ESI,DWORD PTR SS:[ESP+ESI+0x4]
	$+34     >  83FE 04            CMP ESI,0x4
	$+37     >^ 72 E7              JB SHORT MHOClien.00467370
	$+39     >  5E                 POP ESI
	$+3A     >  83F8 04            CMP EAX,0x4
	$+3D     >  73 0A              JNB SHORT MHOClien.00467399

	518B0156894424??BA????????8D7424??33C02BD6EB

	$-7      00469570     55                  PUSH EBP
	$-6      00469571     8BEC                MOV EBP,ESP
	$-4      00469573     51                  PUSH ECX
	$-3      00469574     8B01                MOV EAX,DWORD PTR DS:[ECX]
	$-1      00469576     56                  PUSH ESI
	$ ==>    00469577     8945 FC             MOV DWORD PTR SS:[EBP-0x4],EAX
	$+3      0046957A     BA 01000000         MOV EDX,0x1
	$+8      0046957F     8D75 FC             LEA ESI,DWORD PTR SS:[EBP-0x4]
	$+B      00469582     33C0                XOR EAX,EAX
	$+D      00469584     2BD6                SUB EDX,ESI
	$+F      00469586     EB 08               JMP SHORT MHOClien.00469590
	$+11     00469588     8DA424 00000000     LEA ESP,DWORD PTR SS:[ESP]
	$+18     0046958F     90                  NOP
	$+19     00469590     66:8B7401 04        MOV SI,WORD PTR DS:[ECX+EAX+0x4]
	$+1E     00469595     66:317405 FC        XOR WORD PTR SS:[EBP+EAX-0x4],SI
	$+23     0046959A     83C0 02             ADD EAX,0x2
	$+26     0046959D     8D3402              LEA ESI,DWORD PTR DS:[EDX+EAX]
	$+29     004695A0     8D7435 FC           LEA ESI,DWORD PTR SS:[EBP+ESI-0x4]
	$+2D     004695A4     83FE 04             CMP ESI,0x4
	$+30     004695A7   ^ 72 E7               JB SHORT MHOClien.00469590

	89????BA????????8D????33??2B??EB??8D
	--*/

	DWORD	dwMMOClientDecryCall = (DWORD)scan_game_.ScanAddr("89????BA????????8D????33??2B??EB??8D", -0x07);
	assert(dwMMOClientDecryCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_DECRY_CALL  " << dwMMOClientDecryCall - GetModuleCryGameBase() << "  // " << dwMMOClientDecryCall << " CryGame.dll" << FC_OFstream_EndlLog;



	/*++
	$ ==>    007C77CE     8B0D B0E00902       MOV ECX,DWORD PTR DS:[0x209E0B0]         ; CryRende.14C938A0
	$+6      007C77D4     50                  PUSH EAX
	$+7      007C77D5     8B4424 4C           MOV EAX,DWORD PTR SS:[ESP+0x4C]
	$+B      007C77D9     8B50 1C             MOV EDX,DWORD PTR DS:[EAX+0x1C]
	$+E      007C77DC     6A 00               PUSH 0x0
	$+10     007C77DE     6A 00               PUSH 0x0
	$+12     007C77E0     FFD2                CALL EDX
	$+14     007C77E2     8986 080A0000       MOV DWORD PTR DS:[ESI+0xA08],EAX         ; eax = 主窗口句柄
	$+1A     007C77E8     8947 04             MOV DWORD PTR DS:[EDI+0x4],EAX
	$+1D     007C77EB     80BE C9040000 00    CMP BYTE PTR DS:[ESI+0x4C9],0x0
	$+24     007C77F2     74 77               JE SHORT MHOClien.007C786B

	8B0D????????508B??????8B????6A??6A??FFD2

	$ ==>    006D6AF4       8B42 1C                 MOV EAX,DWORD PTR DS:[EDX+0x1C]
	$+3      006D6AF7       6A 00                   PUSH 0x0
	$+5      006D6AF9       6A 00                   PUSH 0x0
	$+7      006D6AFB       FFD0                    CALL EAX
	$+9      006D6AFD       8986 080A0000           MOV DWORD PTR DS:[ESI+0xA08],EAX         ; eax = 主窗口句柄
	$+F      006D6B03       8947 04                 MOV DWORD PTR DS:[EDI+0x4],EAX
	$+12     006D6B06       80BE C9040000 00        CMP BYTE PTR DS:[ESI+0x4C9],0x0
	$+19     006D6B0D       74 77                   JE SHORT MHOClien.006D6B86
	$+1B     006D6B0F       8B0D E470FF01           MOV ECX,DWORD PTR DS:[0x1FF70E4]
	$+21     006D6B15       8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+23     006D6B17       8B42 54                 MOV EAX,DWORD PTR DS:[EDX+0x54]
	$+26     006D6B1A       68 3C0BFD01             PUSH MHOClien.01FD0B3C                   ; ASCII "r_Width"
	$+2B     006D6B1F       FFD0                    CALL EAX
	$+2D     006D6B21       8B0D E470FF01           MOV ECX,DWORD PTR DS:[0x1FF70E4]
	$+33     006D6B27       8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+35     006D6B29       8B42 54                 MOV EAX,DWORD PTR DS:[EDX+0x54]
	$+38     006D6B2C       68 3C0BFD01             PUSH MHOClien.01FD0B3C                   ; ASCII "r_Width"

	8B????6A006A00FF??89??????????89????80

	$ ==>    >  FFD2                  CALL EDX
	$+2      >  8B0D 98705F01         MOV ECX,DWORD PTR DS:[0x15F7098]         ; CryRende.1A9E1270
	$+8      >  50                    PUSH EAX
	$+9      >  8B45 F8               MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+C      >  8B50 3C               MOV EDX,DWORD PTR DS:[EAX+0x3C]
	$+F      >  6A 00                 PUSH 0x0
	$+11     >  6A 00                 PUSH 0x0
	$+13     >  FFD2                  CALL EDX
	$+15     >  8B5D 10               MOV EBX,DWORD PTR SS:[EBP+0x10]
	$+18     >  8986 200A0000         MOV DWORD PTR DS:[ESI+0xA20],EAX
	$+1E     >  8943 04               MOV DWORD PTR DS:[EBX+0x4],EAX
	$+21     >  80BE C9040000 00      CMP BYTE PTR DS:[ESI+0x4C9],0x0
	$+28     >  74 77                 JE SHORT MHOClien.00467F5A
	$+2A     >  8B0D B4705F01         MOV ECX,DWORD PTR DS:[0x15F70B4]
	$+30     >  8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+32     >  8B50 54               MOV EDX,DWORD PTR DS:[EAX+0x54]
	$+35     >  68 08BC3901           PUSH MHOClien.0139BC08                   ; ASCII "r_Width"
	$+3A     >  FFD2                  CALL EDX
	$+3C     >  8B0D B4705F01         MOV ECX,DWORD PTR DS:[0x15F70B4]
	$+42     >  8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+44     >  8B50 54               MOV EDX,DWORD PTR DS:[EAX+0x54]
	$+47     >  68 08BC3901           PUSH MHOClien.0139BC08                   ; ASCII "r_Width"
	$+4C     >  FFD2                  CALL EDX

	FF??8B????????????8B????8B????6A006A00FF
	--*/
	DWORD	dwMMOClientCreateWindowHook = (DWORD)scan_exe_.ScanAddr("FF??8B????????????8B????8B????6A006A00FF", 0x15);
	assert(dwMMOClientCreateWindowHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_CREATEWINDOW_HOOK  " << dwMMOClientCreateWindowHook << "  // MHOClient.exe" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00C34F40     56              PUSH ESI
	$+1      00C34F41     8BF1            MOV ESI,ECX
	$+3      00C34F43     8B06            MOV EAX,DWORD PTR DS:[ESI]
	$+5      00C34F45     8B90 98020000   MOV EDX,DWORD PTR DS:[EAX+0x298]
	$+B      00C34F4B     FFD2            CALL EDX
	$+D      00C34F4D     8B8E F4040000   MOV ECX,DWORD PTR DS:[ESI+0x4F4]
	$+13     00C34F53     8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+15     00C34F55     50              PUSH EAX
	$+16     00C34F56     8B42 0C         MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+19     00C34F59     FFD0            CALL EAX
	$+1B     00C34F5B     5E              POP ESI
	$+1C     00C34F5C     C3              RETN

	568BF18B068B90????????FFD28B8E????????8B11
	--*/
	DWORD	dwMMOClientObjOffset3 = *scan_cry_action_.ScanImme<DWORD>("568BF18B068B90????????FFD28B8E????????8B11", 0x0F);
	assert(dwMMOClientObjOffset3);
	assert(dwMMOClientObjOffset3 == 0x000004F4);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET3  " << dwMMOClientObjOffset3 << "  // CryAction.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0070D0FD     32C0            XOR AL,AL
	$+2      0070D0FF     5E              POP ESI
	$+3      0070D100     C2 0400         RETN 0x4
	$+6      0070D103     837E 1C 00      CMP DWORD PTR DS:[ESI+0x1C],0x0
	$+A      0070D107   ^ 74 F4           JE SHORT MHOClien.0070D0FD
	$+C      0070D109     8B4E 1C         MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$+F      0070D10C     8B01            MOV EAX,DWORD PTR DS:[ECX]
	$+11     0070D10E     8B5424 08       MOV EDX,DWORD PTR SS:[ESP+0x8]
	$+15     0070D112     8B40 10         MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+18     0070D115     52              PUSH EDX
	$+19     0070D116     FFD0            CALL EAX
	$+1B     0070D118     85C0            TEST EAX,EAX
	$+1D     0070D11A     0F94C0          SETE AL
	$+20     0070D11D     5E              POP ESI
	$+21     0070D11E     C2 0400         RETN 0x4

	32C0????????837E????74??8B4E??8B018B5424??8B40??52FFD0
	--*/
	// 	DWORD	dwMMOClientSocketOffset5 = ScanImme((HANDLE)-1,"32C0????????837E????74??8B4E??8B018B5424??8B40??52FFD0",0x0E,1);
	// 	ASSERT_GWLR(dwMMOClientSocketOffset5);
	// 	ASSERT_GWLR(dwMMOClientSocketOffset5==0x0000001C);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_SOCKET_OFFSET5  ";
	// 	LOG << dwMMOClientSocketOffset5;
	// 	LOG_END;

	/*++
	$ ==>    >  FFD2            CALL EDX
	$+2      >  3BC3            CMP EAX,EBX
	$+4      >  74 10           JE SHORT MHOClien.0063586C
	$+6      >  0FB74F 24       MOVZX ECX,WORD PTR DS:[EDI+0x24]
	$+A      >  8988 38430000   MOV DWORD PTR DS:[EAX+0x4338],ECX
	$+10     >  8898 40430000   MOV BYTE PTR DS:[EAX+0x4340],BL
	$+16     >  8D5424 18       LEA EDX,DWORD PTR SS:[ESP+0x18]
	$+1A     >  52              PUSH EDX
	$+1B     >  885C24 1D       MOV BYTE PTR SS:[ESP+0x1D],BL
	$+1F     >  885C24 1C       MOV BYTE PTR SS:[ESP+0x1C],BL
	$+23     >  E8 E2BB0000     CALL MHOClien.00641460

	FF??3B??74??0F??????89??????????88

	$ ==>    0D10C0E4       0FB74E 24            MOVZX ECX,WORD PTR DS:[ESI+0x24]
	$+4      0D10C0E8       8988 6C420000        MOV DWORD PTR DS:[EAX+0x426C],ECX        ; 写入线路序号
	$+A      0D10C0EE       8898 70420000        MOV BYTE PTR DS:[EAX+0x4270],BL
	$+10     0D10C0F4       8BCF                 MOV ECX,EDI
	$+12     0D10C0F6       885D 8D              MOV BYTE PTR SS:[EBP-0x73],BL
	$+15     0D10C0F9       885D 8C              MOV BYTE PTR SS:[EBP-0x74],BL
	$+18     0D10C0FC       E8 0FABF4FF          CALL CryGame.0D056C10
	$+1D     0D10C101       85C0                 TEST EAX,EAX
	$+1F     0D10C103       74 16                JE SHORT CryGame.0D10C11B
	$+21     0D10C105       8BCF                 MOV ECX,EDI
	$+23     0D10C107       E8 04ABF4FF          CALL CryGame.0D056C10

	0F??????89??????????88??????????8B??88

	$-12     >  E8 2798F4FF       CALL CryGame.0D079A60
	$-D      >  8B10              MOV EDX,DWORD PTR DS:[EAX]
	$-B      >  8BC8              MOV ECX,EAX
	$-9      >  8B42 24           MOV EAX,DWORD PTR DS:[EDX+0x24]
	$-6      >  FFD0              CALL EAX
	$-4      >  33DB              XOR EBX,EBX
	$-2      >  3BC3              CMP EAX,EBX
	$ ==>    >  74 10             JE SHORT CryGame.0D130258
	$+2      >  0FB74F 24         MOVZX ECX,WORD PTR DS:[EDI+0x24]
	$+6      >  8988 18430000     MOV DWORD PTR DS:[EAX+0x4318],ECX
	$+C      >  8898 20430000     MOV BYTE PTR DS:[EAX+0x4320],BL
	$+12     >  8BCE              MOV ECX,ESI
	$+14     >  889D 65FFFFFF     MOV BYTE PTR SS:[EBP-0x9B],BL
	$+1A     >  889D 64FFFFFF     MOV BYTE PTR SS:[EBP-0x9C],BL
	$+20     >  E8 B598F4FF       CALL CryGame.0D079B20
	$+25     >  85C0              TEST EAX,EAX
	$+27     >  74 19             JE SHORT CryGame.0D130288
	$+29     >  8BCE              MOV ECX,ESI
	$+2B     >  E8 AA98F4FF       CALL CryGame.0D079B20

	//zhp
	$-2E     >  52              PUSH EDX
	$-2D     >  8D45 B4         LEA EAX,DWORD PTR SS:[EBP-0x4C]
	$-2A     >  50              PUSH EAX
	$-29     >  E8 6CB55AFF     CALL CryGame.0A7DB770
	$-24     >  83C4 14         ADD ESP,0x14
	$-21     >  8D4D 90         LEA ECX,DWORD PTR SS:[EBP-0x70]
	$-1E     >  51              PUSH ECX
	$-1D     >  8BCE            MOV ECX,ESI
	$-1B     >  E8 FEE4FEFF     CALL CryGame.0B21E710
	$-16     >  8BCE            MOV ECX,ESI
	$-14     >  E8 97E0F2FF     CALL CryGame.0B15E2B0
	$-F      >  8B10            MOV EDX,DWORD PTR DS:[EAX]
	$-D      >  8BC8            MOV ECX,EAX
	$-B      >  8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	$-8      >  FFD0            CALL EAX
	$-6      >  33DB            XOR EBX,EBX
	$-4      >  3BC3            CMP EAX,EBX
	$-2      >  74 10           JE SHORT CryGame.0B230238
	$ ==>    >  0FB74F 24       MOVZX ECX,WORD PTR DS:[EDI+0x24]
	$+4      >  8988 984B0000   MOV DWORD PTR DS:[EAX+0x4B98],ECX
	$+A      >  8898 A04B0000   MOV BYTE PTR DS:[EAX+0x4BA0],BL
	$+10     >  8BCE            MOV ECX,ESI
	$+12     >  889D 65FFFFFF   MOV BYTE PTR SS:[EBP-0x9B],BL
	$+18     >  889D 64FFFFFF   MOV BYTE PTR SS:[EBP-0x9C],BL
	$+1E     >  E8 25E1F2FF     CALL CryGame.0B15E370
	$+23     >  85C0            TEST EAX,EAX
	$+25     >  74 19           JE SHORT CryGame.0B230268
	$+27     >  8BCE            MOV ECX,ESI
	$+29     >  E8 1AE1F2FF     CALL CryGame.0B15E370
	$+2E     >  8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+30     >  8B52 04         MOV EDX,DWORD PTR DS:[EDX+0x4]
	$+33     >  8D8D 64FFFFFF   LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+39     >  51              PUSH ECX
	$+3A     >  6A 66           PUSH 0x66
	$+3C     >  8BC8            MOV ECX,EAX
	$+3E     >  FFD2            CALL EDX
	$+40     >  8BCE            MOV ECX,ESI
	$+42     >  899D 10FFFFFF   MOV DWORD PTR SS:[EBP-0xF0],EBX
	$+48     >  899D 14FFFFFF   MOV DWORD PTR SS:[EBP-0xEC],EBX
	$+4E     >  899D 18FFFFFF   MOV DWORD PTR SS:[EBP-0xE8],EBX
	$+54     >  889D 0CFFFFFF   MOV BYTE PTR SS:[EBP-0xF4],BL
	$+5A     >  E8 E9E0F2FF     CALL CryGame.0B15E370
	$+5F     >  85C0            TEST EAX,EAX
	$+61     >  74 1C           JE SHORT CryGame.0B2302A7

	0F??????89??????????88??????????8B??88
	--*/
	DWORD	dwMMOClientActorOffsetXianLu = *scan_game_.ScanImme<DWORD>("0F??????89??????????88??????????8B??88", 0x6);
	assert(dwMMOClientActorOffsetXianLu);
	//assert(dwMMOClientActorOffsetXianLu==0x4e08/*0x4B98*//*0x0000463C*//*0x000045DC*//*0x000045CC*//*0x000045B8*//*0x00004350*//*0x0000427C*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET_XIAN_LU  " << dwMMOClientActorOffsetXianLu << "  // " << " CryGame.dll" << FC_OFstream_EndlLog;



	/*++
	$ ==>    004680E0     8B4424 04      MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+4      004680E4     55             PUSH EBP
	$+5      004680E5     56             PUSH ESI
	$+6      004680E6     8BF1           MOV ESI,ECX
	$+8      004680E8     8B08           MOV ECX,DWORD PTR DS:[EAX]
	$+A      004680EA     57             PUSH EDI
	$+B      004680EB     8D6E 04        LEA EBP,DWORD PTR DS:[ESI+0x4]
	$+E      004680EE     890E           MOV DWORD PTR DS:[ESI],ECX
	$+10     004680F0     33FF           XOR EDI,EDI
	$+12     004680F2     E8 89C28801    CALL MHOClien.01CF4380                   ; JMP 到 msvcr90.rand
	$+17     004680F7     66:89042F      MOV WORD PTR DS:[EDI+EBP],AX
	$+1B     004680FB     83C7 02        ADD EDI,0x2
	$+1E     004680FE     83FF 03        CMP EDI,0x3

	8B4424??55568BF18B08578D6E??890E33FFE8

	$-3D     004695F0     55              PUSH EBP                                 ; 0611更新后的特征码函数
	$-3C     004695F1     8BEC            MOV EBP,ESP
	$-3A     004695F3     8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-37     004695F6     56              PUSH ESI
	$-36     004695F7     8BF1            MOV ESI,ECX
	$-34     004695F9     8B08            MOV ECX,DWORD PTR DS:[EAX]
	$-32     004695FB     57              PUSH EDI
	$-31     004695FC     890E            MOV DWORD PTR DS:[ESI],ECX
	$-2F     004695FE     33FF            XOR EDI,EDI
	$-2D     00469600     E8 3B376F01     CALL MHOClien.01B5CD40                   ; JMP 到 msvcr90.rand
	$-28     00469605     66:89443E 04    MOV WORD PTR DS:[ESI+EDI+0x4],AX
	$-23     0046960A     83C7 02         ADD EDI,0x2
	$-20     0046960D     83FF 03         CMP EDI,0x3
	$-1D     00469610   ^ 72 EE           JB SHORT MHOClien.00469600
	$-1B     00469612     83FF 04         CMP EDI,0x4
	$-18     00469615     73 09           JNB SHORT MHOClien.00469620
	$-16     00469617     E8 24376F01     CALL MHOClien.01B5CD40                   ; JMP 到 msvcr90.rand
	$-11     0046961C     884437 04       MOV BYTE PTR DS:[EDI+ESI+0x4],AL
	$-D      00469620     B9 03000000     MOV ECX,0x3
	$-8      00469625     33C0            XOR EAX,EAX
	$-6      00469627     53              PUSH EBX
	$-5      00469628     83F9 04         CMP ECX,0x4
	$-2      0046962B     73 17           JNB SHORT MHOClien.00469644
	$ ==>    0046962D     8A1C31          MOV BL,BYTE PTR DS:[ECX+ESI]
	$+3      00469630     8A1430          MOV DL,BYTE PTR DS:[EAX+ESI]
	$+6      00469633     881C30          MOV BYTE PTR DS:[EAX+ESI],BL
	$+9      00469636     881431          MOV BYTE PTR DS:[ECX+ESI],DL
	$+C      00469639     83C0 02         ADD EAX,0x2
	$+F      0046963C     83E9 02         SUB ECX,0x2
	$+12     0046963F     83F8 04         CMP EAX,0x4
	$+15     00469642   ^ 72 E4           JB SHORT MHOClien.00469628
	$+17     00469644     8D56 04         LEA EDX,DWORD PTR DS:[ESI+0x4]
	$+1A     00469647     BF 01000000     MOV EDI,0x1
	$+1F     0046964C     33C9            XOR ECX,ECX
	$+21     0046964E     2BD6            SUB EDX,ESI
	$+23     00469650     8BC6            MOV EAX,ESI
	$+25     00469652     2BFE            SUB EDI,ESI

	8A????8A????88????88????83

	$-2C     >  55              PUSH EBP
	$-2B     >  8BEC            MOV EBP,ESP
	$-29     >  8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-26     >  56              PUSH ESI
	$-25     >  8BF1            MOV ESI,ECX
	$-23     >  8B08            MOV ECX,DWORD PTR DS:[EAX]
	$-21     >  57              PUSH EDI
	$-20     >  890E            MOV DWORD PTR DS:[ESI],ECX
	$-1E     >  33FF            XOR EDI,EDI
	$-1C     >  E8 FB7C5B01     CALL MHOClien.01B5DD50                   ; JMP 到 msvcr90.rand
	$-17     >  66:89443E 04    MOV WORD PTR DS:[ESI+EDI+0x4],AX
	$-12     >  83C7 02         ADD EDI,0x2
	$-F      >  83FF 03         CMP EDI,0x3
	$-C      >^ 72 EE           JB SHORT MHOClien.005A6050
	$-A      >  83FF 04         CMP EDI,0x4
	$-7      >  73 09           JNB SHORT MHOClien.005A6070
	$-5      >  E8 E47C5B01     CALL MHOClien.01B5DD50                   ; JMP 到 msvcr90.rand
	$ ==>    >  884437 04       MOV BYTE PTR DS:[EDI+ESI+0x4],AL
	$+4      >  B9 03000000     MOV ECX,0x3
	$+9      >  33C0            XOR EAX,EAX
	$+B      >  53              PUSH EBX
	$+C      >  83F9 04         CMP ECX,0x4
	$+F      >  73 17           JNB SHORT MHOClien.005A6094
	$+11     >  8A1C31          MOV BL,BYTE PTR DS:[ECX+ESI]             ; 1
	$+14     >  8A1430          MOV DL,BYTE PTR DS:[EAX+ESI]
	$+17     >  881C30          MOV BYTE PTR DS:[EAX+ESI],BL
	$+1A     >  881431          MOV BYTE PTR DS:[ECX+ESI],DL
	$+1D     >  83C0 02         ADD EAX,0x2
	$+20     >  83E9 02         SUB ECX,0x2
	$+23     >  83F8 04         CMP EAX,0x4
	$+26     >^ 72 E4           JB SHORT MHOClien.005A6078

	88??????B9????????33????83????73??8A????8A????88????88????83

	--*/

	DWORD	dwMMOClientEncryption = (DWORD)scan_game_.ScanAddr("88??????B9????????33????83????73??8A????8A????88????88????83", -0x2C);

	assert(dwMMOClientEncryption);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ENCRYPTION_CALL  " << dwMMOClientEncryption - GetModuleCryGameBase() << "  // " << dwMMOClientEncryption << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-19     0C90A457       8B15 F08EAC0D        MOV EDX,DWORD PTR DS:[0xDAC8EF0]         ; 基址
	$-13     0C90A45D       8B82 C8000000        MOV EAX,DWORD PTR DS:[EDX+0xC8]
	$-D      0C90A463       8B48 1C              MOV ECX,DWORD PTR DS:[EAX+0x1C]
	$-A      0C90A466       8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$-8      0C90A468       8B82 A8000000        MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$-2      0C90A46E       FFD0                 CALL EAX                                 ; MOV EAX,DWORD PTR DS:[ECX+0x214]
	$ ==>    0C90A470       8BF0                 MOV ESI,EAX
	$+2      0C90A472       85F6                 TEST ESI,ESI
	$+4      0C90A474       0F84 13020000        JE CryGame.0C90A68D
	$+A      0C90A47A       8B5424 1C            MOV EDX,DWORD PTR SS:[ESP+0x1C]
	$+E      0C90A47E       8B8E 88420000        MOV ECX,DWORD PTR DS:[ESI+0x4288]
	$+14     0C90A484       57                   PUSH EDI
	$+15     0C90A485       52                   PUSH EDX
	$+16     0C90A486       E8 B502C700          CALL CryGame.0D57A740
	$+1B     0C90A48B       894424 20            MOV DWORD PTR SS:[ESP+0x20],EAX
	$+1F     0C90A48F       85C0                 TEST EAX,EAX
	$+21     0C90A491       0F84 F6010000        JE CryGame.0C90A68D

	8B??85??0F84????????8B??????8B??????????????E8
	--*/
	DWORD	dwMMOClientTraverseGeZiOffset1 = *scan_game_.ScanImme<DWORD>("8B??85??0F84????????8B??????8B??????????????E8", 0x10);
	assert(dwMMOClientTraverseGeZiOffset1);
	//assert(dwMMOClientTraverseGeZiOffset1==0x4BB8/*0x00004658*//*0x000045F8*//*0x000045E8*//*0x000045D8*//*0x00004370*/  /*0x00004298*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_OFFSET1  " << dwMMOClientTraverseGeZiOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    01C48C5B      8BCE            MOV ECX,ESI
	$+2      01C48C5D      E8 1E2899FE     CALL MHOClien.005DB480
	$+7      01C48C62      50              PUSH EAX                                 ; 模板对象地址
	$+8      01C48C63      E8 88F7FFFF     CALL MHOClien.01C483F0
	$+D      01C48C68      8BF0            MOV ESI,EAX                              ; 返回值为1，则可出售，返回值为0，则不可出售
	$+F      01C48C6A      83C4 08         ADD ESP,0x8
	$+12     01C48C6D      85F6            TEST ESI,ESI
	$+14     01C48C6F      75 09           JNZ SHORT MHOClien.01C48C7A
	$+16     01C48C71      8D46 08         LEA EAX,DWORD PTR DS:[ESI+0x8]
	$+19     01C48C74      5E              POP ESI
	$+1A     01C48C75      5F              POP EDI
	$+1B     01C48C76      5B              POP EBX
	$+1C     01C48C77      C2 0C00         RETN 0xC
	$+1F     01C48C7A      8B4B 2C         MOV ECX,DWORD PTR DS:[EBX+0x2C]

	8B??E8??????????E8????????8B??83????85??75??8D
	--*/
	DWORD	dwMMOClientGetSellItemPriceCall = (DWORD)scan_game_.ScanCall("8B??E8??????????E8????????8B??83????85??75??8D", 9);
	assert(dwMMOClientGetSellItemPriceCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_SELL_ITEM_PRICE_CALL  " << dwMMOClientGetSellItemPriceCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetSellItemPriceCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-20     005F6AC0     55             PUSH EBP                                 ; 怪物血值访问函数
	$-1F     005F6AC1     8BEC           MOV EBP,ESP
	$-1D     005F6AC3     83E4 F8        AND ESP,0xFFFFFFF8
	$-1A     005F6AC6     83EC 24        SUB ESP,0x24
	$-17     005F6AC9     53             PUSH EBX
	$-16     005F6ACA     56             PUSH ESI
	$-15     005F6ACB     8BF1           MOV ESI,ECX
	$-13     005F6ACD     8B4E 04        MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-10     005F6AD0     57             PUSH EDI
	$-F      005F6AD1     8B7D 08        MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-C      005F6AD4     8B1F           MOV EBX,DWORD PTR DS:[EDI]
	$-A      005F6AD6     81C1 AE090000  ADD ECX,0x9AE
	$-4      005F6ADC     895C24 0C      MOV DWORD PTR SS:[ESP+0xC],EBX
	$ ==>    005F6AE0     E8 7B15E7FF    CALL MHOClien.00468060
	$+5      005F6AE5     83F8 04        CMP EAX,0x4
	$+8      005F6AE8     0F84 E1010000  JE MHOClien.005F6CCF
	$+E      005F6AEE     8B4E 04        MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+11     005F6AF1     8B01           MOV EAX,DWORD PTR DS:[ECX]
	$+13     005F6AF3     8B90 70010000  MOV EDX,DWORD PTR DS:[EAX+0x170]
	$+19     005F6AF9     FFD2           CALL EDX
	$+1B     005F6AFB     84C0           TEST AL,AL
	$+1D     005F6AFD     74 09          JE SHORT MHOClien.005F6B08

	E8????????83F8??0F84????????8B4E??8B018B90????????FFD2

	$ ==>    01C5BC90      837C24 08 00         CMP DWORD PTR SS:[ESP+0x8],0x0           ; 怪物血值
	$+5      01C5BC95      56                   PUSH ESI
	$+6      01C5BC96      57                   PUSH EDI
	$+7      01C5BC97      8BF9                 MOV EDI,ECX
	$+9      01C5BC99      75 45                JNZ SHORT MHOClien.01C5BCE0
	$+B      01C5BC9B      8B4424 0C            MOV EAX,DWORD PTR SS:[ESP+0xC]
	$+F      01C5BC9F      8D77 44              LEA ESI,DWORD PTR DS:[EDI+0x44]
	$+12     01C5BCA2      50                   PUSH EAX
	$+13     01C5BCA3      8BCE                 MOV ECX,ESI
	$+15     01C5BCA5      E8 06E980FE          CALL MHOClien.0046A5B0
	$+1A     01C5BCAA      807C24 14 00         CMP BYTE PTR SS:[ESP+0x14],0x0
	$+1F     01C5BCAF      75 2F                JNZ SHORT MHOClien.01C5BCE0
	$+21     01C5BCB1      8BCE                 MOV ECX,ESI
	$+23     01C5BCB3      E8 78E880FE          CALL MHOClien.0046A530

	83????????????8B??75??8B??????8D??????8B??E8

	$-21     01BC7FE0     55                  PUSH EBP
	$-20     01BC7FE1     8BEC                MOV EBP,ESP
	$-1E     01BC7FE3     837D 0C 00          CMP DWORD PTR SS:[EBP+0xC],0x0
	$-1A     01BC7FE7     56                  PUSH ESI
	$-19     01BC7FE8     57                  PUSH EDI
	$-18     01BC7FE9     8BF9                MOV EDI,ECX
	$-16     01BC7FEB     75 43               JNZ SHORT MHOClien.01BC8030
	$-14     01BC7FED     8B45 08             MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-11     01BC7FF0     8D77 0C             LEA ESI,DWORD PTR DS:[EDI+0xC]
	$-E      01BC7FF3     50                  PUSH EAX
	$-D      01BC7FF4     8BCE                MOV ECX,ESI
	$-B      01BC7FF6     E8 F5158AFE         CALL MHOClien.004695F0
	$-6      01BC7FFB     807D 10 00          CMP BYTE PTR SS:[EBP+0x10],0x0
	$-2      01BC7FFF     75 2F               JNZ SHORT MHOClien.01BC8030
	$ ==>    01BC8001     8BCE                MOV ECX,ESI
	$+2      01BC8003     E8 68158AFE         CALL MHOClien.00469570
	$+7      01BC8008     83EC 20             SUB ESP,0x20
	$+A      01BC800B     8BD4                MOV EDX,ESP
	$+C      01BC800D     8D4A 04             LEA ECX,DWORD PTR DS:[EDX+0x4]
	$+F      01BC8010     8949 10             MOV DWORD PTR DS:[ECX+0x10],ECX
	$+12     01BC8013     8949 14             MOV DWORD PTR DS:[ECX+0x14],ECX
	$+15     01BC8016     8B49 10             MOV ECX,DWORD PTR DS:[ECX+0x10]
	$+18     01BC8019     C601 00             MOV BYTE PTR DS:[ECX],0x0
	$+1B     01BC801C     6A 00               PUSH 0x0
	$+1D     01BC801E     6A 01               PUSH 0x1

	8B??E8????????83????8B??8D????89????89????8B

	$-8      >  55              PUSH EBP                                 ; 新的hook血值的点，目前看来有效
	$-7      >  8BEC            MOV EBP,ESP
	$-5      >  83EC 20         SUB ESP,0x20
	$-2      >  56              PUSH ESI
	$-1      >  57              PUSH EDI
	$ ==>    >  8B7D 08         MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+3      >  8B07            MOV EAX,DWORD PTR DS:[EDI]
	$+5      >  8BF1            MOV ESI,ECX
	$+7      >  8B4E 04         MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+A      >  8945 08         MOV DWORD PTR SS:[EBP+0x8],EAX
	$+D      >  E8 76D0FFFF     CALL MHOClien.01BA13C0
	$+12     >  84C0            TEST AL,AL
	$+14     >  0F85 A1010000   JNZ MHOClien.01BA44F3
	$+1A     >  8B4E 04         MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+1D     >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+1F     >  8B82 70010000   MOV EAX,DWORD PTR DS:[EDX+0x170]
	$+25     >  FFD0            CALL EAX
	$+27     >  84C0            TEST AL,AL
	$+29     >  74 09           JE SHORT MHOClien.01BA436C
	$+2B     >  833F 00         CMP DWORD PTR DS:[EDI],0x0
	$+2E     >  0F8F 87010000   JG MHOClien.01BA44F3
	$+34     >  53              PUSH EBX
	$+35     >  83CB FF         OR EBX,0xFFFFFFFF
	$+38     >  837D 08 00      CMP DWORD PTR SS:[EBP+0x8],0x0
	$+3C     >  7F 4C           JG SHORT MHOClien.01BA43C2

	8B????8B??8B??8B????89????E8
	--*/
	DWORD	dwMMOClientMonsterHpCallHook = (DWORD)scan_game_.ScanAddr("8B????8B??8B??8B????89????E8", -0x8);

	assert(dwMMOClientMonsterHpCallHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK  " << dwMMOClientMonsterHpCallHook - GetModuleCryGameBase() << "  // " << dwMMOClientMonsterHpCallHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00621A7C     6A 00          PUSH 0x0
	$+2      00621A7E     51             PUSH ECX
	$+3      00621A7F     8B48 14        MOV ECX,DWORD PTR DS:[EAX+0x14]
	$+6      00621A82     034C24 0C      ADD ECX,DWORD PTR SS:[ESP+0xC]
	$+A      00621A86     8B40 10        MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+D      00621A89     895424 10      MOV DWORD PTR SS:[ESP+0x10],EDX
	$+11     00621A8D     8D5424 10      LEA EDX,DWORD PTR SS:[ESP+0x10]
	$+15     00621A91     52             PUSH EDX
	$+16     00621A92     FFD0           CALL EAX                                 ; 初始化和修改怪物血值
	$+18     00621A94     C2 0C00        RETN 0xC

	6A??518B48??034C24??8B40??895424??8D5424??52FFD0
	--*/
	// 	DWORD	dwMMOClientMonsterHpCallCmpAddr = ScanAddr((HANDLE)-1,"6A??518B48??034C24??8B40??895424??8D5424??52FFD0",0x18,4,2);
	// 	ASSERT_GWLR(dwMMOClientMonsterHpCallCmpAddr);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HP_CALL_CMP_ADDR  ";
	// 	LOG << dwMMOClientMonsterHpCallCmpAddr;
	// 	LOG_END;

	/*++
	$ ==>    005D6B60      8B4F 04        MOV ECX,DWORD PTR DS:[EDI+0x4]           ; 玩家血值访问函数  访问edi
	$+3      005D6B63      8B01           MOV EAX,DWORD PTR DS:[ECX]
	$+5      005D6B65      8B90 70010000  MOV EDX,DWORD PTR DS:[EAX+0x170]
	$+B      005D6B6B      83EC 24        SUB ESP,0x24
	$+E      005D6B6E      FFD2           CALL EDX
	$+10     005D6B70      84C0           TEST AL,AL
	$+12     005D6B72      0F85 38020000  JNZ MHOClien.005D6DB0
	$+18     005D6B78      53             PUSH EBX
	$+19     005D6B79      56             PUSH ESI
	$+1A     005D6B7A      8B7424 30      MOV ESI,DWORD PTR SS:[ESP+0x30]
	$+1E     005D6B7E      8B06           MOV EAX,DWORD PTR DS:[ESI]
	$+20     005D6B80      8BD8           MOV EBX,EAX
	$+22     005D6B82      895C24 08      MOV DWORD PTR SS:[ESP+0x8],EBX
	$+26     005D6B86      85C0           TEST EAX,EAX

	8B4F??8B018B90????????83EC??FFD284C00F85????????5356

	$-34     01C1E3C0      83EC 24              SUB ESP,0x24                             ; 玩家虚弱血值增减	第一个
	$-31     01C1E3C3      56                   PUSH ESI
	$-30     01C1E3C4      8BF1                 MOV ESI,ECX
	$-2E     01C1E3C6      8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-2B     01C1E3C9      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$-29     01C1E3CB      8B90 70010000        MOV EDX,DWORD PTR DS:[EAX+0x170]
	$-23     01C1E3D1      FFD2                 CALL EDX
	$-21     01C1E3D3      84C0                 TEST AL,AL
	$-1F     01C1E3D5      0F85 BB010000        JNZ MHOClien.01C1E596
	$-19     01C1E3DB      55                   PUSH EBP
	$-18     01C1E3DC      8B6C24 30            MOV EBP,DWORD PTR SS:[ESP+0x30]
	$-14     01C1E3E0      8B45 00              MOV EAX,DWORD PTR SS:[EBP]
	$-11     01C1E3E3      894424 08            MOV DWORD PTR SS:[ESP+0x8],EAX
	$-D      01C1E3E7      85C0                 TEST EAX,EAX
	$-B      01C1E3E9      7D 08                JGE SHORT MHOClien.01C1E3F3
	$-9      01C1E3EB      C74424 08 00000000   MOV DWORD PTR SS:[ESP+0x8],0x0
	$-1      01C1E3F3      53                   PUSH EBX
	$ ==>    01C1E3F4      8D9E B4000000        LEA EBX,DWORD PTR DS:[ESI+0xB4]
	$+6      01C1E3FA      8BCB                 MOV ECX,EBX
	$+8      01C1E3FC      E8 2FB1A4FE          CALL MHOClien.00669530
	$+D      01C1E401      394424 0C            CMP DWORD PTR SS:[ESP+0xC],EAX
	$+11     01C1E405      7E 0B                JLE SHORT MHOClien.01C1E412
	$+13     01C1E407      8BCB                 MOV ECX,EBX
	$+15     01C1E409      E8 22B1A4FE          CALL MHOClien.00669530
	$+1A     01C1E40E      894424 0C            MOV DWORD PTR SS:[ESP+0xC],EAX
	$+1E     01C1E412      8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+21     01C1E415      6A 01                PUSH 0x1
	$+23     01C1E417      E8 C49C0200          CALL MHOClien.01C480E0
	$+28     01C1E41C      84C0                 TEST AL,AL
	$+2A     01C1E41E      0F85 70010000        JNZ MHOClien.01C1E594
	$+30     01C1E424      57                   PUSH EDI
	$+31     01C1E425      8DBE AC000000        LEA EDI,DWORD PTR DS:[ESI+0xAC]
	$+37     01C1E42B      8BCF                 MOV ECX,EDI
	$+39     01C1E42D      E8 FEC084FE          CALL MHOClien.0046A530

	8D??????????8B??E8????????39??????7E??8B??E8????????89??????8B????6A

	$-32     01B8AF10       55                      PUSH EBP
	$-31     01B8AF11       8BEC                    MOV EBP,ESP
	$-2F     01B8AF13       83EC 24                 SUB ESP,0x24
	$-2C     01B8AF16       56                      PUSH ESI
	$-2B     01B8AF17       8BF1                    MOV ESI,ECX
	$-29     01B8AF19       8B4E 04                 MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-26     01B8AF1C       8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-24     01B8AF1E       8B90 70010000           MOV EDX,DWORD PTR DS:[EAX+0x170]
	$-1E     01B8AF24       FFD2                    CALL EDX
	$-1C     01B8AF26       84C0                    TEST AL,AL
	$-1A     01B8AF28       0F85 A2010000           JNZ MHOClien.01B8B0D0
	$-14     01B8AF2E       8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-11     01B8AF31       8B00                    MOV EAX,DWORD PTR DS:[EAX]
	$-F      01B8AF33       8945 FC                 MOV DWORD PTR SS:[EBP-0x4],EAX
	$-C      01B8AF36       85C0                    TEST EAX,EAX
	$-A      01B8AF38       7D 07                   JGE SHORT MHOClien.01B8AF41
	$-8      01B8AF3A       C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
	$-1      01B8AF41       53                      PUSH EBX
	$ ==>    01B8AF42       8D9E B4000000           LEA EBX,DWORD PTR DS:[ESI+0xB4]
	$+6      01B8AF48       8BCB                    MOV ECX,EBX
	$+8      01B8AF4A       E8 71E5ADFE             CALL MHOClien.006694C0
	$+D      01B8AF4F       3945 FC                 CMP DWORD PTR SS:[EBP-0x4],EAX
	$+10     01B8AF52       7E 0A                   JLE SHORT MHOClien.01B8AF5E
	$+12     01B8AF54       8BCB                    MOV ECX,EBX
	$+14     01B8AF56       E8 65E5ADFE             CALL MHOClien.006694C0
	$+19     01B8AF5B       8945 FC                 MOV DWORD PTR SS:[EBP-0x4],EAX
	$+1C     01B8AF5E       8B4E 04                 MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+1F     01B8AF61       6A 01                   PUSH 0x1
	$+21     01B8AF63       E8 88940200             CALL MHOClien.01BB43F0
	$+26     01B8AF68       84C0                    TEST AL,AL
	$+28     01B8AF6A       0F85 5F010000           JNZ MHOClien.01B8B0CF
	$+2E     01B8AF70       57                      PUSH EDI
	$+2F     01B8AF71       8DBE AC000000           LEA EDI,DWORD PTR DS:[ESI+0xAC]
	$+35     01B8AF77       8BCF                    MOV ECX,EDI
	$+37     01B8AF79       E8 F2E58DFE             CALL MHOClien.00469570

	8D??????????8B??E8????????39????7E??8B??E8????????89????8B????6A

	//zhp 2016/5/24 14:03
	$-3      0C1CC610      55                 PUSH EBP
	$-2      0C1CC611      8BEC               MOV EBP, ESP
	$ ==>    0C1CC613      83EC 10            SUB ESP, 10
	$+3      0C1CC616      56                 PUSH ESI
	$+4      0C1CC617      8BF1               MOV ESI, ECX                             ; ntdll.77C80130
	$+6      0C1CC619      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+9      0C1CC61C      8B01               MOV EAX, DWORD PTR DS:[ECX]
	$+B      0C1CC61E      8B90 78010000      MOV EDX, DWORD PTR DS:[EAX+178]
	$+11     0C1CC624      FFD2               CALL EDX
	$+13     0C1CC626      84C0               TEST AL, AL
	$+15     0C1CC628      0F85 73010000      JNZ CryGame.0C1CC7A1
	$+1B     0C1CC62E      8B45 08            MOV EAX, DWORD PTR SS:[EBP+8]
	$+1E     0C1CC631      8B00               MOV EAX, DWORD PTR DS:[EAX]
	$+20     0C1CC633      8945 FC            MOV DWORD PTR SS:[EBP-4], EAX
	$+23     0C1CC636      85C0               TEST EAX, EAX
	$+25     0C1CC638      7D 07              JGE SHORT CryGame.0C1CC641
	$+27     0C1CC63A      C745 FC 00000000   MOV DWORD PTR SS:[EBP-4], 0
	$+2E     0C1CC641      8BCE               MOV ECX, ESI
	$+30     0C1CC643      E8 E8DFFAFF        CALL CryGame.0C17A630
	$+35     0C1CC648      3945 FC            CMP DWORD PTR SS:[EBP-4], EAX
	$+38     0C1CC64B      7E 0A              JLE SHORT CryGame.0C1CC657
	$+3A     0C1CC64D      8BCE               MOV ECX, ESI
	$+3C     0C1CC64F      E8 DCDFFAFF        CALL CryGame.0C17A630
	$+41     0C1CC654      8945 FC            MOV DWORD PTR SS:[EBP-4], EAX
	$+44     0C1CC657      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+47     0C1CC65A      6A 01              PUSH 1
	$+49     0C1CC65C      E8 3F93FEFF        CALL CryGame.0C1B59A0
	$+4E     0C1CC661      84C0               TEST AL, AL
	$+50     0C1CC663      74 14              JE SHORT CryGame.0C1CC679
	$+52     0C1CC665      8D8E AC000000      LEA ECX, DWORD PTR DS:[ESI+AC]
	$+58     0C1CC66B      E8 60A4E3FE        CALL CryGame.0B006AD0
	$+5D     0C1CC670      3945 FC            CMP DWORD PTR SS:[EBP-4], EAX
	$+60     0C1CC673      0F8C 28010000      JL CryGame.0C1CC7A1
	$+66     0C1CC679      53                 PUSH EBX
	$+67     0C1CC67A      57                 PUSH EDI
	$+68     0C1CC67B      8DBE AC000000      LEA EDI, DWORD PTR DS:[ESI+AC]
	$+6E     0C1CC681      8BCF               MOV ECX, EDI
	$+70     0C1CC683      E8 48A4E3FE        CALL CryGame.0B006AD0
	$+75     0C1CC688      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+78     0C1CC68B      DB45 08            FILD DWORD PTR SS:[EBP+8]
	$+7B     0C1CC68E      8BCE               MOV ECX, ESI
	$+7D     0C1CC690      D95D F8            FSTP DWORD PTR SS:[EBP-8]
	$+80     0C1CC693      E8 98DFFAFF        CALL CryGame.0C17A630
	$+85     0C1CC698      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+88     0C1CC69B      DB45 08            FILD DWORD PTR SS:[EBP+8]
	$+8B     0C1CC69E      D87D F8            FDIVR DWORD PTR SS:[EBP-8]
	$+8E     0C1CC6A1      DC1D 4005640C      FCOMP QWORD PTR DS:[C640540]
	$+94     0C1CC6A7      DFE0               FSTSW AX
	$+96     0C1CC6A9      F6C4 41            TEST AH, 41
	$+99     0C1CC6AC      75 56              JNZ SHORT CryGame.0C1CC704
	$+9B     0C1CC6AE      DB45 FC            FILD DWORD PTR SS:[EBP-4]
	$+9E     0C1CC6B1      8BCE               MOV ECX, ESI
	$+A0     0C1CC6B3      DD5D F4            FSTP QWORD PTR SS:[EBP-C]
	$+A3     0C1CC6B6      E8 75DFFAFF        CALL CryGame.0C17A630
	$+A8     0C1CC6BB      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+AB     0C1CC6BE      DB45 08            FILD DWORD PTR SS:[EBP+8]
	$+AE     0C1CC6C1      DC7D F4            FDIVR QWORD PTR SS:[EBP-C]
	$+B1     0C1CC6C4      DC1D 4005640C      FCOMP QWORD PTR DS:[C640540]
	$+B7     0C1CC6CA      DFE0               FSTSW AX
	$+B9     0C1CC6CC      F6C4 41            TEST AH, 41
	$+BC     0C1CC6CF      7A 33              JPE SHORT CryGame.0C1CC704
	$+BE     0C1CC6D1      8B46 04            MOV EAX, DWORD PTR DS:[ESI+4]
	$+C1     0C1CC6D4      8B48 04            MOV ECX, DWORD PTR DS:[EAX+4]
	$+C4     0C1CC6D7      C745 F0 09040000   MOV DWORD PTR SS:[EBP-10], 409
	$+CB     0C1CC6DE      894D F4            MOV DWORD PTR SS:[EBP-C], ECX            ; ntdll.77C80130
	$+CE     0C1CC6E1      C745 F8 FFFFFFFF   MOV DWORD PTR SS:[EBP-8], -1
	$+D5     0C1CC6E8      8378 0C 00         CMP DWORD PTR DS:[EAX+C], 0
	$+D9     0C1CC6EC      74 16              JE SHORT CryGame.0C1CC704
	$+DB     0C1CC6EE      8BC8               MOV ECX, EAX
	$+DD     0C1CC6F0      8B51 0C            MOV EDX, DWORD PTR DS:[ECX+C]            ; mhoclien.00FDAFE8
	$+E0     0C1CC6F3      8B8A 84000000      MOV ECX, DWORD PTR DS:[EDX+84]
	$+E6     0C1CC6F9      8B01               MOV EAX, DWORD PTR DS:[ECX]
	$+E8     0C1CC6FB      8B40 14            MOV EAX, DWORD PTR DS:[EAX+14]
	$+EB     0C1CC6FE      8D55 F0            LEA EDX, DWORD PTR SS:[EBP-10]
	$+EE     0C1CC701      52                 PUSH EDX
	$+EF     0C1CC702      FFD0               CALL EAX
	$+F1     0C1CC704      8B5D 10            MOV EBX, DWORD PTR SS:[EBP+10]
	$+F4     0C1CC707      8B4D 0C            MOV ECX, DWORD PTR SS:[EBP+C]
	$+F7     0C1CC70A      53                 PUSH EBX
	$+F8     0C1CC70B      51                 PUSH ECX                                 ; ntdll.77C80130
	$+F9     0C1CC70C      8D55 FC            LEA EDX, DWORD PTR SS:[EBP-4]
	$+FC     0C1CC70F      52                 PUSH EDX
	$+FD     0C1CC710      8BCE               MOV ECX, ESI
	$+FF     0C1CC712      E8 7921FCFF        CALL CryGame.0C18E890
	$+104    0C1CC717      8BCF               MOV ECX, EDI
	$+106    0C1CC719      E8 B2A3E3FE        CALL CryGame.0B006AD0
	$+10B    0C1CC71E      8BCE               MOV ECX, ESI
	$+10D    0C1CC720      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+110    0C1CC723      E8 48E2FAFF        CALL CryGame.0C17A970
	$+115    0C1CC728      8B4E 08            MOV ECX, DWORD PTR DS:[ESI+8]
	$+118    0C1CC72B      6A 12              PUSH 12
	$+11A    0C1CC72D      8BF8               MOV EDI, EAX
	$+11C    0C1CC72F      E8 5CC10A00        CALL CryGame.0C278890
	$+121    0C1CC734      05 D0000000        ADD EAX, 0D0
	$+126    0C1CC739      8B00               MOV EAX, DWORD PTR DS:[EAX]
	$+128    0C1CC73B      85C0               TEST EAX, EAX
	$+12A    0C1CC73D      74 06              JE SHORT CryGame.0C1CC745
	$+12C    0C1CC73F      3BF8               CMP EDI, EAX
	$+12E    0C1CC741      7C 02              JL SHORT CryGame.0C1CC745
	$+130    0C1CC743      8BF8               MOV EDI, EAX
	$+132    0C1CC745      397D 08            CMP DWORD PTR SS:[EBP+8], EDI
	$+135    0C1CC748      7E 1C              JLE SHORT CryGame.0C1CC766
	$+137    0C1CC74A      8D8E AC000000      LEA ECX, DWORD PTR DS:[ESI+AC]
	$+13D    0C1CC750      E8 7BA3E3FE        CALL CryGame.0B006AD0
	$+142    0C1CC755      53                 PUSH EBX
	$+143    0C1CC756      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+146    0C1CC759      6A 00              PUSH 0
	$+148    0C1CC75B      8D45 08            LEA EAX, DWORD PTR SS:[EBP+8]
	$+14B    0C1CC75E      50                 PUSH EAX
	$+14C    0C1CC75F      8BCE               MOV ECX, ESI
	$+14E    0C1CC761      E8 9AB9FFFF        CALL CryGame.0C1C8100
	$+153    0C1CC766      5F                 POP EDI                                  ; ntdll.77C8FD91
	$+154    0C1CC767      84DB               TEST BL, BL
	$+156    0C1CC769      5B                 POP EBX                                  ; ntdll.77C8FD91
	$+157    0C1CC76A      75 1B              JNZ SHORT CryGame.0C1CC787
	$+159    0C1CC76C      D9EE               FLDZ
	$+15B    0C1CC76E      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+15E    0C1CC771      8B11               MOV EDX, DWORD PTR DS:[ECX]
	$+160    0C1CC773      8B82 90020000      MOV EAX, DWORD PTR DS:[EDX+290]
	$+166    0C1CC779      83EC 08            SUB ESP, 8
	$+169    0C1CC77C      D95424 04          FST DWORD PTR SS:[ESP+4]
	$+16D    0C1CC780      D91C24             FSTP DWORD PTR SS:[ESP]
	$+170    0C1CC783      6A 00              PUSH 0
	$+172    0C1CC785      FFD0               CALL EAX
	$+174    0C1CC787      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+177    0C1CC78A      8B11               MOV EDX, DWORD PTR DS:[ECX]
	$+179    0C1CC78C      8B82 70030000      MOV EAX, DWORD PTR DS:[EDX+370]
	$+17F    0C1CC792      FFD0               CALL EAX
	$+181    0C1CC794      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+184    0C1CC797      8B11               MOV EDX, DWORD PTR DS:[ECX]
	$+186    0C1CC799      8B82 8C020000      MOV EAX, DWORD PTR DS:[EDX+28C]
	$+18C    0C1CC79F      FFD0               CALL EAX
	$+18E    0C1CC7A1      5E                 POP ESI                                  ; ntdll.77C8FD91
	$+18F    0C1CC7A2      8BE5               MOV ESP, EBP
	$+191    0C1CC7A4      5D                 POP EBP                                  ; ntdll.77C8FD91
	$+192    0C1CC7A5      C2 0C00            RETN 0C
	--*/
	//83????568b??8b????8b??8b??????????ff??84??0f??????????8b????8b??89????85??7d??c7
	DWORD	dwMMOClientActorFeebHpCallHook = (DWORD)scan_game_.ScanAddr("83????568b??8b????8b??8b??????????ff??84??0f??????????8b????8b??89????85??7d??c7", -3);
	assert(dwMMOClientActorFeebHpCallHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_FEEB_HP_CALL_HOOK  " << dwMMOClientActorFeebHpCallHook - GetModuleCryGameBase() << "  // " << dwMMOClientActorFeebHpCallHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    005D6DC0      83EC 14        SUB ESP,0x14                             ; 访问玩家血值
	$+3      005D6DC3      57             PUSH EDI
	$+4      005D6DC4      8BF9           MOV EDI,ECX
	$+6      005D6DC6      8B4F 04        MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+9      005D6DC9      8B01           MOV EAX,DWORD PTR DS:[ECX]
	$+B      005D6DCB      8B90 70010000  MOV EDX,DWORD PTR DS:[EAX+0x170]
	$+11     005D6DD1      FFD2           CALL EDX
	$+13     005D6DD3      84C0           TEST AL,AL
	$+15     005D6DD5      0F85 FC010000  JNZ MHOClien.005D6FD7
	$+1B     005D6DDB      8B4424 1C      MOV EAX,DWORD PTR SS:[ESP+0x1C]
	$+1F     005D6DDF      56             PUSH ESI
	$+20     005D6DE0      8B30           MOV ESI,DWORD PTR DS:[EAX]
	$+22     005D6DE2      897424 20      MOV DWORD PTR SS:[ESP+0x20],ESI
	$+26     005D6DE6      85F6           TEST ESI,ESI
	$+28     005D6DE8      7D 06          JGE SHORT MHOClien.005D6DF0

	83EC??578BF98B4F??8B018B90????????FFD284C00F85

	$-32     01C1E5A0      83EC 14              SUB ESP,0x14                             ; 访问玩家血值 , 此函数为玩家被怪物攻击直接减掉的血值  第2个
	$-2F     01C1E5A3      56                   PUSH ESI
	$-2E     01C1E5A4      8BF1                 MOV ESI,ECX
	$-2C     01C1E5A6      8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-29     01C1E5A9      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$-27     01C1E5AB      8B90 70010000        MOV EDX,DWORD PTR DS:[EAX+0x170]
	$-21     01C1E5B1      FFD2                 CALL EDX
	$-1F     01C1E5B3      84C0                 TEST AL,AL
	$-1D     01C1E5B5      0F85 6C010000        JNZ MHOClien.01C1E727
	$-17     01C1E5BB      8B4424 1C            MOV EAX,DWORD PTR SS:[ESP+0x1C]
	$-13     01C1E5BF      8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$-11     01C1E5C1      894424 04            MOV DWORD PTR SS:[ESP+0x4],EAX
	$-D      01C1E5C5      85C0                 TEST EAX,EAX
	$-B      01C1E5C7      7D 08                JGE SHORT MHOClien.01C1E5D1
	$-9      01C1E5C9      C74424 04 00000000   MOV DWORD PTR SS:[ESP+0x4],0x0
	$-1      01C1E5D1      53                   PUSH EBX
	$ ==>    01C1E5D2      8D9E B4000000        LEA EBX,DWORD PTR DS:[ESI+0xB4]
	$+6      01C1E5D8      8BCB                 MOV ECX,EBX
	$+8      01C1E5DA      E8 51AFA4FE          CALL MHOClien.00669530
	$+D      01C1E5DF      394424 08            CMP DWORD PTR SS:[ESP+0x8],EAX
	$+11     01C1E5E3      7E 0B                JLE SHORT MHOClien.01C1E5F0
	$+13     01C1E5E5      8BCB                 MOV ECX,EBX
	$+15     01C1E5E7      E8 44AFA4FE          CALL MHOClien.00669530
	$+1A     01C1E5EC      894424 08            MOV DWORD PTR SS:[ESP+0x8],EAX
	$+1E     01C1E5F0      8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+21     01C1E5F3      6A 01                PUSH 0x1
	$+23     01C1E5F5      E8 E69A0200          CALL MHOClien.01C480E0
	$+28     01C1E5FA      84C0                 TEST AL,AL
	$+2A     01C1E5FC      74 15                JE SHORT MHOClien.01C1E613
	$+2C     01C1E5FE      8D8E AC000000        LEA ECX,DWORD PTR DS:[ESI+0xAC]
	$+32     01C1E604      E8 27BF84FE          CALL MHOClien.0046A530
	$+37     01C1E609      394424 08            CMP DWORD PTR SS:[ESP+0x8],EAX

	8D??????????8B??E8????????39??????7E??8B??E8????????89??????8B????6A

	$-32     01B8B0E0     55                  PUSH EBP
	$-31     01B8B0E1     8BEC                MOV EBP,ESP
	$-2F     01B8B0E3     83EC 14             SUB ESP,0x14
	$-2C     01B8B0E6     56                  PUSH ESI
	$-2B     01B8B0E7     8BF1                MOV ESI,ECX
	$-29     01B8B0E9     8B4E 04             MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-26     01B8B0EC     8B01                MOV EAX,DWORD PTR DS:[ECX]
	$-24     01B8B0EE     8B90 70010000       MOV EDX,DWORD PTR DS:[EAX+0x170]
	$-1E     01B8B0F4     FFD2                CALL EDX
	$-1C     01B8B0F6     84C0                TEST AL,AL
	$-1A     01B8B0F8     0F85 54010000       JNZ MHOClien.01B8B252
	$-14     01B8B0FE     8B45 08             MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-11     01B8B101     8B00                MOV EAX,DWORD PTR DS:[EAX]
	$-F      01B8B103     8945 FC             MOV DWORD PTR SS:[EBP-0x4],EAX
	$-C      01B8B106     85C0                TEST EAX,EAX
	$-A      01B8B108     7D 07               JGE SHORT MHOClien.01B8B111
	$-8      01B8B10A     C745 FC 00000000    MOV DWORD PTR SS:[EBP-0x4],0x0
	$-1      01B8B111     53                  PUSH EBX
	$ ==>    01B8B112     8D9E B4000000       LEA EBX,DWORD PTR DS:[ESI+0xB4]
	$+6      01B8B118     8BCB                MOV ECX,EBX
	$+8      01B8B11A     E8 A1E3ADFE         CALL MHOClien.006694C0
	$+D      01B8B11F     3945 FC             CMP DWORD PTR SS:[EBP-0x4],EAX
	$+10     01B8B122     7E 0A               JLE SHORT MHOClien.01B8B12E
	$+12     01B8B124     8BCB                MOV ECX,EBX
	$+14     01B8B126     E8 95E3ADFE         CALL MHOClien.006694C0
	$+19     01B8B12B     8945 FC             MOV DWORD PTR SS:[EBP-0x4],EAX
	$+1C     01B8B12E     8B4E 04             MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+1F     01B8B131     6A 01               PUSH 0x1
	$+21     01B8B133     E8 B8920200         CALL MHOClien.01BB43F0
	$+26     01B8B138     84C0                TEST AL,AL
	$+28     01B8B13A     74 14               JE SHORT MHOClien.01B8B150

	8D??????????8B??E8????????39????7E??8B??E8????????89????8B????6A01

	//zhp 2016/5/24 14:19 这个hook竟然跟上面那个一样，肯定哪里出了问题，以后再搞
	$-3      0C1CC610      55                 PUSH EBP
	$-2      0C1CC611      8BEC               MOV EBP, ESP
	$ ==>    0C1CC613      83EC 10            SUB ESP, 10
	$+3      0C1CC616      56                 PUSH ESI
	$+4      0C1CC617      8BF1               MOV ESI, ECX                             ; ntdll.77C80130
	$+6      0C1CC619      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+9      0C1CC61C      8B01               MOV EAX, DWORD PTR DS:[ECX]
	$+B      0C1CC61E      8B90 78010000      MOV EDX, DWORD PTR DS:[EAX+178]
	$+11     0C1CC624      FFD2               CALL EDX
	$+13     0C1CC626      84C0               TEST AL, AL
	$+15     0C1CC628      0F85 73010000      JNZ CryGame.0C1CC7A1
	$+1B     0C1CC62E      8B45 08            MOV EAX, DWORD PTR SS:[EBP+8]
	$+1E     0C1CC631      8B00               MOV EAX, DWORD PTR DS:[EAX]
	$+20     0C1CC633      8945 FC            MOV DWORD PTR SS:[EBP-4], EAX
	$+23     0C1CC636      85C0               TEST EAX, EAX
	$+25     0C1CC638      7D 07              JGE SHORT CryGame.0C1CC641
	$+27     0C1CC63A      C745 FC 00000000   MOV DWORD PTR SS:[EBP-4], 0
	$+2E     0C1CC641      8BCE               MOV ECX, ESI
	$+30     0C1CC643      E8 E8DFFAFF        CALL CryGame.0C17A630
	$+35     0C1CC648      3945 FC            CMP DWORD PTR SS:[EBP-4], EAX
	$+38     0C1CC64B      7E 0A              JLE SHORT CryGame.0C1CC657
	$+3A     0C1CC64D      8BCE               MOV ECX, ESI
	$+3C     0C1CC64F      E8 DCDFFAFF        CALL CryGame.0C17A630
	$+41     0C1CC654      8945 FC            MOV DWORD PTR SS:[EBP-4], EAX
	$+44     0C1CC657      8B4E 04            MOV ECX, DWORD PTR DS:[ESI+4]
	$+47     0C1CC65A      6A 01              PUSH 1
	$+49     0C1CC65C      E8 3F93FEFF        CALL CryGame.0C1B59A0
	$+4E     0C1CC661      84C0               TEST AL, AL
	$+50     0C1CC663      74 14              JE SHORT CryGame.0C1CC679
	$+52     0C1CC665      8D8E AC000000      LEA ECX, DWORD PTR DS:[ESI+AC]
	$+58     0C1CC66B      E8 60A4E3FE        CALL CryGame.0B006AD0
	$+5D     0C1CC670      3945 FC            CMP DWORD PTR SS:[EBP-4], EAX
	$+60     0C1CC673      0F8C 28010000      JL CryGame.0C1CC7A1
	$+66     0C1CC679      53                 PUSH EBX
	$+67     0C1CC67A      57                 PUSH EDI
	$+68     0C1CC67B      8DBE AC000000      LEA EDI, DWORD PTR DS:[ESI+AC]
	$+6E     0C1CC681      8BCF               MOV ECX, EDI
	$+70     0C1CC683      E8 48A4E3FE        CALL CryGame.0B006AD0
	$+75     0C1CC688      8945 08            MOV DWORD PTR SS:[EBP+8], EAX
	$+78     0C1CC68B      DB45 08            FILD DWORD PTR SS:[EBP+8]
	$+7B     0C1CC68E      8BCE               MOV ECX, ESI
	$+7D     0C1CC690      D95D F8            FSTP DWORD PTR SS:[EBP-8]
	$+80     0C1CC693      E8 98DFFAFF        CALL CryGame.0C17A630
	--*/
	//83????568b??8b????8b??8b??????????ff??84??0f??????????8b????8b??89????85??7d??c7
	DWORD	dwMMOClientActorHitHpCallHook = (DWORD)scan_game_.ScanAddr("83????568b??8b????8b??8b??????????ff??84??0f??????????8b????8b??89????85??7d??c7", -3);
	assert(dwMMOClientActorHitHpCallHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_HIT_HP_CALL_HOOK  " << dwMMOClientActorHitHpCallHook - GetModuleCryGameBase() << "  // " << dwMMOClientActorHitHpCallHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00715141      68 AA130000    PUSH 0x13AA
	$+5      00715146      BE 680F0902    MOV ESI,MHOClien.02090F68                ; ASCII "Server Connection Time Out."
	$+A      0071514B      E8 20D5FFFF    CALL MHOClien.00712670
	$+F      00715150      83C4 04        ADD ESP,0x4
	$+12     00715153      85C0           TEST EAX,EAX
	$+14     00715155      74 0E          JE SHORT MHOClien.00715165
	$+16     00715157      8B40 14        MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+19     0071515A      8BF0           MOV ESI,EAX
	$+1B     0071515C      85C0           TEST EAX,EAX

	68????????BE????????E8????????83C4??85C074??8B40??8BF085C0

	$ ==>    >  8B4424 28       MOV EAX,DWORD PTR SS:[ESP+0x28]
	$+4      >  50              PUSH EAX
	$+5      >  C64424 17 01    MOV BYTE PTR SS:[ESP+0x17],0x1
	$+A      >  E8 F471F9FF     CALL MHOClien.00635FB0		// call
	$+F      >  8BF0            MOV ESI,EAX
	$+11     >  83C4 04         ADD ESP,0x4
	$+14     >  897424 14       MOV DWORD PTR SS:[ESP+0x14],ESI
	$+18     >  85F6            TEST ESI,ESI
	$+1A     >^ 74 DA           JE SHORT MHOClien.0069EDA3
	$+1C     >  8B4E 40         MOV ECX,DWORD PTR DS:[ESI+0x40]
	$+1F     >  8D46 40         LEA EAX,DWORD PTR DS:[ESI+0x40]
	$+22     >  33DB            XOR EBX,EBX
	$+24     >  33D2            XOR EDX,EDX
	$+26     >  3BC8            CMP ECX,EAX
	$+28     >  74 0B           JE SHORT MHOClien.0069EDE2

	8B????????C6????????E8????????8BF083C40489

	$-46     006A09BD       8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-43     006A09C0       50                      PUSH EAX
	$-42     006A09C1       6A 00                   PUSH 0x0
	$-40     006A09C3       68 A4ADFE01             PUSH MHOClien.01FEADA4                   ; ASCII "CMessageInfo"
	$-3B     006A09C8       6A 00                   PUSH 0x0
	$-39     006A09CA       C645 FF 01              MOV BYTE PTR SS:[EBP-0x1],0x1
	$-35     006A09CE       E8 FDBDF9FF             CALL MHOClien.0063C7D0                   ; 获取基址
	$-30     006A09D3       83C4 0C                 ADD ESP,0xC
	$-2D     006A09D6       8BC8                    MOV ECX,EAX
	$-2B     006A09D8       E8 B3B5FBFF             CALL MHOClien.0065BF90                   ; 取对象地址
	$-26     006A09DD       8BF0                    MOV ESI,EAX
	$-24     006A09DF       8975 F8                 MOV DWORD PTR SS:[EBP-0x8],ESI
	$-21     006A09E2       85F6                    TEST ESI,ESI
	$-1F     006A09E4     ^ 74 CE                   JE SHORT MHOClien.006A09B4
	$-1D     006A09E6       8B4E 40                 MOV ECX,DWORD PTR DS:[ESI+0x40]
	$-1A     006A09E9       8D46 40                 LEA EAX,DWORD PTR DS:[ESI+0x40]
	$-17     006A09EC       33D2                    XOR EDX,EDX
	$-15     006A09EE       C745 F4 00000000        MOV DWORD PTR SS:[EBP-0xC],0x0
	$-E      006A09F5       3BC8                    CMP ECX,EAX
	$-C      006A09F7       74 12                   JE SHORT MHOClien.006A0A0B
	$-A      006A09F9       8DA424 00000000         LEA ESP,DWORD PTR SS:[ESP]
	$-3      006A0A00       8B09                    MOV ECX,DWORD PTR DS:[ECX]
	$-1      006A0A02       42                      INC EDX
	$ ==>    006A0A03       3BC8                    CMP ECX,EAX
	$+2      006A0A05     ^ 75 F9                   JNZ SHORT MHOClien.006A0A00
	$+4      006A0A07       85D2                    TEST EDX,EDX
	$+6      006A0A09       75 28                   JNZ SHORT MHOClien.006A0A33
	$+8      006A0A0B       8B46 14                 MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+B      006A0A0E       85C0                    TEST EAX,EAX
	$+D      006A0A10       75 05                   JNZ SHORT MHOClien.006A0A17
	$+F      006A0A12       B8 AE97FC01             MOV EAX,MHOClien.01FC97AE
	$+14     006A0A17       50                      PUSH EAX
	$+15     006A0A18       68 00100000             PUSH 0x1000
	$+1A     006A0A1D       68 40E14E02             PUSH MHOClien.024EE140
	$+1F     006A0A22       E8 8DBD4B01             CALL MHOClien.01B5C7B4                   ; JMP 到 msvcr90.sprintf_s

	3B??75??85??75??8B????85??75??B8??????????68

	$-3D     006A49AD          8B4424 28             MOV EAX,DWORD PTR SS:[ESP+0x28]
	$-39     006A49B1          50                    PUSH EAX
	$-38     006A49B2          6A 00                 PUSH 0x0
	$-36     006A49B4          68 C4990502           PUSH MHOClien.020599C4                   ; ASCII "CMessageInfo"
	$-31     006A49B9          6A 00                 PUSH 0x0
	$-2F     006A49BB          C64424 23 01          MOV BYTE PTR SS:[ESP+0x23],0x1
	$-2A     006A49C0          E8 4BBFF9FF           CALL MHOClien.00640910			// call
	$-25     006A49C5          83C4 0C               ADD ESP,0xC
	$-22     006A49C8          8BC8                  MOV ECX,EAX
	$-20     006A49CA          E8 01EBEBFF           CALL MHOClien.005634D0			// call
	$-1B     006A49CF          8BF0                  MOV ESI,EAX
	$-19     006A49D1          897424 14             MOV DWORD PTR SS:[ESP+0x14],ESI
	$-15     006A49D5          85F6                  TEST ESI,ESI
	$-13     006A49D7        ^ 74 CA                 JE SHORT MHOClien.006A49A3
	$-11     006A49D9          8B4E 40               MOV ECX,DWORD PTR DS:[ESI+0x40]
	$-E      006A49DC          8D46 40               LEA EAX,DWORD PTR DS:[ESI+0x40]
	$-B      006A49DF          33DB                  XOR EBX,EBX
	$-9      006A49E1          33D2                  XOR EDX,EDX
	$-7      006A49E3          3BC8                  CMP ECX,EAX
	$-5      006A49E5          74 0B                 JE SHORT MHOClien.006A49F2
	$-3      006A49E7          8B09                  MOV ECX,DWORD PTR DS:[ECX]
	$-1      006A49E9          42                    INC EDX
	$ ==>    006A49EA          3BC8                  CMP ECX,EAX
	$+2      006A49EC        ^ 75 F9                 JNZ SHORT MHOClien.006A49E7
	$+4      006A49EE          85D2                  TEST EDX,EDX
	$+6      006A49F0          75 29                 JNZ SHORT MHOClien.006A4A1B
	$+8      006A49F2          8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+B      006A49F5          85C0                  TEST EAX,EAX
	$+D      006A49F7          75 05                 JNZ SHORT MHOClien.006A49FE
	$+F      006A49F9          B8 96820302           MOV EAX,MHOClien.02038296
	$+14     006A49FE          50                    PUSH EAX
	$+15     006A49FF          68 00100000           PUSH 0x1000
	$+1A     006A4A04          68 D0D15502           PUSH MHOClien.0255D1D0
	$+1F     006A4A09          E8 063D5201           CALL MHOClien.01BC8714                   ; JMP 到 msvcr90.sprintf_s
	$+24     006A4A0E          83C4 0C               ADD ESP,0xC
	$+27     006A4A11          8BC6                  MOV EAX,ESI
	$+29     006A4A13          5F                    POP EDI
	$+2A     006A4A14          5E                    POP ESI
	$+2B     006A4A15          5D                    POP EBP
	$+2C     006A4A16          5B                    POP EBX
	$+2D     006A4A17          83C4 14               ADD ESP,0x14
	$+30     006A4A1A          C3                    RETN

	$-40     006A1EA3     68 D4CC0002     PUSH MHOClien.0200CCD4                   ; ASCII "CMessageInfo"
	$-3B     006A1EA8     6A 00           PUSH 0x0
	$-39     006A1EAA     C645 FF 01      MOV BYTE PTR SS:[EBP-0x1],0x1
	$-35     006A1EAE     E8 2DB2F9FF     CALL MHOClien.0063D0E0
	$-30     006A1EB3     83C4 0C         ADD ESP,0xC
	$-2D     006A1EB6     8BC8            MOV ECX,EAX
	$-2B     006A1EB8     E8 33EFEEFF     CALL MHOClien.00590DF0
	$-26     006A1EBD     8BF0            MOV ESI,EAX
	$-24     006A1EBF     8975 F8         MOV DWORD PTR SS:[EBP-0x8],ESI
	$-21     006A1EC2     85F6            TEST ESI,ESI
	$-1F     006A1EC4   ^ 74 CE           JE SHORT MHOClien.006A1E94
	$-1D     006A1EC6     8B4E 40         MOV ECX,DWORD PTR DS:[ESI+0x40]
	$-1A     006A1EC9     8D46 40         LEA EAX,DWORD PTR DS:[ESI+0x40]
	$-17     006A1ECC     33D2            XOR EDX,EDX
	$-15     006A1ECE     C745 F4 0000000>MOV DWORD PTR SS:[EBP-0xC],0x0
	$-E      006A1ED5     3BC8            CMP ECX,EAX
	$-C      006A1ED7     74 12           JE SHORT MHOClien.006A1EEB
	$-A      006A1ED9     8DA424 00000000 LEA ESP,DWORD PTR SS:[ESP]
	$-3      006A1EE0     8B09            MOV ECX,DWORD PTR DS:[ECX]
	$-1      006A1EE2     42              INC EDX
	$ ==>    006A1EE3     3BC8            CMP ECX,EAX
	$+2      006A1EE5   ^ 75 F9           JNZ SHORT MHOClien.006A1EE0
	$+4      006A1EE7     85D2            TEST EDX,EDX
	$+6      006A1EE9     75 28           JNZ SHORT MHOClien.006A1F13
	$+8      006A1EEB     8B46 14         MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+B      006A1EEE     85C0            TEST EAX,EAX
	$+D      006A1EF0     75 05           JNZ SHORT MHOClien.006A1EF7

	3B??75??85??75??8B????85??75??B8
	--*/
	DWORD	dwMMOClientGetUiInfoAddrCall = (DWORD)scan_game_.ScanCall("3B??75??85??75??8B????85??75??B8", -0x2A);
	assert(dwMMOClientGetUiInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_UI_INFO_ADDR_CALL  " << dwMMOClientGetUiInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetUiInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientGetUiInfoBaseAddrCall = (DWORD)scan_game_.ScanCall("3B??75??85??75??8B????85??75??B8", -0x34);
	assert(dwMMOClientGetUiInfoBaseAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_UI_INFO_BASE_ADDR_CALL  " << dwMMOClientGetUiInfoBaseAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetUiInfoBaseAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    0063A1D1      E8 7AA3DCFF    CALL MHOClien.00404550                   ; 申请存储任务对象的地址
	$+5      0063A1D6      8BF8           MOV EDI,EAX
	$+7      0063A1D8      83C4 04        ADD ESP,0x4
	$+A      0063A1DB      85FF           TEST EDI,EDI
	$+C      0063A1DD      74 2B          JE SHORT MHOClien.0063A20A
	$+E      0063A1DF      8B45 0C        MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+11     0063A1E2      50             PUSH EAX                                 ; task id
	$+12     0063A1E3      E8 6814E2FF    CALL MHOClien.0045B650                   ; 返回任务id的对象
	$+17     0063A1E8      83C4 04        ADD ESP,0x4
	$+1A     0063A1EB      85C0           TEST EAX,EAX                             ; eax为当前接受任务的对象
	$+1C     0063A1ED      74 0E          JE SHORT MHOClien.0063A1FD
	$+1E     0063A1EF      8B40 18        MOV EAX,DWORD PTR DS:[EAX+0x18]
	$+21     0063A1F2      53             PUSH EBX
	$+22     0063A1F3      50             PUSH EAX                                 ; 处理任务对象
	$+23     0063A1F4      E8 E7CFFFFF    CALL MHOClien.006371E0

	E8????????8BF883C4??85FF74??8B45??50E8

	$-30     005367D1        8B40 14         MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-2D     005367D4        8D4C24 40       LEA ECX,DWORD PTR SS:[ESP+0x40]
	$-29     005367D8        51              PUSH ECX
	$-28     005367D9        BE D8090702     MOV ESI,MHOClien.020709D8                ; ASCII "taskName"
	$-23     005367DE        8BCB            MOV ECX,EBX
	$-21     005367E0        E8 DB59F2FF     CALL MHOClien.0045C1C0
	$-1C     005367E5        83C4 04         ADD ESP,0x4
	$-19     005367E8        8D8C24 E0000000 LEA ECX,DWORD PTR SS:[ESP+0xE0]
	$-12     005367EF        E8 3CE3ECFF     CALL MHOClien.00404B30
	$-D      005367F4        8B4424 14       MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-9      005367F8        8B48 04         MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-6      005367FB        8D5424 0C       LEA EDX,DWORD PTR SS:[ESP+0xC]
	$-2      005367FF        52              PUSH EDX
	$-1      00536800        51              PUSH ECX
	$ ==>    00536801        E8 4A71F2FF     CALL MHOClien.0045D950                   ; 获取模板任务地址
	$+5      00536806        83C4 04         ADD ESP,0x4
	$+8      00536809        8BC8            MOV ECX,EAX
	$+A      0053680B        E8 A0F73200     CALL MHOClien.00865FB0
	$+F      00536810        8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+11     00536812        52              PUSH EDX
	$+12     00536813        8D4424 44       LEA EAX,DWORD PTR SS:[ESP+0x44]
	$+16     00536817        50              PUSH EAX
	$+17     00536818        53              PUSH EBX
	$+18     00536819        BA 500B0702     MOV EDX,MHOClien.02070B50                ; ASCII "taskLevel"
	$+1D     0053681E        E8 BD58F2FF     CALL MHOClien.0045C0E0

	E8????????83C4048BC8E8????????8B????8D
	--*/
	DWORD	dwMMOClientGetTaskObjAddrCall = (DWORD)scan_game_.ScanCall("E8????????83C4048BC8E8????????8B????8D", 0x1);
	assert(dwMMOClientGetTaskObjAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_TASK_OBJ_ADDR_CALL  " << dwMMOClientGetTaskObjAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetTaskObjAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0040455F      53             PUSH EBX
	$+1      00404560      56             PUSH ESI
	$+2      00404561      8B7424 10      MOV ESI,DWORD PTR SS:[ESP+0x10]
	$+6      00404565      57             PUSH EDI
	$+7      00404566      8D4424 0C      LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+B      0040456A      50             PUSH EAX
	$+C      0040456B      56             PUSH ESI
	$+D      0040456C      FF15 38595802  CALL DWORD PTR DS:[0x2585938]            ; MHOClien.CryMalloc
	$+13     00404572      8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+0x14]
	$+17     00404576      8B1D DC62DA01  MOV EBX,DWORD PTR DS:[0x1DA62DC]
	$+1D     0040457C      83C4 08        ADD ESP,0x8
	$+20     0040457F      51             PUSH ECX
	$+21     00404580      68 28595802    PUSH MHOClien.02585928
	$+26     00404585      8BF8           MOV EDI,EAX
	$+28     00404587      FFD3           CALL EBX
	$+2A     00404589      56             PUSH ESI
	$+2B     0040458A      68 30595802    PUSH MHOClien.02585930
	$+30     0040458F      FFD3           CALL EBX
	$+32     00404591      68 34595802    PUSH MHOClien.02585934
	$+37     00404596      FF15 1063DA01  CALL DWORD PTR DS:[0x1DA6310]
	$+3D     0040459C      8BC7           MOV EAX,EDI
	$+3F     0040459E      5F             POP EDI
	$+40     0040459F      5E             POP ESI
	$+41     004045A0      5B             POP EBX
	$+42     004045A1      59             POP ECX
	$+43     004045A2      C3             RETN

	53568B7424??578D4424??5056FF15????????8B4C24


	$ ==>    004045F4       8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+3      004045F7       57                      PUSH EDI
	$+4      004045F8       8D45 FC                 LEA EAX,DWORD PTR SS:[EBP-0x4]
	$+7      004045FB       50                      PUSH EAX
	$+8      004045FC       56                      PUSH ESI
	$+9      004045FD       FF15 8CDA4E02           CALL DWORD PTR DS:[0x24EDA8C]            ; MHOClien.CryMalloc
	$+F      00404603       8B4D FC                 MOV ECX,DWORD PTR SS:[EBP-0x4]
	$+12     00404606       8B1D F801D001           MOV EBX,DWORD PTR DS:[0x1D001F8]
	$+18     0040460C       83C4 08                 ADD ESP,0x8
	$+1B     0040460F       51                      PUSH ECX
	$+1C     00404610       68 7CDA4E02             PUSH MHOClien.024EDA7C
	$+21     00404615       8BF8                    MOV EDI,EAX
	$+23     00404617       FFD3                    CALL EBX
	$+25     00404619       56                      PUSH ESI
	$+26     0040461A       68 84DA4E02             PUSH MHOClien.024EDA84
	$+2B     0040461F       FFD3                    CALL EBX
	$+2D     00404621       68 88DA4E02             PUSH MHOClien.024EDA88
	$+32     00404626       FF15 0403D001           CALL DWORD PTR DS:[0x1D00304]
	$+38     0040462C       8BC7                    MOV EAX,EDI
	$+3A     0040462E       5F                      POP EDI
	$+3B     0040462F       5E                      POP ESI
	$+3C     00404630       5B                      POP EBX
	$+3D     00404631       8BE5                    MOV ESP,EBP
	$+3F     00404633       5D                      POP EBP
	$+40     00404634       C3                      RETN

	8B??????8D????5056FF15????????8B
	--*/
	DWORD	dwMMOClientCrymallocFunAddr = (DWORD)scan_game_.ScanAddr("8B??????8D????5056FF15????????8B", 0x40, 2);
	assert(dwMMOClientCrymallocFunAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CRYMALLOC_FUNADDR  " << dwMMOClientCrymallocFunAddr - GetModuleCryGameBase() << "  // " << dwMMOClientCrymallocFunAddr << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00536736      8B0D 985C5802  MOV ECX,DWORD PTR DS:[0x2585C98]         ; 任务基址
	$+6      0053673C      3BCE           CMP ECX,ESI
	$+8      0053673E      0F84 76100000  JE MHOClien.005377BA
	$+E      00536744      8B01           MOV EAX,DWORD PTR DS:[ECX]
	$+10     00536746      8B50 50        MOV EDX,DWORD PTR DS:[EAX+0x50]
	$+13     00536749      53             PUSH EBX                                 ; task id
	$+14     0053674A      FFD2           CALL EDX                                 ; 返回task对象地址
	$+16     0053674C      8B0D 985C5802  MOV ECX,DWORD PTR DS:[0x2585C98]
	$+1C     00536752      8B51 08        MOV EDX,DWORD PTR DS:[ECX+0x8]
	$+1F     00536755      894424 54      MOV DWORD PTR SS:[ESP+0x54],EAX
	$+23     00536759      3972 08        CMP DWORD PTR DS:[EDX+0x8],ESI

	8B??????????3B??0F??????????8B??8B??????FF
	--*/
	DWORD	dwMMOClientTaskBaseAddr = *scan_game_.ScanImme<DWORD>("8B??????????3B??0F??????????8B??8B??????FF", 0x2);
	assert(dwMMOClientTaskBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TASK_BASE_ADDR  " << dwMMOClientTaskBaseAddr - GetModuleCryGameBase() << "  // " << dwMMOClientTaskBaseAddr << " CryGame.dll" << FC_OFstream_EndlLog;





	/*++
	$ ==>    006C6410      8379 74 00        CMP DWORD PTR DS:[ECX+0x74],0x0
	$+4      006C6414      75 05             JNZ SHORT MHOClien.006C641B
	$+6      006C6416      33C0              XOR EAX,EAX
	$+8      006C6418      C2 0400           RETN 0x4
	$+B      006C641B      8B49 74           MOV ECX,DWORD PTR DS:[ECX+0x74]
	$+E      006C641E      8B01              MOV EAX,DWORD PTR DS:[ECX]
	$+10     006C6420      8B40 14           MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+13     006C6423      FFE0              JMP EAX

	8379????75??33C0C2????8B49??8B018B40

	$ ==>    0045CF24       E8 171D7001             CALL MHOClien.01B5EC40
	$+5      0045CF29       8B4B 08                 MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+8      0045CF2C       51                      PUSH ECX
	$+9      0045CF2D       E8 AEEC7201             CALL MHOClien.01B8BBE0
	$+E      0045CF32       8BC8                    MOV ECX,EAX
	$+10     0045CF34       E8 F7611800             CALL MHOClien.005E3130
	{
	$ ==>    005E3130       55                      PUSH EBP
	$+1      005E3131       8BEC                    MOV EBP,ESP
	$+3      005E3133       8379 74 00              CMP DWORD PTR DS:[ECX+0x74],0x0
	$+7      005E3137       75 06                   JNZ SHORT MHOClien.005E313F
	$+9      005E3139       33C0                    XOR EAX,EAX
	$+B      005E313B       5D                      POP EBP
	$+C      005E313C       C2 0400                 RETN 0x4
	$+F      005E313F       8B49 74                 MOV ECX,DWORD PTR DS:[ECX+0x74]
	$+12     005E3142       8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+14     005E3144       8B40 14                 MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+17     005E3147       5D                      POP EBP
	$+18     005E3148       FFE0                    JMP EAX
	}
	$+15     0045CF39       8BF0                    MOV ESI,EAX
	$+17     0045CF3B       85F6                    TEST ESI,ESI
	$+19     0045CF3D       74 2F                   JE SHORT MHOClien.0045CF6E
	$+1B     0045CF3F       8B16                    MOV EDX,DWORD PTR DS:[ESI]
	$+1D     0045CF41       8B82 D0000000           MOV EAX,DWORD PTR DS:[EDX+0xD0]
	$+23     0045CF47       8BCE                    MOV ECX,ESI
	$+25     0045CF49       FFD0                    CALL EAX


	E8????????8B??????E8????????8B??E8????????8B??85??74??8B
	--*/
	DWORD	dwMMOClientObjCall = (DWORD)scan_cry_action_.ScanCall("E8????????8B??????E8????????8B??E8????????8B??85??74??8B", 0x11);
	assert(dwMMOClientObjCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_CALL  " << dwMMOClientObjCall - GetModuleCryActionBase() << "  // " << dwMMOClientObjCall << " CryAction.dll" << FC_OFstream_EndlLog;


	DWORD	dwMMOClientObjAddr = (DWORD)scan_cry_action_.ScanCall("E8????????8B??????E8????????8B??E8????????8B??85??74??8B", 0xA);
	assert(dwMMOClientObjAddr);
	scan_tmp_.SetCodeRange((char*)dwMMOClientObjAddr, (char*)dwMMOClientObjAddr + 0x10);
	dwMMOClientObjAddr = *scan_tmp_.ScanImme<DWORD>("A1", 1);
	assert(dwMMOClientObjAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_ADDR  " << dwMMOClientObjAddr - GetModuleCryActionBase() << "  // " << dwMMOClientObjAddr << " CryAction.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00610E00      8B8B A01F0000             MOV ECX,DWORD PTR DS:[EBX+0x1FA0]        ; ebx = 角色对象地址
	$+6      00610E06      8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+9      00610E09      897424 1C                 MOV DWORD PTR SS:[ESP+0x1C],ESI
	$+D      00610E0D      75 18                     JNZ SHORT MHOClien.00610E27
	$+F      00610E0F      830D 6CF65802 01          OR DWORD PTR DS:[0x258F66C],0x1
	$+16     00610E16      C705 64F65802 00000000    MOV DWORD PTR DS:[0x258F664],0x0
	$+20     00610E20      C605 68F65802 01          MOV BYTE PTR DS:[0x258F668],0x1
	$+27     00610E27      8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$+29     00610E29      68 64F65802               PUSH MHOClien.0258F664
	$+2E     00610E2E      50                        PUSH EAX                                 ; 循环任务id
	$+2F     00610E2F      8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+32     00610E32      FFD0                      CALL EAX                                 ; 判断任务是否需要显示
	$+34     00610E34      84C0                      TEST AL,AL
	$+36     00610E36      74 29                     JE SHORT MHOClien.00610E61

	8B8B????????8B46??897424??75??830D

	$ ==>    01C5021D        8D49 00         LEA ECX,DWORD PTR DS:[ECX]
	$+3      01C50220        8B1E            MOV EBX,DWORD PTR DS:[ESI]
	$+5      01C50222        8B4424 14       MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+9      01C50226        8B53 04         MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+C      01C50229        8B88 A01F0000   MOV ECX,DWORD PTR DS:[EAX+0x1FA0]
	$+12     01C5022F        52              PUSH EDX                                 ; 任务id
	$+13     01C50230        895C24 20       MOV DWORD PTR SS:[ESP+0x20],EBX
	$+17     01C50234        E8 A73B0200     CALL MHOClien.01C73DE0                   ; 查询任务是否可接call
	$+1C     01C50239        84C0            TEST AL,AL
	$+1E     01C5023B        74 26           JE SHORT MHOClien.01C50263
	$+20     01C5023D        8B47 04         MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+23     01C50240        3B47 08         CMP EAX,DWORD PTR DS:[EDI+0x8]
	$+26     01C50243        74 08           JE SHORT MHOClien.01C5024D

	8D????8B??8B??????8B????8B????????????89??????E8

	$ ==>    01BBC2DD     8D49 00             LEA ECX,DWORD PTR DS:[ECX]
	$+3      01BBC2E0     8B1E                MOV EBX,DWORD PTR DS:[ESI]
	$+5      01BBC2E2     8B45 08             MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8      01BBC2E5     8B53 04             MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+B      01BBC2E8     8B88 A01F0000       MOV ECX,DWORD PTR DS:[EAX+0x1FA0]
	$+11     01BBC2EE     52                  PUSH EDX
	$+12     01BBC2EF     895D 10             MOV DWORD PTR SS:[EBP+0x10],EBX
	$+15     01BBC2F2     E8 E9390200         CALL MHOClien.01BDFCE0
	$+1A     01BBC2F7     84C0                TEST AL,AL
	$+1C     01BBC2F9     74 24               JE SHORT MHOClien.01BBC31F
	$+1E     01BBC2FB     8B47 04             MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+21     01BBC2FE     3B47 08             CMP EAX,DWORD PTR DS:[EDI+0x8]
	$+24     01BBC301     74 08               JE SHORT MHOClien.01BBC30B

	// 偏移和之前不一样了
	$ ==>    >  8D49 00         LEA ECX,DWORD PTR DS:[ECX]
	$+3      >  8B1E            MOV EBX,DWORD PTR DS:[ESI]
	$+5      >  8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8      >  8B53 04         MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+B      >  8B88 08200000   MOV ECX,DWORD PTR DS:[EAX+0x2008]	// 由之前的0x1FA0变成0x2008
	$+11     >  52              PUSH EDX
	$+12     >  895D 10         MOV DWORD PTR SS:[EBP+0x10],EBX
	$+15     >  E8 D93C0200     CALL MHOClien.01BE1970
	$+1A     >  84C0            TEST AL,AL
	$+1C     >  74 24           JE SHORT MHOClien.01BBDCBF
	$+1E     >  8B47 04         MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+21     >  3B47 08         CMP EAX,DWORD PTR DS:[EDI+0x8]

	8D????8B??8B????8B????8B????????????89

	$ ==>    >  8D49 00         LEA ECX,DWORD PTR DS:[ECX]
	$+3      >  8B1E            MOV EBX,DWORD PTR DS:[ESI]
	$+5      >  8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8      >  8B53 04         MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+B      >  8B88 E4200000   MOV ECX,DWORD PTR DS:[EAX+0x20E4]
	$+11     >  52              PUSH EDX                                 ; 0x00000963
	$+12     >  895D 10         MOV DWORD PTR SS:[EBP+0x10],EBX
	$+15     >  E8 693A0E00     CALL CryGame.0D6BBB70
	$+1A     >  84C0            TEST AL,AL
	$+1C     >  74 24           JE SHORT CryGame.0D5D812F
	$+1E     >  8B47 04         MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+21     >  3B47 08         CMP EAX,DWORD PTR DS:[EDI+0x8]
	$+24     >  74 08           JE SHORT CryGame.0D5D811B
	$+26     >  8918            MOV DWORD PTR DS:[EAX],EBX
	$+28     >  8347 04 04      ADD DWORD PTR DS:[EDI+0x4],0x4
	$+2C     >  EB 14           JMP SHORT CryGame.0D5D812F

	//zhp 2016/5/16 16:44,上面的特征码可以定位到两份，两份基本相同
	$-3D     0C0704B0      55                    PUSH EBP
	$-3C     0C0704B1      8BEC                  MOV EBP, ESP
	$-3A     0C0704B3      56                    PUSH ESI
	$-39     0C0704B4      57                    PUSH EDI
	$-38     0C0704B5      8B7D 0C               MOV EDI, DWORD PTR SS:[EBP+C]
	$-35     0C0704B8      8BCF                  MOV ECX, EDI
	$-33     0C0704BA      E8 C15BFCFF           CALL CryGame.0C036080
	$-2E     0C0704BF      8BC8                  MOV ECX, EAX
	$-2C     0C0704C1      E8 0A27B7FF           CALL CryGame.0BBE2BD0
	$-27     0C0704C6      8BC8                  MOV ECX, EAX
	$-25     0C0704C8      E8 634BBCFF           CALL CryGame.0BC35030
	$-20     0C0704CD      8B30                  MOV ESI, DWORD PTR DS:[EAX]
	$-1E     0C0704CF      8BCF                  MOV ECX, EDI
	$-1C     0C0704D1      E8 AA5BFCFF           CALL CryGame.0C036080
	$-17     0C0704D6      8BC8                  MOV ECX, EAX
	$-15     0C0704D8      E8 F326B7FF           CALL CryGame.0BBE2BD0
	$-10     0C0704DD      8BC8                  MOV ECX, EAX
	$-E      0C0704DF      E8 4C4BBCFF           CALL CryGame.0BC35030
	$-9      0C0704E4      8B7D 10               MOV EDI, DWORD PTR SS:[EBP+10]
	$-6      0C0704E7      3B70 04               CMP ESI, DWORD PTR DS:[EAX+4]
	$-3      0C0704EA      74 62                 JE SHORT CryGame.0C07054E
	$-1      0C0704EC      53                    PUSH EBX
	$ ==>    0C0704ED      8D49 00               LEA ECX, DWORD PTR DS:[ECX]
	$+3      0C0704F0      8B1E                  MOV EBX, DWORD PTR DS:[ESI]
	$+5      0C0704F2      8B45 08               MOV EAX, DWORD PTR SS:[EBP+8]
	$+8      0C0704F5      8B53 04               MOV EDX, DWORD PTR DS:[EBX+4]
	$+B      0C0704F8      8B88 54220000         MOV ECX, DWORD PTR DS:[EAX+2254]
	$+11     0C0704FE      52                    PUSH EDX
	$+12     0C0704FF      895D 10               MOV DWORD PTR SS:[EBP+10], EBX
	$+15     0C070502      E8 C98A0F00           CALL CryGame.0C168FD0
	$+1A     0C070507      84C0                  TEST AL, AL
	$+1C     0C070509      74 24                 JE SHORT CryGame.0C07052F
	$+1E     0C07050B      8B47 04               MOV EAX, DWORD PTR DS:[EDI+4]
	$+21     0C07050E      3B47 08               CMP EAX, DWORD PTR DS:[EDI+8]
	$+24     0C070511      74 08                 JE SHORT CryGame.0C07051B
	$+26     0C070513      8918                  MOV DWORD PTR DS:[EAX], EBX
	$+28     0C070515      8347 04 04            ADD DWORD PTR DS:[EDI+4], 4
	$+2C     0C070519      EB 14                 JMP SHORT CryGame.0C07052F
	$+2E     0C07051B      6A 01                 PUSH 1
	$+30     0C07051D      6A 01                 PUSH 1
	$+32     0C07051F      8D4D 13               LEA ECX, DWORD PTR SS:[EBP+13]
	$+35     0C070522      51                    PUSH ECX                                 ; ntdll.779301E4
	$+36     0C070523      8D55 10               LEA EDX, DWORD PTR SS:[EBP+10]
	$+39     0C070526      52                    PUSH EDX
	$+3A     0C070527      50                    PUSH EAX
	$+3B     0C070528      8BCF                  MOV ECX, EDI
	$+3D     0C07052A      E8 7113B8FF           CALL CryGame.0BBF18A0
	$+42     0C07052F      8B4D 0C               MOV ECX, DWORD PTR SS:[EBP+C]
	$+45     0C070532      83C6 04               ADD ESI, 4
	$+48     0C070535      E8 465BFCFF           CALL CryGame.0C036080
	$+4D     0C07053A      8BC8                  MOV ECX, EAX
	$+4F     0C07053C      E8 8F26B7FF           CALL CryGame.0BBE2BD0
	$+54     0C070541      8BC8                  MOV ECX, EAX
	$+56     0C070543      E8 E84ABCFF           CALL CryGame.0BC35030
	$+5B     0C070548      3B70 04               CMP ESI, DWORD PTR DS:[EAX+4]
	$+5E     0C07054B    ^ 75 A3                 JNZ SHORT CryGame.0C0704F0
	$+60     0C07054D      5B                    POP EBX                                  ; ntdll.7793FD91
	$+61     0C07054E      8B47 04               MOV EAX, DWORD PTR DS:[EDI+4]
	$+64     0C070551      2B07                  SUB EAX, DWORD PTR DS:[EDI]
	$+66     0C070553      5F                    POP EDI                                  ; ntdll.7793FD91
	$+67     0C070554      C1F8 02               SAR EAX, 2
	$+6A     0C070557      5E                    POP ESI                                  ; ntdll.7793FD91
	$+6B     0C070558      5D                    POP EBP                                  ; ntdll.7793FD91
	$+6C     0C070559      C3                    RETN
	--*/
	//feature code:8D????8B??8B????8B????8B????????????89
	/*DWORD	dwMMOClientObjOffset1 = *scan_game.ScanImme<DWORD>("8D????8B??8B??????8B????8B????????????89??????E8",0x0E);
	if( dwMMOClientObjOffset1 == 0 )
	{*/
	DWORD	dwMMOClientObjOffset1 = *scan_cry_action_.ScanImme<DWORD>("8D????8B??8B????8B????8B????????????89", 0x0D);
	//}
	assert(dwMMOClientObjOffset1);
	//assert(dwMMOClientObjOffset1==0x2254/*0x21DC*//*0x0002184*//*0x00002144*//*0x000020E4*/  /*0x00002044*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_OFFSET1  " << dwMMOClientObjOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientGetCanAccpetTaskCall = (DWORD)scan_game_.ScanCall("8D????8B??8B????8B????8B????????????89", 0x16);
	assert(dwMMOClientGetCanAccpetTaskCall);
	ofs << "#define UPDATES_MMOCLINET_REVERSAL_GAME_GET_CAN_ACCPET_TASK_CALL  " << dwMMOClientGetCanAccpetTaskCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetCanAccpetTaskCall << " CryGame.dll" << FC_OFstream_EndlLog;

	// 	DWORD	dwMMOClientObjIsReceviedTaskVfunAddrOffset = ScanImme((HANDLE)-1,"8B8B????????8B46??897424??75??830D",0x31,1);
	// 	ASSERT_GWLR(dwMMOClientObjIsReceviedTaskVfunAddrOffset);
	// 	LOG << "#define MMOCLIENT_REVERSAL_GAME_OBJ_IS_RECEVIED_TASK_VFUNADDR_OFFSET  ";
	// 	LOG << dwMMOClientObjIsReceviedTaskVfunAddrOffset;
	// 	LOG_END;

	// 	DWORD	dwMMOClientObjTaskAddr = ScanImme((HANDLE)-1,"8B8B????????8B46??897424??75??830D",0x18);
	// 	ASSERT_GWLR(dwMMOClientObjTaskAddr);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_TASK_ADDR  ";
	// 	LOG << dwMMOClientObjTaskAddr;
	// 	LOG_END;

	/*++
	$ ==>    00864C40      55                        PUSH EBP
	$+1      00864C41      8BEC                      MOV EBP,ESP
	$+3      00864C43      33C0                      XOR EAX,EAX
	$+5      00864C45    ^ 75 FC                     JNZ SHORT MHOClien.00864C43
	$+7      00864C47      833D 54C81002 00          CMP DWORD PTR DS:[0x210C854],0x0
	$+E      00864C4E      75 04                     JNZ SHORT MHOClien.00864C54
	$+10     00864C50      33C0                      XOR EAX,EAX
	$+12     00864C52      EB 13                     JMP SHORT MHOClien.00864C67
	$+14     00864C54      8B4D 10                   MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+17     00864C57      51                        PUSH ECX
	$+18     00864C58      8B55 0C                   MOV EDX,DWORD PTR SS:[EBP+0xC]
	$+1B     00864C5B      52                        PUSH EDX
	$+1C     00864C5C      8B0D 54C81002             MOV ECX,DWORD PTR DS:[0x210C854]
	$+22     00864C62      E8 19F90000               CALL MHOClien.00874580
	$+27     00864C67      5D                        POP EBP
	$+28     00864C68      C3                        RETN

	558BEC33C075??833D??????????75??33C0
	--*/
	DWORD	dwMMOClientInfoHook = (DWORD)scan_cry_action_.ScanAddr("558BEC33C075??833D??????????75??33C0", 0x28);
	assert(dwMMOClientInfoHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_INFO_HOOK  " << dwMMOClientInfoHook - GetModuleCryActionBase() << "  // " << dwMMOClientInfoHook << " CryAction.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    004597F0      56                PUSH ESI
	$+1      004597F1      8B35 2C735802     MOV ESI,DWORD PTR DS:[0x258732C]         ; 链表头
	$+7      004597F7      81FE 2C735802     CMP ESI,MHOClien.0258732C
	$+D      004597FD      74 19             JE SHORT MHOClien.00459818
	$+F      004597FF      90                NOP
	$+10     00459800      8B4E 08           MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+13     00459803      85C9              TEST ECX,ECX
	$+15     00459805      74 07             JE SHORT MHOClien.0045980E
	$+17     00459807      8B01              MOV EAX,DWORD PTR DS:[ECX]
	$+19     00459809      8B50 0C           MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+1C     0045980C      FFD2              CALL EDX                                 ; 依次访问所有对象
	$+1E     0045980E      8B36              MOV ESI,DWORD PTR DS:[ESI]
	$+20     00459810      81FE 2C735802     CMP ESI,MHOClien.0258732C                ; 双向循环链表
	$+26     00459816    ^ 75 E8             JNZ SHORT MHOClien.00459800
	$+28     00459818      5E                POP ESI
	$+29     00459819      C3                RETN

	568B35????????81FE????????74??908B4E??85C9
	--*/
	DWORD	dwMMOClientAllObjAddr = *scan_game_.ScanImme<DWORD>("568B35????????81FE????????74??908B4E??85C9", 0x3);
	assert(dwMMOClientAllObjAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_ADDR  " << dwMMOClientAllObjAddr - GetModuleCryGameBase() << "  // " << dwMMOClientAllObjAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientAllObjOffset = *scan_game_.ScanImme<unsigned char>("568B35????????81FE????????74??908B4E??85C9", 0x12);
	assert(dwMMOClientAllObjOffset);
	assert(dwMMOClientAllObjOffset == 0x00000008);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_OFFSET  " << dwMMOClientAllObjOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0052899B      8B15 0C595802     MOV EDX,DWORD PTR DS:[0x258590C]         ; MHOClien.0209E0A0
	$+6      005289A1      8B4A 50           MOV ECX,DWORD PTR DS:[EDX+0x50]
	$+9      005289A4      8B01              MOV EAX,DWORD PTR DS:[ECX]
	$+B      005289A6      8B50 48           MOV EDX,DWORD PTR DS:[EAX+0x48]
	$+E      005289A9      FFD2              CALL EDX
	$+10     005289AB      50                PUSH EAX                                 ; 访问对象的返回地址
	$+11     005289AC      8D4C24 58         LEA ECX,DWORD PTR SS:[ESP+0x58]
	$+15     005289B0      E8 9B420000       CALL MHOClien.0052CC50
	$+1A     005289B5      8D4C24 54         LEA ECX,DWORD PTR SS:[ESP+0x54]
	$+1E     005289B9      E8 A2804C00       CALL MHOClien.009F0A60

	8B15????????8B4A??8B018B50??FFD250
	--*/
	DWORD	dwMMOClientTraversalsigleObjOffset1 = *scan_game_.ScanImme<byte>("8B15????????8B4A??8B018B50??FFD250", 0x8);
	assert(dwMMOClientTraversalsigleObjOffset1);
	assert(dwMMOClientTraversalsigleObjOffset1 == 0x00000050);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET1  " << dwMMOClientTraversalsigleObjOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00A322C2      8B46 0C           MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+3      00A322C5      8B56 04           MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+6      00A322C8      8D48 01           LEA ECX,DWORD PTR DS:[EAX+0x1]
	$+9      00A322CB      894E 0C           MOV DWORD PTR DS:[ESI+0xC],ECX
	$+C      00A322CE      8B4A 5C           MOV ECX,DWORD PTR DS:[EDX+0x5C]          ; +0x5c
	$+F      00A322D1      8B0481            MOV EAX,DWORD PTR DS:[ECX+EAX*4]
	$+12     00A322D4      5E                POP ESI
	$+13     00A322D5      C3                RETN

	8B46??8B56??8D48??894E??8B4A??8B0481
	--*/
	DWORD	dwMMOClientTraversalsigleObjOffset2 = *scan_exe_.ScanImme<BYTE>("8B46??8B56??8D48??894E??8B4A??8B0481", 0x0E);
	assert(dwMMOClientTraversalsigleObjOffset2);
	assert(dwMMOClientTraversalsigleObjOffset2 == 0x0000005C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET2  " << dwMMOClientTraversalsigleObjOffset2 << FC_OFstream_EndlLog;

	/*++
	// 先遣测试 将CMHLevelInfo代码放到一个函数里了，所以这个特征码可以暂时取消
	$-9      0052C9D0      55                    PUSH EBP
	$-8      0052C9D1      8BEC                  MOV EBP,ESP
	$-6      0052C9D3      83EC 1C               SUB ESP,0x1C
	$-3      0052C9D6      8D4D FC               LEA ECX,DWORD PTR SS:[EBP-0x4]
	$ ==>    0052C9D9      E8 D207F1FF           CALL MHOClien.0043D1B0
	$+5      0052C9DE      C745 FC 6C7A0702      MOV DWORD PTR SS:[EBP-0x4],MHOClien.02077A6C
	$+C      0052C9E5      8B45 10               MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+F      0052C9E8      50                    PUSH EAX
	$+10     0052C9E9      8B4D 0C               MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+13     0052C9EC      51                    PUSH ECX
	$+14     0052C9ED      8D55 FC               LEA EDX,DWORD PTR SS:[EBP-0x4]
	$+17     0052C9F0      52                    PUSH EDX
	$+18     0052C9F1      E8 DA6B3300           CALL MHOClien.008635D0
	$+1D     0052C9F6      83C4 0C               ADD ESP,0xC
	$+20     0052C9F9      8945 F8               MOV DWORD PTR SS:[EBP-0x8],EAX
	$+23     0052C9FC      837D F8 00            CMP DWORD PTR SS:[EBP-0x8],0x0
	$+27     0052CA00      74 13                 JE SHORT MHOClien.0052CA15

	E8????????C745??????????8B45??508B4D??518D55??52
	--*/
	// 	DWORD	dwMMOClientGetFbBaseAddrCall = ScanAddr((HANDLE)-1,"E8????????C745??????????8B45??508B4D??518D55??52",-0x09,4,2);
	// 	ASSERT_GWLR(dwMMOClientGetFbBaseAddrCall);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_GET_FB_BASE_ADDR_CALL  ";
	// 	LOG << dwMMOClientGetFbBaseAddrCall;
	// 	LOG_END;

	/*++
	$-C      00914FA0      55                    PUSH EBP
	$-B      00914FA1      8BEC                  MOV EBP,ESP
	$-9      00914FA3      83EC 0C               SUB ESP,0xC
	$-6      00914FA6      894D F4               MOV DWORD PTR SS:[EBP-0xC],ECX
	$-3      00914FA9      8B45 F4               MOV EAX,DWORD PTR SS:[EBP-0xC]
	$ ==>    00914FAC      8378 28 00            CMP DWORD PTR DS:[EAX+0x28],0x0
	$+4      00914FB0      75 04                 JNZ SHORT MHOClien.00914FB6
	$+6      00914FB2      33C0                  XOR EAX,EAX
	$+8      00914FB4      EB 60                 JMP SHORT MHOClien.00915016
	$+A      00914FB6      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+D      00914FB9      8B55 08               MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+10     00914FBC      2B51 30               SUB EDX,DWORD PTR DS:[ECX+0x30]
	$+13     00914FBF      8955 08               MOV DWORD PTR SS:[EBP+0x8],EDX
	$+16     00914FC2      78 0B                 JS SHORT MHOClien.00914FCF

	8378????75??33C0EB??8B4D??8B55??2B51??8955


	$ ==>    0CF6FDC8       8B15 F08EAC0D        MOV EDX,DWORD PTR DS:[0xDAC8EF0]                     ; MHOClien.015D3088
	$+6      0CF6FDCE       8B82 C8000000        MOV EAX,DWORD PTR DS:[EDX+0xC8]
	$+C      0CF6FDD4       8B0D F08EAC0D        MOV ECX,DWORD PTR DS:[0xDAC8EF0]                     ; MHOClien.015D3088
	$+12     0CF6FDDA       8B89 C8000000        MOV ECX,DWORD PTR DS:[ECX+0xC8]
	$+18     0CF6FDE0       8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$+1A     0CF6FDE2       8B42 6C              MOV EAX,DWORD PTR DS:[EDX+0x6C]
	$+1D     0CF6FDE5       FFD0                 CALL EAX
	$+1F     0CF6FDE7       50                   PUSH EAX
	$+20     0CF6FDE8       E8 934691FF          CALL CryGame.0C884480
	{
	$ ==>    0C884480       55                   PUSH EBP
	$+1      0C884481       8BEC                 MOV EBP,ESP
	$+3      0C884483       8B45 08              MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+6      0C884486       50                   PUSH EAX
	$+7      0C884487       6A 00                PUSH 0x0
	$+9      0C884489       68 C068A80D          PUSH CryGame.0DA868C0                                ; ASCII "CMHLevelInfo"
	$+E      0C88448E       6A 00                PUSH 0x0
	$+10     0C884490       E8 5BFFFFFF          CALL CryGame.0C8843F0
	$+15     0C884495       83C4 0C              ADD ESP,0xC
	$+18     0C884498       8BC8                 MOV ECX,EAX
	$+1A     0C88449A       E8 01F4FFFF          CALL CryGame.0C8838A0
	$+1F     0C88449F       5D                   POP EBP
	$+20     0C8844A0       C3                   RETN
	}
	$+25     0CF6FDED       83C4 04              ADD ESP,0x4
	$+28     0CF6FDF0       8945 D0              MOV DWORD PTR SS:[EBP-0x30],EAX
	$+2B     0CF6FDF3       837D D0 00           CMP DWORD PTR SS:[EBP-0x30],0x0
	$+2F     0CF6FDF7       74 3F                JE SHORT CryGame.0CF6FE38

	8B??????????8B??????????8B??????????8B??????????8B??8B????FF????E8

	--*/
	DWORD	dwMMOClientGetFbInfoAddrCall = (DWORD)scan_game_.ScanCall("8B??????????8B??????????8B??????????8B??????????8B??8B????FF????E8", 0x21);
	assert(dwMMOClientGetFbInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_FB_INFO_ADDR_CALL  " << dwMMOClientGetFbInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetFbInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++

	$ ==>    0D135BC3       83F8 01              CMP EAX,0x1
	$+3      0D135BC6       74 05                JE SHORT CryGame.0D135BCD
	$+5      0D135BC8       83F8 02              CMP EAX,0x2
	$+8      0D135BCB       75 21                JNZ SHORT CryGame.0D135BEE
	$+A      0D135BCD       8B8B E0200000        MOV ECX,DWORD PTR DS:[EBX+0x20E0]                    ; +0x20E0
	$+10     0D135BD3       8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+12     0D135BD5       8B50 2C              MOV EDX,DWORD PTR DS:[EAX+0x2C]
	$+15     0D135BD8       57                   PUSH EDI
	$+16     0D135BD9       FFD2                 CALL EDX
	$+18     0D135BDB       84C0                 TEST AL,AL
	$+1A     0D135BDD       75 0F                JNZ SHORT CryGame.0D135BEE
	$+1C     0D135BDF       B8 01000000          MOV EAX,0x1
	$+21     0D135BE4       3947 40              CMP DWORD PTR DS:[EDI+0x40],EAX
	$+24     0D135BE7       75 19                JNZ SHORT CryGame.0D135C02
	$+26     0D135BE9       8946 30              MOV DWORD PTR DS:[ESI+0x30],EAX
	$+29     0D135BEC       EB 07                JMP SHORT CryGame.0D135BF5
	$+2B     0D135BEE       C746 30 00000000     MOV DWORD PTR DS:[ESI+0x30],0x0

	83????74??83????75??8B??????????8B??8B??????FF

	// 不删档测试
	$ ==>    >  8B8B 88210000   MOV ECX,DWORD PTR DS:[EBX+0x2188]
	$+6      >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+8      >  8B42 2C         MOV EAX,DWORD PTR DS:[EDX+0x2C]
	$+B      >  57              PUSH EDI
	$+C      >  FFD0            CALL EAX
	$+E      >  84C0            TEST AL,AL
	$+10     >  75 12           JNZ SHORT CryGame.0D16F097
	$+12     >  B8 01000000     MOV EAX,0x1
	$+17     >  3947 40         CMP DWORD PTR DS:[EDI+0x40],EAX
	$+1A     >  75 1C           JNZ SHORT CryGame.0D16F0AB
	$+1C     >  8946 30         MOV DWORD PTR DS:[ESI+0x30],EAX
	$+1F     >  EB 0A           JMP SHORT CryGame.0D16F09E
	$+21     >  8B5D 08         MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+24     >  C746 30 0000000>MOV DWORD PTR DS:[ESI+0x30],0x0


	8B??????????8B??8B??????FF??84??75??B8????????39

	--*/
	DWORD	dwMMOClientInFbInfoOffset1 = *scan_game_.ScanImme<DWORD>("8B??????????8B??8B??????FF??84??75??B8????????39", 2);
	assert(dwMMOClientInFbInfoOffset1);
	//assert(dwMMOClientInFbInfoOffset1==0x2288/*0x00002230*//*0x000021F0*//*0x2188*/  /*0x000020E8*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_IN_FB_INFO_OFFSET1  " << dwMMOClientInFbInfoOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++

	$ ==>    0CF4CDDD       E8 DECF75FF          CALL CryGame.0C6A9DC0
	$+5      0CF4CDE2       57                   PUSH EDI
	$+6      0CF4CDE3       E8 38586BFF          CALL CryGame.0C602620                                ; CItemBaseInfo
	$+B      0CF4CDE8       8BF0                 MOV ESI,EAX
	$+D      0CF4CDEA       83C4 04              ADD ESP,0x4
	$+10     0CF4CDED       3BF3                 CMP ESI,EBX
	$+12     0CF4CDEF       0F84 3F060000        JE CryGame.0CF4D434
	$+18     0CF4CDF5       89BD D4FEFFFF        MOV DWORD PTR SS:[EBP-0x12C],EDI
	$+1E     0CF4CDFB       C785 C8FEFFFF 010000>MOV DWORD PTR SS:[EBP-0x138],0x1
	$+28     0CF4CE05       8B46 14              MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+2B     0CF4CE08       3BC3                 CMP EAX,EBX
	$+2D     0CF4CE0A       75 05                JNZ SHORT CryGame.0CF4CE11

	E8??????????E8????????8B????????3B??0F84????????89
	--*/
	DWORD	dwMMOClientGetItemInfoAddrCall = (DWORD)scan_game_.ScanCall("E8??????????E8????????8B????????3B??0F84????????89", 0x07);
	assert(dwMMOClientGetItemInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_ITEM_INFO_ADDR_CALL  " << dwMMOClientGetItemInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetItemInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0046C3A7      391D B85C5802   CMP DWORD PTR DS:[0x2585CB8],EBX         ; 基址
	$+6      0046C3AD      75 20           JNZ SHORT MHOClien.0046C3CF
	$+8      0046C3AF      6A 50           PUSH 0x50
	$+A      0046C3B1      E8 AA81F9FF     CALL MHOClien.00404560
	$+F      0046C3B6      83C4 04         ADD ESP,0x4
	$+12     0046C3B9      3BC3            CMP EAX,EBX
	$+14     0046C3BB      74 0C           JE SHORT MHOClien.0046C3C9
	$+16     0046C3BD      E8 7E630D00     CALL MHOClien.00542740
	$+1B     0046C3C2      A3 B85C5802     MOV DWORD PTR DS:[0x2585CB8],EAX
	$+20     0046C3C7      EB 06           JMP SHORT MHOClien.0046C3CF

	// 先遣
	$ ==>    0C68A694       391D 6C21C40D        CMP DWORD PTR DS:[0xDC4216C],EBX
	$+6      0C68A69A       75 22                JNZ SHORT CryGame.0C68A6BE
	$+8      0C68A69C       6A 50                PUSH 0x50
	$+A      0C68A69E       E8 CDC43700          CALL CryGame.0CA06B70
	$+F      0C68A6A3       83C4 04              ADD ESP,0x4
	$+12     0C68A6A6       3BC3                 CMP EAX,EBX
	$+14     0C68A6A8       74 0E                JE SHORT CryGame.0C68A6B8
	$+16     0C68A6AA       8BF0                 MOV ESI,EAX
	$+18     0C68A6AC       E8 DFF12700          CALL CryGame.0C909890
	$+1D     0C68A6B1       A3 6C21C40D          MOV DWORD PTR DS:[0xDC4216C],EAX
	$+22     0C68A6B6       EB 06                JMP SHORT CryGame.0C68A6BE
	$+24     0C68A6B8       891D 6C21C40D        MOV DWORD PTR DS:[0xDC4216C],EBX

	39??????????75??6A50E8????????83C4043BC374
	--*/
	DWORD	dwMMOClientUseItemObjAddr = *scan_game_.ScanImme<DWORD>("39??????????75??6A50E8????????83C4043BC374", 0x02);
	assert(dwMMOClientUseItemObjAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_USE_ITEM_OBJ_ADDR  " << dwMMOClientUseItemObjAddr - GetModuleCryGameBase() << "  // " << dwMMOClientUseItemObjAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-22     >  68 0CB2FE01     PUSH MHOClien.01FEB20C                   ; ASCII "DeathMatch"
	$-1D     >  52              PUSH EDX
	$-1C     >  894C24 68       MOV DWORD PTR SS:[ESP+0x68],ECX
	$-18     >  E8 B980EAFF     CALL MHOClien.004DD000
	$-13     >  8B4424 3C       MOV EAX,DWORD PTR SS:[ESP+0x3C]
	$-F      >  8B40 18         MOV EAX,DWORD PTR DS:[EAX+0x18]
	$-C      >  8B40 44         MOV EAX,DWORD PTR DS:[EAX+0x44]
	$-9      >  50              PUSH EAX
	$-8      >  8D8C24 90000000 LEA ECX,DWORD PTR SS:[ESP+0x90]
	$-1      >  51              PUSH ECX
	$ ==>    >  E8 A180EAFF     CALL MHOClien.004DD000
	$+5      >  83C4 10         ADD ESP,0x10
	$+8      >  8D5424 60       LEA EDX,DWORD PTR SS:[ESP+0x60]
	$+C      >  52              PUSH EDX
	$+D      >  E8 44B90000     CALL MHOClien.006408B0                   ; 城镇副本加载函数
	$+12     >  8B06            MOV EAX,DWORD PTR DS:[ESI]
	$+14     >  8B48 3C         MOV ECX,DWORD PTR DS:[EAX+0x3C]
	$+17     >  8B01            MOV EAX,DWORD PTR DS:[ECX]
	$+19     >  8B50 24         MOV EDX,DWORD PTR DS:[EAX+0x24]
	$+1C     >  FFD2            CALL EDX

	E8????????83????8D????????E8????????8B??8B????8B??8B????FF

	$-21     0D10C09A       68 F8309C0D          PUSH CryGame.0D9C30F8                                ; ASCII "DeathMatch"
	$-1C     0D10C09F       8D55 BC              LEA EDX,DWORD PTR SS:[EBP-0x44]
	$-19     0D10C0A2       6A 20                PUSH 0x20
	$-17     0D10C0A4       52                   PUSH EDX
	$-16     0D10C0A5       894D B8              MOV DWORD PTR SS:[EBP-0x48],ECX
	$-13     0D10C0A8       E8 73BF28FF          CALL CryGame.0C398020
	$-E      0D10C0AD       8B45 90              MOV EAX,DWORD PTR SS:[EBP-0x70]
	$-B      0D10C0B0       8B48 18              MOV ECX,DWORD PTR DS:[EAX+0x18]
	$-8      0D10C0B3       8B51 44              MOV EDX,DWORD PTR DS:[ECX+0x44]
	$-5      0D10C0B6       52                   PUSH EDX
	$-4      0D10C0B7       8D45 DC              LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-1      0D10C0BA       50                   PUSH EAX
	$ ==>    0D10C0BB       E8 C0BE62FF          CALL CryGame.0C737F80
	$+5      0D10C0C0       83C4 14              ADD ESP,0x14
	$+8      0D10C0C3       8D4D B8              LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+B      0D10C0C6       51                   PUSH ECX
	$+C      0D10C0C7       8BCF                 MOV ECX,EDI
	$+E      0D10C0C9       E8 1202FFFF          CALL CryGame.0D0FC2E0                                ; 加载城镇地图函数
	$+13     0D10C0CE       8BCF                 MOV ECX,EDI
	$+15     0D10C0D0       E8 7BAAF4FF          CALL CryGame.0D056B50
	$+1A     0D10C0D5       8B10                 MOV EDX,DWORD PTR DS:[EAX]			// hook
	$+1C     0D10C0D7       8BC8                 MOV ECX,EAX
	$+1E     0D10C0D9       8B42 24              MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+21     0D10C0DC       FFD0                 CALL EAX
	$+23     0D10C0DE       33DB                 XOR EBX,EBX
	$+25     0D10C0E0       3BC3                 CMP EAX,EBX
	$+27     0D10C0E2       74 10                JE SHORT CryGame.0D10C0F4
	$+29     0D10C0E4       0FB74E 24            MOVZX ECX,WORD PTR DS:[ESI+0x24]
	$+2D     0D10C0E8       8988 6C420000        MOV DWORD PTR DS:[EAX+0x426C],ECX                    ; 写入线路序号
	$+33     0D10C0EE       8898 70420000        MOV BYTE PTR DS:[EAX+0x4270],BL

	E8????????83????8D??????8B??E8????????8B??E8????????8B??8B??8B????FF
	--*/
	DWORD	dwMMOClientLoadmapEndHook = (DWORD)scan_game_.ScanAddr("E8????????83????8D??????8B??E8????????8B??E8????????8B??8B??8B????FF", 0x1A);
	assert(dwMMOClientLoadmapEndHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK  " << dwMMOClientLoadmapEndHook - GetModuleCryGameBase() << "  // " << dwMMOClientLoadmapEndHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++

	$-15     >  68 0CB2FE01            PUSH MHOClien.01FEB20C                   ; ASCII "DeathMatch"
	$-10     >  33FF                   XOR EDI,EDI
	$-E      >  50                     PUSH EAX
	$-D      >  897C24 60              MOV DWORD PTR SS:[ESP+0x60],EDI
	$-9      >  E8 1283EAFF            CALL MHOClien.004DD000
	$-4      >  8B4C24 1C              MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$ ==>    >  8B51 18                MOV EDX,DWORD PTR DS:[ECX+0x18]
	$+3      >  8B42 44                MOV EAX,DWORD PTR DS:[EDX+0x44]
	$+6      >  50                     PUSH EAX
	$+7      >  8D8C24 88000000        LEA ECX,DWORD PTR SS:[ESP+0x88]
	$+E      >  51                     PUSH ECX
	$+F      >  E8 FA82EAFF            CALL MHOClien.004DD000
	$+14     >  83C4 10                ADD ESP,0x10
	$+17     >  8D5424 58              LEA EDX,DWORD PTR SS:[ESP+0x58]
	$+1B     >  52                     PUSH EDX
	$+1C     >  E8 9DBB0000            CALL MHOClien.006408B0                   ; 副本地图加载函数
	$+21     >  8B0E                   MOV ECX,DWORD PTR DS:[ESI]
	$+23     >  897C24 48              MOV DWORD PTR SS:[ESP+0x48],EDI

	8B????8B??????8D??????????????E8????????83????8D????????E8????????8B??89

	$ ==>    0D10BE2E       8B51 18              MOV EDX,DWORD PTR DS:[ECX+0x18]
	$+3      0D10BE31       8B42 44              MOV EAX,DWORD PTR DS:[EDX+0x44]
	$+6      0D10BE34       50                   PUSH EAX
	$+7      0D10BE35       8D4D DC              LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+A      0D10BE38       51                   PUSH ECX
	$+B      0D10BE39       E8 42C162FF          CALL CryGame.0C737F80
	$+10     0D10BE3E       83C4 14              ADD ESP,0x14
	$+13     0D10BE41       8D55 B8              LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+16     0D10BE44       52                   PUSH EDX
	$+17     0D10BE45       8BCB                 MOV ECX,EBX
	$+19     0D10BE47       E8 9404FFFF          CALL CryGame.0D0FC2E0                                ; 加载进入副本的地图
	$+1E     0D10BE4C       8BCB                 MOV ECX,EBX
	$+20     0D10BE4E       89B5 7CFFFFFF        MOV DWORD PTR SS:[EBP-0x84],ESI

	8B????8B??????8D??????E8????????83????8D??????8B??E8????????8B??89

	--*/
	DWORD	dwMMOClientLoadmapEndHook2 = (DWORD)scan_game_.ScanAddr("8B????8B??????8D??????E8????????83????8D??????8B??E8????????8B??89", 0x1E);
	assert(dwMMOClientLoadmapEndHook2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK2  " << dwMMOClientLoadmapEndHook2 - GetModuleCryGameBase() << "  // " << dwMMOClientLoadmapEndHook2 << " CryGame.dll" << FC_OFstream_EndlLog;


	/*



	$-CF     >  55              push ebp
	$-CE     >  8BEC            mov ebp,esp
	$-CC     >  83EC 30         sub esp,0x30
	$-C9     >  53              push ebx
	$-C8     >  56              push esi
	$-C7     >  8B75 08         mov esi,dword ptr ss:[ebp+0x8]
	$-C4     >  57              push edi
	$-C3     >  33FF            xor edi,edi
	$-C1     >  33DB            xor ebx,ebx
	$-BF     >  897D E8         mov dword ptr ss:[ebp-0x18],edi
	$-BC     >  897D EC         mov dword ptr ss:[ebp-0x14],edi
	$-B9     >  897D F0         mov dword ptr ss:[ebp-0x10],edi
	$-B6     >  385E 10         cmp byte ptr ds:[esi+0x10],bl
	$-B3     >  0F86 A2000000   jbe CryGame.0B1CEE34
	$-AD     >  83C6 11         add esi,0x11
	$-AA     >  8A46 04         mov al,byte ptr ds:[esi+0x4]
	$-A7     >  3C 01           cmp al,0x1
	$-A5     >  75 0E           jnz short CryGame.0B1CEDAA
	$-A3     >  8B06            mov eax,dword ptr ds:[esi]
	$-A1     >  8B4E 05         mov ecx,dword ptr ds:[esi+0x5]
	$-9E     >  50              push eax
	$-9D     >  51              push ecx
	$-9C     >  E8 584B4CFF     call CryGame.0A693900
	$-97     >  EB 42           jmp short CryGame.0B1CEDEC
	$-95     >  3C 02           cmp al,0x2
	$-93     >  75 15           jnz short CryGame.0B1CEDC3
	$-91     >  8B16            mov edx,dword ptr ds:[esi]
	$-8F     >  8B46 09         mov eax,dword ptr ds:[esi+0x9]
	$-8C     >  8B4E 05         mov ecx,dword ptr ds:[esi+0x5]
	$-89     >  52              push edx
	$-88     >  50              push eax
	$-87     >  51              push ecx
	$-86     >  E8 B24F4CFF     call CryGame.0A693D70
	$-81     >  83C4 0C         add esp,0xC
	$-7E     >  EB 2C           jmp short CryGame.0B1CEDEF
	$-7C     >  3C 03           cmp al,0x3
	$-7A     >  75 11           jnz short CryGame.0B1CEDD8
	$-78     >  8B16            mov edx,dword ptr ds:[esi]
	$-76     >  D946 05         fld dword ptr ds:[esi+0x5]
	$-73     >  52              push edx
	$-72     >  51              push ecx
	$-71     >  D91C24          fstp dword ptr ss:[esp]
	$-6E     >  E8 5A4C4CFF     call CryGame.0A693A30
	$-69     >  EB 14           jmp short CryGame.0B1CEDEC
	$-67     >  3C 04           cmp al,0x4
	$-65     >  0F85 93010000   jnz CryGame.0B1CEF73
	$-5F     >  8B06            mov eax,dword ptr ds:[esi]
	$-5D     >  50              push eax
	$-5C     >  8D4E 05         lea ecx,dword ptr ds:[esi+0x5]
	$-59     >  51              push ecx
	$-58     >  E8 F44B4CFF     call CryGame.0A6939E0
	$-53     >  83C4 08         add esp,0x8
	$-50     >  8945 F8         mov dword ptr ss:[ebp-0x8],eax
	$-4D     >  85C0            test eax,eax
	$-4B     >  0F84 79010000   je CryGame.0B1CEF73
	$-45     >  3B7D F0         cmp edi,dword ptr ss:[ebp-0x10]
	$-42     >  74 0A           je short CryGame.0B1CEE09
	$-40     >  8907            mov dword ptr ds:[edi],eax
	$-3E     >  83C7 04         add edi,0x4
	$-3B     >  897D EC         mov dword ptr ss:[ebp-0x14],edi
	$-38     >  EB 18           jmp short CryGame.0B1CEE21
	$-36     >  6A 01           push 0x1
	$-34     >  6A 01           push 0x1
	$-32     >  8D55 0B         lea edx,dword ptr ss:[ebp+0xB]
	$-2F     >  52              push edx
	$-2E     >  8D45 F8         lea eax,dword ptr ss:[ebp-0x8]
	$-2B     >  50              push eax
	$-2A     >  57              push edi
	$-29     >  8D4D E8         lea ecx,dword ptr ss:[ebp-0x18]
	$-26     >  E8 C2EC7DFF     call CryGame.0A9ADAE0
	$-21     >  8B7D EC         mov edi,dword ptr ss:[ebp-0x14]
	$-1E     >  8B4D 08         mov ecx,dword ptr ss:[ebp+0x8]
	$-1B     >  0FB651 10       movzx edx,byte ptr ds:[ecx+0x10]
	$-17     >  43              inc ebx
	$-16     >  83C6 45         add esi,0x45
	$-13     >  3BDA            cmp ebx,edx
	$-11     >^ 0F82 61FFFFFF   jb CryGame.0B1CED95
	$-B      >  8D4D FC         lea ecx,dword ptr ss:[ebp-0x4]
	$-8      >  E8 A45B1FFF     call CryGame.0A3C49E0
	$-3      >  8B55 E8         mov edx,dword ptr ss:[ebp-0x18]
	$ ==>    >  8BDF            mov ebx,edi
	$+2      >  2BDA            sub ebx,edx
	$+4      >  83EC 0C         sub esp,0xC
	$+7      >  33C0            xor eax,eax
	$+9      >  8BF4            mov esi,esp
	$+B      >  8BCB            mov ecx,ebx
	$+D      >  C1F9 02         sar ecx,0x2
	$+10     >  8906            mov dword ptr ds:[esi],eax
	$+12     >  8946 04         mov dword ptr ds:[esi+0x4],eax
	$+15     >  8946 08         mov dword ptr ds:[esi+0x8],eax
	$+18     >  81F9 FFFFFF3F   cmp ecx,0x3FFFFFFF
	$+1E     >  76 14           jbe short CryGame.0B1CEE73
	$+20     >  68 10A6BB0B     push CryGame.0BBBA610                                                ; ASCII "out of memory\n"
	$+25     >  E8 05596A00     call <jmp.&MSVCR90.puts>
	$+2A     >  83C4 04         add esp,0x4
	$+2D     >  6A 01           push 0x1
	$+2F     >  E8 79576A00     call <jmp.&MSVCR90.exit>
	$+34     >  3BC8            cmp ecx,eax
	$+36     >  74 35           je short CryGame.0B1CEEAC
	$+38     >  8D3C8D 00000000 lea edi,dword ptr ds:[ecx*4]
	$+3F     >  57              push edi
	$+40     >  E8 BCF68FFF     call CryGame.0AACE540
	$+45     >  83C4 04         add esp,0x4
	$+48     >  85C0            test eax,eax
	$+4A     >  75 14           jnz short CryGame.0B1CEE9F
	$+4C     >  68 10A6BB0B     push CryGame.0BBBA610                                                ; ASCII "out of memory\n"
	$+51     >  E8 D9586A00     call <jmp.&MSVCR90.puts>
	$+56     >  83C4 04         add esp,0x4
	$+59     >  6A 01           push 0x1
	$+5B     >  E8 4D576A00     call <jmp.&MSVCR90.exit>
	$+60     >  8B55 E8         mov edx,dword ptr ss:[ebp-0x18]
	$+63     >  C1EF 02         shr edi,0x2
	$+66     >  8BCF            mov ecx,edi
	$+68     >  8B7D EC         mov edi,dword ptr ss:[ebp-0x14]
	$+6B     >  EB 02           jmp short CryGame.0B1CEEAE
	$+6D     >  33C0            xor eax,eax
	$+6F     >  8D0C88          lea ecx,dword ptr ds:[eax+ecx*4]
	$+72     >  8906            mov dword ptr ds:[esi],eax
	$+74     >  8946 04         mov dword ptr ds:[esi+0x4],eax
	$+77     >  894E 08         mov dword ptr ds:[esi+0x8],ecx
	$+7A     >  3BFA            cmp edi,edx
	$+7C     >  74 0D           je short CryGame.0B1CEECA
	$+7E     >  53              push ebx
	$+7F     >  52              push edx
	$+80     >  50              push eax
	$+81     >  E8 EB556A00     call <jmp.&MSVCR90.memcpy>
	$+86     >  83C4 0C         add esp,0xC
	$+89     >  03C3            add eax,ebx
	$+8B     >  8946 04         mov dword ptr ds:[esi+0x4],eax
	$+8E     >  8B75 08         mov esi,dword ptr ss:[ebp+0x8]
	$+91     >  8B16            mov edx,dword ptr ds:[esi]
	$+93     >  52              push edx
	$+94     >  8D45 F4         lea eax,dword ptr ss:[ebp-0xC]
	$+97     >  50              push eax
	$+98     >  8D4D FC         lea ecx,dword ptr ss:[ebp-0x4]
	$+9B     >  51              push ecx
	$+9C     >  E8 104F4CFF     call CryGame.0A693DF0
	$+A1     >  83C4 18         add esp,0x18
	$+A4     >  84C0            test al,al
	$+A6     >  0F84 80000000   je CryGame.0B1CEF6B
	$+AC     >  8B36            mov esi,dword ptr ds:[esi]
	$+AE     >  6A 00           push 0x0
	$+B0     >  68 9C71BD0B     push CryGame.0BBD719C                                                ; ASCII 43,"MessageInfo"
	$+B5     >  6A 00           push 0x0
	$+B7     >  E8 E57F4CFF     call CryGame.0A696EE0
	$+BC     >  83C4 0C         add esp,0xC
	$+BF     >  8BC8            mov ecx,eax
	$+C1     >  83FE FF         cmp esi,-0x1
	$+C4     >  74 66           je short CryGame.0B1CEF6B
	$+C6     >  85F6            test esi,esi
	$+C8     >  75 05           jnz short CryGame.0B1CEF0E
	$+CA     >  3971 30         cmp dword ptr ds:[ecx+0x30],esi
	$+CD     >  75 5D           jnz short CryGame.0B1CEF6B
	$+CF     >  8379 28 00      cmp dword ptr ds:[ecx+0x28],0x0
	$+D3     >  74 57           je short CryGame.0B1CEF6B
	$+D5     >  2B71 30         sub esi,dword ptr ds:[ecx+0x30]
	$+D8     >  78 52           js short CryGame.0B1CEF6B
	$+DA     >  3B71 24         cmp esi,dword ptr ds:[ecx+0x24]
	$+DD     >  7D 4D           jge short CryGame.0B1CEF6B
	$+DF     >  8B79 28         mov edi,dword ptr ds:[ecx+0x28]
	$+E2     >  8BC6            mov eax,esi
	$+E4     >  99              cdq
	$+E5     >  F7FF            idiv edi
	$+E7     >  8B49 20         mov ecx,dword ptr ds:[ecx+0x20]
	$+EA     >  833C81 00       cmp dword ptr ds:[ecx+eax*4],0x0
	$+EE     >  8D0481          lea eax,dword ptr ds:[ecx+eax*4]
	$+F1     >  74 39           je short CryGame.0B1CEF6B
	$+F3     >  8B00            mov eax,dword ptr ds:[eax]
	$+F5     >  8B3490          mov esi,dword ptr ds:[eax+edx*4]
	$+F8     >  85F6            test esi,esi
	$+FA     >  74 30           je short CryGame.0B1CEF6B
	$+FC     >  8D4D D0         lea ecx,dword ptr ss:[ebp-0x30]
	$+FF     >  51              push ecx
	$+100    >  8D4D FC         lea ecx,dword ptr ss:[ebp-0x4]
	$+103    >  E8 495F1FFF     call CryGame.0A3C4E90
	$+108    >  8B50 14         mov edx,dword ptr ds:[eax+0x14]
	$+10B    >  52              push edx
	$+10C    >  56              push esi
	$+10D    >  E8 CF33F1FF     call CryGame.0B0E2320
	$+112    >  8B45 E4         mov eax,dword ptr ss:[ebp-0x1C]
	$+115    >  8D4D D0         lea ecx,dword ptr ss:[ebp-0x30]
	$+118    >  83C4 08         add esp,0x8
	$+11B    >  3BC1            cmp eax,ecx
	$+11D    >  74 0D           je short CryGame.0B1CEF6B
	$+11F    >  85C0            test eax,eax
	$+121    >  74 09           je short CryGame.0B1CEF6B
	$+123    >  50              push eax
	$+124    >  E8 F8F58FFF     call CryGame.0AACE560
	$+129    >  83C4 04         add esp,0x4
	$+12C    >  8D4D FC         lea ecx,dword ptr ss:[ebp-0x4]
	$+12F    >  E8 7D5B1FFF     call CryGame.0A3C4AF0
	$+134    >  8B45 E8         mov eax,dword ptr ss:[ebp-0x18]
	$+137    >  85C0            test eax,eax
	$+139    >  74 09           je short CryGame.0B1CEF83
	$+13B    >  50              push eax
	$+13C    >  E8 E0F58FFF     call CryGame.0AACE560
	$+141    >  83C4 04         add esp,0x4
	$+144    >  5F              pop edi
	$+145    >  5E              pop esi
	$+146    >  5B              pop ebx
	$+147    >  8BE5            mov esp,ebp
	$+149    >  5D              pop ebp
	$+14A    >  C2 0400         retn 0x4

	8B DF 2B DA 83 EC 0C 33 C0 8B F4 8B CB C1 F9 02  -0xCF

	*/




	DWORD	dwMMOHIGHIGHTHook2 = (DWORD)scan_game_.ScanAddr("8BDF2BDA83EC0C33C08BF48BCBC1F902", -0xCE);
	assert(dwMMOHIGHIGHTHook2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIGHIGHT_HOOK  " << dwMMOHIGHIGHTHook2 - GetModuleCryGameBase() << "  // " << dwMMOHIGHIGHTHook2 << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	邮件
	$ ==>    >  8B4C24 24           MOV ECX,DWORD PTR SS:[ESP+0x24]
	$+4      >  C1E9 06             SHR ECX,0x6
	$+7      >  F6C1 01             TEST CL,0x1
	$+A      >  E9 9D050000         JMP CryGame.0C89E725
	$+F      >  8B80 50200000       MOV EAX,DWORD PTR DS:[EAX+0x2050]        ; +0x2050
	$+15     >  894424 14           MOV DWORD PTR SS:[ESP+0x14],EAX
	$+19     >  3BC6                CMP EAX,ESI
	$+1B     >  8B4424 24           MOV EAX,DWORD PTR SS:[ESP+0x24]
	$+1F     >  0F84 82050000       JE CryGame.0C89E720
	$+25     >  25 8F000000         AND EAX,0x8F
	$+2A     >  33FF                XOR EDI,EDI
	$+2C     >  3C 0A               CMP AL,0xA
	$+2E     >  0F94C1              SETE CL
	$+31     >  884C24 18           MOV BYTE PTR SS:[ESP+0x18],CL

	8B??????C1????F6????E9????????8B??????????89

	//zhp,2016/5/16 16:53 上面的特征码可以定位到3份，每份都不大一样，具体得到游戏里面测试才知道
	//现已废弃不用，2016/5/26 15:07
	--*/
	DWORD	dwMMOClientMailOffset1 = *scan_game_.ScanImme<DWORD>("8B??????C1????F6????E9????????8B??????????89", 0x11);
	assert(dwMMOClientMailOffset1);
	//assert(dwMMOClientMailOffset1==0x22a0/*0x21F0*//*0x00002158*//*0x000020F4*/  /*0x00002054*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MAIL_OFFSET1  " << dwMMOClientMailOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    005AE608      A1 985F5802          MOV EAX,DWORD PTR DS:[0x2585F98]
	$+5      005AE60D      8B48 74              MOV ECX,DWORD PTR DS:[EAX+0x74]
	$+8      005AE610      8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+A      005AE612      8B42 14              MOV EAX,DWORD PTR DS:[EDX+0x14]
	$+D      005AE615      57                   PUSH EDI                                 ; 采集物品id
	$+E      005AE616      FFD0                 CALL EAX                                 ; eax
	$+10     005AE618      85C0                 TEST EAX,EAX                             ; [eax+0x15c] == 采集物品封包内的数据
	$+12     005AE61A      75 0A                JNZ SHORT MHOClien.005AE626

	A1????????8B????8B??8B????57FFD085C075
	--*/
	// 	DWORD	dwMMOClientCollectVfunAddrOffset1 = ScanImme((HANDLE)-1,"A1????????8B????8B??8B????57FFD085C075",0x07,1);
	// 	ASSERT_GWLR(dwMMOClientCollectVfunAddrOffset1);
	// 	LOG << "#define UPDATES_";
	// 	LOG << "MMOCLIENT_REVERSAL_COLLECT_VFUN_ADDR_OFFSET1  ";
	// 	LOG << dwMMOClientCollectVfunAddrOffset1;
	// 	LOG_END;
	// 
	// 	DWORD	dwMMOClientCollectVfunAddrOffset2 = ScanImme((HANDLE)-1,"A1????????8B????8B??8B????57FFD085C075",0x0C,1);
	// 	ASSERT_GWLR(dwMMOClientCollectVfunAddrOffset2);
	// 	LOG << "#define UPDATES_";
	// 	LOG << "MMOCLIENT_REVERSAL_COLLECT_VFUN_ADDR_OFFSET2  ";
	// 	LOG << dwMMOClientCollectVfunAddrOffset2;
	// 	LOG_END;

	/*++
	$ ==>    01516998      A1 0C595802          MOV EAX,DWORD PTR DS:[0x258590C]
	$+5      0151699D      85C0                 TEST EAX,EAX
	$+7      0151699F    ^ 0F84 F9F7FFFF        JE MHOClien.0151619E
	$+D      015169A5      83B8 90000000 00     CMP DWORD PTR DS:[EAX+0x90],0x0
	$+14     015169AC    ^ 0F84 ECF7FFFF        JE MHOClien.0151619E
	$+1A     015169B2      8B8C24 08010000      MOV ECX,DWORD PTR SS:[ESP+0x108]
	$+21     015169B9      8B80 90000000        MOV EAX,DWORD PTR DS:[EAX+0x90]
	$+27     015169BF      8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$+29     015169C1      8B52 10              MOV EDX,DWORD PTR DS:[EDX+0x10]
	$+2C     015169C4      6A 00                PUSH 0x0
	$+2E     015169C6      8BF1                 MOV ESI,ECX
	$+30     015169C8      6A 01                PUSH 0x1
	$+32     015169CA      0FB7C9               MOVZX ECX,CX
	$+35     015169CD      C1EE 10              SHR ESI,0x10
	$+38     015169D0      56                   PUSH ESI
	$+39     015169D1      51                   PUSH ECX
	$+3A     015169D2      8BC8                 MOV ECX,EAX
	$+3C     015169D4      FFD2                 CALL EDX                                 ; 鼠标按下

	A1????????85C00F84????????83B8??????????0F84????????8B8C24

	$ ==>    >  803D 58842802 00       CMP BYTE PTR DS:[0x2288458],0x0
	$+7      >  74 14                  JE SHORT MHOClien.013AC252
	$+9      >  8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+C      >  8B10                   MOV EDX,DWORD PTR DS:[EAX]
	$+E      >  8BC8                   MOV ECX,EAX
	$+10     >  8B82 00010000          MOV EAX,DWORD PTR DS:[EDX+0x100]
	$+16     >  FFD0                   CALL EAX
	$+18     >  A1 60DA4E02            MOV EAX,DWORD PTR DS:[0x24EDA60]
	$+1D     >  85C0                   TEST EAX,EAX
	$+1F     >  74 29                  JE SHORT MHOClien.013AC27F
	$+21     >  83B8 90000000 00       CMP DWORD PTR DS:[EAX+0x90],0x0
	$+28     >  74 20                  JE SHORT MHOClien.013AC27F
	$+2A     >  8B4D 14                MOV ECX,DWORD PTR SS:[EBP+0x14]
	$+2D     >  8B80 90000000          MOV EAX,DWORD PTR DS:[EAX+0x90]
	$+33     >  8B10                   MOV EDX,DWORD PTR DS:[EAX]
	$+35     >  8B52 10                MOV EDX,DWORD PTR DS:[EDX+0x10]
	$+38     >  6A 00                  PUSH 0x0
	$+3A     >  8BF1                   MOV ESI,ECX
	$+3C     >  6A 01                  PUSH 0x1
	$+3E     >  0FB7C9                 MOVZX ECX,CX
	$+41     >  C1EE 10                SHR ESI,0x10
	$+44     >  56                     PUSH ESI
	$+45     >  51                     PUSH ECX
	$+46     >  8BC8                   MOV ECX,EAX
	$+48     >  FFD2                   CALL EDX

	803D??????????74??8B????8B??8B??8B??????????FF??A1
	--*/
	DWORD	dwMMOClientGwlrWindowsBaseAddr = *scan_game_.ScanImme<DWORD>("803D??????????74??8B????8B??8B??8B??????????FF??A1", 0x19);
	assert(dwMMOClientGwlrWindowsBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GWLR_WINDOWS_BASE_ADDR  " << dwMMOClientGwlrWindowsBaseAddr - GetModuleCryGameBase() << "  // " << dwMMOClientGwlrWindowsBaseAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientMouseLbuttonDownOffset1 = *scan_game_.ScanImme<DWORD>("803D??????????74??8B????8B??8B??8B??????????FF??A1", 0x23);
	assert(dwMMOClientMouseLbuttonDownOffset1);
	assert(dwMMOClientMouseLbuttonDownOffset1 == 0x00000090);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MOUSE_LBUTTONDOWN_OFFSET1  " << dwMMOClientMouseLbuttonDownOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00717996      8B4E 30              MOV ECX,DWORD PTR DS:[ESI+0x30]
	$+3      00717999      8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+5      0071799B      8B52 10              MOV EDX,DWORD PTR DS:[EDX+0x10]
	$+8      0071799E      8D4424 5C            LEA EAX,DWORD PTR SS:[ESP+0x5C]
	$+C      007179A2      50                   PUSH EAX
	$+D      007179A3      FFD2                 CALL EDX
	$+F      007179A5      8D4C24 5C            LEA ECX,DWORD PTR SS:[ESP+0x5C]
	$+13     007179A9      E8 92ADFAFF          CALL MHOClien.006C2740
	$+18     007179AE      8D4C24 44            LEA ECX,DWORD PTR SS:[ESP+0x44]
	$+1C     007179B2      E8 39D1CEFF          CALL MHOClien.00404AF0
	$+21     007179B7      8D4C24 2C            LEA ECX,DWORD PTR SS:[ESP+0x2C]
	$+25     007179BB      E8 30D1CEFF          CALL MHOClien.00404AF0
	$+2A     007179C0      8B8C24 84040000      MOV ECX,DWORD PTR SS:[ESP+0x484]
	$+31     007179C7      5F                   POP EDI
	$+32     007179C8      5E                   POP ESI
	$+33     007179C9      5B                   POP EBX
	$+34     007179CA      33CC                 XOR ECX,ESP
	$+36     007179CC      E8 53CE5D01          CALL MHOClien.01CF4824
	$+3B     007179D1      8BE5                 MOV ESP,EBP                              ; hook
	$+3D     007179D3      5D                   POP EBP
	$+3E     007179D4      C2 0C00              RETN 0xC

	8B????8B??8B????8D??????50FF??8D??????E8????????8D??????E8

	$+3      >  8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+5      >  8B40 14             MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+8      >  8D95 A8FBFFFF       LEA EDX,DWORD PTR SS:[EBP-0x458]
	$+E      >  52                  PUSH EDX
	$+F      >  FFD0                CALL EAX
	$+11     >  8D8D A8FBFFFF       LEA ECX,DWORD PTR SS:[EBP-0x458]
	$+17     >  E8 62808EFF         CALL CryGame.0C9F1BF0
	$+1C     >  8D8D CCFBFFFF       LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+22     >  E8 C77626FF         CALL CryGame.0C371260
	$+27     >  8B85 F8FBFFFF       MOV EAX,DWORD PTR SS:[EBP-0x408]
	$+2D     >  8D8D E4FBFFFF       LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+33     >  3BC1                CMP EAX,ECX
	$+35     >  74 0D               JE SHORT CryGame.0D109BB6
	$+37     >  85C0                TEST EAX,EAX
	$+39     >  74 09               JE SHORT CryGame.0D109BB6
	$+3B     >  50                  PUSH EAX
	$+3C     >  E8 DDCF8FFF         CALL CryGame.0CA06B90
	$+41     >  83C4 04             ADD ESP,0x4
	$+44     >  5F                  POP EDI
	$+45     >  8B4D FC             MOV ECX,DWORD PTR SS:[EBP-0x4]
	$+48     >  5E                  POP ESI
	$+49     >  33CD                XOR ECX,EBP
	$+4B     >  5B                  POP EBX
	$+4C     >  E8 3C406200         CALL CryGame.0D72DBFF
	$+51     >  8BE5                MOV ESP,EBP
	$+53     >  5D                  POP EBP
	$+54     >  C2 0C00             RETN 0xC

	8B??8B????8D????????????FF??8D??????????E8????????8D
	--*/
	DWORD	dwMMOClientFbGameOverHook = (DWORD)scan_game_.ScanAddr("8B??8B????8D????????????FF??8D??????????E8????????8D", 0x51);
	assert(dwMMOClientFbGameOverHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FB_GAME_OVER_HOOK  " << dwMMOClientFbGameOverHook - GetModuleCryGameBase() << "  // " << dwMMOClientFbGameOverHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    >  83E8 02         SUB EAX,0x2
	$+3      >  F7D8            NEG EAX
	$+5      >  1BC0            SBB EAX,EAX
	$+7      >  8D8D FCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x404]
	$+D      >  83C0 02         ADD EAX,0x2
	$+10     >  68 AC28A00D     PUSH CryGame.0DA028AC                    ; 战斗结束, 你胜利了
	$+15     >  51              PUSH ECX
	$+16     >  8985 98FBFFFF   MOV DWORD PTR SS:[EBP-0x468],EAX
	$+1C     >  E8 D01D29FF     CALL CryGame.0C39BB60
	$+21     >  83C4 08         ADD ESP,0x8
	$+24     >  8D95 A7FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x459]
	$+2A     >  52              PUSH EDX
	$+2B     >  68 270FA60D     PUSH CryGame.0DA60F27
	$+30     >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+36     >  E8 068B26FF     CALL CryGame.0C3728B0
	$+3B     >  8D85 A7FBFFFF   LEA EAX,DWORD PTR SS:[EBP-0x459]
	$+41     >  50              PUSH EAX
	$+42     >  8D8D FCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x404]
	$+48     >  51              PUSH ECX
	$+49     >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+4F     >  E8 ED8A26FF     CALL CryGame.0C3728B0
	$+54     >  83C6 08         ADD ESI,0x8
	$+57     >  8BCE            MOV ECX,ESI
	$+59     >  E8 53D3F4FF     CALL CryGame.0D057120
	$+5E     >  6A 00           PUSH 0x0
	$+60     >  6A 00           PUSH 0x0
	$+62     >  6A 00           PUSH 0x0
	$+64     >  6A FF           PUSH -0x1
	$+66     >  6A 00           PUSH 0x0
	$+68     >  6A 00           PUSH 0x0
	$+6A     >  6A 01           PUSH 0x1
	$+6C     >  6A 00           PUSH 0x0
	$+6E     >  8D95 E4FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x41C]
	$+74     >  52              PUSH EDX
	$+75     >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+7B     >  51              PUSH ECX
	$+7C     >  8B88 8C000000   MOV ECX,DWORD PTR DS:[EAX+0x8C]
	$+82     >  6A 08           PUSH 0x8
	$+84     >  E8 3841F2FF     CALL CryGame.0D02DF30
	$+89     >  8B85 E0FBFFFF   MOV EAX,DWORD PTR SS:[EBP-0x420]
	$+8F     >  8D95 CCFBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x434]
	$+95     >  3BC2            CMP EAX,EDX
	$+97     >  74 0D           JE SHORT CryGame.0D109E15
	$+99     >  85C0            TEST EAX,EAX
	$+9B     >  74 09           JE SHORT CryGame.0D109E15
	$+9D     >  50              PUSH EAX
	$+9E     >  E8 AED38FFF     CALL CryGame.0CA071C0
	$+A3     >  83C4 04         ADD ESP,0x4
	$+A6     >  8B85 F8FBFFFF   MOV EAX,DWORD PTR SS:[EBP-0x408]
	$+AC     >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+B2     >  3BC1            CMP EAX,ECX
	$+B4     >  74 0D           JE SHORT CryGame.0D109E32
	$+B6     >  85C0            TEST EAX,EAX
	$+B8     >  74 09           JE SHORT CryGame.0D109E32
	$+BA     >  50              PUSH EAX
	$+BB     >  E8 91D38FFF     CALL CryGame.0CA071C0
	$+C0     >  83C4 04         ADD ESP,0x4
	$+C3     >  8B95 A0FBFFFF   MOV EDX,DWORD PTR SS:[EBP-0x460]
	$+C9     >  8B4A 24         MOV ECX,DWORD PTR DS:[EDX+0x24]
	$+CC     >  83C8 FF         OR EAX,0xFFFFFFFF
	$+CF     >  8985 B0FBFFFF   MOV DWORD PTR SS:[EBP-0x450],EAX
	$+D5     >  8985 B4FBFFFF   MOV DWORD PTR SS:[EBP-0x44C],EAX
	$+DB     >  C785 ACFBFFFF 8>MOV DWORD PTR SS:[EBP-0x454],0x581
	$+E5     >  8B01            MOV EAX,DWORD PTR DS:[ECX]
	$+E7     >  8B40 14         MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+EA     >  8D95 ACFBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x454]
	$+F0     >  52              PUSH EDX
	$+F1     >  FFD0            CALL EAX
	$+F3     >  C685 8CFBFFFF 0>MOV BYTE PTR SS:[EBP-0x474],0x1
	$+FA     >  E9 EE000000     JMP CryGame.0D109F5C
	$+FF     >  3943 14         CMP DWORD PTR DS:[EBX+0x14],EAX
	$+102    >  75 19           JNZ SHORT CryGame.0D109E8C
	$+104    >  8B17            MOV EDX,DWORD PTR DS:[EDI]
	$+106    >  8B42 44         MOV EAX,DWORD PTR DS:[EDX+0x44]
	$+109    >  8BCF            MOV ECX,EDI
	$+10B    >  FFD0            CALL EAX
	$+10D    >  48              DEC EAX
	$+10E    >  F7D8            NEG EAX
	$+110    >  1BC0            SBB EAX,EAX
	$+112    >  83C0 02         ADD EAX,0x2
	$+115    >  8985 98FBFFFF   MOV DWORD PTR SS:[EBP-0x468],EAX
	$+11B    >  EB 0A           JMP SHORT CryGame.0D109E96
	$+11D    >  C785 98FBFFFF 0>MOV DWORD PTR SS:[EBP-0x468],0x0
	$+127    >  8D8D FCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x404]
	$+12D    >  68 9828A00D     PUSH CryGame.0DA02898                    ; 战斗结束, 你失败了
	$+132    >  51              PUSH ECX
	$+133    >  E8 B91C29FF     CALL CryGame.0C39BB60
	$+138    >  83C4 08         ADD ESP,0x8
	$+13B    >  8D95 A7FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x459]
	$+141    >  52              PUSH EDX

	//zhp
	$ ==>    >  83E8 02         SUB EAX,0x2
	$+3      >  F7D8            NEG EAX
	$+5      >  1BC0            SBB EAX,EAX
	$+7      >  8D8D FCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x404]
	$+D      >  83C0 02         ADD EAX,0x2
	$+10     >  68 A00EA70B     PUSH CryGame.0BA70EA0
	$+15     >  51              PUSH ECX
	$+16     >  8985 98FBFFFF   MOV DWORD PTR SS:[EBP-0x468],EAX
	$+1C     >  E8 A65B1CFF     CALL CryGame.0A2D3010
	$+21     >  83C4 08         ADD ESP,0x8
	$+24     >  8D95 A7FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x459]
	$+2A     >  52              PUSH EDX
	$+2B     >  68 4671AD0B     PUSH CryGame.0BAD7146
	$+30     >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+36     >  E8 3C5319FF     CALL CryGame.0A2A27C0
	$+3B     >  8D85 A7FBFFFF   LEA EAX,DWORD PTR SS:[EBP-0x459]
	$+41     >  50              PUSH EAX
	$+42     >  8D8D FCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x404]
	$+48     >  51              PUSH ECX
	$+49     >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+4F     >  E8 235319FF     CALL CryGame.0A2A27C0
	$+54     >  83C6 08         ADD ESI,0x8
	$+57     >  8BCE            MOV ECX,ESI
	$+59     >  E8 890DF3FF     CALL CryGame.0B03E230
	$+5E     >  6A 00           PUSH 0x0
	$+60     >  6A 00           PUSH 0x0
	$+62     >  6A 00           PUSH 0x0
	$+64     >  6A 00           PUSH 0x0
	$+66     >  6A 00           PUSH 0x0
	$+68     >  6A 00           PUSH 0x0
	$+6A     >  6A 00           PUSH 0x0
	$+6C     >  6A FF           PUSH -0x1
	$+6E     >  6A 00           PUSH 0x0
	$+70     >  6A 00           PUSH 0x0
	$+72     >  6A 01           PUSH 0x1
	$+74     >  6A 00           PUSH 0x0
	$+76     >  8D95 E4FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x41C]
	$+7C     >  52              PUSH EDX
	$+7D     >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+83     >  51              PUSH ECX
	$+84     >  8B88 8C000000   MOV ECX,DWORD PTR DS:[EAX+0x8C]
	$+8A     >  6A 08           PUSH 0x8
	$+8C     >  E8 A659F0FF     CALL CryGame.0B012E80
	$+91     >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+97     >  E8 7B3D19FF     CALL CryGame.0A2A1260
	$+9C     >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+A2     >  E8 703D19FF     CALL CryGame.0A2A1260
	$+A7     >  8B4B 24         MOV ECX,DWORD PTR DS:[EBX+0x24]
	$+AA     >  83C8 FF         OR EAX,0xFFFFFFFF
	$+AD     >  8985 B0FBFFFF   MOV DWORD PTR SS:[EBP-0x450],EAX
	$+B3     >  8985 B4FBFFFF   MOV DWORD PTR SS:[EBP-0x44C],EAX
	$+B9     >  C785 ACFBFFFF 8>MOV DWORD PTR SS:[EBP-0x454],0x581
	$+C3     >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+C5     >  8B52 14         MOV EDX,DWORD PTR DS:[EDX+0x14]
	$+C8     >  8D85 ACFBFFFF   LEA EAX,DWORD PTR SS:[EBP-0x454]
	$+CE     >  50              PUSH EAX
	$+CF     >  FFD2            CALL EDX
	$+D1     >  C685 8CFBFFFF 0>MOV BYTE PTR SS:[EBP-0x474],0x1
	$+D8     >  E9 B8010000     JMP CryGame.0B10D6DE
	$+DD     >  3948 14         CMP DWORD PTR DS:[EAX+0x14],ECX
	$+E0     >  0F85 DD000000   JNZ CryGame.0B10D60C
	$+E6     >  8B07            MOV EAX,DWORD PTR DS:[EDI]
	$+E8     >  8B50 44         MOV EDX,DWORD PTR DS:[EAX+0x44]
	$+EB     >  8BCF            MOV ECX,EDI
	$+ED     >  FFD2            CALL EDX
	$+EF     >  48              DEC EAX
	$+F0     >  F7D8            NEG EAX
	$+F2     >  1BC0            SBB EAX,EAX
	$+F4     >  83C0 02         ADD EAX,0x2
	$+F7     >  8985 98FBFFFF   MOV DWORD PTR SS:[EBP-0x468],EAX
	$+FD     >  8D85 FCFBFFFF   LEA EAX,DWORD PTR SS:[EBP-0x404]
	$+103    >  68 8C0EA70B     PUSH CryGame.0BA70E8C
	$+108    >  50              PUSH EAX
	$+109    >  E8 B95A1CFF     CALL CryGame.0A2D3010
	$+10E    >  83C4 08         ADD ESP,0x8
	$+111    >  8D8D A7FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x459]
	$+117    >  51              PUSH ECX
	$+118    >  68 4671AD0B     PUSH CryGame.0BAD7146
	$+11D    >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+123    >  E8 4F5219FF     CALL CryGame.0A2A27C0
	$+128    >  8D95 A7FBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x459]
	$+12E    >  52              PUSH EDX
	$+12F    >  8D85 FCFBFFFF   LEA EAX,DWORD PTR SS:[EBP-0x404]
	$+135    >  50              PUSH EAX
	$+136    >  8D8D CCFBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x434]
	$+13C    >  E8 365219FF     CALL CryGame.0A2A27C0
	$+141    >  83C6 08         ADD ESI,0x8					;战斗结束，你失败了。貌似这里才是。
	$+144    >  8BCE            MOV ECX,ESI
	$+146    >  E8 9C0CF3FF     CALL CryGame.0B03E230
	$+14B    >  6A 00           PUSH 0x0
	$+14D    >  6A 00           PUSH 0x0
	$+14F    >  6A 00           PUSH 0x0
	$+151    >  6A 00           PUSH 0x0
	$+153    >  6A 00           PUSH 0x0
	$+155    >  6A 00           PUSH 0x0
	$+157    >  6A 00           PUSH 0x0
	$+159    >  6A FF           PUSH -0x1
	$+15B    >  6A 00           PUSH 0x0
	$+15D    >  6A 00           PUSH 0x0
	$+15F    >  6A 01           PUSH 0x1
	$+161    >  6A 00           PUSH 0x0
	$+163    >  8D8D E4FBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x41C]
	$+169    >  51              PUSH ECX
	$+16A    >  8B88 8C000000   MOV ECX,DWORD PTR DS:[EAX+0x8C]
	$+170    >  8D95 CCFBFFFF   LEA EDX,DWORD PTR SS:[EBP-0x434]
	$+176    >  52              PUSH EDX
	$+177    >  6A 08           PUSH 0x8
	$+179    >  E8 B958F0FF     CALL CryGame.0B012E80


	83E8??F7D81BC08D??????????83????68
	--*/

	DWORD	dwMMOClientFbGameOverWinHook = (DWORD)scan_game_.ScanAddr("83E8??F7D81BC08D??????????83????68", 0x21);
	assert(dwMMOClientFbGameOverWinHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FB_GAME_OVER_WIN_HOOK  " << dwMMOClientFbGameOverWinHook - GetModuleCryGameBase() << "  // " << dwMMOClientFbGameOverWinHook << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientFbGameOverLoseHook = (DWORD)scan_game_.ScanAddr("83E8??F7D81BC08D??????????83????68", 0x141/*0x138*/);
	assert(dwMMOClientFbGameOverLoseHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FB_GAME_OVER_LOSE_HOOK  " << dwMMOClientFbGameOverLoseHook - GetModuleCryGameBase() << "  // " << dwMMOClientFbGameOverLoseHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00512B74      8B5424 20            MOV EDX,DWORD PTR SS:[ESP+0x20]
	$+4      00512B78      52                   PUSH EDX
	$+5      00512B79      E8 A252F6FF          CALL MHOClien.00477E20                   ; 返回商品的数组
	$+A      00512B7E      8BF8                 MOV EDI,EAX
	$+C      00512B80      83C4 04              ADD ESP,0x4
	$+F      00512B83      89BC24 CC000000      MOV DWORD PTR SS:[ESP+0xCC],EDI
	$+16     00512B8A      3BFE                 CMP EDI,ESI
	$+18     00512B8C      0F84 79080000        JE MHOClien.0051340B
	$+1E     00512B92      8B4424 48            MOV EAX,DWORD PTR SS:[ESP+0x48]
	$+22     00512B96      3BC6                 CMP EAX,ESI
	$+24     00512B98      897424 54            MOV DWORD PTR SS:[ESP+0x54],ESI
	$+28     00512B9C      897424 4C            MOV DWORD PTR SS:[ESP+0x4C],ESI
	$+2C     00512BA0      89B424 AC000000      MOV DWORD PTR SS:[ESP+0xAC],ESI
	$+33     00512BA7      7C 12                JL SHORT MHOClien.00512BBB

	8B????????E8????????8B??83????89????????????3B??0F

	$-28     >  68 98A6A80D     PUSH CryGame.0DA8A698                    ; ASCII "EXCHANGE_DATA_CLASS"
	$-23     >  50              PUSH EAX
	$-22     >  FFD2            CALL EDX
	$-20     >  84C0            TEST AL,AL
	$-1E     >  0F84 C20C0000   JE CryGame.0C8B7225
	$-18     >  8B8424 F4000000 MOV EAX,DWORD PTR SS:[ESP+0xF4]
	$-11     >  8BC8            MOV ECX,EAX
	$-F      >  81E1 8F000000   AND ECX,0x8F
	$-9      >  80F9 06         CMP CL,0x6
	$-6      >  0F85 AA0C0000   JNZ CryGame.0C8B7225
	$ ==>    >  C1E8 06         SHR EAX,0x6
	$+3      >  A8 01           TEST AL,0x1
	$+5      >  74 0F           JE SHORT CryGame.0C8B6591
	$+7      >  8B9424 F8000000 MOV EDX,DWORD PTR SS:[ESP+0xF8]
	$+E      >  8B02            MOV EAX,DWORD PTR DS:[EDX]
	$+10     >  894424 74       MOV DWORD PTR SS:[ESP+0x74],EAX
	$+14     >  EB 0B           JMP SHORT CryGame.0C8B659C
	$+16     >  8B8C24 F8000000 MOV ECX,DWORD PTR SS:[ESP+0xF8]
	$+1D     >  894C24 74       MOV DWORD PTR SS:[ESP+0x74],ECX
	$+21     >  8B5424 24       MOV EDX,DWORD PTR SS:[ESP+0x24]
	$+25     >  52              PUSH EDX
	$+26     >  E8 BA7BDEFF     CALL CryGame.0C69E160                    ; CNpcSaleInfo
	$+2B     >  8BF0            MOV ESI,EAX

	C1????????74??8B????????????8B??89??????EB??8B????????????89??????8B????????E8

	//zhp 2016/5/16 17:07
	$-158    0B2B3D75      68 E04C5B0C           PUSH CryGame.0C5B4CE0                    ; shopsaleitemdataarray
	$-153    0B2B3D7A      50                    PUSH EAX
	$-152    0B2B3D7B      FFD2                  CALL EDX
	$-150    0B2B3D7D      84C0                  TEST AL, AL
	$-14E    0B2B3D7F      8B8424 BC000000       MOV EAX, DWORD PTR SS:[ESP+BC]           ; mhoclien.005C0073
	$-147    0B2B3D86      75 68                 JNZ SHORT CryGame.0B2B3DF0
	$-145    0B2B3D88      C1E8 06               SHR EAX, 6
	$-142    0B2B3D8B      A8 01                 TEST AL, 1
	$-140    0B2B3D8D      74 25                 JE SHORT CryGame.0B2B3DB4
	$-13E    0B2B3D8F      8B8424 C0000000       MOV EAX, DWORD PTR SS:[ESP+C0]           ; mhoclien.00770067
	$-137    0B2B3D96      8B8C24 B8000000       MOV ECX, DWORD PTR SS:[ESP+B8]           ; mhoclien.0065006C
	$-130    0B2B3D9D      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-12E    0B2B3D9F      8B52 08               MOV EDX, DWORD PTR DS:[EDX+8]
	$-12B    0B2B3DA2      50                    PUSH EAX
	$-12A    0B2B3DA3      8D8424 BC000000       LEA EAX, DWORD PTR SS:[ESP+BC]
	$-123    0B2B3DAA      50                    PUSH EAX
	$-122    0B2B3DAB      FFD2                  CALL EDX
	$-120    0B2B3DAD      89B424 B8000000       MOV DWORD PTR SS:[ESP+B8], ESI
	$-119    0B2B3DB4      89B424 BC000000       MOV DWORD PTR SS:[ESP+BC], ESI
	$-112    0B2B3DBB      8B4424 54             MOV EAX, DWORD PTR SS:[ESP+54]           ; MHOCli_1.0FA6E466
	$-10E    0B2B3DBF      C1E8 06               SHR EAX, 6
	$-10B    0B2B3DC2      A8 01                 TEST AL, 1
	$-109    0B2B3DC4      74 15                 JE SHORT CryGame.0B2B3DDB
	$-107    0B2B3DC6      8B4424 58             MOV EAX, DWORD PTR SS:[ESP+58]
	$-103    0B2B3DCA      8B4C24 50             MOV ECX, DWORD PTR SS:[ESP+50]
	$-FF     0B2B3DCE      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-FD     0B2B3DD0      8B52 08               MOV EDX, DWORD PTR DS:[EDX+8]
	$-FA     0B2B3DD3      50                    PUSH EAX
	$-F9     0B2B3DD4      8D4424 54             LEA EAX, DWORD PTR SS:[ESP+54]
	$-F5     0B2B3DD8      50                    PUSH EAX
	$-F4     0B2B3DD9      FFD2                  CALL EDX
	$-F2     0B2B3DDB      5F                    POP EDI                                  ; ntdll.7793FD91
	$-F1     0B2B3DDC      5E                    POP ESI                                  ; ntdll.7793FD91
	$-F0     0B2B3DDD      5B                    POP EBX                                  ; ntdll.7793FD91
	$-EF     0B2B3DDE      8B8C24 48010000       MOV ECX, DWORD PTR SS:[ESP+148]
	$-E8     0B2B3DE5      33CC                  XOR ECX, ESP
	$-E6     0B2B3DE7      E8 296AF800           CALL CryGame.0C23A815
	$-E1     0B2B3DEC      8BE5                  MOV ESP, EBP
	$-DF     0B2B3DEE      5D                    POP EBP                                  ; ntdll.7793FD91
	$-DE     0B2B3DEF      C3                    RETN
	$-DD     0B2B3DF0      25 8F000000           AND EAX, 8F
	$-D8     0B2B3DF5      3C 09                 CMP AL, 9
	$-D6     0B2B3DF7      0F85 290F0000         JNZ CryGame.0B2B4D26
	$-D0     0B2B3DFD      8B8C24 B8000000       MOV ECX, DWORD PTR SS:[ESP+B8]           ; mhoclien.0065006C
	$-C9     0B2B3E04      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-C7     0B2B3E06      8B8424 C0000000       MOV EAX, DWORD PTR SS:[ESP+C0]           ; mhoclien.00770067
	$-C0     0B2B3E0D      8B52 44               MOV EDX, DWORD PTR DS:[EDX+44]
	$-BD     0B2B3E10      6A FF                 PUSH -1
	$-BB     0B2B3E12      56                    PUSH ESI
	$-BA     0B2B3E13      50                    PUSH EAX
	$-B9     0B2B3E14      FFD2                  CALL EDX
	$-B7     0B2B3E16      8B4424 54             MOV EAX, DWORD PTR SS:[ESP+54]           ; MHOCli_1.0FA6E466
	$-B3     0B2B3E1A      25 8F000000           AND EAX, 8F
	$-AE     0B2B3E1F      3C 0A                 CMP AL, 0A
	$-AC     0B2B3E21      0F94C1                SETE CL
	$-A9     0B2B3E24      884C24 14             MOV BYTE PTR SS:[ESP+14], CL
	$-A5     0B2B3E28      8B4424 14             MOV EAX, DWORD PTR SS:[ESP+14]
	$-A1     0B2B3E2C      8B4C24 50             MOV ECX, DWORD PTR SS:[ESP+50]
	$-9D     0B2B3E30      50                    PUSH EAX
	$-9C     0B2B3E31      8D8424 04010000       LEA EAX, DWORD PTR SS:[ESP+104]
	$-95     0B2B3E38      50                    PUSH EAX
	$-94     0B2B3E39      8B4424 60             MOV EAX, DWORD PTR SS:[ESP+60]
	$-90     0B2B3E3D      89B424 08010000       MOV DWORD PTR SS:[ESP+108], ESI
	$-89     0B2B3E44      89B424 0C010000       MOV DWORD PTR SS:[ESP+10C], ESI
	$-82     0B2B3E4B      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-80     0B2B3E4D      8B52 10               MOV EDX, DWORD PTR DS:[EDX+10]
	$-7D     0B2B3E50      68 F84C5B0C           PUSH CryGame.0C5B4CF8                    ; sale_item_data
	$-78     0B2B3E55      50                    PUSH EAX
	$-77     0B2B3E56      FFD2                  CALL EDX
	$-75     0B2B3E58      84C0                  TEST AL, AL
	$-73     0B2B3E5A      0F84 BA0E0000         JE CryGame.0B2B4D1A
	$-6D     0B2B3E60      8B8424 04010000       MOV EAX, DWORD PTR SS:[ESP+104]          ; mhoclien.006F0068
	$-66     0B2B3E67      8BC8                  MOV ECX, EAX
	$-64     0B2B3E69      81E1 8F000000         AND ECX, 8F
	$-5E     0B2B3E6F      80F9 06               CMP CL, 6
	$-5B     0B2B3E72      0F85 A20E0000         JNZ CryGame.0B2B4D1A
	$-55     0B2B3E78      C1E8 06               SHR EAX, 6
	$-52     0B2B3E7B      A8 01                 TEST AL, 1
	$-50     0B2B3E7D      74 12                 JE SHORT CryGame.0B2B3E91
	$-4E     0B2B3E7F      8B9424 08010000       MOV EDX, DWORD PTR SS:[ESP+108]          ; mhoclien.006C0063
	$-47     0B2B3E86      8B02                  MOV EAX, DWORD PTR DS:[EDX]
	$-45     0B2B3E88      898424 E4000000       MOV DWORD PTR SS:[ESP+E4], EAX
	$-3E     0B2B3E8F      EB 0E                 JMP SHORT CryGame.0B2B3E9F
	$-3C     0B2B3E91      8B8C24 08010000       MOV ECX, DWORD PTR SS:[ESP+108]          ; mhoclien.006C0063
	$-35     0B2B3E98      898C24 E4000000       MOV DWORD PTR SS:[ESP+E4], ECX           ; ntdll.779301E4
	$-2E     0B2B3E9F      8B5424 54             MOV EDX, DWORD PTR SS:[ESP+54]           ; MHOCli_1.0FA6E466
	$-2A     0B2B3EA3      8B4C24 50             MOV ECX, DWORD PTR SS:[ESP+50]
	$-26     0B2B3EA7      81E2 8F000000         AND EDX, 8F
	$-20     0B2B3EAD      80FA 0A               CMP DL, 0A
	$-1D     0B2B3EB0      8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-1B     0B2B3EB2      8B52 10               MOV EDX, DWORD PTR DS:[EDX+10]
	$-18     0B2B3EB5      0F94C0                SETE AL
	$-15     0B2B3EB8      884424 14             MOV BYTE PTR SS:[ESP+14], AL
	$-11     0B2B3EBC      8B4424 14             MOV EAX, DWORD PTR SS:[ESP+14]
	$-D      0B2B3EC0      50                    PUSH EAX
	$-C      0B2B3EC1      8D8424 04010000       LEA EAX, DWORD PTR SS:[ESP+104]
	$-5      0B2B3EC8      50                    PUSH EAX
	$-4      0B2B3EC9      8B4424 60             MOV EAX, DWORD PTR SS:[ESP+60]
	$ ==>    0B2B3ECD      68 084D5B0C           PUSH CryGame.0C5B4D08                    ; exchange_data_class
	$+5      0B2B3ED2      50                    PUSH EAX
	$+6      0B2B3ED3      FFD2                  CALL EDX
	$+8      0B2B3ED5      84C0                  TEST AL, AL
	$+A      0B2B3ED7      0F84 3D0E0000         JE CryGame.0B2B4D1A
	$+10     0B2B3EDD      8B8424 04010000       MOV EAX, DWORD PTR SS:[ESP+104]          ; mhoclien.006F0068
	$+17     0B2B3EE4      8BC8                  MOV ECX, EAX
	$+19     0B2B3EE6      81E1 8F000000         AND ECX, 8F
	$+1F     0B2B3EEC      80F9 06               CMP CL, 6
	$+22     0B2B3EEF      0F85 250E0000         JNZ CryGame.0B2B4D1A
	$+28     0B2B3EF5      C1E8 06               SHR EAX, 6
	$+2B     0B2B3EF8      A8 01                 TEST AL, 1
	$+2D     0B2B3EFA      74 12                 JE SHORT CryGame.0B2B3F0E
	$+2F     0B2B3EFC      8B9424 08010000       MOV EDX, DWORD PTR SS:[ESP+108]          ; mhoclien.006C0063
	$+36     0B2B3F03      8B02                  MOV EAX, DWORD PTR DS:[EDX]
	$+38     0B2B3F05      898424 A4000000       MOV DWORD PTR SS:[ESP+A4], EAX
	$+3F     0B2B3F0C      EB 0E                 JMP SHORT CryGame.0B2B3F1C
	$+41     0B2B3F0E      8B8C24 08010000       MOV ECX, DWORD PTR SS:[ESP+108]          ; mhoclien.006C0063
	$+48     0B2B3F15      898C24 A4000000       MOV DWORD PTR SS:[ESP+A4], ECX           ; ntdll.779301E4
	$+4F     0B2B3F1C      8B5424 28             MOV EDX, DWORD PTR SS:[ESP+28]
	$+53     0B2B3F20      52                    PUSH EDX
	$+54     0B2B3F21      E8 CA8ADAFF           CALL CryGame.0B05C9F0
	$+59     0B2B3F26      83C4 04               ADD ESP, 4
	$+5C     0B2B3F29      894424 20             MOV DWORD PTR SS:[ESP+20], EAX
	$+60     0B2B3F2D      3BC6                  CMP EAX, ESI
	$+62     0B2B3F2F      0F84 6E0D0000         JE CryGame.0B2B4CA3
	$+68     0B2B3F35      33FF                  XOR EDI, EDI
	$+6A     0B2B3F37      89BC24 D4000000       MOV DWORD PTR SS:[ESP+D4], EDI
	$+71     0B2B3F3E      89BC24 D8000000       MOV DWORD PTR SS:[ESP+D8], EDI
	$+78     0B2B3F45      89BC24 DC000000       MOV DWORD PTR SS:[ESP+DC], EDI
	$+7F     0B2B3F4C      E8 6FBBFBFF           CALL CryGame.0B26FAC0
	$+84     0B2B3F51      894424 4C             MOV DWORD PTR SS:[ESP+4C], EAX
	$+88     0B2B3F55      897C24 2C             MOV DWORD PTR SS:[ESP+2C], EDI
	$+8C     0B2B3F59      3BC7                  CMP EAX, EDI
	$+8E     0B2B3F5B      74 0A                 JE SHORT CryGame.0B2B3F67
	$+90     0B2B3F5D      8BB8 A0220000         MOV EDI, DWORD PTR DS:[EAX+22A0]
	$+96     0B2B3F63      897C24 2C             MOV DWORD PTR SS:[ESP+2C], EDI
	$+9A     0B2B3F67      8B4424 20             MOV EAX, DWORD PTR SS:[ESP+20]
	$+9E     0B2B3F6B      8B70 1C               MOV ESI, DWORD PTR DS:[EAX+1C]
	$+A1     0B2B3F6E      3B70 20               CMP ESI, DWORD PTR DS:[EAX+20]
	$+A4     0B2B3F71      74 4B                 JE SHORT CryGame.0B2B3FBE
	$+A6     0B2B3F73      8B06                  MOV EAX, DWORD PTR DS:[ESI]
	$+A8     0B2B3F75      8078 5C 00            CMP BYTE PTR DS:[EAX+5C], 0
	$+AC     0B2B3F79      894424 14             MOV DWORD PTR SS:[ESP+14], EAX
	$+B0     0B2B3F7D      74 33                 JE SHORT CryGame.0B2B3FB2
	$+B2     0B2B3F7F      85FF                  TEST EDI, EDI
	$+B4     0B2B3F81      74 1F                 JE SHORT CryGame.0B2B3FA2
	$+B6     0B2B3F83      8B48 2C               MOV ECX, DWORD PTR DS:[EAX+2C]
	$+B9     0B2B3F86      8B50 28               MOV EDX, DWORD PTR DS:[EAX+28]
	$+BC     0B2B3F89      51                    PUSH ECX                                 ; ntdll.779301E4
	$+BD     0B2B3F8A      8B48 08               MOV ECX, DWORD PTR DS:[EAX+8]
	$+C0     0B2B3F8D      52                    PUSH EDX
	$+C1     0B2B3F8E      8B50 0C               MOV EDX, DWORD PTR DS:[EAX+C]
	$+C4     0B2B3F91      8B40 50               MOV EAX, DWORD PTR DS:[EAX+50]
	$+C7     0B2B3F94      51                    PUSH ECX                                 ; ntdll.779301E4
	$+C8     0B2B3F95      52                    PUSH EDX
	$+C9     0B2B3F96      50                    PUSH EAX
	$+CA     0B2B3F97      8BCF                  MOV ECX, EDI
	$+CC     0B2B3F99      E8 129DE300           CALL CryGame.0C0EDCB0
	$+D1     0B2B3F9E      84C0                  TEST AL, AL
	$+D3     0B2B3FA0      74 10                 JE SHORT CryGame.0B2B3FB2
	$+D5     0B2B3FA2      8D4C24 14             LEA ECX, DWORD PTR SS:[ESP+14]
	$+D9     0B2B3FA6      8D8424 D4000000       LEA EAX, DWORD PTR SS:[ESP+D4]
	$+E0     0B2B3FAD      E8 0E340000           CALL CryGame.0B2B73C0
	$+E5     0B2B3FB2      8B4C24 20             MOV ECX, DWORD PTR SS:[ESP+20]
	$+E9     0B2B3FB6      83C6 04               ADD ESI, 4
	$+EC     0B2B3FB9      3B71 20               CMP ESI, DWORD PTR DS:[ECX+20]
	$+EF     0B2B3FBC    ^ 75 B5                 JNZ SHORT CryGame.0B2B3F73
	$+F1     0B2B3FBE      33FF                  XOR EDI, EDI
	$+F3     0B2B3FC0      89BC24 B0000000       MOV DWORD PTR SS:[ESP+B0], EDI
	$+FA     0B2B3FC7      89BC24 B4000000       MOV DWORD PTR SS:[ESP+B4], EDI
	$+101    0B2B3FCE      E8 FDBF8500           CALL CryGame.0BB0FFD0
	$+106    0B2B3FD3      8B4C24 2C             MOV ECX, DWORD PTR SS:[ESP+2C]
	$+10A    0B2B3FD7      899424 9C000000       MOV DWORD PTR SS:[ESP+9C], EDX
	$+111    0B2B3FDE      8B5424 28             MOV EDX, DWORD PTR SS:[ESP+28]
	$+115    0B2B3FE2      52                    PUSH EDX
	$+116    0B2B3FE3      898424 9C000000       MOV DWORD PTR SS:[ESP+9C], EAX
	$+11D    0B2B3FEA      897C24 24             MOV DWORD PTR SS:[ESP+24], EDI
	$+121    0B2B3FEE      897C24 10             MOV DWORD PTR SS:[ESP+10], EDI
	$+125    0B2B3FF2      E8 8993E300           CALL CryGame.0C0ED380
	$+12A    0B2B3FF7      8BF0                  MOV ESI, EAX
	$+12C    0B2B3FF9      85F6                  TEST ESI, ESI
	$+12E    0B2B3FFB      74 69                 JE SHORT CryGame.0B2B4066
	$+130    0B2B3FFD      8B4E 20               MOV ECX, DWORD PTR DS:[ESI+20]
	$+133    0B2B4000      894C24 14             MOV DWORD PTR SS:[ESP+14], ECX           ; ntdll.779301E4
	$+137    0B2B4004      E8 C7589C00           CALL CryGame.0BC798D0
	$+13C    0B2B4009      8B4C24 2C             MOV ECX, DWORD PTR SS:[ESP+2C]
	$+140    0B2B400D      8BF8                  MOV EDI, EAX
	$+142    0B2B400F      8B4424 28             MOV EAX, DWORD PTR SS:[ESP+28]
	$+146    0B2B4013      50                    PUSH EAX
	$+147    0B2B4014      E8 37437E00           CALL CryGame.0BA98350
	$+14C    0B2B4019      8D8C24 A8000000       LEA ECX, DWORD PTR SS:[ESP+A8]
	$+153    0B2B4020      898424 B0000000       MOV DWORD PTR SS:[ESP+B0], EAX
	$+15A    0B2B4027      33C0                  XOR EAX, EAX
	$+15C    0B2B4029      51                    PUSH ECX                                 ; ntdll.779301E4
	$+15D    0B2B402A      8BCE                  MOV ECX, ESI
	$+15F    0B2B402C      899424 B8000000       MOV DWORD PTR SS:[ESP+B8], EDX
	$+166    0B2B4033      898424 AC000000       MOV DWORD PTR SS:[ESP+AC], EAX
	$+16D    0B2B403A      898424 B0000000       MOV DWORD PTR SS:[ESP+B0], EAX
	$+174    0B2B4041      E8 6A69E300           CALL CryGame.0C0EA9B0
	$+179    0B2B4046      8B4C24 18             MOV ECX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+17D    0B2B404A      50                    PUSH EAX
	$+17E    0B2B404B      E8 60699C00           CALL CryGame.0BC7A9B0
	$+183    0B2B4050      8B9424 AC000000       MOV EDX, DWORD PTR SS:[ESP+AC]           ; mhoclien.00610072
	$+18A    0B2B4057      8B8424 A8000000       MOV EAX, DWORD PTR SS:[ESP+A8]           ; mhoclien.0067006F
	$+191    0B2B405E      895424 20             MOV DWORD PTR SS:[ESP+20], EDX
	$+195    0B2B4062      894424 0C             MOV DWORD PTR SS:[ESP+C], EAX
	$+199    0B2B4066      57                    PUSH EDI
	$+19A    0B2B4067      8D4C24 54             LEA ECX, DWORD PTR SS:[ESP+54]
	$+19E    0B2B406B      51                    PUSH ECX                                 ; ntdll.779301E4
	$+19F    0B2B406C      53                    PUSH EBX
	$+1A0    0B2B406D      BA 1C4D5B0C           MOV EDX, CryGame.0C5B4D1C                ; shopLibType
	$+1A5    0B2B4072      E8 19D0D6FF           CALL CryGame.0B021090
	$+1AA    0B2B4077      8B9424 BC000000       MOV EDX, DWORD PTR SS:[ESP+BC]           ; mhoclien.005C0073
	$+1B1    0B2B407E      83C4 0C               ADD ESP, 0C
	$+1B4    0B2B4081      52                    PUSH EDX
	$+1B5    0B2B4082      8D4424 54             LEA EAX, DWORD PTR SS:[ESP+54]
	$+1B9    0B2B4086      50                    PUSH EAX
	$+1BA    0B2B4087      53                    PUSH EBX
	$+1BB    0B2B4088      BA 284D5B0C           MOV EDX, CryGame.0C5B4D28                ; nextResetTime
	$+1C0    0B2B408D      E8 FECFD6FF           CALL CryGame.0B021090
	$+1C5    0B2B4092      8B8C24 A4000000       MOV ECX, DWORD PTR SS:[ESP+A4]           ; mhoclien.00720070
	$+1CC    0B2B4099      83C4 0C               ADD ESP, 0C
	$+1CF    0B2B409C      51                    PUSH ECX                                 ; ntdll.779301E4
	$+1D0    0B2B409D      8D5424 54             LEA EDX, DWORD PTR SS:[ESP+54]
	$+1D4    0B2B40A1      52                    PUSH EDX
	$+1D5    0B2B40A2      53                    PUSH EBX
	$+1D6    0B2B40A3      BA 384D5B0C           MOV EDX, CryGame.0C5B4D38                ; currentTime
	$+1DB    0B2B40A8      E8 E3CFD6FF           CALL CryGame.0B021090
	$+1E0    0B2B40AD      8B4424 2C             MOV EAX, DWORD PTR SS:[ESP+2C]
	$+1E4    0B2B40B1      83C4 0C               ADD ESP, 0C
	$+1E7    0B2B40B4      50                    PUSH EAX
	$+1E8    0B2B40B5      8D4C24 54             LEA ECX, DWORD PTR SS:[ESP+54]
	$+1EC    0B2B40B9      51                    PUSH ECX                                 ; ntdll.779301E4
	$+1ED    0B2B40BA      53                    PUSH EBX
	$+1EE    0B2B40BB      BA 444D5B0C           MOV EDX, CryGame.0C5B4D44                ; resetBindCredit
	$+1F3    0B2B40C0      E8 CBCFD6FF           CALL CryGame.0B021090
	$+1F8    0B2B40C5      8B5424 18             MOV EDX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+1FC    0B2B40C9      83C4 0C               ADD ESP, 0C
	$+1FF    0B2B40CC      52                    PUSH EDX
	$+200    0B2B40CD      8D4424 54             LEA EAX, DWORD PTR SS:[ESP+54]
	$+204    0B2B40D1      50                    PUSH EAX
	$+205    0B2B40D2      53                    PUSH EBX
	$+206    0B2B40D3      BA 544D5B0C           MOV EDX, CryGame.0C5B4D54                ; ASCII "resetCredit"
	$+20B    0B2B40D8      E8 B3CFD6FF           CALL CryGame.0B021090
	$+210    0B2B40DD      8B4424 7C             MOV EAX, DWORD PTR SS:[ESP+7C]           ; mhoclien.0040E245
	$+214    0B2B40E1      33FF                  XOR EDI, EDI
	$+216    0B2B40E3      83C4 0C               ADD ESP, 0C
	$+219    0B2B40E6      3BC7                  CMP EAX, EDI
	$+21B    0B2B40E8      897C24 20             MOV DWORD PTR SS:[ESP+20], EDI
	$+21F    0B2B40EC      897C24 78             MOV DWORD PTR SS:[ESP+78], EDI
	$+223    0B2B40F0      89BC24 E0000000       MOV DWORD PTR SS:[ESP+E0], EDI
	$+22A    0B2B40F7      7C 15                 JL SHORT CryGame.0B2B410E
	$+22C    0B2B40F9      8B8C24 18010000       MOV ECX, DWORD PTR SS:[ESP+118]          ; mhoclien.00650078
	$+233    0B2B4100      3BCF                  CMP ECX, EDI
	$+235    0B2B4102      7E 0A                 JLE SHORT CryGame.0B2B410E
	$+237    0B2B4104      0FAFC1                IMUL EAX, ECX                            ; ntdll.779301E4
	$+23A    0B2B4107      898424 E0000000       MOV DWORD PTR SS:[ESP+E0], EAX
	$+241    0B2B410E      8B8424 D8000000       MOV EAX, DWORD PTR SS:[ESP+D8]           ; mhoclien.006E0069
	$+248    0B2B4115      8BB424 D4000000       MOV ESI, DWORD PTR SS:[ESP+D4]           ; mhoclien.006C006E
	$+24F    0B2B411C      8B4C24 4C             MOV ECX, DWORD PTR SS:[ESP+4C]
	$+253    0B2B4120      8B89 284E0000         MOV ECX, DWORD PTR DS:[ECX+4E28]
	$+259    0B2B4126      2BC6                  SUB EAX, ESI
	$+25B    0B2B4128      C1F8 02               SAR EAX, 2
	$+25E    0B2B412B      898424 B0000000       MOV DWORD PTR SS:[ESP+B0], EAX
	$+265    0B2B4132      3BCF                  CMP ECX, EDI
	$+267    0B2B4134      74 1C                 JE SHORT CryGame.0B2B4152
	$+269    0B2B4136      57                    PUSH EDI
	$+26A    0B2B4137      6A 03                 PUSH 3
	$+26C    0B2B4139      E8 B2BDDA00           CALL CryGame.0C05FEF0
	$+271    0B2B413E      3BC7                  CMP EAX, EDI
	$+273    0B2B4140      74 10                 JE SHORT CryGame.0B2B4152
	$+275    0B2B4142      8BC8                  MOV ECX, EAX
	$+277    0B2B4144      E8 D7B4CFFF           CALL CryGame.0AFAF620
	$+27C    0B2B4149      50                    PUSH EAX
	$+27D    0B2B414A      E8 E186DAFF           CALL CryGame.0B05C830
	$+282    0B2B414F      83C4 04               ADD ESP, 4
	$+285    0B2B4152      897424 7C             MOV DWORD PTR SS:[ESP+7C], ESI
	$+289    0B2B4156      3BB424 D8000000       CMP ESI, DWORD PTR SS:[ESP+D8]           ; mhoclien.006E0069
	$+290    0B2B415D      0F84 2C0B0000         JE CryGame.0B2B4C8F
	$+296    0B2B4163      8B5424 7C             MOV EDX, DWORD PTR SS:[ESP+7C]           ; mhoclien.0040E245
	$+29A    0B2B4167      8B8C24 E0000000       MOV ECX, DWORD PTR SS:[ESP+E0]           ; mhoclien.00690062
	$+2A1    0B2B416E      8B02                  MOV EAX, DWORD PTR DS:[EDX]
	$+2A3    0B2B4170      894424 24             MOV DWORD PTR SS:[ESP+24], EAX
	$+2A7    0B2B4174      394C24 78             CMP DWORD PTR SS:[ESP+78], ECX           ; ntdll.779301E4
	$+2AB    0B2B4178      0F82 990A0000         JB CryGame.0B2B4C17
	$+2B1    0B2B417E      8B4424 20             MOV EAX, DWORD PTR SS:[ESP+20]
	$+2B5    0B2B4182      C74424 6C 01000000    MOV DWORD PTR SS:[ESP+6C], 1
	$+2BD    0B2B418A      85C0                  TEST EAX, EAX
	$+2BF    0B2B418C      75 06                 JNZ SHORT CryGame.0B2B4194
	$+2C1    0B2B418E      894424 6C             MOV DWORD PTR SS:[ESP+6C], EAX
	$+2C5    0B2B4192      EB 14                 JMP SHORT CryGame.0B2B41A8
	$+2C7    0B2B4194      8B9424 18010000       MOV EDX, DWORD PTR SS:[ESP+118]          ; mhoclien.00650078
	$+2CE    0B2B419B      4A                    DEC EDX
	$+2CF    0B2B419C      3BC2                  CMP EAX, EDX
	$+2D1    0B2B419E      75 08                 JNZ SHORT CryGame.0B2B41A8
	$+2D3    0B2B41A0      C74424 6C 02000000    MOV DWORD PTR SS:[ESP+6C], 2
	$+2DB    0B2B41A8      8B4424 7C             MOV EAX, DWORD PTR SS:[ESP+7C]           ; mhoclien.0040E245
	$+2DF    0B2B41AC      83C0 04               ADD EAX, 4
	$+2E2    0B2B41AF      3B8424 D8000000       CMP EAX, DWORD PTR SS:[ESP+D8]           ; mhoclien.006E0069
	$+2E9    0B2B41B6      75 08                 JNZ SHORT CryGame.0B2B41C0
	$+2EB    0B2B41B8      C74424 6C 02000000    MOV DWORD PTR SS:[ESP+6C], 2
	$+2F3    0B2B41C0      8B8C24 E4000000       MOV ECX, DWORD PTR SS:[ESP+E4]           ; mhoclien.005C006E
	$+2FA    0B2B41C7      33F6                  XOR ESI, ESI
	$+2FC    0B2B41C9      56                    PUSH ESI
	$+2FD    0B2B41CA      56                    PUSH ESI
	$+2FE    0B2B41CB      51                    PUSH ECX                                 ; ntdll.779301E4
	$+2FF    0B2B41CC      8D5424 3C             LEA EDX, DWORD PTR SS:[ESP+3C]
	$+303    0B2B41D0      52                    PUSH EDX
	$+304    0B2B41D1      8BCB                  MOV ECX, EBX
	$+306    0B2B41D3      897424 40             MOV DWORD PTR SS:[ESP+40], ESI
	$+30A    0B2B41D7      897424 44             MOV DWORD PTR SS:[ESP+44], ESI
	$+30E    0B2B41DB      E8 10DCF800           CALL CryGame.0C241DF0
	$+313    0B2B41E0      8B7C24 2C             MOV EDI, DWORD PTR SS:[ESP+2C]
	$+317    0B2B41E4      897424 18             MOV DWORD PTR SS:[ESP+18], ESI
	$+31B    0B2B41E8      897424 10             MOV DWORD PTR SS:[ESP+10], ESI
	$+31F    0B2B41EC      8B7424 24             MOV ESI, DWORD PTR SS:[ESP+24]
	$+323    0B2B41F0      8B46 28               MOV EAX, DWORD PTR DS:[ESI+28]           ; KERNELBA.771C4498
	$+326    0B2B41F3      C64424 70 00          MOV BYTE PTR SS:[ESP+70], 0
	$+32B    0B2B41F8      C64424 74 00          MOV BYTE PTR SS:[ESP+74], 0
	$+330    0B2B41FD      C68424 98000000 00    MOV BYTE PTR SS:[ESP+98], 0
	$+338    0B2B4205      C64424 4C 00          MOV BYTE PTR SS:[ESP+4C], 0
	$+33D    0B2B420A      894424 0C             MOV DWORD PTR SS:[ESP+C], EAX
	$+341    0B2B420E      85FF                  TEST EDI, EDI
	$+343    0B2B4210      0F84 77000000         JE CryGame.0B2B428D
	$+349    0B2B4216      8B4E 08               MOV ECX, DWORD PTR DS:[ESI+8]
	$+34C    0B2B4219      8B5424 28             MOV EDX, DWORD PTR SS:[ESP+28]
	$+350    0B2B421D      51                    PUSH ECX                                 ; ntdll.779301E4
	$+351    0B2B421E      52                    PUSH EDX
	$+352    0B2B421F      6A 01                 PUSH 1
	$+354    0B2B4221      8BCF                  MOV ECX, EDI
	$+356    0B2B4223      E8 58BAE300           CALL CryGame.0C0EFC80
	$+35B    0B2B4228      8B4C24 28             MOV ECX, DWORD PTR SS:[ESP+28]
	$+35F    0B2B422C      894424 18             MOV DWORD PTR SS:[ESP+18], EAX
	$+363    0B2B4230      8B46 08               MOV EAX, DWORD PTR DS:[ESI+8]
	$+366    0B2B4233      50                    PUSH EAX
	$+367    0B2B4234      51                    PUSH ECX                                 ; ntdll.779301E4
	$+368    0B2B4235      6A 01                 PUSH 1
	$+36A    0B2B4237      8BCF                  MOV ECX, EDI
	$+36C    0B2B4239      E8 52C6E300           CALL CryGame.0C0F0890
	$+371    0B2B423E      8B5424 0C             MOV EDX, DWORD PTR SS:[ESP+C]
	$+375    0B2B4242      52                    PUSH EDX
	$+376    0B2B4243      8BCF                  MOV ECX, EDI
	$+378    0B2B4245      894424 14             MOV DWORD PTR SS:[ESP+14], EAX
	$+37C    0B2B4249      E8 62B8E300           CALL CryGame.0C0EFAB0
	$+381    0B2B424E      884424 70             MOV BYTE PTR SS:[ESP+70], AL
	$+385    0B2B4252      8B4424 0C             MOV EAX, DWORD PTR SS:[ESP+C]
	$+389    0B2B4256      50                    PUSH EAX
	$+38A    0B2B4257      8BCF                  MOV ECX, EDI
	$+38C    0B2B4259      E8 12B8E300           CALL CryGame.0C0EFA70
	$+391    0B2B425E      8B4E 08               MOV ECX, DWORD PTR DS:[ESI+8]
	$+394    0B2B4261      8B5424 28             MOV EDX, DWORD PTR SS:[ESP+28]
	$+398    0B2B4265      51                    PUSH ECX                                 ; ntdll.779301E4
	$+399    0B2B4266      52                    PUSH EDX
	$+39A    0B2B4267      8BCF                  MOV ECX, EDI
	$+39C    0B2B4269      884424 7C             MOV BYTE PTR SS:[ESP+7C], AL
	$+3A0    0B2B426D      E8 4EC2E300           CALL CryGame.0C0F04C0
	$+3A5    0B2B4272      8B4C24 28             MOV ECX, DWORD PTR SS:[ESP+28]
	$+3A9    0B2B4276      888424 98000000       MOV BYTE PTR SS:[ESP+98], AL
	$+3B0    0B2B427D      8B46 08               MOV EAX, DWORD PTR DS:[ESI+8]
	$+3B3    0B2B4280      50                    PUSH EAX
	$+3B4    0B2B4281      51                    PUSH ECX                                 ; ntdll.779301E4
	$+3B5    0B2B4282      8BCF                  MOV ECX, EDI
	$+3B7    0B2B4284      E8 C7C1E300           CALL CryGame.0C0F0450
	$+3BC    0B2B4289      884424 4C             MOV BYTE PTR SS:[ESP+4C], AL
	$+3C0    0B2B428D      8B5424 18             MOV EDX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+3C4    0B2B4291      52                    PUSH EDX
	$+3C5    0B2B4292      8D4424 34             LEA EAX, DWORD PTR SS:[ESP+34]
	$+3C9    0B2B4296      50                    PUSH EAX
	$+3CA    0B2B4297      53                    PUSH EBX
	$+3CB    0B2B4298      BA 604D5B0C           MOV EDX, CryGame.0C5B4D60                ; ASCII "itemLimitType"
	$+3D0    0B2B429D      E8 EECDD6FF           CALL CryGame.0B021090
	$+3D5    0B2B42A2      8B4C24 7C             MOV ECX, DWORD PTR SS:[ESP+7C]           ; mhoclien.0040E245
	$+3D9    0B2B42A6      83C4 0C               ADD ESP, 0C
	$+3DC    0B2B42A9      51                    PUSH ECX                                 ; ntdll.779301E4
	$+3DD    0B2B42AA      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+3E1    0B2B42AE      52                    PUSH EDX
	$+3E2    0B2B42AF      53                    PUSH EBX
	$+3E3    0B2B42B0      BA 704D5B0C           MOV EDX, CryGame.0C5B4D70                ; ASCII "isSkillFormula"
	$+3E8    0B2B42B5      E8 46CED6FF           CALL CryGame.0B021100
	$+3ED    0B2B42BA      8B8424 80000000       MOV EAX, DWORD PTR SS:[ESP+80]
	$+3F4    0B2B42C1      83C4 0C               ADD ESP, 0C
	$+3F7    0B2B42C4      50                    PUSH EAX
	$+3F8    0B2B42C5      8D4C24 34             LEA ECX, DWORD PTR SS:[ESP+34]
	$+3FC    0B2B42C9      51                    PUSH ECX                                 ; ntdll.779301E4
	$+3FD    0B2B42CA      53                    PUSH EBX
	$+3FE    0B2B42CB      BA 804D5B0C           MOV EDX, CryGame.0C5B4D80                ; ASCII "isManuFormula"
	$+403    0B2B42D0      E8 2BCED6FF           CALL CryGame.0B021100
	$+408    0B2B42D5      8B5424 1C             MOV EDX, DWORD PTR SS:[ESP+1C]
	$+40C    0B2B42D9      83C4 0C               ADD ESP, 0C
	$+40F    0B2B42DC      52                    PUSH EDX
	$+410    0B2B42DD      8D4424 34             LEA EAX, DWORD PTR SS:[ESP+34]
	$+414    0B2B42E1      50                    PUSH EAX
	$+415    0B2B42E2      53                    PUSH EBX
	$+416    0B2B42E3      BA 904D5B0C           MOV EDX, CryGame.0C5B4D90                ; ASCII "currentLimitBuyCounts"
	$+41B    0B2B42E8      E8 A3CDD6FF           CALL CryGame.0B021090
	$+420    0B2B42ED      8B8C24 A4000000       MOV ECX, DWORD PTR SS:[ESP+A4]           ; mhoclien.00720070
	$+427    0B2B42F4      83C4 0C               ADD ESP, 0C
	$+42A    0B2B42F7      51                    PUSH ECX                                 ; ntdll.779301E4
	$+42B    0B2B42F8      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+42F    0B2B42FC      52                    PUSH EDX
	$+430    0B2B42FD      53                    PUSH EBX
	$+431    0B2B42FE      BA A84D5B0C           MOV EDX, CryGame.0C5B4DA8                ; ASCII "hasBeenLearnedSkill"
	$+436    0B2B4303      E8 F8CDD6FF           CALL CryGame.0B021100
	$+43B    0B2B4308      8B4424 58             MOV EAX, DWORD PTR SS:[ESP+58]
	$+43F    0B2B430C      83C4 0C               ADD ESP, 0C
	$+442    0B2B430F      50                    PUSH EAX
	$+443    0B2B4310      8D4C24 34             LEA ECX, DWORD PTR SS:[ESP+34]
	$+447    0B2B4314      51                    PUSH ECX                                 ; ntdll.779301E4
	$+448    0B2B4315      53                    PUSH EBX
	$+449    0B2B4316      BA BC4D5B0C           MOV EDX, CryGame.0C5B4DBC                ; ASCII "hasBeenLearnedFormula"
	$+44E    0B2B431B      E8 E0CDD6FF           CALL CryGame.0B021100
	$+453    0B2B4320      83C4 0C               ADD ESP, 0C
	$+456    0B2B4323      33FF                  XOR EDI, EDI
	$+458    0B2B4325      57                    PUSH EDI
	$+459    0B2B4326      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+45D    0B2B432A      52                    PUSH EDX
	$+45E    0B2B432B      53                    PUSH EBX
	$+45F    0B2B432C      BA D44D5B0C           MOV EDX, CryGame.0C5B4DD4                ; ASCII "backItem"
	$+464    0B2B4331      E8 CACDD6FF           CALL CryGame.0B021100
	$+469    0B2B4336      8B46 08               MOV EAX, DWORD PTR DS:[ESI+8]
	$+46C    0B2B4339      83C4 0C               ADD ESP, 0C
	$+46F    0B2B433C      50                    PUSH EAX
	$+470    0B2B433D      8D4C24 34             LEA ECX, DWORD PTR SS:[ESP+34]
	$+474    0B2B4341      51                    PUSH ECX                                 ; ntdll.779301E4
	$+475    0B2B4342      53                    PUSH EBX
	$+476    0B2B4343      BA E04D5B0C           MOV EDX, CryGame.0C5B4DE0                ; ASCII "SaleItemId"
	$+47B    0B2B4348      E8 43CDD6FF           CALL CryGame.0B021090
	$+480    0B2B434D      8B56 0C               MOV EDX, DWORD PTR DS:[ESI+C]
	$+483    0B2B4350      83C4 0C               ADD ESP, 0C
	$+486    0B2B4353      52                    PUSH EDX
	$+487    0B2B4354      8D4424 34             LEA EAX, DWORD PTR SS:[ESP+34]
	$+48B    0B2B4358      50                    PUSH EAX
	$+48C    0B2B4359      53                    PUSH EBX
	$+48D    0B2B435A      BA EC4D5B0C           MOV EDX, CryGame.0C5B4DEC                ; ASCII "shopID"
	$+492    0B2B435F      E8 2CCDD6FF           CALL CryGame.0B021090
	$+497    0B2B4364      8B4E 28               MOV ECX, DWORD PTR DS:[ESI+28]           ; KERNELBA.771C4498
	$+49A    0B2B4367      83C4 0C               ADD ESP, 0C
	$+49D    0B2B436A      51                    PUSH ECX                                 ; ntdll.779301E4
	$+49E    0B2B436B      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+4A2    0B2B436F      52                    PUSH EDX
	$+4A3    0B2B4370      53                    PUSH EBX
	$+4A4    0B2B4371      BA 647D5A0C           MOV EDX, CryGame.0C5A7D64
	$+4A9    0B2B4376      E8 15CDD6FF           CALL CryGame.0B021090
	$+4AE    0B2B437B      8B46 2C               MOV EAX, DWORD PTR DS:[ESI+2C]
	$+4B1    0B2B437E      83C4 0C               ADD ESP, 0C
	$+4B4    0B2B4381      50                    PUSH EAX
	$+4B5    0B2B4382      8D4C24 34             LEA ECX, DWORD PTR SS:[ESP+34]
	$+4B9    0B2B4386      51                    PUSH ECX                                 ; ntdll.779301E4
	$+4BA    0B2B4387      53                    PUSH EBX
	$+4BB    0B2B4388      BA F44D5B0C           MOV EDX, CryGame.0C5B4DF4                ; ASCII "ItemNum"
	$+4C0    0B2B438D      E8 FECCD6FF           CALL CryGame.0B021090
	$+4C5    0B2B4392      0FB656 34             MOVZX EDX, BYTE PTR DS:[ESI+34]
	$+4C9    0B2B4396      83C4 0C               ADD ESP, 0C
	$+4CC    0B2B4399      52                    PUSH EDX
	$+4CD    0B2B439A      8D4424 34             LEA EAX, DWORD PTR SS:[ESP+34]
	$+4D1    0B2B439E      50                    PUSH EAX
	$+4D2    0B2B439F      53                    PUSH EBX
	$+4D3    0B2B43A0      BA FC4D5B0C           MOV EDX, CryGame.0C5B4DFC                ; ASCII "UnBindGold"
	$+4D8    0B2B43A5      E8 56CDD6FF           CALL CryGame.0B021100
	$+4DD    0B2B43AA      8B8C24 BC000000       MOV ECX, DWORD PTR SS:[ESP+BC]           ; mhoclien.005C0073
	$+4E4    0B2B43B1      83C4 0C               ADD ESP, 0C
	$+4E7    0B2B43B4      51                    PUSH ECX                                 ; ntdll.779301E4
	$+4E8    0B2B43B5      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+4EC    0B2B43B9      52                    PUSH EDX
	$+4ED    0B2B43BA      53                    PUSH EBX
	$+4EE    0B2B43BB      BA 084E5B0C           MOV EDX, CryGame.0C5B4E08                ; ASCII "salesCount"
	$+4F3    0B2B43C0      E8 CBCCD6FF           CALL CryGame.0B021090
	$+4F8    0B2B43C5      83C4 0C               ADD ESP, 0C
	$+4FB    0B2B43C8      E8 F3B6FBFF           CALL CryGame.0B26FAC0
	$+500    0B2B43CD      894424 18             MOV DWORD PTR SS:[ESP+18], EAX
	$+504    0B2B43D1      897C24 0C             MOV DWORD PTR SS:[ESP+C], EDI
	$+508    0B2B43D5      3BC7                  CMP EAX, EDI
	$+50A    0B2B43D7      74 20                 JE SHORT CryGame.0B2B43F9
	$+50C    0B2B43D9      8D88 F60D0000         LEA ECX, DWORD PTR DS:[EAX+DF6]
	$+512    0B2B43DF      E8 6CEEBEFF           CALL CryGame.0AEA3250
	$+517    0B2B43E4      8B4C24 18             MOV ECX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+51B    0B2B43E8      81C1 EE0D0000         ADD ECX, 0DEE
	$+521    0B2B43EE      8BF8                  MOV EDI, EAX
	$+523    0B2B43F0      E8 5BEEBEFF           CALL CryGame.0AEA3250
	$+528    0B2B43F5      894424 0C             MOV DWORD PTR SS:[ESP+C], EAX
	$+52C    0B2B43F9      57                    PUSH EDI
	$+52D    0B2B43FA      8D4424 34             LEA EAX, DWORD PTR SS:[ESP+34]
	$+531    0B2B43FE      50                    PUSH EAX
	$+532    0B2B43FF      53                    PUSH EBX
	$+533    0B2B4400      BA 144E5B0C           MOV EDX, CryGame.0C5B4E14                ; ASCII "currentBindMoney"
	$+538    0B2B4405      E8 86CCD6FF           CALL CryGame.0B021090
	$+53D    0B2B440A      8B4C24 18             MOV ECX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+541    0B2B440E      83C4 0C               ADD ESP, 0C
	$+544    0B2B4411      51                    PUSH ECX                                 ; ntdll.779301E4
	$+545    0B2B4412      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+549    0B2B4416      52                    PUSH EDX
	$+54A    0B2B4417      53                    PUSH EBX
	$+54B    0B2B4418      BA 284E5B0C           MOV EDX, CryGame.0C5B4E28                ; ASCII "currentMoney"
	$+550    0B2B441D      E8 6ECCD6FF           CALL CryGame.0B021090
	$+555    0B2B4422      8B8424 84000000       MOV EAX, DWORD PTR SS:[ESP+84]           ; mhoclien.01E27E5C
	$+55C    0B2B4429      83C4 0C               ADD ESP, 0C
	$+55F    0B2B442C      50                    PUSH EAX
	$+560    0B2B442D      8D4C24 34             LEA ECX, DWORD PTR SS:[ESP+34]
	$+564    0B2B4431      51                    PUSH ECX                                 ; ntdll.779301E4
	$+565    0B2B4432      53                    PUSH EBX
	$+566    0B2B4433      BA 384E5B0C           MOV EDX, CryGame.0C5B4E38                ; ASCII "backItemIndex"
	$+56B    0B2B4438      E8 73CBD6FF           CALL CryGame.0B020FB0
	$+570    0B2B443D      8B56 28               MOV EDX, DWORD PTR DS:[ESI+28]           ; KERNELBA.771C4498
	$+573    0B2B4440      83C4 0C               ADD ESP, 0C
	$+576    0B2B4443      52                    PUSH EDX
	$+577    0B2B4444      E8 5709D0FF           CALL CryGame.0AFB4DA0
	$+57C    0B2B4449      8BF8                  MOV EDI, EAX
	$+57E    0B2B444B      83C4 04               ADD ESP, 4
	$+581    0B2B444E      85FF                  TEST EDI, EDI
	$+583    0B2B4450      0F84 04010000         JE CryGame.0B2B455A
	$+589    0B2B4456      8B4C24 2C             MOV ECX, DWORD PTR SS:[ESP+2C]
	$+58D    0B2B445A      6A 01                 PUSH 1
	$+58F    0B2B445C      56                    PUSH ESI
	$+590    0B2B445D      E8 8EBAE300           CALL CryGame.0C0EFEF0
	$+595    0B2B4462      8B4C24 28             MOV ECX, DWORD PTR SS:[ESP+28]
	$+599    0B2B4466      50                    PUSH EAX
	$+59A    0B2B4467      8B46 08               MOV EAX, DWORD PTR DS:[ESI+8]
	$+59D    0B2B446A      50                    PUSH EAX
	$+59E    0B2B446B      51                    PUSH ECX                                 ; ntdll.779301E4
	$+59F    0B2B446C      E8 4F250000           CALL CryGame.0B2B69C0
	$+5A4    0B2B4471      83C4 0C               ADD ESP, 0C
	$+5A7    0B2B4474      837C24 18 00          CMP DWORD PTR SS:[ESP+18], 0
	$+5AC    0B2B4479      894424 0C             MOV DWORD PTR SS:[ESP+C], EAX
	$+5B0    0B2B447D      74 27                 JE SHORT CryGame.0B2B44A6
	$+5B2    0B2B447F      8B4424 18             MOV EAX, DWORD PTR SS:[ESP+18]           ; kernel32.GetTickCount
	$+5B6    0B2B4483      E8 08270000           CALL CryGame.0B2B6B90
	$+5BB    0B2B4488      84C0                  TEST AL, AL
	$+5BD    0B2B448A      74 1A                 JE SHORT CryGame.0B2B44A6
	$+5BF    0B2B448C      8B56 08               MOV EDX, DWORD PTR DS:[ESI+8]
	$+5C2    0B2B448F      8B46 0C               MOV EAX, DWORD PTR DS:[ESI+C]
	$+5C5    0B2B4492      8B4C24 2C             MOV ECX, DWORD PTR SS:[ESP+2C]
	$+5C9    0B2B4496      52                    PUSH EDX
	$+5CA    0B2B4497      50                    PUSH EAX
	$+5CB    0B2B4498      E8 53BFE300           CALL CryGame.0C0F03F0
	$+5D0    0B2B449D      C64424 14 01          MOV BYTE PTR SS:[ESP+14], 1
	$+5D5    0B2B44A2      84C0                  TEST AL, AL
	$+5D7    0B2B44A4      75 05                 JNZ SHORT CryGame.0B2B44AB
	$+5D9    0B2B44A6      C64424 14 00          MOV BYTE PTR SS:[ESP+14], 0
	$+5DE    0B2B44AB      8B4C24 14             MOV ECX, DWORD PTR SS:[ESP+14]
	$+5E2    0B2B44AF      51                    PUSH ECX                                 ; ntdll.779301E4
	$+5E3    0B2B44B0      8D5424 34             LEA EDX, DWORD PTR SS:[ESP+34]
	$+5E7    0B2B44B4      52                    PUSH EDX
	$+5E8    0B2B44B5      53                    PUSH EBX
	$+5E9    0B2B44B6      BA 484E5B0C           MOV EDX, CryGame.0C5B4E48                ; ASCII "itemTypeCanUse"
	$+5EE    0B2B44BB      E8 40CCD6FF           CALL CryGame.0B021100
	--*/
	//feature code:68????????50ff??84??0f??????????8b????????????8b??81??????????80????0f??????????c1????a8??74??8b????????????8b??89????????????eb??8b????????????89????????????8b??????52e8????????83
	DWORD	dwMMOClientgetEquipInfoBaseCall = (DWORD)scan_game_.ScanCall("68????????50ff??84??0f??????????8b????????????8b??81??????????80????0f??????????c1????a8??74??8b????????????8b??89????????????eb??8b????????????89????????????8b??????52e8????????83",
		0x54 + 1);
	assert(dwMMOClientgetEquipInfoBaseCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_EQUIP_INF0_BASE_ADDR_CALL  " << dwMMOClientgetEquipInfoBaseCall - GetModuleCryGameBase() << "  // " << dwMMOClientgetEquipInfoBaseCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    006E9249      53                   PUSH EBX
	$+1      006E924A      56                   PUSH ESI
	$+2      006E924B      57                   PUSH EDI
	$+3      006E924C      FFD2                 CALL EDX
	$+5      006E924E      8BF0                 MOV ESI,EAX
	$+7      006E9250      33DB                 XOR EBX,EBX
	$+9      006E9252      33C0                 XOR EAX,EAX
	$+B      006E9254      897424 1C            MOV DWORD PTR SS:[ESP+0x1C],ESI
	$+F      006E9258      3BF3                 CMP ESI,EBX
	$+11     006E925A      0F84 89000000        JE MHOClien.006E92E9
	$+17     006E9260      8B45 08              MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1A     006E9263      50                   PUSH EAX                                 ; 锻造id
	$+1B     006E9264      E8 5794E7FF          CALL MHOClien.005626C0                   ; 获取该id的锻造对象
	$+20     006E9269      8B50 44              MOV EDX,DWORD PTR DS:[EAX+0x44]
	$+23     006E926C      33C9                 XOR ECX,ECX
	$+25     006E926E      895C24 7C            MOV DWORD PTR SS:[ESP+0x7C],EBX
	$+29     006E9272      899C24 80000000      MOV DWORD PTR SS:[ESP+0x80],EBX

	535657FFD28BF033DB33C089??????3BF3

	$ ==>    0060D646      8B50 24         MOV EDX,DWORD PTR DS:[EAX+0x24]
	$+3      0060D649      53              PUSH EBX
	$+4      0060D64A      56              PUSH ESI
	$+5      0060D64B      57              PUSH EDI
	$+6      0060D64C      FFD2            CALL EDX
	$+8      0060D64E      8BF8            MOV EDI,EAX
	$+A      0060D650      33DB            XOR EBX,EBX
	$+C      0060D652      33C0            XOR EAX,EAX
	$+E      0060D654      897C24 1C       MOV DWORD PTR SS:[ESP+0x1C],EDI
	$+12     0060D658      3BFB            CMP EDI,EBX
	$+14     0060D65A      0F84 89000000   JE MHOClien.0060D6E9
	$+1A     0060D660      8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1D     0060D663      50              PUSH EAX
	$+1E     0060D664      E8 6789F5FF     CALL MHOClien.00565FD0                   ; ASCII "CCraftInfo"  锻造物品查询
	$+23     0060D669      8B50 44         MOV EDX,DWORD PTR DS:[EAX+0x44]

	8B??????????FF??8B??33??33??89??????3B??0F84

	$ ==>    >  E8 0164FCFE            CALL MHOClien.00566360                                     ; "CCraftInfo"  获取锻造物品地址
	$+5      >  8BF0                   MOV ESI,EAX
	$+7      >  83C4 04                ADD ESP,0x4
	$+A      >  85F6                   TEST ESI,ESI
	$+C      >  74 49                  JE SHORT MHOClien.0159FFB1
	$+E      >  85FF                   TEST EDI,EDI
	$+10     >  74 45                  JE SHORT MHOClien.0159FFB1
	$+12     >  8D8F DE0D0000          LEA ECX,DWORD PTR DS:[EDI+0xDDE]
	$+18     >  E8 F995ECFE            CALL MHOClien.00469570
	$+1D     >  3946 68                CMP DWORD PTR DS:[ESI+0x68],EAX
	$+20     >  7E 31                  JLE SHORT MHOClien.0159FFAD
	$+22     >  8D9F E60D0000          LEA EBX,DWORD PTR DS:[EDI+0xDE6]
	$+28     >  8BCB                   MOV ECX,EBX
	$+2A     >  E8 E795ECFE            CALL MHOClien.00469570

	E8????????8B??83????85??74??85??74??8D??????????E8


	--*/
	DWORD	dwMMOClientGetCraftInfoAddrCall = (DWORD)scan_game_.ScanCall("E8????????8B??83????85??74??85??74??8D??????????E8", 0x1, 2);
	assert(dwMMOClientGetCraftInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_CRAFT_INFO_ADDR_CALL  " << dwMMOClientGetCraftInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetCraftInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    >  8B81 1C0D0000   MOV EAX,DWORD PTR DS:[ECX+0xD1C]
	$+6      >  85C0            TEST EAX,EAX
	$+8      >  75 05           JNZ SHORT CryGame.0D55475F
	$+A      >  E8 B15FBFFF     CALL CryGame.0D14A710
	$+F      >  33C9            XOR ECX,ECX
	$+11     >  8378 1C 01      CMP DWORD PTR DS:[EAX+0x1C],0x1
	$+15     >  0F94C1          SETE CL
	$+18     >  8AC1            MOV AL,CL
	$+1A     >  C3              RETN

	8B81????????85C075??E8????????33C983
	--*/
	DWORD	dwMMOClientTraverseObjXyzOffsetName4 = *scan_game_.ScanImme<DWORD>("8B81????????85C075??E8????????33C983", 0x2);
	assert(dwMMOClientTraverseObjXyzOffsetName4);
	//assert(dwMMOClientTraverseObjXyzOffsetName4==0x00000D2C  /*0xD1C*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME4  " << dwMMOClientTraverseObjXyzOffsetName4 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00476140       8B0D 0C595802       MOV ECX,DWORD PTR DS:[0x258590C]                          ; MHOClien.0209E0A0
	$+6      00476146       8B91 C8000000       MOV EDX,DWORD PTR DS:[ECX+0xC8]
	$+C      0047614C       8B4A 28             MOV ECX,DWORD PTR DS:[EDX+0x28]
	$+F      0047614F       8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+11     00476151       8B90 A8000000       MOV EDX,DWORD PTR DS:[EAX+0xA8]
	$+17     00476157       FFD2                CALL EDX    // MOV EAX,DWORD PTR DS:[ECX+0x214]
	$+19     00476159       3B46 2C             CMP EAX,DWORD PTR DS:[ESI+0x2C]
	$+1C     0047615C       A1 0C595802         MOV EAX,DWORD PTR DS:[0x258590C]
	$+21     00476161       74 34               JE SHORT MHOClien.00476197
	$+23     00476163       8B88 C8000000       MOV ECX,DWORD PTR DS:[EAX+0xC8]
	$+29     00476169       8B49 28             MOV ECX,DWORD PTR DS:[ECX+0x28]
	$+2C     0047616C       8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+2E     0047616E       8B82 A8000000       MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+34     00476174       FFD0                CALL EAX

	8B??????????8B??????????8B????8B??8B??????????FF??3B????A1????????74
	--*/
	DWORD	dwMMOClientActorOffset1 = *scan_game_.ScanImme<DWORD>("8B??????????8B??????????8B????8B??8B??????????FF??3B????A1????????74", 0x8);
	assert(dwMMOClientActorOffset1);
	assert(dwMMOClientActorOffset1 == 0x000000D0/*0x000000CC*/  /*0x000000C8*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET1  " << dwMMOClientActorOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientActorOffset2 = *scan_game_.ScanImme<byte>("8B??????????8B??????????8B????8B??8B??????????FF??3B????A1????????74", 0x0E);
	assert(dwMMOClientActorOffset2);
	assert(dwMMOClientActorOffset2 == 0x0000007C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET2  " << dwMMOClientActorOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;



	/*++
	$ ==>    005F29D0       8B41 08             MOV EAX,DWORD PTR DS:[ECX+0x8]
	$+3      005F29D3       85C0                TEST EAX,EAX
	$+5      005F29D5       75 03               JNZ SHORT MHOClien.005F29DA
	$+7      005F29D7       32C0                XOR AL,AL
	$+9      005F29D9       C3                  RETN
	$+A      005F29DA       8378 10 00          CMP DWORD PTR DS:[EAX+0x10],0x0
	$+E      005F29DE     ^ 74 F7               JE SHORT MHOClien.005F29D7
	$+10     005F29E0       33C9                XOR ECX,ECX
	$+12     005F29E2       3948 58             CMP DWORD PTR DS:[EAX+0x58],ECX
	$+15     005F29E5       0F94C1              SETE CL
	$+18     005F29E8       8AC1                MOV AL,CL
	$+1A     005F29EA       C3                  RETN

	8B????85??75??32C0??83??????74
	--*/
	DWORD	dwMMOClientActorInMapOffset2 = *scan_game_.ScanImme<byte>("8B????85??75??32C0??83??????74", 0x02);
	assert(dwMMOClientActorInMapOffset2);
	assert(dwMMOClientActorInMapOffset2 == 0x0000068);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_IN_MAP_OFFFSET2  " << dwMMOClientActorInMapOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    006E33E0        8B47 24           MOV EAX,DWORD PTR DS:[EDI+0x24]          ; MOV EAX,DWORD PTR DS:[EDI+0x24]
	$+3      006E33E3        8B88 10210000     MOV ECX,DWORD PTR DS:[EAX+0x2110]        ; MOV ECX,DWORD PTR DS:[EAX+0x2110]
	$+9      006E33E9        85C9              TEST ECX,ECX
	$+B      006E33EB        74 5E             JE SHORT MHOClien.006E344B
	$+D      006E33ED        56                PUSH ESI
	$+E      006E33EE        53                PUSH EBX
	$+F      006E33EF        E8 1C8BF2FF       CALL MHOClien.0060BF10
	$+14     006E33F4        85C0              TEST EAX,EAX                             ; eax
	$+16     006E33F6        75 0F             JNZ SHORT MHOClien.006E3407
	$+18     006E33F8        B8 02000000       MOV EAX,0x2

	$ ==>    0060250F       8B4F 24         MOV ECX,DWORD PTR DS:[EDI+0x24]
	$+3      00602512       8B89 78210000   MOV ECX,DWORD PTR DS:[ECX+0x2178]
	$+9      00602518       85C9            TEST ECX,ECX
	$+B      0060251A       74 56           JE SHORT MHOClien.00602572
	$+D      0060251C       56              PUSH ESI
	$+E      0060251D       53              PUSH EBX
	$+F      0060251E       E8 5D555B01     CALL MHOClien.01BB7A80
	$+14     00602523       85C0            TEST EAX,EAX
	$+16     00602525       75 0E           JNZ SHORT MHOClien.00602535
	$+18     00602527       B8 02000000     MOV EAX,0x2

	8B????8B??????????85??74??????E8????????85??75
	--*/
	// 	DWORD	dwMMOClientUpEquipOffset2 = ScanImme((HANDLE)-1,"8B????8B??????????85??74??????E8????????85??75",0x02,1);
	// 	ASSERT_GWLR(dwMMOClientUpEquipOffset2);
	// 	ASSERT_GWLR(dwMMOClientUpEquipOffset2==0x00000024);
	// 	LOG << "#define UPDATES_";
	// 	LOG << "MMOCLIENT_REVERSAL_UP_EQUIP_OFFSET2  ";
	// 	LOG << dwMMOClientUpEquipOffset2;
	// 	LOG_END;
	// 
	// 	DWORD	dwMMOClientUpEquipOffset3 = ScanImme((HANDLE)-1,"8B????8B??????????85??74??????E8????????85??75",0x05);
	// 	ASSERT_GWLR(dwMMOClientUpEquipOffset3);
	// 	ASSERT_GWLR(dwMMOClientUpEquipOffset3==0x2178);
	// 	LOG << "#define UPDATES_";
	// 	LOG << "MMOCLIENT_REVERSAL_UP_EQUIP_OFFSET3  ";
	// 	LOG << dwMMOClientUpEquipOffset3;
	// 	LOG_END;

	/*++
	$ ==>    007A90A9       8B0D B4F00902             MOV ECX,DWORD PTR DS:[0x209F0B4]         ; 基址
	$+6      007A90AF       8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$+8      007A90B1       8B45 5C                   MOV EAX,DWORD PTR SS:[EBP+0x5C]
	$+B      007A90B4       8B52 3C                   MOV EDX,DWORD PTR DS:[EDX+0x3C]
	$+E      007A90B7       50                        PUSH EAX
	$+F      007A90B8       FFD2                      CALL EDX
	$+11     007A90BA       C705 BCF50902 00000000    MOV DWORD PTR DS:[0x209F5BC],0x0
	$+1B     007A90C4       C745 58 E08C1002          MOV DWORD PTR SS:[EBP+0x58],MHOClien.021>
	$+22     007A90CB       8B55 58                   MOV EDX,DWORD PTR SS:[EBP+0x58]
	$+25     007A90CE       B8 0000FFFF               MOV EAX,0xFFFF0000
	$+2A     007A90D3       F0:0102                   LOCK ADD DWORD PTR DS:[EDX],EAX

	8B??????????8B??8B????8B????50FF??C705
	--*/
	DWORD	dwMMOClientDxKeyBoardPressBaseAddr = *scan_exe_.ScanImme<DWORD>("8B??????????8B??8B????8B????50FF??C705", 2);
	assert(dwMMOClientDxKeyBoardPressBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_BASE_ADDR  " << dwMMOClientDxKeyBoardPressBaseAddr << "  // MHOClient.exe" << FC_OFstream_EndlLog;


	/*++

	$ ==>    018367CE       8D7424 14            LEA ESI,DWORD PTR SS:[ESP+0x14]
	$+4      018367D2       8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+7      018367D5       8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+9      018367D7       C1E9 07              SHR ECX,0x7
	$+C      018367DA       80E1 01              AND CL,0x1
	$+F      018367DD       0FB6D1               MOVZX EDX,CL
	$+12     018367E0       52                   PUSH EDX
	$+13     018367E1       50                   PUSH EAX
	$+14     018367E2       8BCD                 MOV ECX,EBP
	$+16     018367E4       E8 17FBFFFF          CALL MHOClien.01836300
	$+1B     018367E9       47                   INC EDI
	$+1C     018367EA       83C6 14              ADD ESI,0x14
	$+1F     018367ED       3B7C24 10            CMP EDI,DWORD PTR SS:[ESP+0x10]
	$+23     018367F1     ^ 72 DF                JB SHORT MHOClien.018367D2

	8D??????8B????8B??C1????80????0F

	$ ==>    016A97C3       8B4E 04                MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+3      016A97C6       8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+5      016A97C8       C1E9 07                SHR ECX,0x7
	$+8      016A97CB       80E1 01                AND CL,0x1
	$+B      016A97CE       0FB6D1                 MOVZX EDX,CL
	$+E      016A97D1       52                     PUSH EDX
	$+F      016A97D2       50                     PUSH EAX
	$+10     016A97D3       8BCB                   MOV ECX,EBX
	$+12     016A97D5       E8 26FBFFFF            CALL MHOClien.016A9300
	$+17     016A97DA       47                     INC EDI
	$+18     016A97DB       83C6 14                ADD ESI,0x14
	$+1B     016A97DE       3B7D FC                CMP EDI,DWORD PTR SS:[EBP-0x4]
	$+1E     016A97E1     ^ 72 E0                  JB SHORT MHOClien.016A97C3

	8B????8B??C1????80????0F
	--*/
	DWORD	dwMMOClientDxKeyBoardPressCall = (DWORD)scan_exe_.ScanCall("8B????8B??C1????80????0F", 0x13);
	assert(dwMMOClientDxKeyBoardPressCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_CALL  " << dwMMOClientDxKeyBoardPressCall << "  // MHOClient.exe" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientDxKeyBoardPressHook = (DWORD)scan_exe_.ScanAddr("8B????8B??C1????80????0F", 0x0B);
	assert(dwMMOClientDxKeyBoardPressHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_HOOK  " << dwMMOClientDxKeyBoardPressHook << "  // MHOClient.exe" << FC_OFstream_EndlLog;




	/*++
	$ ==>    >  8B46 0C         MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+3      >  85C0            TEST EAX,EAX
	$+5      >  76 23           JBE SHORT CryGame.0CBB742B
	$+7      >  8B0D 7CDAAC0D   MOV ECX,DWORD PTR DS:[0xDACDA7C]         ; CryGame.0DACA348
	$+D      >  50              PUSH EAX
	$+E      >  E8 8C920A00     CALL CryGame.0CC606A0
	$+13     >  8BC8            MOV ECX,EAX
	$+15     >  E8 95902600     CALL CryGame.0CE204B0
	$+1A     >  85C0            TEST EAX,EAX
	$+1C     >  74 0C           JE SHORT CryGame.0CBB742B
	$+1E     >  8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+20     >  8BC8            MOV ECX,EAX
	$+22     >  8B82 B4010000   MOV EAX,DWORD PTR DS:[EDX+0x1B4]
	$+28     >  FFD0            CALL EAX
	$+2A     >  8B0D F09EAC0D   MOV ECX,DWORD PTR DS:[0xDAC9EF0]         ; MHOClien.015D3088
	$+30     >  80B9 F9010000 0>CMP BYTE PTR DS:[ECX+0x1F9],0x0
	$+37     >  75 1A           JNZ SHORT CryGame.0CBB7454
	$+39     >  807D FF 00      CMP BYTE PTR SS:[EBP-0x1],0x0
	$+3D     >  75 14           JNZ SHORT CryGame.0CBB7454
	$+3F     >  807D 0B 00      CMP BYTE PTR SS:[EBP+0xB],0x0
	$+43     >  75 0E           JNZ SHORT CryGame.0CBB7454
	$+45     >  84DB            TEST BL,BL
	$+47     >  75 0A           JNZ SHORT CryGame.0CBB7454
	$+49     >  385D FE         CMP BYTE PTR SS:[EBP-0x2],BL
	$+4C     >  75 05           JNZ SHORT CryGame.0CBB7454
	$+4E     >  385D FD         CMP BYTE PTR SS:[EBP-0x3],BL
	$+51     >  74 60           JE SHORT CryGame.0CBB74B4
	$+53     >  8B0D 7CDAAC0D   MOV ECX,DWORD PTR DS:[0xDACDA7C]         ; CryGame.0DACA348
	$+59     >  E8 41920A00     CALL CryGame.0CC606A0
	$+5E     >  807D FC 00      CMP BYTE PTR SS:[EBP-0x4],0x0
	$+62     >  8BF8            MOV EDI,EAX
	$+64     >  74 0A           JE SHORT CryGame.0CBB7471
	$+66     >  807D FB 00      CMP BYTE PTR SS:[EBP-0x5],0x0
	$+6A     >  75 04           JNZ SHORT CryGame.0CBB7471
	$+6C     >  84DB            TEST BL,BL
	$+6E     >  74 12           JE SHORT CryGame.0CBB7483
	$+70     >  807D FA 00      CMP BYTE PTR SS:[EBP-0x6],0x0
	$+74     >  75 0C           JNZ SHORT CryGame.0CBB7483
	$+76     >  807D F9 00      CMP BYTE PTR SS:[EBP-0x7],0x0
	$+7A     >  75 06           JNZ SHORT CryGame.0CBB7483
	$+7C     >  807D F8 00      CMP BYTE PTR SS:[EBP-0x8],0x0
	$+80     >  74 29           JE SHORT CryGame.0CBB74AC
	$+82     >  807D F7 00      CMP BYTE PTR SS:[EBP-0x9],0x0
	$+86     >  75 23           JNZ SHORT CryGame.0CBB74AC
	$+88     >  807D FE 00      CMP BYTE PTR SS:[EBP-0x2],0x0
	$+8C     >  75 1D           JNZ SHORT CryGame.0CBB74AC
	$+8E     >  8B4D EC         MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+91     >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+93     >  8B82 F0000000   MOV EAX,DWORD PTR DS:[EDX+0xF0]
	$+99     >  FFD0            CALL EAX
	$+9B     >  84C0            TEST AL,AL
	$+9D     >  75 0C           JNZ SHORT CryGame.0CBB74AC
	$+9F     >  8B0D 7CDAAC0D   MOV ECX,DWORD PTR DS:[0xDACDA7C]         ; CryGame.0DACA348
	$+A5     >  56              PUSH ESI
	$+A6     >  E8 B4960B00     CALL CryGame.0CC70B60

	8B????85??76??8B????????????E8????????8B??E8????????85??74??8B??8B??8B??????????FF
	--*/
	DWORD	dwMMOClientHitCallHook = (DWORD)scan_game_.ScanAddr("8B????85??76??8B????????????E8????????8B??E8????????85??74??8B??8B??8B??????????FF", 0xA6);
	assert(dwMMOClientHitCallHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_CALL_HOOK  " << dwMMOClientHitCallHook - GetModuleCryGameBase() << "  // " << dwMMOClientHitCallHook << " CryGame.dll" << FC_OFstream_EndlLog;

	// 	DWORD	dwMMOClientHitCall = CRY_GAME_ScanCall((HANDLE)-1,"8B????85??76??8B????????????E8????????8B??E8????????85??74??8B??8B??8B??????????FF",0xA7,4,1);
	// 	ASSERT_GWLR(dwMMOClientHitCall);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_CALL  ";
	// 	LOG << dwMMOClientHitCall-GetModuleCryGameBase();
	// 	LOG << "  // ";
	// 	LOG << dwMMOClientHitCall;
	// 	LOG << " CryGame.dll";
	// 	LOG_END;   //没用，等会更新

	// 	DWORD	dwMMOClientHitBaseAddr = CRY_GAME_ScanImme((HANDLE)-1,"8B????85??76??8B????????????E8????????8B??E8????????85??74??8B??8B??8B??????????FF",0xA1,4,1);
	// 	ASSERT_GWLR(dwMMOClientHitBaseAddr);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_BASE_ADDR  ";
	// 	LOG << dwMMOClientHitBaseAddr-GetModuleCryGameBase();
	// 	LOG << "  // ";
	// 	LOG << dwMMOClientHitBaseAddr;
	// 	LOG << " CryGame.dll";
	// 	LOG_END;




	/*++
	// 先遣测试版本
	$ ==>    >  D983 D4000000        FLD DWORD PTR DS:[EBX+0xD4]
	$+6      >  D99E BC0C0000        FSTP DWORD PTR DS:[ESI+0xCBC]
	$+C      >  D983 D8000000        FLD DWORD PTR DS:[EBX+0xD8]
	$+12     >  D99E C00C0000        FSTP DWORD PTR DS:[ESI+0xCC0]
	$+18     >  D983 DC000000        FLD DWORD PTR DS:[EBX+0xDC]
	$+1E     >  D99E C40C0000        FSTP DWORD PTR DS:[ESI+0xCC4]
	$+24     >  D983 E0000000        FLD DWORD PTR DS:[EBX+0xE0]
	$+2A     >  D99E C80C0000        FSTP DWORD PTR DS:[ESI+0xCC8]
	$+30     >  D983 E4000000        FLD DWORD PTR DS:[EBX+0xE4]
	$+36     >  D99E CC0C0000        FSTP DWORD PTR DS:[ESI+0xCCC]
	$+3C     >  D983 E8000000        FLD DWORD PTR DS:[EBX+0xE8]
	$+42     >  D99E D00C0000        FSTP DWORD PTR DS:[ESI+0xCD0]
	$+48     >  D983 EC000000        FLD DWORD PTR DS:[EBX+0xEC]
	$+4E     >  D99E D40C0000        FSTP DWORD PTR DS:[ESI+0xCD4]
	$+54     >  D983 F0000000        FLD DWORD PTR DS:[EBX+0xF0]
	$+5A     >  D99E D80C0000        FSTP DWORD PTR DS:[ESI+0xCD8]

	D983????????D99E????????D983????????D99E????????D983

	// 不删档测试版本
	$ ==>    >  E8 C138DCFF     CALL CryGame.0CA9A550
	$+5      >  83EC 1C         SUB ESP,0x1C
	$+8      >  8BFC            MOV EDI,ESP
	$+A      >  B9 07000000     MOV ECX,0x7
	$+F      >  8D75 D0         LEA ESI,DWORD PTR SS:[EBP-0x30]
	$+12     >  F3:A5           REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
	$+14     >  8B75 F4         MOV ESI,DWORD PTR SS:[EBP-0xC]
	$+17     >  8BCE            MOV ECX,ESI
	$+19     >  E8 E8FBDCFF     CALL CryGame.0CAA6890

	E8????????83????8B??B9????????8D


	//小陈修改
	$ ==>    >  8B4E 0C         mov ecx,dword ptr ds:[esi+0xC]
	$+3      >  8855 F8         mov byte ptr ss:[ebp-0x8],dl
	$+6      >  8B11            mov edx,dword ptr ds:[ecx]
	$+8      >  8B82 A0010000   mov eax,dword ptr ds:[edx+0x1A0]
	$+E      >  57              push edi
	$+F      >  FFD0            call eax
	$+11     >  3945 FC         cmp dword ptr ss:[ebp-0x4],eax
	$+14     >  74 04           je short CryGame.10EB5625
	$+16     >  C645 F8 01      mov byte ptr ss:[ebp-0x8],0x1
	$+1A     >  8B4D F8         mov ecx,dword ptr ss:[ebp-0x8]
	$+1D     >  8B55 E8         mov edx,dword ptr ss:[ebp-0x18]
	$+20     >  51              push ecx
	$+21     >  52              push edx
	$+22     >  8BCE            mov ecx,esi
	$+24     >  E8 0C1EDEFF     call CryGame.10C97440
	$+29     >  33C0            xor eax,eax
	$+2B     >  8BC8            mov ecx,eax
	$+2D     >  894D C4         mov dword ptr ss:[ebp-0x3C],ecx
	$+30     >  8BD0            mov edx,eax
	$+32     >  8955 CC         mov dword ptr ss:[ebp-0x34],edx
	$+35     >  894D C8         mov dword ptr ss:[ebp-0x38],ecx
	$+38     >  8945 D0         mov dword ptr ss:[ebp-0x30],eax
	$+3B     >  8845 C4         mov byte ptr ss:[ebp-0x3C],al
	$+3E     >  8995 6CFFFFFF   mov dword ptr ss:[ebp-0x94],edx
	$+44     >  897D A8         mov dword ptr ss:[ebp-0x58],edi
	$+47     >  897D AC         mov dword ptr ss:[ebp-0x54],edi
	$+4A     >  897D B0         mov dword ptr ss:[ebp-0x50],edi
	$+4D     >  897D B4         mov dword ptr ss:[ebp-0x4C],edi
	$+50     >  897D B8         mov dword ptr ss:[ebp-0x48],edi
	$+53     >  897D BC         mov dword ptr ss:[ebp-0x44],edi
	$+56     >  897D C0         mov dword ptr ss:[ebp-0x40],edi
	$+59     >  898D 64FFFFFF   mov dword ptr ss:[ebp-0x9C],ecx
	$+5F     >  8A4D DF         mov cl,byte ptr ss:[ebp-0x21]
	$+62     >  884D D8         mov byte ptr ss:[ebp-0x28],cl
	$+65     >  897D C8         mov dword ptr ss:[ebp-0x38],edi
	$+68     >  897D D4         mov dword ptr ss:[ebp-0x2C],edi
	$+6B     >  8985 68FFFFFF   mov dword ptr ss:[ebp-0x98],eax
	$+71     >  8985 70FFFFFF   mov dword ptr ss:[ebp-0x90],eax
	$+77     >  8D55 C4         lea edx,dword ptr ss:[ebp-0x3C]
	$+7A     >  8955 CC         mov dword ptr ss:[ebp-0x34],edx
	$+7D     >  8BC2            mov eax,edx
	$+7F     >  8945 D0         mov dword ptr ss:[ebp-0x30],eax
	$+82     >  8B93 D8000000   mov edx,dword ptr ds:[ebx+0xD8]
	$+88     >  8955 B8         mov dword ptr ss:[ebp-0x48],edx
	$+8B     >  8B83 DC000000   mov eax,dword ptr ds:[ebx+0xDC]
	$+91     >  8945 BC         mov dword ptr ss:[ebp-0x44],eax
	$+94     >  8B8B CC000000   mov ecx,dword ptr ds:[ebx+0xCC]
	$+9A     >  894D AC         mov dword ptr ss:[ebp-0x54],ecx
	$+9D     >  8B93 D0000000   mov edx,dword ptr ds:[ebx+0xD0]
	$+A3     >  8955 B0         mov dword ptr ss:[ebp-0x50],edx
	$+A6     >  8B83 C8000000   mov eax,dword ptr ds:[ebx+0xC8]
	$+AC     >  8945 A8         mov dword ptr ss:[ebp-0x58],eax
	$+AF     >  8B8B E0000000   mov ecx,dword ptr ds:[ebx+0xE0]
	$+B5     >  894D C0         mov dword ptr ss:[ebp-0x40],ecx
	$+B8     >  8B93 D4000000   mov edx,dword ptr ds:[ebx+0xD4]
	$+BE     >  8BCE            mov ecx,esi
	$+C0     >  8955 B4         mov dword ptr ss:[ebp-0x4C],edx
	$+C3     >  E8 5D9CDBFF     call CryGame.10C6F330
	$+C8     >  8B4D A8         mov ecx,dword ptr ss:[ebp-0x58]
	$+CB     >  83EC 34         sub esp,0x34
	$+CE     >  8BC4            mov eax,esp


	--*/
	DWORD	dwMMOClientRetRoleChooseJieMianHook = (DWORD)scan_game_.ScanAddr("8B4E0C8855F88B11", 0xCB);
	assert(dwMMOClientRetRoleChooseJieMianHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RET_ROLE_CHOOSE_JIE_MIAN_HOOK  " << dwMMOClientRetRoleChooseJieMianHook - GetModuleCryGameBase() << "  // " << dwMMOClientRetRoleChooseJieMianHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-5      >  BA F012A90D     MOV EDX,CryGame.0DA912F0                                         ; ASCII "curendure"
	$ ==>    >  E8 CD93D3FF     CALL CryGame.0C669640
	$+5      >  83C4 0C         ADD ESP,0xC
	$+8      >  8BCE            MOV ECX,ESI
	$+A      >  E8 335A0000     CALL CryGame.0C935CB0
	$+F      >  50              PUSH EAX
	$+10     >  8D5424 1C       LEA EDX,DWORD PTR SS:[ESP+0x1C]
	$+14     >  52              PUSH EDX
	$+15     >  57              PUSH EDI
	$+16     >  BA FC12A90D     MOV EDX,CryGame.0DA912FC                                         ; ASCII "Attack"
	$+1B     >  E8 B293D3FF     CALL CryGame.0C669640
	$+20     >  8DB3 34040000   LEA ESI,DWORD PTR DS:[EBX+0x434]
	$+26     >  83C4 0C         ADD ESP,0xC
	$+29     >  8BCE            MOV ECX,ESI
	$+2B     >  E8 3223D6FF     CALL CryGame.0C6925D0

	E8????????83????8B??E8??????????8D??????????BA????????E8????????8D
	--*/
	DWORD	dwMMOClientActorLevelOffset = *scan_game_.ScanImme<DWORD>("E8????????83????8B??E8??????????8D??????????BA????????E8????????8D", 0x22);
	assert(dwMMOClientActorLevelOffset);
	//assert(dwMMOClientActorLevelOffset==0x0000043C/*0x00000434*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_LEVEL_OFFSET  " << dwMMOClientActorLevelOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    014D47F0      55              PUSH EBP
	$+1      014D47F1      8BEC            MOV EBP,ESP
	$+3      014D47F3      83E4 F0         AND ESP,0xFFFFFFF0
	$+6      014D47F6      81EC C4000000   SUB ESP,0xC4
	$+C      014D47FC      D9EE            FLDZ
	$+E      014D47FE      53              PUSH EBX
	$+F      014D47FF      D85D 08         FCOMP DWORD PTR SS:[EBP+0x8]
	$+12     014D4802      56              PUSH ESI
	$+13     014D4803      57              PUSH EDI
	$+14     014D4804      8BF9            MOV EDI,ECX
	$+16     014D4806      897C24 44       MOV DWORD PTR SS:[ESP+0x44],EDI
	$+1A     014D480A      DFE0            FSTSW AX
	$+1C     014D480C      F6C4 01         TEST AH,0x1
	$+1F     014D480F      0F84 61060000   JE MHOClien.014D4E76

	558B??83????81EC????????D9EE53D85D

	$-39     0136C9F0       53                     PUSH EBX
	$-38     0136C9F1       8BDC                   MOV EBX,ESP
	$-36     0136C9F3       83EC 08                SUB ESP,0x8
	$-33     0136C9F6       83E4 F0                AND ESP,0xFFFFFFF0
	$-30     0136C9F9       83C4 04                ADD ESP,0x4
	$-2D     0136C9FC       55                     PUSH EBP
	$-2C     0136C9FD       8B6B 04                MOV EBP,DWORD PTR DS:[EBX+0x4]
	$-29     0136CA00       896C24 04              MOV DWORD PTR SS:[ESP+0x4],EBP
	$-25     0136CA04       8BEC                   MOV EBP,ESP
	$-23     0136CA06       81EC C8000000          SUB ESP,0xC8
	$-1D     0136CA0C       D9EE                   FLDZ
	$-1B     0136CA0E       56                     PUSH ESI
	$-1A     0136CA0F       D85B 08                FCOMP DWORD PTR DS:[EBX+0x8]
	$-17     0136CA12       57                     PUSH EDI
	$-16     0136CA13       8BF9                   MOV EDI,ECX
	$-14     0136CA15       897D CC                MOV DWORD PTR SS:[EBP-0x34],EDI
	$-11     0136CA18       DFE0                   FSTSW AX
	$-F      0136CA1A       F6C4 01                TEST AH,0x1
	$-C      0136CA1D       0F84 D2050000          JE MHOClien.0136CFF5
	$-6      0136CA23       8B77 0C                MOV ESI,DWORD PTR DS:[EDI+0xC]
	$-3      0136CA26       8975 94                MOV DWORD PTR SS:[EBP-0x6C],ESI
	$ ==>    0136CA29       85F6                   TEST ESI,ESI
	$+2      0136CA2B       0F84 C4050000          JE MHOClien.0136CFF5
	$+8      0136CA31       837F 34 00             CMP DWORD PTR DS:[EDI+0x34],0x0
	$+C      0136CA35       0F86 BA050000          JBE MHOClien.0136CFF5
	$+12     0136CA3B       8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+14     0136CA3D       8B90 A0010000          MOV EDX,DWORD PTR DS:[EAX+0x1A0]
	$+1A     0136CA43       6A 00                  PUSH 0x0
	$+1C     0136CA45       8BCE                   MOV ECX,ESI
	$+1E     0136CA47       FFD2                   CALL EDX

	85??0F84????????837F????0F86????????8B??8B??????????6A
	--*/
	DWORD	dwMMOClientMonsterHitAddrHook = (DWORD)scan_game_.ScanAddr("85??0F84????????837F????0F86????????8B??8B??????????6A", -0x39);
	assert(dwMMOClientMonsterHitAddrHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_HOOK  ";		// 最开始的时候使用的无敌
	ofs << dwMMOClientMonsterHitAddrHook - GetModuleCryGameBase() << "  // " << dwMMOClientMonsterHitAddrHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    006A0A03       3BC8            CMP ECX,EAX
	$+2      006A0A05     ^ 75 F9           JNZ SHORT MHOClien.006A0A00
	$+4      006A0A07       85D2            TEST EDX,EDX
	$+6      006A0A09       75 28           JNZ SHORT MHOClien.006A0A33
	$+8      006A0A0B       8B46 14         MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+B      006A0A0E       85C0            TEST EAX,EAX
	$+D      006A0A10       75 05           JNZ SHORT MHOClien.006A0A17
	$+F      006A0A12       B8 AE97FC01     MOV EAX,MHOClien.01FC97AE
	$+14     006A0A17       50              PUSH EAX
	$+15     006A0A18       68 00100000     PUSH 0x1000
	$+1A     006A0A1D       68 40E14E02     PUSH MHOClien.024EE140
	$+1F     006A0A22       E8 8DBD4B01     CALL MHOClien.01B5C7B4                   ; JMP 到 msvcr90.sprintf_s
	$+24     006A0A27       83C4 0C         ADD ESP,0xC
	$+27     006A0A2A       8BC6            MOV EAX,ESI
	$+29     006A0A2C       5F              POP EDI
	$+2A     006A0A2D       5E              POP ESI
	$+2B     006A0A2E       5B              POP EBX
	$+2C     006A0A2F       8BE5            MOV ESP,EBP
	$+2E     006A0A31       5D              POP EBP
	$+2F     006A0A32       C3              RETN

	3B??75??85??75??8B????85??75??B8??????????68????????68

	$-D      >  6A 00              PUSH 0x0
	$-B      >  68 B8C6A70D        PUSH CryGame.0DA7C6B8                                            ; ASCII "CMessageInfo"
	$-6      >  6A 00              PUSH 0x0
	$-4      >  C645 FF 01         MOV BYTE PTR SS:[EBP-0x1],0x1
	$ ==>    >  E8 ED290000        CALL CryGame.0C612CD0
	$+5      >  83C4 0C            ADD ESP,0xC
	$+8      >  8BC8               MOV ECX,EAX
	$+A      >  E8 730D0000        CALL CryGame.0C611060
	$+F      >  8BF0               MOV ESI,EAX
	$+11     >  8975 F8            MOV DWORD PTR SS:[EBP-0x8],ESI
	$+14     >  85F6               TEST ESI,ESI
	$+16     >^ 74 CE              JE SHORT CryGame.0C6102C4
	$+18     >  8B4E 40            MOV ECX,DWORD PTR DS:[ESI+0x40]
	$+1B     >  8D46 40            LEA EAX,DWORD PTR DS:[ESI+0x40]
	$+1E     >  33D2               XOR EDX,EDX
	$+20     >  C745 F4 00000000   MOV DWORD PTR SS:[EBP-0xC],0x0
	$+27     >  3BC8               CMP ECX,EAX
	$+29     >  74 12              JE SHORT CryGame.0C61031B
	$+2B     >  8DA424 00000000    LEA ESP,DWORD PTR SS:[ESP]
	$+32     >  8B09               MOV ECX,DWORD PTR DS:[ECX]
	$+34     >  42                 INC EDX
	$+35     >  3BC8               CMP ECX,EAX
	$+37     >^ 75 F9              JNZ SHORT CryGame.0C610310
	$+39     >  85D2               TEST EDX,EDX
	$+3B     >  75 23              JNZ SHORT CryGame.0C61033E
	$+3D     >  8B46 14            MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+40     >  85C0               TEST EAX,EAX
	$+42     >  75 05              JNZ SHORT CryGame.0C610327
	$+44     >  B8 270FA60D        MOV EAX,CryGame.0DA60F27
	$+49     >  8B4D 0C            MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+4C     >  50                 PUSH EAX
	$+4D     >  57                 PUSH EDI
	$+4E     >  51                 PUSH ECX
	$+4F     >  E8 F8D91101        CALL <JMP.&MSVCR90.sprintf_s>
	$+54     >  83C4 0C            ADD ESP,0xC
	$+57     >  8BC6               MOV EAX,ESI
	$+59     >  5F                 POP EDI
	$+5A     >  5E                 POP ESI
	$+5B     >  5B                 POP EBX
	$+5C     >  8BE5               MOV ESP,EBP
	$+5E     >  5D                 POP EBP
	$+5F     >  C3                 RETN

	E8????????83C40C8B??E8????????8B??89????85??74??8B????8D????33??C7
	--*/
	DWORD	dwMMOClientSprintfJuQingHook = (DWORD)scan_game_.ScanAddr("E8????????83C40C8B??E8????????8B??89????85??74??8B????8D????33??C7", 0x5B);
	assert(dwMMOClientSprintfJuQingHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_HOOK  " << dwMMOClientSprintfJuQingHook - GetModuleCryGameBase() << "  // " << dwMMOClientSprintfJuQingHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++

	$ ==>    00604469       8BF9            MOV EDI,ECX
	$+2      0060446B       8B57 04         MOV EDX,DWORD PTR DS:[EDI+0x4]
	$+5      0060446E       8B0F            MOV ECX,DWORD PTR DS:[EDI]
	$+7      00604470       52              PUSH EDX
	$+8      00604471       8D58 14         LEA EBX,DWORD PTR DS:[EAX+0x14]
	$+B      00604474       51              PUSH ECX
	$+C      00604475       8BCB            MOV ECX,EBX
	$+E      00604477       E8 54EB6701     CALL MHOClien.01C82FD0
	$+13     0060447C       8BF0            MOV ESI,EAX
	$+15     0060447E       85F6            TEST ESI,ESI
	$+17     00604480       0F85 AF000000   JNZ MHOClien.00604535
	$+1D     00604486       0FB757 6D       MOVZX EDX,WORD PTR DS:[EDI+0x6D]	// 0x6D
	$+21     0060448A       8985 68FFFFFF   MOV DWORD PTR SS:[EBP-0x98],EAX
	$+27     00604490       8D85 68FFFFFF   LEA EAX,DWORD PTR SS:[EBP-0x98]
	$+2D     00604496       50              PUSH EAX
	$+2E     00604497       8D4F 6F         LEA ECX,DWORD PTR DS:[EDI+0x6F]		// 0x6F
	$+31     0060449A       51              PUSH ECX
	$+32     0060449B       52              PUSH EDX
	$+33     0060449C       E8 EFE36701     CALL MHOClien.01C82890

	8B??8B????8B????8D??????8B??E8????????8B??85??0F85????????0F

	$ ==>    >  E8 A05D5A00        CALL CryGame.0D626550
	$+5      >  8BD8               MOV EBX,EAX
	$+7      >  85DB               TEST EBX,EBX
	$+9      >  0F85 B3000000      JNZ CryGame.0D08086D
	$+F      >  0FB756 6D          MOVZX EDX,WORD PTR DS:[ESI+0x6D]
	$+13     >  8985 70FFFFFF      MOV DWORD PTR SS:[EBP-0x90],EAX
	$+19     >  8D85 70FFFFFF      LEA EAX,DWORD PTR SS:[EBP-0x90]
	$+1F     >  50                 PUSH EAX
	$+20     >  8D4E 6F            LEA ECX,DWORD PTR DS:[ESI+0x6F]
	$+23     >  51                 PUSH ECX
	$+24     >  52                 PUSH EDX
	$+25     >  E8 5B765A00        CALL CryGame.0D627E30

	E8????????8B??85??0F??????????0F??????89
	--*/
	DWORD	dwMMOClientReqItemByGetItemIdCall = (DWORD)scan_game_.ScanCall("E8????????8B??85??0F??????????0F??????89", 0x26);
	assert(dwMMOClientReqItemByGetItemIdCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_CALL  " << dwMMOClientReqItemByGetItemIdCall - GetModuleCryGameBase() << "  // " << dwMMOClientReqItemByGetItemIdCall << "  CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientReqItemByGetItemIdOffset1 = *scan_game_.ScanImme<byte>("E8????????8B??85??0F??????????0F??????89", 0x12);
	assert(dwMMOClientReqItemByGetItemIdOffset1);
	assert(dwMMOClientReqItemByGetItemIdOffset1 == 0x0000006D);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET1  " << dwMMOClientReqItemByGetItemIdOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientReqItemByGetItemIdOffset2 = *scan_game_.ScanImme<byte>("E8????????8B??85??0F??????????0F??????89", 0x22);
	assert(dwMMOClientReqItemByGetItemIdOffset2);
	assert(dwMMOClientReqItemByGetItemIdOffset2 == 0x0000006F);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET2  " << dwMMOClientReqItemByGetItemIdOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0>  894E 1C         MOV DWORD PTR DS:[ESI+0x1C],ECX
	$+3      0>  8B4F 04         MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+6      0>  52              PUSH EDX
	$+7      0>  50              PUSH EAX
	$+8      0>  81C1 28040000   ADD ECX,0x428                            ; +0x428
	$+E      0>  E8 6D25FEFF     CALL CryGame.0D554630

	89????8B????????81C1????????E8
	--*/
	DWORD	dwMMOClientRoleExpOffset1 = *scan_game_.ScanImme<DWORD>("89????8B????????81C1????????E8", 0xA);
	assert(dwMMOClientRoleExpOffset1);
	//assert(dwMMOClientRoleExpOffset1==0x00000430/*0x00000428*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_EXP_OFFSET1  " << dwMMOClientRoleExpOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D52303E      6A 02           PUSH 0x2
	$+2      0D523040      8D87 8A030000   LEA EAX,DWORD PTR DS:[EDI+0x38A]
	$+8      0D523046      50              PUSH EAX
	$+9      0D523047      8D8E 86090000   LEA ECX,DWORD PTR DS:[ESI+0x986]
	$+F      0D52304D      51              PUSH ECX
	$+10     0D52304E      E8 DD77FFFF     CALL CryGame.0D51A830
	$+15     0D523053      83C4 18         ADD ESP,0x18
	$+18     0D523056      8D97 A2030000   LEA EDX,DWORD PTR DS:[EDI+0x3A2]
	$+1E     0D52305C      52              PUSH EDX
	$+1F     0D52305D      8D8E B6090000   LEA ECX,DWORD PTR DS:[ESI+0x9B6]         ; 流通金币
	$+25     0D523063      E8 7851FFFE     CALL CryGame.0C5181E0
	$+2A     0D523068      8D87 A6030000   LEA EAX,DWORD PTR DS:[EDI+0x3A6]
	$+30     0D52306E      50              PUSH EAX
	$+31     0D52306F      8D8E BE090000   LEA ECX,DWORD PTR DS:[ESI+0x9BE]         ; 绑定金币
	$+37     0D523075      E8 6651FFFE     CALL CryGame.0C5181E0
	$+3C     0D52307A      8D8F AA030000   LEA ECX,DWORD PTR DS:[EDI+0x3AA]
	$+42     0D523080      51              PUSH ECX
	$+43     0D523081      8D8E C6090000   LEA ECX,DWORD PTR DS:[ESI+0x9C6]         ; 点劵
	$+49     0D523087      E8 5451FFFE     CALL CryGame.0C5181E0
	$+4E     0D52308C      8D97 AE030000   LEA EDX,DWORD PTR DS:[EDI+0x3AE]
	$+54     0D523092      52              PUSH EDX
	$+55     0D523093      8D8E CE090000   LEA ECX,DWORD PTR DS:[ESI+0x9CE]
	$+5B     0D523099      E8 4251FFFE     CALL CryGame.0C5181E0
	$+60     0D52309E      8D87 B2030000   LEA EAX,DWORD PTR DS:[EDI+0x3B2]
	$+66     0D5230A4      50              PUSH EAX
	$+67     0D5230A5      8D8E 060A0000   LEA ECX,DWORD PTR DS:[ESI+0xA06]         ; 猎守次数
	...
	$+5D0    0D52369E           8D8F 8A060000         LEA ECX,DWORD PTR DS:[EDI+0x68A]
	$+5D6    0D5236A4           51                    PUSH ECX
	$+5D7    0D5236A5           8D8E FA140000         LEA ECX,DWORD PTR DS:[ESI+0x14FA]        ; 过图时读取庄园精算点，和金币，经验等一起
	$+5DD    0D5236AB           E8 304BFFFE           CALL CryGame.0C5181E0



	6A??8D????????????8D????????????E8????????83??188D????????????8D??????????E8????????8D????????????8D
	--*/
	DWORD	dwMMOClientMemoryBangDingOffset = *scan_game_.ScanImme<DWORD>("6A??8D????????????8D????????????E8????????83??188D????????????8D??????????E8????????8D????????????8D", 0x33);
	assert(dwMMOClientMemoryBangDingOffset);
	assert(dwMMOClientMemoryBangDingOffset == 0x9BE);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_BANG_DING_OFFSET  " << dwMMOClientMemoryBangDingOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientMemoryLiuTongOffset = *scan_game_.ScanImme<DWORD>("6A??8D????????????8D????????????E8????????83??188D????????????8D??????????E8????????8D????????????8D", 0x21);
	assert(dwMMOClientMemoryLiuTongOffset);
	assert(dwMMOClientMemoryLiuTongOffset == 0x9B6);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_LIU_TONG_OFFSET  " << dwMMOClientMemoryLiuTongOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	// 暂时号里没有，所以没有办法确定
	// 	DWORD	dwMMOClientMemoryLieBiOffset = 0; // ScanImme((HANDLE)-1,"FF??8B??33??3B??74??8D??????????89??????89??????89??????89",0x45);
	//  	ASSERT_GWLR(dwMMOClientMemoryLieBiOffset);
	//  	ASSERT_GWLR(dwMMOClientMemoryLieBiOffset==0x00000DF6);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_LIE_BI_OFFSET  ";
	// 	LOG << dwMMOClientMemoryLieBiOffset;
	// 	LOG_END;

	DWORD	dwMMOClientLieShouXuKeZhengCountOffset1 = *scan_game_.ScanImme<DWORD>("6A??8D????????????8D????????????E8????????83??188D????????????8D??????????E8????????8D????????????8D", 0x69);
	assert(dwMMOClientLieShouXuKeZhengCountOffset1);
	assert(dwMMOClientLieShouXuKeZhengCountOffset1 == 0xA06);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_LIE_SHOU_XU_KE_ZHENG_COUNT_OFFSET1  " << dwMMOClientLieShouXuKeZhengCountOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientRoleMemoryJingSuanDianOffset = *scan_game_.ScanImme<DWORD>("6A??8D????????????8D????????????E8????????83??188D????????????8D??????????E8????????8D????????????8D", 0x5D9);
	assert(dwMMOClientRoleMemoryJingSuanDianOffset);
	assert(dwMMOClientRoleMemoryJingSuanDianOffset == 0x14FA);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_JING_SUAN_DIAN_OFFSET  " << dwMMOClientRoleMemoryJingSuanDianOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    01177C35     C1E2 0C                 SHL EDX,0xC
	$+3      01177C38     C1FA 0C                 SAR EDX,0xC
	$+6      01177C3B     895408 3C               MOV DWORD PTR DS:[EAX+ECX+0x3C],EDX
	$+A      01177C3F     8B55 DC                 MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+D      01177C42     8B0A                    MOV ECX,DWORD PTR DS:[EDX]
	$+F      01177C44     8B91 C8000000           MOV EDX,DWORD PTR DS:[ECX+0xC8]
	$+15     01177C4A     8B4C3A 40               MOV ECX,DWORD PTR DS:[EDX+EDI+0x40]                 ; 从某内存取攻击部位id
	$+19     01177C4E     8B55 FC                 MOV EDX,DWORD PTR SS:[EBP-0x4]
	$+1C     01177C51     894C10 40               MOV DWORD PTR DS:[EAX+EDX+0x40],ECX                 ; 将计算的攻击部位结果写入到某对象中，然后拷贝给攻击参数
	$+20     01177C55     8B4D C8                 MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+23     01177C58     8B55 FC                 MOV EDX,DWORD PTR SS:[EBP-0x4]
	$+26     01177C5B     8B4410 38               MOV EAX,DWORD PTR DS:[EAX+EDX+0x38]
	$+2A     01177C5F     51                      PUSH ECX

	C1??0CC1??0C89??????8B????8B??8B??????????8B
	C1????C1????89??????8B????8B??8B??????????8B
	--*/
	DWORD	dwMMOClientHitBuWeiHook = (DWORD)scan_exe_.ScanAddr("C1????C1????89??????8B????8B??8B??????????8B", 0x0F);
	assert(dwMMOClientHitBuWeiHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_BU_WEI_HOOK  " << dwMMOClientHitBuWeiHook << "  // MHOClient.exe" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0095153F        8BF9                     MOV EDI,ECX
	$+2      00951541        F747 04 00001000         TEST DWORD PTR DS:[EDI+0x4],0x100000
	$+9      00951548        74 09                    JE SHORT MHOClien.00951553
	$+B      0095154A        F6C3 20                  TEST BL,0x20
	$+E      0095154D        0F85 93020000            JNZ MHOClien.009517E6
	$+14     00951553        8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+17     00951556        D905 90C9E301            FLD DWORD PTR DS:[0x1E3C990]
	$+1D     0095155C        51                       PUSH ECX
	$+1E     0095155D        8BCE                     MOV ECX,ESI
	$+20     0095155F        D91C24                   FSTP DWORD PTR SS:[ESP]
	$+23     00951562        E8 19B6FFFF              CALL MHOClien.0094CB80
	$+28     00951567        85C0                     TEST EAX,EAX
	$+2A     00951569        0F84 51020000            JE MHOClien.009517C0
	$+30     0095156F        F3:0F103D B03A0802       MOVSS XMM7,DWORD PTR DS:[0x2083AB0]      ; ADDR1
	$+38     00951577        F3:0F1076 28             MOVSS XMM6,DWORD PTR DS:[ESI+0x28]
	$+3D     0095157C        F3:0F106E 14             MOVSS XMM5,DWORD PTR DS:[ESI+0x14]
	$+42     00951581        F3:0F1026                MOVSS XMM4,DWORD PTR DS:[ESI]
	$+46     00951585        F3:0F105E 08             MOVSS XMM3,DWORD PTR DS:[ESI+0x8]
	$+4B     0095158A        F3:0F1046 10             MOVSS XMM0,DWORD PTR DS:[ESI+0x10]
	$+50     0095158F        F3:0F1056 20             MOVSS XMM2,DWORD PTR DS:[ESI+0x20]
	$+55     00951594        F3:0F104E 24             MOVSS XMM1,DWORD PTR DS:[ESI+0x24]
	$+5A     00951599        F3:0F117424 6C           MOVSS DWORD PTR SS:[ESP+0x6C],XMM6
	$+60     0095159F        F3:0F117C24 1C           MOVSS DWORD PTR SS:[ESP+0x1C],XMM7
	$+66     009515A5        F3:0F117C24 20           MOVSS DWORD PTR SS:[ESP+0x20],XMM7
	$+6C     009515AB        F3:0F117C24 24           MOVSS DWORD PTR SS:[ESP+0x24],XMM7
	$+72     009515B1        0F57FF                   XORPS XMM7,XMM7
	$+75     009515B4        F3:0F58F5                ADDSS XMM6,XMM5
	$+79     009515B8        F3:0F58F4                ADDSS XMM6,XMM4
	$+7D     009515BC        0F2FF7                   COMISS XMM6,XMM7
	$+80     009515BF        76 66                    JBE SHORT MHOClien.00951627
	$+82     009515C1        F3:0F5835 B03A0802       ADDSS XMM6,DWORD PTR DS:[0x2083AB0]
	$+8A     009515C9        0F57E4                   XORPS XMM4,XMM4
	$+8D     009515CC        F3:0F10E6                MOVSS XMM4,XMM6
	$+91     009515D0        F3:0F51E4                SQRTSS XMM4,XMM4
	$+95     009515D4        0F28EC                   MOVAPS XMM5,XMM4
	$+98     009515D7        F3:0F1025 403B0802       MOVSS XMM4,DWORD PTR DS:[0x2083B40]      ; ADDR2
	$+A0     009515DF        F3:0F5EE5                DIVSS XMM4,XMM5
	$+A4     009515E3        F3:0F116C24 18           MOVSS DWORD PTR SS:[ESP+0x18],XMM5

	8B??F7????????????74??F6????0F??????????8B????D9

	$ ==>    0094E48C       F3:0F103D D458E201      MOVSS XMM7,DWORD PTR DS:[0x1E258D4]      ; ADDR1
	$+8      0094E494       F3:0F1076 28            MOVSS XMM6,DWORD PTR DS:[ESI+0x28]
	$+D      0094E499       F3:0F106E 14            MOVSS XMM5,DWORD PTR DS:[ESI+0x14]
	$+12     0094E49E       F3:0F1026               MOVSS XMM4,DWORD PTR DS:[ESI]
	$+16     0094E4A2       F3:0F105E 08            MOVSS XMM3,DWORD PTR DS:[ESI+0x8]
	$+1B     0094E4A7       F3:0F1046 10            MOVSS XMM0,DWORD PTR DS:[ESI+0x10]
	$+20     0094E4AC       F3:0F1056 20            MOVSS XMM2,DWORD PTR DS:[ESI+0x20]
	$+25     0094E4B1       F3:0F104E 24            MOVSS XMM1,DWORD PTR DS:[ESI+0x24]
	$+2A     0094E4B6       F3:0F1175 CC            MOVSS DWORD PTR SS:[EBP-0x34],XMM6
	$+2F     0094E4BB       F3:0F117D F0            MOVSS DWORD PTR SS:[EBP-0x10],XMM7
	$+34     0094E4C0       F3:0F117D F4            MOVSS DWORD PTR SS:[EBP-0xC],XMM7
	$+39     0094E4C5       F3:0F117D F8            MOVSS DWORD PTR SS:[EBP-0x8],XMM7
	$+3E     0094E4CA       0F57FF                  XORPS XMM7,XMM7
	$+41     0094E4CD       F3:0F58F5               ADDSS XMM6,XMM5
	$+45     0094E4D1       F3:0F58F4               ADDSS XMM6,XMM4
	$+49     0094E4D5       0F2FF7                  COMISS XMM6,XMM7
	$+4C     0094E4D8       76 64                   JBE SHORT MHOClien.0094E53E
	$+4E     0094E4DA       F3:0F5835 D458E201      ADDSS XMM6,DWORD PTR DS:[0x1E258D4]
	$+56     0094E4E2       0F57E4                  XORPS XMM4,XMM4
	$+59     0094E4E5       F3:0F10E6               MOVSS XMM4,XMM6
	$+5D     0094E4E9       F3:0F51E4               SQRTSS XMM4,XMM4
	$+61     0094E4ED       0F28EC                  MOVAPS XMM5,XMM4
	$+64     0094E4F0       F3:0F1025 1492DC01      MOVSS XMM4,DWORD PTR DS:[0x1DC9214]      ; ADDR2
	$+6C     0094E4F8       F3:0F5EE5               DIVSS XMM4,XMM5
	$+70     0094E4FC       F3:0F116D FC            MOVSS DWORD PTR SS:[EBP-0x4],XMM5
	$+75     0094E501       F3:0F106E 18            MOVSS XMM5,DWORD PTR DS:[ESI+0x18]

	F30F103D????????F30F??????F30F??????F30F????F30F??????F30F??????F30F??????F30F

	$ ==>    0095D4BF          F3:0F103D 98E10502    MOVSS XMM7,DWORD PTR DS:[0x205E198]	; ADDR1
	$+8      0095D4C7          F3:0F1076 28          MOVSS XMM6,DWORD PTR DS:[ESI+0x28]
	$+D      0095D4CC          F3:0F106E 14          MOVSS XMM5,DWORD PTR DS:[ESI+0x14]
	$+12     0095D4D1          F3:0F1026             MOVSS XMM4,DWORD PTR DS:[ESI]
	$+16     0095D4D5          F3:0F105E 08          MOVSS XMM3,DWORD PTR DS:[ESI+0x8]
	$+1B     0095D4DA          F3:0F1046 10          MOVSS XMM0,DWORD PTR DS:[ESI+0x10]
	$+20     0095D4DF          F3:0F1056 20          MOVSS XMM2,DWORD PTR DS:[ESI+0x20]
	$+25     0095D4E4          F3:0F104E 24          MOVSS XMM1,DWORD PTR DS:[ESI+0x24]
	$+2A     0095D4E9          F3:0F117424 6C        MOVSS DWORD PTR SS:[ESP+0x6C],XMM6
	$+30     0095D4EF          F3:0F117C24 1C        MOVSS DWORD PTR SS:[ESP+0x1C],XMM7
	$+36     0095D4F5          F3:0F117C24 20        MOVSS DWORD PTR SS:[ESP+0x20],XMM7
	$+3C     0095D4FB          F3:0F117C24 24        MOVSS DWORD PTR SS:[ESP+0x24],XMM7
	$+42     0095D501          0F57FF                XORPS XMM7,XMM7
	$+45     0095D504          F3:0F58F5             ADDSS XMM6,XMM5
	$+49     0095D508          F3:0F58F4             ADDSS XMM6,XMM4
	$+4D     0095D50C          0F2FF7                COMISS XMM6,XMM7
	$+50     0095D50F          76 66                 JBE SHORT MHOClien.0095D577
	$+52     0095D511          F3:0F5835 98E10502    ADDSS XMM6,DWORD PTR DS:[0x205E198]
	$+5A     0095D519          0F57E4                XORPS XMM4,XMM4
	$+5D     0095D51C          F3:0F10E6             MOVSS XMM4,XMM6
	$+61     0095D520          F3:0F51E4             SQRTSS XMM4,XMM4
	$+65     0095D524          0F28EC                MOVAPS XMM5,XMM4
	$+68     0095D527          F3:0F1025 28E20502    MOVSS XMM4,DWORD PTR DS:[0x205E228]	; ADDR2
	$+70     0095D52F          F3:0F5EE5             DIVSS XMM4,XMM5
	$+74     0095D533          F3:0F116C24 18        MOVSS DWORD PTR SS:[ESP+0x18],XMM5
	$+7A     0095D539          F3:0F106E 18          MOVSS XMM5,DWORD PTR DS:[ESI+0x18]
	$+7F     0095D53E          F3:0F5CCD             SUBSS XMM1,XMM5
	$+83     0095D542          F3:0F59CC             MULSS XMM1,XMM4
	$+87     0095D546          0F28F1                MOVAPS XMM6,XMM1
	$+8A     0095D549          F3:0F104E 04          MOVSS XMM1,DWORD PTR DS:[ESI+0x4]

	F30F103D????????F30F??????F30F??????F30F????F30F??????F30F??????F30F??????F30F
	--*/
	DWORD	dwMMOClientHitDirCalcCallBaseAddr1 = *scan_exe_.ScanImme<DWORD>("F30F103D????????F30F??????F30F??????F30F????F30F??????F30F??????F30F??????F30F", 0x4);
	assert(dwMMOClientHitDirCalcCallBaseAddr1);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1  " << dwMMOClientHitDirCalcCallBaseAddr1 << "  // MHOClient.exe" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientHitDirCalcCallBaseAddr2 = *scan_exe_.ScanImme<DWORD>("F30F103D????????F30F??????F30F??????F30F????F30F??????F30F??????F30F??????F30F", 0x68);
	assert(dwMMOClientHitDirCalcCallBaseAddr2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2  " << dwMMOClientHitDirCalcCallBaseAddr2 << "  // MHOClient.exe" << FC_OFstream_EndlLog;


	/*++
	$-D      015681B0     55              PUSH EBP
	$-C      015681B1     8BEC            MOV EBP,ESP
	$-A      015681B3     D9EE            FLDZ
	$-8      015681B5     D891 8C000000   FCOM DWORD PTR DS:[ECX+0x8C]
	$-2      015681BB     DFE0            FSTSW AX
	$ ==>    015681BD     F6C4 41         TEST AH,0x41
	$+3      015681C0     7A 2B           JPE SHORT MHOClien.015681ED
	$+5      015681C2     D899 EC000000   FCOMP DWORD PTR DS:[ECX+0xEC]
	$+B      015681C8     DFE0            FSTSW AX
	$+D      015681CA     F6C4 41         TEST AH,0x41
	$+10     015681CD     75 20           JNZ SHORT MHOClien.015681EF
	$+12     015681CF     8079 54 00      CMP BYTE PTR DS:[ECX+0x54],0x0
	$+16     015681D3     75 1A           JNZ SHORT MHOClien.015681EF
	$+18     015681D5     8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1B     015681D8     8138 2C010000   CMP DWORD PTR DS:[EAX],0x12C
	$+21     015681DE     75 0F           JNZ SHORT MHOClien.015681EF
	$+23     015681E0     8B40 08         MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+26     015681E3     50              PUSH EAX
	$+27     015681E4     E8 BD8CF4FF     CALL MHOClien.014B0EA6                   ; 调用vmp
	$+2C     015681E9     5D              POP EBP
	$+2D     015681EA     C2 0400         RETN 0x4
	$+30     015681ED     DDD8            FSTP ST(0)
	$+32     015681EF     5D              POP EBP
	$+33     015681F0     C2 0400         RETN 0x4

	F6????7A??D8??????????DF??F6????75??80??????75??8B
	--*/
	DWORD	dwMMOClientMonsterRemoteHitAddrHook = (DWORD)scan_game_.ScanAddr("F6????7A??D8??????????DF??F6????75??80??????75??8B", -0x0D);
	assert(dwMMOClientMonsterRemoteHitAddrHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK  " << dwMMOClientMonsterRemoteHitAddrHook - GetModuleCryGameBase() << "  // " << dwMMOClientMonsterRemoteHitAddrHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*

	$-1C     > /7D 20           jge short CryGame.11B23AF4
	$-1A     > |8B46 24         mov eax,dword ptr ds:[esi+0x24]
	$-17     > |85C0            test eax,eax
	$-15     > |74 09           je short CryGame.11B23AE4
	$-13     > |50              push eax
	$-12     > |52              push edx
	$-11     > |E8 7E02FFFF     call CryGame.11B13D60
	$-C      > |EB 10           jmp short CryGame.11B23AF4
	$-A      > |6A 00           push 0x0
	$-8      > |6A 00           push 0x0
	$-6      > |52              push edx
	$-5      > |E8 5202FFFF     call CryGame.11B13D40
	$ ==>    > |83F8 01         cmp eax,0x1
	$+3      > |0f94c0          sete al
	$+6      > \53              push ebx
	$+7      >  8AD8            mov bl,al
	$+9      >  84C0            test al,al
	$+B      >  75 06           jnz short CryGame.11B23B01
	$+D      >  5B              pop ebx
	$+E      >  5E              pop esi
	$+F      >  5D              pop ebp
	$+10     >  C2 0400         retn 0x4
	$+13     >  8B46 24         mov eax,dword ptr ds:[esi+0x24]
	$+16     >  85C0            test eax,eax
	$+18     >  75 0E           jnz short CryGame.11B23B16
	$+1A     >  8B4E 1C         mov ecx,dword ptr ds:[esi+0x1C]
	$+1D     >  8AC3            mov al,bl
	$+1F     >  5B              pop ebx
	$+20     >  894E 34         mov dword ptr ds:[esi+0x34],ecx
	$+23     >  5E              pop esi
	$+24     >  5D              pop ebp


	83 F8 01 0F 94 C0 53 8A D8 84 C0

	cwjvf18635

	*/
	DWORD	dwMMOKILLNIREN = (DWORD)scan_game_.ScanAddr("83F8010F94C0538AD884C0", 0xB);
	assert(dwMMOKILLNIREN);
	ofs << "#define UPDATES_REVERSAL_KILLNIREN_ADDR_HOOK  " << dwMMOKILLNIREN - GetModuleCryGameBase() << "  // " << dwMMOKILLNIREN << " CryGame.dll" << FC_OFstream_EndlLog;



	/*++
	$ ==>    00600A10     3950 10                      CMP DWORD PTR DS:[EAX+0x10],EDX
	$+3      00600A13     7C 07                        JL SHORT MHOClien.00600A1C
	$+5      00600A15     8BC8                         MOV ECX,EAX
	$+7      00600A17     8B40 08                      MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+A      00600A1A     EB 03                        JMP SHORT MHOClien.00600A1F
	$+C      00600A1C     8B40 0C                      MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+F      00600A1F     3BC3                         CMP EAX,EBX
	$+11     00600A21   ^ 75 ED                        JNZ SHORT MHOClien.00600A10
	$+13     00600A23     8D4424 20                    LEA EAX,DWORD PTR SS:[ESP+0x20]
	$+17     00600A27     3BC8                         CMP ECX,EAX
	$+19     00600A29     74 07                        JE SHORT MHOClien.00600A32
	$+1B     00600A2B     3B51 10                      CMP EDX,DWORD PTR DS:[ECX+0x10]
	$+1E     00600A2E     7D 02                        JGE SHORT MHOClien.00600A32
	$+20     00600A30     8BC8                         MOV ECX,EAX
	$+22     00600A32     8D5424 20                    LEA EDX,DWORD PTR SS:[ESP+0x20]
	$+26     00600A36     3BCA                         CMP ECX,EDX
	$+28     00600A38     53                           PUSH EBX
	$+29     00600A39     0F95C0                       SETNE AL
	$+2C     00600A3C     68 0C6FFD01                  PUSH MHOClien.01FD6F0C                                 ; ASCII "CManufactureInfo"
	$+31     00600A41     888424 84000000              MOV BYTE PTR SS:[ESP+0x84],AL
	$+38     00600A48     8B76 1C                      MOV ESI,DWORD PTR DS:[ESI+0x1C]
	$+3B     00600A4B     53                           PUSH EBX
	$+3C     00600A4C     E8 5FBDEFFF                  CALL MHOClien.004FC7B0	// call

	$ ==>    0D09C5F2      3950 10                    CMP DWORD PTR DS:[EAX+0x10],EDX
	$+3      0D09C5F5      7C 07                      JL SHORT CryGame.0D09C5FE
	$+5      0D09C5F7      8BC8                       MOV ECX,EAX
	$+7      0D09C5F9      8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+A      0D09C5FC      EB 03                      JMP SHORT CryGame.0D09C601
	$+C      0D09C5FE      8B40 0C                    MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+F      0D09C601      3BC3                       CMP EAX,EBX
	$+11     0D09C603    ^ 75 ED                      JNZ SHORT CryGame.0D09C5F2
	$+13     0D09C605      8D45 80                    LEA EAX,DWORD PTR SS:[EBP-0x80]
	$+16     0D09C608      3BC8                       CMP ECX,EAX
	$+18     0D09C60A      74 07                      JE SHORT CryGame.0D09C613
	$+1A     0D09C60C      3B51 10                    CMP EDX,DWORD PTR DS:[ECX+0x10]
	$+1D     0D09C60F      7D 02                      JGE SHORT CryGame.0D09C613
	$+1F     0D09C611      8BC8                       MOV ECX,EAX
	$+21     0D09C613      8D55 80                    LEA EDX,DWORD PTR SS:[EBP-0x80]
	$+24     0D09C616      3BCA                       CMP ECX,EDX
	$+26     0D09C618      53                         PUSH EBX
	$+27     0D09C619      0F95C0                     SETNE AL
	$+2A     0D09C61C      68 9481A80D                PUSH CryGame.0DA88194                                    ; ASCII "CManufactureInfo"
	$+2F     0D09C621      8845 D4                    MOV BYTE PTR SS:[EBP-0x2C],AL
	$+32     0D09C624      8B76 1C                    MOV ESI,DWORD PTR DS:[ESI+0x1C]
	$+35     0D09C627      53                         PUSH EBX
	$+36     0D09C628      E8 83217FFF                CALL CryGame.0C88E7B0

	39????7C??8B??8B????EB??8B????3B??75??8D

	// 不删档测试版本
	$ ==>    > /74 07           JE SHORT CryGame.0D0C1F43
	$+2      > |3B51 10         CMP EDX,DWORD PTR DS:[ECX+0x10]
	$+5      > |7D 02           JGE SHORT CryGame.0D0C1F43
	$+7      > |8BC8            MOV ECX,EAX
	$+9      > \8D55 80         LEA EDX,DWORD PTR SS:[EBP-0x80]
	$+C      >  3BCA            CMP ECX,EDX
	$+E      >  53              PUSH EBX
	$+F      >  0F95C0          SETNE AL
	$+12     >  68 1CE1AC0D     PUSH CryGame.0DACE11C                                  ; ASCII "CManufactureInfo"
	$+17     >  8845 D4         MOV BYTE PTR SS:[EBP-0x2C],AL
	$+1A     >  8B76 1C         MOV ESI,DWORD PTR DS:[ESI+0x1C]
	$+1D     >  53              PUSH EBX
	$+1E     >  E8 532C7EFF     CALL CryGame.0C8A4BB0
	$+23     >  83C4 0C         ADD ESP,0xC
	$+26     >  8BC8            MOV ECX,EAX
	$+28     >  83FE FF         CMP ESI,-0x1
	$+2B     >  0F84 B7000000   JE CryGame.0D0C2022

	74??3B????7D??8B??8D????3B????0F????68
	--*/
	DWORD	dwMMOClientGetManuFactureInfoAdrrCall = (DWORD)scan_game_.ScanCall("74??3B????7D??8B??8D????3B????0F????68", 0x1F);
	assert(dwMMOClientGetManuFactureInfoAdrrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_MANU_FACTURE_INFO_ADDR_CALL  " << dwMMOClientGetManuFactureInfoAdrrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetManuFactureInfoAdrrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-11     006A0B7D     8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-E      006A0B80     50              PUSH EAX
	$-D      006A0B81     6A 00           PUSH 0x0
	$-B      006A0B83     68 E8EDFE01     PUSH MHOClien.01FEEDE8                   ; ASCII "CDialogInfo"
	$-6      006A0B88     6A 00           PUSH 0x0
	$-4      006A0B8A     C645 FF 01      MOV BYTE PTR SS:[EBP-0x1],0x1
	$ ==>    006A0B8E     E8 8D090000     CALL MHOClien.006A1520                   ; 地址
	$+5      006A0B93     83C4 0C         ADD ESP,0xC
	$+8      006A0B96     8BC8            MOV ECX,EAX
	$+A      006A0B98     E8 F3B3FBFF     CALL MHOClien.0065BF90
	$+F      006A0B9D     8BF0            MOV ESI,EAX
	$+11     006A0B9F     8975 F8         MOV DWORD PTR SS:[EBP-0x8],ESI
	$+14     006A0BA2     85F6            TEST ESI,ESI
	$+16     006A0BA4   ^ 74 CE           JE SHORT MHOClien.006A0B74
	$+18     006A0BA6     8B4E 54         MOV ECX,DWORD PTR DS:[ESI+0x54]          ; hook 取esi
	$+1B     006A0BA9     8D46 54         LEA EAX,DWORD PTR DS:[ESI+0x54]
	$+1E     006A0BAC     33D2            XOR EDX,EDX
	$+20     006A0BAE     C745 F4 0000000>MOV DWORD PTR SS:[EBP-0xC],0x0
	$+27     006A0BB5     3BC8            CMP ECX,EAX
	$+29     006A0BB7     74 12           JE SHORT MHOClien.006A0BCB
	$+2B     006A0BB9     8DA424 00000000 LEA ESP,DWORD PTR SS:[ESP]
	$+32     006A0BC0     8B09            MOV ECX,DWORD PTR DS:[ECX]
	$+34     006A0BC2     42              INC EDX
	$+35     006A0BC3     3BC8            CMP ECX,EAX
	$+37     006A0BC5   ^ 75 F9           JNZ SHORT MHOClien.006A0BC0

	E8????????83????8B??E8????????8b??89????85??74??8B????8D????33

	$-1C     006A4B5D          8B4424 28             MOV EAX,DWORD PTR SS:[ESP+0x28]
	$-18     006A4B61          50                    PUSH EAX                                 ; "CDialogInfo"		// 第2个，第一个是 CMessageInfo
	$-17     006A4B62          C64424 17 01          MOV BYTE PTR SS:[ESP+0x17],0x1
	$-12     006A4B67          E8 C4FDFFFF           CALL MHOClien.006A4930
	$-D      006A4B6C          8BF0                  MOV ESI,EAX
	$-B      006A4B6E          83C4 04               ADD ESP,0x4
	$-8      006A4B71          897424 14             MOV DWORD PTR SS:[ESP+0x14],ESI
	$-4      006A4B75          85F6                  TEST ESI,ESI
	$-2      006A4B77        ^ 74 DA                 JE SHORT MHOClien.006A4B53
	$ ==>    006A4B79          8B4E 54               MOV ECX,DWORD PTR DS:[ESI+0x54]
	$+3      006A4B7C          8D46 54               LEA EAX,DWORD PTR DS:[ESI+0x54]
	$+6      006A4B7F          33DB                  XOR EBX,EBX
	$+8      006A4B81          33D2                  XOR EDX,EDX
	$+A      006A4B83          3BC8                  CMP ECX,EAX
	$+C      006A4B85          74 0B                 JE SHORT MHOClien.006A4B92
	$+E      006A4B87          8B09                  MOV ECX,DWORD PTR DS:[ECX]
	$+10     006A4B89          42                    INC EDX
	$+11     006A4B8A          3BC8                  CMP ECX,EAX
	$+13     006A4B8C        ^ 75 F9                 JNZ SHORT MHOClien.006A4B87
	$+15     006A4B8E          85D2                  TEST EDX,EDX
	$+17     006A4B90          75 29                 JNZ SHORT MHOClien.006A4BBB
	$+19     006A4B92          8B46 20               MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+1C     006A4B95          85C0                  TEST EAX,EAX
	$+1E     006A4B97          75 05                 JNZ SHORT MHOClien.006A4B9E
	$+20     006A4B99          B8 96820302           MOV EAX,MHOClien.02038296
	$+25     006A4B9E          50                    PUSH EAX
	$+26     006A4B9F          68 00100000           PUSH 0x1000
	$+2B     006A4BA4          68 D0D15502           PUSH MHOClien.0255D1D0
	$+30     006A4BA9          E8 663B5201           CALL MHOClien.01BC8714                   ; JMP 到 msvcr90.sprintf_s

	8B????8D????33??33??3B??74??8B????3B??75??85??75
	--*/
	DWORD	dwMMOClientGetDialogInfoAddrCall = (DWORD)scan_game_.ScanCall("8B????8D????33??33??3B??74??8B????3B??75??85??75", -0x11, 2);
	if (dwMMOClientGetDialogInfoAddrCall == 0)
	{
		dwMMOClientGetDialogInfoAddrCall = (DWORD)scan_game_.ScanCall("E8????????83????8B??E8????????8b??89????85??74??8B????8D????33", 1, 2);
	}
	assert(dwMMOClientGetDialogInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_DIALOG_INFO_BASE_ADDR_CALL  " << dwMMOClientGetDialogInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetDialogInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientDialogHook = (DWORD)scan_game_.ScanAddr("8B????8D????33??33??3B??74??8B????3B??75??85??75", 0, 2);
	if (dwMMOClientDialogHook == 0)
	{
		dwMMOClientDialogHook = (DWORD)scan_game_.ScanAddr("E8????????83????8B??E8????????8b??89????85??74??8B????8D????33", 0x18, 2);
	}
	assert(dwMMOClientDialogHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_DIALOG_HOOK  " << dwMMOClientDialogHook - GetModuleCryGameBase() << "  // " << dwMMOClientDialogHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    01BB0964      8D0440                LEA EAX,DWORD PTR DS:[EAX+EAX*2]
	$+3      01BB0967      80BC87 84010000 00    CMP BYTE PTR DS:[EDI+EAX*4+0x184],0x0
	$+B      01BB096F      8DB487 84010000       LEA ESI,DWORD PTR DS:[EDI+EAX*4+0x184]
	$+12     01BB0976      0F84 B7000000         JE MHOClien.01BB0A33
	$+18     01BB097C      8B46 04               MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+1B     01BB097F      85C0                  TEST EAX,EAX
	$+1D     01BB0981      0F84 AC000000         JE MHOClien.01BB0A33
	$+23     01BB0987      50                    PUSH EAX
	$+24     01BB0988      E8 73099BFE           CALL MHOClien.00561300                   ; 获取农场种子信息
	{
	$ ==>    00561300      55                    PUSH EBP
	$+1      00561301      8BEC                  MOV EBP,ESP
	$+3      00561303      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+6      00561306      50                    PUSH EAX
	$+7      00561307      6A 00                 PUSH 0x0
	$+9      00561309      68 18E9FD01           PUSH MHOClien.01FDE918                   ; ASCII "CFarmSeed"
	$+E      0056130E      6A 00                 PUSH 0x0
	$+10     00561310      E8 3B4A0000           CALL MHOClien.00565D50
	$+15     00561315      83C4 0C               ADD ESP,0xC
	$+18     00561318      8BC8                  MOV ECX,EAX
	$+1A     0056131A      E8 71AC0F00           CALL MHOClien.0065BF90
	$+1F     0056131F      5D                    POP EBP
	$+20     00561320      C3                    RETN
	}
	$+29     01BB098D      83C4 04               ADD ESP,0x4
	$+2C     01BB0990      85C0                  TEST EAX,EAX
	$+2E     01BB0992      0F84 9B000000         JE MHOClien.01BB0A33

	8D????80??????????????8D????????????0F84
	--*/
	DWORD	dwMMOClientFramSeedCall = (DWORD)scan_game_.ScanCall("8D????80??????????????8D????????????0F84", 0x25);
	assert(dwMMOClientFramSeedCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_SEED_CALL  " << dwMMOClientFramSeedCall - GetModuleCryGameBase() << "  // " << dwMMOClientFramSeedCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00562CC1      E8 FAE5FFFF           CALL MHOClien.005612C0                   ; 获取模板采集点对象地址
	{
	$ ==>    005612C0      55                    PUSH EBP
	$+1      005612C1      8BEC                  MOV EBP,ESP
	$+3      005612C3      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+6      005612C6      50                    PUSH EAX
	$+7      005612C7      6A 00                 PUSH 0x0
	$+9      005612C9      68 04E9FD01           PUSH MHOClien.01FDE904                   ; ASCII "CFarmFacilityInfo"
	$+E      005612CE      6A 00                 PUSH 0x0
	$+10     005612D0      E8 CB490000           CALL MHOClien.00565CA0
	$+15     005612D5      83C4 0C               ADD ESP,0xC
	$+18     005612D8      8BC8                  MOV ECX,EAX
	$+1A     005612DA      E8 B1AC0F00           CALL MHOClien.0065BF90
	$+1F     005612DF      5D                    POP EBP
	$+20     005612E0      C3                    RETN
	}
	$+5      00562CC6      8BF8                  MOV EDI,EAX                              ; 返回农场采集点对象
	$+7      00562CC8      83C4 04               ADD ESP,0x4
	$+A      00562CCB      3BFE                  CMP EDI,ESI
	$+C      00562CCD      75 0E                 JNZ SHORT MHOClien.00562CDD
	$+E      00562CCF      8D4C24 28             LEA ECX,DWORD PTR SS:[ESP+0x28]
	$+12     00562CD3      E8 489AEFFF           CALL MHOClien.0045C720
	$+17     00562CD8      E9 A5080000           JMP MHOClien.00563582
	$+1C     00562CDD      8D57 50               LEA EDX,DWORD PTR DS:[EDI+0x50]
	$+1F     00562CE0      52                    PUSH EDX
	$+20     00562CE1      8D4C24 24             LEA ECX,DWORD PTR SS:[ESP+0x24]
	$+24     00562CE5      E8 4699EBFF           CALL MHOClien.0041C630

	E8????????8B??83????3B??75??8D??????E8

	$ ==>    0C937DA4      E8 D7E1FFFF                CALL CryGame.0C935F80
	$+5      0C937DA9      8BD8                       MOV EBX,EAX
	$+7      0C937DAB      83C4 04                    ADD ESP,0x4
	$+A      0C937DAE      85DB                       TEST EBX,EBX
	$+C      0C937DB0      75 0E                      JNZ SHORT CryGame.0C937DC0
	$+E      0C937DB2      8D4C24 20                  LEA ECX,DWORD PTR SS:[ESP+0x20]
	$+12     0C937DB6      E8 7572D3FF                CALL CryGame.0C66F030
	$+17     0C937DBB      E9 03090000                JMP CryGame.0C9386C3
	$+1C     0C937DC0      8D53 50                    LEA EDX,DWORD PTR DS:[EBX+0x50]
	$+1F     0C937DC3      52                         PUSH EDX
	$+20     0C937DC4      8D4C24 5C                  LEA ECX,DWORD PTR SS:[ESP+0x5C]
	$+24     0C937DC8      E8 43CAA3FF                CALL CryGame.0C374810
	$+29     0C937DCD      8D4424 40                  LEA EAX,DWORD PTR SS:[ESP+0x40]
	$+2D     0C937DD1      50                         PUSH EAX
	$+2E     0C937DD2      8D4C24 5C                  LEA ECX,DWORD PTR SS:[ESP+0x5C]
	$+32     0C937DD6      E8 65CFA3FF                CALL CryGame.0C374D40
	$+37     0C937DDB      8B40 14                    MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+3A     0C937DDE      8D4C24 20                  LEA ECX,DWORD PTR SS:[ESP+0x20]
	$+3E     0C937DE2      51                         PUSH ECX
	$+3F     0C937DE3      BE 84CAA70D                MOV ESI,CryGame.0DA7CA84                                 ; ASCII "name"

	E8????????8B??83????????75??8D??????E8????????E9

	--*/
	DWORD	dwMMOClientFramFacilityInfoCall = (DWORD)scan_game_.ScanCall("E8????????8B??83????????75??8D??????E8????????E9", 1);
	assert(dwMMOClientFramFacilityInfoCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_CALL  " << dwMMOClientFramFacilityInfoCall - GetModuleCryGameBase() << "  // " << dwMMOClientFramFacilityInfoCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00562ED2      E8 A9E3FFFF           CALL MHOClien.00561280                   ; "CFarmCPInfo" 获取植物需要成熟时间的对象
	{
	$ ==>    00561280      55                    PUSH EBP
	$+1      00561281      8BEC                  MOV EBP,ESP
	$+3      00561283      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+6      00561286      50                    PUSH EAX
	$+7      00561287      6A 00                 PUSH 0x0
	$+9      00561289      68 F8E8FD01           PUSH MHOClien.01FDE8F8                   ; ASCII "CFarmCPInfo"
	$+E      0056128E      6A 00                 PUSH 0x0
	$+10     00561290      E8 5B490000           CALL MHOClien.00565BF0
	$+15     00561295      83C4 0C               ADD ESP,0xC
	$+18     00561298      8BC8                  MOV ECX,EAX
	$+1A     0056129A      E8 F1AC0F00           CALL MHOClien.0065BF90
	$+1F     0056129F      5D                    POP EBP
	$+20     005612A0      C3                    RETN
	}
	$+5      00562ED7      83C4 04               ADD ESP,0x4
	$+8      00562EDA      894424 10             MOV DWORD PTR SS:[ESP+0x10],EAX          ; 种植成熟需要的时间对象
	$+C      00562EDE      56                    PUSH ESI
	$+D      00562EDF      3BC6                  CMP EAX,ESI
	$+F      00562EE1      74 52                 JE SHORT MHOClien.00562F35
	$+11     00562EE3      E8 4E9A5F01           CALL MHOClien.01B5C936                   ; JMP 到 msvcr90._time64
	$+16     00562EE8      8B5424 14             MOV EDX,DWORD PTR SS:[ESP+0x14]
	$+1A     00562EEC      8B4A 1C               MOV ECX,DWORD PTR DS:[EDX+0x1C]          ; 取对象成熟时间
	$+1D     00562EEF      8BD1                  MOV EDX,ECX
	$+1F     00562EF1      C1E2 04               SHL EDX,0x4
	$+22     00562EF4      2BD1                  SUB EDX,ECX                              ; 计算得到需要的成熟时间，以秒为单位
	$+24     00562EF6      8B4C24 18             MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+28     00562EFA      8B49 04               MOV ECX,DWORD PTR DS:[ECX+0x4]           ; 取采集点时间信息
	$+2B     00562EFD      8D0C91                LEA ECX,DWORD PTR DS:[ECX+EDX*4]         ; ecx 为种植时间 + 需要等待时间
	$+2E     00562F00      2BC8                  SUB ECX,EAX                              ; 成熟时间-当前时间
	$+30     00562F02      83C4 04               ADD ESP,0x4                              ; ecx = 剩余时间秒

	E8????????83????89????????3B??74??E8
	--*/
	DWORD	dwMMOClientFarmCpInfoCall = (DWORD)scan_game_.ScanCall("E8????????83????89????????3B??74??E8", 1);
	assert(dwMMOClientFarmCpInfoCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_CP_INFO_CALL  " << dwMMOClientFarmCpInfoCall - GetModuleCryGameBase() << "  // " << dwMMOClientFarmCpInfoCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-3      01BAFF60      55                    PUSH EBP
	$-2      01BAFF61      8BEC                  MOV EBP,ESP
	$ ==>    01BAFF63      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+3      01BAFF66      85C0                  TEST EAX,EAX
	$+5      01BAFF68      75 11                 JNZ SHORT MHOClien.01BAFF7B
	$+7      01BAFF6A      8B45 0C               MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+A      01BAFF6D      83F8 05               CMP EAX,0x5
	$+D      01BAFF70      77 36                 JA SHORT MHOClien.01BAFFA8
	$+F      01BAFF72      40                    INC EAX
	$+10     01BAFF73      6BC0 64               IMUL EAX,EAX,0x64
	$+13     01BAFF76      0345 10               ADD EAX,DWORD PTR SS:[EBP+0x10]
	$+16     01BAFF79      5D                    POP EBP
	$+17     01BAFF7A      C3                    RETN

	8B????85??75??8B????83????77????6B????03

	$ ==>    01C1CE40          8B4424 04             MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+4      01C1CE44          85C0                  TEST EAX,EAX
	$+6      01C1CE46          75 12                 JNZ SHORT MHOClien.01C1CE5A
	$+8      01C1CE48          8B4424 08             MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+C      01C1CE4C          83F8 05               CMP EAX,0x5
	$+F      01C1CE4F          77 37                 JA SHORT MHOClien.01C1CE88
	$+11     01C1CE51          40                    INC EAX
	$+12     01C1CE52          6BC0 64               IMUL EAX,EAX,0x64
	$+15     01C1CE55          034424 0C             ADD EAX,DWORD PTR SS:[ESP+0xC]
	$+19     01C1CE59          C3                    RETN

	8B??????85??75??8B??????83????77????6B????03
	--*/
	DWORD	dwMMOClientGetFarmFacilityIdCall = (DWORD)scan_game_.ScanAddr("8B??????85??75??8B??????83????77????6B????03", 0);
	if (dwMMOClientGetFarmFacilityIdCall == 0)
	{
		dwMMOClientGetFarmFacilityIdCall = (DWORD)scan_game_.ScanAddr("8B????85??75??8B????83????77????6B????03", -0x3);
	}
	assert(dwMMOClientGetFarmFacilityIdCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_FARM_FACILITY_ID_CALL  " << dwMMOClientGetFarmFacilityIdCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetFarmFacilityIdCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    00561F64      8B82 A8000000         MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+6      00561F6A      FFD0                  CALL EAX
	$+8      00561F6C      3BC6                  CMP EAX,ESI
	$+A      00561F6E      0F84 BF070000         JE MHOClien.00562733
	$+10     00561F74      8B4C24 0C             MOV ECX,DWORD PTR SS:[ESP+0xC]
	$+14     00561F78      8D98 203F0000         LEA EBX,DWORD PTR DS:[EAX+0x3F20]        ; 草药田偏移
	$+1A     00561F7E      51                    PUSH ECX
	$+1B     00561F7F      8D4C24 74             LEA ECX,DWORD PTR SS:[ESP+0x74]
	$+1F     00561F83      895C24 70             MOV DWORD PTR SS:[ESP+0x70],EBX
	$+23     00561F87      E8 1458F0FF           CALL MHOClien.004677A0

	$ ==>    00561FC4       8B82 A8000000      MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+6      00561FCA       FFD0               CALL EAX
	$+8      00561FCC       3BC6               CMP EAX,ESI
	$+A      00561FCE       0F84 BF070000      JE MHOClien.00562793
	$+10     00561FD4       8B4C24 0C          MOV ECX,DWORD PTR SS:[ESP+0xC]
	$+14     00561FD8       8D98 883F0000      LEA EBX,DWORD PTR DS:[EAX+0x3F88]
	$+1A     00561FDE       51                 PUSH ECX
	$+1B     00561FDF       8D4C24 74          LEA ECX,DWORD PTR SS:[ESP+0x74]
	$+1F     00561FE3       895C24 70          MOV DWORD PTR SS:[ESP+0x70],EBX
	$+23     00561FE7       E8 D45AF0FF        CALL MHOClien.00467AC0

	8B??????????FF??3B??0F84????????8B??????8D????????????8D

	$ ==>    0C9370C3      8B82 A8000000              MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+6      0C9370C9      FFD0                       CALL EAX
	$+8      0C9370CB      3BC3                       CMP EAX,EBX
	$+A      0C9370CD      0F84 50070000              JE CryGame.0C937823
	$+10     0C9370D3      05 603F0000                ADD EAX,0x3F60                                           ; +0x3F60
	$+15     0C9370D8      894424 54                  MOV DWORD PTR SS:[ESP+0x54],EAX
	$+19     0C9370DC      8B4424 0C                  MOV EAX,DWORD PTR SS:[ESP+0xC]
	$+1D     0C9370E0      8D7424 70                  LEA ESI,DWORD PTR SS:[ESP+0x70]
	$+21     0C9370E4      E8 B7B0D3FF                CALL CryGame.0C6721A0
	$+26     0C9370E9      8D8C24 A8000000            LEA ECX,DWORD PTR SS:[ESP+0xA8]
	$+2D     0C9370F0      51                         PUSH ECX
	$+2E     0C9370F1      8BCF                       MOV ECX,EDI
	$+30     0C9370F3      899C24 AC000000            MOV DWORD PTR SS:[ESP+0xAC],EBX
	$+37     0C9370FA      899C24 B0000000            MOV DWORD PTR SS:[ESP+0xB0],EBX
	$+3E     0C937101      E8 3AE7DF00                CALL CryGame.0D735840
	$+43     0C937106      8B5424 74                  MOV EDX,DWORD PTR SS:[ESP+0x74]
	$+47     0C93710A      8B4C24 70                  MOV ECX,DWORD PTR SS:[ESP+0x70]
	$+4B     0C93710E      81E2 8F000000              AND EDX,0x8F
	$+51     0C937114      80FA 0A                    CMP DL,0xA
	$+54     0C937117      0F94C0                     SETE AL

	8B??????????FF??3B??0F??????????05????????89
	--*/
	DWORD	dwMMOClientFarmFacilityStateOffset1 = *scan_game_.ScanImme<DWORD>("8B??????????FF??3B??0F??????????05????????89", 0x11);
	assert(dwMMOClientFarmFacilityStateOffset1);
	//assert(dwMMOClientFarmFacilityStateOffset1==0x47C8/*0x000042A8*//*0x00004298*//*0x00004028*/  /*0x3F60*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET1  " << dwMMOClientFarmFacilityStateOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-13     01BB0A40      55                    PUSH EBP
	$-12     01BB0A41      8BEC                  MOV EBP,ESP
	$-10     01BB0A43      8B45 0C               MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-D      01BB0A46      83F8 02               CMP EAX,0x2
	$-A      01BB0A49      0F87 94000000         JA MHOClien.01BB0AE3
	$-4      01BB0A4F      56                    PUSH ESI
	$-3      01BB0A50      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$ ==>    01BB0A53      8D0440                LEA EAX,DWORD PTR DS:[EAX+EAX*2]
	$+3      01BB0A56      80BC86 84010000 00    CMP BYTE PTR DS:[ESI+EAX*4+0x184],0x0
	$+B      01BB0A5E      8D8486 84010000       LEA EAX,DWORD PTR DS:[ESI+EAX*4+0x184]
	$+12     01BB0A65      8945 0C               MOV DWORD PTR SS:[EBP+0xC],EAX
	$+15     01BB0A68      74 72                 JE SHORT MHOClien.01BB0ADC
	$+17     01BB0A6A      8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+1A     01BB0A6D      85C0                  TEST EAX,EAX
	$+1C     01BB0A6F      74 6B                 JE SHORT MHOClien.01BB0ADC
	$+1E     01BB0A71      50                    PUSH EAX
	$+1F     01BB0A72      E8 89089BFE           CALL MHOClien.00561300                   ; 返回种子信息地址
	$+24     01BB0A77      83C4 04               ADD ESP,0x4
	$+27     01BB0A7A      85C0                  TEST EAX,EAX
	$+29     01BB0A7C      74 5E                 JE SHORT MHOClien.01BB0ADC
	$+2B     01BB0A7E      0FB78E 8C000000       MOVZX ECX,WORD PTR DS:[ESI+0x8C]
	$+32     01BB0A85      53                    PUSH EBX                                 ; 采集点种植的植物物品地址
	$+33     01BB0A86      51                    PUSH ECX                                 ; 采集点序号
	$+34     01BB0A87      8BC8                  MOV ECX,EAX                              ; 种子信息地址
	$+36     01BB0A89      E8 92C3CEFE           CALL MHOClien.0089CE20
	$+3B     01BB0A8E      8BD8                  MOV EBX,EAX
	$+3D     01BB0A90      85DB                  TEST EBX,EBX
	$+3F     01BB0A92      75 06                 JNZ SHORT MHOClien.01BB0A9A

	8D????80??????????????8D????????????89????74??8B

	$-13     01C1D9D0          8B4424 08             MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-F      01C1D9D4          83F8 02               CMP EAX,0x2
	$-C      01C1D9D7          0F87 90000000         JA MHOClien.01C1DA6D
	$-6      01C1D9DD          55                    PUSH EBP
	$-5      01C1D9DE          56                    PUSH ESI
	$-4      01C1D9DF          8B7424 0C             MOV ESI,DWORD PTR SS:[ESP+0xC]
	$ ==>    01C1D9E3          8D0440                LEA EAX,DWORD PTR DS:[EAX+EAX*2]
	$+3      01C1D9E6          80BC86 84010000 00    CMP BYTE PTR DS:[ESI+EAX*4+0x184],0x0
	$+B      01C1D9EE          8DAC86 84010000       LEA EBP,DWORD PTR DS:[ESI+EAX*4+0x184]
	$+12     01C1D9F5          74 6F                 JE SHORT MHOClien.01C1DA66
	$+14     01C1D9F7          8B45 04               MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+17     01C1D9FA          85C0                  TEST EAX,EAX
	$+19     01C1D9FC          74 68                 JE SHORT MHOClien.01C1DA66
	$+1B     01C1D9FE          50                    PUSH EAX
	$+1C     01C1D9FF          E8 DC6494FE           CALL MHOClien.00563EE0
	$+21     01C1DA04          83C4 04               ADD ESP,0x4
	$+24     01C1DA07          85C0                  TEST EAX,EAX
	$+26     01C1DA09          74 5B                 JE SHORT MHOClien.01C1DA66
	$+28     01C1DA0B          0FB78E 8C000000       MOVZX ECX,WORD PTR DS:[ESI+0x8C]
	$+2F     01C1DA12          53                    PUSH EBX
	$+30     01C1DA13          51                    PUSH ECX
	$+31     01C1DA14          8BC8                  MOV ECX,EAX
	$+33     01C1DA16          E8 B5B2C8FE           CALL MHOClien.008A8CD0
	$+38     01C1DA1B          8BD8                  MOV EBX,EAX
	$+3A     01C1DA1D          85DB                  TEST EBX,EBX
	$+3C     01C1DA1F          75 06                 JNZ SHORT MHOClien.01C1DA27
	$+3E     01C1DA21          5B                    POP EBX
	$+3F     01C1DA22          5E                    POP ESI
	$+40     01C1DA23          33D2                  XOR EDX,EDX
	$+42     01C1DA25          5D                    POP EBP
	$+43     01C1DA26          C3                    RETN
	$+44     01C1DA27          57                    PUSH EDI
	$+45     01C1DA28          6A 00                 PUSH 0x0
	$+47     01C1DA2A          E8 67AEFAFF           CALL MHOClien.01BC8896                   ; JMP 到 msvcr90._time64

	8D04??80??????????????8D????????????74
	--*/
	auto optional_dword = scan_game_.ScanImme<DWORD>("8D????80??????????????8D????????????89????74??8B", 0x6);
	DWORD	dwMMOClientFarmFacilityStateOffset2 = 0;
	if (optional_dword)
		dwMMOClientFarmFacilityStateOffset2 = *optional_dword;
	if (dwMMOClientFarmFacilityStateOffset2 == 0)
		dwMMOClientFarmFacilityStateOffset2 = *scan_game_.ScanImme<DWORD>("8D04??80??????????????8D????????????74", 0x6);
	assert(dwMMOClientFarmFacilityStateOffset2);
	assert(dwMMOClientFarmFacilityStateOffset2 == 0xBC);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET2  " << dwMMOClientFarmFacilityStateOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientFarmFacilityStateTimeCall = (DWORD)scan_game_.ScanAddr("8D????80??????????????8D????????????89????74??8B", -0x13);
	if (dwMMOClientFarmFacilityStateTimeCall == 0)
		dwMMOClientFarmFacilityStateTimeCall = (DWORD)scan_game_.ScanAddr("8D04??80??????????????8D????????????74", -0x13);
	assert(dwMMOClientFarmFacilityStateTimeCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_TIME_CALL  " << dwMMOClientFarmFacilityStateTimeCall - GetModuleCryGameBase() << "  // " << dwMMOClientFarmFacilityStateTimeCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    01BF80C2         E8 694A88FE     CALL MHOClien.0047CB30
	$+5      01BF80C7         8BF8            MOV EDI,EAX
	$+7      01BF80C9         83C4 04         ADD ESP,0x4
	$+A      01BF80CC         85FF            TEST EDI,EDI
	$+C      01BF80CE         74 19           JE SHORT MHOClien.01BF80E9
	$+E      01BF80D0         8B4E 04         MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+11     01BF80D3         81C1 16080000   ADD ECX,0x816                            ; 存储斩味值
	$+17     01BF80D9         E8 D24187FE     CALL MHOClien.0046C2B0                   ; 解密
	$+1C     01BF80DE         50              PUSH EAX
	$+1D     01BF80DF         8BCF            MOV ECX,EDI
	$+1F     01BF80E1         E8 4A81C0FE     CALL MHOClien.00800230

	E8????????8B??83????85??74??8B????81C1????????E8

	//zhp
	$-12     >  56              PUSH ESI
	$-11     >  8BF1            MOV ESI,ECX
	$-F      >  8B4E 04         MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-C      >  8B01            MOV EAX,DWORD PTR DS:[ECX]
	$-A      >  8B90 EC030000   MOV EDX,DWORD PTR DS:[EAX+0x3EC]
	$-4      >  57              PUSH EDI
	$-3      >  FFD2            CALL EDX
	$-1      >  50              PUSH EAX
	$ ==>    >  E8 D9F305FF     CALL CryGame.0A732E50
	$+5      >  8BF8            MOV EDI,EAX
	$+7      >  83C4 04         ADD ESP,0x4
	$+A      >  85FF            TEST EDI,EDI
	$+C      >  74 19           JE SHORT CryGame.0B6D3A99
	$+E      >  8B4E 04         MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+11     >  81C1 1E080000   ADD ECX,0x81E
	$+17     >  E8 B243EBFE     CALL CryGame.0A587E40
	$+1C     >  50              PUSH EAX
	$+1D     >  8BCF            MOV ECX,EDI
	$+1F     >  E8 2A21BEFF     CALL CryGame.0B2B5BC0
	$+24     >  5F              POP EDI
	$+25     >  5E              POP ESI
	$+26     >  C3              RETN
	$+27     >  5F              POP EDI
	$+28     >  33C0            XOR EAX,EAX
	$+2A     >  5E              POP ESI
	$+2B     >  C3              RETN

	E8????????8B??83????85??74??8B????81C1????????E8????????508BCFE8????????5F5EC35F33C05EC3
	--*/
	DWORD	dwMMOClientRoleZhanWeiZhiOffset = *scan_game_.ScanImme<DWORD>("E8????????8B??83????85??74??8B????81C1????????E8????????508BCFE8????????5F5EC35F33C05EC3", 0x13);
	assert(dwMMOClientRoleZhanWeiZhiOffset);
	//assert(dwMMOClientRoleZhanWeiZhiOffset==0x81E/*0x816*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_ZHAN_WEI_ZHI_OFFSET  " << dwMMOClientRoleZhanWeiZhiOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-2E     005F50FA         8D8F D60F0000       LEA ECX,DWORD PTR DS:[EDI+0xFD6]         ; 炼气槽
	$-28     005F5100         895C24 20           MOV DWORD PTR SS:[ESP+0x20],EBX
	$-24     005F5104         895C24 24           MOV DWORD PTR SS:[ESP+0x24],EBX
	$-20     005F5108         895C24 28           MOV DWORD PTR SS:[ESP+0x28],EBX
	$-1C     005F510C         885C24 2C           MOV BYTE PTR SS:[ESP+0x2C],BL
	$-18     005F5110         E8 9B71E7FF         CALL MHOClien.0046C2B0
	$-13     005F5115         8D8F 0E100000       LEA ECX,DWORD PTR DS:[EDI+0x100E]
	$-D      005F511B         894424 24           MOV DWORD PTR SS:[ESP+0x24],EAX
	$-9      005F511F         E8 8C71E7FF         CALL MHOClien.0046C2B0
	$-4      005F5124         894424 20           MOV DWORD PTR SS:[ESP+0x20],EAX
	$ ==>    005F5128         399F 20210000       CMP DWORD PTR DS:[EDI+0x2120],EBX
	$+6      005F512E         74 3E               JE SHORT MHOClien.005F516E
	$+8      005F5130         8B8F 20210000       MOV ECX,DWORD PTR DS:[EDI+0x2120]
	$+E      005F5136         8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+10     005F5138         8B90 80000000       MOV EDX,DWORD PTR DS:[EAX+0x80]
	$+16     005F513E         FFD2                CALL EDX
	$+18     005F5140         8B8F 20210000       MOV ECX,DWORD PTR DS:[EDI+0x2120]
	$+1E     005F5146         884424 2C           MOV BYTE PTR SS:[ESP+0x2C],AL
	$+22     005F514A         8B01                MOV EAX,DWORD PTR DS:[ECX]
	$+24     005F514C         8B50 78             MOV EDX,DWORD PTR DS:[EAX+0x78]
	$+27     005F514F         FFD2                CALL EDX

	39??????????74??8B??????????8B??8B??????????FF??8B

	$-28     0D05225E      8D8E AE0F0000              LEA ECX,DWORD PTR DS:[ESI+0xFAE]                         ; 炼气槽
	$-22     0D052264      895D E4                    MOV DWORD PTR SS:[EBP-0x1C],EBX
	$-1F     0D052267      895D E8                    MOV DWORD PTR SS:[EBP-0x18],EBX
	$-1C     0D05226A      895D EC                    MOV DWORD PTR SS:[EBP-0x14],EBX
	$-19     0D05226D      885D F0                    MOV BYTE PTR SS:[EBP-0x10],BL
	$-16     0D052270      E8 FB5F4CFF                CALL CryGame.0C518270
	$-11     0D052275      8D8E E60F0000              LEA ECX,DWORD PTR DS:[ESI+0xFE6]
	$-B      0D05227B      8945 E8                    MOV DWORD PTR SS:[EBP-0x18],EAX
	$-8      0D05227E      E8 ED5F4CFF                CALL CryGame.0C518270
	$-3      0D052283      8945 E4                    MOV DWORD PTR SS:[EBP-0x1C],EAX
	$ ==>    0D052286      399E F0210000              CMP DWORD PTR DS:[ESI+0x21F0],EBX
	$+6      0D05228C      74 3B                      JE SHORT CryGame.0D0522C9
	$+8      0D05228E      8B8E F0210000              MOV ECX,DWORD PTR DS:[ESI+0x21F0]
	$+E      0D052294      8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+10     0D052296      8B90 84000000              MOV EDX,DWORD PTR DS:[EAX+0x84]
	$+16     0D05229C      FFD2                       CALL EDX
	$+18     0D05229E      8B8E F0210000              MOV ECX,DWORD PTR DS:[ESI+0x21F0]
	$+1E     0D0522A4      8845 F0                    MOV BYTE PTR SS:[EBP-0x10],AL
	$+21     0D0522A7      8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+23     0D0522A9      8B50 7C                    MOV EDX,DWORD PTR DS:[EAX+0x7C]
	$+26     0D0522AC      FFD2                       CALL EDX

	39??????????74??8B??????????8B??8B??????????FF??8B
	--*/
	DWORD	dwMMOClientRoleLianQiCaoOffset = *scan_game_.ScanImme<DWORD>("39??????????74??8B??????????8B??8B??????????FF??8B", -0x26);
	assert(dwMMOClientRoleLianQiCaoOffset);
	//assert(dwMMOClientRoleLianQiCaoOffset==0x00000FB6/*0xFAE*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_LIAN_QI_CAO_OFFSET  " << dwMMOClientRoleLianQiCaoOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*

	$-9      >  55              push ebp
	$-8      >  8BEC            mov ebp,esp
	$-6      >  8B45 08         mov eax,dword ptr ss:[ebp+0x8]
	$-3      >  56              push esi
	$-2      >  8BF1            mov esi,ecx
	$ ==>    >  83F8 08         cmp eax,0x8
	$+3      >  75 0C           jnz short CryGame.0B823E1A
	$+5      >  6A 06           push 0x6
	$+7      >  E8 EBFFFFFF     call CryGame.0B823E00
	$+C      >  B8 07000000     mov eax,0x7
	$+11     >  FF8486 B0220000 inc dword ptr ds:[esi+eax*4+0x22B0]
	$+18     >  83BC86 B0220000>cmp dword ptr ds:[esi+eax*4+0x22B0],0x1  ; 人物状态 中毒要加*0XA*4
	$+20     >  75 0D           jnz short CryGame.0B823E38
	$+22     >  8B16            mov edx,dword ptr ds:[esi]
	$+24     >  50              push eax
	$+25     >  8B82 38040000   mov eax,dword ptr ds:[edx+0x438]
	$+2B     >  8BCE            mov ecx,esi
	$+2D     >  FFD0            call eax
	$+2F     >  5E              pop esi
	$+30     >  5D              pop ebp
	$+31     >  C2 0400         retn 0x4
	$+34     >  CC              int3
	$+35     >  CC              int3
	$+36     >  CC              int3
	$+37     >  C2 0400         retn 0x4

	83F808750C6A06

	*/

	DWORD	dwMMOClientRoleBuffStateOffset = *scan_game_.ScanImme<DWORD>("83F808750C6A06", 0x14);
	assert(dwMMOClientRoleBuffStateOffset);
	//assert(dwMMOClientRoleBuffStateOffset==0x22B8);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET  " << dwMMOClientRoleBuffStateOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;




	/*++
	$ ==>    >  8B46 04         MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+3      >  53              PUSH EBX
	$+4      >  50              PUSH EAX
	$+5      >  E8 8F0B8DFE     CALL MHOClien.0047A200                   ; 获取buff信息
	{
	$ ==>    0047A200      55                  PUSH EBP
	$+1      0047A201      8BEC                MOV EBP,ESP
	$+3      0047A203      8B45 08             MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+6      0047A206      50                  PUSH EAX
	$+7      0047A207      6A 00               PUSH 0x0
	$+9      0047A209      68 803FFF01         PUSH MHOClien.01FF3F80                   ; ASCII "CBuffInfo"
	$+E      0047A20E      6A 00               PUSH 0x0
	$+10     0047A210      E8 5B550000         CALL MHOClien.0047F770
	$+15     0047A215      83C4 0C             ADD ESP,0xC
	$+18     0047A218      8BC8                MOV ECX,EAX
	$+1A     0047A21A      E8 D16B1100         CALL MHOClien.00590DF0
	$+1F     0047A21F      5D                  POP EBP
	$+20     0047A220      C3                  RETN
	}
	$+A      >  8BD8            MOV EBX,EAX
	$+C      >  83C4 04         ADD ESP,0x4
	$+F      >  899D 74FFFFFF   MOV DWORD PTR SS:[EBP-0x8C],EBX
	$+15     >  85DB            TEST EBX,EBX
	$+17     >  75 16           JNZ SHORT MHOClien.01BA9696

	8B????????E8????????8B??83????89??????????85??75
	--*/
	DWORD	dwMMOClientGetBuffInfoAddrCall = (DWORD)scan_game_.ScanCall("8B????????E8????????8B??83????89??????????85??75", 6);
	assert(dwMMOClientGetBuffInfoAddrCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_BUFF_INFO_ADDR_CALL  " << dwMMOClientGetBuffInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetBuffInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    006B3132      A1 587F5102         MOV EAX,DWORD PTR DS:[0x2517F58]
	$+5      006B3137      8B48 04             MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+8      006B313A      8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+A      006B313C      8B42 38             MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+D      006B313F      FFD0                CALL EAX
	$+F      006B3141      8B10                MOV EDX,DWORD PTR DS:[EAX]
	$+11     006B3143      8BC8                MOV ECX,EAX
	$+13     006B3145      8B82 70010000       MOV EAX,DWORD PTR DS:[EDX+0x170]
	$+19     006B314B      FFD0                CALL EAX
	$+1B     006B314D      84C0                TEST AL,AL
	$+1D     006B314F      75 18               JNZ SHORT MHOClien.006B3169
	$+1F     006B3151      6A 05               PUSH 0x5
	$+21     006B3153      FF15 EC22D201       CALL DWORD PTR DS:[0x1D222EC]
	$+27     006B3159      8B45 FC             MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+2A     006B315C      40                  INC EAX
	$+2B     006B315D      83F8 05             CMP EAX,0x5
	$+2E     006B3160      8945 FC             MOV DWORD PTR SS:[EBP-0x4],EAX
	$+31     006B3163    ^ 0F8C 5FFFFFFF       JL MHOClien.006B30C8
	$+37     006B3169      5F                  POP EDI
	$+38     006B316A      5B                  POP EBX
	$+39     006B316B      5E                  POP ESI                                  ; hook此处，Sleep优化
	$+3A     006B316C      8BE5                MOV ESP,EBP
	$+3C     006B316E      5D                  POP EBP
	$+3D     006B316F      C3                  RETN

	A1????????8B????8B??8B????FF??8B??8B??8B??????????FF??84??75
	--*/
	DWORD dwMMOClientRenderAddrHook = (DWORD)scan_exe_.ScanAddr("A1????????8B????8B??8B????FF??8B??8B??8B??????????FF??84??75", 0x39);
	assert(*(byte*)dwMMOClientRenderAddrHook == 0x5E);
	assert(dwMMOClientRenderAddrHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RENDER_ADDR_HOOK  " << dwMMOClientRenderAddrHook << "  // MHOClient.exe" << FC_OFstream_EndlLog;

	/*++
	$-5      01B8F6B0      55              PUSH EBP
	$-4      01B8F6B1      8BEC            MOV EBP,ESP
	$-2      01B8F6B3      8BC1            MOV EAX,ECX
	$ ==>    01B8F6B5      8B4D 0C         MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+3      01B8F6B8      8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+5      01B8F6BA      8B4D 10         MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+8      01B8F6BD      6A 00           PUSH 0x0
	$+A      01B8F6BF      51              PUSH ECX
	$+B      01B8F6C0      8B48 14         MOV ECX,DWORD PTR DS:[EAX+0x14]
	$+E      01B8F6C3      034D 08         ADD ECX,DWORD PTR SS:[EBP+0x8]
	$+11     01B8F6C6      8B40 10         MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+14     01B8F6C9      8955 0C         MOV DWORD PTR SS:[EBP+0xC],EDX
	$+17     01B8F6CC      8D55 0C         LEA EDX,DWORD PTR SS:[EBP+0xC]
	$+1A     01B8F6CF      52              PUSH EDX
	$+1B     01B8F6D0      FFD0            CALL EAX	// 调用 UPDATES_MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK,如果特征码无效，可根据此宏函数地址，找到调用他的上一层，过滤一下，就可以得到正确的血值了
	$+1D     01B8F6D2      5D              POP EBP                                  ; 取调用层地址
	$+1E     01B8F6D3      C2 0C00         RETN 0xC

	8B????8B??8B????6A????8B????03????8B????89????8D??????FF
	--*/
	// 	DWORD dwMMOClientMonsterHpCallCmpAddr2 = CRY_GAME_ScanAddr((HANDLE)-1,"8B????8B??8B????6A????8B????03????8B????89????8D??????FF",0x1D,4,3);
	// 	ASSERT_GWLR(dwMMOClientMonsterHpCallCmpAddr2);
	// 	LOG << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HP_CALL_CMP_ADDR2  ";
	// 	LOG << dwMMOClientMonsterHpCallCmpAddr2-GetModuleCryGameBase();
	// 	LOG << "  // ";
	// 	LOG << dwMMOClientMonsterHpCallCmpAddr2;
	// 	LOG << " CryGame.dll";
	// 	LOG_END;//特征码失效

	/*++
	$ ==>    005713C1      C703 00000000              MOV DWORD PTR DS:[EBX],0x0
	$+6      005713C7      C743 04 02000000           MOV DWORD PTR DS:[EBX+0x4],0x2
	$+D      005713CE      C643 08 00                 MOV BYTE PTR DS:[EBX+0x8],0x0
	$+11     005713D2      A1 587F5102                MOV EAX,DWORD PTR DS:[0x2517F58]
	$+16     005713D7      8B88 C8000000              MOV ECX,DWORD PTR DS:[EAX+0xC8]
	$+1C     005713DD      8B49 28                    MOV ECX,DWORD PTR DS:[ECX+0x28]
	$+1F     005713E0      8B11                       MOV EDX,DWORD PTR DS:[ECX]
	$+21     005713E2      8B82 A8000000              MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+27     005713E8      FFD0                       CALL EAX
	$+29     005713EA      894424 7C                  MOV DWORD PTR SS:[ESP+0x7C],EAX
	$+2D     005713EE      85C0                       TEST EAX,EAX
	$+2F     005713F0      0F84 83090000              JE MHOClien.00571D79
	$+35     005713F6      8B80 1C200000              MOV EAX,DWORD PTR DS:[EAX+0x201C]		// +0x201C
	$+3B     005713FC      898424 80000000            MOV DWORD PTR SS:[ESP+0x80],EAX
	$+42     00571403      85C0                       TEST EAX,EAX
	$+44     00571405      0F84 6E090000              JE MHOClien.00571D79
	$+4A     0057140B      8BC8                       MOV ECX,EAX

	C7??????????C7????????????C6??????A1????????8B??????????8B????8B??8B??????????FF??89

	$ ==>    0C94F4D4      8B88 C8000000              MOV ECX,DWORD PTR DS:[EAX+0xC8]
	$+6      0C94F4DA      8B49 1C                    MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$+9      0C94F4DD      8B11                       MOV EDX,DWORD PTR DS:[ECX]
	$+B      0C94F4DF      8B82 A8000000              MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+11     0C94F4E5      FFD0                       CALL EAX
	$+13     0C94F4E7      898424 80000000            MOV DWORD PTR SS:[ESP+0x80],EAX
	$+1A     0C94F4EE      85C0                       TEST EAX,EAX
	$+1C     0C94F4F0      0F84 88090000              JE CryGame.0C94FE7E
	$+22     0C94F4F6      8B80 58200000              MOV EAX,DWORD PTR DS:[EAX+0x2058]  // +0x2058
	$+28     0C94F4FC      898424 84000000            MOV DWORD PTR SS:[ESP+0x84],EAX
	$+2F     0C94F503      85C0                       TEST EAX,EAX
	$+31     0C94F505      0F84 73090000              JE CryGame.0C94FE7E
	$+37     0C94F50B      8BC8                       MOV ECX,EAX
	$+39     0C94F50D      E8 5E04D000                CALL CryGame.0D64F970
	$+3E     0C94F512      8BC8                       MOV ECX,EAX
	$+40     0C94F514      E8 C715D000                CALL CryGame.0D650AE0
	$+45     0C94F519      898424 88000000            MOV DWORD PTR SS:[ESP+0x88],EAX
	$+4C     0C94F520      85C0                       TEST EAX,EAX
	$+4E     0C94F522      0F84 56090000              JE CryGame.0C94FE7E

	8B??????????8B????8B??8B??????????FF??89????????????85??0F??????????8B??????????89

	// 不删档测试
	$ ==>    >  8B88 CC000000   MOV ECX,DWORD PTR DS:[EAX+0xCC]
	$+6      >  8B49 1C         MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$+9      >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+B      >  8B82 A8000000   MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+11     >  FFD0            CALL EAX
	$+13     >  894424 68       MOV DWORD PTR SS:[ESP+0x68],EAX
	$+17     >  85C0            TEST EAX,EAX
	$+19     >  0F84 D7080000   JE CryGame.0C96E88A
	$+1F     >  8B80 F8200000   MOV EAX,DWORD PTR DS:[EAX+0x20F8]
	$+25     >  894424 70       MOV DWORD PTR SS:[ESP+0x70],EAX
	$+29     >  85C0            TEST EAX,EAX

	8B??????????8B????8B??8B??????????FF??89??????85??0F??????????8B??????????89??????85

	//zhp
	$-40     >  85DB            TEST EBX,EBX
	$-3E     >  0F84 21090000   JE CryGame.0AA1B56D
	$-38     >  837D 18 00      CMP DWORD PTR SS:[EBP+0x18],0x0
	$-34     >  0F84 17090000   JE CryGame.0AA1B56D
	$-2E     >  8B46 04         MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-2B     >  C1E8 06         SHR EAX,0x6
	$-28     >  A8 01           TEST AL,0x1
	$-26     >  74 14           JE SHORT CryGame.0AA1AC74
	$-24     >  8B0E            MOV ECX,DWORD PTR DS:[ESI]
	$-22     >  8B46 08         MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-1F     >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$-1D     >  8B52 08         MOV EDX,DWORD PTR DS:[EDX+0x8]
	$-1A     >  50              PUSH EAX
	$-19     >  56              PUSH ESI
	$-18     >  FFD2            CALL EDX
	$-16     >  C706 00000000   MOV DWORD PTR DS:[ESI],0x0
	$-10     >  A1 0098DE0B     MOV EAX,DWORD PTR DS:[0xBDE9800]
	$-B      >  C746 04 0200000>MOV DWORD PTR DS:[ESI+0x4],0x2
	$-4      >  C646 08 00      MOV BYTE PTR DS:[ESI+0x8],0x0
	$ ==>    >  8B88 D0000000   MOV ECX,DWORD PTR DS:[EAX+0xD0]
	$+6      >  8B49 1C         MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$+9      >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+B      >  8B82 A8000000   MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+11     >  FFD0            CALL EAX
	$+13     >  894424 68       MOV DWORD PTR SS:[ESP+0x68],EAX
	$+17     >  85C0            TEST EAX,EAX
	$+19     >  0F84 CA080000   JE CryGame.0AA1B56D
	$+1F     >  8B80 F4210000   MOV EAX,DWORD PTR DS:[EAX+0x21F4]
	$+25     >  894424 70       MOV DWORD PTR SS:[ESP+0x70],EAX
	$+29     >  85C0            TEST EAX,EAX
	$+2B     >  0F84 B8080000   JE CryGame.0AA1B56D
	$+31     >  8BC8            MOV ECX,EAX
	$+33     >  E8 A4DFDA00     CALL CryGame.0B7C8C60
	$+38     >  8BC8            MOV ECX,EAX
	$+3A     >  E8 CDF1DA00     CALL CryGame.0B7C9E90
	$+3F     >  898424 88000000 MOV DWORD PTR SS:[ESP+0x88],EAX
	$+46     >  85C0            TEST EAX,EAX
	$+48     >  0F84 9B080000   JE CryGame.0AA1B56D
	$+4E     >  8BC3            MOV EAX,EBX
	$+50     >  8D7424 20       LEA ESI,DWORD PTR SS:[ESP+0x20]
	$+54     >  E8 A383CEFF     CALL CryGame.0A703080

	8B??????????8B????8B??8B??????????FF??89??????85??0F??????????8B??????????89??????85??0F84????????8BC8E8????????8BC8
	--*/
	DWORD dwMMOClientGiftBagInfoOffset1 = *scan_game_.ScanImme<DWORD>("8B??????????8B????8B??8B??????????FF??89??????85??0F??????????8B??????????89??????85??0F84????????8BC8E8????????8BC8", 0x1F + 2);
	assert(dwMMOClientGiftBagInfoOffset1);
	//assert(dwMMOClientGiftBagInfoOffset1==0x21F4 /*0x0000219C*//*0x0000215C*//*0x000020F8*/  /*0x2058*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET1  " << dwMMOClientGiftBagInfoOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    015A4DFE      FFD0                 CALL EAX
	$+2      015A4E00      8BF0                 MOV ESI,EAX
	$+4      015A4E02      33FF                 XOR EDI,EDI
	$+6      015A4E04      3BF7                 CMP ESI,EDI
	$+8      015A4E06      0F84 16010000        JE MHOClien.015A4F22
	$+E      015A4E0C      8D8E 34040000        LEA ECX,DWORD PTR DS:[ESI+0x434]
	$+14     015A4E12      53                   PUSH EBX
	$+15     015A4E13      894D FC              MOV DWORD PTR SS:[EBP-0x4],ECX
	$+18     015A4E16      E8 85C5FBFE          CALL MHOClien.005613A0
	$+1D     015A4E1B      50                   PUSH EAX                                 ; push 等级
	$+1E     015A4E1C      E8 DF7CFBFE          CALL MHOClien.0055CB00                   ; 返回等级经验信息
	$+23     015A4E21      8BD8                 MOV EBX,EAX
	$+25     015A4E23      83C4 04              ADD ESP,0x4
	$+28     015A4E26      3BDF                 CMP EBX,EDI
	$+2A     015A4E28      0F84 F3000000        JE MHOClien.015A4F21

	FF??8B??33??3B??0F??????????8D????????????89????E8??????????E8

	$-13     006BC5FA      8B4D F0         MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-10     006BC5FD      51              PUSH ECX
	$-F      006BC5FE      E8 0D010000     CALL MHOClien.006BC710                   ; CPlayerLevelUpInfo
	$-A      006BC603      83C4 04         ADD ESP,0x4
	$-7      006BC606      8945 E8         MOV DWORD PTR SS:[EBP-0x18],EAX
	$-4      006BC609      837D E8 00      CMP DWORD PTR SS:[EBP-0x18],0x0
	$ ==>    006BC60D      75 0B           JNZ SHORT MHOClien.006BC61A
	$+2      006BC60F      33D2            XOR EDX,EDX
	$+4      006BC611    ^ 75 FC           JNZ SHORT MHOClien.006BC60F
	$+6      006BC613      33C0            XOR EAX,EAX
	$+8      006BC615      E9 B2000000     JMP MHOClien.006BC6CC
	$+D      006BC61A      33C0            XOR EAX,EAX
	$+F      006BC61C    ^ 75 EB           JNZ SHORT MHOClien.006BC609
	$+11     006BC61E      8B4D E8         MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+14     006BC621      8B55 F4         MOV EDX,DWORD PTR SS:[EBP-0xC]
	$+17     006BC624      0351 14         ADD EDX,DWORD PTR DS:[ECX+0x14]
	$+1A     006BC627      8955 F4         MOV DWORD PTR SS:[EBP-0xC],EDX
	$+1D     006BC62A      8B45 F0         MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+20     006BC62D      50              PUSH EAX
	$+21     006BC62E      E8 6D9EF9FF     CALL MHOClien.006564A0

	75??33??75??33??E9????????33??75??8B

	--*/
	DWORD dwMMOClientPlayerLevelUpInfoCall = (DWORD)scan_game_.ScanCall("75??33??75??33??E9????????33??75??8B", -0xF + 1);
	assert(dwMMOClientPlayerLevelUpInfoCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_PLAYER_LEVEL_UP_INFO_CALL  " << dwMMOClientPlayerLevelUpInfoCall - GetModuleCryGameBase() << "  // " << dwMMOClientPlayerLevelUpInfoCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	RECV 变化很大，留着最后更新
	$ ==>    007748B5     52                  PUSH EDX
	$+1      007748B6     50                  PUSH EAX
	$+2      007748B7     8D4F 10             LEA ECX,DWORD PTR DS:[EDI+0x10]
	$+5      007748BA     8BC6                MOV EAX,ESI
	$+7      007748BC     E8 0FB9FFFF         CALL MHOClien.007701D0
	$+C      007748C1     5B                  POP EBX                                  ; hook 处
	$+D      007748C2     8BE5                MOV ESP,EBP
	$+F      007748C4     5D                  POP EBP
	$+10     007748C5     C3                  RETN


	52508D4F??8BC6E8????????5B8BE55DC3

	$ ==>    >  E8 7CE5FDFF     CALL CryGame.0C4ED3B0
	$+5      >  85C0            TEST EAX,EAX
	$+7      >  75 1F           JNZ SHORT CryGame.0C50EE57
	$+9      >  8B4E 08         MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+C      >  0FB707          MOVZX EAX,WORD PTR DS:[EDI]
	$+F      >  2B4E 04         SUB ECX,DWORD PTR DS:[ESI+0x4]
	$+12     >  99              CDQ
	$+13     >  394F 04         CMP DWORD PTR DS:[EDI+0x4],ECX
	$+16     >  77 17           JA SHORT CryGame.0C50EE5E
	$+18     >  56              PUSH ESI
	$+19     >  52              PUSH EDX
	$+1A     >  50              PUSH EAX
	$+1B     >  8D57 10         LEA EDX,DWORD PTR DS:[EDI+0x10]
	$+1E     >  B9 01000000     MOV ECX,0x1
	$+23     >  E8 9989FEFF     CALL CryGame.0C4F77F0
	$+28     >  5F              POP EDI							; HOOK
	$+29     >  5E              POP ESI
	$+2A     >  5B              POP EBX
	$+2B     >  8BE5            MOV ESP,EBP
	$+2D     >  5D              POP EBP
	$+2E     >  C3              RETN

	E8????????85??75??8b????0F????2B????9939????77
	--*/
	DWORD	dwMMOClientRecvHook = (DWORD)scan_game_.ScanAddr("E8????????85??75??8b????0F????2B????9939????77", 0x28);
	assert(dwMMOClientRecvHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_RECV_HOOK  " << dwMMOClientRecvHook - GetModuleCryGameBase() << "  // " << dwMMOClientRecvHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    00624302      E8 8112C500     CALL MHOClien.01275588                   ; 攻击函数 hook 处理无敌
	$+5      00624307      C7C0 329DF25B   MOV EAX,0x5BF29D32
	$+B      0062430D      8D80 0ACC65A6   LEA EAX,DWORD PTR DS:[EAX+0xA665CC0A]
	$+11     00624313      8B08            MOV ECX,DWORD PTR DS:[EAX]
	$+13     00624315      8B49 04         MOV ECX,DWORD PTR DS:[ECX+0x4]
	$+16     00624318      8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+18     0062431A      8B42 38         MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+1B     0062431D      FFD0            CALL EAX

	E8????????C7??????????8D??????????8B??8B????8B??8B????FF

	$ ==>    02A5FDA1      E8 1AE99CFD        CALL MHOClien.0042E6C0                   ; 处理无敌的函数
	$+5      02A5FDA6      C7C0 2C2AAF3B      MOV EAX,0x3BAF2A2C
	$+B      02A5FDAC      8D80 34B09FC6      LEA EAX,DWORD PTR DS:[EAX+0xC69FB034]
	$+11     02A5FDB2      8B08               MOV ECX,DWORD PTR DS:[EAX]
	$+13     02A5FDB4      8B49 04            MOV ECX,DWORD PTR DS:[ECX+0x4]
	$+16     02A5FDB7      8B11               MOV EDX,DWORD PTR DS:[ECX]
	$+18     02A5FDB9      8B42 38            MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+1B     02A5FDBC      51                 PUSH ECX
	$+1C     02A5FDBD      E8 01000000        CALL MHOClien.02A5FDC3

	E8????????C7??????????8D??????????8B??8B????8B??8B??????E8

	$ ==>    02AC507A       E8 7678FFFF         CALL MHOClien.02ABC8F5                   ; 1
	$+5      02AC507F       C7C0 F6448F77       MOV EAX,0x778F44F6
	$+B      02AC5085       8D80 FA85C68A       LEA EAX,DWORD PTR DS:[EAX+0x8AC685FA]
	$+11     02AC508B       8B08                MOV ECX,DWORD PTR DS:[EAX]
	$+13     02AC508D       8B49 04             MOV ECX,DWORD PTR DS:[ECX+0x4]
	$+16     02AC5090       8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+18     02AC5092       8B42 38             MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+1B     02AC5095       FFD0                CALL EAX
	$+1D     02AC5097       8B4B 48             MOV ECX,DWORD PTR DS:[EBX+0x48]
	$+20     02AC509A       8B10                MOV EDX,DWORD PTR DS:[EAX]

	E8????????C7??????????8D??????????8B??8B????8B??8B????FF??8B????8B

	$ ==>    >  8D95 E8FDFFFF   LEA EDX,DWORD PTR SS:[EBP-0x218]
	$+6      >  52              PUSH EDX
	$+7      >  8BCE            MOV ECX,ESI
	$+9      >  E8 40FEFFFF     CALL CryGame.0CFF8F80                    ; 调用攻击封包函数，以前被VMP了，现在没有了
	$+E      >  A1 F09EAC0D     MOV EAX,DWORD PTR DS:[0xDAC9EF0]
	$+13     >  8B48 04         MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+16     >  8B11            MOV EDX,DWORD PTR DS:[ECX]
	$+18     >  8B42 38         MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+1B     >  FFD0            CALL EAX
	$+1D     >  8B4B 48         MOV ECX,DWORD PTR DS:[EBX+0x48]
	$+20     >  8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+22     >  8B92 44010000   MOV EDX,DWORD PTR DS:[EDX+0x144]
	$+28     >  51              PUSH ECX
	$+29     >  8BC8            MOV ECX,EAX
	$+2B     >  FFD2            CALL EDX

	8D????????????8B??E8????????A1????????8B????8B??8B
	--*/
	DWORD	dwMMOClientMonsterHitAddrHook2 = (DWORD)scan_game_.ScanAddr("8D????????????8B??E8????????A1????????8B????8B??8B", 9);
	assert(dwMMOClientMonsterHitAddrHook2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_HOOK2  ";
	ofs << dwMMOClientMonsterHitAddrHook2 - GetModuleCryGameBase();
	ofs << "  // ";
	ofs << dwMMOClientMonsterHitAddrHook2;
	ofs << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientMonsterHitAddrCall2 = (DWORD)scan_game_.ScanCall("8D????????????8B??E8????????A1????????8B????8B??8B", 0x0A);
	assert(dwMMOClientMonsterHitAddrCall2);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_CALL2  " << dwMMOClientMonsterHitAddrCall2 - GetModuleCryGameBase() << "  // " << dwMMOClientMonsterHitAddrCall2 << " CryGame.dll" << FC_OFstream_EndlLog;



	/*++
	$-6      >  55              PUSH EBP
	$-5      >  8BEC            MOV EBP,ESP
	$-3      >  53              PUSH EBX
	$-2      >  56              PUSH ESI
	$-1      >  57              PUSH EDI
	$ ==>    >  68 E0000000     PUSH 0xE0
	$+5      >  8BD9            MOV EBX,ECX
	$+7      >  E8 FEB2F8FE     CALL MHOClien.004045E0
	$+C      >  8D78 08         LEA EDI,DWORD PTR DS:[EAX+0x8]
	$+F      >  83C4 04         ADD ESP,0x4
	$+12     >  85FF            TEST EDI,EDI
	$+14     >  74 0A           JE SHORT MHOClien.014792F6
	$+16     >  8B75 08         MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+19     >  B9 36000000     MOV ECX,0x36
	$+1E     >  F3:A5           REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS>
	$+20     >  8B4B 04         MOV ECX,DWORD PTR DS:[EBX+0x4]
	$+23     >  8918            MOV DWORD PTR DS:[EAX],EBX
	$+25     >  5F              POP EDI
	$+26     >  8948 04         MOV DWORD PTR DS:[EAX+0x4],ECX
	$+29     >  8901            MOV DWORD PTR DS:[ECX],EAX
	$+2B     >  5E              POP ESI
	$+2C     >  8943 04         MOV DWORD PTR DS:[EBX+0x4],EAX
	$+2F     >  5B              POP EBX
	$+30     >  5D              POP EBP
	$+31     >  C2 0400         RETN 0x4


	68????????8B??E8????????8D????83????85

	// 先遣测试版本，这个函数相同的存在两份，选择第2个
	$-5      >  55              PUSH EBP
	$-4      >  8BEC            MOV EBP,ESP
	$-2      >  56              PUSH ESI
	$-1      >  57              PUSH EDI
	$ ==>    >  68 F8000000     PUSH 0xF8
	$+5      >  8BF9            MOV EDI,ECX
	$+7      >  E8 8F1AA1FF     CALL CryGame.0CA071A0
	$+C      >  8BF0            MOV ESI,EAX
	$+E      >  8D4E 08         LEA ECX,DWORD PTR DS:[ESI+0x8]
	$+11     >  83C4 04         ADD ESP,0x4
	$+14     >  85C9            TEST ECX,ECX
	$+16     >  74 09           JE SHORT CryGame.0CFF5726
	$+18     >  8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1B     >  50              PUSH EAX
	$+1C     >  E8 0AE5FFFF     CALL CryGame.0CFF3C30
	$+21     >  8B47 04         MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+24     >  893E            MOV DWORD PTR DS:[ESI],EDI
	$+26     >  8946 04         MOV DWORD PTR DS:[ESI+0x4],EAX
	$+29     >  8930            MOV DWORD PTR DS:[EAX],ESI
	$+2B     >  8977 04         MOV DWORD PTR DS:[EDI+0x4],ESI
	$+2E     >  5F              POP EDI
	$+2F     >  5E              POP ESI
	$+30     >  5D              POP EBP
	$+31     >  C2 0400         RETN 0x4

	68????????8B??E8????????8B??8D????83????85
	--*/

	DWORD	dwMMOClientHitParameterCallHook = (DWORD)scan_game_.ScanAddr("68????????8B??E8????????8B??8D????83????85", -5, 2);
	assert(dwMMOClientHitParameterCallHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_HIT_PARAMETER_CALL_HOOK  " << dwMMOClientHitParameterCallHook - GetModuleCryGameBase() << "  // " << dwMMOClientHitParameterCallHook << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-E      0CB485D0       55                 PUSH EBP
	$-D      0CB485D1       8BEC               MOV EBP,ESP
	$-B      0CB485D3       56                 PUSH ESI
	$-A      0CB485D4       57                 PUSH EDI
	$-9      0CB485D5       8B7D 08            MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-6      0CB485D8       8B47 10            MOV EAX,DWORD PTR DS:[EDI+0x10]
	$-3      0CB485DB       8B70 50            MOV ESI,DWORD PTR DS:[EAX+0x50]
	$ ==>    0CB485DE       85F6               TEST ESI,ESI
	$+2      0CB485E0       74 3A              JE SHORT CryGame.0CB4861C
	$+4      0CB485E2       8B16               MOV EDX,DWORD PTR DS:[ESI]
	$+6      0CB485E4       8B82 10020000      MOV EAX,DWORD PTR DS:[EDX+0x210]
	$+C      0CB485EA       8BCE               MOV ECX,ESI
	$+E      0CB485EC       FFD0               CALL EAX
	$+10     0CB485EE       84C0               TEST AL,AL
	$+12     0CB485F0       74 2A              JE SHORT CryGame.0CB4861C
	$+14     0CB485F2       6A 00              PUSH 0x0
	$+16     0CB485F4       56                 PUSH ESI
	$+17     0CB485F5       E8 D615FFFF        CALL CryGame.0CB39BD0
	$+1C     0CB485FA       83C4 08            ADD ESP,0x8
	$+1F     0CB485FD       83F8 03            CMP EAX,0x3
	$+22     0CB48600       75 1A              JNZ SHORT CryGame.0CB4861C                              ; 修改为jmp，即可在空中使用技能攻击
	$+24     0CB48602       6A 00              PUSH 0x0
	$+26     0CB48604       6A 00              PUSH 0x0
	$+28     0CB48606       6A 01              PUSH 0x1
	$+2A     0CB48608       51                 PUSH ECX
	$+2B     0CB48609       8BCC               MOV ECX,ESP
	$+2D     0CB4860B       68 788E9B0D        PUSH CryGame.0D9B8E78                                   ; ASCII "NORMAL_FALL"
	$+32     0CB48610       E8 0B5BB5FF        CALL CryGame.0C69E120
	$+37     0CB48615       8BCF               MOV ECX,EDI
	$+39     0CB48617       E8 C95B1301        CALL CryGame.0DC7E1E5                                   ; 当坐标被修改时，这里到达
	$+3E     0CB4861C       5F                 POP EDI
	$+3F     0CB4861D       5E                 POP ESI
	$+40     0CB4861E       5D                 POP EBP
	$+41     0CB4861F       C3                 RETN

	85??74??8B??8B??????????8B??FF??84??74??6A????E8
	--*/

	DWORD	dwMMOClientKongZhongUseSkillJmpAddr = (DWORD)scan_game_.ScanAddr("85??74??8B??8B??????????8B??FF??84??74??6A????E8", 0x22);
	assert(dwMMOClientKongZhongUseSkillJmpAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_KONG_ZHONG_USE_SKILL_JMP_ADDR  " << dwMMOClientKongZhongUseSkillJmpAddr - GetModuleCryGameBase() << "  // " << dwMMOClientKongZhongUseSkillJmpAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-B      >  E8 4E5E1B00     CALL CryGame.09F8307F
	$-6      >  8BE5            MOV ESP,EBP
	$-4      >  5D              POP EBP
	$-3      >  C2 0400         RETN 0x4
	$ ==>    >  8B46 04         MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+3      >  53              PUSH EBX
	$+4      >  50              PUSH EAX
	$+5      >  E8 6F1A12FF     CALL CryGame.08EEECB0
	$+A      >  8BD8            MOV EBX,EAX
	$+C      >  83C4 04         ADD ESP,0x4
	$+F      >  899D 74FFFFFF   MOV DWORD PTR SS:[EBP-0x8C],EBX
	$+15     >  85DB            TEST EBX,EBX
	$+17     >  75 16           JNZ SHORT CryGame.09DCD266

	8B????????E8????????8B??83????89??????????85??75
	--*/
	DWORD	dwMMOClientAddBufferHook = (DWORD)scan_game_.ScanAddr("8B????????E8????????8B??83????89??????????85??75", 0);
	assert(dwMMOClientAddBufferHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ADD_BUFFER_HOOK  " << dwMMOClientAddBufferHook - GetModuleCryGameBase() << "  // " << dwMMOClientAddBufferHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	// 多开hook函数的特征码
	$-8      >  8BFF            MOV EDI,EDI
	$-6      >  55              PUSH EBP
	$-5      >  8BEC            MOV EBP,ESP
	$-3      >  56              PUSH ESI
	$-2      >  8BF0            MOV ESI,EAX
	$ ==>    >  EB 13           JMP SHORT MHOCli_1.1011784C
	$+2      >  8B4D 10         MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+5      >  8A45 08         MOV AL,BYTE PTR SS:[EBP+0x8]
	$+8      >  FF4D 0C         DEC DWORD PTR SS:[EBP+0xC]
	$+B      >  E8 B5FFFFFF     CALL MHOCli_1.101177FC
	$+10     >  833E FF         CMP DWORD PTR DS:[ESI],-0x1
	$+13     >  74 06           JE SHORT MHOCli_1.10117852
	$+15     >  837D 0C 00      CMP DWORD PTR SS:[EBP+0xC],0x0
	$+19     >^ 7F E7           JG SHORT MHOCli_1.10117839
	$+1B     >  5E              POP ESI
	$+1C     >  5D              POP EBP
	$+1D     >  C3              RETN

	EB??8B????8A????FF????E8????????83????74??83??????7F
	--*/
	DWORD	dwMMOClientGwlrBaseSprintfAddr = (DWORD)scan_mho_base_.ScanAddr("EB??8B????8A????FF????E8????????83????74??83??????7F", -0x08);
	assert(dwMMOClientGwlrBaseSprintfAddr);
	ofs << "#define UPDATES_JFZR_REVERSAL_GWLR_BASE_SPRINTF_ADDR  " << dwMMOClientGwlrBaseSprintfAddr - GetModuleMhoClientBase() << "  // " << dwMMOClientGwlrBaseSprintfAddr << " MHOClientBase.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    0D5C5E32        E8 F9F3FFFF     CALL CryGame.0D5C5230
	$+5      0D5C5E37        84C0            TEST AL,AL
	$+7      0D5C5E39        74 4D           JE SHORT CryGame.0D5C5E88
	$+9      0D5C5E3B        85DB            TEST EBX,EBX
	$+B      0D5C5E3D        74 13           JE SHORT CryGame.0D5C5E52
	$+D      0D5C5E3F        83FB 01         CMP EBX,0x1
	$+10     0D5C5E42        74 0E           JE SHORT CryGame.0D5C5E52
	$+12     0D5C5E44        83FB 02         CMP EBX,0x2
	$+15     0D5C5E47        74 09           JE SHORT CryGame.0D5C5E52
	$+17     0D5C5E49        8B57 38         MOV EDX,DWORD PTR DS:[EDI+0x38]
	$+1A     0D5C5E4C        807A 48 00      CMP BYTE PTR DS:[EDX+0x48],0x0
	$+1E     0D5C5E50        75 36           JNZ SHORT CryGame.0D5C5E88
	$+20     0D5C5E52        C645 08 00      MOV BYTE PTR SS:[EBP+0x8],0x0
	$+24     0D5C5E56        85DB            TEST EBX,EBX
	$+26     0D5C5E58        74 0A           JE SHORT CryGame.0D5C5E64
	$+28     0D5C5E5A        83FB 04         CMP EBX,0x4
	$+2B     0D5C5E5D        7E 09           JLE SHORT CryGame.0D5C5E68
	$+2D     0D5C5E5F        83FB 08         CMP EBX,0x8
	$+30     0D5C5E62        7F 04           JG SHORT CryGame.0D5C5E68
	$+32     0D5C5E64        C645 08 01      MOV BYTE PTR SS:[EBP+0x8],0x1
	$+36     0D5C5E68        8B47 08         MOV EAX,DWORD PTR DS:[EDI+0x8]
	$+39     0D5C5E6B        8B4D FC         MOV ECX,DWORD PTR SS:[EBP-0x4]
	$+3C     0D5C5E6E        8B49 28         MOV ECX,DWORD PTR DS:[ECX+0x28]          ; MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET1___
	$+3F     0D5C5E71        50              PUSH EAX
	$+40     0D5C5E72        E8 89551400     CALL CryGame.0D70B400

	E8????????84??74??85??74??83
	--*/

	DWORD	dwMMOClientRoleBuffStateOffset1 = *scan_game_.ScanImme<byte>("E8????????84??74??85??74??83", 0x3C + 2);
	assert(dwMMOClientRoleBuffStateOffset1 == 0x28);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET1  " << dwMMOClientRoleBuffStateOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	// 打开一个NPC，此NPC包含的任务的地址处+0x2C下断，即可到达
	$ ==>    0D6C1745        8B77 2C             MOV ESI,DWORD PTR DS:[EDI+0x2C]
	$+3      0D6C1748        8975 FC             MOV DWORD PTR SS:[EBP-0x4],ESI
	$+6      0D6C174B        85F6                TEST ESI,ESI
	$+8      0D6C174D        0F84 AA000000       JE CryGame.0D6C17FD
	$+E      0D6C1753        80BE 2C010000 00    CMP BYTE PTR DS:[ESI+0x12C],0x0
	$+15     0D6C175A        0F85 9D000000       JNZ CryGame.0D6C17FD
	$+1B     0D6C1760        8B13                MOV EDX,DWORD PTR DS:[EBX]
	$+1D     0D6C1762        8B45 08             MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+20     0D6C1765        8B52 4C             MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+23     0D6C1768        50                  PUSH EAX
	$+24     0D6C1769        8BCB                MOV ECX,EBX
	$+26     0D6C176B        FFD2                CALL EDX
	$+28     0D6C176D        84C0                TEST AL,AL
	$+2A     0D6C176F        74 18               JE SHORT CryGame.0D6C1789
	$+2C     0D6C1771        8B4D 08             MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+2F     0D6C1774        8B03                MOV EAX,DWORD PTR DS:[EBX]
	$+31     0D6C1776        8B50 50             MOV EDX,DWORD PTR DS:[EAX+0x50]
	$+34     0D6C1779        51                  PUSH ECX
	$+35     0D6C177A        8BCB                MOV ECX,EBX
	$+37     0D6C177C        FFD2                CALL EDX

	8B????89????85??0F??????????80????????????0F
	--*/

	DWORD	dwMMOClientTaskObjOffet1 = *scan_game_.ScanImme<BYTE>("8B????89????85??0F??????????80????????????0F", 2);
	assert(dwMMOClientTaskObjOffet1 == 0x2C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TASK_OBJ_OFFSET1  " << dwMMOClientTaskObjOffet1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D5D9106        E8 5592FCFF         CALL CryGame.0D5A2360
	$+5      0D5D910B        8B55 08             MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+8      0D5D910E        3B42 30             CMP EAX,DWORD PTR DS:[EDX+0x30]          ; MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1
	$+B      0D5D9111        75 4F               JNZ SHORT CryGame.0D5D9162               ; 这里在接受任务时访问
	$+D      0D5D9113        8B4D 14             MOV ECX,DWORD PTR SS:[EBP+0x14]
	$+10     0D5D9116        33C0                XOR EAX,EAX
	$+12     0D5D9118        C707 01000000       MOV DWORD PTR DS:[EDI],0x1
	$+18     0D5D911E        8947 04             MOV DWORD PTR DS:[EDI+0x4],EAX
	$+1B     0D5D9121        895F 08             MOV DWORD PTR DS:[EDI+0x8],EBX
	$+1E     0D5D9124        8947 0C             MOV DWORD PTR DS:[EDI+0xC],EAX
	$+21     0D5D9127        8947 10             MOV DWORD PTR DS:[EDI+0x10],EAX
	$+24     0D5D912A        8B06                MOV EAX,DWORD PTR DS:[ESI]
	$+26     0D5D912C        8B50 08             MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+29     0D5D912F        51                  PUSH ECX
	$+2A     0D5D9130        53                  PUSH EBX
	$+2B     0D5D9131        8BCE                MOV ECX,ESI
	$+2D     0D5D9133        FFD2                CALL EDX

	E8????????8B????3B????75??8B????33??C7
	--*/

	DWORD	dwMMOClientTaskReceviedNpcNameOffset1 = *scan_game_.ScanImme<BYTE>("E8????????8B????3B????75??8B????33??C7", 0x8 + 2);
	assert(dwMMOClientTaskReceviedNpcNameOffset1 == 0x30);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1  " << dwMMOClientTaskReceviedNpcNameOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0C8F9E0B        E8 6044D8FF         CALL CryGame.0C67E270
	$+5      0C8F9E10        8B17                MOV EDX,DWORD PTR DS:[EDI]
	$+7      0C8F9E12        8B82 C4000000       MOV EAX,DWORD PTR DS:[EDX+0xC4]
	$+D      0C8F9E18        83C4 0C             ADD ESP,0xC
	$+10     0C8F9E1B        8BCF                MOV ECX,EDI
	$+12     0C8F9E1D        FFD0                CALL EAX
	$+14     0C8F9E1F        8D8C24 DC010000     LEA ECX,DWORD PTR SS:[ESP+0x1DC]
	$+1B     0C8F9E26        51                  PUSH ECX
	$+1C     0C8F9E27        8D48 1C             LEA ECX,DWORD PTR DS:[EAX+0x1C]          ; MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET2___
	$+1F     0C8F9E2A        E8 71B1A7FF         CALL CryGame.0C374FA0
	$+24     0C8F9E2F        8B40 14             MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+27     0C8F9E32        8B0D C0D8E10D       MOV ECX,DWORD PTR DS:[0xDE1D8C0]
	$+2D     0C8F9E38        8D5424 28           LEA EDX,DWORD PTR SS:[ESP+0x28]
	$+31     0C8F9E3C        52                  PUSH EDX
	$+32     0C8F9E3D        BE 6CE2AD0D         MOV ESI,CryGame.0DADE26C                 ; ASCII "name"
	$+37     0C8F9E42        E8 0945D8FF         CALL CryGame.0C67E350
	$+3C     0C8F9E47        83C4 04             ADD ESP,0x4
	$+3F     0C8F9E4A        8D8C24 DC010000     LEA ECX,DWORD PTR SS:[ESP+0x1DC]
	$+46     0C8F9E51        E8 0A74A7FF         CALL CryGame.0C371260

	E8????????8B??8B??????????83????8B??FF??8D??????????????8D
	--*/
	DWORD	dwMMOClientTaskReceviedNpcNameOffset2 = *scan_game_.ScanImme<BYTE>("E8????????8B??8B??????????83????8B??FF??8D??????????????8D", 0x1C + 2);
	assert(dwMMOClientTaskReceviedNpcNameOffset2 == 0x1C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET2  " << dwMMOClientTaskReceviedNpcNameOffset2 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D035F8B        E8 903A5900         CALL CryGame.0D5C9A20
	$+5      0D035F90        85C0                TEST EAX,EAX
	$+7      0D035F92        75 0B               JNZ SHORT CryGame.0D035F9F
	$+9      0D035F94        8B8F 70430000       MOV ECX,DWORD PTR DS:[EDI+0x4370]
	$+F      0D035F9A        E8 91530700         CALL CryGame.0D0AB330
	$+14     0D035F9F        8B86 FDA40300       MOV EAX,DWORD PTR DS:[ESI+0x3A4FD]
	$+1A     0D035FA5        50                  PUSH EAX
	$+1B     0D035FA6        8D8E 01A50300       LEA ECX,DWORD PTR DS:[ESI+0x3A501]
	$+21     0D035FAC        8D9F 303F0000       LEA EBX,DWORD PTR DS:[EDI+0x3F30]
	$+27     0D035FB2        51                  PUSH ECX
	$+28     0D035FB3        8BCB                MOV ECX,EBX
	$+2A     0D035FB5        E8 46A05A00         CALL CryGame.0D5E0000
	$+2F     0D035FBA        8B56 1C             MOV EDX,DWORD PTR DS:[ESI+0x1C]
	$+32     0D035FBD        52                  PUSH EDX
	$+33     0D035FBE        8BCB                MOV ECX,EBX
	$+35     0D035FC0        E8 BB8A5A00         CALL CryGame.0D5DEA80
	$+3A     0D035FC5        53                  PUSH EBX
	$+3B     0D035FC6        8D8F 28400000       LEA ECX,DWORD PTR DS:[EDI+0x4028]		// 庄园id偏移对象 需要+8才是庄园id
	$+41     0D035FCC        E8 0FF4FFFF         CALL CryGame.0D0353E0
	$+46     0D035FD1        8B46 14             MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+49     0D035FD4        8987 E0220000       MOV DWORD PTR DS:[EDI+0x22E0],EAX
	$+4F     0D035FDA        8B4E 18             MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+52     0D035FDD        898F E4220000       MOV DWORD PTR DS:[EDI+0x22E4],ECX

	E8????????85??75??8B??????????E8????????8B????????????8D

	//zhp
	$-49     >  51              PUSH ECX
	$-48     >  52              PUSH EDX
	$-47     >  8D86 40A40400   LEA EAX,DWORD PTR DS:[ESI+0x4A440]
	$-41     >  50              PUSH EAX
	$-40     >  8D8D F4EBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x140C]
	$-3A     >  C785 F0EBFFFF 0>MOV DWORD PTR SS:[EBP-0x1410],0x0
	$-30     >  E8 87AE48FF     CALL CryGame.0A594ED0
	$-2B     >  8B97 B84B0000   MOV EDX,DWORD PTR DS:[EDI+0x4BB8]
	$-25     >  8D8D F4EBFFFF   LEA ECX,DWORD PTR SS:[EBP-0x140C]
	$-1F     >  51              PUSH ECX
	$-1E     >  8B8A F0000000   MOV ECX,DWORD PTR DS:[EDX+0xF0]
	$-18     >  E8 AF906500     CALL CryGame.0B763110
	$-13     >  8B86 45230400   MOV EAX,DWORD PTR DS:[ESI+0x42345]
	$-D      >  50              PUSH EAX
	$-C      >  8D8E 49230400   LEA ECX,DWORD PTR DS:[ESI+0x42349]
	$-6      >  8D9F C0460000   LEA EBX,DWORD PTR DS:[EDI+0x46C0]
	$ ==>    >  51              PUSH ECX
	$+1      >  8BCB            MOV ECX,EBX
	$+3      >  E8 249F5E00     CALL CryGame.0B6F3FA0
	$+8      >  8B56 20         MOV EDX,DWORD PTR DS:[ESI+0x20]
	$+B      >  52              PUSH EDX
	$+C      >  8BCB            MOV ECX,EBX
	$+E      >  E8 F9835E00     CALL CryGame.0B6F2480
	$+13     >  53              PUSH EBX
	$+14     >  8D8F C8470000   LEA ECX,DWORD PTR DS:[EDI+0x47C8]
	$+1A     >  E8 ADF2FFFF     CALL CryGame.0B109340
	$+1F     >  8B46 14         MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+22     >  8987 F8230000   MOV DWORD PTR DS:[EDI+0x23F8],EAX
	$+28     >  8B4E 18         MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+2B     >  898F FC230000   MOV DWORD PTR DS:[EDI+0x23FC],ECX
	$+31     >  8B56 1C         MOV EDX,DWORD PTR DS:[ESI+0x1C]
	$+34     >  8997 F4230000   MOV DWORD PTR DS:[EDI+0x23F4],EDX
	$+3A     >  8B46 20         MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+3D     >  6A 00           PUSH 0x0
	$+3F     >  50              PUSH EAX
	$+40     >  E8 C7F70800     CALL CryGame.0B199880
	$+45     >  8B4D FC         MOV ECX,DWORD PTR SS:[EBP-0x4]

	518BCBE8????????8B????528BCBE8????????53

	//zhp 2016/5/17
	$-174    0BAB17C0      55                          PUSH EBP
	$-173    0BAB17C1      8BEC                        MOV EBP, ESP
	$-171    0BAB17C3      B8 3C140000                 MOV EAX, 143C
	$-16C    0BAB17C8      E8 03A07C00                 CALL CryGame.0C27B7D0
	$-167    0BAB17CD      A1 4066620C                 MOV EAX, DWORD PTR DS:[C626640]
	$-162    0BAB17D2      33C5                        XOR EAX, EBP
	$-160    0BAB17D4      8945 FC                     MOV DWORD PTR SS:[EBP-4], EAX
	$-15D    0BAB17D7      53                          PUSH EBX
	$-15C    0BAB17D8      56                          PUSH ESI
	$-15B    0BAB17D9      8B75 08                     MOV ESI, DWORD PTR SS:[EBP+8]
	$-158    0BAB17DC      8B46 20                     MOV EAX, DWORD PTR DS:[ESI+20]
	$-155    0BAB17DF      57                          PUSH EDI
	$-154    0BAB17E0      6A 00                       PUSH 0
	$-152    0BAB17E2      50                          PUSH EAX
	$-151    0BAB17E3      8BF9                        MOV EDI, ECX                             ; ntdll.778701BF
	$-14F    0BAB17E5      E8 B6F80900                 CALL CryGame.0BB510A0
	$-14A    0BAB17EA      83C4 08                     ADD ESP, 8
	$-147    0BAB17ED      56                          PUSH ESI
	$-146    0BAB17EE      8BCF                        MOV ECX, EDI
	$-144    0BAB17F0      E8 2B940400                 CALL CryGame.0BAFAC20
	$-13F    0BAB17F5      8B8F A0220000               MOV ECX, DWORD PTR DS:[EDI+22A0]
	$-139    0BAB17FB      85C9                        TEST ECX, ECX                            ; ntdll.778701BF
	$-137    0BAB17FD      74 13                       JE SHORT CryGame.0BAB1812
	$-135    0BAB17FF      8B96 DA540400               MOV EDX, DWORD PTR DS:[ESI+454DA]
	$-12F    0BAB1805      52                          PUSH EDX
	$-12E    0BAB1806      8D86 DE540400               LEA EAX, DWORD PTR DS:[ESI+454DE]
	$-128    0BAB180C      50                          PUSH EAX
	$-127    0BAB180D      E8 CEA86700                 CALL CryGame.0C12C0E0
	$-122    0BAB1812      8B8F 604E0000               MOV ECX, DWORD PTR DS:[EDI+4E60]
	$-11C    0BAB1818      85C9                        TEST ECX, ECX                            ; ntdll.778701BF
	$-11A    0BAB181A      74 0C                       JE SHORT CryGame.0BAB1828
	$-118    0BAB181C      8D96 E4E00400               LEA EDX, DWORD PTR DS:[ESI+4E0E4]
	$-112    0BAB1822      52                          PUSH EDX
	$-111    0BAB1823      E8 788F0700                 CALL CryGame.0BB2A7A0
	$-10C    0BAB1828      0FB74E 7F                   MOVZX ECX, WORD PTR DS:[ESI+7F]
	$-108    0BAB182C      0FB786 81000000             MOVZX EAX, WORD PTR DS:[ESI+81]
	$-101    0BAB1833      0FB796 83000000             MOVZX EDX, WORD PTR DS:[ESI+83]
	$-FA     0BAB183A      898D E0EBFFFF               MOV DWORD PTR SS:[EBP-1420], ECX         ; ntdll.778701BF
	$-F4     0BAB1840      8B8E 85000000               MOV ECX, DWORD PTR DS:[ESI+85]
	$-EE     0BAB1846      898D C8EBFFFF               MOV DWORD PTR SS:[EBP-1438], ECX         ; ntdll.778701BF
	$-E8     0BAB184C      8985 DCEBFFFF               MOV DWORD PTR SS:[EBP-1424], EAX
	$-E2     0BAB1852      8D8E 7F5B0000               LEA ECX, DWORD PTR DS:[ESI+5B7F]
	$-DC     0BAB1858      8D86 89000000               LEA EAX, DWORD PTR DS:[ESI+89]
	$-D6     0BAB185E      898D CCEBFFFF               MOV DWORD PTR SS:[EBP-1434], ECX         ; ntdll.778701BF
	$-D0     0BAB1864      0FB78E CB2D0300             MOVZX ECX, WORD PTR DS:[ESI+32DCB]
	$-C9     0BAB186B      8985 C4EBFFFF               MOV DWORD PTR SS:[EBP-143C], EAX
	$-C3     0BAB1871      0FB786 89500000             MOVZX EAX, WORD PTR DS:[ESI+5089]
	$-BC     0BAB1878      8995 E4EBFFFF               MOV DWORD PTR SS:[EBP-141C], EDX
	$-B6     0BAB187E      8D96 8B500000               LEA EDX, DWORD PTR DS:[ESI+508B]
	$-B0     0BAB1884      898D E8EBFFFF               MOV DWORD PTR SS:[EBP-1418], ECX         ; ntdll.778701BF
	$-AA     0BAB188A      8B8F 284E0000               MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-A4     0BAB1890      8995 D4EBFFFF               MOV DWORD PTR SS:[EBP-142C], EDX
	$-9E     0BAB1896      8B96 7B5B0000               MOV EDX, DWORD PTR DS:[ESI+5B7B]
	$-98     0BAB189C      8985 D8EBFFFF               MOV DWORD PTR SS:[EBP-1428], EAX
	$-92     0BAB18A2      8D86 CD2D0300               LEA EAX, DWORD PTR DS:[ESI+32DCD]
	$-8C     0BAB18A8      8995 D0EBFFFF               MOV DWORD PTR SS:[EBP-1430], EDX
	$-86     0BAB18AE      8985 ECEBFFFF               MOV DWORD PTR SS:[EBP-1414], EAX
	$-80     0BAB18B4      85C9                        TEST ECX, ECX                            ; ntdll.778701BF
	$-7E     0BAB18B6      74 69                       JE SHORT CryGame.0BAB1921
	$-7C     0BAB18B8      8D95 C4EBFFFF               LEA EDX, DWORD PTR SS:[EBP-143C]
	$-76     0BAB18BE      52                          PUSH EDX
	$-75     0BAB18BF      E8 6C0A5F00                 CALL CryGame.0C0A2330
	$-70     0BAB18C4      85C0                        TEST EAX, EAX
	$-6E     0BAB18C6      75 0B                       JNZ SHORT CryGame.0BAB18D3
	$-6C     0BAB18C8      8B8F 284E0000               MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-66     0BAB18CE      E8 0DB70800                 CALL CryGame.0BB3CFE0
	$-61     0BAB18D3      8D8D F4EBFFFF               LEA ECX, DWORD PTR SS:[EBP-140C]
	$-5B     0BAB18D9      E8 22E643FF                 CALL CryGame.0AEEFF00
	$-56     0BAB18DE      0FB78E DEA40400             MOVZX ECX, WORD PTR DS:[ESI+4A4DE]
	$-4F     0BAB18E5      8D85 F0EBFFFF               LEA EAX, DWORD PTR SS:[EBP-1410]
	$-49     0BAB18EB      50                          PUSH EAX
	$-48     0BAB18EC      51                          PUSH ECX                                 ; ntdll.778701BF
	$-47     0BAB18ED      8D96 E0A40400               LEA EDX, DWORD PTR DS:[ESI+4A4E0]
	$-41     0BAB18F3      52                          PUSH EDX
	$-40     0BAB18F4      8D8D F4EBFFFF               LEA ECX, DWORD PTR SS:[EBP-140C]
	$-3A     0BAB18FA      C785 F0EBFFFF 00000000      MOV DWORD PTR SS:[EBP-1410], 0
	$-30     0BAB1904      E8 D7E943FF                 CALL CryGame.0AEF02E0
	$-2B     0BAB1909      8B8F 284E0000               MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-25     0BAB190F      8B89 F0000000               MOV ECX, DWORD PTR DS:[ECX+F0]
	$-1F     0BAB1915      8D85 F4EBFFFF               LEA EAX, DWORD PTR SS:[EBP-140C]
	$-19     0BAB191B      50                          PUSH EAX
	$-18     0BAB191C      E8 5F9E6700                 CALL CryGame.0C12B780
	$-13     0BAB1921      8B96 E5230400               MOV EDX, DWORD PTR DS:[ESI+423E5]
	$-D      0BAB1927      52                          PUSH EDX
	$-C      0BAB1928      8D86 E9230400               LEA EAX, DWORD PTR DS:[ESI+423E9]
	$-6      0BAB192E      8D9F C8480000               LEA EBX, DWORD PTR DS:[EDI+48C8]
	$ ==>    0BAB1934      50                          PUSH EAX
	$+1      0BAB1935      8BCB                        MOV ECX, EBX
	$+3      0BAB1937      E8 347A6000                 CALL CryGame.0C0B9370
	$+8      0BAB193C      8B4E 20                     MOV ECX, DWORD PTR DS:[ESI+20]
	$+B      0BAB193F      51                          PUSH ECX                                 ; ntdll.778701BF
	$+C      0BAB1940      8BCB                        MOV ECX, EBX
	$+E      0BAB1942      E8 195F6000                 CALL CryGame.0C0B7860
	$+13     0BAB1947      53                          PUSH EBX
	$+14     0BAB1948      8D8F 004A0000               LEA ECX, DWORD PTR DS:[EDI+4A00]
	$+1A     0BAB194E      E8 9DFAFFFF                 CALL CryGame.0BAB13F0
	$+1F     0BAB1953      8B56 14                     MOV EDX, DWORD PTR DS:[ESI+14]
	$+22     0BAB1956      8997 80240000               MOV DWORD PTR DS:[EDI+2480], EDX
	$+28     0BAB195C      8B46 18                     MOV EAX, DWORD PTR DS:[ESI+18]           ; KERNELBA.75706FF0
	$+2B     0BAB195F      8987 84240000               MOV DWORD PTR DS:[EDI+2484], EAX
	$+31     0BAB1965      8B4E 1C                     MOV ECX, DWORD PTR DS:[ESI+1C]
	$+34     0BAB1968      898F 7C240000               MOV DWORD PTR DS:[EDI+247C], ECX         ; ntdll.778701BF
	$+3A     0BAB196E      8B4D FC                     MOV ECX, DWORD PTR SS:[EBP-4]
	$+3D     0BAB1971      5F                          POP EDI                                  ; ntdll.7787FD91
	$+3E     0BAB1972      5E                          POP ESI                                  ; ntdll.7787FD91
	$+3F     0BAB1973      33CD                        XOR ECX, EBP
	$+41     0BAB1975      5B                          POP EBX                                  ; ntdll.7787FD91
	$+42     0BAB1976      E8 AAA67C00                 CALL CryGame.0C27C025
	$+47     0BAB197B      8BE5                        MOV ESP, EBP
	$+49     0BAB197D      5D                          POP EBP                                  ; ntdll.7787FD91
	$+4A     0BAB197E      C2 0400                     RETN 4

	//2016/5/24 14:39
	$-191    0BBDC960      55                 PUSH EBP
	$-190    0BBDC961      8BEC               MOV EBP, ESP
	$-18E    0BBDC963      B8 3C140000        MOV EAX, 143C
	$-189    0BBDC968      E8 F3587D00        CALL CryGame.0C3B2260
	$-184    0BBDC96D      A1 50D6750C        MOV EAX, DWORD PTR DS:[C75D650]
	$-17F    0BBDC972      33C5               XOR EAX, EBP
	$-17D    0BBDC974      8945 FC            MOV DWORD PTR SS:[EBP-4], EAX
	$-17A    0BBDC977      53                 PUSH EBX
	$-179    0BBDC978      56                 PUSH ESI
	$-178    0BBDC979      8B75 08            MOV ESI, DWORD PTR SS:[EBP+8]
	$-175    0BBDC97C      8B46 20            MOV EAX, DWORD PTR DS:[ESI+20]
	$-172    0BBDC97F      57                 PUSH EDI
	$-171    0BBDC980      6A 00              PUSH 0
	$-16F    0BBDC982      50                 PUSH EAX
	$-16E    0BBDC983      8BF9               MOV EDI, ECX                             ; ntdll.77C80130
	$-16C    0BBDC985      E8 16FE0900        CALL CryGame.0BC7C7A0
	$-167    0BBDC98A      83C4 08            ADD ESP, 8
	$-164    0BBDC98D      56                 PUSH ESI
	$-163    0BBDC98E      8BCF               MOV ECX, EDI
	$-161    0BBDC990      E8 6B9C0400        CALL CryGame.0BC26600
	$-15C    0BBDC995      8B8F A0220000      MOV ECX, DWORD PTR DS:[EDI+22A0]
	$-156    0BBDC99B      85C9               TEST ECX, ECX                            ; ntdll.77C80130
	$-154    0BBDC99D      74 13              JE SHORT CryGame.0BBDC9B2
	$-152    0BBDC99F      8B96 DA540400      MOV EDX, DWORD PTR DS:[ESI+454DA]
	$-14C    0BBDC9A5      52                 PUSH EDX
	$-14B    0BBDC9A6      8D86 DE540400      LEA EAX, DWORD PTR DS:[ESI+454DE]
	$-145    0BBDC9AC      50                 PUSH EAX
	$-144    0BBDC9AD      E8 4E596800        CALL CryGame.0C262300
	$-13F    0BBDC9B2      8B8F 604E0000      MOV ECX, DWORD PTR DS:[EDI+4E60]
	$-139    0BBDC9B8      85C9               TEST ECX, ECX                            ; ntdll.77C80130
	$-137    0BBDC9BA      74 0C              JE SHORT CryGame.0BBDC9C8
	$-135    0BBDC9BC      8D96 E4E00400      LEA EDX, DWORD PTR DS:[ESI+4E0E4]
	$-12F    0BBDC9C2      52                 PUSH EDX
	$-12E    0BBDC9C3      E8 189D0700        CALL CryGame.0BC566E0
	$-129    0BBDC9C8      0FB74E 7F          MOVZX ECX, WORD PTR DS:[ESI+7F]
	$-125    0BBDC9CC      0FB786 81000000    MOVZX EAX, WORD PTR DS:[ESI+81]
	$-11E    0BBDC9D3      0FB796 83000000    MOVZX EDX, WORD PTR DS:[ESI+83]
	$-117    0BBDC9DA      898D E0EBFFFF      MOV DWORD PTR SS:[EBP-1420], ECX         ; ntdll.77C80130
	$-111    0BBDC9E0      8B8E 85000000      MOV ECX, DWORD PTR DS:[ESI+85]
	$-10B    0BBDC9E6      898D C8EBFFFF      MOV DWORD PTR SS:[EBP-1438], ECX         ; ntdll.77C80130
	$-105    0BBDC9EC      8985 DCEBFFFF      MOV DWORD PTR SS:[EBP-1424], EAX
	$-FF     0BBDC9F2      8D8E 7F5B0000      LEA ECX, DWORD PTR DS:[ESI+5B7F]
	$-F9     0BBDC9F8      8D86 89000000      LEA EAX, DWORD PTR DS:[ESI+89]
	$-F3     0BBDC9FE      898D CCEBFFFF      MOV DWORD PTR SS:[EBP-1434], ECX         ; ntdll.77C80130
	$-ED     0BBDCA04      0FB78E CB2D0300    MOVZX ECX, WORD PTR DS:[ESI+32DCB]
	$-E6     0BBDCA0B      8985 C4EBFFFF      MOV DWORD PTR SS:[EBP-143C], EAX
	$-E0     0BBDCA11      0FB786 89500000    MOVZX EAX, WORD PTR DS:[ESI+5089]
	$-D9     0BBDCA18      8995 E4EBFFFF      MOV DWORD PTR SS:[EBP-141C], EDX
	$-D3     0BBDCA1E      8D96 8B500000      LEA EDX, DWORD PTR DS:[ESI+508B]
	$-CD     0BBDCA24      898D E8EBFFFF      MOV DWORD PTR SS:[EBP-1418], ECX         ; ntdll.77C80130
	$-C7     0BBDCA2A      8B8F 284E0000      MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-C1     0BBDCA30      8995 D4EBFFFF      MOV DWORD PTR SS:[EBP-142C], EDX
	$-BB     0BBDCA36      8B96 7B5B0000      MOV EDX, DWORD PTR DS:[ESI+5B7B]
	$-B5     0BBDCA3C      8985 D8EBFFFF      MOV DWORD PTR SS:[EBP-1428], EAX
	$-AF     0BBDCA42      8D86 CD2D0300      LEA EAX, DWORD PTR DS:[ESI+32DCD]
	$-A9     0BBDCA48      8995 D0EBFFFF      MOV DWORD PTR SS:[EBP-1430], EDX
	$-A3     0BBDCA4E      8985 ECEBFFFF      MOV DWORD PTR SS:[EBP-1414], EAX
	$-9D     0BBDCA54      85C9               TEST ECX, ECX                            ; ntdll.77C80130
	$-9B     0BBDCA56      0F84 82000000      JE CryGame.0BBDCADE
	$-95     0BBDCA5C      8D95 C4EBFFFF      LEA EDX, DWORD PTR SS:[EBP-143C]
	$-8F     0BBDCA62      52                 PUSH EDX
	$-8E     0BBDCA63      E8 48A95F00        CALL CryGame.0C1D73B0
	$-89     0BBDCA68      85C0               TEST EAX, EAX
	$-87     0BBDCA6A      75 0B              JNZ SHORT CryGame.0BBDCA77
	$-85     0BBDCA6C      8B8F 284E0000      MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-7F     0BBDCA72      E8 F9C00800        CALL CryGame.0BC68B70
	$-7A     0BBDCA77      8B86 44E90400      MOV EAX, DWORD PTR DS:[ESI+4E944]
	$-74     0BBDCA7D      50                 PUSH EAX
	$-73     0BBDCA7E      8D8E 48E90400      LEA ECX, DWORD PTR DS:[ESI+4E948]
	$-6D     0BBDCA84      51                 PUSH ECX                                 ; ntdll.77C80130
	$-6C     0BBDCA85      8B8F 284E0000      MOV ECX, DWORD PTR DS:[EDI+4E28]
	$-66     0BBDCA8B      E8 30D75F00        CALL CryGame.0C1DA1C0
	$-61     0BBDCA90      8D8D F4EBFFFF      LEA ECX, DWORD PTR SS:[EBP-140C]
	$-5B     0BBDCA96      E8 457043FF        CALL CryGame.0B013AE0
	$-56     0BBDCA9B      0FB786 DEA40400    MOVZX EAX, WORD PTR DS:[ESI+4A4DE]
	$-4F     0BBDCAA2      8D95 F0EBFFFF      LEA EDX, DWORD PTR SS:[EBP-1410]
	$-49     0BBDCAA8      52                 PUSH EDX
	$-48     0BBDCAA9      50                 PUSH EAX
	$-47     0BBDCAAA      8D8E E0A40400      LEA ECX, DWORD PTR DS:[ESI+4A4E0]
	$-41     0BBDCAB0      51                 PUSH ECX                                 ; ntdll.77C80130
	$-40     0BBDCAB1      8D8D F4EBFFFF      LEA ECX, DWORD PTR SS:[EBP-140C]
	$-3A     0BBDCAB7      C785 F0EBFFFF 0000>MOV DWORD PTR SS:[EBP-1410], 0
	$-30     0BBDCAC1      E8 FA7343FF        CALL CryGame.0B013EC0
	$-2B     0BBDCAC6      8B87 284E0000      MOV EAX, DWORD PTR DS:[EDI+4E28]
	$-25     0BBDCACC      8B88 F0000000      MOV ECX, DWORD PTR DS:[EAX+F0]
	$-1F     0BBDCAD2      8D95 F4EBFFFF      LEA EDX, DWORD PTR SS:[EBP-140C]
	$-19     0BBDCAD8      52                 PUSH EDX
	$-18     0BBDCAD9      E8 C24E6800        CALL CryGame.0C2619A0
	$-13     0BBDCADE      8B8E E5230400      MOV ECX, DWORD PTR DS:[ESI+423E5]
	$-D      0BBDCAE4      51                 PUSH ECX                                 ; ntdll.77C80130
	$-C      0BBDCAE5      8D96 E9230400      LEA EDX, DWORD PTR DS:[ESI+423E9]
	$-6      0BBDCAEB      8D9F C8480000      LEA EBX, DWORD PTR DS:[EDI+48C8]
	$ ==>    0BBDCAF1      52                 PUSH EDX
	$+1      0BBDCAF2      8BCB               MOV ECX, EBX
	$+3      0BBDCAF4      E8 171E6100        CALL CryGame.0C1EE910
	$+8      0BBDCAF9      8B46 20            MOV EAX, DWORD PTR DS:[ESI+20]
	$+B      0BBDCAFC      50                 PUSH EAX
	$+C      0BBDCAFD      8BCB               MOV ECX, EBX
	$+E      0BBDCAFF      E8 FC026100        CALL CryGame.0C1ECE00
	$+13     0BBDCB04      53                 PUSH EBX
	$+14     0BBDCB05      8D8F 004A0000      LEA ECX, DWORD PTR DS:[EDI+4A00]
	$+1A     0BBDCB0B      E8 70FAFFFF        CALL CryGame.0BBDC580
	$+1F     0BBDCB10      8B4E 14            MOV ECX, DWORD PTR DS:[ESI+14]
	$+22     0BBDCB13      898F 80240000      MOV DWORD PTR DS:[EDI+2480], ECX         ; ntdll.77C80130
	$+28     0BBDCB19      8B56 18            MOV EDX, DWORD PTR DS:[ESI+18]           ; KERNELBA.75EF6FF0
	$+2B     0BBDCB1C      8B4D FC            MOV ECX, DWORD PTR SS:[EBP-4]
	$+2E     0BBDCB1F      8997 84240000      MOV DWORD PTR DS:[EDI+2484], EDX
	$+34     0BBDCB25      8B46 1C            MOV EAX, DWORD PTR DS:[ESI+1C]
	$+37     0BBDCB28      8987 7C240000      MOV DWORD PTR DS:[EDI+247C], EAX
	$+3D     0BBDCB2E      5F                 POP EDI                                  ; ntdll.77C8FD91
	$+3E     0BBDCB2F      5E                 POP ESI                                  ; ntdll.77C8FD91
	$+3F     0BBDCB30      33CD               XOR ECX, EBP
	$+41     0BBDCB32      5B                 POP EBX                                  ; ntdll.77C8FD91
	$+42     0BBDCB33      E8 7D5F7D00        CALL CryGame.0C3B2AB5
	$+47     0BBDCB38      8BE5               MOV ESP, EBP
	$+49     0BBDCB3A      5D                 POP EBP                                  ; ntdll.77C8FD91
	$+4A     0BBDCB3B      C2 0400            RETN 4
	--*/
	//528b??e8????????8b????508b??e8????????538d??????????e8????????8b????89
	DWORD	dwMMOClientActorZhuangYuanIdOffet = *scan_game_.ScanImme<DWORD>("528b??e8????????8b????508b??e8????????538d??????????e8????????8b????89", 0x14 + 2);
	//assert(dwMMOClientActorZhuangYuanIdOffet==0x47C8/*0x000042F0*//*(0x000042B0-8)*//*(0x000042A0-8)*//*0x4028*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ACTOR_ZHUANG_YUAN_ID_OFFSET  " << dwMMOClientActorZhuangYuanIdOffet + 8 << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-7      0D5D5070        8B41 0C             MOV EAX,DWORD PTR DS:[ECX+0xC]
	$-4      0D5D5073        85C0                TEST EAX,EAX
	$-2      0D5D5075        74 3B               JE SHORT CryGame.0D5D50B2
	$ ==>    0D5D5077        8B40 1C             MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$+3      0D5D507A        85C0                TEST EAX,EAX
	$+5      0D5D507C        74 34               JE SHORT CryGame.0D5D50B2
	$+7      0D5D507E        8B89 90010000       MOV ECX,DWORD PTR DS:[ECX+0x190]         ; MMOCLIENT_REVERSAL_COLLECT_OBJ_MONSTER_PACKET_ID_OFFSET
	$+D      0D5D5084        8B10                MOV EDX,DWORD PTR DS:[EAX]
	$+F      0D5D5086        56                  PUSH ESI
	$+10     0D5D5087        51                  PUSH ECX
	$+11     0D5D5088        8BC8                MOV ECX,EAX
	$+13     0D5D508A        8B42 74             MOV EAX,DWORD PTR DS:[EDX+0x74]
	$+16     0D5D508D        FFD0                CALL EAX
	$+18     0D5D508F        8BF0                MOV ESI,EAX
	$+1A     0D5D5091        85F6                TEST ESI,ESI
	$+1C     0D5D5093        74 19               JE SHORT CryGame.0D5D50AE
	$+1E     0D5D5095        8B16                MOV EDX,DWORD PTR DS:[ESI]
	$+20     0D5D5097        8B82 A8000000       MOV EAX,DWORD PTR DS:[EDX+0xA8]
	$+26     0D5D509D        8BCE                MOV ECX,ESI
	$+28     0D5D509F        FFD0                CALL EAX
	$+2A     0D5D50A1        83F8 01             CMP EAX,0x1
	$+2D     0D5D50A4        75 08               JNZ SHORT CryGame.0D5D50AE
	$+2F     0D5D50A6        8BCE                MOV ECX,ESI
	$+31     0D5D50A8        5E                  POP ESI
	$+32     0D5D50A9        E9 F2100300         JMP CryGame.0D6061A0
	$+37     0D5D50AE        33C0                XOR EAX,EAX
	$+39     0D5D50B0        5E                  POP ESI
	$+3A     0D5D50B1        C3                  RETN
	$+3B     0D5D50B2        33C0                XOR EAX,EAX
	$+3D     0D5D50B4        C3                  RETN

	//zhp
	$-7      0C6B0840     8B41 0C          mov eax, dword ptr ds:[ecx+0xC]
	$-4      0C6B0843     85C0             test eax, eax
	$-2      0C6B0845     74 3B            je short CryGame.0C6B0882
	$ ==>    0C6B0847     8B40 1C          mov eax, dword ptr ds:[eax+0x1C]
	$+3      0C6B084A     85C0             test eax, eax
	$+5      0C6B084C     74 34            je short CryGame.0C6B0882
	$+7      0C6B084E     8B89 90010000    mov ecx, dword ptr ds:[ecx+0x190]
	$+D      0C6B0854     8B10             mov edx, dword ptr ds:[eax]
	$+F      0C6B0856     56               push esi
	$+10     0C6B0857     51               push ecx
	$+11     0C6B0858     8BC8             mov ecx, eax
	$+13     0C6B085A     8B42 74          mov eax, dword ptr ds:[edx+0x74]
	$+16     0C6B085D     FFD0             call near eax
	$+18     0C6B085F     8BF0             mov esi, eax
	$+1A     0C6B0861     85F6             test esi, esi
	$+1C     0C6B0863     74 19            je short CryGame.0C6B087E
	$+1E     0C6B0865     8B16             mov edx, dword ptr ds:[esi]
	$+20     0C6B0867     8B82 A8000000    mov eax, dword ptr ds:[edx+0xA8]
	$+26     0C6B086D     8BCE             mov ecx, esi
	$+28     0C6B086F     FFD0             call near eax
	$+2A     0C6B0871     83F8 01          cmp eax, 0x1
	$+2D     0C6B0874     75 08            jnz short CryGame.0C6B087E
	$+2F     0C6B0876     8BCE             mov ecx, esi
	$+31     0C6B0878     5E               pop esi                                  ; ntdll_12.775AFD91
	$+32     0C6B0879     E9 72170300      jmp CryGame.0C6E1FF0
	$+37     0C6B087E     33C0             xor eax, eax
	$+39     0C6B0880     5E               pop esi                                  ; ntdll_12.775AFD91
	$+3A     0C6B0881     C3               retn
	$+3B     0C6B0882     33C0             xor eax, eax
	$+3D     0C6B0884     C3               retn

	8B????85??74??8B??????????8B??????8B??8B????FF
	--*/

	DWORD	dwMMOClientCollectObjMonsterPacketIdOffset = *scan_game_.ScanImme<DWORD>("8B????85??74??8B??????????8B??????8B??8B????FF", 7 + 2, 2);
	//assert(dwMMOClientCollectObjMonsterPacketIdOffset==0x190);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_COLLECT_OBJ_MONSTER_PACKET_ID_OFFSET  " << dwMMOClientCollectObjMonsterPacketIdOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    >  E8 AE110000           CALL CryGame.0C625F00
	$+5      >  53                    PUSH EBX
	$+6      >  E8 D8824000           CALL CryGame.0CA2D030
	$+B      >  53                    PUSH EBX
	$+C      >  8BF8                  MOV EDI,EAX
	$+E      >  6A 00                 PUSH 0x0
	$+10     >  57                    PUSH EDI
	$+11     >  E8 67E41501           CALL <JMP.&MSVCR90.memset>
	$+16     >  83C4 10               ADD ESP,0x10
	$+19     >  85DB                  TEST EBX,EBX
	$+1B     >  7E 45                 JLE SHORT CryGame.0C624DAF
	$+1D     >  8B5D 18               MOV EBX,DWORD PTR SS:[EBP+0x18]
	$+20     >  83C3 FC               ADD EBX,-0x4
	$+23     >  8B73 04               MOV ESI,DWORD PTR DS:[EBX+0x4]
	$+26     >  83C3 04               ADD EBX,0x4
	$+29     >  85F6                  TEST ESI,ESI
	$+2B     >  74 31                 JE SHORT CryGame.0C624DAB
	$+2D     >  E8 21030000           CALL CryGame.0C6250A0
	$+32     >  84C0                  TEST AL,AL
	$+34     >  74 28                 JE SHORT CryGame.0C624DAB
	$+36     >  8B55 F8               MOV EDX,DWORD PTR SS:[EBP-0x8]
	$+39     >  56                    PUSH ESI
	$+3A     >  E8 54030000           CALL CryGame.0C6250E0
	$+3F     >  83C4 04               ADD ESP,0x4
	$+42     >  83F8 FF               CMP EAX,-0x1
	$+45     >  74 17                 JE SHORT CryGame.0C624DAB
	$+47     >  8B55 E8               MOV EDX,DWORD PTR SS:[EBP-0x18]
	$+4A     >  0FB74D 14             MOVZX ECX,WORD PTR SS:[EBP+0x14]
	$+4E     >  893482                MOV DWORD PTR DS:[EDX+EAX*4],ESI
	$+51     >  8B45 F4               MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+54     >  40                    INC EAX
	$+55     >  3BC1                  CMP EAX,ECX
	$+57     >  8945 F4               MOV DWORD PTR SS:[EBP-0xC],EAX
	$+5A     >^ 7C C7                 JL SHORT CryGame.0C624D70
	$+5C     >  EB 04                 JMP SHORT CryGame.0C624DAF
	$+5E     >  C645 FF 00            MOV BYTE PTR SS:[EBP-0x1],0x0
	$+62     >  57                    PUSH EDI
	$+63     >  E8 BB824000           CALL CryGame.0CA2D070
	$+68     >  83C4 04               ADD ESP,0x4
	$+6B     >  807D FF 00            CMP BYTE PTR SS:[EBP-0x1],0x0
	$+6F     >  74 30                 JE SHORT CryGame.0C624DEE
	$+71     >  8B45 F8               MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+74     >  8B48 14               MOV ECX,DWORD PTR DS:[EAX+0x14]
	$+77     >  85C9                  TEST ECX,ECX
	$+79     >  75 05                 JNZ SHORT CryGame.0C624DCD
	$+7B     >  B9 5E21AC0D           MOV ECX,CryGame.0DAC215E
	$+80     >  0FB745 14             MOVZX EAX,WORD PTR SS:[EBP+0x14]
	$+84     >  8B0485 50BEB20D       MOV EAX,DWORD PTR DS:[EAX*4+0xDB2BE50]
	$+8B     >  8D55 E8               LEA EDX,DWORD PTR SS:[EBP-0x18]
	$+8E     >  52                    PUSH EDX
	$+8F     >  8B55 0C               MOV EDX,DWORD PTR SS:[EBP+0xC]
	$+92     >  51                    PUSH ECX
	$+93     >  8B4D 10               MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+96     >  51                    PUSH ECX
	$+97     >  52                    PUSH EDX
	$+98     >  FFD0                  CALL EAX
	$+9A     >  83C4 10               ADD ESP,0x10
	$+9D     >  84C0                  TEST AL,AL
	$+9F     >  75 07                 JNZ SHORT CryGame.0C624DF5
	$+A1     >  C745 F8 00000000      MOV DWORD PTR SS:[EBP-0x8],0x0
	$+A8     >  8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+AB     >  85C0                  TEST EAX,EAX
	$+AD     >  74 09                 JE SHORT CryGame.0C624E05
	$+AF     >  50                    PUSH EAX
	$+B0     >  E8 3E824000           CALL CryGame.0CA2D040
	$+B5     >  83C4 04               ADD ESP,0x4
	$+B8     >  8B45 F8               MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+BB     >  5F                    POP EDI
	$+BC     >  5E                    POP ESI
	$+BD     >  5B                    POP EBX                                  ; hook
	$+BE     >  8BE5                  MOV ESP,EBP
	$+C0     >  5D                    POP EBP
	$+C1     >  C3                    RETN

	E8??????????E8??????????8B??6A????E8????????83????85??7E
	--*/

	DWORD	dwMMOClientCountDownHook = (DWORD)scan_game_.ScanAddr("E8??????????E8??????????8B??6A????E8????????83????85??7E", 0x0BD);
	assert(dwMMOClientCountDownHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_COUNT_DOWN_HOOK  " << dwMMOClientCountDownHook - GetModuleCryGameBase() << "  // " << dwMMOClientCountDownHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$-5      >  BA 8486AF0D            MOV EDX,CryGame.0DAF8684                 ; ASCII "allCreditMax"
	$ ==>    >  E8 92B7CEFF            CALL CryGame.0C67E270
	$+5      >  83C4 18                ADD ESP,0x18
	$+8      >  8BB6 FC200000          MOV ESI,DWORD PTR DS:[ESI+0x20FC]		; MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET1
	$+E      >  85F6                   TEST ESI,ESI
	$+10     >  74 46                  JE SHORT CryGame.0C992B31
	$+12     >  8BCE                   MOV ECX,ESI
	$+14     >  C74424 0C 02000000     MOV DWORD PTR SS:[ESP+0xC],0x2
	$+1C     >  E8 C68BCE00            CALL CryGame.0D67B6C0                    ; [[[[[0x01929EA0+0C370000]+0x000000CC]+0x1C]+0x214]+0x20FC]+0x0B4
	$+21     >  8BF0                   MOV ESI,EAX
	$+23     >  8D5424 0C              LEA EDX,DWORD PTR SS:[ESP+0xC]
	$+27     >  52                     PUSH EDX                                 ; 2
	$+28     >  8BCE                   MOV ECX,ESI
	$+2A     >  E8 A8210000            CALL CryGame.0C994CB0                    ; 查表 返回已获取狩猎券数量的对象地址  MMOCLIENT_REVERSAL_WEEK_CREDIT_INFO_CALL
	$+2F     >  3BC6                   CMP EAX,ESI
	$+31     >  74 25                  JE SHORT CryGame.0C992B31
	$+33     >  8B40 14                MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+36     >  85C0                   TEST EAX,EAX
	$+38     >  74 1E                  JE SHORT CryGame.0C992B31
	$+3A     >  8BC8                   MOV ECX,EAX
	$+3C     >  E8 C602D100            CALL CryGame.0D6A2DE0                    ; 调用此CALL 返回剩余可领取的周狩猎券数量	MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_CALL
	$+41     >  8B4D 0C                MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+44     >  50                     PUSH EAX
	$+45     >  8D4424 14              LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+49     >  50                     PUSH EAX
	$+4A     >  51                     PUSH ECX
	$+4B     >  BA 9486AF0D            MOV EDX,CryGame.0DAF8694                 ; ASCII "weekCreditCount"
	$+50     >  E8 42B7CEFF            CALL CryGame.0C67E270
	E8????????83????8B??????????85??74??8B??C7

	//zhp 2016/5/24 14:43
	$-129    0B4F1160      55                 PUSH EBP
	$-128    0B4F1161      8BEC               MOV EBP, ESP
	$-126    0B4F1163      83E4 F8            AND ESP, FFFFFFF8
	$-123    0B4F1166      83EC 1C            SUB ESP, 1C
	$-120    0B4F1169      53                 PUSH EBX
	$-11F    0B4F116A      56                 PUSH ESI
	$-11E    0B4F116B      57                 PUSH EDI
	$-11D    0B4F116C      8B7D 08            MOV EDI, DWORD PTR SS:[EBP+8]
	$-11A    0B4F116F      85FF               TEST EDI, EDI
	$-118    0B4F1171      0F84 9B010000      JE CryGame.0B4F1312
	$-112    0B4F1177      837D 0C 00         CMP DWORD PTR SS:[EBP+C], 0
	$-10E    0B4F117B      0F84 91010000      JE CryGame.0B4F1312
	$-108    0B4F1181      837D 10 00         CMP DWORD PTR SS:[EBP+10], 0
	$-104    0B4F1185      0F84 87010000      JE CryGame.0B4F1312
	$-FE     0B4F118B      8B5D 14            MOV EBX, DWORD PTR SS:[EBP+14]           ; mhoclien.0040E2C5
	$-FB     0B4F118E      85DB               TEST EBX, EBX
	$-F9     0B4F1190      0F84 7C010000      JE CryGame.0B4F1312
	$-F3     0B4F1196      8B75 18            MOV ESI, DWORD PTR SS:[EBP+18]
	$-F0     0B4F1199      85F6               TEST ESI, ESI
	$-EE     0B4F119B      0F84 71010000      JE CryGame.0B4F1312
	$-E8     0B4F11A1      8B47 04            MOV EAX, DWORD PTR DS:[EDI+4]
	$-E5     0B4F11A4      C1E8 06            SHR EAX, 6
	$-E2     0B4F11A7      A8 01              TEST AL, 1
	$-E0     0B4F11A9      74 14              JE SHORT CryGame.0B4F11BF
	$-DE     0B4F11AB      8B0F               MOV ECX, DWORD PTR DS:[EDI]
	$-DC     0B4F11AD      8B47 08            MOV EAX, DWORD PTR DS:[EDI+8]
	$-D9     0B4F11B0      8B11               MOV EDX, DWORD PTR DS:[ECX]
	$-D7     0B4F11B2      8B52 08            MOV EDX, DWORD PTR DS:[EDX+8]
	$-D4     0B4F11B5      50                 PUSH EAX
	$-D3     0B4F11B6      57                 PUSH EDI
	$-D2     0B4F11B7      FFD2               CALL EDX
	$-D0     0B4F11B9      C707 00000000      MOV DWORD PTR DS:[EDI], 0
	$-CA     0B4F11BF      C747 04 02000000   MOV DWORD PTR DS:[EDI+4], 2
	$-C3     0B4F11C6      C647 08 00         MOV BYTE PTR DS:[EDI+8], 0
	$-BF     0B4F11CA      83FE 01            CMP ESI, 1
	$-BC     0B4F11CD      0F85 3F010000      JNZ CryGame.0B4F1312
	$-B6     0B4F11D3      8BC3               MOV EAX, EBX
	$-B4     0B4F11D5      8D7424 10          LEA ESI, DWORD PTR SS:[ESP+10]
	$-B0     0B4F11D9      E8 92E4C9FF        CALL CryGame.0B18F670
	$-AB     0B4F11DE      A1 7833900C        MOV EAX, DWORD PTR DS:[C903378]
	$-A6     0B4F11E3      8B88 D0000000      MOV ECX, DWORD PTR DS:[EAX+D0]
	$-A0     0B4F11E9      8B49 7C            MOV ECX, DWORD PTR DS:[ECX+7C]
	$-9D     0B4F11EC      8B11               MOV EDX, DWORD PTR DS:[ECX]
	$-9B     0B4F11EE      8B82 A8000000      MOV EAX, DWORD PTR DS:[EDX+A8]
	$-95     0B4F11F4      FFD0               CALL EAX
	$-93     0B4F11F6      8BF0               MOV ESI, EAX
	$-91     0B4F11F8      85F6               TEST ESI, ESI
	$-8F     0B4F11FA      0F84 09010000      JE CryGame.0B4F1309
	$-89     0B4F1200      8B9E 20230000      MOV EBX, DWORD PTR DS:[ESI+2320]         ; mhoclien.00730074
	$-83     0B4F1206      85DB               TEST EBX, EBX
	$-81     0B4F1208      74 3C              JE SHORT CryGame.0B4F1246
	$-7F     0B4F120A      8BCB               MOV ECX, EBX
	$-7D     0B4F120C      E8 BF9AD600        CALL CryGame.0C25ACD0
	$-78     0B4F1211      8B55 0C            MOV EDX, DWORD PTR SS:[EBP+C]
	$-75     0B4F1214      50                 PUSH EAX
	$-74     0B4F1215      8D4C24 14          LEA ECX, DWORD PTR SS:[ESP+14]
	$-70     0B4F1219      51                 PUSH ECX                                 ; ntdll.77C80130
	$-6F     0B4F121A      52                 PUSH EDX
	$-6E     0B4F121B      BA BC7E730C        MOV EDX, CryGame.0C737EBC
	$-69     0B4F1220      E8 3B5CC9FF        CALL CryGame.0B186E60
	$-64     0B4F1225      83C4 0C            ADD ESP, 0C
	$-61     0B4F1228      8BCB               MOV ECX, EBX
	$-5F     0B4F122A      E8 319BD600        CALL CryGame.0C25AD60
	$-5A     0B4F122F      8B4D 0C            MOV ECX, DWORD PTR SS:[EBP+C]
	$-57     0B4F1232      50                 PUSH EAX
	$-56     0B4F1233      8D4424 14          LEA EAX, DWORD PTR SS:[ESP+14]
	$-52     0B4F1237      50                 PUSH EAX
	$-51     0B4F1238      51                 PUSH ECX                                 ; ntdll.77C80130
	$-50     0B4F1239      BA D07E730C        MOV EDX, CryGame.0C737ED0
	$-4B     0B4F123E      E8 1D5CC9FF        CALL CryGame.0B186E60
	$-46     0B4F1243      83C4 0C            ADD ESP, 0C
	$-43     0B4F1246      8D8E 38040000      LEA ECX, DWORD PTR DS:[ESI+438]
	$-3D     0B4F124C      E8 0F8CC800        CALL CryGame.0C179E60
	$-38     0B4F1251      50                 PUSH EAX
	$-37     0B4F1252      E8 E9EBFFFF        CALL CryGame.0B4EFE40
	$-32     0B4F1257      8BD8               MOV EBX, EAX
	$-30     0B4F1259      83C4 04            ADD ESP, 4
	$-2D     0B4F125C      85DB               TEST EBX, EBX
	$-2B     0B4F125E      74 31              JE SHORT CryGame.0B4F1291
	$-29     0B4F1260      8B53 1C            MOV EDX, DWORD PTR DS:[EBX+1C]
	$-26     0B4F1263      8B4D 0C            MOV ECX, DWORD PTR SS:[EBP+C]
	$-23     0B4F1266      52                 PUSH EDX
	$-22     0B4F1267      8D4424 14          LEA EAX, DWORD PTR SS:[ESP+14]
	$-1E     0B4F126B      50                 PUSH EAX
	$-1D     0B4F126C      51                 PUSH ECX                                 ; ntdll.77C80130
	$-1C     0B4F126D      BA E07E730C        MOV EDX, CryGame.0C737EE0
	$-17     0B4F1272      E8 E95BC9FF        CALL CryGame.0B186E60
	$-12     0B4F1277      8B53 14            MOV EDX, DWORD PTR DS:[EBX+14]
	$-F      0B4F127A      8B4D 0C            MOV ECX, DWORD PTR SS:[EBP+C]
	$-C      0B4F127D      52                 PUSH EDX
	$-B      0B4F127E      8D4424 20          LEA EAX, DWORD PTR SS:[ESP+20]
	$-7      0B4F1282      50                 PUSH EAX
	$-6      0B4F1283      51                 PUSH ECX                                 ; ntdll.77C80130
	$-5      0B4F1284      BA F07E730C        MOV EDX, CryGame.0C737EF0                ; ASCII "allCreditMax"
	$ ==>    0B4F1289      E8 D25BC9FF        CALL CryGame.0B186E60
	$+5      0B4F128E      83C4 18            ADD ESP, 18
	$+8      0B4F1291      8B8E 70220000      MOV ECX, DWORD PTR DS:[ESI+2270]         ; mhoclien.00540079
	$+E      0B4F1297      85C9               TEST ECX, ECX                            ; ntdll.77C80130
	$+10     0B4F1299      74 44              JE SHORT CryGame.0B4F12DF
	$+12     0B4F129B      C74424 0C 02000000 MOV DWORD PTR SS:[ESP+C], 2
	$+1A     0B4F12A3      E8 28BEDA00        CALL CryGame.0C29D0D0
	$+1F     0B4F12A8      8BF0               MOV ESI, EAX
	$+21     0B4F12AA      8D5424 0C          LEA EDX, DWORD PTR SS:[ESP+C]
	$+25     0B4F12AE      52                 PUSH EDX
	$+26     0B4F12AF      8BCE               MOV ECX, ESI
	$+28     0B4F12B1      E8 6A350000        CALL CryGame.0B4F4820
	$+2D     0B4F12B6      3BC6               CMP EAX, ESI
	$+2F     0B4F12B8      74 25              JE SHORT CryGame.0B4F12DF
	$+31     0B4F12BA      8B40 14            MOV EAX, DWORD PTR DS:[EAX+14]
	$+34     0B4F12BD      85C0               TEST EAX, EAX
	$+36     0B4F12BF      74 1E              JE SHORT CryGame.0B4F12DF
	$+38     0B4F12C1      8BC8               MOV ECX, EAX
	$+3A     0B4F12C3      E8 1877DD00        CALL CryGame.0C2C89E0
	$+3F     0B4F12C8      8B4D 0C            MOV ECX, DWORD PTR SS:[EBP+C]
	$+42     0B4F12CB      50                 PUSH EAX
	$+43     0B4F12CC      8D4424 14          LEA EAX, DWORD PTR SS:[ESP+14]
	$+47     0B4F12D0      50                 PUSH EAX
	$+48     0B4F12D1      51                 PUSH ECX                                 ; ntdll.77C80130
	$+49     0B4F12D2      BA 007F730C        MOV EDX, CryGame.0C737F00
	$+4E     0B4F12D7      E8 845BC9FF        CALL CryGame.0B186E60
	$+53     0B4F12DC      83C4 0C            ADD ESP, 0C
	$+56     0B4F12DF      8B57 04            MOV EDX, DWORD PTR DS:[EDI+4]
	$+59     0B4F12E2      C1EA 06            SHR EDX, 6
	$+5C     0B4F12E5      F6C2 01            TEST DL, 1
	$+5F     0B4F12E8      74 14              JE SHORT CryGame.0B4F12FE
	$+61     0B4F12EA      8B0F               MOV ECX, DWORD PTR DS:[EDI]
	$+63     0B4F12EC      8B57 08            MOV EDX, DWORD PTR DS:[EDI+8]
	$+66     0B4F12EF      8B01               MOV EAX, DWORD PTR DS:[ECX]
	$+68     0B4F12F1      8B40 08            MOV EAX, DWORD PTR DS:[EAX+8]
	$+6B     0B4F12F4      52                 PUSH EDX
	$+6C     0B4F12F5      57                 PUSH EDI
	$+6D     0B4F12F6      FFD0               CALL EAX
	$+6F     0B4F12F8      C707 00000000      MOV DWORD PTR DS:[EDI], 0
	$+75     0B4F12FE      C747 04 02000000   MOV DWORD PTR DS:[EDI+4], 2
	$+7C     0B4F1305      C647 08 01         MOV BYTE PTR DS:[EDI+8], 1
	$+80     0B4F1309      8D4C24 10          LEA ECX, DWORD PTR SS:[ESP+10]
	$+84     0B4F130D      E8 8EB4C9FF        CALL CryGame.0B18C7A0
	$+89     0B4F1312      5F                 POP EDI                                  ; ntdll.77C8FD91
	$+8A     0B4F1313      5E                 POP ESI                                  ; ntdll.77C8FD91
	$+8B     0B4F1314      5B                 POP EBX                                  ; ntdll.77C8FD91
	$+8C     0B4F1315      8BE5               MOV ESP, EBP
	$+8E     0B4F1317      5D                 POP EBP                                  ; ntdll.77C8FD91
	$+8F     0B4F1318      C3                 RETN
	--*/
	//e8????????83????8b??????????85??74??c7??????????????e8????????8b??8d

	DWORD	dwMMOClientWeekCreditInfoCall = (DWORD)scan_game_.ScanCall("e8????????83????8b??????????85??74??c7??????????????e8????????8b??8d", 0x28 + 1);
	assert(dwMMOClientWeekCreditInfoCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_INFO_CALL  " << dwMMOClientWeekCreditInfoCall - GetModuleCryGameBase() << "  // " << dwMMOClientWeekCreditInfoCall << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientWeekCreditCountCall = (DWORD)scan_game_.ScanCall("e8????????83????8b??????????85??74??c7??????????????e8????????8b??8d", 0x3a + 1);
	assert(dwMMOClientWeekCreditCountCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_CALL  ";
	ofs << dwMMOClientWeekCreditCountCall - GetModuleCryGameBase();
	ofs << "  // ";
	ofs << dwMMOClientWeekCreditCountCall;
	ofs << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientWeekCreditCountOffset1 = *scan_game_.ScanImme<DWORD>("e8????????83????8b??????????85??74??c7??????????????e8????????8b??8d", 8 + 2);
	//assert(dwMMOClientWeekCreditCountOffset1==0x21F8/*0x000021A0*//*0x00002160*//*0x20FC*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET1  " << dwMMOClientWeekCreditCountOffset1 << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-6      >  55                   PUSH EBP
	$-5      >  8BEC                 MOV EBP,ESP
	$-3      >  56                   PUSH ESI
	$-2      >  8BF1                 MOV ESI,ECX
	$ ==>    >  8A86 40200000        MOV AL,BYTE PTR DS:[ESI+0x2040]
	$+6      >  57                   PUSH EDI
	$+7      >  84C0                 TEST AL,AL
	$+9      >  0F85 B2000000        JNZ CryGame.0CAF22A7                     ; jmp 不许游戏执行过图封包
	$+F      >  F605 A494C90D 01     TEST BYTE PTR DS:[0xDC994A4],0x1
	$+16     >  BF E8030000          MOV EDI,0x3E8
	$+1B     >  75 22                JNZ SHORT CryGame.0CAF2225
	$+1D     >  A1 007DC90D          MOV EAX,DWORD PTR DS:[0xDC97D00]
	$+22     >  8B48 2C              MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$+25     >  8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+27     >  8B42 54              MOV EAX,DWORD PTR DS:[EDX+0x54]
	$+2A     >  830D A494C90D 01     OR DWORD PTR DS:[0xDC994A4],0x1
	$+31     >  68 DCD2A00D          PUSH CryGame.0DA0D2DC                    ; ASCII "g_iRegionJumpFadeTime"
	$+36     >  FFD0                 CALL EAX
	$+38     >  A3 A094C90D          MOV DWORD PTR DS:[0xDC994A0],EAX
	$+3D     >  EB 05                JMP SHORT CryGame.0CAF222A
	$+3F     >  A1 A094C90D          MOV EAX,DWORD PTR DS:[0xDC994A0]
	$+44     >  85C0                 TEST EAX,EAX
	$+46     >  74 0B                JE SHORT CryGame.0CAF2239


	8A????????????84C00F85????????F6
	--*/
	DWORD	dwMMOClientGuoTuCodeHook = (DWORD)scan_game_.ScanAddr("8A????????????84C00F85????????F6", -6);
	assert(dwMMOClientGuoTuCodeHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GUOTU_CODE_HOOK  " << dwMMOClientGuoTuCodeHook - GetModuleCryGameBase() << "  // " << dwMMOClientGuoTuCodeHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0CCE8140        55                 PUSH EBP                                            ; retn 8 处理此处代码，可禁止城镇过图
	$+1      0CCE8141        8BEC               MOV EBP,ESP
	$+3      0CCE8143        A1 8097CE0D        MOV EAX,DWORD PTR DS:[0xDCE9780]
	$+8      0CCE8148        80B8 01020000 00   CMP BYTE PTR DS:[EAX+0x201],0x0
	$+F      0CCE814F        56                 PUSH ESI
	$+10     0CCE8150        0F85 C8000000      JNZ CryGame.0CCE821E
	$+16     0CCE8156        8B48 04            MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+19     0CCE8159        8B01               MOV EAX,DWORD PTR DS:[ECX]
	$+1B     0CCE815B        8B50 38            MOV EDX,DWORD PTR DS:[EAX+0x38]
	$+1E     0CCE815E        FFD2               CALL EDX
	$+20     0CCE8160        8B10               MOV EDX,DWORD PTR DS:[EAX]
	$+22     0CCE8162        8BC8               MOV ECX,EAX
	$+24     0CCE8164        8B82 24010000      MOV EAX,DWORD PTR DS:[EDX+0x124]
	$+2A     0CCE816A        FFD0               CALL EAX
	$+2C     0CCE816C        8BF0               MOV ESI,EAX
	$+2E     0CCE816E        85F6               TEST ESI,ESI
	$+30     0CCE8170        0F84 A8000000      JE CryGame.0CCE821E
	$+36     0CCE8176        8B16               MOV EDX,DWORD PTR DS:[ESI]
	$+38     0CCE8178        8B82 B4010000      MOV EAX,DWORD PTR DS:[EDX+0x1B4]
	$+3E     0CCE817E        8BCE               MOV ECX,ESI
	$+40     0CCE8180        FFD0               CALL EAX

	558BECA1????????80??????????????0F??????????8B????8B??8B
	--*/
	DWORD	dwMMOClientCityGuoTuCodeHook = (DWORD)scan_game_.ScanAddr("558BECA1????????80??????????????0F??????????8B????8B??8B", 0);
	assert(dwMMOClientCityGuoTuCodeHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CITY_GUOTU_CODE_HOOK  " << dwMMOClientCityGuoTuCodeHook - GetModuleCryGameBase() << "  // " << dwMMOClientCityGuoTuCodeHook << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D16DF15       8B46 20               MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+3      0D16DF18       83F8 08               CMP EAX,0x8
	$+6      0D16DF1B       74 05                 JE SHORT CryGame.0D16DF22
	$+8      0D16DF1D       83F8 17               CMP EAX,0x17
	$+B      0D16DF20       75 2E                 JNZ SHORT CryGame.0D16DF50
	$+D      0D16DF22       8B46 10               MOV EAX,DWORD PTR DS:[ESI+0x10]
	$+10     0D16DF25       8B55 08               MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+13     0D16DF28       8B8A 8C210000         MOV ECX,DWORD PTR DS:[EDX+0x218C]
	$+19     0D16DF2E       50                    PUSH EAX
	$+1A     0D16DF2F       E8 DC4C5400           CALL CryGame.0D6B2C10
	$+1F     0D16DF34       84C0                  TEST AL,AL
	$+21     0D16DF36       74 18                 JE SHORT CryGame.0D16DF50

	8B????83????74??83????75??8B????8B????8B????????????E8
	--*/

	DWORD	dwMMOClientIsInTaskFbCall = (DWORD)scan_game_.ScanCall("8B????83????74??83????75??8B????8B????8B????????????E8", 0x1A + 1);
	assert(dwMMOClientIsInTaskFbCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_IS_IN_TASK_FB_CALL  " << dwMMOClientIsInTaskFbCall - GetModuleCryGameBase() << "  // " << dwMMOClientIsInTaskFbCall << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientTaskFbOffset = *scan_game_.ScanImme<DWORD>("8B????83????74??83????75??8B????8B????8B????????????E8", 0x13 + 2);
	//assert(dwMMOClientTaskFbOffset==0x228C/*0x00002234*//*0x000021F4*//*0x218C*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TASK_FB_OFFSET  " << dwMMOClientTaskFbOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    >  E8 6BEFFFFF        CALL CryGame.0D16D0D0
	{
	$ ==>    >  55                 PUSH EBP
	$+1      >  8BEC               MOV EBP,ESP
	$+3      >  6A 00              PUSH 0x0
	$+5      >  68 9887AE0D        PUSH CryGame.0DAE8798                    ; ASCII "CHubEntryConfigInfo"
	$+A      >  6A 00              PUSH 0x0
	$+C      >  E8 9F0F73FF        CALL CryGame.0C89E080
	}
	$+5      >  33DB               XOR EBX,EBX
	$+7      >  83C4 04            ADD ESP,0x4
	$+A      >  3BC3               CMP EAX,EBX
	$+C      >  0F84 6C010000      JE CryGame.0D16E2DE
	$+12     >  395D 08            CMP DWORD PTR SS:[EBP+0x8],EBX
	$+15     >  0F84 63010000      JE CryGame.0D16E2DE
	$+1B     >  8B55 10            MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+1E     >  8D4D EC            LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+21     >  51                 PUSH ECX
	$+22     >  52                 PUSH EDX
	$+23     >  8BC8               MOV ECX,EAX
	$+25     >  895D EC            MOV DWORD PTR SS:[EBP-0x14],EBX
	$+28     >  895D F0            MOV DWORD PTR SS:[EBP-0x10],EBX
	$+2B     >  895D F4            MOV DWORD PTR SS:[EBP-0xC],EBX
	$+2E     >  E8 1D2A0900        CALL CryGame.0D200BB0
	$+33     >  8B7D F0            MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+36     >  8B75 EC            MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+39     >  8BCF               MOV ECX,EDI
	$+3B     >  2BCE               SUB ECX,ESI
	$+3D     >  B8 ABAAAA2A        MOV EAX,0x2AAAAAAB
	$+42     >  F7E9               IMUL ECX
	$+44     >  C1FA 04            SAR EDX,0x4
	$+47     >  8BC2               MOV EAX,EDX
	$+49     >  C1E8 1F            SHR EAX,0x1F
	$+4C     >  03C2               ADD EAX,EDX

	E8????????33??83????3B??0F??????????39????0F??????????8B????8D

	$-5      >  55                 PUSH EBP
	$-4      >  8BEC               MOV EBP,ESP
	$-2      >  6A 00              PUSH 0x0
	$ ==>    >  68 9887AE0D        PUSH CryGame.0DAE8798                    ; ASCII "CHubEntryConfigInfo"
	$+5      >  6A 00              PUSH 0x0
	$+7      >  E8 9F0F73FF        CALL CryGame.0C89E080


	68????????6A
	--*/
	DWORD	dwMMOClientZhuXianTaskFbInfoAddrCall = (DWORD)scan_game_.ScanCall("E8????????33??83????3B??0F??????????39????0F??????????8B????8D", 1);
	assert(dwMMOClientZhuXianTaskFbInfoAddrCall);


	try
	{
		scan_tmp_.SetCodeRange((char*)dwMMOClientZhuXianTaskFbInfoAddrCall, 0x50);
		DWORD dwCHubEntryConfigInfoAddr = *scan_tmp_.ScanImme<DWORD>("68????????6A", 1);
		assert(dwCHubEntryConfigInfoAddr);
		if (dwCHubEntryConfigInfoAddr != 0)
		{
			char* pszCHubEntryConfigInfo = (char*)dwCHubEntryConfigInfoAddr;

			if (strcmp(pszCHubEntryConfigInfo, "CHubEntryConfigInfo") != 0)
			{
				assert(false);
			}
		}

	}
	catch (...)
	{
		assert(false);
	}

	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ZHU_XIAN_TASK_FB_INFO_ADDR_CALL  " << dwMMOClientZhuXianTaskFbInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientZhuXianTaskFbInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D0EE760       E8 5BEDFFFF        CALL CryGame.0D0ED4C0
	$+5      0D0EE765       33DB               XOR EBX,EBX
	$+7      0D0EE767       83C4 04            ADD ESP,0x4
	$+A      0D0EE76A       3BC3               CMP EAX,EBX
	$+C      0D0EE76C       0F84 5B010000      JE CryGame.0D0EE8CD
	$+12     0D0EE772       395D 08            CMP DWORD PTR SS:[EBP+0x8],EBX
	$+15     0D0EE775       0F84 52010000      JE CryGame.0D0EE8CD
	$+1B     0D0EE77B       8B55 10            MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+1E     0D0EE77E       8D4D EC            LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+21     0D0EE781       51                 PUSH ECX
	$+22     0D0EE782       52                 PUSH EDX
	$+23     0D0EE783       8BC8               MOV ECX,EAX
	$+25     0D0EE785       895D EC            MOV DWORD PTR SS:[EBP-0x14],EBX
	$+28     0D0EE788       895D F0            MOV DWORD PTR SS:[EBP-0x10],EBX
	$+2B     0D0EE78B       895D F4            MOV DWORD PTR SS:[EBP-0xC],EBX
	$+2E     0D0EE78E       E8 7D5D1600        CALL CryGame.0D254510

	E8????????33??83????3B??0F??????????39????0F

	$-5      0D0ED4C0       55                 PUSH EBP
	$-4      0D0ED4C1       8BEC               MOV EBP,ESP
	$-2      0D0ED4C3       6A 00              PUSH 0x0
	$ ==>    0D0ED4C5       68 701EB30D        PUSH CryGame.0DB31E70                    ; ASCII "CHubEntryConfigKingQuestInfo"
	$+5      0D0ED4CA       6A 00              PUSH 0x0
	$+7      0D0ED4CC       E8 AFB47CFF        CALL CryGame.0C8B8980

	68????????6A
	--*/
	DWORD	dwMMOClientGetKingQuestInfoAddrCall = (DWORD)scan_game_.ScanCall("E8????????33??83????3B??0F??????????39????0F??????????8B????8D", 1, 2);
	assert(dwMMOClientGetKingQuestInfoAddrCall);


	try
	{
		scan_tmp_.SetCodeRange((char*)dwMMOClientGetKingQuestInfoAddrCall, 0x50);
		DWORD dwCHubEntryConfigKingQuestInfo = *scan_tmp_.ScanImme<DWORD>("68????????6A", 1);
		assert(dwCHubEntryConfigKingQuestInfo);
		if (dwCHubEntryConfigKingQuestInfo != 0)
		{
			char* pszCHubEntryConfigKingQuestInfo = (char*)dwCHubEntryConfigKingQuestInfo;

			if (strcmp(pszCHubEntryConfigKingQuestInfo, "CHubEntryConfigKingQuestInfo") != 0)
			{
				assert(false);
			}
		}

	}
	catch (...)
	{
		assert(false);
	}

	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_GET_KING_QUEST_INFO_ADDR_CALL  " << dwMMOClientGetKingQuestInfoAddrCall - GetModuleCryGameBase() << "  // " << dwMMOClientGetKingQuestInfoAddrCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    0D5FD824      FFD0              CALL EAX
	$+2      0D5FD826      85C0              TEST EAX,EAX
	$+4      0D5FD828      75 58             JNZ SHORT CryGame.0D5FD882
	$+6      0D5FD82A      8B4E 40           MOV ECX,DWORD PTR DS:[ESI+0x40]
	$+9      0D5FD82D      53                PUSH EBX
	$+A      0D5FD82E      57                PUSH EDI
	$+B      0D5FD82F      8B79 0C           MOV EDI,DWORD PTR DS:[ECX+0xC]
	$+E      0D5FD832      8B4E 3C           MOV ECX,DWORD PTR DS:[ESI+0x3C]                  ; MMOCLIENT_REVERSAL_ROLE_BUFF_INFO_ADDR_OFFSET___
	$+11     0D5FD835      6A 14             PUSH 0x14
	$+13     0D5FD837      E8 14A4BEFF       CALL CryGame.0D1E7C50
	{
	$ ==>    0D1E7C50      55                   PUSH EBP
	$+1      0D1E7C51      8BEC                 MOV EBP,ESP
	$+3      0D1E7C53      83EC 08              SUB ESP,0x8
	$+6      0D1E7C56      894D FC              MOV DWORD PTR SS:[EBP-0x4],ECX
	$+9      0D1E7C59      8B45 FC              MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+C      0D1E7C5C      8B48 4C              MOV ECX,DWORD PTR DS:[EAX+0x4C]
	$+F      0D1E7C5F      3B4D 08              CMP ECX,DWORD PTR SS:[EBP+0x8]
	$+12     0D1E7C62      74 14                JE SHORT CryGame.0D1E7C78
	$+14     0D1E7C64      8B55 FC              MOV EDX,DWORD PTR SS:[EBP-0x4]
	$+17     0D1E7C67      8B42 50              MOV EAX,DWORD PTR DS:[EDX+0x50]
	$+1A     0D1E7C6A      3B45 08              CMP EAX,DWORD PTR SS:[EBP+0x8]
	$+1D     0D1E7C6D      74 09                JE SHORT CryGame.0D1E7C78
	$+1F     0D1E7C6F      C745 F8 00000000     MOV DWORD PTR SS:[EBP-0x8],0x0
	$+26     0D1E7C76      EB 07                JMP SHORT CryGame.0D1E7C7F
	$+28     0D1E7C78      C745 F8 01000000     MOV DWORD PTR SS:[EBP-0x8],0x1
	$+2F     0D1E7C7F      8A45 F8              MOV AL,BYTE PTR SS:[EBP-0x8]
	$+32     0D1E7C82      8BE5                 MOV ESP,EBP
	$+34     0D1E7C84      5D                   POP EBP
	$+35     0D1E7C85      C2 0400              RETN 0x4

	}

	FF??85??75??8B????????8B????8B????6A??E8
	--*/

	DWORD	dwMMOClientRoleBuffInfoAddrOffset = *scan_game_.ScanImme<byte>("FF??85??75??8B????????8B????8B????6A??E8", 0x0E + 2);
	assert(dwMMOClientRoleBuffInfoAddrOffset == 0x3C);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_INFO_ADDR_OFFSET  " << dwMMOClientRoleBuffInfoAddrOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0D5FD7C6      8B46 20              MOV EAX,DWORD PTR DS:[ESI+0x20]                  ; MMOCLIENT_REVERSAL_ROLE_BUFF_FLAG_OFFSET___
	$+3      0D5FD7C9      83F8 FF              CMP EAX,-0x1
	$+6      0D5FD7CC      74 13                JE SHORT CryGame.0D5FD7E1
	$+8      0D5FD7CE      8B4E 40              MOV ECX,DWORD PTR DS:[ESI+0x40]
	$+B      0D5FD7D1      50                   PUSH EAX
	$+C      0D5FD7D2      83C1 18              ADD ECX,0x18
	$+F      0D5FD7D5      E8 8606DAFE          CALL CryGame.0C39DE60
	$+14     0D5FD7DA      C746 20 FFFFFFFF     MOV DWORD PTR DS:[ESI+0x20],-0x1
	$+1B     0D5FD7E1      8B4E 48              MOV ECX,DWORD PTR DS:[ESI+0x48]
	$+1E     0D5FD7E4      C646 39 01           MOV BYTE PTR DS:[ESI+0x39],0x1
	$+22     0D5FD7E8      E8 83EB1300          CALL CryGame.0D73C370

	8B????83????74??8B??????83????E8????????C7????????????8B????C6
	--*/

	DWORD	dwMMOClientRoleBuffFlagOffset = *scan_game_.ScanImme<BYTE>("8B????83????74??8B??????83????E8????????C7????????????8B????C6", 2);
	assert(dwMMOClientRoleBuffFlagOffset == 0x20);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_FLAG_OFFSET  " << dwMMOClientRoleBuffFlagOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	cpuid
	$-7      >  55              PUSH EBP
	$-6      >  8BEC            MOV EBP,ESP
	$-4      >  53              PUSH EBX
	$-3      >  8B45 08         MOV EAX,DWORD PTR SS:[EBP+0x8]
	$ ==>    >  0FA2            CPUID
	$+2      >  A3 9821AE06     MOV DWORD PTR DS:[0x6AE2198],EAX
	$+7      >  891D 9C21AE06   MOV DWORD PTR DS:[0x6AE219C],EBX
	$+D      >  890D A021AE06   MOV DWORD PTR DS:[0x6AE21A0],ECX
	$+13     >  8915 A421AE06   MOV DWORD PTR DS:[0x6AE21A4],EDX
	$+19     >  5B              POP EBX
	$+1A     >  5D              POP EBP
	$+1B     >  C2 0400         RETN 0x4

	0FA2A3
	--*/
	DWORD	dwMMOClientTersafeGetCpuIdHook = (DWORD)scan_ter_safe_.ScanAddr("0FA2A3????????89", -0x07);
	assert(dwMMOClientTersafeGetCpuIdHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TERSAFE_GET_CPUID_HOOK  " << dwMMOClientTersafeGetCpuIdHook - GetModuleTersafeBase() << "  // " << dwMMOClientTersafeGetCpuIdHook << " tersafe.dll" << FC_OFstream_EndlLog;

	/*++


	$ ==>    0CFA51BA      E8 D1FF3CFF               CALL CryGame.0C375190
	$+5      0CFA51BF      8BCE                      MOV ECX,ESI
	$+7      0CFA51C1      E8 7A222900               CALL CryGame.0D237440
	$+C      0CFA51C6      8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+E      0CFA51C8      3BC3                      CMP EAX,EBX
	$+10     0CFA51CA      75 05                     JNZ SHORT CryGame.0CFA51D1
	$+12     0CFA51CC      B8 AE5CB00D               MOV EAX,CryGame.0DB05CAE
	$+17     0CFA51D1      50                        PUSH EAX
	$+18     0CFA51D2      8D8D DCFEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x124]
	$+1E     0CFA51D8      E8 B3FF3CFF               CALL CryGame.0C375190
	$+23     0CFA51DD      8B56 1C                   MOV EDX,DWORD PTR DS:[ESI+0x1C]
	$+26     0CFA51E0      8995 D4FEFFFF             MOV DWORD PTR SS:[EBP-0x12C],EDX
	$+2C     0CFA51E6      8B46 2C                   MOV EAX,DWORD PTR DS:[ESI+0x2C]
	$+2F     0CFA51E9      8985 E0FEFFFF             MOV DWORD PTR SS:[EBP-0x120],EAX
	$+35     0CFA51EF      8A4E 50                   MOV CL,BYTE PTR DS:[ESI+0x50]
	$+38     0CFA51F2      888D E4FEFFFF             MOV BYTE PTR SS:[EBP-0x11C],CL
	$+3E     0CFA51F8      0FB696 84000000           MOVZX EDX,BYTE PTR DS:[ESI+0x84]
	$+45     0CFA51FF      8895 E6FEFFFF             MOV BYTE PTR SS:[EBP-0x11A],DL
	$+4B     0CFA5205      8B46 28                   MOV EAX,DWORD PTR DS:[ESI+0x28]
	$+4E     0CFA5208      8985 E8FEFFFF             MOV DWORD PTR SS:[EBP-0x118],EAX
	$+54     0CFA520E      8B8E A8000000             MOV ECX,DWORD PTR DS:[ESI+0xA8]
	$+5A     0CFA5214      898D ECFEFFFF             MOV DWORD PTR SS:[EBP-0x114],ECX
	$+60     0CFA521A      8B86 B0000000             MOV EAX,DWORD PTR DS:[ESI+0xB0]          ; MMOCLIENT_REVERSAL_ITME_USE_LEVEL_OFFSET___
	$+66     0CFA5220      8B96 B4000000             MOV EDX,DWORD PTR DS:[ESI+0xB4]
	$+6C     0CFA5226      8985 F0FEFFFF             MOV DWORD PTR SS:[EBP-0x110],EAX
	$+72     0CFA522C      8B86 B0000000             MOV EAX,DWORD PTR DS:[ESI+0xB0]
	$+78     0CFA5232      8985 B4FEFFFF             MOV DWORD PTR SS:[EBP-0x14C],EAX
	$+7E     0CFA5238      8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$+84     0CFA523E      8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+87     0CFA5241      8B8E E4000000             MOV ECX,DWORD PTR DS:[ESI+0xE4]

	//zhp
	$ ==>    >  E8 846837FF     CALL CryGame.0A3C4DC0
	$+5      >  8BCE            MOV ECX,ESI
	$+7      >  E8 0D8A2A00     CALL CryGame.0B2F6F50
	$+C      >  8B00            MOV EAX,DWORD PTR DS:[EAX]
	$+E      >  3BC3            CMP EAX,EBX
	$+10     >  75 05           JNZ SHORT CryGame.0B04E54E
	$+12     >  B8 4671BF0B     MOV EAX,CryGame.0BBF7146
	$+17     >  50              PUSH EAX
	$+18     >  8D8D C4FEFFFF   LEA ECX,DWORD PTR SS:[EBP-0x13C]
	$+1E     >  E8 666837FF     CALL CryGame.0A3C4DC0
	$+23     >  8B56 1C         MOV EDX,DWORD PTR DS:[ESI+0x1C]
	$+26     >  8995 BCFEFFFF   MOV DWORD PTR SS:[EBP-0x144],EDX
	$+2C     >  8B46 2C         MOV EAX,DWORD PTR DS:[ESI+0x2C]
	$+2F     >  8985 C8FEFFFF   MOV DWORD PTR SS:[EBP-0x138],EAX
	$+35     >  8A4E 50         MOV CL,BYTE PTR DS:[ESI+0x50]
	$+38     >  888D CCFEFFFF   MOV BYTE PTR SS:[EBP-0x134],CL
	$+3E     >  399E 84000000   CMP DWORD PTR DS:[ESI+0x84],EBX
	$+44     >  0F95C2          SETNE DL
	$+47     >  8895 CEFEFFFF   MOV BYTE PTR SS:[EBP-0x132],DL
	$+4D     >  8B46 28         MOV EAX,DWORD PTR DS:[ESI+0x28]
	$+50     >  8985 D0FEFFFF   MOV DWORD PTR SS:[EBP-0x130],EAX
	$+56     >  8B8E A8000000   MOV ECX,DWORD PTR DS:[ESI+0xA8]
	$+5C     >  898D D4FEFFFF   MOV DWORD PTR SS:[EBP-0x12C],ECX
	$+62     >  8B86 B0000000   MOV EAX,DWORD PTR DS:[ESI+0xB0]
	$+68     >  8B96 B4000000   MOV EDX,DWORD PTR DS:[ESI+0xB4]
	$+6E     >  8985 D8FEFFFF   MOV DWORD PTR SS:[EBP-0x128],EAX
	$+74     >  8B86 B0000000   MOV EAX,DWORD PTR DS:[ESI+0xB0]
	$+7A     >  8985 A0FEFFFF   MOV DWORD PTR SS:[EBP-0x160],EAX
	$+80     >  8B86 B4000000   MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$+86     >  8945 D8         MOV DWORD PTR SS:[EBP-0x28],EAX
	$+89     >  8B8E E4000000   MOV ECX,DWORD PTR DS:[ESI+0xE4]
	$+8F     >  898D DCFEFFFF   MOV DWORD PTR SS:[EBP-0x124],ECX
	$+95     >  8995 A4FEFFFF   MOV DWORD PTR SS:[EBP-0x15C],EDX
	$+9B     >  8B96 C0000000   MOV EDX,DWORD PTR DS:[ESI+0xC0]
	$+A1     >  8BCE            MOV ECX,ESI
	$+A3     >  8995 F0FEFFFF   MOV DWORD PTR SS:[EBP-0x110],EDX
	$+A9     >  E8 8B892A00     CALL CryGame.0B2F6F70

	E8????????8B??E8????????8B??3B??75??B8
	--*/
	DWORD	dwMMOClientItemUseLevelOffset = *scan_game_.ScanImme<DWORD>("E8????????8B??E8????????8B??3B??75??B8", 0x62 + 2);
	assert(dwMMOClientItemUseLevelOffset == 0xB0);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_ITME_USE_LEVEL_OFFSET  " << dwMMOClientItemUseLevelOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$ ==>    >  8B88 90000000   MOV ECX,DWORD PTR DS:[EAX+0x90]
	$+6      >  3B88 94000000   CMP ECX,DWORD PTR DS:[EAX+0x94]
	$+C      >^ 74 EB           JE SHORT CryGame.0D145F96
	$+E      >  53              PUSH EBX
	$+F      >  8B98 94000000   MOV EBX,DWORD PTR DS:[EAX+0x94]
	$+15     >  56              PUSH ESI
	$+16     >  8BF1            MOV ESI,ECX
	$+18     >  8D4F 04         LEA ECX,DWORD PTR DS:[EDI+0x4]
	$+1B     >  E8 83DCF6FF     CALL CryGame.0D0B3C40
	$+20     >  8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+22     >  8BC8            MOV ECX,EAX
	$+24     >  8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	$+27     >  FFD0            CALL EAX
	$+29     >  8BF8            MOV EDI,EAX
	$+2B     >  85FF            TEST EDI,EDI

	8B??????????3B??????????74????8B????????????8B
	--*/
	DWORD	dwMMOClientCraftInfoMaterialOffset = *scan_game_.ScanImme<DWORD>("8B??????????3B??????????74????8B????????????8B", 2);
	assert(dwMMOClientCraftInfoMaterialOffset == 0x90);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CRAFT_INF0_MATERIAL_OFFSET  " << dwMMOClientCraftInfoMaterialOffset << "  // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0C965BCC      8B0D B0F8E60D        MOV ECX,DWORD PTR DS:[0xDE6F8B0]
	$+6      0C965BD2      33DB                 XOR EBX,EBX
	$+8      0C965BD4      899C24 A8000000      MOV DWORD PTR SS:[ESP+0xA8],EBX
	$+F      0C965BDB      899C24 AC000000      MOV DWORD PTR SS:[ESP+0xAC],EBX
	$+16     0C965BE2      895C24 48            MOV DWORD PTR SS:[ESP+0x48],EBX
	$+1A     0C965BE6      895C24 4C            MOV DWORD PTR SS:[ESP+0x4C],EBX
	$+1E     0C965BEA      895C24 78            MOV DWORD PTR SS:[ESP+0x78],EBX
	$+22     0C965BEE      895C24 7C            MOV DWORD PTR SS:[ESP+0x7C],EBX
	$+26     0C965BF2      895C24 28            MOV DWORD PTR SS:[ESP+0x28],EBX
	$+2A     0C965BF6      895C24 2C            MOV DWORD PTR SS:[ESP+0x2C],EBX
	$+2E     0C965BFA      8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+30     0C965BFC      8B82 D8000000        MOV EAX,DWORD PTR DS:[EDX+0xD8]
	$+36     0C965C02      FFD0                 CALL EAX                                 ; 取赏金任务的数量
	$+38     0C965C04      894424 10            MOV DWORD PTR SS:[ESP+0x10],EAX
	$+3C     0C965C08      3BC3                 CMP EAX,EBX
	$+3E     0C965C0A      0F85 81000000        JNZ CryGame.0C965C91

	8B??????????33??89????????????89????????????89??????89
	--*/
	DWORD	dwMMOClientShangJinTaskBaseAddr = *scan_game_.ScanImme<DWORD>("8B??????????33??89????????????89????????????89??????89", 2);
	assert(dwMMOClientShangJinTaskBaseAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_BASE_ADDR  " << dwMMOClientShangJinTaskBaseAddr - GetModuleCryGameBase() << "  // " << dwMMOClientShangJinTaskBaseAddr << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientShangJinTaskCountCallOffset = *scan_game_.ScanImme<DWORD>("8B??????????33??89????????????89????????????89??????89", 0x30 + 2);
	assert(dwMMOClientShangJinTaskCountCallOffset == 0xD8);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_COUNT_CALL_OFFSET  " << dwMMOClientShangJinTaskCountCallOffset << " // CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0C965C91      8B0D B0F8E60D        MOV ECX,DWORD PTR DS:[0xDE6F8B0]
	$+6      0C965C97      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+8      0C965C99      8B80 E4000000        MOV EAX,DWORD PTR DS:[EAX+0xE4]
	$+E      0C965C9F      8D9424 C4000000      LEA EDX,DWORD PTR SS:[ESP+0xC4]
	$+15     0C965CA6      52                   PUSH EDX
	$+16     0C965CA7      895C24 48            MOV DWORD PTR SS:[ESP+0x48],EBX
	$+1A     0C965CAB      FFD0                 CALL EAX
	$+1C     0C965CAD      8B0D B0F8E60D        MOV ECX,DWORD PTR DS:[0xDE6F8B0]
	$+22     0C965CB3      8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+24     0C965CB5      8B92 DC000000        MOV EDX,DWORD PTR DS:[EDX+0xDC]
	$+2A     0C965CBB      8D4424 1C            LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$+2E     0C965CBF      50                   PUSH EAX
	$+2F     0C965CC0      FFD2                 CALL EDX                                 ; 取赏金任务

	8B??????????8B??8B??????????8D??????????????89??????FF
	--*/
	DWORD	dwMMOClientShangJinTaskCallOffset = *scan_game_.ScanImme<DWORD>("8B??????????8B??8B??????????8D??????????????89??????FF", 0x24 + 2);
	assert(dwMMOClientShangJinTaskCallOffset == 0xDC);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_CALL_OFFSET  " << dwMMOClientShangJinTaskCallOffset << " // CryGame.dll" << FC_OFstream_EndlLog;

	/*++

	$ ==>    0C966620      8B0D B0F8E60D        MOV ECX,DWORD PTR DS:[0xDE6F8B0]
	$+6      0C966626      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+8      0C966628      8B50 50              MOV EDX,DWORD PTR DS:[EAX+0x50]
	$+B      0C96662B      56                   PUSH ESI
	$+C      0C96662C      FFD2                 CALL EDX
	$+E      0C96662E      85C0                 TEST EAX,EAX
	$+10     0C966630      74 04                JE SHORT CryGame.0C966636
	$+12     0C966632      8B40 14              MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+15     0C966635      C3                   RETN
	$+16     0C966636      8B0D B0F8E60D        MOV ECX,DWORD PTR DS:[0xDE6F8B0]
	$+1C     0C96663C      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+1E     0C96663E      8B50 60              MOV EDX,DWORD PTR DS:[EAX+0x60]
	$+21     0C966641      56                   PUSH ESI
	$+22     0C966642      FFD2                 CALL EDX
	$+24     0C966644      0FB6C0               MOVZX EAX,AL
	$+27     0C966647      F7D8                 NEG EAX
	$+29     0C966649      1BC0                 SBB EAX,EAX
	$+2B     0C96664B      83E0 0A              AND EAX,0xA
	$+2E     0C96664E      C3                   RETN

	8B??????????8B??8B??????FF??85??74??8B????C38B
	--*/
	DWORD	dwMMOClientShangJinTaskStatusCall = (DWORD)scan_game_.ScanAddr("8B??????????8B??8B??????FF??85??74??8B????C38B", 0);
	assert(dwMMOClientShangJinTaskStatusCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_STATUS_CALL  " << dwMMOClientShangJinTaskStatusCall - GetModuleCryGameBase() << "  // " << dwMMOClientShangJinTaskStatusCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0C965E50      68 64C1B30D          PUSH CryGame.0DB3C164                    ; ASCII "compledaily"
	$+5      0C965E55      52                   PUSH EDX
	$+6      0C965E56      8B50 10              MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+9      0C965E59      FFD2                 CALL EDX
	$+B      0C965E5B      84C0                 TEST AL,AL
	$+D      0C965E5D      74 36                JE SHORT CryGame.0C965E95
	$+F      0C965E5F      8B7424 10            MOV ESI,DWORD PTR SS:[ESP+0x10]
	$+13     0C965E63      8B46 14              MOV EAX,DWORD PTR DS:[ESI+0x14]			// 完成赏金任务数量偏移
	$+16     0C965E66      50                   PUSH EAX
	$+17     0C965E67      8D4C24 7C            LEA ECX,DWORD PTR SS:[ESP+0x7C]
	$+1B     0C965E6B      51                   PUSH ECX
	$+1C     0C965E6C      57                   PUSH EDI
	$+1D     0C965E6D      BA 70C1B30D          MOV EDX,CryGame.0DB3C170                 ; ASCII "completecount"
	$+22     0C965E72      E8 3998D2FF          CALL CryGame.0C68F6B0                    ; 完成数量
	$+27     0C965E77      83C4 0C              ADD ESP,0xC
	$+2A     0C965E7A      8BCE                 MOV ECX,ESI
	$+2C     0C965E7C      E8 6F76DA00          CALL CryGame.0D70D4F0                    ; 取可接的总任务数量
	$+31     0C965E81      50                   PUSH EAX
	$+32     0C965E82      8D5424 7C            LEA EDX,DWORD PTR SS:[ESP+0x7C]
	$+36     0C965E86      52                   PUSH EDX
	$+37     0C965E87      57                   PUSH EDI
	$+38     0C965E88      BA 80C1B30D          MOV EDX,CryGame.0DB3C180                 ; ASCII "totalcount"
	$+3D     0C965E8D      E8 1E98D2FF          CALL CryGame.0C68F6B0

	68??????????8B????FF??84??74??8B??????8B??????8D??????????BA

	//ZHP
	$-36     > /0F85 C7000000   JNZ CryGame.0A9EE743
	$-30     > |E8 5FABD2FF     CALL CryGame.0A7191E0
	$-2B     > |8B4C24 44       MOV ECX,DWORD PTR SS:[ESP+0x44]
	$-27     > |81E1 8F000000   AND ECX,0x8F
	$-21     > |80F9 0A         CMP CL,0xA
	$-1E     > |8B4C24 40       MOV ECX,DWORD PTR SS:[ESP+0x40]
	$-1A     > |8B01            MOV EAX,DWORD PTR DS:[ECX]
	$-18     > |8B40 10         MOV EAX,DWORD PTR DS:[EAX+0x10]
	$-15     > |0F94C2          SETE DL
	$-12     > |885424 0C       MOV BYTE PTR SS:[ESP+0xC],DL
	$-E      > |8B5424 0C       MOV EDX,DWORD PTR SS:[ESP+0xC]
	$-A      > |52              PUSH EDX
	$-9      > |8D5424 7C       LEA EDX,DWORD PTR SS:[ESP+0x7C]
	$-5      > |52              PUSH EDX
	$-4      > |8B5424 50       MOV EDX,DWORD PTR SS:[ESP+0x50]
	$ ==>    > |68 5CB1C20B     PUSH CryGame.0BC2B15C                    ; ASCII "compledaily"
	$+5      > |52              PUSH EDX
	$+6      > |FFD0            CALL EAX
	$+8      > |84C0            TEST AL,AL
	$+A      > |74 36           JE SHORT CryGame.0A9EE6EE
	$+C      > |8B7424 20       MOV ESI,DWORD PTR SS:[ESP+0x20]
	$+10     > |8B4E 14         MOV ECX,DWORD PTR DS:[ESI+0x14]
	$+13     > |51              PUSH ECX
	$+14     > |8D5424 7C       LEA EDX,DWORD PTR SS:[ESP+0x7C]
	$+18     > |52              PUSH EDX
	$+19     > |53              PUSH EBX
	$+1A     > |BA 68B1C20B     MOV EDX,CryGame.0BC2B168                 ; ASCII "completecount"
	$+1F     > |E8 A0C1D0FF     CALL CryGame.0A6FA870
	$+24     > |83C4 0C         ADD ESP,0xC
	$+27     > |8BCE            MOV ECX,ESI
	$+29     > |E8 46F6DF00     CALL CryGame.0B7EDD20
	$+2E     > |50              PUSH EAX
	$+2F     > |8D4424 7C       LEA EAX,DWORD PTR SS:[ESP+0x7C]
	$+33     > |50              PUSH EAX
	$+34     > |53              PUSH EBX
	$+35     > |BA 78B1C20B     MOV EDX,CryGame.0BC2B178                 ; ASCII "totalcount"
	$+3A     > |E8 85C1D0FF     CALL CryGame.0A6FA870
	$+3F     > |83C4 0C         ADD ESP,0xC
	$+42     > |8B4C24 44       MOV ECX,DWORD PTR SS:[ESP+0x44]
	$+46     > |81E1 8F000000   AND ECX,0x8F
	$+4C     > |80F9 0A         CMP CL,0xA
	$+4F     > |8B4C24 40       MOV ECX,DWORD PTR SS:[ESP+0x40]
	$+53     > |8B01            MOV EAX,DWORD PTR DS:[ECX]
	$+55     > |8B40 10         MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+58     > |0F94C2          SETE DL
	$+5B     > |885424 0C       MOV BYTE PTR SS:[ESP+0xC],DL
	$+5F     > |8B5424 0C       MOV EDX,DWORD PTR SS:[ESP+0xC]
	$+63     > |52              PUSH EDX
	$+64     > |8D5424 64       LEA EDX,DWORD PTR SS:[ESP+0x64]
	$+68     > |52              PUSH EDX
	$+69     > |8B5424 50       MOV EDX,DWORD PTR SS:[ESP+0x50]
	$+6D     > |68 48B1C20B     PUSH CryGame.0BC2B148                    ; ASCII "refresh_tasks_list"
	$+72     > |52              PUSH EDX
	$+73     > |FFD0            CALL EAX

	68????????52FFD084C074??8B??????8B????518D??????5253BA????????E8
	--*/
	DWORD	dwMMOClientShangJinCountCompleOffset = *scan_game_.ScanImme<BYTE>("68????????52FFD084C074??8B??????8B????518D??????5253BA????????E8", 0x10 + 2);
	assert(dwMMOClientShangJinCountCompleOffset == 0x14);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_COMPLE_OFFSET  " << dwMMOClientShangJinCountCompleOffset << " // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientShangJinCountTotalCall = (DWORD)scan_game_.ScanCall("68????????52FFD084C074??8B??????8B????518D??????5253BA????????E8", 0x29 + 1);
	assert(dwMMOClientShangJinCountTotalCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_TOTAL_CALL  " << dwMMOClientShangJinCountTotalCall - GetModuleCryGameBase() << "  // " << dwMMOClientShangJinCountTotalCall << " CryGame.dll" << FC_OFstream_EndlLog;

	/*++
	$ ==>    0C947FA7        FFD0               CALL EAX
	$+2      0C947FA9        8BF0               MOV ESI,EAX
	$+4      0C947FAB        85F6               TEST ESI,ESI
	$+6      0C947FAD        74 6F              JE SHORT CryGame.0C94801E
	$+8      0C947FAF        83FF 01            CMP EDI,0x1
	$+B      0C947FB2        75 28              JNZ SHORT CryGame.0C947FDC
	$+D      0C947FB4        8B4D 10            MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+10     0C947FB7        8B79 08            MOV EDI,DWORD PTR DS:[ECX+0x8]
	$+13     0C947FBA        8B8E E8450000      MOV ECX,DWORD PTR DS:[ESI+0x45E8]					; UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_OFFSET
	$+19     0C947FC0        57                 PUSH EDI
	$+1A     0C947FC1        E8 0ADECB00        CALL CryGame.0D605DD0                               ; 获取整理的格子大小
	$+1F     0C947FC6        8B8E E8450000      MOV ECX,DWORD PTR DS:[ESI+0x45E8]
	$+25     0C947FCC        48                 DEC EAX
	$+26     0C947FCD        50                 PUSH EAX                                            ; 整理的仓库或者背包的格子数量
	$+27     0C947FCE        6A 00              PUSH 0x0
	$+29     0C947FD0        57                 PUSH EDI                                            ; 普通仓库=2 素材仓库=A
	$+2A     0C947FD1        E8 7A537900        CALL CryGame.0D0DD350

	FF??8B??85??74??83????75??8B????8B????8B
	--*/
	DWORD	dwMMOClientTidyInventoryItemOffset = *scan_game_.ScanImme<DWORD>("FF??8B??85??74??83????75??8B????8B????8B", 0x13 + 2);
	//assert(dwMMOClientTidyInventoryItemOffset==0x4BB8/*0x00004658*//*0x000045F8*/);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_OFFSET  " << dwMMOClientTidyInventoryItemOffset << " // CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientTidyInventoryItemSizeCall = (DWORD)scan_game_.ScanCall("FF??8B??85??74??83????75??8B????8B????8B", 0x1A + 1);
	assert(dwMMOClientTidyInventoryItemSizeCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_SIZE_CALL  " << dwMMOClientTidyInventoryItemSizeCall - GetModuleCryGameBase() << "  // " << dwMMOClientTidyInventoryItemSizeCall << " CryGame.dll" << FC_OFstream_EndlLog;

	DWORD	dwMMOClientTidyInventoryItemCall = (DWORD)scan_game_.ScanCall("FF??8B??85??74??83????75??8B????8B????8B", 0x2A + 1);
	assert(dwMMOClientTidyInventoryItemCall);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_CALL  " << dwMMOClientTidyInventoryItemCall - GetModuleCryGameBase() << "  // " << dwMMOClientTidyInventoryItemCall << " CryGame.dll" << FC_OFstream_EndlLog;


	/*++
	$-2      >  56              PUSH ESI
	$-1      >  57              PUSH EDI
	$ ==>    >  E8 39F7FDFF     CALL tersafe.06899020
	$+5      >  8B3D 54A09B06   MOV EDI,DWORD PTR DS:[<&KERNEL32.Sleep>]
	$+B      >  8BF0            MOV ESI,EAX
	$+D      >  8BCE            MOV ECX,ESI
	$+F      >  E8 B9AA1400     CALL tersafe.06A043AF                    ; hook
	$+14     >  8B86 7C020000   MOV EAX,DWORD PTR DS:[ESI+0x27C]
	$+1A     >  50              PUSH EAX
	$+1B     >  FFD7            CALL EDI
	$+1D     >^ EB EE           JMP SHORT tersafe.068B98EF

	E8????????8B??????????8B??8B??E8????????8B????????????FF??EB
	--*/

	DWORD	dwMMOClientCheckTersafeCodeCRCHook = (DWORD)scan_ter_safe_.ScanCall("E8????????8B??????????8B??8B??E8????????8B????????????FF??EB", 0xF + 1);
	assert(dwMMOClientCheckTersafeCodeCRCHook);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_TERSAFE_CODE_CRC_HOOK  " << dwMMOClientCheckTersafeCodeCRCHook - GetModuleTersafeBase() << "  // " << dwMMOClientCheckTersafeCodeCRCHook << " tersafe.dll" << FC_OFstream_EndlLog;


	/*
	//这是小陈加的
	$-8C     0040E169               /E9 C7000000   jmp MHOClien.0040E235
	$-87     0040E16E               |90            nop
	$-86     0040E16F             . |8B90 F0010000 mov edx,dword ptr ds:[eax+0x1F0]
	$-80     0040E175             . |85D2          test edx,edx
	$-7E     0040E177             . |0F8E A5000000 jle MHOClien.0040E222
	$-78     0040E17D             . |F605 B487C701>test byte ptr ds:[0x1C787B4],0x1
	$-71     0040E184             . |75 0E         jnz short MHOClien.0040E194
	$-6F     0040E186             . |830D B487C701>or dword ptr ds:[0x1C787B4],0x1
	$-68     0040E18D             . |FFD6          call esi
	$-66     0040E18F             . |A3 B087C701   mov dword ptr ds:[0x1C787B0],eax
	$-61     0040E194             > |A1 B050C701   mov eax,dword ptr ds:[0x1C750B0]
	$-5C     0040E199             . |8B88 F4010000 mov ecx,dword ptr ds:[eax+0x1F4]
	$-56     0040E19F             . |83F9 05       cmp ecx,0x5
	$-53     0040E1A2             . |7D 13         jge short MHOClien.0040E1B7
	$-51     0040E1A4             . |891D 1056C701 mov dword ptr ds:[0x1C75610],ebx
	$-4B     0040E1AA             . |891D 1456C701 mov dword ptr ds:[0x1C75614],ebx
	$-45     0040E1B0             . |FFD6          call esi
	$-43     0040E1B2             . |A3 B087C701   mov dword ptr ds:[0x1C787B0],eax
	$-3E     0040E1B7             > |8B15 B050C701 mov edx,dword ptr ds:[0x1C750B0]         ;  MHOClien.0161F088
	$-38     0040E1BD             . |8B82 F0010000 mov eax,dword ptr ds:[edx+0x1F0]
	$-32     0040E1C3             . |3905 68C87E01 cmp dword ptr ds:[0x17EC868],eax
	$-2C     0040E1C9             . |74 22         je short MHOClien.0040E1ED
	$-2A     0040E1CB             . |891D 1056C701 mov dword ptr ds:[0x1C75610],ebx
	$-24     0040E1D1             . |891D 1456C701 mov dword ptr ds:[0x1C75614],ebx
	$-1E     0040E1D7             . |FFD6          call esi
	$-1C     0040E1D9             . |8B0D B050C701 mov ecx,dword ptr ds:[0x1C750B0]         ;  MHOClien.0161F088
	$-16     0040E1DF             . |8B91 F0010000 mov edx,dword ptr ds:[ecx+0x1F0]
	$-10     0040E1E5             . |8915 68C87E01 mov dword ptr ds:[0x17EC868],edx
	$-A      0040E1EB             . |EB 30         jmp short MHOClien.0040E21D
	$-8      0040E1ED             > |FFD6          call esi
	$-6      0040E1EF             . |8B3D B087C701 mov edi,dword ptr ds:[0x1C787B0]         ;  MHOClien.01A40E5A
	$ ==>    0040E1F5             . |33C9          xor ecx,ecx
	$+2      0040E1F7             . |33DB          xor ebx,ebx
	$+4      0040E1F9             . |8BD0          mov edx,eax
	$+6      0040E1FB             . |2BD7          sub edx,edi
	$+8      0040E1FD             . |1BCB          sbb ecx,ebx
	$+A      0040E1FF             . |3BCB          cmp ecx,ebx


	//zhp 2016/5/17
	$-FE     0040E11B      68 68D56101           PUSH mhoclien.0161D568                   ; ASCII "user32.dll"
	$-F9     0040E120      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$-F7     0040E122      8B3D 04123C01         MOV EDI, DWORD PTR DS:[13C1204]
	$-F1     0040E128      33DB                  XOR EBX, EBX
	$-EF     0040E12A      3BC3                  CMP EAX, EBX
	$-ED     0040E12C      74 0E                 JE SHORT mhoclien.0040E13C
	$-EB     0040E12E      68 74D56101           PUSH mhoclien.0161D574                   ; ASCII "SetProcessDPIAware"
	$-E6     0040E133      50                    PUSH EAX                                 ; mhoclien.01820E70
	$-E5     0040E134      FFD7                  CALL EDI
	$-E3     0040E136      3BC3                  CMP EAX, EBX
	$-E1     0040E138      74 02                 JE SHORT mhoclien.0040E13C
	$-DF     0040E13A      FFD0                  CALL EAX                                 ; mhoclien.01820E70
	$-DD     0040E13C      8B45 08               MOV EAX, DWORD PTR SS:[EBP+8]            ; mhoclien.00400000
	$-DA     0040E13F      8B4D 10               MOV ECX, DWORD PTR SS:[EBP+10]
	$-D7     0040E142      C705 6C1FC901 90E0400>MOV DWORD PTR DS:[1C91F6C], mhoclien.004>
	$-CD     0040E14C      A3 3C1DC901           MOV DWORD PTR DS:[1C91D3C], EAX          ; mhoclien.01820E70
	$-C8     0040E151      890D 381DC901         MOV DWORD PTR DS:[1C91D38], ECX
	$-C2     0040E157      FF15 40143C01         CALL DWORD PTR DS:[13C1440]
	$-BC     0040E15D      B9 E8030000           MOV ECX, 3E8
	$-B7     0040E162      8D5424 18             LEA EDX, DWORD PTR SS:[ESP+18]
	$-B3     0040E166      E8 65790000           CALL mhoclien.00415AD0
	$-AE     0040E16B      68 FCC46101           PUSH mhoclien.0161C4FC                   ; ASCII "MemoryProfiler.dll"
	$-A9     0040E170      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$-A7     0040E172      8BF0                  MOV ESI, EAX                             ; mhoclien.01820E70
	$-A5     0040E174      3BF3                  CMP ESI, EBX
	$-A3     0040E176      74 41                 JE SHORT mhoclien.0040E1B9
	$-A1     0040E178      68 18C76101           PUSH mhoclien.0161C718                   ; ASCII "Init"
	$-9C     0040E17D      56                    PUSH ESI                                 ; kernel32.GetTickCount
	$-9B     0040E17E      FFD7                  CALL EDI
	$-99     0040E180      68 88D56101           PUSH mhoclien.0161D588                   ; ASCII "BeginProfiling"
	$-94     0040E185      56                    PUSH ESI                                 ; kernel32.GetTickCount
	$-93     0040E186      A3 2C1DC901           MOV DWORD PTR DS:[1C91D2C], EAX          ; mhoclien.01820E70
	$-8E     0040E18B      FFD7                  CALL EDI
	$-8C     0040E18D      68 98D56101           PUSH mhoclien.0161D598                   ; ASCII "TryWaitProfData"
	$-87     0040E192      56                    PUSH ESI                                 ; kernel32.GetTickCount
	$-86     0040E193      A3 301DC901           MOV DWORD PTR DS:[1C91D30], EAX          ; mhoclien.01820E70
	$-81     0040E198      FFD7                  CALL EDI
	$-7F     0040E19A      A3 341DC901           MOV DWORD PTR DS:[1C91D34], EAX          ; mhoclien.01820E70
	$-7A     0040E19F      FF15 2C1DC901         CALL DWORD PTR DS:[1C91D2C]
	$-74     0040E1A5      8D5424 18             LEA EDX, DWORD PTR SS:[ESP+18]
	$-70     0040E1A9      52                    PUSH EDX
	$-6F     0040E1AA      FF15 341DC901         CALL DWORD PTR DS:[1C91D34]
	$-69     0040E1B0      83C4 04               ADD ESP, 4
	$-66     0040E1B3      FF15 301DC901         CALL DWORD PTR DS:[1C91D30]
	$-60     0040E1B9      68 90A34000           PUSH mhoclien.0040A390
	$-5B     0040E1BE      E8 7E7DF100           CALL mhoclien.01325F41
	$-56     0040E1C3      83C4 04               ADD ESP, 4
	$-53     0040E1C6      8D4424 10             LEA EAX, DWORD PTR SS:[ESP+10]
	$-4F     0040E1CA      50                    PUSH EAX                                 ; mhoclien.01820E70
	$-4E     0040E1CB      6A 04                 PUSH 4
	$-4C     0040E1CD      53                    PUSH EBX
	$-4B     0040E1CE      68 00E34000           PUSH mhoclien.0040E300
	$-46     0040E1D3      68 00002000           PUSH 200000
	$-41     0040E1D8      53                    PUSH EBX
	$-40     0040E1D9      895C24 28             MOV DWORD PTR SS:[ESP+28], EBX
	$-3C     0040E1DD      E8 7C7DF100           CALL mhoclien.01325F5E                   ; JMP 到 MSVCR90._beginthreadex
	$-37     0040E1E2      8BF8                  MOV EDI, EAX                             ; mhoclien.01820E70
	$-35     0040E1E4      83C4 18               ADD ESP, 18
	$-32     0040E1E7      57                    PUSH EDI
	$-31     0040E1E8      897C24 18             MOV DWORD PTR SS:[ESP+18], EDI
	$-2D     0040E1EC      FF15 9C113C01         CALL DWORD PTR DS:[13C119C]
	$-27     0040E1F2      8D4C24 0C             LEA ECX, DWORD PTR SS:[ESP+C]
	$-23     0040E1F6      51                    PUSH ECX
	$-22     0040E1F7      57                    PUSH EDI
	$-21     0040E1F8      895C24 14             MOV DWORD PTR SS:[ESP+14], EBX
	$-1D     0040E1FC      FF15 EC113C01         CALL DWORD PTR DS:[13C11EC]
	$-17     0040E202      395C24 0C             CMP DWORD PTR SS:[ESP+C], EBX
	$-13     0040E206      0F84 E3000000         JE mhoclien.0040E2EF
	$-D      0040E20C      8B35 14133C01         MOV ESI, DWORD PTR DS:[13C1314]          ; kernel32.GetTickCount
	$-7      0040E212      A1 C81CC901           MOV EAX, DWORD PTR DS:[1C91CC8]
	$-2      0040E217      3BC3                  CMP EAX, EBX
	$ ==>    0040E219      0F84 B3000000         JE mhoclien.0040E2D2
	$+6      0040E21F      8B90 F8010000         MOV EDX, DWORD PTR DS:[EAX+1F8]
	$+C      0040E225      85D2                  TEST EDX, EDX
	$+E      0040E227      0F8E A5000000         JLE mhoclien.0040E2D2
	$+14     0040E22D      F605 145EC901 01      TEST BYTE PTR DS:[1C95E14], 1
	$+1B     0040E234      75 0E                 JNZ SHORT mhoclien.0040E244
	$+1D     0040E236      830D 145EC901 01      OR DWORD PTR DS:[1C95E14], 1
	$+24     0040E23D      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$+26     0040E23F      A3 105EC901           MOV DWORD PTR DS:[1C95E10], EAX          ; mhoclien.01820E70
	$+2B     0040E244      A1 C81CC901           MOV EAX, DWORD PTR DS:[1C91CC8]
	$+30     0040E249      8B88 FC010000         MOV ECX, DWORD PTR DS:[EAX+1FC]
	$+36     0040E24F      83F9 05               CMP ECX, 5
	$+39     0040E252      7D 13                 JGE SHORT mhoclien.0040E267
	$+3B     0040E254      891D 702CC901         MOV DWORD PTR DS:[1C92C70], EBX
	$+41     0040E25A      891D 742CC901         MOV DWORD PTR DS:[1C92C74], EBX
	$+47     0040E260      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$+49     0040E262      A3 105EC901           MOV DWORD PTR DS:[1C95E10], EAX          ; mhoclien.01820E70
	$+4E     0040E267      8B15 C81CC901         MOV EDX, DWORD PTR DS:[1C91CC8]          ; mhoclien.01820E70
	$+54     0040E26D      8B82 F8010000         MOV EAX, DWORD PTR DS:[EDX+1F8]
	$+5A     0040E273      3905 C87E8001         CMP DWORD PTR DS:[1807EC8], EAX          ; mhoclien.01820E70
	$+60     0040E279      74 22                 JE SHORT mhoclien.0040E29D
	$+62     0040E27B      891D 702CC901         MOV DWORD PTR DS:[1C92C70], EBX
	$+68     0040E281      891D 742CC901         MOV DWORD PTR DS:[1C92C74], EBX
	$+6E     0040E287      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$+70     0040E289      8B0D C81CC901         MOV ECX, DWORD PTR DS:[1C91CC8]          ; mhoclien.01820E70
	$+76     0040E28F      8B91 F8010000         MOV EDX, DWORD PTR DS:[ECX+1F8]
	$+7C     0040E295      8915 C87E8001         MOV DWORD PTR DS:[1807EC8], EDX
	$+82     0040E29B      EB 30                 JMP SHORT mhoclien.0040E2CD
	$+84     0040E29D      FFD6                  CALL ESI                                 ; kernel32.GetTickCount
	$+86     0040E29F      8B3D 105EC901         MOV EDI, DWORD PTR DS:[1C95E10]          ; mhoclien.0059FBFC
	$+8C     0040E2A5      33C9                  XOR ECX, ECX
	$+8E     0040E2A7      33DB                  XOR EBX, EBX
	$+90     0040E2A9      8BD0                  MOV EDX, EAX                             ; mhoclien.01820E70
	$+92     0040E2AB      2BD7                  SUB EDX, EDI
	$+94     0040E2AD      1BCB                  SBB ECX, EBX
	$+96     0040E2AF      3BCB                  CMP ECX, EBX
	$+98     0040E2B1      77 16                 JA SHORT mhoclien.0040E2C9
	$+9A     0040E2B3      72 08                 JB SHORT mhoclien.0040E2BD
	$+9C     0040E2B5      81FA D0070000         CMP EDX, 7D0
	$+A2     0040E2BB      73 0C                 JNB SHORT mhoclien.0040E2C9
	$+A4     0040E2BD      0115 702CC901         ADD DWORD PTR DS:[1C92C70], EDX
	$+AA     0040E2C3      110D 742CC901         ADC DWORD PTR DS:[1C92C74], ECX
	$+B0     0040E2C9      8B7C24 14             MOV EDI, DWORD PTR SS:[ESP+14]
	$+B4     0040E2CD      A3 105EC901           MOV DWORD PTR DS:[1C95E10], EAX          ; mhoclien.01820E70
	$+B9     0040E2D2      8D4424 0C             LEA EAX, DWORD PTR SS:[ESP+C]
	$+BD     0040E2D6      50                    PUSH EAX                                 ; mhoclien.01820E70
	$+BE     0040E2D7      57                    PUSH EDI
	$+BF     0040E2D8      FF15 EC113C01         CALL DWORD PTR DS:[13C11EC]
	$+C5     0040E2DE      6A 01                 PUSH 1
	$+C7     0040E2E0      E8 3B320200           CALL mhoclien.00431520
	$+CC     0040E2E5      395C24 0C             CMP DWORD PTR SS:[ESP+C], EBX
	$+D0     0040E2E9    ^ 0F85 23FFFFFF         JNZ mhoclien.0040E212
	$+D6     0040E2EF      5F                    POP EDI
	$+D7     0040E2F0      5E                    POP ESI                                  ; kernel32.GetTickCount
	$+D8     0040E2F1      33C0                  XOR EAX, EAX                             ; mhoclien.01820E70
	$+DA     0040E2F3      5B                    POP EBX
	$+DB     0040E2F4      8BE5                  MOV ESP, EBP
	$+DD     0040E2F6      5D                    POP EBP
	$+DE     0040E2F7      C2 1000               RETN 10
	*/
	//0f??????????8b??????????85??0f??????????f6????????????75??83????????????ff??a3????????a1????????8b??????????83
	DWORD	dwMMOClientGAMEOUTMESSAGEOBXAddr = (DWORD)scan_exe_.ScanAddr("0f??????????8b??????????85??0f??????????f6????????????75??83????????????ff??a3????????a1????????8b??????????83", 0);
	assert(dwMMOClientGAMEOUTMESSAGEOBXAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMEOUTMESSAGEOBX_WRITE  " << dwMMOClientGAMEOUTMESSAGEOBXAddr << "  // " << dwMMOClientGAMEOUTMESSAGEOBXAddr << " MHOClient.exe" << FC_OFstream_EndlLog;

	/*
	$-FD     05FED040                            /$  64:A1 0000000>mov eax,dword ptr fs:[0]
	$-F7     05FED046                            |.  6A FF         push -0x1
	$-F5     05FED048                            |.  68 736E0E06   push tersafe.060E6E73
	$-F0     05FED04D                            |.  50            push eax
	$-EF     05FED04E                            |.  A1 A0101206   mov eax,dword ptr ds:[0x61210A0]
	$-EA     05FED053                            |.  64:8925 00000>mov dword ptr fs:[0],esp
	$-E3     05FED05A                            |.  83EC 08       sub esp,0x8
	$-E0     05FED05D                            |.  85C0          test eax,eax
	$-DE     05FED05F                            |.  75 5E         jnz short tersafe.05FED0BF
	$-DC     05FED061                            |.  68 80101206   push tersafe.06121080                    ; /pCriticalSection = tersafe.06121080
	$-D7     05FED066                            |.  C74424 04 801>mov dword ptr ss:[esp+0x4],tersafe.06121>; |
	$-CF     05FED06E                            |.  FF15 0CA10E06 call dword ptr ds:[<&KERNEL32.EnterCriti>; \EnterCriticalSection
	$-C9     05FED074                            |.  A1 A0101206   mov eax,dword ptr ds:[0x61210A0]
	$-C4     05FED079                            |.  C74424 10 000>mov dword ptr ss:[esp+0x10],0x0
	$-BC     05FED081                            |.  85C0          test eax,eax
	$-BA     05FED083                            |.  75 2A         jnz short tersafe.05FED0AF
	$-B8     05FED085                            |.  68 90010000   push 0x190
	$-B3     05FED08A                            |.  E8 7BC80600   call <jmp.&MSVCRT.??2@YAPAXI@Z>
	$-AE     05FED08F                            |.  83C4 04       add esp,0x4
	$-AB     05FED092                            |.  894424 04     mov dword ptr ss:[esp+0x4],eax
	$-A7     05FED096                            |.  85C0          test eax,eax
	$-A5     05FED098                            |.  C64424 10 01  mov byte ptr ss:[esp+0x10],0x1
	$-A0     05FED09D                            |.  74 09         je short tersafe.05FED0A8
	$-9E     05FED09F                            |.  8BC8          mov ecx,eax
	$-9C     05FED0A1                            |.  E8 2A000000   call tersafe.05FED0D0
	$-97     05FED0A6                            |.  EB 02         jmp short tersafe.05FED0AA
	$-95     05FED0A8                            |>  33C0          xor eax,eax
	$-93     05FED0AA                            |>  A3 A0101206   mov dword ptr ds:[0x61210A0],eax
	$-8E     05FED0AF                            |>  68 80101206   push tersafe.06121080                    ; /pCriticalSection = tersafe.06121080
	$-89     05FED0B4                            |.  FF15 10A10E06 call dword ptr ds:[<&KERNEL32.LeaveCriti>; \LeaveCriticalSection
	$-83     05FED0BA                            |.  A1 A0101206   mov eax,dword ptr ds:[0x61210A0]         ;  TerSafe首地址
	$-7E     05FED0BF                            |>  8B4C24 08     mov ecx,dword ptr ss:[esp+0x8]
	$-7A     05FED0C3                            |.  64:890D 00000>mov dword ptr fs:[0],ecx
	$-73     05FED0CA                            |.  83C4 14       add esp,0x14
	$-70     05FED0CD                            \.  C3            retn
	$-6F     05FED0CE                                90            nop
	$-6E     05FED0CF                                90            nop
	$-6D     05FED0D0                            /$  6A FF         push -0x1
	$-6B     05FED0D2                            |.  68 FF6E0E06   push tersafe.060E6EFF                    ;  SE 处理程序安装
	$-66     05FED0D7                            |.  64:A1 0000000>mov eax,dword ptr fs:[0]
	$-60     05FED0DD                            |.  50            push eax
	$-5F     05FED0DE                            |.  64:8925 00000>mov dword ptr fs:[0],esp
	$-58     05FED0E5                            |.  81EC 4C090000 sub esp,0x94C
	$-52     05FED0EB                            |.  53            push ebx
	$-51     05FED0EC                            |.  55            push ebp
	$-50     05FED0ED                            |.  8A4424 0B     mov al,byte ptr ss:[esp+0xB]
	$-4C     05FED0F1                            |.  8BE9          mov ebp,ecx
	$-4A     05FED0F3                            |.  56            push esi
	$-49     05FED0F4                            |.  57            push edi
	$-48     05FED0F5                            |.  8D7D 04       lea edi,dword ptr ss:[ebp+0x4]
	$-45     05FED0F8                            |.  C745 00 1CAC0>mov dword ptr ss:[ebp],tersafe.060EAC1C
	$-3E     05FED0FF                            |.  33DB          xor ebx,ebx
	$-3C     05FED101                            |.  896C24 34     mov dword ptr ss:[esp+0x34],ebp
	$-38     05FED105                            |.  C707 18AC0E06 mov dword ptr ds:[edi],tersafe.060EAC18
	$-32     05FED10B                            |.  C745 08 F0A80>mov [arg.1],tersafe.060EA8F0
	$-2B     05FED112                            |.  C745 0C 10AC0>mov [arg.2],tersafe.060EAC10
	$-24     05FED119                            |.  8845 10       mov byte ptr ss:[ebp+0x10],al
	$-21     05FED11C                            |.  895D 14       mov [arg.4],ebx
	$-1E     05FED11F                            |.  895D 18       mov [arg.5],ebx
	$-1B     05FED122                            |.  895D 1C       mov [arg.6],ebx
	$-18     05FED125                            |.  895D 20       mov [arg.7],ebx
	$-15     05FED128                            |.  895D 24       mov [arg.8],ebx
	$-12     05FED12B                            |.  895D 28       mov [arg.9],ebx
	$-F      05FED12E                            |.  895D 2C       mov [arg.10],ebx
	$-C      05FED131                            |.  895D 30       mov [arg.11],ebx
	$-9      05FED134                            |.  895D 34       mov [arg.12],ebx
	$-6      05FED137                            |.  895D 38       mov [arg.13],ebx
	$-3      05FED13A                            |.  895D 3C       mov [arg.14],ebx
	$ ==>    05FED13D                            |.  8A5424 13     mov dl,byte ptr ss:[esp+0x13]
	$+4      05FED141                            |.  8A4424 13     mov al,byte ptr ss:[esp+0x13]
	$+8      05FED145                            |.  8D4D 44       lea ecx,[arg.16]
	$+B      05FED148                            |.  899C24 640900>mov dword ptr ss:[esp+0x964],ebx
	$+12     05FED14F                            |.  8811          mov byte ptr ds:[ecx],dl
	$+14     05FED151                            |.  8841 01       mov byte ptr ds:[ecx+0x1],al
	$+17     05FED154                            |.  8859 08       mov byte ptr ds:[ecx+0x8],bl
	$+1A     05FED157                            |.  E8 B4560000   call tersafe.05FF2810

	8A 54 24 13 8A 44 24 13 8D 4D 44


	*/

	DWORD	dwMMOGAMETersafeDllCheckVitAddr = *scan_ter_safe_.ScanImme<DWORD>("8A5424138A4424138D4D44", -0x82);
	assert(dwMMOGAMETersafeDllCheckVitAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMETersafeDllCheckVit_ADDR  " << dwMMOGAMETersafeDllCheckVitAddr - GetModuleTersafeBase() << "  // " << dwMMOGAMETersafeDllCheckVitAddr << " terSafe.dll" << FC_OFstream_EndlLog;


	/*
	$-2E     004E2501      90              nop                                      ; 干掉点1
	$-2D     004E2502      90              nop
	$-2C     004E2503      5E              pop esi
	$-2B     004E2504      B8 0B000000     mov eax,0xB
	$-26     004E2509      5B              pop ebx
	$-25     004E250A      8BE5            mov esp,ebp
	$-23     004E250C      5D              pop ebp
	$-22     004E250D      C2 1000         retn 0x10
	$-1F     004E2510      57              push edi
	$-1E     004E2511      E8 CA0CFAFF     call MHOClien.004831E0
	$-19     004E2516      8D8B 08910000   lea ecx,dword ptr ds:[ebx+0x9108]
	$-13     004E251C      E8 6FF3F1FF     call MHOClien.00401890
	$-E      004E2521      8B7B 14         mov edi,dword ptr ds:[ebx+0x14]
	$-B      004E2524      8D97 80000000   lea edx,dword ptr ds:[edi+0x80]
	$-5      004E252A      8BC2            mov eax,edx
	$-3      004E252C      2B43 10         sub eax,dword ptr ds:[ebx+0x10]
	$ ==>    004E252F      C1F8 1F         sar eax,0x1F
	8B C2 2B 43 10 C1 F8 1F 8B C8 F7 D1 23 4B 0C  //通过函数返回


	*/



	DWORD	dwMMOCHECK_GAMECheckNop1Addr = (DWORD)scan_exe_.ScanAddr("8BC22B4310C1F81F8BC8F7D1234B0C", -0x29);
	assert(dwMMOCHECK_GAMECheckNop1Addr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckNop1_ADDR  " << dwMMOCHECK_GAMECheckNop1Addr << "  // " << dwMMOCHECK_GAMECheckNop1Addr << " MHOClient.exe" << FC_OFstream_EndlLog;




	/*


	$ ==>    00E69490         55              push ebp
	$+1      00E69491         8BEC            mov ebp,esp
	$+3      00E69493         83EC 14         sub esp,0x14
	$+6      00E69496         53              push ebx
	$+7      00E69497         8BD9            mov ebx,ecx
	$+9      00E69499         8A43 08         mov al,byte ptr ds:[ebx+0x8]
	$+C      00E6949C         84C0            test al,al
	$+E      00E6949E         75 3D           jnz short MHOClien.00E694DD
	$+10     00E694A0         56              push esi
	$+11     00E694A1         57              push edi
	$+12     00E694A2         8B3D FC713B01   mov edi,dword ptr ds:[0x13B71FC]
	$+18     00E694A8         33F6            xor esi,esi
	$+1A     00E694AA         8D9B 00000000   lea ebx,dword ptr ds:[ebx]
	$+20     00E694B0         8D4D EC         lea ecx,dword ptr ss:[ebp-0x14]
	$+23     00E694B3         51              push ecx
	$+24     00E694B4         6A 01           push 0x1
	$+26     00E694B6         56              push esi
	$+27     00E694B7         E8 6AEC4A00     call MHOClien.01318126
	$+2C     00E694BC         85C0            test eax,eax
	$+2E     00E694BE         0f94c2          sete dl
	$+31     00E694C1         8896 D00EA201   mov byte ptr ds:[esi+0x1A20ED0],dl
	$+37     00E694C7         46              inc esi
	$+38     00E694C8         83FE 04         cmp esi,0x4
	$+3B     00E694CB       ^ 72 E3           jb short MHOClien.00E694B0
	$+3D     00E694CD         68 E8030000     push 0x3E8
	$+42     00E694D2         FFD7            call edi
	$+44     00E694D4         8A43 08         mov al,byte ptr ds:[ebx+0x8]
	$+47     00E694D7         84C0            test al,al                               ; 干掉点2
	$+49     00E694D9       ^ 74 CD           je short MHOClien.00E694A8
	$+4B     00E694DB         5F              pop edi
	$+4C     00E694DC         5E              pop esi
	$+4D     00E694DD         5B              pop ebx
	$+4E     00E694DE         8BE5            mov esp,ebp
	$+50     00E694E0         5D              pop ebp
	$+51     00E694E1         C3              retn



	8D9B000000008D4DEC516A0156


	*/

	DWORD	dwMMOCHECK_GAMECheckNop2Addr = (DWORD)scan_exe_.ScanAddr("8D9B000000008D4DEC516A0156", 0x2F);
	assert(dwMMOCHECK_GAMECheckNop2Addr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckNop2_ADDR  " << dwMMOCHECK_GAMECheckNop2Addr << "  // " << dwMMOCHECK_GAMECheckNop2Addr << " MHOClient.exe" << FC_OFstream_EndlLog;
	/*
	$ ==>    00E687D0    - E9 25692E01     jmp Lpk.0214F0FA
	$+5      00E687D5      0F85 C7010000   jnz MHOClien.00E689A2                    ; 这里也要处理
	$+B      00E687DB      8B4E 0C         mov ecx,dword ptr ds:[esi+0xC]
	$+E      00E687DE      83B9 84000000 0>cmp dword ptr ds:[ecx+0x84],0x0
	$+15     00E687E5      0F84 DF000000   je MHOClien.00E688CA
	$+1B     00E687EB      8B56 04         mov edx,dword ptr ds:[esi+0x4]
	$+1E     00E687EE      85D2            test edx,edx
	$+20     00E687F0      0F85 D4000000   jnz MHOClien.00E688CA
	$+26     00E687F6      3815 CC0EA201   cmp byte ptr ds:[0x1A20ECC],dl
	$+2C     00E687FC      0F85 C8000000   jnz MHOClien.00E688CA
	$+32     00E68802      8B7E 0C         mov edi,dword ptr ds:[esi+0xC]
	$+35     00E68805      3997 88000000   cmp dword ptr ds:[edi+0x88],edx
	$+3B     00E6880B      0F84 B9000000   je MHOClien.00E688CA
	$+41     00E68811      68 94040000     push 0x494
	$+46     00E68816      52              push edx
	$+47     00E68817      8D85 44FBFFFF   lea eax,dword ptr ss:[ebp-0x4BC]
	$+4D     00E6881D      50              push eax
	$+4E     00E6881E      E8 BD414B00     call MHOClien.0131C9E0                   ; jmp 到 msvcr90.memset
	$+53     00E68823      83C4 0C         add esp,0xC
	$+56     00E68826      C785 40FBFFFF 9>mov dword ptr ss:[ebp-0x4C0],0x498
	$+60     00E68830      C745 EC 0000000>mov dword ptr ss:[ebp-0x14],0x0
	$+67     00E68837      8D8D 40FBFFFF   lea ecx,dword ptr ss:[ebp-0x4C0]
	$+6D     00E6883D      894D D8         mov dword ptr ss:[ebp-0x28],ecx
	$+70     00E68840      C645 DC 00      mov byte ptr ss:[ebp-0x24],0x0
	$+74     00E68844      8B97 88000000   mov edx,dword ptr ds:[edi+0x88]
	$+7A     00E6884A      8955 E0         mov dword ptr ss:[ebp-0x20],edx
	$+7D     00E6884D      8D45 EC         lea eax,dword ptr ss:[ebp-0x14]
	$+80     00E68850      8945 E4         mov dword ptr ss:[ebp-0x1C],eax
	$+83     00E68853      6A 00           push 0x0
	$+85     00E68855      68 1A346101     push MHOClien.0161341A
	$+8A     00E6885A      B9 0C7F6201     mov ecx,MHOClien.01627F0C
	$+8F     00E6885F      E8 1CA45AFF     call MHOClien.00412C80
	$+94     00E68864      C605 A80EA201 0>mov byte ptr ds:[0x1A20EA8],0x0
	$+9B     00E6886B      33FF            xor edi,edi
	$+9D     00E6886D      897E 08         mov dword ptr ds:[esi+0x8],edi
	$+A0     00E68870      897D FC         mov dword ptr ss:[ebp-0x4],edi
	$+A3     00E68873      8B4E 0C         mov ecx,dword ptr ds:[esi+0xC]
	$+A6     00E68876      8B81 88000000   mov eax,dword ptr ds:[ecx+0x88]
	$+AC     00E6887C      8B10            mov edx,dword ptr ds:[eax]
	$+AE     00E6887E      6A 01           push 0x1
	$+B0     00E68880      8D4D D8         lea ecx,dword ptr ss:[ebp-0x28]
	$+B3     00E68883      51              push ecx
	$+B4     00E68884      68 1086E600     push MHOClien.00E68610
	$+B9     00E68889      6A 04           push 0x4
	$+BB     00E6888B      50              push eax
	$+BC     00E6888C      8B52 10         mov edx,dword ptr ds:[edx+0x10]
	$+BF     00E6888F      FFD2            call edx
	$+C1     00E68891      85C0            test eax,eax
	$+C3     00E68893      7D 1D           jge short MHOClien.00E688B2
	$+C5     00E68895      B9 B00EA201     mov ecx,MHOClien.01A20EB0
	$+CA     00E6889A      E8 D1F3FFFF     call MHOClien.00E67C70
	$+CF     00E6889F      68 E8030000     push 0x3E8
	$+D4     00E688A4      FFD3            call ebx
	$+D6     00E688A6      C745 FC FFFFFFF>mov dword ptr ss:[ebp-0x4],-0x1
	$+DD     00E688AD    ^ E9 1EFFFFFF     jmp MHOClien.00E687D0

	68E8030000FFD3C745FCFFFFFFFF


	*/
	DWORD	dwMMOGAMECheckSleepAddr = (DWORD)scan_exe_.ScanAddr("68E8030000FFD3C745FCFFFFFFFF", -0xCF);
	assert(dwMMOGAMECheckSleepAddr);
	ofs << "#define UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckSleep_ADDR  " << dwMMOGAMECheckSleepAddr << "  // " << dwMMOGAMECheckSleepAddr << " MHOClient.exe" << FC_OFstream_EndlLog;


	/*
	$-8      > > \C647 10 00    mov byte ptr ds:[edi+0x10],0x0
	$-4      > .  C647 11 00    mov byte ptr ds:[edi+0x11],0x0
	$ ==>    > >  8345 F8 04    add dword ptr ss:[ebp-0x8],0x4
	$+4      > .  836D F4 01    sub dword ptr ss:[ebp-0xC],0x1
	$+8      > .^ 0F85 E6FEFFFF jnz CryActio.10D77723
	$+E      > .  A1 58745511   mov eax,dword ptr ds:[0x11557458]        ;  UI首地址
	$+13     > .  83B8 BC000000>cmp dword ptr ds:[eax+0xBC],0x0
	$+1A     > .  74 14         je short CryActio.10D7785F
	$+1C     > .  8B88 BC000000 mov ecx,dword ptr ds:[eax+0xBC]          ;  一级偏移
	$+22     > .  D945 F0       fld dword ptr ss:[ebp-0x10]
	$+25     > .  8B11          mov edx,dword ptr ds:[ecx]
	$+27     > .  8B42 18       mov eax,dword ptr ds:[edx+0x18]
	$+2A     > .  51            push ecx
	$+2B     > .  D91C24        fstp dword ptr ss:[esp]
	$+2E     > .  FFD0          call eax
	$+30     > >  8B5D EC       mov ebx,dword ptr ss:[ebp-0x14]
	$+33     > .  D945 FC       fld dword ptr ss:[ebp-0x4]
	$+36     > .  8B8B 1C050000 mov ecx,dword ptr ds:[ebx+0x51C]
	$+3C     > .  8B11          mov edx,dword ptr ds:[ecx]
	$+3E     > .  8B42 2C       mov eax,dword ptr ds:[edx+0x2C]
	$+41     > .  51            push ecx
	$+42     > .  D91C24        fstp dword ptr ss:[esp]
	$+45     > .  FFD0          call eax
	$+47     > .  A1 58745511   mov eax,dword ptr ds:[0x11557458]
	$+4C     > .  83B8 A8000000>cmp dword ptr ds:[eax+0xA8],0x0

	8345 F8 04  836D F4 01 0F85 ???????? A1

	*/
	DWORD	dwMMOUI_BASEAddr = *scan_cry_action_.ScanImme<DWORD>("8345F804836DF4010F85????????A1", 0xF);
	assert(dwMMOUI_BASEAddr);
	ofs << "#define UPDATES_REVERSAL_UI_BASE_ADDR  " << dwMMOUI_BASEAddr - GetModuleCryActionBase() << "  // " << dwMMOUI_BASEAddr << " CryAction.dll" << FC_OFstream_EndlLog;


	/*
	$-3B     >/$  55            push ebp
	$-3A     >|.  8BEC          mov ebp,esp
	$-38     >|.  56            push esi
	$-37     >|.  8BF1          mov esi,ecx
	$-35     >|.  0FB746 3E     movzx eax,word ptr ds:[esi+0x3E]
	$-31     >|.  8AC8          mov cl,al
	$-2F     >|.  C0E9 04       shr cl,0x4
	$-2C     >|.  F6C1 01       test cl,0x1
	$-29     >|.  75 68         jnz short MHOClien.00FE6A1C
	$-27     >|.  A9 00100000   test eax,0x1000
	$-22     >|.  75 61         jnz short MHOClien.00FE6A1C
	$-20     >|.  837E 18 FF    cmp dword ptr ds:[esi+0x18],-0x1
	$-1C     >|.  7C 5B         jl short MHOClien.00FE6A1C
	$-1A     >|.  8B56 10       mov edx,dword ptr ds:[esi+0x10]
	$-17     >|.  57            push edi
	$-16     >|.  8B7A 08       mov edi,dword ptr ds:[edx+0x8]
	$-13     >|.  68 60F1A001   push MHOClien.01A0F160
	$-E      >|.  FF15 08723A01 call dword ptr ds:[0x13A7208]            ;  ntdll_12.RtlEnterCriticalSection
	$-8      >|.  8BBF 783F0000 mov edi,dword ptr ds:[edi+0x3F78]
	$-2      >|.  3BFE          cmp edi,esi
	$ ==>    >|.  74 33         je short MHOClien.00FE6A10
	$+2      >|.  837E 54 00    cmp dword ptr ds:[esi+0x54],0x0
	$+6      >|.  75 2D         jnz short MHOClien.00FE6A10
	$+8      >|.  837E 58 00    cmp dword ptr ds:[esi+0x58],0x0
	$+C      >|.  75 27         jnz short MHOClien.00FE6A10
	$+E      >|.  8B45 08       mov eax,[arg.1]
	$+11     >|.  8378 58 00    cmp dword ptr ds:[eax+0x58],0x0

	7433837E5400752D837E58007527


	*/

	DWORD	dwMMOClientUI_OFFSET6 = *scan_exe_.ScanImme<DWORD>("7433837E5400752D837E58007527", -0x6);
	assert(dwMMOClientUI_OFFSET6 == 0x3F78);
	ofs << "#define UPDATES_REVERSAL_UI_OFFSET6  " << dwMMOClientUI_OFFSET6 << "  // " << dwMMOClientUI_OFFSET6 << " MHOClient.exe" << FC_OFstream_EndlLog;



	/*
	$-D      >    55            push ebp
	$-C      >    8BEC          mov ebp,esp
	$-A      >|.  8B45 0C       mov eax,[arg.2]
	$-7      >|.  8B55 08       mov edx,[arg.1]
	$-4      >|.  3BC2          cmp eax,edx
	$-2      >|.  72 10         jb short MHOClien.0107AC3D
	$ ==>    >|.  8B49 08       mov ecx,dword ptr ds:[ecx+0x8]
	$+3      >|.  2BC2          sub eax,edx
	$+5      >|.  50            push eax
	$+6      >|.  52            push edx
	$+7      >|.  E8 A7F40000   call MHOClien.0108A0E0
	$+C      >|.  5D            pop ebp
	$+D      >|.  C2 0800       retn 0x8
	$+10     >|>  8B49 08       mov ecx,dword ptr ds:[ecx+0x8]
	$+13     >|.  33C0          xor eax,eax
	$+15     >|.  50            push eax
	$+16     >|.  52            push edx
	$+17     >|.  E8 97F40000   call MHOClien.0108A0E0
	$+1C     >|.  5D            pop ebp
	$+1D     >\.  C2 0800       retn 0x8

	8B 49 08 2B C2 50 52



	*/


	DWORD	dwMMOUPDATES_REVERSAL_UI_CLAENSTRING_CALL = (DWORD)scan_exe_.ScanCall("8B49082BC25052", 8);
	assert(dwMMOUPDATES_REVERSAL_UI_CLAENSTRING_CALL);
	ofs << "#define UPDATES_REVERSAL_UI_CLAENSTRING_CALL  " << dwMMOUPDATES_REVERSAL_UI_CLAENSTRING_CALL << "  // " << dwMMOUPDATES_REVERSAL_UI_CLAENSTRING_CALL << " MHOClient.exe" << FC_OFstream_EndlLog;

	ofs << "/**************************************特征码更新结束*******************************************/";
	LOG_O(Log_trace) << "ScanCode_Updates_Old更新完毕";
}
