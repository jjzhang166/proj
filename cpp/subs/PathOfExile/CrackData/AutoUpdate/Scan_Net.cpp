#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Net(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	pt_dword dw_tmp;
	/*
	//用这个函数来定位发包call
	$-10D    008D35D0      55                      PUSH EBP
	$-10C    008D35D1      8BEC                    MOV EBP, ESP
	$-10A    008D35D3      64:A1 00000000          MOV EAX, DWORD PTR FS:[0]
	$-104    008D35D9      6A FF                   PUSH -0x1
	$-102    008D35DB      68 00BED500             PUSH PathOfEx.00D5BE00
	$-FD     008D35E0      50                      PUSH EAX
	$-FC     008D35E1      64:8925 00000000        MOV DWORD PTR FS:[0], ESP
	$-F5     008D35E8      83EC 18                 SUB ESP, 0x18
	$-F2     008D35EB      57                      PUSH EDI
	$-F1     008D35EC      8BF9                    MOV EDI, ECX
	$-EF     008D35EE      8B87 883A0000           MOV EAX, DWORD PTR DS:[EDI+0x3A88]
	$-E9     008D35F4      83B8 E8010000 02        CMP DWORD PTR DS:[EAX+0x1E8], 0x2
	$-E2     008D35FB      0F84 F1000000           JE PathOfEx.008D36F2
	$-DC     008D3601      56                      PUSH ESI
	$-DB     008D3602      8B75 08                 MOV ESI, DWORD PTR SS:[EBP+0x8]
	$-D8     008D3605      8BCE                    MOV ECX, ESI
	$-D6     008D3607      E8 34B02000             CALL PathOfEx.00ADE640
	$-D1     008D360C      B9 00800000             MOV ECX, 0x8000
	$-CC     008D3611      66:85C1                 TEST CX, AX
	$-C9     008D3614      0F85 D7000000           JNZ PathOfEx.008D36F1
	$-C3     008D361A      837E 1C 00              CMP DWORD PTR DS:[ESI+0x1C], 0x0
	$-BF     008D361E      74 41                   JE SHORT PathOfEx.008D3661
	$-BD     008D3620      B8 15000000             MOV EAX, 0x15
	$-B8     008D3625      C745 E0 080BEF00        MOV DWORD PTR SS:[EBP-0x20], PathOfEx.00EF0B08
	$-B1     008D362C      66:8945 E4              MOV WORD PTR SS:[EBP-0x1C], AX
	$-AD     008D3630      8BCE                    MOV ECX, ESI
	$-AB     008D3632      C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-A4     008D3639      E8 72B22000             CALL PathOfEx.00ADE8B0
	$-9F     008D363E      8BCE                    MOV ECX, ESI
	$-9D     008D3640      66:8945 EC              MOV WORD PTR SS:[EBP-0x14], AX
	$-99     008D3644      E8 F7AF2000             CALL PathOfEx.00ADE640
	$-94     008D3649      66:8945 EE              MOV WORD PTR SS:[EBP-0x12], AX
	$-90     008D364D      8A46 17                 MOV AL, BYTE PTR DS:[ESI+0x17]
	$-8D     008D3650      8845 F0                 MOV BYTE PTR SS:[EBP-0x10], AL
	$-8A     008D3653      8B46 1C                 MOV EAX, DWORD PTR DS:[ESI+0x1C]
	$-87     008D3656      8B40 14                 MOV EAX, DWORD PTR DS:[EAX+0x14]
	$-84     008D3659      8945 E8                 MOV DWORD PTR SS:[EBP-0x18], EAX
	$-81     008D365C      8D45 E0                 LEA EAX, DWORD PTR SS:[EBP-0x20]
	$-7E     008D365F      EB 70                   JMP SHORT PathOfEx.008D36D1
	$-7C     008D3661      F3:0F1087 A43C0000      MOVSS XMM0, DWORD PTR DS:[EDI+0x3CA4]
	$-74     008D3669      F3:0F5905 2069E600      MULSS XMM0, DWORD PTR DS:[0xE66920]
	$-6C     008D3671      C745 DC 140BEF00        MOV DWORD PTR SS:[EBP-0x24], PathOfEx.00EF0B14
	$-65     008D3678      F3:0F2CC8               CVTTSS2SI ECX, XMM0
	$-61     008D367C      F3:0F1087 A03C0000      MOVSS XMM0, DWORD PTR DS:[EDI+0x3CA0]
	$-59     008D3684      F3:0F5905 2069E600      MULSS XMM0, DWORD PTR DS:[0xE66920]
	$-51     008D368C      894E 24                 MOV DWORD PTR DS:[ESI+0x24], ECX
	$-4E     008D368F      F3:0F2CC0               CVTTSS2SI EAX, XMM0
	$-4A     008D3693      8946 20                 MOV DWORD PTR DS:[ESI+0x20], EAX
	$-47     008D3696      B8 16000000             MOV EAX, 0x16
	$-42     008D369B      66:8945 E0              MOV WORD PTR SS:[EBP-0x20], AX
	$-3E     008D369F      8BCE                    MOV ECX, ESI
	$-3C     008D36A1      C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-35     008D36A8      E8 03B22000             CALL PathOfEx.00ADE8B0
	$-30     008D36AD      8BCE                    MOV ECX, ESI
	$-2E     008D36AF      66:8945 EC              MOV WORD PTR SS:[EBP-0x14], AX
	$-2A     008D36B3      E8 88AF2000             CALL PathOfEx.00ADE640
	$-25     008D36B8      66:8945 EE              MOV WORD PTR SS:[EBP-0x12], AX
	$-21     008D36BC      8A46 17                 MOV AL, BYTE PTR DS:[ESI+0x17]
	$-1E     008D36BF      8845 F0                 MOV BYTE PTR SS:[EBP-0x10], AL
	$-1B     008D36C2      8B46 20                 MOV EAX, DWORD PTR DS:[ESI+0x20]
	$-18     008D36C5      8945 E4                 MOV DWORD PTR SS:[EBP-0x1C], EAX
	$-15     008D36C8      8B46 24                 MOV EAX, DWORD PTR DS:[ESI+0x24]
	$-12     008D36CB      8945 E8                 MOV DWORD PTR SS:[EBP-0x18], EAX
	$-F      008D36CE      8D45 DC                 LEA EAX, DWORD PTR SS:[EBP-0x24]
	$-C      008D36D1      50                      PUSH EAX
	$-B      008D36D2      FFB7 8C3A0000           PUSH DWORD PTR DS:[EDI+0x3A8C]
	$-5      008D36D8      E8 33DEDDFF             CALL PathOfEx.006B1510
	$ ==>    008D36DD      8BC8                    MOV ECX, EAX
	$+2      008D36DF      E8 FC110100             CALL PathOfEx.008E48E0
	$+7      008D36E4      8B8F 8C3A0000           MOV ECX, DWORD PTR DS:[EDI+0x3A8C]
	$+D      008D36EA      6A 00                   PUSH 0x0
	$+F      008D36EC      E8 4FA2EBFF             CALL PathOfEx.0078D940                                 ; 调用发包call
	$+14     008D36F1      5E                      POP ESI                                                ; 0018EEF0
	$+15     008D36F2      8B4D F4                 MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+18     008D36F5      5F                      POP EDI                                                ; 0018EEF0
	$+19     008D36F6      64:890D 00000000        MOV DWORD PTR FS:[0], ECX
	$+20     008D36FD      8BE5                    MOV ESP, EBP
	$+22     008D36FF      5D                      POP EBP                                                ; 0018EEF0
	$+23     008D3700      C2 0400                 RETN 0x4

	//这个函数也可以定位发包call,feature code：83EC28B80A000000，向下偏移0x68
	$-18     008DE110      55                             PUSH EBP
	$-17     008DE111      8BEC                           MOV EBP, ESP
	$-15     008DE113      6A FF                          PUSH -0x1
	$-13     008DE115      68 48BDD500                    PUSH PathOfEx.00D5BD48
	$-E      008DE11A      64:A1 00000000                 MOV EAX, DWORD PTR FS:[0]
	$-8      008DE120      50                             PUSH EAX
	$-7      008DE121      64:8925 00000000               MOV DWORD PTR FS:[0], ESP
	$ ==>    008DE128      83EC 28                        SUB ESP, 0x28
	$+3      008DE12B      B8 0A000000                    MOV EAX, 0xA
	$+8      008DE130      C745 D0 300AEF00               MOV DWORD PTR SS:[EBP-0x30], PathOfEx.00EF0A30
	$+F      008DE137      56                             PUSH ESI
	$+10     008DE138      66:8945 D4                     MOV WORD PTR SS:[EBP-0x2C], AX
	$+14     008DE13C      33C0                           XOR EAX, EAX
	$+16     008DE13E      57                             PUSH EDI
	$+17     008DE13F      8BF9                           MOV EDI, ECX
	$+19     008DE141      C745 EC 07000000               MOV DWORD PTR SS:[EBP-0x14], 0x7
	$+20     008DE148      C745 E8 00000000               MOV DWORD PTR SS:[EBP-0x18], 0x0
	$+27     008DE14F      66:8945 D8                     MOV WORD PTR SS:[EBP-0x28], AX
	$+2B     008DE153      8B75 08                        MOV ESI, DWORD PTR SS:[EBP+0x8]
	$+2E     008DE156      8945 FC                        MOV DWORD PTR SS:[EBP-0x4], EAX
	$+31     008DE159      8D45 D8                        LEA EAX, DWORD PTR SS:[EBP-0x28]
	$+34     008DE15C      3BC6                           CMP EAX, ESI
	$+36     008DE15E      74 0C                          JE SHORT PathOfEx.008DE16C
	$+38     008DE160      6A FF                          PUSH -0x1
	$+3A     008DE162      6A 00                          PUSH 0x0
	$+3C     008DE164      56                             PUSH ESI
	$+3D     008DE165      8BC8                           MOV ECX, EAX
	$+3F     008DE167      E8 2457B8FF                    CALL PathOfEx.00463890
	$+44     008DE16C      E8 9F60E4FF                    CALL PathOfEx.00724210
	$+49     008DE171      A0 50D7FC00                    MOV AL, BYTE PTR DS:[0xFCD750]
	$+4E     008DE176      8D4D D0                        LEA ECX, DWORD PTR SS:[EBP-0x30]
	$+51     008DE179      FFB7 28080000                  PUSH DWORD PTR DS:[EDI+0x828]
	$+57     008DE17F      8845 F0                        MOV BYTE PTR SS:[EBP-0x10], AL
	$+5A     008DE182      E8 F97FFEFF                    CALL PathOfEx.008C6180
	$+5F     008DE187      8B8F 28080000                  MOV ECX, DWORD PTR DS:[EDI+0x828]
	$+65     008DE18D      6A 01                          PUSH 0x1
	$+67     008DE18F      E8 ACF7EAFF                    CALL PathOfEx.0078D940                               ; call发包
	$+6C     008DE194      8D8F 38080000                  LEA ECX, DWORD PTR DS:[EDI+0x838]
	$+72     008DE19A      3BCE                           CMP ECX, ESI
	$+74     008DE19C      74 0A                          JE SHORT PathOfEx.008DE1A8
	$+76     008DE19E      6A FF                          PUSH -0x1
	$+78     008DE1A0      6A 00                          PUSH 0x0
	$+7A     008DE1A2      56                             PUSH ESI
	$+7B     008DE1A3      E8 E856B8FF                    CALL PathOfEx.00463890
	$+80     008DE1A8      8B45 EC                        MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+83     008DE1AB      83F8 08                        CMP EAX, 0x8
	$+86     008DE1AE      72 0F                          JB SHORT PathOfEx.008DE1BF
	$+88     008DE1B0      6A 02                          PUSH 0x2
	$+8A     008DE1B2      40                             INC EAX
	$+8B     008DE1B3      50                             PUSH EAX
	$+8C     008DE1B4      FF75 D8                        PUSH DWORD PTR SS:[EBP-0x28]
	$+8F     008DE1B7      E8 5446B8FF                    CALL PathOfEx.00462810
	$+94     008DE1BC      83C4 0C                        ADD ESP, 0xC
	$+97     008DE1BF      8B4D F4                        MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+9A     008DE1C2      5F                             POP EDI                                              ; 0018E950
	$+9B     008DE1C3      64:890D 00000000               MOV DWORD PTR FS:[0], ECX
	$+A2     008DE1CA      5E                             POP ESI                                              ; 0018E950
	$+A3     008DE1CB      8BE5                           MOV ESP, EBP
	$+A5     008DE1CD      5D                             POP EBP                                              ; 0018E950
	$+A6     008DE1CE      C2 0400                        RETN 0x4


	//下面是发包call
	$ ==>    0078D940      55                      PUSH EBP                                               ; 发包
	$+1      0078D941      8D6C24 C0               LEA EBP, DWORD PTR SS:[ESP-0x40]
	$+5      0078D945      83EC 40                 SUB ESP, 0x40
	$+8      0078D948      6A FF                   PUSH -0x1
	$+A      0078D94A      68 1B8ED400             PUSH PathOfEx.00D48E1B                                 ; ASCII "葛怊"
	$+F      0078D94F      64:A1 00000000          MOV EAX, DWORD PTR FS:[0]
	$+15     0078D955      50                      PUSH EAX
	$+16     0078D956      64:8925 00000000        MOV DWORD PTR FS:[0], ESP
	$+1D     0078D95D      81EC 64020000           SUB ESP, 0x264
	$+23     0078D963      53                      PUSH EBX
	$+24     0078D964      56                      PUSH ESI
	$+25     0078D965      8BF1                    MOV ESI, ECX
	$+27     0078D967      C745 3C 00000000        MOV DWORD PTR SS:[EBP+0x3C], 0x0
	$+2E     0078D96E      57                      PUSH EDI
	$+2F     0078D96F      8BBE B4000000           MOV EDI, DWORD PTR DS:[ESI+0xB4]                       ; PathOfEx.00630074
	$+35     0078D975      8B9E B8000000           MOV EBX, DWORD PTR DS:[ESI+0xB8]                       ; PathOfEx.0073006E
	$+3B     0078D97B      2BFB                    SUB EDI, EBX
	$+3D     0078D97D      8B8E 90000000           MOV ECX, DWORD PTR DS:[ESI+0x90]                       ; PathOfEx.0065006D
	$+43     0078D983      85C9                    TEST ECX, ECX
	$+45     0078D985      0F95C0                  SETNE AL
	$+48     0078D988      84C0                    TEST AL, AL
	$+4A     0078D98A      74 13                   JE SHORT PathOfEx.0078D99F
	$+4C     0078D98C      85FF                    TEST EDI, EDI
	$+4E     0078D98E      74 0F                   JE SHORT PathOfEx.0078D99F
	$+50     0078D990      8B86 C8000000           MOV EAX, DWORD PTR DS:[ESI+0xC8]                       ; PathOf_1.0FC0B3FE
	$+56     0078D996      8B11                    MOV EDX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+58     0078D998      03C3                    ADD EAX, EBX
	$+5A     0078D99A      57                      PUSH EDI
	$+5B     0078D99B      50                      PUSH EAX
	$+5C     0078D99C      FF52 04                 CALL NEAR DWORD PTR DS:[EDX+0x4]                       ; PathOfEx.009C0BD0
	$+5F     0078D99F      8B86 B4000000           MOV EAX, DWORD PTR DS:[ESI+0xB4]                       ; PathOfEx.00630074
	$+65     0078D9A5      33FF                    XOR EDI, EDI
	$+67     0078D9A7      8986 B8000000           MOV DWORD PTR DS:[ESI+0xB8], EAX
	$+6D     0078D9AD      85C0                    TEST EAX, EAX
	$+6F     0078D9AF      74 43                   JE SHORT PathOfEx.0078D9F4
	$+71     0078D9B1      8B1D 2855D800           MOV EBX, DWORD PTR DS:[0xD85528]
	$+77     0078D9B7      66:0F1F8400 00000000    NOP WORD PTR DS:[EAX+EAX]
	$+80     0078D9C0      8B8E C8000000           MOV ECX, DWORD PTR DS:[ESI+0xC8]                       ; PathOf_1.0FC0B3FE
	$+86     0078D9C6      8B86 B4000000           MOV EAX, DWORD PTR DS:[ESI+0xB4]                       ; PathOfEx.00630074
	$+8C     0078D9CC      6A 00                   PUSH 0x0
	$+8E     0078D9CE      2BC7                    SUB EAX, EDI
	$+90     0078D9D0      50                      PUSH EAX
	$+91     0078D9D1      8D0439                  LEA EAX, DWORD PTR DS:[ECX+EDI]
	$+94     0078D9D4      50                      PUSH EAX
	$+95     0078D9D5      FF36                    PUSH DWORD PTR DS:[ESI]                                ; PathOfEx.00E6AE58
	$+97     0078D9D7      FFD3                    CALL NEAR EBX
	$+99     0078D9D9      83F8 FF                 CMP EAX, -0x1
	$+9C     0078D9DC      0F84 85000000           JE PathOfEx.0078DA67
	$+A2     0078D9E2      8B8E 8C000000           MOV ECX, DWORD PTR DS:[ESI+0x8C]                       ; PathOfEx.006E006F
	$+A8     0078D9E8      03F8                    ADD EDI, EAX
	$+AA     0078D9EA      0101                    ADD DWORD PTR DS:[ECX], EAX
	$+AC     0078D9EC      3BBE B4000000           CMP EDI, DWORD PTR DS:[ESI+0xB4]                       ; PathOfEx.00630074
	$+B2     0078D9F2    ^ 72 CC                   JB SHORT PathOfEx.0078D9C0
	$+B4     0078D9F4      C786 B4000000 00000000  MOV DWORD PTR DS:[ESI+0xB4], 0x0
	$+BE     0078D9FE      C786 B8000000 00000000  MOV DWORD PTR DS:[ESI+0xB8], 0x0
	$+C8     0078DA08      837E 54 00              CMP DWORD PTR DS:[ESI+0x54], 0x0
	$+CC     0078DA0C      0F94C0                  SETE AL
	$+CF     0078DA0F      84C0                    TEST AL, AL
	$+D1     0078DA11      0F94C0                  SETE AL
	$+D4     0078DA14      84C0                    TEST AL, AL
	$+D6     0078DA16      74 39                   JE SHORT PathOfEx.0078DA51
	$+D8     0078DA18      C745 24 00000000        MOV DWORD PTR SS:[EBP+0x24], 0x0
	$+DF     0078DA1F      8D45 00                 LEA EAX, DWORD PTR SS:[EBP]
	$+E2     0078DA22      C745 FC 0A000000        MOV DWORD PTR SS:[EBP-0x4], 0xA
	$+E9     0078DA29      50                      PUSH EAX
	$+EA     0078DA2A      8D4E 30                 LEA ECX, DWORD PTR DS:[ESI+0x30]
	$+ED     0078DA2D      E8 9E060000             CALL PathOfEx.0078E0D0
	$+F2     0078DA32      C745 FC 0B000000        MOV DWORD PTR SS:[EBP-0x4], 0xB
	$+F9     0078DA39      8B4D 24                 MOV ECX, DWORD PTR SS:[EBP+0x24]
	$+FC     0078DA3C      85C9                    TEST ECX, ECX
	$+FE     0078DA3E      74 11                   JE SHORT PathOfEx.0078DA51
	$+100    0078DA40      8B11                    MOV EDX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+102    0078DA42      8D45 00                 LEA EAX, DWORD PTR SS:[EBP]
	$+105    0078DA45      3BC8                    CMP ECX, EAX
	$+107    0078DA47      0F95C0                  SETNE AL
	$+10A    0078DA4A      0FB6C0                  MOVZX EAX, AL
	$+10D    0078DA4D      50                      PUSH EAX
	$+10E    0078DA4E      FF52 10                 CALL NEAR DWORD PTR DS:[EDX+0x10]                      ; PathOfEx.006EE770
	$+111    0078DA51      5F                      POP EDI
	$+112    0078DA52      8BC6                    MOV EAX, ESI
	$+114    0078DA54      5E                      POP ESI
	$+115    0078DA55      5B                      POP EBX
	$+116    0078DA56      8B4D F4                 MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+119    0078DA59      64:890D 00000000        MOV DWORD PTR FS:[0], ECX
	$+120    0078DA60      8D65 40                 LEA ESP, DWORD PTR SS:[EBP+0x40]
	$+123    0078DA63      5D                      POP EBP
	$+124    0078DA64      C2 0400                 RETN 0x4
	$+127    0078DA67      8B1D 1455D800           MOV EBX, DWORD PTR DS:[0xD85514]
	$+12D    0078DA6D      FFD3                    CALL NEAR EBX
	$+12F    0078DA6F      3D 33270000             CMP EAX, 0x2733
	$+134    0078DA74      0F85 45030000           JNZ PathOfEx.0078DDBF
	$+13A    0078DA7A      85FF                    TEST EDI, EDI
	$+13C    0078DA7C      74 28                   JE SHORT PathOfEx.0078DAA6
	$+13E    0078DA7E      8B8E C8000000           MOV ECX, DWORD PTR DS:[ESI+0xC8]                       ; PathOf_1.0FC0B3FE
	$+144    0078DA84      8B86 B4000000           MOV EAX, DWORD PTR DS:[ESI+0xB4]                       ; PathOfEx.00630074
	$+14A    0078DA8A      2BC7                    SUB EAX, EDI
	$+14C    0078DA8C      50                      PUSH EAX
	$+14D    0078DA8D      8D0439                  LEA EAX, DWORD PTR DS:[ECX+EDI]
	$+150    0078DA90      50                      PUSH EAX
	$+151    0078DA91      51                      PUSH ECX
	$+152    0078DA92      E8 99D04F00             CALL PathOfEx.00C8AB30
	$+157    0078DA97      29BE B4000000           SUB DWORD PTR DS:[ESI+0xB4], EDI
	$+15D    0078DA9D      83C4 0C                 ADD ESP, 0xC
	$+160    0078DAA0      29BE B8000000           SUB DWORD PTR DS:[ESI+0xB8], EDI
	$+166    0078DAA6      8B7D 48                 MOV EDI, DWORD PTR SS:[EBP+0x48]
	$+169    0078DAA9      85FF                    TEST EDI, EDI
	$+16B    0078DAAB      0F8E E3020000           JLE PathOfEx.0078DD94
	$+171    0078DAB1      80BE B3000000 00        CMP BYTE PTR DS:[ESI+0xB3], 0x0
	$+178    0078DAB8      74 17                   JE SHORT PathOfEx.0078DAD1
	$+17A    0078DABA      56                      PUSH ESI
	$+17B    0078DABB      8D4D 30                 LEA ECX, DWORD PTR SS:[EBP+0x30]
	$+17E    0078DABE      E8 FDF2FFFF             CALL PathOfEx.0078CDC0
	$+183    0078DAC3      68 1C5CFC00             PUSH PathOfEx.00FC5C1C
	$+188    0078DAC8      8D45 30                 LEA EAX, DWORD PTR SS:[EBP+0x30]
	$+18B    0078DACB      50                      PUSH EAX
	$+18C    0078DACC      E8 A2E64F00             CALL PathOfEx.00C8C173
	$+191    0078DAD1      8B06                    MOV EAX, DWORD PTR DS:[ESI]                            ; PathOfEx.00E6AE58
	$+193    0078DAD3      8985 94FDFFFF           MOV DWORD PTR SS:[EBP-0x26C], EAX
	$+199    0078DAD9      C785 90FDFFFF 01000000  MOV DWORD PTR SS:[EBP-0x270], 0x1
	$+1A3    0078DAE3      897D 28                 MOV DWORD PTR SS:[EBP+0x28], EDI
	$+1A6    0078DAE6      8D48 01                 LEA ECX, DWORD PTR DS:[EAX+0x1]
	$+1A9    0078DAE9      C745 2C 00000000        MOV DWORD PTR SS:[EBP+0x2C], 0x0
	$+1B0    0078DAF0      8D45 28                 LEA EAX, DWORD PTR SS:[EBP+0x28]
	$+1B3    0078DAF3      50                      PUSH EAX
	$+1B4    0078DAF4      6A 00                   PUSH 0x0
	$+1B6    0078DAF6      8D85 90FDFFFF           LEA EAX, DWORD PTR SS:[EBP-0x270]
	$+1BC    0078DAFC      50                      PUSH EAX
	$+1BD    0078DAFD      6A 00                   PUSH 0x0
	$+1BF    0078DAFF      51                      PUSH ECX
	$+1C0    0078DB00      FF15 1C55D800           CALL NEAR DWORD PTR DS:[0xD8551C]
	$+1C6    0078DB06      83F8 FF                 CMP EAX, -0x1
	$+1C9    0078DB09      0F85 2A010000           JNZ PathOfEx.0078DC39
	$+1CF    0078DB0F      FFD3                    CALL NEAR EBX
	$+1D1    0078DB11      3D 14270000             CMP EAX, 0x2714
	$+1D6    0078DB16      0F84 5C020000           JE PathOfEx.0078DD78
	$+1DC    0078DB1C      C785 44FFFFFF 7412EE00  MOV DWORD PTR SS:[EBP-0xBC], PathOfEx.00EE1274
	$+1E6    0078DB26      C785 54FFFFFF AC58EA00  MOV DWORD PTR SS:[EBP-0xAC], PathOfEx.00EA58AC
	$+1F0    0078DB30      C745 AC 24C5DD00        MOV DWORD PTR SS:[EBP-0x54], PathOfEx.00DDC524         ; ASCII "悫F"
	$+1F7    0078DB37      51                      PUSH ECX
	$+1F8    0078DB38      8D85 5CFFFFFF           LEA EAX, DWORD PTR SS:[EBP-0xA4]
	$+1FE    0078DB3E      C745 FC 05000000        MOV DWORD PTR SS:[EBP-0x4], 0x5
	$+205    0078DB45      50                      PUSH EAX
	$+206    0078DB46      8D8D 44FFFFFF           LEA ECX, DWORD PTR SS:[EBP-0xBC]
	$+20C    0078DB4C      C745 3C 02000000        MOV DWORD PTR SS:[EBP+0x3C], 0x2
	$+213    0078DB53      E8 3808CEFF             CALL PathOfEx.0046E390
	$+218    0078DB58      C745 FC 06000000        MOV DWORD PTR SS:[EBP-0x4], 0x6
	$+21F    0078DB5F      8B85 44FFFFFF           MOV EAX, DWORD PTR SS:[EBP-0xBC]
	$+225    0078DB65      6A 03                   PUSH 0x3
	$+227    0078DB67      8B40 04                 MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+22A    0078DB6A      C78405 44FFFFFF B86DE60>MOV DWORD PTR SS:[EBP+EAX-0xBC], PathOfEx.00E66DB8     ; ASCII "蹒F"
	$+235    0078DB75      8B85 44FFFFFF           MOV EAX, DWORD PTR SS:[EBP-0xBC]
	$+23B    0078DB7B      8B48 04                 MOV ECX, DWORD PTR DS:[EAX+0x4]
	$+23E    0078DB7E      8D41 98                 LEA EAX, DWORD PTR DS:[ECX-0x68]
	$+241    0078DB81      89840D 40FFFFFF         MOV DWORD PTR SS:[EBP+ECX-0xC0], EAX
	$+248    0078DB88      8D8D 5CFFFFFF           LEA ECX, DWORD PTR SS:[EBP-0xA4]
	$+24E    0078DB8E      E8 DD0DCEFF             CALL PathOfEx.0046E970
	$+253    0078DB93      C745 FC 07000000        MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+25A    0078DB9A      FFD3                    CALL NEAR EBX
	$+25C    0078DB9C      50                      PUSH EAX
	$+25D    0078DB9D      8D85 54FFFFFF           LEA EAX, DWORD PTR SS:[EBP-0xAC]
	$+263    0078DBA3      68 D811ED00             PUSH PathOfEx.00ED11D8                                 ; ASCII "Select returned a fatal error. Error Code: "
	$+268    0078DBA8      50                      PUSH EAX
	$+269    0078DBA9      E8 B28ED9FF             CALL PathOfEx.00526A60
	$+26E    0078DBAE      83C4 08                 ADD ESP, 0x8
	$+271    0078DBB1      8BC8                    MOV ECX, EAX
	$+273    0078DBB3      E8 48F9CEFF             CALL PathOfEx.0047D500
	$+278    0078DBB8      8D45 10                 LEA EAX, DWORD PTR SS:[EBP+0x10]
	$+27B    0078DBBB      50                      PUSH EAX
	$+27C    0078DBBC      8D8D 44FFFFFF           LEA ECX, DWORD PTR SS:[EBP-0xBC]
	$+282    0078DBC2      E8 D9FECEFF             CALL PathOfEx.0047DAA0
	$+287    0078DBC7      8BF8                    MOV EDI, EAX
	$+289    0078DBC9      C645 FC 08              MOV BYTE PTR SS:[EBP-0x4], 0x8
	$+28D    0078DBCD      FF15 CC50D800           CALL NEAR DWORD PTR DS:[0xD850CC]                      ; kernel32.GetTickCount
	$+293    0078DBD3      8BD8                    MOV EBX, EAX
	$+295    0078DBD5      8D45 38                 LEA EAX, DWORD PTR SS:[EBP+0x38]
	$+298    0078DBD8      50                      PUSH EAX
	$+299    0078DBD9      E8 D9345100             CALL PathOfEx.00CA10B7
	$+29E    0078DBDE      64:8B0D 2C000000        MOV ECX, DWORD PTR FS:[0x2C]
	$+2A5    0078DBE5      83C4 04                 ADD ESP, 0x4
	$+2A8    0078DBE8      8B09                    MOV ECX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+2AA    0078DBEA      57                      PUSH EDI
	$+2AB    0078DBEB      83EC 0C                 SUB ESP, 0xC
	$+2AE    0078DBEE      8B49 18                 MOV ECX, DWORD PTR DS:[ECX+0x18]                       ; PathOfEx.00720069
	$+2B1    0078DBF1      6A 02                   PUSH 0x2
	$+2B3    0078DBF3      53                      PUSH EBX
	$+2B4    0078DBF4      FF75 3C                 PUSH DWORD PTR SS:[EBP+0x3C]
	$+2B7    0078DBF7      FF75 38                 PUSH DWORD PTR SS:[EBP+0x38]
	$+2BA    0078DBFA      68 66020000             PUSH 0x266
	$+2BF    0078DBFF      E8 DCFACEFF             CALL PathOfEx.0047D6E0
	$+2C4    0078DC04      8B45 24                 MOV EAX, DWORD PTR SS:[EBP+0x24]
	$+2C7    0078DC07      83F8 08                 CMP EAX, 0x8
	$+2CA    0078DC0A      72 0F                   JB SHORT PathOfEx.0078DC1B
	$+2CC    0078DC0C      40                      INC EAX
	$+2CD    0078DC0D      6A 02                   PUSH 0x2
	$+2CF    0078DC0F      50                      PUSH EAX
	$+2D0    0078DC10      FF75 10                 PUSH DWORD PTR SS:[EBP+0x10]
	$+2D3    0078DC13      E8 F84BCDFF             CALL PathOfEx.00462810
	$+2D8    0078DC18      83C4 0C                 ADD ESP, 0xC
	$+2DB    0078DC1B      8D4D AC                 LEA ECX, DWORD PTR SS:[EBP-0x54]
	$+2DE    0078DC1E      E8 7DF0CDFF             CALL PathOfEx.0046CCA0
	$+2E3    0078DC23      C745 FC 09000000        MOV DWORD PTR SS:[EBP-0x4], 0x9
	$+2EA    0078DC2A      8D45 AC                 LEA EAX, DWORD PTR SS:[EBP-0x54]
	$+2ED    0078DC2D      C745 AC 9CC4DD00        MOV DWORD PTR SS:[EBP-0x54], PathOfEx.00DDC49C         ; ASCII "疴F"
	$+2F4    0078DC34      E9 1E010000             JMP PathOfEx.0078DD57
	$+2F9    0078DC39      85C0                    TEST EAX, EAX
	$+2FB    0078DC3B      0F85 37010000           JNZ PathOfEx.0078DD78
	$+301    0078DC41      C785 94FEFFFF 7412EE00  MOV DWORD PTR SS:[EBP-0x16C], PathOfEx.00EE1274
	$+30B    0078DC4B      C785 A4FEFFFF AC58EA00  MOV DWORD PTR SS:[EBP-0x15C], PathOfEx.00EA58AC
	$+315    0078DC55      C785 FCFEFFFF 24C5DD00  MOV DWORD PTR SS:[EBP-0x104], PathOfEx.00DDC524        ; ASCII "悫F"
	$+31F    0078DC5F      8945 FC                 MOV DWORD PTR SS:[EBP-0x4], EAX
	$+322    0078DC62      8D85 ACFEFFFF           LEA EAX, DWORD PTR SS:[EBP-0x154]
	$+328    0078DC68      51                      PUSH ECX
	$+329    0078DC69      50                      PUSH EAX
	$+32A    0078DC6A      8D8D 94FEFFFF           LEA ECX, DWORD PTR SS:[EBP-0x16C]
	$+330    0078DC70      C745 3C 01000000        MOV DWORD PTR SS:[EBP+0x3C], 0x1
	$+337    0078DC77      E8 1407CEFF             CALL PathOfEx.0046E390
	$+33C    0078DC7C      C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+343    0078DC83      8B85 94FEFFFF           MOV EAX, DWORD PTR SS:[EBP-0x16C]
	$+349    0078DC89      6A 03                   PUSH 0x3
	$+34B    0078DC8B      8B40 04                 MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+34E    0078DC8E      C78405 94FEFFFF B86DE60>MOV DWORD PTR SS:[EBP+EAX-0x16C], PathOfEx.00E66DB8    ; ASCII "蹒F"
	$+359    0078DC99      8B85 94FEFFFF           MOV EAX, DWORD PTR SS:[EBP-0x16C]
	$+35F    0078DC9F      8B48 04                 MOV ECX, DWORD PTR DS:[EAX+0x4]
	$+362    0078DCA2      8D41 98                 LEA EAX, DWORD PTR DS:[ECX-0x68]
	$+365    0078DCA5      89840D 90FEFFFF         MOV DWORD PTR SS:[EBP+ECX-0x170], EAX
	$+36C    0078DCAC      8D8D ACFEFFFF           LEA ECX, DWORD PTR SS:[EBP-0x154]
	$+372    0078DCB2      E8 B90CCEFF             CALL PathOfEx.0046E970
	$+377    0078DCB7      8D85 A4FEFFFF           LEA EAX, DWORD PTR SS:[EBP-0x15C]
	$+37D    0078DCBD      C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+384    0078DCC4      68 0412ED00             PUSH PathOfEx.00ED1204                                 ; ASCII "Select timed out in a blocking call."
	$+389    0078DCC9      50                      PUSH EAX
	$+38A    0078DCCA      E8 918DD9FF             CALL PathOfEx.00526A60
	$+38F    0078DCCF      83C4 08                 ADD ESP, 0x8
	$+392    0078DCD2      8D45 10                 LEA EAX, DWORD PTR SS:[EBP+0x10]
	$+395    0078DCD5      8D8D 94FEFFFF           LEA ECX, DWORD PTR SS:[EBP-0x16C]
	$+39B    0078DCDB      50                      PUSH EAX
	$+39C    0078DCDC      E8 BFFDCEFF             CALL PathOfEx.0047DAA0
	$+3A1    0078DCE1      8BF8                    MOV EDI, EAX
	$+3A3    0078DCE3      C645 FC 03              MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+3A7    0078DCE7      FF15 CC50D800           CALL NEAR DWORD PTR DS:[0xD850CC]                      ; kernel32.GetTickCount
	$+3AD    0078DCED      8BD8                    MOV EBX, EAX
	$+3AF    0078DCEF      8D45 38                 LEA EAX, DWORD PTR SS:[EBP+0x38]
	$+3B2    0078DCF2      50                      PUSH EAX
	$+3B3    0078DCF3      E8 BF335100             CALL PathOfEx.00CA10B7
	$+3B8    0078DCF8      64:8B0D 2C000000        MOV ECX, DWORD PTR FS:[0x2C]
	$+3BF    0078DCFF      83C4 04                 ADD ESP, 0x4
	$+3C2    0078DD02      8B09                    MOV ECX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+3C4    0078DD04      57                      PUSH EDI
	$+3C5    0078DD05      83EC 0C                 SUB ESP, 0xC
	$+3C8    0078DD08      8B49 18                 MOV ECX, DWORD PTR DS:[ECX+0x18]                       ; PathOfEx.00720069
	$+3CB    0078DD0B      6A 02                   PUSH 0x2
	$+3CD    0078DD0D      53                      PUSH EBX
	$+3CE    0078DD0E      FF75 3C                 PUSH DWORD PTR SS:[EBP+0x3C]
	$+3D1    0078DD11      FF75 38                 PUSH DWORD PTR SS:[EBP+0x38]
	$+3D4    0078DD14      68 64020000             PUSH 0x264
	$+3D9    0078DD19      E8 C2F9CEFF             CALL PathOfEx.0047D6E0
	$+3DE    0078DD1E      8B45 24                 MOV EAX, DWORD PTR SS:[EBP+0x24]
	$+3E1    0078DD21      83F8 08                 CMP EAX, 0x8
	$+3E4    0078DD24      72 0F                   JB SHORT PathOfEx.0078DD35
	$+3E6    0078DD26      40                      INC EAX
	$+3E7    0078DD27      6A 02                   PUSH 0x2
	$+3E9    0078DD29      50                      PUSH EAX
	$+3EA    0078DD2A      FF75 10                 PUSH DWORD PTR SS:[EBP+0x10]
	$+3ED    0078DD2D      E8 DE4ACDFF             CALL PathOfEx.00462810
	$+3F2    0078DD32      83C4 0C                 ADD ESP, 0xC
	$+3F5    0078DD35      8D8D FCFEFFFF           LEA ECX, DWORD PTR SS:[EBP-0x104]
	$+3FB    0078DD3B      E8 60EFCDFF             CALL PathOfEx.0046CCA0
	$+400    0078DD40      C745 FC 04000000        MOV DWORD PTR SS:[EBP-0x4], 0x4
	$+407    0078DD47      8D85 FCFEFFFF           LEA EAX, DWORD PTR SS:[EBP-0x104]
	$+40D    0078DD4D      C785 FCFEFFFF 9CC4DD00  MOV DWORD PTR SS:[EBP-0x104], PathOfEx.00DDC49C        ; ASCII "疴F"
	$+417    0078DD57      50                      PUSH EAX
	$+418    0078DD58      E8 D1254D00             CALL PathOfEx.00C6032E
	$+41D    0078DD5D      83C4 04                 ADD ESP, 0x4
	$+420    0078DD60      6A 02                   PUSH 0x2
	$+422    0078DD62      FF36                    PUSH DWORD PTR DS:[ESI]                                ; PathOfEx.00E6AE58
	$+424    0078DD64      FF15 0455D800           CALL NEAR DWORD PTR DS:[0xD85504]
	$+42A    0078DD6A      66:C786 B0000000 0000   MOV WORD PTR DS:[ESI+0xB0], 0x0
	$+433    0078DD73    ^ E9 D9FCFFFF             JMP PathOfEx.0078DA51
	$+438    0078DD78      57                      PUSH EDI
	$+439    0078DD79      8BCE                    MOV ECX, ESI
	$+43B    0078DD7B      E8 C0FBFFFF             CALL PathOfEx.0078D940
	$+440    0078DD80      8B4D F4                 MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+443    0078DD83      5F                      POP EDI
	$+444    0078DD84      5E                      POP ESI
	$+445    0078DD85      5B                      POP EBX
	$+446    0078DD86      64:890D 00000000        MOV DWORD PTR FS:[0], ECX
	$+44D    0078DD8D      8D65 40                 LEA ESP, DWORD PTR SS:[EBP+0x40]
	$+450    0078DD90      5D                      POP EBP
	$+451    0078DD91      C2 0400                 RETN 0x4
	$+454    0078DD94      83BE 88000000 00        CMP DWORD PTR DS:[ESI+0x88], 0x0
	$+45B    0078DD9B    ^ 0F84 B0FCFFFF           JE PathOfEx.0078DA51
	$+461    0078DDA1      837E 54 00              CMP DWORD PTR DS:[ESI+0x54], 0x0
	$+465    0078DDA5    ^ 0F85 A6FCFFFF           JNZ PathOfEx.0078DA51
	$+46B    0078DDAB      8D45 48                 LEA EAX, DWORD PTR SS:[EBP+0x48]
	$+46E    0078DDAE      8975 48                 MOV DWORD PTR SS:[EBP+0x48], ESI
	$+471    0078DDB1      50                      PUSH EAX
	$+472    0078DDB2      8D4E 30                 LEA ECX, DWORD PTR DS:[ESI+0x30]
	$+475    0078DDB5      E8 D6040000             CALL PathOfEx.0078E290
	$+47A    0078DDBA    ^ E9 92FCFFFF             JMP PathOfEx.0078DA51
	$+47F    0078DDBF      C786 B4000000 00000000  MOV DWORD PTR DS:[ESI+0xB4], 0x0
	$+489    0078DDC9      C786 B8000000 00000000  MOV DWORD PTR DS:[ESI+0xB8], 0x0
	$+493    0078DDD3      C686 B0000000 00        MOV BYTE PTR DS:[ESI+0xB0], 0x0
	$+49A    0078DDDA    ^ E9 72FCFFFF             JMP PathOfEx.0078DA51
	$+49F    0078DDDF      CC                      INT3
	$+4A0    0078DDE0      55                      PUSH EBP
	$+4A1    0078DDE1      8BEC                    MOV EBP, ESP
	$+4A3    0078DDE3      6A FF                   PUSH -0x1
	$+4A5    0078DDE5      68 388ED400             PUSH PathOfEx.00D48E38
	$+4AA    0078DDEA      64:A1 00000000          MOV EAX, DWORD PTR FS:[0]
	$+4B0    0078DDF0      50                      PUSH EAX
	$+4B1    0078DDF1      64:8925 00000000        MOV DWORD PTR FS:[0], ESP
	$+4B8    0078DDF8      83EC 08                 SUB ESP, 0x8
	$+4BB    0078DDFB      53                      PUSH EBX
	$+4BC    0078DDFC      56                      PUSH ESI
	$+4BD    0078DDFD      57                      PUSH EDI
	$+4BE    0078DDFE      8BF9                    MOV EDI, ECX
	$+4C0    0078DE00      8D9F 90000000           LEA EBX, DWORD PTR DS:[EDI+0x90]
	$+4C6    0078DE06      C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+4CD    0078DE0D      8D45 08                 LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+4D0    0078DE10      3BD8                    CMP EBX, EAX
	$+4D2    0078DE12      74 18                   JE SHORT PathOfEx.0078DE2C
	$+4D4    0078DE14      8B0B                    MOV ECX, DWORD PTR DS:[EBX]
	$+4D6    0078DE16      8B45 08                 MOV EAX, DWORD PTR SS:[EBP+0x8]
	$+4D9    0078DE19      C745 08 00000000        MOV DWORD PTR SS:[EBP+0x8], 0x0
	$+4E0    0078DE20      8903                    MOV DWORD PTR DS:[EBX], EAX
	$+4E2    0078DE22      85C9                    TEST ECX, ECX
	$+4E4    0078DE24      74 06                   JE SHORT PathOfEx.0078DE2C
	$+4E6    0078DE26      8B01                    MOV EAX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+4E8    0078DE28      6A 01                   PUSH 0x1
	$+4EA    0078DE2A      FF10                    CALL NEAR DWORD PTR DS:[EAX]
	$+4EC    0078DE2C      8B87 B4000000           MOV EAX, DWORD PTR DS:[EDI+0xB4]
	$+4F2    0078DE32      33D2                    XOR EDX, EDX                                           ; PathOfEx.00EA3238
	$+4F4    0078DE34      8BB7 C4000000           MOV ESI, DWORD PTR DS:[EDI+0xC4]
	$+4FA    0078DE3A      8987 B8000000           MOV DWORD PTR DS:[EDI+0xB8], EAX
	$+500    0078DE40      8B8F D8000000           MOV ECX, DWORD PTR DS:[EDI+0xD8]
	$+506    0078DE46      2B8F D4000000           SUB ECX, DWORD PTR DS:[EDI+0xD4]
	$+50C    0078DE4C      8B87 BC000000           MOV EAX, DWORD PTR DS:[EDI+0xBC]
	$+512    0078DE52      03C6                    ADD EAX, ESI
	$+514    0078DE54      F7F1                    DIV ECX
	$+516    0078DE56      8B87 C0000000           MOV EAX, DWORD PTR DS:[EDI+0xC0]
	$+51C    0078DE5C      8B8F D8000000           MOV ECX, DWORD PTR DS:[EDI+0xD8]
	$+522    0078DE62      2BC6                    SUB EAX, ESI
	$+524    0078DE64      2B8F D4000000           SUB ECX, DWORD PTR DS:[EDI+0xD4]
	$+52A    0078DE6A      8945 EC                 MOV DWORD PTR SS:[EBP-0x14], EAX
	$+52D    0078DE6D      03C2                    ADD EAX, EDX                                           ; PathOfEx.00EA3238
	$+52F    0078DE6F      3BC1                    CMP EAX, ECX
	$+531    0078DE71      8955 F0                 MOV DWORD PTR SS:[EBP-0x10], EDX                       ; PathOfEx.00EA3238
	$+534    0078DE74      8B0B                    MOV ECX, DWORD PTR DS:[EBX]
	$+536    0078DE76      8B87 D4000000           MOV EAX, DWORD PTR DS:[EDI+0xD4]
	$+53C    0078DE7C      77 0F                   JA SHORT PathOfEx.0078DE8D
	$+53E    0078DE7E      8B11                    MOV EDX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+540    0078DE80      8B5D EC                 MOV EBX, DWORD PTR SS:[EBP-0x14]
	$+543    0078DE83      0345 F0                 ADD EAX, DWORD PTR SS:[EBP-0x10]
	$+546    0078DE86      53                      PUSH EBX
	$+547    0078DE87      50                      PUSH EAX
	$+548    0078DE88      FF52 08                 CALL NEAR DWORD PTR DS:[EDX+0x8]                       ; PathOfEx.0046F4C0
	$+54B    0078DE8B      EB 2B                   JMP SHORT PathOfEx.0078DEB8
	$+54D    0078DE8D      8BB7 D8000000           MOV ESI, DWORD PTR DS:[EDI+0xD8]
	$+553    0078DE93      2BB7 D4000000           SUB ESI, DWORD PTR DS:[EDI+0xD4]
	$+559    0078DE99      0345 F0                 ADD EAX, DWORD PTR SS:[EBP-0x10]
	$+55C    0078DE9C      2BF2                    SUB ESI, EDX                                           ; PathOfEx.00EA3238
	$+55E    0078DE9E      8B11                    MOV EDX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+560    0078DEA0      56                      PUSH ESI
	$+561    0078DEA1      50                      PUSH EAX
	$+562    0078DEA2      FF52 08                 CALL NEAR DWORD PTR DS:[EDX+0x8]                       ; PathOfEx.0046F4C0
	$+565    0078DEA5      8B0B                    MOV ECX, DWORD PTR DS:[EBX]
	$+567    0078DEA7      8B5D EC                 MOV EBX, DWORD PTR SS:[EBP-0x14]
	$+56A    0078DEAA      2BDE                    SUB EBX, ESI
	$+56C    0078DEAC      53                      PUSH EBX
	$+56D    0078DEAD      8B01                    MOV EAX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+56F    0078DEAF      FFB7 D4000000           PUSH DWORD PTR DS:[EDI+0xD4]
	$+575    0078DEB5      FF50 08                 CALL NEAR DWORD PTR DS:[EAX+0x8]
	$+578    0078DEB8      8B4D 08                 MOV ECX, DWORD PTR SS:[EBP+0x8]
	$+57B    0078DEBB      5F                      POP EDI
	$+57C    0078DEBC      5E                      POP ESI
	$+57D    0078DEBD      5B                      POP EBX
	$+57E    0078DEBE      85C9                    TEST ECX, ECX
	$+580    0078DEC0      74 06                   JE SHORT PathOfEx.0078DEC8
	$+582    0078DEC2      8B01                    MOV EAX, DWORD PTR DS:[ECX]                            ; PathOfEx.00E6AE58
	$+584    0078DEC4      6A 01                   PUSH 0x1
	$+586    0078DEC6      FF10                    CALL NEAR DWORD PTR DS:[EAX]
	$+588    0078DEC8      8B4D F4                 MOV ECX, DWORD PTR SS:[EBP-0xC]                        ; PathOfEx.005C9CFF
	$+58B    0078DECB      64:890D 00000000        MOV DWORD PTR FS:[0], ECX
	$+592    0078DED2      8BE5                    MOV ESP, EBP
	$+594    0078DED4      5D                      POP EBP                                                ; 221A3D10
	$+595    0078DED5      C2 0400                 RETN 0x4
	*/

	//gj
	/*
	$-363    01416C0F      CC                          INT3
	$-362    01416C10      55                          PUSH EBP
	$-361    01416C11      8BEC                        MOV EBP,ESP
	$-35F    01416C13      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$-359    01416C19      6A FF                       PUSH -0x1
	$-357    01416C1B      68 B8968A01                 PUSH PathOfEx.018A96B8
	$-352    01416C20      50                          PUSH EAX
	$-351    01416C21      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$-34A    01416C28      83EC 20                     SUB ESP,0x20
	$-347    01416C2B      53                          PUSH EBX
	$-346    01416C2C      8B5D 08                     MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-343    01416C2F      56                          PUSH ESI
	$-342    01416C30      57                          PUSH EDI
	$-341    01416C31      8BF9                        MOV EDI,ECX
	$-33F    01416C33      8B0B                        MOV ECX,DWORD PTR DS:[EBX]
	$-33D    01416C35      85C9                        TEST ECX,ECX
	$-33B    01416C37      74 05                       JE SHORT PathOfEx.01416C3E
	$-339    01416C39      E8 D2F8E3FF                 CALL PathOfEx.01256510
	$-334    01416C3E      8B03                        MOV EAX,DWORD PTR DS:[EBX]
	$-332    01416C40      C745 08 649CA701            MOV DWORD PTR SS:[EBP+0x8],PathOfEx.01A79C64       ; ASCII 50,"layer"
	$-32B    01416C47      8B70 30                     MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-328    01416C4A      8D45 08                     LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-325    01416C4D      50                          PUSH EAX
	$-324    01416C4E      8D45 F0                     LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-321    01416C51      50                          PUSH EAX
	$-320    01416C52      8D4E 14                     LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-31D    01416C55      E8 661CBAFF                 CALL PathOfEx.00FB88C0
	$-318    01416C5A      8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-315    01416C5D      3B46 18                     CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-312    01416C60      0F84 C2000000               JE PathOfEx.01416D28
	$-30C    01416C66      8B48 0C                     MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-309    01416C69      83F9 FF                     CMP ECX,-0x1
	$-306    01416C6C      0F84 B6000000               JE PathOfEx.01416D28
	$-300    01416C72      8B43 04                     MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-2FD    01416C75      8B3488                      MOV ESI,DWORD PTR DS:[EAX+ECX*4]
	$-2FA    01416C78      85F6                        TEST ESI,ESI
	$-2F8    01416C7A      0F84 A8000000               JE PathOfEx.01416D28
	$-2F2    01416C80      8B43 14                     MOV EAX,DWORD PTR DS:[EBX+0x14]
	$-2EF    01416C83      3B87 AC3A0000               CMP EAX,DWORD PTR DS:[EDI+0x3AAC]
	$-2E9    01416C89      0F84 D1000000               JE PathOfEx.01416D60
	$-2E3    01416C8F      33C9                        XOR ECX,ECX
	$-2E1    01416C91      C745 E8 07000000            MOV DWORD PTR SS:[EBP-0x18],0x7
	$-2DA    01416C98      6A FF                       PUSH -0x1
	$-2D8    01416C9A      51                          PUSH ECX
	$-2D7    01416C9B      8D46 10                     LEA EAX,DWORD PTR DS:[ESI+0x10]
	$-2D4    01416C9E      66:894D D4                  MOV WORD PTR SS:[EBP-0x2C],CX
	$-2D0    01416CA2      50                          PUSH EAX
	$-2CF    01416CA3      8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-2CC    01416CA6      C745 E4 00000000            MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-2C5    01416CAD      E8 6E34BAFF                 CALL PathOfEx.00FBA120
	$-2C0    01416CB2      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$-2B9    01416CB9      8D45 D4                     LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-2B6    01416CBC      8B97 543A0000               MOV EDX,DWORD PTR DS:[EDI+0x3A54]
	$-2B0    01416CC2      8B8F 503A0000               MOV ECX,DWORD PTR DS:[EDI+0x3A50]
	$-2AA    01416CC8      8945 08                     MOV DWORD PTR SS:[EBP+0x8],EAX
	$-2A7    01416CCB      8D45 08                     LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-2A4    01416CCE      50                          PUSH EAX
	$-2A3    01416CCF      E8 2C510000                 CALL PathOfEx.0141BE00
	$-29E    01416CD4      83C4 04                     ADD ESP,0x4
	$-29B    01416CD7      8845 F0                     MOV BYTE PTR SS:[EBP-0x10],AL
	$-298    01416CDA      8D45 EC                     LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-295    01416CDD      8975 EC                     MOV DWORD PTR SS:[EBP-0x14],ESI
	$-292    01416CE0      8D8F C43A0000               LEA ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-28C    01416CE6      50                          PUSH EAX
	$-28B    01416CE7      E8 3475C8FF                 CALL PathOfEx.0109E220
	$-286    01416CEC      A1 403AD001                 MOV EAX,DWORD PTR DS:[0x1D03A40]
	$-281    01416CF1      85C0                        TEST EAX,EAX
	$-27F    01416CF3      74 22                       JE SHORT PathOfEx.01416D17
	$-27D    01416CF5      8B8F C83A0000               MOV ECX,DWORD PTR DS:[EDI+0x3AC8]
	$-277    01416CFB      8D50 18                     LEA EDX,DWORD PTR DS:[EAX+0x18]
	$-274    01416CFE      2B8F C43A0000               SUB ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-26E    01416D04      8D45 08                     LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-26B    01416D07      C1F9 03                     SAR ECX,0x3
	$-268    01416D0A      41                          INC ECX
	$-267    01416D0B      390A                        CMP DWORD PTR DS:[EDX],ECX
	$-265    01416D0D      894D 08                     MOV DWORD PTR SS:[EBP+0x8],ECX
	$-262    01416D10      0F43C2                      CMOVNB EAX,EDX
	$-25F    01416D13      8B00                        MOV EAX,DWORD PTR DS:[EAX]
	$-25D    01416D15      8902                        MOV DWORD PTR DS:[EDX],EAX
	$-25B    01416D17      8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-258    01416D1A      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-251    01416D21      E8 BA32BAFF                 CALL PathOfEx.00FB9FE0
	$-24C    01416D26      EB 0C                       JMP SHORT PathOfEx.01416D34
	$-24A    01416D28      53                          PUSH EBX
	$-249    01416D29      8D8F D03A0000               LEA ECX,DWORD PTR DS:[EDI+0x3AD0]
	$-243    01416D2F      E8 7C1A0100                 CALL PathOfEx.014287B0
	$-23E    01416D34      8B03                        MOV EAX,DWORD PTR DS:[EBX]
	$-23C    01416D36      3B87 B43A0000               CMP EAX,DWORD PTR DS:[EDI+0x3AB4]
	$-236    01416D3C      75 22                       JNZ SHORT PathOfEx.01416D60
	$-234    01416D3E      8B8F 8C380000               MOV ECX,DWORD PTR DS:[EDI+0x388C]
	$-22E    01416D44      8D97 483D0000               LEA EDX,DWORD PTR DS:[EDI+0x3D48]
	$-228    01416D4A      83C1 04                     ADD ECX,0x4
	$-225    01416D4D      E8 CEE2F3FF                 CALL PathOfEx.01355020
	$-220    01416D52      84C0                        TEST AL,AL
	$-21E    01416D54      74 0A                       JE SHORT PathOfEx.01416D60
	$-21C    01416D56      6A 00                       PUSH 0x0
	$-21A    01416D58      8D4F F8                     LEA ECX,DWORD PTR DS:[EDI-0x8]
	$-217    01416D5B      E8 301D0000                 CALL PathOfEx.01418A90
	$-212    01416D60      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-20F    01416D63      5F                          POP EDI
	$-20E    01416D64      5E                          POP ESI
	$-20D    01416D65      5B                          POP EBX
	$-20C    01416D66      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$-205    01416D6D      8BE5                        MOV ESP,EBP
	$-203    01416D6F      5D                          POP EBP
	$-202    01416D70      C2 0400                     RETN 0x4
	$-1FF    01416D73      CC                          INT3
	$-1FE    01416D74      CC                          INT3
	$-1FD    01416D75      CC                          INT3
	$-1FC    01416D76      CC                          INT3
	$-1FB    01416D77      CC                          INT3
	$-1FA    01416D78      CC                          INT3
	$-1F9    01416D79      CC                          INT3
	$-1F8    01416D7A      CC                          INT3
	$-1F7    01416D7B      CC                          INT3
	$-1F6    01416D7C      CC                          INT3
	$-1F5    01416D7D      CC                          INT3
	$-1F4    01416D7E      CC                          INT3
	$-1F3    01416D7F      CC                          INT3
	$-1F2    01416D80      83EC 08                     SUB ESP,0x8
	$-1EF    01416D83      53                          PUSH EBX
	$-1EE    01416D84      56                          PUSH ESI
	$-1ED    01416D85      57                          PUSH EDI
	$-1EC    01416D86      8B7C24 18                   MOV EDI,DWORD PTR SS:[ESP+0x18]
	$-1E8    01416D8A      8BD9                        MOV EBX,ECX
	$-1E6    01416D8C      8B0F                        MOV ECX,DWORD PTR DS:[EDI]
	$-1E4    01416D8E      85C9                        TEST ECX,ECX
	$-1E2    01416D90      74 05                       JE SHORT PathOfEx.01416D97
	$-1E0    01416D92      E8 79F7E3FF                 CALL PathOfEx.01256510
	$-1DB    01416D97      8B07                        MOV EAX,DWORD PTR DS:[EDI]
	$-1D9    01416D99      C74424 18 649CA701          MOV DWORD PTR SS:[ESP+0x18],PathOfEx.01A79C64      ; ASCII 50,"layer"
	$-1D1    01416DA1      8B70 30                     MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-1CE    01416DA4      8D4424 18                   LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-1CA    01416DA8      50                          PUSH EAX
	$-1C9    01416DA9      8D4424 10                   LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-1C5    01416DAD      50                          PUSH EAX
	$-1C4    01416DAE      8D4E 14                     LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-1C1    01416DB1      E8 0A1BBAFF                 CALL PathOfEx.00FB88C0
	$-1BC    01416DB6      8B4424 0C                   MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-1B8    01416DBA      3B46 18                     CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-1B5    01416DBD      74 52                       JE SHORT PathOfEx.01416E11
	$-1B3    01416DBF      8B70 0C                     MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-1B0    01416DC2      83FE FF                     CMP ESI,-0x1
	$-1AD    01416DC5      74 4A                       JE SHORT PathOfEx.01416E11
	$-1AB    01416DC7      8B47 04                     MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-1A8    01416DCA      8B34B0                      MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-1A5    01416DCD      897424 10                   MOV DWORD PTR SS:[ESP+0x10],ESI
	$-1A1    01416DD1      85F6                        TEST ESI,ESI
	$-19F    01416DD3      74 44                       JE SHORT PathOfEx.01416E19
	$-19D    01416DD5      8B47 14                     MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-19A    01416DD8      3B83 AC3A0000               CMP EAX,DWORD PTR DS:[EBX+0x3AAC]
	$-194    01416DDE      74 45                       JE SHORT PathOfEx.01416E25
	$-192    01416DE0      8B83 C83A0000               MOV EAX,DWORD PTR DS:[EBX+0x3AC8]
	$-18C    01416DE6      8D8B C43A0000               LEA ECX,DWORD PTR DS:[EBX+0x3AC4]
	$-186    01416DEC      8B11                        MOV EDX,DWORD PTR DS:[ECX]
	$-184    01416DEE      3BD0                        CMP EDX,EAX
	$-182    01416DF0      74 0B                       JE SHORT PathOfEx.01416DFD
	$-180    01416DF2      3932                        CMP DWORD PTR DS:[EDX],ESI
	$-17E    01416DF4      74 07                       JE SHORT PathOfEx.01416DFD
	$-17C    01416DF6      83C2 08                     ADD EDX,0x8
	$-179    01416DF9      3BD0                        CMP EDX,EAX
	$-177    01416DFB    ^ 75 F5                       JNZ SHORT PathOfEx.01416DF2
	$-175    01416DFD      52                          PUSH EDX
	$-174    01416DFE      8D4424 1C                   LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-170    01416E02      50                          PUSH EAX
	$-16F    01416E03      E8 982F0000                 CALL PathOfEx.01419DA0
	$-16A    01416E08      5F                          POP EDI
	$-169    01416E09      5E                          POP ESI
	$-168    01416E0A      5B                          POP EBX
	$-167    01416E0B      83C4 08                     ADD ESP,0x8
	$-164    01416E0E      C2 0400                     RETN 0x4
	$-161    01416E11      C74424 10 00000000          MOV DWORD PTR SS:[ESP+0x10],0x0
	$-159    01416E19      57                          PUSH EDI
	$-158    01416E1A      8D8B D03A0000               LEA ECX,DWORD PTR DS:[EBX+0x3AD0]
	$-152    01416E20      E8 8B1C0100                 CALL PathOfEx.01428AB0
	$-14D    01416E25      5F                          POP EDI
	$-14C    01416E26      5E                          POP ESI
	$-14B    01416E27      5B                          POP EBX
	$-14A    01416E28      83C4 08                     ADD ESP,0x8
	$-147    01416E2B      C2 0400                     RETN 0x4
	$-144    01416E2E      CC                          INT3
	$-143    01416E2F      CC                          INT3
	$-142    01416E30      8B81 88380000               MOV EAX,DWORD PTR DS:[ECX+0x3888]
	$-13C    01416E36      83B8 68010000 02            CMP DWORD PTR DS:[EAX+0x168],0x2
	$-135    01416E3D      74 11                       JE SHORT PathOfEx.01416E50
	$-133    01416E3F      FFB1 AC3A0000               PUSH DWORD PTR DS:[ECX+0x3AAC]
	$-12D    01416E45      81C1 2C390000               ADD ECX,0x392C
	$-127    01416E4B      E8 B0F82000                 CALL PathOfEx.01626700
	$-122    01416E50      C3                          RETN
	$-121    01416E51      CC                          INT3
	$-120    01416E52      CC                          INT3
	$-11F    01416E53      CC                          INT3
	$-11E    01416E54      CC                          INT3
	$-11D    01416E55      CC                          INT3
	$-11C    01416E56      CC                          INT3
	$-11B    01416E57      CC                          INT3
	$-11A    01416E58      CC                          INT3
	$-119    01416E59      CC                          INT3
	$-118    01416E5A      CC                          INT3
	$-117    01416E5B      CC                          INT3
	$-116    01416E5C      CC                          INT3
	$-115    01416E5D      CC                          INT3
	$-114    01416E5E      CC                          INT3
	$-113    01416E5F      CC                          INT3
	$-112    01416E60      83B9 643D0000 02            CMP DWORD PTR DS:[ECX+0x3D64],0x2
	$-10B    01416E67      75 1D                       JNZ SHORT PathOfEx.01416E86
	$-109    01416E69      0FB681 A83C0000             MOVZX EAX,BYTE PTR DS:[ECX+0x3CA8]
	$-102    01416E70      C781 643D0000 00000000      MOV DWORD PTR DS:[ECX+0x3D64],0x0
	$-F8     01416E7A      81C1 88150000               ADD ECX,0x1588
	$-F2     01416E80      50                          PUSH EAX
	$-F1     01416E81      E8 BAA8BEFF                 CALL PathOfEx.01001740
	$-EC     01416E86      C3                          RETN
	$-EB     01416E87      CC                          INT3
	$-EA     01416E88      CC                          INT3
	$-E9     01416E89      CC                          INT3
	$-E8     01416E8A      CC                          INT3
	$-E7     01416E8B      CC                          INT3
	$-E6     01416E8C      CC                          INT3
	$-E5     01416E8D      CC                          INT3
	$-E4     01416E8E      CC                          INT3
	$-E3     01416E8F      CC                          INT3
	$-E2     01416E90      55                          PUSH EBP
	$-E1     01416E91      8BEC                        MOV EBP,ESP
	$-DF     01416E93      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$-D9     01416E99      6A FF                       PUSH -0x1
	$-D7     01416E9B      68 80C78E01                 PUSH PathOfEx.018EC780
	$-D2     01416EA0      50                          PUSH EAX
	$-D1     01416EA1      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$-CA     01416EA8      83EC 18                     SUB ESP,0x18
	$-C7     01416EAB      57                          PUSH EDI
	$-C6     01416EAC      8BF9                        MOV EDI,ECX
	$-C4     01416EAE      8B87 88380000               MOV EAX,DWORD PTR DS:[EDI+0x3888]
	$-BE     01416EB4      83B8 68010000 02            CMP DWORD PTR DS:[EAX+0x168],0x2
	$-B7     01416EBB      0F84 ED000000               JE PathOfEx.01416FAE
	$-B1     01416EC1      56                          PUSH ESI
	$-B0     01416EC2      8B75 08                     MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-AD     01416EC5      8BCE                        MOV ECX,ESI
	$-AB     01416EC7      E8 A41D2500                 CALL PathOfEx.01668C70
	$-A6     01416ECC      B9 00800000                 MOV ECX,0x8000
	$-A1     01416ED1      66:85C1                     TEST CX,AX
	$-9E     01416ED4      0F85 D3000000               JNZ PathOfEx.01416FAD
	$-98     01416EDA      837E 1C 00                  CMP DWORD PTR DS:[ESI+0x1C],0x0
	$-94     01416EDE      74 44                       JE SHORT PathOfEx.01416F24
	$-92     01416EE0      B8 12000000                 MOV EAX,0x12
	$-8D     01416EE5      C745 E0 E882A801            MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01A882E8
	$-86     01416EEC      66:8945 E4                  MOV WORD PTR SS:[EBP-0x1C],AX
	$-82     01416EF0      8BCE                        MOV ECX,ESI
	$-80     01416EF2      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$-79     01416EF9      E8 C2202500                 CALL PathOfEx.01668FC0
	$-74     01416EFE      8BCE                        MOV ECX,ESI
	$-72     01416F00      66:8945 EC                  MOV WORD PTR SS:[EBP-0x14],AX
	$-6E     01416F04      E8 671D2500                 CALL PathOfEx.01668C70
	$-69     01416F09      66:8945 EE                  MOV WORD PTR SS:[EBP-0x12],AX
	$-65     01416F0D      8D4D E0                     LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-62     01416F10      8A46 17                     MOV AL,BYTE PTR DS:[ESI+0x17]
	$-5F     01416F13      8845 F0                     MOV BYTE PTR SS:[EBP-0x10],AL
	$-5C     01416F16      8B46 1C                     MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$-59     01416F19      8B40 14                     MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-56     01416F1C      8945 E8                     MOV DWORD PTR SS:[EBP-0x18],EAX
	$-53     01416F1F      8B45 E0                     MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-50     01416F22      EB 73                       JMP SHORT PathOfEx.01416F97
	$-4E     01416F24      F3:0F1087 A43A0000          MOVSS XMM0,DWORD PTR DS:[EDI+0x3AA4]
	$-46     01416F2C      F3:0F5905 C408A001          MULSS XMM0,DWORD PTR DS:[0x1A008C4]
	$-3E     01416F34      C745 DC F482A801            MOV DWORD PTR SS:[EBP-0x24],PathOfEx.01A882F4
	$-37     01416F3B      F3:0F2CC8                   CVTTSS2SI ECX,XMM0
	$-33     01416F3F      F3:0F1087 A03A0000          MOVSS XMM0,DWORD PTR DS:[EDI+0x3AA0]
	$-2B     01416F47      F3:0F5905 C408A001          MULSS XMM0,DWORD PTR DS:[0x1A008C4]
	$-23     01416F4F      894E 24                     MOV DWORD PTR DS:[ESI+0x24],ECX
	$-20     01416F52      F3:0F2CC0                   CVTTSS2SI EAX,XMM0
	$-1C     01416F56      8946 20                     MOV DWORD PTR DS:[ESI+0x20],EAX
	$-19     01416F59      B8 13000000                 MOV EAX,0x13
	$-14     01416F5E      66:8945 E0                  MOV WORD PTR SS:[EBP-0x20],AX
	$-10     01416F62      8BCE                        MOV ECX,ESI
	$-E      01416F64      C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4],0x1
	$-7      01416F6B      E8 50202500                 CALL PathOfEx.01668FC0
	$-2      01416F70      8BCE                        MOV ECX,ESI
	$ ==>    01416F72      66:8945 EC                  MOV WORD PTR SS:[EBP-0x14],AX
	$+4      01416F76      E8 F51C2500                 CALL PathOfEx.01668C70
	$+9      01416F7B      66:8945 EE                  MOV WORD PTR SS:[EBP-0x12],AX
	$+D      01416F7F      8D4D DC                     LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+10     01416F82      8A46 17                     MOV AL,BYTE PTR DS:[ESI+0x17]
	$+13     01416F85      8845 F0                     MOV BYTE PTR SS:[EBP-0x10],AL
	$+16     01416F88      8B46 20                     MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+19     01416F8B      8945 E4                     MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+1C     01416F8E      8B46 24                     MOV EAX,DWORD PTR DS:[ESI+0x24]
	$+1F     01416F91      8945 E8                     MOV DWORD PTR SS:[EBP-0x18],EAX
	$+22     01416F94      8B45 DC                     MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+25     01416F97      FFB7 8C380000               PUSH DWORD PTR DS:[EDI+0x388C]
	$+2B     01416F9D      FF50 04                     CALL DWORD PTR DS:[EAX+0x4]
	$+2E     01416FA0      8B8F 8C380000               MOV ECX,DWORD PTR DS:[EDI+0x388C]
	$+34     01416FA6      6A 00                       PUSH 0x0
	$+36     01416FA8      E8 33BBEBFF                 CALL PathOfEx.012D2AE0                             ; 调用发包call
	$+3B     01416FAD      5E                          POP ESI
	$+3C     01416FAE      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3F     01416FB1      5F                          POP EDI
	$+40     01416FB2      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+47     01416FB9      8BE5                        MOV ESP,EBP
	$+49     01416FBB      5D                          POP EBP
	$+4A     01416FBC      C2 0400                     RETN 0x4
	$+4D     01416FBF      CC                          INT3
	$+4E     01416FC0      55                          PUSH EBP
	$+4F     01416FC1      8BEC                        MOV EBP,ESP
	$+51     01416FC3      6A FF                       PUSH -0x1
	$+53     01416FC5      68 58208B01                 PUSH PathOfEx.018B2058
	$+58     01416FCA      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$+5E     01416FD0      50                          PUSH EAX
	$+5F     01416FD1      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$+66     01416FD8      83EC 0C                     SUB ESP,0xC
	$+69     01416FDB      B8 65000000                 MOV EAX,0x65
	$+6E     01416FE0      C745 E8 4C83A801            MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01A8834C
	$+75     01416FE7      56                          PUSH ESI
	$+76     01416FE8      8BF1                        MOV ESI,ECX
	$+78     01416FEA      66:8945 EC                  MOV WORD PTR SS:[EBP-0x14],AX
	$+7C     01416FEE      8A45 08                     MOV AL,BYTE PTR SS:[EBP+0x8]
	$+7F     01416FF1      8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+82     01416FF4      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$+89     01416FFB      FFB6 98380000               PUSH DWORD PTR DS:[ESI+0x3898]
	$+8F     01417001      8845 F0                     MOV BYTE PTR SS:[EBP-0x10],AL
	$+92     01417004      E8 0726FFFF                 CALL PathOfEx.01409610
	$+97     01417009      8B8E 98380000               MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+9D     0141700F      6A 00                       PUSH 0x0
	$+9F     01417011      E8 CABAEBFF                 CALL PathOfEx.012D2AE0
	$+A4     01417016      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+A7     01417019      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+AE     01417020      5E                          POP ESI
	$+AF     01417021      8BE5                        MOV ESP,EBP
	$+B1     01417023      5D                          POP EBP
	$+B2     01417024      C2 0400                     RETN 0x4
	$+B5     01417027      CC                          INT3
	$+B6     01417028      CC                          INT3
	$+B7     01417029      CC                          INT3
	$+B8     0141702A      CC                          INT3
	$+B9     0141702B      CC                          INT3
	$+BA     0141702C      CC                          INT3
	$+BB     0141702D      CC                          INT3
	$+BC     0141702E      CC                          INT3
	$+BD     0141702F      CC                          INT3
	$+BE     01417030      55                          PUSH EBP
	$+BF     01417031      8BEC                        MOV EBP,ESP
	$+C1     01417033      6A FF                       PUSH -0x1
	$+C3     01417035      68 98C78E01                 PUSH PathOfEx.018EC798
	$+C8     0141703A      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$+CE     01417040      50                          PUSH EAX
	$+CF     01417041      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$+D6     01417048      83EC 44                     SUB ESP,0x44
	$+D9     0141704B      B8 8C000000                 MOV EAX,0x8C
	$+DE     01417050      C745 F0 07000000            MOV DWORD PTR SS:[EBP-0x10],0x7
	$+E5     01417057      66:8945 B4                  MOV WORD PTR SS:[EBP-0x4C],AX
	$+E9     0141705B      BA 8081A801                 MOV EDX,PathOfEx.01A88180
	$+EE     01417060      33C0                        XOR EAX,EAX
	$+F0     01417062      8955 B0                     MOV DWORD PTR SS:[EBP-0x50],EDX
	$+F3     01417065      56                          PUSH ESI
	$+F4     01417066      8BF1                        MOV ESI,ECX
	$+F6     01417068      C745 EC 00000000            MOV DWORD PTR SS:[EBP-0x14],0x0
	$+FD     0141706F      66:8945 DC                  MOV WORD PTR SS:[EBP-0x24],AX
	$+101    01417073      8945 FC                     MOV DWORD PTR SS:[EBP-0x4],EAX
	$+104    01417076      8D4D DC                     LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+107    01417079      8A45 08                     MOV AL,BYTE PTR SS:[EBP+0x8]
	$+10A    0141707C      8845 B8                     MOV BYTE PTR SS:[EBP-0x48],AL
	$+10D    0141707F      8B45 10                     MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+110    01417082      3BC8                        CMP ECX,EAX
	$+112    01417084      74 0D                       JE SHORT PathOfEx.01417093
	$+114    01417086      6A FF                       PUSH -0x1
	$+116    01417088      6A 00                       PUSH 0x0
	$+118    0141708A      50                          PUSH EAX
	$+119    0141708B      E8 9030BAFF                 CALL PathOfEx.00FBA120
	$+11E    01417090      8B55 B0                     MOV EDX,DWORD PTR SS:[EBP-0x50]
	$+121    01417093      8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+124    01417096      8D4D B0                     LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+127    01417099      FFB6 98380000               PUSH DWORD PTR DS:[ESI+0x3898]
	$+12D    0141709F      0F1000                      MOVUPS XMM0,DQWORD PTR DS:[EAX]
	$+130    014170A2      0F1145 B9                   MOVUPS DQWORD PTR SS:[EBP-0x47],XMM0
	$+134    014170A6      0F1040 10                   MOVUPS XMM0,DQWORD PTR DS:[EAX+0x10]
	$+138    014170AA      0F1145 C9                   MOVUPS DQWORD PTR SS:[EBP-0x37],XMM0
	$+13C    014170AE      FF52 04                     CALL DWORD PTR DS:[EDX+0x4]
	$+13F    014170B1      8B8E 98380000               MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+145    014170B7      6A 00                       PUSH 0x0
	$+147    014170B9      E8 22BAEBFF                 CALL PathOfEx.012D2AE0
	$+14C    014170BE      8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+14F    014170C1      5E                          POP ESI
	$+150    014170C2      83F8 08                     CMP EAX,0x8
	$+153    014170C5      72 0F                       JB SHORT PathOfEx.014170D6
	$+155    014170C7      6A 02                       PUSH 0x2
	$+157    014170C9      40                          INC EAX
	$+158    014170CA      50                          PUSH EAX
	$+159    014170CB      FF75 DC                     PUSH DWORD PTR SS:[EBP-0x24]
	$+15C    014170CE      E8 DD1FBAFF                 CALL PathOfEx.00FB90B0
	$+161    014170D3      83C4 0C                     ADD ESP,0xC
	$+164    014170D6      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+167    014170D9      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+16E    014170E0      8BE5                        MOV ESP,EBP
	$+170    014170E2      5D                          POP EBP
	$+171    014170E3      C2 0C00                     RETN 0xC
	$+174    014170E6      CC                          INT3
	$+175    014170E7      CC                          INT3
	$+176    014170E8      CC                          INT3
	$+177    014170E9      CC                          INT3
	$+178    014170EA      CC                          INT3
	$+179    014170EB      CC                          INT3
	$+17A    014170EC      CC                          INT3
	$+17B    014170ED      CC                          INT3
	$+17C    014170EE      CC                          INT3
	$+17D    014170EF      CC                          INT3
	$+17E    014170F0      55                          PUSH EBP
	$+17F    014170F1      8BEC                        MOV EBP,ESP
	$+181    014170F3      6A FF                       PUSH -0x1
	$+183    014170F5      68 58208B01                 PUSH PathOfEx.018B2058
	$+188    014170FA      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$+18E    01417100      50                          PUSH EAX
	$+18F    01417101      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$+196    01417108      83EC 0C                     SUB ESP,0xC
	$+199    0141710B      B8 5B000000                 MOV EAX,0x5B
	$+19E    01417110      C745 E8 5883A801            MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01A88358
	$+1A5    01417117      56                          PUSH ESI
	$+1A6    01417118      8BF1                        MOV ESI,ECX
	$+1A8    0141711A      66:8945 EC                  MOV WORD PTR SS:[EBP-0x14],AX
	$+1AC    0141711E      8B45 08                     MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1AF    01417121      8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+1B2    01417124      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1B9    0141712B      FFB6 98380000               PUSH DWORD PTR DS:[ESI+0x3898]
	$+1BF    01417131      8945 F0                     MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1C2    01417134      E8 B71CFFFF                 CALL PathOfEx.01408DF0
	$+1C7    01417139      8B8E 98380000               MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+1CD    0141713F      6A 00                       PUSH 0x0
	$+1CF    01417141      E8 9AB9EBFF                 CALL PathOfEx.012D2AE0
	$+1D4    01417146      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1D7    01417149      5E                          POP ESI
	$+1D8    0141714A      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+1DF    01417151      8BE5                        MOV ESP,EBP
	$+1E1    01417153      5D                          POP EBP
	$+1E2    01417154      C2 0400                     RETN 0x4
	$+1E5    01417157      CC                          INT3
	*/
