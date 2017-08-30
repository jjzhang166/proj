#pragma once
/*
@author zhp
@date 2017/1/26 11:59
@purpose for packet asm record
*/

//tag_使用技能第一个发包

//a1????????8d????518b??????????25????????6a??50e8????????e9????????83????????????74??8d
/*
$-6A     006D0CB5      84C0                   TEST AL, AL
$-68     006D0CB7      74 1E                  JE SHORT PathOfEx.006D0CD7
$-66     006D0CB9      8B87 3C010000          MOV EAX, DWORD PTR DS:[EDI+0x13C]
$-60     006D0CBF      8A80 6B3B0000          MOV AL, BYTE PTR DS:[EAX+0x3B6B]
$-5A     006D0CC5      84C0                   TEST AL, AL
$-58     006D0CC7      75 0E                  JNZ SHORT PathOfEx.006D0CD7
$-56     006D0CC9      FF75 E4                PUSH DWORD PTR SS:[EBP-0x1C]
$-53     006D0CCC      8BCF                   MOV ECX, EDI
$-51     006D0CCE      52                     PUSH EDX
$-50     006D0CCF      E8 CCF7FFFF            CALL PathOfEx.006D04A0
$-4B     006D0CD4      8B4D EC                MOV ECX, DWORD PTR SS:[EBP-0x14]         ; user32.754A0D27
$-48     006D0CD7      F3:0F1087 A0180000     MOVSS XMM0, DWORD PTR DS:[EDI+0x18A0]
$-40     006D0CDF      F3:0F5905 B0D7E600     MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
$-38     006D0CE7      F3:0F2CC0              CVTTSS2SI EAX, XMM0
$-34     006D0CEB      F3:0F1087 9C180000     MOVSS XMM0, DWORD PTR DS:[EDI+0x189C]
$-2C     006D0CF3      F3:0F5905 B0D7E600     MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
$-24     006D0CFB      8945 E0                MOV DWORD PTR SS:[EBP-0x20], EAX
$-21     006D0CFE      8945 D4                MOV DWORD PTR SS:[EBP-0x2C], EAX
$-1E     006D0D01      8B45 E4                MOV EAX, DWORD PTR SS:[EBP-0x1C]
$-1B     006D0D04      F3:0F2CD8              CVTTSS2SI EBX, XMM0
$-17     006D0D08      48                     DEC EAX
$-16     006D0D09      895D D0                MOV DWORD PTR SS:[EBP-0x30], EBX
$-13     006D0D0C      83F8 04                CMP EAX, 0x4
$-10     006D0D0F      0F87 C9010000          JA PathOfEx.006D0EDE
$-A      006D0D15    - FF2485 B80F6D00        JMP NEAR DWORD PTR DS:[EAX*4+0x6D0FB8]
$-3      006D0D1C      FF75 E8                PUSH DWORD PTR SS:[EBP-0x18]             ; user32.75496D51
$ ==>    006D0D1F      A1 0CD70001            MOV EAX, DWORD PTR DS:[0x100D70C]        ; 当前使用的技能id存放在此全局变量
$+5      006D0D24      8D4D D0                LEA ECX, DWORD PTR SS:[EBP-0x30]
$+8      006D0D27      51                     PUSH ECX
$+9      006D0D28      8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+F      006D0D2E      25 FF3F0000            AND EAX, 0x3FFF
$+14     006D0D33      6A 00                  PUSH 0x0
$+16     006D0D35      50                     PUSH EAX
$+17     006D0D36      E8 D5482000            CALL PathOfEx.008D5610                   ; 使用技能发包
	$ ==>    008D5610      55                     PUSH EBP                                           ; 第一个参数是skill_id，第四个参数是pt_byte
	$+1      008D5611      8BEC                   MOV EBP, ESP
	$+3      008D5613      64:A1 00000000         MOV EAX, DWORD PTR FS:[0]
	$+9      008D5619      6A FF                  PUSH -0x1
	$+B      008D561B      68 E832D600            PUSH PathOfEx.00D632E8                             ; ASCII "咐伫"
	$+10     008D5620      50                     PUSH EAX
	$+11     008D5621      64:8925 00000000       MOV DWORD PTR FS:[0], ESP
	$+18     008D5628      83EC 14                SUB ESP, 0x14
	$+1B     008D562B      56                     PUSH ESI
	$+1C     008D562C      8BF1                   MOV ESI, ECX
	$+1E     008D562E      57                     PUSH EDI
	$+1F     008D562F      8B8E B83C0000          MOV ECX, DWORD PTR DS:[ESI+0x3CB8]
	$+25     008D5635      85C9                   TEST ECX, ECX
	$+27     008D5637      0F84 7F010000          JE PathOfEx.008D57BC
	$+2D     008D563D      83BE 683F0000 00       CMP DWORD PTR DS:[ESI+0x3F68], 0x0
	$+34     008D5644      0F8F 72010000          JG PathOfEx.008D57BC
	$+3A     008D564A      8B7D 08                MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+3D     008D564D      66:85FF                TEST DI, DI
	$+40     008D5650      75 17                  JNZ SHORT PathOfEx.008D5669
	$+42     008D5652      0FB705 F4151301        MOVZX EAX, WORD PTR DS:[0x11315F4]
	$+49     008D5659      50                     PUSH EAX
	$+4A     008D565A      8B01                   MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00F3357C
	$+4C     008D565C      FF50 54                CALL NEAR DWORD PTR DS:[EAX+0x54]
	$+4F     008D565F      8BC8                   MOV ECX, EAX
	$+51     008D5661      E8 8AC5F3FF            CALL PathOfEx.00811BF0
	$+56     008D5666      0FB7F8                 MOVZX EDI, AX
	$+59     008D5669      8D8E 14070000          LEA ECX, DWORD PTR DS:[ESI+0x714]
	$+5F     008D566F      E8 3CDFBDFF            CALL PathOfEx.004B35B0
	$+64     008D5674      8B86 943A0000          MOV EAX, DWORD PTR DS:[ESI+0x3A94]
	$+6A     008D567A      66:89BE 643F0000       MOV WORD PTR DS:[ESI+0x3F64], DI
	$+71     008D5681      83B8 E8010000 02       CMP DWORD PTR DS:[EAX+0x1E8], 0x2
	$+78     008D5688      0F85 90000000          JNZ PathOfEx.008D571E
	$+7E     008D568E      8B45 0C                MOV EAX, DWORD PTR SS:[EBP+0xC]
	$+81     008D5691      85C0                   TEST EAX, EAX
	$+83     008D5693      74 25                  JE SHORT PathOfEx.008D56BA
	$+85     008D5695      B9 CC000000            MOV ECX, 0xCC
	$+8A     008D569A      C745 E4 DC78EF00       MOV DWORD PTR SS:[EBP-0x1C], PathOfEx.00EF78DC
	$+91     008D56A1      66:894D E8             MOV WORD PTR SS:[EBP-0x18], CX
	$+95     008D56A5      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+9C     008D56AC      8945 EC                MOV DWORD PTR SS:[EBP-0x14], EAX
	$+9F     008D56AF      8A45 14                MOV AL, BYTE PTR SS:[EBP+0x14]
	$+A2     008D56B2      8845 F2                MOV BYTE PTR SS:[EBP-0xE], AL
	$+A5     008D56B5      8D45 E4                LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+A8     008D56B8      EB 2E                  JMP SHORT PathOfEx.008D56E8
	$+AA     008D56BA      B8 CB000000            MOV EAX, 0xCB
	$+AF     008D56BF      C745 E0 2479EF00       MOV DWORD PTR SS:[EBP-0x20], PathOfEx.00EF7924     ; 下面进行发包结构体填充
	$+B6     008D56C6      66:8945 E4             MOV WORD PTR SS:[EBP-0x1C], AX                     ; msg_id
	$+BA     008D56CA      8B4D 10                MOV ECX, DWORD PTR SS:[EBP+0x10]
	$+BD     008D56CD      C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+C4     008D56D4      8B01                   MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00F3357C
	$+C6     008D56D6      8945 E8                MOV DWORD PTR SS:[EBP-0x18], EAX                   ; dst_pos_x
	$+C9     008D56D9      8B41 04                MOV EAX, DWORD PTR DS:[ECX+0x4]
	$+CC     008D56DC      8945 EC                MOV DWORD PTR SS:[EBP-0x14], EAX                   ; dst_pos_y
	$+CF     008D56DF      8A45 14                MOV AL, BYTE PTR SS:[EBP+0x14]                     ; ukn_byte_flag_
	$+D2     008D56E2      8845 F2                MOV BYTE PTR SS:[EBP-0xE], AL
	$+D5     008D56E5      8D45 E0                LEA EAX, DWORD PTR SS:[EBP-0x20]
	$+D8     008D56E8      50                     PUSH EAX
	$+D9     008D56E9      FFB6 983A0000          PUSH DWORD PTR DS:[ESI+0x3A98]
	$+DF     008D56EF      66:897D F0             MOV WORD PTR SS:[EBP-0x10], DI                     ; di=skill_id
	$+E3     008D56F3      E8 C833DEFF            CALL PathOfEx.006B8AC0
	$+E8     008D56F8      8BC8                   MOV ECX, EAX
	$+EA     008D56FA      E8 C1310100            CALL PathOfEx.008E88C0                             ; 构建发包内容
	$+EF     008D56FF      8B8E 983A0000          MOV ECX, DWORD PTR DS:[ESI+0x3A98]
	$+F5     008D5705      6A 00                  PUSH 0x0
	$+F7     008D5707      E8 54E1EBFF            CALL PathOfEx.00793860                             ; 使用技能发包
	$+FC     008D570C      8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+FF     008D570F      64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+106    008D5716      5F                     POP EDI                                            ; 492436B4
	$+107    008D5717      5E                     POP ESI                                            ; 492436B4
	$+108    008D5718      8BE5                   MOV ESP, EBP
	$+10A    008D571A      5D                     POP EBP                                            ; 492436B4
	$+10B    008D571B      C2 1000                RETN 0x10
	$+10E    008D571E      8B8E B83C0000          MOV ECX, DWORD PTR DS:[ESI+0x3CB8]
	$+114    008D5724      E8 C741F1FF            CALL PathOfEx.007E98F0
	$+119    008D5729      84C0                   TEST AL, AL
	$+11B    008D572B      74 55                  JE SHORT PathOfEx.008D5782
	$+11D    008D572D      B8 D1000000            MOV EAX, 0xD1
	$+122    008D5732      C745 EC 9879EF00       MOV DWORD PTR SS:[EBP-0x14], PathOfEx.00EF7998
	$+129    008D5739      66:8945 F0             MOV WORD PTR SS:[EBP-0x10], AX
	$+12D    008D573D      8D45 EC                LEA EAX, DWORD PTR SS:[EBP-0x14]
	$+130    008D5740      C745 FC 02000000       MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+137    008D5747      50                     PUSH EAX
	$+138    008D5748      FFB6 983A0000          PUSH DWORD PTR DS:[ESI+0x3A98]
	$+13E    008D574E      E8 6D33DEFF            CALL PathOfEx.006B8AC0
	$+143    008D5753      8BC8                   MOV ECX, EAX
	$+145    008D5755      E8 66310100            CALL PathOfEx.008E88C0
	$+14A    008D575A      8B8E 983A0000          MOV ECX, DWORD PTR DS:[ESI+0x3A98]
	$+150    008D5760      6A 00                  PUSH 0x0
	$+152    008D5762      E8 F9E0EBFF            CALL PathOfEx.00793860
	$+157    008D5767      8B8E B83C0000          MOV ECX, DWORD PTR DS:[ESI+0x3CB8]
	$+15D    008D576D      6A 00                  PUSH 0x0
	$+15F    008D576F      8B01                   MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00F3357C
	$+161    008D5771      FF50 50                CALL NEAR DWORD PTR DS:[EAX+0x50]
	$+164    008D5774      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+16B    008D577B      C745 EC 8079EF00       MOV DWORD PTR SS:[EBP-0x14], PathOfEx.00EF7980
	$+172    008D5782      8B45 0C                MOV EAX, DWORD PTR SS:[EBP+0xC]
	$+175    008D5785      8D8E 383B0000          LEA ECX, DWORD PTR DS:[ESI+0x3B38]
	$+17B    008D578B      85C0                   TEST EAX, EAX
	$+17D    008D578D      74 04                  JE SHORT PathOfEx.008D5793
	$+17F    008D578F      8901                   MOV DWORD PTR DS:[ECX], EAX
	$+181    008D5791      EB 14                  JMP SHORT PathOfEx.008D57A7
	$+183    008D5793      8B55 10                MOV EDX, DWORD PTR SS:[EBP+0x10]
	$+186    008D5796      C701 00000000          MOV DWORD PTR DS:[ECX], 0x0
	$+18C    008D579C      8B02                   MOV EAX, DWORD PTR DS:[EDX]                        ; PathOfEx.00E72CBC
	$+18E    008D579E      8941 04                MOV DWORD PTR DS:[ECX+0x4], EAX
	$+191    008D57A1      8B42 04                MOV EAX, DWORD PTR DS:[EDX+0x4]
	$+194    008D57A4      8941 08                MOV DWORD PTR DS:[ECX+0x8], EAX
	$+197    008D57A7      8A45 14                MOV AL, BYTE PTR SS:[EBP+0x14]
	$+19A    008D57AA      8841 0E                MOV BYTE PTR DS:[ECX+0xE], AL
	$+19D    008D57AD      66:8979 0C             MOV WORD PTR DS:[ECX+0xC], DI
	$+1A1    008D57B1      FFB6 B83C0000          PUSH DWORD PTR DS:[ESI+0x3CB8]
	$+1A7    008D57B7      E8 C4D01C00            CALL PathOfEx.00AA2880
	$+1AC    008D57BC      8B4D F4                MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+1AF    008D57BF      5F                     POP EDI                                            ; 492436B4
	$+1B0    008D57C0      64:890D 00000000       MOV DWORD PTR FS:[0], ECX
	$+1B7    008D57C7      5E                     POP ESI                                            ; 492436B4
	$+1B8    008D57C8      8BE5                   MOV ESP, EBP
	$+1BA    008D57CA      5D                     POP EBP                                            ; 492436B4
	$+1BB    008D57CB      C2 1000                RETN 0x10
$+1C     006D0D3B      E9 9E010000            JMP PathOfEx.006D0EDE
$+21     006D0D40      83BF BC180000 00       CMP DWORD PTR DS:[EDI+0x18BC], 0x0
$+28     006D0D47      74 38                  JE SHORT PathOfEx.006D0D81
$+2A     006D0D49      8D45 C8                LEA EAX, DWORD PTR SS:[EBP-0x38]
$+2D     006D0D4C      50                     PUSH EAX
$+2E     006D0D4D      E8 5EE5FFFF            CALL PathOfEx.006CF2B0
$+33     006D0D52      84C0                   TEST AL, AL
$+35     006D0D54      74 2B                  JE SHORT PathOfEx.006D0D81
$+37     006D0D56      FFB7 BC180000          PUSH DWORD PTR DS:[EDI+0x18BC]
$+3D     006D0D5C      8BCF                   MOV ECX, EDI
$+3F     006D0D5E      E8 4DDEFFFF            CALL PathOfEx.006CEBB0
$+44     006D0D63      84C0                   TEST AL, AL
$+46     006D0D65      74 1A                  JE SHORT PathOfEx.006D0D81
$+48     006D0D67      8B87 BC180000          MOV EAX, DWORD PTR DS:[EDI+0x18BC]
$+4E     006D0D6D      8B40 20                MOV EAX, DWORD PTR DS:[EAX+0x20]
$+51     006D0D70      8B58 10                MOV EBX, DWORD PTR DS:[EAX+0x10]
$+54     006D0D73      8B40 14                MOV EAX, DWORD PTR DS:[EAX+0x14]
$+57     006D0D76      895D D0                MOV DWORD PTR SS:[EBP-0x30], EBX
$+5A     006D0D79      8945 E0                MOV DWORD PTR SS:[EBP-0x20], EAX
$+5D     006D0D7C      8945 D4                MOV DWORD PTR SS:[EBP-0x2C], EAX
$+60     006D0D7F      EB 03                  JMP SHORT PathOfEx.006D0D84
$+62     006D0D81      8B5D D0                MOV EBX, DWORD PTR SS:[EBP-0x30]
$+65     006D0D84      FF75 E8                PUSH DWORD PTR SS:[EBP-0x18]             ; user32.75496D51
$+68     006D0D87      8B45 C8                MOV EAX, DWORD PTR SS:[EBP-0x38]
$+6B     006D0D8A      8D4D D0                LEA ECX, DWORD PTR SS:[EBP-0x30]
$+6E     006D0D8D      51                     PUSH ECX
$+6F     006D0D8E      8B8F 3C010000          MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+75     006D0D94      6A 00                  PUSH 0x0
$+77     006D0D96      0FB700                 MOVZX EAX, WORD PTR DS:[EAX]
$+7A     006D0D99      50                     PUSH EAX
$+7B     006D0D9A      E8 71482000            CALL PathOfEx.008D5610
$+80     006D0D9F      E9 3D010000            JMP PathOfEx.006D0EE1
$+85     006D0DA4      85C9                   TEST ECX, ECX
$+87     006D0DA6      0F84 32010000          JE PathOfEx.006D0EDE
$+8D     006D0DAC      8B97 3C010000          MOV EDX, DWORD PTR DS:[EDI+0x13C]
$+93     006D0DB2      8A82 AC3E0000          MOV AL, BYTE PTR DS:[EDX+0x3EAC]
$+99     006D0DB8      84C0                   TEST AL, AL
$+9B     006D0DBA      74 39                  JE SHORT PathOfEx.006D0DF5
$+9D     006D0DBC      80BF D51A0000 00       CMP BYTE PTR DS:[EDI+0x1AD5], 0x0
$+A4     006D0DC3      74 30                  JE SHORT PathOfEx.006D0DF5
$+A6     006D0DC5      83BF D81A0000 00       CMP DWORD PTR DS:[EDI+0x1AD8], 0x0
$+AD     006D0DCC      0F84 0C010000          JE PathOfEx.006D0EDE
$+B3     006D0DD2      51                     PUSH ECX
$+B4     006D0DD3      E8 68CBDFFF            CALL PathOfEx.004CD940
$+B9     006D0DD8      84C0                   TEST AL, AL
$+BB     006D0DDA      0F84 FE000000          JE PathOfEx.006D0EDE
$+C1     006D0DE0      8B8F D81A0000          MOV ECX, DWORD PTR DS:[EDI+0x1AD8]
$+C7     006D0DE6      6A 01                  PUSH 0x1
$+C9     006D0DE8      FF75 EC                PUSH DWORD PTR SS:[EBP-0x14]             ; user32.754A0D27
$+CC     006D0DEB      E8 10CCDFFF            CALL PathOfEx.004CDA00
$+D1     006D0DF0      E9 E9000000            JMP PathOfEx.006D0EDE
$+D6     006D0DF5      FF75 E8                PUSH DWORD PTR SS:[EBP-0x18]             ; user32.75496D51
$+D9     006D0DF8      A1 18D70001            MOV EAX, DWORD PTR DS:[0x100D718]
$+DE     006D0DFD      FF71 14                PUSH DWORD PTR DS:[ECX+0x14]
$+E1     006D0E00      25 FF3F0000            AND EAX, 0x3FFF
*/
/**/
//tag_获取发包对象_828
/*
$ ==>    008DD5E0     55                               PUSH EBP                                              ; 登陆界面，点击注销的时候会断住
$+1      008DD5E1     8BEC                             MOV EBP, ESP
$+3      008DD5E3     6A FF                            PUSH -0x1
$+5      008DD5E5     68 8AC7D500                      PUSH PathOfEx.00D5C78A
$+A      008DD5EA     64:A1 00000000                   MOV EAX, DWORD PTR FS:[0]
$+10     008DD5F0     50                               PUSH EAX
$+11     008DD5F1     64:8925 00000000                 MOV DWORD PTR FS:[0], ESP
$+18     008DD5F8     81EC 8C000000                    SUB ESP, 0x8C
$+1E     008DD5FE     53                               PUSH EBX
$+1F     008DD5FF     56                               PUSH ESI
$+20     008DD600     57                               PUSH EDI
$+21     008DD601     6A 28                            PUSH 0x28
$+23     008DD603     8BD9                             MOV EBX, ECX
$+25     008DD605     E8 58523900                      CALL PathOfEx.00C72862
$+2A     008DD60A     8945 F0                          MOV DWORD PTR SS:[EBP-0x10], EAX
$+2D     008DD60D     8BC8                             MOV ECX, EAX
$+2F     008DD60F     C745 FC 00000000                 MOV DWORD PTR SS:[EBP-0x4], 0x0
$+36     008DD616     E8 65F0FFFF                      CALL PathOfEx.008DC680
$+3B     008DD61B     8BB3 2C080000                    MOV ESI, DWORD PTR DS:[EBX+0x82C]
$+41     008DD621     8983 2C080000                    MOV DWORD PTR DS:[EBX+0x82C], EAX
$+47     008DD627     85F6                             TEST ESI, ESI
$+49     008DD629     74 12                            JE SHORT PathOfEx.008DD63D
$+4B     008DD62B     8BCE                             MOV ECX, ESI
$+4D     008DD62D     E8 FE250000                      CALL PathOfEx.008DFC30
$+52     008DD632     6A 28                            PUSH 0x28
$+54     008DD634     56                               PUSH ESI
$+55     008DD635     E8 1A523900                      CALL PathOfEx.00C72854
$+5A     008DD63A     83C4 08                          ADD ESP, 0x8
$+5D     008DD63D     B8 01000000                      MOV EAX, 0x1
$+62     008DD642     C785 68FFFFFF F417EF00           MOV DWORD PTR SS:[EBP-0x98], PathOfEx.00EF17F4
$+6C     008DD64C     66:8985 6CFFFFFF                 MOV WORD PTR SS:[EBP-0x94], AX
$+73     008DD653     8945 FC                          MOV DWORD PTR SS:[EBP-0x4], EAX
$+76     008DD656     8DBD 70FFFFFF                    LEA EDI, DWORD PTR SS:[EBP-0x90]
$+7C     008DD65C     8BB3 2C080000                    MOV ESI, DWORD PTR DS:[EBX+0x82C]
$+82     008DD662     B9 20000000                      MOV ECX, 0x20
$+87     008DD667     50                               PUSH EAX
$+88     008DD668     8B76 18                          MOV ESI, DWORD PTR DS:[ESI+0x18]
$+8B     008DD66B     F3:A5                            REP MOVS DWORD PTR ES:[EDI], DWORD PTR DS:[ESI]
$+8D     008DD66D     8BB3 28080000                    MOV ESI, DWORD PTR DS:[EBX+0x828]
$+93     008DD673     FF15 4455D800                    CALL NEAR DWORD PTR DS:[0xD85544]
$+99     008DD679     0FB7C0                           MOVZX EAX, AX
$+9C     008DD67C     8BCE                             MOV ECX, ESI
$+9E     008DD67E     8945 F0                          MOV DWORD PTR SS:[EBP-0x10], EAX
$+A1     008DD681     8D45 F0                          LEA EAX, DWORD PTR SS:[EBP-0x10]
$+A4     008DD684     6A 02                            PUSH 0x2
$+A6     008DD686     50                               PUSH EAX
$+A7     008DD687     E8 9401EBFF                      CALL PathOfEx.0078D820                                ; 构建发包内容
$+AC     008DD68C     68 80000000                      PUSH 0x80
$+B1     008DD691     8D85 70FFFFFF                    LEA EAX, DWORD PTR SS:[EBP-0x90]
$+B7     008DD697     8BCE                             MOV ECX, ESI
$+B9     008DD699     50                               PUSH EAX
$+BA     008DD69A     E8 8101EBFF                      CALL PathOfEx.0078D820                                ; 构建发包内容
$+BF     008DD69F     8B8B 28080000                    MOV ECX, DWORD PTR DS:[EBX+0x828]
$+C5     008DD6A5     6A 00                            PUSH 0x0
$+C7     008DD6A7     E8 9402EBFF                      CALL PathOfEx.0078D940                                ; call发包
$+CC     008DD6AC     8B4D F4                          MOV ECX, DWORD PTR SS:[EBP-0xC]
$+CF     008DD6AF     5F                               POP EDI                                               ; 233504B0
$+D0     008DD6B0     5E                               POP ESI                                               ; 233504B0
$+D1     008DD6B1     5B                               POP EBX                                               ; 233504B0
$+D2     008DD6B2     64:890D 00000000                 MOV DWORD PTR FS:[0], ECX
$+D9     008DD6B9     8BE5                             MOV ESP, EBP
$+DB     008DD6BB     5D                               POP EBP                                               ; 233504B0
$+DC     008DD6BC     C3                               RETN
*/
//tag_获取发包对象的上层对象_28
/*
$-A7     006B73FD     FF10                             CALL NEAR DWORD PTR DS:[EAX]
$-A5     006B73FF     8B4D 10                          MOV ECX, DWORD PTR SS:[EBP+0x10]
$-A2     006B7402     8BC6                             MOV EAX, ESI
$-A0     006B7404     F0:0FC141 08                     LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$-9B     006B7409     75 05                            JNZ SHORT PathOfEx.006B7410
$-99     006B740B     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$-97     006B740D     FF50 04                          CALL NEAR DWORD PTR DS:[EAX+0x4]
$-94     006B7410     C645 FC 08                       MOV BYTE PTR SS:[EBP-0x4], 0x8
$-90     006B7414     FF77 30                          PUSH DWORD PTR DS:[EDI+0x30]
$-8D     006B7417     8B4D E4                          MOV ECX, DWORD PTR SS:[EBP-0x1C]
$-8A     006B741A     E8 E1B40000                      CALL PathOfEx.006C2900
$-85     006B741F     8B47 30                          MOV EAX, DWORD PTR DS:[EDI+0x30]
$-82     006B7422     8945 10                          MOV DWORD PTR SS:[EBP+0x10], EAX
$-7F     006B7425     C747 30 00000000                 MOV DWORD PTR DS:[EDI+0x30], 0x0
$-78     006B742C     85C0                             TEST EAX, EAX
$-76     006B742E     74 14                            JE SHORT PathOfEx.006B7444
$-74     006B7430     8BC8                             MOV ECX, EAX
$-72     006B7432     E8 E9350000                      CALL PathOfEx.006BAA20
$-6D     006B7437     6A 78                            PUSH 0x78
$-6B     006B7439     FF75 10                          PUSH DWORD PTR SS:[EBP+0x10]
$-68     006B743C     E8 13B45B00                      CALL PathOfEx.00C72854
$-63     006B7441     83C4 08                          ADD ESP, 0x8
$-60     006B7444     C745 FC 0F000000                 MOV DWORD PTR SS:[EBP-0x4], 0xF
$-59     006B744B     8B7D E8                          MOV EDI, DWORD PTR SS:[EBP-0x18]
$-56     006B744E     85FF                             TEST EDI, EDI
$-54     006B7450     0F84 86020000                    JE PathOfEx.006B76DC
$-4E     006B7456     8BC6                             MOV EAX, ESI
$-4C     006B7458     F0:0FC147 04                     LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
$-47     006B745D     0F85 79020000                    JNZ PathOfEx.006B76DC
$-41     006B7463     8B07                             MOV EAX, DWORD PTR DS:[EDI]                           ; PathOfEx.00EA1668
$-3F     006B7465     8BCF                             MOV ECX, EDI
$-3D     006B7467     FF10                             CALL NEAR DWORD PTR DS:[EAX]
$-3B     006B7469     F0:0FC177 08                     LOCK XADD DWORD PTR DS:[EDI+0x8], ESI
$-36     006B746E     4E                               DEC ESI
$-35     006B746F     0F85 67020000                    JNZ PathOfEx.006B76DC
$-2F     006B7475     8B07                             MOV EAX, DWORD PTR DS:[EDI]                           ; PathOfEx.00EA1668
$-2D     006B7477     8BCF                             MOV ECX, EDI
$-2B     006B7479     FF50 04                          CALL NEAR DWORD PTR DS:[EAX+0x4]
$-28     006B747C     8B4D F4                          MOV ECX, DWORD PTR SS:[EBP-0xC]
$-25     006B747F     64:890D 00000000                 MOV DWORD PTR FS:[0], ECX
$-1E     006B7486     5F                               POP EDI                                               ; 0D4EB1D8
$-1D     006B7487     5E                               POP ESI                                               ; 0D4EB1D8
$-1C     006B7488     8BE5                             MOV ESP, EBP
$-1A     006B748A     5D                               POP EBP                                               ; 0D4EB1D8
$-19     006B748B     C2 0C00                          RETN 0xC
$-16     006B748E     837F 28 00                       CMP DWORD PTR DS:[EDI+0x28], 0x0                      ; 存放的是发包对象上层对象的地址
$-12     006B7492     8D57 28                          LEA EDX, DWORD PTR DS:[EDI+0x28]
$-F      006B7495     0F94C0                           SETE AL
$-C      006B7498     84C0                             TEST AL, AL
$-A      006B749A     0F94C0                           SETE AL
$-7      006B749D     84C0                             TEST AL, AL
$-5      006B749F     74 56                            JE SHORT PathOfEx.006B74F7
$-3      006B74A1     8D4D C0                          LEA ECX, DWORD PTR SS:[EBP-0x40]
$ ==>    006B74A4     E8 D7270000                      CALL PathOfEx.006B9C80                                ; 未知获取
$+5      006B74A9     FF75 10                          PUSH DWORD PTR SS:[EBP+0x10]
$+8      006B74AC     C745 FC 10000000                 MOV DWORD PTR SS:[EBP-0x4], 0x10
$+F      006B74B3     8B08                             MOV ECX, DWORD PTR DS:[EAX]
$+11     006B74B5     FF75 0C                          PUSH DWORD PTR SS:[EBP+0xC]
$+14     006B74B8     FF75 08                          PUSH DWORD PTR SS:[EBP+0x8]                           ; PathOfEx.006109A0
$+17     006B74BB     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$+19     006B74BD     FF10                             CALL NEAR DWORD PTR DS:[EAX]                          ; 函数内部的内部会调用发包
$+1B     006B74BF     C745 FC 11000000                 MOV DWORD PTR SS:[EBP-0x4], 0x11
$+22     006B74C6     8B4D C4                          MOV ECX, DWORD PTR SS:[EBP-0x3C]
$+25     006B74C9     894D 0C                          MOV DWORD PTR SS:[EBP+0xC], ECX
$+28     006B74CC     85C9                             TEST ECX, ECX
$+2A     006B74CE     74 20                            JE SHORT PathOfEx.006B74F0
$+2C     006B74D0     83CE FF                          OR ESI, -0x1
$+2F     006B74D3     8BC6                             MOV EAX, ESI
$+31     006B74D5     F0:0FC141 04                     LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
$+36     006B74DA     75 14                            JNZ SHORT PathOfEx.006B74F0
$+38     006B74DC     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$+3A     006B74DE     FF10                             CALL NEAR DWORD PTR DS:[EAX]
$+3C     006B74E0     8B4D 0C                          MOV ECX, DWORD PTR SS:[EBP+0xC]
$+3F     006B74E3     F0:0FC171 08                     LOCK XADD DWORD PTR DS:[ECX+0x8], ESI
$+44     006B74E8     4E                               DEC ESI
$+45     006B74E9     75 05                            JNZ SHORT PathOfEx.006B74F0
$+47     006B74EB     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$+49     006B74ED     FF50 04                          CALL NEAR DWORD PTR DS:[EAX+0x4]
$+4C     006B74F0     C745 FC FFFFFFFF                 MOV DWORD PTR SS:[EBP-0x4], -0x1
$+53     006B74F7     8BCF                             MOV ECX, EDI
$+55     006B74F9     E8 A271FEFF                      CALL PathOfEx.0069E6A0
$+5A     006B74FE     8B45 10                          MOV EAX, DWORD PTR SS:[EBP+0x10]
$+5D     006B7501     8D4F 40                          LEA ECX, DWORD PTR DS:[EDI+0x40]
$+60     006B7504     0187 540B0000                    ADD DWORD PTR DS:[EDI+0xB54], EAX
$+66     006B750A     8B87 540B0000                    MOV EAX, DWORD PTR DS:[EDI+0xB54]
$+6C     006B7510     6A 01                            PUSH 0x1
$+6E     006B7512     6A 00                            PUSH 0x0
$+70     006B7514     68 00020000                      PUSH 0x200
$+75     006B7519     66:0F6EC0                        MOVD MM0, EAX
$+79     006B751D     F30FE6C0                         CVTDQ2PD XMM0, XMM0
$+7D     006B7521     C1E8 1F                          SHR EAX, 0x1F
$+80     006B7524     F20F5804C5 806AF300              ADDSD XMM0, QWORD PTR DS:[EAX*8+0xF36A80]
$+89     006B752D     660F5AC0                         CVTPD2PS XMM0, XMM0
$+8D     006B7531     F3:0F5905 D859F300               MULSS XMM0, DWORD PTR DS:[0xF359D8]
$+95     006B7539     F3:0F1187 580B0000               MOVSS DWORD PTR DS:[EDI+0xB58], XMM0
$+9D     006B7541     E8 8AAF3500                      CALL PathOfEx.00A124D0
$+A2     006B7546     8B45 10                          MOV EAX, DWORD PTR SS:[EBP+0x10]
$+A5     006B7549     8D4F 40                          LEA ECX, DWORD PTR DS:[EDI+0x40]
$+A8     006B754C     FFB7 E40A0000                    PUSH DWORD PTR DS:[EDI+0xAE4]
$+AE     006B7552     F3:0F1097 580B0000               MOVSS XMM2, DWORD PTR DS:[EDI+0xB58]
$+B6     006B755A     66:0F6EC0                        MOVD MM0, EAX
$+BA     006B755E     F30FE6C0                         CVTDQ2PD XMM0, XMM0
$+BE     006B7562     C1E8 1F                          SHR EAX, 0x1F
$+C1     006B7565     F20F5804C5 806AF300              ADDSD XMM0, QWORD PTR DS:[EAX*8+0xF36A80]
$+CA     006B756E     660F5AC0                         CVTPD2PS XMM0, XMM0
$+CE     006B7572     F3:0F1145 EC                     MOVSS DWORD PTR SS:[EBP-0x14], XMM0
$+D3     006B7577     F3:0F5905 D859F300               MULSS XMM0, DWORD PTR DS:[0xF359D8]
$+DB     006B757F     0F28D8                           MOVAPS XMM3, XMM0
$+DE     006B7582     F3:0F1145 0C                     MOVSS DWORD PTR SS:[EBP+0xC], XMM0
$+E3     006B7587     E8 04963500                      CALL PathOfEx.00A10B90
$+E8     006B758C     8B4F 38                          MOV ECX, DWORD PTR DS:[EDI+0x38]
$+EB     006B758F     85C9                             TEST ECX, ECX
$+ED     006B7591     74 0A                            JE SHORT PathOfEx.006B759D
$+EF     006B7593     F3:0F104D 0C                     MOVSS XMM1, DWORD PTR SS:[EBP+0xC]
$+F4     006B7598     E8 B350FFFF                      CALL PathOfEx.006AC650
$+F9     006B759D     8B4F 3C                          MOV ECX, DWORD PTR DS:[EDI+0x3C]
$+FC     006B75A0     85C9                             TEST ECX, ECX
$+FE     006B75A2     74 05                            JE SHORT PathOfEx.006B75A9
$+100    006B75A4     E8 37383000                      CALL PathOfEx.009BADE0
$+105    006B75A9     837F 3C 00                       CMP DWORD PTR DS:[EDI+0x3C], 0x0
$+109    006B75AD     0F84 29010000                    JE PathOfEx.006B76DC
$+10F    006B75B3     E8 68F12300                      CALL PathOfEx.008F6720
$+114    006B75B8     8D8D 50FFFFFF                    LEA ECX, DWORD PTR SS:[EBP-0xB0]
$+11A    006B75BE     51                               PUSH ECX
$+11B    006B75BF     6A 00                            PUSH 0x0
$+11D    006B75C1     8B00                             MOV EAX, DWORD PTR DS:[EAX]
$+11F    006B75C3     FF70 64                          PUSH DWORD PTR DS:[EAX+0x64]
$+122    006B75C6     FF15 DCA6FE00                    CALL NEAR DWORD PTR DS:[0xFEA6DC]                     ; fmodstud.FMOD::Studio::System::getListenerAttributes
$+128    006B75CC     F3:0F1085 50FFFFFF               MOVSS XMM0, DWORD PTR SS:[EBP-0xB0]
$+130    006B75D4     8D4D BC                          LEA ECX, DWORD PTR SS:[EBP-0x44]
$+133    006B75D7     8B77 3C                          MOV ESI, DWORD PTR DS:[EDI+0x3C]
$+136    006B75DA     F3:0F1025 406BF300               MOVSS XMM4, DWORD PTR DS:[0xF36B40]
$+13E    006B75E2     F3:0F1015 485EF300               MOVSS XMM2, DWORD PTR DS:[0xF35E48]
$+146    006B75EA     F3:0F1145 E0                     MOVSS DWORD PTR SS:[EBP-0x20], XMM0
$+14B    006B75EF     F3:0F1085 54FFFFFF               MOVSS XMM0, DWORD PTR SS:[EBP-0xAC]
$+153    006B75F7     8D96 B0010000                    LEA EDX, DWORD PTR DS:[ESI+0x1B0]
$+159    006B75FD     F3:0F1145 E4                     MOVSS DWORD PTR SS:[EBP-0x1C], XMM0
$+15E    006B7602     F3:0F1085 58FFFFFF               MOVSS XMM0, DWORD PTR SS:[EBP-0xA8]
$+166    006B760A     0F57C4                           XORPS XMM0, XMM4
$+169    006B760D     F3:0F1145 E8                     MOVSS DWORD PTR SS:[EBP-0x18], XMM0
$+16E    006B7612     E8 09EDFFFF                      CALL PathOfEx.006B6320
$+173    006B7617     837D 10 00                       CMP DWORD PTR SS:[EBP+0x10], 0x0
$+177    006B761B     8D45 E0                          LEA EAX, DWORD PTR SS:[EBP-0x20]
$+17A    006B761E     F3:0F1045 C4                     MOVSS XMM0, DWORD PTR SS:[EBP-0x3C]
$+17F    006B7623     0F57C4                           XORPS XMM0, XMM4
$+182    006B7626     F3:0F1145 C4                     MOVSS DWORD PTR SS:[EBP-0x3C], XMM0
$+187    006B762B     76 1F                            JBE SHORT PathOfEx.006B764C
$+189    006B762D     8BD1                             MOV EDX, ECX
$+18B    006B762F     8D4D 80                          LEA ECX, DWORD PTR SS:[EBP-0x80]
$+18E    006B7632     50                               PUSH EAX
$+18F    006B7633     E8 F8D1DDFF                      CALL PathOfEx.00494830
$+194    006B7638     F3:0F1055 EC                     MOVSS XMM2, DWORD PTR SS:[EBP-0x14]
$+199    006B763D     8D4D 98                          LEA ECX, DWORD PTR SS:[EBP-0x68]
$+19C    006B7640     83C4 04                          ADD ESP, 0x4
$+19F    006B7643     8BD0                             MOV EDX, EAX
$+1A1    006B7645     E8 D6ECFFFF                      CALL PathOfEx.006B6320
$+1A6    006B764A     EB 15                            JMP SHORT PathOfEx.006B7661
$+1A8    006B764C     C745 E0 00000000                 MOV DWORD PTR SS:[EBP-0x20], 0x0
$+1AF    006B7653     C745 E4 00000000                 MOV DWORD PTR SS:[EBP-0x1C], 0x0
$+1B6    006B765A     C745 E8 00000000                 MOV DWORD PTR SS:[EBP-0x18], 0x0
$+1BD    006B7661     F30F7E00                         MOVQ XMM0, QWORD PTR DS:[EAX]
$+1C1    006B7665     8B40 08                          MOV EAX, DWORD PTR DS:[EAX+0x8]
$+1C4    006B7668     660FD645 C8                      MOVQ QWORD PTR SS:[EBP-0x38], XMM0
$+1C9    006B766D     F3:0F1086 EC010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1EC]
$+1D1    006B7675     F3:0F1145 D4                     MOVSS DWORD PTR SS:[EBP-0x2C], XMM0
$+1D6    006B767A     F3:0F1086 F0010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1F0]
$+1DE    006B7682     F3:0F1145 D8                     MOVSS DWORD PTR SS:[EBP-0x28], XMM0
$+1E3    006B7687     F3:0F1086 F4010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1F4]
$+1EB    006B768F     0F57C4                           XORPS XMM0, XMM4
$+1EE    006B7692     8945 D0                          MOV DWORD PTR SS:[EBP-0x30], EAX
$+1F1    006B7695     F3:0F1145 DC                     MOVSS DWORD PTR SS:[EBP-0x24], XMM0
$+1F6    006B769A     F3:0F1086 D8010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1D8]
$+1FE    006B76A2     F3:0F1145 E0                     MOVSS DWORD PTR SS:[EBP-0x20], XMM0
$+203    006B76A7     F3:0F1086 DC010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1DC]
$+20B    006B76AF     F3:0F1145 E4                     MOVSS DWORD PTR SS:[EBP-0x1C], XMM0
$+210    006B76B4     F3:0F1086 E0010000               MOVSS XMM0, DWORD PTR DS:[ESI+0x1E0]
$+218    006B76BC     0F57C4                           XORPS XMM0, XMM4
$+21B    006B76BF     F3:0F1145 E8                     MOVSS DWORD PTR SS:[EBP-0x18], XMM0
$+220    006B76C4     E8 57F02300                      CALL PathOfEx.008F6720
$+225    006B76C9     8D4D D4                          LEA ECX, DWORD PTR SS:[EBP-0x2C]
$+228    006B76CC     51                               PUSH ECX
$+229    006B76CD     8D4D C8                          LEA ECX, DWORD PTR SS:[EBP-0x38]
$+22C    006B76D0     51                               PUSH ECX
$+22D    006B76D1     8D4D BC                          LEA ECX, DWORD PTR SS:[EBP-0x44]
$+230    006B76D4     51                               PUSH ECX
$+231    006B76D5     8B08                             MOV ECX, DWORD PTR DS:[EAX]
$+233    006B76D7     E8 D4EB2300                      CALL PathOfEx.008F62B0
$+238    006B76DC     8B4D F4                          MOV ECX, DWORD PTR SS:[EBP-0xC]
$+23B    006B76DF     5F                               POP EDI                                               ; 0D4EB1D8
$+23C    006B76E0     64:890D 00000000                 MOV DWORD PTR FS:[0], ECX
$+243    006B76E7     5E                               POP ESI                                               ; 0D4EB1D8
$+244    006B76E8     8BE5                             MOV ESP, EBP
$+246    006B76EA     5D                               POP EBP                                               ; 0D4EB1D8
$+247    006B76EB     C2 0C00                          RETN 0xC
*/
//tag_游戏基址索引ui对象偏移_38
/*
$ ==>    0069DEA0     55                               PUSH EBP
$+1      0069DEA1     8BEC                             MOV EBP, ESP
$+3      0069DEA3     83E4 F8                          AND ESP, -0x8
$+6      0069DEA6     51                               PUSH ECX
$+7      0069DEA7     53                               PUSH EBX
$+8      0069DEA8     8BD9                             MOV EBX, ECX
$+A      0069DEAA     56                               PUSH ESI
$+B      0069DEAB     57                               PUSH EDI
$+C      0069DEAC     8B73 1C                          MOV ESI, DWORD PTR DS:[EBX+0x1C]
$+F      0069DEAF     8B7B 20                          MOV EDI, DWORD PTR DS:[EBX+0x20]
$+12     0069DEB2     8B55 10                          MOV EDX, DWORD PTR SS:[EBP+0x10]
$+15     0069DEB5     3BF7                             CMP ESI, EDI
$+17     0069DEB7     74 12                            JE SHORT PathOfEx.0069DECB
$+19     0069DEB9     8B0E                             MOV ECX, DWORD PTR DS:[ESI]
$+1B     0069DEBB     52                               PUSH EDX
$+1C     0069DEBC     FF75 0C                          PUSH DWORD PTR SS:[EBP+0xC]
$+1F     0069DEBF     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$+21     0069DEC1     FF75 08                          PUSH DWORD PTR SS:[EBP+0x8]
$+24     0069DEC4     FF10                             CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00F307B0
$+26     0069DEC6     83C6 04                          ADD ESI, 0x4
$+29     0069DEC9   ^ EB E7                            JMP SHORT PathOfEx.0069DEB2
$+2B     0069DECB     807B 44 00                       CMP BYTE PTR DS:[EBX+0x44], 0x0
$+2F     0069DECF     74 13                            JE SHORT PathOfEx.0069DEE4
$+31     0069DED1     8D43 48                          LEA EAX, DWORD PTR DS:[EBX+0x48]
$+34     0069DED4     50                               PUSH EAX
$+35     0069DED5     8D4B 38                          LEA ECX, DWORD PTR DS:[EBX+0x38]
$+38     0069DED8     E8 039BE3FF                      CALL PathOfEx.004D79E0
$+3D     0069DEDD     8B55 10                          MOV EDX, DWORD PTR SS:[EBP+0x10]
$+40     0069DEE0     C643 44 00                       MOV BYTE PTR DS:[EBX+0x44], 0x0
$+44     0069DEE4     8B73 38                          MOV ESI, DWORD PTR DS:[EBX+0x38]                  ; ui对象begin地址
$+47     0069DEE7     8B7B 3C                          MOV EDI, DWORD PTR DS:[EBX+0x3C]                  ; ui对象end地址
$+4A     0069DEEA     66:0F1F4400 00                   NOP WORD PTR DS:[EAX+EAX]
$+50     0069DEF0     3BF7                             CMP ESI, EDI                                      ; 循环开始。每次地址+4执行函数
$+52     0069DEF2     74 15                            JE SHORT PathOfEx.0069DF09
$+54     0069DEF4     8B0E                             MOV ECX, DWORD PTR DS:[ESI]
$+56     0069DEF6     52                               PUSH EDX
$+57     0069DEF7     FF75 0C                          PUSH DWORD PTR SS:[EBP+0xC]
$+5A     0069DEFA     8B01                             MOV EAX, DWORD PTR DS:[ECX]
$+5C     0069DEFC     FF75 08                          PUSH DWORD PTR SS:[EBP+0x8]
$+5F     0069DEFF     FF10                             CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00F307B0
$+61     0069DF01     8B55 10                          MOV EDX, DWORD PTR SS:[EBP+0x10]
$+64     0069DF04     83C6 04                          ADD ESI, 0x4
$+67     0069DF07   ^ EB E7                            JMP SHORT PathOfEx.0069DEF0                       ; 循环结束
$+69     0069DF09     5F                               POP EDI                                           ; usp10.76C95E0D
$+6A     0069DF0A     5E                               POP ESI                                           ; usp10.76C95E0D
$+6B     0069DF0B     5B                               POP EBX                                           ; usp10.76C95E0D
$+6C     0069DF0C     8BE5                             MOV ESP, EBP
$+6E     0069DF0E     5D                               POP EBP                                           ; usp10.76C95E0D
$+6F     0069DF0F     C2 0C00                          RETN 0xC
*/
//tag_获取UI对象中的成员变量偏移_10
/*
$-E3     0069F011        FF10                         CALL NEAR DWORD PTR DS:[EAX]
$-E1     0069F013        8B4D E8                      MOV ECX, DWORD PTR SS:[EBP-0x18]
$-DE     0069F016        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$-DC     0069F018        F0:0FC141 08                 LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$-D7     0069F01D        75 05                        JNZ SHORT PathOfEx.0069F024
$-D5     0069F01F        8B01                         MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$-D3     0069F021        FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
$-D0     0069F024        8B55 EC                      MOV EDX, DWORD PTR SS:[EBP-0x14]                   ; PathOfEx.004E47CE
$-CD     0069F027        C645 FC 01                   MOV BYTE PTR SS:[EBP-0x4], 0x1
$-C9     0069F02B        8347 10 FF                   ADD DWORD PTR DS:[EDI+0x10], -0x1
$-C5     0069F02F        75 09                        JNZ SHORT PathOfEx.0069F03A
$-C3     0069F031        C747 0C 00000000             MOV DWORD PTR DS:[EDI+0xC], 0x0
$-BC     0069F038        EB 03                        JMP SHORT PathOfEx.0069F03D
$-BA     0069F03A        FF47 0C                      INC DWORD PTR DS:[EDI+0xC]
$-B7     0069F03D        8B45 E4                      MOV EAX, DWORD PTR SS:[EBP-0x1C]
$-B4     0069F040        83E8 00                      SUB EAX, 0x0
$-B1     0069F043        74 61                        JE SHORT PathOfEx.0069F0A6
$-AF     0069F045        83E8 01                      SUB EAX, 0x1
$-AC     0069F048        75 67                        JNZ SHORT PathOfEx.0069F0B1
$-AA     0069F04A        83EC 08                      SUB ESP, 0x8
$-A7     0069F04D        8BCC                         MOV ECX, ESP
$-A5     0069F04F        894D E8                      MOV DWORD PTR SS:[EBP-0x18], ECX
$-A2     0069F052        C645 FC 03                   MOV BYTE PTR SS:[EBP-0x4], 0x3
$-9E     0069F056        8901                         MOV DWORD PTR DS:[ECX], EAX
$-9C     0069F058        8941 04                      MOV DWORD PTR DS:[ECX+0x4], EAX
$-99     0069F05B        85F6                         TEST ESI, ESI
$-97     0069F05D        74 04                        JE SHORT PathOfEx.0069F063
$-95     0069F05F        F0:FF46 04                   LOCK INC DWORD PTR DS:[ESI+0x4]
$-91     0069F063        8B49 04                      MOV ECX, DWORD PTR DS:[ECX+0x4]
$-8E     0069F066        894D E4                      MOV DWORD PTR SS:[EBP-0x1C], ECX
$-8B     0069F069        85C9                         TEST ECX, ECX
$-89     0069F06B        74 1E                        JE SHORT PathOfEx.0069F08B
$-87     0069F06D        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$-85     0069F06F        F0:0FC141 04                 LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
$-80     0069F074        75 15                        JNZ SHORT PathOfEx.0069F08B
$-7E     0069F076        8B01                         MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$-7C     0069F078        FF10                         CALL NEAR DWORD PTR DS:[EAX]
$-7A     0069F07A        8B4D E4                      MOV ECX, DWORD PTR SS:[EBP-0x1C]
$-77     0069F07D        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$-75     0069F07F        F0:0FC141 08                 LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$-70     0069F084        75 05                        JNZ SHORT PathOfEx.0069F08B
$-6E     0069F086        8B01                         MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$-6C     0069F088        FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
$-69     0069F08B        8B45 E8                      MOV EAX, DWORD PTR SS:[EBP-0x18]
$-66     0069F08E        8B4D E0                      MOV ECX, DWORD PTR SS:[EBP-0x20]                   ; PathOfEx.004E47CE
$-63     0069F091        6A 00                        PUSH 0x0
$-61     0069F093        C645 FC 01                   MOV BYTE PTR SS:[EBP-0x4], 0x1
$-5D     0069F097        8908                         MOV DWORD PTR DS:[EAX], ECX
$-5B     0069F099        8B4D F0                      MOV ECX, DWORD PTR SS:[EBP-0x10]
$-58     0069F09C        8970 04                      MOV DWORD PTR DS:[EAX+0x4], ESI
$-55     0069F09F        E8 DCF9FFFF                  CALL PathOfEx.0069EA80
$-50     0069F0A4        EB 0B                        JMP SHORT PathOfEx.0069F0B1
$-4E     0069F0A6        8B4D F0                      MOV ECX, DWORD PTR SS:[EBP-0x10]
$-4B     0069F0A9        6A 00                        PUSH 0x0
$-49     0069F0AB        52                           PUSH EDX
$-48     0069F0AC        E8 FFF6FFFF                  CALL PathOfEx.0069E7B0
$-43     0069F0B1        C745 FC 04000000             MOV DWORD PTR SS:[EBP-0x4], 0x4
$-3C     0069F0B8        85F6                         TEST ESI, ESI
$-3A     0069F0BA        74 1F                        JE SHORT PathOfEx.0069F0DB
$-38     0069F0BC        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$-36     0069F0BE        F0:0FC146 04                 LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
$-31     0069F0C3        75 16                        JNZ SHORT PathOfEx.0069F0DB
$-2F     0069F0C5        8B06                         MOV EAX, DWORD PTR DS:[ESI]
$-2D     0069F0C7        8BCE                         MOV ECX, ESI
$-2B     0069F0C9        FF10                         CALL NEAR DWORD PTR DS:[EAX]
$-29     0069F0CB        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$-27     0069F0CD        F0:0FC146 08                 LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
$-22     0069F0D2        75 07                        JNZ SHORT PathOfEx.0069F0DB
$-20     0069F0D4        8B06                         MOV EAX, DWORD PTR DS:[ESI]
$-1E     0069F0D6        8BCE                         MOV ECX, ESI
$-1C     0069F0D8        FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
$-19     0069F0DB        C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$-12     0069F0E2        837F 10 00                   CMP DWORD PTR DS:[EDI+0x10], 0x0
$-E      0069F0E6        0F94C0                       SETE AL
$-B      0069F0E9        84C0                         TEST AL, AL
$-9      0069F0EB      ^ 0F84 67FEFFFF                JE PathOfEx.0069EF58
$-3      0069F0F1        8B7D F0                      MOV EDI, DWORD PTR SS:[EBP-0x10]                   ; [ebp-10]=this
$ ==>    0069F0F4        8B47 10                      MOV EAX, DWORD PTR DS:[EDI+0x10]                   ; 这个0x10是个偏移，获取成员变量
$+3      0069F0F7        8D4F 10                      LEA ECX, DWORD PTR DS:[EDI+0x10]                   ; 好像是个pair的成员变量，像是存放的是begin和end指针
$+6      0069F0FA        3B41 04                      CMP EAX, DWORD PTR DS:[ECX+0x4]
$+9      0069F0FD        894D EC                      MOV DWORD PTR SS:[EBP-0x14], ECX
$+C      0069F100        0F94C0                       SETE AL
$+F      0069F103        84C0                         TEST AL, AL
$+11     0069F105        0F85 04010000                JNZ PathOfEx.0069F20F
$+17     0069F10B        FF47 1C                      INC DWORD PTR DS:[EDI+0x1C]
$+1A     0069F10E        8B71 04                      MOV ESI, DWORD PTR DS:[ECX+0x4]
$+1D     0069F111        3B31                         CMP ESI, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$+1F     0069F113        0F84 BF000000                JE PathOfEx.0069F1D8
$+25     0069F119        0F1F80 00000000              NOP DWORD PTR DS:[EAX]
$+2C     0069F120        8B7E FC                      MOV EDI, DWORD PTR DS:[ESI-0x4]
$+2F     0069F123        83C6 F8                      ADD ESI, -0x8
$+32     0069F126        8B0E                         MOV ECX, DWORD PTR DS:[ESI]
$+34     0069F128        894D E0                      MOV DWORD PTR SS:[EBP-0x20], ECX
$+37     0069F12B        85FF                         TEST EDI, EDI
$+39     0069F12D        74 04                        JE SHORT PathOfEx.0069F133
$+3B     0069F12F        F0:FF47 04                   LOCK INC DWORD PTR DS:[EDI+0x4]
$+3F     0069F133        897D DC                      MOV DWORD PTR SS:[EBP-0x24], EDI
$+42     0069F136        894D D8                      MOV DWORD PTR SS:[EBP-0x28], ECX
$+45     0069F139        85C9                         TEST ECX, ECX
$+47     0069F13B        C745 FC 06000000             MOV DWORD PTR SS:[EBP-0x4], 0x6
$+4E     0069F142        0F94C0                       SETE AL
$+51     0069F145        84C0                         TEST AL, AL
$+53     0069F147        74 09                        JE SHORT PathOfEx.0069F152
$+55     0069F149        C745 FC 07000000             MOV DWORD PTR SS:[EBP-0x4], 0x7
$+5C     0069F150        EB 1D                        JMP SHORT PathOfEx.0069F16F
$+5E     0069F152        FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
$+61     0069F155        8B01                         MOV EAX, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$+63     0069F157        FF75 0C                      PUSH DWORD PTR SS:[EBP+0xC]
$+66     0069F15A        FF75 08                      PUSH DWORD PTR SS:[EBP+0x8]                        ; PathOfEx.004E47CE
$+69     0069F15D        FF10                         CALL NEAR DWORD PTR DS:[EAX]                       ; ecx==PreGameState
$+6B     0069F15F        8B45 E0                      MOV EAX, DWORD PTR SS:[EBP-0x20]                   ; PathOfEx.004E47CE
$+6E     0069F162        8078 04 00                   CMP BYTE PTR DS:[EAX+0x4], 0x0
$+72     0069F166        75 3A                        JNZ SHORT PathOfEx.0069F1A2
$+74     0069F168        C745 FC 09000000             MOV DWORD PTR SS:[EBP-0x4], 0x9
$+7B     0069F16F        85FF                         TEST EDI, EDI
$+7D     0069F171        74 1F                        JE SHORT PathOfEx.0069F192
$+7F     0069F173        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$+81     0069F175        F0:0FC147 04                 LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
$+86     0069F17A        75 16                        JNZ SHORT PathOfEx.0069F192
$+88     0069F17C        8B07                         MOV EAX, DWORD PTR DS:[EDI]
$+8A     0069F17E        8BCF                         MOV ECX, EDI
$+8C     0069F180        FF10                         CALL NEAR DWORD PTR DS:[EAX]
$+8E     0069F182        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$+90     0069F184        F0:0FC147 08                 LOCK XADD DWORD PTR DS:[EDI+0x8], EAX
$+95     0069F189        75 07                        JNZ SHORT PathOfEx.0069F192
$+97     0069F18B        8B07                         MOV EAX, DWORD PTR DS:[EDI]
$+99     0069F18D        8BCF                         MOV ECX, EDI
$+9B     0069F18F        FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
$+9E     0069F192        8B4D EC                      MOV ECX, DWORD PTR SS:[EBP-0x14]                   ; PathOfEx.004E47CE
$+A1     0069F195        C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$+A8     0069F19C        3B31                         CMP ESI, DWORD PTR DS:[ECX]                        ; PathOfEx.00E672E0
$+AA     0069F19E      ^ 75 80                        JNZ SHORT PathOfEx.0069F120
$+AC     0069F1A0        EB 33                        JMP SHORT PathOfEx.0069F1D5
$+AE     0069F1A2        C745 FC 08000000             MOV DWORD PTR SS:[EBP-0x4], 0x8
$+B5     0069F1A9        85FF                         TEST EDI, EDI
$+B7     0069F1AB        74 1E                        JE SHORT PathOfEx.0069F1CB
$+B9     0069F1AD        8BC3                         MOV EAX, EBX                                       ; PathOfEx.00FF0140
$+BB     0069F1AF        F0:0FC147 04                 LOCK XADD DWORD PTR DS:[EDI+0x4], EAX
$+C0     0069F1B4        75 15                        JNZ SHORT PathOfEx.0069F1CB
$+C2     0069F1B6        8B07                         MOV EAX, DWORD PTR DS:[EDI]
$+C4     0069F1B8        8BCF                         MOV ECX, EDI
$+C6     0069F1BA        FF10                         CALL NEAR DWORD PTR DS:[EAX]
$+C8     0069F1BC        F0:0FC15F 08                 LOCK XADD DWORD PTR DS:[EDI+0x8], EBX
$+CD     0069F1C1        4B                           DEC EBX                                            ; PathOfEx.00FF0140
$+CE     0069F1C2        75 07                        JNZ SHORT PathOfEx.0069F1CB
$+D0     0069F1C4        8B07                         MOV EAX, DWORD PTR DS:[EDI]
$+D2     0069F1C6        8BCF                         MOV ECX, EDI
$+D4     0069F1C8        FF50 04                      CALL NEAR DWORD PTR DS:[EAX+0x4]
$+D7     0069F1CB        8B4D EC                      MOV ECX, DWORD PTR SS:[EBP-0x14]                   ; PathOfEx.004E47CE
$+DA     0069F1CE        C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$+E1     0069F1D5        8B7D F0                      MOV EDI, DWORD PTR SS:[EBP-0x10]
$+E4     0069F1D8        8347 1C FF                   ADD DWORD PTR DS:[EDI+0x1C], -0x1
$+E8     0069F1DC        75 31                        JNZ SHORT PathOfEx.0069F20F
$+EA     0069F1DE        807F 20 00                   CMP BYTE PTR DS:[EDI+0x20], 0x0
$+EE     0069F1E2        74 2B                        JE SHORT PathOfEx.0069F20F
$+F0     0069F1E4        8B47 08                      MOV EAX, DWORD PTR DS:[EDI+0x8]
$+F3     0069F1E7        2B47 04                      SUB EAX, DWORD PTR DS:[EDI+0x4]
$+F6     0069F1EA        C1F8 03                      SAR EAX, 0x3
$+F9     0069F1ED        50                           PUSH EAX
$+FA     0069F1EE        E8 ED0C0000                  CALL PathOfEx.0069FEE0
$+FF     0069F1F3        8B57 08                      MOV EDX, DWORD PTR DS:[EDI+0x8]
$+102    0069F1F6        8B4F 04                      MOV ECX, DWORD PTR DS:[EDI+0x4]
$+105    0069F1F9        C645 10 00                   MOV BYTE PTR SS:[EBP+0x10], 0x0
$+109    0069F1FD        FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
$+10C    0069F200        FF77 10                      PUSH DWORD PTR DS:[EDI+0x10]
$+10F    0069F203        E8 D81A0000                  CALL PathOfEx.006A0CE0
$+114    0069F208        83C4 08                      ADD ESP, 0x8
$+117    0069F20B        C647 20 00                   MOV BYTE PTR DS:[EDI+0x20], 0x0
$+11B    0069F20F        8B4D F4                      MOV ECX, DWORD PTR SS:[EBP-0xC]
$+11E    0069F212        5F                           POP EDI                                            ; PathOfEx.004E47CE
$+11F    0069F213        5E                           POP ESI                                            ; PathOfEx.004E47CE
$+120    0069F214        64:890D 00000000             MOV DWORD PTR FS:[0], ECX
$+127    0069F21B        5B                           POP EBX                                            ; PathOfEx.004E47CE
$+128    0069F21C        8BE5                         MOV ESP, EBP
$+12A    0069F21E        5D                           POP EBP                                            ; PathOfEx.004E47CE
$+12B    0069F21F        C2 0C00                      RETN 0xC
*/
/**/
//点击创建角色的时候会断住，这是个构建发包的函数。里面有关于htonl的调用。
/*
$-AC     008DABF0      83EC 08                    SUB ESP, 0x8
$-A9     008DABF3      53                         PUSH EBX
$-A8     008DABF4      55                         PUSH EBP
$-A7     008DABF5      8B2D 4455D800              MOV EBP, DWORD PTR DS:[0xD85544]
$-A1     008DABFB      8BD9                       MOV EBX, ECX
$-9F     008DABFD      56                         PUSH ESI
$-9E     008DABFE      57                         PUSH EDI
$-9D     008DABFF      0FB743 04                  MOVZX EAX, WORD PTR DS:[EBX+0x4]
$-99     008DAC03      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-98     008DAC04      FFD5                       CALL NEAR EBP
$-96     008DAC06      8B7C24 1C                  MOV EDI, DWORD PTR SS:[ESP+0x1C]
$-92     008DAC0A      8BCF                       MOV ECX, EDI
$-90     008DAC0C      0FB7C0                     MOVZX EAX, AX
$-8D     008DAC0F      894424 14                  MOV DWORD PTR SS:[ESP+0x14], EAX            ; PathOfEx.00E11800
$-89     008DAC13      8D4424 14                  LEA EAX, DWORD PTR SS:[ESP+0x14]
$-85     008DAC17      6A 02                      PUSH 0x2
$-83     008DAC19      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-82     008DAC1A      E8 012CEBFF                CALL PathOfEx.0078D820
$-7D     008DAC1F      0FB743 18                  MOVZX EAX, WORD PTR DS:[EBX+0x18]
$-79     008DAC23      8D73 08                    LEA ESI, DWORD PTR DS:[EBX+0x8]
$-76     008DAC26      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-75     008DAC27      894424 18                  MOV DWORD PTR SS:[ESP+0x18], EAX            ; PathOfEx.00E11800
$-71     008DAC2B      FFD5                       CALL NEAR EBP
$-6F     008DAC2D      0FB7C0                     MOVZX EAX, AX
$-6C     008DAC30      8BCF                       MOV ECX, EDI
$-6A     008DAC32      894424 1C                  MOV DWORD PTR SS:[ESP+0x1C], EAX            ; PathOfEx.00E11800
$-66     008DAC36      8D4424 1C                  LEA EAX, DWORD PTR SS:[ESP+0x1C]
$-62     008DAC3A      6A 02                      PUSH 0x2
$-60     008DAC3C      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-5F     008DAC3D      E8 DE2BEBFF                CALL PathOfEx.0078D820
$-5A     008DAC42      837E 14 08                 CMP DWORD PTR DS:[ESI+0x14], 0x8
$-56     008DAC46      72 02                      JB SHORT PathOfEx.008DAC4A
$-54     008DAC48      8B36                       MOV ESI, DWORD PTR DS:[ESI]
$-52     008DAC4A      8B4424 14                  MOV EAX, DWORD PTR SS:[ESP+0x14]
$-4E     008DAC4E      8BCF                       MOV ECX, EDI
$-4C     008DAC50      0FB7C0                     MOVZX EAX, AX
$-49     008DAC53      03C0                       ADD EAX, EAX                                ; PathOfEx.00E11800
$-47     008DAC55      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-46     008DAC56      56                         PUSH ESI
$-45     008DAC57      E8 C42BEBFF                CALL PathOfEx.0078D820
$-40     008DAC5C      0FB743 30                  MOVZX EAX, WORD PTR DS:[EBX+0x30]
$-3C     008DAC60      8D73 20                    LEA ESI, DWORD PTR DS:[EBX+0x20]
$-39     008DAC63      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-38     008DAC64      894424 18                  MOV DWORD PTR SS:[ESP+0x18], EAX            ; PathOfEx.00E11800
$-34     008DAC68      FFD5                       CALL NEAR EBP
$-32     008DAC6A      0FB7C0                     MOVZX EAX, AX
$-2F     008DAC6D      8BCF                       MOV ECX, EDI
$-2D     008DAC6F      894424 1C                  MOV DWORD PTR SS:[ESP+0x1C], EAX            ; PathOfEx.00E11800
$-29     008DAC73      8D4424 1C                  LEA EAX, DWORD PTR SS:[ESP+0x1C]
$-25     008DAC77      6A 02                      PUSH 0x2
$-23     008DAC79      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-22     008DAC7A      E8 A12BEBFF                CALL PathOfEx.0078D820
$-1D     008DAC7F      837E 14 08                 CMP DWORD PTR DS:[ESI+0x14], 0x8
$-19     008DAC83      72 02                      JB SHORT PathOfEx.008DAC87
$-17     008DAC85      8B36                       MOV ESI, DWORD PTR DS:[ESI]
$-15     008DAC87      8B4424 14                  MOV EAX, DWORD PTR SS:[ESP+0x14]
$-11     008DAC8B      8BCF                       MOV ECX, EDI
$-F      008DAC8D      0FB7C0                     MOVZX EAX, AX
$-C      008DAC90      03C0                       ADD EAX, EAX                                ; PathOfEx.00E11800
$-A      008DAC92      50                         PUSH EAX                                    ; PathOfEx.00E11800
$-9      008DAC93      56                         PUSH ESI
$-8      008DAC94      E8 872BEBFF                CALL PathOfEx.0078D820
$-3      008DAC99      FF73 38                    PUSH DWORD PTR DS:[EBX+0x38]
$ ==>    008DAC9C      8B35 1055D800              MOV ESI, DWORD PTR DS:[0xD85510]            ; 存放的这个函数可能是用来加密或解密的。这个函数很有可能是htonl
$+6      008DACA2      FFD6                       CALL NEAR ESI                               ; push一个dword，返回一个dword
$+8      008DACA4      894424 1C                  MOV DWORD PTR SS:[ESP+0x1C], EAX            ; PathOfEx.00E11800
$+C      008DACA8      8BCF                       MOV ECX, EDI
$+E      008DACAA      6A 04                      PUSH 0x4
$+10     008DACAC      8D4424 20                  LEA EAX, DWORD PTR SS:[ESP+0x20]
$+14     008DACB0      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+15     008DACB1      E8 6A2BEBFF                CALL PathOfEx.0078D820
$+1A     008DACB6      FF73 3C                    PUSH DWORD PTR DS:[EBX+0x3C]
$+1D     008DACB9      FFD6                       CALL NEAR ESI
$+1F     008DACBB      894424 1C                  MOV DWORD PTR SS:[ESP+0x1C], EAX            ; PathOfEx.00E11800
$+23     008DACBF      8BCF                       MOV ECX, EDI
$+25     008DACC1      6A 04                      PUSH 0x4
$+27     008DACC3      8D4424 20                  LEA EAX, DWORD PTR SS:[ESP+0x20]
$+2B     008DACC7      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+2C     008DACC8      E8 532BEBFF                CALL PathOfEx.0078D820
$+31     008DACCD      0FB743 70                  MOVZX EAX, WORD PTR DS:[EBX+0x70]
$+35     008DACD1      8D73 60                    LEA ESI, DWORD PTR DS:[EBX+0x60]
$+38     008DACD4      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+39     008DACD5      894424 18                  MOV DWORD PTR SS:[ESP+0x18], EAX            ; PathOfEx.00E11800
$+3D     008DACD9      FFD5                       CALL NEAR EBP
$+3F     008DACDB      0FB7C0                     MOVZX EAX, AX
$+42     008DACDE      8BCF                       MOV ECX, EDI
$+44     008DACE0      894424 1C                  MOV DWORD PTR SS:[ESP+0x1C], EAX            ; PathOfEx.00E11800
$+48     008DACE4      8D4424 1C                  LEA EAX, DWORD PTR SS:[ESP+0x1C]
$+4C     008DACE8      6A 02                      PUSH 0x2
$+4E     008DACEA      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+4F     008DACEB      E8 302BEBFF                CALL PathOfEx.0078D820
$+54     008DACF0      837E 14 08                 CMP DWORD PTR DS:[ESI+0x14], 0x8
$+58     008DACF4      72 02                      JB SHORT PathOfEx.008DACF8
$+5A     008DACF6      8B36                       MOV ESI, DWORD PTR DS:[ESI]
$+5C     008DACF8      8B4424 14                  MOV EAX, DWORD PTR SS:[ESP+0x14]
$+60     008DACFC      8BCF                       MOV ECX, EDI
$+62     008DACFE      0FB7C0                     MOVZX EAX, AX
$+65     008DAD01      03C0                       ADD EAX, EAX                                ; PathOfEx.00E11800
$+67     008DAD03      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+68     008DAD04      56                         PUSH ESI
$+69     008DAD05      E8 162BEBFF                CALL PathOfEx.0078D820
$+6E     008DAD0A      6A 20                      PUSH 0x20
$+70     008DAD0C      8D43 40                    LEA EAX, DWORD PTR DS:[EBX+0x40]
$+73     008DAD0F      8BCF                       MOV ECX, EDI
$+75     008DAD11      50                         PUSH EAX                                    ; PathOfEx.00E11800
$+76     008DAD12      E8 092BEBFF                CALL PathOfEx.0078D820
$+7B     008DAD17      5F                         POP EDI                                     ; 0018E170
$+7C     008DAD18      5E                         POP ESI                                     ; 0018E170
$+7D     008DAD19      5D                         POP EBP                                     ; 0018E170
$+7E     008DAD1A      5B                         POP EBX                                     ; 0018E170
$+7F     008DAD1B      83C4 08                    ADD ESP, 0x8
$+82     008DAD1E      C2 0400                    RETN 0x4
*/
/**/
//tag_游戏内得到发包对象holder_13c

//走路发送的第一个包
/*
$-95     006C9C80     55                        PUSH EBP
$-94     006C9C81     8BEC                      MOV EBP, ESP
$-92     006C9C83     6A FF                     PUSH -0x1
$-90     006C9C85     68 20C0D200               PUSH PathOfEx.00D2C020                   ; ASCII "肛仵"
$-8B     006C9C8A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
$-85     006C9C90     50                        PUSH EAX
$-84     006C9C91     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
$-7D     006C9C98     83EC 30                   SUB ESP, 0x30
$-7A     006C9C9B     56                        PUSH ESI
$-79     006C9C9C     57                        PUSH EDI
$-78     006C9C9D     8BF1                      MOV ESI, ECX
$-76     006C9C9F     E8 1C4AFFFF               CALL PathOfEx.006BE6C0
$-71     006C9CA4     8B8E 64150000             MOV ECX, DWORD PTR DS:[ESI+0x1564]
$-6B     006C9CAA     8DBE 64150000             LEA EDI, DWORD PTR DS:[ESI+0x1564]
$-65     006C9CB0     3B4F 04                   CMP ECX, DWORD PTR DS:[EDI+0x4]
$-62     006C9CB3     8845 F2                   MOV BYTE PTR SS:[EBP-0xE], AL
$-5F     006C9CB6     0F94C1                    SETE CL
$-5C     006C9CB9     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14], 0x0
$-55     006C9CC0     84C9                      TEST CL, CL
$-53     006C9CC2     0F85 62010000             JNZ PathOfEx.006C9E2A
$-4D     006C9CC8     8B47 04                   MOV EAX, DWORD PTR DS:[EDI+0x4]
$-4A     006C9CCB     8D4D D4                   LEA ECX, DWORD PTR SS:[EBP-0x2C]
$-47     006C9CCE     53                        PUSH EBX
$-46     006C9CCF     51                        PUSH ECX
$-45     006C9CD0     8D48 F8                   LEA ECX, DWORD PTR DS:[EAX-0x8]
$-42     006C9CD3     E8 98BEEAFF               CALL PathOfEx.00575B70
$-3D     006C9CD8     8D45 EC                   LEA EAX, DWORD PTR SS:[EBP-0x14]
$-3A     006C9CDB     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
$-33     006C9CE2     50                        PUSH EAX
$-32     006C9CE3     56                        PUSH ESI
$-31     006C9CE4     FF77 04                   PUSH DWORD PTR DS:[EDI+0x4]
$-2E     006C9CE7     8D4D E8                   LEA ECX, DWORD PTR SS:[EBP-0x18]
$-2B     006C9CEA     FF37                      PUSH DWORD PTR DS:[EDI]
$-29     006C9CEC     E8 1F350000               CALL PathOfEx.006CD210
$-24     006C9CF1     83C4 10                   ADD ESP, 0x10
$-21     006C9CF4     8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-0x1C]
$-1E     006C9CF7     8BCF                      MOV ECX, EDI
$-1C     006C9CF9     FF77 04                   PUSH DWORD PTR DS:[EDI+0x4]
$-19     006C9CFC     FF75 E8                   PUSH DWORD PTR SS:[EBP-0x18]
$-16     006C9CFF     50                        PUSH EAX
$-15     006C9D00     E8 CB180000               CALL PathOfEx.006CB5D0
$-10     006C9D05     8B07                      MOV EAX, DWORD PTR DS:[EDI]
$-E      006C9D07     3B47 04                   CMP EAX, DWORD PTR DS:[EDI+0x4]
$-B      006C9D0A     0F94C0                    SETE AL
$-8      006C9D0D     84C0                      TEST AL, AL
$-6      006C9D0F     0F84 80000000             JE PathOfEx.006C9D95
$ ==>    006C9D15     8B8E 3C010000             MOV ECX, DWORD PTR DS:[ESI+0x13C]        ; 得到游戏内发包对象holder的地址
$+6      006C9D1B     E8 607D2000               CALL PathOfEx.008D1A80                   ; 走路发的第一个包，会断住。此函数内进行发包的
$+B      006C9D20     8D45 E8                   LEA EAX, DWORD PTR SS:[EBP-0x18]
$+E      006C9D23     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18], 0x0
$+15     006C9D2A     50                        PUSH EAX
$+16     006C9D2B     8D45 DC                   LEA EAX, DWORD PTR SS:[EBP-0x24]
$+19     006C9D2E     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C], 0x0
$+20     006C9D35     50                        PUSH EAX
$+21     006C9D36     8D55 E4                   LEA EDX, DWORD PTR SS:[EBP-0x1C]
$+24     006C9D39     C745 DC 00000000          MOV DWORD PTR SS:[EBP-0x24], 0x0
$+2B     006C9D40     8D4D C4                   LEA ECX, DWORD PTR SS:[EBP-0x3C]
$+2E     006C9D43     C745 E0 00000000          MOV DWORD PTR SS:[EBP-0x20], 0x0
$+35     006C9D4A     E8 41290000               CALL PathOfEx.006CC690
$+3A     006C9D4F     83C4 08                   ADD ESP, 0x8
$+3D     006C9D52     8D8E 70150000             LEA ECX, DWORD PTR DS:[ESI+0x1570]
$+43     006C9D58     50                        PUSH EAX
$+44     006C9D59     E8 422C0000               CALL PathOfEx.006CC9A0
$+49     006C9D5E     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4], 0x1
$+4D     006C9D62     83CF FF                   OR EDI, -0x1
$+50     006C9D65     8B4D CC                   MOV ECX, DWORD PTR SS:[EBP-0x34]
$+53     006C9D68     85C9                      TEST ECX, ECX
$+55     006C9D6A     74 0E                     JE SHORT PathOfEx.006C9D7A
$+57     006C9D6C     8BC7                      MOV EAX, EDI
$+59     006C9D6E     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$+5E     006C9D73     75 05                     JNZ SHORT PathOfEx.006C9D7A
$+60     006C9D75     8B01                      MOV EAX, DWORD PTR DS:[ECX]              ; PathOfEx.00EF08E4
$+62     006C9D77     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+65     006C9D7A     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4], 0x2
$+69     006C9D7E     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
$+6C     006C9D81     85C9                      TEST ECX, ECX
$+6E     006C9D83     74 6B                     JE SHORT PathOfEx.006C9DF0
$+70     006C9D85     8BC7                      MOV EAX, EDI
$+72     006C9D87     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$+77     006C9D8C     75 62                     JNZ SHORT PathOfEx.006C9DF0
$+79     006C9D8E     8B01                      MOV EAX, DWORD PTR DS:[ECX]              ; PathOfEx.00EF08E4
$+7B     006C9D90     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+7E     006C9D93     EB 5B                     JMP SHORT PathOfEx.006C9DF0
$+80     006C9D95     8B47 04                   MOV EAX, DWORD PTR DS:[EDI+0x4]
$+83     006C9D98     8D4D DC                   LEA ECX, DWORD PTR SS:[EBP-0x24]
$+86     006C9D9B     51                        PUSH ECX
$+87     006C9D9C     8D48 F8                   LEA ECX, DWORD PTR DS:[EAX-0x8]
$+8A     006C9D9F     E8 CCBDEAFF               CALL PathOfEx.00575B70
$+8F     006C9DA4     8B00                      MOV EAX, DWORD PTR DS:[EAX]              ; PathOfEx.00EA23C0
$+91     006C9DA6     3B45 D4                   CMP EAX, DWORD PTR SS:[EBP-0x2C]
$+94     006C9DA9     0F94C0                    SETE AL
$+97     006C9DAC     84C0                      TEST AL, AL
$+99     006C9DAE     0F9445 F3                 SETE BYTE PTR SS:[EBP-0xD]
$+9D     006C9DB2     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4], 0x3
$+A1     006C9DB6     83CF FF                   OR EDI, -0x1
$+A4     006C9DB9     8B5D E0                   MOV EBX, DWORD PTR SS:[EBP-0x20]
$+A7     006C9DBC     85DB                      TEST EBX, EBX
$+A9     006C9DBE     74 1F                     JE SHORT PathOfEx.006C9DDF
$+AB     006C9DC0     8BC7                      MOV EAX, EDI
$+AD     006C9DC2     F0:0FC143 04              LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
$+B2     006C9DC7     75 16                     JNZ SHORT PathOfEx.006C9DDF
$+B4     006C9DC9     8B03                      MOV EAX, DWORD PTR DS:[EBX]
$+B6     006C9DCB     8BCB                      MOV ECX, EBX
$+B8     006C9DCD     FF10                      CALL NEAR DWORD PTR DS:[EAX]             ; PathOfEx.00EA23C0
$+BA     006C9DCF     8BC7                      MOV EAX, EDI
$+BC     006C9DD1     F0:0FC143 08              LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
$+C1     006C9DD6     75 07                     JNZ SHORT PathOfEx.006C9DDF
$+C3     006C9DD8     8B03                      MOV EAX, DWORD PTR DS:[EBX]
$+C5     006C9DDA     8BCB                      MOV ECX, EBX
$+C7     006C9DDC     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+CA     006C9DDF     807D F3 00                CMP BYTE PTR SS:[EBP-0xD], 0x0
$+CE     006C9DE3     C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
$+D2     006C9DE7     74 07                     JE SHORT PathOfEx.006C9DF0
$+D4     006C9DE9     8BCE                      MOV ECX, ESI
$+D6     006C9DEB     E8 90F5FFFF               CALL PathOfEx.006C9380
$+DB     006C9DF0     C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4], 0x4
$+E2     006C9DF7     8B5D D8                   MOV EBX, DWORD PTR SS:[EBP-0x28]
$+E5     006C9DFA     85DB                      TEST EBX, EBX
$+E7     006C9DFC     74 1E                     JE SHORT PathOfEx.006C9E1C
$+E9     006C9DFE     8BC7                      MOV EAX, EDI
$+EB     006C9E00     F0:0FC143 04              LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
$+F0     006C9E05     75 15                     JNZ SHORT PathOfEx.006C9E1C
$+F2     006C9E07     8B03                      MOV EAX, DWORD PTR DS:[EBX]
$+F4     006C9E09     8BCB                      MOV ECX, EBX
$+F6     006C9E0B     FF10                      CALL NEAR DWORD PTR DS:[EAX]             ; PathOfEx.00EA23C0
$+F8     006C9E0D     F0:0FC17B 08              LOCK XADD DWORD PTR DS:[EBX+0x8], EDI
$+FD     006C9E12     4F                        DEC EDI
$+FE     006C9E13     75 07                     JNZ SHORT PathOfEx.006C9E1C
$+100    006C9E15     8B03                      MOV EAX, DWORD PTR DS:[EBX]
$+102    006C9E17     8BCB                      MOV ECX, EBX
$+104    006C9E19     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+107    006C9E1C     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+10E    006C9E23     837D EC 00                CMP DWORD PTR SS:[EBP-0x14], 0x0
$+112    006C9E27     5B                        POP EBX
$+113    006C9E28     75 36                     JNZ SHORT PathOfEx.006C9E60
$+115    006C9E2A     8B8E D8180000             MOV ECX, DWORD PTR DS:[ESI+0x18D8]
$+11B    006C9E30     8B86 040C0000             MOV EAX, DWORD PTR DS:[ESI+0xC04]
$+121    006C9E36     3B81 400A0000             CMP EAX, DWORD PTR DS:[ECX+0xA40]
$+127    006C9E3C     75 22                     JNZ SHORT PathOfEx.006C9E60
$+129    006C9E3E     FF15 6054D800             CALL NEAR DWORD PTR DS:[0xD85460]
$+12F    006C9E44     8B8E 040C0000             MOV ECX, DWORD PTR DS:[ESI+0xC04]
$+135    006C9E4A     85C9                      TEST ECX, ECX
$+137    006C9E4C     74 08                     JE SHORT PathOfEx.006C9E56
$+139    006C9E4E     8B01                      MOV EAX, DWORD PTR DS:[ECX]              ; PathOfEx.00EF08E4
$+13B    006C9E50     FF90 04010000             CALL NEAR DWORD PTR DS:[EAX+0x104]
$+141    006C9E56     C786 040C0000 00000000    MOV DWORD PTR DS:[ESI+0xC04], 0x0
$+14B    006C9E60     8BCE                      MOV ECX, ESI
$+14D    006C9E62     E8 5948FFFF               CALL PathOfEx.006BE6C0
$+152    006C9E67     807D F2 00                CMP BYTE PTR SS:[EBP-0xE], 0x0
$+156    006C9E6B     74 39                     JE SHORT PathOfEx.006C9EA6
$+158    006C9E6D     84C0                      TEST AL, AL
$+15A    006C9E6F     75 35                     JNZ SHORT PathOfEx.006C9EA6
$+15C    006C9E71     66:0F6E86 340C0000        MOVD MM0, DWORD PTR DS:[ESI+0xC34]
$+164    006C9E79     8D45 D4                   LEA EAX, DWORD PTR SS:[EBP-0x2C]
$+167    006C9E7C     8B8E D8180000             MOV ECX, DWORD PTR DS:[ESI+0x18D8]
$+16D    006C9E82     0F5BC0                    CVTDQ2PS XMM0, XMM0
$+170    006C9E85     50                        PUSH EAX
$+171    006C9E86     F3:0F1145 D4              MOVSS DWORD PTR SS:[EBP-0x2C], XMM0
$+176    006C9E8B     66:0F6E86 380C0000        MOVD MM0, DWORD PTR DS:[ESI+0xC38]
$+17E    006C9E93     0F5BC0                    CVTDQ2PS XMM0, XMM0
$+181    006C9E96     F3:0F1145 D8              MOVSS DWORD PTR SS:[EBP-0x28], XMM0
$+186    006C9E9B     8B89 380A0000             MOV ECX, DWORD PTR DS:[ECX+0xA38]
$+18C    006C9EA1     E8 3A07F9FF               CALL PathOfEx.0065A5E0
$+191    006C9EA6     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+194    006C9EA9     5F                        POP EDI
$+195    006C9EAA     5E                        POP ESI
$+196    006C9EAB     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+19D    006C9EB2     8BE5                      MOV ESP, EBP
$+19F    006C9EB4     5D                        POP EBP
$+1A0    006C9EB5     C3                        RETN
*/
/**/
//鼠标消息处理函数
/*
$ ==>    00A10C70       83EC 0C                      SUB ESP, 0xC                             ; 消息处理函数，鼠标按下、弹起、移动时会断住
$+3      00A10C73       53                           PUSH EBX
$+4      00A10C74       56                           PUSH ESI
$+5      00A10C75       57                           PUSH EDI
$+6      00A10C76       8BF9                         MOV EDI, ECX
$+8      00A10C78       C687 E00A0000 00             MOV BYTE PTR DS:[EDI+0xAE0], 0x0
$+F      00A10C7F       8D9F E00A0000                LEA EBX, DWORD PTR DS:[EDI+0xAE0]
$+15     00A10C85       C787 F40A0000 20000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x20
$+1F     00A10C8F       66:0F6E87 D80A0000           MOVD MM0, DWORD PTR DS:[EDI+0xAD8]
$+27     00A10C97       0F5BC0                       CVTDQ2PS XMM0, XMM0
$+2A     00A10C9A       F3:0F114424 10               MOVSS DWORD PTR SS:[ESP+0x10], XMM0
$+30     00A10CA0       66:0F6E87 DC0A0000           MOVD MM0, DWORD PTR DS:[EDI+0xADC]
$+38     00A10CA8       8B4424 10                    MOV EAX, DWORD PTR SS:[ESP+0x10]
$+3C     00A10CAC       0F5BC0                       CVTDQ2PS XMM0, XMM0
$+3F     00A10CAF       8987 EC0A0000                MOV DWORD PTR DS:[EDI+0xAEC], EAX
$+45     00A10CB5       F3:0F114424 14               MOVSS DWORD PTR SS:[ESP+0x14], XMM0
$+4B     00A10CBB       8B4424 14                    MOV EAX, DWORD PTR SS:[ESP+0x14]
$+4F     00A10CBF       8987 F00A0000                MOV DWORD PTR DS:[EDI+0xAF0], EAX
$+55     00A10CC5       A1 54CFFD00                  MOV EAX, DWORD PTR DS:[0xFDCF54]
$+5A     00A10CCA       8987 E40A0000                MOV DWORD PTR DS:[EDI+0xAE4], EAX
$+60     00A10CD0       A1 58CFFD00                  MOV EAX, DWORD PTR DS:[0xFDCF58]
$+65     00A10CD5       8987 E80A0000                MOV DWORD PTR DS:[EDI+0xAE8], EAX
$+6B     00A10CDB       8B4424 20                    MOV EAX, DWORD PTR SS:[ESP+0x20]
$+6F     00A10CDF       C787 F80A0000 00000000       MOV DWORD PTR DS:[EDI+0xAF8], 0x0
$+79     00A10CE9       C787 FC0A0000 00000000       MOV DWORD PTR DS:[EDI+0xAFC], 0x0
$+83     00A10CF3       3D 00020000                  CMP EAX, 0x200
$+88     00A10CF8       75 1C                        JNZ SHORT PathOfEx.00A10D16
$+8A     00A10CFA       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+8E     00A10CFE       53                           PUSH EBX
$+8F     00A10CFF       57                           PUSH EDI
$+90     00A10D00       8D89 80010000                LEA ECX, DWORD PTR DS:[ECX+0x180]
$+96     00A10D06       E8 D561A7FF                  CALL PathOfEx.00486EE0
$+9B     00A10D0B       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+9D     00A10D0D       5F                           POP EDI
$+9E     00A10D0E       5E                           POP ESI
$+9F     00A10D0F       5B                           POP EBX
$+A0     00A10D10       83C4 0C                      ADD ESP, 0xC
$+A3     00A10D13       C2 0C00                      RETN 0xC
$+A6     00A10D16       3D 01020000                  CMP EAX, 0x201
$+AB     00A10D1B       75 26                        JNZ SHORT PathOfEx.00A10D43
$+AD     00A10D1D       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+B1     00A10D21       53                           PUSH EBX
$+B2     00A10D22       57                           PUSH EDI
$+B3     00A10D23       C787 F40A0000 01000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x1
$+BD     00A10D2D       8D89 80000000                LEA ECX, DWORD PTR DS:[ECX+0x80]
$+C3     00A10D33       E8 A861A7FF                  CALL PathOfEx.00486EE0                   ; 鼠标按下的时候会断住，按下断住一次，弹起断住一次,0x80
$+C8     00A10D38       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+CA     00A10D3A       5F                           POP EDI
$+CB     00A10D3B       5E                           POP ESI
$+CC     00A10D3C       5B                           POP EBX
$+CD     00A10D3D       83C4 0C                      ADD ESP, 0xC
$+D0     00A10D40       C2 0C00                      RETN 0xC
$+D3     00A10D43       3D 02020000                  CMP EAX, 0x202
$+D8     00A10D48       75 26                        JNZ SHORT PathOfEx.00A10D70
$+DA     00A10D4A       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+DE     00A10D4E       53                           PUSH EBX
$+DF     00A10D4F       57                           PUSH EDI
$+E0     00A10D50       C787 F40A0000 01000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x1
$+EA     00A10D5A       8D89 00010000                LEA ECX, DWORD PTR DS:[ECX+0x100]
$+F0     00A10D60       E8 7B61A7FF                  CALL PathOfEx.00486EE0
$+F5     00A10D65       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+F7     00A10D67       5F                           POP EDI
$+F8     00A10D68       5E                           POP ESI
$+F9     00A10D69       5B                           POP EBX
$+FA     00A10D6A       83C4 0C                      ADD ESP, 0xC
$+FD     00A10D6D       C2 0C00                      RETN 0xC
$+100    00A10D70       3D 03020000                  CMP EAX, 0x203
$+105    00A10D75       75 2F                        JNZ SHORT PathOfEx.00A10DA6
$+107    00A10D77       C787 F40A0000 01000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x1
$+111    00A10D81       8B7424 1C                    MOV ESI, DWORD PTR SS:[ESP+0x1C]
$+115    00A10D85       8BCE                         MOV ECX, ESI
$+117    00A10D87       53                           PUSH EBX
$+118    00A10D88       57                           PUSH EDI
$+119    00A10D89       E8 5261A7FF                  CALL PathOfEx.00486EE0
$+11E    00A10D8E       53                           PUSH EBX
$+11F    00A10D8F       8D8E 80000000                LEA ECX, DWORD PTR DS:[ESI+0x80]
$+125    00A10D95       57                           PUSH EDI
$+126    00A10D96       E8 4561A7FF                  CALL PathOfEx.00486EE0
$+12B    00A10D9B       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+12D    00A10D9D       5F                           POP EDI
$+12E    00A10D9E       5E                           POP ESI
$+12F    00A10D9F       5B                           POP EBX
$+130    00A10DA0       83C4 0C                      ADD ESP, 0xC
$+133    00A10DA3       C2 0C00                      RETN 0xC
$+136    00A10DA6       3D 07020000                  CMP EAX, 0x207
$+13B    00A10DAB       75 26                        JNZ SHORT PathOfEx.00A10DD3
$+13D    00A10DAD       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+141    00A10DB1       53                           PUSH EBX
$+142    00A10DB2       57                           PUSH EDI
$+143    00A10DB3       C787 F40A0000 04000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x4
$+14D    00A10DBD       8D89 80000000                LEA ECX, DWORD PTR DS:[ECX+0x80]
$+153    00A10DC3       E8 1861A7FF                  CALL PathOfEx.00486EE0
$+158    00A10DC8       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+15A    00A10DCA       5F                           POP EDI
$+15B    00A10DCB       5E                           POP ESI
$+15C    00A10DCC       5B                           POP EBX
$+15D    00A10DCD       83C4 0C                      ADD ESP, 0xC
$+160    00A10DD0       C2 0C00                      RETN 0xC
$+163    00A10DD3       3D 08020000                  CMP EAX, 0x208
$+168    00A10DD8       75 26                        JNZ SHORT PathOfEx.00A10E00
$+16A    00A10DDA       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+16E    00A10DDE       53                           PUSH EBX
$+16F    00A10DDF       57                           PUSH EDI
$+170    00A10DE0       C787 F40A0000 04000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x4
$+17A    00A10DEA       8D89 00010000                LEA ECX, DWORD PTR DS:[ECX+0x100]
$+180    00A10DF0       E8 EB60A7FF                  CALL PathOfEx.00486EE0
$+185    00A10DF5       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+187    00A10DF7       5F                           POP EDI
$+188    00A10DF8       5E                           POP ESI
$+189    00A10DF9       5B                           POP EBX
$+18A    00A10DFA       83C4 0C                      ADD ESP, 0xC
$+18D    00A10DFD       C2 0C00                      RETN 0xC
$+190    00A10E00       3D 09020000                  CMP EAX, 0x209
$+195    00A10E05       75 0F                        JNZ SHORT PathOfEx.00A10E16
$+197    00A10E07       C787 F40A0000 04000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x4
$+1A1    00A10E11     ^ E9 6BFFFFFF                  JMP PathOfEx.00A10D81
$+1A6    00A10E16       3D 04020000                  CMP EAX, 0x204
$+1AB    00A10E1B       75 26                        JNZ SHORT PathOfEx.00A10E43
$+1AD    00A10E1D       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+1B1    00A10E21       53                           PUSH EBX
$+1B2    00A10E22       57                           PUSH EDI
$+1B3    00A10E23       C787 F40A0000 02000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x2
$+1BD    00A10E2D       8D89 80000000                LEA ECX, DWORD PTR DS:[ECX+0x80]
$+1C3    00A10E33       E8 A860A7FF                  CALL PathOfEx.00486EE0
$+1C8    00A10E38       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+1CA    00A10E3A       5F                           POP EDI
$+1CB    00A10E3B       5E                           POP ESI
$+1CC    00A10E3C       5B                           POP EBX
$+1CD    00A10E3D       83C4 0C                      ADD ESP, 0xC
$+1D0    00A10E40       C2 0C00                      RETN 0xC
$+1D3    00A10E43       3D 05020000                  CMP EAX, 0x205
$+1D8    00A10E48       75 26                        JNZ SHORT PathOfEx.00A10E70
$+1DA    00A10E4A       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+1DE    00A10E4E       53                           PUSH EBX
$+1DF    00A10E4F       57                           PUSH EDI
$+1E0    00A10E50       C787 F40A0000 02000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x2
$+1EA    00A10E5A       8D89 00010000                LEA ECX, DWORD PTR DS:[ECX+0x100]
$+1F0    00A10E60       E8 7B60A7FF                  CALL PathOfEx.00486EE0
$+1F5    00A10E65       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+1F7    00A10E67       5F                           POP EDI                                  ; 0DB68000
$+1F8    00A10E68       5E                           POP ESI                                  ; 0DB68000
$+1F9    00A10E69       5B                           POP EBX                                  ; 0DB68000
$+1FA    00A10E6A       83C4 0C                      ADD ESP, 0xC
$+1FD    00A10E6D       C2 0C00                      RETN 0xC
$+200    00A10E70       3D 06020000                  CMP EAX, 0x206
$+205    00A10E75       75 0F                        JNZ SHORT PathOfEx.00A10E86
$+207    00A10E77       C787 F40A0000 02000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x2
$+211    00A10E81     ^ E9 FBFEFFFF                  JMP PathOfEx.00A10D81
$+216    00A10E86       3D 0B020000                  CMP EAX, 0x20B
$+21B    00A10E8B       75 5F                        JNZ SHORT PathOfEx.00A10EEC
$+21D    00A10E8D       8B4424 24                    MOV EAX, DWORD PTR SS:[ESP+0x24]
$+221    00A10E91       C1E8 10                      SHR EAX, 0x10
$+224    00A10E94       66:83F8 01                   CMP AX, 0x1
$+228    00A10E98       75 26                        JNZ SHORT PathOfEx.00A10EC0
$+22A    00A10E9A       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+22E    00A10E9E       53                           PUSH EBX
$+22F    00A10E9F       57                           PUSH EDI
$+230    00A10EA0       C787 F40A0000 08000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x8
$+23A    00A10EAA       8D89 80000000                LEA ECX, DWORD PTR DS:[ECX+0x80]
$+240    00A10EB0       E8 2B60A7FF                  CALL PathOfEx.00486EE0
$+245    00A10EB5       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+247    00A10EB7       5F                           POP EDI                                  ; 0DB68000
$+248    00A10EB8       5E                           POP ESI                                  ; 0DB68000
$+249    00A10EB9       5B                           POP EBX                                  ; 0DB68000
$+24A    00A10EBA       83C4 0C                      ADD ESP, 0xC
$+24D    00A10EBD       C2 0C00                      RETN 0xC
$+250    00A10EC0       66:83F8 02                   CMP AX, 0x2
$+254    00A10EC4       75 0A                        JNZ SHORT PathOfEx.00A10ED0
$+256    00A10EC6       C787 F40A0000 10000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x10
$+260    00A10ED0       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+264    00A10ED4       53                           PUSH EBX
$+265    00A10ED5       57                           PUSH EDI
$+266    00A10ED6       8D89 80000000                LEA ECX, DWORD PTR DS:[ECX+0x80]
$+26C    00A10EDC       E8 FF5FA7FF                  CALL PathOfEx.00486EE0
$+271    00A10EE1       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+273    00A10EE3       5F                           POP EDI                                  ; 0DB68000
$+274    00A10EE4       5E                           POP ESI                                  ; 0DB68000
$+275    00A10EE5       5B                           POP EBX                                  ; 0DB68000
$+276    00A10EE6       83C4 0C                      ADD ESP, 0xC
$+279    00A10EE9       C2 0C00                      RETN 0xC
$+27C    00A10EEC       3D 0C020000                  CMP EAX, 0x20C
$+281    00A10EF1       75 5F                        JNZ SHORT PathOfEx.00A10F52
$+283    00A10EF3       8B4424 24                    MOV EAX, DWORD PTR SS:[ESP+0x24]
$+287    00A10EF7       C1E8 10                      SHR EAX, 0x10
$+28A    00A10EFA       66:83F8 01                   CMP AX, 0x1
$+28E    00A10EFE       75 26                        JNZ SHORT PathOfEx.00A10F26
$+290    00A10F00       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+294    00A10F04       53                           PUSH EBX
$+295    00A10F05       57                           PUSH EDI
$+296    00A10F06       C787 F40A0000 08000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x8
$+2A0    00A10F10       8D89 00010000                LEA ECX, DWORD PTR DS:[ECX+0x100]
$+2A6    00A10F16       E8 C55FA7FF                  CALL PathOfEx.00486EE0
$+2AB    00A10F1B       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+2AD    00A10F1D       5F                           POP EDI                                  ; 0DB68000
$+2AE    00A10F1E       5E                           POP ESI                                  ; 0DB68000
$+2AF    00A10F1F       5B                           POP EBX                                  ; 0DB68000
$+2B0    00A10F20       83C4 0C                      ADD ESP, 0xC
$+2B3    00A10F23       C2 0C00                      RETN 0xC
$+2B6    00A10F26       66:83F8 02                   CMP AX, 0x2
$+2BA    00A10F2A       75 0A                        JNZ SHORT PathOfEx.00A10F36
$+2BC    00A10F2C       C787 F40A0000 10000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x10
$+2C6    00A10F36       8B4C24 1C                    MOV ECX, DWORD PTR SS:[ESP+0x1C]
$+2CA    00A10F3A       53                           PUSH EBX
$+2CB    00A10F3B       57                           PUSH EDI
$+2CC    00A10F3C       8D89 00010000                LEA ECX, DWORD PTR DS:[ECX+0x100]
$+2D2    00A10F42       E8 995FA7FF                  CALL PathOfEx.00486EE0
$+2D7    00A10F47       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+2D9    00A10F49       5F                           POP EDI                                  ; 0DB68000
$+2DA    00A10F4A       5E                           POP ESI                                  ; 0DB68000
$+2DB    00A10F4B       5B                           POP EBX                                  ; 0DB68000
$+2DC    00A10F4C       83C4 0C                      ADD ESP, 0xC
$+2DF    00A10F4F       C2 0C00                      RETN 0xC
$+2E2    00A10F52       3D 0D020000                  CMP EAX, 0x20D
$+2E7    00A10F57       75 35                        JNZ SHORT PathOfEx.00A10F8E
$+2E9    00A10F59       8B4424 24                    MOV EAX, DWORD PTR SS:[ESP+0x24]
$+2ED    00A10F5D       C1E8 10                      SHR EAX, 0x10
$+2F0    00A10F60       66:83F8 01                   CMP AX, 0x1
$+2F4    00A10F64       75 0F                        JNZ SHORT PathOfEx.00A10F75
$+2F6    00A10F66       C787 F40A0000 08000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x8
$+300    00A10F70     ^ E9 0CFEFFFF                  JMP PathOfEx.00A10D81
$+305    00A10F75       66:83F8 02                   CMP AX, 0x2
$+309    00A10F79     ^ 0F85 02FEFFFF                JNZ PathOfEx.00A10D81
$+30F    00A10F7F       C787 F40A0000 10000000       MOV DWORD PTR DS:[EDI+0xAF4], 0x10
$+319    00A10F89     ^ E9 F3FDFFFF                  JMP PathOfEx.00A10D81
$+31E    00A10F8E       3D 0A020000                  CMP EAX, 0x20A
$+323    00A10F93       75 4A                        JNZ SHORT PathOfEx.00A10FDF
$+325    00A10F95       8B4424 24                    MOV EAX, DWORD PTR SS:[ESP+0x24]
$+329    00A10F99       C1E8 10                      SHR EAX, 0x10
$+32C    00A10F9C       0FBFC8                       MOVSX ECX, AX
$+32F    00A10F9F       B8 89888888                  MOV EAX, 0x88888889
$+334    00A10FA4       F7E9                         IMUL ECX
$+336    00A10FA6       03D1                         ADD EDX, ECX
$+338    00A10FA8       C1FA 06                      SAR EDX, 0x6
$+33B    00A10FAB       8BF2                         MOV ESI, EDX
$+33D    00A10FAD       C1EE 1F                      SHR ESI, 0x1F
$+340    00A10FB0       03F2                         ADD ESI, EDX
$+342    00A10FB2       E8 29190000                  CALL PathOfEx.00A128E0
$+347    00A10FB7       8BC8                         MOV ECX, EAX
$+349    00A10FB9       8BC6                         MOV EAX, ESI
$+34B    00A10FBB       0FAFCE                       IMUL ECX, ESI
$+34E    00A10FBE       F7D8                         NEG EAX
$+350    00A10FC0       53                           PUSH EBX
$+351    00A10FC1       57                           PUSH EDI
$+352    00A10FC2       8987 F80A0000                MOV DWORD PTR DS:[EDI+0xAF8], EAX
$+358    00A10FC8       F7D9                         NEG ECX
$+35A    00A10FCA       898F FC0A0000                MOV DWORD PTR DS:[EDI+0xAFC], ECX
$+360    00A10FD0       8B4C24 24                    MOV ECX, DWORD PTR SS:[ESP+0x24]
$+364    00A10FD4       81C1 00030000                ADD ECX, 0x300
$+36A    00A10FDA       E8 015FA7FF                  CALL PathOfEx.00486EE0
$+36F    00A10FDF       8A03                         MOV AL, BYTE PTR DS:[EBX]
$+371    00A10FE1       5F                           POP EDI                                  ; 0DB68000
$+372    00A10FE2       5E                           POP ESI                                  ; 0DB68000
$+373    00A10FE3       5B                           POP EBX                                  ; 0DB68000
$+374    00A10FE4       83C4 0C                      ADD ESP, 0xC
$+377    00A10FE7       C2 0C00                      RETN 0xC
*/
//tag_收包处理范例，下面是stCDR_PromptInfo收包的例子
/*
$-8D     00A64E5F      CC                          INT3
$-8C     00A64E60      55                          PUSH EBP
$-8B     00A64E61      8BEC                        MOV EBP,ESP
$-89     00A64E63      6A FF                       PUSH -0x1
$-87     00A64E65      68 4370D700                 PUSH PathOfEx.00D77043
$-82     00A64E6A      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
$-7C     00A64E70      50                          PUSH EAX
$-7B     00A64E71      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
$-74     00A64E78      81EC E8000000               SUB ESP,0xE8
$-6E     00A64E7E      53                          PUSH EBX
$-6D     00A64E7F      8B5D 08                     MOV EBX,DWORD PTR SS:[EBP+0x8]
$-6A     00A64E82      57                          PUSH EDI
$-69     00A64E83      8BF9                        MOV EDI,ECX
$-67     00A64E85      8BCB                        MOV ECX,EBX
$-65     00A64E87      897D E8                     MOV DWORD PTR SS:[EBP-0x18],EDI
$-62     00A64E8A      803F 00                     CMP BYTE PTR DS:[EDI],0x0
$-5F     00A64E8D      74 13                       JE SHORT PathOfEx.00A64EA2
$-5D     00A64E8F      6A 01                       PUSH 0x1
$-5B     00A64E91      8D45 F3                     LEA EAX,DWORD PTR SS:[EBP-0xD]
$-58     00A64E94      50                          PUSH EAX
$-57     00A64E95      E8 B601D3FF                 CALL PathOfEx.00795050
$-52     00A64E9A      0FB645 F3                   MOVZX EAX,BYTE PTR SS:[EBP-0xD]
$-4E     00A64E9E      8BD0                        MOV EDX,EAX
$-4C     00A64EA0      EB 17                       JMP SHORT PathOfEx.00A64EB9
$-4A     00A64EA2      6A 02                       PUSH 0x2
$-48     00A64EA4      8D45 EC                     LEA EAX,DWORD PTR SS:[EBP-0x14]
$-45     00A64EA7      50                          PUSH EAX
$-44     00A64EA8      E8 A301D3FF                 CALL PathOfEx.00795050
$-3F     00A64EAD      FF75 EC                     PUSH DWORD PTR SS:[EBP-0x14]
$-3C     00A64EB0      FF15 4CE5D800               CALL DWORD PTR DS:[0xD8E54C]
$-36     00A64EB6      0FB7D0                      MOVZX EDX,AX
$-33     00A64EB9      8A83 B2000000               MOV AL,BYTE PTR DS:[EBX+0xB2]
$-2D     00A64EBF      8955 EC                     MOV DWORD PTR SS:[EBP-0x14],EDX
$-2A     00A64EC2      84C0                        TEST AL,AL
$-28     00A64EC4      74 0E                       JE SHORT PathOfEx.00A64ED4
$-26     00A64EC6      C683 B2000000 00            MOV BYTE PTR DS:[EBX+0xB2],0x0
$-1F     00A64ECD      32C0                        XOR AL,AL
$-1D     00A64ECF      E9 4D010000                 JMP PathOfEx.00A65021
$-18     00A64ED4      56                          PUSH ESI
$-17     00A64ED5      8B77 0C                     MOV ESI,DWORD PTR DS:[EDI+0xC]
$-14     00A64ED8      8B7F 10                     MOV EDI,DWORD PTR DS:[EDI+0x10]
$-11     00A64EDB      C645 0B 00                  MOV BYTE PTR SS:[EBP+0xB],0x0
$-D      00A64EDF      90                          NOP
$-C      00A64EE0      3BF7                        CMP ESI,EDI
$-A      00A64EE2      74 1A                       JE SHORT PathOfEx.00A64EFE
$-8      00A64EE4      8B0E                        MOV ECX,DWORD PTR DS:[ESI]			; g_h_AnalysisRecvPacket_.此处进行HOOK：过滤收包的patch_asm.txt
$-6      00A64EE6      53                          PUSH EBX                                 ; msg_data,收包结构，网络数据，还未转换成本地数据。这个地址是缓冲区长度，并不是当前收包的结构。
$-5      00A64EE7      52                          PUSH EDX                                 ; msg_id，经过ntohs转换成本地数据了
$-4      00A64EE8      8B01                        MOV EAX,DWORD PTR DS:[ECX]
$-2      00A64EEA      8B00                        MOV EAX,DWORD PTR DS:[EAX]               ; call tag_收包处理之顶层函数
$ ==>    00A64EEC      FFD0                        CALL EAX                                 ; 封包分发call，序列化解密后的网络数据，并生成一个经过ntohs后的对象，然后处理之
$+2      00A64EEE      84C0                        TEST AL,AL                               ; al=1则成功
$+4      00A64EF0      0F85 E6000000               JNZ PathOfEx.00A64FDC
$+A      00A64EF6      8B55 EC                     MOV EDX,DWORD PTR SS:[EBP-0x14]
$+D      00A64EF9      83C6 04                     ADD ESI,0x4
$+10     00A64EFC    ^ EB E2                       JMP SHORT PathOfEx.00A64EE0
$+12     00A64EFE      807D 0B 00                  CMP BYTE PTR SS:[EBP+0xB],0x0
$+16     00A64F02      0F85 D4000000               JNZ PathOfEx.00A64FDC
$+1C     00A64F08      83EC 08                     SUB ESP,0x8
$+1F     00A64F0B      8D8D 0CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xF4]
$+25     00A64F11      E8 FA70CAFF                 CALL PathOfEx.0070C010
$+2A     00A64F16      BA 34E6F300                 MOV EDX,PathOfEx.00F3E634                ; ASCII "Unable to deserialise packet with pid "
$+2F     00A64F1B      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
$+36     00A64F22      8D8D 1CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xE4]
$+3C     00A64F28      E8 638FCAFF                 CALL PathOfEx.0070DE90
$+41     00A64F2D      8B4D E8                     MOV ECX,DWORD PTR SS:[EBP-0x18]
$+44     00A64F30      8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
$+47     00A64F33      8B51 04                     MOV EDX,DWORD PTR DS:[ECX+0x4]
$+4A     00A64F36      85D2                        TEST EDX,EDX
$+4C     00A64F38      74 3A                       JE SHORT PathOfEx.00A64F74
$+4E     00A64F3A      0FB7F0                      MOVZX ESI,AX
$+51     00A64F3D      8D14B2                      LEA EDX,DWORD PTR DS:[EDX+ESI*4]
$+54     00A64F40      3B51 08                     CMP EDX,DWORD PTR DS:[ECX+0x8]
$+57     00A64F43      73 2F                       JNB SHORT PathOfEx.00A64F74
$+59     00A64F45      8B12                        MOV EDX,DWORD PTR DS:[EDX]
$+5B     00A64F47      8D8D 1CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xE4]
$+61     00A64F4D      56                          PUSH ESI
$+62     00A64F4E      E8 3D8FCAFF                 CALL PathOfEx.0070DE90
$+67     00A64F53      BA 1C74EF00                 MOV EDX,PathOfEx.00EF741C                ; UNICODE "("
$+6C     00A64F58      8BC8                        MOV ECX,EAX
$+6E     00A64F5A      E8 318FCAFF                 CALL PathOfEx.0070DE90
$+73     00A64F5F      8BC8                        MOV ECX,EAX
$+75     00A64F61      E8 0A1BCBFF                 CALL PathOfEx.00716A70
$+7A     00A64F66      BA 2074EF00                 MOV EDX,PathOfEx.00EF7420                ; UNICODE ")"
$+7F     00A64F6B      8BC8                        MOV ECX,EAX
$+81     00A64F6D      E8 1E8FCAFF                 CALL PathOfEx.0070DE90
$+86     00A64F72      EB 0F                       JMP SHORT PathOfEx.00A64F83
$+88     00A64F74      0FB7C0                      MOVZX EAX,AX
$+8B     00A64F77      8D8D 1CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xE4]
$+91     00A64F7D      50                          PUSH EAX
$+92     00A64F7E      E8 ED1ACBFF                 CALL PathOfEx.00716A70
$+97     00A64F83      8D45 BC                     LEA EAX,DWORD PTR SS:[EBP-0x44]
$+9A     00A64F86      50                          PUSH EAX
$+9B     00A64F87      8D8D 0CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xF4]
$+A1     00A64F8D      E8 7E6FCAFF                 CALL PathOfEx.0070BF10
$+A6     00A64F92      C645 FC 01                  MOV BYTE PTR SS:[EBP-0x4],0x1
$+AA     00A64F96      8378 14 10                  CMP DWORD PTR DS:[EAX+0x14],0x10
$+AE     00A64F9A      72 02                       JB SHORT PathOfEx.00A64F9E
$+B0     00A64F9C      8B00                        MOV EAX,DWORD PTR DS:[EAX]
$+B2     00A64F9E      8945 E4                     MOV DWORD PTR SS:[EBP-0x1C],EAX
$+B5     00A64FA1      0F57C0                      XORPS XMM0,XMM0
$+B8     00A64FA4      8D45 D8                     LEA EAX,DWORD PTR SS:[EBP-0x28]
$+BB     00A64FA7      C745 D4 6C3EDE00            MOV DWORD PTR SS:[EBP-0x2C],PathOfEx.00D>
$+C2     00A64FAE      50                          PUSH EAX
$+C3     00A64FAF      8D45 E4                     LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+C6     00A64FB2      660FD645 D8                 MOVQ QWORD PTR SS:[EBP-0x28],XMM0
$+CB     00A64FB7      50                          PUSH EAX
$+CC     00A64FB8      C645 E8 01                  MOV BYTE PTR SS:[EBP-0x18],0x1
$+D0     00A64FBC      E8 63FE2200                 CALL PathOfEx.00C94E24
$+D5     00A64FC1      83C4 08                     ADD ESP,0x8
$+D8     00A64FC4      C745 D4 3CA3ED00            MOV DWORD PTR SS:[EBP-0x2C],PathOfEx.00E>
$+DF     00A64FCB      8D45 D4                     LEA EAX,DWORD PTR SS:[EBP-0x2C]
$+E2     00A64FCE      895D E0                     MOV DWORD PTR SS:[EBP-0x20],EBX
$+E5     00A64FD1      68 F8F6FC00                 PUSH PathOfEx.00FCF6F8
$+EA     00A64FD6      50                          PUSH EAX
$+EB     00A64FD7      E8 170F2300                 CALL PathOfEx.00C95EF3
$+F0     00A64FDC      80BB B2000000 00            CMP BYTE PTR DS:[EBX+0xB2],0x0
$+F7     00A64FE3      74 0B                       JE SHORT PathOfEx.00A64FF0
$+F9     00A64FE5      C683 B2000000 00            MOV BYTE PTR DS:[EBX+0xB2],0x0
$+100    00A64FEC      32C0                        XOR AL,AL
$+102    00A64FEE      EB 30                       JMP SHORT PathOfEx.00A65020
*/
/**/
//tag_收包处理之顶层函数
/*
$-1      00A650AF      CC                          INT3
$ ==>    00A650B0      55                          PUSH EBP                                 ; tag_收包处理之顶层函数
$+1      00A650B1      8BEC                        MOV EBP,ESP
$+3      00A650B3      64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
$+9      00A650B9      6A FF                       PUSH -0x1
$+B      00A650BB      68 5E70D700                 PUSH PathOfEx.00D7705E
$+10     00A650C0      50                          PUSH EAX
$+11     00A650C1      64:8925 00000000            MOV DWORD PTR FS:[0],ESP
$+18     00A650C8      53                          PUSH EBX
$+19     00A650C9      56                          PUSH ESI
$+1A     00A650CA      57                          PUSH EDI
$+1B     00A650CB      8BF9                        MOV EDI,ECX
$+1D     00A650CD      8B4D 08                     MOV ECX,DWORD PTR SS:[EBP+0x8]
$+20     00A650D0      66:85C9                     TEST CX,CX
$+23     00A650D3      75 28                       JNZ SHORT PathOfEx.00A650FD
$+25     00A650D5      51                          PUSH ECX
$+26     00A650D6      8BC4                        MOV EAX,ESP
$+28     00A650D8      FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
$+2B     00A650DB      C700 00000000               MOV DWORD PTR DS:[EAX],0x0
$+31     00A650E1      8B4F 04                     MOV ECX,DWORD PTR DS:[EDI+0x4]
$+34     00A650E4      8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+36     00A650E6      FF10                        CALL DWORD PTR DS:[EAX]
$+38     00A650E8      B0 01                       MOV AL,0x1
$+3A     00A650EA      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
$+3D     00A650ED      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
$+44     00A650F4      5F                          POP EDI
$+45     00A650F5      5E                          POP ESI
$+46     00A650F6      5B                          POP EBX
$+47     00A650F7      8BE5                        MOV ESP,EBP
$+49     00A650F9      5D                          POP EBP
$+4A     00A650FA      C2 0800                     RETN 0x8
$+4D     00A650FD      8B5D 0C                     MOV EBX,DWORD PTR SS:[EBP+0xC]
$+50     00A65100      8B07                        MOV EAX,DWORD PTR DS:[EDI]
$+52     00A65102      53                          PUSH EBX
$+53     00A65103      51                          PUSH ECX
$+54     00A65104      8BCF                        MOV ECX,EDI
$+56     00A65106      FF50 04                     CALL DWORD PTR DS:[EAX+0x4]              ; tag_收包处理之序列化消息
$+59     00A65109      8BF0                        MOV ESI,EAX
$+5B     00A6510B      C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
$+62     00A65112      85F6                        TEST ESI,ESI
$+64     00A65114      75 04                       JNZ SHORT PathOfEx.00A6511A
$+66     00A65116      32DB                        XOR BL,BL
$+68     00A65118      EB 20                       JMP SHORT PathOfEx.00A6513A
$+6A     00A6511A      8A83 B2000000               MOV AL,BYTE PTR DS:[EBX+0xB2]
$+70     00A65120      84C0                        TEST AL,AL
$+72     00A65122      75 14                       JNZ SHORT PathOfEx.00A65138
$+74     00A65124      51                          PUSH ECX
$+75     00A65125      8BCC                        MOV ECX,ESP
$+77     00A65127      8BC6                        MOV EAX,ESI
$+79     00A65129      33F6                        XOR ESI,ESI
$+7B     00A6512B      53                          PUSH EBX
$+7C     00A6512C      8975 0C                     MOV DWORD PTR SS:[EBP+0xC],ESI
$+7F     00A6512F      8901                        MOV DWORD PTR DS:[ECX],EAX
$+81     00A65131      8B4F 04                     MOV ECX,DWORD PTR DS:[EDI+0x4]
$+84     00A65134      8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+86     00A65136      FF10                        CALL DWORD PTR DS:[EAX]
$+88     00A65138      B3 01                       MOV BL,0x1
$+8A     00A6513A      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
$+91     00A65141      85F6                        TEST ESI,ESI
$+93     00A65143      74 0F                       JE SHORT PathOfEx.00A65154
$+95     00A65145      C745 FC 03000000            MOV DWORD PTR SS:[EBP-0x4],0x3
$+9C     00A6514C      8BCE                        MOV ECX,ESI
$+9E     00A6514E      8B16                        MOV EDX,DWORD PTR DS:[ESI]
$+A0     00A65150      6A 01                       PUSH 0x1
$+A2     00A65152      FF12                        CALL DWORD PTR DS:[EDX]
$+A4     00A65154      8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
$+A7     00A65157      8AC3                        MOV AL,BL
$+A9     00A65159      5F                          POP EDI
$+AA     00A6515A      5E                          POP ESI
$+AB     00A6515B      64:890D 00000000            MOV DWORD PTR FS:[0],ECX
$+B2     00A65162      5B                          POP EBX
$+B3     00A65163      8BE5                        MOV ESP,EBP
$+B5     00A65165      5D                          POP EBP
$+B6     00A65166      C2 0800                     RETN 0x8
$+B9     00A65169      CC                          INT3
$+BA     00A6516A      CC                          INT3
$+BB     00A6516B      CC                          INT3
$+BC     00A6516C      CC                          INT3
$+BD     00A6516D      CC                          INT3
$+BE     00A6516E      CC                          INT3
$+BF     00A6516F      CC                          INT3
$+C0     00A65170      B8 5CE6F300                 MOV EAX,PathOfEx.00F3E65C                ; ASCII "HNC Protocol Violated"
$+C5     00A65175      C3                          RETN
$+C6     00A65176      CC                          INT3
*/
/**/
//tag_收包处理之序列化消息
/*
$-1      008F8FAF      CC                          INT3
$ ==>    008F8FB0      55                          PUSH EBP                                      ; tag_收包处理之序列化消息
$+1      008F8FB1      8BEC                        MOV EBP,ESP
$+3      008F8FB3      83E4 F8                     AND ESP,0xFFFFFFF8
$+6      008F8FB6      0FB745 08                   MOVZX EAX,WORD PTR SS:[EBP+0x8]
$+A      008F8FBA      83EC 0C                     SUB ESP,0xC
$+D      008F8FBD      48                          DEC EAX
$+E      008F8FBE      53                          PUSH EBX
$+F      008F8FBF      56                          PUSH ESI
$+10     008F8FC0      33DB                        XOR EBX,EBX
$+12     008F8FC2      57                          PUSH EDI
$+13     008F8FC3      83F8 24                     CMP EAX,0x24                                  ; 比对消息id
$+16     008F8FC6      0F87 57030000               JA PathOfEx.008F9323
$+1C     008F8FCC      0FB680 70938F00             MOVZX EAX,BYTE PTR DS:[EAX+0x8F9370]
$+23     008F8FD3      FF2485 30938F00             JMP DWORD PTR DS:[EAX*4+0x8F9330]             ; switch，包括 tag_stCDR_PromptInfo返回封包
$+2A     008F8FDA      68 88000000                 PUSH 0x88
$+2F     008F8FDF      E8 B2333800                 CALL PathOfEx.00C7C396
$+34     008F8FE4      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+37     008F8FE7      8BD8                        MOV EBX,EAX
$+39     008F8FE9      83C4 04                     ADD ESP,0x4
$+3C     008F8FEC      895C24 10                   MOV DWORD PTR SS:[ESP+0x10],EBX
$+40     008F8FF0      B8 01000000                 MOV EAX,0x1
$+45     008F8FF5      66:8943 04                  MOV WORD PTR DS:[EBX+0x4],AX
$+49     008F8FF9      8D43 08                     LEA EAX,DWORD PTR DS:[EBX+0x8]
$+4C     008F8FFC      68 80000000                 PUSH 0x80
$+51     008F9001      50                          PUSH EAX
$+52     008F9002      C703 80A8EF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EFA880
$+58     008F9008      E8 43C0E9FF                 CALL PathOfEx.00795050
$+5D     008F900D      8BC3                        MOV EAX,EBX
$+5F     008F900F      5F                          POP EDI
$+60     008F9010      5E                          POP ESI
$+61     008F9011      5B                          POP EBX
$+62     008F9012      8BE5                        MOV ESP,EBP
$+64     008F9014      5D                          POP EBP
$+65     008F9015      C2 0800                     RETN 0x8
$+68     008F9018      6A 50                       PUSH 0x50
$+6A     008F901A      E8 77333800                 CALL PathOfEx.00C7C396
$+6F     008F901F      8BD8                        MOV EBX,EAX
$+71     008F9021      83C4 04                     ADD ESP,0x4
$+74     008F9024      895C24 10                   MOV DWORD PTR SS:[ESP+0x10],EBX
$+78     008F9028      B8 03000000                 MOV EAX,0x3
$+7D     008F902D      8BCB                        MOV ECX,EBX
$+7F     008F902F      FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
$+82     008F9032      66:8943 04                  MOV WORD PTR DS:[EBX+0x4],AX
$+86     008F9036      33C0                        XOR EAX,EAX
$+88     008F9038      C703 C8B1EF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EFB1C8
$+8E     008F903E      C743 2C 00000000            MOV DWORD PTR DS:[EBX+0x2C],0x0
$+95     008F9045      C743 30 00000000            MOV DWORD PTR DS:[EBX+0x30],0x0
$+9C     008F904C      C743 48 07000000            MOV DWORD PTR DS:[EBX+0x48],0x7
$+A3     008F9053      C743 44 00000000            MOV DWORD PTR DS:[EBX+0x44],0x0
$+AA     008F905A      66:8943 34                  MOV WORD PTR DS:[EBX+0x34],AX
$+AE     008F905E      8943 4C                     MOV DWORD PTR DS:[EBX+0x4C],EAX
$+B1     008F9061      E8 5AF8FFFF                 CALL PathOfEx.008F88C0
$+B6     008F9066      8BC3                        MOV EAX,EBX
$+B8     008F9068      5F                          POP EDI
$+B9     008F9069      5E                          POP ESI
$+BA     008F906A      5B                          POP EBX
$+BB     008F906B      8BE5                        MOV ESP,EBP
$+BD     008F906D      5D                          POP EBP
$+BE     008F906E      C2 0800                     RETN 0x8
$+C1     008F9071      6A 0C                       PUSH 0xC
$+C3     008F9073      E8 1E333800                 CALL PathOfEx.00C7C396
$+C8     008F9078      8BD8                        MOV EBX,EAX
$+CA     008F907A      B8 07000000                 MOV EAX,0x7
$+CF     008F907F      895C24 14                   MOV DWORD PTR SS:[ESP+0x14],EBX
$+D3     008F9083      C703 9C9AEF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EF9A9C
$+D9     008F9089      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+DC     008F908C      8D73 08                     LEA ESI,DWORD PTR DS:[EBX+0x8]
$+DF     008F908F      83C4 04                     ADD ESP,0x4
$+E2     008F9092      66:8943 04                  MOV WORD PTR DS:[EBX+0x4],AX
$+E6     008F9096      6A 02                       PUSH 0x2
$+E8     008F9098      56                          PUSH ESI
$+E9     008F9099      E8 B2BFE9FF                 CALL PathOfEx.00795050
$+EE     008F909E      0FB706                      MOVZX EAX,WORD PTR DS:[ESI]
$+F1     008F90A1      50                          PUSH EAX
$+F2     008F90A2      FF15 4CE5D800               CALL DWORD PTR DS:[0xD8E54C]
$+F8     008F90A8      66:8906                     MOV WORD PTR DS:[ESI],AX
$+FB     008F90AB      8BC3                        MOV EAX,EBX
$+FD     008F90AD      5F                          POP EDI
$+FE     008F90AE      5E                          POP ESI
$+FF     008F90AF      5B                          POP EBX
$+100    008F90B0      8BE5                        MOV ESP,EBP
$+102    008F90B2      5D                          POP EBP
$+103    008F90B3      C2 0800                     RETN 0x8
$+106    008F90B6      6A 0C                       PUSH 0xC
$+108    008F90B8      E8 D9323800                 CALL PathOfEx.00C7C396
$+10D    008F90BD      8BD8                        MOV EBX,EAX
$+10F    008F90BF      B8 09000000                 MOV EAX,0x9
$+114    008F90C4      895C24 14                   MOV DWORD PTR SS:[ESP+0x14],EBX
$+118    008F90C8      C703 9C9AEF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EF9A9C
$+11E    008F90CE    ^ EB B9                       JMP SHORT PathOfEx.008F9089
$+120    008F90D0      6A 0C                       PUSH 0xC
$+122    008F90D2      E8 BF323800                 CALL PathOfEx.00C7C396
$+127    008F90D7      8BD8                        MOV EBX,EAX
$+129    008F90D9      B8 0B000000                 MOV EAX,0xB
$+12E    008F90DE      895C24 14                   MOV DWORD PTR SS:[ESP+0x14],EBX
$+132    008F90E2      C703 9C9AEF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EF9A9C
$+138    008F90E8    ^ EB 9F                       JMP SHORT PathOfEx.008F9089
$+13A    008F90EA      6A 0C                       PUSH 0xC                                      ; tag_stCDR_PromptInfo返回封包
$+13C    008F90EC      E8 A5323800                 CALL PathOfEx.00C7C396                        ; tag_收包之申请对象
$+141    008F90F1      83C4 04                     ADD ESP,0x4                                   ; eax=申请到的对象
$+144    008F90F4      894424 10                   MOV DWORD PTR SS:[ESP+0x10],EAX
$+148    008F90F8      B9 0D000000                 MOV ECX,0xD                                   ; 当前处理的消息id，对于创建角色返回封包来说，是0xd
$+14D    008F90FD      C640 08 00                  MOV BYTE PTR DS:[EAX+0x8],0x0                 ; 开始初始化结构体
$+151    008F9101      8D78 0A                     LEA EDI,DWORD PTR DS:[EAX+0xA]
$+154    008F9104      66:8948 04                  MOV WORD PTR DS:[EAX+0x4],CX                  ; 消息id
$+158    008F9108      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+15B    008F910B      8D58 08                     LEA EBX,DWORD PTR DS:[EAX+0x8]
$+15E    008F910E      6A 02                       PUSH 0x2
$+160    008F9110      57                          PUSH EDI
$+161    008F9111      C700 A4B1EF00               MOV DWORD PTR DS:[EAX],PathOfEx.00EFB1A4
$+167    008F9117      E8 34BFE9FF                 CALL PathOfEx.00795050                        ; tag_读取收包字段内容，读取的是网络数据
$+16C    008F911C      0FB707                      MOVZX EAX,WORD PTR DS:[EDI]
$+16F    008F911F      50                          PUSH EAX
$+170    008F9120      FF15 4CE5D800               CALL DWORD PTR DS:[0xD8E54C]                  ; 转换成本地数据
$+176    008F9126      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+179    008F9129      6A 01                       PUSH 0x1
$+17B    008F912B      53                          PUSH EBX
$+17C    008F912C      66:8907                     MOV WORD PTR DS:[EDI],AX
$+17F    008F912F      E8 1CBFE9FF                 CALL PathOfEx.00795050
$+184    008F9134      8B5C24 10                   MOV EBX,DWORD PTR SS:[ESP+0x10]
$+188    008F9138      8BC3                        MOV EAX,EBX
$+18A    008F913A      5F                          POP EDI
$+18B    008F913B      5E                          POP ESI
$+18C    008F913C      5B                          POP EBX
$+18D    008F913D      8BE5                        MOV ESP,EBP
$+18F    008F913F      5D                          POP EBP
$+190    008F9140      C2 0800                     RETN 0x8
$+193    008F9143      6A 60                       PUSH 0x60
$+195    008F9145      E8 4C323800                 CALL PathOfEx.00C7C396
$+19A    008F914A      8BD8                        MOV EBX,EAX
$+19C    008F914C      83C4 04                     ADD ESP,0x4
$+19F    008F914F      895C24 10                   MOV DWORD PTR SS:[ESP+0x10],EBX
$+1A3    008F9153      B8 0E000000                 MOV EAX,0xE
$+1A8    008F9158      8BCB                        MOV ECX,EBX
$+1AA    008F915A      FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
$+1AD    008F915D      66:8943 04                  MOV WORD PTR DS:[EBX+0x4],AX
$+1B1    008F9161      C703 B0B1EF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EFB1B0
$+1B7    008F9167      C743 08 00000000            MOV DWORD PTR DS:[EBX+0x8],0x0
$+1BE    008F916E      C743 0C 00000000            MOV DWORD PTR DS:[EBX+0xC],0x0
$+1C5    008F9175      C743 10 00000000            MOV DWORD PTR DS:[EBX+0x10],0x0
$+1CC    008F917C      E8 6FFBFFFF                 CALL PathOfEx.008F8CF0
$+1D1    008F9181      8BC3                        MOV EAX,EBX
$+1D3    008F9183      5F                          POP EDI
$+1D4    008F9184      5E                          POP ESI
$+1D5    008F9185      5B                          POP EBX
$+1D6    008F9186      8BE5                        MOV ESP,EBP
$+1D8    008F9188      5D                          POP EBP
$+1D9    008F9189      C2 0800                     RETN 0x8
$+1DC    008F918C      6A 1C                       PUSH 0x1C
$+1DE    008F918E      E8 03323800                 CALL PathOfEx.00C7C396
$+1E3    008F9193      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+1E6    008F9196      8BD8                        MOV EBX,EAX
$+1E8    008F9198      895C24 14                   MOV DWORD PTR SS:[ESP+0x14],EBX
$+1EC    008F919C      8D53 08                     LEA EDX,DWORD PTR DS:[EBX+0x8]
$+1EF    008F919F      B8 0F000000                 MOV EAX,0xF
$+1F4    008F91A4      83C4 04                     ADD ESP,0x4
$+1F7    008F91A7      66:8943 04                  MOV WORD PTR DS:[EBX+0x4],AX
$+1FB    008F91AB      C703 D4B1EF00               MOV DWORD PTR DS:[EBX],PathOfEx.00EFB1D4
$+201    008F91B1      C702 00000000               MOV DWORD PTR DS:[EDX],0x0
$+207    008F91B7      C742 04 00000000            MOV DWORD PTR DS:[EDX+0x4],0x0
$+20E    008F91BE      C742 08 00000000            MOV DWORD PTR DS:[EDX+0x8],0x0
$+215    008F91C5      C742 0C FFFFFFFF            MOV DWORD PTR DS:[EDX+0xC],-0x1
$+21C    008F91CC      E8 BFCF1600                 CALL PathOfEx.00A66190
$+221    008F91D1      8BC3                        MOV EAX,EBX
$+223    008F91D3      5F                          POP EDI
$+224    008F91D4      5E                          POP ESI
$+225    008F91D5      5B                          POP EBX
$+226    008F91D6      8BE5                        MOV ESP,EBP
$+228    008F91D8      5D                          POP EBP
$+229    008F91D9      C2 0800                     RETN 0x8
*/
/**/
//tag_收包之申请对象
/*
$-F      00C7C387      C3                          RETN
$-E      00C7C388      55                          PUSH EBP
$-D      00C7C389      8BEC                        MOV EBP,ESP
$-B      00C7C38B      FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
$-8      00C7C38E      E8 ADBFDCFF                 CALL PathOfEx.00A48340
$-3      00C7C393      59                          POP ECX
$-2      00C7C394      5D                          POP EBP
$-1      00C7C395      C3                          RETN
$ ==>    00C7C396      55                          PUSH EBP                                      ; tag_收包之申请对象
$+1      00C7C397      8BEC                        MOV EBP,ESP
$+3      00C7C399      EB 1F                       JMP SHORT PathOfEx.00C7C3BA
$+5      00C7C39B      FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
$+8      00C7C39E      E8 4F840300                 CALL PathOfEx.00CB47F2
$+D      00C7C3A3      59                          POP ECX
$+E      00C7C3A4      85C0                        TEST EAX,EAX
$+10     00C7C3A6      75 12                       JNZ SHORT PathOfEx.00C7C3BA
$+12     00C7C3A8      837D 08 FF                  CMP DWORD PTR SS:[EBP+0x8],-0x1
$+16     00C7C3AC      75 07                       JNZ SHORT PathOfEx.00C7C3B5
$+18     00C7C3AE      E8 9B160000                 CALL PathOfEx.00C7DA4E
$+1D     00C7C3B3      EB 05                       JMP SHORT PathOfEx.00C7C3BA
$+1F     00C7C3B5      E8 86DCFEFF                 CALL PathOfEx.00C6A040
$+24     00C7C3BA      FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
$+27     00C7C3BD      E8 93220200                 CALL PathOfEx.00C9E655
$+2C     00C7C3C2      59                          POP ECX
$+2D     00C7C3C3      85C0                        TEST EAX,EAX
$+2F     00C7C3C5    ^ 74 D4                       JE SHORT PathOfEx.00C7C39B
$+31     00C7C3C7      5D                          POP EBP
$+32     00C7C3C8      C3                          RETN
$+33     00C7C3C9      53                          PUSH EBX
$+34     00C7C3CA      56                          PUSH ESI
$+35     00C7C3CB      57                          PUSH EDI
$+36     00C7C3CC      6A 00                       PUSH 0x0
$+38     00C7C3CE      68 A00F0000                 PUSH 0xFA0
$+3D     00C7C3D3      68 046DFF00                 PUSH PathOfEx.00FF6D04
$+42     00C7C3D8      E8 25BC0100                 CALL PathOfEx.00C98002
$+47     00C7C3DD      83C4 0C                     ADD ESP,0xC
$+4A     00C7C3E0      68 8061EA00                 PUSH PathOfEx.00EA6180                        ; UNICODE "kernel32.dll"
$+4F     00C7C3E5      FF15 E0E0D800               CALL DWORD PTR DS:[0xD8E0E0]
$+55     00C7C3EB      8BF0                        MOV ESI,EAX
$+57     00C7C3ED      85F6                        TEST ESI,ESI
$+59     00C7C3EF      0F84 8C000000               JE PathOfEx.00C7C481
$+5F     00C7C3F5      68 6061DE00                 PUSH PathOfEx.00DE6160                        ; ASCII "InitializeConditionVariable"
$+64     00C7C3FA      56                          PUSH ESI
$+65     00C7C3FB      FF15 DCE0D800               CALL DWORD PTR DS:[0xD8E0DC]
$+6B     00C7C401      68 B061DE00                 PUSH PathOfEx.00DE61B0                        ; ASCII "SleepConditionVariableCS"
$+70     00C7C406      56                          PUSH ESI
$+71     00C7C407      8BD8                        MOV EBX,EAX
$+73     00C7C409      FF15 DCE0D800               CALL DWORD PTR DS:[0xD8E0DC]
$+79     00C7C40F      68 9461DE00                 PUSH PathOfEx.00DE6194                        ; ASCII "WakeAllConditionVariable"
$+7E     00C7C414      56                          PUSH ESI
$+7F     00C7C415      8BF8                        MOV EDI,EAX
$+81     00C7C417      FF15 DCE0D800               CALL DWORD PTR DS:[0xD8E0DC]
$+87     00C7C41D      8BF0                        MOV ESI,EAX
$+89     00C7C41F      85DB                        TEST EBX,EBX
$+8B     00C7C421      74 37                       JE SHORT PathOfEx.00C7C45A
$+8D     00C7C423      85FF                        TEST EDI,EDI
$+8F     00C7C425      74 33                       JE SHORT PathOfEx.00C7C45A
$+91     00C7C427      85F6                        TEST ESI,ESI
$+93     00C7C429      74 2F                       JE SHORT PathOfEx.00C7C45A
$+95     00C7C42B      8325 206DFF00 00            AND DWORD PTR DS:[0xFF6D20],0x0
$+9C     00C7C432      8BCB                        MOV ECX,EBX
$+9E     00C7C434      68 1C6DFF00                 PUSH PathOfEx.00FF6D1C
$+A3     00C7C439      E8 A7100000                 CALL PathOfEx.00C7D4E5
$+A8     00C7C43E      FFD3                        CALL EBX
$+AA     00C7C440      57                          PUSH EDI
$+AB     00C7C441      E8 43000000                 CALL PathOfEx.00C7C489
$+B0     00C7C446      56                          PUSH ESI
$+B1     00C7C447      A3 246DFF00                 MOV DWORD PTR DS:[0xFF6D24],EAX
$+B6     00C7C44C      E8 38000000                 CALL PathOfEx.00C7C489
$+BB     00C7C451      59                          POP ECX
$+BC     00C7C452      59                          POP ECX
$+BD     00C7C453      A3 286DFF00                 MOV DWORD PTR DS:[0xFF6D28],EAX
$+C2     00C7C458      EB 16                       JMP SHORT PathOfEx.00C7C470
$+C4     00C7C45A      33C0                        XOR EAX,EAX
$+C6     00C7C45C      50                          PUSH EAX
$+C7     00C7C45D      50                          PUSH EAX
$+C8     00C7C45E      6A 01                       PUSH 0x1
$+CA     00C7C460      50                          PUSH EAX
$+CB     00C7C461      FF15 38E3D800               CALL DWORD PTR DS:[0xD8E338]
$+D1     00C7C467      A3 206DFF00                 MOV DWORD PTR DS:[0xFF6D20],EAX
*/
//tag_读取收包字段内容
/*
$-1      0079504F      CC                          INT3
$ ==>    00795050      53                          PUSH EBX                                      ; tag_读取收包字段内容，读取的是网络数据
$+1      00795051      8BD9                        MOV EBX,ECX
$+3      00795053      80BB B2000000 00            CMP BYTE PTR DS:[EBX+0xB2],0x0
$+A      0079505A      0F85 EF000000               JNZ PathOfEx.0079514F
$+10     00795060      8B8B C4000000               MOV ECX,DWORD PTR DS:[EBX+0xC4]
$+16     00795066      8B83 D8000000               MOV EAX,DWORD PTR DS:[EBX+0xD8]
$+1C     0079506C      2B83 D4000000               SUB EAX,DWORD PTR DS:[EBX+0xD4]
$+22     00795072      55                          PUSH EBP
$+23     00795073      8B6C24 10                   MOV EBP,DWORD PTR SS:[ESP+0x10]
$+27     00795077      03CD                        ADD ECX,EBP
$+29     00795079      3BC8                        CMP ECX,EAX
$+2B     0079507B      76 26                       JBE SHORT PathOfEx.007950A3
$+2D     0079507D      51                          PUSH ECX
$+2E     0079507E      8BCB                        MOV ECX,EBX
$+30     00795080      E8 EB070000                 CALL PathOfEx.00795870
$+35     00795085      84C0                        TEST AL,AL
$+37     00795087      75 1A                       JNZ SHORT PathOfEx.007950A3
$+39     00795089      6A 02                       PUSH 0x2
$+3B     0079508B      FF33                        PUSH DWORD PTR DS:[EBX]
$+3D     0079508D      FF15 04E5D800               CALL DWORD PTR DS:[0xD8E504]
$+43     00795093      5D                          POP EBP
$+44     00795094      66:C783 B0000000 0000       MOV WORD PTR DS:[EBX+0xB0],0x0
$+4D     0079509D      8BC3                        MOV EAX,EBX
$+4F     0079509F      5B                          POP EBX
$+50     007950A0      C2 0800                     RETN 0x8
$+53     007950A3      8B83 C0000000               MOV EAX,DWORD PTR DS:[EBX+0xC0]
$+59     007950A9      8B8B C4000000               MOV ECX,DWORD PTR DS:[EBX+0xC4]
$+5F     007950AF      2BC1                        SUB EAX,ECX
$+61     007950B1      3BE8                        CMP EBP,EAX
$+63     007950B3      76 0E                       JBE SHORT PathOfEx.007950C3
$+65     007950B5      5D                          POP EBP
$+66     007950B6      C683 B2000000 01            MOV BYTE PTR DS:[EBX+0xB2],0x1
$+6D     007950BD      8BC3                        MOV EAX,EBX
$+6F     007950BF      5B                          POP EBX
$+70     007950C0      C2 0800                     RETN 0x8
$+73     007950C3      8B83 BC000000               MOV EAX,DWORD PTR DS:[EBX+0xBC]
$+79     007950C9      33D2                        XOR EDX,EDX
$+7B     007950CB      03C1                        ADD EAX,ECX
$+7D     007950CD      8B8B D8000000               MOV ECX,DWORD PTR DS:[EBX+0xD8]
$+83     007950D3      2B8B D4000000               SUB ECX,DWORD PTR DS:[EBX+0xD4]
$+89     007950D9      F7F1                        DIV ECX
$+8B     007950DB      8B8B D8000000               MOV ECX,DWORD PTR DS:[EBX+0xD8]
$+91     007950E1      2B8B D4000000               SUB ECX,DWORD PTR DS:[EBX+0xD4]
$+97     007950E7      8D042A                      LEA EAX,DWORD PTR DS:[EDX+EBP]
$+9A     007950EA      3BC1                        CMP EAX,ECX
$+9C     007950EC      8B83 D4000000               MOV EAX,DWORD PTR DS:[EBX+0xD4]
$+A2     007950F2      77 1D                       JA SHORT PathOfEx.00795111
$+A4     007950F4      55                          PUSH EBP
$+A5     007950F5      03C2                        ADD EAX,EDX
$+A7     007950F7      50                          PUSH EAX
$+A8     007950F8      FF7424 14                   PUSH DWORD PTR SS:[ESP+0x14]
$+AC     007950FC      E8 CF045000                 CALL PathOfEx.00C955D0
$+B1     00795101      83C4 0C                     ADD ESP,0xC
$+B4     00795104      8BC3                        MOV EAX,EBX
$+B6     00795106      01AB C4000000               ADD DWORD PTR DS:[EBX+0xC4],EBP
$+BC     0079510C      5D                          POP EBP
$+BD     0079510D      5B                          POP EBX
$+BE     0079510E      C2 0800                     RETN 0x8
$+C1     00795111      56                          PUSH ESI
$+C2     00795112      8B7424 10                   MOV ESI,DWORD PTR SS:[ESP+0x10]
$+C6     00795116      03C2                        ADD EAX,EDX
$+C8     00795118      57                          PUSH EDI
$+C9     00795119      8BBB D8000000               MOV EDI,DWORD PTR DS:[EBX+0xD8]
$+CF     0079511F      2BBB D4000000               SUB EDI,DWORD PTR DS:[EBX+0xD4]
$+D5     00795125      2BFA                        SUB EDI,EDX
$+D7     00795127      57                          PUSH EDI
$+D8     00795128      50                          PUSH EAX
$+D9     00795129      56                          PUSH ESI
$+DA     0079512A      E8 A1045000                 CALL PathOfEx.00C955D0
$+DF     0079512F      8BC5                        MOV EAX,EBP
$+E1     00795131      2BC7                        SUB EAX,EDI
$+E3     00795133      50                          PUSH EAX
$+E4     00795134      FFB3 D4000000               PUSH DWORD PTR DS:[EBX+0xD4]
$+EA     0079513A      8D0437                      LEA EAX,DWORD PTR DS:[EDI+ESI]
$+ED     0079513D      50                          PUSH EAX
$+EE     0079513E      E8 8D045000                 CALL PathOfEx.00C955D0
$+F3     00795143      83C4 18                     ADD ESP,0x18
$+F6     00795146      01AB C4000000               ADD DWORD PTR DS:[EBX+0xC4],EBP
$+FC     0079514C      5F                          POP EDI
$+FD     0079514D      5E                          POP ESI
$+FE     0079514E      5D                          POP EBP
$+FF     0079514F      8BC3                        MOV EAX,EBX
$+101    00795151      5B                          POP EBX
$+102    00795152      C2 0800                     RETN 0x8
$+105    00795155      CC                          INT3
$+106    00795156      CC                          INT3
$+107    00795157      CC                          INT3
$+108    00795158      CC                          INT3
$+109    00795159      CC                          INT3
$+10A    0079515A      CC                          INT3
$+10B    0079515B      CC                          INT3
$+10C    0079515C      CC                          INT3
$+10D    0079515D      CC                          INT3
$+10E    0079515E      CC                          INT3
$+10F    0079515F      CC                          INT3
$+110    00795160      55                          PUSH EBP                                      ; 构建发包内容封包
$+111    00795161      8BEC                        MOV EBP,ESP
$+113    00795163      83E4 F8                     AND ESP,0xFFFFFFF8
$+116    00795166      83EC 10                     SUB ESP,0x10
$+119    00795169      56                          PUSH ESI
$+11A    0079516A      8BF1                        MOV ESI,ECX
$+11C    0079516C      57                          PUSH EDI
$+11D    0079516D      8B7D 0C                     MOV EDI,DWORD PTR SS:[EBP+0xC]
$+120    00795170      8B86 CC000000               MOV EAX,DWORD PTR DS:[ESI+0xCC]
$+126    00795176      2B86 C8000000               SUB EAX,DWORD PTR DS:[ESI+0xC8]
$+12C    0079517C      3BF8                        CMP EDI,EAX
$+12E    0079517E      76 27                       JBE SHORT PathOfEx.007951A7
$+130    00795180      57                          PUSH EDI
$+131    00795181      E8 8A060000                 CALL PathOfEx.00795810
$+136    00795186      84C0                        TEST AL,AL
$+138    00795188      75 1D                       JNZ SHORT PathOfEx.007951A7
$+13A    0079518A      6A 02                       PUSH 0x2
$+13C    0079518C      FF36                        PUSH DWORD PTR DS:[ESI]
$+13E    0079518E      FF15 04E5D800               CALL DWORD PTR DS:[0xD8E504]
$+144    00795194      66:C786 B0000000 0000       MOV WORD PTR DS:[ESI+0xB0],0x0
$+14D    0079519D      8BC6                        MOV EAX,ESI
$+14F    0079519F      5F                          POP EDI
$+150    007951A0      5E                          POP ESI
$+151    007951A1      8BE5                        MOV ESP,EBP
$+153    007951A3      5D                          POP EBP
$+154    007951A4      C2 0800                     RETN 0x8
$+157    007951A7      8B8E CC000000               MOV ECX,DWORD PTR DS:[ESI+0xCC]
$+15D    007951AD      8B86 B4000000               MOV EAX,DWORD PTR DS:[ESI+0xB4]
$+163    007951B3      2B8E C8000000               SUB ECX,DWORD PTR DS:[ESI+0xC8]
$+169    007951B9      03C7                        ADD EAX,EDI
$+16B    007951BB      3BC1                        CMP EAX,ECX
$+16D    007951BD      76 3E                       JBE SHORT PathOfEx.007951FD
$+16F    007951BF      6A 00                       PUSH 0x0
$+171    007951C1      8BCE                        MOV ECX,ESI
$+173    007951C3      E8 B8000000                 CALL PathOfEx.00795280
$+178    007951C8      8B8E B4000000               MOV ECX,DWORD PTR DS:[ESI+0xB4]
$+17E    007951CE      8B86 CC000000               MOV EAX,DWORD PTR DS:[ESI+0xCC]
$+184    007951D4      03CF                        ADD ECX,EDI
$+186    007951D6      2B86 C8000000               SUB EAX,DWORD PTR DS:[ESI+0xC8]
$+18C    007951DC      3BC8                        CMP ECX,EAX
$+18E    007951DE      76 1D                       JBE SHORT PathOfEx.007951FD
$+190    007951E0      51                          PUSH ECX
$+191    007951E1      8BCE                        MOV ECX,ESI
$+193    007951E3      E8 28060000                 CALL PathOfEx.00795810
$+198    007951E8      84C0                        TEST AL,AL
$+19A    007951EA      75 11                       JNZ SHORT PathOfEx.007951FD
$+19C    007951EC      3886 B3000000               CMP BYTE PTR DS:[ESI+0xB3],AL
$+1A2    007951F2      75 32                       JNZ SHORT PathOfEx.00795226
$+1A4    007951F4      6A 0A                       PUSH 0xA
$+1A6    007951F6      8BCE                        MOV ECX,ESI
$+1A8    007951F8      E8 83000000                 CALL PathOfEx.00795280
$+1AD    007951FD      8B86 B4000000               MOV EAX,DWORD PTR DS:[ESI+0xB4]
$+1B3    00795203      0386 C8000000               ADD EAX,DWORD PTR DS:[ESI+0xC8]
$+1B9    00795209      57                          PUSH EDI
$+1BA    0079520A      FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
$+1BD    0079520D      50                          PUSH EAX
$+1BE    0079520E      E8 BD035000                 CALL PathOfEx.00C955D0
$+1C3    00795213      83C4 0C                     ADD ESP,0xC
$+1C6    00795216      8BC6                        MOV EAX,ESI
$+1C8    00795218      01BE B4000000               ADD DWORD PTR DS:[ESI+0xB4],EDI
$+1CE    0079521E      5F                          POP EDI
$+1CF    0079521F      5E                          POP ESI
$+1D0    00795220      8BE5                        MOV ESP,EBP
$+1D2    00795222      5D                          POP EBP
$+1D3    00795223      C2 0800                     RETN 0x8
$+1D6    00795226      56                          PUSH ESI
$+1D7    00795227      8D4C24 0C                   LEA ECX,DWORD PTR SS:[ESP+0xC]
$+1DB    0079522B      E8 E0F4FFFF                 CALL PathOfEx.00794710
$+1E0    00795230      68 54F1FC00                 PUSH PathOfEx.00FCF154
$+1E5    00795235      8D4424 0C                   LEA EAX,DWORD PTR SS:[ESP+0xC]
$+1E9    00795239      50                          PUSH EAX
$+1EA    0079523A      E8 B40C5000                 CALL PathOfEx.00C95EF3
$+1EF    0079523F      CC                          INT3
$+1F0    00795240      56                          PUSH ESI
$+1F1    00795241      8B7424 08                   MOV ESI,DWORD PTR SS:[ESP+0x8]
$+1F5    00795245      0F57C0                      XORPS XMM0,XMM0
$+1F8    00795248      57                          PUSH EDI
$+1F9    00795249      8BF9                        MOV EDI,ECX
$+1FB    0079524B      8D47 04                     LEA EAX,DWORD PTR DS:[EDI+0x4]
$+1FE    0079524E      50                          PUSH EAX
$+1FF    0079524F      C707 6C3EDE00               MOV DWORD PTR DS:[EDI],PathOfEx.00DE3E6C
$+205    00795255      660FD600                    MOVQ QWORD PTR DS:[EAX],XMM0
$+209    00795259      8D46 04                     LEA EAX,DWORD PTR DS:[ESI+0x4]
$+20C    0079525C      50                          PUSH EAX
$+20D    0079525D      E8 C2FB4F00                 CALL PathOfEx.00C94E24
$+212    00795262      83C4 08                     ADD ESP,0x8
$+215    00795265      C707 3CA3ED00               MOV DWORD PTR DS:[EDI],PathOfEx.00EDA33C
$+21B    0079526B      8B46 0C                     MOV EAX,DWORD PTR DS:[ESI+0xC]
$+21E    0079526E      8947 0C                     MOV DWORD PTR DS:[EDI+0xC],EAX
$+221    00795271      8BC7                        MOV EAX,EDI
$+223    00795273      5F                          POP EDI
$+224    00795274      5E                          POP ESI
$+225    00795275      C2 0400                     RETN 0x4
$+228    00795278      CC                          INT3
*/
/**/
//tag_处理具体消息_处理序列化后的对象
/*
$-F2     008BE7DE     CC                      INT3
$-F1     008BE7DF     CC                      INT3                                              ; 收包序列完后就调用的函数
$-F0     008BE7E0     55                      PUSH EBP                                          ; 第2个参数是序列化后的对象
$-EF     008BE7E1     8BEC                    MOV EBP,ESP
$-ED     008BE7E3     6A FF                   PUSH -0x1
$-EB     008BE7E5     68 DA42D600             PUSH PathOfEx.00D642DA
$-E6     008BE7EA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-E0     008BE7F0     50                      PUSH EAX
$-DF     008BE7F1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-D8     008BE7F8     51                      PUSH ECX
$-D7     008BE7F9     56                      PUSH ESI
$-D6     008BE7FA     8BF1                    MOV ESI,ECX
$-D4     008BE7FC     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$-CD     008BE803     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
$-CA     008BE806     85C9                    TEST ECX,ECX
$-C8     008BE808     0F84 8B000000           JE PathOfEx.008BE899
$-C2     008BE80E     83BE BC010000 00        CMP DWORD PTR DS:[ESI+0x1BC],0x0
$-BB     008BE815     BA 08010000             MOV EDX,0x108
$-B6     008BE81A     0FB741 04               MOVZX EAX,WORD PTR DS:[ECX+0x4]
$-B2     008BE81E     75 10                   JNZ SHORT PathOfEx.008BE830
$-B0     008BE820     66:3BC2                 CMP AX,DX
$-AD     008BE823     74 74                   JE SHORT PathOfEx.008BE899
$-AB     008BE825     51                      PUSH ECX
$-AA     008BE826     8D4E D4                 LEA ECX,DWORD PTR DS:[ESI-0x2C]
$-A7     008BE829     E8 A2000000             CALL PathOfEx.008BE8D0
$-A2     008BE82E     EB 66                   JMP SHORT PathOfEx.008BE896
$-A0     008BE830     66:3BC2                 CMP AX,DX
$-9D     008BE833     75 33                   JNZ SHORT PathOfEx.008BE868
$-9B     008BE835     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-94     008BE83C     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
$-91     008BE83F     C645 FC 02              MOV BYTE PTR SS:[EBP-0x4],0x2
$-8D     008BE843     50                      PUSH EAX
$-8C     008BE844     8D8E E8010000           LEA ECX,DWORD PTR DS:[ESI+0x1E8]
$-86     008BE84A     E8 01060000             CALL PathOfEx.008BEE50
$-81     008BE84F     8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$-7E     008BE852     85C9                    TEST ECX,ECX
$-7C     008BE854     74 0A                   JE SHORT PathOfEx.008BE860
$-7A     008BE856     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
$-76     008BE85A     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-74     008BE85C     6A 01                   PUSH 0x1
$-72     008BE85E     FF10                    CALL DWORD PTR DS:[EAX]
$-70     008BE860     FF86 E4010000           INC DWORD PTR DS:[ESI+0x1E4]
$-6A     008BE866     EB 2E                   JMP SHORT PathOfEx.008BE896
$-68     008BE868     C745 0C 00000000        MOV DWORD PTR SS:[EBP+0xC],0x0
$-61     008BE86F     894D F0                 MOV DWORD PTR SS:[EBP-0x10],ECX
$-5E     008BE872     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
$-5B     008BE875     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
$-57     008BE879     50                      PUSH EAX
$-56     008BE87A     8D8E E8010000           LEA ECX,DWORD PTR DS:[ESI+0x1E8]
$-50     008BE880     E8 CB050000             CALL PathOfEx.008BEE50                            ; 啥都没干
$-4B     008BE885     8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$-48     008BE888     85C9                    TEST ECX,ECX
$-46     008BE88A     74 0A                   JE SHORT PathOfEx.008BE896
$-44     008BE88C     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
$-40     008BE890     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-3E     008BE892     6A 01                   PUSH 0x1
$-3C     008BE894     FF10                    CALL DWORD PTR DS:[EAX]
$-3A     008BE896     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
$-37     008BE899     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$-30     008BE8A0     5E                      POP ESI
$-2F     008BE8A1     85C9                    TEST ECX,ECX
$-2D     008BE8A3     74 0D                   JE SHORT PathOfEx.008BE8B2
$-2B     008BE8A5     C745 FC 06000000        MOV DWORD PTR SS:[EBP-0x4],0x6
$-24     008BE8AC     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-22     008BE8AE     6A 01                   PUSH 0x1
$-20     008BE8B0     FF10                    CALL DWORD PTR DS:[EAX]
$-1E     008BE8B2     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1B     008BE8B5     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-14     008BE8BC     8BE5                    MOV ESP,EBP
$-12     008BE8BE     5D                      POP EBP
$-11     008BE8BF     C2 0800                 RETN 0x8
$-E      008BE8C2     CC                      INT3
$-D      008BE8C3     CC                      INT3
$-C      008BE8C4     CC                      INT3
$-B      008BE8C5     CC                      INT3
$-A      008BE8C6     CC                      INT3
$-9      008BE8C7     CC                      INT3
$-8      008BE8C8     CC                      INT3
$-7      008BE8C9     CC                      INT3
$-6      008BE8CA     CC                      INT3
$-5      008BE8CB     CC                      INT3
$-4      008BE8CC     CC                      INT3
$-3      008BE8CD     CC                      INT3
$-2      008BE8CE     CC                      INT3
$-1      008BE8CF     CC                      INT3
$ ==>    008BE8D0     55                      PUSH EBP                                          ; tag_处理具体消息_处理序列化后的对象
$+1      008BE8D1     8BEC                    MOV EBP,ESP                                       ; ecx=stCD_World*
$+3      008BE8D3     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+9      008BE8D9     6A FF                   PUSH -0x1
$+B      008BE8DB     68 1043D600             PUSH PathOfEx.00D64310                            ; ASCII "赴琦"
$+10     008BE8E0     50                      PUSH EAX
$+11     008BE8E1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     008BE8E8     83EC 2C                 SUB ESP,0x2C
$+1B     008BE8EB     53                      PUSH EBX
$+1C     008BE8EC     56                      PUSH ESI
$+1D     008BE8ED     57                      PUSH EDI
$+1E     008BE8EE     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
$+21     008BE8F1     8BD9                    MOV EBX,ECX
$+23     008BE8F3     0FB747 04               MOVZX EAX,WORD PTR DS:[EDI+0x4]                   ; edi=消息对象
$+27     008BE8F7     83E8 44                 SUB EAX,0x44
$+2A     008BE8FA     0F84 69030000           JE PathOfEx.008BEC69
$+30     008BE900     83E8 7C                 SUB EAX,0x7C
$+33     008BE903     0F84 0A030000           JE PathOfEx.008BEC13
$+39     008BE909     83E8 4B                 SUB EAX,0x4B
$+3C     008BE90C     0F84 01020000           JE PathOfEx.008BEB13
$+42     008BE912     FF77 08                 PUSH DWORD PTR DS:[EDI+0x8]                       ; 传入实体id,查找实体对象
$+45     008BE915     81C3 28020000           ADD EBX,0x228                                     ; g_o_map_obj_btree_
$+4B     008BE91B     8BCB                    MOV ECX,EBX
$+4D     008BE91D     E8 EE18F2FF             CALL PathOfEx.007E0210                            ; call tag_通过id得到实体对象
$+52     008BE922     8BF0                    MOV ESI,EAX
$+54     008BE924     85F6                    TEST ESI,ESI
$+56     008BE926     0F84 50030000           JE PathOfEx.008BEC7C
$+5C     008BE92C     0FB74F 04               MOVZX ECX,WORD PTR DS:[EDI+0x4]
$+60     008BE930     81C1 27FFFFFF           ADD ECX,-0xD9
$+66     008BE936     83F9 34                 CMP ECX,0x34
$+69     008BE939     0F87 3D030000           JA PathOfEx.008BEC7C
$+6F     008BE93F     0FB689 C4EC8B00         MOVZX ECX,BYTE PTR DS:[ECX+0x8BECC4]
$+76     008BE946     FF248D 90EC8B00         JMP DWORD PTR DS:[ECX*4+0x8BEC90]                 ; 通过消息id跳转
$+7D     008BE94D     8B47 14                 MOV EAX,DWORD PTR DS:[EDI+0x14]
$+80     008BE950     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+83     008BE953     8945 E4                 MOV DWORD PTR SS:[EBP-0x1C],EAX
$+86     008BE956     8B47 18                 MOV EAX,DWORD PTR DS:[EDI+0x18]
$+89     008BE959     2B47 14                 SUB EAX,DWORD PTR DS:[EDI+0x14]
$+8C     008BE95C     6A 01                   PUSH 0x1
$+8E     008BE95E     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+91     008BE961     C745 EC 00000000        MOV DWORD PTR SS:[EBP-0x14],0x0
$+98     008BE968     E8 C31DBEFF             CALL PathOfEx.004A0730
$+9D     008BE96D     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$+A0     008BE970     8D55 E4                 LEA EDX,DWORD PTR SS:[EBP-0x1C]
$+A3     008BE973     8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+A6     008BE976     52                      PUSH EDX
$+A7     008BE977     8A0401                  MOV AL,BYTE PTR DS:[ECX+EAX]
$+AA     008BE97A     41                      INC ECX
$+AB     008BE97B     894D EC                 MOV DWORD PTR SS:[EBP-0x14],ECX
$+AE     008BE97E     0FB6C8                  MOVZX ECX,AL
$+B1     008BE981     8B46 04                 MOV EAX,DWORD PTR DS:[ESI+0x4]
$+B4     008BE984     8B0C88                  MOV ECX,DWORD PTR DS:[EAX+ECX*4]
$+B7     008BE987     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+B9     008BE989     FF50 2C                 CALL DWORD PTR DS:[EAX+0x2C]
$+BC     008BE98C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+BF     008BE98F     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+C6     008BE996     5F                      POP EDI
$+C7     008BE997     5E                      POP ESI
$+C8     008BE998     5B                      POP EBX
$+C9     008BE999     8BE5                    MOV ESP,EBP
$+CB     008BE99B     5D                      POP EBP
$+CC     008BE99C     C2 0400                 RETN 0x4
$+CF     008BE99F     F646 18 01              TEST BYTE PTR DS:[ESI+0x18],0x1
$+D3     008BE9A3     0F85 D3020000           JNZ PathOfEx.008BEC7C
$+D9     008BE9A9     56                      PUSH ESI
$+DA     008BE9AA     8BCB                    MOV ECX,EBX
$+DC     008BE9AC     E8 1F24F2FF             CALL PathOfEx.007E0DD0
$+E1     008BE9B1     8BCE                    MOV ECX,ESI
$+E3     008BE9B3     E8 4889F1FF             CALL PathOfEx.007D7300
$+E8     008BE9B8     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+EB     008BE9BB     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+F2     008BE9C2     5F                      POP EDI
$+F3     008BE9C3     5E                      POP ESI
$+F4     008BE9C4     5B                      POP EBX
$+F5     008BE9C5     8BE5                    MOV ESP,EBP
$+F7     008BE9C7     5D                      POP EBP
$+F8     008BE9C8     C2 0400                 RETN 0x4
$+FB     008BE9CB     57                      PUSH EDI                                          ; 处理技能_移动等实体对象的消息,edi=消息对象
$+FC     008BE9CC     51                      PUSH ECX
$+FD     008BE9CD     8BCE                    MOV ECX,ESI                                       ; esi=stCD_EntityPlayer*
$+FF     008BE9CF     E8 1CF7BDFF             CALL PathOfEx.0049E0F0
$+104    008BE9D4     83C4 04                 ADD ESP,0x4
$+107    008BE9D7     8BC8                    MOV ECX,EAX
$+109    008BE9D9     E8 627A0000             CALL PathOfEx.008C6440                            ; call tag_处理技能_移动等实体消息
$+10E    008BE9DE     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+111    008BE9E1     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+118    008BE9E8     5F                      POP EDI
$+119    008BE9E9     5E                      POP ESI
$+11A    008BE9EA     5B                      POP EBX
$+11B    008BE9EB     8BE5                    MOV ESP,EBP
$+11D    008BE9ED     5D                      POP EBP
$+11E    008BE9EE     C2 0400                 RETN 0x4
$+121    008BE9F1     57                      PUSH EDI                                          ; stCDR_OnHpMp,0xe0
$+122    008BE9F2     51                      PUSH ECX
$+123    008BE9F3     8BCE                    MOV ECX,ESI
$+125    008BE9F5     E8 16D4BAFF             CALL PathOfEx.0046BE10
$+12A    008BE9FA     83C4 04                 ADD ESP,0x4
$+12D    008BE9FD     8BC8                    MOV ECX,EAX
$+12F    008BE9FF     E8 CC76F1FF             CALL PathOfEx.007D60D0
$+134    008BEA04     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+137    008BEA07     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+13E    008BEA0E     5F                      POP EDI
$+13F    008BEA0F     5E                      POP ESI
$+140    008BEA10     5B                      POP EBX
$+141    008BEA11     8BE5                    MOV ESP,EBP
$+143    008BEA13     5D                      POP EBP
$+144    008BEA14     C2 0400                 RETN 0x4
$+147    008BEA17     57                      PUSH EDI
$+148    008BEA18     51                      PUSH ECX
$+149    008BEA19     8BCE                    MOV ECX,ESI
$+14B    008BEA1B     E8 50D4BAFF             CALL PathOfEx.0046BE70
$+150    008BEA20     83C4 04                 ADD ESP,0x4
$+153    008BEA23     8BC8                    MOV ECX,EAX
$+155    008BEA25     E8 56D80200             CALL PathOfEx.008EC280
$+15A    008BEA2A     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+15D    008BEA2D     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+164    008BEA34     5F                      POP EDI
$+165    008BEA35     5E                      POP ESI
$+166    008BEA36     5B                      POP EBX
$+167    008BEA37     8BE5                    MOV ESP,EBP
$+169    008BEA39     5D                      POP EBP
$+16A    008BEA3A     C2 0400                 RETN 0x4
$+16D    008BEA3D     8B4E 20                 MOV ECX,DWORD PTR DS:[ESI+0x20]
$+170    008BEA40     57                      PUSH EDI
$+171    008BEA41     E8 EA9EF1FF             CALL PathOfEx.007D8930
$+176    008BEA46     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+179    008BEA49     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+180    008BEA50     5F                      POP EDI
$+181    008BEA51     5E                      POP ESI
$+182    008BEA52     5B                      POP EBX
$+183    008BEA53     8BE5                    MOV ESP,EBP
$+185    008BEA55     5D                      POP EBP
$+186    008BEA56     C2 0400                 RETN 0x4
$+189    008BEA59     57                      PUSH EDI
$+18A    008BEA5A     51                      PUSH ECX
$+18B    008BEA5B     8BCE                    MOV ECX,ESI
$+18D    008BEA5D     E8 9E00BEFF             CALL PathOfEx.0049EB00
$+192    008BEA62     83C4 04                 ADD ESP,0x4
$+195    008BEA65     8BC8                    MOV ECX,EAX
$+197    008BEA67     E8 244EF3FF             CALL PathOfEx.007F3890
$+19C    008BEA6C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+19F    008BEA6F     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1A6    008BEA76     5F                      POP EDI
$+1A7    008BEA77     5E                      POP ESI
$+1A8    008BEA78     5B                      POP EBX
$+1A9    008BEA79     8BE5                    MOV ESP,EBP
$+1AB    008BEA7B     5D                      POP EBP
$+1AC    008BEA7C     C2 0400                 RETN 0x4
$+1AF    008BEA7F     57                      PUSH EDI
$+1B0    008BEA80     51                      PUSH ECX
$+1B1    008BEA81     8BCE                    MOV ECX,ESI
$+1B3    008BEA83     E8 A8FDFBFF             CALL PathOfEx.0087E830
$+1B8    008BEA88     83C4 04                 ADD ESP,0x4
$+1BB    008BEA8B     8BC8                    MOV ECX,EAX
$+1BD    008BEA8D     E8 FE90FCFF             CALL PathOfEx.00887B90
$+1C2    008BEA92     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1C5    008BEA95     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1CC    008BEA9C     5F                      POP EDI
$+1CD    008BEA9D     5E                      POP ESI
$+1CE    008BEA9E     5B                      POP EBX
$+1CF    008BEA9F     8BE5                    MOV ESP,EBP
$+1D1    008BEAA1     5D                      POP EBP
$+1D2    008BEAA2     C2 0400                 RETN 0x4
$+1D5    008BEAA5     57                      PUSH EDI
$+1D6    008BEAA6     51                      PUSH ECX
$+1D7    008BEAA7     8BCE                    MOV ECX,ESI
$+1D9    008BEAA9     E8 E2070000             CALL PathOfEx.008BF290
$+1DE    008BEAAE     83C4 04                 ADD ESP,0x4
$+1E1    008BEAB1     8BC8                    MOV ECX,EAX
$+1E3    008BEAB3     E8 A880FCFF             CALL PathOfEx.00886B60
$+1E8    008BEAB8     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1EB    008BEABB     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1F2    008BEAC2     5F                      POP EDI
$+1F3    008BEAC3     5E                      POP ESI
$+1F4    008BEAC4     5B                      POP EBX
$+1F5    008BEAC5     8BE5                    MOV ESP,EBP
$+1F7    008BEAC7     5D                      POP EBP
$+1F8    008BEAC8     C2 0400                 RETN 0x4
$+1FB    008BEACB     8BCE                    MOV ECX,ESI
$+1FD    008BEACD     E8 6E78DAFF             CALL PathOfEx.00666340
$+202    008BEAD2     57                      PUSH EDI
$+203    008BEAD3     8BC8                    MOV ECX,EAX
$+205    008BEAD5     8B10                    MOV EDX,DWORD PTR DS:[EAX]
$+207    008BEAD7     FF52 34                 CALL DWORD PTR DS:[EDX+0x34]
$+20A    008BEADA     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+20D    008BEADD     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+214    008BEAE4     5F                      POP EDI
$+215    008BEAE5     5E                      POP ESI
$+216    008BEAE6     5B                      POP EBX
$+217    008BEAE7     8BE5                    MOV ESP,EBP
$+219    008BEAE9     5D                      POP EBP
$+21A    008BEAEA     C2 0400                 RETN 0x4
$+21D    008BEAED     57                      PUSH EDI
$+21E    008BEAEE     51                      PUSH ECX
$+21F    008BEAEF     8BCE                    MOV ECX,ESI
$+221    008BEAF1     E8 FA070000             CALL PathOfEx.008BF2F0
$+226    008BEAF6     83C4 04                 ADD ESP,0x4
$+229    008BEAF9     8BC8                    MOV ECX,EAX
$+22B    008BEAFB     E8 807EF3FF             CALL PathOfEx.007F6980
$+230    008BEB00     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+233    008BEB03     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+23A    008BEB0A     5F                      POP EDI
$+23B    008BEB0B     5E                      POP ESI
$+23C    008BEB0C     5B                      POP EBX
$+23D    008BEB0D     8BE5                    MOV ESP,EBP
$+23F    008BEB0F     5D                      POP EBP
$+240    008BEB10     C2 0400                 RETN 0x4
$+243    008BEB13     8B8B B0010000           MOV ECX,DWORD PTR DS:[EBX+0x1B0]
$+249    008BEB19     85C9                    TEST ECX,ECX
$+24B    008BEB1B     74 05                   JE SHORT PathOfEx.008BEB22
$+24D    008BEB1D     E8 9EAAE5FF             CALL PathOfEx.007195C0
$+252    008BEB22     8B83 B0010000           MOV EAX,DWORD PTR DS:[EBX+0x1B0]
$+258    008BEB28     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
$+25B    008BEB2B     FF77 14                 PUSH DWORD PTR DS:[EDI+0x14]
$+25E    008BEB2E     51                      PUSH ECX
$+25F    008BEB2F     8B48 30                 MOV ECX,DWORD PTR DS:[EAX+0x30]
$+262    008BEB32     E8 E92DF4FF             CALL PathOfEx.00801920
$+267    008BEB37     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+26E    008BEB3E     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
$+271    008BEB41     85F6                    TEST ESI,ESI
$+273    008BEB43     75 3F                   JNZ SHORT PathOfEx.008BEB84
$+275    008BEB45     8D45 DC                 LEA EAX,DWORD PTR SS:[EBP-0x24]
$+278    008BEB48     C745 D8 6C3EDE00        MOV DWORD PTR SS:[EBP-0x28],PathOfEx.00DE3E6C
$+27F    008BEB4F     50                      PUSH EAX
$+280    008BEB50     8D45 E8                 LEA EAX,DWORD PTR SS:[EBP-0x18]
$+283    008BEB53     C745 E8 E48BEF00        MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00EF8BE4     ; ASCII "Unknown object type serialized by server"
$+28A    008BEB5A     0F57C0                  XORPS XMM0,XMM0
$+28D    008BEB5D     C645 EC 01              MOV BYTE PTR SS:[EBP-0x14],0x1
$+291    008BEB61     50                      PUSH EAX
$+292    008BEB62     660FD645 DC             MOVQ QWORD PTR SS:[EBP-0x24],XMM0
$+297    008BEB67     E8 B8623D00             CALL PathOfEx.00C94E24
$+29C    008BEB6C     83C4 08                 ADD ESP,0x8
$+29F    008BEB6F     C745 D8 C43EDE00        MOV DWORD PTR SS:[EBP-0x28],PathOfEx.00DE3EC4
$+2A6    008BEB76     8D45 D8                 LEA EAX,DWORD PTR SS:[EBP-0x28]
$+2A9    008BEB79     68 A8ECFC00             PUSH PathOfEx.00FCECA8
$+2AE    008BEB7E     50                      PUSH EAX
$+2AF    008BEB7F     E8 6F733D00             CALL PathOfEx.00C95EF3
$+2B4    008BEB84     8B4F 1C                 MOV ECX,DWORD PTR DS:[EDI+0x1C]
$+2B7    008BEB87     2B4F 18                 SUB ECX,DWORD PTR DS:[EDI+0x18]
$+2BA    008BEB8A     8B47 18                 MOV EAX,DWORD PTR DS:[EDI+0x18]
$+2BD    008BEB8D     68 B8010000             PUSH 0x1B8
$+2C2    008BEB92     8945 CC                 MOV DWORD PTR SS:[EBP-0x34],EAX
$+2C5    008BEB95     894D D0                 MOV DWORD PTR SS:[EBP-0x30],ECX
$+2C8    008BEB98     C745 D4 00000000        MOV DWORD PTR SS:[EBP-0x2C],0x0
$+2CF    008BEB9F     E8 F2D73B00             CALL PathOfEx.00C7C396
$+2D4    008BEBA4     83C4 04                 ADD ESP,0x4
$+2D7    008BEBA7     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+2DA    008BEBAA     53                      PUSH EBX
$+2DB    008BEBAB     8D4D CC                 LEA ECX,DWORD PTR SS:[EBP-0x34]
$+2DE    008BEBAE     C645 FC 01              MOV BYTE PTR SS:[EBP-0x4],0x1
$+2E2    008BEBB2     51                      PUSH ECX
$+2E3    008BEBB3     FF77 08                 PUSH DWORD PTR DS:[EDI+0x8]
$+2E6    008BEBB6     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
$+2E9    008BEBB9     51                      PUSH ECX
$+2EA    008BEBBA     8BC8                    MOV ECX,EAX
$+2EC    008BEBBC     E8 2F80F1FF             CALL PathOfEx.007D6BF0
$+2F1    008BEBC1     50                      PUSH EAX
$+2F2    008BEBC2     8D8B 28020000           LEA ECX,DWORD PTR DS:[EBX+0x228]
$+2F8    008BEBC8     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+2FB    008BEBCB     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
$+2FF    008BEBCF     E8 9C21F2FF             CALL PathOfEx.007E0D70
$+304    008BEBD4     8B4F 08                 MOV ECX,DWORD PTR DS:[EDI+0x8]
$+307    008BEBD7     3B8B D4010000           CMP ECX,DWORD PTR DS:[EBX+0x1D4]
$+30D    008BEBDD     75 09                   JNZ SHORT PathOfEx.008BEBE8
$+30F    008BEBDF     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+312    008BEBE2     8983 C4010000           MOV DWORD PTR DS:[EBX+0x1C4],EAX
$+318    008BEBE8     56                      PUSH ESI
$+319    008BEBE9     51                      PUSH ECX
$+31A    008BEBEA     C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4],0x2
$+321    008BEBF1     E8 0A94E5FF             CALL PathOfEx.00718000
$+326    008BEBF6     83C4 04                 ADD ESP,0x4
$+329    008BEBF9     8BC8                    MOV ECX,EAX
$+32B    008BEBFB     E8 908FE5FF             CALL PathOfEx.00717B90
$+330    008BEC00     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+333    008BEC03     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+33A    008BEC0A     5F                      POP EDI
$+33B    008BEC0B     5E                      POP ESI
$+33C    008BEC0C     5B                      POP EBX
$+33D    008BEC0D     8BE5                    MOV ESP,EBP
$+33F    008BEC0F     5D                      POP EBP
$+340    008BEC10     C2 0400                 RETN 0x4
$+343    008BEC13     8B83 F8010000           MOV EAX,DWORD PTR DS:[EBX+0x1F8]
$+349    008BEC19     8D8B F8010000           LEA ECX,DWORD PTR DS:[EBX+0x1F8]
$+34F    008BEC1F     3B41 04                 CMP EAX,DWORD PTR DS:[ECX+0x4]
$+352    008BEC22     74 58                   JE SHORT PathOfEx.008BEC7C
$+354    008BEC24     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+356    008BEC26     51                      PUSH ECX
$+357    008BEC27     8BCB                    MOV ECX,EBX
$+359    008BEC29     FF50 0C                 CALL DWORD PTR DS:[EAX+0xC]
$+35C    008BEC2C     8BC8                    MOV ECX,EAX
$+35E    008BEC2E     E8 7DEFF1FF             CALL PathOfEx.007DDBB0
$+363    008BEC33     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+365    008BEC35     8BCB                    MOV ECX,EBX
$+367    008BEC37     FF50 0C                 CALL DWORD PTR DS:[EAX+0xC]
$+36A    008BEC3A     8BF8                    MOV EDI,EAX
$+36C    008BEC3C     8B4F 04                 MOV ECX,DWORD PTR DS:[EDI+0x4]
$+36F    008BEC3F     8B11                    MOV EDX,DWORD PTR DS:[ECX]
$+371    008BEC41     FF52 14                 CALL DWORD PTR DS:[EDX+0x14]
$+374    008BEC44     8BF0                    MOV ESI,EAX
$+376    008BEC46     57                      PUSH EDI
$+377    008BEC47     8BCE                    MOV ECX,ESI
$+379    008BEC49     E8 2202F2FF             CALL PathOfEx.007DEE70
$+37E    008BEC4E     57                      PUSH EDI
$+37F    008BEC4F     8BCE                    MOV ECX,ESI
$+381    008BEC51     E8 6A02F2FF             CALL PathOfEx.007DEEC0
$+386    008BEC56     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+389    008BEC59     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+390    008BEC60     5F                      POP EDI
$+391    008BEC61     5E                      POP ESI
$+392    008BEC62     5B                      POP EBX
$+393    008BEC63     8BE5                    MOV ESP,EBP
$+395    008BEC65     5D                      POP EBP
$+396    008BEC66     C2 0400                 RETN 0x4
$+399    008BEC69     0FB647 14               MOVZX EAX,BYTE PTR DS:[EDI+0x14]
$+39D    008BEC6D     8B8B C0010000           MOV ECX,DWORD PTR DS:[EBX+0x1C0]
$+3A3    008BEC73     50                      PUSH EAX
$+3A4    008BEC74     FF77 08                 PUSH DWORD PTR DS:[EDI+0x8]
$+3A7    008BEC77     E8 A40B0100             CALL PathOfEx.008CF820
$+3AC    008BEC7C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+3AF    008BEC7F     5F                      POP EDI
$+3B0    008BEC80     5E                      POP ESI
$+3B1    008BEC81     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+3B8    008BEC88     5B                      POP EBX
$+3B9    008BEC89     8BE5                    MOV ESP,EBP
$+3BB    008BEC8B     5D                      POP EBP
$+3BC    008BEC8C     C2 0400                 RETN 0x4
$+3BF    008BEC8F     90                      NOP
$+3C0    008BEC90     A9 E98B00CB             TEST EAX,0xCB008BE9
$+3C5    008BEC95   - E9 8B00F1E9             JMP EA7CED25
$+3CA    008BEC9A     8B00                    MOV EAX,DWORD PTR DS:[EAX]
$+3CC    008BEC9C     17                      POP SS
$+3CD    008BEC9D     EA 8B003DEA 8B00        JMP FAR 008B:0EA3D008B
$+3D4    008BECA4     59                      POP ECX
$+3D5    008BECA5     EA 8B007FEA 8B00        JMP FAR 008B:0EA7F008B
$+3DC    008BECAC     A5                      MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
$+3DD    008BECAD     EA 8B00CBEA 8B00        JMP FAR 008B:0EACB008B
$+3E4    008BECB4     ED                      IN EAX,DX
$+3E5    008BECB5     EA 8B004DE9 8B00        JMP FAR 008B:0E94D008B
$+3EC    008BECBC     9F                      LAHF
$+3ED    008BECBD   - E9 8B007CEC             JMP ED07ED4D
$+3F2    008BECC2     8B00                    MOV EAX,DWORD PTR DS:[EAX]
$+3F4    008BECC4     0001                    ADD BYTE PTR DS:[ECX],AL
$+3F6    008BECC6     0101                    ADD DWORD PTR DS:[ECX],EAX
$+3F8    008BECC8     0101                    ADD DWORD PTR DS:[ECX],EAX
$+3FA    008BECCA     0102                    ADD DWORD PTR DS:[EDX],EAX
$+3FC    008BECCC     0301                    ADD EAX,DWORD PTR DS:[ECX]
$+3FE    008BECCE     04 04                   ADD AL,0x4
$+400    008BECD0     04 05                   ADD AL,0x5
$+402    008BECD2     04 02                   ADD AL,0x2
$+404    008BECD4     0206                    ADD AL,BYTE PTR DS:[ESI]
$+406    008BECD6     06                      PUSH ES
$+407    008BECD7     06                      PUSH ES
$+408    008BECD8     07                      POP ES
$+409    008BECD9     0C 0C                   OR AL,0xC
$+40B    008BECDB     0C 0C                   OR AL,0xC
$+40D    008BECDD     0C 08                   OR AL,0x8
$+40F    008BECDF     0101                    ADD DWORD PTR DS:[ECX],EAX
$+411    008BECE1     0C 09                   OR AL,0x9
$+413    008BECE3     0C 0C                   OR AL,0xC
$+415    008BECE5     0C 0C                   OR AL,0xC
$+417    008BECE7     0C 0C                   OR AL,0xC
$+419    008BECE9     0C 0C                   OR AL,0xC
$+41B    008BECEB     0C 0C                   OR AL,0xC
$+41D    008BECED     0C 0C                   OR AL,0xC
$+41F    008BECEF     0C 0C                   OR AL,0xC
$+421    008BECF1     0C 0C                   OR AL,0xC
$+423    008BECF3     0C 0C                   OR AL,0xC
$+425    008BECF5     0C 0C                   OR AL,0xC
$+427    008BECF7     0A0B                    OR CL,BYTE PTR DS:[EBX]
$+429    008BECF9     CC                      INT3
$+42A    008BECFA     CC                      INT3
$+42B    008BECFB     CC                      INT3
$+42C    008BECFC     CC                      INT3
$+42D    008BECFD     CC                      INT3
$+42E    008BECFE     CC                      INT3
$+42F    008BECFF     CC                      INT3
$+430    008BED00     83EC 08                 SUB ESP,0x8
$+433    008BED03     56                      PUSH ESI
$+434    008BED04     57                      PUSH EDI
$+435    008BED05     8B7C24 14               MOV EDI,DWORD PTR SS:[ESP+0x14]
$+439    008BED09     8B0F                    MOV ECX,DWORD PTR DS:[EDI]
$+43B    008BED0B     85C9                    TEST ECX,ECX
$+43D    008BED0D     74 05                   JE SHORT PathOfEx.008BED14
$+43F    008BED0F     E8 ACA8E5FF             CALL PathOfEx.007195C0
$+444    008BED14     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+446    008BED16     C74424 14 94B4EE00      MOV DWORD PTR SS:[ESP+0x14],PathOfEx.00EEB494     ; ASCII "Player"
$+44E    008BED1E     8B48 30                 MOV ECX,DWORD PTR DS:[EAX+0x30]
$+451    008BED21     8D4424 14               LEA EAX,DWORD PTR SS:[ESP+0x14]
$+455    008BED25     50                      PUSH EAX
$+456    008BED26     8D4424 10               LEA EAX,DWORD PTR SS:[ESP+0x10]
$+45A    008BED2A     50                      PUSH EAX
$+45B    008BED2B     8B71 18                 MOV ESI,DWORD PTR DS:[ECX+0x18]
$+45E    008BED2E     83C1 14                 ADD ECX,0x14
$+461    008BED31     E8 0AD3BAFF             CALL PathOfEx.0046C040
$+466    008BED36     397424 0C               CMP DWORD PTR SS:[ESP+0xC],ESI
$+46A    008BED3A     75 08                   JNZ SHORT PathOfEx.008BED44
$+46C    008BED3C     32C0                    XOR AL,AL
$+46E    008BED3E     5F                      POP EDI
$+46F    008BED3F     5E                      POP ESI
$+470    008BED40     83C4 08                 ADD ESP,0x8
$+473    008BED43     C3                      RETN
$+474    008BED44     8B47 10                 MOV EAX,DWORD PTR DS:[EDI+0x10]
$+477    008BED47     39B8 C4010000           CMP DWORD PTR DS:[EAX+0x1C4],EDI
$+47D    008BED4D     5F                      POP EDI
$+47E    008BED4E     0F94C0                  SETE AL
$+481    008BED51     5E                      POP ESI
$+482    008BED52     83C4 08                 ADD ESP,0x8
$+485    008BED55     C3                      RETN
$+486    008BED56     CC                      INT3
*/
/**/
//tag_处理技能_移动等实体消息
/*
$-2      008C643E     CC                      INT3
$-1      008C643F     CC                      INT3
$ ==>    008C6440     55                      PUSH EBP                                          ; tag_处理技能_移动等实体消息
$+1      008C6441     8BEC                    MOV EBP,ESP
$+3      008C6443     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+9      008C6449     6A FF                   PUSH -0x1
$+B      008C644B     68 D544D600             PUSH PathOfEx.00D644D5
$+10     008C6450     50                      PUSH EAX
$+11     008C6451     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     008C6458     81EC 44020000           SUB ESP,0x244
$+1E     008C645E     53                      PUSH EBX
$+1F     008C645F     56                      PUSH ESI
$+20     008C6460     57                      PUSH EDI
$+21     008C6461     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
$+24     008C6464     8BD9                    MOV EBX,ECX
$+26     008C6466     0FB747 04               MOVZX EAX,WORD PTR DS:[EDI+0x4]                   ; edi=消息对象
$+2A     008C646A     05 26FFFFFF             ADD EAX,-0xDA
$+2F     008C646F     83F8 1B                 CMP EAX,0x1B
$+32     008C6472     0F87 B4060000           JA PathOfEx.008C6B2C
$+38     008C6478     0FB680 686B8C00         MOVZX EAX,BYTE PTR DS:[EAX+0x8C6B68]
$+3F     008C647F     FF2485 406B8C00         JMP DWORD PTR DS:[EAX*4+0x8C6B40]
$+46     008C6486     66:837F 2A 00           CMP WORD PTR DS:[EDI+0x2A],0x0                    ; stCDR_OnMove
$+4B     008C648B     7C 47                   JL SHORT PathOfEx.008C64D4
$+4D     008C648D     8A43 38                 MOV AL,BYTE PTR DS:[EBX+0x38]
$+50     008C6490     84C0                    TEST AL,AL
$+52     008C6492     74 40                   JE SHORT PathOfEx.008C64D4
$+54     008C6494     8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+57     008C6497     83C7 20                 ADD EDI,0x20
$+5A     008C649A     8B48 20                 MOV ECX,DWORD PTR DS:[EAX+0x20]
$+5D     008C649D     8B17                    MOV EDX,DWORD PTR DS:[EDI]
$+5F     008C649F     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$+62     008C64A2     2B41 14                 SUB EAX,DWORD PTR DS:[ECX+0x14]
$+65     008C64A5     2B51 10                 SUB EDX,DWORD PTR DS:[ECX+0x10]
$+68     008C64A8     0FAFC0                  IMUL EAX,EAX
$+6B     008C64AB     0FAFD2                  IMUL EDX,EDX
$+6E     008C64AE     03C2                    ADD EAX,EDX
$+70     008C64B0     3D 10210000             CMP EAX,0x2110
$+75     008C64B5     0F82 71060000           JB PathOfEx.008C6B2C
$+7B     008C64BB     57                      PUSH EDI
$+7C     008C64BC     E8 9F25F1FF             CALL PathOfEx.007D8A60
$+81     008C64C1     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+84     008C64C4     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+8B     008C64CB     5F                      POP EDI
$+8C     008C64CC     5E                      POP ESI
$+8D     008C64CD     5B                      POP EBX
$+8E     008C64CE     8BE5                    MOV ESP,EBP
$+90     008C64D0     5D                      POP EBP
$+91     008C64D1     C2 0400                 RETN 0x4
$+94     008C64D4     8D43 38                 LEA EAX,DWORD PTR DS:[EBX+0x38]                   ; stCDR_OnMove
$+97     008C64D7     50                      PUSH EAX
$+98     008C64D8     0FB747 28               MOVZX EAX,WORD PTR DS:[EDI+0x28]
$+9C     008C64DC     8D8B 30020000           LEA ECX,DWORD PTR DS:[EBX+0x230]
$+A2     008C64E2     50                      PUSH EAX
$+A3     008C64E3     8D45 08                 LEA EAX,DWORD PTR SS:[EBP+0x8]
$+A6     008C64E6     50                      PUSH EAX
$+A7     008C64E7     E8 34CDF4FF             CALL PathOfEx.00813220
$+AC     008C64EC     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+B3     008C64F3     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
$+B6     008C64F6     85F6                    TEST ESI,ESI
$+B8     008C64F8     74 69                   JE SHORT PathOfEx.008C6563
$+BA     008C64FA     0FB747 2A               MOVZX EAX,WORD PTR DS:[EDI+0x2A]
$+BE     008C64FE     8BCE                    MOV ECX,ESI
$+C0     008C6500     50                      PUSH EAX
$+C1     008C6501     E8 6A0B2200             CALL PathOfEx.00AE7070
$+C6     008C6506     8B47 18                 MOV EAX,DWORD PTR DS:[EDI+0x18]
$+C9     008C6509     8946 20                 MOV DWORD PTR DS:[ESI+0x20],EAX
$+CC     008C650C     8B47 1C                 MOV EAX,DWORD PTR DS:[EDI+0x1C]
$+CF     008C650F     8946 24                 MOV DWORD PTR DS:[ESI+0x24],EAX
$+D2     008C6512     8A47 2C                 MOV AL,BYTE PTR DS:[EDI+0x2C]
$+D5     008C6515     8846 17                 MOV BYTE PTR DS:[ESI+0x17],AL
$+D8     008C6518     8B47 14                 MOV EAX,DWORD PTR DS:[EDI+0x14]
$+DB     008C651B     85C0                    TEST EAX,EAX
$+DD     008C651D     74 31                   JE SHORT PathOfEx.008C6550
$+DF     008C651F     3947 08                 CMP DWORD PTR DS:[EDI+0x8],EAX
$+E2     008C6522     75 05                   JNZ SHORT PathOfEx.008C6529
$+E4     008C6524     FF73 04                 PUSH DWORD PTR DS:[EBX+0x4]
$+E7     008C6527     EB 0A                   JMP SHORT PathOfEx.008C6533
$+E9     008C6529     8B4B 04                 MOV ECX,DWORD PTR DS:[EBX+0x4]
$+EC     008C652C     50                      PUSH EAX
$+ED     008C652D     E8 1E0CF1FF             CALL PathOfEx.007D7150
$+F2     008C6532     50                      PUSH EAX
$+F3     008C6533     8BCE                    MOV ECX,ESI
$+F5     008C6535     E8 560B2200             CALL PathOfEx.00AE7090
$+FA     008C653A     8B4E 1C                 MOV ECX,DWORD PTR DS:[ESI+0x1C]
$+FD     008C653D     85C9                    TEST ECX,ECX
$+FF     008C653F     74 0F                   JE SHORT PathOfEx.008C6550
$+101    008C6541     8B49 20                 MOV ECX,DWORD PTR DS:[ECX+0x20]
$+104    008C6544     8B41 10                 MOV EAX,DWORD PTR DS:[ECX+0x10]
$+107    008C6547     8946 20                 MOV DWORD PTR DS:[ESI+0x20],EAX
$+10A    008C654A     8B41 14                 MOV EAX,DWORD PTR DS:[ECX+0x14]
$+10D    008C654D     8946 24                 MOV DWORD PTR DS:[ESI+0x24],EAX
$+110    008C6550     8D47 20                 LEA EAX,DWORD PTR DS:[EDI+0x20]
$+113    008C6553     8BCE                    MOV ECX,ESI
$+115    008C6555     50                      PUSH EAX
$+116    008C6556     33F6                    XOR ESI,ESI
$+118    008C6558     51                      PUSH ECX
$+119    008C6559     8BCB                    MOV ECX,EBX
$+11B    008C655B     8975 08                 MOV DWORD PTR SS:[EBP+0x8],ESI
$+11E    008C655E     E8 6D1CF2FF             CALL PathOfEx.007E81D0
$+123    008C6563     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$+12A    008C656A     85F6                    TEST ESI,ESI
$+12C    008C656C     0F84 BA050000           JE PathOfEx.008C6B2C
$+132    008C6572     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+139    008C6579     8BCE                    MOV ECX,ESI
$+13B    008C657B     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+13D    008C657D     6A 01                   PUSH 0x1
$+13F    008C657F     FF10                    CALL DWORD PTR DS:[EAX]
$+141    008C6581     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+144    008C6584     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+14B    008C658B     5F                      POP EDI
$+14C    008C658C     5E                      POP ESI
$+14D    008C658D     5B                      POP EBX
$+14E    008C658E     8BE5                    MOV ESP,EBP
$+150    008C6590     5D                      POP EBP
$+151    008C6591     C2 0400                 RETN 0x4
$+154    008C6594     0FB783 80000000         MOVZX EAX,WORD PTR DS:[EBX+0x80]                  ; 处理stCDR_UseSkillEffectAtPos消息对象
$+15B    008C659B     66:85C0                 TEST AX,AX
$+15E    008C659E     74 08                   JE SHORT PathOfEx.008C65A8
$+160    008C65A0     48                      DEC EAX
$+161    008C65A1     66:8983 80000000        MOV WORD PTR DS:[EBX+0x80],AX
$+168    008C65A8     8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+16B    008C65AB     8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$+16E    008C65AE     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+170    008C65B0     FF50 20                 CALL DWORD PTR DS:[EAX+0x20]
$+173    008C65B3     83F8 01                 CMP EAX,0x1
$+176    008C65B6     0F85 70050000           JNZ PathOfEx.008C6B2C
$+17C    008C65BC     837B 34 00              CMP DWORD PTR DS:[EBX+0x34],0x0
$+180    008C65C0     74 28                   JE SHORT PathOfEx.008C65EA
$+182    008C65C2     834B 70 20              OR DWORD PTR DS:[EBX+0x70],0x20
$+186    008C65C6     8BCB                    MOV ECX,EBX
$+188    008C65C8     E8 0329F2FF             CALL PathOfEx.007E8ED0
$+18D    008C65CD     C783 A8000000 00000000  MOV DWORD PTR DS:[EBX+0xA8],0x0
$+197    008C65D7     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+19A    008C65DA     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1A1    008C65E1     5F                      POP EDI
$+1A2    008C65E2     5E                      POP ESI
$+1A3    008C65E3     5B                      POP EBX
$+1A4    008C65E4     8BE5                    MOV ESP,EBP
$+1A6    008C65E6     5D                      POP EBP
$+1A7    008C65E7     C2 0400                 RETN 0x4
$+1AA    008C65EA     834B 70 10              OR DWORD PTR DS:[EBX+0x70],0x10
$+1AE    008C65EE     837B 30 00              CMP DWORD PTR DS:[EBX+0x30],0x0
$+1B2    008C65F2     0F84 34050000           JE PathOfEx.008C6B2C
$+1B8    008C65F8     F643 70 82              TEST BYTE PTR DS:[EBX+0x70],0x82
$+1BC    008C65FC     0F85 2A050000           JNZ PathOfEx.008C6B2C
$+1C2    008C6602     8BCB                    MOV ECX,EBX
$+1C4    008C6604     E8 1722F2FF             CALL PathOfEx.007E8820
$+1C9    008C6609     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1CC    008C660C     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1D3    008C6613     5F                      POP EDI
$+1D4    008C6614     5E                      POP ESI
$+1D5    008C6615     5B                      POP EBX
$+1D6    008C6616     8BE5                    MOV ESP,EBP
$+1D8    008C6618     5D                      POP EBP
$+1D9    008C6619     C2 0400                 RETN 0x4
$+1DC    008C661C     66:837F 14 00           CMP WORD PTR DS:[EDI+0x14],0x0
$+1E1    008C6621     7C 0A                   JL SHORT PathOfEx.008C662D
$+1E3    008C6623     807B 38 00              CMP BYTE PTR DS:[EBX+0x38],0x0
$+1E7    008C6627     0F85 FF040000           JNZ PathOfEx.008C6B2C
$+1ED    008C662D     E8 6E4DF2FF             CALL PathOfEx.007EB3A0
$+1F2    008C6632     84C0                    TEST AL,AL
$+1F4    008C6634     0F84 F2040000           JE PathOfEx.008C6B2C
$+1FA    008C663A     8B4B 30                 MOV ECX,DWORD PTR DS:[EBX+0x30]
$+1FD    008C663D     E8 7E032200             CALL PathOfEx.00AE69C0
$+202    008C6642     66:3B47 14              CMP AX,WORD PTR DS:[EDI+0x14]
$+206    008C6646     0F85 E0040000           JNZ PathOfEx.008C6B2C
$+20C    008C664C     FF77 1C                 PUSH DWORD PTR DS:[EDI+0x1C]
$+20F    008C664F     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+211    008C6651     8BCB                    MOV ECX,EBX
$+213    008C6653     FF77 18                 PUSH DWORD PTR DS:[EDI+0x18]
$+216    008C6656     FF50 4C                 CALL DWORD PTR DS:[EAX+0x4C]
$+219    008C6659     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+21C    008C665C     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+223    008C6663     5F                      POP EDI
$+224    008C6664     5E                      POP ESI
$+225    008C6665     5B                      POP EBX
$+226    008C6666     8BE5                    MOV ESP,EBP
$+228    008C6668     5D                      POP EBP
$+229    008C6669     C2 0400                 RETN 0x4
$+22C    008C666C     807F 16 00              CMP BYTE PTR DS:[EDI+0x16],0x0
$+230    008C6670     75 12                   JNZ SHORT PathOfEx.008C6684
$+232    008C6672     66:837F 14 00           CMP WORD PTR DS:[EDI+0x14],0x0
$+237    008C6677     7C 0B                   JL SHORT PathOfEx.008C6684
$+239    008C6679     8A43 38                 MOV AL,BYTE PTR DS:[EBX+0x38]
$+23C    008C667C     84C0                    TEST AL,AL
$+23E    008C667E     0F85 A8040000           JNZ PathOfEx.008C6B2C
$+244    008C6684     E8 474DF2FF             CALL PathOfEx.007EB3D0
$+249    008C6689     84C0                    TEST AL,AL
$+24B    008C668B     0F84 9B040000           JE PathOfEx.008C6B2C
$+251    008C6691     8B4B 30                 MOV ECX,DWORD PTR DS:[EBX+0x30]
$+254    008C6694     E8 27032200             CALL PathOfEx.00AE69C0
$+259    008C6699     66:3B47 14              CMP AX,WORD PTR DS:[EDI+0x14]
$+25D    008C669D     0F84 8F000000           JE PathOfEx.008C6732
$+263    008C66A3     83EC 08                 SUB ESP,0x8
$+266    008C66A6     8D8D 64FEFFFF           LEA ECX,DWORD PTR SS:[EBP-0x19C]
$+26C    008C66AC     E8 DF30BBFF             CALL PathOfEx.00479790
$+271    008C66B1     8D85 74FEFFFF           LEA EAX,DWORD PTR SS:[EBP-0x18C]
$+277    008C66B7     C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4],0x2
$+27E    008C66BE     68 588DEF00             PUSH PathOfEx.00EF8D58                            ; ASCII "InstanceClientFinishChannellingAction id did not match current channelled action"
$+283    008C66C3     50                      PUSH EAX
$+284    008C66C4     E8 77A2C6FF             CALL PathOfEx.00530940
$+289    008C66C9     83C4 08                 ADD ESP,0x8
$+28C    008C66CC     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
$+28F    008C66CF     8D8D 64FEFFFF           LEA ECX,DWORD PTR SS:[EBP-0x19C]
$+295    008C66D5     50                      PUSH EAX
$+296    008C66D6     E8 151DBCFF             CALL PathOfEx.004883F0
$+29B    008C66DB     8BF0                    MOV ESI,EAX
$+29D    008C66DD     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
$+2A1    008C66E1     FF15 C8E0D800           CALL DWORD PTR DS:[0xD8E0C8]                      ; kernel32.GetTickCount
$+2A7    008C66E7     8BF8                    MOV EDI,EAX
$+2A9    008C66E9     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+2AC    008C66EC     50                      PUSH EAX
$+2AD    008C66ED     E8 25433E00             CALL PathOfEx.00CAAA17
$+2B2    008C66F2     64:8B0D 2C000000        MOV ECX,DWORD PTR FS:[0x2C]
$+2B9    008C66F9     83C4 04                 ADD ESP,0x4
$+2BC    008C66FC     8B09                    MOV ECX,DWORD PTR DS:[ECX]
$+2BE    008C66FE     56                      PUSH ESI
$+2BF    008C66FF     83EC 0C                 SUB ESP,0xC
$+2C2    008C6702     8B49 18                 MOV ECX,DWORD PTR DS:[ECX+0x18]
$+2C5    008C6705     6A 01                   PUSH 0x1
$+2C7    008C6707     57                      PUSH EDI
$+2C8    008C6708     FF75 E8                 PUSH DWORD PTR SS:[EBP-0x18]
$+2CB    008C670B     FF75 E4                 PUSH DWORD PTR SS:[EBP-0x1C]
$+2CE    008C670E     68 42010000             PUSH 0x142
$+2D3    008C6713     E8 7819BCFF             CALL PathOfEx.00488090
$+2D8    008C6718     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
$+2DB    008C671B     E8 2070BAFF             CALL PathOfEx.0046D740
$+2E0    008C6720     8D8D 64FEFFFF           LEA ECX,DWORD PTR SS:[EBP-0x19C]
$+2E6    008C6726     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$+2ED    008C672D     E8 1E07BBFF             CALL PathOfEx.00476E50
$+2F2    008C6732     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+2F4    008C6734     8BCB                    MOV ECX,EBX
$+2F6    008C6736     6A 00                   PUSH 0x0
$+2F8    008C6738     FF50 50                 CALL DWORD PTR DS:[EAX+0x50]
$+2FB    008C673B     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+2FE    008C673E     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+305    008C6745     5F                      POP EDI
$+306    008C6746     5E                      POP ESI
$+307    008C6747     5B                      POP EBX
$+308    008C6748     8BE5                    MOV ESP,EBP
$+30A    008C674A     5D                      POP EBP
$+30B    008C674B     C2 0400                 RETN 0x4
$+30E    008C674E     FF77 18                 PUSH DWORD PTR DS:[EDI+0x18]
$+311    008C6751     F3:0F1047 14            MOVSS XMM0,DWORD PTR DS:[EDI+0x14]
$+316    008C6756     51                      PUSH ECX
$+317    008C6757     F3:0F110424             MOVSS DWORD PTR SS:[ESP],XMM0
$+31C    008C675C     E8 3F38F2FF             CALL PathOfEx.007E9FA0
$+321    008C6761     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+324    008C6764     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+32B    008C676B     5F                      POP EDI
$+32C    008C676C     5E                      POP ESI
$+32D    008C676D     5B                      POP EBX
$+32E    008C676E     8BE5                    MOV ESP,EBP
$+330    008C6770     5D                      POP EBP
$+331    008C6771     C2 0400                 RETN 0x4
$+334    008C6774     8D43 38                 LEA EAX,DWORD PTR DS:[EBX+0x38]
$+337    008C6777     50                      PUSH EAX
$+338    008C6778     0FB747 14               MOVZX EAX,WORD PTR DS:[EDI+0x14]
$+33C    008C677C     8D8B 30020000           LEA ECX,DWORD PTR DS:[EBX+0x230]
$+342    008C6782     50                      PUSH EAX
$+343    008C6783     8D45 08                 LEA EAX,DWORD PTR SS:[EBP+0x8]
$+346    008C6786     50                      PUSH EAX
$+347    008C6787     E8 C4CBF4FF             CALL PathOfEx.00813350
$+34C    008C678C     C745 FC 04000000        MOV DWORD PTR SS:[EBP-0x4],0x4
$+353    008C6793     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
$+356    008C6796     85F6                    TEST ESI,ESI
$+358    008C6798     0F85 49010000           JNZ PathOfEx.008C68E7
$+35E    008C679E     83EC 08                 SUB ESP,0x8
$+361    008C67A1     8D8D 14FFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xEC]
$+367    008C67A7     E8 E42FBBFF             CALL PathOfEx.00479790
$+36C    008C67AC     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
$+370    008C67B0     0FB647 24               MOVZX EAX,BYTE PTR DS:[EDI+0x24]
$+374    008C67B4     50                      PUSH EAX
$+375    008C67B5     51                      PUSH ECX
$+376    008C67B6     68 408DEF00             PUSH PathOfEx.00EF8D40                            ; UNICODE "\nflags: "
$+37B    008C67BB     51                      PUSH ECX
$+37C    008C67BC     FF77 18                 PUSH DWORD PTR DS:[EDI+0x18]
$+37F    008C67BF     0FB747 16               MOVZX EAX,WORD PTR DS:[EDI+0x16]
$+383    008C67C3     51                      PUSH ECX
$+384    008C67C4     68 248DEF00             PUSH PathOfEx.00EF8D24                            ; UNICODE "\ntarget_id: "
$+389    008C67C9     51                      PUSH ECX
$+38A    008C67CA     FF77 08                 PUSH DWORD PTR DS:[EDI+0x8]
$+38D    008C67CD     51                      PUSH ECX
$+38E    008C67CE     68 088DEF00             PUSH PathOfEx.00EF8D08                            ; UNICODE "\nobject_id: "
$+393    008C67D3     51                      PUSH ECX
$+394    008C67D4     50                      PUSH EAX
$+395    008C67D5     0FB747 14               MOVZX EAX,WORD PTR DS:[EDI+0x14]
$+399    008C67D9     51                      PUSH ECX
$+39A    008C67DA     68 EC8CEF00             PUSH PathOfEx.00EF8CEC                            ; UNICODE "\naction_id: "
$+39F    008C67DF     51                      PUSH ECX
$+3A0    008C67E0     50                      PUSH EAX
$+3A1    008C67E1     51                      PUSH ECX
$+3A2    008C67E2     68 C08CEF00             PUSH PathOfEx.00EF8CC0                            ; UNICODE "\nskill_instance_id: "
$+3A7    008C67E7     51                      PUSH ECX
$+3A8    008C67E8     8D85 24FFFFFF           LEA EAX,DWORD PTR SS:[EBP-0xDC]
$+3AE    008C67EE     68 108CEF00             PUSH PathOfEx.00EF8C10                            ; UNICODE "Triggered action was serialized to the client, but"
$+3B3    008C67F3     50                      PUSH EAX
$+3B4    008C67F4     E8 171CBCFF             CALL PathOfEx.00488410
$+3B9    008C67F9     83C4 0C                 ADD ESP,0xC
$+3BC    008C67FC     50                      PUSH EAX
$+3BD    008C67FD     E8 0E1CBCFF             CALL PathOfEx.00488410
$+3C2    008C6802     83C4 0C                 ADD ESP,0xC
$+3C5    008C6805     8BC8                    MOV ECX,EAX
$+3C7    008C6807     E8 9490CCFF             CALL PathOfEx.0058F8A0
$+3CC    008C680C     83C4 04                 ADD ESP,0x4
$+3CF    008C680F     50                      PUSH EAX
$+3D0    008C6810     E8 FB1BBCFF             CALL PathOfEx.00488410
$+3D5    008C6815     83C4 0C                 ADD ESP,0xC
$+3D8    008C6818     8BC8                    MOV ECX,EAX
$+3DA    008C681A     E8 8190CCFF             CALL PathOfEx.0058F8A0
$+3DF    008C681F     83C4 04                 ADD ESP,0x4
$+3E2    008C6822     50                      PUSH EAX
$+3E3    008C6823     E8 E81BBCFF             CALL PathOfEx.00488410
$+3E8    008C6828     83C4 0C                 ADD ESP,0xC
$+3EB    008C682B     8BC8                    MOV ECX,EAX
$+3ED    008C682D     E8 7EA2BBFF             CALL PathOfEx.00480AB0
$+3F2    008C6832     83C4 04                 ADD ESP,0x4
$+3F5    008C6835     50                      PUSH EAX
$+3F6    008C6836     E8 D51BBCFF             CALL PathOfEx.00488410
$+3FB    008C683B     83C4 0C                 ADD ESP,0xC
$+3FE    008C683E     8BC8                    MOV ECX,EAX
$+400    008C6840     E8 6BA2BBFF             CALL PathOfEx.00480AB0
$+405    008C6845     83C4 04                 ADD ESP,0x4
$+408    008C6848     50                      PUSH EAX
$+409    008C6849     E8 C21BBCFF             CALL PathOfEx.00488410
$+40E    008C684E     83C4 0C                 ADD ESP,0xC
$+411    008C6851     8BC8                    MOV ECX,EAX
$+413    008C6853     E8 5816BCFF             CALL PathOfEx.00487EB0
$+418    008C6858     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
$+41B    008C685B     50                      PUSH EAX
$+41C    008C685C     8D8D 14FFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xEC]
$+422    008C6862     E8 891BBCFF             CALL PathOfEx.004883F0
$+427    008C6867     8BF0                    MOV ESI,EAX
$+429    008C6869     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
$+42D    008C686D     FF15 C8E0D800           CALL DWORD PTR DS:[0xD8E0C8]                      ; kernel32.GetTickCount
$+433    008C6873     8BF8                    MOV EDI,EAX
$+435    008C6875     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+438    008C6878     50                      PUSH EAX
$+439    008C6879     E8 99413E00             CALL PathOfEx.00CAAA17
$+43E    008C687E     64:8B0D 2C000000        MOV ECX,DWORD PTR FS:[0x2C]
$+445    008C6885     83C4 04                 ADD ESP,0x4
$+448    008C6888     8B09                    MOV ECX,DWORD PTR DS:[ECX]
$+44A    008C688A     56                      PUSH ESI
$+44B    008C688B     83EC 0C                 SUB ESP,0xC
$+44E    008C688E     6A 01                   PUSH 0x1
$+450    008C6890     57                      PUSH EDI
$+451    008C6891     FF75 E8                 PUSH DWORD PTR SS:[EBP-0x18]
$+454    008C6894     8B49 18                 MOV ECX,DWORD PTR DS:[ECX+0x18]
$+457    008C6897     FF75 E4                 PUSH DWORD PTR SS:[EBP-0x1C]
$+45A    008C689A     68 5A010000             PUSH 0x15A
$+45F    008C689F     E8 EC17BCFF             CALL PathOfEx.00488090
$+464    008C68A4     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
$+467    008C68A7     E8 946EBAFF             CALL PathOfEx.0046D740
$+46C    008C68AC     8D8D 7CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0x84]
$+472    008C68B2     E8 B904BBFF             CALL PathOfEx.00476D70
$+477    008C68B7     8D85 7CFFFFFF           LEA EAX,DWORD PTR SS:[EBP-0x84]
$+47D    008C68BD     C645 FC 07              MOV BYTE PTR SS:[EBP-0x4],0x7
$+481    008C68C1     50                      PUSH EAX
$+482    008C68C2     C785 7CFFFFFF 2453DE00  MOV DWORD PTR SS:[EBP-0x84],PathOfEx.00DE5324
$+48C    008C68CC     E8 A0383A00             CALL PathOfEx.00C6A171
$+491    008C68D1     83C4 04                 ADD ESP,0x4
$+494    008C68D4     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+497    008C68D7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+49E    008C68DE     5F                      POP EDI
$+49F    008C68DF     5E                      POP ESI
$+4A0    008C68E0     5B                      POP EBX
$+4A1    008C68E1     8BE5                    MOV ESP,EBP
$+4A3    008C68E3     5D                      POP EBP
$+4A4    008C68E4     C2 0400                 RETN 0x4
$+4A7    008C68E7     8B57 18                 MOV EDX,DWORD PTR DS:[EDI+0x18]
$+4AA    008C68EA     85D2                    TEST EDX,EDX
$+4AC    008C68EC     74 3B                   JE SHORT PathOfEx.008C6929
$+4AE    008C68EE     8B4E 04                 MOV ECX,DWORD PTR DS:[ESI+0x4]
$+4B1    008C68F1     8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+4B4    008C68F4     3950 14                 CMP DWORD PTR DS:[EAX+0x14],EDX
$+4B7    008C68F7     74 13                   JE SHORT PathOfEx.008C690C
$+4B9    008C68F9     8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+4BC    008C68FC     52                      PUSH EDX
$+4BD    008C68FD     8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$+4C0    008C6900     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+4C2    008C6902     FF50 14                 CALL DWORD PTR DS:[EAX+0x14]
$+4C5    008C6905     8BC8                    MOV ECX,EAX
$+4C7    008C6907     E8 0499F1FF             CALL PathOfEx.007E0210
$+4CC    008C690C     50                      PUSH EAX
$+4CD    008C690D     8BCE                    MOV ECX,ESI
$+4CF    008C690F     E8 7C072200             CALL PathOfEx.00AE7090
$+4D4    008C6914     8B4E 1C                 MOV ECX,DWORD PTR DS:[ESI+0x1C]
$+4D7    008C6917     85C9                    TEST ECX,ECX
$+4D9    008C6919     74 1A                   JE SHORT PathOfEx.008C6935
$+4DB    008C691B     8B49 20                 MOV ECX,DWORD PTR DS:[ECX+0x20]
$+4DE    008C691E     8B41 10                 MOV EAX,DWORD PTR DS:[ECX+0x10]
$+4E1    008C6921     8946 20                 MOV DWORD PTR DS:[ESI+0x20],EAX
$+4E4    008C6924     8B41 14                 MOV EAX,DWORD PTR DS:[ECX+0x14]
$+4E7    008C6927     EB 09                   JMP SHORT PathOfEx.008C6932
$+4E9    008C6929     8B47 1C                 MOV EAX,DWORD PTR DS:[EDI+0x1C]
$+4EC    008C692C     8946 20                 MOV DWORD PTR DS:[ESI+0x20],EAX
$+4EF    008C692F     8B47 20                 MOV EAX,DWORD PTR DS:[EDI+0x20]
$+4F2    008C6932     8946 24                 MOV DWORD PTR DS:[ESI+0x24],EAX
$+4F5    008C6935     8A47 24                 MOV AL,BYTE PTR DS:[EDI+0x24]
$+4F8    008C6938     8BCE                    MOV ECX,ESI
$+4FA    008C693A     8846 17                 MOV BYTE PTR DS:[ESI+0x17],AL
$+4FD    008C693D     0FB747 16               MOVZX EAX,WORD PTR DS:[EDI+0x16]
$+501    008C6941     50                      PUSH EAX
$+502    008C6942     E8 29072200             CALL PathOfEx.00AE7070
$+507    008C6947     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+509    008C6949     8BCE                    MOV ECX,ESI
$+50B    008C694B     FF50 64                 CALL DWORD PTR DS:[EAX+0x64]
$+50E    008C694E     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+510    008C6950     8BCE                    MOV ECX,ESI
$+512    008C6952     8B40 18                 MOV EAX,DWORD PTR DS:[EAX+0x18]
$+515    008C6955     FFD0                    CALL EAX
$+517    008C6957     84C0                    TEST AL,AL
$+519    008C6959     0F84 59010000           JE PathOfEx.008C6AB8
$+51F    008C695F     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+521    008C6961     8BCB                    MOV ECX,EBX
$+523    008C6963     FF76 08                 PUSH DWORD PTR DS:[ESI+0x8]
$+526    008C6966     8B40 78                 MOV EAX,DWORD PTR DS:[EAX+0x78]
$+529    008C6969     FFD0                    CALL EAX
$+52B    008C696B     84C0                    TEST AL,AL
$+52D    008C696D     0F84 45010000           JE PathOfEx.008C6AB8
$+533    008C6973     6A 01                   PUSH 0x1
$+535    008C6975     8BCE                    MOV ECX,ESI
$+537    008C6977     E8 04FF2100             CALL PathOfEx.00AE6880
$+53C    008C697C     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+53E    008C697E     8BCE                    MOV ECX,ESI
$+540    008C6980     8B40 60                 MOV EAX,DWORD PTR DS:[EAX+0x60]
$+543    008C6983     FFD0                    CALL EAX
$+545    008C6985     84C0                    TEST AL,AL
$+547    008C6987     0F84 A4000000           JE PathOfEx.008C6A31
$+54D    008C698D     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+54F    008C698F     8BCE                    MOV ECX,ESI
$+551    008C6991     FF50 5C                 CALL DWORD PTR DS:[EAX+0x5C]
$+554    008C6994     8BCE                    MOV ECX,ESI
$+556    008C6996     E8 35002200             CALL PathOfEx.00AE69D0
$+55B    008C699B     68 C60A0000             PUSH 0xAC6
$+560    008C69A0     8BC8                    MOV ECX,EAX
$+562    008C69A2     8B10                    MOV EDX,DWORD PTR DS:[EAX]
$+564    008C69A4     FF12                    CALL DWORD PTR DS:[EDX]
$+566    008C69A6     8BF8                    MOV EDI,EAX
$+568    008C69A8     85FF                    TEST EDI,EDI
$+56A    008C69AA     0F84 FF000000           JE PathOfEx.008C6AAF
$+570    008C69B0     83EC 0C                 SUB ESP,0xC
$+573    008C69B3     68 E88DEF00             PUSH PathOfEx.00EF8DE8                            ; UNICODE "Metadata/Critters/BlackCat/BlackCatTemporary"
$+578    008C69B8     E8 4316E5FF             CALL PathOfEx.00718000
$+57D    008C69BD     8BC8                    MOV ECX,EAX
$+57F    008C69BF     E8 9C24F9FF             CALL PathOfEx.00858E60
$+584    008C69C4     8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
$+587    008C69C7     C645 FC 09              MOV BYTE PTR SS:[EBP-0x4],0x9
$+58B    008C69CB     85FF                    TEST EDI,EDI
$+58D    008C69CD     74 54                   JE SHORT PathOfEx.008C6A23
$+58F    008C69CF     8D46 20                 LEA EAX,DWORD PTR DS:[ESI+0x20]
$+592    008C69D2     8BF0                    MOV ESI,EAX
$+594    008C69D4     68 B8010000             PUSH 0x1B8
$+599    008C69D9     E8 B8593B00             CALL PathOfEx.00C7C396
$+59E    008C69DE     83C4 04                 ADD ESP,0x4
$+5A1    008C69E1     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+5A4    008C69E4     C645 FC 0A              MOV BYTE PTR SS:[EBP-0x4],0xA
$+5A8    008C69E8     8B4B 04                 MOV ECX,DWORD PTR DS:[EBX+0x4]
$+5AB    008C69EB     FF71 10                 PUSH DWORD PTR DS:[ECX+0x10]
$+5AE    008C69EE     8D4D E0                 LEA ECX,DWORD PTR SS:[EBP-0x20]
$+5B1    008C69F1     51                      PUSH ECX
$+5B2    008C69F2     8BC8                    MOV ECX,EAX
$+5B4    008C69F4     E8 5703F1FF             CALL PathOfEx.007D6D50
$+5B9    008C69F9     6A 00                   PUSH 0x0
$+5BB    008C69FB     C645 FC 09              MOV BYTE PTR SS:[EBP-0x4],0x9
$+5BF    008C69FF     56                      PUSH ESI
$+5C0    008C6A00     8B48 20                 MOV ECX,DWORD PTR DS:[EAX+0x20]
$+5C3    008C6A03     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+5C6    008C6A06     E8 8525F1FF             CALL PathOfEx.007D8F90
$+5CB    008C6A0B     84C0                    TEST AL,AL
$+5CD    008C6A0D     75 0C                   JNZ SHORT PathOfEx.008C6A1B
$+5CF    008C6A0F     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$+5D2    008C6A12     56                      PUSH ESI
$+5D3    008C6A13     8B49 20                 MOV ECX,DWORD PTR DS:[ECX+0x20]
$+5D6    008C6A16     E8 4520F1FF             CALL PathOfEx.007D8A60
$+5DB    008C6A1B     83EF 01                 SUB EDI,0x1
$+5DE    008C6A1E   ^ 75 B4                   JNZ SHORT PathOfEx.008C69D4
$+5E0    008C6A20     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
$+5E3    008C6A23     8D4D E0                 LEA ECX,DWORD PTR SS:[EBP-0x20]
$+5E6    008C6A26     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
$+5EA    008C6A2A     E8 E160BAFF             CALL PathOfEx.0046CB10
$+5EF    008C6A2F     EB 7E                   JMP SHORT PathOfEx.008C6AAF
$+5F1    008C6A31     83EC 08                 SUB ESP,0x8
$+5F4    008C6A34     8D8D B4FDFFFF           LEA ECX,DWORD PTR SS:[EBP-0x24C]
$+5FA    008C6A3A     E8 512DBBFF             CALL PathOfEx.00479790
$+5FF    008C6A3F     8D85 C4FDFFFF           LEA EAX,DWORD PTR SS:[EBP-0x23C]
$+605    008C6A45     C645 FC 0B              MOV BYTE PTR SS:[EBP-0x4],0xB
$+609    008C6A49     68 AC8DEF00             PUSH PathOfEx.00EF8DAC                            ; ASCII "Client couldn't execute a triggered action from the server"
$+60E    008C6A4E     50                      PUSH EAX
$+60F    008C6A4F     E8 EC9EC6FF             CALL PathOfEx.00530940
$+614    008C6A54     83C4 08                 ADD ESP,0x8
$+617    008C6A57     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
$+61A    008C6A5A     8D8D B4FDFFFF           LEA ECX,DWORD PTR SS:[EBP-0x24C]
$+620    008C6A60     50                      PUSH EAX
$+621    008C6A61     E8 8A19BCFF             CALL PathOfEx.004883F0
$+626    008C6A66     8BF8                    MOV EDI,EAX
$+628    008C6A68     C645 FC 0C              MOV BYTE PTR SS:[EBP-0x4],0xC
$+62C    008C6A6C     FF15 C8E0D800           CALL DWORD PTR DS:[0xD8E0C8]                      ; kernel32.GetTickCount
$+632    008C6A72     64:8B0D 2C000000        MOV ECX,DWORD PTR FS:[0x2C]
$+639    008C6A79     57                      PUSH EDI
$+63A    008C6A7A     83EC 0C                 SUB ESP,0xC
$+63D    008C6A7D     8B09                    MOV ECX,DWORD PTR DS:[ECX]
$+63F    008C6A7F     6A 01                   PUSH 0x1
$+641    008C6A81     50                      PUSH EAX
$+642    008C6A82     8B59 18                 MOV EBX,DWORD PTR DS:[ECX+0x18]
$+645    008C6A85     E8 06FDE3FF             CALL PathOfEx.00706790
$+64A    008C6A8A     52                      PUSH EDX
$+64B    008C6A8B     50                      PUSH EAX
$+64C    008C6A8C     68 8E010000             PUSH 0x18E
$+651    008C6A91     8BCB                    MOV ECX,EBX
$+653    008C6A93     E8 F815BCFF             CALL PathOfEx.00488090
$+658    008C6A98     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
$+65B    008C6A9B     E8 A06CBAFF             CALL PathOfEx.0046D740
$+660    008C6AA0     8D8D B4FDFFFF           LEA ECX,DWORD PTR SS:[EBP-0x24C]
$+666    008C6AA6     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
$+66A    008C6AAA     E8 A103BBFF             CALL PathOfEx.00476E50
$+66F    008C6AAF     8BCE                    MOV ECX,ESI
$+671    008C6AB1     E8 EAFD2100             CALL PathOfEx.00AE68A0
$+676    008C6AB6     33F6                    XOR ESI,ESI
$+678    008C6AB8     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$+67F    008C6ABF     85F6                    TEST ESI,ESI
$+681    008C6AC1     74 69                   JE SHORT PathOfEx.008C6B2C
$+683    008C6AC3     C745 FC 0D000000        MOV DWORD PTR SS:[EBP-0x4],0xD
$+68A    008C6ACA     8BCE                    MOV ECX,ESI
$+68C    008C6ACC     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+68E    008C6ACE     6A 01                   PUSH 0x1
$+690    008C6AD0     FF10                    CALL DWORD PTR DS:[EAX]
$+692    008C6AD2     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+695    008C6AD5     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+69C    008C6ADC     5F                      POP EDI
$+69D    008C6ADD     5E                      POP ESI
$+69E    008C6ADE     5B                      POP EBX
$+69F    008C6ADF     8BE5                    MOV ESP,EBP
$+6A1    008C6AE1     5D                      POP EBP
$+6A2    008C6AE2     C2 0400                 RETN 0x4
$+6A5    008C6AE5     8D47 14                 LEA EAX,DWORD PTR DS:[EDI+0x14]                   ; stCDR_EffectToEntity
$+6A8    008C6AE8     50                      PUSH EAX
$+6A9    008C6AE9     E8 3226F2FF             CALL PathOfEx.007E9120
$+6AE    008C6AEE     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+6B1    008C6AF1     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+6B8    008C6AF8     5F                      POP EDI
$+6B9    008C6AF9     5E                      POP ESI
$+6BA    008C6AFA     5B                      POP EBX
$+6BB    008C6AFB     8BE5                    MOV ESP,EBP
$+6BD    008C6AFD     5D                      POP EBP
$+6BE    008C6AFE     C2 0400                 RETN 0x4
$+6C1    008C6B01     0FB647 1C               MOVZX EAX,BYTE PTR DS:[EDI+0x1C]                  ; 0xdf,stCDR_NormalHitted
$+6C5    008C6B05     50                      PUSH EAX
$+6C6    008C6B06     8D47 14                 LEA EAX,DWORD PTR DS:[EDI+0x14]
$+6C9    008C6B09     50                      PUSH EAX
$+6CA    008C6B0A     E8 1125F2FF             CALL PathOfEx.007E9020
$+6CF    008C6B0F     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+6D2    008C6B12     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+6D9    008C6B19     5F                      POP EDI
$+6DA    008C6B1A     5E                      POP ESI
$+6DB    008C6B1B     5B                      POP EBX
$+6DC    008C6B1C     8BE5                    MOV ESP,EBP
$+6DE    008C6B1E     5D                      POP EBP
$+6DF    008C6B1F     C2 0400                 RETN 0x4
$+6E2    008C6B22     0FB647 14               MOVZX EAX,BYTE PTR DS:[EDI+0x14]
$+6E6    008C6B26     50                      PUSH EAX
$+6E7    008C6B27     E8 8439F2FF             CALL PathOfEx.007EA4B0
$+6EC    008C6B2C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+6EF    008C6B2F     5F                      POP EDI
$+6F0    008C6B30     5E                      POP ESI
$+6F1    008C6B31     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+6F8    008C6B38     5B                      POP EBX
$+6F9    008C6B39     8BE5                    MOV ESP,EBP
$+6FB    008C6B3B     5D                      POP EBP
$+6FC    008C6B3C     C2 0400                 RETN 0x4
$+6FF    008C6B3F     90                      NOP
$+700    008C6B40     86648C 00               XCHG BYTE PTR SS:[ESP+ECX*4],AH
$+704    008C6B44     94                      XCHG EAX,ESP
$+705    008C6B45     65:8C00                 MOV WORD PTR GS:[EAX],ES
$+708    008C6B48     1C 66                   SBB AL,0x66
$+70A    008C6B4A     8C00                    MOV WORD PTR DS:[EAX],ES
$+70C    008C6B4C     6C                      INS BYTE PTR ES:[EDI],DX
$+70D    008C6B4D     66:8C00                 MOV WORD PTR DS:[EAX],ES
$+710    008C6B50     E5 6A                   IN EAX,0x6A
$+712    008C6B52     8C00                    MOV WORD PTR DS:[EAX],ES
$+714    008C6B54     016B 8C                 ADD DWORD PTR DS:[EBX-0x74],EBP
$+717    008C6B57     0022                    ADD BYTE PTR DS:[EDX],AH
$+719    008C6B59     6B8C00 74678C00 4E      IMUL ECX,DWORD PTR DS:[EAX+EAX+0x8C6774],0x4E
$+721    008C6B61     67:8C00                 MOV WORD PTR DS:[BX+SI],ES
$+724    008C6B64     2C 6B                   SUB AL,0x6B
$+726    008C6B66     8C00                    MOV WORD PTR DS:[EAX],ES
$+728    008C6B68     0001                    ADD BYTE PTR DS:[ECX],AL
$+72A    008C6B6A     0203                    ADD AL,BYTE PTR DS:[EBX]
$+72C    008C6B6C     04 05                   ADD AL,0x5
$+72E    008C6B6E     0909                    OR DWORD PTR DS:[ECX],ECX
$+730    008C6B70     06                      PUSH ES
$+731    008C6B71     0909                    OR DWORD PTR DS:[ECX],ECX
$+733    008C6B73     0909                    OR DWORD PTR DS:[ECX],ECX
$+735    008C6B75     0909                    OR DWORD PTR DS:[ECX],ECX
$+737    008C6B77     0909                    OR DWORD PTR DS:[ECX],ECX
$+739    008C6B79     0909                    OR DWORD PTR DS:[ECX],ECX
$+73B    008C6B7B     0909                    OR DWORD PTR DS:[ECX],ECX
$+73D    008C6B7D     0909                    OR DWORD PTR DS:[ECX],ECX
$+73F    008C6B7F     0909                    OR DWORD PTR DS:[ECX],ECX
$+741    008C6B81     0907                    OR DWORD PTR DS:[EDI],EAX
$+743    008C6B83     08CC                    OR AH,CL
$+745    008C6B85     CC                      INT3
$+746    008C6B86     CC                      INT3
*/
/**/
//tag_收包解密dword
/*
$-1      0075C95F     CC                      INT3
$ ==>    0075C960     51                      PUSH ECX                              ; tag_收包解密dword
$+1      0075C961     53                      PUSH EBX                              ; ecx=stCD_Socket*
$+2      0075C962     56                      PUSH ESI                              ; g_c_DecryptGetInt
$+3      0075C963     6A 01                   PUSH 0x1
$+5      0075C965     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
$+9      0075C969     C64424 0C 00            MOV BYTE PTR SS:[ESP+0xC],0x0
$+E      0075C96E     50                      PUSH EAX
$+F      0075C96F     8BF1                    MOV ESI,ECX
$+11     0075C971     E8 DA860300             CALL PathOfEx.00795050
$+16     0075C976     8A5C24 08               MOV BL,BYTE PTR SS:[ESP+0x8]
$+1A     0075C97A     84DB                    TEST BL,BL
$+1C     0075C97C     78 13                   JS SHORT PathOfEx.0075C991
$+1E     0075C97E     0FB6C3                  MOVZX EAX,BL
$+21     0075C981     F6C3 40                 TEST BL,0x40
$+24     0075C984     0F84 57010000           JE PathOfEx.0075CAE1
$+2A     0075C98A     5E                      POP ESI
$+2B     0075C98B     83C8 80                 OR EAX,0xFFFFFF80
$+2E     0075C98E     5B                      POP EBX
$+2F     0075C98F     59                      POP ECX
$+30     0075C990     C3                      RETN
$+31     0075C991     8AC3                    MOV AL,BL
$+33     0075C993     C64424 08 00            MOV BYTE PTR SS:[ESP+0x8],0x0
$+38     0075C998     24 C0                   AND AL,0xC0
$+3A     0075C99A     8BCE                    MOV ECX,ESI
$+3C     0075C99C     6A 01                   PUSH 0x1
$+3E     0075C99E     3C 80                   CMP AL,0x80
$+40     0075C9A0     75 2B                   JNZ SHORT PathOfEx.0075C9CD
$+42     0075C9A2     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
$+46     0075C9A6     50                      PUSH EAX
$+47     0075C9A7     E8 A4860300             CALL PathOfEx.00795050
$+4C     0075C9AC     0FB64424 08             MOVZX EAX,BYTE PTR SS:[ESP+0x8]
$+51     0075C9B1     0FB6CB                  MOVZX ECX,BL
$+54     0075C9B4     83E1 3F                 AND ECX,0x3F
$+57     0075C9B7     C1E1 08                 SHL ECX,0x8
$+5A     0075C9BA     0BC8                    OR ECX,EAX
$+5C     0075C9BC     8BC1                    MOV EAX,ECX
$+5E     0075C9BE     0D 00C0FFFF             OR EAX,0xFFFFC000
$+63     0075C9C3     80E3 20                 AND BL,0x20
$+66     0075C9C6     5E                      POP ESI
$+67     0075C9C7     0F44C1                  CMOVE EAX,ECX
$+6A     0075C9CA     5B                      POP EBX
$+6B     0075C9CB     59                      POP ECX
$+6C     0075C9CC     C3                      RETN
$+6D     0075C9CD     8AC3                    MOV AL,BL
$+6F     0075C9CF     C64424 0D 00            MOV BYTE PTR SS:[ESP+0xD],0x0
$+74     0075C9D4     24 E0                   AND AL,0xE0
$+76     0075C9D6     3C C0                   CMP AL,0xC0
$+78     0075C9D8     75 43                   JNZ SHORT PathOfEx.0075CA1D
$+7A     0075C9DA     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
$+7E     0075C9DE     50                      PUSH EAX
$+7F     0075C9DF     E8 6C860300             CALL PathOfEx.00795050
$+84     0075C9E4     6A 01                   PUSH 0x1
$+86     0075C9E6     8D4424 0D               LEA EAX,DWORD PTR SS:[ESP+0xD]
$+8A     0075C9EA     8BCE                    MOV ECX,ESI
$+8C     0075C9EC     50                      PUSH EAX
$+8D     0075C9ED     E8 5E860300             CALL PathOfEx.00795050
$+92     0075C9F2     0FB64424 08             MOVZX EAX,BYTE PTR SS:[ESP+0x8]
$+97     0075C9F7     0FB6CB                  MOVZX ECX,BL
$+9A     0075C9FA     83E1 1F                 AND ECX,0x1F
$+9D     0075C9FD     C1E1 08                 SHL ECX,0x8
$+A0     0075CA00     0BC8                    OR ECX,EAX
$+A2     0075CA02     0FB64424 09             MOVZX EAX,BYTE PTR SS:[ESP+0x9]
$+A7     0075CA07     C1E1 08                 SHL ECX,0x8
$+AA     0075CA0A     0BC8                    OR ECX,EAX
$+AC     0075CA0C     8BC1                    MOV EAX,ECX
$+AE     0075CA0E     0D 0000E0FF             OR EAX,0xFFE00000
$+B3     0075CA13     80E3 10                 AND BL,0x10
$+B6     0075CA16     5E                      POP ESI
$+B7     0075CA17     0F44C1                  CMOVE EAX,ECX
$+BA     0075CA1A     5B                      POP EBX
$+BB     0075CA1B     59                      POP ECX
$+BC     0075CA1C     C3                      RETN
$+BD     0075CA1D     8AC3                    MOV AL,BL
$+BF     0075CA1F     C64424 0E 00            MOV BYTE PTR SS:[ESP+0xE],0x0
$+C4     0075CA24     24 F0                   AND AL,0xF0
$+C6     0075CA26     3C E0                   CMP AL,0xE0
$+C8     0075CA28     75 5B                   JNZ SHORT PathOfEx.0075CA85
$+CA     0075CA2A     8D4424 0D               LEA EAX,DWORD PTR SS:[ESP+0xD]
$+CE     0075CA2E     50                      PUSH EAX
$+CF     0075CA2F     E8 1C860300             CALL PathOfEx.00795050
$+D4     0075CA34     6A 01                   PUSH 0x1
$+D6     0075CA36     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
$+DA     0075CA3A     8BCE                    MOV ECX,ESI
$+DC     0075CA3C     50                      PUSH EAX
$+DD     0075CA3D     E8 0E860300             CALL PathOfEx.00795050
$+E2     0075CA42     6A 01                   PUSH 0x1
$+E4     0075CA44     8D4424 0E               LEA EAX,DWORD PTR SS:[ESP+0xE]
$+E8     0075CA48     8BCE                    MOV ECX,ESI
$+EA     0075CA4A     50                      PUSH EAX
$+EB     0075CA4B     E8 00860300             CALL PathOfEx.00795050
$+F0     0075CA50     0FB64424 09             MOVZX EAX,BYTE PTR SS:[ESP+0x9]
$+F5     0075CA55     0FB6CB                  MOVZX ECX,BL
$+F8     0075CA58     83E1 0F                 AND ECX,0xF
$+FB     0075CA5B     C1E1 08                 SHL ECX,0x8
$+FE     0075CA5E     0BC8                    OR ECX,EAX
$+100    0075CA60     0FB64424 08             MOVZX EAX,BYTE PTR SS:[ESP+0x8]
$+105    0075CA65     C1E1 08                 SHL ECX,0x8
$+108    0075CA68     0BC8                    OR ECX,EAX
$+10A    0075CA6A     0FB64424 0A             MOVZX EAX,BYTE PTR SS:[ESP+0xA]
$+10F    0075CA6F     C1E1 08                 SHL ECX,0x8
$+112    0075CA72     0BC8                    OR ECX,EAX
$+114    0075CA74     8BC1                    MOV EAX,ECX
$+116    0075CA76     0D 000000F0             OR EAX,0xF0000000
$+11B    0075CA7B     80E3 08                 AND BL,0x8
$+11E    0075CA7E     5E                      POP ESI
$+11F    0075CA7F     0F44C1                  CMOVE EAX,ECX
$+122    0075CA82     5B                      POP EBX
$+123    0075CA83     59                      POP ECX
$+124    0075CA84     C3                      RETN
$+125    0075CA85     8D4424 0E               LEA EAX,DWORD PTR SS:[ESP+0xE]
$+129    0075CA89     C64424 0F 00            MOV BYTE PTR SS:[ESP+0xF],0x0
$+12E    0075CA8E     50                      PUSH EAX
$+12F    0075CA8F     E8 BC850300             CALL PathOfEx.00795050
$+134    0075CA94     6A 01                   PUSH 0x1
$+136    0075CA96     8D4424 0D               LEA EAX,DWORD PTR SS:[ESP+0xD]
$+13A    0075CA9A     8BCE                    MOV ECX,ESI
$+13C    0075CA9C     50                      PUSH EAX
$+13D    0075CA9D     E8 AE850300             CALL PathOfEx.00795050
$+142    0075CAA2     6A 01                   PUSH 0x1
$+144    0075CAA4     8D4424 0C               LEA EAX,DWORD PTR SS:[ESP+0xC]
$+148    0075CAA8     8BCE                    MOV ECX,ESI
$+14A    0075CAAA     50                      PUSH EAX
$+14B    0075CAAB     E8 A0850300             CALL PathOfEx.00795050
$+150    0075CAB0     6A 01                   PUSH 0x1
$+152    0075CAB2     8D4424 0F               LEA EAX,DWORD PTR SS:[ESP+0xF]
$+156    0075CAB6     8BCE                    MOV ECX,ESI
$+158    0075CAB8     50                      PUSH EAX
$+159    0075CAB9     E8 92850300             CALL PathOfEx.00795050
$+15E    0075CABE     0FB64424 0A             MOVZX EAX,BYTE PTR SS:[ESP+0xA]
$+163    0075CAC3     0FB64C24 09             MOVZX ECX,BYTE PTR SS:[ESP+0x9]
$+168    0075CAC8     C1E0 08                 SHL EAX,0x8
$+16B    0075CACB     0BC1                    OR EAX,ECX
$+16D    0075CACD     0FB64C24 08             MOVZX ECX,BYTE PTR SS:[ESP+0x8]
$+172    0075CAD2     C1E0 08                 SHL EAX,0x8
$+175    0075CAD5     0BC1                    OR EAX,ECX
$+177    0075CAD7     0FB64C24 0B             MOVZX ECX,BYTE PTR SS:[ESP+0xB]
$+17C    0075CADC     C1E0 08                 SHL EAX,0x8
$+17F    0075CADF     0BC1                    OR EAX,ECX
$+181    0075CAE1     5E                      POP ESI
$+182    0075CAE2     5B                      POP EBX
$+183    0075CAE3     59                      POP ECX
$+184    0075CAE4     C3                      RETN
$+185    0075CAE5     CC                      INT3
*/
/**/
//tag_g_c_touch_target_obj_impl
/*
$-A1     008D706F     CC                      INT3
$-A0     008D7070     55                      PUSH EBP
$-9F     008D7071     8BEC                    MOV EBP,ESP
$-9D     008D7073     6A FF                   PUSH -0x1
$-9B     008D7075     68 C8ABD200             PUSH PathOfEx.00D2ABC8
$-96     008D707A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-90     008D7080     50                      PUSH EAX
$-8F     008D7081     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-88     008D7088     83EC 08                 SUB ESP,0x8
$-85     008D708B     B8 1A000000             MOV EAX,0x1A
$-80     008D7090     C745 EC 609CEF00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EF9C60    ; ASCII " 殒"
$-79     008D7097     56                      PUSH ESI
$-78     008D7098     8BF1                    MOV ESI,ECX
$-76     008D709A     66:8945 F0              MOV WORD PTR SS:[EBP-0x10],AX
$-72     008D709E     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$-6F     008D70A1     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$-68     008D70A8     50                      PUSH EAX
$-67     008D70A9     FFB6 983A0000           PUSH DWORD PTR DS:[ESI+0x3A98]
$-61     008D70AF     E8 4C32DEFF             CALL PathOfEx.006BA300
$-5C     008D70B4     8BC8                    MOV ECX,EAX
$-5A     008D70B6     E8 05330100             CALL PathOfEx.008EA3C0
$-55     008D70BB     8B8E 983A0000           MOV ECX,DWORD PTR DS:[ESI+0x3A98]
$-4F     008D70C1     6A 00                   PUSH 0x0
$-4D     008D70C3     E8 B8E1EBFF             CALL PathOfEx.00795280
$-48     008D70C8     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-45     008D70CB     5E                      POP ESI
$-44     008D70CC     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-3D     008D70D3     8BE5                    MOV ESP,EBP
$-3B     008D70D5     5D                      POP EBP
$-3A     008D70D6     C3                      RETN
$-39     008D70D7     CC                      INT3
$-38     008D70D8     CC                      INT3
$-37     008D70D9     CC                      INT3
$-36     008D70DA     CC                      INT3
$-35     008D70DB     CC                      INT3
$-34     008D70DC     CC                      INT3
$-33     008D70DD     CC                      INT3
$-32     008D70DE     CC                      INT3
$-31     008D70DF     CC                      INT3
$-30     008D70E0     55                      PUSH EBP                                         ; g_c_touch_target_obj
$-2F     008D70E1     8BEC                    MOV EBP,ESP
$-2D     008D70E3     83E4 F8                 AND ESP,0xFFFFFFF8
$-2A     008D70E6     83EC 08                 SUB ESP,0x8
$-27     008D70E9     8D0424                  LEA EAX,DWORD PTR SS:[ESP]
$-24     008D70EC     FF75 10                 PUSH DWORD PTR SS:[EBP+0x10]                     ; small_flag
$-21     008D70EF     50                      PUSH EAX
$-20     008D70F0     FF75 0C                 PUSH DWORD PTR SS:[EBP+0xC]                      ; net_id
$-1D     008D70F3     FF75 08                 PUSH DWORD PTR SS:[EBP+0x8]                      ; enCD_TouchTargetFlag
$-1A     008D70F6     E8 15000000             CALL PathOfEx.008D7110
$-15     008D70FB     8BE5                    MOV ESP,EBP
$-13     008D70FD     5D                      POP EBP
$-12     008D70FE     C2 0C00                 RETN 0xC
$-F      008D7101     CC                      INT3
$-E      008D7102     CC                      INT3
$-D      008D7103     CC                      INT3
$-C      008D7104     CC                      INT3
$-B      008D7105     CC                      INT3
$-A      008D7106     CC                      INT3
$-9      008D7107     CC                      INT3
$-8      008D7108     CC                      INT3
$-7      008D7109     CC                      INT3
$-6      008D710A     CC                      INT3
$-5      008D710B     CC                      INT3
$-4      008D710C     CC                      INT3
$-3      008D710D     CC                      INT3
$-2      008D710E     CC                      INT3
$-1      008D710F     CC                      INT3
$ ==>    008D7110     55                      PUSH EBP                                         ; tag_g_c_touch_target_obj_impl
$+1      008D7111     8BEC                    MOV EBP,ESP                                      ; ecx=stCD_GameLogic*
$+3      008D7113     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]                         ; p1=enCD_TouchTargetFlag
$+9      008D7119     6A FF                   PUSH -0x1                                        ; p2=net_id
$+B      008D711B     68 685AD600             PUSH PathOfEx.00D65A68                           ; p3=堆栈地址
$+10     008D7120     50                      PUSH EAX                                         ; p4=enCD_TouchTargetSmallFlag
$+11     008D7121     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     008D7128     83EC 14                 SUB ESP,0x14
$+1B     008D712B     56                      PUSH ESI
$+1C     008D712C     8BF1                    MOV ESI,ECX
$+1E     008D712E     57                      PUSH EDI
$+1F     008D712F     8B8E B83C0000           MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
$+25     008D7135     85C9                    TEST ECX,ECX
$+27     008D7137     0F84 7F010000           JE PathOfEx.008D72BC
$+2D     008D713D     83BE 683F0000 00        CMP DWORD PTR DS:[ESI+0x3F68],0x0
$+34     008D7144     0F8F 72010000           JG PathOfEx.008D72BC
$+3A     008D714A     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]                   ; skill_id
$+3D     008D714D     66:85FF                 TEST DI,DI
$+40     008D7150     75 17                   JNZ SHORT PathOfEx.008D7169
$+42     008D7152     0FB705 40361301         MOVZX EAX,WORD PTR DS:[0x1133640]
$+49     008D7159     50                      PUSH EAX
$+4A     008D715A     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+4C     008D715C     FF50 54                 CALL DWORD PTR DS:[EAX+0x54]
$+4F     008D715F     8BC8                    MOV ECX,EAX
$+51     008D7161     E8 5AC5F3FF             CALL PathOfEx.008136C0
$+56     008D7166     0FB7F8                  MOVZX EDI,AX
$+59     008D7169     8D8E 14070000           LEA ECX,DWORD PTR DS:[ESI+0x714]
$+5F     008D716F     E8 8CC9BDFF             CALL PathOfEx.004B3B00                           ; hear
$+64     008D7174     8B86 943A0000           MOV EAX,DWORD PTR DS:[ESI+0x3A94]
$+6A     008D717A     66:89BE 643F0000        MOV WORD PTR DS:[ESI+0x3F64],DI
$+71     008D7181     83B8 E8010000 02        CMP DWORD PTR DS:[EAX+0x1E8],0x2
$+78     008D7188     0F85 90000000           JNZ PathOfEx.008D721E
$+7E     008D718E     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
$+81     008D7191     85C0                    TEST EAX,EAX
$+83     008D7193     74 25                   JE SHORT PathOfEx.008D71BA
$+85     008D7195     B9 CC000000             MOV ECX,0xCC
$+8A     008D719A     C745 E4 A49BEF00        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EF9BA4
$+91     008D71A1     66:894D E8              MOV WORD PTR SS:[EBP-0x18],CX
$+95     008D71A5     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+9C     008D71AC     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+9F     008D71AF     8A45 14                 MOV AL,BYTE PTR SS:[EBP+0x14]
$+A2     008D71B2     8845 F2                 MOV BYTE PTR SS:[EBP-0xE],AL
$+A5     008D71B5     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+A8     008D71B8     EB 2E                   JMP SHORT PathOfEx.008D71E8
$+AA     008D71BA     B8 CB000000             MOV EAX,0xCB
$+AF     008D71BF     C745 E0 989BEF00        MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EF9B98
$+B6     008D71C6     66:8945 E4              MOV WORD PTR SS:[EBP-0x1C],AX
$+BA     008D71CA     8B4D 10                 MOV ECX,DWORD PTR SS:[EBP+0x10]
$+BD     008D71CD     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+C4     008D71D4     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+C6     008D71D6     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+C9     008D71D9     8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+CC     008D71DC     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+CF     008D71DF     8A45 14                 MOV AL,BYTE PTR SS:[EBP+0x14]
$+D2     008D71E2     8845 F2                 MOV BYTE PTR SS:[EBP-0xE],AL
$+D5     008D71E5     8D45 E0                 LEA EAX,DWORD PTR SS:[EBP-0x20]
$+D8     008D71E8     50                      PUSH EAX
$+D9     008D71E9     FFB6 983A0000           PUSH DWORD PTR DS:[ESI+0x3A98]
$+DF     008D71EF     66:897D F0              MOV WORD PTR SS:[EBP-0x10],DI
$+E3     008D71F3     E8 0831DEFF             CALL PathOfEx.006BA300                           ; 发包相关函数吧
$+E8     008D71F8     8BC8                    MOV ECX,EAX
$+EA     008D71FA     E8 C1310100             CALL PathOfEx.008EA3C0                           ; 构造发包参数
$+EF     008D71FF     8B8E 983A0000           MOV ECX,DWORD PTR DS:[ESI+0x3A98]
$+F5     008D7205     6A 00                   PUSH 0x0
$+F7     008D7207     E8 74E0EBFF             CALL PathOfEx.00795280                           ; 发包
$+FC     008D720C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+FF     008D720F     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+106    008D7216     5F                      POP EDI
$+107    008D7217     5E                      POP ESI
$+108    008D7218     8BE5                    MOV ESP,EBP
$+10A    008D721A     5D                      POP EBP
$+10B    008D721B     C2 1000                 RETN 0x10
$+10E    008D721E     8B8E B83C0000           MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
$+114    008D7224     E8 7741F1FF             CALL PathOfEx.007EB3A0
$+119    008D7229     84C0                    TEST AL,AL
$+11B    008D722B     74 55                   JE SHORT PathOfEx.008D7282
$+11D    008D722D     B8 D1000000             MOV EAX,0xD1
$+122    008D7232     C745 EC 609CEF00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EF9C60    ; ASCII " 殒"
$+129    008D7239     66:8945 F0              MOV WORD PTR SS:[EBP-0x10],AX
$+12D    008D723D     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+130    008D7240     C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4],0x2
$+137    008D7247     50                      PUSH EAX
$+138    008D7248     FFB6 983A0000           PUSH DWORD PTR DS:[ESI+0x3A98]
$+13E    008D724E     E8 AD30DEFF             CALL PathOfEx.006BA300
$+143    008D7253     8BC8                    MOV ECX,EAX
$+145    008D7255     E8 66310100             CALL PathOfEx.008EA3C0
$+14A    008D725A     8B8E 983A0000           MOV ECX,DWORD PTR DS:[ESI+0x3A98]
$+150    008D7260     6A 00                   PUSH 0x0
$+152    008D7262     E8 19E0EBFF             CALL PathOfEx.00795280
$+157    008D7267     8B8E B83C0000           MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
$+15D    008D726D     6A 00                   PUSH 0x0
$+15F    008D726F     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+161    008D7271     FF50 50                 CALL DWORD PTR DS:[EAX+0x50]
$+164    008D7274     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$+16B    008D727B     C745 EC 489CEF00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EF9C48    ; ASCII " 殒"
$+172    008D7282     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
$+175    008D7285     8D8E 383B0000           LEA ECX,DWORD PTR DS:[ESI+0x3B38]
$+17B    008D728B     85C0                    TEST EAX,EAX
$+17D    008D728D     74 04                   JE SHORT PathOfEx.008D7293
$+17F    008D728F     8901                    MOV DWORD PTR DS:[ECX],EAX
$+181    008D7291     EB 14                   JMP SHORT PathOfEx.008D72A7
$+183    008D7293     8B55 10                 MOV EDX,DWORD PTR SS:[EBP+0x10]
$+186    008D7296     C701 00000000           MOV DWORD PTR DS:[ECX],0x0
$+18C    008D729C     8B02                    MOV EAX,DWORD PTR DS:[EDX]
$+18E    008D729E     8941 04                 MOV DWORD PTR DS:[ECX+0x4],EAX
$+191    008D72A1     8B42 04                 MOV EAX,DWORD PTR DS:[EDX+0x4]
$+194    008D72A4     8941 08                 MOV DWORD PTR DS:[ECX+0x8],EAX
$+197    008D72A7     8A45 14                 MOV AL,BYTE PTR SS:[EBP+0x14]
$+19A    008D72AA     8841 0E                 MOV BYTE PTR DS:[ECX+0xE],AL
$+19D    008D72AD     66:8979 0C              MOV WORD PTR DS:[ECX+0xC],DI
$+1A1    008D72B1     FFB6 B83C0000           PUSH DWORD PTR DS:[ESI+0x3CB8]
$+1A7    008D72B7     E8 84D81C00             CALL PathOfEx.00AA4B40
$+1AC    008D72BC     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1AF    008D72BF     5F                      POP EDI
$+1B0    008D72C0     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+1B7    008D72C7     5E                      POP ESI
$+1B8    008D72C8     8BE5                    MOV ESP,EBP
$+1BA    008D72CA     5D                      POP EBP
$+1BB    008D72CB     C2 1000                 RETN 0x10
$+1BE    008D72CE     CC                      INT3
$+1BF    008D72CF     CC                      INT3
$+1C0    008D72D0     55                      PUSH EBP
$+1C1    008D72D1     8BEC                    MOV EBP,ESP
$+1C3    008D72D3     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+1C9    008D72D9     6A FF                   PUSH -0x1
$+1CB    008D72DB     68 905AD600             PUSH PathOfEx.00D65A90
$+1D0    008D72E0     50                      PUSH EAX
$+1D1    008D72E1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+1D8    008D72E8     83EC 14                 SUB ESP,0x14
$+1DB    008D72EB     56                      PUSH ESI
$+1DC    008D72EC     8BF1                    MOV ESI,ECX
$+1DE    008D72EE     8B86 943A0000           MOV EAX,DWORD PTR DS:[ESI+0x3A94]
$+1E4    008D72F4     83B8 E8010000 02        CMP DWORD PTR DS:[EAX+0x1E8],0x2
$+1EB    008D72FB     75 59                   JNZ SHORT PathOfEx.008D7356
$+1ED    008D72FD     B8 CD000000             MOV EAX,0xCD
$+1F2    008D7302     C745 E0 B09BEF00        MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EF9BB0
$+1F9    008D7309     66:8945 E4              MOV WORD PTR SS:[EBP-0x1C],AX
$+1FD    008D730D     8B4D 08                 MOV ECX,DWORD PTR SS:[EBP+0x8]
$+200    008D7310     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+207    008D7317     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+209    008D7319     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+20C    008D731C     8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+20F    008D731F     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+212    008D7322     8D45 E0                 LEA EAX,DWORD PTR SS:[EBP-0x20]
$+215    008D7325     50                      PUSH EAX
$+216    008D7326     FFB6 983A0000           PUSH DWORD PTR DS:[ESI+0x3A98]
$+21C    008D732C     E8 CF2FDEFF             CALL PathOfEx.006BA300
$+221    008D7331     8BC8                    MOV ECX,EAX
$+223    008D7333     E8 88300100             CALL PathOfEx.008EA3C0
$+228    008D7338     8B8E 983A0000           MOV ECX,DWORD PTR DS:[ESI+0x3A98]
$+22E    008D733E     6A 00                   PUSH 0x0
$+230    008D7340     E8 3BDFEBFF             CALL PathOfEx.00795280
$+235    008D7345     5E                      POP ESI
$+236    008D7346     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+239    008D7349     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+240    008D7350     8BE5                    MOV ESP,EBP
$+242    008D7352     5D                      POP EBP
$+243    008D7353     C2 0400                 RETN 0x4
$+246    008D7356     53                      PUSH EBX
$+247    008D7357     8B9E B83C0000           MOV EBX,DWORD PTR DS:[ESI+0x3CB8]
$+24D    008D735D     57                      PUSH EDI
$+24E    008D735E     8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
$+251    008D7361     8B43 70                 MOV EAX,DWORD PTR DS:[EBX+0x70]
$+254    008D7364     D1E8                    SHR EAX,1
$+256    008D7366     24 01                   AND AL,0x1
$+258    008D7368     74 6C                   JE SHORT PathOfEx.008D73D6
$+25A    008D736A     8B4B 30                 MOV ECX,DWORD PTR DS:[EBX+0x30]
$+25D    008D736D     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+25F    008D736F     8B40 4C                 MOV EAX,DWORD PTR DS:[EAX+0x4C]
$+262    008D7372     FFD0                    CALL EAX
$+264    008D7374     84C0                    TEST AL,AL
$+266    008D7376     74 5E                   JE SHORT PathOfEx.008D73D6
$+268    008D7378     8B43 30                 MOV EAX,DWORD PTR DS:[EBX+0x30]
$+26B    008D737B     F640 17 20              TEST BYTE PTR DS:[EAX+0x17],0x20
$+26F    008D737F     75 55                   JNZ SHORT PathOfEx.008D73D6
$+271    008D7381     8B8E B83C0000           MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
$+277    008D7387     FF77 04                 PUSH DWORD PTR DS:[EDI+0x4]
$+27A    008D738A     FF37                    PUSH DWORD PTR DS:[EDI]
$+27C    008D738C     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+27E    008D738E     FF50 4C                 CALL DWORD PTR DS:[EAX+0x4C]
$+281    008D7391     B8 D0000000             MOV EAX,0xD0
$+286    008D7396     C745 E0 B09BEF00        MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EF9BB0
$+28D    008D739D     66:8945 E4              MOV WORD PTR SS:[EBP-0x1C],AX
$+291    008D73A1     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+298    008D73A8     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+29A    008D73AA     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+29D    008D73AD     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$+2A0    008D73B0     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+2A3    008D73B3     8D45 E0                 LEA EAX,DWORD PTR SS:[EBP-0x20]
$+2A6    008D73B6     50                      PUSH EAX
$+2A7    008D73B7     FFB6 983A0000           PUSH DWORD PTR DS:[ESI+0x3A98]
$+2AD    008D73BD     E8 3E2FDEFF             CALL PathOfEx.006BA300
$+2B2    008D73C2     8BC8                    MOV ECX,EAX
$+2B4    008D73C4     E8 F72F0100             CALL PathOfEx.008EA3C0
$+2B9    008D73C9     8B8E 983A0000           MOV ECX,DWORD PTR DS:[ESI+0x3A98]
$+2BF    008D73CF     6A 00                   PUSH 0x0
$+2C1    008D73D1     E8 AADEEBFF             CALL PathOfEx.00795280
$+2C6    008D73D6     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+2C9    008D73D9     C786 383B0000 00000000  MOV DWORD PTR DS:[ESI+0x3B38],0x0
$+2D3    008D73E3     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+2D5    008D73E5     8986 3C3B0000           MOV DWORD PTR DS:[ESI+0x3B3C],EAX
$+2DB    008D73EB     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$+2DE    008D73EE     5F                      POP EDI
$+2DF    008D73EF     8986 403B0000           MOV DWORD PTR DS:[ESI+0x3B40],EAX
$+2E5    008D73F5     5B                      POP EBX
$+2E6    008D73F6     5E                      POP ESI
$+2E7    008D73F7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+2EE    008D73FE     8BE5                    MOV ESP,EBP
$+2F0    008D7400     5D                      POP EBP
$+2F1    008D7401     C2 0400                 RETN 0x4
$+2F4    008D7404     CC                      INT3
*/

//tag_点击选择网关的时候会断到
/*
$-311    00DF96CF     CC                        INT3
$-310    00DF96D0     55                        PUSH EBP
$-30F    00DF96D1     8BEC                      MOV EBP,ESP
$-30D    00DF96D3     6A FF                     PUSH -0x1
$-30B    00DF96D5     68 58984A01               PUSH PathOfEx.014A9858
$-306    00DF96DA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-300    00DF96E0     50                        PUSH EAX
$-2FF    00DF96E1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-2F8    00DF96E8     83EC 18                   SUB ESP,0x18
$-2F5    00DF96EB     56                        PUSH ESI
$-2F4    00DF96EC     8BF1                      MOV ESI,ECX
$-2F2    00DF96EE     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$-2EF    00DF96F1     E8 3AA9FFFF               CALL PathOfEx.00DF4030
$-2EA    00DF96F6     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$-2E7    00DF96F9     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-2E0    00DF9700     E8 ABB10F00               CALL PathOfEx.00EF48B0
$-2DB    00DF9705     84C0                      TEST AL,AL
$-2D9    00DF9707     74 08                     JE SHORT PathOfEx.00DF9711
$-2D7    00DF9709     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$-2D4    00DF970C     E8 9FA50F00               CALL PathOfEx.00EF3CB0
$-2CF    00DF9711     8B8E 8C000000             MOV ECX,DWORD PTR DS:[ESI+0x8C]
$-2C9    00DF9717     85C9                      TEST ECX,ECX
$-2C7    00DF9719     74 0F                     JE SHORT PathOfEx.00DF972A
$-2C5    00DF971B     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-2C3    00DF971D     FF90 58010000             CALL DWORD PTR DS:[EAX+0x158]
$-2BD    00DF9723     8BCE                      MOV ECX,ESI
$-2BB    00DF9725     E8 A60C0000               CALL PathOfEx.00DFA3D0
$-2B6    00DF972A     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
$-2B3    00DF972D     83F8 08                   CMP EAX,0x8
$-2B0    00DF9730     72 0F                     JB SHORT PathOfEx.00DF9741
$-2AE    00DF9732     6A 02                     PUSH 0x2
$-2AC    00DF9734     40                        INC EAX
$-2AB    00DF9735     50                        PUSH EAX
$-2AA    00DF9736     FF75 DC                   PUSH DWORD PTR SS:[EBP-0x24]
$-2A7    00DF9739     E8 72F9DBFF               CALL PathOfEx.00BB90B0
$-2A2    00DF973E     83C4 0C                   ADD ESP,0xC
$-29F    00DF9741     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-29C    00DF9744     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-295    00DF974B     5E                        POP ESI
$-294    00DF974C     8BE5                      MOV ESP,EBP
$-292    00DF974E     5D                        POP EBP
$-291    00DF974F     C3                        RETN
$-290    00DF9750     55                        PUSH EBP
$-28F    00DF9751     8BEC                      MOV EBP,ESP
$-28D    00DF9753     6A FF                     PUSH -0x1
$-28B    00DF9755     68 68A44C01               PUSH PathOfEx.014CA468
$-286    00DF975A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-280    00DF9760     50                        PUSH EAX
$-27F    00DF9761     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-278    00DF9768     83EC 58                   SUB ESP,0x58
$-275    00DF976B     53                        PUSH EBX
$-274    00DF976C     8B5D 08                   MOV EBX,DWORD PTR SS:[EBP+0x8]
$-271    00DF976F     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
$-26E    00DF9772     56                        PUSH ESI
$-26D    00DF9773     8B71 64                   MOV ESI,DWORD PTR DS:[ECX+0x64]
$-26A    00DF9776     8BD6                      MOV EDX,ESI
$-268    00DF9778     57                        PUSH EDI
$-267    00DF9779     8D79 60                   LEA EDI,DWORD PTR DS:[ECX+0x60]
$-264    00DF977C     895D 08                   MOV DWORD PTR SS:[EBP+0x8],EBX
$-261    00DF977F     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$-25F    00DF9781     50                        PUSH EAX
$-25E    00DF9782     E8 D9250000               CALL PathOfEx.00DFBD60
$-259    00DF9787     83C4 04                   ADD ESP,0x4
$-256    00DF978A     3BC6                      CMP EAX,ESI
$-254    00DF978C     75 68                     JNZ SHORT PathOfEx.00DF97F6
$-252    00DF978E     FF75 14                   PUSH DWORD PTR SS:[EBP+0x14]
$-24F    00DF9791     8D4D 9C                   LEA ECX,DWORD PTR SS:[EBP-0x64]
$-24C    00DF9794     FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
$-249    00DF9797     53                        PUSH EBX
$-248    00DF9798     E8 73A6FFFF               CALL PathOfEx.00DF3E10
$-243    00DF979D     8BF0                      MOV ESI,EAX
$-241    00DF979F     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-23A    00DF97A6     8B4F 04                   MOV ECX,DWORD PTR DS:[EDI+0x4]
$-237    00DF97A9     3B4F 08                   CMP ECX,DWORD PTR DS:[EDI+0x8]
$-234    00DF97AC     75 08                     JNZ SHORT PathOfEx.00DF97B6
$-232    00DF97AE     51                        PUSH ECX
$-231    00DF97AF     8BCF                      MOV ECX,EDI
$-22F    00DF97B1     E8 9A1A0000               CALL PathOfEx.00DFB250
$-22A    00DF97B6     8B4F 04                   MOV ECX,DWORD PTR DS:[EDI+0x4]
$-227    00DF97B9     85C9                      TEST ECX,ECX
$-225    00DF97BB     74 06                     JE SHORT PathOfEx.00DF97C3
$-223    00DF97BD     56                        PUSH ESI
$-222    00DF97BE     E8 3D2F0000               CALL PathOfEx.00DFC700
$-21D    00DF97C3     8347 04 58                ADD DWORD PTR DS:[EDI+0x4],0x58
$-219    00DF97C7     8D4D 9C                   LEA ECX,DWORD PTR SS:[EBP-0x64]
$-216    00DF97CA     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
$-20F    00DF97D1     E8 5A000000               CALL PathOfEx.00DF9830
$-20A    00DF97D6     8B4F 04                   MOV ECX,DWORD PTR DS:[EDI+0x4]
$-207    00DF97D9     B8 E9A28B2E               MOV EAX,0x2E8BA2E9
$-202    00DF97DE     2B0F                      SUB ECX,DWORD PTR DS:[EDI]
$-200    00DF97E0     F7E9                      IMUL ECX
$-1FE    00DF97E2     C1FA 04                   SAR EDX,0x4
$-1FB    00DF97E5     8BC2                      MOV EAX,EDX
$-1F9    00DF97E7     C1E8 1F                   SHR EAX,0x1F
$-1F6    00DF97EA     03C2                      ADD EAX,EDX
$-1F4    00DF97EC     6BC8 58                   IMUL ECX,EAX,0x58
$-1F1    00DF97EF     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-1EF    00DF97F1     83C0 A8                   ADD EAX,-0x58
$-1EC    00DF97F4     03C1                      ADD EAX,ECX
$-1EA    00DF97F6     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
$-1E7    00DF97F9     8B70 34                   MOV ESI,DWORD PTR DS:[EAX+0x34]
$-1E4    00DF97FC     8D78 30                   LEA EDI,DWORD PTR DS:[EAX+0x30]
$-1E1    00DF97FF     8B5D 10                   MOV EBX,DWORD PTR SS:[EBP+0x10]
$-1DE    00DF9802     8BD6                      MOV EDX,ESI
$-1DC    00DF9804     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$-1DA    00DF9806     53                        PUSH EBX
$-1D9    00DF9807     E8 7462EDFF               CALL PathOfEx.00CCFA80
$-1D4    00DF980C     83C4 08                   ADD ESP,0x8
$-1D1    00DF980F     3BC6                      CMP EAX,ESI
$-1CF    00DF9811     75 08                     JNZ SHORT PathOfEx.00DF981B
$-1CD    00DF9813     53                        PUSH EBX
$-1CC    00DF9814     8BCF                      MOV ECX,EDI
$-1CA    00DF9816     E8 5541E5FF               CALL PathOfEx.00C4D970
$-1C5    00DF981B     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1C2    00DF981E     5F                        POP EDI
$-1C1    00DF981F     5E                        POP ESI
$-1C0    00DF9820     5B                        POP EBX
$-1BF    00DF9821     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-1B8    00DF9828     8BE5                      MOV ESP,EBP
$-1B6    00DF982A     5D                        POP EBP
$-1B5    00DF982B     C2 1000                   RETN 0x10
$-1B2    00DF982E     CC                        INT3
$-1B1    00DF982F     CC                        INT3
$-1B0    00DF9830     55                        PUSH EBP
$-1AF    00DF9831     8BEC                      MOV EBP,ESP
$-1AD    00DF9833     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-1A7    00DF9839     6A FF                     PUSH -0x1
$-1A5    00DF983B     68 5C964A01               PUSH PathOfEx.014A965C
$-1A0    00DF9840     50                        PUSH EAX
$-19F    00DF9841     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-198    00DF9848     53                        PUSH EBX
$-197    00DF9849     56                        PUSH ESI
$-196    00DF984A     57                        PUSH EDI
$-195    00DF984B     8BF1                      MOV ESI,ECX
$-193    00DF984D     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-18C    00DF9854     83CB FF                   OR EBX,0xFFFFFFFF
$-189    00DF9857     8B7E 54                   MOV EDI,DWORD PTR DS:[ESI+0x54]
$-186    00DF985A     85FF                      TEST EDI,EDI
$-184    00DF985C     74 1F                     JE SHORT PathOfEx.00DF987D
$-182    00DF985E     8BC3                      MOV EAX,EBX
$-180    00DF9860     F0:0FC147 04              LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$-17B    00DF9865     75 16                     JNZ SHORT PathOfEx.00DF987D
$-179    00DF9867     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-177    00DF9869     8BCF                      MOV ECX,EDI
$-175    00DF986B     FF10                      CALL DWORD PTR DS:[EAX]
$-173    00DF986D     8BC3                      MOV EAX,EBX
$-171    00DF986F     F0:0FC147 08              LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$-16C    00DF9874     75 07                     JNZ SHORT PathOfEx.00DF987D
$-16A    00DF9876     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-168    00DF9878     8BCF                      MOV ECX,EDI
$-166    00DF987A     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$-163    00DF987D     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
$-15C    00DF9884     8B7E 48                   MOV EDI,DWORD PTR DS:[ESI+0x48]
$-159    00DF9887     85FF                      TEST EDI,EDI
$-157    00DF9889     74 1E                     JE SHORT PathOfEx.00DF98A9
$-155    00DF988B     8BC3                      MOV EAX,EBX
$-153    00DF988D     F0:0FC147 04              LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$-14E    00DF9892     75 15                     JNZ SHORT PathOfEx.00DF98A9
$-14C    00DF9894     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-14A    00DF9896     8BCF                      MOV ECX,EDI
$-148    00DF9898     FF10                      CALL DWORD PTR DS:[EAX]
$-146    00DF989A     F0:0FC15F 08              LOCK XADD DWORD PTR DS:[EDI+0x8],EBX
$-141    00DF989F     4B                        DEC EBX
$-140    00DF98A0     75 07                     JNZ SHORT PathOfEx.00DF98A9
$-13E    00DF98A2     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-13C    00DF98A4     8BCF                      MOV ECX,EDI
$-13A    00DF98A6     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$-137    00DF98A9     8D4E 30                   LEA ECX,DWORD PTR DS:[ESI+0x30]
$-134    00DF98AC     E8 3FCBDCFF               CALL PathOfEx.00BC63F0
$-12F    00DF98B1     8B46 2C                   MOV EAX,DWORD PTR DS:[ESI+0x2C]
$-12C    00DF98B4     8D7E 18                   LEA EDI,DWORD PTR DS:[ESI+0x18]
$-129    00DF98B7     83F8 08                   CMP EAX,0x8
$-126    00DF98BA     72 0E                     JB SHORT PathOfEx.00DF98CA
$-124    00DF98BC     6A 02                     PUSH 0x2
$-122    00DF98BE     40                        INC EAX
$-121    00DF98BF     50                        PUSH EAX
$-120    00DF98C0     FF37                      PUSH DWORD PTR DS:[EDI]
$-11E    00DF98C2     E8 E9F7DBFF               CALL PathOfEx.00BB90B0
$-119    00DF98C7     83C4 0C                   ADD ESP,0xC
$-116    00DF98CA     C747 14 07000000          MOV DWORD PTR DS:[EDI+0x14],0x7
$-10F    00DF98D1     837F 14 08                CMP DWORD PTR DS:[EDI+0x14],0x8
$-10B    00DF98D5     C747 10 00000000          MOV DWORD PTR DS:[EDI+0x10],0x0
$-104    00DF98DC     72 02                     JB SHORT PathOfEx.00DF98E0
$-102    00DF98DE     8B3F                      MOV EDI,DWORD PTR DS:[EDI]
$-100    00DF98E0     33C0                      XOR EAX,EAX
$-FE     00DF98E2     66:8907                   MOV WORD PTR DS:[EDI],AX
$-FB     00DF98E5     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
$-F8     00DF98E8     83F8 08                   CMP EAX,0x8
$-F5     00DF98EB     72 0E                     JB SHORT PathOfEx.00DF98FB
$-F3     00DF98ED     6A 02                     PUSH 0x2
$-F1     00DF98EF     40                        INC EAX
$-F0     00DF98F0     50                        PUSH EAX
$-EF     00DF98F1     FF36                      PUSH DWORD PTR DS:[ESI]
$-ED     00DF98F3     E8 B8F7DBFF               CALL PathOfEx.00BB90B0
$-E8     00DF98F8     83C4 0C                   ADD ESP,0xC
$-E5     00DF98FB     C746 14 07000000          MOV DWORD PTR DS:[ESI+0x14],0x7
$-DE     00DF9902     837E 14 08                CMP DWORD PTR DS:[ESI+0x14],0x8
$-DA     00DF9906     C746 10 00000000          MOV DWORD PTR DS:[ESI+0x10],0x0
$-D3     00DF990D     72 18                     JB SHORT PathOfEx.00DF9927
$-D1     00DF990F     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$-CF     00DF9911     33C9                      XOR ECX,ECX
$-CD     00DF9913     66:8908                   MOV WORD PTR DS:[EAX],CX
$-CA     00DF9916     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-C7     00DF9919     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-C0     00DF9920     5F                        POP EDI
$-BF     00DF9921     5E                        POP ESI
$-BE     00DF9922     5B                        POP EBX
$-BD     00DF9923     8BE5                      MOV ESP,EBP
$-BB     00DF9925     5D                        POP EBP
$-BA     00DF9926     C3                        RETN
$-B9     00DF9927     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-B6     00DF992A     33C0                      XOR EAX,EAX
$-B4     00DF992C     5F                        POP EDI
$-B3     00DF992D     66:8906                   MOV WORD PTR DS:[ESI],AX
$-B0     00DF9930     5E                        POP ESI
$-AF     00DF9931     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-A8     00DF9938     5B                        POP EBX
$-A7     00DF9939     8BE5                      MOV ESP,EBP
$-A5     00DF993B     5D                        POP EBP
$-A4     00DF993C     C3                        RETN
$-A3     00DF993D     CC                        INT3
$-A2     00DF993E     CC                        INT3
$-A1     00DF993F     CC                        INT3
$-A0     00DF9940     55                        PUSH EBP
$-9F     00DF9941     8BEC                      MOV EBP,ESP
$-9D     00DF9943     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-97     00DF9949     6A FF                     PUSH -0x1
$-95     00DF994B     68 58984A01               PUSH PathOfEx.014A9858
$-90     00DF9950     50                        PUSH EAX
$-8F     00DF9951     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-88     00DF9958     83EC 18                   SUB ESP,0x18
$-85     00DF995B     56                        PUSH ESI
$-84     00DF995C     8BF1                      MOV ESI,ECX
$-82     00DF995E     807E 78 00                CMP BYTE PTR DS:[ESI+0x78],0x0
$-7E     00DF9962     75 60                     JNZ SHORT PathOfEx.00DF99C4
$-7C     00DF9964     BA DB010000               MOV EDX,0x1DB
$-77     00DF9969     C646 78 01                MOV BYTE PTR DS:[ESI+0x78],0x1
$-73     00DF996D     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$-70     00DF9970     E8 AB0A0600               CALL PathOfEx.00E5A420
$-6B     00DF9975     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-64     00DF997C     8B4E 6C                   MOV ECX,DWORD PTR DS:[ESI+0x6C]
$-61     00DF997F     81C1 A8090000             ADD ECX,0x9A8
$-5B     00DF9985     50                        PUSH EAX
$-5A     00DF9986     8B11                      MOV EDX,DWORD PTR DS:[ECX]
$-58     00DF9988     FF52 04                   CALL DWORD PTR DS:[EDX+0x4]
$-55     00DF998B     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
$-4E     00DF9992     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
$-4B     00DF9995     83F8 08                   CMP EAX,0x8
$-48     00DF9998     72 0F                     JB SHORT PathOfEx.00DF99A9
$-46     00DF999A     6A 02                     PUSH 0x2
$-44     00DF999C     40                        INC EAX
$-43     00DF999D     50                        PUSH EAX
$-42     00DF999E     FF75 DC                   PUSH DWORD PTR SS:[EBP-0x24]
$-3F     00DF99A1     E8 0AF7DBFF               CALL PathOfEx.00BB90B0
$-3A     00DF99A6     83C4 0C                   ADD ESP,0xC
$-37     00DF99A9     33C0                      XOR EAX,EAX
$-35     00DF99AB     C745 F0 07000000          MOV DWORD PTR SS:[EBP-0x10],0x7
$-2E     00DF99B2     8BCE                      MOV ECX,ESI
$-2C     00DF99B4     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
$-25     00DF99BB     66:8945 DC                MOV WORD PTR SS:[EBP-0x24],AX
$-21     00DF99BF     E8 1C000000               CALL PathOfEx.00DF99E0
$-1C     00DF99C4     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-19     00DF99C7     5E                        POP ESI
$-18     00DF99C8     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-11     00DF99CF     8BE5                      MOV ESP,EBP
$-F      00DF99D1     5D                        POP EBP
$-E      00DF99D2     C3                        RETN
$-D      00DF99D3     CC                        INT3
$-C      00DF99D4     CC                        INT3
$-B      00DF99D5     CC                        INT3
$-A      00DF99D6     CC                        INT3
$-9      00DF99D7     CC                        INT3
$-8      00DF99D8     CC                        INT3
$-7      00DF99D9     CC                        INT3
$-6      00DF99DA     CC                        INT3
$-5      00DF99DB     CC                        INT3
$-4      00DF99DC     CC                        INT3
$-3      00DF99DD     CC                        INT3
$-2      00DF99DE     CC                        INT3
$-1      00DF99DF     CC                        INT3
$ ==>    00DF99E0     57                        PUSH EDI                                        ; tag_点击选择网关的时候会断到
$+1      00DF99E1     8BF9                      MOV EDI,ECX                                     ; ecx=stCD_SceneLoginState*
$+3      00DF99E3     807F 70 00                CMP BYTE PTR DS:[EDI+0x70],0x0
$+7      00DF99E7     75 38                     JNZ SHORT PathOfEx.00DF9A21
$+9      00DF99E9     53                        PUSH EBX
$+A      00DF99EA     56                        PUSH ESI
$+B      00DF99EB     C647 70 01                MOV BYTE PTR DS:[EDI+0x70],0x1
$+F      00DF99EF     FF15 D8705101             CALL DWORD PTR DS:[<&KERNEL32.GetTickCount>]    ; kernel32.GetTickCount
$+15     00DF99F5     05 DC050000               ADD EAX,0x5DC
$+1A     00DF99FA     8947 74                   MOV DWORD PTR DS:[EDI+0x74],EAX
$+1D     00DF99FD     8B77 60                   MOV ESI,DWORD PTR DS:[EDI+0x60]
$+20     00DF9A00     8B5F 64                   MOV EBX,DWORD PTR DS:[EDI+0x64]
$+23     00DF9A03     83C6 58                   ADD ESI,0x58                                    ; g_so_login_state_gateway_info_size_
$+26     00DF9A06     3BF3                      CMP ESI,EBX
$+28     00DF9A08     74 15                     JE SHORT PathOfEx.00DF9A1F
$+2A     00DF9A0A     66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
$+30     00DF9A10     56                        PUSH ESI
$+31     00DF9A11     8BCF                      MOV ECX,EDI
$+33     00DF9A13     E8 18000000               CALL PathOfEx.00DF9A30
$+38     00DF9A18     83C6 58                   ADD ESI,0x58
$+3B     00DF9A1B     3BF3                      CMP ESI,EBX
$+3D     00DF9A1D   ^ 75 F1                     JNZ SHORT PathOfEx.00DF9A10
$+3F     00DF9A1F     5E                        POP ESI
$+40     00DF9A20     5B                        POP EBX
$+41     00DF9A21     5F                        POP EDI
$+42     00DF9A22     C3                        RETN
$+43     00DF9A23     CC                        INT3
$+44     00DF9A24     CC                        INT3
$+45     00DF9A25     CC                        INT3
$+46     00DF9A26     CC                        INT3
$+47     00DF9A27     CC                        INT3
$+48     00DF9A28     CC                        INT3
$+49     00DF9A29     CC                        INT3
$+4A     00DF9A2A     CC                        INT3
$+4B     00DF9A2B     CC                        INT3
$+4C     00DF9A2C     CC                        INT3
$+4D     00DF9A2D     CC                        INT3
$+4E     00DF9A2E     CC                        INT3
$+4F     00DF9A2F     CC                        INT3
$+50     00DF9A30     55                        PUSH EBP
$+51     00DF9A31     8BEC                      MOV EBP,ESP
$+53     00DF9A33     6A FF                     PUSH -0x1
$+55     00DF9A35     68 90A44C01               PUSH PathOfEx.014CA490
$+5A     00DF9A3A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+60     00DF9A40     50                        PUSH EAX
$+61     00DF9A41     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+68     00DF9A48     83EC 44                   SUB ESP,0x44
$+6B     00DF9A4B     53                        PUSH EBX
$+6C     00DF9A4C     56                        PUSH ESI
$+6D     00DF9A4D     8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
$+70     00DF9A50     57                        PUSH EDI
$+71     00DF9A51     894D E4                   MOV DWORD PTR SS:[EBP-0x1C],ECX
$+74     00DF9A54     807E 3C 00                CMP BYTE PTR DS:[ESI+0x3C],0x0
$+78     00DF9A58     0F85 F8000000             JNZ PathOfEx.00DF9B56
$+7E     00DF9A5E     837E 44 00                CMP DWORD PTR DS:[ESI+0x44],0x0
$+82     00DF9A62     8D46 44                   LEA EAX,DWORD PTR DS:[ESI+0x44]
$+85     00DF9A65     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
$+88     00DF9A68     0F85 E8000000             JNZ PathOfEx.00DF9B56
$+8E     00DF9A6E     837E 50 00                CMP DWORD PTR DS:[ESI+0x50],0x0
$+92     00DF9A72     0F85 DE000000             JNZ PathOfEx.00DF9B56
$+98     00DF9A78     8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
$+9B     00DF9A7B     3B46 34                   CMP EAX,DWORD PTR DS:[ESI+0x34]
$+9E     00DF9A7E     0F84 D2000000             JE PathOfEx.00DF9B56
$+A4     00DF9A84     8378 10 00                CMP DWORD PTR DS:[EAX+0x10],0x0
$+A8     00DF9A88     8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
$+AB     00DF9A8B     0F84 C5000000             JE PathOfEx.00DF9B56
$+B1     00DF9A91     837E 40 FF                CMP DWORD PTR DS:[ESI+0x40],-0x1
$+B5     00DF9A95     0F85 90000000             JNZ PathOfEx.00DF9B2B
$+BB     00DF9A9B     8BD0                      MOV EDX,EAX
$+BD     00DF9A9D     8D4D C8                   LEA ECX,DWORD PTR SS:[EBP-0x38]
$+C0     00DF9AA0     E8 4B970400               CALL PathOfEx.00E431F0
$+C5     00DF9AA5     8378 14 10                CMP DWORD PTR DS:[EAX+0x14],0x10
$+C9     00DF9AA9     72 02                     JB SHORT PathOfEx.00DF9AAD
$+CB     00DF9AAB     8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+CD     00DF9AAD     50                        PUSH EAX
$+CE     00DF9AAE     FF15 B4755101             CALL DWORD PTR DS:[<&WS2_32.#11>]               ; ws2_32.inet_addr
$+D4     00DF9AB4     8D4D C8                   LEA ECX,DWORD PTR SS:[EBP-0x38]
$+D7     00DF9AB7     8946 40                   MOV DWORD PTR DS:[ESI+0x40],EAX
$+DA     00DF9ABA     E8 61F1DBFF               CALL PathOfEx.00BB8C20
$+DF     00DF9ABF     837E 40 FF                CMP DWORD PTR DS:[ESI+0x40],-0x1
$+E3     00DF9AC3     75 66                     JNZ SHORT PathOfEx.00DF9B2B
$+E5     00DF9AC5     68 8C886301               PUSH PathOfEx.0163888C                          ; ASCII "20481"
$+EA     00DF9ACA     8D4D C8                   LEA ECX,DWORD PTR SS:[EBP-0x38]
$+ED     00DF9ACD     E8 8EF8DBFF               CALL PathOfEx.00BB9360
$+F2     00DF9AD2     8D55 C8                   LEA EDX,DWORD PTR SS:[EBP-0x38]
$+F5     00DF9AD5     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+FC     00DF9ADC     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
$+FF     00DF9ADF     E8 3C970400               CALL PathOfEx.00E43220
$+104    00DF9AE4     8B55 08                   MOV EDX,DWORD PTR SS:[EBP+0x8]
$+107    00DF9AE7     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
$+10A    00DF9AEA     50                        PUSH EAX
$+10B    00DF9AEB     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+10F    00DF9AEF     E8 9C140000               CALL PathOfEx.00DFAF90
$+114    00DF9AF4     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
$+117    00DF9AF7     83C4 04                   ADD ESP,0x4
$+11A    00DF9AFA     50                        PUSH EAX
$+11B    00DF9AFB     E8 50E3DBFF               CALL PathOfEx.00BB7E50
$+120    00DF9B00     8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
$+123    00DF9B03     E8 48D7DBFF               CALL PathOfEx.00BB7250
$+128    00DF9B08     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
$+12B    00DF9B0B     E8 D004DCFF               CALL PathOfEx.00BB9FE0
$+130    00DF9B10     8D4D C8                   LEA ECX,DWORD PTR SS:[EBP-0x38]
$+133    00DF9B13     E8 08F1DBFF               CALL PathOfEx.00BB8C20
$+138    00DF9B18     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+13B    00DF9B1B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+142    00DF9B22     5F                        POP EDI
$+143    00DF9B23     5E                        POP ESI
$+144    00DF9B24     5B                        POP EBX
$+145    00DF9B25     8BE5                      MOV ESP,EBP
$+147    00DF9B27     5D                        POP EBP
$+148    00DF9B28     C2 0400                   RETN 0x4
$+14B    00DF9B2B     83EC 08                   SUB ESP,0x8
$+14E    00DF9B2E     8D56 40                   LEA EDX,DWORD PTR DS:[ESI+0x40]
$+151    00DF9B31     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
$+154    00DF9B34     E8 F7140000               CALL PathOfEx.00DFB030
$+159    00DF9B39     83C4 08                   ADD ESP,0x8
$+15C    00DF9B3C     8D4E 50                   LEA ECX,DWORD PTR DS:[ESI+0x50]
$+15F    00DF9B3F     50                        PUSH EAX
$+160    00DF9B40     E8 0BE3DBFF               CALL PathOfEx.00BB7E50
$+165    00DF9B45     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
$+168    00DF9B48     E8 03D7DBFF               CALL PathOfEx.00BB7250
$+16D    00DF9B4D     8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
$+170    00DF9B50     56                        PUSH ESI
$+171    00DF9B51     E8 DA010000               CALL PathOfEx.00DF9D30
$+176    00DF9B56     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+179    00DF9B59     5F                        POP EDI
$+17A    00DF9B5A     5E                        POP ESI
$+17B    00DF9B5B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+182    00DF9B62     5B                        POP EBX
$+183    00DF9B63     8BE5                      MOV ESP,EBP
$+185    00DF9B65     5D                        POP EBP
$+186    00DF9B66     C2 0400                   RETN 0x4
$+189    00DF9B69     CC                        INT3
*/
//tag_构建拿起物品发包
/*
$-191    011F625F     CC                 INT3
$-190    011F6260     51                 PUSH ECX
$-18F    011F6261     53                 PUSH EBX
$-18E    011F6262     56                 PUSH ESI
$-18D    011F6263     57                 PUSH EDI
$-18C    011F6264     8BF9               MOV EDI,ECX
$-18A    011F6266     0FB747 04          MOVZX EAX,WORD PTR DS:[EDI+0x4]
$-186    011F626A     50                 PUSH EAX
$-185    011F626B     FF15 B8754E01      CALL DWORD PTR DS:[<&WS2_32.#9>]         ; ws2_32.ntohs
$-17F    011F6271     8B5C24 14          MOV EBX,DWORD PTR SS:[ESP+0x14]
$-17B    011F6275     8BCB               MOV ECX,EBX
$-179    011F6277     0FB7C0             MOVZX EAX,AX
$-176    011F627A     894424 0C          MOV DWORD PTR SS:[ESP+0xC],EAX
$-172    011F627E     8D4424 0C          LEA EAX,DWORD PTR SS:[ESP+0xC]
$-16E    011F6282     6A 02              PUSH 0x2
$-16C    011F6284     50                 PUSH EAX
$-16B    011F6285     E8 76C1CAFF        CALL PathOfEx.00EA2400
$-166    011F628A     8B73 58            MOV ESI,DWORD PTR DS:[EBX+0x58]
$-163    011F628D     8BCE               MOV ECX,ESI
$-161    011F628F     FF77 08            PUSH DWORD PTR DS:[EDI+0x8]
$-15E    011F6292     E8 E9010000        CALL PathOfEx.011F6480
$-159    011F6297     50                 PUSH EAX
$-158    011F6298     8BCE               MOV ECX,ESI
$-156    011F629A     E8 71020000        CALL PathOfEx.011F6510
$-151    011F629F     50                 PUSH EAX
$-150    011F62A0     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$-14A    011F62A6     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$-146    011F62AA     8BCB               MOV ECX,EBX
$-144    011F62AC     6A 04              PUSH 0x4
$-142    011F62AE     8D4424 18          LEA EAX,DWORD PTR SS:[ESP+0x18]
$-13E    011F62B2     50                 PUSH EAX
$-13D    011F62B3     E8 48C1CAFF        CALL PathOfEx.00EA2400
$-138    011F62B8     5F                 POP EDI
$-137    011F62B9     5E                 POP ESI
$-136    011F62BA     5B                 POP EBX
$-135    011F62BB     59                 POP ECX
$-134    011F62BC     C2 0400            RETN 0x4
$-131    011F62BF     CC                 INT3
$-130    011F62C0     51                 PUSH ECX
$-12F    011F62C1     53                 PUSH EBX
$-12E    011F62C2     56                 PUSH ESI
$-12D    011F62C3     57                 PUSH EDI
$-12C    011F62C4     8BF9               MOV EDI,ECX
$-12A    011F62C6     0FB747 04          MOVZX EAX,WORD PTR DS:[EDI+0x4]
$-126    011F62CA     50                 PUSH EAX
$-125    011F62CB     FF15 B8754E01      CALL DWORD PTR DS:[<&WS2_32.#9>]         ; ws2_32.ntohs
$-11F    011F62D1     8B5C24 14          MOV EBX,DWORD PTR SS:[ESP+0x14]
$-11B    011F62D5     8BCB               MOV ECX,EBX
$-119    011F62D7     0FB7C0             MOVZX EAX,AX
$-116    011F62DA     894424 0C          MOV DWORD PTR SS:[ESP+0xC],EAX
$-112    011F62DE     8D4424 0C          LEA EAX,DWORD PTR SS:[ESP+0xC]
$-10E    011F62E2     6A 02              PUSH 0x2
$-10C    011F62E4     50                 PUSH EAX
$-10B    011F62E5     E8 16C1CAFF        CALL PathOfEx.00EA2400
$-106    011F62EA     8B73 58            MOV ESI,DWORD PTR DS:[EBX+0x58]
$-103    011F62ED     8BCE               MOV ECX,ESI
$-101    011F62EF     FF77 08            PUSH DWORD PTR DS:[EDI+0x8]
$-FE     011F62F2     E8 A9020000        CALL PathOfEx.011F65A0
$-F9     011F62F7     50                 PUSH EAX
$-F8     011F62F8     8BCE               MOV ECX,ESI
$-F6     011F62FA     E8 31030000        CALL PathOfEx.011F6630
$-F1     011F62FF     50                 PUSH EAX
$-F0     011F6300     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$-EA     011F6306     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$-E6     011F630A     8BCB               MOV ECX,EBX
$-E4     011F630C     6A 04              PUSH 0x4
$-E2     011F630E     8D4424 18          LEA EAX,DWORD PTR SS:[ESP+0x18]
$-DE     011F6312     50                 PUSH EAX
$-DD     011F6313     E8 E8C0CAFF        CALL PathOfEx.00EA2400
$-D8     011F6318     5F                 POP EDI
$-D7     011F6319     5E                 POP ESI
$-D6     011F631A     5B                 POP EBX
$-D5     011F631B     59                 POP ECX
$-D4     011F631C     C2 0400            RETN 0x4
$-D1     011F631F     CC                 INT3
$-D0     011F6320     56                 PUSH ESI
$-CF     011F6321     57                 PUSH EDI
$-CE     011F6322     8B7C24 0C          MOV EDI,DWORD PTR SS:[ESP+0xC]
$-CA     011F6326     8D71 08            LEA ESI,DWORD PTR DS:[ECX+0x8]
$-C7     011F6329     6A 04              PUSH 0x4
$-C5     011F632B     56                 PUSH ESI
$-C4     011F632C     8BCF               MOV ECX,EDI
$-C2     011F632E     E8 BDBFCAFF        CALL PathOfEx.00EA22F0
$-BD     011F6333     FF36               PUSH DWORD PTR DS:[ESI]
$-BB     011F6335     FF15 58754E01      CALL DWORD PTR DS:[<&WS2_32.#14>]        ; ws2_32.ntohl
$-B5     011F633B     8906               MOV DWORD PTR DS:[ESI],EAX
$-B3     011F633D     80BF B2000000 00   CMP BYTE PTR DS:[EDI+0xB2],0x0
$-AC     011F6344     75 12              JNZ SHORT PathOfEx.011F6358
$-AA     011F6346     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$-A7     011F6349     56                 PUSH ESI
$-A6     011F634A     E8 71030000        CALL PathOfEx.011F66C0
$-A1     011F634F     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$-9E     011F6352     56                 PUSH ESI
$-9D     011F6353     E8 08040000        CALL PathOfEx.011F6760
$-98     011F6358     5F                 POP EDI
$-97     011F6359     5E                 POP ESI
$-96     011F635A     C2 0400            RETN 0x4
$-93     011F635D     CC                 INT3
$-92     011F635E     CC                 INT3
$-91     011F635F     CC                 INT3
$-90     011F6360     51                 PUSH ECX
$-8F     011F6361     53                 PUSH EBX
$-8E     011F6362     8BD9               MOV EBX,ECX
$-8C     011F6364     57                 PUSH EDI
$-8B     011F6365     0FB743 04          MOVZX EAX,WORD PTR DS:[EBX+0x4]
$-87     011F6369     50                 PUSH EAX
$-86     011F636A     FF15 B8754E01      CALL DWORD PTR DS:[<&WS2_32.#9>]         ; ws2_32.ntohs
$-80     011F6370     8B7C24 10          MOV EDI,DWORD PTR SS:[ESP+0x10]
$-7C     011F6374     8BCF               MOV ECX,EDI
$-7A     011F6376     0FB7C0             MOVZX EAX,AX
$-77     011F6379     894424 08          MOV DWORD PTR SS:[ESP+0x8],EAX
$-73     011F637D     8D4424 08          LEA EAX,DWORD PTR SS:[ESP+0x8]
$-6F     011F6381     6A 02              PUSH 0x2
$-6D     011F6383     50                 PUSH EAX
$-6C     011F6384     E8 77C0CAFF        CALL PathOfEx.00EA2400
$-67     011F6389     FF73 08            PUSH DWORD PTR DS:[EBX+0x8]
$-64     011F638C     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$-61     011F638F     E8 6C040000        CALL PathOfEx.011F6800
$-5C     011F6394     50                 PUSH EAX
$-5B     011F6395     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$-55     011F639B     894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
$-51     011F639F     8BCF               MOV ECX,EDI
$-4F     011F63A1     6A 04              PUSH 0x4
$-4D     011F63A3     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$-49     011F63A7     50                 PUSH EAX
$-48     011F63A8     E8 53C0CAFF        CALL PathOfEx.00EA2400
$-43     011F63AD     FF73 0C            PUSH DWORD PTR DS:[EBX+0xC]
$-40     011F63B0     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$-3A     011F63B6     894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
$-36     011F63BA     8BCF               MOV ECX,EDI
$-34     011F63BC     6A 04              PUSH 0x4
$-32     011F63BE     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$-2E     011F63C2     50                 PUSH EAX
$-2D     011F63C3     E8 38C0CAFF        CALL PathOfEx.00EA2400
$-28     011F63C8     0FB643 10          MOVZX EAX,BYTE PTR DS:[EBX+0x10]
$-24     011F63CC     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$-21     011F63CF     50                 PUSH EAX
$-20     011F63D0     E8 BB040000        CALL PathOfEx.011F6890
$-1B     011F63D5     884424 10          MOV BYTE PTR SS:[ESP+0x10],AL
$-17     011F63D9     8BCF               MOV ECX,EDI
$-15     011F63DB     6A 01              PUSH 0x1
$-13     011F63DD     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$-F      011F63E1     50                 PUSH EAX
$-E      011F63E2     E8 19C0CAFF        CALL PathOfEx.00EA2400
$-9      011F63E7     5F                 POP EDI
$-8      011F63E8     5B                 POP EBX
$-7      011F63E9     59                 POP ECX
$-6      011F63EA     C2 0400            RETN 0x4
$-3      011F63ED     CC                 INT3
$-2      011F63EE     CC                 INT3
$-1      011F63EF     CC                 INT3
$ ==>    011F63F0     51                 PUSH ECX                                 ; tag_构建拿起物品发包
$+1      011F63F1     53                 PUSH EBX                                 ; ecx=stCDGS_TakeupItem*
$+2      011F63F2     8BD9               MOV EBX,ECX
$+4      011F63F4     57                 PUSH EDI
$+5      011F63F5     0FB743 04          MOVZX EAX,WORD PTR DS:[EBX+0x4]
$+9      011F63F9     50                 PUSH EAX
$+A      011F63FA     FF15 B8754E01      CALL DWORD PTR DS:[<&WS2_32.#9>]         ; ws2_32.ntohs
$+10     011F6400     8B7C24 10          MOV EDI,DWORD PTR SS:[ESP+0x10]
$+14     011F6404     8BCF               MOV ECX,EDI
$+16     011F6406     0FB7C0             MOVZX EAX,AX
$+19     011F6409     894424 08          MOV DWORD PTR SS:[ESP+0x8],EAX
$+1D     011F640D     8D4424 08          LEA EAX,DWORD PTR SS:[ESP+0x8]
$+21     011F6411     6A 02              PUSH 0x2
$+23     011F6413     50                 PUSH EAX
$+24     011F6414     E8 E7BFCAFF        CALL PathOfEx.00EA2400
$+29     011F6419     FF73 08            PUSH DWORD PTR DS:[EBX+0x8]
$+2C     011F641C     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$+2F     011F641F     E8 EC040000        CALL PathOfEx.011F6910
$+34     011F6424     50                 PUSH EAX
$+35     011F6425     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$+3B     011F642B     894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
$+3F     011F642F     8BCF               MOV ECX,EDI
$+41     011F6431     6A 04              PUSH 0x4
$+43     011F6433     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$+47     011F6437     50                 PUSH EAX
$+48     011F6438     E8 C3BFCAFF        CALL PathOfEx.00EA2400
$+4D     011F643D     FF73 0C            PUSH DWORD PTR DS:[EBX+0xC]
$+50     011F6440     FF15 54754E01      CALL DWORD PTR DS:[<&WS2_32.#8>]         ; ws2_32.ntohl
$+56     011F6446     894424 10          MOV DWORD PTR SS:[ESP+0x10],EAX
$+5A     011F644A     8BCF               MOV ECX,EDI
$+5C     011F644C     6A 04              PUSH 0x4
$+5E     011F644E     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$+62     011F6452     50                 PUSH EAX
$+63     011F6453     E8 A8BFCAFF        CALL PathOfEx.00EA2400
$+68     011F6458     0FB643 10          MOVZX EAX,BYTE PTR DS:[EBX+0x10]
$+6C     011F645C     8B4F 58            MOV ECX,DWORD PTR DS:[EDI+0x58]
$+6F     011F645F     50                 PUSH EAX
$+70     011F6460     E8 3B050000        CALL PathOfEx.011F69A0
$+75     011F6465     884424 10          MOV BYTE PTR SS:[ESP+0x10],AL
$+79     011F6469     8BCF               MOV ECX,EDI
$+7B     011F646B     6A 01              PUSH 0x1
$+7D     011F646D     8D4424 14          LEA EAX,DWORD PTR SS:[ESP+0x14]
$+81     011F6471     50                 PUSH EAX
$+82     011F6472     E8 89BFCAFF        CALL PathOfEx.00EA2400
$+87     011F6477     5F                 POP EDI
$+88     011F6478     5B                 POP EBX
$+89     011F6479     59                 POP ECX
$+8A     011F647A     C2 0400            RETN 0x4
$+8D     011F647D     CC                 INT3
$+8E     011F647E     CC                 INT3
$+8F     011F647F     CC                 INT3
$+90     011F6480     53                 PUSH EBX
$+91     011F6481     55                 PUSH EBP
$+92     011F6482     8B6C24 0C          MOV EBP,DWORD PTR SS:[ESP+0xC]
$+96     011F6486     56                 PUSH ESI
$+97     011F6487     57                 PUSH EDI
$+98     011F6488     8BF9               MOV EDI,ECX
$+9A     011F648A     BE 0E000000        MOV ESI,0xE
$+9F     011F648F     90                 NOP
$+A0     011F6490     8B9F 3C010000      MOV EBX,DWORD PTR DS:[EDI+0x13C]
$+A6     011F6496     B8 20000000        MOV EAX,0x20
$+AB     011F649B     2BC6               SUB EAX,ESI
$+AD     011F649D     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$+B1     011F64A1     3BD8               CMP EBX,EAX
$+B3     011F64A3     77 24              JA SHORT PathOfEx.011F64C9
$+B5     011F64A5     0FB697 47010000    MOVZX EDX,BYTE PTR DS:[EDI+0x147]
$+BC     011F64AC     8BCE               MOV ECX,ESI
$+BE     011F64AE     D3E2               SHL EDX,CL
$+C0     011F64B0     03F3               ADD ESI,EBX
$+C2     011F64B2     8BCF               MOV ECX,EDI
$+C4     011F64B4     0BEA               OR EBP,EDX
$+C6     011F64B6     E8 35C7F8FF        CALL PathOfEx.01182BF0
$+CB     011F64BB     83FE 20            CMP ESI,0x20
$+CE     011F64BE   ^ 75 D0              JNZ SHORT PathOfEx.011F6490
$+D0     011F64C0     5F                 POP EDI
$+D1     011F64C1     5E                 POP ESI
$+D2     011F64C2     8BC5               MOV EAX,EBP
$+D4     011F64C4     5D                 POP EBP
$+D5     011F64C5     5B                 POP EBX
$+D6     011F64C6     C2 0400            RETN 0x4
$+D9     011F64C9     8A9F 47010000      MOV BL,BYTE PTR DS:[EDI+0x147]
$+DF     011F64CF     8BC8               MOV ECX,EAX
$+E1     011F64D1     BA 01000000        MOV EDX,0x1
$+E6     011F64D6     0FB6C3             MOVZX EAX,BL
$+E9     011F64D9     D3E2               SHL EDX,CL
$+EB     011F64DB     8BCE               MOV ECX,ESI
$+ED     011F64DD     4A                 DEC EDX
$+EE     011F64DE     23D0               AND EDX,EAX
$+F0     011F64E0     8B4424 14          MOV EAX,DWORD PTR SS:[ESP+0x14]
$+F4     011F64E4     2987 3C010000      SUB DWORD PTR DS:[EDI+0x13C],EAX
$+FA     011F64EA     D3E2               SHL EDX,CL
$+FC     011F64EC     8AC8               MOV CL,AL
$+FE     011F64EE     D2EB               SHR BL,CL
$+100    011F64F0     0BEA               OR EBP,EDX
$+102    011F64F2     889F 47010000      MOV BYTE PTR DS:[EDI+0x147],BL
$+108    011F64F8     8BC5               MOV EAX,EBP
$+10A    011F64FA     5F                 POP EDI
$+10B    011F64FB     5E                 POP ESI
$+10C    011F64FC     5D                 POP EBP
$+10D    011F64FD     5B                 POP EBX
$+10E    011F64FE     C2 0400            RETN 0x4
$+111    011F6501     CC                 INT3
$+112    011F6502     CC                 INT3
$+113    011F6503     CC                 INT3
$+114    011F6504     CC                 INT3
$+115    011F6505     CC                 INT3
$+116    011F6506     CC                 INT3
$+117    011F6507     CC                 INT3
$+118    011F6508     CC                 INT3
$+119    011F6509     CC                 INT3
$+11A    011F650A     CC                 INT3
$+11B    011F650B     CC                 INT3
$+11C    011F650C     CC                 INT3
$+11D    011F650D     CC                 INT3
$+11E    011F650E     CC                 INT3
$+11F    011F650F     CC                 INT3
$+120    011F6510     53                 PUSH EBX
$+121    011F6511     55                 PUSH EBP
$+122    011F6512     8B6C24 0C          MOV EBP,DWORD PTR SS:[ESP+0xC]
$+126    011F6516     56                 PUSH ESI
$+127    011F6517     57                 PUSH EDI
$+128    011F6518     8BF9               MOV EDI,ECX
$+12A    011F651A     33F6               XOR ESI,ESI
$+12C    011F651C     0F1F40 00          NOP DWORD PTR DS:[EAX]
$+130    011F6520     8B9F 3C010000      MOV EBX,DWORD PTR DS:[EDI+0x13C]
$+136    011F6526     B8 0E000000        MOV EAX,0xE
$+13B    011F652B     2BC6               SUB EAX,ESI
$+13D    011F652D     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$+141    011F6531     3BD8               CMP EBX,EAX
$+143    011F6533     77 24              JA SHORT PathOfEx.011F6559
$+145    011F6535     0FB697 47010000    MOVZX EDX,BYTE PTR DS:[EDI+0x147]
$+14C    011F653C     8BCE               MOV ECX,ESI
$+14E    011F653E     D3E2               SHL EDX,CL
$+150    011F6540     03F3               ADD ESI,EBX
$+152    011F6542     8BCF               MOV ECX,EDI
$+154    011F6544     0BEA               OR EBP,EDX
$+156    011F6546     E8 A5C6F8FF        CALL PathOfEx.01182BF0
$+15B    011F654B     83FE 0E            CMP ESI,0xE
$+15E    011F654E   ^ 75 D0              JNZ SHORT PathOfEx.011F6520
$+160    011F6550     5F                 POP EDI
$+161    011F6551     5E                 POP ESI
$+162    011F6552     8BC5               MOV EAX,EBP
$+164    011F6554     5D                 POP EBP
$+165    011F6555     5B                 POP EBX
$+166    011F6556     C2 0400            RETN 0x4
$+169    011F6559     8A9F 47010000      MOV BL,BYTE PTR DS:[EDI+0x147]
$+16F    011F655F     8BC8               MOV ECX,EAX
$+171    011F6561     BA 01000000        MOV EDX,0x1
$+176    011F6566     0FB6C3             MOVZX EAX,BL
$+179    011F6569     D3E2               SHL EDX,CL
$+17B    011F656B     8BCE               MOV ECX,ESI
$+17D    011F656D     4A                 DEC EDX
$+17E    011F656E     23D0               AND EDX,EAX
$+180    011F6570     8B4424 14          MOV EAX,DWORD PTR SS:[ESP+0x14]
$+184    011F6574     2987 3C010000      SUB DWORD PTR DS:[EDI+0x13C],EAX
$+18A    011F657A     D3E2               SHL EDX,CL
$+18C    011F657C     8AC8               MOV CL,AL
$+18E    011F657E     D2EB               SHR BL,CL
$+190    011F6580     0BEA               OR EBP,EDX
$+192    011F6582     889F 47010000      MOV BYTE PTR DS:[EDI+0x147],BL
$+198    011F6588     8BC5               MOV EAX,EBP
$+19A    011F658A     5F                 POP EDI
$+19B    011F658B     5E                 POP ESI
$+19C    011F658C     5D                 POP EBP
$+19D    011F658D     5B                 POP EBX
$+19E    011F658E     C2 0400            RETN 0x4
$+1A1    011F6591     CC                 INT3
$+1A2    011F6592     CC                 INT3
$+1A3    011F6593     CC                 INT3
$+1A4    011F6594     CC                 INT3
$+1A5    011F6595     CC                 INT3
$+1A6    011F6596     CC                 INT3
$+1A7    011F6597     CC                 INT3
$+1A8    011F6598     CC                 INT3
$+1A9    011F6599     CC                 INT3
$+1AA    011F659A     CC                 INT3
$+1AB    011F659B     CC                 INT3
$+1AC    011F659C     CC                 INT3
$+1AD    011F659D     CC                 INT3
$+1AE    011F659E     CC                 INT3
$+1AF    011F659F     CC                 INT3
$+1B0    011F65A0     53                 PUSH EBX
$+1B1    011F65A1     55                 PUSH EBP
$+1B2    011F65A2     8B6C24 0C          MOV EBP,DWORD PTR SS:[ESP+0xC]
$+1B6    011F65A6     56                 PUSH ESI
$+1B7    011F65A7     57                 PUSH EDI
$+1B8    011F65A8     8BF9               MOV EDI,ECX
$+1BA    011F65AA     BE 09000000        MOV ESI,0x9
$+1BF    011F65AF     90                 NOP
$+1C0    011F65B0     8B9F 3C010000      MOV EBX,DWORD PTR DS:[EDI+0x13C]
$+1C6    011F65B6     B8 20000000        MOV EAX,0x20
$+1CB    011F65BB     2BC6               SUB EAX,ESI
$+1CD    011F65BD     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$+1D1    011F65C1     3BD8               CMP EBX,EAX
$+1D3    011F65C3     77 24              JA SHORT PathOfEx.011F65E9
$+1D5    011F65C5     0FB697 47010000    MOVZX EDX,BYTE PTR DS:[EDI+0x147]
$+1DC    011F65CC     8BCE               MOV ECX,ESI
$+1DE    011F65CE     D3E2               SHL EDX,CL
$+1E0    011F65D0     03F3               ADD ESI,EBX
$+1E2    011F65D2     8BCF               MOV ECX,EDI
$+1E4    011F65D4     0BEA               OR EBP,EDX
$+1E6    011F65D6     E8 15C6F8FF        CALL PathOfEx.01182BF0
$+1EB    011F65DB     83FE 20            CMP ESI,0x20
$+1EE    011F65DE   ^ 75 D0              JNZ SHORT PathOfEx.011F65B0
$+1F0    011F65E0     5F                 POP EDI
$+1F1    011F65E1     5E                 POP ESI
$+1F2    011F65E2     8BC5               MOV EAX,EBP
$+1F4    011F65E4     5D                 POP EBP
$+1F5    011F65E5     5B                 POP EBX
$+1F6    011F65E6     C2 0400            RETN 0x4
$+1F9    011F65E9     8A9F 47010000      MOV BL,BYTE PTR DS:[EDI+0x147]
$+1FF    011F65EF     8BC8               MOV ECX,EAX
$+201    011F65F1     BA 01000000        MOV EDX,0x1
$+206    011F65F6     0FB6C3             MOVZX EAX,BL
$+209    011F65F9     D3E2               SHL EDX,CL
$+20B    011F65FB     8BCE               MOV ECX,ESI
$+20D    011F65FD     4A                 DEC EDX
$+20E    011F65FE     23D0               AND EDX,EAX
$+210    011F6600     8B4424 14          MOV EAX,DWORD PTR SS:[ESP+0x14]
$+214    011F6604     2987 3C010000      SUB DWORD PTR DS:[EDI+0x13C],EAX
$+21A    011F660A     D3E2               SHL EDX,CL
$+21C    011F660C     8AC8               MOV CL,AL
$+21E    011F660E     D2EB               SHR BL,CL
$+220    011F6610     0BEA               OR EBP,EDX
$+222    011F6612     889F 47010000      MOV BYTE PTR DS:[EDI+0x147],BL
$+228    011F6618     8BC5               MOV EAX,EBP
$+22A    011F661A     5F                 POP EDI
$+22B    011F661B     5E                 POP ESI
$+22C    011F661C     5D                 POP EBP
$+22D    011F661D     5B                 POP EBX
$+22E    011F661E     C2 0400            RETN 0x4
$+231    011F6621     CC                 INT3
$+232    011F6622     CC                 INT3
$+233    011F6623     CC                 INT3
$+234    011F6624     CC                 INT3
$+235    011F6625     CC                 INT3
$+236    011F6626     CC                 INT3
$+237    011F6627     CC                 INT3
$+238    011F6628     CC                 INT3
$+239    011F6629     CC                 INT3
$+23A    011F662A     CC                 INT3
$+23B    011F662B     CC                 INT3
$+23C    011F662C     CC                 INT3
$+23D    011F662D     CC                 INT3
$+23E    011F662E     CC                 INT3
$+23F    011F662F     CC                 INT3
$+240    011F6630     53                 PUSH EBX
$+241    011F6631     55                 PUSH EBP
$+242    011F6632     8B6C24 0C          MOV EBP,DWORD PTR SS:[ESP+0xC]
$+246    011F6636     56                 PUSH ESI
$+247    011F6637     57                 PUSH EDI
$+248    011F6638     8BF9               MOV EDI,ECX
$+24A    011F663A     33F6               XOR ESI,ESI
$+24C    011F663C     0F1F40 00          NOP DWORD PTR DS:[EAX]
$+250    011F6640     8B9F 3C010000      MOV EBX,DWORD PTR DS:[EDI+0x13C]
$+256    011F6646     B8 09000000        MOV EAX,0x9
$+25B    011F664B     2BC6               SUB EAX,ESI
$+25D    011F664D     894424 14          MOV DWORD PTR SS:[ESP+0x14],EAX
$+261    011F6651     3BD8               CMP EBX,EAX
$+263    011F6653     77 24              JA SHORT PathOfEx.011F6679
$+265    011F6655     0FB697 47010000    MOVZX EDX,BYTE PTR DS:[EDI+0x147]
$+26C    011F665C     8BCE               MOV ECX,ESI
$+26E    011F665E     D3E2               SHL EDX,CL
$+270    011F6660     03F3               ADD ESI,EBX
$+272    011F6662     8BCF               MOV ECX,EDI
$+274    011F6664     0BEA               OR EBP,EDX
$+276    011F6666     E8 85C5F8FF        CALL PathOfEx.01182BF0
$+27B    011F666B     83FE 09            CMP ESI,0x9
$+27E    011F666E   ^ 75 D0              JNZ SHORT PathOfEx.011F6640
$+280    011F6670     5F                 POP EDI
$+281    011F6671     5E                 POP ESI
$+282    011F6672     8BC5               MOV EAX,EBP
$+284    011F6674     5D                 POP EBP
$+285    011F6675     5B                 POP EBX
$+286    011F6676     C2 0400            RETN 0x4
$+289    011F6679     8A9F 47010000      MOV BL,BYTE PTR DS:[EDI+0x147]
$+28F    011F667F     8BC8               MOV ECX,EAX
$+291    011F6681     BA 01000000        MOV EDX,0x1
$+296    011F6686     0FB6C3             MOVZX EAX,BL
$+299    011F6689     D3E2               SHL EDX,CL
$+29B    011F668B     8BCE               MOV ECX,ESI
$+29D    011F668D     4A                 DEC EDX
$+29E    011F668E     23D0               AND EDX,EAX
$+2A0    011F6690     8B4424 14          MOV EAX,DWORD PTR SS:[ESP+0x14]
$+2A4    011F6694     2987 3C010000      SUB DWORD PTR DS:[EDI+0x13C],EAX
$+2AA    011F669A     D3E2               SHL EDX,CL
$+2AC    011F669C     8AC8               MOV CL,AL
$+2AE    011F669E     D2EB               SHR BL,CL
$+2B0    011F66A0     0BEA               OR EBP,EDX
$+2B2    011F66A2     889F 47010000      MOV BYTE PTR DS:[EDI+0x147],BL
$+2B8    011F66A8     8BC5               MOV EAX,EBP
$+2BA    011F66AA     5F                 POP EDI
$+2BB    011F66AB     5E                 POP ESI
$+2BC    011F66AC     5D                 POP EBP
$+2BD    011F66AD     5B                 POP EBX
$+2BE    011F66AE     C2 0400            RETN 0x4
$+2C1    011F66B1     CC                 INT3
$+2C2    011F66B2     CC                 INT3
$+2C3    011F66B3     CC                 INT3
$+2C4    011F66B4     CC                 INT3
$+2C5    011F66B5     CC                 INT3
$+2C6    011F66B6     CC                 INT3
$+2C7    011F66B7     CC                 INT3
$+2C8    011F66B8     CC                 INT3
$+2C9    011F66B9     CC                 INT3
$+2CA    011F66BA     CC                 INT3
$+2CB    011F66BB     CC                 INT3
$+2CC    011F66BC     CC                 INT3
$+2CD    011F66BD     CC                 INT3
$+2CE    011F66BE     CC                 INT3
$+2CF    011F66BF     CC                 INT3
$+2D0    011F66C0     53                 PUSH EBX
$+2D1    011F66C1     8B5C24 08          MOV EBX,DWORD PTR SS:[ESP+0x8]
$+2D5    011F66C5     55                 PUSH EBP
$+2D6    011F66C6     56                 PUSH ESI
$+2D7    011F66C7     57                 PUSH EDI
$+2D8    011F66C8     8BE9               MOV EBP,ECX
$+2DA    011F66CA     BF 09000000        MOV EDI,0x9
$+2DF    011F66CF     90                 NOP
$+2E0    011F66D0     8BB5 40010000      MOV ESI,DWORD PTR SS:[EBP+0x140]
$+2E6    011F66D6     B8 20000000        MOV EAX,0x20
$+2EB    011F66DB     2BC7               SUB EAX,EDI
$+2ED    011F66DD     8BCF               MOV ECX,EDI
$+2EF    011F66DF     3BF0               CMP ESI,EAX
$+2F1    011F66E1     77 38              JA SHORT PathOfEx.011F671B
$+2F3    011F66E3     8B13               MOV EDX,DWORD PTR DS:[EBX]
$+2F5    011F66E5     B0 01              MOV AL,0x1
$+2F7    011F66E7     D3EA               SHR EDX,CL
$+2F9    011F66E9     03FE               ADD EDI,ESI
$+2FB    011F66EB     8BCE               MOV ECX,ESI
$+2FD    011F66ED     D2E0               SHL AL,CL
$+2FF    011F66EF     B9 08000000        MOV ECX,0x8
$+304    011F66F4     FEC8               DEC AL
$+306    011F66F6     2BCE               SUB ECX,ESI
$+308    011F66F8     22D0               AND DL,AL
$+30A    011F66FA     D2E2               SHL DL,CL
$+30C    011F66FC     8BCD               MOV ECX,EBP
$+30E    011F66FE     0895 48010000      OR BYTE PTR SS:[EBP+0x148],DL
$+314    011F6704     E8 67C5F8FF        CALL PathOfEx.01182C70
$+319    011F6709     83FF 20            CMP EDI,0x20
$+31C    011F670C   ^ 75 C2              JNZ SHORT PathOfEx.011F66D0
$+31E    011F670E     8123 FF010000      AND DWORD PTR DS:[EBX],0x1FF
$+324    011F6714     5F                 POP EDI
$+325    011F6715     5E                 POP ESI
$+326    011F6716     5D                 POP EBP
$+327    011F6717     5B                 POP EBX
$+328    011F6718     C2 0400            RETN 0x4
$+32B    011F671B     8B1B               MOV EBX,DWORD PTR DS:[EBX]
$+32D    011F671D     B2 01              MOV DL,0x1
$+32F    011F671F     D3EB               SHR EBX,CL
$+331    011F6721     8BC8               MOV ECX,EAX
$+333    011F6723     D2E2               SHL DL,CL
$+335    011F6725     B9 08000000        MOV ECX,0x8
$+33A    011F672A     2BCE               SUB ECX,ESI
$+33C    011F672C     FECA               DEC DL
$+33E    011F672E     2BF0               SUB ESI,EAX
$+340    011F6730     22DA               AND BL,DL
$+342    011F6732     8B4424 14          MOV EAX,DWORD PTR SS:[ESP+0x14]
$+346    011F6736     5F                 POP EDI
$+347    011F6737     89B5 40010000      MOV DWORD PTR SS:[EBP+0x140],ESI
$+34D    011F673D     D2E3               SHL BL,CL
$+34F    011F673F     089D 48010000      OR BYTE PTR SS:[EBP+0x148],BL
$+355    011F6745     8120 FF010000      AND DWORD PTR DS:[EAX],0x1FF
$+35B    011F674B     5E                 POP ESI
$+35C    011F674C     5D                 POP EBP
$+35D    011F674D     5B                 POP EBX
$+35E    011F674E     C2 0400            RETN 0x4
$+361    011F6751     CC                 INT3
*/