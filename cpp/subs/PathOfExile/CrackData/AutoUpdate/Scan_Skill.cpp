#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Skill(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	pt_dword dw_tmp;
	pt_int int_tmp;
	boost::optional<pt_dword> op_dw_tmp;

	/*
	$-61     007D2B30     56                    PUSH ESI
	$-60     007D2B31     57                    PUSH EDI
	$-5F     007D2B32     8BF9                  MOV EDI, ECX
	$-5D     007D2B34     33F6                  XOR ESI, ESI
	$-5B     007D2B36     8B87 98000000         MOV EAX, DWORD PTR DS:[EDI+0x98]
	$-55     007D2B3C     2B87 94000000         SUB EAX, DWORD PTR DS:[EDI+0x94]         ; 技能偏移首地址
	$-4F     007D2B42     C1F8 02               SAR EAX, 0x2
	$-4C     007D2B45     85C0                  TEST EAX, EAX
	$-4A     007D2B47     0F84 C6000000         JE PathOfEx.007D2C13
	$-44     007D2B4D     F3:0F105424 0C        MOVSS XMM2, DWORD PTR SS:[ESP+0xC]
	$-3E     007D2B53     53                    PUSH EBX
	$-3D     007D2B54     8B87 94000000         MOV EAX, DWORD PTR DS:[EDI+0x94]
	$-37     007D2B5A     8B1CB0                MOV EBX, DWORD PTR DS:[EAX+ESI*4]
	$-34     007D2B5D     8B43 04               MOV EAX, DWORD PTR DS:[EBX+0x4]
	$-31     007D2B60     F3:0F1040 0C          MOVSS XMM0, DWORD PTR DS:[EAX+0xC]
	$-2C     007D2B65     0F2F05 40C9F300       COMISS XMM0, DWORD PTR DS:[0xF3C940]
	$-25     007D2B6C     0F86 88000000         JBE PathOfEx.007D2BFA
	$-1F     007D2B72     837B 20 02            CMP DWORD PTR DS:[EBX+0x20], 0x2
	$-1B     007D2B76     74 61                 JE SHORT PathOfEx.007D2BD9
	$-19     007D2B78     8B40 04               MOV EAX, DWORD PTR DS:[EAX+0x4]
	$-16     007D2B7B     8078 48 00            CMP BYTE PTR DS:[EAX+0x48], 0x0
	$-12     007D2B7F     74 05                 JE SHORT PathOfEx.007D2B86
	$-10     007D2B81     0F28CA                MOVAPS XMM1, XMM2
	$-D      007D2B84     EB 42                 JMP SHORT PathOfEx.007D2BC8
	$-B      007D2B86     8B47 10               MOV EAX, DWORD PTR DS:[EDI+0x10]
	$-8      007D2B89     68 19020000           PUSH 0x219
	$-3      007D2B8E     8B48 54               MOV ECX, DWORD PTR DS:[EAX+0x54]
	$ ==>    007D2B91     8B47 04               MOV EAX, DWORD PTR DS:[EDI+0x4]
	$+3      007D2B94     8B40 04               MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+6      007D2B97     8B0488                MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+9      007D2B9A     8B48 10               MOV ECX, DWORD PTR DS:[EAX+0x10]
	$+C      007D2B9D     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$+E      007D2B9F     FF10                  CALL NEAR DWORD PTR DS:[EAX]
	$+10     007D2BA1     F3:0F100D 74CBF300    MOVSS XMM1, DWORD PTR DS:[0xF3CB74]
	$+18     007D2BA9     F3:0F105424 10        MOVSS XMM2, DWORD PTR SS:[ESP+0x10]
	$+1E     007D2BAF     66:0F6EC0             MOVD MM0, EAX
	$+22     007D2BB3     0F5BC0                CVTDQ2PS XMM0, XMM0
	$+25     007D2BB6     F3:0F5905 F0C9F300    MULSS XMM0, DWORD PTR DS:[0xF3C9F0]
	$+2D     007D2BBE     F3:0F5CC8             SUBSS XMM1, XMM0
	$+31     007D2BC2     F3:0F594C24 10        MULSS XMM1, DWORD PTR SS:[ESP+0x10]
	$+37     007D2BC8     8B43 04               MOV EAX, DWORD PTR DS:[EBX+0x4]
	$+3A     007D2BCB     F3:0F1040 0C          MOVSS XMM0, DWORD PTR DS:[EAX+0xC]
	$+3F     007D2BD0     F3:0F5CC1             SUBSS XMM0, XMM1
	$+43     007D2BD4     F3:0F1140 0C          MOVSS DWORD PTR DS:[EAX+0xC], XMM0
	$+48     007D2BD9     8B88 E4000000         MOV ECX, DWORD PTR DS:[EAX+0xE4]
	$+4E     007D2BDF     85C9                  TEST ECX, ECX
	$+50     007D2BE1     74 17                 JE SHORT PathOfEx.007D2BFA
	$+52     007D2BE3     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$+54     007D2BE5     F3:0F104424 10        MOVSS XMM0, DWORD PTR SS:[ESP+0x10]
	$+5A     007D2BEB     51                    PUSH ECX
	$+5B     007D2BEC     F3:0F110424           MOVSS DWORD PTR SS:[ESP], XMM0
	$+60     007D2BF1     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]
	$+63     007D2BF4     F3:0F105424 10        MOVSS XMM2, DWORD PTR SS:[ESP+0x10]
	$+69     007D2BFA     8B87 98000000         MOV EAX, DWORD PTR DS:[EDI+0x98]
	$+6F     007D2C00     46                    INC ESI
	$+70     007D2C01     2B87 94000000         SUB EAX, DWORD PTR DS:[EDI+0x94]
	$+76     007D2C07     C1F8 02               SAR EAX, 0x2
	$+79     007D2C0A     3BF0                  CMP ESI, EAX
	$+7B     007D2C0C   ^ 0F82 42FFFFFF         JB PathOfEx.007D2B54
	$+81     007D2C12     5B                    POP EBX                                  ; PathOf_1.517C0014
	$+82     007D2C13     837F 30 00            CMP DWORD PTR DS:[EDI+0x30], 0x0
	$+86     007D2C17     0F8E 8B000000         JLE PathOfEx.007D2CA8
	$+8C     007D2C1D     8B47 1C               MOV EAX, DWORD PTR DS:[EDI+0x1C]
	$+8F     007D2C20     F3:0F104424 0C        MOVSS XMM0, DWORD PTR SS:[ESP+0xC]
	$+95     007D2C26     51                    PUSH ECX
	$+96     007D2C27     8D4F 1C               LEA ECX, DWORD PTR DS:[EDI+0x1C]
	$+99     007D2C2A     F3:0F110424           MOVSS DWORD PTR SS:[ESP], XMM0
	$+9E     007D2C2F     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]
	$+A1     007D2C32     8B47 64               MOV EAX, DWORD PTR DS:[EDI+0x64]
	$+A4     007D2C35     8D4F 64               LEA ECX, DWORD PTR DS:[EDI+0x64]
	$+A7     007D2C38     F3:0F104424 0C        MOVSS XMM0, DWORD PTR SS:[ESP+0xC]
	$+AD     007D2C3E     51                    PUSH ECX
	$+AE     007D2C3F     F3:0F110424           MOVSS DWORD PTR SS:[ESP], XMM0
	$+B3     007D2C44     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]
	$+B6     007D2C47     8B47 40               MOV EAX, DWORD PTR DS:[EDI+0x40]
	$+B9     007D2C4A     8D4F 40               LEA ECX, DWORD PTR DS:[EDI+0x40]
	$+BC     007D2C4D     F3:0F104424 0C        MOVSS XMM0, DWORD PTR SS:[ESP+0xC]
	$+C2     007D2C53     51                    PUSH ECX
	$+C3     007D2C54     F3:0F110424           MOVSS DWORD PTR SS:[ESP], XMM0
	$+C8     007D2C59     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]
	$+CB     007D2C5C     837F 2C 01            CMP DWORD PTR DS:[EDI+0x2C], 0x1
	$+CF     007D2C60     75 1D                 JNZ SHORT PathOfEx.007D2C7F
	$+D1     007D2C62     8D4F 1C               LEA ECX, DWORD PTR DS:[EDI+0x1C]
	$+D4     007D2C65     E8 767EDFFF           CALL PathOfEx.005CAAE0
	$+D9     007D2C6A     0F57C9                XORPS XMM1, XMM1
	$+DC     007D2C6D     0F2FC8                COMISS XMM1, XMM0
	$+DF     007D2C70     76 0D                 JBE SHORT PathOfEx.007D2C7F
	$+E1     007D2C72     8B47 1C               MOV EAX, DWORD PTR DS:[EDI+0x1C]
	$+E4     007D2C75     8D4F 1C               LEA ECX, DWORD PTR DS:[EDI+0x1C]
	$+E7     007D2C78     6A 00                 PUSH 0x0
	$+E9     007D2C7A     6A FF                 PUSH -0x1
	$+EB     007D2C7C     FF50 04               CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+EE     007D2C7F     80BF A1000000 00      CMP BYTE PTR DS:[EDI+0xA1], 0x0
	$+F5     007D2C86     74 20                 JE SHORT PathOfEx.007D2CA8
	$+F7     007D2C88     C687 A1000000 00      MOV BYTE PTR DS:[EDI+0xA1], 0x0
	$+FE     007D2C8F     8B47 14               MOV EAX, DWORD PTR DS:[EDI+0x14]
	$+101    007D2C92     8B30                  MOV ESI, DWORD PTR DS:[EAX]
	$+103    007D2C94     3BF0                  CMP ESI, EAX
	$+105    007D2C96     74 10                 JE SHORT PathOfEx.007D2CA8
	$+107    007D2C98     8B4E 08               MOV ECX, DWORD PTR DS:[ESI+0x8]
	$+10A    007D2C9B     8B36                  MOV ESI, DWORD PTR DS:[ESI]              ; kernel32.74FC1194
	$+10C    007D2C9D     57                    PUSH EDI
	$+10D    007D2C9E     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$+10F    007D2CA0     FF50 08               CALL NEAR DWORD PTR DS:[EAX+0x8]
	$+112    007D2CA3     3B77 14               CMP ESI, DWORD PTR DS:[EDI+0x14]
	$+115    007D2CA6   ^ 75 F0                 JNZ SHORT PathOfEx.007D2C98
	$+117    007D2CA8     5F                    POP EDI                                  ; PathOfEx.007C76EE
	$+118    007D2CA9     5E                    POP ESI                                  ; PathOfEx.007C76EE
	$+119    007D2CAA     C2 0400               RETN 0x4
	$+11C    007D2CAD     CC                    INT3
	$+11D    007D2CAE     CC                    INT3
	$+11E    007D2CAF     CC                    INT3
	$+11F    007D2CB0     83EC 08               SUB ESP, 0x8
	$+122    007D2CB3     56                    PUSH ESI
	$+123    007D2CB4     57                    PUSH EDI
	$+124    007D2CB5     8BF1                  MOV ESI, ECX
	$+126    007D2CB7     F3:0F114424 08        MOVSS DWORD PTR SS:[ESP+0x8], XMM0
	$+12C    007D2CBD     8BFA                  MOV EDI, EDX
	$+12E    007D2CBF     B8 01000000           MOV EAX, 0x1
	*/
	//gj3 2017/7/28
	/*
	$-FE     006625CF     CC                       INT3
	$-FD     006625D0     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-F9     006625D4     57                       PUSH EDI
	$-F8     006625D5     8BF9                     MOV EDI,ECX
	$-F6     006625D7     3D 9B050000              CMP EAX,0x59B
	$-F1     006625DC     74 58                    JE SHORT PathOfEx.00662636
	$-EF     006625DE     3D 9C050000              CMP EAX,0x59C
	$-EA     006625E3     74 51                    JE SHORT PathOfEx.00662636
	$-E8     006625E5     80BF 8C000000 00         CMP BYTE PTR DS:[EDI+0x8C],0x0
	$-E1     006625EC     75 48                    JNZ SHORT PathOfEx.00662636
	$-DF     006625EE     8B87 88000000            MOV EAX,DWORD PTR DS:[EDI+0x88]
	$-D9     006625F4     56                       PUSH ESI
	$-D8     006625F5     8B77 78                  MOV ESI,DWORD PTR DS:[EDI+0x78]
	$-D5     006625F8     0FAFC6                   IMUL EAX,ESI
	$-D2     006625FB     66:0F6EC0                MOVD MM0,EAX
	$-CE     006625FF     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$-CB     00662602     F3:0F5E05 00B4F700       DIVSS XMM0,DWORD PTR DS:[0xF7B400]
	$-C3     0066260A     E8 3129CBFF              CALL PathOfEx.00314F40
	$-BE     0066260F     2BB7 84000000            SUB ESI,DWORD PTR DS:[EDI+0x84]
	$-B8     00662615     F3:0F2CC0                CVTTSS2SI EAX,XMM0
	$-B4     00662619     2BF0                     SUB ESI,EAX
	$-B2     0066261B     B8 00000000              MOV EAX,0x0
	$-AD     00662620     0F48F0                   CMOVS ESI,EAX
	$-AA     00662623     85F6                     TEST ESI,ESI
	$-A8     00662625     5E                       POP ESI
	$-A7     00662626     74 0E                    JE SHORT PathOfEx.00662636
	$-A5     00662628     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-A3     0066262A     8BCF                     MOV ECX,EDI
	$-A1     0066262C     FF50 58                  CALL DWORD PTR DS:[EAX+0x58]
	$-9E     0066262F     C687 8C000000 01         MOV BYTE PTR DS:[EDI+0x8C],0x1
	$-97     00662636     5F                       POP EDI
	$-96     00662637     C2 0400                  RETN 0x4
	$-93     0066263A     CC                       INT3
	$-92     0066263B     CC                       INT3
	$-91     0066263C     CC                       INT3
	$-90     0066263D     CC                       INT3
	$-8F     0066263E     CC                       INT3
	$-8E     0066263F     CC                       INT3
	$-8D     00662640     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-89     00662644     57                       PUSH EDI
	$-88     00662645     8BF9                     MOV EDI,ECX
	$-86     00662647     3D 9B050000              CMP EAX,0x59B
	$-81     0066264C     74 6A                    JE SHORT PathOfEx.006626B8
	$-7F     0066264E     3D 9C050000              CMP EAX,0x59C
	$-7A     00662653     74 63                    JE SHORT PathOfEx.006626B8
	$-78     00662655     80BF 8D000000 00         CMP BYTE PTR DS:[EDI+0x8D],0x0
	$-71     0066265C     75 5A                    JNZ SHORT PathOfEx.006626B8
	$-6F     0066265E     8B87 88000000            MOV EAX,DWORD PTR DS:[EDI+0x88]
	$-69     00662664     56                       PUSH ESI
	$-68     00662665     8B77 78                  MOV ESI,DWORD PTR DS:[EDI+0x78]
	$-65     00662668     0FAFC6                   IMUL EAX,ESI
	$-62     0066266B     66:0F6EC0                MOVD MM0,EAX
	$-5E     0066266F     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$-5B     00662672     F3:0F5E05 00B4F700       DIVSS XMM0,DWORD PTR DS:[0xF7B400]
	$-53     0066267A     E8 C128CBFF              CALL PathOfEx.00314F40
	$-4E     0066267F     2BB7 84000000            SUB ESI,DWORD PTR DS:[EDI+0x84]
	$-48     00662685     F3:0F2CC0                CVTTSS2SI EAX,XMM0
	$-44     00662689     2BF0                     SUB ESI,EAX
	$-42     0066268B     B8 00000000              MOV EAX,0x0
	$-3D     00662690     0F48F0                   CMOVS ESI,EAX
	$-3A     00662693     85F6                     TEST ESI,ESI
	$-38     00662695     74 20                    JE SHORT PathOfEx.006626B7
	$-36     00662697     8B47 14                  MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-33     0066269A     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$-31     0066269C     3BF0                     CMP ESI,EAX
	$-2F     0066269E     74 10                    JE SHORT PathOfEx.006626B0
	$-2D     006626A0     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-2A     006626A3     8B36                     MOV ESI,DWORD PTR DS:[ESI]
	$-28     006626A5     57                       PUSH EDI
	$-27     006626A6     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-25     006626A8     FF50 30                  CALL DWORD PTR DS:[EAX+0x30]
	$-22     006626AB     3B77 14                  CMP ESI,DWORD PTR DS:[EDI+0x14]
	$-1F     006626AE   ^ 75 F0                    JNZ SHORT PathOfEx.006626A0
	$-1D     006626B0     C687 8D000000 01         MOV BYTE PTR DS:[EDI+0x8D],0x1
	$-16     006626B7     5E                       POP ESI
	$-15     006626B8     5F                       POP EDI
	$-14     006626B9     C2 0400                  RETN 0x4
	$-11     006626BC     CC                       INT3
	$-10     006626BD     CC                       INT3
	$-F      006626BE     CC                       INT3
	$-E      006626BF     CC                       INT3
	$-D      006626C0     51                       PUSH ECX
	$-C      006626C1     56                       PUSH ESI
	$-B      006626C2     57                       PUSH EDI
	$-A      006626C3     8BF9                     MOV EDI,ECX
	$-8      006626C5     33F6                     XOR ESI,ESI
	$-6      006626C7     8B87 A0000000            MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$ ==>    006626CD     2B87 9C000000            SUB EAX,DWORD PTR DS:[EDI+0x9C]              ; 技能偏移首地址
	$+6      006626D3     C1F8 02                  SAR EAX,0x2
	$+9      006626D6     85C0                     TEST EAX,EAX
	$+B      006626D8     0F84 0C010000            JE PathOfEx.006627EA
	$+11     006626DE     53                       PUSH EBX
	$+12     006626DF     0F57C9                   XORPS XMM1,XMM1
	$+15     006626E2     55                       PUSH EBP
	$+16     006626E3     8B87 9C000000            MOV EAX,DWORD PTR DS:[EDI+0x9C]
	$+1C     006626E9     8B1CB0                   MOV EBX,DWORD PTR DS:[EAX+ESI*4]
	$+1F     006626EC     8B53 04                  MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+22     006626EF     F3:0F1042 0C             MOVSS XMM0,DWORD PTR DS:[EDX+0xC]
	$+27     006626F4     0F2FC1                   COMISS XMM0,XMM1
	$+2A     006626F7     0F86 D3000000            JBE PathOfEx.006627D0
	$+30     006626FD     837B 20 02               CMP DWORD PTR DS:[EBX+0x20],0x2
	$+34     00662701     0F84 AB000000            JE PathOfEx.006627B2
	$+3A     00662707     8B47 10                  MOV EAX,DWORD PTR DS:[EDI+0x10]
	$+3D     0066270A     8B88 88000000            MOV ECX,DWORD PTR DS:[EAX+0x88]
	$+43     00662710     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+46     00662713     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+49     00662716     8B2C88                   MOV EBP,DWORD PTR DS:[EAX+ECX*4]
	$+4C     00662719     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+4F     0066271C     8078 48 00               CMP BYTE PTR DS:[EAX+0x48],0x0
	$+53     00662720     74 14                    JE SHORT PathOfEx.00662736
	$+55     00662722     F3:0F104C24 18           MOVSS XMM1,DWORD PTR SS:[ESP+0x18]
	$+5B     00662728     F3:0F5CC1                SUBSS XMM0,XMM1
	$+5F     0066272C     F3:0F1142 0C             MOVSS DWORD PTR DS:[EDX+0xC],XMM0
	$+64     00662731     E9 82000000              JMP PathOfEx.006627B8
	$+69     00662736     8B40 3B                  MOV EAX,DWORD PTR DS:[EAX+0x3B]
	$+6C     00662739     83F8 02                  CMP EAX,0x2
	$+6F     0066273C     74 17                    JE SHORT PathOfEx.00662755
	$+71     0066273E     83F8 05                  CMP EAX,0x5
	$+74     00662741     74 12                    JE SHORT PathOfEx.00662755
	$+76     00662743     83F8 0B                  CMP EAX,0xB
	$+79     00662746     74 0D                    JE SHORT PathOfEx.00662755
	$+7B     00662748     83F8 0C                  CMP EAX,0xC
	$+7E     0066274B     74 08                    JE SHORT PathOfEx.00662755
	$+80     0066274D     F3:0F114C24 10           MOVSS DWORD PTR SS:[ESP+0x10],XMM1
	$+86     00662753     EB 19                    JMP SHORT PathOfEx.0066276E
	$+88     00662755     8B4D 10                  MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+8B     00662758     68 06190000              PUSH 0x1906
	$+90     0066275D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+92     0066275F     FF10                     CALL DWORD PTR DS:[EAX]
	$+94     00662761     66:0F6EC0                MOVD MM0,EAX
	$+98     00662765     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+9B     00662768     F3:0F114424 10           MOVSS DWORD PTR SS:[ESP+0x10],XMM0
	$+A1     0066276E     8B4D 10                  MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+A4     00662771     68 19020000              PUSH 0x219
	$+A9     00662776     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+AB     00662778     FF10                     CALL DWORD PTR DS:[EAX]
	$+AD     0066277A     F3:0F104C24 10           MOVSS XMM1,DWORD PTR SS:[ESP+0x10]
	$+B3     00662780     8B53 04                  MOV EDX,DWORD PTR DS:[EBX+0x4]
	$+B6     00662783     66:0F6EC0                MOVD MM0,EAX
	$+BA     00662787     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+BD     0066278A     F3:0F5CC8                SUBSS XMM1,XMM0
	$+C1     0066278E     F3:0F1042 0C             MOVSS XMM0,DWORD PTR DS:[EDX+0xC]
	$+C6     00662793     F3:0F5E0D 00B4F700       DIVSS XMM1,DWORD PTR DS:[0xF7B400]
	$+CE     0066279B     F3:0F580D 0CB1F700       ADDSS XMM1,DWORD PTR DS:[0xF7B10C]
	$+D6     006627A3     F3:0F594C24 18           MULSS XMM1,DWORD PTR SS:[ESP+0x18]
	$+DC     006627A9     F3:0F5CC1                SUBSS XMM0,XMM1
	$+E0     006627AD     F3:0F1142 0C             MOVSS DWORD PTR DS:[EDX+0xC],XMM0
	$+E5     006627B2     F3:0F104C24 18           MOVSS XMM1,DWORD PTR SS:[ESP+0x18]
	$+EB     006627B8     8B8A E4000000            MOV ECX,DWORD PTR DS:[EDX+0xE4]
	$+F1     006627BE     85C9                     TEST ECX,ECX
	$+F3     006627C0     74 0B                    JE SHORT PathOfEx.006627CD
	$+F5     006627C2     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+F7     006627C4     51                       PUSH ECX
	$+F8     006627C5     F3:0F110C24              MOVSS DWORD PTR SS:[ESP],XMM1
	$+FD     006627CA     FF50 0C                  CALL DWORD PTR DS:[EAX+0xC]
	$+100    006627CD     0F57C9                   XORPS XMM1,XMM1
	$+103    006627D0     8B87 A0000000            MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+109    006627D6     46                       INC ESI
	$+10A    006627D7     2B87 9C000000            SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+110    006627DD     C1F8 02                  SAR EAX,0x2
	$+113    006627E0     3BF0                     CMP ESI,EAX
	$+115    006627E2   ^ 0F82 FBFEFFFF            JB PathOfEx.006626E3
	$+11B    006627E8     5D                       POP EBP
	$+11C    006627E9     5B                       POP EBX
	$+11D    006627EA     837F 30 00               CMP DWORD PTR DS:[EDI+0x30],0x0
	$+121    006627EE     0F8E 8C000000            JLE PathOfEx.00662880
	$+127    006627F4     8B47 1C                  MOV EAX,DWORD PTR DS:[EDI+0x1C]
	$+12A    006627F7     F3:0F104424 10           MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
	$+130    006627FD     51                       PUSH ECX
	$+131    006627FE     8D4F 1C                  LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+134    00662801     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+139    00662806     FF50 0C                  CALL DWORD PTR DS:[EAX+0xC]
	$+13C    00662809     8B47 68                  MOV EAX,DWORD PTR DS:[EDI+0x68]
	$+13F    0066280C     8D4F 68                  LEA ECX,DWORD PTR DS:[EDI+0x68]
	$+142    0066280F     F3:0F104424 10           MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
	$+148    00662815     51                       PUSH ECX
	$+149    00662816     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+14E    0066281B     FF50 0C                  CALL DWORD PTR DS:[EAX+0xC]
	$+151    0066281E     8B47 44                  MOV EAX,DWORD PTR DS:[EDI+0x44]
	$+154    00662821     8D4F 44                  LEA ECX,DWORD PTR DS:[EDI+0x44]
	$+157    00662824     F3:0F104424 10           MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
	$+15D    0066282A     51                       PUSH ECX
	$+15E    0066282B     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+163    00662830     FF50 0C                  CALL DWORD PTR DS:[EAX+0xC]
	$+166    00662833     837F 2C 01               CMP DWORD PTR DS:[EDI+0x2C],0x1
	$+16A    00662837     75 1D                    JNZ SHORT PathOfEx.00662856
	$+16C    00662839     8D4F 1C                  LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+16F    0066283C     E8 2F83DBFF              CALL PathOfEx.0041AB70
	$+174    00662841     0F57C9                   XORPS XMM1,XMM1
	$+177    00662844     0F2FC8                   COMISS XMM1,XMM0
	$+17A    00662847     76 0D                    JBE SHORT PathOfEx.00662856
	$+17C    00662849     8B47 1C                  MOV EAX,DWORD PTR DS:[EDI+0x1C]
	$+17F    0066284C     8D4F 1C                  LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+182    0066284F     6A 00                    PUSH 0x0
	$+184    00662851     6A FF                    PUSH -0x1
	$+186    00662853     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+189    00662856     80BF A9000000 00         CMP BYTE PTR DS:[EDI+0xA9],0x0
	$+190    0066285D     74 21                    JE SHORT PathOfEx.00662880
	$+192    0066285F     C687 A9000000 00         MOV BYTE PTR DS:[EDI+0xA9],0x0
	$+199    00662866     8B47 14                  MOV EAX,DWORD PTR DS:[EDI+0x14]
	$+19C    00662869     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$+19E    0066286B     3BF0                     CMP ESI,EAX
	$+1A0    0066286D     74 11                    JE SHORT PathOfEx.00662880
	$+1A2    0066286F     90                       NOP
	$+1A3    00662870     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+1A6    00662873     8B36                     MOV ESI,DWORD PTR DS:[ESI]
	$+1A8    00662875     57                       PUSH EDI
	$+1A9    00662876     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+1AB    00662878     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]
	$+1AE    0066287B     3B77 14                  CMP ESI,DWORD PTR DS:[EDI+0x14]
	$+1B1    0066287E   ^ 75 F0                    JNZ SHORT PathOfEx.00662870
	$+1B3    00662880     5F                       POP EDI
	$+1B4    00662881     5E                       POP ESI
	$+1B5    00662882     59                       POP ECX
	$+1B6    00662883     C2 0400                  RETN 0x4
	$+1B9    00662886     CC                       INT3
	$+1BA    00662887     CC                       INT3
	$+1BB    00662888     CC                       INT3
	$+1BC    00662889     CC                       INT3
	$+1BD    0066288A     CC                       INT3
	$+1BE    0066288B     CC                       INT3
	$+1BF    0066288C     CC                       INT3
	$+1C0    0066288D     CC                       INT3
	$+1C1    0066288E     CC                       INT3
	$+1C2    0066288F     CC                       INT3
	$+1C3    00662890     8B4C24 08                MOV ECX,DWORD PTR SS:[ESP+0x8]
	$+1C7    00662894     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+1CA    00662897     F640 24 04               TEST BYTE PTR DS:[EAX+0x24],0x4
	$+1CE    0066289B     74 0B                    JE SHORT PathOfEx.006628A8
	$+1D0    0066289D     F3:0F104C24 04           MOVSS XMM1,DWORD PTR SS:[ESP+0x4]
	$+1D6    006628A3     E9 18000000              JMP PathOfEx.006628C0
	$+1DB    006628A8     C3                       RETN
	$+1DC    006628A9     CC                       INT3
	$+1DD    006628AA     CC                       INT3
	$+1DE    006628AB     CC                       INT3
	$+1DF    006628AC     CC                       INT3
	$+1E0    006628AD     CC                       INT3
	$+1E1    006628AE     CC                       INT3
	$+1E2    006628AF     CC                       INT3
	$+1E3    006628B0     F3:0F104C24 04           MOVSS XMM1,DWORD PTR SS:[ESP+0x4]
	$+1E9    006628B6     E8 05000000              CALL PathOfEx.006628C0
	$+1EE    006628BB     C2 0400                  RETN 0x4
	$+1F1    006628BE     CC                       INT3
	$+1F2    006628BF     CC                       INT3
	$+1F3    006628C0     51                       PUSH ECX
	$+1F4    006628C1     56                       PUSH ESI
	$+1F5    006628C2     57                       PUSH EDI
	$+1F6    006628C3     8BF9                     MOV EDI,ECX
	$+1F8    006628C5     0F28D1                   MOVAPS XMM2,XMM1
	$+1FB    006628C8     33F6                     XOR ESI,ESI
	$+1FD    006628CA     F3:0F115424 08           MOVSS DWORD PTR SS:[ESP+0x8],XMM2
	$+203    006628D0     8B87 A0000000            MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+209    006628D6     2B87 9C000000            SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+20F    006628DC     C1F8 02                  SAR EAX,0x2
	$+212    006628DF     85C0                     TEST EAX,EAX
	$+214    006628E1     74 55                    JE SHORT PathOfEx.00662938
	$+216    006628E3     0F57C9                   XORPS XMM1,XMM1
	$+219    006628E6     66                       DATASIZE:
	$+21A    006628E7     66:0F1F8400 00000000     NOP WORD PTR DS:[EAX+EAX]
	$+223    006628F0     8B87 9C000000            MOV EAX,DWORD PTR DS:[EDI+0x9C]
	$+229    006628F6     8B04B0                   MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+22C    006628F9     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+22F    006628FC     F3:0F1041 0C             MOVSS XMM0,DWORD PTR DS:[ECX+0xC]
	$+234    00662901     0F2FC1                   COMISS XMM0,XMM1
	$+237    00662904     76 1E                    JBE SHORT PathOfEx.00662924
	$+239    00662906     8B89 E4000000            MOV ECX,DWORD PTR DS:[ECX+0xE4]
	$+23F    0066290C     85C9                     TEST ECX,ECX
	$+241    0066290E     74 14                    JE SHORT PathOfEx.00662924
	$+243    00662910     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+245    00662912     51                       PUSH ECX
	$+246    00662913     F3:0F111424              MOVSS DWORD PTR SS:[ESP],XMM2
	$+24B    00662918     FF50 10                  CALL DWORD PTR DS:[EAX+0x10]
	$+24E    0066291B     F3:0F105424 08           MOVSS XMM2,DWORD PTR SS:[ESP+0x8]
	$+254    00662921     0F57C9                   XORPS XMM1,XMM1
	$+257    00662924     8B87 A0000000            MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+25D    0066292A     46                       INC ESI
	$+25E    0066292B     2B87 9C000000            SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+264    00662931     C1F8 02                  SAR EAX,0x2
	$+267    00662934     3BF0                     CMP ESI,EAX
	$+269    00662936   ^ 72 B8                    JB SHORT PathOfEx.006628F0
	$+26B    00662938     5F                       POP EDI
	$+26C    00662939     5E                       POP ESI
	$+26D    0066293A     59                       POP ECX
	$+26E    0066293B     C3                       RETN
	$+26F    0066293C     CC                       INT3
	$+270    0066293D     CC                       INT3
	$+271    0066293E     CC                       INT3
	$+272    0066293F     CC                       INT3
	$+273    00662940     57                       PUSH EDI
	$+274    00662941     8BF9                     MOV EDI,ECX
	$+276    00662943     E8 8825CBFF              CALL PathOfEx.00314ED0
	$+27B    00662948     3947 14                  CMP DWORD PTR DS:[EDI+0x14],EAX
	$+27E    0066294B     7D 41                    JGE SHORT PathOfEx.0066298E
	$+280    0066294D     8B57 0C                  MOV EDX,DWORD PTR DS:[EDI+0xC]
	$+283    00662950     56                       PUSH ESI
	$+284    00662951     FF77 08                  PUSH DWORD PTR DS:[EDI+0x8]
	$+287    00662954     8B42 10                  MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+28A    00662957     8BB0 88000000            MOV ESI,DWORD PTR DS:[EAX+0x88]
	$+290    0066295D     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+293    00662960     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+296    00662963     8B04B0                   MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+299    00662966     8B48 10                  MOV ECX,DWORD PTR DS:[EAX+0x10]
	$+29C    00662969     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+29E    0066296B     FF10                     CALL DWORD PTR DS:[EAX]
	$+2A0    0066296D     5E                       POP ESI
	$+2A1    0066296E     85C0                     TEST EAX,EAX
	$+2A3    00662970     7E 1C                    JLE SHORT PathOfEx.0066298E
	$+2A5    00662972     66:0F6EC8                MOVD MM1,EAX
	$+2A9    00662976     8BCF                     MOV ECX,EDI
	$+2AB    00662978     0F5BC9                   CVTDQ2PS XMM1,XMM1
	$+2AE    0066297B     F3:0F5E0D 78B3F700       DIVSS XMM1,DWORD PTR DS:[0xF7B378]
	$+2B6    00662983     F3:0F594C24 08           MULSS XMM1,DWORD PTR SS:[ESP+0x8]
	$+2BC    00662989     E8 12000000              CALL PathOfEx.006629A0
	$+2C1    0066298E     5F                       POP EDI
	$+2C2    0066298F     C2 0400                  RETN 0x4
	$+2C5    00662992     CC                       INT3
	$+2C6    00662993     CC                       INT3
	$+2C7    00662994     CC                       INT3
	$+2C8    00662995     CC                       INT3
	$+2C9    00662996     CC                       INT3
	$+2CA    00662997     CC                       INT3
	$+2CB    00662998     CC                       INT3
	$+2CC    00662999     CC                       INT3
	$+2CD    0066299A     CC                       INT3
	$+2CE    0066299B     CC                       INT3
	$+2CF    0066299C     CC                       INT3
	$+2D0    0066299D     CC                       INT3
	$+2D1    0066299E     CC                       INT3
	$+2D2    0066299F     CC                       INT3
	$+2D3    006629A0     56                       PUSH ESI
	$+2D4    006629A1     8BF1                     MOV ESI,ECX
	$+2D6    006629A3     57                       PUSH EDI
	$+2D7    006629A4     F3:0F584E 18             ADDSS XMM1,DWORD PTR DS:[ESI+0x18]
	$+2DC    006629A9     F3:0F2CF9                CVTTSS2SI EDI,XMM1
	$+2E0    006629AD     017E 14                  ADD DWORD PTR DS:[ESI+0x14],EDI
	$+2E3    006629B0     66:0F6EC7                MOVD MM0,EDI
	$+2E7    006629B4     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+2EA    006629B7     F3:0F5CC8                SUBSS XMM1,XMM0
	$+2EE    006629BB     F3:0F114E 18             MOVSS DWORD PTR DS:[ESI+0x18],XMM1
	$+2F3    006629C0     E8 0B25CBFF              CALL PathOfEx.00314ED0
	$+2F8    006629C5     3946 14                  CMP DWORD PTR DS:[ESI+0x14],EAX
	$+2FB    006629C8     7C 15                    JL SHORT PathOfEx.006629DF
	$+2FD    006629CA     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+300    006629CD     FF76 08                  PUSH DWORD PTR DS:[ESI+0x8]
	$+303    006629D0     8946 14                  MOV DWORD PTR DS:[ESI+0x14],EAX
	$+306    006629D3     C746 18 00000000         MOV DWORD PTR DS:[ESI+0x18],0x0
	$+30D    006629DA     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+30F    006629DC     FF50 54                  CALL DWORD PTR DS:[EAX+0x54]
	$+312    006629DF     85FF                     TEST EDI,EDI
	$+314    006629E1     5F                       POP EDI
	$+315    006629E2     74 09                    JE SHORT PathOfEx.006629ED
	$+317    006629E4     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+31A    006629E7     5E                       POP ESI
	$+31B    006629E8     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+31D    006629EA     FF60 48                  JMP DWORD PTR DS:[EAX+0x48]
	$+320    006629ED     5E                       POP ESI
	$+321    006629EE     C3                       RETN
	$+322    006629EF     CC                       INT3
	$+323    006629F0     56                       PUSH ESI
	$+324    006629F1     8BF1                     MOV ESI,ECX
	$+326    006629F3     0F57D2                   XORPS XMM2,XMM2
	$+329    006629F6     837E 14 00               CMP DWORD PTR DS:[ESI+0x14],0x0
	$+32D    006629FA     7F 06                    JG SHORT PathOfEx.00662A02
	$+32F    006629FC     0F2F56 18                COMISS XMM2,DWORD PTR DS:[ESI+0x18]
	$+333    00662A00     73 6E                    JNB SHORT PathOfEx.00662A70
	$+335    00662A02     F3:0F104C24 08           MOVSS XMM1,DWORD PTR SS:[ESP+0x8]
	$+33B    00662A08     F3:0F584E 18             ADDSS XMM1,DWORD PTR DS:[ESI+0x18]
	$+340    00662A0D     57                       PUSH EDI
	$+341    00662A0E     C74424 0C 00000000       MOV DWORD PTR SS:[ESP+0xC],0x0
	$+349    00662A16     F3:0F2CF9                CVTTSS2SI EDI,XMM1
	$+34D    00662A1A     017E 14                  ADD DWORD PTR DS:[ESI+0x14],EDI
	$+350    00662A1D     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+353    00662A20     66:0F6EC7                MOVD MM0,EDI
	$+357    00662A24     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+35A    00662A27     F3:0F5CC8                SUBSS XMM1,XMM0
	$+35E    00662A2B     F3:0F114E 18             MOVSS DWORD PTR DS:[ESI+0x18],XMM1
	$+363    00662A30     85C0                     TEST EAX,EAX
	$+365    00662A32     7F 2F                    JG SHORT PathOfEx.00662A63
	$+367    00662A34     0F2FD1                   COMISS XMM2,XMM1
	$+36A    00662A37     72 2A                    JB SHORT PathOfEx.00662A63
	$+36C    00662A39     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+36F    00662A3C     FF76 08                  PUSH DWORD PTR DS:[ESI+0x8]
	$+372    00662A3F     66:0F6EC0                MOVD MM0,EAX
	$+376    00662A43     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+379    00662A46     C746 14 00000000         MOV DWORD PTR DS:[ESI+0x14],0x0
	$+380    00662A4D     C746 18 00000000         MOV DWORD PTR DS:[ESI+0x18],0x0
	$+387    00662A54     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+389    00662A56     F3:0F58C1                ADDSS XMM0,XMM1
	$+38D    00662A5A     F3:0F114424 10           MOVSS DWORD PTR SS:[ESP+0x10],XMM0
	$+393    00662A60     FF50 5C                  CALL DWORD PTR DS:[EAX+0x5C]
	$+396    00662A63     85FF                     TEST EDI,EDI
	$+398    00662A65     5F                       POP EDI
	$+399    00662A66     74 08                    JE SHORT PathOfEx.00662A70
	$+39B    00662A68     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+39E    00662A6B     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+3A0    00662A6D     FF50 48                  CALL DWORD PTR DS:[EAX+0x48]
	$+3A3    00662A70     D94424 08                FLD DWORD PTR SS:[ESP+0x8]
	$+3A7    00662A74     5E                       POP ESI
	$+3A8    00662A75     C2 0800                  RETN 0x8
	$+3AB    00662A78     CC                       INT3
	$+3AC    00662A79     CC                       INT3
	$+3AD    00662A7A     CC                       INT3
	$+3AE    00662A7B     CC                       INT3
	$+3AF    00662A7C     CC                       INT3
	$+3B0    00662A7D     CC                       INT3
	$+3B1    00662A7E     CC                       INT3
	$+3B2    00662A7F     CC                       INT3
	$+3B3    00662A80     51                       PUSH ECX
	$+3B4    00662A81     53                       PUSH EBX
	$+3B5    00662A82     56                       PUSH ESI
	$+3B6    00662A83     57                       PUSH EDI
	$+3B7    00662A84     8BF9                     MOV EDI,ECX
	$+3B9    00662A86     68 B3140000              PUSH 0x14B3
	$+3BE    00662A8B     8B57 0C                  MOV EDX,DWORD PTR DS:[EDI+0xC]
	$+3C1    00662A8E     8B42 10                  MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+3C4    00662A91     8BB0 88000000            MOV ESI,DWORD PTR DS:[EAX+0x88]
	$+3CA    00662A97     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+3CD    00662A9A     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+3D0    00662A9D     8B04B0                   MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+3D3    00662AA0     8B70 10                  MOV ESI,DWORD PTR DS:[EAX+0x10]
	$+3D6    00662AA3     8BCE                     MOV ECX,ESI
	$+3D8    00662AA5     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+3DA    00662AA7     FF10                     CALL DWORD PTR DS:[EAX]
	$+3DC    00662AA9     F3:0F104424 14           MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$+3E2    00662AAF     85C0                     TEST EAX,EAX
	$+3E4    00662AB1     6A 00                    PUSH 0x0
	$+3E6    00662AB3     51                       PUSH ECX
	$+3E7    00662AB4     8BCF                     MOV ECX,EDI
	$+3E9    00662AB6     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+3EE    00662ABB     0F95C3                   SETNE BL
	$+3F1    00662ABE     E8 2DFFFFFF              CALL PathOfEx.006629F0
	$+3F6    00662AC3     D95C24 14                FSTP DWORD PTR SS:[ESP+0x14]
	$+3FA    00662AC7     F3:0F104C24 14           MOVSS XMM1,DWORD PTR SS:[ESP+0x14]
	$+400    00662ACD     0F57C0                   XORPS XMM0,XMM0
	$+403    00662AD0     0F2FC1                   COMISS XMM0,XMM1
	$+406    00662AD3     0F86 8F000000            JBE PathOfEx.00662B68
	$+40C    00662AD9     84DB                     TEST BL,BL
	$+40E    00662ADB     74 1E                    JE SHORT PathOfEx.00662AFB
	$+410    00662ADD     8B4F 0C                  MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+413    00662AE0     FF7424 18                PUSH DWORD PTR SS:[ESP+0x18]
	$+417    00662AE4     83C1 44                  ADD ECX,0x44
	$+41A    00662AE7     51                       PUSH ECX
	$+41B    00662AE8     F3:0F110C24              MOVSS DWORD PTR SS:[ESP],XMM1
	$+420    00662AED     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+422    00662AEF     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+425    00662AF2     FFD0                     CALL EAX
	$+427    00662AF4     5F                       POP EDI
	$+428    00662AF5     5E                       POP ESI
	$+429    00662AF6     5B                       POP EBX
	$+42A    00662AF7     59                       POP ECX
	$+42B    00662AF8     C2 0800                  RETN 0x8
	$+42E    00662AFB     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+430    00662AFD     8BCE                     MOV ECX,ESI
	$+432    00662AFF     68 8B060000              PUSH 0x68B
	$+437    00662B04     FF10                     CALL DWORD PTR DS:[EAX]
	$+439    00662B06     8B4F 0C                  MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+43C    00662B09     8B7424 18                MOV ESI,DWORD PTR SS:[ESP+0x18]
	$+440    00662B0D     83C1 44                  ADD ECX,0x44
	$+443    00662B10     56                       PUSH ESI
	$+444    00662B11     66:0F6EC0                MOVD MM0,EAX
	$+448    00662B15     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+44B    00662B18     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+44D    00662B1A     51                       PUSH ECX
	$+44E    00662B1B     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+451    00662B1E     F3:0F594424 1C           MULSS XMM0,DWORD PTR SS:[ESP+0x1C]
	$+457    00662B24     F3:0F5E05 00B4F700       DIVSS XMM0,DWORD PTR DS:[0xF7B400]
	$+45F    00662B2C     F3:0F114424 14           MOVSS DWORD PTR SS:[ESP+0x14],XMM0
	$+465    00662B32     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+46A    00662B37     FFD0                     CALL EAX
	$+46C    00662B39     8B4F 0C                  MOV ECX,DWORD PTR DS:[EDI+0xC]
	$+46F    00662B3C     F3:0F104424 14           MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$+475    00662B42     83C1 1C                  ADD ECX,0x1C
	$+478    00662B45     F3:0F5C4424 0C           SUBSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+47E    00662B4B     56                       PUSH ESI
	$+47F    00662B4C     51                       PUSH ECX
	$+480    00662B4D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+482    00662B4F     D95C24 20                FSTP DWORD PTR SS:[ESP+0x20]
	$+486    00662B53     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+48B    00662B58     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+48E    00662B5B     FFD0                     CALL EAX
	$+490    00662B5D     D84424 18                FADD DWORD PTR SS:[ESP+0x18]
	$+494    00662B61     5F                       POP EDI
	$+495    00662B62     5E                       POP ESI
	$+496    00662B63     5B                       POP EBX
	$+497    00662B64     59                       POP ECX
	$+498    00662B65     C2 0800                  RETN 0x8
	$+49B    00662B68     5F                       POP EDI
	$+49C    00662B69     5E                       POP ESI
	$+49D    00662B6A     D9EE                     FLDZ
	$+49F    00662B6C     5B                       POP EBX
	$+4A0    00662B6D     59                       POP ECX
	$+4A1    00662B6E     C2 0800                  RETN 0x8
	$+4A4    00662B71     CC                       INT3
	*/
	//gj 2017/8/18 16:16
	/*
	$-1BD    00C7FB2F     CC                     INT3
	$-1BC    00C7FB30     53                     PUSH EBX
	$-1BB    00C7FB31     8B5C24 08              MOV EBX,DWORD PTR SS:[ESP+0x8]
	$-1B7    00C7FB35     56                     PUSH ESI
	$-1B6    00C7FB36     8BF1                   MOV ESI,ECX
	$-1B4    00C7FB38     57                     PUSH EDI
	$-1B3    00C7FB39     8B7E 14                MOV EDI,DWORD PTR DS:[ESI+0x14]
	$-1B0    00C7FB3C     397B 14                CMP DWORD PTR DS:[EBX+0x14],EDI
	$-1AD    00C7FB3F     7D 30                  JGE SHORT PathOfEx.00C7FB71
	$-1AB    00C7FB41     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-1A8    00C7FB44     68 A9140000            PUSH 0x14A9
	$-1A3    00C7FB49     8B41 10                MOV EAX,DWORD PTR DS:[ECX+0x10]
	$-1A0    00C7FB4C     8B90 88000000          MOV EDX,DWORD PTR DS:[EAX+0x88]
	$-19A    00C7FB52     8B41 04                MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-197    00C7FB55     8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-194    00C7FB58     8B0490                 MOV EAX,DWORD PTR DS:[EAX+EDX*4]
	$-191    00C7FB5B     8B48 10                MOV ECX,DWORD PTR DS:[EAX+0x10]
	$-18E    00C7FB5E     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-18C    00C7FB60     FF10                   CALL DWORD PTR DS:[EAX]
	$-18A    00C7FB62     85C0                   TEST EAX,EAX
	$-188    00C7FB64     75 0B                  JNZ SHORT PathOfEx.00C7FB71
	$-186    00C7FB66     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-183    00C7FB69     83C1 68                ADD ECX,0x68
	$-180    00C7FB6C     E8 BF110000            CALL PathOfEx.00C80D30
	$-17B    00C7FB71     53                     PUSH EBX
	$-17A    00C7FB72     8BCE                   MOV ECX,ESI
	$-178    00C7FB74     E8 07FFFFFF            CALL PathOfEx.00C7FA80
	$-173    00C7FB79     85FF                   TEST EDI,EDI
	$-171    00C7FB7B     7E 1D                  JLE SHORT PathOfEx.00C7FB9A
	$-16F    00C7FB7D     837E 14 00             CMP DWORD PTR DS:[ESI+0x14],0x0
	$-16B    00C7FB81     7F 13                  JG SHORT PathOfEx.00C7FB96
	$-169    00C7FB83     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-166    00C7FB86     5F                     POP EDI
	$-165    00C7FB87     5E                     POP ESI
	$-164    00C7FB88     5B                     POP EBX
	$-163    00C7FB89     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-161    00C7FB8B     C74424 04 00000000     MOV DWORD PTR SS:[ESP+0x4],0x0
	$-159    00C7FB93     FF60 44                JMP DWORD PTR DS:[EAX+0x44]
	$-156    00C7FB96     85FF                   TEST EDI,EDI
	$-154    00C7FB98     7F 15                  JG SHORT PathOfEx.00C7FBAF
	$-152    00C7FB9A     837E 14 00             CMP DWORD PTR DS:[ESI+0x14],0x0
	$-14E    00C7FB9E     7E 0F                  JLE SHORT PathOfEx.00C7FBAF
	$-14C    00C7FBA0     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$-149    00C7FBA3     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-147    00C7FBA5     FF50 50                CALL DWORD PTR DS:[EAX+0x50]
	$-144    00C7FBA8     C746 18 00000000       MOV DWORD PTR DS:[ESI+0x18],0x0
	$-13D    00C7FBAF     5F                     POP EDI
	$-13C    00C7FBB0     5E                     POP ESI
	$-13B    00C7FBB1     5B                     POP EBX
	$-13A    00C7FBB2     C2 0400                RETN 0x4
	$-137    00C7FBB5     CC                     INT3
	$-136    00C7FBB6     CC                     INT3
	$-135    00C7FBB7     CC                     INT3
	$-134    00C7FBB8     CC                     INT3
	$-133    00C7FBB9     CC                     INT3
	$-132    00C7FBBA     CC                     INT3
	$-131    00C7FBBB     CC                     INT3
	$-130    00C7FBBC     CC                     INT3
	$-12F    00C7FBBD     CC                     INT3
	$-12E    00C7FBBE     CC                     INT3
	$-12D    00C7FBBF     CC                     INT3
	$-12C    00C7FBC0     8B4424 04              MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-128    00C7FBC4     57                     PUSH EDI
	$-127    00C7FBC5     8BF9                   MOV EDI,ECX
	$-125    00C7FBC7     3D 97050000            CMP EAX,0x597
	$-120    00C7FBCC     74 58                  JE SHORT PathOfEx.00C7FC26
	$-11E    00C7FBCE     3D 98050000            CMP EAX,0x598
	$-119    00C7FBD3     74 51                  JE SHORT PathOfEx.00C7FC26
	$-117    00C7FBD5     80BF 8C000000 00       CMP BYTE PTR DS:[EDI+0x8C],0x0
	$-110    00C7FBDC     75 48                  JNZ SHORT PathOfEx.00C7FC26
	$-10E    00C7FBDE     8B87 88000000          MOV EAX,DWORD PTR DS:[EDI+0x88]
	$-108    00C7FBE4     56                     PUSH ESI
	$-107    00C7FBE5     8B77 78                MOV ESI,DWORD PTR DS:[EDI+0x78]
	$-104    00C7FBE8     0FAFC6                 IMUL EAX,ESI
	$-101    00C7FBEB     66:0F6EC0              MOVD MM0,EAX
	$-FD     00C7FBEF     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$-FA     00C7FBF2     F3:0F5E05 107C5B01     DIVSS XMM0,DWORD PTR DS:[0x15B7C10]
	$-F2     00C7FBFA     E8 71A3C9FF            CALL PathOfEx.00919F70
	$-ED     00C7FBFF     2BB7 84000000          SUB ESI,DWORD PTR DS:[EDI+0x84]
	$-E7     00C7FC05     F3:0F2CC0              CVTTSS2SI EAX,XMM0
	$-E3     00C7FC09     2BF0                   SUB ESI,EAX
	$-E1     00C7FC0B     B8 00000000            MOV EAX,0x0
	$-DC     00C7FC10     0F48F0                 CMOVS ESI,EAX
	$-D9     00C7FC13     85F6                   TEST ESI,ESI
	$-D7     00C7FC15     5E                     POP ESI
	$-D6     00C7FC16     74 0E                  JE SHORT PathOfEx.00C7FC26
	$-D4     00C7FC18     8B07                   MOV EAX,DWORD PTR DS:[EDI]
	$-D2     00C7FC1A     8BCF                   MOV ECX,EDI
	$-D0     00C7FC1C     FF50 58                CALL DWORD PTR DS:[EAX+0x58]
	$-CD     00C7FC1F     C687 8C000000 01       MOV BYTE PTR DS:[EDI+0x8C],0x1
	$-C6     00C7FC26     5F                     POP EDI
	$-C5     00C7FC27     C2 0400                RETN 0x4
	$-C2     00C7FC2A     CC                     INT3
	$-C1     00C7FC2B     CC                     INT3
	$-C0     00C7FC2C     CC                     INT3
	$-BF     00C7FC2D     CC                     INT3
	$-BE     00C7FC2E     CC                     INT3
	$-BD     00C7FC2F     CC                     INT3
	$-BC     00C7FC30     8B4424 04              MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-B8     00C7FC34     57                     PUSH EDI
	$-B7     00C7FC35     8BF9                   MOV EDI,ECX
	$-B5     00C7FC37     3D 97050000            CMP EAX,0x597
	$-B0     00C7FC3C     74 6A                  JE SHORT PathOfEx.00C7FCA8
	$-AE     00C7FC3E     3D 98050000            CMP EAX,0x598
	$-A9     00C7FC43     74 63                  JE SHORT PathOfEx.00C7FCA8
	$-A7     00C7FC45     80BF 8D000000 00       CMP BYTE PTR DS:[EDI+0x8D],0x0
	$-A0     00C7FC4C     75 5A                  JNZ SHORT PathOfEx.00C7FCA8
	$-9E     00C7FC4E     8B87 88000000          MOV EAX,DWORD PTR DS:[EDI+0x88]
	$-98     00C7FC54     56                     PUSH ESI
	$-97     00C7FC55     8B77 78                MOV ESI,DWORD PTR DS:[EDI+0x78]
	$-94     00C7FC58     0FAFC6                 IMUL EAX,ESI
	$-91     00C7FC5B     66:0F6EC0              MOVD MM0,EAX
	$-8D     00C7FC5F     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$-8A     00C7FC62     F3:0F5E05 107C5B01     DIVSS XMM0,DWORD PTR DS:[0x15B7C10]
	$-82     00C7FC6A     E8 01A3C9FF            CALL PathOfEx.00919F70
	$-7D     00C7FC6F     2BB7 84000000          SUB ESI,DWORD PTR DS:[EDI+0x84]
	$-77     00C7FC75     F3:0F2CC0              CVTTSS2SI EAX,XMM0
	$-73     00C7FC79     2BF0                   SUB ESI,EAX
	$-71     00C7FC7B     B8 00000000            MOV EAX,0x0
	$-6C     00C7FC80     0F48F0                 CMOVS ESI,EAX
	$-69     00C7FC83     85F6                   TEST ESI,ESI
	$-67     00C7FC85     74 20                  JE SHORT PathOfEx.00C7FCA7
	$-65     00C7FC87     8B47 14                MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-62     00C7FC8A     8B30                   MOV ESI,DWORD PTR DS:[EAX]
	$-60     00C7FC8C     3BF0                   CMP ESI,EAX
	$-5E     00C7FC8E     74 10                  JE SHORT PathOfEx.00C7FCA0
	$-5C     00C7FC90     8B4E 08                MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-59     00C7FC93     8B36                   MOV ESI,DWORD PTR DS:[ESI]
	$-57     00C7FC95     57                     PUSH EDI
	$-56     00C7FC96     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-54     00C7FC98     FF50 30                CALL DWORD PTR DS:[EAX+0x30]
	$-51     00C7FC9B     3B77 14                CMP ESI,DWORD PTR DS:[EDI+0x14]
	$-4E     00C7FC9E   ^ 75 F0                  JNZ SHORT PathOfEx.00C7FC90
	$-4C     00C7FCA0     C687 8D000000 01       MOV BYTE PTR DS:[EDI+0x8D],0x1
	$-45     00C7FCA7     5E                     POP ESI
	$-44     00C7FCA8     5F                     POP EDI
	$-43     00C7FCA9     C2 0400                RETN 0x4
	$-40     00C7FCAC     CC                     INT3
	$-3F     00C7FCAD     CC                     INT3
	$-3E     00C7FCAE     CC                     INT3
	$-3D     00C7FCAF     CC                     INT3
	$-3C     00C7FCB0     53                     PUSH EBX
	$-3B     00C7FCB1     55                     PUSH EBP
	$-3A     00C7FCB2     56                     PUSH ESI
	$-39     00C7FCB3     57                     PUSH EDI
	$-38     00C7FCB4     8BF9                   MOV EDI,ECX
	$-36     00C7FCB6     68 16020000            PUSH 0x216
	$-31     00C7FCBB     8B47 10                MOV EAX,DWORD PTR DS:[EDI+0x10]
	$-2E     00C7FCBE     8B90 88000000          MOV EDX,DWORD PTR DS:[EAX+0x88]
	$-28     00C7FCC4     8B47 04                MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-25     00C7FCC7     8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-22     00C7FCCA     8B3490                 MOV ESI,DWORD PTR DS:[EAX+EDX*4]
	$-1F     00C7FCCD     8B4E 10                MOV ECX,DWORD PTR DS:[ESI+0x10]
	$-1C     00C7FCD0     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-1A     00C7FCD2     FF10                   CALL DWORD PTR DS:[EAX]
	$-18     00C7FCD4     8B4E 10                MOV ECX,DWORD PTR DS:[ESI+0x10]
	$-15     00C7FCD7     8BD8                   MOV EBX,EAX
	$-13     00C7FCD9     68 F9180000            PUSH 0x18F9
	$-E      00C7FCDE     F7DB                   NEG EBX
	$-C      00C7FCE0     8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-A      00C7FCE2     FF12                   CALL DWORD PTR DS:[EDX]
	$-8      00C7FCE4     8B8F A0000000          MOV ECX,DWORD PTR DS:[EDI+0xA0]
	$-2      00C7FCEA     33F6                   XOR ESI,ESI
	$ ==>    00C7FCEC     2B8F 9C000000          SUB ECX,DWORD PTR DS:[EDI+0x9C]          ; 技能偏移首地址
	$+6      00C7FCF2     8BE8                   MOV EBP,EAX
	$+8      00C7FCF4     C1F9 02                SAR ECX,0x2
	$+B      00C7FCF7     85C9                   TEST ECX,ECX
	$+D      00C7FCF9     0F84 D9000000          JE PathOfEx.00C7FDD8
	$+13     00C7FCFF     F3:0F1015 08795B01     MOVSS XMM2,DWORD PTR DS:[0x15B7908]
	$+1B     00C7FD07     66:0F1F8400 00000000   NOP WORD PTR DS:[EAX+EAX]
	$+24     00C7FD10     8B87 9C000000          MOV EAX,DWORD PTR DS:[EDI+0x9C]          ; 技能偏移首地址
	$+2A     00C7FD16     0F57C9                 XORPS XMM1,XMM1
	$+2D     00C7FD19     8B14B0                 MOV EDX,DWORD PTR DS:[EAX+ESI*4]
	$+30     00C7FD1C     8B4A 04                MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+33     00C7FD1F     F3:0F1041 0C           MOVSS XMM0,DWORD PTR DS:[ECX+0xC]
	$+38     00C7FD24     0F2FC1                 COMISS XMM0,XMM1
	$+3B     00C7FD27     0F86 93000000          JBE PathOfEx.00C7FDC0
	$+41     00C7FD2D     837A 20 02             CMP DWORD PTR DS:[EDX+0x20],0x2
	$+45     00C7FD31     74 6A                  JE SHORT PathOfEx.00C7FD9D
	$+47     00C7FD33     8B41 04                MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+4A     00C7FD36     8078 48 00             CMP BYTE PTR DS:[EAX+0x48],0x0
	$+4E     00C7FD3A     74 05                  JE SHORT PathOfEx.00C7FD41
	$+50     00C7FD3C     0F28C2                 MOVAPS XMM0,XMM2
	$+53     00C7FD3F     EB 35                  JMP SHORT PathOfEx.00C7FD76
	$+55     00C7FD41     8B40 3B                MOV EAX,DWORD PTR DS:[EAX+0x3B]
	$+58     00C7FD44     83F8 02                CMP EAX,0x2
	$+5B     00C7FD47     74 0F                  JE SHORT PathOfEx.00C7FD58
	$+5D     00C7FD49     83F8 05                CMP EAX,0x5
	$+60     00C7FD4C     74 0A                  JE SHORT PathOfEx.00C7FD58
	$+62     00C7FD4E     83F8 0B                CMP EAX,0xB
	$+65     00C7FD51     74 05                  JE SHORT PathOfEx.00C7FD58
	$+67     00C7FD53     83F8 0C                CMP EAX,0xC
	$+6A     00C7FD56     75 07                  JNZ SHORT PathOfEx.00C7FD5F
	$+6C     00C7FD58     66:0F6ECD              MOVD MM1,EBP
	$+70     00C7FD5C     0F5BC9                 CVTDQ2PS XMM1,XMM1
	$+73     00C7FD5F     66:0F6EC3              MOVD MM0,EBX
	$+77     00C7FD63     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$+7A     00C7FD66     F3:0F58C1              ADDSS XMM0,XMM1
	$+7E     00C7FD6A     F3:0F5E05 107C5B01     DIVSS XMM0,DWORD PTR DS:[0x15B7C10]
	$+86     00C7FD72     F3:0F58C2              ADDSS XMM0,XMM2
	$+8A     00C7FD76     F3:0F105C24 14         MOVSS XMM3,DWORD PTR SS:[ESP+0x14]
	$+90     00C7FD7C     F3:0F1141 10           MOVSS DWORD PTR DS:[ECX+0x10],XMM0
	$+95     00C7FD81     8B4A 04                MOV ECX,DWORD PTR DS:[EDX+0x4]
	$+98     00C7FD84     F3:0F1049 10           MOVSS XMM1,DWORD PTR DS:[ECX+0x10]
	$+9D     00C7FD89     F3:0F1041 0C           MOVSS XMM0,DWORD PTR DS:[ECX+0xC]
	$+A2     00C7FD8E     F3:0F59CB              MULSS XMM1,XMM3
	$+A6     00C7FD92     F3:0F5CC1              SUBSS XMM0,XMM1
	$+AA     00C7FD96     F3:0F1141 0C           MOVSS DWORD PTR DS:[ECX+0xC],XMM0
	$+AF     00C7FD9B     EB 06                  JMP SHORT PathOfEx.00C7FDA3
	$+B1     00C7FD9D     F3:0F105C24 14         MOVSS XMM3,DWORD PTR SS:[ESP+0x14]
	$+B7     00C7FDA3     8B89 E8000000          MOV ECX,DWORD PTR DS:[ECX+0xE8]
	$+BD     00C7FDA9     85C9                   TEST ECX,ECX
	$+BF     00C7FDAB     74 13                  JE SHORT PathOfEx.00C7FDC0
	$+C1     00C7FDAD     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+C3     00C7FDAF     51                     PUSH ECX
	$+C4     00C7FDB0     F3:0F111C24            MOVSS DWORD PTR SS:[ESP],XMM3
	$+C9     00C7FDB5     FF50 0C                CALL DWORD PTR DS:[EAX+0xC]
	$+CC     00C7FDB8     F3:0F1015 08795B01     MOVSS XMM2,DWORD PTR DS:[0x15B7908]
	$+D4     00C7FDC0     8B87 A0000000          MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+DA     00C7FDC6     46                     INC ESI
	$+DB     00C7FDC7     2B87 9C000000          SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+E1     00C7FDCD     C1F8 02                SAR EAX,0x2
	$+E4     00C7FDD0     3BF0                   CMP ESI,EAX
	$+E6     00C7FDD2   ^ 0F82 38FFFFFF          JB PathOfEx.00C7FD10
	$+EC     00C7FDD8     837F 30 00             CMP DWORD PTR DS:[EDI+0x30],0x0
	$+F0     00C7FDDC     0F8E 8E000000          JLE PathOfEx.00C7FE70
	$+F6     00C7FDE2     8B47 1C                MOV EAX,DWORD PTR DS:[EDI+0x1C]
	$+F9     00C7FDE5     F3:0F104424 14         MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$+FF     00C7FDEB     51                     PUSH ECX
	$+100    00C7FDEC     8D4F 1C                LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+103    00C7FDEF     F3:0F110424            MOVSS DWORD PTR SS:[ESP],XMM0
	$+108    00C7FDF4     FF50 0C                CALL DWORD PTR DS:[EAX+0xC]
	$+10B    00C7FDF7     8B47 68                MOV EAX,DWORD PTR DS:[EDI+0x68]
	$+10E    00C7FDFA     8D4F 68                LEA ECX,DWORD PTR DS:[EDI+0x68]
	$+111    00C7FDFD     F3:0F104424 14         MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$+117    00C7FE03     51                     PUSH ECX
	$+118    00C7FE04     F3:0F110424            MOVSS DWORD PTR SS:[ESP],XMM0
	$+11D    00C7FE09     FF50 0C                CALL DWORD PTR DS:[EAX+0xC]
	$+120    00C7FE0C     8B47 44                MOV EAX,DWORD PTR DS:[EDI+0x44]
	$+123    00C7FE0F     8D4F 44                LEA ECX,DWORD PTR DS:[EDI+0x44]
	$+126    00C7FE12     F3:0F104424 14         MOVSS XMM0,DWORD PTR SS:[ESP+0x14]
	$+12C    00C7FE18     51                     PUSH ECX
	$+12D    00C7FE19     F3:0F110424            MOVSS DWORD PTR SS:[ESP],XMM0
	$+132    00C7FE1E     FF50 0C                CALL DWORD PTR DS:[EAX+0xC]
	$+135    00C7FE21     837F 2C 01             CMP DWORD PTR DS:[EDI+0x2C],0x1
	$+139    00C7FE25     75 1D                  JNZ SHORT PathOfEx.00C7FE44
	$+13B    00C7FE27     8D4F 1C                LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+13E    00C7FE2A     E8 211DDBFF            CALL PathOfEx.00A31B50
	$+143    00C7FE2F     0F57C9                 XORPS XMM1,XMM1
	$+146    00C7FE32     0F2FC8                 COMISS XMM1,XMM0
	$+149    00C7FE35     76 0D                  JBE SHORT PathOfEx.00C7FE44
	$+14B    00C7FE37     8B47 1C                MOV EAX,DWORD PTR DS:[EDI+0x1C]
	$+14E    00C7FE3A     8D4F 1C                LEA ECX,DWORD PTR DS:[EDI+0x1C]
	$+151    00C7FE3D     6A 00                  PUSH 0x0
	$+153    00C7FE3F     6A FF                  PUSH -0x1
	$+155    00C7FE41     FF50 04                CALL DWORD PTR DS:[EAX+0x4]
	$+158    00C7FE44     80BF A9000000 00       CMP BYTE PTR DS:[EDI+0xA9],0x0
	$+15F    00C7FE4B     74 23                  JE SHORT PathOfEx.00C7FE70
	$+161    00C7FE4D     C687 A9000000 00       MOV BYTE PTR DS:[EDI+0xA9],0x0
	$+168    00C7FE54     8B47 14                MOV EAX,DWORD PTR DS:[EDI+0x14]
	$+16B    00C7FE57     8B30                   MOV ESI,DWORD PTR DS:[EAX]
	$+16D    00C7FE59     3BF0                   CMP ESI,EAX
	$+16F    00C7FE5B     74 13                  JE SHORT PathOfEx.00C7FE70
	$+171    00C7FE5D     0F1F00                 NOP DWORD PTR DS:[EAX]
	$+174    00C7FE60     8B4E 08                MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+177    00C7FE63     8B36                   MOV ESI,DWORD PTR DS:[ESI]
	$+179    00C7FE65     57                     PUSH EDI
	$+17A    00C7FE66     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+17C    00C7FE68     FF50 08                CALL DWORD PTR DS:[EAX+0x8]
	$+17F    00C7FE6B     3B77 14                CMP ESI,DWORD PTR DS:[EDI+0x14]
	$+182    00C7FE6E   ^ 75 F0                  JNZ SHORT PathOfEx.00C7FE60
	$+184    00C7FE70     5F                     POP EDI
	$+185    00C7FE71     5E                     POP ESI
	$+186    00C7FE72     5D                     POP EBP
	$+187    00C7FE73     5B                     POP EBX
	$+188    00C7FE74     C2 0400                RETN 0x4
	$+18B    00C7FE77     CC                     INT3
	$+18C    00C7FE78     CC                     INT3
	$+18D    00C7FE79     CC                     INT3
	$+18E    00C7FE7A     CC                     INT3
	$+18F    00C7FE7B     CC                     INT3
	$+190    00C7FE7C     CC                     INT3
	$+191    00C7FE7D     CC                     INT3
	$+192    00C7FE7E     CC                     INT3
	$+193    00C7FE7F     CC                     INT3
	$+194    00C7FE80     8B4C24 08              MOV ECX,DWORD PTR SS:[ESP+0x8]
	$+198    00C7FE84     8B41 04                MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+19B    00C7FE87     F640 24 04             TEST BYTE PTR DS:[EAX+0x24],0x4
	$+19F    00C7FE8B     74 0B                  JE SHORT PathOfEx.00C7FE98
	$+1A1    00C7FE8D     F3:0F104C24 04         MOVSS XMM1,DWORD PTR SS:[ESP+0x4]
	$+1A7    00C7FE93     E9 18000000            JMP PathOfEx.00C7FEB0
	$+1AC    00C7FE98     C3                     RETN
	$+1AD    00C7FE99     CC                     INT3
	$+1AE    00C7FE9A     CC                     INT3
	$+1AF    00C7FE9B     CC                     INT3
	$+1B0    00C7FE9C     CC                     INT3
	$+1B1    00C7FE9D     CC                     INT3
	$+1B2    00C7FE9E     CC                     INT3
	$+1B3    00C7FE9F     CC                     INT3
	$+1B4    00C7FEA0     F3:0F104C24 04         MOVSS XMM1,DWORD PTR SS:[ESP+0x4]
	$+1BA    00C7FEA6     E8 05000000            CALL PathOfEx.00C7FEB0
	$+1BF    00C7FEAB     C2 0400                RETN 0x4
	$+1C2    00C7FEAE     CC                     INT3
	$+1C3    00C7FEAF     CC                     INT3
	$+1C4    00C7FEB0     51                     PUSH ECX
	$+1C5    00C7FEB1     56                     PUSH ESI
	$+1C6    00C7FEB2     57                     PUSH EDI
	$+1C7    00C7FEB3     8BF9                   MOV EDI,ECX
	$+1C9    00C7FEB5     0F28D1                 MOVAPS XMM2,XMM1
	$+1CC    00C7FEB8     33F6                   XOR ESI,ESI
	$+1CE    00C7FEBA     F3:0F115424 08         MOVSS DWORD PTR SS:[ESP+0x8],XMM2
	$+1D4    00C7FEC0     8B87 A0000000          MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+1DA    00C7FEC6     2B87 9C000000          SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+1E0    00C7FECC     C1F8 02                SAR EAX,0x2
	$+1E3    00C7FECF     85C0                   TEST EAX,EAX
	$+1E5    00C7FED1     74 55                  JE SHORT PathOfEx.00C7FF28
	$+1E7    00C7FED3     0F57C9                 XORPS XMM1,XMM1
	$+1EA    00C7FED6     66                     DATASIZE:
	$+1EB    00C7FED7     66:0F1F8400 00000000   NOP WORD PTR DS:[EAX+EAX]
	$+1F4    00C7FEE0     8B87 9C000000          MOV EAX,DWORD PTR DS:[EDI+0x9C]
	$+1FA    00C7FEE6     8B04B0                 MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+1FD    00C7FEE9     8B48 04                MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+200    00C7FEEC     F3:0F1041 0C           MOVSS XMM0,DWORD PTR DS:[ECX+0xC]
	$+205    00C7FEF1     0F2FC1                 COMISS XMM0,XMM1
	$+208    00C7FEF4     76 1E                  JBE SHORT PathOfEx.00C7FF14
	$+20A    00C7FEF6     8B89 E8000000          MOV ECX,DWORD PTR DS:[ECX+0xE8]
	$+210    00C7FEFC     85C9                   TEST ECX,ECX
	$+212    00C7FEFE     74 14                  JE SHORT PathOfEx.00C7FF14
	$+214    00C7FF00     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+216    00C7FF02     51                     PUSH ECX
	$+217    00C7FF03     F3:0F111424            MOVSS DWORD PTR SS:[ESP],XMM2
	$+21C    00C7FF08     FF50 10                CALL DWORD PTR DS:[EAX+0x10]
	$+21F    00C7FF0B     F3:0F105424 08         MOVSS XMM2,DWORD PTR SS:[ESP+0x8]
	$+225    00C7FF11     0F57C9                 XORPS XMM1,XMM1
	$+228    00C7FF14     8B87 A0000000          MOV EAX,DWORD PTR DS:[EDI+0xA0]
	$+22E    00C7FF1A     46                     INC ESI
	$+22F    00C7FF1B     2B87 9C000000          SUB EAX,DWORD PTR DS:[EDI+0x9C]
	$+235    00C7FF21     C1F8 02                SAR EAX,0x2
	$+238    00C7FF24     3BF0                   CMP ESI,EAX
	$+23A    00C7FF26   ^ 72 B8                  JB SHORT PathOfEx.00C7FEE0
	$+23C    00C7FF28     5F                     POP EDI
	$+23D    00C7FF29     5E                     POP ESI
	$+23E    00C7FF2A     59                     POP ECX
	$+23F    00C7FF2B     C3                     RETN
	$+240    00C7FF2C     CC                     INT3
	$+241    00C7FF2D     CC                     INT3
	$+242    00C7FF2E     CC                     INT3
	$+243    00C7FF2F     CC                     INT3
	$+244    00C7FF30     57                     PUSH EDI
	$+245    00C7FF31     8BF9                   MOV EDI,ECX
	$+247    00C7FF33     E8 C89FC9FF            CALL PathOfEx.00919F00
	$+24C    00C7FF38     3947 14                CMP DWORD PTR DS:[EDI+0x14],EAX
	$+24F    00C7FF3B     7D 41                  JGE SHORT PathOfEx.00C7FF7E
	$+251    00C7FF3D     8B57 0C                MOV EDX,DWORD PTR DS:[EDI+0xC]
	$+254    00C7FF40     56                     PUSH ESI
	$+255    00C7FF41     FF77 08                PUSH DWORD PTR DS:[EDI+0x8]
	$+258    00C7FF44     8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+25B    00C7FF47     8BB0 88000000          MOV ESI,DWORD PTR DS:[EAX+0x88]
	$+261    00C7FF4D     8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+264    00C7FF50     8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+267    00C7FF53     8B04B0                 MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+26A    00C7FF56     8B48 10                MOV ECX,DWORD PTR DS:[EAX+0x10]
	$+26D    00C7FF59     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+26F    00C7FF5B     FF10                   CALL DWORD PTR DS:[EAX]
	$+271    00C7FF5D     5E                     POP ESI
	$+272    00C7FF5E     85C0                   TEST EAX,EAX
	$+274    00C7FF60     7E 1C                  JLE SHORT PathOfEx.00C7FF7E
	$+276    00C7FF62     66:0F6EC8              MOVD MM1,EAX
	$+27A    00C7FF66     8BCF                   MOV ECX,EDI
	$+27C    00C7FF68     0F5BC9                 CVTDQ2PS XMM1,XMM1
	$+27F    00C7FF6B     F3:0F5E0D 887B5B01     DIVSS XMM1,DWORD PTR DS:[0x15B7B88]
	$+287    00C7FF73     F3:0F594C24 08         MULSS XMM1,DWORD PTR SS:[ESP+0x8]
	$+28D    00C7FF79     E8 12000000            CALL PathOfEx.00C7FF90
	$+292    00C7FF7E     5F                     POP EDI
	$+293    00C7FF7F     C2 0400                RETN 0x4
	$+296    00C7FF82     CC                     INT3
	$+297    00C7FF83     CC                     INT3
	$+298    00C7FF84     CC                     INT3
	$+299    00C7FF85     CC                     INT3
	$+29A    00C7FF86     CC                     INT3
	$+29B    00C7FF87     CC                     INT3
	$+29C    00C7FF88     CC                     INT3
	$+29D    00C7FF89     CC                     INT3
	$+29E    00C7FF8A     CC                     INT3
	$+29F    00C7FF8B     CC                     INT3
	$+2A0    00C7FF8C     CC                     INT3
	$+2A1    00C7FF8D     CC                     INT3
	$+2A2    00C7FF8E     CC                     INT3
	$+2A3    00C7FF8F     CC                     INT3
	$+2A4    00C7FF90     56                     PUSH ESI
	$+2A5    00C7FF91     8BF1                   MOV ESI,ECX
	$+2A7    00C7FF93     57                     PUSH EDI
	$+2A8    00C7FF94     F3:0F584E 18           ADDSS XMM1,DWORD PTR DS:[ESI+0x18]
	$+2AD    00C7FF99     F3:0F2CF9              CVTTSS2SI EDI,XMM1
	$+2B1    00C7FF9D     017E 14                ADD DWORD PTR DS:[ESI+0x14],EDI
	$+2B4    00C7FFA0     66:0F6EC7              MOVD MM0,EDI
	$+2B8    00C7FFA4     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$+2BB    00C7FFA7     F3:0F5CC8              SUBSS XMM1,XMM0
	$+2BF    00C7FFAB     F3:0F114E 18           MOVSS DWORD PTR DS:[ESI+0x18],XMM1
	$+2C4    00C7FFB0     E8 4B9FC9FF            CALL PathOfEx.00919F00
	$+2C9    00C7FFB5     3946 14                CMP DWORD PTR DS:[ESI+0x14],EAX
	$+2CC    00C7FFB8     7C 15                  JL SHORT PathOfEx.00C7FFCF
	$+2CE    00C7FFBA     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+2D1    00C7FFBD     FF76 08                PUSH DWORD PTR DS:[ESI+0x8]
	$+2D4    00C7FFC0     8946 14                MOV DWORD PTR DS:[ESI+0x14],EAX
	$+2D7    00C7FFC3     C746 18 00000000       MOV DWORD PTR DS:[ESI+0x18],0x0
	$+2DE    00C7FFCA     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+2E0    00C7FFCC     FF50 54                CALL DWORD PTR DS:[EAX+0x54]
	$+2E3    00C7FFCF     85FF                   TEST EDI,EDI
	$+2E5    00C7FFD1     5F                     POP EDI
	$+2E6    00C7FFD2     74 09                  JE SHORT PathOfEx.00C7FFDD
	$+2E8    00C7FFD4     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+2EB    00C7FFD7     5E                     POP ESI
	$+2EC    00C7FFD8     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+2EE    00C7FFDA     FF60 48                JMP DWORD PTR DS:[EAX+0x48]
	$+2F1    00C7FFDD     5E                     POP ESI
	$+2F2    00C7FFDE     C3                     RETN
	$+2F3    00C7FFDF     CC                     INT3
	$+2F4    00C7FFE0     56                     PUSH ESI
	$+2F5    00C7FFE1     8BF1                   MOV ESI,ECX
	$+2F7    00C7FFE3     0F57D2                 XORPS XMM2,XMM2
	$+2FA    00C7FFE6     837E 14 00             CMP DWORD PTR DS:[ESI+0x14],0x0
	$+2FE    00C7FFEA     7F 06                  JG SHORT PathOfEx.00C7FFF2
	$+300    00C7FFEC     0F2F56 18              COMISS XMM2,DWORD PTR DS:[ESI+0x18]
	$+304    00C7FFF0     73 6E                  JNB SHORT PathOfEx.00C80060
	$+306    00C7FFF2     F3:0F104C24 08         MOVSS XMM1,DWORD PTR SS:[ESP+0x8]
	$+30C    00C7FFF8     F3:0F584E 18           ADDSS XMM1,DWORD PTR DS:[ESI+0x18]
	$+311    00C7FFFD     57                     PUSH EDI
	$+312    00C7FFFE     C74424 0C 00000000     MOV DWORD PTR SS:[ESP+0xC],0x0
	$+31A    00C80006     F3:0F2CF9              CVTTSS2SI EDI,XMM1
	$+31E    00C8000A     017E 14                ADD DWORD PTR DS:[ESI+0x14],EDI
	$+321    00C8000D     8B46 14                MOV EAX,DWORD PTR DS:[ESI+0x14]
	$+324    00C80010     66:0F6EC7              MOVD MM0,EDI
	$+328    00C80014     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$+32B    00C80017     F3:0F5CC8              SUBSS XMM1,XMM0
	$+32F    00C8001B     F3:0F114E 18           MOVSS DWORD PTR DS:[ESI+0x18],XMM1
	$+334    00C80020     85C0                   TEST EAX,EAX
	$+336    00C80022     7F 2F                  JG SHORT PathOfEx.00C80053
	$+338    00C80024     0F2FD1                 COMISS XMM2,XMM1
	$+33B    00C80027     72 2A                  JB SHORT PathOfEx.00C80053
	$+33D    00C80029     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+340    00C8002C     FF76 08                PUSH DWORD PTR DS:[ESI+0x8]
	$+343    00C8002F     66:0F6EC0              MOVD MM0,EAX
	$+347    00C80033     0F5BC0                 CVTDQ2PS XMM0,XMM0
	$+34A    00C80036     C746 14 00000000       MOV DWORD PTR DS:[ESI+0x14],0x0
	$+351    00C8003D     C746 18 00000000       MOV DWORD PTR DS:[ESI+0x18],0x0
	$+358    00C80044     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+35A    00C80046     F3:0F58C1              ADDSS XMM0,XMM1
	$+35E    00C8004A     F3:0F114424 10         MOVSS DWORD PTR SS:[ESP+0x10],XMM0
	$+364    00C80050     FF50 5C                CALL DWORD PTR DS:[EAX+0x5C]
	$+367    00C80053     85FF                   TEST EDI,EDI
	$+369    00C80055     5F                     POP EDI
	$+36A    00C80056     74 08                  JE SHORT PathOfEx.00C80060
	$+36C    00C80058     8B4E 0C                MOV ECX,DWORD PTR DS:[ESI+0xC]
	$+36F    00C8005B     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+371    00C8005D     FF50 48                CALL DWORD PTR DS:[EAX+0x48]
	$+374    00C80060     D94424 08              FLD DWORD PTR SS:[ESP+0x8]
	$+378    00C80064     5E                     POP ESI
	$+379    00C80065     C2 0800                RETN 0x8
	$+37C    00C80068     CC                     INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("2b??????????8b??c1????85??0f??????????f3??????????????66????????????????8b", 2);
#else
	//op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8B47048B40048B04888B48108B01", -0x55 + 2);
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("2b??????????c1????85??0f??????????530f????558b", 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x50 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_life_get_buff_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-12B    00523EAF     CC                     INT3
	$-12A    00523EB0     55                     PUSH EBP
	$-129    00523EB1     8BEC                   MOV EBP, ESP
	$-127    00523EB3     64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$-121    00523EB9     8B55 08                MOV EDX, DWORD PTR SS:[EBP+0x8]
	$-11E    00523EBC     6A FF                  PUSH -0x1
	$-11C    00523EBE     68 D071D200            PUSH PathOfEx.00D271D0
	$-117    00523EC3     50                     PUSH EAX
	$-116    00523EC4     64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$-10F    00523ECB     83EC 10                SUB ESP, 0x10
	$-10C    00523ECE     807A 04 00             CMP BYTE PTR DS:[EDX+0x4], 0x0
	$-108    00523ED2     8B02                   MOV EAX, DWORD PTR DS:[EDX]
	$-106    00523ED4     56                     PUSH ESI
	$-105    00523ED5     57                     PUSH EDI
	$-104    00523ED6     8B80 B0080000          MOV EAX, DWORD PTR DS:[EAX+0x8B0]
	$-FE     00523EDC     8B40 08                MOV EAX, DWORD PTR DS:[EAX+0x8]
	$-FB     00523EDF     8B78 20                MOV EDI, DWORD PTR DS:[EAX+0x20]
	$-F8     00523EE2     8B01                   MOV EAX, DWORD PTR DS:[ECX]
	$-F6     00523EE4     8BB0 38090000          MOV ESI, DWORD PTR DS:[EAX+0x938]
	$-F0     00523EEA     74 19                  JE SHORT PathOfEx.00523F05
	$-EE     00523EEC     B8 20000000            MOV EAX, 0x20
	$-E9     00523EF1     C745 E8 6879EF00       MOV DWORD PTR SS:[EBP-0x18], PathOfEx.00>
	$-E2     00523EF8     66:8945 EC             MOV WORD PTR SS:[EBP-0x14], AX
	$-DE     00523EFC     C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-D7     00523F03     EB 17                  JMP SHORT PathOfEx.00523F1C
	$-D5     00523F05     B8 21000000            MOV EAX, 0x21
	$-D0     00523F0A     C745 E8 6879EF00       MOV DWORD PTR SS:[EBP-0x18], PathOfEx.00>
	$-C9     00523F11     66:8945 EC             MOV WORD PTR SS:[EBP-0x14], AX
	$-C5     00523F15     C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-BE     00523F1C     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-BB     00523F1F     897D F0                MOV DWORD PTR SS:[EBP-0x10], EDI
	$-B8     00523F22     50                     PUSH EAX
	$-B7     00523F23     FFB6 983A0000          PUSH DWORD PTR DS:[ESI+0x3A98]
	$-B1     00523F29     E8 924B1900            CALL PathOfEx.006B8AC0
	$-AC     00523F2E     8BC8                   MOV ECX, EAX
	$-AA     00523F30     E8 8B493C00            CALL PathOfEx.008E88C0
	$-A5     00523F35     8B8E 983A0000          MOV ECX, DWORD PTR DS:[ESI+0x3A98]
	$-9F     00523F3B     6A 00                  PUSH 0x0
	$-9D     00523F3D     E8 1EF92600            CALL PathOfEx.00793860
	$-98     00523F42     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-95     00523F45     5F                     POP EDI                                  ; 0018EC48
	$-94     00523F46     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$-8D     00523F4D     5E                     POP ESI                                  ; 0018EC48
	$-8C     00523F4E     8BE5                   MOV ESP, EBP
	$-8A     00523F50     5D                     POP EBP                                  ; 0018EC48
	$-89     00523F51     C2 0400                RETN 0x4
	$-86     00523F54     CC                     INT3
	$-85     00523F55     CC                     INT3
	$-84     00523F56     CC                     INT3
	$-83     00523F57     CC                     INT3
	$-82     00523F58     CC                     INT3
	$-81     00523F59     CC                     INT3
	$-80     00523F5A     CC                     INT3
	$-7F     00523F5B     CC                     INT3
	$-7E     00523F5C     CC                     INT3
	$-7D     00523F5D     CC                     INT3
	$-7C     00523F5E     CC                     INT3
	$-7B     00523F5F     CC                     INT3
	$-7A     00523F60     55                     PUSH EBP
	$-79     00523F61     8BEC                   MOV EBP, ESP
	$-77     00523F63     6A FF                  PUSH -0x1
	$-75     00523F65     68 9A92D200            PUSH PathOfEx.00D2929A
	$-70     00523F6A     64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$-6A     00523F70     50                     PUSH EAX
	$-69     00523F71     64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$-62     00523F78     83EC 0C                SUB ESP, 0xC
	$-5F     00523F7B     56                     PUSH ESI
	$-5E     00523F7C     57                     PUSH EDI
	$-5D     00523F7D     8BF9                   MOV EDI, ECX
	$-5B     00523F7F     8B87 68090000          MOV EAX, DWORD PTR DS:[EDI+0x968]
	$-55     00523F85     8987 6C090000          MOV DWORD PTR DS:[EDI+0x96C], EAX
	$-4F     00523F8B     8D8F 78090000          LEA ECX, DWORD PTR DS:[EDI+0x978]
	$-49     00523F91     C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4], 0x0          ; 是否进行后悔点操作
	$-42     00523F98     E8 2343F9FF            CALL PathOfEx.004B82C0
	$-3D     00523F9D     6A 08                  PUSH 0x8
	$-3B     00523F9F     8D8F 74090000          LEA ECX, DWORD PTR DS:[EDI+0x974]
	$-35     00523FA5     E8 26B7F4FF            CALL PathOfEx.0046F6D0
	$-30     00523FAA     8D8F 98090000          LEA ECX, DWORD PTR DS:[EDI+0x998]
	$-2A     00523FB0     C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-23     00523FB7     E8 B4490000            CALL PathOfEx.00528970
	$-1E     00523FBC     6A 08                  PUSH 0x8
	$-1C     00523FBE     8D8F 94090000          LEA ECX, DWORD PTR DS:[EDI+0x994]
	$-16     00523FC4     E8 07B7F4FF            CALL PathOfEx.0046F6D0
	$-11     00523FC9     C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-A      00523FD0     8B87 38090000          MOV EAX, DWORD PTR DS:[EDI+0x938]
	$-4      00523FD6     85C0                   TEST EAX, EAX
	$-2      00523FD8     74 5E                  JE SHORT PathOfEx.00524038
	$ ==>    00523FDA     C745 EC 00000000       MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+7      00523FE1     C745 F0 00000000       MOV DWORD PTR SS:[EBP-0x10], 0x0
	$+E      00523FE8     8D4D EC                LEA ECX, DWORD PTR SS:[EBP-0x14]
	$+11     00523FEB     C745 FC 02000000       MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+18     00523FF2     51                     PUSH ECX
	$+19     00523FF3     05 C43A0000            ADD EAX, 0x3AC4                          ; PassiveSkill的偏移
	$+1E     00523FF8     8BCF                   MOV ECX, EDI
	$+20     00523FFA     50                     PUSH EAX                                 ; ui天赋遍历
	$+21     00523FFB     E8 10050000            CALL PathOfEx.00524510
	$+26     00524000     C745 FC 03000000       MOV DWORD PTR SS:[EBP-0x4], 0x3
	$+2D     00524007     8B45 EC                MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+30     0052400A     85C0                   TEST EAX, EAX
	$+32     0052400C     74 18                  JE SHORT PathOfEx.00524026
	$+34     0052400E     50                     PUSH EAX
	$+35     0052400F     51                     PUSH ECX
	$+36     00524010     E8 BB241F00            CALL PathOfEx.007164D0
	$+3B     00524015     83C4 04                ADD ESP, 0x4
	$+3E     00524018     8BC8                   MOV ECX, EAX
	$+40     0052401A     E8 41201F00            CALL PathOfEx.00716060
	$+45     0052401F     C745 EC 00000000       MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+4C     00524026     8D8F B8080000          LEA ECX, DWORD PTR DS:[EDI+0x8B8]
	$+52     0052402C     C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+59     00524033     E8 78F5F8FF            CALL PathOfEx.004B35B0
	$+5E     00524038     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+61     0052403B     5F                     POP EDI                                  ; PathOf_1.51666E07
	$+62     0052403C     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+69     00524043     5E                     POP ESI                                  ; PathOf_1.51666E07
	$+6A     00524044     8BE5                   MOV ESP, EBP
	$+6C     00524046     5D                     POP EBP                                  ; PathOf_1.51666E07
	$+6D     00524047     C3                     RETN
	$+6E     00524048     CC                     INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("C745EC00000000C745F0000000008D4DECC745FC??000000", 0x19 + 1);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x3000 && dw_tmp < 0x5000);
	ofs << "pt_cdword	g_o_get_passive_skill_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-144    0080847F     CC                     INT3
	$-143    00808480     8B4424 04              MOV EAX, DWORD PTR SS:[ESP+0x4]
	$-13F    00808484     56                     PUSH ESI
	$-13E    00808485     57                     PUSH EDI
	$-13D    00808486     8BF9                   MOV EDI, ECX
	$-13B    00808488     2947 48                SUB DWORD PTR DS:[EDI+0x48], EAX
	$-138    0080848B     8B77 64                MOV ESI, DWORD PTR DS:[EDI+0x64]
	$-135    0080848E     3B77 68                CMP ESI, DWORD PTR DS:[EDI+0x68]
	$-132    00808491     74 10                  JE SHORT PathOfEx.008084A3
	$-130    00808493     8B0E                   MOV ECX, DWORD PTR DS:[ESI]
	$-12E    00808495     57                     PUSH EDI
	$-12D    00808496     8B01                   MOV EAX, DWORD PTR DS:[ECX]
	$-12B    00808498     FF50 10                CALL NEAR DWORD PTR DS:[EAX+0x10]
	$-128    0080849B     83C6 04                ADD ESI, 0x4
	$-125    0080849E     3B77 68                CMP ESI, DWORD PTR DS:[EDI+0x68]
	$-122    008084A1   ^ 75 F0                  JNZ SHORT PathOfEx.00808493
	$-120    008084A3     5F                     POP EDI                                  ; 0018EC48
	$-11F    008084A4     5E                     POP ESI                                  ; 0018EC48
	$-11E    008084A5     C2 0400                RETN 0x4
	$-11B    008084A8     CC                     INT3
	$-11A    008084A9     CC                     INT3
	$-119    008084AA     CC                     INT3
	$-118    008084AB     CC                     INT3
	$-117    008084AC     CC                     INT3
	$-116    008084AD     CC                     INT3
	$-115    008084AE     CC                     INT3
	$-114    008084AF     CC                     INT3
	$-113    008084B0     55                     PUSH EBP                                 ; tag_后悔点数运算
	$-112    008084B1     8BEC                   MOV EBP, ESP
	$-110    008084B3     6A FF                  PUSH -0x1
	$-10E    008084B5     68 AD7CD500            PUSH PathOfEx.00D57CAD                   ; ASCII "给澍"
	$-109    008084BA     64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$-103    008084C0     50                     PUSH EAX
	$-102    008084C1     64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$-FB     008084C8     83EC 14                SUB ESP, 0x14
	$-F8     008084CB     56                     PUSH ESI
	$-F7     008084CC     8BF1                   MOV ESI, ECX
	$-F5     008084CE     C745 E8 00000000       MOV DWORD PTR SS:[EBP-0x18], 0x0
	$-EE     008084D5     57                     PUSH EDI
	$-ED     008084D6     8D4D E0                LEA ECX, DWORD PTR SS:[EBP-0x20]
	$-EA     008084D9     8D56 04                LEA EDX, DWORD PTR DS:[ESI+0x4]
	$-E7     008084DC     E8 CF520000            CALL PathOfEx.0080D7B0
	$-E2     008084E1     C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-DB     008084E8     8D46 08                LEA EAX, DWORD PTR DS:[ESI+0x8]
	$-D8     008084EB     8B4D E0                MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-D5     008084EE     50                     PUSH EAX
	$-D4     008084EF     8D49 08                LEA ECX, DWORD PTR DS:[ECX+0x8]
	$-D1     008084F2     E8 09500000            CALL PathOfEx.0080D500
	$-CC     008084F7     8B4D E0                MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-C9     008084FA     8D46 14                LEA EAX, DWORD PTR DS:[ESI+0x14]
	$-C6     008084FD     50                     PUSH EAX
	$-C5     008084FE     8D49 14                LEA ECX, DWORD PTR DS:[ECX+0x14]
	$-C2     00808501     E8 FA4F0000            CALL PathOfEx.0080D500
	$-BD     00808506     8B55 E0                MOV EDX, DWORD PTR SS:[EBP-0x20]
	$-BA     00808509     8D46 20                LEA EAX, DWORD PTR DS:[ESI+0x20]
	$-B7     0080850C     8D4A 20                LEA ECX, DWORD PTR DS:[EDX+0x20]
	$-B4     0080850F     3BC8                   CMP ECX, EAX
	$-B2     00808511     74 09                  JE SHORT PathOfEx.0080851C
	$-B0     00808513     50                     PUSH EAX
	$-AF     00808514     E8 A7510000            CALL PathOfEx.0080D6C0
	$-AA     00808519     8B55 E0                MOV EDX, DWORD PTR SS:[EBP-0x20]
	$-A7     0080851C     8A46 40                MOV AL, BYTE PTR DS:[ESI+0x40]
	$-A4     0080851F     8842 40                MOV BYTE PTR DS:[EDX+0x40], AL
	$-A1     00808522     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-9E     00808525     8B4E 44                MOV ECX, DWORD PTR DS:[ESI+0x44]
	$-9B     00808528     8948 44                MOV DWORD PTR DS:[EAX+0x44], ECX
	$-98     0080852B     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-95     0080852E     8B4E 48                MOV ECX, DWORD PTR DS:[ESI+0x48]
	$-92     00808531     8948 48                MOV DWORD PTR DS:[EAX+0x48], ECX         ; 后悔点数偏移
	$-8F     00808534     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-8C     00808537     8B4E 4C                MOV ECX, DWORD PTR DS:[ESI+0x4C]
	$-89     0080853A     8948 4C                MOV DWORD PTR DS:[EAX+0x4C], ECX
	$-86     0080853D     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-83     00808540     8B4E 50                MOV ECX, DWORD PTR DS:[ESI+0x50]
	$-80     00808543     8948 50                MOV DWORD PTR DS:[EAX+0x50], ECX
	$-7D     00808546     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-7A     00808549     8B4E 54                MOV ECX, DWORD PTR DS:[ESI+0x54]
	$-77     0080854C     8948 54                MOV DWORD PTR DS:[EAX+0x54], ECX
	$-74     0080854F     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-71     00808552     8B4E 58                MOV ECX, DWORD PTR DS:[ESI+0x58]
	$-6E     00808555     8948 58                MOV DWORD PTR DS:[EAX+0x58], ECX
	$-6B     00808558     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-68     0080855B     8B4E 5C                MOV ECX, DWORD PTR DS:[ESI+0x5C]
	$-65     0080855E     8948 5C                MOV DWORD PTR DS:[EAX+0x5C], ECX
	$-62     00808561     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-5F     00808564     8B4E 60                MOV ECX, DWORD PTR DS:[ESI+0x60]
	$-5C     00808567     8948 60                MOV DWORD PTR DS:[EAX+0x60], ECX
	$-59     0080856A     C645 FC 02             MOV BYTE PTR SS:[EBP-0x4], 0x2
	$-55     0080856E     8B7D 08                MOV EDI, DWORD PTR SS:[EBP+0x8]
	$-52     00808571     8B75 E4                MOV ESI, DWORD PTR SS:[EBP-0x1C]         ; PathOf_1.51748FF6
	$-4F     00808574     8B45 E0                MOV EAX, DWORD PTR SS:[EBP-0x20]
	$-4C     00808577     8975 F0                MOV DWORD PTR SS:[EBP-0x10], ESI
	$-49     0080857A     C707 00000000          MOV DWORD PTR DS:[EDI], 0x0
	$-43     00808580     C747 04 00000000       MOV DWORD PTR DS:[EDI+0x4], 0x0
	$-3C     00808587     8945 EC                MOV DWORD PTR SS:[EBP-0x14], EAX
	$-39     0080858A     85F6                   TEST ESI, ESI
	$-37     0080858C     74 07                  JE SHORT PathOfEx.00808595
	$-35     0080858E     F0:FF46 04             LOCK INC DWORD PTR DS:[ESI+0x4]
	$-31     00808592     8B75 E4                MOV ESI, DWORD PTR SS:[EBP-0x1C]         ; PathOf_1.51748FF6
	$-2E     00808595     53                     PUSH EBX
	$-2D     00808596     8B5F 04                MOV EBX, DWORD PTR DS:[EDI+0x4]
	$-2A     00808599     83C8 FF                OR EAX, -0x1
	$-27     0080859C     85DB                   TEST EBX, EBX
	$-25     0080859E     74 26                  JE SHORT PathOfEx.008085C6
	$-23     008085A0     F0:0FC143 04           LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$-1E     008085A5     75 19                  JNZ SHORT PathOfEx.008085C0
	$-1C     008085A7     8B03                   MOV EAX, DWORD PTR DS:[EBX]
	$-1A     008085A9     8BCB                   MOV ECX, EBX
	$-18     008085AB     FF10                   CALL NEAR DWORD PTR DS:[EAX]
	$-16     008085AD     83C9 FF                OR ECX, -0x1
	$-13     008085B0     8BC1                   MOV EAX, ECX
	$-11     008085B2     F0:0FC143 08           LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$-C      008085B7     75 07                  JNZ SHORT PathOfEx.008085C0
	$-A      008085B9     8B03                   MOV EAX, DWORD PTR DS:[EBX]
	$-8      008085BB     8BCB                   MOV ECX, EBX
	$-6      008085BD     FF50 04                CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-3      008085C0     8B75 E4                MOV ESI, DWORD PTR SS:[EBP-0x1C]         ; PathOf_1.51748FF6
	$ ==>    008085C3     83C8 FF                OR EAX, -0x1
	$+3      008085C6     8B4D F0                MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+6      008085C9     894F 04                MOV DWORD PTR DS:[EDI+0x4], ECX
	$+9      008085CC     8B4D EC                MOV ECX, DWORD PTR SS:[EBP-0x14]         ; PathOf_1.517F140C
	$+C      008085CF     890F                   MOV DWORD PTR DS:[EDI], ECX
	$+E      008085D1     C745 E8 01000000       MOV DWORD PTR SS:[EBP-0x18], 0x1
	$+15     008085D8     C745 FC 03000000       MOV DWORD PTR SS:[EBP-0x4], 0x3
	$+1C     008085DF     5B                     POP EBX                                  ; PathOf_1.51666E07
	$+1D     008085E0     85F6                   TEST ESI, ESI
	$+1F     008085E2     74 1F                  JE SHORT PathOfEx.00808603
	$+21     008085E4     F0:0FC146 04           LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+26     008085E9     75 18                  JNZ SHORT PathOfEx.00808603
	$+28     008085EB     8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$+2A     008085ED     8BCE                   MOV ECX, ESI
	$+2C     008085EF     FF10                   CALL NEAR DWORD PTR DS:[EAX]
	$+2E     008085F1     83C9 FF                OR ECX, -0x1
	$+31     008085F4     F0:0FC14E 08           LOCK XADD DWORD PTR DS:[ESI+0x8], ECX
	$+36     008085F9     49                     DEC ECX
	$+37     008085FA     75 07                  JNZ SHORT PathOfEx.00808603
	$+39     008085FC     8B16                   MOV EDX, DWORD PTR DS:[ESI]
	$+3B     008085FE     8BCE                   MOV ECX, ESI
	$+3D     00808600     FF52 04                CALL NEAR DWORD PTR DS:[EDX+0x4]
	$+40     00808603     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+43     00808606     8BC7                   MOV EAX, EDI
	$+45     00808608     5F                     POP EDI                                  ; PathOf_1.51666E07
	$+46     00808609     5E                     POP ESI                                  ; PathOf_1.51666E07
	$+47     0080860A     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+4E     00808611     8BE5                   MOV ESP, EBP
	$+50     00808613     5D                     POP EBP                                  ; PathOf_1.51666E07
	$+51     00808614     C2 0400                RETN 0x4
	$+54     00808617     CC                     INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_byte>("83C8FF8B4DF0894F048B4DEC890FC745E801000000C745FC03000000", -0x92 + 2);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x20 && dw_tmp < 0x100);
	ofs << "pt_cdword	g_o_regret_skill_point_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-A7     0076326F      CC                     INT3
	$-A6     00763270      83EC 0C                SUB ESP, 0xC								; tag_函数内部有TblItem大小的常量
	$-A3     00763273      53                     PUSH EBX
	$-A2     00763274      56                     PUSH ESI
	$-A1     00763275      57                     PUSH EDI
	$-A0     00763276      8BFA                   MOV EDI, EDX
	$-9E     00763278      C74424 14 00000000     MOV DWORD PTR SS:[ESP+0x14], 0x0
	$-96     00763280      8BF1                   MOV ESI, ECX
	$-94     00763282      897424 10              MOV DWORD PTR SS:[ESP+0x10], ESI
	$-90     00763286      8B0F                   MOV ECX, DWORD PTR DS:[EDI]
	$-8E     00763288      85C9                   TEST ECX, ECX
	$-8C     0076328A      74 05                  JE SHORT PathOfEx.00763291
	$-8A     0076328C      E8 0F48FBFF            CALL PathOfEx.00717AA0
	$-85     00763291      8B0F                   MOV ECX, DWORD PTR DS:[EDI]
	$-83     00763293      8B5C24 1C              MOV EBX, DWORD PTR SS:[ESP+0x1C]
	$-7F     00763297      8B41 30                MOV EAX, DWORD PTR DS:[ECX+0x30]             ; PathOfEx.00E7EF58
	$-7C     0076329A      3B58 14                CMP EBX, DWORD PTR DS:[EAX+0x14]
	$-79     0076329D      72 16                  JB SHORT PathOfEx.007632B5
	$-77     0076329F      C706 00000000          MOV DWORD PTR DS:[ESI], 0x0
	$-71     007632A5      8BC6                   MOV EAX, ESI
	$-6F     007632A7      C746 04 00000000       MOV DWORD PTR DS:[ESI+0x4], 0x0
	$-68     007632AE      5F                     POP EDI
	$-67     007632AF      5E                     POP ESI
	$-66     007632B0      5B                     POP EBX
	$-65     007632B1      83C4 0C                ADD ESP, 0xC
	$-62     007632B4      C3                     RETN
	$-61     007632B5      85C9                   TEST ECX, ECX
	$-5F     007632B7      74 05                  JE SHORT PathOfEx.007632BE
	$-5D     007632B9      E8 E247FBFF            CALL PathOfEx.00717AA0
	$-58     007632BE      8B17                   MOV EDX, DWORD PTR DS:[EDI]
	$-56     007632C0      6BFB 73                IMUL EDI, EBX, 0x73                          ; stCD_TblItemSkill的大小
	$-53     007632C3      8B42 30                MOV EAX, DWORD PTR DS:[EDX+0x30]
	$-50     007632C6      0378 04                ADD EDI, DWORD PTR DS:[EAX+0x4]
	$-4D     007632C9      8916                   MOV DWORD PTR DS:[ESI], EDX
	$-4B     007632CB      85D2                   TEST EDX, EDX
	$-49     007632CD      74 14                  JE SHORT PathOfEx.007632E3
	$-47     007632CF      8D4A 38                LEA ECX, DWORD PTR DS:[EDX+0x38]
	$-44     007632D2      B8 01000000            MOV EAX, 0x1
	$-3F     007632D7      F0:0FC101              LOCK XADD DWORD PTR DS:[ECX], EAX
	$-3B     007632DB      A1 0C47FD00            MOV EAX, DWORD PTR DS:[0xFD470C]
	$-36     007632E0      8942 34                MOV DWORD PTR DS:[EDX+0x34], EAX
	$-33     007632E3      897E 04                MOV DWORD PTR DS:[ESI+0x4], EDI
	$-30     007632E6      8BC6                   MOV EAX, ESI
	$-2E     007632E8      5F                     POP EDI
	$-2D     007632E9      5E                     POP ESI
	$-2C     007632EA      5B                     POP EBX
	$-2B     007632EB      83C4 0C                ADD ESP, 0xC
	$-28     007632EE      C3                     RETN
	$-27     007632EF      CC                     INT3
	$-26     007632F0      55                     PUSH EBP
	$-25     007632F1      8BEC                   MOV EBP, ESP
	$-23     007632F3      6A FF                  PUSH -0x1
	$-21     007632F5      68 6AC8D400            PUSH PathOfEx.00D4C86A
	$-1C     007632FA      64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$-16     00763300      50                     PUSH EAX
	$-15     00763301      64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$-E      00763308      81EC 18030000          SUB ESP, 0x318
	$-8      0076330E      53                     PUSH EBX
	$-7      0076330F      8BD9                   MOV EBX, ECX
	$-5      00763311      895D F0                MOV DWORD PTR SS:[EBP-0x10], EBX
	$-2      00763314      56                     PUSH ESI
	$-1      00763315      57                     PUSH EDI
	$ ==>    00763316      C703 1ED70001          MOV DWORD PTR DS:[EBX], PathOfEx.0100D71E
	$+6      0076331C      C743 04 00000000       MOV DWORD PTR DS:[EBX+0x4], 0x0
	$+D      00763323      C743 08 00000000       MOV DWORD PTR DS:[EBX+0x8], 0x0
	$+14     0076332A      C743 0C 00000000       MOV DWORD PTR DS:[EBX+0xC], 0x0
	$+1B     00763331      8D43 18                LEA EAX, DWORD PTR DS:[EBX+0x18]
	$+1E     00763334      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+25     0076333B      8BC8                   MOV ECX, EAX
	$+27     0076333D      C743 10 00000000       MOV DWORD PTR DS:[EBX+0x10], 0x0
	$+2E     00763344      C743 14 00000000       MOV DWORD PTR DS:[EBX+0x14], 0x0
	$+35     0076334B      8945 E0                MOV DWORD PTR SS:[EBP-0x20], EAX
	$+38     0076334E      E8 DD46D5FF            CALL PathOfEx.004B7A30
	$+3D     00763353      8D43 38                LEA EAX, DWORD PTR DS:[EBX+0x38]
	$+40     00763356      C645 FC 01             MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+44     0076335A      8BC8                   MOV ECX, EAX
	$+46     0076335C      8945 E4                MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+49     0076335F      E8 3C46D5FF            CALL PathOfEx.004B79A0
	$+4E     00763364      8B75 08                MOV ESI, DWORD PTR SS:[EBP+0x8]
	$+51     00763367      8D4D 80                LEA ECX, DWORD PTR SS:[EBP-0x80]
	$+54     0076336A      8BD6                   MOV EDX, ESI
	$+56     0076336C      C645 FC 02             MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+5A     00763370      E8 BB7CFCFF            CALL PathOfEx.0072B030
	$+5F     00763375      51                     PUSH ECX
	$+60     00763376      6A 00                  PUSH 0x0
	$+62     00763378      6A 00                  PUSH 0x0
	$+64     0076337A      C645 FC 03             MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+68     0076337E      8D45 80                LEA EAX, DWORD PTR SS:[EBP-0x80]
	$+6B     00763381      FF35 F86BFF00          PUSH DWORD PTR DS:[0xFF6BF8]
	$+71     00763387      8D8D C0FEFFFF          LEA ECX, DWORD PTR SS:[EBP-0x140]
	$+77     0076338D      50                     PUSH EAX
	$+78     0076338E      E8 4D2F0500            CALL PathOfEx.007B62E0
	$+7D     00763393      C645 FC 04             MOV BYTE PTR SS:[EBP-0x4], 0x4
	$+81     00763397      8B95 04FFFFFF          MOV EDX, DWORD PTR SS:[EBP-0xFC]
	$+87     0076339D      8B8D FCFEFFFF          MOV ECX, DWORD PTR SS:[EBP-0x104]
	$+8D     007633A3      2BD1                   SUB EDX, ECX
	$+8F     007633A5      83FA 04                CMP EDX, 0x4
	$+92     007633A8      73 49                  JNB SHORT PathOfEx.007633F3
	$+94     007633AA      56                     PUSH ESI
	$+95     007633AB      8D8D E0FCFFFF          LEA ECX, DWORD PTR SS:[EBP-0x320]
	$+9B     007633B1      E8 1ACAF2FF            CALL PathOfEx.0068FDD0
	$+A0     007633B6      8BF0                   MOV ESI, EAX
	$+A2     007633B8      C645 FC 05             MOV BYTE PTR SS:[EBP-0x4], 0x5
	$+A6     007633BC      68 98D1EA00            PUSH PathOfEx.00EAD198                       ; Data file too small
	$+AB     007633C1      8D56 40                LEA EDX, DWORD PTR DS:[ESI+0x40]
	$+AE     007633C4      F7DA                   NEG EDX
	$+B0     007633C6      8D4E 50                LEA ECX, DWORD PTR DS:[ESI+0x50]
	$+B3     007633C9      1BD2                   SBB EDX, EDX
	$+B5     007633CB      23D1                   AND EDX, ECX
	$+B7     007633CD      52                     PUSH EDX
	$+B8     007633CE      E8 ED4AD2FF            CALL PathOfEx.00487EC0
	$+BD     007633D3      83C4 08                ADD ESP, 0x8
	$+C0     007633D6      8D8D D0FDFFFF          LEA ECX, DWORD PTR SS:[EBP-0x230]
	$+C6     007633DC      56                     PUSH ESI
	$+C7     007633DD      E8 EECAF2FF            CALL PathOfEx.0068FED0
	$+CC     007633E2      68 14CBFC00            PUSH PathOfEx.00FCCB14
	$+D1     007633E7      8D85 D0FDFFFF          LEA EAX, DWORD PTR SS:[EBP-0x230]
	$+D7     007633ED      50                     PUSH EAX
	$+D8     007633EE      E8 C0025300            CALL PathOfEx.00C936B3
	$+DD     007633F3      8B01                   MOV EAX, DWORD PTR DS:[ECX]
	$+DF     007633F5      8943 14                MOV DWORD PTR DS:[EBX+0x14], EAX
	$+E2     007633F8      8D59 04                LEA EBX, DWORD PTR DS:[ECX+0x4]
	$+E5     007633FB      6BC8 73                IMUL ECX, EAX, 0x73
	$+E8     007633FE      895D E8                MOV DWORD PTR SS:[EBP-0x18], EBX
	$+EB     00763401      894D 08                MOV DWORD PTR SS:[EBP+0x8], ECX
	$+EE     00763404      8D41 04                LEA EAX, DWORD PTR DS:[ECX+0x4]
	$+F1     00763407      3BD0                   CMP EDX, EAX
	$+F3     00763409      73 14                  JNB SHORT PathOfEx.0076341F
	$+F5     0076340B      56                     PUSH ESI
	$+F6     0076340C      8D8D E0FCFFFF          LEA ECX, DWORD PTR SS:[EBP-0x320]
	$+FC     00763412      E8 B9C9F2FF            CALL PathOfEx.0068FDD0
	$+101    00763417      8BF0                   MOV ESI, EAX
	$+103    00763419      C645 FC 06             MOV BYTE PTR SS:[EBP-0x4], 0x6
	$+107    0076341D    ^ EB 9D                  JMP SHORT PathOfEx.007633BC
	$+109    0076341F      813C19 BBBBBBBB        CMP DWORD PTR DS:[ECX+EBX], 0xBBBBBBBB
	$+110    00763426      8D0419                 LEA EAX, DWORD PTR DS:[ECX+EBX]
	$+113    00763429      8945 EC                MOV DWORD PTR SS:[EBP-0x14], EAX
	$+116    0076342C      0F85 59040000          JNZ PathOfEx.0076388B
	$+11C    00763432      8178 04 BBBBBBBB       CMP DWORD PTR DS:[EAX+0x4], 0xBBBBBBBB
	$+123    00763439      0F85 4C040000          JNZ PathOfEx.0076388B
	$+129    0076343F      83EC 10                SUB ESP, 0x10
	$+12C    00763442      E8 8930FBFF            CALL PathOfEx.007164D0
	$+131    00763447      8BC8                   MOV ECX, EAX
	$+133    00763449      E8 921EDAFF            CALL PathOfEx.005052E0
	$+138    0076344E      8BF0                   MOV ESI, EAX
	$+13A    00763450      8975 DC                MOV DWORD PTR SS:[EBP-0x24], ESI
	$+13D    00763453      83EC 10                SUB ESP, 0x10
	$+140    00763456      C645 FC 09             MOV BYTE PTR SS:[EBP-0x4], 0x9
	$+144    0076345A      E8 7130FBFF            CALL PathOfEx.007164D0
	$+149    0076345F      8BC8                   MOV ECX, EAX
	$+14B    00763461      E8 CA74DCFF            CALL PathOfEx.0052A930
	$+150    00763466      8BF8                   MOV EDI, EAX
	$+152    00763468      897D D8                MOV DWORD PTR SS:[EBP-0x28], EDI
	$+155    0076346B      6A 01                  PUSH 0x1
	$+157    0076346D      68 50AA4600            PUSH PathOfEx.0046AA50
	$+15C    00763472      68 50AA4600            PUSH PathOfEx.0046AA50
	$+161    00763477      68 A8F0E600            PUSH PathOfEx.00E6F0A8                       ; Data/BuffDefinitions.dat
	$+166    0076347C      C645 FC 0A             MOV BYTE PTR SS:[EBP-0x4], 0xA
	$+16A    00763480      E8 4B30FBFF            CALL PathOfEx.007164D0
	$+16F    00763485      8BC8                   MOV ECX, EAX
	$+171    00763487      E8 A416D3FF            CALL PathOfEx.00494B30
	$+176    0076348C      8BD8                   MOV EBX, EAX
	$+178    0076348E      895D D4                MOV DWORD PTR SS:[EBP-0x2C], EBX
	$+17B    00763491      6A 01                  PUSH 0x1
	$+17D    00763493      68 50AA4600            PUSH PathOfEx.0046AA50
	$+182    00763498      68 50AA4600            PUSH PathOfEx.0046AA50
	$+187    0076349D      68 2C21EA00            PUSH PathOfEx.00EA212C                       ; Data/BuffVisuals.dat
	$+18C    007634A2      C645 FC 0B             MOV BYTE PTR SS:[EBP-0x4], 0xB
	$+190    007634A6      E8 2530FBFF            CALL PathOfEx.007164D0
	$+195    007634AB      8BC8                   MOV ECX, EAX
	$+197    007634AD      E8 6ED4F0FF            CALL PathOfEx.00670920
	$+19C    007634B2      8945 C0                MOV DWORD PTR SS:[EBP-0x40], EAX
	$+19F    007634B5      83EC 10                SUB ESP, 0x10
	*/
	//2017/2/28 15:55，更新的时候，特征码失效过一次
	/*op_dw_tmp = scan_exe_.ScanImme<pt_byte>("c7??????????c7????????????c7????????????c7????????????8d????c7????????????8b??c7????????????c7????????????89????e8????????8d????c6??????8b??89????e8????????8b????8d????8b??c6",
	-0x66 + 2);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
	assert(false);
	return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x60 && dw_tmp < 0xa0);
	ofs << "pt_cdword	g_v_tbl_item_passive_skill_size_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;*/

	/*
	$-25A    006CBFDF     CC                     INT3
	$-259    006CBFE0     55                     PUSH EBP
	$-258    006CBFE1     8BEC                   MOV EBP, ESP
	$-256    006CBFE3     6A FF                  PUSH -0x1
	$-254    006CBFE5     68 E721D400            PUSH PathOfEx.00D421E7
	$-24F    006CBFEA     64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$-249    006CBFF0     50                     PUSH EAX
	$-248    006CBFF1     64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$-241    006CBFF8     81EC 8C000000          SUB ESP, 0x8C
	$-23B    006CBFFE     53                     PUSH EBX
	$-23A    006CBFFF     56                     PUSH ESI
	$-239    006CC000     57                     PUSH EDI
	$-238    006CC001     8BF9                   MOV EDI, ECX
	$-236    006CC003     33DB                   XOR EBX, EBX
	$-234    006CC005     895D D8                MOV DWORD PTR SS:[EBP-0x28], EBX
	$-231    006CC008     399F 3C010000          CMP DWORD PTR DS:[EDI+0x13C], EBX
	$-22B    006CC00E     74 28                  JE SHORT PathOfEx.006CC038
	$-229    006CC010     399F D8180000          CMP DWORD PTR DS:[EDI+0x18D8], EBX
	$-223    006CC016     74 20                  JE SHORT PathOfEx.006CC038
	$-221    006CC018     8D4D E8                LEA ECX, DWORD PTR SS:[EBP-0x18]
	$-21E    006CC01B     E8 C016DBFF            CALL PathOfEx.0047D6E0
	$-219    006CC020     8D4D DC                LEA ECX, DWORD PTR SS:[EBP-0x24]
	$-216    006CC023     51                     PUSH ECX
	$-215    006CC024     8B08                   MOV ECX, DWORD PTR DS:[EAX]
	$-213    006CC026     E8 E5A3FDFF            CALL PathOfEx.006A6410
	$-20E    006CC02B     BB 03000000            MOV EBX, 0x3
	$-209    006CC030     C645 F3 00             MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-205    006CC034     3938                   CMP DWORD PTR DS:[EAX], EDI
	$-203    006CC036     74 04                  JE SHORT PathOfEx.006CC03C
	$-201    006CC038     C645 F3 01             MOV BYTE PTR SS:[EBP-0xD], 0x1
	$-1FD    006CC03C     83C8 FF                OR EAX, -0x1
	$-1FA    006CC03F     F6C3 02                TEST BL, 0x2
	$-1F7    006CC042     74 3B                  JE SHORT PathOfEx.006CC07F
	$-1F5    006CC044     83E3 FD                AND EBX, -0x3
	$-1F2    006CC047     C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-1EB    006CC04E     8B75 E0                MOV ESI, DWORD PTR SS:[EBP-0x20]
	$-1E8    006CC051     85F6                   TEST ESI, ESI
	$-1E6    006CC053     74 23                  JE SHORT PathOfEx.006CC078
	$-1E4    006CC055     F0:0FC146 04           LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$-1DF    006CC05A     75 19                  JNZ SHORT PathOfEx.006CC075
	$-1DD    006CC05C     8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$-1DB    006CC05E     8BCE                   MOV ECX, ESI
	$-1D9    006CC060     FF10                   CALL NEAR DWORD PTR DS:[EAX]
	$-1D7    006CC062     83C9 FF                OR ECX, -0x1
	$-1D4    006CC065     8BC1                   MOV EAX, ECX
	$-1D2    006CC067     F0:0FC146 08           LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
	$-1CD    006CC06C     75 07                  JNZ SHORT PathOfEx.006CC075
	$-1CB    006CC06E     8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$-1C9    006CC070     8BCE                   MOV ECX, ESI
	$-1C7    006CC072     FF50 04                CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-1C4    006CC075     83C8 FF                OR EAX, -0x1
	$-1C1    006CC078     C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-1BA    006CC07F     F6C3 01                TEST BL, 0x1
	$-1B7    006CC082     74 37                  JE SHORT PathOfEx.006CC0BB
	$-1B5    006CC084     83E3 FE                AND EBX, -0x2
	$-1B2    006CC087     C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-1AB    006CC08E     8B75 EC                MOV ESI, DWORD PTR SS:[EBP-0x14]
	$-1A8    006CC091     85F6                   TEST ESI, ESI
	$-1A6    006CC093     74 1F                  JE SHORT PathOfEx.006CC0B4
	$-1A4    006CC095     F0:0FC146 04           LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$-19F    006CC09A     75 18                  JNZ SHORT PathOfEx.006CC0B4
	$-19D    006CC09C     8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$-19B    006CC09E     8BCE                   MOV ECX, ESI
	$-199    006CC0A0     FF10                   CALL NEAR DWORD PTR DS:[EAX]
	$-197    006CC0A2     83C9 FF                OR ECX, -0x1
	$-194    006CC0A5     F0:0FC14E 08           LOCK XADD DWORD PTR DS:[ESI+0x8], ECX
	$-18F    006CC0AA     49                     DEC ECX
	$-18E    006CC0AB     75 07                  JNZ SHORT PathOfEx.006CC0B4
	$-18C    006CC0AD     8B06                   MOV EAX, DWORD PTR DS:[ESI]
	$-18A    006CC0AF     8BCE                   MOV ECX, ESI
	$-188    006CC0B1     FF50 04                CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-185    006CC0B4     C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-17E    006CC0BB     807D F3 00             CMP BYTE PTR SS:[EBP-0xD], 0x0
	$-17A    006CC0BF     0F85 0F0B0000          JNZ PathOfEx.006CCBD4
	$-174    006CC0C5     8B75 08                MOV ESI, DWORD PTR SS:[EBP+0x8]
	$-171    006CC0C8     83FE 22                CMP ESI, 0x22
	$-16E    006CC0CB     74 4D                  JE SHORT PathOfEx.006CC11A
	$-16C    006CC0CD     85F6                   TEST ESI, ESI
	$-16A    006CC0CF     74 49                  JE SHORT PathOfEx.006CC11A
	$-168    006CC0D1     83FE 01                CMP ESI, 0x1
	$-165    006CC0D4     74 44                  JE SHORT PathOfEx.006CC11A
	$-163    006CC0D6     83FE 02                CMP ESI, 0x2
	$-160    006CC0D9     74 3F                  JE SHORT PathOfEx.006CC11A
	$-15E    006CC0DB     83FE 03                CMP ESI, 0x3
	$-15B    006CC0DE     74 3A                  JE SHORT PathOfEx.006CC11A
	$-159    006CC0E0     83FE 04                CMP ESI, 0x4
	$-156    006CC0E3     74 35                  JE SHORT PathOfEx.006CC11A
	$-154    006CC0E5     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$-14E    006CC0EB     8B81 440A0000          MOV EAX, DWORD PTR DS:[ECX+0xA44]
	$-148    006CC0F1     85C0                   TEST EAX, EAX
	$-146    006CC0F3     74 0E                  JE SHORT PathOfEx.006CC103
	$-144    006CC0F5     8A80 54070000          MOV AL, BYTE PTR DS:[EAX+0x754]
	$-13E    006CC0FB     84C0                   TEST AL, AL
	$-13C    006CC0FD     0F85 D10A0000          JNZ PathOfEx.006CCBD4
	$-136    006CC103     8B89 240A0000          MOV ECX, DWORD PTR DS:[ECX+0xA24]
	$-130    006CC109     85C9                   TEST ECX, ECX
	$-12E    006CC10B     74 0D                  JE SHORT PathOfEx.006CC11A
	$-12C    006CC10D     E8 BE3DF2FF            CALL PathOfEx.005EFED0
	$-127    006CC112     84C0                   TEST AL, AL
	$-125    006CC114     0F85 BA0A0000          JNZ PathOfEx.006CCBD4
	$-11F    006CC11A     8B87 D8180000          MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$-119    006CC120     8B88 240A0000          MOV ECX, DWORD PTR DS:[EAX+0xA24]
	$-113    006CC126     85C9                   TEST ECX, ECX
	$-111    006CC128     74 1E                  JE SHORT PathOfEx.006CC148
	$-10F    006CC12A     8A81 54070000          MOV AL, BYTE PTR DS:[ECX+0x754]
	$-109    006CC130     84C0                   TEST AL, AL
	$-107    006CC132     74 14                  JE SHORT PathOfEx.006CC148
	$-105    006CC134     8B81 A8090000          MOV EAX, DWORD PTR DS:[ECX+0x9A8]
	$-FF     006CC13A     8A80 56070000          MOV AL, BYTE PTR DS:[EAX+0x756]
	$-F9     006CC140     84C0                   TEST AL, AL
	$-F7     006CC142     74 04                  JE SHORT PathOfEx.006CC148
	$-F5     006CC144     B1 01                  MOV CL, 0x1
	$-F3     006CC146     EB 02                  JMP SHORT PathOfEx.006CC14A
	$-F1     006CC148     32C9                   XOR CL, CL
	$-EF     006CC14A     8B55 14                MOV EDX, DWORD PTR SS:[EBP+0x14]
	$-EC     006CC14D     C602 01                MOV BYTE PTR DS:[EDX], 0x1
	$-E9     006CC150     83FE 25                CMP ESI, 0x25
	$-E6     006CC153     0F87 780A0000          JA PathOfEx.006CCBD1
	$-E0     006CC159     0FB686 44CC6C00        MOVZX EAX, BYTE PTR DS:[ESI+0x6CCC44]
	$-D9     006CC160   - FF2485 E8CB6C00        JMP NEAR DWORD PTR DS:[EAX*4+0x6CCBE8]
	$-D2     006CC167     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$-CE     006CC16B     0F84 630A0000          JE PathOfEx.006CCBD4
	$-C8     006CC171     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$-C4     006CC175     0F85 590A0000          JNZ PathOfEx.006CCBD4
	$-BE     006CC17B     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$-B8     006CC181     56                     PUSH ESI
	$-B7     006CC182     8B89 F4080000          MOV ECX, DWORD PTR DS:[ECX+0x8F4]
	$-B1     006CC188     E8 93BBECFF            CALL PathOfEx.00597D20
	$-AC     006CC18D     5F                     POP EDI                                    ; 22FA24B0
	$-AB     006CC18E     5E                     POP ESI                                    ; 22FA24B0
	$-AA     006CC18F     5B                     POP EBX                                    ; 22FA24B0
	$-A9     006CC190     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-A6     006CC193     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$-9F     006CC19A     8BE5                   MOV ESP, EBP
	$-9D     006CC19C     5D                     POP EBP                                    ; 22FA24B0
	$-9C     006CC19D     C2 1000                RETN 0x10
	$-99     006CC1A0     8B87 64150000          MOV EAX, DWORD PTR DS:[EDI+0x1564]
	$-93     006CC1A6     3B87 68150000          CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-8D     006CC1AC     75 16                  JNZ SHORT PathOfEx.006CC1C4
	$-8B     006CC1AE     C745 E8 00000000       MOV DWORD PTR SS:[EBP-0x18], 0x0
	$-84     006CC1B5     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-81     006CC1B8     C745 EC 00000000       MOV DWORD PTR SS:[EBP-0x14], 0x0
	$-7A     006CC1BF     83CB 04                OR EBX, 0x4
	$-77     006CC1C2     EB 15                  JMP SHORT PathOfEx.006CC1D9
	$-75     006CC1C4     8B87 68150000          MOV EAX, DWORD PTR DS:[EDI+0x1568]
	$-6F     006CC1CA     8D4D D0                LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-6C     006CC1CD     51                     PUSH ECX
	$-6B     006CC1CE     8D48 F8                LEA ECX, DWORD PTR DS:[EAX-0x8]
	$-68     006CC1D1     E8 FA2BEBFF            CALL PathOfEx.0057EDD0
	$-63     006CC1D6     83CB 08                OR EBX, 0x8
	$-60     006CC1D9     8B08                   MOV ECX, DWORD PTR DS:[EAX]
	$-5E     006CC1DB     894D DC                MOV DWORD PTR SS:[EBP-0x24], ECX
	$-5B     006CC1DE     8B48 04                MOV ECX, DWORD PTR DS:[EAX+0x4]
	$-58     006CC1E1     894D E0                MOV DWORD PTR SS:[EBP-0x20], ECX
	$-55     006CC1E4     C700 00000000          MOV DWORD PTR DS:[EAX], 0x0
	$-4F     006CC1EA     C740 04 00000000       MOV DWORD PTR DS:[EAX+0x4], 0x0
	$-48     006CC1F1     C745 FC 02000000       MOV DWORD PTR SS:[EBP-0x4], 0x2
	$-41     006CC1F8     F6C3 08                TEST BL, 0x8
	$-3E     006CC1FB     74 0B                  JE SHORT PathOfEx.006CC208
	$-3C     006CC1FD     8D4D D0                LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-39     006CC200     83E3 F7                AND EBX, -0x9
	$-36     006CC203     E8 A8E7D9FF            CALL PathOfEx.0046A9B0
	$-31     006CC208     F6C3 04                TEST BL, 0x4
	$-2E     006CC20B     74 08                  JE SHORT PathOfEx.006CC215
	$-2C     006CC20D     8D4D E8                LEA ECX, DWORD PTR SS:[EBP-0x18]
	$-29     006CC210     E8 9BE7D9FF            CALL PathOfEx.0046A9B0
	$-24     006CC215     8B8F 34010000          MOV ECX, DWORD PTR DS:[EDI+0x134]
	$-1E     006CC21B     8D45 08                LEA EAX, DWORD PTR SS:[EBP+0x8]
	$-1B     006CC21E     50                     PUSH EAX
	$-1A     006CC21F     56                     PUSH ESI
	$-19     006CC220     E8 0B320F00            CALL PathOfEx.007BF430
	$-14     006CC225     84C0                   TEST AL, AL
	$-12     006CC227     74 6B                  JE SHORT PathOfEx.006CC294
	$-10     006CC229     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$-C      006CC22D     0F84 8A030000          JE PathOfEx.006CC5BD
	$-6      006CC233     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$-2      006CC237     75 5B                  JNZ SHORT PathOfEx.006CC294
	$ ==>    006CC239     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]         ; stCD_DownInventoryMgr的偏移
	$+6      006CC23F     8D46 FB                LEA EAX, DWORD PTR DS:[ESI-0x5]
	$+9      006CC242     50                     PUSH EAX                                   ; eax,鼠标左键是0，中键是1，依此类推
	$+A      006CC243     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$+D      006CC246     50                     PUSH EAX                                   ; eax=stCD_Skill_IdInfo*
	$+E      006CC247     E8 C4D7FAFF            CALL PathOfEx.00679A10                     ; 根据索引查找并填充stCD_Skill_IdInfo
	$-21     006799EF     CC                         INT3
	$-20     006799F0     8B4424 04                  MOV EAX, DWORD PTR SS:[ESP+0x4]
	$-1C     006799F4     8901                       MOV DWORD PTR DS:[ECX], EAX
	$-1A     006799F6     8B4424 08                  MOV EAX, DWORD PTR SS:[ESP+0x8]
	$-16     006799FA     8941 04                    MOV DWORD PTR DS:[ECX+0x4], EAX
	$-13     006799FD     8B4424 0C                  MOV EAX, DWORD PTR SS:[ESP+0xC]            ; apisetsc.0004086C
	$-F      00679A01     8941 08                    MOV DWORD PTR DS:[ECX+0x8], EAX
	$-C      00679A04     8BC1                       MOV EAX, ECX
	$-A      00679A06     C2 0C00                    RETN 0xC
	$-7      00679A09     CC                         INT3
	$-6      00679A0A     CC                         INT3
	$-5      00679A0B     CC                         INT3
	$-4      00679A0C     CC                         INT3
	$-3      00679A0D     CC                         INT3
	$-2      00679A0E     CC                         INT3
	$-1      00679A0F     CC                         INT3
	$ ==>    00679A10     FF7424 08                  PUSH DWORD PTR SS:[ESP+0x8]
	$+4      00679A14     8B89 68090000              MOV ECX, DWORD PTR DS:[ECX+0x968]          ; stCD_InventorySkillMgr的偏移
	$+A      00679A1A     FF7424 08                  PUSH DWORD PTR SS:[ESP+0x8]
	$+E      00679A1E     E8 2DE0FAFF                CALL PathOfEx.00627A50
	$-1      00627A4F     CC                         INT3
	$ ==>    00627A50     FF7424 04                  PUSH DWORD PTR SS:[ESP+0x4]
	$+4      00627A54     8BD1                       MOV EDX, ECX
	$+6      00627A56     8B4C24 0C                  MOV ECX, DWORD PTR SS:[ESP+0xC]            ; apisetsc.0004086C
	$+A      00627A5A     8B8C8A 08090000            MOV ECX, DWORD PTR DS:[EDX+ECX*4+0x908]    ; stCD_InventorySkillItem的偏移
	$+11     00627A61     E8 AA49F5FF                CALL PathOfEx.0057C410
	$-81     0057C38F     51                         PUSH ECX
	$-80     0057C390     52                         PUSH EDX
	$-7F     0057C391     8D4D DC                    LEA ECX, DWORD PTR SS:[EBP-0x24]
	$-7C     0057C394     E8 D718EFFF                CALL PathOfEx.0046DC70
	$-77     0057C399     8D45 DC                    LEA EAX, DWORD PTR SS:[EBP-0x24]
	$-74     0057C39C     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-6D     0057C3A3     50                         PUSH EAX
	$-6C     0057C3A4     8D45 C4                    LEA EAX, DWORD PTR SS:[EBP-0x3C]
	$-69     0057C3A7     50                         PUSH EAX
	$-68     0057C3A8     E8 63AB1900                CALL PathOfEx.00716F10
	$-63     0057C3AD     83C4 08                    ADD ESP, 0x8
	$-60     0057C3B0     8B45 F0                    MOV EAX, DWORD PTR SS:[EBP-0x10]           ; PathOfEx.00E340B0
	$-5D     0057C3B3     83F8 08                    CMP EAX, 0x8
	$-5A     0057C3B6     72 0F                      JB SHORT PathOfEx.0057C3C7
	$-58     0057C3B8     40                         INC EAX
	$-57     0057C3B9     6A 02                      PUSH 0x2
	$-55     0057C3BB     50                         PUSH EAX
	$-54     0057C3BC     FF75 DC                    PUSH DWORD PTR SS:[EBP-0x24]
	$-51     0057C3BF     E8 2C04EFFF                CALL PathOfEx.0046C7F0
	$-4C     0057C3C4     83C4 0C                    ADD ESP, 0xC
	$-49     0057C3C7     8B4D 08                    MOV ECX, DWORD PTR SS:[EBP+0x8]
	$-46     0057C3CA     894D 0C                    MOV DWORD PTR SS:[EBP+0xC], ECX
	$-43     0057C3CD     C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4], 0x3
	$-3F     0057C3D1     85C9                       TEST ECX, ECX
	$-3D     0057C3D3     74 0A                      JE SHORT PathOfEx.0057C3DF
	$-3B     0057C3D5     51                         PUSH ECX
	$-3A     0057C3D6     8D45 C4                    LEA EAX, DWORD PTR SS:[EBP-0x3C]
	$-37     0057C3D9     50                         PUSH EAX
	$-36     0057C3DA     E8 71F11E00                CALL PathOfEx.0076B550
	$-31     0057C3DF     8B45 D8                    MOV EAX, DWORD PTR SS:[EBP-0x28]
	$-2E     0057C3E2     83F8 08                    CMP EAX, 0x8
	$-2B     0057C3E5     72 0F                      JB SHORT PathOfEx.0057C3F6
	$-29     0057C3E7     6A 02                      PUSH 0x2
	$-27     0057C3E9     40                         INC EAX
	$-26     0057C3EA     50                         PUSH EAX
	$-25     0057C3EB     FF75 C4                    PUSH DWORD PTR SS:[EBP-0x3C]               ; PathOfEx.00491313
	$-22     0057C3EE     E8 FD03EFFF                CALL PathOfEx.0046C7F0
	$-1D     0057C3F3     83C4 0C                    ADD ESP, 0xC
	$-1A     0057C3F6     8B4D F4                    MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-17     0057C3F9     64:890D 00000000           MOV DWORD PTR FS:[0], ECX
	$-10     0057C400     5E                         POP ESI                                    ; 22FA24B0
	$-F      0057C401     8BE5                       MOV ESP, EBP
	$-D      0057C403     5D                         POP EBP                                    ; 22FA24B0
	$-C      0057C404     C3                         RETN
	$-B      0057C405     81E9 B0080000              SUB ECX, 0x8B0
	$-5      0057C40B   ^ E9 90EFFFFF                JMP PathOfEx.0057B3A0
	$ ==>    0057C410     FF7424 04                  PUSH DWORD PTR SS:[ESP+0x4]
	$+4      0057C414     81C1 30090000              ADD ECX, 0x930                             ; stCD_InventorySkillItemInfo的偏移
	$+A      0057C41A     E8 B1290000                CALL PathOfEx.0057EDD0
	$+F      0057C41F     8B4424 04                  MOV EAX, DWORD PTR SS:[ESP+0x4]
	$+13     0057C423     C2 0400                    RETN 0x4
	$+16     0057C426     CC                         INT3
	$+16     00627A66     8B4424 04                  MOV EAX, DWORD PTR SS:[ESP+0x4]
	$+1A     00627A6A     C2 0800                    RETN 0x8
	$+1D     00627A6D     CC                         INT3
	$+1E     00627A6E     CC                         INT3
	$+1F     00627A6F     CC                         INT3
	$+20     00627A70     55                         PUSH EBP
	$+21     00627A71     8BEC                       MOV EBP, ESP
	$+23     00627A73     64:A1 00000000             MOV EAX, DWORD PTR FS:[0]
	$+29     00627A79     6A FF                      PUSH -0x1
	$+2B     00627A7B     68 C27FD300                PUSH PathOfEx.00D37FC2
	$+30     00627A80     50                         PUSH EAX
	$+31     00627A81     64:8925 00000000           MOV DWORD PTR FS:[0], ESP
	$+38     00627A88     83EC 10                    SUB ESP, 0x10
	$+3B     00627A8B     56                         PUSH ESI
	$+3C     00627A8C     8BF1                       MOV ESI, ECX
	$+3E     00627A8E     8B86 70090000              MOV EAX, DWORD PTR DS:[ESI+0x970]
	$+44     00627A94     8A80 56070000              MOV AL, BYTE PTR DS:[EAX+0x756]
	$+4A     00627A9A     84C0                       TEST AL, AL
	$+4C     00627A9C     0F84 01010000              JE PathOfEx.00627BA3
	$+52     00627AA2     8B86 BC080000              MOV EAX, DWORD PTR DS:[ESI+0x8BC]
	$+58     00627AA8     85C0                       TEST EAX, EAX
	$+5A     00627AAA     0F84 F3000000              JE PathOfEx.00627BA3
	$+13     00679A23     8B4424 04                  MOV EAX, DWORD PTR SS:[ESP+0x4]
	$+17     00679A27     C2 0800                    RETN 0x8
	$+1A     00679A2A     CC                         INT3
	$+1B     00679A2B     CC                         INT3
	$+1C     00679A2C     CC                         INT3
	$+1D     00679A2D     CC                         INT3
	$+1E     00679A2E     CC                         INT3
	$+1F     00679A2F     CC                         INT3
	$+20     00679A30     51                         PUSH ECX
	$+21     00679A31     56                         PUSH ESI
	$+22     00679A32     57                         PUSH EDI
	$+23     00679A33     8BB9 340A0000              MOV EDI, DWORD PTR DS:[ECX+0xA34]          ; apisetsc.0004086C
	$+29     00679A39     8BB7 34090000              MOV ESI, DWORD PTR DS:[EDI+0x934]
	$+2F     00679A3F     80BE 54070000 00           CMP BYTE PTR DS:[ESI+0x754], 0x0
	$+36     00679A46     74 57                      JE SHORT PathOfEx.00679A9F
	$+38     00679A48     8B89 680B0000              MOV ECX, DWORD PTR DS:[ECX+0xB68]          ; qbclient.0EBB31C9
	$+3E     00679A4E     8D4424 0B                  LEA EAX, DWORD PTR SS:[ESP+0xB]
	$+42     00679A52     50                         PUSH EAX
	$+43     00679A53     68 A048E900                PUSH PathOfEx.00E948A0                     ; auto_recenter_map
	$+48     00679A58     68 08DEE600                PUSH PathOfEx.00E6DE08                     ; UI
	$+4D     00679A5D     8B89 80030000              MOV ECX, DWORD PTR DS:[ECX+0x380]
	$+53     00679A63     C64424 17 00               MOV BYTE PTR SS:[ESP+0x17], 0x0
	$+58     00679A68     E8 F3C71400                CALL PathOfEx.007C6260
	$+5D     00679A6D     807C24 0B 00               CMP BYTE PTR SS:[ESP+0xB], 0x0
	$+62     00679A72     74 1E                      JE SHORT PathOfEx.00679A92
	$+64     00679A74     80BE FC080000 00           CMP BYTE PTR DS:[ESI+0x8FC], 0x0
	$+6B     00679A7B     74 15                      JE SHORT PathOfEx.00679A92
	$+6D     00679A7D     5F                         POP EDI                                    ; 22FA24B0
	$+6E     00679A7E     C686 D0080000 01           MOV BYTE PTR DS:[ESI+0x8D0], 0x1
	$+75     00679A85     C786 D4080000 0000803E     MOV DWORD PTR DS:[ESI+0x8D4], 0x3E800000
	$+7F     00679A8F     5E                         POP ESI                                    ; 22FA24B0
	$+80     00679A90     59                         POP ECX                                    ; 22FA24B0
	$+81     00679A91     C3                         RETN
	$+82     00679A92     6A 00                      PUSH 0x0
	$+84     00679A94     8BCE                       MOV ECX, ESI
	$+86     00679A96     E8 E5193700                CALL PathOfEx.009EB480
	$+8B     00679A9B     5F                         POP EDI                                    ; 22FA24B0
	$+8C     00679A9C     5E                         POP ESI                                    ; 22FA24B0
	$+8D     00679A9D     59                         POP ECX                                    ; 22FA24B0
	$+8E     00679A9E     C3                         RETN
	$+8F     00679A9F     80BE 54070000 01           CMP BYTE PTR DS:[ESI+0x754], 0x1
	$+96     00679AA6     74 24                      JE SHORT PathOfEx.00679ACC
	$+98     00679AA8     8B06                       MOV EAX, DWORD PTR DS:[ESI]
	$+9A     00679AAA     8BCE                       MOV ECX, ESI
	$+9C     00679AAC     C686 54070000 01           MOV BYTE PTR DS:[ESI+0x754], 0x1
	$+A3     00679AB3     FF90 B0000000              CALL NEAR DWORD PTR DS:[EAX+0xB0]
	$+A9     00679AB9     8B8E 5C070000              MOV ECX, DWORD PTR DS:[ESI+0x75C]
	$+AF     00679ABF     85C9                       TEST ECX, ECX
	$+B1     00679AC1     74 09                      JE SHORT PathOfEx.00679ACC
	$+B3     00679AC3     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+B5     00679AC5     56                         PUSH ESI
	$+B6     00679AC6     FF90 D0000000              CALL NEAR DWORD PTR DS:[EAX+0xD0]
	$+BC     00679ACC     8B8F 34090000              MOV ECX, DWORD PTR DS:[EDI+0x934]
	$+C2     00679AD2     5F                         POP EDI                                    ; 22FA24B0
	$+C3     00679AD3     5E                         POP ESI                                    ; 22FA24B0
	$+C4     00679AD4     83C4 04                    ADD ESP, 0x4
	$+C7     00679AD7   ^ E9 24E4F3FF                JMP PathOfEx.005B7F00
	$+CC     00679ADC     CC                         INT3
	$+13     006CC24C     C645 FC 03             MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+17     006CC250     8B4D 08                MOV ECX, DWORD PTR SS:[EBP+0x8]
	$+1A     006CC253     E8 C8383600            CALL PathOfEx.00A2FB20
	$+1F     006CC258     8BD0                   MOV EDX, EAX
	$+21     006CC25A     83FA 20                CMP EDX, 0x20
	$+24     006CC25D     0F95C1                 SETNE CL
	$+27     006CC260     84C9                   TEST CL, CL
	$+29     006CC262     0F84 F1020000          JE PathOfEx.006CC559
	$+2F     006CC268     8B9F D8180000          MOV EBX, DWORD PTR DS:[EDI+0x18D8]
	$+35     006CC26E     8B8F 140C0000          MOV ECX, DWORD PTR DS:[EDI+0xC14]
	$+3B     006CC274     3B8B 400A0000          CMP ECX, DWORD PTR DS:[EBX+0xA40]
	$+41     006CC27A     74 33                  JE SHORT PathOfEx.006CC2AF
	$+43     006CC27C     51                     PUSH ECX
	$+44     006CC27D     8B8B 380A0000          MOV ECX, DWORD PTR DS:[EBX+0xA38]
	$+4A     006CC283     E8 48F03100            CALL PathOfEx.009EB2D0
	$+4F     006CC288     84C0                   TEST AL, AL
	$+51     006CC28A     75 23                  JNZ SHORT PathOfEx.006CC2AF
	$+53     006CC28C     8D4D E8                LEA ECX, DWORD PTR SS:[EBP-0x18]
	$+56     006CC28F     E8 1CE7D9FF            CALL PathOfEx.0046A9B0
	$+5B     006CC294     8D4D DC                LEA ECX, DWORD PTR SS:[EBP-0x24]
	$+5E     006CC297     E8 14E7D9FF            CALL PathOfEx.0046A9B0
	$+63     006CC29C     5F                     POP EDI                                    ; 22FA24B0
	$+64     006CC29D     5E                     POP ESI                                    ; 22FA24B0
	$+65     006CC29E     5B                     POP EBX                                    ; 22FA24B0
	$+66     006CC29F     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+69     006CC2A2     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+70     006CC2A9     8BE5                   MOV ESP, EBP
	$+72     006CC2AB     5D                     POP EBP                                    ; 22FA24B0
	$+73     006CC2AC     C2 1000                RETN 0x10
	$+76     006CC2AF     8B9B 64090000          MOV EBX, DWORD PTR DS:[EBX+0x964]
	$+7C     006CC2B5     83FA 01                CMP EDX, 0x1
	$+7F     006CC2B8     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+85     006CC2BE     0F94C2                 SETE DL
	$+88     006CC2C1     8855 0F                MOV BYTE PTR SS:[EBP+0xF], DL
	$+8B     006CC2C4     83BB 38090000 01       CMP DWORD PTR DS:[EBX+0x938], 0x1
	$+92     006CC2CB     8B83 40090000          MOV EAX, DWORD PTR DS:[EBX+0x940]
	$+98     006CC2D1     8945 D8                MOV DWORD PTR SS:[EBP-0x28], EAX
	$+9B     006CC2D4     8B83 4C090000          MOV EAX, DWORD PTR DS:[EBX+0x94C]
	$+A1     006CC2DA     8945 E4                MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+A4     006CC2DD     8B81 943A0000          MOV EAX, DWORD PTR DS:[ECX+0x3A94]
	$+AA     006CC2E3     8B40 08                MOV EAX, DWORD PTR DS:[EAX+0x8]            ; PathOfEx.00E340B0
	$+AD     006CC2E6     8A70 0C                MOV DH, BYTE PTR DS:[EAX+0xC]
	$+B0     006CC2E9     8B81 943A0000          MOV EAX, DWORD PTR DS:[ECX+0x3A94]
	$+B6     006CC2EF     8875 17                MOV BYTE PTR SS:[EBP+0x17], DH
	$+B9     006CC2F2     8B40 08                MOV EAX, DWORD PTR DS:[EAX+0x8]            ; PathOfEx.00E340B0
	$+BC     006CC2F5     8A88 DE000000          MOV CL, BYTE PTR DS:[EAX+0xDE]
	$+C2     006CC2FB     8B45 E4                MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+C5     006CC2FE     884D F3                MOV BYTE PTR SS:[EBP-0xD], CL
	$+C8     006CC301     75 1F                  JNZ SHORT PathOfEx.006CC322
	$+CA     006CC303     84D2                   TEST DL, DL
	$+CC     006CC305     74 1B                  JE SHORT PathOfEx.006CC322
	$+CE     006CC307     85C0                   TEST EAX, EAX
	$+D0     006CC309     74 17                  JE SHORT PathOfEx.006CC322
	$+D2     006CC30B     84F6                   TEST DH, DH
	$+D4     006CC30D     75 04                  JNZ SHORT PathOfEx.006CC313
	$+D6     006CC30F     84C9                   TEST CL, CL
	$+D8     006CC311     74 09                  JE SHORT PathOfEx.006CC31C
	$+DA     006CC313     83BF BC180000 00       CMP DWORD PTR DS:[EDI+0x18BC], 0x0
	$+E1     006CC31A     75 06                  JNZ SHORT PathOfEx.006CC322
	$+E3     006CC31C     C645 13 01             MOV BYTE PTR SS:[EBP+0x13], 0x1
	$+E7     006CC320     EB 04                  JMP SHORT PathOfEx.006CC326
	$+E9     006CC322     C645 13 00             MOV BYTE PTR SS:[EBP+0x13], 0x0
	$+ED     006CC326     83BB 38090000 02       CMP DWORD PTR DS:[EBX+0x938], 0x2
	$+F4     006CC32D     75 23                  JNZ SHORT PathOfEx.006CC352
	$+F6     006CC32F     84D2                   TEST DL, DL
	$+F8     006CC331     74 1F                  JE SHORT PathOfEx.006CC352
	$+FA     006CC333     85C0                   TEST EAX, EAX
	$+FC     006CC335     74 1B                  JE SHORT PathOfEx.006CC352
	$+FE     006CC337     84F6                   TEST DH, DH
	$+100    006CC339     75 17                  JNZ SHORT PathOfEx.006CC352
	$+102    006CC33B     84C9                   TEST CL, CL
	$+104    006CC33D     75 13                  JNZ SHORT PathOfEx.006CC352
	$+106    006CC33F     8B8F BC180000          MOV ECX, DWORD PTR DS:[EDI+0x18BC]
	$+10C    006CC345     E8 A69FFFFF            CALL PathOfEx.006C62F0
	$+111    006CC34A     C645 0B 01             MOV BYTE PTR SS:[EBP+0xB], 0x1
	$+115    006CC34E     84C0                   TEST AL, AL
	$+117    006CC350     75 04                  JNZ SHORT PathOfEx.006CC356
	$+119    006CC352     C645 0B 00             MOV BYTE PTR SS:[EBP+0xB], 0x0
	$+11D    006CC356     83BB 38090000 02       CMP DWORD PTR DS:[EBX+0x938], 0x2
	$+124    006CC35D     75 2B                  JNZ SHORT PathOfEx.006CC38A
	$+126    006CC35F     807D 0F 00             CMP BYTE PTR SS:[EBP+0xF], 0x0
	$+12A    006CC363     74 25                  JE SHORT PathOfEx.006CC38A
	$+12C    006CC365     837D E4 00             CMP DWORD PTR SS:[EBP-0x1C], 0x0
	$+130    006CC369     74 1F                  JE SHORT PathOfEx.006CC38A
	$+132    006CC36B     807D 17 00             CMP BYTE PTR SS:[EBP+0x17], 0x0
	$+136    006CC36F     75 19                  JNZ SHORT PathOfEx.006CC38A
	$+138    006CC371     807D F3 00             CMP BYTE PTR SS:[EBP-0xD], 0x0
	$+13C    006CC375     75 13                  JNZ SHORT PathOfEx.006CC38A
	$+13E    006CC377     8B8F BC180000          MOV ECX, DWORD PTR DS:[EDI+0x18BC]
	$+144    006CC37D     E8 BE9FFFFF            CALL PathOfEx.006C6340
	$+149    006CC382     84C0                   TEST AL, AL
	$+14B    006CC384     74 04                  JE SHORT PathOfEx.006CC38A
	$+14D    006CC386     B4 01                  MOV AH, 0x1
	$+14F    006CC388     EB 02                  JMP SHORT PathOfEx.006CC38C
	$+151    006CC38A     32E4                   XOR AH, AH
	$+153    006CC38C     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+159    006CC392     8A55 0F                MOV DL, BYTE PTR SS:[EBP+0xF]
	$+15C    006CC395     8A81 AC3E0000          MOV AL, BYTE PTR DS:[ECX+0x3EAC]
	$+162    006CC39B     84C0                   TEST AL, AL
	$+164    006CC39D     74 11                  JE SHORT PathOfEx.006CC3B0
	$+166    006CC39F     83BB 38090000 05       CMP DWORD PTR DS:[EBX+0x938], 0x5
	$+16D    006CC3A6     75 08                  JNZ SHORT PathOfEx.006CC3B0
	$+16F    006CC3A8     84D2                   TEST DL, DL
	$+171    006CC3AA     74 04                  JE SHORT PathOfEx.006CC3B0
	$+173    006CC3AC     B0 01                  MOV AL, 0x1
	$+175    006CC3AE     EB 02                  JMP SHORT PathOfEx.006CC3B2
	$+177    006CC3B0     32C0                   XOR AL, AL
	$+179    006CC3B2     807D 13 00             CMP BYTE PTR SS:[EBP+0x13], 0x0
	$+17D    006CC3B6     74 0C                  JE SHORT PathOfEx.006CC3C4
	$+17F    006CC3B8     8BCF                   MOV ECX, EDI
	$+181    006CC3BA     E8 310D0000            CALL PathOfEx.006CD0F0
	$+186    006CC3BF     E9 ED010000            JMP PathOfEx.006CC5B1
	$+18B    006CC3C4     807D 0B 00             CMP BYTE PTR SS:[EBP+0xB], 0x0
	$+18F    006CC3C8     0F85 ED000000          JNZ PathOfEx.006CC4BB
	$+195    006CC3CE     84E4                   TEST AH, AH
	$+197    006CC3D0     0F85 E5000000          JNZ PathOfEx.006CC4BB
	$+19D    006CC3D6     84C0                   TEST AL, AL
	$+19F    006CC3D8     74 4A                  JE SHORT PathOfEx.006CC424
	$+1A1    006CC3DA     8BB7 D81A0000          MOV ESI, DWORD PTR DS:[EDI+0x1AD8]
	$+1A7    006CC3E0     85F6                   TEST ESI, ESI
	$+1A9    006CC3E2     74 25                  JE SHORT PathOfEx.006CC409
	$+1AB    006CC3E4     8D87 9C180000          LEA EAX, DWORD PTR DS:[EDI+0x189C]
	$+1B1    006CC3EA     50                     PUSH EAX
	$+1B2    006CC3EB     8D45 C8                LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+1B5    006CC3EE     50                     PUSH EAX
	$+1B6    006CC3EF     E8 2C521200            CALL PathOfEx.007F1620
	$+1BB    006CC3F4     83C4 08                ADD ESP, 0x8
	$+1BE    006CC3F7     8BCE                   MOV ECX, ESI
	$+1C0    006CC3F9     FF70 04                PUSH DWORD PTR DS:[EAX+0x4]
	$+1C3    006CC3FC     FF30                   PUSH DWORD PTR DS:[EAX]
	$+1C5    006CC3FE     FF75 E4                PUSH DWORD PTR SS:[EBP-0x1C]
	$+1C8    006CC401     FF75 D8                PUSH DWORD PTR SS:[EBP-0x28]
	$+1CB    006CC404     E8 A721E0FF            CALL PathOfEx.004CE5B0
	$+1D0    006CC409     80BF D51A0000 00       CMP BYTE PTR DS:[EDI+0x1AD5], 0x0
	$+1D7    006CC410     0F85 9B010000          JNZ PathOfEx.006CC5B1
	$+1DD    006CC416     6A 01                  PUSH 0x1
	$+1DF    006CC418     8BCF                   MOV ECX, EDI
	$+1E1    006CC41A     E8 613E0000            CALL PathOfEx.006D0280
	$+1E6    006CC41F     E9 8D010000            JMP PathOfEx.006CC5B1
	$+1EB    006CC424     83B9 0C400000 00       CMP DWORD PTR DS:[ECX+0x400C], 0x0
	$+1F2    006CC42B     76 41                  JBE SHORT PathOfEx.006CC46E
	$+1F4    006CC42D     83B9 0C400000 01       CMP DWORD PTR DS:[ECX+0x400C], 0x1
	$+1FB    006CC434     74 38                  JE SHORT PathOfEx.006CC46E
	$+1FD    006CC436     8B87 D0180000          MOV EAX, DWORD PTR DS:[EDI+0x18D0]
	$+203    006CC43C     3B87 BC180000          CMP EAX, DWORD PTR DS:[EDI+0x18BC]
	$+209    006CC442     74 36                  JE SHORT PathOfEx.006CC47A
	$+20B    006CC444     84D2                   TEST DL, DL
	$+20D    006CC446     75 19                  JNZ SHORT PathOfEx.006CC461
	$+20F    006CC448     8B4D E8                MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+212    006CC44B     85C9                   TEST ECX, ECX
	$+214    006CC44D     0F94C0                 SETE AL
	$+217    006CC450     34 01                  XOR AL, 0x1
	$+219    006CC452     74 26                  JE SHORT PathOfEx.006CC47A
	$+21B    006CC454     66:8B41 02             MOV AX, WORD PTR DS:[ECX+0x2]
	$+21F    006CC458     66:3B05 0CD70001       CMP AX, WORD PTR DS:[0x100D70C]
	$+226    006CC45F     75 19                  JNZ SHORT PathOfEx.006CC47A
	$+228    006CC461     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+22E    006CC467     E8 E47CFBFF            CALL PathOfEx.00684150
	$+233    006CC46C     EB 0C                  JMP SHORT PathOfEx.006CC47A
	$+235    006CC46E     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$+238    006CC471     8BCF                   MOV ECX, EDI
	$+23A    006CC473     50                     PUSH EAX
	$+23B    006CC474     56                     PUSH ESI
	$+23C    006CC475     E8 A64E0000            CALL PathOfEx.006D1320
	$+241    006CC47A     8B87 D0180000          MOV EAX, DWORD PTR DS:[EDI+0x18D0]
	$+247    006CC480     3B87 BC180000          CMP EAX, DWORD PTR DS:[EDI+0x18BC]
	$+24D    006CC486     0F84 25010000          JE PathOfEx.006CC5B1
	$+253    006CC48C     8B87 D8180000          MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$+259    006CC492     8B88 64090000          MOV ECX, DWORD PTR DS:[EAX+0x964]          ; user32.75496D51
	$+25F    006CC498     8B81 38090000          MOV EAX, DWORD PTR DS:[ECX+0x938]
	$+265    006CC49E     83F8 02                CMP EAX, 0x2
	$+268    006CC4A1     74 0E                  JE SHORT PathOfEx.006CC4B1
	$+26A    006CC4A3     83F8 03                CMP EAX, 0x3
	$+26D    006CC4A6     74 09                  JE SHORT PathOfEx.006CC4B1
	$+26F    006CC4A8     83F8 04                CMP EAX, 0x4
	$+272    006CC4AB     0F85 00010000          JNZ PathOfEx.006CC5B1
	$+278    006CC4B1     E8 DAC8ECFF            CALL PathOfEx.00598D90
	$+27D    006CC4B6     E9 F6000000            JMP PathOfEx.006CC5B1
	$+282    006CC4BB     8A83 68090000          MOV AL, BYTE PTR DS:[EBX+0x968]
	$+288    006CC4C1     84C0                   TEST AL, AL
	$+28A    006CC4C3     74 2C                  JE SHORT PathOfEx.006CC4F1
	$+28C    006CC4C5     8A83 78090000          MOV AL, BYTE PTR DS:[EBX+0x978]
	$+292    006CC4CB     84C0                   TEST AL, AL
	$+294    006CC4CD     74 22                  JE SHORT PathOfEx.006CC4F1
	$+296    006CC4CF     FFB7 BC180000          PUSH DWORD PTR DS:[EDI+0x18BC]
	$+29C    006CC4D5     FFB3 5C090000          PUSH DWORD PTR DS:[EBX+0x95C]
	$+2A2    006CC4DB     FFB3 58090000          PUSH DWORD PTR DS:[EBX+0x958]
	$+2A8    006CC4E1     FF75 D8                PUSH DWORD PTR SS:[EBP-0x28]
	$+2AB    006CC4E4     FFB3 50090000          PUSH DWORD PTR DS:[EBX+0x950]
	$+2B1    006CC4EA     E8 919A2000            CALL PathOfEx.008D5F80
	$+2B6    006CC4EF     EB 11                  JMP SHORT PathOfEx.006CC502
	$+2B8    006CC4F1     FFB7 BC180000          PUSH DWORD PTR DS:[EDI+0x18BC]
	$+2BE    006CC4F7     FF75 D8                PUSH DWORD PTR SS:[EBP-0x28]
	$+2C1    006CC4FA     FF75 E4                PUSH DWORD PTR SS:[EBP-0x1C]
	$+2C4    006CC4FD     E8 CE982000            CALL PathOfEx.008D5DD0
	$+2C9    006CC502     8D45 0C                LEA EAX, DWORD PTR SS:[EBP+0xC]
	$+2CC    006CC505     8BCB                   MOV ECX, EBX
	$+2CE    006CC507     50                     PUSH EAX
	$+2CF    006CC508     E8 13C9ECFF            CALL PathOfEx.00598E20
	$+2D4    006CC50D     8D4D 0C                LEA ECX, DWORD PTR SS:[EBP+0xC]
	$+2D7    006CC510     8B30                   MOV ESI, DWORD PTR DS:[EAX]
	$+2D9    006CC512     E8 B905DAFF            CALL PathOfEx.0046CAD0
	$+2DE    006CC517     85F6                   TEST ESI, ESI
	$+2E0    006CC519     0F84 92000000          JE PathOfEx.006CC5B1
	$+2E6    006CC51F     8D45 0C                LEA EAX, DWORD PTR SS:[EBP+0xC]
	$+2E9    006CC522     8BCB                   MOV ECX, EBX
	$+2EB    006CC524     50                     PUSH EAX
	$+2EC    006CC525     E8 F6C8ECFF            CALL PathOfEx.00598E20
	$+2F1    006CC52A     8BF0                   MOV ESI, EAX
	$+2F3    006CC52C     C645 FC 04             MOV BYTE PTR SS:[EBP-0x4], 0x4
	$+2F7    006CC530     E8 ABDD2200            CALL PathOfEx.008FA2E0
	$+2FC    006CC535     83EC 08                SUB ESP, 0x8
	$+2FF    006CC538     8B08                   MOV ECX, DWORD PTR DS:[EAX]
	$+301    006CC53A     C74424 04 0000803F     MOV DWORD PTR SS:[ESP+0x4], 0x3F800000
	$+309    006CC542     C70424 0000803F        MOV DWORD PTR SS:[ESP], 0x3F800000
	$+310    006CC549     56                     PUSH ESI
	$+311    006CC54A     E8 21CD2200            CALL PathOfEx.008F9270
	$+316    006CC54F     8D4D 0C                LEA ECX, DWORD PTR SS:[EBP+0xC]
	$+319    006CC552     E8 7905DAFF            CALL PathOfEx.0046CAD0
	$+31E    006CC557     EB 58                  JMP SHORT PathOfEx.006CC5B1
	$+320    006CC559     8B87 3C010000          MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$+326    006CC55F     83B8 0C400000 00       CMP DWORD PTR DS:[EAX+0x400C], 0x0
	$+32D    006CC566     76 3D                  JBE SHORT PathOfEx.006CC5A5
	$+32F    006CC568     83B8 0C400000 01       CMP DWORD PTR DS:[EAX+0x400C], 0x1
	$+336    006CC56F     74 34                  JE SHORT PathOfEx.006CC5A5
	$+338    006CC571     8B87 D0180000          MOV EAX, DWORD PTR DS:[EDI+0x18D0]
	$+33E    006CC577     3B87 BC180000          CMP EAX, DWORD PTR DS:[EDI+0x18BC]
	$+344    006CC57D     74 32                  JE SHORT PathOfEx.006CC5B1
	$+346    006CC57F     8B4D E8                MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+349    006CC582     85C9                   TEST ECX, ECX
	$+34B    006CC584     0F94C0                 SETE AL
	$+34E    006CC587     34 01                  XOR AL, 0x1
	$+350    006CC589     74 26                  JE SHORT PathOfEx.006CC5B1
	$+352    006CC58B     66:8B41 02             MOV AX, WORD PTR DS:[ECX+0x2]
	$+356    006CC58F     66:3B05 0CD70001       CMP AX, WORD PTR DS:[0x100D70C]
	$+35D    006CC596     75 19                  JNZ SHORT PathOfEx.006CC5B1
	$+35F    006CC598     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+365    006CC59E     E8 AD7BFBFF            CALL PathOfEx.00684150
	$+36A    006CC5A3     EB 0C                  JMP SHORT PathOfEx.006CC5B1
	$+36C    006CC5A5     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$+36F    006CC5A8     8BCF                   MOV ECX, EDI
	$+371    006CC5AA     50                     PUSH EAX
	$+372    006CC5AB     56                     PUSH ESI
	$+373    006CC5AC     E8 6F4D0000            CALL PathOfEx.006D1320
	$+378    006CC5B1     8D4D E8                LEA ECX, DWORD PTR SS:[EBP-0x18]
	$+37B    006CC5B4     C645 FC 02             MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+37F    006CC5B8     E8 F3E3D9FF            CALL PathOfEx.0046A9B0
	$+384    006CC5BD     8BCF                   MOV ECX, EDI
	$+386    006CC5BF     E8 0C4A0000            CALL PathOfEx.006D0FD0
	$+38B    006CC5C4     8D4D DC                LEA ECX, DWORD PTR SS:[EBP-0x24]
	$+38E    006CC5C7     E8 E4E3D9FF            CALL PathOfEx.0046A9B0
	$+393    006CC5CC     5F                     POP EDI                                    ; 22FA24B0
	$+394    006CC5CD     5E                     POP ESI                                    ; 22FA24B0
	$+395    006CC5CE     5B                     POP EBX                                    ; 22FA24B0
	$+396    006CC5CF     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+399    006CC5D2     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+3A0    006CC5D9     8BE5                   MOV ESP, EBP
	$+3A2    006CC5DB     5D                     POP EBP                                    ; 22FA24B0
	$+3A3    006CC5DC     C2 1000                RETN 0x10
	$+3A6    006CC5DF     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+3AA    006CC5E3     0F84 EB050000          JE PathOfEx.006CCBD4
	$+3B0    006CC5E9     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+3B4    006CC5ED     0F85 E1050000          JNZ PathOfEx.006CCBD4
	$+3BA    006CC5F3     8B87 D8180000          MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$+3C0    006CC5F9     8B80 000A0000          MOV EAX, DWORD PTR DS:[EAX+0xA00]
	$+3C6    006CC5FF     85C0                   TEST EAX, EAX
	$+3C8    006CC601     0F84 D8000000          JE PathOfEx.006CC6DF
	$+3CE    006CC607     8A80 54070000          MOV AL, BYTE PTR DS:[EAX+0x754]
	$+3D4    006CC60D     84C0                   TEST AL, AL
	$+3D6    006CC60F     0F85 CA000000          JNZ PathOfEx.006CC6DF
	$+3DC    006CC615     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+3E2    006CC61B     8D45 E8                LEA EAX, DWORD PTR SS:[EBP-0x18]
	$+3E5    006CC61E     6A 0E                  PUSH 0xE
	$+3E7    006CC620     50                     PUSH EAX
	$+3E8    006CC621     81C1 EC3C0000          ADD ECX, 0x3CEC
	$+3EE    006CC627     E8 24ECE3FF            CALL PathOfEx.0050B250
	$+3F3    006CC62C     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+3F9    006CC632     8D45 DC                LEA EAX, DWORD PTR SS:[EBP-0x24]
	$+3FC    006CC635     6A 0F                  PUSH 0xF
	$+3FE    006CC637     50                     PUSH EAX
	$+3FF    006CC638     81C1 EC3C0000          ADD ECX, 0x3CEC
	$+405    006CC63E     E8 0DECE3FF            CALL PathOfEx.0050B250
	$+40A    006CC643     8B4D E8                MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+40D    006CC646     6A 00                  PUSH 0x0
	$+40F    006CC648     6A 00                  PUSH 0x0
	$+411    006CC64A     E8 E1250800            CALL PathOfEx.0074EC30
	$+416    006CC64F     85C0                   TEST EAX, EAX
	$+418    006CC651     75 12                  JNZ SHORT PathOfEx.006CC665
	$+41A    006CC653     8B4D DC                MOV ECX, DWORD PTR SS:[EBP-0x24]
	$+41D    006CC656     50                     PUSH EAX
	$+41E    006CC657     50                     PUSH EAX
	$+41F    006CC658     E8 D3250800            CALL PathOfEx.0074EC30
	$+424    006CC65D     85C0                   TEST EAX, EAX
	$+426    006CC65F     75 04                  JNZ SHORT PathOfEx.006CC665
	$+428    006CC661     32DB                   XOR BL, BL
	$+42A    006CC663     EB 02                  JMP SHORT PathOfEx.006CC667
	$+42C    006CC665     B3 01                  MOV BL, 0x1
	$+42E    006CC667     8D4D DC                LEA ECX, DWORD PTR SS:[EBP-0x24]
	$+431    006CC66A     E8 41E3D9FF            CALL PathOfEx.0046A9B0
	$+436    006CC66F     8D4D E8                LEA ECX, DWORD PTR SS:[EBP-0x18]
	$+439    006CC672     E8 39E3D9FF            CALL PathOfEx.0046A9B0
	$+43E    006CC677     84DB                   TEST BL, BL
	$+440    006CC679     75 64                  JNZ SHORT PathOfEx.006CC6DF
	$+442    006CC67B     68 A0DCE600            PUSH PathOfEx.00E6DCA0
	$+447    006CC680     8D4D B0                LEA ECX, DWORD PTR SS:[EBP-0x50]
	$+44A    006CC683     E8 2817DAFF            CALL PathOfEx.0046DDB0
	$+44F    006CC688     BA 59000000            MOV EDX, 0x59
	$+454    006CC68D     C745 FC 05000000       MOV DWORD PTR SS:[EBP-0x4], 0x5
	$+45B    006CC694     8D4D 80                LEA ECX, DWORD PTR SS:[EBP-0x80]
	$+45E    006CC697     E8 14F30400            CALL PathOfEx.0071B9B0
	$+463    006CC69C     6A 00                  PUSH 0x0
	$+465    006CC69E     6A 00                  PUSH 0x0
	$+467    006CC6A0     C645 FC 06             MOV BYTE PTR SS:[EBP-0x4], 0x6
	$+46B    006CC6A4     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+471    006CC6AA     50                     PUSH EAX
	$+472    006CC6AB     8D45 B0                LEA EAX, DWORD PTR SS:[EBP-0x50]
	$+475    006CC6AE     50                     PUSH EAX
	$+476    006CC6AF     8B89 A8090000          MOV ECX, DWORD PTR DS:[ECX+0x9A8]
	$+47C    006CC6B5     6A 00                  PUSH 0x0
	$+47E    006CC6B7     E8 94BEEBFF            CALL PathOfEx.00588550
	$+483    006CC6BC     8D4D 80                LEA ECX, DWORD PTR SS:[EBP-0x80]
	$+486    006CC6BF     E8 3C10DAFF            CALL PathOfEx.0046D700
	$+48B    006CC6C4     8D4D B0                LEA ECX, DWORD PTR SS:[EBP-0x50]
	$+48E    006CC6C7     E8 3410DAFF            CALL PathOfEx.0046D700
	$+493    006CC6CC     5F                     POP EDI                                    ; 22FA24B0
	$+494    006CC6CD     5E                     POP ESI                                    ; 22FA24B0
	$+495    006CC6CE     5B                     POP EBX                                    ; 22FA24B0
	$+496    006CC6CF     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+499    006CC6D2     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+4A0    006CC6D9     8BE5                   MOV ESP, EBP
	$+4A2    006CC6DB     5D                     POP EBP                                    ; 22FA24B0
	$+4A3    006CC6DC     C2 1000                RETN 0x10
	$+4A6    006CC6DF     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+4AC    006CC6E5     E8 D6BD2000            CALL PathOfEx.008D84C0
	$+4B1    006CC6EA     5F                     POP EDI                                    ; 22FA24B0
	$+4B2    006CC6EB     5E                     POP ESI                                    ; 22FA24B0
	$+4B3    006CC6EC     5B                     POP EBX                                    ; 22FA24B0
	$+4B4    006CC6ED     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+4B7    006CC6F0     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+4BE    006CC6F7     8BE5                   MOV ESP, EBP
	$+4C0    006CC6F9     5D                     POP EBP                                    ; 22FA24B0
	$+4C1    006CC6FA     C2 1000                RETN 0x10
	$+4C4    006CC6FD     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+4C8    006CC701     0F84 CD040000          JE PathOfEx.006CCBD4
	$+4CE    006CC707     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+4D2    006CC70B     0F85 C3040000          JNZ PathOfEx.006CCBD4
	$+4D8    006CC711     8B87 D8180000          MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$+4DE    006CC717     8B88 68090000          MOV ECX, DWORD PTR DS:[EAX+0x968]
	$+4E4    006CC71D     FFB1 70090000          PUSH DWORD PTR DS:[ECX+0x970]              ; qbclient.0EBB6FF3
	$+4EA    006CC723     E8 48B3F5FF            CALL PathOfEx.00627A70
	$+4EF    006CC728     5F                     POP EDI                                    ; 22FA24B0
	$+4F0    006CC729     5E                     POP ESI                                    ; 22FA24B0
	$+4F1    006CC72A     5B                     POP EBX                                    ; 22FA24B0
	$+4F2    006CC72B     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+4F5    006CC72E     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+4FC    006CC735     8BE5                   MOV ESP, EBP
	$+4FE    006CC737     5D                     POP EBP                                    ; 22FA24B0
	$+4FF    006CC738     C2 1000                RETN 0x10
	$+502    006CC73B     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+506    006CC73F     0F84 8F040000          JE PathOfEx.006CCBD4
	$+50C    006CC745     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+510    006CC749     0F85 85040000          JNZ PathOfEx.006CCBD4
	$+516    006CC74F     8BB7 34010000          MOV ESI, DWORD PTR DS:[EDI+0x134]
	$+51C    006CC755     8BCE                   MOV ECX, ESI
	$+51E    006CC757     E8 B41C0F00            CALL PathOfEx.007BE410
	$+523    006CC75C     84C0                   TEST AL, AL
	$+525    006CC75E     8BCE                   MOV ECX, ESI
	$+527    006CC760     0F94C0                 SETE AL
	$+52A    006CC763     0FB6C0                 MOVZX EAX, AL
	$+52D    006CC766     50                     PUSH EAX
	$+52E    006CC767     E8 D41C0F00            CALL PathOfEx.007BE440
	$+533    006CC76C     8B87 D8180000          MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$+539    006CC772     8B80 240A0000          MOV EAX, DWORD PTR DS:[EAX+0xA24]
	$+53F    006CC778     85C0                   TEST EAX, EAX
	$+541    006CC77A     74 14                  JE SHORT PathOfEx.006CC790
	$+543    006CC77C     80B8 54070000 00       CMP BYTE PTR DS:[EAX+0x754], 0x0
	$+54A    006CC783     75 0B                  JNZ SHORT PathOfEx.006CC790
	$+54C    006CC785     8B8F 34010000          MOV ECX, DWORD PTR DS:[EDI+0x134]
	$+552    006CC78B     E8 C03E0F00            CALL PathOfEx.007C0650
	$+557    006CC790     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+55D    006CC796     E8 8522FBFF            CALL PathOfEx.0067EA20
	$+562    006CC79B     5F                     POP EDI                                    ; 22FA24B0
	$+563    006CC79C     5E                     POP ESI                                    ; 22FA24B0
	$+564    006CC79D     5B                     POP EBX                                    ; 22FA24B0
	$+565    006CC79E     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+568    006CC7A1     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+56F    006CC7A8     8BE5                   MOV ESP, EBP
	$+571    006CC7AA     5D                     POP EBP                                    ; 22FA24B0
	$+572    006CC7AB     C2 1000                RETN 0x10
	$+575    006CC7AE     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+579    006CC7B2     0F84 1C040000          JE PathOfEx.006CCBD4
	$+57F    006CC7B8     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+583    006CC7BC     0F85 12040000          JNZ PathOfEx.006CCBD4
	$+589    006CC7C2     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+58F    006CC7C8     E8 13D3FAFF            CALL PathOfEx.00679AE0
	$+594    006CC7CD     5F                     POP EDI                                    ; 22FA24B0
	$+595    006CC7CE     5E                     POP ESI                                    ; 22FA24B0
	$+596    006CC7CF     5B                     POP EBX                                    ; 22FA24B0
	$+597    006CC7D0     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+59A    006CC7D3     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+5A1    006CC7DA     8BE5                   MOV ESP, EBP
	$+5A3    006CC7DC     5D                     POP EBP                                    ; 22FA24B0
	$+5A4    006CC7DD     C2 1000                RETN 0x10
	$+5A7    006CC7E0     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+5AB    006CC7E4     0F84 EA030000          JE PathOfEx.006CCBD4
	$+5B1    006CC7EA     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+5B5    006CC7EE     0F85 E0030000          JNZ PathOfEx.006CCBD4
	$+5BB    006CC7F4     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+5C1    006CC7FA     E8 31D2FAFF            CALL PathOfEx.00679A30
	$+5C6    006CC7FF     5F                     POP EDI                                    ; 22FA24B0
	$+5C7    006CC800     5E                     POP ESI                                    ; 22FA24B0
	$+5C8    006CC801     5B                     POP EBX                                    ; 22FA24B0
	$+5C9    006CC802     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+5CC    006CC805     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+5D3    006CC80C     8BE5                   MOV ESP, EBP
	$+5D5    006CC80E     5D                     POP EBP                                    ; 22FA24B0
	$+5D6    006CC80F     C2 1000                RETN 0x10
	$+5D9    006CC812     8B97 D8180000          MOV EDX, DWORD PTR DS:[EDI+0x18D8]
	$+5DF    006CC818     8B8A 240A0000          MOV ECX, DWORD PTR DS:[EDX+0xA24]
	$+5E5    006CC81E     85C9                   TEST ECX, ECX
	$+5E7    006CC820     0F84 AE030000          JE PathOfEx.006CCBD4
	$+5ED    006CC826     E8 A536F2FF            CALL PathOfEx.005EFED0
	$+5F2    006CC82B     84C0                   TEST AL, AL
	$+5F4    006CC82D     0F85 A1030000          JNZ PathOfEx.006CCBD4
	$+5FA    006CC833     3845 0C                CMP BYTE PTR SS:[EBP+0xC], AL
	$+5FD    006CC836     0F84 98030000          JE PathOfEx.006CCBD4
	$+603    006CC83C     3845 10                CMP BYTE PTR SS:[EBP+0x10], AL
	$+606    006CC83F     0F85 8F030000          JNZ PathOfEx.006CCBD4
	$+60C    006CC845     51                     PUSH ECX
	$+60D    006CC846     8BCA                   MOV ECX, EDX
	$+60F    006CC848     E8 631BFBFF            CALL PathOfEx.0067E3B0
	$+614    006CC84D     5F                     POP EDI                                    ; 22FA24B0
	$+615    006CC84E     5E                     POP ESI                                    ; 22FA24B0
	$+616    006CC84F     5B                     POP EBX                                    ; 22FA24B0
	$+617    006CC850     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+61A    006CC853     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+621    006CC85A     8BE5                   MOV ESP, EBP
	$+623    006CC85C     5D                     POP EBP                                    ; 22FA24B0
	$+624    006CC85D     C2 1000                RETN 0x10
	$+627    006CC860     84C9                   TEST CL, CL
	$+629    006CC862     0F85 6C030000          JNZ PathOfEx.006CCBD4
	$+62F    006CC868     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+632    006CC86B     0F84 63030000          JE PathOfEx.006CCBD4
	$+638    006CC871     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+63B    006CC874     0F85 5A030000          JNZ PathOfEx.006CCBD4
	$+641    006CC87A     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+647    006CC880     FFB1 2C0A0000          PUSH DWORD PTR DS:[ECX+0xA2C]              ; user32.75496D3A
	$+64D    006CC886     E8 251BFBFF            CALL PathOfEx.0067E3B0
	$+652    006CC88B     5F                     POP EDI                                    ; 22FA24B0
	$+653    006CC88C     5E                     POP ESI                                    ; 22FA24B0
	$+654    006CC88D     5B                     POP EBX                                    ; 22FA24B0
	$+655    006CC88E     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+658    006CC891     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+65F    006CC898     8BE5                   MOV ESP, EBP
	$+661    006CC89A     5D                     POP EBP                                    ; 22FA24B0
	$+662    006CC89B     C2 1000                RETN 0x10
	$+665    006CC89E     84C9                   TEST CL, CL
	$+667    006CC8A0     0F85 2E030000          JNZ PathOfEx.006CCBD4
	$+66D    006CC8A6     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+670    006CC8A9     0F84 25030000          JE PathOfEx.006CCBD4
	$+676    006CC8AF     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+679    006CC8B2     0F85 1C030000          JNZ PathOfEx.006CCBD4
	$+67F    006CC8B8     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+685    006CC8BE     FFB1 140A0000          PUSH DWORD PTR DS:[ECX+0xA14]
	$+68B    006CC8C4     E8 E71AFBFF            CALL PathOfEx.0067E3B0
	$+690    006CC8C9     5F                     POP EDI                                    ; 22FA24B0
	$+691    006CC8CA     5E                     POP ESI                                    ; 22FA24B0
	$+692    006CC8CB     5B                     POP EBX                                    ; 22FA24B0
	$+693    006CC8CC     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+696    006CC8CF     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+69D    006CC8D6     8BE5                   MOV ESP, EBP
	$+69F    006CC8D8     5D                     POP EBP                                    ; 22FA24B0
	$+6A0    006CC8D9     C2 1000                RETN 0x10
	$+6A3    006CC8DC     84C9                   TEST CL, CL
	$+6A5    006CC8DE     0F85 F0020000          JNZ PathOfEx.006CCBD4
	$+6AB    006CC8E4     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+6AE    006CC8E7     0F84 E7020000          JE PathOfEx.006CCBD4
	$+6B4    006CC8ED     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+6B7    006CC8F0     0F85 DE020000          JNZ PathOfEx.006CCBD4
	$+6BD    006CC8F6     E8 D5493400            CALL PathOfEx.00A112D0
	$+6C2    006CC8FB     84C0                   TEST AL, AL
	$+6C4    006CC8FD     0F85 D1020000          JNZ PathOfEx.006CCBD4
	$+6CA    006CC903     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+6D0    006CC909     FFB1 200A0000          PUSH DWORD PTR DS:[ECX+0xA20]
	$+6D6    006CC90F     E8 9C1AFBFF            CALL PathOfEx.0067E3B0
	$+6DB    006CC914     5F                     POP EDI                                    ; 22FA24B0
	$+6DC    006CC915     5E                     POP ESI                                    ; 22FA24B0
	$+6DD    006CC916     5B                     POP EBX                                    ; 22FA24B0
	$+6DE    006CC917     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+6E1    006CC91A     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+6E8    006CC921     8BE5                   MOV ESP, EBP
	$+6EA    006CC923     5D                     POP EBP                                    ; 22FA24B0
	$+6EB    006CC924     C2 1000                RETN 0x10
	$+6EE    006CC927     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+6F2    006CC92B     0F84 A3020000          JE PathOfEx.006CCBD4
	$+6F8    006CC931     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+6FC    006CC935     0F85 99020000          JNZ PathOfEx.006CCBD4
	$+702    006CC93B     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+708    006CC941     FFB1 000A0000          PUSH DWORD PTR DS:[ECX+0xA00]              ; user32.754962FA
	$+70E    006CC947     E8 641AFBFF            CALL PathOfEx.0067E3B0
	$+713    006CC94C     5F                     POP EDI                                    ; 22FA24B0
	$+714    006CC94D     5E                     POP ESI                                    ; 22FA24B0
	$+715    006CC94E     5B                     POP EBX                                    ; 22FA24B0
	$+716    006CC94F     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+719    006CC952     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+720    006CC959     8BE5                   MOV ESP, EBP
	$+722    006CC95B     5D                     POP EBP                                    ; 22FA24B0
	$+723    006CC95C     C2 1000                RETN 0x10
	$+726    006CC95F     8B87 3C010000          MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$+72C    006CC965     80B8 693B0000 00       CMP BYTE PTR DS:[EAX+0x3B69], 0x0
	$+733    006CC96C     0F85 62020000          JNZ PathOfEx.006CCBD4
	$+739    006CC972     84C9                   TEST CL, CL
	$+73B    006CC974     0F85 5A020000          JNZ PathOfEx.006CCBD4
	$+741    006CC97A     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+744    006CC97D     0F84 51020000          JE PathOfEx.006CCBD4
	$+74A    006CC983     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+74D    006CC986     0F85 48020000          JNZ PathOfEx.006CCBD4
	$+753    006CC98C     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+759    006CC992     FFB1 300A0000          PUSH DWORD PTR DS:[ECX+0xA30]
	$+75F    006CC998     E8 131AFBFF            CALL PathOfEx.0067E3B0
	$+764    006CC99D     5F                     POP EDI                                    ; 22FA24B0
	$+765    006CC99E     5E                     POP ESI                                    ; 22FA24B0
	$+766    006CC99F     5B                     POP EBX                                    ; 22FA24B0
	$+767    006CC9A0     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+76A    006CC9A3     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+771    006CC9AA     8BE5                   MOV ESP, EBP
	$+773    006CC9AC     5D                     POP EBP                                    ; 22FA24B0
	$+774    006CC9AD     C2 1000                RETN 0x10
	$+777    006CC9B0     84C9                   TEST CL, CL
	$+779    006CC9B2     0F85 1C020000          JNZ PathOfEx.006CCBD4
	$+77F    006CC9B8     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+782    006CC9BB     0F84 13020000          JE PathOfEx.006CCBD4
	$+788    006CC9C1     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+78B    006CC9C4     0F85 0A020000          JNZ PathOfEx.006CCBD4
	$+791    006CC9CA     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+797    006CC9D0     E8 7B1AFBFF            CALL PathOfEx.0067E450
	$+79C    006CC9D5     5F                     POP EDI                                    ; 22FA24B0
	$+79D    006CC9D6     5E                     POP ESI                                    ; 22FA24B0
	$+79E    006CC9D7     5B                     POP EBX                                    ; 22FA24B0
	$+79F    006CC9D8     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+7A2    006CC9DB     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+7A9    006CC9E2     8BE5                   MOV ESP, EBP
	$+7AB    006CC9E4     5D                     POP EBP                                    ; 22FA24B0
	$+7AC    006CC9E5     C2 1000                RETN 0x10
	$+7AF    006CC9E8     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+7B3    006CC9EC     0F84 E2010000          JE PathOfEx.006CCBD4
	$+7B9    006CC9F2     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+7BD    006CC9F6     0F85 D8010000          JNZ PathOfEx.006CCBD4
	$+7C3    006CC9FC     8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+7C9    006CCA02     8B81 743B0000          MOV EAX, DWORD PTR DS:[ECX+0x3B74]
	$+7CF    006CCA08     3B81 783B0000          CMP EAX, DWORD PTR DS:[ECX+0x3B78]
	$+7D5    006CCA0E     0F84 C0010000          JE PathOfEx.006CCBD4
	$+7DB    006CCA14     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+7E1    006CCA1A     FFB1 280A0000          PUSH DWORD PTR DS:[ECX+0xA28]
	$+7E7    006CCA20     E8 8B19FBFF            CALL PathOfEx.0067E3B0
	$+7EC    006CCA25     5F                     POP EDI                                    ; 22FA24B0
	$+7ED    006CCA26     5E                     POP ESI                                    ; 22FA24B0
	$+7EE    006CCA27     5B                     POP EBX                                    ; 22FA24B0
	$+7EF    006CCA28     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+7F2    006CCA2B     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+7F9    006CCA32     8BE5                   MOV ESP, EBP
	$+7FB    006CCA34     5D                     POP EBP                                    ; 22FA24B0
	$+7FC    006CCA35     C2 1000                RETN 0x10
	$+7FF    006CCA38     84C9                   TEST CL, CL
	$+801    006CCA3A     0F85 94010000          JNZ PathOfEx.006CCBD4
	$+807    006CCA40     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+80A    006CCA43     0F84 8B010000          JE PathOfEx.006CCBD4
	$+810    006CCA49     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+813    006CCA4C     0F85 82010000          JNZ PathOfEx.006CCBD4
	$+819    006CCA52     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+81F    006CCA58     E8 B31AFBFF            CALL PathOfEx.0067E510
	$+824    006CCA5D     5F                     POP EDI                                    ; 22FA24B0
	$+825    006CCA5E     5E                     POP ESI                                    ; 22FA24B0
	$+826    006CCA5F     5B                     POP EBX                                    ; 22FA24B0
	$+827    006CCA60     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+82A    006CCA63     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+831    006CCA6A     8BE5                   MOV ESP, EBP
	$+833    006CCA6C     5D                     POP EBP                                    ; 22FA24B0
	$+834    006CCA6D     C2 1000                RETN 0x10
	$+837    006CCA70     84C9                   TEST CL, CL
	$+839    006CCA72     0F85 5C010000          JNZ PathOfEx.006CCBD4
	$+83F    006CCA78     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+842    006CCA7B     0F84 53010000          JE PathOfEx.006CCBD4
	$+848    006CCA81     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+84B    006CCA84     0F85 4A010000          JNZ PathOfEx.006CCBD4
	$+851    006CCA8A     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+857    006CCA90     FFB1 F8090000          PUSH DWORD PTR DS:[ECX+0x9F8]
	$+85D    006CCA96     E8 1519FBFF            CALL PathOfEx.0067E3B0
	$+862    006CCA9B     5F                     POP EDI                                    ; 22FA24B0
	$+863    006CCA9C     5E                     POP ESI                                    ; 22FA24B0
	$+864    006CCA9D     5B                     POP EBX                                    ; 22FA24B0
	$+865    006CCA9E     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+868    006CCAA1     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+86F    006CCAA8     8BE5                   MOV ESP, EBP
	$+871    006CCAAA     5D                     POP EBP                                    ; 22FA24B0
	$+872    006CCAAB     C2 1000                RETN 0x10
	$+875    006CCAAE     84C9                   TEST CL, CL
	$+877    006CCAB0     0F85 1E010000          JNZ PathOfEx.006CCBD4
	$+87D    006CCAB6     384D 0C                CMP BYTE PTR SS:[EBP+0xC], CL
	$+880    006CCAB9     0F84 15010000          JE PathOfEx.006CCBD4
	$+886    006CCABF     384D 10                CMP BYTE PTR SS:[EBP+0x10], CL
	$+889    006CCAC2     0F85 0C010000          JNZ PathOfEx.006CCBD4
	$+88F    006CCAC8     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+895    006CCACE     6A 00                  PUSH 0x0
	$+897    006CCAD0     6A 00                  PUSH 0x0
	$+899    006CCAD2     6A 00                  PUSH 0x0
	$+89B    006CCAD4     E8 071DFBFF            CALL PathOfEx.0067E7E0
	$+8A0    006CCAD9     5F                     POP EDI                                    ; 22FA24B0
	$+8A1    006CCADA     5E                     POP ESI                                    ; 22FA24B0
	$+8A2    006CCADB     5B                     POP EBX                                    ; 22FA24B0
	$+8A3    006CCADC     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+8A6    006CCADF     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+8AD    006CCAE6     8BE5                   MOV ESP, EBP
	$+8AF    006CCAE8     5D                     POP EBP                                    ; 22FA24B0
	$+8B0    006CCAE9     C2 1000                RETN 0x10
	$+8B3    006CCAEC     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+8B7    006CCAF0     0F84 DE000000          JE PathOfEx.006CCBD4
	$+8BD    006CCAF6     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+8C1    006CCAFA     0F85 D4000000          JNZ PathOfEx.006CCBD4
	$+8C7    006CCB00     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+8CD    006CCB06     E8 65D0FAFF            CALL PathOfEx.00679B70
	$+8D2    006CCB0B     5F                     POP EDI                                    ; 22FA24B0
	$+8D3    006CCB0C     5E                     POP ESI                                    ; 22FA24B0
	$+8D4    006CCB0D     5B                     POP EBX                                    ; 22FA24B0
	$+8D5    006CCB0E     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+8D8    006CCB11     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+8DF    006CCB18     8BE5                   MOV ESP, EBP
	$+8E1    006CCB1A     5D                     POP EBP                                    ; 22FA24B0
	$+8E2    006CCB1B     C2 1000                RETN 0x10
	$+8E5    006CCB1E     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+8E9    006CCB22     0F84 AC000000          JE PathOfEx.006CCBD4
	$+8EF    006CCB28     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+8F3    006CCB2C     0F85 A2000000          JNZ PathOfEx.006CCBD4
	$+8F9    006CCB32     51                     PUSH ECX
	$+8FA    006CCB33     8B8F 001B0000          MOV ECX, DWORD PTR DS:[EDI+0x1B00]
	$+900    006CCB39     E8 528D2500            CALL PathOfEx.00925890
	$+905    006CCB3E     83C4 04                ADD ESP, 0x4
	$+908    006CCB41     8D4D 98                LEA ECX, DWORD PTR SS:[EBP-0x68]
	$+90B    006CCB44     68 A0DCE600            PUSH PathOfEx.00E6DCA0
	$+910    006CCB49     E8 6212DAFF            CALL PathOfEx.0046DDB0
	$+915    006CCB4E     BA 52030000            MOV EDX, 0x352
	$+91A    006CCB53     C745 FC 07000000       MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+921    006CCB5A     8D8D 68FFFFFF          LEA ECX, DWORD PTR SS:[EBP-0x98]
	$+927    006CCB60     E8 4BEE0400            CALL PathOfEx.0071B9B0
	$+92C    006CCB65     6A 00                  PUSH 0x0
	$+92E    006CCB67     6A 00                  PUSH 0x0
	$+930    006CCB69     C645 FC 08             MOV BYTE PTR SS:[EBP-0x4], 0x8
	$+934    006CCB6D     8B8F D8180000          MOV ECX, DWORD PTR DS:[EDI+0x18D8]
	$+93A    006CCB73     50                     PUSH EAX
	$+93B    006CCB74     8D45 98                LEA EAX, DWORD PTR SS:[EBP-0x68]
	$+93E    006CCB77     50                     PUSH EAX
	$+93F    006CCB78     8B89 A8090000          MOV ECX, DWORD PTR DS:[ECX+0x9A8]
	$+945    006CCB7E     6A 00                  PUSH 0x0
	$+947    006CCB80     E8 CBB9EBFF            CALL PathOfEx.00588550
	$+94C    006CCB85     8D8D 68FFFFFF          LEA ECX, DWORD PTR SS:[EBP-0x98]
	$+952    006CCB8B     E8 700BDAFF            CALL PathOfEx.0046D700
	$+957    006CCB90     8D4D 98                LEA ECX, DWORD PTR SS:[EBP-0x68]
	$+95A    006CCB93     E8 680BDAFF            CALL PathOfEx.0046D700
	$+95F    006CCB98     5F                     POP EDI                                    ; 22FA24B0
	$+960    006CCB99     5E                     POP ESI                                    ; 22FA24B0
	$+961    006CCB9A     5B                     POP EBX                                    ; 22FA24B0
	$+962    006CCB9B     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+965    006CCB9E     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+96C    006CCBA5     8BE5                   MOV ESP, EBP
	$+96E    006CCBA7     5D                     POP EBP                                    ; 22FA24B0
	$+96F    006CCBA8     C2 1000                RETN 0x10
	$+972    006CCBAB     807D 0C 00             CMP BYTE PTR SS:[EBP+0xC], 0x0
	$+976    006CCBAF     74 23                  JE SHORT PathOfEx.006CCBD4
	$+978    006CCBB1     807D 10 00             CMP BYTE PTR SS:[EBP+0x10], 0x0
	$+97C    006CCBB5     75 1D                  JNZ SHORT PathOfEx.006CCBD4
	$+97E    006CCBB7     8BCF                   MOV ECX, EDI
	$+980    006CCBB9     E8 322F0000            CALL PathOfEx.006CFAF0
	$+985    006CCBBE     5F                     POP EDI                                    ; 22FA24B0
	$+986    006CCBBF     5E                     POP ESI                                    ; 22FA24B0
	$+987    006CCBC0     5B                     POP EBX                                    ; 22FA24B0
	$+988    006CCBC1     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+98B    006CCBC4     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+992    006CCBCB     8BE5                   MOV ESP, EBP
	$+994    006CCBCD     5D                     POP EBP                                    ; 22FA24B0
	$+995    006CCBCE     C2 1000                RETN 0x10
	$+998    006CCBD1     C602 00                MOV BYTE PTR DS:[EDX], 0x0
	$+99B    006CCBD4     8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+99E    006CCBD7     5F                     POP EDI                                    ; 22FA24B0
	$+99F    006CCBD8     5E                     POP ESI                                    ; 22FA24B0
	$+9A0    006CCBD9     5B                     POP EBX                                    ; 22FA24B0
	$+9A1    006CCBDA     64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+9A8    006CCBE1     8BE5                   MOV ESP, EBP
	$+9AA    006CCBE3     5D                     POP EBP                                    ; 22FA24B0
	$+9AB    006CCBE4     C2 1000                RETN 0x10
	$+9AE    006CCBE7     90                     NOP
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??????????8d????508d????50e8????????c6??????8b????e8", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *(pt_byte*)addr_tmp;
	assert(dw_tmp == 0x8b);
	dw_tmp = *(pt_dword*)((pt_byte*)addr_tmp + 2);
	assert(dw_tmp > 0x1000 && dw_tmp < 0x3000);
	ofs << "pt_cdword	g_o_get_down_inventory_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	addr_tmp = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_byte*)addr_tmp + 0xe);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *(pt_byte*)addr_tmp;
	assert(dw_tmp == 0xff);
	dw_tmp = *(pt_dword*)((pt_byte*)addr_tmp + 4 + 2);
	assert(dw_tmp > 0x500 && dw_tmp < 0x1500);
	ofs << "pt_cdword	g_o_get_inventory_skill_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	addr_tmp = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_byte*)addr_tmp + 0xe);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *(pt_byte*)addr_tmp;
	assert(dw_tmp == 0xff);
	dw_tmp = *(pt_dword*)((pt_byte*)addr_tmp + 0xa + 3);
	assert(dw_tmp > 0x500 && dw_tmp < 0x1500);
	ofs << "pt_cdword	g_o_inventory_skill_item_array_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	addr_tmp = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_byte*)addr_tmp + 0x11);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *(pt_byte*)addr_tmp;
	assert(dw_tmp == 0xff);
	dw_tmp = *(pt_dword*)((pt_byte*)addr_tmp + 4 + 2);
	assert(dw_tmp > 0x500 && dw_tmp < 0x1500);
	ofs << "pt_cdword	g_o_get_inventory_skill_item_info_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-213    006D070F     CC                         INT3
	$-212    006D0710     55                         PUSH EBP
	$-211    006D0711     8BEC                       MOV EBP, ESP
	$-20F    006D0713     6A FF                      PUSH -0x1
	$-20D    006D0715     68 9E24D400                PUSH PathOfEx.00D4249E
	$-208    006D071A     64:A1 00000000             MOV EAX, DWORD PTR FS:[0]
	$-202    006D0720     50                         PUSH EAX
	$-201    006D0721     64:8925 00000000           MOV DWORD PTR FS:[0], ESP
	$-1FA    006D0728     83EC 50                    SUB ESP, 0x50
	$-1F7    006D072B     53                         PUSH EBX
	$-1F6    006D072C     33DB                       XOR EBX, EBX
	$-1F4    006D072E     56                         PUSH ESI
	$-1F3    006D072F     895D D8                    MOV DWORD PTR SS:[EBP-0x28], EBX
	$-1F0    006D0732     57                         PUSH EDI
	$-1EF    006D0733     8BF9                       MOV EDI, ECX
	$-1ED    006D0735     381D 14D91401              CMP BYTE PTR DS:[0x114D914], BL
	$-1E7    006D073B     0F85 AD000000              JNZ PathOfEx.006D07EE
	$-1E1    006D0741     8BB7 34010000              MOV ESI, DWORD PTR DS:[EDI+0x134]          ; PathOfEx.00A11856
	$-1DB    006D0747     8D45 F3                    LEA EAX, DWORD PTR SS:[EBP-0xD]
	$-1D8    006D074A     50                         PUSH EAX
	$-1D7    006D074B     68 FC49E900                PUSH PathOfEx.00E949FC                     ; attack_in_place_key_stops_move
	$-1D2    006D0750     68 08DEE600                PUSH PathOfEx.00E6DE08                     ; UI
	$-1CD    006D0755     8B8E 80030000              MOV ECX, DWORD PTR DS:[ESI+0x380]
	$-1C7    006D075B     885D F3                    MOV BYTE PTR SS:[EBP-0xD], BL
	$-1C4    006D075E     E8 FD5A0F00                CALL PathOfEx.007C6260
	$-1BF    006D0763     385D F3                    CMP BYTE PTR SS:[EBP-0xD], BL
	$-1BC    006D0766     0F84 82000000              JE PathOfEx.006D07EE
	$-1B6    006D076C     8D45 E8                    LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-1B3    006D076F     895D E8                    MOV DWORD PTR SS:[EBP-0x18], EBX
	$-1B0    006D0772     50                         PUSH EAX
	$-1AF    006D0773     6A 0D                      PUSH 0xD
	$-1AD    006D0775     8BCE                       MOV ECX, ESI
	$-1AB    006D0777     E8 B4EC0E00                CALL PathOfEx.007BF430
	$-1A6    006D077C     8B75 E8                    MOV ESI, DWORD PTR SS:[EBP-0x18]
	$-1A3    006D077F     56                         PUSH ESI
	$-1A2    006D0780     FF15 38C4D800              CALL NEAR DWORD PTR DS:[0xD8C438]
	$-19C    006D0786     0FB7C0                     MOVZX EAX, AX
	$-199    006D0789     85F6                       TEST ESI, ESI
	$-197    006D078B     74 61                      JE SHORT PathOfEx.006D07EE
	$-195    006D078D     66:85C0                    TEST AX, AX
	$-192    006D0790     79 5C                      JNS SHORT PathOfEx.006D07EE
	$-190    006D0792     8B87 3C010000              MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$-18A    006D0798     C605 14D91401 01           MOV BYTE PTR DS:[0x114D914], 0x1
	$-183    006D079F     85C0                       TEST EAX, EAX
	$-181    006D07A1     74 7B                      JE SHORT PathOfEx.006D081E
	$-17F    006D07A3     8B80 943A0000              MOV EAX, DWORD PTR DS:[EAX+0x3A94]
	$-179    006D07A9     8B88 C4010000              MOV ECX, DWORD PTR DS:[EAX+0x1C4]
	$-173    006D07AF     E8 DCD3DCFF                CALL PathOfEx.0049DB90
	$-16E    006D07B4     85C0                       TEST EAX, EAX
	$-16C    006D07B6     74 66                      JE SHORT PathOfEx.006D081E
	$-16A    006D07B8     8B48 30                    MOV ECX, DWORD PTR DS:[EAX+0x30]
	$-167    006D07BB     85C9                       TEST ECX, ECX
	$-165    006D07BD     74 5F                      JE SHORT PathOfEx.006D081E
	$-163    006D07BF     E8 1C414100                CALL PathOfEx.00AE48E0
	$-15E    006D07C4     8B0D 0CD70001              MOV ECX, DWORD PTR DS:[0x100D70C]
	$-158    006D07CA     66:3BC1                    CMP AX, CX
	$-155    006D07CD     75 4F                      JNZ SHORT PathOfEx.006D081E
	$-153    006D07CF     6A 01                      PUSH 0x1
	$-151    006D07D1     0FB7C1                     MOVZX EAX, CX
	$-14E    006D07D4     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-148    006D07DA     53                         PUSH EBX
	$-147    006D07DB     50                         PUSH EAX
	$-146    006D07DC     E8 FF4D2000                CALL PathOfEx.008D55E0
	$-141    006D07E1     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-13B    006D07E7     E8 24512000                CALL PathOfEx.008D5910
	$-136    006D07EC     EB 30                      JMP SHORT PathOfEx.006D081E
	$-134    006D07EE     8B8F 34010000              MOV ECX, DWORD PTR DS:[EDI+0x134]          ; PathOfEx.00A11856
	$-12E    006D07F4     8D45 E8                    LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-12B    006D07F7     50                         PUSH EAX
	$-12A    006D07F8     6A 0D                      PUSH 0xD
	$-128    006D07FA     895D E8                    MOV DWORD PTR SS:[EBP-0x18], EBX
	$-125    006D07FD     E8 2EEC0E00                CALL PathOfEx.007BF430
	$-120    006D0802     8B75 E8                    MOV ESI, DWORD PTR SS:[EBP-0x18]
	$-11D    006D0805     56                         PUSH ESI
	$-11C    006D0806     FF15 38C4D800              CALL NEAR DWORD PTR DS:[0xD8C438]
	$-116    006D080C     0FB7C0                     MOVZX EAX, AX
	$-113    006D080F     85F6                       TEST ESI, ESI
	$-111    006D0811     74 05                      JE SHORT PathOfEx.006D0818
	$-10F    006D0813     66:85C0                    TEST AX, AX
	$-10C    006D0816     78 06                      JS SHORT PathOfEx.006D081E
	$-10A    006D0818     881D 14D91401              MOV BYTE PTR DS:[0x114D914], BL
	$-104    006D081E     8B87 64150000              MOV EAX, DWORD PTR DS:[EDI+0x1564]
	$-FE     006D0824     3B87 68150000              CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-F8     006D082A     75 13                      JNZ SHORT PathOfEx.006D083F
	$-F6     006D082C     33C0                       XOR EAX, EAX
	$-F4     006D082E     8B4D F4                    MOV ECX, DWORD PTR SS:[EBP-0xC]            ; apisetsc.0004086C
	$-F1     006D0831     64:890D 00000000           MOV DWORD PTR FS:[0], ECX
	$-EA     006D0838     5F                         POP EDI
	$-E9     006D0839     5E                         POP ESI
	$-E8     006D083A     5B                         POP EBX
	$-E7     006D083B     8BE5                       MOV ESP, EBP
	$-E5     006D083D     5D                         POP EBP
	$-E4     006D083E     C3                         RETN
	$-E3     006D083F     399F 780C0000              CMP DWORD PTR DS:[EDI+0xC78], EBX
	$-DD     006D0845     75 28                      JNZ SHORT PathOfEx.006D086F
	$-DB     006D0847     399F D8180000              CMP DWORD PTR DS:[EDI+0x18D8], EBX
	$-D5     006D084D     74 20                      JE SHORT PathOfEx.006D086F
	$-D3     006D084F     8D4D D0                    LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-D0     006D0852     E8 89CEDAFF                CALL PathOfEx.0047D6E0
	$-CB     006D0857     8D4D C0                    LEA ECX, DWORD PTR SS:[EBP-0x40]
	$-C8     006D085A     51                         PUSH ECX
	$-C7     006D085B     8B08                       MOV ECX, DWORD PTR DS:[EAX]
	$-C5     006D085D     E8 AE5BFDFF                CALL PathOfEx.006A6410
	$-C0     006D0862     BB 03000000                MOV EBX, 0x3
	$-BB     006D0867     C645 F3 00                 MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-B7     006D086B     3938                       CMP DWORD PTR DS:[EAX], EDI
	$-B5     006D086D     74 04                      JE SHORT PathOfEx.006D0873
	$-B3     006D086F     C645 F3 01                 MOV BYTE PTR SS:[EBP-0xD], 0x1
	$-AF     006D0873     83CE FF                    OR ESI, -0x1
	$-AC     006D0876     F6C3 02                    TEST BL, 0x2
	$-A9     006D0879     74 39                      JE SHORT PathOfEx.006D08B4
	$-A7     006D087B     83E3 FD                    AND EBX, -0x3
	$-A4     006D087E     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-9D     006D0885     8B4D C4                    MOV ECX, DWORD PTR SS:[EBP-0x3C]
	$-9A     006D0888     894D E0                    MOV DWORD PTR SS:[EBP-0x20], ECX
	$-97     006D088B     85C9                       TEST ECX, ECX
	$-95     006D088D     74 1E                      JE SHORT PathOfEx.006D08AD
	$-93     006D088F     8BC6                       MOV EAX, ESI
	$-91     006D0891     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-8C     006D0896     75 15                      JNZ SHORT PathOfEx.006D08AD
	$-8A     006D0898     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$-88     006D089A     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$-86     006D089C     8B4D E0                    MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-83     006D089F     8BC6                       MOV EAX, ESI
	$-81     006D08A1     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-7C     006D08A6     75 05                      JNZ SHORT PathOfEx.006D08AD
	$-7A     006D08A8     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$-78     006D08AA     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-75     006D08AD     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-6E     006D08B4     F6C3 01                    TEST BL, 0x1
	$-6B     006D08B7     74 39                      JE SHORT PathOfEx.006D08F2
	$-69     006D08B9     83E3 FE                    AND EBX, -0x2
	$-66     006D08BC     C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-5F     006D08C3     8B4D D4                    MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$-5C     006D08C6     894D E0                    MOV DWORD PTR SS:[EBP-0x20], ECX
	$-59     006D08C9     85C9                       TEST ECX, ECX
	$-57     006D08CB     74 1E                      JE SHORT PathOfEx.006D08EB
	$-55     006D08CD     8BC6                       MOV EAX, ESI
	$-53     006D08CF     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-4E     006D08D4     75 15                      JNZ SHORT PathOfEx.006D08EB
	$-4C     006D08D6     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$-4A     006D08D8     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$-48     006D08DA     8B4D E0                    MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-45     006D08DD     8BC6                       MOV EAX, ESI
	$-43     006D08DF     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-3E     006D08E4     75 05                      JNZ SHORT PathOfEx.006D08EB
	$-3C     006D08E6     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$-3A     006D08E8     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-37     006D08EB     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-30     006D08F2     807D F3 00                 CMP BYTE PTR SS:[EBP-0xD], 0x0
	$-2C     006D08F6   ^ 0F85 30FFFFFF              JNZ PathOfEx.006D082C
	$-26     006D08FC     8B87 64150000              MOV EAX, DWORD PTR DS:[EDI+0x1564]
	$-20     006D0902     3B87 68150000              CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-1A     006D0908     75 18                      JNZ SHORT PathOfEx.006D0922
	$-18     006D090A     33C0                       XOR EAX, EAX
	$-16     006D090C     C745 D0 00000000           MOV DWORD PTR SS:[EBP-0x30], 0x0
	$-F      006D0913     C745 D4 00000000           MOV DWORD PTR SS:[EBP-0x2C], 0x0
	$-8      006D091A     8D4D D0                    LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-5      006D091D     83CB 0C                    OR EBX, 0xC
	$-2      006D0920     EB 1A                      JMP SHORT PathOfEx.006D093C
	$ ==>    006D0922     8B87 68150000              MOV EAX, DWORD PTR DS:[EDI+0x1568]         ; g_o_get_cur_skill_type_info_
	$+6      006D0928     8D4D B8                    LEA ECX, DWORD PTR SS:[EBP-0x48]
	$+9      006D092B     51                         PUSH ECX
	$+A      006D092C     8D48 F8                    LEA ECX, DWORD PTR DS:[EAX-0x8]
	$+D      006D092F     E8 9CE4EAFF                CALL PathOfEx.0057EDD0                     ; stCD_Skill_IdInfo的拷贝操作
	$+12     006D0934     8BC8                       MOV ECX, EAX
	$+14     006D0936     83CB 10                    OR EBX, 0x10
	$+17     006D0939     8B45 BC                    MOV EAX, DWORD PTR SS:[EBP-0x44]
	$+1A     006D093C     8945 E8                    MOV DWORD PTR SS:[EBP-0x18], EAX
	$+1D     006D093F     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+1F     006D0941     8945 DC                    MOV DWORD PTR SS:[EBP-0x24], EAX
	$+22     006D0944     8945 C0                    MOV DWORD PTR SS:[EBP-0x40], EAX           ; 设置stCD_Skill_IdInfo之技能指针
	$+25     006D0947     8B41 04                    MOV EAX, DWORD PTR DS:[ECX+0x4]
	$+28     006D094A     8945 D8                    MOV DWORD PTR SS:[EBP-0x28], EAX
	$+2B     006D094D     8945 C4                    MOV DWORD PTR SS:[EBP-0x3C], EAX           ; 设置stCD_Skill_IdInfo之技能信息对象指针
	$+2E     006D0950     C701 00000000              MOV DWORD PTR DS:[ECX], 0x0
	$+34     006D0956     C741 04 00000000           MOV DWORD PTR DS:[ECX+0x4], 0x0
	$+3B     006D095D     C745 FC 02000000           MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+42     006D0964     F6C3 10                    TEST BL, 0x10
	$+45     006D0967     74 33                      JE SHORT PathOfEx.006D099C
	$+47     006D0969     83E3 EF                    AND EBX, -0x11
	$+4A     006D096C     C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+4E     006D0970     8B4D BC                    MOV ECX, DWORD PTR SS:[EBP-0x44]
	$+51     006D0973     894D E0                    MOV DWORD PTR SS:[EBP-0x20], ECX
	$+54     006D0976     85C9                       TEST ECX, ECX
	$+56     006D0978     74 1E                      JE SHORT PathOfEx.006D0998
	$+58     006D097A     8BC6                       MOV EAX, ESI
	$+5A     006D097C     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+5F     006D0981     75 15                      JNZ SHORT PathOfEx.006D0998
	$+61     006D0983     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+63     006D0985     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+65     006D0987     8B4D E0                    MOV ECX, DWORD PTR SS:[EBP-0x20]
	$+68     006D098A     8BC6                       MOV EAX, ESI
	$+6A     006D098C     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+6F     006D0991     75 05                      JNZ SHORT PathOfEx.006D0998
	$+71     006D0993     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+73     006D0995     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+76     006D0998     C645 FC 02                 MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+7A     006D099C     F6C3 08                    TEST BL, 0x8
	$+7D     006D099F     74 30                      JE SHORT PathOfEx.006D09D1
	$+7F     006D09A1     83E3 F7                    AND EBX, -0x9
	$+82     006D09A4     C645 FC 04                 MOV BYTE PTR SS:[EBP-0x4], 0x4
	$+86     006D09A8     8B4D D4                    MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$+89     006D09AB     85C9                       TEST ECX, ECX
	$+8B     006D09AD     74 1E                      JE SHORT PathOfEx.006D09CD
	$+8D     006D09AF     8BC6                       MOV EAX, ESI
	$+8F     006D09B1     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+94     006D09B6     75 15                      JNZ SHORT PathOfEx.006D09CD
	$+96     006D09B8     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+98     006D09BA     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+9A     006D09BC     8B4D D4                    MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$+9D     006D09BF     8BC6                       MOV EAX, ESI
	$+9F     006D09C1     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+A4     006D09C6     75 05                      JNZ SHORT PathOfEx.006D09CD
	$+A6     006D09C8     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+A8     006D09CA     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+AB     006D09CD     C645 FC 02                 MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+AF     006D09D1     F6C3 04                    TEST BL, 0x4
	$+B2     006D09D4     74 2E                      JE SHORT PathOfEx.006D0A04
	$+B4     006D09D6     C645 FC 05                 MOV BYTE PTR SS:[EBP-0x4], 0x5
	$+B8     006D09DA     8B5D E8                    MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+BB     006D09DD     85DB                       TEST EBX, EBX
	$+BD     006D09DF     74 1F                      JE SHORT PathOfEx.006D0A00
	$+BF     006D09E1     8BC6                       MOV EAX, ESI
	$+C1     006D09E3     F0:0FC143 04               LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+C6     006D09E8     75 16                      JNZ SHORT PathOfEx.006D0A00
	$+C8     006D09EA     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+CA     006D09EC     8BCB                       MOV ECX, EBX
	$+CC     006D09EE     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+CE     006D09F0     8BC6                       MOV EAX, ESI
	$+D0     006D09F2     F0:0FC143 08               LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$+D5     006D09F7     75 07                      JNZ SHORT PathOfEx.006D0A00
	$+D7     006D09F9     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+D9     006D09FB     8BCB                       MOV ECX, EBX
	$+DB     006D09FD     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+DE     006D0A00     C645 FC 02                 MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+E2     006D0A04     8B8F 34010000              MOV ECX, DWORD PTR DS:[EDI+0x134]          ; PathOfEx.00A11856
	$+E8     006D0A0A     8D45 E0                    LEA EAX, DWORD PTR SS:[EBP-0x20]
	$+EB     006D0A0D     50                         PUSH EAX
	$+EC     006D0A0E     32DB                       XOR BL, BL
	$+EE     006D0A10     C745 E0 00000000           MOV DWORD PTR SS:[EBP-0x20], 0x0
	$+F5     006D0A17     6A 0D                      PUSH 0xD
	$+F7     006D0A19     885D E8                    MOV BYTE PTR SS:[EBP-0x18], BL
	$+FA     006D0A1C     E8 0FEA0E00                CALL PathOfEx.007BF430
	$+FF     006D0A21     8B75 E0                    MOV ESI, DWORD PTR SS:[EBP-0x20]
	$+102    006D0A24     56                         PUSH ESI
	$+103    006D0A25     FF15 38C4D800              CALL NEAR DWORD PTR DS:[0xD8C438]
	$+109    006D0A2B     0FB7C0                     MOVZX EAX, AX
	$+10C    006D0A2E     85F6                       TEST ESI, ESI
	$+10E    006D0A30     74 09                      JE SHORT PathOfEx.006D0A3B
	$+110    006D0A32     66:85C0                    TEST AX, AX
	$+113    006D0A35     79 04                      JNS SHORT PathOfEx.006D0A3B
	$+115    006D0A37     B7 01                      MOV BH, 0x1
	$+117    006D0A39     EB 02                      JMP SHORT PathOfEx.006D0A3D
	$+119    006D0A3B     32FF                       XOR BH, BH
	$+11B    006D0A3D     8BB7 34010000              MOV ESI, DWORD PTR DS:[EDI+0x134]          ; PathOfEx.00A11856
	$+121    006D0A43     8D45 F3                    LEA EAX, DWORD PTR SS:[EBP-0xD]
	$+124    006D0A46     50                         PUSH EAX
	$+125    006D0A47     68 CC49E900                PUSH PathOfEx.00E949CC                     ; always_attack_in_place
	$+12A    006D0A4C     68 08DEE600                PUSH PathOfEx.00E6DE08                     ; UI
	$+12F    006D0A51     8B8E 80030000              MOV ECX, DWORD PTR DS:[ESI+0x380]
	$+135    006D0A57     C645 F3 00                 MOV BYTE PTR SS:[EBP-0xD], 0x0
	$+139    006D0A5B     E8 00580F00                CALL PathOfEx.007C6260
	$+13E    006D0A60     807D F3 00                 CMP BYTE PTR SS:[EBP-0xD], 0x0
	$+142    006D0A64     75 04                      JNZ SHORT PathOfEx.006D0A6A
	$+144    006D0A66     84FF                       TEST BH, BH
	$+146    006D0A68     74 40                      JE SHORT PathOfEx.006D0AAA
	$+148    006D0A6A     8B4D DC                    MOV ECX, DWORD PTR SS:[EBP-0x24]
	$+14B    006D0A6D     85C9                       TEST ECX, ECX
	$+14D    006D0A6F     0F94C0                     SETE AL
	$+150    006D0A72     34 01                      XOR AL, 0x1
	$+152    006D0A74     74 34                      JE SHORT PathOfEx.006D0AAA
	$+154    006D0A76     0FB741 02                  MOVZX EAX, WORD PTR DS:[ECX+0x2]
	$+158    006D0A7A     66:3B05 0CD70001           CMP AX, WORD PTR DS:[0x100D70C]
	$+15F    006D0A81     0F94C2                     SETE DL
	$+162    006D0A84     66:3B05 1CD70001           CMP AX, WORD PTR DS:[0x100D71C]
	$+169    006D0A8B     0F94C0                     SETE AL
	$+16C    006D0A8E     84D2                       TEST DL, DL
	$+16E    006D0A90     74 13                      JE SHORT PathOfEx.006D0AA5
	$+170    006D0A92     84FF                       TEST BH, BH
	$+172    006D0A94     74 14                      JE SHORT PathOfEx.006D0AAA
	$+174    006D0A96     84C0                       TEST AL, AL
	$+176    006D0A98     75 0B                      JNZ SHORT PathOfEx.006D0AA5
	$+178    006D0A9A     8BCE                       MOV ECX, ESI
	$+17A    006D0A9C     E8 5FDB0E00                CALL PathOfEx.007BE600
	$+17F    006D0AA1     84C0                       TEST AL, AL
	$+181    006D0AA3     74 05                      JE SHORT PathOfEx.006D0AAA
	$+183    006D0AA5     B3 01                      MOV BL, 0x1
	$+185    006D0AA7     885D E8                    MOV BYTE PTR SS:[EBP-0x18], BL
	$+188    006D0AAA     6A 11                      PUSH 0x11
	$+18A    006D0AAC     FF15 38C4D800              CALL NEAR DWORD PTR DS:[0xD8C438]
	$+190    006D0AB2     0FB7C0                     MOVZX EAX, AX
	$+193    006D0AB5     66:85C0                    TEST AX, AX
	$+196    006D0AB8     79 06                      JNS SHORT PathOfEx.006D0AC0
	$+198    006D0ABA     80CB 04                    OR BL, 0x4
	$+19B    006D0ABD     885D E8                    MOV BYTE PTR SS:[EBP-0x18], BL
	$+19E    006D0AC0     8B87 D8180000              MOV EAX, DWORD PTR DS:[EDI+0x18D8]
	$+1A4    006D0AC6     8B80 000A0000              MOV EAX, DWORD PTR DS:[EAX+0xA00]
	$+1AA    006D0ACC     85C0                       TEST EAX, EAX
	$+1AC    006D0ACE     74 10                      JE SHORT PathOfEx.006D0AE0
	$+1AE    006D0AD0     8A80 54070000              MOV AL, BYTE PTR DS:[EAX+0x754]
	$+1B4    006D0AD6     84C0                       TEST AL, AL
	$+1B6    006D0AD8     74 06                      JE SHORT PathOfEx.006D0AE0
	$+1B8    006D0ADA     80CB 02                    OR BL, 0x2
	$+1BB    006D0ADD     885D E8                    MOV BYTE PTR SS:[EBP-0x18], BL
	$+1BE    006D0AE0     8B87 34010000              MOV EAX, DWORD PTR DS:[EDI+0x134]
	$+1C4    006D0AE6     8D4D F3                    LEA ECX, DWORD PTR SS:[EBP-0xD]
	$+1C7    006D0AE9     51                         PUSH ECX
	$+1C8    006D0AEA     68 2049E900                PUSH PathOfEx.00E94920                     ; auto_equip
	$+1CD    006D0AEF     68 08DEE600                PUSH PathOfEx.00E6DE08                     ; UI
	$+1D2    006D0AF4     8B88 80030000              MOV ECX, DWORD PTR DS:[EAX+0x380]
	$+1D8    006D0AFA     C645 F3 00                 MOV BYTE PTR SS:[EBP-0xD], 0x0
	$+1DC    006D0AFE     E8 5D570F00                CALL PathOfEx.007C6260
	$+1E1    006D0B03     807D F3 00                 CMP BYTE PTR SS:[EBP-0xD], 0x0
	$+1E5    006D0B07     74 06                      JE SHORT PathOfEx.006D0B0F
	$+1E7    006D0B09     80CB 08                    OR BL, 0x8
	$+1EA    006D0B0C     885D E8                    MOV BYTE PTR SS:[EBP-0x18], BL
	$+1ED    006D0B0F     8B87 64150000              MOV EAX, DWORD PTR DS:[EDI+0x1564]
	$+1F3    006D0B15     3B87 68150000              CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$+1F9    006D0B1B     75 07                      JNZ SHORT PathOfEx.006D0B24
	$+1FB    006D0B1D     B9 27000000                MOV ECX, 0x27
	$+200    006D0B22     EB 09                      JMP SHORT PathOfEx.006D0B2D
	$+202    006D0B24     8B87 68150000              MOV EAX, DWORD PTR DS:[EDI+0x1568]
	$+208    006D0B2A     8B48 F4                    MOV ECX, DWORD PTR DS:[EAX-0xC]
	$+20B    006D0B2D     C745 C8 00000000           MOV DWORD PTR SS:[EBP-0x38], 0x0
	$+212    006D0B34     C745 CC 00000000           MOV DWORD PTR SS:[EBP-0x34], 0x0
	$+219    006D0B3B     51                         PUSH ECX
	$+21A    006D0B3C     8AC3                       MOV AL, BL
	$+21C    006D0B3E     C645 FC 06                 MOV BYTE PTR SS:[EBP-0x4], 0x6
	$+220    006D0B42     24 01                      AND AL, 0x1
	$+222    006D0B44     0FB6C0                     MOVZX EAX, AL
	$+225    006D0B47     50                         PUSH EAX
	$+226    006D0B48     51                         PUSH ECX
	$+227    006D0B49     83EC 08                    SUB ESP, 0x8                               ; push stCD_Skill_IdInfo
	$+22A    006D0B4C     8D45 C0                    LEA EAX, DWORD PTR SS:[EBP-0x40]
	$+22D    006D0B4F     8BCC                       MOV ECX, ESP                               ; ecx==stCD_Skill_IdInfo*
	$+22F    006D0B51     50                         PUSH EAX
	$+230    006D0B52     E8 593CDBFF                CALL PathOfEx.004847B0                     ; stCD_Skill_IdInfo拷贝操作
	$+235    006D0B57     8D45 A4                    LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$+238    006D0B5A     8BCF                       MOV ECX, EDI
	$+23A    006D0B5C     50                         PUSH EAX                                   ; 第二个参数是stCD_Skill_IdInfo对象
	$+23B    006D0B5D     E8 CE0F0000                CALL PathOfEx.006D1B30                     ; 6个参数，获得技能对象吧
	$+240    006D0B62     8D45 EC                    LEA EAX, DWORD PTR SS:[EBP-0x14]
	$+243    006D0B65     8945 B4                    MOV DWORD PTR SS:[EBP-0x4C], EAX
	$+246    006D0B68     8D4D B4                    LEA ECX, DWORD PTR SS:[EBP-0x4C]
	$+249    006D0B6B     8D45 C8                    LEA EAX, DWORD PTR SS:[EBP-0x38]           ; 技能id指针
	$+24C    006D0B6E     8945 B8                    MOV DWORD PTR SS:[EBP-0x48], EAX
	$+24F    006D0B71     8D45 E4                    LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+252    006D0B74     8945 BC                    MOV DWORD PTR SS:[EBP-0x44], EAX
	$+255    006D0B77     8B45 B0                    MOV EAX, DWORD PTR SS:[EBP-0x50]
	$+258    006D0B7A     8945 E4                    MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+25B    006D0B7D     8D45 A4                    LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$+25E    006D0B80     50                         PUSH EAX                                   ; eax+0x14的地方存放着skill_id指针
	$+25F    006D0B81     E8 9A4A0000                CALL PathOfEx.006D5620                     ; 填空结构体，其中包括技能id信息
	$+264    006D0B86     8B45 E4                    MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+267    006D0B89     85C0                       TEST EAX, EAX
	$+269    006D0B8B     75 0B                      JNZ SHORT PathOfEx.006D0B98
	$+26B    006D0B8D     8D78 01                    LEA EDI, DWORD PTR DS:[EAX+0x1]
	$+26E    006D0B90     83CE FF                    OR ESI, -0x1
	$+271    006D0B93     E9 8C030000                JMP PathOfEx.006D0F24
	$+276    006D0B98     3B87 7C150000              CMP EAX, DWORD PTR DS:[EDI+0x157C]
	$+27C    006D0B9E     8D8F 74150000              LEA ECX, DWORD PTR DS:[EDI+0x1574]
	$+282    006D0BA4     8D45 B8                    LEA EAX, DWORD PTR SS:[EBP-0x48]
	$+285    006D0BA7     50                         PUSH EAX
	$+286    006D0BA8     0F9445 F2                  SETE BYTE PTR SS:[EBP-0xE]
	$+28A    006D0BAC     E8 1FE2EAFF                CALL PathOfEx.0057EDD0
	$+28F    006D0BB1     8B00                       MOV EAX, DWORD PTR DS:[EAX]
	$+291    006D0BB3     3945 C8                    CMP DWORD PTR SS:[EBP-0x38], EAX
	$+294    006D0BB6     0F94C0                     SETE AL
	$+297    006D0BB9     8845 F3                    MOV BYTE PTR SS:[EBP-0xD], AL
	$+29A    006D0BBC     C645 FC 08                 MOV BYTE PTR SS:[EBP-0x4], 0x8
	$+29E    006D0BC0     83CE FF                    OR ESI, -0x1
	$+2A1    006D0BC3     8B4D BC                    MOV ECX, DWORD PTR SS:[EBP-0x44]
	$+2A4    006D0BC6     894D DC                    MOV DWORD PTR SS:[EBP-0x24], ECX
	$+2A7    006D0BC9     85C9                       TEST ECX, ECX
	$+2A9    006D0BCB     74 21                      JE SHORT PathOfEx.006D0BEE
	$+2AB    006D0BCD     8BC6                       MOV EAX, ESI
	$+2AD    006D0BCF     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+2B2    006D0BD4     75 15                      JNZ SHORT PathOfEx.006D0BEB
	$+2B4    006D0BD6     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+2B6    006D0BD8     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+2B8    006D0BDA     8B4D DC                    MOV ECX, DWORD PTR SS:[EBP-0x24]
	$+2BB    006D0BDD     8BC6                       MOV EAX, ESI
	$+2BD    006D0BDF     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+2C2    006D0BE4     75 05                      JNZ SHORT PathOfEx.006D0BEB
	$+2C4    006D0BE6     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$+2C6    006D0BE8     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+2C9    006D0BEB     8A45 F3                    MOV AL, BYTE PTR SS:[EBP-0xD]
	$+2CC    006D0BEE     C645 FC 07                 MOV BYTE PTR SS:[EBP-0x4], 0x7
	$+2D0    006D0BF2     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+2D3    006D0BF5     3B8F 70150000              CMP ECX, DWORD PTR DS:[EDI+0x1570]
	$+2D9    006D0BFB     0FB6D3                     MOVZX EDX, BL
	$+2DC    006D0BFE     8A5D F2                    MOV BL, BYTE PTR SS:[EBP-0xE]
	$+2DF    006D0C01     0F94C7                     SETE BH
	$+2E2    006D0C04     8955 DC                    MOV DWORD PTR SS:[EBP-0x24], EDX
	$+2E5    006D0C07     84DB                       TEST BL, BL
	$+2E7    006D0C09     74 1E                      JE SHORT PathOfEx.006D0C29
	$+2E9    006D0C0B     84C0                       TEST AL, AL
	$+2EB    006D0C0D     0F84 85000000              JE PathOfEx.006D0C98
	$+2F1    006D0C13     84FF                       TEST BH, BH
	$+2F3    006D0C15     74 12                      JE SHORT PathOfEx.006D0C29
	$+2F5    006D0C17     3B97 88150000              CMP EDX, DWORD PTR DS:[EDI+0x1588]
	$+2FB    006D0C1D     75 0A                      JNZ SHORT PathOfEx.006D0C29
	$+2FD    006D0C1F     BF 02000000                MOV EDI, 0x2
	$+302    006D0C24     E9 FB020000                JMP PathOfEx.006D0F24
	$+307    006D0C29     84C0                       TEST AL, AL
	$+309    006D0C2B     74 6B                      JE SHORT PathOfEx.006D0C98
	$+30B    006D0C2D     8B55 C8                    MOV EDX, DWORD PTR SS:[EBP-0x38]
	$+30E    006D0C30     85D2                       TEST EDX, EDX
	$+310    006D0C32     0F94C0                     SETE AL
	$+313    006D0C35     34 01                      XOR AL, 0x1
	$+315    006D0C37     74 5F                      JE SHORT PathOfEx.006D0C98
	$+317    006D0C39     6A 3A                      PUSH 0x3A
	$+319    006D0C3B     8BCA                       MOV ECX, EDX
	$+31B    006D0C3D     E8 EE281400                CALL PathOfEx.00813530
	$+320    006D0C42     84C0                       TEST AL, AL
	$+322    006D0C44     74 4F                      JE SHORT PathOfEx.006D0C95
	$+324    006D0C46     84DB                       TEST BL, BL
	$+326    006D0C48     74 1E                      JE SHORT PathOfEx.006D0C68
	$+328    006D0C4A     84FF                       TEST BH, BH
	$+32A    006D0C4C     74 0A                      JE SHORT PathOfEx.006D0C58
	$+32C    006D0C4E     BF 02000000                MOV EDI, 0x2
	$+331    006D0C53     E9 CC020000                JMP PathOfEx.006D0F24
	$+336    006D0C58     837D E4 03                 CMP DWORD PTR SS:[EBP-0x1C], 0x3
	$+33A    006D0C5C     75 37                      JNZ SHORT PathOfEx.006D0C95
	$+33C    006D0C5E     BF 02000000                MOV EDI, 0x2
	$+341    006D0C63     E9 BC020000                JMP PathOfEx.006D0F24
	$+346    006D0C68     837D E4 03                 CMP DWORD PTR SS:[EBP-0x1C], 0x3
	$+34A    006D0C6C     75 27                      JNZ SHORT PathOfEx.006D0C95
	$+34C    006D0C6E     83BF 7C150000 04           CMP DWORD PTR DS:[EDI+0x157C], 0x4
	$+353    006D0C75     75 1E                      JNZ SHORT PathOfEx.006D0C95
	$+355    006D0C77     C787 70150000 00000000     MOV DWORD PTR DS:[EDI+0x1570], 0x0
	$+35F    006D0C81     C787 7C150000 03000000     MOV DWORD PTR DS:[EDI+0x157C], 0x3
	$+369    006D0C8B     BF 02000000                MOV EDI, 0x2
	$+36E    006D0C90     E9 8F020000                JMP PathOfEx.006D0F24
	$+373    006D0C95     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]           ; 鼠标左键不会到
	$+376    006D0C98     398F BC180000              CMP DWORD PTR DS:[EDI+0x18BC], ECX
	$+37C    006D0C9E     74 0D                      JE SHORT PathOfEx.006D0CAD                 ; 鼠标左键这里会到
	$+37E    006D0CA0     6A 00                      PUSH 0x0
	$+380    006D0CA2     51                         PUSH ECX
	$+381    006D0CA3     8BCF                       MOV ECX, EDI
	$+383    006D0CA5     E8 56DDFFFF                CALL PathOfEx.006CEA00
	$+388    006D0CAA     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]           ; 鼠标左键这里会到
	$+38B    006D0CAD     8B55 C8                    MOV EDX, DWORD PTR SS:[EBP-0x38]
	$+38E    006D0CB0     85D2                       TEST EDX, EDX
	$+390    006D0CB2     0F95C0                     SETNE AL
	$+393    006D0CB5     84C0                       TEST AL, AL
	$+395    006D0CB7     74 1E                      JE SHORT PathOfEx.006D0CD7
	$+397    006D0CB9     8B87 3C010000              MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$+39D    006D0CBF     8A80 6B3B0000              MOV AL, BYTE PTR DS:[EAX+0x3B6B]
	$+3A3    006D0CC5     84C0                       TEST AL, AL
	$+3A5    006D0CC7     75 0E                      JNZ SHORT PathOfEx.006D0CD7
	$+3A7    006D0CC9     FF75 E4                    PUSH DWORD PTR SS:[EBP-0x1C]               ; 鼠标左键这里会到
	$+3AA    006D0CCC     8BCF                       MOV ECX, EDI
	$+3AC    006D0CCE     52                         PUSH EDX
	$+3AD    006D0CCF     E8 CCF7FFFF                CALL PathOfEx.006D04A0
	$+3B2    006D0CD4     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+3B5    006D0CD7     F3:0F1087 A0180000         MOVSS XMM0, DWORD PTR DS:[EDI+0x18A0]
	$+3BD    006D0CDF     F3:0F5905 B0D7E600         MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$+3C5    006D0CE7     F3:0F2CC0                  CVTTSS2SI EAX, XMM0
	$+3C9    006D0CEB     F3:0F1087 9C180000         MOVSS XMM0, DWORD PTR DS:[EDI+0x189C]
	$+3D1    006D0CF3     F3:0F5905 B0D7E600         MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$+3D9    006D0CFB     8945 E0                    MOV DWORD PTR SS:[EBP-0x20], EAX
	$+3DC    006D0CFE     8945 D4                    MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+3DF    006D0D01     8B45 E4                    MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+3E2    006D0D04     F3:0F2CD8                  CVTTSS2SI EBX, XMM0
	$+3E6    006D0D08     48                         DEC EAX
	$+3E7    006D0D09     895D D0                    MOV DWORD PTR SS:[EBP-0x30], EBX
	$+3EA    006D0D0C     83F8 04                    CMP EAX, 0x4
	$+3ED    006D0D0F     0F87 C9010000              JA PathOfEx.006D0EDE
	$+3F3    006D0D15     FF2485 B80F6D00            JMP NEAR DWORD PTR DS:[EAX*4+0x6D0FB8]     ; eax==3,左键点击普通攻击
	$+3FA    006D0D1C     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$+3FD    006D0D1F     A1 0CD70001                MOV EAX, DWORD PTR DS:[0x100D70C]          ; 走路技能id存放在此全局变量
	$+402    006D0D24     8D4D D0                    LEA ECX, DWORD PTR SS:[EBP-0x30]
	$+405    006D0D27     51                         PUSH ECX
	$+406    006D0D28     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]          ; edi=InGameState
	$+40C    006D0D2E     25 FF3F0000                AND EAX, 0x3FFF
	$+411    006D0D33     6A 00                      PUSH 0x0
	$+413    006D0D35     50                         PUSH EAX
	$+414    006D0D36     E8 D5482000                CALL PathOfEx.008D5610                     ; 走路_使用技能发包
	$+419    006D0D3B     E9 9E010000                JMP PathOfEx.006D0EDE
	$+41E    006D0D40     83BF BC180000 00           CMP DWORD PTR DS:[EDI+0x18BC], 0x0
	$+425    006D0D47     74 38                      JE SHORT PathOfEx.006D0D81
	$+427    006D0D49     8D45 C8                    LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+42A    006D0D4C     50                         PUSH EAX
	$+42B    006D0D4D     E8 5EE5FFFF                CALL PathOfEx.006CF2B0
	$+430    006D0D52     84C0                       TEST AL, AL
	$+432    006D0D54     74 2B                      JE SHORT PathOfEx.006D0D81
	$+434    006D0D56     FFB7 BC180000              PUSH DWORD PTR DS:[EDI+0x18BC]             ; PathOfEx.0063006D
	$+43A    006D0D5C     8BCF                       MOV ECX, EDI
	$+43C    006D0D5E     E8 4DDEFFFF                CALL PathOfEx.006CEBB0
	$+441    006D0D63     84C0                       TEST AL, AL
	$+443    006D0D65     74 1A                      JE SHORT PathOfEx.006D0D81
	$+445    006D0D67     8B87 BC180000              MOV EAX, DWORD PTR DS:[EDI+0x18BC]         ; PathOfEx.0063006D
	$+44B    006D0D6D     8B40 20                    MOV EAX, DWORD PTR DS:[EAX+0x20]
	$+44E    006D0D70     8B58 10                    MOV EBX, DWORD PTR DS:[EAX+0x10]
	$+451    006D0D73     8B40 14                    MOV EAX, DWORD PTR DS:[EAX+0x14]
	$+454    006D0D76     895D D0                    MOV DWORD PTR SS:[EBP-0x30], EBX
	$+457    006D0D79     8945 E0                    MOV DWORD PTR SS:[EBP-0x20], EAX
	$+45A    006D0D7C     8945 D4                    MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+45D    006D0D7F     EB 03                      JMP SHORT PathOfEx.006D0D84
	$+45F    006D0D81     8B5D D0                    MOV EBX, DWORD PTR SS:[EBP-0x30]
	$+462    006D0D84     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$+465    006D0D87     8B45 C8                    MOV EAX, DWORD PTR SS:[EBP-0x38]
	$+468    006D0D8A     8D4D D0                    LEA ECX, DWORD PTR SS:[EBP-0x30]
	$+46B    006D0D8D     51                         PUSH ECX
	$+46C    006D0D8E     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+472    006D0D94     6A 00                      PUSH 0x0
	$+474    006D0D96     0FB700                     MOVZX EAX, WORD PTR DS:[EAX]
	$+477    006D0D99     50                         PUSH EAX
	$+478    006D0D9A     E8 71482000                CALL PathOfEx.008D5610
	$+47D    006D0D9F     E9 3D010000                JMP PathOfEx.006D0EE1
	$+482    006D0DA4     85C9                       TEST ECX, ECX
	$+484    006D0DA6     0F84 32010000              JE PathOfEx.006D0EDE
	$+48A    006D0DAC     8B97 3C010000              MOV EDX, DWORD PTR DS:[EDI+0x13C]
	$+490    006D0DB2     8A82 AC3E0000              MOV AL, BYTE PTR DS:[EDX+0x3EAC]
	$+496    006D0DB8     84C0                       TEST AL, AL
	$+498    006D0DBA     74 39                      JE SHORT PathOfEx.006D0DF5
	$+49A    006D0DBC     80BF D51A0000 00           CMP BYTE PTR DS:[EDI+0x1AD5], 0x0
	$+4A1    006D0DC3     74 30                      JE SHORT PathOfEx.006D0DF5
	$+4A3    006D0DC5     83BF D81A0000 00           CMP DWORD PTR DS:[EDI+0x1AD8], 0x0
	$+4AA    006D0DCC     0F84 0C010000              JE PathOfEx.006D0EDE
	$+4B0    006D0DD2     51                         PUSH ECX
	$+4B1    006D0DD3     E8 68CBDFFF                CALL PathOfEx.004CD940
	$+4B6    006D0DD8     84C0                       TEST AL, AL
	$+4B8    006D0DDA     0F84 FE000000              JE PathOfEx.006D0EDE
	$+4BE    006D0DE0     8B8F D81A0000              MOV ECX, DWORD PTR DS:[EDI+0x1AD8]         ; PathOfEx.0076002C
	$+4C4    006D0DE6     6A 01                      PUSH 0x1
	$+4C6    006D0DE8     FF75 EC                    PUSH DWORD PTR SS:[EBP-0x14]               ; PathOfEx.00C00000
	$+4C9    006D0DEB     E8 10CCDFFF                CALL PathOfEx.004CDA00
	$+4CE    006D0DF0     E9 E9000000                JMP PathOfEx.006D0EDE
	$+4D3    006D0DF5     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$+4D6    006D0DF8     A1 18D70001                MOV EAX, DWORD PTR DS:[0x100D718]
	$+4DB    006D0DFD     FF71 14                    PUSH DWORD PTR DS:[ECX+0x14]
	$+4DE    006D0E00     25 FF3F0000                AND EAX, 0x3FFF
	$+4E3    006D0E05     8BCA                       MOV ECX, EDX
	$+4E5    006D0E07     50                         PUSH EAX
	$+4E6    006D0E08     E8 D3472000                CALL PathOfEx.008D55E0
	$+4EB    006D0E0D     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+4EE    006D0E10     898F D0180000              MOV DWORD PTR DS:[EDI+0x18D0], ECX
	$+4F4    006D0E16     E8 053EF9FF                CALL PathOfEx.00664C20
	$+4F9    006D0E1B     84C0                       TEST AL, AL
	$+4FB    006D0E1D     0F84 BB000000              JE PathOfEx.006D0EDE
	$+501    006D0E23     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+507    006D0E29     E8 E24A2000                CALL PathOfEx.008D5910
	$+50C    006D0E2E     E9 AB000000                JMP PathOfEx.006D0EDE                      ; 普通攻击这里不到
	$+511    006D0E33     85C9                       TEST ECX, ECX                              ; 普通攻击这里会到
	$+513    006D0E35     0F84 A3000000              JE PathOfEx.006D0EDE                       ; edi=InGameState
	$+519    006D0E3B     8B87 3C010000              MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$+51F    006D0E41     8A80 6B3B0000              MOV AL, BYTE PTR DS:[EAX+0x3B6B]           ; eax=stCD_GameLogic*
	$+525    006D0E47     84C0                       TEST AL, AL
	$+527    006D0E49     74 48                      JE SHORT PathOfEx.006D0E93                 ; 普通攻击这里会到
	$+529    006D0E4B     E8 C0C4DDFF                CALL PathOfEx.004AD310
	$+52E    006D0E50     84C0                       TEST AL, AL
	$+530    006D0E52     74 14                      JE SHORT PathOfEx.006D0E68
	$+532    006D0E54     FF75 EC                    PUSH DWORD PTR SS:[EBP-0x14]
	$+535    006D0E57     8BCF                       MOV ECX, EDI
	$+537    006D0E59     E8 D2140000                CALL PathOfEx.006D2330
	$+53C    006D0E5E     BF 03000000                MOV EDI, 0x3
	$+541    006D0E63     E9 BC000000                JMP PathOfEx.006D0F24
	$+546    006D0E68     8B45 C8                    MOV EAX, DWORD PTR SS:[EBP-0x38]
	$+549    006D0E6B     66:8B40 02                 MOV AX, WORD PTR DS:[EAX+0x2]
	$+54D    006D0E6F     66:3B05 18D70001           CMP AX, WORD PTR DS:[0x100D718]
	$+554    006D0E76   ^ 75 E6                      JNZ SHORT PathOfEx.006D0E5E
	$+556    006D0E78     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+559    006D0E7B     E8 80340000                CALL PathOfEx.006D4300
	$+55E    006D0E80     84C0                       TEST AL, AL
	$+560    006D0E82     75 0C                      JNZ SHORT PathOfEx.006D0E90
	$+562    006D0E84     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+565    006D0E87     E8 E43DF9FF                CALL PathOfEx.00664C70
	$+56A    006D0E8C     84C0                       TEST AL, AL
	$+56C    006D0E8E   ^ 74 CE                      JE SHORT PathOfEx.006D0E5E
	$+56E    006D0E90     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]           ; 这里不到
	$+571    006D0E93     8B45 C8                    MOV EAX, DWORD PTR SS:[EBP-0x38]
	$+574    006D0E96     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$+577    006D0E99     FF71 14                    PUSH DWORD PTR DS:[ECX+0x14]
	$+57A    006D0E9C     0FB700                     MOVZX EAX, WORD PTR DS:[EAX]               ; 执行后eax=skill_id
	$+57D    006D0E9F     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+583    006D0EA5     EB 31                      JMP SHORT PathOfEx.006D0ED8                ; 左键点击普通攻击从这里跳转
	$+585    006D0EA7     8B87 38010000              MOV EAX, DWORD PTR DS:[EDI+0x138]
	$+58B    006D0EAD     8B55 C8                    MOV EDX, DWORD PTR SS:[EBP-0x38]
	$+58E    006D0EB0     85D2                       TEST EDX, EDX
	$+590    006D0EB2     8B98 C4010000              MOV EBX, DWORD PTR DS:[EAX+0x1C4]
	$+596    006D0EB8     0F94C0                     SETE AL
	$+599    006D0EBB     34 01                      XOR AL, 0x1
	$+59B    006D0EBD     74 1F                      JE SHORT PathOfEx.006D0EDE
	$+59D    006D0EBF     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+5A3    006D0EC5     8A81 6B3B0000              MOV AL, BYTE PTR DS:[ECX+0x3B6B]
	$+5A9    006D0ECB     84C0                       TEST AL, AL
	$+5AB    006D0ECD     75 0F                      JNZ SHORT PathOfEx.006D0EDE
	$+5AD    006D0ECF     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$+5B0    006D0ED2     0FB702                     MOVZX EAX, WORD PTR DS:[EDX]
	$+5B3    006D0ED5     FF73 14                    PUSH DWORD PTR DS:[EBX+0x14]
	$+5B6    006D0ED8     50                         PUSH EAX                                   ; skill_id
	$+5B7    006D0ED9     E8 02472000                CALL PathOfEx.008D55E0                     ; 左键点击之普通攻击发包
	$+5BC    006D0EDE     8B5D D0                    MOV EBX, DWORD PTR SS:[EBP-0x30]
	$+5BF    006D0EE1     8B45 E0                    MOV EAX, DWORD PTR SS:[EBP-0x20]
	$+5C2    006D0EE4     8D8F 70150000              LEA ECX, DWORD PTR DS:[EDI+0x1570]
	$+5C8    006D0EEA     8987 84150000              MOV DWORD PTR DS:[EDI+0x1584], EAX
	$+5CE    006D0EF0     8B45 DC                    MOV EAX, DWORD PTR SS:[EBP-0x24]
	$+5D1    006D0EF3     8987 88150000              MOV DWORD PTR DS:[EDI+0x1588], EAX
	$+5D7    006D0EF9     8D45 A4                    LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$+5DA    006D0EFC     50                         PUSH EAX
	$+5DB    006D0EFD     899F 80150000              MOV DWORD PTR DS:[EDI+0x1580], EBX
	$+5E1    006D0F03     E8 18360000                CALL PathOfEx.006D4520
	$+5E6    006D0F08     E8 53BF0300                CALL PathOfEx.0070CE60
	$+5EB    006D0F0D     83C0 43                    ADD EAX, 0x43
	$+5EE    006D0F10     8987 90150000              MOV DWORD PTR DS:[EDI+0x1590], EAX
	$+5F4    006D0F16     83D2 00                    ADC EDX, 0x0
	$+5F7    006D0F19     8997 94150000              MOV DWORD PTR DS:[EDI+0x1594], EDX
	$+5FD    006D0F1F     BF 04000000                MOV EDI, 0x4
	$+602    006D0F24     C645 FC 09                 MOV BYTE PTR SS:[EBP-0x4], 0x9
	$+606    006D0F28     8B5D AC                    MOV EBX, DWORD PTR SS:[EBP-0x54]
	$+609    006D0F2B     85DB                       TEST EBX, EBX
	$+60B    006D0F2D     74 1F                      JE SHORT PathOfEx.006D0F4E
	$+60D    006D0F2F     8BC6                       MOV EAX, ESI
	$+60F    006D0F31     F0:0FC143 04               LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+614    006D0F36     75 16                      JNZ SHORT PathOfEx.006D0F4E
	$+616    006D0F38     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+618    006D0F3A     8BCB                       MOV ECX, EBX
	$+61A    006D0F3C     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+61C    006D0F3E     8BC6                       MOV EAX, ESI
	$+61E    006D0F40     F0:0FC143 08               LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$+623    006D0F45     75 07                      JNZ SHORT PathOfEx.006D0F4E
	$+625    006D0F47     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+627    006D0F49     8BCB                       MOV ECX, EBX
	$+629    006D0F4B     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+62C    006D0F4E     C645 FC 0A                 MOV BYTE PTR SS:[EBP-0x4], 0xA
	$+630    006D0F52     8B5D CC                    MOV EBX, DWORD PTR SS:[EBP-0x34]
	$+633    006D0F55     85DB                       TEST EBX, EBX
	$+635    006D0F57     74 1F                      JE SHORT PathOfEx.006D0F78
	$+637    006D0F59     8BC6                       MOV EAX, ESI
	$+639    006D0F5B     F0:0FC143 04               LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+63E    006D0F60     75 16                      JNZ SHORT PathOfEx.006D0F78
	$+640    006D0F62     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+642    006D0F64     8BCB                       MOV ECX, EBX
	$+644    006D0F66     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+646    006D0F68     8BC6                       MOV EAX, ESI
	$+648    006D0F6A     F0:0FC143 08               LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$+64D    006D0F6F     75 07                      JNZ SHORT PathOfEx.006D0F78
	$+64F    006D0F71     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+651    006D0F73     8BCB                       MOV ECX, EBX
	$+653    006D0F75     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+656    006D0F78     C745 FC 0B000000           MOV DWORD PTR SS:[EBP-0x4], 0xB
	$+65D    006D0F7F     8B5D D8                    MOV EBX, DWORD PTR SS:[EBP-0x28]           ; PathOfEx.00EE795F
	$+660    006D0F82     85DB                       TEST EBX, EBX
	$+662    006D0F84     74 1E                      JE SHORT PathOfEx.006D0FA4
	$+664    006D0F86     8BC6                       MOV EAX, ESI
	$+666    006D0F88     F0:0FC143 04               LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+66B    006D0F8D     75 15                      JNZ SHORT PathOfEx.006D0FA4
	$+66D    006D0F8F     8B03                       MOV EAX, DWORD PTR DS:[EBX]
	$+66F    006D0F91     8BCB                       MOV ECX, EBX
	$+671    006D0F93     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+673    006D0F95     F0:0FC173 08               LOCK XADD DWORD PTR DS:[EBX+0x8], ESI
	$+678    006D0F9A     4E                         DEC ESI
	$+679    006D0F9B     75 07                      JNZ SHORT PathOfEx.006D0FA4
	$+67B    006D0F9D     8B13                       MOV EDX, DWORD PTR DS:[EBX]
	$+67D    006D0F9F     8BCB                       MOV ECX, EBX
	$+67F    006D0FA1     FF52 04                    CALL NEAR DWORD PTR DS:[EDX+0x4]
	$+682    006D0FA4     8B4D F4                    MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+685    006D0FA7     8BC7                       MOV EAX, EDI
	$+687    006D0FA9     5F                         POP EDI
	$+688    006D0FAA     5E                         POP ESI
	$+689    006D0FAB     64:890D 00000000           MOV DWORD PTR FS:[0], ECX
	$+690    006D0FB2     5B                         POP EBX
	$+691    006D0FB3     8BE5                       MOV ESP, EBP
	$+693    006D0FB5     5D                         POP EBP
	$+694    006D0FB6     C3                         RETN
	$+695    006D0FB7     90                         NOP
	*/

	//gj3
	/*
	$-51B    005BD7B5     E8 C673D6FF              CALL PathOfEx.00324B80
	$-516    005BD7BA     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-512    005BD7BE     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-50F    005BD7C1     74 58                    JE SHORT PathOfEx.005BD81B
	$-50D    005BD7C3     8B70 0C                  MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-50A    005BD7C6     83FE FF                  CMP ESI,-0x1
	$-507    005BD7C9     74 50                    JE SHORT PathOfEx.005BD81B
	$-505    005BD7CB     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-502    005BD7CE     8B34B0                   MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-4FF    005BD7D1     85F6                     TEST ESI,ESI
	$-4FD    005BD7D3     74 46                    JE SHORT PathOfEx.005BD81B
	$-4FB    005BD7D5     8D43 E0                  LEA EAX,DWORD PTR DS:[EBX-0x20]
	$-4F8    005BD7D8     85C0                     TEST EAX,EAX
	$-4F6    005BD7DA     74 09                    JE SHORT PathOfEx.005BD7E5
	$-4F4    005BD7DC     8D43 04                  LEA EAX,DWORD PTR DS:[EBX+0x4]
	$-4F1    005BD7DF     894424 10                MOV DWORD PTR SS:[ESP+0x10],EAX
	$-4ED    005BD7E3     EB 08                    JMP SHORT PathOfEx.005BD7ED
	$-4EB    005BD7E5     C74424 10 00000000       MOV DWORD PTR SS:[ESP+0x10],0x0
	$-4E3    005BD7ED     8B7E 14                  MOV EDI,DWORD PTR DS:[ESI+0x14]
	$-4E0    005BD7F0     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-4DC    005BD7F4     50                       PUSH EAX
	$-4DB    005BD7F5     FF77 04                  PUSH DWORD PTR DS:[EDI+0x4]
	$-4D8    005BD7F8     57                       PUSH EDI
	$-4D7    005BD7F9     E8 62EAE4FF              CALL PathOfEx.0040C260
	$-4D2    005BD7FE     8BD0                     MOV EDX,EAX
	$-4D0    005BD800     B9 54555515              MOV ECX,0x15555554
	$-4CB    005BD805     8B46 18                  MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-4C8    005BD808     2BC8                     SUB ECX,EAX
	$-4C6    005BD80A     83F9 01                  CMP ECX,0x1
	$-4C3    005BD80D     72 15                    JB SHORT PathOfEx.005BD824
	$-4C1    005BD80F     40                       INC EAX
	$-4C0    005BD810     8946 18                  MOV DWORD PTR DS:[ESI+0x18],EAX
	$-4BD    005BD813     8957 04                  MOV DWORD PTR DS:[EDI+0x4],EDX
	$-4BA    005BD816     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-4B7    005BD819     8910                     MOV DWORD PTR DS:[EAX],EDX
	$-4B5    005BD81B     5F                       POP EDI
	$-4B4    005BD81C     5E                       POP ESI
	$-4B3    005BD81D     5B                       POP EBX
	$-4B2    005BD81E     8BE5                     MOV ESP,EBP
	$-4B0    005BD820     5D                       POP EBP
	$-4AF    005BD821     C2 0400                  RETN 0x4
	$-4AC    005BD824     68 AC29ED00              PUSH PathOfEx.00ED29AC
	$-4A7    005BD829     E8 CAA46F00              CALL PathOfEx.00CB7CF8
	$-4A2    005BD82E     CC                       INT3
	$-4A1    005BD82F     CC                       INT3
	$-4A0    005BD830     55                       PUSH EBP
	$-49F    005BD831     8BEC                     MOV EBP,ESP
	$-49D    005BD833     83E4 F8                  AND ESP,0xFFFFFFF8
	$-49A    005BD836     83EC 0C                  SUB ESP,0xC
	$-497    005BD839     53                       PUSH EBX
	$-496    005BD83A     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-493    005BD83D     56                       PUSH ESI
	$-492    005BD83E     57                       PUSH EDI
	$-491    005BD83F     8BF9                     MOV EDI,ECX
	$-48F    005BD841     8B8F 44190000            MOV ECX,DWORD PTR DS:[EDI+0x1944]
	$-489    005BD847     85C9                     TEST ECX,ECX
	$-487    005BD849     74 0C                    JE SHORT PathOfEx.005BD857
	$-485    005BD84B     8B89 740A0000            MOV ECX,DWORD PTR DS:[ECX+0xA74]
	$-47F    005BD851     53                       PUSH EBX
	$-47E    005BD852     E8 89D2F7FF              CALL PathOfEx.0053AAE0
	$-479    005BD857     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-477    005BD859     85C9                     TEST ECX,ECX
	$-475    005BD85B     74 05                    JE SHORT PathOfEx.005BD862
	$-473    005BD85D     E8 6E200500              CALL PathOfEx.0060F8D0
	$-46E    005BD862     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-46C    005BD864     C74424 10 6472ED00       MOV DWORD PTR SS:[ESP+0x10],PathOfEx.00ED7264         ; ASCII "Life"
	$-464    005BD86C     8B70 30                  MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-461    005BD86F     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-45D    005BD873     50                       PUSH EAX
	$-45C    005BD874     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-458    005BD878     50                       PUSH EAX
	$-457    005BD879     8D4E 14                  LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-454    005BD87C     E8 FF72D6FF              CALL PathOfEx.00324B80
	$-44F    005BD881     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$-44B    005BD885     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-448    005BD888     74 24                    JE SHORT PathOfEx.005BD8AE
	$-446    005BD88A     8B48 0C                  MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-443    005BD88D     83F9 FF                  CMP ECX,-0x1
	$-440    005BD890     74 1C                    JE SHORT PathOfEx.005BD8AE
	$-43E    005BD892     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-43B    005BD895     8B0C88                   MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$-438    005BD898     85C9                     TEST ECX,ECX
	$-436    005BD89A     74 12                    JE SHORT PathOfEx.005BD8AE
	$-434    005BD89C     8D57 E0                  LEA EDX,DWORD PTR DS:[EDI-0x20]
	$-431    005BD89F     F7DA                     NEG EDX
	$-42F    005BD8A1     8D47 04                  LEA EAX,DWORD PTR DS:[EDI+0x4]
	$-42C    005BD8A4     1BD2                     SBB EDX,EDX
	$-42A    005BD8A6     23D0                     AND EDX,EAX
	$-428    005BD8A8     52                       PUSH EDX
	$-427    005BD8A9     E8 02701100              CALL PathOfEx.006D48B0
	$-422    005BD8AE     399F 5C1A0000            CMP DWORD PTR DS:[EDI+0x1A5C],EBX
	$-41C    005BD8B4     75 0A                    JNZ SHORT PathOfEx.005BD8C0
	$-41A    005BD8B6     6A 00                    PUSH 0x0
	$-418    005BD8B8     8D4F E0                  LEA ECX,DWORD PTR DS:[EDI-0x20]
	$-415    005BD8BB     E8 901F0000              CALL PathOfEx.005BF850
	$-410    005BD8C0     3B9F 28190000            CMP EBX,DWORD PTR DS:[EDI+0x1928]
	$-40A    005BD8C6     75 1D                    JNZ SHORT PathOfEx.005BD8E5
	$-408    005BD8C8     8B87 28190000            MOV EAX,DWORD PTR DS:[EDI+0x1928]
	$-402    005BD8CE     8D4F E0                  LEA ECX,DWORD PTR DS:[EDI-0x20]
	$-3FF    005BD8D1     85C0                     TEST EAX,EAX
	$-3FD    005BD8D3     74 10                    JE SHORT PathOfEx.005BD8E5
	$-3FB    005BD8D5     50                       PUSH EAX
	$-3FA    005BD8D6     C781 48190000 00000000   MOV DWORD PTR DS:[ECX+0x1948],0x0
	$-3F0    005BD8E0     E8 2BF5FFFF              CALL PathOfEx.005BCE10
	$-3EB    005BD8E5     3B9F 3C190000            CMP EBX,DWORD PTR DS:[EDI+0x193C]
	$-3E5    005BD8EB     75 0A                    JNZ SHORT PathOfEx.005BD8F7
	$-3E3    005BD8ED     C787 3C190000 00000000   MOV DWORD PTR DS:[EDI+0x193C],0x0
	$-3D9    005BD8F7     5F                       POP EDI
	$-3D8    005BD8F8     5E                       POP ESI
	$-3D7    005BD8F9     5B                       POP EBX
	$-3D6    005BD8FA     8BE5                     MOV ESP,EBP
	$-3D4    005BD8FC     5D                       POP EBP
	$-3D3    005BD8FD     C2 0400                  RETN 0x4
	$-3D0    005BD900     55                       PUSH EBP
	$-3CF    005BD901     8BEC                     MOV EBP,ESP
	$-3CD    005BD903     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-3C7    005BD909     6A FF                    PUSH -0x1
	$-3C5    005BD90B     68 5841D800              PUSH PathOfEx.00D84158
	$-3C0    005BD910     50                       PUSH EAX
	$-3BF    005BD911     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-3BC    005BD914     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-3B5    005BD91B     83EC 3C                  SUB ESP,0x3C
	$-3B2    005BD91E     53                       PUSH EBX
	$-3B1    005BD91F     8BD9                     MOV EBX,ECX
	$-3AF    005BD921     83F8 03                  CMP EAX,0x3
	$-3AC    005BD924     74 0E                    JE SHORT PathOfEx.005BD934
	$-3AA    005BD926     83F8 04                  CMP EAX,0x4
	$-3A7    005BD929     74 09                    JE SHORT PathOfEx.005BD934
	$-3A5    005BD92B     83F8 05                  CMP EAX,0x5
	$-3A2    005BD92E     0F85 2A020000            JNZ PathOfEx.005BDB5E
	$-39C    005BD934     57                       PUSH EDI
	$-39B    005BD935     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-398    005BD938     807F 1E 00               CMP BYTE PTR DS:[EDI+0x1E],0x0
	$-394    005BD93C     0F84 1B020000            JE PathOfEx.005BDB5D
	$-38E    005BD942     807F 1F 00               CMP BYTE PTR DS:[EDI+0x1F],0x0
	$-38A    005BD946     0F85 11020000            JNZ PathOfEx.005BDB5D
	$-384    005BD94C     8B83 3C010000            MOV EAX,DWORD PTR DS:[EBX+0x13C]
	$-37E    005BD952     56                       PUSH ESI
	$-37D    005BD953     8B80 14410000            MOV EAX,DWORD PTR DS:[EAX+0x4114]
	$-377    005BD959     8BB0 58010000            MOV ESI,DWORD PTR DS:[EAX+0x158]
	$-371    005BD95F     8975 F0                  MOV DWORD PTR SS:[EBP-0x10],ESI
	$-36E    005BD962     85F6                     TEST ESI,ESI
	$-36C    005BD964     0F84 F2010000            JE PathOfEx.005BDB5C
	$-366    005BD96A     8BCE                     MOV ECX,ESI
	$-364    005BD96C     E8 DF6FD6FF              CALL PathOfEx.00324950
	$-35F    005BD971     8B4F 48                  MOV ECX,DWORD PTR DS:[EDI+0x48]
	$-35C    005BD974     68 5E030000              PUSH 0x35E
	$-357    005BD979     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-354    005BD97C     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-352    005BD97E     FF12                     CALL DWORD PTR DS:[EDX]
	$-350    005BD980     8B4F 48                  MOV ECX,DWORD PTR DS:[EDI+0x48]
	$-34D    005BD983     85C0                     TEST EAX,EAX
	$-34B    005BD985     68 B5140000              PUSH 0x14B5
	$-346    005BD98A     0F9545 0C                SETNE BYTE PTR SS:[EBP+0xC]
	$-342    005BD98E     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-340    005BD990     FF10                     CALL DWORD PTR DS:[EAX]
	$-33E    005BD992     85C0                     TEST EAX,EAX
	$-33C    005BD994     8BCF                     MOV ECX,EDI
	$-33A    005BD996     0F9545 EC                SETNE BYTE PTR SS:[EBP-0x14]
	$-336    005BD99A     E8 11721500              CALL PathOfEx.00714BB0
	$-331    005BD99F     84C0                     TEST AL,AL
	$-32F    005BD9A1     74 37                    JE SHORT PathOfEx.005BD9DA
	$-32D    005BD9A3     8B4F 48                  MOV ECX,DWORD PTR DS:[EDI+0x48]
	$-32A    005BD9A6     33C0                     XOR EAX,EAX
	$-328    005BD9A8     3845 0C                  CMP BYTE PTR SS:[EBP+0xC],AL
	$-325    005BD9AB     0F94C0                   SETE AL
	$-322    005BD9AE     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-320    005BD9B0     05 F8000000              ADD EAX,0xF8
	$-31B    005BD9B5     50                       PUSH EAX
	$-31A    005BD9B6     FF12                     CALL DWORD PTR DS:[EDX]
	$-318    005BD9B8     8BCF                     MOV ECX,EDI
	$-316    005BD9BA     8BF0                     MOV ESI,EAX
	$-314    005BD9BC     E8 7F721500              CALL PathOfEx.00714C40
	$-30F    005BD9C1     0FAFF0                   IMUL ESI,EAX
	$-30C    005BD9C4     B8 1F85EB51              MOV EAX,0x51EB851F
	$-307    005BD9C9     F7EE                     IMUL ESI
	$-305    005BD9CB     8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-302    005BD9CE     C1FA 05                  SAR EDX,0x5
	$-2FF    005BD9D1     8BC2                     MOV EAX,EDX
	$-2FD    005BD9D3     C1E8 1F                  SHR EAX,0x1F
	$-2FA    005BD9D6     03C2                     ADD EAX,EDX
	$-2F8    005BD9D8     EB 07                    JMP SHORT PathOfEx.005BD9E1
	$-2F6    005BD9DA     8BCF                     MOV ECX,EDI
	$-2F4    005BD9DC     E8 5F721500              CALL PathOfEx.00714C40
	$-2EF    005BD9E1     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$-2EC    005BD9E4     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-2E9    005BD9E7     FF75 0C                  PUSH DWORD PTR SS:[EBP+0xC]
	$-2E6    005BD9EA     8945 08                  MOV DWORD PTR SS:[EBP+0x8],EAX
	$-2E3    005BD9ED     E8 3E711100              CALL PathOfEx.006D4B30
	$-2DE    005BD9F2     8BC8                     MOV ECX,EAX
	$-2DC    005BD9F4     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-2D9    005BD9F7     85C0                     TEST EAX,EAX
	$-2D7    005BD9F9     0F88 FE000000            JS PathOfEx.005BDAFD
	$-2D1    005BD9FF     3BC1                     CMP EAX,ECX
	$-2CF    005BDA01     0F8F F6000000            JG PathOfEx.005BDAFD
	$-2C9    005BDA07     8BCE                     MOV ECX,ESI
	$-2C7    005BDA09     E8 62CDD9FF              CALL PathOfEx.0035A770
	$-2C2    005BDA0E     8BC8                     MOV ECX,EAX
	$-2C0    005BDA10     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$-2BE    005BDA12     FF52 58                  CALL DWORD PTR DS:[EDX+0x58]
	$-2BB    005BDA15     57                       PUSH EDI
	$-2BA    005BDA16     8BC8                     MOV ECX,EAX
	$-2B8    005BDA18     E8 835A1500              CALL PathOfEx.007134A0
	$-2B3    005BDA1D     85C0                     TEST EAX,EAX
	$-2B1    005BDA1F     0F84 37010000            JE PathOfEx.005BDB5C
	$-2AB    005BDA25     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-2A8    005BDA28     2B08                     SUB ECX,DWORD PTR DS:[EAX]
	$-2A6    005BDA2A     C1F9 03                  SAR ECX,0x3
	$-2A3    005BDA2D     3B48 0C                  CMP ECX,DWORD PTR DS:[EAX+0xC]
	$-2A0    005BDA30     0F82 26010000            JB PathOfEx.005BDB5C
	$-29A    005BDA36     8B47 34                  MOV EAX,DWORD PTR DS:[EDI+0x34]
	$-297    005BDA39     83E8 01                  SUB EAX,0x1
	$-294    005BDA3C     0F84 91000000            JE PathOfEx.005BDAD3
	$-28E    005BDA42     83E8 01                  SUB EAX,0x1
	$-28B    005BDA45     74 5E                    JE SHORT PathOfEx.005BDAA5
	$-289    005BDA47     83E8 01                  SUB EAX,0x1
	$-286    005BDA4A     74 28                    JE SHORT PathOfEx.005BDA74
	$-284    005BDA4C     68 3CECF000              PUSH PathOfEx.00F0EC3C                                ; UNICODE "Cooldown"
	$-27F    005BDA51     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-27C    005BDA54     E8 F78ED6FF              CALL PathOfEx.00326950
	$-277    005BDA59     51                       PUSH ECX
	$-276    005BDA5A     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-273    005BDA5D     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
	$-26C    005BDA64     50                       PUSH EAX
	$-26B    005BDA65     8BCB                     MOV ECX,EBX
	$-269    005BDA67     E8 64F5FFFF              CALL PathOfEx.005BCFD0
	$-264    005BDA6C     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-261    005BDA6F     E9 E3000000              JMP PathOfEx.005BDB57
	$-25C    005BDA74     8BCE                     MOV ECX,ESI
	$-25A    005BDA76     E8 356FD6FF              CALL PathOfEx.003249B0
	$-255    005BDA7B     6A 07                    PUSH 0x7
	$-253    005BDA7D     8B48 10                  MOV ECX,DWORD PTR DS:[EAX+0x10]
	$-250    005BDA80     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-24E    005BDA82     FF10                     CALL DWORD PTR DS:[EAX]
	$-24C    005BDA84     85C0                     TEST EAX,EAX
	$-24A    005BDA86     0F85 D0000000            JNZ PathOfEx.005BDB5C
	$-244    005BDA8C     68 3CECF000              PUSH PathOfEx.00F0EC3C                                ; UNICODE "Cooldown"
	$-23F    005BDA91     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-23C    005BDA94     E8 B78ED6FF              CALL PathOfEx.00326950
	$-237    005BDA99     C745 FC 03000000         MOV DWORD PTR SS:[EBP-0x4],0x3
	$-230    005BDAA0     E9 A3000000              JMP PathOfEx.005BDB48
	$-22B    005BDAA5     8BCE                     MOV ECX,ESI
	$-229    005BDAA7     E8 046FD6FF              CALL PathOfEx.003249B0
	$-224    005BDAAC     6A 06                    PUSH 0x6
	$-222    005BDAAE     8B48 10                  MOV ECX,DWORD PTR DS:[EAX+0x10]
	$-21F    005BDAB1     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-21D    005BDAB3     FF10                     CALL DWORD PTR DS:[EAX]
	$-21B    005BDAB5     85C0                     TEST EAX,EAX
	$-219    005BDAB7     0F85 9F000000            JNZ PathOfEx.005BDB5C
	$-213    005BDABD     68 3CECF000              PUSH PathOfEx.00F0EC3C                                ; UNICODE "Cooldown"
	$-20E    005BDAC2     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-20B    005BDAC5     E8 868ED6FF              CALL PathOfEx.00326950
	$-206    005BDACA     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$-1FF    005BDAD1     EB 75                    JMP SHORT PathOfEx.005BDB48
	$-1FD    005BDAD3     8BCE                     MOV ECX,ESI
	$-1FB    005BDAD5     E8 D66ED6FF              CALL PathOfEx.003249B0
	$-1F6    005BDADA     6A 05                    PUSH 0x5
	$-1F4    005BDADC     8B48 10                  MOV ECX,DWORD PTR DS:[EAX+0x10]
	$-1F1    005BDADF     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-1EF    005BDAE1     FF10                     CALL DWORD PTR DS:[EAX]
	$-1ED    005BDAE3     85C0                     TEST EAX,EAX
	$-1EB    005BDAE5     75 75                    JNZ SHORT PathOfEx.005BDB5C
	$-1E9    005BDAE7     68 3CECF000              PUSH PathOfEx.00F0EC3C                                ; UNICODE "Cooldown"
	$-1E4    005BDAEC     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-1E1    005BDAEF     E8 5C8ED6FF              CALL PathOfEx.00326950
	$-1DC    005BDAF4     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-1D5    005BDAFB     EB 4B                    JMP SHORT PathOfEx.005BDB48
	$-1D3    005BDAFD     8B4F 48                  MOV ECX,DWORD PTR DS:[EDI+0x48]
	$-1D0    005BDB00     68 61050000              PUSH 0x561
	$-1CB    005BDB05     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-1C9    005BDB07     FF10                     CALL DWORD PTR DS:[EAX]
	$-1C7    005BDB09     85C0                     TEST EAX,EAX
	$-1C5    005BDB0B     74 1A                    JE SHORT PathOfEx.005BDB27
	$-1C3    005BDB0D     6A 01                    PUSH 0x1
	$-1C1    005BDB0F     8BCF                     MOV ECX,EDI
	$-1BF    005BDB11     E8 3A721500              CALL PathOfEx.00714D50
	$-1BA    005BDB16     84C0                     TEST AL,AL
	$-1B8    005BDB18     74 0D                    JE SHORT PathOfEx.005BDB27
	$-1B6    005BDB1A     6A 18                    PUSH 0x18
	$-1B4    005BDB1C     8BCF                     MOV ECX,EDI
	$-1B2    005BDB1E     E8 2D721500              CALL PathOfEx.00714D50
	$-1AD    005BDB23     84C0                     TEST AL,AL
	$-1AB    005BDB25     75 35                    JNZ SHORT PathOfEx.005BDB5C
	$-1A9    005BDB27     807D 0C 00               CMP BYTE PTR SS:[EBP+0xC],0x0
	$-1A5    005BDB2B     B9 2CECF000              MOV ECX,PathOfEx.00F0EC2C                             ; UNICODE "NoMana"
	$-1A0    005BDB30     B8 D0EDF000              MOV EAX,PathOfEx.00F0EDD0                             ; UNICODE "NoLife"
	$-19B    005BDB35     0F44C1                   CMOVE EAX,ECX
	$-198    005BDB38     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-195    005BDB3B     50                       PUSH EAX
	$-194    005BDB3C     E8 0F8ED6FF              CALL PathOfEx.00326950
	$-18F    005BDB41     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-188    005BDB48     51                       PUSH ECX
	$-187    005BDB49     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
	$-184    005BDB4C     8BCB                     MOV ECX,EBX
	$-182    005BDB4E     50                       PUSH EAX
	$-181    005BDB4F     E8 7CF4FFFF              CALL PathOfEx.005BCFD0
	$-17C    005BDB54     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$-179    005BDB57     E8 4487D6FF              CALL PathOfEx.003262A0
	$-174    005BDB5C     5E                       POP ESI
	$-173    005BDB5D     5F                       POP EDI
	$-172    005BDB5E     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-16F    005BDB61     5B                       POP EBX
	$-16E    005BDB62     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-167    005BDB69     8BE5                     MOV ESP,EBP
	$-165    005BDB6B     5D                       POP EBP
	$-164    005BDB6C     C2 0800                  RETN 0x8
	$-161    005BDB6F     CC                       INT3
	$-160    005BDB70     55                       PUSH EBP
	$-15F    005BDB71     8BEC                     MOV EBP,ESP
	$-15D    005BDB73     6A FF                    PUSH -0x1
	$-15B    005BDB75     68 8841D800              PUSH PathOfEx.00D84188
	$-156    005BDB7A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-150    005BDB80     50                       PUSH EAX
	$-14F    005BDB81     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-148    005BDB88     83EC 48                  SUB ESP,0x48
	$-145    005BDB8B     53                       PUSH EBX
	$-144    005BDB8C     56                       PUSH ESI
	$-143    005BDB8D     57                       PUSH EDI
	$-142    005BDB8E     8BF9                     MOV EDI,ECX
	$-140    005BDB90     C745 DC 00000000         MOV DWORD PTR SS:[EBP-0x24],0x0
	$-139    005BDB97     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$-133    005BDB9D     8B80 14410000            MOV EAX,DWORD PTR DS:[EAX+0x4114]
	$-12D    005BDBA3     8B98 58010000            MOV EBX,DWORD PTR DS:[EAX+0x158]
	$-127    005BDBA9     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-125    005BDBAB     85C9                     TEST ECX,ECX
	$-123    005BDBAD     74 05                    JE SHORT PathOfEx.005BDBB4
	$-121    005BDBAF     E8 1C1D0500              CALL PathOfEx.0060F8D0
	$-11C    005BDBB4     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-11A    005BDBB6     C745 E4 CC8DF500         MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00F58DCC         ; ASCII "Actor"
	$-113    005BDBBD     8B70 30                  MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-110    005BDBC0     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-10D    005BDBC3     50                       PUSH EAX
	$-10C    005BDBC4     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-109    005BDBC7     50                       PUSH EAX
	$-108    005BDBC8     8D4E 14                  LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-105    005BDBCB     E8 B06FD6FF              CALL PathOfEx.00324B80
	$-100    005BDBD0     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-FD     005BDBD3     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-FA     005BDBD6     74 10                    JE SHORT PathOfEx.005BDBE8
	$-F8     005BDBD8     8B48 0C                  MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-F5     005BDBDB     83F9 FF                  CMP ECX,-0x1
	$-F2     005BDBDE     74 08                    JE SHORT PathOfEx.005BDBE8
	$-F0     005BDBE0     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-ED     005BDBE3     8B1C88                   MOV EBX,DWORD PTR DS:[EAX+ECX*4]
	$-EA     005BDBE6     EB 02                    JMP SHORT PathOfEx.005BDBEA
	$-E8     005BDBE8     33DB                     XOR EBX,EBX
	$-E6     005BDBEA     803D 58243201 00         CMP BYTE PTR DS:[0x1322458],0x0
	$-DF     005BDBF1     0F85 9E000000            JNZ PathOfEx.005BDC95
	$-D9     005BDBF7     8BB7 34010000            MOV ESI,DWORD PTR DS:[EDI+0x134]
	$-D3     005BDBFD     8D45 F3                  LEA EAX,DWORD PTR SS:[EBP-0xD]
	$-D0     005BDC00     50                       PUSH EAX
	$-CF     005BDC01     68 84A5EF00              PUSH PathOfEx.00EFA584                                ; UNICODE "attack_in_place_key_stops_move"
	$-CA     005BDC06     68 D42CED00              PUSH PathOfEx.00ED2CD4                                ; UNICODE "UI"
	$-C5     005BDC0B     8B8E 80030000            MOV ECX,DWORD PTR DS:[ESI+0x380]
	$-BF     005BDC11     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD],0x0
	$-BB     005BDC15     E8 769F1000              CALL PathOfEx.006C7B90
	$-B6     005BDC1A     807D F3 00               CMP BYTE PTR SS:[EBP-0xD],0x0
	$-B2     005BDC1E     74 75                    JE SHORT PathOfEx.005BDC95
	$-B0     005BDC20     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-AD     005BDC23     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-A6     005BDC2A     50                       PUSH EAX
	$-A5     005BDC2B     6A 0D                    PUSH 0xD
	$-A3     005BDC2D     8BCE                     MOV ECX,ESI
	$-A1     005BDC2F     E8 DC2C1000              CALL PathOfEx.006C0910
	$-9C     005BDC34     8B75 E4                  MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-99     005BDC37     56                       PUSH ESI
	$-98     005BDC38     FF15 0474DD00            CALL DWORD PTR DS:[<&USER32.GetKeyState>]             ; user32.GetKeyState
	$-92     005BDC3E     0FB7C0                   MOVZX EAX,AX
	$-8F     005BDC41     85F6                     TEST ESI,ESI
	$-8D     005BDC43     74 50                    JE SHORT PathOfEx.005BDC95
	$-8B     005BDC45     66:85C0                  TEST AX,AX
	$-88     005BDC48     79 4B                    JNS SHORT PathOfEx.005BDC95
	$-86     005BDC4A     83BF 3C010000 00         CMP DWORD PTR DS:[EDI+0x13C],0x0
	$-7F     005BDC51     C605 58243201 01         MOV BYTE PTR DS:[0x1322458],0x1
	$-78     005BDC58     74 70                    JE SHORT PathOfEx.005BDCCA
	$-76     005BDC5A     85DB                     TEST EBX,EBX
	$-74     005BDC5C     74 6C                    JE SHORT PathOfEx.005BDCCA
	$-72     005BDC5E     8B4B 30                  MOV ECX,DWORD PTR DS:[EBX+0x30]
	$-6F     005BDC61     85C9                     TEST ECX,ECX
	$-6D     005BDC63     74 65                    JE SHORT PathOfEx.005BDCCA
	$-6B     005BDC65     E8 A6C35000              CALL PathOfEx.00ACA010
	$-66     005BDC6A     8B0D 48A50A01            MOV ECX,DWORD PTR DS:[0x10AA548]
	$-60     005BDC70     66:3BC1                  CMP AX,CX
	$-5D     005BDC73     75 55                    JNZ SHORT PathOfEx.005BDCCA
	$-5B     005BDC75     6A 01                    PUSH 0x1
	$-59     005BDC77     0FB7C1                   MOVZX EAX,CX
	$-56     005BDC7A     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$-50     005BDC80     6A 00                    PUSH 0x0
	$-4E     005BDC82     50                       PUSH EAX
	$-4D     005BDC83     E8 880D2200              CALL PathOfEx.007DEA10
	$-48     005BDC88     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$-42     005BDC8E     E8 1D112200              CALL PathOfEx.007DEDB0
	$-3D     005BDC93     EB 35                    JMP SHORT PathOfEx.005BDCCA
	$-3B     005BDC95     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$-35     005BDC9B     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-32     005BDC9E     50                       PUSH EAX
	$-31     005BDC9F     6A 0D                    PUSH 0xD
	$-2F     005BDCA1     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-28     005BDCA8     E8 632C1000              CALL PathOfEx.006C0910
	$-23     005BDCAD     8B75 E4                  MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-20     005BDCB0     56                       PUSH ESI
	$-1F     005BDCB1     FF15 0474DD00            CALL DWORD PTR DS:[<&USER32.GetKeyState>]             ; user32.GetKeyState
	$-19     005BDCB7     0FB7C0                   MOVZX EAX,AX
	$-16     005BDCBA     85F6                     TEST ESI,ESI
	$-14     005BDCBC     74 05                    JE SHORT PathOfEx.005BDCC3
	$-12     005BDCBE     66:85C0                  TEST AX,AX
	$-F      005BDCC1     78 07                    JS SHORT PathOfEx.005BDCCA
	$-D      005BDCC3     C605 58243201 00         MOV BYTE PTR DS:[0x1322458],0x0
	$-6      005BDCCA     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$ ==>    005BDCD0     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$+6      005BDCD6     0F84 4F050000            JE PathOfEx.005BE22B
	$+C      005BDCDC     83BF 780C0000 00         CMP DWORD PTR DS:[EDI+0xC78],0x0
	$+13     005BDCE3     0F85 42050000            JNZ PathOfEx.005BE22B
	$+19     005BDCE9     83BF 64190000 00         CMP DWORD PTR DS:[EDI+0x1964],0x0
	$+20     005BDCF0     0F84 35050000            JE PathOfEx.005BE22B
	$+26     005BDCF6     8BCF                     MOV ECX,EDI
	$+28     005BDCF8     E8 E352FDFF              CALL PathOfEx.00592FE0
	$+2D     005BDCFD     84C0                     TEST AL,AL
	$+2F     005BDCFF     0F84 26050000            JE PathOfEx.005BE22B
	$+35     005BDD05     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$+3B     005BDD0B     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$+41     005BDD11     75 26                    JNZ SHORT PathOfEx.005BDD39
	$+43     005BDD13     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
	$+4A     005BDD1A     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+4D     005BDD1D     C745 DC 00000000         MOV DWORD PTR SS:[EBP-0x24],0x0
	$+54     005BDD24     BB 03000000              MOV EBX,0x3
	$+59     005BDD29     C745 C8 00000000         MOV DWORD PTR SS:[EBP-0x38],0x0
	$+60     005BDD30     C745 CC 00000000         MOV DWORD PTR SS:[EBP-0x34],0x0
	$+67     005BDD37     EB 17                    JMP SHORT PathOfEx.005BDD50
	$+69     005BDD39     8B87 68150000            MOV EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$+6F     005BDD3F     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$+72     005BDD42     51                       PUSH ECX
	$+73     005BDD43     8D48 F8                  LEA ECX,DWORD PTR DS:[EAX-0x8]
	$+76     005BDD46     E8 756FE8FF              CALL PathOfEx.00444CC0
	$+7B     005BDD4B     BB 04000000              MOV EBX,0x4
	$+80     005BDD50     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$+82     005BDD52     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+85     005BDD55     8975 E0                  MOV DWORD PTR SS:[EBP-0x20],ESI
	$+88     005BDD58     894D E4                  MOV DWORD PTR SS:[EBP-0x1C],ECX
	$+8B     005BDD5B     C700 00000000            MOV DWORD PTR DS:[EAX],0x0
	$+91     005BDD61     C740 04 00000000         MOV DWORD PTR DS:[EAX+0x4],0x0
	$+98     005BDD68     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+9F     005BDD6F     F6C3 04                  TEST BL,0x4
	$+A2     005BDD72     74 0B                    JE SHORT PathOfEx.005BDD7F
	$+A4     005BDD74     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$+A7     005BDD77     83E3 FB                  AND EBX,0xFFFFFFFB
	$+AA     005BDD7A     E8 9157D6FF              CALL PathOfEx.00323510
	$+AF     005BDD7F     F6C3 02                  TEST BL,0x2
	$+B2     005BDD82     74 0B                    JE SHORT PathOfEx.005BDD8F
	$+B4     005BDD84     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+B7     005BDD87     83E3 FD                  AND EBX,0xFFFFFFFD
	$+BA     005BDD8A     E8 8157D6FF              CALL PathOfEx.00323510
	$+BF     005BDD8F     F6C3 01                  TEST BL,0x1
	$+C2     005BDD92     74 08                    JE SHORT PathOfEx.005BDD9C
	$+C4     005BDD94     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+C7     005BDD97     E8 7457D6FF              CALL PathOfEx.00323510
	$+CC     005BDD9C     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$+D2     005BDDA2     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+D5     005BDDA5     50                       PUSH EAX
	$+D6     005BDDA6     32DB                     XOR BL,BL
	$+D8     005BDDA8     C745 DC 0D000000         MOV DWORD PTR SS:[EBP-0x24],0xD
	$+DF     005BDDAF     E8 5C2E1000              CALL PathOfEx.006C0C10
	$+E4     005BDDB4     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$+EA     005BDDBA     8AF8                     MOV BH,AL
	$+EC     005BDDBC     E8 9F1A1000              CALL PathOfEx.006BF860
	$+F1     005BDDC1     84C0                     TEST AL,AL
	$+F3     005BDDC3     75 04                    JNZ SHORT PathOfEx.005BDDC9
	$+F5     005BDDC5     84FF                     TEST BH,BH
	$+F7     005BDDC7     74 3E                    JE SHORT PathOfEx.005BDE07
	$+F9     005BDDC9     85F6                     TEST ESI,ESI
	$+FB     005BDDCB     0F94C0                   SETE AL
	$+FE     005BDDCE     34 01                    XOR AL,0x1
	$+100    005BDDD0     74 35                    JE SHORT PathOfEx.005BDE07
	$+102    005BDDD2     0FB746 02                MOVZX EAX,WORD PTR DS:[ESI+0x2]
	$+106    005BDDD6     66:3B05 48A50A01         CMP AX,WORD PTR DS:[0x10AA548]
	$+10D    005BDDDD     0F94C2                   SETE DL
	$+110    005BDDE0     66:3B05 58A50A01         CMP AX,WORD PTR DS:[0x10AA558]
	$+117    005BDDE7     0F94C0                   SETE AL
	$+11A    005BDDEA     84D2                     TEST DL,DL
	$+11C    005BDDEC     74 17                    JE SHORT PathOfEx.005BDE05
	$+11E    005BDDEE     84FF                     TEST BH,BH
	$+120    005BDDF0     74 15                    JE SHORT PathOfEx.005BDE07
	$+122    005BDDF2     84C0                     TEST AL,AL
	$+124    005BDDF4     75 0F                    JNZ SHORT PathOfEx.005BDE05
	$+126    005BDDF6     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$+12C    005BDDFC     E8 8F1A1000              CALL PathOfEx.006BF890
	$+131    005BDE01     84C0                     TEST AL,AL
	$+133    005BDE03     74 02                    JE SHORT PathOfEx.005BDE07
	$+135    005BDE05     B3 01                    MOV BL,0x1
	$+137    005BDE07     6A 11                    PUSH 0x11
	$+139    005BDE09     FF15 0474DD00            CALL DWORD PTR DS:[<&USER32.GetKeyState>]             ; user32.GetKeyState
	$+13F    005BDE0F     0FB7C0                   MOVZX EAX,AX
	$+142    005BDE12     66:85C0                  TEST AX,AX
	$+145    005BDE15     79 03                    JNS SHORT PathOfEx.005BDE1A
	$+147    005BDE17     80CB 04                  OR BL,0x4
	$+14A    005BDE1A     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$+150    005BDE20     8B80 2C0A0000            MOV EAX,DWORD PTR DS:[EAX+0xA2C]
	$+156    005BDE26     85C0                     TEST EAX,EAX
	$+158    005BDE28     74 0D                    JE SHORT PathOfEx.005BDE37
	$+15A    005BDE2A     8A80 54070000            MOV AL,BYTE PTR DS:[EAX+0x754]
	$+160    005BDE30     84C0                     TEST AL,AL
	$+162    005BDE32     74 03                    JE SHORT PathOfEx.005BDE37
	$+164    005BDE34     80CB 02                  OR BL,0x2
	$+167    005BDE37     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$+16D    005BDE3D     E8 EE191000              CALL PathOfEx.006BF830
	$+172    005BDE42     8ACB                     MOV CL,BL
	$+174    005BDE44     80C9 08                  OR CL,0x8
	$+177    005BDE47     84C0                     TEST AL,AL
	$+179    005BDE49     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$+17F    005BDE4F     0FB6D1                   MOVZX EDX,CL
	$+182    005BDE52     0FB6CB                   MOVZX ECX,BL
	$+185    005BDE55     0F44D1                   CMOVE EDX,ECX
	$+188    005BDE58     8ADA                     MOV BL,DL
	$+18A    005BDE5A     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]
	$+190    005BDE60     75 07                    JNZ SHORT PathOfEx.005BDE69
	$+192    005BDE62     B9 29000000              MOV ECX,0x29
	$+197    005BDE67     EB 09                    JMP SHORT PathOfEx.005BDE72
	$+199    005BDE69     8B87 68150000            MOV EAX,DWORD PTR DS:[EDI+0x1568]
	$+19F    005BDE6F     8B48 F4                  MOV ECX,DWORD PTR DS:[EAX-0xC]
	$+1A2    005BDE72     C745 D0 00000000         MOV DWORD PTR SS:[EBP-0x30],0x0
	$+1A9    005BDE79     C745 D4 00000000         MOV DWORD PTR SS:[EBP-0x2C],0x0
	$+1B0    005BDE80     51                       PUSH ECX
	$+1B1    005BDE81     8AC3                     MOV AL,BL
	$+1B3    005BDE83     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+1B7    005BDE87     24 01                    AND AL,0x1
	$+1B9    005BDE89     0FB6C0                   MOVZX EAX,AL
	$+1BC    005BDE8C     50                       PUSH EAX
	$+1BD    005BDE8D     51                       PUSH ECX
	$+1BE    005BDE8E     83EC 08                  SUB ESP,0x8
	$+1C1    005BDE91     8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+1C4    005BDE94     8BCC                     MOV ECX,ESP
	$+1C6    005BDE96     50                       PUSH EAX
	$+1C7    005BDE97     E8 049ED7FF              CALL PathOfEx.00337CA0
	$+1CC    005BDE9C     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+1CF    005BDE9F     8BCF                     MOV ECX,EDI
	$+1D1    005BDEA1     50                       PUSH EAX
	$+1D2    005BDEA2     E8 E9100000              CALL PathOfEx.005BEF90
	$+1D7    005BDEA7     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$+1DB    005BDEAB     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+1DE    005BDEAE     8945 BC                  MOV DWORD PTR SS:[EBP-0x44],EAX
	$+1E1    005BDEB1     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+1E4    005BDEB4     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+1E7    005BDEB7     8945 C0                  MOV DWORD PTR SS:[EBP-0x40],EAX
	$+1EA    005BDEBA     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+1ED    005BDEBD     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
	$+1F0    005BDEC0     8B45 B8                  MOV EAX,DWORD PTR SS:[EBP-0x48]
	$+1F3    005BDEC3     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+1F6    005BDEC6     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+1F9    005BDEC9     50                       PUSH EAX
	$+1FA    005BDECA     E8 E1510000              CALL PathOfEx.005C30B0
	$+1FF    005BDECF     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+202    005BDED2     85C0                     TEST EAX,EAX
	$+204    005BDED4     75 08                    JNZ SHORT PathOfEx.005BDEDE
	$+206    005BDED6     8D70 01                  LEA ESI,DWORD PTR DS:[EAX+0x1]
	$+209    005BDED9     E9 22030000              JMP PathOfEx.005BE200
	$+20E    005BDEDE     3B87 7C150000            CMP EAX,DWORD PTR DS:[EDI+0x157C]
	$+214    005BDEE4     8D8F 74150000            LEA ECX,DWORD PTR DS:[EDI+0x1574]
	$+21A    005BDEEA     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
	$+21D    005BDEED     50                       PUSH EAX
	$+21E    005BDEEE     0F9445 F2                SETE BYTE PTR SS:[EBP-0xE]
	$+222    005BDEF2     E8 C96DE8FF              CALL PathOfEx.00444CC0
	$+227    005BDEF7     8B4D D0                  MOV ECX,DWORD PTR SS:[EBP-0x30]
	$+22A    005BDEFA     3B08                     CMP ECX,DWORD PTR DS:[EAX]
	$+22C    005BDEFC     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$+22F    005BDEFF     0F9445 F3                SETE BYTE PTR SS:[EBP-0xD]
	$+233    005BDF03     E8 0856D6FF              CALL PathOfEx.00323510
	$+238    005BDF08     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+23B    005BDF0B     3B8F 70150000            CMP ECX,DWORD PTR DS:[EDI+0x1570]
	$+241    005BDF11     8A55 F3                  MOV DL,BYTE PTR SS:[EBP-0xD]
	$+244    005BDF14     0F94C0                   SETE AL
	$+247    005BDF17     807D F2 00               CMP BYTE PTR SS:[EBP-0xE],0x0
	$+24B    005BDF1B     8845 F1                  MOV BYTE PTR SS:[EBP-0xF],AL
	$+24E    005BDF1E     74 13                    JE SHORT PathOfEx.005BDF33
	$+250    005BDF20     84D2                     TEST DL,DL
	$+252    005BDF22     74 73                    JE SHORT PathOfEx.005BDF97
	$+254    005BDF24     84C0                     TEST AL,AL
	$+256    005BDF26     74 0B                    JE SHORT PathOfEx.005BDF33
	$+258    005BDF28     0FB6C3                   MOVZX EAX,BL
	$+25B    005BDF2B     3B87 88150000            CMP EAX,DWORD PTR DS:[EDI+0x1588]
	$+261    005BDF31     74 57                    JE SHORT PathOfEx.005BDF8A
	$+263    005BDF33     84D2                     TEST DL,DL
	$+265    005BDF35     74 60                    JE SHORT PathOfEx.005BDF97
	$+267    005BDF37     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+26A    005BDF3A     85D2                     TEST EDX,EDX
	$+26C    005BDF3C     74 59                    JE SHORT PathOfEx.005BDF97
	$+26E    005BDF3E     6A 3A                    PUSH 0x3A
	$+270    005BDF40     8BCA                     MOV ECX,EDX
	$+272    005BDF42     E8 096E1500              CALL PathOfEx.00714D50
	$+277    005BDF47     84C0                     TEST AL,AL
	$+279    005BDF49     74 49                    JE SHORT PathOfEx.005BDF94
	$+27B    005BDF4B     807D F2 00               CMP BYTE PTR SS:[EBP-0xE],0x0
	$+27F    005BDF4F     74 16                    JE SHORT PathOfEx.005BDF67
	$+281    005BDF51     807D F1 00               CMP BYTE PTR SS:[EBP-0xF],0x0
	$+285    005BDF55     75 33                    JNZ SHORT PathOfEx.005BDF8A
	$+287    005BDF57     837D E8 03               CMP DWORD PTR SS:[EBP-0x18],0x3
	$+28B    005BDF5B     75 37                    JNZ SHORT PathOfEx.005BDF94
	$+28D    005BDF5D     BE 02000000              MOV ESI,0x2
	$+292    005BDF62     E9 99020000              JMP PathOfEx.005BE200
	$+297    005BDF67     837D E8 03               CMP DWORD PTR SS:[EBP-0x18],0x3
	$+29B    005BDF6B     75 27                    JNZ SHORT PathOfEx.005BDF94
	$+29D    005BDF6D     83BF 7C150000 04         CMP DWORD PTR DS:[EDI+0x157C],0x4
	$+2A4    005BDF74     75 1E                    JNZ SHORT PathOfEx.005BDF94
	$+2A6    005BDF76     C787 70150000 00000000   MOV DWORD PTR DS:[EDI+0x1570],0x0
	$+2B0    005BDF80     C787 7C150000 03000000   MOV DWORD PTR DS:[EDI+0x157C],0x3
	$+2BA    005BDF8A     BE 02000000              MOV ESI,0x2
	$+2BF    005BDF8F     E9 6C020000              JMP PathOfEx.005BE200
	$+2C4    005BDF94     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+2C7    005BDF97     398F 48190000            CMP DWORD PTR DS:[EDI+0x1948],ECX
	$+2CD    005BDF9D     74 0D                    JE SHORT PathOfEx.005BDFAC
	$+2CF    005BDF9F     6A 00                    PUSH 0x0
	$+2D1    005BDFA1     51                       PUSH ECX
	$+2D2    005BDFA2     8BCF                     MOV ECX,EDI
	$+2D4    005BDFA4     E8 37E0FFFF              CALL PathOfEx.005BBFE0
	$+2D9    005BDFA9     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+2DC    005BDFAC     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+2DF    005BDFAF     85D2                     TEST EDX,EDX
	$+2E1    005BDFB1     74 1D                    JE SHORT PathOfEx.005BDFD0
	$+2E3    005BDFB3     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+2E9    005BDFB9     80B8 EB410000 00         CMP BYTE PTR DS:[EAX+0x41EB],0x0
	$+2F0    005BDFC0     75 0E                    JNZ SHORT PathOfEx.005BDFD0
	$+2F2    005BDFC2     FF75 E8                  PUSH DWORD PTR SS:[EBP-0x18]
	$+2F5    005BDFC5     8BCF                     MOV ECX,EDI
	$+2F7    005BDFC7     52                       PUSH EDX
	$+2F8    005BDFC8     E8 33F9FFFF              CALL PathOfEx.005BD900
	$+2FD    005BDFCD     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+300    005BDFD0     F3:0F1087 28190000       MOVSS XMM0,DWORD PTR DS:[EDI+0x1928]
	$+308    005BDFD8     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$+310    005BDFE0     F3:0F2CF0                CVTTSS2SI ESI,XMM0
	$+314    005BDFE4     F3:0F1087 2C190000       MOVSS XMM0,DWORD PTR DS:[EDI+0x192C]
	$+31C    005BDFEC     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$+324    005BDFF4     8975 D8                  MOV DWORD PTR SS:[EBP-0x28],ESI
	$+327    005BDFF7     F3:0F2CC0                CVTTSS2SI EAX,XMM0
	$+32B    005BDFFB     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+32E    005BDFFE     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+331    005BE001     48                       DEC EAX
	$+332    005BE002     83F8 04                  CMP EAX,0x4
	$+335    005BE005     0F87 B2010000            JA PathOfEx.005BE1BD
	$+33B    005BE00B     FF2485 40E25B00          JMP DWORD PTR DS:[EAX*4+0x5BE240]
	$+342    005BE012     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+348    005BE018     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+34B    005BE01B     53                       PUSH EBX
	$+34C    005BE01C     50                       PUSH EAX
	$+34D    005BE01D     A1 48A50A01              MOV EAX,DWORD PTR DS:[0x10AA548]
	$+352    005BE022     6A 00                    PUSH 0x0
	$+354    005BE024     25 FF3F0000              AND EAX,0x3FFF
	$+359    005BE029     50                       PUSH EAX
	$+35A    005BE02A     E8 110A2200              CALL PathOfEx.007DEA40
	$+35F    005BE02F     E9 89010000              JMP PathOfEx.005BE1BD
	$+364    005BE034     83BF 48190000 00         CMP DWORD PTR DS:[EDI+0x1948],0x0
	$+36B    005BE03B     74 33                    JE SHORT PathOfEx.005BE070
	$+36D    005BE03D     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+370    005BE040     50                       PUSH EAX
	$+371    005BE041     E8 7A4EFFFF              CALL PathOfEx.005B2EC0
	$+376    005BE046     84C0                     TEST AL,AL
	$+378    005BE048     74 26                    JE SHORT PathOfEx.005BE070
	$+37A    005BE04A     FFB7 48190000            PUSH DWORD PTR DS:[EDI+0x1948]
	$+380    005BE050     8BCF                     MOV ECX,EDI
	$+382    005BE052     E8 39E1FFFF              CALL PathOfEx.005BC190
	$+387    005BE057     84C0                     TEST AL,AL
	$+389    005BE059     74 15                    JE SHORT PathOfEx.005BE070
	$+38B    005BE05B     8B87 48190000            MOV EAX,DWORD PTR DS:[EDI+0x1948]
	$+391    005BE061     8B40 2C                  MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+394    005BE064     8B70 10                  MOV ESI,DWORD PTR DS:[EAX+0x10]
	$+397    005BE067     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+39A    005BE06A     8975 D8                  MOV DWORD PTR SS:[EBP-0x28],ESI
	$+39D    005BE06D     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+3A0    005BE070     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+3A6    005BE076     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+3A9    005BE079     53                       PUSH EBX
	$+3AA    005BE07A     50                       PUSH EAX
	$+3AB    005BE07B     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+3AE    005BE07E     6A 00                    PUSH 0x0
	$+3B0    005BE080     0FB700                   MOVZX EAX,WORD PTR DS:[EAX]
	$+3B3    005BE083     50                       PUSH EAX
	$+3B4    005BE084     E8 B7092200              CALL PathOfEx.007DEA40
	$+3B9    005BE089     E9 2F010000              JMP PathOfEx.005BE1BD
	$+3BE    005BE08E     85C9                     TEST ECX,ECX
	$+3C0    005BE090     0F84 27010000            JE PathOfEx.005BE1BD
	$+3C6    005BE096     8B97 3C010000            MOV EDX,DWORD PTR DS:[EDI+0x13C]
	$+3CC    005BE09C     80BA 38450000 00         CMP BYTE PTR DS:[EDX+0x4538],0x0
	$+3D3    005BE0A3     74 3C                    JE SHORT PathOfEx.005BE0E1
	$+3D5    005BE0A5     80BF A61A0000 00         CMP BYTE PTR DS:[EDI+0x1AA6],0x0
	$+3DC    005BE0AC     74 33                    JE SHORT PathOfEx.005BE0E1
	$+3DE    005BE0AE     8B87 A81A0000            MOV EAX,DWORD PTR DS:[EDI+0x1AA8]
	$+3E4    005BE0B4     85C0                     TEST EAX,EAX
	$+3E6    005BE0B6     0F84 01010000            JE PathOfEx.005BE1BD
	$+3EC    005BE0BC     51                       PUSH ECX
	$+3ED    005BE0BD     8BC8                     MOV ECX,EAX
	$+3EF    005BE0BF     E8 1C0ADDFF              CALL PathOfEx.0038EAE0
	$+3F4    005BE0C4     84C0                     TEST AL,AL
	$+3F6    005BE0C6     0F84 F1000000            JE PathOfEx.005BE1BD
	$+3FC    005BE0CC     8B8F A81A0000            MOV ECX,DWORD PTR DS:[EDI+0x1AA8]
	$+402    005BE0D2     6A 01                    PUSH 0x1
	$+404    005BE0D4     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+407    005BE0D7     E8 940ADDFF              CALL PathOfEx.0038EB70
	$+40C    005BE0DC     E9 DC000000              JMP PathOfEx.005BE1BD
	$+411    005BE0E1     A1 54A50A01              MOV EAX,DWORD PTR DS:[0x10AA554]
	$+416    005BE0E6     53                       PUSH EBX
	$+417    005BE0E7     FF71 20                  PUSH DWORD PTR DS:[ECX+0x20]
	$+41A    005BE0EA     25 FF3F0000              AND EAX,0x3FFF
	$+41F    005BE0EF     8BCA                     MOV ECX,EDX
	$+421    005BE0F1     50                       PUSH EAX
	$+422    005BE0F2     E8 19092200              CALL PathOfEx.007DEA10
	$+427    005BE0F7     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+42A    005BE0FA     898F 5C190000            MOV DWORD PTR DS:[EDI+0x195C],ECX
	$+430    005BE100     E8 6BEFF7FF              CALL PathOfEx.0053D070
	$+435    005BE105     84C0                     TEST AL,AL
	$+437    005BE107     0F84 B0000000            JE PathOfEx.005BE1BD
	$+43D    005BE10D     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+443    005BE113     E8 980C2200              CALL PathOfEx.007DEDB0
	$+448    005BE118     E9 A0000000              JMP PathOfEx.005BE1BD
	$+44D    005BE11D     85C9                     TEST ECX,ECX
	$+44F    005BE11F     0F84 98000000            JE PathOfEx.005BE1BD
	$+455    005BE125     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+45B    005BE12B     80B8 EB410000 00         CMP BYTE PTR DS:[EAX+0x41EB],0x0
	$+462    005BE132     74 48                    JE SHORT PathOfEx.005BE17C
	$+464    005BE134     E8 C7C0DAFF              CALL PathOfEx.0036A200
	$+469    005BE139     84C0                     TEST AL,AL
	$+46B    005BE13B     74 14                    JE SHORT PathOfEx.005BE151
	$+46D    005BE13D     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+470    005BE140     8BCF                     MOV ECX,EDI
	$+472    005BE142     E8 09170000              CALL PathOfEx.005BF850
	$+477    005BE147     BE 03000000              MOV ESI,0x3
	$+47C    005BE14C     E9 AF000000              JMP PathOfEx.005BE200
	$+481    005BE151     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+484    005BE154     66:8B40 02               MOV AX,WORD PTR DS:[EAX+0x2]
	$+488    005BE158     66:3B05 54A50A01         CMP AX,WORD PTR DS:[0x10AA554]
	$+48F    005BE15F   ^ 75 E6                    JNZ SHORT PathOfEx.005BE147
	$+491    005BE161     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+494    005BE164     E8 D73B0000              CALL PathOfEx.005C1D40
	$+499    005BE169     84C0                     TEST AL,AL
	$+49B    005BE16B     75 0C                    JNZ SHORT PathOfEx.005BE179
	$+49D    005BE16D     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+4A0    005BE170     E8 4BEFF7FF              CALL PathOfEx.0053D0C0
	$+4A5    005BE175     84C0                     TEST AL,AL
	$+4A7    005BE177   ^ 74 CE                    JE SHORT PathOfEx.005BE147
	$+4A9    005BE179     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+4AC    005BE17C     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+4AF    005BE17F     53                       PUSH EBX
	$+4B0    005BE180     FF71 20                  PUSH DWORD PTR DS:[ECX+0x20]
	$+4B3    005BE183     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+4B9    005BE189     0FB700                   MOVZX EAX,WORD PTR DS:[EAX]
	$+4BC    005BE18C     EB 29                    JMP SHORT PathOfEx.005BE1B7
	$+4BE    005BE18E     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+4C1    005BE191     85D2                     TEST EDX,EDX
	$+4C3    005BE193     74 28                    JE SHORT PathOfEx.005BE1BD
	$+4C5    005BE195     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+4CB    005BE19B     80B9 EB410000 00         CMP BYTE PTR DS:[ECX+0x41EB],0x0
	$+4D2    005BE1A2     75 19                    JNZ SHORT PathOfEx.005BE1BD
	$+4D4    005BE1A4     8B87 38010000            MOV EAX,DWORD PTR DS:[EDI+0x138]
	$+4DA    005BE1AA     53                       PUSH EBX
	$+4DB    005BE1AB     8B80 58010000            MOV EAX,DWORD PTR DS:[EAX+0x158]
	$+4E1    005BE1B1     FF70 20                  PUSH DWORD PTR DS:[EAX+0x20]
	$+4E4    005BE1B4     0FB702                   MOVZX EAX,WORD PTR DS:[EDX]
	$+4E7    005BE1B7     50                       PUSH EAX
	$+4E8    005BE1B8     E8 53082200              CALL PathOfEx.007DEA10
	$+4ED    005BE1BD     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+4F0    005BE1C0     8D8F 70150000            LEA ECX,DWORD PTR DS:[EDI+0x1570]
	$+4F6    005BE1C6     8987 84150000            MOV DWORD PTR DS:[EDI+0x1584],EAX
	$+4FC    005BE1CC     0FB6C3                   MOVZX EAX,BL
	$+4FF    005BE1CF     8987 88150000            MOV DWORD PTR DS:[EDI+0x1588],EAX
	$+505    005BE1D5     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+508    005BE1D8     50                       PUSH EAX
	$+509    005BE1D9     89B7 80150000            MOV DWORD PTR DS:[EDI+0x1580],ESI
	$+50F    005BE1DF     E8 8C3D0000              CALL PathOfEx.005C1F70
	$+514    005BE1E4     E8 87330400              CALL PathOfEx.00601570
	$+519    005BE1E9     83C0 43                  ADD EAX,0x43
	$+51C    005BE1EC     BE 04000000              MOV ESI,0x4
	$+521    005BE1F1     8987 90150000            MOV DWORD PTR DS:[EDI+0x1590],EAX
	$+527    005BE1F7     83D2 00                  ADC EDX,0x0
	$+52A    005BE1FA     8997 94150000            MOV DWORD PTR DS:[EDI+0x1594],EDX
	$+530    005BE200     8D4D AC                  LEA ECX,DWORD PTR SS:[EBP-0x54]
	$+533    005BE203     E8 E8B2D7FF              CALL PathOfEx.003394F0
	$+538    005BE208     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+53B    005BE20B     E8 0053D6FF              CALL PathOfEx.00323510
	$+540    005BE210     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+543    005BE213     E8 F852D6FF              CALL PathOfEx.00323510
	$+548    005BE218     8BC6                     MOV EAX,ESI
	$+54A    005BE21A     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+54D    005BE21D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+554    005BE224     5F                       POP EDI
	$+555    005BE225     5E                       POP ESI
	$+556    005BE226     5B                       POP EBX
	$+557    005BE227     8BE5                     MOV ESP,EBP
	$+559    005BE229     5D                       POP EBP
	$+55A    005BE22A     C3                       RETN
	$+55B    005BE22B     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+55E    005BE22E     33C0                     XOR EAX,EAX
	$+560    005BE230     5F                       POP EDI
	$+561    005BE231     5E                       POP ESI
	$+562    005BE232     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+569    005BE239     5B                       POP EBX
	$+56A    005BE23A     8BE5                     MOV ESP,EBP
	$+56C    005BE23C     5D                       POP EBP
	$+56D    005BE23D     C3                       RETN
	$+56E    005BE23E     66:90                    NOP
	$+570    005BE240     12E0                     ADC AH,AL
	$+572    005BE242     5B                       POP EBX
	$+573    005BE243     008E E05B0034            ADD BYTE PTR DS:[ESI+0x34005BE0],CL
	$+579    005BE249     E0 5B                    LOOPDNE SHORT PathOfEx.005BE2A6
	$+57B    005BE24B     001D E15B008E            ADD BYTE PTR DS:[0x8E005BE1],BL
	$+581    005BE251     E1 5B                    LOOPDE SHORT PathOfEx.005BE2AE
	$+583    005BE253     00CC                     ADD AH,CL
	$+585    005BE255     CC                       INT3
	$+586    005BE256     CC                       INT3
	$+587    005BE257     CC                       INT3
	$+588    005BE258     CC                       INT3
	$+589    005BE259     CC                       INT3
	$+58A    005BE25A     CC                       INT3
	$+58B    005BE25B     CC                       INT3
	$+58C    005BE25C     CC                       INT3
	$+58D    005BE25D     CC                       INT3
	$+58E    005BE25E     CC                       INT3
	$+58F    005BE25F     CC                       INT3
	$+590    005BE260     55                       PUSH EBP
	$+591    005BE261     8BEC                     MOV EBP,ESP
	$+593    005BE263     6A FF                    PUSH -0x1
	$+595    005BE265     68 C041D800              PUSH PathOfEx.00D841C0
	$+59A    005BE26A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+5A0    005BE270     50                       PUSH EAX
	$+5A1    005BE271     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+5A8    005BE278     83EC 30                  SUB ESP,0x30
	$+5AB    005BE27B     56                       PUSH ESI
	$+5AC    005BE27C     57                       PUSH EDI
	$+5AD    005BE27D     8BF1                     MOV ESI,ECX
	$+5AF    005BE27F     E8 5C4BFFFF              CALL PathOfEx.005B2DE0
	$+5B4    005BE284     8B8E 64150000            MOV ECX,DWORD PTR DS:[ESI+0x1564]
	$+5BA    005BE28A     8DBE 64150000            LEA EDI,DWORD PTR DS:[ESI+0x1564]
	$+5C0    005BE290     8845 F2                  MOV BYTE PTR SS:[EBP-0xE],AL
	$+5C3    005BE293     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$+5CA    005BE29A     3B4F 04                  CMP ECX,DWORD PTR DS:[EDI+0x4]
	$+5CD    005BE29D     0F84 69010000            JE PathOfEx.005BE40C
	$+5D3    005BE2A3     8B86 68150000            MOV EAX,DWORD PTR DS:[ESI+0x1568]
	$+5D9    005BE2A9     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+5DC    005BE2AC     53                       PUSH EBX
	$+5DD    005BE2AD     51                       PUSH ECX
	$+5DE    005BE2AE     8D48 F8                  LEA ECX,DWORD PTR DS:[EAX-0x8]
	$+5E1    005BE2B1     E8 0A6AE8FF              CALL PathOfEx.00444CC0
	$+5E6    005BE2B6     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+5ED    005BE2BD     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+5F0    005BE2C0     8B96 68150000            MOV EDX,DWORD PTR DS:[ESI+0x1568]
	$+5F6    005BE2C6     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$+5F8    005BE2C8     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+5FB    005BE2CB     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+5FE    005BE2CE     50                       PUSH EAX
	$+5FF    005BE2CF     8975 DC                  MOV DWORD PTR SS:[EBP-0x24],ESI
	$+602    005BE2D2     E8 694E0000              CALL PathOfEx.005C3140
	$+607    005BE2D7     83C4 04                  ADD ESP,0x4
	$+60A    005BE2DA     8BCF                     MOV ECX,EDI
	$+60C    005BE2DC     FF77 04                  PUSH DWORD PTR DS:[EDI+0x4]
	$+60F    005BE2DF     50                       PUSH EAX
	$+610    005BE2E0     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+613    005BE2E3     50                       PUSH EAX
	$+614    005BE2E4     E8 671C0000              CALL PathOfEx.005BFF50
	$+619    005BE2E9     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+61B    005BE2EB     3B47 04                  CMP EAX,DWORD PTR DS:[EDI+0x4]
	$+61E    005BE2EE     0F85 80000000            JNZ PathOfEx.005BE374
	$+624    005BE2F4     8B8E 3C010000            MOV ECX,DWORD PTR DS:[ESI+0x13C]
	$+62A    005BE2FA     E8 B10A2200              CALL PathOfEx.007DEDB0
	$+62F    005BE2FF     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+632    005BE302     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$+639    005BE309     50                       PUSH EAX
	$+63A    005BE30A     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+63D    005BE30D     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+644    005BE314     50                       PUSH EAX
	$+645    005BE315     8D55 E4                  LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+648    005BE318     C745 DC 00000000         MOV DWORD PTR SS:[EBP-0x24],0x0
	$+64F    005BE31F     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$+652    005BE322     C745 E0 00000000         MOV DWORD PTR SS:[EBP-0x20],0x0
	$+659    005BE329     E8 F22E0000              CALL PathOfEx.005C1220
	$+65E    005BE32E     83C4 08                  ADD ESP,0x8
	$+661    005BE331     8D8E 70150000            LEA ECX,DWORD PTR DS:[ESI+0x1570]
	$+667    005BE337     50                       PUSH EAX
	$+668    005BE338     E8 A3340000              CALL PathOfEx.005C17E0
	$+66D    005BE33D     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+671    005BE341     83CF FF                  OR EDI,0xFFFFFFFF
	$+674    005BE344     8B4D CC                  MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+677    005BE347     85C9                     TEST ECX,ECX
	$+679    005BE349     74 0E                    JE SHORT PathOfEx.005BE359
	$+67B    005BE34B     8BC7                     MOV EAX,EDI
	$+67D    005BE34D     F0:0FC141 08             LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$+682    005BE352     75 05                    JNZ SHORT PathOfEx.005BE359
	$+684    005BE354     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+686    005BE356     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+689    005BE359     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$+68D    005BE35D     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+690    005BE360     85C9                     TEST ECX,ECX
	$+692    005BE362     74 6E                    JE SHORT PathOfEx.005BE3D2
	$+694    005BE364     8BC7                     MOV EAX,EDI
	$+696    005BE366     F0:0FC141 08             LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$+69B    005BE36B     75 65                    JNZ SHORT PathOfEx.005BE3D2
	$+69D    005BE36D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+69F    005BE36F     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+6A2    005BE372     EB 5E                    JMP SHORT PathOfEx.005BE3D2
	$+6A4    005BE374     8B86 68150000            MOV EAX,DWORD PTR DS:[ESI+0x1568]
	$+6AA    005BE37A     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+6AD    005BE37D     51                       PUSH ECX
	$+6AE    005BE37E     8D48 F8                  LEA ECX,DWORD PTR DS:[EAX-0x8]
	$+6B1    005BE381     E8 3A69E8FF              CALL PathOfEx.00444CC0
	$+6B6    005BE386     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+6B8    005BE388     3B45 D4                  CMP EAX,DWORD PTR SS:[EBP-0x2C]
	$+6BB    005BE38B     0F94C3                   SETE BL
	$+6BE    005BE38E     80F3 01                  XOR BL,0x1
	$+6C1    005BE391     885D F3                  MOV BYTE PTR SS:[EBP-0xD],BL
	$+6C4    005BE394     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$+6C8    005BE398     83CF FF                  OR EDI,0xFFFFFFFF
	$+6CB    005BE39B     8B5D E0                  MOV EBX,DWORD PTR SS:[EBP-0x20]
	$+6CE    005BE39E     85DB                     TEST EBX,EBX
	$+6D0    005BE3A0     74 1F                    JE SHORT PathOfEx.005BE3C1
	$+6D2    005BE3A2     8BC7                     MOV EAX,EDI
	$+6D4    005BE3A4     F0:0FC143 04             LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
	$+6D9    005BE3A9     75 16                    JNZ SHORT PathOfEx.005BE3C1
	$+6DB    005BE3AB     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+6DD    005BE3AD     8BCB                     MOV ECX,EBX
	$+6DF    005BE3AF     FF10                     CALL DWORD PTR DS:[EAX]
	$+6E1    005BE3B1     8BC7                     MOV EAX,EDI
	$+6E3    005BE3B3     F0:0FC143 08             LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
	$+6E8    005BE3B8     75 07                    JNZ SHORT PathOfEx.005BE3C1
	$+6EA    005BE3BA     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+6EC    005BE3BC     8BCB                     MOV ECX,EBX
	$+6EE    005BE3BE     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+6F1    005BE3C1     807D F3 00               CMP BYTE PTR SS:[EBP-0xD],0x0
	$+6F5    005BE3C5     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$+6F9    005BE3C9     74 07                    JE SHORT PathOfEx.005BE3D2
	$+6FB    005BE3CB     8BCE                     MOV ECX,ESI
	$+6FD    005BE3CD     E8 9EF7FFFF              CALL PathOfEx.005BDB70
	$+702    005BE3D2     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
	$+709    005BE3D9     8B5D D8                  MOV EBX,DWORD PTR SS:[EBP-0x28]
	$+70C    005BE3DC     85DB                     TEST EBX,EBX
	$+70E    005BE3DE     74 1E                    JE SHORT PathOfEx.005BE3FE
	$+710    005BE3E0     8BC7                     MOV EAX,EDI
	$+712    005BE3E2     F0:0FC143 04             LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
	$+717    005BE3E7     75 15                    JNZ SHORT PathOfEx.005BE3FE
	$+719    005BE3E9     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+71B    005BE3EB     8BCB                     MOV ECX,EBX
	$+71D    005BE3ED     FF10                     CALL DWORD PTR DS:[EAX]
	$+71F    005BE3EF     F0:0FC17B 08             LOCK XADD DWORD PTR DS:[EBX+0x8],EDI
	$+724    005BE3F4     4F                       DEC EDI
	$+725    005BE3F5     75 07                    JNZ SHORT PathOfEx.005BE3FE
	$+727    005BE3F7     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+729    005BE3F9     8BCB                     MOV ECX,EBX
	$+72B    005BE3FB     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+72E    005BE3FE     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+735    005BE405     837D EC 00               CMP DWORD PTR SS:[EBP-0x14],0x0
	$+739    005BE409     5B                       POP EBX
	$+73A    005BE40A     75 36                    JNZ SHORT PathOfEx.005BE442
	$+73C    005BE40C     8B8E 64190000            MOV ECX,DWORD PTR DS:[ESI+0x1964]
	$+742    005BE412     8B86 040C0000            MOV EAX,DWORD PTR DS:[ESI+0xC04]
	$+748    005BE418     3B81 7C0A0000            CMP EAX,DWORD PTR DS:[ECX+0xA7C]
	$+74E    005BE41E     75 22                    JNZ SHORT PathOfEx.005BE442
	$+750    005BE420     FF15 C474DD00            CALL DWORD PTR DS:[<&USER32.ReleaseCapture>]          ; user32.ReleaseCapture
	$+756    005BE426     8B8E 040C0000            MOV ECX,DWORD PTR DS:[ESI+0xC04]
	$+75C    005BE42C     85C9                     TEST ECX,ECX
	$+75E    005BE42E     74 08                    JE SHORT PathOfEx.005BE438
	$+760    005BE430     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+762    005BE432     FF90 04010000            CALL DWORD PTR DS:[EAX+0x104]
	$+768    005BE438     C786 040C0000 00000000   MOV DWORD PTR DS:[ESI+0xC04],0x0
	$+772    005BE442     8BCE                     MOV ECX,ESI
	$+774    005BE444     E8 9749FFFF              CALL PathOfEx.005B2DE0
	$+779    005BE449     807D F2 00               CMP BYTE PTR SS:[EBP-0xE],0x0
	$+77D    005BE44D     74 39                    JE SHORT PathOfEx.005BE488
	$+77F    005BE44F     84C0                     TEST AL,AL
	$+781    005BE451     75 35                    JNZ SHORT PathOfEx.005BE488
	$+783    005BE453     66:0F6E86 340C0000       MOVD MM0,DWORD PTR DS:[ESI+0xC34]
	$+78B    005BE45B     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+78E    005BE45E     8B8E 64190000            MOV ECX,DWORD PTR DS:[ESI+0x1964]
	$+794    005BE464     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+797    005BE467     50                       PUSH EAX
	$+798    005BE468     F3:0F1145 D4             MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+79D    005BE46D     66:0F6E86 380C0000       MOVD MM0,DWORD PTR DS:[ESI+0xC38]
	$+7A5    005BE475     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+7A8    005BE478     F3:0F1145 D8             MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$+7AD    005BE47D     8B89 740A0000            MOV ECX,DWORD PTR DS:[ECX+0xA74]
	$+7B3    005BE483     E8 E8C7F7FF              CALL PathOfEx.0053AC70
	$+7B8    005BE488     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+7BB    005BE48B     5F                       POP EDI
	$+7BC    005BE48C     5E                       POP ESI
	$+7BD    005BE48D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+7C4    005BE494     8BE5                     MOV ESP,EBP
	$+7C6    005BE496     5D                       POP EBP
	$+7C7    005BE497     C3                       RETN
	$+7C8    005BE498     CC                       INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("3b??????????0f??????????83????????????0f??????????83????????????0f??????????8b??e8", 2);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("3b??????????0f??????????83????????????0f??????????83????????????0f??????????8b??e8", 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x1000 && dw_tmp < 0x3000);
	ofs << "pt_cdword	g_o_get_cur_skill_type_info_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//这个函数其实是上面那个函数的一部分
	/*
	$-5A     006D0CC5     84C0                       TEST AL, AL
	$-58     006D0CC7     75 0E                      JNZ SHORT PathOfEx.006D0CD7
	$-56     006D0CC9     FF75 E4                    PUSH DWORD PTR SS:[EBP-0x1C]               ; 鼠标左键这里会到
	$-53     006D0CCC     8BCF                       MOV ECX, EDI
	$-51     006D0CCE     52                         PUSH EDX
	$-50     006D0CCF     E8 CCF7FFFF                CALL PathOfEx.006D04A0
	$-4B     006D0CD4     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$-48     006D0CD7     F3:0F1087 A0180000         MOVSS XMM0, DWORD PTR DS:[EDI+0x18A0]
	$-40     006D0CDF     F3:0F5905 B0D7E600         MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$-38     006D0CE7     F3:0F2CC0                  CVTTSS2SI EAX, XMM0
	$-34     006D0CEB     F3:0F1087 9C180000         MOVSS XMM0, DWORD PTR DS:[EDI+0x189C]
	$-2C     006D0CF3     F3:0F5905 B0D7E600         MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$-24     006D0CFB     8945 E0                    MOV DWORD PTR SS:[EBP-0x20], EAX
	$-21     006D0CFE     8945 D4                    MOV DWORD PTR SS:[EBP-0x2C], EAX
	$-1E     006D0D01     8B45 E4                    MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$-1B     006D0D04     F3:0F2CD8                  CVTTSS2SI EBX, XMM0
	$-17     006D0D08     48                         DEC EAX
	$-16     006D0D09     895D D0                    MOV DWORD PTR SS:[EBP-0x30], EBX
	$-13     006D0D0C     83F8 04                    CMP EAX, 0x4
	$-10     006D0D0F     0F87 C9010000              JA PathOfEx.006D0EDE
	$-A      006D0D15     FF2485 B80F6D00            JMP NEAR DWORD PTR DS:[EAX*4+0x6D0FB8]     ; eax==3,左键点击普通攻击
	$-3      006D0D1C     FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
	$ ==>    006D0D1F     A1 0CD70001                MOV EAX, DWORD PTR DS:[0x100D70C]          ; 走路技能id存放在此全局变量
	$+5      006D0D24     8D4D D0                    LEA ECX, DWORD PTR SS:[EBP-0x30]
	$+8      006D0D27     51                         PUSH ECX
	$+9      006D0D28     8B8F 3C010000              MOV ECX, DWORD PTR DS:[EDI+0x13C]          ; edi=InGameState
	$+F      006D0D2E     25 FF3F0000                AND EAX, 0x3FFF
	$+14     006D0D33     6A 00                      PUSH 0x0
	$+16     006D0D35     50                         PUSH EAX
	$+17     006D0D36     E8 D5482000                CALL PathOfEx.008D5610                     ; 走路_使用技能发包
	$+1C     006D0D3B     E9 9E010000                JMP PathOfEx.006D0EDE
	$+21     006D0D40     83BF BC180000 00           CMP DWORD PTR DS:[EDI+0x18BC], 0x0
	$+28     006D0D47     74 38                      JE SHORT PathOfEx.006D0D81
	$+2A     006D0D49     8D45 C8                    LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+2D     006D0D4C     50                         PUSH EAX
	$+2E     006D0D4D     E8 5EE5FFFF                CALL PathOfEx.006CF2B0
	$+33     006D0D52     84C0                       TEST AL, AL
	$+35     006D0D54     74 2B                      JE SHORT PathOfEx.006D0D81
	$+37     006D0D56     FFB7 BC180000              PUSH DWORD PTR DS:[EDI+0x18BC]
	$+3D     006D0D5C     8BCF                       MOV ECX, EDI
	$+3F     006D0D5E     E8 4DDEFFFF                CALL PathOfEx.006CEBB0
	$+44     006D0D63     84C0                       TEST AL, AL
	$+46     006D0D65     74 1A                      JE SHORT PathOfEx.006D0D81
	*/

	//这个函数其实是上面那个函数的一部分
	//gj3
	/*
	$-36D    005BDCB0     56                       PUSH ESI
	$-36C    005BDCB1     FF15 0474DD00            CALL DWORD PTR DS:[<&USER32.GetKeyState>]             ; user32.GetKeyState
	$-366    005BDCB7     0FB7C0                   MOVZX EAX,AX
	$-363    005BDCBA     85F6                     TEST ESI,ESI
	$-361    005BDCBC     74 05                    JE SHORT PathOfEx.005BDCC3
	$-35F    005BDCBE     66:85C0                  TEST AX,AX
	$-35C    005BDCC1     78 07                    JS SHORT PathOfEx.005BDCCA
	$-35A    005BDCC3     C605 58243201 00         MOV BYTE PTR DS:[0x1322458],0x0
	$-353    005BDCCA     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$-34D    005BDCD0     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$-347    005BDCD6     0F84 4F050000            JE PathOfEx.005BE22B
	$-341    005BDCDC     83BF 780C0000 00         CMP DWORD PTR DS:[EDI+0xC78],0x0
	$-33A    005BDCE3     0F85 42050000            JNZ PathOfEx.005BE22B
	$-334    005BDCE9     83BF 64190000 00         CMP DWORD PTR DS:[EDI+0x1964],0x0
	$-32D    005BDCF0     0F84 35050000            JE PathOfEx.005BE22B
	$-327    005BDCF6     8BCF                     MOV ECX,EDI
	$-325    005BDCF8     E8 E352FDFF              CALL PathOfEx.00592FE0
	$-320    005BDCFD     84C0                     TEST AL,AL
	$-31E    005BDCFF     0F84 26050000            JE PathOfEx.005BE22B
	$-318    005BDD05     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$-312    005BDD0B     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$-30C    005BDD11     75 26                    JNZ SHORT PathOfEx.005BDD39
	$-30A    005BDD13     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
	$-303    005BDD1A     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-300    005BDD1D     C745 DC 00000000         MOV DWORD PTR SS:[EBP-0x24],0x0
	$-2F9    005BDD24     BB 03000000              MOV EBX,0x3
	$-2F4    005BDD29     C745 C8 00000000         MOV DWORD PTR SS:[EBP-0x38],0x0
	$-2ED    005BDD30     C745 CC 00000000         MOV DWORD PTR SS:[EBP-0x34],0x0
	$-2E6    005BDD37     EB 17                    JMP SHORT PathOfEx.005BDD50
	$-2E4    005BDD39     8B87 68150000            MOV EAX,DWORD PTR DS:[EDI+0x1568]                     ; g_o_get_cur_skill_type_info_
	$-2DE    005BDD3F     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-2DB    005BDD42     51                       PUSH ECX
	$-2DA    005BDD43     8D48 F8                  LEA ECX,DWORD PTR DS:[EAX-0x8]
	$-2D7    005BDD46     E8 756FE8FF              CALL PathOfEx.00444CC0
	$-2D2    005BDD4B     BB 04000000              MOV EBX,0x4
	$-2CD    005BDD50     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$-2CB    005BDD52     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-2C8    005BDD55     8975 E0                  MOV DWORD PTR SS:[EBP-0x20],ESI
	$-2C5    005BDD58     894D E4                  MOV DWORD PTR SS:[EBP-0x1C],ECX
	$-2C2    005BDD5B     C700 00000000            MOV DWORD PTR DS:[EAX],0x0
	$-2BC    005BDD61     C740 04 00000000         MOV DWORD PTR DS:[EAX+0x4],0x0
	$-2B5    005BDD68     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-2AE    005BDD6F     F6C3 04                  TEST BL,0x4
	$-2AB    005BDD72     74 0B                    JE SHORT PathOfEx.005BDD7F
	$-2A9    005BDD74     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-2A6    005BDD77     83E3 FB                  AND EBX,0xFFFFFFFB
	$-2A3    005BDD7A     E8 9157D6FF              CALL PathOfEx.00323510
	$-29E    005BDD7F     F6C3 02                  TEST BL,0x2
	$-29B    005BDD82     74 0B                    JE SHORT PathOfEx.005BDD8F
	$-299    005BDD84     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-296    005BDD87     83E3 FD                  AND EBX,0xFFFFFFFD
	$-293    005BDD8A     E8 8157D6FF              CALL PathOfEx.00323510
	$-28E    005BDD8F     F6C3 01                  TEST BL,0x1
	$-28B    005BDD92     74 08                    JE SHORT PathOfEx.005BDD9C
	$-289    005BDD94     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$-286    005BDD97     E8 7457D6FF              CALL PathOfEx.00323510
	$-281    005BDD9C     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$-27B    005BDDA2     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-278    005BDDA5     50                       PUSH EAX
	$-277    005BDDA6     32DB                     XOR BL,BL
	$-275    005BDDA8     C745 DC 0D000000         MOV DWORD PTR SS:[EBP-0x24],0xD
	$-26E    005BDDAF     E8 5C2E1000              CALL PathOfEx.006C0C10
	$-269    005BDDB4     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$-263    005BDDBA     8AF8                     MOV BH,AL
	$-261    005BDDBC     E8 9F1A1000              CALL PathOfEx.006BF860
	$-25C    005BDDC1     84C0                     TEST AL,AL
	$-25A    005BDDC3     75 04                    JNZ SHORT PathOfEx.005BDDC9
	$-258    005BDDC5     84FF                     TEST BH,BH
	$-256    005BDDC7     74 3E                    JE SHORT PathOfEx.005BDE07
	$-254    005BDDC9     85F6                     TEST ESI,ESI
	$-252    005BDDCB     0F94C0                   SETE AL
	$-24F    005BDDCE     34 01                    XOR AL,0x1
	$-24D    005BDDD0     74 35                    JE SHORT PathOfEx.005BDE07
	$-24B    005BDDD2     0FB746 02                MOVZX EAX,WORD PTR DS:[ESI+0x2]
	$-247    005BDDD6     66:3B05 48A50A01         CMP AX,WORD PTR DS:[0x10AA548]
	$-240    005BDDDD     0F94C2                   SETE DL
	$-23D    005BDDE0     66:3B05 58A50A01         CMP AX,WORD PTR DS:[0x10AA558]
	$-236    005BDDE7     0F94C0                   SETE AL
	$-233    005BDDEA     84D2                     TEST DL,DL
	$-231    005BDDEC     74 17                    JE SHORT PathOfEx.005BDE05
	$-22F    005BDDEE     84FF                     TEST BH,BH
	$-22D    005BDDF0     74 15                    JE SHORT PathOfEx.005BDE07
	$-22B    005BDDF2     84C0                     TEST AL,AL
	$-229    005BDDF4     75 0F                    JNZ SHORT PathOfEx.005BDE05
	$-227    005BDDF6     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$-221    005BDDFC     E8 8F1A1000              CALL PathOfEx.006BF890
	$-21C    005BDE01     84C0                     TEST AL,AL
	$-21A    005BDE03     74 02                    JE SHORT PathOfEx.005BDE07
	$-218    005BDE05     B3 01                    MOV BL,0x1
	$-216    005BDE07     6A 11                    PUSH 0x11
	$-214    005BDE09     FF15 0474DD00            CALL DWORD PTR DS:[<&USER32.GetKeyState>]             ; user32.GetKeyState
	$-20E    005BDE0F     0FB7C0                   MOVZX EAX,AX
	$-20B    005BDE12     66:85C0                  TEST AX,AX
	$-208    005BDE15     79 03                    JNS SHORT PathOfEx.005BDE1A
	$-206    005BDE17     80CB 04                  OR BL,0x4
	$-203    005BDE1A     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$-1FD    005BDE20     8B80 2C0A0000            MOV EAX,DWORD PTR DS:[EAX+0xA2C]
	$-1F7    005BDE26     85C0                     TEST EAX,EAX
	$-1F5    005BDE28     74 0D                    JE SHORT PathOfEx.005BDE37
	$-1F3    005BDE2A     8A80 54070000            MOV AL,BYTE PTR DS:[EAX+0x754]
	$-1ED    005BDE30     84C0                     TEST AL,AL
	$-1EB    005BDE32     74 03                    JE SHORT PathOfEx.005BDE37
	$-1E9    005BDE34     80CB 02                  OR BL,0x2
	$-1E6    005BDE37     8B8F 34010000            MOV ECX,DWORD PTR DS:[EDI+0x134]
	$-1E0    005BDE3D     E8 EE191000              CALL PathOfEx.006BF830
	$-1DB    005BDE42     8ACB                     MOV CL,BL
	$-1D9    005BDE44     80C9 08                  OR CL,0x8
	$-1D6    005BDE47     84C0                     TEST AL,AL
	$-1D4    005BDE49     8B87 64150000            MOV EAX,DWORD PTR DS:[EDI+0x1564]
	$-1CE    005BDE4F     0FB6D1                   MOVZX EDX,CL
	$-1CB    005BDE52     0FB6CB                   MOVZX ECX,BL
	$-1C8    005BDE55     0F44D1                   CMOVE EDX,ECX
	$-1C5    005BDE58     8ADA                     MOV BL,DL
	$-1C3    005BDE5A     3B87 68150000            CMP EAX,DWORD PTR DS:[EDI+0x1568]
	$-1BD    005BDE60     75 07                    JNZ SHORT PathOfEx.005BDE69
	$-1BB    005BDE62     B9 29000000              MOV ECX,0x29
	$-1B6    005BDE67     EB 09                    JMP SHORT PathOfEx.005BDE72
	$-1B4    005BDE69     8B87 68150000            MOV EAX,DWORD PTR DS:[EDI+0x1568]
	$-1AE    005BDE6F     8B48 F4                  MOV ECX,DWORD PTR DS:[EAX-0xC]
	$-1AB    005BDE72     C745 D0 00000000         MOV DWORD PTR SS:[EBP-0x30],0x0
	$-1A4    005BDE79     C745 D4 00000000         MOV DWORD PTR SS:[EBP-0x2C],0x0
	$-19D    005BDE80     51                       PUSH ECX
	$-19C    005BDE81     8AC3                     MOV AL,BL
	$-19A    005BDE83     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$-196    005BDE87     24 01                    AND AL,0x1
	$-194    005BDE89     0FB6C0                   MOVZX EAX,AL
	$-191    005BDE8C     50                       PUSH EAX
	$-190    005BDE8D     51                       PUSH ECX
	$-18F    005BDE8E     83EC 08                  SUB ESP,0x8
	$-18C    005BDE91     8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-189    005BDE94     8BCC                     MOV ECX,ESP
	$-187    005BDE96     50                       PUSH EAX
	$-186    005BDE97     E8 049ED7FF              CALL PathOfEx.00337CA0
	$-181    005BDE9C     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$-17E    005BDE9F     8BCF                     MOV ECX,EDI
	$-17C    005BDEA1     50                       PUSH EAX
	$-17B    005BDEA2     E8 E9100000              CALL PathOfEx.005BEF90
	$-176    005BDEA7     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$-172    005BDEAB     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-16F    005BDEAE     8945 BC                  MOV DWORD PTR SS:[EBP-0x44],EAX
	$-16C    005BDEB1     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-169    005BDEB4     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-166    005BDEB7     8945 C0                  MOV DWORD PTR SS:[EBP-0x40],EAX
	$-163    005BDEBA     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-160    005BDEBD     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
	$-15D    005BDEC0     8B45 B8                  MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-15A    005BDEC3     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-157    005BDEC6     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$-154    005BDEC9     50                       PUSH EAX
	$-153    005BDECA     E8 E1510000              CALL PathOfEx.005C30B0
	$-14E    005BDECF     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-14B    005BDED2     85C0                     TEST EAX,EAX
	$-149    005BDED4     75 08                    JNZ SHORT PathOfEx.005BDEDE
	$-147    005BDED6     8D70 01                  LEA ESI,DWORD PTR DS:[EAX+0x1]
	$-144    005BDED9     E9 22030000              JMP PathOfEx.005BE200
	$-13F    005BDEDE     3B87 7C150000            CMP EAX,DWORD PTR DS:[EDI+0x157C]
	$-139    005BDEE4     8D8F 74150000            LEA ECX,DWORD PTR DS:[EDI+0x1574]
	$-133    005BDEEA     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
	$-130    005BDEED     50                       PUSH EAX
	$-12F    005BDEEE     0F9445 F2                SETE BYTE PTR SS:[EBP-0xE]
	$-12B    005BDEF2     E8 C96DE8FF              CALL PathOfEx.00444CC0
	$-126    005BDEF7     8B4D D0                  MOV ECX,DWORD PTR SS:[EBP-0x30]
	$-123    005BDEFA     3B08                     CMP ECX,DWORD PTR DS:[EAX]
	$-121    005BDEFC     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
	$-11E    005BDEFF     0F9445 F3                SETE BYTE PTR SS:[EBP-0xD]
	$-11A    005BDF03     E8 0856D6FF              CALL PathOfEx.00323510
	$-115    005BDF08     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-112    005BDF0B     3B8F 70150000            CMP ECX,DWORD PTR DS:[EDI+0x1570]
	$-10C    005BDF11     8A55 F3                  MOV DL,BYTE PTR SS:[EBP-0xD]
	$-109    005BDF14     0F94C0                   SETE AL
	$-106    005BDF17     807D F2 00               CMP BYTE PTR SS:[EBP-0xE],0x0
	$-102    005BDF1B     8845 F1                  MOV BYTE PTR SS:[EBP-0xF],AL
	$-FF     005BDF1E     74 13                    JE SHORT PathOfEx.005BDF33
	$-FD     005BDF20     84D2                     TEST DL,DL
	$-FB     005BDF22     74 73                    JE SHORT PathOfEx.005BDF97
	$-F9     005BDF24     84C0                     TEST AL,AL
	$-F7     005BDF26     74 0B                    JE SHORT PathOfEx.005BDF33
	$-F5     005BDF28     0FB6C3                   MOVZX EAX,BL
	$-F2     005BDF2B     3B87 88150000            CMP EAX,DWORD PTR DS:[EDI+0x1588]
	$-EC     005BDF31     74 57                    JE SHORT PathOfEx.005BDF8A
	$-EA     005BDF33     84D2                     TEST DL,DL
	$-E8     005BDF35     74 60                    JE SHORT PathOfEx.005BDF97
	$-E6     005BDF37     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$-E3     005BDF3A     85D2                     TEST EDX,EDX
	$-E1     005BDF3C     74 59                    JE SHORT PathOfEx.005BDF97
	$-DF     005BDF3E     6A 3A                    PUSH 0x3A
	$-DD     005BDF40     8BCA                     MOV ECX,EDX
	$-DB     005BDF42     E8 096E1500              CALL PathOfEx.00714D50
	$-D6     005BDF47     84C0                     TEST AL,AL
	$-D4     005BDF49     74 49                    JE SHORT PathOfEx.005BDF94
	$-D2     005BDF4B     807D F2 00               CMP BYTE PTR SS:[EBP-0xE],0x0
	$-CE     005BDF4F     74 16                    JE SHORT PathOfEx.005BDF67
	$-CC     005BDF51     807D F1 00               CMP BYTE PTR SS:[EBP-0xF],0x0
	$-C8     005BDF55     75 33                    JNZ SHORT PathOfEx.005BDF8A
	$-C6     005BDF57     837D E8 03               CMP DWORD PTR SS:[EBP-0x18],0x3
	$-C2     005BDF5B     75 37                    JNZ SHORT PathOfEx.005BDF94
	$-C0     005BDF5D     BE 02000000              MOV ESI,0x2
	$-BB     005BDF62     E9 99020000              JMP PathOfEx.005BE200
	$-B6     005BDF67     837D E8 03               CMP DWORD PTR SS:[EBP-0x18],0x3
	$-B2     005BDF6B     75 27                    JNZ SHORT PathOfEx.005BDF94
	$-B0     005BDF6D     83BF 7C150000 04         CMP DWORD PTR DS:[EDI+0x157C],0x4
	$-A9     005BDF74     75 1E                    JNZ SHORT PathOfEx.005BDF94
	$-A7     005BDF76     C787 70150000 00000000   MOV DWORD PTR DS:[EDI+0x1570],0x0
	$-9D     005BDF80     C787 7C150000 03000000   MOV DWORD PTR DS:[EDI+0x157C],0x3
	$-93     005BDF8A     BE 02000000              MOV ESI,0x2
	$-8E     005BDF8F     E9 6C020000              JMP PathOfEx.005BE200
	$-89     005BDF94     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-86     005BDF97     398F 48190000            CMP DWORD PTR DS:[EDI+0x1948],ECX
	$-80     005BDF9D     74 0D                    JE SHORT PathOfEx.005BDFAC
	$-7E     005BDF9F     6A 00                    PUSH 0x0
	$-7C     005BDFA1     51                       PUSH ECX
	$-7B     005BDFA2     8BCF                     MOV ECX,EDI
	$-79     005BDFA4     E8 37E0FFFF              CALL PathOfEx.005BBFE0
	$-74     005BDFA9     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-71     005BDFAC     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$-6E     005BDFAF     85D2                     TEST EDX,EDX
	$-6C     005BDFB1     74 1D                    JE SHORT PathOfEx.005BDFD0
	$-6A     005BDFB3     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$-64     005BDFB9     80B8 EB410000 00         CMP BYTE PTR DS:[EAX+0x41EB],0x0
	$-5D     005BDFC0     75 0E                    JNZ SHORT PathOfEx.005BDFD0
	$-5B     005BDFC2     FF75 E8                  PUSH DWORD PTR SS:[EBP-0x18]
	$-58     005BDFC5     8BCF                     MOV ECX,EDI
	$-56     005BDFC7     52                       PUSH EDX
	$-55     005BDFC8     E8 33F9FFFF              CALL PathOfEx.005BD900
	$-50     005BDFCD     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-4D     005BDFD0     F3:0F1087 28190000       MOVSS XMM0,DWORD PTR DS:[EDI+0x1928]
	$-45     005BDFD8     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$-3D     005BDFE0     F3:0F2CF0                CVTTSS2SI ESI,XMM0
	$-39     005BDFE4     F3:0F1087 2C190000       MOVSS XMM0,DWORD PTR DS:[EDI+0x192C]
	$-31     005BDFEC     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$-29     005BDFF4     8975 D8                  MOV DWORD PTR SS:[EBP-0x28],ESI
	$-26     005BDFF7     F3:0F2CC0                CVTTSS2SI EAX,XMM0
	$-22     005BDFFB     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$-1F     005BDFFE     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-1C     005BE001     48                       DEC EAX
	$-1B     005BE002     83F8 04                  CMP EAX,0x4
	$-18     005BE005     0F87 B2010000            JA PathOfEx.005BE1BD
	$-12     005BE00B     FF2485 40E25B00          JMP DWORD PTR DS:[EAX*4+0x5BE240]                     ; eax==3,左键点击普通攻击
	$-B      005BE012     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]                      ; edi=InGameState
	$-5      005BE018     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-2      005BE01B     53                       PUSH EBX
	$-1      005BE01C     50                       PUSH EAX
	$ ==>    005BE01D     A1 48A50A01              MOV EAX,DWORD PTR DS:[0x10AA548]                      ; 走路技能id存放在此全局变量
	$+5      005BE022     6A 00                    PUSH 0x0
	$+7      005BE024     25 FF3F0000              AND EAX,0x3FFF
	$+C      005BE029     50                       PUSH EAX
	$+D      005BE02A     E8 110A2200              CALL PathOfEx.007DEA40                                ; 走路_使用技能发包
	$+12     005BE02F     E9 89010000              JMP PathOfEx.005BE1BD
	$+17     005BE034     83BF 48190000 00         CMP DWORD PTR DS:[EDI+0x1948],0x0
	$+1E     005BE03B     74 33                    JE SHORT PathOfEx.005BE070
	$+20     005BE03D     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+23     005BE040     50                       PUSH EAX
	$+24     005BE041     E8 7A4EFFFF              CALL PathOfEx.005B2EC0
	$+29     005BE046     84C0                     TEST AL,AL
	$+2B     005BE048     74 26                    JE SHORT PathOfEx.005BE070
	$+2D     005BE04A     FFB7 48190000            PUSH DWORD PTR DS:[EDI+0x1948]
	$+33     005BE050     8BCF                     MOV ECX,EDI
	$+35     005BE052     E8 39E1FFFF              CALL PathOfEx.005BC190
	$+3A     005BE057     84C0                     TEST AL,AL
	$+3C     005BE059     74 15                    JE SHORT PathOfEx.005BE070
	$+3E     005BE05B     8B87 48190000            MOV EAX,DWORD PTR DS:[EDI+0x1948]
	$+44     005BE061     8B40 2C                  MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+47     005BE064     8B70 10                  MOV ESI,DWORD PTR DS:[EAX+0x10]
	$+4A     005BE067     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+4D     005BE06A     8975 D8                  MOV DWORD PTR SS:[EBP-0x28],ESI
	$+50     005BE06D     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+53     005BE070     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+59     005BE076     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+5C     005BE079     53                       PUSH EBX
	$+5D     005BE07A     50                       PUSH EAX
	$+5E     005BE07B     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+61     005BE07E     6A 00                    PUSH 0x0
	$+63     005BE080     0FB700                   MOVZX EAX,WORD PTR DS:[EAX]
	$+66     005BE083     50                       PUSH EAX
	$+67     005BE084     E8 B7092200              CALL PathOfEx.007DEA40
	$+6C     005BE089     E9 2F010000              JMP PathOfEx.005BE1BD
	$+71     005BE08E     85C9                     TEST ECX,ECX
	$+73     005BE090     0F84 27010000            JE PathOfEx.005BE1BD
	$+79     005BE096     8B97 3C010000            MOV EDX,DWORD PTR DS:[EDI+0x13C]
	$+7F     005BE09C     80BA 38450000 00         CMP BYTE PTR DS:[EDX+0x4538],0x0
	$+86     005BE0A3     74 3C                    JE SHORT PathOfEx.005BE0E1
	$+88     005BE0A5     80BF A61A0000 00         CMP BYTE PTR DS:[EDI+0x1AA6],0x0
	$+8F     005BE0AC     74 33                    JE SHORT PathOfEx.005BE0E1
	$+91     005BE0AE     8B87 A81A0000            MOV EAX,DWORD PTR DS:[EDI+0x1AA8]
	$+97     005BE0B4     85C0                     TEST EAX,EAX
	$+99     005BE0B6     0F84 01010000            JE PathOfEx.005BE1BD
	$+9F     005BE0BC     51                       PUSH ECX
	$+A0     005BE0BD     8BC8                     MOV ECX,EAX
	$+A2     005BE0BF     E8 1C0ADDFF              CALL PathOfEx.0038EAE0
	$+A7     005BE0C4     84C0                     TEST AL,AL
	$+A9     005BE0C6     0F84 F1000000            JE PathOfEx.005BE1BD
	$+AF     005BE0CC     8B8F A81A0000            MOV ECX,DWORD PTR DS:[EDI+0x1AA8]
	$+B5     005BE0D2     6A 01                    PUSH 0x1
	$+B7     005BE0D4     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+BA     005BE0D7     E8 940ADDFF              CALL PathOfEx.0038EB70
	$+BF     005BE0DC     E9 DC000000              JMP PathOfEx.005BE1BD
	$+C4     005BE0E1     A1 54A50A01              MOV EAX,DWORD PTR DS:[0x10AA554]
	$+C9     005BE0E6     53                       PUSH EBX
	$+CA     005BE0E7     FF71 20                  PUSH DWORD PTR DS:[ECX+0x20]
	$+CD     005BE0EA     25 FF3F0000              AND EAX,0x3FFF
	$+D2     005BE0EF     8BCA                     MOV ECX,EDX
	$+D4     005BE0F1     50                       PUSH EAX
	$+D5     005BE0F2     E8 19092200              CALL PathOfEx.007DEA10
	$+DA     005BE0F7     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+DD     005BE0FA     898F 5C190000            MOV DWORD PTR DS:[EDI+0x195C],ECX
	$+E3     005BE100     E8 6BEFF7FF              CALL PathOfEx.0053D070
	$+E8     005BE105     84C0                     TEST AL,AL
	$+EA     005BE107     0F84 B0000000            JE PathOfEx.005BE1BD
	$+F0     005BE10D     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+F6     005BE113     E8 980C2200              CALL PathOfEx.007DEDB0
	$+FB     005BE118     E9 A0000000              JMP PathOfEx.005BE1BD
	$+100    005BE11D     85C9                     TEST ECX,ECX
	$+102    005BE11F     0F84 98000000            JE PathOfEx.005BE1BD
	$+108    005BE125     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+10E    005BE12B     80B8 EB410000 00         CMP BYTE PTR DS:[EAX+0x41EB],0x0
	$+115    005BE132     74 48                    JE SHORT PathOfEx.005BE17C
	$+117    005BE134     E8 C7C0DAFF              CALL PathOfEx.0036A200
	$+11C    005BE139     84C0                     TEST AL,AL
	$+11E    005BE13B     74 14                    JE SHORT PathOfEx.005BE151
	$+120    005BE13D     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+123    005BE140     8BCF                     MOV ECX,EDI
	$+125    005BE142     E8 09170000              CALL PathOfEx.005BF850
	$+12A    005BE147     BE 03000000              MOV ESI,0x3
	$+12F    005BE14C     E9 AF000000              JMP PathOfEx.005BE200
	$+134    005BE151     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+137    005BE154     66:8B40 02               MOV AX,WORD PTR DS:[EAX+0x2]
	$+13B    005BE158     66:3B05 54A50A01         CMP AX,WORD PTR DS:[0x10AA554]
	$+142    005BE15F   ^ 75 E6                    JNZ SHORT PathOfEx.005BE147
	$+144    005BE161     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+147    005BE164     E8 D73B0000              CALL PathOfEx.005C1D40
	$+14C    005BE169     84C0                     TEST AL,AL
	$+14E    005BE16B     75 0C                    JNZ SHORT PathOfEx.005BE179
	$+150    005BE16D     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+153    005BE170     E8 4BEFF7FF              CALL PathOfEx.0053D0C0
	$+158    005BE175     84C0                     TEST AL,AL
	$+15A    005BE177   ^ 74 CE                    JE SHORT PathOfEx.005BE147
	$+15C    005BE179     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+15F    005BE17C     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+162    005BE17F     53                       PUSH EBX
	$+163    005BE180     FF71 20                  PUSH DWORD PTR DS:[ECX+0x20]
	$+166    005BE183     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+16C    005BE189     0FB700                   MOVZX EAX,WORD PTR DS:[EAX]
	$+16F    005BE18C     EB 29                    JMP SHORT PathOfEx.005BE1B7
	$+171    005BE18E     8B55 D0                  MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+174    005BE191     85D2                     TEST EDX,EDX
	$+176    005BE193     74 28                    JE SHORT PathOfEx.005BE1BD
	$+178    005BE195     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+17E    005BE19B     80B9 EB410000 00         CMP BYTE PTR DS:[ECX+0x41EB],0x0
	$+185    005BE1A2     75 19                    JNZ SHORT PathOfEx.005BE1BD
	$+187    005BE1A4     8B87 38010000            MOV EAX,DWORD PTR DS:[EDI+0x138]
	$+18D    005BE1AA     53                       PUSH EBX
	$+18E    005BE1AB     8B80 58010000            MOV EAX,DWORD PTR DS:[EAX+0x158]
	$+194    005BE1B1     FF70 20                  PUSH DWORD PTR DS:[EAX+0x20]
	$+197    005BE1B4     0FB702                   MOVZX EAX,WORD PTR DS:[EDX]
	$+19A    005BE1B7     50                       PUSH EAX
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("a1????????6a??25????????50e8????????e9????????83", 1);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("a1????????6a??25????????50e8????????e9????????83", 1);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_skill_id_addr_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-2F     006292DF     CC                   INT3
	$-2E     006292E0     55                   PUSH EBP
	$-2D     006292E1     8BEC                 MOV EBP, ESP
	$-2B     006292E3     6A FF                PUSH -0x1
	$-29     006292E5     68 6A81D300          PUSH PathOfEx.00D3816A
	$-24     006292EA     64:A1 00000000       MOV EAX, DWORD PTR FS:[0]
	$-1E     006292F0     50                   PUSH EAX
	$-1D     006292F1     64:8925 00000000     MOV DWORD PTR FS:[0], ESP
	$-16     006292F8     83EC 0C              SUB ESP, 0xC
	$-13     006292FB     56                   PUSH ESI
	$-12     006292FC     33F6                 XOR ESI, ESI
	$-10     006292FE     C745 E8 00000000     MOV DWORD PTR SS:[EBP-0x18], 0x0
	$-9      00629305     57                   PUSH EDI
	$-8      00629306     8BF9                 MOV EDI, ECX
	$-6      00629308     8975 EC              MOV DWORD PTR SS:[EBP-0x14], ESI
	$-3      0062930B     8975 FC              MOV DWORD PTR SS:[EBP-0x4], ESI
	$ ==>    0062930E     8D8F BC080000        LEA ECX, DWORD PTR DS:[EDI+0x8BC]        ; g_o_get_my_skills_
	$+6      00629314     8B45 08              MOV EAX, DWORD PTR SS:[EBP+0x8]
	$+9      00629317     8945 08              MOV DWORD PTR SS:[EBP+0x8], EAX
	$+C      0062931A     8D45 08              LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+F      0062931D     50                   PUSH EAX
	$+10     0062931E     8D45 F0              LEA EAX, DWORD PTR SS:[EBP-0x10]
	$+13     00629321     50                   PUSH EAX                                 ; eax==stCD_SkillNodeInfo*&
	$+14     00629322     E8 D92CE4FF          CALL PathOfEx.0046C000                   ; find新的技能信息，hash查找函数
	$+19     00629327     8B45 F0              MOV EAX, DWORD PTR SS:[EBP-0x10]
	$+1C     0062932A     3B87 C0080000        CMP EAX, DWORD PTR DS:[EDI+0x8C0]        ; eax=stCD_SkillNodeInfo*
	$+22     00629330     74 18                JE SHORT PathOfEx.0062934A
	$+24     00629332     8B70 10              MOV ESI, DWORD PTR DS:[EAX+0x10]
	$+27     00629335     8B40 0C              MOV EAX, DWORD PTR DS:[EAX+0xC]
	$+2A     00629338     85F6                 TEST ESI, ESI
	$+2C     0062933A     74 04                JE SHORT PathOfEx.00629340
	$+2E     0062933C     F0:FF46 04           LOCK INC DWORD PTR DS:[ESI+0x4]
	$+32     00629340     8975 EC              MOV DWORD PTR SS:[EBP-0x14], ESI         ; ebp-18=stCD_Skill_IdInfo*
	$+35     00629343     8945 E8              MOV DWORD PTR SS:[EBP-0x18], EAX
	$+38     00629346     C645 FC 00           MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+3C     0062934A     83BF 04090000 00     CMP DWORD PTR DS:[EDI+0x904], 0x0
	$+43     00629351     74 19                JE SHORT PathOfEx.0062936C
	$+45     00629353     83EC 08              SUB ESP, 0x8
	$+48     00629356     8D45 E8              LEA EAX, DWORD PTR SS:[EBP-0x18]
	$+4B     00629359     8BCC                 MOV ECX, ESP
	$+4D     0062935B     50                   PUSH EAX
	$+4E     0062935C     E8 4FB4E5FF          CALL PathOfEx.004847B0                   ; 拷贝stCD_Skill_IdInfo
	$+53     00629361     8D8F E0080000        LEA ECX, DWORD PTR DS:[EDI+0x8E0]
	$+59     00629367     E8 E4030000          CALL PathOfEx.00629750                   ; 设置新的技能信息，参数是stCD_Skill_IdInfo对象
	$+5E     0062936C     6A 00                PUSH 0x0
	$+60     0062936E     8BCF                 MOV ECX, EDI
	$+62     00629370     E8 0B213C00          CALL PathOfEx.009EB480
	$+67     00629375     C745 FC 03000000     MOV DWORD PTR SS:[EBP-0x4], 0x3
	$+6E     0062937C     85F6                 TEST ESI, ESI
	$+70     0062937E     74 21                JE SHORT PathOfEx.006293A1
	$+72     00629380     83CF FF              OR EDI, -0x1
	$+75     00629383     8BC7                 MOV EAX, EDI
	$+77     00629385     F0:0FC146 04         LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+7C     0062938A     75 15                JNZ SHORT PathOfEx.006293A1
	$+7E     0062938C     8B06                 MOV EAX, DWORD PTR DS:[ESI]
	$+80     0062938E     8BCE                 MOV ECX, ESI
	$+82     00629390     FF10                 CALL NEAR DWORD PTR DS:[EAX]
	$+84     00629392     F0:0FC17E 08         LOCK XADD DWORD PTR DS:[ESI+0x8], EDI
	$+89     00629397     4F                   DEC EDI
	$+8A     00629398     75 07                JNZ SHORT PathOfEx.006293A1
	$+8C     0062939A     8B06                 MOV EAX, DWORD PTR DS:[ESI]
	$+8E     0062939C     8BCE                 MOV ECX, ESI
	$+90     0062939E     FF50 04              CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+93     006293A1     8B4D F4              MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+96     006293A4     5F                   POP EDI                                  ; 0018EA54
	$+97     006293A5     64:890D 00000000     MOV DWORD PTR FS:[0], ECX
	$+9E     006293AC     5E                   POP ESI                                  ; 0018EA54
	$+9F     006293AD     8BE5                 MOV ESP, EBP
	$+A1     006293AF     5D                   POP EBP                                  ; 0018EA54
	$+A2     006293B0     C2 0800              RETN 0x8
	$+A5     006293B3     CC                   INT3

	//调用了上面的函数
	$-41     004912CF     CC                   INT3
	$-40     004912D0     83EC 0C              SUB ESP, 0xC
	$-3D     004912D3     53                   PUSH EBX
	$-3C     004912D4     55                   PUSH EBP
	$-3B     004912D5     56                   PUSH ESI
	$-3A     004912D6     57                   PUSH EDI
	$-39     004912D7     8BF9                 MOV EDI, ECX
	$-37     004912D9     8D4C24 10            LEA ECX, DWORD PTR SS:[ESP+0x10]
	$-33     004912DD     8B77 04              MOV ESI, DWORD PTR DS:[EDI+0x4]
	$-30     004912E0     8B07                 MOV EAX, DWORD PTR DS:[EDI]
	$-2E     004912E2     897424 10            MOV DWORD PTR SS:[ESP+0x10], ESI
	$-2A     004912E6     894424 14            MOV DWORD PTR SS:[ESP+0x14], EAX         ; PathOfEx.006292E0
	$-26     004912EA     890F                 MOV DWORD PTR DS:[EDI], ECX
	$-24     004912EC     833E 00              CMP DWORD PTR DS:[ESI], 0x0
	$-21     004912EF     74 33                JE SHORT PathOfEx.00491324
	$-1F     004912F1     8B5C24 24            MOV EBX, DWORD PTR SS:[ESP+0x24]
	$-1B     004912F5     8B6C24 20            MOV EBP, DWORD PTR SS:[ESP+0x20]
	$-17     004912F9     0F1F80 00000000      NOP DWORD PTR DS:[EAX]
	$-10     00491300     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
	$-E      00491302     8BCE                 MOV ECX, ESI
	$-C      00491304     FF10                 CALL NEAR DWORD PTR DS:[EAX]
	$-A      00491306     014424 10            ADD DWORD PTR SS:[ESP+0x10], EAX         ; PathOfEx.006292E0
	$-6      0049130A     8BCE                 MOV ECX, ESI
	$-4      0049130C     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
	$-2      0049130E     53                   PUSH EBX
	$-1      0049130F     55                   PUSH EBP
	$ ==>    00491310     FF50 08              CALL NEAR DWORD PTR DS:[EAX+0x8]         ; 调用了上面的函数
	$+3      00491313     8B7424 10            MOV ESI, DWORD PTR SS:[ESP+0x10]
	$+7      00491317     85F6                 TEST ESI, ESI
	$+9      00491319     74 0B                JE SHORT PathOfEx.00491326
	$+B      0049131B     833E 00              CMP DWORD PTR DS:[ESI], 0x0
	$+E      0049131E   ^ 75 E0                JNZ SHORT PathOfEx.00491300
	$+10     00491320     8B4424 14            MOV EAX, DWORD PTR SS:[ESP+0x14]
	$+14     00491324     8907                 MOV DWORD PTR DS:[EDI], EAX              ; PathOfEx.006292E0
	$+16     00491326     5F                   POP EDI                                  ; 0018D6B8
	$+17     00491327     5E                   POP ESI                                  ; 0018D6B8
	$+18     00491328     5D                   POP EBP                                  ; 0018D6B8
	$+19     00491329     5B                   POP EBX                                  ; 0018D6B8
	$+1A     0049132A     83C4 0C              ADD ESP, 0xC
	$+1D     0049132D     C2 0800              RETN 0x8
	$+20     00491330     55                   PUSH EBP
	$+21     00491331     8BEC                 MOV EBP, ESP
	$+23     00491333     6A FF                PUSH -0x1
	$+25     00491335     68 8013D200          PUSH PathOfEx.00D21380
	$+2A     0049133A     64:A1 00000000       MOV EAX, DWORD PTR FS:[0]
	$+30     00491340     50                   PUSH EAX                                 ; PathOfEx.006292E0
	$+31     00491341     64:8925 00000000     MOV DWORD PTR FS:[0], ESP
	$+38     00491348     83EC 0C              SUB ESP, 0xC
	$+3B     0049134B     53                   PUSH EBX
	$+3C     0049134C     56                   PUSH ESI
	$+3D     0049134D     8BF1                 MOV ESI, ECX
	$+3F     0049134F     B8 ABAAAA2A          MOV EAX, 0x2AAAAAAB
	$+44     00491354     57                   PUSH EDI
	$+45     00491355     8B7D 08              MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+48     00491358     8965 F0              MOV DWORD PTR SS:[EBP-0x10], ESP
	$+4B     0049135B     C706 00000000        MOV DWORD PTR DS:[ESI], 0x0
	$+51     00491361     C746 04 00000000     MOV DWORD PTR DS:[ESI+0x4], 0x0
	$+58     00491368     C746 08 00000000     MOV DWORD PTR DS:[ESI+0x8], 0x0
	$+5F     0049136F     8B57 04              MOV EDX, DWORD PTR DS:[EDI+0x4]
	$+62     00491372     2B17                 SUB EDX, DWORD PTR DS:[EDI]
	$+64     00491374     F7EA                 IMUL EDX
	$+66     00491376     8975 EC              MOV DWORD PTR SS:[EBP-0x14], ESI
	$+69     00491379     C1FA 02              SAR EDX, 0x2
	$+6C     0049137C     8BC2                 MOV EAX, EDX
	$+6E     0049137E     C1E8 1F              SHR EAX, 0x1F
	$+71     00491381     03C2                 ADD EAX, EDX
	$+73     00491383     50                   PUSH EAX                                 ; PathOfEx.006292E0
	$+74     00491384     E8 A7FBFDFF          CALL PathOfEx.00470F30
	$+79     00491389     84C0                 TEST AL, AL
	$+7B     0049138B     74 22                JE SHORT PathOfEx.004913AF
	$+7D     0049138D     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+84     00491394     8B57 04              MOV EDX, DWORD PTR DS:[EDI+0x4]
	$+87     00491397     51                   PUSH ECX
	$+88     00491398     C645 08 00           MOV BYTE PTR SS:[EBP+0x8], 0x0
	$+8C     0049139C     FF75 08              PUSH DWORD PTR SS:[EBP+0x8]
	$+8F     0049139F     51                   PUSH ECX
	$+90     004913A0     FF36                 PUSH DWORD PTR DS:[ESI]                  ; PathOfEx.00E72B2C
	$+92     004913A2     8B0F                 MOV ECX, DWORD PTR DS:[EDI]
	$+94     004913A4     E8 0745FEFF          CALL PathOfEx.004758B0
	$+99     004913A9     83C4 10              ADD ESP, 0x10
	$+9C     004913AC     8946 04              MOV DWORD PTR DS:[ESI+0x4], EAX          ; PathOfEx.006292E0
	$+9F     004913AF     8B4D F4              MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+A2     004913B2     8BC6                 MOV EAX, ESI
	$+A4     004913B4     5F                   POP EDI                                  ; 0018D6B8
	$+A5     004913B5     5E                   POP ESI                                  ; 0018D6B8
	$+A6     004913B6     64:890D 00000000     MOV DWORD PTR FS:[0], ECX
	$+AD     004913BD     5B                   POP EBX                                  ; 0018D6B8
	$+AE     004913BE     8BE5                 MOV ESP, EBP
	$+B0     004913C0     5D                   POP EBP                                  ; 0018D6B8
	$+B1     004913C1     C2 0400              RETN 0x4
	$+B4     004913C4     8B4D EC              MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+B7     004913C7     E8 0487FEFF          CALL PathOfEx.00479AD0
	$+BC     004913CC     6A 00                PUSH 0x0
	$+BE     004913CE     6A 00                PUSH 0x0
	$+C0     004913D0     E8 DE228000          CALL PathOfEx.00C936B3
	$+C5     004913D5     CC                   INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8d??????????8b????89????8d????508d????50e8????????8b????3b", 2);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x500 && dw_tmp < 0x1500);
	ofs << "pt_cdword	g_o_get_my_skills_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-90     008C423F     CC                        INT3
	$-8F     008C4240     55                        PUSH EBP
	$-8E     008C4241     8BEC                      MOV EBP, ESP
	$-8C     008C4243     6A FF                     PUSH -0x1
	$-8A     008C4245     68 CA1CD600               PUSH PathOfEx.00D61CCA
	$-85     008C424A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
	$-7F     008C4250     50                        PUSH EAX
	$-7E     008C4251     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
	$-77     008C4258     83EC 70                   SUB ESP, 0x70
	$-74     008C425B     53                        PUSH EBX
	$-73     008C425C     56                        PUSH ESI
	$-72     008C425D     57                        PUSH EDI
	$-71     008C425E     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
	$-6E     008C4261     8BF1                      MOV ESI, ECX
	$-6C     008C4263     6A 02                     PUSH 0x2
	$-6A     008C4265     8BCF                      MOV ECX, EDI
	$-68     008C4267     8975 EC                   MOV DWORD PTR SS:[EBP-0x14], ESI
	$-65     008C426A     E8 61BFBDFF               CALL PathOfEx.004A01D0
	$-60     008C426F     8B57 08                   MOV EDX, DWORD PTR DS:[EDI+0x8]
	$-5D     008C4272     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$-5B     008C4274     66:8B1C10                 MOV BX, WORD PTR DS:[EAX+EDX]
	$-57     008C4278     8D42 02                   LEA EAX, DWORD PTR DS:[EDX+0x2]
	$-54     008C427B     66:895D F2                MOV WORD PTR SS:[EBP-0xE], BX
	$-50     008C427F     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$-4D     008C4282     F6C7 02                   TEST BH, 0x2
	$-4A     008C4285     74 12                     JE SHORT PathOfEx.008C4299
	$-48     008C4287     8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$-46     008C4289     8AC7                      MOV AL, BH
	$-44     008C428B     C0E8 02                   SHR AL, 0x2
	$-41     008C428E     8BCE                      MOV ECX, ESI
	$-3F     008C4290     24 01                     AND AL, 0x1
	$-3D     008C4292     0FB6C0                    MOVZX EAX, AL
	$-3A     008C4295     50                        PUSH EAX
	$-39     008C4296     FF52 74                   CALL NEAR DWORD PTR DS:[EDX+0x74]
	$-36     008C4299     F6C3 40                   TEST BL, 0x40
	$-33     008C429C     74 2B                     JE SHORT PathOfEx.008C42C9
	$-31     008C429E     6A 01                     PUSH 0x1
	$-2F     008C42A0     8BCF                      MOV ECX, EDI
	$-2D     008C42A2     E8 29BFBDFF               CALL PathOfEx.004A01D0
	$-28     008C42A7     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$-25     008C42AA     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$-23     008C42AC     8A0401                    MOV AL, BYTE PTR DS:[ECX+EAX]
	$-20     008C42AF     8845 D4                   MOV BYTE PTR SS:[EBP-0x2C], AL
	$-1D     008C42B2     8D41 01                   LEA EAX, DWORD PTR DS:[ECX+0x1]
	$-1A     008C42B5     FF75 D4                   PUSH DWORD PTR SS:[EBP-0x2C]                     ; d3d9.72F8CD5E
	$-17     008C42B8     8D8E 30020000             LEA ECX, DWORD PTR DS:[ESI+0x230]
	$-11     008C42BE     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$-E      008C42C1     57                        PUSH EDI
	$-D      008C42C2     E8 F9130000               CALL PathOfEx.008C56C0
	$-8      008C42C7     EB 11                     JMP SHORT PathOfEx.008C42DA
	$-6      008C42C9     F6C3 20                   TEST BL, 0x20
	$-3      008C42CC     74 0C                     JE SHORT PathOfEx.008C42DA
	$-1      008C42CE     57                        PUSH EDI                                         ; esi=this
	$ ==>    008C42CF     8D8E 30020000             LEA ECX, DWORD PTR DS:[ESI+0x230]                ; g_o_actor_get_skill_mgr_
	$+6      008C42D5     E8 361B0000               CALL PathOfEx.008C5E10
	$+B      008C42DA     33DB                      XOR EBX, EBX
	$+D      008C42DC     895D E8                   MOV DWORD PTR SS:[EBP-0x18], EBX
	$+10     008C42DF     895D DC                   MOV DWORD PTR SS:[EBP-0x24], EBX
	$+13     008C42E2     895D FC                   MOV DWORD PTR SS:[EBP-0x4], EBX
	$+16     008C42E5     66:8B45 F2                MOV AX, WORD PTR SS:[EBP-0xE]
	$+1A     008C42E9     A8 01                     TEST AL, 0x1
	$+1C     008C42EB     0F84 A3000000             JE PathOfEx.008C4394
	$+22     008C42F1     6A 02                     PUSH 0x2
	$+24     008C42F3     8BCF                      MOV ECX, EDI
	$+26     008C42F5     E8 D6BEBDFF               CALL PathOfEx.004A01D0
	$+2B     008C42FA     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+2E     008C42FD     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+30     008C42FF     0FB71401                  MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+34     008C4303     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+37     008C4306     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$+3A     008C4309     8D8E 30020000             LEA ECX, DWORD PTR DS:[ESI+0x230]
	$+40     008C430F     8D46 38                   LEA EAX, DWORD PTR DS:[ESI+0x38]
	$+43     008C4312     50                        PUSH EAX
	$+44     008C4313     52                        PUSH EDX
	$+45     008C4314     8D45 D4                   LEA EAX, DWORD PTR SS:[EBP-0x2C]
	$+48     008C4317     50                        PUSH EAX
	$+49     008C4318     E8 33D4F4FF               CALL PathOfEx.00811750
	$+4E     008C431D     8D4D DC                   LEA ECX, DWORD PTR SS:[EBP-0x24]
	$+51     008C4320     3BC8                      CMP ECX, EAX
	$+53     008C4322     74 0E                     JE SHORT PathOfEx.008C4332
	$+55     008C4324     8B18                      MOV EBX, DWORD PTR DS:[EAX]
	$+57     008C4326     895D E8                   MOV DWORD PTR SS:[EBP-0x18], EBX
	$+5A     008C4329     C700 00000000             MOV DWORD PTR DS:[EAX], 0x0
	$+60     008C432F     895D DC                   MOV DWORD PTR SS:[EBP-0x24], EBX
	$+63     008C4332     8B4D D4                   MOV ECX, DWORD PTR SS:[EBP-0x2C]                 ; d3d9.72F8CD5E
	$+66     008C4335     85C9                      TEST ECX, ECX
	$+68     008C4337     74 0E                     JE SHORT PathOfEx.008C4347
	$+6A     008C4339     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+6E     008C433D     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+70     008C433F     6A 01                     PUSH 0x1
	$+72     008C4341     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+74     008C4343     C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+78     008C4347     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
	$+7B     008C434A     8B48 10                   MOV ECX, DWORD PTR DS:[EAX+0x10]
	$+7E     008C434D     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+80     008C434F     FF50 14                   CALL NEAR DWORD PTR DS:[EAX+0x14]
	$+83     008C4352     50                        PUSH EAX
	$+84     008C4353     53                        PUSH EBX
	$+85     008C4354     57                        PUSH EDI
	$+86     008C4355     E8 E6002200               CALL PathOfEx.00AE4440
	$+8B     008C435A     83C4 0C                   ADD ESP, 0xC
	$+8E     008C435D     8BCB                      MOV ECX, EBX
	$+90     008C435F     E8 EC022200               CALL PathOfEx.00AE4650
	$+95     008C4364     B9 00800000               MOV ECX, 0x8000
	$+9A     008C4369     66:85C1                   TEST CX, AX
	$+9D     008C436C     75 22                     JNZ SHORT PathOfEx.008C4390
	$+9F     008C436E     8A46 38                   MOV AL, BYTE PTR DS:[ESI+0x38]
	$+A2     008C4371     84C0                      TEST AL, AL
	$+A4     008C4373     74 1B                     JE SHORT PathOfEx.008C4390
	$+A6     008C4375     8BCB                      MOV ECX, EBX
	$+A8     008C4377     33DB                      XOR EBX, EBX
	$+AA     008C4379     895D E8                   MOV DWORD PTR SS:[EBP-0x18], EBX
	$+AD     008C437C     895D DC                   MOV DWORD PTR SS:[EBP-0x24], EBX
	$+B0     008C437F     85C9                      TEST ECX, ECX
	$+B2     008C4381     74 0D                     JE SHORT PathOfEx.008C4390
	$+B4     008C4383     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+B8     008C4387     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+BA     008C4389     6A 01                     PUSH 0x1
	$+BC     008C438B     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+BE     008C438D     885D FC                   MOV BYTE PTR SS:[EBP-0x4], BL
	$+C1     008C4390     66:8B45 F2                MOV AX, WORD PTR SS:[EBP-0xE]
	$+C5     008C4394     A8 04                     TEST AL, 0x4
	$+C7     008C4396     0F84 AB000000             JE PathOfEx.008C4447
	$+CD     008C439C     6A 02                     PUSH 0x2
	$+CF     008C439E     8BCF                      MOV ECX, EDI
	$+D1     008C43A0     E8 2BBEBDFF               CALL PathOfEx.004A01D0
	$+D6     008C43A5     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+D9     008C43A8     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+DB     008C43AA     0FB71401                  MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+DF     008C43AE     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+E2     008C43B1     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$+E5     008C43B4     8D8E 30020000             LEA ECX, DWORD PTR DS:[ESI+0x230]
	$+EB     008C43BA     8D46 38                   LEA EAX, DWORD PTR DS:[ESI+0x38]
	$+EE     008C43BD     50                        PUSH EAX
	$+EF     008C43BE     52                        PUSH EDX
	$+F0     008C43BF     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
	$+F3     008C43C2     50                        PUSH EAX
	$+F4     008C43C3     E8 88D3F4FF               CALL PathOfEx.00811750
	$+F9     008C43C8     C645 FC 04                MOV BYTE PTR SS:[EBP-0x4], 0x4
	$+FD     008C43CC     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
	$+100    008C43CF     8B48 10                   MOV ECX, DWORD PTR DS:[EAX+0x10]
	$+103    008C43D2     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+105    008C43D4     FF50 14                   CALL NEAR DWORD PTR DS:[EAX+0x14]
	$+108    008C43D7     8B75 D8                   MOV ESI, DWORD PTR SS:[EBP-0x28]
	$+10B    008C43DA     50                        PUSH EAX
	$+10C    008C43DB     56                        PUSH ESI
	$+10D    008C43DC     57                        PUSH EDI
	$+10E    008C43DD     E8 5E002200               CALL PathOfEx.00AE4440
	$+113    008C43E2     83C4 0C                   ADD ESP, 0xC
	$+116    008C43E5     C745 AC 00000000          MOV DWORD PTR SS:[EBP-0x54], 0x0
	$+11D    008C43EC     C745 B0 00000000          MOV DWORD PTR SS:[EBP-0x50], 0x0
	$+124    008C43F3     C745 B4 00000000          MOV DWORD PTR SS:[EBP-0x4C], 0x0
	$+12B    008C43FA     8D55 AC                   LEA EDX, DWORD PTR SS:[EBP-0x54]
	$+12E    008C43FD     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4], 0x5
	$+132    008C4401     8BCF                      MOV ECX, EDI
	$+134    008C4403     E8 980D0000               CALL PathOfEx.008C51A0
	$+139    008C4408     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+13C    008C440B     8D4D AC                   LEA ECX, DWORD PTR SS:[EBP-0x54]
	$+13F    008C440E     51                        PUSH ECX
	$+140    008C440F     8BCE                      MOV ECX, ESI
	$+142    008C4411     C745 D8 00000000          MOV DWORD PTR SS:[EBP-0x28], 0x0
	$+149    008C4418     8348 70 02                OR DWORD PTR DS:[EAX+0x70], 0x2
	$+14D    008C441C     8970 30                   MOV DWORD PTR DS:[EAX+0x30], ESI
	$+150    008C441F     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+152    008C4421     FF50 78                   CALL NEAR DWORD PTR DS:[EAX+0x78]
	$+155    008C4424     8B4D AC                   MOV ECX, DWORD PTR SS:[EBP-0x54]
	$+158    008C4427     85C9                      TEST ECX, ECX
	$+15A    008C4429     74 11                     JE SHORT PathOfEx.008C443C
	$+15C    008C442B     8B45 B4                   MOV EAX, DWORD PTR SS:[EBP-0x4C]
	$+15F    008C442E     2BC1                      SUB EAX, ECX
	$+161    008C4430     6A 01                     PUSH 0x1
	$+163    008C4432     50                        PUSH EAX
	$+164    008C4433     51                        PUSH ECX
	$+165    008C4434     E8 B783BAFF               CALL PathOfEx.0046C7F0
	$+16A    008C4439     83C4 0C                   ADD ESP, 0xC
	$+16D    008C443C     8B75 EC                   MOV ESI, DWORD PTR SS:[EBP-0x14]
	$+170    008C443F     C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+174    008C4443     66:8B45 F2                MOV AX, WORD PTR SS:[EBP-0xE]
	$+178    008C4447     A8 08                     TEST AL, 0x8
	$+17A    008C4449     0F84 0C020000             JE PathOfEx.008C465B
	$+180    008C444F     6A 02                     PUSH 0x2
	$+182    008C4451     8BCF                      MOV ECX, EDI
	$+184    008C4453     E8 78BDBDFF               CALL PathOfEx.004A01D0
	$+189    008C4458     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+18C    008C445B     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+18E    008C445D     0FB71401                  MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+192    008C4461     8BC2                      MOV EAX, EDX
	$+194    008C4463     8945 D4                   MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+197    008C4466     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+19A    008C4469     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$+19D    008C446C     33C0                      XOR EAX, EAX
	$+19F    008C446E     66:3BC2                   CMP AX, DX
	$+1A2    008C4471     0F83 E0010000             JNB PathOfEx.008C4657
	$+1A8    008C4477     8B5D 08                   MOV EBX, DWORD PTR SS:[EBP+0x8]
	$+1AB    008C447A     8DBE 30020000             LEA EDI, DWORD PTR DS:[ESI+0x230]
	$+1B1    008C4480     8BC2                      MOV EAX, EDX
	$+1B3    008C4482     897D E0                   MOV DWORD PTR SS:[EBP-0x20], EDI
	$+1B6    008C4485     0FB7C0                    MOVZX EAX, AX
	$+1B9    008C4488     8945 D4                   MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+1BC    008C448B     0F1F4400 00               NOP DWORD PTR DS:[EAX+EAX]
	$+1C1    008C4490     33F6                      XOR ESI, ESI
	$+1C3    008C4492     8975 E4                   MOV DWORD PTR SS:[EBP-0x1C], ESI
	$+1C6    008C4495     6A 02                     PUSH 0x2
	$+1C8    008C4497     8BCB                      MOV ECX, EBX
	$+1CA    008C4499     C645 FC 07                MOV BYTE PTR SS:[EBP-0x4], 0x7
	$+1CE    008C449D     E8 2EBDBDFF               CALL PathOfEx.004A01D0
	$+1D3    008C44A2     8B4B 08                   MOV ECX, DWORD PTR DS:[EBX+0x8]
	$+1D6    008C44A5     8B03                      MOV EAX, DWORD PTR DS:[EBX]
	$+1D8    008C44A7     51                        PUSH ECX
	$+1D9    008C44A8     0FB71401                  MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+1DD    008C44AC     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+1E0    008C44AF     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
	$+1E3    008C44B2     8943 08                   MOV DWORD PTR DS:[EBX+0x8], EAX
	$+1E6    008C44B5     8B7F 14                   MOV EDI, DWORD PTR DS:[EDI+0x14]
	$+1E9    008C44B8     0FB7C2                    MOVZX EAX, DX
	$+1EC    008C44BB     8B49 10                   MOV ECX, DWORD PTR DS:[ECX+0x10]
	$+1EF    008C44BE     8945 D0                   MOV DWORD PTR SS:[EBP-0x30], EAX
	$+1F2    008C44C1     8D45 D0                   LEA EAX, DWORD PTR SS:[EBP-0x30]
	$+1F5    008C44C4     8955 D8                   MOV DWORD PTR SS:[EBP-0x28], EDX
	$+1F8    008C44C7     8BD7                      MOV EDX, EDI
	$+1FA    008C44C9     50                        PUSH EAX
	$+1FB    008C44CA     E8 E1E1F4FF               CALL PathOfEx.008126B0
	$+200    008C44CF     8B55 D8                   MOV EDX, DWORD PTR SS:[EBP-0x28]
	$+203    008C44D2     83C4 08                   ADD ESP, 0x8
	$+206    008C44D5     3BC7                      CMP EAX, EDI
	$+208    008C44D7     74 07                     JE SHORT PathOfEx.008C44E0
	$+20A    008C44D9     8B08                      MOV ECX, DWORD PTR DS:[EAX]
	$+20C    008C44DB     66:3B11                   CMP DX, WORD PTR DS:[ECX]
	$+20F    008C44DE     73 02                     JNB SHORT PathOfEx.008C44E2
	$+211    008C44E0     8BC7                      MOV EAX, EDI
	$+213    008C44E2     8B78 04                   MOV EDI, DWORD PTR DS:[EAX+0x4]
	$+216    008C44E5     8B00                      MOV EAX, DWORD PTR DS:[EAX]
	$+218    008C44E7     85FF                      TEST EDI, EDI
	$+21A    008C44E9     74 04                     JE SHORT PathOfEx.008C44EF
	$+21C    008C44EB     F0:FF47 04                LOCK INC DWORD PTR DS:[EDI+0x4]
	$+220    008C44EF     897D BC                   MOV DWORD PTR SS:[EBP-0x44], EDI
	$+223    008C44F2     8945 B8                   MOV DWORD PTR SS:[EBP-0x48], EAX
	$+226    008C44F5     C645 FC 09                MOV BYTE PTR SS:[EBP-0x4], 0x9
	$+22A    008C44F9     8A48 16                   MOV CL, BYTE PTR DS:[EAX+0x16]
	$+22D    008C44FC     84C9                      TEST CL, CL
	$+22F    008C44FE     74 78                     JE SHORT PathOfEx.008C4578
	$+231    008C4500     8078 17 00                CMP BYTE PTR DS:[EAX+0x17], 0x0
	$+235    008C4504     75 33                     JNZ SHORT PathOfEx.008C4539
	$+237    008C4506     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]                 ; PathOfEx.006A55D1
	$+23A    008C4509     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]                 ; PathOfEx.00D3FC90
	$+23D    008C450C     83C0 38                   ADD EAX, 0x38
	$+240    008C450F     50                        PUSH EAX
	$+241    008C4510     52                        PUSH EDX
	$+242    008C4511     8D45 CC                   LEA EAX, DWORD PTR SS:[EBP-0x34]
	$+245    008C4514     50                        PUSH EAX
	$+246    008C4515     E8 36D2F4FF               CALL PathOfEx.00811750
	$+24B    008C451A     8D4D E4                   LEA ECX, DWORD PTR SS:[EBP-0x1C]
	$+24E    008C451D     3BC8                      CMP ECX, EAX
	$+250    008C451F     74 0B                     JE SHORT PathOfEx.008C452C
	$+252    008C4521     8B30                      MOV ESI, DWORD PTR DS:[EAX]
	$+254    008C4523     C700 00000000             MOV DWORD PTR DS:[EAX], 0x0
	$+25A    008C4529     8975 E4                   MOV DWORD PTR SS:[EBP-0x1C], ESI
	$+25D    008C452C     8B4D CC                   MOV ECX, DWORD PTR SS:[EBP-0x34]
	$+260    008C452F     85C9                      TEST ECX, ECX
	$+262    008C4531     74 45                     JE SHORT PathOfEx.008C4578
	$+264    008C4533     C645 FC 0B                MOV BYTE PTR SS:[EBP-0x4], 0xB
	$+268    008C4537     EB 35                     JMP SHORT PathOfEx.008C456E
	$+26A    008C4539     84C9                      TEST CL, CL
	$+26C    008C453B     74 3B                     JE SHORT PathOfEx.008C4578
	$+26E    008C453D     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]                 ; PathOfEx.006A55D1
	$+271    008C4540     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]                 ; PathOfEx.00D3FC90
	$+274    008C4543     83C0 38                   ADD EAX, 0x38
	$+277    008C4546     50                        PUSH EAX
	$+278    008C4547     52                        PUSH EDX
	$+279    008C4548     8D45 C8                   LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+27C    008C454B     50                        PUSH EAX
	$+27D    008C454C     E8 2FD3F4FF               CALL PathOfEx.00811880
	$+282    008C4551     8D4D E4                   LEA ECX, DWORD PTR SS:[EBP-0x1C]
	$+285    008C4554     3BC8                      CMP ECX, EAX
	$+287    008C4556     74 0B                     JE SHORT PathOfEx.008C4563
	$+289    008C4558     8B30                      MOV ESI, DWORD PTR DS:[EAX]
	$+28B    008C455A     C700 00000000             MOV DWORD PTR DS:[EAX], 0x0
	$+291    008C4560     8975 E4                   MOV DWORD PTR SS:[EBP-0x1C], ESI
	$+294    008C4563     8B4D C8                   MOV ECX, DWORD PTR SS:[EBP-0x38]
	$+297    008C4566     85C9                      TEST ECX, ECX
	$+299    008C4568     74 0E                     JE SHORT PathOfEx.008C4578
	$+29B    008C456A     C645 FC 0D                MOV BYTE PTR SS:[EBP-0x4], 0xD
	$+29F    008C456E     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+2A1    008C4570     6A 01                     PUSH 0x1
	$+2A3    008C4572     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+2A5    008C4574     C645 FC 09                MOV BYTE PTR SS:[EBP-0x4], 0x9
	$+2A9    008C4578     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]                 ; PathOfEx.006A55D1
	$+2AC    008C457B     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+2AF    008C457E     8B48 10                   MOV ECX, DWORD PTR DS:[EAX+0x10]
	$+2B2    008C4581     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+2B4    008C4583     FF50 14                   CALL NEAR DWORD PTR DS:[EAX+0x14]
	$+2B7    008C4586     50                        PUSH EAX
	$+2B8    008C4587     56                        PUSH ESI
	$+2B9    008C4588     53                        PUSH EBX
	$+2BA    008C4589     E8 B2FE2100               CALL PathOfEx.00AE4440
	$+2BF    008C458E     83C4 0C                   ADD ESP, 0xC
	$+2C2    008C4591     8BCB                      MOV ECX, EBX
	$+2C4    008C4593     6A 01                     PUSH 0x1
	$+2C6    008C4595     E8 36BCBDFF               CALL PathOfEx.004A01D0
	$+2CB    008C459A     8B4B 08                   MOV ECX, DWORD PTR DS:[EBX+0x8]
	$+2CE    008C459D     8B03                      MOV EAX, DWORD PTR DS:[EBX]
	$+2D0    008C459F     8A0401                    MOV AL, BYTE PTR DS:[ECX+EAX]
	$+2D3    008C45A2     8845 C4                   MOV BYTE PTR SS:[EBP-0x3C], AL
	$+2D6    008C45A5     8D41 01                   LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+2D9    008C45A8     8943 08                   MOV DWORD PTR DS:[EBX+0x8], EAX
	$+2DC    008C45AB     C745 AC 00000000          MOV DWORD PTR SS:[EBP-0x54], 0x0
	$+2E3    008C45B2     C745 B0 00000000          MOV DWORD PTR SS:[EBP-0x50], 0x0
	$+2EA    008C45B9     C745 B4 00000000          MOV DWORD PTR SS:[EBP-0x4C], 0x0
	$+2F1    008C45C0     8D55 AC                   LEA EDX, DWORD PTR SS:[EBP-0x54]
	$+2F4    008C45C3     C645 FC 0E                MOV BYTE PTR SS:[EBP-0x4], 0xE
	$+2F8    008C45C7     8BCB                      MOV ECX, EBX
	$+2FA    008C45C9     E8 D20B0000               CALL PathOfEx.008C51A0
	$+2FF    008C45CE     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+301    008C45D0     8D4D AC                   LEA ECX, DWORD PTR SS:[EBP-0x54]
	$+304    008C45D3     51                        PUSH ECX
	$+305    008C45D4     8BCE                      MOV ECX, ESI
	$+307    008C45D6     FF50 78                   CALL NEAR DWORD PTR DS:[EAX+0x78]
	$+30A    008C45D9     FF75 C4                   PUSH DWORD PTR SS:[EBP-0x3C]
	$+30D    008C45DC     8BCE                      MOV ECX, ESI
	$+30F    008C45DE     E8 2DFF2100               CALL PathOfEx.00AE4510
	$+314    008C45E3     8B4D AC                   MOV ECX, DWORD PTR SS:[EBP-0x54]
	$+317    008C45E6     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C], 0x0
	$+31E    008C45ED     85C9                      TEST ECX, ECX
	$+320    008C45EF     74 26                     JE SHORT PathOfEx.008C4617
	$+322    008C45F1     8B45 B4                   MOV EAX, DWORD PTR SS:[EBP-0x4C]                 ; PathOfEx.00FF6BE4
	$+325    008C45F4     2BC1                      SUB EAX, ECX
	$+327    008C45F6     6A 01                     PUSH 0x1
	$+329    008C45F8     50                        PUSH EAX
	$+32A    008C45F9     51                        PUSH ECX
	$+32B    008C45FA     E8 F181BAFF               CALL PathOfEx.0046C7F0
	$+330    008C45FF     83C4 0C                   ADD ESP, 0xC
	$+333    008C4602     C745 AC 00000000          MOV DWORD PTR SS:[EBP-0x54], 0x0
	$+33A    008C4609     C745 B0 00000000          MOV DWORD PTR SS:[EBP-0x50], 0x0
	$+341    008C4610     C745 B4 00000000          MOV DWORD PTR SS:[EBP-0x4C], 0x0
	$+348    008C4617     C645 FC 0F                MOV BYTE PTR SS:[EBP-0x4], 0xF
	$+34C    008C461B     85FF                      TEST EDI, EDI
	$+34E    008C461D     74 21                     JE SHORT PathOfEx.008C4640
	$+350    008C461F     83C8 FF                   OR EAX, -0x1
	$+353    008C4622     F0:0FC147 04              LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+358    008C4627     75 17                     JNZ SHORT PathOfEx.008C4640
	$+35A    008C4629     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+35C    008C462B     8BCF                      MOV ECX, EDI
	$+35E    008C462D     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+360    008C462F     83C8 FF                   OR EAX, -0x1
	$+363    008C4632     F0:0FC147 08              LOCK XADD DWORD PTR DS:[EDI+0x8], EAX
	$+368    008C4637     75 07                     JNZ SHORT PathOfEx.008C4640
	$+36A    008C4639     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+36C    008C463B     8BCF                      MOV ECX, EDI
	$+36E    008C463D     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+371    008C4640     C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+375    008C4644     836D D4 01                SUB DWORD PTR SS:[EBP-0x2C], 0x1
	$+379    008C4648     8B7D E0                   MOV EDI, DWORD PTR SS:[EBP-0x20]
	$+37C    008C464B   ^ 0F85 3FFEFFFF             JNZ PathOfEx.008C4490
	$+382    008C4651     8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+385    008C4654     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+388    008C4657     66:8B45 F2                MOV AX, WORD PTR SS:[EBP-0xE]
	$+38C    008C465B     33F6                      XOR ESI, ESI
	$+38E    008C465D     33C9                      XOR ECX, ECX
	$+390    008C465F     8975 D4                   MOV DWORD PTR SS:[EBP-0x2C], ESI
	$+393    008C4662     894D E0                   MOV DWORD PTR SS:[EBP-0x20], ECX
	$+396    008C4665     A8 10                     TEST AL, 0x10
	$+398    008C4667     74 6F                     JE SHORT PathOfEx.008C46D8
	$+39A    008C4669     6A 02                     PUSH 0x2
	$+39C    008C466B     8BCF                      MOV ECX, EDI
	$+39E    008C466D     E8 5EBBBDFF               CALL PathOfEx.004A01D0
	$+3A3    008C4672     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+3A6    008C4675     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+3A8    008C4677     0FB71401                  MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+3AC    008C467B     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+3AF    008C467E     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$+3B2    008C4681     0FB7C2                    MOVZX EAX, DX
	$+3B5    008C4684     8945 08                   MOV DWORD PTR SS:[EBP+0x8], EAX
	$+3B8    008C4687     8955 E0                   MOV DWORD PTR SS:[EBP-0x20], EDX
	$+3BB    008C468A     8D0480                    LEA EAX, DWORD PTR DS:[EAX+EAX*4]
	$+3BE    008C468D     C1E0 03                   SHL EAX, 0x3
	$+3C1    008C4690     E8 AB643B00               CALL PathOfEx.00C7AB40
	$+3C6    008C4695     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
	$+3C9    008C4698     33C0                      XOR EAX, EAX
	$+3CB    008C469A     8BF4                      MOV ESI, ESP
	$+3CD    008C469C     8975 D4                   MOV DWORD PTR SS:[EBP-0x2C], ESI
	$+3D0    008C469F     66:3BC1                   CMP AX, CX
	$+3D3    008C46A2     73 30                     JNB SHORT PathOfEx.008C46D4
	$+3D5    008C46A4     8B5D 08                   MOV EBX, DWORD PTR SS:[EBP+0x8]
	$+3D8    008C46A7     57                        PUSH EDI
	$+3D9    008C46A8     8D4D 84                   LEA ECX, DWORD PTR SS:[EBP-0x7C]
	$+3DC    008C46AB     E8 D0094300               CALL PathOfEx.00CF5080
	$+3E1    008C46B0     8D76 28                   LEA ESI, DWORD PTR DS:[ESI+0x28]
	$+3E4    008C46B3     0F1000                    MOVUPS XMM0, DQWORD PTR DS:[EAX]
	$+3E7    008C46B6     0F1146 D8                 MOVUPS DQWORD PTR DS:[ESI-0x28], XMM0
	$+3EB    008C46BA     0F1040 10                 MOVUPS XMM0, DQWORD PTR DS:[EAX+0x10]
	$+3EF    008C46BE     0F1146 E8                 MOVUPS DQWORD PTR DS:[ESI-0x18], XMM0
	$+3F3    008C46C2     F30F7E40 20               MOVQ XMM0, QWORD PTR DS:[EAX+0x20]
	$+3F8    008C46C7     660FD646 F8               MOVQ QWORD PTR DS:[ESI-0x8], XMM0
	$+3FD    008C46CC     83EB 01                   SUB EBX, 0x1
	$+400    008C46CF   ^ 75 D6                     JNZ SHORT PathOfEx.008C46A7
	$+402    008C46D1     8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+405    008C46D4     66:8B45 F2                MOV AX, WORD PTR SS:[EBP-0xE]
	$+409    008C46D8     C745 D8 00000000          MOV DWORD PTR SS:[EBP-0x28], 0x0
	$+410    008C46DF     C745 08 00000000          MOV DWORD PTR SS:[EBP+0x8], 0x0
	$+417    008C46E6     F6C4 01                   TEST AH, 0x1
	$+41A    008C46E9     0F84 8E000000             JE PathOfEx.008C477D
	$+420    008C46EF     6A 02                     PUSH 0x2
	$+422    008C46F1     8BCF                      MOV ECX, EDI
	$+424    008C46F3     E8 D8BABDFF               CALL PathOfEx.004A01D0
	$+429    008C46F8     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+42C    008C46FB     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+42E    008C46FD     0FB73401                  MOVZX ESI, WORD PTR DS:[ECX+EAX]
	$+432    008C4701     8D41 02                   LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+435    008C4704     8947 08                   MOV DWORD PTR DS:[EDI+0x8], EAX
	$+438    008C4707     C1E6 03                   SHL ESI, 0x3
	$+43B    008C470A     8BC6                      MOV EAX, ESI
	$+43D    008C470C     E8 2F643B00               CALL PathOfEx.00C7AB40
	$+442    008C4711     8BCC                      MOV ECX, ESP
	$+444    008C4713     894D D8                   MOV DWORD PTR SS:[EBP-0x28], ECX
	$+447    008C4716     8BC1                      MOV EAX, ECX
	$+449    008C4718     8D140E                    LEA EDX, DWORD PTR DS:[ESI+ECX]
	$+44C    008C471B     8955 08                   MOV DWORD PTR SS:[EBP+0x8], EDX
	$+44F    008C471E     3BCA                      CMP ECX, EDX
	$+451    008C4720     74 17                     JE SHORT PathOfEx.008C4739
	$+453    008C4722     85C0                      TEST EAX, EAX
	$+455    008C4724     74 0C                     JE SHORT PathOfEx.008C4732
	$+457    008C4726     33F6                      XOR ESI, ESI
	$+459    008C4728     C700 00000000             MOV DWORD PTR DS:[EAX], 0x0
	$+45F    008C472E     66:8970 04                MOV WORD PTR DS:[EAX+0x4], SI
	$+463    008C4732     83C0 08                   ADD EAX, 0x8
	$+466    008C4735     3BC2                      CMP EAX, EDX
	$+468    008C4737   ^ 75 E9                     JNZ SHORT PathOfEx.008C4722
	$+46A    008C4739     8BF1                      MOV ESI, ECX
	$+46C    008C473B     3BCA                      CMP ECX, EDX
	$+46E    008C473D     74 3E                     JE SHORT PathOfEx.008C477D
	$+470    008C473F     8B5D 08                   MOV EBX, DWORD PTR SS:[EBP+0x8]
	$+473    008C4742     6A 04                     PUSH 0x4
	$+475    008C4744     8BCF                      MOV ECX, EDI
	$+477    008C4746     E8 85BABDFF               CALL PathOfEx.004A01D0
	$+47C    008C474B     8B0F                      MOV ECX, DWORD PTR DS:[EDI]
	$+47E    008C474D     8B47 08                   MOV EAX, DWORD PTR DS:[EDI+0x8]
	$+481    008C4750     6A 02                     PUSH 0x2
	$+483    008C4752     8B0401                    MOV EAX, DWORD PTR DS:[ECX+EAX]                  ; wship6.6F440000
	$+486    008C4755     8BCF                      MOV ECX, EDI
	$+488    008C4757     8906                      MOV DWORD PTR DS:[ESI], EAX
	$+48A    008C4759     8347 08 04                ADD DWORD PTR DS:[EDI+0x8], 0x4
	$+48E    008C475D     E8 6EBABDFF               CALL PathOfEx.004A01D0
	$+493    008C4762     8B47 08                   MOV EAX, DWORD PTR DS:[EDI+0x8]
	$+496    008C4765     8B0F                      MOV ECX, DWORD PTR DS:[EDI]
	$+498    008C4767     66:8B0401                 MOV AX, WORD PTR DS:[ECX+EAX]
	$+49C    008C476B     66:8946 04                MOV WORD PTR DS:[ESI+0x4], AX
	$+4A0    008C476F     83C6 08                   ADD ESI, 0x8
	$+4A3    008C4772     8347 08 02                ADD DWORD PTR DS:[EDI+0x8], 0x2
	$+4A7    008C4776     3BF3                      CMP ESI, EBX
	$+4A9    008C4778   ^ 75 C8                     JNZ SHORT PathOfEx.008C4742
	$+4AB    008C477A     8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+4AE    008C477D     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]                 ; PluginTe.20000000
	$+4B1    008C4780     33C9                      XOR ECX, ECX
	$+4B3    008C4782     8B7D EC                   MOV EDI, DWORD PTR SS:[EBP-0x14]
	$+4B6    008C4785     66:3BC8                   CMP CX, AX
	$+4B9    008C4788     73 19                     JNB SHORT PathOfEx.008C47A3
	$+4BB    008C478A     8B75 D4                   MOV ESI, DWORD PTR SS:[EBP-0x2C]
	$+4BE    008C478D     0FB7D8                    MOVZX EBX, AX
	$+4C1    008C4790     56                        PUSH ESI
	$+4C2    008C4791     8BCF                      MOV ECX, EDI
	$+4C4    008C4793     E8 D82EF2FF               CALL PathOfEx.007E7670
	$+4C9    008C4798     83C6 28                   ADD ESI, 0x28
	$+4CC    008C479B     83EB 01                   SUB EBX, 0x1
	$+4CF    008C479E   ^ 75 F0                     JNZ SHORT PathOfEx.008C4790
	$+4D1    008C47A0     8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+4D4    008C47A3     8B47 70                   MOV EAX, DWORD PTR DS:[EDI+0x70]
	$+4D7    008C47A6     8A4D F2                   MOV CL, BYTE PTR SS:[EBP-0xE]
	$+4DA    008C47A9     C1E8 0B                   SHR EAX, 0xB
	$+4DD    008C47AC     C0E9 07                   SHR CL, 0x7
	$+4E0    008C47AF     24 01                     AND AL, 0x1
	$+4E2    008C47B1     3AC8                      CMP CL, AL
	$+4E4    008C47B3     74 39                     JE SHORT PathOfEx.008C47EE
	$+4E6    008C47B5     84C9                      TEST CL, CL
	$+4E8    008C47B7     74 09                     JE SHORT PathOfEx.008C47C2
	$+4EA    008C47B9     814F 70 00080000          OR DWORD PTR DS:[EDI+0x70], 0x800
	$+4F1    008C47C0     EB 07                     JMP SHORT PathOfEx.008C47C9
	$+4F3    008C47C2     8167 70 FFF7FFFF          AND DWORD PTR DS:[EDI+0x70], -0x801
	$+4FA    008C47C9     8B77 58                   MOV ESI, DWORD PTR DS:[EDI+0x58]                 ; PathOfEx.00DE35BC
	$+4FD    008C47CC     3B77 5C                   CMP ESI, DWORD PTR DS:[EDI+0x5C]
	$+500    008C47CF     74 1D                     JE SHORT PathOfEx.008C47EE
	$+502    008C47D1     8B47 70                   MOV EAX, DWORD PTR DS:[EDI+0x70]
	$+505    008C47D4     8B0E                      MOV ECX, DWORD PTR DS:[ESI]
	$+507    008C47D6     C1E8 0B                   SHR EAX, 0xB
	$+50A    008C47D9     24 01                     AND AL, 0x1
	$+50C    008C47DB     8845 C4                   MOV BYTE PTR SS:[EBP-0x3C], AL
	$+50F    008C47DE     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+511    008C47E0     FF75 C4                   PUSH DWORD PTR SS:[EBP-0x3C]                     ; PluginTe.20000000
	$+514    008C47E3     FF50 28                   CALL NEAR DWORD PTR DS:[EAX+0x28]
	$+517    008C47E6     83C6 04                   ADD ESI, 0x4
	$+51A    008C47E9     3B77 5C                   CMP ESI, DWORD PTR DS:[EDI+0x5C]
	$+51D    008C47EC   ^ 75 E3                     JNZ SHORT PathOfEx.008C47D1
	$+51F    008C47EE     8B45 08                   MOV EAX, DWORD PTR SS:[EBP+0x8]
	$+522    008C47F1     3945 D8                   CMP DWORD PTR SS:[EBP-0x28], EAX
	$+525    008C47F4     0F84 DE000000             JE PathOfEx.008C48D8
	$+52B    008C47FA     8B5D D8                   MOV EBX, DWORD PTR SS:[EBP-0x28]
	$+52E    008C47FD     8D97 68020000             LEA EDX, DWORD PTR DS:[EDI+0x268]
	$+534    008C4803     66:837B 04 00             CMP WORD PTR DS:[EBX+0x4], 0x0
	$+539    008C4808     74 0A                     JE SHORT PathOfEx.008C4814
	$+53B    008C480A     53                        PUSH EBX
	$+53C    008C480B     8BCA                      MOV ECX, EDX
	$+53E    008C480D     E8 1EFFC3FF               CALL PathOfEx.00504730
	$+543    008C4812     EB 4E                     JMP SHORT PathOfEx.008C4862
	$+545    008C4814     8B8F 6C020000             MOV ECX, DWORD PTR DS:[EDI+0x26C]                ; PathOfEx.00DE3574
	$+54B    008C481A     8B02                      MOV EAX, DWORD PTR DS:[EDX]
	$+54D    008C481C     3BC1                      CMP EAX, ECX
	$+54F    008C481E     74 42                     JE SHORT PathOfEx.008C4862
	$+551    008C4820     8B13                      MOV EDX, DWORD PTR DS:[EBX]
	$+553    008C4822     3910                      CMP DWORD PTR DS:[EAX], EDX
	$+555    008C4824     74 07                     JE SHORT PathOfEx.008C482D
	$+557    008C4826     83C0 08                   ADD EAX, 0x8
	$+55A    008C4829     3BC1                      CMP EAX, ECX
	$+55C    008C482B   ^ 75 F5                     JNZ SHORT PathOfEx.008C4822
	$+55E    008C482D     8D97 68020000             LEA EDX, DWORD PTR DS:[EDI+0x268]
	$+564    008C4833     3BC1                      CMP EAX, ECX
	$+566    008C4835     74 2B                     JE SHORT PathOfEx.008C4862
	$+568    008C4837     8B72 04                   MOV ESI, DWORD PTR DS:[EDX+0x4]
	$+56B    008C483A     8D48 08                   LEA ECX, DWORD PTR DS:[EAX+0x8]
	$+56E    008C483D     3BCE                      CMP ECX, ESI
	$+570    008C483F     74 1D                     JE SHORT PathOfEx.008C485E
	$+572    008C4841     2BC1                      SUB EAX, ECX
	$+574    008C4843     8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$+576    008C4845     891401                    MOV DWORD PTR DS:[ECX+EAX], EDX
	$+579    008C4848     66:8B51 04                MOV DX, WORD PTR DS:[ECX+0x4]
	$+57D    008C484C     66:895408 04              MOV WORD PTR DS:[EAX+ECX+0x4], DX
	$+582    008C4851     83C1 08                   ADD ECX, 0x8
	$+585    008C4854     3BCE                      CMP ECX, ESI
	$+587    008C4856   ^ 75 EB                     JNZ SHORT PathOfEx.008C4843
	$+589    008C4858     8D97 68020000             LEA EDX, DWORD PTR DS:[EDI+0x268]
	$+58F    008C485E     8342 04 F8                ADD DWORD PTR DS:[EDX+0x4], -0x8
	$+593    008C4862     8B4F 04                   MOV ECX, DWORD PTR DS:[EDI+0x4]
	$+596    008C4865     8B03                      MOV EAX, DWORD PTR DS:[EBX]
	$+598    008C4867     8945 D4                   MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+59B    008C486A     8B49 10                   MOV ECX, DWORD PTR DS:[ECX+0x10]
	$+59E    008C486D     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+5A0    008C486F     FF50 14                   CALL NEAR DWORD PTR DS:[EAX+0x14]
	$+5A3    008C4872     8945 C8                   MOV DWORD PTR SS:[EBP-0x38], EAX
	$+5A6    008C4875     8D70 0C                   LEA ESI, DWORD PTR DS:[EAX+0xC]
	$+5A9    008C4878     8D45 D4                   LEA EAX, DWORD PTR SS:[EBP-0x2C]
	$+5AC    008C487B     8BCE                      MOV ECX, ESI
	$+5AE    008C487D     50                        PUSH EAX
	$+5AF    008C487E     8D45 C4                   LEA EAX, DWORD PTR SS:[EBP-0x3C]
	$+5B2    008C4881     50                        PUSH EAX
	$+5B3    008C4882     E8 B981CEFF               CALL PathOfEx.005ACA40
	$+5B8    008C4887     8B45 C4                   MOV EAX, DWORD PTR SS:[EBP-0x3C]
	$+5BB    008C488A     3B06                      CMP EAX, DWORD PTR DS:[ESI]
	$+5BD    008C488C     75 1F                     JNZ SHORT PathOfEx.008C48AD
	$+5BF    008C488E     8B75 C8                   MOV ESI, DWORD PTR SS:[EBP-0x38]
	$+5C2    008C4891     8D45 D4                   LEA EAX, DWORD PTR SS:[EBP-0x2C]
	$+5C5    008C4894     50                        PUSH EAX
	$+5C6    008C4895     8D45 CC                   LEA EAX, DWORD PTR SS:[EBP-0x34]
	$+5C9    008C4898     50                        PUSH EAX
	$+5CA    008C4899     8D4E 14                   LEA ECX, DWORD PTR DS:[ESI+0x14]
	$+5CD    008C489C     E8 9F81CEFF               CALL PathOfEx.005ACA40
	$+5D2    008C48A1     8B45 CC                   MOV EAX, DWORD PTR SS:[EBP-0x34]
	$+5D5    008C48A4     3B46 14                   CMP EAX, DWORD PTR DS:[ESI+0x14]
	$+5D8    008C48A7     75 04                     JNZ SHORT PathOfEx.008C48AD
	$+5DA    008C48A9     33C9                      XOR ECX, ECX
	$+5DC    008C48AB     EB 03                     JMP SHORT PathOfEx.008C48B0
	$+5DE    008C48AD     8B48 14                   MOV ECX, DWORD PTR DS:[EAX+0x14]
	$+5E1    008C48B0     0FB743 04                 MOVZX EAX, WORD PTR DS:[EBX+0x4]
	$+5E5    008C48B4     50                        PUSH EAX
	$+5E6    008C48B5     FF33                      PUSH DWORD PTR DS:[EBX]
	$+5E8    008C48B7     51                        PUSH ECX
	$+5E9    008C48B8     8D8F B0010000             LEA ECX, DWORD PTR DS:[EDI+0x1B0]
	$+5EF    008C48BE     E8 7DC5D6FF               CALL PathOfEx.00630E40
	$+5F4    008C48C3     83C3 08                   ADD EBX, 0x8
	$+5F7    008C48C6     8D97 68020000             LEA EDX, DWORD PTR DS:[EDI+0x268]
	$+5FD    008C48CC     3B5D 08                   CMP EBX, DWORD PTR SS:[EBP+0x8]
	$+600    008C48CF   ^ 0F85 2EFFFFFF             JNZ PathOfEx.008C4803
	$+606    008C48D5     8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-0x18]
	$+609    008C48D8     85DB                      TEST EBX, EBX
	$+60B    008C48DA     74 21                     JE SHORT PathOfEx.008C48FD
	$+60D    008C48DC     8D4D C0                   LEA ECX, DWORD PTR SS:[EBP-0x40]
	$+610    008C48DF     C745 C0 00000080          MOV DWORD PTR SS:[EBP-0x40], 0x80000000
	$+617    008C48E6     8BC3                      MOV EAX, EBX
	$+619    008C48E8     C745 C4 00000080          MOV DWORD PTR SS:[EBP-0x3C], 0x80000000
	$+620    008C48EF     51                        PUSH ECX
	$+621    008C48F0     33DB                      XOR EBX, EBX
	$+623    008C48F2     8BCF                      MOV ECX, EDI
	$+625    008C48F4     50                        PUSH EAX
	$+626    008C48F5     895D DC                   MOV DWORD PTR SS:[EBP-0x24], EBX
	$+629    008C48F8     E8 231EF2FF               CALL PathOfEx.007E6720
	$+62E    008C48FD     F645 F2 02                TEST BYTE PTR SS:[EBP-0xE], 0x2
	$+632    008C4901     74 24                     JE SHORT PathOfEx.008C4927
	$+634    008C4903     834F 70 10                OR DWORD PTR DS:[EDI+0x70], 0x10
	$+638    008C4907     837F 30 00                CMP DWORD PTR DS:[EDI+0x30], 0x0
	$+63C    008C490B     74 1A                     JE SHORT PathOfEx.008C4927
	$+63E    008C490D     8B4F 70                   MOV ECX, DWORD PTR DS:[EDI+0x70]
	$+641    008C4910     8BC1                      MOV EAX, ECX
	$+643    008C4912     C1E8 07                   SHR EAX, 0x7
	$+646    008C4915     24 01                     AND AL, 0x1
	$+648    008C4917     75 0E                     JNZ SHORT PathOfEx.008C4927
	$+64A    008C4919     D1E9                      SHR ECX, 1
	$+64C    008C491B     80E1 01                   AND CL, 0x1
	$+64F    008C491E     75 07                     JNZ SHORT PathOfEx.008C4927
	$+651    008C4920     8BCF                      MOV ECX, EDI
	$+653    008C4922     E8 4924F2FF               CALL PathOfEx.007E6D70
	$+658    008C4927     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+65F    008C492E     85DB                      TEST EBX, EBX
	$+661    008C4930     74 0F                     JE SHORT PathOfEx.008C4941
	$+663    008C4932     C745 FC 11000000          MOV DWORD PTR SS:[EBP-0x4], 0x11
	$+66A    008C4939     8BCB                      MOV ECX, EBX
	$+66C    008C493B     8B03                      MOV EAX, DWORD PTR DS:[EBX]
	$+66E    008C493D     6A 01                     PUSH 0x1
	$+670    008C493F     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+672    008C4941     8DA5 78FFFFFF             LEA ESP, DWORD PTR SS:[EBP-0x88]
	$+678    008C4947     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+67B    008C494A     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
	$+682    008C4951     5F                        POP EDI                                          ; 0018ED28
	$+683    008C4952     5E                        POP ESI                                          ; 0018ED28
	$+684    008C4953     5B                        POP EBX                                          ; 0018ED28
	$+685    008C4954     8BE5                      MOV ESP, EBP
	$+687    008C4956     5D                        POP EBP                                          ; 0018ED28
	$+688    008C4957     C2 0400                   RETN 0x4
	$+68B    008C495A     CC                        INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8d??????????e8????????33??89????89????89????66??????a8??0f", 2);
	assert(op_dw_tmp);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x150 && dw_tmp < 0x400);
	ofs << "pt_cdword	g_o_actor_get_skill_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-44     006289AC     5E                        POP ESI                                          ; 0C61D8B0
	$-43     006289AD     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
	$-3C     006289B4     8BE5                      MOV ESP, EBP
	$-3A     006289B6     5D                        POP EBP                                          ; 0C61D8B0
	$-39     006289B7     C2 0400                   RETN 0x4
	$-36     006289BA     CC                        INT3
	$-35     006289BB     CC                        INT3
	$-34     006289BC     CC                        INT3
	$-33     006289BD     CC                        INT3
	$-32     006289BE     CC                        INT3
	$-31     006289BF     CC                        INT3
	$-30     006289C0     81C1 50F7FFFF             ADD ECX, -0x8B0                                  ; g_c_my_skill_mgr_base_fn_tag_
	$-2A     006289C6     E9 05000000               JMP PathOfEx.006289D0                            ; g_o_get_my_skill_mgr_
	$-25     006289CB     CC                        INT3
	$-24     006289CC     CC                        INT3
	$-23     006289CD     CC                        INT3
	$-22     006289CE     CC                        INT3
	$-21     006289CF     CC                        INT3
	$-20     006289D0     55                        PUSH EBP
	$-1F     006289D1     8BEC                      MOV EBP, ESP
	$-1D     006289D3     6A FF                     PUSH -0x1
	$-1B     006289D5     68 CA80D300               PUSH PathOfEx.00D380CA
	$-16     006289DA     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
	$-10     006289E0     50                        PUSH EAX
	$-F      006289E1     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
	$-8      006289E8     83EC 34                   SUB ESP, 0x34
	$-5      006289EB     8BC1                      MOV EAX, ECX
	$-3      006289ED     53                        PUSH EBX
	$-2      006289EE     56                        PUSH ESI
	$-1      006289EF     57                        PUSH EDI
	$ ==>    006289F0     8BB0 C0080000             MOV ESI, DWORD PTR DS:[EAX+0x8C0]
	$+6      006289F6     8BB8 C0080000             MOV EDI, DWORD PTR DS:[EAX+0x8C0]
	$+C      006289FC     8945 E8                   MOV DWORD PTR SS:[EBP-0x18], EAX
	$+F      006289FF     8B36                      MOV ESI, DWORD PTR DS:[ESI]
	$+11     00628A01     3BF7                      CMP ESI, EDI
	$+13     00628A03     74 67                     JE SHORT PathOfEx.00628A6C
	$+15     00628A05     8D98 04070000             LEA EBX, DWORD PTR DS:[EAX+0x704]
	$+1B     00628A0B     895D F0                   MOV DWORD PTR SS:[EBP-0x10], EBX
	$+1E     00628A0E     66:90                     NOP
	$+20     00628A10     8B43 08                   MOV EAX, DWORD PTR DS:[EBX+0x8]
	$+23     00628A13     8B53 04                   MOV EDX, DWORD PTR DS:[EBX+0x4]
	$+26     00628A16     8B4E 08                   MOV ECX, DWORD PTR DS:[ESI+0x8]
	$+29     00628A19     3BD0                      CMP EDX, EAX
	$+2B     00628A1B     74 0E                     JE SHORT PathOfEx.00628A2B
	$+2D     00628A1D     0F1F00                    NOP DWORD PTR DS:[EAX]
	$+30     00628A20     390A                      CMP DWORD PTR DS:[EDX], ECX
	$+32     00628A22     74 07                     JE SHORT PathOfEx.00628A2B
	$+34     00628A24     83C2 04                   ADD EDX, 0x4
	$+37     00628A27     3BD0                      CMP EDX, EAX
	$+39     00628A29   ^ 75 F5                     JNZ SHORT PathOfEx.00628A20
	$+3B     00628A2B     2B53 04                   SUB EDX, DWORD PTR DS:[EBX+0x4]
	$+3E     00628A2E     8B43 04                   MOV EAX, DWORD PTR DS:[EBX+0x4]
	$+41     00628A31     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+44     00628A34     C1FA 02                   SAR EDX, 0x2
	$+47     00628A37     6A 01                     PUSH 0x1
	$+49     00628A39     52                        PUSH EDX
	$+4A     00628A3A     8B1C90                    MOV EBX, DWORD PTR DS:[EAX+EDX*4]
	$+4D     00628A3D     E8 DE683D00               CALL PathOfEx.009FF320
	$+52     00628A42     80BB 24080000 00          CMP BYTE PTR DS:[EBX+0x824], 0x0
	$+59     00628A49     74 0B                     JE SHORT PathOfEx.00628A56
	$+5B     00628A4B     8B03                      MOV EAX, DWORD PTR DS:[EBX]
	$+5D     00628A4D     8BCB                      MOV ECX, EBX
	$+5F     00628A4F     6A 01                     PUSH 0x1
	$+61     00628A51     FF50 20                   CALL NEAR DWORD PTR DS:[EAX+0x20]
	$+64     00628A54     EB 0A                     JMP SHORT PathOfEx.00628A60
	$+66     00628A56     C783 5C070000 00000000    MOV DWORD PTR DS:[EBX+0x75C], 0x0
	$+70     00628A60     8B36                      MOV ESI, DWORD PTR DS:[ESI]
	$+72     00628A62     8B5D F0                   MOV EBX, DWORD PTR SS:[EBP-0x10]
	$+75     00628A65     3BF7                      CMP ESI, EDI
	$+77     00628A67   ^ 75 A7                     JNZ SHORT PathOfEx.00628A10
	$+79     00628A69     8B45 E8                   MOV EAX, DWORD PTR SS:[EBP-0x18]                 ; ebp-18处存放的是this
	$+7C     00628A6C     8D98 BC080000             LEA EBX, DWORD PTR DS:[EAX+0x8BC]                ; eax=stCD_MySkillMgr*,eax=this
	$+82     00628A72     8D4B 04                   LEA ECX, DWORD PTR DS:[EBX+0x4]
	$+85     00628A75     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+8C     00628A7C     E8 CF83FAFF               CALL PathOfEx.005D0E50
	$+91     00628A81     6A 08                     PUSH 0x8
	$+93     00628A83     8BCB                      MOV ECX, EBX
	$+95     00628A85     E8 466CE4FF               CALL PathOfEx.0046F6D0
	$+9A     00628A8A     8B45 E8                   MOV EAX, DWORD PTR SS:[EBP-0x18]
	$+9D     00628A8D     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+A4     00628A94     83B8 B4080000 00          CMP DWORD PTR DS:[EAX+0x8B4], 0x0
	$+AB     00628A9B     0F84 1A030000             JE PathOfEx.00628DBB
	$+B1     00628AA1     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+B4     00628AA4     8945 F0                   MOV DWORD PTR SS:[EBP-0x10], EAX
	$+B7     00628AA7     8B77 14                   MOV ESI, DWORD PTR DS:[EDI+0x14]
	$+BA     00628AAA     8B7F 10                   MOV EDI, DWORD PTR DS:[EDI+0x10]
	$+BD     00628AAD     3BFE                      CMP EDI, ESI
	$+BF     00628AAF     74 13                     JE SHORT PathOfEx.00628AC4
	$+C1     00628AB1     57                        PUSH EDI
	$+C2     00628AB2     8D4D F0                   LEA ECX, DWORD PTR SS:[EBP-0x10]
	$+C5     00628AB5     E8 16030000               CALL PathOfEx.00628DD0
	$+CA     00628ABA     83C7 08                   ADD EDI, 0x8
	$+CD     00628ABD     3BFE                      CMP EDI, ESI
	$+CF     00628ABF   ^ 75 F0                     JNZ SHORT PathOfEx.00628AB1
	$+D1     00628AC1     8B45 E8                   MOV EAX, DWORD PTR SS:[EBP-0x18]
	$+D4     00628AC4     8BC8                      MOV ECX, EAX
	$+D6     00628AC6     E8 B5050000               CALL PathOfEx.00629080
	$+DB     00628ACB     68 8C0B0000               PUSH 0xB8C
	$+E0     00628AD0     8945 F0                   MOV DWORD PTR SS:[EBP-0x10], EAX
	$+E3     00628AD3     E8 BB116500               CALL PathOfEx.00C79C93
	$+E8     00628AD8     83C4 04                   ADD ESP, 0x4
	$+EB     00628ADB     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+EE     00628ADE     8BC8                      MOV ECX, EAX
	$+F0     00628AE0     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+F7     00628AE7     E8 C4203F00               CALL PathOfEx.00A1ABB0
	$+FC     00628AEC     51                        PUSH ECX
	$+FD     00628AED     8BF0                      MOV ESI, EAX
	$+FF     00628AEF     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+106    00628AF6     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+109    00628AF9     56                        PUSH ESI
	$+10A    00628AFA     6A 01                     PUSH 0x1
	$+10C    00628AFC     E8 1F393C00               CALL PathOfEx.009EC420
	$+111    00628B01     BA 45030000               MOV EDX, 0x345
	$+116    00628B06     8D4D C0                   LEA ECX, DWORD PTR SS:[EBP-0x40]
	$+119    00628B09     E8 A22E0F00               CALL PathOfEx.0071B9B0
	$+11E    00628B0E     C745 FC 02000000          MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+125    00628B15     8BCE                      MOV ECX, ESI
	$+127    00628B17     8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$+129    00628B19     50                        PUSH EAX
	$+12A    00628B1A     FF92 9C010000             CALL NEAR DWORD PTR DS:[EDX+0x19C]
	$+130    00628B20     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+137    00628B27     8B45 D4                   MOV EAX, DWORD PTR SS:[EBP-0x2C]                 ; d3d9.72F8CD5E
	$+13A    00628B2A     83F8 08                   CMP EAX, 0x8
	$+13D    00628B2D     72 0F                     JB SHORT PathOfEx.00628B3E
	$+13F    00628B2F     40                        INC EAX
	$+140    00628B30     6A 02                     PUSH 0x2
	$+142    00628B32     50                        PUSH EAX
	$+143    00628B33     FF75 C0                   PUSH DWORD PTR SS:[EBP-0x40]
	$+146    00628B36     E8 B53CE4FF               CALL PathOfEx.0046C7F0
	$+14B    00628B3B     83C4 0C                   ADD ESP, 0xC
	$+14E    00628B3E     BA 8893E800               MOV EDX, PathOfEx.00E89388                       ; Art/2DArt/UIImages/InGame/SkillMoveOnly
	$+153    00628B43     8D4D E0                   LEA ECX, DWORD PTR SS:[EBP-0x20]
	$+156    00628B46     E8 F53E3D00               CALL PathOfEx.009FCA40
	$+15B    00628B4B     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4], 0x3
	$+162    00628B52     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+165    00628B55     81C1 40080000             ADD ECX, 0x840
	$+16B    00628B5B     50                        PUSH EAX
	$+16C    00628B5C     8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$+16E    00628B5E     FF52 20                   CALL NEAR DWORD PTR DS:[EDX+0x20]
	$+171    00628B61     8D4D E0                   LEA ECX, DWORD PTR SS:[EBP-0x20]
	$+174    00628B64     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+17B    00628B6B     E8 40F6E5FF               CALL PathOfEx.004881B0
	$+180    00628B70     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+183    00628B73     81C1 40080000             ADD ECX, 0x840
	$+189    00628B79     6A 01                     PUSH 0x1
	$+18B    00628B7B     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+18D    00628B7D     FF50 28                   CALL NEAR DWORD PTR DS:[EAX+0x28]
	$+190    00628B80     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+193    00628B83     66:8B15 C4D00001          MOV DX, WORD PTR DS:[0x100D0C4]
	$+19A    00628B8A     0FB7C2                    MOVZX EAX, DX
	$+19D    00628B8D     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+1A0    00628B90     8B77 14                   MOV ESI, DWORD PTR DS:[EDI+0x14]
	$+1A3    00628B93     8B4F 10                   MOV ECX, DWORD PTR DS:[EDI+0x10]
	$+1A6    00628B96     3BCE                      CMP ECX, ESI
	$+1A8    00628B98     74 19                     JE SHORT PathOfEx.00628BB3
	$+1AA    00628B9A     66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
	$+1B0    00628BA0     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+1B2    00628BA2     66:3950 02                CMP WORD PTR DS:[EAX+0x2], DX
	$+1B6    00628BA6     74 07                     JE SHORT PathOfEx.00628BAF
	$+1B8    00628BA8     83C1 08                   ADD ECX, 0x8
	$+1BB    00628BAB     3BCE                      CMP ECX, ESI
	$+1BD    00628BAD   ^ 75 F1                     JNZ SHORT PathOfEx.00628BA0
	$+1BF    00628BAF     3BCE                      CMP ECX, ESI
	$+1C1    00628BB1     75 04                     JNZ SHORT PathOfEx.00628BB7
	$+1C3    00628BB3     33C0                      XOR EAX, EAX
	$+1C5    00628BB5     EB 05                     JMP SHORT PathOfEx.00628BBC
	$+1C7    00628BB7     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+1C9    00628BB9     0FB700                    MOVZX EAX, WORD PTR DS:[EAX]
	$+1CC    00628BBC     8945 08                   MOV DWORD PTR SS:[EBP+0x8], EAX
	$+1CF    00628BBF     8BD6                      MOV EDX, ESI
	$+1D1    00628BC1     0FB7C0                    MOVZX EAX, AX
	$+1D4    00628BC4     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+1D7    00628BC7     8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+1DA    00628BCA     51                        PUSH ECX
	$+1DB    00628BCB     8B4F 10                   MOV ECX, DWORD PTR DS:[EDI+0x10]
	$+1DE    00628BCE     50                        PUSH EAX
	$+1DF    00628BCF     E8 DC9A1E00               CALL PathOfEx.008126B0
	$+1E4    00628BD4     8BF8                      MOV EDI, EAX
	$+1E6    00628BD6     83C4 08                   ADD ESP, 0x8
	$+1E9    00628BD9     3BFE                      CMP EDI, ESI
	$+1EB    00628BDB     74 0A                     JE SHORT PathOfEx.00628BE7
	$+1ED    00628BDD     8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+1EF    00628BDF     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]
	$+1F2    00628BE2     66:3B08                   CMP CX, WORD PTR DS:[EAX]
	$+1F5    00628BE5     73 02                     JNB SHORT PathOfEx.00628BE9
	$+1F7    00628BE7     8BFE                      MOV EDI, ESI
	$+1F9    00628BE9     8D45 F0                   LEA EAX, DWORD PTR SS:[EBP-0x10]
	$+1FC    00628BEC     8BCB                      MOV ECX, EBX
	$+1FE    00628BEE     50                        PUSH EAX
	$+1FF    00628BEF     8D45 08                   LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+202    00628BF2     50                        PUSH EAX
	$+203    00628BF3     E8 0834E4FF               CALL PathOfEx.0046C000
	$+208    00628BF8     8B45 08                   MOV EAX, DWORD PTR SS:[EBP+0x8]
	$+20B    00628BFB     3B43 04                   CMP EAX, DWORD PTR DS:[EBX+0x4]
	$+20E    00628BFE     75 1A                     JNZ SHORT PathOfEx.00628C1A
	$+210    00628C00     8D45 F0                   LEA EAX, DWORD PTR SS:[EBP-0x10]
	$+213    00628C03     51                        PUSH ECX
	$+214    00628C04     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+217    00628C07     8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+21A    00628C0A     50                        PUSH EAX
	$+21B    00628C0B     51                        PUSH ECX
	$+21C    00628C0C     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
	$+21F    00628C0F     8BCB                      MOV ECX, EBX
	$+221    00628C11     50                        PUSH EAX
	$+222    00628C12     E8 2992FAFF               CALL PathOfEx.005D1E40
	$+227    00628C17     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
	$+22A    00628C1A     8B4F 04                   MOV ECX, DWORD PTR DS:[EDI+0x4]
	$+22D    00628C1D     8B17                      MOV EDX, DWORD PTR DS:[EDI]
	$+22F    00628C1F     85C9                      TEST ECX, ECX
	$+231    00628C21     74 04                     JE SHORT PathOfEx.00628C27
	$+233    00628C23     F0:FF41 04                LOCK INC DWORD PTR DS:[ECX+0x4]
	$+237    00628C27     8B70 10                   MOV ESI, DWORD PTR DS:[EAX+0x10]
	$+23A    00628C2A     8948 10                   MOV DWORD PTR DS:[EAX+0x10], ECX
	$+23D    00628C2D     8950 0C                   MOV DWORD PTR DS:[EAX+0xC], EDX
	$+240    00628C30     83CF FF                   OR EDI, -0x1
	$+243    00628C33     C745 FC 05000000          MOV DWORD PTR SS:[EBP-0x4], 0x5
	$+24A    00628C3A     85F6                      TEST ESI, ESI
	$+24C    00628C3C     74 1F                     JE SHORT PathOfEx.00628C5D
	$+24E    00628C3E     8BC7                      MOV EAX, EDI
	$+250    00628C40     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+255    00628C45     75 16                     JNZ SHORT PathOfEx.00628C5D
	$+257    00628C47     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+259    00628C49     8BCE                      MOV ECX, ESI
	$+25B    00628C4B     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+25D    00628C4D     8BC7                      MOV EAX, EDI
	$+25F    00628C4F     F0:0FC146 08              LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
	$+264    00628C54     75 07                     JNZ SHORT PathOfEx.00628C5D
	$+266    00628C56     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+268    00628C58     8BCE                      MOV ECX, ESI
	$+26A    00628C5A     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+26D    00628C5D     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+270    00628C60     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+277    00628C67     E8 14040000               CALL PathOfEx.00629080
	$+27C    00628C6C     68 8C0B0000               PUSH 0xB8C
	$+281    00628C71     8945 08                   MOV DWORD PTR SS:[EBP+0x8], EAX
	$+284    00628C74     E8 1A106500               CALL PathOfEx.00C79C93
	$+289    00628C79     83C4 04                   ADD ESP, 0x4
	$+28C    00628C7C     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+28F    00628C7F     8BC8                      MOV ECX, EAX
	$+291    00628C81     C745 FC 06000000          MOV DWORD PTR SS:[EBP-0x4], 0x6
	$+298    00628C88     E8 231F3F00               CALL PathOfEx.00A1ABB0
	$+29D    00628C8D     51                        PUSH ECX
	$+29E    00628C8E     8BF0                      MOV ESI, EAX
	$+2A0    00628C90     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+2A7    00628C97     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]
	$+2AA    00628C9A     56                        PUSH ESI
	$+2AB    00628C9B     6A 01                     PUSH 0x1
	$+2AD    00628C9D     E8 7E373C00               CALL PathOfEx.009EC420
	$+2B2    00628CA2     BA 44030000               MOV EDX, 0x344
	$+2B7    00628CA7     8D4D C0                   LEA ECX, DWORD PTR SS:[EBP-0x40]
	$+2BA    00628CAA     E8 012D0F00               CALL PathOfEx.0071B9B0
	$+2BF    00628CAF     C745 FC 07000000          MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+2C6    00628CB6     8BCE                      MOV ECX, ESI
	$+2C8    00628CB8     8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$+2CA    00628CBA     50                        PUSH EAX
	$+2CB    00628CBB     FF92 9C010000             CALL NEAR DWORD PTR DS:[EDX+0x19C]
	$+2D1    00628CC1     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+2D8    00628CC8     8B45 D4                   MOV EAX, DWORD PTR SS:[EBP-0x2C]
	$+2DB    00628CCB     83F8 08                   CMP EAX, 0x8
	$+2DE    00628CCE     72 0F                     JB SHORT PathOfEx.00628CDF
	$+2E0    00628CD0     40                        INC EAX
	$+2E1    00628CD1     6A 02                     PUSH 0x2
	$+2E3    00628CD3     50                        PUSH EAX
	$+2E4    00628CD4     FF75 C0                   PUSH DWORD PTR SS:[EBP-0x40]
	$+2E7    00628CD7     E8 143BE4FF               CALL PathOfEx.0046C7F0
	$+2EC    00628CDC     83C4 0C                   ADD ESP, 0xC
	$+2EF    00628CDF     BA D09DE900               MOV EDX, PathOfEx.00E99DD0                       ; Art/2DArt/UIImages/InGame/SkillClear
	$+2F4    00628CE4     8D4D D8                   LEA ECX, DWORD PTR SS:[EBP-0x28]
	$+2F7    00628CE7     E8 543D3D00               CALL PathOfEx.009FCA40
	$+2FC    00628CEC     C745 FC 08000000          MOV DWORD PTR SS:[EBP-0x4], 0x8
	$+303    00628CF3     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]                  ; PathOfEx.00FF7168
	$+306    00628CF6     81C1 40080000             ADD ECX, 0x840
	$+30C    00628CFC     50                        PUSH EAX
	$+30D    00628CFD     8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$+30F    00628CFF     FF52 20                   CALL NEAR DWORD PTR DS:[EDX+0x20]
	$+312    00628D02     8D4D D8                   LEA ECX, DWORD PTR SS:[EBP-0x28]
	$+315    00628D05     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+31C    00628D0C     E8 9FF4E5FF               CALL PathOfEx.004881B0
	$+321    00628D11     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]                  ; PathOfEx.00FF7168
	$+324    00628D14     81C1 40080000             ADD ECX, 0x840
	$+32A    00628D1A     6A 01                     PUSH 0x1
	$+32C    00628D1C     8B01                      MOV EAX, DWORD PTR DS:[ECX]
	$+32E    00628D1E     FF50 28                   CALL NEAR DWORD PTR DS:[EAX+0x28]
	$+331    00628D21     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+338    00628D28     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10], 0x0
	$+33F    00628D2F     8D45 08                   LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+342    00628D32     C745 FC 09000000          MOV DWORD PTR SS:[EBP-0x4], 0x9
	$+349    00628D39     50                        PUSH EAX
	$+34A    00628D3A     8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+34D    00628D3D     8BCB                      MOV ECX, EBX
	$+34F    00628D3F     50                        PUSH EAX
	$+350    00628D40     E8 BB32E4FF               CALL PathOfEx.0046C000
	$+355    00628D45     8B45 E4                   MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+358    00628D48     3B43 04                   CMP EAX, DWORD PTR DS:[EBX+0x4]
	$+35B    00628D4B     75 1A                     JNZ SHORT PathOfEx.00628D67
	$+35D    00628D4D     8D45 08                   LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+360    00628D50     51                        PUSH ECX
	$+361    00628D51     8945 E4                   MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+364    00628D54     8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+367    00628D57     50                        PUSH EAX
	$+368    00628D58     51                        PUSH ECX
	$+369    00628D59     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
	$+36C    00628D5C     8BCB                      MOV ECX, EBX
	$+36E    00628D5E     50                        PUSH EAX
	$+36F    00628D5F     E8 DC90FAFF               CALL PathOfEx.005D1E40                           ; 改写hash大小
	$+374    00628D64     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
	$+377    00628D67     8B70 10                   MOV ESI, DWORD PTR DS:[EAX+0x10]
	$+37A    00628D6A     C740 10 00000000          MOV DWORD PTR DS:[EAX+0x10], 0x0
	$+381    00628D71     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+388    00628D78     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10], 0x0
	$+38F    00628D7F     C740 0C 00000000          MOV DWORD PTR DS:[EAX+0xC], 0x0
	$+396    00628D86     C645 FC 0A                MOV BYTE PTR SS:[EBP-0x4], 0xA
	$+39A    00628D8A     85F6                      TEST ESI, ESI
	$+39C    00628D8C     74 1E                     JE SHORT PathOfEx.00628DAC
	$+39E    00628D8E     8BC7                      MOV EAX, EDI
	$+3A0    00628D90     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+3A5    00628D95     75 15                     JNZ SHORT PathOfEx.00628DAC
	$+3A7    00628D97     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+3A9    00628D99     8BCE                      MOV ECX, ESI
	$+3AB    00628D9B     FF10                      CALL NEAR DWORD PTR DS:[EAX]
	$+3AD    00628D9D     F0:0FC17E 08              LOCK XADD DWORD PTR DS:[ESI+0x8], EDI
	$+3B2    00628DA2     4F                        DEC EDI
	$+3B3    00628DA3     75 07                     JNZ SHORT PathOfEx.00628DAC
	$+3B5    00628DA5     8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$+3B7    00628DA7     8BCE                      MOV ECX, ESI
	$+3B9    00628DA9     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+3BC    00628DAC     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+3BF    00628DAF     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+3C6    00628DB6     E8 85030000               CALL PathOfEx.00629140
	$+3CB    00628DBB     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+3CE    00628DBE     5F                        POP EDI                                          ; 0018ED08
	$+3CF    00628DBF     5E                        POP ESI                                          ; 003B296C
	$+3D0    00628DC0     5B                        POP EBX                                          ; 0018ED08
	$+3D1    00628DC1     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
	$+3D8    00628DC8     8BE5                      MOV ESP, EBP
	$+3DA    00628DCA     5D                        POP EBP                                          ; 0018ED08
	$+3DB    00628DCB     C2 0400                   RETN 0x4
	$+3DE    00628DCE     CC                        INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??????????8b??????????89????8b??3b??74??8d", -0x30);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *(pt_byte*)addr_tmp;
	if (dw_tmp != 0x81)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_my_skill_mgr_base_fn_tag_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	int_tmp = *(pt_int*)((pt_byte*)addr_tmp + 2);
	assert(int_tmp > -0x1000 && int_tmp < -0x600);
	ofs << "pt_cdword	g_o_get_my_skill_mgr_ = " << int_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