#ifdef VER_GJ_
	addr_tmp = scan_exe_.ScanCall("66??????e8????????66??????8d????8a????88????8b????89", 0x36 + 1);
#else
	addr_tmp = scan_exe_.ScanCall("8b??e8????????8b??????????6a??e8????????5e8b????5f64", 0xf + 1);
#endif
	assert(addr_tmp);
	ofs << "pt_cdword	g_c_SendPacket_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword(addr_tmp, 0x2f + 2);
	assert(dw_tmp > 0xa0 && dw_tmp < 0x150);
	ofs << "pt_cdword	g_o_send_msg_total_len_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//构建发包内容的函数，要与g_c_SendPacket_相配合才成。一般是先构建，随后再调用g_c_SendPacket_函数进行发包。
	/*
	$-D      0078D820     55                              PUSH EBP                                                ; 构建发包内容
	$-C      0078D821     8BEC                            MOV EBP, ESP
	$-A      0078D823     83E4 F8                         AND ESP, -0x8
	$-7      0078D826     83EC 10                         SUB ESP, 0x10
	$-4      0078D829     56                              PUSH ESI
	$-3      0078D82A     8BF1                            MOV ESI, ECX
	$-1      0078D82C     57                              PUSH EDI
	$ ==>    0078D82D     8B7D 0C                         MOV EDI, DWORD PTR SS:[EBP+0xC]
	$+3      0078D830     8B86 CC000000                   MOV EAX, DWORD PTR DS:[ESI+0xCC]
	$+9      0078D836     2B86 C8000000                   SUB EAX, DWORD PTR DS:[ESI+0xC8]
	$+F      0078D83C     3BF8                            CMP EDI, EAX
	$+11     0078D83E     76 27                           JBE SHORT PathOfEx.0078D867
	$+13     0078D840     57                              PUSH EDI
	$+14     0078D841     E8 9A060000                     CALL PathOfEx.0078DEE0
	$+19     0078D846     84C0                            TEST AL, AL
	$+1B     0078D848     75 1D                           JNZ SHORT PathOfEx.0078D867
	$+1D     0078D84A     6A 02                           PUSH 0x2
	$+1F     0078D84C     FF36                            PUSH DWORD PTR DS:[ESI]
	$+21     0078D84E     FF15 0455D800                   CALL NEAR DWORD PTR DS:[0xD85504]
	$+27     0078D854     66:C786 B0000000 0000           MOV WORD PTR DS:[ESI+0xB0], 0x0
	$+30     0078D85D     8BC6                            MOV EAX, ESI
	$+32     0078D85F     5F                              POP EDI                                                 ; ntdll_1z.7703F8D1
	$+33     0078D860     5E                              POP ESI                                                 ; ntdll_1z.7703F8D1
	$+34     0078D861     8BE5                            MOV ESP, EBP
	$+36     0078D863     5D                              POP EBP                                                 ; ntdll_1z.7703F8D1
	$+37     0078D864     C2 0800                         RETN 0x8
	$+3A     0078D867     8B8E CC000000                   MOV ECX, DWORD PTR DS:[ESI+0xCC]
	$+40     0078D86D     8B86 B4000000                   MOV EAX, DWORD PTR DS:[ESI+0xB4]
	$+46     0078D873     2B8E C8000000                   SUB ECX, DWORD PTR DS:[ESI+0xC8]
	$+4C     0078D879     03C7                            ADD EAX, EDI
	$+4E     0078D87B     3BC1                            CMP EAX, ECX
	$+50     0078D87D     76 3E                           JBE SHORT PathOfEx.0078D8BD
	$+52     0078D87F     6A 00                           PUSH 0x0
	$+54     0078D881     8BCE                            MOV ECX, ESI
	$+56     0078D883     E8 B8000000                     CALL PathOfEx.0078D940
	$+5B     0078D888     8B8E B4000000                   MOV ECX, DWORD PTR DS:[ESI+0xB4]
	$+61     0078D88E     8B86 CC000000                   MOV EAX, DWORD PTR DS:[ESI+0xCC]
	$+67     0078D894     03CF                            ADD ECX, EDI
	$+69     0078D896     2B86 C8000000                   SUB EAX, DWORD PTR DS:[ESI+0xC8]
	$+6F     0078D89C     3BC8                            CMP ECX, EAX
	$+71     0078D89E     76 1D                           JBE SHORT PathOfEx.0078D8BD
	$+73     0078D8A0     51                              PUSH ECX
	$+74     0078D8A1     8BCE                            MOV ECX, ESI
	$+76     0078D8A3     E8 38060000                     CALL PathOfEx.0078DEE0
	$+7B     0078D8A8     84C0                            TEST AL, AL
	$+7D     0078D8AA     75 11                           JNZ SHORT PathOfEx.0078D8BD
	$+7F     0078D8AC     3886 B3000000                   CMP BYTE PTR DS:[ESI+0xB3], AL
	$+85     0078D8B2     75 32                           JNZ SHORT PathOfEx.0078D8E6
	$+87     0078D8B4     6A 0A                           PUSH 0xA
	$+89     0078D8B6     8BCE                            MOV ECX, ESI
	$+8B     0078D8B8     E8 83000000                     CALL PathOfEx.0078D940
	$+90     0078D8BD     8B86 B4000000                   MOV EAX, DWORD PTR DS:[ESI+0xB4]
	$+96     0078D8C3     0386 C8000000                   ADD EAX, DWORD PTR DS:[ESI+0xC8]
	$+9C     0078D8C9     57                              PUSH EDI
	$+9D     0078D8CA     FF75 08                         PUSH DWORD PTR SS:[EBP+0x8]
	$+A0     0078D8CD     50                              PUSH EAX
	$+A1     0078D8CE     E8 7DDF4F00                     CALL PathOfEx.00C8B850
	$+A6     0078D8D3     83C4 0C                         ADD ESP, 0xC
	$+A9     0078D8D6     8BC6                            MOV EAX, ESI
	$+AB     0078D8D8     01BE B4000000                   ADD DWORD PTR DS:[ESI+0xB4], EDI
	$+B1     0078D8DE     5F                              POP EDI                                                 ; ntdll_1z.7703F8D1
	$+B2     0078D8DF     5E                              POP ESI                                                 ; ntdll_1z.7703F8D1
	$+B3     0078D8E0     8BE5                            MOV ESP, EBP
	$+B5     0078D8E2     5D                              POP EBP                                                 ; ntdll_1z.7703F8D1
	$+B6     0078D8E3     C2 0800                         RETN 0x8
	$+B9     0078D8E6     56                              PUSH ESI
	$+BA     0078D8E7     8D4C24 0C                       LEA ECX, DWORD PTR SS:[ESP+0xC]
	$+BE     0078D8EB     E8 D0F4FFFF                     CALL PathOfEx.0078CDC0
	$+C3     0078D8F0     68 1C5CFC00                     PUSH PathOfEx.00FC5C1C
	$+C8     0078D8F5     8D4424 0C                       LEA EAX, DWORD PTR SS:[ESP+0xC]
	$+CC     0078D8F9     50                              PUSH EAX
	$+CD     0078D8FA     E8 74E84F00                     CALL PathOfEx.00C8C173
	$+D2     0078D8FF     CC                              INT3
	$+D3     0078D900     56                              PUSH ESI
	$+D4     0078D901     8B7424 08                       MOV ESI, DWORD PTR SS:[ESP+0x8]
	$+D8     0078D905     0F57C0                          XORPS XMM0, XMM0
	$+DB     0078D908     57                              PUSH EDI
	$+DC     0078D909     8BF9                            MOV EDI, ECX
	$+DE     0078D90B     8D47 04                         LEA EAX, DWORD PTR DS:[EDI+0x4]
	$+E1     0078D90E     50                              PUSH EAX
	$+E2     0078D90F     C707 E8AFDD00                   MOV DWORD PTR DS:[EDI], PathOfEx.00DDAFE8               ; ASCII "罢F"
	$+E8     0078D915     660FD600                        MOVQ QWORD PTR DS:[EAX], XMM0
	$+EC     0078D919     8D46 04                         LEA EAX, DWORD PTR DS:[ESI+0x4]
	$+EF     0078D91C     50                              PUSH EAX
	$+F0     0078D91D     E8 82D74F00                     CALL PathOfEx.00C8B0A4
	$+F5     0078D922     83C4 08                         ADD ESP, 0x8
	$+F8     0078D925     C707 5012ED00                   MOV DWORD PTR DS:[EDI], PathOfEx.00ED1250
	$+FE     0078D92B     8B46 0C                         MOV EAX, DWORD PTR DS:[ESI+0xC]
	$+101    0078D92E     8947 0C                         MOV DWORD PTR DS:[EDI+0xC], EAX
	$+104    0078D931     8BC7                            MOV EAX, EDI
	$+106    0078D933     5F                              POP EDI                                                 ; ntdll_1z.7703F8D1
	$+107    0078D934     5E                              POP ESI                                                 ; ntdll_1z.7703F8D1
	$+108    0078D935     C2 0400                         RETN 0x4
	*/
	addr_tmp = scan_exe_.ScanAddr("8b????8b??????????2b??????????3b??76??57e8????????84??75??6a??ff??ff??????????66", -0xd);
	assert(addr_tmp);
	ofs << "pt_cdword	g_c_BuildPacket_ = " << ExeAddr2Offset(addr_tmp) << ";	//紧挨着的上面那个函数就是读取收包字段内容的函数，即read，而这个函数是write。" << FC_OFstream_EndlLog;

	/*
	$-482    008C9530     55                       PUSH EBP
	$-481    008C9531     8D6C24 C4                LEA EBP, DWORD PTR SS:[ESP-0x3C]
	$-47D    008C9535     83EC 3C                  SUB ESP, 0x3C
	$-47A    008C9538     6A FF                    PUSH -0x1
	$-478    008C953A     68 D5B3D500              PUSH PathOfEx.00D5B3D5
	$-473    008C953F     64:A1 00000000           MOV EAX, DWORD PTR FS:[0]
	$-46D    008C9545     50                       PUSH EAX
	$-46C    008C9546     64:8925 00000000         MOV DWORD PTR FS:[0], ESP
	$-465    008C954D     81EC B0000000            SUB ESP, 0xB0
	$-45F    008C9553     53                       PUSH EBX
	$-45E    008C9554     8BD9                     MOV EBX, ECX
	$-45C    008C9556     C745 30 00000000         MOV DWORD PTR SS:[EBP+0x30], 0x0
	$-455    008C955D     56                       PUSH ESI
	$-454    008C955E     57                       PUSH EDI
	$-453    008C955F     8B8B 9C3A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A9C]
	$-44D    008C9565     85C9                     TEST ECX, ECX
	$-44B    008C9567     0F84 1C020000            JE PathOfEx.008C9789
	$-445    008C956D     8079 38 00               CMP BYTE PTR DS:[ECX+0x38], 0x0
	$-441    008C9571     75 10                    JNZ SHORT PathOfEx.008C9583
	$-43F    008C9573     83C1 3C                  ADD ECX, 0x3C
	$-43C    008C9576     E8 35DA1800              CALL PathOfEx.00A56FB0
	$-437    008C957B     84C0                     TEST AL, AL
	$-435    008C957D     0F84 06020000            JE PathOfEx.008C9789
	$-42F    008C9583     8BB3 9C3A0000            MOV ESI, DWORD PTR DS:[EBX+0x3A9C]
	$-429    008C9589     807E 38 00               CMP BYTE PTR DS:[ESI+0x38], 0x0
	$-425    008C958D     75 24                    JNZ SHORT PathOfEx.008C95B3
	$-423    008C958F     8B46 40                  MOV EAX, DWORD PTR DS:[ESI+0x40]
	$-420    008C9592     85C0                     TEST EAX, EAX
	$-41E    008C9594     74 19                    JE SHORT PathOfEx.008C95AF
	$-41C    008C9596     6A FF                    PUSH -0x1
	$-41A    008C9598     50                       PUSH EAX
	$-419    008C9599     FF15 BC50D800            CALL NEAR DWORD PTR DS:[0xD850BC]
	$-413    008C959F     FF76 40                  PUSH DWORD PTR DS:[ESI+0x40]
	$-410    008C95A2     FF15 AC50D800            CALL NEAR DWORD PTR DS:[0xD850AC]
	$-40A    008C95A8     C746 40 00000000         MOV DWORD PTR DS:[ESI+0x40], 0x0
	$-403    008C95AF     C646 38 01               MOV BYTE PTR DS:[ESI+0x38], 0x1
	$-3FF    008C95B3     8B46 30                  MOV EAX, DWORD PTR DS:[ESI+0x30]
	$-3FC    008C95B6     8945 38                  MOV DWORD PTR SS:[EBP+0x38], EAX
	$-3F9    008C95B9     8B83 9C3A0000            MOV EAX, DWORD PTR DS:[EBX+0x3A9C]
	$-3F3    008C95BF     C785 44FFFFFF 7412EE00   MOV DWORD PTR SS:[EBP-0xBC], PathOfEx.00EE1274
	$-3E9    008C95C9     C785 54FFFFFF AC58EA00   MOV DWORD PTR SS:[EBP-0xAC], PathOfEx.00EA58AC
	$-3DF    008C95D3     C745 AC 24C5DD00         MOV DWORD PTR SS:[EBP-0x54], PathOfEx.00DDC524     ; ASCII "悫F"
	$-3D8    008C95DA     8B70 34                  MOV ESI, DWORD PTR DS:[EAX+0x34]
	$-3D5    008C95DD     8975 18                  MOV DWORD PTR SS:[EBP+0x18], ESI
	$-3D2    008C95E0     51                       PUSH ECX
	$-3D1    008C95E1     8D85 5CFFFFFF            LEA EAX, DWORD PTR SS:[EBP-0xA4]
	$-3CB    008C95E7     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-3C4    008C95EE     50                       PUSH EAX
	$-3C3    008C95EF     8D8D 44FFFFFF            LEA ECX, DWORD PTR SS:[EBP-0xBC]
	$-3BD    008C95F5     C745 30 01000000         MOV DWORD PTR SS:[EBP+0x30], 0x1
	$-3B6    008C95FC     E8 8F4DBAFF              CALL PathOfEx.0046E390
	$-3B1    008C9601     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-3AA    008C9608     8B85 44FFFFFF            MOV EAX, DWORD PTR SS:[EBP-0xBC]
	$-3A4    008C960E     6A 03                    PUSH 0x3
	$-3A2    008C9610     8B40 04                  MOV EAX, DWORD PTR DS:[EAX+0x4]
	$-39F    008C9613     C78405 44FFFFFF B86DE600 MOV DWORD PTR SS:[EBP+EAX-0xBC], PathOfEx.00E66DB8 ; ASCII "蹒F"
	$-394    008C961E     8B85 44FFFFFF            MOV EAX, DWORD PTR SS:[EBP-0xBC]
	$-38E    008C9624     8B48 04                  MOV ECX, DWORD PTR DS:[EAX+0x4]
	$-38B    008C9627     8D41 98                  LEA EAX, DWORD PTR DS:[ECX-0x68]
	$-388    008C962A     89840D 40FFFFFF          MOV DWORD PTR SS:[EBP+ECX-0xC0], EAX
	$-381    008C9631     8D8D 5CFFFFFF            LEA ECX, DWORD PTR SS:[EBP-0xA4]
	$-37B    008C9637     E8 3453BAFF              CALL PathOfEx.0046E970
	$-376    008C963C     68 BC00EF00              PUSH PathOfEx.00EF00BC                             ; UNICODE "ms"
	$-371    008C9641     56                       PUSH ESI
	$-370    008C9642     8D85 54FFFFFF            LEA EAX, DWORD PTR SS:[EBP-0xAC]
	$-36A    008C9648     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4], 0x2
	$-363    008C964F     68 7000EF00              PUSH PathOfEx.00EF0070                             ; UNICODE "Connect time to instance server was "
	$-35E    008C9654     50                       PUSH EAX
	$-35D    008C9655     E8 6644BBFF              CALL PathOfEx.0047DAC0
	$-358    008C965A     83C4 08                  ADD ESP, 0x8
	$-355    008C965D     8BC8                     MOV ECX, EAX
	$-353    008C965F     E8 CCD0BAFF              CALL PathOfEx.00476730
	$-34E    008C9664     50                       PUSH EAX
	$-34D    008C9665     E8 5644BBFF              CALL PathOfEx.0047DAC0
	$-348    008C966A     83C4 08                  ADD ESP, 0x8
	$-345    008C966D     8D45 00                  LEA EAX, DWORD PTR SS:[EBP]
	$-342    008C9670     8D8D 44FFFFFF            LEA ECX, DWORD PTR SS:[EBP-0xBC]
	$-33C    008C9676     50                       PUSH EAX
	$-33B    008C9677     E8 2444BBFF              CALL PathOfEx.0047DAA0
	$-336    008C967C     8BF0                     MOV ESI, EAX
	$-334    008C967E     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4], 0x3
	$-330    008C9682     FF15 CC50D800            CALL NEAR DWORD PTR DS:[0xD850CC]                  ; kernel32.GetTickCount
	$-32A    008C9688     8BF8                     MOV EDI, EAX
	$-328    008C968A     8D45 2C                  LEA EAX, DWORD PTR SS:[EBP+0x2C]
	$-325    008C968D     50                       PUSH EAX
	$-324    008C968E     E8 247A3D00              CALL PathOfEx.00CA10B7
	$-31F    008C9693     64:8B0D 2C000000         MOV ECX, DWORD PTR FS:[0x2C]
	$-318    008C969A     83C4 04                  ADD ESP, 0x4
	$-315    008C969D     8B09                     MOV ECX, DWORD PTR DS:[ECX]
	$-313    008C969F     56                       PUSH ESI
	$-312    008C96A0     83EC 0C                  SUB ESP, 0xC
	$-30F    008C96A3     8B49 18                  MOV ECX, DWORD PTR DS:[ECX+0x18]
	$-30C    008C96A6     6A 03                    PUSH 0x3
	$-30A    008C96A8     57                       PUSH EDI
	$-309    008C96A9     FF75 30                  PUSH DWORD PTR SS:[EBP+0x30]
	$-306    008C96AC     FF75 2C                  PUSH DWORD PTR SS:[EBP+0x2C]
	$-303    008C96AF     68 19010000              PUSH 0x119
	$-2FE    008C96B4     E8 2740BBFF              CALL PathOfEx.0047D6E0
	$-2F9    008C96B9     8B45 14                  MOV EAX, DWORD PTR SS:[EBP+0x14]
	$-2F6    008C96BC     83F8 08                  CMP EAX, 0x8
	$-2F3    008C96BF     72 0F                    JB SHORT PathOfEx.008C96D0
	$-2F1    008C96C1     40                       INC EAX
	$-2F0    008C96C2     6A 02                    PUSH 0x2
	$-2EE    008C96C4     50                       PUSH EAX
	$-2ED    008C96C5     FF75 00                  PUSH DWORD PTR SS:[EBP]
	$-2EA    008C96C8     E8 4391B9FF              CALL PathOfEx.00462810
	$-2E5    008C96CD     83C4 0C                  ADD ESP, 0xC
	$-2E2    008C96D0     8D4D AC                  LEA ECX, DWORD PTR SS:[EBP-0x54]
	$-2DF    008C96D3     E8 C835BAFF              CALL PathOfEx.0046CCA0
	$-2DA    008C96D8     8D45 AC                  LEA EAX, DWORD PTR SS:[EBP-0x54]
	$-2D7    008C96DB     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4], 0x4
	$-2D0    008C96E2     50                       PUSH EAX
	$-2CF    008C96E3     C745 AC 9CC4DD00         MOV DWORD PTR SS:[EBP-0x54], PathOfEx.00DDC49C     ; ASCII "疴F"
	$-2C8    008C96EA     E8 3F6C3900              CALL PathOfEx.00C6032E
	$-2C3    008C96EF     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-2BC    008C96F6     83C4 04                  ADD ESP, 0x4
	$-2B9    008C96F9     8B8B 943A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A94]
	$-2B3    008C96FF     83B9 E8010000 03         CMP DWORD PTR DS:[ECX+0x1E8], 0x3
	$-2AC    008C9706     75 2F                    JNZ SHORT PathOfEx.008C9737
	$-2AA    008C9708     A1 48CFFD00              MOV EAX, DWORD PTR DS:[0xFDCF48]
	$-2A5    008C970D     83F8 02                  CMP EAX, 0x2
	$-2A2    008C9710     75 0A                    JNZ SHORT PathOfEx.008C971C
	$-2A0    008C9712     837D 18 5A               CMP DWORD PTR SS:[EBP+0x18], 0x5A
	$-29C    008C9716     76 19                    JBE SHORT PathOfEx.008C9731
	$-29A    008C9718     33C0                     XOR EAX, EAX
	$-298    008C971A     EB 10                    JMP SHORT PathOfEx.008C972C
	$-296    008C971C     85C0                     TEST EAX, EAX
	$-294    008C971E     75 11                    JNZ SHORT PathOfEx.008C9731
	$-292    008C9720     837D 18 46               CMP DWORD PTR SS:[EBP+0x18], 0x46
	$-28E    008C9724     BA 02000000              MOV EDX, 0x2
	$-289    008C9729     0F42C2                   CMOVB EAX, EDX
	$-286    008C972C     A3 48CFFD00              MOV DWORD PTR DS:[0xFDCF48], EAX
	$-281    008C9731     8981 E8010000            MOV DWORD PTR DS:[ECX+0x1E8], EAX
	$-27B    008C9737     8B45 38                  MOV EAX, DWORD PTR SS:[EBP+0x38]
	$-278    008C973A     85C0                     TEST EAX, EAX
	$-276    008C973C     74 0F                    JE SHORT PathOfEx.008C974D
	$-274    008C973E     8BCB                     MOV ECX, EBX
	$-272    008C9740     8983 983A0000            MOV DWORD PTR DS:[EBX+0x3A98], EAX
	$-26C    008C9746     E8 E5FCFFFF              CALL PathOfEx.008C9430
	$-267    008C974B     EB 16                    JMP SHORT PathOfEx.008C9763
	$-265    008C974D     B8 83000000              MOV EAX, 0x83
	$-260    008C9752     C783 343B0000 01000000   MOV DWORD PTR DS:[EBX+0x3B34], 0x1
	$-256    008C975C     66:8983 803B0000         MOV WORD PTR DS:[EBX+0x3B80], AX
	$-24F    008C9763     8BB3 9C3A0000            MOV ESI, DWORD PTR DS:[EBX+0x3A9C]
	$-249    008C9769     C783 9C3A0000 00000000   MOV DWORD PTR DS:[EBX+0x3A9C], 0x0
	$-23F    008C9773     85F6                     TEST ESI, ESI
	$-23D    008C9775     74 12                    JE SHORT PathOfEx.008C9789
	$-23B    008C9777     8BCE                     MOV ECX, ESI
	$-239    008C9779     E8 D2D61800              CALL PathOfEx.00A56E50
	$-234    008C977E     6A 50                    PUSH 0x50
	$-232    008C9780     56                       PUSH ESI
	$-231    008C9781     E8 CE903A00              CALL PathOfEx.00C72854
	$-22C    008C9786     83C4 08                  ADD ESP, 0x8
	$-229    008C9789     8B83 CC3E0000            MOV EAX, DWORD PTR DS:[EBX+0x3ECC]
	$-223    008C978F     8B75 4C                  MOV ESI, DWORD PTR SS:[EBP+0x4C]
	$-220    008C9792     85C0                     TEST EAX, EAX
	$-21E    008C9794     74 18                    JE SHORT PathOfEx.008C97AE
	$-21C    008C9796     3BF0                     CMP ESI, EAX
	$-21A    008C9798     76 0C                    JBE SHORT PathOfEx.008C97A6
	$-218    008C979A     C783 CC3E0000 00000000   MOV DWORD PTR DS:[EBX+0x3ECC], 0x0
	$-20E    008C97A4     EB 08                    JMP SHORT PathOfEx.008C97AE
	$-20C    008C97A6     2BC6                     SUB EAX, ESI
	$-20A    008C97A8     8983 CC3E0000            MOV DWORD PTR DS:[EBX+0x3ECC], EAX
	$-204    008C97AE     8B83 343B0000            MOV EAX, DWORD PTR DS:[EBX+0x3B34]
	$-1FE    008C97B4     85C0                     TEST EAX, EAX
	$-1FC    008C97B6     0F84 30050000            JE PathOfEx.008C9CEC
	$-1F6    008C97BC     83F8 01                  CMP EAX, 0x1
	$-1F3    008C97BF     0F84 27050000            JE PathOfEx.008C9CEC
	$-1ED    008C97C5     8B8B 983A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A98]
	$-1E7    008C97CB     E8 E03DECFF              CALL PathOfEx.0078D5B0
	$-1E2    008C97D0     FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$-1DC    008C97D6     8D8B AC3A0000            LEA ECX, DWORD PTR DS:[EBX+0x3AAC]
	$-1D6    008C97DC     E8 FF8D1900              CALL PathOfEx.00A625E0
	$-1D1    008C97E1     83BB 343B0000 03         CMP DWORD PTR DS:[EBX+0x3B34], 0x3
	$-1CA    008C97E8     0F85 C5000000            JNZ PathOfEx.008C98B3
	$-1C4    008C97EE     80BB 12400000 00         CMP BYTE PTR DS:[EBX+0x4012], 0x0
	$-1BD    008C97F5     74 13                    JE SHORT PathOfEx.008C980A
	$-1BB    008C97F7     8B83 943A0000            MOV EAX, DWORD PTR DS:[EBX+0x3A94]
	$-1B5    008C97FD     83B8 E8010000 02         CMP DWORD PTR DS:[EAX+0x1E8], 0x2
	$-1AE    008C9804     0F85 A9000000            JNZ PathOfEx.008C98B3
	$-1A8    008C980A     66:0F6EC6                MOVD MM0, ESI
	$-1A4    008C980E     8BC6                     MOV EAX, ESI
	$-1A2    008C9810     F30FE6C0                 CVTDQ2PD XMM0, XMM0
	$-19E    008C9814     C1E8 1F                  SHR EAX, 0x1F
	$-19B    008C9817     8B8B 943A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A94]
	$-195    008C981D     51                       PUSH ECX
	$-194    008C981E     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$-192    008C9820     F20F5804C5 806AF300      ADDSD XMM0, QWORD PTR DS:[EAX*8+0xF36A80]
	$-189    008C9829     660F5AC0                 CVTPD2PS XMM0, XMM0
	$-185    008C982D     F3:0F5E05 2C61F300       DIVSS XMM0, DWORD PTR DS:[0xF3612C]
	$-17D    008C9835     F3:0F110424              MOVSS DWORD PTR SS:[ESP], XMM0
	$-178    008C983A     FF52 18                  CALL NEAR DWORD PTR DS:[EDX+0x18]
	$-175    008C983D     8BB3 B43E0000            MOV ESI, DWORD PTR DS:[EBX+0x3EB4]
	$-16F    008C9843     3BB3 B83E0000            CMP ESI, DWORD PTR DS:[EBX+0x3EB8]
	$-169    008C9849     74 68                    JE SHORT PathOfEx.008C98B3
	$-167    008C984B     8B4D 4C                  MOV ECX, DWORD PTR SS:[EBP+0x4C]
	$-164    008C984E     8D7E 2C                  LEA EDI, DWORD PTR DS:[ESI+0x2C]
	$-161    008C9851     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-15F    008C9853     3BC1                     CMP EAX, ECX
	$-15D    008C9855     77 4A                    JA SHORT PathOfEx.008C98A1
	$-15B    008C9857     8B0F                     MOV ECX, DWORD PTR DS:[EDI]
	$-159    008C9859     85C9                     TEST ECX, ECX
	$-157    008C985B     0F94C0                   SETE AL
	$-154    008C985E     84C0                     TEST AL, AL
	$-152    008C9860     0F85 00010000            JNZ PathOfEx.008C9966
	$-14C    008C9866     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$-14A    008C9868     FF50 08                  CALL NEAR DWORD PTR DS:[EAX+0x8]
	$-147    008C986B     8B93 B83E0000            MOV EDX, DWORD PTR DS:[EBX+0x3EB8]
	$-141    008C9871     8D4F 04                  LEA ECX, DWORD PTR DS:[EDI+0x4]
	$-13E    008C9874     C645 38 00               MOV BYTE PTR SS:[EBP+0x38], 0x0
	$-13A    008C9878     FF75 38                  PUSH DWORD PTR SS:[EBP+0x38]
	$-137    008C987B     56                       PUSH ESI
	$-136    008C987C     E8 6FF80000              CALL PathOfEx.008D90F0
	$-131    008C9881     8B93 B83E0000            MOV EDX, DWORD PTR DS:[EBX+0x3EB8]
	$-12B    008C9887     83C4 04                  ADD ESP, 0x4
	$-128    008C988A     8D4A D0                  LEA ECX, DWORD PTR DS:[EDX-0x30]
	$-125    008C988D     E8 8E64BDFF              CALL PathOfEx.0049FD20
	$-120    008C9892     8B4D 4C                  MOV ECX, DWORD PTR SS:[EBP+0x4C]
	$-11D    008C9895     83C4 04                  ADD ESP, 0x4
	$-11A    008C9898     8383 B83E0000 D0         ADD DWORD PTR DS:[EBX+0x3EB8], -0x30
	$-113    008C989F     EB 0A                    JMP SHORT PathOfEx.008C98AB
	$-111    008C98A1     2BC1                     SUB EAX, ECX
	$-10F    008C98A3     8906                     MOV DWORD PTR DS:[ESI], EAX
	$-10D    008C98A5     83C6 30                  ADD ESI, 0x30
	$-10A    008C98A8     83C7 30                  ADD EDI, 0x30
	$-107    008C98AB     3BB3 B83E0000            CMP ESI, DWORD PTR DS:[EBX+0x3EB8]
	$-101    008C98B1   ^ 75 9E                    JNZ SHORT PathOfEx.008C9851
	$-FF     008C98B3     8D45 34                  LEA EAX, DWORD PTR SS:[EBP+0x34]
	$-FC     008C98B6     50                       PUSH EAX
	$-FB     008C98B7     FF15 DC52D800            CALL NEAR DWORD PTR DS:[0xD852DC]
	$-F5     008C98BD     FF35 34670001            PUSH DWORD PTR DS:[0x1006734]
	$-EF     008C98C3     FF35 30670001            PUSH DWORD PTR DS:[0x1006730]
	$-E9     008C98C9     FF75 38                  PUSH DWORD PTR SS:[EBP+0x38]
	$-E6     008C98CC     FF75 34                  PUSH DWORD PTR SS:[EBP+0x34]
	$-E3     008C98CF     E8 9C993A00              CALL PathOfEx.00C73270
	$-DE     008C98D4     83BB 343B0000 03         CMP DWORD PTR DS:[EBX+0x3B34], 0x3
	$-D7     008C98DB     8BF0                     MOV ESI, EAX
	$-D5     008C98DD     8BFA                     MOV EDI, EDX
	$-D3     008C98DF     0F85 9E010000            JNZ PathOfEx.008C9A83
	$-CD     008C98E5     80BB 903B0000 00         CMP BYTE PTR DS:[EBX+0x3B90], 0x0
	$-C6     008C98EC     8B8B 883B0000            MOV ECX, DWORD PTR DS:[EBX+0x3B88]
	$-C0     008C98F2     8B83 8C3B0000            MOV EAX, DWORD PTR DS:[EBX+0x3B8C]
	$-BA     008C98F8     74 71                    JE SHORT PathOfEx.008C996B
	$-B8     008C98FA     81C1 D0070000            ADD ECX, 0x7D0
	$-B2     008C9900     83D0 00                  ADC EAX, 0x0
	$-AF     008C9903     3BF8                     CMP EDI, EAX
	$-AD     008C9905     0F8C D5000000            JL PathOfEx.008C99E0
	$-A7     008C990B     7F 08                    JG SHORT PathOfEx.008C9915
	$-A5     008C990D     3BF1                     CMP ESI, ECX
	$-A3     008C990F     0F82 CB000000            JB PathOfEx.008C99E0
	$-9D     008C9915     B8 0F000000              MOV EAX, 0xF
	$-98     008C991A     C745 28 700BEF00         MOV DWORD PTR SS:[EBP+0x28], PathOfEx.00EF0B70
	$-91     008C9921     66:8945 2C               MOV WORD PTR SS:[EBP+0x2C], AX
	$-8D     008C9925     8D45 28                  LEA EAX, DWORD PTR SS:[EBP+0x28]
	$-8A     008C9928     C745 FC 05000000         MOV DWORD PTR SS:[EBP-0x4], 0x5
	$-83     008C992F     50                       PUSH EAX
	$-82     008C9930     FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$-7C     008C9936     C745 30 00000000         MOV DWORD PTR SS:[EBP+0x30], 0x0
	$-75     008C993D     E8 CE7BDEFF              CALL PathOfEx.006B1510
	$-70     008C9942     8BC8                     MOV ECX, EAX
	$-6E     008C9944     E8 97AF0100              CALL PathOfEx.008E48E0
	$-69     008C9949     8B8B 983A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A98]
	$-63     008C994F     6A 00                    PUSH 0x0
	$-61     008C9951     E8 EA3FECFF              CALL PathOfEx.0078D940
	$-5C     008C9956     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-55     008C995D     C745 28 DC0BEF00         MOV DWORD PTR SS:[EBP+0x28], PathOfEx.00EF0BDC
	$-4E     008C9964     EB 6E                    JMP SHORT PathOfEx.008C99D4
	$-4C     008C9966     E8 AF683900              CALL PathOfEx.00C6021A
	$-47     008C996B     81C1 F4010000            ADD ECX, 0x1F4
	$-41     008C9971     83D0 00                  ADC EAX, 0x0
	$-3E     008C9974     3BF8                     CMP EDI, EAX
	$-3C     008C9976     7C 68                    JL SHORT PathOfEx.008C99E0
	$-3A     008C9978     7F 04                    JG SHORT PathOfEx.008C997E
	$-38     008C997A     3BF1                     CMP ESI, ECX
	$-36     008C997C     72 62                    JB SHORT PathOfEx.008C99E0
	$-34     008C997E     B8 0F000000              MOV EAX, 0xF
	$-2F     008C9983     C745 1C 700BEF00         MOV DWORD PTR SS:[EBP+0x1C], PathOfEx.00EF0B70
	$-28     008C998A     66:8945 20               MOV WORD PTR SS:[EBP+0x20], AX
	$-24     008C998E     8D45 1C                  LEA EAX, DWORD PTR SS:[EBP+0x1C]
	$-21     008C9991     C745 FC 06000000         MOV DWORD PTR SS:[EBP-0x4], 0x6
	$-1A     008C9998     50                       PUSH EAX
	$-19     008C9999     FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$-13     008C999F     C745 24 00000000         MOV DWORD PTR SS:[EBP+0x24], 0x0
	$-C      008C99A6     E8 657BDEFF              CALL PathOfEx.006B1510
	$-7      008C99AB     8BC8                     MOV ECX, EAX
	$-5      008C99AD     E8 2EAF0100              CALL PathOfEx.008E48E0
	$ ==>    008C99B2     8B8B 983A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A98]                 ; InGameState时的发包对象偏移
	$+6      008C99B8     6A 00                    PUSH 0x0                                           ; msg_id=0x100，猜测是心跳包
	$+8      008C99BA     E8 813FECFF              CALL PathOfEx.0078D940                             ; call发包
	$+D      008C99BF     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1                   ; 往上跟踪，能够追踪到GameBase
	$+14     008C99C6     C683 903B0000 01         MOV BYTE PTR DS:[EBX+0x3B90], 0x1
	$+1B     008C99CD     C745 1C DC0BEF00         MOV DWORD PTR SS:[EBP+0x1C], PathOfEx.00EF0BDC
	$+22     008C99D4     89BB 8C3B0000            MOV DWORD PTR DS:[EBX+0x3B8C], EDI
	$+28     008C99DA     89B3 883B0000            MOV DWORD PTR DS:[EBX+0x3B88], ESI
	$+2E     008C99E0     8B93 983A0000            MOV EDX, DWORD PTR DS:[EBX+0x3A98]
	$+34     008C99E6     837A 58 00               CMP DWORD PTR DS:[EDX+0x58], 0x0
	$+38     008C99EA     0F95C0                   SETNE AL
	$+3B     008C99ED     84C0                     TEST AL, AL
	$+3D     008C99EF     74 49                    JE SHORT PathOfEx.008C9A3A
	$+3F     008C99F1     8B4A 58                  MOV ECX, DWORD PTR DS:[EDX+0x58]
	$+42     008C99F4     8B81 30010000            MOV EAX, DWORD PTR DS:[ECX+0x130]                  ; ntdll_12.77D5E38C
	$+48     008C99FA     3B81 34010000            CMP EAX, DWORD PTR DS:[ECX+0x134]                  ; setupapi.759E7B49
	$+4E     008C9A00     0F94C0                   SETE AL
	$+51     008C9A03     84C0                     TEST AL, AL
	$+53     008C9A05     75 33                    JNZ SHORT PathOfEx.008C9A3A
	$+55     008C9A07     8D45 0C                  LEA EAX, DWORD PTR SS:[EBP+0xC]
	$+58     008C9A0A     50                       PUSH EAX
	$+59     008C9A0B     E8 70911900              CALL PathOfEx.00A62B80
	$+5E     008C9A10     8B4D 0C                  MOV ECX, DWORD PTR SS:[EBP+0xC]
	$+61     008C9A13     FF83 F83F0000            INC DWORD PTR DS:[EBX+0x3FF8]
	$+67     008C9A19     8B55 10                  MOV EDX, DWORD PTR SS:[EBP+0x10]                   ; PathOfEx.004671D2
	$+6A     008C9A1C     8B83 F83F0000            MOV EAX, DWORD PTR DS:[EBX+0x3FF8]
	$+70     008C9A22     2BD1                     SUB EDX, ECX
	$+72     008C9A24     51                       PUSH ECX
	$+73     008C9A25     A3 ECFBFE00              MOV DWORD PTR DS:[0xFEFBEC], EAX
	$+78     008C9A2A     E8 113AE3FF              CALL PathOfEx.006FD440
	$+7D     008C9A2F     83C4 04                  ADD ESP, 0x4
	$+80     008C9A32     8D4D 0C                  LEA ECX, DWORD PTR SS:[EBP+0xC]
	$+83     008C9A35     E8 16B9DEFF              CALL PathOfEx.006B5350
	$+88     008C9A3A     833D E8FBFE00 00         CMP DWORD PTR DS:[0xFEFBE8], 0x0
	$+8F     008C9A41     74 40                    JE SHORT PathOfEx.008C9A83
	$+91     008C9A43     8745 4C                  XCHG DWORD PTR SS:[EBP+0x4C], EAX
	$+94     008C9A46     8B83 F83F0000            MOV EAX, DWORD PTR DS:[EBX+0x3FF8]
	$+9A     008C9A4C     C705 E8FBFE00 00000000   MOV DWORD PTR DS:[0xFEFBE8], 0x0
	$+A4     008C9A56     3B05 ECFBFE00            CMP EAX, DWORD PTR DS:[0xFEFBEC]
	$+AA     008C9A5C     75 17                    JNZ SHORT PathOfEx.008C9A75
	$+AC     008C9A5E     8B83 983A0000            MOV EAX, DWORD PTR DS:[EBX+0x3A98]
	$+B2     008C9A64     8B48 58                  MOV ECX, DWORD PTR DS:[EAX+0x58]
	$+B5     008C9A67     0FB605 E4FBFE00          MOVZX EAX, BYTE PTR DS:[0xFEFBE4]
	$+BC     008C9A6E     50                       PUSH EAX
	$+BD     008C9A6F     51                       PUSH ECX
	$+BE     008C9A70     E8 6B901900              CALL PathOfEx.00A62AE0
	$+C3     008C9A75     FF35 E0FBFE00            PUSH DWORD PTR DS:[0xFEFBE0]
	$+C9     008C9A7B     E8 11B33C00              CALL PathOfEx.00C94D91
	$+CE     008C9A80     83C4 04                  ADD ESP, 0x4
	$+D1     008C9A83     8B83 B83C0000            MOV EAX, DWORD PTR DS:[EBX+0x3CB8]
	$+D7     008C9A89     85C0                     TEST EAX, EAX
	$+D9     008C9A8B     0F84 8B000000            JE PathOfEx.008C9B1C
	$+DF     008C9A91     8B40 04                  MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+E2     008C9A94     8D8B D83C0000            LEA ECX, DWORD PTR DS:[EBX+0x3CD8]
	$+E8     008C9A9A     8B40 20                  MOV EAX, DWORD PTR DS:[EAX+0x20]
	$+EB     008C9A9D     83C0 10                  ADD EAX, 0x10
	$+EE     008C9AA0     50                       PUSH EAX
	$+EF     008C9AA1     E8 3A780100              CALL PathOfEx.008E12E0
	$+F4     008C9AA6     8B83 B83C0000            MOV EAX, DWORD PTR DS:[EBX+0x3CB8]
	$+FA     008C9AAC     66:83B8 80000000 04      CMP WORD PTR DS:[EAX+0x80], 0x4
	$+102    008C9AB4     72 21                    JB SHORT PathOfEx.008C9AD7
	$+104    008C9AB6     83EC 18                  SUB ESP, 0x18
	$+107    008C9AB9     33C9                     XOR ECX, ECX
	$+109    008C9ABB     66:8988 80000000         MOV WORD PTR DS:[EAX+0x80], CX
	$+110    008C9AC2     8BCC                     MOV ECX, ESP
	$+112    008C9AC4     68 086EE600              PUSH PathOfEx.00E66E08
	$+117    008C9AC9     E8 42A3B9FF              CALL PathOfEx.00463E10
	$+11C    008C9ACE     6A 08                    PUSH 0x8
	$+11E    008C9AD0     8BCB                     MOV ECX, EBX
	$+120    008C9AD2     E8 A9740000              CALL PathOfEx.008D0F80
	$+125    008C9AD7     8B8B 983B0000            MOV ECX, DWORD PTR DS:[EBX+0x3B98]
	$+12B    008C9ADD     8B83 9C3B0000            MOV EAX, DWORD PTR DS:[EBX+0x3B9C]
	$+131    008C9AE3     81C1 88130000            ADD ECX, 0x1388
	$+137    008C9AE9     83D0 00                  ADC EAX, 0x0
	$+13A    008C9AEC     3BF8                     CMP EDI, EAX
	$+13C    008C9AEE     7C 2C                    JL SHORT PathOfEx.008C9B1C
	$+13E    008C9AF0     7F 04                    JG SHORT PathOfEx.008C9AF6
	$+140    008C9AF2     3BF1                     CMP ESI, ECX
	$+142    008C9AF4     72 26                    JB SHORT PathOfEx.008C9B1C
	$+144    008C9AF6     8B8B B83C0000            MOV ECX, DWORD PTR DS:[EBX+0x3CB8]
	$+14A    008C9AFC     89B3 983B0000            MOV DWORD PTR DS:[EBX+0x3B98], ESI
	$+150    008C9B02     89BB 9C3B0000            MOV DWORD PTR DS:[EBX+0x3B9C], EDI
	$+156    008C9B08     0FB781 80000000          MOVZX EAX, WORD PTR DS:[ECX+0x80]
	$+15D    008C9B0F     66:85C0                  TEST AX, AX
	$+160    008C9B12     74 08                    JE SHORT PathOfEx.008C9B1C
	$+162    008C9B14     48                       DEC EAX
	$+163    008C9B15     66:8981 80000000         MOV WORD PTR DS:[ECX+0x80], AX
	$+16A    008C9B1C     83BB 343B0000 01         CMP DWORD PTR DS:[EBX+0x3B34], 0x1
	$+171    008C9B23     75 10                    JNZ SHORT PathOfEx.008C9B35
	$+173    008C9B25     0FB783 803B0000          MOVZX EAX, WORD PTR DS:[EBX+0x3B80]
	$+17A    008C9B2C     8D4B 14                  LEA ECX, DWORD PTR DS:[EBX+0x14]
	$+17D    008C9B2F     50                       PUSH EAX
	$+17E    008C9B30     E8 7B84C2FF              CALL PathOfEx.004F1FB0
	$+183    008C9B35     83BB 343B0000 03         CMP DWORD PTR DS:[EBX+0x3B34], 0x3
	$+18A    008C9B3C     0F85 A0000000            JNZ PathOfEx.008C9BE2
	$+190    008C9B42     0FB783 BA3B0000          MOVZX EAX, WORD PTR DS:[EBX+0x3BBA]
	$+197    008C9B49     B9 FFFF0000              MOV ECX, 0xFFFF
	$+19C    008C9B4E     66:3BC1                  CMP AX, CX
	$+19F    008C9B51     0F84 8B000000            JE PathOfEx.008C9BE2
	$+1A5    008C9B57     8B7D 48                  MOV EDI, DWORD PTR SS:[EBP+0x48]
	$+1A8    008C9B5A     3BBB C43B0000            CMP EDI, DWORD PTR DS:[EBX+0x3BC4]
	$+1AE    008C9B60     0F8C 7C000000            JL PathOfEx.008C9BE2
	$+1B4    008C9B66     8B75 44                  MOV ESI, DWORD PTR SS:[EBP+0x44]
	$+1B7    008C9B69     7F 08                    JG SHORT PathOfEx.008C9B73
	$+1B9    008C9B6B     3BB3 C03B0000            CMP ESI, DWORD PTR DS:[EBX+0x3BC0]
	$+1BF    008C9B71     72 6F                    JB SHORT PathOfEx.008C9BE2
	$+1C1    008C9B73     B9 6E000000              MOV ECX, 0x6E
	$+1C6    008C9B78     C745 1C F00AEF00         MOV DWORD PTR SS:[EBP+0x1C], PathOfEx.00EF0AF0
	$+1CD    008C9B7F     66:894D 20               MOV WORD PTR SS:[EBP+0x20], CX
	$+1D1    008C9B83     C745 FC 07000000         MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+1D8    008C9B8A     66:8945 24               MOV WORD PTR SS:[EBP+0x24], AX
	$+1DC    008C9B8E     8A83 B83B0000            MOV AL, BYTE PTR DS:[EBX+0x3BB8]
	$+1E2    008C9B94     8845 26                  MOV BYTE PTR SS:[EBP+0x26], AL
	$+1E5    008C9B97     8D45 1C                  LEA EAX, DWORD PTR SS:[EBP+0x1C]
	$+1E8    008C9B9A     50                       PUSH EAX
	$+1E9    008C9B9B     FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$+1EF    008C9BA1     E8 6A79DEFF              CALL PathOfEx.006B1510
	$+1F4    008C9BA6     8BC8                     MOV ECX, EAX
	$+1F6    008C9BA8     E8 33AD0100              CALL PathOfEx.008E48E0
	$+1FB    008C9BAD     8B8B 983A0000            MOV ECX, DWORD PTR DS:[EBX+0x3A98]
	$+201    008C9BB3     6A 00                    PUSH 0x0
	$+203    008C9BB5     E8 863DECFF              CALL PathOfEx.0078D940
	$+208    008C9BBA     81C6 C8000000            ADD ESI, 0xC8
	$+20E    008C9BC0     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+215    008C9BC7     B8 FFFF0000              MOV EAX, 0xFFFF
	$+21A    008C9BCC     89B3 C03B0000            MOV DWORD PTR DS:[EBX+0x3BC0], ESI
	$+220    008C9BD2     83D7 00                  ADC EDI, 0x0
	$+223    008C9BD5     66:8983 BA3B0000         MOV WORD PTR DS:[EBX+0x3BBA], AX
	$+22A    008C9BDC     89BB C43B0000            MOV DWORD PTR DS:[EBX+0x3BC4], EDI
	$+230    008C9BE2     8B83 183F0000            MOV EAX, DWORD PTR DS:[EBX+0x3F18]
	$+236    008C9BE8     2383 1C3F0000            AND EAX, DWORD PTR DS:[EBX+0x3F1C]
	$+23C    008C9BEE     8B35 DC52D800            MOV ESI, DWORD PTR DS:[0xD852DC]
	$+242    008C9BF4     83F8 FF                  CMP EAX, -0x1
	$+245    008C9BF7     74 71                    JE SHORT PathOfEx.008C9C6A
	$+247    008C9BF9     8D45 44                  LEA EAX, DWORD PTR SS:[EBP+0x44]
	$+24A    008C9BFC     50                       PUSH EAX
	$+24B    008C9BFD     FFD6                     CALL NEAR ESI
	$+24D    008C9BFF     FF35 34670001            PUSH DWORD PTR DS:[0x1006734]
	$+253    008C9C05     FF35 30670001            PUSH DWORD PTR DS:[0x1006730]
	$+259    008C9C0B     FF75 48                  PUSH DWORD PTR SS:[EBP+0x48]
	$+25C    008C9C0E     FF75 44                  PUSH DWORD PTR SS:[EBP+0x44]
	$+25F    008C9C11     E8 5A963A00              CALL PathOfEx.00C73270
	$+264    008C9C16     3B93 1C3F0000            CMP EDX, DWORD PTR DS:[EBX+0x3F1C]
	$+26A    008C9C1C     7C 4C                    JL SHORT PathOfEx.008C9C6A
	$+26C    008C9C1E     7F 08                    JG SHORT PathOfEx.008C9C28
	$+26E    008C9C20     3B83 183F0000            CMP EAX, DWORD PTR DS:[EBX+0x3F18]
	$+274    008C9C26     72 42                    JB SHORT PathOfEx.008C9C6A
	$+276    008C9C28     FFB3 103F0000            PUSH DWORD PTR DS:[EBX+0x3F10]
	$+27C    008C9C2E     0FB783 0C3F0000          MOVZX EAX, WORD PTR DS:[EBX+0x3F0C]
	$+283    008C9C35     8D8B 14050000            LEA ECX, DWORD PTR DS:[EBX+0x514]
	$+289    008C9C3B     50                       PUSH EAX
	$+28A    008C9C3C     6A 01                    PUSH 0x1
	$+28C    008C9C3E     E8 6DC50000              CALL PathOfEx.008D61B0
	$+291    008C9C43     33C0                     XOR EAX, EAX
	$+293    008C9C45     C783 183F0000 FFFFFFFF   MOV DWORD PTR DS:[EBX+0x3F18], -0x1
	$+29D    008C9C4F     C783 1C3F0000 FFFFFFFF   MOV DWORD PTR DS:[EBX+0x3F1C], -0x1
	$+2A7    008C9C59     66:8983 0C3F0000         MOV WORD PTR DS:[EBX+0x3F0C], AX
	$+2AE    008C9C60     C783 103F0000 09000000   MOV DWORD PTR DS:[EBX+0x3F10], 0x9
	$+2B8    008C9C6A     8B83 283F0000            MOV EAX, DWORD PTR DS:[EBX+0x3F28]
	$+2BE    008C9C70     2383 2C3F0000            AND EAX, DWORD PTR DS:[EBX+0x3F2C]
	$+2C4    008C9C76     83F8 FF                  CMP EAX, -0x1
	$+2C7    008C9C79     74 71                    JE SHORT PathOfEx.008C9CEC
	$+2C9    008C9C7B     8D45 44                  LEA EAX, DWORD PTR SS:[EBP+0x44]
	$+2CC    008C9C7E     50                       PUSH EAX
	$+2CD    008C9C7F     FFD6                     CALL NEAR ESI
	$+2CF    008C9C81     FF35 34670001            PUSH DWORD PTR DS:[0x1006734]
	$+2D5    008C9C87     FF35 30670001            PUSH DWORD PTR DS:[0x1006730]
	$+2DB    008C9C8D     FF75 48                  PUSH DWORD PTR SS:[EBP+0x48]
	$+2DE    008C9C90     FF75 44                  PUSH DWORD PTR SS:[EBP+0x44]
	$+2E1    008C9C93     E8 D8953A00              CALL PathOfEx.00C73270
	$+2E6    008C9C98     3B93 2C3F0000            CMP EDX, DWORD PTR DS:[EBX+0x3F2C]
	$+2EC    008C9C9E     7C 4C                    JL SHORT PathOfEx.008C9CEC
	$+2EE    008C9CA0     7F 08                    JG SHORT PathOfEx.008C9CAA
	$+2F0    008C9CA2     3B83 283F0000            CMP EAX, DWORD PTR DS:[EBX+0x3F28]
	$+2F6    008C9CA8     72 42                    JB SHORT PathOfEx.008C9CEC
	$+2F8    008C9CAA     FFB3 243F0000            PUSH DWORD PTR DS:[EBX+0x3F24]
	$+2FE    008C9CB0     0FB783 203F0000          MOVZX EAX, WORD PTR DS:[EBX+0x3F20]
	$+305    008C9CB7     8D8B 14050000            LEA ECX, DWORD PTR DS:[EBX+0x514]
	$+30B    008C9CBD     50                       PUSH EAX
	$+30C    008C9CBE     6A 00                    PUSH 0x0
	$+30E    008C9CC0     E8 EBC40000              CALL PathOfEx.008D61B0
	$+313    008C9CC5     33C0                     XOR EAX, EAX
	$+315    008C9CC7     C783 283F0000 FFFFFFFF   MOV DWORD PTR DS:[EBX+0x3F28], -0x1
	$+31F    008C9CD1     C783 2C3F0000 FFFFFFFF   MOV DWORD PTR DS:[EBX+0x3F2C], -0x1
	$+329    008C9CDB     66:8983 203F0000         MOV WORD PTR DS:[EBX+0x3F20], AX
	$+330    008C9CE2     C783 243F0000 09000000   MOV DWORD PTR DS:[EBX+0x3F24], 0x9
	$+33A    008C9CEC     8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+33D    008C9CEF     5F                       POP EDI                                            ; 0D05F048
	$+33E    008C9CF0     5E                       POP ESI                                            ; 0D05F048
	$+33F    008C9CF1     64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$+346    008C9CF8     5B                       POP EBX                                            ; 0D05F048
	$+347    008C9CF9     8D65 3C                  LEA ESP, DWORD PTR SS:[EBP+0x3C]
	$+34A    008C9CFC     5D                       POP EBP                                            ; 0D05F048
	$+34B    008C9CFD     C2 0C00                  RETN 0xC
	$+34E    008C9D00     66:8B4424 04             MOV AX, WORD PTR SS:[ESP+0x4]
	$+353    008C9D05     56                       PUSH ESI
	$+354    008C9D06     8BB1 983A0000            MOV ESI, DWORD PTR DS:[ECX+0x3A98]                 ; PathOfEx.0064002E
	$+35A    008C9D0C     6A 01                    PUSH 0x1
	$+35C    008C9D0E     66:8981 803B0000         MOV WORD PTR DS:[ECX+0x3B80], AX
	$+363    008C9D15     C781 343B0000 01000000   MOV DWORD PTR DS:[ECX+0x3B34], 0x1
	$+36D    008C9D1F     FF36                     PUSH DWORD PTR DS:[ESI]
	$+36F    008C9D21     FF15 0455D800            CALL NEAR DWORD PTR DS:[0xD85504]
	$+375    008C9D27     C686 B0000000 00         MOV BYTE PTR DS:[ESI+0xB0], 0x0
	$+37C    008C9D2E     E8 DD77DEFF              CALL PathOfEx.006B1510
	$+381    008C9D33     5E                       POP ESI                                            ; 0D05F048
	$+382    008C9D34     C740 08 00000000         MOV DWORD PTR DS:[EAX+0x8], 0x0
	$+389    008C9D3B     8B40 04                  MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+38C    008C9D3E     C740 04 00000000         MOV DWORD PTR DS:[EAX+0x4], 0x0
	$+393    008C9D45     C2 0400                  RETN 0x4
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????6a??e8????????c7????????????c6????????????c7????????????89", 2);
	assert(op_dw_tmp);
	ofs << "pt_cdword	g_o_game_session_packet_ = " << *op_dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$ ==>    0069EF20      55                           PUSH EBP
	$+1      0069EF21      8BEC                         MOV EBP, ESP
	$+3      0069EF23      6A FF                        PUSH -0x1
	$+5      0069EF25      68 8086D300                  PUSH PathOfEx.00D38680
	$+A      0069EF2A      64:A1 00000000               MOV EAX, DWORD PTR FS:[0]
	$+10     0069EF30      50                           PUSH EAX
	$+11     0069EF31      64:8925 00000000             MOV DWORD PTR FS:[0], ESP
	$+18     0069EF38      83EC 40                      SUB ESP, 0x40
	$+1B     0069EF3B      53                           PUSH EBX
	$+1C     0069EF3C      8BC1                         MOV EAX, ECX
	$+1E     0069EF3E      8378 3C 00                   CMP DWORD PTR DS:[EAX+0x3C], 0x0
	$+22     0069EF42      56                           PUSH ESI                                         ; D3DX9_42.08CA89E8
	$+23     0069EF43      57                           PUSH EDI
	$+24     0069EF44      8D78 2C                      LEA EDI, DWORD PTR DS:[EAX+0x2C]
	$+27     0069EF47      8945 F0                      MOV DWORD PTR SS:[EBP-0x10], EAX
	$+2A     0069EF4A      0F94C0                       SETE AL
	$+2D     0069EF4D      83CB FF                      OR EBX, -0x1
	$+30     0069EF50      84C0                         TEST AL, AL
	$+32     0069EF52      0F85 99010000                JNZ PathOfEx.0069F0F1
	$+38     0069EF58      8D45 CC                      LEA EAX, DWORD PTR SS:[EBP-0x34]
	$+3B     0069EF5B      8BCF                         MOV ECX, EDI
	$+3D     0069EF5D      50                           PUSH EAX
	$+3E     0069EF5E      E8 5DFEE1FF                  CALL PathOfEx.004BEDC0
	$+43     0069EF63      8BD0                         MOV EDX, EAX
	$+45     0069EF65      8B02                         MOV EAX, DWORD PTR DS:[EDX]                      ; D3DX9_42.08CA89E8
	$+47     0069EF67      85C0                         TEST EAX, EAX
	$+49     0069EF69      74 02                        JE SHORT PathOfEx.0069EF6D
	$+4B     0069EF6B      8B00                         MOV EAX, DWORD PTR DS:[EAX]                      ; PathOfEx.00F279A4
	$+4D     0069EF6D      8B48 08                      MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+50     0069EF70      8B40 04                      MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+53     0069EF73      49                           DEC ECX
	$+54     0069EF74      234A 08                      AND ECX, DWORD PTR DS:[EDX+0x8]
	$+57     0069EF77      8B0488                       MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+5A     0069EF7A      8BCF                         MOV ECX, EDI
	$+5C     0069EF7C      8B40 0C                      MOV EAX, DWORD PTR DS:[EAX+0xC]
	$+5F     0069EF7F      8945 E4                      MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+62     0069EF82      8D45 C0                      LEA EAX, DWORD PTR SS:[EBP-0x40]
	$+65     0069EF85      50                           PUSH EAX
	$+66     0069EF86      E8 35FEE1FF                  CALL PathOfEx.004BEDC0
	$+6B     0069EF8B      8BD0                         MOV EDX, EAX
	$+6D     0069EF8D      8B02                         MOV EAX, DWORD PTR DS:[EDX]                      ; D3DX9_42.08CA89E8
	$+6F     0069EF8F      85C0                         TEST EAX, EAX
	$+71     0069EF91      74 02                        JE SHORT PathOfEx.0069EF95
	$+73     0069EF93      8B00                         MOV EAX, DWORD PTR DS:[EAX]                      ; PathOfEx.00F279A4
	$+75     0069EF95      8B48 08                      MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+78     0069EF98      8B40 04                      MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+7B     0069EF9B      49                           DEC ECX
	$+7C     0069EF9C      234A 08                      AND ECX, DWORD PTR DS:[EDX+0x8]
	$+7F     0069EF9F      8B0488                       MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+82     0069EFA2      8B70 08                      MOV ESI, DWORD PTR DS:[EAX+0x8]
	$+85     0069EFA5      8B40 04                      MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+88     0069EFA8      8945 E0                      MOV DWORD PTR SS:[EBP-0x20], EAX
	$+8B     0069EFAB      85F6                         TEST ESI, ESI                                    ; D3DX9_42.08CA89E8
	$+8D     0069EFAD      74 04                        JE SHORT PathOfEx.0069EFB3
	$+8F     0069EFAF      F0:FF46 04                   LOCK INC DWORD PTR DS:[ESI+0x4]
	$+93     0069EFB3      8975 DC                      MOV DWORD PTR SS:[EBP-0x24], ESI                 ; D3DX9_42.08CA89E8
	$+96     0069EFB6      8945 D8                      MOV DWORD PTR SS:[EBP-0x28], EAX
	$+99     0069EFB9      8D45 B4                      LEA EAX, DWORD PTR SS:[EBP-0x4C]
	$+9C     0069EFBC      C745 FC 01000000             MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+A3     0069EFC3      50                           PUSH EAX
	$+A4     0069EFC4      8BCF                         MOV ECX, EDI
	$+A6     0069EFC6      E8 F5FDE1FF                  CALL PathOfEx.004BEDC0
	$+AB     0069EFCB      8BD0                         MOV EDX, EAX
	$+AD     0069EFCD      8B0A                         MOV ECX, DWORD PTR DS:[EDX]                      ; D3DX9_42.08CA89E8
	$+AF     0069EFCF      85C9                         TEST ECX, ECX
	$+B1     0069EFD1      75 04                        JNZ SHORT PathOfEx.0069EFD7
	$+B3     0069EFD3      33C0                         XOR EAX, EAX
	$+B5     0069EFD5      EB 02                        JMP SHORT PathOfEx.0069EFD9
	$+B7     0069EFD7      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+B9     0069EFD9      8B48 08                      MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+BC     0069EFDC      8B40 04                      MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+BF     0069EFDF      49                           DEC ECX
	$+C0     0069EFE0      234A 08                      AND ECX, DWORD PTR DS:[EDX+0x8]
	$+C3     0069EFE3      8B0488                       MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+C6     0069EFE6      8B4F 08                      MOV ECX, DWORD PTR DS:[EDI+0x8]                  ; d3d9.6EB4D187
	$+C9     0069EFE9      49                           DEC ECX
	$+CA     0069EFEA      234F 0C                      AND ECX, DWORD PTR DS:[EDI+0xC]
	$+CD     0069EFED      8B10                         MOV EDX, DWORD PTR DS:[EAX]                      ; PathOfEx.00F279A4
	$+CF     0069EFEF      8B47 04                      MOV EAX, DWORD PTR DS:[EDI+0x4]
	$+D2     0069EFF2      8955 EC                      MOV DWORD PTR SS:[EBP-0x14], EDX
	$+D5     0069EFF5      8B0488                       MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+D8     0069EFF8      C645 FC 02                   MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+DC     0069EFFC      8B48 08                      MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+DF     0069EFFF      894D E8                      MOV DWORD PTR SS:[EBP-0x18], ECX
	$+E2     0069F002      85C9                         TEST ECX, ECX
	$+E4     0069F004      74 21                        JE SHORT PathOfEx.0069F027
	$+E6     0069F006      8BC3                         MOV EAX, EBX
	$+E8     0069F008      F0:0FC141 04                 LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+ED     0069F00D      75 18                        JNZ SHORT PathOfEx.0069F027
	$+EF     0069F00F      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+F1     0069F011      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; PathOfEx.00F279A4
	$+F3     0069F013      8B4D E8                      MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+F6     0069F016      8BC3                         MOV EAX, EBX
	$+F8     0069F018      F0:0FC141 08                 LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+FD     0069F01D      75 05                        JNZ SHORT PathOfEx.0069F024
	$+FF     0069F01F      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+101    0069F021      FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+104    0069F024      8B55 EC                      MOV EDX, DWORD PTR SS:[EBP-0x14]
	$+107    0069F027      C645 FC 01                   MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+10B    0069F02B      8347 10 FF                   ADD DWORD PTR DS:[EDI+0x10], -0x1
	$+10F    0069F02F      75 09                        JNZ SHORT PathOfEx.0069F03A
	$+111    0069F031      C747 0C 00000000             MOV DWORD PTR DS:[EDI+0xC], 0x0
	$+118    0069F038      EB 03                        JMP SHORT PathOfEx.0069F03D
	$+11A    0069F03A      FF47 0C                      INC DWORD PTR DS:[EDI+0xC]
	$+11D    0069F03D      8B45 E4                      MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+120    0069F040      83E8 00                      SUB EAX, 0x0
	$+123    0069F043      74 61                        JE SHORT PathOfEx.0069F0A6
	$+125    0069F045      83E8 01                      SUB EAX, 0x1
	$+128    0069F048      75 67                        JNZ SHORT PathOfEx.0069F0B1
	$+12A    0069F04A      83EC 08                      SUB ESP, 0x8
	$+12D    0069F04D      8BCC                         MOV ECX, ESP
	$+12F    0069F04F      894D E8                      MOV DWORD PTR SS:[EBP-0x18], ECX
	$+132    0069F052      C645 FC 03                   MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+136    0069F056      8901                         MOV DWORD PTR DS:[ECX], EAX
	$+138    0069F058      8941 04                      MOV DWORD PTR DS:[ECX+0x4], EAX
	$+13B    0069F05B      85F6                         TEST ESI, ESI                                    ; D3DX9_42.08CA89E8
	$+13D    0069F05D      74 04                        JE SHORT PathOfEx.0069F063
	$+13F    0069F05F      F0:FF46 04                   LOCK INC DWORD PTR DS:[ESI+0x4]
	$+143    0069F063      8B49 04                      MOV ECX, DWORD PTR DS:[ECX+0x4]
	$+146    0069F066      894D E4                      MOV DWORD PTR SS:[EBP-0x1C], ECX
	$+149    0069F069      85C9                         TEST ECX, ECX
	$+14B    0069F06B      74 1E                        JE SHORT PathOfEx.0069F08B
	$+14D    0069F06D      8BC3                         MOV EAX, EBX
	$+14F    0069F06F      F0:0FC141 04                 LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+154    0069F074      75 15                        JNZ SHORT PathOfEx.0069F08B
	$+156    0069F076      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+158    0069F078      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; PathOfEx.00F279A4
	$+15A    0069F07A      8B4D E4                      MOV ECX, DWORD PTR SS:[EBP-0x1C]
	$+15D    0069F07D      8BC3                         MOV EAX, EBX
	$+15F    0069F07F      F0:0FC141 08                 LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+164    0069F084      75 05                        JNZ SHORT PathOfEx.0069F08B
	$+166    0069F086      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+168    0069F088      FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+16B    0069F08B      8B45 E8                      MOV EAX, DWORD PTR SS:[EBP-0x18]
	$+16E    0069F08E      8B4D E0                      MOV ECX, DWORD PTR SS:[EBP-0x20]
	$+171    0069F091      6A 00                        PUSH 0x0
	$+173    0069F093      C645 FC 01                   MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+177    0069F097      8908                         MOV DWORD PTR DS:[EAX], ECX
	$+179    0069F099      8B4D F0                      MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+17C    0069F09C      8970 04                      MOV DWORD PTR DS:[EAX+0x4], ESI                  ; D3DX9_42.08CA89E8
	$+17F    0069F09F      E8 DCF9FFFF                  CALL PathOfEx.0069EA80
	$+184    0069F0A4      EB 0B                        JMP SHORT PathOfEx.0069F0B1
	$+186    0069F0A6      8B4D F0                      MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+189    0069F0A9      6A 00                        PUSH 0x0
	$+18B    0069F0AB      52                           PUSH EDX
	$+18C    0069F0AC      E8 FFF6FFFF                  CALL PathOfEx.0069E7B0
	$+191    0069F0B1      C745 FC 04000000             MOV DWORD PTR SS:[EBP-0x4], 0x4
	$+198    0069F0B8      85F6                         TEST ESI, ESI                                    ; D3DX9_42.08CA89E8
	$+19A    0069F0BA      74 1F                        JE SHORT PathOfEx.0069F0DB
	$+19C    0069F0BC      8BC3                         MOV EAX, EBX
	$+19E    0069F0BE      F0:0FC146 04                 LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+1A3    0069F0C3      75 16                        JNZ SHORT PathOfEx.0069F0DB
	$+1A5    0069F0C5      8B06                         MOV EAX, DWORD PTR DS:[ESI]                      ; D3DX9_42.08D7AB74
	$+1A7    0069F0C7      8BCE                         MOV ECX, ESI                                     ; D3DX9_42.08CA89E8
	$+1A9    0069F0C9      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; PathOfEx.00F279A4
	$+1AB    0069F0CB      8BC3                         MOV EAX, EBX
	$+1AD    0069F0CD      F0:0FC146 08                 LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
	$+1B2    0069F0D2      75 07                        JNZ SHORT PathOfEx.0069F0DB
	$+1B4    0069F0D4      8B06                         MOV EAX, DWORD PTR DS:[ESI]                      ; D3DX9_42.08D7AB74
	$+1B6    0069F0D6      8BCE                         MOV ECX, ESI                                     ; D3DX9_42.08CA89E8
	$+1B8    0069F0D8      FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+1BB    0069F0DB      C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+1C2    0069F0E2      837F 10 00                   CMP DWORD PTR DS:[EDI+0x10], 0x0
	$+1C6    0069F0E6      0F94C0                       SETE AL
	$+1C9    0069F0E9      84C0                         TEST AL, AL
	$+1CB    0069F0EB    ^ 0F84 67FEFFFF                JE PathOfEx.0069EF58
	$+1D1    0069F0F1      8B7D F0                      MOV EDI, DWORD PTR SS:[EBP-0x10]                 ; [ebp-10]=this
	$+1D4    0069F0F4      8B47 10                      MOV EAX, DWORD PTR DS:[EDI+0x10]                 ; 这个0x10是个偏移，获取成员变量
	$+1D7    0069F0F7      8D4F 10                      LEA ECX, DWORD PTR DS:[EDI+0x10]                 ; 好像是个pair的成员变量，像是存放的是begin和end指针
	$+1DA    0069F0FA      3B41 04                      CMP EAX, DWORD PTR DS:[ECX+0x4]
	$+1DD    0069F0FD      894D EC                      MOV DWORD PTR SS:[EBP-0x14], ECX
	$+1E0    0069F100      0F94C0                       SETE AL
	$+1E3    0069F103      84C0                         TEST AL, AL
	$+1E5    0069F105      0F85 04010000                JNZ PathOfEx.0069F20F
	$+1EB    0069F10B      FF47 1C                      INC DWORD PTR DS:[EDI+0x1C]
	$+1EE    0069F10E      8B71 04                      MOV ESI, DWORD PTR DS:[ECX+0x4]
	$+1F1    0069F111      3B31                         CMP ESI, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+1F3    0069F113      0F84 BF000000                JE PathOfEx.0069F1D8
	$+1F9    0069F119      0F1F80 00000000              NOP DWORD PTR DS:[EAX]
	$+200    0069F120      8B7E FC                      MOV EDI, DWORD PTR DS:[ESI-0x4]
	$+203    0069F123      83C6 F8                      ADD ESI, -0x8
	$+206    0069F126      8B0E                         MOV ECX, DWORD PTR DS:[ESI]                      ; D3DX9_42.08D7AB74
	$+208    0069F128      894D E0                      MOV DWORD PTR SS:[EBP-0x20], ECX
	$+20B    0069F12B      85FF                         TEST EDI, EDI
	$+20D    0069F12D      74 04                        JE SHORT PathOfEx.0069F133
	$+20F    0069F12F      F0:FF47 04                   LOCK INC DWORD PTR DS:[EDI+0x4]
	$+213    0069F133      897D DC                      MOV DWORD PTR SS:[EBP-0x24], EDI
	$+216    0069F136      894D D8                      MOV DWORD PTR SS:[EBP-0x28], ECX
	$+219    0069F139      85C9                         TEST ECX, ECX
	$+21B    0069F13B      C745 FC 06000000             MOV DWORD PTR SS:[EBP-0x4], 0x6
	$+222    0069F142      0F94C0                       SETE AL
	$+225    0069F145      84C0                         TEST AL, AL
	$+227    0069F147      74 09                        JE SHORT PathOfEx.0069F152
	$+229    0069F149      C745 FC 07000000             MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+230    0069F150      EB 1D                        JMP SHORT PathOfEx.0069F16F
	$+232    0069F152      FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
	$+235    0069F155      8B01                         MOV EAX, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+237    0069F157      FF75 0C                      PUSH DWORD PTR SS:[EBP+0xC]
	$+23A    0069F15A      FF75 08                      PUSH DWORD PTR SS:[EBP+0x8]
	$+23D    0069F15D      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; ecx==PreGameState
	$+23F    0069F15F      8B45 E0                      MOV EAX, DWORD PTR SS:[EBP-0x20]
	$+242    0069F162      8078 04 00                   CMP BYTE PTR DS:[EAX+0x4], 0x0
	$+246    0069F166      75 3A                        JNZ SHORT PathOfEx.0069F1A2
	$+248    0069F168      C745 FC 09000000             MOV DWORD PTR SS:[EBP-0x4], 0x9
	$+24F    0069F16F      85FF                         TEST EDI, EDI
	$+251    0069F171      74 1F                        JE SHORT PathOfEx.0069F192
	$+253    0069F173      8BC3                         MOV EAX, EBX
	$+255    0069F175      F0:0FC147 04                 LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+25A    0069F17A      75 16                        JNZ SHORT PathOfEx.0069F192
	$+25C    0069F17C      8B07                         MOV EAX, DWORD PTR DS:[EDI]
	$+25E    0069F17E      8BCF                         MOV ECX, EDI
	$+260    0069F180      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; PathOfEx.00F279A4
	$+262    0069F182      8BC3                         MOV EAX, EBX
	$+264    0069F184      F0:0FC147 08                 LOCK XADD DWORD PTR DS:[EDI+0x8], EAX
	$+269    0069F189      75 07                        JNZ SHORT PathOfEx.0069F192
	$+26B    0069F18B      8B07                         MOV EAX, DWORD PTR DS:[EDI]
	$+26D    0069F18D      8BCF                         MOV ECX, EDI
	$+26F    0069F18F      FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+272    0069F192      8B4D EC                      MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+275    0069F195      C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+27C    0069F19C      3B31                         CMP ESI, DWORD PTR DS:[ECX]                      ; PathOfEx.00F29304
	$+27E    0069F19E    ^ 75 80                        JNZ SHORT PathOfEx.0069F120
	$+280    0069F1A0      EB 33                        JMP SHORT PathOfEx.0069F1D5
	$+282    0069F1A2      C745 FC 08000000             MOV DWORD PTR SS:[EBP-0x4], 0x8
	$+289    0069F1A9      85FF                         TEST EDI, EDI
	$+28B    0069F1AB      74 1E                        JE SHORT PathOfEx.0069F1CB
	$+28D    0069F1AD      8BC3                         MOV EAX, EBX
	$+28F    0069F1AF      F0:0FC147 04                 LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+294    0069F1B4      75 15                        JNZ SHORT PathOfEx.0069F1CB
	$+296    0069F1B6      8B07                         MOV EAX, DWORD PTR DS:[EDI]
	$+298    0069F1B8      8BCF                         MOV ECX, EDI
	$+29A    0069F1BA      FF10                         CALL NEAR DWORD PTR DS:[EAX]                     ; PathOfEx.00F279A4
	$+29C    0069F1BC      F0:0FC15F 08                 LOCK XADD DWORD PTR DS:[EDI+0x8], EBX
	$+2A1    0069F1C1      4B                           DEC EBX
	$+2A2    0069F1C2      75 07                        JNZ SHORT PathOfEx.0069F1CB
	$+2A4    0069F1C4      8B07                         MOV EAX, DWORD PTR DS:[EDI]
	$+2A6    0069F1C6      8BCF                         MOV ECX, EDI
	$+2A8    0069F1C8      FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+2AB    0069F1CB      8B4D EC                      MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+2AE    0069F1CE      C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+2B5    0069F1D5      8B7D F0                      MOV EDI, DWORD PTR SS:[EBP-0x10]
	$+2B8    0069F1D8      8347 1C FF                   ADD DWORD PTR DS:[EDI+0x1C], -0x1
	$+2BC    0069F1DC      75 31                        JNZ SHORT PathOfEx.0069F20F
	$+2BE    0069F1DE      807F 20 00                   CMP BYTE PTR DS:[EDI+0x20], 0x0
	$+2C2    0069F1E2      74 2B                        JE SHORT PathOfEx.0069F20F
	$+2C4    0069F1E4      8B47 08                      MOV EAX, DWORD PTR DS:[EDI+0x8]                  ; d3d9.6EB4D187
	$+2C7    0069F1E7      2B47 04                      SUB EAX, DWORD PTR DS:[EDI+0x4]
	$+2CA    0069F1EA      C1F8 03                      SAR EAX, 0x3
	$+2CD    0069F1ED      50                           PUSH EAX
	$+2CE    0069F1EE      E8 ED0C0000                  CALL PathOfEx.0069FEE0
	$+2D3    0069F1F3      8B57 08                      MOV EDX, DWORD PTR DS:[EDI+0x8]                  ; d3d9.6EB4D187
	$+2D6    0069F1F6      8B4F 04                      MOV ECX, DWORD PTR DS:[EDI+0x4]
	$+2D9    0069F1F9      C645 10 00                   MOV BYTE PTR SS:[EBP+0x10], 0x0
	$+2DD    0069F1FD      FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
	$+2E0    0069F200      FF77 10                      PUSH DWORD PTR DS:[EDI+0x10]
	$+2E3    0069F203      E8 D81A0000                  CALL PathOfEx.006A0CE0
	$+2E8    0069F208      83C4 08                      ADD ESP, 0x8
	$+2EB    0069F20B      C647 20 00                   MOV BYTE PTR DS:[EDI+0x20], 0x0
	$+2EF    0069F20F      8B4D F4                      MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+2F2    0069F212      5F                           POP EDI
	$+2F3    0069F213      5E                           POP ESI                                          ; D3DX9_42.08CA89E8
	$+2F4    0069F214      64:890D 00000000             MOV DWORD PTR FS:[0], ECX
	$+2FB    0069F21B      5B                           POP EBX
	$+2FC    0069F21C      8BE5                         MOV ESP, EBP
	$+2FE    0069F21E      5D                           POP EBP
	$+2FF    0069F21F      C2 0C00                      RETN 0xC

	//2017/2/10 14:38
	$-1E     006A65D0     55                         PUSH EBP
	$-1D     006A65D1     8BEC                       MOV EBP, ESP
	$-1B     006A65D3     6A FF                      PUSH -0x1
	$-19     006A65D5     68 90FCD300                PUSH PathOfEx.00D3FC90
	$-14     006A65DA     64:A1 00000000             MOV EAX, DWORD PTR FS:[0]
	$-E      006A65E0     50                         PUSH EAX
	$-D      006A65E1     64:8925 00000000           MOV DWORD PTR FS:[0], ESP
	$-6      006A65E8     83EC 44                    SUB ESP, 0x44
	$-3      006A65EB     53                         PUSH EBX
	$-2      006A65EC     56                         PUSH ESI
	$-1      006A65ED     57                         PUSH EDI
	$ ==>    006A65EE     8BF9                       MOV EDI, ECX
	$+2      006A65F0     83CB FF                    OR EBX, -0x1
	$+5      006A65F3     897D F0                    MOV DWORD PTR SS:[EBP-0x10], EDI
	$+8      006A65F6     837F 3C 00                 CMP DWORD PTR DS:[EDI+0x3C], 0x0
	$+C      006A65FA     0F84 A0010000              JE PathOfEx.006A67A0
	$+12     006A6600     83C7 2C                    ADD EDI, 0x2C
	$+15     006A6603     897D DC                    MOV DWORD PTR SS:[EBP-0x24], EDI
	$+18     006A6606     8D45 C8                    LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+1B     006A6609     8BCF                       MOV ECX, EDI
	$+1D     006A660B     50                         PUSH EAX
	$+1E     006A660C     E8 1F26E2FF                CALL PathOfEx.004C8C30
	$+23     006A6611     8BD0                       MOV EDX, EAX
	$+25     006A6613     8B02                       MOV EAX, DWORD PTR DS:[EDX]
	$+27     006A6615     85C0                       TEST EAX, EAX
	$+29     006A6617     74 02                      JE SHORT PathOfEx.006A661B
	$+2B     006A6619     8B00                       MOV EAX, DWORD PTR DS:[EAX]
	$+2D     006A661B     8B48 08                    MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+30     006A661E     8B40 04                    MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+33     006A6621     49                         DEC ECX
	$+34     006A6622     234A 08                    AND ECX, DWORD PTR DS:[EDX+0x8]
	$+37     006A6625     8B0488                     MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+3A     006A6628     8BCF                       MOV ECX, EDI
	$+3C     006A662A     8B40 0C                    MOV EAX, DWORD PTR DS:[EAX+0xC]
	$+3F     006A662D     8945 E4                    MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+42     006A6630     8D45 BC                    LEA EAX, DWORD PTR SS:[EBP-0x44]
	$+45     006A6633     50                         PUSH EAX
	$+46     006A6634     E8 F725E2FF                CALL PathOfEx.004C8C30
	$+4B     006A6639     8BD0                       MOV EDX, EAX
	$+4D     006A663B     8B02                       MOV EAX, DWORD PTR DS:[EDX]
	$+4F     006A663D     85C0                       TEST EAX, EAX
	$+51     006A663F     74 02                      JE SHORT PathOfEx.006A6643
	$+53     006A6641     8B00                       MOV EAX, DWORD PTR DS:[EAX]
	$+55     006A6643     8B48 08                    MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+58     006A6646     8B40 04                    MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+5B     006A6649     49                         DEC ECX
	$+5C     006A664A     234A 08                    AND ECX, DWORD PTR DS:[EDX+0x8]
	$+5F     006A664D     8B0488                     MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+62     006A6650     8B70 08                    MOV ESI, DWORD PTR DS:[EAX+0x8]
	$+65     006A6653     8B40 04                    MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+68     006A6656     8945 E0                    MOV DWORD PTR SS:[EBP-0x20], EAX
	$+6B     006A6659     85F6                       TEST ESI, ESI
	$+6D     006A665B     74 04                      JE SHORT PathOfEx.006A6661
	$+6F     006A665D     F0:FF46 04                 LOCK INC DWORD PTR DS:[ESI+0x4]
	$+73     006A6661     8975 D8                    MOV DWORD PTR SS:[EBP-0x28], ESI
	$+76     006A6664     8945 D4                    MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+79     006A6667     8D45 B0                    LEA EAX, DWORD PTR SS:[EBP-0x50]
	$+7C     006A666A     C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+83     006A6671     50                         PUSH EAX
	$+84     006A6672     8BCF                       MOV ECX, EDI
	$+86     006A6674     E8 B725E2FF                CALL PathOfEx.004C8C30
	$+8B     006A6679     8BD0                       MOV EDX, EAX
	$+8D     006A667B     8B0A                       MOV ECX, DWORD PTR DS:[EDX]
	$+8F     006A667D     85C9                       TEST ECX, ECX
	$+91     006A667F     75 04                      JNZ SHORT PathOfEx.006A6685
	$+93     006A6681     33C0                       XOR EAX, EAX
	$+95     006A6683     EB 02                      JMP SHORT PathOfEx.006A6687
	$+97     006A6685     8B01                       MOV EAX, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+99     006A6687     8B48 08                    MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+9C     006A668A     8B40 04                    MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+9F     006A668D     49                         DEC ECX
	$+A0     006A668E     234A 08                    AND ECX, DWORD PTR DS:[EDX+0x8]
	$+A3     006A6691     8B0488                     MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+A6     006A6694     8B4F 08                    MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+A9     006A6697     49                         DEC ECX
	$+AA     006A6698     234F 0C                    AND ECX, DWORD PTR DS:[EDI+0xC]
	$+AD     006A669B     8B10                       MOV EDX, DWORD PTR DS:[EAX]
	$+AF     006A669D     8B47 04                    MOV EAX, DWORD PTR DS:[EDI+0x4]
	$+B2     006A66A0     8955 EC                    MOV DWORD PTR SS:[EBP-0x14], EDX
	$+B5     006A66A3     8B0488                     MOV EAX, DWORD PTR DS:[EAX+ECX*4]
	$+B8     006A66A6     C645 FC 02                 MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+BC     006A66AA     8B48 08                    MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+BF     006A66AD     894D E8                    MOV DWORD PTR SS:[EBP-0x18], ECX
	$+C2     006A66B0     85C9                       TEST ECX, ECX
	$+C4     006A66B2     74 21                      JE SHORT PathOfEx.006A66D5
	$+C6     006A66B4     8BC3                       MOV EAX, EBX
	$+C8     006A66B6     F0:0FC141 04               LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$+CD     006A66BB     75 18                      JNZ SHORT PathOfEx.006A66D5
	$+CF     006A66BD     8B01                       MOV EAX, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+D1     006A66BF     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+D3     006A66C1     8B4D E8                    MOV ECX, DWORD PTR SS:[EBP-0x18]
	$+D6     006A66C4     8BC3                       MOV EAX, EBX
	$+D8     006A66C6     F0:0FC141 08               LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$+DD     006A66CB     75 05                      JNZ SHORT PathOfEx.006A66D2
	$+DF     006A66CD     8B01                       MOV EAX, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+E1     006A66CF     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+E4     006A66D2     8B55 EC                    MOV EDX, DWORD PTR SS:[EBP-0x14]
	$+E7     006A66D5     C645 FC 01                 MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+EB     006A66D9     8347 10 FF                 ADD DWORD PTR DS:[EDI+0x10], -0x1
	$+EF     006A66DD     75 09                      JNZ SHORT PathOfEx.006A66E8
	$+F1     006A66DF     C747 0C 00000000           MOV DWORD PTR DS:[EDI+0xC], 0x0
	$+F8     006A66E6     EB 03                      JMP SHORT PathOfEx.006A66EB
	$+FA     006A66E8     FF47 0C                    INC DWORD PTR DS:[EDI+0xC]
	$+FD     006A66EB     8B45 E4                    MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+100    006A66EE     83E8 00                    SUB EAX, 0x0
	$+103    006A66F1     74 62                      JE SHORT PathOfEx.006A6755
	$+105    006A66F3     83E8 01                    SUB EAX, 0x1
	$+108    006A66F6     75 68                      JNZ SHORT PathOfEx.006A6760
	$+10A    006A66F8     83EC 08                    SUB ESP, 0x8
	$+10D    006A66FB     8BCC                       MOV ECX, ESP
	$+10F    006A66FD     894D EC                    MOV DWORD PTR SS:[EBP-0x14], ECX
	$+112    006A6700     C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+116    006A6704     8901                       MOV DWORD PTR DS:[ECX], EAX
	$+118    006A6706     8941 04                    MOV DWORD PTR DS:[ECX+0x4], EAX
	$+11B    006A6709     85F6                       TEST ESI, ESI
	$+11D    006A670B     74 04                      JE SHORT PathOfEx.006A6711
	$+11F    006A670D     F0:FF46 04                 LOCK INC DWORD PTR DS:[ESI+0x4]
	$+123    006A6711     8B79 04                    MOV EDI, DWORD PTR DS:[ECX+0x4]
	$+126    006A6714     85FF                       TEST EDI, EDI
	$+128    006A6716     74 22                      JE SHORT PathOfEx.006A673A
	$+12A    006A6718     8BC3                       MOV EAX, EBX
	$+12C    006A671A     F0:0FC147 04               LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+131    006A671F     75 19                      JNZ SHORT PathOfEx.006A673A
	$+133    006A6721     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+135    006A6723     8BCF                       MOV ECX, EDI
	$+137    006A6725     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+139    006A6727     8BC3                       MOV EAX, EBX
	$+13B    006A6729     F0:0FC147 08               LOCK XADD DWORD PTR DS:[EDI+0x8], EAX
	$+140    006A672E     75 07                      JNZ SHORT PathOfEx.006A6737
	$+142    006A6730     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+144    006A6732     8BCF                       MOV ECX, EDI
	$+146    006A6734     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+149    006A6737     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+14C    006A673A     8B45 E0                    MOV EAX, DWORD PTR SS:[EBP-0x20]
	$+14F    006A673D     8971 04                    MOV DWORD PTR DS:[ECX+0x4], ESI
	$+152    006A6740     8901                       MOV DWORD PTR DS:[ECX], EAX
	$+154    006A6742     8B4D F0                    MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+157    006A6745     6A 00                      PUSH 0x0
	$+159    006A6747     C645 FC 01                 MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+15D    006A674B     E8 E0F9FFFF                CALL PathOfEx.006A6130
	$+162    006A6750     8B7D DC                    MOV EDI, DWORD PTR SS:[EBP-0x24]
	$+165    006A6753     EB 0B                      JMP SHORT PathOfEx.006A6760
	$+167    006A6755     8B4D F0                    MOV ECX, DWORD PTR SS:[EBP-0x10]
	$+16A    006A6758     6A 00                      PUSH 0x0
	$+16C    006A675A     52                         PUSH EDX
	$+16D    006A675B     E8 10F7FFFF                CALL PathOfEx.006A5E70
	$+172    006A6760     C745 FC 04000000           MOV DWORD PTR SS:[EBP-0x4], 0x4
	$+179    006A6767     85F6                       TEST ESI, ESI
	$+17B    006A6769     74 1F                      JE SHORT PathOfEx.006A678A
	$+17D    006A676B     8BC3                       MOV EAX, EBX
	$+17F    006A676D     F0:0FC146 04               LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
	$+184    006A6772     75 16                      JNZ SHORT PathOfEx.006A678A
	$+186    006A6774     8B06                       MOV EAX, DWORD PTR DS:[ESI]
	$+188    006A6776     8BCE                       MOV ECX, ESI
	$+18A    006A6778     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+18C    006A677A     8BC3                       MOV EAX, EBX
	$+18E    006A677C     F0:0FC146 08               LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
	$+193    006A6781     75 07                      JNZ SHORT PathOfEx.006A678A
	$+195    006A6783     8B06                       MOV EAX, DWORD PTR DS:[ESI]
	$+197    006A6785     8BCE                       MOV ECX, ESI
	$+199    006A6787     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+19C    006A678A     8B45 F0                    MOV EAX, DWORD PTR SS:[EBP-0x10]
	$+19F    006A678D     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+1A6    006A6794     8378 3C 00                 CMP DWORD PTR DS:[EAX+0x3C], 0x0
	$+1AA    006A6798   ^ 0F85 68FEFFFF              JNZ PathOfEx.006A6606
	$+1B0    006A679E     8BF8                       MOV EDI, EAX
	$+1B2    006A67A0     8B47 10                    MOV EAX, DWORD PTR DS:[EDI+0x10]
	$+1B5    006A67A3     8D4F 10                    LEA ECX, DWORD PTR DS:[EDI+0x10]
	$+1B8    006A67A6     894D EC                    MOV DWORD PTR SS:[EBP-0x14], ECX
	$+1BB    006A67A9     3B41 04                    CMP EAX, DWORD PTR DS:[ECX+0x4]
	$+1BE    006A67AC     0F84 FD000000              JE PathOfEx.006A68AF
	$+1C4    006A67B2     FF47 1C                    INC DWORD PTR DS:[EDI+0x1C]
	$+1C7    006A67B5     8B77 14                    MOV ESI, DWORD PTR DS:[EDI+0x14]
	$+1CA    006A67B8     3B31                       CMP ESI, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+1CC    006A67BA     0F84 B8000000              JE PathOfEx.006A6878
	$+1D2    006A67C0     8B7E FC                    MOV EDI, DWORD PTR DS:[ESI-0x4]
	$+1D5    006A67C3     83C6 F8                    ADD ESI, -0x8
	$+1D8    006A67C6     8B0E                       MOV ECX, DWORD PTR DS:[ESI]
	$+1DA    006A67C8     894D DC                    MOV DWORD PTR SS:[EBP-0x24], ECX
	$+1DD    006A67CB     85FF                       TEST EDI, EDI
	$+1DF    006A67CD     74 04                      JE SHORT PathOfEx.006A67D3
	$+1E1    006A67CF     F0:FF47 04                 LOCK INC DWORD PTR DS:[EDI+0x4]
	$+1E5    006A67D3     897D D8                    MOV DWORD PTR SS:[EBP-0x28], EDI
	$+1E8    006A67D6     894D D4                    MOV DWORD PTR SS:[EBP-0x2C], ECX
	$+1EB    006A67D9     85C9                       TEST ECX, ECX
	$+1ED    006A67DB     C745 FC 06000000           MOV DWORD PTR SS:[EBP-0x4], 0x6
	$+1F4    006A67E2     0F94C0                     SETE AL
	$+1F7    006A67E5     84C0                       TEST AL, AL
	$+1F9    006A67E7     74 09                      JE SHORT PathOfEx.006A67F2
	$+1FB    006A67E9     C745 FC 07000000           MOV DWORD PTR SS:[EBP-0x4], 0x7
	$+202    006A67F0     EB 1D                      JMP SHORT PathOfEx.006A680F
	$+204    006A67F2     FF75 10                    PUSH DWORD PTR SS:[EBP+0x10]
	$+207    006A67F5     8B01                       MOV EAX, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+209    006A67F7     FF75 0C                    PUSH DWORD PTR SS:[EBP+0xC]
	$+20C    006A67FA     FF75 08                    PUSH DWORD PTR SS:[EBP+0x8]
	$+20F    006A67FD     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+211    006A67FF     8B45 DC                    MOV EAX, DWORD PTR SS:[EBP-0x24]
	$+214    006A6802     8078 04 00                 CMP BYTE PTR DS:[EAX+0x4], 0x0
	$+218    006A6806     75 3A                      JNZ SHORT PathOfEx.006A6842
	$+21A    006A6808     C745 FC 09000000           MOV DWORD PTR SS:[EBP-0x4], 0x9
	$+221    006A680F     85FF                       TEST EDI, EDI
	$+223    006A6811     74 1F                      JE SHORT PathOfEx.006A6832
	$+225    006A6813     8BC3                       MOV EAX, EBX
	$+227    006A6815     F0:0FC147 04               LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+22C    006A681A     75 16                      JNZ SHORT PathOfEx.006A6832
	$+22E    006A681C     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+230    006A681E     8BCF                       MOV ECX, EDI
	$+232    006A6820     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+234    006A6822     8BC3                       MOV EAX, EBX
	$+236    006A6824     F0:0FC147 08               LOCK XADD DWORD PTR DS:[EDI+0x8], EAX
	$+23B    006A6829     75 07                      JNZ SHORT PathOfEx.006A6832
	$+23D    006A682B     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+23F    006A682D     8BCF                       MOV ECX, EDI
	$+241    006A682F     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+244    006A6832     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+247    006A6835     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+24E    006A683C     3B31                       CMP ESI, DWORD PTR DS:[ECX]                  ; PathOfEx.00F2FCD8
	$+250    006A683E   ^ 75 80                      JNZ SHORT PathOfEx.006A67C0
	$+252    006A6840     EB 33                      JMP SHORT PathOfEx.006A6875
	$+254    006A6842     C745 FC 08000000           MOV DWORD PTR SS:[EBP-0x4], 0x8
	$+25B    006A6849     85FF                       TEST EDI, EDI
	$+25D    006A684B     74 1E                      JE SHORT PathOfEx.006A686B
	$+25F    006A684D     8BC3                       MOV EAX, EBX
	$+261    006A684F     F0:0FC147 04               LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
	$+266    006A6854     75 15                      JNZ SHORT PathOfEx.006A686B
	$+268    006A6856     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+26A    006A6858     8BCF                       MOV ECX, EDI
	$+26C    006A685A     FF10                       CALL NEAR DWORD PTR DS:[EAX]
	$+26E    006A685C     F0:0FC15F 08               LOCK XADD DWORD PTR DS:[EDI+0x8], EBX
	$+273    006A6861     4B                         DEC EBX
	$+274    006A6862     75 07                      JNZ SHORT PathOfEx.006A686B
	$+276    006A6864     8B07                       MOV EAX, DWORD PTR DS:[EDI]
	$+278    006A6866     8BCF                       MOV ECX, EDI
	$+27A    006A6868     FF50 04                    CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+27D    006A686B     8B4D EC                    MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+280    006A686E     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+287    006A6875     8B7D F0                    MOV EDI, DWORD PTR SS:[EBP-0x10]
	$+28A    006A6878     8347 1C FF                 ADD DWORD PTR DS:[EDI+0x1C], -0x1
	$+28E    006A687C     75 31                      JNZ SHORT PathOfEx.006A68AF
	$+290    006A687E     807F 20 00                 CMP BYTE PTR DS:[EDI+0x20], 0x0
	$+294    006A6882     74 2B                      JE SHORT PathOfEx.006A68AF
	$+296    006A6884     8B47 08                    MOV EAX, DWORD PTR DS:[EDI+0x8]
	$+299    006A6887     2B47 04                    SUB EAX, DWORD PTR DS:[EDI+0x4]
	$+29C    006A688A     C1F8 03                    SAR EAX, 0x3
	$+29F    006A688D     50                         PUSH EAX
	$+2A0    006A688E     E8 DD0C0000                CALL PathOfEx.006A7570
	$+2A5    006A6893     8B57 08                    MOV EDX, DWORD PTR DS:[EDI+0x8]
	$+2A8    006A6896     8B4F 04                    MOV ECX, DWORD PTR DS:[EDI+0x4]
	$+2AB    006A6899     C645 10 00                 MOV BYTE PTR SS:[EBP+0x10], 0x0
	$+2AF    006A689D     FF75 10                    PUSH DWORD PTR SS:[EBP+0x10]
	$+2B2    006A68A0     FF77 10                    PUSH DWORD PTR DS:[EDI+0x10]
	$+2B5    006A68A3     E8 281A0000                CALL PathOfEx.006A82D0
	$+2BA    006A68A8     83C4 08                    ADD ESP, 0x8
	$+2BD    006A68AB     C647 20 00                 MOV BYTE PTR DS:[EDI+0x20], 0x0
	$+2C1    006A68AF     8B4D F4                    MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+2C4    006A68B2     5F                         POP EDI
	$+2C5    006A68B3     5E                         POP ESI
	$+2C6    006A68B4     64:890D 00000000           MOV DWORD PTR FS:[0], ECX
	$+2CD    006A68BB     5B                         POP EBX
	$+2CE    006A68BC     8BE5                       MOV ESP, EBP
	$+2D0    006A68BE     5D                         POP EBP
	$+2D1    006A68BF     C2 0C00                    RETN 0xC
	*/
	/**/
	//游戏里基本上都是，先构建封包内容，紧接着再调用这个发包函数。
	addr_tmp = scan_exe_.ScanAddr("8b??83????89????83??????0f??????????83????89????8d????8b??50", -0x1e);
	assert(addr_tmp);
	if (addr_tmp)
		assert(*(pt_pcbyte)addr_tmp == 0x55);
	ofs << "pt_cdword	g_c_scene_holder_vtbl_fn_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//去参考：tag_收包处理范例
	/*
	$-190    00A62620     55                         PUSH EBP
	$-18F    00A62621     8BEC                       MOV EBP, ESP
	$-18D    00A62623     6A FF                      PUSH -0x1
	$-18B    00A62625     68 23D4D600                PUSH PathOfEx.00D6D423
	$-186    00A6262A     64:A1 00000000             MOV EAX, DWORD PTR FS:[0]
	$-180    00A62630     50                         PUSH EAX
	$-17F    00A62631     64:8925 00000000           MOV DWORD PTR FS:[0], ESP
	$-178    00A62638     81EC E8000000              SUB ESP, 0xE8
	$-172    00A6263E     53                         PUSH EBX
	$-171    00A6263F     8B5D 08                    MOV EBX, DWORD PTR SS:[EBP+0x8]
	$-16E    00A62642     57                         PUSH EDI
	$-16D    00A62643     8BF9                       MOV EDI, ECX
	$-16B    00A62645     8BCB                       MOV ECX, EBX
	$-169    00A62647     897D E8                    MOV DWORD PTR SS:[EBP-0x18], EDI
	$-166    00A6264A     803F 00                    CMP BYTE PTR DS:[EDI], 0x0
	$-163    00A6264D     74 13                      JE SHORT PathOfEx.00A62662
	$-161    00A6264F     6A 01                      PUSH 0x1
	$-15F    00A62651     8D45 F3                    LEA EAX, DWORD PTR SS:[EBP-0xD]
	$-15C    00A62654     50                         PUSH EAX
	$-15B    00A62655     E8 B6B0D2FF                CALL PathOfEx.0078D710                   ; 根据长度取数据(此处没有执行到过)
	$-156    00A6265A     0FB645 F3                  MOVZX EAX, BYTE PTR SS:[EBP-0xD]
	$-152    00A6265E     8BD0                       MOV EDX, EAX
	$-150    00A62660     EB 17                      JMP SHORT PathOfEx.00A62679
	$-14E    00A62662     6A 02                      PUSH 0x2
	$-14C    00A62664     8D45 EC                    LEA EAX, DWORD PTR SS:[EBP-0x14]
	$-149    00A62667     50                         PUSH EAX
	$-148    00A62668     E8 A3B0D2FF                CALL PathOfEx.0078D710                   ; 根据长度取数据(取到的word是消息长度)
	$-143    00A6266D     FF75 EC                    PUSH DWORD PTR SS:[EBP-0x14]
	$-140    00A62670     FF15 5055D800              CALL NEAR DWORD PTR DS:[0xD85550]        ; ntohs
	$-13A    00A62676     0FB7D0                     MOVZX EDX, AX
	$-137    00A62679     8A83 B2000000              MOV AL, BYTE PTR DS:[EBX+0xB2]
	$-131    00A6267F     8955 EC                    MOV DWORD PTR SS:[EBP-0x14], EDX
	$-12E    00A62682     84C0                       TEST AL, AL
	$-12C    00A62684     74 0E                      JE SHORT PathOfEx.00A62694
	$-12A    00A62686     C683 B2000000 00           MOV BYTE PTR DS:[EBX+0xB2], 0x0
	$-123    00A6268D     32C0                       XOR AL, AL
	$-121    00A6268F     E9 4D010000                JMP PathOfEx.00A627E1
	$-11C    00A62694     56                         PUSH ESI                                 ; 如果有封包的话就会到这里
	$-11B    00A62695     8B77 0C                    MOV ESI, DWORD PTR DS:[EDI+0xC]
	$-118    00A62698     8B7F 10                    MOV EDI, DWORD PTR DS:[EDI+0x10]
	$-115    00A6269B     C645 0B 00                 MOV BYTE PTR SS:[EBP+0xB], 0x0
	$-111    00A6269F     90                         NOP
	$-110    00A626A0     3BF7                       CMP ESI, EDI
	$-10E    00A626A2     74 1A                      JE SHORT PathOfEx.00A626BE
	$-10C    00A626A4     8B0E                       MOV ECX, DWORD PTR DS:[ESI]			; g_h_AnalysisRecvPacket_.去参考：tag_收包处理范例。此处进行HOOK：过滤收包的patch_asm.txt
	$-10A    00A626A6     53                         PUSH EBX                                 ; 收包结构，网络数据，还未转换成本地数据。这个地址是缓冲区长度，并不是当前收包的结构。
	$-109    00A626A7     52                         PUSH EDX                                 ; msg_id，经过ntohs转换成本地数据了
	$-108    00A626A8     8B01                       MOV EAX, DWORD PTR DS:[ECX]
	$-106    00A626AA     8B00                       MOV EAX, DWORD PTR DS:[EAX]
	$-104    00A626AC     FFD0                       CALL NEAR EAX                            ; 封包分发call，序列化解密后的网络数据，并生成一个经过ntohs后的对象，然后处理之
	$-102    00A626AE     84C0                       TEST AL, AL                              ; al=1则成功
	$-100    00A626B0     0F85 E6000000              JNZ PathOfEx.00A6279C
	$-FA     00A626B6     8B55 EC                    MOV EDX, DWORD PTR SS:[EBP-0x14]
	$-F7     00A626B9     83C6 04                    ADD ESI, 0x4
	$-F4     00A626BC   ^ EB E2                      JMP SHORT PathOfEx.00A626A0
	$-F2     00A626BE     807D 0B 00                 CMP BYTE PTR SS:[EBP+0xB], 0x0
	$-EE     00A626C2     0F85 D4000000              JNZ PathOfEx.00A6279C
	$-E8     00A626C8     83EC 08                    SUB ESP, 0x8
	$-E5     00A626CB     8D8D 0CFFFFFF              LEA ECX, DWORD PTR SS:[EBP-0xF4]
	$-DF     00A626D1     E8 5A11CAFF                CALL PathOfEx.00703830
	$-DA     00A626D6     BA AC54F300                MOV EDX, PathOfEx.00F354AC               ; ASCII "Unable to deserialise packet with pid "
	$-D5     00A626DB     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-CE     00A626E2     8D8D 1CFFFFFF              LEA ECX, DWORD PTR SS:[EBP-0xE4]
	$-C8     00A626E8     E8 C32FCAFF                CALL PathOfEx.007056B0
	$-C3     00A626ED     8B4D E8                    MOV ECX, DWORD PTR SS:[EBP-0x18]
	$-C0     00A626F0     8B45 EC                    MOV EAX, DWORD PTR SS:[EBP-0x14]
	$-BD     00A626F3     8B51 04                    MOV EDX, DWORD PTR DS:[ECX+0x4]
	$-BA     00A626F6     85D2                       TEST EDX, EDX
	$-B8     00A626F8     74 3A                      JE SHORT PathOfEx.00A62734
	$-B6     00A626FA     0FB7F0                     MOVZX ESI, AX
	$-B3     00A626FD     8D14B2                     LEA EDX, DWORD PTR DS:[EDX+ESI*4]
	$-B0     00A62700     3B51 08                    CMP EDX, DWORD PTR DS:[ECX+0x8]
	$-AD     00A62703     73 2F                      JNB SHORT PathOfEx.00A62734
	$-AB     00A62705     8B12                       MOV EDX, DWORD PTR DS:[EDX]
	$-A9     00A62707     8D8D 1CFFFFFF              LEA ECX, DWORD PTR SS:[EBP-0xE4]
	$-A3     00A6270D     56                         PUSH ESI
	$-A2     00A6270E     E8 9D2FCAFF                CALL PathOfEx.007056B0
	$-9D     00A62713     BA F0E2EE00                MOV EDX, PathOfEx.00EEE2F0               ; UNICODE "("
	$-98     00A62718     8BC8                       MOV ECX, EAX
	$-96     00A6271A     E8 912FCAFF                CALL PathOfEx.007056B0
	$-91     00A6271F     8BC8                       MOV ECX, EAX
	$-8F     00A62721     E8 6ABBCAFF                CALL PathOfEx.0070E290
	$-8A     00A62726     BA F4E2EE00                MOV EDX, PathOfEx.00EEE2F4               ; UNICODE ")"
	$-85     00A6272B     8BC8                       MOV ECX, EAX
	$-83     00A6272D     E8 7E2FCAFF                CALL PathOfEx.007056B0
	$-7E     00A62732     EB 0F                      JMP SHORT PathOfEx.00A62743
	$-7C     00A62734     0FB7C0                     MOVZX EAX, AX
	$-79     00A62737     8D8D 1CFFFFFF              LEA ECX, DWORD PTR SS:[EBP-0xE4]
	$-73     00A6273D     50                         PUSH EAX
	$-72     00A6273E     E8 4DBBCAFF                CALL PathOfEx.0070E290
	$-6D     00A62743     8D45 BC                    LEA EAX, DWORD PTR SS:[EBP-0x44]
	$-6A     00A62746     50                         PUSH EAX
	$-69     00A62747     8D8D 0CFFFFFF              LEA ECX, DWORD PTR SS:[EBP-0xF4]
	$-63     00A6274D     E8 DE0FCAFF                CALL PathOfEx.00703730
	$-5E     00A62752     C645 FC 01                 MOV BYTE PTR SS:[EBP-0x4], 0x1
	$-5A     00A62756     8378 14 10                 CMP DWORD PTR DS:[EAX+0x14], 0x10
	$-56     00A6275A     72 02                      JB SHORT PathOfEx.00A6275E
	$-54     00A6275C     8B00                       MOV EAX, DWORD PTR DS:[EAX]
	$-52     00A6275E     8945 E4                    MOV DWORD PTR SS:[EBP-0x1C], EAX
	$-4F     00A62761     0F57C0                     XORPS XMM0, XMM0
	$-4C     00A62764     8D45 D8                    LEA EAX, DWORD PTR SS:[EBP-0x28]
	$-49     00A62767     C745 D4 E8AFDD00           MOV DWORD PTR SS:[EBP-0x2C], PathOfEx.00>; ASCII "罢F"
	$-42     00A6276E     50                         PUSH EAX
	$-41     00A6276F     8D45 E4                    LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$-3E     00A62772     660FD645 D8                MOVQ QWORD PTR SS:[EBP-0x28], XMM0
	$-39     00A62777     50                         PUSH EAX
	$-38     00A62778     C645 E8 01                 MOV BYTE PTR SS:[EBP-0x18], 0x1
	$-34     00A6277C     E8 23892200                CALL PathOfEx.00C8B0A4
	$-2F     00A62781     83C4 08                    ADD ESP, 0x8
	$-2C     00A62784     C745 D4 5012ED00           MOV DWORD PTR SS:[EBP-0x2C], PathOfEx.00>
	$-25     00A6278B     8D45 D4                    LEA EAX, DWORD PTR SS:[EBP-0x2C]
	$-22     00A6278E     895D E0                    MOV DWORD PTR SS:[EBP-0x20], EBX
	$-1F     00A62791     68 C061FC00                PUSH PathOfEx.00FC61C0
	$-1A     00A62796     50                         PUSH EAX
	$-19     00A62797     E8 D7992200                CALL PathOfEx.00C8C173
	$-14     00A6279C     80BB B2000000 00           CMP BYTE PTR DS:[EBX+0xB2], 0x0
	$-D      00A627A3     74 0B                      JE SHORT PathOfEx.00A627B0
	$-B      00A627A5     C683 B2000000 00           MOV BYTE PTR DS:[EBX+0xB2], 0x0
	$-4      00A627AC     32C0                       XOR AL, AL
	$-2      00A627AE     EB 30                      JMP SHORT PathOfEx.00A627E0
	$ ==>    00A627B0     8BB3 C4000000              MOV ESI, DWORD PTR DS:[EBX+0xC4]         ; 当前消息长度
	$+6      00A627B6     33D2                       XOR EDX, EDX								;去参考：tag_收包处理范例
	$+8      00A627B8     01B3 BC000000              ADD DWORD PTR DS:[EBX+0xBC], ESI         ; 收到的所有包的总长度。每次注销会清空，缓冲区满了会清空。
	$+E      00A627BE     8B8B D8000000              MOV ECX, DWORD PTR DS:[EBX+0xD8]
	$+14     00A627C4     2B8B D4000000              SUB ECX, DWORD PTR DS:[EBX+0xD4]
	$+1A     00A627CA     8B83 BC000000              MOV EAX, DWORD PTR DS:[EBX+0xBC]
	$+20     00A627D0     F7F1                       DIV ECX
	$+22     00A627D2     29B3 C0000000              SUB DWORD PTR DS:[EBX+0xC0], ESI         ; 未序列化的包长度
	$+28     00A627D8     B0 01                      MOV AL, 0x1
	$+2A     00A627DA     8993 BC000000              MOV DWORD PTR DS:[EBX+0xBC], EDX
	$+30     00A627E0     5E                         POP ESI                                  ; 0018EFD8
	$+31     00A627E1     8B4D F4                    MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+34     00A627E4     C783 C4000000 00000000     MOV DWORD PTR DS:[EBX+0xC4], 0x0
	$+3E     00A627EE     5F                         POP EDI                                  ; 0018EFD8
	$+3F     00A627EF     5B                         POP EBX                                  ; 0018EFD8
	$+40     00A627F0     64:890D 00000000           MOV DWORD PTR FS:[0], ECX
	$+47     00A627F7     8BE5                       MOV ESP, EBP
	$+49     00A627F9     5D                         POP EBP                                  ; 0018EFD8
	$+4A     00A627FA     C2 0400                    RETN 0x4
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??????????33??01??????????8b??????????2b??????????8b", 0);
	assert(addr_tmp);
	ofs << "pt_cdword	g_h_recv_packet_read_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = scan_exe_.ScanAddr("8b??53528b??8b??ff??84??0f??????????8b????83????eb??80??????0f??????????83????8d", 0);
	assert(addr_tmp);
	ofs << "pt_cdword	g_h_AnalysisRecvPacket_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	//tag_发包之加密函数
	//拿起背包物品时构建发包内容
	/*
	$-30     00AA2A6F     CC                   INT3                                     ; tag_发包之加密函数
	$-2F     00AA2A70     51                   PUSH ECX                                 ; 第一个参数是edi
	$-2E     00AA2A71     53                   PUSH EBX                                 ; p1=stCD_SendPacket*
	$-2D     00AA2A72     8BD9                 MOV EBX, ECX                             ; 拿起背包物品的时候会断到，构建发包内容
	$-2B     00AA2A74     57                   PUSH EDI
	$-2A     00AA2A75     0FB743 04            MOVZX EAX, WORD PTR DS:[EBX+0x4]
	$-26     00AA2A79     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$-25     00AA2A7A     FF15 40C5D800        CALL NEAR DWORD PTR DS:[0xD8C540]
	$-1F     00AA2A80     8B7C24 10            MOV EDI, DWORD PTR SS:[ESP+0x10]
	$-1B     00AA2A84     8BCF                 MOV ECX, EDI
	$-19     00AA2A86     0FB7C0               MOVZX EAX, AX
	$-16     00AA2A89     894424 08            MOV DWORD PTR SS:[ESP+0x8], EAX          ; PathOfEx.00EF79A4
	$-12     00AA2A8D     8D4424 08            LEA EAX, DWORD PTR SS:[ESP+0x8]
	$-E      00AA2A91     6A 02                PUSH 0x2
	$-C      00AA2A93     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$-B      00AA2A94     E8 A70CCFFF          CALL PathOfEx.00793740                   ; 构建发包内容
	$-6      00AA2A99     FF73 08              PUSH DWORD PTR DS:[EBX+0x8]              ; 字段1,enCD_InventoryType
	$-3      00AA2A9C     8B4F 58              MOV ECX, DWORD PTR DS:[EDI+0x58]
	$ ==>    00AA2A9F     E8 EC040000          CALL PathOfEx.00AA2F90                   ; 加密函数，加密dword
	$+5      00AA2AA4     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$+6      00AA2AA5     FF15 F8C4D800        CALL NEAR DWORD PTR DS:[0xD8C4F8]
	$+C      00AA2AAB     894424 10            MOV DWORD PTR SS:[ESP+0x10], EAX         ; PathOfEx.00EF79A4
	$+10     00AA2AAF     8BCF                 MOV ECX, EDI
	$+12     00AA2AB1     6A 04                PUSH 0x4
	$+14     00AA2AB3     8D4424 14            LEA EAX, DWORD PTR SS:[ESP+0x14]
	$+18     00AA2AB7     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$+19     00AA2AB8     E8 830CCFFF          CALL PathOfEx.00793740
	$+1E     00AA2ABD     FF73 0C              PUSH DWORD PTR DS:[EBX+0xC]              ; 字段2，item_id
	$+21     00AA2AC0     FF15 F8C4D800        CALL NEAR DWORD PTR DS:[0xD8C4F8]
	$+27     00AA2AC6     894424 10            MOV DWORD PTR SS:[ESP+0x10], EAX         ; PathOfEx.00EF79A4
	$+2B     00AA2ACA     8BCF                 MOV ECX, EDI
	$+2D     00AA2ACC     6A 04                PUSH 0x4
	$+2F     00AA2ACE     8D4424 14            LEA EAX, DWORD PTR SS:[ESP+0x14]
	$+33     00AA2AD2     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$+34     00AA2AD3     E8 680CCFFF          CALL PathOfEx.00793740
	$+39     00AA2AD8     0FB643 10            MOVZX EAX, BYTE PTR DS:[EBX+0x10]        ; 字段3,物品id
	$+3D     00AA2ADC     8B4F 58              MOV ECX, DWORD PTR DS:[EDI+0x58]         ; g_so_send_packet_encrypt_
	$+40     00AA2ADF     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$+41     00AA2AE0     E8 3B050000          CALL PathOfEx.00AA3020                   ; 加密函数，加密byte
	$+46     00AA2AE5     884424 10            MOV BYTE PTR SS:[ESP+0x10], AL
	$+4A     00AA2AE9     8BCF                 MOV ECX, EDI
	$+4C     00AA2AEB     6A 01                PUSH 0x1
	$+4E     00AA2AED     8D4424 14            LEA EAX, DWORD PTR SS:[ESP+0x14]
	$+52     00AA2AF1     50                   PUSH EAX                                 ; PathOfEx.00EF79A4
	$+53     00AA2AF2     E8 490CCFFF          CALL PathOfEx.00793740
	$+58     00AA2AF7     5F                   POP EDI                                  ; PathOfEx.008E8903
	$+59     00AA2AF8     5B                   POP EBX                                  ; PathOfEx.008E8903
	$+5A     00AA2AF9     59                   POP ECX                                  ; PathOfEx.008E8903
	$+5B     00AA2AFA     C2 0400              RETN 0x4
	$+5E     00AA2AFD     CC                   INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("e8????????50ff??????????89??????8b??6a??8d??????50e8????????ff", 0, 2);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsByte(addr_tmp, 0x3d + 2);
	assert(dw_tmp > 0x40 && dw_tmp < 0xa0);
	ofs << "pt_cdword	g_o_send_packet_encrypt_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	dw_tmp = (pt_dword)Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr(addr_tmp);
	if (!dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_encrypt_int_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = (pt_byte*)addr_tmp + 0x41;
	dw_tmp = (pt_dword)Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr(addr_tmp);
	if (!dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_encrypt_byte_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-70     00AA4C5F      CC                 INT3
	$-6F     00AA4C60      56                 PUSH ESI
	$-6E     00AA4C61      57                 PUSH EDI
	$-6D     00AA4C62      8B7C24 0C          MOV EDI,DWORD PTR SS:[ESP+0xC]
	$-69     00AA4C66      8D71 08            LEA ESI,DWORD PTR DS:[ECX+0x8]
	$-66     00AA4C69      6A 04              PUSH 0x4
	$-64     00AA4C6B      56                 PUSH ESI
	$-63     00AA4C6C      8BCF               MOV ECX,EDI
	$-61     00AA4C6E      E8 DD03CFFF        CALL PathOfEx.00795050
	$-5C     00AA4C73      FF36               PUSH DWORD PTR DS:[ESI]
	$-5A     00AA4C75      FF15 FCE4D800      CALL DWORD PTR DS:[0xD8E4FC]
	$-54     00AA4C7B      8906               MOV DWORD PTR DS:[ESI],EAX
	$-52     00AA4C7D      80BF B2000000 00   CMP BYTE PTR DS:[EDI+0xB2],0x0
	$-4B     00AA4C84      75 12              JNZ SHORT PathOfEx.00AA4C98
	$-49     00AA4C86      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$-46     00AA4C89      56                 PUSH ESI
	$-45     00AA4C8A      E8 71030000        CALL PathOfEx.00AA5000
	$-40     00AA4C8F      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$-3D     00AA4C92      56                 PUSH ESI
	$-3C     00AA4C93      E8 08040000        CALL PathOfEx.00AA50A0
	$-37     00AA4C98      5F                 POP EDI
	$-36     00AA4C99      5E                 POP ESI
	$-35     00AA4C9A      C2 0400            RETN 0x4
	$-32     00AA4C9D      CC                 INT3
	$-31     00AA4C9E      CC                 INT3
	$-30     00AA4C9F      CC                 INT3
	$-2F     00AA4CA0      51                 PUSH ECX                                 ; 构建stCDS_MergeItem
	$-2E     00AA4CA1      53                 PUSH EBX
	$-2D     00AA4CA2      8BD9               MOV EBX,ECX
	$-2B     00AA4CA4      57                 PUSH EDI
	$-2A     00AA4CA5      0FB743 04          MOVZX EAX,WORD PTR DS:[EBX+0x4]
	$-26     00AA4CA9      50                 PUSH EAX
	$-25     00AA4CAA      FF15 40E5D800      CALL DWORD PTR DS:[0xD8E540]
	$-1F     00AA4CB0      8B7C24 10          MOV EDI,DWORD PTR SS:[ESP+0x10]
	$-1B     00AA4CB4      8BCF               MOV ECX,EDI
	$-19     00AA4CB6      0FB7C0             MOVZX EAX,AX
	$-16     00AA4CB9      894424 08          MOV DWORD PTR SS:[ESP+0x8],EAX
	$-12     00AA4CBD      8D4424 08          LEA EAX,DWORD PTR SS:[ESP+0x8]
	$-E      00AA4CC1      6A 02              PUSH 0x2
	$-C      00AA4CC3      50                 PUSH EAX
	$-B      00AA4CC4      E8 9704CFFF        CALL PathOfEx.00795160
	$-6      00AA4CC9      FF73 08            PUSH DWORD PTR DS:[EBX+0x8]
	$-3      00AA4CCC      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$ ==>    00AA4CCF      E8 6C040000        CALL PathOfEx.00AA5140                   ; g_c_encrypt_int1_
	$+5      00AA4CD4      50                 PUSH EAX
	$+6      00AA4CD5      FF15 F8E4D800      CALL DWORD PTR DS:[0xD8E4F8]
	$+C      00AA4CDB      894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
	$+10     00AA4CDF      8BCF               MOV ECX,EDI
	$+12     00AA4CE1      6A 04              PUSH 0x4
	$+14     00AA4CE3      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+18     00AA4CE7      50                 PUSH EAX
	$+19     00AA4CE8      E8 7304CFFF        CALL PathOfEx.00795160
	$+1E     00AA4CED      FF73 0C            PUSH DWORD PTR DS:[EBX+0xC]
	$+21     00AA4CF0      FF15 F8E4D800      CALL DWORD PTR DS:[0xD8E4F8]
	$+27     00AA4CF6      894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
	$+2B     00AA4CFA      8BCF               MOV ECX,EDI
	$+2D     00AA4CFC      6A 04              PUSH 0x4
	$+2F     00AA4CFE      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+33     00AA4D02      50                 PUSH EAX
	$+34     00AA4D03      E8 5804CFFF        CALL PathOfEx.00795160
	$+39     00AA4D08      0FB643 10          MOVZX EAX,BYTE PTR DS:[EBX+0x10]
	$+3D     00AA4D0C      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$+40     00AA4D0F      50                 PUSH EAX
	$+41     00AA4D10      E8 BB040000        CALL PathOfEx.00AA51D0                   ; g_c_encrypt_byte1_
	$+46     00AA4D15      884424 10          MOV BYTE PTR SS:[ESP+0x10],AL
	$+4A     00AA4D19      8BCF               MOV ECX,EDI
	$+4C     00AA4D1B      6A 01              PUSH 0x1
	$+4E     00AA4D1D      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+52     00AA4D21      50                 PUSH EAX
	$+53     00AA4D22      E8 3904CFFF        CALL PathOfEx.00795160
	$+58     00AA4D27      5F                 POP EDI
	$+59     00AA4D28      5B                 POP EBX
	$+5A     00AA4D29      59                 POP ECX
	$+5B     00AA4D2A      C2 0400            RETN 0x4
	$+5E     00AA4D2D      CC                 INT3
	$+5F     00AA4D2E      CC                 INT3
	$+60     00AA4D2F      CC                 INT3
	$+61     00AA4D30      51                 PUSH ECX
	$+62     00AA4D31      53                 PUSH EBX
	$+63     00AA4D32      8BD9               MOV EBX,ECX
	$+65     00AA4D34      57                 PUSH EDI
	$+66     00AA4D35      0FB743 04          MOVZX EAX,WORD PTR DS:[EBX+0x4]
	$+6A     00AA4D39      50                 PUSH EAX
	$+6B     00AA4D3A      FF15 40E5D800      CALL DWORD PTR DS:[0xD8E540]
	$+71     00AA4D40      8B7C24 10          MOV EDI,DWORD PTR SS:[ESP+0x10]
	$+75     00AA4D44      8BCF               MOV ECX,EDI
	$+77     00AA4D46      0FB7C0             MOVZX EAX,AX
	$+7A     00AA4D49      894424 08          MOV DWORD PTR SS:[ESP+0x8],EAX
	$+7E     00AA4D4D      8D4424 08          LEA EAX,DWORD PTR SS:[ESP+0x8]
	$+82     00AA4D51      6A 02              PUSH 0x2
	$+84     00AA4D53      50                 PUSH EAX
	$+85     00AA4D54      E8 0704CFFF        CALL PathOfEx.00795160
	$+8A     00AA4D59      FF73 08            PUSH DWORD PTR DS:[EBX+0x8]
	$+8D     00AA4D5C      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$+90     00AA4D5F      E8 EC040000        CALL PathOfEx.00AA5250                   ; g_c_encrypt_int1_
	$+95     00AA4D64      50                 PUSH EAX
	$+96     00AA4D65      FF15 F8E4D800      CALL DWORD PTR DS:[0xD8E4F8]
	$+9C     00AA4D6B      894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
	$+A0     00AA4D6F      8BCF               MOV ECX,EDI
	$+A2     00AA4D71      6A 04              PUSH 0x4
	$+A4     00AA4D73      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+A8     00AA4D77      50                 PUSH EAX
	$+A9     00AA4D78      E8 E303CFFF        CALL PathOfEx.00795160
	$+AE     00AA4D7D      FF73 0C            PUSH DWORD PTR DS:[EBX+0xC]
	$+B1     00AA4D80      FF15 F8E4D800      CALL DWORD PTR DS:[0xD8E4F8]
	$+B7     00AA4D86      894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
	$+BB     00AA4D8A      8BCF               MOV ECX,EDI
	$+BD     00AA4D8C      6A 04              PUSH 0x4
	$+BF     00AA4D8E      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+C3     00AA4D92      50                 PUSH EAX
	$+C4     00AA4D93      E8 C803CFFF        CALL PathOfEx.00795160
	$+C9     00AA4D98      0FB643 10          MOVZX EAX,BYTE PTR DS:[EBX+0x10]
	$+CD     00AA4D9C      8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
	$+D0     00AA4D9F      50                 PUSH EAX
	$+D1     00AA4DA0      E8 3B050000        CALL PathOfEx.00AA52E0                   ; g_c_encrypt_byte1_
	$+D6     00AA4DA5      884424 10          MOV BYTE PTR SS:[ESP+0x10],AL
	$+DA     00AA4DA9      8BCF               MOV ECX,EDI
	$+DC     00AA4DAB      6A 01              PUSH 0x1
	$+DE     00AA4DAD      8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+E2     00AA4DB1      50                 PUSH EAX
	$+E3     00AA4DB2      E8 A903CFFF        CALL PathOfEx.00795160
	$+E8     00AA4DB7      5F                 POP EDI
	$+E9     00AA4DB8      5B                 POP EBX
	$+EA     00AA4DB9      59                 POP ECX
	$+EB     00AA4DBA      C2 0400            RETN 0x4
	$+EE     00AA4DBD      CC                 INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("e8????????50ff??????????89??????8b??6a??8d??????50e8????????ff????ff??????????89??????8b??6a??8d??????50e8????????0f??????8b????50e8", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = (pt_dword)Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr(addr_tmp);
	if (!dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_encrypt_int1_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	addr_tmp = (pt_byte*)addr_tmp + 0x41;
	dw_tmp = (pt_dword)Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr(addr_tmp);
	if (!dw_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_encrypt_byte1_ = " << ExeAddr2Offset(dw_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-7F     00AA550F     CC                      INT3
	$-7E     00AA5510     53                      PUSH EBX                              ; stCDR_OnMove,stCDGR_OnMove
	$-7D     00AA5511     8B5C24 08               MOV EBX,DWORD PTR SS:[ESP+0x8]
	$-79     00AA5515     55                      PUSH EBP
	$-78     00AA5516     56                      PUSH ESI
	$-77     00AA5517     57                      PUSH EDI
	$-76     00AA5518     8BF9                    MOV EDI,ECX
	$-74     00AA551A     8BCB                    MOV ECX,EBX
	$-72     00AA551C     6A 04                   PUSH 0x4
	$-70     00AA551E     8D77 08                 LEA ESI,DWORD PTR DS:[EDI+0x8]
	$-6D     00AA5521     56                      PUSH ESI
	$-6C     00AA5522     E8 29FBCEFF             CALL PathOfEx.00795050
	$-67     00AA5527     FF36                    PUSH DWORD PTR DS:[ESI]
	$-65     00AA5529     8B2D FCE4D800           MOV EBP,DWORD PTR DS:[0xD8E4FC]
	$-5F     00AA552F     FFD5                    CALL EBP
	$-5D     00AA5531     8906                    MOV DWORD PTR DS:[ESI],EAX
	$-5B     00AA5533     8BCB                    MOV ECX,EBX
	$-59     00AA5535     6A 04                   PUSH 0x4
	$-57     00AA5537     8D77 0C                 LEA ESI,DWORD PTR DS:[EDI+0xC]
	$-54     00AA553A     56                      PUSH ESI
	$-53     00AA553B     E8 10FBCEFF             CALL PathOfEx.00795050
	$-4E     00AA5540     FF36                    PUSH DWORD PTR DS:[ESI]
	$-4C     00AA5542     FFD5                    CALL EBP
	$-4A     00AA5544     8906                    MOV DWORD PTR DS:[ESI],EAX
	$-48     00AA5546     8BCB                    MOV ECX,EBX
	$-46     00AA5548     6A 02                   PUSH 0x2
	$-44     00AA554A     8D77 10                 LEA ESI,DWORD PTR DS:[EDI+0x10]
	$-41     00AA554D     56                      PUSH ESI
	$-40     00AA554E     E8 FDFACEFF             CALL PathOfEx.00795050
	$-3B     00AA5553     0FB706                  MOVZX EAX,WORD PTR DS:[ESI]
	$-38     00AA5556     50                      PUSH EAX
	$-37     00AA5557     FF15 4CE5D800           CALL DWORD PTR DS:[0xD8E54C]
	$-31     00AA555D     66:8906                 MOV WORD PTR DS:[ESI],AX
	$-2E     00AA5560     8BCB                    MOV ECX,EBX
	$-2C     00AA5562     6A 01                   PUSH 0x1
	$-2A     00AA5564     8D77 2C                 LEA ESI,DWORD PTR DS:[EDI+0x2C]
	$-27     00AA5567     56                      PUSH ESI
	$-26     00AA5568     E8 E3FACEFF             CALL PathOfEx.00795050
	$-21     00AA556D     803E 00                 CMP BYTE PTR DS:[ESI],0x0
	$-1E     00AA5570     7D 15                   JGE SHORT PathOfEx.00AA5587
	$-1C     00AA5572     6A 04                   PUSH 0x4
	$-1A     00AA5574     8D77 14                 LEA ESI,DWORD PTR DS:[EDI+0x14]
	$-17     00AA5577     8BCB                    MOV ECX,EBX
	$-15     00AA5579     56                      PUSH ESI
	$-14     00AA557A     E8 D1FACEFF             CALL PathOfEx.00795050
	$-F      00AA557F     FF36                    PUSH DWORD PTR DS:[ESI]
	$-D      00AA5581     FFD5                    CALL EBP
	$-B      00AA5583     8906                    MOV DWORD PTR DS:[ESI],EAX
	$-9      00AA5585     EB 07                   JMP SHORT PathOfEx.00AA558E
	$-7      00AA5587     C747 14 00000000        MOV DWORD PTR DS:[EDI+0x14],0x0       ; 这个字段不知是干嘛的，直接就初始化为0了
	$ ==>    00AA558E     8BCB                    MOV ECX,EBX
	$+2      00AA5590     E8 CB73CBFF             CALL PathOfEx.0075C960                ; tag_收包解密dword
	$+7      00AA5595     8BCB                    MOV ECX,EBX
	$+9      00AA5597     8947 20                 MOV DWORD PTR DS:[EDI+0x20],EAX       ; stCDGR_OnMove.pos_src_.x
	$+C      00AA559A     E8 C173CBFF             CALL PathOfEx.0075C960                ; tag_收包解密dword
	$+11     00AA559F     8BCB                    MOV ECX,EBX
	$+13     00AA55A1     8947 24                 MOV DWORD PTR DS:[EDI+0x24],EAX
	$+16     00AA55A4     E8 B773CBFF             CALL PathOfEx.0075C960                ; tag_收包解密dword
	$+1B     00AA55A9     8BCB                    MOV ECX,EBX
	$+1D     00AA55AB     8947 18                 MOV DWORD PTR DS:[EDI+0x18],EAX       ; pos_dst_.x
	$+20     00AA55AE     E8 AD73CBFF             CALL PathOfEx.0075C960                ; tag_收包解密dword
	$+25     00AA55B3     6A 02                   PUSH 0x2
	$+27     00AA55B5     8D77 28                 LEA ESI,DWORD PTR DS:[EDI+0x28]
	$+2A     00AA55B8     8947 1C                 MOV DWORD PTR DS:[EDI+0x1C],EAX       ; pos_dst_.y
	$+2D     00AA55BB     56                      PUSH ESI
	$+2E     00AA55BC     8BCB                    MOV ECX,EBX
	$+30     00AA55BE     E8 8DFACEFF             CALL PathOfEx.00795050
	$+35     00AA55C3     0FB706                  MOVZX EAX,WORD PTR DS:[ESI]
	$+38     00AA55C6     8B2D 4CE5D800           MOV EBP,DWORD PTR DS:[0xD8E54C]
	$+3E     00AA55CC     50                      PUSH EAX
	$+3F     00AA55CD     FFD5                    CALL EBP
	$+41     00AA55CF     66:8906                 MOV WORD PTR DS:[ESI],AX
	$+44     00AA55D2     8BCB                    MOV ECX,EBX
	$+46     00AA55D4     6A 02                   PUSH 0x2
	$+48     00AA55D6     8D77 2A                 LEA ESI,DWORD PTR DS:[EDI+0x2A]
	$+4B     00AA55D9     56                      PUSH ESI
	$+4C     00AA55DA     E8 71FACEFF             CALL PathOfEx.00795050
	$+51     00AA55DF     0FB706                  MOVZX EAX,WORD PTR DS:[ESI]
	$+54     00AA55E2     50                      PUSH EAX
	$+55     00AA55E3     FFD5                    CALL EBP
	$+57     00AA55E5     5F                      POP EDI                               ; hook点，g_h_recv_on_move_
	$+58     00AA55E6     66:8906                 MOV WORD PTR DS:[ESI],AX
	$+5B     00AA55E9     5E                      POP ESI
	$+5C     00AA55EA     5D                      POP EBP
	$+5D     00AA55EB     5B                      POP EBX
	$+5E     00AA55EC     C2 0400                 RETN 0x4
	$+61     00AA55EF     CC                      INT3
	$+62     00AA55F0     57                      PUSH EDI
	$+63     00AA55F1     FF7424 08               PUSH DWORD PTR SS:[ESP+0x8]
	$+67     00AA55F5     8BF9                    MOV EDI,ECX
	$+69     00AA55F7     E8 64FDFFFF             CALL PathOfEx.00AA5360
	$+6E     00AA55FC     FF7424 08               PUSH DWORD PTR SS:[ESP+0x8]
	$+72     00AA5600     8D4F 14                 LEA ECX,DWORD PTR DS:[EDI+0x14]
	$+75     00AA5603     E8 B8252500             CALL PathOfEx.00CF7BC0
	$+7A     00AA5608     5F                      POP EDI
	$+7B     00AA5609     C2 0400                 RETN 0x4
	$+7E     00AA560C     CC                      INT3
	$+7F     00AA560D     CC                      INT3
	$+80     00AA560E     CC                      INT3
	$+81     00AA560F     CC                      INT3
	$+82     00AA5610     53                      PUSH EBX                              ; stCDR_Damaged
	$+83     00AA5611     8B5C24 08               MOV EBX,DWORD PTR SS:[ESP+0x8]
	$+87     00AA5615     55                      PUSH EBP
	$+88     00AA5616     8BE9                    MOV EBP,ECX
	$+8A     00AA5618     8BCB                    MOV ECX,EBX
	$+8C     00AA561A     56                      PUSH ESI
	$+8D     00AA561B     6A 04                   PUSH 0x4
	$+8F     00AA561D     8D75 08                 LEA ESI,DWORD PTR SS:[EBP+0x8]
	$+92     00AA5620     56                      PUSH ESI
	$+93     00AA5621     E8 2AFACEFF             CALL PathOfEx.00795050
	$+98     00AA5626     FF36                    PUSH DWORD PTR DS:[ESI]
	$+9A     00AA5628     FF15 FCE4D800           CALL DWORD PTR DS:[0xD8E4FC]
	$+A0     00AA562E     8906                    MOV DWORD PTR DS:[ESI],EAX
	$+A2     00AA5630     8BCB                    MOV ECX,EBX
	$+A4     00AA5632     6A 04                   PUSH 0x4
	$+A6     00AA5634     8D75 0C                 LEA ESI,DWORD PTR SS:[EBP+0xC]
	$+A9     00AA5637     56                      PUSH ESI
	$+AA     00AA5638     E8 13FACEFF             CALL PathOfEx.00795050
	$+AF     00AA563D     FF36                    PUSH DWORD PTR DS:[ESI]
	$+B1     00AA563F     FF15 FCE4D800           CALL DWORD PTR DS:[0xD8E4FC]
	$+B7     00AA5645     8906                    MOV DWORD PTR DS:[ESI],EAX
	$+B9     00AA5647     8BCB                    MOV ECX,EBX
	$+BB     00AA5649     6A 02                   PUSH 0x2
	$+BD     00AA564B     8D75 10                 LEA ESI,DWORD PTR SS:[EBP+0x10]
	$+C0     00AA564E     56                      PUSH ESI
	$+C1     00AA564F     E8 FCF9CEFF             CALL PathOfEx.00795050
	$+C6     00AA5654     0FB706                  MOVZX EAX,WORD PTR DS:[ESI]
	$+C9     00AA5657     50                      PUSH EAX
	$+CA     00AA5658     FF15 4CE5D800           CALL DWORD PTR DS:[0xD8E54C]
	$+D0     00AA565E     53                      PUSH EBX
	$+D1     00AA565F     8D4D 14                 LEA ECX,DWORD PTR SS:[EBP+0x14]
	$+D4     00AA5662     66:8906                 MOV WORD PTR DS:[ESI],AX
	$+D7     00AA5665     E8 16232500             CALL PathOfEx.00CF7980
	$+DC     00AA566A     5E                      POP ESI
	$+DD     00AA566B     5D                      POP EBP
	$+DE     00AA566C     5B                      POP EBX
	$+DF     00AA566D     C2 0400                 RETN 0x4
	$+E2     00AA5670     56                      PUSH ESI
	$+E3     00AA5671     57                      PUSH EDI
	$+E4     00AA5672     8B7C24 0C               MOV EDI,DWORD PTR SS:[ESP+0xC]
	$+E8     00AA5676     8BF1                    MOV ESI,ECX
	$+EA     00AA5678     57                      PUSH EDI
	$+EB     00AA5679     E8 E2FCFFFF             CALL PathOfEx.00AA5360
	$+F0     00AA567E     57                      PUSH EDI
	$+F1     00AA567F     8D4E 14                 LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+F4     00AA5682     E8 79262500             CALL PathOfEx.00CF7D00
	$+F9     00AA5687     8A46 1C                 MOV AL,BYTE PTR DS:[ESI+0x1C]
	$+FC     00AA568A     8BCF                    MOV ECX,EDI
	$+FE     00AA568C     884424 0C               MOV BYTE PTR SS:[ESP+0xC],AL
	$+102    00AA5690     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
	$+106    00AA5694     6A 01                   PUSH 0x1
	$+108    00AA5696     50                      PUSH EAX
	$+109    00AA5697     E8 C4FACEFF             CALL PathOfEx.00795160
	$+10E    00AA569C     5F                      POP EDI
	$+10F    00AA569D     5E                      POP ESI
	$+110    00AA569E     C2 0400                 RETN 0x4
	$+113    00AA56A1     CC                      INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??e8????????8b??89????e8????????8b??89????e8????????8b??89????e8????????6a??8d????89????568b??e8????????0f????8b??????????50ff??66????8b??6a??8d????56e8????????0f????50ff??5f66????5e",
		0x57);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_h_recv_on_move_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
