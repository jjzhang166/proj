#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Hooks(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;

	/*
	$-22F    008D362F     CC                        INT3
	$-22E    008D3630     51                        PUSH ECX
	$-22D    008D3631     53                        PUSH EBX
	$-22C    008D3632     56                        PUSH ESI
	$-22B    008D3633     57                        PUSH EDI
	$-22A    008D3634     FF7424 14                 PUSH DWORD PTR SS:[ESP+0x14]
	$-226    008D3638     8BF9                      MOV EDI,ECX
	$-224    008D363A     8D9F 083C0000             LEA EBX,DWORD PTR DS:[EDI+0x3C08]
	$-21E    008D3640     8BCB                      MOV ECX,EBX
	$-21C    008D3642     E8 4930D3FF               CALL PathOfEx.00606690
	$-217    008D3647     8BB7 CC3C0000             MOV ESI,DWORD PTR DS:[EDI+0x3CCC]
	$-211    008D364D     897424 14                 MOV DWORD PTR SS:[ESP+0x14],ESI
	$-20D    008D3651     3BB7 D03C0000             CMP ESI,DWORD PTR DS:[EDI+0x3CD0]
	$-207    008D3657     74 46                     JE SHORT PathOfEx.008D369F
	$-205    008D3659     8D4424 14                 LEA EAX,DWORD PTR SS:[ESP+0x14]
	$-201    008D365D     894424 0C                 MOV DWORD PTR SS:[ESP+0xC],EAX
	$-1FD    008D3661     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$-1F9    008D3665     66                        DATASIZE:
	$-1F8    008D3666     66                        DATASIZE:
	$-1F7    008D3667     66:0F1F8400 00000000      NOP WORD PTR DS:[EAX+EAX]
	$-1EE    008D3670     8B97 5C3C0000             MOV EDX,DWORD PTR DS:[EDI+0x3C5C]
	$-1E8    008D3676     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-1E4    008D367A     8B8F 583C0000             MOV ECX,DWORD PTR DS:[EDI+0x3C58]
	$-1DE    008D3680     50                        PUSH EAX
	$-1DD    008D3681     E8 6AAE0000               CALL PathOfEx.008DE4F0
	$-1D8    008D3686     8846 04                   MOV BYTE PTR DS:[ESI+0x4],AL
	$-1D5    008D3689     83C4 04                   ADD ESP,0x4
	$-1D2    008D368C     8B7424 14                 MOV ESI,DWORD PTR SS:[ESP+0x14]
	$-1CE    008D3690     83C6 08                   ADD ESI,0x8
	$-1CB    008D3693     897424 14                 MOV DWORD PTR SS:[ESP+0x14],ESI
	$-1C7    008D3697     3BB7 D03C0000             CMP ESI,DWORD PTR DS:[EDI+0x3CD0]
	$-1C1    008D369D   ^ 75 D1                     JNZ SHORT PathOfEx.008D3670
	$-1BF    008D369F     53                        PUSH EBX
	$-1BE    008D36A0     8D8F 94180000             LEA ECX,DWORD PTR DS:[EDI+0x1894]
	$-1B8    008D36A6     E8 259DC2FF               CALL PathOfEx.004FD3D0
	$-1B3    008D36AB     5F                        POP EDI
	$-1B2    008D36AC     5E                        POP ESI
	$-1B1    008D36AD     5B                        POP EBX
	$-1B0    008D36AE     59                        POP ECX
	$-1AF    008D36AF     C2 0400                   RETN 0x4
	$-1AC    008D36B2     CC                        INT3
	$-1AB    008D36B3     CC                        INT3
	$-1AA    008D36B4     CC                        INT3
	$-1A9    008D36B5     CC                        INT3
	$-1A8    008D36B6     CC                        INT3
	$-1A7    008D36B7     CC                        INT3
	$-1A6    008D36B8     CC                        INT3
	$-1A5    008D36B9     CC                        INT3
	$-1A4    008D36BA     CC                        INT3
	$-1A3    008D36BB     CC                        INT3
	$-1A2    008D36BC     CC                        INT3
	$-1A1    008D36BD     CC                        INT3
	$-1A0    008D36BE     CC                        INT3
	$-19F    008D36BF     CC                        INT3
	$-19E    008D36C0     55                        PUSH EBP                                        ; 这是个具体收包处理的函数
	$-19D    008D36C1     8BEC                      MOV EBP,ESP
	$-19B    008D36C3     6A FF                     PUSH -0x1
	$-199    008D36C5     68 6C57D600               PUSH PathOfEx.00D6576C
	$-194    008D36CA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-18E    008D36D0     50                        PUSH EAX
	$-18D    008D36D1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-186    008D36D8     83EC 18                   SUB ESP,0x18
	$-183    008D36DB     53                        PUSH EBX
	$-182    008D36DC     56                        PUSH ESI
	$-181    008D36DD     8BD9                      MOV EBX,ECX
	$-17F    008D36DF     57                        PUSH EDI
	$-17E    008D36E0     895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$-17B    008D36E3     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$-174    008D36EA     83EC 0C                   SUB ESP,0xC
	$-171    008D36ED     8BD4                      MOV EDX,ESP
	$-16F    008D36EF     FF75 14                   PUSH DWORD PTR SS:[EBP+0x14]
	$-16C    008D36F2     C702 00000000             MOV DWORD PTR DS:[EDX],0x0
	$-166    008D36F8     C742 04 00000000          MOV DWORD PTR DS:[EDX+0x4],0x0
	$-15F    008D36FF     C742 08 00000000          MOV DWORD PTR DS:[EDX+0x8],0x0
	$-158    008D3706     8B45 2C                   MOV EAX,DWORD PTR SS:[EBP+0x2C]
	$-155    008D3709     8902                      MOV DWORD PTR DS:[EDX],EAX
	$-153    008D370B     8B45 30                   MOV EAX,DWORD PTR SS:[EBP+0x30]
	$-150    008D370E     8942 04                   MOV DWORD PTR DS:[EDX+0x4],EAX
	$-14D    008D3711     8B45 34                   MOV EAX,DWORD PTR SS:[EBP+0x34]
	$-14A    008D3714     8942 08                   MOV DWORD PTR DS:[EDX+0x8],EAX
	$-147    008D3717     0FB645 28                 MOVZX EAX,BYTE PTR SS:[EBP+0x28]
	$-143    008D371B     50                        PUSH EAX
	$-142    008D371C     83EC 10                   SUB ESP,0x10
	$-13F    008D371F     C745 2C 00000000          MOV DWORD PTR SS:[EBP+0x2C],0x0
	$-138    008D3726     8BCC                      MOV ECX,ESP
	$-136    008D3728     C745 30 00000000          MOV DWORD PTR SS:[EBP+0x30],0x0
	$-12F    008D372F     C745 34 00000000          MOV DWORD PTR SS:[EBP+0x34],0x0
	$-128    008D3736     FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
	$-125    008D3739     C701 44BBE700             MOV DWORD PTR DS:[ECX],PathOfEx.00E7BB44        ; ASCII "鹈u"
	$-11F    008D373F     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$-11C    008D3742     C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
	$-115    008D3749     C741 08 00000000          MOV DWORD PTR DS:[ECX+0x8],0x0
	$-10E    008D3750     C741 0C 00000000          MOV DWORD PTR DS:[ECX+0xC],0x0
	$-107    008D3757     8B45 1C                   MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$-104    008D375A     8941 04                   MOV DWORD PTR DS:[ECX+0x4],EAX
	$-101    008D375D     8B45 20                   MOV EAX,DWORD PTR SS:[EBP+0x20]
	$-FE     008D3760     8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
	$-FB     008D3763     8B45 24                   MOV EAX,DWORD PTR SS:[EBP+0x24]
	$-F8     008D3766     8941 0C                   MOV DWORD PTR DS:[ECX+0xC],EAX
	$-F5     008D3769     8B8B 943A0000             MOV ECX,DWORD PTR DS:[EBX+0x3A94]
	$-EF     008D376F     C745 1C 00000000          MOV DWORD PTR SS:[EBP+0x1C],0x0
	$-E8     008D3776     C745 20 00000000          MOV DWORD PTR SS:[EBP+0x20],0x0
	$-E1     008D377D     C745 24 00000000          MOV DWORD PTR SS:[EBP+0x24],0x0
	$-DA     008D3784     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-D8     008D3786     FF50 24                   CALL DWORD PTR DS:[EAX+0x24]
	$-D5     008D3789     8B8B 943A0000             MOV ECX,DWORD PTR DS:[EBX+0x3A94]
	$-CF     008D378F     8B41 0C                   MOV EAX,DWORD PTR DS:[ECX+0xC]
	$-CC     008D3792     8883 10400000             MOV BYTE PTR DS:[EBX+0x4010],AL
	$-C6     008D3798     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-C4     008D379A     FF50 14                   CALL DWORD PTR DS:[EAX+0x14]
	$-C1     008D379D     C640 5C 00                MOV BYTE PTR DS:[EAX+0x5C],0x0
	$-BD     008D37A1     8D45 14                   LEA EAX,DWORD PTR SS:[EBP+0x14]
	$-BA     008D37A4     50                        PUSH EAX
	$-B9     008D37A5     E8 6686F2FF               CALL PathOfEx.007FBE10
	$-B4     008D37AA     83C4 04                   ADD ESP,0x4
	$-B1     008D37AD     8B5D 10                   MOV EBX,DWORD PTR SS:[EBP+0x10]
	$-AE     008D37B0     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$-AA     008D37B4     8B7D 14                   MOV EDI,DWORD PTR SS:[EBP+0x14]
	$-A7     008D37B7     8B33                      MOV ESI,DWORD PTR DS:[EBX]
	$-A5     008D37B9     3B73 04                   CMP ESI,DWORD PTR DS:[EBX+0x4]
	$-A2     008D37BC     74 46                     JE SHORT PathOfEx.008D3804
	$-A0     008D37BE     66:90                     NOP
	$-9E     008D37C0     85FF                      TEST EDI,EDI                                    ; 循环开始
	$-9C     008D37C2     74 07                     JE SHORT PathOfEx.008D37CB
	$-9A     008D37C4     8BCF                      MOV ECX,EDI
	$-98     008D37C6     E8 F55DE4FF               CALL PathOfEx.007195C0
	$-93     008D37CB     FF36                      PUSH DWORD PTR DS:[ESI]
	$-91     008D37CD     8B4F 30                   MOV ECX,DWORD PTR DS:[EDI+0x30]
	$-8E     008D37D0     8D45 28                   LEA EAX,DWORD PTR SS:[EBP+0x28]
	$-8B     008D37D3     50                        PUSH EAX
	$-8A     008D37D4     E8 47E1F2FF               CALL PathOfEx.00801920
	$-85     008D37D9     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$-81     008D37DD     8B45 28                   MOV EAX,DWORD PTR SS:[EBP+0x28]
	$-7E     008D37E0     85C0                      TEST EAX,EAX
	$-7C     008D37E2     74 14                     JE SHORT PathOfEx.008D37F8
	$-7A     008D37E4     50                        PUSH EAX
	$-79     008D37E5     E8 1648E4FF               CALL PathOfEx.00718000
	$-74     008D37EA     8BC8                      MOV ECX,EAX
	$-72     008D37EC     E8 9F43E4FF               CALL PathOfEx.00717B90
	$-6D     008D37F1     C745 28 00000000          MOV DWORD PTR SS:[EBP+0x28],0x0
	$-66     008D37F8     83C6 04                   ADD ESI,0x4
	$-63     008D37FB     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$-5F     008D37FF     3B73 04                   CMP ESI,DWORD PTR DS:[EBX+0x4]
	$-5C     008D3802   ^ 75 BC                     JNZ SHORT PathOfEx.008D37C0                     ; 循环结束
	$-5A     008D3804     C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
	$-56     008D3808     85FF                      TEST EDI,EDI
	$-54     008D380A     74 0D                     JE SHORT PathOfEx.008D3819
	$-52     008D380C     57                        PUSH EDI
	$-51     008D380D     E8 EE47E4FF               CALL PathOfEx.00718000
	$-4C     008D3812     8BC8                      MOV ECX,EAX
	$-4A     008D3814     E8 7743E4FF               CALL PathOfEx.00717B90
	$-45     008D3819     B8 3E000000               MOV EAX,0x3E
	$-40     008D381E     C745 DC 209BEF00          MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EF9B20
	$-39     008D3825     66:8945 E0                MOV WORD PTR SS:[EBP-0x20],AX
	$-35     008D3829     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-32     008D382C     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$-2E     008D3830     8B8E 943A0000             MOV ECX,DWORD PTR DS:[ESI+0x3A94]
	$-28     008D3836     8B41 1C                   MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$-25     008D3839     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C],EAX
	$-22     008D383C     8B41 20                   MOV EAX,DWORD PTR DS:[ECX+0x20]
	$-1F     008D383F     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-1C     008D3842     8D45 DC                   LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-19     008D3845     50                        PUSH EAX
	$-18     008D3846     FFB6 983A0000             PUSH DWORD PTR DS:[ESI+0x3A98]
	$-12     008D384C     E8 AF6ADEFF               CALL PathOfEx.006BA300
	$-D      008D3851     8BC8                      MOV ECX,EAX
	$-B      008D3853     E8 686B0100               CALL PathOfEx.008EA3C0
	$-6      008D3858     8B8E 983A0000             MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$ ==>    008D385E     6A 00                     PUSH 0x0
	$+2      008D3860     E8 1B1AECFF               CALL PathOfEx.00795280
	$+7      008D3865     8B4D 1C                   MOV ECX,DWORD PTR SS:[EBP+0x1C]
	$+A      008D3868     C745 DC C49BEF00          MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EF9BC4   ; ASCII " 殒"
	$+11     008D386F     85C9                      TEST ECX,ECX
	$+13     008D3871     74 29                     JE SHORT PathOfEx.008D389C
	$+15     008D3873     8B45 24                   MOV EAX,DWORD PTR SS:[EBP+0x24]
	$+18     008D3876     2BC1                      SUB EAX,ECX
	$+1A     008D3878     6A 08                     PUSH 0x8
	$+1C     008D387A     C1F8 03                   SAR EAX,0x3
	$+1F     008D387D     50                        PUSH EAX
	$+20     008D387E     51                        PUSH ECX
	$+21     008D387F     E8 AC8FB9FF               CALL PathOfEx.0046C830
	$+26     008D3884     83C4 0C                   ADD ESP,0xC
	$+29     008D3887     C745 1C 00000000          MOV DWORD PTR SS:[EBP+0x1C],0x0
	$+30     008D388E     C745 20 00000000          MOV DWORD PTR SS:[EBP+0x20],0x0
	$+37     008D3895     C745 24 00000000          MOV DWORD PTR SS:[EBP+0x24],0x0
	$+3E     008D389C     8B4D 2C                   MOV ECX,DWORD PTR SS:[EBP+0x2C]
	$+41     008D389F     C745 18 38BBE700          MOV DWORD PTR SS:[EBP+0x18],PathOfEx.00E7BB38   ; ASCII "0yo"
	$+48     008D38A6     85C9                      TEST ECX,ECX
	$+4A     008D38A8     74 14                     JE SHORT PathOfEx.008D38BE
	$+4C     008D38AA     8B45 34                   MOV EAX,DWORD PTR SS:[EBP+0x34]
	$+4F     008D38AD     2BC1                      SUB EAX,ECX
	$+51     008D38AF     6A 04                     PUSH 0x4
	$+53     008D38B1     C1F8 02                   SAR EAX,0x2
	$+56     008D38B4     50                        PUSH EAX
	$+57     008D38B5     51                        PUSH ECX
	$+58     008D38B6     E8 758FB9FF               CALL PathOfEx.0046C830
	$+5D     008D38BB     83C4 0C                   ADD ESP,0xC
	$+60     008D38BE     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]                  ; g_h_before_init_game_map_
	$+63     008D38C1     5F                        POP EDI
	$+64     008D38C2     5E                        POP ESI
	$+65     008D38C3     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+6C     008D38CA     5B                        POP EBX
	$+6D     008D38CB     8BE5                      MOV ESP,EBP
	$+6F     008D38CD     5D                        POP EBP
	$+70     008D38CE     C2 3000                   RETN 0x30
	$+73     008D38D1     CC                        INT3
	$+74     008D38D2     CC                        INT3
	$+75     008D38D3     CC                        INT3
	$+76     008D38D4     CC                        INT3
	$+77     008D38D5     CC                        INT3
	$+78     008D38D6     CC                        INT3
	$+79     008D38D7     CC                        INT3
	$+7A     008D38D8     CC                        INT3
	$+7B     008D38D9     CC                        INT3
	$+7C     008D38DA     CC                        INT3
	$+7D     008D38DB     CC                        INT3
	$+7E     008D38DC     CC                        INT3
	$+7F     008D38DD     CC                        INT3
	$+80     008D38DE     CC                        INT3
	$+81     008D38DF     CC                        INT3
	$+82     008D38E0     55                        PUSH EBP
	$+83     008D38E1     8BEC                      MOV EBP,ESP
	$+85     008D38E3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+8B     008D38E9     6A FF                     PUSH -0x1
	$+8D     008D38EB     68 821AD200               PUSH PathOfEx.00D21A82                          ; ASCII "肛圄"
	$+92     008D38F0     50                        PUSH EAX
	$+93     008D38F1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+9A     008D38F8     83EC 08                   SUB ESP,0x8
	$+9D     008D38FB     53                        PUSH EBX
	$+9E     008D38FC     56                        PUSH ESI
	$+9F     008D38FD     57                        PUSH EDI
	$+A0     008D38FE     8BF9                      MOV EDI,ECX
	$+A2     008D3900     8D5F 08                   LEA EBX,DWORD PTR DS:[EDI+0x8]
	$+A5     008D3903     C747 0C C498EF00          MOV DWORD PTR DS:[EDI+0xC],PathOfEx.00EF98C4
	$+AC     008D390A     8D4F 0C                   LEA ECX,DWORD PTR DS:[EDI+0xC]
	$+AF     008D390D     C703 B498EF00             MOV DWORD PTR DS:[EBX],PathOfEx.00EF98B4
	$+B5     008D3913     8BB7 B83C0000             MOV ESI,DWORD PTR DS:[EDI+0x3CB8]
	$+BB     008D3919     C707 AC98EF00             MOV DWORD PTR DS:[EDI],PathOfEx.00EF98AC
	$+C1     008D391F     C747 04 B098EF00          MOV DWORD PTR DS:[EDI+0x4],PathOfEx.00EF98B0
	$+C8     008D3926     C747 10 FC98EF00          MOV DWORD PTR DS:[EDI+0x10],PathOfEx.00EF98FC   ; ASCII "廓F"
	$+CF     008D392D     85F6                      TEST ESI,ESI
	$+D1     008D392F     74 49                     JE SHORT PathOfEx.008D397A
	$+D3     008D3931     8B46 5C                   MOV EAX,DWORD PTR DS:[ESI+0x5C]
	$+D6     008D3934     8B56 58                   MOV EDX,DWORD PTR DS:[ESI+0x58]
	$+D9     008D3937     3BD0                      CMP EDX,EAX
	$+DB     008D3939     74 10                     JE SHORT PathOfEx.008D394B
	$+DD     008D393B     0F1F4400 00               NOP DWORD PTR DS:[EAX+EAX]
	$+E2     008D3940     390A                      CMP DWORD PTR DS:[EDX],ECX
	$+E4     008D3942     74 07                     JE SHORT PathOfEx.008D394B
	$+E6     008D3944     83C2 04                   ADD EDX,0x4
	$+E9     008D3947     3BD0                      CMP EDX,EAX
	$+EB     008D3949   ^ 75 F5                     JNZ SHORT PathOfEx.008D3940
	$+ED     008D394B     8BC8                      MOV ECX,EAX
	$+EF     008D394D     8D42 04                   LEA EAX,DWORD PTR DS:[EDX+0x4]
	$+F2     008D3950     2BC8                      SUB ECX,EAX
	$+F4     008D3952     51                        PUSH ECX
	$+F5     008D3953     50                        PUSH EAX
	$+F6     008D3954     52                        PUSH EDX
	$+F7     008D3955     E8 26133C00               CALL PathOfEx.00C94C80
	$+FC     008D395A     8346 5C FC                ADD DWORD PTR DS:[ESI+0x5C],-0x4
	$+100    008D395E     8D4F 10                   LEA ECX,DWORD PTR DS:[EDI+0x10]
	$+103    008D3961     8B87 B83C0000             MOV EAX,DWORD PTR DS:[EDI+0x3CB8]
	$+109    008D3967     83C4 0C                   ADD ESP,0xC
	$+10C    008D396A     51                        PUSH ECX
	$+10D    008D396B     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+110    008D396E     E8 5DEDBBFF               CALL PathOfEx.004926D0
	$+115    008D3973     8BC8                      MOV ECX,EAX
	$+117    008D3975     E8 66F8BCFF               CALL PathOfEx.004A31E0
	$+11C    008D397A     80BF 913B0000 00          CMP BYTE PTR DS:[EDI+0x3B91],0x0
	$+123    008D3981     74 5D                     JE SHORT PathOfEx.008D39E0
	$+125    008D3983     FFB7 503D0000             PUSH DWORD PTR DS:[EDI+0x3D50]
	$+12B    008D3989     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+12E    008D398C     50                        PUSH EAX
	$+12F    008D398D     8D8F EC3C0000             LEA ECX,DWORD PTR DS:[EDI+0x3CEC]
	$+135    008D3993     E8 3850F2FF               CALL PathOfEx.007F89D0
	$+13A    008D3998     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+13D    008D399B     8D87 C43A0000             LEA EAX,DWORD PTR DS:[EDI+0x3AC4]
	$+143    008D39A1     50                        PUSH EAX
	$+144    008D39A2     E8 79C8E7FF               CALL PathOfEx.00750220
	$+149    008D39A7     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+150    008D39AE     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+153    008D39B1     85F6                      TEST ESI,ESI
	$+155    008D39B3     74 24                     JE SHORT PathOfEx.008D39D9
	$+157    008D39B5     83CB FF                   OR EBX,0xFFFFFFFF
	$+15A    008D39B8     8BC3                      MOV EAX,EBX
	$+15C    008D39BA     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+161    008D39BF     75 15                     JNZ SHORT PathOfEx.008D39D6
	$+163    008D39C1     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+165    008D39C3     8BCE                      MOV ECX,ESI
	$+167    008D39C5     FF10                      CALL DWORD PTR DS:[EAX]
	$+169    008D39C7     F0:0FC15E 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
	$+16E    008D39CC     4B                        DEC EBX
	$+16F    008D39CD     75 07                     JNZ SHORT PathOfEx.008D39D6
	$+171    008D39CF     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+173    008D39D1     8BCE                      MOV ECX,ESI
	$+175    008D39D3     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$+178    008D39D6     8D5F 08                   LEA EBX,DWORD PTR DS:[EDI+0x8]
	$+17B    008D39D9     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+182    008D39E0     8B8F 943A0000             MOV ECX,DWORD PTR DS:[EDI+0x3A94]
	$+188    008D39E6     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+18A    008D39E8     FF50 14                   CALL DWORD PTR DS:[EAX+0x14]
	$+18D    008D39EB     8BF0                      MOV ESI,EAX
	$+18F    008D39ED     8B4E 2C                   MOV ECX,DWORD PTR DS:[ESI+0x2C]
	$+192    008D39F0     8B56 28                   MOV EDX,DWORD PTR DS:[ESI+0x28]
	$+195    008D39F3     3BD1                      CMP EDX,ECX
	$+197    008D39F5     74 23                     JE SHORT PathOfEx.008D3A1A
	$+199    008D39F7     391A                      CMP DWORD PTR DS:[EDX],EBX
	$+19B    008D39F9     74 07                     JE SHORT PathOfEx.008D3A02
	$+19D    008D39FB     83C2 04                   ADD EDX,0x4
	$+1A0    008D39FE     3BD1                      CMP EDX,ECX
	$+1A2    008D3A00   ^ 75 F5                     JNZ SHORT PathOfEx.008D39F7
	$+1A4    008D3A02     3BD1                      CMP EDX,ECX
	$+1A6    008D3A04     74 14                     JE SHORT PathOfEx.008D3A1A
	$+1A8    008D3A06     8D42 04                   LEA EAX,DWORD PTR DS:[EDX+0x4]
	$+1AB    008D3A09     2BC8                      SUB ECX,EAX
	$+1AD    008D3A0B     51                        PUSH ECX
	$+1AE    008D3A0C     50                        PUSH EAX
	$+1AF    008D3A0D     52                        PUSH EDX
	$+1B0    008D3A0E     E8 6D123C00               CALL PathOfEx.00C94C80
	$+1B5    008D3A13     83C4 0C                   ADD ESP,0xC
	$+1B8    008D3A16     8346 2C FC                ADD DWORD PTR DS:[ESI+0x2C],-0x4
	$+1BC    008D3A1A     8B87 943A0000             MOV EAX,DWORD PTR DS:[EDI+0x3A94]
	$+1C2    008D3A20     33D2                      XOR EDX,EDX
	$+1C4    008D3A22     33DB                      XOR EBX,EBX
	$+1C6    008D3A24     C780 C0010000 00000000    MOV DWORD PTR DS:[EAX+0x1C0],0x0
	$+1D0    008D3A2E     8B87 843F0000             MOV EAX,DWORD PTR DS:[EDI+0x3F84]
	$+1D6    008D3A34     8BC8                      MOV ECX,EAX
	$+1D8    008D3A36     8BB7 803F0000             MOV ESI,DWORD PTR DS:[EDI+0x3F80]
	$+1DE    008D3A3C     2BCE                      SUB ECX,ESI
	$+1E0    008D3A3E     83C1 03                   ADD ECX,0x3
	$+1E3    008D3A41     C1E9 02                   SHR ECX,0x2
	$+1E6    008D3A44     3BF0                      CMP ESI,EAX
	$+1E8    008D3A46     0F47CA                    CMOVA ECX,EDX
	$+1EB    008D3A49     894D F0                   MOV DWORD PTR SS:[EBP-0x10],ECX
	$+1EE    008D3A4C     85C9                      TEST ECX,ECX
	$+1F0    008D3A4E     74 1B                     JE SHORT PathOfEx.008D3A6B
	$+1F2    008D3A50     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+1F4    008D3A52     6A 00                     PUSH 0x0
	$+1F6    008D3A54     68 A0568D00               PUSH PathOfEx.008D56A0
	$+1FB    008D3A59     57                        PUSH EDI
	$+1FC    008D3A5A     83C1 38                   ADD ECX,0x38
	$+1FF    008D3A5D     E8 2EA6BCFF               CALL PathOfEx.0049E090
	$+204    008D3A62     43                        INC EBX
	$+205    008D3A63     8D76 04                   LEA ESI,DWORD PTR DS:[ESI+0x4]
	$+208    008D3A66     3B5D F0                   CMP EBX,DWORD PTR SS:[EBP-0x10]
	$+20B    008D3A69   ^ 75 E5                     JNZ SHORT PathOfEx.008D3A50
	$+20D    008D3A6B     8BB7 983A0000             MOV ESI,DWORD PTR DS:[EDI+0x3A98]
	$+213    008D3A71     85F6                      TEST ESI,ESI
	$+215    008D3A73     74 15                     JE SHORT PathOfEx.008D3A8A
	$+217    008D3A75     8BCE                      MOV ECX,ESI
	$+219    008D3A77     E8 5413ECFF               CALL PathOfEx.00794DD0
	$+21E    008D3A7C     68 E0000000               PUSH 0xE0
	$+223    008D3A81     56                        PUSH ESI
	$+224    008D3A82     E8 818C3A00               CALL PathOfEx.00C7C708
	$+229    008D3A87     83C4 08                   ADD ESP,0x8
	$+22C    008D3A8A     8D8F 00400000             LEA ECX,DWORD PTR DS:[EDI+0x4000]
	$+232    008D3A90     E8 4B8F0000               CALL PathOfEx.008DC9E0
	$+237    008D3A95     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+23E    008D3A9C     8B87 FC3F0000             MOV EAX,DWORD PTR DS:[EDI+0x3FFC]
	$+244    008D3AA2     85C0                      TEST EAX,EAX
	$+246    008D3AA4     74 17                     JE SHORT PathOfEx.008D3ABD
	$+248    008D3AA6     50                        PUSH EAX
	$+249    008D3AA7     E8 5445E4FF               CALL PathOfEx.00718000
	$+24E    008D3AAC     8BC8                      MOV ECX,EAX
	$+250    008D3AAE     E8 DD40E4FF               CALL PathOfEx.00717B90
	$+255    008D3AB3     C787 FC3F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FFC],0x0
	$+25F    008D3ABD     8D8F EC3F0000             LEA ECX,DWORD PTR DS:[EDI+0x3FEC]
	$+265    008D3AC3     E8 288D0000               CALL PathOfEx.008DC7F0
	$+26A    008D3AC8     8B8F D83F0000             MOV ECX,DWORD PTR DS:[EDI+0x3FD8]
	$+270    008D3ACE     85C9                      TEST ECX,ECX
	$+272    008D3AD0     74 35                     JE SHORT PathOfEx.008D3B07
	$+274    008D3AD2     8B87 E03F0000             MOV EAX,DWORD PTR DS:[EDI+0x3FE0]
	$+27A    008D3AD8     2BC1                      SUB EAX,ECX
	$+27C    008D3ADA     C1F8 02                   SAR EAX,0x2
	$+27F    008D3ADD     6A 04                     PUSH 0x4
	$+281    008D3ADF     50                        PUSH EAX
	$+282    008D3AE0     51                        PUSH ECX
	$+283    008D3AE1     E8 4A8DB9FF               CALL PathOfEx.0046C830
	$+288    008D3AE6     C787 D83F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FD8],0x0
	$+292    008D3AF0     83C4 0C                   ADD ESP,0xC
	$+295    008D3AF3     C787 DC3F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FDC],0x0
	$+29F    008D3AFD     C787 E03F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FE0],0x0
	$+2A9    008D3B07     8D8F D03F0000             LEA ECX,DWORD PTR DS:[EDI+0x3FD0]
	$+2AF    008D3B0D     E8 AE0AD4FF               CALL PathOfEx.006145C0
	$+2B4    008D3B12     6A 10                     PUSH 0x10
	$+2B6    008D3B14     6A 01                     PUSH 0x1
	$+2B8    008D3B16     FFB7 D03F0000             PUSH DWORD PTR DS:[EDI+0x3FD0]
	$+2BE    008D3B1C     E8 0F8DB9FF               CALL PathOfEx.0046C830
	$+2C3    008D3B21     8B8F B83F0000             MOV ECX,DWORD PTR DS:[EDI+0x3FB8]
	$+2C9    008D3B27     83C4 0C                   ADD ESP,0xC
	$+2CC    008D3B2A     85C9                      TEST ECX,ECX
	$+2CE    008D3B2C     74 35                     JE SHORT PathOfEx.008D3B63
	$+2D0    008D3B2E     8B87 C03F0000             MOV EAX,DWORD PTR DS:[EDI+0x3FC0]
	$+2D6    008D3B34     2BC1                      SUB EAX,ECX
	$+2D8    008D3B36     C1F8 02                   SAR EAX,0x2
	$+2DB    008D3B39     6A 04                     PUSH 0x4
	$+2DD    008D3B3B     50                        PUSH EAX
	$+2DE    008D3B3C     51                        PUSH ECX
	$+2DF    008D3B3D     E8 EE8CB9FF               CALL PathOfEx.0046C830
	$+2E4    008D3B42     C787 B83F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FB8],0x0
	$+2EE    008D3B4C     83C4 0C                   ADD ESP,0xC
	$+2F1    008D3B4F     C787 BC3F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FBC],0x0
	$+2FB    008D3B59     C787 C03F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FC0],0x0
	$+305    008D3B63     8D8F B03F0000             LEA ECX,DWORD PTR DS:[EDI+0x3FB0]
	$+30B    008D3B69     E8 520AD4FF               CALL PathOfEx.006145C0
	$+310    008D3B6E     6A 10                     PUSH 0x10
	$+312    008D3B70     6A 01                     PUSH 0x1
	$+314    008D3B72     FFB7 B03F0000             PUSH DWORD PTR DS:[EDI+0x3FB0]
	$+31A    008D3B78     E8 B38CB9FF               CALL PathOfEx.0046C830
	$+31F    008D3B7D     8B8F 983F0000             MOV ECX,DWORD PTR DS:[EDI+0x3F98]
	$+325    008D3B83     83C4 0C                   ADD ESP,0xC
	$+328    008D3B86     85C9                      TEST ECX,ECX
	$+32A    008D3B88     74 35                     JE SHORT PathOfEx.008D3BBF
	$+32C    008D3B8A     8B87 A03F0000             MOV EAX,DWORD PTR DS:[EDI+0x3FA0]
	$+332    008D3B90     2BC1                      SUB EAX,ECX
	$+334    008D3B92     C1F8 02                   SAR EAX,0x2
	$+337    008D3B95     6A 04                     PUSH 0x4
	$+339    008D3B97     50                        PUSH EAX
	$+33A    008D3B98     51                        PUSH ECX
	$+33B    008D3B99     E8 928CB9FF               CALL PathOfEx.0046C830
	$+340    008D3B9E     C787 983F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F98],0x0
	$+34A    008D3BA8     83C4 0C                   ADD ESP,0xC
	$+34D    008D3BAB     C787 9C3F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F9C],0x0
	$+357    008D3BB5     C787 A03F0000 00000000    MOV DWORD PTR DS:[EDI+0x3FA0],0x0
	$+361    008D3BBF     8D8F 903F0000             LEA ECX,DWORD PTR DS:[EDI+0x3F90]
	$+367    008D3BC5     E8 F609D4FF               CALL PathOfEx.006145C0
	$+36C    008D3BCA     6A 10                     PUSH 0x10
	$+36E    008D3BCC     6A 01                     PUSH 0x1
	$+370    008D3BCE     FFB7 903F0000             PUSH DWORD PTR DS:[EDI+0x3F90]
	$+376    008D3BD4     E8 578CB9FF               CALL PathOfEx.0046C830
	$+37B    008D3BD9     8B8F 803F0000             MOV ECX,DWORD PTR DS:[EDI+0x3F80]
	$+381    008D3BDF     83C4 0C                   ADD ESP,0xC
	$+384    008D3BE2     85C9                      TEST ECX,ECX
	$+386    008D3BE4     74 35                     JE SHORT PathOfEx.008D3C1B
	$+388    008D3BE6     8B87 883F0000             MOV EAX,DWORD PTR DS:[EDI+0x3F88]
	$+38E    008D3BEC     2BC1                      SUB EAX,ECX
	$+390    008D3BEE     C1F8 02                   SAR EAX,0x2
	$+393    008D3BF1     6A 04                     PUSH 0x4
	$+395    008D3BF3     50                        PUSH EAX
	$+396    008D3BF4     51                        PUSH ECX
	$+397    008D3BF5     E8 368CB9FF               CALL PathOfEx.0046C830
	$+39C    008D3BFA     C787 803F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F80],0x0
	$+3A6    008D3C04     83C4 0C                   ADD ESP,0xC
	$+3A9    008D3C07     C787 843F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F84],0x0
	$+3B3    008D3C11     C787 883F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F88],0x0
	$+3BD    008D3C1B     8B8F 6C3F0000             MOV ECX,DWORD PTR DS:[EDI+0x3F6C]
	$+3C3    008D3C21     85C9                      TEST ECX,ECX
	$+3C5    008D3C23     74 35                     JE SHORT PathOfEx.008D3C5A
	$+3C7    008D3C25     8B87 743F0000             MOV EAX,DWORD PTR DS:[EDI+0x3F74]
	$+3CD    008D3C2B     2BC1                      SUB EAX,ECX
	$+3CF    008D3C2D     C1F8 03                   SAR EAX,0x3
	$+3D2    008D3C30     6A 08                     PUSH 0x8
	$+3D4    008D3C32     50                        PUSH EAX
	$+3D5    008D3C33     51                        PUSH ECX
	$+3D6    008D3C34     E8 F78BB9FF               CALL PathOfEx.0046C830
	$+3DB    008D3C39     C787 6C3F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F6C],0x0
	$+3E5    008D3C43     83C4 0C                   ADD ESP,0xC
	$+3E8    008D3C46     C787 703F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F70],0x0
	$+3F2    008D3C50     C787 743F0000 00000000    MOV DWORD PTR DS:[EDI+0x3F74],0x0
	$+3FC    008D3C5A     83BF F03E0000 08          CMP DWORD PTR DS:[EDI+0x3EF0],0x8
	$+403    008D3C61     8DB7 DC3E0000             LEA ESI,DWORD PTR DS:[EDI+0x3EDC]
	$+409    008D3C67     72 11                     JB SHORT PathOfEx.008D3C7A
	$+40B    008D3C69     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+40E    008D3C6C     40                        INC EAX
	$+40F    008D3C6D     6A 02                     PUSH 0x2
	$+411    008D3C6F     50                        PUSH EAX
	$+412    008D3C70     FF36                      PUSH DWORD PTR DS:[ESI]
	$+414    008D3C72     E8 B98BB9FF               CALL PathOfEx.0046C830
	*/
	addr_tmp = scan_exe_.ScanAddr("6a??e8????????8b????c7????????????85??74??8b????2b??6a??c1????5051e8????????83????c7????????????c7????????????c7????????????8b????c7????????????85??74??8b????2b??6a??c1????5051e8????????83????8b", 0x60);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_before_init_game_map_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-39A    006CBF5F     CC                        INT3
	$-399    006CBF60     51                        PUSH ECX
	$-398    006CBF61     57                        PUSH EDI
	$-397    006CBF62     8BF9                      MOV EDI,ECX
	$-395    006CBF64     8B4F 04                   MOV ECX,DWORD PTR DS:[EDI+0x4]
	$-392    006CBF67     85C9                      TEST ECX,ECX
	$-390    006CBF69     74 46                     JE SHORT PathOfEx.006CBFB1
	$-38E    006CBF6B     FF7424 04                 PUSH DWORD PTR SS:[ESP+0x4]
	$-38A    006CBF6F     8B57 08                   MOV EDX,DWORD PTR DS:[EDI+0x8]
	$-387    006CBF72     51                        PUSH ECX
	$-386    006CBF73     E8 082BDDFF               CALL PathOfEx.0049EA80
	$-381    006CBF78     8B4F 0C                   MOV ECX,DWORD PTR DS:[EDI+0xC]
	$-37E    006CBF7B     B8 ABAAAA2A               MOV EAX,0x2AAAAAAB
	$-379    006CBF80     2B4F 04                   SUB ECX,DWORD PTR DS:[EDI+0x4]
	$-376    006CBF83     F7E9                      IMUL ECX
	$-374    006CBF85     6A 0C                     PUSH 0xC
	$-372    006CBF87     D1FA                      SAR EDX,1
	$-370    006CBF89     8BC2                      MOV EAX,EDX
	$-36E    006CBF8B     C1E8 1F                   SHR EAX,0x1F
	$-36B    006CBF8E     03C2                      ADD EAX,EDX
	$-369    006CBF90     50                        PUSH EAX
	$-368    006CBF91     FF77 04                   PUSH DWORD PTR DS:[EDI+0x4]
	$-365    006CBF94     E8 9708DAFF               CALL PathOfEx.0046C830
	$-360    006CBF99     C747 04 00000000          MOV DWORD PTR DS:[EDI+0x4],0x0
	$-359    006CBFA0     83C4 14                   ADD ESP,0x14
	$-356    006CBFA3     C747 08 00000000          MOV DWORD PTR DS:[EDI+0x8],0x0
	$-34F    006CBFAA     C747 0C 00000000          MOV DWORD PTR DS:[EDI+0xC],0x0
	$-348    006CBFB1     5F                        POP EDI
	$-347    006CBFB2     59                        POP ECX
	$-346    006CBFB3     C3                        RETN
	$-345    006CBFB4     CC                        INT3
	$-344    006CBFB5     CC                        INT3
	$-343    006CBFB6     CC                        INT3
	$-342    006CBFB7     CC                        INT3
	$-341    006CBFB8     CC                        INT3
	$-340    006CBFB9     CC                        INT3
	$-33F    006CBFBA     CC                        INT3
	$-33E    006CBFBB     CC                        INT3
	$-33D    006CBFBC     CC                        INT3
	$-33C    006CBFBD     CC                        INT3
	$-33B    006CBFBE     CC                        INT3
	$-33A    006CBFBF     CC                        INT3
	$-339    006CBFC0     55                        PUSH EBP
	$-338    006CBFC1     8BEC                      MOV EBP,ESP
	$-336    006CBFC3     6A FF                     PUSH -0x1
	$-334    006CBFC5     68 F248D400               PUSH PathOfEx.00D448F2
	$-32F    006CBFCA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-329    006CBFD0     50                        PUSH EAX
	$-328    006CBFD1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-321    006CBFD8     81EC 8C020000             SUB ESP,0x28C
	$-31B    006CBFDE     56                        PUSH ESI
	$-31A    006CBFDF     57                        PUSH EDI
	$-319    006CBFE0     8BF9                      MOV EDI,ECX
	$-317    006CBFE2     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$-310    006CBFE9     6A 00                     PUSH 0x0
	$-30E    006CBFEB     6A 00                     PUSH 0x0
	$-30C    006CBFED     897D BC                   MOV DWORD PTR SS:[EBP-0x44],EDI
	$-309    006CBFF0     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$-303    006CBFF6     FF87 A81A0000             INC DWORD PTR DS:[EDI+0x1AA8]
	$-2FD    006CBFFC     C787 BC180000 00000000    MOV DWORD PTR DS:[EDI+0x18BC],0x0
	$-2F3    006CC006     C787 D0180000 00000000    MOV DWORD PTR DS:[EDI+0x18D0],0x0
	$-2E9    006CC010     C787 AC1A0000 00000000    MOV DWORD PTR DS:[EDI+0x1AAC],0x0
	$-2DF    006CC01A     C687 D61A0000 00          MOV BYTE PTR DS:[EDI+0x1AD6],0x0
	$-2D8    006CC021     E8 AA01FBFF               CALL PathOfEx.0067C1D0
	$-2D3    006CC026     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$-2CD    006CC02C     6A 00                     PUSH 0x0
	$-2CB    006CC02E     E8 1D96FBFF               CALL PathOfEx.00685650
	$-2C6    006CC033     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-2C3    006CC036     E8 2514DBFF               CALL PathOfEx.0047D460
	$-2BE    006CC03B     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-2B7    006CC042     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$-2B5    006CC044     8975 C8                   MOV DWORD PTR SS:[EBP-0x38],ESI
	$-2B2    006CC047     8B86 10010000             MOV EAX,DWORD PTR DS:[ESI+0x110]
	$-2AC    006CC04D     85C0                      TEST EAX,EAX
	$-2AA    006CC04F     74 1C                     JE SHORT PathOfEx.006CC06D
	$-2A8    006CC051     8B80 943A0000             MOV EAX,DWORD PTR DS:[EAX+0x3A94]
	$-2A2    006CC057     8B88 C4010000             MOV ECX,DWORD PTR DS:[EAX+0x1C4]
	$-29C    006CC05D     E8 6E66DCFF               CALL PathOfEx.004926D0
	$-297    006CC062     8D4E 04                   LEA ECX,DWORD PTR DS:[ESI+0x4]
	$-294    006CC065     51                        PUSH ECX
	$-293    006CC066     8BC8                      MOV ECX,EAX
	$-291    006CC068     E8 7371DDFF               CALL PathOfEx.004A31E0
	$-28C    006CC06D     8B45 C8                   MOV EAX,DWORD PTR SS:[EBP-0x38]
	$-289    006CC070     C786 10010000 00000000    MOV DWORD PTR DS:[ESI+0x110],0x0
	$-27F    006CC07A     8BB6 18010000             MOV ESI,DWORD PTR DS:[ESI+0x118]
	$-279    006CC080     8B80 14010000             MOV EAX,DWORD PTR DS:[EAX+0x114]
	$-273    006CC086     3BC6                      CMP EAX,ESI
	$-271    006CC088     74 19                     JE SHORT PathOfEx.006CC0A3
	$-26F    006CC08A     8BF8                      MOV EDI,EAX
	$-26D    006CC08C     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$-269    006CC090     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
	$-267    006CC092     6A 00                     PUSH 0x0
	$-265    006CC094     E8 0752FDFF               CALL PathOfEx.006A12A0
	$-260    006CC099     83C7 20                   ADD EDI,0x20
	$-25D    006CC09C     3BFE                      CMP EDI,ESI
	$-25B    006CC09E   ^ 75 F0                     JNZ SHORT PathOfEx.006CC090
	$-259    006CC0A0     8B7D BC                   MOV EDI,DWORD PTR SS:[EBP-0x44]
	$-256    006CC0A3     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$-24F    006CC0AA     83CE FF                   OR ESI,0xFFFFFFFF
	$-24C    006CC0AD     8B4D E8                   MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-249    006CC0B0     894D C8                   MOV DWORD PTR SS:[EBP-0x38],ECX
	$-246    006CC0B3     85C9                      TEST ECX,ECX
	$-244    006CC0B5     74 1E                     JE SHORT PathOfEx.006CC0D5
	$-242    006CC0B7     8BC6                      MOV EAX,ESI
	$-240    006CC0B9     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$-23B    006CC0BE     75 15                     JNZ SHORT PathOfEx.006CC0D5
	$-239    006CC0C0     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-237    006CC0C2     FF10                      CALL DWORD PTR DS:[EAX]
	$-235    006CC0C4     8B4D C8                   MOV ECX,DWORD PTR SS:[EBP-0x38]
	$-232    006CC0C7     8BC6                      MOV EAX,ESI
	$-230    006CC0C9     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-22B    006CC0CE     75 05                     JNZ SHORT PathOfEx.006CC0D5
	$-229    006CC0D0     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-227    006CC0D2     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-224    006CC0D5     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-21D    006CC0DC     8B87 7C0C0000             MOV EAX,DWORD PTR DS:[EDI+0xC7C]
	$-217    006CC0E2     8945 C8                   MOV DWORD PTR SS:[EBP-0x38],EAX
	$-214    006CC0E5     C787 7C0C0000 00000000    MOV DWORD PTR DS:[EDI+0xC7C],0x0
	$-20A    006CC0EF     85C0                      TEST EAX,EAX
	$-208    006CC0F1     74 17                     JE SHORT PathOfEx.006CC10A
	$-206    006CC0F3     8BC8                      MOV ECX,EAX
	$-204    006CC0F5     E8 36A62500               CALL PathOfEx.00926730
	$-1FF    006CC0FA     68 9C000000               PUSH 0x9C
	$-1FA    006CC0FF     FF75 C8                   PUSH DWORD PTR SS:[EBP-0x38]
	$-1F7    006CC102     E8 01065B00               CALL PathOfEx.00C7C708
	$-1F2    006CC107     83C4 08                   ADD ESP,0x8
	$-1EF    006CC10A     8B87 D81A0000             MOV EAX,DWORD PTR DS:[EDI+0x1AD8]
	$-1E9    006CC110     8945 C8                   MOV DWORD PTR SS:[EBP-0x38],EAX
	$-1E6    006CC113     C787 D81A0000 00000000    MOV DWORD PTR DS:[EDI+0x1AD8],0x0
	$-1DC    006CC11D     85C0                      TEST EAX,EAX
	$-1DA    006CC11F     74 14                     JE SHORT PathOfEx.006CC135
	$-1D8    006CC121     8BC8                      MOV ECX,EAX
	$-1D6    006CC123     E8 281CE0FF               CALL PathOfEx.004CDD50
	$-1D1    006CC128     6A 28                     PUSH 0x28
	$-1CF    006CC12A     FF75 C8                   PUSH DWORD PTR SS:[EBP-0x38]
	$-1CC    006CC12D     E8 D6055B00               CALL PathOfEx.00C7C708
	$-1C7    006CC132     83C4 08                   ADD ESP,0x8
	$-1C4    006CC135     8B8F 98150000             MOV ECX,DWORD PTR DS:[EDI+0x1598]
	$-1BE    006CC13B     6A 00                     PUSH 0x0
	$-1BC    006CC13D     83C1 30                   ADD ECX,0x30
	$-1B9    006CC140     E8 7B092E00               CALL PathOfEx.009ACAC0
	$-1B4    006CC145     8B8F 98150000             MOV ECX,DWORD PTR DS:[EDI+0x1598]
	$-1AE    006CC14B     6A 00                     PUSH 0x0
	$-1AC    006CC14D     83C1 30                   ADD ECX,0x30
	$-1A9    006CC150     E8 1B0B2E00               CALL PathOfEx.009ACC70
	$-1A4    006CC155     8BCF                      MOV ECX,EDI
	$-1A2    006CC157     E8 F46B0000               CALL PathOfEx.006D2D50
	$-19D    006CC15C     8D45 C8                   LEA EAX,DWORD PTR SS:[EBP-0x38]
	$-19A    006CC15F     C745 C8 00000000          MOV DWORD PTR SS:[EBP-0x38],0x0
	$-193    006CC166     50                        PUSH EAX
	$-192    006CC167     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-18F    006CC16A     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
	$-188    006CC171     50                        PUSH EAX
	$-187    006CC172     8D55 F0                   LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-184    006CC175     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-17D    006CC17C     8D4D AC                   LEA ECX,DWORD PTR SS:[EBP-0x54]
	$-17A    006CC17F     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18],0x0
	$-173    006CC186     E8 C5900000               CALL PathOfEx.006D5250
	$-16E    006CC18B     8BD0                      MOV EDX,EAX
	$-16C    006CC18D     83C4 08                   ADD ESP,0x8
	$-169    006CC190     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$-166    006CC193     8B4A 0C                   MOV ECX,DWORD PTR DS:[EDX+0xC]
	$-163    006CC196     898F 7C150000             MOV DWORD PTR DS:[EDI+0x157C],ECX
	$-15D    006CC19C     8B4A 04                   MOV ECX,DWORD PTR DS:[EDX+0x4]
	$-15A    006CC19F     8B42 08                   MOV EAX,DWORD PTR DS:[EDX+0x8]
	$-157    006CC1A2     C742 04 00000000          MOV DWORD PTR DS:[EDX+0x4],0x0
	$-150    006CC1A9     C742 08 00000000          MOV DWORD PTR DS:[EDX+0x8],0x0
	$-149    006CC1B0     8B97 78150000             MOV EDX,DWORD PTR DS:[EDI+0x1578]
	$-143    006CC1B6     8987 78150000             MOV DWORD PTR DS:[EDI+0x1578],EAX
	$-13D    006CC1BC     8955 C8                   MOV DWORD PTR SS:[EBP-0x38],EDX
	$-13A    006CC1BF     898F 74150000             MOV DWORD PTR DS:[EDI+0x1574],ECX
	$-134    006CC1C5     8BCA                      MOV ECX,EDX
	$-132    006CC1C7     C745 FC 02000000          MOV DWORD PTR SS:[EBP-0x4],0x2
	$-12B    006CC1CE     8B55 EC                   MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-128    006CC1D1     85C9                      TEST ECX,ECX
	$-126    006CC1D3     74 11                     JE SHORT PathOfEx.006CC1E6
	$-124    006CC1D5     8BC6                      MOV EAX,ESI
	$-122    006CC1D7     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-11D    006CC1DC     75 08                     JNZ SHORT PathOfEx.006CC1E6
	$-11B    006CC1DE     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-119    006CC1E0     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-116    006CC1E3     8B55 EC                   MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-113    006CC1E6     8B02                      MOV EAX,DWORD PTR DS:[EDX]
	$-111    006CC1E8     8987 70150000             MOV DWORD PTR DS:[EDI+0x1570],EAX
	$-10B    006CC1EE     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4],0x3
	$-104    006CC1F5     8B4D B4                   MOV ECX,DWORD PTR SS:[EBP-0x4C]
	$-101    006CC1F8     85C9                      TEST ECX,ECX
	$-FF     006CC1FA     74 0E                     JE SHORT PathOfEx.006CC20A
	$-FD     006CC1FC     8BC6                      MOV EAX,ESI
	$-FB     006CC1FE     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-F6     006CC203     75 05                     JNZ SHORT PathOfEx.006CC20A
	$-F4     006CC205     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-F2     006CC207     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-EF     006CC20A     C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4],0x4
	$-E8     006CC211     8B4D E8                   MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-E5     006CC214     85C9                      TEST ECX,ECX
	$-E3     006CC216     74 0E                     JE SHORT PathOfEx.006CC226
	$-E1     006CC218     8BC6                      MOV EAX,ESI
	$-DF     006CC21A     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-DA     006CC21F     75 05                     JNZ SHORT PathOfEx.006CC226
	$-D8     006CC221     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-D6     006CC223     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-D3     006CC226     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-CC     006CC22D     C787 88150000 00000000    MOV DWORD PTR DS:[EDI+0x1588],0x0
	$-C2     006CC237     83BF 38010000 00          CMP DWORD PTR DS:[EDI+0x138],0x0
	$-BB     006CC23E     74 31                     JE SHORT PathOfEx.006CC271
	$-B9     006CC240     8B87 38010000             MOV EAX,DWORD PTR DS:[EDI+0x138]
	$-B3     006CC246     8B48 08                   MOV ECX,DWORD PTR DS:[EAX+0x8]
	$-B0     006CC249     8A41 0C                   MOV AL,BYTE PTR DS:[ECX+0xC]
	$-AD     006CC24C     84C0                      TEST AL,AL
	$-AB     006CC24E     75 14                     JNZ SHORT PathOfEx.006CC264
	$-A9     006CC250     8A81 DE000000             MOV AL,BYTE PTR DS:[ECX+0xDE]
	$-A3     006CC256     84C0                      TEST AL,AL
	$-A1     006CC258     75 0A                     JNZ SHORT PathOfEx.006CC264
	$-9F     006CC25A     33C0                      XOR EAX,EAX
	$-9D     006CC25C     84C0                      TEST AL,AL
	$-9B     006CC25E     0F9445 C8                 SETE BYTE PTR SS:[EBP-0x38]
	$-97     006CC262     EB 11                     JMP SHORT PathOfEx.006CC275
	$-95     006CC264     B8 01000000               MOV EAX,0x1
	$-90     006CC269     84C0                      TEST AL,AL
	$-8E     006CC26B     0F9445 C8                 SETE BYTE PTR SS:[EBP-0x38]
	$-8A     006CC26F     EB 04                     JMP SHORT PathOfEx.006CC275
	$-88     006CC271     C645 C8 00                MOV BYTE PTR SS:[EBP-0x38],0x0
	$-84     006CC275     83BF 3C010000 00          CMP DWORD PTR DS:[EDI+0x13C],0x0
	$-7D     006CC27C     74 2A                     JE SHORT PathOfEx.006CC2A8
	$-7B     006CC27E     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$-75     006CC284     8B8F 44010000             MOV ECX,DWORD PTR DS:[EDI+0x144]
	$-6F     006CC28A     0F1080 483F0000           MOVUPS XMM0,DQWORD PTR DS:[EAX+0x3F48]
	$-68     006CC291     0F1181 483F0000           MOVUPS DQWORD PTR DS:[ECX+0x3F48],XMM0
	$-61     006CC298     F30F7E80 583F0000         MOVQ XMM0,QWORD PTR DS:[EAX+0x3F58]
	$-59     006CC2A0     660FD681 583F0000         MOVQ QWORD PTR DS:[ECX+0x3F58],XMM0
	$-51     006CC2A8     8B8F 44010000             MOV ECX,DWORD PTR DS:[EDI+0x144]
	$-4B     006CC2AE     6A 00                     PUSH 0x0
	$-49     006CC2B0     68 A0BD6C00               PUSH PathOfEx.006CBDA0
	$-44     006CC2B5     57                        PUSH EDI                                          ; edi=stCD_SceneInGameState*
	$-43     006CC2B6     81C1 14030000             ADD ECX,0x314
	$-3D     006CC2BC     E8 CF1DDDFF               CALL PathOfEx.0049E090
	$-38     006CC2C1     8B87 44010000             MOV EAX,DWORD PTR DS:[EDI+0x144]                  ; then,eax=stCD_GameLogic*
	$-32     006CC2C7     C787 44010000 00000000    MOV DWORD PTR DS:[EDI+0x144],0x0
	$-28     006CC2D1     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$-22     006CC2D7     894D F0                   MOV DWORD PTR SS:[EBP-0x10],ECX
	$-1F     006CC2DA     8987 3C010000             MOV DWORD PTR DS:[EDI+0x13C],EAX                  ; 设置stCD_GameLogic*
	$-19     006CC2E0     85C9                      TEST ECX,ECX
	$-17     006CC2E2     74 15                     JE SHORT PathOfEx.006CC2F9
	$-15     006CC2E4     E8 F7752000               CALL PathOfEx.008D38E0
	$-10     006CC2E9     68 18400000               PUSH 0x4018
	$-B      006CC2EE     FF75 F0                   PUSH DWORD PTR SS:[EBP-0x10]
	$-8      006CC2F1     E8 12045B00               CALL PathOfEx.00C7C708
	$-3      006CC2F6     83C4 08                   ADD ESP,0x8
	$ ==>    006CC2F9     8B87 40010000             MOV EAX,DWORD PTR DS:[EDI+0x140]                  ; g_h_after_init_game_map_
	$+6      006CC2FF     C787 40010000 00000000    MOV DWORD PTR DS:[EDI+0x140],0x0
	$+10     006CC309     8B8F 38010000             MOV ECX,DWORD PTR DS:[EDI+0x138]
	$+16     006CC30F     8987 38010000             MOV DWORD PTR DS:[EDI+0x138],EAX
	$+1C     006CC315     85C9                      TEST ECX,ECX
	$+1E     006CC317     74 15                     JE SHORT PathOfEx.006CC32E
	$+20     006CC319     C745 FC 05000000          MOV DWORD PTR SS:[EBP-0x4],0x5
	$+27     006CC320     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+29     006CC322     6A 01                     PUSH 0x1
	$+2B     006CC324     FF50 1C                   CALL DWORD PTR DS:[EAX+0x1C]
	$+2E     006CC327     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+35     006CC32E     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+38     006CC331     E8 2A11DBFF               CALL PathOfEx.0047D460
	$+3D     006CC336     C745 FC 06000000          MOV DWORD PTR SS:[EBP-0x4],0x6
	$+44     006CC33D     FFB7 3C010000             PUSH DWORD PTR DS:[EDI+0x13C]
	$+4A     006CC343     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+4C     006CC345     E8 B65CFDFF               CALL PathOfEx.006A2000
	$+51     006CC34A     C745 FC 07000000          MOV DWORD PTR SS:[EBP-0x4],0x7
	$+58     006CC351     8B4D E8                   MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+5B     006CC354     894D F0                   MOV DWORD PTR SS:[EBP-0x10],ECX
	$+5E     006CC357     85C9                      TEST ECX,ECX
	$+60     006CC359     74 1E                     JE SHORT PathOfEx.006CC379
	$+62     006CC35B     8BC6                      MOV EAX,ESI
	$+64     006CC35D     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$+69     006CC362     75 15                     JNZ SHORT PathOfEx.006CC379
	$+6B     006CC364     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+6D     006CC366     FF10                      CALL DWORD PTR DS:[EAX]
	$+6F     006CC368     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+72     006CC36B     8BC6                      MOV EAX,ESI
	$+74     006CC36D     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$+79     006CC372     75 05                     JNZ SHORT PathOfEx.006CC379
	$+7B     006CC374     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+7D     006CC376     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$+80     006CC379     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+87     006CC380     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+8A     006CC383     8B8F 38010000             MOV ECX,DWORD PTR DS:[EDI+0x138]
	$+90     006CC389     50                        PUSH EAX
	$+91     006CC38A     81C1 B0000000             ADD ECX,0xB0
	$+97     006CC390     C745 E4 F0AFEA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAAFF0
	$+9E     006CC397     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+A1     006CC39A     E8 A1A20000               CALL PathOfEx.006D6640
	$+A6     006CC39F     8D47 20                   LEA EAX,DWORD PTR DS:[EDI+0x20]
	$+A9     006CC3A2     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+AC     006CC3A5     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+B2     006CC3AB     8B88 943A0000             MOV ECX,DWORD PTR DS:[EAX+0x3A94]
	$+B8     006CC3B1     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+BA     006CC3B3     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+BD     006CC3B6     FFD0                      CALL EAX
	$+BF     006CC3B8     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+C2     006CC3BB     51                        PUSH ECX
	$+C3     006CC3BC     8D48 28                   LEA ECX,DWORD PTR DS:[EAX+0x28]
	$+C6     006CC3BF     E8 3C54DDFF               CALL PathOfEx.004A1800
	$+CB     006CC3C4     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+D1     006CC3CA     57                        PUSH EDI
	$+D2     006CC3CB     8B88 943A0000             MOV ECX,DWORD PTR DS:[EAX+0x3A94]
	$+D8     006CC3D1     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+DA     006CC3D3     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+DD     006CC3D6     FFD0                      CALL EAX
	$+DF     006CC3D8     8BC8                      MOV ECX,EAX
	$+E1     006CC3DA     E8 F1910000               CALL PathOfEx.006D55D0
	$+E6     006CC3DF     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+EC     006CC3E5     57                        PUSH EDI
	$+ED     006CC3E6     8B88 943A0000             MOV ECX,DWORD PTR DS:[EAX+0x3A94]
	$+F3     006CC3EC     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+F5     006CC3EE     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+F8     006CC3F1     FFD0                      CALL EAX
	$+FA     006CC3F3     8BC8                      MOV ECX,EAX
	$+FC     006CC3F5     E8 16920000               CALL PathOfEx.006D5610
	$+101    006CC3FA     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+107    006CC400     8D45 AC                   LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+10A    006CC403     50                        PUSH EAX
	$+10B    006CC404     81C1 14010000             ADD ECX,0x114
	$+111    006CC40A     C745 AC 9880E900          MOV DWORD PTR SS:[EBP-0x54],PathOfEx.00E98098
	$+118    006CC411     897D B0                   MOV DWORD PTR SS:[EBP-0x50],EDI
	$+11B    006CC414     C745 B4 60B56C00          MOV DWORD PTR SS:[EBP-0x4C],PathOfEx.006CB560
	$+122    006CC41B     C745 B8 00000000          MOV DWORD PTR SS:[EBP-0x48],0x0
	$+129    006CC422     E8 9923DDFF               CALL PathOfEx.0049E7C0
	$+12E    006CC427     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+134    006CC42D     8D45 AC                   LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+137    006CC430     50                        PUSH EAX
	$+138    006CC431     81C1 94010000             ADD ECX,0x194
	$+13E    006CC437     C745 AC 3C4BE700          MOV DWORD PTR SS:[EBP-0x54],PathOfEx.00E74B3C
	$+145    006CC43E     897D B0                   MOV DWORD PTR SS:[EBP-0x50],EDI
	$+148    006CC441     C745 B4 20BB6C00          MOV DWORD PTR SS:[EBP-0x4C],PathOfEx.006CBB20
	$+14F    006CC448     C745 B8 00000000          MOV DWORD PTR SS:[EBP-0x48],0x0
	$+156    006CC44F     E8 0CCDDEFF               CALL PathOfEx.004B9160
	$+15B    006CC454     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+161    006CC45A     8D45 AC                   LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+164    006CC45D     50                        PUSH EAX
	$+165    006CC45E     81C1 14060000             ADD ECX,0x614
	$+16B    006CC464     C745 AC 1CA5EA00          MOV DWORD PTR SS:[EBP-0x54],PathOfEx.00EAA51C
	$+172    006CC46B     897D B0                   MOV DWORD PTR SS:[EBP-0x50],EDI
	$+175    006CC46E     C745 B4 90EF6C00          MOV DWORD PTR SS:[EBP-0x4C],PathOfEx.006CEF90
	$+17C    006CC475     C745 B8 00000000          MOV DWORD PTR SS:[EBP-0x48],0x0
	$+183    006CC47C     E8 9F67FFFF               CALL PathOfEx.006C2C20
	$+188    006CC481     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+18E    006CC487     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+191    006CC48A     50                        PUSH EAX
	$+192    006CC48B     81C1 94310000             ADD ECX,0x3194
	$+198    006CC491     C745 E4 FCAFEA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAAFFC
	$+19F    006CC498     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+1A2    006CC49B     E8 20A20000               CALL PathOfEx.006D66C0
	$+1A7    006CC4A0     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+1AD    006CC4A6     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+1B0    006CC4A9     50                        PUSH EAX
	$+1B1    006CC4AA     81C1 14140000             ADD ECX,0x1414
	$+1B7    006CC4B0     C745 E4 2CB0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB02C
	$+1BE    006CC4B7     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+1C1    006CC4BA     E8 81A20000               CALL PathOfEx.006D6740
	$+1C6    006CC4BF     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+1CC    006CC4C5     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+1CF    006CC4C8     C745 E4 20B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB020
	$+1D6    006CC4CF     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+1D9    006CC4D2     50                        PUSH EAX
	$+1DA    006CC4D3     81C1 94140000             ADD ECX,0x1494
	$+1E0    006CC4D9     E8 E2A20000               CALL PathOfEx.006D67C0
	$+1E5    006CC4DE     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+1EB    006CC4E4     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+1EE    006CC4E7     50                        PUSH EAX
	$+1EF    006CC4E8     81C1 94240000             ADD ECX,0x2494
	$+1F5    006CC4EE     C745 E4 14B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB014
	$+1FC    006CC4F5     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+1FF    006CC4F8     E8 43A30000               CALL PathOfEx.006D6840
	$+204    006CC4FD     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+20A    006CC503     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+20D    006CC506     50                        PUSH EAX
	$+20E    006CC507     81C1 14090000             ADD ECX,0x914
	$+214    006CC50D     C745 E4 08B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB008
	$+21B    006CC514     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+21E    006CC517     E8 A4A30000               CALL PathOfEx.006D68C0
	$+223    006CC51C     C787 F0180000 FFFFFFFF    MOV DWORD PTR DS:[EDI+0x18F0],-0x1
	$+22D    006CC526     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+230    006CC529     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+236    006CC52F     50                        PUSH EAX
	$+237    006CC530     81C1 14210000             ADD ECX,0x2114
	$+23D    006CC536     C745 E4 50B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB050
	$+244    006CC53D     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+247    006CC540     E8 FBA30000               CALL PathOfEx.006D6940
	$+24C    006CC545     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+252    006CC54B     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+255    006CC54E     50                        PUSH EAX
	$+256    006CC54F     81C1 94210000             ADD ECX,0x2194
	$+25C    006CC555     C745 E4 44B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB044
	$+263    006CC55C     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+266    006CC55F     E8 5CA40000               CALL PathOfEx.006D69C0
	$+26B    006CC564     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+271    006CC56A     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+274    006CC56D     50                        PUSH EAX
	$+275    006CC56E     81C1 14150000             ADD ECX,0x1514
	$+27B    006CC574     C745 E4 38B0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB038
	$+282    006CC57B     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+285    006CC57E     E8 BDA40000               CALL PathOfEx.006D6A40
	$+28A    006CC583     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+290    006CC589     8D87 800C0000             LEA EAX,DWORD PTR DS:[EDI+0xC80]
	$+296    006CC58F     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+299    006CC592     81C1 94160000             ADD ECX,0x1694
	$+29F    006CC598     8D45 E0                   LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+2A2    006CC59B     C745 E0 3CD8D900          MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00D9D83C
	$+2A9    006CC5A2     50                        PUSH EAX
	$+2AA    006CC5A3     C745 E8 E04D6700          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00674DE0
	$+2B1    006CC5AA     E8 7164DDFF               CALL PathOfEx.004A2A20
	$+2B6    006CC5AF     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+2BC    006CC5B5     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+2BF    006CC5B8     50                        PUSH EAX
	$+2C0    006CC5B9     81C1 94330000             ADD ECX,0x3394
	$+2C6    006CC5BF     C745 E4 5CB0EA00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EAB05C
	$+2CD    006CC5C6     897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$+2D0    006CC5C9     E8 F2A40000               CALL PathOfEx.006D6AC0
	$+2D5    006CC5CE     C787 780C0000 00000000    MOV DWORD PTR DS:[EDI+0xC78],0x0
	$+2DF    006CC5D8     8B87 38010000             MOV EAX,DWORD PTR DS:[EDI+0x138]
	$+2E5    006CC5DE     8BB0 C4010000             MOV ESI,DWORD PTR DS:[EAX+0x1C4]
	$+2EB    006CC5E4     8975 C0                   MOV DWORD PTR SS:[EBP-0x40],ESI
	$+2EE    006CC5E7     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+2F0    006CC5E9     85C9                      TEST ECX,ECX
	$+2F2    006CC5EB     74 05                     JE SHORT PathOfEx.006CC5F2
	$+2F4    006CC5ED     E8 CECF0400               CALL PathOfEx.007195C0
	$+2F9    006CC5F2     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+2FB    006CC5F4     C745 F0 6CB4EE00          MOV DWORD PTR SS:[EBP-0x10],PathOfEx.00EEB46C     ; ASCII "Player"
	$+302    006CC5FB     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+305    006CC5FE     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+308    006CC601     50                        PUSH EAX
	$+309    006CC602     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+30C    006CC605     50                        PUSH EAX
	$+30D    006CC606     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+310    006CC609     E8 32FAD9FF               CALL PathOfEx.0046C040
	$+315    006CC60E     8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+318    006CC611     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+31B    006CC614     74 16                     JE SHORT PathOfEx.006CC62C
	$+31D    006CC616     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+320    006CC619     83F9 FF                   CMP ECX,-0x1
	$+323    006CC61C     74 0E                     JE SHORT PathOfEx.006CC62C
	$+325    006CC61E     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+328    006CC621     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+32B    006CC624     8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
	$+32E    006CC627     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+331    006CC62A     EB 0A                     JMP SHORT PathOfEx.006CC636
	$+333    006CC62C     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+336    006CC62F     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$+33D    006CC636     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+33F    006CC638     85C9                      TEST ECX,ECX
	$+341    006CC63A     74 05                     JE SHORT PathOfEx.006CC641
	$+343    006CC63C     E8 7FCF0400               CALL PathOfEx.007195C0
	$+348    006CC641     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+34A    006CC643     C745 F0 303FE700          MOV DWORD PTR SS:[EBP-0x10],PathOfEx.00E73F30     ; ASCII "Life"
	$+351    006CC64A     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+354    006CC64D     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+357    006CC650     50                        PUSH EAX
	$+358    006CC651     8D45 D0                   LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+35B    006CC654     50                        PUSH EAX
	$+35C    006CC655     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+35F    006CC658     E8 E3F9D9FF               CALL PathOfEx.0046C040
	$+364    006CC65D     8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+367    006CC660     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+36A    006CC663     74 13                     JE SHORT PathOfEx.006CC678
	$+36C    006CC665     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+36F    006CC668     83F9 FF                   CMP ECX,-0x1
	$+372    006CC66B     74 0B                     JE SHORT PathOfEx.006CC678
	$+374    006CC66D     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+377    006CC670     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+37A    006CC673     8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
	$+37D    006CC676     EB 05                     JMP SHORT PathOfEx.006CC67D
	$+37F    006CC678     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+382    006CC67B     33C0                      XOR EAX,EAX
	$+384    006CC67D     8D4F 24                   LEA ECX,DWORD PTR DS:[EDI+0x24]
	$+387    006CC680     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+38A    006CC683     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+38D    006CC686     894D D0                   MOV DWORD PTR SS:[EBP-0x30],ECX
	$+390    006CC689     8D4D D0                   LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+393    006CC68C     51                        PUSH ECX
	$+394    006CC68D     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+397    006CC690     FF70 04                   PUSH DWORD PTR DS:[EAX+0x4]
	$+39A    006CC693     50                        PUSH EAX
	$+39B    006CC694     E8 2711E6FF               CALL PathOfEx.0052D7C0
	$+3A0    006CC699     8BD0                      MOV EDX,EAX
	$+3A2    006CC69B     B9 54555515               MOV ECX,0x15555554
	$+3A7    006CC6A0     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+3AA    006CC6A3     2B48 18                   SUB ECX,DWORD PTR DS:[EAX+0x18]
	$+3AD    006CC6A6     83F9 01                   CMP ECX,0x1
	$+3B0    006CC6A9     73 0A                     JNB SHORT PathOfEx.006CC6B5
	$+3B2    006CC6AB     68 40FAE600               PUSH PathOfEx.00E6FA40                            ; ASCII "list<T> too long"
	$+3B7    006CC6B0     E8 65DD5900               CALL PathOfEx.00C6A41A
	$+3BC    006CC6B5     FF40 18                   INC DWORD PTR DS:[EAX+0x18]
	$+3BF    006CC6B8     8B45 E8                   MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+3C2    006CC6BB     8950 04                   MOV DWORD PTR DS:[EAX+0x4],EDX
	$+3C5    006CC6BE     8B42 04                   MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+3C8    006CC6C1     8910                      MOV DWORD PTR DS:[EAX],EDX
	$+3CA    006CC6C3     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+3CC    006CC6C5     85C9                      TEST ECX,ECX
	$+3CE    006CC6C7     74 05                     JE SHORT PathOfEx.006CC6CE
	$+3D0    006CC6C9     E8 F2CE0400               CALL PathOfEx.007195C0
	$+3D5    006CC6CE     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+3D7    006CC6D0     C745 E8 7CB5EE00          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00EEB57C     ; ASCII "Actor"
	$+3DE    006CC6D7     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+3E1    006CC6DA     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+3E4    006CC6DD     50                        PUSH EAX
	$+3E5    006CC6DE     8D45 D0                   LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+3E8    006CC6E1     50                        PUSH EAX
	$+3E9    006CC6E2     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+3EC    006CC6E5     E8 56F9D9FF               CALL PathOfEx.0046C040
	$+3F1    006CC6EA     8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+3F4    006CC6ED     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+3F7    006CC6F0     74 13                     JE SHORT PathOfEx.006CC705
	$+3F9    006CC6F2     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+3FC    006CC6F5     83F9 FF                   CMP ECX,-0x1
	$+3FF    006CC6F8     74 0B                     JE SHORT PathOfEx.006CC705
	$+401    006CC6FA     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+404    006CC6FD     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+407    006CC700     8B0C88                    MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$+40A    006CC703     EB 05                     JMP SHORT PathOfEx.006CC70A
	$+40C    006CC705     8B75 C0                   MOV ESI,DWORD PTR SS:[EBP-0x40]
	$+40F    006CC708     33C9                      XOR ECX,ECX
	$+411    006CC70A     8D47 28                   LEA EAX,DWORD PTR DS:[EDI+0x28]
	$+414    006CC70D     83C1 58                   ADD ECX,0x58
	$+417    006CC710     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+41A    006CC713     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+41D    006CC716     50                        PUSH EAX
	$+41E    006CC717     E8 44EED9FF               CALL PathOfEx.0046B560
	$+423    006CC71C     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+426    006CC71F     8D47 2C                   LEA EAX,DWORD PTR DS:[EDI+0x2C]
	$+429    006CC722     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+42C    006CC725     81C1 D4020000             ADD ECX,0x2D4
	$+432    006CC72B     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+435    006CC72E     50                        PUSH EAX
	$+436    006CC72F     E8 CC50DDFF               CALL PathOfEx.004A1800
	$+43B    006CC734     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+43D    006CC736     85C9                      TEST ECX,ECX
	$+43F    006CC738     74 05                     JE SHORT PathOfEx.006CC73F
	$+441    006CC73A     E8 81CE0400               CALL PathOfEx.007195C0
	$+446    006CC73F     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+448    006CC741     C745 E8 A8AAEE00          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00EEAAA8     ; ASCII "Stats"
	$+44F    006CC748     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+452    006CC74B     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+455    006CC74E     50                        PUSH EAX
	$+456    006CC74F     8D45 D0                   LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+459    006CC752     50                        PUSH EAX
	$+45A    006CC753     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+45D    006CC756     E8 E5F8D9FF               CALL PathOfEx.0046C040
	$+462    006CC75B     8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+465    006CC75E     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+468    006CC761     74 13                     JE SHORT PathOfEx.006CC776
	$+46A    006CC763     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+46D    006CC766     83F9 FF                   CMP ECX,-0x1
	$+470    006CC769     74 0B                     JE SHORT PathOfEx.006CC776
	$+472    006CC76B     8B45 C0                   MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+475    006CC76E     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+478    006CC771     8B0C88                    MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$+47B    006CC774     EB 02                     JMP SHORT PathOfEx.006CC778
	$+47D    006CC776     33C9                      XOR ECX,ECX
	$+47F    006CC778     8B49 10                   MOV ECX,DWORD PTR DS:[ECX+0x10]
	$+482    006CC77B     68 150B0000               PUSH 0xB15
	$+487    006CC780     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+489    006CC782     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+48B    006CC784     FFD0                      CALL EAX
	$+48D    006CC786     85C0                      TEST EAX,EAX
	$+48F    006CC788     74 07                     JE SHORT PathOfEx.006CC791
	$+491    006CC78A     C687 D41A0000 01          MOV BYTE PTR DS:[EDI+0x1AD4],0x1
	$+498    006CC791     8B8F 38010000             MOV ECX,DWORD PTR DS:[EDI+0x138]
	$+49E    006CC797     E8 54ECDCFF               CALL PathOfEx.0049B3F0
	$+4A3    006CC79C     83BF A81A0000 01          CMP DWORD PTR DS:[EDI+0x1AA8],0x1
	$+4AA    006CC7A3     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+4AD    006CC7A6     75 3E                     JNZ SHORT PathOfEx.006CC7E6
	$+4AF    006CC7A8     8B55 EC                   MOV EDX,DWORD PTR SS:[EBP-0x14]
	$+4B2    006CC7AB     81C7 10190000             ADD EDI,0x1910
	$+4B8    006CC7B1     8B45 BC                   MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+4BB    006CC7B4     B9 20000000               MOV ECX,0x20
	$+4C0    006CC7B9     8D72 50                   LEA ESI,DWORD PTR DS:[EDX+0x50]
	$+4C3    006CC7BC     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
	$+4C5    006CC7BE     8DB2 D0000000             LEA ESI,DWORD PTR DS:[EDX+0xD0]
	$+4CB    006CC7C4     B9 20000000               MOV ECX,0x20
	$+4D0    006CC7C9     8DB8 90190000             LEA EDI,DWORD PTR DS:[EAX+0x1990]
	$+4D6    006CC7CF     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
	$+4D8    006CC7D1     8DB2 50010000             LEA ESI,DWORD PTR DS:[EDX+0x150]
	$+4DE    006CC7D7     B9 20000000               MOV ECX,0x20
	$+4E3    006CC7DC     8DB8 101A0000             LEA EDI,DWORD PTR DS:[EAX+0x1A10]
	$+4E9    006CC7E2     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
	$+4EB    006CC7E4     8BF8                      MOV EDI,EAX
	$+4ED    006CC7E6     8B47 2C                   MOV EAX,DWORD PTR DS:[EDI+0x2C]
	$+4F0    006CC7E9     8D4F 2C                   LEA ECX,DWORD PTR DS:[EDI+0x2C]
	$+4F3    006CC7EC     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$+4F6    006CC7EF     8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+4F9    006CC7F2     8B4D E8                   MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+4FC    006CC7F5     C1E1 07                   SHL ECX,0x7
	$+4FF    006CC7F8     8D70 D0                   LEA ESI,DWORD PTR DS:[EAX-0x30]
	$+502    006CC7FB     03F1                      ADD ESI,ECX
	$+504    006CC7FD     81C1 90180000             ADD ECX,0x1890
	$+50A    006CC803     03F9                      ADD EDI,ECX
	$+50C    006CC805     B9 20000000               MOV ECX,0x20
	$+511    006CC80A     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
	$+513    006CC80C     8B7D BC                   MOV EDI,DWORD PTR SS:[EBP-0x44]
	$+516    006CC80F     83BF 001B0000 00          CMP DWORD PTR DS:[EDI+0x1B00],0x0
	$+51D    006CC816     0F84 97000000             JE PathOfEx.006CC8B3
	$+523    006CC81C     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+529    006CC822     8A80 AC3E0000             MOV AL,BYTE PTR DS:[EAX+0x3EAC]
	$+52F    006CC828     84C0                      TEST AL,AL
	$+531    006CC82A     0F84 83000000             JE PathOfEx.006CC8B3
	$+537    006CC830     6A 28                     PUSH 0x28
	$+539    006CC832     E8 DFFE5A00               CALL PathOfEx.00C7C716
	$+53E    006CC837     83C4 04                   ADD ESP,0x4
	$+541    006CC83A     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+544    006CC83D     8BC8                      MOV ECX,EAX
	$+546    006CC83F     C745 FC 08000000          MOV DWORD PTR SS:[EBP-0x4],0x8
	$+54D    006CC846     E8 D512E0FF               CALL PathOfEx.004CDB20
	$+552    006CC84B     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+559    006CC852     8BB7 D81A0000             MOV ESI,DWORD PTR DS:[EDI+0x1AD8]
	$+55F    006CC858     8987 D81A0000             MOV DWORD PTR DS:[EDI+0x1AD8],EAX
	$+565    006CC85E     85F6                      TEST ESI,ESI
	$+567    006CC860     74 12                     JE SHORT PathOfEx.006CC874
	$+569    006CC862     8BCE                      MOV ECX,ESI
	$+56B    006CC864     E8 E714E0FF               CALL PathOfEx.004CDD50
	$+570    006CC869     6A 28                     PUSH 0x28
	$+572    006CC86B     56                        PUSH ESI
	$+573    006CC86C     E8 97FE5A00               CALL PathOfEx.00C7C708
	$+578    006CC871     83C4 08                   ADD ESP,0x8
	$+57B    006CC874     8B8F 001B0000             MOV ECX,DWORD PTR DS:[EDI+0x1B00]
	$+581    006CC87A     8D45 84                   LEA EAX,DWORD PTR SS:[EBP-0x7C]
	$+584    006CC87D     50                        PUSH EAX
	$+585    006CC87E     83EC 0C                   SUB ESP,0xC
	$+588    006CC881     E8 2A232E00               CALL PathOfEx.009AEBB0
	$+58D    006CC886     8B8F D81A0000             MOV ECX,DWORD PTR DS:[EDI+0x1AD8]
	$+593    006CC88C     8D45 84                   LEA EAX,DWORD PTR SS:[EBP-0x7C]
	$+596    006CC88F     50                        PUSH EAX
	$+597    006CC890     FFB7 001B0000             PUSH DWORD PTR DS:[EDI+0x1B00]
	$+59D    006CC896     E8 2519E0FF               CALL PathOfEx.004CE1C0
	$+5A2    006CC89B     8B8F D81A0000             MOV ECX,DWORD PTR DS:[EDI+0x1AD8]
	$+5A8    006CC8A1     8D87 A0150000             LEA EAX,DWORD PTR DS:[EDI+0x15A0]
	$+5AE    006CC8A7     50                        PUSH EAX
	$+5AF    006CC8A8     FFB7 3C010000             PUSH DWORD PTR DS:[EDI+0x13C]
	$+5B5    006CC8AE     E8 6D15E0FF               CALL PathOfEx.004CDE20
	$+5BA    006CC8B3     80BF D51A0000 00          CMP BYTE PTR DS:[EDI+0x1AD5],0x0
	$+5C1    006CC8BA     74 14                     JE SHORT PathOfEx.006CC8D0
	$+5C3    006CC8BC     8B87 3C010000             MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+5C9    006CC8C2     8A80 AC3E0000             MOV AL,BYTE PTR DS:[EAX+0x3EAC]
	$+5CF    006CC8C8     84C0                      TEST AL,AL
	$+5D1    006CC8CA     74 04                     JE SHORT PathOfEx.006CC8D0
	$+5D3    006CC8CC     B0 01                     MOV AL,0x1
	$+5D5    006CC8CE     EB 02                     JMP SHORT PathOfEx.006CC8D2
	$+5D7    006CC8D0     32C0                      XOR AL,AL
	$+5D9    006CC8D2     8887 D51A0000             MOV BYTE PTR DS:[EDI+0x1AD5],AL
	$+5DF    006CC8D8     83BF D81A0000 00          CMP DWORD PTR DS:[EDI+0x1AD8],0x0
	$+5E6    006CC8DF     74 0F                     JE SHORT PathOfEx.006CC8F0
	$+5E8    006CC8E1     8B8F D81A0000             MOV ECX,DWORD PTR DS:[EDI+0x1AD8]
	$+5EE    006CC8E7     6A 01                     PUSH 0x1
	$+5F0    006CC8E9     6A 00                     PUSH 0x0
	$+5F2    006CC8EB     E8 6016E0FF               CALL PathOfEx.004CDF50
	$+5F7    006CC8F0     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$+5FD    006CC8F6     85C9                      TEST ECX,ECX
	$+5FF    006CC8F8     74 17                     JE SHORT PathOfEx.006CC911
	$+601    006CC8FA     8B89 380A0000             MOV ECX,DWORD PTR DS:[ECX+0xA38]
	$+607    006CC900     8A87 D51A0000             MOV AL,BYTE PTR DS:[EDI+0x1AD5]
	$+60D    006CC906     8881 94090000             MOV BYTE PTR DS:[ECX+0x994],AL
	$+613    006CC90C     E8 CF8DF9FF               CALL PathOfEx.006656E0
	$+618    006CC911     8D8F B4000000             LEA ECX,DWORD PTR DS:[EDI+0xB4]
	$+61E    006CC917     E8 E471DEFF               CALL PathOfEx.004B3B00
	$+623    006CC91C     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$+629    006CC922     8D87 A0150000             LEA EAX,DWORD PTR DS:[EDI+0x15A0]
	$+62F    006CC928     50                        PUSH EAX
	$+630    006CC929     C787 C81A0000 00000000    MOV DWORD PTR DS:[EDI+0x1AC8],0x0
	$+63A    006CC933     C787 CC1A0000 20BF0200    MOV DWORD PTR DS:[EDI+0x1ACC],0x2BF20
	$+644    006CC93D     FFB7 3C010000             PUSH DWORD PTR DS:[EDI+0x13C]
	$+64A    006CC943     E8 88F8FAFF               CALL PathOfEx.0067C1D0
	$+64F    006CC948     8BB7 D8180000             MOV ESI,DWORD PTR DS:[EDI+0x18D8]
	$+655    006CC94E     80BE 54070000 01          CMP BYTE PTR DS:[ESI+0x754],0x1
	$+65C    006CC955     74 24                     JE SHORT PathOfEx.006CC97B
	$+65E    006CC957     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+660    006CC959     8BCE                      MOV ECX,ESI
	$+662    006CC95B     C686 54070000 01          MOV BYTE PTR DS:[ESI+0x754],0x1
	$+669    006CC962     FF90 B0000000             CALL DWORD PTR DS:[EAX+0xB0]
	$+66F    006CC968     8B8E 5C070000             MOV ECX,DWORD PTR DS:[ESI+0x75C]
	$+675    006CC96E     85C9                      TEST ECX,ECX
	$+677    006CC970     74 09                     JE SHORT PathOfEx.006CC97B
	$+679    006CC972     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+67B    006CC974     56                        PUSH ESI
	$+67C    006CC975     FF90 D0000000             CALL DWORD PTR DS:[EAX+0xD0]
	$+682    006CC97B     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$+688    006CC981     E8 CA36FBFF               CALL PathOfEx.00680050
	$+68D    006CC986     8B8F D8180000             MOV ECX,DWORD PTR DS:[EDI+0x18D8]
	$+693    006CC98C     8A81 2E0B0000             MOV AL,BYTE PTR DS:[ECX+0xB2E]
	$+699    006CC992     84C0                      TEST AL,AL
	$+69B    006CC994     74 4E                     JE SHORT PathOfEx.006CC9E4
	$+69D    006CC996     83EC 18                   SUB ESP,0x18
	$+6A0    006CC999     8D91 300B0000             LEA EDX,DWORD PTR DS:[ECX+0xB30]
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??????????c7??????????????????8b??????????89??????????85??74??c7????????????8b", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_after_init_game_map_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-1F8    008D1872     FFB7 4C3D0000        PUSH DWORD PTR DS:[EDI+0x3D4C]
	$-1F2    008D1878     50                   PUSH EAX
	$-1F1    008D1879     E8 5271F2FF          CALL PathOfEx.007F89D0
	$-1EC    008D187E     C645 FC 65           MOV BYTE PTR SS:[EBP-0x4],0x65
	$-1E8    008D1882     80BF 8D3B0000 00     CMP BYTE PTR DS:[EDI+0x3B8D],0x0
	$-1E1    008D1889     8B75 5C              MOV ESI,DWORD PTR SS:[EBP+0x5C]
	$-1DE    008D188C     75 1C                JNZ SHORT PathOfEx.008D18AA
	$-1DC    008D188E     8D87 C03A0000        LEA EAX,DWORD PTR DS:[EDI+0x3AC0]
	$-1D6    008D1894     8945 6C              MOV DWORD PTR SS:[EBP+0x6C],EAX
	$-1D3    008D1897     8D4E 34              LEA ECX,DWORD PTR DS:[ESI+0x34]
	$-1D0    008D189A     8D45 6C              LEA EAX,DWORD PTR SS:[EBP+0x6C]
	$-1CD    008D189D     50                   PUSH EAX
	$-1CC    008D189E     E8 5DFFBCFF          CALL PathOfEx.004A1800
	$-1C7    008D18A3     C687 8D3B0000 01     MOV BYTE PTR DS:[EDI+0x3B8D],0x1
	$-1C0    008D18AA     8B43 0C              MOV EAX,DWORD PTR DS:[EBX+0xC]
	$-1BD    008D18AD     8D8F C03A0000        LEA ECX,DWORD PTR DS:[EDI+0x3AC0]
	$-1B7    008D18B3     8945 6C              MOV DWORD PTR SS:[EBP+0x6C],EAX
	$-1B4    008D18B6     8B43 08              MOV EAX,DWORD PTR DS:[EBX+0x8]
	$-1B1    008D18B9     8945 54              MOV DWORD PTR SS:[EBP+0x54],EAX
	$-1AE    008D18BC     0FB743 18            MOVZX EAX,WORD PTR DS:[EBX+0x18]
	$-1AA    008D18C0     56                   PUSH ESI
	$-1A9    008D18C1     50                   PUSH EAX
	$-1A8    008D18C2     0FB743 16            MOVZX EAX,WORD PTR DS:[EBX+0x16]
	$-1A4    008D18C6     50                   PUSH EAX
	$-1A3    008D18C7     0FB743 14            MOVZX EAX,WORD PTR DS:[EBX+0x14]
	$-19F    008D18CB     50                   PUSH EAX
	$-19E    008D18CC     0FB643 1B            MOVZX EAX,BYTE PTR DS:[EBX+0x1B]
	$-19A    008D18D0     50                   PUSH EAX
	$-199    008D18D1     0FB643 1A            MOVZX EAX,BYTE PTR DS:[EBX+0x1A]
	$-195    008D18D5     50                   PUSH EAX
	$-194    008D18D6     8D45 6C              LEA EAX,DWORD PTR SS:[EBP+0x6C]
	$-191    008D18D9     50                   PUSH EAX
	$-190    008D18DA     8D45 54              LEA EAX,DWORD PTR SS:[EBP+0x54]
	$-18D    008D18DD     50                   PUSH EAX
	$-18C    008D18DE     E8 4D6EF3FF          CALL PathOfEx.00808730
	$-187    008D18E3     8D4D 5C              LEA ECX,DWORD PTR SS:[EBP+0x5C]
	$-184    008D18E6     E8 0591B9FF          CALL PathOfEx.0046A9F0
	$-17F    008D18EB     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-17C    008D18EE     E9 F6020000          JMP PathOfEx.008D1BE9
	$-177    008D18F3     8B53 08              MOV EDX,DWORD PTR DS:[EBX+0x8]
	$-174    008D18F6     8D87 10150000        LEA EAX,DWORD PTR DS:[EDI+0x1510]
	$-16E    008D18FC     50                   PUSH EAX
	$-16D    008D18FD     8D87 90150000        LEA EAX,DWORD PTR DS:[EDI+0x1590]
	$-167    008D1903     50                   PUSH EAX
	$-166    008D1904     8D8F E83C0000        LEA ECX,DWORD PTR DS:[EDI+0x3CE8]
	$-160    008D190A     E8 C1BF0000          CALL PathOfEx.008DD8D0
	$-15B    008D190F     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-158    008D1912     83C4 08              ADD ESP,0x8
	$-155    008D1915     E9 CF020000          JMP PathOfEx.008D1BE9
	$-150    008D191A     807B 10 00           CMP BYTE PTR DS:[EBX+0x10],0x0
	$-14C    008D191E     0FB743 08            MOVZX EAX,WORD PTR DS:[EBX+0x8]
	$-148    008D1922     8D14C0               LEA EDX,DWORD PTR DS:[EAX+EAX*8]
	$-145    008D1925     8B43 0C              MOV EAX,DWORD PTR DS:[EBX+0xC]
	$-142    008D1928     74 22                JE SHORT PathOfEx.008D194C
	$-140    008D192A     8B8F A83B0000        MOV ECX,DWORD PTR DS:[EDI+0x3BA8]
	$-13A    008D1930     8944D1 08            MOV DWORD PTR DS:[ECX+EDX*8+0x8],EAX
	$-136    008D1934     8D8F 10230000        LEA ECX,DWORD PTR DS:[EDI+0x2310]
	$-130    008D193A     0FB743 08            MOVZX EAX,WORD PTR DS:[EBX+0x8]
	$-12C    008D193E     50                   PUSH EAX
	$-12B    008D193F     E8 FC1FBEFF          CALL PathOfEx.004B3940
	$-126    008D1944     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-123    008D1947     E9 9D020000          JMP PathOfEx.008D1BE9
	$-11E    008D194C     8B8F 9C3B0000        MOV ECX,DWORD PTR DS:[EDI+0x3B9C]
	$-118    008D1952     8944D1 08            MOV DWORD PTR DS:[ECX+EDX*8+0x8],EAX
	$-114    008D1956     8D8F 90220000        LEA ECX,DWORD PTR DS:[EDI+0x2290]
	$-10E    008D195C     0FB743 08            MOVZX EAX,WORD PTR DS:[EBX+0x8]
	$-10A    008D1960     50                   PUSH EAX
	$-109    008D1961     E8 DA1FBEFF          CALL PathOfEx.004B3940
	$-104    008D1966     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-101    008D1969     E9 7B020000          JMP PathOfEx.008D1BE9
	$-FC     008D196E     8B43 38              MOV EAX,DWORD PTR DS:[EBX+0x38]
	$-F9     008D1971     C1E8 02              SHR EAX,0x2
	$-F6     008D1974     24 01                AND AL,0x1
	$-F4     008D1976     74 78                JE SHORT PathOfEx.008D19F0
	$-F2     008D1978     83C3 08              ADD EBX,0x8
	$-EF     008D197B     81C7 C43B0000        ADD EDI,0x3BC4
	$-E9     008D1981     53                   PUSH EBX
	$-E8     008D1982     8BCF                 MOV ECX,EDI
	$-E6     008D1984     895D 54              MOV DWORD PTR SS:[EBP+0x54],EBX
	$-E3     008D1987     897D 6C              MOV DWORD PTR SS:[EBP+0x6C],EDI
	$-E0     008D198A     E8 71EDB9FF          CALL PathOfEx.00470700
	$-DB     008D198F     8B3F                 MOV EDI,DWORD PTR DS:[EDI]
	$-D9     008D1991     8BF0                 MOV ESI,EAX
	$-D7     008D1993     3BF7                 CMP ESI,EDI
	$-D5     008D1995     74 0E                JE SHORT PathOfEx.008D19A5
	$-D3     008D1997     8D46 10              LEA EAX,DWORD PTR DS:[ESI+0x10]
	$-D0     008D199A     50                   PUSH EAX
	$-CF     008D199B     53                   PUSH EBX
	$-CE     008D199C     E8 FFECB9FF          CALL PathOfEx.004706A0
	$-C9     008D19A1     84C0                 TEST AL,AL
	$-C7     008D19A3     74 02                JE SHORT PathOfEx.008D19A7
	$-C5     008D19A5     8BF7                 MOV ESI,EDI
	$-C3     008D19A7     8B4D 6C              MOV ECX,DWORD PTR SS:[EBP+0x6C]
	$-C0     008D19AA     8D45 64              LEA EAX,DWORD PTR SS:[EBP+0x64]
	$-BD     008D19AD     56                   PUSH ESI
	$-BC     008D19AE     50                   PUSH EAX
	$-BB     008D19AF     E8 DCD5D2FF          CALL PathOfEx.005FEF90
	$-B6     008D19B4     8B5D 48              MOV EBX,DWORD PTR SS:[EBP+0x48]
	$-B3     008D19B7     8D45 6C              LEA EAX,DWORD PTR SS:[EBP+0x6C]
	$-B0     008D19BA     8B7D 54              MOV EDI,DWORD PTR SS:[EBP+0x54]
	$-AD     008D19BD     57                   PUSH EDI
	$-AC     008D19BE     50                   PUSH EAX
	$-AB     008D19BF     8D8B CC3B0000        LEA ECX,DWORD PTR DS:[EBX+0x3BCC]
	$-A5     008D19C5     E8 06D8B9FF          CALL PathOfEx.0046F1D0
	$-A0     008D19CA     FF75 6C              PUSH DWORD PTR SS:[EBP+0x6C]
	$-9D     008D19CD     8D45 64              LEA EAX,DWORD PTR SS:[EBP+0x64]
	$-9A     008D19D0     50                   PUSH EAX
	$-99     008D19D1     8D8B CC3B0000        LEA ECX,DWORD PTR DS:[EBX+0x3BCC]
	$-93     008D19D7     E8 741BDEFF          CALL PathOfEx.006B3550
	$-8E     008D19DC     57                   PUSH EDI
	$-8D     008D19DD     8D8B 101F0000        LEA ECX,DWORD PTR DS:[EBX+0x1F10]
	$-87     008D19E3     E8 E8B9C2FF          CALL PathOfEx.004FD3D0
	$-82     008D19E8     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-7F     008D19EB     E9 F9010000          JMP PathOfEx.008D1BE9
	$-7A     008D19F0     8B4D 48              MOV ECX,DWORD PTR SS:[EBP+0x48]
	$-77     008D19F3     8D7B 08              LEA EDI,DWORD PTR DS:[EBX+0x8]
	$-74     008D19F6     57                   PUSH EDI
	$-73     008D19F7     8D45 5C              LEA EAX,DWORD PTR SS:[EBP+0x5C]
	$-70     008D19FA     50                   PUSH EAX
	$-6F     008D19FB     8D89 C43B0000        LEA ECX,DWORD PTR DS:[ECX+0x3BC4]
	$-69     008D1A01     E8 DACF0000          CALL PathOfEx.008DE9E0
	$-64     008D1A06     8A43 38              MOV AL,BYTE PTR DS:[EBX+0x38]
	$-61     008D1A09     8B75 5C              MOV ESI,DWORD PTR SS:[EBP+0x5C]
	$-5E     008D1A0C     24 01                AND AL,0x1
	$-5C     008D1A0E     8B4B 38              MOV ECX,DWORD PTR DS:[EBX+0x38]
	$-59     008D1A11     D1E9                 SHR ECX,1
	$-57     008D1A13     80E1 01              AND CL,0x1
	$-54     008D1A16     8846 28              MOV BYTE PTR DS:[ESI+0x28],AL
	$-51     008D1A19     8D43 20              LEA EAX,DWORD PTR DS:[EBX+0x20]
	$-4E     008D1A1C     8B5D 48              MOV EBX,DWORD PTR SS:[EBP+0x48]
	$-4B     008D1A1F     50                   PUSH EAX
	$-4A     008D1A20     884E 29              MOV BYTE PTR DS:[ESI+0x29],CL
	$-47     008D1A23     57                   PUSH EDI
	$-46     008D1A24     8D8B CC3B0000        LEA ECX,DWORD PTR DS:[EBX+0x3BCC]
	$-40     008D1A2A     E8 B160EBFF          CALL PathOfEx.00787AE0
	$-3B     008D1A2F     8BC8                 MOV ECX,EAX
	$-39     008D1A31     E8 5A59BBFF          CALL PathOfEx.00487390
	$-34     008D1A36     0FB646 29            MOVZX EAX,BYTE PTR DS:[ESI+0x29]
	$-30     008D1A3A     8D8B 901E0000        LEA ECX,DWORD PTR DS:[EBX+0x1E90]
	$-2A     008D1A40     50                   PUSH EAX
	$-29     008D1A41     0FB646 28            MOVZX EAX,BYTE PTR DS:[ESI+0x28]
	$-25     008D1A45     50                   PUSH EAX
	$-24     008D1A46     57                   PUSH EDI
	$-23     008D1A47     E8 849F0000          CALL PathOfEx.008DB9D0
	$-1E     008D1A4C     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$-1B     008D1A4F     E9 95010000          JMP PathOfEx.008D1BE9
	$-16     008D1A54     0FB643 1E            MOVZX EAX,BYTE PTR DS:[EBX+0x1E]
	$-12     008D1A58     8B8F 903A0000        MOV ECX,DWORD PTR DS:[EDI+0x3A90]        ; edi=stCD_GameLogic指针+4
	$-C      008D1A5E     F3:0F1043 10         MOVSS XMM0,DWORD PTR DS:[EBX+0x10]
	$-7      008D1A63     50                   PUSH EAX
	$-6      008D1A64     0FB743 1C            MOVZX EAX,WORD PTR DS:[EBX+0x1C]         ; 环境id,控制着游戏环境，包括音乐、灯光、下雨、雾等
	$-2      008D1A68     8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$ ==>    008D1A6A     50                   PUSH EAX                                 ; stCD_TblItemEnvironments的id
	$+1      008D1A6B     51                   PUSH ECX
	$+2      008D1A6C     F3:0F110424          MOVSS DWORD PTR SS:[ESP],XMM0
	$+7      008D1A71     8D43 14              LEA EAX,DWORD PTR DS:[EBX+0x14]
	$+A      008D1A74     F3:0F1043 0C         MOVSS XMM0,DWORD PTR DS:[EBX+0xC]
	$+F      008D1A79     50                   PUSH EAX
	$+10     008D1A7A     51                   PUSH ECX
	$+11     008D1A7B     F3:0F110424          MOVSS DWORD PTR SS:[ESP],XMM0
	$+16     008D1A80     FF73 08              PUSH DWORD PTR DS:[EBX+0x8]
	$+19     008D1A83     FF52 40              CALL DWORD PTR DS:[EDX+0x40]             ; call tag_切换地图中的房间会断到
	$+1C     008D1A86     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$+1F     008D1A89     E9 5B010000          JMP PathOfEx.008D1BE9
	$+24     008D1A8E     8D43 08              LEA EAX,DWORD PTR DS:[EBX+0x8]
	$+27     008D1A91     50                   PUSH EAX
	$+28     008D1A92     8D8F 901B0000        LEA ECX,DWORD PTR DS:[EDI+0x1B90]
	$+2E     008D1A98     E8 33B9C2FF          CALL PathOfEx.004FD3D0
	$+33     008D1A9D     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$+36     008D1AA0     E9 44010000          JMP PathOfEx.008D1BE9
	$+3B     008D1AA5     8B8F 903A0000        MOV ECX,DWORD PTR DS:[EDI+0x3A90]
	$+41     008D1AAB     0FB743 08            MOVZX EAX,WORD PTR DS:[EBX+0x8]
	$+45     008D1AAF     50                   PUSH EAX
	$+46     008D1AB0     8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+48     008D1AB2     FF52 38              CALL DWORD PTR DS:[EDX+0x38]
	$+4B     008D1AB5     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$+4E     008D1AB8     E9 2C010000          JMP PathOfEx.008D1BE9
	$+53     008D1ABD     8D45 58              LEA EAX,DWORD PTR SS:[EBP+0x58]
	$+56     008D1AC0     50                   PUSH EAX
	$+57     008D1AC1     E8 4AA3F2FF          CALL PathOfEx.007FBE10
	$+5C     008D1AC6     83C4 04              ADD ESP,0x4
	$+5F     008D1AC9     6A 01                PUSH 0x1
	$+61     008D1ACB     68 90AA4600          PUSH PathOfEx.0046AA90
	$+66     008D1AD0     68 90AA4600          PUSH PathOfEx.0046AA90
	$+6B     008D1AD5     68 C033E700          PUSH PathOfEx.00E733C0                   ; UNICODE "Data/MonsterVarieties.dat"
	$+70     008D1ADA     C645 FC 66           MOV BYTE PTR SS:[EBP-0x4],0x66
	$+74     008D1ADE     E8 1D65E4FF          CALL PathOfEx.00718000
	$+79     008D1AE3     8BC8                 MOV ECX,EAX
	$+7B     008D1AE5     E8 C60FBDFF          CALL PathOfEx.004A2AB0
	$+80     008D1AEA     8945 30              MOV DWORD PTR SS:[EBP+0x30],EAX
	$+83     008D1AED     C645 FC 67           MOV BYTE PTR SS:[EBP-0x4],0x67
	$+87     008D1AF1     8D97 BC3C0000        LEA EDX,DWORD PTR DS:[EDI+0x3CBC]
	$+8D     008D1AF7     8B4A 04              MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+90     008D1AFA     8B43 0C              MOV EAX,DWORD PTR DS:[EBX+0xC]
	$+93     008D1AFD     2B0A                 SUB ECX,DWORD PTR DS:[EDX]
	$+95     008D1AFF     2B43 08              SUB EAX,DWORD PTR DS:[EBX+0x8]
	$+98     008D1B02     C1F9 02              SAR ECX,0x2
	$+9B     008D1B05     D1F8                 SAR EAX,1
	$+9D     008D1B07     03C1                 ADD EAX,ECX
	$+9F     008D1B09     8955 68              MOV DWORD PTR SS:[EBP+0x68],EDX
	$+A2     008D1B0C     50                   PUSH EAX
	$+A3     008D1B0D     8BCA                 MOV ECX,EDX
	$+A5     008D1B0F     E8 6C990000          CALL PathOfEx.008DB480
	$+AA     008D1B14     8B73 08              MOV ESI,DWORD PTR DS:[EBX+0x8]
	$+AD     008D1B17     8B7B 14              MOV EDI,DWORD PTR DS:[EBX+0x14]
	$+B0     008D1B1A     3B73 0C              CMP ESI,DWORD PTR DS:[EBX+0xC]
	$+B3     008D1B1D     0F84 8B000000        JE PathOfEx.008D1BAE
	$+B9     008D1B23     0FB706               MOVZX EAX,WORD PTR DS:[ESI]
	$+BC     008D1B26     8D55 30              LEA EDX,DWORD PTR SS:[EBP+0x30]
	$+BF     008D1B29     50                   PUSH EAX
	$+C0     008D1B2A     8D4D 5C              LEA ECX,DWORD PTR SS:[EBP+0x5C]
	$+C3     008D1B2D     E8 CE66E6FF          CALL PathOfEx.00738200
	$+C8     008D1B32     83C4 04              ADD ESP,0x4
	$+CB     008D1B35     C645 FC 68           MOV BYTE PTR SS:[EBP-0x4],0x68
	$+CF     008D1B39     8D4D 58              LEA ECX,DWORD PTR SS:[EBP+0x58]
	$+D2     008D1B3C     8A07                 MOV AL,BYTE PTR DS:[EDI]
	$+D4     008D1B3E     84C0                 TEST AL,AL
	$+D6     008D1B40     74 2A                JE SHORT PathOfEx.008D1B6C
	$+D8     008D1B42     0FB6C0               MOVZX EAX,AL
	$+DB     008D1B45     50                   PUSH EAX
	$+DC     008D1B46     8D45 5C              LEA EAX,DWORD PTR SS:[EBP+0x5C]
	$+DF     008D1B49     50                   PUSH EAX
	$+E0     008D1B4A     8D45 6C              LEA EAX,DWORD PTR SS:[EBP+0x6C]
	$+E3     008D1B4D     50                   PUSH EAX
	$+E4     008D1B4E     E8 1DFCBBFF          CALL PathOfEx.00491770
	$+E9     008D1B53     8BC8                 MOV ECX,EAX
	$+EB     008D1B55     E8 A69EF2FF          CALL PathOfEx.007FBA00
	$+F0     008D1B5A     C645 FC 69           MOV BYTE PTR SS:[EBP-0x4],0x69
	$+F4     008D1B5E     8B4D 68              MOV ECX,DWORD PTR SS:[EBP+0x68]
	$+F7     008D1B61     50                   PUSH EAX
	$+F8     008D1B62     E8 A9FDD9FF          CALL PathOfEx.00671910
	$+FD     008D1B67     8D4D 6C              LEA ECX,DWORD PTR SS:[EBP+0x6C]
	$+100    008D1B6A     EB 24                JMP SHORT PathOfEx.008D1B90
	$+102    008D1B6C     8D45 5C              LEA EAX,DWORD PTR SS:[EBP+0x5C]
	$+105    008D1B6F     50                   PUSH EAX
	$+106    008D1B70     8D45 54              LEA EAX,DWORD PTR SS:[EBP+0x54]
	$+109    008D1B73     50                   PUSH EAX
	$+10A    008D1B74     E8 F7FBBBFF          CALL PathOfEx.00491770
	$+10F    008D1B79     8BC8                 MOV ECX,EAX
	$+111    008D1B7B     E8 E0A0F2FF          CALL PathOfEx.007FBC60
	$+116    008D1B80     C645 FC 6A           MOV BYTE PTR SS:[EBP-0x4],0x6A
	$+11A    008D1B84     8B4D 68              MOV ECX,DWORD PTR SS:[EBP+0x68]
	$+11D    008D1B87     50                   PUSH EAX
	$+11E    008D1B88     E8 83FDD9FF          CALL PathOfEx.00671910
	$+123    008D1B8D     8D4D 54              LEA ECX,DWORD PTR SS:[EBP+0x54]
	$+126    008D1B90     E8 7BAFB9FF          CALL PathOfEx.0046CB10
	$+12B    008D1B95     8D4D 5C              LEA ECX,DWORD PTR SS:[EBP+0x5C]
	$+12E    008D1B98     C645 FC 67           MOV BYTE PTR SS:[EBP-0x4],0x67
	$+132    008D1B9C     E8 FFE0B9FF          CALL PathOfEx.0046FCA0
	$+137    008D1BA1     83C6 02              ADD ESI,0x2
	$+13A    008D1BA4     47                   INC EDI
	$+13B    008D1BA5     3B73 0C              CMP ESI,DWORD PTR DS:[EBX+0xC]
	$+13E    008D1BA8   ^ 0F85 75FFFFFF        JNZ PathOfEx.008D1B23
	$+144    008D1BAE     8D4D 30              LEA ECX,DWORD PTR SS:[EBP+0x30]
	$+147    008D1BB1     E8 5AAFB9FF          CALL PathOfEx.0046CB10
	$+14C    008D1BB6     8D4D 58              LEA ECX,DWORD PTR SS:[EBP+0x58]
	$+14F    008D1BB9     E8 52AFB9FF          CALL PathOfEx.0046CB10
	$+154    008D1BBE     8B4D 7C              MOV ECX,DWORD PTR SS:[EBP+0x7C]
	$+157    008D1BC1     EB 26                JMP SHORT PathOfEx.008D1BE9
	$+159    008D1BC3     8A43 08              MOV AL,BYTE PTR DS:[EBX+0x8]
	$+15C    008D1BC6     8A97 A43E0000        MOV DL,BYTE PTR DS:[EDI+0x3EA4]
	$+162    008D1BCC     24 01                AND AL,0x1
	$+164    008D1BCE     8887 A43E0000        MOV BYTE PTR DS:[EDI+0x3EA4],AL
	$+16A    008D1BD4     8AC8                 MOV CL,AL
	$+16C    008D1BD6     F643 08 02           TEST BYTE PTR DS:[EBX+0x8],0x2
	$+170    008D1BDA     74 39                JE SHORT PathOfEx.008D1C15
	$+172    008D1BDC     C787 643F0000 020000>MOV DWORD PTR DS:[EDI+0x3F64],0x2
	$+17C    008D1BE6     8B4D 50              MOV ECX,DWORD PTR SS:[EBP+0x50]
	$+17F    008D1BE9     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+186    008D1BF0     85C9                 TEST ECX,ECX
	$+188    008D1BF2     74 0D                JE SHORT PathOfEx.008D1C01
	$+18A    008D1BF4     C745 FC 98000000     MOV DWORD PTR SS:[EBP-0x4],0x98
	$+191    008D1BFB     8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+193    008D1BFD     6A 01                PUSH 0x1
	$+195    008D1BFF     FF10                 CALL DWORD PTR DS:[EAX]
	$+197    008D1C01     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+19A    008D1C04     5F                   POP EDI
	$+19B    008D1C05     5E                   POP ESI
	$+19C    008D1C06     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+1A3    008D1C0D     5B                   POP EBX
	$+1A4    008D1C0E     8D65 70              LEA ESP,DWORD PTR SS:[EBP+0x70]
	$+1A7    008D1C11     5D                   POP EBP
	$+1A8    008D1C12     C2 0800              RETN 0x8                                 ; 最后的返回，收包已处理完毕
	*/
	/*addr_tmp = scan_exe_.ScanAddr("5051f3????????8d????f3????????5051f3????????ff????ff", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_modify_game_env_id_at_change_room_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;*/

	/*
	$-235    006D95C6          8D85 18FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0xE8]
	$-22F    006D95CC          FF73 1C                   PUSH DWORD PTR DS:[EBX+0x1C]
	$-22C    006D95CF          68 5CB1EA00               PUSH PathOfEx.00EAB15C                               ; UNICODE "Tile hash: "
	$-227    006D95D4          50                        PUSH EAX
	$-226    006D95D5          E8 36EEDAFF               CALL PathOfEx.00488410
	$-221    006D95DA          83C4 08                   ADD ESP,0x8
	$-21E    006D95DD          8BC8                      MOV ECX,EAX
	$-21C    006D95DF          E8 CC74DAFF               CALL PathOfEx.00480AB0
	$-217    006D95E4          8D45 00                   LEA EAX,DWORD PTR SS:[EBP]
	$-214    006D95E7          50                        PUSH EAX
	$-213    006D95E8          8D8D 08FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0xF8]
	$-20D    006D95EE          E8 FDEDDAFF               CALL PathOfEx.004883F0
	$-208    006D95F3          8BF8                      MOV EDI,EAX
	$-206    006D95F5          C645 FC 07                MOV BYTE PTR SS:[EBP-0x4],0x7
	$-202    006D95F9          FF15 C8E0D800             CALL DWORD PTR DS:[0xD8E0C8]                         ; kernel32.GetTickCount
	$-1FC    006D95FF          8BF0                      MOV ESI,EAX
	$-1FA    006D9601          8D45 20                   LEA EAX,DWORD PTR SS:[EBP+0x20]
	$-1F7    006D9604          50                        PUSH EAX
	$-1F6    006D9605          E8 BD175D00               CALL PathOfEx.00CAADC7
	$-1F1    006D960A          64:8B0D 2C000000          MOV ECX,DWORD PTR FS:[0x2C]
	$-1EA    006D9611          83C4 04                   ADD ESP,0x4
	$-1E7    006D9614          8B09                      MOV ECX,DWORD PTR DS:[ECX]
	$-1E5    006D9616          57                        PUSH EDI
	$-1E4    006D9617          83EC 0C                   SUB ESP,0xC
	$-1E1    006D961A          8B49 18                   MOV ECX,DWORD PTR DS:[ECX+0x18]
	$-1DE    006D961D          6A 00                     PUSH 0x0
	$-1DC    006D961F          56                        PUSH ESI
	$-1DB    006D9620          FF75 24                   PUSH DWORD PTR SS:[EBP+0x24]
	$-1D8    006D9623          FF75 20                   PUSH DWORD PTR SS:[EBP+0x20]
	$-1D5    006D9626          6A 6F                     PUSH 0x6F
	$-1D3    006D9628          E8 63EADAFF               CALL PathOfEx.00488090
	$-1CE    006D962D          8B45 14                   MOV EAX,DWORD PTR SS:[EBP+0x14]
	$-1CB    006D9630          83F8 08                   CMP EAX,0x8
	$-1C8    006D9633          72 0F                     JB SHORT PathOfEx.006D9644
	$-1C6    006D9635          40                        INC EAX
	$-1C5    006D9636          6A 02                     PUSH 0x2
	$-1C3    006D9638          50                        PUSH EAX
	$-1C2    006D9639          FF75 00                   PUSH DWORD PTR SS:[EBP]
	$-1BF    006D963C          E8 EF31D9FF               CALL PathOfEx.0046C830
	$-1BA    006D9641          83C4 0C                   ADD ESP,0xC
	$-1B7    006D9644          8D8D 70FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x90]
	$-1B1    006D964A          E8 21D7D9FF               CALL PathOfEx.00476D70
	$-1AC    006D964F          8D85 70FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x90]
	$-1A6    006D9655          C645 FC 08                MOV BYTE PTR SS:[EBP-0x4],0x8
	$-1A2    006D9659          50                        PUSH EAX
	$-1A1    006D965A          C785 70FFFFFF C453DE00    MOV DWORD PTR SS:[EBP-0x90],PathOfEx.00DE53C4
	$-197    006D9664          E8 880E5900               CALL PathOfEx.00C6A4F1
	$-192    006D9669          C785 58FEFFFF ECE8EA00    MOV DWORD PTR SS:[EBP-0x1A8],PathOfEx.00EAE8EC
	$-188    006D9673          C785 68FEFFFF 98E7EA00    MOV DWORD PTR SS:[EBP-0x198],PathOfEx.00EAE798
	$-17E    006D967D          C785 C0FEFFFF 4C54DE00    MOV DWORD PTR SS:[EBP-0x140],PathOfEx.00DE544C
	$-174    006D9687          8D85 70FEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x190]
	$-16E    006D968D          C645 FC 09                MOV BYTE PTR SS:[EBP-0x4],0x9
	$-16A    006D9691          50                        PUSH EAX
	$-169    006D9692          8D8D 58FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x1A8]
	$-163    006D9698          C745 24 03000000          MOV DWORD PTR SS:[EBP+0x24],0x3
	$-15C    006D969F          E8 0CEED9FF               CALL PathOfEx.004784B0
	$-157    006D96A4          C745 FC 0A000000          MOV DWORD PTR SS:[EBP-0x4],0xA
	$-150    006D96AB          8B85 58FEFFFF             MOV EAX,DWORD PTR SS:[EBP-0x1A8]
	$-14A    006D96B1          6A 03                     PUSH 0x3
	$-148    006D96B3          8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-145    006D96B6          C78405 58FEFFFF 64FCE600  MOV DWORD PTR SS:[EBP+EAX-0x1A8],PathOfEx.00E6FC64
	$-13A    006D96C1          8B85 58FEFFFF             MOV EAX,DWORD PTR SS:[EBP-0x1A8]
	$-134    006D96C7          8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-131    006D96CA          8D41 98                   LEA EAX,DWORD PTR DS:[ECX-0x68]
	$-12E    006D96CD          89840D 54FEFFFF           MOV DWORD PTR SS:[EBP+ECX-0x1AC],EAX
	$-127    006D96D4          8D8D 70FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x190]
	$-121    006D96DA          E8 B1F3D9FF               CALL PathOfEx.00478A90
	$-11C    006D96DF          C645 FC 0B                MOV BYTE PTR SS:[EBP-0x4],0xB
	$-118    006D96E3          8D85 68FEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x198]
	$-112    006D96E9          FF73 20                   PUSH DWORD PTR DS:[EBX+0x20]
	$-10F    006D96EC          68 40B1EA00               PUSH PathOfEx.00EAB140                               ; UNICODE "Doodad hash: "
	$-10A    006D96F1          50                        PUSH EAX
	$-109    006D96F2          E8 19EDDAFF               CALL PathOfEx.00488410
	$-104    006D96F7          83C4 08                   ADD ESP,0x8
	$-101    006D96FA          8BC8                      MOV ECX,EAX
	$-FF     006D96FC          E8 AF73DAFF               CALL PathOfEx.00480AB0
	$-FA     006D9701          8D45 00                   LEA EAX,DWORD PTR SS:[EBP]
	$-F7     006D9704          50                        PUSH EAX
	$-F6     006D9705          8D8D 58FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x1A8]
	$-F0     006D970B          E8 E0ECDAFF               CALL PathOfEx.004883F0
	$-EB     006D9710          8BF0                      MOV ESI,EAX
	$-E9     006D9712          C645 FC 0C                MOV BYTE PTR SS:[EBP-0x4],0xC
	$-E5     006D9716          FF15 C8E0D800             CALL DWORD PTR DS:[0xD8E0C8]                         ; kernel32.GetTickCount
	$-DF     006D971C          8BF8                      MOV EDI,EAX
	$-DD     006D971E          8D45 20                   LEA EAX,DWORD PTR SS:[EBP+0x20]
	$-DA     006D9721          50                        PUSH EAX
	$-D9     006D9722          E8 A0165D00               CALL PathOfEx.00CAADC7
	$-D4     006D9727          64:8B0D 2C000000          MOV ECX,DWORD PTR FS:[0x2C]
	$-CD     006D972E          83C4 04                   ADD ESP,0x4
	$-CA     006D9731          8B09                      MOV ECX,DWORD PTR DS:[ECX]
	$-C8     006D9733          56                        PUSH ESI
	$-C7     006D9734          83EC 0C                   SUB ESP,0xC
	$-C4     006D9737          8B49 18                   MOV ECX,DWORD PTR DS:[ECX+0x18]
	$-C1     006D973A          6A 00                     PUSH 0x0
	$-BF     006D973C          57                        PUSH EDI
	$-BE     006D973D          FF75 24                   PUSH DWORD PTR SS:[EBP+0x24]
	$-BB     006D9740          FF75 20                   PUSH DWORD PTR SS:[EBP+0x20]
	$-B8     006D9743          6A 70                     PUSH 0x70
	$-B6     006D9745          E8 46E9DAFF               CALL PathOfEx.00488090
	$-B1     006D974A          8B45 14                   MOV EAX,DWORD PTR SS:[EBP+0x14]
	$-AE     006D974D          83F8 08                   CMP EAX,0x8
	$-AB     006D9750          72 0F                     JB SHORT PathOfEx.006D9761
	$-A9     006D9752          40                        INC EAX
	$-A8     006D9753          6A 02                     PUSH 0x2
	$-A6     006D9755          50                        PUSH EAX
	$-A5     006D9756          FF75 00                   PUSH DWORD PTR SS:[EBP]
	$-A2     006D9759          E8 D230D9FF               CALL PathOfEx.0046C830
	$-9D     006D975E          83C4 0C                   ADD ESP,0xC
	$-9A     006D9761          8D8D C0FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x140]
	$-94     006D9767          E8 04D6D9FF               CALL PathOfEx.00476D70
	$-8F     006D976C          8D85 C0FEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x140]
	$-89     006D9772          C645 FC 0D                MOV BYTE PTR SS:[EBP-0x4],0xD
	$-85     006D9776          50                        PUSH EAX
	$-84     006D9777          C785 C0FEFFFF C453DE00    MOV DWORD PTR SS:[EBP-0x140],PathOfEx.00DE53C4
	$-7A     006D9781          E8 6B0D5900               CALL PathOfEx.00C6A4F1
	$-75     006D9786          8B45 30                   MOV EAX,DWORD PTR SS:[EBP+0x30]
	$-72     006D9789          83C4 04                   ADD ESP,0x4
	$-6F     006D978C          C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$-6B     006D9790          8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-68     006D9793          8A80 DE000000             MOV AL,BYTE PTR DS:[EAX+0xDE]
	$-62     006D9799          84C0                      TEST AL,AL
	$-60     006D979B          74 36                     JE SHORT PathOfEx.006D97D3
	$-5E     006D979D          8D45 1C                   LEA EAX,DWORD PTR SS:[EBP+0x1C]
	$-5B     006D97A0          8BCB                      MOV ECX,EBX
	$-59     006D97A2          50                        PUSH EAX
	$-58     006D97A3          8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$-56     006D97A5          FF50 0C                   CALL DWORD PTR DS:[EAX+0xC]
	$-53     006D97A8          8BC8                      MOV ECX,EAX
	$-51     006D97AA          E8 61431000               CALL PathOfEx.007DDB10
	$-4C     006D97AF          50                        PUSH EAX
	$-4B     006D97B0          8D8B F8010000             LEA ECX,DWORD PTR DS:[EBX+0x1F8]
	$-45     006D97B6          E8 D5470000               CALL PathOfEx.006DDF90
	$-40     006D97BB          8B4D 1C                   MOV ECX,DWORD PTR SS:[EBP+0x1C]
	$-3D     006D97BE          85C9                      TEST ECX,ECX
	$-3B     006D97C0          74 11                     JE SHORT PathOfEx.006D97D3
	$-39     006D97C2          8B45 24                   MOV EAX,DWORD PTR SS:[EBP+0x24]
	$-36     006D97C5          2BC1                      SUB EAX,ECX
	$-34     006D97C7          6A 01                     PUSH 0x1
	$-32     006D97C9          50                        PUSH EAX
	$-31     006D97CA          51                        PUSH ECX
	$-30     006D97CB          E8 6030D9FF               CALL PathOfEx.0046C830
	$-2B     006D97D0          83C4 0C                   ADD ESP,0xC
	$-28     006D97D3          8D4D B8                   LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-25     006D97D6          E8 256F0100               CALL PathOfEx.006F0700
	$-20     006D97DB          8D4D B8                   LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-1D     006D97DE          E8 9D7D0100               CALL PathOfEx.006F1580
	$-18     006D97E3          FFB3 04040000             PUSH DWORD PTR DS:[EBX+0x404]
	$-12     006D97E9          8D4D B8                   LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-F      006D97EC          E8 9F830100               CALL PathOfEx.006F1B90
	$-A      006D97F1          8B8D E8FDFFFF             MOV ECX,DWORD PTR SS:[EBP-0x218]
	$-4      006D97F7          8D45 20                   LEA EAX,DWORD PTR SS:[EBP+0x20]
	$-1      006D97FA          50                        PUSH EAX
	$ ==>    006D97FB          E8 7042E5FF               CALL PathOfEx.0052DA70                               ; call tag_通过Meata地形数据结构查找TblItemEnvironmentsPair
	$+5      006D9800          8BF0                      MOV ESI,EAX
	$+7      006D9802          81C3 B8020000             ADD EBX,0x2B8
	$+D      006D9808          3BDE                      CMP EBX,ESI
	$+F      006D980A          74 23                     JE SHORT PathOfEx.006D982F
	$+11     006D980C          8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+13     006D980E          85C0                      TEST EAX,EAX
	$+15     006D9810          74 13                     JE SHORT PathOfEx.006D9825
	$+17     006D9812          50                        PUSH EAX
	$+18     006D9813          E8 E8E70300               CALL PathOfEx.00718000
	$+1D     006D9818          8BC8                      MOV ECX,EAX
	$+1F     006D981A          E8 71E30300               CALL PathOfEx.00717B90
	$+24     006D981F          C703 00000000             MOV DWORD PTR DS:[EBX],0x0
	$+2A     006D9825          8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+2C     006D9827          8903                      MOV DWORD PTR DS:[EBX],EAX
	$+2E     006D9829          C706 00000000             MOV DWORD PTR DS:[ESI],0x0
	$+34     006D982F          8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+37     006D9832          8943 04                   MOV DWORD PTR DS:[EBX+0x4],EAX
	$+3A     006D9835          C645 FC 0E                MOV BYTE PTR SS:[EBP-0x4],0xE
	$+3E     006D9839          8B45 20                   MOV EAX,DWORD PTR SS:[EBP+0x20]
	$+41     006D983C          85C0                      TEST EAX,EAX
	$+43     006D983E          74 0D                     JE SHORT PathOfEx.006D984D
	$+45     006D9840          50                        PUSH EAX
	$+46     006D9841          E8 BAE70300               CALL PathOfEx.00718000
	$+4B     006D9846          8BC8                      MOV ECX,EAX
	$+4D     006D9848          E8 43E30300               CALL PathOfEx.00717B90
	$+52     006D984D          8B4D 4C                   MOV ECX,DWORD PTR SS:[EBP+0x4C]
	$+55     006D9850          C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+59     006D9854          E8 2756F9FF               CALL PathOfEx.0066EE80
	$+5E     006D9859          8D4D B8                   LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+61     006D985C          E8 AFFAFFFF               CALL PathOfEx.006D9310
	$+66     006D9861          8D8D CCFDFFFF             LEA ECX,DWORD PTR SS:[EBP-0x234]
	$+6C     006D9867          E8 74000000               CALL PathOfEx.006D98E0
	$+71     006D986C          8B4D 3C                   MOV ECX,DWORD PTR SS:[EBP+0x3C]
	$+74     006D986F          85C9                      TEST ECX,ECX
	$+76     006D9871          74 29                     JE SHORT PathOfEx.006D989C
	$+78     006D9873          8B45 44                   MOV EAX,DWORD PTR SS:[EBP+0x44]
	$+7B     006D9876          2BC1                      SUB EAX,ECX
	$+7D     006D9878          6A 08                     PUSH 0x8
	$+7F     006D987A          C1F8 03                   SAR EAX,0x3
	$+82     006D987D          50                        PUSH EAX
	$+83     006D987E          51                        PUSH ECX
	$+84     006D987F          E8 AC2FD9FF               CALL PathOfEx.0046C830
	$+89     006D9884          83C4 0C                   ADD ESP,0xC
	$+8C     006D9887          C745 3C 00000000          MOV DWORD PTR SS:[EBP+0x3C],0x0
	$+93     006D988E          C745 40 00000000          MOV DWORD PTR SS:[EBP+0x40],0x0
	$+9A     006D9895          C745 44 00000000          MOV DWORD PTR SS:[EBP+0x44],0x0
	$+A1     006D989C          8B4D 50                   MOV ECX,DWORD PTR SS:[EBP+0x50]
	$+A4     006D989F          C745 38 38BBE700          MOV DWORD PTR SS:[EBP+0x38],PathOfEx.00E7BB38        ; ASCII "0yo"
	$+AB     006D98A6          85C9                      TEST ECX,ECX
	$+AD     006D98A8          74 14                     JE SHORT PathOfEx.006D98BE
	$+AF     006D98AA          8B45 58                   MOV EAX,DWORD PTR SS:[EBP+0x58]
	$+B2     006D98AD          2BC1                      SUB EAX,ECX
	$+B4     006D98AF          6A 04                     PUSH 0x4
	$+B6     006D98B1          C1F8 02                   SAR EAX,0x2
	$+B9     006D98B4          50                        PUSH EAX
	$+BA     006D98B5          51                        PUSH ECX
	$+BB     006D98B6          E8 752FD9FF               CALL PathOfEx.0046C830
	$+C0     006D98BB          83C4 0C                   ADD ESP,0xC
	$+C3     006D98BE          8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+C6     006D98C1          5F                        POP EDI
	$+C7     006D98C2          5E                        POP ESI
	$+C8     006D98C3          64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+CF     006D98CA          5B                        POP EBX
	$+D0     006D98CB          8D65 28                   LEA ESP,DWORD PTR SS:[EBP+0x28]
	$+D3     006D98CE          5D                        POP EBP
	$+D4     006D98CF          C2 2C00                   RETN 0x2C
	$+D7     006D98D2          CC                        INT3
	$+D8     006D98D3          CC                        INT3
	$+D9     006D98D4          CC                        INT3
	$+DA     006D98D5          CC                        INT3
	$+DB     006D98D6          CC                        INT3
	$+DC     006D98D7          CC                        INT3
	$+DD     006D98D8          CC                        INT3
	$+DE     006D98D9          CC                        INT3
	$+DF     006D98DA          CC                        INT3
	$+E0     006D98DB          CC                        INT3
	$+E1     006D98DC          CC                        INT3
	$+E2     006D98DD          CC                        INT3
	$+E3     006D98DE          CC                        INT3
	$+E4     006D98DF          CC                        INT3
	$+E5     006D98E0          55                        PUSH EBP
	$+E6     006D98E1          8BEC                      MOV EBP,ESP
	$+E8     006D98E3          6A FF                     PUSH -0x1
	$+EA     006D98E5          68 5C18D200               PUSH PathOfEx.00D2185C
	$+EF     006D98EA          64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+F5     006D98F0          50                        PUSH EAX
	$+F6     006D98F1          64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+FD     006D98F8          51                        PUSH ECX
	$+FE     006D98F9          56                        PUSH ESI
	$+FF     006D98FA          8BF1                      MOV ESI,ECX
	$+101    006D98FC          8B8E 80000000             MOV ECX,DWORD PTR DS:[ESI+0x80]
	$+107    006D9902          85C9                      TEST ECX,ECX
	$+109    006D9904          74 42                     JE SHORT PathOfEx.006D9948
	$+10B    006D9906          8B96 88000000             MOV EDX,DWORD PTR DS:[ESI+0x88]
	$+111    006D990C          B8 ABAAAA2A               MOV EAX,0x2AAAAAAB
	$+116    006D9911          2BD1                      SUB EDX,ECX
	$+118    006D9913          F7EA                      IMUL EDX
	$+11A    006D9915          6A 0C                     PUSH 0xC
	$+11C    006D9917          D1FA                      SAR EDX,1
	$+11E    006D9919          8BC2                      MOV EAX,EDX
	$+120    006D991B          C1E8 1F                   SHR EAX,0x1F
	$+123    006D991E          03C2                      ADD EAX,EDX
	$+125    006D9920          50                        PUSH EAX
	$+126    006D9921          51                        PUSH ECX
	$+127    006D9922          E8 092FD9FF               CALL PathOfEx.0046C830
	$+12C    006D9927          C786 80000000 00000000    MOV DWORD PTR DS:[ESI+0x80],0x0
	$+136    006D9931          83C4 0C                   ADD ESP,0xC
	$+139    006D9934          C786 84000000 00000000    MOV DWORD PTR DS:[ESI+0x84],0x0
	$+143    006D993E          C786 88000000 00000000    MOV DWORD PTR DS:[ESI+0x88],0x0
	$+14D    006D9948          8B4E 74                   MOV ECX,DWORD PTR DS:[ESI+0x74]
	$+150    006D994B          85C9                      TEST ECX,ECX
	$+152    006D994D          74 38                     JE SHORT PathOfEx.006D9987
	$+154    006D994F          FF75 F0                   PUSH DWORD PTR SS:[EBP-0x10]
	$+157    006D9952          8B56 78                   MOV EDX,DWORD PTR DS:[ESI+0x78]
	$+15A    006D9955          51                        PUSH ECX
	$+15B    006D9956          E8 65BFDAFF               CALL PathOfEx.004858C0
	$+160    006D995B          8B4E 74                   MOV ECX,DWORD PTR DS:[ESI+0x74]
	$+163    006D995E          8B46 7C                   MOV EAX,DWORD PTR DS:[ESI+0x7C]
	$+166    006D9961          2BC1                      SUB EAX,ECX
	$+168    006D9963          C1F8 03                   SAR EAX,0x3
	$+16B    006D9966          6A 08                     PUSH 0x8
	$+16D    006D9968          50                        PUSH EAX
	$+16E    006D9969          51                        PUSH ECX
	$+16F    006D996A          E8 C12ED9FF               CALL PathOfEx.0046C830
	$+174    006D996F          C746 74 00000000          MOV DWORD PTR DS:[ESI+0x74],0x0
	$+17B    006D9976          83C4 14                   ADD ESP,0x14
	$+17E    006D9979          C746 78 00000000          MOV DWORD PTR DS:[ESI+0x78],0x0
	$+185    006D9980          C746 7C 00000000          MOV DWORD PTR DS:[ESI+0x7C],0x0
	$+18C    006D9987          8B4E 68                   MOV ECX,DWORD PTR DS:[ESI+0x68]
	$+18F    006D998A          85C9                      TEST ECX,ECX
	$+191    006D998C          74 38                     JE SHORT PathOfEx.006D99C6
	$+193    006D998E          FF75 F0                   PUSH DWORD PTR SS:[EBP-0x10]
	$+196    006D9991          8B56 6C                   MOV EDX,DWORD PTR DS:[ESI+0x6C]
	$+199    006D9994          51                        PUSH ECX
	$+19A    006D9995          E8 26BFDAFF               CALL PathOfEx.004858C0
	$+19F    006D999A          8B4E 68                   MOV ECX,DWORD PTR DS:[ESI+0x68]
	$+1A2    006D999D          8B46 70                   MOV EAX,DWORD PTR DS:[ESI+0x70]
	$+1A5    006D99A0          2BC1                      SUB EAX,ECX
	$+1A7    006D99A2          C1F8 03                   SAR EAX,0x3
	$+1AA    006D99A5          6A 08                     PUSH 0x8
	$+1AC    006D99A7          50                        PUSH EAX
	$+1AD    006D99A8          51                        PUSH ECX
	$+1AE    006D99A9          E8 822ED9FF               CALL PathOfEx.0046C830
	$+1B3    006D99AE          C746 68 00000000          MOV DWORD PTR DS:[ESI+0x68],0x0
	$+1BA    006D99B5          83C4 14                   ADD ESP,0x14
	$+1BD    006D99B8          C746 6C 00000000          MOV DWORD PTR DS:[ESI+0x6C],0x0
	$+1C4    006D99BF          C746 70 00000000          MOV DWORD PTR DS:[ESI+0x70],0x0
	$+1CB    006D99C6          8B4E 5C                   MOV ECX,DWORD PTR DS:[ESI+0x5C]
	$+1CE    006D99C9          85C9                      TEST ECX,ECX
	$+1D0    006D99CB          74 29                     JE SHORT PathOfEx.006D99F6
	$+1D2    006D99CD          8B46 64                   MOV EAX,DWORD PTR DS:[ESI+0x64]
	$+1D5    006D99D0          2BC1                      SUB EAX,ECX
	$+1D7    006D99D2          C1F8 02                   SAR EAX,0x2
	$+1DA    006D99D5          6A 04                     PUSH 0x4
	$+1DC    006D99D7          50                        PUSH EAX
	$+1DD    006D99D8          51                        PUSH ECX
	$+1DE    006D99D9          E8 522ED9FF               CALL PathOfEx.0046C830
	$+1E3    006D99DE          C746 5C 00000000          MOV DWORD PTR DS:[ESI+0x5C],0x0
	$+1EA    006D99E5          83C4 0C                   ADD ESP,0xC
	$+1ED    006D99E8          C746 60 00000000          MOV DWORD PTR DS:[ESI+0x60],0x0
	$+1F4    006D99EF          C746 64 00000000          MOV DWORD PTR DS:[ESI+0x64],0x0
	$+1FB    006D99F6          8B4E 50                   MOV ECX,DWORD PTR DS:[ESI+0x50]
	$+1FE    006D99F9          85C9                      TEST ECX,ECX
	$+200    006D99FB          74 35                     JE SHORT PathOfEx.006D9A32
	$+202    006D99FD          8B56 54                   MOV EDX,DWORD PTR DS:[ESI+0x54]
	$+205    006D9A00          51                        PUSH ECX
	$+206    006D9A01          E8 5A480000               CALL PathOfEx.006DE260
	$+20B    006D9A06          8B4E 50                   MOV ECX,DWORD PTR DS:[ESI+0x50]
	$+20E    006D9A09          8B46 58                   MOV EAX,DWORD PTR DS:[ESI+0x58]
	$+211    006D9A0C          2BC1                      SUB EAX,ECX
	$+213    006D9A0E          C1F8 02                   SAR EAX,0x2
	$+216    006D9A11          6A 04                     PUSH 0x4
	$+218    006D9A13          50                        PUSH EAX
	$+219    006D9A14          51                        PUSH ECX
	$+21A    006D9A15          E8 162ED9FF               CALL PathOfEx.0046C830
	$+21F    006D9A1A          C746 50 00000000          MOV DWORD PTR DS:[ESI+0x50],0x0
	$+226    006D9A21          83C4 10                   ADD ESP,0x10
	$+229    006D9A24          C746 54 00000000          MOV DWORD PTR DS:[ESI+0x54],0x0
	$+230    006D9A2B          C746 58 00000000          MOV DWORD PTR DS:[ESI+0x58],0x0
	$+237    006D9A32          8B4E 44                   MOV ECX,DWORD PTR DS:[ESI+0x44]
	$+23A    006D9A35          85C9                      TEST ECX,ECX
	$+23C    006D9A37          74 29                     JE SHORT PathOfEx.006D9A62
	$+23E    006D9A39          8B46 4C                   MOV EAX,DWORD PTR DS:[ESI+0x4C]
	$+241    006D9A3C          2BC1                      SUB EAX,ECX
	$+243    006D9A3E          C1F8 04                   SAR EAX,0x4
	$+246    006D9A41          6A 10                     PUSH 0x10
	$+248    006D9A43          50                        PUSH EAX
	$+249    006D9A44          51                        PUSH ECX
	$+24A    006D9A45          E8 E62DD9FF               CALL PathOfEx.0046C830
	$+24F    006D9A4A          C746 44 00000000          MOV DWORD PTR DS:[ESI+0x44],0x0
	$+256    006D9A51          83C4 0C                   ADD ESP,0xC
	$+259    006D9A54          C746 48 00000000          MOV DWORD PTR DS:[ESI+0x48],0x0
	$+260    006D9A5B          C746 4C 00000000          MOV DWORD PTR DS:[ESI+0x4C],0x0
	$+267    006D9A62          8B4E 38                   MOV ECX,DWORD PTR DS:[ESI+0x38]
	$+26A    006D9A65          85C9                      TEST ECX,ECX
	$+26C    006D9A67          74 35                     JE SHORT PathOfEx.006D9A9E
	$+26E    006D9A69          8B56 3C                   MOV EDX,DWORD PTR DS:[ESI+0x3C]
	$+271    006D9A6C          51                        PUSH ECX
	$+272    006D9A6D          E8 DE8EDCFF               CALL PathOfEx.004A2950
	$+277    006D9A72          8B4E 38                   MOV ECX,DWORD PTR DS:[ESI+0x38]
	$+27A    006D9A75          8B46 40                   MOV EAX,DWORD PTR DS:[ESI+0x40]
	$+27D    006D9A78          2BC1                      SUB EAX,ECX
	$+27F    006D9A7A          C1F8 03                   SAR EAX,0x3
	$+282    006D9A7D          6A 08                     PUSH 0x8
	$+284    006D9A7F          50                        PUSH EAX
	*/
	/*addr_tmp = scan_exe_.ScanCall("e8????????8b??81??????????3b??74??8b??85??74??50e8", 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	addr_tmp = (pt_csz)addr_tmp + 0x2a;
	if (AsWord(addr_tmp, 0) != 0x4889)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_modify_game_env_id_at_change_map_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;*/

	/*
	$-32C    00896CCF     CC                        INT3
	$-32B    00896CD0     55                        PUSH EBP
	$-32A    00896CD1     8BEC                      MOV EBP,ESP
	$-328    00896CD3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-322    00896CD9     6A FF                     PUSH -0x1
	$-320    00896CDB     68 E897D200               PUSH PathOfEx.00D297E8
	$-31B    00896CE0     50                        PUSH EAX
	$-31A    00896CE1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-313    00896CE8     83EC 20                   SUB ESP,0x20
	$-310    00896CEB     53                        PUSH EBX
	$-30F    00896CEC     8B5D 08                   MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-30C    00896CEF     56                        PUSH ESI
	$-30B    00896CF0     57                        PUSH EDI
	$-30A    00896CF1     8BF9                      MOV EDI,ECX
	$-308    00896CF3     8B0B                      MOV ECX,DWORD PTR DS:[EBX]
	$-306    00896CF5     85C9                      TEST ECX,ECX
	$-304    00896CF7     74 05                     JE SHORT PathOfEx.00896CFE
	$-302    00896CF9     E8 12F8E3FF               CALL PathOfEx.006D6510
	$-2FD    00896CFE     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$-2FB    00896D00     C745 08 789CEF00          MOV DWORD PTR SS:[EBP+0x8],PathOfEx.00EF9C78      ; ASCII "Player"
	$-2F4    00896D07     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-2F1    00896D0A     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-2EE    00896D0D     50                        PUSH EAX
	$-2ED    00896D0E     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-2EA    00896D11     50                        PUSH EAX
	$-2E9    00896D12     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-2E6    00896D15     E8 A61BBAFF               CALL PathOfEx.004388C0
	$-2E1    00896D1A     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-2DE    00896D1D     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-2DB    00896D20     0F84 C2000000             JE PathOfEx.00896DE8
	$-2D5    00896D26     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-2D2    00896D29     83F9 FF                   CMP ECX,-0x1
	$-2CF    00896D2C     0F84 B6000000             JE PathOfEx.00896DE8
	$-2C9    00896D32     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-2C6    00896D35     8B3488                    MOV ESI,DWORD PTR DS:[EAX+ECX*4]
	$-2C3    00896D38     85F6                      TEST ESI,ESI
	$-2C1    00896D3A     0F84 A8000000             JE PathOfEx.00896DE8
	$-2BB    00896D40     8B43 14                   MOV EAX,DWORD PTR DS:[EBX+0x14]
	$-2B8    00896D43     3B87 AC3A0000             CMP EAX,DWORD PTR DS:[EDI+0x3AAC]
	$-2B2    00896D49     0F84 D1000000             JE PathOfEx.00896E20
	$-2AC    00896D4F     33C9                      XOR ECX,ECX
	$-2AA    00896D51     C745 E8 07000000          MOV DWORD PTR SS:[EBP-0x18],0x7
	$-2A3    00896D58     6A FF                     PUSH -0x1
	$-2A1    00896D5A     51                        PUSH ECX
	$-2A0    00896D5B     8D46 10                   LEA EAX,DWORD PTR DS:[ESI+0x10]
	$-29D    00896D5E     66:894D D4                MOV WORD PTR SS:[EBP-0x2C],CX
	$-299    00896D62     50                        PUSH EAX
	$-298    00896D63     8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-295    00896D66     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-28E    00896D6D     E8 AE33BAFF               CALL PathOfEx.0043A120
	$-289    00896D72     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-282    00896D79     8D45 D4                   LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-27F    00896D7C     8B97 543A0000             MOV EDX,DWORD PTR DS:[EDI+0x3A54]
	$-279    00896D82     8B8F 503A0000             MOV ECX,DWORD PTR DS:[EDI+0x3A50]
	$-273    00896D88     8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$-270    00896D8B     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-26D    00896D8E     50                        PUSH EAX
	$-26C    00896D8F     E8 2C510000               CALL PathOfEx.0089BEC0
	$-267    00896D94     83C4 04                   ADD ESP,0x4
	$-264    00896D97     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$-261    00896D9A     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-25E    00896D9D     8975 EC                   MOV DWORD PTR SS:[EBP-0x14],ESI
	$-25B    00896DA0     8D8F C43A0000             LEA ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-255    00896DA6     50                        PUSH EAX
	$-254    00896DA7     E8 D474C8FF               CALL PathOfEx.0051E280
	$-24F    00896DAC     A1 603A1801               MOV EAX,DWORD PTR DS:[0x1183A60]
	$-24A    00896DB1     85C0                      TEST EAX,EAX
	$-248    00896DB3     74 22                     JE SHORT PathOfEx.00896DD7
	$-246    00896DB5     8B8F C83A0000             MOV ECX,DWORD PTR DS:[EDI+0x3AC8]
	$-240    00896DBB     8D50 18                   LEA EDX,DWORD PTR DS:[EAX+0x18]
	$-23D    00896DBE     2B8F C43A0000             SUB ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-237    00896DC4     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-234    00896DC7     C1F9 03                   SAR ECX,0x3
	$-231    00896DCA     41                        INC ECX
	$-230    00896DCB     390A                      CMP DWORD PTR DS:[EDX],ECX
	$-22E    00896DCD     894D 08                   MOV DWORD PTR SS:[EBP+0x8],ECX
	$-22B    00896DD0     0F43C2                    CMOVNB EAX,EDX
	$-228    00896DD3     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$-226    00896DD5     8902                      MOV DWORD PTR DS:[EDX],EAX
	$-224    00896DD7     8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-221    00896DDA     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-21A    00896DE1     E8 FA31BAFF               CALL PathOfEx.00439FE0
	$-215    00896DE6     EB 0C                     JMP SHORT PathOfEx.00896DF4
	$-213    00896DE8     53                        PUSH EBX
	$-212    00896DE9     8D8F D03A0000             LEA ECX,DWORD PTR DS:[EDI+0x3AD0]
	$-20C    00896DEF     E8 5C1A0100               CALL PathOfEx.008A8850
	$-207    00896DF4     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$-205    00896DF6     3B87 B43A0000             CMP EAX,DWORD PTR DS:[EDI+0x3AB4]
	$-1FF    00896DFC     75 22                     JNZ SHORT PathOfEx.00896E20
	$-1FD    00896DFE     8B8F 8C380000             MOV ECX,DWORD PTR DS:[EDI+0x388C]
	$-1F7    00896E04     8D97 483D0000             LEA EDX,DWORD PTR DS:[EDI+0x3D48]
	$-1F1    00896E0A     83C1 04                   ADD ECX,0x4
	$-1EE    00896E0D     E8 DEE2F3FF               CALL PathOfEx.007D50F0
	$-1E9    00896E12     84C0                      TEST AL,AL
	$-1E7    00896E14     74 0A                     JE SHORT PathOfEx.00896E20
	$-1E5    00896E16     6A 00                     PUSH 0x0
	$-1E3    00896E18     8D4F F8                   LEA ECX,DWORD PTR DS:[EDI-0x8]
	$-1E0    00896E1B     E8 301D0000               CALL PathOfEx.00898B50
	$-1DB    00896E20     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1D8    00896E23     5F                        POP EDI
	$-1D7    00896E24     5E                        POP ESI
	$-1D6    00896E25     5B                        POP EBX
	$-1D5    00896E26     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1CE    00896E2D     8BE5                      MOV ESP,EBP
	$-1CC    00896E2F     5D                        POP EBP
	$-1CB    00896E30     C2 0400                   RETN 0x4
	$-1C8    00896E33     CC                        INT3
	$-1C7    00896E34     CC                        INT3
	$-1C6    00896E35     CC                        INT3
	$-1C5    00896E36     CC                        INT3
	$-1C4    00896E37     CC                        INT3
	$-1C3    00896E38     CC                        INT3
	$-1C2    00896E39     CC                        INT3
	$-1C1    00896E3A     CC                        INT3
	$-1C0    00896E3B     CC                        INT3
	$-1BF    00896E3C     CC                        INT3
	$-1BE    00896E3D     CC                        INT3
	$-1BD    00896E3E     CC                        INT3
	$-1BC    00896E3F     CC                        INT3
	$-1BB    00896E40     83EC 08                   SUB ESP,0x8
	$-1B8    00896E43     53                        PUSH EBX
	$-1B7    00896E44     56                        PUSH ESI
	$-1B6    00896E45     57                        PUSH EDI
	$-1B5    00896E46     8B7C24 18                 MOV EDI,DWORD PTR SS:[ESP+0x18]
	$-1B1    00896E4A     8BD9                      MOV EBX,ECX
	$-1AF    00896E4C     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
	$-1AD    00896E4E     85C9                      TEST ECX,ECX
	$-1AB    00896E50     74 05                     JE SHORT PathOfEx.00896E57
	$-1A9    00896E52     E8 B9F6E3FF               CALL PathOfEx.006D6510
	$-1A4    00896E57     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$-1A2    00896E59     C74424 18 789CEF00        MOV DWORD PTR SS:[ESP+0x18],PathOfEx.00EF9C78     ; ASCII "Player"
	$-19A    00896E61     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-197    00896E64     8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-193    00896E68     50                        PUSH EAX
	$-192    00896E69     8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-18E    00896E6D     50                        PUSH EAX
	$-18D    00896E6E     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-18A    00896E71     E8 4A1ABAFF               CALL PathOfEx.004388C0
	$-185    00896E76     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-181    00896E7A     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-17E    00896E7D     74 52                     JE SHORT PathOfEx.00896ED1
	$-17C    00896E7F     8B70 0C                   MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-179    00896E82     83FE FF                   CMP ESI,-0x1
	$-176    00896E85     74 4A                     JE SHORT PathOfEx.00896ED1
	$-174    00896E87     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-171    00896E8A     8B34B0                    MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-16E    00896E8D     897424 10                 MOV DWORD PTR SS:[ESP+0x10],ESI
	$-16A    00896E91     85F6                      TEST ESI,ESI
	$-168    00896E93     74 44                     JE SHORT PathOfEx.00896ED9
	$-166    00896E95     8B47 14                   MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-163    00896E98     3B83 AC3A0000             CMP EAX,DWORD PTR DS:[EBX+0x3AAC]
	$-15D    00896E9E     74 45                     JE SHORT PathOfEx.00896EE5
	$-15B    00896EA0     8B83 C83A0000             MOV EAX,DWORD PTR DS:[EBX+0x3AC8]
	$-155    00896EA6     8D8B C43A0000             LEA ECX,DWORD PTR DS:[EBX+0x3AC4]
	$-14F    00896EAC     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$-14D    00896EAE     3BD0                      CMP EDX,EAX
	$-14B    00896EB0     74 0B                     JE SHORT PathOfEx.00896EBD
	$-149    00896EB2     3932                      CMP DWORD PTR DS:[EDX],ESI
	$-147    00896EB4     74 07                     JE SHORT PathOfEx.00896EBD
	$-145    00896EB6     83C2 08                   ADD EDX,0x8
	$-142    00896EB9     3BD0                      CMP EDX,EAX
	$-140    00896EBB   ^ 75 F5                     JNZ SHORT PathOfEx.00896EB2
	$-13E    00896EBD     52                        PUSH EDX
	$-13D    00896EBE     8D4424 1C                 LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-139    00896EC2     50                        PUSH EAX
	$-138    00896EC3     E8 982F0000               CALL PathOfEx.00899E60
	$-133    00896EC8     5F                        POP EDI
	$-132    00896EC9     5E                        POP ESI
	$-131    00896ECA     5B                        POP EBX
	$-130    00896ECB     83C4 08                   ADD ESP,0x8
	$-12D    00896ECE     C2 0400                   RETN 0x4
	$-12A    00896ED1     C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
	$-122    00896ED9     57                        PUSH EDI
	$-121    00896EDA     8D8B D03A0000             LEA ECX,DWORD PTR DS:[EBX+0x3AD0]
	$-11B    00896EE0     E8 6B1C0100               CALL PathOfEx.008A8B50
	$-116    00896EE5     5F                        POP EDI
	$-115    00896EE6     5E                        POP ESI
	$-114    00896EE7     5B                        POP EBX
	$-113    00896EE8     83C4 08                   ADD ESP,0x8
	$-110    00896EEB     C2 0400                   RETN 0x4
	$-10D    00896EEE     CC                        INT3
	$-10C    00896EEF     CC                        INT3
	$-10B    00896EF0     8B81 88380000             MOV EAX,DWORD PTR DS:[ECX+0x3888]
	$-105    00896EF6     83B8 68010000 02          CMP DWORD PTR DS:[EAX+0x168],0x2
	$-FE     00896EFD     74 11                     JE SHORT PathOfEx.00896F10
	$-FC     00896EFF     FFB1 AC3A0000             PUSH DWORD PTR DS:[ECX+0x3AAC]
	$-F6     00896F05     81C1 2C390000             ADD ECX,0x392C
	$-F0     00896F0B     E8 F0F72000               CALL PathOfEx.00AA6700
	$-EB     00896F10     C3                        RETN
	$-EA     00896F11     CC                        INT3
	$-E9     00896F12     CC                        INT3
	$-E8     00896F13     CC                        INT3
	$-E7     00896F14     CC                        INT3
	$-E6     00896F15     CC                        INT3
	$-E5     00896F16     CC                        INT3
	$-E4     00896F17     CC                        INT3
	$-E3     00896F18     CC                        INT3
	$-E2     00896F19     CC                        INT3
	$-E1     00896F1A     CC                        INT3
	$-E0     00896F1B     CC                        INT3
	$-DF     00896F1C     CC                        INT3
	$-DE     00896F1D     CC                        INT3
	$-DD     00896F1E     CC                        INT3
	$-DC     00896F1F     CC                        INT3
	$-DB     00896F20     83B9 643D0000 02          CMP DWORD PTR DS:[ECX+0x3D64],0x2
	$-D4     00896F27     75 1D                     JNZ SHORT PathOfEx.00896F46
	$-D2     00896F29     0FB681 A83C0000           MOVZX EAX,BYTE PTR DS:[ECX+0x3CA8]
	$-CB     00896F30     C781 643D0000 00000000    MOV DWORD PTR DS:[ECX+0x3D64],0x0
	$-C1     00896F3A     81C1 88150000             ADD ECX,0x1588
	$-BB     00896F40     50                        PUSH EAX
	$-BA     00896F41     E8 FAA7BEFF               CALL PathOfEx.00481740
	$-B5     00896F46     C3                        RETN
	$-B4     00896F47     CC                        INT3
	$-B3     00896F48     CC                        INT3
	$-B2     00896F49     CC                        INT3
	$-B1     00896F4A     CC                        INT3
	$-B0     00896F4B     CC                        INT3
	$-AF     00896F4C     CC                        INT3
	$-AE     00896F4D     CC                        INT3
	$-AD     00896F4E     CC                        INT3
	$-AC     00896F4F     CC                        INT3
	$-AB     00896F50     55                        PUSH EBP                                          ; tag_根据最新的鼠标坐标来更新技能对象的坐标
	$-AA     00896F51     8BEC                      MOV EBP,ESP                                       ; ecx=stCD_GameLogic*+0xc
	$-A8     00896F53     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]                          ; p1=stCD_SkillObj*
	$-A2     00896F59     6A FF                     PUSH -0x1
	$-A0     00896F5B     68 90C8D600               PUSH PathOfEx.00D6C890
	$-9B     00896F60     50                        PUSH EAX
	$-9A     00896F61     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-93     00896F68     83EC 18                   SUB ESP,0x18
	$-90     00896F6B     57                        PUSH EDI
	$-8F     00896F6C     8BF9                      MOV EDI,ECX
	$-8D     00896F6E     8B87 88380000             MOV EAX,DWORD PTR DS:[EDI+0x3888]
	$-87     00896F74     83B8 68010000 02          CMP DWORD PTR DS:[EAX+0x168],0x2
	$-80     00896F7B     0F84 ED000000             JE PathOfEx.0089706E
	$-7A     00896F81     56                        PUSH ESI
	$-79     00896F82     8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-76     00896F85     8BCE                      MOV ECX,ESI
	$-74     00896F87     E8 D41C2500               CALL PathOfEx.00AE8C60
	$-6F     00896F8C     B9 00800000               MOV ECX,0x8000
	$-6A     00896F91     66:85C1                   TEST CX,AX
	$-67     00896F94     0F85 D3000000             JNZ PathOfEx.0089706D
	$-61     00896F9A     837E 1C 00                CMP DWORD PTR DS:[ESI+0x1C],0x0                   ; g_so_skill_obj_update_mouse_pos_
	$-5D     00896F9E     74 44                     JE SHORT PathOfEx.00896FE4
	$-5B     00896FA0     B8 12000000               MOV EAX,0x12
	$-56     00896FA5     C745 E0 5483F000          MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00F08354
	$-4F     00896FAC     66:8945 E4                MOV WORD PTR SS:[EBP-0x1C],AX
	$-4B     00896FB0     8BCE                      MOV ECX,ESI
	$-49     00896FB2     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-42     00896FB9     E8 F21F2500               CALL PathOfEx.00AE8FB0
	$-3D     00896FBE     8BCE                      MOV ECX,ESI
	$-3B     00896FC0     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$-37     00896FC4     E8 971C2500               CALL PathOfEx.00AE8C60
	$-32     00896FC9     66:8945 EE                MOV WORD PTR SS:[EBP-0x12],AX
	$-2E     00896FCD     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-2B     00896FD0     8A46 17                   MOV AL,BYTE PTR DS:[ESI+0x17]
	$-28     00896FD3     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$-25     00896FD6     8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$-22     00896FD9     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-1F     00896FDC     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-1C     00896FDF     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-19     00896FE2     EB 73                     JMP SHORT PathOfEx.00897057
	$-17     00896FE4     F3:0F1087 A43A0000        MOVSS XMM0,DWORD PTR DS:[EDI+0x3AA4]
	$-F      00896FEC     F3:0F5905 D808E800        MULSS XMM0,DWORD PTR DS:[0xE808D8]
	$-7      00896FF4     C745 DC 0C83F000          MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00F0830C
	$ ==>    00896FFB     F3:0F2CC8                 CVTTSS2SI ECX,XMM0                                ; 在此处hook,g_h_skill_obj_update_mouse_pos_
	$+4      00896FFF     F3:0F1087 A03A0000        MOVSS XMM0,DWORD PTR DS:[EDI+0x3AA0]
	$+C      00897007     F3:0F5905 D808E800        MULSS XMM0,DWORD PTR DS:[0xE808D8]
	$+14     0089700F     894E 24                   MOV DWORD PTR DS:[ESI+0x24],ECX                   ; 更新鼠标坐标y到技能对象中
	$+17     00897012     F3:0F2CC0                 CVTTSS2SI EAX,XMM0
	$+1B     00897016     8946 20                   MOV DWORD PTR DS:[ESI+0x20],EAX                   ; 更新鼠标坐标x到技能对象中
	$+1E     00897019     B8 13000000               MOV EAX,0x13                                      ; hook要跳转到此处
	$+23     0089701E     66:8945 E0                MOV WORD PTR SS:[EBP-0x20],AX
	$+27     00897022     8BCE                      MOV ECX,ESI
	$+29     00897024     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+30     0089702B     E8 801F2500               CALL PathOfEx.00AE8FB0
	$+35     00897030     8BCE                      MOV ECX,ESI
	$+37     00897032     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+3B     00897036     E8 251C2500               CALL PathOfEx.00AE8C60
	$+40     0089703B     66:8945 EE                MOV WORD PTR SS:[EBP-0x12],AX
	$+44     0089703F     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+47     00897042     8A46 17                   MOV AL,BYTE PTR DS:[ESI+0x17]
	$+4A     00897045     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+4D     00897048     8B46 20                   MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+50     0089704B     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+53     0089704E     8B46 24                   MOV EAX,DWORD PTR DS:[ESI+0x24]
	$+56     00897051     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+59     00897054     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+5C     00897057     FFB7 8C380000             PUSH DWORD PTR DS:[EDI+0x388C]
	$+62     0089705D     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$+65     00897060     8B8F 8C380000             MOV ECX,DWORD PTR DS:[EDI+0x388C]
	$+6B     00897066     6A 00                     PUSH 0x0
	$+6D     00897068     E8 73BAEBFF               CALL PathOfEx.00752AE0
	$+72     0089706D     5E                        POP ESI
	$+73     0089706E     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+76     00897071     5F                        POP EDI
	$+77     00897072     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+7E     00897079     8BE5                      MOV ESP,EBP
	$+80     0089707B     5D                        POP EBP
	$+81     0089707C     C2 0400                   RETN 0x4
	$+84     0089707F     CC                        INT3
	$+85     00897080     55                        PUSH EBP
	$+86     00897081     8BEC                      MOV EBP,ESP
	$+88     00897083     6A FF                     PUSH -0x1
	$+8A     00897085     68 7821D300               PUSH PathOfEx.00D32178
	$+8F     0089708A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+95     00897090     50                        PUSH EAX
	$+96     00897091     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+9D     00897098     83EC 0C                   SUB ESP,0xC
	$+A0     0089709B     B8 65000000               MOV EAX,0x65
	$+A5     008970A0     C745 E8 4883F000          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00F08348
	$+AC     008970A7     56                        PUSH ESI
	$+AD     008970A8     8BF1                      MOV ESI,ECX
	$+AF     008970AA     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+B3     008970AE     8A45 08                   MOV AL,BYTE PTR SS:[EBP+0x8]
	$+B6     008970B1     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+B9     008970B4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+C0     008970BB     FFB6 98380000             PUSH DWORD PTR DS:[ESI+0x3898]
	$+C6     008970C1     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+C9     008970C4     E8 F725FFFF               CALL PathOfEx.008896C0
	$+CE     008970C9     8B8E 98380000             MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+D4     008970CF     6A 00                     PUSH 0x0
	$+D6     008970D1     E8 0ABAEBFF               CALL PathOfEx.00752AE0
	$+DB     008970D6     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+DE     008970D9     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+E5     008970E0     5E                        POP ESI
	$+E6     008970E1     8BE5                      MOV ESP,EBP
	$+E8     008970E3     5D                        POP EBP
	$+E9     008970E4     C2 0400                   RETN 0x4
	$+EC     008970E7     CC                        INT3
	$+ED     008970E8     CC                        INT3
	$+EE     008970E9     CC                        INT3
	$+EF     008970EA     CC                        INT3
	$+F0     008970EB     CC                        INT3
	$+F1     008970EC     CC                        INT3
	$+F2     008970ED     CC                        INT3
	$+F3     008970EE     CC                        INT3
	$+F4     008970EF     CC                        INT3
	$+F5     008970F0     55                        PUSH EBP
	$+F6     008970F1     8BEC                      MOV EBP,ESP
	$+F8     008970F3     6A FF                     PUSH -0x1
	$+FA     008970F5     68 A8C8D600               PUSH PathOfEx.00D6C8A8
	$+FF     008970FA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+105    00897100     50                        PUSH EAX
	$+106    00897101     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+10D    00897108     83EC 44                   SUB ESP,0x44
	$+110    0089710B     B8 8C000000               MOV EAX,0x8C
	$+115    00897110     C745 F0 07000000          MOV DWORD PTR SS:[EBP-0x10],0x7
	$+11C    00897117     66:8945 B4                MOV WORD PTR SS:[EBP-0x4C],AX
	$+120    0089711B     BA A481F000               MOV EDX,PathOfEx.00F081A4
	$+125    00897120     33C0                      XOR EAX,EAX
	$+127    00897122     8955 B0                   MOV DWORD PTR SS:[EBP-0x50],EDX
	$+12A    00897125     56                        PUSH ESI
	$+12B    00897126     8BF1                      MOV ESI,ECX
	$+12D    00897128     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$+134    0089712F     66:8945 DC                MOV WORD PTR SS:[EBP-0x24],AX
	$+138    00897133     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+13B    00897136     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+13E    00897139     8A45 08                   MOV AL,BYTE PTR SS:[EBP+0x8]
	$+141    0089713C     8845 B8                   MOV BYTE PTR SS:[EBP-0x48],AL
	$+144    0089713F     8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+147    00897142     3BC8                      CMP ECX,EAX
	$+149    00897144     74 0D                     JE SHORT PathOfEx.00897153
	$+14B    00897146     6A FF                     PUSH -0x1
	$+14D    00897148     6A 00                     PUSH 0x0
	$+14F    0089714A     50                        PUSH EAX
	$+150    0089714B     E8 D02FBAFF               CALL PathOfEx.0043A120
	$+155    00897150     8B55 B0                   MOV EDX,DWORD PTR SS:[EBP-0x50]
	$+158    00897153     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+15B    00897156     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+15E    00897159     FFB6 98380000             PUSH DWORD PTR DS:[ESI+0x3898]
	$+164    0089715F     0F1000                    MOVUPS XMM0,DQWORD PTR DS:[EAX]
	$+167    00897162     0F1145 B9                 MOVUPS DQWORD PTR SS:[EBP-0x47],XMM0
	$+16B    00897166     0F1040 10                 MOVUPS XMM0,DQWORD PTR DS:[EAX+0x10]
	$+16F    0089716A     0F1145 C9                 MOVUPS DQWORD PTR SS:[EBP-0x37],XMM0
	$+173    0089716E     FF52 04                   CALL DWORD PTR DS:[EDX+0x4]
	$+176    00897171     8B8E 98380000             MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+17C    00897177     6A 00                     PUSH 0x0
	$+17E    00897179     E8 62B9EBFF               CALL PathOfEx.00752AE0
	$+183    0089717E     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+186    00897181     5E                        POP ESI
	$+187    00897182     83F8 08                   CMP EAX,0x8
	$+18A    00897185     72 0F                     JB SHORT PathOfEx.00897196
	$+18C    00897187     6A 02                     PUSH 0x2
	$+18E    00897189     40                        INC EAX
	$+18F    0089718A     50                        PUSH EAX
	$+190    0089718B     FF75 DC                   PUSH DWORD PTR SS:[EBP-0x24]
	$+193    0089718E     E8 1D1FBAFF               CALL PathOfEx.004390B0
	$+198    00897193     83C4 0C                   ADD ESP,0xC
	$+19B    00897196     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+19E    00897199     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+1A5    008971A0     8BE5                      MOV ESP,EBP
	$+1A7    008971A2     5D                        POP EBP
	$+1A8    008971A3     C2 0C00                   RETN 0xC
	$+1AB    008971A6     CC                        INT3
	$+1AC    008971A7     CC                        INT3
	$+1AD    008971A8     CC                        INT3
	$+1AE    008971A9     CC                        INT3
	$+1AF    008971AA     CC                        INT3
	$+1B0    008971AB     CC                        INT3
	$+1B1    008971AC     CC                        INT3
	$+1B2    008971AD     CC                        INT3
	$+1B3    008971AE     CC                        INT3
	$+1B4    008971AF     CC                        INT3
	$+1B5    008971B0     55                        PUSH EBP
	$+1B6    008971B1     8BEC                      MOV EBP,ESP
	$+1B8    008971B3     6A FF                     PUSH -0x1
	$+1BA    008971B5     68 7821D300               PUSH PathOfEx.00D32178
	$+1BF    008971BA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+1C5    008971C0     50                        PUSH EAX
	$+1C6    008971C1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+1CD    008971C8     83EC 0C                   SUB ESP,0xC
	$+1D0    008971CB     B8 5B000000               MOV EAX,0x5B
	$+1D5    008971D0     C745 E8 C483F000          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00F083C4
	$+1DC    008971D7     56                        PUSH ESI
	$+1DD    008971D8     8BF1                      MOV ESI,ECX
	$+1DF    008971DA     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+1E3    008971DE     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1E6    008971E1     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+1E9    008971E4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1F0    008971EB     FFB6 98380000             PUSH DWORD PTR DS:[ESI+0x3898]
	$+1F6    008971F1     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1F9    008971F4     E8 A71CFFFF               CALL PathOfEx.00888EA0
	$+1FE    008971F9     8B8E 98380000             MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+204    008971FF     6A 00                     PUSH 0x0
	$+206    00897201     E8 DAB8EBFF               CALL PathOfEx.00752AE0
	$+20B    00897206     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+20E    00897209     5E                        POP ESI
	$+20F    0089720A     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+216    00897211     8BE5                      MOV ESP,EBP
	$+218    00897213     5D                        POP EBP
	$+219    00897214     C2 0400                   RETN 0x4
	$+21C    00897217     CC                        INT3
	$+21D    00897218     CC                        INT3
	$+21E    00897219     CC                        INT3
	$+21F    0089721A     CC                        INT3
	$+220    0089721B     CC                        INT3
	$+221    0089721C     CC                        INT3
	$+222    0089721D     CC                        INT3
	$+223    0089721E     CC                        INT3
	$+224    0089721F     CC                        INT3
	$+225    00897220     55                        PUSH EBP
	$+226    00897221     8BEC                      MOV EBP,ESP
	$+228    00897223     6A FF                     PUSH -0x1
	$+22A    00897225     68 287CD300               PUSH PathOfEx.00D37C28
	$+22F    0089722A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+235    00897230     50                        PUSH EAX
	$+236    00897231     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+23D    00897238     83EC 10                   SUB ESP,0x10
	$+240    0089723B     B8 5C000000               MOV EAX,0x5C
	$+245    00897240     C745 E4 F881F000          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00F081F8
	$+24C    00897247     56                        PUSH ESI
	$+24D    00897248     8BF1                      MOV ESI,ECX
	$+24F    0089724A     66:8945 E8                MOV WORD PTR SS:[EBP-0x18],AX
	$+253    0089724E     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+256    00897251     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+259    00897254     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+260    0089725B     FFB6 98380000             PUSH DWORD PTR DS:[ESI+0x3898]
	$+266    00897261     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+269    00897264     8A45 0C                   MOV AL,BYTE PTR SS:[EBP+0xC]
	$+26C    00897267     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+26F    0089726A     E8 F13DFFFF               CALL PathOfEx.0088B060
	$+274    0089726F     8B8E 98380000             MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+27A    00897275     6A 00                     PUSH 0x0
	$+27C    00897277     E8 64B8EBFF               CALL PathOfEx.00752AE0
	$+281    0089727C     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+284    0089727F     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+28B    00897286     5E                        POP ESI
	$+28C    00897287     8BE5                      MOV ESP,EBP
	$+28E    00897289     5D                        POP EBP
	$+28F    0089728A     C2 0800                   RETN 0x8
	$+292    0089728D     CC                        INT3
	$+293    0089728E     CC                        INT3
	$+294    0089728F     CC                        INT3
	$+295    00897290     55                        PUSH EBP
	$+296    00897291     8BEC                      MOV EBP,ESP
	$+298    00897293     6A FF                     PUSH -0x1
	$+29A    00897295     68 58E8D200               PUSH PathOfEx.00D2E858
	$+29F    0089729A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+2A5    008972A0     50                        PUSH EAX
	$+2A6    008972A1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+2AD    008972A8     83EC 14                   SUB ESP,0x14
	$+2B0    008972AB     B8 5D000000               MOV EAX,0x5D
	$+2B5    008972B0     C745 E0 0482F000          MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00F08204
	$+2BC    008972B7     56                        PUSH ESI
	$+2BD    008972B8     8BF1                      MOV ESI,ECX
	$+2BF    008972BA     66:8945 E4                MOV WORD PTR SS:[EBP-0x1C],AX
	$+2C3    008972BE     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2C6    008972C1     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+2C9    008972C4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2D0    008972CB     FFB6 98380000             PUSH DWORD PTR DS:[ESI+0x3898]
	$+2D6    008972D1     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+2D9    008972D4     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2DC    008972D7     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+2DF    008972DA     66:C745 F0 FFFF           MOV WORD PTR SS:[EBP-0x10],0xFFFF
	$+2E5    008972E0     E8 0B3EFFFF               CALL PathOfEx.0088B0F0
	$+2EA    008972E5     8B8E 98380000             MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+2F0    008972EB     6A 00                     PUSH 0x0
	$+2F2    008972ED     E8 EEB7EBFF               CALL PathOfEx.00752AE0
	$+2F7    008972F2     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2FA    008972F5     5E                        POP ESI
	$+2FB    008972F6     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+302    008972FD     8BE5                      MOV ESP,EBP
	$+304    008972FF     5D                        POP EBP
	$+305    00897300     C2 0800                   RETN 0x8
	$+308    00897303     CC                        INT3
	*/

	//gj3
	/*
	$-1BC    0165053F     CC                        INT3
	$-1BB    01650540     83EC 08                   SUB ESP,0x8
	$-1B8    01650543     53                        PUSH EBX
	$-1B7    01650544     56                        PUSH ESI
	$-1B6    01650545     57                        PUSH EDI
	$-1B5    01650546     8B7C24 18                 MOV EDI,DWORD PTR SS:[ESP+0x18]
	$-1B1    0165054A     8BD9                      MOV EBX,ECX
	$-1AF    0165054C     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
	$-1AD    0165054E     85C9                      TEST ECX,ECX
	$-1AB    01650550     74 05                     JE SHORT PathOfEx.01650557
	$-1A9    01650552     E8 79F3E2FF               CALL PathOfEx.0147F8D0
	$-1A4    01650557     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$-1A2    01650559     C74424 18 F48CDC01        MOV DWORD PTR SS:[ESP+0x18],PathOfEx.01DC8CF4
	$-19A    01650561     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-197    01650564     8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-193    01650568     50                        PUSH EAX
	$-192    01650569     8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-18E    0165056D     50                        PUSH EAX
	$-18D    0165056E     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-18A    01650571     E8 0A46B4FF               CALL PathOfEx.01194B80
	$-185    01650576     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-181    0165057A     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-17E    0165057D     74 52                     JE SHORT PathOfEx.016505D1
	$-17C    0165057F     8B70 0C                   MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-179    01650582     83FE FF                   CMP ESI,-0x1
	$-176    01650585     74 4A                     JE SHORT PathOfEx.016505D1
	$-174    01650587     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-171    0165058A     8B34B0                    MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-16E    0165058D     897424 10                 MOV DWORD PTR SS:[ESP+0x10],ESI
	$-16A    01650591     85F6                      TEST ESI,ESI
	$-168    01650593     74 44                     JE SHORT PathOfEx.016505D9
	$-166    01650595     8B47 20                   MOV EAX,DWORD PTR DS:[EDI+0x20]
	$-163    01650598     3B83 2C430000             CMP EAX,DWORD PTR DS:[EBX+0x432C]
	$-15D    0165059E     74 45                     JE SHORT PathOfEx.016505E5
	$-15B    016505A0     8B83 48430000             MOV EAX,DWORD PTR DS:[EBX+0x4348]
	$-155    016505A6     8D8B 44430000             LEA ECX,DWORD PTR DS:[EBX+0x4344]
	$-14F    016505AC     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$-14D    016505AE     3BD0                      CMP EDX,EAX
	$-14B    016505B0     74 0B                     JE SHORT PathOfEx.016505BD
	$-149    016505B2     3932                      CMP DWORD PTR DS:[EDX],ESI
	$-147    016505B4     74 07                     JE SHORT PathOfEx.016505BD
	$-145    016505B6     83C2 08                   ADD EDX,0x8
	$-142    016505B9     3BD0                      CMP EDX,EAX
	$-140    016505BB   ^ 75 F5                     JNZ SHORT PathOfEx.016505B2
	$-13E    016505BD     52                        PUSH EDX
	$-13D    016505BE     8D4424 1C                 LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-139    016505C2     50                        PUSH EAX
	$-138    016505C3     E8 E8E0C0FF               CALL PathOfEx.0125E6B0
	$-133    016505C8     5F                        POP EDI
	$-132    016505C9     5E                        POP ESI
	$-131    016505CA     5B                        POP EBX
	$-130    016505CB     83C4 08                   ADD ESP,0x8
	$-12D    016505CE     C2 0400                   RETN 0x4
	$-12A    016505D1     C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
	$-122    016505D9     57                        PUSH EDI
	$-121    016505DA     8D8B 50430000             LEA ECX,DWORD PTR DS:[EBX+0x4350]
	$-11B    016505E0     E8 0B2A0100               CALL PathOfEx.01662FF0
	$-116    016505E5     5F                        POP EDI
	$-115    016505E6     5E                        POP ESI
	$-114    016505E7     5B                        POP EBX
	$-113    016505E8     83C4 08                   ADD ESP,0x8
	$-110    016505EB     C2 0400                   RETN 0x4
	$-10D    016505EE     CC                        INT3
	$-10C    016505EF     CC                        INT3                                            ; 这个函数一直会到，下层函数是执行当前技能的
	$-10B    016505F0     8B81 08410000             MOV EAX,DWORD PTR DS:[ECX+0x4108]               ; tag_一直运行当前技能对象
	$-105    016505F6     80B8 53010000 02          CMP BYTE PTR DS:[EAX+0x153],0x2                 ; ecx=stCD_GameLogic*+0xc
	$-FE     016505FD     74 11                     JE SHORT PathOfEx.01650610
	$-FC     016505FF     FFB1 2C430000             PUSH DWORD PTR DS:[ECX+0x432C]
	$-F6     01650605     81C1 AC410000             ADD ECX,0x41AC                                  ; g_o_cur_using_skill_info_
	$-F0     0165060B     E8 B0472A00               CALL PathOfEx.018F4DC0                          ; call tag_运行一次技能
	$-EB     01650610     C3                        RETN
	$-EA     01650611     CC                        INT3
	$-E9     01650612     CC                        INT3
	$-E8     01650613     CC                        INT3
	$-E7     01650614     CC                        INT3
	$-E6     01650615     CC                        INT3
	$-E5     01650616     CC                        INT3
	$-E4     01650617     CC                        INT3
	$-E3     01650618     CC                        INT3
	$-E2     01650619     CC                        INT3
	$-E1     0165061A     CC                        INT3
	$-E0     0165061B     CC                        INT3
	$-DF     0165061C     CC                        INT3
	$-DE     0165061D     CC                        INT3
	$-DD     0165061E     CC                        INT3
	$-DC     0165061F     CC                        INT3
	$-DB     01650620     83B9 E4450000 02          CMP DWORD PTR DS:[ECX+0x45E4],0x2
	$-D4     01650627     75 1D                     JNZ SHORT PathOfEx.01650646
	$-D2     01650629     0FB681 28450000           MOVZX EAX,BYTE PTR DS:[ECX+0x4528]
	$-CB     01650630     C781 E4450000 00000000    MOV DWORD PTR DS:[ECX+0x45E4],0x0
	$-C1     0165063A     81C1 88170000             ADD ECX,0x1788
	$-BB     01650640     50                        PUSH EAX
	$-BA     01650641     E8 DAF6B8FF               CALL PathOfEx.011DFD20
	$-B5     01650646     C3                        RETN
	$-B4     01650647     CC                        INT3
	$-B3     01650648     CC                        INT3
	$-B2     01650649     CC                        INT3
	$-B1     0165064A     CC                        INT3
	$-B0     0165064B     CC                        INT3
	$-AF     0165064C     CC                        INT3
	$-AE     0165064D     CC                        INT3
	$-AD     0165064E     CC                        INT3
	$-AC     0165064F     CC                        INT3
	$-AB     01650650     55                        PUSH EBP                                        ; tag_根据最新的鼠标坐标来更新技能对象的坐标
	$-AA     01650651     8BEC                      MOV EBP,ESP                                     ; ecx=stCD_GameLogic*+0xc
	$-A8     01650653     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]                        ; p1=stCD_SkillObj*
	$-A2     01650659     6A FF                     PUSH -0x1
	$-A0     0165065B     68 2074C101               PUSH PathOfEx.01C17420
	$-9B     01650660     50                        PUSH EAX
	$-9A     01650661     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-93     01650668     83EC 18                   SUB ESP,0x18
	$-90     0165066B     57                        PUSH EDI
	$-8F     0165066C     8BF9                      MOV EDI,ECX
	$-8D     0165066E     8B87 08410000             MOV EAX,DWORD PTR DS:[EDI+0x4108]
	$-87     01650674     80B8 53010000 02          CMP BYTE PTR DS:[EAX+0x153],0x2
	$-80     0165067B     0F84 ED000000             JE PathOfEx.0165076E
	$-7A     01650681     56                        PUSH ESI
	$-79     01650682     8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-76     01650685     8BCE                      MOV ECX,ESI
	$-74     01650687     E8 14962E00               CALL PathOfEx.01939CA0
	$-6F     0165068C     B9 00800000               MOV ECX,0x8000
	$-6A     01650691     66:85C1                   TEST CX,AX
	$-67     01650694     0F85 D3000000             JNZ PathOfEx.0165076D
	$-61     0165069A     837E 1C 00                CMP DWORD PTR DS:[ESI+0x1C],0x0                 ; g_so_skill_obj_update_mouse_pos_
	$-5D     0165069E     74 44                     JE SHORT PathOfEx.016506E4
	$-5B     016506A0     B8 13000000               MOV EAX,0x13
	$-56     016506A5     C745 E0 847FDD01          MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01DD7F84
	$-4F     016506AC     66:8945 E4                MOV WORD PTR SS:[EBP-0x1C],AX
	$-4B     016506B0     8BCE                      MOV ECX,ESI
	$-49     016506B2     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-42     016506B9     E8 32992E00               CALL PathOfEx.01939FF0
	$-3D     016506BE     8BCE                      MOV ECX,ESI
	$-3B     016506C0     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$-37     016506C4     E8 D7952E00               CALL PathOfEx.01939CA0
	$-32     016506C9     66:8945 EE                MOV WORD PTR SS:[EBP-0x12],AX
	$-2E     016506CD     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-2B     016506D0     8A46 17                   MOV AL,BYTE PTR DS:[ESI+0x17]
	$-28     016506D3     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$-25     016506D6     8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$-22     016506D9     8B40 20                   MOV EAX,DWORD PTR DS:[EAX+0x20]
	$-1F     016506DC     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-1C     016506DF     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-19     016506E2     EB 73                     JMP SHORT PathOfEx.01650757
	$-17     016506E4     F3:0F1087 24430000        MOVSS XMM0,DWORD PTR DS:[EDI+0x4324]
	$-F      016506EC     F3:0F5905 1C27D401        MULSS XMM0,DWORD PTR DS:[0x1D4271C]
	$-7      016506F4     C745 DC 787FDD01          MOV DWORD PTR SS:[EBP-0x24],PathOfEx.01DD7F78
	$ ==>    016506FB     F3:0F2CC8                 CVTTSS2SI ECX,XMM0                              ; 在此处hook,g_h_skill_obj_update_mouse_pos_
	$+4      016506FF     F3:0F1087 20430000        MOVSS XMM0,DWORD PTR DS:[EDI+0x4320]
	$+C      01650707     F3:0F5905 1C27D401        MULSS XMM0,DWORD PTR DS:[0x1D4271C]
	$+14     0165070F     894E 24                   MOV DWORD PTR DS:[ESI+0x24],ECX                 ; 更新鼠标坐标y到技能对象中
	$+17     01650712     F3:0F2CC0                 CVTTSS2SI EAX,XMM0
	$+1B     01650716     8946 20                   MOV DWORD PTR DS:[ESI+0x20],EAX                 ; 更新鼠标坐标x到技能对象中
	$+1E     01650719     B8 14000000               MOV EAX,0x14                                    ; hook要跳转到此处
	$+23     0165071E     66:8945 E0                MOV WORD PTR SS:[EBP-0x20],AX
	$+27     01650722     8BCE                      MOV ECX,ESI
	$+29     01650724     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+30     0165072B     E8 C0982E00               CALL PathOfEx.01939FF0
	$+35     01650730     8BCE                      MOV ECX,ESI
	$+37     01650732     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+3B     01650736     E8 65952E00               CALL PathOfEx.01939CA0
	$+40     0165073B     66:8945 EE                MOV WORD PTR SS:[EBP-0x12],AX
	$+44     0165073F     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+47     01650742     8A46 17                   MOV AL,BYTE PTR DS:[ESI+0x17]
	$+4A     01650745     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+4D     01650748     8B46 20                   MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+50     0165074B     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+53     0165074E     8B46 24                   MOV EAX,DWORD PTR DS:[ESI+0x24]
	$+56     01650751     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+59     01650754     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+5C     01650757     FFB7 0C410000             PUSH DWORD PTR DS:[EDI+0x410C]
	$+62     0165075D     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$+65     01650760     8B8F 0C410000             MOV ECX,DWORD PTR DS:[EDI+0x410C]
	$+6B     01650766     6A 00                     PUSH 0x0
	$+6D     01650768     E8 932AEBFF               CALL PathOfEx.01503200
	$+72     0165076D     5E                        POP ESI
	$+73     0165076E     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+76     01650771     5F                        POP EDI
	$+77     01650772     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+7E     01650779     8BE5                      MOV ESP,EBP
	$+80     0165077B     5D                        POP EBP
	$+81     0165077C     C2 0400                   RETN 0x4
	$+84     0165077F     CC                        INT3
	$+85     01650780     55                        PUSH EBP
	$+86     01650781     8BEC                      MOV EBP,ESP
	$+88     01650783     6A FF                     PUSH -0x1
	$+8A     01650785     68 B866BD01               PUSH PathOfEx.01BD66B8
	$+8F     0165078A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+95     01650790     50                        PUSH EAX
	$+96     01650791     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+9D     01650798     83EC 0C                   SUB ESP,0xC
	$+A0     0165079B     B8 67000000               MOV EAX,0x67
	$+A5     016507A0     C745 E8 3C7FDD01          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01DD7F3C
	$+AC     016507A7     56                        PUSH ESI
	$+AD     016507A8     8BF1                      MOV ESI,ECX
	$+AF     016507AA     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+B3     016507AE     8A45 08                   MOV AL,BYTE PTR SS:[EBP+0x8]
	$+B6     016507B1     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+B9     016507B4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+C0     016507BB     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+C6     016507C1     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+C9     016507C4     E8 271AFFFF               CALL PathOfEx.016421F0
	$+CE     016507C9     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+D4     016507CF     6A 00                     PUSH 0x0
	$+D6     016507D1     E8 2A2AEBFF               CALL PathOfEx.01503200
	$+DB     016507D6     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+DE     016507D9     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+E5     016507E0     5E                        POP ESI
	$+E6     016507E1     8BE5                      MOV ESP,EBP
	$+E8     016507E3     5D                        POP EBP
	$+E9     016507E4     C2 0400                   RETN 0x4
	$+EC     016507E7     CC                        INT3
	$+ED     016507E8     CC                        INT3
	$+EE     016507E9     CC                        INT3
	$+EF     016507EA     CC                        INT3
	$+F0     016507EB     CC                        INT3
	$+F1     016507EC     CC                        INT3
	$+F2     016507ED     CC                        INT3
	$+F3     016507EE     CC                        INT3
	$+F4     016507EF     CC                        INT3
	$+F5     016507F0     55                        PUSH EBP
	$+F6     016507F1     8BEC                      MOV EBP,ESP
	$+F8     016507F3     6A FF                     PUSH -0x1
	$+FA     016507F5     68 3874C101               PUSH PathOfEx.01C17438
	$+FF     016507FA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+105    01650800     50                        PUSH EAX
	$+106    01650801     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+10D    01650808     83EC 44                   SUB ESP,0x44
	$+110    0165080B     B8 8E000000               MOV EAX,0x8E
	$+115    01650810     C745 F0 07000000          MOV DWORD PTR SS:[EBP-0x10],0x7
	$+11C    01650817     66:8945 B4                MOV WORD PTR SS:[EBP-0x4C],AX
	$+120    0165081B     BA E07DDD01               MOV EDX,PathOfEx.01DD7DE0
	$+125    01650820     33C0                      XOR EAX,EAX
	$+127    01650822     8955 B0                   MOV DWORD PTR SS:[EBP-0x50],EDX
	$+12A    01650825     56                        PUSH ESI
	$+12B    01650826     8BF1                      MOV ESI,ECX
	$+12D    01650828     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$+134    0165082F     66:8945 DC                MOV WORD PTR SS:[EBP-0x24],AX
	$+138    01650833     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+13B    01650836     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+13E    01650839     8A45 08                   MOV AL,BYTE PTR SS:[EBP+0x8]
	$+141    0165083C     8845 B8                   MOV BYTE PTR SS:[EBP-0x48],AL
	$+144    0165083F     8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+147    01650842     3BC8                      CMP ECX,EAX
	$+149    01650844     74 0D                     JE SHORT PathOfEx.01650853
	$+14B    01650846     6A FF                     PUSH -0x1
	$+14D    01650848     6A 00                     PUSH 0x0
	$+14F    0165084A     50                        PUSH EAX
	$+150    0165084B     E8 905BB4FF               CALL PathOfEx.011963E0
	$+155    01650850     8B55 B0                   MOV EDX,DWORD PTR SS:[EBP-0x50]
	$+158    01650853     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+15B    01650856     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+15E    01650859     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+164    0165085F     0F1000                    MOVUPS XMM0,DQWORD PTR DS:[EAX]
	$+167    01650862     0F1145 B9                 MOVUPS DQWORD PTR SS:[EBP-0x47],XMM0
	$+16B    01650866     0F1040 10                 MOVUPS XMM0,DQWORD PTR DS:[EAX+0x10]
	$+16F    0165086A     0F1145 C9                 MOVUPS DQWORD PTR SS:[EBP-0x37],XMM0
	$+173    0165086E     FF52 04                   CALL DWORD PTR DS:[EDX+0x4]
	$+176    01650871     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+17C    01650877     6A 00                     PUSH 0x0
	$+17E    01650879     E8 8229EBFF               CALL PathOfEx.01503200
	$+183    0165087E     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+186    01650881     5E                        POP ESI
	$+187    01650882     83F8 08                   CMP EAX,0x8
	$+18A    01650885     72 0F                     JB SHORT PathOfEx.01650896
	$+18C    01650887     6A 02                     PUSH 0x2
	$+18E    01650889     40                        INC EAX
	$+18F    0165088A     50                        PUSH EAX
	$+190    0165088B     FF75 DC                   PUSH DWORD PTR SS:[EBP-0x24]
	$+193    0165088E     E8 DD4AB4FF               CALL PathOfEx.01195370
	$+198    01650893     83C4 0C                   ADD ESP,0xC
	$+19B    01650896     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+19E    01650899     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+1A5    016508A0     8BE5                      MOV ESP,EBP
	$+1A7    016508A2     5D                        POP EBP
	$+1A8    016508A3     C2 0C00                   RETN 0xC
	$+1AB    016508A6     CC                        INT3
	$+1AC    016508A7     CC                        INT3
	$+1AD    016508A8     CC                        INT3
	$+1AE    016508A9     CC                        INT3
	$+1AF    016508AA     CC                        INT3
	$+1B0    016508AB     CC                        INT3
	$+1B1    016508AC     CC                        INT3
	$+1B2    016508AD     CC                        INT3
	$+1B3    016508AE     CC                        INT3
	$+1B4    016508AF     CC                        INT3
	$+1B5    016508B0     55                        PUSH EBP
	$+1B6    016508B1     8BEC                      MOV EBP,ESP
	$+1B8    016508B3     6A FF                     PUSH -0x1
	$+1BA    016508B5     68 B866BD01               PUSH PathOfEx.01BD66B8
	$+1BF    016508BA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+1C5    016508C0     50                        PUSH EAX
	$+1C6    016508C1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+1CD    016508C8     83EC 0C                   SUB ESP,0xC
	$+1D0    016508CB     B8 5D000000               MOV EAX,0x5D
	$+1D5    016508D0     C745 E8 907FDD01          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01DD7F90
	$+1DC    016508D7     56                        PUSH ESI
	$+1DD    016508D8     8BF1                      MOV ESI,ECX
	$+1DF    016508DA     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+1E3    016508DE     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1E6    016508E1     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+1E9    016508E4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1F0    016508EB     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+1F6    016508F1     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1F9    016508F4     E8 D710FFFF               CALL PathOfEx.016419D0
	$+1FE    016508F9     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+204    016508FF     6A 00                     PUSH 0x0
	$+206    01650901     E8 FA28EBFF               CALL PathOfEx.01503200
	$+20B    01650906     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+20E    01650909     5E                        POP ESI
	$+20F    0165090A     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+216    01650911     8BE5                      MOV ESP,EBP
	$+218    01650913     5D                        POP EBP
	$+219    01650914     C2 0400                   RETN 0x4
	$+21C    01650917     CC                        INT3
	$+21D    01650918     CC                        INT3
	$+21E    01650919     CC                        INT3
	$+21F    0165091A     CC                        INT3
	$+220    0165091B     CC                        INT3
	$+221    0165091C     CC                        INT3
	$+222    0165091D     CC                        INT3
	$+223    0165091E     CC                        INT3
	$+224    0165091F     CC                        INT3
	$+225    01650920     55                        PUSH EBP
	$+226    01650921     8BEC                      MOV EBP,ESP
	$+228    01650923     6A FF                     PUSH -0x1
	$+22A    01650925     68 987FBD01               PUSH PathOfEx.01BD7F98
	$+22F    0165092A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+235    01650930     50                        PUSH EAX
	$+236    01650931     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+23D    01650938     83EC 10                   SUB ESP,0x10
	$+240    0165093B     B8 5E000000               MOV EAX,0x5E
	$+245    01650940     C745 E4 647EDD01          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.01DD7E64
	$+24C    01650947     56                        PUSH ESI
	$+24D    01650948     8BF1                      MOV ESI,ECX
	$+24F    0165094A     66:8945 E8                MOV WORD PTR SS:[EBP-0x18],AX
	$+253    0165094E     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+256    01650951     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+259    01650954     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+260    0165095B     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+266    01650961     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+269    01650964     8A45 0C                   MOV AL,BYTE PTR SS:[EBP+0xC]
	$+26C    01650967     8845 F0                   MOV BYTE PTR SS:[EBP-0x10],AL
	$+26F    0165096A     E8 3131FFFF               CALL PathOfEx.01643AA0
	$+274    0165096F     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+27A    01650975     6A 00                     PUSH 0x0
	$+27C    01650977     E8 8428EBFF               CALL PathOfEx.01503200
	$+281    0165097C     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+284    0165097F     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+28B    01650986     5E                        POP ESI
	$+28C    01650987     8BE5                      MOV ESP,EBP
	$+28E    01650989     5D                        POP EBP
	$+28F    0165098A     C2 0800                   RETN 0x8
	$+292    0165098D     CC                        INT3
	$+293    0165098E     CC                        INT3
	$+294    0165098F     CC                        INT3
	$+295    01650990     55                        PUSH EBP
	$+296    01650991     8BEC                      MOV EBP,ESP
	$+298    01650993     6A FF                     PUSH -0x1
	$+29A    01650995     68 C827BD01               PUSH PathOfEx.01BD27C8
	$+29F    0165099A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+2A5    016509A0     50                        PUSH EAX
	$+2A6    016509A1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+2AD    016509A8     83EC 14                   SUB ESP,0x14
	$+2B0    016509AB     B8 5F000000               MOV EAX,0x5F
	$+2B5    016509B0     C745 E0 587EDD01          MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01DD7E58
	$+2BC    016509B7     56                        PUSH ESI
	$+2BD    016509B8     8BF1                      MOV ESI,ECX
	$+2BF    016509BA     66:8945 E4                MOV WORD PTR SS:[EBP-0x1C],AX
	$+2C3    016509BE     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2C6    016509C1     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+2C9    016509C4     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2D0    016509CB     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+2D6    016509D1     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+2D9    016509D4     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2DC    016509D7     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+2DF    016509DA     66:C745 F0 FFFF           MOV WORD PTR SS:[EBP-0x10],0xFFFF
	$+2E5    016509E0     E8 4B31FFFF               CALL PathOfEx.01643B30
	$+2EA    016509E5     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+2F0    016509EB     6A 00                     PUSH 0x0
	$+2F2    016509ED     E8 0E28EBFF               CALL PathOfEx.01503200
	$+2F7    016509F2     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2FA    016509F5     5E                        POP ESI
	$+2FB    016509F6     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+302    016509FD     8BE5                      MOV ESP,EBP
	$+304    016509FF     5D                        POP EBP
	$+305    01650A00     C2 0800                   RETN 0x8
	$+308    01650A03     CC                        INT3
	$+309    01650A04     CC                        INT3
	$+30A    01650A05     CC                        INT3
	$+30B    01650A06     CC                        INT3
	$+30C    01650A07     CC                        INT3
	$+30D    01650A08     CC                        INT3
	$+30E    01650A09     CC                        INT3
	$+30F    01650A0A     CC                        INT3
	$+310    01650A0B     CC                        INT3
	$+311    01650A0C     CC                        INT3
	$+312    01650A0D     CC                        INT3
	$+313    01650A0E     CC                        INT3
	$+314    01650A0F     CC                        INT3
	$+315    01650A10     55                        PUSH EBP
	$+316    01650A11     8BEC                      MOV EBP,ESP
	$+318    01650A13     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+31E    01650A19     6A FF                     PUSH -0x1
	$+320    01650A1B     68 B866BD01               PUSH PathOfEx.01BD66B8
	$+325    01650A20     50                        PUSH EAX
	$+326    01650A21     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+32D    01650A28     83EC 0C                   SUB ESP,0xC
	$+330    01650A2B     56                        PUSH ESI
	$+331    01650A2C     8BF1                      MOV ESI,ECX
	$+333    01650A2E     80BE EB410000 00          CMP BYTE PTR DS:[ESI+0x41EB],0x0
	$+33A    01650A35     75 38                     JNZ SHORT PathOfEx.01650A6F
	$+33C    01650A37     B8 61000000               MOV EAX,0x61
	$+341    01650A3C     C745 E8 907FDD01          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01DD7F90
	$+348    01650A43     66:8945 EC                MOV WORD PTR SS:[EBP-0x14],AX
	$+34C    01650A47     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+34F    01650A4A     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+352    01650A4D     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+359    01650A54     FFB6 18410000             PUSH DWORD PTR DS:[ESI+0x4118]
	$+35F    01650A5A     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+362    01650A5D     E8 6E0FFFFF               CALL PathOfEx.016419D0
	$+367    01650A62     8B8E 18410000             MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+36D    01650A68     6A 00                     PUSH 0x0
	$+36F    01650A6A     E8 9127EBFF               CALL PathOfEx.01503200
	$+374    01650A6F     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+377    01650A72     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+37E    01650A79     5E                        POP ESI
	$+37F    01650A7A     8BE5                      MOV ESP,EBP
	$+381    01650A7C     5D                        POP EBP
	$+382    01650A7D     C2 0400                   RETN 0x4
	$+385    01650A80     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+389    01650A84     8981 78410000             MOV DWORD PTR DS:[ECX+0x4178],EAX
	$+38F    01650A8A     C2 0400                   RETN 0x4
	$+392    01650A8D     CC                        INT3
	$+393    01650A8E     CC                        INT3
	$+394    01650A8F     CC                        INT3
	$+395    01650A90     53                        PUSH EBX
	$+396    01650A91     8BD9                      MOV EBX,ECX
	$+398    01650A93     56                        PUSH ESI
	$+399    01650A94     57                        PUSH EDI
	$+39A    01650A95     8B83 F0420000             MOV EAX,DWORD PTR DS:[EBX+0x42F0]
	$+3A0    01650A9B     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+3A2    01650A9D     3BF0                      CMP ESI,EAX
	$+3A4    01650A9F     74 36                     JE SHORT PathOfEx.01650AD7
	$+3A6    01650AA1     8B7C24 10                 MOV EDI,DWORD PTR SS:[ESP+0x10]
	$+3AA    01650AA5     8B47 10                   MOV EAX,DWORD PTR DS:[EDI+0x10]
	$+3AD    01650AA8     8D56 20                   LEA EDX,DWORD PTR DS:[ESI+0x20]
	$+3B0    01650AAB     3B42 10                   CMP EAX,DWORD PTR DS:[EDX+0x10]
	$+3B3    01650AAE     75 1D                     JNZ SHORT PathOfEx.01650ACD
	$+3B5    01650AB0     837A 14 08                CMP DWORD PTR DS:[EDX+0x14],0x8
	$+3B9    01650AB4     72 02                     JB SHORT PathOfEx.01650AB8
	$+3BB    01650AB6     8B12                      MOV EDX,DWORD PTR DS:[EDX]
	$+3BD    01650AB8     837F 14 08                CMP DWORD PTR DS:[EDI+0x14],0x8
	$+3C1    01650ABC     72 04                     JB SHORT PathOfEx.01650AC2
	$+3C3    01650ABE     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
	$+3C5    01650AC0     EB 02                     JMP SHORT PathOfEx.01650AC4
	$+3C7    01650AC2     8BCF                      MOV ECX,EDI
	$+3C9    01650AC4     E8 D760B5FF               CALL PathOfEx.011A6BA0
	$+3CE    01650AC9     84C0                      TEST AL,AL
	$+3D0    01650ACB     75 12                     JNZ SHORT PathOfEx.01650ADF
	$+3D2    01650ACD     8B36                      MOV ESI,DWORD PTR DS:[ESI]
	$+3D4    01650ACF     3BB3 F0420000             CMP ESI,DWORD PTR DS:[EBX+0x42F0]
	$+3DA    01650AD5   ^ 75 CE                     JNZ SHORT PathOfEx.01650AA5
	$+3DC    01650AD7     5F                        POP EDI
	$+3DD    01650AD8     5E                        POP ESI
	$+3DE    01650AD9     33C0                      XOR EAX,EAX
	$+3E0    01650ADB     5B                        POP EBX
	$+3E1    01650ADC     C2 0400                   RETN 0x4
	$+3E4    01650ADF     5F                        POP EDI
	$+3E5    01650AE0     8D46 20                   LEA EAX,DWORD PTR DS:[ESI+0x20]
	$+3E8    01650AE3     5E                        POP ESI
	$+3E9    01650AE4     5B                        POP EBX
	$+3EA    01650AE5     C2 0400                   RETN 0x4
	$+3ED    01650AE8     CC                        INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("f3??????f3??????????????f3??????????????89????f3??????89????b8????????66??????8b??c7????????????e8", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_skill_obj_update_mouse_pos_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	
	//gj3
	/*
	$-3F3    015AEB1F     CC                       INT3
	$-3F2    015AEB20     55                       PUSH EBP
	$-3F1    015AEB21     8BEC                     MOV EBP,ESP
	$-3EF    015AEB23     6A FF                    PUSH -0x1
	$-3ED    015AEB25     68 4D68D401              PUSH PathOfEx.01D4684D
	$-3E8    015AEB2A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-3E2    015AEB30     50                       PUSH EAX
	$-3E1    015AEB31     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-3DA    015AEB38     81EC 84000000            SUB ESP,0x84
	$-3D4    015AEB3E     56                       PUSH ESI
	$-3D3    015AEB3F     57                       PUSH EDI
	$-3D2    015AEB40     8BF9                     MOV EDI,ECX
	$-3D0    015AEB42     E8 C9140300              CALL PathOfEx.015E0010
	$-3CB    015AEB47     33C0                     XOR EAX,EAX
	$-3C9    015AEB49     C745 EC 07000000         MOV DWORD PTR SS:[EBP-0x14],0x7
	$-3C2    015AEB50     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$-3BB    015AEB57     66:8945 D8               MOV WORD PTR SS:[EBP-0x28],AX
	$-3B7    015AEB5B     8945 FC                  MOV DWORD PTR SS:[EBP-0x4],EAX
	$-3B4    015AEB5E     833D A4AA0102 0A         CMP DWORD PTR DS:[0x201AAA4],0xA
	$-3AD    015AEB65     0F84 8C000000            JE PathOfEx.015AEBF7
	$-3A7    015AEB6B     83EC 10                  SUB ESP,0x10
	$-3A4    015AEB6E     E8 DDF70100              CALL PathOfEx.015CE350
	$-39F    015AEB73     8BC8                     MOV ECX,EAX
	$-39D    015AEB75     E8 E69ED4FF              CALL PathOfEx.012F8A60
	$-398    015AEB7A     8BF0                     MOV ESI,EAX
	$-396    015AEB7C     8975 F0                  MOV DWORD PTR SS:[EBP-0x10],ESI
	$-393    015AEB7F     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$-38F    015AEB83     8D55 F0                  LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-38C    015AEB86     FF35 A4AA0102            PUSH DWORD PTR DS:[0x201AAA4]
	$-386    015AEB8C     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-383    015AEB8F     E8 5C320300              CALL PathOfEx.015E1DF0
	$-37E    015AEB94     83C4 04                  ADD ESP,0x4
	$-37B    015AEB97     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$-377    015AEB9B     85F6                     TEST ESI,ESI
	$-375    015AEB9D     74 0D                    JE SHORT PathOfEx.015AEBAC
	$-373    015AEB9F     56                       PUSH ESI
	$-372    015AEBA0     E8 ABF70100              CALL PathOfEx.015CE350
	$-36D    015AEBA5     8BC8                     MOV ECX,EAX
	$-36B    015AEBA7     E8 74F30100              CALL PathOfEx.015CDF20
	$-366    015AEBAC     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$-362    015AEBB0     8B4D BC                  MOV ECX,DWORD PTR SS:[EBP-0x44]
	$-35F    015AEBB3     85C9                     TEST ECX,ECX
	$-35D    015AEBB5     74 1A                    JE SHORT PathOfEx.015AEBD1
	$-35B    015AEBB7     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-358    015AEBBA     50                       PUSH EAX
	$-357    015AEBBB     E8 8054DBFF              CALL PathOfEx.01364040
	$-352    015AEBC0     50                       PUSH EAX
	$-351    015AEBC1     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-34E    015AEBC4     E8 77D1D4FF              CALL PathOfEx.012FBD40
	$-349    015AEBC9     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-346    015AEBCC     E8 CF76D3FF              CALL PathOfEx.012E62A0
	$-341    015AEBD1     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
	$-33D    015AEBD5     8B45 B8                  MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-33A    015AEBD8     85C0                     TEST EAX,EAX
	$-338    015AEBDA     74 0D                    JE SHORT PathOfEx.015AEBE9
	$-336    015AEBDC     50                       PUSH EAX
	$-335    015AEBDD     E8 6EF70100              CALL PathOfEx.015CE350
	$-330    015AEBE2     8BC8                     MOV ECX,EAX
	$-32E    015AEBE4     E8 37F30100              CALL PathOfEx.015CDF20
	$-329    015AEBE9     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$-325    015AEBED     837D E8 00               CMP DWORD PTR SS:[EBP-0x18],0x0
	$-321    015AEBF1     0F85 C0000000            JNZ PathOfEx.015AECB7
	$-31B    015AEBF7     6A 0A                    PUSH 0xA
	$-319    015AEBF9     33C0                     XOR EAX,EAX
	$-317    015AEBFB     C745 D4 07000000         MOV DWORD PTR SS:[EBP-0x2C],0x7
	$-310    015AEC02     68 302CE901              PUSH PathOfEx.01E92C30                                  ; UNICODE "--language"
	$-30B    015AEC07     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-308    015AEC0A     C745 D0 00000000         MOV DWORD PTR SS:[EBP-0x30],0x0
	$-301    015AEC11     66:8945 C0               MOV WORD PTR SS:[EBP-0x40],AX
	$-2FD    015AEC15     E8 F67BD3FF              CALL PathOfEx.012E6810
	$-2F8    015AEC1A     8B37                     MOV ESI,DWORD PTR DS:[EDI]
	$-2F6    015AEC1C     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-2F3    015AEC1F     50                       PUSH EAX
	$-2F2    015AEC20     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-2EF    015AEC23     50                       PUSH EAX
	$-2EE    015AEC24     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$-2EB    015AEC27     83C1 1C                  ADD ECX,0x1C
	$-2E8    015AEC2A     E8 3191D3FF              CALL PathOfEx.012E7D60
	$-2E3    015AEC2F     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-2E0    015AEC32     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$-2DD    015AEC35     8B40 48                  MOV EAX,DWORD PTR DS:[EAX+0x48]
	$-2DA    015AEC38     8D0C40                   LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$-2D7    015AEC3B     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-2D5    015AEC3D     8B74C8 10                MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$-2D1    015AEC41     8B45 D4                  MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$-2CE    015AEC44     83F8 08                  CMP EAX,0x8
	$-2CB    015AEC47     72 0F                    JB SHORT PathOfEx.015AEC58
	$-2C9    015AEC49     40                       INC EAX
	$-2C8    015AEC4A     6A 02                    PUSH 0x2
	$-2C6    015AEC4C     50                       PUSH EAX
	$-2C5    015AEC4D     FF75 C0                  PUSH DWORD PTR SS:[EBP-0x40]
	$-2C2    015AEC50     E8 1B67D3FF              CALL PathOfEx.012E5370
	$-2BD    015AEC55     83C4 0C                  ADD ESP,0xC
	$-2BA    015AEC58     85F6                     TEST ESI,ESI
	$-2B8    015AEC5A     74 3A                    JE SHORT PathOfEx.015AEC96
	$-2B6    015AEC5C     68 302CE901              PUSH PathOfEx.01E92C30                                  ; UNICODE "--language"
	$-2B1    015AEC61     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-2AE    015AEC64     E8 E77CD3FF              CALL PathOfEx.012E6950
	$-2A9    015AEC69     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-2A6    015AEC6C     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
	$-2A2    015AEC70     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$-2A0    015AEC72     50                       PUSH EAX
	$-29F    015AEC73     8D45 A0                  LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-29C    015AEC76     50                       PUSH EAX
	$-29B    015AEC77     E8 04D30100              CALL PathOfEx.015CBF80
	$-296    015AEC7C     50                       PUSH EAX
	$-295    015AEC7D     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-292    015AEC80     E8 BBD0D4FF              CALL PathOfEx.012FBD40
	$-28D    015AEC85     8D4D A0                  LEA ECX,DWORD PTR SS:[EBP-0x60]
	$-28A    015AEC88     E8 1376D3FF              CALL PathOfEx.012E62A0
	$-285    015AEC8D     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$-281    015AEC91     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-27E    015AEC94     EB 1C                    JMP SHORT PathOfEx.015AECB2
	$-27C    015AEC96     8D45 A0                  LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-279    015AEC99     50                       PUSH EAX
	$-278    015AEC9A     E8 41F20C00              CALL PathOfEx.0167DEE0
	$-273    015AEC9F     8BC8                     MOV ECX,EAX
	$-271    015AECA1     E8 FA120D00              CALL PathOfEx.0167FFA0
	$-26C    015AECA6     50                       PUSH EAX
	$-26B    015AECA7     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-268    015AECAA     E8 91D0D4FF              CALL PathOfEx.012FBD40
	$-263    015AECAF     8D4D A0                  LEA ECX,DWORD PTR SS:[EBP-0x60]
	$-260    015AECB2     E8 E975D3FF              CALL PathOfEx.012E62A0
	$-25B    015AECB7     8D55 D8                  LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-258    015AECBA     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-255    015AECBD     E8 9ED40000              CALL PathOfEx.015BC160
	$-250    015AECC2     8BF0                     MOV ESI,EAX
	$-24E    015AECC4     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-24B    015AECC7     3BC6                     CMP EAX,ESI
	$-249    015AECC9     74 34                    JE SHORT PathOfEx.015AECFF
	$-247    015AECCB     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-244    015AECCE     83F8 08                  CMP EAX,0x8
	$-241    015AECD1     72 0F                    JB SHORT PathOfEx.015AECE2
	$-23F    015AECD3     40                       INC EAX
	$-23E    015AECD4     6A 02                    PUSH 0x2
	$-23C    015AECD6     50                       PUSH EAX
	$-23B    015AECD7     FF75 D8                  PUSH DWORD PTR SS:[EBP-0x28]
	$-238    015AECDA     E8 9166D3FF              CALL PathOfEx.012E5370
	$-233    015AECDF     83C4 0C                  ADD ESP,0xC
	$-230    015AECE2     33C0                     XOR EAX,EAX
	$-22E    015AECE4     C745 EC 07000000         MOV DWORD PTR SS:[EBP-0x14],0x7
	$-227    015AECEB     56                       PUSH ESI
	$-226    015AECEC     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-223    015AECEF     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$-21C    015AECF6     66:8945 D8               MOV WORD PTR SS:[EBP-0x28],AX
	$-218    015AECFA     E8 9199D3FF              CALL PathOfEx.012E8690
	$-213    015AECFF     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$-20F    015AED03     8B45 D4                  MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$-20C    015AED06     5F                       POP EDI
	$-20B    015AED07     5E                       POP ESI
	$-20A    015AED08     83F8 08                  CMP EAX,0x8
	$-207    015AED0B     72 0F                    JB SHORT PathOfEx.015AED1C
	$-205    015AED0D     40                       INC EAX
	$-204    015AED0E     6A 02                    PUSH 0x2
	$-202    015AED10     50                       PUSH EAX
	$-201    015AED11     FF75 C0                  PUSH DWORD PTR SS:[EBP-0x40]
	$-1FE    015AED14     E8 5766D3FF              CALL PathOfEx.012E5370
	$-1F9    015AED19     83C4 0C                  ADD ESP,0xC
	$-1F6    015AED1C     837D E8 00               CMP DWORD PTR SS:[EBP-0x18],0x0
	$-1F2    015AED20     0F84 9C000000            JE PathOfEx.015AEDC2
	$-1EC    015AED26     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-1E9    015AED29     E8 C2120300              CALL PathOfEx.015DFFF0
	$-1E4    015AED2E     84C0                     TEST AL,AL
	$-1E2    015AED30     75 79                    JNZ SHORT PathOfEx.015AEDAB
	$-1E0    015AED32     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-1DD    015AED35     50                       PUSH EAX
	$-1DC    015AED36     8D45 A0                  LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-1D9    015AED39     50                       PUSH EAX
	$-1D8    015AED3A     E8 E1D00000              CALL PathOfEx.015BBE20
	$-1D3    015AED3F     50                       PUSH EAX
	$-1D2    015AED40     8D45 88                  LEA EAX,DWORD PTR SS:[EBP-0x78]
	$-1CF    015AED43     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$-1CB    015AED47     68 0813ED01              PUSH PathOfEx.01ED1308                                  ; ASCII "Failed to set language to ""
	$-1C6    015AED4C     50                       PUSH EAX
	$-1C5    015AED4D     E8 5EAAD5FF              CALL PathOfEx.013097B0
	$-1C0    015AED52     68 ECA0EC01              PUSH PathOfEx.01ECA0EC                                  ; UNICODE """
	$-1BB    015AED57     50                       PUSH EAX
	$-1BA    015AED58     8D85 70FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0x90]
	$-1B4    015AED5E     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
	$-1B0    015AED62     50                       PUSH EAX
	$-1AF    015AED63     E8 88FCD3FF              CALL PathOfEx.012EE9F0
	$-1AA    015AED68     83C4 20                  ADD ESP,0x20
	$-1A7    015AED6B     C645 FC 0A               MOV BYTE PTR SS:[EBP-0x4],0xA
	$-1A3    015AED6F     8378 14 10               CMP DWORD PTR DS:[EAX+0x14],0x10
	$-19F    015AED73     72 02                    JB SHORT PathOfEx.015AED77
	$-19D    015AED75     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-19B    015AED77     8945 B8                  MOV DWORD PTR SS:[EBP-0x48],EAX
	$-198    015AED7A     0F57C0                   XORPS XMM0,XMM0
	$-195    015AED7D     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-192    015AED80     C745 CC B817E001         MOV DWORD PTR SS:[EBP-0x34],PathOfEx.01E017B8
	$-18B    015AED87     50                       PUSH EAX
	$-18A    015AED88     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
	$-187    015AED8B     660FD645 D0              MOVQ QWORD PTR SS:[EBP-0x30],XMM0
	$-182    015AED90     50                       PUSH EAX
	$-181    015AED91     C645 BC 01               MOV BYTE PTR SS:[EBP-0x44],0x1
	$-17D    015AED95     E8 1A3F6F00              CALL PathOfEx.01CA2CB4
	$-178    015AED9A     83C4 08                  ADD ESP,0x8
	$-175    015AED9D     8D45 CC                  LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-172    015AEDA0     68 BC2B0102              PUSH PathOfEx.02012BBC
	$-16D    015AEDA5     50                       PUSH EAX
	$-16C    015AEDA6     E8 C84F6F00              CALL PathOfEx.01CA3D73
	$-167    015AEDAB     E8 B0100300              CALL PathOfEx.015DFE60
	$-162    015AEDB0     FF35 B8AA0102            PUSH DWORD PTR DS:[0x201AAB8]
	$-15C    015AEDB6     E8 25F10C00              CALL PathOfEx.0167DEE0
	$-157    015AEDBB     8BC8                     MOV ECX,EAX
	$-155    015AEDBD     E8 DE0F0D00              CALL PathOfEx.0167FDA0
	$-150    015AEDC2     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-14D    015AEDC5     83F8 08                  CMP EAX,0x8
	$-14A    015AEDC8     72 0F                    JB SHORT PathOfEx.015AEDD9
	$-148    015AEDCA     6A 02                    PUSH 0x2
	$-146    015AEDCC     40                       INC EAX
	$-145    015AEDCD     50                       PUSH EAX
	$-144    015AEDCE     FF75 D8                  PUSH DWORD PTR SS:[EBP-0x28]
	$-141    015AEDD1     E8 9A65D3FF              CALL PathOfEx.012E5370
	$-13C    015AEDD6     83C4 0C                  ADD ESP,0xC
	$-139    015AEDD9     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-136    015AEDDC     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-12F    015AEDE3     8BE5                     MOV ESP,EBP
	$-12D    015AEDE5     5D                       POP EBP
	$-12C    015AEDE6     C3                       RETN
	$-12B    015AEDE7     CC                       INT3
	$-12A    015AEDE8     CC                       INT3
	$-129    015AEDE9     CC                       INT3
	$-128    015AEDEA     CC                       INT3
	$-127    015AEDEB     CC                       INT3
	$-126    015AEDEC     CC                       INT3
	$-125    015AEDED     CC                       INT3
	$-124    015AEDEE     CC                       INT3
	$-123    015AEDEF     CC                       INT3
	$-122    015AEDF0     83EC 14                  SUB ESP,0x14
	$-11F    015AEDF3     68 2413ED01              PUSH PathOfEx.01ED1324                                  ; UNICODE "Global\PoERunMutexA"
	$-11A    015AEDF8     6A 00                    PUSH 0x0
	$-118    015AEDFA     6A 00                    PUSH 0x0
	$-116    015AEDFC     FF15 1471D901            CALL DWORD PTR DS:[<&KERNEL32.CreateMutexW>]            ; kernel32.CreateMutexW
	$-110    015AEE02     85C0                     TEST EAX,EAX
	$-10E    015AEE04     74 3B                    JE SHORT PathOfEx.015AEE41
	$-10C    015AEE06     6A 00                    PUSH 0x0
	$-10A    015AEE08     50                       PUSH EAX
	$-109    015AEE09     FF15 A071D901            CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]     ; kernel32.WaitForSingleObject
	$-103    015AEE0F     3D 02010000              CMP EAX,0x102
	$-FE     015AEE14     75 27                    JNZ SHORT PathOfEx.015AEE3D
	$-FC     015AEE16     68 8012ED01              PUSH PathOfEx.01ED1280                                  ; UNICODE "Global\PoERunMutexB"
	$-F7     015AEE1B     6A 00                    PUSH 0x0
	$-F5     015AEE1D     6A 00                    PUSH 0x0
	$-F3     015AEE1F     FF15 1471D901            CALL DWORD PTR DS:[<&KERNEL32.CreateMutexW>]            ; kernel32.CreateMutexW
	$-ED     015AEE25     85C0                     TEST EAX,EAX
	$-EB     015AEE27     74 5F                    JE SHORT PathOfEx.015AEE88
	$-E9     015AEE29     6A 00                    PUSH 0x0
	$-E7     015AEE2B     50                       PUSH EAX
	$-E6     015AEE2C     FF15 A071D901            CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]     ; kernel32.WaitForSingleObject
	$-E0     015AEE32     3D 02010000              CMP EAX,0x102
	$-DB     015AEE37     0F84 92000000            JE PathOfEx.015AEECF
	$-D5     015AEE3D     83C4 14                  ADD ESP,0x14
	$-D2     015AEE40     C3                       RETN
	$-D1     015AEE41     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-CD     015AEE45     C74424 08 B817E001       MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01E017B8
	$-C5     015AEE4D     50                       PUSH EAX
	$-C4     015AEE4E     8D4424 04                LEA EAX,DWORD PTR SS:[ESP+0x4]
	$-C0     015AEE52     C74424 04 6812ED01       MOV DWORD PTR SS:[ESP+0x4],PathOfEx.01ED1268            ; ASCII "Failed to create mutex"
	$-B8     015AEE5A     0F57C0                   XORPS XMM0,XMM0
	$-B5     015AEE5D     C64424 08 01             MOV BYTE PTR SS:[ESP+0x8],0x1
	$-B0     015AEE62     50                       PUSH EAX
	$-AF     015AEE63     660FD64424 14            MOVQ QWORD PTR SS:[ESP+0x14],XMM0
	$-A9     015AEE69     E8 463E6F00              CALL PathOfEx.01CA2CB4
	$-A4     015AEE6E     83C4 08                  ADD ESP,0x8
	$-A1     015AEE71     C74424 08 1018E001       MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01E01810
	$-99     015AEE79     8D4424 08                LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-95     015AEE7D     68 98270102              PUSH PathOfEx.02012798
	$-90     015AEE82     50                       PUSH EAX
	$-8F     015AEE83     E8 EB4E6F00              CALL PathOfEx.01CA3D73
	$-8A     015AEE88     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-86     015AEE8C     C74424 08 B817E001       MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01E017B8
	$-7E     015AEE94     50                       PUSH EAX
	$-7D     015AEE95     8D4424 04                LEA EAX,DWORD PTR SS:[ESP+0x4]
	$-79     015AEE99     C74424 04 6812ED01       MOV DWORD PTR SS:[ESP+0x4],PathOfEx.01ED1268            ; ASCII "Failed to create mutex"
	$-71     015AEEA1     0F57C0                   XORPS XMM0,XMM0
	$-6E     015AEEA4     C64424 08 01             MOV BYTE PTR SS:[ESP+0x8],0x1
	$-69     015AEEA9     50                       PUSH EAX
	$-68     015AEEAA     660FD64424 14            MOVQ QWORD PTR SS:[ESP+0x14],XMM0
	$-62     015AEEB0     E8 FF3D6F00              CALL PathOfEx.01CA2CB4
	$-5D     015AEEB5     83C4 08                  ADD ESP,0x8
	$-5A     015AEEB8     C74424 08 1018E001       MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01E01810
	$-52     015AEEC0     8D4424 08                LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-4E     015AEEC4     68 98270102              PUSH PathOfEx.02012798
	$-49     015AEEC9     50                       PUSH EAX
	$-48     015AEECA     E8 A44E6F00              CALL PathOfEx.01CA3D73
	$-43     015AEECF     68 A812ED01              PUSH PathOfEx.01ED12A8                                  ; ASCII "You may only run up to two copies of Path of Exile at one time."
	$-3E     015AEED4     8D4C24 0C                LEA ECX,DWORD PTR SS:[ESP+0xC]
	$-3A     015AEED8     E8 E3EDD3FF              CALL PathOfEx.012EDCC0
	$-35     015AEEDD     68 BC2B0102              PUSH PathOfEx.02012BBC
	$-30     015AEEE2     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-2C     015AEEE6     50                       PUSH EAX
	$-2B     015AEEE7     E8 874E6F00              CALL PathOfEx.01CA3D73
	$-26     015AEEEC     CC                       INT3
	$-25     015AEEED     CC                       INT3
	$-24     015AEEEE     CC                       INT3
	$-23     015AEEEF     CC                       INT3
	$-22     015AEEF0     55                       PUSH EBP                                                ; tag_disable-client-switching
	$-21     015AEEF1     8BEC                     MOV EBP,ESP
	$-1F     015AEEF3     83E4 F8                  AND ESP,0xFFFFFFF8
	$-1C     015AEEF6     83EC 1C                  SUB ESP,0x1C
	$-19     015AEEF9     53                       PUSH EBX
	$-18     015AEEFA     56                       PUSH ESI
	$-17     015AEEFB     57                       PUSH EDI
	$-16     015AEEFC     6A 1A                    PUSH 0x1A
	$-14     015AEEFE     8BF1                     MOV ESI,ECX
	$-12     015AEF00     C74424 28 07000000       MOV DWORD PTR SS:[ESP+0x28],0x7
	$-A      015AEF08     33C0                     XOR EAX,EAX
	$-8      015AEF0A     C74424 24 00000000       MOV DWORD PTR SS:[ESP+0x24],0x0
	$ ==>    015AEF12     68 B810ED01              PUSH PathOfEx.01ED10B8                                  ; UNICODE "--disable-client-switching"
	$+5      015AEF17     8D4C24 18                LEA ECX,DWORD PTR SS:[ESP+0x18]
	$+9      015AEF1B     66:894424 18             MOV WORD PTR SS:[ESP+0x18],AX
	$+E      015AEF20     E8 EB78D3FF              CALL PathOfEx.012E6810                                  ; 返回std::wstring
	$+13     015AEF25     8B36                     MOV ESI,DWORD PTR DS:[ESI]
	$+15     015AEF27     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+19     015AEF2B     50                       PUSH EAX
	$+1A     015AEF2C     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+1E     015AEF30     50                       PUSH EAX
	$+1F     015AEF31     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+22     015AEF34     83C1 1C                  ADD ECX,0x1C
	$+25     015AEF37     E8 248ED3FF              CALL PathOfEx.012E7D60
	$+2A     015AEF3C     8B4424 0C                MOV EAX,DWORD PTR SS:[ESP+0xC]
	$+2E     015AEF40     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+31     015AEF43     8B40 48                  MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+34     015AEF46     8D0C40                   LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+37     015AEF49     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+39     015AEF4B     8B74C8 10                MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+3D     015AEF4F     8B4424 24                MOV EAX,DWORD PTR SS:[ESP+0x24]
	$+41     015AEF53     83F8 08                  CMP EAX,0x8
	$+44     015AEF56     72 10                    JB SHORT PathOfEx.015AEF68
	$+46     015AEF58     6A 02                    PUSH 0x2
	$+48     015AEF5A     40                       INC EAX
	$+49     015AEF5B     50                       PUSH EAX
	$+4A     015AEF5C     FF7424 18                PUSH DWORD PTR SS:[ESP+0x18]
	$+4E     015AEF60     E8 0B64D3FF              CALL PathOfEx.012E5370                                  ; 释放wstring
	$+53     015AEF65     83C4 0C                  ADD ESP,0xC
	$+56     015AEF68     85F6                     TEST ESI,ESI
	$+58     015AEF6A     0F85 B2000000            JNZ PathOfEx.015AF022
	$+5E     015AEF70     E8 6BEF0C00              CALL PathOfEx.0167DEE0                                  ; nothing
	$+63     015AEF75     8BC8                     MOV ECX,EAX
	$+65     015AEF77     E8 940C0D00              CALL PathOfEx.0167FC10
	$+6A     015AEF7C     68 7C1AED01              PUSH PathOfEx.01ED1A7C
	$+6F     015AEF81     68 941AED01              PUSH PathOfEx.01ED1A94                                  ; UNICODE "kernel32"
	$+74     015AEF86     8BF0                     MOV ESI,EAX
	$+76     015AEF88     C74424 14 00000000       MOV DWORD PTR SS:[ESP+0x14],0x0
	$+7E     015AEF90     FF15 F470D901            CALL DWORD PTR DS:[<&KERNEL32.GetModuleHandleW>]        ; kernel32.GetModuleHandleW
	$+84     015AEF96     50                       PUSH EAX
	$+85     015AEF97     FF15 F070D901            CALL DWORD PTR DS:[<&KERNEL32.GetProcAddress>]          ; kernel32.GetProcAddress
	$+8B     015AEF9D     8BF8                     MOV EDI,EAX
	$+8D     015AEF9F     85FF                     TEST EDI,EDI
	$+8F     015AEFA1     74 0E                    JE SHORT PathOfEx.015AEFB1
	$+91     015AEFA3     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+95     015AEFA7     50                       PUSH EAX
	$+96     015AEFA8     FF15 0471D901            CALL DWORD PTR DS:[<&KERNEL32.GetCurrentProcess>]       ; kernel32.GetCurrentProcess
	$+9C     015AEFAE     50                       PUSH EAX
	$+9D     015AEFAF     FFD7                     CALL EDI
	$+9F     015AEFB1     837C24 0C 00             CMP DWORD PTR SS:[ESP+0xC],0x0
	$+A4     015AEFB6     74 6A                    JE SHORT PathOfEx.015AF022
	$+A6     015AEFB8     83FE 03                  CMP ESI,0x3
	$+A9     015AEFBB     74 04                    JE SHORT PathOfEx.015AEFC1
	$+AB     015AEFBD     85F6                     TEST ESI,ESI
	$+AD     015AEFBF     75 61                    JNZ SHORT PathOfEx.015AF022
	$+AF     015AEFC1     E8 2A420000              CALL PathOfEx.015B31F0                                  ; SwitchProcess
	$+B4     015AEFC6     84C0                     TEST AL,AL
	$+B6     015AEFC8     74 09                    JE SHORT PathOfEx.015AEFD3
	$+B8     015AEFCA     32C0                     XOR AL,AL
	$+BA     015AEFCC     5F                       POP EDI
	$+BB     015AEFCD     5E                       POP ESI
	$+BC     015AEFCE     5B                       POP EBX
	$+BD     015AEFCF     8BE5                     MOV ESP,EBP
	$+BF     015AEFD1     5D                       POP EBP
	$+C0     015AEFD2     C3                       RETN
	$+C1     015AEFD3     33DB                     XOR EBX,EBX
	$+C3     015AEFD5     381D 4AA50602            CMP BYTE PTR DS:[0x206A54A],BL
	$+C9     015AEFDB     0F95C3                   SETNE BL
	$+CC     015AEFDE     E8 FDEE0C00              CALL PathOfEx.0167DEE0
	$+D1     015AEFE3     FF349D B8B0E101          PUSH DWORD PTR DS:[EBX*4+0x1E1B0B8]
	$+D8     015AEFEA     68 EC95EB01              PUSH PathOfEx.01EB95EC                                  ; UNICODE "directx_version"
	$+DD     015AEFEF     8B88 80030000            MOV ECX,DWORD PTR DS:[EAX+0x380]
	$+E3     015AEFF5     68 2896EB01              PUSH PathOfEx.01EB9628                                  ; UNICODE "DISPLAY"
	$+E8     015AEFFA     E8 618C0D00              CALL PathOfEx.01687C60
	$+ED     015AEFFF     E8 DCEE0C00              CALL PathOfEx.0167DEE0
	$+F2     015AF004     8BF0                     MOV ESI,EAX
	$+F4     015AF006     8B8E 80030000            MOV ECX,DWORD PTR DS:[ESI+0x380]
	$+FA     015AF00C     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$+FC     015AF00E     8B52 08                  MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+FF     015AF011     FFD2                     CALL EDX
	$+101    015AF013     84C0                     TEST AL,AL
	$+103    015AF015     74 0B                    JE SHORT PathOfEx.015AF022
	$+105    015AF017     8B8E 80030000            MOV ECX,DWORD PTR DS:[ESI+0x380]
	$+10B    015AF01D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+10D    015AF01F     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+110    015AF022     5F                       POP EDI
	$+111    015AF023     5E                       POP ESI
	$+112    015AF024     B0 01                    MOV AL,0x1
	$+114    015AF026     5B                       POP EBX
	$+115    015AF027     8BE5                     MOV ESP,EBP
	$+117    015AF029     5D                       POP EBP
	$+118    015AF02A     C3                       RETN
	$+119    015AF02B     CC                       INT3
	$+11A    015AF02C     CC                       INT3
	$+11B    015AF02D     CC                       INT3
	$+11C    015AF02E     CC                       INT3
	$+11D    015AF02F     CC                       INT3
	$+11E    015AF030     55                       PUSH EBP
	$+11F    015AF031     8BEC                     MOV EBP,ESP
	$+121    015AF033     6A FF                    PUSH -0x1
	$+123    015AF035     68 8368D401              PUSH PathOfEx.01D46883
	$+128    015AF03A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+12E    015AF040     50                       PUSH EAX
	$+12F    015AF041     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+136    015AF048     81EC F0010000            SUB ESP,0x1F0
	$+13C    015AF04E     53                       PUSH EBX
	$+13D    015AF04F     56                       PUSH ESI
	$+13E    015AF050     57                       PUSH EDI
	$+13F    015AF051     8BD9                     MOV EBX,ECX
	$+141    015AF053     51                       PUSH ECX
	$+142    015AF054     8D8D 04FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1FC]
	$+148    015AF05A     E8 61FA3200              CALL PathOfEx.018DEAC0
	$+14D    015AF05F     83EC 14                  SUB ESP,0x14
	$+150    015AF062     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+157    015AF069     8B0D 98232E02            MOV ECX,DWORD PTR DS:[0x22E2398]
	$+15D    015AF06F     8D85 04FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1FC]
	$+163    015AF075     50                       PUSH EAX
	$+164    015AF076     E8 851BD4FF              CALL PathOfEx.012F0C00
	$+169    015AF07B     84C0                     TEST AL,AL
	$+16B    015AF07D     75 38                    JNZ SHORT PathOfEx.015AF0B7
	$+16D    015AF07F     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+170    015AF082     C745 E0 B817E001         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01E017B8
	$+177    015AF089     50                       PUSH EAX
	$+178    015AF08A     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+17B    015AF08D     C745 EC D013ED01         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01ED13D0           ; ASCII "Failed to initialise"
	$+182    015AF094     0F57C0                   XORPS XMM0,XMM0
	$+185    015AF097     C645 F0 01               MOV BYTE PTR SS:[EBP-0x10],0x1
	$+189    015AF09B     50                       PUSH EAX
	$+18A    015AF09C     660FD645 E4              MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$+18F    015AF0A1     E8 0E3C6F00              CALL PathOfEx.01CA2CB4
	$+194    015AF0A6     83C4 08                  ADD ESP,0x8
	$+197    015AF0A9     8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+19A    015AF0AC     68 BC2B0102              PUSH PathOfEx.02012BBC
	$+19F    015AF0B1     50                       PUSH EAX
	$+1A0    015AF0B2     E8 BC4C6F00              CALL PathOfEx.01CA3D73
	$+1A5    015AF0B7     6A 0C                    PUSH 0xC
	$+1A7    015AF0B9     33C0                     XOR EAX,EAX
	$+1A9    015AF0BB     C745 E8 07000000         MOV DWORD PTR SS:[EBP-0x18],0x7
	$+1B0    015AF0C2     68 7404ED01              PUSH PathOfEx.01ED0474                                  ; UNICODE "--login-port"
	$+1B5    015AF0C7     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+1B8    015AF0CA     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+1BF    015AF0D1     66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+1C3    015AF0D5     E8 3677D3FF              CALL PathOfEx.012E6810
	$+1C8    015AF0DA     8B33                     MOV ESI,DWORD PTR DS:[EBX]
	$+1CA    015AF0DC     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+1CD    015AF0DF     50                       PUSH EAX
	$+1CE    015AF0E0     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+1D1    015AF0E3     50                       PUSH EAX
	$+1D2    015AF0E4     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+1D5    015AF0E7     83C1 1C                  ADD ECX,0x1C
	$+1D8    015AF0EA     E8 718CD3FF              CALL PathOfEx.012E7D60
	$+1DD    015AF0EF     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+1E0    015AF0F2     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+1E3    015AF0F5     8B40 48                  MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+1E6    015AF0F8     8D0C40                   LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+1E9    015AF0FB     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+1EB    015AF0FD     8B74C8 10                MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+1EF    015AF101     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+1F2    015AF104     83F8 08                  CMP EAX,0x8
	$+1F5    015AF107     72 0F                    JB SHORT PathOfEx.015AF118
	$+1F7    015AF109     40                       INC EAX
	$+1F8    015AF10A     6A 02                    PUSH 0x2
	$+1FA    015AF10C     50                       PUSH EAX
	$+1FB    015AF10D     FF75 D4                  PUSH DWORD PTR SS:[EBP-0x2C]
	$+1FE    015AF110     E8 5B62D3FF              CALL PathOfEx.012E5370
	$+203    015AF115     83C4 0C                  ADD ESP,0xC
	$+206    015AF118     85F6                     TEST ESI,ESI
	$+208    015AF11A     74 57                    JE SHORT PathOfEx.015AF173
	$+20A    015AF11C     68 7404ED01              PUSH PathOfEx.01ED0474                                  ; UNICODE "--login-port"
	$+20F    015AF121     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+212    015AF124     E8 2778D3FF              CALL PathOfEx.012E6950
	$+217    015AF129     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+21A    015AF12C     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+21E    015AF130     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+220    015AF132     50                       PUSH EAX
	$+221    015AF133     8D45 BC                  LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+224    015AF136     50                       PUSH EAX
	$+225    015AF137     E8 44CE0100              CALL PathOfEx.015CBF80
	$+22A    015AF13C     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+22D    015AF13F     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$+231    015AF143     E8 5871D3FF              CALL PathOfEx.012E62A0
	$+236    015AF148     A1 98232E02              MOV EAX,DWORD PTR DS:[0x22E2398]
	$+23B    015AF14D     8B48 30                  MOV ECX,DWORD PTR DS:[EAX+0x30]
	$+23E    015AF150     8D45 BC                  LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+241    015AF153     81C1 CC000000            ADD ECX,0xCC
	$+247    015AF159     3BC8                     CMP ECX,EAX
	$+249    015AF15B     74 0A                    JE SHORT PathOfEx.015AF167
	$+24B    015AF15D     6A FF                    PUSH -0x1
	$+24D    015AF15F     6A 00                    PUSH 0x0
	$+24F    015AF161     50                       PUSH EAX
	$+250    015AF162     E8 7972D3FF              CALL PathOfEx.012E63E0
	$+255    015AF167     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+258    015AF16A     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$+25C    015AF16E     E8 2D71D3FF              CALL PathOfEx.012E62A0
	$+261    015AF173     6A 15                    PUSH 0x15
	$+263    015AF175     33C0                     XOR EAX,EAX
	$+265    015AF177     C745 E8 07000000         MOV DWORD PTR SS:[EBP-0x18],0x7
	$+26C    015AF17E     68 C407ED01              PUSH PathOfEx.01ED07C4                                  ; UNICODE "--garbage-generations"
	$+271    015AF183     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+274    015AF186     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+27B    015AF18D     66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+27F    015AF191     E8 7A76D3FF              CALL PathOfEx.012E6810
	$+284    015AF196     8B33                     MOV ESI,DWORD PTR DS:[EBX]
	$+286    015AF198     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+289    015AF19B     50                       PUSH EAX
	$+28A    015AF19C     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+28D    015AF19F     50                       PUSH EAX
	$+28E    015AF1A0     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+291    015AF1A3     83C1 1C                  ADD ECX,0x1C
	$+294    015AF1A6     E8 B58BD3FF              CALL PathOfEx.012E7D60
	$+299    015AF1AB     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+29C    015AF1AE     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+29F    015AF1B1     8B40 48                  MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+2A2    015AF1B4     8D0C40                   LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+2A5    015AF1B7     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+2A7    015AF1B9     8B74C8 10                MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+2AB    015AF1BD     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+2AE    015AF1C0     83F8 08                  CMP EAX,0x8
	$+2B1    015AF1C3     72 0F                    JB SHORT PathOfEx.015AF1D4
	$+2B3    015AF1C5     40                       INC EAX
	$+2B4    015AF1C6     6A 02                    PUSH 0x2
	$+2B6    015AF1C8     50                       PUSH EAX
	$+2B7    015AF1C9     FF75 D4                  PUSH DWORD PTR SS:[EBP-0x2C]
	$+2BA    015AF1CC     E8 9F61D3FF              CALL PathOfEx.012E5370
	$+2BF    015AF1D1     83C4 0C                  ADD ESP,0xC
	$+2C2    015AF1D4     85F6                     TEST ESI,ESI
	$+2C4    015AF1D6     74 7C                    JE SHORT PathOfEx.015AF254
	$+2C6    015AF1D8     68 C407ED01              PUSH PathOfEx.01ED07C4                                  ; UNICODE "--garbage-generations"
	$+2CB    015AF1DD     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+2CE    015AF1E0     E8 6B77D3FF              CALL PathOfEx.012E6950
	$+2D3    015AF1E5     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+2D6    015AF1E8     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+2DA    015AF1EC     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+2DC    015AF1EE     50                       PUSH EAX
	$+2DD    015AF1EF     E8 ECCD0100              CALL PathOfEx.015CBFE0
	$+2E2    015AF1F4     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+2E5    015AF1F7     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$+2E9    015AF1FB     8BF0                     MOV ESI,EAX
	$+2EB    015AF1FD     E8 9E70D3FF              CALL PathOfEx.012E62A0
	$+2F0    015AF202     83FE 01                  CMP ESI,0x1
	$+2F3    015AF205     7D 3F                    JGE SHORT PathOfEx.015AF246
	$+2F5    015AF207     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+2F8    015AF20A     C745 E0 B817E001         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01E017B8
	$+2FF    015AF211     50                       PUSH EAX
	$+300    015AF212     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+303    015AF215     C745 EC E813ED01         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01ED13E8           ; ASCII "Must set a garbage generations to 1 or higher"
	$+30A    015AF21C     0F57C0                   XORPS XMM0,XMM0
	$+30D    015AF21F     C645 F0 01               MOV BYTE PTR SS:[EBP-0x10],0x1
	$+311    015AF223     50                       PUSH EAX
	$+312    015AF224     660FD645 E4              MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$+317    015AF229     E8 863A6F00              CALL PathOfEx.01CA2CB4
	$+31C    015AF22E     83C4 08                  ADD ESP,0x8
	$+31F    015AF231     C745 E0 1018E001         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01E01810
	$+326    015AF238     8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+329    015AF23B     68 98270102              PUSH PathOfEx.02012798
	$+32E    015AF240     50                       PUSH EAX
	$+32F    015AF241     E8 2D4B6F00              CALL PathOfEx.01CA3D73
	$+334    015AF246     A1 98232E02              MOV EAX,DWORD PTR DS:[0x22E2398]
	$+339    015AF24B     8B40 08                  MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+33C    015AF24E     89B0 98190000            MOV DWORD PTR DS:[EAX+0x1998],ESI
	$+342    015AF254     E8 87EC0C00              CALL PathOfEx.0167DEE0
	$+347    015AF259     68 DC0CED01              PUSH PathOfEx.01ED0CDC                                  ; UNICODE "--use-defaults"
	$+34C    015AF25E     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+34F    015AF261     8BF8                     MOV EDI,EAX
	$+351    015AF263     E8 E876D3FF              CALL PathOfEx.012E6950
	$+356    015AF268     8B33                     MOV ESI,DWORD PTR DS:[EBX]
	$+358    015AF26A     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+35B    015AF26D     50                       PUSH EAX
	$+35C    015AF26E     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+35F    015AF271     50                       PUSH EAX
	$+360    015AF272     8B4E 18                  MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+363    015AF275     83C1 1C                  ADD ECX,0x1C
	$+366    015AF278     E8 E38AD3FF              CALL PathOfEx.012E7D60
	$+36B    015AF27D     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+36E    015AF280     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+371    015AF283     8B40 48                  MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+374    015AF286     8D0C40                   LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+377    015AF289     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+379    015AF28B     8B74C8 10                MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+37D    015AF28F     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+380    015AF292     83F8 08                  CMP EAX,0x8
	$+383    015AF295     72 0F                    JB SHORT PathOfEx.015AF2A6
	$+385    015AF297     6A 02                    PUSH 0x2
	$+387    015AF299     40                       INC EAX
	$+388    015AF29A     50                       PUSH EAX
	$+389    015AF29B     FF75 D4                  PUSH DWORD PTR SS:[EBP-0x2C]
	$+38C    015AF29E     E8 CD60D3FF              CALL PathOfEx.012E5370
	$+391    015AF2A3     83C4 0C                  ADD ESP,0xC
	$+394    015AF2A6     85F6                     TEST ESI,ESI
	$+396    015AF2A8     74 19                    JE SHORT PathOfEx.015AF2C3
	$+398    015AF2AA     8B8F 80030000            MOV ECX,DWORD PTR DS:[EDI+0x380]
	$+39E    015AF2B0     8D87 84030000            LEA EAX,DWORD PTR DS:[EDI+0x384]
	$+3A4    015AF2B6     50                       PUSH EAX
	$+3A5    015AF2B7     E8 A4920D00              CALL PathOfEx.01688560
	$+3AA    015AF2BC     8BCF                     MOV ECX,EDI
	$+3AC    015AF2BE     E8 ADEC0C00              CALL PathOfEx.0167DF70
	$+3B1    015AF2C3     8D8D 04FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1FC]
	$+3B7    015AF2C9     E8 D229D4FF              CALL PathOfEx.012F1CA0
	$+3BC    015AF2CE     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3BF    015AF2D1     5F                       POP EDI
	$+3C0    015AF2D2     5E                       POP ESI
	$+3C1    015AF2D3     5B                       POP EBX
	$+3C2    015AF2D4     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+3C9    015AF2DB     8BE5                     MOV ESP,EBP
	$+3CB    015AF2DD     5D                       POP EBP
	$+3CC    015AF2DE     C3                       RETN
	$+3CD    015AF2DF     CC                       INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("68????????8d??????66????????e8????????8b??8d??????508d??????508b????83????e8????????8b", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_disable_client_switching_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
#endif

	/*
	$-E1     003BEB1F     CC                   INT3
	$-E0     003BEB20     55                   PUSH EBP
	$-DF     003BEB21     8BEC                 MOV EBP,ESP
	$-DD     003BEB23     6A FF                PUSH -0x1
	$-DB     003BEB25     68 9191C200          PUSH PathOfEx.00C29191
	$-D6     003BEB2A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-D0     003BEB30     50                   PUSH EAX
	$-CF     003BEB31     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-C8     003BEB38     83EC 0C              SUB ESP,0xC
	$-C5     003BEB3B     56                   PUSH ESI
	$-C4     003BEB3C     57                   PUSH EDI
	$-C3     003BEB3D     6A 01                PUSH 0x1
	$-C1     003BEB3F     6A 08                PUSH 0x8
	$-BF     003BEB41     8BF9                 MOV EDI,ECX
	$-BD     003BEB43     68 18010000          PUSH 0x118
	$-B8     003BEB48     897D F0              MOV DWORD PTR SS:[EBP-0x10],EDI
	$-B5     003BEB4B     6A 1C                PUSH 0x1C
	$-B3     003BEB4D     C745 F0 2A03F300     MOV DWORD PTR SS:[EBP-0x10],PathOfEx.00F3032A
	$-AC     003BEB54     E8 AC507E00          CALL PathOfEx.00BA3C05
	$-A7     003BEB59     8945 EC              MOV DWORD PTR SS:[EBP-0x14],EAX
	$-A4     003BEB5C     83C4 04              ADD ESP,0x4
	$-A1     003BEB5F     8945 E8              MOV DWORD PTR SS:[EBP-0x18],EAX
	$-9E     003BEB62     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-97     003BEB69     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-94     003BEB6C     85C0                 TEST EAX,EAX
	$-92     003BEB6E     74 2D                JE SHORT PathOfEx.003BEB9D
	$-90     003BEB70     837E 14 08           CMP DWORD PTR DS:[ESI+0x14],0x8
	$-8C     003BEB74     C745 08 2A03F300     MOV DWORD PTR SS:[EBP+0x8],PathOfEx.00F3032A
	$-85     003BEB7B     72 10                JB SHORT PathOfEx.003BEB8D
	$-83     003BEB7D     8B0E                 MOV ECX,DWORD PTR DS:[ESI]
	$-81     003BEB7F     8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-7E     003BEB82     52                   PUSH EDX
	$-7D     003BEB83     51                   PUSH ECX
	$-7C     003BEB84     8BC8                 MOV ECX,EAX
	$-7A     003BEB86     E8 95C0F7FF          CALL PathOfEx.0033AC20
	$-75     003BEB8B     EB 12                JMP SHORT PathOfEx.003BEB9F
	$-73     003BEB8D     8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-70     003BEB90     8BCE                 MOV ECX,ESI
	$-6E     003BEB92     52                   PUSH EDX
	$-6D     003BEB93     51                   PUSH ECX
	$-6C     003BEB94     8BC8                 MOV ECX,EAX
	$-6A     003BEB96     E8 85C0F7FF          CALL PathOfEx.0033AC20
	$-65     003BEB9B     EB 02                JMP SHORT PathOfEx.003BEB9F
	$-63     003BEB9D     33C0                 XOR EAX,EAX
	$-61     003BEB9F     50                   PUSH EAX
	$-60     003BEBA0     68 F0723300          PUSH PathOfEx.003372F0
	$-5B     003BEBA5     68 F0723300          PUSH PathOfEx.003372F0
	$-56     003BEBAA     68 309E3300          PUSH PathOfEx.00339E30
	$-51     003BEBAF     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-4A     003BEBB6     837E 14 08           CMP DWORD PTR DS:[ESI+0x14],0x8
	$-46     003BEBBA     68 80EE3B00          PUSH PathOfEx.003BEE80
	$-41     003BEBBF     68 F0ED3B00          PUSH PathOfEx.003BEDF0
	$-3C     003BEBC4     72 02                JB SHORT PathOfEx.003BEBC8
	$-3A     003BEBC6     8B36                 MOV ESI,DWORD PTR DS:[ESI]
	$-38     003BEBC8     83EC 1C              SUB ESP,0x1C
	$-35     003BEBCB     8D45 F0              LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-32     003BEBCE     8BCC                 MOV ECX,ESP
	$-30     003BEBD0     50                   PUSH EAX
	$-2F     003BEBD1     56                   PUSH ESI
	$-2E     003BEBD2     E8 49C0F7FF          CALL PathOfEx.0033AC20
	$-29     003BEBD7     8BCF                 MOV ECX,EDI
	$-27     003BEBD9     E8 026C2100          CALL PathOfEx.005D57E0
	$-22     003BEBDE     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1F     003BEBE1     8BC7                 MOV EAX,EDI
	$-1D     003BEBE3     C707 F4BCD800        MOV DWORD PTR DS:[EDI],PathOfEx.00D8BCF4
	$-17     003BEBE9     5F                   POP EDI
	$-16     003BEBEA     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-F      003BEBF1     5E                   POP ESI
	$-E      003BEBF2     8BE5                 MOV ESP,EBP
	$-C      003BEBF4     5D                   POP EBP
	$-B      003BEBF5     C2 1000              RETN 0x10
	$-8      003BEBF8     CC                   INT3
	$-7      003BEBF9     CC                   INT3
	$-6      003BEBFA     CC                   INT3
	$-5      003BEBFB     CC                   INT3
	$-4      003BEBFC     CC                   INT3
	$-3      003BEBFD     CC                   INT3
	$-2      003BEBFE     CC                   INT3
	$-1      003BEBFF     CC                   INT3
	$ ==>    003BEC00     8B4424 04            MOV EAX,DWORD PTR SS:[ESP+0x4]                  ; then,eax=stCD_ThrdParam*
	$+4      003BEC04     FF70 08              PUSH DWORD PTR DS:[EAX+0x8]
	$+7      003BEC07     8B40 04              MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+A      003BEC0A     FFD0                 CALL EAX                                        ; call stCD_ThrdParam::ThrdRun，此函数无参数
	$+C      003BEC0C     83C4 04              ADD ESP,0x4
	$+F      003BEC0F     33C0                 XOR EAX,EAX
	$+11     003BEC11     C2 0400              RETN 0x4
	$+14     003BEC14     CC                   INT3
	$+15     003BEC15     CC                   INT3
	$+16     003BEC16     CC                   INT3
	$+17     003BEC17     CC                   INT3
	$+18     003BEC18     CC                   INT3
	$+19     003BEC19     CC                   INT3
	$+1A     003BEC1A     CC                   INT3
	$+1B     003BEC1B     CC                   INT3
	$+1C     003BEC1C     CC                   INT3
	$+1D     003BEC1D     CC                   INT3
	$+1E     003BEC1E     CC                   INT3
	$+1F     003BEC1F     CC                   INT3
	$+20     003BEC20     55                   PUSH EBP
	$+21     003BEC21     8BEC                 MOV EBP,ESP
	$+23     003BEC23     83E4 F8              AND ESP,0xFFFFFFF8
	$+26     003BEC26     51                   PUSH ECX
	$+27     003BEC27     56                   PUSH ESI
	$+28     003BEC28     8BF1                 MOV ESI,ECX
	$+2A     003BEC2A     6A 00                PUSH 0x0
	$+2C     003BEC2C     8B46 54              MOV EAX,DWORD PTR DS:[ESI+0x54]
	$+2F     003BEC2F     8D4E 04              LEA ECX,DWORD PTR DS:[ESI+0x4]
	$+32     003BEC32     FF76 68              PUSH DWORD PTR DS:[ESI+0x68]
	$+35     003BEC35     8946 4C              MOV DWORD PTR DS:[ESI+0x4C],EAX
	$+38     003BEC38     8906                 MOV DWORD PTR DS:[ESI],EAX
	$+3A     003BEC3A     E8 D198FCFF          CALL PathOfEx.00388510
	$+3F     003BEC3F     FF76 68              PUSH DWORD PTR DS:[ESI+0x68]
	$+42     003BEC42     8D4E 14              LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+45     003BEC45     E8 3691FCFF          CALL PathOfEx.00387D80
	$+4A     003BEC4A     FF76 58              PUSH DWORD PTR DS:[ESI+0x58]
	$+4D     003BEC4D     8BCE                 MOV ECX,ESI
	$+4F     003BEC4F     66:C746 64 0000      MOV WORD PTR DS:[ESI+0x64],0x0
	$+55     003BEC55     C646 74 01           MOV BYTE PTR DS:[ESI+0x74],0x1
	$+59     003BEC59     C746 78 80969800     MOV DWORD PTR DS:[ESI+0x78],PathOfEx.00989680
	$+60     003BEC60     C746 7C E8030000     MOV DWORD PTR DS:[ESI+0x7C],0x3E8
	$+67     003BEC67     E8 F4020000          CALL PathOfEx.003BEF60
	$+6C     003BEC6C     84C0                 TEST AL,AL
	$+6E     003BEC6E     5E                   POP ESI
	$+6F     003BEC6F     0F95C0               SETNE AL
	$+72     003BEC72     8BE5                 MOV ESP,EBP
	$+74     003BEC74     5D                   POP EBP
	$+75     003BEC75     C2 0800              RETN 0x8
	$+78     003BEC78     CC                   INT3
	$+79     003BEC79     CC                   INT3
	$+7A     003BEC7A     CC                   INT3
	$+7B     003BEC7B     CC                   INT3
	$+7C     003BEC7C     CC                   INT3
	$+7D     003BEC7D     CC                   INT3
	$+7E     003BEC7E     CC                   INT3
	$+7F     003BEC7F     CC                   INT3
	$+80     003BEC80     55                   PUSH EBP
	$+81     003BEC81     8BEC                 MOV EBP,ESP
	$+83     003BEC83     6A FF                PUSH -0x1
	$+85     003BEC85     68 CA91C200          PUSH PathOfEx.00C291CA
	$+8A     003BEC8A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+90     003BEC90     50                   PUSH EAX
	$+91     003BEC91     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+98     003BEC98     83EC 24              SUB ESP,0x24
	$+9B     003BEC9B     53                   PUSH EBX
	$+9C     003BEC9C     33DB                 XOR EBX,EBX
	$+9E     003BEC9E     895D F0              MOV DWORD PTR SS:[EBP-0x10],EBX
	$+A1     003BECA1     56                   PUSH ESI
	$+A2     003BECA2     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+A5     003BECA5     8975 EC              MOV DWORD PTR SS:[EBP-0x14],ESI
	$+A8     003BECA8     895D FC              MOV DWORD PTR SS:[EBP-0x4],EBX
	$+AB     003BECAB     85F6                 TEST ESI,ESI
	$+AD     003BECAD     74 31                JE SHORT PathOfEx.003BECE0
	$+AF     003BECAF     8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+B2     003BECB2     83C0 04              ADD EAX,0x4
	$+B5     003BECB5     8378 14 08           CMP DWORD PTR DS:[EAX+0x14],0x8
	$+B9     003BECB9     72 02                JB SHORT PathOfEx.003BECBD
	$+BB     003BECBB     8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$+BD     003BECBD     50                   PUSH EAX
	$+BE     003BECBE     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+C1     003BECC1     E8 CAB9F7FF          CALL PathOfEx.0033A690
	$+C6     003BECC6     BB 01000000          MOV EBX,0x1
	$+CB     003BECCB     C645 FC 01           MOV BYTE PTR SS:[EBP-0x4],0x1
	$+CF     003BECCF     53                   PUSH EBX
	$+D0     003BECD0     68 C0AE7E00          PUSH PathOfEx.007EAEC0
	$+D5     003BECD5     50                   PUSH EAX
	$+D6     003BECD6     8BCE                 MOV ECX,ESI
	$+D8     003BECD8     895D F0              MOV DWORD PTR SS:[EBP-0x10],EBX
	$+DB     003BECDB     E8 50A94200          CALL PathOfEx.007E9630
	$+E0     003BECE0     F6C3 01              TEST BL,0x1
	$+E3     003BECE3     74 17                JE SHORT PathOfEx.003BECFC
	$+E5     003BECE5     8B45 E8              MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+E8     003BECE8     83F8 08              CMP EAX,0x8
	$+EB     003BECEB     72 0F                JB SHORT PathOfEx.003BECFC
	$+ED     003BECED     6A 02                PUSH 0x2
	$+EF     003BECEF     40                   INC EAX
	$+F0     003BECF0     50                   PUSH EAX
	$+F1     003BECF1     FF75 D4              PUSH DWORD PTR SS:[EBP-0x2C]
	$+F4     003BECF4     E8 B7A3F7FF          CALL PathOfEx.003390B0
	$+F9     003BECF9     83C4 0C              ADD ESP,0xC
	$+FC     003BECFC     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+FF     003BECFF     5E                   POP ESI
	$+100    003BED00     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+107    003BED07     5B                   POP EBX
	$+108    003BED08     8BE5                 MOV ESP,EBP
	$+10A    003BED0A     5D                   POP EBP
	$+10B    003BED0B     C3                   RETN
	$+10C    003BED0C     CC                   INT3
	$+10D    003BED0D     CC                   INT3
	$+10E    003BED0E     CC                   INT3
	$+10F    003BED0F     CC                   INT3
	$+110    003BED10     55                   PUSH EBP
	$+111    003BED11     8BEC                 MOV EBP,ESP
	$+113    003BED13     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+119    003BED19     6A FF                PUSH -0x1
	$+11B    003BED1B     68 0192C200          PUSH PathOfEx.00C29201
	$+120    003BED20     50                   PUSH EAX
	$+121    003BED21     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+128    003BED28     83EC 30              SUB ESP,0x30
	$+12B    003BED2B     8B55 0C              MOV EDX,DWORD PTR SS:[EBP+0xC]
	$+12E    003BED2E     83C2 04              ADD EDX,0x4
	$+131    003BED31     56                   PUSH ESI
	$+132    003BED32     837A 14 08           CMP DWORD PTR DS:[EDX+0x14],0x8
	$+136    003BED36     72 02                JB SHORT PathOfEx.003BED3A
	$+138    003BED38     8B12                 MOV EDX,DWORD PTR DS:[EDX]
	$+13A    003BED3A     33C0                 XOR EAX,EAX
	$+13C    003BED3C     C745 F0 07000000     MOV DWORD PTR SS:[EBP-0x10],0x7
	$+143    003BED43     C745 EC 00000000     MOV DWORD PTR SS:[EBP-0x14],0x0
	$+14A    003BED4A     66:8945 DC           MOV WORD PTR SS:[EBP-0x24],AX
	$+14E    003BED4E     66:3902              CMP WORD PTR DS:[EDX],AX
	$+151    003BED51     75 04                JNZ SHORT PathOfEx.003BED57
	$+153    003BED53     33C9                 XOR ECX,ECX
	$+155    003BED55     EB 18                JMP SHORT PathOfEx.003BED6F
	$+157    003BED57     8BCA                 MOV ECX,EDX
	$+159    003BED59     8D71 02              LEA ESI,DWORD PTR DS:[ECX+0x2]
	$+15C    003BED5C     0F1F40 00            NOP DWORD PTR DS:[EAX]
	$+160    003BED60     66:8B01              MOV AX,WORD PTR DS:[ECX]
	$+163    003BED63     83C1 02              ADD ECX,0x2
	$+166    003BED66     66:85C0              TEST AX,AX
	$+169    003BED69   ^ 75 F5                JNZ SHORT PathOfEx.003BED60
	$+16B    003BED6B     2BCE                 SUB ECX,ESI
	$+16D    003BED6D     D1F9                 SAR ECX,1
	$+16F    003BED6F     51                   PUSH ECX
	$+170    003BED70     52                   PUSH EDX
	$+171    003BED71     8D4D DC              LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+174    003BED74     E8 D7B7F7FF          CALL PathOfEx.0033A550
	$+179    003BED79     8D45 DC              LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+17C    003BED7C     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+183    003BED83     50                   PUSH EAX
	$+184    003BED84     8D45 C4              LEA EAX,DWORD PTR SS:[EBP-0x3C]
	$+187    003BED87     50                   PUSH EAX
	$+188    003BED88     E8 C3662100          CALL PathOfEx.005D5450
	$+18D    003BED8D     83C4 08              ADD ESP,0x8
	$+190    003BED90     8B45 F0              MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+193    003BED93     83F8 08              CMP EAX,0x8
	$+196    003BED96     72 0F                JB SHORT PathOfEx.003BEDA7
	$+198    003BED98     40                   INC EAX
	$+199    003BED99     6A 02                PUSH 0x2
	$+19B    003BED9B     50                   PUSH EAX
	$+19C    003BED9C     FF75 DC              PUSH DWORD PTR SS:[EBP-0x24]
	$+19F    003BED9F     E8 0CA3F7FF          CALL PathOfEx.003390B0
	$+1A4    003BEDA4     83C4 0C              ADD ESP,0xC
	$+1A7    003BEDA7     8B4D 08              MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+1AA    003BEDAA     894D 0C              MOV DWORD PTR SS:[EBP+0xC],ECX
	$+1AD    003BEDAD     C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$+1B1    003BEDB1     85C9                 TEST ECX,ECX
	$+1B3    003BEDB3     74 10                JE SHORT PathOfEx.003BEDC5
	$+1B5    003BEDB5     6A 01                PUSH 0x1
	$+1B7    003BEDB7     68 C0AE7E00          PUSH PathOfEx.007EAEC0
	$+1BC    003BEDBC     8D45 C4              LEA EAX,DWORD PTR SS:[EBP-0x3C]
	$+1BF    003BEDBF     50                   PUSH EAX
	$+1C0    003BEDC0     E8 6BA84200          CALL PathOfEx.007E9630
	$+1C5    003BEDC5     8B45 D8              MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+1C8    003BEDC8     83F8 08              CMP EAX,0x8
	$+1CB    003BEDCB     72 0F                JB SHORT PathOfEx.003BEDDC
	$+1CD    003BEDCD     6A 02                PUSH 0x2
	$+1CF    003BEDCF     40                   INC EAX
	$+1D0    003BEDD0     50                   PUSH EAX
	$+1D1    003BEDD1     FF75 C4              PUSH DWORD PTR SS:[EBP-0x3C]
	$+1D4    003BEDD4     E8 D7A2F7FF          CALL PathOfEx.003390B0
	$+1D9    003BEDD9     83C4 0C              ADD ESP,0xC
	$+1DC    003BEDDC     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1DF    003BEDDF     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+1E6    003BEDE6     5E                   POP ESI
	$+1E7    003BEDE7     8BE5                 MOV ESP,EBP
	$+1E9    003BEDE9     5D                   POP EBP
	$+1EA    003BEDEA     C3                   RETN
	$+1EB    003BEDEB     CC                   INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8B442404FF70088B4004FFD083C40433C0C20400", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_hfac_thrd_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-39E    0086BA7F    CC                        INT3
	$-39D    0086BA80    55                        PUSH EBP
	$-39C    0086BA81    8BEC                      MOV EBP,ESP
	$-39A    0086BA83    6A FF                     PUSH -0x1
	$-398    0086BA85    68 DC47C800               PUSH PathOfEx.00C847DC
	$-393    0086BA8A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-38D    0086BA90    50                        PUSH EAX
	$-38C    0086BA91    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-385    0086BA98    83EC 3C                   SUB ESP,0x3C
	$-382    0086BA9B    56                        PUSH ESI
	$-381    0086BA9C    8B75 0C                   MOV ESI,DWORD PTR SS:[EBP+0xC]
	$-37E    0086BA9F    57                        PUSH EDI
	$-37D    0086BAA0    8BF9                      MOV EDI,ECX
	$-37B    0086BAA2    C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-374    0086BAA9    56                        PUSH ESI
	$-373    0086BAAA    8BD7                      MOV EDX,EDI
	$-371    0086BAAC    8D4D C8                   LEA ECX,DWORD PTR SS:[EBP-0x38]
	$-36E    0086BAAF    E8 EC0FB1FF               CALL PathOfEx.0037CAA0
	$-369    0086BAB4    83C4 04                   ADD ESP,0x4
	$-366    0086BAB7    807F 30 00                CMP BYTE PTR DS:[EDI+0x30],0x0
	$-362    0086BABB    F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
	$-35E    0086BABF    F3:0F1145 D4              MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$-359    0086BAC4    F3:0F1040 04              MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$-354    0086BAC9    F3:0F1145 D8              MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$-34F    0086BACE    F3:0F1040 08              MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$-34A    0086BAD3    F3:0F1145 DC              MOVSS DWORD PTR SS:[EBP-0x24],XMM0
	$-345    0086BAD8    0F84 DD000000             JE PathOfEx.0086BBBB
	$-33F    0086BADE    56                        PUSH ESI
	$-33E    0086BADF    8D57 0C                   LEA EDX,DWORD PTR DS:[EDI+0xC]
	$-33B    0086BAE2    8D4D BC                   LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-338    0086BAE5    E8 E612D4FF               CALL PathOfEx.005ACDD0
	$-333    0086BAEA    83C4 04                   ADD ESP,0x4
	$-330    0086BAED    F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
	$-32C    0086BAF1    F3:0F1145 C8              MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$-327    0086BAF6    F3:0F1040 04              MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$-322    0086BAFB    F3:0F1145 CC              MOVSS DWORD PTR SS:[EBP-0x34],XMM0
	$-31D    0086BB00    F3:0F1040 08              MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$-318    0086BB05    68 6C010000               PUSH 0x16C
	$-313    0086BB0A    F3:0F1145 D0              MOVSS DWORD PTR SS:[EBP-0x30],XMM0
	$-30E    0086BB0F    E8 D15D3200               CALL PathOfEx.00B918E5
	$-309    0086BB14    8BF0                      MOV ESI,EAX
	$-307    0086BB16    83C4 04                   ADD ESP,0x4
	$-304    0086BB19    8975 E0                   MOV DWORD PTR SS:[EBP-0x20],ESI
	$-301    0086BB1C    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-2FA    0086BB23    8BCE                      MOV ECX,ESI
	$-2F8    0086BB25    F3:0F1047 24              MOVSS XMM0,DWORD PTR DS:[EDI+0x24]
	$-2F3    0086BB2A    8A47 31                   MOV AL,BYTE PTR DS:[EDI+0x31]
	$-2F0    0086BB2D    F3:0F1145 E8              MOVSS DWORD PTR SS:[EBP-0x18],XMM0
	$-2EB    0086BB32    F3:0F1047 28              MOVSS XMM0,DWORD PTR DS:[EDI+0x28]
	$-2E6    0086BB37    8845 0C                   MOV BYTE PTR SS:[EBP+0xC],AL
	$-2E3    0086BB3A    FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
	$-2E0    0086BB3D    F3:0F1145 EC              MOVSS DWORD PTR SS:[EBP-0x14],XMM0
	$-2DB    0086BB42    F3:0F1047 2C              MOVSS XMM0,DWORD PTR DS:[EDI+0x2C]
	$-2D6    0086BB47    68 88530601               PUSH PathOfEx.01065388
	$-2D1    0086BB4C    F3:0F1145 F0              MOVSS DWORD PTR SS:[EBP-0x10],XMM0
	$-2CC    0086BB51    E8 3A1C0200               CALL PathOfEx.0088D790
	$-2C7    0086BB56    F3:0F1045 F0              MOVSS XMM0,DWORD PTR SS:[EBP-0x10]
	$-2C2    0086BB5B    8D47 18                   LEA EAX,DWORD PTR DS:[EDI+0x18]
	$-2BF    0086BB5E    83EC 0C                   SUB ESP,0xC
	$-2BC    0086BB61    C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
	$-2B8    0086BB65    8BCE                      MOV ECX,ESI
	$-2B6    0086BB67    C706 801DE500             MOV DWORD PTR DS:[ESI],PathOfEx.00E51D80
	$-2B0    0086BB6D    C746 04 781DE500          MOV DWORD PTR DS:[ESI+0x4],PathOfEx.00E51D78
	$-2A9    0086BB74    C686 68010000 00          MOV BYTE PTR DS:[ESI+0x168],0x0
	$-2A2    0086BB7B    F3:0F114424 08            MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$-29C    0086BB81    F3:0F1045 EC              MOVSS XMM0,DWORD PTR SS:[EBP-0x14]
	$-297    0086BB86    F3:0F114424 04            MOVSS DWORD PTR SS:[ESP+0x4],XMM0
	$-291    0086BB8C    F3:0F1045 E8              MOVSS XMM0,DWORD PTR SS:[EBP-0x18]
	$-28C    0086BB91    F3:0F110424               MOVSS DWORD PTR SS:[ESP],XMM0
	$-287    0086BB96    50                        PUSH EAX
	$-286    0086BB97    8D45 C8                   LEA EAX,DWORD PTR SS:[EBP-0x38]
	$-283    0086BB9A    50                        PUSH EAX
	$-282    0086BB9B    8D45 D4                   LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-27F    0086BB9E    50                        PUSH EAX
	$-27E    0086BB9F    E8 4C360200               CALL PathOfEx.0088F1F0
	$-279    0086BBA4    8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-276    0086BBA7    8930                      MOV DWORD PTR DS:[EAX],ESI
	$-274    0086BBA9    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-271    0086BBAC    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-26A    0086BBB3    5F                        POP EDI
	$-269    0086BBB4    5E                        POP ESI
	$-268    0086BBB5    8BE5                      MOV ESP,EBP
	$-266    0086BBB7    5D                        POP EBP
	$-265    0086BBB8    C2 0800                   RETN 0x8
	$-262    0086BBBB    68 68010000               PUSH 0x168
	$-25D    0086BBC0    E8 205D3200               CALL PathOfEx.00B918E5
	$-258    0086BBC5    8945 0C                   MOV DWORD PTR SS:[EBP+0xC],EAX
	$-255    0086BBC8    8D4F 18                   LEA ECX,DWORD PTR DS:[EDI+0x18]
	$-252    0086BBCB    C745 FC 02000000          MOV DWORD PTR SS:[EBP-0x4],0x2
	$-24B    0086BBD2    F3:0F105F 24              MOVSS XMM3,DWORD PTR DS:[EDI+0x24]
	$-246    0086BBD7    51                        PUSH ECX
	$-245    0086BBD8    8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-242    0086BBDB    51                        PUSH ECX
	$-241    0086BBDC    8BC8                      MOV ECX,EAX
	$-23F    0086BBDE    E8 DD3B0200               CALL PathOfEx.0088F7C0
	$-23A    0086BBE3    8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-237    0086BBE6    5F                        POP EDI
	$-236    0086BBE7    5E                        POP ESI
	$-235    0086BBE8    8901                      MOV DWORD PTR DS:[ECX],EAX
	$-233    0086BBEA    8BC1                      MOV EAX,ECX
	$-231    0086BBEC    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-22E    0086BBEF    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-227    0086BBF6    8BE5                      MOV ESP,EBP
	$-225    0086BBF8    5D                        POP EBP
	$-224    0086BBF9    C2 0800                   RETN 0x8
	$-221    0086BBFC    CC                        INT3
	$-220    0086BBFD    CC                        INT3
	$-21F    0086BBFE    CC                        INT3
	$-21E    0086BBFF    CC                        INT3
	$-21D    0086BC00    55                        PUSH EBP
	$-21C    0086BC01    8BEC                      MOV EBP,ESP
	$-21A    0086BC03    83E4 F8                   AND ESP,0xFFFFFFF8
	$-217    0086BC06    83EC 24                   SUB ESP,0x24
	$-214    0086BC09    56                        PUSH ESI
	$-213    0086BC0A    8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-210    0086BC0D    8BD1                      MOV EDX,ECX
	$-20E    0086BC0F    56                        PUSH ESI
	$-20D    0086BC10    8D4C24 20                 LEA ECX,DWORD PTR SS:[ESP+0x20]
	$-209    0086BC14    E8 870EB1FF               CALL PathOfEx.0037CAA0
	$-204    0086BC19    83C4 04                   ADD ESP,0x4
	$-201    0086BC1C    807A 30 00                CMP BYTE PTR DS:[EDX+0x30],0x0
	$-1FD    0086BC20    F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
	$-1F9    0086BC24    F3:0F114424 04            MOVSS DWORD PTR SS:[ESP+0x4],XMM0
	$-1F3    0086BC2A    F3:0F1040 04              MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$-1EE    0086BC2F    F3:0F114424 08            MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$-1E8    0086BC35    F3:0F1040 08              MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$-1E3    0086BC3A    F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$-1DD    0086BC40    0F84 AF000000             JE PathOfEx.0086BCF5
	$-1D7    0086BC46    F3:0F1042 0C              MOVSS XMM0,DWORD PTR DS:[EDX+0xC]
	$-1D2    0086BC4B    83EC 0C                   SUB ESP,0xC
	$-1CF    0086BC4E    0F1026                    MOVUPS XMM4,DQWORD PTR DS:[ESI]
	$-1CC    0086BC51    8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-1C9    0086BC54    0F1056 10                 MOVUPS XMM2,DQWORD PTR DS:[ESI+0x10]
	$-1C5    0086BC58    0F106E 20                 MOVUPS XMM5,DQWORD PTR DS:[ESI+0x20]
	$-1C1    0086BC5C    0F28D8                    MOVAPS XMM3,XMM0
	$-1BE    0086BC5F    F3:0F1042 10              MOVSS XMM0,DWORD PTR DS:[EDX+0x10]
	$-1B9    0086BC64    0F28C8                    MOVAPS XMM1,XMM0
	$-1B6    0086BC67    F3:0F1042 14              MOVSS XMM0,DWORD PTR DS:[EDX+0x14]
	$-1B1    0086BC6C    0F14D9                    UNPCKLPS XMM3,XMM1
	$-1AE    0086BC6F    0F28C8                    MOVAPS XMM1,XMM0
	$-1AB    0086BC72    0F16D9                    MOVLHPS XMM3,XMM1
	$-1A8    0086BC75    0F28F3                    MOVAPS XMM6,XMM3
	$-1A5    0086BC78    0F28C3                    MOVAPS XMM0,XMM3
	$-1A2    0086BC7B    0FC6C3 00                 SHUFPS XMM0,XMM3,0x0
	$-19E    0086BC7F    0FC6F3 55                 SHUFPS XMM6,XMM3,0x55
	$-19A    0086BC83    0F59C4                    MULPS XMM0,XMM4
	$-197    0086BC86    0F59F2                    MULPS XMM6,XMM2
	$-194    0086BC89    0FC6DB AA                 SHUFPS XMM3,XMM3,0xAA
	$-190    0086BC8D    0F59DD                    MULPS XMM3,XMM5
	$-18D    0086BC90    0F58F0                    ADDPS XMM6,XMM0
	$-18A    0086BC93    0F58F3                    ADDPS XMM6,XMM3
	$-187    0086BC96    0F28C6                    MOVAPS XMM0,XMM6
	$-184    0086BC99    0F28CE                    MOVAPS XMM1,XMM6
	$-181    0086BC9C    0FC6C6 AA                 SHUFPS XMM0,XMM6,0xAA
	$-17D    0086BCA0    F3:0F114424 30            MOVSS DWORD PTR SS:[ESP+0x30],XMM0
	$-177    0086BCA6    F3:0F1042 2C              MOVSS XMM0,DWORD PTR DS:[EDX+0x2C]
	$-172    0086BCAB    8B4424 30                 MOV EAX,DWORD PTR SS:[ESP+0x30]
	$-16E    0086BCAF    F3:0F114424 08            MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$-168    0086BCB5    F3:0F1042 28              MOVSS XMM0,DWORD PTR DS:[EDX+0x28]
	$-163    0086BCBA    F3:0F114424 04            MOVSS DWORD PTR SS:[ESP+0x4],XMM0
	$-15D    0086BCC0    F3:0F1042 24              MOVSS XMM0,DWORD PTR DS:[EDX+0x24]
	$-158    0086BCC5    894424 24                 MOV DWORD PTR SS:[ESP+0x24],EAX
	$-154    0086BCC9    8D42 18                   LEA EAX,DWORD PTR DS:[EDX+0x18]
	$-151    0086BCCC    F3:0F110424               MOVSS DWORD PTR SS:[ESP],XMM0
	$-14C    0086BCD1    50                        PUSH EAX
	$-14B    0086BCD2    8D4424 20                 LEA EAX,DWORD PTR SS:[ESP+0x20]
	$-147    0086BCD6    0FC6CE 55                 SHUFPS XMM1,XMM6,0x55
	$-143    0086BCDA    50                        PUSH EAX
	$-142    0086BCDB    8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-13E    0086BCDF    0F14F1                    UNPCKLPS XMM6,XMM1
	$-13B    0086BCE2    50                        PUSH EAX
	$-13A    0086BCE3    660FD67424 28             MOVQ QWORD PTR SS:[ESP+0x28],XMM6
	$-134    0086BCE9    E8 02350200               CALL PathOfEx.0088F1F0
	$-12F    0086BCEE    5E                        POP ESI
	$-12E    0086BCEF    8BE5                      MOV ESP,EBP
	$-12C    0086BCF1    5D                        POP EBP
	$-12B    0086BCF2    C2 0800                   RETN 0x8
	$-128    0086BCF5    F3:0F1052 24              MOVSS XMM2,DWORD PTR DS:[EDX+0x24]
	$-123    0086BCFA    8D42 18                   LEA EAX,DWORD PTR DS:[EDX+0x18]
	$-120    0086BCFD    8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-11D    0086BD00    50                        PUSH EAX
	$-11C    0086BD01    8D4424 08                 LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-118    0086BD05    50                        PUSH EAX
	$-117    0086BD06    E8 D53C0200               CALL PathOfEx.0088F9E0
	$-112    0086BD0B    5E                        POP ESI
	$-111    0086BD0C    8BE5                      MOV ESP,EBP
	$-10F    0086BD0E    5D                        POP EBP
	$-10E    0086BD0F    C2 0800                   RETN 0x8
	$-10B    0086BD12    CC                        INT3
	$-10A    0086BD13    CC                        INT3
	$-109    0086BD14    CC                        INT3
	$-108    0086BD15    CC                        INT3
	$-107    0086BD16    CC                        INT3
	$-106    0086BD17    CC                        INT3
	$-105    0086BD18    CC                        INT3
	$-104    0086BD19    CC                        INT3
	$-103    0086BD1A    CC                        INT3
	$-102    0086BD1B    CC                        INT3
	$-101    0086BD1C    CC                        INT3
	$-100    0086BD1D    CC                        INT3
	$-FF     0086BD1E    CC                        INT3
	$-FE     0086BD1F    CC                        INT3
	$-FD     0086BD20    CC                        INT3
	$-FC     0086BD21    CC                        INT3
	$-FB     0086BD22    CC                        INT3
	$-FA     0086BD23    CC                        INT3
	$-F9     0086BD24    CC                        INT3
	$-F8     0086BD25    CC                        INT3
	$-F7     0086BD26    CC                        INT3
	$-F6     0086BD27    CC                        INT3
	$-F5     0086BD28    CC                        INT3
	$-F4     0086BD29    CC                        INT3
	$-F3     0086BD2A    CC                        INT3
	$-F2     0086BD2B    CC                        INT3
	$-F1     0086BD2C    CC                        INT3
	$-F0     0086BD2D    CC                        INT3
	$-EF     0086BD2E    CC                        INT3
	$-EE     0086BD2F    CC                        INT3
	$-ED     0086BD30    55                        PUSH EBP                                        ; tag_获得机器码_cpuid
	$-EC     0086BD31    8BEC                      MOV EBP,ESP
	$-EA     0086BD33    6A FF                     PUSH -0x1
	$-E8     0086BD35    68 5E48C800               PUSH PathOfEx.00C8485E
	$-E3     0086BD3A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-DD     0086BD40    50                        PUSH EAX
	$-DC     0086BD41    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-D5     0086BD48    83EC 7C                   SUB ESP,0x7C
	$-D2     0086BD4B    53                        PUSH EBX
	$-D1     0086BD4C    56                        PUSH ESI
	$-D0     0086BD4D    57                        PUSH EDI
	$-CF     0086BD4E    C745 E8 C0470901          MOV DWORD PTR SS:[EBP-0x18],PathOfEx.010947C0
	$-C8     0086BD55    C705 E0470901 0F000000    MOV DWORD PTR DS:[0x10947E0],0xF
	$-BE     0086BD5F    C705 DC470901 00000000    MOV DWORD PTR DS:[0x10947DC],0x0
	$-B4     0086BD69    C605 CC470901 00          MOV BYTE PTR DS:[0x10947CC],0x0
	$-AD     0086BD70    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-A6     0086BD77    C705 F8470901 0F000000    MOV DWORD PTR DS:[0x10947F8],0xF
	$-9C     0086BD81    C705 F4470901 00000000    MOV DWORD PTR DS:[0x10947F4],0x0
	$-92     0086BD8B    C605 E4470901 00          MOV BYTE PTR DS:[0x10947E4],0x0
	$-8B     0086BD92    C705 FC470901 00000000    MOV DWORD PTR DS:[0x10947FC],0x0
	$-81     0086BD9C    C705 00480901 00000000    MOV DWORD PTR DS:[0x1094800],0x0
	$-77     0086BDA6    C705 04480901 00000000    MOV DWORD PTR DS:[0x1094804],0x0
	$-6D     0086BDB0    C705 08480901 00000000    MOV DWORD PTR DS:[0x1094808],0x0
	$-63     0086BDBA    C705 0C480901 00000000    MOV DWORD PTR DS:[0x109480C],0x0
	$-59     0086BDC4    C705 10480901 00000000    MOV DWORD PTR DS:[0x1094810],0x0
	$-4F     0086BDCE    C705 14480901 00000000    MOV DWORD PTR DS:[0x1094814],0x0
	$-45     0086BDD8    C705 18480901 00000000    MOV DWORD PTR DS:[0x1094818],0x0
	$-3B     0086BDE2    C705 1C480901 00000000    MOV DWORD PTR DS:[0x109481C],0x0
	$-31     0086BDEC    C705 20480901 00000000    MOV DWORD PTR DS:[0x1094820],0x0
	$-27     0086BDF6    C705 24480901 00000000    MOV DWORD PTR DS:[0x1094824],0x0
	$-1D     0086BE00    C705 28480901 00000000    MOV DWORD PTR DS:[0x1094828],0x0
	$-13     0086BE0A    33C0                      XOR EAX,EAX
	$-11     0086BE0C    C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$-D      0086BE10    33C9                      XOR ECX,ECX
	$-B      0086BE12    C705 C4470901 00000000    MOV DWORD PTR DS:[0x10947C4],0x0
	$-1      0086BE1C    53                        PUSH EBX
	$ ==>    0086BE1D    0FA2                      CPUID                                           ; g_h_cpuid1_
	$+2      0086BE1F    8BF3                      MOV ESI,EBX
	$+4      0086BE21    66:C705 C8470901 0000     MOV WORD PTR DS:[0x10947C8],0x0
	$+D      0086BE2A    8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$+10     0086BE2D    5B                        POP EBX
	$+11     0086BE2E    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+13     0086BE30    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+16     0086BE33    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+19     0086BE36    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+1C     0086BE39    33FF                      XOR EDI,EDI
	$+1E     0086BE3B    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+21     0086BE3E    A3 C0470901               MOV DWORD PTR DS:[0x10947C0],EAX
	$+26     0086BE43    85C0                      TEST EAX,EAX
	$+28     0086BE45    78 33                     JS SHORT PathOfEx.0086BE7A
	$+2A     0086BE47    8BC7                      MOV EAX,EDI
	$+2C     0086BE49    33C9                      XOR ECX,ECX
	$+2E     0086BE4B    53                        PUSH EBX
	$+2F     0086BE4C    0FA2                      CPUID                                           ; g_h_cpuid2_
	$+31     0086BE4E    895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$+34     0086BE51    5B                        POP EBX
	$+35     0086BE52    8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+38     0086BE55    8906                      MOV DWORD PTR DS:[ESI],EAX
	$+3A     0086BE57    8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+3D     0086BE5A    8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+40     0086BE5D    8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+43     0086BE60    8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+46     0086BE63    B9 14480901               MOV ECX,PathOfEx.01094814
	$+4B     0086BE68    50                        PUSH EAX
	$+4C     0086BE69    8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+4F     0086BE6C    E8 CF26F1FF               CALL PathOfEx.0077E540
	$+54     0086BE71    47                        INC EDI
	$+55     0086BE72    3B3D C0470901             CMP EDI,DWORD PTR DS:[0x10947C0]
	$+5B     0086BE78  ^ 7E CD                     JLE SHORT PathOfEx.0086BE47
	$+5D     0086BE7A    8B15 14480901             MOV EDX,DWORD PTR DS:[0x1094814]
	$+63     0086BE80    0F57C0                    XORPS XMM0,XMM0
	$+66     0086BE83    0F1145 B8                 MOVUPS DQWORD PTR SS:[EBP-0x48],XMM0
	$+6A     0086BE87    0F1145 C8                 MOVUPS DQWORD PTR SS:[EBP-0x38],XMM0
	$+6E     0086BE8B    8B4A 04                   MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+71     0086BE8E    894D B8                   MOV DWORD PTR SS:[EBP-0x48],ECX
	$+74     0086BE91    8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+77     0086BE94    8945 BC                   MOV DWORD PTR SS:[EBP-0x44],EAX
	$+7A     0086BE97    8B42 08                   MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+7D     0086BE9A    8945 C0                   MOV DWORD PTR SS:[EBP-0x40],EAX
	$+80     0086BE9D    84C9                      TEST CL,CL
	$+82     0086BE9F    75 04                     JNZ SHORT PathOfEx.0086BEA5
	$+84     0086BEA1    33C9                      XOR ECX,ECX
	$+86     0086BEA3    EB 14                     JMP SHORT PathOfEx.0086BEB9
	$+88     0086BEA5    8D4D B8                   LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+8B     0086BEA8    8D51 01                   LEA EDX,DWORD PTR DS:[ECX+0x1]
	$+8E     0086BEAB    0F1F4400 00               NOP DWORD PTR DS:[EAX+EAX]
	$+93     0086BEB0    8A01                      MOV AL,BYTE PTR DS:[ECX]
	$+95     0086BEB2    41                        INC ECX
	$+96     0086BEB3    84C0                      TEST AL,AL
	$+98     0086BEB5  ^ 75 F9                     JNZ SHORT PathOfEx.0086BEB0
	$+9A     0086BEB7    2BCA                      SUB ECX,EDX
	$+9C     0086BEB9    51                        PUSH ECX
	$+9D     0086BEBA    8D45 B8                   LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+A0     0086BEBD    BE CC470901               MOV ESI,PathOfEx.010947CC
	$+A5     0086BEC2    50                        PUSH EAX
	$+A6     0086BEC3    8BCE                      MOV ECX,ESI
	$+A8     0086BEC5    E8 56D3ADFF               CALL PathOfEx.00349220
	$+AD     0086BECA    833D E0470901 10          CMP DWORD PTR DS:[0x10947E0],0x10
	$+B4     0086BED1    8BCE                      MOV ECX,ESI
	$+B6     0086BED3    8B35 DC470901             MOV ESI,DWORD PTR DS:[0x10947DC]
	$+BC     0086BED9    B8 0C000000               MOV EAX,0xC
	$+C1     0086BEDE    0F430D CC470901           CMOVNB ECX,DWORD PTR DS:[0x10947CC]
	$+C8     0086BEE5    3BF0                      CMP ESI,EAX
	$+CA     0086BEE7    0F42C6                    CMOVB EAX,ESI
	$+CD     0086BEEA    50                        PUSH EAX
	$+CE     0086BEEB    68 1C18E200               PUSH PathOfEx.00E2181C                          ; ASCII "GenuineIntel"
	$+D3     0086BEF0    51                        PUSH ECX
	$+D4     0086BEF1    E8 8A9BAFFF               CALL PathOfEx.00365A80
	$+D9     0086BEF6    83C4 0C                   ADD ESP,0xC
	$+DC     0086BEF9    BF 01000000               MOV EDI,0x1
	$+E1     0086BEFE    85C0                      TEST EAX,EAX
	$+E3     0086BF00    75 10                     JNZ SHORT PathOfEx.0086BF12
	$+E5     0086BF02    83FE 0C                   CMP ESI,0xC
	$+E8     0086BF05    72 0B                     JB SHORT PathOfEx.0086BF12
	$+EA     0086BF07    77 09                     JA SHORT PathOfEx.0086BF12
	$+EC     0086BF09    C605 C8470901 01          MOV BYTE PTR DS:[0x10947C8],0x1
	$+F3     0086BF10    EB 45                     JMP SHORT PathOfEx.0086BF57
	$+F5     0086BF12    833D E0470901 10          CMP DWORD PTR DS:[0x10947E0],0x10
	$+FC     0086BF19    B8 CC470901               MOV EAX,PathOfEx.010947CC
	$+101    0086BF1E    B9 0C000000               MOV ECX,0xC
	$+106    0086BF23    0F4305 CC470901           CMOVNB EAX,DWORD PTR DS:[0x10947CC]
	$+10D    0086BF2A    83FE 0C                   CMP ESI,0xC
	$+110    0086BF2D    0F42CE                    CMOVB ECX,ESI
	$+113    0086BF30    51                        PUSH ECX
	$+114    0086BF31    68 2C18E200               PUSH PathOfEx.00E2182C                          ; ASCII "AuthenticAMD"
	$+119    0086BF36    50                        PUSH EAX
	$+11A    0086BF37    E8 449BAFFF               CALL PathOfEx.00365A80
	$+11F    0086BF3C    83C4 0C                   ADD ESP,0xC
	$+122    0086BF3F    85C0                      TEST EAX,EAX
	$+124    0086BF41    75 14                     JNZ SHORT PathOfEx.0086BF57
	$+126    0086BF43    83FE 0C                   CMP ESI,0xC
	$+129    0086BF46    72 0F                     JB SHORT PathOfEx.0086BF57
	$+12B    0086BF48    0FB605 C9470901           MOVZX EAX,BYTE PTR DS:[0x10947C9]
	$+132    0086BF4F    0F46C7                    CMOVBE EAX,EDI
	$+135    0086BF52    A2 C9470901               MOV BYTE PTR DS:[0x10947C9],AL
	$+13A    0086BF57    8B35 C0470901             MOV ESI,DWORD PTR DS:[0x10947C0]
	$+140    0086BF5D    8B0D 14480901             MOV ECX,DWORD PTR DS:[0x1094814]
	$+146    0086BF63    83FE 01                   CMP ESI,0x1
	$+149    0086BF66    7C 18                     JL SHORT PathOfEx.0086BF80
	$+14B    0086BF68    8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$+14E    0086BF6B    99                        CDQ
	$+14F    0086BF6C    A3 FC470901               MOV DWORD PTR DS:[0x10947FC],EAX
	$+154    0086BF71    8B41 1C                   MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$+157    0086BF74    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+15A    0086BF77    99                        CDQ
	$+15B    0086BF78    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+15E    0086BF7B    A3 00480901               MOV DWORD PTR DS:[0x1094800],EAX
	$+163    0086BF80    83FE 07                   CMP ESI,0x7
	$+166    0086BF83    7C 18                     JL SHORT PathOfEx.0086BF9D
	$+168    0086BF85    8B41 74                   MOV EAX,DWORD PTR DS:[ECX+0x74]
	$+16B    0086BF88    99                        CDQ
	$+16C    0086BF89    A3 04480901               MOV DWORD PTR DS:[0x1094804],EAX
	$+171    0086BF8E    8B41 78                   MOV EAX,DWORD PTR DS:[ECX+0x78]
	$+174    0086BF91    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+177    0086BF94    99                        CDQ
	$+178    0086BF95    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+17B    0086BF98    A3 08480901               MOV DWORD PTR DS:[0x1094808],EAX
	$+180    0086BF9D    B8 00000080               MOV EAX,0x80000000
	$+185    0086BFA2    8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$+188    0086BFA5    33C9                      XOR ECX,ECX
	$+18A    0086BFA7    53                        PUSH EBX
	$+18B    0086BFA8    0FA2                      CPUID                                           ; g_h_cpuid3_
	$+18D    0086BFAA    8BF3                      MOV ESI,EBX
	$+18F    0086BFAC    5B                        POP EBX
	$+190    0086BFAD    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+192    0086BFAF    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+195    0086BFB2    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+198    0086BFB5    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+19B    0086BFB8    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+19E    0086BFBB    6A 40                     PUSH 0x40
	$+1A0    0086BFBD    A3 C4470901               MOV DWORD PTR DS:[0x10947C4],EAX
	$+1A5    0086BFC2    8D85 78FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x88]
	$+1AB    0086BFC8    6A 00                     PUSH 0x0
	$+1AD    0086BFCA    50                        PUSH EAX
	$+1AE    0086BFCB    E8 50EA3300               CALL PathOfEx.00BAAA20
	$+1B3    0086BFD0    83C4 0C                   ADD ESP,0xC
	$+1B6    0086BFD3    BF 00000080               MOV EDI,0x80000000
	$+1BB    0086BFD8    8BC7                      MOV EAX,EDI
	$+1BD    0086BFDA    33C9                      XOR ECX,ECX
	$+1BF    0086BFDC    53                        PUSH EBX
	$+1C0    0086BFDD    0FA2                      CPUID                                           ; g_h_cpuid4_
	$+1C2    0086BFDF    895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$+1C5    0086BFE2    5B                        POP EBX
	$+1C6    0086BFE3    8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+1C9    0086BFE6    8906                      MOV DWORD PTR DS:[ESI],EAX
	$+1CB    0086BFE8    8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+1CE    0086BFEB    8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+1D1    0086BFEE    8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+1D4    0086BFF1    8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+1D7    0086BFF4    B9 20480901               MOV ECX,PathOfEx.01094820
	$+1DC    0086BFF9    50                        PUSH EAX
	$+1DD    0086BFFA    8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+1E0    0086BFFD    E8 3E25F1FF               CALL PathOfEx.0077E540
	$+1E5    0086C002    8B0D C4470901             MOV ECX,DWORD PTR DS:[0x10947C4]
	$+1EB    0086C008    47                        INC EDI
	$+1EC    0086C009    3BF9                      CMP EDI,ECX
	$+1EE    0086C00B  ^ 7E CB                     JLE SHORT PathOfEx.0086BFD8
	$+1F0    0086C00D    8B35 20480901             MOV ESI,DWORD PTR DS:[0x1094820]
	$+1F6    0086C013    BF 01000000               MOV EDI,0x1
	$+1FB    0086C018    81F9 01000080             CMP ECX,0x80000001
	$+201    0086C01E    72 18                     JB SHORT PathOfEx.0086C038
	$+203    0086C020    8B46 18                   MOV EAX,DWORD PTR DS:[ESI+0x18]
	$+206    0086C023    99                        CDQ
	$+207    0086C024    A3 0C480901               MOV DWORD PTR DS:[0x109480C],EAX
	$+20C    0086C029    8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+20F    0086C02C    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+212    0086C02F    99                        CDQ
	$+213    0086C030    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+216    0086C033    A3 10480901               MOV DWORD PTR DS:[0x1094810],EAX
	$+21B    0086C038    81F9 04000080             CMP ECX,0x80000004
	$+221    0086C03E    72 4B                     JB SHORT PathOfEx.0086C08B
	$+223    0086C040    0F104E 20                 MOVUPS XMM1,DQWORD PTR DS:[ESI+0x20]
	$+227    0086C044    0F118D 78FFFFFF           MOVUPS DQWORD PTR SS:[EBP-0x88],XMM1
	$+22E    0086C04B    0F1046 30                 MOVUPS XMM0,DQWORD PTR DS:[ESI+0x30]
	$+232    0086C04F    66:0F7EC8                 MOVD EAX,MM1
	$+236    0086C053    0F1145 88                 MOVUPS DQWORD PTR SS:[EBP-0x78],XMM0
	$+23A    0086C057    0F1046 40                 MOVUPS XMM0,DQWORD PTR DS:[ESI+0x40]
	$+23E    0086C05B    0F1145 98                 MOVUPS DQWORD PTR SS:[EBP-0x68],XMM0
	$+242    0086C05F    84C0                      TEST AL,AL
	$+244    0086C061    75 04                     JNZ SHORT PathOfEx.0086C067
	$+246    0086C063    33C9                      XOR ECX,ECX
	$+248    0086C065    EB 12                     JMP SHORT PathOfEx.0086C079
	$+24A    0086C067    8D8D 78FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+250    0086C06D    8D51 01                   LEA EDX,DWORD PTR DS:[ECX+0x1]
	$+253    0086C070    8A01                      MOV AL,BYTE PTR DS:[ECX]
	$+255    0086C072    41                        INC ECX
	$+256    0086C073    84C0                      TEST AL,AL
	$+258    0086C075  ^ 75 F9                     JNZ SHORT PathOfEx.0086C070
	$+25A    0086C077    2BCA                      SUB ECX,EDX
	$+25C    0086C079    51                        PUSH ECX
	$+25D    0086C07A    8D85 78FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x88]
	$+263    0086C080    B9 E4470901               MOV ECX,PathOfEx.010947E4
	$+268    0086C085    50                        PUSH EAX
	$+269    0086C086    E8 95D1ADFF               CALL PathOfEx.00349220
	$+26E    0086C08B    6A 19                     PUSH 0x19
	$+270    0086C08D    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+273    0086C090    B9 00480901               MOV ECX,PathOfEx.01094800
	$+278    0086C095    50                        PUSH EAX
	$+279    0086C096    E8 15E0F3FF               CALL PathOfEx.007AA0B0
	$+27E    0086C09B    C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
	$+282    0086C09F    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+285    0086C0A2    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+287    0086C0A4    83FA 20                   CMP EDX,0x20
	$+28A    0086C0A7    72 0A                     JB SHORT PathOfEx.0086C0B3
	$+28C    0086C0A9    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+291    0086C0AE    E8 5A353100               CALL PathOfEx.00B7F60D
	$+296    0086C0B3    8BCA                      MOV ECX,EDX
	$+298    0086C0B5    8BC7                      MOV EAX,EDI
	$+29A    0086C0B7    83E1 1F                   AND ECX,0x1F
	$+29D    0086C0BA    C1EA 05                   SHR EDX,0x5
	$+2A0    0086C0BD    D3E0                      SHL EAX,CL
	$+2A2    0086C0BF    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+2A5    0086C0C2    0F84 38010000             JE PathOfEx.0086C200
	$+2AB    0086C0C8    6A 1A                     PUSH 0x1A
	$+2AD    0086C0CA    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+2B0    0086C0CD    B9 00480901               MOV ECX,PathOfEx.01094800
	$+2B5    0086C0D2    50                        PUSH EAX
	$+2B6    0086C0D3    E8 D8DFF3FF               CALL PathOfEx.007AA0B0
	$+2BB    0086C0D8    C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$+2BF    0086C0DC    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+2C2    0086C0DF    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+2C4    0086C0E1    83FA 20                   CMP EDX,0x20
	$+2C7    0086C0E4    72 0A                     JB SHORT PathOfEx.0086C0F0
	$+2C9    0086C0E6    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+2CE    0086C0EB    E8 1D353100               CALL PathOfEx.00B7F60D
	$+2D3    0086C0F0    8BCA                      MOV ECX,EDX
	$+2D5    0086C0F2    8BC7                      MOV EAX,EDI
	$+2D7    0086C0F4    83E1 1F                   AND ECX,0x1F
	$+2DA    0086C0F7    C1EA 05                   SHR EDX,0x5
	$+2DD    0086C0FA    D3E0                      SHL EAX,CL
	$+2DF    0086C0FC    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+2E2    0086C0FF    0F84 FB000000             JE PathOfEx.0086C200
	$+2E8    0086C105    6A 00                     PUSH 0x0
	$+2EA    0086C107    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+2ED    0086C10A    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+2F2    0086C10F    50                        PUSH EAX
	$+2F3    0086C110    E8 9BDFF3FF               CALL PathOfEx.007AA0B0
	$+2F8    0086C115    C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
	$+2FC    0086C119    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+2FF    0086C11C    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+301    0086C11E    83FA 20                   CMP EDX,0x20
	$+304    0086C121    72 0A                     JB SHORT PathOfEx.0086C12D
	$+306    0086C123    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+30B    0086C128    E8 E0343100               CALL PathOfEx.00B7F60D
	$+310    0086C12D    8BCA                      MOV ECX,EDX
	$+312    0086C12F    8BC7                      MOV EAX,EDI
	$+314    0086C131    83E1 1F                   AND ECX,0x1F
	$+317    0086C134    C1EA 05                   SHR EDX,0x5
	$+31A    0086C137    D3E0                      SHL EAX,CL
	$+31C    0086C139    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+31F    0086C13C    0F84 BE000000             JE PathOfEx.0086C200
	$+325    0086C142    6A 13                     PUSH 0x13
	$+327    0086C144    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+32A    0086C147    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+32F    0086C14C    50                        PUSH EAX
	$+330    0086C14D    E8 5EDFF3FF               CALL PathOfEx.007AA0B0
	$+335    0086C152    C645 FC 07                MOV BYTE PTR SS:[EBP-0x4],0x7
	$+339    0086C156    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+33C    0086C159    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+33E    0086C15B    83FA 20                   CMP EDX,0x20
	$+341    0086C15E    72 0A                     JB SHORT PathOfEx.0086C16A
	$+343    0086C160    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+348    0086C165    E8 A3343100               CALL PathOfEx.00B7F60D
	$+34D    0086C16A    8BCA                      MOV ECX,EDX
	$+34F    0086C16C    8BC7                      MOV EAX,EDI
	$+351    0086C16E    83E1 1F                   AND ECX,0x1F
	$+354    0086C171    C1EA 05                   SHR EDX,0x5
	$+357    0086C174    D3E0                      SHL EAX,CL
	$+359    0086C176    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+35C    0086C179    0F84 81000000             JE PathOfEx.0086C200
	$+362    0086C17F    6A 1C                     PUSH 0x1C
	$+364    0086C181    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+367    0086C184    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+36C    0086C189    50                        PUSH EAX
	$+36D    0086C18A    E8 21DFF3FF               CALL PathOfEx.007AA0B0
	$+372    0086C18F    C645 FC 08                MOV BYTE PTR SS:[EBP-0x4],0x8
	$+376    0086C193    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+379    0086C196    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+37B    0086C198    83FA 20                   CMP EDX,0x20
	$+37E    0086C19B    72 0A                     JB SHORT PathOfEx.0086C1A7
	$+380    0086C19D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+385    0086C1A2    E8 66343100               CALL PathOfEx.00B7F60D
	$+38A    0086C1A7    8BCA                      MOV ECX,EDX
	$+38C    0086C1A9    C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+390    0086C1AD    83E1 1F                   AND ECX,0x1F
	$+393    0086C1B0    C1EA 05                   SHR EDX,0x5
	$+396    0086C1B3    8BC7                      MOV EAX,EDI
	$+398    0086C1B5    D3E0                      SHL EAX,CL
	$+39A    0086C1B7    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+39D    0086C1BA    74 44                     JE SHORT PathOfEx.0086C200
	$+39F    0086C1BC    E8 2F030000               CALL PathOfEx.0086C4F0
	$+3A4    0086C1C1    84C0                      TEST AL,AL
	$+3A6    0086C1C3    74 3B                     JE SHORT PathOfEx.0086C200
	$+3A8    0086C1C5    E8 D6010000               CALL PathOfEx.0086C3A0
	$+3AD    0086C1CA    84C0                      TEST AL,AL
	$+3AF    0086C1CC    74 32                     JE SHORT PathOfEx.0086C200
	$+3B1    0086C1CE    E8 AD020000               CALL PathOfEx.0086C480
	$+3B6    0086C1D3    84C0                      TEST AL,AL
	$+3B8    0086C1D5    74 29                     JE SHORT PathOfEx.0086C200
	$+3BA    0086C1D7    E8 34020000               CALL PathOfEx.0086C410
	$+3BF    0086C1DC    84C0                      TEST AL,AL
	$+3C1    0086C1DE    74 20                     JE SHORT PathOfEx.0086C200
	$+3C3    0086C1E0    33C9                      XOR ECX,ECX
	$+3C5    0086C1E2    0F01D0                    XGETBV
	$+3C8    0086C1E5    83E0 06                   AND EAX,0x6
	$+3CB    0086C1E8    83F8 06                   CMP EAX,0x6
	$+3CE    0086C1EB    75 13                     JNZ SHORT PathOfEx.0086C200
	$+3D0    0086C1ED    85C9                      TEST ECX,ECX
	$+3D2    0086C1EF    75 0F                     JNZ SHORT PathOfEx.0086C200
	$+3D4    0086C1F1    C705 2C480901 04000000    MOV DWORD PTR DS:[0x109482C],0x4
	$+3DE    0086C1FB    E9 87010000               JMP PathOfEx.0086C387
	$+3E3    0086C200    6A 19                     PUSH 0x19
	$+3E5    0086C202    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+3E8    0086C205    B9 00480901               MOV ECX,PathOfEx.01094800
	$+3ED    0086C20A    50                        PUSH EAX
	$+3EE    0086C20B    E8 A0DEF3FF               CALL PathOfEx.007AA0B0
	$+3F3    0086C210    C645 FC 09                MOV BYTE PTR SS:[EBP-0x4],0x9
	$+3F7    0086C214    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+3FA    0086C217    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+3FC    0086C219    83FA 20                   CMP EDX,0x20
	$+3FF    0086C21C    72 0A                     JB SHORT PathOfEx.0086C228
	$+401    0086C21E    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+406    0086C223    E8 E5333100               CALL PathOfEx.00B7F60D
	$+40B    0086C228    8BCA                      MOV ECX,EDX
	$+40D    0086C22A    8BC7                      MOV EAX,EDI
	$+40F    0086C22C    83E1 1F                   AND ECX,0x1F
	$+412    0086C22F    C1EA 05                   SHR EDX,0x5
	$+415    0086C232    D3E0                      SHL EAX,CL
	$+417    0086C234    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+41A    0086C237    0F84 C2000000             JE PathOfEx.0086C2FF
	$+420    0086C23D    6A 1A                     PUSH 0x1A
	$+422    0086C23F    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+425    0086C242    B9 00480901               MOV ECX,PathOfEx.01094800
	$+42A    0086C247    50                        PUSH EAX
	$+42B    0086C248    E8 63DEF3FF               CALL PathOfEx.007AA0B0
	$+430    0086C24D    C645 FC 0A                MOV BYTE PTR SS:[EBP-0x4],0xA
	$+434    0086C251    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+437    0086C254    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+439    0086C256    83FA 20                   CMP EDX,0x20
	$+43C    0086C259    72 0A                     JB SHORT PathOfEx.0086C265
	$+43E    0086C25B    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+443    0086C260    E8 A8333100               CALL PathOfEx.00B7F60D
	$+448    0086C265    8BCA                      MOV ECX,EDX
	$+44A    0086C267    8BC7                      MOV EAX,EDI
	$+44C    0086C269    83E1 1F                   AND ECX,0x1F
	$+44F    0086C26C    C1EA 05                   SHR EDX,0x5
	$+452    0086C26F    D3E0                      SHL EAX,CL
	$+454    0086C271    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+457    0086C274    0F84 85000000             JE PathOfEx.0086C2FF
	$+45D    0086C27A    6A 00                     PUSH 0x0
	$+45F    0086C27C    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+462    0086C27F    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+467    0086C284    50                        PUSH EAX
	$+468    0086C285    E8 26DEF3FF               CALL PathOfEx.007AA0B0
	$+46D    0086C28A    C645 FC 0B                MOV BYTE PTR SS:[EBP-0x4],0xB
	$+471    0086C28E    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+474    0086C291    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+476    0086C293    83FA 20                   CMP EDX,0x20
	$+479    0086C296    72 0A                     JB SHORT PathOfEx.0086C2A2
	$+47B    0086C298    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+480    0086C29D    E8 6B333100               CALL PathOfEx.00B7F60D
	$+485    0086C2A2    8BCA                      MOV ECX,EDX
	$+487    0086C2A4    8BC7                      MOV EAX,EDI
	$+489    0086C2A6    83E1 1F                   AND ECX,0x1F
	$+48C    0086C2A9    C1EA 05                   SHR EDX,0x5
	$+48F    0086C2AC    D3E0                      SHL EAX,CL
	$+491    0086C2AE    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+494    0086C2B1    74 4C                     JE SHORT PathOfEx.0086C2FF
	$+496    0086C2B3    6A 13                     PUSH 0x13
	$+498    0086C2B5    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+49B    0086C2B8    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+4A0    0086C2BD    50                        PUSH EAX
	$+4A1    0086C2BE    E8 EDDDF3FF               CALL PathOfEx.007AA0B0
	$+4A6    0086C2C3    C645 FC 0C                MOV BYTE PTR SS:[EBP-0x4],0xC
	$+4AA    0086C2C7    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+4AD    0086C2CA    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+4AF    0086C2CC    83FA 20                   CMP EDX,0x20
	$+4B2    0086C2CF    72 0A                     JB SHORT PathOfEx.0086C2DB
	$+4B4    0086C2D1    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+4B9    0086C2D6    E8 32333100               CALL PathOfEx.00B7F60D
	$+4BE    0086C2DB    8BCA                      MOV ECX,EDX
	$+4C0    0086C2DD    C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+4C4    0086C2E1    83E1 1F                   AND ECX,0x1F
	$+4C7    0086C2E4    C1EA 05                   SHR EDX,0x5
	$+4CA    0086C2E7    8BC7                      MOV EAX,EDI
	$+4CC    0086C2E9    D3E0                      SHL EAX,CL
	$+4CE    0086C2EB    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+4D1    0086C2EE    74 0F                     JE SHORT PathOfEx.0086C2FF
	$+4D3    0086C2F0    C705 2C480901 03000000    MOV DWORD PTR DS:[0x109482C],0x3
	$+4DD    0086C2FA    E9 88000000               JMP PathOfEx.0086C387
	$+4E2    0086C2FF    6A 19                     PUSH 0x19
	$+4E4    0086C301    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+4E7    0086C304    B9 00480901               MOV ECX,PathOfEx.01094800
	$+4EC    0086C309    50                        PUSH EAX
	$+4ED    0086C30A    E8 A1DDF3FF               CALL PathOfEx.007AA0B0
	$+4F2    0086C30F    C645 FC 0D                MOV BYTE PTR SS:[EBP-0x4],0xD
	$+4F6    0086C313    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+4F9    0086C316    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+4FB    0086C318    83FA 20                   CMP EDX,0x20
	$+4FE    0086C31B    72 0A                     JB SHORT PathOfEx.0086C327
	$+500    0086C31D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+505    0086C322    E8 E6323100               CALL PathOfEx.00B7F60D
	$+50A    0086C327    8BCA                      MOV ECX,EDX
	$+50C    0086C329    C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+510    0086C32D    83E1 1F                   AND ECX,0x1F
	$+513    0086C330    C1EA 05                   SHR EDX,0x5
	$+516    0086C333    8BC7                      MOV EAX,EDI
	$+518    0086C335    D3E0                      SHL EAX,CL
	$+51A    0086C337    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+51D    0086C33A    74 41                     JE SHORT PathOfEx.0086C37D
	$+51F    0086C33C    6A 1A                     PUSH 0x1A
	$+521    0086C33E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+524    0086C341    B9 00480901               MOV ECX,PathOfEx.01094800
	$+529    0086C346    50                        PUSH EAX
	$+52A    0086C347    E8 64DDF3FF               CALL PathOfEx.007AA0B0
	$+52F    0086C34C    C645 FC 0E                MOV BYTE PTR SS:[EBP-0x4],0xE
	$+533    0086C350    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+536    0086C353    8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+538    0086C355    83FA 20                   CMP EDX,0x20
	$+53B    0086C358    72 0A                     JB SHORT PathOfEx.0086C364
	$+53D    0086C35A    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+542    0086C35F    E8 A9323100               CALL PathOfEx.00B7F60D
	$+547    0086C364    8BCA                      MOV ECX,EDX
	$+549    0086C366    C1EA 05                   SHR EDX,0x5
	$+54C    0086C369    83E1 1F                   AND ECX,0x1F
	$+54F    0086C36C    D3E7                      SHL EDI,CL
	$+551    0086C36E    853C90                    TEST DWORD PTR DS:[EAX+EDX*4],EDI
	$+554    0086C371    C705 2C480901 02000000    MOV DWORD PTR DS:[0x109482C],0x2
	$+55E    0086C37B    75 0A                     JNZ SHORT PathOfEx.0086C387
	$+560    0086C37D    C705 2C480901 01000000    MOV DWORD PTR DS:[0x109482C],0x1
	$+56A    0086C387    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+56D    0086C38A    B8 C0470901               MOV EAX,PathOfEx.010947C0
	$+572    0086C38F    5F                        POP EDI
	$+573    0086C390    5E                        POP ESI
	$+574    0086C391    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+57B    0086C398    5B                        POP EBX
	$+57C    0086C399    8BE5                      MOV ESP,EBP
	$+57E    0086C39B    5D                        POP EBP
	$+57F    0086C39C    C3                        RETN
	$+580    0086C39D    CC                        INT3
	$+581    0086C39E    CC                        INT3
	$+582    0086C39F    CC                        INT3
	$+583    0086C3A0    55                        PUSH EBP
	$+584    0086C3A1    8BEC                      MOV EBP,ESP
	$+586    0086C3A3    6A FF                     PUSH -0x1
	$+588    0086C3A5    68 B68FC300               PUSH PathOfEx.00C38FB6
	$+58D    0086C3AA    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+593    0086C3B0    50                        PUSH EAX
	$+594    0086C3B1    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+59B    0086C3B8    83EC 08                   SUB ESP,0x8
	$+59E    0086C3BB    56                        PUSH ESI
	$+59F    0086C3BC    6A 0C                     PUSH 0xC
	$+5A1    0086C3BE    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+5A4    0086C3C1    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+5A9    0086C3C6    50                        PUSH EAX
	$+5AA    0086C3C7    E8 E4DCF3FF               CALL PathOfEx.007AA0B0
	$+5AF    0086C3CC    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+5B6    0086C3D3    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+5B9    0086C3D6    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+5BB    0086C3D8    83FA 20                   CMP EDX,0x20
	$+5BE    0086C3DB    72 0A                     JB SHORT PathOfEx.0086C3E7
	$+5C0    0086C3DD    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+5C5    0086C3E2    E8 26323100               CALL PathOfEx.00B7F60D
	$+5CA    0086C3E7    8BCA                      MOV ECX,EDX
	$+5CC    0086C3E9    B8 01000000               MOV EAX,0x1
	$+5D1    0086C3EE    83E1 1F                   AND ECX,0x1F
	$+5D4    0086C3F1    C1EA 05                   SHR EDX,0x5
	$+5D7    0086C3F4    D3E0                      SHL EAX,CL
	$+5D9    0086C3F6    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+5DC    0086C3F9    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+5DF    0086C3FC    5E                        POP ESI
	$+5E0    0086C3FD    0F95C0                    SETNE AL
	$+5E3    0086C400    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+5EA    0086C407    8BE5                      MOV ESP,EBP
	$+5EC    0086C409    5D                        POP EBP
	$+5ED    0086C40A    C3                        RETN
	$+5EE    0086C40B    CC                        INT3
	$+5EF    0086C40C    CC                        INT3
	$+5F0    0086C40D    CC                        INT3
	$+5F1    0086C40E    CC                        INT3
	$+5F2    0086C40F    CC                        INT3
	$+5F3    0086C410    55                        PUSH EBP
	$+5F4    0086C411    8BEC                      MOV EBP,ESP
	$+5F6    0086C413    6A FF                     PUSH -0x1
	$+5F8    0086C415    68 B68FC300               PUSH PathOfEx.00C38FB6
	$+5FD    0086C41A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+603    0086C420    50                        PUSH EAX
	$+604    0086C421    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+60B    0086C428    83EC 08                   SUB ESP,0x8
	$+60E    0086C42B    56                        PUSH ESI
	$+60F    0086C42C    6A 1B                     PUSH 0x1B
	$+611    0086C42E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+614    0086C431    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+619    0086C436    50                        PUSH EAX
	$+61A    0086C437    E8 74DCF3FF               CALL PathOfEx.007AA0B0
	$+61F    0086C43C    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+626    0086C443    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+629    0086C446    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+62B    0086C448    83FA 20                   CMP EDX,0x20
	$+62E    0086C44B    72 0A                     JB SHORT PathOfEx.0086C457
	$+630    0086C44D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+635    0086C452    E8 B6313100               CALL PathOfEx.00B7F60D
	$+63A    0086C457    8BCA                      MOV ECX,EDX
	$+63C    0086C459    B8 01000000               MOV EAX,0x1
	$+641    0086C45E    83E1 1F                   AND ECX,0x1F
	$+644    0086C461    C1EA 05                   SHR EDX,0x5
	$+647    0086C464    D3E0                      SHL EAX,CL
	$+649    0086C466    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+64C    0086C469    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+64F    0086C46C    5E                        POP ESI
	$+650    0086C46D    0F95C0                    SETNE AL
	$+653    0086C470    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+65A    0086C477    8BE5                      MOV ESP,EBP
	$+65C    0086C479    5D                        POP EBP
	$+65D    0086C47A    C3                        RETN
	$+65E    0086C47B    CC                        INT3
	$+65F    0086C47C    CC                        INT3
	$+660    0086C47D    CC                        INT3
	$+661    0086C47E    CC                        INT3
	$+662    0086C47F    CC                        INT3
	$+663    0086C480    55                        PUSH EBP
	$+664    0086C481    8BEC                      MOV EBP,ESP
	$+666    0086C483    6A FF                     PUSH -0x1
	$+668    0086C485    68 B68FC300               PUSH PathOfEx.00C38FB6
	$+66D    0086C48A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+673    0086C490    50                        PUSH EAX
	$+674    0086C491    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+67B    0086C498    83EC 08                   SUB ESP,0x8
	$+67E    0086C49B    56                        PUSH ESI
	$+67F    0086C49C    6A 1D                     PUSH 0x1D
	$+681    0086C49E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+684    0086C4A1    B9 FC470901               MOV ECX,PathOfEx.010947FC
	$+689    0086C4A6    50                        PUSH EAX
	$+68A    0086C4A7    E8 04DCF3FF               CALL PathOfEx.007AA0B0
	$+68F    0086C4AC    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+696    0086C4B3    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+699    0086C4B6    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+69B    0086C4B8    83FA 20                   CMP EDX,0x20
	$+69E    0086C4BB    72 0A                     JB SHORT PathOfEx.0086C4C7
	$+6A0    0086C4BD    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+6A5    0086C4C2    E8 46313100               CALL PathOfEx.00B7F60D
	$+6AA    0086C4C7    8BCA                      MOV ECX,EDX
	$+6AC    0086C4C9    B8 01000000               MOV EAX,0x1
	$+6B1    0086C4CE    83E1 1F                   AND ECX,0x1F
	$+6B4    0086C4D1    C1EA 05                   SHR EDX,0x5
	$+6B7    0086C4D4    D3E0                      SHL EAX,CL
	$+6B9    0086C4D6    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+6BC    0086C4D9    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+6BF    0086C4DC    5E                        POP ESI
	$+6C0    0086C4DD    0F95C0                    SETNE AL
	$+6C3    0086C4E0    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+6CA    0086C4E7    8BE5                      MOV ESP,EBP
	$+6CC    0086C4E9    5D                        POP EBP
	$+6CD    0086C4EA    C3                        RETN
	$+6CE    0086C4EB    CC                        INT3
	$+6CF    0086C4EC    CC                        INT3
	$+6D0    0086C4ED    CC                        INT3
	$+6D1    0086C4EE    CC                        INT3
	$+6D2    0086C4EF    CC                        INT3
	$+6D3    0086C4F0    55                        PUSH EBP
	$+6D4    0086C4F1    8BEC                      MOV EBP,ESP
	$+6D6    0086C4F3    6A FF                     PUSH -0x1
	$+6D8    0086C4F5    68 B68FC300               PUSH PathOfEx.00C38FB6
	$+6DD    0086C4FA    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+6E3    0086C500    50                        PUSH EAX
	$+6E4    0086C501    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+6EB    0086C508    83EC 08                   SUB ESP,0x8
	$+6EE    0086C50B    56                        PUSH ESI
	$+6EF    0086C50C    6A 05                     PUSH 0x5
	$+6F1    0086C50E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+6F4    0086C511    B9 04480901               MOV ECX,PathOfEx.01094804
	$+6F9    0086C516    50                        PUSH EAX
	$+6FA    0086C517    E8 94DBF3FF               CALL PathOfEx.007AA0B0
	$+6FF    0086C51C    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+706    0086C523    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+709    0086C526    8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+70B    0086C528    83FA 20                   CMP EDX,0x20
	$+70E    0086C52B    72 0A                     JB SHORT PathOfEx.0086C537
	$+710    0086C52D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
	$+715    0086C532    E8 D6303100               CALL PathOfEx.00B7F60D
	$+71A    0086C537    8BCA                      MOV ECX,EDX
	$+71C    0086C539    B8 01000000               MOV EAX,0x1
	$+721    0086C53E    83E1 1F                   AND ECX,0x1F
	$+724    0086C541    C1EA 05                   SHR EDX,0x5
	$+727    0086C544    D3E0                      SHL EAX,CL
	$+729    0086C546    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+72C    0086C549    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+72F    0086C54C    5E                        POP ESI
	$+730    0086C54D    0F95C0                    SETNE AL
	$+733    0086C550    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+73A    0086C557    8BE5                      MOV ESP,EBP
	$+73C    0086C559    5D                        POP EBP
	$+73D    0086C55A    C3                        RETN
	$+73E    0086C55B    CC                        INT3
	*/
	//gj3
	/*
	$-769    00EE49E7     50                        PUSH EAX
	$-768    00EE49E8     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-766    00EE49EA     FF91 70010000             CALL DWORD PTR DS:[ECX+0x170]
	$-760    00EE49F0     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-75D    00EE49F3     8D95 E4FEFFFF             LEA EDX,DWORD PTR SS:[EBP-0x11C]
	$-757    00EE49F9     6A 02                     PUSH 0x2
	$-755    00EE49FB     52                        PUSH EDX
	$-754    00EE49FC     6A 00                     PUSH 0x0
	$-752    00EE49FE     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-750    00EE4A00     50                        PUSH EAX
	$-74F    00EE4A01     FF91 78010000             CALL DWORD PTR DS:[ECX+0x178]
	$-749    00EE4A07     8B56 28                   MOV EDX,DWORD PTR DS:[ESI+0x28]
	$-746    00EE4A0A     85D2                      TEST EDX,EDX
	$-744    00EE4A0C     74 05                     JE SHORT PathOfEx.00EE4A13
	$-742    00EE4A0E     8B52 04                   MOV EDX,DWORD PTR DS:[EDX+0x4]
	$-73F    00EE4A11     EB 02                     JMP SHORT PathOfEx.00EE4A15
	$-73D    00EE4A13     33D2                      XOR EDX,EDX
	$-73B    00EE4A15     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-738    00EE4A18     6A 08                     PUSH 0x8
	$-736    00EE4A1A     6A 00                     PUSH 0x0
	$-734    00EE4A1C     52                        PUSH EDX
	$-733    00EE4A1D     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-731    00EE4A1F     6A 00                     PUSH 0x0
	$-72F    00EE4A21     50                        PUSH EAX
	$-72E    00EE4A22     FF91 90010000             CALL DWORD PTR DS:[ECX+0x190]
	$-728    00EE4A28     33C0                      XOR EAX,EAX
	$-726    00EE4A2A     3947 34                   CMP DWORD PTR DS:[EDI+0x34],EAX
	$-723    00EE4A2D     0F95C0                    SETNE AL
	$-720    00EE4A30     8D0485 1C000000           LEA EAX,DWORD PTR DS:[EAX*4+0x1C]
	$-719    00EE4A37     8B0430                    MOV EAX,DWORD PTR DS:[EAX+ESI]
	$-716    00EE4A3A     8B50 60                   MOV EDX,DWORD PTR DS:[EAX+0x60]
	$-713    00EE4A3D     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-710    00EE4A40     52                        PUSH EDX
	$-70F    00EE4A41     50                        PUSH EAX
	$-70E    00EE4A42     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-70C    00EE4A44     FF91 AC010000             CALL DWORD PTR DS:[ECX+0x1AC]
	$-706    00EE4A4A     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-703    00EE4A4D     8D95 04FFFFFF             LEA EDX,DWORD PTR SS:[EBP-0xFC]
	$-6FD    00EE4A53     6A 01                     PUSH 0x1
	$-6FB    00EE4A55     52                        PUSH EDX
	$-6FA    00EE4A56     6A 00                     PUSH 0x0
	$-6F8    00EE4A58     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-6F6    00EE4A5A     50                        PUSH EAX
	$-6F5    00EE4A5B     FF91 B4010000             CALL DWORD PTR DS:[ECX+0x1B4]
	$-6EF    00EE4A61     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-6EC    00EE4A64     8B57 5C                   MOV EDX,DWORD PTR DS:[EDI+0x5C]
	$-6E9    00EE4A67     6A 10                     PUSH 0x10
	$-6E7    00EE4A69     83C2 60                   ADD EDX,0x60
	$-6E4    00EE4A6C     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-6E2    00EE4A6E     52                        PUSH EDX
	$-6E1    00EE4A6F     6A 01                     PUSH 0x1
	$-6DF    00EE4A71     50                        PUSH EAX
	$-6DE    00EE4A72     FF91 B4010000             CALL DWORD PTR DS:[ECX+0x1B4]
	$-6D8    00EE4A78     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-6D5    00EE4A7B     6A 02                     PUSH 0x2
	$-6D3    00EE4A7D     6A 00                     PUSH 0x0
	$-6D1    00EE4A7F     6A 05                     PUSH 0x5
	$-6CF    00EE4A81     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-6CD    00EE4A83     50                        PUSH EAX
	$-6CC    00EE4A84     FF91 44010000             CALL DWORD PTR DS:[ECX+0x144]
	$-6C6    00EE4A8A     8B75 D0                   MOV ESI,DWORD PTR SS:[EBP-0x30]
	$-6C3    00EE4A8D     85F6                      TEST ESI,ESI
	$-6C1    00EE4A8F     74 3C                     JE SHORT PathOfEx.00EE4ACD
	$-6BF    00EE4A91     837E 04 00                CMP DWORD PTR DS:[ESI+0x4],0x0
	$-6BB    00EE4A95     74 0E                     JE SHORT PathOfEx.00EE4AA5
	$-6B9    00EE4A97     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-6B6    00EE4A9A     50                        PUSH EAX
	$-6B5    00EE4A9B     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-6B3    00EE4A9D     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-6B0    00EE4AA0     8B76 04                   MOV ESI,DWORD PTR DS:[ESI+0x4]
	$-6AD    00EE4AA3     EB 2A                     JMP SHORT PathOfEx.00EE4ACF
	$-6AB    00EE4AA5     837E 0C 00                CMP DWORD PTR DS:[ESI+0xC],0x0
	$-6A7    00EE4AA9     74 0E                     JE SHORT PathOfEx.00EE4AB9
	$-6A5    00EE4AAB     8B46 0C                   MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-6A2    00EE4AAE     50                        PUSH EAX
	$-6A1    00EE4AAF     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-69F    00EE4AB1     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-69C    00EE4AB4     8B76 0C                   MOV ESI,DWORD PTR DS:[ESI+0xC]
	$-699    00EE4AB7     EB 16                     JMP SHORT PathOfEx.00EE4ACF
	$-697    00EE4AB9     837E 14 00                CMP DWORD PTR DS:[ESI+0x14],0x0
	$-693    00EE4ABD     74 0E                     JE SHORT PathOfEx.00EE4ACD
	$-691    00EE4ABF     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-68E    00EE4AC2     50                        PUSH EAX
	$-68D    00EE4AC3     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-68B    00EE4AC5     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-688    00EE4AC8     8B76 14                   MOV ESI,DWORD PTR DS:[ESI+0x14]
	$-685    00EE4ACB     EB 02                     JMP SHORT PathOfEx.00EE4ACF
	$-683    00EE4ACD     33F6                      XOR ESI,ESI
	$-681    00EE4ACF     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-67E    00EE4AD2     56                        PUSH ESI
	$-67D    00EE4AD3     6A 00                     PUSH 0x0
	$-67B    00EE4AD5     50                        PUSH EAX
	$-67A    00EE4AD6     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-678    00EE4AD8     FF91 04010000             CALL DWORD PTR DS:[ECX+0x104]
	$-672    00EE4ADE     85F6                      TEST ESI,ESI
	$-670    00EE4AE0     74 06                     JE SHORT PathOfEx.00EE4AE8
	$-66E    00EE4AE2     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-66C    00EE4AE4     56                        PUSH ESI
	$-66B    00EE4AE5     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$-668    00EE4AE8     8B75 D8                   MOV ESI,DWORD PTR SS:[EBP-0x28]
	$-665    00EE4AEB     85F6                      TEST ESI,ESI
	$-663    00EE4AED     74 3C                     JE SHORT PathOfEx.00EE4B2B
	$-661    00EE4AEF     837E 04 00                CMP DWORD PTR DS:[ESI+0x4],0x0
	$-65D    00EE4AF3     74 0E                     JE SHORT PathOfEx.00EE4B03
	$-65B    00EE4AF5     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-658    00EE4AF8     50                        PUSH EAX
	$-657    00EE4AF9     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-655    00EE4AFB     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-652    00EE4AFE     8B76 04                   MOV ESI,DWORD PTR DS:[ESI+0x4]
	$-64F    00EE4B01     EB 2A                     JMP SHORT PathOfEx.00EE4B2D
	$-64D    00EE4B03     837E 0C 00                CMP DWORD PTR DS:[ESI+0xC],0x0
	$-649    00EE4B07     74 0E                     JE SHORT PathOfEx.00EE4B17
	$-647    00EE4B09     8B46 0C                   MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-644    00EE4B0C     50                        PUSH EAX
	$-643    00EE4B0D     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-641    00EE4B0F     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-63E    00EE4B12     8B76 0C                   MOV ESI,DWORD PTR DS:[ESI+0xC]
	$-63B    00EE4B15     EB 16                     JMP SHORT PathOfEx.00EE4B2D
	$-639    00EE4B17     837E 14 00                CMP DWORD PTR DS:[ESI+0x14],0x0
	$-635    00EE4B1B     74 0E                     JE SHORT PathOfEx.00EE4B2B
	$-633    00EE4B1D     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-630    00EE4B20     50                        PUSH EAX
	$-62F    00EE4B21     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-62D    00EE4B23     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-62A    00EE4B26     8B76 14                   MOV ESI,DWORD PTR DS:[ESI+0x14]
	$-627    00EE4B29     EB 02                     JMP SHORT PathOfEx.00EE4B2D
	$-625    00EE4B2B     33F6                      XOR ESI,ESI
	$-623    00EE4B2D     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-620    00EE4B30     56                        PUSH ESI
	$-61F    00EE4B31     6A 01                     PUSH 0x1
	$-61D    00EE4B33     50                        PUSH EAX
	$-61C    00EE4B34     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-61A    00EE4B36     FF91 04010000             CALL DWORD PTR DS:[ECX+0x104]
	$-614    00EE4B3C     85F6                      TEST ESI,ESI
	$-612    00EE4B3E     74 06                     JE SHORT PathOfEx.00EE4B46
	$-610    00EE4B40     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-60E    00EE4B42     56                        PUSH ESI
	$-60D    00EE4B43     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$-60A    00EE4B46     8B75 E0                   MOV ESI,DWORD PTR SS:[EBP-0x20]
	$-607    00EE4B49     85F6                      TEST ESI,ESI
	$-605    00EE4B4B     74 3C                     JE SHORT PathOfEx.00EE4B89
	$-603    00EE4B4D     837E 04 00                CMP DWORD PTR DS:[ESI+0x4],0x0
	$-5FF    00EE4B51     74 0E                     JE SHORT PathOfEx.00EE4B61
	$-5FD    00EE4B53     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-5FA    00EE4B56     50                        PUSH EAX
	$-5F9    00EE4B57     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-5F7    00EE4B59     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-5F4    00EE4B5C     8B76 04                   MOV ESI,DWORD PTR DS:[ESI+0x4]
	$-5F1    00EE4B5F     EB 2A                     JMP SHORT PathOfEx.00EE4B8B
	$-5EF    00EE4B61     837E 0C 00                CMP DWORD PTR DS:[ESI+0xC],0x0
	$-5EB    00EE4B65     74 0E                     JE SHORT PathOfEx.00EE4B75
	$-5E9    00EE4B67     8B46 0C                   MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-5E6    00EE4B6A     50                        PUSH EAX
	$-5E5    00EE4B6B     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-5E3    00EE4B6D     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-5E0    00EE4B70     8B76 0C                   MOV ESI,DWORD PTR DS:[ESI+0xC]
	$-5DD    00EE4B73     EB 16                     JMP SHORT PathOfEx.00EE4B8B
	$-5DB    00EE4B75     837E 14 00                CMP DWORD PTR DS:[ESI+0x14],0x0
	$-5D7    00EE4B79     74 0E                     JE SHORT PathOfEx.00EE4B89
	$-5D5    00EE4B7B     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-5D2    00EE4B7E     50                        PUSH EAX
	$-5D1    00EE4B7F     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-5CF    00EE4B81     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-5CC    00EE4B84     8B76 14                   MOV ESI,DWORD PTR DS:[ESI+0x14]
	$-5C9    00EE4B87     EB 02                     JMP SHORT PathOfEx.00EE4B8B
	$-5C7    00EE4B89     33F6                      XOR ESI,ESI
	$-5C5    00EE4B8B     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-5C2    00EE4B8E     56                        PUSH ESI
	$-5C1    00EE4B8F     6A 02                     PUSH 0x2
	$-5BF    00EE4B91     50                        PUSH EAX
	$-5BE    00EE4B92     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-5BC    00EE4B94     FF91 04010000             CALL DWORD PTR DS:[ECX+0x104]
	$-5B6    00EE4B9A     85F6                      TEST ESI,ESI
	$-5B4    00EE4B9C     74 06                     JE SHORT PathOfEx.00EE4BA4
	$-5B2    00EE4B9E     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-5B0    00EE4BA0     56                        PUSH ESI
	$-5AF    00EE4BA1     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$-5AC    00EE4BA4     8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
	$-5A9    00EE4BA7     85F6                      TEST ESI,ESI
	$-5A7    00EE4BA9     74 3C                     JE SHORT PathOfEx.00EE4BE7
	$-5A5    00EE4BAB     837E 04 00                CMP DWORD PTR DS:[ESI+0x4],0x0
	$-5A1    00EE4BAF     74 0E                     JE SHORT PathOfEx.00EE4BBF
	$-59F    00EE4BB1     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-59C    00EE4BB4     50                        PUSH EAX
	$-59B    00EE4BB5     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-599    00EE4BB7     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-596    00EE4BBA     8B76 04                   MOV ESI,DWORD PTR DS:[ESI+0x4]
	$-593    00EE4BBD     EB 2A                     JMP SHORT PathOfEx.00EE4BE9
	$-591    00EE4BBF     837E 0C 00                CMP DWORD PTR DS:[ESI+0xC],0x0
	$-58D    00EE4BC3     74 0E                     JE SHORT PathOfEx.00EE4BD3
	$-58B    00EE4BC5     8B46 0C                   MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-588    00EE4BC8     50                        PUSH EAX
	$-587    00EE4BC9     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-585    00EE4BCB     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-582    00EE4BCE     8B76 0C                   MOV ESI,DWORD PTR DS:[ESI+0xC]
	$-57F    00EE4BD1     EB 16                     JMP SHORT PathOfEx.00EE4BE9
	$-57D    00EE4BD3     837E 14 00                CMP DWORD PTR DS:[ESI+0x14],0x0
	$-579    00EE4BD7     74 0E                     JE SHORT PathOfEx.00EE4BE7
	$-577    00EE4BD9     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-574    00EE4BDC     50                        PUSH EAX
	$-573    00EE4BDD     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-571    00EE4BDF     FF51 04                   CALL DWORD PTR DS:[ECX+0x4]
	$-56E    00EE4BE2     8B76 14                   MOV ESI,DWORD PTR DS:[ESI+0x14]
	$-56B    00EE4BE5     EB 02                     JMP SHORT PathOfEx.00EE4BE9
	$-569    00EE4BE7     33F6                      XOR ESI,ESI
	$-567    00EE4BE9     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-564    00EE4BEC     56                        PUSH ESI
	$-563    00EE4BED     6A 03                     PUSH 0x3
	$-561    00EE4BEF     50                        PUSH EAX
	$-560    00EE4BF0     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-55E    00EE4BF2     FF91 04010000             CALL DWORD PTR DS:[ECX+0x104]
	$-558    00EE4BF8     85F6                      TEST ESI,ESI
	$-556    00EE4BFA     74 06                     JE SHORT PathOfEx.00EE4C02
	$-554    00EE4BFC     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-552    00EE4BFE     56                        PUSH ESI
	$-551    00EE4BFF     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$-54E    00EE4C02     8B4B 04                   MOV ECX,DWORD PTR DS:[EBX+0x4]
	$-54B    00EE4C05     8B7D CC                   MOV EDI,DWORD PTR SS:[EBP-0x34]
	$-548    00EE4C08     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-546    00EE4C0A     FF77 04                   PUSH DWORD PTR DS:[EDI+0x4]
	$-543    00EE4C0D     51                        PUSH ECX
	$-542    00EE4C0E     FF90 5C010000             CALL DWORD PTR DS:[EAX+0x15C]
	$-53C    00EE4C14     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-539    00EE4C17     8B5D C8                   MOV EBX,DWORD PTR SS:[EBP-0x38]
	$-536    00EE4C1A     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-533    00EE4C1D     FF73 5C                   PUSH DWORD PTR DS:[EBX+0x5C]
	$-530    00EE4C20     50                        PUSH EAX
	$-52F    00EE4C21     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-52D    00EE4C23     FF91 70010000             CALL DWORD PTR DS:[ECX+0x170]
	$-527    00EE4C29     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-524    00EE4C2C     8D95 C8FDFFFF             LEA EDX,DWORD PTR SS:[EBP-0x238]
	$-51E    00EE4C32     6A 02                     PUSH 0x2
	$-51C    00EE4C34     52                        PUSH EDX
	$-51B    00EE4C35     6A 00                     PUSH 0x0
	$-519    00EE4C37     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-517    00EE4C39     50                        PUSH EAX
	$-516    00EE4C3A     FF91 78010000             CALL DWORD PTR DS:[ECX+0x178]
	$-510    00EE4C40     8B75 C4                   MOV ESI,DWORD PTR SS:[EBP-0x3C]
	$-50D    00EE4C43     85F6                      TEST ESI,ESI
	$-50B    00EE4C45     74 05                     JE SHORT PathOfEx.00EE4C4C
	$-509    00EE4C47     8B56 04                   MOV EDX,DWORD PTR DS:[ESI+0x4]
	$-506    00EE4C4A     EB 02                     JMP SHORT PathOfEx.00EE4C4E
	$-504    00EE4C4C     33D2                      XOR EDX,EDX
	$-502    00EE4C4E     FF75 C0                   PUSH DWORD PTR SS:[EBP-0x40]
	$-4FF    00EE4C51     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-4FC    00EE4C54     FF75 BC                   PUSH DWORD PTR SS:[EBP-0x44]
	$-4F9    00EE4C57     52                        PUSH EDX
	$-4F8    00EE4C58     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-4F5    00EE4C5B     6A 00                     PUSH 0x0
	$-4F3    00EE4C5D     50                        PUSH EAX
	$-4F2    00EE4C5E     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-4F0    00EE4C60     FF91 90010000             CALL DWORD PTR DS:[ECX+0x190]
	$-4EA    00EE4C66     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-4E7    00EE4C69     8B50 60                   MOV EDX,DWORD PTR DS:[EAX+0x60]
	$-4E4    00EE4C6C     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-4E1    00EE4C6F     52                        PUSH EDX
	$-4E0    00EE4C70     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-4DD    00EE4C73     50                        PUSH EAX
	$-4DC    00EE4C74     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-4DA    00EE4C76     FF91 AC010000             CALL DWORD PTR DS:[ECX+0x1AC]
	$-4D4    00EE4C7C     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-4D1    00EE4C7F     8D95 C8FBFFFF             LEA EDX,DWORD PTR SS:[EBP-0x438]
	$-4CB    00EE4C85     6A 01                     PUSH 0x1
	$-4C9    00EE4C87     52                        PUSH EDX
	$-4C8    00EE4C88     6A 00                     PUSH 0x0
	$-4C6    00EE4C8A     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-4C3    00EE4C8D     50                        PUSH EAX
	$-4C2    00EE4C8E     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-4C0    00EE4C90     FF91 B4010000             CALL DWORD PTR DS:[ECX+0x1B4]
	$-4BA    00EE4C96     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-4B7    00EE4C99     8D95 C8FCFFFF             LEA EDX,DWORD PTR SS:[EBP-0x338]
	$-4B1    00EE4C9F     6A 02                     PUSH 0x2
	$-4AF    00EE4CA1     52                        PUSH EDX
	$-4AE    00EE4CA2     6A 01                     PUSH 0x1
	$-4AC    00EE4CA4     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-4A9    00EE4CA7     50                        PUSH EAX
	$-4A8    00EE4CA8     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-4A6    00EE4CAA     FF91 B4010000             CALL DWORD PTR DS:[ECX+0x1B4]
	$-4A0    00EE4CB0     8B4D D4                   MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$-49D    00EE4CB3     894D 0C                   MOV DWORD PTR SS:[EBP+0xC],ECX
	$-49A    00EE4CB6     C745 D4 00000000          MOV DWORD PTR SS:[EBP-0x2C],0x0
	$-493    00EE4CBD     C745 D0 00000000          MOV DWORD PTR SS:[EBP-0x30],0x0
	$-48C    00EE4CC4     83C8 FF                   OR EAX,0xFFFFFFFF
	$-489    00EE4CC7     C645 FC 13                MOV BYTE PTR SS:[EBP-0x4],0x13
	$-485    00EE4CCB     85C9                      TEST ECX,ECX
	$-483    00EE4CCD     74 20                     JE SHORT PathOfEx.00EE4CEF
	$-481    00EE4CCF     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$-47C    00EE4CD4     75 16                     JNZ SHORT PathOfEx.00EE4CEC
	$-47A    00EE4CD6     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-478    00EE4CD8     FF10                      CALL DWORD PTR DS:[EAX]
	$-476    00EE4CDA     8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-473    00EE4CDD     83C8 FF                   OR EAX,0xFFFFFFFF
	$-470    00EE4CE0     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-46B    00EE4CE5     75 05                     JNZ SHORT PathOfEx.00EE4CEC
	$-469    00EE4CE7     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-467    00EE4CE9     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-464    00EE4CEC     83C8 FF                   OR EAX,0xFFFFFFFF
	$-461    00EE4CEF     8B4D DC                   MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-45E    00EE4CF2     894D 0C                   MOV DWORD PTR SS:[EBP+0xC],ECX
	$-45B    00EE4CF5     C745 DC 00000000          MOV DWORD PTR SS:[EBP-0x24],0x0
	$-454    00EE4CFC     C745 D8 00000000          MOV DWORD PTR SS:[EBP-0x28],0x0
	$-44D    00EE4D03     C645 FC 14                MOV BYTE PTR SS:[EBP-0x4],0x14
	$-449    00EE4D07     85C9                      TEST ECX,ECX
	$-447    00EE4D09     74 1D                     JE SHORT PathOfEx.00EE4D28
	$-445    00EE4D0B     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$-440    00EE4D10     75 16                     JNZ SHORT PathOfEx.00EE4D28
	$-43E    00EE4D12     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-43C    00EE4D14     FF10                      CALL DWORD PTR DS:[EAX]
	$-43A    00EE4D16     8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-437    00EE4D19     83C8 FF                   OR EAX,0xFFFFFFFF
	$-434    00EE4D1C     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-42F    00EE4D21     75 05                     JNZ SHORT PathOfEx.00EE4D28
	$-42D    00EE4D23     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-42B    00EE4D25     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-428    00EE4D28     8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$-425    00EE4D2B     894D 0C                   MOV DWORD PTR SS:[EBP+0xC],ECX
	$-422    00EE4D2E     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-41B    00EE4D35     C745 E0 00000000          MOV DWORD PTR SS:[EBP-0x20],0x0
	$-414    00EE4D3C     C645 FC 15                MOV BYTE PTR SS:[EBP-0x4],0x15
	$-410    00EE4D40     85C9                      TEST ECX,ECX
	$-40E    00EE4D42     74 20                     JE SHORT PathOfEx.00EE4D64
	$-40C    00EE4D44     83C8 FF                   OR EAX,0xFFFFFFFF
	$-409    00EE4D47     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$-404    00EE4D4C     75 16                     JNZ SHORT PathOfEx.00EE4D64
	$-402    00EE4D4E     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-400    00EE4D50     FF10                      CALL DWORD PTR DS:[EAX]
	$-3FE    00EE4D52     8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-3FB    00EE4D55     83C8 FF                   OR EAX,0xFFFFFFFF
	$-3F8    00EE4D58     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-3F3    00EE4D5D     75 05                     JNZ SHORT PathOfEx.00EE4D64
	$-3F1    00EE4D5F     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-3EF    00EE4D61     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-3EC    00EE4D64     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-3E9    00EE4D67     894D 0C                   MOV DWORD PTR SS:[EBP+0xC],ECX
	$-3E6    00EE4D6A     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$-3DF    00EE4D71     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18],0x0
	$-3D8    00EE4D78     C645 FC 16                MOV BYTE PTR SS:[EBP-0x4],0x16
	$-3D4    00EE4D7C     85C9                      TEST ECX,ECX
	$-3D2    00EE4D7E     74 21                     JE SHORT PathOfEx.00EE4DA1
	$-3D0    00EE4D80     83C8 FF                   OR EAX,0xFFFFFFFF
	$-3CD    00EE4D83     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4],EAX
	$-3C8    00EE4D88     75 17                     JNZ SHORT PathOfEx.00EE4DA1
	$-3C6    00EE4D8A     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-3C4    00EE4D8C     FF10                      CALL DWORD PTR DS:[EAX]
	$-3C2    00EE4D8E     8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-3BF    00EE4D91     83C8 FF                   OR EAX,0xFFFFFFFF
	$-3BC    00EE4D94     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$-3B7    00EE4D99     48                        DEC EAX
	$-3B6    00EE4D9A     75 05                     JNZ SHORT PathOfEx.00EE4DA1
	$-3B4    00EE4D9C     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-3B2    00EE4D9E     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-3AF    00EE4DA1     85F6                      TEST ESI,ESI
	$-3AD    00EE4DA3     74 0C                     JE SHORT PathOfEx.00EE4DB1
	$-3AB    00EE4DA5     C645 FC 17                MOV BYTE PTR SS:[EBP-0x4],0x17
	$-3A7    00EE4DA9     8BCE                      MOV ECX,ESI
	$-3A5    00EE4DAB     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-3A3    00EE4DAD     6A 01                     PUSH 0x1
	$-3A1    00EE4DAF     FF10                      CALL DWORD PTR DS:[EAX]
	$-39F    00EE4DB1     85DB                      TEST EBX,EBX
	$-39D    00EE4DB3     74 0C                     JE SHORT PathOfEx.00EE4DC1
	$-39B    00EE4DB5     C645 FC 18                MOV BYTE PTR SS:[EBP-0x4],0x18
	$-397    00EE4DB9     8BCB                      MOV ECX,EBX
	$-395    00EE4DBB     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$-393    00EE4DBD     6A 01                     PUSH 0x1
	$-391    00EE4DBF     FF10                      CALL DWORD PTR DS:[EAX]
	$-38F    00EE4DC1     85FF                      TEST EDI,EDI
	$-38D    00EE4DC3     74 0C                     JE SHORT PathOfEx.00EE4DD1
	$-38B    00EE4DC5     C645 FC 19                MOV BYTE PTR SS:[EBP-0x4],0x19
	$-387    00EE4DC9     8BCF                      MOV ECX,EDI
	$-385    00EE4DCB     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$-383    00EE4DCD     6A 01                     PUSH 0x1
	$-381    00EE4DCF     FF10                      CALL DWORD PTR DS:[EAX]
	$-37F    00EE4DD1     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-37C    00EE4DD4     85C0                      TEST EAX,EAX
	$-37A    00EE4DD6     74 0C                     JE SHORT PathOfEx.00EE4DE4
	$-378    00EE4DD8     C645 FC 1A                MOV BYTE PTR SS:[EBP-0x4],0x1A
	$-374    00EE4DDC     8BC8                      MOV ECX,EAX
	$-372    00EE4DDE     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-370    00EE4DE0     6A 01                     PUSH 0x1
	$-36E    00EE4DE2     FF12                      CALL DWORD PTR DS:[EDX]
	$-36C    00EE4DE4     68 103B9400               PUSH PathOfEx.00943B10
	$-367    00EE4DE9     6A 04                     PUSH 0x4
	$-365    00EE4DEB     6A 08                     PUSH 0x8
	$-363    00EE4DED     8D45 D0                   LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-360    00EE4DF0     C645 FC 0D                MOV BYTE PTR SS:[EBP-0x4],0xD
	$-35C    00EE4DF4     50                        PUSH EAX
	$-35B    00EE4DF5     E8 EA6B4100               CALL PathOfEx.012FB9E4
	$-356    00EE4DFA     C645 FC 1B                MOV BYTE PTR SS:[EBP-0x4],0x1B
	$-352    00EE4DFE     8B45 B0                   MOV EAX,DWORD PTR SS:[EBP-0x50]
	$-34F    00EE4E01     FF75 B8                   PUSH DWORD PTR SS:[EBP-0x48]
	$-34C    00EE4E04     FF75 B4                   PUSH DWORD PTR SS:[EBP-0x4C]
	$-349    00EE4E07     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-346    00EE4E0A     50                        PUSH EAX
	$-345    00EE4E0B     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-343    00EE4E0D     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-33D    00EE4E13     C645 FC 1C                MOV BYTE PTR SS:[EBP-0x4],0x1C
	$-339    00EE4E17     8B45 A4                   MOV EAX,DWORD PTR SS:[EBP-0x5C]
	$-336    00EE4E1A     FF75 AC                   PUSH DWORD PTR SS:[EBP-0x54]
	$-333    00EE4E1D     FF75 A8                   PUSH DWORD PTR SS:[EBP-0x58]
	$-330    00EE4E20     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-32D    00EE4E23     50                        PUSH EAX
	$-32C    00EE4E24     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-32A    00EE4E26     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-324    00EE4E2C     C645 FC 1D                MOV BYTE PTR SS:[EBP-0x4],0x1D
	$-320    00EE4E30     8B45 98                   MOV EAX,DWORD PTR SS:[EBP-0x68]
	$-31D    00EE4E33     FF75 A0                   PUSH DWORD PTR SS:[EBP-0x60]
	$-31A    00EE4E36     FF75 9C                   PUSH DWORD PTR SS:[EBP-0x64]
	$-317    00EE4E39     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-314    00EE4E3C     50                        PUSH EAX
	$-313    00EE4E3D     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-311    00EE4E3F     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-30B    00EE4E45     C645 FC 1E                MOV BYTE PTR SS:[EBP-0x4],0x1E
	$-307    00EE4E49     8B45 8C                   MOV EAX,DWORD PTR SS:[EBP-0x74]
	$-304    00EE4E4C     FF75 94                   PUSH DWORD PTR SS:[EBP-0x6C]
	$-301    00EE4E4F     FF75 90                   PUSH DWORD PTR SS:[EBP-0x70]
	$-2FE    00EE4E52     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-2FB    00EE4E55     50                        PUSH EAX
	$-2FA    00EE4E56     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-2F8    00EE4E58     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-2F2    00EE4E5E     C645 FC 1F                MOV BYTE PTR SS:[EBP-0x4],0x1F
	$-2EE    00EE4E62     8B45 80                   MOV EAX,DWORD PTR SS:[EBP-0x80]
	$-2EB    00EE4E65     FF75 88                   PUSH DWORD PTR SS:[EBP-0x78]
	$-2E8    00EE4E68     FF75 84                   PUSH DWORD PTR SS:[EBP-0x7C]
	$-2E5    00EE4E6B     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-2E2    00EE4E6E     50                        PUSH EAX
	$-2E1    00EE4E6F     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-2DF    00EE4E71     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-2D9    00EE4E77     C645 FC 20                MOV BYTE PTR SS:[EBP-0x4],0x20
	$-2D5    00EE4E7B     8B85 74FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0x8C]
	$-2CF    00EE4E81     FFB5 7CFFFFFF             PUSH DWORD PTR SS:[EBP-0x84]
	$-2C9    00EE4E87     FFB5 78FFFFFF             PUSH DWORD PTR SS:[EBP-0x88]
	$-2C3    00EE4E8D     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-2C0    00EE4E90     50                        PUSH EAX
	$-2BF    00EE4E91     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-2BD    00EE4E93     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-2B7    00EE4E99     C645 FC 21                MOV BYTE PTR SS:[EBP-0x4],0x21
	$-2B3    00EE4E9D     8B85 68FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0x98]
	$-2AD    00EE4EA3     FFB5 70FFFFFF             PUSH DWORD PTR SS:[EBP-0x90]
	$-2A7    00EE4EA9     FFB5 6CFFFFFF             PUSH DWORD PTR SS:[EBP-0x94]
	$-2A1    00EE4EAF     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-29E    00EE4EB2     50                        PUSH EAX
	$-29D    00EE4EB3     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-29B    00EE4EB5     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-295    00EE4EBB     C645 FC 22                MOV BYTE PTR SS:[EBP-0x4],0x22
	$-291    00EE4EBF     8B85 5CFFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xA4]
	$-28B    00EE4EC5     FFB5 64FFFFFF             PUSH DWORD PTR SS:[EBP-0x9C]
	$-285    00EE4ECB     FFB5 60FFFFFF             PUSH DWORD PTR SS:[EBP-0xA0]
	$-27F    00EE4ED1     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-27C    00EE4ED4     50                        PUSH EAX
	$-27B    00EE4ED5     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-279    00EE4ED7     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-273    00EE4EDD     C645 FC 23                MOV BYTE PTR SS:[EBP-0x4],0x23
	$-26F    00EE4EE1     8B85 50FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xB0]
	$-269    00EE4EE7     FFB5 58FFFFFF             PUSH DWORD PTR SS:[EBP-0xA8]
	$-263    00EE4EED     FFB5 54FFFFFF             PUSH DWORD PTR SS:[EBP-0xAC]
	$-25D    00EE4EF3     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-25A    00EE4EF6     50                        PUSH EAX
	$-259    00EE4EF7     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-257    00EE4EF9     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-251    00EE4EFF     C645 FC 24                MOV BYTE PTR SS:[EBP-0x4],0x24
	$-24D    00EE4F03     8B85 44FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xBC]
	$-247    00EE4F09     FFB5 4CFFFFFF             PUSH DWORD PTR SS:[EBP-0xB4]
	$-241    00EE4F0F     FFB5 48FFFFFF             PUSH DWORD PTR SS:[EBP-0xB8]
	$-23B    00EE4F15     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-238    00EE4F18     50                        PUSH EAX
	$-237    00EE4F19     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-235    00EE4F1B     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-22F    00EE4F21     C645 FC 25                MOV BYTE PTR SS:[EBP-0x4],0x25
	$-22B    00EE4F25     8B85 38FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xC8]
	$-225    00EE4F2B     FFB5 40FFFFFF             PUSH DWORD PTR SS:[EBP-0xC0]
	$-21F    00EE4F31     FFB5 3CFFFFFF             PUSH DWORD PTR SS:[EBP-0xC4]
	$-219    00EE4F37     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-216    00EE4F3A     50                        PUSH EAX
	$-215    00EE4F3B     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-213    00EE4F3D     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-20D    00EE4F43     C645 FC 26                MOV BYTE PTR SS:[EBP-0x4],0x26
	$-209    00EE4F47     8B85 2CFFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xD4]
	$-203    00EE4F4D     FFB5 34FFFFFF             PUSH DWORD PTR SS:[EBP-0xCC]
	$-1FD    00EE4F53     FFB5 30FFFFFF             PUSH DWORD PTR SS:[EBP-0xD0]
	$-1F7    00EE4F59     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-1F4    00EE4F5C     50                        PUSH EAX
	$-1F3    00EE4F5D     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-1F1    00EE4F5F     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-1EB    00EE4F65     C645 FC 27                MOV BYTE PTR SS:[EBP-0x4],0x27
	$-1E7    00EE4F69     8B85 20FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xE0]
	$-1E1    00EE4F6F     FFB5 28FFFFFF             PUSH DWORD PTR SS:[EBP-0xD8]
	$-1DB    00EE4F75     FFB5 24FFFFFF             PUSH DWORD PTR SS:[EBP-0xDC]
	$-1D5    00EE4F7B     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-1D2    00EE4F7E     50                        PUSH EAX
	$-1D1    00EE4F7F     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-1CF    00EE4F81     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-1C9    00EE4F87     C745 FC 28000000          MOV DWORD PTR SS:[EBP-0x4],0x28
	$-1C2    00EE4F8E     8B85 14FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xEC]
	$-1BC    00EE4F94     FFB5 1CFFFFFF             PUSH DWORD PTR SS:[EBP-0xE4]
	$-1B6    00EE4F9A     FFB5 18FFFFFF             PUSH DWORD PTR SS:[EBP-0xE8]
	$-1B0    00EE4FA0     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-1AD    00EE4FA3     50                        PUSH EAX
	$-1AC    00EE4FA4     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-1AA    00EE4FA6     FF91 E4000000             CALL DWORD PTR DS:[ECX+0xE4]
	$-1A4    00EE4FAC     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1A1    00EE4FAF     5F                        POP EDI
	$-1A0    00EE4FB0     5E                        POP ESI
	$-19F    00EE4FB1     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-198    00EE4FB8     5B                        POP EBX
	$-197    00EE4FB9     8BE5                      MOV ESP,EBP
	$-195    00EE4FBB     5D                        POP EBP
	$-194    00EE4FBC     C3                        RETN
	$-193    00EE4FBD     CC                        INT3
	$-192    00EE4FBE     CC                        INT3
	$-191    00EE4FBF     CC                        INT3
	$-190    00EE4FC0     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-18C    00EE4FC4     F3:0F104424 08            MOVSS XMM0,DWORD PTR SS:[ESP+0x8]
	$-186    00EE4FCA     F3:0F1180 4C010000        MOVSS DWORD PTR DS:[EAX+0x14C],XMM0
	$-17E    00EE4FD2     F3:0F104424 0C            MOVSS XMM0,DWORD PTR SS:[ESP+0xC]
	$-178    00EE4FD8     F3:0F1180 50010000        MOVSS DWORD PTR DS:[EAX+0x150],XMM0
	$-170    00EE4FE0     F3:0F104424 10            MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
	$-16A    00EE4FE6     F3:0F1180 54010000        MOVSS DWORD PTR DS:[EAX+0x154],XMM0
	$-162    00EE4FEE     F3:0F104424 14            MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$-15C    00EE4FF4     F3:0F1180 58010000        MOVSS DWORD PTR DS:[EAX+0x158],XMM0
	$-154    00EE4FFC     C3                        RETN
	$-153    00EE4FFD     CC                        INT3
	$-152    00EE4FFE     CC                        INT3
	$-151    00EE4FFF     CC                        INT3
	$-150    00EE5000     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-14C    00EE5004     F3:0F104424 08            MOVSS XMM0,DWORD PTR SS:[ESP+0x8]
	$-146    00EE500A     F3:0F1180 64010000        MOVSS DWORD PTR DS:[EAX+0x164],XMM0
	$-13E    00EE5012     F3:0F104424 0C            MOVSS XMM0,DWORD PTR SS:[ESP+0xC]
	$-138    00EE5018     F3:0F1180 68010000        MOVSS DWORD PTR DS:[EAX+0x168],XMM0
	$-130    00EE5020     F3:0F104424 10            MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
	$-12A    00EE5026     F3:0F1180 6C010000        MOVSS DWORD PTR DS:[EAX+0x16C],XMM0
	$-122    00EE502E     F3:0F104424 14            MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$-11C    00EE5034     F3:0F1180 70010000        MOVSS DWORD PTR DS:[EAX+0x170],XMM0
	$-114    00EE503C     C3                        RETN
	$-113    00EE503D     CC                        INT3
	$-112    00EE503E     CC                        INT3
	$-111    00EE503F     CC                        INT3
	$-110    00EE5040     8B4C24 04                 MOV ECX,DWORD PTR SS:[ESP+0x4]
	$-10C    00EE5044     F3:0F104424 08            MOVSS XMM0,DWORD PTR SS:[ESP+0x8]
	$-106    00EE504A     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-102    00EE504E     F3:0F1181 5C010000        MOVSS DWORD PTR DS:[ECX+0x15C],XMM0
	$-FA     00EE5056     8981 60010000             MOV DWORD PTR DS:[ECX+0x160],EAX
	$-F4     00EE505C     C3                        RETN
	$-F3     00EE505D     CC                        INT3
	$-F2     00EE505E     CC                        INT3
	$-F1     00EE505F     CC                        INT3
	$-F0     00EE5060     55                        PUSH EBP                                            ; tag_获得机器码_cpuid
	$-EF     00EE5061     8BEC                      MOV EBP,ESP
	$-ED     00EE5063     6A FF                     PUSH -0x1
	$-EB     00EE5065     68 42393E01               PUSH PathOfEx.013E3942
	$-E6     00EE506A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-E0     00EE5070     50                        PUSH EAX
	$-DF     00EE5071     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-D8     00EE5078     81EC 80000000             SUB ESP,0x80
	$-D2     00EE507E     53                        PUSH EBX
	$-D1     00EE507F     56                        PUSH ESI
	$-D0     00EE5080     57                        PUSH EDI
	$-CF     00EE5081     C745 D4 08D39501          MOV DWORD PTR SS:[EBP-0x2C],PathOfEx.0195D308       ; ASCII "\r"
	$-C8     00EE5088     C705 28D39501 0F000000    MOV DWORD PTR DS:[0x195D328],0xF
	$-BE     00EE5092     C705 24D39501 00000000    MOV DWORD PTR DS:[0x195D324],0x0
	$-B4     00EE509C     C605 14D39501 00          MOV BYTE PTR DS:[0x195D314],0x0
	$-AD     00EE50A3     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-A6     00EE50AA     C705 40D39501 0F000000    MOV DWORD PTR DS:[0x195D340],0xF
	$-9C     00EE50B4     C705 3CD39501 00000000    MOV DWORD PTR DS:[0x195D33C],0x0
	$-92     00EE50BE     C605 2CD39501 00          MOV BYTE PTR DS:[0x195D32C],0x0
	$-8B     00EE50C5     C705 44D39501 00000000    MOV DWORD PTR DS:[0x195D344],0x0
	$-81     00EE50CF     C705 48D39501 00000000    MOV DWORD PTR DS:[0x195D348],0x0
	$-77     00EE50D9     C705 4CD39501 00000000    MOV DWORD PTR DS:[0x195D34C],0x0
	$-6D     00EE50E3     C705 50D39501 00000000    MOV DWORD PTR DS:[0x195D350],0x0
	$-63     00EE50ED     C705 54D39501 00000000    MOV DWORD PTR DS:[0x195D354],0x0
	$-59     00EE50F7     C705 58D39501 00000000    MOV DWORD PTR DS:[0x195D358],0x0
	$-4F     00EE5101     C705 5CD39501 00000000    MOV DWORD PTR DS:[0x195D35C],0x0
	$-45     00EE510B     C705 60D39501 00000000    MOV DWORD PTR DS:[0x195D360],0x0
	$-3B     00EE5115     C705 64D39501 00000000    MOV DWORD PTR DS:[0x195D364],0x0
	$-31     00EE511F     C705 68D39501 00000000    MOV DWORD PTR DS:[0x195D368],0x0
	$-27     00EE5129     C705 6CD39501 00000000    MOV DWORD PTR DS:[0x195D36C],0x0
	$-1D     00EE5133     C705 70D39501 00000000    MOV DWORD PTR DS:[0x195D370],0x0
	$-13     00EE513D     33C0                      XOR EAX,EAX
	$-11     00EE513F     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$-D      00EE5143     33C9                      XOR ECX,ECX
	$-B      00EE5145     C705 0CD39501 00000000    MOV DWORD PTR DS:[0x195D30C],0x0
	$-1      00EE514F     53                        PUSH EBX
	$ ==>    00EE5150     0FA2                      CPUID                                               ; g_h_cpuid1_
	$+2      00EE5152     8BF3                      MOV ESI,EBX
	$+4      00EE5154     66:C705 10D39501 0000     MOV WORD PTR DS:[0x195D310],0x0
	$+D      00EE515D     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$+10     00EE5160     5B                        POP EBX
	$+11     00EE5161     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+13     00EE5163     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+16     00EE5166     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+19     00EE5169     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+1C     00EE516C     33FF                      XOR EDI,EDI
	$+1E     00EE516E     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+21     00EE5171     A3 08D39501               MOV DWORD PTR DS:[0x195D308],EAX
	$+26     00EE5176     85C0                      TEST EAX,EAX
	$+28     00EE5178     78 39                     JS SHORT PathOfEx.00EE51B3
	$+2A     00EE517A     66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
	$+30     00EE5180     8BC7                      MOV EAX,EDI
	$+32     00EE5182     33C9                      XOR ECX,ECX
	$+34     00EE5184     53                        PUSH EBX
	$+35     00EE5185     0FA2                      CPUID                                               ; g_h_cpuid2_
	$+37     00EE5187     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$+3A     00EE518A     5B                        POP EBX
	$+3B     00EE518B     8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+3E     00EE518E     8906                      MOV DWORD PTR DS:[ESI],EAX
	$+40     00EE5190     8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+43     00EE5193     8B75 EC                   MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+46     00EE5196     8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+49     00EE5199     8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+4C     00EE519C     B9 5CD39501               MOV ECX,PathOfEx.0195D35C
	$+51     00EE51A1     50                        PUSH EAX
	$+52     00EE51A2     8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+55     00EE51A5     E8 A65FEFFF               CALL PathOfEx.00DDB150
	$+5A     00EE51AA     47                        INC EDI
	$+5B     00EE51AB     3B3D 08D39501             CMP EDI,DWORD PTR DS:[0x195D308]
	$+61     00EE51B1   ^ 7E CD                     JLE SHORT PathOfEx.00EE5180
	$+63     00EE51B3     8B15 5CD39501             MOV EDX,DWORD PTR DS:[0x195D35C]
	$+69     00EE51B9     0F57C0                    XORPS XMM0,XMM0
	$+6C     00EE51BC     0F1145 B4                 MOVUPS DQWORD PTR SS:[EBP-0x4C],XMM0
	$+70     00EE51C0     0F1145 C4                 MOVUPS DQWORD PTR SS:[EBP-0x3C],XMM0
	$+74     00EE51C4     8B4A 04                   MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+77     00EE51C7     894D B4                   MOV DWORD PTR SS:[EBP-0x4C],ECX
	$+7A     00EE51CA     8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+7D     00EE51CD     8945 B8                   MOV DWORD PTR SS:[EBP-0x48],EAX
	$+80     00EE51D0     8B42 08                   MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+83     00EE51D3     8945 BC                   MOV DWORD PTR SS:[EBP-0x44],EAX
	$+86     00EE51D6     84C9                      TEST CL,CL
	$+88     00EE51D8     75 04                     JNZ SHORT PathOfEx.00EE51DE
	$+8A     00EE51DA     33C9                      XOR ECX,ECX
	$+8C     00EE51DC     EB 0F                     JMP SHORT PathOfEx.00EE51ED
	$+8E     00EE51DE     8D4D B4                   LEA ECX,DWORD PTR SS:[EBP-0x4C]
	$+91     00EE51E1     8D51 01                   LEA EDX,DWORD PTR DS:[ECX+0x1]
	$+94     00EE51E4     8A01                      MOV AL,BYTE PTR DS:[ECX]
	$+96     00EE51E6     41                        INC ECX
	$+97     00EE51E7     84C0                      TEST AL,AL
	$+99     00EE51E9   ^ 75 F9                     JNZ SHORT PathOfEx.00EE51E4
	$+9B     00EE51EB     2BCA                      SUB ECX,EDX
	$+9D     00EE51ED     51                        PUSH ECX
	$+9E     00EE51EE     8D45 B4                   LEA EAX,DWORD PTR SS:[EBP-0x4C]
	$+A1     00EE51F1     BE 14D39501               MOV ESI,PathOfEx.0195D314                           ; ASCII "GenuineIntel"
	$+A6     00EE51F6     50                        PUSH EAX
	$+A7     00EE51F7     8BCE                      MOV ECX,ESI
	$+A9     00EE51F9     E8 E208A6FF               CALL PathOfEx.00945AE0
	$+AE     00EE51FE     833D 28D39501 10          CMP DWORD PTR DS:[0x195D328],0x10
	$+B5     00EE5205     8BCE                      MOV ECX,ESI
	$+B7     00EE5207     8B35 24D39501             MOV ESI,DWORD PTR DS:[0x195D324]
	$+BD     00EE520D     B8 0C000000               MOV EAX,0xC
	$+C2     00EE5212     0F430D 14D39501           CMOVNB ECX,DWORD PTR DS:[0x195D314]                 ; shell32.756E6547
	$+C9     00EE5219     3BF0                      CMP ESI,EAX
	$+CB     00EE521B     0F42C6                    CMOVB EAX,ESI
	$+CE     00EE521E     50                        PUSH EAX
	$+CF     00EE521F     68 00A45D01               PUSH PathOfEx.015DA400
	$+D4     00EE5224     51                        PUSH ECX
	$+D5     00EE5225     E8 362DA9FF               CALL PathOfEx.00977F60
	$+DA     00EE522A     83C4 0C                   ADD ESP,0xC
	$+DD     00EE522D     BF 01000000               MOV EDI,0x1
	$+E2     00EE5232     85C0                      TEST EAX,EAX
	$+E4     00EE5234     75 10                     JNZ SHORT PathOfEx.00EE5246
	$+E6     00EE5236     83FE 0C                   CMP ESI,0xC
	$+E9     00EE5239     72 0B                     JB SHORT PathOfEx.00EE5246
	$+EB     00EE523B     77 09                     JA SHORT PathOfEx.00EE5246
	$+ED     00EE523D     C605 10D39501 01          MOV BYTE PTR DS:[0x195D310],0x1
	$+F4     00EE5244     EB 45                     JMP SHORT PathOfEx.00EE528B
	$+F6     00EE5246     833D 28D39501 10          CMP DWORD PTR DS:[0x195D328],0x10
	$+FD     00EE524D     B8 14D39501               MOV EAX,PathOfEx.0195D314                           ; ASCII "GenuineIntel"
	$+102    00EE5252     B9 0C000000               MOV ECX,0xC
	$+107    00EE5257     0F4305 14D39501           CMOVNB EAX,DWORD PTR DS:[0x195D314]                 ; shell32.756E6547
	$+10E    00EE525E     83FE 0C                   CMP ESI,0xC
	$+111    00EE5261     0F42CE                    CMOVB ECX,ESI
	$+114    00EE5264     51                        PUSH ECX
	$+115    00EE5265     68 10A45D01               PUSH PathOfEx.015DA410
	$+11A    00EE526A     50                        PUSH EAX
	$+11B    00EE526B     E8 F02CA9FF               CALL PathOfEx.00977F60
	$+120    00EE5270     83C4 0C                   ADD ESP,0xC
	$+123    00EE5273     85C0                      TEST EAX,EAX
	$+125    00EE5275     75 14                     JNZ SHORT PathOfEx.00EE528B
	$+127    00EE5277     83FE 0C                   CMP ESI,0xC
	$+12A    00EE527A     72 0F                     JB SHORT PathOfEx.00EE528B
	$+12C    00EE527C     0FB605 11D39501           MOVZX EAX,BYTE PTR DS:[0x195D311]
	$+133    00EE5283     0F46C7                    CMOVBE EAX,EDI
	$+136    00EE5286     A2 11D39501               MOV BYTE PTR DS:[0x195D311],AL
	$+13B    00EE528B     8B35 08D39501             MOV ESI,DWORD PTR DS:[0x195D308]
	$+141    00EE5291     8B0D 5CD39501             MOV ECX,DWORD PTR DS:[0x195D35C]
	$+147    00EE5297     83FE 01                   CMP ESI,0x1
	$+14A    00EE529A     7C 18                     JL SHORT PathOfEx.00EE52B4
	$+14C    00EE529C     8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$+14F    00EE529F     99                        CDQ
	$+150    00EE52A0     A3 44D39501               MOV DWORD PTR DS:[0x195D344],EAX
	$+155    00EE52A5     8B41 1C                   MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$+158    00EE52A8     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+15B    00EE52AB     99                        CDQ
	$+15C    00EE52AC     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+15F    00EE52AF     A3 48D39501               MOV DWORD PTR DS:[0x195D348],EAX
	$+164    00EE52B4     83FE 07                   CMP ESI,0x7
	$+167    00EE52B7     7C 18                     JL SHORT PathOfEx.00EE52D1
	$+169    00EE52B9     8B41 74                   MOV EAX,DWORD PTR DS:[ECX+0x74]
	$+16C    00EE52BC     99                        CDQ
	$+16D    00EE52BD     A3 4CD39501               MOV DWORD PTR DS:[0x195D34C],EAX
	$+172    00EE52C2     8B41 78                   MOV EAX,DWORD PTR DS:[ECX+0x78]
	$+175    00EE52C5     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+178    00EE52C8     99                        CDQ
	$+179    00EE52C9     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+17C    00EE52CC     A3 50D39501               MOV DWORD PTR DS:[0x195D350],EAX
	$+181    00EE52D1     B8 00000080               MOV EAX,0x80000000
	$+186    00EE52D6     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$+189    00EE52D9     33C9                      XOR ECX,ECX
	$+18B    00EE52DB     53                        PUSH EBX
	$+18C    00EE52DC     0FA2                      CPUID                                               ; g_h_cpuid3_
	$+18E    00EE52DE     8BF3                      MOV ESI,EBX
	$+190    00EE52E0     5B                        POP EBX
	$+191    00EE52E1     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+193    00EE52E3     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+196    00EE52E6     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+199    00EE52E9     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+19C    00EE52EC     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+19F    00EE52EF     6A 40                     PUSH 0x40
	$+1A1    00EE52F1     A3 0CD39501               MOV DWORD PTR DS:[0x195D30C],EAX
	$+1A6    00EE52F6     8D85 74FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x8C]
	$+1AC    00EE52FC     6A 00                     PUSH 0x0
	$+1AE    00EE52FE     50                        PUSH EAX
	$+1AF    00EE52FF     E8 FCF44200               CALL PathOfEx.01314800
	$+1B4    00EE5304     83C4 0C                   ADD ESP,0xC
	$+1B7    00EE5307     BF 00000080               MOV EDI,0x80000000
	$+1BC    00EE530C     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$+1C0    00EE5310     8BC7                      MOV EAX,EDI
	$+1C2    00EE5312     33C9                      XOR ECX,ECX
	$+1C4    00EE5314     53                        PUSH EBX
	$+1C5    00EE5315     0FA2                      CPUID                                               ; g_h_cpuid4_
	$+1C7    00EE5317     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$+1CA    00EE531A     5B                        POP EBX
	$+1CB    00EE531B     8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+1CE    00EE531E     8906                      MOV DWORD PTR DS:[ESI],EAX
	$+1D0    00EE5320     8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+1D3    00EE5323     8B75 EC                   MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+1D6    00EE5326     8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+1D9    00EE5329     8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+1DC    00EE532C     B9 68D39501               MOV ECX,PathOfEx.0195D368                           ; ASCII "x=U"
	$+1E1    00EE5331     50                        PUSH EAX
	$+1E2    00EE5332     8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+1E5    00EE5335     E8 165EEFFF               CALL PathOfEx.00DDB150
	$+1EA    00EE533A     8B0D 0CD39501             MOV ECX,DWORD PTR DS:[0x195D30C]
	$+1F0    00EE5340     47                        INC EDI
	$+1F1    00EE5341     3BF9                      CMP EDI,ECX
	$+1F3    00EE5343   ^ 7E CB                     JLE SHORT PathOfEx.00EE5310
	$+1F5    00EE5345     8B35 68D39501             MOV ESI,DWORD PTR DS:[0x195D368]
	$+1FB    00EE534B     BF 01000000               MOV EDI,0x1
	$+200    00EE5350     81F9 01000080             CMP ECX,0x80000001
	$+206    00EE5356     72 18                     JB SHORT PathOfEx.00EE5370
	$+208    00EE5358     8B46 18                   MOV EAX,DWORD PTR DS:[ESI+0x18]
	$+20B    00EE535B     99                        CDQ
	$+20C    00EE535C     A3 54D39501               MOV DWORD PTR DS:[0x195D354],EAX
	$+211    00EE5361     8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+214    00EE5364     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+217    00EE5367     99                        CDQ
	$+218    00EE5368     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$+21B    00EE536B     A3 58D39501               MOV DWORD PTR DS:[0x195D358],EAX
	$+220    00EE5370     81F9 04000080             CMP ECX,0x80000004
	$+226    00EE5376     72 4B                     JB SHORT PathOfEx.00EE53C3
	$+228    00EE5378     0F104E 20                 MOVUPS XMM1,DQWORD PTR DS:[ESI+0x20]
	$+22C    00EE537C     0F118D 74FFFFFF           MOVUPS DQWORD PTR SS:[EBP-0x8C],XMM1
	$+233    00EE5383     0F1046 30                 MOVUPS XMM0,DQWORD PTR DS:[ESI+0x30]
	$+237    00EE5387     66:0F7EC8                 MOVD EAX,MM1
	$+23B    00EE538B     0F1145 84                 MOVUPS DQWORD PTR SS:[EBP-0x7C],XMM0
	$+23F    00EE538F     0F1046 40                 MOVUPS XMM0,DQWORD PTR DS:[ESI+0x40]
	$+243    00EE5393     0F1145 94                 MOVUPS DQWORD PTR SS:[EBP-0x6C],XMM0
	$+247    00EE5397     84C0                      TEST AL,AL
	$+249    00EE5399     75 04                     JNZ SHORT PathOfEx.00EE539F
	$+24B    00EE539B     33C9                      XOR ECX,ECX
	$+24D    00EE539D     EB 12                     JMP SHORT PathOfEx.00EE53B1
	$+24F    00EE539F     8D8D 74FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+255    00EE53A5     8D51 01                   LEA EDX,DWORD PTR DS:[ECX+0x1]
	$+258    00EE53A8     8A01                      MOV AL,BYTE PTR DS:[ECX]
	$+25A    00EE53AA     41                        INC ECX
	$+25B    00EE53AB     84C0                      TEST AL,AL
	$+25D    00EE53AD   ^ 75 F9                     JNZ SHORT PathOfEx.00EE53A8
	$+25F    00EE53AF     2BCA                      SUB ECX,EDX
	$+261    00EE53B1     51                        PUSH ECX
	$+262    00EE53B2     8D85 74FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x8C]
	$+268    00EE53B8     B9 2CD39501               MOV ECX,PathOfEx.0195D32C
	$+26D    00EE53BD     50                        PUSH EAX
	$+26E    00EE53BE     E8 1D07A6FF               CALL PathOfEx.00945AE0
	$+273    00EE53C3     6A 19                     PUSH 0x19
	$+275    00EE53C5     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+278    00EE53C8     B9 48D39501               MOV ECX,PathOfEx.0195D348
	$+27D    00EE53CD     50                        PUSH EAX
	$+27E    00EE53CE     E8 3D4AA8FF               CALL PathOfEx.00969E10
	$+283    00EE53D3     C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
	$+287    00EE53D7     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+28A    00EE53DA     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+28C    00EE53DC     83FA 20                   CMP EDX,0x20
	$+28F    00EE53DF     72 0A                     JB SHORT PathOfEx.00EE53EB
	$+291    00EE53E1     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+296    00EE53E6     E8 AA3E4000               CALL PathOfEx.012E9295
	$+29B    00EE53EB     8BCA                      MOV ECX,EDX
	$+29D    00EE53ED     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2A1    00EE53F1     83E1 1F                   AND ECX,0x1F
	$+2A4    00EE53F4     C1EA 05                   SHR EDX,0x5
	$+2A7    00EE53F7     8BC7                      MOV EAX,EDI
	$+2A9    00EE53F9     D3E0                      SHL EAX,CL
	$+2AB    00EE53FB     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+2AE    00EE53FE     0F84 33020000             JE PathOfEx.00EE5637
	$+2B4    00EE5404     6A 1A                     PUSH 0x1A
	$+2B6    00EE5406     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+2B9    00EE5409     B9 48D39501               MOV ECX,PathOfEx.0195D348
	$+2BE    00EE540E     50                        PUSH EAX
	$+2BF    00EE540F     E8 FC49A8FF               CALL PathOfEx.00969E10
	$+2C4    00EE5414     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$+2C8    00EE5418     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+2CB    00EE541B     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+2CD    00EE541D     83FA 20                   CMP EDX,0x20
	$+2D0    00EE5420     72 0A                     JB SHORT PathOfEx.00EE542C
	$+2D2    00EE5422     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+2D7    00EE5427     E8 693E4000               CALL PathOfEx.012E9295
	$+2DC    00EE542C     8BCA                      MOV ECX,EDX
	$+2DE    00EE542E     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2E2    00EE5432     83E1 1F                   AND ECX,0x1F
	$+2E5    00EE5435     C1EA 05                   SHR EDX,0x5
	$+2E8    00EE5438     8BC7                      MOV EAX,EDI
	$+2EA    00EE543A     D3E0                      SHL EAX,CL
	$+2EC    00EE543C     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+2EF    00EE543F     0F84 F2010000             JE PathOfEx.00EE5637
	$+2F5    00EE5445     6A 00                     PUSH 0x0
	$+2F7    00EE5447     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+2FA    00EE544A     C645 F2 01                MOV BYTE PTR SS:[EBP-0xE],0x1
	$+2FE    00EE544E     50                        PUSH EAX
	$+2FF    00EE544F     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+304    00EE5454     E8 B749A8FF               CALL PathOfEx.00969E10
	$+309    00EE5459     C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
	$+30D    00EE545D     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+310    00EE5460     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+312    00EE5462     83FA 20                   CMP EDX,0x20
	$+315    00EE5465     72 0A                     JB SHORT PathOfEx.00EE5471
	$+317    00EE5467     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+31C    00EE546C     E8 243E4000               CALL PathOfEx.012E9295
	$+321    00EE5471     8BCA                      MOV ECX,EDX
	$+323    00EE5473     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+327    00EE5477     83E1 1F                   AND ECX,0x1F
	$+32A    00EE547A     C1EA 05                   SHR EDX,0x5
	$+32D    00EE547D     8BC7                      MOV EAX,EDI
	$+32F    00EE547F     D3E0                      SHL EAX,CL
	$+331    00EE5481     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+334    00EE5484     0F84 B1010000             JE PathOfEx.00EE563B
	$+33A    00EE548A     6A 13                     PUSH 0x13
	$+33C    00EE548C     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+33F    00EE548F     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+344    00EE5494     50                        PUSH EAX
	$+345    00EE5495     E8 7649A8FF               CALL PathOfEx.00969E10
	$+34A    00EE549A     C645 FC 07                MOV BYTE PTR SS:[EBP-0x4],0x7
	$+34E    00EE549E     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+351    00EE54A1     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+353    00EE54A3     83FA 20                   CMP EDX,0x20
	$+356    00EE54A6     72 0A                     JB SHORT PathOfEx.00EE54B2
	$+358    00EE54A8     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+35D    00EE54AD     E8 E33D4000               CALL PathOfEx.012E9295
	$+362    00EE54B2     8BCA                      MOV ECX,EDX
	$+364    00EE54B4     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+368    00EE54B8     83E1 1F                   AND ECX,0x1F
	$+36B    00EE54BB     C1EA 05                   SHR EDX,0x5
	$+36E    00EE54BE     8BC7                      MOV EAX,EDI
	$+370    00EE54C0     D3E0                      SHL EAX,CL
	$+372    00EE54C2     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+375    00EE54C5     0F84 70010000             JE PathOfEx.00EE563B
	$+37B    00EE54CB     6A 1C                     PUSH 0x1C
	$+37D    00EE54CD     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+380    00EE54D0     C645 F3 01                MOV BYTE PTR SS:[EBP-0xD],0x1
	$+384    00EE54D4     50                        PUSH EAX
	$+385    00EE54D5     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+38A    00EE54DA     E8 3149A8FF               CALL PathOfEx.00969E10
	$+38F    00EE54DF     C645 FC 08                MOV BYTE PTR SS:[EBP-0x4],0x8
	$+393    00EE54E3     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+396    00EE54E6     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+398    00EE54E8     83FA 20                   CMP EDX,0x20
	$+39B    00EE54EB     72 0A                     JB SHORT PathOfEx.00EE54F7
	$+39D    00EE54ED     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+3A2    00EE54F2     E8 9E3D4000               CALL PathOfEx.012E9295
	$+3A7    00EE54F7     8BCA                      MOV ECX,EDX
	$+3A9    00EE54F9     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+3AD    00EE54FD     83E1 1F                   AND ECX,0x1F
	$+3B0    00EE5500     C1EA 05                   SHR EDX,0x5
	$+3B3    00EE5503     8BC7                      MOV EAX,EDI
	$+3B5    00EE5505     D3E0                      SHL EAX,CL
	$+3B7    00EE5507     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+3BA    00EE550A     0F84 2F010000             JE PathOfEx.00EE563F
	$+3C0    00EE5510     6A 1D                     PUSH 0x1D
	$+3C2    00EE5512     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+3C5    00EE5515     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+3CA    00EE551A     50                        PUSH EAX
	$+3CB    00EE551B     E8 F048A8FF               CALL PathOfEx.00969E10
	$+3D0    00EE5520     C645 FC 09                MOV BYTE PTR SS:[EBP-0x4],0x9
	$+3D4    00EE5524     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+3D7    00EE5527     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+3D9    00EE5529     83FA 20                   CMP EDX,0x20
	$+3DC    00EE552C     72 0A                     JB SHORT PathOfEx.00EE5538
	$+3DE    00EE552E     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+3E3    00EE5533     E8 5D3D4000               CALL PathOfEx.012E9295
	$+3E8    00EE5538     8BCA                      MOV ECX,EDX
	$+3EA    00EE553A     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+3EE    00EE553E     83E1 1F                   AND ECX,0x1F
	$+3F1    00EE5541     C1EA 05                   SHR EDX,0x5
	$+3F4    00EE5544     8BC7                      MOV EAX,EDI
	$+3F6    00EE5546     D3E0                      SHL EAX,CL
	$+3F8    00EE5548     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+3FB    00EE554B     0F84 EE000000             JE PathOfEx.00EE563F
	$+401    00EE5551     6A 1B                     PUSH 0x1B
	$+403    00EE5553     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+406    00EE5556     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+40B    00EE555B     50                        PUSH EAX
	$+40C    00EE555C     E8 AF48A8FF               CALL PathOfEx.00969E10
	$+411    00EE5561     C645 FC 0A                MOV BYTE PTR SS:[EBP-0x4],0xA
	$+415    00EE5565     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+418    00EE5568     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+41A    00EE556A     83FA 20                   CMP EDX,0x20
	$+41D    00EE556D     72 0A                     JB SHORT PathOfEx.00EE5579
	$+41F    00EE556F     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+424    00EE5574     E8 1C3D4000               CALL PathOfEx.012E9295
	$+429    00EE5579     8BCA                      MOV ECX,EDX
	$+42B    00EE557B     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+42F    00EE557F     83E1 1F                   AND ECX,0x1F
	$+432    00EE5582     C1EA 05                   SHR EDX,0x5
	$+435    00EE5585     8BC7                      MOV EAX,EDI
	$+437    00EE5587     D3E0                      SHL EAX,CL
	$+439    00EE5589     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+43C    00EE558C     0F84 AD000000             JE PathOfEx.00EE563F
	$+442    00EE5592     33C9                      XOR ECX,ECX
	$+444    00EE5594     0F01D0                    XGETBV
	$+447    00EE5597     83E0 06                   AND EAX,0x6
	$+44A    00EE559A     83F8 06                   CMP EAX,0x6
	$+44D    00EE559D     0F85 9C000000             JNZ PathOfEx.00EE563F
	$+453    00EE55A3     85C9                      TEST ECX,ECX
	$+455    00EE55A5     0F85 94000000             JNZ PathOfEx.00EE563F
	$+45B    00EE55AB     6A 05                     PUSH 0x5
	$+45D    00EE55AD     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+460    00EE55B0     B9 4CD39501               MOV ECX,PathOfEx.0195D34C
	$+465    00EE55B5     50                        PUSH EAX
	$+466    00EE55B6     E8 5548A8FF               CALL PathOfEx.00969E10
	$+46B    00EE55BB     C645 FC 0B                MOV BYTE PTR SS:[EBP-0x4],0xB
	$+46F    00EE55BF     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+472    00EE55C2     8B30                      MOV ESI,DWORD PTR DS:[EAX]
	$+474    00EE55C4     83FA 20                   CMP EDX,0x20
	$+477    00EE55C7     72 0A                     JB SHORT PathOfEx.00EE55D3
	$+479    00EE55C9     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+47E    00EE55CE     E8 C23C4000               CALL PathOfEx.012E9295
	$+483    00EE55D3     8BCA                      MOV ECX,EDX
	$+485    00EE55D5     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+489    00EE55D9     83E1 1F                   AND ECX,0x1F
	$+48C    00EE55DC     C1EA 05                   SHR EDX,0x5
	$+48F    00EE55DF     8BC7                      MOV EAX,EDI
	$+491    00EE55E1     D3E0                      SHL EAX,CL
	$+493    00EE55E3     850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
	$+496    00EE55E6     74 43                     JE SHORT PathOfEx.00EE562B
	$+498    00EE55E8     6A 0C                     PUSH 0xC
	$+49A    00EE55EA     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+49D    00EE55ED     B9 44D39501               MOV ECX,PathOfEx.0195D344
	$+4A2    00EE55F2     50                        PUSH EAX
	$+4A3    00EE55F3     E8 1848A8FF               CALL PathOfEx.00969E10
	$+4A8    00EE55F8     C645 FC 0C                MOV BYTE PTR SS:[EBP-0x4],0xC
	$+4AC    00EE55FC     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+4AF    00EE55FF     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+4B1    00EE5601     83FA 20                   CMP EDX,0x20
	$+4B4    00EE5604     72 0A                     JB SHORT PathOfEx.00EE5610
	$+4B6    00EE5606     68 34BA5001               PUSH PathOfEx.0150BA34                              ; ASCII 69,"nvalid bitset<N> position"
	$+4BB    00EE560B     E8 853C4000               CALL PathOfEx.012E9295
	$+4C0    00EE5610     8BCA                      MOV ECX,EDX
	$+4C2    00EE5612     C1EA 05                   SHR EDX,0x5
	$+4C5    00EE5615     83E1 1F                   AND ECX,0x1F
	$+4C8    00EE5618     D3E7                      SHL EDI,CL
	$+4CA    00EE561A     853C90                    TEST DWORD PTR DS:[EAX+EDX*4],EDI
	$+4CD    00EE561D     74 0C                     JE SHORT PathOfEx.00EE562B
	$+4CF    00EE561F     C705 74D39501 05000000    MOV DWORD PTR DS:[0x195D374],0x5
	$+4D9    00EE5629     EB 35                     JMP SHORT PathOfEx.00EE5660
	$+4DB    00EE562B     C705 74D39501 04000000    MOV DWORD PTR DS:[0x195D374],0x4
	$+4E5    00EE5635     EB 29                     JMP SHORT PathOfEx.00EE5660
	$+4E7    00EE5637     C645 F2 00                MOV BYTE PTR SS:[EBP-0xE],0x0
	$+4EB    00EE563B     32C0                      XOR AL,AL
	$+4ED    00EE563D     EB 03                     JMP SHORT PathOfEx.00EE5642
	$+4EF    00EE563F     8A45 F3                   MOV AL,BYTE PTR SS:[EBP-0xD]
	$+4F2    00EE5642     84C0                      TEST AL,AL
	$+4F4    00EE5644     74 0C                     JE SHORT PathOfEx.00EE5652
	$+4F6    00EE5646     C705 74D39501 03000000    MOV DWORD PTR DS:[0x195D374],0x3
	$+500    00EE5650     EB 0E                     JMP SHORT PathOfEx.00EE5660
	$+502    00EE5652     33C0                      XOR EAX,EAX
	$+504    00EE5654     3845 F2                   CMP BYTE PTR SS:[EBP-0xE],AL
	$+507    00EE5657     0F95C0                    SETNE AL
	$+50A    00EE565A     40                        INC EAX
	$+50B    00EE565B     A3 74D39501               MOV DWORD PTR DS:[0x195D374],EAX
	$+510    00EE5660     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+513    00EE5663     B8 08D39501               MOV EAX,PathOfEx.0195D308                           ; ASCII "\r"
	$+518    00EE5668     5F                        POP EDI
	$+519    00EE5669     5E                        POP ESI
	$+51A    00EE566A     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+521    00EE5671     5B                        POP EBX
	$+522    00EE5672     8BE5                      MOV ESP,EBP
	$+524    00EE5674     5D                        POP EBP
	$+525    00EE5675     C3                        RETN
	$+526    00EE5676     CC                        INT3
	$+527    00EE5677     CC                        INT3
	$+528    00EE5678     CC                        INT3
	$+529    00EE5679     CC                        INT3
	$+52A    00EE567A     CC                        INT3
	$+52B    00EE567B     CC                        INT3
	$+52C    00EE567C     CC                        INT3
	$+52D    00EE567D     CC                        INT3
	$+52E    00EE567E     CC                        INT3
	$+52F    00EE567F     CC                        INT3
	$+530    00EE5680     55                        PUSH EBP
	$+531    00EE5681     8BEC                      MOV EBP,ESP
	$+533    00EE5683     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+539    00EE5689     6A FF                     PUSH -0x1
	$+53B    00EE568B     68 5C393E01               PUSH PathOfEx.013E395C
	$+540    00EE5690     50                        PUSH EAX
	$+541    00EE5691     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+548    00EE5698     64:A1 2C000000            MOV EAX,DWORD PTR FS:[0x2C]
	$+54E    00EE569E     8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+550    00EE56A0     A1 00D39501               MOV EAX,DWORD PTR DS:[0x195D300]
	$+555    00EE56A5     3B81 20000000             CMP EAX,DWORD PTR DS:[ECX+0x20]
	$+55B    00EE56AB     7E 39                     JLE SHORT PathOfEx.00EE56E6
	$+55D    00EE56AD     68 00D39501               PUSH PathOfEx.0195D300                              ; ASCII "\n"
	$+562    00EE56B2     E8 29624100               CALL PathOfEx.012FB8E0
	$+567    00EE56B7     83C4 04                   ADD ESP,0x4
	$+56A    00EE56BA     833D 00D39501 FF          CMP DWORD PTR DS:[0x195D300],-0x1
	$+571    00EE56C1     75 23                     JNZ SHORT PathOfEx.00EE56E6
	$+573    00EE56C3     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+57A    00EE56CA     E8 91F9FFFF               CALL PathOfEx.00EE5060
	$+57F    00EE56CF     68 308C4001               PUSH PathOfEx.01408C30
	$+584    00EE56D4     E8 7D604100               CALL PathOfEx.012FB756
	$+589    00EE56D9     68 00D39501               PUSH PathOfEx.0195D300                              ; ASCII "\n"
	$+58E    00EE56DE     E8 BE614100               CALL PathOfEx.012FB8A1
	$+593    00EE56E3     83C4 08                   ADD ESP,0x8
	$+596    00EE56E6     A1 74D39501               MOV EAX,DWORD PTR DS:[0x195D374]
	$+59B    00EE56EB     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+59E    00EE56EE     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+5A5    00EE56F5     8BE5                      MOV ESP,EBP
	$+5A7    00EE56F7     5D                        POP EBP
	$+5A8    00EE56F8     C3                        RETN
	$+5A9    00EE56F9     CC                        INT3
	$+5AA    00EE56FA     CC                        INT3
	$+5AB    00EE56FB     CC                        INT3
	$+5AC    00EE56FC     CC                        INT3
	$+5AD    00EE56FD     CC                        INT3
	$+5AE    00EE56FE     CC                        INT3
	$+5AF    00EE56FF     CC                        INT3
	$+5B0    00EE5700     8B0D 68D39501             MOV ECX,DWORD PTR DS:[0x195D368]
	$+5B6    00EE5706     85C9                      TEST ECX,ECX
	$+5B8    00EE5708     74 34                     JE SHORT PathOfEx.00EE573E
	$+5BA    00EE570A     A1 70D39501               MOV EAX,DWORD PTR DS:[0x195D370]
	$+5BF    00EE570F     2BC1                      SUB EAX,ECX
	$+5C1    00EE5711     6A 10                     PUSH 0x10
	$+5C3    00EE5713     C1F8 04                   SAR EAX,0x4
	$+5C6    00EE5716     50                        PUSH EAX
	$+5C7    00EE5717     51                        PUSH ECX
	$+5C8    00EE5718     E8 5302A6FF               CALL PathOfEx.00945970
	$+5CD    00EE571D     83C4 0C                   ADD ESP,0xC
	$+5D0    00EE5720     C705 68D39501 00000000    MOV DWORD PTR DS:[0x195D368],0x0
	$+5DA    00EE572A     C705 6CD39501 00000000    MOV DWORD PTR DS:[0x195D36C],0x0
	$+5E4    00EE5734     C705 70D39501 00000000    MOV DWORD PTR DS:[0x195D370],0x0
	$+5EE    00EE573E     8B0D 5CD39501             MOV ECX,DWORD PTR DS:[0x195D35C]
	$+5F4    00EE5744     85C9                      TEST ECX,ECX
	$+5F6    00EE5746     74 34                     JE SHORT PathOfEx.00EE577C
	$+5F8    00EE5748     A1 64D39501               MOV EAX,DWORD PTR DS:[0x195D364]
	$+5FD    00EE574D     2BC1                      SUB EAX,ECX
	$+5FF    00EE574F     6A 10                     PUSH 0x10
	$+601    00EE5751     C1F8 04                   SAR EAX,0x4
	$+604    00EE5754     50                        PUSH EAX
	$+605    00EE5755     51                        PUSH ECX
	$+606    00EE5756     E8 1502A6FF               CALL PathOfEx.00945970
	$+60B    00EE575B     83C4 0C                   ADD ESP,0xC
	$+60E    00EE575E     C705 5CD39501 00000000    MOV DWORD PTR DS:[0x195D35C],0x0
	$+618    00EE5768     C705 60D39501 00000000    MOV DWORD PTR DS:[0x195D360],0x0
	$+622    00EE5772     C705 64D39501 00000000    MOV DWORD PTR DS:[0x195D364],0x0
	$+62C    00EE577C     A1 40D39501               MOV EAX,DWORD PTR DS:[0x195D340]
	$+631    00EE5781     83F8 10                   CMP EAX,0x10
	$+634    00EE5784     72 12                     JB SHORT PathOfEx.00EE5798
	$+636    00EE5786     6A 01                     PUSH 0x1
	$+638    00EE5788     40                        INC EAX
	$+639    00EE5789     50                        PUSH EAX
	$+63A    00EE578A     FF35 2CD39501             PUSH DWORD PTR DS:[0x195D32C]
	$+640    00EE5790     E8 DB01A6FF               CALL PathOfEx.00945970
	$+645    00EE5795     83C4 0C                   ADD ESP,0xC
	$+648    00EE5798     A1 28D39501               MOV EAX,DWORD PTR DS:[0x195D328]
	$+64D    00EE579D     C705 40D39501 0F000000    MOV DWORD PTR DS:[0x195D340],0xF
	$+657    00EE57A7     C705 3CD39501 00000000    MOV DWORD PTR DS:[0x195D33C],0x0
	$+661    00EE57B1     C605 2CD39501 00          MOV BYTE PTR DS:[0x195D32C],0x0
	$+668    00EE57B8     83F8 10                   CMP EAX,0x10
	$+66B    00EE57BB     72 12                     JB SHORT PathOfEx.00EE57CF
	$+66D    00EE57BD     6A 01                     PUSH 0x1
	$+66F    00EE57BF     40                        INC EAX
	$+670    00EE57C0     50                        PUSH EAX
	$+671    00EE57C1     FF35 14D39501             PUSH DWORD PTR DS:[0x195D314]                       ; shell32.756E6547
	$+677    00EE57C7     E8 A401A6FF               CALL PathOfEx.00945970
	$+67C    00EE57CC     83C4 0C                   ADD ESP,0xC
	$+67F    00EE57CF     C705 28D39501 0F000000    MOV DWORD PTR DS:[0x195D328],0xF
	$+689    00EE57D9     C705 24D39501 00000000    MOV DWORD PTR DS:[0x195D324],0x0
	$+693    00EE57E3     C605 14D39501 00          MOV BYTE PTR DS:[0x195D314],0x0
	$+69A    00EE57EA     C3                        RETN
	$+69B    00EE57EB     CC                        INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("0f??8b??66????????????????8d????5b89??89????89????89????33??8b????a3????????85??78??66??????????8b??33??530fa2", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("0f??8b??66????????????????8d????5b89??89????89????89????33??8b????a3????????85??78??66??????????8b??33??530fa2", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_cpuid1_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

#ifdef VER_GJ3_
	addr_tmp = (pt_pcbyte)addr_tmp + 0x35;
#else
	addr_tmp = (pt_pcbyte)addr_tmp + 0x2f;
#endif
	ofs << "pt_cdword	g_h_cpuid2_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//上面函数的一部分
	/*
	$-7B     0086BF2D    0F42CE                    CMOVB ECX,ESI
	$-78     0086BF30    51                        PUSH ECX
	$-77     0086BF31    68 2C18E200               PUSH PathOfEx.00E2182C                          ; ASCII "AuthenticAMD"
	$-72     0086BF36    50                        PUSH EAX
	$-71     0086BF37    E8 449BAFFF               CALL PathOfEx.00365A80
	$-6C     0086BF3C    83C4 0C                   ADD ESP,0xC
	$-69     0086BF3F    85C0                      TEST EAX,EAX
	$-67     0086BF41    75 14                     JNZ SHORT PathOfEx.0086BF57
	$-65     0086BF43    83FE 0C                   CMP ESI,0xC
	$-62     0086BF46    72 0F                     JB SHORT PathOfEx.0086BF57
	$-60     0086BF48    0FB605 C9470901           MOVZX EAX,BYTE PTR DS:[0x10947C9]
	$-59     0086BF4F    0F46C7                    CMOVBE EAX,EDI
	$-56     0086BF52    A2 C9470901               MOV BYTE PTR DS:[0x10947C9],AL
	$-51     0086BF57    8B35 C0470901             MOV ESI,DWORD PTR DS:[0x10947C0]
	$-4B     0086BF5D    8B0D 14480901             MOV ECX,DWORD PTR DS:[0x1094814]
	$-45     0086BF63    83FE 01                   CMP ESI,0x1
	$-42     0086BF66    7C 18                     JL SHORT PathOfEx.0086BF80
	$-40     0086BF68    8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-3D     0086BF6B    99                        CDQ
	$-3C     0086BF6C    A3 FC470901               MOV DWORD PTR DS:[0x10947FC],EAX
	$-37     0086BF71    8B41 1C                   MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$-34     0086BF74    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$-31     0086BF77    99                        CDQ
	$-30     0086BF78    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$-2D     0086BF7B    A3 00480901               MOV DWORD PTR DS:[0x1094800],EAX
	$-28     0086BF80    83FE 07                   CMP ESI,0x7
	$-25     0086BF83    7C 18                     JL SHORT PathOfEx.0086BF9D
	$-23     0086BF85    8B41 74                   MOV EAX,DWORD PTR DS:[ECX+0x74]
	$-20     0086BF88    99                        CDQ
	$-1F     0086BF89    A3 04480901               MOV DWORD PTR DS:[0x1094804],EAX
	$-1A     0086BF8E    8B41 78                   MOV EAX,DWORD PTR DS:[ECX+0x78]
	$-17     0086BF91    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$-14     0086BF94    99                        CDQ
	$-13     0086BF95    8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$-10     0086BF98    A3 08480901               MOV DWORD PTR DS:[0x1094808],EAX
	$-B      0086BF9D    B8 00000080               MOV EAX,0x80000000
	$-6      0086BFA2    8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-3      0086BFA5    33C9                      XOR ECX,ECX
	$-1      0086BFA7    53                        PUSH EBX
	$ ==>    0086BFA8    0FA2                      CPUID                                           ; g_h_cpuid3_
	$+2      0086BFAA    8BF3                      MOV ESI,EBX
	$+4      0086BFAC    5B                        POP EBX
	$+5      0086BFAD    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      0086BFAF    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+A      0086BFB2    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      0086BFB5    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     0086BFB8    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+13     0086BFBB    6A 40                     PUSH 0x40
	$+15     0086BFBD    A3 C4470901               MOV DWORD PTR DS:[0x10947C4],EAX
	$+1A     0086BFC2    8D85 78FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x88]
	$+20     0086BFC8    6A 00                     PUSH 0x0
	$+22     0086BFCA    50                        PUSH EAX
	$+23     0086BFCB    E8 50EA3300               CALL PathOfEx.00BAAA20
	$+28     0086BFD0    83C4 0C                   ADD ESP,0xC
	$+2B     0086BFD3    BF 00000080               MOV EDI,0x80000000
	$+30     0086BFD8    8BC7                      MOV EAX,EDI
	$+32     0086BFDA    33C9                      XOR ECX,ECX
	$+34     0086BFDC    53                        PUSH EBX
	$+35     0086BFDD    0FA2                      CPUID                                           ; g_h_cpuid4_
	$+37     0086BFDF    895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$+3A     0086BFE2    5B                        POP EBX
	$+3B     0086BFE3    8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+3E     0086BFE6    8906                      MOV DWORD PTR DS:[ESI],EAX
	$+40     0086BFE8    8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+43     0086BFEB    8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+46     0086BFEE    8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+49     0086BFF1    8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+4C     0086BFF4    B9 20480901               MOV ECX,PathOfEx.01094820
	$+51     0086BFF9    50                        PUSH EAX
	$+52     0086BFFA    8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+55     0086BFFD    E8 3E25F1FF               CALL PathOfEx.0077E540
	$+5A     0086C002    8B0D C4470901             MOV ECX,DWORD PTR DS:[0x10947C4]
	$+60     0086C008    47                        INC EDI
	$+61     0086C009    3BF9                      CMP EDI,ECX
	$+63     0086C00B  ^ 7E CB                     JLE SHORT PathOfEx.0086BFD8
	$+65     0086C00D    8B35 20480901             MOV ESI,DWORD PTR DS:[0x1094820]
	*/
	//gj3
	/*
	$-18D    00EE514F     53                        PUSH EBX
	$-18C    00EE5150     0FA2                      CPUID                                               ; g_h_cpuid1_
	$-18A    00EE5152     8BF3                      MOV ESI,EBX
	$-188    00EE5154     66:C705 10D39501 0000     MOV WORD PTR DS:[0x195D310],0x0
	$-17F    00EE515D     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-17C    00EE5160     5B                        POP EBX
	$-17B    00EE5161     8907                      MOV DWORD PTR DS:[EDI],EAX
	$-179    00EE5163     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$-176    00EE5166     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$-173    00EE5169     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$-170    00EE516C     33FF                      XOR EDI,EDI
	$-16E    00EE516E     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-16B    00EE5171     A3 08D39501               MOV DWORD PTR DS:[0x195D308],EAX
	$-166    00EE5176     85C0                      TEST EAX,EAX
	$-164    00EE5178     78 39                     JS SHORT PathOfEx.00EE51B3
	$-162    00EE517A     66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
	$-15C    00EE5180     8BC7                      MOV EAX,EDI
	$-15A    00EE5182     33C9                      XOR ECX,ECX
	$-158    00EE5184     53                        PUSH EBX
	$-157    00EE5185     0FA2                      CPUID                                               ; g_h_cpuid2_
	$-155    00EE5187     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$-152    00EE518A     5B                        POP EBX
	$-151    00EE518B     8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$-14E    00EE518E     8906                      MOV DWORD PTR DS:[ESI],EAX
	$-14C    00EE5190     8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-149    00EE5193     8B75 EC                   MOV ESI,DWORD PTR SS:[EBP-0x14]
	$-146    00EE5196     8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$-143    00EE5199     8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$-140    00EE519C     B9 5CD39501               MOV ECX,PathOfEx.0195D35C
	$-13B    00EE51A1     50                        PUSH EAX
	$-13A    00EE51A2     8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$-137    00EE51A5     E8 A65FEFFF               CALL PathOfEx.00DDB150
	$-132    00EE51AA     47                        INC EDI
	$-131    00EE51AB     3B3D 08D39501             CMP EDI,DWORD PTR DS:[0x195D308]
	$-12B    00EE51B1   ^ 7E CD                     JLE SHORT PathOfEx.00EE5180
	$-129    00EE51B3     8B15 5CD39501             MOV EDX,DWORD PTR DS:[0x195D35C]
	$-123    00EE51B9     0F57C0                    XORPS XMM0,XMM0
	$-120    00EE51BC     0F1145 B4                 MOVUPS DQWORD PTR SS:[EBP-0x4C],XMM0
	$-11C    00EE51C0     0F1145 C4                 MOVUPS DQWORD PTR SS:[EBP-0x3C],XMM0
	$-118    00EE51C4     8B4A 04                   MOV ECX,DWORD PTR DS:[EDX+0x4]
	$-115    00EE51C7     894D B4                   MOV DWORD PTR SS:[EBP-0x4C],ECX
	$-112    00EE51CA     8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
	$-10F    00EE51CD     8945 B8                   MOV DWORD PTR SS:[EBP-0x48],EAX
	$-10C    00EE51D0     8B42 08                   MOV EAX,DWORD PTR DS:[EDX+0x8]
	$-109    00EE51D3     8945 BC                   MOV DWORD PTR SS:[EBP-0x44],EAX
	$-106    00EE51D6     84C9                      TEST CL,CL
	$-104    00EE51D8     75 04                     JNZ SHORT PathOfEx.00EE51DE
	$-102    00EE51DA     33C9                      XOR ECX,ECX
	$-100    00EE51DC     EB 0F                     JMP SHORT PathOfEx.00EE51ED
	$-FE     00EE51DE     8D4D B4                   LEA ECX,DWORD PTR SS:[EBP-0x4C]
	$-FB     00EE51E1     8D51 01                   LEA EDX,DWORD PTR DS:[ECX+0x1]
	$-F8     00EE51E4     8A01                      MOV AL,BYTE PTR DS:[ECX]
	$-F6     00EE51E6     41                        INC ECX
	$-F5     00EE51E7     84C0                      TEST AL,AL
	$-F3     00EE51E9   ^ 75 F9                     JNZ SHORT PathOfEx.00EE51E4
	$-F1     00EE51EB     2BCA                      SUB ECX,EDX
	$-EF     00EE51ED     51                        PUSH ECX
	$-EE     00EE51EE     8D45 B4                   LEA EAX,DWORD PTR SS:[EBP-0x4C]
	$-EB     00EE51F1     BE 14D39501               MOV ESI,PathOfEx.0195D314                           ; ASCII "GenuineIntel"
	$-E6     00EE51F6     50                        PUSH EAX
	$-E5     00EE51F7     8BCE                      MOV ECX,ESI
	$-E3     00EE51F9     E8 E208A6FF               CALL PathOfEx.00945AE0
	$-DE     00EE51FE     833D 28D39501 10          CMP DWORD PTR DS:[0x195D328],0x10
	$-D7     00EE5205     8BCE                      MOV ECX,ESI
	$-D5     00EE5207     8B35 24D39501             MOV ESI,DWORD PTR DS:[0x195D324]
	$-CF     00EE520D     B8 0C000000               MOV EAX,0xC
	$-CA     00EE5212     0F430D 14D39501           CMOVNB ECX,DWORD PTR DS:[0x195D314]                 ; shell32.756E6547
	$-C3     00EE5219     3BF0                      CMP ESI,EAX
	$-C1     00EE521B     0F42C6                    CMOVB EAX,ESI
	$-BE     00EE521E     50                        PUSH EAX
	$-BD     00EE521F     68 00A45D01               PUSH PathOfEx.015DA400
	$-B8     00EE5224     51                        PUSH ECX
	$-B7     00EE5225     E8 362DA9FF               CALL PathOfEx.00977F60
	$-B2     00EE522A     83C4 0C                   ADD ESP,0xC
	$-AF     00EE522D     BF 01000000               MOV EDI,0x1
	$-AA     00EE5232     85C0                      TEST EAX,EAX
	$-A8     00EE5234     75 10                     JNZ SHORT PathOfEx.00EE5246
	$-A6     00EE5236     83FE 0C                   CMP ESI,0xC
	$-A3     00EE5239     72 0B                     JB SHORT PathOfEx.00EE5246
	$-A1     00EE523B     77 09                     JA SHORT PathOfEx.00EE5246
	$-9F     00EE523D     C605 10D39501 01          MOV BYTE PTR DS:[0x195D310],0x1
	$-98     00EE5244     EB 45                     JMP SHORT PathOfEx.00EE528B
	$-96     00EE5246     833D 28D39501 10          CMP DWORD PTR DS:[0x195D328],0x10
	$-8F     00EE524D     B8 14D39501               MOV EAX,PathOfEx.0195D314                           ; ASCII "GenuineIntel"
	$-8A     00EE5252     B9 0C000000               MOV ECX,0xC
	$-85     00EE5257     0F4305 14D39501           CMOVNB EAX,DWORD PTR DS:[0x195D314]                 ; shell32.756E6547
	$-7E     00EE525E     83FE 0C                   CMP ESI,0xC
	$-7B     00EE5261     0F42CE                    CMOVB ECX,ESI
	$-78     00EE5264     51                        PUSH ECX
	$-77     00EE5265     68 10A45D01               PUSH PathOfEx.015DA410
	$-72     00EE526A     50                        PUSH EAX
	$-71     00EE526B     E8 F02CA9FF               CALL PathOfEx.00977F60
	$-6C     00EE5270     83C4 0C                   ADD ESP,0xC
	$-69     00EE5273     85C0                      TEST EAX,EAX
	$-67     00EE5275     75 14                     JNZ SHORT PathOfEx.00EE528B
	$-65     00EE5277     83FE 0C                   CMP ESI,0xC
	$-62     00EE527A     72 0F                     JB SHORT PathOfEx.00EE528B
	$-60     00EE527C     0FB605 11D39501           MOVZX EAX,BYTE PTR DS:[0x195D311]
	$-59     00EE5283     0F46C7                    CMOVBE EAX,EDI
	$-56     00EE5286     A2 11D39501               MOV BYTE PTR DS:[0x195D311],AL
	$-51     00EE528B     8B35 08D39501             MOV ESI,DWORD PTR DS:[0x195D308]
	$-4B     00EE5291     8B0D 5CD39501             MOV ECX,DWORD PTR DS:[0x195D35C]
	$-45     00EE5297     83FE 01                   CMP ESI,0x1
	$-42     00EE529A     7C 18                     JL SHORT PathOfEx.00EE52B4
	$-40     00EE529C     8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-3D     00EE529F     99                        CDQ
	$-3C     00EE52A0     A3 44D39501               MOV DWORD PTR DS:[0x195D344],EAX
	$-37     00EE52A5     8B41 1C                   MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$-34     00EE52A8     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$-31     00EE52AB     99                        CDQ
	$-30     00EE52AC     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$-2D     00EE52AF     A3 48D39501               MOV DWORD PTR DS:[0x195D348],EAX
	$-28     00EE52B4     83FE 07                   CMP ESI,0x7
	$-25     00EE52B7     7C 18                     JL SHORT PathOfEx.00EE52D1
	$-23     00EE52B9     8B41 74                   MOV EAX,DWORD PTR DS:[ECX+0x74]
	$-20     00EE52BC     99                        CDQ
	$-1F     00EE52BD     A3 4CD39501               MOV DWORD PTR DS:[0x195D34C],EAX
	$-1A     00EE52C2     8B41 78                   MOV EAX,DWORD PTR DS:[ECX+0x78]
	$-17     00EE52C5     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$-14     00EE52C8     99                        CDQ
	$-13     00EE52C9     8955 EC                   MOV DWORD PTR SS:[EBP-0x14],EDX
	$-10     00EE52CC     A3 50D39501               MOV DWORD PTR DS:[0x195D350],EAX
	$-B      00EE52D1     B8 00000080               MOV EAX,0x80000000
	$-6      00EE52D6     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-3      00EE52D9     33C9                      XOR ECX,ECX
	$-1      00EE52DB     53                        PUSH EBX
	$ ==>    00EE52DC     0FA2                      CPUID                                               ; g_h_cpuid3_
	$+2      00EE52DE     8BF3                      MOV ESI,EBX
	$+4      00EE52E0     5B                        POP EBX
	$+5      00EE52E1     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      00EE52E3     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+A      00EE52E6     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      00EE52E9     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     00EE52EC     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+13     00EE52EF     6A 40                     PUSH 0x40
	$+15     00EE52F1     A3 0CD39501               MOV DWORD PTR DS:[0x195D30C],EAX
	$+1A     00EE52F6     8D85 74FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0x8C]
	$+20     00EE52FC     6A 00                     PUSH 0x0
	$+22     00EE52FE     50                        PUSH EAX
	$+23     00EE52FF     E8 FCF44200               CALL PathOfEx.01314800
	$+28     00EE5304     83C4 0C                   ADD ESP,0xC
	$+2B     00EE5307     BF 00000080               MOV EDI,0x80000000
	$+30     00EE530C     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$+34     00EE5310     8BC7                      MOV EAX,EDI
	$+36     00EE5312     33C9                      XOR ECX,ECX
	$+38     00EE5314     53                        PUSH EBX
	$+39     00EE5315     0FA2                      CPUID                                               ; g_h_cpuid4_
	$+3B     00EE5317     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$+3E     00EE531A     5B                        POP EBX
	$+3F     00EE531B     8D75 D8                   LEA ESI,DWORD PTR SS:[EBP-0x28]
	$+42     00EE531E     8906                      MOV DWORD PTR DS:[ESI],EAX
	$+44     00EE5320     8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+47     00EE5323     8B75 EC                   MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+4A     00EE5326     8970 04                   MOV DWORD PTR DS:[EAX+0x4],ESI
	$+4D     00EE5329     8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+50     00EE532C     B9 68D39501               MOV ECX,PathOfEx.0195D368                           ; ASCII "x=U"
	$+55     00EE5331     50                        PUSH EAX
	$+56     00EE5332     8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+59     00EE5335     E8 165EEFFF               CALL PathOfEx.00DDB150
	$+5E     00EE533A     8B0D 0CD39501             MOV ECX,DWORD PTR DS:[0x195D30C]
	$+64     00EE5340     47                        INC EDI
	$+65     00EE5341     3BF9                      CMP EDI,ECX
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("0f??8b??5b89??89????89????89????8b????6a??a3????????8d??????????6a??50e8????????83????bf????????0f??????8b??33??530fa2", 0);
#else
addr_tmp = scan_exe_.ScanAddr("0f??8b??5b89??89????89????89????8b????6a??a3????????8d??????????6a??50e8????????83????bf????????0f??????8b??33??530fa2", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_cpuid3_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

#ifdef VER_GJ3_
	addr_tmp = (pt_pcbyte)addr_tmp + 0x39;
#else
	addr_tmp = (pt_pcbyte)addr_tmp + 0x35;
#endif
	ofs << "pt_cdword	g_h_cpuid4_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-6AA    00B9254F    CC                        INT3
	$-6A9    00B92550    51                        PUSH ECX
	$-6A8    00B92551    8D4C24 04                 LEA ECX,DWORD PTR SS:[ESP+0x4]
	$-6A4    00B92555    2BC8                      SUB ECX,EAX
	$-6A2    00B92557    1BC0                      SBB EAX,EAX
	$-6A0    00B92559    F7D0                      NOT EAX
	$-69E    00B9255B    23C8                      AND ECX,EAX
	$-69C    00B9255D    8BC4                      MOV EAX,ESP
	$-69A    00B9255F    25 00F0FFFF               AND EAX,0xFFFFF000
	$-695    00B92564    3BC8                      CMP ECX,EAX
	$-693    00B92566    F2:72 0B                  REPNE JB SHORT 00B92574
	$-690    00B92569    8BC1                      MOV EAX,ECX
	$-68E    00B9256B    59                        POP ECX
	$-68D    00B9256C    94                        XCHG EAX,ESP
	$-68C    00B9256D    8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$-68A    00B9256F    890424                    MOV DWORD PTR SS:[ESP],EAX
	$-687    00B92572    F2                        REPNE
	$-686    00B92573    C3                        RETN
	$-685    00B92574    2D 00100000               SUB EAX,0x1000
	$-680    00B92579    8500                      TEST DWORD PTR DS:[EAX],EAX
	$-67E    00B9257B  ^ EB E7                     JMP SHORT PathOfEx.00B92564
	$-67C    00B9257D    55                        PUSH EBP
	$-67B    00B9257E    8BEC                      MOV EBP,ESP
	$-679    00B92580    6A 00                     PUSH 0x0
	$-677    00B92582    FF15 6472CA00             CALL DWORD PTR DS:[<&KERNEL32.SetUnhandledExceptionFilter>]   ; kernel32.SetUnhandledExceptionFilter
	$-671    00B92588    FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$-66E    00B9258B    FF15 6872CA00             CALL DWORD PTR DS:[<&KERNEL32.UnhandledExceptionFilter>]      ; kernel32.UnhandledExceptionFilter
	$-668    00B92591    68 090400C0               PUSH 0xC0000409
	$-663    00B92596    FF15 0071CA00             CALL DWORD PTR DS:[<&KERNEL32.GetCurrentProcess>]             ; kernel32.GetCurrentProcess
	$-65D    00B9259C    50                        PUSH EAX
	$-65C    00B9259D    FF15 0871CA00             CALL DWORD PTR DS:[<&KERNEL32.TerminateProcess>]              ; kernel32.TerminateProcess
	$-656    00B925A3    5D                        POP EBP
	$-655    00B925A4    C3                        RETN
	$-654    00B925A5    55                        PUSH EBP
	$-653    00B925A6    8BEC                      MOV EBP,ESP
	$-651    00B925A8    81EC 24030000             SUB ESP,0x324
	$-64B    00B925AE    6A 17                     PUSH 0x17
	$-649    00B925B0    E8 01BF0700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$-644    00B925B5    85C0                      TEST EAX,EAX
	$-642    00B925B7    74 05                     JE SHORT PathOfEx.00B925BE
	$-640    00B925B9    6A 02                     PUSH 0x2
	$-63E    00B925BB    59                        POP ECX
	$-63D    00B925BC    CD 29                     INT 0x29
	$-63B    00B925BE    A3 08C2F100               MOV DWORD PTR DS:[0xF1C208],EAX
	$-636    00B925C3    890D 04C2F100             MOV DWORD PTR DS:[0xF1C204],ECX
	$-630    00B925C9    8915 00C2F100             MOV DWORD PTR DS:[0xF1C200],EDX
	$-62A    00B925CF    891D FCC1F100             MOV DWORD PTR DS:[0xF1C1FC],EBX
	$-624    00B925D5    8935 F8C1F100             MOV DWORD PTR DS:[0xF1C1F8],ESI
	$-61E    00B925DB    893D F4C1F100             MOV DWORD PTR DS:[0xF1C1F4],EDI
	$-618    00B925E1    66:8C15 20C2F100          MOV WORD PTR DS:[0xF1C220],SS
	$-611    00B925E8    66:8C0D 14C2F100          MOV WORD PTR DS:[0xF1C214],CS
	$-60A    00B925EF    66:8C1D F0C1F100          MOV WORD PTR DS:[0xF1C1F0],DS
	$-603    00B925F6    66:8C05 ECC1F100          MOV WORD PTR DS:[0xF1C1EC],ES
	$-5FC    00B925FD    66:8C25 E8C1F100          MOV WORD PTR DS:[0xF1C1E8],FS
	$-5F5    00B92604    66:8C2D E4C1F100          MOV WORD PTR DS:[0xF1C1E4],GS
	$-5EE    00B9260B    9C                        PUSHFD
	$-5ED    00B9260C    8F05 18C2F100             POP DWORD PTR DS:[0xF1C218]
	$-5E7    00B92612    8B45 00                   MOV EAX,DWORD PTR SS:[EBP]
	$-5E4    00B92615    A3 0CC2F100               MOV DWORD PTR DS:[0xF1C20C],EAX
	$-5DF    00B9261A    8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$-5DC    00B9261D    A3 10C2F100               MOV DWORD PTR DS:[0xF1C210],EAX
	$-5D7    00B92622    8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-5D4    00B92625    A3 1CC2F100               MOV DWORD PTR DS:[0xF1C21C],EAX
	$-5CF    00B9262A    8B85 DCFCFFFF             MOV EAX,DWORD PTR SS:[EBP-0x324]
	$-5C9    00B92630    C705 58C1F100 01000100    MOV DWORD PTR DS:[0xF1C158],0x10001
	$-5BF    00B9263A    A1 10C2F100               MOV EAX,DWORD PTR DS:[0xF1C210]
	$-5BA    00B9263F    A3 14C1F100               MOV DWORD PTR DS:[0xF1C114],EAX
	$-5B5    00B92644    C705 08C1F100 090400C0    MOV DWORD PTR DS:[0xF1C108],0xC0000409
	$-5AB    00B9264E    C705 0CC1F100 01000000    MOV DWORD PTR DS:[0xF1C10C],0x1
	$-5A1    00B92658    C705 18C1F100 01000000    MOV DWORD PTR DS:[0xF1C118],0x1
	$-597    00B92662    6A 04                     PUSH 0x4
	$-595    00B92664    58                        POP EAX
	$-594    00B92665    6BC0 00                   IMUL EAX,EAX,0x0
	$-591    00B92668    C780 1CC1F100 02000000    MOV DWORD PTR DS:[EAX+0xF1C11C],0x2
	$-587    00B92672    6A 04                     PUSH 0x4
	$-585    00B92674    58                        POP EAX
	$-584    00B92675    6BC0 00                   IMUL EAX,EAX,0x0
	$-581    00B92678    8B0D 2898EF00             MOV ECX,DWORD PTR DS:[0xEF9828]
	$-57B    00B9267E    894C05 F8                 MOV DWORD PTR SS:[EBP+EAX-0x8],ECX
	$-577    00B92682    6A 04                     PUSH 0x4
	$-575    00B92684    58                        POP EAX
	$-574    00B92685    C1E0 00                   SHL EAX,0x0
	$-571    00B92688    8B0D 2498EF00             MOV ECX,DWORD PTR DS:[0xEF9824]
	$-56B    00B9268E    894C05 F8                 MOV DWORD PTR SS:[EBP+EAX-0x8],ECX
	$-567    00B92692    68 643BD000               PUSH PathOfEx.00D03B64
	$-562    00B92697    E8 E1FEFFFF               CALL PathOfEx.00B9257D
	$-55D    00B9269C    8BE5                      MOV ESP,EBP
	$-55B    00B9269E    5D                        POP EBP
	$-55A    00B9269F    C3                        RETN
	$-559    00B926A0    55                        PUSH EBP
	$-558    00B926A1    8BEC                      MOV EBP,ESP
	$-556    00B926A3    6A 08                     PUSH 0x8
	$-554    00B926A5    E8 02000000               CALL PathOfEx.00B926AC
	$-54F    00B926AA    5D                        POP EBP
	$-54E    00B926AB    C3                        RETN
	$-54D    00B926AC    55                        PUSH EBP
	$-54C    00B926AD    8BEC                      MOV EBP,ESP
	$-54A    00B926AF    81EC 1C030000             SUB ESP,0x31C
	$-544    00B926B5    6A 17                     PUSH 0x17
	$-542    00B926B7    E8 FABD0700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$-53D    00B926BC    85C0                      TEST EAX,EAX
	$-53B    00B926BE    74 05                     JE SHORT PathOfEx.00B926C5
	$-539    00B926C0    8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-536    00B926C3    CD 29                     INT 0x29
	$-534    00B926C5    A3 08C2F100               MOV DWORD PTR DS:[0xF1C208],EAX
	$-52F    00B926CA    890D 04C2F100             MOV DWORD PTR DS:[0xF1C204],ECX
	$-529    00B926D0    8915 00C2F100             MOV DWORD PTR DS:[0xF1C200],EDX
	$-523    00B926D6    891D FCC1F100             MOV DWORD PTR DS:[0xF1C1FC],EBX
	$-51D    00B926DC    8935 F8C1F100             MOV DWORD PTR DS:[0xF1C1F8],ESI
	$-517    00B926E2    893D F4C1F100             MOV DWORD PTR DS:[0xF1C1F4],EDI
	$-511    00B926E8    66:8C15 20C2F100          MOV WORD PTR DS:[0xF1C220],SS
	$-50A    00B926EF    66:8C0D 14C2F100          MOV WORD PTR DS:[0xF1C214],CS
	$-503    00B926F6    66:8C1D F0C1F100          MOV WORD PTR DS:[0xF1C1F0],DS
	$-4FC    00B926FD    66:8C05 ECC1F100          MOV WORD PTR DS:[0xF1C1EC],ES
	$-4F5    00B92704    66:8C25 E8C1F100          MOV WORD PTR DS:[0xF1C1E8],FS
	$-4EE    00B9270B    66:8C2D E4C1F100          MOV WORD PTR DS:[0xF1C1E4],GS
	$-4E7    00B92712    9C                        PUSHFD
	$-4E6    00B92713    8F05 18C2F100             POP DWORD PTR DS:[0xF1C218]
	$-4E0    00B92719    8B45 00                   MOV EAX,DWORD PTR SS:[EBP]
	$-4DD    00B9271C    A3 0CC2F100               MOV DWORD PTR DS:[0xF1C20C],EAX
	$-4D8    00B92721    8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$-4D5    00B92724    A3 10C2F100               MOV DWORD PTR DS:[0xF1C210],EAX
	$-4D0    00B92729    8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-4CD    00B9272C    A3 1CC2F100               MOV DWORD PTR DS:[0xF1C21C],EAX
	$-4C8    00B92731    8B85 E4FCFFFF             MOV EAX,DWORD PTR SS:[EBP-0x31C]
	$-4C2    00B92737    A1 10C2F100               MOV EAX,DWORD PTR DS:[0xF1C210]
	$-4BD    00B9273C    A3 14C1F100               MOV DWORD PTR DS:[0xF1C114],EAX
	$-4B8    00B92741    C705 08C1F100 090400C0    MOV DWORD PTR DS:[0xF1C108],0xC0000409
	$-4AE    00B9274B    C705 0CC1F100 01000000    MOV DWORD PTR DS:[0xF1C10C],0x1
	$-4A4    00B92755    C705 18C1F100 01000000    MOV DWORD PTR DS:[0xF1C118],0x1
	$-49A    00B9275F    6A 04                     PUSH 0x4
	$-498    00B92761    58                        POP EAX
	$-497    00B92762    6BC0 00                   IMUL EAX,EAX,0x0
	$-494    00B92765    8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-491    00B92768    8988 1CC1F100             MOV DWORD PTR DS:[EAX+0xF1C11C],ECX
	$-48B    00B9276E    68 643BD000               PUSH PathOfEx.00D03B64
	$-486    00B92773    E8 05FEFFFF               CALL PathOfEx.00B9257D
	$-481    00B92778    8BE5                      MOV ESP,EBP
	$-47F    00B9277A    5D                        POP EBP
	$-47E    00B9277B    C3                        RETN
	$-47D    00B9277C    55                        PUSH EBP
	$-47C    00B9277D    8BEC                      MOV EBP,ESP
	$-47A    00B9277F    5D                        POP EBP
	$-479    00B92780  ^ E9 60F1FFFF               JMP PathOfEx.00B918E5
	$-474    00B92785    CC                        INT3
	$-473    00B92786    CC                        INT3
	$-472    00B92787    CC                        INT3
	$-471    00B92788    CC                        INT3
	$-470    00B92789    CC                        INT3
	$-46F    00B9278A    CC                        INT3
	$-46E    00B9278B    CC                        INT3
	$-46D    00B9278C    CC                        INT3
	$-46C    00B9278D    CC                        INT3
	$-46B    00B9278E    CC                        INT3
	$-46A    00B9278F    CC                        INT3
	$-469    00B92790    51                        PUSH ECX
	$-468    00B92791    8D4C24 08                 LEA ECX,DWORD PTR SS:[ESP+0x8]
	$-464    00B92795    2BC8                      SUB ECX,EAX
	$-462    00B92797    83E1 0F                   AND ECX,0xF
	$-45F    00B9279A    03C1                      ADD EAX,ECX
	$-45D    00B9279C    1BC9                      SBB ECX,ECX
	$-45B    00B9279E    0BC1                      OR EAX,ECX
	$-459    00B927A0    59                        POP ECX
	$-458    00B927A1  ^ E9 AAFDFFFF               JMP PathOfEx.00B92550
	$-453    00B927A6    51                        PUSH ECX
	$-452    00B927A7    8D4C24 08                 LEA ECX,DWORD PTR SS:[ESP+0x8]
	$-44E    00B927AB    2BC8                      SUB ECX,EAX
	$-44C    00B927AD    83E1 07                   AND ECX,0x7
	$-449    00B927B0    03C1                      ADD EAX,ECX
	$-447    00B927B2    1BC9                      SBB ECX,ECX
	$-445    00B927B4    0BC1                      OR EAX,ECX
	$-443    00B927B6    59                        POP ECX
	$-442    00B927B7  ^ E9 94FDFFFF               JMP PathOfEx.00B92550
	$-43D    00B927BC    55                        PUSH EBP
	$-43C    00B927BD    8BEC                      MOV EBP,ESP
	$-43A    00B927BF    FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$-437    00B927C2    E8 B0F4FFFF               CALL PathOfEx.00B91C77
	$-432    00B927C7    59                        POP ECX
	$-431    00B927C8    5D                        POP EBP
	$-430    00B927C9    C3                        RETN
	$-42F    00B927CA    6A 10                     PUSH 0x10
	$-42D    00B927CC    68 18C7EE00               PUSH PathOfEx.00EEC718
	$-428    00B927D1    E8 7A070000               CALL PathOfEx.00B92F50
	$-423    00B927D6    33DB                      XOR EBX,EBX
	$-421    00B927D8    895D E0                   MOV DWORD PTR SS:[EBP-0x20],EBX
	$-41E    00B927DB    885D E7                   MOV BYTE PTR SS:[EBP-0x19],BL
	$-41B    00B927DE    895D FC                   MOV DWORD PTR SS:[EBP-0x4],EBX
	$-418    00B927E1    3B5D 14                   CMP EBX,DWORD PTR SS:[EBP+0x14]
	$-415    00B927E4    74 20                     JE SHORT PathOfEx.00B92806
	$-413    00B927E6    FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
	$-410    00B927E9    8B4D 18                   MOV ECX,DWORD PTR SS:[EBP+0x18]
	$-40D    00B927EC    E8 44020000               CALL PathOfEx.00B92A35
	$-408    00B927F1    8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-405    00B927F4    FF55 18                   CALL DWORD PTR SS:[EBP+0x18]
	$-402    00B927F7    8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
	$-3FF    00B927FA    0145 08                   ADD DWORD PTR SS:[EBP+0x8],EAX
	$-3FC    00B927FD    0145 0C                   ADD DWORD PTR SS:[EBP+0xC],EAX
	$-3F9    00B92800    43                        INC EBX
	$-3F8    00B92801    895D E0                   MOV DWORD PTR SS:[EBP-0x20],EBX
	$-3F5    00B92804  ^ EB DB                     JMP SHORT PathOfEx.00B927E1
	$-3F3    00B92806    B0 01                     MOV AL,0x1
	$-3F1    00B92808    8845 E7                   MOV BYTE PTR SS:[EBP-0x19],AL
	$-3EE    00B9280B    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$-3E7    00B92812    E8 0E000000               CALL PathOfEx.00B92825
	$-3E2    00B92817    E8 7A070000               CALL PathOfEx.00B92F96
	$-3DD    00B9281C    C2 1800                   RETN 0x18
	$-3DA    00B9281F    8B5D E0                   MOV EBX,DWORD PTR SS:[EBP-0x20]
	$-3D7    00B92822    8A45 E7                   MOV AL,BYTE PTR SS:[EBP-0x19]
	$-3D4    00B92825    84C0                      TEST AL,AL
	$-3D2    00B92827    75 0F                     JNZ SHORT PathOfEx.00B92838
	$-3D0    00B92829    FF75 1C                   PUSH DWORD PTR SS:[EBP+0x1C]
	$-3CD    00B9282C    53                        PUSH EBX
	$-3CC    00B9282D    FF75 10                   PUSH DWORD PTR SS:[EBP+0x10]
	$-3C9    00B92830    FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$-3C6    00B92833    E8 D2F3FFFF               CALL PathOfEx.00B91C0A
	$-3C1    00B92838    C3                        RETN
	$-3C0    00B92839    68 28C4F100               PUSH PathOfEx.00F1C428
	$-3BB    00B9283E    FF15 5C72CA00             CALL DWORD PTR DS:[<&KERNEL32.InitializeSListHead>]           ; ntdll_12.RtlInitializeSListHead
	$-3B5    00B92844    C3                        RETN
	$-3B4    00B92845    6A 0C                     PUSH 0xC
	$-3B2    00B92847    68 38C7EE00               PUSH PathOfEx.00EEC738
	$-3AD    00B9284C    E8 FF060000               CALL PathOfEx.00B92F50
	$-3A8    00B92851    837D 0C 02                CMP DWORD PTR SS:[EBP+0xC],0x2
	$-3A4    00B92855    75 2F                     JNZ SHORT PathOfEx.00B92886
	$-3A2    00B92857    8365 FC 00                AND DWORD PTR SS:[EBP-0x4],0x0
	$-39E    00B9285B    BE 58B2CA00               MOV ESI,PathOfEx.00CAB258
	$-399    00B92860    8975 E4                   MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-396    00B92863    81FE 5CB2CA00             CMP ESI,PathOfEx.00CAB25C
	$-390    00B92869    74 14                     JE SHORT PathOfEx.00B9287F
	$-38E    00B9286B    8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$-38C    00B9286D    85FF                      TEST EDI,EDI
	$-38A    00B9286F    74 09                     JE SHORT PathOfEx.00B9287A
	$-388    00B92871    8BCF                      MOV ECX,EDI
	$-386    00B92873    E8 BD010000               CALL PathOfEx.00B92A35
	$-381    00B92878    FFD7                      CALL EDI
	$-37F    00B9287A    83C6 04                   ADD ESI,0x4
	$-37C    00B9287D  ^ EB E1                     JMP SHORT PathOfEx.00B92860
	$-37A    00B9287F    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$-373    00B92886    E8 0B070000               CALL PathOfEx.00B92F96
	$-36E    00B9288B    C2 0C00                   RETN 0xC
	$-36B    00B9288E    8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-368    00B92891    8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$-366    00B92893    FF30                      PUSH DWORD PTR DS:[EAX]
	$-364    00B92895    E8 0D000000               CALL PathOfEx.00B928A7
	$-35F    00B9289A    83C4 04                   ADD ESP,0x4
	$-35C    00B9289D    C3                        RETN
	$-35B    00B9289E    8B65 E8                   MOV ESP,DWORD PTR SS:[EBP-0x18]
	$-358    00B928A1    E8 C7E10200               CALL PathOfEx.00BC0A6D
	$-353    00B928A6    CC                        INT3
	$-352    00B928A7    55                        PUSH EBP
	$-351    00B928A8    8BEC                      MOV EBP,ESP
	$-34F    00B928AA    33C0                      XOR EAX,EAX
	$-34D    00B928AC    817D 08 63736DE0          CMP DWORD PTR SS:[EBP+0x8],0xE06D7363
	$-346    00B928B3    0F94C0                    SETE AL
	$-343    00B928B6    5D                        POP EBP
	$-342    00B928B7    C3                        RETN
	$-341    00B928B8    55                        PUSH EBP
	$-340    00B928B9    8BEC                      MOV EBP,ESP
	$-33E    00B928BB    51                        PUSH ECX
	$-33D    00B928BC    51                        PUSH ECX
	$-33C    00B928BD    837D 0C 03                CMP DWORD PTR SS:[EBP+0xC],0x3
	$-338    00B928C1    74 06                     JE SHORT PathOfEx.00B928C9
	$-336    00B928C3    837D 0C 00                CMP DWORD PTR SS:[EBP+0xC],0x0
	$-332    00B928C7    75 67                     JNZ SHORT PathOfEx.00B92930
	$-330    00B928C9    8B0D 24C4F100             MOV ECX,DWORD PTR DS:[0xF1C424]
	$-32A    00B928CF    64:A1 2C000000            MOV EAX,DWORD PTR FS:[0x2C]
	$-324    00B928D5    53                        PUSH EBX
	$-323    00B928D6    56                        PUSH ESI
	$-322    00B928D7    8B1C88                    MOV EBX,DWORD PTR DS:[EAX+ECX*4]
	$-31F    00B928DA    895D F8                   MOV DWORD PTR SS:[EBP-0x8],EBX
	$-31C    00B928DD    8BB3 30000000             MOV ESI,DWORD PTR DS:[EBX+0x30]
	$-316    00B928E3    85F6                      TEST ESI,ESI
	$-314    00B928E5    74 47                     JE SHORT PathOfEx.00B9292E
	$-312    00B928E7    57                        PUSH EDI
	$-311    00B928E8    8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$-30F    00B928EA    83EF 01                   SUB EDI,0x1
	$-30C    00B928ED    78 24                     JS SHORT PathOfEx.00B92913
	$-30A    00B928EF    8D5F 02                   LEA EBX,DWORD PTR DS:[EDI+0x2]
	$-307    00B928F2    8D1C9E                    LEA EBX,DWORD PTR DS:[ESI+EBX*4]
	$-304    00B928F5    8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$-302    00B928F7    8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$-2FF    00B928FA    85C0                      TEST EAX,EAX
	$-2FD    00B928FC    74 0A                     JE SHORT PathOfEx.00B92908
	$-2FB    00B928FE    8BC8                      MOV ECX,EAX
	$-2F9    00B92900    E8 30010000               CALL PathOfEx.00B92A35
	$-2F4    00B92905    FF55 FC                   CALL DWORD PTR SS:[EBP-0x4]
	$-2F1    00B92908    83EB 04                   SUB EBX,0x4
	$-2EE    00B9290B    83EF 01                   SUB EDI,0x1
	$-2EB    00B9290E  ^ 79 E5                     JNS SHORT PathOfEx.00B928F5
	$-2E9    00B92910    8B5D F8                   MOV EBX,DWORD PTR SS:[EBP-0x8]
	$-2E6    00B92913    8B7E 04                   MOV EDI,DWORD PTR DS:[ESI+0x4]
	$-2E3    00B92916    85FF                      TEST EDI,EDI
	$-2E1    00B92918    74 07                     JE SHORT PathOfEx.00B92921
	$-2DF    00B9291A    56                        PUSH ESI
	$-2DE    00B9291B    E8 C65F0300               CALL PathOfEx.00BC88E6
	$-2D9    00B92920    59                        POP ECX
	$-2D8    00B92921    89BB 30000000             MOV DWORD PTR DS:[EBX+0x30],EDI
	$-2D2    00B92927    8BF7                      MOV ESI,EDI
	$-2D0    00B92929    85FF                      TEST EDI,EDI
	$-2CE    00B9292B  ^ 75 BB                     JNZ SHORT PathOfEx.00B928E8
	$-2CC    00B9292D    5F                        POP EDI
	$-2CB    00B9292E    5E                        POP ESI
	$-2CA    00B9292F    5B                        POP EBX
	$-2C9    00B92930    8BE5                      MOV ESP,EBP
	$-2C7    00B92932    5D                        POP EBP
	$-2C6    00B92933    C2 0C00                   RETN 0xC
	$-2C3    00B92936    55                        PUSH EBP
	$-2C2    00B92937    8BEC                      MOV EBP,ESP
	$-2C0    00B92939    64:A1 2C000000            MOV EAX,DWORD PTR FS:[0x2C]
	$-2BA    00B9293F    8B0D 24C4F100             MOV ECX,DWORD PTR DS:[0xF1C424]
	$-2B4    00B92945    56                        PUSH ESI
	$-2B3    00B92946    57                        PUSH EDI
	$-2B2    00B92947    8B3488                    MOV ESI,DWORD PTR DS:[EAX+ECX*4]
	$-2AF    00B9294A    8B86 30000000             MOV EAX,DWORD PTR DS:[ESI+0x30]
	$-2A9    00B92950    85C0                      TEST EAX,EAX
	$-2A7    00B92952    75 11                     JNZ SHORT PathOfEx.00B92965
	$-2A5    00B92954    8D86 38000000             LEA EAX,DWORD PTR DS:[ESI+0x38]
	$-29F    00B9295A    8320 00                   AND DWORD PTR DS:[EAX],0x0
	$-29C    00B9295D    8986 30000000             MOV DWORD PTR DS:[ESI+0x30],EAX
	$-296    00B92963    EB 35                     JMP SHORT PathOfEx.00B9299A
	$-294    00B92965    8338 1E                   CMP DWORD PTR DS:[EAX],0x1E
	$-291    00B92968    75 30                     JNZ SHORT PathOfEx.00B9299A
	$-28F    00B9296A    68 80000000               PUSH 0x80
	$-28A    00B9296F    E8 AC5F0300               CALL PathOfEx.00BC8920
	$-285    00B92974    6A 00                     PUSH 0x0
	$-283    00B92976    8BF8                      MOV EDI,EAX
	$-281    00B92978    E8 695F0300               CALL PathOfEx.00BC88E6
	$-27C    00B9297D    59                        POP ECX
	$-27B    00B9297E    59                        POP ECX
	$-27A    00B9297F    85FF                      TEST EDI,EDI
	$-278    00B92981    75 05                     JNZ SHORT PathOfEx.00B92988
	$-276    00B92983    83C8 FF                   OR EAX,0xFFFFFFFF
	$-273    00B92986    EB 25                     JMP SHORT PathOfEx.00B929AD
	$-271    00B92988    8B86 30000000             MOV EAX,DWORD PTR DS:[ESI+0x30]
	$-26B    00B9298E    8327 00                   AND DWORD PTR DS:[EDI],0x0
	$-268    00B92991    8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
	$-265    00B92994    89BE 30000000             MOV DWORD PTR DS:[ESI+0x30],EDI
	$-25F    00B9299A    8B96 30000000             MOV EDX,DWORD PTR DS:[ESI+0x30]
	$-259    00B929A0    8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-256    00B929A3    8B0A                      MOV ECX,DWORD PTR DS:[EDX]
	$-254    00B929A5    89448A 08                 MOV DWORD PTR DS:[EDX+ECX*4+0x8],EAX
	$-250    00B929A9    FF02                      INC DWORD PTR DS:[EDX]
	$-24E    00B929AB    33C0                      XOR EAX,EAX
	$-24C    00B929AD    5F                        POP EDI
	$-24B    00B929AE    5E                        POP ESI
	$-24A    00B929AF    5D                        POP EBP
	$-249    00B929B0    C3                        RETN
	$-248    00B929B1    CC                        INT3
	$-247    00B929B2    CC                        INT3
	$-246    00B929B3    CC                        INT3
	$-245    00B929B4    CC                        INT3
	$-244    00B929B5    CC                        INT3
	$-243    00B929B6    CC                        INT3
	$-242    00B929B7    CC                        INT3
	$-241    00B929B8    CC                        INT3
	$-240    00B929B9    CC                        INT3
	$-23F    00B929BA    CC                        INT3
	$-23E    00B929BB    CC                        INT3
	$-23D    00B929BC    CC                        INT3
	$-23C    00B929BD    CC                        INT3
	$-23B    00B929BE    CC                        INT3
	$-23A    00B929BF    CC                        INT3
	$-239    00B929C0    53                        PUSH EBX
	$-238    00B929C1    8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-234    00B929C5    0BC0                      OR EAX,EAX
	$-232    00B929C7    75 18                     JNZ SHORT PathOfEx.00B929E1
	$-230    00B929C9    8B4C24 10                 MOV ECX,DWORD PTR SS:[ESP+0x10]
	$-22C    00B929CD    8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-228    00B929D1    33D2                      XOR EDX,EDX
	$-226    00B929D3    F7F1                      DIV ECX
	$-224    00B929D5    8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-220    00B929D9    F7F1                      DIV ECX
	$-21E    00B929DB    8BC2                      MOV EAX,EDX
	$-21C    00B929DD    33D2                      XOR EDX,EDX
	$-21A    00B929DF    EB 50                     JMP SHORT PathOfEx.00B92A31
	$-218    00B929E1    8BC8                      MOV ECX,EAX
	$-216    00B929E3    8B5C24 10                 MOV EBX,DWORD PTR SS:[ESP+0x10]
	$-212    00B929E7    8B5424 0C                 MOV EDX,DWORD PTR SS:[ESP+0xC]
	$-20E    00B929EB    8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-20A    00B929EF    D1E9                      SHR ECX,1
	$-208    00B929F1    D1DB                      RCR EBX,1
	$-206    00B929F3    D1EA                      SHR EDX,1
	$-204    00B929F5    D1D8                      RCR EAX,1
	$-202    00B929F7    0BC9                      OR ECX,ECX
	$-200    00B929F9  ^ 75 F4                     JNZ SHORT PathOfEx.00B929EF
	$-1FE    00B929FB    F7F3                      DIV EBX
	$-1FC    00B929FD    8BC8                      MOV ECX,EAX
	$-1FA    00B929FF    F76424 14                 MUL DWORD PTR SS:[ESP+0x14]
	$-1F6    00B92A03    91                        XCHG EAX,ECX
	$-1F5    00B92A04    F76424 10                 MUL DWORD PTR SS:[ESP+0x10]
	$-1F1    00B92A08    03D1                      ADD EDX,ECX
	$-1EF    00B92A0A    72 0E                     JB SHORT PathOfEx.00B92A1A
	$-1ED    00B92A0C    3B5424 0C                 CMP EDX,DWORD PTR SS:[ESP+0xC]
	$-1E9    00B92A10    77 08                     JA SHORT PathOfEx.00B92A1A
	$-1E7    00B92A12    72 0E                     JB SHORT PathOfEx.00B92A22
	$-1E5    00B92A14    3B4424 08                 CMP EAX,DWORD PTR SS:[ESP+0x8]
	$-1E1    00B92A18    76 08                     JBE SHORT PathOfEx.00B92A22
	$-1DF    00B92A1A    2B4424 10                 SUB EAX,DWORD PTR SS:[ESP+0x10]
	$-1DB    00B92A1E    1B5424 14                 SBB EDX,DWORD PTR SS:[ESP+0x14]
	$-1D7    00B92A22    2B4424 08                 SUB EAX,DWORD PTR SS:[ESP+0x8]
	$-1D3    00B92A26    1B5424 0C                 SBB EDX,DWORD PTR SS:[ESP+0xC]
	$-1CF    00B92A2A    F7DA                      NEG EDX
	$-1CD    00B92A2C    F7D8                      NEG EAX
	$-1CB    00B92A2E    83DA 00                   SBB EDX,0x0
	$-1C8    00B92A31    5B                        POP EBX
	$-1C7    00B92A32    C2 1000                   RETN 0x10
	$-1C4    00B92A35  ^ FF25 3C76CA00             JMP DWORD PTR DS:[0xCA763C]                                   ; PathOfEx.003472F0
	$-1BE    00B92A3B    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1BB    00B92A3E    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1B4    00B92A45    59                        POP ECX
	$-1B3    00B92A46    5F                        POP EDI
	$-1B2    00B92A47    5F                        POP EDI
	$-1B1    00B92A48    5E                        POP ESI
	$-1B0    00B92A49    5B                        POP EBX
	$-1AF    00B92A4A    8BE5                      MOV ESP,EBP
	$-1AD    00B92A4C    5D                        POP EBP
	$-1AC    00B92A4D    51                        PUSH ECX
	$-1AB    00B92A4E    F2                        REPNE
	$-1AA    00B92A4F    C3                        RETN
	$-1A9    00B92A50    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-1A6    00B92A53    33CD                      XOR ECX,EBP
	$-1A4    00B92A55    F2:E8 0CF5FFFF            REPNE CALL 00B91F67
	$-19E    00B92A5B    F2:E9 DAFFFFFF            REPNE JMP 00B92A3B
	$-198    00B92A61    8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-195    00B92A64    33CD                      XOR ECX,EBP
	$-193    00B92A66    F2:E8 FBF4FFFF            REPNE CALL 00B91F67
	$-18D    00B92A6C    F2:E9 C9FFFFFF            REPNE JMP 00B92A3B
	$-187    00B92A72    50                        PUSH EAX
	$-186    00B92A73    64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-17F    00B92A7A    8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-17B    00B92A7E    2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-177    00B92A82    53                        PUSH EBX
	$-176    00B92A83    56                        PUSH ESI
	$-175    00B92A84    57                        PUSH EDI
	$-174    00B92A85    8928                      MOV DWORD PTR DS:[EAX],EBP
	$-172    00B92A87    8BE8                      MOV EBP,EAX
	$-170    00B92A89    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$-16B    00B92A8E    33C5                      XOR EAX,EBP
	$-169    00B92A90    50                        PUSH EAX
	$-168    00B92A91    FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-165    00B92A94    C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-15E    00B92A9B    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-15B    00B92A9E    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-155    00B92AA4    F2                        REPNE
	$-154    00B92AA5    C3                        RETN
	$-153    00B92AA6    50                        PUSH EAX
	$-152    00B92AA7    64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-14B    00B92AAE    8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-147    00B92AB2    2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-143    00B92AB6    53                        PUSH EBX
	$-142    00B92AB7    56                        PUSH ESI
	$-141    00B92AB8    57                        PUSH EDI
	$-140    00B92AB9    8928                      MOV DWORD PTR DS:[EAX],EBP
	$-13E    00B92ABB    8BE8                      MOV EBP,EAX
	$-13C    00B92ABD    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$-137    00B92AC2    33C5                      XOR EAX,EBP
	$-135    00B92AC4    50                        PUSH EAX
	$-134    00B92AC5    8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$-131    00B92AC8    FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-12E    00B92ACB    C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-127    00B92AD2    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-124    00B92AD5    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-11E    00B92ADB    F2                        REPNE
	$-11D    00B92ADC    C3                        RETN
	$-11C    00B92ADD    50                        PUSH EAX
	$-11B    00B92ADE    64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-114    00B92AE5    8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-110    00B92AE9    2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-10C    00B92AED    53                        PUSH EBX
	$-10B    00B92AEE    56                        PUSH ESI
	$-10A    00B92AEF    57                        PUSH EDI
	$-109    00B92AF0    8928                      MOV DWORD PTR DS:[EAX],EBP
	$-107    00B92AF2    8BE8                      MOV EBP,EAX
	$-105    00B92AF4    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$-100    00B92AF9    33C5                      XOR EAX,EBP
	$-FE     00B92AFB    50                        PUSH EAX
	$-FD     00B92AFC    8965 F0                   MOV DWORD PTR SS:[EBP-0x10],ESP
	$-FA     00B92AFF    FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-F7     00B92B02    C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-F0     00B92B09    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-ED     00B92B0C    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-E7     00B92B12    F2                        REPNE
	$-E6     00B92B13    C3                        RETN
	$-E5     00B92B14    50                        PUSH EAX
	$-E4     00B92B15    64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-DD     00B92B1C    8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-D9     00B92B20    2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-D5     00B92B24    53                        PUSH EBX
	$-D4     00B92B25    56                        PUSH ESI
	$-D3     00B92B26    57                        PUSH EDI
	$-D2     00B92B27    8928                      MOV DWORD PTR DS:[EAX],EBP
	$-D0     00B92B29    8BE8                      MOV EBP,EAX
	$-CE     00B92B2B    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$-C9     00B92B30    33C5                      XOR EAX,EBP
	$-C7     00B92B32    50                        PUSH EAX
	$-C6     00B92B33    8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$-C3     00B92B36    8965 F0                   MOV DWORD PTR SS:[EBP-0x10],ESP
	$-C0     00B92B39    FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-BD     00B92B3C    C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-B6     00B92B43    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-B3     00B92B46    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-AD     00B92B4C    F2                        REPNE
	$-AC     00B92B4D    C3                        RETN
	$-AB     00B92B4E    CC                        INT3
	$-AA     00B92B4F    CC                        INT3
	$-A9     00B92B50    53                        PUSH EBX
	$-A8     00B92B51    56                        PUSH ESI
	$-A7     00B92B52    8B4424 18                 MOV EAX,DWORD PTR SS:[ESP+0x18]
	$-A3     00B92B56    0BC0                      OR EAX,EAX
	$-A1     00B92B58    75 18                     JNZ SHORT PathOfEx.00B92B72
	$-9F     00B92B5A    8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
	$-9B     00B92B5E    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$-97     00B92B62    33D2                      XOR EDX,EDX
	$-95     00B92B64    F7F1                      DIV ECX
	$-93     00B92B66    8BD8                      MOV EBX,EAX
	$-91     00B92B68    8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-8D     00B92B6C    F7F1                      DIV ECX
	$-8B     00B92B6E    8BD3                      MOV EDX,EBX
	$-89     00B92B70    EB 41                     JMP SHORT PathOfEx.00B92BB3
	$-87     00B92B72    8BC8                      MOV ECX,EAX
	$-85     00B92B74    8B5C24 14                 MOV EBX,DWORD PTR SS:[ESP+0x14]
	$-81     00B92B78    8B5424 10                 MOV EDX,DWORD PTR SS:[ESP+0x10]
	$-7D     00B92B7C    8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-79     00B92B80    D1E9                      SHR ECX,1
	$-77     00B92B82    D1DB                      RCR EBX,1
	$-75     00B92B84    D1EA                      SHR EDX,1
	$-73     00B92B86    D1D8                      RCR EAX,1
	$-71     00B92B88    0BC9                      OR ECX,ECX
	$-6F     00B92B8A  ^ 75 F4                     JNZ SHORT PathOfEx.00B92B80
	$-6D     00B92B8C    F7F3                      DIV EBX
	$-6B     00B92B8E    8BF0                      MOV ESI,EAX
	$-69     00B92B90    F76424 18                 MUL DWORD PTR SS:[ESP+0x18]
	$-65     00B92B94    8BC8                      MOV ECX,EAX
	$-63     00B92B96    8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-5F     00B92B9A    F7E6                      MUL ESI
	$-5D     00B92B9C    03D1                      ADD EDX,ECX
	$-5B     00B92B9E    72 0E                     JB SHORT PathOfEx.00B92BAE
	$-59     00B92BA0    3B5424 10                 CMP EDX,DWORD PTR SS:[ESP+0x10]
	$-55     00B92BA4    77 08                     JA SHORT PathOfEx.00B92BAE
	$-53     00B92BA6    72 07                     JB SHORT PathOfEx.00B92BAF
	$-51     00B92BA8    3B4424 0C                 CMP EAX,DWORD PTR SS:[ESP+0xC]
	$-4D     00B92BAC    76 01                     JBE SHORT PathOfEx.00B92BAF
	$-4B     00B92BAE    4E                        DEC ESI
	$-4A     00B92BAF    33D2                      XOR EDX,EDX
	$-48     00B92BB1    8BC6                      MOV EAX,ESI
	$-46     00B92BB3    5E                        POP ESI
	$-45     00B92BB4    5B                        POP EBX
	$-44     00B92BB5    C2 1000                   RETN 0x10
	$-41     00B92BB8    55                        PUSH EBP                                                      ; pass
	$-40     00B92BB9    8BEC                      MOV EBP,ESP
	$-3E     00B92BBB    8325 30C4F100 00          AND DWORD PTR DS:[0xF1C430],0x0
	$-37     00B92BC2    83EC 28                   SUB ESP,0x28
	$-34     00B92BC5    53                        PUSH EBX
	$-33     00B92BC6    33DB                      XOR EBX,EBX
	$-31     00B92BC8    43                        INC EBX
	$-30     00B92BC9    091D 3098EF00             OR DWORD PTR DS:[0xEF9830],EBX
	$-2A     00B92BCF    6A 0A                     PUSH 0xA
	$-28     00B92BD1    E8 E0B80700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$-23     00B92BD6    85C0                      TEST EAX,EAX
	$-21     00B92BD8    0F84 6D010000             JE PathOfEx.00B92D4B
	$-1B     00B92BDE    8365 F0 00                AND DWORD PTR SS:[EBP-0x10],0x0
	$-17     00B92BE2    33C0                      XOR EAX,EAX
	$-15     00B92BE4    830D 3098EF00 02          OR DWORD PTR DS:[0xEF9830],0x2
	$-E      00B92BEB    33C9                      XOR ECX,ECX
	$-C      00B92BED    56                        PUSH ESI
	$-B      00B92BEE    57                        PUSH EDI
	$-A      00B92BEF    891D 30C4F100             MOV DWORD PTR DS:[0xF1C430],EBX
	$-4      00B92BF5    8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-1      00B92BF8    53                        PUSH EBX
	$ ==>    00B92BF9    0FA2                      CPUID                                                         ; g_h_cpuid5_
	$+2      00B92BFB    8BF3                      MOV ESI,EBX
	$+4      00B92BFD    5B                        POP EBX
	$+5      00B92BFE    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      00B92C00    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+A      00B92C03    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      00B92C06    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     00B92C09    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+13     00B92C0C    8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+16     00B92C0F    8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+19     00B92C12    81F1 696E6549             XOR ECX,0x49656E69
	$+1F     00B92C18    8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+22     00B92C1B    35 6E74656C               XOR EAX,0x6C65746E
	$+27     00B92C20    0BC8                      OR ECX,EAX
	$+29     00B92C22    8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+2C     00B92C25    6A 01                     PUSH 0x1
	$+2E     00B92C27    35 47656E75               XOR EAX,0x756E6547
	$+33     00B92C2C    0BC8                      OR ECX,EAX
	$+35     00B92C2E    58                        POP EAX
	$+36     00B92C2F    6A 00                     PUSH 0x0
	$+38     00B92C31    59                        POP ECX
	$+39     00B92C32    53                        PUSH EBX
	$+3A     00B92C33    0FA2                      CPUID                                                         ; g_h_cpuid6_
	$+3C     00B92C35    8BF3                      MOV ESI,EBX
	$+3E     00B92C37    5B                        POP EBX
	$+3F     00B92C38    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+41     00B92C3A    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+44     00B92C3D    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+47     00B92C40    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+4A     00B92C43    75 43                     JNZ SHORT PathOfEx.00B92C88
	$+4C     00B92C45    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+4F     00B92C48    25 F03FFF0F               AND EAX,0xFFF3FF0
	$+54     00B92C4D    3D C0060100               CMP EAX,0x106C0
	$+59     00B92C52    74 23                     JE SHORT PathOfEx.00B92C77
	$+5B     00B92C54    3D 60060200               CMP EAX,0x20660                                               ; UNICODE "4_none_41e6975e2bd6f2b2"
	$+60     00B92C59    74 1C                     JE SHORT PathOfEx.00B92C77
	$+62     00B92C5B    3D 70060200               CMP EAX,0x20670                                               ; UNICODE "1e6975e2bd6f2b2"
	$+67     00B92C60    74 15                     JE SHORT PathOfEx.00B92C77
	$+69     00B92C62    3D 50060300               CMP EAX,0x30650
	$+6E     00B92C67    74 0E                     JE SHORT PathOfEx.00B92C77
	$+70     00B92C69    3D 60060300               CMP EAX,0x30660
	$+75     00B92C6E    74 07                     JE SHORT PathOfEx.00B92C77
	$+77     00B92C70    3D 70060300               CMP EAX,0x30670
	$+7C     00B92C75    75 11                     JNZ SHORT PathOfEx.00B92C88
	$+7E     00B92C77    8B3D 34C4F100             MOV EDI,DWORD PTR DS:[0xF1C434]
	$+84     00B92C7D    83CF 01                   OR EDI,0x1
	$+87     00B92C80    893D 34C4F100             MOV DWORD PTR DS:[0xF1C434],EDI
	$+8D     00B92C86    EB 06                     JMP SHORT PathOfEx.00B92C8E
	$+8F     00B92C88    8B3D 34C4F100             MOV EDI,DWORD PTR DS:[0xF1C434]
	$+95     00B92C8E    837D F8 07                CMP DWORD PTR SS:[EBP-0x8],0x7
	$+99     00B92C92    8B45 E4                   MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+9C     00B92C95    8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+9F     00B92C98    8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+A2     00B92C9B    8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+A5     00B92C9E    8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+A8     00B92CA1    7C 32                     JL SHORT PathOfEx.00B92CD5
	$+AA     00B92CA3    6A 07                     PUSH 0x7
	$+AC     00B92CA5    58                        POP EAX
	$+AD     00B92CA6    33C9                      XOR ECX,ECX
	$+AF     00B92CA8    53                        PUSH EBX
	$+B0     00B92CA9    0FA2                      CPUID                                                         ; g_h_cpuid7_
	$+B2     00B92CAB    8BF3                      MOV ESI,EBX
	$+B4     00B92CAD    5B                        POP EBX
	$+B5     00B92CAE    8D5D D8                   LEA EBX,DWORD PTR SS:[EBP-0x28]
	$+B8     00B92CB1    8903                      MOV DWORD PTR DS:[EBX],EAX
	$+BA     00B92CB3    8973 04                   MOV DWORD PTR DS:[EBX+0x4],ESI
	$+BD     00B92CB6    894B 08                   MOV DWORD PTR DS:[EBX+0x8],ECX
	$+C0     00B92CB9    8953 0C                   MOV DWORD PTR DS:[EBX+0xC],EDX
	$+C3     00B92CBC    8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+C6     00B92CBF    A9 00020000               TEST EAX,0x200
	$+CB     00B92CC4    8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+CE     00B92CC7    8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+D1     00B92CCA    74 09                     JE SHORT PathOfEx.00B92CD5
	$+D3     00B92CCC    83CF 02                   OR EDI,0x2
	$+D6     00B92CCF    893D 34C4F100             MOV DWORD PTR DS:[0xF1C434],EDI
	$+DC     00B92CD5    5F                        POP EDI
	$+DD     00B92CD6    5E                        POP ESI
	$+DE     00B92CD7    A9 00001000               TEST EAX,0x100000
	$+E3     00B92CDC    74 6D                     JE SHORT PathOfEx.00B92D4B
	$+E5     00B92CDE    830D 3098EF00 04          OR DWORD PTR DS:[0xEF9830],0x4
	$+EC     00B92CE5    C705 30C4F100 02000000    MOV DWORD PTR DS:[0xF1C430],0x2
	$+F6     00B92CEF    A9 00000008               TEST EAX,0x8000000
	$+FB     00B92CF4    74 55                     JE SHORT PathOfEx.00B92D4B
	$+FD     00B92CF6    A9 00000010               TEST EAX,0x10000000
	$+102    00B92CFB    74 4E                     JE SHORT PathOfEx.00B92D4B
	$+104    00B92CFD    33C9                      XOR ECX,ECX
	$+106    00B92CFF    0F01D0                    XGETBV
	$+109    00B92D02    8945 F4                   MOV DWORD PTR SS:[EBP-0xC],EAX
	$+10C    00B92D05    8955 F8                   MOV DWORD PTR SS:[EBP-0x8],EDX
	$+10F    00B92D08    8B45 F4                   MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+112    00B92D0B    8B4D F8                   MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+115    00B92D0E    83E0 06                   AND EAX,0x6
	$+118    00B92D11    33C9                      XOR ECX,ECX
	$+11A    00B92D13    83F8 06                   CMP EAX,0x6
	$+11D    00B92D16    75 33                     JNZ SHORT PathOfEx.00B92D4B
	$+11F    00B92D18    85C9                      TEST ECX,ECX
	$+121    00B92D1A    75 2F                     JNZ SHORT PathOfEx.00B92D4B
	$+123    00B92D1C    A1 3098EF00               MOV EAX,DWORD PTR DS:[0xEF9830]
	$+128    00B92D21    83C8 08                   OR EAX,0x8
	$+12B    00B92D24    C705 30C4F100 03000000    MOV DWORD PTR DS:[0xF1C430],0x3
	$+135    00B92D2E    F645 F0 20                TEST BYTE PTR SS:[EBP-0x10],0x20
	$+139    00B92D32    A3 3098EF00               MOV DWORD PTR DS:[0xEF9830],EAX
	$+13E    00B92D37    74 12                     JE SHORT PathOfEx.00B92D4B
	$+140    00B92D39    83C8 20                   OR EAX,0x20
	$+143    00B92D3C    C705 30C4F100 05000000    MOV DWORD PTR DS:[0xF1C430],0x5
	$+14D    00B92D46    A3 3098EF00               MOV DWORD PTR DS:[0xEF9830],EAX
	$+152    00B92D4B    33C0                      XOR EAX,EAX
	$+154    00B92D4D    5B                        POP EBX
	$+155    00B92D4E    8BE5                      MOV ESP,EBP
	$+157    00B92D50    5D                        POP EBP
	$+158    00B92D51    C3                        RETN
	$+159    00B92D52    33C0                      XOR EAX,EAX
	$+15B    00B92D54    40                        INC EAX
	$+15C    00B92D55    C3                        RETN
	$+15D    00B92D56    33C0                      XOR EAX,EAX
	$+15F    00B92D58    3905 30760901             CMP DWORD PTR DS:[0x1097630],EAX
	$+165    00B92D5E    0F95C0                    SETNE AL
	$+168    00B92D61    C3                        RETN
	$+169    00B92D62    55                        PUSH EBP
	$+16A    00B92D63    8BEC                      MOV EBP,ESP
	$+16C    00B92D65    81EC 24030000             SUB ESP,0x324
	$+172    00B92D6B    53                        PUSH EBX
	$+173    00B92D6C    56                        PUSH ESI
	$+174    00B92D6D    6A 17                     PUSH 0x17
	$+176    00B92D6F    E8 42B70700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$+17B    00B92D74    85C0                      TEST EAX,EAX
	$+17D    00B92D76    74 05                     JE SHORT PathOfEx.00B92D7D
	$+17F    00B92D78    8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+182    00B92D7B    CD 29                     INT 0x29
	$+184    00B92D7D    33F6                      XOR ESI,ESI
	$+186    00B92D7F    8D85 DCFCFFFF             LEA EAX,DWORD PTR SS:[EBP-0x324]
	$+18C    00B92D85    68 CC020000               PUSH 0x2CC
	$+191    00B92D8A    56                        PUSH ESI
	$+192    00B92D8B    50                        PUSH EAX
	$+193    00B92D8C    8935 38C4F100             MOV DWORD PTR DS:[0xF1C438],ESI
	$+199    00B92D92    E8 897C0100               CALL PathOfEx.00BAAA20
	$+19E    00B92D97    83C4 0C                   ADD ESP,0xC
	$+1A1    00B92D9A    8985 8CFDFFFF             MOV DWORD PTR SS:[EBP-0x274],EAX
	$+1A7    00B92DA0    898D 88FDFFFF             MOV DWORD PTR SS:[EBP-0x278],ECX
	$+1AD    00B92DA6    8995 84FDFFFF             MOV DWORD PTR SS:[EBP-0x27C],EDX
	$+1B3    00B92DAC    899D 80FDFFFF             MOV DWORD PTR SS:[EBP-0x280],EBX
	$+1B9    00B92DB2    89B5 7CFDFFFF             MOV DWORD PTR SS:[EBP-0x284],ESI
	$+1BF    00B92DB8    89BD 78FDFFFF             MOV DWORD PTR SS:[EBP-0x288],EDI
	$+1C5    00B92DBE    66:8C95 A4FDFFFF          MOV WORD PTR SS:[EBP-0x25C],SS
	$+1CC    00B92DC5    66:8C8D 98FDFFFF          MOV WORD PTR SS:[EBP-0x268],CS
	$+1D3    00B92DCC    66:8C9D 74FDFFFF          MOV WORD PTR SS:[EBP-0x28C],DS
	$+1DA    00B92DD3    66:8C85 70FDFFFF          MOV WORD PTR SS:[EBP-0x290],ES
	$+1E1    00B92DDA    66:8CA5 6CFDFFFF          MOV WORD PTR SS:[EBP-0x294],FS
	$+1E8    00B92DE1    66:8CAD 68FDFFFF          MOV WORD PTR SS:[EBP-0x298],GS
	$+1EF    00B92DE8    9C                        PUSHFD
	$+1F0    00B92DE9    8F85 9CFDFFFF             POP DWORD PTR SS:[EBP-0x264]
	$+1F6    00B92DEF    8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+1F9    00B92DF2    8985 94FDFFFF             MOV DWORD PTR SS:[EBP-0x26C],EAX
	$+1FF    00B92DF8    8D45 04                   LEA EAX,DWORD PTR SS:[EBP+0x4]
	$+202    00B92DFB    8985 A0FDFFFF             MOV DWORD PTR SS:[EBP-0x260],EAX
	$+208    00B92E01    C785 DCFCFFFF 01000100    MOV DWORD PTR SS:[EBP-0x324],0x10001
	$+212    00B92E0B    8B40 FC                   MOV EAX,DWORD PTR DS:[EAX-0x4]
	$+215    00B92E0E    6A 50                     PUSH 0x50
	$+217    00B92E10    8985 90FDFFFF             MOV DWORD PTR SS:[EBP-0x270],EAX
	$+21D    00B92E16    8D45 A8                   LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+220    00B92E19    56                        PUSH ESI
	$+221    00B92E1A    50                        PUSH EAX
	$+222    00B92E1B    E8 007C0100               CALL PathOfEx.00BAAA20
	$+227    00B92E20    8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+22A    00B92E23    83C4 0C                   ADD ESP,0xC
	$+22D    00B92E26    C745 A8 15000040          MOV DWORD PTR SS:[EBP-0x58],0x40000015
	$+234    00B92E2D    C745 AC 01000000          MOV DWORD PTR SS:[EBP-0x54],0x1
	$+23B    00B92E34    8945 B4                   MOV DWORD PTR SS:[EBP-0x4C],EAX
	$+23E    00B92E37    FF15 5872CA00             CALL DWORD PTR DS:[<&KERNEL32.IsDebuggerPresent>]             ; kernel32.IsDebuggerPresent
	$+244    00B92E3D    56                        PUSH ESI
	$+245    00B92E3E    8D58 FF                   LEA EBX,DWORD PTR DS:[EAX-0x1]
	$+248    00B92E41    F7DB                      NEG EBX
	$+24A    00B92E43    8D45 A8                   LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+24D    00B92E46    8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+250    00B92E49    8D85 DCFCFFFF             LEA EAX,DWORD PTR SS:[EBP-0x324]
	$+256    00B92E4F    1ADB                      SBB BL,BL
	$+258    00B92E51    8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+25B    00B92E54    FEC3                      INC BL
	$+25D    00B92E56    FF15 6472CA00             CALL DWORD PTR DS:[<&KERNEL32.SetUnhandledExceptionFilter>]   ; kernel32.SetUnhandledExceptionFilter
	$+263    00B92E5C    8D45 F8                   LEA EAX,DWORD PTR SS:[EBP-0x8]
	$+266    00B92E5F    50                        PUSH EAX
	$+267    00B92E60    FF15 6872CA00             CALL DWORD PTR DS:[<&KERNEL32.UnhandledExceptionFilter>]      ; kernel32.UnhandledExceptionFilter
	$+26D    00B92E66    85C0                      TEST EAX,EAX
	$+26F    00B92E68    75 0D                     JNZ SHORT PathOfEx.00B92E77
	$+271    00B92E6A    0FB6C3                    MOVZX EAX,BL
	$+274    00B92E6D    F7D8                      NEG EAX
	$+276    00B92E6F    1BC0                      SBB EAX,EAX
	$+278    00B92E71    2105 38C4F100             AND DWORD PTR DS:[0xF1C438],EAX
	$+27E    00B92E77    5E                        POP ESI
	$+27F    00B92E78    5B                        POP EBX
	$+280    00B92E79    8BE5                      MOV ESP,EBP
	$+282    00B92E7B    5D                        POP EBP
	$+283    00B92E7C    C3                        RETN
	$+284    00B92E7D    55                        PUSH EBP
	$+285    00B92E7E    8BEC                      MOV EBP,ESP
	$+287    00B92E80    83EC 44                   SUB ESP,0x44
	$+28A    00B92E83    6A 44                     PUSH 0x44
	$+28C    00B92E85    8D45 BC                   LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+28F    00B92E88    6A 00                     PUSH 0x0
	$+291    00B92E8A    50                        PUSH EAX
	$+292    00B92E8B    E8 907B0100               CALL PathOfEx.00BAAA20
	$+297    00B92E90    83C4 0C                   ADD ESP,0xC
	$+29A    00B92E93    8D45 BC                   LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+29D    00B92E96    50                        PUSH EAX
	$+29E    00B92E97    FF15 5472CA00             CALL DWORD PTR DS:[<&KERNEL32.GetStartupInfoW>]               ; kernel32.GetStartupInfoW
	$+2A4    00B92E9D    F645 E8 01                TEST BYTE PTR SS:[EBP-0x18],0x1
	$+2A8    00B92EA1    74 06                     JE SHORT PathOfEx.00B92EA9
	$+2AA    00B92EA3    0FB745 EC                 MOVZX EAX,WORD PTR SS:[EBP-0x14]
	$+2AE    00B92EA7    EB 03                     JMP SHORT PathOfEx.00B92EAC
	$+2B0    00B92EA9    6A 0A                     PUSH 0xA
	$+2B2    00B92EAB    58                        POP EAX
	$+2B3    00B92EAC    8BE5                      MOV ESP,EBP
	$+2B5    00B92EAE    5D                        POP EBP
	$+2B6    00B92EAF    C3                        RETN
	$+2B7    00B92EB0    6A 00                     PUSH 0x0
	$+2B9    00B92EB2    FF15 F070CA00             CALL DWORD PTR DS:[<&KERNEL32.GetModuleHandleW>]              ; kernel32.GetModuleHandleW
	$+2BF    00B92EB8    8BC8                      MOV ECX,EAX
	$+2C1    00B92EBA    85C9                      TEST ECX,ECX
	$+2C3    00B92EBC    75 03                     JNZ SHORT PathOfEx.00B92EC1
	$+2C5    00B92EBE    32C0                      XOR AL,AL
	$+2C7    00B92EC0    C3                        RETN
	$+2C8    00B92EC1    B8 4D5A0000               MOV EAX,0x5A4D
	$+2CD    00B92EC6    66:3901                   CMP WORD PTR DS:[ECX],AX
	$+2D0    00B92EC9  ^ 75 F3                     JNZ SHORT PathOfEx.00B92EBE
	$+2D2    00B92ECB    8B41 3C                   MOV EAX,DWORD PTR DS:[ECX+0x3C]
	$+2D5    00B92ECE    03C1                      ADD EAX,ECX
	$+2D7    00B92ED0    8138 50450000             CMP DWORD PTR DS:[EAX],0x4550
	$+2DD    00B92ED6  ^ 75 E6                     JNZ SHORT PathOfEx.00B92EBE
	$+2DF    00B92ED8    B9 0B010000               MOV ECX,0x10B
	$+2E4    00B92EDD    66:3948 18                CMP WORD PTR DS:[EAX+0x18],CX
	$+2E8    00B92EE1  ^ 75 DB                     JNZ SHORT PathOfEx.00B92EBE
	$+2EA    00B92EE3    8378 74 0E                CMP DWORD PTR DS:[EAX+0x74],0xE
	$+2EE    00B92EE7  ^ 76 D5                     JBE SHORT PathOfEx.00B92EBE
	$+2F0    00B92EE9    83B8 E8000000 00          CMP DWORD PTR DS:[EAX+0xE8],0x0
	$+2F7    00B92EF0    0F95C0                    SETNE AL
	$+2FA    00B92EF3    C3                        RETN
	$+2FB    00B92EF4    68 002FB900               PUSH PathOfEx.00B92F00
	$+300    00B92EF9    FF15 6472CA00             CALL DWORD PTR DS:[<&KERNEL32.SetUnhandledExceptionFilter>]   ; kernel32.SetUnhandledExceptionFilter
	$+306    00B92EFF    C3                        RETN
	$+307    00B92F00    55                        PUSH EBP
	$+308    00B92F01    8BEC                      MOV EBP,ESP
	$+30A    00B92F03    8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+30D    00B92F06    8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+30F    00B92F08    8138 63736DE0             CMP DWORD PTR DS:[EAX],0xE06D7363
	$+315    00B92F0E    75 25                     JNZ SHORT PathOfEx.00B92F35
	$+317    00B92F10    8378 10 03                CMP DWORD PTR DS:[EAX+0x10],0x3
	$+31B    00B92F14    75 1F                     JNZ SHORT PathOfEx.00B92F35
	$+31D    00B92F16    8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+320    00B92F19    3D 20059319               CMP EAX,0x19930520
	$+325    00B92F1E    74 1B                     JE SHORT PathOfEx.00B92F3B
	$+327    00B92F20    3D 21059319               CMP EAX,0x19930521
	$+32C    00B92F25    74 14                     JE SHORT PathOfEx.00B92F3B
	$+32E    00B92F27    3D 22059319               CMP EAX,0x19930522
	$+333    00B92F2C    74 0D                     JE SHORT PathOfEx.00B92F3B
	$+335    00B92F2E    3D 00409901               CMP EAX,0x1994000
	$+33A    00B92F33    74 06                     JE SHORT PathOfEx.00B92F3B
	$+33C    00B92F35    33C0                      XOR EAX,EAX
	$+33E    00B92F37    5D                        POP EBP
	$+33F    00B92F38    C2 0400                   RETN 0x4
	$+342    00B92F3B    E8 2DDB0200               CALL PathOfEx.00BC0A6D
	$+347    00B92F40    CC                        INT3
	$+348    00B92F41    8325 38C4F100 00          AND DWORD PTR DS:[0xF1C438],0x0
	$+34F    00B92F48    C3                        RETN
	$+350    00B92F49    CC                        INT3
	$+351    00B92F4A    CC                        INT3
	$+352    00B92F4B    CC                        INT3
	$+353    00B92F4C    CC                        INT3
	$+354    00B92F4D    CC                        INT3
	$+355    00B92F4E    CC                        INT3
	$+356    00B92F4F    CC                        INT3
	$+357    00B92F50    68 40D2BA00               PUSH PathOfEx.00BAD240
	$+35C    00B92F55    64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$+363    00B92F5C    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+367    00B92F60    896C24 10                 MOV DWORD PTR SS:[ESP+0x10],EBP
	$+36B    00B92F64    8D6C24 10                 LEA EBP,DWORD PTR SS:[ESP+0x10]
	$+36F    00B92F68    2BE0                      SUB ESP,EAX
	$+371    00B92F6A    53                        PUSH EBX
	$+372    00B92F6B    56                        PUSH ESI
	$+373    00B92F6C    57                        PUSH EDI
	$+374    00B92F6D    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$+379    00B92F72    3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+37C    00B92F75    33C5                      XOR EAX,EBP
	$+37E    00B92F77    50                        PUSH EAX
	$+37F    00B92F78    8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
	$+382    00B92F7B    FF75 F8                   PUSH DWORD PTR SS:[EBP-0x8]
	$+385    00B92F7E    8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+388    00B92F81    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$+38F    00B92F88    8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+392    00B92F8B    8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+395    00B92F8E    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+39B    00B92F94    F2                        REPNE
	$+39C    00B92F95    C3                        RETN
	$+39D    00B92F96    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+3A0    00B92F99    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+3A7    00B92FA0    59                        POP ECX
	$+3A8    00B92FA1    5F                        POP EDI
	$+3A9    00B92FA2    5F                        POP EDI
	$+3AA    00B92FA3    5E                        POP ESI
	$+3AB    00B92FA4    5B                        POP EBX
	$+3AC    00B92FA5    8BE5                      MOV ESP,EBP
	$+3AE    00B92FA7    5D                        POP EBP
	$+3AF    00B92FA8    51                        PUSH ECX
	$+3B0    00B92FA9    F2                        REPNE
	$+3B1    00B92FAA    C3                        RETN
	$+3B2    00B92FAB    55                        PUSH EBP
	$+3B3    00B92FAC    8BEC                      MOV EBP,ESP
	$+3B5    00B92FAE    56                        PUSH ESI
	$+3B6    00B92FAF    FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$+3B9    00B92FB2    8BF1                      MOV ESI,ECX
	$+3BB    00B92FB4    E8 D7317CFF               CALL PathOfEx.00356190
	$+3C0    00B92FB9    C706 783BD000             MOV DWORD PTR DS:[ESI],PathOfEx.00D03B78
	$+3C6    00B92FBF    8BC6                      MOV EAX,ESI
	$+3C8    00B92FC1    5E                        POP ESI
	$+3C9    00B92FC2    5D                        POP EBP
	$+3CA    00B92FC3    C2 0400                   RETN 0x4
	$+3CD    00B92FC6    8361 04 00                AND DWORD PTR DS:[ECX+0x4],0x0
	$+3D1    00B92FCA    8BC1                      MOV EAX,ECX
	$+3D3    00B92FCC    8361 08 00                AND DWORD PTR DS:[ECX+0x8],0x0
	$+3D7    00B92FD0    C741 04 803BD000          MOV DWORD PTR DS:[ECX+0x4],PathOfEx.00D03B80
	$+3DE    00B92FD7    C701 783BD000             MOV DWORD PTR DS:[ECX],PathOfEx.00D03B78
	$+3E4    00B92FDD    C3                        RETN
	$+3E5    00B92FDE    55                        PUSH EBP
	$+3E6    00B92FDF    8BEC                      MOV EBP,ESP
	$+3E8    00B92FE1    83EC 0C                   SUB ESP,0xC
	$+3EB    00B92FE4    8D4D F4                   LEA ECX,DWORD PTR SS:[EBP-0xC]
	$+3EE    00B92FE7    E8 F4C3FEFF               CALL PathOfEx.00B7F3E0
	$+3F3    00B92FEC    68 28B1EE00               PUSH PathOfEx.00EEB128
	$+3F8    00B92FF1    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+3FB    00B92FF4    50                        PUSH EAX
	$+3FC    00B92FF5    E8 A9840100               CALL PathOfEx.00BAB4A3
	$+401    00B92FFA    CC                        INT3
	$+402    00B92FFB    55                        PUSH EBP
	$+403    00B92FFC    8BEC                      MOV EBP,ESP
	$+405    00B92FFE    83EC 0C                   SUB ESP,0xC
	$+408    00B93001    8D4D F4                   LEA ECX,DWORD PTR SS:[EBP-0xC]
	$+40B    00B93004    E8 BDFFFFFF               CALL PathOfEx.00B92FC6
	$+410    00B93009    68 54C7EE00               PUSH PathOfEx.00EEC754
	$+415    00B9300E    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+418    00B93011    50                        PUSH EAX
	$+419    00B93012    E8 8C840100               CALL PathOfEx.00BAB4A3
	$+41E    00B93017    CC                        INT3
	$+41F    00B93018    55                        PUSH EBP
	$+420    00B93019    8BEC                      MOV EBP,ESP
	$+422    00B9301B    83EC 14                   SUB ESP,0x14
	$+425    00B9301E    8365 F4 00                AND DWORD PTR SS:[EBP-0xC],0x0
	$+429    00B93022    8365 F8 00                AND DWORD PTR SS:[EBP-0x8],0x0
	$+42D    00B93026    A1 2898EF00               MOV EAX,DWORD PTR DS:[0xEF9828]
	$+432    00B9302B    56                        PUSH ESI
	$+433    00B9302C    57                        PUSH EDI
	$+434    00B9302D    BF 4EE640BB               MOV EDI,0xBB40E64E
	$+439    00B93032    BE 0000FFFF               MOV ESI,0xFFFF0000
	$+43E    00B93037    3BC7                      CMP EAX,EDI
	$+440    00B93039    74 0D                     JE SHORT PathOfEx.00B93048
	$+442    00B9303B    85C6                      TEST ESI,EAX
	$+444    00B9303D    74 09                     JE SHORT PathOfEx.00B93048
	$+446    00B9303F    F7D0                      NOT EAX
	$+448    00B93041    A3 2498EF00               MOV DWORD PTR DS:[0xEF9824],EAX
	$+44D    00B93046    EB 66                     JMP SHORT PathOfEx.00B930AE
	$+44F    00B93048    8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+452    00B9304B    50                        PUSH EAX
	$+453    00B9304C    FF15 9072CA00             CALL DWORD PTR DS:[<&KERNEL32.GetSystemTimeAsFileTime>]       ; kernel32.GetSystemTimeAsFileTime
	$+459    00B93052    8B45 F8                   MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+45C    00B93055    3345 F4                   XOR EAX,DWORD PTR SS:[EBP-0xC]
	$+45F    00B93058    8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+462    00B9305B    FF15 FC70CA00             CALL DWORD PTR DS:[<&KERNEL32.GetCurrentThreadId>]            ; kernel32.GetCurrentThreadId
	$+468    00B93061    3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+46B    00B93064    FF15 B070CA00             CALL DWORD PTR DS:[<&KERNEL32.GetCurrentProcessId>]           ; kernel32.GetCurrentProcessId
	$+471    00B9306A    3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+474    00B9306D    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+477    00B93070    50                        PUSH EAX
	$+478    00B93071    FF15 EC72CA00             CALL DWORD PTR DS:[<&KERNEL32.QueryPerformanceCounter>]       ; kernel32.QueryPerformanceCounter
	$+47E    00B93077    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+481    00B9307A    8D45 FC                   LEA EAX,DWORD PTR SS:[EBP-0x4]
	$+484    00B9307D    334D EC                   XOR ECX,DWORD PTR SS:[EBP-0x14]
	$+487    00B93080    334D FC                   XOR ECX,DWORD PTR SS:[EBP-0x4]
	$+48A    00B93083    33C8                      XOR ECX,EAX
	$+48C    00B93085    3BCF                      CMP ECX,EDI
	$+48E    00B93087    75 07                     JNZ SHORT PathOfEx.00B93090
	$+490    00B93089    B9 4FE640BB               MOV ECX,0xBB40E64F
	$+495    00B9308E    EB 10                     JMP SHORT PathOfEx.00B930A0
	$+497    00B93090    85CE                      TEST ESI,ECX
	$+499    00B93092    75 0C                     JNZ SHORT PathOfEx.00B930A0
	$+49B    00B93094    8BC1                      MOV EAX,ECX
	$+49D    00B93096    0D 11470000               OR EAX,0x4711
	$+4A2    00B9309B    C1E0 10                   SHL EAX,0x10
	$+4A5    00B9309E    0BC8                      OR ECX,EAX
	$+4A7    00B930A0    890D 2898EF00             MOV DWORD PTR DS:[0xEF9828],ECX
	$+4AD    00B930A6    F7D1                      NOT ECX
	$+4AF    00B930A8    890D 2498EF00             MOV DWORD PTR DS:[0xEF9824],ECX
	$+4B5    00B930AE    5F                        POP EDI
	$+4B6    00B930AF    5E                        POP ESI
	$+4B7    00B930B0    8BE5                      MOV ESP,EBP
	$+4B9    00B930B2    5D                        POP EBP
	$+4BA    00B930B3    C3                        RETN
	$+4BB    00B930B4    B8 00400000               MOV EAX,0x4000
	$+4C0    00B930B9    C3                        RETN
	$+4C1    00B930BA    68 00000300               PUSH 0x30000
	$+4C6    00B930BF    68 00000100               PUSH 0x10000
	$+4CB    00B930C4    6A 00                     PUSH 0x0
	$+4CD    00B930C6    E8 35890300               CALL PathOfEx.00BCBA00
	$+4D2    00B930CB    83C4 0C                   ADD ESP,0xC
	$+4D5    00B930CE    85C0                      TEST EAX,EAX
	$+4D7    00B930D0    75 01                     JNZ SHORT PathOfEx.00B930D3
	$+4D9    00B930D2    C3                        RETN
	$+4DA    00B930D3    6A 07                     PUSH 0x7
	$+4DC    00B930D5    E8 88FCFFFF               CALL PathOfEx.00B92D62
	$+4E1    00B930DA    CC                        INT3
	$+4E2    00B930DB    E8 00267DFF               CALL PathOfEx.003656E0
	$+4E7    00B930E0    8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+4EA    00B930E3    8308 04                   OR DWORD PTR DS:[EAX],0x4
	$+4ED    00B930E6    8948 04                   MOV DWORD PTR DS:[EAX+0x4],ECX
	$+4F0    00B930E9    E8 E2DEA3FF               CALL PathOfEx.005D0FD0
	$+4F5    00B930EE    8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+4F8    00B930F1    8308 02                   OR DWORD PTR DS:[EAX],0x2
	$+4FB    00B930F4    8948 04                   MOV DWORD PTR DS:[EAX+0x4],ECX
	$+4FE    00B930F7    C3                        RETN
	$+4FF    00B930F8    33C0                      XOR EAX,EAX
	$+501    00B930FA    3905 4098EF00             CMP DWORD PTR DS:[0xEF9840],EAX
	$+507    00B93100    0F94C0                    SETE AL
	$+50A    00B93103    C3                        RETN
	$+50B    00B93104    B8 6C3BD000               MOV EAX,PathOfEx.00D03B6C
	$+510    00B93109    C3                        RETN
	$+511    00B9310A    B8 703BD000               MOV EAX,PathOfEx.00D03B70
	$+516    00B9310F    C3                        RETN
	$+517    00B93110    53                        PUSH EBX
	$+518    00B93111    56                        PUSH ESI
	$+519    00B93112    BE 7403E700               MOV ESI,PathOfEx.00E70374
	$+51E    00B93117    BB 7403E700               MOV EBX,PathOfEx.00E70374
	$+523    00B9311C    3BF3                      CMP ESI,EBX
	$+525    00B9311E    73 18                     JNB SHORT PathOfEx.00B93138
	$+527    00B93120    57                        PUSH EDI
	$+528    00B93121    8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$+52A    00B93123    85FF                      TEST EDI,EDI
	$+52C    00B93125    74 09                     JE SHORT PathOfEx.00B93130
	$+52E    00B93127    8BCF                      MOV ECX,EDI
	$+530    00B93129    E8 07F9FFFF               CALL PathOfEx.00B92A35
	$+535    00B9312E    FFD7                      CALL EDI
	$+537    00B93130    83C6 04                   ADD ESI,0x4
	$+53A    00B93133    3BF3                      CMP ESI,EBX
	$+53C    00B93135  ^ 72 EA                     JB SHORT PathOfEx.00B93121
	$+53E    00B93137    5F                        POP EDI
	$+53F    00B93138    5E                        POP ESI
	$+540    00B93139    5B                        POP EBX
	$+541    00B9313A    C3                        RETN
	$+542    00B9313B    53                        PUSH EBX
	$+543    00B9313C    56                        PUSH ESI
	$+544    00B9313D    BE 7C03E700               MOV ESI,PathOfEx.00E7037C
	$+549    00B93142    BB 7C03E700               MOV EBX,PathOfEx.00E7037C
	$+54E    00B93147    3BF3                      CMP ESI,EBX
	$+550    00B93149    73 18                     JNB SHORT PathOfEx.00B93163
	$+552    00B9314B    57                        PUSH EDI
	$+553    00B9314C    8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$+555    00B9314E    85FF                      TEST EDI,EDI
	$+557    00B93150    74 09                     JE SHORT PathOfEx.00B9315B
	$+559    00B93152    8BCF                      MOV ECX,EDI
	$+55B    00B93154    E8 DCF8FFFF               CALL PathOfEx.00B92A35
	$+560    00B93159    FFD7                      CALL EDI
	$+562    00B9315B    83C6 04                   ADD ESI,0x4
	$+565    00B9315E    3BF3                      CMP ESI,EBX
	$+567    00B93160  ^ 72 EA                     JB SHORT PathOfEx.00B9314C
	$+569    00B93162    5F                        POP EDI
	$+56A    00B93163    5E                        POP ESI
	$+56B    00B93164    5B                        POP EBX
	$+56C    00B93165    C3                        RETN
	$+56D    00B93166    B8 A889F100               MOV EAX,PathOfEx.00F189A8
	$+572    00B9316B    E9 00000000               JMP PathOfEx.00B93170
	$+577    00B93170    51                        PUSH ECX
	$+578    00B93171    52                        PUSH EDX
	$+579    00B93172    50                        PUSH EAX
	$+57A    00B93173    68 F434EF00               PUSH PathOfEx.00EF34F4
	$+57F    00B93178    E8 37B60700               CALL PathOfEx.00C0E7B4
	$+584    00B9317D    5A                        POP EDX
	$+585    00B9317E    59                        POP ECX
	$+586    00B9317F    FFE0                      JMP EAX
	$+588    00B93181    B8 AC89F100               MOV EAX,PathOfEx.00F189AC
	$+58D    00B93186  ^ E9 E5FFFFFF               JMP PathOfEx.00B93170
	$+592    00B9318B    B8 B089F100               MOV EAX,PathOfEx.00F189B0
	$+597    00B93190  ^ E9 DBFFFFFF               JMP PathOfEx.00B93170
	$+59C    00B93195    B8 B489F100               MOV EAX,PathOfEx.00F189B4
	$+5A1    00B9319A  ^ E9 D1FFFFFF               JMP PathOfEx.00B93170
	$+5A6    00B9319F    B8 B889F100               MOV EAX,PathOfEx.00F189B8
	$+5AB    00B931A4  ^ E9 C7FFFFFF               JMP PathOfEx.00B93170
	$+5B0    00B931A9    B8 BC89F100               MOV EAX,PathOfEx.00F189BC
	$+5B5    00B931AE  ^ E9 BDFFFFFF               JMP PathOfEx.00B93170
	$+5BA    00B931B3    B8 C089F100               MOV EAX,PathOfEx.00F189C0
	$+5BF    00B931B8  ^ E9 B3FFFFFF               JMP PathOfEx.00B93170
	$+5C4    00B931BD    B8 C489F100               MOV EAX,PathOfEx.00F189C4
	$+5C9    00B931C2  ^ E9 A9FFFFFF               JMP PathOfEx.00B93170
	$+5CE    00B931C7    B8 C889F100               MOV EAX,PathOfEx.00F189C8
	$+5D3    00B931CC  ^ E9 9FFFFFFF               JMP PathOfEx.00B93170
	$+5D8    00B931D1    B8 CC89F100               MOV EAX,PathOfEx.00F189CC
	$+5DD    00B931D6  ^ E9 95FFFFFF               JMP PathOfEx.00B93170
	$+5E2    00B931DB    B8 D089F100               MOV EAX,PathOfEx.00F189D0
	$+5E7    00B931E0  ^ E9 8BFFFFFF               JMP PathOfEx.00B93170
	$+5EC    00B931E5    B8 D489F100               MOV EAX,PathOfEx.00F189D4
	$+5F1    00B931EA  ^ E9 81FFFFFF               JMP PathOfEx.00B93170
	$+5F6    00B931EF    B8 D889F100               MOV EAX,PathOfEx.00F189D8
	$+5FB    00B931F4  ^ E9 77FFFFFF               JMP PathOfEx.00B93170
	$+600    00B931F9    B8 DC89F100               MOV EAX,PathOfEx.00F189DC
	$+605    00B931FE  ^ E9 6DFFFFFF               JMP PathOfEx.00B93170
	$+60A    00B93203    B8 E089F100               MOV EAX,PathOfEx.00F189E0
	$+60F    00B93208  ^ E9 63FFFFFF               JMP PathOfEx.00B93170
	$+614    00B9320D    B8 E489F100               MOV EAX,PathOfEx.00F189E4
	$+619    00B93212  ^ E9 59FFFFFF               JMP PathOfEx.00B93170
	$+61E    00B93217    B8 E889F100               MOV EAX,PathOfEx.00F189E8
	$+623    00B9321C  ^ E9 4FFFFFFF               JMP PathOfEx.00B93170
	$+628    00B93221    B8 EC89F100               MOV EAX,PathOfEx.00F189EC
	$+62D    00B93226  ^ E9 45FFFFFF               JMP PathOfEx.00B93170
	$+632    00B9322B    B8 F089F100               MOV EAX,PathOfEx.00F189F0
	$+637    00B93230  ^ E9 3BFFFFFF               JMP PathOfEx.00B93170
	$+63C    00B93235    B8 F489F100               MOV EAX,PathOfEx.00F189F4
	$+641    00B9323A  ^ E9 31FFFFFF               JMP PathOfEx.00B93170
	$+646    00B9323F    CC                        INT3
	$+647    00B93240    55                        PUSH EBP
	$+648    00B93241    8BEC                      MOV EBP,ESP
	$+64A    00B93243    56                        PUSH ESI
	$+64B    00B93244    8B35 4498EF00             MOV ESI,DWORD PTR DS:[0xEF9844]                               ; PathOfEx.00B93520
	$+651    00B9324A    8BCE                      MOV ECX,ESI
	$+653    00B9324C    6A 00                     PUSH 0x0
	$+655    00B9324E    FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$+658    00B93251    E8 DFF7FFFF               CALL PathOfEx.00B92A35
	$+65D    00B93256    FFD6                      CALL ESI
	$+65F    00B93258    5E                        POP ESI
	$+660    00B93259    5D                        POP EBP
	$+661    00B9325A    C2 0400                   RETN 0x4
	$+664    00B9325D    CC                        INT3
	*/
	//gj3
	/*
	$-23A    012FC82F     CC                        INT3
	$-239    012FC830     53                        PUSH EBX
	$-238    012FC831     8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-234    012FC835     0BC0                      OR EAX,EAX
	$-232    012FC837     75 18                     JNZ SHORT PathOfEx.012FC851
	$-230    012FC839     8B4C24 10                 MOV ECX,DWORD PTR SS:[ESP+0x10]
	$-22C    012FC83D     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-228    012FC841     33D2                      XOR EDX,EDX
	$-226    012FC843     F7F1                      DIV ECX
	$-224    012FC845     8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-220    012FC849     F7F1                      DIV ECX
	$-21E    012FC84B     8BC2                      MOV EAX,EDX
	$-21C    012FC84D     33D2                      XOR EDX,EDX
	$-21A    012FC84F     EB 50                     JMP SHORT PathOfEx.012FC8A1
	$-218    012FC851     8BC8                      MOV ECX,EAX
	$-216    012FC853     8B5C24 10                 MOV EBX,DWORD PTR SS:[ESP+0x10]
	$-212    012FC857     8B5424 0C                 MOV EDX,DWORD PTR SS:[ESP+0xC]
	$-20E    012FC85B     8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-20A    012FC85F     D1E9                      SHR ECX,1
	$-208    012FC861     D1DB                      RCR EBX,1
	$-206    012FC863     D1EA                      SHR EDX,1
	$-204    012FC865     D1D8                      RCR EAX,1
	$-202    012FC867     0BC9                      OR ECX,ECX
	$-200    012FC869   ^ 75 F4                     JNZ SHORT PathOfEx.012FC85F
	$-1FE    012FC86B     F7F3                      DIV EBX
	$-1FC    012FC86D     8BC8                      MOV ECX,EAX
	$-1FA    012FC86F     F76424 14                 MUL DWORD PTR SS:[ESP+0x14]
	$-1F6    012FC873     91                        XCHG EAX,ECX
	$-1F5    012FC874     F76424 10                 MUL DWORD PTR SS:[ESP+0x10]
	$-1F1    012FC878     03D1                      ADD EDX,ECX
	$-1EF    012FC87A     72 0E                     JB SHORT PathOfEx.012FC88A
	$-1ED    012FC87C     3B5424 0C                 CMP EDX,DWORD PTR SS:[ESP+0xC]
	$-1E9    012FC880     77 08                     JA SHORT PathOfEx.012FC88A
	$-1E7    012FC882     72 0E                     JB SHORT PathOfEx.012FC892
	$-1E5    012FC884     3B4424 08                 CMP EAX,DWORD PTR SS:[ESP+0x8]
	$-1E1    012FC888     76 08                     JBE SHORT PathOfEx.012FC892
	$-1DF    012FC88A     2B4424 10                 SUB EAX,DWORD PTR SS:[ESP+0x10]
	$-1DB    012FC88E     1B5424 14                 SBB EDX,DWORD PTR SS:[ESP+0x14]
	$-1D7    012FC892     2B4424 08                 SUB EAX,DWORD PTR SS:[ESP+0x8]
	$-1D3    012FC896     1B5424 0C                 SBB EDX,DWORD PTR SS:[ESP+0xC]
	$-1CF    012FC89A     F7DA                      NEG EDX
	$-1CD    012FC89C     F7D8                      NEG EAX
	$-1CB    012FC89E     83DA 00                   SBB EDX,0x0
	$-1C8    012FC8A1     5B                        POP EBX
	$-1C7    012FC8A2     C2 1000                   RETN 0x10
	$-1C4    012FC8A5   ^ FF25 7CB64001             JMP DWORD PTR DS:[0x140B67C]                                  ; PathOfEx.00943BB0
	$-1BE    012FC8AB     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1BB    012FC8AE     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1B4    012FC8B5     59                        POP ECX
	$-1B3    012FC8B6     5F                        POP EDI
	$-1B2    012FC8B7     5F                        POP EDI
	$-1B1    012FC8B8     5E                        POP ESI
	$-1B0    012FC8B9     5B                        POP EBX
	$-1AF    012FC8BA     8BE5                      MOV ESP,EBP
	$-1AD    012FC8BC     5D                        POP EBP
	$-1AC    012FC8BD     51                        PUSH ECX
	$-1AB    012FC8BE     F2                        REPNE
	$-1AA    012FC8BF     C3                        RETN
	$-1A9    012FC8C0     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-1A6    012FC8C3     33CD                      XOR ECX,EBP
	$-1A4    012FC8C5     F2:E8 13F5FFFF            REPNE CALL 012FBDDE
	$-19E    012FC8CB     F2:E9 DAFFFFFF            REPNE JMP 012FC8AB
	$-198    012FC8D1     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-195    012FC8D4     33CD                      XOR ECX,EBP
	$-193    012FC8D6     F2:E8 02F5FFFF            REPNE CALL 012FBDDE
	$-18D    012FC8DC     F2:E9 C9FFFFFF            REPNE JMP 012FC8AB
	$-187    012FC8E2     50                        PUSH EAX
	$-186    012FC8E3     64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-17F    012FC8EA     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-17B    012FC8EE     2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-177    012FC8F2     53                        PUSH EBX
	$-176    012FC8F3     56                        PUSH ESI
	$-175    012FC8F4     57                        PUSH EDI
	$-174    012FC8F5     8928                      MOV DWORD PTR DS:[EAX],EBP
	$-172    012FC8F7     8BE8                      MOV EBP,EAX
	$-170    012FC8F9     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$-16B    012FC8FE     33C5                      XOR EAX,EBP
	$-169    012FC900     50                        PUSH EAX
	$-168    012FC901     FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-165    012FC904     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-15E    012FC90B     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-15B    012FC90E     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-155    012FC914     F2                        REPNE
	$-154    012FC915     C3                        RETN
	$-153    012FC916     50                        PUSH EAX
	$-152    012FC917     64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-14B    012FC91E     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-147    012FC922     2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-143    012FC926     53                        PUSH EBX
	$-142    012FC927     56                        PUSH ESI
	$-141    012FC928     57                        PUSH EDI
	$-140    012FC929     8928                      MOV DWORD PTR DS:[EAX],EBP
	$-13E    012FC92B     8BE8                      MOV EBP,EAX
	$-13C    012FC92D     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$-137    012FC932     33C5                      XOR EAX,EBP
	$-135    012FC934     50                        PUSH EAX
	$-134    012FC935     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$-131    012FC938     FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-12E    012FC93B     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-127    012FC942     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-124    012FC945     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-11E    012FC94B     F2                        REPNE
	$-11D    012FC94C     C3                        RETN
	$-11C    012FC94D     50                        PUSH EAX
	$-11B    012FC94E     64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-114    012FC955     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-110    012FC959     2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-10C    012FC95D     53                        PUSH EBX
	$-10B    012FC95E     56                        PUSH ESI
	$-10A    012FC95F     57                        PUSH EDI
	$-109    012FC960     8928                      MOV DWORD PTR DS:[EAX],EBP
	$-107    012FC962     8BE8                      MOV EBP,EAX
	$-105    012FC964     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$-100    012FC969     33C5                      XOR EAX,EBP
	$-FE     012FC96B     50                        PUSH EAX
	$-FD     012FC96C     8965 F0                   MOV DWORD PTR SS:[EBP-0x10],ESP
	$-FA     012FC96F     FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-F7     012FC972     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-F0     012FC979     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-ED     012FC97C     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-E7     012FC982     F2                        REPNE
	$-E6     012FC983     C3                        RETN
	$-E5     012FC984     50                        PUSH EAX
	$-E4     012FC985     64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$-DD     012FC98C     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-D9     012FC990     2B6424 0C                 SUB ESP,DWORD PTR SS:[ESP+0xC]
	$-D5     012FC994     53                        PUSH EBX
	$-D4     012FC995     56                        PUSH ESI
	$-D3     012FC996     57                        PUSH EDI
	$-D2     012FC997     8928                      MOV DWORD PTR DS:[EAX],EBP
	$-D0     012FC999     8BE8                      MOV EBP,EAX
	$-CE     012FC99B     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$-C9     012FC9A0     33C5                      XOR EAX,EBP
	$-C7     012FC9A2     50                        PUSH EAX
	$-C6     012FC9A3     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$-C3     012FC9A6     8965 F0                   MOV DWORD PTR SS:[EBP-0x10],ESP
	$-C0     012FC9A9     FF75 FC                   PUSH DWORD PTR SS:[EBP-0x4]
	$-BD     012FC9AC     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-B6     012FC9B3     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-B3     012FC9B6     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-AD     012FC9BC     F2                        REPNE
	$-AC     012FC9BD     C3                        RETN
	$-AB     012FC9BE     CC                        INT3
	$-AA     012FC9BF     CC                        INT3
	$-A9     012FC9C0     53                        PUSH EBX
	$-A8     012FC9C1     56                        PUSH ESI
	$-A7     012FC9C2     8B4424 18                 MOV EAX,DWORD PTR SS:[ESP+0x18]
	$-A3     012FC9C6     0BC0                      OR EAX,EAX
	$-A1     012FC9C8     75 18                     JNZ SHORT PathOfEx.012FC9E2
	$-9F     012FC9CA     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
	$-9B     012FC9CE     8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$-97     012FC9D2     33D2                      XOR EDX,EDX
	$-95     012FC9D4     F7F1                      DIV ECX
	$-93     012FC9D6     8BD8                      MOV EBX,EAX
	$-91     012FC9D8     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-8D     012FC9DC     F7F1                      DIV ECX
	$-8B     012FC9DE     8BD3                      MOV EDX,EBX
	$-89     012FC9E0     EB 41                     JMP SHORT PathOfEx.012FCA23
	$-87     012FC9E2     8BC8                      MOV ECX,EAX
	$-85     012FC9E4     8B5C24 14                 MOV EBX,DWORD PTR SS:[ESP+0x14]
	$-81     012FC9E8     8B5424 10                 MOV EDX,DWORD PTR SS:[ESP+0x10]
	$-7D     012FC9EC     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-79     012FC9F0     D1E9                      SHR ECX,1
	$-77     012FC9F2     D1DB                      RCR EBX,1
	$-75     012FC9F4     D1EA                      SHR EDX,1
	$-73     012FC9F6     D1D8                      RCR EAX,1
	$-71     012FC9F8     0BC9                      OR ECX,ECX
	$-6F     012FC9FA   ^ 75 F4                     JNZ SHORT PathOfEx.012FC9F0
	$-6D     012FC9FC     F7F3                      DIV EBX
	$-6B     012FC9FE     8BF0                      MOV ESI,EAX
	$-69     012FCA00     F76424 18                 MUL DWORD PTR SS:[ESP+0x18]
	$-65     012FCA04     8BC8                      MOV ECX,EAX
	$-63     012FCA06     8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-5F     012FCA0A     F7E6                      MUL ESI
	$-5D     012FCA0C     03D1                      ADD EDX,ECX
	$-5B     012FCA0E     72 0E                     JB SHORT PathOfEx.012FCA1E
	$-59     012FCA10     3B5424 10                 CMP EDX,DWORD PTR SS:[ESP+0x10]
	$-55     012FCA14     77 08                     JA SHORT PathOfEx.012FCA1E
	$-53     012FCA16     72 07                     JB SHORT PathOfEx.012FCA1F
	$-51     012FCA18     3B4424 0C                 CMP EAX,DWORD PTR SS:[ESP+0xC]
	$-4D     012FCA1C     76 01                     JBE SHORT PathOfEx.012FCA1F
	$-4B     012FCA1E     4E                        DEC ESI
	$-4A     012FCA1F     33D2                      XOR EDX,EDX
	$-48     012FCA21     8BC6                      MOV EAX,ESI
	$-46     012FCA23     5E                        POP ESI
	$-45     012FCA24     5B                        POP EBX
	$-44     012FCA25     C2 1000                   RETN 0x10
	$-41     012FCA28     55                        PUSH EBP
	$-40     012FCA29     8BEC                      MOV EBP,ESP
	$-3E     012FCA2B     8325 80E96B01 00          AND DWORD PTR DS:[0x16BE980],0x0
	$-37     012FCA32     83EC 28                   SUB ESP,0x28
	$-34     012FCA35     53                        PUSH EBX
	$-33     012FCA36     33DB                      XOR EBX,EBX
	$-31     012FCA38     43                        INC EBX
	$-30     012FCA39     091D E02F6901             OR DWORD PTR DS:[0x1692FE0],EBX
	$-2A     012FCA3F     6A 0A                     PUSH 0xA
	$-28     012FCA41     E8 1ABA0700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$-23     012FCA46     85C0                      TEST EAX,EAX
	$-21     012FCA48     0F84 6D010000             JE PathOfEx.012FCBBB
	$-1B     012FCA4E     8365 F0 00                AND DWORD PTR SS:[EBP-0x10],0x0
	$-17     012FCA52     33C0                      XOR EAX,EAX
	$-15     012FCA54     830D E02F6901 02          OR DWORD PTR DS:[0x1692FE0],0x2
	$-E      012FCA5B     33C9                      XOR ECX,ECX
	$-C      012FCA5D     56                        PUSH ESI
	$-B      012FCA5E     57                        PUSH EDI
	$-A      012FCA5F     891D 80E96B01             MOV DWORD PTR DS:[0x16BE980],EBX
	$-4      012FCA65     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-1      012FCA68     53                        PUSH EBX
	$ ==>    012FCA69     0FA2                      CPUID                                                         ; g_h_cpuid5_
	$+2      012FCA6B     8BF3                      MOV ESI,EBX
	$+4      012FCA6D     5B                        POP EBX
	$+5      012FCA6E     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      012FCA70     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+A      012FCA73     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      012FCA76     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     012FCA79     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+13     012FCA7C     8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+16     012FCA7F     8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+19     012FCA82     81F1 696E6549             XOR ECX,0x49656E69
	$+1F     012FCA88     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+22     012FCA8B     35 6E74656C               XOR EAX,0x6C65746E
	$+27     012FCA90     0BC8                      OR ECX,EAX
	$+29     012FCA92     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+2C     012FCA95     6A 01                     PUSH 0x1
	$+2E     012FCA97     35 47656E75               XOR EAX,0x756E6547
	$+33     012FCA9C     0BC8                      OR ECX,EAX
	$+35     012FCA9E     58                        POP EAX
	$+36     012FCA9F     6A 00                     PUSH 0x0
	$+38     012FCAA1     59                        POP ECX
	$+39     012FCAA2     53                        PUSH EBX
	$+3A     012FCAA3     0FA2                      CPUID                                                         ; g_h_cpuid6_
	$+3C     012FCAA5     8BF3                      MOV ESI,EBX
	$+3E     012FCAA7     5B                        POP EBX
	$+3F     012FCAA8     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+41     012FCAAA     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$+44     012FCAAD     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+47     012FCAB0     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+4A     012FCAB3     75 43                     JNZ SHORT PathOfEx.012FCAF8
	$+4C     012FCAB5     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+4F     012FCAB8     25 F03FFF0F               AND EAX,0xFFF3FF0
	$+54     012FCABD     3D C0060100               CMP EAX,0x106C0
	$+59     012FCAC2     74 23                     JE SHORT PathOfEx.012FCAE7
	$+5B     012FCAC4     3D 60060200               CMP EAX,0x20660
	$+60     012FCAC9     74 1C                     JE SHORT PathOfEx.012FCAE7
	$+62     012FCACB     3D 70060200               CMP EAX,0x20670
	$+67     012FCAD0     74 15                     JE SHORT PathOfEx.012FCAE7
	$+69     012FCAD2     3D 50060300               CMP EAX,0x30650
	$+6E     012FCAD7     74 0E                     JE SHORT PathOfEx.012FCAE7
	$+70     012FCAD9     3D 60060300               CMP EAX,0x30660
	$+75     012FCADE     74 07                     JE SHORT PathOfEx.012FCAE7
	$+77     012FCAE0     3D 70060300               CMP EAX,0x30670
	$+7C     012FCAE5     75 11                     JNZ SHORT PathOfEx.012FCAF8
	$+7E     012FCAE7     8B3D 84E96B01             MOV EDI,DWORD PTR DS:[0x16BE984]
	$+84     012FCAED     83CF 01                   OR EDI,0x1
	$+87     012FCAF0     893D 84E96B01             MOV DWORD PTR DS:[0x16BE984],EDI
	$+8D     012FCAF6     EB 06                     JMP SHORT PathOfEx.012FCAFE
	$+8F     012FCAF8     8B3D 84E96B01             MOV EDI,DWORD PTR DS:[0x16BE984]
	$+95     012FCAFE     837D F8 07                CMP DWORD PTR SS:[EBP-0x8],0x7
	$+99     012FCB02     8B45 E4                   MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+9C     012FCB05     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+9F     012FCB08     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+A2     012FCB0B     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+A5     012FCB0E     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+A8     012FCB11     7C 32                     JL SHORT PathOfEx.012FCB45
	$+AA     012FCB13     6A 07                     PUSH 0x7
	$+AC     012FCB15     58                        POP EAX
	$+AD     012FCB16     33C9                      XOR ECX,ECX
	$+AF     012FCB18     53                        PUSH EBX
	$+B0     012FCB19     0FA2                      CPUID                                                         ; g_h_cpuid7_
	$+B2     012FCB1B     8BF3                      MOV ESI,EBX
	$+B4     012FCB1D     5B                        POP EBX
	$+B5     012FCB1E     8D5D D8                   LEA EBX,DWORD PTR SS:[EBP-0x28]
	$+B8     012FCB21     8903                      MOV DWORD PTR DS:[EBX],EAX
	$+BA     012FCB23     8973 04                   MOV DWORD PTR DS:[EBX+0x4],ESI
	$+BD     012FCB26     894B 08                   MOV DWORD PTR DS:[EBX+0x8],ECX
	$+C0     012FCB29     8953 0C                   MOV DWORD PTR DS:[EBX+0xC],EDX
	$+C3     012FCB2C     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+C6     012FCB2F     A9 00020000               TEST EAX,0x200
	$+CB     012FCB34     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+CE     012FCB37     8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+D1     012FCB3A     74 09                     JE SHORT PathOfEx.012FCB45
	$+D3     012FCB3C     83CF 02                   OR EDI,0x2
	$+D6     012FCB3F     893D 84E96B01             MOV DWORD PTR DS:[0x16BE984],EDI
	$+DC     012FCB45     5F                        POP EDI
	$+DD     012FCB46     5E                        POP ESI
	$+DE     012FCB47     A9 00001000               TEST EAX,0x100000
	$+E3     012FCB4C     74 6D                     JE SHORT PathOfEx.012FCBBB
	$+E5     012FCB4E     830D E02F6901 04          OR DWORD PTR DS:[0x1692FE0],0x4
	$+EC     012FCB55     C705 80E96B01 02000000    MOV DWORD PTR DS:[0x16BE980],0x2
	$+F6     012FCB5F     A9 00000008               TEST EAX,0x8000000
	$+FB     012FCB64     74 55                     JE SHORT PathOfEx.012FCBBB
	$+FD     012FCB66     A9 00000010               TEST EAX,0x10000000
	$+102    012FCB6B     74 4E                     JE SHORT PathOfEx.012FCBBB
	$+104    012FCB6D     33C9                      XOR ECX,ECX
	$+106    012FCB6F     0F01D0                    XGETBV
	$+109    012FCB72     8945 F4                   MOV DWORD PTR SS:[EBP-0xC],EAX
	$+10C    012FCB75     8955 F8                   MOV DWORD PTR SS:[EBP-0x8],EDX
	$+10F    012FCB78     8B45 F4                   MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+112    012FCB7B     8B4D F8                   MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+115    012FCB7E     83E0 06                   AND EAX,0x6
	$+118    012FCB81     33C9                      XOR ECX,ECX
	$+11A    012FCB83     83F8 06                   CMP EAX,0x6
	$+11D    012FCB86     75 33                     JNZ SHORT PathOfEx.012FCBBB
	$+11F    012FCB88     85C9                      TEST ECX,ECX
	$+121    012FCB8A     75 2F                     JNZ SHORT PathOfEx.012FCBBB
	$+123    012FCB8C     A1 E02F6901               MOV EAX,DWORD PTR DS:[0x1692FE0]
	$+128    012FCB91     83C8 08                   OR EAX,0x8
	$+12B    012FCB94     C705 80E96B01 03000000    MOV DWORD PTR DS:[0x16BE980],0x3
	$+135    012FCB9E     F645 F0 20                TEST BYTE PTR SS:[EBP-0x10],0x20
	$+139    012FCBA2     A3 E02F6901               MOV DWORD PTR DS:[0x1692FE0],EAX
	$+13E    012FCBA7     74 12                     JE SHORT PathOfEx.012FCBBB
	$+140    012FCBA9     83C8 20                   OR EAX,0x20
	$+143    012FCBAC     C705 80E96B01 05000000    MOV DWORD PTR DS:[0x16BE980],0x5
	$+14D    012FCBB6     A3 E02F6901               MOV DWORD PTR DS:[0x1692FE0],EAX
	$+152    012FCBBB     33C0                      XOR EAX,EAX
	$+154    012FCBBD     5B                        POP EBX
	$+155    012FCBBE     8BE5                      MOV ESP,EBP
	$+157    012FCBC0     5D                        POP EBP
	$+158    012FCBC1     C3                        RETN
	$+159    012FCBC2     33C0                      XOR EAX,EAX
	$+15B    012FCBC4     3905 00059601             CMP DWORD PTR DS:[0x1960500],EAX
	$+161    012FCBCA     0F95C0                    SETNE AL
	$+164    012FCBCD     C3                        RETN
	$+165    012FCBCE     55                        PUSH EBP
	$+166    012FCBCF     8BEC                      MOV EBP,ESP
	$+168    012FCBD1     81EC 24030000             SUB ESP,0x324
	$+16E    012FCBD7     53                        PUSH EBX
	$+16F    012FCBD8     56                        PUSH ESI
	$+170    012FCBD9     6A 17                     PUSH 0x17
	$+172    012FCBDB     E8 80B80700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$+177    012FCBE0     85C0                      TEST EAX,EAX
	$+179    012FCBE2     74 05                     JE SHORT PathOfEx.012FCBE9
	$+17B    012FCBE4     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+17E    012FCBE7     CD 29                     INT 0x29
	$+180    012FCBE9     33F6                      XOR ESI,ESI
	$+182    012FCBEB     8D85 DCFCFFFF             LEA EAX,DWORD PTR SS:[EBP-0x324]
	$+188    012FCBF1     68 CC020000               PUSH 0x2CC
	$+18D    012FCBF6     56                        PUSH ESI
	$+18E    012FCBF7     50                        PUSH EAX
	$+18F    012FCBF8     8935 88E96B01             MOV DWORD PTR DS:[0x16BE988],ESI
	$+195    012FCBFE     E8 FD7B0100               CALL PathOfEx.01314800
	$+19A    012FCC03     83C4 0C                   ADD ESP,0xC
	$+19D    012FCC06     8985 8CFDFFFF             MOV DWORD PTR SS:[EBP-0x274],EAX
	$+1A3    012FCC0C     898D 88FDFFFF             MOV DWORD PTR SS:[EBP-0x278],ECX
	$+1A9    012FCC12     8995 84FDFFFF             MOV DWORD PTR SS:[EBP-0x27C],EDX
	$+1AF    012FCC18     899D 80FDFFFF             MOV DWORD PTR SS:[EBP-0x280],EBX
	$+1B5    012FCC1E     89B5 7CFDFFFF             MOV DWORD PTR SS:[EBP-0x284],ESI
	$+1BB    012FCC24     89BD 78FDFFFF             MOV DWORD PTR SS:[EBP-0x288],EDI
	$+1C1    012FCC2A     66:8C95 A4FDFFFF          MOV WORD PTR SS:[EBP-0x25C],SS
	$+1C8    012FCC31     66:8C8D 98FDFFFF          MOV WORD PTR SS:[EBP-0x268],CS
	$+1CF    012FCC38     66:8C9D 74FDFFFF          MOV WORD PTR SS:[EBP-0x28C],DS
	$+1D6    012FCC3F     66:8C85 70FDFFFF          MOV WORD PTR SS:[EBP-0x290],ES
	$+1DD    012FCC46     66:8CA5 6CFDFFFF          MOV WORD PTR SS:[EBP-0x294],FS
	$+1E4    012FCC4D     66:8CAD 68FDFFFF          MOV WORD PTR SS:[EBP-0x298],GS
	$+1EB    012FCC54     9C                        PUSHFD
	$+1EC    012FCC55     8F85 9CFDFFFF             POP DWORD PTR SS:[EBP-0x264]
	$+1F2    012FCC5B     8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+1F5    012FCC5E     8985 94FDFFFF             MOV DWORD PTR SS:[EBP-0x26C],EAX
	$+1FB    012FCC64     8D45 04                   LEA EAX,DWORD PTR SS:[EBP+0x4]
	$+1FE    012FCC67     8985 A0FDFFFF             MOV DWORD PTR SS:[EBP-0x260],EAX
	$+204    012FCC6D     C785 DCFCFFFF 01000100    MOV DWORD PTR SS:[EBP-0x324],0x10001
	$+20E    012FCC77     8B40 FC                   MOV EAX,DWORD PTR DS:[EAX-0x4]
	$+211    012FCC7A     6A 50                     PUSH 0x50
	$+213    012FCC7C     8985 90FDFFFF             MOV DWORD PTR SS:[EBP-0x270],EAX
	$+219    012FCC82     8D45 A8                   LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+21C    012FCC85     56                        PUSH ESI
	$+21D    012FCC86     50                        PUSH EAX
	$+21E    012FCC87     E8 747B0100               CALL PathOfEx.01314800
	$+223    012FCC8C     8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+226    012FCC8F     83C4 0C                   ADD ESP,0xC
	$+229    012FCC92     C745 A8 15000040          MOV DWORD PTR SS:[EBP-0x58],0x40000015
	$+230    012FCC99     C745 AC 01000000          MOV DWORD PTR SS:[EBP-0x54],0x1
	$+237    012FCCA0     8945 B4                   MOV DWORD PTR SS:[EBP-0x4C],EAX
	$+23A    012FCCA3     FF15 80B24001             CALL DWORD PTR DS:[<&KERNEL32.IsDebuggerPresent>]             ; kernel32.IsDebuggerPresent
	$+240    012FCCA9     56                        PUSH ESI
	$+241    012FCCAA     8D58 FF                   LEA EBX,DWORD PTR DS:[EAX-0x1]
	$+244    012FCCAD     F7DB                      NEG EBX
	$+246    012FCCAF     8D45 A8                   LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+249    012FCCB2     8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+24C    012FCCB5     8D85 DCFCFFFF             LEA EAX,DWORD PTR SS:[EBP-0x324]
	$+252    012FCCBB     1ADB                      SBB BL,BL
	$+254    012FCCBD     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+257    012FCCC0     FEC3                      INC BL
	$+259    012FCCC2     FF15 8CB24001             CALL DWORD PTR DS:[<&KERNEL32.SetUnhandledExceptionFilter>]   ; kernel32.SetUnhandledExceptionFilter
	$+25F    012FCCC8     8D45 F8                   LEA EAX,DWORD PTR SS:[EBP-0x8]
	$+262    012FCCCB     50                        PUSH EAX
	$+263    012FCCCC     FF15 90B24001             CALL DWORD PTR DS:[<&KERNEL32.UnhandledExceptionFilter>]      ; kernel32.UnhandledExceptionFilter
	$+269    012FCCD2     85C0                      TEST EAX,EAX
	$+26B    012FCCD4     75 0D                     JNZ SHORT PathOfEx.012FCCE3
	$+26D    012FCCD6     0FB6C3                    MOVZX EAX,BL
	$+270    012FCCD9     F7D8                      NEG EAX
	$+272    012FCCDB     1BC0                      SBB EAX,EAX
	$+274    012FCCDD     2105 88E96B01             AND DWORD PTR DS:[0x16BE988],EAX
	$+27A    012FCCE3     5E                        POP ESI
	$+27B    012FCCE4     5B                        POP EBX
	$+27C    012FCCE5     8BE5                      MOV ESP,EBP
	$+27E    012FCCE7     5D                        POP EBP
	$+27F    012FCCE8     C3                        RETN
	$+280    012FCCE9     55                        PUSH EBP
	$+281    012FCCEA     8BEC                      MOV EBP,ESP
	$+283    012FCCEC     83EC 44                   SUB ESP,0x44
	$+286    012FCCEF     6A 44                     PUSH 0x44
	$+288    012FCCF1     8D45 BC                   LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+28B    012FCCF4     6A 00                     PUSH 0x0
	$+28D    012FCCF6     50                        PUSH EAX
	$+28E    012FCCF7     E8 047B0100               CALL PathOfEx.01314800
	$+293    012FCCFC     83C4 0C                   ADD ESP,0xC
	$+296    012FCCFF     8D45 BC                   LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+299    012FCD02     50                        PUSH EAX
	$+29A    012FCD03     FF15 7CB24001             CALL DWORD PTR DS:[<&KERNEL32.GetStartupInfoW>]               ; kernel32.GetStartupInfoW
	$+2A0    012FCD09     F645 E8 01                TEST BYTE PTR SS:[EBP-0x18],0x1
	$+2A4    012FCD0D     74 06                     JE SHORT PathOfEx.012FCD15
	$+2A6    012FCD0F     0FB745 EC                 MOVZX EAX,WORD PTR SS:[EBP-0x14]
	$+2AA    012FCD13     EB 03                     JMP SHORT PathOfEx.012FCD18
	$+2AC    012FCD15     6A 0A                     PUSH 0xA
	$+2AE    012FCD17     58                        POP EAX
	$+2AF    012FCD18     8BE5                      MOV ESP,EBP
	$+2B1    012FCD1A     5D                        POP EBP
	$+2B2    012FCD1B     C3                        RETN
	$+2B3    012FCD1C     68 28CD2F01               PUSH PathOfEx.012FCD28
	$+2B8    012FCD21     FF15 8CB24001             CALL DWORD PTR DS:[<&KERNEL32.SetUnhandledExceptionFilter>]   ; kernel32.SetUnhandledExceptionFilter
	$+2BE    012FCD27     C3                        RETN
	$+2BF    012FCD28     55                        PUSH EBP
	$+2C0    012FCD29     8BEC                      MOV EBP,ESP
	$+2C2    012FCD2B     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2C5    012FCD2E     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+2C7    012FCD30     8138 63736DE0             CMP DWORD PTR DS:[EAX],0xE06D7363
	$+2CD    012FCD36     75 25                     JNZ SHORT PathOfEx.012FCD5D
	$+2CF    012FCD38     8378 10 03                CMP DWORD PTR DS:[EAX+0x10],0x3
	$+2D3    012FCD3C     75 1F                     JNZ SHORT PathOfEx.012FCD5D
	$+2D5    012FCD3E     8B40 14                   MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+2D8    012FCD41     3D 20059319               CMP EAX,0x19930520
	$+2DD    012FCD46     74 1B                     JE SHORT PathOfEx.012FCD63
	$+2DF    012FCD48     3D 21059319               CMP EAX,0x19930521
	$+2E4    012FCD4D     74 14                     JE SHORT PathOfEx.012FCD63
	$+2E6    012FCD4F     3D 22059319               CMP EAX,0x19930522
	$+2EB    012FCD54     74 0D                     JE SHORT PathOfEx.012FCD63
	$+2ED    012FCD56     3D 00409901               CMP EAX,PathOfEx.01994000
	$+2F2    012FCD5B     74 06                     JE SHORT PathOfEx.012FCD63
	$+2F4    012FCD5D     33C0                      XOR EAX,EAX
	$+2F6    012FCD5F     5D                        POP EBP
	$+2F7    012FCD60     C2 0400                   RETN 0x4
	$+2FA    012FCD63     E8 93DA0200               CALL PathOfEx.0132A7FB
	$+2FF    012FCD68     CC                        INT3
	$+300    012FCD69     8325 88E96B01 00          AND DWORD PTR DS:[0x16BE988],0x0
	$+307    012FCD70     C3                        RETN
	$+308    012FCD71     CC                        INT3
	$+309    012FCD72     CC                        INT3
	$+30A    012FCD73     CC                        INT3
	$+30B    012FCD74     CC                        INT3
	$+30C    012FCD75     CC                        INT3
	$+30D    012FCD76     CC                        INT3
	$+30E    012FCD77     CC                        INT3
	$+30F    012FCD78     CC                        INT3
	$+310    012FCD79     CC                        INT3
	$+311    012FCD7A     CC                        INT3
	$+312    012FCD7B     CC                        INT3
	$+313    012FCD7C     CC                        INT3
	$+314    012FCD7D     CC                        INT3
	$+315    012FCD7E     CC                        INT3
	$+316    012FCD7F     CC                        INT3
	$+317    012FCD80     68 30703101               PUSH PathOfEx.01317030
	$+31C    012FCD85     64:FF35 00000000          PUSH DWORD PTR FS:[0]
	$+323    012FCD8C     8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+327    012FCD90     896C24 10                 MOV DWORD PTR SS:[ESP+0x10],EBP
	$+32B    012FCD94     8D6C24 10                 LEA EBP,DWORD PTR SS:[ESP+0x10]
	$+32F    012FCD98     2BE0                      SUB ESP,EAX
	$+331    012FCD9A     53                        PUSH EBX
	$+332    012FCD9B     56                        PUSH ESI
	$+333    012FCD9C     57                        PUSH EDI
	$+334    012FCD9D     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$+339    012FCDA2     3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+33C    012FCDA5     33C5                      XOR EAX,EBP
	$+33E    012FCDA7     50                        PUSH EAX
	$+33F    012FCDA8     8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
	$+342    012FCDAB     FF75 F8                   PUSH DWORD PTR SS:[EBP-0x8]
	$+345    012FCDAE     8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+348    012FCDB1     C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$+34F    012FCDB8     8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$+352    012FCDBB     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+355    012FCDBE     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+35B    012FCDC4     F2                        REPNE
	$+35C    012FCDC5     C3                        RETN
	$+35D    012FCDC6     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+360    012FCDC9     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+367    012FCDD0     59                        POP ECX
	$+368    012FCDD1     5F                        POP EDI
	$+369    012FCDD2     5F                        POP EDI
	$+36A    012FCDD3     5E                        POP ESI
	$+36B    012FCDD4     5B                        POP EBX
	$+36C    012FCDD5     8BE5                      MOV ESP,EBP
	$+36E    012FCDD7     5D                        POP EBP
	$+36F    012FCDD8     51                        PUSH ECX
	$+370    012FCDD9     F2                        REPNE
	$+371    012FCDDA     C3                        RETN
	$+372    012FCDDB     55                        PUSH EBP
	$+373    012FCDDC     8BEC                      MOV EBP,ESP
	$+375    012FCDDE     56                        PUSH ESI
	$+376    012FCDDF     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$+379    012FCDE2     8BF1                      MOV ESI,ECX
	$+37B    012FCDE4     E8 871A65FF               CALL PathOfEx.0094E870
	$+380    012FCDE9     C706 18BB4701             MOV DWORD PTR DS:[ESI],PathOfEx.0147BB18
	$+386    012FCDEF     8BC6                      MOV EAX,ESI
	$+388    012FCDF1     5E                        POP ESI
	$+389    012FCDF2     5D                        POP EBP
	$+38A    012FCDF3     C2 0400                   RETN 0x4
	$+38D    012FCDF6     8361 04 00                AND DWORD PTR DS:[ECX+0x4],0x0
	$+391    012FCDFA     8BC1                      MOV EAX,ECX
	$+393    012FCDFC     8361 08 00                AND DWORD PTR DS:[ECX+0x8],0x0
	$+397    012FCE00     C741 04 20BB4701          MOV DWORD PTR DS:[ECX+0x4],PathOfEx.0147BB20
	$+39E    012FCE07     C701 18BB4701             MOV DWORD PTR DS:[ECX],PathOfEx.0147BB18
	$+3A4    012FCE0D     C3                        RETN
	$+3A5    012FCE0E     55                        PUSH EBP
	$+3A6    012FCE0F     8BEC                      MOV EBP,ESP
	$+3A8    012FCE11     83EC 0C                   SUB ESP,0xC
	$+3AB    012FCE14     8D4D F4                   LEA ECX,DWORD PTR SS:[EBP-0xC]
	$+3AE    012FCE17     E8 DAFFFFFF               CALL PathOfEx.012FCDF6
	$+3B3    012FCE1C     68 EC4F6801               PUSH PathOfEx.01684FEC
	$+3B8    012FCE21     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+3BB    012FCE24     50                        PUSH EAX
	$+3BC    012FCE25     E8 59840100               CALL PathOfEx.01315283
	$+3C1    012FCE2A     CC                        INT3
	$+3C2    012FCE2B     55                        PUSH EBP
	$+3C3    012FCE2C     8BEC                      MOV EBP,ESP
	$+3C5    012FCE2E     83EC 14                   SUB ESP,0x14
	$+3C8    012FCE31     8365 F4 00                AND DWORD PTR SS:[EBP-0xC],0x0
	$+3CC    012FCE35     8365 F8 00                AND DWORD PTR SS:[EBP-0x8],0x0
	$+3D0    012FCE39     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$+3D5    012FCE3E     56                        PUSH ESI
	$+3D6    012FCE3F     57                        PUSH EDI
	$+3D7    012FCE40     BF 4EE640BB               MOV EDI,0xBB40E64E
	$+3DC    012FCE45     BE 0000FFFF               MOV ESI,0xFFFF0000
	$+3E1    012FCE4A     3BC7                      CMP EAX,EDI
	$+3E3    012FCE4C     74 0D                     JE SHORT PathOfEx.012FCE5B
	$+3E5    012FCE4E     85C6                      TEST ESI,EAX
	$+3E7    012FCE50     74 09                     JE SHORT PathOfEx.012FCE5B
	$+3E9    012FCE52     F7D0                      NOT EAX
	$+3EB    012FCE54     A3 D42F6901               MOV DWORD PTR DS:[0x1692FD4],EAX
	$+3F0    012FCE59     EB 66                     JMP SHORT PathOfEx.012FCEC1
	$+3F2    012FCE5B     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+3F5    012FCE5E     50                        PUSH EAX
	$+3F6    012FCE5F     FF15 BCB24001             CALL DWORD PTR DS:[<&KERNEL32.GetSystemTimeAsFileTime>]       ; kernel32.GetSystemTimeAsFileTime
	$+3FC    012FCE65     8B45 F8                   MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+3FF    012FCE68     3345 F4                   XOR EAX,DWORD PTR SS:[EBP-0xC]
	$+402    012FCE6B     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+405    012FCE6E     FF15 00B14001             CALL DWORD PTR DS:[<&KERNEL32.GetCurrentThreadId>]            ; kernel32.GetCurrentThreadId
	$+40B    012FCE74     3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+40E    012FCE77     FF15 B8B14001             CALL DWORD PTR DS:[<&KERNEL32.GetCurrentProcessId>]           ; kernel32.GetCurrentProcessId
	$+414    012FCE7D     3145 FC                   XOR DWORD PTR SS:[EBP-0x4],EAX
	$+417    012FCE80     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+41A    012FCE83     50                        PUSH EAX
	$+41B    012FCE84     FF15 18B34001             CALL DWORD PTR DS:[<&KERNEL32.QueryPerformanceCounter>]       ; kernel32.QueryPerformanceCounter
	$+421    012FCE8A     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+424    012FCE8D     8D45 FC                   LEA EAX,DWORD PTR SS:[EBP-0x4]
	$+427    012FCE90     334D EC                   XOR ECX,DWORD PTR SS:[EBP-0x14]
	$+42A    012FCE93     334D FC                   XOR ECX,DWORD PTR SS:[EBP-0x4]
	$+42D    012FCE96     33C8                      XOR ECX,EAX
	$+42F    012FCE98     3BCF                      CMP ECX,EDI
	$+431    012FCE9A     75 07                     JNZ SHORT PathOfEx.012FCEA3
	$+433    012FCE9C     B9 4FE640BB               MOV ECX,0xBB40E64F
	$+438    012FCEA1     EB 10                     JMP SHORT PathOfEx.012FCEB3
	$+43A    012FCEA3     85CE                      TEST ESI,ECX
	$+43C    012FCEA5     75 0C                     JNZ SHORT PathOfEx.012FCEB3
	$+43E    012FCEA7     8BC1                      MOV EAX,ECX
	$+440    012FCEA9     0D 11470000               OR EAX,0x4711
	$+445    012FCEAE     C1E0 10                   SHL EAX,0x10
	$+448    012FCEB1     0BC8                      OR ECX,EAX
	$+44A    012FCEB3     890D D82F6901             MOV DWORD PTR DS:[0x1692FD8],ECX
	$+450    012FCEB9     F7D1                      NOT ECX
	$+452    012FCEBB     890D D42F6901             MOV DWORD PTR DS:[0x1692FD4],ECX
	$+458    012FCEC1     5F                        POP EDI
	$+459    012FCEC2     5E                        POP ESI
	$+45A    012FCEC3     8BE5                      MOV ESP,EBP
	$+45C    012FCEC5     5D                        POP EBP
	$+45D    012FCEC6     C3                        RETN
	$+45E    012FCEC7     B8 00400000               MOV EAX,0x4000
	$+463    012FCECC     C3                        RETN
	$+464    012FCECD     68 00000300               PUSH 0x30000
	$+469    012FCED2     68 00000100               PUSH 0x10000
	$+46E    012FCED7     6A 00                     PUSH 0x0
	$+470    012FCED9     E8 E6880300               CALL PathOfEx.013357C4
	$+475    012FCEDE     83C4 0C                   ADD ESP,0xC
	$+478    012FCEE1     85C0                      TEST EAX,EAX
	$+47A    012FCEE3     75 01                     JNZ SHORT PathOfEx.012FCEE6
	$+47C    012FCEE5     C3                        RETN
	$+47D    012FCEE6     6A 07                     PUSH 0x7
	$+47F    012FCEE8     E8 E1FCFFFF               CALL PathOfEx.012FCBCE
	$+484    012FCEED     CC                        INT3
	$+485    012FCEEE     E8 5DF765FF               CALL PathOfEx.0095C650
	$+48A    012FCEF3     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+48D    012FCEF6     8308 04                   OR DWORD PTR DS:[EAX],0x4
	$+490    012FCEF9     8948 04                   MOV DWORD PTR DS:[EAX+0x4],ECX
	$+493    012FCEFC     E8 EF0392FF               CALL PathOfEx.00C1D2F0
	$+498    012FCF01     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+49B    012FCF04     8308 02                   OR DWORD PTR DS:[EAX],0x2
	$+49E    012FCF07     8948 04                   MOV DWORD PTR DS:[EAX+0x4],ECX
	$+4A1    012FCF0A     C3                        RETN
	$+4A2    012FCF0B     33C0                      XOR EAX,EAX
	$+4A4    012FCF0D     3905 F02F6901             CMP DWORD PTR DS:[0x1692FF0],EAX
	$+4AA    012FCF13     0F94C0                    SETE AL
	$+4AD    012FCF16     C3                        RETN
	$+4AE    012FCF17     B8 0CBB4701               MOV EAX,PathOfEx.0147BB0C
	$+4B3    012FCF1C     C3                        RETN
	$+4B4    012FCF1D     B8 10BB4701               MOV EAX,PathOfEx.0147BB10
	$+4B9    012FCF22     C3                        RETN
	$+4BA    012FCF23     53                        PUSH EBX
	$+4BB    012FCF24     56                        PUSH ESI
	$+4BC    012FCF25     BE DCB15F01               MOV ESI,PathOfEx.015FB1DC
	$+4C1    012FCF2A     BB DCB15F01               MOV EBX,PathOfEx.015FB1DC
	$+4C6    012FCF2F     3BF3                      CMP ESI,EBX
	$+4C8    012FCF31     73 18                     JNB SHORT PathOfEx.012FCF4B
	$+4CA    012FCF33     57                        PUSH EDI
	$+4CB    012FCF34     8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$+4CD    012FCF36     85FF                      TEST EDI,EDI
	$+4CF    012FCF38     74 09                     JE SHORT PathOfEx.012FCF43
	$+4D1    012FCF3A     8BCF                      MOV ECX,EDI
	$+4D3    012FCF3C     E8 64F9FFFF               CALL PathOfEx.012FC8A5
	$+4D8    012FCF41     FFD7                      CALL EDI
	$+4DA    012FCF43     83C6 04                   ADD ESI,0x4
	$+4DD    012FCF46     3BF3                      CMP ESI,EBX
	$+4DF    012FCF48   ^ 72 EA                     JB SHORT PathOfEx.012FCF34
	$+4E1    012FCF4A     5F                        POP EDI
	$+4E2    012FCF4B     5E                        POP ESI
	$+4E3    012FCF4C     5B                        POP EBX
	$+4E4    012FCF4D     C3                        RETN
	$+4E5    012FCF4E     53                        PUSH EBX
	$+4E6    012FCF4F     56                        PUSH ESI
	$+4E7    012FCF50     BE E4B15F01               MOV ESI,PathOfEx.015FB1E4
	$+4EC    012FCF55     BB E4B15F01               MOV EBX,PathOfEx.015FB1E4
	$+4F1    012FCF5A     3BF3                      CMP ESI,EBX
	$+4F3    012FCF5C     73 18                     JNB SHORT PathOfEx.012FCF76
	$+4F5    012FCF5E     57                        PUSH EDI
	$+4F6    012FCF5F     8B3E                      MOV EDI,DWORD PTR DS:[ESI]
	$+4F8    012FCF61     85FF                      TEST EDI,EDI
	$+4FA    012FCF63     74 09                     JE SHORT PathOfEx.012FCF6E
	$+4FC    012FCF65     8BCF                      MOV ECX,EDI
	$+4FE    012FCF67     E8 39F9FFFF               CALL PathOfEx.012FC8A5
	$+503    012FCF6C     FFD7                      CALL EDI
	$+505    012FCF6E     83C6 04                   ADD ESI,0x4
	$+508    012FCF71     3BF3                      CMP ESI,EBX
	$+50A    012FCF73   ^ 72 EA                     JB SHORT PathOfEx.012FCF5F
	$+50C    012FCF75     5F                        POP EDI
	$+50D    012FCF76     5E                        POP ESI
	$+50E    012FCF77     5B                        POP EBX
	$+50F    012FCF78     C3                        RETN
	$+510    012FCF79     CC                        INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("0f??8b??5b89??89????89????89????8b????8b????89????81??????????8b????35????????0b??8b????6a??35????????0b??586a??59530fa2", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("0f??8b??5b89??89????89????89????8b????8b????89????81??????????8b????35????????0b??8b????6a??35????????0b??586a??59530fa2", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_cpuid5_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = (pt_pcbyte)addr_tmp + 0x3a;
	ofs << "pt_cdword	g_h_cpuid6_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//上面函数的一部分
	/*
	$-78     00B92C31    59                        POP ECX
	$-77     00B92C32    53                        PUSH EBX
	$-76     00B92C33    0FA2                      CPUID                                                         ; g_h_cpuid6_
	$-74     00B92C35    8BF3                      MOV ESI,EBX
	$-72     00B92C37    5B                        POP EBX
	$-71     00B92C38    8907                      MOV DWORD PTR DS:[EDI],EAX
	$-6F     00B92C3A    8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$-6C     00B92C3D    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$-69     00B92C40    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$-66     00B92C43    75 43                     JNZ SHORT PathOfEx.00B92C88
	$-64     00B92C45    8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-61     00B92C48    25 F03FFF0F               AND EAX,0xFFF3FF0
	$-5C     00B92C4D    3D C0060100               CMP EAX,0x106C0
	$-57     00B92C52    74 23                     JE SHORT PathOfEx.00B92C77
	$-55     00B92C54    3D 60060200               CMP EAX,0x20660                                               ; UNICODE "4_none_41e6975e2bd6f2b2"
	$-50     00B92C59    74 1C                     JE SHORT PathOfEx.00B92C77
	$-4E     00B92C5B    3D 70060200               CMP EAX,0x20670                                               ; UNICODE "1e6975e2bd6f2b2"
	$-49     00B92C60    74 15                     JE SHORT PathOfEx.00B92C77
	$-47     00B92C62    3D 50060300               CMP EAX,0x30650
	$-42     00B92C67    74 0E                     JE SHORT PathOfEx.00B92C77
	$-40     00B92C69    3D 60060300               CMP EAX,0x30660
	$-3B     00B92C6E    74 07                     JE SHORT PathOfEx.00B92C77
	$-39     00B92C70    3D 70060300               CMP EAX,0x30670
	$-34     00B92C75    75 11                     JNZ SHORT PathOfEx.00B92C88
	$-32     00B92C77    8B3D 34C4F100             MOV EDI,DWORD PTR DS:[0xF1C434]
	$-2C     00B92C7D    83CF 01                   OR EDI,0x1
	$-29     00B92C80    893D 34C4F100             MOV DWORD PTR DS:[0xF1C434],EDI
	$-23     00B92C86    EB 06                     JMP SHORT PathOfEx.00B92C8E
	$-21     00B92C88    8B3D 34C4F100             MOV EDI,DWORD PTR DS:[0xF1C434]
	$-1B     00B92C8E    837D F8 07                CMP DWORD PTR SS:[EBP-0x8],0x7
	$-17     00B92C92    8B45 E4                   MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$-14     00B92C95    8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-11     00B92C98    8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-E      00B92C9B    8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$-B      00B92C9E    8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$-8      00B92CA1    7C 32                     JL SHORT PathOfEx.00B92CD5
	$-6      00B92CA3    6A 07                     PUSH 0x7
	$-4      00B92CA5    58                        POP EAX
	$-3      00B92CA6    33C9                      XOR ECX,ECX
	$-1      00B92CA8    53                        PUSH EBX
	$ ==>    00B92CA9    0FA2                      CPUID                                                         ; g_h_cpuid7_
	$+2      00B92CAB    8BF3                      MOV ESI,EBX
	$+4      00B92CAD    5B                        POP EBX
	$+5      00B92CAE    8D5D D8                   LEA EBX,DWORD PTR SS:[EBP-0x28]
	$+8      00B92CB1    8903                      MOV DWORD PTR DS:[EBX],EAX
	$+A      00B92CB3    8973 04                   MOV DWORD PTR DS:[EBX+0x4],ESI
	$+D      00B92CB6    894B 08                   MOV DWORD PTR DS:[EBX+0x8],ECX
	$+10     00B92CB9    8953 0C                   MOV DWORD PTR DS:[EBX+0xC],EDX
	$+13     00B92CBC    8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+16     00B92CBF    A9 00020000               TEST EAX,0x200
	$+1B     00B92CC4    8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1E     00B92CC7    8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+21     00B92CCA    74 09                     JE SHORT PathOfEx.00B92CD5
	$+23     00B92CCC    83CF 02                   OR EDI,0x2
	$+26     00B92CCF    893D 34C4F100             MOV DWORD PTR DS:[0xF1C434],EDI
	$+2C     00B92CD5    5F                        POP EDI
	$+2D     00B92CD6    5E                        POP ESI
	$+2E     00B92CD7    A9 00001000               TEST EAX,0x100000
	$+33     00B92CDC    74 6D                     JE SHORT PathOfEx.00B92D4B
	$+35     00B92CDE    830D 3098EF00 04          OR DWORD PTR DS:[0xEF9830],0x4
	$+3C     00B92CE5    C705 30C4F100 02000000    MOV DWORD PTR DS:[0xF1C430],0x2
	$+46     00B92CEF    A9 00000008               TEST EAX,0x8000000
	$+4B     00B92CF4    74 55                     JE SHORT PathOfEx.00B92D4B
	$+4D     00B92CF6    A9 00000010               TEST EAX,0x10000000
	$+52     00B92CFB    74 4E                     JE SHORT PathOfEx.00B92D4B
	$+54     00B92CFD    33C9                      XOR ECX,ECX
	$+56     00B92CFF    0F01D0                    XGETBV
	$+59     00B92D02    8945 F4                   MOV DWORD PTR SS:[EBP-0xC],EAX
	$+5C     00B92D05    8955 F8                   MOV DWORD PTR SS:[EBP-0x8],EDX
	$+5F     00B92D08    8B45 F4                   MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+62     00B92D0B    8B4D F8                   MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+65     00B92D0E    83E0 06                   AND EAX,0x6
	$+68     00B92D11    33C9                      XOR ECX,ECX
	$+6A     00B92D13    83F8 06                   CMP EAX,0x6
	$+6D     00B92D16    75 33                     JNZ SHORT PathOfEx.00B92D4B
	$+6F     00B92D18    85C9                      TEST ECX,ECX
	$+71     00B92D1A    75 2F                     JNZ SHORT PathOfEx.00B92D4B
	$+73     00B92D1C    A1 3098EF00               MOV EAX,DWORD PTR DS:[0xEF9830]
	$+78     00B92D21    83C8 08                   OR EAX,0x8
	$+7B     00B92D24    C705 30C4F100 03000000    MOV DWORD PTR DS:[0xF1C430],0x3
	$+85     00B92D2E    F645 F0 20                TEST BYTE PTR SS:[EBP-0x10],0x20
	$+89     00B92D32    A3 3098EF00               MOV DWORD PTR DS:[0xEF9830],EAX
	$+8E     00B92D37    74 12                     JE SHORT PathOfEx.00B92D4B
	$+90     00B92D39    83C8 20                   OR EAX,0x20
	$+93     00B92D3C    C705 30C4F100 05000000    MOV DWORD PTR DS:[0xF1C430],0x5
	$+9D     00B92D46    A3 3098EF00               MOV DWORD PTR DS:[0xEF9830],EAX
	$+A2     00B92D4B    33C0                      XOR EAX,EAX
	$+A4     00B92D4D    5B                        POP EBX
	$+A5     00B92D4E    8BE5                      MOV ESP,EBP
	$+A7     00B92D50    5D                        POP EBP
	$+A8     00B92D51    C3                        RETN
	$+A9     00B92D52    33C0                      XOR EAX,EAX
	*/
	//gj3
	/*
	$-B4     012FCA65     8D7D D8                   LEA EDI,DWORD PTR SS:[EBP-0x28]
	$-B1     012FCA68     53                        PUSH EBX
	$-B0     012FCA69     0FA2                      CPUID                                                         ; g_h_cpuid5_
	$-AE     012FCA6B     8BF3                      MOV ESI,EBX
	$-AC     012FCA6D     5B                        POP EBX
	$-AB     012FCA6E     8907                      MOV DWORD PTR DS:[EDI],EAX
	$-A9     012FCA70     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$-A6     012FCA73     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$-A3     012FCA76     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$-A0     012FCA79     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-9D     012FCA7C     8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$-9A     012FCA7F     8945 F8                   MOV DWORD PTR SS:[EBP-0x8],EAX
	$-97     012FCA82     81F1 696E6549             XOR ECX,0x49656E69
	$-91     012FCA88     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-8E     012FCA8B     35 6E74656C               XOR EAX,0x6C65746E
	$-89     012FCA90     0BC8                      OR ECX,EAX
	$-87     012FCA92     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-84     012FCA95     6A 01                     PUSH 0x1
	$-82     012FCA97     35 47656E75               XOR EAX,0x756E6547
	$-7D     012FCA9C     0BC8                      OR ECX,EAX
	$-7B     012FCA9E     58                        POP EAX
	$-7A     012FCA9F     6A 00                     PUSH 0x0
	$-78     012FCAA1     59                        POP ECX
	$-77     012FCAA2     53                        PUSH EBX
	$-76     012FCAA3     0FA2                      CPUID                                                         ; g_h_cpuid6_
	$-74     012FCAA5     8BF3                      MOV ESI,EBX
	$-72     012FCAA7     5B                        POP EBX
	$-71     012FCAA8     8907                      MOV DWORD PTR DS:[EDI],EAX
	$-6F     012FCAAA     8977 04                   MOV DWORD PTR DS:[EDI+0x4],ESI
	$-6C     012FCAAD     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$-69     012FCAB0     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$-66     012FCAB3     75 43                     JNZ SHORT PathOfEx.012FCAF8
	$-64     012FCAB5     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-61     012FCAB8     25 F03FFF0F               AND EAX,0xFFF3FF0
	$-5C     012FCABD     3D C0060100               CMP EAX,0x106C0
	$-57     012FCAC2     74 23                     JE SHORT PathOfEx.012FCAE7
	$-55     012FCAC4     3D 60060200               CMP EAX,0x20660
	$-50     012FCAC9     74 1C                     JE SHORT PathOfEx.012FCAE7
	$-4E     012FCACB     3D 70060200               CMP EAX,0x20670
	$-49     012FCAD0     74 15                     JE SHORT PathOfEx.012FCAE7
	$-47     012FCAD2     3D 50060300               CMP EAX,0x30650
	$-42     012FCAD7     74 0E                     JE SHORT PathOfEx.012FCAE7
	$-40     012FCAD9     3D 60060300               CMP EAX,0x30660
	$-3B     012FCADE     74 07                     JE SHORT PathOfEx.012FCAE7
	$-39     012FCAE0     3D 70060300               CMP EAX,0x30670
	$-34     012FCAE5     75 11                     JNZ SHORT PathOfEx.012FCAF8
	$-32     012FCAE7     8B3D 84E96B01             MOV EDI,DWORD PTR DS:[0x16BE984]
	$-2C     012FCAED     83CF 01                   OR EDI,0x1
	$-29     012FCAF0     893D 84E96B01             MOV DWORD PTR DS:[0x16BE984],EDI
	$-23     012FCAF6     EB 06                     JMP SHORT PathOfEx.012FCAFE
	$-21     012FCAF8     8B3D 84E96B01             MOV EDI,DWORD PTR DS:[0x16BE984]
	$-1B     012FCAFE     837D F8 07                CMP DWORD PTR SS:[EBP-0x8],0x7
	$-17     012FCB02     8B45 E4                   MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$-14     012FCB05     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-11     012FCB08     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-E      012FCB0B     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$-B      012FCB0E     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$-8      012FCB11     7C 32                     JL SHORT PathOfEx.012FCB45
	$-6      012FCB13     6A 07                     PUSH 0x7
	$-4      012FCB15     58                        POP EAX
	$-3      012FCB16     33C9                      XOR ECX,ECX
	$-1      012FCB18     53                        PUSH EBX
	$ ==>    012FCB19     0FA2                      CPUID                                                         ; g_h_cpuid7_
	$+2      012FCB1B     8BF3                      MOV ESI,EBX
	$+4      012FCB1D     5B                        POP EBX
	$+5      012FCB1E     8D5D D8                   LEA EBX,DWORD PTR SS:[EBP-0x28]
	$+8      012FCB21     8903                      MOV DWORD PTR DS:[EBX],EAX
	$+A      012FCB23     8973 04                   MOV DWORD PTR DS:[EBX+0x4],ESI
	$+D      012FCB26     894B 08                   MOV DWORD PTR DS:[EBX+0x8],ECX
	$+10     012FCB29     8953 0C                   MOV DWORD PTR DS:[EBX+0xC],EDX
	$+13     012FCB2C     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+16     012FCB2F     A9 00020000               TEST EAX,0x200
	$+1B     012FCB34     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1E     012FCB37     8B45 FC                   MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+21     012FCB3A     74 09                     JE SHORT PathOfEx.012FCB45
	$+23     012FCB3C     83CF 02                   OR EDI,0x2
	$+26     012FCB3F     893D 84E96B01             MOV DWORD PTR DS:[0x16BE984],EDI
	$+2C     012FCB45     5F                        POP EDI
	$+2D     012FCB46     5E                        POP ESI
	$+2E     012FCB47     A9 00001000               TEST EAX,0x100000
	$+33     012FCB4C     74 6D                     JE SHORT PathOfEx.012FCBBB
	$+35     012FCB4E     830D E02F6901 04          OR DWORD PTR DS:[0x1692FE0],0x4
	$+3C     012FCB55     C705 80E96B01 02000000    MOV DWORD PTR DS:[0x16BE980],0x2
	$+46     012FCB5F     A9 00000008               TEST EAX,0x8000000
	$+4B     012FCB64     74 55                     JE SHORT PathOfEx.012FCBBB
	$+4D     012FCB66     A9 00000010               TEST EAX,0x10000000
	$+52     012FCB6B     74 4E                     JE SHORT PathOfEx.012FCBBB
	$+54     012FCB6D     33C9                      XOR ECX,ECX
	$+56     012FCB6F     0F01D0                    XGETBV
	$+59     012FCB72     8945 F4                   MOV DWORD PTR SS:[EBP-0xC],EAX
	$+5C     012FCB75     8955 F8                   MOV DWORD PTR SS:[EBP-0x8],EDX
	$+5F     012FCB78     8B45 F4                   MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+62     012FCB7B     8B4D F8                   MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+65     012FCB7E     83E0 06                   AND EAX,0x6
	$+68     012FCB81     33C9                      XOR ECX,ECX
	$+6A     012FCB83     83F8 06                   CMP EAX,0x6
	$+6D     012FCB86     75 33                     JNZ SHORT PathOfEx.012FCBBB
	$+6F     012FCB88     85C9                      TEST ECX,ECX
	$+71     012FCB8A     75 2F                     JNZ SHORT PathOfEx.012FCBBB
	$+73     012FCB8C     A1 E02F6901               MOV EAX,DWORD PTR DS:[0x1692FE0]
	$+78     012FCB91     83C8 08                   OR EAX,0x8
	$+7B     012FCB94     C705 80E96B01 03000000    MOV DWORD PTR DS:[0x16BE980],0x3
	$+85     012FCB9E     F645 F0 20                TEST BYTE PTR SS:[EBP-0x10],0x20
	$+89     012FCBA2     A3 E02F6901               MOV DWORD PTR DS:[0x1692FE0],EAX
	$+8E     012FCBA7     74 12                     JE SHORT PathOfEx.012FCBBB
	$+90     012FCBA9     83C8 20                   OR EAX,0x20
	$+93     012FCBAC     C705 80E96B01 05000000    MOV DWORD PTR DS:[0x16BE980],0x5
	$+9D     012FCBB6     A3 E02F6901               MOV DWORD PTR DS:[0x1692FE0],EAX
	$+A2     012FCBBB     33C0                      XOR EAX,EAX
	$+A4     012FCBBD     5B                        POP EBX
	$+A5     012FCBBE     8BE5                      MOV ESP,EBP
	$+A7     012FCBC0     5D                        POP EBP
	$+A8     012FCBC1     C3                        RETN
	$+A9     012FCBC2     33C0                      XOR EAX,EAX
	$+AB     012FCBC4     3905 00059601             CMP DWORD PTR DS:[0x1960500],EAX
	$+B1     012FCBCA     0F95C0                    SETNE AL
	$+B4     012FCBCD     C3                        RETN
	$+B5     012FCBCE     55                        PUSH EBP
	$+B6     012FCBCF     8BEC                      MOV EBP,ESP
	$+B8     012FCBD1     81EC 24030000             SUB ESP,0x324
	$+BE     012FCBD7     53                        PUSH EBX
	$+BF     012FCBD8     56                        PUSH ESI
	$+C0     012FCBD9     6A 17                     PUSH 0x17
	$+C2     012FCBDB     E8 80B80700               CALL <JMP.&KERNEL32.IsProcessorFeaturePresent>
	$+C7     012FCBE0     85C0                      TEST EAX,EAX
	$+C9     012FCBE2     74 05                     JE SHORT PathOfEx.012FCBE9
	$+CB     012FCBE4     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+CE     012FCBE7     CD 29                     INT 0x29
	$+D0     012FCBE9     33F6                      XOR ESI,ESI
	$+D2     012FCBEB     8D85 DCFCFFFF             LEA EAX,DWORD PTR SS:[EBP-0x324]
	$+D8     012FCBF1     68 CC020000               PUSH 0x2CC
	$+DD     012FCBF6     56                        PUSH ESI
	$+DE     012FCBF7     50                        PUSH EAX
	$+DF     012FCBF8     8935 88E96B01             MOV DWORD PTR DS:[0x16BE988],ESI
	$+E5     012FCBFE     E8 FD7B0100               CALL PathOfEx.01314800
	$+EA     012FCC03     83C4 0C                   ADD ESP,0xC
	$+ED     012FCC06     8985 8CFDFFFF             MOV DWORD PTR SS:[EBP-0x274],EAX
	$+F3     012FCC0C     898D 88FDFFFF             MOV DWORD PTR SS:[EBP-0x278],ECX
	$+F9     012FCC12     8995 84FDFFFF             MOV DWORD PTR SS:[EBP-0x27C],EDX
	$+FF     012FCC18     899D 80FDFFFF             MOV DWORD PTR SS:[EBP-0x280],EBX
	$+105    012FCC1E     89B5 7CFDFFFF             MOV DWORD PTR SS:[EBP-0x284],ESI
	$+10B    012FCC24     89BD 78FDFFFF             MOV DWORD PTR SS:[EBP-0x288],EDI
	$+111    012FCC2A     66:8C95 A4FDFFFF          MOV WORD PTR SS:[EBP-0x25C],SS
	$+118    012FCC31     66:8C8D 98FDFFFF          MOV WORD PTR SS:[EBP-0x268],CS
	$+11F    012FCC38     66:8C9D 74FDFFFF          MOV WORD PTR SS:[EBP-0x28C],DS
	$+126    012FCC3F     66:8C85 70FDFFFF          MOV WORD PTR SS:[EBP-0x290],ES
	$+12D    012FCC46     66:8CA5 6CFDFFFF          MOV WORD PTR SS:[EBP-0x294],FS
	$+134    012FCC4D     66:8CAD 68FDFFFF          MOV WORD PTR SS:[EBP-0x298],GS
	$+13B    012FCC54     9C                        PUSHFD
	$+13C    012FCC55     8F85 9CFDFFFF             POP DWORD PTR SS:[EBP-0x264]
	$+142    012FCC5B     8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+145    012FCC5E     8985 94FDFFFF             MOV DWORD PTR SS:[EBP-0x26C],EAX
	$+14B    012FCC64     8D45 04                   LEA EAX,DWORD PTR SS:[EBP+0x4]
	$+14E    012FCC67     8985 A0FDFFFF             MOV DWORD PTR SS:[EBP-0x260],EAX
	$+154    012FCC6D     C785 DCFCFFFF 01000100    MOV DWORD PTR SS:[EBP-0x324],0x10001
	$+15E    012FCC77     8B40 FC                   MOV EAX,DWORD PTR DS:[EAX-0x4]
	$+161    012FCC7A     6A 50                     PUSH 0x50
	$+163    012FCC7C     8985 90FDFFFF             MOV DWORD PTR SS:[EBP-0x270],EAX
	$+169    012FCC82     8D45 A8                   LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+16C    012FCC85     56                        PUSH ESI
	$+16D    012FCC86     50                        PUSH EAX
	$+16E    012FCC87     E8 747B0100               CALL PathOfEx.01314800
	$+173    012FCC8C     8B45 04                   MOV EAX,DWORD PTR SS:[EBP+0x4]
	$+176    012FCC8F     83C4 0C                   ADD ESP,0xC
	$+179    012FCC92     C745 A8 15000040          MOV DWORD PTR SS:[EBP-0x58],0x40000015
	$+180    012FCC99     C745 AC 01000000          MOV DWORD PTR SS:[EBP-0x54],0x1
	$+187    012FCCA0     8945 B4                   MOV DWORD PTR SS:[EBP-0x4C],EAX
	$+18A    012FCCA3     FF15 80B24001             CALL DWORD PTR DS:[<&KERNEL32.IsDebuggerPresent>]             ; kernel32.IsDebuggerPresent
	$+190    012FCCA9     56                        PUSH ESI
	$+191    012FCCAA     8D58 FF                   LEA EBX,DWORD PTR DS:[EAX-0x1]
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("0f??8b??5b8d????89??89????89????89????8b????a9????????89????8b", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("0f??8b??5b8d????89??89????89????89????8b????a9", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_cpuid7_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-43     012ABDFF    CC                        INT3
	$-42     012ABE00    55                        PUSH EBP                                                      ; tag_获得cpuid的包装函数
	$-41     012ABE01    8BEC                      MOV EBP,ESP
	$-3F     012ABE03    6A FE                     PUSH -0x2
	$-3D     012ABE05    68 C8225901               PUSH PathOfEx.015922C8
	$-38     012ABE0A    68 40D22401               PUSH PathOfEx.0124D240
	$-33     012ABE0F    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-2D     012ABE15    50                        PUSH EAX
	$-2C     012ABE16    83EC 08                   SUB ESP,0x8
	$-29     012ABE19    53                        PUSH EBX
	$-28     012ABE1A    56                        PUSH ESI
	$-27     012ABE1B    57                        PUSH EDI
	$-26     012ABE1C    A1 28985901               MOV EAX,DWORD PTR DS:[0x1599828]
	$-21     012ABE21    3145 F8                   XOR DWORD PTR SS:[EBP-0x8],EAX
	$-1E     012ABE24    33C5                      XOR EAX,EBP
	$-1C     012ABE26    50                        PUSH EAX
	$-1B     012ABE27    8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-18     012ABE2A    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-12     012ABE30    8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
	$-F      012ABE33    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-8      012ABE3A    8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-5      012ABE3D    B9 00000000               MOV ECX,0x0
	$ ==>    012ABE42    0FA2                      CPUID                                                         ; g_h_cpuid8_
	$+2      012ABE44    8B7D 0C                   MOV EDI,DWORD PTR SS:[EBP+0xC]
	$+5      012ABE47    8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      012ABE49    895F 04                   MOV DWORD PTR DS:[EDI+0x4],EBX
	$+A      012ABE4C    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      012ABE4F    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     012ABE52    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$+17     012ABE59    837D 08 00                CMP DWORD PTR SS:[EBP+0x8],0x0
	$+1B     012ABE5D    75 1C                     JNZ SHORT PathOfEx.012ABE7B
	$+1D     012ABE5F    33C0                      XOR EAX,EAX
	$+1F     012ABE61    8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+22     012ABE64    3901                      CMP DWORD PTR DS:[ECX],EAX
	$+24     012ABE66    0F95C0                    SETNE AL
	$+27     012ABE69    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+2A     012ABE6C    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+31     012ABE73    59                        POP ECX
	$+32     012ABE74    5F                        POP EDI
	$+33     012ABE75    5E                        POP ESI
	$+34     012ABE76    5B                        POP EBX
	$+35     012ABE77    8BE5                      MOV ESP,EBP
	$+37     012ABE79    5D                        POP EBP
	$+38     012ABE7A    C3                        RETN
	$+39     012ABE7B    B0 01                     MOV AL,0x1
	$+3B     012ABE7D    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+3E     012ABE80    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+45     012ABE87    59                        POP ECX
	$+46     012ABE88    5F                        POP EDI
	$+47     012ABE89    5E                        POP ESI
	$+48     012ABE8A    5B                        POP EBX
	$+49     012ABE8B    8BE5                      MOV ESP,EBP
	$+4B     012ABE8D    5D                        POP EBP
	$+4C     012ABE8E    C3                        RETN
	$+4D     012ABE8F    B8 01000000               MOV EAX,0x1
	$+52     012ABE94    C3                        RETN
	*/
	//gj3
	/*
	$-113    01375B6F     CC                        INT3
	$-112    01375B70     51                        PUSH ECX
	$-111    01375B71     53                        PUSH EBX
	$-110    01375B72     55                        PUSH EBP
	$-10F    01375B73     56                        PUSH ESI
	$-10E    01375B74     57                        PUSH EDI
	$-10D    01375B75     8D4C24 13                 LEA ECX,DWORD PTR SS:[ESP+0x13]
	$-109    01375B79     E8 32FDFFFF               CALL PathOfEx.013758B0
	$-104    01375B7E     8B5C24 1C                 MOV EBX,DWORD PTR SS:[ESP+0x1C]
	$-100    01375B82     33F6                      XOR ESI,ESI
	$-FE     01375B84     8B6C24 18                 MOV EBP,DWORD PTR SS:[ESP+0x18]
	$-FA     01375B88     8B38                      MOV EDI,DWORD PTR DS:[EAX]
	$-F8     01375B8A     0FB707                    MOVZX EAX,WORD PTR DS:[EDI]
	$-F5     01375B8D     3BC3                      CMP EAX,EBX
	$-F3     01375B8F     73 1D                     JNB SHORT PathOfEx.01375BAE
	$-F1     01375B91     8BCF                      MOV ECX,EDI
	$-EF     01375B93     0FB701                    MOVZX EAX,WORD PTR DS:[ECX]
	$-EC     01375B96     8BCD                      MOV ECX,EBP
	$-EA     01375B98     50                        PUSH EAX
	$-E9     01375B99     E8 2236FEFF               CALL PathOfEx.013591C0
	$-E4     01375B9E     85C0                      TEST EAX,EAX
	$-E2     01375BA0     74 27                     JE SHORT PathOfEx.01375BC9
	$-E0     01375BA2     46                        INC ESI
	$-DF     01375BA3     0FB70477                  MOVZX EAX,WORD PTR DS:[EDI+ESI*2]
	$-DB     01375BA7     8D0C77                    LEA ECX,DWORD PTR DS:[EDI+ESI*2]
	$-D8     01375BAA     3BC3                      CMP EAX,EBX
	$-D6     01375BAC   ^ 72 E5                     JB SHORT PathOfEx.01375B93
	$-D4     01375BAE     0FB70477                  MOVZX EAX,WORD PTR DS:[EDI+ESI*2]
	$-D0     01375BB2     3BD8                      CMP EBX,EAX
	$-CE     01375BB4     75 1B                     JNZ SHORT PathOfEx.01375BD1
	$-CC     01375BB6     53                        PUSH EBX
	$-CB     01375BB7     8BCD                      MOV ECX,EBP
	$-C9     01375BB9     E8 0236FEFF               CALL PathOfEx.013591C0
	$-C4     01375BBE     5F                        POP EDI
	$-C3     01375BBF     F7D8                      NEG EAX
	$-C1     01375BC1     5E                        POP ESI
	$-C0     01375BC2     1BC0                      SBB EAX,EAX
	$-BE     01375BC4     5D                        POP EBP
	$-BD     01375BC5     40                        INC EAX
	$-BC     01375BC6     5B                        POP EBX
	$-BB     01375BC7     59                        POP ECX
	$-BA     01375BC8     C3                        RETN
	$-B9     01375BC9     5F                        POP EDI
	$-B8     01375BCA     5E                        POP ESI
	$-B7     01375BCB     5D                        POP EBP
	$-B6     01375BCC     B0 01                     MOV AL,0x1
	$-B4     01375BCE     5B                        POP EBX
	$-B3     01375BCF     59                        POP ECX
	$-B2     01375BD0     C3                        RETN
	$-B1     01375BD1     5F                        POP EDI
	$-B0     01375BD2     5E                        POP ESI
	$-AF     01375BD3     5D                        POP EBP
	$-AE     01375BD4     32C0                      XOR AL,AL
	$-AC     01375BD6     5B                        POP EBX
	$-AB     01375BD7     59                        POP ECX
	$-AA     01375BD8     C3                        RETN
	$-A9     01375BD9     CC                        INT3
	$-A8     01375BDA     CC                        INT3
	$-A7     01375BDB     CC                        INT3
	$-A6     01375BDC     CC                        INT3
	$-A5     01375BDD     CC                        INT3
	$-A4     01375BDE     CC                        INT3
	$-A3     01375BDF     CC                        INT3
	$-A2     01375BE0     56                        PUSH ESI
	$-A1     01375BE1     8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
	$-9D     01375BE5     56                        PUSH ESI
	$-9C     01375BE6     E8 25E3FFFF               CALL PathOfEx.01373F10
	$-97     01375BEB     83C4 04                   ADD ESP,0x4
	$-94     01375BEE     84C0                      TEST AL,AL
	$-92     01375BF0     74 17                     JE SHORT PathOfEx.01375C09
	$-90     01375BF2     6A 01                     PUSH 0x1
	$-8E     01375BF4     56                        PUSH ESI
	$-8D     01375BF5     FF7424 10                 PUSH DWORD PTR SS:[ESP+0x10]
	$-89     01375BF9     E8 D2F9FFFF               CALL PathOfEx.013755D0
	$-84     01375BFE     83C4 0C                   ADD ESP,0xC
	$-81     01375C01     84C0                      TEST AL,AL
	$-7F     01375C03     74 04                     JE SHORT PathOfEx.01375C09
	$-7D     01375C05     B0 01                     MOV AL,0x1
	$-7B     01375C07     EB 02                     JMP SHORT PathOfEx.01375C0B
	$-79     01375C09     32C0                      XOR AL,AL
	$-77     01375C0B     837C24 10 01              CMP DWORD PTR SS:[ESP+0x10],0x1
	$-72     01375C10     72 1D                     JB SHORT PathOfEx.01375C2F
	$-70     01375C12     84C0                      TEST AL,AL
	$-6E     01375C14     74 17                     JE SHORT PathOfEx.01375C2D
	$-6C     01375C16     6A 0A                     PUSH 0xA
	$-6A     01375C18     56                        PUSH ESI
	$-69     01375C19     FF7424 10                 PUSH DWORD PTR SS:[ESP+0x10]
	$-65     01375C1D     E8 AEF9FFFF               CALL PathOfEx.013755D0
	$-60     01375C22     83C4 0C                   ADD ESP,0xC
	$-5D     01375C25     84C0                      TEST AL,AL
	$-5B     01375C27     74 04                     JE SHORT PathOfEx.01375C2D
	$-59     01375C29     B0 01                     MOV AL,0x1
	$-57     01375C2B     5E                        POP ESI
	$-56     01375C2C     C3                        RETN
	$-55     01375C2D     32C0                      XOR AL,AL
	$-53     01375C2F     5E                        POP ESI
	$-52     01375C30     C3                        RETN
	$-51     01375C31     CC                        INT3
	$-50     01375C32     CC                        INT3
	$-4F     01375C33     CC                        INT3
	$-4E     01375C34     CC                        INT3
	$-4D     01375C35     CC                        INT3
	$-4C     01375C36     CC                        INT3
	$-4B     01375C37     CC                        INT3
	$-4A     01375C38     CC                        INT3
	$-49     01375C39     CC                        INT3
	$-48     01375C3A     CC                        INT3
	$-47     01375C3B     CC                        INT3
	$-46     01375C3C     CC                        INT3
	$-45     01375C3D     CC                        INT3
	$-44     01375C3E     CC                        INT3
	$-43     01375C3F     CC                        INT3
	$-42     01375C40     55                        PUSH EBP                                                      ; tag_获得cpuid的包装函数
	$-41     01375C41     8BEC                      MOV EBP,ESP
	$-3F     01375C43     6A FE                     PUSH -0x2
	$-3D     01375C45     68 30AB6801               PUSH PathOfEx.0168AB30
	$-38     01375C4A     68 30703101               PUSH PathOfEx.01317030
	$-33     01375C4F     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-2D     01375C55     50                        PUSH EAX
	$-2C     01375C56     83EC 08                   SUB ESP,0x8
	$-29     01375C59     53                        PUSH EBX
	$-28     01375C5A     56                        PUSH ESI
	$-27     01375C5B     57                        PUSH EDI
	$-26     01375C5C     A1 D82F6901               MOV EAX,DWORD PTR DS:[0x1692FD8]
	$-21     01375C61     3145 F8                   XOR DWORD PTR SS:[EBP-0x8],EAX
	$-1E     01375C64     33C5                      XOR EAX,EBP
	$-1C     01375C66     50                        PUSH EAX
	$-1B     01375C67     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-18     01375C6A     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-12     01375C70     8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
	$-F      01375C73     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-8      01375C7A     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-5      01375C7D     B9 00000000               MOV ECX,0x0
	$ ==>    01375C82     0FA2                      CPUID                                                         ; g_h_cpuid8_
	$+2      01375C84     8B7D 0C                   MOV EDI,DWORD PTR SS:[EBP+0xC]
	$+5      01375C87     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+7      01375C89     895F 04                   MOV DWORD PTR DS:[EDI+0x4],EBX
	$+A      01375C8C     894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
	$+D      01375C8F     8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
	$+10     01375C92     C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$+17     01375C99     837D 08 00                CMP DWORD PTR SS:[EBP+0x8],0x0
	$+1B     01375C9D     75 1C                     JNZ SHORT PathOfEx.01375CBB
	$+1D     01375C9F     33C0                      XOR EAX,EAX
	$+1F     01375CA1     8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+22     01375CA4     3901                      CMP DWORD PTR DS:[ECX],EAX
	$+24     01375CA6     0F95C0                    SETNE AL
	$+27     01375CA9     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+2A     01375CAC     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+31     01375CB3     59                        POP ECX
	$+32     01375CB4     5F                        POP EDI
	$+33     01375CB5     5E                        POP ESI
	$+34     01375CB6     5B                        POP EBX
	$+35     01375CB7     8BE5                      MOV ESP,EBP
	$+37     01375CB9     5D                        POP EBP
	$+38     01375CBA     C3                        RETN
	$+39     01375CBB     B0 01                     MOV AL,0x1
	$+3B     01375CBD     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+3E     01375CC0     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+45     01375CC7     59                        POP ECX
	$+46     01375CC8     5F                        POP EDI
	$+47     01375CC9     5E                        POP ESI
	$+48     01375CCA     5B                        POP EBX
	$+49     01375CCB     8BE5                      MOV ESP,EBP
	$+4B     01375CCD     5D                        POP EBP
	$+4C     01375CCE     C3                        RETN
	$+4D     01375CCF     B8 01000000               MOV EAX,0x1
	$+52     01375CD4     C3                        RETN
	$+53     01375CD5     8B65 E8                   MOV ESP,DWORD PTR SS:[EBP-0x18]
	$+56     01375CD8     C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
	$+5D     01375CDF     32C0                      XOR AL,AL
	$+5F     01375CE1     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+62     01375CE4     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+69     01375CEB     59                        POP ECX
	$+6A     01375CEC     5F                        POP EDI
	$+6B     01375CED     5E                        POP ESI
	$+6C     01375CEE     5B                        POP EBX
	$+6D     01375CEF     8BE5                      MOV ESP,EBP
	$+6F     01375CF1     5D                        POP EBP
	$+70     01375CF2     C3                        RETN
	$+71     01375CF3     CC                        INT3
	$+72     01375CF4     CC                        INT3
	$+73     01375CF5     CC                        INT3
	$+74     01375CF6     CC                        INT3
	$+75     01375CF7     CC                        INT3
	$+76     01375CF8     CC                        INT3
	$+77     01375CF9     CC                        INT3
	$+78     01375CFA     CC                        INT3
	$+79     01375CFB     CC                        INT3
	$+7A     01375CFC     CC                        INT3
	$+7B     01375CFD     CC                        INT3
	$+7C     01375CFE     CC                        INT3
	$+7D     01375CFF     CC                        INT3
	$+7E     01375D00     83EC 40                   SUB ESP,0x40
	$+81     01375D03     8D0424                    LEA EAX,DWORD PTR SS:[ESP]
	$+84     01375D06     50                        PUSH EAX
	$+85     01375D07     6A 00                     PUSH 0x0
	$+87     01375D09     E8 32FFFFFF               CALL PathOfEx.01375C40
	$+8C     01375D0E     83C4 08                   ADD ESP,0x8
	$+8F     01375D11     84C0                      TEST AL,AL
	$+91     01375D13     0F84 7F010000             JE PathOfEx.01375E98
	$+97     01375D19     8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+9B     01375D1D     50                        PUSH EAX
	$+9C     01375D1E     6A 01                     PUSH 0x1
	$+9E     01375D20     E8 1BFFFFFF               CALL PathOfEx.01375C40
	$+A3     01375D25     83C4 08                   ADD ESP,0x8
	$+A6     01375D28     84C0                      TEST AL,AL
	$+A8     01375D2A     0F84 68010000             JE PathOfEx.01375E98
	$+AE     01375D30     8B4C24 1C                 MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+B2     01375D34     8BC1                      MOV EAX,ECX
	$+B4     01375D36     C1E8 17                   SHR EAX,0x17
	$+B7     01375D39     24 01                     AND AL,0x1
	$+B9     01375D3B     A2 B5036C01               MOV BYTE PTR DS:[0x16C03B5],AL
	$+BE     01375D40     F7C1 00000004             TEST ECX,0x4000000
	$+C4     01375D46     74 10                     JE SHORT PathOfEx.01375D58
	$+C6     01375D48     E8 93010000               CALL PathOfEx.01375EE0
	$+CB     01375D4D     8B4C24 1C                 MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+CF     01375D51     A2 B7036C01               MOV BYTE PTR DS:[0x16C03B7],AL
	$+D4     01375D56     EB 05                     JMP SHORT PathOfEx.01375D5D
	$+D6     01375D58     A0 B7036C01               MOV AL,BYTE PTR DS:[0x16C03B7]
	$+DB     01375D5D     8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
	$+DF     01375D61     84C0                      TEST AL,AL
	$+E1     01375D63     74 0F                     JE SHORT PathOfEx.01375D74
	$+E3     01375D65     C605 B8036C01 01          MOV BYTE PTR DS:[0x16C03B8],0x1
	$+EA     01375D6C     F7C2 00020000             TEST EDX,0x200
	$+F0     01375D72     75 07                     JNZ SHORT PathOfEx.01375D7B
	$+F2     01375D74     C605 B8036C01 00          MOV BYTE PTR DS:[0x16C03B8],0x0
	$+F9     01375D7B     84C0                      TEST AL,AL
	$+FB     01375D7D     74 0F                     JE SHORT PathOfEx.01375D8E
	$+FD     01375D7F     C605 B9036C01 01          MOV BYTE PTR DS:[0x16C03B9],0x1
	$+104    01375D86     F7C2 00000002             TEST EDX,0x2000000
	$+10A    01375D8C     75 07                     JNZ SHORT PathOfEx.01375D95
	$+10C    01375D8E     C605 B9036C01 00          MOV BYTE PTR DS:[0x16C03B9],0x0
	$+113    01375D95     84C0                      TEST AL,AL
	$+115    01375D97     74 0C                     JE SHORT PathOfEx.01375DA5
	$+117    01375D99     C605 BA036C01 01          MOV BYTE PTR DS:[0x16C03BA],0x1
	$+11E    01375DA0     F6C2 02                   TEST DL,0x2
	$+121    01375DA3     75 07                     JNZ SHORT PathOfEx.01375DAC
	$+123    01375DA5     C605 BA036C01 00          MOV BYTE PTR DS:[0x16C03BA],0x0
	$+12A    01375DAC     F7C1 00000002             TEST ECX,0x2000000
	$+130    01375DB2     74 09                     JE SHORT PathOfEx.01375DBD
	$+132    01375DB4     C605 B6036C01 01          MOV BYTE PTR DS:[0x16C03B6],0x1
	$+139    01375DBB     EB 40                     JMP SHORT PathOfEx.01375DFD
	$+13B    01375DBD     8D4424 20                 LEA EAX,DWORD PTR SS:[ESP+0x20]
	$+13F    01375DC1     50                        PUSH EAX
	$+140    01375DC2     68 00000080               PUSH 0x80000000
	$+145    01375DC7     E8 74FEFFFF               CALL PathOfEx.01375C40
	$+14A    01375DCC     83C4 08                   ADD ESP,0x8
	$+14D    01375DCF     817C24 20 01000080        CMP DWORD PTR SS:[ESP+0x20],0x80000001
	$+155    01375DD7     72 20                     JB SHORT PathOfEx.01375DF9
	$+157    01375DD9     8D4424 20                 LEA EAX,DWORD PTR SS:[ESP+0x20]
	$+15B    01375DDD     50                        PUSH EAX
	$+15C    01375DDE     68 01000080               PUSH 0x80000001
	$+161    01375DE3     E8 58FEFFFF               CALL PathOfEx.01375C40
	$+166    01375DE8     8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
	$+16A    01375DEC     83C4 08                   ADD ESP,0x8
	$+16D    01375DEF     C1E8 16                   SHR EAX,0x16
	$+170    01375DF2     24 01                     AND AL,0x1
	$+172    01375DF4     A2 B6036C01               MOV BYTE PTR DS:[0x16C03B6],AL
	$+177    01375DF9     8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
	$+17B    01375DFD     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+17F    01375E01     3D 47656E75               CMP EAX,0x756E6547
	$+184    01375E06     0F85 90000000             JNZ PathOfEx.01375E9C
	$+18A    01375E0C     817C24 08 6E74656C        CMP DWORD PTR SS:[ESP+0x8],0x6C65746E
	$+192    01375E14     75 65                     JNZ SHORT PathOfEx.01375E7B
	$+194    01375E16     817C24 0C 696E6549        CMP DWORD PTR SS:[ESP+0xC],0x49656E69
	$+19C    01375E1E     75 5B                     JNZ SHORT PathOfEx.01375E7B
	$+19E    01375E20     8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+1A2    01375E24     25 000F0000               AND EAX,0xF00
	$+1A7    01375E29     3D 000F0000               CMP EAX,0xF00
	$+1AC    01375E2E     8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+1B0    01375E32     0F9405 BB036C01           SETE BYTE PTR DS:[0x16C03BB]
	$+1B7    01375E39     C1E8 08                   SHR EAX,0x8
	$+1BA    01375E3C     C1EA 1E                   SHR EDX,0x1E
	$+1BD    01375E3F     0FB6C8                    MOVZX ECX,AL
	$+1C0    01375E42     80E2 01                   AND DL,0x1
	$+1C3    01375E45     C1E1 03                   SHL ECX,0x3
	$+1C6    01375E48     833C24 07                 CMP DWORD PTR SS:[ESP],0x7
	$+1CA    01375E4C     890D 003A6901             MOV DWORD PTR DS:[0x1693A00],ECX
	$+1D0    01375E52     8815 BC036C01             MOV BYTE PTR DS:[0x16C03BC],DL
	$+1D6    01375E58     72 27                     JB SHORT PathOfEx.01375E81
	$+1D8    01375E5A     8D4424 30                 LEA EAX,DWORD PTR SS:[ESP+0x30]
	$+1DC    01375E5E     50                        PUSH EAX
	$+1DD    01375E5F     6A 07                     PUSH 0x7
	$+1DF    01375E61     E8 DAFDFFFF               CALL PathOfEx.01375C40
	$+1E4    01375E66     83C4 08                   ADD ESP,0x8
	$+1E7    01375E69     84C0                      TEST AL,AL
	$+1E9    01375E6B     74 0E                     JE SHORT PathOfEx.01375E7B
	$+1EB    01375E6D     8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
	$+1EF    01375E71     C1E8 12                   SHR EAX,0x12
	$+1F2    01375E74     24 01                     AND AL,0x1
	$+1F4    01375E76     A2 BD036C01               MOV BYTE PTR DS:[0x16C03BD],AL
	$+1F9    01375E7B     8B0D 003A6901             MOV ECX,DWORD PTR DS:[0x1693A00]
	$+1FF    01375E81     85C9                      TEST ECX,ECX
	$+201    01375E83     B8 20000000               MOV EAX,0x20
	$+206    01375E88     0F44C8                    CMOVE ECX,EAX
	$+209    01375E8B     890D 003A6901             MOV DWORD PTR DS:[0x1693A00],ECX
	$+20F    01375E91     C605 B4036C01 01          MOV BYTE PTR DS:[0x16C03B4],0x1
	$+216    01375E98     83C4 40                   ADD ESP,0x40
	$+219    01375E9B     C3                        RETN
	$+21A    01375E9C     3D 41757468               CMP EAX,0x68747541
	$+21F    01375EA1   ^ 75 D8                     JNZ SHORT PathOfEx.01375E7B
	$+221    01375EA3     817C24 08 656E7469        CMP DWORD PTR SS:[ESP+0x8],0x69746E65
	$+229    01375EAB   ^ 75 CE                     JNZ SHORT PathOfEx.01375E7B
	$+22B    01375EAD     817C24 0C 63414D44        CMP DWORD PTR SS:[ESP+0xC],0x444D4163
	$+233    01375EB5   ^ 75 C4                     JNZ SHORT PathOfEx.01375E7B
	$+235    01375EB7     8D0424                    LEA EAX,DWORD PTR SS:[ESP]
	$+238    01375EBA     50                        PUSH EAX
	$+239    01375EBB     68 05000080               PUSH 0x80000005
	$+23E    01375EC0     E8 7BFDFFFF               CALL PathOfEx.01375C40
	$+243    01375EC5     8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+247    01375EC9     83C4 08                   ADD ESP,0x8
	$+24A    01375ECC     0FB6C8                    MOVZX ECX,AL
	$+24D    01375ECF     C1E8 1E                   SHR EAX,0x1E
	$+250    01375ED2     24 01                     AND AL,0x1
	$+252    01375ED4     A2 BC036C01               MOV BYTE PTR DS:[0x16C03BC],AL
	$+257    01375ED9   ^ EB A6                     JMP SHORT PathOfEx.01375E81
	$+259    01375EDB     CC                        INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("0f??8b????89??89????89????89????c7????????????83??????75??33", 2);
#else
	addr_tmp = scan_exe_.ScanAddr("0f??8b????89??89????89????89????c7????????????83", 2);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_cpuid8_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-1AA    012323FF     CC                        INT3
	$-1A9    01232400     55                        PUSH EBP
	$-1A8    01232401     8BEC                      MOV EBP,ESP
	$-1A6    01232403     83E4 F8                   AND ESP,0xFFFFFFF8
	$-1A3    01232406     83EC 10                   SUB ESP,0x10
	$-1A0    01232409     56                        PUSH ESI
	$-19F    0123240A     8BF1                      MOV ESI,ECX
	$-19D    0123240C     57                        PUSH EDI
	$-19C    0123240D     8B7D 0C                   MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-199    01232410     8B86 CC000000             MOV EAX,DWORD PTR DS:[ESI+0xCC]
	$-193    01232416     2B86 C8000000             SUB EAX,DWORD PTR DS:[ESI+0xC8]
	$-18D    0123241C     3BF8                      CMP EDI,EAX
	$-18B    0123241E     76 27                     JBE SHORT PathOfEx.01232447
	$-189    01232420     57                        PUSH EDI
	$-188    01232421     E8 8A060000               CALL PathOfEx.01232AB0
	$-183    01232426     84C0                      TEST AL,AL
	$-181    01232428     75 1D                     JNZ SHORT PathOfEx.01232447
	$-17F    0123242A     6A 02                     PUSH 0x2
	$-17D    0123242C     FF36                      PUSH DWORD PTR DS:[ESI]
	$-17B    0123242E     FF15 60758701             CALL DWORD PTR DS:[<&WS2_32.#22>]                    ; ws2_32.shutdown
	$-175    01232434     66:C786 B0000000 0000     MOV WORD PTR DS:[ESI+0xB0],0x0
	$-16C    0123243D     8BC6                      MOV EAX,ESI
	$-16A    0123243F     5F                        POP EDI
	$-169    01232440     5E                        POP ESI
	$-168    01232441     8BE5                      MOV ESP,EBP
	$-166    01232443     5D                        POP EBP
	$-165    01232444     C2 0800                   RETN 0x8
	$-162    01232447     8B8E CC000000             MOV ECX,DWORD PTR DS:[ESI+0xCC]
	$-15C    0123244D     8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$-156    01232453     2B8E C8000000             SUB ECX,DWORD PTR DS:[ESI+0xC8]
	$-150    01232459     03C7                      ADD EAX,EDI
	$-14E    0123245B     3BC1                      CMP EAX,ECX
	$-14C    0123245D     76 3E                     JBE SHORT PathOfEx.0123249D
	$-14A    0123245F     6A 00                     PUSH 0x0
	$-148    01232461     8BCE                      MOV ECX,ESI
	$-146    01232463     E8 B8000000               CALL PathOfEx.01232520
	$-141    01232468     8B8E B4000000             MOV ECX,DWORD PTR DS:[ESI+0xB4]
	$-13B    0123246E     8B86 CC000000             MOV EAX,DWORD PTR DS:[ESI+0xCC]
	$-135    01232474     03CF                      ADD ECX,EDI
	$-133    01232476     2B86 C8000000             SUB EAX,DWORD PTR DS:[ESI+0xC8]
	$-12D    0123247C     3BC8                      CMP ECX,EAX
	$-12B    0123247E     76 1D                     JBE SHORT PathOfEx.0123249D
	$-129    01232480     51                        PUSH ECX
	$-128    01232481     8BCE                      MOV ECX,ESI
	$-126    01232483     E8 28060000               CALL PathOfEx.01232AB0
	$-121    01232488     84C0                      TEST AL,AL
	$-11F    0123248A     75 11                     JNZ SHORT PathOfEx.0123249D
	$-11D    0123248C     3886 B3000000             CMP BYTE PTR DS:[ESI+0xB3],AL
	$-117    01232492     75 32                     JNZ SHORT PathOfEx.012324C6
	$-115    01232494     6A 0A                     PUSH 0xA
	$-113    01232496     8BCE                      MOV ECX,ESI
	$-111    01232498     E8 83000000               CALL PathOfEx.01232520
	$-10C    0123249D     8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$-106    012324A3     0386 C8000000             ADD EAX,DWORD PTR DS:[ESI+0xC8]
	$-100    012324A9     57                        PUSH EDI
	$-FF     012324AA     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
	$-FC     012324AD     50                        PUSH EAX
	$-FB     012324AE     E8 CD865400               CALL PathOfEx.0177AB80
	$-F6     012324B3     83C4 0C                   ADD ESP,0xC
	$-F3     012324B6     8BC6                      MOV EAX,ESI
	$-F1     012324B8     01BE B4000000             ADD DWORD PTR DS:[ESI+0xB4],EDI
	$-EB     012324BE     5F                        POP EDI
	$-EA     012324BF     5E                        POP ESI
	$-E9     012324C0     8BE5                      MOV ESP,EBP
	$-E7     012324C2     5D                        POP EBP
	$-E6     012324C3     C2 0800                   RETN 0x8
	$-E3     012324C6     56                        PUSH ESI
	$-E2     012324C7     8D4C24 0C                 LEA ECX,DWORD PTR SS:[ESP+0xC]
	$-DE     012324CB     E8 50F4FFFF               CALL PathOfEx.01231920
	$-D9     012324D0     68 DC2DAC01               PUSH PathOfEx.01AC2DDC
	$-D4     012324D5     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-D0     012324D9     50                        PUSH EAX
	$-CF     012324DA     E8 C48F5400               CALL PathOfEx.0177B4A3
	$-CA     012324DF     CC                        INT3
	$-C9     012324E0     56                        PUSH ESI
	$-C8     012324E1     8B7424 08                 MOV ESI,DWORD PTR SS:[ESP+0x8]
	$-C4     012324E5     0F57C0                    XORPS XMM0,XMM0
	$-C1     012324E8     57                        PUSH EDI
	$-C0     012324E9     8BF9                      MOV EDI,ECX
	$-BE     012324EB     8D47 04                   LEA EAX,DWORD PTR DS:[EDI+0x4]
	$-BB     012324EE     50                        PUSH EAX
	$-BA     012324EF     C707 54F68C01             MOV DWORD PTR DS:[EDI],PathOfEx.018CF654
	$-B4     012324F5     660FD600                  MOVQ QWORD PTR DS:[EAX],XMM0
	$-B0     012324F9     8D46 04                   LEA EAX,DWORD PTR DS:[ESI+0x4]
	$-AD     012324FC     50                        PUSH EAX
	$-AC     012324FD     E8 D27E5400               CALL PathOfEx.0177A3D4
	$-A7     01232502     83C4 08                   ADD ESP,0x8
	$-A4     01232505     C707 0C8D9C01             MOV DWORD PTR DS:[EDI],PathOfEx.019C8D0C
	$-9E     0123250B     8B46 0C                   MOV EAX,DWORD PTR DS:[ESI+0xC]
	$-9B     0123250E     8947 0C                   MOV DWORD PTR DS:[EDI+0xC],EAX
	$-98     01232511     8BC7                      MOV EAX,EDI
	$-96     01232513     5F                        POP EDI
	$-95     01232514     5E                        POP ESI
	$-94     01232515     C2 0400                   RETN 0x4
	$-91     01232518     CC                        INT3
	$-90     01232519     CC                        INT3
	$-8F     0123251A     CC                        INT3
	$-8E     0123251B     CC                        INT3
	$-8D     0123251C     CC                        INT3
	$-8C     0123251D     CC                        INT3
	$-8B     0123251E     CC                        INT3
	$-8A     0123251F     CC                        INT3
	$-89     01232520     55                        PUSH EBP                                             ; g_c_SendPacket_
	$-88     01232521     8D6C24 C0                 LEA EBP,DWORD PTR SS:[ESP-0x40]
	$-84     01232525     83EC 40                   SUB ESP,0x40                                         ; 发包
	$-81     01232528     6A FF                     PUSH -0x1
	$-7F     0123252A     68 DB918301               PUSH PathOfEx.018391DB
	$-7A     0123252F     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-74     01232535     50                        PUSH EAX
	$-73     01232536     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-6C     0123253D     81EC 64020000             SUB ESP,0x264
	$-66     01232543     53                        PUSH EBX
	$-65     01232544     56                        PUSH ESI
	$-64     01232545     8BF1                      MOV ESI,ECX
	$-62     01232547     C745 3C 00000000          MOV DWORD PTR SS:[EBP+0x3C],0x0
	$-5B     0123254E     57                        PUSH EDI
	$-5A     0123254F     8BBE B4000000             MOV EDI,DWORD PTR DS:[ESI+0xB4]
	$-54     01232555     8B9E B8000000             MOV EBX,DWORD PTR DS:[ESI+0xB8]
	$-4E     0123255B     2BFB                      SUB EDI,EBX
	$-4C     0123255D     8B8E 90000000             MOV ECX,DWORD PTR DS:[ESI+0x90]
	$-46     01232563     85C9                      TEST ECX,ECX
	$-44     01232565     74 13                     JE SHORT PathOfEx.0123257A
	$-42     01232567     85FF                      TEST EDI,EDI
	$-40     01232569     74 0F                     JE SHORT PathOfEx.0123257A
	$-3E     0123256B     8B86 C8000000             MOV EAX,DWORD PTR DS:[ESI+0xC8]
	$-38     01232571     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$-36     01232573     03C3                      ADD EAX,EBX
	$-34     01232575     57                        PUSH EDI
	$-33     01232576     50                        PUSH EAX
	$-32     01232577     FF52 04                   CALL DWORD PTR DS:[EDX+0x4]
	$-2F     0123257A     8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$-29     01232580     33FF                      XOR EDI,EDI
	$-27     01232582     8986 B8000000             MOV DWORD PTR DS:[ESI+0xB8],EAX
	$-21     01232588     85C0                      TEST EAX,EAX
	$-1F     0123258A     74 36                     JE SHORT PathOfEx.012325C2
	$-1D     0123258C     8B1D 64758701             MOV EBX,DWORD PTR DS:[<&WS2_32.#19>]                 ; ws2_32.send
	$-17     01232592     8B8E C8000000             MOV ECX,DWORD PTR DS:[ESI+0xC8]
	$-11     01232598     8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$-B      0123259E     6A 00                     PUSH 0x0
	$-9      012325A0     2BC7                      SUB EAX,EDI
	$-7      012325A2     50                        PUSH EAX
	$-6      012325A3     8D0439                    LEA EAX,DWORD PTR DS:[ECX+EDI]
	$-3      012325A6     50                        PUSH EAX
	$-2      012325A7     FF36                      PUSH DWORD PTR DS:[ESI]
	$ ==>    012325A9     FFD3                      CALL EBX                                             ; call send,g_h_send_
	$+2      012325AB     83F8 FF                   CMP EAX,-0x1
	$+5      012325AE     74 7B                     JE SHORT PathOfEx.0123262B
	$+7      012325B0     8B8E 8C000000             MOV ECX,DWORD PTR DS:[ESI+0x8C]
	$+D      012325B6     03F8                      ADD EDI,EAX
	$+F      012325B8     0101                      ADD DWORD PTR DS:[ECX],EAX
	$+11     012325BA     3BBE B4000000             CMP EDI,DWORD PTR DS:[ESI+0xB4]
	$+17     012325C0   ^ 72 D0                     JB SHORT PathOfEx.01232592
	$+19     012325C2     C786 B4000000 00000000    MOV DWORD PTR DS:[ESI+0xB4],0x0
	$+23     012325CC     C786 B8000000 00000000    MOV DWORD PTR DS:[ESI+0xB8],0x0
	$+2D     012325D6     837E 54 00                CMP DWORD PTR DS:[ESI+0x54],0x0
	$+31     012325DA     74 39                     JE SHORT PathOfEx.01232615
	$+33     012325DC     C745 24 00000000          MOV DWORD PTR SS:[EBP+0x24],0x0
	$+3A     012325E3     8D45 00                   LEA EAX,DWORD PTR SS:[EBP]
	$+3D     012325E6     C745 FC 0A000000          MOV DWORD PTR SS:[EBP-0x4],0xA
	$+44     012325ED     50                        PUSH EAX
	$+45     012325EE     8D4E 30                   LEA ECX,DWORD PTR DS:[ESI+0x30]
	$+48     012325F1     E8 5AA6CEFF               CALL PathOfEx.00F1CC50
	$+4D     012325F6     C745 FC 0B000000          MOV DWORD PTR SS:[EBP-0x4],0xB
	$+54     012325FD     8B4D 24                   MOV ECX,DWORD PTR SS:[EBP+0x24]
	$+57     01232600     85C9                      TEST ECX,ECX
	$+59     01232602     74 11                     JE SHORT PathOfEx.01232615
	$+5B     01232604     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$+5D     01232606     8D45 00                   LEA EAX,DWORD PTR SS:[EBP]
	$+60     01232609     3BC8                      CMP ECX,EAX
	$+62     0123260B     0F95C0                    SETNE AL
	$+65     0123260E     0FB6C0                    MOVZX EAX,AL
	$+68     01232611     50                        PUSH EAX
	$+69     01232612     FF52 10                   CALL DWORD PTR DS:[EDX+0x10]
	$+6C     01232615     5F                        POP EDI
	$+6D     01232616     8BC6                      MOV EAX,ESI
	$+6F     01232618     5E                        POP ESI
	$+70     01232619     5B                        POP EBX
	$+71     0123261A     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+74     0123261D     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+7B     01232624     8D65 40                   LEA ESP,DWORD PTR SS:[EBP+0x40]
	$+7E     01232627     5D                        POP EBP
	$+7F     01232628     C2 0400                   RETN 0x4
	$+82     0123262B     8B1D 78758701             MOV EBX,DWORD PTR DS:[<&WS2_32.#111>]                ; ws2_32.WSAGetLastError
	$+88     01232631     FFD3                      CALL EBX
	$+8A     01232633     3D 33270000               CMP EAX,0x2733
	$+8F     01232638     0F85 45030000             JNZ PathOfEx.01232983
	$+95     0123263E     85FF                      TEST EDI,EDI
	$+97     01232640     74 28                     JE SHORT PathOfEx.0123266A
	$+99     01232642     8B8E C8000000             MOV ECX,DWORD PTR DS:[ESI+0xC8]
	$+9F     01232648     8B86 B4000000             MOV EAX,DWORD PTR DS:[ESI+0xB4]
	$+A5     0123264E     2BC7                      SUB EAX,EDI
	$+A7     01232650     50                        PUSH EAX
	$+A8     01232651     8D0439                    LEA EAX,DWORD PTR DS:[ECX+EDI]
	$+AB     01232654     50                        PUSH EAX
	$+AC     01232655     51                        PUSH ECX
	$+AD     01232656     E8 05785400               CALL PathOfEx.01779E60
	$+B2     0123265B     29BE B4000000             SUB DWORD PTR DS:[ESI+0xB4],EDI
	$+B8     01232661     83C4 0C                   ADD ESP,0xC
	$+BB     01232664     29BE B8000000             SUB DWORD PTR DS:[ESI+0xB8],EDI
	$+C1     0123266A     8B7D 48                   MOV EDI,DWORD PTR SS:[EBP+0x48]
	$+C4     0123266D     85FF                      TEST EDI,EDI
	$+C6     0123266F     0F8E E3020000             JLE PathOfEx.01232958
	$+CC     01232675     80BE B3000000 00          CMP BYTE PTR DS:[ESI+0xB3],0x0
	$+D3     0123267C     74 17                     JE SHORT PathOfEx.01232695
	$+D5     0123267E     56                        PUSH ESI
	$+D6     0123267F     8D4D 30                   LEA ECX,DWORD PTR SS:[EBP+0x30]
	$+D9     01232682     E8 99F2FFFF               CALL PathOfEx.01231920
	$+DE     01232687     68 DC2DAC01               PUSH PathOfEx.01AC2DDC
	$+E3     0123268C     8D45 30                   LEA EAX,DWORD PTR SS:[EBP+0x30]
	$+E6     0123268F     50                        PUSH EAX
	$+E7     01232690     E8 0E8E5400               CALL PathOfEx.0177B4A3
	$+EC     01232695     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+EE     01232697     8985 94FDFFFF             MOV DWORD PTR SS:[EBP-0x26C],EAX
	$+F4     0123269D     C785 90FDFFFF 01000000    MOV DWORD PTR SS:[EBP-0x270],0x1
	$+FE     012326A7     897D 28                   MOV DWORD PTR SS:[EBP+0x28],EDI
	$+101    012326AA     8D48 01                   LEA ECX,DWORD PTR DS:[EAX+0x1]
	$+104    012326AD     C745 2C 00000000          MOV DWORD PTR SS:[EBP+0x2C],0x0
	$+10B    012326B4     8D45 28                   LEA EAX,DWORD PTR SS:[EBP+0x28]
	$+10E    012326B7     50                        PUSH EAX
	$+10F    012326B8     6A 00                     PUSH 0x0
	$+111    012326BA     8D85 90FDFFFF             LEA EAX,DWORD PTR SS:[EBP-0x270]
	$+117    012326C0     50                        PUSH EAX
	$+118    012326C1     6A 00                     PUSH 0x0
	$+11A    012326C3     51                        PUSH ECX
	$+11B    012326C4     FF15 70758701             CALL DWORD PTR DS:[<&WS2_32.#18>]                    ; ws2_32.select
	$+121    012326CA     83F8 FF                   CMP EAX,-0x1
	$+124    012326CD     0F85 2A010000             JNZ PathOfEx.012327FD
	$+12A    012326D3     FFD3                      CALL EBX
	$+12C    012326D5     3D 14270000               CMP EAX,0x2714
	$+131    012326DA     0F84 5C020000             JE PathOfEx.0123293C
	$+137    012326E0     C785 44FFFFFF 182B9901    MOV DWORD PTR SS:[EBP-0xBC],PathOfEx.01992B18
	$+141    012326EA     C785 54FFFFFF A0C99901    MOV DWORD PTR SS:[EBP-0xAC],PathOfEx.0199C9A0
	$+14B    012326F4     C745 AC 940B8D01          MOV DWORD PTR SS:[EBP-0x54],PathOfEx.018D0B94
	$+152    012326FB     51                        PUSH ECX
	$+153    012326FC     8D85 5CFFFFFF             LEA EAX,DWORD PTR SS:[EBP-0xA4]
	$+159    01232702     C745 FC 05000000          MOV DWORD PTR SS:[EBP-0x4],0x5
	$+160    01232709     50                        PUSH EAX
	$+161    0123270A     8D8D 44FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0xBC]
	$+167    01232710     C745 3C 02000000          MOV DWORD PTR SS:[EBP+0x3C],0x2
	$+16E    01232717     E8 7426CFFF               CALL PathOfEx.00F24D90
	$+173    0123271C     C745 FC 06000000          MOV DWORD PTR SS:[EBP-0x4],0x6
	$+17A    01232723     8B85 44FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xBC]
	$+180    01232729     6A 03                     PUSH 0x3
	$+182    0123272B     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+185    0123272E     C78405 44FFFFFF 400D9601  MOV DWORD PTR SS:[EBP+EAX-0xBC],PathOfEx.01960D40
	$+190    01232739     8B85 44FFFFFF             MOV EAX,DWORD PTR SS:[EBP-0xBC]
	$+196    0123273F     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+199    01232742     8D41 98                   LEA EAX,DWORD PTR DS:[ECX-0x68]
	$+19C    01232745     89840D 40FFFFFF           MOV DWORD PTR SS:[EBP+ECX-0xC0],EAX
	$+1A3    0123274C     8D8D 5CFFFFFF             LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$+1A9    01232752     E8 192CCFFF               CALL PathOfEx.00F25370
	$+1AE    01232757     C745 FC 07000000          MOV DWORD PTR SS:[EBP-0x4],0x7
	$+1B5    0123275E     FFD3                      CALL EBX
	$+1B7    01232760     50                        PUSH EAX
	$+1B8    01232761     8D85 54FFFFFF             LEA EAX,DWORD PTR SS:[EBP-0xAC]
	$+1BE    01232767     68 BC8C9C01               PUSH PathOfEx.019C8CBC
	$+1C3    0123276C     50                        PUSH EAX
	$+1C4    0123276D     E8 CE08DBFF               CALL PathOfEx.00FE3040
	$+1C9    01232772     83C4 08                   ADD ESP,0x8
	$+1CC    01232775     8BC8                      MOV ECX,EAX
	$+1CE    01232777     E8 F430D0FF               CALL PathOfEx.00F35870
	$+1D3    0123277C     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]
	$+1D6    0123277F     50                        PUSH EAX
	$+1D7    01232780     8D8D 44FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0xBC]
	$+1DD    01232786     E8 2536D0FF               CALL PathOfEx.00F35DB0
	$+1E2    0123278B     8BF8                      MOV EDI,EAX
	$+1E4    0123278D     C645 FC 08                MOV BYTE PTR SS:[EBP-0x4],0x8
	$+1E8    01232791     FF15 D8708701             CALL DWORD PTR DS:[<&KERNEL32.GetTickCount>]         ; kernel32.GetTickCount
	$+1EE    01232797     8BD8                      MOV EBX,EAX
	$+1F0    01232799     8D45 38                   LEA EAX,DWORD PTR SS:[EBP+0x38]
	$+1F3    0123279C     50                        PUSH EAX
	$+1F4    0123279D     E8 25D85500               CALL PathOfEx.0178FFC7
	$+1F9    012327A2     64:8B0D 2C000000          MOV ECX,DWORD PTR FS:[0x2C]
	$+200    012327A9     83C4 04                   ADD ESP,0x4
	$+203    012327AC     8B09                      MOV ECX,DWORD PTR DS:[ECX]
	$+205    012327AE     57                        PUSH EDI
	$+206    012327AF     83EC 0C                   SUB ESP,0xC
	$+209    012327B2     8B49 1C                   MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$+20C    012327B5     6A 02                     PUSH 0x2
	$+20E    012327B7     53                        PUSH EBX
	$+20F    012327B8     FF75 3C                   PUSH DWORD PTR SS:[EBP+0x3C]
	$+212    012327BB     FF75 38                   PUSH DWORD PTR SS:[EBP+0x38]
	$+215    012327BE     68 69020000               PUSH 0x269
	$+21A    012327C3     E8 8832D0FF               CALL PathOfEx.00F35A50
	$+21F    012327C8     8B45 24                   MOV EAX,DWORD PTR SS:[EBP+0x24]
	$+222    012327CB     83F8 08                   CMP EAX,0x8
	$+225    012327CE     72 0F                     JB SHORT PathOfEx.012327DF
	$+227    012327D0     40                        INC EAX
	$+228    012327D1     6A 02                     PUSH 0x2
	$+22A    012327D3     50                        PUSH EAX
	$+22B    012327D4     FF75 10                   PUSH DWORD PTR SS:[EBP+0x10]
	$+22E    012327D7     E8 D468CEFF               CALL PathOfEx.00F190B0
	$+233    012327DC     83C4 0C                   ADD ESP,0xC
	$+236    012327DF     8D4D AC                   LEA ECX,DWORD PTR SS:[EBP-0x54]
	$+239    012327E2     E8 590ECFFF               CALL PathOfEx.00F23640
	$+23E    012327E7     C745 FC 09000000          MOV DWORD PTR SS:[EBP-0x4],0x9
	$+245    012327EE     8D45 AC                   LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+248    012327F1     C745 AC 0C0B8D01          MOV DWORD PTR SS:[EBP-0x54],PathOfEx.018D0B0C
	$+24F    012327F8     E9 1E010000               JMP PathOfEx.0123291B
	$+254    012327FD     85C0                      TEST EAX,EAX
	$+256    012327FF     0F85 37010000             JNZ PathOfEx.0123293C
	$+25C    01232805     C785 94FEFFFF 182B9901    MOV DWORD PTR SS:[EBP-0x16C],PathOfEx.01992B18
	$+266    0123280F     C785 A4FEFFFF A0C99901    MOV DWORD PTR SS:[EBP-0x15C],PathOfEx.0199C9A0
	$+270    01232819     C785 FCFEFFFF 940B8D01    MOV DWORD PTR SS:[EBP-0x104],PathOfEx.018D0B94
	$+27A    01232823     8945 FC                   MOV DWORD PTR SS:[EBP-0x4],EAX
	$+27D    01232826     8D85 ACFEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x154]
	$+283    0123282C     51                        PUSH ECX
	$+284    0123282D     50                        PUSH EAX
	$+285    0123282E     8D8D 94FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x16C]
	$+28B    01232834     C745 3C 01000000          MOV DWORD PTR SS:[EBP+0x3C],0x1
	$+292    0123283B     E8 5025CFFF               CALL PathOfEx.00F24D90
	$+297    01232840     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+29E    01232847     8B85 94FEFFFF             MOV EAX,DWORD PTR SS:[EBP-0x16C]
	$+2A4    0123284D     6A 03                     PUSH 0x3
	$+2A6    0123284F     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+2A9    01232852     C78405 94FEFFFF 400D9601  MOV DWORD PTR SS:[EBP+EAX-0x16C],PathOfEx.01960D40
	$+2B4    0123285D     8B85 94FEFFFF             MOV EAX,DWORD PTR SS:[EBP-0x16C]
	$+2BA    01232863     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+2BD    01232866     8D41 98                   LEA EAX,DWORD PTR DS:[ECX-0x68]
	$+2C0    01232869     89840D 90FEFFFF           MOV DWORD PTR SS:[EBP+ECX-0x170],EAX
	$+2C7    01232870     8D8D ACFEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x154]
	$+2CD    01232876     E8 F52ACFFF               CALL PathOfEx.00F25370
	$+2D2    0123287B     8D85 A4FEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x15C]
	$+2D8    01232881     C745 FC 02000000          MOV DWORD PTR SS:[EBP-0x4],0x2
	$+2DF    01232888     68 748C9C01               PUSH PathOfEx.019C8C74                               ; ASCII 53,"elect timed out in a blocking call."
	$+2E4    0123288D     50                        PUSH EAX
	$+2E5    0123288E     E8 AD07DBFF               CALL PathOfEx.00FE3040
	$+2EA    01232893     83C4 08                   ADD ESP,0x8
	$+2ED    01232896     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]
	$+2F0    01232899     8D8D 94FEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x16C]
	$+2F6    0123289F     50                        PUSH EAX
	$+2F7    012328A0     E8 0B35D0FF               CALL PathOfEx.00F35DB0
	$+2FC    012328A5     8BF8                      MOV EDI,EAX
	$+2FE    012328A7     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+302    012328AB     FF15 D8708701             CALL DWORD PTR DS:[<&KERNEL32.GetTickCount>]         ; kernel32.GetTickCount
	$+308    012328B1     8BD8                      MOV EBX,EAX
	$+30A    012328B3     8D45 38                   LEA EAX,DWORD PTR SS:[EBP+0x38]
	$+30D    012328B6     50                        PUSH EAX
	$+30E    012328B7     E8 0BD75500               CALL PathOfEx.0178FFC7
	$+313    012328BC     64:8B0D 2C000000          MOV ECX,DWORD PTR FS:[0x2C]
	$+31A    012328C3     83C4 04                   ADD ESP,0x4
	$+31D    012328C6     8B09                      MOV ECX,DWORD PTR DS:[ECX]
	$+31F    012328C8     57                        PUSH EDI
	$+320    012328C9     83EC 0C                   SUB ESP,0xC
	$+323    012328CC     8B49 1C                   MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$+326    012328CF     6A 02                     PUSH 0x2
	$+328    012328D1     53                        PUSH EBX
	$+329    012328D2     FF75 3C                   PUSH DWORD PTR SS:[EBP+0x3C]
	$+32C    012328D5     FF75 38                   PUSH DWORD PTR SS:[EBP+0x38]
	$+32F    012328D8     68 67020000               PUSH 0x267
	$+334    012328DD     E8 6E31D0FF               CALL PathOfEx.00F35A50
	$+339    012328E2     8B45 24                   MOV EAX,DWORD PTR SS:[EBP+0x24]
	$+33C    012328E5     83F8 08                   CMP EAX,0x8
	$+33F    012328E8     72 0F                     JB SHORT PathOfEx.012328F9
	$+341    012328EA     40                        INC EAX
	$+342    012328EB     6A 02                     PUSH 0x2
	$+344    012328ED     50                        PUSH EAX
	$+345    012328EE     FF75 10                   PUSH DWORD PTR SS:[EBP+0x10]
	$+348    012328F1     E8 BA67CEFF               CALL PathOfEx.00F190B0
	$+34D    012328F6     83C4 0C                   ADD ESP,0xC
	$+350    012328F9     8D8D FCFEFFFF             LEA ECX,DWORD PTR SS:[EBP-0x104]
	$+356    012328FF     E8 3C0DCFFF               CALL PathOfEx.00F23640
	$+35B    01232904     C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4],0x4
	$+362    0123290B     8D85 FCFEFFFF             LEA EAX,DWORD PTR SS:[EBP-0x104]
	$+368    01232911     C785 FCFEFFFF 0C0B8D01    MOV DWORD PTR SS:[EBP-0x104],PathOfEx.018D0B0C
	$+372    0123291B     50                        PUSH EAX
	$+373    0123291C     E8 A3CD5100               CALL PathOfEx.0174F6C4
	$+378    01232921     83C4 04                   ADD ESP,0x4
	$+37B    01232924     6A 02                     PUSH 0x2
	$+37D    01232926     FF36                      PUSH DWORD PTR DS:[ESI]
	$+37F    01232928     FF15 60758701             CALL DWORD PTR DS:[<&WS2_32.#22>]                    ; ws2_32.shutdown
	$+385    0123292E     66:C786 B0000000 0000     MOV WORD PTR DS:[ESI+0xB0],0x0
	$+38E    01232937   ^ E9 D9FCFFFF               JMP PathOfEx.01232615
	$+393    0123293C     57                        PUSH EDI
	$+394    0123293D     8BCE                      MOV ECX,ESI
	$+396    0123293F     E8 DCFBFFFF               CALL PathOfEx.01232520
	$+39B    01232944     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+39E    01232947     5F                        POP EDI
	$+39F    01232948     5E                        POP ESI
	$+3A0    01232949     5B                        POP EBX
	$+3A1    0123294A     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+3A8    01232951     8D65 40                   LEA ESP,DWORD PTR SS:[EBP+0x40]
	$+3AB    01232954     5D                        POP EBP
	$+3AC    01232955     C2 0400                   RETN 0x4
	$+3AF    01232958     83BE 88000000 00          CMP DWORD PTR DS:[ESI+0x88],0x0
	$+3B6    0123295F   ^ 0F84 B0FCFFFF             JE PathOfEx.01232615
	$+3BC    01232965     837E 54 00                CMP DWORD PTR DS:[ESI+0x54],0x0
	$+3C0    01232969   ^ 0F85 A6FCFFFF             JNZ PathOfEx.01232615
	$+3C6    0123296F     8D45 48                   LEA EAX,DWORD PTR SS:[EBP+0x48]
	$+3C9    01232972     8975 48                   MOV DWORD PTR SS:[EBP+0x48],ESI
	$+3CC    01232975     50                        PUSH EAX
	$+3CD    01232976     8D4E 30                   LEA ECX,DWORD PTR DS:[ESI+0x30]
	$+3D0    01232979     E8 22060000               CALL PathOfEx.01232FA0
	$+3D5    0123297E   ^ E9 92FCFFFF               JMP PathOfEx.01232615
	$+3DA    01232983     C786 B4000000 00000000    MOV DWORD PTR DS:[ESI+0xB4],0x0
	$+3E4    0123298D     C786 B8000000 00000000    MOV DWORD PTR DS:[ESI+0xB8],0x0
	$+3EE    01232997     C686 B0000000 00          MOV BYTE PTR DS:[ESI+0xB0],0x0
	$+3F5    0123299E   ^ E9 72FCFFFF               JMP PathOfEx.01232615
	$+3FA    012329A3     CC                        INT3
	$+3FB    012329A4     CC                        INT3
	$+3FC    012329A5     CC                        INT3
	$+3FD    012329A6     CC                        INT3
	$+3FE    012329A7     CC                        INT3
	$+3FF    012329A8     CC                        INT3
	$+400    012329A9     CC                        INT3
	$+401    012329AA     CC                        INT3
	$+402    012329AB     CC                        INT3
	$+403    012329AC     CC                        INT3
	$+404    012329AD     CC                        INT3
	$+405    012329AE     CC                        INT3
	$+406    012329AF     CC                        INT3
	$+407    012329B0     55                        PUSH EBP
	$+408    012329B1     8BEC                      MOV EBP,ESP
	$+40A    012329B3     6A FF                     PUSH -0x1
	$+40C    012329B5     68 F8918301               PUSH PathOfEx.018391F8
	$+411    012329BA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+417    012329C0     50                        PUSH EAX
	$+418    012329C1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+41F    012329C8     83EC 08                   SUB ESP,0x8
	$+422    012329CB     53                        PUSH EBX
	$+423    012329CC     56                        PUSH ESI
	$+424    012329CD     57                        PUSH EDI
	$+425    012329CE     8BF9                      MOV EDI,ECX
	$+427    012329D0     8D9F 90000000             LEA EBX,DWORD PTR DS:[EDI+0x90]
	$+42D    012329D6     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+434    012329DD     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+437    012329E0     3BD8                      CMP EBX,EAX
	$+439    012329E2     74 18                     JE SHORT PathOfEx.012329FC
	$+43B    012329E4     8B0B                      MOV ECX,DWORD PTR DS:[EBX]
	$+43D    012329E6     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+440    012329E9     C745 08 00000000          MOV DWORD PTR SS:[EBP+0x8],0x0
	$+447    012329F0     8903                      MOV DWORD PTR DS:[EBX],EAX
	$+449    012329F2     85C9                      TEST ECX,ECX
	$+44B    012329F4     74 06                     JE SHORT PathOfEx.012329FC
	$+44D    012329F6     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+44F    012329F8     6A 01                     PUSH 0x1
	$+451    012329FA     FF10                      CALL DWORD PTR DS:[EAX]
	$+453    012329FC     8B87 B4000000             MOV EAX,DWORD PTR DS:[EDI+0xB4]
	$+459    01232A02     33D2                      XOR EDX,EDX
	$+45B    01232A04     8BB7 C4000000             MOV ESI,DWORD PTR DS:[EDI+0xC4]
	$+461    01232A0A     8987 B8000000             MOV DWORD PTR DS:[EDI+0xB8],EAX
	$+467    01232A10     8B87 BC000000             MOV EAX,DWORD PTR DS:[EDI+0xBC]
	$+46D    01232A16     8B8F D8000000             MOV ECX,DWORD PTR DS:[EDI+0xD8]
	$+473    01232A1C     03C6                      ADD EAX,ESI
	$+475    01232A1E     2B8F D4000000             SUB ECX,DWORD PTR DS:[EDI+0xD4]
	$+47B    01232A24     F7F1                      DIV ECX
	$+47D    01232A26     8B87 C0000000             MOV EAX,DWORD PTR DS:[EDI+0xC0]
	$+483    01232A2C     8B8F D8000000             MOV ECX,DWORD PTR DS:[EDI+0xD8]
	$+489    01232A32     2BC6                      SUB EAX,ESI
	$+48B    01232A34     2B8F D4000000             SUB ECX,DWORD PTR DS:[EDI+0xD4]
	$+491    01232A3A     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+494    01232A3D     03C2                      ADD EAX,EDX
	$+496    01232A3F     3BC1                      CMP EAX,ECX
	$+498    01232A41     8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+49B    01232A44     8B0B                      MOV ECX,DWORD PTR DS:[EBX]
	$+49D    01232A46     8B87 D4000000             MOV EAX,DWORD PTR DS:[EDI+0xD4]
	$+4A3    01232A4C     77 0F                     JA SHORT PathOfEx.01232A5D
	$+4A5    01232A4E     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$+4A7    01232A50     8B5D EC                   MOV EBX,DWORD PTR SS:[EBP-0x14]
	$+4AA    01232A53     0345 F0                   ADD EAX,DWORD PTR SS:[EBP-0x10]
	$+4AD    01232A56     53                        PUSH EBX
	$+4AE    01232A57     50                        PUSH EAX
	$+4AF    01232A58     FF52 08                   CALL DWORD PTR DS:[EDX+0x8]
	$+4B2    01232A5B     EB 2B                     JMP SHORT PathOfEx.01232A88
	$+4B4    01232A5D     8BB7 D8000000             MOV ESI,DWORD PTR DS:[EDI+0xD8]
	$+4BA    01232A63     2BB7 D4000000             SUB ESI,DWORD PTR DS:[EDI+0xD4]
	$+4C0    01232A69     0345 F0                   ADD EAX,DWORD PTR SS:[EBP-0x10]
	$+4C3    01232A6C     2BF2                      SUB ESI,EDX
	$+4C5    01232A6E     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$+4C7    01232A70     56                        PUSH ESI
	$+4C8    01232A71     50                        PUSH EAX
	$+4C9    01232A72     FF52 08                   CALL DWORD PTR DS:[EDX+0x8]
	$+4CC    01232A75     8B0B                      MOV ECX,DWORD PTR DS:[EBX]
	$+4CE    01232A77     8B5D EC                   MOV EBX,DWORD PTR SS:[EBP-0x14]
	$+4D1    01232A7A     2BDE                      SUB EBX,ESI
	$+4D3    01232A7C     53                        PUSH EBX
	$+4D4    01232A7D     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+4D6    01232A7F     FFB7 D4000000             PUSH DWORD PTR DS:[EDI+0xD4]
	$+4DC    01232A85     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$+4DF    01232A88     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+4E2    01232A8B     5F                        POP EDI
	$+4E3    01232A8C     5E                        POP ESI
	$+4E4    01232A8D     5B                        POP EBX
	$+4E5    01232A8E     85C9                      TEST ECX,ECX
	$+4E7    01232A90     74 06                     JE SHORT PathOfEx.01232A98
	$+4E9    01232A92     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+4EB    01232A94     6A 01                     PUSH 0x1
	$+4ED    01232A96     FF10                      CALL DWORD PTR DS:[EAX]
	$+4EF    01232A98     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+4F2    01232A9B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+4F9    01232AA2     8BE5                      MOV ESP,EBP
	$+4FB    01232AA4     5D                        POP EBP
	$+4FC    01232AA5     C2 0400                   RETN 0x4
	$+4FF    01232AA8     CC                        INT3
	$+500    01232AA9     CC                        INT3
	$+501    01232AAA     CC                        INT3
	$+502    01232AAB     CC                        INT3
	$+503    01232AAC     CC                        INT3
	$+504    01232AAD     CC                        INT3
	$+505    01232AAE     CC                        INT3
	$+506    01232AAF     CC                        INT3
	$+507    01232AB0     8B81 CC000000             MOV EAX,DWORD PTR DS:[ECX+0xCC]
	$+50D    01232AB6     81C1 C8000000             ADD ECX,0xC8
	$+513    01232ABC     8B5424 04                 MOV EDX,DWORD PTR SS:[ESP+0x4]
	$+517    01232AC0     2B01                      SUB EAX,DWORD PTR DS:[ECX]
	$+519    01232AC2     3BD0                      CMP EDX,EAX
	$+51B    01232AC4     77 05                     JA SHORT PathOfEx.01232ACB
	$+51D    01232AC6     32C0                      XOR AL,AL
	$+51F    01232AC8     C2 0400                   RETN 0x4
	$+522    01232ACB     81FA 00002000             CMP EDX,0x200000
	$+528    01232AD1   ^ 77 F3                     JA SHORT PathOfEx.01232AC6
	$+52A    01232AD3     4A                        DEC EDX
	$+52B    01232AD4     8BC2                      MOV EAX,EDX
	$+52D    01232AD6     D1E8                      SHR EAX,1
	$+52F    01232AD8     0BD0                      OR EDX,EAX
	$+531    01232ADA     8BC2                      MOV EAX,EDX
	$+533    01232ADC     C1E8 02                   SHR EAX,0x2
	$+536    01232ADF     0BD0                      OR EDX,EAX
	$+538    01232AE1     8BC2                      MOV EAX,EDX
	$+53A    01232AE3     C1E8 04                   SHR EAX,0x4
	$+53D    01232AE6     0BD0                      OR EDX,EAX
	$+53F    01232AE8     8BC2                      MOV EAX,EDX
	$+541    01232AEA     C1E8 08                   SHR EAX,0x8
	$+544    01232AED     0BD0                      OR EDX,EAX
	$+546    01232AEF     8BC2                      MOV EAX,EDX
	$+548    01232AF1     C1E8 10                   SHR EAX,0x10
	$+54B    01232AF4     0BC2                      OR EAX,EDX
	$+54D    01232AF6     BA 00002000               MOV EDX,0x200000
	$+552    01232AFB     40                        INC EAX
	$+553    01232AFC     3BC2                      CMP EAX,EDX
	$+555    01232AFE     0F47C2                    CMOVA EAX,EDX
	$+558    01232B01     50                        PUSH EAX
	$+559    01232B02     E8 D971F6FF               CALL PathOfEx.01199CE0
	$+55E    01232B07     B0 01                     MOV AL,0x1
	$+560    01232B09     C2 0400                   RETN 0x4
	$+563    01232B0C     CC                        INT3
	*/
	/*addr_tmp = scan_exe_.ScanAddr("ff??83????74??8b??????????03??01??3b??????????72??c7??????????????????c7??????????????????83", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_send_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;*/

	/*
	$-CF     00CB467F     CC                 INT3
	$-CE     00CB4680     55                 PUSH EBP
	$-CD     00CB4681     8BEC               MOV EBP,ESP
	$-CB     00CB4683     6A FF              PUSH -0x1
	$-C9     00CB4685     68 8C914801        PUSH PathOfEx.0148918C
	$-C4     00CB468A     64:A1 00000000     MOV EAX,DWORD PTR FS:[0]
	$-BE     00CB4690     50                 PUSH EAX
	$-BD     00CB4691     64:8925 00000000   MOV DWORD PTR FS:[0],ESP
	$-B6     00CB4698     83EC 2C            SUB ESP,0x2C
	$-B3     00CB469B     53                 PUSH EBX
	$-B2     00CB469C     56                 PUSH ESI
	$-B1     00CB469D     8BD9               MOV EBX,ECX
	$-AF     00CB469F     8D4D E0            LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-AC     00CB46A2     57                 PUSH EDI
	$-AB     00CB46A3     E8 7859EEFF        CALL PathOfEx.00B9A020
	$-A6     00CB46A8     51                 PUSH ECX
	$-A5     00CB46A9     8D4D E8            LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-A2     00CB46AC     51                 PUSH ECX
	$-A1     00CB46AD     8B08               MOV ECX,DWORD PTR DS:[EAX]
	$-9F     00CB46AF     E8 FCCFEEFF        CALL PathOfEx.00BA16B0
	$-9A     00CB46B4     C745 FC 01000000   MOV DWORD PTR SS:[EBP-0x4],0x1
	$-93     00CB46BB     83CF FF            OR EDI,0xFFFFFFFF
	$-90     00CB46BE     8B75 E4            MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-8D     00CB46C1     85F6               TEST ESI,ESI
	$-8B     00CB46C3     74 1F              JE SHORT PathOfEx.00CB46E4
	$-89     00CB46C5     8BC7               MOV EAX,EDI
	$-87     00CB46C7     F0:0FC146 04       LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$-82     00CB46CC     75 16              JNZ SHORT PathOfEx.00CB46E4
	$-80     00CB46CE     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$-7E     00CB46D0     8BCE               MOV ECX,ESI
	$-7C     00CB46D2     FF10               CALL DWORD PTR DS:[EAX]
	$-7A     00CB46D4     8BC7               MOV EAX,EDI
	$-78     00CB46D6     F0:0FC146 08       LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
	$-73     00CB46DB     75 07              JNZ SHORT PathOfEx.00CB46E4
	$-71     00CB46DD     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$-6F     00CB46DF     8BCE               MOV ECX,ESI
	$-6D     00CB46E1     FF50 04            CALL DWORD PTR DS:[EAX+0x4]
	$-6A     00CB46E4     C645 FC 00         MOV BYTE PTR SS:[EBP-0x4],0x0
	$-66     00CB46E8     8B45 E8            MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-63     00CB46EB     83B8 F8180000 FF   CMP DWORD PTR DS:[EAX+0x18F8],-0x1
	$-5C     00CB46F2     74 24              JE SHORT PathOfEx.00CB4718
	$-5A     00CB46F4     8B8B 64090000      MOV ECX,DWORD PTR DS:[EBX+0x964]
	$-54     00CB46FA     6A FF              PUSH -0x1
	$-52     00CB46FC     6A 00              PUSH 0x0
	$-50     00CB46FE     6A 00              PUSH 0x0
	$-4E     00CB4700     FF75 08            PUSH DWORD PTR SS:[EBP+0x8]
	$-4B     00CB4703     FFB3 BC080000      PUSH DWORD PTR DS:[EBX+0x8BC]
	$-45     00CB4709     FFB3 B8080000      PUSH DWORD PTR DS:[EBX+0x8B8]
	$-3F     00CB470F     6A 03              PUSH 0x3
	$-3D     00CB4711     E8 2A23FFFF        CALL PathOfEx.00CA6A40
	$-38     00CB4716     EB 48              JMP SHORT PathOfEx.00CB4760
	$-36     00CB4718     8B83 BC080000      MOV EAX,DWORD PTR DS:[EBX+0x8BC]
	$-30     00CB471E     B9 16000000        MOV ECX,0x16
	$-2B     00CB4723     8BB3 4C090000      MOV ESI,DWORD PTR DS:[EBX+0x94C]
	$-25     00CB4729     66:894D D0         MOV WORD PTR SS:[EBP-0x30],CX
	$-21     00CB472D     C745 CC F4836501   MOV DWORD PTR SS:[EBP-0x34],PathOfEx.016583F4
	$-1A     00CB4734     C645 FC 02         MOV BYTE PTR SS:[EBP-0x4],0x2
	$-16     00CB4738     8D4D CC            LEA ECX,DWORD PTR SS:[EBP-0x34]
	$-13     00CB473B     FFB6 98380000      PUSH DWORD PTR DS:[ESI+0x3898]
	$-D      00CB4741     8945 D4            MOV DWORD PTR SS:[EBP-0x2C],EAX
	$-A      00CB4744     8B45 08            MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-7      00CB4747     8945 D8            MOV DWORD PTR SS:[EBP-0x28],EAX
	$-4      00CB474A     C645 DC 00         MOV BYTE PTR SS:[EBP-0x24],0x0
	$ ==>    00CB474E     E8 9D1C5400        CALL PathOfEx.011F63F0                          ; call tag_构建拿起物品发包
	$+5      00CB4753     8B8E 98380000      MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+B      00CB4759     6A 00              PUSH 0x0
	$+D      00CB475B     E8 C0DD1E00        CALL PathOfEx.00EA2520                          ; TakeupItem发包
	$+12     00CB4760     C745 FC 03000000   MOV DWORD PTR SS:[EBP-0x4],0x3
	$+19     00CB4767     8B75 EC            MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+1C     00CB476A     85F6               TEST ESI,ESI
	$+1E     00CB476C     74 1E              JE SHORT PathOfEx.00CB478C
	$+20     00CB476E     8BC7               MOV EAX,EDI
	$+22     00CB4770     F0:0FC146 04       LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+27     00CB4775     75 15              JNZ SHORT PathOfEx.00CB478C
	$+29     00CB4777     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+2B     00CB4779     8BCE               MOV ECX,ESI
	$+2D     00CB477B     FF10               CALL DWORD PTR DS:[EAX]
	$+2F     00CB477D     F0:0FC17E 08       LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+34     00CB4782     4F                 DEC EDI
	$+35     00CB4783     75 07              JNZ SHORT PathOfEx.00CB478C
	$+37     00CB4785     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+39     00CB4787     8BCE               MOV ECX,ESI
	$+3B     00CB4789     FF50 04            CALL DWORD PTR DS:[EAX+0x4]
	$+3E     00CB478C     8B4D F4            MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+41     00CB478F     5F                 POP EDI
	$+42     00CB4790     5E                 POP ESI
	$+43     00CB4791     64:890D 00000000   MOV DWORD PTR FS:[0],ECX
	$+4A     00CB4798     5B                 POP EBX
	$+4B     00CB4799     8BE5               MOV ESP,EBP
	$+4D     00CB479B     5D                 POP EBP
	$+4E     00CB479C     C2 0400            RETN 0x4
	$+51     00CB479F     CC                 INT3
	$+52     00CB47A0     55                 PUSH EBP
	$+53     00CB47A1     8BEC               MOV EBP,ESP
	$+55     00CB47A3     64:A1 00000000     MOV EAX,DWORD PTR FS:[0]
	$+5B     00CB47A9     6A FF              PUSH -0x1
	$+5D     00CB47AB     68 74E34701        PUSH PathOfEx.0147E374
	$+62     00CB47B0     50                 PUSH EAX
	$+63     00CB47B1     64:8925 00000000   MOV DWORD PTR FS:[0],ESP
	$+6A     00CB47B8     83EC 1C            SUB ESP,0x1C
	$+6D     00CB47BB     53                 PUSH EBX
	$+6E     00CB47BC     8BD9               MOV EBX,ECX
	$+70     00CB47BE     80BB AF0A0000 00   CMP BYTE PTR DS:[EBX+0xAAF],0x0
	$+77     00CB47C5     0F85 5A010000      JNZ PathOfEx.00CB4925
	$+7D     00CB47CB     56                 PUSH ESI
	$+7E     00CB47CC     57                 PUSH EDI
	$+7F     00CB47CD     8D4D D8            LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+82     00CB47D0     E8 4B58EEFF        CALL PathOfEx.00B9A020
	$+87     00CB47D5     51                 PUSH ECX
	$+88     00CB47D6     8D4D E0            LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+8B     00CB47D9     51                 PUSH ECX
	$+8C     00CB47DA     8B08               MOV ECX,DWORD PTR DS:[EAX]
	$+8E     00CB47DC     E8 CFCEEEFF        CALL PathOfEx.00BA16B0
	$+93     00CB47E1     C745 FC 01000000   MOV DWORD PTR SS:[EBP-0x4],0x1
	$+9A     00CB47E8     83CF FF            OR EDI,0xFFFFFFFF
	$+9D     00CB47EB     8B75 DC            MOV ESI,DWORD PTR SS:[EBP-0x24]
	$+A0     00CB47EE     85F6               TEST ESI,ESI
	$+A2     00CB47F0     74 1F              JE SHORT PathOfEx.00CB4811
	$+A4     00CB47F2     8BC7               MOV EAX,EDI
	$+A6     00CB47F4     F0:0FC146 04       LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+AB     00CB47F9     75 16              JNZ SHORT PathOfEx.00CB4811
	$+AD     00CB47FB     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+AF     00CB47FD     8BCE               MOV ECX,ESI
	$+B1     00CB47FF     FF10               CALL DWORD PTR DS:[EAX]
	$+B3     00CB4801     8BC7               MOV EAX,EDI
	$+B5     00CB4803     F0:0FC146 08       LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
	$+BA     00CB4808     75 07              JNZ SHORT PathOfEx.00CB4811
	$+BC     00CB480A     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+BE     00CB480C     8BCE               MOV ECX,ESI
	$+C0     00CB480E     FF50 04            CALL DWORD PTR DS:[EAX+0x4]
	$+C3     00CB4811     C645 FC 00         MOV BYTE PTR SS:[EBP-0x4],0x0
	$+C7     00CB4815     8B45 E0            MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+CA     00CB4818     83B8 F8180000 FF   CMP DWORD PTR DS:[EAX+0x18F8],-0x1
	$+D1     00CB481F     0F85 D2000000      JNZ PathOfEx.00CB48F7
	$+D7     00CB4825     8B80 E0180000      MOV EAX,DWORD PTR DS:[EAX+0x18E0]
	$+DD     00CB482B     8BB3 50090000      MOV ESI,DWORD PTR DS:[EBX+0x950]
	$+E3     00CB4831     8B4D 08            MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+E6     00CB4834     894D F0            MOV DWORD PTR SS:[EBP-0x10],ECX
	$+E9     00CB4837     8B80 B40A0000      MOV EAX,DWORD PTR DS:[EAX+0xAB4]
	$+EF     00CB483D     8945 E8            MOV DWORD PTR SS:[EBP-0x18],EAX
	$+F2     00CB4840     8D4E 2C            LEA ECX,DWORD PTR DS:[ESI+0x2C]
	$+F5     00CB4843     8D45 F0            LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+F8     00CB4846     50                 PUSH EAX
	$+F9     00CB4847     8D45 EC            LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+FC     00CB484A     50                 PUSH EAX
	$+FD     00CB484B     E8 30620000        CALL PathOfEx.00CBAA80
	$+102    00CB4850     8B45 EC            MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+105    00CB4853     3B46 2C            CMP EAX,DWORD PTR DS:[ESI+0x2C]
	$+108    00CB4856     75 04              JNZ SHORT PathOfEx.00CB485C
	$+10A    00CB4858     33C9               XOR ECX,ECX
	$+10C    00CB485A     EB 05              JMP SHORT PathOfEx.00CB4861
	$+10E    00CB485C     8B40 14            MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+111    00CB485F     8B08               MOV ECX,DWORD PTR DS:[EAX]
	$+113    00CB4861     E8 AADAF3FF        CALL PathOfEx.00BF2310
	$+118    00CB4866     85C0               TEST EAX,EAX
	$+11A    00CB4868     0F84 89000000      JE PathOfEx.00CB48F7
	$+120    00CB486E     8B48 08            MOV ECX,DWORD PTR DS:[EAX+0x8]
	$+123    00CB4871     8D75 F0            LEA ESI,DWORD PTR SS:[EBP-0x10]
	$+126    00CB4874     8B40 0C            MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+129    00CB4877     8945 F0            MOV DWORD PTR SS:[EBP-0x10],EAX
	$+12C    00CB487A     8B51 14            MOV EDX,DWORD PTR DS:[ECX+0x14]
	$+12F    00CB487D     3BD0               CMP EDX,EAX
	$+131    00CB487F     8D4D EC            LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+134    00CB4882     8955 EC            MOV DWORD PTR SS:[EBP-0x14],EDX
	$+137    00CB4885     0F43CE             CMOVNB ECX,ESI
	$+13A    00CB4888     8B01               MOV EAX,DWORD PTR DS:[ECX]
	$+13C    00CB488A     8945 EC            MOV DWORD PTR SS:[EBP-0x14],EAX
	$+13F    00CB488D     83F8 01            CMP EAX,0x1
	$+142    00CB4890     76 65              JBE SHORT PathOfEx.00CB48F7
	$+144    00CB4892     FF75 08            PUSH DWORD PTR SS:[EBP+0x8]
	$+147    00CB4895     8BCB               MOV ECX,EBX
	$+149    00CB4897     E8 B49FFFFF        CALL PathOfEx.00CAE850
	$+14E    00CB489C     8BF0               MOV ESI,EAX
	$+150    00CB489E     85F6               TEST ESI,ESI
	$+152    00CB48A0     74 55              JE SHORT PathOfEx.00CB48F7
	$+154    00CB48A2     8B4D E8            MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+157    00CB48A5     6A 00              PUSH 0x0
	$+159    00CB48A7     FF75 EC            PUSH DWORD PTR SS:[EBP-0x14]
	$+15C    00CB48AA     FF75 08            PUSH DWORD PTR SS:[EBP+0x8]
	$+15F    00CB48AD     FFB3 BC080000      PUSH DWORD PTR DS:[EBX+0x8BC]
	$+165    00CB48B3     E8 98060900        CALL PathOfEx.00D44F50
	$+16A    00CB48B8     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+16C    00CB48BA     8BCE               MOV ECX,ESI
	$+16E    00CB48BC     8B40 3C            MOV EAX,DWORD PTR DS:[EAX+0x3C]
	$+171    00CB48BF     FFD0               CALL EAX
	$+173    00CB48C1     D80D B4056A01      FMUL DWORD PTR DS:[0x16A05B4]
	$+179    00CB48C7     8BCE               MOV ECX,ESI
	$+17B    00CB48C9     D95D 08            FSTP DWORD PTR SS:[EBP+0x8]
	$+17E    00CB48CC     E8 0F534500        CALL PathOfEx.01109BE0
	$+183    00CB48D1     8BCE               MOV ECX,ESI
	$+185    00CB48D3     0F28D8             MOVAPS XMM3,XMM0
	$+188    00CB48D6     E8 55524500        CALL PathOfEx.01109B30
	$+18D    00CB48DB     F3:0F5845 08       ADDSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+192    00CB48E0     8B4D E8            MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+195    00CB48E3     0F28D3             MOVAPS XMM2,XMM3
	$+198    00CB48E6     0F28C8             MOVAPS XMM1,XMM0
	$+19B    00CB48E9     E8 92070900        CALL PathOfEx.00D45080
	$+1A0    00CB48EE     6A 00              PUSH 0x0
	$+1A2    00CB48F0     8BCE               MOV ECX,ESI
	$+1A4    00CB48F2     E8 195F4500        CALL PathOfEx.0110A810
	$+1A9    00CB48F7     C745 FC 02000000   MOV DWORD PTR SS:[EBP-0x4],0x2
	$+1B0    00CB48FE     8B75 E4            MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+1B3    00CB4901     85F6               TEST ESI,ESI
	$+1B5    00CB4903     74 1E              JE SHORT PathOfEx.00CB4923
	$+1B7    00CB4905     8BC7               MOV EAX,EDI
	$+1B9    00CB4907     F0:0FC146 04       LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+1BE    00CB490C     75 15              JNZ SHORT PathOfEx.00CB4923
	$+1C0    00CB490E     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+1C2    00CB4910     8BCE               MOV ECX,ESI
	$+1C4    00CB4912     FF10               CALL DWORD PTR DS:[EAX]
	$+1C6    00CB4914     F0:0FC17E 08       LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+1CB    00CB4919     4F                 DEC EDI
	$+1CC    00CB491A     75 07              JNZ SHORT PathOfEx.00CB4923
	$+1CE    00CB491C     8B06               MOV EAX,DWORD PTR DS:[ESI]
	$+1D0    00CB491E     8BCE               MOV ECX,ESI
	$+1D2    00CB4920     FF50 04            CALL DWORD PTR DS:[EAX+0x4]
	$+1D5    00CB4923     5F                 POP EDI
	$+1D6    00CB4924     5E                 POP ESI
	$+1D7    00CB4925     8B4D F4            MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1DA    00CB4928     5B                 POP EBX
	$+1DB    00CB4929     64:890D 00000000   MOV DWORD PTR FS:[0],ECX
	$+1E2    00CB4930     8BE5               MOV ESP,EBP
	$+1E4    00CB4932     5D                 POP EBP
	$+1E5    00CB4933     C2 0400            RETN 0x4
	$+1E8    00CB4936     CC                 INT3
	*/
	addr_tmp = scan_exe_.ScanCall("e8????????8b??????????6a??e8????????c7????????????8b????85??74??8b??f0????????75??8b??8b??ff??f0????????4f75??8b??8b??ff????8b????5f5e64", 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_build_takeup_item_packet_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//对某个箱子的stCD_Positioned::pos_字段下硬件断点，若在该箱子附近移动，就会断到。
	/*
	$-321    016F8EBF     CC                       INT3
	$-320    016F8EC0     51                       PUSH ECX
	$-31F    016F8EC1     53                       PUSH EBX
	$-31E    016F8EC2     8BD9                     MOV EBX,ECX
	$-31C    016F8EC4     8D4C24 0C                LEA ECX,DWORD PTR SS:[ESP+0xC]
	$-318    016F8EC8     55                       PUSH EBP
	$-317    016F8EC9     8B6C24 10                MOV EBP,DWORD PTR SS:[ESP+0x10]
	$-313    016F8ECD     56                       PUSH ESI
	$-312    016F8ECE     8B13                     MOV EDX,DWORD PTR DS:[EBX]
	$-310    016F8ED0     57                       PUSH EDI
	$-30F    016F8ED1     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$-30C    016F8ED4     8B75 10                  MOV ESI,DWORD PTR SS:[EBP+0x10]
	$-309    016F8ED7     2B42 04                  SUB EAX,DWORD PTR DS:[EDX+0x4]
	$-306    016F8EDA     2B32                     SUB ESI,DWORD PTR DS:[EDX]
	$-304    016F8EDC     99                       CDQ
	$-303    016F8EDD     8BF8                     MOV EDI,EAX
	$-301    016F8EDF     8BC6                     MOV EAX,ESI
	$-2FF    016F8EE1     33FA                     XOR EDI,EDX
	$-2FD    016F8EE3     8D7424 10                LEA ESI,DWORD PTR SS:[ESP+0x10]
	$-2F9    016F8EE7     2BFA                     SUB EDI,EDX
	$-2F7    016F8EE9     99                       CDQ
	$-2F6    016F8EEA     33C2                     XOR EAX,EDX
	$-2F4    016F8EEC     897C24 18                MOV DWORD PTR SS:[ESP+0x18],EDI
	$-2F0    016F8EF0     2BC2                     SUB EAX,EDX
	$-2EE    016F8EF2     8B55 28                  MOV EDX,DWORD PTR SS:[EBP+0x28]
	$-2EB    016F8EF5     3BC7                     CMP EAX,EDI
	$-2E9    016F8EF7     894424 10                MOV DWORD PTR SS:[ESP+0x10],EAX
	$-2E5    016F8EFB     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-2E2    016F8EFE     0F4DCE                   CMOVGE ECX,ESI
	$-2DF    016F8F01     8B09                     MOV ECX,DWORD PTR DS:[ECX]
	$-2DD    016F8F03     2BCA                     SUB ECX,EDX
	$-2DB    016F8F05     3908                     CMP DWORD PTR DS:[EAX],ECX
	$-2D9    016F8F07     7E 23                    JLE SHORT PathOfEx.016F8F2C
	$-2D7    016F8F09     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$-2D4    016F8F0C     8338 00                  CMP DWORD PTR DS:[EAX],0x0
	$-2D1    016F8F0F     75 09                    JNZ SHORT PathOfEx.016F8F1A
	$-2CF    016F8F11     83BD A4000000 01         CMP DWORD PTR SS:[EBP+0xA4],0x1
	$-2C8    016F8F18     74 12                    JE SHORT PathOfEx.016F8F2C
	$-2C6    016F8F1A     83FA FF                  CMP EDX,-0x1
	$-2C3    016F8F1D     74 0D                    JE SHORT PathOfEx.016F8F2C
	$-2C1    016F8F1F     8B43 0C                  MOV EAX,DWORD PTR DS:[EBX+0xC]
	$-2BE    016F8F22     3B28                     CMP EBP,DWORD PTR DS:[EAX]
	$-2BC    016F8F24     74 06                    JE SHORT PathOfEx.016F8F2C
	$-2BA    016F8F26     8B43 10                  MOV EAX,DWORD PTR DS:[EBX+0x10]
	$-2B7    016F8F29     C600 01                  MOV BYTE PTR DS:[EAX],0x1
	$-2B4    016F8F2C     5F                       POP EDI
	$-2B3    016F8F2D     5E                       POP ESI
	$-2B2    016F8F2E     5D                       POP EBP
	$-2B1    016F8F2F     5B                       POP EBX
	$-2B0    016F8F30     59                       POP ECX
	$-2AF    016F8F31     C2 0400                  RETN 0x4
	$-2AC    016F8F34     CC                       INT3
	$-2AB    016F8F35     CC                       INT3
	$-2AA    016F8F36     CC                       INT3
	$-2A9    016F8F37     CC                       INT3
	$-2A8    016F8F38     CC                       INT3
	$-2A7    016F8F39     CC                       INT3
	$-2A6    016F8F3A     CC                       INT3
	$-2A5    016F8F3B     CC                       INT3
	$-2A4    016F8F3C     CC                       INT3
	$-2A3    016F8F3D     CC                       INT3
	$-2A2    016F8F3E     CC                       INT3
	$-2A1    016F8F3F     CC                       INT3
	$-2A0    016F8F40     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-29C    016F8F44     8B5424 0C                MOV EDX,DWORD PTR SS:[ESP+0xC]
	$-298    016F8F48     8901                     MOV DWORD PTR DS:[ECX],EAX
	$-296    016F8F4A     8941 04                  MOV DWORD PTR DS:[ECX+0x4],EAX
	$-293    016F8F4D     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$-28F    016F8F51     3BC2                     CMP EAX,EDX
	$-28D    016F8F53     74 24                    JE SHORT PathOfEx.016F8F79
	$-28B    016F8F55     56                       PUSH ESI
	$-28A    016F8F56     8B71 04                  MOV ESI,DWORD PTR DS:[ECX+0x4]
	$-287    016F8F59     85F6                     TEST ESI,ESI
	$-285    016F8F5B     74 0C                    JE SHORT PathOfEx.016F8F69
	$-283    016F8F5D     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-281    016F8F5F     8906                     MOV DWORD PTR DS:[ESI],EAX
	$-27F    016F8F61     8B5424 10                MOV EDX,DWORD PTR SS:[ESP+0x10]
	$-27B    016F8F65     8B4424 0C                MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-277    016F8F69     8341 04 04               ADD DWORD PTR DS:[ECX+0x4],0x4
	$-273    016F8F6D     83C0 04                  ADD EAX,0x4
	$-270    016F8F70     894424 0C                MOV DWORD PTR SS:[ESP+0xC],EAX
	$-26C    016F8F74     3BC2                     CMP EAX,EDX
	$-26A    016F8F76   ^ 75 DE                    JNZ SHORT PathOfEx.016F8F56
	$-268    016F8F78     5E                       POP ESI
	$-267    016F8F79     8BC1                     MOV EAX,ECX
	$-265    016F8F7B     C2 0C00                  RETN 0xC
	$-262    016F8F7E     CC                       INT3
	$-261    016F8F7F     CC                       INT3
	$-260    016F8F80     83EC 7C                  SUB ESP,0x7C
	$-25D    016F8F83     A1 C09D0B02              MOV EAX,DWORD PTR DS:[0x20B9DC0]
	$-258    016F8F88     83C0 FE                  ADD EAX,-0x2
	$-255    016F8F8B     894C24 0C                MOV DWORD PTR SS:[ESP+0xC],ECX
	$-251    016F8F8F     53                       PUSH EBX
	$-250    016F8F90     55                       PUSH EBP
	$-24F    016F8F91     56                       PUSH ESI
	$-24E    016F8F92     57                       PUSH EDI
	$-24D    016F8F93     8BBC24 94000000          MOV EDI,DWORD PTR SS:[ESP+0x94]
	$-246    016F8F9A     03F8                     ADD EDI,EAX
	$-244    016F8F9C     C64424 18 00             MOV BYTE PTR SS:[ESP+0x18],0x0
	$-23F    016F8FA1     8B8424 90000000          MOV EAX,DWORD PTR SS:[ESP+0x90]
	$-238    016F8FA8     C64424 14 00             MOV BYTE PTR SS:[ESP+0x14],0x0
	$-233    016F8FAD     C74424 20 00000000       MOV DWORD PTR SS:[ESP+0x20],0x0
	$-22B    016F8FB5     C74424 24 00000000       MOV DWORD PTR SS:[ESP+0x24],0x0
	$-223    016F8FBD     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$-221    016F8FBF     8BDA                     MOV EBX,EDX
	$-21F    016F8FC1     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-21C    016F8FC4     2BDF                     SUB EBX,EDI
	$-21A    016F8FC6     8BE8                     MOV EBP,EAX
	$-218    016F8FC8     2BEF                     SUB EBP,EDI
	$-216    016F8FCA     8D3407                   LEA ESI,DWORD PTR DS:[EDI+EAX]
	$-213    016F8FCD     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$-210    016F8FD0     894424 28                MOV DWORD PTR SS:[ESP+0x28],EAX
	$-20C    016F8FD4     8B41 38                  MOV EAX,DWORD PTR DS:[ECX+0x38]
	$-209    016F8FD7     8D0C17                   LEA ECX,DWORD PTR DS:[EDI+EDX]
	$-206    016F8FDA     894424 2C                MOV DWORD PTR SS:[ESP+0x2C],EAX
	$-202    016F8FDE     B8 C94216B2              MOV EAX,0xB21642C9
	$-1FD    016F8FE3     F7E9                     IMUL ECX
	$-1FB    016F8FE5     B8 C94216B2              MOV EAX,0xB21642C9
	$-1F6    016F8FEA     03D1                     ADD EDX,ECX
	$-1F4    016F8FEC     C1FA 04                  SAR EDX,0x4
	$-1F1    016F8FEF     8BCA                     MOV ECX,EDX
	$-1EF    016F8FF1     C1E9 1F                  SHR ECX,0x1F
	$-1EC    016F8FF4     03CA                     ADD ECX,EDX
	$-1EA    016F8FF6     F7EE                     IMUL ESI
	$-1E8    016F8FF8     B8 C94216B2              MOV EAX,0xB21642C9
	$-1E3    016F8FFD     03D6                     ADD EDX,ESI
	$-1E1    016F8FFF     8D71 01                  LEA ESI,DWORD PTR DS:[ECX+0x1]
	$-1DE    016F9002     C1FA 04                  SAR EDX,0x4
	$-1DB    016F9005     8BFA                     MOV EDI,EDX
	$-1D9    016F9007     897424 44                MOV DWORD PTR SS:[ESP+0x44],ESI
	$-1D5    016F900B     42                       INC EDX
	$-1D4    016F900C     C1EF 1F                  SHR EDI,0x1F
	$-1D1    016F900F     03FA                     ADD EDI,EDX
	$-1CF    016F9011     F7EB                     IMUL EBX
	$-1CD    016F9013     B8 C94216B2              MOV EAX,0xB21642C9
	$-1C8    016F9018     897C24 48                MOV DWORD PTR SS:[ESP+0x48],EDI
	$-1C4    016F901C     03D3                     ADD EDX,EBX
	$-1C2    016F901E     C1FA 04                  SAR EDX,0x4
	$-1BF    016F9021     8BCA                     MOV ECX,EDX
	$-1BD    016F9023     C1E9 1F                  SHR ECX,0x1F
	$-1BA    016F9026     03CA                     ADD ECX,EDX
	$-1B8    016F9028     F7ED                     IMUL EBP
	$-1B6    016F902A     894C24 3C                MOV DWORD PTR SS:[ESP+0x3C],ECX
	$-1B2    016F902E     8D4C24 3C                LEA ECX,DWORD PTR SS:[ESP+0x3C]
	$-1AE    016F9032     03D5                     ADD EDX,EBP
	$-1AC    016F9034     C1FA 04                  SAR EDX,0x4
	$-1A9    016F9037     8BC2                     MOV EAX,EDX
	$-1A7    016F9039     C1E8 1F                  SHR EAX,0x1F
	$-1A4    016F903C     03C2                     ADD EAX,EDX
	$-1A2    016F903E     894424 40                MOV DWORD PTR SS:[ESP+0x40],EAX
	$-19E    016F9042     8D4424 20                LEA EAX,DWORD PTR SS:[ESP+0x20]
	$-19A    016F9046     50                       PUSH EAX
	$-199    016F9047     8D4424 58                LEA EAX,DWORD PTR SS:[ESP+0x58]
	$-195    016F904B     50                       PUSH EAX
	$-194    016F904C     E8 DF77F2FF              CALL PathOfEx.01620830
	$-18F    016F9051     0F104C24 54              MOVUPS XMM1,DQWORD PTR SS:[ESP+0x54]
	$-18A    016F9056     8B5424 54                MOV EDX,DWORD PTR SS:[ESP+0x54]
	$-186    016F905A     8B4C24 60                MOV ECX,DWORD PTR SS:[ESP+0x60]
	$-182    016F905E     8B4424 58                MOV EAX,DWORD PTR SS:[ESP+0x58]
	$-17E    016F9062     0F28C1                   MOVAPS XMM0,XMM1
	$-17B    016F9065     660F73D8 08              PSRLDQ XMM0,0x8
	$-176    016F906A     66:0F7EC6                MOVD ESI,MM0
	$-172    016F906E     895424 4C                MOV DWORD PTR SS:[ESP+0x4C],EDX
	$-16E    016F9072     894C24 50                MOV DWORD PTR SS:[ESP+0x50],ECX
	$-16A    016F9076     3BD6                     CMP EDX,ESI
	$-168    016F9078     75 0A                    JNZ SHORT PathOfEx.016F9084
	$-166    016F907A     897424 4C                MOV DWORD PTR SS:[ESP+0x4C],ESI
	$-162    016F907E     894C24 50                MOV DWORD PTR SS:[ESP+0x50],ECX
	$-15E    016F9082     EB 0C                    JMP SHORT PathOfEx.016F9090
	$-15C    016F9084     3BC1                     CMP EAX,ECX
	$-15A    016F9086     75 08                    JNZ SHORT PathOfEx.016F9090
	$-158    016F9088     895424 4C                MOV DWORD PTR SS:[ESP+0x4C],EDX
	$-154    016F908C     894424 50                MOV DWORD PTR SS:[ESP+0x50],EAX
	$-150    016F9090     895424 64                MOV DWORD PTR SS:[ESP+0x64],EDX
	$-14C    016F9094     894424 68                MOV DWORD PTR SS:[ESP+0x68],EAX
	$-148    016F9098     3BD6                     CMP EDX,ESI
	$-146    016F909A     75 0A                    JNZ SHORT PathOfEx.016F90A6
	$-144    016F909C     897424 64                MOV DWORD PTR SS:[ESP+0x64],ESI
	$-140    016F90A0     894C24 68                MOV DWORD PTR SS:[ESP+0x68],ECX
	$-13C    016F90A4     EB 0C                    JMP SHORT PathOfEx.016F90B2
	$-13A    016F90A6     3BC1                     CMP EAX,ECX
	$-138    016F90A8     75 08                    JNZ SHORT PathOfEx.016F90B2
	$-136    016F90AA     895424 64                MOV DWORD PTR SS:[ESP+0x64],EDX
	$-132    016F90AE     894424 68                MOV DWORD PTR SS:[ESP+0x68],EAX
	$-12E    016F90B2     8D8424 98000000          LEA EAX,DWORD PTR SS:[ESP+0x98]
	$-127    016F90B9     83EC 18                  SUB ESP,0x18
	$-124    016F90BC     894424 38                MOV DWORD PTR SS:[ESP+0x38],EAX
	$-120    016F90C0     8D8C24 84000000          LEA ECX,DWORD PTR SS:[ESP+0x84]
	$-119    016F90C7     8B4424 34                MOV EAX,DWORD PTR SS:[ESP+0x34]
	$-115    016F90CB     894424 3C                MOV DWORD PTR SS:[ESP+0x3C],EAX
	$-111    016F90CF     8B8424 A8000000          MOV EAX,DWORD PTR SS:[ESP+0xA8]
	$-10A    016F90D6     894424 40                MOV DWORD PTR SS:[ESP+0x40],EAX
	$-106    016F90DA     8D8424 AC000000          LEA EAX,DWORD PTR SS:[ESP+0xAC]
	$-FF     016F90E1     894424 44                MOV DWORD PTR SS:[ESP+0x44],EAX
	$-FB     016F90E5     8D8424 B8000000          LEA EAX,DWORD PTR SS:[ESP+0xB8]
	$-F4     016F90EC     0F104424 38              MOVUPS XMM0,DQWORD PTR SS:[ESP+0x38]
	$-EF     016F90F1     894424 48                MOV DWORD PTR SS:[ESP+0x48],EAX
	$-EB     016F90F5     8D4424 2C                LEA EAX,DWORD PTR SS:[ESP+0x2C]
	$-E7     016F90F9     894424 4C                MOV DWORD PTR SS:[ESP+0x4C],EAX
	$-E3     016F90FD     8D4424 30                LEA EAX,DWORD PTR SS:[ESP+0x30]
	$-DF     016F9101     0F118424 84000000        MOVUPS DQWORD PTR SS:[ESP+0x84],XMM0
	$-D7     016F9109     898424 9C000000          MOV DWORD PTR SS:[ESP+0x9C],EAX
	$-D0     016F9110     8BC4                     MOV EAX,ESP
	$-CE     016F9112     F30F7E4424 48            MOVQ XMM0,QWORD PTR SS:[ESP+0x48]
	$-C8     016F9118     83EC 18                  SUB ESP,0x18
	$-C5     016F911B     660FD68424 AC000000      MOVQ QWORD PTR SS:[ESP+0xAC],XMM0
	$-BC     016F9124     F30F7E4424 7C            MOVQ XMM0,QWORD PTR SS:[ESP+0x7C]
	$-B6     016F912A     0F1108                   MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-B3     016F912D     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-AE     016F9132     8BC4                     MOV EAX,ESP
	$-AC     016F9134     F30F7E8424 94000000      MOVQ XMM0,QWORD PTR SS:[ESP+0x94]
	$-A3     016F913D     0F1108                   MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-A0     016F9140     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-9B     016F9145     E8 26030000              CALL PathOfEx.016F9470
	$-96     016F914A     83C4 30                  ADD ESP,0x30
	$-93     016F914D     5F                       POP EDI
	$-92     016F914E     5E                       POP ESI
	$-91     016F914F     5D                       POP EBP
	$-90     016F9150     5B                       POP EBX
	$-8F     016F9151     83C4 7C                  ADD ESP,0x7C
	$-8C     016F9154     C2 1800                  RETN 0x18
	$-89     016F9157     CC                       INT3
	$-88     016F9158     CC                       INT3
	$-87     016F9159     CC                       INT3
	$-86     016F915A     CC                       INT3
	$-85     016F915B     CC                       INT3
	$-84     016F915C     CC                       INT3
	$-83     016F915D     CC                       INT3
	$-82     016F915E     CC                       INT3
	$-81     016F915F     CC                       INT3
	$-80     016F9160     83EC 18                  SUB ESP,0x18
	$-7D     016F9163     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-7B     016F9165     8B51 04                  MOV EDX,DWORD PTR DS:[ECX+0x4]
	$-78     016F9168     56                       PUSH ESI
	$-77     016F9169     57                       PUSH EDI
	$-76     016F916A     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-74     016F916C     8D7C24 24                LEA EDI,DWORD PTR SS:[ESP+0x24]
	$-70     016F9170     894424 08                MOV DWORD PTR SS:[ESP+0x8],EAX
	$-6C     016F9174     8B4424 24                MOV EAX,DWORD PTR SS:[ESP+0x24]
	$-68     016F9178     8B72 34                  MOV ESI,DWORD PTR DS:[EDX+0x34]
	$-65     016F917B     0FAF70 04                IMUL ESI,DWORD PTR DS:[EAX+0x4]
	$-61     016F917F     0330                     ADD ESI,DWORD PTR DS:[EAX]
	$-5F     016F9181     8B41 08                  MOV EAX,DWORD PTR DS:[ECX+0x8]
	$-5C     016F9184     894424 0C                MOV DWORD PTR SS:[ESP+0xC],EAX
	$-58     016F9188     8B41 0C                  MOV EAX,DWORD PTR DS:[ECX+0xC]
	$-55     016F918B     894424 10                MOV DWORD PTR SS:[ESP+0x10],EAX
	$-51     016F918F     8D4424 08                LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-4D     016F9193     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$-49     016F9197     8B41 10                  MOV EAX,DWORD PTR DS:[ECX+0x10]
	$-46     016F919A     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$-42     016F919E     8B41 18                  MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-3F     016F91A1     0F104424 0C              MOVUPS XMM0,DQWORD PTR SS:[ESP+0xC]
	$-3A     016F91A6     C1E6 05                  SHL ESI,0x5
	$-37     016F91A9     0372 24                  ADD ESI,DWORD PTR DS:[EDX+0x24]
	$-34     016F91AC     33D2                     XOR EDX,EDX
	$-32     016F91AE     C64424 24 00             MOV BYTE PTR SS:[ESP+0x24],0x0
	$-2D     016F91B3     0FB600                   MOVZX EAX,BYTE PTR DS:[EAX]
	$-2A     016F91B6     50                       PUSH EAX
	$-29     016F91B7     8B41 14                  MOV EAX,DWORD PTR DS:[ECX+0x14]
	$-26     016F91BA     8BCE                     MOV ECX,ESI
	$-24     016F91BC     3810                     CMP BYTE PTR DS:[EAX],DL
	$-22     016F91BE     0F95C2                   SETNE DL
	$-1F     016F91C1     52                       PUSH EDX
	$-1E     016F91C2     83EC 14                  SUB ESP,0x14
	$-1B     016F91C5     8BC4                     MOV EAX,ESP
	$-19     016F91C7     0F1100                   MOVUPS DQWORD PTR DS:[EAX],XMM0
	$-16     016F91CA     8978 10                  MOV DWORD PTR DS:[EAX+0x10],EDI
	$-13     016F91CD     E8 2E010000              CALL PathOfEx.016F9300
	$-E      016F91D2     8A4424 24                MOV AL,BYTE PTR SS:[ESP+0x24]
	$-A      016F91D6     5F                       POP EDI
	$-9      016F91D7     5E                       POP ESI
	$-8      016F91D8     83C4 18                  ADD ESP,0x18
	$-5      016F91DB     C2 0400                  RETN 0x4
	$-2      016F91DE     CC                       INT3
	$-1      016F91DF     CC                       INT3
	$ ==>    016F91E0     83EC 08                  SUB ESP,0x8                                 ; tag_判断是否产生了碰撞
	$+3      016F91E3     53                       PUSH EBX
	$+4      016F91E4     8BD9                     MOV EBX,ECX
	$+6      016F91E6     8D4C24 10                LEA ECX,DWORD PTR SS:[ESP+0x10]
	$+A      016F91EA     55                       PUSH EBP
	$+B      016F91EB     8B6C24 14                MOV EBP,DWORD PTR SS:[ESP+0x14]
	$+F      016F91EF     56                       PUSH ESI
	$+10     016F91F0     8B13                     MOV EDX,DWORD PTR DS:[EBX]
	$+12     016F91F2     57                       PUSH EDI
	$+13     016F91F3     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$+16     016F91F6     8B75 10                  MOV ESI,DWORD PTR SS:[EBP+0x10]
	$+19     016F91F9     2B42 04                  SUB EAX,DWORD PTR DS:[EDX+0x4]
	$+1C     016F91FC     2B32                     SUB ESI,DWORD PTR DS:[EDX]
	$+1E     016F91FE     99                       CDQ
	$+1F     016F91FF     8BF8                     MOV EDI,EAX
	$+21     016F9201     8BC6                     MOV EAX,ESI
	$+23     016F9203     33FA                     XOR EDI,EDX
	$+25     016F9205     8D7424 14                LEA ESI,DWORD PTR SS:[ESP+0x14]
	$+29     016F9209     2BFA                     SUB EDI,EDX
	$+2B     016F920B     99                       CDQ
	$+2C     016F920C     33C2                     XOR EAX,EDX
	$+2E     016F920E     897C24 1C                MOV DWORD PTR SS:[ESP+0x1C],EDI
	$+32     016F9212     2BC2                     SUB EAX,EDX
	$+34     016F9214     8B55 28                  MOV EDX,DWORD PTR SS:[EBP+0x28]
	$+37     016F9217     3BC7                     CMP EAX,EDI
	$+39     016F9219     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$+3D     016F921D     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+40     016F9220     0F4DCE                   CMOVGE ECX,ESI
	$+43     016F9223     8B09                     MOV ECX,DWORD PTR DS:[ECX]
	$+45     016F9225     2BCA                     SUB ECX,EDX
	$+47     016F9227     3908                     CMP DWORD PTR DS:[EAX],ECX
	$+49     016F9229     7E 29                    JLE SHORT PathOfEx.016F9254
	$+4B     016F922B     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+4E     016F922E     8338 00                  CMP DWORD PTR DS:[EAX],0x0
	$+51     016F9231     75 09                    JNZ SHORT PathOfEx.016F923C
	$+53     016F9233     83BD A4000000 01         CMP DWORD PTR SS:[EBP+0xA4],0x1
	$+5A     016F923A     74 18                    JE SHORT PathOfEx.016F9254
	$+5C     016F923C     83FA FF                  CMP EDX,-0x1
	$+5F     016F923F     74 13                    JE SHORT PathOfEx.016F9254
	$+61     016F9241     8B4B 0C                  MOV ECX,DWORD PTR DS:[EBX+0xC]
	$+64     016F9244     55                       PUSH EBP
	$+65     016F9245     E8 36E8FFFF              CALL PathOfEx.016F7A80
	$+6A     016F924A     84C0                     TEST AL,AL
	$+6C     016F924C     74 06                    JE SHORT PathOfEx.016F9254                  ; 如果没有产生碰撞则跳转
	$+6E     016F924E     8B43 10                  MOV EAX,DWORD PTR DS:[EBX+0x10]
	$+71     016F9251     C600 01                  MOV BYTE PTR DS:[EAX],0x1
	$+74     016F9254     5F                       POP EDI
	$+75     016F9255     5E                       POP ESI
	$+76     016F9256     5D                       POP EBP
	$+77     016F9257     5B                       POP EBX
	$+78     016F9258     83C4 08                  ADD ESP,0x8
	$+7B     016F925B     C2 0400                  RETN 0x4
	$+7E     016F925E     CC                       INT3
	$+7F     016F925F     CC                       INT3
	$+80     016F9260     83EC 0C                  SUB ESP,0xC
	$+83     016F9263     56                       PUSH ESI
	$+84     016F9264     8BF1                     MOV ESI,ECX
	$+86     016F9266     8D4424 04                LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+8A     016F926A     8D56 14                  LEA EDX,DWORD PTR DS:[ESI+0x14]
	$+8D     016F926D     33C9                     XOR ECX,ECX
	$+8F     016F926F     57                       PUSH EDI
	$+90     016F9270     8B3A                     MOV EDI,DWORD PTR DS:[EDX]
	$+92     016F9272     8902                     MOV DWORD PTR DS:[EDX],EAX
	$+94     016F9274     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+97     016F9277     2B46 08                  SUB EAX,DWORD PTR DS:[ESI+0x8]
	$+9A     016F927A     C1F8 03                  SAR EAX,0x3
	$+9D     016F927D     895424 0C                MOV DWORD PTR SS:[ESP+0xC],EDX
	$+A1     016F9281     897C24 10                MOV DWORD PTR SS:[ESP+0x10],EDI
	$+A5     016F9285     894C24 08                MOV DWORD PTR SS:[ESP+0x8],ECX
	$+A9     016F9289     85C0                     TEST EAX,EAX
	$+AB     016F928B     74 64                    JE SHORT PathOfEx.016F92F1
	$+AD     016F928D     53                       PUSH EBX
	$+AE     016F928E     8A5C24 34                MOV BL,BYTE PTR SS:[ESP+0x34]
	$+B2     016F9292     55                       PUSH EBP
	$+B3     016F9293     8B6C24 34                MOV EBP,DWORD PTR SS:[ESP+0x34]
	$+B7     016F9297     66:0F1F8400 00000000     NOP WORD PTR DS:[EAX+EAX]
	$+C0     016F92A0     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+C3     016F92A3     8A54C8 04                MOV DL,BYTE PTR DS:[EAX+ECX*8+0x4]
	$+C7     016F92A7     8D3CC8                   LEA EDI,DWORD PTR DS:[EAX+ECX*8]
	$+CA     016F92AA     0FB6C2                   MOVZX EAX,DL
	$+CD     016F92AD     83E0 7F                  AND EAX,0x7F
	$+D0     016F92B0     3BC5                     CMP EAX,EBP
	$+D2     016F92B2     7F 29                    JG SHORT PathOfEx.016F92DD
	$+D4     016F92B4     84D2                     TEST DL,DL
	$+D6     016F92B6     79 04                    JNS SHORT PathOfEx.016F92BC
	$+D8     016F92B8     84DB                     TEST BL,BL
	$+DA     016F92BA     74 0F                    JE SHORT PathOfEx.016F92CB
	$+DC     016F92BC     FF37                     PUSH DWORD PTR DS:[EDI]
	$+DE     016F92BE     8D4C24 24                LEA ECX,DWORD PTR SS:[ESP+0x24]
	$+E2     016F92C2     E8 F9FBFFFF              CALL PathOfEx.016F8EC0
	$+E7     016F92C7     8B4C24 10                MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+EB     016F92CB     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+EE     016F92CE     41                       INC ECX
	$+EF     016F92CF     2B46 08                  SUB EAX,DWORD PTR DS:[ESI+0x8]
	$+F2     016F92D2     C1F8 03                  SAR EAX,0x3
	$+F5     016F92D5     894C24 10                MOV DWORD PTR SS:[ESP+0x10],ECX
	$+F9     016F92D9     3BC8                     CMP ECX,EAX
	$+FB     016F92DB   ^ 72 C3                    JB SHORT PathOfEx.016F92A0
	$+FD     016F92DD     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+101    016F92E1     8B4C24 18                MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+105    016F92E5     5D                       POP EBP
	$+106    016F92E6     5B                       POP EBX
	$+107    016F92E7     5F                       POP EDI
	$+108    016F92E8     8908                     MOV DWORD PTR DS:[EAX],ECX
	$+10A    016F92EA     5E                       POP ESI
	$+10B    016F92EB     83C4 0C                  ADD ESP,0xC
	$+10E    016F92EE     C2 1C00                  RETN 0x1C
	$+111    016F92F1     893A                     MOV DWORD PTR DS:[EDX],EDI
	$+113    016F92F3     5F                       POP EDI
	$+114    016F92F4     5E                       POP ESI
	$+115    016F92F5     83C4 0C                  ADD ESP,0xC
	$+118    016F92F8     C2 1C00                  RETN 0x1C
	$+11B    016F92FB     CC                       INT3
	$+11C    016F92FC     CC                       INT3
	$+11D    016F92FD     CC                       INT3
	$+11E    016F92FE     CC                       INT3
	$+11F    016F92FF     CC                       INT3
	$+120    016F9300     55                       PUSH EBP
	$+121    016F9301     8BEC                     MOV EBP,ESP
	$+123    016F9303     6A FF                    PUSH -0x1
	$+125    016F9305     68 E897DA01              PUSH PathOfEx.01DA97E8
	$+12A    016F930A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+130    016F9310     50                       PUSH EAX
	$+131    016F9311     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+138    016F9318     83EC 0C                  SUB ESP,0xC
	$+13B    016F931B     53                       PUSH EBX
	$+13C    016F931C     56                       PUSH ESI
	$+13D    016F931D     8BF1                     MOV ESI,ECX
	$+13F    016F931F     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+142    016F9322     8D56 14                  LEA EDX,DWORD PTR DS:[ESI+0x14]
	$+145    016F9325     57                       PUSH EDI
	$+146    016F9326     8B3A                     MOV EDI,DWORD PTR DS:[EDX]
	$+148    016F9328     8955 EC                  MOV DWORD PTR SS:[EBP-0x14],EDX
	$+14B    016F932B     897D F0                  MOV DWORD PTR SS:[EBP-0x10],EDI
	$+14E    016F932E     8902                     MOV DWORD PTR DS:[EDX],EAX
	$+150    016F9330     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+157    016F9337     33C9                     XOR ECX,ECX
	$+159    016F9339     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+15C    016F933C     2B46 08                  SUB EAX,DWORD PTR DS:[ESI+0x8]
	$+15F    016F933F     C1F8 03                  SAR EAX,0x3
	$+162    016F9342     894D E8                  MOV DWORD PTR SS:[EBP-0x18],ECX
	$+165    016F9345     85C0                     TEST EAX,EAX
	$+167    016F9347     74 5D                    JE SHORT PathOfEx.016F93A6
	$+169    016F9349     8A5D 20                  MOV BL,BYTE PTR SS:[EBP+0x20]
	$+16C    016F934C     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$+170    016F9350     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+173    016F9353     8A54C8 04                MOV DL,BYTE PTR DS:[EAX+ECX*8+0x4]
	$+177    016F9357     8D3CC8                   LEA EDI,DWORD PTR DS:[EAX+ECX*8]
	$+17A    016F935A     0FB6C2                   MOVZX EAX,DL
	$+17D    016F935D     83E0 7F                  AND EAX,0x7F
	$+180    016F9360     3B45 1C                  CMP EAX,DWORD PTR SS:[EBP+0x1C]
	$+183    016F9363     7F 26                    JG SHORT PathOfEx.016F938B
	$+185    016F9365     84D2                     TEST DL,DL
	$+187    016F9367     79 04                    JNS SHORT PathOfEx.016F936D
	$+189    016F9369     84DB                     TEST BL,BL
	$+18B    016F936B     74 0D                    JE SHORT PathOfEx.016F937A
	$+18D    016F936D     FF37                     PUSH DWORD PTR DS:[EDI]
	$+18F    016F936F     8D4D 08                  LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+192    016F9372     E8 69FEFFFF              CALL PathOfEx.016F91E0
	$+197    016F9377     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+19A    016F937A     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+19D    016F937D     41                       INC ECX
	$+19E    016F937E     2B46 08                  SUB EAX,DWORD PTR DS:[ESI+0x8]
	$+1A1    016F9381     C1F8 03                  SAR EAX,0x3
	$+1A4    016F9384     894D E8                  MOV DWORD PTR SS:[EBP-0x18],ECX
	$+1A7    016F9387     3BC8                     CMP ECX,EAX
	$+1A9    016F9389   ^ 72 C5                    JB SHORT PathOfEx.016F9350
	$+1AB    016F938B     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+1AE    016F938E     8B4D F0                  MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+1B1    016F9391     8908                     MOV DWORD PTR DS:[EAX],ECX
	$+1B3    016F9393     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1B6    016F9396     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+1BD    016F939D     5F                       POP EDI
	$+1BE    016F939E     5E                       POP ESI
	$+1BF    016F939F     5B                       POP EBX
	$+1C0    016F93A0     8BE5                     MOV ESP,EBP
	$+1C2    016F93A2     5D                       POP EBP
	$+1C3    016F93A3     C2 1C00                  RETN 0x1C
	$+1C6    016F93A6     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1C9    016F93A9     893A                     MOV DWORD PTR DS:[EDX],EDI
	$+1CB    016F93AB     5F                       POP EDI
	$+1CC    016F93AC     5E                       POP ESI
	$+1CD    016F93AD     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+1D4    016F93B4     5B                       POP EBX
	$+1D5    016F93B5     8BE5                     MOV ESP,EBP
	$+1D7    016F93B7     5D                       POP EBP
	$+1D8    016F93B8     C2 1C00                  RETN 0x1C
	$+1DB    016F93BB     CC                       INT3
	$+1DC    016F93BC     CC                       INT3
	$+1DD    016F93BD     CC                       INT3
	$+1DE    016F93BE     CC                       INT3
	$+1DF    016F93BF     CC                       INT3
	$+1E0    016F93C0     B8 9CEE0D02              MOV EAX,PathOfEx.020DEE9C
	$+1E5    016F93C5     C3                       RETN
	$+1E6    016F93C6     CC                       INT3
	$+1E7    016F93C7     CC                       INT3
	$+1E8    016F93C8     CC                       INT3
	$+1E9    016F93C9     CC                       INT3
	$+1EA    016F93CA     CC                       INT3
	$+1EB    016F93CB     CC                       INT3
	$+1EC    016F93CC     CC                       INT3
	$+1ED    016F93CD     CC                       INT3
	$+1EE    016F93CE     CC                       INT3
	$+1EF    016F93CF     CC                       INT3
	$+1F0    016F93D0     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+1F4    016F93D4     85C0                     TEST EAX,EAX
	$+1F6    016F93D6     74 06                    JE SHORT PathOfEx.016F93DE
	$+1F8    016F93D8     C700 204DFB01            MOV DWORD PTR DS:[EAX],PathOfEx.01FB4D20
	$+1FE    016F93DE     C2 0400                  RETN 0x4
	$+201    016F93E1     CC                       INT3
	$+202    016F93E2     CC                       INT3
	$+203    016F93E3     CC                       INT3
	$+204    016F93E4     CC                       INT3
	$+205    016F93E5     CC                       INT3
	$+206    016F93E6     CC                       INT3
	$+207    016F93E7     CC                       INT3
	$+208    016F93E8     CC                       INT3
	$+209    016F93E9     CC                       INT3
	$+20A    016F93EA     CC                       INT3
	$+20B    016F93EB     CC                       INT3
	$+20C    016F93EC     CC                       INT3
	$+20D    016F93ED     CC                       INT3
	$+20E    016F93EE     CC                       INT3
	$+20F    016F93EF     CC                       INT3
	$+210    016F93F0     B8 64EE0D02              MOV EAX,PathOfEx.020DEE64
	$+215    016F93F5     C3                       RETN
	$+216    016F93F6     CC                       INT3
	$+217    016F93F7     CC                       INT3
	$+218    016F93F8     CC                       INT3
	$+219    016F93F9     CC                       INT3
	$+21A    016F93FA     CC                       INT3
	$+21B    016F93FB     CC                       INT3
	$+21C    016F93FC     CC                       INT3
	$+21D    016F93FD     CC                       INT3
	$+21E    016F93FE     CC                       INT3
	$+21F    016F93FF     CC                       INT3
	$+220    016F9400     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+224    016F9404     85C0                     TEST EAX,EAX
	$+226    016F9406     74 06                    JE SHORT PathOfEx.016F940E
	$+228    016F9408     C700 384DFB01            MOV DWORD PTR DS:[EAX],PathOfEx.01FB4D38
	$+22E    016F940E     C2 0400                  RETN 0x4
	$+231    016F9411     CC                       INT3
	$+232    016F9412     CC                       INT3
	$+233    016F9413     CC                       INT3
	$+234    016F9414     CC                       INT3
	$+235    016F9415     CC                       INT3
	$+236    016F9416     CC                       INT3
	$+237    016F9417     CC                       INT3
	$+238    016F9418     CC                       INT3
	$+239    016F9419     CC                       INT3
	$+23A    016F941A     CC                       INT3
	$+23B    016F941B     CC                       INT3
	$+23C    016F941C     CC                       INT3
	$+23D    016F941D     CC                       INT3
	$+23E    016F941E     CC                       INT3
	$+23F    016F941F     CC                       INT3
	$+240    016F9420     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+244    016F9424     56                       PUSH ESI
	$+245    016F9425     8BF1                     MOV ESI,ECX
	$+247    016F9427     3B4424 30                CMP EAX,DWORD PTR SS:[ESP+0x30]
	$+24B    016F942B     75 0A                    JNZ SHORT PathOfEx.016F9437
	$+24D    016F942D     8B4424 1C                MOV EAX,DWORD PTR SS:[ESP+0x1C]
	$+251    016F9431     3B4424 34                CMP EAX,DWORD PTR SS:[ESP+0x34]
	$+255    016F9435     74 2D                    JE SHORT PathOfEx.016F9464
	$+257    016F9437     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+25B    016F943B     8BCE                     MOV ECX,ESI
	$+25D    016F943D     50                       PUSH EAX
	$+25E    016F943E     E8 FDF9FFFF              CALL PathOfEx.016F8E40
	$+263    016F9443     84C0                     TEST AL,AL
	$+265    016F9445     75 21                    JNZ SHORT PathOfEx.016F9468
	$+267    016F9447     8B4424 18                MOV EAX,DWORD PTR SS:[ESP+0x18]
	$+26B    016F944B     40                       INC EAX
	$+26C    016F944C     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$+270    016F9450     3B4424 10                CMP EAX,DWORD PTR SS:[ESP+0x10]
	$+274    016F9454   ^ 75 D1                    JNZ SHORT PathOfEx.016F9427
	$+276    016F9456     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+27A    016F945A     FF4424 1C                INC DWORD PTR SS:[ESP+0x1C]
	$+27E    016F945E     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$+282    016F9462   ^ EB C3                    JMP SHORT PathOfEx.016F9427
	$+284    016F9464     32C0                     XOR AL,AL
	$+286    016F9466     5E                       POP ESI
	$+287    016F9467     C3                       RETN
	$+288    016F9468     B0 01                    MOV AL,0x1
	$+28A    016F946A     5E                       POP ESI
	$+28B    016F946B     C3                       RETN
	$+28C    016F946C     CC                       INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("83????538b??8d??????558b??????568b??578b????8b", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_is_collisioned_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//特征码与上面的一样。
	/*
	$-321    01757B7F     CC                       INT3
	$-320    01757B80     56                       PUSH ESI
	$-31F    01757B81     FF7424 10                PUSH DWORD PTR SS:[ESP+0x10]
	$-31B    01757B85     8BF1                     MOV ESI,ECX
	$-319    01757B87     FF7424 0C                PUSH DWORD PTR SS:[ESP+0xC]
	$-315    01757B8B     E8 E077FAFF              CALL PathOfEx.016FF370
	$-310    01757B90     8B4C24 0C                MOV ECX,DWORD PTR SS:[ESP+0xC]
	$-30C    01757B94     0FB6C0                   MOVZX EAX,AL
	$-309    01757B97     3BC8                     CMP ECX,EAX
	$-307    01757B99     76 06                    JBE SHORT PathOfEx.01757BA1
	$-305    01757B9B     B0 01                    MOV AL,0x1
	$-303    01757B9D     5E                       POP ESI
	$-302    01757B9E     C2 1800                  RETN 0x18
	$-2FF    01757BA1     51                       PUSH ECX
	$-2FE    01757BA2     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$-2FA    01757BA6     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$-2F6    01757BAA     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$-2F2    01757BAE     51                       PUSH ECX
	$-2F1    01757BAF     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$-2ED    01757BB3     8BCE                     MOV ECX,ESI
	$-2EB    01757BB5     E8 86000000              CALL PathOfEx.01757C40
	$-2E6    01757BBA     5E                       POP ESI
	$-2E5    01757BBB     C2 1800                  RETN 0x18
	$-2E2    01757BBE     CC                       INT3
	$-2E1    01757BBF     CC                       INT3
	$-2E0    01757BC0     55                       PUSH EBP
	$-2DF    01757BC1     8BEC                     MOV EBP,ESP
	$-2DD    01757BC3     83E4 F0                  AND ESP,0xFFFFFFF0
	$-2DA    01757BC6     83EC 30                  SUB ESP,0x30
	$-2D7    01757BC9     8D45 10                  LEA EAX,DWORD PTR SS:[EBP+0x10]
	$-2D4    01757BCC     894C24 10                MOV DWORD PTR SS:[ESP+0x10],ECX
	$-2D0    01757BD0     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$-2CC    01757BD4     8D4D 1C                  LEA ECX,DWORD PTR SS:[EBP+0x1C]
	$-2C9    01757BD7     8D45 14                  LEA EAX,DWORD PTR SS:[EBP+0x14]
	$-2C6    01757BDA     C64424 0C 00             MOV BYTE PTR SS:[ESP+0xC],0x0
	$-2C1    01757BDF     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$-2BD    01757BE3     8D4424 0C                LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-2B9    01757BE7     894424 1C                MOV DWORD PTR SS:[ESP+0x1C],EAX
	$-2B5    01757BEB     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-2B2    01757BEE     0F104424 10              MOVUPS XMM0,DQWORD PTR SS:[ESP+0x10]
	$-2AD    01757BF3     FF70 04                  PUSH DWORD PTR DS:[EAX+0x4]
	$-2AA    01757BF6     FF30                     PUSH DWORD PTR DS:[EAX]
	$-2A8    01757BF8     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-2A5    01757BFB     FF70 04                  PUSH DWORD PTR DS:[EAX+0x4]
	$-2A2    01757BFE     FF30                     PUSH DWORD PTR DS:[EAX]
	$-2A0    01757C00     83EC 14                  SUB ESP,0x14
	$-29D    01757C03     8BC4                     MOV EAX,ESP
	$-29B    01757C05     0F1100                   MOVUPS DQWORD PTR DS:[EAX],XMM0
	$-298    01757C08     8948 10                  MOV DWORD PTR DS:[EAX+0x10],ECX
	$-295    01757C0B     E8 10030000              CALL PathOfEx.01757F20
	$-290    01757C10     33C9                     XOR ECX,ECX
	$-28E    01757C12     83C4 24                  ADD ESP,0x24
	$-28B    01757C15     84C0                     TEST AL,AL
	$-289    01757C17     0F94C1                   SETE CL
	$-286    01757C1A     8AC1                     MOV AL,CL
	$-284    01757C1C     8BE5                     MOV ESP,EBP
	$-282    01757C1E     5D                       POP EBP
	$-281    01757C1F     C2 1800                  RETN 0x18
	$-27E    01757C22     CC                       INT3
	$-27D    01757C23     CC                       INT3
	$-27C    01757C24     CC                       INT3
	$-27B    01757C25     CC                       INT3
	$-27A    01757C26     CC                       INT3
	$-279    01757C27     CC                       INT3
	$-278    01757C28     CC                       INT3
	$-277    01757C29     CC                       INT3
	$-276    01757C2A     CC                       INT3
	$-275    01757C2B     CC                       INT3
	$-274    01757C2C     CC                       INT3
	$-273    01757C2D     CC                       INT3
	$-272    01757C2E     CC                       INT3
	$-271    01757C2F     CC                       INT3
	$-270    01757C30     B8 74F51002              MOV EAX,PathOfEx.0210F574
	$-26B    01757C35     C3                       RETN
	$-26A    01757C36     CC                       INT3
	$-269    01757C37     CC                       INT3
	$-268    01757C38     CC                       INT3
	$-267    01757C39     CC                       INT3
	$-266    01757C3A     CC                       INT3
	$-265    01757C3B     CC                       INT3
	$-264    01757C3C     CC                       INT3
	$-263    01757C3D     CC                       INT3
	$-262    01757C3E     CC                       INT3
	$-261    01757C3F     CC                       INT3
	$-260    01757C40     55                       PUSH EBP
	$-25F    01757C41     8BEC                     MOV EBP,ESP
	$-25D    01757C43     83E4 F0                  AND ESP,0xFFFFFFF0
	$-25A    01757C46     81EC 98000000            SUB ESP,0x98
	$-254    01757C4C     A1 C09D0B02              MOV EAX,DWORD PTR DS:[0x20B9DC0]
	$-24F    01757C51     83C0 FE                  ADD EAX,-0x2
	$-24C    01757C54     894C24 10                MOV DWORD PTR SS:[ESP+0x10],ECX
	$-248    01757C58     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-245    01757C5B     56                       PUSH ESI
	$-244    01757C5C     57                       PUSH EDI
	$-243    01757C5D     8B7D 0C                  MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-240    01757C60     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-23E    01757C62     03F8                     ADD EDI,EAX
	$-23C    01757C64     8BC2                     MOV EAX,EDX
	$-23A    01757C66     C64424 10 00             MOV BYTE PTR SS:[ESP+0x10],0x0
	$-235    01757C6B     2BC7                     SUB EAX,EDI
	$-233    01757C6D     C74424 20 00000000       MOV DWORD PTR SS:[ESP+0x20],0x0
	$-22B    01757C75     894424 1C                MOV DWORD PTR SS:[ESP+0x1C],EAX
	$-227    01757C79     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-224    01757C7C     8BC8                     MOV ECX,EAX
	$-222    01757C7E     2BCF                     SUB ECX,EDI
	$-220    01757C80     C74424 24 00000000       MOV DWORD PTR SS:[ESP+0x24],0x0
	$-218    01757C88     894C24 14                MOV DWORD PTR SS:[ESP+0x14],ECX
	$-214    01757C8C     8B4C24 18                MOV ECX,DWORD PTR SS:[ESP+0x18]
	$-210    01757C90     8D3407                   LEA ESI,DWORD PTR DS:[EDI+EAX]
	$-20D    01757C93     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$-20A    01757C96     894424 28                MOV DWORD PTR SS:[ESP+0x28],EAX
	$-206    01757C9A     8B41 38                  MOV EAX,DWORD PTR DS:[ECX+0x38]
	$-203    01757C9D     8D0C17                   LEA ECX,DWORD PTR DS:[EDI+EDX]
	$-200    01757CA0     894424 2C                MOV DWORD PTR SS:[ESP+0x2C],EAX
	$-1FC    01757CA4     B8 C94216B2              MOV EAX,0xB21642C9
	$-1F7    01757CA9     F7E9                     IMUL ECX
	$-1F5    01757CAB     B8 C94216B2              MOV EAX,0xB21642C9
	$-1F0    01757CB0     03D1                     ADD EDX,ECX
	$-1EE    01757CB2     C1FA 04                  SAR EDX,0x4
	$-1EB    01757CB5     8BCA                     MOV ECX,EDX
	$-1E9    01757CB7     C1E9 1F                  SHR ECX,0x1F
	$-1E6    01757CBA     03CA                     ADD ECX,EDX
	$-1E4    01757CBC     F7EE                     IMUL ESI
	$-1E2    01757CBE     B8 C94216B2              MOV EAX,0xB21642C9
	$-1DD    01757CC3     03D6                     ADD EDX,ESI
	$-1DB    01757CC5     8D71 01                  LEA ESI,DWORD PTR DS:[ECX+0x1]
	$-1D8    01757CC8     C1FA 04                  SAR EDX,0x4
	$-1D5    01757CCB     8BFA                     MOV EDI,EDX
	$-1D3    01757CCD     897424 48                MOV DWORD PTR SS:[ESP+0x48],ESI
	$-1CF    01757CD1     42                       INC EDX
	$-1CE    01757CD2     C1EF 1F                  SHR EDI,0x1F
	$-1CB    01757CD5     03FA                     ADD EDI,EDX
	$-1C9    01757CD7     F76C24 1C                IMUL DWORD PTR SS:[ESP+0x1C]
	$-1C5    01757CDB     B8 C94216B2              MOV EAX,0xB21642C9
	$-1C0    01757CE0     897C24 4C                MOV DWORD PTR SS:[ESP+0x4C],EDI
	$-1BC    01757CE4     035424 1C                ADD EDX,DWORD PTR SS:[ESP+0x1C]
	$-1B8    01757CE8     C1FA 04                  SAR EDX,0x4
	$-1B5    01757CEB     8BCA                     MOV ECX,EDX
	$-1B3    01757CED     C1E9 1F                  SHR ECX,0x1F
	$-1B0    01757CF0     03CA                     ADD ECX,EDX
	$-1AE    01757CF2     F76C24 14                IMUL DWORD PTR SS:[ESP+0x14]
	$-1AA    01757CF6     894C24 40                MOV DWORD PTR SS:[ESP+0x40],ECX
	$-1A6    01757CFA     8D4C24 40                LEA ECX,DWORD PTR SS:[ESP+0x40]
	$-1A2    01757CFE     035424 14                ADD EDX,DWORD PTR SS:[ESP+0x14]
	$-19E    01757D02     C1FA 04                  SAR EDX,0x4
	$-19B    01757D05     8BC2                     MOV EAX,EDX
	$-199    01757D07     C1E8 1F                  SHR EAX,0x1F
	$-196    01757D0A     03C2                     ADD EAX,EDX
	$-194    01757D0C     894424 44                MOV DWORD PTR SS:[ESP+0x44],EAX
	$-190    01757D10     8D4424 20                LEA EAX,DWORD PTR SS:[ESP+0x20]
	$-18C    01757D14     50                       PUSH EAX
	$-18B    01757D15     8D4424 64                LEA EAX,DWORD PTR SS:[ESP+0x64]
	$-187    01757D19     50                       PUSH EAX
	$-186    01757D1A     E8 118BECFF              CALL PathOfEx.01620830
	$-181    01757D1F     0F284C24 60              MOVAPS XMM1,DQWORD PTR SS:[ESP+0x60]
	$-17C    01757D24     8B5424 60                MOV EDX,DWORD PTR SS:[ESP+0x60]
	$-178    01757D28     0F28C1                   MOVAPS XMM0,XMM1
	$-175    01757D2B     8B4C24 6C                MOV ECX,DWORD PTR SS:[ESP+0x6C]
	$-171    01757D2F     8B4424 64                MOV EAX,DWORD PTR SS:[ESP+0x64]
	$-16D    01757D33     660F73D8 08              PSRLDQ XMM0,0x8
	$-168    01757D38     66:0F7EC6                MOVD ESI,MM0
	$-164    01757D3C     895424 50                MOV DWORD PTR SS:[ESP+0x50],EDX
	$-160    01757D40     894C24 54                MOV DWORD PTR SS:[ESP+0x54],ECX
	$-15C    01757D44     3BD6                     CMP EDX,ESI
	$-15A    01757D46     75 0A                    JNZ SHORT PathOfEx.01757D52
	$-158    01757D48     897424 50                MOV DWORD PTR SS:[ESP+0x50],ESI
	$-154    01757D4C     894C24 54                MOV DWORD PTR SS:[ESP+0x54],ECX
	$-150    01757D50     EB 0C                    JMP SHORT PathOfEx.01757D5E
	$-14E    01757D52     3BC1                     CMP EAX,ECX
	$-14C    01757D54     75 08                    JNZ SHORT PathOfEx.01757D5E
	$-14A    01757D56     895424 50                MOV DWORD PTR SS:[ESP+0x50],EDX
	$-146    01757D5A     894424 54                MOV DWORD PTR SS:[ESP+0x54],EAX
	$-142    01757D5E     895424 70                MOV DWORD PTR SS:[ESP+0x70],EDX
	$-13E    01757D62     894424 74                MOV DWORD PTR SS:[ESP+0x74],EAX
	$-13A    01757D66     3BD6                     CMP EDX,ESI
	$-138    01757D68     75 0A                    JNZ SHORT PathOfEx.01757D74
	$-136    01757D6A     897424 70                MOV DWORD PTR SS:[ESP+0x70],ESI
	$-132    01757D6E     894C24 74                MOV DWORD PTR SS:[ESP+0x74],ECX
	$-12E    01757D72     EB 0C                    JMP SHORT PathOfEx.01757D80
	$-12C    01757D74     3BC1                     CMP EAX,ECX
	$-12A    01757D76     75 08                    JNZ SHORT PathOfEx.01757D80
	$-128    01757D78     895424 70                MOV DWORD PTR SS:[ESP+0x70],EDX
	$-124    01757D7C     894424 74                MOV DWORD PTR SS:[ESP+0x74],EAX
	$-120    01757D80     8D45 10                  LEA EAX,DWORD PTR SS:[EBP+0x10]
	$-11D    01757D83     83EC 18                  SUB ESP,0x18
	$-11A    01757D86     894424 38                MOV DWORD PTR SS:[ESP+0x38],EAX
	$-116    01757D8A     8D8C24 98000000          LEA ECX,DWORD PTR SS:[ESP+0x98]
	$-10F    01757D91     8B4424 30                MOV EAX,DWORD PTR SS:[ESP+0x30]
	$-10B    01757D95     894424 3C                MOV DWORD PTR SS:[ESP+0x3C],EAX
	$-107    01757D99     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-104    01757D9C     894424 40                MOV DWORD PTR SS:[ESP+0x40],EAX
	$-100    01757DA0     8D45 0C                  LEA EAX,DWORD PTR SS:[EBP+0xC]
	$-FD     01757DA3     894424 44                MOV DWORD PTR SS:[ESP+0x44],EAX
	$-F9     01757DA7     8D45 18                  LEA EAX,DWORD PTR SS:[EBP+0x18]
	$-F6     01757DAA     0F284424 38              MOVAPS XMM0,DQWORD PTR SS:[ESP+0x38]
	$-F1     01757DAF     894424 48                MOV DWORD PTR SS:[ESP+0x48],EAX
	$-ED     01757DB3     8D45 14                  LEA EAX,DWORD PTR SS:[EBP+0x14]
	$-EA     01757DB6     894424 4C                MOV DWORD PTR SS:[ESP+0x4C],EAX
	$-E6     01757DBA     8D4424 28                LEA EAX,DWORD PTR SS:[ESP+0x28]
	$-E2     01757DBE     0F298424 98000000        MOVAPS DQWORD PTR SS:[ESP+0x98],XMM0
	$-DA     01757DC6     F30F7E4424 48            MOVQ XMM0,QWORD PTR SS:[ESP+0x48]
	$-D4     01757DCC     898424 B0000000          MOV DWORD PTR SS:[ESP+0xB0],EAX
	$-CD     01757DD3     8BC4                     MOV EAX,ESP
	$-CB     01757DD5     660FD68424 A8000000      MOVQ QWORD PTR SS:[ESP+0xA8],XMM0
	$-C2     01757DDE     83EC 18                  SUB ESP,0x18
	$-BF     01757DE1     F30F7E8424 80000000      MOVQ XMM0,QWORD PTR SS:[ESP+0x80]
	$-B6     01757DEA     0F1108                   MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-B3     01757DED     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-AE     01757DF2     8BC4                     MOV EAX,ESP
	$-AC     01757DF4     F30F7E8424 A0000000      MOVQ XMM0,QWORD PTR SS:[ESP+0xA0]
	$-A3     01757DFD     0F1108                   MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-A0     01757E00     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-9B     01757E05     E8 86050000              CALL PathOfEx.01758390
	$-96     01757E0A     83C4 30                  ADD ESP,0x30
	$-93     01757E0D     5F                       POP EDI
	$-92     01757E0E     5E                       POP ESI
	$-91     01757E0F     8BE5                     MOV ESP,EBP
	$-8F     01757E11     5D                       POP EBP
	$-8E     01757E12     C2 1800                  RETN 0x18
	$-8B     01757E15     CC                       INT3
	$-8A     01757E16     CC                       INT3
	$-89     01757E17     CC                       INT3
	$-88     01757E18     CC                       INT3
	$-87     01757E19     CC                       INT3
	$-86     01757E1A     CC                       INT3
	$-85     01757E1B     CC                       INT3
	$-84     01757E1C     CC                       INT3
	$-83     01757E1D     CC                       INT3
	$-82     01757E1E     CC                       INT3
	$-81     01757E1F     CC                       INT3
	$-80     01757E20     83EC 18                  SUB ESP,0x18
	$-7D     01757E23     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-7B     01757E25     8B51 04                  MOV EDX,DWORD PTR DS:[ECX+0x4]
	$-78     01757E28     56                       PUSH ESI
	$-77     01757E29     57                       PUSH EDI
	$-76     01757E2A     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-74     01757E2C     8D7C24 24                LEA EDI,DWORD PTR SS:[ESP+0x24]
	$-70     01757E30     894424 08                MOV DWORD PTR SS:[ESP+0x8],EAX
	$-6C     01757E34     8B4424 24                MOV EAX,DWORD PTR SS:[ESP+0x24]
	$-68     01757E38     8B72 34                  MOV ESI,DWORD PTR DS:[EDX+0x34]
	$-65     01757E3B     0FAF70 04                IMUL ESI,DWORD PTR DS:[EAX+0x4]
	$-61     01757E3F     0330                     ADD ESI,DWORD PTR DS:[EAX]
	$-5F     01757E41     8B41 08                  MOV EAX,DWORD PTR DS:[ECX+0x8]
	$-5C     01757E44     894424 0C                MOV DWORD PTR SS:[ESP+0xC],EAX
	$-58     01757E48     8B41 0C                  MOV EAX,DWORD PTR DS:[ECX+0xC]
	$-55     01757E4B     894424 10                MOV DWORD PTR SS:[ESP+0x10],EAX
	$-51     01757E4F     8D4424 08                LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-4D     01757E53     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$-49     01757E57     8B41 10                  MOV EAX,DWORD PTR DS:[ECX+0x10]
	$-46     01757E5A     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$-42     01757E5E     8B41 18                  MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-3F     01757E61     0F104424 0C              MOVUPS XMM0,DQWORD PTR SS:[ESP+0xC]
	$-3A     01757E66     C1E6 05                  SHL ESI,0x5
	$-37     01757E69     0372 24                  ADD ESI,DWORD PTR DS:[EDX+0x24]
	$-34     01757E6C     33D2                     XOR EDX,EDX
	$-32     01757E6E     C64424 24 00             MOV BYTE PTR SS:[ESP+0x24],0x0
	$-2D     01757E73     0FB600                   MOVZX EAX,BYTE PTR DS:[EAX]
	$-2A     01757E76     50                       PUSH EAX
	$-29     01757E77     8B41 14                  MOV EAX,DWORD PTR DS:[ECX+0x14]
	$-26     01757E7A     8BCE                     MOV ECX,ESI
	$-24     01757E7C     3810                     CMP BYTE PTR DS:[EAX],DL
	$-22     01757E7E     0F95C2                   SETNE DL
	$-1F     01757E81     52                       PUSH EDX
	$-1E     01757E82     83EC 14                  SUB ESP,0x14
	$-1B     01757E85     8BC4                     MOV EAX,ESP
	$-19     01757E87     0F1100                   MOVUPS DQWORD PTR DS:[EAX],XMM0
	$-16     01757E8A     8978 10                  MOV DWORD PTR DS:[EAX+0x10],EDI
	$-13     01757E8D     E8 AE030000              CALL PathOfEx.01758240
	$-E      01757E92     8A4424 24                MOV AL,BYTE PTR SS:[ESP+0x24]
	$-A      01757E96     5F                       POP EDI
	$-9      01757E97     5E                       POP ESI
	$-8      01757E98     83C4 18                  ADD ESP,0x18
	$-5      01757E9B     C2 0400                  RETN 0x4
	$-2      01757E9E     CC                       INT3
	$-1      01757E9F     CC                       INT3                                        ; 计算寻路的时候会调用，即g_c_player_move_时会调用
	$ ==>    01757EA0     83EC 08                  SUB ESP,0x8                                 ; tag_判断是否在碰撞范围
	$+3      01757EA3     53                       PUSH EBX                                    ; p1=stCD_Positioned*
	$+4      01757EA4     8BD9                     MOV EBX,ECX
	$+6      01757EA6     8D4C24 10                LEA ECX,DWORD PTR SS:[ESP+0x10]             ; then,ecx=p1*
	$+A      01757EAA     55                       PUSH EBP
	$+B      01757EAB     8B6C24 14                MOV EBP,DWORD PTR SS:[ESP+0x14]             ; then,ebp=p1
	$+F      01757EAF     56                       PUSH ESI
	$+10     01757EB0     8B13                     MOV EDX,DWORD PTR DS:[EBX]
	$+12     01757EB2     57                       PUSH EDI
	$+13     01757EB3     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$+16     01757EB6     8B75 10                  MOV ESI,DWORD PTR SS:[EBP+0x10]
	$+19     01757EB9     2B42 04                  SUB EAX,DWORD PTR DS:[EDX+0x4]
	$+1C     01757EBC     2B32                     SUB ESI,DWORD PTR DS:[EDX]
	$+1E     01757EBE     99                       CDQ
	$+1F     01757EBF     8BF8                     MOV EDI,EAX
	$+21     01757EC1     8BC6                     MOV EAX,ESI
	$+23     01757EC3     33FA                     XOR EDI,EDX
	$+25     01757EC5     8D7424 14                LEA ESI,DWORD PTR SS:[ESP+0x14]
	$+29     01757EC9     2BFA                     SUB EDI,EDX
	$+2B     01757ECB     99                       CDQ
	$+2C     01757ECC     33C2                     XOR EAX,EDX
	$+2E     01757ECE     897C24 1C                MOV DWORD PTR SS:[ESP+0x1C],EDI
	$+32     01757ED2     2BC2                     SUB EAX,EDX
	$+34     01757ED4     8B55 28                  MOV EDX,DWORD PTR SS:[EBP+0x28]
	$+37     01757ED7     3BC7                     CMP EAX,EDI
	$+39     01757ED9     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$+3D     01757EDD     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+40     01757EE0     0F4DCE                   CMOVGE ECX,ESI
	$+43     01757EE3     8B09                     MOV ECX,DWORD PTR DS:[ECX]
	$+45     01757EE5     2BCA                     SUB ECX,EDX
	$+47     01757EE7     3908                     CMP DWORD PTR DS:[EAX],ECX
	$+49     01757EE9     7E 29                    JLE SHORT PathOfEx.01757F14
	$+4B     01757EEB     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+4E     01757EEE     8338 00                  CMP DWORD PTR DS:[EAX],0x0
	$+51     01757EF1     75 09                    JNZ SHORT PathOfEx.01757EFC
	$+53     01757EF3     83BD A4000000 01         CMP DWORD PTR SS:[EBP+0xA4],0x1
	$+5A     01757EFA     74 18                    JE SHORT PathOfEx.01757F14
	$+5C     01757EFC     83FA FF                  CMP EDX,-0x1
	$+5F     01757EFF     74 13                    JE SHORT PathOfEx.01757F14
	$+61     01757F01     8B4B 0C                  MOV ECX,DWORD PTR DS:[EBX+0xC]
	$+64     01757F04     55                       PUSH EBP
	$+65     01757F05     E8 06FAFFFF              CALL PathOfEx.01757910
	$+6A     01757F0A     84C0                     TEST AL,AL
	$+6C     01757F0C     74 06                    JE SHORT PathOfEx.01757F14                  ; 如果没有产生碰撞则跳转
	$+6E     01757F0E     8B43 10                  MOV EAX,DWORD PTR DS:[EBX+0x10]
	$+71     01757F11     C600 01                  MOV BYTE PTR DS:[EAX],0x1                   ; 产生了碰撞
	$+74     01757F14     5F                       POP EDI
	$+75     01757F15     5E                       POP ESI
	$+76     01757F16     5D                       POP EBP
	$+77     01757F17     5B                       POP EBX
	$+78     01757F18     83C4 08                  ADD ESP,0x8
	$+7B     01757F1B     C2 0400                  RETN 0x4
	$+7E     01757F1E     CC                       INT3
	$+7F     01757F1F     CC                       INT3
	$+80     01757F20     83EC 60                  SUB ESP,0x60
	$+83     01757F23     8B8C24 80000000          MOV ECX,DWORD PTR SS:[ESP+0x80]
	$+8A     01757F2A     8BC1                     MOV EAX,ECX
	$+8C     01757F2C     53                       PUSH EBX
	$+8D     01757F2D     8B9C24 80000000          MOV EBX,DWORD PTR SS:[ESP+0x80]
	$+94     01757F34     55                       PUSH EBP
	$+95     01757F35     8BAC24 8C000000          MOV EBP,DWORD PTR SS:[ESP+0x8C]
	$+9C     01757F3C     56                       PUSH ESI
	$+9D     01757F3D     57                       PUSH EDI
	$+9E     01757F3E     8BBC24 88000000          MOV EDI,DWORD PTR SS:[ESP+0x88]
	$+A5     01757F45     2BC7                     SUB EAX,EDI
	$+A7     01757F47     99                       CDQ
	$+A8     01757F48     8BF0                     MOV ESI,EAX
	$+AA     01757F4A     8BC5                     MOV EAX,EBP
	$+AC     01757F4C     33F2                     XOR ESI,EDX
	$+AE     01757F4E     2BC3                     SUB EAX,EBX
	$+B0     01757F50     2BF2                     SUB ESI,EDX
	$+B2     01757F52     99                       CDQ
	$+B3     01757F53     33C2                     XOR EAX,EDX
	$+B5     01757F55     897424 10                MOV DWORD PTR SS:[ESP+0x10],ESI
	$+B9     01757F59     2BC2                     SUB EAX,EDX
	$+BB     01757F5B     894424 14                MOV DWORD PTR SS:[ESP+0x14],EAX
	$+BF     01757F5F     3BC6                     CMP EAX,ESI
	$+C1     01757F61     7E 28                    JLE SHORT PathOfEx.01757F8B
	$+C3     01757F63     8BC7                     MOV EAX,EDI
	$+C5     01757F65     8BFB                     MOV EDI,EBX
	$+C7     01757F67     8BD8                     MOV EBX,EAX
	$+C9     01757F69     89BC24 88000000          MOV DWORD PTR SS:[ESP+0x88],EDI
	$+D0     01757F70     8BC1                     MOV EAX,ECX
	$+D2     01757F72     899C24 8C000000          MOV DWORD PTR SS:[ESP+0x8C],EBX
	$+D9     01757F79     8BCD                     MOV ECX,EBP
	$+DB     01757F7B     8BE8                     MOV EBP,EAX
	$+DD     01757F7D     898C24 90000000          MOV DWORD PTR SS:[ESP+0x90],ECX
	$+E4     01757F84     89AC24 94000000          MOV DWORD PTR SS:[ESP+0x94],EBP
	$+EB     01757F8B     8BC1                     MOV EAX,ECX
	$+ED     01757F8D     2BC7                     SUB EAX,EDI
	$+EF     01757F8F     99                       CDQ
	$+F0     01757F90     33C2                     XOR EAX,EDX
	$+F2     01757F92     2BC2                     SUB EAX,EDX
	$+F4     01757F94     894424 1C                MOV DWORD PTR SS:[ESP+0x1C],EAX
	$+F8     01757F98     8BC5                     MOV EAX,EBP
	$+FA     01757F9A     2BC3                     SUB EAX,EBX
	$+FC     01757F9C     99                       CDQ
	$+FD     01757F9D     33C2                     XOR EAX,EDX
	$+FF     01757F9F     2BC2                     SUB EAX,EDX
	$+101    01757FA1     894424 2C                MOV DWORD PTR SS:[ESP+0x2C],EAX
	$+105    01757FA5     3BDD                     CMP EBX,EBP
	$+107    01757FA7     7D 0A                    JGE SHORT PathOfEx.01757FB3
	$+109    01757FA9     C74424 18 01000000       MOV DWORD PTR SS:[ESP+0x18],0x1
	$+111    01757FB1     EB 0C                    JMP SHORT PathOfEx.01757FBF
	$+113    01757FB3     33C0                     XOR EAX,EAX
	$+115    01757FB5     3BDD                     CMP EBX,EBP
	$+117    01757FB7     0F9EC0                   SETLE AL
	$+11A    01757FBA     48                       DEC EAX
	$+11B    01757FBB     894424 18                MOV DWORD PTR SS:[ESP+0x18],EAX
	$+11F    01757FBF     33C0                     XOR EAX,EAX
	$+121    01757FC1     3BF9                     CMP EDI,ECX
	$+123    01757FC3     0F9CC0                   SETL AL
	$+126    01757FC6     8D0445 FFFFFFFF          LEA EAX,DWORD PTR DS:[EAX*2-0x1]
	$+12D    01757FCD     894424 34                MOV DWORD PTR SS:[ESP+0x34],EAX
	$+131    01757FD1     8B4424 1C                MOV EAX,DWORD PTR SS:[ESP+0x1C]
	$+135    01757FD5     99                       CDQ
	$+136    01757FD6     2BC2                     SUB EAX,EDX
	$+138    01757FD8     D1F8                     SAR EAX,1
	$+13A    01757FDA     8BE8                     MOV EBP,EAX
	$+13C    01757FDC     F7DD                     NEG EBP
	$+13E    01757FDE     3BF9                     CMP EDI,ECX
	$+140    01757FE0     0F84 56010000            JE PathOfEx.0175813C
	$+146    01757FE6     397424 14                CMP DWORD PTR SS:[ESP+0x14],ESI
	$+14A    01757FEA     7E 0E                    JLE SHORT PathOfEx.01757FFA
	$+14C    01757FEC     895C24 50                MOV DWORD PTR SS:[ESP+0x50],EBX
	$+150    01757FF0     8D4C24 50                LEA ECX,DWORD PTR SS:[ESP+0x50]
	$+154    01757FF4     897C24 54                MOV DWORD PTR SS:[ESP+0x54],EDI
	$+158    01757FF8     EB 0C                    JMP SHORT PathOfEx.01758006
	$+15A    01757FFA     897C24 58                MOV DWORD PTR SS:[ESP+0x58],EDI
	$+15E    01757FFE     8D4C24 58                LEA ECX,DWORD PTR SS:[ESP+0x58]
	$+162    01758002     895C24 5C                MOV DWORD PTR SS:[ESP+0x5C],EBX
	$+166    01758006     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+168    01758008     894424 38                MOV DWORD PTR SS:[ESP+0x38],EAX
	$+16C    0175800C     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+16F    0175800F     894424 3C                MOV DWORD PTR SS:[ESP+0x3C],EAX
	$+173    01758013     8B8424 80000000          MOV EAX,DWORD PTR SS:[ESP+0x80]
	$+17A    0175801A     8BB424 84000000          MOV ESI,DWORD PTR SS:[ESP+0x84]
	$+181    01758021     8A00                     MOV AL,BYTE PTR DS:[EAX]
	$+183    01758023     884424 24                MOV BYTE PTR SS:[ESP+0x24],AL
	$+187    01758027     8B4424 7C                MOV EAX,DWORD PTR SS:[ESP+0x7C]
	$+18B    0175802B     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+18D    0175802D     8B4424 78                MOV EAX,DWORD PTR SS:[ESP+0x78]
	$+191    01758031     51                       PUSH ECX
	$+192    01758032     894C24 2C                MOV DWORD PTR SS:[ESP+0x2C],ECX
	$+196    01758036     8B4C24 78                MOV ECX,DWORD PTR SS:[ESP+0x78]
	$+19A    0175803A     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+19C    0175803C     894424 24                MOV DWORD PTR SS:[ESP+0x24],EAX
	$+1A0    01758040     8D4424 3C                LEA EAX,DWORD PTR SS:[ESP+0x3C]
	$+1A4    01758044     50                       PUSH EAX
	$+1A5    01758045     E8 2673FAFF              CALL PathOfEx.016FF370
	$+1AA    0175804A     8B5424 20                MOV EDX,DWORD PTR SS:[ESP+0x20]
	$+1AE    0175804E     0FB6C0                   MOVZX EAX,AL
	$+1B1    01758051     3BD0                     CMP EDX,EAX
	$+1B3    01758053     0F87 67010000            JA PathOfEx.017581C0
	$+1B9    01758059     51                       PUSH ECX
	$+1BA    0175805A     FF36                     PUSH DWORD PTR DS:[ESI]
	$+1BC    0175805C     8B4C24 7C                MOV ECX,DWORD PTR SS:[ESP+0x7C]
	$+1C0    01758060     8D4424 40                LEA EAX,DWORD PTR SS:[ESP+0x40]
	$+1C4    01758064     FF7424 2C                PUSH DWORD PTR SS:[ESP+0x2C]
	$+1C8    01758068     FF7424 34                PUSH DWORD PTR SS:[ESP+0x34]
	$+1CC    0175806C     52                       PUSH EDX
	$+1CD    0175806D     50                       PUSH EAX
	$+1CE    0175806E     E8 CDFBFFFF              CALL PathOfEx.01757C40
	$+1D3    01758073     84C0                     TEST AL,AL
	$+1D5    01758075     0F85 45010000            JNZ PathOfEx.017581C0
	$+1DB    0175807B     036C24 2C                ADD EBP,DWORD PTR SS:[ESP+0x2C]
	$+1DF    0175807F     85ED                     TEST EBP,EBP
	$+1E1    01758081     0F8E A1000000            JLE PathOfEx.01758128
	$+1E7    01758087     035C24 18                ADD EBX,DWORD PTR SS:[ESP+0x18]
	$+1EB    0175808B     2B6C24 1C                SUB EBP,DWORD PTR SS:[ESP+0x1C]
	$+1EF    0175808F     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+1F3    01758093     3B4424 10                CMP EAX,DWORD PTR SS:[ESP+0x10]
	$+1F7    01758097     7E 0E                    JLE SHORT PathOfEx.017580A7
	$+1F9    01758099     895C24 60                MOV DWORD PTR SS:[ESP+0x60],EBX
	$+1FD    0175809D     8D4C24 60                LEA ECX,DWORD PTR SS:[ESP+0x60]
	$+201    017580A1     897C24 64                MOV DWORD PTR SS:[ESP+0x64],EDI
	$+205    017580A5     EB 0C                    JMP SHORT PathOfEx.017580B3
	$+207    017580A7     897C24 68                MOV DWORD PTR SS:[ESP+0x68],EDI
	$+20B    017580AB     8D4C24 68                LEA ECX,DWORD PTR SS:[ESP+0x68]
	$+20F    017580AF     895C24 6C                MOV DWORD PTR SS:[ESP+0x6C],EBX
	$+213    017580B3     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+215    017580B5     894424 40                MOV DWORD PTR SS:[ESP+0x40],EAX
	$+219    017580B9     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+21C    017580BC     894424 44                MOV DWORD PTR SS:[ESP+0x44],EAX
	$+220    017580C0     8B8424 80000000          MOV EAX,DWORD PTR SS:[ESP+0x80]
	$+227    017580C7     8BB424 84000000          MOV ESI,DWORD PTR SS:[ESP+0x84]
	$+22E    017580CE     8A00                     MOV AL,BYTE PTR DS:[EAX]
	$+230    017580D0     884424 30                MOV BYTE PTR SS:[ESP+0x30],AL
	$+234    017580D4     8B4424 7C                MOV EAX,DWORD PTR SS:[ESP+0x7C]
	$+238    017580D8     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+23A    017580DA     8B4424 78                MOV EAX,DWORD PTR SS:[ESP+0x78]
	$+23E    017580DE     51                       PUSH ECX
	$+23F    017580DF     894C24 24                MOV DWORD PTR SS:[ESP+0x24],ECX
	$+243    017580E3     8B4C24 78                MOV ECX,DWORD PTR SS:[ESP+0x78]
	$+247    017580E7     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+249    017580E9     894424 2C                MOV DWORD PTR SS:[ESP+0x2C],EAX
	$+24D    017580ED     8D4424 44                LEA EAX,DWORD PTR SS:[ESP+0x44]
	$+251    017580F1     50                       PUSH EAX
	$+252    017580F2     E8 7972FAFF              CALL PathOfEx.016FF370
	$+257    017580F7     8B5424 28                MOV EDX,DWORD PTR SS:[ESP+0x28]
	$+25B    017580FB     0FB6C0                   MOVZX EAX,AL
	$+25E    017580FE     3BD0                     CMP EDX,EAX
	$+260    01758100     0F87 BA000000            JA PathOfEx.017581C0
	$+266    01758106     51                       PUSH ECX
	$+267    01758107     FF36                     PUSH DWORD PTR DS:[ESI]
	$+269    01758109     8B4C24 7C                MOV ECX,DWORD PTR SS:[ESP+0x7C]
	$+26D    0175810D     8D4424 48                LEA EAX,DWORD PTR SS:[ESP+0x48]
	$+271    01758111     FF7424 38                PUSH DWORD PTR SS:[ESP+0x38]
	$+275    01758115     FF7424 2C                PUSH DWORD PTR SS:[ESP+0x2C]
	$+279    01758119     52                       PUSH EDX
	$+27A    0175811A     50                       PUSH EAX
	$+27B    0175811B     E8 20FBFFFF              CALL PathOfEx.01757C40
	$+280    01758120     84C0                     TEST AL,AL
	$+282    01758122     0F85 98000000            JNZ PathOfEx.017581C0
	$+288    01758128     037C24 34                ADD EDI,DWORD PTR SS:[ESP+0x34]
	$+28C    0175812C     8B8C24 90000000          MOV ECX,DWORD PTR SS:[ESP+0x90]
	$+293    01758133     8B7424 10                MOV ESI,DWORD PTR SS:[ESP+0x10]
	$+297    01758137   ^ E9 A2FEFFFF              JMP PathOfEx.01757FDE
	$+29C    0175813C     397424 14                CMP DWORD PTR SS:[ESP+0x14],ESI
	$+2A0    01758140     7E 0A                    JLE SHORT PathOfEx.0175814C
	$+2A2    01758142     895C24 68                MOV DWORD PTR SS:[ESP+0x68],EBX
	$+2A6    01758146     894C24 6C                MOV DWORD PTR SS:[ESP+0x6C],ECX
	$+2AA    0175814A     EB 08                    JMP SHORT PathOfEx.01758154
	$+2AC    0175814C     894C24 68                MOV DWORD PTR SS:[ESP+0x68],ECX
	$+2B0    01758150     895C24 6C                MOV DWORD PTR SS:[ESP+0x6C],EBX
	$+2B4    01758154     8B6C24 74                MOV EBP,DWORD PTR SS:[ESP+0x74]
	$+2B8    01758158     8D4C24 68                LEA ECX,DWORD PTR SS:[ESP+0x68]
	$+2BC    0175815C     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+2BE    0175815E     894424 48                MOV DWORD PTR SS:[ESP+0x48],EAX
	$+2C2    01758162     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+2C5    01758165     8BCD                     MOV ECX,EBP
	$+2C7    01758167     8BB424 84000000          MOV ESI,DWORD PTR SS:[ESP+0x84]
	$+2CE    0175816E     894424 4C                MOV DWORD PTR SS:[ESP+0x4C],EAX
	$+2D2    01758172     8B8424 80000000          MOV EAX,DWORD PTR SS:[ESP+0x80]
	$+2D9    01758179     8A00                     MOV AL,BYTE PTR DS:[EAX]
	$+2DB    0175817B     884424 34                MOV BYTE PTR SS:[ESP+0x34],AL
	$+2DF    0175817F     8B4424 7C                MOV EAX,DWORD PTR SS:[ESP+0x7C]
	$+2E3    01758183     8B38                     MOV EDI,DWORD PTR DS:[EAX]
	$+2E5    01758185     8B4424 78                MOV EAX,DWORD PTR SS:[ESP+0x78]
	$+2E9    01758189     57                       PUSH EDI
	$+2EA    0175818A     8B18                     MOV EBX,DWORD PTR DS:[EAX]
	$+2EC    0175818C     8D4424 4C                LEA EAX,DWORD PTR SS:[ESP+0x4C]
	$+2F0    01758190     50                       PUSH EAX
	$+2F1    01758191     E8 DA71FAFF              CALL PathOfEx.016FF370
	$+2F6    01758196     0FB6C0                   MOVZX EAX,AL
	$+2F9    01758199     3BD8                     CMP EBX,EAX
	$+2FB    0175819B     77 23                    JA SHORT PathOfEx.017581C0
	$+2FD    0175819D     51                       PUSH ECX
	$+2FE    0175819E     FF36                     PUSH DWORD PTR DS:[ESI]
	$+300    017581A0     8D4424 50                LEA EAX,DWORD PTR SS:[ESP+0x50]
	$+304    017581A4     8BCD                     MOV ECX,EBP
	$+306    017581A6     FF7424 3C                PUSH DWORD PTR SS:[ESP+0x3C]
	$+30A    017581AA     57                       PUSH EDI
	$+30B    017581AB     53                       PUSH EBX
	$+30C    017581AC     50                       PUSH EAX
	$+30D    017581AD     E8 8EFAFFFF              CALL PathOfEx.01757C40
	$+312    017581B2     84C0                     TEST AL,AL
	$+314    017581B4     75 0A                    JNZ SHORT PathOfEx.017581C0
	$+316    017581B6     B0 01                    MOV AL,0x1
	$+318    017581B8     5F                       POP EDI
	$+319    017581B9     5E                       POP ESI
	$+31A    017581BA     5D                       POP EBP
	$+31B    017581BB     5B                       POP EBX
	$+31C    017581BC     83C4 60                  ADD ESP,0x60
	$+31F    017581BF     C3                       RETN
	$+320    017581C0     5F                       POP EDI
	$+321    017581C1     5E                       POP ESI
	$+322    017581C2     5D                       POP EBP
	$+323    017581C3     32C0                     XOR AL,AL
	$+325    017581C5     5B                       POP EBX
	$+326    017581C6     83C4 60                  ADD ESP,0x60
	$+329    017581C9     C3                       RETN
	$+32A    017581CA     CC                       INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("83????538b??8d??????558b??????568b??578b????8b", 0, 2);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_is_in_collision_range_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-2E0    00E5FFCF     CC                      INT3
	$-2DF    00E5FFD0     55                      PUSH EBP
	$-2DE    00E5FFD1     8BEC                    MOV EBP,ESP
	$-2DC    00E5FFD3     83E4 F0                 AND ESP,0xFFFFFFF0
	$-2D9    00E5FFD6     83EC 68                 SUB ESP,0x68
	$-2D6    00E5FFD9     56                      PUSH ESI
	$-2D5    00E5FFDA     57                      PUSH EDI
	$-2D4    00E5FFDB     FF75 0C                 PUSH DWORD PTR SS:[EBP+0xC]
	$-2D1    00E5FFDE     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-2CE    00E5FFE1     8BF1                    MOV ESI,ECX
	$-2CC    00E5FFE3     57                      PUSH EDI
	$-2CB    00E5FFE4     897424 24               MOV DWORD PTR SS:[ESP+0x24],ESI
	$-2C7    00E5FFE8     E8 83F3FFFF             CALL PathOfEx.00E5F370
	$-2C2    00E5FFED     8B0F                    MOV ECX,DWORD PTR DS:[EDI]
	$-2C0    00E5FFEF     8B57 04                 MOV EDX,DWORD PTR DS:[EDI+0x4]
	$-2BD    00E5FFF2     884424 13               MOV BYTE PTR SS:[ESP+0x13],AL
	$-2B9    00E5FFF6     C74424 20 00000000      MOV DWORD PTR SS:[ESP+0x20],0x0
	$-2B1    00E5FFFE     8D41 DA                 LEA EAX,DWORD PTR DS:[ECX-0x26]
	$-2AE    00E60001     C74424 24 00000000      MOV DWORD PTR SS:[ESP+0x24],0x0
	$-2A6    00E60009     894424 18               MOV DWORD PTR SS:[ESP+0x18],EAX
	$-2A2    00E6000D     83C1 26                 ADD ECX,0x26
	$-29F    00E60010     8D42 DA                 LEA EAX,DWORD PTR DS:[EDX-0x26]
	$-29C    00E60013     894424 14               MOV DWORD PTR SS:[ESP+0x14],EAX
	$-298    00E60017     8B46 34                 MOV EAX,DWORD PTR DS:[ESI+0x34]
	$-295    00E6001A     894424 28               MOV DWORD PTR SS:[ESP+0x28],EAX
	$-291    00E6001E     8B46 38                 MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-28E    00E60021     8D72 26                 LEA ESI,DWORD PTR DS:[EDX+0x26]
	$-28B    00E60024     894424 2C               MOV DWORD PTR SS:[ESP+0x2C],EAX
	$-287    00E60028     B8 C94216B2             MOV EAX,0xB21642C9
	$-282    00E6002D     F7E9                    IMUL ECX
	$-280    00E6002F     B8 C94216B2             MOV EAX,0xB21642C9
	$-27B    00E60034     03D1                    ADD EDX,ECX
	$-279    00E60036     C1FA 04                 SAR EDX,0x4
	$-276    00E60039     8BCA                    MOV ECX,EDX
	$-274    00E6003B     C1E9 1F                 SHR ECX,0x1F
	$-271    00E6003E     03CA                    ADD ECX,EDX
	$-26F    00E60040     F7EE                    IMUL ESI
	$-26D    00E60042     B8 C94216B2             MOV EAX,0xB21642C9
	$-268    00E60047     03D6                    ADD EDX,ESI
	$-266    00E60049     8D71 01                 LEA ESI,DWORD PTR DS:[ECX+0x1]
	$-263    00E6004C     C1FA 04                 SAR EDX,0x4
	$-260    00E6004F     8BFA                    MOV EDI,EDX
	$-25E    00E60051     897424 38               MOV DWORD PTR SS:[ESP+0x38],ESI
	$-25A    00E60055     42                      INC EDX
	$-259    00E60056     C1EF 1F                 SHR EDI,0x1F
	$-256    00E60059     03FA                    ADD EDI,EDX
	$-254    00E6005B     F76C24 18               IMUL DWORD PTR SS:[ESP+0x18]
	$-250    00E6005F     B8 C94216B2             MOV EAX,0xB21642C9
	$-24B    00E60064     897C24 3C               MOV DWORD PTR SS:[ESP+0x3C],EDI
	$-247    00E60068     035424 18               ADD EDX,DWORD PTR SS:[ESP+0x18]
	$-243    00E6006C     C1FA 04                 SAR EDX,0x4
	$-240    00E6006F     8BCA                    MOV ECX,EDX
	$-23E    00E60071     C1E9 1F                 SHR ECX,0x1F
	$-23B    00E60074     03CA                    ADD ECX,EDX
	$-239    00E60076     F76C24 14               IMUL DWORD PTR SS:[ESP+0x14]
	$-235    00E6007A     894C24 30               MOV DWORD PTR SS:[ESP+0x30],ECX
	$-231    00E6007E     8D4C24 30               LEA ECX,DWORD PTR SS:[ESP+0x30]
	$-22D    00E60082     035424 14               ADD EDX,DWORD PTR SS:[ESP+0x14]
	$-229    00E60086     C1FA 04                 SAR EDX,0x4
	$-226    00E60089     8BC2                    MOV EAX,EDX
	$-224    00E6008B     C1E8 1F                 SHR EAX,0x1F
	$-221    00E6008E     03C2                    ADD EAX,EDX
	$-21F    00E60090     894424 34               MOV DWORD PTR SS:[ESP+0x34],EAX
	$-21B    00E60094     8D4424 20               LEA EAX,DWORD PTR SS:[ESP+0x20]
	$-217    00E60098     50                      PUSH EAX
	$-216    00E60099     8D4424 54               LEA EAX,DWORD PTR SS:[ESP+0x54]
	$-212    00E6009D     50                      PUSH EAX
	$-211    00E6009E     E8 8D07F2FF             CALL PathOfEx.00D80830
	$-20C    00E600A3     0F284C24 50             MOVAPS XMM1,DQWORD PTR SS:[ESP+0x50]
	$-207    00E600A8     8B5424 50               MOV EDX,DWORD PTR SS:[ESP+0x50]
	$-203    00E600AC     0F28C1                  MOVAPS XMM0,XMM1
	$-200    00E600AF     8B4C24 5C               MOV ECX,DWORD PTR SS:[ESP+0x5C]
	$-1FC    00E600B3     8B4424 54               MOV EAX,DWORD PTR SS:[ESP+0x54]
	$-1F8    00E600B7     660F73D8 08             PSRLDQ XMM0,0x8
	$-1F3    00E600BC     66:0F7EC6               MOVD ESI,MM0
	$-1EF    00E600C0     895424 40               MOV DWORD PTR SS:[ESP+0x40],EDX
	$-1EB    00E600C4     894C24 44               MOV DWORD PTR SS:[ESP+0x44],ECX
	$-1E7    00E600C8     3BD6                    CMP EDX,ESI
	$-1E5    00E600CA     75 0A                   JNZ SHORT PathOfEx.00E600D6
	$-1E3    00E600CC     897424 40               MOV DWORD PTR SS:[ESP+0x40],ESI
	$-1DF    00E600D0     894C24 44               MOV DWORD PTR SS:[ESP+0x44],ECX
	$-1DB    00E600D4     EB 0C                   JMP SHORT PathOfEx.00E600E2
	$-1D9    00E600D6     3BC1                    CMP EAX,ECX
	$-1D7    00E600D8     75 08                   JNZ SHORT PathOfEx.00E600E2
	$-1D5    00E600DA     895424 40               MOV DWORD PTR SS:[ESP+0x40],EDX
	$-1D1    00E600DE     894424 44               MOV DWORD PTR SS:[ESP+0x44],EAX
	$-1CD    00E600E2     895424 60               MOV DWORD PTR SS:[ESP+0x60],EDX
	$-1C9    00E600E6     894424 64               MOV DWORD PTR SS:[ESP+0x64],EAX
	$-1C5    00E600EA     3BD6                    CMP EDX,ESI
	$-1C3    00E600EC     75 0A                   JNZ SHORT PathOfEx.00E600F8
	$-1C1    00E600EE     897424 60               MOV DWORD PTR SS:[ESP+0x60],ESI
	$-1BD    00E600F2     894C24 64               MOV DWORD PTR SS:[ESP+0x64],ECX
	$-1B9    00E600F6     EB 0C                   JMP SHORT PathOfEx.00E60104
	$-1B7    00E600F8     3BC1                    CMP EAX,ECX
	$-1B5    00E600FA     75 08                   JNZ SHORT PathOfEx.00E60104
	$-1B3    00E600FC     895424 60               MOV DWORD PTR SS:[ESP+0x60],EDX
	$-1AF    00E60100     894424 64               MOV DWORD PTR SS:[ESP+0x64],EAX
	$-1AB    00E60104     8B4424 1C               MOV EAX,DWORD PTR SS:[ESP+0x1C]
	$-1A7    00E60108     8D4C24 20               LEA ECX,DWORD PTR SS:[ESP+0x20]
	$-1A3    00E6010C     894424 20               MOV DWORD PTR SS:[ESP+0x20],EAX
	$-19F    00E60110     83EC 18                 SUB ESP,0x18
	$-19C    00E60113     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-199    00E60116     894424 3C               MOV DWORD PTR SS:[ESP+0x3C],EAX
	$-195    00E6011A     8D4424 2B               LEA EAX,DWORD PTR SS:[ESP+0x2B]
	$-191    00E6011E     F30F7E4424 38           MOVQ XMM0,QWORD PTR SS:[ESP+0x38]
	$-18B    00E60124     894424 40               MOV DWORD PTR SS:[ESP+0x40],EAX
	$-187    00E60128     8BC4                    MOV EAX,ESP
	$-185    00E6012A     660FD64424 38           MOVQ QWORD PTR SS:[ESP+0x38],XMM0
	$-17F    00E60130     83EC 18                 SUB ESP,0x18
	$-17C    00E60133     F30F7E4424 70           MOVQ XMM0,QWORD PTR SS:[ESP+0x70]
	$-176    00E60139     0F1108                  MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-173    00E6013C     660FD640 10             MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-16E    00E60141     8BC4                    MOV EAX,ESP
	$-16C    00E60143     F30F7E8424 90000000     MOVQ XMM0,QWORD PTR SS:[ESP+0x90]
	$-163    00E6014C     0F1108                  MOVUPS DQWORD PTR DS:[EAX],XMM1
	$-160    00E6014F     660FD640 10             MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$-15B    00E60154     E8 971C0000             CALL PathOfEx.00E61DF0
	$-156    00E60159     8A4424 43               MOV AL,BYTE PTR SS:[ESP+0x43]
	$-152    00E6015D     83C4 30                 ADD ESP,0x30
	$-14F    00E60160     5F                      POP EDI
	$-14E    00E60161     5E                      POP ESI
	$-14D    00E60162     8BE5                    MOV ESP,EBP
	$-14B    00E60164     5D                      POP EBP
	$-14A    00E60165     C2 0800                 RETN 0x8
	$-147    00E60168     CC                      INT3
	$-146    00E60169     CC                      INT3
	$-145    00E6016A     CC                      INT3
	$-144    00E6016B     CC                      INT3
	$-143    00E6016C     CC                      INT3
	$-142    00E6016D     CC                      INT3
	$-141    00E6016E     CC                      INT3
	$-140    00E6016F     CC                      INT3
	$-13F    00E60170     837C24 08 FF            CMP DWORD PTR SS:[ESP+0x8],-0x1
	$-13A    00E60175     75 05                   JNZ SHORT PathOfEx.00E6017C
	$-138    00E60177     32C0                    XOR AL,AL
	$-136    00E60179     C2 0C00                 RETN 0xC
	$-133    00E6017C     FF7424 0C               PUSH DWORD PTR SS:[ESP+0xC]
	$-12F    00E60180     FF7424 08               PUSH DWORD PTR SS:[ESP+0x8]
	$-12B    00E60184     E8 E7F1FFFF             CALL PathOfEx.00E5F370
	$-126    00E60189     0FB6C0                  MOVZX EAX,AL
	$-123    00E6018C     394424 08               CMP DWORD PTR SS:[ESP+0x8],EAX
	$-11F    00E60190     0F97C0                  SETA AL
	$-11C    00E60193     C2 0C00                 RETN 0xC
	$-119    00E60196     CC                      INT3
	$-118    00E60197     CC                      INT3
	$-117    00E60198     CC                      INT3
	$-116    00E60199     CC                      INT3
	$-115    00E6019A     CC                      INT3
	$-114    00E6019B     CC                      INT3
	$-113    00E6019C     CC                      INT3
	$-112    00E6019D     CC                      INT3
	$-111    00E6019E     CC                      INT3
	$-110    00E6019F     CC                      INT3
	$-10F    00E601A0     55                      PUSH EBP
	$-10E    00E601A1     8BEC                    MOV EBP,ESP
	$-10C    00E601A3     83E4 F8                 AND ESP,0xFFFFFFF8
	$-109    00E601A6     83EC 10                 SUB ESP,0x10
	$-106    00E601A9     837D 10 FF              CMP DWORD PTR SS:[EBP+0x10],-0x1
	$-102    00E601AD     75 08                   JNZ SHORT PathOfEx.00E601B7
	$-100    00E601AF     32C0                    XOR AL,AL
	$-FE     00E601B1     8BE5                    MOV ESP,EBP
	$-FC     00E601B3     5D                      POP EBP
	$-FB     00E601B4     C2 1000                 RETN 0x10
	$-F8     00E601B7     8D45 10                 LEA EAX,DWORD PTR SS:[EBP+0x10]
	$-F5     00E601BA     894C24 04               MOV DWORD PTR SS:[ESP+0x4],ECX
	$-F1     00E601BE     894424 08               MOV DWORD PTR SS:[ESP+0x8],EAX
	$-ED     00E601C2     8D4D 14                 LEA ECX,DWORD PTR SS:[EBP+0x14]
	$-EA     00E601C5     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-E7     00E601C8     F30F7E4424 04           MOVQ XMM0,QWORD PTR SS:[ESP+0x4]
	$-E1     00E601CE     FF70 04                 PUSH DWORD PTR DS:[EAX+0x4]
	$-DE     00E601D1     FF30                    PUSH DWORD PTR DS:[EAX]
	$-DC     00E601D3     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-D9     00E601D6     FF70 04                 PUSH DWORD PTR DS:[EAX+0x4]
	$-D6     00E601D9     FF30                    PUSH DWORD PTR DS:[EAX]
	$-D4     00E601DB     83EC 0C                 SUB ESP,0xC
	$-D1     00E601DE     8BC4                    MOV EAX,ESP
	$-CF     00E601E0     660FD600                MOVQ QWORD PTR DS:[EAX],XMM0
	$-CB     00E601E4     8948 08                 MOV DWORD PTR DS:[EAX+0x8],ECX
	$-C8     00E601E7     E8 74110000             CALL PathOfEx.00E61360
	$-C3     00E601EC     83C4 1C                 ADD ESP,0x1C
	$-C0     00E601EF     84C0                    TEST AL,AL
	$-BE     00E601F1     0F94C0                  SETE AL
	$-BB     00E601F4     8BE5                    MOV ESP,EBP
	$-B9     00E601F6     5D                      POP EBP
	$-B8     00E601F7     C2 1000                 RETN 0x10
	$-B5     00E601FA     CC                      INT3
	$-B4     00E601FB     CC                      INT3
	$-B3     00E601FC     CC                      INT3
	$-B2     00E601FD     CC                      INT3
	$-B1     00E601FE     CC                      INT3
	$-B0     00E601FF     CC                      INT3
	$-AF     00E60200     55                      PUSH EBP
	$-AE     00E60201     8BEC                    MOV EBP,ESP
	$-AC     00E60203     83E4 F0                 AND ESP,0xFFFFFFF0
	$-A9     00E60206     83EC 20                 SUB ESP,0x20
	$-A6     00E60209     8BD1                    MOV EDX,ECX
	$-A4     00E6020B     837D 14 FF              CMP DWORD PTR SS:[EBP+0x14],-0x1
	$-A0     00E6020F     75 16                   JNZ SHORT PathOfEx.00E60227
	$-9E     00E60211     8B55 10                 MOV EDX,DWORD PTR SS:[EBP+0x10]
	$-9B     00E60214     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-98     00E60217     8B0A                    MOV ECX,DWORD PTR DS:[EDX]
	$-96     00E60219     8908                    MOV DWORD PTR DS:[EAX],ECX
	$-94     00E6021B     8B4A 04                 MOV ECX,DWORD PTR DS:[EDX+0x4]
	$-91     00E6021E     8948 04                 MOV DWORD PTR DS:[EAX+0x4],ECX
	$-8E     00E60221     8BE5                    MOV ESP,EBP
	$-8C     00E60223     5D                      POP EBP
	$-8B     00E60224     C2 1400                 RETN 0x14
	$-88     00E60227     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-85     00E6022A     83EC 10                 SUB ESP,0x10
	$-82     00E6022D     895424 20               MOV DWORD PTR SS:[ESP+0x20],EDX
	$-7E     00E60231     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-7C     00E60233     894424 18               MOV DWORD PTR SS:[ESP+0x18],EAX
	$-78     00E60237     8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-75     00E6023A     894424 1C               MOV DWORD PTR SS:[ESP+0x1C],EAX
	$-71     00E6023E     8D45 14                 LEA EAX,DWORD PTR SS:[EBP+0x14]
	$-6E     00E60241     894424 24               MOV DWORD PTR SS:[ESP+0x24],EAX
	$-6A     00E60245     8D45 18                 LEA EAX,DWORD PTR SS:[EBP+0x18]
	$-67     00E60248     894424 28               MOV DWORD PTR SS:[ESP+0x28],EAX
	$-63     00E6024C     8D4424 18               LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-5F     00E60250     894424 2C               MOV DWORD PTR SS:[ESP+0x2C],EAX
	$-5B     00E60254     8BC4                    MOV EAX,ESP
	$-59     00E60256     0F104424 20             MOVUPS XMM0,DQWORD PTR SS:[ESP+0x20]
	$-54     00E6025B     0F1100                  MOVUPS DQWORD PTR DS:[EAX],XMM0
	$-51     00E6025E     8B45 10                 MOV EAX,DWORD PTR SS:[EBP+0x10]
	$-4E     00E60261     FF70 04                 PUSH DWORD PTR DS:[EAX+0x4]
	$-4B     00E60264     FF30                    PUSH DWORD PTR DS:[EAX]
	$-49     00E60266     FF71 04                 PUSH DWORD PTR DS:[ECX+0x4]
	$-46     00E60269     FF31                    PUSH DWORD PTR DS:[ECX]
	$-44     00E6026B     E8 F0120000             CALL PathOfEx.00E61560
	$-3F     00E60270     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-3C     00E60273     83C4 20                 ADD ESP,0x20
	$-39     00E60276     8B4C24 08               MOV ECX,DWORD PTR SS:[ESP+0x8]
	$-35     00E6027A     8908                    MOV DWORD PTR DS:[EAX],ECX
	$-33     00E6027C     8B4C24 0C               MOV ECX,DWORD PTR SS:[ESP+0xC]
	$-2F     00E60280     8948 04                 MOV DWORD PTR DS:[EAX+0x4],ECX
	$-2C     00E60283     8BE5                    MOV ESP,EBP
	$-2A     00E60285     5D                      POP EBP
	$-29     00E60286     C2 1400                 RETN 0x14
	$-26     00E60289     CC                      INT3
	$-25     00E6028A     CC                      INT3
	$-24     00E6028B     CC                      INT3
	$-23     00E6028C     CC                      INT3
	$-22     00E6028D     CC                      INT3
	$-21     00E6028E     CC                      INT3                                            ; 比如迷宫的长钉会断到，函数内会改写地图数据，因为长钉的对象有TriggerableBlockage属性
	$-20     00E6028F     CC                      INT3                                            ; g_h_map_data_modify_by_entity_
	$-1F     00E60290     55                      PUSH EBP                                        ; tag_地形对象操作地图数据等方法的渲染
	$-1E     00E60291     8BEC                    MOV EBP,ESP                                     ; ecx=stCD_MapData*
	$-1C     00E60293     83E4 F0                 AND ESP,0xFFFFFFF0                              ; p1=stCD_EntityPosBased*
	$-19     00E60296     83EC 38                 SUB ESP,0x38
	$-16     00E60299     56                      PUSH ESI
	$-15     00E6029A     57                      PUSH EDI
	$-14     00E6029B     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-11     00E6029E     894C24 10               MOV DWORD PTR SS:[ESP+0x10],ECX
	$-D      00E602A2     8B0F                    MOV ECX,DWORD PTR DS:[EDI]
	$-B      00E602A4     85C9                    TEST ECX,ECX
	$-9      00E602A6     74 05                   JE SHORT PathOfEx.00E602AD
	$-7      00E602A8     E8 93CDF2FF             CALL PathOfEx.00D8D040
	$-2      00E602AD     8B07                    MOV EAX,DWORD PTR DS:[EDI]
	$ ==>    00E602AF     C74424 0C D85C6801      MOV DWORD PTR SS:[ESP+0xC],PathOfEx.01685CD8    ; ASCII 41,"nimated"
	$+8      00E602B7     8B70 2C                 MOV ESI,DWORD PTR DS:[EAX+0x2C]
	$+B      00E602BA     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+F      00E602BE     50                      PUSH EAX
	$+10     00E602BF     8D4424 18               LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+14     00E602C3     50                      PUSH EAX
	$+15     00E602C4     8D4E 14                 LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+18     00E602C7     E8 7462C3FF             CALL PathOfEx.00A96540
	$+1D     00E602CC     8B4424 14               MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+21     00E602D0     3B46 18                 CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+24     00E602D3     74 10                   JE SHORT PathOfEx.00E602E5
	$+26     00E602D5     8B48 0C                 MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+29     00E602D8     83F9 FF                 CMP ECX,-0x1
	$+2C     00E602DB     74 08                   JE SHORT PathOfEx.00E602E5
	$+2E     00E602DD     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+31     00E602E0     8B0488                  MOV EAX,DWORD PTR DS:[EAX+ECX*4]
	$+34     00E602E3     EB 02                   JMP SHORT PathOfEx.00E602E7
	$+36     00E602E5     33C0                    XOR EAX,EAX
	$+38     00E602E7     8B48 30                 MOV ECX,DWORD PTR DS:[EAX+0x30]
	$+3B     00E602EA     85C9                    TEST ECX,ECX
	$+3D     00E602EC     0F84 D1000000           JE PathOfEx.00E603C3
	$+43     00E602F2     E8 B9140000             CALL PathOfEx.00E617B0
	$+48     00E602F7     894424 14               MOV DWORD PTR SS:[ESP+0x14],EAX
	$+4C     00E602FB     8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+4F     00E602FE     8B4A 08                 MOV ECX,DWORD PTR DS:[EDX+0x8]
	$+52     00E60301     3B4A 0C                 CMP ECX,DWORD PTR DS:[EDX+0xC]
	$+55     00E60304     75 0C                   JNZ SHORT PathOfEx.00E60312
	$+57     00E60306     8B42 14                 MOV EAX,DWORD PTR DS:[EDX+0x14]
	$+5A     00E60309     3B42 18                 CMP EAX,DWORD PTR DS:[EDX+0x18]
	$+5D     00E6030C     0F84 B1000000           JE PathOfEx.00E603C3
	$+63     00E60312     8B4C24 10               MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+67     00E60316     33F6                    XOR ESI,ESI
	$+69     00E60318     8B7F 2C                 MOV EDI,DWORD PTR DS:[EDI+0x2C]
	$+6C     00E6031B     C74424 30 00000000      MOV DWORD PTR SS:[ESP+0x30],0x0
	$+74     00E60323     C74424 34 00000000      MOV DWORD PTR SS:[ESP+0x34],0x0
	$+7C     00E6032B     6B41 34 17              IMUL EAX,DWORD PTR DS:[ECX+0x34],0x17
	$+80     00E6032F     897424 0C               MOV DWORD PTR SS:[ESP+0xC],ESI
	$+84     00E60333     894424 38               MOV DWORD PTR SS:[ESP+0x38],EAX
	$+88     00E60337     6B41 38 17              IMUL EAX,DWORD PTR DS:[ECX+0x38],0x17
	$+8C     00E6033B     894424 3C               MOV DWORD PTR SS:[ESP+0x3C],EAX
	$+90     00E6033F     8B47 10                 MOV EAX,DWORD PTR DS:[EDI+0x10]
	$+93     00E60342     894424 18               MOV DWORD PTR SS:[ESP+0x18],EAX
	$+97     00E60346     8B47 14                 MOV EAX,DWORD PTR DS:[EDI+0x14]
	$+9A     00E60349     894424 1C               MOV DWORD PTR SS:[ESP+0x1C],EAX
	$+9E     00E6034D     83FE 02                 CMP ESI,0x2
	$+A1     00E60350     73 67                   JNB SHORT PathOfEx.00E603B9
	$+A3     00E60352     8D4424 18               LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+A7     00E60356     894C24 20               MOV DWORD PTR SS:[ESP+0x20],ECX
	$+AB     00E6035A     894424 24               MOV DWORD PTR SS:[ESP+0x24],EAX
	$+AF     00E6035E     8D4424 30               LEA EAX,DWORD PTR SS:[ESP+0x30]
	$+B3     00E60362     894424 28               MOV DWORD PTR SS:[ESP+0x28],EAX
	$+B7     00E60366     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+BB     00E6036A     894424 2C               MOV DWORD PTR SS:[ESP+0x2C],EAX
	$+BF     00E6036E     0F284424 20             MOVAPS XMM0,DQWORD PTR SS:[ESP+0x20]
	$+C4     00E60373     83EC 10                 SUB ESP,0x10
	$+C7     00E60376     8BCF                    MOV ECX,EDI
	$+C9     00E60378     8BC4                    MOV EAX,ESP
	$+CB     00E6037A     0F1100                  MOVUPS DQWORD PTR DS:[EAX],XMM0
	$+CE     00E6037D     E8 0EB5EFFF             CALL PathOfEx.00D5B890
	$+D3     00E60382     8B47 2C                 MOV EAX,DWORD PTR DS:[EDI+0x2C]
	$+D6     00E60385     C1E8 04                 SHR EAX,0x4
	$+D9     00E60388     51                      PUSH ECX
	$+DA     00E60389     24 01                   AND AL,0x1
	$+DC     00E6038B     F3:0F110424             MOVSS DWORD PTR SS:[ESP],XMM0
	$+E1     00E60390     0FB6C0                  MOVZX EAX,AL
	$+E4     00E60393     50                      PUSH EAX
	$+E5     00E60394     E8 A7B9C6FF             CALL PathOfEx.00ACBD40
	$+EA     00E60399     8B4C24 2C               MOV ECX,DWORD PTR SS:[ESP+0x2C]
	$+EE     00E6039D     0F28D0                  MOVAPS XMM2,XMM0
	$+F1     00E603A0     56                      PUSH ESI
	$+F2     00E603A1     E8 6A140000             CALL PathOfEx.00E61810                          ; 改写区域的函数
	$+F7     00E603A6     0F284424 20             MOVAPS XMM0,DQWORD PTR SS:[ESP+0x20]
	$+FC     00E603AB     46                      INC ESI
	$+FD     00E603AC     83FE 02                 CMP ESI,0x2
	$+100    00E603AF   ^ 72 C2                   JB SHORT PathOfEx.00E60373
	$+102    00E603B1     8B7424 0C               MOV ESI,DWORD PTR SS:[ESP+0xC]
	$+106    00E603B5     8B4C24 10               MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+10A    00E603B9     46                      INC ESI
	$+10B    00E603BA     897424 0C               MOV DWORD PTR SS:[ESP+0xC],ESI
	$+10F    00E603BE     83FE 02                 CMP ESI,0x2
	$+112    00E603C1   ^ 72 8F                   JB SHORT PathOfEx.00E60352
	$+114    00E603C3     5F                      POP EDI
	$+115    00E603C4     5E                      POP ESI
	$+116    00E603C5     8BE5                    MOV ESP,EBP
	$+118    00E603C7     5D                      POP EBP
	$+119    00E603C8     C2 0400                 RETN 0x4
	$+11C    00E603CB     CC                      INT3
	$+11D    00E603CC     CC                      INT3
	$+11E    00E603CD     CC                      INT3
	$+11F    00E603CE     CC                      INT3
	$+120    00E603CF     CC                      INT3
	$+121    00E603D0     83EC 64                 SUB ESP,0x64
	$+124    00E603D3     8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+127    00E603D6     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+129    00E603D8     53                      PUSH EBX
	$+12A    00E603D9     8B59 08                 MOV EBX,DWORD PTR DS:[ECX+0x8]
	$+12D    00E603DC     55                      PUSH EBP
	$+12E    00E603DD     56                      PUSH ESI
	$+12F    00E603DE     8B70 04                 MOV ESI,DWORD PTR DS:[EAX+0x4]
	$+132    00E603E1     037424 78               ADD ESI,DWORD PTR SS:[ESP+0x78]
	$+136    00E603E5     57                      PUSH EDI
	$+137    00E603E6     8B38                    MOV EDI,DWORD PTR DS:[EAX]
	$+139    00E603E8     037C24 78               ADD EDI,DWORD PTR SS:[ESP+0x78]
	$+13D    00E603EC     897C24 18               MOV DWORD PTR SS:[ESP+0x18],EDI
	$+141    00E603F0     897424 1C               MOV DWORD PTR SS:[ESP+0x1C],ESI
	$+145    00E603F4     895424 14               MOV DWORD PTR SS:[ESP+0x14],EDX
	$+149    00E603F8     3B3B                    CMP EDI,DWORD PTR DS:[EBX]
	$+14B    00E603FA     0F8C 08010000           JL PathOfEx.00E60508
	$+151    00E60400     3B7B 08                 CMP EDI,DWORD PTR DS:[EBX+0x8]
	$+154    00E60403     0F8D FF000000           JGE PathOfEx.00E60508
	$+15A    00E60409     3B73 04                 CMP ESI,DWORD PTR DS:[EBX+0x4]
	$+15D    00E6040C     0F8C F6000000           JL PathOfEx.00E60508
	$+163    00E60412     3B73 0C                 CMP ESI,DWORD PTR DS:[EBX+0xC]
	$+166    00E60415     0F8D ED000000           JGE PathOfEx.00E60508
	$+16C    00E6041B     8B69 0C                 MOV EBP,DWORD PTR DS:[ECX+0xC]
	$+16F    00E6041E     8D4424 18               LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+173    00E60422     8BCA                    MOV ECX,EDX
	$+175    00E60424     FF75 00                 PUSH DWORD PTR SS:[EBP]
	$+178    00E60427     50                      PUSH EAX
	$+179    00E60428     E8 43EFFFFF             CALL PathOfEx.00E5F370
	$+17E    00E6042D     84C0                    TEST AL,AL
	$+180    00E6042F     0F84 D3000000           JE PathOfEx.00E60508
	$+186    00E60435     8D47 FC                 LEA EAX,DWORD PTR DS:[EDI-0x4]
	$+189    00E60438     894424 20               MOV DWORD PTR SS:[ESP+0x20],EAX
	$+18D    00E6043C     8D4E FC                 LEA ECX,DWORD PTR DS:[ESI-0x4]
	$+190    00E6043F     894C24 24               MOV DWORD PTR SS:[ESP+0x24],ECX
	$+194    00E60443     8D47 05                 LEA EAX,DWORD PTR DS:[EDI+0x5]
	$+197    00E60446     8D4E 05                 LEA ECX,DWORD PTR DS:[ESI+0x5]
	$+19A    00E60449     894424 28               MOV DWORD PTR SS:[ESP+0x28],EAX
	$+19E    00E6044D     53                      PUSH EBX
	$+19F    00E6044E     8D4424 34               LEA EAX,DWORD PTR SS:[ESP+0x34]
	$+1A3    00E60452     894C24 30               MOV DWORD PTR SS:[ESP+0x30],ECX
	$+1A7    00E60456     50                      PUSH EAX
	$+1A8    00E60457     8D4C24 28               LEA ECX,DWORD PTR SS:[ESP+0x28]
	$+1AC    00E6045B     E8 D003F2FF             CALL PathOfEx.00D80830
	$+1B1    00E60460     0F104424 30             MOVUPS XMM0,DQWORD PTR SS:[ESP+0x30]
	$+1B6    00E60465     8B5424 30               MOV EDX,DWORD PTR SS:[ESP+0x30]
	$+1BA    00E60469     8B4C24 3C               MOV ECX,DWORD PTR SS:[ESP+0x3C]
	$+1BE    00E6046D     660F73D8 08             PSRLDQ XMM0,0x8
	$+1C3    00E60472     66:0F7EC0               MOVD EAX,MM0
	$+1C7    00E60476     895424 50               MOV DWORD PTR SS:[ESP+0x50],EDX
	$+1CB    00E6047A     894C24 54               MOV DWORD PTR SS:[ESP+0x54],ECX
	$+1CF    00E6047E     3BD0                    CMP EDX,EAX
	$+1D1    00E60480     75 0A                   JNZ SHORT PathOfEx.00E6048C
	$+1D3    00E60482     894424 50               MOV DWORD PTR SS:[ESP+0x50],EAX
	$+1D7    00E60486     894C24 54               MOV DWORD PTR SS:[ESP+0x54],ECX
	$+1DB    00E6048A     EB 10                   JMP SHORT PathOfEx.00E6049C
	$+1DD    00E6048C     8B4424 34               MOV EAX,DWORD PTR SS:[ESP+0x34]
	$+1E1    00E60490     3BC1                    CMP EAX,ECX
	$+1E3    00E60492     75 08                   JNZ SHORT PathOfEx.00E6049C
	$+1E5    00E60494     895424 50               MOV DWORD PTR SS:[ESP+0x50],EDX
	$+1E9    00E60498     894424 54               MOV DWORD PTR SS:[ESP+0x54],EAX
	$+1ED    00E6049C     8D4424 18               LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+1F1    00E604A0     894424 20               MOV DWORD PTR SS:[ESP+0x20],EAX
	$+1F5    00E604A4     8D4C24 30               LEA ECX,DWORD PTR SS:[ESP+0x30]
	$+1F9    00E604A8     8B4424 14               MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+1FD    00E604AC     894424 24               MOV DWORD PTR SS:[ESP+0x24],EAX
	$+201    00E604B0     8D4424 58               LEA EAX,DWORD PTR SS:[ESP+0x58]
	$+205    00E604B4     50                      PUSH EAX
	$+206    00E604B5     E8 4604F2FF             CALL PathOfEx.00D80900
	$+20B    00E604BA     F30F7E4424 20           MOVQ XMM0,QWORD PTR SS:[ESP+0x20]
	$+211    00E604C0     83EC 18                 SUB ESP,0x18
	$+214    00E604C3     660FD64424 38           MOVQ QWORD PTR SS:[ESP+0x38],XMM0
	$+21A    00E604C9     8BCC                    MOV ECX,ESP
	$+21C    00E604CB     0F104424 48             MOVUPS XMM0,DQWORD PTR SS:[ESP+0x48]
	$+221    00E604D0     8BD0                    MOV EDX,EAX
	$+223    00E604D2     896C24 40               MOV DWORD PTR SS:[ESP+0x40],EBP
	$+227    00E604D6     83EC 18                 SUB ESP,0x18
	$+22A    00E604D9     0F1101                  MOVUPS DQWORD PTR DS:[ECX],XMM0
	$+22D    00E604DC     8BC4                    MOV EAX,ESP
	$+22F    00E604DE     F30F7E8424 80000000     MOVQ XMM0,QWORD PTR SS:[ESP+0x80]
	$+238    00E604E7     660FD641 10             MOVQ QWORD PTR DS:[ECX+0x10],XMM0
	$+23D    00E604EC     8D4C24 50               LEA ECX,DWORD PTR SS:[ESP+0x50]
	$+241    00E604F0     0F1002                  MOVUPS XMM0,DQWORD PTR DS:[EDX]
	$+244    00E604F3     0F1100                  MOVUPS DQWORD PTR DS:[EAX],XMM0
	$+247    00E604F6     F30F7E42 10             MOVQ XMM0,QWORD PTR DS:[EDX+0x10]
	$+24C    00E604FB     660FD640 10             MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$+251    00E60500     E8 4B190000             CALL PathOfEx.00E61E50                          ; 改写地图数据
	$+256    00E60505     83C4 30                 ADD ESP,0x30
	$+259    00E60508     5F                      POP EDI
	$+25A    00E60509     5E                      POP ESI
	$+25B    00E6050A     5D                      POP EBP
	$+25C    00E6050B     5B                      POP EBX
	$+25D    00E6050C     83C4 64                 ADD ESP,0x64
	$+260    00E6050F     C2 0800                 RETN 0x8
	$+263    00E60512     CC                      INT3
	$+264    00E60513     CC                      INT3
	$+265    00E60514     CC                      INT3
	$+266    00E60515     CC                      INT3
	$+267    00E60516     CC                      INT3
	$+268    00E60517     CC                      INT3
	$+269    00E60518     CC                      INT3
	$+26A    00E60519     CC                      INT3
	$+26B    00E6051A     CC                      INT3
	$+26C    00E6051B     CC                      INT3
	$+26D    00E6051C     CC                      INT3
	$+26E    00E6051D     CC                      INT3
	$+26F    00E6051E     CC                      INT3
	$+270    00E6051F     CC                      INT3
	$+271    00E60520     53                      PUSH EBX
	$+272    00E60521     55                      PUSH EBP
	$+273    00E60522     8B6C24 0C               MOV EBP,DWORD PTR SS:[ESP+0xC]
	$+277    00E60526     56                      PUSH ESI
	$+278    00E60527     57                      PUSH EDI
	$+279    00E60528     8BF9                    MOV EDI,ECX
	$+27B    00E6052A     8B37                    MOV ESI,DWORD PTR DS:[EDI]
	$+27D    00E6052C     8B46 04                 MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+280    00E6052F     2B45 04                 SUB EAX,DWORD PTR SS:[EBP+0x4]
	$+283    00E60532     99                      CDQ
	$+284    00E60533     8BD8                    MOV EBX,EAX
	$+286    00E60535     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$+288    00E60537     2B45 00                 SUB EAX,DWORD PTR SS:[EBP]
	$+28B    00E6053A     33DA                    XOR EBX,EDX
	$+28D    00E6053C     2BDA                    SUB EBX,EDX
	$+28F    00E6053E     99                      CDQ
	$+290    00E6053F     33C2                    XOR EAX,EDX
	$+292    00E60541     0FB6CB                  MOVZX ECX,BL
	$+295    00E60544     2BC2                    SUB EAX,EDX
	$+297    00E60546     0FB6D0                  MOVZX EDX,AL
	$+29A    00E60549     3BC3                    CMP EAX,EBX
	$+29C    00E6054B     8B47 08                 MOV EAX,DWORD PTR DS:[EDI+0x8]
	$+29F    00E6054E     8B7F 04                 MOV EDI,DWORD PTR DS:[EDI+0x4]
	$+2A2    00E60551     0F4CD1                  CMOVL EDX,ECX
	$+2A5    00E60554     8BCF                    MOV ECX,EDI
	$+2A7    00E60556     895424 14               MOV DWORD PTR SS:[ESP+0x14],EDX
	$+2AB    00E6055A     8B30                    MOV ESI,DWORD PTR DS:[EAX]
	$+2AD    00E6055C     56                      PUSH ESI
	$+2AE    00E6055D     55                      PUSH EBP
	$+2AF    00E6055E     E8 0DEEFFFF             CALL PathOfEx.00E5F370
	$+2B4    00E60563     8B5424 14               MOV EDX,DWORD PTR SS:[ESP+0x14]
	$+2B8    00E60567     3AC2                    CMP AL,DL
	$+2BA    00E60569     0FB6C8                  MOVZX ECX,AL
	$+2BD    00E6056C     8D0476                  LEA EAX,DWORD PTR DS:[ESI+ESI*2]
	$+2C0    00E6056F     8B7487 3C               MOV ESI,DWORD PTR DS:[EDI+EAX*4+0x3C]
	$+2C4    00E60573     0FB6DA                  MOVZX EBX,DL
	$+2C7    00E60576     0F42D9                  CMOVB EBX,ECX
	$+2CA    00E60579     8B4F 54                 MOV ECX,DWORD PTR DS:[EDI+0x54]
	$+2CD    00E6057C     0FAF4D 04               IMUL ECX,DWORD PTR SS:[EBP+0x4]
	$+2D1    00E60580     03F1                    ADD ESI,ECX
	$+2D3    00E60582     8B4D 00                 MOV ECX,DWORD PTR SS:[EBP]
	$+2D6    00E60585     8BC1                    MOV EAX,ECX
	$+2D8    00E60587     99                      CDQ
	$+2D9    00E60588     2BC2                    SUB EAX,EDX
	$+2DB    00E6058A     D1F8                    SAR EAX,1
	$+2DD    00E6058C     03F0                    ADD ESI,EAX
	$+2DF    00E6058E     8A06                    MOV AL,BYTE PTR DS:[ESI]
	$+2E1    00E60590     F6C1 01                 TEST CL,0x1
	$+2E4    00E60593     74 10                   JE SHORT PathOfEx.00E605A5
	$+2E6    00E60595     C0E3 04                 SHL BL,0x4
	$+2E9    00E60598     24 0F                   AND AL,0xF
	$+2EB    00E6059A     0AD8                    OR BL,AL
	$+2ED    00E6059C     5F                      POP EDI
	$+2EE    00E6059D     881E                    MOV BYTE PTR DS:[ESI],BL
	$+2F0    00E6059F     5E                      POP ESI
	$+2F1    00E605A0     5D                      POP EBP
	$+2F2    00E605A1     5B                      POP EBX
	$+2F3    00E605A2     C2 0400                 RETN 0x4
	$+2F6    00E605A5     32C3                    XOR AL,BL
	$+2F8    00E605A7     5F                      POP EDI
	$+2F9    00E605A8     24 0F                   AND AL,0xF
	$+2FB    00E605AA     3006                    XOR BYTE PTR DS:[ESI],AL
	$+2FD    00E605AC     5E                      POP ESI
	$+2FE    00E605AD     5D                      POP EBP
	$+2FF    00E605AE     5B                      POP EBX
	$+300    00E605AF     C2 0400                 RETN 0x4
	$+303    00E605B2     CC                      INT3
	$+304    00E605B3     CC                      INT3
	$+305    00E605B4     CC                      INT3
	$+306    00E605B5     CC                      INT3
	$+307    00E605B6     CC                      INT3
	$+308    00E605B7     CC                      INT3
	$+309    00E605B8     CC                      INT3
	$+30A    00E605B9     CC                      INT3
	$+30B    00E605BA     CC                      INT3
	$+30C    00E605BB     CC                      INT3
	$+30D    00E605BC     CC                      INT3
	$+30E    00E605BD     CC                      INT3
	$+30F    00E605BE     CC                      INT3
	$+310    00E605BF     CC                      INT3
	$+311    00E605C0     55                      PUSH EBP
	$+312    00E605C1     8BEC                    MOV EBP,ESP
	$+314    00E605C3     6A FF                   PUSH -0x1
	$+316    00E605C5     68 481B5401             PUSH PathOfEx.01541B48
	$+31B    00E605CA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$+321    00E605D0     50                      PUSH EAX
	$+322    00E605D1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
	$+329    00E605D8     83EC 20                 SUB ESP,0x20
	$+32C    00E605DB     53                      PUSH EBX
	$+32D    00E605DC     8B5D 0C                 MOV EBX,DWORD PTR SS:[EBP+0xC]
	$+330    00E605DF     894D F0                 MOV DWORD PTR SS:[EBP-0x10],ECX
	$+333    00E605E2     56                      PUSH ESI
	$+334    00E605E3     57                      PUSH EDI
	$+335    00E605E4     8B03                    MOV EAX,DWORD PTR DS:[EBX]
	$+337    00E605E6     99                      CDQ
	$+338    00E605E7     2BC2                    SUB EAX,EDX
	$+33A    00E605E9     C745 E0 00000000        MOV DWORD PTR SS:[EBP-0x20],0x0
	$+341    00E605F0     8BC8                    MOV ECX,EAX
	$+343    00E605F2     C745 D4 00000000        MOV DWORD PTR SS:[EBP-0x2C],0x0
	$+34A    00E605F9     8B43 08                 MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+34D    00E605FC     40                      INC EAX
	$+34E    00E605FD     D1F9                    SAR ECX,1
	$+350    00E605FF     99                      CDQ
	$+351    00E60600     2BC2                    SUB EAX,EDX
	$+353    00E60602     894D E8                 MOV DWORD PTR SS:[EBP-0x18],ECX
	$+356    00E60605     D1F8                    SAR EAX,1
	$+358    00E60607     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
	$+35B    00E6060A     C745 D8 00000000        MOV DWORD PTR SS:[EBP-0x28],0x0
	$+362    00E60611     C745 DC 00000000        MOV DWORD PTR SS:[EBP-0x24],0x0
	$+369    00E60618     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
	$+370    00E6061F     2BC1                    SUB EAX,ECX
	$+372    00E60621     8B53 0C                 MOV EDX,DWORD PTR DS:[EBX+0xC]
	$+375    00E60624     2B53 04                 SUB EDX,DWORD PTR DS:[EBX+0x4]
	$+378    00E60627     0FAFD0                  IMUL EDX,EAX
	$+37B    00E6062A     03D2                    ADD EDX,EDX
	$+37D    00E6062C     74 09                   JE SHORT PathOfEx.00E60637
	$+37F    00E6062E     52                      PUSH EDX
	$+380    00E6062F     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+383    00E60632     E8 19DDEAFF             CALL PathOfEx.00D0E350
	$+388    00E60637     8B55 F0                 MOV EDX,DWORD PTR SS:[EBP-0x10]
	$+38B    00E6063A     B9 02000000             MOV ECX,0x2
	$+390    00E6063F     8B43 0C                 MOV EAX,DWORD PTR DS:[EBX+0xC]
	$+393    00E60642     894D 0C                 MOV DWORD PTR SS:[EBP+0xC],ECX
	$+396    00E60645     8D7A 3C                 LEA EDI,DWORD PTR DS:[EDX+0x3C]
	$+399    00E60648     8B73 04                 MOV ESI,DWORD PTR DS:[EBX+0x4]
	$+39C    00E6064B     3BF0                    CMP ESI,EAX
	$+39E    00E6064D     74 33                   JE SHORT PathOfEx.00E60682
	$+3A0    00E6064F     90                      NOP
	$+3A1    00E60650     8B42 54                 MOV EAX,DWORD PTR DS:[EDX+0x54]
	$+3A4    00E60653     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+3A7    00E60656     0FAFC6                  IMUL EAX,ESI
	$+3AA    00E60659     C645 E4 00              MOV BYTE PTR SS:[EBP-0x1C],0x0
	$+3AE    00E6065D     FF75 E4                 PUSH DWORD PTR SS:[EBP-0x1C]
	$+3B1    00E60660     0307                    ADD EAX,DWORD PTR DS:[EDI]
	$+3B3    00E60662     03C8                    ADD ECX,EAX
	$+3B5    00E60664     0345 E8                 ADD EAX,DWORD PTR SS:[EBP-0x18]
	$+3B8    00E60667     51                      PUSH ECX
	$+3B9    00E60668     50                      PUSH EAX
	$+3BA    00E60669     FF75 D8                 PUSH DWORD PTR SS:[EBP-0x28]
	$+3BD    00E6066C     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+3C0    00E6066F     E8 1C1B0000             CALL PathOfEx.00E62190
	$+3C5    00E60674     8B43 0C                 MOV EAX,DWORD PTR DS:[EBX+0xC]
	$+3C8    00E60677     46                      INC ESI
	$+3C9    00E60678     8B55 F0                 MOV EDX,DWORD PTR SS:[EBP-0x10]
	$+3CC    00E6067B     3BF0                    CMP ESI,EAX
	$+3CE    00E6067D   ^ 75 D1                   JNZ SHORT PathOfEx.00E60650
	$+3D0    00E6067F     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+3D3    00E60682     83C7 0C                 ADD EDI,0xC
	$+3D6    00E60685     83E9 01                 SUB ECX,0x1
	$+3D9    00E60688     894D 0C                 MOV DWORD PTR SS:[EBP+0xC],ECX
	$+3DC    00E6068B   ^ 75 BB                   JNZ SHORT PathOfEx.00E60648
	$+3DE    00E6068D     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+3E1    00E60690     8B4D D4                 MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+3E4    00E60693     5F                      POP EDI
	$+3E5    00E60694     5E                      POP ESI
	$+3E6    00E60695     8908                    MOV DWORD PTR DS:[EAX],ECX
	$+3E8    00E60697     8B4D D8                 MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+3EB    00E6069A     8948 04                 MOV DWORD PTR DS:[EAX+0x4],ECX
	$+3EE    00E6069D     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+3F1    00E606A0     8948 08                 MOV DWORD PTR DS:[EAX+0x8],ECX
	$+3F4    00E606A3     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3F7    00E606A6     5B                      POP EBX
	$+3F8    00E606A7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+3FF    00E606AE     8BE5                    MOV ESP,EBP
	$+401    00E606B0     5D                      POP EBP
	$+402    00E606B1     C2 0800                 RETN 0x8
	$+405    00E606B4     CC                      INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("c7??????????????8b????8d??????508d??????508d????e8????????8b??????3b????74??8b????83????74??8b????8b????eb??33??8b????85??0f??????????e8????????89??????8b????8b????3b????75", -0x1f);
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
	ofs << "pt_cdword	g_h_map_data_modify_by_entity_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-298    008609CF     CC                    INT3
	$-297    008609D0     83EC 08               SUB ESP,0x8
	$-294    008609D3     53                    PUSH EBX
	$-293    008609D4     8BD9                  MOV EBX,ECX
	$-291    008609D6     56                    PUSH ESI
	$-290    008609D7     57                    PUSH EDI
	$-28F    008609D8     8B43 20               MOV EAX,DWORD PTR DS:[EBX+0x20]
	$-28C    008609DB     8B73 10               MOV ESI,DWORD PTR DS:[EBX+0x10]
	$-289    008609DE     0FAFC6                IMUL EAX,ESI
	$-286    008609E1     8B7B 14               MOV EDI,DWORD PTR DS:[EBX+0x14]
	$-283    008609E4     66:0F6EC0             MOVD MM0,EAX
	$-27F    008609E8     0F5BC0                CVTDQ2PS XMM0,XMM0
	$-27C    008609EB     F3:0F5E05 04311501    DIVSS XMM0,DWORD PTR DS:[0x1153104]
	$-274    008609F3     E8 F898C8FF           CALL PathOfEx.004EA2F0
	$-26F    008609F8     2B73 1C               SUB ESI,DWORD PTR DS:[EBX+0x1C]
	$-26C    008609FB     8D4C24 0C             LEA ECX,DWORD PTR SS:[ESP+0xC]
	$-268    008609FF     F3:0F2CC0             CVTTSS2SI EAX,XMM0
	$-264    00860A03     8D5424 18             LEA EDX,DWORD PTR SS:[ESP+0x18]
	$-260    00860A07     2BF0                  SUB ESI,EAX
	$-25E    00860A09     B8 00000000           MOV EAX,0x0
	$-259    00860A0E     894424 0C             MOV DWORD PTR SS:[ESP+0xC],EAX
	$-255    00860A12     0F48F0                CMOVS ESI,EAX
	$-252    00860A15     8B4424 18             MOV EAX,DWORD PTR SS:[ESP+0x18]
	$-24E    00860A19     85C0                  TEST EAX,EAX
	$-24C    00860A1B     894424 18             MOV DWORD PTR SS:[ESP+0x18],EAX
	$-248    00860A1F     8D4424 10             LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-244    00860A23     897424 10             MOV DWORD PTR SS:[ESP+0x10],ESI
	$-240    00860A27     0F49CA                CMOVNS ECX,EDX
	$-23D    00860A2A     3B31                  CMP ESI,DWORD PTR DS:[ECX]
	$-23B    00860A2C     0F4DC1                CMOVGE EAX,ECX
	$-238    00860A2F     8B00                  MOV EAX,DWORD PTR DS:[EAX]
	$-236    00860A31     8943 14               MOV DWORD PTR DS:[EBX+0x14],EAX          ; 修改蓝值
	$-233    00860A34     3BC7                  CMP EAX,EDI                              ; g_h_modify_mp_value_
	$-231    00860A36     74 4B                 JE SHORT PathOfEx.00860A83
	$-22F    00860A38     8B4B 0C               MOV ECX,DWORD PTR DS:[EBX+0xC]
	$-22C    00860A3B     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-22A    00860A3D     FF50 4C               CALL DWORD PTR DS:[EAX+0x4C]
	$-227    00860A40     8B43 0C               MOV EAX,DWORD PTR DS:[EBX+0xC]
	$-224    00860A43     8BCB                  MOV ECX,EBX
	$-222    00860A45     8B30                  MOV ESI,DWORD PTR DS:[EAX]
	$-220    00860A47     8B03                  MOV EAX,DWORD PTR DS:[EBX]
	$-21E    00860A49     FF10                  CALL DWORD PTR DS:[EAX]
	$-21C    00860A4B     8B4B 0C               MOV ECX,DWORD PTR DS:[EBX+0xC]
	$-219    00860A4E     50                    PUSH EAX
	$-218    00860A4F     FF56 50               CALL DWORD PTR DS:[ESI+0x50]
	$-215    00860A52     8B73 14               MOV ESI,DWORD PTR DS:[EBX+0x14]
	$-212    00860A55     85F6                  TEST ESI,ESI
	$-210    00860A57     75 14                 JNZ SHORT PathOfEx.00860A6D
	$-20E    00860A59     8B4B 0C               MOV ECX,DWORD PTR DS:[EBX+0xC]
	$-20B    00860A5C     FF73 08               PUSH DWORD PTR DS:[EBX+0x8]
	$-208    00860A5F     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-206    00860A61     FF50 60               CALL DWORD PTR DS:[EAX+0x60]
	$-203    00860A64     5F                    POP EDI
	$-202    00860A65     5E                    POP ESI
	$-201    00860A66     5B                    POP EBX
	$-200    00860A67     83C4 08               ADD ESP,0x8
	$-1FD    00860A6A     C2 0400               RETN 0x4
	$-1FA    00860A6D     8BCB                  MOV ECX,EBX
	$-1F8    00860A6F     E8 0C98C8FF           CALL PathOfEx.004EA280
	$-1F3    00860A74     3BF0                  CMP ESI,EAX
	$-1F1    00860A76     75 0B                 JNZ SHORT PathOfEx.00860A83
	$-1EF    00860A78     8B4B 0C               MOV ECX,DWORD PTR DS:[EBX+0xC]
	$-1EC    00860A7B     FF73 08               PUSH DWORD PTR DS:[EBX+0x8]
	$-1E9    00860A7E     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-1E7    00860A80     FF50 58               CALL DWORD PTR DS:[EAX+0x58]
	$-1E4    00860A83     5F                    POP EDI
	$-1E3    00860A84     5E                    POP ESI
	$-1E2    00860A85     5B                    POP EBX
	$-1E1    00860A86     83C4 08               ADD ESP,0x8
	$-1DE    00860A89     C2 0400               RETN 0x4
	$-1DB    00860A8C     CC                    INT3
	$-1DA    00860A8D     CC                    INT3
	$-1D9    00860A8E     CC                    INT3
	$-1D8    00860A8F     CC                    INT3
	$-1D7    00860A90     51                    PUSH ECX
	$-1D6    00860A91     56                    PUSH ESI
	$-1D5    00860A92     8BF1                  MOV ESI,ECX
	$-1D3    00860A94     837E 14 00            CMP DWORD PTR DS:[ESI+0x14],0x0
	$-1CF    00860A98     0F8E 23010000         JLE PathOfEx.00860BC1
	$-1C9    00860A9E     8B56 0C               MOV EDX,DWORD PTR DS:[ESI+0xC]
	$-1C6    00860AA1     53                    PUSH EBX
	$-1C5    00860AA2     55                    PUSH EBP
	$-1C4    00860AA3     57                    PUSH EDI
	$-1C3    00860AA4     8B42 10               MOV EAX,DWORD PTR DS:[EDX+0x10]
	$-1C0    00860AA7     32DB                  XOR BL,BL
	$-1BE    00860AA9     68 950C0000           PUSH 0xC95
	$-1B9    00860AAE     885C24 17             MOV BYTE PTR SS:[ESP+0x17],BL
	$-1B5    00860AB2     8B48 7C               MOV ECX,DWORD PTR DS:[EAX+0x7C]
	$-1B2    00860AB5     8B42 04               MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-1AF    00860AB8     8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-1AC    00860ABB     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
	$-1A9    00860ABE     8B78 10               MOV EDI,DWORD PTR DS:[EAX+0x10]
	$-1A6    00860AC1     8BCF                  MOV ECX,EDI
	$-1A4    00860AC3     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-1A2    00860AC5     FF10                  CALL DWORD PTR DS:[EAX]
	$-1A0    00860AC7     8B6C24 18             MOV EBP,DWORD PTR SS:[ESP+0x18]
	$-19C    00860ACB     85C0                  TEST EAX,EAX
	$-19A    00860ACD     7E 63                 JLE SHORT PathOfEx.00860B32
	$-198    00860ACF     66:0F6EC8             MOVD MM1,EAX
	$-194    00860AD3     0F5BC9                CVTDQ2PS XMM1,XMM1
	$-191    00860AD6     8B4E 14               MOV ECX,DWORD PTR DS:[ESI+0x14]
	$-18E    00860AD9     66:0F6E46 10          MOVD MM0,DWORD PTR DS:[ESI+0x10]
	$-189    00860ADE     8D0429                LEA EAX,DWORD PTR DS:[ECX+EBP]
	$-186    00860AE1     F3:0F5E0D 04311501    DIVSS XMM1,DWORD PTR DS:[0x1153104]
	$-17E    00860AE9     0F5BC0                CVTDQ2PS XMM0,XMM0
	$-17B    00860AEC     F3:0F59C8             MULSS XMM1,XMM0
	$-177    00860AF0     F3:0F2CD9             CVTTSS2SI EBX,XMM1
	$-173    00860AF4     3BC3                  CMP EAX,EBX
	$-171    00860AF6     7F 36                 JG SHORT PathOfEx.00860B2E
	$-16F    00860AF8     2BD9                  SUB EBX,ECX
	$-16D    00860AFA     83C3 01               ADD EBX,0x1
	$-16A    00860AFD     74 1A                 JE SHORT PathOfEx.00860B19
	$-168    00860AFF     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-166    00860B01     8BCF                  MOV ECX,EDI
	$-164    00860B03     68 B3140000           PUSH 0x14B3
	$-15F    00860B08     FF10                  CALL DWORD PTR DS:[EAX]
	$-15D    00860B0A     85C0                  TEST EAX,EAX
	$-15B    00860B0C     75 0B                 JNZ SHORT PathOfEx.00860B19
	$-159    00860B0E     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-156    00860B11     83C1 64               ADD ECX,0x64
	$-153    00860B14     E8 07140000           CALL PathOfEx.00861F20
	$-14E    00860B19     8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-14B    00860B1C     8BCE                  MOV ECX,ESI
	$-149    00860B1E     03C3                  ADD EAX,EBX
	$-147    00860B20     50                    PUSH EAX
	$-146    00860B21     E8 AAFEFFFF           CALL PathOfEx.008609D0
	$-141    00860B26     5F                    POP EDI
	$-140    00860B27     5D                    POP EBP
	$-13F    00860B28     5B                    POP EBX
	$-13E    00860B29     5E                    POP ESI
	$-13D    00860B2A     59                    POP ECX
	$-13C    00860B2B     C2 0800               RETN 0x8
	$-139    00860B2E     8A5C24 13             MOV BL,BYTE PTR SS:[ESP+0x13]
	$-135    00860B32     8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-132    00860B35     03C5                  ADD EAX,EBP
	$-130    00860B37     85C0                  TEST EAX,EAX
	$-12E    00860B39     7F 42                 JG SHORT PathOfEx.00860B7D
	$-12C    00860B3B     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-12A    00860B3D     8BCF                  MOV ECX,EDI
	$-128    00860B3F     68 93010000           PUSH 0x193
	$-123    00860B44     FF10                  CALL DWORD PTR DS:[EAX]
	$-121    00860B46     85C0                  TEST EAX,EAX
	$-11F    00860B48     74 31                 JE SHORT PathOfEx.00860B7B
	$-11D    00860B4A     837E 14 01            CMP DWORD PTR DS:[ESI+0x14],0x1
	$-119    00860B4E     7E 1A                 JLE SHORT PathOfEx.00860B6A
	$-117    00860B50     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-115    00860B52     8BCF                  MOV ECX,EDI
	$-113    00860B54     68 B3140000           PUSH 0x14B3
	$-10E    00860B59     FF10                  CALL DWORD PTR DS:[EAX]
	$-10C    00860B5B     85C0                  TEST EAX,EAX
	$-10A    00860B5D     75 0B                 JNZ SHORT PathOfEx.00860B6A
	$-108    00860B5F     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-105    00860B62     83C1 64               ADD ECX,0x64
	$-102    00860B65     E8 B6130000           CALL PathOfEx.00861F20
	$-FD     00860B6A     6A 01                 PUSH 0x1
	$-FB     00860B6C     8BCE                  MOV ECX,ESI
	$-F9     00860B6E     E8 5DFEFFFF           CALL PathOfEx.008609D0
	$-F4     00860B73     5F                    POP EDI
	$-F3     00860B74     5D                    POP EBP
	$-F2     00860B75     5B                    POP EBX
	$-F1     00860B76     5E                    POP ESI
	$-F0     00860B77     59                    POP ECX
	$-EF     00860B78     C2 0800               RETN 0x8
	$-EC     00860B7B     B3 01                 MOV BL,0x1
	$-EA     00860B7D     85ED                  TEST EBP,EBP
	$-E8     00860B7F     79 1A                 JNS SHORT PathOfEx.00860B9B
	$-E6     00860B81     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-E4     00860B83     8BCF                  MOV ECX,EDI
	$-E2     00860B85     68 B3140000           PUSH 0x14B3
	$-DD     00860B8A     FF10                  CALL DWORD PTR DS:[EAX]
	$-DB     00860B8C     85C0                  TEST EAX,EAX
	$-D9     00860B8E     75 0B                 JNZ SHORT PathOfEx.00860B9B
	$-D7     00860B90     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-D4     00860B93     83C1 64               ADD ECX,0x64
	$-D1     00860B96     E8 85130000           CALL PathOfEx.00861F20
	$-CC     00860B9B     8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-C9     00860B9E     8BCE                  MOV ECX,ESI
	$-C7     00860BA0     03C5                  ADD EAX,EBP
	$-C5     00860BA2     50                    PUSH EAX
	$-C4     00860BA3     E8 28FEFFFF           CALL PathOfEx.008609D0
	$-BF     00860BA8     837E 14 00            CMP DWORD PTR DS:[ESI+0x14],0x0
	$-BB     00860BAC     7F 10                 JG SHORT PathOfEx.00860BBE
	$-B9     00860BAE     84DB                  TEST BL,BL
	$-B7     00860BB0     74 0C                 JE SHORT PathOfEx.00860BBE
	$-B5     00860BB2     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-B2     00860BB5     FF7424 1C             PUSH DWORD PTR SS:[ESP+0x1C]
	$-AE     00860BB9     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-AC     00860BBB     FF50 48               CALL DWORD PTR DS:[EAX+0x48]
	$-A9     00860BBE     5F                    POP EDI
	$-A8     00860BBF     5D                    POP EBP
	$-A7     00860BC0     5B                    POP EBX
	$-A6     00860BC1     5E                    POP ESI
	$-A5     00860BC2     59                    POP ECX
	$-A4     00860BC3     C2 0800               RETN 0x8
	$-A1     00860BC6     CC                    INT3
	$-A0     00860BC7     CC                    INT3
	$-9F     00860BC8     CC                    INT3
	$-9E     00860BC9     CC                    INT3
	$-9D     00860BCA     CC                    INT3
	$-9C     00860BCB     CC                    INT3
	$-9B     00860BCC     CC                    INT3
	$-9A     00860BCD     CC                    INT3
	$-99     00860BCE     CC                    INT3
	$-98     00860BCF     CC                    INT3
	$-97     00860BD0     56                    PUSH ESI
	$-96     00860BD1     8B7424 08             MOV ESI,DWORD PTR SS:[ESP+0x8]
	$-92     00860BD5     57                    PUSH EDI
	$-91     00860BD6     8BF9                  MOV EDI,ECX
	$-8F     00860BD8     8BCE                  MOV ECX,ESI
	$-8D     00860BDA     6A 04                 PUSH 0x4
	$-8B     00860BDC     E8 4F3EC5FF           CALL PathOfEx.004B4A30
	$-86     00860BE1     8B46 08               MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-83     00860BE4     8BCE                  MOV ECX,ESI
	$-81     00860BE6     8B16                  MOV EDX,DWORD PTR DS:[ESI]
	$-7F     00860BE8     6A 04                 PUSH 0x4
	$-7D     00860BEA     8B0402                MOV EAX,DWORD PTR DS:[EDX+EAX]
	$-7A     00860BED     8947 14               MOV DWORD PTR DS:[EDI+0x14],EAX
	$-77     00860BF0     8346 08 04            ADD DWORD PTR DS:[ESI+0x8],0x4
	$-73     00860BF4     E8 373EC5FF           CALL PathOfEx.004B4A30
	$-6E     00860BF9     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$-6C     00860BFB     8B46 08               MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-69     00860BFE     6A 01                 PUSH 0x1
	$-67     00860C00     8B0401                MOV EAX,DWORD PTR DS:[ECX+EAX]
	$-64     00860C03     8BCE                  MOV ECX,ESI
	$-62     00860C05     8947 1C               MOV DWORD PTR DS:[EDI+0x1C],EAX
	$-5F     00860C08     8346 08 04            ADD DWORD PTR DS:[ESI+0x8],0x4
	$-5B     00860C0C     E8 1F3EC5FF           CALL PathOfEx.004B4A30
	$-56     00860C11     8B4E 08               MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-53     00860C14     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-51     00860C16     6A 04                 PUSH 0x4
	$-4F     00860C18     8A1401                MOV DL,BYTE PTR DS:[ECX+EAX]
	$-4C     00860C1B     8D41 01               LEA EAX,DWORD PTR DS:[ECX+0x1]
	$-49     00860C1E     8946 08               MOV DWORD PTR DS:[ESI+0x8],EAX
	$-46     00860C21     8BCE                  MOV ECX,ESI
	$-44     00860C23     0FBEC2                MOVSX EAX,DL
	$-41     00860C26     8947 20               MOV DWORD PTR DS:[EDI+0x20],EAX
	$-3E     00860C29     E8 023EC5FF           CALL PathOfEx.004B4A30
	$-39     00860C2E     8B46 08               MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-36     00860C31     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$-34     00860C33     8B0401                MOV EAX,DWORD PTR DS:[ECX+EAX]
	$-31     00860C36     8947 18               MOV DWORD PTR DS:[EDI+0x18],EAX
	$-2E     00860C39     8346 08 04            ADD DWORD PTR DS:[ESI+0x8],0x4
	$-2A     00860C3D     5F                    POP EDI
	$-29     00860C3E     5E                    POP ESI
	$-28     00860C3F     C2 0400               RETN 0x4
	$-25     00860C42     CC                    INT3
	$-24     00860C43     CC                    INT3
	$-23     00860C44     CC                    INT3
	$-22     00860C45     CC                    INT3
	$-21     00860C46     CC                    INT3
	$-20     00860C47     CC                    INT3
	$-1F     00860C48     CC                    INT3
	$-1E     00860C49     CC                    INT3
	$-1D     00860C4A     CC                    INT3
	$-1C     00860C4B     CC                    INT3
	$-1B     00860C4C     CC                    INT3
	$-1A     00860C4D     CC                    INT3
	$-19     00860C4E     CC                    INT3
	$-18     00860C4F     CC                    INT3
	$-17     00860C50     51                    PUSH ECX
	$-16     00860C51     8B5424 08             MOV EDX,DWORD PTR SS:[ESP+0x8]
	$-12     00860C55     53                    PUSH EBX
	$-11     00860C56     55                    PUSH EBP
	$-10     00860C57     56                    PUSH ESI
	$-F      00860C58     8B42 14               MOV EAX,DWORD PTR DS:[EDX+0x14]
	$-C      00860C5B     57                    PUSH EDI
	$-B      00860C5C     8BF9                  MOV EDI,ECX
	$-9      00860C5E     8B5F 14               MOV EBX,DWORD PTR DS:[EDI+0x14]
	$-6      00860C61     8B4F 1C               MOV ECX,DWORD PTR DS:[EDI+0x1C]
	$-3      00860C64     8B77 20               MOV ESI,DWORD PTR DS:[EDI+0x20]
	$ ==>    00860C67     8947 14               MOV DWORD PTR DS:[EDI+0x14],EAX          ; 修改血值、蓝值、护盾
	$+3      00860C6A     8B42 18               MOV EAX,DWORD PTR DS:[EDX+0x18]          ; g_h_modify_life_data_
	$+6      00860C6D     8947 1C               MOV DWORD PTR DS:[EDI+0x1C],EAX
	$+9      00860C70     0FB642 1D             MOVZX EAX,BYTE PTR DS:[EDX+0x1D]
	$+D      00860C74     8947 20               MOV DWORD PTR DS:[EDI+0x20],EAX
	$+10     00860C77     395F 14               CMP DWORD PTR DS:[EDI+0x14],EBX
	$+13     00860C7A     75 09                 JNZ SHORT PathOfEx.00860C85
	$+15     00860C7C     394F 1C               CMP DWORD PTR DS:[EDI+0x1C],ECX
	$+18     00860C7F     75 04                 JNZ SHORT PathOfEx.00860C85
	$+1A     00860C81     3BC6                  CMP EAX,ESI
	$+1C     00860C83     74 08                 JE SHORT PathOfEx.00860C8D
	$+1E     00860C85     8B4F 0C               MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+21     00860C88     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+23     00860C8A     FF50 4C               CALL DWORD PTR DS:[EAX+0x4C]
	$+26     00860C8D     8B6F 14               MOV EBP,DWORD PTR DS:[EDI+0x14]
	$+29     00860C90     3BEB                  CMP EBP,EBX
	$+2B     00860C92     74 4C                 JE SHORT PathOfEx.00860CE0
	$+2D     00860C94     8B47 20               MOV EAX,DWORD PTR DS:[EDI+0x20]
	$+30     00860C97     8B77 10               MOV ESI,DWORD PTR DS:[EDI+0x10]
	$+33     00860C9A     0FAFC6                IMUL EAX,ESI
	$+36     00860C9D     C74424 18 00000000    MOV DWORD PTR SS:[ESP+0x18],0x0
	$+3E     00860CA5     66:0F6EC0             MOVD MM0,EAX
	$+42     00860CA9     0F5BC0                CVTDQ2PS XMM0,XMM0
	$+45     00860CAC     F3:0F5E05 04311501    DIVSS XMM0,DWORD PTR DS:[0x1153104]
	$+4D     00860CB4     E8 3796C8FF           CALL PathOfEx.004EA2F0
	$+52     00860CB9     2B77 1C               SUB ESI,DWORD PTR DS:[EDI+0x1C]
	$+55     00860CBC     8D4C24 10             LEA ECX,DWORD PTR SS:[ESP+0x10]
	$+59     00860CC0     F3:0F2CC0             CVTTSS2SI EAX,XMM0
	$+5D     00860CC4     2BF0                  SUB ESI,EAX
	$+5F     00860CC6     8D4424 18             LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+63     00860CCA     897424 10             MOV DWORD PTR SS:[ESP+0x10],ESI
	$+67     00860CCE     0F49C1                CMOVNS EAX,ECX
	$+6A     00860CD1     3B28                  CMP EBP,DWORD PTR DS:[EAX]
	$+6C     00860CD3     75 0B                 JNZ SHORT PathOfEx.00860CE0
	$+6E     00860CD5     8B4F 0C               MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+71     00860CD8     FF77 08               PUSH DWORD PTR DS:[EDI+0x8]
	$+74     00860CDB     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+76     00860CDD     FF50 58               CALL DWORD PTR DS:[EAX+0x58]
	$+79     00860CE0     8B47 14               MOV EAX,DWORD PTR DS:[EDI+0x14]
	$+7C     00860CE3     3BC3                  CMP EAX,EBX
	$+7E     00860CE5     74 0F                 JE SHORT PathOfEx.00860CF6
	$+80     00860CE7     85C0                  TEST EAX,EAX
	$+82     00860CE9     75 0B                 JNZ SHORT PathOfEx.00860CF6
	$+84     00860CEB     8B4F 0C               MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+87     00860CEE     FF77 08               PUSH DWORD PTR DS:[EDI+0x8]
	$+8A     00860CF1     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+8C     00860CF3     FF50 60               CALL DWORD PTR DS:[EAX+0x60]
	$+8F     00860CF6     5F                    POP EDI
	$+90     00860CF7     5E                    POP ESI
	$+91     00860CF8     5D                    POP EBP
	$+92     00860CF9     5B                    POP EBX
	$+93     00860CFA     59                    POP ECX
	$+94     00860CFB     C2 0400               RETN 0x4
	$+97     00860CFE     CC                    INT3
	$+98     00860CFF     CC                    INT3
	$+99     00860D00     53                    PUSH EBX
	$+9A     00860D01     8B5C24 08             MOV EBX,DWORD PTR SS:[ESP+0x8]
	$+9E     00860D05     56                    PUSH ESI
	$+9F     00860D06     8BF1                  MOV ESI,ECX
	$+A1     00860D08     57                    PUSH EDI
	$+A2     00860D09     8B7E 14               MOV EDI,DWORD PTR DS:[ESI+0x14]
	$+A5     00860D0C     397B 14               CMP DWORD PTR DS:[EBX+0x14],EDI
	$+A8     00860D0F     7D 2D                 JGE SHORT PathOfEx.00860D3E
	$+AA     00860D11     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+AD     00860D14     68 B3140000           PUSH 0x14B3
	$+B2     00860D19     8B41 10               MOV EAX,DWORD PTR DS:[ECX+0x10]
	$+B5     00860D1C     8B50 7C               MOV EDX,DWORD PTR DS:[EAX+0x7C]
	$+B8     00860D1F     8B41 04               MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+BB     00860D22     8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+BE     00860D25     8B0490                MOV EAX,DWORD PTR DS:[EAX+EDX*4]
	$+C1     00860D28     8B48 10               MOV ECX,DWORD PTR DS:[EAX+0x10]
	$+C4     00860D2B     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+C6     00860D2D     FF10                  CALL DWORD PTR DS:[EAX]
	$+C8     00860D2F     85C0                  TEST EAX,EAX
	$+CA     00860D31     75 0B                 JNZ SHORT PathOfEx.00860D3E
	$+CC     00860D33     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+CF     00860D36     83C1 64               ADD ECX,0x64
	$+D2     00860D39     E8 E2110000           CALL PathOfEx.00861F20
	$+D7     00860D3E     53                    PUSH EBX
	$+D8     00860D3F     8BCE                  MOV ECX,ESI
	$+DA     00860D41     E8 0AFFFFFF           CALL PathOfEx.00860C50
	$+DF     00860D46     85FF                  TEST EDI,EDI
	$+E1     00860D48     7E 1D                 JLE SHORT PathOfEx.00860D67
	$+E3     00860D4A     837E 14 00            CMP DWORD PTR DS:[ESI+0x14],0x0
	$+E7     00860D4E     7F 13                 JG SHORT PathOfEx.00860D63
	$+E9     00860D50     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+EC     00860D53     5F                    POP EDI
	$+ED     00860D54     5E                    POP ESI
	$+EE     00860D55     5B                    POP EBX
	$+EF     00860D56     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+F1     00860D58     C74424 04 00000000    MOV DWORD PTR SS:[ESP+0x4],0x0
	$+F9     00860D60     FF60 48               JMP DWORD PTR DS:[EAX+0x48]
	$+FC     00860D63     85FF                  TEST EDI,EDI
	$+FE     00860D65     7F 15                 JG SHORT PathOfEx.00860D7C
	$+100    00860D67     837E 14 00            CMP DWORD PTR DS:[ESI+0x14],0x0
	$+104    00860D6B     7E 0F                 JLE SHORT PathOfEx.00860D7C
	$+106    00860D6D     8B4E 0C               MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+109    00860D70     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+10B    00860D72     FF50 54               CALL DWORD PTR DS:[EAX+0x54]
	$+10E    00860D75     C746 18 00000000      MOV DWORD PTR DS:[ESI+0x18],0x0
	$+115    00860D7C     5F                    POP EDI
	$+116    00860D7D     5E                    POP ESI
	$+117    00860D7E     5B                    POP EBX
	$+118    00860D7F     C2 0400               RETN 0x4
	$+11B    00860D82     CC                    INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("89????8b????89????0f??????89????39????75??39", 3);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	CheckExeAddr(addr_tmp);
	ofs << "pt_cdword	g_h_modify_life_data_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-18D    0185F08F     CC                   INT3
	$-18C    0185F090     55                   PUSH EBP
	$-18B    0185F091     8BEC                 MOV EBP,ESP
	$-189    0185F093     83E4 F8              AND ESP,0xFFFFFFF8
	$-186    0185F096     83EC 08              SUB ESP,0x8
	$-183    0185F099     56                   PUSH ESI
	$-182    0185F09A     57                   PUSH EDI
	$-181    0185F09B     8BF9                 MOV EDI,ECX
	$-17F    0185F09D     0FB747 04            MOVZX EAX,WORD PTR DS:[EDI+0x4]
	$-17B    0185F0A1     50                   PUSH EAX
	$-17A    0185F0A2     FF15 0836E701        CALL DWORD PTR DS:[<&WS2_32.#9>]                    ; ws2_32.ntohs
	$-174    0185F0A8     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-171    0185F0AB     8BCE                 MOV ECX,ESI
	$-16F    0185F0AD     0FB7C0               MOVZX EAX,AX
	$-16C    0185F0B0     894424 08            MOV DWORD PTR SS:[ESP+0x8],EAX
	$-168    0185F0B4     8D4424 08            LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-164    0185F0B8     6A 02                PUSH 0x2
	$-162    0185F0BA     50                   PUSH EAX
	$-161    0185F0BB     E8 1023E9FF          CALL PathOfEx.016F13D0
	$-15C    0185F0C0     FF77 08              PUSH DWORD PTR DS:[EDI+0x8]
	$-159    0185F0C3     FF15 A035E701        CALL DWORD PTR DS:[<&WS2_32.#8>]                    ; ws2_32.ntohl
	$-153    0185F0C9     894424 08            MOV DWORD PTR SS:[ESP+0x8],EAX
	$-14F    0185F0CD     8BCE                 MOV ECX,ESI
	$-14D    0185F0CF     6A 04                PUSH 0x4
	$-14B    0185F0D1     8D4424 0C            LEA EAX,DWORD PTR SS:[ESP+0xC]
	$-147    0185F0D5     50                   PUSH EAX
	$-146    0185F0D6     E8 F522E9FF          CALL PathOfEx.016F13D0
	$-141    0185F0DB     8B47 10              MOV EAX,DWORD PTR DS:[EDI+0x10]
	$-13E    0185F0DE     8BCE                 MOV ECX,ESI
	$-13C    0185F0E0     894424 08            MOV DWORD PTR SS:[ESP+0x8],EAX
	$-138    0185F0E4     8B47 14              MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-135    0185F0E7     894424 0C            MOV DWORD PTR SS:[ESP+0xC],EAX
	$-131    0185F0EB     8D4424 08            LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-12D    0185F0EF     6A 08                PUSH 0x8
	$-12B    0185F0F1     50                   PUSH EAX
	$-12A    0185F0F2     E8 D922E9FF          CALL PathOfEx.016F13D0
	$-125    0185F0F7     5F                   POP EDI
	$-124    0185F0F8     5E                   POP ESI
	$-123    0185F0F9     8BE5                 MOV ESP,EBP
	$-121    0185F0FB     5D                   POP EBP
	$-120    0185F0FC     C2 0400              RETN 0x4
	$-11D    0185F0FF     CC                   INT3
	$-11C    0185F100     56                   PUSH ESI
	$-11B    0185F101     6A 18                PUSH 0x18
	$-119    0185F103     8BF1                 MOV ESI,ECX
	$-117    0185F105     E8 82005000          CALL PathOfEx.01D5F18C
	$-112    0185F10A     83C4 04              ADD ESP,0x4
	$-10F    0185F10D     C700 0CC40102        MOV DWORD PTR DS:[EAX],PathOfEx.0201C40C
	$-109    0185F113     66:8B56 04           MOV DX,WORD PTR DS:[ESI+0x4]
	$-105    0185F117     66:8950 04           MOV WORD PTR DS:[EAX+0x4],DX
	$-101    0185F11B     C700 F4D50102        MOV DWORD PTR DS:[EAX],PathOfEx.0201D5F4
	$-FB     0185F121     8B4E 08              MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-F8     0185F124     8948 08              MOV DWORD PTR DS:[EAX+0x8],ECX
	$-F5     0185F127     8B4E 10              MOV ECX,DWORD PTR DS:[ESI+0x10]
	$-F2     0185F12A     8948 10              MOV DWORD PTR DS:[EAX+0x10],ECX
	$-EF     0185F12D     8B4E 14              MOV ECX,DWORD PTR DS:[ESI+0x14]
	$-EC     0185F130     8948 14              MOV DWORD PTR DS:[EAX+0x14],ECX
	$-E9     0185F133     5E                   POP ESI
	$-E8     0185F134     C3                   RETN
	$-E7     0185F135     CC                   INT3
	$-E6     0185F136     CC                   INT3
	$-E5     0185F137     CC                   INT3
	$-E4     0185F138     CC                   INT3
	$-E3     0185F139     CC                   INT3
	$-E2     0185F13A     CC                   INT3
	$-E1     0185F13B     CC                   INT3
	$-E0     0185F13C     CC                   INT3
	$-DF     0185F13D     CC                   INT3
	$-DE     0185F13E     CC                   INT3
	$-DD     0185F13F     CC                   INT3
	$-DC     0185F140     0FB74424 04          MOVZX EAX,WORD PTR SS:[ESP+0x4]
	$-D7     0185F145     53                   PUSH EBX
	$-D6     0185F146     55                   PUSH EBP
	$-D5     0185F147     48                   DEC EAX
	$-D4     0185F148     33ED                 XOR EBP,EBP
	$-D2     0185F14A     3D 12010000          CMP EAX,0x112
	$-CD     0185F14F     0F87 F2160000        JA PathOfEx.01860847
	$-C7     0185F155     0FB680 FC098601      MOVZX EAX,BYTE PTR DS:[EAX+0x18609FC]
	$-C0     0185F15C     56                   PUSH ESI
	$-BF     0185F15D     57                   PUSH EDI
	$-BE     0185F15E     FF2485 50088601      JMP DWORD PTR DS:[EAX*4+0x1860850]
	$-B7     0185F165     6A 24                PUSH 0x24
	$-B5     0185F167     E8 20005000          CALL PathOfEx.01D5F18C
	$-B0     0185F16C     8B4C24 1C            MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$-AC     0185F170     8BE8                 MOV EBP,EAX
	$-AA     0185F172     83C4 04              ADD ESP,0x4
	$-A7     0185F175     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$-A3     0185F179     B8 04000000          MOV EAX,0x4
	$-9E     0185F17E     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$-9A     0185F182     8D7D 08              LEA EDI,DWORD PTR SS:[EBP+0x8]
	$-97     0185F185     C745 00 A0D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D8A0
	$-90     0185F18C     33C0                 XOR EAX,EAX
	$-8E     0185F18E     6A 04                PUSH 0x4
	$-8C     0185F190     C745 20 07000000     MOV DWORD PTR SS:[EBP+0x20],0x7
	$-85     0185F197     C745 1C 00000000     MOV DWORD PTR SS:[EBP+0x1C],0x0
	$-7E     0185F19E     57                   PUSH EDI
	$-7D     0185F19F     66:8945 0C           MOV WORD PTR SS:[EBP+0xC],AX
	$-79     0185F1A3     E8 1821E9FF          CALL PathOfEx.016F12C0
	$-74     0185F1A8     FF37                 PUSH DWORD PTR DS:[EDI]
	$-72     0185F1AA     FF15 A435E701        CALL DWORD PTR DS:[<&WS2_32.#14>]                   ; ws2_32.ntohl
	$-6C     0185F1B0     8B4C24 18            MOV ECX,DWORD PTR SS:[ESP+0x18]
	$-68     0185F1B4     8D55 0C              LEA EDX,DWORD PTR SS:[EBP+0xC]
	$-65     0185F1B7     8907                 MOV DWORD PTR DS:[EDI],EAX
	$-63     0185F1B9     E8 B22DE0FF          CALL PathOfEx.01661F70
	$-5E     0185F1BE     5F                   POP EDI
	$-5D     0185F1BF     5E                   POP ESI
	$-5C     0185F1C0     8BC5                 MOV EAX,EBP
	$-5A     0185F1C2     5D                   POP EBP
	$-59     0185F1C3     5B                   POP EBX
	$-58     0185F1C4     C2 0800              RETN 0x8
	$-55     0185F1C7     6A 08                PUSH 0x8
	$-53     0185F1C9     E8 BEFF4F00          CALL PathOfEx.01D5F18C
	$-4E     0185F1CE     83C4 04              ADD ESP,0x4
	$-4B     0185F1D1     8BE8                 MOV EBP,EAX
	$-49     0185F1D3     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$-45     0185F1D7     B8 05000000          MOV EAX,0x5
	$-40     0185F1DC     5F                   POP EDI
	$-3F     0185F1DD     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$-3B     0185F1E1     8BC5                 MOV EAX,EBP
	$-39     0185F1E3     5E                   POP ESI
	$-38     0185F1E4     C745 00 ACC20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C2AC
	$-31     0185F1EB     5D                   POP EBP
	$-30     0185F1EC     5B                   POP EBX
	$-2F     0185F1ED     C2 0800              RETN 0x8
	$-2C     0185F1F0     6A 60                PUSH 0x60
	$-2A     0185F1F2     E8 95FF4F00          CALL PathOfEx.01D5F18C
	$-25     0185F1F7     83C4 04              ADD ESP,0x4
	$-22     0185F1FA     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$-1E     0185F1FE     8BC8                 MOV ECX,EAX
	$-1C     0185F200     E8 AB93FFFF          CALL PathOfEx.018585B0
	$-17     0185F205     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$-13     0185F209     8BE8                 MOV EBP,EAX
	$-11     0185F20B     8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-E      0185F20E     E8 8D53E6FF          CALL PathOfEx.016C45A0
	$-9      0185F213     5F                   POP EDI
	$-8      0185F214     5E                   POP ESI
	$-7      0185F215     8BC5                 MOV EAX,EBP
	$-5      0185F217     5D                   POP EBP
	$-4      0185F218     5B                   POP EBX
	$-3      0185F219     C2 0800              RETN 0x8
	$ ==>    0185F21C     6A 2C                PUSH 0x2C                                           ; stCDR_PromptInfo,收到提示信息的包的时候会断到，解析收包。
	$+2      0185F21E     E8 69FF4F00          CALL PathOfEx.01D5F18C
	$+7      0185F223     83C4 04              ADD ESP,0x4
	$+A      0185F226     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+E      0185F22A     8BC8                 MOV ECX,EAX
	$+10     0185F22C     E8 BF94FFFF          CALL PathOfEx.018586F0
	$+15     0185F231     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+19     0185F235     8BE8                 MOV EBP,EAX
	$+1B     0185F237     8BCD                 MOV ECX,EBP
	$+1D     0185F239     E8 B295FFFF          CALL PathOfEx.018587F0
	$+22     0185F23E     5F                   POP EDI                                             ; g_h_prompt_info_
	$+23     0185F23F     5E                   POP ESI
	$+24     0185F240     8BC5                 MOV EAX,EBP
	$+26     0185F242     5D                   POP EBP
	$+27     0185F243     5B                   POP EBX
	$+28     0185F244     C2 0800              RETN 0x8
	$+2B     0185F247     6A 0C                PUSH 0xC
	$+2D     0185F249     E8 3EFF4F00          CALL PathOfEx.01D5F18C
	$+32     0185F24E     8BE8                 MOV EBP,EAX
	$+34     0185F250     83C4 04              ADD ESP,0x4
	$+37     0185F253     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+3B     0185F257     B8 0E000000          MOV EAX,0xE
	$+40     0185F25C     8BCD                 MOV ECX,EBP
	$+42     0185F25E     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+46     0185F262     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+4A     0185F266     C745 00 C4D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D8C4
	$+51     0185F26D     E8 8E472900          CALL PathOfEx.01AF3A00
	$+56     0185F272     5F                   POP EDI
	$+57     0185F273     5E                   POP ESI
	$+58     0185F274     8BC5                 MOV EAX,EBP
	$+5A     0185F276     5D                   POP EBP
	$+5B     0185F277     5B                   POP EBX
	$+5C     0185F278     C2 0800              RETN 0x8
	$+5F     0185F27B     68 88000000          PUSH 0x88
	$+64     0185F280     E8 07FF4F00          CALL PathOfEx.01D5F18C
	$+69     0185F285     83C4 04              ADD ESP,0x4
	$+6C     0185F288     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+70     0185F28C     8BC8                 MOV ECX,EAX
	$+72     0185F28E     E8 4D97FFFF          CALL PathOfEx.018589E0
	$+77     0185F293     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+7B     0185F297     8BE8                 MOV EBP,EAX
	$+7D     0185F299     8BCD                 MOV ECX,EBP
	$+7F     0185F29B     E8 9099FFFF          CALL PathOfEx.01858C30
	$+84     0185F2A0     5F                   POP EDI
	$+85     0185F2A1     5E                   POP ESI
	$+86     0185F2A2     8BC5                 MOV EAX,EBP
	$+88     0185F2A4     5D                   POP EBP
	$+89     0185F2A5     5B                   POP EBX
	$+8A     0185F2A6     C2 0800              RETN 0x8
	$+8D     0185F2A9     6A 0C                PUSH 0xC
	$+8F     0185F2AB     E8 DCFE4F00          CALL PathOfEx.01D5F18C
	$+94     0185F2B0     8BE8                 MOV EBP,EAX
	$+96     0185F2B2     B8 15000000          MOV EAX,0x15
	$+9B     0185F2B7     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+9F     0185F2BB     C745 00 70C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C270
	$+A6     0185F2C2     E9 1C100000          JMP PathOfEx.018602E3
	$+AB     0185F2C7     6A 68                PUSH 0x68
	$+AD     0185F2C9     E8 BEFE4F00          CALL PathOfEx.01D5F18C
	$+B2     0185F2CE     8BE8                 MOV EBP,EAX
	$+B4     0185F2D0     83C4 04              ADD ESP,0x4
	$+B7     0185F2D3     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+BB     0185F2D7     B8 16000000          MOV EAX,0x16
	$+C0     0185F2DC     8BCD                 MOV ECX,EBP
	$+C2     0185F2DE     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+C6     0185F2E2     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+CA     0185F2E6     C745 00 ACD80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D8AC
	$+D1     0185F2ED     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+D8     0185F2F4     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+DF     0185F2FB     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+E6     0185F302     E8 A99EFFFF          CALL PathOfEx.018591B0
	$+EB     0185F307     5F                   POP EDI
	$+EC     0185F308     5E                   POP ESI
	$+ED     0185F309     8BC5                 MOV EAX,EBP
	$+EF     0185F30B     5D                   POP EBP
	$+F0     0185F30C     5B                   POP EBX
	$+F1     0185F30D     C2 0800              RETN 0x8
	$+F4     0185F310     6A 20                PUSH 0x20
	$+F6     0185F312     E8 75FE4F00          CALL PathOfEx.01D5F18C
	$+FB     0185F317     8BE8                 MOV EBP,EAX
	$+FD     0185F319     83C4 04              ADD ESP,0x4
	$+100    0185F31C     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+104    0185F320     B8 1D000000          MOV EAX,0x1D
	$+109    0185F325     8BCD                 MOV ECX,EBP
	$+10B    0185F327     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+10F    0185F32B     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+113    0185F32F     C745 00 88D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D888
	$+11A    0185F336     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+121    0185F33D     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+128    0185F344     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+12F    0185F34B     C645 1A 07           MOV BYTE PTR SS:[EBP+0x1A],0x7
	$+133    0185F34F     E8 4CA1FFFF          CALL PathOfEx.018594A0
	$+138    0185F354     5F                   POP EDI
	$+139    0185F355     5E                   POP ESI
	$+13A    0185F356     8BC5                 MOV EAX,EBP
	$+13C    0185F358     5D                   POP EBP
	$+13D    0185F359     5B                   POP EBX
	$+13E    0185F35A     C2 0800              RETN 0x8
	$+141    0185F35D     6A 2C                PUSH 0x2C
	$+143    0185F35F     E8 28FE4F00          CALL PathOfEx.01D5F18C
	$+148    0185F364     83C4 04              ADD ESP,0x4
	$+14B    0185F367     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+14F    0185F36B     8BC8                 MOV ECX,EAX
	$+151    0185F36D     E8 0EA3FFFF          CALL PathOfEx.01859680
	$+156    0185F372     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+15A    0185F376     8BE8                 MOV EBP,EAX
	$+15C    0185F378     8BCD                 MOV ECX,EBP
	$+15E    0185F37A     E8 D1A4FFFF          CALL PathOfEx.01859850
	$+163    0185F37F     5F                   POP EDI
	$+164    0185F380     5E                   POP ESI
	$+165    0185F381     8BC5                 MOV EAX,EBP
	$+167    0185F383     5D                   POP EBP
	$+168    0185F384     5B                   POP EBX
	$+169    0185F385     C2 0800              RETN 0x8
	$+16C    0185F388     6A 28                PUSH 0x28
	$+16E    0185F38A     E8 FDFD4F00          CALL PathOfEx.01D5F18C
	$+173    0185F38F     8B4C24 1C            MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+177    0185F393     8BE8                 MOV EBP,EAX
	$+179    0185F395     83C4 04              ADD ESP,0x4
	$+17C    0185F398     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+180    0185F39C     B8 28000000          MOV EAX,0x28
	$+185    0185F3A1     8D5D 10              LEA EBX,DWORD PTR SS:[EBP+0x10]
	$+188    0185F3A4     0F57C0               XORPS XMM0,XMM0
	$+18B    0185F3A7     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+18F    0185F3AB     8D7D 08              LEA EDI,DWORD PTR SS:[EBP+0x8]
	$+192    0185F3AE     C745 00 70D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D870
	$+199    0185F3B5     6A 04                PUSH 0x4
	$+19B    0185F3B7     0F1103               MOVUPS DQWORD PTR DS:[EBX],XMM0
	$+19E    0185F3BA     57                   PUSH EDI
	$+19F    0185F3BB     660FD643 10          MOVQ QWORD PTR DS:[EBX+0x10],XMM0
	$+1A4    0185F3C0     E8 FB1EE9FF          CALL PathOfEx.016F12C0
	$+1A9    0185F3C5     FF37                 PUSH DWORD PTR DS:[EDI]
	$+1AB    0185F3C7     FF15 A435E701        CALL DWORD PTR DS:[<&WS2_32.#14>]                   ; ws2_32.ntohl
	$+1B1    0185F3CD     8B4C24 18            MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+1B5    0185F3D1     6A 18                PUSH 0x18
	$+1B7    0185F3D3     53                   PUSH EBX
	$+1B8    0185F3D4     8907                 MOV DWORD PTR DS:[EDI],EAX
	$+1BA    0185F3D6     E8 E51EE9FF          CALL PathOfEx.016F12C0
	$+1BF    0185F3DB     5F                   POP EDI
	$+1C0    0185F3DC     5E                   POP ESI
	$+1C1    0185F3DD     8BC5                 MOV EAX,EBP
	$+1C3    0185F3DF     5D                   POP EBP
	$+1C4    0185F3E0     5B                   POP EBX
	$+1C5    0185F3E1     C2 0800              RETN 0x8
	$+1C8    0185F3E4     6A 18                PUSH 0x18
	$+1CA    0185F3E6     E8 A1FD4F00          CALL PathOfEx.01D5F18C
	$+1CF    0185F3EB     8B4C24 1C            MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+1D3    0185F3EF     8BE8                 MOV EBP,EAX
	$+1D5    0185F3F1     83C4 04              ADD ESP,0x4
	$+1D8    0185F3F4     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+1DC    0185F3F8     B8 2B000000          MOV EAX,0x2B
	$+1E1    0185F3FD     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+1E5    0185F401     8D45 08              LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+1E8    0185F404     6A 10                PUSH 0x10
	$+1EA    0185F406     50                   PUSH EAX
	$+1EB    0185F407     C745 00 64D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D864
	$+1F2    0185F40E     E8 AD1EE9FF          CALL PathOfEx.016F12C0
	$+1F7    0185F413     5F                   POP EDI
	$+1F8    0185F414     5E                   POP ESI
	$+1F9    0185F415     8BC5                 MOV EAX,EBP
	$+1FB    0185F417     5D                   POP EBP
	$+1FC    0185F418     5B                   POP EBX
	$+1FD    0185F419     C2 0800              RETN 0x8
	$+200    0185F41C     6A 14                PUSH 0x14
	$+202    0185F41E     E8 69FD4F00          CALL PathOfEx.01D5F18C
	$+207    0185F423     8BE8                 MOV EBP,EAX
	$+209    0185F425     83C4 04              ADD ESP,0x4
	$+20C    0185F428     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+210    0185F42C     B8 2D000000          MOV EAX,0x2D
	$+215    0185F431     8BCD                 MOV ECX,EBP
	$+217    0185F433     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+21B    0185F437     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+21F    0185F43B     C745 00 58D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D858
	$+226    0185F442     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+22D    0185F449     C645 0C 00           MOV BYTE PTR SS:[EBP+0xC],0x0
	$+231    0185F44D     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+238    0185F454     E8 B7A7FFFF          CALL PathOfEx.01859C10
	$+23D    0185F459     5F                   POP EDI
	$+23E    0185F45A     5E                   POP ESI
	$+23F    0185F45B     8BC5                 MOV EAX,EBP
	$+241    0185F45D     5D                   POP EBP
	$+242    0185F45E     5B                   POP EBX
	$+243    0185F45F     C2 0800              RETN 0x8
	$+246    0185F462     6A 10                PUSH 0x10
	$+248    0185F464     E8 23FD4F00          CALL PathOfEx.01D5F18C
	$+24D    0185F469     8BE8                 MOV EBP,EAX
	$+24F    0185F46B     83C4 04              ADD ESP,0x4
	$+252    0185F46E     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+256    0185F472     B8 3B000000          MOV EAX,0x3B
	$+25B    0185F477     8BCD                 MOV ECX,EBP
	$+25D    0185F479     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+261    0185F47D     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+265    0185F481     C745 00 78C30102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C378
	$+26C    0185F488     E8 53A8FFFF          CALL PathOfEx.01859CE0
	$+271    0185F48D     5F                   POP EDI
	$+272    0185F48E     5E                   POP ESI
	$+273    0185F48F     8BC5                 MOV EAX,EBP
	$+275    0185F491     5D                   POP EBP
	$+276    0185F492     5B                   POP EBX
	$+277    0185F493     C2 0800              RETN 0x8
	$+27A    0185F496     6A 0C                PUSH 0xC
	$+27C    0185F498     E8 EFFC4F00          CALL PathOfEx.01D5F18C
	$+281    0185F49D     8BE8                 MOV EBP,EAX
	$+283    0185F49F     B8 0E010000          MOV EAX,0x10E
	$+288    0185F4A4     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+28C    0185F4A8     C745 00 70C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C270
	$+293    0185F4AF     E9 2F0E0000          JMP PathOfEx.018602E3
	$+298    0185F4B4     6A 0C                PUSH 0xC
	$+29A    0185F4B6     E8 D1FC4F00          CALL PathOfEx.01D5F18C
	$+29F    0185F4BB     8BE8                 MOV EBP,EAX
	$+2A1    0185F4BD     B8 0F010000          MOV EAX,0x10F
	$+2A6    0185F4C2     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+2AA    0185F4C6     C745 00 24C40102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C424
	$+2B1    0185F4CD     E9 32120000          JMP PathOfEx.01860704
	$+2B6    0185F4D2     6A 08                PUSH 0x8
	$+2B8    0185F4D4     E8 B3FC4F00          CALL PathOfEx.01D5F18C
	$+2BD    0185F4D9     83C4 04              ADD ESP,0x4
	$+2C0    0185F4DC     8BE8                 MOV EBP,EAX
	$+2C2    0185F4DE     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+2C6    0185F4E2     B8 10010000          MOV EAX,0x110
	$+2CB    0185F4E7     5F                   POP EDI
	$+2CC    0185F4E8     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+2D0    0185F4EC     8BC5                 MOV EAX,EBP
	$+2D2    0185F4EE     5E                   POP ESI
	$+2D3    0185F4EF     C745 00 F4C30102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C3F4
	$+2DA    0185F4F6     5D                   POP EBP
	$+2DB    0185F4F7     5B                   POP EBX
	$+2DC    0185F4F8     C2 0800              RETN 0x8
	$+2DF    0185F4FB     6A 08                PUSH 0x8
	$+2E1    0185F4FD     E8 8AFC4F00          CALL PathOfEx.01D5F18C
	$+2E6    0185F502     83C4 04              ADD ESP,0x4
	$+2E9    0185F505     8BE8                 MOV EBP,EAX
	$+2EB    0185F507     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+2EF    0185F50B     B8 11010000          MOV EAX,0x111
	$+2F4    0185F510     5F                   POP EDI
	$+2F5    0185F511     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+2F9    0185F515     8BC5                 MOV EAX,EBP
	$+2FB    0185F517     5E                   POP ESI
	$+2FC    0185F518     C745 00 F4C30102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C3F4
	$+303    0185F51F     5D                   POP EBP
	$+304    0185F520     5B                   POP EBX
	$+305    0185F521     C2 0800              RETN 0x8
	$+308    0185F524     6A 14                PUSH 0x14
	$+30A    0185F526     E8 61FC4F00          CALL PathOfEx.01D5F18C
	$+30F    0185F52B     8BE8                 MOV EBP,EAX
	$+311    0185F52D     83C4 04              ADD ESP,0x4
	$+314    0185F530     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+318    0185F534     B8 13010000          MOV EAX,0x113
	$+31D    0185F539     8BCD                 MOV ECX,EBP
	$+31F    0185F53B     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+323    0185F53F     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+327    0185F543     C745 00 34D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D834
	$+32E    0185F54A     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+335    0185F551     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+33C    0185F558     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+343    0185F55F     E8 6CA8FFFF          CALL PathOfEx.01859DD0
	$+348    0185F564     5F                   POP EDI
	$+349    0185F565     5E                   POP ESI
	$+34A    0185F566     8BC5                 MOV EAX,EBP
	$+34C    0185F568     5D                   POP EBP
	$+34D    0185F569     5B                   POP EBX
	$+34E    0185F56A     C2 0800              RETN 0x8
	$+351    0185F56D     6A 14                PUSH 0x14
	$+353    0185F56F     E8 18FC4F00          CALL PathOfEx.01D5F18C
	$+358    0185F574     8BE8                 MOV EBP,EAX
	$+35A    0185F576     83C4 04              ADD ESP,0x4
	$+35D    0185F579     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+361    0185F57D     B8 4B000000          MOV EAX,0x4B
	$+366    0185F582     8BCD                 MOV ECX,EBP
	$+368    0185F584     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+36C    0185F588     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+370    0185F58C     C745 00 28D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D828
	$+377    0185F593     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+37E    0185F59A     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+385    0185F5A1     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+38C    0185F5A8     E8 93AAFFFF          CALL PathOfEx.0185A040
	$+391    0185F5AD     5F                   POP EDI
	$+392    0185F5AE     5E                   POP ESI
	$+393    0185F5AF     8BC5                 MOV EAX,EBP
	$+395    0185F5B1     5D                   POP EBP
	$+396    0185F5B2     5B                   POP EBX
	$+397    0185F5B3     C2 0800              RETN 0x8
	$+39A    0185F5B6     68 88000000          PUSH 0x88
	$+39F    0185F5BB     E8 CCFB4F00          CALL PathOfEx.01D5F18C
	$+3A4    0185F5C0     83C4 04              ADD ESP,0x4
	$+3A7    0185F5C3     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+3AB    0185F5C7     8BC8                 MOV ECX,EAX
	$+3AD    0185F5C9     E8 A2ABFFFF          CALL PathOfEx.0185A170
	$+3B2    0185F5CE     8B4C24 18            MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+3B6    0185F5D2     8BE8                 MOV EBP,EAX
	$+3B8    0185F5D4     8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$+3BB    0185F5D7     E8 946C2900          CALL PathOfEx.01AF6270
	$+3C0    0185F5DC     5F                   POP EDI
	$+3C1    0185F5DD     5E                   POP ESI
	$+3C2    0185F5DE     8BC5                 MOV EAX,EBP
	$+3C4    0185F5E0     5D                   POP EBP
	$+3C5    0185F5E1     5B                   POP EBX
	$+3C6    0185F5E2     C2 0800              RETN 0x8
	$+3C9    0185F5E5     6A 20                PUSH 0x20
	$+3CB    0185F5E7     E8 A0FB4F00          CALL PathOfEx.01D5F18C
	$+3D0    0185F5EC     8BE8                 MOV EBP,EAX
	$+3D2    0185F5EE     B8 55000000          MOV EAX,0x55
	$+3D7    0185F5F3     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+3DB    0185F5F7     C745 00 C4C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C2C4
	$+3E2    0185F5FE     8B4C24 1C            MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+3E6    0185F602     8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$+3E9    0185F605     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+3ED    0185F609     83C4 04              ADD ESP,0x4
	$+3F0    0185F60C     C742 14 07000000     MOV DWORD PTR DS:[EDX+0x14],0x7
	$+3F7    0185F613     33C0                 XOR EAX,EAX
	$+3F9    0185F615     C742 10 00000000     MOV DWORD PTR DS:[EDX+0x10],0x0
	$+400    0185F61C     66:8902              MOV WORD PTR DS:[EDX],AX
	$+403    0185F61F     E8 4C29E0FF          CALL PathOfEx.01661F70
	$+408    0185F624     5F                   POP EDI
	$+409    0185F625     5E                   POP ESI
	$+40A    0185F626     8BC5                 MOV EAX,EBP
	$+40C    0185F628     5D                   POP EBP
	$+40D    0185F629     5B                   POP EBX
	$+40E    0185F62A     C2 0800              RETN 0x8
	$+411    0185F62D     6A 38                PUSH 0x38
	$+413    0185F62F     E8 58FB4F00          CALL PathOfEx.01D5F18C
	$+418    0185F634     83C4 04              ADD ESP,0x4
	$+41B    0185F637     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+41F    0185F63B     8BC8                 MOV ECX,EAX
	$+421    0185F63D     E8 6EACFFFF          CALL PathOfEx.0185A2B0
	$+426    0185F642     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+42A    0185F646     8BE8                 MOV EBP,EAX
	$+42C    0185F648     8BCD                 MOV ECX,EBP
	$+42E    0185F64A     E8 11AEFFFF          CALL PathOfEx.0185A460
	$+433    0185F64F     5F                   POP EDI
	$+434    0185F650     5E                   POP ESI
	$+435    0185F651     8BC5                 MOV EAX,EBP
	$+437    0185F653     5D                   POP EBP
	$+438    0185F654     5B                   POP EBX
	$+439    0185F655     C2 0800              RETN 0x8
	$+43C    0185F658     6A 0C                PUSH 0xC
	$+43E    0185F65A     E8 2DFB4F00          CALL PathOfEx.01D5F18C
	$+443    0185F65F     8BE8                 MOV EBP,EAX
	$+445    0185F661     B8 57000000          MOV EAX,0x57
	$+44A    0185F666     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+44E    0185F66A     C745 00 24C40102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C424
	$+455    0185F671     E9 8E100000          JMP PathOfEx.01860704
	$+45A    0185F676     6A 14                PUSH 0x14
	$+45C    0185F678     E8 0FFB4F00          CALL PathOfEx.01D5F18C
	$+461    0185F67D     8BE8                 MOV EBP,EAX
	$+463    0185F67F     83C4 04              ADD ESP,0x4
	$+466    0185F682     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+46A    0185F686     B8 5A000000          MOV EAX,0x5A
	$+46F    0185F68B     8BCD                 MOV ECX,EBP
	$+471    0185F68D     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+475    0185F691     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+479    0185F695     C745 00 C8D70102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D7C8
	$+480    0185F69C     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+487    0185F6A3     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+48E    0185F6AA     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+495    0185F6B1     E8 8AC7FFFF          CALL PathOfEx.0185BE40
	$+49A    0185F6B6     5F                   POP EDI
	$+49B    0185F6B7     5E                   POP ESI
	$+49C    0185F6B8     8BC5                 MOV EAX,EBP
	$+49E    0185F6BA     5D                   POP EBP
	$+49F    0185F6BB     5B                   POP EBX
	$+4A0    0185F6BC     C2 0800              RETN 0x8
	$+4A3    0185F6BF     6A 0C                PUSH 0xC
	$+4A5    0185F6C1     E8 C6FA4F00          CALL PathOfEx.01D5F18C
	$+4AA    0185F6C6     8BE8                 MOV EBP,EAX
	$+4AC    0185F6C8     B8 45000000          MOV EAX,0x45
	$+4B1    0185F6CD     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+4B5    0185F6D1     C745 00 24C40102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C424
	$+4BC    0185F6D8     E9 27100000          JMP PathOfEx.01860704
	$+4C1    0185F6DD     6A 18                PUSH 0x18
	$+4C3    0185F6DF     E8 A8FA4F00          CALL PathOfEx.01D5F18C
	$+4C8    0185F6E4     8BE8                 MOV EBP,EAX
	$+4CA    0185F6E6     83C4 04              ADD ESP,0x4
	$+4CD    0185F6E9     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+4D1    0185F6ED     B8 49000000          MOV EAX,0x49
	$+4D6    0185F6F2     8BCD                 MOV ECX,EBP
	$+4D8    0185F6F4     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+4DC    0185F6F8     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+4E0    0185F6FC     C745 00 4CD80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D84C
	$+4E7    0185F703     C745 08 00000000     MOV DWORD PTR SS:[EBP+0x8],0x0
	$+4EE    0185F70A     C745 0C 00000000     MOV DWORD PTR SS:[EBP+0xC],0x0
	$+4F5    0185F711     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+4FC    0185F718     E8 E3AFFFFF          CALL PathOfEx.0185A700
	$+501    0185F71D     5F                   POP EDI
	$+502    0185F71E     5E                   POP ESI
	$+503    0185F71F     8BC5                 MOV EAX,EBP
	$+505    0185F721     5D                   POP EBP
	$+506    0185F722     5B                   POP EBX
	$+507    0185F723     C2 0800              RETN 0x8
	$+50A    0185F726     6A 0C                PUSH 0xC
	$+50C    0185F728     E8 5FFA4F00          CALL PathOfEx.01D5F18C
	$+511    0185F72D     8BE8                 MOV EBP,EAX
	$+513    0185F72F     B8 23000000          MOV EAX,0x23
	$+518    0185F734     896C24 18            MOV DWORD PTR SS:[ESP+0x18],EBP
	$+51C    0185F738     C745 00 70C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C270
	$+523    0185F73F     E9 9F0B0000          JMP PathOfEx.018602E3
	$+528    0185F744     6A 10                PUSH 0x10
	$+52A    0185F746     E8 41FA4F00          CALL PathOfEx.01D5F18C
	$+52F    0185F74B     8B4C24 1C            MOV ECX,DWORD PTR SS:[ESP+0x1C]
	$+533    0185F74F     8BE8                 MOV EBP,EAX
	$+535    0185F751     83C4 04              ADD ESP,0x4
	$+538    0185F754     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+53C    0185F758     B8 4A000000          MOV EAX,0x4A
	$+541    0185F75D     8D75 08              LEA ESI,DWORD PTR SS:[EBP+0x8]
	$+544    0185F760     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+548    0185F764     6A 04                PUSH 0x4
	$+54A    0185F766     56                   PUSH ESI
	$+54B    0185F767     C745 00 88C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C288
	$+552    0185F76E     E8 4D1BE9FF          CALL PathOfEx.016F12C0
	$+557    0185F773     FF36                 PUSH DWORD PTR DS:[ESI]
	$+559    0185F775     FF15 A435E701        CALL DWORD PTR DS:[<&WS2_32.#14>]                   ; ws2_32.ntohl
	$+55F    0185F77B     8B4C24 18            MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+563    0185F77F     8906                 MOV DWORD PTR DS:[ESI],EAX
	$+565    0185F781     8D45 0C              LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+568    0185F784     6A 01                PUSH 0x1
	$+56A    0185F786     50                   PUSH EAX
	$+56B    0185F787     E8 341BE9FF          CALL PathOfEx.016F12C0
	$+570    0185F78C     5F                   POP EDI
	$+571    0185F78D     5E                   POP ESI
	$+572    0185F78E     8BC5                 MOV EAX,EBP
	$+574    0185F790     5D                   POP EBP
	$+575    0185F791     5B                   POP EBX
	$+576    0185F792     C2 0800              RETN 0x8
	$+579    0185F795     6A 1C                PUSH 0x1C
	$+57B    0185F797     E8 F0F94F00          CALL PathOfEx.01D5F18C
	$+580    0185F79C     8BE8                 MOV EBP,EAX
	$+582    0185F79E     83C4 04              ADD ESP,0x4
	$+585    0185F7A1     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+589    0185F7A5     B8 5B000000          MOV EAX,0x5B
	$+58E    0185F7AA     8BCD                 MOV ECX,EBP
	$+590    0185F7AC     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+594    0185F7B0     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+598    0185F7B4     C745 00 40D80102     MOV DWORD PTR SS:[EBP],PathOfEx.0201D840
	$+59F    0185F7BB     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
	$+5A6    0185F7C2     C745 14 00000000     MOV DWORD PTR SS:[EBP+0x14],0x0
	$+5AD    0185F7C9     C745 18 00000000     MOV DWORD PTR SS:[EBP+0x18],0x0
	$+5B4    0185F7D0     E8 9BB1FFFF          CALL PathOfEx.0185A970
	$+5B9    0185F7D5     5F                   POP EDI
	$+5BA    0185F7D6     5E                   POP ESI
	$+5BB    0185F7D7     8BC5                 MOV EAX,EBP
	$+5BD    0185F7D9     5D                   POP EBP
	$+5BE    0185F7DA     5B                   POP EBX
	$+5BF    0185F7DB     C2 0800              RETN 0x8
	$+5C2    0185F7DE     6A 34                PUSH 0x34
	$+5C4    0185F7E0     E8 A7F94F00          CALL PathOfEx.01D5F18C
	$+5C9    0185F7E5     83C4 04              ADD ESP,0x4
	$+5CC    0185F7E8     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+5D0    0185F7EC     8BC8                 MOV ECX,EAX
	$+5D2    0185F7EE     E8 BDB3FFFF          CALL PathOfEx.0185ABB0
	$+5D7    0185F7F3     8B4C24 18            MOV ECX,DWORD PTR SS:[ESP+0x18]
	$+5DB    0185F7F7     8BE8                 MOV EBP,EAX
	$+5DD    0185F7F9     8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$+5E0    0185F7FC     E8 3F63F1FF          CALL PathOfEx.01775B40
	$+5E5    0185F801     5F                   POP EDI
	$+5E6    0185F802     5E                   POP ESI
	$+5E7    0185F803     8BC5                 MOV EAX,EBP
	$+5E9    0185F805     5D                   POP EBP
	$+5EA    0185F806     5B                   POP EBX
	$+5EB    0185F807     C2 0800              RETN 0x8
	$+5EE    0185F80A     6A 10                PUSH 0x10
	$+5F0    0185F80C     E8 7BF94F00          CALL PathOfEx.01D5F18C
	$+5F5    0185F811     8BE8                 MOV EBP,EAX
	$+5F7    0185F813     83C4 04              ADD ESP,0x4
	$+5FA    0185F816     896C24 14            MOV DWORD PTR SS:[ESP+0x14],EBP
	$+5FE    0185F81A     B8 64000000          MOV EAX,0x64
	$+603    0185F81F     8BCD                 MOV ECX,EBP
	$+605    0185F821     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+609    0185F825     66:8945 04           MOV WORD PTR SS:[EBP+0x4],AX
	$+60D    0185F829     C745 00 88C20102     MOV DWORD PTR SS:[EBP],PathOfEx.0201C288
	$+614    0185F830     E8 5BB5FFFF          CALL PathOfEx.0185AD90
	$+619    0185F835     5F                   POP EDI
	$+61A    0185F836     5E                   POP ESI
	$+61B    0185F837     8BC5                 MOV EAX,EBP
	$+61D    0185F839     5D                   POP EBP
	$+61E    0185F83A     5B                   POP EBX
	$+61F    0185F83B     C2 0800              RETN 0x8
	$+622    0185F83E     6A 40                PUSH 0x40
	$+624    0185F840     E8 47F94F00          CALL PathOfEx.01D5F18C
	$+629    0185F845     83C4 04              ADD ESP,0x4
	$+62C    0185F848     894424 14            MOV DWORD PTR SS:[ESP+0x14],EAX
	$+630    0185F84C     8BC8                 MOV ECX,EAX
	$+632    0185F84E     E8 0DB6FFFF          CALL PathOfEx.0185AE60
	$+637    0185F853     FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
	$+63B    0185F857     8BE8                 MOV EBP,EAX
	$+63D    0185F859     8BCD                 MOV ECX,EBP
	$+63F    0185F85B     E8 20B7FFFF          CALL PathOfEx.0185AF80
	$+644    0185F860     5F                   POP EDI
	$+645    0185F861     5E                   POP ESI
	$+646    0185F862     8BC5                 MOV EAX,EBP
	*/
	addr_tmp = scan_exe_.ScanAddr("6a??e8????????83????89??????8b??e8????????ff??????8b??8b??e8????????5f5e8b??5d5bc2????6a??e8", 0x22);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x5f)
	{
		assert(false);
		return false;
	}
	CheckExeAddr(addr_tmp);
	ofs << "pt_cdword	g_h_prompt_info_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//gj3
	/*
	$-2F4    01416E0F     CC                    INT3
	$-2F3    01416E10     55                    PUSH EBP
	$-2F2    01416E11     8BEC                  MOV EBP,ESP
	$-2F0    01416E13     6A FF                 PUSH -0x1
	$-2EE    01416E15     68 9D07BF01           PUSH PathOfEx.01BF079D
	$-2E9    01416E1A     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-2E3    01416E20     50                    PUSH EAX
	$-2E2    01416E21     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$-2DB    01416E28     81EC 84000000         SUB ESP,0x84
	$-2D5    01416E2E     56                    PUSH ESI
	$-2D4    01416E2F     57                    PUSH EDI
	$-2D3    01416E30     8BF9                  MOV EDI,ECX
	$-2D1    01416E32     E8 392A0300           CALL PathOfEx.01449870
	$-2CC    01416E37     8B0D 144DEE01         MOV ECX,DWORD PTR DS:[0x1EE4D14]
	$-2C6    01416E3D     E8 CE2A0300           CALL PathOfEx.01449910
	$-2C1    01416E42     33C0                  XOR EAX,EAX
	$-2BF    01416E44     C745 EC 07000000      MOV DWORD PTR SS:[EBP-0x14],0x7
	$-2B8    01416E4B     C745 E8 00000000      MOV DWORD PTR SS:[EBP-0x18],0x0
	$-2B1    01416E52     66:8945 D8            MOV WORD PTR SS:[EBP-0x28],AX
	$-2AD    01416E56     8945 FC               MOV DWORD PTR SS:[EBP-0x4],EAX
	$-2AA    01416E59     833D 044DEE01 0A      CMP DWORD PTR DS:[0x1EE4D04],0xA
	$-2A3    01416E60     0F84 94000000         JE PathOfEx.01416EFA
	$-29D    01416E66     83EC 10               SUB ESP,0x10
	$-29A    01416E69     E8 920F0200           CALL PathOfEx.01437E00
	$-295    01416E6E     8BC8                  MOV ECX,EAX
	$-293    01416E70     E8 CB4DD2FF           CALL PathOfEx.0113BC40
	$-28E    01416E75     8BF0                  MOV ESI,EAX
	$-28C    01416E77     8975 F0               MOV DWORD PTR SS:[EBP-0x10],ESI
	$-289    01416E7A     C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$-285    01416E7E     8D55 F0               LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-282    01416E81     FF35 044DEE01         PUSH DWORD PTR DS:[0x1EE4D04]
	$-27C    01416E87     8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-279    01416E8A     E8 314E0300           CALL PathOfEx.0144BCC0
	$-274    01416E8F     83C4 04               ADD ESP,0x4
	$-271    01416E92     C645 FC 04            MOV BYTE PTR SS:[EBP-0x4],0x4
	$-26D    01416E96     85F6                  TEST ESI,ESI
	$-26B    01416E98     74 11                 JE SHORT PathOfEx.01416EAB
	$-269    01416E9A     56                    PUSH ESI
	$-268    01416E9B     51                    PUSH ECX
	$-267    01416E9C     E8 5F0F0200           CALL PathOfEx.01437E00
	$-262    01416EA1     83C4 04               ADD ESP,0x4
	$-25F    01416EA4     8BC8                  MOV ECX,EAX
	$-25D    01416EA6     E8 A50A0200           CALL PathOfEx.01437950
	$-258    01416EAB     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$-254    01416EAF     8B4D BC               MOV ECX,DWORD PTR SS:[EBP-0x44]
	$-251    01416EB2     85C9                  TEST ECX,ECX
	$-24F    01416EB4     74 1A                 JE SHORT PathOfEx.01416ED0
	$-24D    01416EB6     8D45 C0               LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-24A    01416EB9     50                    PUSH EAX
	$-249    01416EBA     E8 316CD9FF           CALL PathOfEx.011ADAF0
	$-244    01416EBF     50                    PUSH EAX
	$-243    01416EC0     8D4D D8               LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-240    01416EC3     E8 3880D2FF           CALL PathOfEx.0113EF00
	$-23B    01416EC8     8D4D C0               LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-238    01416ECB     E8 101ED1FF           CALL PathOfEx.01128CE0
	$-233    01416ED0     C645 FC 05            MOV BYTE PTR SS:[EBP-0x4],0x5
	$-22F    01416ED4     8B45 B8               MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-22C    01416ED7     85C0                  TEST EAX,EAX
	$-22A    01416ED9     74 11                 JE SHORT PathOfEx.01416EEC
	$-228    01416EDB     50                    PUSH EAX
	$-227    01416EDC     51                    PUSH ECX
	$-226    01416EDD     E8 1E0F0200           CALL PathOfEx.01437E00
	$-221    01416EE2     83C4 04               ADD ESP,0x4
	$-21E    01416EE5     8BC8                  MOV ECX,EAX
	$-21C    01416EE7     E8 640A0200           CALL PathOfEx.01437950
	$-217    01416EEC     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-213    01416EF0     837D E8 00            CMP DWORD PTR SS:[EBP-0x18],0x0
	$-20F    01416EF4     0F85 C4000000         JNZ PathOfEx.01416FBE
	$-209    01416EFA     6A 0A                 PUSH 0xA
	$-207    01416EFC     33C0                  XOR EAX,EAX
	$-205    01416EFE     C745 D4 07000000      MOV DWORD PTR SS:[EBP-0x2C],0x7
	$-1FE    01416F05     68 F45ED401           PUSH PathOfEx.01D45EF4                                     ; UNICODE "--language"
	$-1F9    01416F0A     8D4D C0               LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-1F6    01416F0D     C745 D0 00000000      MOV DWORD PTR SS:[EBP-0x30],0x0
	$-1EF    01416F14     66:8945 C0            MOV WORD PTR SS:[EBP-0x40],AX
	$-1EB    01416F18     E8 3323D1FF           CALL PathOfEx.01129250
	$-1E6    01416F1D     8B37                  MOV ESI,DWORD PTR DS:[EDI]
	$-1E4    01416F1F     8D45 C0               LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-1E1    01416F22     50                    PUSH EAX
	$-1E0    01416F23     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-1DD    01416F26     50                    PUSH EAX
	$-1DC    01416F27     8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$-1D9    01416F2A     83C1 1C               ADD ECX,0x1C
	$-1D6    01416F2D     E8 CE40D1FF           CALL PathOfEx.0112B000
	$-1D1    01416F32     8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-1CE    01416F35     8B40 20               MOV EAX,DWORD PTR DS:[EAX+0x20]
	$-1CB    01416F38     8B40 48               MOV EAX,DWORD PTR DS:[EAX+0x48]
	$-1C8    01416F3B     8D0C40                LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$-1C5    01416F3E     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-1C3    01416F40     8B74C8 10             MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$-1BF    01416F44     8B45 D4               MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$-1BC    01416F47     83F8 08               CMP EAX,0x8
	$-1B9    01416F4A     72 0F                 JB SHORT PathOfEx.01416F5B
	$-1B7    01416F4C     40                    INC EAX
	$-1B6    01416F4D     6A 02                 PUSH 0x2
	$-1B4    01416F4F     50                    PUSH EAX
	$-1B3    01416F50     FF75 C0               PUSH DWORD PTR SS:[EBP-0x40]
	$-1B0    01416F53     E8 680DD1FF           CALL PathOfEx.01127CC0
	$-1AB    01416F58     83C4 0C               ADD ESP,0xC
	$-1A8    01416F5B     85F6                  TEST ESI,ESI
	$-1A6    01416F5D     74 3A                 JE SHORT PathOfEx.01416F99
	$-1A4    01416F5F     68 F45ED401           PUSH PathOfEx.01D45EF4                                     ; UNICODE "--language"
	$-19F    01416F64     8D4D C0               LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-19C    01416F67     E8 E423D1FF           CALL PathOfEx.01129350
	$-197    01416F6C     8D45 C0               LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-194    01416F6F     C645 FC 06            MOV BYTE PTR SS:[EBP-0x4],0x6
	$-190    01416F73     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
	$-18E    01416F75     50                    PUSH EAX
	$-18D    01416F76     8D45 A0               LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-18A    01416F79     50                    PUSH EAX
	$-189    01416F7A     E8 31ED0100           CALL PathOfEx.01435CB0
	$-184    01416F7F     50                    PUSH EAX
	$-183    01416F80     8D4D D8               LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-180    01416F83     E8 787FD2FF           CALL PathOfEx.0113EF00
	$-17B    01416F88     8D4D A0               LEA ECX,DWORD PTR SS:[EBP-0x60]
	$-178    01416F8B     E8 501DD1FF           CALL PathOfEx.01128CE0
	$-173    01416F90     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-16F    01416F94     8D4D C0               LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-16C    01416F97     EB 20                 JMP SHORT PathOfEx.01416FB9
	$-16A    01416F99     8D45 A0               LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-167    01416F9C     50                    PUSH EAX
	$-166    01416F9D     51                    PUSH ECX
	$-165    01416F9E     E8 3D370D00           CALL PathOfEx.014EA6E0
	$-160    01416FA3     83C4 04               ADD ESP,0x4
	$-15D    01416FA6     8BC8                  MOV ECX,EAX
	$-15B    01416FA8     E8 83590D00           CALL PathOfEx.014EC930
	$-156    01416FAD     50                    PUSH EAX
	$-155    01416FAE     8D4D D8               LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-152    01416FB1     E8 4A7FD2FF           CALL PathOfEx.0113EF00
	$-14D    01416FB6     8D4D A0               LEA ECX,DWORD PTR SS:[EBP-0x60]
	$-14A    01416FB9     E8 221DD1FF           CALL PathOfEx.01128CE0
	$-145    01416FBE     8D55 D8               LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-142    01416FC1     8D4D C0               LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-13F    01416FC4     E8 D7ED0000           CALL PathOfEx.01425DA0
	$-13A    01416FC9     8BF0                  MOV ESI,EAX
	$-138    01416FCB     8D45 D8               LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-135    01416FCE     3BC6                  CMP EAX,ESI
	$-133    01416FD0     74 34                 JE SHORT PathOfEx.01417006
	$-131    01416FD2     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-12E    01416FD5     83F8 08               CMP EAX,0x8
	$-12B    01416FD8     72 0F                 JB SHORT PathOfEx.01416FE9
	$-129    01416FDA     40                    INC EAX
	$-128    01416FDB     6A 02                 PUSH 0x2
	$-126    01416FDD     50                    PUSH EAX
	$-125    01416FDE     FF75 D8               PUSH DWORD PTR SS:[EBP-0x28]
	$-122    01416FE1     E8 DA0CD1FF           CALL PathOfEx.01127CC0
	$-11D    01416FE6     83C4 0C               ADD ESP,0xC
	$-11A    01416FE9     33C0                  XOR EAX,EAX
	$-118    01416FEB     C745 EC 07000000      MOV DWORD PTR SS:[EBP-0x14],0x7
	$-111    01416FF2     56                    PUSH ESI
	$-110    01416FF3     8D4D D8               LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-10D    01416FF6     C745 E8 00000000      MOV DWORD PTR SS:[EBP-0x18],0x0
	$-106    01416FFD     66:8945 D8            MOV WORD PTR SS:[EBP-0x28],AX
	$-102    01417001     E8 2A49D1FF           CALL PathOfEx.0112B930
	$-FD     01417006     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-F9     0141700A     8B45 D4               MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$-F6     0141700D     83F8 08               CMP EAX,0x8
	$-F3     01417010     72 0F                 JB SHORT PathOfEx.01417021
	$-F1     01417012     40                    INC EAX
	$-F0     01417013     6A 02                 PUSH 0x2
	$-EE     01417015     50                    PUSH EAX
	$-ED     01417016     FF75 C0               PUSH DWORD PTR SS:[EBP-0x40]
	$-EA     01417019     E8 A20CD1FF           CALL PathOfEx.01127CC0
	$-E5     0141701E     83C4 0C               ADD ESP,0xC
	$-E2     01417021     837D E8 00            CMP DWORD PTR SS:[EBP-0x18],0x0
	$-DE     01417025     0F84 AA000000         JE PathOfEx.014170D5
	$-D8     0141702B     8D4D D8               LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-D5     0141702E     E8 BD1D0300           CALL PathOfEx.01448DF0
	$-D0     01417033     8BC8                  MOV ECX,EAX
	$-CE     01417035     E8 D6280300           CALL PathOfEx.01449910
	$-C9     0141703A     84C0                  TEST AL,AL
	$-C7     0141703C     75 7C                 JNZ SHORT PathOfEx.014170BA
	$-C5     0141703E     8D45 D8               LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-C2     01417041     50                    PUSH EAX
	$-C1     01417042     8D45 A0               LEA EAX,DWORD PTR SS:[EBP-0x60]
	$-BE     01417045     50                    PUSH EAX
	$-BD     01417046     E8 15EA0000           CALL PathOfEx.01425A60
	$-B8     0141704B     50                    PUSH EAX
	$-B7     0141704C     8D45 88               LEA EAX,DWORD PTR SS:[EBP-0x78]
	$-B4     0141704F     C645 FC 08            MOV BYTE PTR SS:[EBP-0x4],0x8
	$-B0     01417053     68 7468D801           PUSH PathOfEx.01D86874                                     ; ASCII "Failed to set language to ""
	$-AB     01417058     50                    PUSH EAX
	$-AA     01417059     E8 627BD3FF           CALL PathOfEx.0114EBC0
	$-A5     0141705E     83C4 14               ADD ESP,0x14
	$-A2     01417061     68 6CF1D701           PUSH PathOfEx.01D7F16C                                     ; UNICODE """
	$-9D     01417066     50                    PUSH EAX
	$-9C     01417067     8D85 70FFFFFF         LEA EAX,DWORD PTR SS:[EBP-0x90]
	$-96     0141706D     C645 FC 09            MOV BYTE PTR SS:[EBP-0x4],0x9
	$-92     01417071     50                    PUSH EAX
	$-91     01417072     E8 B9ACD1FF           CALL PathOfEx.01131D30
	$-8C     01417077     83C4 0C               ADD ESP,0xC
	$-89     0141707A     C645 FC 0A            MOV BYTE PTR SS:[EBP-0x4],0xA
	$-85     0141707E     8378 14 10            CMP DWORD PTR DS:[EAX+0x14],0x10
	$-81     01417082     72 02                 JB SHORT PathOfEx.01417086
	$-7F     01417084     8B00                  MOV EAX,DWORD PTR DS:[EAX]
	$-7D     01417086     8945 B8               MOV DWORD PTR SS:[EBP-0x48],EAX
	$-7A     01417089     0F57C0                XORPS XMM0,XMM0
	$-77     0141708C     8D45 D0               LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-74     0141708F     C745 CC 9045CB01      MOV DWORD PTR SS:[EBP-0x34],PathOfEx.01CB4590
	$-6D     01417096     50                    PUSH EAX
	$-6C     01417097     8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$-69     0141709A     660FD645 D0           MOVQ QWORD PTR SS:[EBP-0x30],XMM0
	$-64     0141709F     50                    PUSH EAX
	$-63     014170A0     C645 BC 01            MOV BYTE PTR SS:[EBP-0x44],0x1
	$-5F     014170A4     E8 0B127300           CALL PathOfEx.01B482B4
	$-5A     014170A9     83C4 08               ADD ESP,0x8
	$-57     014170AC     8D45 CC               LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-54     014170AF     68 A4C1ED01           PUSH PathOfEx.01EDC1A4
	$-4F     014170B4     50                    PUSH EAX
	$-4E     014170B5     E8 B9227300           CALL PathOfEx.01B49373
	$-49     014170BA     E8 B1270300           CALL PathOfEx.01449870
	$-44     014170BF     FF35 184DEE01         PUSH DWORD PTR DS:[0x1EE4D18]
	$-3E     014170C5     51                    PUSH ECX
	$-3D     014170C6     E8 15360D00           CALL PathOfEx.014EA6E0
	$-38     014170CB     83C4 04               ADD ESP,0x4
	$-35     014170CE     8BC8                  MOV ECX,EAX
	$-33     014170D0     E8 5B560D00           CALL PathOfEx.014EC730
	$-2E     014170D5     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-2B     014170D8     83F8 08               CMP EAX,0x8
	$-28     014170DB     72 0F                 JB SHORT PathOfEx.014170EC
	$-26     014170DD     6A 02                 PUSH 0x2
	$-24     014170DF     40                    INC EAX
	$-23     014170E0     50                    PUSH EAX
	$-22     014170E1     FF75 D8               PUSH DWORD PTR SS:[EBP-0x28]
	$-1F     014170E4     E8 D70BD1FF           CALL PathOfEx.01127CC0
	$-1A     014170E9     83C4 0C               ADD ESP,0xC
	$-17     014170EC     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-14     014170EF     5F                    POP EDI
	$-13     014170F0     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-C      014170F7     5E                    POP ESI
	$-B      014170F8     8BE5                  MOV ESP,EBP
	$-9      014170FA     5D                    POP EBP
	$-8      014170FB     C3                    RETN
	$-7      014170FC     CC                    INT3
	$-6      014170FD     CC                    INT3
	$-5      014170FE     CC                    INT3
	$-4      014170FF     CC                    INT3
	$-3      01417100     83EC 14               SUB ESP,0x14                                               ; g_h_duo_kai_two_copies_
	$ ==>    01417103     68 9067D801           PUSH PathOfEx.01D86790                                     ; UNICODE "Global\PoERunMutexA"
	$+5      01417108     6A 00                 PUSH 0x0
	$+7      0141710A     6A 00                 PUSH 0x0
	$+9      0141710C     FF15 1431C401         CALL DWORD PTR DS:[<&KERNEL32.CreateMutexW>]               ; kernel32.CreateMutexW
	$+F      01417112     85C0                  TEST EAX,EAX
	$+11     01417114     74 3B                 JE SHORT PathOfEx.01417151
	$+13     01417116     6A 00                 PUSH 0x0
	$+15     01417118     50                    PUSH EAX
	$+16     01417119     FF15 A431C401         CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]        ; kernel32.WaitForSingleObject
	$+1C     0141711F     3D 02010000           CMP EAX,0x102
	$+21     01417124     75 27                 JNZ SHORT PathOfEx.0141714D
	$+23     01417126     68 D067D801           PUSH PathOfEx.01D867D0                                     ; UNICODE "Global\PoERunMutexB"
	$+28     0141712B     6A 00                 PUSH 0x0
	$+2A     0141712D     6A 00                 PUSH 0x0
	$+2C     0141712F     FF15 1431C401         CALL DWORD PTR DS:[<&KERNEL32.CreateMutexW>]               ; kernel32.CreateMutexW
	$+32     01417135     85C0                  TEST EAX,EAX
	$+34     01417137     74 5F                 JE SHORT PathOfEx.01417198
	$+36     01417139     6A 00                 PUSH 0x0
	$+38     0141713B     50                    PUSH EAX
	$+39     0141713C     FF15 A431C401         CALL DWORD PTR DS:[<&KERNEL32.WaitForSingleObject>]        ; kernel32.WaitForSingleObject
	$+3F     01417142     3D 02010000           CMP EAX,0x102
	$+44     01417147     0F84 92000000         JE PathOfEx.014171DF
	$+4A     0141714D     83C4 14               ADD ESP,0x14
	$+4D     01417150     C3                    RETN
	$+4E     01417151     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+52     01417155     C74424 08 9045CB01    MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01CB4590
	$+5A     0141715D     50                    PUSH EAX
	$+5B     0141715E     8D4424 04             LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+5F     01417162     C74424 04 B867D801    MOV DWORD PTR SS:[ESP+0x4],PathOfEx.01D867B8               ; ASCII "Failed to create mutex"
	$+67     0141716A     0F57C0                XORPS XMM0,XMM0
	$+6A     0141716D     C64424 08 01          MOV BYTE PTR SS:[ESP+0x8],0x1
	$+6F     01417172     50                    PUSH EAX
	$+70     01417173     660FD64424 14         MOVQ QWORD PTR SS:[ESP+0x14],XMM0
	$+76     01417179     E8 36117300           CALL PathOfEx.01B482B4
	$+7B     0141717E     83C4 08               ADD ESP,0x8
	$+7E     01417181     C74424 08 E845CB01    MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01CB45E8
	$+86     01417189     8D4424 08             LEA EAX,DWORD PTR SS:[ESP+0x8]
	$+8A     0141718D     68 78BDED01           PUSH PathOfEx.01EDBD78
	$+8F     01417192     50                    PUSH EAX
	$+90     01417193     E8 DB217300           CALL PathOfEx.01B49373
	$+95     01417198     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+99     0141719C     C74424 08 9045CB01    MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01CB4590
	$+A1     014171A4     50                    PUSH EAX
	$+A2     014171A5     8D4424 04             LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+A6     014171A9     C74424 04 B867D801    MOV DWORD PTR SS:[ESP+0x4],PathOfEx.01D867B8               ; ASCII "Failed to create mutex"
	$+AE     014171B1     0F57C0                XORPS XMM0,XMM0
	$+B1     014171B4     C64424 08 01          MOV BYTE PTR SS:[ESP+0x8],0x1
	$+B6     014171B9     50                    PUSH EAX
	$+B7     014171BA     660FD64424 14         MOVQ QWORD PTR SS:[ESP+0x14],XMM0
	$+BD     014171C0     E8 EF107300           CALL PathOfEx.01B482B4
	$+C2     014171C5     83C4 08               ADD ESP,0x8
	$+C5     014171C8     C74424 08 E845CB01    MOV DWORD PTR SS:[ESP+0x8],PathOfEx.01CB45E8
	$+CD     014171D0     8D4424 08             LEA EAX,DWORD PTR SS:[ESP+0x8]
	$+D1     014171D4     68 78BDED01           PUSH PathOfEx.01EDBD78
	$+D6     014171D9     50                    PUSH EAX
	$+D7     014171DA     E8 94217300           CALL PathOfEx.01B49373
	$+DC     014171DF     68 F867D801           PUSH PathOfEx.01D867F8                                     ; ASCII "You may only run up to two copies of Path of Exile at one time."
	$+E1     014171E4     8D4C24 0C             LEA ECX,DWORD PTR SS:[ESP+0xC]
	$+E5     014171E8     E8 139ED1FF           CALL PathOfEx.01131000
	$+EA     014171ED     68 A4C1ED01           PUSH PathOfEx.01EDC1A4
	$+EF     014171F2     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+F3     014171F6     50                    PUSH EAX
	$+F4     014171F7     E8 77217300           CALL PathOfEx.01B49373
	$+F9     014171FC     CC                    INT3
	$+FA     014171FD     CC                    INT3
	$+FB     014171FE     CC                    INT3
	$+FC     014171FF     CC                    INT3
	$+FD     01417200     83EC 20               SUB ESP,0x20
	$+100    01417203     53                    PUSH EBX
	$+101    01417204     56                    PUSH ESI
	$+102    01417205     57                    PUSH EDI
	$+103    01417206     6A 1A                 PUSH 0x1A
	$+105    01417208     8BF1                  MOV ESI,ECX
	$+107    0141720A     C74424 28 07000000    MOV DWORD PTR SS:[ESP+0x28],0x7
	$+10F    01417212     33C0                  XOR EAX,EAX
	$+111    01417214     C74424 24 00000000    MOV DWORD PTR SS:[ESP+0x24],0x0
	$+119    0141721C     68 7064D801           PUSH PathOfEx.01D86470                                     ; UNICODE "--disable-client-switching"
	$+11E    01417221     8D4C24 18             LEA ECX,DWORD PTR SS:[ESP+0x18]
	$+122    01417225     66:894424 18          MOV WORD PTR SS:[ESP+0x18],AX
	$+127    0141722A     E8 2120D1FF           CALL PathOfEx.01129250
	$+12C    0141722F     8B36                  MOV ESI,DWORD PTR DS:[ESI]
	$+12E    01417231     8D4424 10             LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+132    01417235     50                    PUSH EAX
	$+133    01417236     8D4424 10             LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+137    0141723A     50                    PUSH EAX
	$+138    0141723B     8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+13B    0141723E     83C1 1C               ADD ECX,0x1C
	$+13E    01417241     E8 BA3DD1FF           CALL PathOfEx.0112B000
	$+143    01417246     8B4424 0C             MOV EAX,DWORD PTR SS:[ESP+0xC]
	$+147    0141724A     8B40 20               MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+14A    0141724D     8B40 48               MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+14D    01417250     8D0C40                LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+150    01417253     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+152    01417255     8B74C8 10             MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+156    01417259     8B4424 24             MOV EAX,DWORD PTR SS:[ESP+0x24]
	$+15A    0141725D     83F8 08               CMP EAX,0x8
	$+15D    01417260     72 10                 JB SHORT PathOfEx.01417272
	$+15F    01417262     6A 02                 PUSH 0x2
	$+161    01417264     40                    INC EAX
	$+162    01417265     50                    PUSH EAX
	$+163    01417266     FF7424 18             PUSH DWORD PTR SS:[ESP+0x18]
	$+167    0141726A     E8 510AD1FF           CALL PathOfEx.01127CC0
	$+16C    0141726F     83C4 0C               ADD ESP,0xC
	$+16F    01417272     85F6                  TEST ESI,ESI
	$+171    01417274     0F85 B2000000         JNZ PathOfEx.0141732C
	$+177    0141727A     E8 61340D00           CALL PathOfEx.014EA6E0
	$+17C    0141727F     8BC8                  MOV ECX,EAX
	$+17E    01417281     E8 EA520D00           CALL PathOfEx.014EC570
	$+183    01417286     68 6871D801           PUSH PathOfEx.01D87168                                     ; ASCII "IsWow64Process"
	$+188    0141728B     68 7871D801           PUSH PathOfEx.01D87178                                     ; UNICODE "kernel32"
	$+18D    01417290     8BF0                  MOV ESI,EAX
	$+18F    01417292     C74424 14 00000000    MOV DWORD PTR SS:[ESP+0x14],0x0
	$+197    0141729A     FF15 F430C401         CALL DWORD PTR DS:[<&KERNEL32.GetModuleHandleW>]           ; kernel32.GetModuleHandleW
	$+19D    014172A0     50                    PUSH EAX
	$+19E    014172A1     FF15 F030C401         CALL DWORD PTR DS:[<&KERNEL32.GetProcAddress>]             ; kernel32.GetProcAddress
	$+1A4    014172A7     8BF8                  MOV EDI,EAX
	$+1A6    014172A9     85FF                  TEST EDI,EDI
	$+1A8    014172AB     74 0E                 JE SHORT PathOfEx.014172BB
	$+1AA    014172AD     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+1AE    014172B1     50                    PUSH EAX
	$+1AF    014172B2     FF15 0431C401         CALL DWORD PTR DS:[<&KERNEL32.GetCurrentProcess>]          ; kernel32.GetCurrentProcess
	$+1B5    014172B8     50                    PUSH EAX
	$+1B6    014172B9     FFD7                  CALL EDI
	$+1B8    014172BB     837C24 0C 00          CMP DWORD PTR SS:[ESP+0xC],0x0
	$+1BD    014172C0     74 6A                 JE SHORT PathOfEx.0141732C
	$+1BF    014172C2     83FE 03               CMP ESI,0x3
	$+1C2    014172C5     74 04                 JE SHORT PathOfEx.014172CB
	$+1C4    014172C7     85F6                  TEST ESI,ESI
	$+1C6    014172C9     75 61                 JNZ SHORT PathOfEx.0141732C
	$+1C8    014172CB     E8 B0560000           CALL PathOfEx.0141C980
	$+1CD    014172D0     84C0                  TEST AL,AL
	$+1CF    014172D2     74 09                 JE SHORT PathOfEx.014172DD
	$+1D1    014172D4     32C0                  XOR AL,AL
	$+1D3    014172D6     5F                    POP EDI
	$+1D4    014172D7     5E                    POP ESI
	$+1D5    014172D8     5B                    POP EBX
	$+1D6    014172D9     83C4 20               ADD ESP,0x20
	$+1D9    014172DC     C3                    RETN
	$+1DA    014172DD     33DB                  XOR EBX,EBX
	$+1DC    014172DF     381D B285F301         CMP BYTE PTR DS:[0x1F385B2],BL
	$+1E2    014172E5     0F95C3                SETNE BL
	$+1E5    014172E8     E8 F3330D00           CALL PathOfEx.014EA6E0
	$+1EA    014172ED     FF349D C0E2CC01       PUSH DWORD PTR DS:[EBX*4+0x1CCE2C0]
	$+1F1    014172F4     68 78E5D601           PUSH PathOfEx.01D6E578                                     ; UNICODE "directx_version"
	$+1F6    014172F9     8B88 80030000         MOV ECX,DWORD PTR DS:[EAX+0x380]
	$+1FC    014172FF     68 B4E5D601           PUSH PathOfEx.01D6E5B4                                     ; UNICODE "DISPLAY"
	$+201    01417304     E8 D7D20D00           CALL PathOfEx.014F45E0
	$+206    01417309     E8 D2330D00           CALL PathOfEx.014EA6E0
	$+20B    0141730E     8BF0                  MOV ESI,EAX
	$+20D    01417310     8B8E 80030000         MOV ECX,DWORD PTR DS:[ESI+0x380]
	$+213    01417316     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+215    01417318     8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+218    0141731B     FFD2                  CALL EDX
	$+21A    0141731D     84C0                  TEST AL,AL
	$+21C    0141731F     74 0B                 JE SHORT PathOfEx.0141732C
	$+21E    01417321     8B8E 80030000         MOV ECX,DWORD PTR DS:[ESI+0x380]
	$+224    01417327     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+226    01417329     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+229    0141732C     5F                    POP EDI
	$+22A    0141732D     5E                    POP ESI
	$+22B    0141732E     B0 01                 MOV AL,0x1
	$+22D    01417330     5B                    POP EBX
	$+22E    01417331     83C4 20               ADD ESP,0x20
	$+231    01417334     C3                    RETN
	$+232    01417335     CC                    INT3
	$+233    01417336     CC                    INT3
	$+234    01417337     CC                    INT3
	$+235    01417338     CC                    INT3
	$+236    01417339     CC                    INT3
	$+237    0141733A     CC                    INT3
	$+238    0141733B     CC                    INT3
	$+239    0141733C     CC                    INT3
	$+23A    0141733D     CC                    INT3
	$+23B    0141733E     CC                    INT3
	$+23C    0141733F     CC                    INT3
	$+23D    01417340     55                    PUSH EBP
	$+23E    01417341     8BEC                  MOV EBP,ESP
	$+240    01417343     6A FF                 PUSH -0x1
	$+242    01417345     68 D307BF01           PUSH PathOfEx.01BF07D3
	$+247    0141734A     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+24D    01417350     50                    PUSH EAX
	$+24E    01417351     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$+255    01417358     81EC F8010000         SUB ESP,0x1F8
	$+25B    0141735E     53                    PUSH EBX
	$+25C    0141735F     56                    PUSH ESI
	$+25D    01417360     57                    PUSH EDI
	$+25E    01417361     8BF9                  MOV EDI,ECX
	$+260    01417363     51                    PUSH ECX
	$+261    01417364     8D8D 00FEFFFF         LEA ECX,DWORD PTR SS:[EBP-0x200]
	$+267    0141736A     E8 911B3400           CALL PathOfEx.01758F00
	$+26C    0141736F     83EC 14               SUB ESP,0x14
	$+26F    01417372     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+276    01417379     8B0D 9C160902         MOV ECX,DWORD PTR DS:[0x209169C]
	$+27C    0141737F     8D85 00FEFFFF         LEA EAX,DWORD PTR SS:[EBP-0x200]
	$+282    01417385     50                    PUSH EAX
	$+283    01417386     E8 15CCD1FF           CALL PathOfEx.01133FA0
	$+288    0141738B     84C0                  TEST AL,AL
	$+28A    0141738D     75 38                 JNZ SHORT PathOfEx.014173C7
	$+28C    0141738F     8D45 E4               LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+28F    01417392     C745 E0 9045CB01      MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01CB4590
	$+296    01417399     50                    PUSH EAX
	$+297    0141739A     8D45 EC               LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+29A    0141739D     C745 EC E868D801      MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01D868E8              ; ASCII "Failed to initialise"
	$+2A1    014173A4     0F57C0                XORPS XMM0,XMM0
	$+2A4    014173A7     C645 F0 01            MOV BYTE PTR SS:[EBP-0x10],0x1
	$+2A8    014173AB     50                    PUSH EAX
	$+2A9    014173AC     660FD645 E4           MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$+2AE    014173B1     E8 FE0E7300           CALL PathOfEx.01B482B4
	$+2B3    014173B6     83C4 08               ADD ESP,0x8
	$+2B6    014173B9     8D45 E0               LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+2B9    014173BC     68 A4C1ED01           PUSH PathOfEx.01EDC1A4
	$+2BE    014173C1     50                    PUSH EAX
	$+2BF    014173C2     E8 AC1F7300           CALL PathOfEx.01B49373
	$+2C4    014173C7     6A 0C                 PUSH 0xC
	$+2C6    014173C9     33C0                  XOR EAX,EAX
	$+2C8    014173CB     C745 E8 07000000      MOV DWORD PTR SS:[EBP-0x18],0x7
	$+2CF    014173D2     68 CC59D801           PUSH PathOfEx.01D859CC                                     ; UNICODE "--login-port"
	$+2D4    014173D7     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+2D7    014173DA     C745 E4 00000000      MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+2DE    014173E1     66:8945 D4            MOV WORD PTR SS:[EBP-0x2C],AX
	$+2E2    014173E5     E8 661ED1FF           CALL PathOfEx.01129250
	$+2E7    014173EA     8B37                  MOV ESI,DWORD PTR DS:[EDI]
	$+2E9    014173EC     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+2EC    014173EF     50                    PUSH EAX
	$+2ED    014173F0     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+2F0    014173F3     50                    PUSH EAX
	$+2F1    014173F4     8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+2F4    014173F7     83C1 1C               ADD ECX,0x1C
	$+2F7    014173FA     E8 013CD1FF           CALL PathOfEx.0112B000
	$+2FC    014173FF     8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+2FF    01417402     8B40 20               MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+302    01417405     8B40 48               MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+305    01417408     8D0C40                LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+308    0141740B     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+30A    0141740D     8B74C8 10             MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+30E    01417411     8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+311    01417414     83F8 08               CMP EAX,0x8
	$+314    01417417     72 0F                 JB SHORT PathOfEx.01417428
	$+316    01417419     40                    INC EAX
	$+317    0141741A     6A 02                 PUSH 0x2
	$+319    0141741C     50                    PUSH EAX
	$+31A    0141741D     FF75 D4               PUSH DWORD PTR SS:[EBP-0x2C]
	$+31D    01417420     E8 9B08D1FF           CALL PathOfEx.01127CC0
	$+322    01417425     83C4 0C               ADD ESP,0xC
	$+325    01417428     85F6                  TEST ESI,ESI
	$+327    0141742A     74 57                 JE SHORT PathOfEx.01417483
	$+329    0141742C     68 CC59D801           PUSH PathOfEx.01D859CC                                     ; UNICODE "--login-port"
	$+32E    01417431     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+331    01417434     E8 171FD1FF           CALL PathOfEx.01129350
	$+336    01417439     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+339    0141743C     C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+33D    01417440     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
	$+33F    01417442     50                    PUSH EAX
	$+340    01417443     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+343    01417446     50                    PUSH EAX
	$+344    01417447     E8 64E80100           CALL PathOfEx.01435CB0
	$+349    0141744C     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+34C    0141744F     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$+350    01417453     E8 8818D1FF           CALL PathOfEx.01128CE0
	$+355    01417458     A1 9C160902           MOV EAX,DWORD PTR DS:[0x209169C]
	$+35A    0141745D     8B48 30               MOV ECX,DWORD PTR DS:[EAX+0x30]
	$+35D    01417460     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+360    01417463     81C1 CC000000         ADD ECX,0xCC
	$+366    01417469     3BC8                  CMP ECX,EAX
	$+368    0141746B     74 0A                 JE SHORT PathOfEx.01417477
	$+36A    0141746D     6A FF                 PUSH -0x1
	$+36C    0141746F     6A 00                 PUSH 0x0
	$+36E    01417471     50                    PUSH EAX
	$+36F    01417472     E8 A919D1FF           CALL PathOfEx.01128E20
	$+374    01417477     8D4D BC               LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+377    0141747A     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+37B    0141747E     E8 5D18D1FF           CALL PathOfEx.01128CE0
	$+380    01417483     6A 15                 PUSH 0x15
	$+382    01417485     33C0                  XOR EAX,EAX
	$+384    01417487     C745 E8 07000000      MOV DWORD PTR SS:[EBP-0x18],0x7
	$+38B    0141748E     68 FC5CD801           PUSH PathOfEx.01D85CFC                                     ; UNICODE "--garbage-generations"
	$+390    01417493     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+393    01417496     C745 E4 00000000      MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+39A    0141749D     66:8945 D4            MOV WORD PTR SS:[EBP-0x2C],AX
	$+39E    014174A1     E8 AA1DD1FF           CALL PathOfEx.01129250
	$+3A3    014174A6     8B37                  MOV ESI,DWORD PTR DS:[EDI]
	$+3A5    014174A8     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+3A8    014174AB     50                    PUSH EAX
	$+3A9    014174AC     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+3AC    014174AF     50                    PUSH EAX
	$+3AD    014174B0     8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+3B0    014174B3     83C1 1C               ADD ECX,0x1C
	$+3B3    014174B6     E8 453BD1FF           CALL PathOfEx.0112B000
	$+3B8    014174BB     8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+3BB    014174BE     8B40 20               MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+3BE    014174C1     8B40 48               MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+3C1    014174C4     8D0C40                LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+3C4    014174C7     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+3C6    014174C9     8B74C8 10             MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+3CA    014174CD     8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+3CD    014174D0     83F8 08               CMP EAX,0x8
	$+3D0    014174D3     72 0F                 JB SHORT PathOfEx.014174E4
	$+3D2    014174D5     40                    INC EAX
	$+3D3    014174D6     6A 02                 PUSH 0x2
	$+3D5    014174D8     50                    PUSH EAX
	$+3D6    014174D9     FF75 D4               PUSH DWORD PTR SS:[EBP-0x2C]
	$+3D9    014174DC     E8 DF07D1FF           CALL PathOfEx.01127CC0
	$+3DE    014174E1     83C4 0C               ADD ESP,0xC
	$+3E1    014174E4     85F6                  TEST ESI,ESI
	$+3E3    014174E6     74 7C                 JE SHORT PathOfEx.01417564
	$+3E5    014174E8     68 FC5CD801           PUSH PathOfEx.01D85CFC                                     ; UNICODE "--garbage-generations"
	$+3EA    014174ED     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+3ED    014174F0     E8 5B1ED1FF           CALL PathOfEx.01129350
	$+3F2    014174F5     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+3F5    014174F8     C645 FC 04            MOV BYTE PTR SS:[EBP-0x4],0x4
	$+3F9    014174FC     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
	$+3FB    014174FE     50                    PUSH EAX
	$+3FC    014174FF     E8 0CE80100           CALL PathOfEx.01435D10
	$+401    01417504     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+404    01417507     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+408    0141750B     8BF0                  MOV ESI,EAX
	$+40A    0141750D     E8 CE17D1FF           CALL PathOfEx.01128CE0
	$+40F    01417512     83FE 01               CMP ESI,0x1
	$+412    01417515     7D 3F                 JGE SHORT PathOfEx.01417556
	$+414    01417517     8D45 E4               LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+417    0141751A     C745 E0 9045CB01      MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01CB4590
	$+41E    01417521     50                    PUSH EAX
	$+41F    01417522     8D45 EC               LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+422    01417525     C745 EC 0069D801      MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01D86900              ; ASCII "Must set a garbage generations to 1 or higher"
	$+429    0141752C     0F57C0                XORPS XMM0,XMM0
	$+42C    0141752F     C645 F0 01            MOV BYTE PTR SS:[EBP-0x10],0x1
	$+430    01417533     50                    PUSH EAX
	$+431    01417534     660FD645 E4           MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$+436    01417539     E8 760D7300           CALL PathOfEx.01B482B4
	$+43B    0141753E     83C4 08               ADD ESP,0x8
	$+43E    01417541     C745 E0 E845CB01      MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01CB45E8
	$+445    01417548     8D45 E0               LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+448    0141754B     68 78BDED01           PUSH PathOfEx.01EDBD78
	$+44D    01417550     50                    PUSH EAX
	$+44E    01417551     E8 1D1E7300           CALL PathOfEx.01B49373
	$+453    01417556     A1 9C160902           MOV EAX,DWORD PTR DS:[0x209169C]
	$+458    0141755B     8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+45B    0141755E     89B0 10190000         MOV DWORD PTR DS:[EAX+0x1910],ESI
	$+461    01417564     E8 77310D00           CALL PathOfEx.014EA6E0
	$+466    01417569     68 6C62D801           PUSH PathOfEx.01D8626C                                     ; UNICODE "--use-defaults"
	$+46B    0141756E     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+46E    01417571     8BD8                  MOV EBX,EAX
	$+470    01417573     E8 D81DD1FF           CALL PathOfEx.01129350
	$+475    01417578     8B37                  MOV ESI,DWORD PTR DS:[EDI]
	$+477    0141757A     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+47A    0141757D     50                    PUSH EAX
	$+47B    0141757E     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+47E    01417581     50                    PUSH EAX
	$+47F    01417582     8B4E 18               MOV ECX,DWORD PTR DS:[ESI+0x18]
	$+482    01417585     83C1 1C               ADD ECX,0x1C
	$+485    01417588     E8 733AD1FF           CALL PathOfEx.0112B000
	$+48A    0141758D     8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+48D    01417590     8B40 20               MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+490    01417593     8B40 48               MOV EAX,DWORD PTR DS:[EAX+0x48]
	$+493    01417596     8D0C40                LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+496    01417599     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+498    0141759B     8B74C8 10             MOV ESI,DWORD PTR DS:[EAX+ECX*8+0x10]
	$+49C    0141759F     8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+49F    014175A2     83F8 08               CMP EAX,0x8
	$+4A2    014175A5     72 0F                 JB SHORT PathOfEx.014175B6
	$+4A4    014175A7     6A 02                 PUSH 0x2
	$+4A6    014175A9     40                    INC EAX
	$+4A7    014175AA     50                    PUSH EAX
	$+4A8    014175AB     FF75 D4               PUSH DWORD PTR SS:[EBP-0x2C]
	$+4AB    014175AE     E8 0D07D1FF           CALL PathOfEx.01127CC0
	$+4B0    014175B3     83C4 0C               ADD ESP,0xC
	$+4B3    014175B6     85F6                  TEST ESI,ESI
	$+4B5    014175B8     74 07                 JE SHORT PathOfEx.014175C1
	$+4B7    014175BA     8BCB                  MOV ECX,EBX
	$+4B9    014175BC     E8 2F6F0D00           CALL PathOfEx.014EE4F0
	$+4BE    014175C1     8D8D 00FEFFFF         LEA ECX,DWORD PTR SS:[EBP-0x200]
	$+4C4    014175C7     E8 C4D9D1FF           CALL PathOfEx.01134F90
	$+4C9    014175CC     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+4CC    014175CF     5F                    POP EDI
	$+4CD    014175D0     5E                    POP ESI
	$+4CE    014175D1     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+4D5    014175D8     5B                    POP EBX
	$+4D6    014175D9     8BE5                  MOV ESP,EBP
	$+4D8    014175DB     5D                    POP EBP
	$+4D9    014175DC     C3                    RETN
	$+4DA    014175DD     CC                    INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("68????????6a??6a??ff??????????85??74??6a??50ff??????????3d", -3);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x83)
	{
		assert(false);
		return false;
	}
	CheckExeAddr(addr_tmp);
	ofs << "pt_cdword	g_h_duo_kai_two_copies_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
