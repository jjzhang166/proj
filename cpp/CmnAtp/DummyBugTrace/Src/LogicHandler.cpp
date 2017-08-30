#include "stdafx.h"
#include "LogicHandler.h"

void* g_pCheckLoginFailed = nullptr;

void SetInitParam(const stAtpDummyBugTrace& param)
{
	GlobalSetter::GetMe().params_ = param;
#ifndef _LOG_OUTPUT_EMPTY
	auto res = 
#endif
		InitCodeHooker(param);
#ifndef _LOG_OUTPUT_EMPTY
	if (!res)
	{
		OutputDebugString(_T("flog:InitCodeHooker error"));
		assert(false);
	}
#endif
}

void AsmInfo()
{
	//stAtpDummyBugTrace.h_safe_check_done_。SecureCheck.dll

	//下面这个特征码是足球时候的

	//8b??????????bb????????8d????????????8b????83????0f??????????73??68????????68????????ff
	/*
	$ ==>    6542E3DE      8B3D 10B34465                MOV EDI, DWORD PTR DS:[<&MSVCR90.wcsncpy>; MSVCR90.wcsncpy
	$+6      6542E3E4      BB 01000000                  MOV EBX, 1
	$+B      6542E3E9      8DA424 00000000              LEA ESP, DWORD PTR SS:[ESP]
	$+12     6542E3F0      8B46 18                      MOV EAX, DWORD PTR DS:[ESI+18]
	$+15     6542E3F3      83F8 5F                      CMP EAX, 5F
	$+18     6542E3F6      0F84 A9000000                JE SecureCh.6542E4A5
	$+1E     6542E3FC      73 1A                        JNB SHORT SecureCh.6542E418
	$+20     6542E3FE      68 E0D44465                  PUSH SecureCh.6544D4E0                   ; UNICODE "正在取消扫描"
	$+25     6542E403      68 20C04565                  PUSH SecureCh.6545C020                   ; UNICODE "安全检查进行中..."
	$+2A     6542E408      FF15 D4B24465                CALL DWORD PTR DS:[<&MSVCR90._wcsicmp>]  ; MSVCR90._wcsicmp
	$+30     6542E40E      83C4 08                      ADD ESP, 8
	$+33     6542E411      85C0                         TEST EAX, EAX
	$+35     6542E413      74 03                        JE SHORT SecureCh.6542E418
	$+37     6542E415      015E 18                      ADD DWORD PTR DS:[ESI+18], EBX
	$+3A     6542E418      8B4E 18                      MOV ECX, DWORD PTR DS:[ESI+18]
	$+3D     6542E41B      68 04010000                  PUSH 104
	$+42     6542E420      8D5424 24                    LEA EDX, DWORD PTR SS:[ESP+24]
	$+46     6542E424      68 20C04565                  PUSH SecureCh.6545C020                   ; UNICODE "安全检查进行中..."
	$+4B     6542E429      52                           PUSH EDX
	$+4C     6542E42A      894C24 24                    MOV DWORD PTR SS:[ESP+24], ECX
	$+50     6542E42E      C74424 28 64000000           MOV DWORD PTR SS:[ESP+28], 64
	$+58     6542E436      FFD7                         CALL EDI
	$+5A     6542E438      83C4 0C                      ADD ESP, 0C
	$+5D     6542E43B      837E 04 00                   CMP DWORD PTR DS:[ESI+4], 0
	$+61     6542E43F      74 4F                        JE SHORT SecureCh.6542E490
	$+63     6542E441      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
	$+66     6542E444      6A FF                        PUSH -1
	$+68     6542E446      50                           PUSH EAX
	$+69     6542E447      FF15 28B04465                CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; kernel32.WaitForSingleObject
	$+6F     6542E44D      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
	$+73     6542E451      75 3D                        JNZ SHORT SecureCh.6542E490
	$+75     6542E453      8B4C24 18                    MOV ECX, DWORD PTR SS:[ESP+18]
	$+79     6542E457      3B4C24 1C                    CMP ECX, DWORD PTR SS:[ESP+1C]
	$+7D     6542E45B      75 17                        JNZ SHORT SecureCh.6542E474
	$+7F     6542E45D      68 04010000                  PUSH 104
	$+84     6542E462      8D5424 24                    LEA EDX, DWORD PTR SS:[ESP+24]
	$+88     6542E466      68 D0D44465                  PUSH SecureCh.6544D4D0                   ; UNICODE "安全检查完成"
	$+8D     6542E46B      52                           PUSH EDX
	$+8E     6542E46C      895E 08                      MOV DWORD PTR DS:[ESI+8], EBX
	$+91     6542E46F      FFD7                         CALL EDI
	$+93     6542E471      83C4 0C                      ADD ESP, 0C
	$+96     6542E474      8B4E 1C                      MOV ECX, DWORD PTR DS:[ESI+1C]
	$+99     6542E477      85C9                         TEST ECX, ECX
	$+9B     6542E479      74 0B                        JE SHORT SecureCh.6542E486
	$+9D     6542E47B      8B01                         MOV EAX, DWORD PTR DS:[ECX]
	$+9F     6542E47D      8B00                         MOV EAX, DWORD PTR DS:[EAX]
	$+A1     6542E47F      8D5424 10                    LEA EDX, DWORD PTR SS:[ESP+10]
	$+A5     6542E483      52                           PUSH EDX
	$+A6     6542E484      FFD0                         CALL EAX
	$+A8     6542E486      8B4E 14                      MOV ECX, DWORD PTR DS:[ESI+14]
	$+AB     6542E489      51                           PUSH ECX
	$+AC     6542E48A      FF15 2CB04465                CALL DWORD PTR DS:[<&KERNEL32.SetEvent>] ; kernel32.SetEvent
	$+B2     6542E490      68 04010000                  PUSH 104
	$+B7     6542E495      FF15 60B14465                CALL DWORD PTR DS:[<&KERNEL32.Sleep>]    ; kernel32.Sleep
	$+BD     6542E49B      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
	$+C1     6542E49F    ^ 0F84 4BFFFFFF                JE SecureCh.6542E3F0
	$+C7     6542E4A5      8B8C24 34040000              MOV ECX, DWORD PTR SS:[ESP+434]
	$+CE     6542E4AC      5F                           POP EDI                                  ; 03C978F7
	$+CF     6542E4AD      5E                           POP ESI                                  ; 03C978F7
	$+D0     6542E4AE      5B                           POP EBX                                  ; 03C978F7
	$+D1     6542E4AF      33CC                         XOR ECX, ESP
	$+D3     6542E4B1      33C0                         XOR EAX, EAX
	$+D5     6542E4B3      E8 E0880100                  CALL SecureCh.65446D98
	$+DA     6542E4B8      8BE5                         MOV ESP, EBP
	$+DC     6542E4BA      5D                           POP EBP                                  ; 03C978F7
	$+DD     6542E4BB      C2 0400                      RETN 4
	$+E0     6542E4BE      CC                           INT3
	$+E1     6542E4BF      CC                           INT3
	$+E2     6542E4C0      837E 04 00                   CMP DWORD PTR DS:[ESI+4], 0
	$+E6     6542E4C4      74 54                        JE SHORT SecureCh.6542E51A
	$+E8     6542E4C6      85FF                         TEST EDI, EDI
	$+EA     6542E4C8      74 50                        JE SHORT SecureCh.6542E51A
	$+EC     6542E4CA      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
	$+EF     6542E4CD      6A FF                        PUSH -1
	$+F1     6542E4CF      50                           PUSH EAX
	$+F2     6542E4D0      FF15 28B04465                CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; kernel32.WaitForSingleObject
	$+F8     6542E4D6      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
	$+FC     6542E4DA      75 3E                        JNZ SHORT SecureCh.6542E51A
	$+FE     6542E4DC      8B4F 08                      MOV ECX, DWORD PTR DS:[EDI+8]
	$+101    6542E4DF      3B4F 0C                      CMP ECX, DWORD PTR DS:[EDI+C]
	$+104    6542E4E2      75 1E                        JNZ SHORT SecureCh.6542E502
	$+106    6542E4E4      68 04010000                  PUSH 104
	$+10B    6542E4E9      8D57 10                      LEA EDX, DWORD PTR DS:[EDI+10]
	$+10E    6542E4EC      68 D0D44465                  PUSH SecureCh.6544D4D0                   ; UNICODE "安全检查完成"
	$+113    6542E4F1      52                           PUSH EDX
	$+114    6542E4F2      C746 08 01000000             MOV DWORD PTR DS:[ESI+8], 1
	$+11B    6542E4F9      FF15 10B34465                CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]   ; MSVCR90.wcsncpy
	$+121    6542E4FF      83C4 0C                      ADD ESP, 0C								;要hook这里
	$+124    6542E502      8B4E 1C                      MOV ECX, DWORD PTR DS:[ESI+1C]
	$+127    6542E505      85C9                         TEST ECX, ECX
	$+129    6542E507      74 07                        JE SHORT SecureCh.6542E510
	$+12B    6542E509      8B01                         MOV EAX, DWORD PTR DS:[ECX]
	$+12D    6542E50B      8B10                         MOV EDX, DWORD PTR DS:[EAX]
	$+12F    6542E50D      57                           PUSH EDI
	$+130    6542E50E      FFD2                         CALL EDX
	$+132    6542E510      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
	$+135    6542E513      50                           PUSH EAX
	$+136    6542E514      FF15 2CB04465                CALL DWORD PTR DS:[<&KERNEL32.SetEvent>] ; kernel32.SetEvent
	$+13C    6542E51A      C3                           RETN
	*/
	//fifa 2017/8/10 19:54

	//83????8b????85??74??8b??8b??57ff??8b????50
	/*
	$-310    5E93DBAF      CC                    INT3
	$-30F    5E93DBB0      53                    PUSH EBX
	$-30E    5E93DBB1      56                    PUSH ESI
	$-30D    5E93DBB2      8BF1                  MOV ESI,ECX
	$-30B    5E93DBB4      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-309    5E93DBB6      8B50 0C               MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-306    5E93DBB9      57                    PUSH EDI
	$-305    5E93DBBA      33FF                  XOR EDI,EDI
	$-303    5E93DBBC      897E 04               MOV DWORD PTR DS:[ESI+0x4],EDI
	$-300    5E93DBBF      FFD2                  CALL EDX
	$-2FE    5E93DBC1      8B1D B4B0965E         MOV EBX,DWORD PTR DS:[<&KERNEL32.CloseHandle>]         ; kernel_2.CloseHandle
	$-2F8    5E93DBC7      397E 0C               CMP DWORD PTR DS:[ESI+0xC],EDI
	$-2F5    5E93DBCA      74 14                 JE SHORT SecureCh.5E93DBE0
	$-2F3    5E93DBCC      68 F4010000           PUSH 0x1F4
	$-2EE    5E93DBD1      FF15 60B1965E         CALL DWORD PTR DS:[<&KERNEL32.Sleep>]                  ; kernel_2.Sleep
	$-2E8    5E93DBD7      8B46 0C               MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-2E5    5E93DBDA      50                    PUSH EAX
	$-2E4    5E93DBDB      FFD3                  CALL EBX
	$-2E2    5E93DBDD      897E 0C               MOV DWORD PTR DS:[ESI+0xC],EDI
	$-2DF    5E93DBE0      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-2DC    5E93DBE3      3BCF                  CMP ECX,EDI
	$-2DA    5E93DBE5      74 0C                 JE SHORT SecureCh.5E93DBF3
	$-2D8    5E93DBE7      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-2D6    5E93DBE9      8B42 1C               MOV EAX,DWORD PTR DS:[EDX+0x1C]
	$-2D3    5E93DBEC      6A 01                 PUSH 0x1
	$-2D1    5E93DBEE      FFD0                  CALL EAX
	$-2CF    5E93DBF0      897E 20               MOV DWORD PTR DS:[ESI+0x20],EDI
	$-2CC    5E93DBF3      8B46 24               MOV EAX,DWORD PTR DS:[ESI+0x24]
	$-2C9    5E93DBF6      3BC7                  CMP EAX,EDI
	$-2C7    5E93DBF8      74 15                 JE SHORT SecureCh.5E93DC0F
	$-2C5    5E93DBFA      50                    PUSH EAX
	$-2C4    5E93DBFB      C700 2018975E         MOV DWORD PTR DS:[EAX],SecureCh.5E971820
	$-2BE    5E93DC01      8978 04               MOV DWORD PTR DS:[EAX+0x4],EDI
	$-2BB    5E93DC04      E8 DD980200           CALL <JMP.&MSVCR90.operator delete>
	$-2B6    5E93DC09      83C4 04               ADD ESP,0x4
	$-2B3    5E93DC0C      897E 24               MOV DWORD PTR DS:[ESI+0x24],EDI
	$-2B0    5E93DC0F      8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-2AD    5E93DC12      897E 18               MOV DWORD PTR DS:[ESI+0x18],EDI
	$-2AA    5E93DC15      897E 08               MOV DWORD PTR DS:[ESI+0x8],EDI
	$-2A7    5E93DC18      897E 0C               MOV DWORD PTR DS:[ESI+0xC],EDI
	$-2A4    5E93DC1B      897E 10               MOV DWORD PTR DS:[ESI+0x10],EDI
	$-2A1    5E93DC1E      897E 1C               MOV DWORD PTR DS:[ESI+0x1C],EDI
	$-29E    5E93DC21      3BC7                  CMP EAX,EDI
	$-29C    5E93DC23      74 06                 JE SHORT SecureCh.5E93DC2B
	$-29A    5E93DC25      50                    PUSH EAX
	$-299    5E93DC26      FFD3                  CALL EBX
	$-297    5E93DC28      897E 14               MOV DWORD PTR DS:[ESI+0x14],EDI
	$-294    5E93DC2B      5F                    POP EDI
	$-293    5E93DC2C      5E                    POP ESI
	$-292    5E93DC2D      B8 01000000           MOV EAX,0x1
	$-28D    5E93DC32      5B                    POP EBX
	$-28C    5E93DC33      C3                    RETN
	$-28B    5E93DC34      CC                    INT3
	$-28A    5E93DC35      CC                    INT3
	$-289    5E93DC36      CC                    INT3
	$-288    5E93DC37      CC                    INT3
	$-287    5E93DC38      CC                    INT3
	$-286    5E93DC39      CC                    INT3
	$-285    5E93DC3A      CC                    INT3
	$-284    5E93DC3B      CC                    INT3
	$-283    5E93DC3C      CC                    INT3
	$-282    5E93DC3D      CC                    INT3
	$-281    5E93DC3E      CC                    INT3
	$-280    5E93DC3F      CC                    INT3
	$-27F    5E93DC40      56                    PUSH ESI
	$-27E    5E93DC41      8BF1                  MOV ESI,ECX
	$-27C    5E93DC43      33C0                  XOR EAX,EAX
	$-27A    5E93DC45      3946 20               CMP DWORD PTR DS:[ESI+0x20],EAX
	$-277    5E93DC48      74 44                 JE SHORT SecureCh.5E93DC8E
	$-275    5E93DC4A      68 04010000           PUSH 0x104
	$-270    5E93DC4F      68 E815975E           PUSH SecureCh.5E9715E8                                 ; UNICODE "安全检查进行中..."
	$-26B    5E93DC54      68 00F1975E           PUSH SecureCh.5E97F100                                 ; UNICODE "安全检查进行中..."
	$-266    5E93DC59      FF15 1CB3965E         CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$-260    5E93DC5F      83C4 0C               ADD ESP,0xC
	$-25D    5E93DC62      837E 0C 00            CMP DWORD PTR DS:[ESI+0xC],0x0
	$-259    5E93DC66      75 17                 JNZ SHORT SecureCh.5E93DC7F
	$-257    5E93DC68      6A 00                 PUSH 0x0
	$-255    5E93DC6A      6A 00                 PUSH 0x0
	$-253    5E93DC6C      56                    PUSH ESI
	$-252    5E93DC6D      68 50DD935E           PUSH SecureCh.5E93DD50
	$-24D    5E93DC72      6A 00                 PUSH 0x0
	$-24B    5E93DC74      6A 00                 PUSH 0x0
	$-249    5E93DC76      FF15 C0B0965E         CALL DWORD PTR DS:[<&KERNEL32.CreateThread>]           ; kernel_2.CreateThread
	$-243    5E93DC7C      8946 0C               MOV DWORD PTR DS:[ESI+0xC],EAX
	$-240    5E93DC7F      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-23D    5E93DC82      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-23B    5E93DC84      8B50 04               MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-238    5E93DC87      FFD2                  CALL EDX
	$-236    5E93DC89      B8 01000000           MOV EAX,0x1
	$-231    5E93DC8E      5E                    POP ESI
	$-230    5E93DC8F      C2 0400               RETN 0x4
	$-22D    5E93DC92      CC                    INT3
	$-22C    5E93DC93      CC                    INT3
	$-22B    5E93DC94      CC                    INT3
	$-22A    5E93DC95      CC                    INT3
	$-229    5E93DC96      CC                    INT3
	$-228    5E93DC97      CC                    INT3
	$-227    5E93DC98      CC                    INT3
	$-226    5E93DC99      CC                    INT3
	$-225    5E93DC9A      CC                    INT3
	$-224    5E93DC9B      CC                    INT3
	$-223    5E93DC9C      CC                    INT3
	$-222    5E93DC9D      CC                    INT3
	$-221    5E93DC9E      CC                    INT3
	$-220    5E93DC9F      CC                    INT3
	$-21F    5E93DCA0      56                    PUSH ESI
	$-21E    5E93DCA1      8BF1                  MOV ESI,ECX
	$-21C    5E93DCA3      33C0                  XOR EAX,EAX
	$-21A    5E93DCA5      3946 20               CMP DWORD PTR DS:[ESI+0x20],EAX
	$-217    5E93DCA8      74 2E                 JE SHORT SecureCh.5E93DCD8
	$-215    5E93DCAA      68 04010000           PUSH 0x104
	$-210    5E93DCAF      68 1016975E           PUSH SecureCh.5E971610                                 ; UNICODE "正在取消扫描"
	$-20B    5E93DCB4      68 00F1975E           PUSH SecureCh.5E97F100                                 ; UNICODE "安全检查进行中..."
	$-206    5E93DCB9      C746 08 01000000      MOV DWORD PTR DS:[ESI+0x8],0x1
	$-1FF    5E93DCC0      FF15 1CB3965E         CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$-1F9    5E93DCC6      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-1F6    5E93DCC9      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-1F4    5E93DCCB      8B50 0C               MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-1F1    5E93DCCE      83C4 0C               ADD ESP,0xC
	$-1EE    5E93DCD1      FFD2                  CALL EDX
	$-1EC    5E93DCD3      B8 01000000           MOV EAX,0x1
	$-1E7    5E93DCD8      5E                    POP ESI
	$-1E6    5E93DCD9      C3                    RETN
	$-1E5    5E93DCDA      CC                    INT3
	$-1E4    5E93DCDB      CC                    INT3
	$-1E3    5E93DCDC      CC                    INT3
	$-1E2    5E93DCDD      CC                    INT3
	$-1E1    5E93DCDE      CC                    INT3
	$-1E0    5E93DCDF      CC                    INT3
	$-1DF    5E93DCE0      8B49 20               MOV ECX,DWORD PTR DS:[ECX+0x20]
	$-1DC    5E93DCE3      B8 01000000           MOV EAX,0x1
	$-1D7    5E93DCE8      85C9                  TEST ECX,ECX
	$-1D5    5E93DCEA      74 16                 JE SHORT SecureCh.5E93DD02
	$-1D3    5E93DCEC      F681 98000000 04      TEST BYTE PTR DS:[ECX+0x98],0x4
	$-1CC    5E93DCF3      74 07                 JE SHORT SecureCh.5E93DCFC
	$-1CA    5E93DCF5      8389 3C060000 08      OR DWORD PTR DS:[ECX+0x63C],0x8
	$-1C3    5E93DCFC      8B81 3C060000         MOV EAX,DWORD PTR DS:[ECX+0x63C]
	$-1BD    5E93DD02      C3                    RETN
	$-1BC    5E93DD03      CC                    INT3
	$-1BB    5E93DD04      CC                    INT3
	$-1BA    5E93DD05      CC                    INT3
	$-1B9    5E93DD06      CC                    INT3
	$-1B8    5E93DD07      CC                    INT3
	$-1B7    5E93DD08      CC                    INT3
	$-1B6    5E93DD09      CC                    INT3
	$-1B5    5E93DD0A      CC                    INT3
	$-1B4    5E93DD0B      CC                    INT3
	$-1B3    5E93DD0C      CC                    INT3
	$-1B2    5E93DD0D      CC                    INT3
	$-1B1    5E93DD0E      CC                    INT3
	$-1B0    5E93DD0F      CC                    INT3
	$-1AF    5E93DD10      8B49 20               MOV ECX,DWORD PTR DS:[ECX+0x20]
	$-1AC    5E93DD13      85C9                  TEST ECX,ECX
	$-1AA    5E93DD15      74 35                 JE SHORT SecureCh.5E93DD4C
	$-1A8    5E93DD17      8B4424 08             MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-1A4    5E93DD1B      85C0                  TEST EAX,EAX
	$-1A2    5E93DD1D      75 1C                 JNZ SHORT SecureCh.5E93DD3B
	$-1A0    5E93DD1F      8B81 40060000         MOV EAX,DWORD PTR DS:[ECX+0x640]
	$-19A    5E93DD25      8B91 44060000         MOV EDX,DWORD PTR DS:[ECX+0x644]
	$-194    5E93DD2B      B1 20                 MOV CL,0x20
	$-192    5E93DD2D      E8 1EBA0200           CALL SecureCh.5E969750
	$-18D    5E93DD32      8B4C24 04             MOV ECX,DWORD PTR SS:[ESP+0x4]
	$-189    5E93DD36      8901                  MOV DWORD PTR DS:[ECX],EAX
	$-187    5E93DD38      C2 0800               RETN 0x8
	$-184    5E93DD3B      83F8 01               CMP EAX,0x1
	$-181    5E93DD3E      75 0C                 JNZ SHORT SecureCh.5E93DD4C
	$-17F    5E93DD40      8B91 40060000         MOV EDX,DWORD PTR DS:[ECX+0x640]
	$-179    5E93DD46      8B4424 04             MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-175    5E93DD4A      8910                  MOV DWORD PTR DS:[EAX],EDX
	$-173    5E93DD4C      C2 0800               RETN 0x8
	$-170    5E93DD4F      CC                    INT3
	$-16F    5E93DD50      55                    PUSH EBP
	$-16E    5E93DD51      8BEC                  MOV EBP,ESP
	$-16C    5E93DD53      83E4 F8               AND ESP,0xFFFFFFF8
	$-169    5E93DD56      81EC 2C040000         SUB ESP,0x42C
	$-163    5E93DD5C      A1 9070975E           MOV EAX,DWORD PTR DS:[0x5E977090]
	$-15E    5E93DD61      33C4                  XOR EAX,ESP
	$-15C    5E93DD63      898424 28040000       MOV DWORD PTR SS:[ESP+0x428],EAX
	$-155    5E93DD6A      53                    PUSH EBX
	$-154    5E93DD6B      56                    PUSH ESI
	$-153    5E93DD6C      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-150    5E93DD6F      57                    PUSH EDI
	$-14F    5E93DD70      68 1C040000           PUSH 0x41C
	$-14A    5E93DD75      8D4424 18             LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-146    5E93DD79      6A 00                 PUSH 0x0
	$-144    5E93DD7B      50                    PUSH EAX
	$-143    5E93DD7C      C74424 1C 00000000    MOV DWORD PTR SS:[ESP+0x1C],0x0
	$-13B    5E93DD84      E8 579C0200           CALL <JMP.&MSVCR90.memset>
	$-136    5E93DD89      83C4 0C               ADD ESP,0xC
	$-133    5E93DD8C      85F6                  TEST ESI,ESI
	$-131    5E93DD8E      0F84 D1000000         JE SecureCh.5E93DE65
	$-12B    5E93DD94      837E 08 00            CMP DWORD PTR DS:[ESI+0x8],0x0
	$-127    5E93DD98      0F85 C7000000         JNZ SecureCh.5E93DE65
	$-121    5E93DD9E      8B3D 1CB3965E         MOV EDI,DWORD PTR DS:[<&MSVCR90.wcsncpy>]              ; msvcr90.wcsncpy
	$-11B    5E93DDA4      BB 01000000           MOV EBX,0x1
	$-116    5E93DDA9      8DA424 00000000       LEA ESP,DWORD PTR SS:[ESP]
	$-10F    5E93DDB0      8B46 18               MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-10C    5E93DDB3      83F8 5F               CMP EAX,0x5F
	$-109    5E93DDB6      0F84 A9000000         JE SecureCh.5E93DE65
	$-103    5E93DDBC      73 1A                 JNB SHORT SecureCh.5E93DDD8
	$-101    5E93DDBE      68 1016975E           PUSH SecureCh.5E971610                                 ; UNICODE "正在取消扫描"
	$-FC     5E93DDC3      68 00F1975E           PUSH SecureCh.5E97F100                                 ; UNICODE "安全检查进行中..."
	$-F7     5E93DDC8      FF15 DCB2965E         CALL DWORD PTR DS:[<&MSVCR90._wcsicmp>]                ; msvcr90._wcsicmp
	$-F1     5E93DDCE      83C4 08               ADD ESP,0x8
	$-EE     5E93DDD1      85C0                  TEST EAX,EAX
	$-EC     5E93DDD3      74 03                 JE SHORT SecureCh.5E93DDD8
	$-EA     5E93DDD5      015E 18               ADD DWORD PTR DS:[ESI+0x18],EBX
	$-E7     5E93DDD8      8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$-E4     5E93DDDB      68 04010000           PUSH 0x104
	$-DF     5E93DDE0      8D5424 24             LEA EDX,DWORD PTR SS:[ESP+0x24]
	$-DB     5E93DDE4      68 00F1975E           PUSH SecureCh.5E97F100                                 ; UNICODE "安全检查进行中..."
	$-D6     5E93DDE9      52                    PUSH EDX
	$-D5     5E93DDEA      894C24 24             MOV DWORD PTR SS:[ESP+0x24],ECX
	$-D1     5E93DDEE      C74424 28 64000000    MOV DWORD PTR SS:[ESP+0x28],0x64
	$-C9     5E93DDF6      FFD7                  CALL EDI
	$-C7     5E93DDF8      83C4 0C               ADD ESP,0xC
	$-C4     5E93DDFB      837E 04 00            CMP DWORD PTR DS:[ESI+0x4],0x0
	$-C0     5E93DDFF      74 4F                 JE SHORT SecureCh.5E93DE50
	$-BE     5E93DE01      8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-BB     5E93DE04      6A FF                 PUSH -0x1
	$-B9     5E93DE06      50                    PUSH EAX
	$-B8     5E93DE07      FF15 34B0965E         CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]    ; kernel_2.WaitForSingleObject
	$-B2     5E93DE0D      837E 08 00            CMP DWORD PTR DS:[ESI+0x8],0x0
	$-AE     5E93DE11      75 3D                 JNZ SHORT SecureCh.5E93DE50
	$-AC     5E93DE13      8B4C24 18             MOV ECX,DWORD PTR SS:[ESP+0x18]
	$-A8     5E93DE17      3B4C24 1C             CMP ECX,DWORD PTR SS:[ESP+0x1C]
	$-A4     5E93DE1B      75 17                 JNZ SHORT SecureCh.5E93DE34
	$-A2     5E93DE1D      68 04010000           PUSH 0x104
	$-9D     5E93DE22      8D5424 24             LEA EDX,DWORD PTR SS:[ESP+0x24]
	$-99     5E93DE26      68 0016975E           PUSH SecureCh.5E971600                                 ; UNICODE "安全检查完成"
	$-94     5E93DE2B      52                    PUSH EDX
	$-93     5E93DE2C      895E 08               MOV DWORD PTR DS:[ESI+0x8],EBX
	$-90     5E93DE2F      FFD7                  CALL EDI
	$-8E     5E93DE31      83C4 0C               ADD ESP,0xC
	$-8B     5E93DE34      8B4E 1C               MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$-88     5E93DE37      85C9                  TEST ECX,ECX
	$-86     5E93DE39      74 0B                 JE SHORT SecureCh.5E93DE46
	$-84     5E93DE3B      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-82     5E93DE3D      8B00                  MOV EAX,DWORD PTR DS:[EAX]
	$-80     5E93DE3F      8D5424 10             LEA EDX,DWORD PTR SS:[ESP+0x10]
	$-7C     5E93DE43      52                    PUSH EDX
	$-7B     5E93DE44      FFD0                  CALL EAX
	$-79     5E93DE46      8B4E 14               MOV ECX,DWORD PTR DS:[ESI+0x14]
	$-76     5E93DE49      51                    PUSH ECX
	$-75     5E93DE4A      FF15 38B0965E         CALL DWORD PTR DS:[<&KERNEL32.SetEvent>]               ; kernel_2.SetEvent
	$-6F     5E93DE50      68 04010000           PUSH 0x104
	$-6A     5E93DE55      FF15 60B1965E         CALL DWORD PTR DS:[<&KERNEL32.Sleep>]                  ; kernel_2.Sleep
	$-64     5E93DE5B      837E 08 00            CMP DWORD PTR DS:[ESI+0x8],0x0
	$-60     5E93DE5F    ^ 0F84 4BFFFFFF         JE SecureCh.5E93DDB0
	$-5A     5E93DE65      8B8C24 34040000       MOV ECX,DWORD PTR SS:[ESP+0x434]
	$-53     5E93DE6C      5F                    POP EDI
	$-52     5E93DE6D      5E                    POP ESI
	$-51     5E93DE6E      5B                    POP EBX
	$-50     5E93DE6F      33CC                  XOR ECX,ESP
	$-4E     5E93DE71      33C0                  XOR EAX,EAX
	$-4C     5E93DE73      E8 5E960200           CALL SecureCh.5E9674D6
	$-47     5E93DE78      8BE5                  MOV ESP,EBP
	$-45     5E93DE7A      5D                    POP EBP
	$-44     5E93DE7B      C2 0400               RETN 0x4
	$-41     5E93DE7E      CC                    INT3
	$-40     5E93DE7F      CC                    INT3
	$-3F     5E93DE80      837E 04 00            CMP DWORD PTR DS:[ESI+0x4],0x0
	$-3B     5E93DE84      74 54                 JE SHORT SecureCh.5E93DEDA
	$-39     5E93DE86      85FF                  TEST EDI,EDI
	$-37     5E93DE88      74 50                 JE SHORT SecureCh.5E93DEDA
	$-35     5E93DE8A      8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-32     5E93DE8D      6A FF                 PUSH -0x1
	$-30     5E93DE8F      50                    PUSH EAX
	$-2F     5E93DE90      FF15 34B0965E         CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]    ; kernel_2.WaitForSingleObject
	$-29     5E93DE96      837E 08 00            CMP DWORD PTR DS:[ESI+0x8],0x0
	$-25     5E93DE9A      75 3E                 JNZ SHORT SecureCh.5E93DEDA
	$-23     5E93DE9C      8B4F 08               MOV ECX,DWORD PTR DS:[EDI+0x8]
	$-20     5E93DE9F      3B4F 0C               CMP ECX,DWORD PTR DS:[EDI+0xC]
	$-1D     5E93DEA2      75 1E                 JNZ SHORT SecureCh.5E93DEC2
	$-1B     5E93DEA4      68 04010000           PUSH 0x104
	$-16     5E93DEA9      8D57 10               LEA EDX,DWORD PTR DS:[EDI+0x10]
	$-13     5E93DEAC      68 0016975E           PUSH SecureCh.5E971600                                 ; UNICODE "安全检查完成"
	$-E      5E93DEB1      52                    PUSH EDX
	$-D      5E93DEB2      C746 08 01000000      MOV DWORD PTR DS:[ESI+0x8],0x1
	$-6      5E93DEB9      FF15 1CB3965E         CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$ ==>    5E93DEBF      83C4 0C               ADD ESP,0xC                                            ; hook此处
	$+3      5E93DEC2      8B4E 1C               MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$+6      5E93DEC5      85C9                  TEST ECX,ECX
	$+8      5E93DEC7      74 07                 JE SHORT SecureCh.5E93DED0
	$+A      5E93DEC9      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+C      5E93DECB      8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$+E      5E93DECD      57                    PUSH EDI
	$+F      5E93DECE      FFD2                  CALL EDX
	$+11     5E93DED0      8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+14     5E93DED3      50                    PUSH EAX
	$+15     5E93DED4      FF15 38B0965E         CALL DWORD PTR DS:[<&KERNEL32.SetEvent>]               ; kernel_2.SetEvent
	$+1B     5E93DEDA      C3                    RETN
	$+1C     5E93DEDB      CC                    INT3
	$+1D     5E93DEDC      CC                    INT3
	$+1E     5E93DEDD      CC                    INT3
	$+1F     5E93DEDE      CC                    INT3
	$+20     5E93DEDF      CC                    INT3
	$+21     5E93DEE0    - E9 51FE0D00           JMP SecureCh.5EA1DD36
	$+26     5E93DEE5      6909 17D22D25         IMUL ECX,DWORD PTR DS:[ECX],0x252DD217
	$+2C     5E93DEEB      C2 D6B6               RETN 0xB6D6
	$+2F     5E93DEEE      06                    PUSH ES
	$+30     5E93DEEF      1913                  SBB DWORD PTR DS:[EBX],EDX
	$+32     5E93DEF1      D22E                  SHR BYTE PTR DS:[ESI],CL
	$+34     5E93DEF3      10ED                  ADC CH,CH
	$+36     5E93DEF5      FFA3 718119D2         JMP DWORD PTR DS:[EBX+0xD2198171]
	$+3C     5E93DEFB      2D 960517D2           SUB EAX,0xD2170596
	$+41     5E93DF00      26:A9 5A87B6AD        TEST EAX,0xADB6875A
	$+47     5E93DF06      60                    PUSHAD
	$+48     5E93DF07      D3D6                  RCL ESI,CL
	$+4A     5E93DF09      2D 655407D2           SUB EAX,0xD2075465
	$+4F     5E93DF0E      2E:9F                 LAHF
	$+51     5E93DF10      D005 D2264FC2         ROL BYTE PTR DS:[0xC24F26D2],1
	$+57     5E93DF16      84B6 BA92CCD6         TEST BYTE PTR DS:[ESI+0xD6CC92BA],DH
	$+5D     5E93DF1C      2E:A1 D661DACA        MOV EAX,DWORD PTR CS:[0xCADA61D6]
	$+63     5E93DF22      54                    PUSH ESP
	$+64     5E93DF23      CD E3                 INT 0xE3
	$+66     5E93DF25      0948 14               OR DWORD PTR DS:[EAX+0x14],ECX
	$+69     5E93DF28      D233                  SAL BYTE PTR DS:[EBX],CL
	$+6B     5E93DF2A      A9 5A87B6E6           TEST EAX,0xE6B6875A
	$+70     5E93DF2F      E6 CA                 OUT 0xCA,AL
	$+72     5E93DF31      D6                    SALC
	$+73     5E93DF32      33E8                  XOR EBP,EAX
	$+75     5E93DF34      F9                    STC
	$+76     5E93DF35      69DA F1B7CC63         IMUL EBX,EDX,0x63CCB7F1
	$+7C     5E93DF3B      41                    INC ECX
	$+7D     5E93DF3C      2C 6C                 SUB AL,0x6C
	$+7F     5E93DF3E      DA9F 188CC600         FICOMP DWORD PTR DS:[EDI+0xC68C18]
	$+85     5E93DF44      0000                  ADD BYTE PTR DS:[EAX],AL
	$+87     5E93DF46      007E 68               ADD BYTE PTR DS:[ESI+0x68],BH
	$+8A     5E93DF49      CF                    IRETD
	$+8B     5E93DF4A      46                    INC ESI
	$+8C     5E93DF4B      29AA 51E8B5C3         SUB DWORD PTR DS:[EDX+0xC3B5E851],EBP
	$+92     5E93DF51      0F00E8                VERW AX
	$+95     5E93DF54      8D6424 04             LEA ESP,DWORD PTR SS:[ESP+0x4]
	$+99     5E93DF58      56                    PUSH ESI
	$+9A     5E93DF59      9C                    PUSHFD
	$+9B     5E93DF5A      55                    PUSH EBP
	$+9C     5E93DF5B      57                    PUSH EDI
	$+9D     5E93DF5C    - E9 4E320700           JMP SecureCh.5E9B11AF
	$+A2     5E93DF61      7A 89                 JPE SHORT SecureCh.5E93DEEC
	$+A4     5E93DF63      1C 24                 SBB AL,0x24
	$+A6     5E93DF65      51                    PUSH ECX
	$+A7     5E93DF66      8B8C24 04000000       MOV ECX,DWORD PTR SS:[ESP+0x4]
	$+AE     5E93DF6D      9C                    PUSHFD
	$+AF     5E93DF6E      9C                    PUSHFD
	$+B0     5E93DF6F      55                    PUSH EBP
	$+B1     5E93DF70      51                    PUSH ECX
	$+B2     5E93DF71      8B8C24 10000000       MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+B9     5E93DF78      C2 1400               RETN 0x14
	$+BC     5E93DF7B    ^ 74 8D                 JE SHORT SecureCh.5E93DF0A
	$+BE     5E93DF7D      64:24 04              AND AL,0x4
	$+C1     5E93DF80      9C                    PUSHFD
	$+C2     5E93DF81      55                    PUSH EBP
	$+C3     5E93DF82      E8 5D5D0800           CALL SecureCh.5E9C3CE4
	$+C8     5E93DF87      78 57                 JS SHORT SecureCh.5E93DFE0
	$+CA     5E93DF89    - E9 21320700           JMP SecureCh.5E9B11AF
	$+CF     5E93DF8E      E8 CC578B78           CALL D71F375F
	$+D4     5E93DF93      2085 FF743D83         AND BYTE PTR SS:[EBP+0x833D74FF],AL
	$+DA     5E93DF99      7F 28                 JG SHORT SecureCh.5E93DFC3
	$+DC     5E93DF9B      0075 37               ADD BYTE PTR SS:[EBP+0x37],DH
	$+DF     5E93DF9E      837F 24 00            CMP DWORD PTR DS:[EDI+0x24],0x0
	$+E3     5E93DFA2      75 31                 JNZ SHORT SecureCh.5E93DFD5
	$+E5     5E93DFA4      68 38090000           PUSH 0x938
	$+EA     5E93DFA9      E8 3E950200           CALL <JMP.&MSVCR90.operator new>
	$+EF     5E93DFAE      83C4 04               ADD ESP,0x4
	$+F2     5E93DFB1      85C0                  TEST EAX,EAX
	$+F4     5E93DFB3      74 0B                 JE SHORT SecureCh.5E93DFC0
	$+F6     5E93DFB5      56                    PUSH ESI
	$+F7     5E93DFB6      8BF0                  MOV ESI,EAX
	$+F9     5E93DFB8      E8 730C0000           CALL SecureCh.5E93EC30
	$+FE     5E93DFBD      5E                    POP ESI
	$+FF     5E93DFBE      EB 02                 JMP SHORT SecureCh.5E93DFC2
	$+101    5E93DFC0      33C0                  XOR EAX,EAX
	$+103    5E93DFC2      8947 24               MOV DWORD PTR DS:[EDI+0x24],EAX
	$+106    5E93DFC5      85C0                  TEST EAX,EAX
	$+108    5E93DFC7      74 0C                 JE SHORT SecureCh.5E93DFD5
	$+10A    5E93DFC9      6A 02                 PUSH 0x2
	$+10C    5E93DFCB      8BC8                  MOV ECX,EAX
	$+10E    5E93DFCD      E8 7F4DFFFF           CALL SecureCh.5E932D51
	$+113    5E93DFD2      8947 28               MOV DWORD PTR DS:[EDI+0x28],EAX
	$+116    5E93DFD5      5F                    POP EDI
	$+117    5E93DFD6      C3                    RETN
	$+118    5E93DFD7      CC                    INT3
	$+119    5E93DFD8      CC                    INT3
	$+11A    5E93DFD9      CC                    INT3
	$+11B    5E93DFDA      CC                    INT3
	$+11C    5E93DFDB      CC                    INT3
	$+11D    5E93DFDC      CC                    INT3
	$+11E    5E93DFDD      CC                    INT3
	$+11F    5E93DFDE      CC                    INT3
	$+120    5E93DFDF      CC                    INT3
	$+121    5E93DFE0      8379 20 00            CMP DWORD PTR DS:[ECX+0x20],0x0
	$+125    5E93DFE4      74 0A                 JE SHORT SecureCh.5E93DFF0
	$+127    5E93DFE6      8B49 20               MOV ECX,DWORD PTR DS:[ECX+0x20]
	$+12A    5E93DFE9      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+12C    5E93DFEB      8B40 28               MOV EAX,DWORD PTR DS:[EAX+0x28]
	$+12F    5E93DFEE      FFE0                  JMP EAX
	$+131    5E93DFF0      C2 0800               RETN 0x8
	$+134    5E93DFF3      CC                    INT3
	$+135    5E93DFF4      CC                    INT3
	$+136    5E93DFF5      CC                    INT3
	$+137    5E93DFF6      CC                    INT3
	$+138    5E93DFF7      CC                    INT3
	$+139    5E93DFF8      CC                    INT3
	$+13A    5E93DFF9      CC                    INT3
	$+13B    5E93DFFA      CC                    INT3
	$+13C    5E93DFFB      CC                    INT3
	$+13D    5E93DFFC      CC                    INT3
	$+13E    5E93DFFD      CC                    INT3
	$+13F    5E93DFFE      CC                    INT3
	$+140    5E93DFFF      CC                    INT3
	$+141    5E93E000      C2 0400               RETN 0x4
	$+144    5E93E003      CC                    INT3
	$+145    5E93E004      CC                    INT3
	$+146    5E93E005      CC                    INT3
	$+147    5E93E006      CC                    INT3
	$+148    5E93E007      CC                    INT3
	$+149    5E93E008      CC                    INT3
	$+14A    5E93E009      CC                    INT3
	$+14B    5E93E00A      CC                    INT3
	$+14C    5E93E00B      CC                    INT3
	$+14D    5E93E00C      CC                    INT3
	$+14E    5E93E00D      CC                    INT3
	$+14F    5E93E00E      CC                    INT3
	$+150    5E93E00F      CC                    INT3
	$+151    5E93E010      55                    PUSH EBP
	$+152    5E93E011      8BEC                  MOV EBP,ESP
	$+154    5E93E013      83E4 F8               AND ESP,0xFFFFFFF8
	$+157    5E93E016      81EC 28040000         SUB ESP,0x428
	$+15D    5E93E01C      A1 9070975E           MOV EAX,DWORD PTR DS:[0x5E977090]
	$+162    5E93E021      33C4                  XOR EAX,ESP
	$+164    5E93E023      898424 24040000       MOV DWORD PTR SS:[ESP+0x424],EAX
	$+16B    5E93E02A      56                    PUSH ESI
	$+16C    5E93E02B      8BF1                  MOV ESI,ECX
	$+16E    5E93E02D      837E 04 00            CMP DWORD PTR DS:[ESI+0x4],0x0
	$+172    5E93E031      57                    PUSH EDI
	$+173    5E93E032      74 4A                 JE SHORT SecureCh.5E93E07E
	$+175    5E93E034      68 1C040000           PUSH 0x41C
	$+17A    5E93E039      8D4424 10             LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+17E    5E93E03D      6A 00                 PUSH 0x0
	$+180    5E93E03F      50                    PUSH EAX
	$+181    5E93E040      C74424 14 00000000    MOV DWORD PTR SS:[ESP+0x14],0x0
	$+189    5E93E048      E8 93990200           CALL <JMP.&MSVCR90.memset>
	$+18E    5E93E04D      68 04010000           PUSH 0x104
	$+193    5E93E052      B8 64000000           MOV EAX,0x64
	$+198    5E93E057      8D4C24 28             LEA ECX,DWORD PTR SS:[ESP+0x28]
	$+19C    5E93E05B      68 0016975E           PUSH SecureCh.5E971600                                 ; UNICODE "安全检查完成"
	$+1A1    5E93E060      51                    PUSH ECX
	$+1A2    5E93E061      894424 28             MOV DWORD PTR SS:[ESP+0x28],EAX
	$+1A6    5E93E065      894424 2C             MOV DWORD PTR SS:[ESP+0x2C],EAX
	$+1AA    5E93E069      FF15 1CB3965E         CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$+1B0    5E93E06F      8B76 04               MOV ESI,DWORD PTR DS:[ESI+0x4]
	$+1B3    5E93E072      83C4 18               ADD ESP,0x18
	$+1B6    5E93E075      8D7C24 08             LEA EDI,DWORD PTR SS:[ESP+0x8]
	$+1BA    5E93E079      E8 02FEFFFF           CALL SecureCh.5E93DE80
	$+1BF    5E93E07E      8B8C24 2C040000       MOV ECX,DWORD PTR SS:[ESP+0x42C]
	$+1C6    5E93E085      5F                    POP EDI
	$+1C7    5E93E086      5E                    POP ESI
	$+1C8    5E93E087      33CC                  XOR ECX,ESP
	$+1CA    5E93E089      33C0                  XOR EAX,EAX
	$+1CC    5E93E08B      E8 46940200           CALL SecureCh.5E9674D6
	$+1D1    5E93E090      8BE5                  MOV ESP,EBP
	$+1D3    5E93E092      5D                    POP EBP
	$+1D4    5E93E093      C3                    RETN
	$+1D5    5E93E094      CC                    INT3
	$+1D6    5E93E095      CC                    INT3
	$+1D7    5E93E096      CC                    INT3
	$+1D8    5E93E097      CC                    INT3
	$+1D9    5E93E098      CC                    INT3
	$+1DA    5E93E099      CC                    INT3
	$+1DB    5E93E09A      CC                    INT3
	$+1DC    5E93E09B      CC                    INT3
	$+1DD    5E93E09C      CC                    INT3
	$+1DE    5E93E09D      CC                    INT3
	$+1DF    5E93E09E      CC                    INT3
	$+1E0    5E93E09F      CC                    INT3
	$+1E1    5E93E0A0      33C0                  XOR EAX,EAX
	$+1E3    5E93E0A2      C2 0800               RETN 0x8
	$+1E6    5E93E0A5      CC                    INT3
	$+1E7    5E93E0A6      CC                    INT3
	$+1E8    5E93E0A7      CC                    INT3
	$+1E9    5E93E0A8      CC                    INT3
	$+1EA    5E93E0A9      CC                    INT3
	$+1EB    5E93E0AA      CC                    INT3
	$+1EC    5E93E0AB      CC                    INT3
	$+1ED    5E93E0AC      CC                    INT3
	$+1EE    5E93E0AD      CC                    INT3
	$+1EF    5E93E0AE      CC                    INT3
	$+1F0    5E93E0AF      CC                    INT3
	$+1F1    5E93E0B0      8B41 04               MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+1F4    5E93E0B3      85C0                  TEST EAX,EAX
	$+1F6    5E93E0B5      74 12                 JE SHORT SecureCh.5E93E0C9
	$+1F8    5E93E0B7      8378 1C 00            CMP DWORD PTR DS:[EAX+0x1C],0x0
	$+1FC    5E93E0BB      74 0C                 JE SHORT SecureCh.5E93E0C9
	$+1FE    5E93E0BD      8B40 1C               MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$+201    5E93E0C0      8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$+203    5E93E0C2      8B52 0C               MOV EDX,DWORD PTR DS:[EDX+0xC]
	$+206    5E93E0C5      8BC8                  MOV ECX,EAX
	$+208    5E93E0C7      FFE2                  JMP EDX
	$+20A    5E93E0C9      C2 0800               RETN 0x8
	$+20D    5E93E0CC      CC                    INT3
	$+20E    5E93E0CD      CC                    INT3
	$+20F    5E93E0CE      CC                    INT3
	$+210    5E93E0CF      CC                    INT3
	$+211    5E93E0D0      8B41 04               MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+214    5E93E0D3      85C0                  TEST EAX,EAX
	$+216    5E93E0D5      74 12                 JE SHORT SecureCh.5E93E0E9
	$+218    5E93E0D7      8378 1C 00            CMP DWORD PTR DS:[EAX+0x1C],0x0
	$+21C    5E93E0DB      74 0C                 JE SHORT SecureCh.5E93E0E9
	$+21E    5E93E0DD      8B40 1C               MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$+221    5E93E0E0      8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$+223    5E93E0E2      8B52 10               MOV EDX,DWORD PTR DS:[EDX+0x10]
	$+226    5E93E0E5      8BC8                  MOV ECX,EAX
	$+228    5E93E0E7      FFE2                  JMP EDX
	$+22A    5E93E0E9      C2 0C00               RETN 0xC
	$+22D    5E93E0EC      CC                    INT3
	*/

	//下面这个特征码是流放之路的，2017/4/3 8:58

	//83????8b????85??74??8b??8b??57ff??8b????50ff??????????c3
	/*
	$-310    52AAE1EF     CC                       INT3
	$-30F    52AAE1F0     53                       PUSH EBX
	$-30E    52AAE1F1     56                       PUSH ESI
	$-30D    52AAE1F2     8BF1                     MOV ESI,ECX
	$-30B    52AAE1F4     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-309    52AAE1F6     8B50 0C                  MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-306    52AAE1F9     57                       PUSH EDI
	$-305    52AAE1FA     33FF                     XOR EDI,EDI
	$-303    52AAE1FC     897E 04                  MOV DWORD PTR DS:[ESI+0x4],EDI
	$-300    52AAE1FF     FFD2                     CALL EDX
	$-2FE    52AAE201     8B1D A8B0AC52            MOV EBX,DWORD PTR DS:[<&KERNEL32.CloseHandle>]         ; kernel_2.CloseHandle
	$-2F8    52AAE207     397E 0C                  CMP DWORD PTR DS:[ESI+0xC],EDI
	$-2F5    52AAE20A     74 14                    JE SHORT SecureCh.52AAE220
	$-2F3    52AAE20C     68 F4010000              PUSH 0x1F4
	$-2EE    52AAE211     FF15 60B1AC52            CALL DWORD PTR DS:[<&KERNEL32.Sleep>]                  ; kernel_2.Sleep
	$-2E8    52AAE217     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-2E5    52AAE21A     50                       PUSH EAX
	$-2E4    52AAE21B     FFD3                     CALL EBX
	$-2E2    52AAE21D     897E 0C                  MOV DWORD PTR DS:[ESI+0xC],EDI
	$-2DF    52AAE220     8B4E 20                  MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-2DC    52AAE223     3BCF                     CMP ECX,EDI
	$-2DA    52AAE225     74 0C                    JE SHORT SecureCh.52AAE233
	$-2D8    52AAE227     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-2D6    52AAE229     8B42 1C                  MOV EAX,DWORD PTR DS:[EDX+0x1C]
	$-2D3    52AAE22C     6A 01                    PUSH 0x1
	$-2D1    52AAE22E     FFD0                     CALL EAX
	$-2CF    52AAE230     897E 20                  MOV DWORD PTR DS:[ESI+0x20],EDI
	$-2CC    52AAE233     8B46 24                  MOV EAX,DWORD PTR DS:[ESI+0x24]
	$-2C9    52AAE236     3BC7                     CMP EAX,EDI
	$-2C7    52AAE238     74 15                    JE SHORT SecureCh.52AAE24F
	$-2C5    52AAE23A     50                       PUSH EAX
	$-2C4    52AAE23B     C700 C8D6AC52            MOV DWORD PTR DS:[EAX],SecureCh.52ACD6C8
	$-2BE    52AAE241     8978 04                  MOV DWORD PTR DS:[EAX+0x4],EDI
	$-2BB    52AAE244     E8 5F8B0100              CALL <JMP.&MSVCR90.operator delete>
	$-2B6    52AAE249     83C4 04                  ADD ESP,0x4
	$-2B3    52AAE24C     897E 24                  MOV DWORD PTR DS:[ESI+0x24],EDI
	$-2B0    52AAE24F     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-2AD    52AAE252     897E 18                  MOV DWORD PTR DS:[ESI+0x18],EDI
	$-2AA    52AAE255     897E 08                  MOV DWORD PTR DS:[ESI+0x8],EDI
	$-2A7    52AAE258     897E 0C                  MOV DWORD PTR DS:[ESI+0xC],EDI
	$-2A4    52AAE25B     897E 10                  MOV DWORD PTR DS:[ESI+0x10],EDI
	$-2A1    52AAE25E     897E 1C                  MOV DWORD PTR DS:[ESI+0x1C],EDI
	$-29E    52AAE261     3BC7                     CMP EAX,EDI
	$-29C    52AAE263     74 06                    JE SHORT SecureCh.52AAE26B
	$-29A    52AAE265     50                       PUSH EAX
	$-299    52AAE266     FFD3                     CALL EBX
	$-297    52AAE268     897E 14                  MOV DWORD PTR DS:[ESI+0x14],EDI
	$-294    52AAE26B     5F                       POP EDI
	$-293    52AAE26C     5E                       POP ESI
	$-292    52AAE26D     B8 01000000              MOV EAX,0x1
	$-28D    52AAE272     5B                       POP EBX
	$-28C    52AAE273     C3                       RETN
	$-28B    52AAE274     CC                       INT3
	$-28A    52AAE275     CC                       INT3
	$-289    52AAE276     CC                       INT3
	$-288    52AAE277     CC                       INT3
	$-287    52AAE278     CC                       INT3
	$-286    52AAE279     CC                       INT3
	$-285    52AAE27A     CC                       INT3
	$-284    52AAE27B     CC                       INT3
	$-283    52AAE27C     CC                       INT3
	$-282    52AAE27D     CC                       INT3
	$-281    52AAE27E     CC                       INT3
	$-280    52AAE27F     CC                       INT3
	$-27F    52AAE280     56                       PUSH ESI
	$-27E    52AAE281     8BF1                     MOV ESI,ECX
	$-27C    52AAE283     33C0                     XOR EAX,EAX
	$-27A    52AAE285     3946 20                  CMP DWORD PTR DS:[ESI+0x20],EAX
	$-277    52AAE288     74 44                    JE SHORT SecureCh.52AAE2CE
	$-275    52AAE28A     68 04010000              PUSH 0x104
	$-270    52AAE28F     68 B8D4AC52              PUSH SecureCh.52ACD4B8                                 ; UNICODE "安全检查进行中..."
	$-26B    52AAE294     68 20C0AD52              PUSH SecureCh.52ADC020                                 ; UNICODE "安全检查进行中..."
	$-266    52AAE299     FF15 10B3AC52            CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$-260    52AAE29F     83C4 0C                  ADD ESP,0xC
	$-25D    52AAE2A2     837E 0C 00               CMP DWORD PTR DS:[ESI+0xC],0x0
	$-259    52AAE2A6     75 17                    JNZ SHORT SecureCh.52AAE2BF
	$-257    52AAE2A8     6A 00                    PUSH 0x0
	$-255    52AAE2AA     6A 00                    PUSH 0x0
	$-253    52AAE2AC     56                       PUSH ESI
	$-252    52AAE2AD     68 90E3AA52              PUSH SecureCh.52AAE390
	$-24D    52AAE2B2     6A 00                    PUSH 0x0
	$-24B    52AAE2B4     6A 00                    PUSH 0x0
	$-249    52AAE2B6     FF15 B4B0AC52            CALL DWORD PTR DS:[<&KERNEL32.CreateThread>]           ; kernel_2.CreateThread
	$-243    52AAE2BC     8946 0C                  MOV DWORD PTR DS:[ESI+0xC],EAX
	$-240    52AAE2BF     8B4E 20                  MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-23D    52AAE2C2     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-23B    52AAE2C4     8B50 04                  MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-238    52AAE2C7     FFD2                     CALL EDX
	$-236    52AAE2C9     B8 01000000              MOV EAX,0x1
	$-231    52AAE2CE     5E                       POP ESI
	$-230    52AAE2CF     C2 0400                  RETN 0x4
	$-22D    52AAE2D2     CC                       INT3
	$-22C    52AAE2D3     CC                       INT3
	$-22B    52AAE2D4     CC                       INT3
	$-22A    52AAE2D5     CC                       INT3
	$-229    52AAE2D6     CC                       INT3
	$-228    52AAE2D7     CC                       INT3
	$-227    52AAE2D8     CC                       INT3
	$-226    52AAE2D9     CC                       INT3
	$-225    52AAE2DA     CC                       INT3
	$-224    52AAE2DB     CC                       INT3
	$-223    52AAE2DC     CC                       INT3
	$-222    52AAE2DD     CC                       INT3
	$-221    52AAE2DE     CC                       INT3
	$-220    52AAE2DF     CC                       INT3
	$-21F    52AAE2E0     56                       PUSH ESI
	$-21E    52AAE2E1     8BF1                     MOV ESI,ECX
	$-21C    52AAE2E3     33C0                     XOR EAX,EAX
	$-21A    52AAE2E5     3946 20                  CMP DWORD PTR DS:[ESI+0x20],EAX
	$-217    52AAE2E8     74 2E                    JE SHORT SecureCh.52AAE318
	$-215    52AAE2EA     68 04010000              PUSH 0x104
	$-210    52AAE2EF     68 E0D4AC52              PUSH SecureCh.52ACD4E0                                 ; UNICODE "正在取消扫描"
	$-20B    52AAE2F4     68 20C0AD52              PUSH SecureCh.52ADC020                                 ; UNICODE "安全检查进行中..."
	$-206    52AAE2F9     C746 08 01000000         MOV DWORD PTR DS:[ESI+0x8],0x1
	$-1FF    52AAE300     FF15 10B3AC52            CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$-1F9    52AAE306     8B4E 20                  MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-1F6    52AAE309     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-1F4    52AAE30B     8B50 0C                  MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-1F1    52AAE30E     83C4 0C                  ADD ESP,0xC
	$-1EE    52AAE311     FFD2                     CALL EDX
	$-1EC    52AAE313     B8 01000000              MOV EAX,0x1
	$-1E7    52AAE318     5E                       POP ESI
	$-1E6    52AAE319     C3                       RETN
	$-1E5    52AAE31A     CC                       INT3
	$-1E4    52AAE31B     CC                       INT3
	$-1E3    52AAE31C     CC                       INT3
	$-1E2    52AAE31D     CC                       INT3
	$-1E1    52AAE31E     CC                       INT3
	$-1E0    52AAE31F     CC                       INT3
	$-1DF    52AAE320     8B49 20                  MOV ECX,DWORD PTR DS:[ECX+0x20]
	$-1DC    52AAE323     B8 01000000              MOV EAX,0x1
	$-1D7    52AAE328     85C9                     TEST ECX,ECX
	$-1D5    52AAE32A     74 16                    JE SHORT SecureCh.52AAE342
	$-1D3    52AAE32C     F681 98000000 04         TEST BYTE PTR DS:[ECX+0x98],0x4
	$-1CC    52AAE333     74 07                    JE SHORT SecureCh.52AAE33C
	$-1CA    52AAE335     8389 3C060000 08         OR DWORD PTR DS:[ECX+0x63C],0x8
	$-1C3    52AAE33C     8B81 3C060000            MOV EAX,DWORD PTR DS:[ECX+0x63C]
	$-1BD    52AAE342     C3                       RETN
	$-1BC    52AAE343     CC                       INT3
	$-1BB    52AAE344     CC                       INT3
	$-1BA    52AAE345     CC                       INT3
	$-1B9    52AAE346     CC                       INT3
	$-1B8    52AAE347     CC                       INT3
	$-1B7    52AAE348     CC                       INT3
	$-1B6    52AAE349     CC                       INT3
	$-1B5    52AAE34A     CC                       INT3
	$-1B4    52AAE34B     CC                       INT3
	$-1B3    52AAE34C     CC                       INT3
	$-1B2    52AAE34D     CC                       INT3
	$-1B1    52AAE34E     CC                       INT3
	$-1B0    52AAE34F     CC                       INT3
	$-1AF    52AAE350     8B49 20                  MOV ECX,DWORD PTR DS:[ECX+0x20]
	$-1AC    52AAE353     85C9                     TEST ECX,ECX
	$-1AA    52AAE355     74 35                    JE SHORT SecureCh.52AAE38C
	$-1A8    52AAE357     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-1A4    52AAE35B     85C0                     TEST EAX,EAX
	$-1A2    52AAE35D     75 1C                    JNZ SHORT SecureCh.52AAE37B
	$-1A0    52AAE35F     8B81 40060000            MOV EAX,DWORD PTR DS:[ECX+0x640]
	$-19A    52AAE365     8B91 44060000            MOV EDX,DWORD PTR DS:[ECX+0x644]
	$-194    52AAE36B     B1 20                    MOV CL,0x20
	$-192    52AAE36D     E8 7EAC0100              CALL SecureCh.52AC8FF0
	$-18D    52AAE372     8B4C24 04                MOV ECX,DWORD PTR SS:[ESP+0x4]
	$-189    52AAE376     8901                     MOV DWORD PTR DS:[ECX],EAX
	$-187    52AAE378     C2 0800                  RETN 0x8
	$-184    52AAE37B     83F8 01                  CMP EAX,0x1
	$-181    52AAE37E     75 0C                    JNZ SHORT SecureCh.52AAE38C
	$-17F    52AAE380     8B91 40060000            MOV EDX,DWORD PTR DS:[ECX+0x640]
	$-179    52AAE386     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-175    52AAE38A     8910                     MOV DWORD PTR DS:[EAX],EDX
	$-173    52AAE38C     C2 0800                  RETN 0x8
	$-170    52AAE38F     CC                       INT3
	$-16F    52AAE390     55                       PUSH EBP
	$-16E    52AAE391     8BEC                     MOV EBP,ESP
	$-16C    52AAE393     83E4 F8                  AND ESP,0xFFFFFFF8
	$-169    52AAE396     81EC 2C040000            SUB ESP,0x42C
	$-163    52AAE39C     A1 9040AD52              MOV EAX,DWORD PTR DS:[0x52AD4090]
	$-15E    52AAE3A1     33C4                     XOR EAX,ESP
	$-15C    52AAE3A3     898424 28040000          MOV DWORD PTR SS:[ESP+0x428],EAX
	$-155    52AAE3AA     53                       PUSH EBX
	$-154    52AAE3AB     56                       PUSH ESI
	$-153    52AAE3AC     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-150    52AAE3AF     57                       PUSH EDI
	$-14F    52AAE3B0     68 1C040000              PUSH 0x41C
	$-14A    52AAE3B5     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-146    52AAE3B9     6A 00                    PUSH 0x0
	$-144    52AAE3BB     50                       PUSH EAX
	$-143    52AAE3BC     C74424 1C 00000000       MOV DWORD PTR SS:[ESP+0x1C],0x0
	$-13B    52AAE3C4     E8 D78E0100              CALL <JMP.&MSVCR90.memset>
	$-136    52AAE3C9     83C4 0C                  ADD ESP,0xC
	$-133    52AAE3CC     85F6                     TEST ESI,ESI
	$-131    52AAE3CE     0F84 D1000000            JE SecureCh.52AAE4A5
	$-12B    52AAE3D4     837E 08 00               CMP DWORD PTR DS:[ESI+0x8],0x0
	$-127    52AAE3D8     0F85 C7000000            JNZ SecureCh.52AAE4A5
	$-121    52AAE3DE     8B3D 10B3AC52            MOV EDI,DWORD PTR DS:[<&MSVCR90.wcsncpy>]              ; msvcr90.wcsncpy
	$-11B    52AAE3E4     BB 01000000              MOV EBX,0x1
	$-116    52AAE3E9     8DA424 00000000          LEA ESP,DWORD PTR SS:[ESP]
	$-10F    52AAE3F0     8B46 18                  MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-10C    52AAE3F3     83F8 5F                  CMP EAX,0x5F
	$-109    52AAE3F6     0F84 A9000000            JE SecureCh.52AAE4A5
	$-103    52AAE3FC     73 1A                    JNB SHORT SecureCh.52AAE418
	$-101    52AAE3FE     68 E0D4AC52              PUSH SecureCh.52ACD4E0                                 ; UNICODE "正在取消扫描"
	$-FC     52AAE403     68 20C0AD52              PUSH SecureCh.52ADC020                                 ; UNICODE "安全检查进行中..."
	$-F7     52AAE408     FF15 D4B2AC52            CALL DWORD PTR DS:[<&MSVCR90._wcsicmp>]                ; msvcr90._wcsicmp
	$-F1     52AAE40E     83C4 08                  ADD ESP,0x8
	$-EE     52AAE411     85C0                     TEST EAX,EAX
	$-EC     52AAE413     74 03                    JE SHORT SecureCh.52AAE418
	$-EA     52AAE415     015E 18                  ADD DWORD PTR DS:[ESI+0x18],EBX
	$-E7     52AAE418     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$-E4     52AAE41B     68 04010000              PUSH 0x104
	$-DF     52AAE420     8D5424 24                LEA EDX,DWORD PTR SS:[ESP+0x24]
	$-DB     52AAE424     68 20C0AD52              PUSH SecureCh.52ADC020                                 ; UNICODE "安全检查进行中..."
	$-D6     52AAE429     52                       PUSH EDX
	$-D5     52AAE42A     894C24 24                MOV DWORD PTR SS:[ESP+0x24],ECX
	$-D1     52AAE42E     C74424 28 64000000       MOV DWORD PTR SS:[ESP+0x28],0x64
	$-C9     52AAE436     FFD7                     CALL EDI
	$-C7     52AAE438     83C4 0C                  ADD ESP,0xC
	$-C4     52AAE43B     837E 04 00               CMP DWORD PTR DS:[ESI+0x4],0x0
	$-C0     52AAE43F     74 4F                    JE SHORT SecureCh.52AAE490
	$-BE     52AAE441     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-BB     52AAE444     6A FF                    PUSH -0x1
	$-B9     52AAE446     50                       PUSH EAX
	$-B8     52AAE447     FF15 28B0AC52            CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]    ; kernel_2.WaitForSingleObject
	$-B2     52AAE44D     837E 08 00               CMP DWORD PTR DS:[ESI+0x8],0x0
	$-AE     52AAE451     75 3D                    JNZ SHORT SecureCh.52AAE490
	$-AC     52AAE453     8B4C24 18                MOV ECX,DWORD PTR SS:[ESP+0x18]
	$-A8     52AAE457     3B4C24 1C                CMP ECX,DWORD PTR SS:[ESP+0x1C]
	$-A4     52AAE45B     75 17                    JNZ SHORT SecureCh.52AAE474
	$-A2     52AAE45D     68 04010000              PUSH 0x104
	$-9D     52AAE462     8D5424 24                LEA EDX,DWORD PTR SS:[ESP+0x24]
	$-99     52AAE466     68 D0D4AC52              PUSH SecureCh.52ACD4D0                                 ; UNICODE "安全检查完成"
	$-94     52AAE46B     52                       PUSH EDX
	$-93     52AAE46C     895E 08                  MOV DWORD PTR DS:[ESI+0x8],EBX
	$-90     52AAE46F     FFD7                     CALL EDI
	$-8E     52AAE471     83C4 0C                  ADD ESP,0xC
	$-8B     52AAE474     8B4E 1C                  MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$-88     52AAE477     85C9                     TEST ECX,ECX
	$-86     52AAE479     74 0B                    JE SHORT SecureCh.52AAE486
	$-84     52AAE47B     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-82     52AAE47D     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-80     52AAE47F     8D5424 10                LEA EDX,DWORD PTR SS:[ESP+0x10]
	$-7C     52AAE483     52                       PUSH EDX
	$-7B     52AAE484     FFD0                     CALL EAX
	$-79     52AAE486     8B4E 14                  MOV ECX,DWORD PTR DS:[ESI+0x14]
	$-76     52AAE489     51                       PUSH ECX
	$-75     52AAE48A     FF15 2CB0AC52            CALL DWORD PTR DS:[<&KERNEL32.SetEvent>]               ; kernel_2.SetEvent
	$-6F     52AAE490     68 04010000              PUSH 0x104
	$-6A     52AAE495     FF15 60B1AC52            CALL DWORD PTR DS:[<&KERNEL32.Sleep>]                  ; kernel_2.Sleep
	$-64     52AAE49B     837E 08 00               CMP DWORD PTR DS:[ESI+0x8],0x0
	$-60     52AAE49F   ^ 0F84 4BFFFFFF            JE SecureCh.52AAE3F0
	$-5A     52AAE4A5     8B8C24 34040000          MOV ECX,DWORD PTR SS:[ESP+0x434]
	$-53     52AAE4AC     5F                       POP EDI
	$-52     52AAE4AD     5E                       POP ESI
	$-51     52AAE4AE     5B                       POP EBX
	$-50     52AAE4AF     33CC                     XOR ECX,ESP
	$-4E     52AAE4B1     33C0                     XOR EAX,EAX
	$-4C     52AAE4B3     E8 E0880100              CALL SecureCh.52AC6D98
	$-47     52AAE4B8     8BE5                     MOV ESP,EBP
	$-45     52AAE4BA     5D                       POP EBP
	$-44     52AAE4BB     C2 0400                  RETN 0x4
	$-41     52AAE4BE     CC                       INT3
	$-40     52AAE4BF     CC                       INT3
	$-3F     52AAE4C0     837E 04 00               CMP DWORD PTR DS:[ESI+0x4],0x0
	$-3B     52AAE4C4     74 54                    JE SHORT SecureCh.52AAE51A
	$-39     52AAE4C6     85FF                     TEST EDI,EDI
	$-37     52AAE4C8     74 50                    JE SHORT SecureCh.52AAE51A
	$-35     52AAE4CA     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-32     52AAE4CD     6A FF                    PUSH -0x1
	$-30     52AAE4CF     50                       PUSH EAX
	$-2F     52AAE4D0     FF15 28B0AC52            CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]    ; kernel_2.WaitForSingleObject
	$-29     52AAE4D6     837E 08 00               CMP DWORD PTR DS:[ESI+0x8],0x0
	$-25     52AAE4DA     75 3E                    JNZ SHORT SecureCh.52AAE51A
	$-23     52AAE4DC     8B4F 08                  MOV ECX,DWORD PTR DS:[EDI+0x8]
	$-20     52AAE4DF     3B4F 0C                  CMP ECX,DWORD PTR DS:[EDI+0xC]
	$-1D     52AAE4E2     75 1E                    JNZ SHORT SecureCh.52AAE502
	$-1B     52AAE4E4     68 04010000              PUSH 0x104
	$-16     52AAE4E9     8D57 10                  LEA EDX,DWORD PTR DS:[EDI+0x10]
	$-13     52AAE4EC     68 D0D4AC52              PUSH SecureCh.52ACD4D0                                 ; UNICODE "安全检查完成"
	$-E      52AAE4F1     52                       PUSH EDX
	$-D      52AAE4F2     C746 08 01000000         MOV DWORD PTR DS:[ESI+0x8],0x1
	$-6      52AAE4F9     FF15 10B3AC52            CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]                 ; msvcr90.wcsncpy
	$ ==>    52AAE4FF     83C4 0C                  ADD ESP,0xC                                            ; hook此处
	$+3      52AAE502     8B4E 1C                  MOV ECX,DWORD PTR DS:[ESI+0x1C]
	$+6      52AAE505     85C9                     TEST ECX,ECX
	$+8      52AAE507     74 07                    JE SHORT SecureCh.52AAE510
	$+A      52AAE509     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+C      52AAE50B     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$+E      52AAE50D     57                       PUSH EDI
	$+F      52AAE50E     FFD2                     CALL EDX
	$+11     52AAE510     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+14     52AAE513     50                       PUSH EAX
	$+15     52AAE514     FF15 2CB0AC52            CALL DWORD PTR DS:[<&KERNEL32.SetEvent>]               ; kernel_2.SetEvent
	$+1B     52AAE51A     C3                       RETN
	$+1C     52AAE51B     CC                       INT3
	$+1D     52AAE51C     CC                       INT3
	$+1E     52AAE51D     CC                       INT3
	$+1F     52AAE51E     CC                       INT3
	$+20     52AAE51F     CC                       INT3
	$+21     52AAE520   - E9 7E570900              JMP SecureCh.52B43CA3
	$+26     52AAE525     A8 78                    TEST AL,0x78
	$+28     52AAE527     D8CC                     FMUL ST,ST(4)
	$+2A     52AAE529     0F                       DB 0F
	$+2B     52AAE52A     3BD1                     CMP EDX,ECX
	$+2D     52AAE52C     C9                       LEAVE
	$+2E     52AAE52D     CC                       INT3
	$+2F     52AAE52E     0FCE                     BSWAP ESI
	$+31     52AAE530     C0B2 C7ED6075 DE         SAL BYTE PTR DS:[EDX+0x7560EDC7],0xDE
	$+38     52AAE537     64:A7                    CMPS DWORD PTR FS:[ESI],DWORD PTR ES:[EDI]
	$+3A     52AAE539     B7 C7                    MOV BH,0xC7
	$+3C     52AAE53B     58                       POP EAX
	$+3D     52AAE53C     D6                       SALC
	$+3E     52AAE53D   ^ 7C DE                    JL SHORT SecureCh.52AAE51D
	$+40     52AAE53F     F7E4                     MUL ESP
	$+42     52AAE541     0FB6FC                   MOVZX EDI,AH
	$+45     52AAE544     6BC5 C7                  IMUL EAX,EBP,-0x39
	$+48     52AAE547     AF                       SCAS DWORD PTR ES:[EDI]
	$+49     52AAE548     96                       XCHG EAX,ESI
	$+4A     52AAE549     14 EA                    ADC AL,0xEA
	$+4C     52AAE54B     635D 23                  ARPL WORD PTR SS:[EBP+0x23],BX
	$+4F     52AAE54E     41                       INC ECX
	$+50     52AAE54F     BD 157F81DE              MOV EBP,0xDE817F15
	$+55     52AAE554     BF E1B6C798              MOV EDI,0x98C7B6E1
	$+5A     52AAE559     317D DE                  XOR DWORD PTR SS:[EBP-0x22],EDI
	$+5D     52AAE55C     621D 0BB6269E            BOUND EBX,QWORD PTR DS:[0x9E26B60B]
	$+63     52AAE562     0C B6                    OR AL,0xB6
	$+65     52AAE564     EC                       IN AL,DX
	$+66     52AAE565     9C                       PUSHFD
	$+67     52AAE566     C8 A324 E5               ENTER 0x24A3,0xE5
	$+6B     52AAE56A     ED                       IN EAX,DX
	$+6C     52AAE56B     0BB6 1181D3CC            OR ESI,DWORD PTR DS:[ESI+0xCCD38111]
	$+72     52AAE571     24 2B                    AND AL,0x2B
	$+74     52AAE573     34 3A                    XOR AL,0x3A
	$+76     52AAE575     D4 C7                    AAM 0xC7
	$+78     52AAE577   ^ E0 C1                    LOOPDNE SHORT SecureCh.52AAE53A
	$+7A     52AAE579     A3 0867D1BB              MOV DWORD PTR DS:[0xBBD16708],EAX
	$+7F     52AAE57E     A3 0FD95BA0              MOV DWORD PTR DS:[0xA05BD90F],EAX
	$+84     52AAE583     9F                       LAHF
	$+85     52AAE584     61                       POPAD
	$+86     52AAE585     26:D025 AD8A9F9F         SHL BYTE PTR ES:[0x9F9F8AAD],1
	$+8D     52AAE58C     0A1F                     OR BL,BYTE PTR DS:[EDI]
	$+8F     52AAE58E     9E                       SAHF
	$+90     52AAE58F     17                       POP SS
	$+91     52AAE590     0000                     ADD BYTE PTR DS:[EAX],AL
	$+93     52AAE592     0000                     ADD BYTE PTR DS:[EAX],AL
	$+95     52AAE594   ^ 76 8D                    JBE SHORT SecureCh.52AAE523
	$+97     52AAE596     64:24 04                 AND AL,0x4
	$+9A     52AAE599     57                       PUSH EDI
	$+9B     52AAE59A   - E9 E9BF0400              JMP SecureCh.52AFA588
	$+A0     52AAE59F     70 68                    JO SHORT SecureCh.52AAE609
	$+A2     52AAE5A1     65:43                    INC EBX
	$+A4     52AAE5A3     290A                     SUB DWORD PTR DS:[EDX],ECX
	$+A6     52AAE5A5     51                       PUSH ECX
	$+A7     52AAE5A6     52                       PUSH EDX
	$+A8     52AAE5A7     53                       PUSH EBX
	$+A9     52AAE5A8     50                       PUSH EAX
	$+AA     52AAE5A9     E8 FDF10A00              CALL SecureCh.52B5D7AB
	$+AF     52AAE5AE     7A 56                    JPE SHORT SecureCh.52AAE606
	$+B1     52AAE5B0     9C                       PUSHFD
	$+B2     52AAE5B1     55                       PUSH EBP
	$+B3     52AAE5B2     57                       PUSH EDI
	$+B4     52AAE5B3   - E9 2A360600              JMP SecureCh.52B11BE2
	$+B9     52AAE5B8   ^ 7D 8D                    JGE SHORT SecureCh.52AAE547
	$+BB     52AAE5BA     64:24 04                 AND AL,0x4
	$+BE     52AAE5BD     51                       PUSH ECX
	$+BF     52AAE5BE     52                       PUSH EDX
	$+C0     52AAE5BF     53                       PUSH EBX
	$+C1     52AAE5C0     50                       PUSH EAX
	$+C2     52AAE5C1   - E9 FB8C0600              JMP SecureCh.52B172C1
	$+C7     52AAE5C6     75 56                    JNZ SHORT SecureCh.52AAE61E
	$+C9     52AAE5C8     9C                       PUSHFD
	$+CA     52AAE5C9     E8 52E40500              CALL SecureCh.52B0CA20
	$+CF     52AAE5CE   ^ 74 CC                    JE SHORT SecureCh.52AAE59C
	$+D1     52AAE5D0     57                       PUSH EDI
	$+D2     52AAE5D1     8B78 20                  MOV EDI,DWORD PTR DS:[EAX+0x20]
	$+D5     52AAE5D4     85FF                     TEST EDI,EDI
	$+D7     52AAE5D6     74 3D                    JE SHORT SecureCh.52AAE615
	$+D9     52AAE5D8     837F 28 00               CMP DWORD PTR DS:[EDI+0x28],0x0
	$+DD     52AAE5DC     75 37                    JNZ SHORT SecureCh.52AAE615
	$+DF     52AAE5DE     837F 24 00               CMP DWORD PTR DS:[EDI+0x24],0x0
	$+E3     52AAE5E2     75 31                    JNZ SHORT SecureCh.52AAE615
	$+E5     52AAE5E4     68 38090000              PUSH 0x938
	$+EA     52AAE5E9     E8 C0870100              CALL <JMP.&MSVCR90.operator new>
	$+EF     52AAE5EE     83C4 04                  ADD ESP,0x4
	$+F2     52AAE5F1     85C0                     TEST EAX,EAX
	$+F4     52AAE5F3     74 0B                    JE SHORT SecureCh.52AAE600
	$+F6     52AAE5F5     56                       PUSH ESI
	$+F7     52AAE5F6     8BF0                     MOV ESI,EAX
	$+F9     52AAE5F8     E8 930C0000              CALL SecureCh.52AAF290
	$+FE     52AAE5FD     5E                       POP ESI
	$+FF     52AAE5FE     EB 02                    JMP SHORT SecureCh.52AAE602
	$+101    52AAE600     33C0                     XOR EAX,EAX
	$+103    52AAE602     8947 24                  MOV DWORD PTR DS:[EDI+0x24],EAX
	$+106    52AAE605     85C0                     TEST EAX,EAX
	$+108    52AAE607     74 0C                    JE SHORT SecureCh.52AAE615
	$+10A    52AAE609     6A 02                    PUSH 0x2
	$+10C    52AAE60B     8BC8                     MOV ECX,EAX
	$+10E    52AAE60D     E8 ED58FEFF              CALL SecureCh.52A93EFF
	$+113    52AAE612     8947 28                  MOV DWORD PTR DS:[EDI+0x28],EAX
	$+116    52AAE615     5F                       POP EDI
	$+117    52AAE616     C3                       RETN
	$+118    52AAE617     CC                       INT3
	*/
	
	//////////////////////////////////////////////////////////////////////////
	/*
	stAtpDummyBugTrace.o_msg_hook_proc_、InputPwd等函数所需要的HOOKPROC函数，
	其实就是SetWindowsHookEx的第二个参数，HOOK类型为WH_KEYBOARD_LL，可通过雪特查看得知：
	雪特->应用层钩子->消息钩子->查找TASLogin.exe进程的消息钩子，其地址便是HOOKPROC地址，由此便可计算得出偏移。
	*/
	//////////////////////////////////////////////////////////////////////////
}
