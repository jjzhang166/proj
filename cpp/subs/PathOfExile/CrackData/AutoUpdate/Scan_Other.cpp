#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Other(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp, addr_tmp1;
	//pt_char c_tmp;
	pt_dword dw_tmp;
	//boost::optional<pt_dword> op_dw_tmp;

	/*
	$-3F     005B0CAF     CC                        INT3
	$-3E     005B0CB0     51                        PUSH ECX
	$-3D     005B0CB1     E8 3AFFFFFF               CALL PathOfEx.005B0BF0
	$-38     005B0CB6     8378 14 08                CMP DWORD PTR DS:[EAX+0x14],0x8
	$-34     005B0CBA     8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$-31     005B0CBD     72 02                     JB SHORT PathOfEx.005B0CC1
	$-2F     005B0CBF     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$-2D     005B0CC1     8BC8                      MOV ECX,EAX
	$-2B     005B0CC3     E8 F8441600               CALL PathOfEx.007151C0
	$-26     005B0CC8     59                        POP ECX
	$-25     005B0CC9     C3                        RETN
	$-24     005B0CCA     CC                        INT3
	$-23     005B0CCB     CC                        INT3
	$-22     005B0CCC     CC                        INT3
	$-21     005B0CCD     CC                        INT3
	$-20     005B0CCE     CC                        INT3
	$-1F     005B0CCF     CC                        INT3                                               ; g_c_ui_create_item_tips_
	$-1E     005B0CD0     83EC 08                   SUB ESP,0x8                                        ; tag_ui创建物品tips时会断到,tag_无参创建物品tips
	$-1B     005B0CD3     53                        PUSH EBX                                           ; ecx=stCD_UiItemTips*
	$-1A     005B0CD4     8BD9                      MOV EBX,ECX
	$-18     005B0CD6     56                        PUSH ESI
	$-17     005B0CD7     57                        PUSH EDI
	$-16     005B0CD8     F683 C0080000 04          TEST BYTE PTR DS:[EBX+0x8C0],0x4
	$-F      005B0CDF     75 0D                     JNZ SHORT PathOfEx.005B0CEE
	$-D      005B0CE1     8B83 F0080000             MOV EAX,DWORD PTR DS:[EBX+0x8F0]
	$-7      005B0CE7     5F                        POP EDI
	$-6      005B0CE8     5E                        POP ESI
	$-5      005B0CE9     5B                        POP EBX
	$-4      005B0CEA     83C4 08                   ADD ESP,0x8
	$-1      005B0CED     C3                        RETN
	$ ==>    005B0CEE     83BB CC080000 00          CMP DWORD PTR DS:[EBX+0x8CC],0x0                   ; g_o_items_tips2item_
	$+7      005B0CF5     74 47                     JE SHORT PathOfEx.005B0D3E
	$+9      005B0CF7     8BBB C8080000             MOV EDI,DWORD PTR DS:[EBX+0x8C8]
	$+F      005B0CFD     8BCF                      MOV ECX,EDI
	$+11     005B0CFF     8B77 40                   MOV ESI,DWORD PTR DS:[EDI+0x40]
	$+14     005B0D02     E8 E97C2300               CALL PathOfEx.007E89F0
	$+19     005B0D07     8B83 C0080000             MOV EAX,DWORD PTR DS:[EBX+0x8C0]
	$+1F     005B0D0D     B9 F03A0101               MOV ECX,PathOfEx.01013AF0                          ; g_a_item_tips_params_
	$+24     005B0D12     FFB3 D4080000             PUSH DWORD PTR DS:[EBX+0x8D4]                      ; push 0
	$+2A     005B0D18     D1E8                      SHR EAX,1
	$+2C     005B0D1A     24 01                     AND AL,0x1
	$+2E     005B0D1C     0FB6C0                    MOVZX EAX,AL
	$+31     005B0D1F     50                        PUSH EAX                                           ; push 0
	$+32     005B0D20     56                        PUSH ESI                                           ; push 3f
	$+33     005B0D21     8D47 34                   LEA EAX,DWORD PTR DS:[EDI+0x34]
	$+36     005B0D24     50                        PUSH EAX
	$+37     005B0D25     6A 00                     PUSH 0x0
	$+39     005B0D27     FFB3 CC080000             PUSH DWORD PTR DS:[EBX+0x8CC]                      ; push stCD_EntityItemBased*,g_o_items_tips2item_
	$+3F     005B0D2D     E8 1E2F1700               CALL PathOfEx.00723C50                             ; call tag_索引物品属性并创建tips
	$+44     005B0D32     B8 F03A0101               MOV EAX,PathOfEx.01013AF0                          ; g_a_item_tips_params_
	$+49     005B0D37     5F                        POP EDI
	$+4A     005B0D38     5E                        POP ESI
	$+4B     005B0D39     5B                        POP EBX
	$+4C     005B0D3A     83C4 08                   ADD ESP,0x8
	$+4F     005B0D3D     C3                        RETN
	$+50     005B0D3E     8B83 D0080000             MOV EAX,DWORD PTR DS:[EBX+0x8D0]
	$+56     005B0D44     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+59     005B0D47     E8 84F8EBFF               CALL PathOfEx.004705D0
	$+5E     005B0D4C     85C0                      TEST EAX,EAX
	$+60     005B0D4E     74 0D                     JE SHORT PathOfEx.005B0D5D
	$+62     005B0D50     8378 6C 08                CMP DWORD PTR DS:[EAX+0x6C],0x8
	$+66     005B0D54     8D48 58                   LEA ECX,DWORD PTR DS:[EAX+0x58]
	$+69     005B0D57     72 09                     JB SHORT PathOfEx.005B0D62
	$+6B     005B0D59     8B09                      MOV ECX,DWORD PTR DS:[ECX]
	$+6D     005B0D5B     EB 05                     JMP SHORT PathOfEx.005B0D62
	$+6F     005B0D5D     B9 BC2CE700               MOV ECX,PathOfEx.00E72CBC
	$+74     005B0D62     8B93 D0080000             MOV EDX,DWORD PTR DS:[EBX+0x8D0]
	$+7A     005B0D68     8B82 10010000             MOV EAX,DWORD PTR DS:[EDX+0x110]
	$+80     005B0D6E     85C0                      TEST EAX,EAX
	$+82     005B0D70     74 05                     JE SHORT PathOfEx.005B0D77
	$+84     005B0D72     8B70 08                   MOV ESI,DWORD PTR DS:[EAX+0x8]
	$+87     005B0D75     EB 02                     JMP SHORT PathOfEx.005B0D79
	$+89     005B0D77     33F6                      XOR ESI,ESI
	$+8B     005B0D79     8A82 20010000             MOV AL,BYTE PTR DS:[EDX+0x120]
	$+91     005B0D7F     83C2 10                   ADD EDX,0x10
	$+94     005B0D82     884424 10                 MOV BYTE PTR SS:[ESP+0x10],AL
	$+98     005B0D86     56                        PUSH ESI
	$+99     005B0D87     FF7424 14                 PUSH DWORD PTR SS:[ESP+0x14]
	$+9D     005B0D8B     E8 E0F9FFFF               CALL PathOfEx.005B0770
	$+A2     005B0D90     83C4 08                   ADD ESP,0x8
	$+A5     005B0D93     5F                        POP EDI
	$+A6     005B0D94     5E                        POP ESI
	$+A7     005B0D95     5B                        POP EBX
	$+A8     005B0D96     83C4 08                   ADD ESP,0x8
	$+AB     005B0D99     C3                        RETN
	$+AC     005B0D9A     CC                        INT3
	$+AD     005B0D9B     CC                        INT3
	$+AE     005B0D9C     CC                        INT3
	$+AF     005B0D9D     CC                        INT3
	$+B0     005B0D9E     CC                        INT3
	$+B1     005B0D9F     CC                        INT3
	$+B2     005B0DA0     55                        PUSH EBP
	$+B3     005B0DA1     8BEC                      MOV EBP,ESP
	$+B5     005B0DA3     83E4 F8                   AND ESP,0xFFFFFFF8
	$+B8     005B0DA6     83EC 14                   SUB ESP,0x14
	$+BB     005B0DA9     56                        PUSH ESI
	$+BC     005B0DAA     8BF1                      MOV ESI,ECX
	$+BE     005B0DAC     F686 C0080000 04          TEST BYTE PTR DS:[ESI+0x8C0],0x4
	$+C5     005B0DB3     74 7C                     JE SHORT PathOfEx.005B0E31
	$+C7     005B0DB5     80BE BC080000 00          CMP BYTE PTR DS:[ESI+0x8BC],0x0
	$+CE     005B0DBC     74 32                     JE SHORT PathOfEx.005B0DF0
	$+D0     005B0DBE     8B8E B8080000             MOV ECX,DWORD PTR DS:[ESI+0x8B8]
	$+D6     005B0DC4     8D4424 08                 LEA EAX,DWORD PTR SS:[ESP+0x8]
	$+DA     005B0DC8     50                        PUSH EAX
	$+DB     005B0DC9     81C1 94150000             ADD ECX,0x1594
	$+E1     005B0DCF     C74424 0C 8C90E700        MOV DWORD PTR SS:[ESP+0xC],PathOfEx.00E7908C
	$+E9     005B0DD7     897424 10                 MOV DWORD PTR SS:[ESP+0x10],ESI
	$+ED     005B0DDB     C74424 14 00105B00        MOV DWORD PTR SS:[ESP+0x14],PathOfEx.005B1000
	$+F5     005B0DE3     C74424 18 00000000        MOV DWORD PTR SS:[ESP+0x18],0x0
	$+FD     005B0DEB     E8 E090F1FF               CALL PathOfEx.004C9ED0
	$+102    005B0DF0     8B8E C8080000             MOV ECX,DWORD PTR DS:[ESI+0x8C8]
	$+108    005B0DF6     8D86 B0080000             LEA EAX,DWORD PTR DS:[ESI+0x8B0]
	$+10E    005B0DFC     894424 04                 MOV DWORD PTR SS:[ESP+0x4],EAX
	$+112    005B0E00     81C1 D4020000             ADD ECX,0x2D4
	$+118    005B0E06     8D4424 04                 LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+11C    005B0E0A     50                        PUSH EAX
	$+11D    005B0E0B     E8 000AEFFF               CALL PathOfEx.004A1810
	$+122    005B0E10     8B86 C8080000             MOV EAX,DWORD PTR DS:[ESI+0x8C8]
	$+128    005B0E16     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+12B    005B0E19     E8 52B0EBFF               CALL PathOfEx.0046BE70
	$+130    005B0E1E     8D8E B4080000             LEA ECX,DWORD PTR DS:[ESI+0x8B4]
	$+136    005B0E24     51                        PUSH ECX
	$+137    005B0E25     8BC8                      MOV ECX,EAX
	$+139    005B0E27     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+13B    005B0E29     FF52 34                   CALL DWORD PTR DS:[EDX+0x34]
	$+13E    005B0E2C     5E                        POP ESI
	$+13F    005B0E2D     8BE5                      MOV ESP,EBP
	$+141    005B0E2F     5D                        POP EBP
	$+142    005B0E30     C3                        RETN
	$+143    005B0E31     E8 0A000000               CALL PathOfEx.005B0E40
	$+148    005B0E36     5E                        POP ESI
	$+149    005B0E37     8BE5                      MOV ESP,EBP
	$+14B    005B0E39     5D                        POP EBP
	$+14C    005B0E3A     C3                        RETN
	$+14D    005B0E3B     CC                        INT3
	*/

	//gj3
	/*
	$-21F    004767CF     CC                       INT3
	$-21E    004767D0     55                       PUSH EBP
	$-21D    004767D1     8BEC                     MOV EBP,ESP
	$-21B    004767D3     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-215    004767D9     6A FF                    PUSH -0x1
	$-213    004767DB     68 B0D4D500              PUSH PathOfEx.00D5D4B0
	$-20E    004767E0     50                       PUSH EAX
	$-20D    004767E1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-206    004767E8     53                       PUSH EBX
	$-205    004767E9     56                       PUSH ESI
	$-204    004767EA     8BF1                     MOV ESI,ECX
	$-202    004767EC     57                       PUSH EDI
	$-201    004767ED     8DBE B4080000            LEA EDI,DWORD PTR DS:[ESI+0x8B4]
	$-1FB    004767F3     C786 B8080000 FCA2ED00   MOV DWORD PTR DS:[ESI+0x8B8],PathOfEx.00EDA2FC
	$-1F1    004767FD     8D9E B8080000            LEA EBX,DWORD PTR DS:[ESI+0x8B8]
	$-1EB    00476803     C707 F0B3ED00            MOV DWORD PTR DS:[EDI],PathOfEx.00EDB3F0
	$-1E5    00476809     F686 C4080000 04         TEST BYTE PTR DS:[ESI+0x8C4],0x4
	$-1DE    00476810     C706 F434EF00            MOV DWORD PTR DS:[ESI],PathOfEx.00EF34F4
	$-1D8    00476816     C786 3C080000 30E5EF00   MOV DWORD PTR DS:[ESI+0x83C],PathOfEx.00EFE530
	$-1CE    00476820     C786 40080000 5C24F000   MOV DWORD PTR DS:[ESI+0x840],PathOfEx.00F0245C
	$-1C4    0047682A     74 44                    JE SHORT PathOfEx.00476870
	$-1C2    0047682C     80BE C0080000 00         CMP BYTE PTR DS:[ESI+0x8C0],0x0
	$-1BB    00476833     74 19                    JE SHORT PathOfEx.0047684E
	$-1B9    00476835     8B8E BC080000            MOV ECX,DWORD PTR DS:[ESI+0x8BC]
	$-1B3    0047683B     6A 00                    PUSH 0x0
	$-1B1    0047683D     68 206D4700              PUSH PathOfEx.00476D20
	$-1AC    00476842     56                       PUSH ESI
	$-1AB    00476843     81C1 14170000            ADD ECX,0x1714
	$-1A5    00476849     E8 C23EEEFF              CALL PathOfEx.0035A710
	$-1A0    0047684E     8B8E CC080000            MOV ECX,DWORD PTR DS:[ESI+0x8CC]
	$-19A    00476854     57                       PUSH EDI
	$-199    00476855     E8 8693EEFF              CALL PathOfEx.0035FBE0
	$-194    0047685A     8B86 CC080000            MOV EAX,DWORD PTR DS:[ESI+0x8CC]
	$-18E    00476860     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-18B    00476863     E8 48E1EAFF              CALL PathOfEx.003249B0
	$-186    00476868     53                       PUSH EBX
	$-185    00476869     8BC8                     MOV ECX,EAX
	$-183    0047686B     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$-181    0047686D     FF52 38                  CALL DWORD PTR DS:[EDX+0x38]
	$-17E    00476870     8BBE F4080000            MOV EDI,DWORD PTR DS:[ESI+0x8F4]
	$-178    00476876     85FF                     TEST EDI,EDI
	$-176    00476878     74 15                    JE SHORT PathOfEx.0047688F
	$-174    0047687A     8BCF                     MOV ECX,EDI
	$-172    0047687C     E8 AFE0FFFF              CALL PathOfEx.00474930
	$-16D    00476881     68 F0020000              PUSH 0x2F0
	$-168    00476886     57                       PUSH EDI
	$-167    00476887     E8 71398500              CALL PathOfEx.00CCA1FD
	$-162    0047688C     83C4 08                  ADD ESP,0x8
	$-15F    0047688F     8B86 F0080000            MOV EAX,DWORD PTR DS:[ESI+0x8F0]
	$-159    00476895     8DBE DC080000            LEA EDI,DWORD PTR DS:[ESI+0x8DC]
	$-153    0047689B     83F8 08                  CMP EAX,0x8
	$-150    0047689E     72 0E                    JB SHORT PathOfEx.004768AE
	$-14E    004768A0     6A 02                    PUSH 0x2
	$-14C    004768A2     40                       INC EAX
	$-14B    004768A3     50                       PUSH EAX
	$-14A    004768A4     FF37                     PUSH DWORD PTR DS:[EDI]
	$-148    004768A6     E8 C5EAEAFF              CALL PathOfEx.00325370
	$-143    004768AB     83C4 0C                  ADD ESP,0xC
	$-140    004768AE     C747 14 07000000         MOV DWORD PTR DS:[EDI+0x14],0x7
	$-139    004768B5     837F 14 08               CMP DWORD PTR DS:[EDI+0x14],0x8
	$-135    004768B9     C747 10 00000000         MOV DWORD PTR DS:[EDI+0x10],0x0
	$-12E    004768C0     72 02                    JB SHORT PathOfEx.004768C4
	$-12C    004768C2     8B3F                     MOV EDI,DWORD PTR DS:[EDI]
	$-12A    004768C4     33C0                     XOR EAX,EAX
	$-128    004768C6     8BCE                     MOV ECX,ESI
	$-126    004768C8     66:8907                  MOV WORD PTR DS:[EDI],AX
	$-123    004768CB     E8 E082EFFF              CALL PathOfEx.0036EBB0
	$-11E    004768D0     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-11B    004768D3     5F                       POP EDI
	$-11A    004768D4     5E                       POP ESI
	$-119    004768D5     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-112    004768DC     5B                       POP EBX
	$-111    004768DD     8BE5                     MOV ESP,EBP
	$-10F    004768DF     5D                       POP EBP
	$-10E    004768E0     C3                       RETN
	$-10D    004768E1     CC                       INT3
	$-10C    004768E2     CC                       INT3
	$-10B    004768E3     CC                       INT3
	$-10A    004768E4     CC                       INT3
	$-109    004768E5     CC                       INT3
	$-108    004768E6     CC                       INT3
	$-107    004768E7     CC                       INT3
	$-106    004768E8     CC                       INT3
	$-105    004768E9     CC                       INT3
	$-104    004768EA     CC                       INT3
	$-103    004768EB     CC                       INT3
	$-102    004768EC     CC                       INT3
	$-101    004768ED     CC                       INT3
	$-100    004768EE     CC                       INT3
	$-FF     004768EF     CC                       INT3
	$-FE     004768F0     55                       PUSH EBP
	$-FD     004768F1     8BEC                     MOV EBP,ESP
	$-FB     004768F3     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-F5     004768F9     6A FF                    PUSH -0x1
	$-F3     004768FB     68 9801D700              PUSH PathOfEx.00D70198
	$-EE     00476900     50                       PUSH EAX
	$-ED     00476901     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-E6     00476908     83EC 6C                  SUB ESP,0x6C
	$-E3     0047690B     56                       PUSH ESI
	$-E2     0047690C     57                       PUSH EDI
	$-E1     0047690D     8BF9                     MOV EDI,ECX
	$-DF     0047690F     83BF EC080000 00         CMP DWORD PTR DS:[EDI+0x8EC],0x0
	$-D8     00476916     8DB7 DC080000            LEA ESI,DWORD PTR DS:[EDI+0x8DC]
	$-D2     0047691C     75 79                    JNZ SHORT PathOfEx.00476997
	$-D0     0047691E     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$-C9     00476925     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$-C2     0047692C     C745 F0 00000000         MOV DWORD PTR SS:[EBP-0x10],0x0
	$-BB     00476933     83EC 10                  SUB ESP,0x10
	$-B8     00476936     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-B1     0047693D     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
	$-AE     00476940     C745 88 D0A8FA00         MOV DWORD PTR SS:[EBP-0x78],PathOfEx.00FAA8D0
	$-A7     00476947     6A FF                    PUSH -0x1
	$-A5     00476949     83EC 0C                  SUB ESP,0xC
	$-A2     0047694C     E8 CF9C4C00              CALL PathOfEx.00940620
	$-9D     00476951     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$-99     00476955     803D 73720801 00         CMP BYTE PTR DS:[0x1087273],0x0
	$-92     0047695C     C645 E0 00               MOV BYTE PTR SS:[EBP-0x20],0x0
	$-8E     00476960     75 05                    JNZ SHORT PathOfEx.00476967
	$-8C     00476962     E8 69BB4C00              CALL PathOfEx.009424D0
	$-87     00476967     8BCF                     MOV ECX,EDI
	$-85     00476969     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$-81     0047696D     E8 5E000000              CALL PathOfEx.004769D0
	$-7C     00476972     8BC8                     MOV ECX,EAX
	$-7A     00476974     E8 B7661A00              CALL PathOfEx.0061D030
	$-75     00476979     50                       PUSH EAX
	$-74     0047697A     56                       PUSH ESI
	$-73     0047697B     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-70     0047697E     50                       PUSH EAX
	$-6F     0047697F     8D4D 88                  LEA ECX,DWORD PTR SS:[EBP-0x78]
	$-6C     00476982     E8 89A7EFFF              CALL PathOfEx.00371110
	$-67     00476987     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
	$-64     0047698A     E8 21B4EFFF              CALL PathOfEx.00371DB0
	$-5F     0047698F     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-5C     00476992     E8 A98EF0FF              CALL PathOfEx.0037F840
	$-57     00476997     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-54     0047699A     8BC6                     MOV EAX,ESI
	$-52     0047699C     5F                       POP EDI
	$-51     0047699D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-4A     004769A4     5E                       POP ESI
	$-49     004769A5     8BE5                     MOV ESP,EBP
	$-47     004769A7     5D                       POP EBP
	$-46     004769A8     C3                       RETN
	$-45     004769A9     CC                       INT3
	$-44     004769AA     CC                       INT3
	$-43     004769AB     CC                       INT3
	$-42     004769AC     CC                       INT3
	$-41     004769AD     CC                       INT3
	$-40     004769AE     CC                       INT3
	$-3F     004769AF     CC                       INT3
	$-3E     004769B0     51                       PUSH ECX
	$-3D     004769B1     E8 3AFFFFFF              CALL PathOfEx.004768F0
	$-38     004769B6     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
	$-34     004769BA     8B50 10                  MOV EDX,DWORD PTR DS:[EAX+0x10]
	$-31     004769BD     72 02                    JB SHORT PathOfEx.004769C1
	$-2F     004769BF     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-2D     004769C1     8BC8                     MOV ECX,EAX
	$-2B     004769C3     E8 781D1900              CALL PathOfEx.00608740
	$-26     004769C8     59                       POP ECX
	$-25     004769C9     C3                       RETN
	$-24     004769CA     CC                       INT3
	$-23     004769CB     CC                       INT3
	$-22     004769CC     CC                       INT3
	$-21     004769CD     CC                       INT3
	$-20     004769CE     CC                       INT3
	$-1F     004769CF     CC                       INT3
	$-1E     004769D0     83EC 08                  SUB ESP,0x8                                      ; g_c_ui_create_item_tips_
	$-1B     004769D3     53                       PUSH EBX                                         ; tag_ui创建物品tips时会断到,tag_无参创建物品tips
	$-1A     004769D4     8BD9                     MOV EBX,ECX                                      ; ecx=stCD_UiItemTips*
	$-18     004769D6     56                       PUSH ESI
	$-17     004769D7     57                       PUSH EDI
	$-16     004769D8     F683 C4080000 04         TEST BYTE PTR DS:[EBX+0x8C4],0x4
	$-F      004769DF     75 0D                    JNZ SHORT PathOfEx.004769EE
	$-D      004769E1     8B83 F4080000            MOV EAX,DWORD PTR DS:[EBX+0x8F4]
	$-7      004769E7     5F                       POP EDI
	$-6      004769E8     5E                       POP ESI
	$-5      004769E9     5B                       POP EBX
	$-4      004769EA     83C4 08                  ADD ESP,0x8
	$-1      004769ED     C3                       RETN
	$ ==>    004769EE     83BB D0080000 00         CMP DWORD PTR DS:[EBX+0x8D0],0x0                 ; g_o_items_tips2item_
	$+7      004769F5     74 48                    JE SHORT PathOfEx.00476A3F
	$+9      004769F7     8BBB CC080000            MOV EDI,DWORD PTR DS:[EBX+0x8CC]
	$+F      004769FD     8BCF                     MOV ECX,EDI
	$+11     004769FF     0FB677 40                MOVZX ESI,BYTE PTR DS:[EDI+0x40]
	$+15     00476A03     E8 88F42600              CALL PathOfEx.006E5E90
	$+1A     00476A08     8B83 C4080000            MOV EAX,DWORD PTR DS:[EBX+0x8C4]
	$+20     00476A0E     B9 00A90A01              MOV ECX,PathOfEx.010AA900                        ; g_a_item_tips_params_
	$+25     00476A13     FFB3 D8080000            PUSH DWORD PTR DS:[EBX+0x8D8]
	$+2B     00476A19     D1E8                     SHR EAX,1
	$+2D     00476A1B     24 01                    AND AL,0x1
	$+2F     00476A1D     0FB6C0                   MOVZX EAX,AL
	$+32     00476A20     50                       PUSH EAX
	$+33     00476A21     56                       PUSH ESI
	$+34     00476A22     8D47 34                  LEA EAX,DWORD PTR DS:[EDI+0x34]
	$+37     00476A25     50                       PUSH EAX
	$+38     00476A26     6A 00                    PUSH 0x0
	$+3A     00476A28     FFB3 D0080000            PUSH DWORD PTR DS:[EBX+0x8D0]                    ; push stCD_EntityItemBased*,g_o_items_tips2item_
	$+40     00476A2E     E8 5DF71900              CALL PathOfEx.00616190                           ; call tag_索引物品属性并创建tips
	$+45     00476A33     B8 00A90A01              MOV EAX,PathOfEx.010AA900                        ; g_a_item_tips_params_
	$+4A     00476A38     5F                       POP EDI
	$+4B     00476A39     5E                       POP ESI
	$+4C     00476A3A     5B                       POP EBX
	$+4D     00476A3B     83C4 08                  ADD ESP,0x8
	$+50     00476A3E     C3                       RETN
	$+51     00476A3F     8B83 D4080000            MOV EAX,DWORD PTR DS:[EBX+0x8D4]
	$+57     00476A45     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+5A     00476A48     E8 A337EDFF              CALL PathOfEx.0034A1F0
	$+5F     00476A4D     85C0                     TEST EAX,EAX
	$+61     00476A4F     74 0D                    JE SHORT PathOfEx.00476A5E
	$+63     00476A51     8378 6C 08               CMP DWORD PTR DS:[EAX+0x6C],0x8
	$+67     00476A55     8D48 58                  LEA ECX,DWORD PTR DS:[EAX+0x58]
	$+6A     00476A58     72 09                    JB SHORT PathOfEx.00476A63
	$+6C     00476A5A     8B09                     MOV ECX,DWORD PTR DS:[ECX]
	$+6E     00476A5C     EB 05                    JMP SHORT PathOfEx.00476A63
	$+70     00476A5E     B9 782BED00              MOV ECX,PathOfEx.00ED2B78
	$+75     00476A63     8B93 D4080000            MOV EDX,DWORD PTR DS:[EBX+0x8D4]
	$+7B     00476A69     8B42 0C                  MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+7E     00476A6C     8BB2 10010000            MOV ESI,DWORD PTR DS:[EDX+0x110]
	$+84     00476A72     8A40 0D                  MOV AL,BYTE PTR DS:[EAX+0xD]
	$+87     00476A75     884424 0C                MOV BYTE PTR SS:[ESP+0xC],AL
	$+8B     00476A79     85F6                     TEST ESI,ESI
	$+8D     00476A7B     74 05                    JE SHORT PathOfEx.00476A82
	$+8F     00476A7D     8B76 08                  MOV ESI,DWORD PTR DS:[ESI+0x8]
	$+92     00476A80     EB 02                    JMP SHORT PathOfEx.00476A84
	$+94     00476A82     33F6                     XOR ESI,ESI
	$+96     00476A84     8A82 20010000            MOV AL,BYTE PTR DS:[EDX+0x120]
	$+9C     00476A8A     83C2 10                  ADD EDX,0x10
	$+9F     00476A8D     FF7424 0C                PUSH DWORD PTR SS:[ESP+0xC]
	$+A3     00476A91     884424 14                MOV BYTE PTR SS:[ESP+0x14],AL
	$+A7     00476A95     56                       PUSH ESI
	$+A8     00476A96     FF7424 18                PUSH DWORD PTR SS:[ESP+0x18]
	$+AC     00476A9A     E8 D1F9FFFF              CALL PathOfEx.00476470
	$+B1     00476A9F     83C4 0C                  ADD ESP,0xC
	$+B4     00476AA2     5F                       POP EDI
	$+B5     00476AA3     5E                       POP ESI
	$+B6     00476AA4     5B                       POP EBX
	$+B7     00476AA5     83C4 08                  ADD ESP,0x8
	$+BA     00476AA8     C3                       RETN
	$+BB     00476AA9     CC                       INT3
	$+BC     00476AAA     CC                       INT3
	$+BD     00476AAB     CC                       INT3
	$+BE     00476AAC     CC                       INT3
	$+BF     00476AAD     CC                       INT3
	$+C0     00476AAE     CC                       INT3
	$+C1     00476AAF     CC                       INT3
	$+C2     00476AB0     55                       PUSH EBP
	$+C3     00476AB1     8BEC                     MOV EBP,ESP
	$+C5     00476AB3     83E4 F8                  AND ESP,0xFFFFFFF8
	$+C8     00476AB6     83EC 14                  SUB ESP,0x14
	$+CB     00476AB9     56                       PUSH ESI
	$+CC     00476ABA     8BF1                     MOV ESI,ECX
	$+CE     00476ABC     F686 C4080000 04         TEST BYTE PTR DS:[ESI+0x8C4],0x4
	$+D5     00476AC3     74 7C                    JE SHORT PathOfEx.00476B41
	$+D7     00476AC5     80BE C0080000 00         CMP BYTE PTR DS:[ESI+0x8C0],0x0
	$+DE     00476ACC     74 32                    JE SHORT PathOfEx.00476B00
	$+E0     00476ACE     8B8E BC080000            MOV ECX,DWORD PTR DS:[ESI+0x8BC]
	$+E6     00476AD4     8D4424 08                LEA EAX,DWORD PTR SS:[ESP+0x8]
	$+EA     00476AD8     50                       PUSH EAX
	$+EB     00476AD9     81C1 14170000            ADD ECX,0x1714
	$+F1     00476ADF     C74424 0C 0088ED00       MOV DWORD PTR SS:[ESP+0xC],PathOfEx.00ED8800
	$+F9     00476AE7     897424 10                MOV DWORD PTR SS:[ESP+0x10],ESI
	$+FD     00476AEB     C74424 14 206D4700       MOV DWORD PTR SS:[ESP+0x14],PathOfEx.00476D20
	$+105    00476AF3     C74424 18 00000000       MOV DWORD PTR SS:[ESP+0x18],0x0
	$+10D    00476AFB     E8 20D3F0FF              CALL PathOfEx.00383E20
	$+112    00476B00     8B8E CC080000            MOV ECX,DWORD PTR DS:[ESI+0x8CC]
	$+118    00476B06     8D86 B4080000            LEA EAX,DWORD PTR DS:[ESI+0x8B4]
	$+11E    00476B0C     894424 04                MOV DWORD PTR SS:[ESP+0x4],EAX
	$+122    00476B10     81C1 00020000            ADD ECX,0x200
	$+128    00476B16     8D4424 04                LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+12C    00476B1A     50                       PUSH EAX
	$+12D    00476B1B     E8 B07AEEFF              CALL PathOfEx.0035E5D0
	$+132    00476B20     8B86 CC080000            MOV EAX,DWORD PTR DS:[ESI+0x8CC]
	$+138    00476B26     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+13B    00476B29     E8 82DEEAFF              CALL PathOfEx.003249B0
	$+140    00476B2E     8D8E B8080000            LEA ECX,DWORD PTR DS:[ESI+0x8B8]
	$+146    00476B34     51                       PUSH ECX
	$+147    00476B35     8BC8                     MOV ECX,EAX
	$+149    00476B37     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$+14B    00476B39     FF52 34                  CALL DWORD PTR DS:[EDX+0x34]
	$+14E    00476B3C     5E                       POP ESI
	$+14F    00476B3D     8BE5                     MOV ESP,EBP
	$+151    00476B3F     5D                       POP EBP
	$+152    00476B40     C3                       RETN
	$+153    00476B41     E8 0A000000              CALL PathOfEx.00476B50
	$+158    00476B46     5E                       POP ESI
	$+159    00476B47     8BE5                     MOV ESP,EBP
	$+15B    00476B49     5D                       POP EBP
	$+15C    00476B4A     C3                       RETN
	$+15D    00476B4B     CC                       INT3
	$+15E    00476B4C     CC                       INT3
	$+15F    00476B4D     CC                       INT3
	$+160    00476B4E     CC                       INT3
	$+161    00476B4F     CC                       INT3
	$+162    00476B50     55                       PUSH EBP
	$+163    00476B51     8BEC                     MOV EBP,ESP
	$+165    00476B53     6A FF                    PUSH -0x1
	$+167    00476B55     68 D401D700              PUSH PathOfEx.00D701D4
	$+16C    00476B5A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+172    00476B60     50                       PUSH EAX
	$+173    00476B61     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+17A    00476B68     83EC 10                  SUB ESP,0x10
	$+17D    00476B6B     53                       PUSH EBX
	$+17E    00476B6C     56                       PUSH ESI
	$+17F    00476B6D     8BF1                     MOV ESI,ECX
	$+181    00476B6F     57                       PUSH EDI
	$+182    00476B70     8975 F0                  MOV DWORD PTR SS:[EBP-0x10],ESI
	$+185    00476B73     83BE D0080000 00         CMP DWORD PTR DS:[ESI+0x8D0],0x0
	$+18C    00476B7A     74 59                    JE SHORT PathOfEx.00476BD5
	$+18E    00476B7C     68 F0020000              PUSH 0x2F0
	$+193    00476B81     E8 85368500              CALL PathOfEx.00CCA20B
	$+198    00476B86     8BD8                     MOV EBX,EAX
	$+19A    00476B88     83C4 04                  ADD ESP,0x4
	$+19D    00476B8B     895D EC                  MOV DWORD PTR SS:[EBP-0x14],EBX
	$+1A0    00476B8E     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1A7    00476B95     8BBE CC080000            MOV EDI,DWORD PTR DS:[ESI+0x8CC]
	$+1AD    00476B9B     8BCF                     MOV ECX,EDI
	$+1AF    00476B9D     0FB677 40                MOVZX ESI,BYTE PTR DS:[EDI+0x40]
	$+1B3    00476BA1     E8 EAF22600              CALL PathOfEx.006E5E90
	$+1B8    00476BA6     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+1BB    00476BA9     FFB0 D8080000            PUSH DWORD PTR DS:[EAX+0x8D8]
	$+1C1    00476BAF     8B80 C4080000            MOV EAX,DWORD PTR DS:[EAX+0x8C4]
	$+1C7    00476BB5     D1E8                     SHR EAX,1
	$+1C9    00476BB7     24 01                    AND AL,0x1
	$+1CB    00476BB9     0FB6C0                   MOVZX EAX,AL
	$+1CE    00476BBC     50                       PUSH EAX
	$+1CF    00476BBD     56                       PUSH ESI
	$+1D0    00476BBE     8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+1D3    00476BC1     8D47 34                  LEA EAX,DWORD PTR DS:[EDI+0x34]
	$+1D6    00476BC4     50                       PUSH EAX
	$+1D7    00476BC5     51                       PUSH ECX
	$+1D8    00476BC6     8BCB                     MOV ECX,EBX
	$+1DA    00476BC8     FFB6 D0080000            PUSH DWORD PTR DS:[ESI+0x8D0]
	$+1E0    00476BCE     E8 6DEF1900              CALL PathOfEx.00615B40
	$+1E5    00476BD3     EB 79                    JMP SHORT PathOfEx.00476C4E
	$+1E7    00476BD5     8B86 D4080000            MOV EAX,DWORD PTR DS:[ESI+0x8D4]
	$+1ED    00476BDB     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+1F0    00476BDE     E8 0D36EDFF              CALL PathOfEx.0034A1F0
	$+1F5    00476BE3     85C0                     TEST EAX,EAX
	$+1F7    00476BE5     74 0D                    JE SHORT PathOfEx.00476BF4
	$+1F9    00476BE7     8378 6C 08               CMP DWORD PTR DS:[EAX+0x6C],0x8
	$+1FD    00476BEB     8D78 58                  LEA EDI,DWORD PTR DS:[EAX+0x58]
	$+200    00476BEE     72 09                    JB SHORT PathOfEx.00476BF9
	$+202    00476BF0     8B3F                     MOV EDI,DWORD PTR DS:[EDI]
	$+204    00476BF2     EB 05                    JMP SHORT PathOfEx.00476BF9
	$+206    00476BF4     BF 782BED00              MOV EDI,PathOfEx.00ED2B78
	$+20B    00476BF9     68 F0020000              PUSH 0x2F0
	$+210    00476BFE     E8 08368500              CALL PathOfEx.00CCA20B
	$+215    00476C03     8BD8                     MOV EBX,EAX
	$+217    00476C05     83C4 04                  ADD ESP,0x4
	$+21A    00476C08     895D E8                  MOV DWORD PTR SS:[EBP-0x18],EBX
	$+21D    00476C0B     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+224    00476C12     8B96 D4080000            MOV EDX,DWORD PTR DS:[ESI+0x8D4]
	$+22A    00476C18     8B4A 0C                  MOV ECX,DWORD PTR DS:[EDX+0xC]
	$+22D    00476C1B     8A49 0D                  MOV CL,BYTE PTR DS:[ECX+0xD]
	$+230    00476C1E     884D F0                  MOV BYTE PTR SS:[EBP-0x10],CL
	$+233    00476C21     8B8A 10010000            MOV ECX,DWORD PTR DS:[EDX+0x110]
	$+239    00476C27     85C9                     TEST ECX,ECX
	$+23B    00476C29     74 05                    JE SHORT PathOfEx.00476C30
	$+23D    00476C2B     8B49 08                  MOV ECX,DWORD PTR DS:[ECX+0x8]
	$+240    00476C2E     EB 02                    JMP SHORT PathOfEx.00476C32
	$+242    00476C30     33C9                     XOR ECX,ECX
	$+244    00476C32     FF75 F0                  PUSH DWORD PTR SS:[EBP-0x10]
	$+247    00476C35     8A82 20010000            MOV AL,BYTE PTR DS:[EDX+0x120]
	$+24D    00476C3B     51                       PUSH ECX
	$+24E    00476C3C     8845 EC                  MOV BYTE PTR SS:[EBP-0x14],AL
	$+251    00476C3F     8BCB                     MOV ECX,EBX
	$+253    00476C41     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+256    00476C44     8D42 10                  LEA EAX,DWORD PTR DS:[EDX+0x10]
	$+259    00476C47     50                       PUSH EAX
	$+25A    00476C48     57                       PUSH EDI
	$+25B    00476C49     E8 12F21900              CALL PathOfEx.00615E60
	$+260    00476C4E     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+267    00476C55     8BBE F4080000            MOV EDI,DWORD PTR DS:[ESI+0x8F4]
	$+26D    00476C5B     8986 F4080000            MOV DWORD PTR DS:[ESI+0x8F4],EAX
	$+273    00476C61     85FF                     TEST EDI,EDI
	$+275    00476C63     74 15                    JE SHORT PathOfEx.00476C7A
	$+277    00476C65     8BCF                     MOV ECX,EDI
	$+279    00476C67     E8 C4DCFFFF              CALL PathOfEx.00474930
	$+27E    00476C6C     68 F0020000              PUSH 0x2F0
	$+283    00476C71     57                       PUSH EDI
	$+284    00476C72     E8 86358500              CALL PathOfEx.00CCA1FD
	$+289    00476C77     83C4 08                  ADD ESP,0x8
	$+28C    00476C7A     8B8E F4080000            MOV ECX,DWORD PTR DS:[ESI+0x8F4]
	$+292    00476C80     E8 AB631A00              CALL PathOfEx.0061D030
	$+297    00476C85     8D8E DC080000            LEA ECX,DWORD PTR DS:[ESI+0x8DC]
	$+29D    00476C8B     3BC8                     CMP ECX,EAX
	$+29F    00476C8D     74 0A                    JE SHORT PathOfEx.00476C99
	$+2A1    00476C8F     6A FF                    PUSH -0x1
	$+2A3    00476C91     6A 00                    PUSH 0x0
	$+2A5    00476C93     50                       PUSH EAX
	$+2A6    00476C94     E8 47F7EAFF              CALL PathOfEx.003263E0
	$+2AB    00476C99     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2AE    00476C9C     5F                       POP EDI
	$+2AF    00476C9D     5E                       POP ESI
	$+2B0    00476C9E     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+2B7    00476CA5     5B                       POP EBX
	$+2B8    00476CA6     8BE5                     MOV ESP,EBP
	$+2BA    00476CA8     5D                       POP EBP
	$+2BB    00476CA9     C3                       RETN
	$+2BC    00476CAA     CC                       INT3
	$+2BD    00476CAB     CC                       INT3
	$+2BE    00476CAC     CC                       INT3
	$+2BF    00476CAD     CC                       INT3
	$+2C0    00476CAE     CC                       INT3
	$+2C1    00476CAF     CC                       INT3
	$+2C2    00476CB0     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+2C6    00476CB4     3D 43020000              CMP EAX,0x243
	$+2CB    00476CB9     74 0E                    JE SHORT PathOfEx.00476CC9
	$+2CD    00476CBB     3D 49020000              CMP EAX,0x249
	$+2D2    00476CC0     74 07                    JE SHORT PathOfEx.00476CC9
	$+2D4    00476CC2     3D 46020000              CMP EAX,0x246
	$+2D9    00476CC7     75 21                    JNZ SHORT PathOfEx.00476CEA
	$+2DB    00476CC9     8079 40 00               CMP BYTE PTR DS:[ECX+0x40],0x0
	$+2DF    00476CCD     75 1B                    JNZ SHORT PathOfEx.00476CEA
	$+2E1    00476CCF     8D41 24                  LEA EAX,DWORD PTR DS:[ECX+0x24]
	$+2E4    00476CD2     C641 40 01               MOV BYTE PTR DS:[ECX+0x40],0x1
	$+2E8    00476CD6     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
	$+2EC    00476CDA     C740 10 00000000         MOV DWORD PTR DS:[EAX+0x10],0x0
	$+2F3    00476CE1     72 02                    JB SHORT PathOfEx.00476CE5
	$+2F5    00476CE3     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+2F7    00476CE5     33C9                     XOR ECX,ECX
	$+2F9    00476CE7     66:8908                  MOV WORD PTR DS:[EAX],CX
	$+2FC    00476CEA     C2 0800                  RETN 0x8
	$+2FF    00476CED     CC                       INT3
	$+300    00476CEE     CC                       INT3
	$+301    00476CEF     CC                       INT3
	$+302    00476CF0     8079 44 00               CMP BYTE PTR DS:[ECX+0x44],0x0
	$+306    00476CF4     75 1B                    JNZ SHORT PathOfEx.00476D11
	$+308    00476CF6     8D41 28                  LEA EAX,DWORD PTR DS:[ECX+0x28]
	$+30B    00476CF9     C641 44 01               MOV BYTE PTR DS:[ECX+0x44],0x1
	$+30F    00476CFD     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
	$+313    00476D01     C740 10 00000000         MOV DWORD PTR DS:[EAX+0x10],0x0
	$+31A    00476D08     72 02                    JB SHORT PathOfEx.00476D0C
	$+31C    00476D0A     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+31E    00476D0C     33C9                     XOR ECX,ECX
	$+320    00476D0E     66:8908                  MOV WORD PTR DS:[EAX],CX
	$+323    00476D11     C2 0400                  RETN 0x4
	$+326    00476D14     CC                       INT3
	$+327    00476D15     CC                       INT3
	$+328    00476D16     CC                       INT3
	$+329    00476D17     CC                       INT3
	$+32A    00476D18     CC                       INT3
	$+32B    00476D19     CC                       INT3
	$+32C    00476D1A     CC                       INT3
	$+32D    00476D1B     CC                       INT3
	$+32E    00476D1C     CC                       INT3
	$+32F    00476D1D     CC                       INT3
	$+330    00476D1E     CC                       INT3
	$+331    00476D1F     CC                       INT3
	$+332    00476D20     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+336    00476D24     3B81 D0080000            CMP EAX,DWORD PTR DS:[ECX+0x8D0]
	$+33C    00476D2A     75 2A                    JNZ SHORT PathOfEx.00476D56
	$+33E    00476D2C     80B9 F8080000 00         CMP BYTE PTR DS:[ECX+0x8F8],0x0
	$+345    00476D33     75 21                    JNZ SHORT PathOfEx.00476D56
	$+347    00476D35     8D81 DC080000            LEA EAX,DWORD PTR DS:[ECX+0x8DC]
	$+34D    00476D3B     C681 F8080000 01         MOV BYTE PTR DS:[ECX+0x8F8],0x1
	$+354    00476D42     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
	$+358    00476D46     C740 10 00000000         MOV DWORD PTR DS:[EAX+0x10],0x0
	$+35F    00476D4D     72 02                    JB SHORT PathOfEx.00476D51
	$+361    00476D4F     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+363    00476D51     33C9                     XOR ECX,ECX
	$+365    00476D53     66:8908                  MOV WORD PTR DS:[EAX],CX
	$+368    00476D56     C2 1000                  RETN 0x10
	$+36B    00476D59     CC                       INT3
	$+36C    00476D5A     CC                       INT3
	$+36D    00476D5B     CC                       INT3
	$+36E    00476D5C     CC                       INT3
	$+36F    00476D5D     CC                       INT3
	$+370    00476D5E     CC                       INT3
	$+371    00476D5F     CC                       INT3
	$+372    00476D60     56                       PUSH ESI
	$+373    00476D61     8BF1                     MOV ESI,ECX
	$+375    00476D63     80BE AD080000 01         CMP BYTE PTR DS:[ESI+0x8AD],0x1
	$+37C    00476D6A     74 1B                    JE SHORT PathOfEx.00476D87
	$+37E    00476D6C     80BE AE080000 01         CMP BYTE PTR DS:[ESI+0x8AE],0x1
	$+385    00476D73     74 12                    JE SHORT PathOfEx.00476D87
	$+387    00476D75     80BE AF080000 01         CMP BYTE PTR DS:[ESI+0x8AF],0x1
	$+38E    00476D7C     74 09                    JE SHORT PathOfEx.00476D87
	$+390    00476D7E     80BE B0080000 01         CMP BYTE PTR DS:[ESI+0x8B0],0x1
	$+397    00476D85     75 0F                    JNZ SHORT PathOfEx.00476D96
	$+399    00476D87     8B86 3C080000            MOV EAX,DWORD PTR DS:[ESI+0x83C]
	$+39F    00476D8D     8D8E 3C080000            LEA ECX,DWORD PTR DS:[ESI+0x83C]
	$+3A5    00476D93     FF50 10                  CALL DWORD PTR DS:[EAX+0x10]
	$+3A8    00476D96     80BE F8080000 00         CMP BYTE PTR DS:[ESI+0x8F8],0x0
	$+3AF    00476D9D     74 2D                    JE SHORT PathOfEx.00476DCC
	$+3B1    00476D9F     F686 C4080000 10         TEST BYTE PTR DS:[ESI+0x8C4],0x10
	$+3B8    00476DA6     75 0B                    JNZ SHORT PathOfEx.00476DB3
	$+3BA    00476DA8     8BCE                     MOV ECX,ESI
	$+3BC    00476DAA     E8 C1684B00              CALL PathOfEx.0092D670
	$+3C1    00476DAF     84C0                     TEST AL,AL
	$+3C3    00476DB1     74 19                    JE SHORT PathOfEx.00476DCC
	$+3C5    00476DB3     8BCE                     MOV ECX,ESI
	$+3C7    00476DB5     C686 F8080000 00         MOV BYTE PTR DS:[ESI+0x8F8],0x0
	$+3CE    00476DBC     E8 0FFCFFFF              CALL PathOfEx.004769D0
	$+3D3    00476DC1     8B16                     MOV EDX,DWORD PTR DS:[ESI]
	$+3D5    00476DC3     8BCE                     MOV ECX,ESI
	$+3D7    00476DC5     50                       PUSH EAX
	$+3D8    00476DC6     FF92 5C010000            CALL DWORD PTR DS:[EDX+0x15C]
	$+3DE    00476DCC     5E                       POP ESI
	$+3DF    00476DCD     C2 0800                  RETN 0x8
	$+3E2    00476DD0     56                       PUSH ESI
	$+3E3    00476DD1     FF7424 08                PUSH DWORD PTR SS:[ESP+0x8]
	$+3E7    00476DD5     8BF1                     MOV ESI,ECX
	$+3E9    00476DD7     E8 44814B00              CALL PathOfEx.0092EF20
	$+3EE    00476DDC     F686 C4080000 04         TEST BYTE PTR DS:[ESI+0x8C4],0x4
	$+3F5    00476DE3     74 2A                    JE SHORT PathOfEx.00476E0F
	$+3F7    00476DE5     80BE F8080000 00         CMP BYTE PTR DS:[ESI+0x8F8],0x0
	$+3FE    00476DEC     75 21                    JNZ SHORT PathOfEx.00476E0F
	$+400    00476DEE     8D86 DC080000            LEA EAX,DWORD PTR DS:[ESI+0x8DC]
	$+406    00476DF4     C686 F8080000 01         MOV BYTE PTR DS:[ESI+0x8F8],0x1
	$+40D    00476DFB     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
	$+411    00476DFF     C740 10 00000000         MOV DWORD PTR DS:[EAX+0x10],0x0
	$+418    00476E06     72 02                    JB SHORT PathOfEx.00476E0A
	$+41A    00476E08     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+41C    00476E0A     33C9                     XOR ECX,ECX
	$+41E    00476E0C     66:8908                  MOV WORD PTR DS:[EAX],CX
	$+421    00476E0F     5E                       POP ESI
	$+422    00476E10     C2 0400                  RETN 0x4
	$+425    00476E13     CC                       INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("83????????????74??8b??????????8b??0f??????e8????????8b??????????b9", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("83????????????74??8b??????????8b??0f??????e8????????8b??????????b9", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	addr_tmp1 = (pt_byte*)addr_tmp - 0x1e;
	if (AsByte(addr_tmp1, 0) != 0x83)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_ui_create_item_tips_ = " << ExeAddr2Offset(addr_tmp1) << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword(addr_tmp, 2);
	assert(dw_tmp > 0x800 && dw_tmp < 0x950);
	ofs << "pt_cdword	g_o_items_tips2item_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

#ifdef VER_GJ3_
	dw_tmp = AsDword(addr_tmp, 0x20 + 1);
#else
	dw_tmp = AsDword(addr_tmp, 0x20 + 1);
#endif
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_item_tips_params_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

#ifdef VER_GF_
	ofs << "\n////////////////////////////////////\n//PathOfExileBase\n";
	if (!Scan_PathOfExileBase(ofs))
	{
		assert(false);
		return false;
	}
#endif
	return true;
}

bool CAutoUpdateApp::Scan_PathOfExileBase(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;

	/*
	$-35A    0FD2F359     C3                          RETN
	$-359    0FD2F35A     8BFF                        MOV EDI,EDI
	$-357    0FD2F35C     55                          PUSH EBP
	$-356    0FD2F35D     8BEC                        MOV EBP,ESP
	$-354    0FD2F35F     56                          PUSH ESI
	$-353    0FD2F360     57                          PUSH EDI
	$-352    0FD2F361     33F6                        XOR ESI,ESI
	$-350    0FD2F363     FF75 10                     PUSH DWORD PTR SS:[EBP+0x10]
	$-34D    0FD2F366     FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
	$-34A    0FD2F369     FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
	$-347    0FD2F36C     E8 5BFA0000                 CALL PathOf_1.0FD3EDCC
	$-342    0FD2F371     8BF8                        MOV EDI,EAX
	$-340    0FD2F373     83C4 0C                     ADD ESP,0xC
	$-33D    0FD2F376     85FF                        TEST EDI,EDI
	$-33B    0FD2F378     75 2C                       JNZ SHORT PathOf_1.0FD2F3A6
	$-339    0FD2F37A     3945 10                     CMP DWORD PTR SS:[EBP+0x10],EAX
	$-336    0FD2F37D     74 27                       JE SHORT PathOf_1.0FD2F3A6
	$-334    0FD2F37F     3905 14AEE70F               CMP DWORD PTR DS:[0xFE7AE14],EAX
	$-32E    0FD2F385     76 1F                       JBE SHORT PathOf_1.0FD2F3A6
	$-32C    0FD2F387     56                          PUSH ESI
	$-32B    0FD2F388     FF15 C0321210               CALL DWORD PTR DS:[0x101232C0]
	$-325    0FD2F38E     8D86 E8030000               LEA EAX,DWORD PTR DS:[ESI+0x3E8]
	$-31F    0FD2F394     3B05 14AEE70F               CMP EAX,DWORD PTR DS:[0xFE7AE14]
	$-319    0FD2F39A     76 03                       JBE SHORT PathOf_1.0FD2F39F
	$-317    0FD2F39C     83C8 FF                     OR EAX,0xFFFFFFFF
	$-314    0FD2F39F     8BF0                        MOV ESI,EAX
	$-312    0FD2F3A1     83F8 FF                     CMP EAX,-0x1
	$-30F    0FD2F3A4   ^ 75 BD                       JNZ SHORT PathOf_1.0FD2F363
	$-30D    0FD2F3A6     8BC7                        MOV EAX,EDI
	$-30B    0FD2F3A8     5F                          POP EDI
	$-30A    0FD2F3A9     5E                          POP ESI
	$-309    0FD2F3AA     5D                          POP EBP
	$-308    0FD2F3AB     C3                          RETN
	$-307    0FD2F3AC     6A 10                       PUSH 0x10
	$-305    0FD2F3AE     68 50E3E20F                 PUSH PathOf_1.0FE2E350
	$-300    0FD2F3B3     E8 98000000                 CALL PathOf_1.0FD2F450
	$-2FB    0FD2F3B8     33C0                        XOR EAX,EAX
	$-2F9    0FD2F3BA     8B5D 08                     MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-2F6    0FD2F3BD     33FF                        XOR EDI,EDI
	$-2F4    0FD2F3BF     3BDF                        CMP EBX,EDI
	$-2F2    0FD2F3C1     0F95C0                      SETNE AL
	$-2EF    0FD2F3C4     3BC7                        CMP EAX,EDI
	$-2ED    0FD2F3C6     75 1D                       JNZ SHORT PathOf_1.0FD2F3E5
	$-2EB    0FD2F3C8     E8 6B8CFFFF                 CALL PathOf_1.0FD28038
	$-2E6    0FD2F3CD     C700 16000000               MOV DWORD PTR DS:[EAX],0x16
	$-2E0    0FD2F3D3     57                          PUSH EDI
	$-2DF    0FD2F3D4     57                          PUSH EDI
	$-2DE    0FD2F3D5     57                          PUSH EDI
	$-2DD    0FD2F3D6     57                          PUSH EDI
	$-2DC    0FD2F3D7     57                          PUSH EDI
	$-2DB    0FD2F3D8     E8 BC31FFFF                 CALL PathOf_1.0FD22599
	$-2D6    0FD2F3DD     83C4 14                     ADD ESP,0x14
	$-2D3    0FD2F3E0     83C8 FF                     OR EAX,0xFFFFFFFF
	$-2D0    0FD2F3E3     EB 53                       JMP SHORT PathOf_1.0FD2F438
	$-2CE    0FD2F3E5     833D FCCAEE0F 03            CMP DWORD PTR DS:[0xFEECAFC],0x3
	$-2C7    0FD2F3EC     75 38                       JNZ SHORT PathOf_1.0FD2F426
	$-2C5    0FD2F3EE     6A 04                       PUSH 0x4
	$-2C3    0FD2F3F0     E8 97170000                 CALL PathOf_1.0FD30B8C
	$-2BE    0FD2F3F5     59                          POP ECX
	$-2BD    0FD2F3F6     897D FC                     MOV DWORD PTR SS:[EBP-0x4],EDI
	$-2BA    0FD2F3F9     53                          PUSH EBX
	$-2B9    0FD2F3FA     E8 C0170000                 CALL PathOf_1.0FD30BBF
	$-2B4    0FD2F3FF     59                          POP ECX
	$-2B3    0FD2F400     8945 E0                     MOV DWORD PTR SS:[EBP-0x20],EAX
	$-2B0    0FD2F403     3BC7                        CMP EAX,EDI
	$-2AE    0FD2F405     74 0B                       JE SHORT PathOf_1.0FD2F412
	$-2AC    0FD2F407     8B73 FC                     MOV ESI,DWORD PTR DS:[EBX-0x4]
	$-2A9    0FD2F40A     83EE 09                     SUB ESI,0x9
	$-2A6    0FD2F40D     8975 E4                     MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-2A3    0FD2F410     EB 03                       JMP SHORT PathOf_1.0FD2F415
	$-2A1    0FD2F412     8B75 E4                     MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-29E    0FD2F415     C745 FC FEFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x2
	$-297    0FD2F41C     E8 25000000                 CALL PathOf_1.0FD2F446
	$-292    0FD2F421     397D E0                     CMP DWORD PTR SS:[EBP-0x20],EDI
	$-28F    0FD2F424     75 10                       JNZ SHORT PathOf_1.0FD2F436
	$-28D    0FD2F426     53                          PUSH EBX
	$-28C    0FD2F427     57                          PUSH EDI
	$-28B    0FD2F428     FF35 1CAEE70F               PUSH DWORD PTR DS:[0xFE7AE1C]
	$-285    0FD2F42E     FF15 00321210               CALL DWORD PTR DS:[0x10123200]                 ; ntdll_1a.RtlSizeHeap
	$-27F    0FD2F434     8BF0                        MOV ESI,EAX
	$-27D    0FD2F436     8BC6                        MOV EAX,ESI
	$-27B    0FD2F438     E8 58000000                 CALL PathOf_1.0FD2F495
	$-276    0FD2F43D     C3                          RETN
	$-275    0FD2F43E     33FF                        XOR EDI,EDI
	$-273    0FD2F440     8B5D 08                     MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-270    0FD2F443     8B75 E4                     MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-26D    0FD2F446     6A 04                       PUSH 0x4
	$-26B    0FD2F448     E8 65160000                 CALL PathOf_1.0FD30AB2
	$-266    0FD2F44D     59                          POP ECX
	$-265    0FD2F44E     C3                          RETN
	$-264    0FD2F44F     CC                          INT3
	$-263    0FD2F450     68 B0F4D20F                 PUSH PathOf_1.0FD2F4B0
	$-25E    0FD2F455     64:FF35 00000000            PUSH DWORD PTR FS:[0]
	$-257    0FD2F45C     8B4424 10                   MOV EAX,DWORD PTR SS:[ESP+0x10]
	$-253    0FD2F460     896C24 10                   MOV DWORD PTR SS:[ESP+0x10],EBP
	$-24F    0FD2F464     8D6C24 10                   LEA EBP,DWORD PTR SS:[ESP+0x10]
	$-24B    0FD2F468     2BE0                        SUB ESP,EAX
	$-249    0FD2F46A     53                          PUSH EBX
	$-248    0FD2F46B     56                          PUSH ESI
	$-247    0FD2F46C     57                          PUSH EDI
	$-246    0FD2F46D     A1 B0B0E40F                 MOV EAX,DWORD PTR DS:[0xFE4B0B0]
	$-241    0FD2F472     3145 FC                     XOR DWORD PTR SS:[EBP-0x4],EAX
	$-23E    0FD2F475     33C5                        XOR EAX,EBP
	$-23C    0FD2F477     50                          PUSH EAX
	$-23B    0FD2F478     8965 E8                     MOV DWORD PTR SS:[EBP-0x18],ESP
	$-238    0FD2F47B     FF75 F8                     PUSH DWORD PTR SS:[EBP-0x8]
	$-235    0FD2F47E     8B45 FC                     MOV EAX,DWORD PTR SS:[EBP-0x4]
	$-232    0FD2F481     C745 FC FEFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x2
	$-22B    0FD2F488     8945 F8                     MOV DWORD PTR SS:[EBP-0x8],EAX
	$-228    0FD2F48B     8D45 F0                     LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-225    0FD2F48E     64:A3 00000000              MOV DWORD PTR FS:[0],EAX
	$-21F    0FD2F494     C3                          RETN
	$-21E    0FD2F495     8B4D F0                     MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-21B    0FD2F498     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$-214    0FD2F49F     59                          POP ECX
	$-213    0FD2F4A0     5F                          POP EDI
	$-212    0FD2F4A1     5F                          POP EDI
	$-211    0FD2F4A2     5E                          POP ESI
	$-210    0FD2F4A3     5B                          POP EBX
	$-20F    0FD2F4A4     8BE5                        MOV ESP,EBP
	$-20D    0FD2F4A6     5D                          POP EBP
	$-20C    0FD2F4A7     51                          PUSH ECX
	$-20B    0FD2F4A8     C3                          RETN
	$-20A    0FD2F4A9     CC                          INT3
	$-209    0FD2F4AA     CC                          INT3
	$-208    0FD2F4AB     CC                          INT3
	$-207    0FD2F4AC     CC                          INT3
	$-206    0FD2F4AD     CC                          INT3
	$-205    0FD2F4AE     CC                          INT3
	$-204    0FD2F4AF     CC                          INT3
	$-203    0FD2F4B0     8BFF                        MOV EDI,EDI
	$-201    0FD2F4B2     55                          PUSH EBP
	$-200    0FD2F4B3     8BEC                        MOV EBP,ESP
	$-1FE    0FD2F4B5     83EC 18                     SUB ESP,0x18
	$-1FB    0FD2F4B8     53                          PUSH EBX
	$-1FA    0FD2F4B9     8B5D 0C                     MOV EBX,DWORD PTR SS:[EBP+0xC]
	$-1F7    0FD2F4BC     56                          PUSH ESI
	$-1F6    0FD2F4BD     8B73 08                     MOV ESI,DWORD PTR DS:[EBX+0x8]
	$-1F3    0FD2F4C0     3335 B0B0E40F               XOR ESI,DWORD PTR DS:[0xFE4B0B0]
	$-1ED    0FD2F4C6     57                          PUSH EDI
	$-1EC    0FD2F4C7     8B06                        MOV EAX,DWORD PTR DS:[ESI]
	$-1EA    0FD2F4C9     C645 FF 00                  MOV BYTE PTR SS:[EBP-0x1],0x0
	$-1E6    0FD2F4CD     C745 F4 01000000            MOV DWORD PTR SS:[EBP-0xC],0x1
	$-1DF    0FD2F4D4     8D7B 10                     LEA EDI,DWORD PTR DS:[EBX+0x10]
	$-1DC    0FD2F4D7     83F8 FE                     CMP EAX,-0x2
	$-1D9    0FD2F4DA     74 0D                       JE SHORT PathOf_1.0FD2F4E9
	$-1D7    0FD2F4DC     8B4E 04                     MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-1D4    0FD2F4DF     03CF                        ADD ECX,EDI
	$-1D2    0FD2F4E1     330C38                      XOR ECX,DWORD PTR DS:[EAX+EDI]
	$-1CF    0FD2F4E4     E8 017DFFFF                 CALL PathOf_1.0FD271EA
	$-1CA    0FD2F4E9     8B4E 0C                     MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-1C7    0FD2F4EC     8B46 08                     MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-1C4    0FD2F4EF     03CF                        ADD ECX,EDI
	$-1C2    0FD2F4F1     330C38                      XOR ECX,DWORD PTR DS:[EAX+EDI]
	$-1BF    0FD2F4F4     E8 F17CFFFF                 CALL PathOf_1.0FD271EA
	$-1BA    0FD2F4F9     8B45 08                     MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-1B7    0FD2F4FC     F640 04 66                  TEST BYTE PTR DS:[EAX+0x4],0x66
	$-1B3    0FD2F500     0F85 16010000               JNZ PathOf_1.0FD2F61C
	$-1AD    0FD2F506     8B4D 10                     MOV ECX,DWORD PTR SS:[EBP+0x10]
	$-1AA    0FD2F509     8D55 E8                     LEA EDX,DWORD PTR SS:[EBP-0x18]
	$-1A7    0FD2F50C     8953 FC                     MOV DWORD PTR DS:[EBX-0x4],EDX
	$-1A4    0FD2F50F     8B5B 0C                     MOV EBX,DWORD PTR DS:[EBX+0xC]
	$-1A1    0FD2F512     8945 E8                     MOV DWORD PTR SS:[EBP-0x18],EAX
	$-19E    0FD2F515     894D EC                     MOV DWORD PTR SS:[EBP-0x14],ECX
	$-19B    0FD2F518     83FB FE                     CMP EBX,-0x2
	$-198    0FD2F51B     74 5F                       JE SHORT PathOf_1.0FD2F57C
	$-196    0FD2F51D     8D49 00                     LEA ECX,DWORD PTR DS:[ECX]
	$-193    0FD2F520     8D045B                      LEA EAX,DWORD PTR DS:[EBX+EBX*2]
	$-190    0FD2F523     8B4C86 14                   MOV ECX,DWORD PTR DS:[ESI+EAX*4+0x14]
	$-18C    0FD2F527     8D4486 10                   LEA EAX,DWORD PTR DS:[ESI+EAX*4+0x10]
	$-188    0FD2F52B     8945 F0                     MOV DWORD PTR SS:[EBP-0x10],EAX
	$-185    0FD2F52E     8B00                        MOV EAX,DWORD PTR DS:[EAX]
	$-183    0FD2F530     8945 F8                     MOV DWORD PTR SS:[EBP-0x8],EAX
	$-180    0FD2F533     85C9                        TEST ECX,ECX
	$-17E    0FD2F535     74 14                       JE SHORT PathOf_1.0FD2F54B
	$-17C    0FD2F537     8BD7                        MOV EDX,EDI
	$-17A    0FD2F539     E8 382D0000                 CALL PathOf_1.0FD32276
	$-175    0FD2F53E     C645 FF 01                  MOV BYTE PTR SS:[EBP-0x1],0x1
	$-171    0FD2F542     85C0                        TEST EAX,EAX
	$-16F    0FD2F544     7C 40                       JL SHORT PathOf_1.0FD2F586
	$-16D    0FD2F546     7F 47                       JG SHORT PathOf_1.0FD2F58F
	$-16B    0FD2F548     8B45 F8                     MOV EAX,DWORD PTR SS:[EBP-0x8]
	$-168    0FD2F54B     8BD8                        MOV EBX,EAX
	$-166    0FD2F54D     83F8 FE                     CMP EAX,-0x2
	$-163    0FD2F550   ^ 75 CE                       JNZ SHORT PathOf_1.0FD2F520
	$-161    0FD2F552     807D FF 00                  CMP BYTE PTR SS:[EBP-0x1],0x0
	$-15D    0FD2F556     74 24                       JE SHORT PathOf_1.0FD2F57C
	$-15B    0FD2F558     8B06                        MOV EAX,DWORD PTR DS:[ESI]
	$-159    0FD2F55A     83F8 FE                     CMP EAX,-0x2
	$-156    0FD2F55D     74 0D                       JE SHORT PathOf_1.0FD2F56C
	$-154    0FD2F55F     8B4E 04                     MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-151    0FD2F562     03CF                        ADD ECX,EDI
	$-14F    0FD2F564     330C38                      XOR ECX,DWORD PTR DS:[EAX+EDI]
	$-14C    0FD2F567     E8 7E7CFFFF                 CALL PathOf_1.0FD271EA
	$-147    0FD2F56C     8B4E 0C                     MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-144    0FD2F56F     8B56 08                     MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-141    0FD2F572     03CF                        ADD ECX,EDI
	$-13F    0FD2F574     330C3A                      XOR ECX,DWORD PTR DS:[EDX+EDI]
	$-13C    0FD2F577     E8 6E7CFFFF                 CALL PathOf_1.0FD271EA
	$-137    0FD2F57C     8B45 F4                     MOV EAX,DWORD PTR SS:[EBP-0xC]
	$-134    0FD2F57F     5F                          POP EDI
	$-133    0FD2F580     5E                          POP ESI
	$-132    0FD2F581     5B                          POP EBX
	$-131    0FD2F582     8BE5                        MOV ESP,EBP
	$-12F    0FD2F584     5D                          POP EBP
	$-12E    0FD2F585     C3                          RETN
	$-12D    0FD2F586     C745 F4 00000000            MOV DWORD PTR SS:[EBP-0xC],0x0
	$-126    0FD2F58D   ^ EB C9                       JMP SHORT PathOf_1.0FD2F558
	$-124    0FD2F58F     8B4D 08                     MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-121    0FD2F592     8139 63736DE0               CMP DWORD PTR DS:[ECX],0xE06D7363
	$-11B    0FD2F598     75 29                       JNZ SHORT PathOf_1.0FD2F5C3
	$-119    0FD2F59A     833D 602AE10F 00            CMP DWORD PTR DS:[0xFE12A60],0x0
	$-112    0FD2F5A1     74 20                       JE SHORT PathOf_1.0FD2F5C3
	$-110    0FD2F5A3     68 602AE10F                 PUSH PathOf_1.0FE12A60
	$-10B    0FD2F5A8     E8 83F00000                 CALL PathOf_1.0FD3E630
	$-106    0FD2F5AD     83C4 04                     ADD ESP,0x4
	$-103    0FD2F5B0     85C0                        TEST EAX,EAX
	$-101    0FD2F5B2     74 0F                       JE SHORT PathOf_1.0FD2F5C3
	$-FF     0FD2F5B4     8B55 08                     MOV EDX,DWORD PTR SS:[EBP+0x8]
	$-FC     0FD2F5B7     6A 01                       PUSH 0x1
	$-FA     0FD2F5B9     52                          PUSH EDX
	$-F9     0FD2F5BA     FF15 602AE10F               CALL DWORD PTR DS:[0xFE12A60]                  ; PathOf_1.0FD35F11
	$-F3     0FD2F5C0     83C4 08                     ADD ESP,0x8
	$-F0     0FD2F5C3     8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-ED     0FD2F5C6     E8 DB2C0000                 CALL PathOf_1.0FD322A6
	$-E8     0FD2F5CB     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-E5     0FD2F5CE     3958 0C                     CMP DWORD PTR DS:[EAX+0xC],EBX
	$-E2     0FD2F5D1     74 12                       JE SHORT PathOf_1.0FD2F5E5
	$-E0     0FD2F5D3     68 B0B0E40F                 PUSH PathOf_1.0FE4B0B0
	$-DB     0FD2F5D8     57                          PUSH EDI
	$-DA     0FD2F5D9     8BD3                        MOV EDX,EBX
	$-D8     0FD2F5DB     8BC8                        MOV ECX,EAX
	$-D6     0FD2F5DD     E8 DE2C0000                 CALL PathOf_1.0FD322C0
	$-D1     0FD2F5E2     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-CE     0FD2F5E5     8B4D F8                     MOV ECX,DWORD PTR SS:[EBP-0x8]
	$-CB     0FD2F5E8     8948 0C                     MOV DWORD PTR DS:[EAX+0xC],ECX
	$-C8     0FD2F5EB     8B06                        MOV EAX,DWORD PTR DS:[ESI]
	$-C6     0FD2F5ED     83F8 FE                     CMP EAX,-0x2
	$-C3     0FD2F5F0     74 0D                       JE SHORT PathOf_1.0FD2F5FF
	$-C1     0FD2F5F2     8B4E 04                     MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-BE     0FD2F5F5     03CF                        ADD ECX,EDI
	$-BC     0FD2F5F7     330C38                      XOR ECX,DWORD PTR DS:[EAX+EDI]
	$-B9     0FD2F5FA     E8 EB7BFFFF                 CALL PathOf_1.0FD271EA
	$-B4     0FD2F5FF     8B4E 0C                     MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-B1     0FD2F602     8B56 08                     MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-AE     0FD2F605     03CF                        ADD ECX,EDI
	$-AC     0FD2F607     330C3A                      XOR ECX,DWORD PTR DS:[EDX+EDI]
	$-A9     0FD2F60A     E8 DB7BFFFF                 CALL PathOf_1.0FD271EA
	$-A4     0FD2F60F     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-A1     0FD2F612     8B48 08                     MOV ECX,DWORD PTR DS:[EAX+0x8]
	$-9E     0FD2F615     8BD7                        MOV EDX,EDI
	$-9C     0FD2F617     E8 712C0000                 CALL PathOf_1.0FD3228D
	$-97     0FD2F61C     BA FEFFFFFF                 MOV EDX,-0x2
	$-92     0FD2F621     3953 0C                     CMP DWORD PTR DS:[EBX+0xC],EDX
	$-8F     0FD2F624   ^ 0F84 52FFFFFF               JE PathOf_1.0FD2F57C
	$-89     0FD2F62A     68 B0B0E40F                 PUSH PathOf_1.0FE4B0B0
	$-84     0FD2F62F     57                          PUSH EDI
	$-83     0FD2F630     8BCB                        MOV ECX,EBX
	$-81     0FD2F632     E8 892C0000                 CALL PathOf_1.0FD322C0
	$-7C     0FD2F637   ^ E9 1CFFFFFF                 JMP PathOf_1.0FD2F558
	$-77     0FD2F63C     F641 0C 40                  TEST BYTE PTR DS:[ECX+0xC],0x40
	$-73     0FD2F640     74 06                       JE SHORT PathOf_1.0FD2F648
	$-71     0FD2F642     8379 08 00                  CMP DWORD PTR DS:[ECX+0x8],0x0
	$-6D     0FD2F646     74 24                       JE SHORT PathOf_1.0FD2F66C
	$-6B     0FD2F648     FF49 04                     DEC DWORD PTR DS:[ECX+0x4]
	$-68     0FD2F64B     78 0B                       JS SHORT PathOf_1.0FD2F658
	$-66     0FD2F64D     8B11                        MOV EDX,DWORD PTR DS:[ECX]
	$-64     0FD2F64F     8802                        MOV BYTE PTR DS:[EDX],AL
	$-62     0FD2F651     FF01                        INC DWORD PTR DS:[ECX]
	$-60     0FD2F653     0FB6C0                      MOVZX EAX,AL
	$-5D     0FD2F656     EB 0C                       JMP SHORT PathOf_1.0FD2F664
	$-5B     0FD2F658     0FBEC0                      MOVSX EAX,AL
	$-58     0FD2F65B     51                          PUSH ECX
	$-57     0FD2F65C     50                          PUSH EAX
	$-56     0FD2F65D     E8 11E1FFFF                 CALL PathOf_1.0FD2D773
	$-51     0FD2F662     59                          POP ECX
	$-50     0FD2F663     59                          POP ECX
	$-4F     0FD2F664     83F8 FF                     CMP EAX,-0x1
	$-4C     0FD2F667     75 03                       JNZ SHORT PathOf_1.0FD2F66C
	$-4A     0FD2F669     0906                        OR DWORD PTR DS:[ESI],EAX
	$-48     0FD2F66B     C3                          RETN
	$-47     0FD2F66C     FF06                        INC DWORD PTR DS:[ESI]
	$-45     0FD2F66E     C3                          RETN
	$-44     0FD2F66F     8BFF                        MOV EDI,EDI                                    ; 怪物猎人hook的是此处
	$-42     0FD2F671     55                          PUSH EBP
	$-41     0FD2F672     8BEC                        MOV EBP,ESP
	$-3F     0FD2F674     56                          PUSH ESI
	$-3E     0FD2F675     8BF0                        MOV ESI,EAX
	$-3C     0FD2F677     EB 13                       JMP SHORT PathOf_1.0FD2F68C
	$-3A     0FD2F679     8B4D 10                     MOV ECX,DWORD PTR SS:[EBP+0x10]
	$-37     0FD2F67C     8A45 08                     MOV AL,BYTE PTR SS:[EBP+0x8]
	$-34     0FD2F67F     FF4D 0C                     DEC DWORD PTR SS:[EBP+0xC]
	$-31     0FD2F682     E8 B5FFFFFF                 CALL PathOf_1.0FD2F63C
	$-2C     0FD2F687     833E FF                     CMP DWORD PTR DS:[ESI],-0x1
	$-29     0FD2F68A     74 06                       JE SHORT PathOf_1.0FD2F692
	$-27     0FD2F68C     837D 0C 00                  CMP DWORD PTR SS:[EBP+0xC],0x0
	$-23     0FD2F690   ^ 7F E7                       JG SHORT PathOf_1.0FD2F679
	$-21     0FD2F692     5E                          POP ESI
	$-20     0FD2F693     5D                          POP EBP
	$-1F     0FD2F694     C3                          RETN
	$-1E     0FD2F695     8BFF                        MOV EDI,EDI                                    ; g_h_base_sprintf1_
	$-1C     0FD2F697     55                          PUSH EBP
	$-1B     0FD2F698     8BEC                        MOV EBP,ESP
	$-19     0FD2F69A     F647 0C 40                  TEST BYTE PTR DS:[EDI+0xC],0x40
	$-15     0FD2F69E     53                          PUSH EBX
	$-14     0FD2F69F     56                          PUSH ESI
	$-13     0FD2F6A0     8BF0                        MOV ESI,EAX
	$-11     0FD2F6A2     8BD9                        MOV EBX,ECX
	$-F      0FD2F6A4     74 32                       JE SHORT PathOf_1.0FD2F6D8
	$-D      0FD2F6A6     837F 08 00                  CMP DWORD PTR DS:[EDI+0x8],0x0
	$-9      0FD2F6AA     75 2C                       JNZ SHORT PathOf_1.0FD2F6D8
	$-7      0FD2F6AC     8B45 08                     MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-4      0FD2F6AF     0106                        ADD DWORD PTR DS:[ESI],EAX
	$-2      0FD2F6B1     EB 2B                       JMP SHORT PathOf_1.0FD2F6DE
	$ ==>    0FD2F6B3     8A03                        MOV AL,BYTE PTR DS:[EBX]
	$+2      0FD2F6B5     FF4D 08                     DEC DWORD PTR SS:[EBP+0x8]
	$+5      0FD2F6B8     8BCF                        MOV ECX,EDI
	$+7      0FD2F6BA     E8 7DFFFFFF                 CALL PathOf_1.0FD2F63C
	$+C      0FD2F6BF     43                          INC EBX
	$+D      0FD2F6C0     833E FF                     CMP DWORD PTR DS:[ESI],-0x1
	$+10     0FD2F6C3     75 13                       JNZ SHORT PathOf_1.0FD2F6D8
	$+12     0FD2F6C5     E8 6E89FFFF                 CALL PathOf_1.0FD28038
	$+17     0FD2F6CA     8338 2A                     CMP DWORD PTR DS:[EAX],0x2A
	$+1A     0FD2F6CD     75 0F                       JNZ SHORT PathOf_1.0FD2F6DE
	$+1C     0FD2F6CF     8BCF                        MOV ECX,EDI
	$+1E     0FD2F6D1     B0 3F                       MOV AL,0x3F
	$+20     0FD2F6D3     E8 64FFFFFF                 CALL PathOf_1.0FD2F63C
	$+25     0FD2F6D8     837D 08 00                  CMP DWORD PTR SS:[EBP+0x8],0x0
	$+29     0FD2F6DC   ^ 7F D5                       JG SHORT PathOf_1.0FD2F6B3
	$+2B     0FD2F6DE     5E                          POP ESI
	$+2C     0FD2F6DF     5B                          POP EBX
	$+2D     0FD2F6E0     5D                          POP EBP
	$+2E     0FD2F6E1     C3                          RETN
	$+2F     0FD2F6E2     8BFF                        MOV EDI,EDI
	$+31     0FD2F6E4     55                          PUSH EBP
	$+32     0FD2F6E5     8BEC                        MOV EBP,ESP
	$+34     0FD2F6E7     81EC 78020000               SUB ESP,0x278
	$+3A     0FD2F6ED     A1 B0B0E40F                 MOV EAX,DWORD PTR DS:[0xFE4B0B0]
	$+3F     0FD2F6F2     33C5                        XOR EAX,EBP
	$+41     0FD2F6F4     8945 FC                     MOV DWORD PTR SS:[EBP-0x4],EAX
	$+44     0FD2F6F7     53                          PUSH EBX
	$+45     0FD2F6F8     8B5D 0C                     MOV EBX,DWORD PTR SS:[EBP+0xC]
	$+48     0FD2F6FB     56                          PUSH ESI
	$+49     0FD2F6FC     8B75 08                     MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+4C     0FD2F6FF     33C0                        XOR EAX,EAX
	$+4E     0FD2F701     57                          PUSH EDI
	$+4F     0FD2F702     8B7D 14                     MOV EDI,DWORD PTR SS:[EBP+0x14]
	$+52     0FD2F705     FF75 10                     PUSH DWORD PTR SS:[EBP+0x10]
	$+55     0FD2F708     8D8D B0FDFFFF               LEA ECX,DWORD PTR SS:[EBP-0x250]
	$+5B     0FD2F70E     89B5 A0FDFFFF               MOV DWORD PTR SS:[EBP-0x260],ESI
	$+61     0FD2F714     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+67     0FD2F71A     8985 A4FDFFFF               MOV DWORD PTR SS:[EBP-0x25C],EAX
	$+6D     0FD2F720     8985 F0FDFFFF               MOV DWORD PTR SS:[EBP-0x210],EAX
	$+73     0FD2F726     8985 CCFDFFFF               MOV DWORD PTR SS:[EBP-0x234],EAX
	$+79     0FD2F72C     8985 E8FDFFFF               MOV DWORD PTR SS:[EBP-0x218],EAX
	$+7F     0FD2F732     8985 D0FDFFFF               MOV DWORD PTR SS:[EBP-0x230],EAX
	$+85     0FD2F738     8985 A8FDFFFF               MOV DWORD PTR SS:[EBP-0x258],EAX
	$+8B     0FD2F73E     8985 C8FDFFFF               MOV DWORD PTR SS:[EBP-0x238],EAX
	$+91     0FD2F744     E8 7327FFFF                 CALL PathOf_1.0FD21EBC
	$+96     0FD2F749     85F6                        TEST ESI,ESI
	$+98     0FD2F74B     75 35                       JNZ SHORT PathOf_1.0FD2F782
	$+9A     0FD2F74D     E8 E688FFFF                 CALL PathOf_1.0FD28038
	$+9F     0FD2F752     C700 16000000               MOV DWORD PTR DS:[EAX],0x16
	$+A5     0FD2F758     33C0                        XOR EAX,EAX
	$+A7     0FD2F75A     50                          PUSH EAX
	$+A8     0FD2F75B     50                          PUSH EAX
	$+A9     0FD2F75C     50                          PUSH EAX
	$+AA     0FD2F75D     50                          PUSH EAX
	$+AB     0FD2F75E     50                          PUSH EAX
	$+AC     0FD2F75F     E8 352EFFFF                 CALL PathOf_1.0FD22599
	$+B1     0FD2F764     83C4 14                     ADD ESP,0x14
	$+B4     0FD2F767     80BD BCFDFFFF 00            CMP BYTE PTR SS:[EBP-0x244],0x0
	$+BB     0FD2F76E     74 0A                       JE SHORT PathOf_1.0FD2F77A
	$+BD     0FD2F770     8B85 B8FDFFFF               MOV EAX,DWORD PTR SS:[EBP-0x248]
	$+C3     0FD2F776     8360 70 FD                  AND DWORD PTR DS:[EAX+0x70],0xFFFFFFFD
	$+C7     0FD2F77A     83C8 FF                     OR EAX,0xFFFFFFFF
	$+CA     0FD2F77D     E9 FF0A0000                 JMP PathOf_1.0FD30281
	$+CF     0FD2F782     F646 0C 40                  TEST BYTE PTR DS:[ESI+0xC],0x40
	$+D3     0FD2F786     75 5E                       JNZ SHORT PathOf_1.0FD2F7E6
	$+D5     0FD2F788     56                          PUSH ESI
	$+D6     0FD2F789     E8 33330000                 CALL PathOf_1.0FD32AC1
	$+DB     0FD2F78E     59                          POP ECX
	$+DC     0FD2F78F     BA 10C0E40F                 MOV EDX,PathOf_1.0FE4C010
	$+E1     0FD2F794     83F8 FF                     CMP EAX,-0x1
	$+E4     0FD2F797     74 1B                       JE SHORT PathOf_1.0FD2F7B4
	$+E6     0FD2F799     83F8 FE                     CMP EAX,-0x2
	$+E9     0FD2F79C     74 16                       JE SHORT PathOf_1.0FD2F7B4
	$+EB     0FD2F79E     8BC8                        MOV ECX,EAX
	$+ED     0FD2F7A0     83E1 1F                     AND ECX,0x1F
	$+F0     0FD2F7A3     8BF0                        MOV ESI,EAX
	$+F2     0FD2F7A5     C1FE 05                     SAR ESI,0x5
	$+F5     0FD2F7A8     C1E1 06                     SHL ECX,0x6
	$+F8     0FD2F7AB     030CB5 E0C9EE0F             ADD ECX,DWORD PTR DS:[ESI*4+0xFEEC9E0]
	$+FF     0FD2F7B2     EB 02                       JMP SHORT PathOf_1.0FD2F7B6
	$+101    0FD2F7B4     8BCA                        MOV ECX,EDX
	$+103    0FD2F7B6     F641 24 7F                  TEST BYTE PTR DS:[ECX+0x24],0x7F
	$+107    0FD2F7BA   ^ 75 91                       JNZ SHORT PathOf_1.0FD2F74D
	$+109    0FD2F7BC     83F8 FF                     CMP EAX,-0x1
	$+10C    0FD2F7BF     74 19                       JE SHORT PathOf_1.0FD2F7DA
	$+10E    0FD2F7C1     83F8 FE                     CMP EAX,-0x2
	$+111    0FD2F7C4     74 14                       JE SHORT PathOf_1.0FD2F7DA
	$+113    0FD2F7C6     8BC8                        MOV ECX,EAX
	$+115    0FD2F7C8     83E0 1F                     AND EAX,0x1F
	$+118    0FD2F7CB     C1F9 05                     SAR ECX,0x5
	$+11B    0FD2F7CE     C1E0 06                     SHL EAX,0x6
	$+11E    0FD2F7D1     03048D E0C9EE0F             ADD EAX,DWORD PTR DS:[ECX*4+0xFEEC9E0]
	$+125    0FD2F7D8     EB 02                       JMP SHORT PathOf_1.0FD2F7DC
	$+127    0FD2F7DA     8BC2                        MOV EAX,EDX
	$+129    0FD2F7DC     F640 24 80                  TEST BYTE PTR DS:[EAX+0x24],0x80
	$+12D    0FD2F7E0   ^ 0F85 67FFFFFF               JNZ PathOf_1.0FD2F74D
	$+133    0FD2F7E6     33C0                        XOR EAX,EAX
	$+135    0FD2F7E8     3BD8                        CMP EBX,EAX
	$+137    0FD2F7EA   ^ 0F84 5DFFFFFF               JE PathOf_1.0FD2F74D
	$+13D    0FD2F7F0     8A13                        MOV DL,BYTE PTR DS:[EBX]
	$+13F    0FD2F7F2     8985 D8FDFFFF               MOV DWORD PTR SS:[EBP-0x228],EAX
	$+145    0FD2F7F8     8985 E0FDFFFF               MOV DWORD PTR SS:[EBP-0x220],EAX
	$+14B    0FD2F7FE     8985 C0FDFFFF               MOV DWORD PTR SS:[EBP-0x240],EAX
	$+151    0FD2F804     8985 ACFDFFFF               MOV DWORD PTR SS:[EBP-0x254],EAX
	$+157    0FD2F80A     8895 EFFDFFFF               MOV BYTE PTR SS:[EBP-0x211],DL
	$+15D    0FD2F810     84D2                        TEST DL,DL
	$+15F    0FD2F812     0F84 500A0000               JE PathOf_1.0FD30268
	$+165    0FD2F818     43                          INC EBX
	$+166    0FD2F819     33C0                        XOR EAX,EAX
	$+168    0FD2F81B     3985 D8FDFFFF               CMP DWORD PTR SS:[EBP-0x228],EAX
	$+16E    0FD2F821     899D C4FDFFFF               MOV DWORD PTR SS:[EBP-0x23C],EBX
	$+174    0FD2F827     0F8C 130A0000               JL PathOf_1.0FD30240
	$+17A    0FD2F82D     8ACA                        MOV CL,DL
	$+17C    0FD2F82F     80E9 20                     SUB CL,0x20
	$+17F    0FD2F832     80F9 58                     CMP CL,0x58
	$+182    0FD2F835     77 0D                       JA SHORT PathOf_1.0FD2F844
	$+184    0FD2F837     0FBEC2                      MOVSX EAX,DL
	$+187    0FD2F83A     0FB680 E822E10F             MOVZX EAX,BYTE PTR DS:[EAX+0xFE122E8]
	$+18E    0FD2F841     83E0 0F                     AND EAX,0xF
	$+191    0FD2F844     8B8D C0FDFFFF               MOV ECX,DWORD PTR SS:[EBP-0x240]
	$+197    0FD2F84A     6BC0 09                     IMUL EAX,EAX,0x9
	$+19A    0FD2F84D     0FB68408 0823E10F           MOVZX EAX,BYTE PTR DS:[EAX+ECX+0xFE12308]
	$+1A2    0FD2F855     6A 08                       PUSH 0x8
	$+1A4    0FD2F857     C1E8 04                     SHR EAX,0x4
	$+1A7    0FD2F85A     5E                          POP ESI
	$+1A8    0FD2F85B     8985 C0FDFFFF               MOV DWORD PTR SS:[EBP-0x240],EAX
	$+1AE    0FD2F861     3BC6                        CMP EAX,ESI
	$+1B0    0FD2F863   ^ 0F84 E4FEFFFF               JE PathOf_1.0FD2F74D
	$+1B6    0FD2F869     6A 07                       PUSH 0x7
	$+1B8    0FD2F86B     59                          POP ECX
	$+1B9    0FD2F86C     3BC1                        CMP EAX,ECX
	$+1BB    0FD2F86E     0F87 AD090000               JA PathOf_1.0FD30221
	$+1C1    0FD2F874     FF2485 9202D30F             JMP DWORD PTR DS:[EAX*4+0xFD30292]
	$+1C8    0FD2F87B     33C0                        XOR EAX,EAX
	$+1CA    0FD2F87D     838D E8FDFFFF FF            OR DWORD PTR SS:[EBP-0x218],0xFFFFFFFF
	$+1D1    0FD2F884     8985 94FDFFFF               MOV DWORD PTR SS:[EBP-0x26C],EAX
	$+1D7    0FD2F88A     8985 A8FDFFFF               MOV DWORD PTR SS:[EBP-0x258],EAX
	$+1DD    0FD2F890     8985 CCFDFFFF               MOV DWORD PTR SS:[EBP-0x234],EAX
	$+1E3    0FD2F896     8985 D0FDFFFF               MOV DWORD PTR SS:[EBP-0x230],EAX
	$+1E9    0FD2F89C     8985 F0FDFFFF               MOV DWORD PTR SS:[EBP-0x210],EAX
	$+1EF    0FD2F8A2     8985 C8FDFFFF               MOV DWORD PTR SS:[EBP-0x238],EAX
	$+1F5    0FD2F8A8     E9 74090000                 JMP PathOf_1.0FD30221
	$+1FA    0FD2F8AD     0FBEC2                      MOVSX EAX,DL
	$+1FD    0FD2F8B0     83E8 20                     SUB EAX,0x20
	$+200    0FD2F8B3     74 48                       JE SHORT PathOf_1.0FD2F8FD
	$+202    0FD2F8B5     83E8 03                     SUB EAX,0x3
	$+205    0FD2F8B8     74 34                       JE SHORT PathOf_1.0FD2F8EE
	$+207    0FD2F8BA     2BC6                        SUB EAX,ESI
	$+209    0FD2F8BC     74 24                       JE SHORT PathOf_1.0FD2F8E2
	$+20B    0FD2F8BE     48                          DEC EAX
	$+20C    0FD2F8BF     48                          DEC EAX
	$+20D    0FD2F8C0     74 14                       JE SHORT PathOf_1.0FD2F8D6
	$+20F    0FD2F8C2     83E8 03                     SUB EAX,0x3
	$+212    0FD2F8C5     0F85 56090000               JNZ PathOf_1.0FD30221
	$+218    0FD2F8CB     09B5 F0FDFFFF               OR DWORD PTR SS:[EBP-0x210],ESI
	$+21E    0FD2F8D1     E9 4B090000                 JMP PathOf_1.0FD30221
	$+223    0FD2F8D6     838D F0FDFFFF 04            OR DWORD PTR SS:[EBP-0x210],0x4
	$+22A    0FD2F8DD     E9 3F090000                 JMP PathOf_1.0FD30221
	$+22F    0FD2F8E2     838D F0FDFFFF 01            OR DWORD PTR SS:[EBP-0x210],0x1
	$+236    0FD2F8E9     E9 33090000                 JMP PathOf_1.0FD30221
	$+23B    0FD2F8EE     818D F0FDFFFF 80000000      OR DWORD PTR SS:[EBP-0x210],0x80
	$+245    0FD2F8F8     E9 24090000                 JMP PathOf_1.0FD30221
	$+24A    0FD2F8FD     838D F0FDFFFF 02            OR DWORD PTR SS:[EBP-0x210],0x2
	$+251    0FD2F904     E9 18090000                 JMP PathOf_1.0FD30221
	$+256    0FD2F909     80FA 2A                     CMP DL,0x2A
	$+259    0FD2F90C     75 2C                       JNZ SHORT PathOf_1.0FD2F93A
	$+25B    0FD2F90E     83C7 04                     ADD EDI,0x4
	$+25E    0FD2F911     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+264    0FD2F917     8B7F FC                     MOV EDI,DWORD PTR DS:[EDI-0x4]
	$+267    0FD2F91A     89BD CCFDFFFF               MOV DWORD PTR SS:[EBP-0x234],EDI
	$+26D    0FD2F920     85FF                        TEST EDI,EDI
	$+26F    0FD2F922     0F8D F9080000               JGE PathOf_1.0FD30221
	$+275    0FD2F928     838D F0FDFFFF 04            OR DWORD PTR SS:[EBP-0x210],0x4
	$+27C    0FD2F92F     F79D CCFDFFFF               NEG DWORD PTR SS:[EBP-0x234]
	$+282    0FD2F935     E9 E7080000                 JMP PathOf_1.0FD30221
	$+287    0FD2F93A     8B85 CCFDFFFF               MOV EAX,DWORD PTR SS:[EBP-0x234]
	$+28D    0FD2F940     6BC0 0A                     IMUL EAX,EAX,0xA
	$+290    0FD2F943     0FBECA                      MOVSX ECX,DL
	$+293    0FD2F946     8D4408 D0                   LEA EAX,DWORD PTR DS:[EAX+ECX-0x30]
	$+297    0FD2F94A     8985 CCFDFFFF               MOV DWORD PTR SS:[EBP-0x234],EAX
	$+29D    0FD2F950     E9 CC080000                 JMP PathOf_1.0FD30221
	$+2A2    0FD2F955     83A5 E8FDFFFF 00            AND DWORD PTR SS:[EBP-0x218],0x0
	$+2A9    0FD2F95C     E9 C0080000                 JMP PathOf_1.0FD30221
	$+2AE    0FD2F961     80FA 2A                     CMP DL,0x2A
	$+2B1    0FD2F964     75 26                       JNZ SHORT PathOf_1.0FD2F98C
	$+2B3    0FD2F966     83C7 04                     ADD EDI,0x4
	$+2B6    0FD2F969     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+2BC    0FD2F96F     8B7F FC                     MOV EDI,DWORD PTR DS:[EDI-0x4]
	$+2BF    0FD2F972     89BD E8FDFFFF               MOV DWORD PTR SS:[EBP-0x218],EDI
	$+2C5    0FD2F978     85FF                        TEST EDI,EDI
	$+2C7    0FD2F97A     0F8D A1080000               JGE PathOf_1.0FD30221
	$+2CD    0FD2F980     838D E8FDFFFF FF            OR DWORD PTR SS:[EBP-0x218],0xFFFFFFFF
	$+2D4    0FD2F987     E9 95080000                 JMP PathOf_1.0FD30221
	$+2D9    0FD2F98C     8B85 E8FDFFFF               MOV EAX,DWORD PTR SS:[EBP-0x218]
	$+2DF    0FD2F992     6BC0 0A                     IMUL EAX,EAX,0xA
	$+2E2    0FD2F995     0FBECA                      MOVSX ECX,DL
	$+2E5    0FD2F998     8D4408 D0                   LEA EAX,DWORD PTR DS:[EAX+ECX-0x30]
	$+2E9    0FD2F99C     8985 E8FDFFFF               MOV DWORD PTR SS:[EBP-0x218],EAX
	$+2EF    0FD2F9A2     E9 7A080000                 JMP PathOf_1.0FD30221
	$+2F4    0FD2F9A7     80FA 49                     CMP DL,0x49
	$+2F7    0FD2F9AA     74 55                       JE SHORT PathOf_1.0FD2FA01
	$+2F9    0FD2F9AC     80FA 68                     CMP DL,0x68
	$+2FC    0FD2F9AF     74 44                       JE SHORT PathOf_1.0FD2F9F5
	$+2FE    0FD2F9B1     80FA 6C                     CMP DL,0x6C
	$+301    0FD2F9B4     74 18                       JE SHORT PathOf_1.0FD2F9CE
	$+303    0FD2F9B6     80FA 77                     CMP DL,0x77
	$+306    0FD2F9B9     0F85 62080000               JNZ PathOf_1.0FD30221
	$+30C    0FD2F9BF     818D F0FDFFFF 00080000      OR DWORD PTR SS:[EBP-0x210],0x800
	$+316    0FD2F9C9     E9 53080000                 JMP PathOf_1.0FD30221
	$+31B    0FD2F9CE     803B 6C                     CMP BYTE PTR DS:[EBX],0x6C
	$+31E    0FD2F9D1     75 16                       JNZ SHORT PathOf_1.0FD2F9E9
	$+320    0FD2F9D3     43                          INC EBX
	$+321    0FD2F9D4     818D F0FDFFFF 00100000      OR DWORD PTR SS:[EBP-0x210],0x1000
	$+32B    0FD2F9DE     899D C4FDFFFF               MOV DWORD PTR SS:[EBP-0x23C],EBX
	$+331    0FD2F9E4     E9 38080000                 JMP PathOf_1.0FD30221
	$+336    0FD2F9E9     838D F0FDFFFF 10            OR DWORD PTR SS:[EBP-0x210],0x10
	$+33D    0FD2F9F0     E9 2C080000                 JMP PathOf_1.0FD30221
	$+342    0FD2F9F5     838D F0FDFFFF 20            OR DWORD PTR SS:[EBP-0x210],0x20
	$+349    0FD2F9FC     E9 20080000                 JMP PathOf_1.0FD30221
	$+34E    0FD2FA01     8A03                        MOV AL,BYTE PTR DS:[EBX]
	$+350    0FD2FA03     3C 36                       CMP AL,0x36
	$+352    0FD2FA05     75 1D                       JNZ SHORT PathOf_1.0FD2FA24
	$+354    0FD2FA07     807B 01 34                  CMP BYTE PTR DS:[EBX+0x1],0x34
	$+358    0FD2FA0B     75 17                       JNZ SHORT PathOf_1.0FD2FA24
	$+35A    0FD2FA0D     43                          INC EBX
	$+35B    0FD2FA0E     43                          INC EBX
	$+35C    0FD2FA0F     818D F0FDFFFF 00800000      OR DWORD PTR SS:[EBP-0x210],0x8000
	$+366    0FD2FA19     899D C4FDFFFF               MOV DWORD PTR SS:[EBP-0x23C],EBX
	$+36C    0FD2FA1F     E9 FD070000                 JMP PathOf_1.0FD30221
	$+371    0FD2FA24     3C 33                       CMP AL,0x33
	$+373    0FD2FA26     75 1D                       JNZ SHORT PathOf_1.0FD2FA45
	$+375    0FD2FA28     807B 01 32                  CMP BYTE PTR DS:[EBX+0x1],0x32
	$+379    0FD2FA2C     75 17                       JNZ SHORT PathOf_1.0FD2FA45
	$+37B    0FD2FA2E     43                          INC EBX
	$+37C    0FD2FA2F     43                          INC EBX
	$+37D    0FD2FA30     81A5 F0FDFFFF FF7FFFFF      AND DWORD PTR SS:[EBP-0x210],0xFFFF7FFF
	$+387    0FD2FA3A     899D C4FDFFFF               MOV DWORD PTR SS:[EBP-0x23C],EBX
	$+38D    0FD2FA40     E9 DC070000                 JMP PathOf_1.0FD30221
	$+392    0FD2FA45     3C 64                       CMP AL,0x64
	$+394    0FD2FA47     0F84 D4070000               JE PathOf_1.0FD30221
	$+39A    0FD2FA4D     3C 69                       CMP AL,0x69
	$+39C    0FD2FA4F     0F84 CC070000               JE PathOf_1.0FD30221
	$+3A2    0FD2FA55     3C 6F                       CMP AL,0x6F
	$+3A4    0FD2FA57     0F84 C4070000               JE PathOf_1.0FD30221
	$+3AA    0FD2FA5D     3C 75                       CMP AL,0x75
	$+3AC    0FD2FA5F     0F84 BC070000               JE PathOf_1.0FD30221
	$+3B2    0FD2FA65     3C 78                       CMP AL,0x78
	$+3B4    0FD2FA67     0F84 B4070000               JE PathOf_1.0FD30221
	$+3BA    0FD2FA6D     3C 58                       CMP AL,0x58
	$+3BC    0FD2FA6F     0F84 AC070000               JE PathOf_1.0FD30221
	$+3C2    0FD2FA75     83A5 C0FDFFFF 00            AND DWORD PTR SS:[EBP-0x240],0x0
	$+3C9    0FD2FA7C     83A5 C8FDFFFF 00            AND DWORD PTR SS:[EBP-0x238],0x0
	$+3D0    0FD2FA83     8D85 B0FDFFFF               LEA EAX,DWORD PTR SS:[EBP-0x250]
	$+3D6    0FD2FA89     50                          PUSH EAX
	$+3D7    0FD2FA8A     0FB6C2                      MOVZX EAX,DL
	$+3DA    0FD2FA8D     50                          PUSH EAX
	$+3DB    0FD2FA8E     E8 780C0000                 CALL PathOf_1.0FD3070B
	$+3E0    0FD2FA93     59                          POP ECX
	$+3E1    0FD2FA94     85C0                        TEST EAX,EAX
	$+3E3    0FD2FA96     8A85 EFFDFFFF               MOV AL,BYTE PTR SS:[EBP-0x211]
	$+3E9    0FD2FA9C     59                          POP ECX
	$+3EA    0FD2FA9D     74 22                       JE SHORT PathOf_1.0FD2FAC1
	$+3EC    0FD2FA9F     8B8D A0FDFFFF               MOV ECX,DWORD PTR SS:[EBP-0x260]
	$+3F2    0FD2FAA5     8DB5 D8FDFFFF               LEA ESI,DWORD PTR SS:[EBP-0x228]
	$+3F8    0FD2FAAB     E8 8CFBFFFF                 CALL PathOf_1.0FD2F63C
	$+3FD    0FD2FAB0     8A03                        MOV AL,BYTE PTR DS:[EBX]
	$+3FF    0FD2FAB2     43                          INC EBX
	$+400    0FD2FAB3     899D C4FDFFFF               MOV DWORD PTR SS:[EBP-0x23C],EBX
	$+406    0FD2FAB9     84C0                        TEST AL,AL
	$+408    0FD2FABB   ^ 0F84 8CFCFFFF               JE PathOf_1.0FD2F74D
	$+40E    0FD2FAC1     8B8D A0FDFFFF               MOV ECX,DWORD PTR SS:[EBP-0x260]
	$+414    0FD2FAC7     8DB5 D8FDFFFF               LEA ESI,DWORD PTR SS:[EBP-0x228]
	$+41A    0FD2FACD     E8 6AFBFFFF                 CALL PathOf_1.0FD2F63C
	$+41F    0FD2FAD2     E9 4A070000                 JMP PathOf_1.0FD30221
	$+424    0FD2FAD7     0FBEC2                      MOVSX EAX,DL
	$+427    0FD2FADA     83F8 64                     CMP EAX,0x64
	$+42A    0FD2FADD     0F8F EA010000               JG PathOf_1.0FD2FCCD
	$+430    0FD2FAE3     0F84 77020000               JE PathOf_1.0FD2FD60
	$+436    0FD2FAE9     83F8 53                     CMP EAX,0x53
	$+439    0FD2FAEC     0F8F F3000000               JG PathOf_1.0FD2FBE5
	$+43F    0FD2FAF2     0F84 81000000               JE PathOf_1.0FD2FB79
	$+445    0FD2FAF8     83E8 41                     SUB EAX,0x41
	$+448    0FD2FAFB     74 10                       JE SHORT PathOf_1.0FD2FB0D
	$+44A    0FD2FAFD     48                          DEC EAX
	$+44B    0FD2FAFE     48                          DEC EAX
	$+44C    0FD2FAFF     74 59                       JE SHORT PathOf_1.0FD2FB5A
	$+44E    0FD2FB01     48                          DEC EAX
	$+44F    0FD2FB02     48                          DEC EAX
	$+450    0FD2FB03     74 08                       JE SHORT PathOf_1.0FD2FB0D
	$+452    0FD2FB05     48                          DEC EAX
	$+453    0FD2FB06     48                          DEC EAX
	$+454    0FD2FB07     0F85 8F050000               JNZ PathOf_1.0FD3009C
	$+45A    0FD2FB0D     80C2 20                     ADD DL,0x20
	$+45D    0FD2FB10     C785 94FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x26C],0x1
	$+467    0FD2FB1A     8895 EFFDFFFF               MOV BYTE PTR SS:[EBP-0x211],DL
	$+46D    0FD2FB20     838D F0FDFFFF 40            OR DWORD PTR SS:[EBP-0x210],0x40
	$+474    0FD2FB27     83BD E8FDFFFF 00            CMP DWORD PTR SS:[EBP-0x218],0x0
	$+47B    0FD2FB2E     8D9D F4FDFFFF               LEA EBX,DWORD PTR SS:[EBP-0x20C]
	$+481    0FD2FB34     B8 00020000                 MOV EAX,0x200
	$+486    0FD2FB39     899D E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],EBX
	$+48C    0FD2FB3F     8985 9CFDFFFF               MOV DWORD PTR SS:[EBP-0x264],EAX
	$+492    0FD2FB45     0F8D 45020000               JGE PathOf_1.0FD2FD90
	$+498    0FD2FB4B     C785 E8FDFFFF 06000000      MOV DWORD PTR SS:[EBP-0x218],0x6
	$+4A2    0FD2FB55     E9 A3020000                 JMP PathOf_1.0FD2FDFD
	$+4A7    0FD2FB5A     F785 F0FDFFFF 30080000      TEST DWORD PTR SS:[EBP-0x210],0x830
	$+4B1    0FD2FB64     0F85 98000000               JNZ PathOf_1.0FD2FC02
	$+4B7    0FD2FB6A     818D F0FDFFFF 00080000      OR DWORD PTR SS:[EBP-0x210],0x800
	$+4C1    0FD2FB74     E9 89000000                 JMP PathOf_1.0FD2FC02
	$+4C6    0FD2FB79     F785 F0FDFFFF 30080000      TEST DWORD PTR SS:[EBP-0x210],0x830
	$+4D0    0FD2FB83     75 0A                       JNZ SHORT PathOf_1.0FD2FB8F
	$+4D2    0FD2FB85     818D F0FDFFFF 00080000      OR DWORD PTR SS:[EBP-0x210],0x800
	$+4DC    0FD2FB8F     8B8D E8FDFFFF               MOV ECX,DWORD PTR SS:[EBP-0x218]
	$+4E2    0FD2FB95     83F9 FF                     CMP ECX,-0x1
	$+4E5    0FD2FB98     75 05                       JNZ SHORT PathOf_1.0FD2FB9F
	$+4E7    0FD2FB9A     B9 FFFFFF7F                 MOV ECX,0x7FFFFFFF
	$+4EC    0FD2FB9F     83C7 04                     ADD EDI,0x4
	$+4EF    0FD2FBA2     F785 F0FDFFFF 10080000      TEST DWORD PTR SS:[EBP-0x210],0x810
	$+4F9    0FD2FBAC     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+4FF    0FD2FBB2     8B7F FC                     MOV EDI,DWORD PTR DS:[EDI-0x4]
	$+502    0FD2FBB5     89BD E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],EDI
	$+508    0FD2FBBB     0F84 AD040000               JE PathOf_1.0FD3006E
	$+50E    0FD2FBC1     85FF                        TEST EDI,EDI
	$+510    0FD2FBC3     75 0B                       JNZ SHORT PathOf_1.0FD2FBD0
	$+512    0FD2FBC5     A1 04BCE40F                 MOV EAX,DWORD PTR DS:[0xFE4BC04]
	$+517    0FD2FBCA     8985 E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],EAX
	$+51D    0FD2FBD0     8B85 E4FDFFFF               MOV EAX,DWORD PTR SS:[EBP-0x21C]
	$+523    0FD2FBD6     C785 C8FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x238],0x1
	$+52D    0FD2FBE0     E9 7B040000                 JMP PathOf_1.0FD30060
	$+532    0FD2FBE5     83E8 58                     SUB EAX,0x58
	$+535    0FD2FBE8     0F84 D5020000               JE PathOf_1.0FD2FEC3
	$+53B    0FD2FBEE     48                          DEC EAX
	$+53C    0FD2FBEF     48                          DEC EAX
	$+53D    0FD2FBF0     74 79                       JE SHORT PathOf_1.0FD2FC6B
	$+53F    0FD2FBF2     2BC1                        SUB EAX,ECX
	$+541    0FD2FBF4   ^ 0F84 26FFFFFF               JE PathOf_1.0FD2FB20
	$+547    0FD2FBFA     48                          DEC EAX
	$+548    0FD2FBFB     48                          DEC EAX
	$+549    0FD2FBFC     0F85 9A040000               JNZ PathOf_1.0FD3009C
	$+54F    0FD2FC02     83C7 04                     ADD EDI,0x4
	$+552    0FD2FC05     F785 F0FDFFFF 10080000      TEST DWORD PTR SS:[EBP-0x210],0x810
	$+55C    0FD2FC0F     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+562    0FD2FC15     74 30                       JE SHORT PathOf_1.0FD2FC47
	$+564    0FD2FC17     0FB747 FC                   MOVZX EAX,WORD PTR DS:[EDI-0x4]
	$+568    0FD2FC1B     50                          PUSH EAX
	$+569    0FD2FC1C     68 00020000                 PUSH 0x200
	$+56E    0FD2FC21     8D85 F4FDFFFF               LEA EAX,DWORD PTR SS:[EBP-0x20C]
	$+574    0FD2FC27     50                          PUSH EAX
	$+575    0FD2FC28     8D85 E0FDFFFF               LEA EAX,DWORD PTR SS:[EBP-0x220]
	$+57B    0FD2FC2E     50                          PUSH EAX
	$+57C    0FD2FC2F     E8 5DF00000                 CALL PathOf_1.0FD3EC91
	$+581    0FD2FC34     83C4 10                     ADD ESP,0x10
	$+584    0FD2FC37     85C0                        TEST EAX,EAX
	$+586    0FD2FC39     74 1F                       JE SHORT PathOf_1.0FD2FC5A
	$+588    0FD2FC3B     C785 A8FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x258],0x1
	$+592    0FD2FC45     EB 13                       JMP SHORT PathOf_1.0FD2FC5A
	$+594    0FD2FC47     8A47 FC                     MOV AL,BYTE PTR DS:[EDI-0x4]
	$+597    0FD2FC4A     8885 F4FDFFFF               MOV BYTE PTR SS:[EBP-0x20C],AL
	$+59D    0FD2FC50     C785 E0FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x220],0x1
	$+5A7    0FD2FC5A     8D85 F4FDFFFF               LEA EAX,DWORD PTR SS:[EBP-0x20C]
	$+5AD    0FD2FC60     8985 E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],EAX
	$+5B3    0FD2FC66     E9 31040000                 JMP PathOf_1.0FD3009C
	$+5B8    0FD2FC6B     8B07                        MOV EAX,DWORD PTR DS:[EDI]
	$+5BA    0FD2FC6D     83C7 04                     ADD EDI,0x4
	$+5BD    0FD2FC70     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+5C3    0FD2FC76     85C0                        TEST EAX,EAX
	$+5C5    0FD2FC78     74 3C                       JE SHORT PathOf_1.0FD2FCB6
	$+5C7    0FD2FC7A     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+5CA    0FD2FC7D     85C9                        TEST ECX,ECX
	$+5CC    0FD2FC7F     74 35                       JE SHORT PathOf_1.0FD2FCB6
	$+5CE    0FD2FC81     F785 F0FDFFFF 00080000      TEST DWORD PTR SS:[EBP-0x210],0x800
	$+5D8    0FD2FC8B     0FBF00                      MOVSX EAX,WORD PTR DS:[EAX]
	$+5DB    0FD2FC8E     898D E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],ECX
	$+5E1    0FD2FC94     74 14                       JE SHORT PathOf_1.0FD2FCAA
	$+5E3    0FD2FC96     99                          CDQ
	$+5E4    0FD2FC97     2BC2                        SUB EAX,EDX
	$+5E6    0FD2FC99     D1F8                        SAR EAX,1
	$+5E8    0FD2FC9B     C785 C8FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x238],0x1
	$+5F2    0FD2FCA5     E9 EC030000                 JMP PathOf_1.0FD30096
	$+5F7    0FD2FCAA     83A5 C8FDFFFF 00            AND DWORD PTR SS:[EBP-0x238],0x0
	$+5FE    0FD2FCB1     E9 E0030000                 JMP PathOf_1.0FD30096
	$+603    0FD2FCB6     A1 00BCE40F                 MOV EAX,DWORD PTR DS:[0xFE4BC00]
	$+608    0FD2FCBB     8985 E4FDFFFF               MOV DWORD PTR SS:[EBP-0x21C],EAX
	$+60E    0FD2FCC1     50                          PUSH EAX
	$+60F    0FD2FCC2     E8 19DAFFFF                 CALL PathOf_1.0FD2D6E0
	$+614    0FD2FCC7     59                          POP ECX
	$+615    0FD2FCC8     E9 C9030000                 JMP PathOf_1.0FD30096
	$+61A    0FD2FCCD     83F8 70                     CMP EAX,0x70
	$+61D    0FD2FCD0     0F8F F5010000               JG PathOf_1.0FD2FECB
	$+623    0FD2FCD6     0F84 E1010000               JE PathOf_1.0FD2FEBD
	$+629    0FD2FCDC     83F8 65                     CMP EAX,0x65
	$+62C    0FD2FCDF     0F8C B7030000               JL PathOf_1.0FD3009C
	$+632    0FD2FCE5     83F8 67                     CMP EAX,0x67
	$+635    0FD2FCE8   ^ 0F8E 32FEFFFF               JLE PathOf_1.0FD2FB20
	$+63B    0FD2FCEE     83F8 69                     CMP EAX,0x69
	$+63E    0FD2FCF1     74 6D                       JE SHORT PathOf_1.0FD2FD60
	$+640    0FD2FCF3     83F8 6E                     CMP EAX,0x6E
	$+643    0FD2FCF6     74 24                       JE SHORT PathOf_1.0FD2FD1C
	$+645    0FD2FCF8     83F8 6F                     CMP EAX,0x6F
	$+648    0FD2FCFB     0F85 9B030000               JNZ PathOf_1.0FD3009C
	$+64E    0FD2FD01     F685 F0FDFFFF 80            TEST BYTE PTR SS:[EBP-0x210],0x80
	$+655    0FD2FD08     89B5 E0FDFFFF               MOV DWORD PTR SS:[EBP-0x220],ESI
	$+65B    0FD2FD0E     74 61                       JE SHORT PathOf_1.0FD2FD71
	$+65D    0FD2FD10     818D F0FDFFFF 00020000      OR DWORD PTR SS:[EBP-0x210],0x200
	$+667    0FD2FD1A     EB 55                       JMP SHORT PathOf_1.0FD2FD71
	$+669    0FD2FD1C     8B37                        MOV ESI,DWORD PTR DS:[EDI]
	$+66B    0FD2FD1E     83C7 04                     ADD EDI,0x4
	$+66E    0FD2FD21     89BD DCFDFFFF               MOV DWORD PTR SS:[EBP-0x224],EDI
	$+674    0FD2FD27     E8 F84EFFFF                 CALL PathOf_1.0FD24C24
	$+679    0FD2FD2C     85C0                        TEST EAX,EAX
	$+67B    0FD2FD2E   ^ 0F84 19FAFFFF               JE PathOf_1.0FD2F74D
	$+681    0FD2FD34     F685 F0FDFFFF 20            TEST BYTE PTR SS:[EBP-0x210],0x20
	$+688    0FD2FD3B     74 0C                       JE SHORT PathOf_1.0FD2FD49
	$+68A    0FD2FD3D     66:8B85 D8FDFFFF            MOV AX,WORD PTR SS:[EBP-0x228]
	$+691    0FD2FD44     66:8906                     MOV WORD PTR DS:[ESI],AX
	$+694    0FD2FD47     EB 08                       JMP SHORT PathOf_1.0FD2FD51
	$+696    0FD2FD49     8B85 D8FDFFFF               MOV EAX,DWORD PTR SS:[EBP-0x228]
	$+69C    0FD2FD4F     8906                        MOV DWORD PTR DS:[ESI],EAX
	$+69E    0FD2FD51     C785 A8FDFFFF 01000000      MOV DWORD PTR SS:[EBP-0x258],0x1
	$+6A8    0FD2FD5B     E9 A5040000                 JMP PathOf_1.0FD30205
	$+6AD    0FD2FD60     838D F0FDFFFF 40            OR DWORD PTR SS:[EBP-0x210],0x40
	$+6B4    0FD2FD67     C785 E0FDFFFF 0A000000      MOV DWORD PTR SS:[EBP-0x220],0xA
	$+6BE    0FD2FD71     8B8D F0FDFFFF               MOV ECX,DWORD PTR SS:[EBP-0x210]
	*/
	addr_tmp = scan_base_.ScanAddr("8A03FF4D??8BCFE8????????43833E", -0x1E);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_base_sprintf1_ = " << scan_base_.OffsetToModuleBegin(addr_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = scan_base_.ScanAddr("8A03FF4D??8BCFE8????????43833E", -0x1E, 2);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_base_sprintf2_ = " << scan_base_.OffsetToModuleBegin(addr_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = scan_base_.ScanAddr("8A03FF4D??8BCFE8????????43833E", -0x1E, 3);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_base_sprintf3_ = " << scan_base_.OffsetToModuleBegin(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
