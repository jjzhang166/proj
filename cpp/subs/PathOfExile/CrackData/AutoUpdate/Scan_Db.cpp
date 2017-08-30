#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Db(Cmn_FC_OFstream& ofs)
{
	boost::optional<pt_dword> op_dw_tmp;
	pt_dword dw_tmp;
	pt_pcvoid addr_tmp;
	/*
	$-13D    00715D3F     CC                        INT3
	$-13C    00715D40     FF71 0C                   PUSH DWORD PTR DS:[ECX+0xC]                       ; PathOfEx.00680043
	$-139    00715D43     E8 88070000               CALL PathOfEx.007164D0
	$-134    00715D48     8BC8                      MOV ECX, EAX
	$-132    00715D4A     E8 11030000               CALL PathOfEx.00716060
	$-12D    00715D4F     C3                        RETN
	$-12C    00715D50     55                        PUSH EBP
	$-12B    00715D51     8BEC                      MOV EBP, ESP
	$-129    00715D53     6A FF                     PUSH -0x1
	$-127    00715D55     68 8B59D400               PUSH PathOfEx.00D4598B
	$-122    00715D5A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
	$-11C    00715D60     50                        PUSH EAX
	$-11B    00715D61     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
	$-114    00715D68     83EC 0C                   SUB ESP, 0xC
	$-111    00715D6B     51                        PUSH ECX
	$-110    00715D6C     8D45 F0                   LEA EAX, DWORD PTR SS:[EBP-0x10]
	$-10D    00715D6F     C745 EC C81E1501          MOV DWORD PTR SS:[EBP-0x14], PathOfEx.01151EC8
	$-106    00715D76     50                        PUSH EAX
	$-105    00715D77     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10], 0x0
	$-FE     00715D7E     E8 8D0B0000               CALL PathOfEx.00716910
	$-F9     00715D83     6A 00                     PUSH 0x0
	$-F7     00715D85     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-F0     00715D8C     6A 00                     PUSH 0x0
	$-EE     00715D8E     C705 E81E1501 00000000    MOV DWORD PTR DS:[0x1151EE8], 0x0
	$-E4     00715D98     C705 EC1E1501 00000000    MOV DWORD PTR DS:[0x1151EEC], 0x0
	$-DA     00715DA2     E8 A9DCDDFF               CALL PathOfEx.004F3A50
	$-D5     00715DA7     A3 E81E1501               MOV DWORD PTR DS:[0x1151EE8], EAX
	$-D0     00715DAC     68 F81E1501               PUSH PathOfEx.01151EF8
	$-CB     00715DB1     C705 F01E1501 00000000    MOV DWORD PTR DS:[0x1151EF0], 0x0
	$-C1     00715DBB     C605 F41E1501 00          MOV BYTE PTR DS:[0x1151EF4], 0x0
	$-BA     00715DC2     FF15 2CC1D800             CALL NEAR DWORD PTR DS:[0xD8C12C]                 ; ntdll_12.RtlInitializeCriticalSection
	$-B4     00715DC8     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4], 0x2
	$-B0     00715DCC     F3:0F1005 90D1F300        MOVSS XMM0, DWORD PTR DS:[0xF3D190]
	$-A8     00715DD4     F3:0F5E05 C81E1501        DIVSS XMM0, DWORD PTR DS:[0x1151EC8]
	$-A0     00715DDC     E8 7F5BDDFF               CALL PathOfEx.004EB960
	$-9B     00715DE1     E8 4A455600               CALL PathOfEx.00C7A330
	$-96     00715DE6     50                        PUSH EAX
	$-95     00715DE7     E8 84080000               CALL PathOfEx.00716670
	$-90     00715DEC     B9 C81E1501               MOV ECX, PathOfEx.01151EC8
	$-8B     00715DF1     E8 EA050000               CALL PathOfEx.007163E0
	$-86     00715DF6     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-83     00715DF9     B8 C81E1501               MOV EAX, PathOfEx.01151EC8
	$-7E     00715DFE     C605 D96BFF00 01          MOV BYTE PTR DS:[0xFF6BD9], 0x1
	$-77     00715E05     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
	$-70     00715E0C     8BE5                      MOV ESP, EBP
	$-6E     00715E0E     5D                        POP EBP                                           ; 3EF5C28F
	$-6D     00715E0F     C3                        RETN
	$-6C     00715E10     E9 1B090000               JMP PathOfEx.00716730
	$-67     00715E15     CC                        INT3
	$-66     00715E16     CC                        INT3
	$-65     00715E17     CC                        INT3
	$-64     00715E18     CC                        INT3
	$-63     00715E19     CC                        INT3
	$-62     00715E1A     CC                        INT3
	$-61     00715E1B     CC                        INT3
	$-60     00715E1C     CC                        INT3
	$-5F     00715E1D     CC                        INT3
	$-5E     00715E1E     CC                        INT3
	$-5D     00715E1F     CC                        INT3
	$-5C     00715E20     55                        PUSH EBP
	$-5B     00715E21     8BEC                      MOV EBP, ESP
	$-59     00715E23     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
	$-53     00715E29     B9 C81E1501               MOV ECX, PathOfEx.01151EC8                        ; db地址
	$-4E     00715E2E     6A FF                     PUSH -0x1
	$-4C     00715E30     68 20F2D100               PUSH PathOfEx.00D1F220                            ; 入口地址
	$-47     00715E35     50                        PUSH EAX
	$-46     00715E36     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
	$-3F     00715E3D     81EC 10010000             SUB ESP, 0x110
	$-39     00715E43     53                        PUSH EBX                                          ; PathOfEx.00E6DDE8
	$-38     00715E44     56                        PUSH ESI
	$-37     00715E45     57                        PUSH EDI
	$-36     00715E46     6A 00                     PUSH 0x0
	$-34     00715E48     E8 E3030000               CALL PathOfEx.00716230
	$-2F     00715E4D     8B1D CC1E1501             MOV EBX, DWORD PTR DS:[0x1151ECC]
	$-29     00715E53     8B33                      MOV ESI, DWORD PTR DS:[EBX]                       ; PathOfEx.006E0049
	$-27     00715E55     8975 F0                   MOV DWORD PTR SS:[EBP-0x10], ESI
	$-24     00715E58     3BF3                      CMP ESI, EBX                                      ; PathOfEx.00E6DDE8
	$-22     00715E5A     0F84 F0000000             JE PathOfEx.00715F50
	$-1C     00715E60     8B56 08                   MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-19     00715E63     33C0                      XOR EAX, EAX
	$-17     00715E65     C745 EC 07000000          MOV DWORD PTR SS:[EBP-0x14], 0x7
	$-10     00715E6C     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18], 0x0
	$-9      00715E73     66:8945 D8                MOV WORD PTR SS:[EBP-0x28], AX
	$-5      00715E77     66:3902                   CMP WORD PTR DS:[EDX], AX
	$-2      00715E7A     74 14                     JE SHORT PathOfEx.00715E90
	$ ==>    00715E7C     8BC2                      MOV EAX, EDX                                      ; PathOfEx.00E6DDE8
	$+2      00715E7E     8D78 02                   LEA EDI, DWORD PTR DS:[EAX+0x2]
	$+5      00715E81     66:8B08                   MOV CX, WORD PTR DS:[EAX]
	$+8      00715E84     83C0 02                   ADD EAX, 0x2
	$+B      00715E87     66:85C9                   TEST CX, CX
	$+E      00715E8A   ^ 75 F5                     JNZ SHORT PathOfEx.00715E81
	$+10     00715E8C     2BC7                      SUB EAX, EDI
	$+12     00715E8E     D1F8                      SAR EAX, 1
	$+14     00715E90     50                        PUSH EAX
	$+15     00715E91     52                        PUSH EDX                                          ; PathOfEx.00E6DDE8
	$+16     00715E92     8D4D D8                   LEA ECX, DWORD PTR SS:[EBP-0x28]
	$+19     00715E95     E8 D67DD5FF               CALL PathOfEx.0046DC70
	$+1E     00715E9A     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
	$+21     00715E9D     50                        PUSH EAX
	$+22     00715E9E     8D8D E8FEFFFF             LEA ECX, DWORD PTR SS:[EBP-0x118]
	$+28     00715EA4     E8 279FF7FF               CALL PathOfEx.0068FDD0
	$+2D     00715EA9     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+30     00715EAC     83F8 08                   CMP EAX, 0x8
	$+33     00715EAF     72 0F                     JB SHORT PathOfEx.00715EC0
	$+35     00715EB1     6A 02                     PUSH 0x2
	$+37     00715EB3     40                        INC EAX
	$+38     00715EB4     50                        PUSH EAX
	$+39     00715EB5     FF75 D8                   PUSH DWORD PTR SS:[EBP-0x28]
	$+3C     00715EB8     E8 3369D5FF               CALL PathOfEx.0046C7F0
	$+41     00715EBD     83C4 0C                   ADD ESP, 0xC
	$+44     00715EC0     33C0                      XOR EAX, EAX
	$+46     00715EC2     C745 EC 07000000          MOV DWORD PTR SS:[EBP-0x14], 0x7
	$+4D     00715EC9     66:8945 D8                MOV WORD PTR SS:[EBP-0x28], AX
	$+51     00715ECD     8B46 0C                   MOV EAX, DWORD PTR DS:[ESI+0xC]
	$+54     00715ED0     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18], 0x0
	$+5B     00715ED7     8B78 38                   MOV EDI, DWORD PTR DS:[EAX+0x38]
	$+5E     00715EDA     8D85 38FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xC8]
	$+64     00715EE0     68 A0CAEA00               PUSH PathOfEx.00EACAA0                            ; UNICODE "Reference count for loaded resource "
	$+69     00715EE5     50                        PUSH EAX
	$+6A     00715EE6     E8 D51FD7FF               CALL PathOfEx.00487EC0
	$+6F     00715EEB     FF76 08                   PUSH DWORD PTR DS:[ESI+0x8]
	$+72     00715EEE     8D85 38FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xC8]
	$+78     00715EF4     50                        PUSH EAX
	$+79     00715EF5     E8 C61FD7FF               CALL PathOfEx.00487EC0
	$+7E     00715EFA     8D85 38FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xC8]
	$+84     00715F00     68 ECCAEA00               PUSH PathOfEx.00EACAEC                            ; UNICODE " is "
	$+89     00715F05     50                        PUSH EAX
	$+8A     00715F06     E8 B51FD7FF               CALL PathOfEx.00487EC0
	$+8F     00715F0B     83C4 18                   ADD ESP, 0x18
	$+92     00715F0E     8D8D 38FFFFFF             LEA ECX, DWORD PTR SS:[EBP-0xC8]
	$+98     00715F14     57                        PUSH EDI
	$+99     00715F15     E8 D6A9D6FF               CALL PathOfEx.004808F0
	$+9E     00715F1A     8D85 38FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xC8]
	$+A4     00715F20     68 44D6E900               PUSH PathOfEx.00E9D644                            ; UNICODE "."
	$+A9     00715F25     50                        PUSH EAX
	$+AA     00715F26     E8 951FD7FF               CALL PathOfEx.00487EC0
	$+AF     00715F2B     8D85 E8FEFFFF             LEA EAX, DWORD PTR SS:[EBP-0x118]
	$+B5     00715F31     50                        PUSH EAX
	$+B6     00715F32     FF15 1847FD00             CALL NEAR DWORD PTR DS:[0xFD4718]                 ; PathOfEx.006F6550
	$+BC     00715F38     83C4 0C                   ADD ESP, 0xC
	$+BF     00715F3B     8D8D E8FEFFFF             LEA ECX, DWORD PTR SS:[EBP-0x118]
	$+C5     00715F41     E8 BA9DF7FF               CALL PathOfEx.0068FD00
	$+CA     00715F46     8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-0x10]
	$+CD     00715F49     8B36                      MOV ESI, DWORD PTR DS:[ESI]
	$+CF     00715F4B   ^ E9 05FFFFFF               JMP PathOfEx.00715E55
	$+D4     00715F50     68 F81E1501               PUSH PathOfEx.01151EF8
	$+D9     00715F55     C605 D96BFF00 00          MOV BYTE PTR DS:[0xFF6BD9], 0x0
	$+E0     00715F5C     FF15 38C1D800             CALL NEAR DWORD PTR DS:[0xD8C138]                 ; ntdll_12.RtlDeleteCriticalSection
	$+E6     00715F62     B9 E81E1501               MOV ECX, PathOfEx.01151EE8
	$+EB     00715F67     E8 24080000               CALL PathOfEx.00716790
	$+F0     00715F6C     6A 14                     PUSH 0x14
	$+F2     00715F6E     6A 01                     PUSH 0x1
	$+F4     00715F70     FF35 E81E1501             PUSH DWORD PTR DS:[0x1151EE8]
	$+FA     00715F76     E8 7568D5FF               CALL PathOfEx.0046C7F0
	$+FF     00715F7B     83C4 0C                   ADD ESP, 0xC
	$+102    00715F7E     E8 AD070000               CALL PathOfEx.00716730
	$+107    00715F83     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+10A    00715F86     5F                        POP EDI                                           ; 0C210D70
	$+10B    00715F87     5E                        POP ESI                                           ; PathOfEx.00A19559
	$+10C    00715F88     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
	$+113    00715F8F     5B                        POP EBX                                           ; PathOfEx.00A19559
	$+114    00715F90     8BE5                      MOV ESP, EBP
	$+116    00715F92     5D                        POP EBP                                           ; PathOfEx.00A19559
	$+117    00715F93     C3                        RETN
	$+118    00715F94     CC                        INT3
	$+119    00715F95     CC                        INT3
	$+11A    00715F96     CC                        INT3
	$+11B    00715F97     CC                        INT3
	$+11C    00715F98     CC                        INT3
	$+11D    00715F99     CC                        INT3
	$+11E    00715F9A     CC                        INT3
	$+11F    00715F9B     CC                        INT3
	$+120    00715F9C     CC                        INT3
	$+121    00715F9D     CC                        INT3
	$+122    00715F9E     CC                        INT3
	$+123    00715F9F     CC                        INT3
	$+124    00715FA0     51                        PUSH ECX
	$+125    00715FA1     8B4424 08                 MOV EAX, DWORD PTR SS:[ESP+0x8]
	$+129    00715FA5     83C0 14                   ADD EAX, 0x14
	$+12C    00715FA8     56                        PUSH ESI
	$+12D    00715FA9     8BF1                      MOV ESI, ECX
	$+12F    00715FAB     8378 14 08                CMP DWORD PTR DS:[EAX+0x14], 0x8
	$+133    00715FAF     72 02                     JB SHORT PathOfEx.00715FB3
	$+135    00715FB1     8B00                      MOV EAX, DWORD PTR DS:[EAX]
	$+137    00715FB3     894424 0C                 MOV DWORD PTR SS:[ESP+0xC], EAX
	$+13B    00715FB7     8D4424 0C                 LEA EAX, DWORD PTR SS:[ESP+0xC]
	$+13F    00715FBB     50                        PUSH EAX
	$+140    00715FBC     8D4424 08                 LEA EAX, DWORD PTR SS:[ESP+0x8]
	$+144    00715FC0     50                        PUSH EAX
	$+145    00715FC1     E8 5A080000               CALL PathOfEx.00716820
	$+14A    00715FC6     8B4424 04                 MOV EAX, DWORD PTR SS:[ESP+0x4]
	$+14E    00715FCA     3B46 04                   CMP EAX, DWORD PTR DS:[ESI+0x4]
	$+151    00715FCD     74 0D                     JE SHORT PathOfEx.00715FDC
	$+153    00715FCF     50                        PUSH EAX
	$+154    00715FD0     8D4424 10                 LEA EAX, DWORD PTR SS:[ESP+0x10]
	$+158    00715FD4     8BCE                      MOV ECX, ESI
	$+15A    00715FD6     50                        PUSH EAX
	$+15B    00715FD7     E8 14060000               CALL PathOfEx.007165F0
	$+160    00715FDC     5E                        POP ESI                                           ; PathOfEx.00A19559
	$+161    00715FDD     59                        POP ECX                                           ; PathOfEx.00A19559
	$+162    00715FDE     C2 0400                   RETN 0x4
	$+165    00715FE1     CC                        INT3
	*/

	//gj3
	/*
	$-18D    0060DBFF     CC                       INT3
	$-18C    0060DC00     55                       PUSH EBP
	$-18B    0060DC01     8B6C24 08                MOV EBP,DWORD PTR SS:[ESP+0x8]
	$-187    0060DC05     8BCD                     MOV ECX,EBP
	$-185    0060DC07     56                       PUSH ESI
	$-184    0060DC08     57                       PUSH EDI
	$-183    0060DC09     BF C59D1C81              MOV EDI,0x811C9DC5
	$-17E    0060DC0E     33F6                     XOR ESI,ESI
	$-17C    0060DC10     8D51 02                  LEA EDX,DWORD PTR DS:[ECX+0x2]
	$-179    0060DC13     66:8B01                  MOV AX,WORD PTR DS:[ECX]
	$-176    0060DC16     83C1 02                  ADD ECX,0x2
	$-173    0060DC19     66:85C0                  TEST AX,AX
	$-170    0060DC1C   ^ 75 F5                    JNZ SHORT PathOfEx.0060DC13
	$-16E    0060DC1E     2BCA                     SUB ECX,EDX
	$-16C    0060DC20     B8 CDCCCCCC              MOV EAX,0xCCCCCCCD
	$-167    0060DC25     D1F9                     SAR ECX,1
	$-165    0060DC27     F7E1                     MUL ECX
	$-163    0060DC29     C1EA 03                  SHR EDX,0x3
	$-160    0060DC2C     85C9                     TEST ECX,ECX
	$-15E    0060DC2E     74 24                    JE SHORT PathOfEx.0060DC54
	$-15C    0060DC30     53                       PUSH EBX
	$-15B    0060DC31     8D5A 01                  LEA EBX,DWORD PTR DS:[EDX+0x1]
	$-158    0060DC34     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$-154    0060DC38     0F1F8400 00000000        NOP DWORD PTR DS:[EAX+EAX]
	$-14C    0060DC40     0FB75475 00              MOVZX EDX,WORD PTR SS:[EBP+ESI*2]
	$-147    0060DC45     03F3                     ADD ESI,EBX
	$-145    0060DC47     69FF 93010001            IMUL EDI,EDI,PathOfEx.01000193
	$-13F    0060DC4D     33FA                     XOR EDI,EDX
	$-13D    0060DC4F     3BF1                     CMP ESI,ECX
	$-13B    0060DC51   ^ 72 ED                    JB SHORT PathOfEx.0060DC40
	$-139    0060DC53     5B                       POP EBX
	$-138    0060DC54     8BC7                     MOV EAX,EDI
	$-136    0060DC56     5F                       POP EDI
	$-135    0060DC57     5E                       POP ESI
	$-134    0060DC58     5D                       POP EBP
	$-133    0060DC59     C2 0400                  RETN 0x4
	$-130    0060DC5C     CC                       INT3
	$-12F    0060DC5D     CC                       INT3
	$-12E    0060DC5E     CC                       INT3
	$-12D    0060DC5F     CC                       INT3
	$-12C    0060DC60     55                       PUSH EBP
	$-12B    0060DC61     8BEC                     MOV EBP,ESP
	$-129    0060DC63     6A FF                    PUSH -0x1
	$-127    0060DC65     68 EB7ED800              PUSH PathOfEx.00D87EEB
	$-122    0060DC6A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-11C    0060DC70     50                       PUSH EAX
	$-11B    0060DC71     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-114    0060DC78     83EC 0C                  SUB ESP,0xC
	$-111    0060DC7B     51                       PUSH ECX
	$-110    0060DC7C     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-10D    0060DC7F     C745 EC 102A3201         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01322A10
	$-106    0060DC86     50                       PUSH EAX
	$-105    0060DC87     C745 F0 00000000         MOV DWORD PTR SS:[EBP-0x10],0x0
	$-FE     0060DC8E     E8 BD0B0000              CALL PathOfEx.0060E850
	$-F9     0060DC93     6A 00                    PUSH 0x0
	$-F7     0060DC95     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-F0     0060DC9C     6A 00                    PUSH 0x0
	$-EE     0060DC9E     C705 302A3201 00000000   MOV DWORD PTR DS:[0x1322A30],0x0
	$-E4     0060DCA8     C705 342A3201 00000000   MOV DWORD PTR DS:[0x1322A34],0x0
	$-DA     0060DCB2     E8 F953DAFF              CALL PathOfEx.003B30B0
	$-D5     0060DCB7     A3 302A3201              MOV DWORD PTR DS:[0x1322A30],EAX
	$-D0     0060DCBC     68 402A3201              PUSH PathOfEx.01322A40
	$-CB     0060DCC1     C705 382A3201 00000000   MOV DWORD PTR DS:[0x1322A38],0x0
	$-C1     0060DCCB     C605 3C2A3201 00         MOV BYTE PTR DS:[0x1322A3C],0x0
	$-BA     0060DCD2     FF15 3C71DD00            CALL DWORD PTR DS:[<&KERNEL32.InitializeCriticalSection>]   ; ntdll_1.RtlInitializeCriticalSection
	$-B4     0060DCD8     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$-B0     0060DCDC     F3:0F1005 A038FB00       MOVSS XMM0,DWORD PTR DS:[0xFB38A0]
	$-A8     0060DCE4     F3:0F5E05 102A3201       DIVSS XMM0,DWORD PTR DS:[0x1322A10]
	$-A0     0060DCEC     E8 8F2BD8FF              CALL PathOfEx.00390880
	$-9B     0060DCF1     E8 9ACB6B00              CALL PathOfEx.00CCA890
	$-96     0060DCF6     50                       PUSH EAX
	$-95     0060DCF7     E8 F4070000              CALL PathOfEx.0060E4F0
	$-90     0060DCFC     B9 102A3201              MOV ECX,PathOfEx.01322A10
	$-8B     0060DD01     E8 5A050000              CALL PathOfEx.0060E260
	$-86     0060DD06     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-83     0060DD09     B8 102A3201              MOV EAX,PathOfEx.01322A10
	$-7E     0060DD0E     C605 686D0801 01         MOV BYTE PTR DS:[0x1086D68],0x1
	$-77     0060DD15     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-70     0060DD1C     8BE5                     MOV ESP,EBP
	$-6E     0060DD1E     5D                       POP EBP
	$-6D     0060DD1F     C3                       RETN
	$-6C     0060DD20     E9 8B080000              JMP PathOfEx.0060E5B0
	$-67     0060DD25     CC                       INT3
	$-66     0060DD26     CC                       INT3
	$-65     0060DD27     CC                       INT3
	$-64     0060DD28     CC                       INT3
	$-63     0060DD29     CC                       INT3
	$-62     0060DD2A     CC                       INT3
	$-61     0060DD2B     CC                       INT3
	$-60     0060DD2C     CC                       INT3
	$-5F     0060DD2D     CC                       INT3
	$-5E     0060DD2E     CC                       INT3
	$-5D     0060DD2F     CC                       INT3
	$-5C     0060DD30     55                       PUSH EBP
	$-5B     0060DD31     8BEC                     MOV EBP,ESP
	$-59     0060DD33     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-53     0060DD39     B9 102A3201              MOV ECX,PathOfEx.01322A10                                   ; db地址
	$-4E     0060DD3E     6A FF                    PUSH -0x1
	$-4C     0060DD40     68 B0D4D500              PUSH PathOfEx.00D5D4B0
	$-47     0060DD45     50                       PUSH EAX
	$-46     0060DD46     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-3F     0060DD4D     81EC 10010000            SUB ESP,0x110
	$-39     0060DD53     53                       PUSH EBX
	$-38     0060DD54     56                       PUSH ESI
	$-37     0060DD55     57                       PUSH EDI
	$-36     0060DD56     6A 00                    PUSH 0x0
	$-34     0060DD58     E8 73030000              CALL PathOfEx.0060E0D0
	$-2F     0060DD5D     8B1D 142A3201            MOV EBX,DWORD PTR DS:[0x1322A14]
	$-29     0060DD63     8B33                     MOV ESI,DWORD PTR DS:[EBX]
	$-27     0060DD65     8975 F0                  MOV DWORD PTR SS:[EBP-0x10],ESI
	$-24     0060DD68     3BF3                     CMP ESI,EBX
	$-22     0060DD6A     0F84 F0000000            JE PathOfEx.0060DE60
	$-1C     0060DD70     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-19     0060DD73     33C0                     XOR EAX,EAX
	$-17     0060DD75     C745 EC 07000000         MOV DWORD PTR SS:[EBP-0x14],0x7
	$-10     0060DD7C     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$-9      0060DD83     66:8945 D8               MOV WORD PTR SS:[EBP-0x28],AX
	$-5      0060DD87     66:3902                  CMP WORD PTR DS:[EDX],AX
	$-2      0060DD8A     74 14                    JE SHORT PathOfEx.0060DDA0
	$ ==>    0060DD8C     8BC2                     MOV EAX,EDX
	$+2      0060DD8E     8D78 02                  LEA EDI,DWORD PTR DS:[EAX+0x2]
	$+5      0060DD91     66:8B08                  MOV CX,WORD PTR DS:[EAX]
	$+8      0060DD94     83C0 02                  ADD EAX,0x2
	$+B      0060DD97     66:85C9                  TEST CX,CX
	$+E      0060DD9A   ^ 75 F5                    JNZ SHORT PathOfEx.0060DD91
	$+10     0060DD9C     2BC7                     SUB EAX,EDI
	$+12     0060DD9E     D1F8                     SAR EAX,1
	$+14     0060DDA0     50                       PUSH EAX
	$+15     0060DDA1     52                       PUSH EDX
	$+16     0060DDA2     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+19     0060DDA5     E8 668AD1FF              CALL PathOfEx.00326810
	$+1E     0060DDAA     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+21     0060DDAD     50                       PUSH EAX
	$+22     0060DDAE     8D8D E8FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x118]
	$+28     0060DDB4     E8 D7EBDEFF              CALL PathOfEx.003FC990
	$+2D     0060DDB9     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+30     0060DDBC     83F8 08                  CMP EAX,0x8
	$+33     0060DDBF     72 0F                    JB SHORT PathOfEx.0060DDD0
	$+35     0060DDC1     6A 02                    PUSH 0x2
	$+37     0060DDC3     40                       INC EAX
	$+38     0060DDC4     50                       PUSH EAX
	$+39     0060DDC5     FF75 D8                  PUSH DWORD PTR SS:[EBP-0x28]
	$+3C     0060DDC8     E8 A375D1FF              CALL PathOfEx.00325370
	$+41     0060DDCD     83C4 0C                  ADD ESP,0xC
	$+44     0060DDD0     33C0                     XOR EAX,EAX
	$+46     0060DDD2     C745 EC 07000000         MOV DWORD PTR SS:[EBP-0x14],0x7
	$+4D     0060DDD9     66:8945 D8               MOV WORD PTR SS:[EBP-0x28],AX
	$+51     0060DDDD     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+54     0060DDE0     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$+5B     0060DDE7     8B78 38                  MOV EDI,DWORD PTR DS:[EAX+0x38]
	$+5E     0060DDEA     8D85 38FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xC8]
	$+64     0060DDF0     68 C021F100              PUSH PathOfEx.00F121C0                                      ; UNICODE "Reference count for loaded resource "
	$+69     0060DDF5     50                       PUSH EAX
	$+6A     0060DDF6     E8 D5C9D2FF              CALL PathOfEx.0033A7D0
	$+6F     0060DDFB     FF76 08                  PUSH DWORD PTR DS:[ESI+0x8]
	$+72     0060DDFE     8D85 38FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xC8]
	$+78     0060DE04     50                       PUSH EAX
	$+79     0060DE05     E8 C6C9D2FF              CALL PathOfEx.0033A7D0
	$+7E     0060DE0A     8D85 38FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xC8]
	$+84     0060DE10     68 0C22F100              PUSH PathOfEx.00F1220C                                      ; UNICODE " is "
	$+89     0060DE15     50                       PUSH EAX
	$+8A     0060DE16     E8 B5C9D2FF              CALL PathOfEx.0033A7D0
	$+8F     0060DE1B     83C4 18                  ADD ESP,0x18
	$+92     0060DE1E     8D8D 38FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xC8]
	$+98     0060DE24     57                       PUSH EDI
	$+99     0060DE25     E8 F654D2FF              CALL PathOfEx.00333320
	$+9E     0060DE2A     8D85 38FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xC8]
	$+A4     0060DE30     68 D82AF000              PUSH PathOfEx.00F02AD8                                      ; UNICODE "."
	$+A9     0060DE35     50                       PUSH EAX
	$+AA     0060DE36     E8 95C9D2FF              CALL PathOfEx.0033A7D0
	$+AF     0060DE3B     8D85 E8FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x118]
	$+B5     0060DE41     50                       PUSH EAX
	$+B6     0060DE42     FF15 B4AA0501            CALL DWORD PTR DS:[0x105AAB4]                               ; PathOfEx.005ECFD0
	$+BC     0060DE48     83C4 0C                  ADD ESP,0xC
	$+BF     0060DE4B     8D8D E8FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x118]
	$+C5     0060DE51     E8 6AEADEFF              CALL PathOfEx.003FC8C0
	$+CA     0060DE56     8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+CD     0060DE59     8B36                     MOV ESI,DWORD PTR DS:[ESI]
	$+CF     0060DE5B   ^ E9 05FFFFFF              JMP PathOfEx.0060DD65
	$+D4     0060DE60     68 402A3201              PUSH PathOfEx.01322A40
	$+D9     0060DE65     C605 686D0801 00         MOV BYTE PTR DS:[0x1086D68],0x0
	$+E0     0060DE6C     FF15 4871DD00            CALL DWORD PTR DS:[<&KERNEL32.DeleteCriticalSection>]       ; ntdll_1.RtlDeleteCriticalSection
	$+E6     0060DE72     B9 302A3201              MOV ECX,PathOfEx.01322A30
	$+EB     0060DE77     E8 54080000              CALL PathOfEx.0060E6D0
	$+F0     0060DE7C     6A 14                    PUSH 0x14
	$+F2     0060DE7E     6A 01                    PUSH 0x1
	$+F4     0060DE80     FF35 302A3201            PUSH DWORD PTR DS:[0x1322A30]
	$+FA     0060DE86     E8 E574D1FF              CALL PathOfEx.00325370
	$+FF     0060DE8B     83C4 0C                  ADD ESP,0xC
	$+102    0060DE8E     E8 1D070000              CALL PathOfEx.0060E5B0
	$+107    0060DE93     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+10A    0060DE96     5F                       POP EDI
	$+10B    0060DE97     5E                       POP ESI
	$+10C    0060DE98     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+113    0060DE9F     5B                       POP EBX
	$+114    0060DEA0     8BE5                     MOV ESP,EBP
	$+116    0060DEA2     5D                       POP EBP
	$+117    0060DEA3     C3                       RETN
	$+118    0060DEA4     CC                       INT3
	$+119    0060DEA5     CC                       INT3
	$+11A    0060DEA6     CC                       INT3
	$+11B    0060DEA7     CC                       INT3
	$+11C    0060DEA8     CC                       INT3
	$+11D    0060DEA9     CC                       INT3
	$+11E    0060DEAA     CC                       INT3
	$+11F    0060DEAB     CC                       INT3
	$+120    0060DEAC     CC                       INT3
	$+121    0060DEAD     CC                       INT3
	$+122    0060DEAE     CC                       INT3
	$+123    0060DEAF     CC                       INT3
	$+124    0060DEB0     51                       PUSH ECX
	$+125    0060DEB1     56                       PUSH ESI
	$+126    0060DEB2     57                       PUSH EDI
	$+127    0060DEB3     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+12B    0060DEB7     8BF9                     MOV EDI,ECX
	$+12D    0060DEB9     50                       PUSH EAX
	$+12E    0060DEBA     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+132    0060DEBE     50                       PUSH EAX
	$+133    0060DEBF     E8 9C080000              CALL PathOfEx.0060E760
	$+138    0060DEC4     8B7424 08                MOV ESI,DWORD PTR SS:[ESP+0x8]
	$+13C    0060DEC8     3B77 04                  CMP ESI,DWORD PTR DS:[EDI+0x4]
	$+13F    0060DECB     75 08                    JNZ SHORT PathOfEx.0060DED5
	$+141    0060DECD     5F                       POP EDI
	$+142    0060DECE     33C0                     XOR EAX,EAX
	$+144    0060DED0     5E                       POP ESI
	$+145    0060DED1     59                       POP ECX
	$+146    0060DED2     C2 0400                  RETN 0x4
	$+149    0060DED5     FF76 0C                  PUSH DWORD PTR DS:[ESI+0xC]
	$+14C    0060DED8     8BCF                     MOV ECX,EDI
	$+14E    0060DEDA     E8 91010000              CALL PathOfEx.0060E070
	$+153    0060DEDF     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+156    0060DEE2     5F                       POP EDI
	$+157    0060DEE3     5E                       POP ESI
	$+158    0060DEE4     59                       POP ECX
	$+159    0060DEE5     C2 0400                  RETN 0x4
	$+15C    0060DEE8     CC                       INT3
	$+15D    0060DEE9     CC                       INT3
	$+15E    0060DEEA     CC                       INT3
	$+15F    0060DEEB     CC                       INT3
	$+160    0060DEEC     CC                       INT3
	$+161    0060DEED     CC                       INT3
	$+162    0060DEEE     CC                       INT3
	$+163    0060DEEF     CC                       INT3
	$+164    0060DEF0     8B5424 04                MOV EDX,DWORD PTR SS:[ESP+0x4]
	$+168    0060DEF4     83EC 10                  SUB ESP,0x10
	$+16B    0060DEF7     837A 28 08               CMP DWORD PTR DS:[EDX+0x28],0x8
	$+16F    0060DEFB     8D42 14                  LEA EAX,DWORD PTR DS:[EDX+0x14]
	$+172    0060DEFE     72 02                    JB SHORT PathOfEx.0060DF02
	$+174    0060DF00     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+176    0060DF02     890424                   MOV DWORD PTR SS:[ESP],EAX
	$+179    0060DF05     8D0424                   LEA EAX,DWORD PTR SS:[ESP]
	$+17C    0060DF08     50                       PUSH EAX
	$+17D    0060DF09     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+181    0060DF0D     895424 08                MOV DWORD PTR SS:[ESP+0x8],EDX
	$+185    0060DF11     50                       PUSH EAX
	$+186    0060DF12     E8 890D0000              CALL PathOfEx.0060ECA0
	$+18B    0060DF17     83C4 10                  ADD ESP,0x10
	$+18E    0060DF1A     C2 0400                  RETN 0x4
	$+191    0060DF1D     CC                       INT3
	$+192    0060DF1E     CC                       INT3
	$+193    0060DF1F     CC                       INT3
	$+194    0060DF20     55                       PUSH EBP
	$+195    0060DF21     8BEC                     MOV EBP,ESP
	$+197    0060DF23     6A FF                    PUSH -0x1
	$+199    0060DF25     68 C8FED500              PUSH PathOfEx.00D5FEC8
	$+19E    0060DF2A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+1A4    0060DF30     50                       PUSH EAX
	$+1A5    0060DF31     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+1AC    0060DF38     83EC 10                  SUB ESP,0x10
	$+1AF    0060DF3B     53                       PUSH EBX
	$+1B0    0060DF3C     56                       PUSH ESI
	$+1B1    0060DF3D     57                       PUSH EDI
	$+1B2    0060DF3E     8BF9                     MOV EDI,ECX
	$+1B4    0060DF40     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD],0x0
	$+1B8    0060DF44     8D5F 30                  LEA EBX,DWORD PTR DS:[EDI+0x30]
	$+1BB    0060DF47     53                       PUSH EBX
	$+1BC    0060DF48     895D E8                  MOV DWORD PTR SS:[EBP-0x18],EBX
	$+1BF    0060DF4B     FF15 4071DD00            CALL DWORD PTR DS:[<&KERNEL32.EnterCriticalSection>]        ; ntdll_1.RtlEnterCriticalSection
	$+1C5    0060DF51     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1CC    0060DF58     83C8 FF                  OR EAX,0xFFFFFFFF
	$+1CF    0060DF5B     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+1D2    0060DF5E     8D4E 38                  LEA ECX,DWORD PTR DS:[ESI+0x38]
	$+1D5    0060DF61     F0:0FC101                LOCK XADD DWORD PTR DS:[ECX],EAX
	$+1D9    0060DF65     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+1DB    0060DF67     85C0                     TEST EAX,EAX
	$+1DD    0060DF69     75 47                    JNZ SHORT PathOfEx.0060DFB2
	$+1DF    0060DF6B     8B46 34                  MOV EAX,DWORD PTR DS:[ESI+0x34]
	$+1E2    0060DF6E     3947 28                  CMP DWORD PTR DS:[EDI+0x28],EAX
	$+1E5    0060DF71     72 37                    JB SHORT PathOfEx.0060DFAA
	$+1E7    0060DF73     837E 28 08               CMP DWORD PTR DS:[ESI+0x28],0x8
	$+1EB    0060DF77     8D46 14                  LEA EAX,DWORD PTR DS:[ESI+0x14]
	$+1EE    0060DF7A     72 02                    JB SHORT PathOfEx.0060DF7E
	$+1F0    0060DF7C     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+1F2    0060DF7E     8945 08                  MOV DWORD PTR SS:[EBP+0x8],EAX
	$+1F5    0060DF81     8BCF                     MOV ECX,EDI
	$+1F7    0060DF83     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+1FA    0060DF86     50                       PUSH EAX
	$+1FB    0060DF87     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+1FE    0060DF8A     50                       PUSH EAX
	$+1FF    0060DF8B     E8 D0070000              CALL PathOfEx.0060E760
	$+204    0060DF90     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+207    0060DF93     3B47 04                  CMP EAX,DWORD PTR DS:[EDI+0x4]
	$+20A    0060DF96     74 0C                    JE SHORT PathOfEx.0060DFA4
	$+20C    0060DF98     50                       PUSH EAX
	$+20D    0060DF99     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+210    0060DF9C     8BCF                     MOV ECX,EDI
	$+212    0060DF9E     50                       PUSH EAX
	$+213    0060DF9F     E8 CC040000              CALL PathOfEx.0060E470
	$+218    0060DFA4     C645 F3 01               MOV BYTE PTR SS:[EBP-0xD],0x1
	$+21C    0060DFA8     EB 08                    JMP SHORT PathOfEx.0060DFB2
	$+21E    0060DFAA     56                       PUSH ESI
	$+21F    0060DFAB     8BCF                     MOV ECX,EDI
	$+221    0060DFAD     E8 3E000000              CALL PathOfEx.0060DFF0
	$+226    0060DFB2     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+22D    0060DFB9     85DB                     TEST EBX,EBX
	$+22F    0060DFBB     74 07                    JE SHORT PathOfEx.0060DFC4
	$+231    0060DFBD     53                       PUSH EBX
	$+232    0060DFBE     FF15 4471DD00            CALL DWORD PTR DS:[<&KERNEL32.LeaveCriticalSection>]        ; ntdll_1.RtlLeaveCriticalSection
	$+238    0060DFC4     807D F3 00               CMP BYTE PTR SS:[EBP-0xD],0x0
	$+23C    0060DFC8     74 11                    JE SHORT PathOfEx.0060DFDB
	$+23E    0060DFCA     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+240    0060DFCC     8BCE                     MOV ECX,ESI
	$+242    0060DFCE     6A 00                    PUSH 0x0
	$+244    0060DFD0     FF10                     CALL DWORD PTR DS:[EAX]
	$+246    0060DFD2     56                       PUSH ESI
	$+247    0060DFD3     E8 434F6D00              CALL PathOfEx.00CE2F1B
	$+24C    0060DFD8     83C4 04                  ADD ESP,0x4
	$+24F    0060DFDB     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+252    0060DFDE     5F                       POP EDI
	$+253    0060DFDF     5E                       POP ESI
	$+254    0060DFE0     5B                       POP EBX
	$+255    0060DFE1     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+25C    0060DFE8     8BE5                     MOV ESP,EBP
	$+25E    0060DFEA     5D                       POP EBP
	$+25F    0060DFEB     C2 0400                  RETN 0x4
	$+262    0060DFEE     CC                       INT3
	$+263    0060DFEF     CC                       INT3
	$+264    0060DFF0     51                       PUSH ECX
	$+265    0060DFF1     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+269    0060DFF5     56                       PUSH ESI
	$+26A    0060DFF6     8B50 34                  MOV EDX,DWORD PTR DS:[EAX+0x34]
	$+26D    0060DFF9     8B41 20                  MOV EAX,DWORD PTR DS:[ECX+0x20]
	$+270    0060DFFC     895424 04                MOV DWORD PTR SS:[ESP+0x4],EDX
	$+274    0060E000     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$+276    0060E002     3BF0                     CMP ESI,EAX
	$+278    0060E004     74 4C                    JE SHORT PathOfEx.0060E052
	$+27A    0060E006     3956 08                  CMP DWORD PTR DS:[ESI+0x8],EDX
	$+27D    0060E009     74 06                    JE SHORT PathOfEx.0060E011
	$+27F    0060E00B     8B36                     MOV ESI,DWORD PTR DS:[ESI]
	$+281    0060E00D     3BF0                     CMP ESI,EAX
	$+283    0060E00F   ^ 75 F5                    JNZ SHORT PathOfEx.0060E006
	$+285    0060E011     3BF0                     CMP ESI,EAX
	$+287    0060E013     74 3D                    JE SHORT PathOfEx.0060E052
	$+289    0060E015     57                       PUSH EDI
	$+28A    0060E016     8B7E 0C                  MOV EDI,DWORD PTR DS:[ESI+0xC]
	$+28D    0060E019     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+291    0060E01D     50                       PUSH EAX
	$+292    0060E01E     FF77 04                  PUSH DWORD PTR DS:[EDI+0x4]
	$+295    0060E021     57                       PUSH EDI
	$+296    0060E022     E8 39E2DFFF              CALL PathOfEx.0040C260
	$+29B    0060E027     8BD0                     MOV EDX,EAX
	$+29D    0060E029     B9 54555515              MOV ECX,0x15555554
	$+2A2    0060E02E     8B46 10                  MOV EAX,DWORD PTR DS:[ESI+0x10]
	$+2A5    0060E031     2BC8                     SUB ECX,EAX
	$+2A7    0060E033     83F9 01                  CMP ECX,0x1
	$+2AA    0060E036     72 1F                    JB SHORT PathOfEx.0060E057
	$+2AC    0060E038     8B4C24 10                MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+2B0    0060E03C     40                       INC EAX
	$+2B1    0060E03D     8946 10                  MOV DWORD PTR DS:[ESI+0x10],EAX
	$+2B4    0060E040     8957 04                  MOV DWORD PTR DS:[EDI+0x4],EDX
	$+2B7    0060E043     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+2BA    0060E046     5F                       POP EDI
	$+2BB    0060E047     8910                     MOV DWORD PTR DS:[EAX],EDX
	$+2BD    0060E049     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+2C0    0060E04C     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+2C3    0060E04F     8941 04                  MOV DWORD PTR DS:[ECX+0x4],EAX
	$+2C6    0060E052     5E                       POP ESI
	$+2C7    0060E053     59                       POP ECX
	$+2C8    0060E054     C2 0400                  RETN 0x4
	$+2CB    0060E057     68 AC29ED00              PUSH PathOfEx.00ED29AC
	$+2D0    0060E05C     E8 979C6A00              CALL PathOfEx.00CB7CF8
	$+2D5    0060E061     CC                       INT3
	$+2D6    0060E062     CC                       INT3
	$+2D7    0060E063     CC                       INT3
	$+2D8    0060E064     CC                       INT3
	$+2D9    0060E065     CC                       INT3
	$+2DA    0060E066     CC                       INT3
	$+2DB    0060E067     CC                       INT3
	$+2DC    0060E068     CC                       INT3
	$+2DD    0060E069     CC                       INT3
	$+2DE    0060E06A     CC                       INT3
	$+2DF    0060E06B     CC                       INT3
	$+2E0    0060E06C     CC                       INT3
	$+2E1    0060E06D     CC                       INT3
	$+2E2    0060E06E     CC                       INT3
	$+2E3    0060E06F     CC                       INT3
	$+2E4    0060E070     56                       PUSH ESI
	$+2E5    0060E071     8B7424 08                MOV ESI,DWORD PTR SS:[ESP+0x8]
	$+2E9    0060E075     8B46 38                  MOV EAX,DWORD PTR DS:[ESI+0x38]
	$+2EC    0060E078     85C0                     TEST EAX,EAX
	$+2EE    0060E07A     75 45                    JNZ SHORT PathOfEx.0060E0C1
	$+2F0    0060E07C     8B49 20                  MOV ECX,DWORD PTR DS:[ECX+0x20]
	$+2F3    0060E07F     8B56 34                  MOV EDX,DWORD PTR DS:[ESI+0x34]
	$+2F6    0060E082     895424 08                MOV DWORD PTR SS:[ESP+0x8],EDX
	$+2FA    0060E086     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+2FC    0060E088     3BC1                     CMP EAX,ECX
	$+2FE    0060E08A     74 35                    JE SHORT PathOfEx.0060E0C1
	$+300    0060E08C     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$+304    0060E090     3950 08                  CMP DWORD PTR DS:[EAX+0x8],EDX
	$+307    0060E093     74 06                    JE SHORT PathOfEx.0060E09B
	$+309    0060E095     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+30B    0060E097     3BC1                     CMP EAX,ECX
	$+30D    0060E099   ^ 75 F5                    JNZ SHORT PathOfEx.0060E090
	$+30F    0060E09B     3BC1                     CMP EAX,ECX
	$+311    0060E09D     74 22                    JE SHORT PathOfEx.0060E0C1
	$+313    0060E09F     8B76 04                  MOV ESI,DWORD PTR DS:[ESI+0x4]
	$+316    0060E0A2     6A 0C                    PUSH 0xC
	$+318    0060E0A4     6A 01                    PUSH 0x1
	$+31A    0060E0A6     56                       PUSH ESI
	$+31B    0060E0A7     8B56 04                  MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+31E    0060E0AA     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+320    0060E0AC     890A                     MOV DWORD PTR DS:[EDX],ECX
	$+322    0060E0AE     8B16                     MOV EDX,DWORD PTR DS:[ESI]
	$+324    0060E0B0     8B4E 04                  MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+327    0060E0B3     894A 04                  MOV DWORD PTR DS:[EDX+0x4],ECX
	$+32A    0060E0B6     FF48 10                  DEC DWORD PTR DS:[EAX+0x10]
	$+32D    0060E0B9     E8 B272D1FF              CALL PathOfEx.00325370
	$+332    0060E0BE     83C4 0C                  ADD ESP,0xC
	$+335    0060E0C1     5E                       POP ESI
	$+336    0060E0C2     C2 0400                  RETN 0x4
	$+339    0060E0C5     CC                       INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??8d????66????83????66????75??2b??d1??50528d????e8????????8d????508d??????????e8????????8b????83????72??6a??40", -0x53 + 1);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??8d????66????83????66????75??2b??d1??50528d????e8????????8d????508d??????????e8????????8b????83????72??6a??40", -0x53 + 1);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_db_base_addr_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	//这个call如果成功，则返回指向结果的指针
	/*
	$-53     007167DF      CC                        INT3
	$-52     007167E0      55                        PUSH EBP
	$-51     007167E1      8BEC                      MOV EBP, ESP
	$-4F     007167E3      83E4 F8                   AND ESP, -0x8
	$-4C     007167E6      83EC 0C                   SUB ESP, 0xC
	$-49     007167E9      53                        PUSH EBX
	$-48     007167EA      8BD9                      MOV EBX, ECX
	$-46     007167EC      56                        PUSH ESI
	$-45     007167ED      57                        PUSH EDI
	$-44     007167EE      8B43 04                   MOV EAX, DWORD PTR DS:[EBX+0x4]          ; PathOfEx.00EF7710
	$-41     007167F1      3900                      CMP DWORD PTR DS:[EAX], EAX
	$-3F     007167F3      74 1D                     JE SHORT PathOfEx.00716812
	$-3D     007167F5      8B78 04                   MOV EDI, DWORD PTR DS:[EAX+0x4]
	$-3A     007167F8      8B73 04                   MOV ESI, DWORD PTR DS:[EBX+0x4]          ; PathOfEx.00EF7710
	$-37     007167FB      8BCB                      MOV ECX, EBX
	$-35     007167FD      8B36                      MOV ESI, DWORD PTR DS:[ESI]
	$-33     007167FF      56                        PUSH ESI
	$-32     00716800      8D46 08                   LEA EAX, DWORD PTR DS:[ESI+0x8]
	$-2F     00716803      50                        PUSH EAX
	$-2E     00716804      8D4424 18                 LEA EAX, DWORD PTR SS:[ESP+0x18]
	$-2A     00716808      50                        PUSH EAX
	$-29     00716809      E8 C2020000               CALL PathOfEx.00716AD0
	$-24     0071680E      3BF7                      CMP ESI, EDI
	$-22     00716810    ^ 75 E6                     JNZ SHORT PathOfEx.007167F8
	$-20     00716812      5F                        POP EDI                                  ; PathOfEx.008CD96A
	$-1F     00716813      5E                        POP ESI                                  ; PathOfEx.008CD96A
	$-1E     00716814      5B                        POP EBX                                  ; PathOfEx.008CD96A
	$-1D     00716815      8BE5                      MOV ESP, EBP
	$-1B     00716817      5D                        POP EBP                                  ; PathOfEx.008CD96A
	$-1A     00716818      C3                        RETN
	$-19     00716819      CC                        INT3
	$-18     0071681A      CC                        INT3
	$-17     0071681B      CC                        INT3
	$-16     0071681C      CC                        INT3
	$-15     0071681D      CC                        INT3
	$-14     0071681E      CC                        INT3
	$-13     0071681F      CC                        INT3                                     ; 需要两个参数，第一个参数是保存结果的指针
	$-12     00716820      8B4424 08                 MOV EAX, DWORD PTR SS:[ESP+0x8]          ; 根据字符串取文件/db的地址
	$-E      00716824      53                        PUSH EBX                                 ; eax=第二个参数
	$-D      00716825      55                        PUSH EBP                                 ; 第二个参数是字符串指针的指针
	$-C      00716826      56                        PUSH ESI
	$-B      00716827      57                        PUSH EDI
	$-A      00716828      8B38                      MOV EDI, DWORD PTR DS:[EAX]
	$-8      0071682A      8BF1                      MOV ESI, ECX
	$-6      0071682C      57                        PUSH EDI                                 ; edi=字符串指针
	$-5      0071682D      E8 2EF4FFFF               CALL PathOfEx.00715C60                   ; 根据字符串取id，也就是哈希值
	$ ==>    00716832      2346 18                   AND EAX, DWORD PTR DS:[ESI+0x18]
	$+3      00716835      8B56 0C                   MOV EDX, DWORD PTR DS:[ESI+0xC]          ; shell32.7549850F
	$+6      00716838      8B5E 04                   MOV EBX, DWORD PTR DS:[ESI+0x4]
	$+9      0071683B      8B0CC2                    MOV ECX, DWORD PTR DS:[EDX+EAX*8]
	$+C      0071683E      03C0                      ADD EAX, EAX
	$+E      00716840      8B2C82                    MOV EBP, DWORD PTR DS:[EDX+EAX*4]
	$+11     00716843      40                        INC EAX
	$+12     00716844      8D0482                    LEA EAX, DWORD PTR DS:[EDX+EAX*4]
	$+15     00716847      894424 18                 MOV DWORD PTR SS:[ESP+0x18], EAX
	$+19     0071684B      0F1F4400 00               NOP DWORD PTR DS:[EAX+EAX]
	$+1E     00716850      3BEB                      CMP EBP, EBX
	$+20     00716852      75 04                     JNZ SHORT PathOfEx.00716858
	$+22     00716854      8BC3                      MOV EAX, EBX
	$+24     00716856      EB 04                     JMP SHORT PathOfEx.0071685C
	$+26     00716858      8B00                      MOV EAX, DWORD PTR DS:[EAX]
	$+28     0071685A      8B00                      MOV EAX, DWORD PTR DS:[EAX]
	$+2A     0071685C      3BC8                      CMP ECX, EAX                             ; 下面这个循环的结果就保存在ecx里面
	$+2C     0071685E      0F84 99000000             JE PathOfEx.007168FD
	$+32     00716864      8B51 08                   MOV EDX, DWORD PTR DS:[ECX+0x8]          ; PathOfEx.00720075
	$+35     00716867      8BC7                      MOV EAX, EDI
	$+37     00716869      0F1F80 00000000           NOP DWORD PTR DS:[EAX]
	$+3E     00716870      66:8B32                   MOV SI, WORD PTR DS:[EDX]                ; 循环开始
	$+41     00716873      66:3B30                   CMP SI, WORD PTR DS:[EAX]                ; eax是参数2提供的字符串
	$+44     00716876      75 1E                     JNZ SHORT PathOfEx.00716896
	$+46     00716878      66:85F6                   TEST SI, SI
	$+49     0071687B      74 15                     JE SHORT PathOfEx.00716892
	$+4B     0071687D      66:8B72 02                MOV SI, WORD PTR DS:[EDX+0x2]
	$+4F     00716881      66:3B70 02                CMP SI, WORD PTR DS:[EAX+0x2]
	$+53     00716885      75 0F                     JNZ SHORT PathOfEx.00716896
	$+55     00716887      83C2 04                   ADD EDX, 0x4
	$+58     0071688A      83C0 04                   ADD EAX, 0x4
	$+5B     0071688D      66:85F6                   TEST SI, SI
	$+5E     00716890    ^ 75 DE                     JNZ SHORT PathOfEx.00716870              ; 循环结束
	$+60     00716892      33C0                      XOR EAX, EAX
	$+62     00716894      EB 05                     JMP SHORT PathOfEx.0071689B
	$+64     00716896      1BC0                      SBB EAX, EAX
	$+66     00716898      83C8 01                   OR EAX, 0x1
	$+69     0071689B      85C0                      TEST EAX, EAX
	$+6B     0071689D      74 08                     JE SHORT PathOfEx.007168A7
	$+6D     0071689F      8B09                      MOV ECX, DWORD PTR DS:[ECX]              ; 链表之取下个结点
	$+6F     007168A1      8B4424 18                 MOV EAX, DWORD PTR SS:[ESP+0x18]
	$+73     007168A5    ^ EB A9                     JMP SHORT PathOfEx.00716850
	$+75     007168A7      8B41 08                   MOV EAX, DWORD PTR DS:[ECX+0x8]          ; PathOfEx.00720075
	$+78     007168AA      66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
	$+7E     007168B0      66:8B17                   MOV DX, WORD PTR DS:[EDI]                ; 第二个循环开始
	$+81     007168B3      66:3B10                   CMP DX, WORD PTR DS:[EAX]
	$+84     007168B6      75 2E                     JNZ SHORT PathOfEx.007168E6
	$+86     007168B8      66:85D2                   TEST DX, DX
	$+89     007168BB      74 15                     JE SHORT PathOfEx.007168D2               ; 判断字符串是否结束
	$+8B     007168BD      66:8B57 02                MOV DX, WORD PTR DS:[EDI+0x2]
	$+8F     007168C1      66:3B50 02                CMP DX, WORD PTR DS:[EAX+0x2]
	$+93     007168C5      75 1F                     JNZ SHORT PathOfEx.007168E6
	$+95     007168C7      83C7 04                   ADD EDI, 0x4
	$+98     007168CA      83C0 04                   ADD EAX, 0x4
	$+9B     007168CD      66:85D2                   TEST DX, DX
	$+9E     007168D0    ^ 75 DE                     JNZ SHORT PathOfEx.007168B0              ; 第二个循环结束
	$+A0     007168D2      33C0                      XOR EAX, EAX
	$+A2     007168D4      5F                        POP EDI                                  ; PathOfEx.008CD96A
	$+A3     007168D5      85C0                      TEST EAX, EAX
	$+A5     007168D7      8B4424 10                 MOV EAX, DWORD PTR SS:[ESP+0x10]
	$+A9     007168DB      5E                        POP ESI                                  ; eax=第一个参数
	$+AA     007168DC      0F45CB                    CMOVNE ECX, EBX                          ; ecx就是结果
	$+AD     007168DF      5D                        POP EBP                                  ; PathOfEx.008CD96A
	$+AE     007168E0      8908                      MOV DWORD PTR DS:[EAX], ECX              ; 结果存放在第一个参数表示的指针中
	$+B0     007168E2      5B                        POP EBX                                  ; PathOfEx.008CD96A
	$+B1     007168E3      C2 0800                   RETN 0x8
	$+B4     007168E6      1BC0                      SBB EAX, EAX
	$+B6     007168E8      83C8 01                   OR EAX, 0x1
	$+B9     007168EB      5F                        POP EDI                                  ; PathOfEx.008CD96A
	$+BA     007168EC      85C0                      TEST EAX, EAX
	$+BC     007168EE      8B4424 10                 MOV EAX, DWORD PTR SS:[ESP+0x10]
	$+C0     007168F2      5E                        POP ESI                                  ; PathOfEx.008CD96A
	$+C1     007168F3      0F45CB                    CMOVNE ECX, EBX
	$+C4     007168F6      5D                        POP EBP                                  ; PathOfEx.008CD96A
	$+C5     007168F7      8908                      MOV DWORD PTR DS:[EAX], ECX
	$+C7     007168F9      5B                        POP EBX                                  ; PathOfEx.008CD96A
	$+C8     007168FA      C2 0800                   RETN 0x8
	$+CB     007168FD      8B4424 14                 MOV EAX, DWORD PTR SS:[ESP+0x14]
	$+CF     00716901      5F                        POP EDI                                  ; PathOfEx.008CD96A
	$+D0     00716902      5E                        POP ESI                                  ; PathOfEx.008CD96A
	$+D1     00716903      5D                        POP EBP                                  ; PathOfEx.008CD96A
	$+D2     00716904      8918                      MOV DWORD PTR DS:[EAX], EBX
	$+D4     00716906      5B                        POP EBX                                  ; PathOfEx.008CD96A
	$+D5     00716907      C2 0800                   RETN 0x8
	$+D8     0071690A      CC                        INT3

	//这个函数调用了上面的函数
	$-3C     00777EAF      CC                        INT3
	$-3B     00777EB0      55                        PUSH EBP
	$-3A     00777EB1      8BEC                      MOV EBP, ESP
	$-38     00777EB3      6A FF                     PUSH -0x1
	$-36     00777EB5      68 C9F3D100               PUSH PathOfEx.00D1F3C9
	$-31     00777EBA      64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
	$-2B     00777EC0      50                        PUSH EAX
	$-2A     00777EC1      64:8925 00000000          MOV DWORD PTR FS:[0], ESP
	$-23     00777EC8      83EC 18                   SUB ESP, 0x18
	$-20     00777ECB      53                        PUSH EBX                                 ; PathOfEx.01151EC8
	$-1F     00777ECC      8BD9                      MOV EBX, ECX                             ; PathOfEx.01151EC8
	$-1D     00777ECE      56                        PUSH ESI
	$-1C     00777ECF      57                        PUSH EDI                                 ; PathOfEx.01151EF8
	$-1B     00777ED0      8D7B 30                   LEA EDI, DWORD PTR DS:[EBX+0x30]
	$-18     00777ED3      57                        PUSH EDI                                 ; PathOfEx.01151EF8
	$-17     00777ED4      897D E0                   MOV DWORD PTR SS:[EBP-0x20], EDI         ; PathOfEx.01151EF8
	$-14     00777ED7      FF15 30C1D800             CALL NEAR DWORD PTR DS:[0xD8C130]        ; ntdll_12.RtlEnterCriticalSection
	$-E      00777EDD      8D45 F0                   LEA EAX, DWORD PTR SS:[EBP-0x10]
	$-B      00777EE0      C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-4      00777EE7      50                        PUSH EAX
	$-3      00777EE8      8D45 E8                   LEA EAX, DWORD PTR SS:[EBP-0x18]
	$ ==>    00777EEB      C745 F0 0070ED00          MOV DWORD PTR SS:[EBP-0x10], PathOfEx.00>; UNICODE "Data/GrantedEffectsPerLevel.dat"
	$+7      00777EF2      50                        PUSH EAX
	$+8      00777EF3      8BCB                      MOV ECX, EBX                             ; PathOfEx.01151EC8
	$+A      00777EF5      E8 26E9F9FF               CALL PathOfEx.00716820                   ; 获取文件db对象指针
	$+F      00777EFA      8B75 E8                   MOV ESI, DWORD PTR SS:[EBP-0x18]         ; PathOfEx.009EDEB1
	$+12     00777EFD      3B73 04                   CMP ESI, DWORD PTR DS:[EBX+0x4]
	$+15     00777F00      74 11                     JE SHORT PathOfEx.00777F13
	$+17     00777F02      FF76 0C                   PUSH DWORD PTR DS:[ESI+0xC]
	$+1A     00777F05      8BCB                      MOV ECX, EBX                             ; PathOfEx.01151EC8
	$+1C     00777F07      E8 C4E2F9FF               CALL PathOfEx.007161D0
	$+21     00777F0C      8B76 0C                   MOV ESI, DWORD PTR DS:[ESI+0xC]
	$+24     00777F0F      85F6                      TEST ESI, ESI
	$+26     00777F11      75 4C                     JNZ SHORT PathOfEx.00777F5F
	$+28     00777F13      6A 60                     PUSH 0x60
	$+2A     00777F15      E8 2C3F5200               CALL PathOfEx.00C9BE46
	$+2F     00777F1A      8945 E8                   MOV DWORD PTR SS:[EBP-0x18], EAX
	$+32     00777F1D      83C4 04                   ADD ESP, 0x4
	$+35     00777F20      8945 F0                   MOV DWORD PTR SS:[EBP-0x10], EAX
	$+38     00777F23      C645 FC 01                MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+3C     00777F27      85C0                      TEST EAX, EAX
	$+3E     00777F29      74 0E                     JE SHORT PathOfEx.00777F39
	$+40     00777F2B      83EC 10                   SUB ESP, 0x10
	$+43     00777F2E      8BC8                      MOV ECX, EAX
	$+45     00777F30      E8 BB000000               CALL PathOfEx.00777FF0
	$+4A     00777F35      8BF0                      MOV ESI, EAX
	$+4C     00777F37      EB 02                     JMP SHORT PathOfEx.00777F3B
	$+4E     00777F39      33F6                      XOR ESI, ESI
	$+50     00777F3B      8D46 14                   LEA EAX, DWORD PTR DS:[ESI+0x14]
	$+53     00777F3E      C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+57     00777F42      8378 14 08                CMP DWORD PTR DS:[EAX+0x14], 0x8
	$+5B     00777F46      72 02                     JB SHORT PathOfEx.00777F4A
	$+5D     00777F48      8B00                      MOV EAX, DWORD PTR DS:[EAX]              ; PathOfEx.009EDEB1
	$+5F     00777F4A      8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+62     00777F4D      8BCB                      MOV ECX, EBX                             ; PathOfEx.01151EC8
	$+64     00777F4F      8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+67     00777F52      8975 E8                   MOV DWORD PTR SS:[EBP-0x18], ESI
	$+6A     00777F55      50                        PUSH EAX
	$+6B     00777F56      8D45 EC                   LEA EAX, DWORD PTR SS:[EBP-0x14]
	$+6E     00777F59      50                        PUSH EAX
	$+6F     00777F5A      E8 61EDF9FF               CALL PathOfEx.00716CC0
	$+74     00777F5F      8D56 38                   LEA EDX, DWORD PTR DS:[ESI+0x38]
	$+77     00777F62      B9 01000000               MOV ECX, 0x1
	$+7C     00777F67      F0:0FC10A                 LOCK XADD DWORD PTR DS:[EDX], ECX
	$+80     00777F6B      8B0D 0C47FD00             MOV ECX, DWORD PTR DS:[0xFD470C]
	$+86     00777F71      894E 34                   MOV DWORD PTR DS:[ESI+0x34], ECX         ; PathOfEx.01151EC8
	$+89     00777F74      85FF                      TEST EDI, EDI                            ; PathOfEx.01151EF8
	$+8B     00777F76      74 07                     JE SHORT PathOfEx.00777F7F
	$+8D     00777F78      57                        PUSH EDI                                 ; PathOfEx.01151EF8
	$+8E     00777F79      FF15 34C1D800             CALL NEAR DWORD PTR DS:[0xD8C134]        ; ntdll_12.RtlLeaveCriticalSection
	$+94     00777F7F      8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+97     00777F82      8BC6                      MOV EAX, ESI
	$+99     00777F84      5F                        POP EDI                                  ; PathOfEx.00777EFA
	$+9A     00777F85      5E                        POP ESI                                  ; PathOfEx.00777EFA
	$+9B     00777F86      64:890D 00000000          MOV DWORD PTR FS:[0], ECX                ; PathOfEx.01151EC8
	$+A2     00777F8D      5B                        POP EBX                                  ; PathOfEx.00777EFA
	$+A3     00777F8E      8BE5                      MOV ESP, EBP
	$+A5     00777F90      5D                        POP EBP                                  ; PathOfEx.00777EFA
	$+A6     00777F91      C2 1000                   RETN 0x10
	$+A9     00777F94      CC                        INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("23????8b????8b????8b????03??8b????408d????89??????0f????????3b", -0x12);
	assert(addr_tmp);
	dw_tmp = *(pt_byte*)addr_tmp;
	assert(dw_tmp == 0x8b);
	ofs << "pt_cdword	g_c_find_db_file_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
