#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Task(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	//pt_char c_tmp;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-54     007F428F         CC               INT3
	$-53     007F4290         53               PUSH EBX
	$-52     007F4291         56               PUSH ESI
	$-51     007F4292         57               PUSH EDI
	$-50     007F4293         8BFA             MOV EDI, EDX
	$-4E     007F4295         8BF1             MOV ESI, ECX
	$-4C     007F4297         3BF7             CMP ESI, EDI
	$-4A     007F4299         74 2D            JE SHORT PathOfEx.007F42C8
	$-48     007F429B         8B5C24 10        MOV EBX, DWORD PTR SS:[ESP+0x10]
	$-44     007F429F         90               NOP
	$-43     007F42A0         8B0B             MOV ECX, DWORD PTR DS:[EBX]
	$-41     007F42A2         8B06             MOV EAX, DWORD PTR DS:[ESI]
	$-3F     007F42A4         8B51 44          MOV EDX, DWORD PTR DS:[ECX+0x44]
	$-3C     007F42A7         3D E1030000      CMP EAX, 0x3E1
	$-37     007F42AC         74 20            JE SHORT PathOfEx.007F42CE
	$-35     007F42AE         48               DEC EAX
	$-34     007F42AF         C1E2 07          SHL EDX, 0x7
	$-31     007F42B2         83C1 D0          ADD ECX, -0x30
	$-2E     007F42B5         50               PUSH EAX
	$-2D     007F42B6         03CA             ADD ECX, EDX
	$-2B     007F42B8         E8 331FD0FF      CALL PathOfEx.004F61F0
	$-26     007F42BD         84C0             TEST AL, AL
	$-24     007F42BF         74 0D            JE SHORT PathOfEx.007F42CE
	$-22     007F42C1         83C6 04          ADD ESI, 0x4
	$-1F     007F42C4         3BF7             CMP ESI, EDI
	$-1D     007F42C6       ^ 75 D8            JNZ SHORT PathOfEx.007F42A0
	$-1B     007F42C8         5F               POP EDI                                  ; PathOfEx.007F316F
	$-1A     007F42C9         5E               POP ESI                                  ; PathOfEx.007F316F
	$-19     007F42CA         B0 01            MOV AL, 0x1
	$-17     007F42CC         5B               POP EBX                                  ; PathOfEx.007F316F
	$-16     007F42CD         C3               RETN
	$-15     007F42CE         5F               POP EDI                                  ; PathOfEx.007F316F
	$-14     007F42CF         5E               POP ESI                                  ; PathOfEx.007F316F
	$-13     007F42D0         32C0             XOR AL, AL
	$-11     007F42D2         5B               POP EBX                                  ; PathOfEx.007F316F
	$-10     007F42D3         C3               RETN
	$-F      007F42D4         CC               INT3
	$-E      007F42D5         CC               INT3
	$-D      007F42D6         CC               INT3
	$-C      007F42D7         CC               INT3
	$-B      007F42D8         CC               INT3
	$-A      007F42D9         CC               INT3
	$-9      007F42DA         CC               INT3
	$-8      007F42DB         CC               INT3
	$-7      007F42DC         CC               INT3
	$-6      007F42DD         CC               INT3
	$-5      007F42DE         CC               INT3							;tag_根据ID判断任务状态
	$-4      007F42DF         CC               INT3                                     ; 当角色走到任务npc附近时，这个函数就一直会到
	$-3      007F42E0         53               PUSH EBX									; 此时的分支任务id是npc所关联的那个id，不是当前任务状态的id
	$-2      007F42E1         56               PUSH ESI
	$-1      007F42E2         57               PUSH EDI
	$ ==>    007F42E3         8BFA             MOV EDI, EDX                             ; edx跟ecx一样的类型，edx是个占位符吧
	$+2      007F42E5         8BF1             MOV ESI, ECX                             ; ecx保存的是分支任务id的指针
	$+4      007F42E7         3BF7             CMP ESI, EDI
	$+6      007F42E9         74 2D            JE SHORT PathOfEx.007F4318
	$+8      007F42EB         8B5C24 10        MOV EBX, DWORD PTR SS:[ESP+0x10]
	$+C      007F42EF         90               NOP
	$+D      007F42F0         8B0B             MOV ECX, DWORD PTR DS:[EBX]              ; 执行后，ecx==Player实体属性
	$+F      007F42F2         8B06             MOV EAX, DWORD PTR DS:[ESI]              ; 取branch_quest_id
	$+11     007F42F4         8B51 44          MOV EDX, DWORD PTR DS:[ECX+0x44]         ; 取地图难度等级
	$+14     007F42F7         3D E1030000      CMP EAX, 0x3E1
	$+19     007F42FC         74 13            JE SHORT PathOfEx.007F4311
	$+1B     007F42FE         48               DEC EAX                                  ; branch_quest_id-1
	$+1C     007F42FF         C1E2 07          SHL EDX, 0x7                             ; 难度等级*80
	$+1F     007F4302         83C1 D0          ADD ECX, -0x30                           ; 用于计算偏移的数据
	$+22     007F4305         50               PUSH EAX
	$+23     007F4306         03CA             ADD ECX, EDX
	$+25     007F4308         E8 E31ED0FF      CALL PathOfEx.004F61F0                   ; tag_计算分支任务状态
	$+2A     007F430D         84C0             TEST AL, AL
	$+2C     007F430F         75 0D            JNZ SHORT PathOfEx.007F431E              ; 不等于0则跳转
	$+2E     007F4311         83C6 04          ADD ESI, 0x4
	$+31     007F4314         3BF7             CMP ESI, EDI
	$+33     007F4316       ^ 75 D8            JNZ SHORT PathOfEx.007F42F0
	$+35     007F4318         5F               POP EDI                                  ; PathOfEx.007F316F
	$+36     007F4319         5E               POP ESI                                  ; PathOfEx.007F316F
	$+37     007F431A         B0 01            MOV AL, 0x1
	$+39     007F431C         5B               POP EBX                                  ; PathOfEx.007F316F
	$+3A     007F431D         C3               RETN
	$+3B     007F431E         5F               POP EDI                                  ; PathOfEx.007F316F
	$+3C     007F431F         5E               POP ESI                                  ; PathOfEx.007F316F
	$+3D     007F4320         32C0             XOR AL, AL
	$+3F     007F4322         5B               POP EBX                                  ; PathOfEx.007F316F
	$+40     007F4323         C3               RETN
	$+41     007F4324         CC               INT3

	//调用了上面的函数
	$-6B     007F30FF       CC                   INT3
	$-6A     007F3100       55                   PUSH EBP
	$-69     007F3101       8BEC                 MOV EBP, ESP
	$-67     007F3103       6A FF                PUSH -0x1
	$-65     007F3105       68 5E67D500          PUSH PathOfEx.00D5675E
	$-60     007F310A       64:A1 00000000       MOV EAX, DWORD PTR FS:[0]
	$-5A     007F3110       50                   PUSH EAX
	$-59     007F3111       64:8925 00000000     MOV DWORD PTR FS:[0], ESP
	$-52     007F3118       83EC 4C              SUB ESP, 0x4C
	$-4F     007F311B       C745 EC 00000000     MOV DWORD PTR SS:[EBP-0x14], 0x0
	$-48     007F3122       53                   PUSH EBX
	$-47     007F3123       8B5D 08              MOV EBX, DWORD PTR SS:[EBP+0x8]
	$-44     007F3126       894D E8              MOV DWORD PTR SS:[EBP-0x18], ECX
	$-41     007F3129       56                   PUSH ESI
	$-40     007F312A       57                   PUSH EDI
	$-3F     007F312B       8B4B 04              MOV ECX, DWORD PTR DS:[EBX+0x4]
	$-3C     007F312E       8B7D 0C              MOV EDI, DWORD PTR SS:[EBP+0xC]
	$-39     007F3131       897D 0C              MOV DWORD PTR SS:[EBP+0xC], EDI
	$-36     007F3134       8B41 14              MOV EAX, DWORD PTR DS:[ECX+0x14]
	$-33     007F3137       8B51 10              MOV EDX, DWORD PTR DS:[ECX+0x10]
	$-30     007F313A       8B49 14              MOV ECX, DWORD PTR DS:[ECX+0x14]
	$-2D     007F313D       8D1490               LEA EDX, DWORD PTR DS:[EAX+EDX*4]
	$-2A     007F3140       8D45 0C              LEA EAX, DWORD PTR SS:[EBP+0xC]
	$-27     007F3143       50                   PUSH EAX
	$-26     007F3144       E8 47110000          CALL PathOfEx.007F4290
	$-21     007F3149       83C4 04              ADD ESP, 0x4
	$-1E     007F314C       84C0                 TEST AL, AL
	$-1C     007F314E       0F84 95070000        JE PathOfEx.007F38E9
	$-16     007F3154       8B73 04              MOV ESI, DWORD PTR DS:[EBX+0x4]
	$-13     007F3157       897D 0C              MOV DWORD PTR SS:[EBP+0xC], EDI
	$-10     007F315A       8B4E 18              MOV ECX, DWORD PTR DS:[ESI+0x18]
	$-D      007F315D       8B46 1C              MOV EAX, DWORD PTR DS:[ESI+0x1C]
	$-A      007F3160       8D1488               LEA EDX, DWORD PTR DS:[EAX+ECX*4]
	$-7      007F3163       8B4E 1C              MOV ECX, DWORD PTR DS:[ESI+0x1C]
	$-4      007F3166       8D45 0C              LEA EAX, DWORD PTR SS:[EBP+0xC]
	$-1      007F3169       50                   PUSH EAX
	$ ==>    007F316A       E8 71110000          CALL PathOfEx.007F42E0                   ; 调用了上面的函数
	$+5      007F316F       83C4 04              ADD ESP, 0x4
	$+8      007F3172       84C0                 TEST AL, AL
	$+A      007F3174       0F84 6F070000        JE PathOfEx.007F38E9
	$+10     007F317A       8B47 04              MOV EAX, DWORD PTR DS:[EDI+0x4]
	$+13     007F317D       8D4D B8              LEA ECX, DWORD PTR SS:[EBP-0x48]
	$+16     007F3180       51                   PUSH ECX
	$+17     007F3181       8B40 10              MOV EAX, DWORD PTR DS:[EAX+0x10]
	$+1A     007F3184       8B48 08              MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+1D     007F3187       E8 947CCAFF          CALL PathOfEx.0049AE20
	$+22     007F318C       C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+29     007F3193       8B73 04              MOV ESI, DWORD PTR DS:[EBX+0x4]
	$+2C     007F3196       837E 40 00           CMP DWORD PTR DS:[ESI+0x40], 0x0
	$+30     007F319A       76 31                JBE SHORT PathOfEx.007F31CD
	$+32     007F319C       8B56 44              MOV EDX, DWORD PTR DS:[ESI+0x44]
	$+35     007F319F       8B4E 40              MOV ECX, DWORD PTR DS:[ESI+0x40]
	$+38     007F31A2       8D04CA               LEA EAX, DWORD PTR DS:[EDX+ECX*8]
	$+3B     007F31A5       3BD0                 CMP EDX, EAX
	$+3D     007F31A7       74 13                JE SHORT PathOfEx.007F31BC
	$+3F     007F31A9       8B4D BC              MOV ECX, DWORD PTR SS:[EBP-0x44]
	$+42     007F31AC       0F1F40 00            NOP DWORD PTR DS:[EAX]
	$+46     007F31B0       394A 04              CMP DWORD PTR DS:[EDX+0x4], ECX
	$+49     007F31B3       74 07                JE SHORT PathOfEx.007F31BC
	$+4B     007F31B5       83C2 08              ADD EDX, 0x8
	$+4E     007F31B8       3BD0                 CMP EDX, EAX
	$+50     007F31BA     ^ 75 F4                JNZ SHORT PathOfEx.007F31B0
	$+52     007F31BC       8B4E 40              MOV ECX, DWORD PTR DS:[ESI+0x40]
	$+55     007F31BF       8B46 44              MOV EAX, DWORD PTR DS:[ESI+0x44]
	$+58     007F31C2       8D04C8               LEA EAX, DWORD PTR DS:[EAX+ECX*8]
	$+5B     007F31C5       3BD0                 CMP EDX, EAX
	$+5D     007F31C7       0F84 22020000        JE PathOfEx.007F33EF
	$+63     007F31CD       8B56 5E              MOV EDX, DWORD PTR DS:[ESI+0x5E]
	$+66     007F31D0       8D45 0C              LEA EAX, DWORD PTR SS:[EBP+0xC]
	$+69     007F31D3       50                   PUSH EAX
	$+6A     007F31D4       8B46 62              MOV EAX, DWORD PTR DS:[ESI+0x62]
	$+6D     007F31D7       8BC8                 MOV ECX, EAX
	$+6F     007F31D9       897D 0C              MOV DWORD PTR SS:[EBP+0xC], EDI
	$+72     007F31DC       8D1490               LEA EDX, DWORD PTR DS:[EAX+EDX*4]
	$+75     007F31DF       E8 AC24FFFF          CALL PathOfEx.007E5690
	$+7A     007F31E4       83C4 04              ADD ESP, 0x4
	$+7D     007F31E7       84C0                 TEST AL, AL
	$+7F     007F31E9       0F84 00020000        JE PathOfEx.007F33EF
	$+85     007F31EF       8B56 72              MOV EDX, DWORD PTR DS:[ESI+0x72]
	$+88     007F31F2       8D45 0C              LEA EAX, DWORD PTR SS:[EBP+0xC]
	$+8B     007F31F5       50                   PUSH EAX
	$+8C     007F31F6       8B46 76              MOV EAX, DWORD PTR DS:[ESI+0x76]
	$+8F     007F31F9       8BC8                 MOV ECX, EAX
	$+91     007F31FB       897D 0C              MOV DWORD PTR SS:[EBP+0xC], EDI
	$+94     007F31FE       8D1490               LEA EDX, DWORD PTR DS:[EAX+EDX*4]
	$+97     007F3201       E8 2A110000          CALL PathOfEx.007F4330
	$+9C     007F3206       83C4 04              ADD ESP, 0x4
	$+9F     007F3209       84C0                 TEST AL, AL
	$+A1     007F320B       0F84 DE010000        JE PathOfEx.007F33EF
	$+A7     007F3211       8B45 10              MOV EAX, DWORD PTR SS:[EBP+0x10]
	$+AA     007F3214       3B46 6A              CMP EAX, DWORD PTR DS:[ESI+0x6A]
	$+AD     007F3217       0F82 D2010000        JB PathOfEx.007F33EF
	$+B3     007F321D       3B46 6E              CMP EAX, DWORD PTR DS:[ESI+0x6E]
	$+B6     007F3220       0F87 C9010000        JA PathOfEx.007F33EF
	$+BC     007F3226       8B96 83000000        MOV EDX, DWORD PTR DS:[ESI+0x83]
	$+C2     007F322C       8B4E 7F              MOV ECX, DWORD PTR DS:[ESI+0x7F]
	$+C5     007F322F       8955 0C              MOV DWORD PTR SS:[EBP+0xC], EDX
	$+C8     007F3232       8D04CA               LEA EAX, DWORD PTR DS:[EDX+ECX*8]
	$+CB     007F3235       3BD0                 CMP EDX, EAX
	$+CD     007F3237       0F84 76010000        JE PathOfEx.007F33B3
	$+D3     007F323D       0F1F00               NOP DWORD PTR DS:[EAX]
	$+D6     007F3240       8B32                 MOV ESI, DWORD PTR DS:[EDX]
	$+D8     007F3242       8975 DC              MOV DWORD PTR SS:[EBP-0x24], ESI
	$+DB     007F3245       8975 D0              MOV DWORD PTR SS:[EBP-0x30], ESI
	$+DE     007F3248       85F6                 TEST ESI, ESI
	$+E0     007F324A       74 14                JE SHORT PathOfEx.007F3260
	$+E2     007F324C       8D4E 38              LEA ECX, DWORD PTR DS:[ESI+0x38]
	$+E5     007F324F       B8 01000000          MOV EAX, 0x1
	$+EA     007F3254       F0:0FC101            LOCK XADD DWORD PTR DS:[ECX], EAX
	$+EE     007F3258       A1 0C47FD00          MOV EAX, DWORD PTR DS:[0xFD470C]
	$+F3     007F325D       8946 34              MOV DWORD PTR DS:[ESI+0x34], EAX
	$+F6     007F3260       8B5A 04              MOV EBX, DWORD PTR DS:[EDX+0x4]
	$+F9     007F3263       895D D4              MOV DWORD PTR SS:[EBP-0x2C], EBX
	$+FC     007F3266       8D45 C8              LEA EAX, DWORD PTR SS:[EBP-0x38]
	$+FF     007F3269       8BCB                 MOV ECX, EBX
	$+101    007F326B       50                   PUSH EAX
	$+102    007F326C       E8 CF75DDFF          CALL PathOfEx.005CA840
	$+107    007F3271       8B40 04              MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+10A    007F3274       8945 E4              MOV DWORD PTR SS:[EBP-0x1C], EAX
	$+10D    007F3277       C645 FC 02           MOV BYTE PTR SS:[EBP-0x4], 0x2
	$+111    007F327B       8B45 C8              MOV EAX, DWORD PTR SS:[EBP-0x38]
	$+114    007F327E       85C0                 TEST EAX, EAX
	$+116    007F3280       74 18                JE SHORT PathOfEx.007F329A
	$+118    007F3282       50                   PUSH EAX
	$+119    007F3283       51                   PUSH ECX
	$+11A    007F3284       E8 4732F2FF          CALL PathOfEx.007164D0
	$+11F    007F3289       83C4 04              ADD ESP, 0x4
	$+122    007F328C       8BC8                 MOV ECX, EAX
	$+124    007F328E       E8 CD2DF2FF          CALL PathOfEx.00716060
	$+129    007F3293       C745 C8 00000000     MOV DWORD PTR SS:[EBP-0x38], 0x0
	$+130    007F329A       837D E4 00           CMP DWORD PTR SS:[EBP-0x1C], 0x0
	$+134    007F329E       0F84 C8000000        JE PathOfEx.007F336C
	$+13A    007F32A4       8D45 C0              LEA EAX, DWORD PTR SS:[EBP-0x40]
	$+13D    007F32A7       8BCB                 MOV ECX, EBX
	$+13F    007F32A9       50                   PUSH EAX
	$+140    007F32AA       E8 9175DDFF          CALL PathOfEx.005CA840
	$+145    007F32AF       8B4B 28              MOV ECX, DWORD PTR DS:[EBX+0x28]
	$+148    007F32B2       894D E4              MOV DWORD PTR SS:[EBP-0x1C], ECX
	$+14B    007F32B5       8B40 04              MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+14E    007F32B8       8B40 04              MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+151    007F32BB       48                   DEC EAX
	$+152    007F32BC       8945 E0              MOV DWORD PTR SS:[EBP-0x20], EAX
	$+155    007F32BF       C645 FC 03           MOV BYTE PTR SS:[EBP-0x4], 0x3
	$+159    007F32C3       8B45 C0              MOV EAX, DWORD PTR SS:[EBP-0x40]         ; PathOfEx.00D23B7E
	$+15C    007F32C6       85C0                 TEST EAX, EAX
	$+15E    007F32C8       74 1B                JE SHORT PathOfEx.007F32E5
	$+160    007F32CA       50                   PUSH EAX
	$+161    007F32CB       51                   PUSH ECX
	$+162    007F32CC       E8 FF31F2FF          CALL PathOfEx.007164D0
	$+167    007F32D1       83C4 04              ADD ESP, 0x4
	$+16A    007F32D4       8BC8                 MOV ECX, EAX
	$+16C    007F32D6       E8 852DF2FF          CALL PathOfEx.00716060
	$+171    007F32DB       8B4D E4              MOV ECX, DWORD PTR SS:[EBP-0x1C]         ; PathOfEx.01151EF8
	$+174    007F32DE       C745 C0 00000000     MOV DWORD PTR SS:[EBP-0x40], 0x0
	$+17B    007F32E5       3B4D E0              CMP ECX, DWORD PTR SS:[EBP-0x20]         ; PathOfEx.01151E00
	$+17E    007F32E8       8BCB                 MOV ECX, EBX
	$+180    007F32EA       75 54                JNZ SHORT PathOfEx.007F3340
	$+182    007F32EC       8D45 B0              LEA EAX, DWORD PTR SS:[EBP-0x50]
	$+185    007F32EF       50                   PUSH EAX
	$+186    007F32F0       E8 4B75DDFF          CALL PathOfEx.005CA840
	$+18B    007F32F5       8B40 04              MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+18E    007F32F8       8B50 10              MOV EDX, DWORD PTR DS:[EAX+0x10]
	$+191    007F32FB       83FA 30              CMP EDX, 0x30
	$+194    007F32FE       74 20                JE SHORT PathOfEx.007F3320
	$+196    007F3300       4A                   DEC EDX
	$+197    007F3301       B8 01000000          MOV EAX, 0x1
	$+19C    007F3306       8BCA                 MOV ECX, EDX
	$+19E    007F3308       C1EA 03              SHR EDX, 0x3
	$+1A1    007F330B       83E1 07              AND ECX, 0x7
	$+1A4    007F330E       D3E0                 SHL EAX, CL
	$+1A6    007F3310       84843A FC010000      TEST BYTE PTR DS:[EDX+EDI+0x1FC], AL
	$+1AD    007F3317       74 07                JE SHORT PathOfEx.007F3320
	$+1AF    007F3319       B8 01000000          MOV EAX, 0x1
	$+1B4    007F331E       EB 02                JMP SHORT PathOfEx.007F3322
	$+1B6    007F3320       33C0                 XOR EAX, EAX
	$+1B8    007F3322       84C0                 TEST AL, AL
	$+1BA    007F3324       8D4D B0              LEA ECX, DWORD PTR SS:[EBP-0x50]
	$+1BD    007F3327       0F94C3               SETE BL
	$+1C0    007F332A       E8 31C9C7FF          CALL PathOfEx.0046FC60
	$+1C5    007F332F       84DB                 TEST BL, BL
	$+1C7    007F3331       74 39                JE SHORT PathOfEx.007F336C
	$+1C9    007F3333       8D4D D0              LEA ECX, DWORD PTR SS:[EBP-0x30]
	$+1CC    007F3336       E8 25C9C7FF          CALL PathOfEx.0046FC60
	$+1D1    007F333B       E9 AF000000          JMP PathOfEx.007F33EF
	$+1D6    007F3340       8D45 A8              LEA EAX, DWORD PTR SS:[EBP-0x58]
	$+1D9    007F3343       50                   PUSH EAX
	$+1DA    007F3344       E8 F774DDFF          CALL PathOfEx.005CA840
	$+1DF    007F3349       C645 FC 04           MOV BYTE PTR SS:[EBP-0x4], 0x4
	$+1E3    007F334D       8BCF                 MOV ECX, EDI
	$+1E5    007F334F       8B73 28              MOV ESI, DWORD PTR DS:[EBX+0x28]
	$+1E8    007F3352       50                   PUSH EAX
	$+1E9    007F3353       E8 F81BFFFF          CALL PathOfEx.007E4F50
	$+1EE    007F3358       3BC6                 CMP EAX, ESI
	$+1F0    007F335A       8D4D A8              LEA ECX, DWORD PTR SS:[EBP-0x58]
	$+1F3    007F335D       0F9EC3               SETLE BL
	$+1F6    007F3360       E8 FBC8C7FF          CALL PathOfEx.0046FC60
	$+1FB    007F3365       84DB                 TEST BL, BL
	$+1FD    007F3367     ^ 75 CA                JNZ SHORT PathOfEx.007F3333
	$+1FF    007F3369       8B75 DC              MOV ESI, DWORD PTR SS:[EBP-0x24]
	$+202    007F336C       C645 FC 05           MOV BYTE PTR SS:[EBP-0x4], 0x5
	$+206    007F3370       85F6                 TEST ESI, ESI
	$+208    007F3372       74 18                JE SHORT PathOfEx.007F338C
	$+20A    007F3374       56                   PUSH ESI
	$+20B    007F3375       51                   PUSH ECX
	$+20C    007F3376       E8 5531F2FF          CALL PathOfEx.007164D0
	$+211    007F337B       83C4 04              ADD ESP, 0x4
	$+214    007F337E       8BC8                 MOV ECX, EAX
	$+216    007F3380       E8 DB2CF2FF          CALL PathOfEx.00716060
	$+21B    007F3385       C745 D0 00000000     MOV DWORD PTR SS:[EBP-0x30], 0x0
	$+222    007F338C       C645 FC 00           MOV BYTE PTR SS:[EBP-0x4], 0x0
	$+226    007F3390       8B5D 08              MOV EBX, DWORD PTR SS:[EBP+0x8]
	$+229    007F3393       8B55 0C              MOV EDX, DWORD PTR SS:[EBP+0xC]
	$+22C    007F3396       83C2 08              ADD EDX, 0x8
	$+22F    007F3399       8955 0C              MOV DWORD PTR SS:[EBP+0xC], EDX
	$+232    007F339C       8B43 04              MOV EAX, DWORD PTR DS:[EBX+0x4]
	$+235    007F339F       8B48 7F              MOV ECX, DWORD PTR DS:[EAX+0x7F]
	$+238    007F33A2       8B80 83000000        MOV EAX, DWORD PTR DS:[EAX+0x83]
	$+23E    007F33A8       8D04C8               LEA EAX, DWORD PTR DS:[EAX+ECX*8]
	$+241    007F33AB       3BD0                 CMP EDX, EAX
	$+243    007F33AD     ^ 0F85 8DFEFFFF        JNZ PathOfEx.007F3240
	$+249    007F33B3       8B43 04              MOV EAX, DWORD PTR DS:[EBX+0x4]
	$+24C    007F33B6       8B40 30              MOV EAX, DWORD PTR DS:[EAX+0x30]         ; PathOfEx.00924037
	$+24F    007F33B9       48                   DEC EAX
	$+250    007F33BA       83F8 15              CMP EAX, 0x15
	$+253    007F33BD       0F87 1C050000        JA PathOfEx.007F38DF
	$+259    007F33C3       0FB680 30397F00      MOVZX EAX, BYTE PTR DS:[EAX+0x7F3930]
	$+260    007F33CA     - FF2485 00397F00      JMP NEAR DWORD PTR DS:[EAX*4+0x7F3900]
	$+267    007F33D1       8D45 A8              LEA EAX, DWORD PTR SS:[EBP-0x58]
	$+26A    007F33D4       8BCF                 MOV ECX, EDI
	$+26C    007F33D6       50                   PUSH EAX
	$+26D    007F33D7       E8 64A5E1FF          CALL PathOfEx.0060D940
	$+272    007F33DC       8D4D A8              LEA ECX, DWORD PTR SS:[EBP-0x58]
	$+275    007F33DF       8B70 04              MOV ESI, DWORD PTR DS:[EAX+0x4]
	$+278    007F33E2       E8 79C8C7FF          CALL PathOfEx.0046FC60
	$+27D    007F33E7       85F6                 TEST ESI, ESI
	$+27F    007F33E9       0F85 F0040000        JNZ PathOfEx.007F38DF
	$+285    007F33EF       33F6                 XOR ESI, ESI
	$+287    007F33F1       C745 FC 11000000     MOV DWORD PTR SS:[EBP-0x4], 0x11
	$+28E    007F33F8       8B45 B8              MOV EAX, DWORD PTR SS:[EBP-0x48]
	$+291    007F33FB       85C0                 TEST EAX, EAX
	$+293    007F33FD       74 11                JE SHORT PathOfEx.007F3410
	$+295    007F33FF       50                   PUSH EAX
	$+296    007F3400       51                   PUSH ECX
	$+297    007F3401       E8 CA30F2FF          CALL PathOfEx.007164D0
	$+29C    007F3406       83C4 04              ADD ESP, 0x4
	$+29F    007F3409       8BC8                 MOV ECX, EAX
	$+2A1    007F340B       E8 502CF2FF          CALL PathOfEx.00716060
	$+2A6    007F3410       8BC6                 MOV EAX, ESI
	$+2A8    007F3412       8B4D F4              MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+2AB    007F3415       64:890D 00000000     MOV DWORD PTR FS:[0], ECX
	$+2B2    007F341C       5F                   POP EDI                                  ; PathOfEx.007F316F
	$+2B3    007F341D       5E                   POP ESI                                  ; PathOfEx.007F316F
	$+2B4    007F341E       5B                   POP EBX                                  ; PathOfEx.007F316F
	$+2B5    007F341F       8BE5                 MOV ESP, EBP
	$+2B7    007F3421       5D                   POP EBP                                  ; PathOfEx.007F316F
	$+2B8    007F3422       C2 0C00              RETN 0xC
	*/

	//3.0以后就没有了，这个数据其实并未真正用到，所以不必更新了。tag_根据ID判断任务状态
/*
#ifndef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("8b??8b??3b??74??8b??????908b??8b??8b????3d", 0x1c, 2);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	{
		pt_char c_tmp = *((pt_char*)addr_tmp + 2);
		assert(c_tmp >= 6 && c_tmp <= 10);
		ofs << "pt_cint		g_v_branch_quest_shl_deta_ = " << (int)c_tmp << ";" << FC_OFstream_EndlLog;
		c_tmp = *((pt_char*)addr_tmp + 3 + 2);
		assert(c_tmp <= -0x20 && c_tmp >= -0x60);
		ofs << "pt_cint		g_v_branch_quest_delta_offset_ = " << (int)c_tmp << ";" << FC_OFstream_EndlLog;
	}
#endif*/

	/*
	$-6F9    006D070F     CC                          INT3
	$-6F8    006D0710     55                          PUSH EBP
	$-6F7    006D0711     8BEC                        MOV EBP, ESP
	$-6F5    006D0713     6A FF                       PUSH -0x1
	$-6F3    006D0715     68 9E24D400                 PUSH PathOfEx.00D4249E
	$-6EE    006D071A     64:A1 00000000              MOV EAX, DWORD PTR FS:[0]
	$-6E8    006D0720     50                          PUSH EAX
	$-6E7    006D0721     64:8925 00000000            MOV DWORD PTR FS:[0], ESP
	$-6E0    006D0728     83EC 50                     SUB ESP, 0x50
	$-6DD    006D072B     53                          PUSH EBX
	$-6DC    006D072C     33DB                        XOR EBX, EBX
	$-6DA    006D072E     56                          PUSH ESI
	$-6D9    006D072F     895D D8                     MOV DWORD PTR SS:[EBP-0x28], EBX
	$-6D6    006D0732     57                          PUSH EDI
	$-6D5    006D0733     8BF9                        MOV EDI, ECX
	$-6D3    006D0735     381D 14D91401               CMP BYTE PTR DS:[0x114D914], BL
	$-6CD    006D073B     0F85 AD000000               JNZ PathOfEx.006D07EE
	$-6C7    006D0741     8BB7 34010000               MOV ESI, DWORD PTR DS:[EDI+0x134]
	$-6C1    006D0747     8D45 F3                     LEA EAX, DWORD PTR SS:[EBP-0xD]
	$-6BE    006D074A     50                          PUSH EAX
	$-6BD    006D074B     68 FC49E900                 PUSH PathOfEx.00E949FC                          ; attack_in_place_key_stops_move
	$-6B8    006D0750     68 08DEE600                 PUSH PathOfEx.00E6DE08                          ; UI
	$-6B3    006D0755     8B8E 80030000               MOV ECX, DWORD PTR DS:[ESI+0x380]
	$-6AD    006D075B     885D F3                     MOV BYTE PTR SS:[EBP-0xD], BL
	$-6AA    006D075E     E8 FD5A0F00                 CALL PathOfEx.007C6260
	$-6A5    006D0763     385D F3                     CMP BYTE PTR SS:[EBP-0xD], BL
	$-6A2    006D0766     0F84 82000000               JE PathOfEx.006D07EE
	$-69C    006D076C     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-699    006D076F     895D E8                     MOV DWORD PTR SS:[EBP-0x18], EBX
	$-696    006D0772     50                          PUSH EAX
	$-695    006D0773     6A 0D                       PUSH 0xD
	$-693    006D0775     8BCE                        MOV ECX, ESI
	$-691    006D0777     E8 B4EC0E00                 CALL PathOfEx.007BF430
	$-68C    006D077C     8B75 E8                     MOV ESI, DWORD PTR SS:[EBP-0x18]
	$-689    006D077F     56                          PUSH ESI
	$-688    006D0780     FF15 38C4D800               CALL NEAR DWORD PTR DS:[0xD8C438]
	$-682    006D0786     0FB7C0                      MOVZX EAX, AX
	$-67F    006D0789     85F6                        TEST ESI, ESI
	$-67D    006D078B     74 61                       JE SHORT PathOfEx.006D07EE
	$-67B    006D078D     66:85C0                     TEST AX, AX
	$-678    006D0790     79 5C                       JNS SHORT PathOfEx.006D07EE
	$-676    006D0792     8B87 3C010000               MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$-670    006D0798     C605 14D91401 01            MOV BYTE PTR DS:[0x114D914], 0x1
	$-669    006D079F     85C0                        TEST EAX, EAX
	$-667    006D07A1     74 7B                       JE SHORT PathOfEx.006D081E
	$-665    006D07A3     8B80 943A0000               MOV EAX, DWORD PTR DS:[EAX+0x3A94]
	$-65F    006D07A9     8B88 C4010000               MOV ECX, DWORD PTR DS:[EAX+0x1C4]
	$-659    006D07AF     E8 DCD3DCFF                 CALL PathOfEx.0049DB90
	$-654    006D07B4     85C0                        TEST EAX, EAX
	$-652    006D07B6     74 66                       JE SHORT PathOfEx.006D081E
	$-650    006D07B8     8B48 30                     MOV ECX, DWORD PTR DS:[EAX+0x30]
	$-64D    006D07BB     85C9                        TEST ECX, ECX
	$-64B    006D07BD     74 5F                       JE SHORT PathOfEx.006D081E
	$-649    006D07BF     E8 1C414100                 CALL PathOfEx.00AE48E0
	$-644    006D07C4     8B0D 0CD70001               MOV ECX, DWORD PTR DS:[0x100D70C]
	$-63E    006D07CA     66:3BC1                     CMP AX, CX
	$-63B    006D07CD     75 4F                       JNZ SHORT PathOfEx.006D081E
	$-639    006D07CF     6A 01                       PUSH 0x1
	$-637    006D07D1     0FB7C1                      MOVZX EAX, CX
	$-634    006D07D4     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-62E    006D07DA     53                          PUSH EBX
	$-62D    006D07DB     50                          PUSH EAX
	$-62C    006D07DC     E8 FF4D2000                 CALL PathOfEx.008D55E0
	$-627    006D07E1     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-621    006D07E7     E8 24512000                 CALL PathOfEx.008D5910
	$-61C    006D07EC     EB 30                       JMP SHORT PathOfEx.006D081E
	$-61A    006D07EE     8B8F 34010000               MOV ECX, DWORD PTR DS:[EDI+0x134]
	$-614    006D07F4     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
	$-611    006D07F7     50                          PUSH EAX
	$-610    006D07F8     6A 0D                       PUSH 0xD
	$-60E    006D07FA     895D E8                     MOV DWORD PTR SS:[EBP-0x18], EBX
	$-60B    006D07FD     E8 2EEC0E00                 CALL PathOfEx.007BF430
	$-606    006D0802     8B75 E8                     MOV ESI, DWORD PTR SS:[EBP-0x18]
	$-603    006D0805     56                          PUSH ESI
	$-602    006D0806     FF15 38C4D800               CALL NEAR DWORD PTR DS:[0xD8C438]
	$-5FC    006D080C     0FB7C0                      MOVZX EAX, AX
	$-5F9    006D080F     85F6                        TEST ESI, ESI
	$-5F7    006D0811     74 05                       JE SHORT PathOfEx.006D0818
	$-5F5    006D0813     66:85C0                     TEST AX, AX
	$-5F2    006D0816     78 06                       JS SHORT PathOfEx.006D081E
	$-5F0    006D0818     881D 14D91401               MOV BYTE PTR DS:[0x114D914], BL
	$-5EA    006D081E     8B87 64150000               MOV EAX, DWORD PTR DS:[EDI+0x1564]              ; PathOfEx.00400000
	$-5E4    006D0824     3B87 68150000               CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-5DE    006D082A     75 13                       JNZ SHORT PathOfEx.006D083F
	$-5DC    006D082C     33C0                        XOR EAX, EAX
	$-5DA    006D082E     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-5D7    006D0831     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
	$-5D0    006D0838     5F                          POP EDI                                         ; 0018E5DC
	$-5CF    006D0839     5E                          POP ESI                                         ; 0018E5DC
	$-5CE    006D083A     5B                          POP EBX                                         ; 0018E5DC
	$-5CD    006D083B     8BE5                        MOV ESP, EBP
	$-5CB    006D083D     5D                          POP EBP                                         ; 0018E5DC
	$-5CA    006D083E     C3                          RETN
	$-5C9    006D083F     399F 780C0000               CMP DWORD PTR DS:[EDI+0xC78], EBX
	$-5C3    006D0845     75 28                       JNZ SHORT PathOfEx.006D086F
	$-5C1    006D0847     399F D8180000               CMP DWORD PTR DS:[EDI+0x18D8], EBX
	$-5BB    006D084D     74 20                       JE SHORT PathOfEx.006D086F
	$-5B9    006D084F     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-5B6    006D0852     E8 89CEDAFF                 CALL PathOfEx.0047D6E0
	$-5B1    006D0857     8D4D C0                     LEA ECX, DWORD PTR SS:[EBP-0x40]
	$-5AE    006D085A     51                          PUSH ECX
	$-5AD    006D085B     8B08                        MOV ECX, DWORD PTR DS:[EAX]                     ; PathOfEx.011370DC
	$-5AB    006D085D     E8 AE5BFDFF                 CALL PathOfEx.006A6410
	$-5A6    006D0862     BB 03000000                 MOV EBX, 0x3
	$-5A1    006D0867     C645 F3 00                  MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-59D    006D086B     3938                        CMP DWORD PTR DS:[EAX], EDI
	$-59B    006D086D     74 04                       JE SHORT PathOfEx.006D0873
	$-599    006D086F     C645 F3 01                  MOV BYTE PTR SS:[EBP-0xD], 0x1
	$-595    006D0873     83CE FF                     OR ESI, -0x1
	$-592    006D0876     F6C3 02                     TEST BL, 0x2
	$-58F    006D0879     74 39                       JE SHORT PathOfEx.006D08B4
	$-58D    006D087B     83E3 FD                     AND EBX, -0x3
	$-58A    006D087E     C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-583    006D0885     8B4D C4                     MOV ECX, DWORD PTR SS:[EBP-0x3C]
	$-580    006D0888     894D E0                     MOV DWORD PTR SS:[EBP-0x20], ECX
	$-57D    006D088B     85C9                        TEST ECX, ECX
	$-57B    006D088D     74 1E                       JE SHORT PathOfEx.006D08AD
	$-579    006D088F     8BC6                        MOV EAX, ESI
	$-577    006D0891     F0:0FC141 04                LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-572    006D0896     75 15                       JNZ SHORT PathOfEx.006D08AD
	$-570    006D0898     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-56E    006D089A     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-56C    006D089C     8B4D E0                     MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-569    006D089F     8BC6                        MOV EAX, ESI
	$-567    006D08A1     F0:0FC141 08                LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-562    006D08A6     75 05                       JNZ SHORT PathOfEx.006D08AD
	$-560    006D08A8     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-55E    006D08AA     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-55B    006D08AD     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-554    006D08B4     F6C3 01                     TEST BL, 0x1
	$-551    006D08B7     74 39                       JE SHORT PathOfEx.006D08F2
	$-54F    006D08B9     83E3 FE                     AND EBX, -0x2
	$-54C    006D08BC     C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4], 0x1
	$-545    006D08C3     8B4D D4                     MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$-542    006D08C6     894D E0                     MOV DWORD PTR SS:[EBP-0x20], ECX
	$-53F    006D08C9     85C9                        TEST ECX, ECX
	$-53D    006D08CB     74 1E                       JE SHORT PathOfEx.006D08EB
	$-53B    006D08CD     8BC6                        MOV EAX, ESI
	$-539    006D08CF     F0:0FC141 04                LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-534    006D08D4     75 15                       JNZ SHORT PathOfEx.006D08EB
	$-532    006D08D6     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-530    006D08D8     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-52E    006D08DA     8B4D E0                     MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-52B    006D08DD     8BC6                        MOV EAX, ESI
	$-529    006D08DF     F0:0FC141 08                LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-524    006D08E4     75 05                       JNZ SHORT PathOfEx.006D08EB
	$-522    006D08E6     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-520    006D08E8     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-51D    006D08EB     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-516    006D08F2     807D F3 00                  CMP BYTE PTR SS:[EBP-0xD], 0x0
	$-512    006D08F6   ^ 0F85 30FFFFFF               JNZ PathOfEx.006D082C
	$-50C    006D08FC     8B87 64150000               MOV EAX, DWORD PTR DS:[EDI+0x1564]              ; PathOfEx.00400000
	$-506    006D0902     3B87 68150000               CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-500    006D0908     75 18                       JNZ SHORT PathOfEx.006D0922
	$-4FE    006D090A     33C0                        XOR EAX, EAX
	$-4FC    006D090C     C745 D0 00000000            MOV DWORD PTR SS:[EBP-0x30], 0x0
	$-4F5    006D0913     C745 D4 00000000            MOV DWORD PTR SS:[EBP-0x2C], 0x0
	$-4EE    006D091A     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-4EB    006D091D     83CB 0C                     OR EBX, 0xC
	$-4E8    006D0920     EB 1A                       JMP SHORT PathOfEx.006D093C
	$-4E6    006D0922     8B87 68150000               MOV EAX, DWORD PTR DS:[EDI+0x1568]
	$-4E0    006D0928     8D4D B8                     LEA ECX, DWORD PTR SS:[EBP-0x48]
	$-4DD    006D092B     51                          PUSH ECX
	$-4DC    006D092C     8D48 F8                     LEA ECX, DWORD PTR DS:[EAX-0x8]
	$-4D9    006D092F     E8 9CE4EAFF                 CALL PathOfEx.0057EDD0
	$-4D4    006D0934     8BC8                        MOV ECX, EAX
	$-4D2    006D0936     83CB 10                     OR EBX, 0x10
	$-4CF    006D0939     8B45 BC                     MOV EAX, DWORD PTR SS:[EBP-0x44]
	$-4CC    006D093C     8945 E8                     MOV DWORD PTR SS:[EBP-0x18], EAX
	$-4C9    006D093F     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-4C7    006D0941     8945 DC                     MOV DWORD PTR SS:[EBP-0x24], EAX
	$-4C4    006D0944     8945 C0                     MOV DWORD PTR SS:[EBP-0x40], EAX
	$-4C1    006D0947     8B41 04                     MOV EAX, DWORD PTR DS:[ECX+0x4]
	$-4BE    006D094A     8945 D8                     MOV DWORD PTR SS:[EBP-0x28], EAX
	$-4BB    006D094D     8945 C4                     MOV DWORD PTR SS:[EBP-0x3C], EAX
	$-4B8    006D0950     C701 00000000               MOV DWORD PTR DS:[ECX], 0x0
	$-4B2    006D0956     C741 04 00000000            MOV DWORD PTR DS:[ECX+0x4], 0x0
	$-4AB    006D095D     C745 FC 02000000            MOV DWORD PTR SS:[EBP-0x4], 0x2
	$-4A4    006D0964     F6C3 10                     TEST BL, 0x10
	$-4A1    006D0967     74 33                       JE SHORT PathOfEx.006D099C
	$-49F    006D0969     83E3 EF                     AND EBX, -0x11
	$-49C    006D096C     C645 FC 03                  MOV BYTE PTR SS:[EBP-0x4], 0x3
	$-498    006D0970     8B4D BC                     MOV ECX, DWORD PTR SS:[EBP-0x44]
	$-495    006D0973     894D E0                     MOV DWORD PTR SS:[EBP-0x20], ECX
	$-492    006D0976     85C9                        TEST ECX, ECX
	$-490    006D0978     74 1E                       JE SHORT PathOfEx.006D0998
	$-48E    006D097A     8BC6                        MOV EAX, ESI
	$-48C    006D097C     F0:0FC141 04                LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-487    006D0981     75 15                       JNZ SHORT PathOfEx.006D0998
	$-485    006D0983     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-483    006D0985     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-481    006D0987     8B4D E0                     MOV ECX, DWORD PTR SS:[EBP-0x20]
	$-47E    006D098A     8BC6                        MOV EAX, ESI
	$-47C    006D098C     F0:0FC141 08                LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-477    006D0991     75 05                       JNZ SHORT PathOfEx.006D0998
	$-475    006D0993     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-473    006D0995     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-470    006D0998     C645 FC 02                  MOV BYTE PTR SS:[EBP-0x4], 0x2
	$-46C    006D099C     F6C3 08                     TEST BL, 0x8
	$-469    006D099F     74 30                       JE SHORT PathOfEx.006D09D1
	$-467    006D09A1     83E3 F7                     AND EBX, -0x9
	$-464    006D09A4     C645 FC 04                  MOV BYTE PTR SS:[EBP-0x4], 0x4
	$-460    006D09A8     8B4D D4                     MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$-45D    006D09AB     85C9                        TEST ECX, ECX
	$-45B    006D09AD     74 1E                       JE SHORT PathOfEx.006D09CD
	$-459    006D09AF     8BC6                        MOV EAX, ESI
	$-457    006D09B1     F0:0FC141 04                LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-452    006D09B6     75 15                       JNZ SHORT PathOfEx.006D09CD
	$-450    006D09B8     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-44E    006D09BA     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-44C    006D09BC     8B4D D4                     MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$-449    006D09BF     8BC6                        MOV EAX, ESI
	$-447    006D09C1     F0:0FC141 08                LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-442    006D09C6     75 05                       JNZ SHORT PathOfEx.006D09CD
	$-440    006D09C8     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-43E    006D09CA     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-43B    006D09CD     C645 FC 02                  MOV BYTE PTR SS:[EBP-0x4], 0x2
	$-437    006D09D1     F6C3 04                     TEST BL, 0x4
	$-434    006D09D4     74 2E                       JE SHORT PathOfEx.006D0A04
	$-432    006D09D6     C645 FC 05                  MOV BYTE PTR SS:[EBP-0x4], 0x5
	$-42E    006D09DA     8B5D E8                     MOV EBX, DWORD PTR SS:[EBP-0x18]
	$-42B    006D09DD     85DB                        TEST EBX, EBX
	$-429    006D09DF     74 1F                       JE SHORT PathOfEx.006D0A00
	$-427    006D09E1     8BC6                        MOV EAX, ESI
	$-425    006D09E3     F0:0FC143 04                LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$-420    006D09E8     75 16                       JNZ SHORT PathOfEx.006D0A00
	$-41E    006D09EA     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$-41C    006D09EC     8BCB                        MOV ECX, EBX
	$-41A    006D09EE     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-418    006D09F0     8BC6                        MOV EAX, ESI
	$-416    006D09F2     F0:0FC143 08                LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$-411    006D09F7     75 07                       JNZ SHORT PathOfEx.006D0A00
	$-40F    006D09F9     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$-40D    006D09FB     8BCB                        MOV ECX, EBX
	$-40B    006D09FD     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-408    006D0A00     C645 FC 02                  MOV BYTE PTR SS:[EBP-0x4], 0x2
	$-404    006D0A04     8B8F 34010000               MOV ECX, DWORD PTR DS:[EDI+0x134]
	$-3FE    006D0A0A     8D45 E0                     LEA EAX, DWORD PTR SS:[EBP-0x20]
	$-3FB    006D0A0D     50                          PUSH EAX
	$-3FA    006D0A0E     32DB                        XOR BL, BL
	$-3F8    006D0A10     C745 E0 00000000            MOV DWORD PTR SS:[EBP-0x20], 0x0
	$-3F1    006D0A17     6A 0D                       PUSH 0xD
	$-3EF    006D0A19     885D E8                     MOV BYTE PTR SS:[EBP-0x18], BL
	$-3EC    006D0A1C     E8 0FEA0E00                 CALL PathOfEx.007BF430
	$-3E7    006D0A21     8B75 E0                     MOV ESI, DWORD PTR SS:[EBP-0x20]
	$-3E4    006D0A24     56                          PUSH ESI
	$-3E3    006D0A25     FF15 38C4D800               CALL NEAR DWORD PTR DS:[0xD8C438]               ; call GetKeyState
	$-3DD    006D0A2B     0FB7C0                      MOVZX EAX, AX
	$-3DA    006D0A2E     85F6                        TEST ESI, ESI
	$-3D8    006D0A30     74 09                       JE SHORT PathOfEx.006D0A3B
	$-3D6    006D0A32     66:85C0                     TEST AX, AX
	$-3D3    006D0A35     79 04                       JNS SHORT PathOfEx.006D0A3B
	$-3D1    006D0A37     B7 01                       MOV BH, 0x1
	$-3CF    006D0A39     EB 02                       JMP SHORT PathOfEx.006D0A3D
	$-3CD    006D0A3B     32FF                        XOR BH, BH
	$-3CB    006D0A3D     8BB7 34010000               MOV ESI, DWORD PTR DS:[EDI+0x134]
	$-3C5    006D0A43     8D45 F3                     LEA EAX, DWORD PTR SS:[EBP-0xD]
	$-3C2    006D0A46     50                          PUSH EAX
	$-3C1    006D0A47     68 CC49E900                 PUSH PathOfEx.00E949CC                          ; always_attack_in_place
	$-3BC    006D0A4C     68 08DEE600                 PUSH PathOfEx.00E6DE08                          ; UI
	$-3B7    006D0A51     8B8E 80030000               MOV ECX, DWORD PTR DS:[ESI+0x380]
	$-3B1    006D0A57     C645 F3 00                  MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-3AD    006D0A5B     E8 00580F00                 CALL PathOfEx.007C6260
	$-3A8    006D0A60     807D F3 00                  CMP BYTE PTR SS:[EBP-0xD], 0x0
	$-3A4    006D0A64     75 04                       JNZ SHORT PathOfEx.006D0A6A
	$-3A2    006D0A66     84FF                        TEST BH, BH
	$-3A0    006D0A68     74 40                       JE SHORT PathOfEx.006D0AAA
	$-39E    006D0A6A     8B4D DC                     MOV ECX, DWORD PTR SS:[EBP-0x24]
	$-39B    006D0A6D     85C9                        TEST ECX, ECX
	$-399    006D0A6F     0F94C0                      SETE AL
	$-396    006D0A72     34 01                       XOR AL, 0x1
	$-394    006D0A74     74 34                       JE SHORT PathOfEx.006D0AAA
	$-392    006D0A76     0FB741 02                   MOVZX EAX, WORD PTR DS:[ECX+0x2]
	$-38E    006D0A7A     66:3B05 0CD70001            CMP AX, WORD PTR DS:[0x100D70C]
	$-387    006D0A81     0F94C2                      SETE DL
	$-384    006D0A84     66:3B05 1CD70001            CMP AX, WORD PTR DS:[0x100D71C]
	$-37D    006D0A8B     0F94C0                      SETE AL
	$-37A    006D0A8E     84D2                        TEST DL, DL
	$-378    006D0A90     74 13                       JE SHORT PathOfEx.006D0AA5
	$-376    006D0A92     84FF                        TEST BH, BH
	$-374    006D0A94     74 14                       JE SHORT PathOfEx.006D0AAA
	$-372    006D0A96     84C0                        TEST AL, AL
	$-370    006D0A98     75 0B                       JNZ SHORT PathOfEx.006D0AA5
	$-36E    006D0A9A     8BCE                        MOV ECX, ESI
	$-36C    006D0A9C     E8 5FDB0E00                 CALL PathOfEx.007BE600
	$-367    006D0AA1     84C0                        TEST AL, AL
	$-365    006D0AA3     74 05                       JE SHORT PathOfEx.006D0AAA
	$-363    006D0AA5     B3 01                       MOV BL, 0x1
	$-361    006D0AA7     885D E8                     MOV BYTE PTR SS:[EBP-0x18], BL
	$-35E    006D0AAA     6A 11                       PUSH 0x11
	$-35C    006D0AAC     FF15 38C4D800               CALL NEAR DWORD PTR DS:[0xD8C438]
	$-356    006D0AB2     0FB7C0                      MOVZX EAX, AX
	$-353    006D0AB5     66:85C0                     TEST AX, AX
	$-350    006D0AB8     79 06                       JNS SHORT PathOfEx.006D0AC0
	$-34E    006D0ABA     80CB 04                     OR BL, 0x4
	$-34B    006D0ABD     885D E8                     MOV BYTE PTR SS:[EBP-0x18], BL
	$-348    006D0AC0     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]              ; PathOfEx.00400000
	$-342    006D0AC6     8B80 000A0000               MOV EAX, DWORD PTR DS:[EAX+0xA00]
	$-33C    006D0ACC     85C0                        TEST EAX, EAX
	$-33A    006D0ACE     74 10                       JE SHORT PathOfEx.006D0AE0
	$-338    006D0AD0     8A80 54070000               MOV AL, BYTE PTR DS:[EAX+0x754]
	$-332    006D0AD6     84C0                        TEST AL, AL
	$-330    006D0AD8     74 06                       JE SHORT PathOfEx.006D0AE0
	$-32E    006D0ADA     80CB 02                     OR BL, 0x2
	$-32B    006D0ADD     885D E8                     MOV BYTE PTR SS:[EBP-0x18], BL
	$-328    006D0AE0     8B87 34010000               MOV EAX, DWORD PTR DS:[EDI+0x134]
	$-322    006D0AE6     8D4D F3                     LEA ECX, DWORD PTR SS:[EBP-0xD]
	$-31F    006D0AE9     51                          PUSH ECX
	$-31E    006D0AEA     68 2049E900                 PUSH PathOfEx.00E94920                          ; auto_equip
	$-319    006D0AEF     68 08DEE600                 PUSH PathOfEx.00E6DE08                          ; UI
	$-314    006D0AF4     8B88 80030000               MOV ECX, DWORD PTR DS:[EAX+0x380]               ; PathOfEx.00716797
	$-30E    006D0AFA     C645 F3 00                  MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-30A    006D0AFE     E8 5D570F00                 CALL PathOfEx.007C6260
	$-305    006D0B03     807D F3 00                  CMP BYTE PTR SS:[EBP-0xD], 0x0
	$-301    006D0B07     74 06                       JE SHORT PathOfEx.006D0B0F
	$-2FF    006D0B09     80CB 08                     OR BL, 0x8
	$-2FC    006D0B0C     885D E8                     MOV BYTE PTR SS:[EBP-0x18], BL
	$-2F9    006D0B0F     8B87 64150000               MOV EAX, DWORD PTR DS:[EDI+0x1564]              ; PathOfEx.00400000
	$-2F3    006D0B15     3B87 68150000               CMP EAX, DWORD PTR DS:[EDI+0x1568]
	$-2ED    006D0B1B     75 07                       JNZ SHORT PathOfEx.006D0B24                     ; 这里会到
	$-2EB    006D0B1D     B9 27000000                 MOV ECX, 0x27                                   ; 这里不会到
	$-2E6    006D0B22     EB 09                       JMP SHORT PathOfEx.006D0B2D
	$-2E4    006D0B24     8B87 68150000               MOV EAX, DWORD PTR DS:[EDI+0x1568]              ; 这里会到
	$-2DE    006D0B2A     8B48 F4                     MOV ECX, DWORD PTR DS:[EAX-0xC]
	$-2DB    006D0B2D     C745 C8 00000000            MOV DWORD PTR SS:[EBP-0x38], 0x0
	$-2D4    006D0B34     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34], 0x0
	$-2CD    006D0B3B     51                          PUSH ECX
	$-2CC    006D0B3C     8AC3                        MOV AL, BL
	$-2CA    006D0B3E     C645 FC 06                  MOV BYTE PTR SS:[EBP-0x4], 0x6
	$-2C6    006D0B42     24 01                       AND AL, 0x1
	$-2C4    006D0B44     0FB6C0                      MOVZX EAX, AL
	$-2C1    006D0B47     50                          PUSH EAX
	$-2C0    006D0B48     51                          PUSH ECX
	$-2BF    006D0B49     83EC 08                     SUB ESP, 0x8
	$-2BC    006D0B4C     8D45 C0                     LEA EAX, DWORD PTR SS:[EBP-0x40]
	$-2B9    006D0B4F     8BCC                        MOV ECX, ESP
	$-2B7    006D0B51     50                          PUSH EAX
	$-2B6    006D0B52     E8 593CDBFF                 CALL PathOfEx.004847B0
	$-2B1    006D0B57     8D45 A4                     LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$-2AE    006D0B5A     8BCF                        MOV ECX, EDI
	$-2AC    006D0B5C     50                          PUSH EAX
	$-2AB    006D0B5D     E8 CE0F0000                 CALL PathOfEx.006D1B30
	$-2A6    006D0B62     8D45 EC                     LEA EAX, DWORD PTR SS:[EBP-0x14]                ; 这里会到
	$-2A3    006D0B65     8945 B4                     MOV DWORD PTR SS:[EBP-0x4C], EAX
	$-2A0    006D0B68     8D4D B4                     LEA ECX, DWORD PTR SS:[EBP-0x4C]
	$-29D    006D0B6B     8D45 C8                     LEA EAX, DWORD PTR SS:[EBP-0x38]
	$-29A    006D0B6E     8945 B8                     MOV DWORD PTR SS:[EBP-0x48], EAX
	$-297    006D0B71     8D45 E4                     LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$-294    006D0B74     8945 BC                     MOV DWORD PTR SS:[EBP-0x44], EAX
	$-291    006D0B77     8B45 B0                     MOV EAX, DWORD PTR SS:[EBP-0x50]
	$-28E    006D0B7A     8945 E4                     MOV DWORD PTR SS:[EBP-0x1C], EAX
	$-28B    006D0B7D     8D45 A4                     LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$-288    006D0B80     50                          PUSH EAX
	$-287    006D0B81     E8 9A4A0000                 CALL PathOfEx.006D5620
	$-282    006D0B86     8B45 E4                     MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$-27F    006D0B89     85C0                        TEST EAX, EAX
	$-27D    006D0B8B     75 0B                       JNZ SHORT PathOfEx.006D0B98
	$-27B    006D0B8D     8D78 01                     LEA EDI, DWORD PTR DS:[EAX+0x1]
	$-278    006D0B90     83CE FF                     OR ESI, -0x1
	$-275    006D0B93     E9 8C030000                 JMP PathOfEx.006D0F24
	$-270    006D0B98     3B87 7C150000               CMP EAX, DWORD PTR DS:[EDI+0x157C]
	$-26A    006D0B9E     8D8F 74150000               LEA ECX, DWORD PTR DS:[EDI+0x1574]
	$-264    006D0BA4     8D45 B8                     LEA EAX, DWORD PTR SS:[EBP-0x48]
	$-261    006D0BA7     50                          PUSH EAX
	$-260    006D0BA8     0F9445 F2                   SETE BYTE PTR SS:[EBP-0xE]
	$-25C    006D0BAC     E8 1FE2EAFF                 CALL PathOfEx.0057EDD0
	$-257    006D0BB1     8B00                        MOV EAX, DWORD PTR DS:[EAX]                     ; PathOfEx.011370DC
	$-255    006D0BB3     3945 C8                     CMP DWORD PTR SS:[EBP-0x38], EAX
	$-252    006D0BB6     0F94C0                      SETE AL
	$-24F    006D0BB9     8845 F3                     MOV BYTE PTR SS:[EBP-0xD], AL
	$-24C    006D0BBC     C645 FC 08                  MOV BYTE PTR SS:[EBP-0x4], 0x8
	$-248    006D0BC0     83CE FF                     OR ESI, -0x1
	$-245    006D0BC3     8B4D BC                     MOV ECX, DWORD PTR SS:[EBP-0x44]
	$-242    006D0BC6     894D DC                     MOV DWORD PTR SS:[EBP-0x24], ECX
	$-23F    006D0BC9     85C9                        TEST ECX, ECX
	$-23D    006D0BCB     74 21                       JE SHORT PathOfEx.006D0BEE
	$-23B    006D0BCD     8BC6                        MOV EAX, ESI
	$-239    006D0BCF     F0:0FC141 04                LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
	$-234    006D0BD4     75 15                       JNZ SHORT PathOfEx.006D0BEB
	$-232    006D0BD6     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-230    006D0BD8     FF10                        CALL NEAR DWORD PTR DS:[EAX]                    ; PathOfEx.011370DC
	$-22E    006D0BDA     8B4D DC                     MOV ECX, DWORD PTR SS:[EBP-0x24]
	$-22B    006D0BDD     8BC6                        MOV EAX, ESI
	$-229    006D0BDF     F0:0FC141 08                LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
	$-224    006D0BE4     75 05                       JNZ SHORT PathOfEx.006D0BEB
	$-222    006D0BE6     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$-220    006D0BE8     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-21D    006D0BEB     8A45 F3                     MOV AL, BYTE PTR SS:[EBP-0xD]
	$-21A    006D0BEE     C645 FC 07                  MOV BYTE PTR SS:[EBP-0x4], 0x7
	$-216    006D0BF2     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$-213    006D0BF5     3B8F 70150000               CMP ECX, DWORD PTR DS:[EDI+0x1570]
	$-20D    006D0BFB     0FB6D3                      MOVZX EDX, BL
	$-20A    006D0BFE     8A5D F2                     MOV BL, BYTE PTR SS:[EBP-0xE]
	$-207    006D0C01     0F94C7                      SETE BH
	$-204    006D0C04     8955 DC                     MOV DWORD PTR SS:[EBP-0x24], EDX
	$-201    006D0C07     84DB                        TEST BL, BL
	$-1FF    006D0C09     74 1E                       JE SHORT PathOfEx.006D0C29
	$-1FD    006D0C0B     84C0                        TEST AL, AL
	$-1FB    006D0C0D     0F84 85000000               JE PathOfEx.006D0C98
	$-1F5    006D0C13     84FF                        TEST BH, BH
	$-1F3    006D0C15     74 12                       JE SHORT PathOfEx.006D0C29
	$-1F1    006D0C17     3B97 88150000               CMP EDX, DWORD PTR DS:[EDI+0x1588]
	$-1EB    006D0C1D     75 0A                       JNZ SHORT PathOfEx.006D0C29
	$-1E9    006D0C1F     BF 02000000                 MOV EDI, 0x2
	$-1E4    006D0C24     E9 FB020000                 JMP PathOfEx.006D0F24
	$-1DF    006D0C29     84C0                        TEST AL, AL
	$-1DD    006D0C2B     74 6B                       JE SHORT PathOfEx.006D0C98
	$-1DB    006D0C2D     8B55 C8                     MOV EDX, DWORD PTR SS:[EBP-0x38]                ; ntdll_1z.777C7500
	$-1D8    006D0C30     85D2                        TEST EDX, EDX
	$-1D6    006D0C32     0F94C0                      SETE AL
	$-1D3    006D0C35     34 01                       XOR AL, 0x1
	$-1D1    006D0C37     74 5F                       JE SHORT PathOfEx.006D0C98
	$-1CF    006D0C39     6A 3A                       PUSH 0x3A
	$-1CD    006D0C3B     8BCA                        MOV ECX, EDX
	$-1CB    006D0C3D     E8 EE281400                 CALL PathOfEx.00813530
	$-1C6    006D0C42     84C0                        TEST AL, AL
	$-1C4    006D0C44     74 4F                       JE SHORT PathOfEx.006D0C95
	$-1C2    006D0C46     84DB                        TEST BL, BL
	$-1C0    006D0C48     74 1E                       JE SHORT PathOfEx.006D0C68
	$-1BE    006D0C4A     84FF                        TEST BH, BH
	$-1BC    006D0C4C     74 0A                       JE SHORT PathOfEx.006D0C58
	$-1BA    006D0C4E     BF 02000000                 MOV EDI, 0x2
	$-1B5    006D0C53     E9 CC020000                 JMP PathOfEx.006D0F24
	$-1B0    006D0C58     837D E4 03                  CMP DWORD PTR SS:[EBP-0x1C], 0x3
	$-1AC    006D0C5C     75 37                       JNZ SHORT PathOfEx.006D0C95
	$-1AA    006D0C5E     BF 02000000                 MOV EDI, 0x2
	$-1A5    006D0C63     E9 BC020000                 JMP PathOfEx.006D0F24
	$-1A0    006D0C68     837D E4 03                  CMP DWORD PTR SS:[EBP-0x1C], 0x3
	$-19C    006D0C6C     75 27                       JNZ SHORT PathOfEx.006D0C95
	$-19A    006D0C6E     83BF 7C150000 04            CMP DWORD PTR DS:[EDI+0x157C], 0x4
	$-193    006D0C75     75 1E                       JNZ SHORT PathOfEx.006D0C95
	$-191    006D0C77     C787 70150000 00000000      MOV DWORD PTR DS:[EDI+0x1570], 0x0
	$-187    006D0C81     C787 7C150000 03000000      MOV DWORD PTR DS:[EDI+0x157C], 0x3
	$-17D    006D0C8B     BF 02000000                 MOV EDI, 0x2
	$-178    006D0C90     E9 8F020000                 JMP PathOfEx.006D0F24
	$-173    006D0C95     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]                ; ntdll_1z.777C7500
	$-170    006D0C98     398F BC180000               CMP DWORD PTR DS:[EDI+0x18BC], ECX
	$-16A    006D0C9E     74 0D                       JE SHORT PathOfEx.006D0CAD
	$-168    006D0CA0     6A 00                       PUSH 0x0
	$-166    006D0CA2     51                          PUSH ECX
	$-165    006D0CA3     8BCF                        MOV ECX, EDI
	$-163    006D0CA5     E8 56DDFFFF                 CALL PathOfEx.006CEA00
	$-15E    006D0CAA     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$-15B    006D0CAD     8B55 C8                     MOV EDX, DWORD PTR SS:[EBP-0x38]
	$-158    006D0CB0     85D2                        TEST EDX, EDX
	$-156    006D0CB2     0F95C0                      SETNE AL
	$-153    006D0CB5     84C0                        TEST AL, AL
	$-151    006D0CB7     74 1E                       JE SHORT PathOfEx.006D0CD7
	$-14F    006D0CB9     8B87 3C010000               MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$-149    006D0CBF     8A80 6B3B0000               MOV AL, BYTE PTR DS:[EAX+0x3B6B]
	$-143    006D0CC5     84C0                        TEST AL, AL
	$-141    006D0CC7     75 0E                       JNZ SHORT PathOfEx.006D0CD7
	$-13F    006D0CC9     FF75 E4                     PUSH DWORD PTR SS:[EBP-0x1C]
	$-13C    006D0CCC     8BCF                        MOV ECX, EDI
	$-13A    006D0CCE     52                          PUSH EDX
	$-139    006D0CCF     E8 CCF7FFFF                 CALL PathOfEx.006D04A0
	$-134    006D0CD4     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$-131    006D0CD7     F3:0F1087 A0180000          MOVSS XMM0, DWORD PTR DS:[EDI+0x18A0]
	$-129    006D0CDF     F3:0F5905 B0D7E600          MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$-121    006D0CE7     F3:0F2CC0                   CVTTSS2SI EAX, XMM0
	$-11D    006D0CEB     F3:0F1087 9C180000          MOVSS XMM0, DWORD PTR DS:[EDI+0x189C]
	$-115    006D0CF3     F3:0F5905 B0D7E600          MULSS XMM0, DWORD PTR DS:[0xE6D7B0]
	$-10D    006D0CFB     8945 E0                     MOV DWORD PTR SS:[EBP-0x20], EAX
	$-10A    006D0CFE     8945 D4                     MOV DWORD PTR SS:[EBP-0x2C], EAX
	$-107    006D0D01     8B45 E4                     MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$-104    006D0D04     F3:0F2CD8                   CVTTSS2SI EBX, XMM0
	$-100    006D0D08     48                          DEC EAX
	$-FF     006D0D09     895D D0                     MOV DWORD PTR SS:[EBP-0x30], EBX
	$-FC     006D0D0C     83F8 04                     CMP EAX, 0x4
	$-F9     006D0D0F     0F87 C9010000               JA PathOfEx.006D0EDE
	$-F3     006D0D15   - FF2485 B80F6D00             JMP NEAR DWORD PTR DS:[EAX*4+0x6D0FB8]
	$-EC     006D0D1C     FF75 E8                     PUSH DWORD PTR SS:[EBP-0x18]
	$-E9     006D0D1F     A1 0CD70001                 MOV EAX, DWORD PTR DS:[0x100D70C]
	$-E4     006D0D24     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-E1     006D0D27     51                          PUSH ECX
	$-E0     006D0D28     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-DA     006D0D2E     25 FF3F0000                 AND EAX, 0x3FFF
	$-D5     006D0D33     6A 00                       PUSH 0x0
	$-D3     006D0D35     50                          PUSH EAX
	$-D2     006D0D36     E8 D5482000                 CALL PathOfEx.008D5610
	$-CD     006D0D3B     E9 9E010000                 JMP PathOfEx.006D0EDE
	$-C8     006D0D40     83BF BC180000 00            CMP DWORD PTR DS:[EDI+0x18BC], 0x0
	$-C1     006D0D47     74 38                       JE SHORT PathOfEx.006D0D81
	$-BF     006D0D49     8D45 C8                     LEA EAX, DWORD PTR SS:[EBP-0x38]
	$-BC     006D0D4C     50                          PUSH EAX
	$-BB     006D0D4D     E8 5EE5FFFF                 CALL PathOfEx.006CF2B0
	$-B6     006D0D52     84C0                        TEST AL, AL
	$-B4     006D0D54     74 2B                       JE SHORT PathOfEx.006D0D81
	$-B2     006D0D56     FFB7 BC180000               PUSH DWORD PTR DS:[EDI+0x18BC]
	$-AC     006D0D5C     8BCF                        MOV ECX, EDI
	$-AA     006D0D5E     E8 4DDEFFFF                 CALL PathOfEx.006CEBB0
	$-A5     006D0D63     84C0                        TEST AL, AL
	$-A3     006D0D65     74 1A                       JE SHORT PathOfEx.006D0D81
	$-A1     006D0D67     8B87 BC180000               MOV EAX, DWORD PTR DS:[EDI+0x18BC]
	$-9B     006D0D6D     8B40 20                     MOV EAX, DWORD PTR DS:[EAX+0x20]
	$-98     006D0D70     8B58 10                     MOV EBX, DWORD PTR DS:[EAX+0x10]
	$-95     006D0D73     8B40 14                     MOV EAX, DWORD PTR DS:[EAX+0x14]
	$-92     006D0D76     895D D0                     MOV DWORD PTR SS:[EBP-0x30], EBX
	$-8F     006D0D79     8945 E0                     MOV DWORD PTR SS:[EBP-0x20], EAX
	$-8C     006D0D7C     8945 D4                     MOV DWORD PTR SS:[EBP-0x2C], EAX
	$-89     006D0D7F     EB 03                       JMP SHORT PathOfEx.006D0D84
	$-87     006D0D81     8B5D D0                     MOV EBX, DWORD PTR SS:[EBP-0x30]                ; nvd3dum.67BFFABE
	$-84     006D0D84     FF75 E8                     PUSH DWORD PTR SS:[EBP-0x18]
	$-81     006D0D87     8B45 C8                     MOV EAX, DWORD PTR SS:[EBP-0x38]                ; nvd3dum.67BFF867
	$-7E     006D0D8A     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
	$-7B     006D0D8D     51                          PUSH ECX
	$-7A     006D0D8E     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$-74     006D0D94     6A 00                       PUSH 0x0
	$-72     006D0D96     0FB700                      MOVZX EAX, WORD PTR DS:[EAX]
	$-6F     006D0D99     50                          PUSH EAX
	$-6E     006D0D9A     E8 71482000                 CALL PathOfEx.008D5610                          ; 使用方向性技能
	$-69     006D0D9F     E9 3D010000                 JMP PathOfEx.006D0EE1
	$-64     006D0DA4     85C9                        TEST ECX, ECX
	$-62     006D0DA6     0F84 32010000               JE PathOfEx.006D0EDE
	$-5C     006D0DAC     8B97 3C010000               MOV EDX, DWORD PTR DS:[EDI+0x13C]
	$-56     006D0DB2     8A82 AC3E0000               MOV AL, BYTE PTR DS:[EDX+0x3EAC]
	$-50     006D0DB8     84C0                        TEST AL, AL
	$-4E     006D0DBA     74 39                       JE SHORT PathOfEx.006D0DF5
	$-4C     006D0DBC     80BF D51A0000 00            CMP BYTE PTR DS:[EDI+0x1AD5], 0x0
	$-45     006D0DC3     74 30                       JE SHORT PathOfEx.006D0DF5
	$-43     006D0DC5     83BF D81A0000 00            CMP DWORD PTR DS:[EDI+0x1AD8], 0x0
	$-3C     006D0DCC     0F84 0C010000               JE PathOfEx.006D0EDE
	$-36     006D0DD2     51                          PUSH ECX
	$-35     006D0DD3     E8 68CBDFFF                 CALL PathOfEx.004CD940
	$-30     006D0DD8     84C0                        TEST AL, AL
	$-2E     006D0DDA     0F84 FE000000               JE PathOfEx.006D0EDE
	$-28     006D0DE0     8B8F D81A0000               MOV ECX, DWORD PTR DS:[EDI+0x1AD8]
	$-22     006D0DE6     6A 01                       PUSH 0x1
	$-20     006D0DE8     FF75 EC                     PUSH DWORD PTR SS:[EBP-0x14]
	$-1D     006D0DEB     E8 10CCDFFF                 CALL PathOfEx.004CDA00
	$-18     006D0DF0     E9 E9000000                 JMP PathOfEx.006D0EDE                           ; edi=InGameState*
	$-13     006D0DF5     FF75 E8                     PUSH DWORD PTR SS:[EBP-0x18]
	$-10     006D0DF8     A1 18D70001                 MOV EAX, DWORD PTR DS:[0x100D718]
	$-B      006D0DFD     FF71 14                     PUSH DWORD PTR DS:[ECX+0x14]                    ; push id，比如npc_id,ecx==stCD_EntityPosBased*,ecx=目标实体
	$-8      006D0E00     25 FF3F0000                 AND EAX, 0x3FFF
	$-3      006D0E05     8BCA                        MOV ECX, EDX                                    ; edx=stCD_GameLogic*
	$-1      006D0E07     50                          PUSH EAX                                        ; eax一直等于0x266
	$ ==>    006D0E08     E8 D3472000                 CALL PathOfEx.008D55E0                          ; 打开传送点 npc 打开仓库 捡取物品 打开箱子
	$-1      008D55DF     CC                          INT3                                            ; g_c_touch_target_obj_
	$ ==>    008D55E0     55                          PUSH EBP                                        ; 打开传送点 npc 打开仓库 捡取物品 打开箱子
	$+1      008D55E1     8BEC                        MOV EBP, ESP                                    ; 3个参数，p1=0x266,p2=id,p3=8,p3是个byte
	$+3      008D55E3     83E4 F8                     AND ESP, -0x8                                   ; 对于打开npc来说，p1一直等于0x226，p3一直等于8
	$+6      008D55E6     83EC 08                     SUB ESP, 0x8
	$+9      008D55E9     8D0424                      LEA EAX, DWORD PTR SS:[ESP]
	$+C      008D55EC     FF75 10                     PUSH DWORD PTR SS:[EBP+0x10]
	$+F      008D55EF     50                          PUSH EAX
	$+10     008D55F0     FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
	$+13     008D55F3     FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
	$+16     008D55F6     E8 15000000                 CALL PathOfEx.008D5610			;call tag_g_c_touch_target_obj_impl
	$+1B     008D55FB     8BE5                        MOV ESP, EBP
	$+1D     008D55FD     5D                          POP EBP                                         ; 0018E5DC
	$+1E     008D55FE     C2 0C00                     RETN 0xC
	$+21     008D5601     CC                          INT3
	$+5      006D0E0D     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+8      006D0E10     898F D0180000               MOV DWORD PTR DS:[EDI+0x18D0], ECX
	$+E      006D0E16     E8 053EF9FF                 CALL PathOfEx.00664C20
	$+13     006D0E1B     84C0                        TEST AL, AL
	$+15     006D0E1D     0F84 BB000000               JE PathOfEx.006D0EDE
	$+1B     006D0E23     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+21     006D0E29     E8 E24A2000                 CALL PathOfEx.008D5910
	$+26     006D0E2E     E9 AB000000                 JMP PathOfEx.006D0EDE
	$+2B     006D0E33     85C9                        TEST ECX, ECX
	$+2D     006D0E35     0F84 A3000000               JE PathOfEx.006D0EDE
	$+33     006D0E3B     8B87 3C010000               MOV EAX, DWORD PTR DS:[EDI+0x13C]
	$+39     006D0E41     8A80 6B3B0000               MOV AL, BYTE PTR DS:[EAX+0x3B6B]
	$+3F     006D0E47     84C0                        TEST AL, AL
	$+41     006D0E49     74 48                       JE SHORT PathOfEx.006D0E93
	$+43     006D0E4B     E8 C0C4DDFF                 CALL PathOfEx.004AD310
	$+48     006D0E50     84C0                        TEST AL, AL
	$+4A     006D0E52     74 14                       JE SHORT PathOfEx.006D0E68
	$+4C     006D0E54     FF75 EC                     PUSH DWORD PTR SS:[EBP-0x14]
	$+4F     006D0E57     8BCF                        MOV ECX, EDI
	$+51     006D0E59     E8 D2140000                 CALL PathOfEx.006D2330
	$+56     006D0E5E     BF 03000000                 MOV EDI, 0x3
	$+5B     006D0E63     E9 BC000000                 JMP PathOfEx.006D0F24
	$+60     006D0E68     8B45 C8                     MOV EAX, DWORD PTR SS:[EBP-0x38]                ; nvd3dum.67BFF867
	$+63     006D0E6B     66:8B40 02                  MOV AX, WORD PTR DS:[EAX+0x2]
	$+67     006D0E6F     66:3B05 18D70001            CMP AX, WORD PTR DS:[0x100D718]
	$+6E     006D0E76   ^ 75 E6                       JNZ SHORT PathOfEx.006D0E5E
	$+70     006D0E78     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+73     006D0E7B     E8 80340000                 CALL PathOfEx.006D4300
	$+78     006D0E80     84C0                        TEST AL, AL
	$+7A     006D0E82     75 0C                       JNZ SHORT PathOfEx.006D0E90
	$+7C     006D0E84     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+7F     006D0E87     E8 E43DF9FF                 CALL PathOfEx.00664C70
	$+84     006D0E8C     84C0                        TEST AL, AL
	$+86     006D0E8E   ^ 74 CE                       JE SHORT PathOfEx.006D0E5E
	$+88     006D0E90     8B4D EC                     MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+8B     006D0E93     8B45 C8                     MOV EAX, DWORD PTR SS:[EBP-0x38]                ; nvd3dum.67BFF867
	$+8E     006D0E96     FF75 E8                     PUSH DWORD PTR SS:[EBP-0x18]
	$+91     006D0E99     FF71 14                     PUSH DWORD PTR DS:[ECX+0x14]
	$+94     006D0E9C     0FB700                      MOVZX EAX, WORD PTR DS:[EAX]
	$+97     006D0E9F     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+9D     006D0EA5     EB 31                       JMP SHORT PathOfEx.006D0ED8
	$+9F     006D0EA7     8B87 38010000               MOV EAX, DWORD PTR DS:[EDI+0x138]
	$+A5     006D0EAD     8B55 C8                     MOV EDX, DWORD PTR SS:[EBP-0x38]                ; nvd3dum.67BFF867
	$+A8     006D0EB0     85D2                        TEST EDX, EDX
	$+AA     006D0EB2     8B98 C4010000               MOV EBX, DWORD PTR DS:[EAX+0x1C4]
	$+B0     006D0EB8     0F94C0                      SETE AL
	$+B3     006D0EBB     34 01                       XOR AL, 0x1
	$+B5     006D0EBD     74 1F                       JE SHORT PathOfEx.006D0EDE
	$+B7     006D0EBF     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
	$+BD     006D0EC5     8A81 6B3B0000               MOV AL, BYTE PTR DS:[ECX+0x3B6B]
	$+C3     006D0ECB     84C0                        TEST AL, AL
	$+C5     006D0ECD     75 0F                       JNZ SHORT PathOfEx.006D0EDE
	$+C7     006D0ECF     FF75 E8                     PUSH DWORD PTR SS:[EBP-0x18]
	$+CA     006D0ED2     0FB702                      MOVZX EAX, WORD PTR DS:[EDX]
	$+CD     006D0ED5     FF73 14                     PUSH DWORD PTR DS:[EBX+0x14]                    ; PathOfEx.01000000
	$+D0     006D0ED8     50                          PUSH EAX
	$+D1     006D0ED9     E8 02472000                 CALL PathOfEx.008D55E0
	$+D6     006D0EDE     8B5D D0                     MOV EBX, DWORD PTR SS:[EBP-0x30]                ; nvd3dum.67BFFABE
	$+D9     006D0EE1     8B45 E0                     MOV EAX, DWORD PTR SS:[EBP-0x20]
	$+DC     006D0EE4     8D8F 70150000               LEA ECX, DWORD PTR DS:[EDI+0x1570]
	$+E2     006D0EEA     8987 84150000               MOV DWORD PTR DS:[EDI+0x1584], EAX
	$+E8     006D0EF0     8B45 DC                     MOV EAX, DWORD PTR SS:[EBP-0x24]
	$+EB     006D0EF3     8987 88150000               MOV DWORD PTR DS:[EDI+0x1588], EAX
	$+F1     006D0EF9     8D45 A4                     LEA EAX, DWORD PTR SS:[EBP-0x5C]
	$+F4     006D0EFC     50                          PUSH EAX
	$+F5     006D0EFD     899F 80150000               MOV DWORD PTR DS:[EDI+0x1580], EBX
	$+FB     006D0F03     E8 18360000                 CALL PathOfEx.006D4520
	$+100    006D0F08     E8 53BF0300                 CALL PathOfEx.0070CE60
	$+105    006D0F0D     83C0 43                     ADD EAX, 0x43
	$+108    006D0F10     8987 90150000               MOV DWORD PTR DS:[EDI+0x1590], EAX
	$+10E    006D0F16     83D2 00                     ADC EDX, 0x0
	$+111    006D0F19     8997 94150000               MOV DWORD PTR DS:[EDI+0x1594], EDX
	$+117    006D0F1F     BF 04000000                 MOV EDI, 0x4
	$+11C    006D0F24     C645 FC 09                  MOV BYTE PTR SS:[EBP-0x4], 0x9
	$+120    006D0F28     8B5D AC                     MOV EBX, DWORD PTR SS:[EBP-0x54]
	$+123    006D0F2B     85DB                        TEST EBX, EBX
	$+125    006D0F2D     74 1F                       JE SHORT PathOfEx.006D0F4E
	$+127    006D0F2F     8BC6                        MOV EAX, ESI
	$+129    006D0F31     F0:0FC143 04                LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+12E    006D0F36     75 16                       JNZ SHORT PathOfEx.006D0F4E
	$+130    006D0F38     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$+132    006D0F3A     8BCB                        MOV ECX, EBX
	$+134    006D0F3C     FF10                        CALL NEAR DWORD PTR DS:[EAX]
	$+136    006D0F3E     8BC6                        MOV EAX, ESI
	$+138    006D0F40     F0:0FC143 08                LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$+13D    006D0F45     75 07                       JNZ SHORT PathOfEx.006D0F4E
	$+13F    006D0F47     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$+141    006D0F49     8BCB                        MOV ECX, EBX
	$+143    006D0F4B     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+146    006D0F4E     C645 FC 0A                  MOV BYTE PTR SS:[EBP-0x4], 0xA
	$+14A    006D0F52     8B5D CC                     MOV EBX, DWORD PTR SS:[EBP-0x34]
	$+14D    006D0F55     85DB                        TEST EBX, EBX
	$+14F    006D0F57     74 1F                       JE SHORT PathOfEx.006D0F78
	$+151    006D0F59     8BC6                        MOV EAX, ESI
	$+153    006D0F5B     F0:0FC143 04                LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+158    006D0F60     75 16                       JNZ SHORT PathOfEx.006D0F78
	$+15A    006D0F62     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$+15C    006D0F64     8BCB                        MOV ECX, EBX
	$+15E    006D0F66     FF10                        CALL NEAR DWORD PTR DS:[EAX]
	$+160    006D0F68     8BC6                        MOV EAX, ESI
	$+162    006D0F6A     F0:0FC143 08                LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
	$+167    006D0F6F     75 07                       JNZ SHORT PathOfEx.006D0F78
	$+169    006D0F71     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$+16B    006D0F73     8BCB                        MOV ECX, EBX
	$+16D    006D0F75     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+170    006D0F78     C745 FC 0B000000            MOV DWORD PTR SS:[EBP-0x4], 0xB
	$+177    006D0F7F     8B5D D8                     MOV EBX, DWORD PTR SS:[EBP-0x28]
	$+17A    006D0F82     85DB                        TEST EBX, EBX
	$+17C    006D0F84     74 1E                       JE SHORT PathOfEx.006D0FA4
	$+17E    006D0F86     8BC6                        MOV EAX, ESI
	$+180    006D0F88     F0:0FC143 04                LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
	$+185    006D0F8D     75 15                       JNZ SHORT PathOfEx.006D0FA4
	$+187    006D0F8F     8B03                        MOV EAX, DWORD PTR DS:[EBX]
	$+189    006D0F91     8BCB                        MOV ECX, EBX
	$+18B    006D0F93     FF10                        CALL NEAR DWORD PTR DS:[EAX]
	$+18D    006D0F95     F0:0FC173 08                LOCK XADD DWORD PTR DS:[EBX+0x8], ESI
	$+192    006D0F9A     4E                          DEC ESI
	$+193    006D0F9B     75 07                       JNZ SHORT PathOfEx.006D0FA4
	$+195    006D0F9D     8B13                        MOV EDX, DWORD PTR DS:[EBX]
	$+197    006D0F9F     8BCB                        MOV ECX, EBX
	$+199    006D0FA1     FF52 04                     CALL NEAR DWORD PTR DS:[EDX+0x4]
	$+19C    006D0FA4     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+19F    006D0FA7     8BC7                        MOV EAX, EDI
	$+1A1    006D0FA9     5F                          POP EDI                                         ; 0018E5DC
	$+1A2    006D0FAA     5E                          POP ESI                                         ; 0018E5DC
	$+1A3    006D0FAB     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
	$+1AA    006D0FB2     5B                          POP EBX                                         ; 0018E5DC
	$+1AB    006D0FB3     8BE5                        MOV ESP, EBP
	$+1AD    006D0FB5     5D                          POP EBP                                         ; 0018E5DC
	$+1AE    006D0FB6     C3                          RETN
	$+1AF    006D0FB7     90                          NOP
	*/
	addr_tmp = scan_exe_.ScanCall("e8????????8b????89??????????e8????????84??0f??????????8b??????????e8", 1);
	assert(addr_tmp);
	ofs << "pt_cdword	g_c_touch_target_obj_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-1D     0067FB1F       CC                       INT3
	$-1C     0067FB20       55                       PUSH EBP                                        ; tag_关闭npc面板
	$-1B     0067FB21       8BEC                     MOV EBP, ESP
	$-19     0067FB23       64:A1 00000000           MOV EAX, DWORD PTR FS:[0]
	$-13     0067FB29       6A FF                    PUSH -0x1
	$-11     0067FB2B       68 9878D200              PUSH PathOfEx.00D27898
	$-C      0067FB30       50                       PUSH EAX
	$-B      0067FB31       64:8925 00000000         MOV DWORD PTR FS:[0], ESP
	$-4      0067FB38       83EC 10                  SUB ESP, 0x10
	$-1      0067FB3B       56                       PUSH ESI
	$ ==>    0067FB3C       8BF1                     MOV ESI, ECX
	$+2      0067FB3E       8B86 5C0A0000            MOV EAX, DWORD PTR DS:[ESI+0xA5C]
	$+8      0067FB44       8A80 54070000            MOV AL, BYTE PTR DS:[EAX+0x754]
	$+E      0067FB4A       84C0                     TEST AL, AL
	$+10     0067FB4C       74 5C                    JE SHORT PathOfEx.0067FBAA
	$+12     0067FB4E       8BB6 BC080000            MOV ESI, DWORD PTR DS:[ESI+0x8BC]
	$+18     0067FB54       B8 5F000000              MOV EAX, 0x5F
	$+1D     0067FB59       66:8945 E8               MOV WORD PTR SS:[EBP-0x18], AX
	$+21     0067FB5D       C745 E4 E077EF00         MOV DWORD PTR SS:[EBP-0x1C], PathOfEx.00EF77E0
	$+28     0067FB64       8D45 E4                  LEA EAX, DWORD PTR SS:[EBP-0x1C]
	$+2B     0067FB67       C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+32     0067FB6E       50                       PUSH EAX
	$+33     0067FB6F       FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]                  ; PathOfEx.00BD7240
	$+39     0067FB75       C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+40     0067FB7C       C645 F0 00               MOV BYTE PTR SS:[EBP-0x10], 0x0
	$+44     0067FB80       E8 3B8F0300              CALL PathOfEx.006B8AC0
	$+49     0067FB85       8BC8                     MOV ECX, EAX
	$+4B     0067FB87       E8 348D2600              CALL PathOfEx.008E88C0
	$+50     0067FB8C       8B8E 983A0000            MOV ECX, DWORD PTR DS:[ESI+0x3A98]              ; PathOfEx.00BD7240
	$+56     0067FB92       6A 00                    PUSH 0x0
	$+58     0067FB94       E8 C73C1100              CALL PathOfEx.00793860
	$+5D     0067FB99       8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+60     0067FB9C       64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$+67     0067FBA3       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
	$+68     0067FBA4       8BE5                     MOV ESP, EBP
	$+6A     0067FBA6       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
	$+6B     0067FBA7       C2 0400                  RETN 0x4
	$+6E     0067FBAA       FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]
	$+71     0067FBAD       8B8E 4C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA4C]
	$+77     0067FBB3       6A 01                    PUSH 0x1
	$+79     0067FBB5       E8 4683F4FF              CALL PathOfEx.005C7F00
	$+7E     0067FBBA       8B8E BC080000            MOV ECX, DWORD PTR DS:[ESI+0x8BC]
	$+84     0067FBC0       E8 DB742500              CALL PathOfEx.008D70A0
	$+89     0067FBC5       8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+8C     0067FBC8       64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$+93     0067FBCF       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
	$+94     0067FBD0       8BE5                     MOV ESP, EBP
	$+96     0067FBD2       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
	$+97     0067FBD3       C2 0400                  RETN 0x4
	$+9A     0067FBD6       CC                       INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??8b??????????8a??????????84??74??8b??????????b8????????66", -0x1c);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (*(pt_byte*)addr_tmp != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_close_npc_ui_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-23F    006105CF     CC                       INT3
	$-23E    006105D0     55                       PUSH EBP
	$-23D    006105D1     8BEC                     MOV EBP, ESP
	$-23B    006105D3     6A FF                    PUSH -0x1
	$-239    006105D5     68 FA64D300              PUSH PathOfEx.00D364FA
	$-234    006105DA     64:A1 00000000           MOV EAX, DWORD PTR FS:[0]
	$-22E    006105E0     50                       PUSH EAX
	$-22D    006105E1     64:8925 00000000         MOV DWORD PTR FS:[0], ESP
	$-226    006105E8     83EC 0C                  SUB ESP, 0xC
	$-223    006105EB     8B55 0C                  MOV EDX, DWORD PTR SS:[EBP+0xC]
	$-220    006105EE     53                       PUSH EBX
	$-21F    006105EF     56                       PUSH ESI
	$-21E    006105F0     8BD9                     MOV EBX, ECX
	$-21C    006105F2     8D4D E8                  LEA ECX, DWORD PTR SS:[EBP-0x18]
	$-219    006105F5     57                       PUSH EDI
	$-218    006105F6     E8 45C43E00              CALL PathOfEx.009FCA40
	$-213    006105FB     68 84080000              PUSH 0x884
	$-20E    00610600     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4], 0x0
	$-207    00610607     E8 87966600              CALL PathOfEx.00C79C93
	$-202    0061060C     83C4 04                  ADD ESP, 0x4
	$-1FF    0061060F     8945 0C                  MOV DWORD PTR SS:[EBP+0xC], EAX
	$-1FC    00610612     8BC8                     MOV ECX, EAX
	$-1FA    00610614     E8 27E83D00              CALL PathOfEx.009EEE40
	$-1F5    00610619     8B0B                     MOV ECX, DWORD PTR DS:[EBX]
	$-1F3    0061061B     8BF8                     MOV EDI, EAX
	$-1F1    0061061D     57                       PUSH EDI
	$-1F0    0061061E     8B09                     MOV ECX, DWORD PTR DS:[ECX]
	$-1EE    00610620     81C1 04070000            ADD ECX, 0x704
	$-1E8    00610626     8B51 08                  MOV EDX, DWORD PTR DS:[ECX+0x8]
	$-1E5    00610629     2B51 04                  SUB EDX, DWORD PTR DS:[ECX+0x4]
	$-1E2    0061062C     C1FA 02                  SAR EDX, 0x2
	$-1DF    0061062F     52                       PUSH EDX
	$-1DE    00610630     E8 9BE43E00              CALL PathOfEx.009FEAD0
	$-1D9    00610635     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-1D7    00610637     8D4D E8                  LEA ECX, DWORD PTR SS:[EBP-0x18]
	$-1D4    0061063A     6A 01                    PUSH 0x1
	$-1D2    0061063C     51                       PUSH ECX
	$-1D1    0061063D     8BCF                     MOV ECX, EDI
	$-1CF    0061063F     FF90 20010000            CALL NEAR DWORD PTR DS:[EAX+0x120]
	$-1C9    00610645     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-1C7    00610647     8BCF                     MOV ECX, EDI
	$-1C5    00610649     8B40 3C                  MOV EAX, DWORD PTR DS:[EAX+0x3C]
	$-1C2    0061064C     FFD0                     CALL NEAR EAX
	$-1C0    0061064E     D80D E4CAF300            FMUL DWORD PTR DS:[0xF3CAE4]
	$-1BA    00610654     8BCF                     MOV ECX, EDI
	$-1B8    00610656     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-1B6    00610658     8B73 04                  MOV ESI, DWORD PTR DS:[EBX+0x4]
	$-1B3    0061065B     D82D 68CDF300            FSUBR DWORD PTR DS:[0xF3CD68]
	$-1AD    00610661     8B40 40                  MOV EAX, DWORD PTR DS:[EAX+0x40]
	$-1AA    00610664     D95D F0                  FSTP DWORD PTR SS:[EBP-0x10]
	$-1A7    00610667     FFD0                     CALL NEAR EAX
	$-1A5    00610669     F3:0F1087 60070000       MOVSS XMM0, DWORD PTR DS:[EDI+0x760]
	$-19D    00610671     F3:0F1055 F0             MOVSS XMM2, DWORD PTR SS:[EBP-0x10]
	$-198    00610676     D80D E4CAF300            FMUL DWORD PTR DS:[0xF3CAE4]
	$-192    0061067C     F3:0F100E                MOVSS XMM1, DWORD PTR DS:[ESI]
	$-18E    00610680     0F2EC2                   UCOMISS XMM0, XMM2
	$-18B    00610683     D95D 0C                  FSTP DWORD PTR SS:[EBP+0xC]
	$-188    00610686     F3:0F5C4D 0C             SUBSS XMM1, DWORD PTR SS:[EBP+0xC]
	$-183    0061068B     9F                       LAHF
	$-182    0061068C     F6C4 44                  TEST AH, 0x44
	$-17F    0061068F     7A 11                    JPE SHORT PathOfEx.006106A2
	$-17D    00610691     F3:0F1087 64070000       MOVSS XMM0, DWORD PTR DS:[EDI+0x764]
	$-175    00610699     0F2EC1                   UCOMISS XMM0, XMM1
	$-172    0061069C     9F                       LAHF
	$-171    0061069D     F6C4 44                  TEST AH, 0x44
	$-16E    006106A0     7B 23                    JPO SHORT PathOfEx.006106C5
	$-16C    006106A2     83BF 68070000 00         CMP DWORD PTR DS:[EDI+0x768], 0x0
	$-165    006106A9     75 08                    JNZ SHORT PathOfEx.006106B3
	$-163    006106AB     F3:0F1197 60070000       MOVSS DWORD PTR DS:[EDI+0x760], XMM2
	$-15B    006106B3     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-159    006106B5     8BCF                     MOV ECX, EDI
	$-157    006106B7     F3:0F118F 64070000       MOVSS DWORD PTR DS:[EDI+0x764], XMM1
	$-14F    006106BF     FF90 08010000            CALL NEAR DWORD PTR DS:[EAX+0x108]
	$-149    006106C5     8B37                     MOV ESI, DWORD PTR DS:[EDI]
	$-147    006106C7     8BCF                     MOV ECX, EDI
	$-145    006106C9     8B46 40                  MOV EAX, DWORD PTR DS:[ESI+0x40]
	$-142    006106CC     FFD0                     CALL NEAR EAX
	$-140    006106CE     D80D 60CBF300            FMUL DWORD PTR DS:[0xF3CB60]
	$-13A    006106D4     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-138    006106D6     51                       PUSH ECX
	$-137    006106D7     8BCF                     MOV ECX, EDI
	$-135    006106D9     8B40 3C                  MOV EAX, DWORD PTR DS:[EAX+0x3C]
	$-132    006106DC     D91C24                   FSTP DWORD PTR SS:[ESP]
	$-12F    006106DF     FFD0                     CALL NEAR EAX
	$-12D    006106E1     D80D 60CBF300            FMUL DWORD PTR DS:[0xF3CB60]
	$-127    006106E7     51                       PUSH ECX
	$-126    006106E8     8BCF                     MOV ECX, EDI
	$-124    006106EA     D91C24                   FSTP DWORD PTR SS:[ESP]
	$-121    006106ED     FF56 54                  CALL NEAR DWORD PTR DS:[ESI+0x54]
	$-11E    006106F0     68 8C0B0000              PUSH 0xB8C
	$-119    006106F5     E8 99956600              CALL PathOfEx.00C79C93
	$-114    006106FA     83C4 04                  ADD ESP, 0x4
	$-111    006106FD     8945 0C                  MOV DWORD PTR SS:[EBP+0xC], EAX
	$-10E    00610700     8BC8                     MOV ECX, EAX
	$-10C    00610702     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4], 0x1
	$-108    00610706     E8 05503F00              CALL PathOfEx.00A05710
	$-103    0061070B     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4], 0x0
	$-FF     0061070F     8BF8                     MOV EDI, EAX
	$-FD     00610711     8B0B                     MOV ECX, DWORD PTR DS:[EBX]
	$-FB     00610713     57                       PUSH EDI
	$-FA     00610714     8B09                     MOV ECX, DWORD PTR DS:[ECX]
	$-F8     00610716     81C1 04070000            ADD ECX, 0x704
	$-F2     0061071C     8B51 08                  MOV EDX, DWORD PTR DS:[ECX+0x8]
	$-EF     0061071F     2B51 04                  SUB EDX, DWORD PTR DS:[ECX+0x4]
	$-EC     00610722     C1FA 02                  SAR EDX, 0x2
	$-E9     00610725     52                       PUSH EDX
	$-E8     00610726     E8 A5E33E00              CALL PathOfEx.009FEAD0
	$-E3     0061072B     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-E1     0061072D     8BCF                     MOV ECX, EDI
	$-DF     0061072F     FF73 08                  PUSH DWORD PTR DS:[EBX+0x8]
	$-DC     00610732     FF50 78                  CALL NEAR DWORD PTR DS:[EAX+0x78]
	$-D9     00610735     8B43 0C                  MOV EAX, DWORD PTR DS:[EBX+0xC]
	$-D6     00610738     8D8F 40080000            LEA ECX, DWORD PTR DS:[EDI+0x840]
	$-D0     0061073E     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$-CE     00610740     FF30                     PUSH DWORD PTR DS:[EAX]
	$-CC     00610742     FF52 0C                  CALL NEAR DWORD PTR DS:[EDX+0xC]
	$-C9     00610745     8B87 B0080000            MOV EAX, DWORD PTR DS:[EDI+0x8B0]
	$-C3     0061074B     8D8F B0080000            LEA ECX, DWORD PTR DS:[EDI+0x8B0]
	$-BD     00610751     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]
	$-BA     00610754     FF50 04                  CALL NEAR DWORD PTR DS:[EAX+0x4]
	$-B7     00610757     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-B5     00610759     8BCF                     MOV ECX, EDI
	$-B3     0061075B     8B73 04                  MOV ESI, DWORD PTR DS:[EBX+0x4]
	$-B0     0061075E     8B40 40                  MOV EAX, DWORD PTR DS:[EAX+0x40]
	$-AD     00610761     FFD0                     CALL NEAR EAX
	$-AB     00610763     F3:0F1087 60070000       MOVSS XMM0, DWORD PTR DS:[EDI+0x760]
	$-A3     0061076B     0F2E05 04CEF300          UCOMISS XMM0, DWORD PTR DS:[0xF3CE04]
	$-9C     00610772     D80D E4CAF300            FMUL DWORD PTR DS:[0xF3CAE4]
	$-96     00610778     F3:0F100E                MOVSS XMM1, DWORD PTR DS:[ESI]
	$-92     0061077C     9F                       LAHF
	$-91     0061077D     D95D 0C                  FSTP DWORD PTR SS:[EBP+0xC]
	$-8E     00610780     F3:0F5C4D 0C             SUBSS XMM1, DWORD PTR SS:[EBP+0xC]
	$-89     00610785     F6C4 44                  TEST AH, 0x44
	$-86     00610788     7A 11                    JPE SHORT PathOfEx.0061079B
	$-84     0061078A     F3:0F1087 64070000       MOVSS XMM0, DWORD PTR DS:[EDI+0x764]
	$-7C     00610792     0F2EC1                   UCOMISS XMM0, XMM1
	$-79     00610795     9F                       LAHF
	$-78     00610796     F6C4 44                  TEST AH, 0x44
	$-75     00610799     7B 25                    JPO SHORT PathOfEx.006107C0
	$-73     0061079B     83BF 68070000 00         CMP DWORD PTR DS:[EDI+0x768], 0x0
	$-6C     006107A2     75 0A                    JNZ SHORT PathOfEx.006107AE
	$-6A     006107A4     C787 60070000 0000A042   MOV DWORD PTR DS:[EDI+0x760], 0x42A00000     ; Art/particles/monster_particles/monster_mods/elemental_reflec/lightning _runec.mat
	$-60     006107AE     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-5E     006107B0     8BCF                     MOV ECX, EDI
	$-5C     006107B2     F3:0F118F 64070000       MOVSS DWORD PTR DS:[EDI+0x764], XMM1
	$-54     006107BA     FF90 08010000            CALL NEAR DWORD PTR DS:[EAX+0x108]
	$-4E     006107C0     8B4B 04                  MOV ECX, DWORD PTR DS:[EBX+0x4]
	$-4B     006107C3     8B43 10                  MOV EAX, DWORD PTR DS:[EBX+0x10]
	$-48     006107C6     F3:0F1000                MOVSS XMM0, DWORD PTR DS:[EAX]
	$-44     006107CA     F3:0F5801                ADDSS XMM0, DWORD PTR DS:[ECX]
	$-40     006107CE     F3:0F1101                MOVSS DWORD PTR DS:[ECX], XMM0
	$-3C     006107D2     8D4D E8                  LEA ECX, DWORD PTR SS:[EBP-0x18]
	$-39     006107D5     E8 D679E7FF              CALL PathOfEx.004881B0
	$-34     006107DA     8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$-31     006107DD     5F                       POP EDI
	$-30     006107DE     5E                       POP ESI
	$-2F     006107DF     5B                       POP EBX
	$-2E     006107E0     64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$-27     006107E7     8BE5                     MOV ESP, EBP
	$-25     006107E9     5D                       POP EBP
	$-24     006107EA     C2 0800                  RETN 0x8
	$-21     006107ED     CC                       INT3
	$-20     006107EE     CC                       INT3                                         ; 有任务的npc吧
	$-1F     006107EF     CC                       INT3                                         ; 打开npc的时候会断住
	$-1E     006107F0     55                       PUSH EBP                                     ; g_c_player_quest_info_vfn_
	$-1D     006107F1     8BEC                     MOV EBP, ESP
	$-1B     006107F3     64:A1 00000000           MOV EAX, DWORD PTR FS:[0]
	$-15     006107F9     6A FF                    PUSH -0x1
	$-13     006107FB     68 9CF6D100              PUSH PathOfEx.00D1F69C                       ; ASCII "柑爵"
	$-E      00610800     50                       PUSH EAX
	$-D      00610801     64:8925 00000000         MOV DWORD PTR FS:[0], ESP
	$-6      00610808     83EC 08                  SUB ESP, 0x8
	$-3      0061080B     53                       PUSH EBX
	$-2      0061080C     56                       PUSH ESI
	$-1      0061080D     57                       PUSH EDI
	$ ==>    0061080E     8BF9                     MOV EDI, ECX
	$+2      00610810     837F 20 00               CMP DWORD PTR DS:[EDI+0x20], 0x0
	$+6      00610814     75 36                    JNZ SHORT PathOfEx.0061084C                  ; tag_当前任务信息的任务状态偏移
	$+8      00610816     8D77 24                  LEA ESI, DWORD PTR DS:[EDI+0x24]             ; g_so_cur_quest_info_quest_state_
	$+B      00610819     8D45 EC                  LEA EAX, DWORD PTR SS:[EBP-0x14]
	$+E      0061081C     3BF0                     CMP ESI, EAX
	$+10     0061081E     74 23                    JE SHORT PathOfEx.00610843
	$+12     00610820     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+14     00610822     85C0                     TEST EAX, EAX
	$+16     00610824     74 17                    JE SHORT PathOfEx.0061083D
	$+18     00610826     50                       PUSH EAX
	$+19     00610827     51                       PUSH ECX
	$+1A     00610828     E8 A35C1000              CALL PathOfEx.007164D0
	$+1F     0061082D     83C4 04                  ADD ESP, 0x4
	$+22     00610830     8BC8                     MOV ECX, EAX
	$+24     00610832     E8 29581000              CALL PathOfEx.00716060
	$+29     00610837     C706 00000000            MOV DWORD PTR DS:[ESI], 0x0
	$+2F     0061083D     C706 00000000            MOV DWORD PTR DS:[ESI], 0x0
	$+35     00610843     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4], 0x0
	$+3C     0061084A     EB 6F                    JMP SHORT PathOfEx.006108BB
	$+3E     0061084C     FF77 18                  PUSH DWORD PTR DS:[EDI+0x18]
	$+41     0061084F     8B4F 10                  MOV ECX, DWORD PTR DS:[EDI+0x10]
	$+44     00610852     8D47 1C                  LEA EAX, DWORD PTR DS:[EDI+0x1C]
	$+47     00610855     50                       PUSH EAX
	$+48     00610856     8D45 EC                  LEA EAX, DWORD PTR SS:[EBP-0x14]
	$+4B     00610859     50                       PUSH EAX
	$+4C     0061085A     E8 813F1D00              CALL PathOfEx.007E47E0
	$+51     0061085F     8BD8                     MOV EBX, EAX
	$+53     00610861     8D77 24                  LEA ESI, DWORD PTR DS:[EDI+0x24]             ; tag_当前任务信息的任务状态偏移
	$+56     00610864     3BF3                     CMP ESI, EBX                                 ; g_so_cur_quest_info_quest_state_
	$+58     00610866     74 27                    JE SHORT PathOfEx.0061088F
	$+5A     00610868     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+5C     0061086A     85C0                     TEST EAX, EAX
	$+5E     0061086C     74 17                    JE SHORT PathOfEx.00610885
	$+60     0061086E     50                       PUSH EAX
	$+61     0061086F     51                       PUSH ECX
	$+62     00610870     E8 5B5C1000              CALL PathOfEx.007164D0
	$+67     00610875     83C4 04                  ADD ESP, 0x4
	$+6A     00610878     8BC8                     MOV ECX, EAX
	$+6C     0061087A     E8 E1571000              CALL PathOfEx.00716060
	$+71     0061087F     C706 00000000            MOV DWORD PTR DS:[ESI], 0x0
	$+77     00610885     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+79     00610887     8906                     MOV DWORD PTR DS:[ESI], EAX
	$+7B     00610889     C703 00000000            MOV DWORD PTR DS:[EBX], 0x0
	$+81     0061088F     8B43 04                  MOV EAX, DWORD PTR DS:[EBX+0x4]
	$+84     00610892     8946 04                  MOV DWORD PTR DS:[ESI+0x4], EAX              ; 这里改写stCD_TblItemQuestStates*指针
	$+87     00610895     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4], 0x1
	$+8E     0061089C     8B45 EC                  MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+91     0061089F     85C0                     TEST EAX, EAX
	$+93     006108A1     74 18                    JE SHORT PathOfEx.006108BB
	$+95     006108A3     50                       PUSH EAX
	$+96     006108A4     51                       PUSH ECX
	$+97     006108A5     E8 265C1000              CALL PathOfEx.007164D0
	$+9C     006108AA     83C4 04                  ADD ESP, 0x4
	$+9F     006108AD     8BC8                     MOV ECX, EAX
	$+A1     006108AF     E8 AC571000              CALL PathOfEx.00716060
	$+A6     006108B4     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14], 0x0
	$+AD     006108BB     8DB7 50F7FFFF            LEA ESI, DWORD PTR DS:[EDI-0x8B0]
	$+B3     006108C1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+BA     006108C8     8BCE                     MOV ECX, ESI
	$+BC     006108CA     E8 31000000              CALL PathOfEx.00610900
	$+C1     006108CF     8B8E E0090000            MOV ECX, DWORD PTR DS:[ESI+0x9E0]
	$+C7     006108D5     56                       PUSH ESI
	$+C8     006108D6     81C1 04070000            ADD ECX, 0x704
	$+CE     006108DC     E8 FF2B0000              CALL PathOfEx.006134E0
	$+D3     006108E1     8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+D6     006108E4     5F                       POP EDI
	$+D7     006108E5     5E                       POP ESI
	$+D8     006108E6     64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$+DF     006108ED     5B                       POP EBX
	$+E0     006108EE     8BE5                     MOV ESP, EBP
	$+E2     006108F0     5D                       POP EBP
	$+E3     006108F1     C3                       RETN
	$+E4     006108F2     CC                       INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8b??83??????75??8d????8d????3b??74??8b??85??74", -0x1e);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (*(pt_byte*)addr_tmp != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_player_quest_info_vfn_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-47D    007E39BF     CC                       INT3
	$-47C    007E39C0     51                       PUSH ECX
	$-47B    007E39C1     53                       PUSH EBX
	$-47A    007E39C2     55                       PUSH EBP
	$-479    007E39C3     56                       PUSH ESI
	$-478    007E39C4     8B7424 14                MOV ESI, DWORD PTR SS:[ESP+0x14]
	$-474    007E39C8     8BE9                     MOV EBP, ECX
	$-472    007E39CA     57                       PUSH EDI
	$-471    007E39CB     6A 02                    PUSH 0x2
	$-46F    007E39CD     8BCE                     MOV ECX, ESI
	$-46D    007E39CF     896C24 14                MOV DWORD PTR SS:[ESP+0x14], EBP
	$-469    007E39D3     E8 581DF6FF              CALL PathOfEx.00745730
	$-464    007E39D8     8B7E 08                  MOV EDI, DWORD PTR DS:[ESI+0x8]
	$-461    007E39DB     8D5D 50                  LEA EBX, DWORD PTR SS:[EBP+0x50]
	$-45E    007E39DE     8B16                     MOV EDX, DWORD PTR DS:[ESI]
	$-45C    007E39E0     B9 03000000              MOV ECX, 0x3
	$-457    007E39E5     66:8B85 B4020000         MOV AX, WORD PTR SS:[EBP+0x2B4]
	$-450    007E39EC     894C24 18                MOV DWORD PTR SS:[ESP+0x18], ECX
	$-44C    007E39F0     66:890417                MOV WORD PTR DS:[EDI+EDX], AX
	$-448    007E39F4     BF 01000000              MOV EDI, 0x1
	$-443    007E39F9     8346 08 02               ADD DWORD PTR DS:[ESI+0x8], 0x2
	$-43F    007E39FD     D1C7                     ROL EDI, 1
	$-43D    007E39FF     90                       NOP
	$-43C    007E3A00     0FB785 B4020000          MOVZX EAX, WORD PTR SS:[EBP+0x2B4]
	$-435    007E3A07     85C7                     TEST EDI, EAX
	$-433    007E3A09     74 28                    JE SHORT PathOfEx.007E3A33
	$-431    007E3A0B     68 80000000              PUSH 0x80
	$-42C    007E3A10     8BCE                     MOV ECX, ESI
	$-42A    007E3A12     E8 191DF6FF              CALL PathOfEx.00745730
	$-425    007E3A17     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$-422    007E3A1A     0306                     ADD EAX, DWORD PTR DS:[ESI]
	$-420    007E3A1C     68 80000000              PUSH 0x80
	$-41B    007E3A21     53                       PUSH EBX
	$-41A    007E3A22     50                       PUSH EAX
	$-419    007E3A23     E8 68F34A00              CALL PathOfEx.00C92D90
	$-414    007E3A28     8B4C24 24                MOV ECX, DWORD PTR SS:[ESP+0x24]
	$-410    007E3A2C     83C4 0C                  ADD ESP, 0xC
	$-40D    007E3A2F     836E 08 80               SUB DWORD PTR DS:[ESI+0x8], -0x80
	$-409    007E3A33     83EB 80                  SUB EBX, -0x80
	$-406    007E3A36     D1C7                     ROL EDI, 1
	$-404    007E3A38     83E9 01                  SUB ECX, 0x1
	$-401    007E3A3B     894C24 18                MOV DWORD PTR SS:[ESP+0x18], ECX
	$-3FD    007E3A3F   ^ 75 BF                    JNZ SHORT PathOfEx.007E3A00
	$-3FB    007E3A41     F685 B4020000 20         TEST BYTE PTR SS:[EBP+0x2B4], 0x20
	$-3F4    007E3A48     74 24                    JE SHORT PathOfEx.007E3A6E
	$-3F2    007E3A4A     6A 2C                    PUSH 0x2C
	$-3F0    007E3A4C     8BCE                     MOV ECX, ESI
	$-3EE    007E3A4E     E8 DD1CF6FF              CALL PathOfEx.00745730
	$-3E9    007E3A53     6A 2C                    PUSH 0x2C
	$-3E7    007E3A55     8D85 D0010000            LEA EAX, DWORD PTR SS:[EBP+0x1D0]
	$-3E1    007E3A5B     50                       PUSH EAX
	$-3E0    007E3A5C     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$-3DD    007E3A5F     0306                     ADD EAX, DWORD PTR DS:[ESI]
	$-3DB    007E3A61     50                       PUSH EAX
	$-3DA    007E3A62     E8 29F34A00              CALL PathOfEx.00C92D90
	$-3D5    007E3A67     83C4 0C                  ADD ESP, 0xC
	$-3D2    007E3A6A     8346 08 2C               ADD DWORD PTR DS:[ESI+0x8], 0x2C
	$-3CE    007E3A6E     B8 00020000              MOV EAX, 0x200
	$-3C9    007E3A73     66:8585 B4020000         TEST WORD PTR SS:[EBP+0x2B4], AX
	$-3C2    007E3A7A     74 1E                    JE SHORT PathOfEx.007E3A9A
	$-3C0    007E3A7C     6A 08                    PUSH 0x8
	$-3BE    007E3A7E     8BCE                     MOV ECX, ESI
	$-3BC    007E3A80     E8 AB1CF6FF              CALL PathOfEx.00745730
	$-3B7    007E3A85     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$-3B4    007E3A88     0306                     ADD EAX, DWORD PTR DS:[ESI]
	$-3B2    007E3A8A     F30F7E85 FC010000        MOVQ XMM0, QWORD PTR SS:[EBP+0x1FC]
	$-3AA    007E3A92     660FD600                 MOVQ QWORD PTR DS:[EAX], XMM0
	$-3A6    007E3A96     8346 08 08               ADD DWORD PTR DS:[ESI+0x8], 0x8
	$-3A2    007E3A9A     F685 B4020000 01         TEST BYTE PTR SS:[EBP+0x2B4], 0x1
	$-39B    007E3AA1     74 1B                    JE SHORT PathOfEx.007E3ABE
	$-399    007E3AA3     6A 04                    PUSH 0x4
	$-397    007E3AA5     8BCE                     MOV ECX, ESI
	$-395    007E3AA7     E8 841CF6FF              CALL PathOfEx.00745730
	$-390    007E3AAC     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-38D    007E3AAF     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-38B    007E3AB1     8B85 B0020000            MOV EAX, DWORD PTR SS:[EBP+0x2B0]
	$-385    007E3AB7     89040A                   MOV DWORD PTR DS:[EDX+ECX], EAX
	$-382    007E3ABA     8346 08 04               ADD DWORD PTR DS:[ESI+0x8], 0x4
	$-37E    007E3ABE     F685 B4020000 10         TEST BYTE PTR SS:[EBP+0x2B4], 0x10
	$-377    007E3AC5     74 17                    JE SHORT PathOfEx.007E3ADE
	$-375    007E3AC7     6A 01                    PUSH 0x1
	$-373    007E3AC9     8BCE                     MOV ECX, ESI
	$-371    007E3ACB     E8 601CF6FF              CALL PathOfEx.00745730
	$-36C    007E3AD0     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-369    007E3AD3     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-367    007E3AD5     8A45 48                  MOV AL, BYTE PTR SS:[EBP+0x48]
	$-364    007E3AD8     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-361    007E3ADB     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-35E    007E3ADE     B8 00080000              MOV EAX, 0x800
	$-359    007E3AE3     66:8585 B4020000         TEST WORD PTR SS:[EBP+0x2B4], AX
	$-352    007E3AEA     74 17                    JE SHORT PathOfEx.007E3B03
	$-350    007E3AEC     6A 01                    PUSH 0x1
	$-34E    007E3AEE     8BCE                     MOV ECX, ESI
	$-34C    007E3AF0     E8 3B1CF6FF              CALL PathOfEx.00745730
	$-347    007E3AF5     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-344    007E3AF8     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-342    007E3AFA     8A45 49                  MOV AL, BYTE PTR SS:[EBP+0x49]
	$-33F    007E3AFD     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-33C    007E3B00     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-339    007E3B03     F685 B4020000 40         TEST BYTE PTR SS:[EBP+0x2B4], 0x40
	$-332    007E3B0A     0F84 87000000            JE PathOfEx.007E3B97
	$-32C    007E3B10     8DBD 08020000            LEA EDI, DWORD PTR SS:[EBP+0x208]
	$-326    007E3B16     8D85 98020000            LEA EAX, DWORD PTR SS:[EBP+0x298]
	$-320    007E3B1C     3BF8                     CMP EDI, EAX
	$-31E    007E3B1E     74 77                    JE SHORT PathOfEx.007E3B97
	$-31C    007E3B20     BB F4FDFFFF              MOV EBX, -0x20C
	$-317    007E3B25     2BDD                     SUB EBX, EBP
	$-315    007E3B27     8BE8                     MOV EBP, EAX
	$-313    007E3B29     0F1F80 00000000          NOP DWORD PTR DS:[EAX]
	$-30C    007E3B30     8D43 04                  LEA EAX, DWORD PTR DS:[EBX+0x4]
	$-309    007E3B33     03C7                     ADD EAX, EDI
	$-307    007E3B35     C1F8 04                  SAR EAX, 0x4
	$-304    007E3B38     40                       INC EAX
	$-303    007E3B39     83F8 09                  CMP EAX, 0x9
	$-300    007E3B3C     7D 4E                    JGE SHORT PathOfEx.007E3B8C
	$-2FE    007E3B3E     6A 04                    PUSH 0x4
	$-2FC    007E3B40     8BCE                     MOV ECX, ESI
	$-2FA    007E3B42     E8 E91BF6FF              CALL PathOfEx.00745730
	$-2F5    007E3B47     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-2F2    007E3B4A     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-2F0    007E3B4C     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-2EE    007E3B4E     6A 04                    PUSH 0x4
	$-2EC    007E3B50     89040A                   MOV DWORD PTR DS:[EDX+ECX], EAX
	$-2E9    007E3B53     8BCE                     MOV ECX, ESI
	$-2E7    007E3B55     8346 08 04               ADD DWORD PTR DS:[ESI+0x8], 0x4
	$-2E3    007E3B59     E8 D21BF6FF              CALL PathOfEx.00745730
	$-2DE    007E3B5E     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-2DB    007E3B61     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-2D9    007E3B63     8B47 04                  MOV EAX, DWORD PTR DS:[EDI+0x4]
	$-2D6    007E3B66     6A 08                    PUSH 0x8
	$-2D4    007E3B68     89040A                   MOV DWORD PTR DS:[EDX+ECX], EAX
	$-2D1    007E3B6B     8BCE                     MOV ECX, ESI
	$-2CF    007E3B6D     8346 08 04               ADD DWORD PTR DS:[ESI+0x8], 0x4
	$-2CB    007E3B71     E8 BA1BF6FF              CALL PathOfEx.00745730
	$-2C6    007E3B76     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-2C3    007E3B79     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-2C1    007E3B7B     8B47 08                  MOV EAX, DWORD PTR DS:[EDI+0x8]
	$-2BE    007E3B7E     89040A                   MOV DWORD PTR DS:[EDX+ECX], EAX
	$-2BB    007E3B81     8B47 0C                  MOV EAX, DWORD PTR DS:[EDI+0xC]
	$-2B8    007E3B84     89440A 04                MOV DWORD PTR DS:[EDX+ECX+0x4], EAX
	$-2B4    007E3B88     8346 08 08               ADD DWORD PTR DS:[ESI+0x8], 0x8
	$-2B0    007E3B8C     83C7 10                  ADD EDI, 0x10
	$-2AD    007E3B8F     3BFD                     CMP EDI, EBP
	$-2AB    007E3B91   ^ 75 9D                    JNZ SHORT PathOfEx.007E3B30
	$-2A9    007E3B93     8B6C24 10                MOV EBP, DWORD PTR SS:[ESP+0x10]
	$-2A5    007E3B97     F685 B4020000 80         TEST BYTE PTR SS:[EBP+0x2B4], 0x80
	$-29E    007E3B9E     0F84 95000000            JE PathOfEx.007E3C39
	$-298    007E3BA4     8B85 9C020000            MOV EAX, DWORD PTR SS:[EBP+0x29C]
	$-292    007E3BAA     8BCE                     MOV ECX, ESI
	$-290    007E3BAC     6A 02                    PUSH 0x2
	$-28E    007E3BAE     85C0                     TEST EAX, EAX
	$-28C    007E3BB0     75 16                    JNZ SHORT PathOfEx.007E3BC8
	$-28A    007E3BB2     E8 791BF6FF              CALL PathOfEx.00745730
	$-285    007E3BB7     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$-282    007E3BBA     33D2                     XOR EDX, EDX
	$-280    007E3BBC     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-27E    007E3BBE     66:891401                MOV WORD PTR DS:[ECX+EAX], DX
	$-27A    007E3BC2     8346 08 02               ADD DWORD PTR DS:[ESI+0x8], 0x2
	$-276    007E3BC6     EB 71                    JMP SHORT PathOfEx.007E3C39
	$-274    007E3BC8     0FB778 24                MOVZX EDI, WORD PTR DS:[EAX+0x24]
	$-270    007E3BCC     E8 5F1BF6FF              CALL PathOfEx.00745730
	$-26B    007E3BD1     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$-268    007E3BD4     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-266    007E3BD6     6A 01                    PUSH 0x1
	$-264    007E3BD8     66:893C01                MOV WORD PTR DS:[ECX+EAX], DI
	$-260    007E3BDC     8BCE                     MOV ECX, ESI
	$-25E    007E3BDE     8346 08 02               ADD DWORD PTR DS:[ESI+0x8], 0x2
	$-25A    007E3BE2     E8 491BF6FF              CALL PathOfEx.00745730
	$-255    007E3BE7     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-252    007E3BEA     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-250    007E3BEC     8A85 A0020000            MOV AL, BYTE PTR SS:[EBP+0x2A0]
	$-24A    007E3BF2     6A 01                    PUSH 0x1
	$-248    007E3BF4     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-245    007E3BF7     8BCE                     MOV ECX, ESI
	$-243    007E3BF9     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-240    007E3BFC     8B9D A8020000            MOV EBX, DWORD PTR SS:[EBP+0x2A8]
	$-23A    007E3C02     2B9D A4020000            SUB EBX, DWORD PTR SS:[EBP+0x2A4]
	$-234    007E3C08     E8 231BF6FF              CALL PathOfEx.00745730
	$-22F    007E3C0D     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$-22C    007E3C10     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-22A    007E3C12     53                       PUSH EBX
	$-229    007E3C13     881C01                   MOV BYTE PTR DS:[ECX+EAX], BL
	$-226    007E3C16     8BCE                     MOV ECX, ESI
	$-224    007E3C18     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-221    007E3C1B     8BBD A4020000            MOV EDI, DWORD PTR SS:[EBP+0x2A4]
	$-21B    007E3C21     E8 0A1BF6FF              CALL PathOfEx.00745730
	$-216    007E3C26     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$-213    007E3C29     0306                     ADD EAX, DWORD PTR DS:[ESI]
	$-211    007E3C2B     53                       PUSH EBX
	$-210    007E3C2C     57                       PUSH EDI
	$-20F    007E3C2D     50                       PUSH EAX
	$-20E    007E3C2E     E8 5DF14A00              CALL PathOfEx.00C92D90
	$-209    007E3C33     83C4 0C                  ADD ESP, 0xC
	$-206    007E3C36     015E 08                  ADD DWORD PTR DS:[ESI+0x8], EBX
	$-203    007E3C39     B8 00010000              MOV EAX, 0x100
	$-1FE    007E3C3E     66:8585 B4020000         TEST WORD PTR SS:[EBP+0x2B4], AX
	$-1F7    007E3C45     74 17                    JE SHORT PathOfEx.007E3C5E
	$-1F5    007E3C47     8A5D 28                  MOV BL, BYTE PTR SS:[EBP+0x28]
	$-1F2    007E3C4A     8BCE                     MOV ECX, ESI
	$-1F0    007E3C4C     6A 01                    PUSH 0x1
	$-1EE    007E3C4E     E8 DD1AF6FF              CALL PathOfEx.00745730
	$-1E9    007E3C53     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$-1E6    007E3C56     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-1E4    007E3C58     881C01                   MOV BYTE PTR DS:[ECX+EAX], BL
	$-1E1    007E3C5B     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-1DE    007E3C5E     B8 00040000              MOV EAX, 0x400
	$-1D9    007E3C63     66:8585 B4020000         TEST WORD PTR SS:[EBP+0x2B4], AX
	$-1D2    007E3C6A     0F84 87000000            JE PathOfEx.007E3CF7
	$-1CC    007E3C70     6A 01                    PUSH 0x1
	$-1CA    007E3C72     8BCE                     MOV ECX, ESI
	$-1C8    007E3C74     E8 B71AF6FF              CALL PathOfEx.00745730
	$-1C3    007E3C79     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-1C0    007E3C7C     32DB                     XOR BL, BL
	$-1BE    007E3C7E     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-1BC    007E3C80     8A85 D2020000            MOV AL, BYTE PTR SS:[EBP+0x2D2]
	$-1B6    007E3C86     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-1B3    007E3C89     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-1B0    007E3C8C     389D D2020000            CMP BYTE PTR SS:[EBP+0x2D2], BL
	$-1AA    007E3C92     76 63                    JBE SHORT PathOfEx.007E3CF7
	$-1A8    007E3C94     6A 02                    PUSH 0x2
	$-1A6    007E3C96     8BCE                     MOV ECX, ESI
	$-1A4    007E3C98     E8 931AF6FF              CALL PathOfEx.00745730
	$-19F    007E3C9D     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-19C    007E3CA0     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-19A    007E3CA2     0FB6FB                   MOVZX EDI, BL
	$-197    007E3CA5     6A 01                    PUSH 0x1
	$-195    007E3CA7     66:8B84BD B6020000       MOV AX, WORD PTR SS:[EBP+EDI*4+0x2B6]
	$-18D    007E3CAF     66:89040A                MOV WORD PTR DS:[EDX+ECX], AX
	$-189    007E3CB3     8BCE                     MOV ECX, ESI
	$-187    007E3CB5     8346 08 02               ADD DWORD PTR DS:[ESI+0x8], 0x2
	$-183    007E3CB9     E8 721AF6FF              CALL PathOfEx.00745730
	$-17E    007E3CBE     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-17B    007E3CC1     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-179    007E3CC3     0FB684BD B9020000        MOVZX EAX, BYTE PTR SS:[EBP+EDI*4+0x2B9]
	$-171    007E3CCB     6A 01                    PUSH 0x1
	$-16F    007E3CCD     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-16C    007E3CD0     8BCE                     MOV ECX, ESI
	$-16A    007E3CD2     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-167    007E3CD5     E8 561AF6FF              CALL PathOfEx.00745730
	$-162    007E3CDA     8B56 08                  MOV EDX, DWORD PTR DS:[ESI+0x8]
	$-15F    007E3CDD     FEC3                     INC BL
	$-15D    007E3CDF     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-15B    007E3CE1     0FB684BD B8020000        MOVZX EAX, BYTE PTR SS:[EBP+EDI*4+0x2B8]
	$-153    007E3CE9     88040A                   MOV BYTE PTR DS:[EDX+ECX], AL
	$-150    007E3CEC     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-14D    007E3CEF     3A9D D2020000            CMP BL, BYTE PTR SS:[EBP+0x2D2]
	$-147    007E3CF5   ^ 72 9D                    JB SHORT PathOfEx.007E3C94
	$-145    007E3CF7     5F                       POP EDI
	$-144    007E3CF8     5E                       POP ESI
	$-143    007E3CF9     5D                       POP EBP
	$-142    007E3CFA     5B                       POP EBX
	$-141    007E3CFB     59                       POP ECX
	$-140    007E3CFC     C2 0400                  RETN 0x4
	$-13D    007E3CFF     CC                       INT3
	$-13C    007E3D00     55                       PUSH EBP
	$-13B    007E3D01     8BEC                     MOV EBP, ESP
	$-139    007E3D03     6A FF                    PUSH -0x1
	$-137    007E3D05     68 AA58D500              PUSH PathOfEx.00D558AA
	$-132    007E3D0A     64:A1 00000000           MOV EAX, DWORD PTR FS:[0]
	$-12C    007E3D10     50                       PUSH EAX
	$-12B    007E3D11     64:8925 00000000         MOV DWORD PTR FS:[0], ESP
	$-124    007E3D18     81EC FC000000            SUB ESP, 0xFC
	$-11E    007E3D1E     53                       PUSH EBX
	$-11D    007E3D1F     56                       PUSH ESI
	$-11C    007E3D20     8B75 08                  MOV ESI, DWORD PTR SS:[EBP+0x8]
	$-119    007E3D23     8BD9                     MOV EBX, ECX
	$-117    007E3D25     57                       PUSH EDI
	$-116    007E3D26     6A 02                    PUSH 0x2
	$-114    007E3D28     8BCE                     MOV ECX, ESI
	$-112    007E3D2A     895D CC                  MOV DWORD PTR SS:[EBP-0x34], EBX
	$-10F    007E3D2D     E8 9EC4CBFF              CALL PathOfEx.004A01D0
	$-10A    007E3D32     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$-107    007E3D35     8D7B 50                  LEA EDI, DWORD PTR DS:[EBX+0x50]
	$-104    007E3D38     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-102    007E3D3A     8B5D 08                  MOV EBX, DWORD PTR SS:[EBP+0x8]
	$-FF     007E3D3D     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD], 0x0
	$-FB     007E3D41     897D EC                  MOV DWORD PTR SS:[EBP-0x14], EDI
	$-F8     007E3D44     0FB71401                 MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$-F4     007E3D48     8D41 02                  LEA EAX, DWORD PTR DS:[ECX+0x2]
	$-F1     007E3D4B     8946 08                  MOV DWORD PTR DS:[ESI+0x8], EAX
	$-EE     007E3D4E     B9 03000000              MOV ECX, 0x3
	$-E9     007E3D53     B8 01000000              MOV EAX, 0x1
	$-E4     007E3D58     0FB7F2                   MOVZX ESI, DX
	$-E1     007E3D5B     D1C0                     ROL EAX, 1
	$-DF     007E3D5D     8955 E4                  MOV DWORD PTR SS:[EBP-0x1C], EDX
	$-DC     007E3D60     8945 D8                  MOV DWORD PTR SS:[EBP-0x28], EAX
	$-D9     007E3D63     8975 C8                  MOV DWORD PTR SS:[EBP-0x38], ESI
	$-D6     007E3D66     894D DC                  MOV DWORD PTR SS:[EBP-0x24], ECX
	$-D3     007E3D69     0F1F80 00000000          NOP DWORD PTR DS:[EAX]
	$-CC     007E3D70     85C6                     TEST ESI, EAX
	$-CA     007E3D72     74 51                    JE SHORT PathOfEx.007E3DC5
	$-C8     007E3D74     68 80000000              PUSH 0x80
	$-C3     007E3D79     8BCB                     MOV ECX, EBX
	$-C1     007E3D7B     E8 50C4CBFF              CALL PathOfEx.004A01D0
	$-BC     007E3D80     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$-BA     007E3D82     8B73 08                  MOV ESI, DWORD PTR DS:[EBX+0x8]
	$-B7     007E3D85     03C6                     ADD EAX, ESI
	$-B5     007E3D87     68 80000000              PUSH 0x80
	$-B0     007E3D8C     50                       PUSH EAX
	$-AF     007E3D8D     8D85 F8FEFFFF            LEA EAX, DWORD PTR SS:[EBP-0x108]
	$-A9     007E3D93     50                       PUSH EAX
	$-A8     007E3D94     E8 F7EF4A00              CALL PathOfEx.00C92D90
	$-A3     007E3D99     8D86 80000000            LEA EAX, DWORD PTR DS:[ESI+0x80]
	$-9D     007E3D9F     B9 20000000              MOV ECX, 0x20
	$-98     007E3DA4     8943 08                  MOV DWORD PTR DS:[EBX+0x8], EAX
	$-95     007E3DA7     8DB5 F8FEFFFF            LEA ESI, DWORD PTR SS:[EBP-0x108]
	$-8F     007E3DAD     8B45 D8                  MOV EAX, DWORD PTR SS:[EBP-0x28]
	$-8C     007E3DB0     B2 01                    MOV DL, 0x1
	$-8A     007E3DB2     F3:A5                    REP MOVS DWORD PTR ES:[EDI], DWORD PTR DS:[ESI]
	$-88     007E3DB4     8B7D EC                  MOV EDI, DWORD PTR SS:[EBP-0x14]
	$-85     007E3DB7     83C4 0C                  ADD ESP, 0xC
	$-82     007E3DBA     8B4D DC                  MOV ECX, DWORD PTR SS:[EBP-0x24]
	$-7F     007E3DBD     8B75 C8                  MOV ESI, DWORD PTR SS:[EBP-0x38]
	$-7C     007E3DC0     8855 F3                  MOV BYTE PTR SS:[EBP-0xD], DL
	$-79     007E3DC3     EB 03                    JMP SHORT PathOfEx.007E3DC8
	$-77     007E3DC5     8A55 F3                  MOV DL, BYTE PTR SS:[EBP-0xD]
	$-74     007E3DC8     83EF 80                  SUB EDI, -0x80
	$-71     007E3DCB     D1C0                     ROL EAX, 1
	$-6F     007E3DCD     83E9 01                  SUB ECX, 0x1
	$-6C     007E3DD0     897D EC                  MOV DWORD PTR SS:[EBP-0x14], EDI
	$-69     007E3DD3     8945 D8                  MOV DWORD PTR SS:[EBP-0x28], EAX
	$-66     007E3DD6     894D DC                  MOV DWORD PTR SS:[EBP-0x24], ECX
	$-63     007E3DD9   ^ 75 95                    JNZ SHORT PathOfEx.007E3D70
	$-61     007E3DDB     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$-5E     007E3DDE     8B5D CC                  MOV EBX, DWORD PTR SS:[EBP-0x34]
	$-5B     007E3DE1     8B75 08                  MOV ESI, DWORD PTR SS:[EBP+0x8]
	$-58     007E3DE4     A8 20                    TEST AL, 0x20
	$-56     007E3DE6     74 29                    JE SHORT PathOfEx.007E3E11
	$-54     007E3DE8     6A 2C                    PUSH 0x2C
	$-52     007E3DEA     8BCE                     MOV ECX, ESI
	$-50     007E3DEC     E8 DFC3CBFF              CALL PathOfEx.004A01D0
	$-4B     007E3DF1     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-49     007E3DF3     0346 08                  ADD EAX, DWORD PTR DS:[ESI+0x8]
	$-46     007E3DF6     6A 2C                    PUSH 0x2C
	$-44     007E3DF8     50                       PUSH EAX
	$-43     007E3DF9     8D83 D0010000            LEA EAX, DWORD PTR DS:[EBX+0x1D0]
	$-3D     007E3DFF     50                       PUSH EAX
	$-3C     007E3E00     E8 8BEF4A00              CALL PathOfEx.00C92D90
	$-37     007E3E05     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$-34     007E3E08     83C4 0C                  ADD ESP, 0xC
	$-31     007E3E0B     8346 08 2C               ADD DWORD PTR DS:[ESI+0x8], 0x2C
	$-2D     007E3E0F     B2 01                    MOV DL, 0x1
	$-2B     007E3E11     A9 00020000              TEST EAX, 0x200
	$-26     007E3E16     74 20                    JE SHORT PathOfEx.007E3E38
	$-24     007E3E18     6A 08                    PUSH 0x8
	$-22     007E3E1A     8BCE                     MOV ECX, ESI
	$-20     007E3E1C     E8 AFC3CBFF              CALL PathOfEx.004A01D0
	$-1B     007E3E21     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$-19     007E3E23     0346 08                  ADD EAX, DWORD PTR DS:[ESI+0x8]
	$-16     007E3E26     F30F7E00                 MOVQ XMM0, QWORD PTR DS:[EAX]
	$-12     007E3E2A     660FD683 FC010000        MOVQ QWORD PTR DS:[EBX+0x1FC], XMM0
	$-A      007E3E32     8346 08 08               ADD DWORD PTR DS:[ESI+0x8], 0x8
	$-6      007E3E36     EB 04                    JMP SHORT PathOfEx.007E3E3C
	$-4      007E3E38     84D2                     TEST DL, DL
	$-2      007E3E3A     74 26                    JE SHORT PathOfEx.007E3E62
	$ ==>    007E3E3C     8BB3 D4020000            MOV ESI, DWORD PTR DS:[EBX+0x2D4]                  ; g_o_cur_quest_info_
	$+6      007E3E42     3BB3 D8020000            CMP ESI, DWORD PTR DS:[EBX+0x2D8]
	$+C      007E3E48     74 18                    JE SHORT PathOfEx.007E3E62
	$+E      007E3E4A     66:0F1F4400 00           NOP WORD PTR DS:[EAX+EAX]
	$+14     007E3E50     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$+16     007E3E52     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+18     007E3E54     FF50 08                  CALL NEAR DWORD PTR DS:[EAX+0x8]                   ; call g_c_player_quest_info_vfn_
	$+1B     007E3E57     83C6 04                  ADD ESI, 0x4
	$+1E     007E3E5A     3BB3 D8020000            CMP ESI, DWORD PTR DS:[EBX+0x2D8]
	$+24     007E3E60   ^ 75 EE                    JNZ SHORT PathOfEx.007E3E50
	$+26     007E3E62     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+29     007E3E65     8B75 08                  MOV ESI, DWORD PTR SS:[EBP+0x8]
	$+2C     007E3E68     C745 C8 00000000         MOV DWORD PTR SS:[EBP-0x38], 0x0
	$+33     007E3E6F     A8 01                    TEST AL, 0x1
	$+35     007E3E71     74 1D                    JE SHORT PathOfEx.007E3E90
	$+37     007E3E73     6A 04                    PUSH 0x4
	$+39     007E3E75     8BCE                     MOV ECX, ESI
	$+3B     007E3E77     E8 54C3CBFF              CALL PathOfEx.004A01D0
	$+40     007E3E7C     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$+43     007E3E7F     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+45     007E3E81     8B0401                   MOV EAX, DWORD PTR DS:[ECX+EAX]
	$+48     007E3E84     8945 C8                  MOV DWORD PTR SS:[EBP-0x38], EAX
	$+4B     007E3E87     8D41 04                  LEA EAX, DWORD PTR DS:[ECX+0x4]
	$+4E     007E3E8A     8946 08                  MOV DWORD PTR DS:[ESI+0x8], EAX
	$+51     007E3E8D     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+54     007E3E90     8A4B 48                  MOV CL, BYTE PTR DS:[EBX+0x48]
	$+57     007E3E93     884D E3                  MOV BYTE PTR SS:[EBP-0x1D], CL
	$+5A     007E3E96     A8 10                    TEST AL, 0x10
	$+5C     007E3E98     74 1A                    JE SHORT PathOfEx.007E3EB4
	$+5E     007E3E9A     6A 01                    PUSH 0x1
	$+60     007E3E9C     8BCE                     MOV ECX, ESI
	$+62     007E3E9E     E8 2DC3CBFF              CALL PathOfEx.004A01D0
	$+67     007E3EA3     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$+6A     007E3EA6     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$+6C     007E3EA8     8A0401                   MOV AL, BYTE PTR DS:[ECX+EAX]
	$+6F     007E3EAB     8843 48                  MOV BYTE PTR DS:[EBX+0x48], AL
	$+72     007E3EAE     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$+75     007E3EB1     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+78     007E3EB4     8A4B 49                  MOV CL, BYTE PTR DS:[EBX+0x49]
	$+7B     007E3EB7     884D E2                  MOV BYTE PTR SS:[EBP-0x1E], CL
	$+7E     007E3EBA     A9 00080000              TEST EAX, 0x800
	$+83     007E3EBF     74 1A                    JE SHORT PathOfEx.007E3EDB
	$+85     007E3EC1     6A 01                    PUSH 0x1
	$+87     007E3EC3     8BCE                     MOV ECX, ESI
	$+89     007E3EC5     E8 06C3CBFF              CALL PathOfEx.004A01D0
	$+8E     007E3ECA     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$+91     007E3ECD     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$+93     007E3ECF     8A0401                   MOV AL, BYTE PTR DS:[ECX+EAX]
	$+96     007E3ED2     8843 49                  MOV BYTE PTR DS:[EBX+0x49], AL
	$+99     007E3ED5     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$+9C     007E3ED8     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+9F     007E3EDB     A8 40                    TEST AL, 0x40
	$+A1     007E3EDD     0F84 3F010000            JE PathOfEx.007E4022
	$+A7     007E3EE3     8D8B 08020000            LEA ECX, DWORD PTR DS:[EBX+0x208]
	$+AD     007E3EE9     8D93 98020000            LEA EDX, DWORD PTR DS:[EBX+0x298]
	$+B3     007E3EEF     3BCA                     CMP ECX, EDX
	$+B5     007E3EF1     0F84 2B010000            JE PathOfEx.007E4022
	$+BB     007E3EF7     83C1 04                  ADD ECX, 0x4
	$+BE     007E3EFA     B8 F4FDFFFF              MOV EAX, -0x20C
	$+C3     007E3EFF     2BC3                     SUB EAX, EBX
	$+C5     007E3F01     894D EC                  MOV DWORD PTR SS:[EBP-0x14], ECX
	$+C8     007E3F04     8945 D4                  MOV DWORD PTR SS:[EBP-0x2C], EAX
	$+CB     007E3F07     03C1                     ADD EAX, ECX
	$+CD     007E3F09     C1F8 04                  SAR EAX, 0x4
	$+D0     007E3F0C     40                       INC EAX
	$+D1     007E3F0D     8945 DC                  MOV DWORD PTR SS:[EBP-0x24], EAX
	$+D4     007E3F10     83F8 09                  CMP EAX, 0x9
	$+D7     007E3F13     0F8D E9000000            JGE PathOfEx.007E4002
	$+DD     007E3F19     8B39                     MOV EDI, DWORD PTR DS:[ECX]
	$+DF     007E3F1B     8B49 FC                  MOV ECX, DWORD PTR DS:[ECX-0x4]
	$+E2     007E3F1E     894D B4                  MOV DWORD PTR SS:[EBP-0x4C], ECX
	$+E5     007E3F21     8BCB                     MOV ECX, EBX
	$+E7     007E3F23     50                       PUSH EAX
	$+E8     007E3F24     E8 0704DEFF              CALL PathOfEx.005C4330
	$+ED     007E3F29     8945 D8                  MOV DWORD PTR SS:[EBP-0x28], EAX
	$+F0     007E3F2C     8B45 EC                  MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+F3     007E3F2F     6A 04                    PUSH 0x4
	$+F5     007E3F31     8B48 04                  MOV ECX, DWORD PTR DS:[EAX+0x4]
	$+F8     007E3F34     8B40 08                  MOV EAX, DWORD PTR DS:[EAX+0x8]
	$+FB     007E3F37     894D B0                  MOV DWORD PTR SS:[EBP-0x50], ECX
	$+FE     007E3F3A     8BCE                     MOV ECX, ESI
	$+100    007E3F3C     8945 C0                  MOV DWORD PTR SS:[EBP-0x40], EAX
	$+103    007E3F3F     E8 8CC2CBFF              CALL PathOfEx.004A01D0
	$+108    007E3F44     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$+10A    007E3F46     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$+10D    007E3F49     6A 04                    PUSH 0x4
	$+10F    007E3F4B     8B0401                   MOV EAX, DWORD PTR DS:[ECX+EAX]
	$+112    007E3F4E     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+115    007E3F51     8941 FC                  MOV DWORD PTR DS:[ECX-0x4], EAX
	$+118    007E3F54     8BCE                     MOV ECX, ESI
	$+11A    007E3F56     8346 08 04               ADD DWORD PTR DS:[ESI+0x8], 0x4
	$+11E    007E3F5A     E8 71C2CBFF              CALL PathOfEx.004A01D0
	$+123    007E3F5F     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$+125    007E3F61     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+0x8]
	$+128    007E3F64     6A 08                    PUSH 0x8
	$+12A    007E3F66     8B0401                   MOV EAX, DWORD PTR DS:[ECX+EAX]
	$+12D    007E3F69     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+130    007E3F6C     8901                     MOV DWORD PTR DS:[ECX], EAX
	$+132    007E3F6E     8BCE                     MOV ECX, ESI
	$+134    007E3F70     8346 08 04               ADD DWORD PTR DS:[ESI+0x8], 0x4
	$+138    007E3F74     E8 57C2CBFF              CALL PathOfEx.004A01D0
	$+13D    007E3F79     8B4E 08                  MOV ECX, DWORD PTR DS:[ESI+0x8]
	$+140    007E3F7C     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+142    007E3F7E     8B7408 04                MOV ESI, DWORD PTR DS:[EAX+ECX+0x4]
	$+146    007E3F82     8B1408                   MOV EDX, DWORD PTR DS:[EAX+ECX]
	$+149    007E3F85     8D41 08                  LEA EAX, DWORD PTR DS:[ECX+0x8]
	$+14C    007E3F88     8B4D 08                  MOV ECX, DWORD PTR SS:[EBP+0x8]
	$+14F    007E3F8B     8941 08                  MOV DWORD PTR DS:[ECX+0x8], EAX
	$+152    007E3F8E     8B45 EC                  MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+155    007E3F91     8B08                     MOV ECX, DWORD PTR DS:[EAX]
	$+157    007E3F93     8970 08                  MOV DWORD PTR DS:[EAX+0x8], ESI
	$+15A    007E3F96     8B75 DC                  MOV ESI, DWORD PTR SS:[EBP-0x24]
	$+15D    007E3F99     8950 04                  MOV DWORD PTR DS:[EAX+0x4], EDX
	$+160    007E3F9C     3BCF                     CMP ECX, EDI
	$+162    007E3F9E     74 09                    JE SHORT PathOfEx.007E3FA9
	$+164    007E3FA0     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+166    007E3FA2     51                       PUSH ECX
	$+167    007E3FA3     56                       PUSH ESI
	$+168    007E3FA4     8BCB                     MOV ECX, EBX
	$+16A    007E3FA6     FF50 48                  CALL NEAR DWORD PTR DS:[EAX+0x48]
	$+16D    007E3FA9     56                       PUSH ESI
	$+16E    007E3FAA     8BCB                     MOV ECX, EBX
	$+170    007E3FAC     E8 7F03DEFF              CALL PathOfEx.005C4330
	$+175    007E3FB1     8BF8                     MOV EDI, EAX
	$+177    007E3FB3     3B7D D8                  CMP EDI, DWORD PTR SS:[EBP-0x28]
	$+17A    007E3FB6     74 09                    JE SHORT PathOfEx.007E3FC1
	$+17C    007E3FB8     8B13                     MOV EDX, DWORD PTR DS:[EBX]
	$+17E    007E3FBA     8BCB                     MOV ECX, EBX
	$+180    007E3FBC     57                       PUSH EDI
	$+181    007E3FBD     56                       PUSH ESI
	$+182    007E3FBE     FF52 4C                  CALL NEAR DWORD PTR DS:[EDX+0x4C]
	$+185    007E3FC1     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+188    007E3FC4     8B41 FC                  MOV EAX, DWORD PTR DS:[ECX-0x4]
	$+18B    007E3FC7     8945 DC                  MOV DWORD PTR SS:[EBP-0x24], EAX
	$+18E    007E3FCA     3B45 B4                  CMP EAX, DWORD PTR SS:[EBP-0x4C]
	$+191    007E3FCD     74 18                    JE SHORT PathOfEx.007E3FE7
	$+193    007E3FCF     3B7D D8                  CMP EDI, DWORD PTR SS:[EBP-0x28]
	$+196    007E3FD2     8BCB                     MOV ECX, EBX
	$+198    007E3FD4     8B13                     MOV EDX, DWORD PTR DS:[EBX]
	$+19A    007E3FD6     0F95C0                   SETNE AL
	$+19D    007E3FD9     0FB6C0                   MOVZX EAX, AL
	$+1A0    007E3FDC     50                       PUSH EAX
	$+1A1    007E3FDD     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
	$+1A4    007E3FE0     56                       PUSH ESI
	$+1A5    007E3FE1     FF52 50                  CALL NEAR DWORD PTR DS:[EDX+0x50]
	$+1A8    007E3FE4     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+1AB    007E3FE7     8B45 B0                  MOV EAX, DWORD PTR SS:[EBP-0x50]
	$+1AE    007E3FEA     3941 04                  CMP DWORD PTR DS:[ECX+0x4], EAX
	$+1B1    007E3FED     75 08                    JNZ SHORT PathOfEx.007E3FF7
	$+1B3    007E3FEF     8B45 C0                  MOV EAX, DWORD PTR SS:[EBP-0x40]
	$+1B6    007E3FF2     3941 08                  CMP DWORD PTR DS:[ECX+0x8], EAX
	$+1B9    007E3FF5     74 0B                    JE SHORT PathOfEx.007E4002
	$+1BB    007E3FF7     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+1BD    007E3FF9     8BCB                     MOV ECX, EBX
	$+1BF    007E3FFB     56                       PUSH ESI
	$+1C0    007E3FFC     FF50 54                  CALL NEAR DWORD PTR DS:[EAX+0x54]
	$+1C3    007E3FFF     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-0x14]
	$+1C6    007E4002     83C1 10                  ADD ECX, 0x10
	$+1C9    007E4005     8DB3 98020000            LEA ESI, DWORD PTR DS:[EBX+0x298]
	$+1CF    007E400B     894D EC                  MOV DWORD PTR SS:[EBP-0x14], ECX
	$+1D2    007E400E     8D41 FC                  LEA EAX, DWORD PTR DS:[ECX-0x4]
	$+1D5    007E4011     3BC6                     CMP EAX, ESI
	$+1D7    007E4013     8B75 08                  MOV ESI, DWORD PTR SS:[EBP+0x8]
	$+1DA    007E4016     8B45 D4                  MOV EAX, DWORD PTR SS:[EBP-0x2C]
	$+1DD    007E4019   ^ 0F85 E8FEFFFF            JNZ PathOfEx.007E3F07
	$+1E3    007E401F     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+1E6    007E4022     8B7D 08                  MOV EDI, DWORD PTR SS:[EBP+0x8]
	$+1E9    007E4025     84C0                     TEST AL, AL
	$+1EB    007E4027     0F89 57010000            JNS PathOfEx.007E4184
	$+1F1    007E402D     6A 02                    PUSH 0x2
	$+1F3    007E402F     8BCF                     MOV ECX, EDI
	$+1F5    007E4031     E8 9AC1CBFF              CALL PathOfEx.004A01D0
	$+1FA    007E4036     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+1FD    007E4039     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+1FF    007E403B     0FB71401                 MOVZX EDX, WORD PTR DS:[ECX+EAX]
	$+203    007E403F     8D41 02                  LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+206    007E4042     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+209    007E4045     66:85D2                  TEST DX, DX
	$+20C    007E4048     0F84 40020000            JE PathOfEx.007E428E
	$+212    007E404E     83EC 10                  SUB ESP, 0x10
	$+215    007E4051     8BC2                     MOV EAX, EDX
	$+217    007E4053     8945 08                  MOV DWORD PTR SS:[EBP+0x8], EAX
	$+21A    007E4056     E8 7524F3FF              CALL PathOfEx.007164D0
	$+21F    007E405B     8BC8                     MOV ECX, EAX
	$+221    007E405D     E8 9E140000              CALL PathOfEx.007E5500
	$+226    007E4062     8BF0                     MOV ESI, EAX
	$+228    007E4064     8975 D4                  MOV DWORD PTR SS:[EBP-0x2C], ESI
	$+22B    007E4067     8D45 08                  LEA EAX, DWORD PTR SS:[EBP+0x8]
	$+22E    007E406A     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4], 0x0
	$+235    007E4071     50                       PUSH EAX
	$+236    007E4072     8D55 D4                  LEA EDX, DWORD PTR SS:[EBP-0x2C]
	$+239    007E4075     8D4D BC                  LEA ECX, DWORD PTR SS:[EBP-0x44]
	$+23C    007E4078     E8 930DF7FF              CALL PathOfEx.00754E10
	$+241    007E407D     8BD0                     MOV EDX, EAX
	$+243    007E407F     8D8B 98020000            LEA ECX, DWORD PTR DS:[EBX+0x298]
	$+249    007E4085     83C4 04                  ADD ESP, 0x4
	$+24C    007E4088     8955 08                  MOV DWORD PTR SS:[EBP+0x8], EDX
	$+24F    007E408B     3BCA                     CMP ECX, EDX
	$+251    007E408D     74 2C                    JE SHORT PathOfEx.007E40BB
	$+253    007E408F     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+255    007E4091     85C0                     TEST EAX, EAX
	$+257    007E4093     74 1C                    JE SHORT PathOfEx.007E40B1
	$+259    007E4095     50                       PUSH EAX
	$+25A    007E4096     E8 3524F3FF              CALL PathOfEx.007164D0
	$+25F    007E409B     8BC8                     MOV ECX, EAX
	$+261    007E409D     E8 BE1FF3FF              CALL PathOfEx.00716060
	$+266    007E40A2     8B55 08                  MOV EDX, DWORD PTR SS:[EBP+0x8]
	$+269    007E40A5     8D8B 98020000            LEA ECX, DWORD PTR DS:[EBX+0x298]
	$+26F    007E40AB     C701 00000000            MOV DWORD PTR DS:[ECX], 0x0
	$+275    007E40B1     8B02                     MOV EAX, DWORD PTR DS:[EDX]
	$+277    007E40B3     8901                     MOV DWORD PTR DS:[ECX], EAX
	$+279    007E40B5     C702 00000000            MOV DWORD PTR DS:[EDX], 0x0
	$+27F    007E40BB     8B42 04                  MOV EAX, DWORD PTR DS:[EDX+0x4]
	$+282    007E40BE     8941 04                  MOV DWORD PTR DS:[ECX+0x4], EAX
	$+285    007E40C1     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4], 0x1
	$+289    007E40C5     8B45 BC                  MOV EAX, DWORD PTR SS:[EBP-0x44]
	$+28C    007E40C8     85C0                     TEST EAX, EAX
	$+28E    007E40CA     74 14                    JE SHORT PathOfEx.007E40E0
	$+290    007E40CC     50                       PUSH EAX
	$+291    007E40CD     E8 FE23F3FF              CALL PathOfEx.007164D0
	$+296    007E40D2     8BC8                     MOV ECX, EAX
	$+298    007E40D4     E8 871FF3FF              CALL PathOfEx.00716060
	$+29D    007E40D9     C745 BC 00000000         MOV DWORD PTR SS:[EBP-0x44], 0x0
	$+2A4    007E40E0     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4], 0x2
	$+2AB    007E40E7     85F6                     TEST ESI, ESI
	$+2AD    007E40E9     74 0D                    JE SHORT PathOfEx.007E40F8
	$+2AF    007E40EB     56                       PUSH ESI
	$+2B0    007E40EC     E8 DF23F3FF              CALL PathOfEx.007164D0
	$+2B5    007E40F1     8BC8                     MOV ECX, EAX
	$+2B7    007E40F3     E8 681FF3FF              CALL PathOfEx.00716060
	$+2BC    007E40F8     6A 01                    PUSH 0x1
	$+2BE    007E40FA     8BCF                     MOV ECX, EDI
	$+2C0    007E40FC     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+2C7    007E4103     E8 C8C0CBFF              CALL PathOfEx.004A01D0
	$+2CC    007E4108     8B0F                     MOV ECX, DWORD PTR DS:[EDI]
	$+2CE    007E410A     8B47 08                  MOV EAX, DWORD PTR DS:[EDI+0x8]
	$+2D1    007E410D     6A 01                    PUSH 0x1
	$+2D3    007E410F     8A0401                   MOV AL, BYTE PTR DS:[ECX+EAX]
	$+2D6    007E4112     8BCF                     MOV ECX, EDI
	$+2D8    007E4114     8883 A0020000            MOV BYTE PTR DS:[EBX+0x2A0], AL
	$+2DE    007E411A     FF47 08                  INC DWORD PTR DS:[EDI+0x8]
	$+2E1    007E411D     E8 AEC0CBFF              CALL PathOfEx.004A01D0
	$+2E6    007E4122     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+2E9    007E4125     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+2EB    007E4127     8A1401                   MOV DL, BYTE PTR DS:[ECX+EAX]
	$+2EE    007E412A     8D41 01                  LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+2F1    007E412D     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+2F4    007E4130     8D8B A4020000            LEA ECX, DWORD PTR DS:[EBX+0x2A4]
	$+2FA    007E4136     0FB6C2                   MOVZX EAX, DL
	$+2FD    007E4139     50                       PUSH EAX
	$+2FE    007E413A     8855 0B                  MOV BYTE PTR SS:[EBP+0xB], DL
	$+301    007E413D     8945 EC                  MOV DWORD PTR SS:[EBP-0x14], EAX
	$+304    007E4140     E8 8B73F1FF              CALL PathOfEx.006FB4D0
	$+309    007E4145     807D 0B 00               CMP BYTE PTR SS:[EBP+0xB], 0x0
	$+30D    007E4149     74 28                    JE SHORT PathOfEx.007E4173
	$+30F    007E414B     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+312    007E414E     8BB3 A4020000            MOV ESI, DWORD PTR DS:[EBX+0x2A4]
	$+318    007E4154     8BCF                     MOV ECX, EDI
	$+31A    007E4156     E8 75C0CBFF              CALL PathOfEx.004A01D0
	$+31F    007E415B     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+321    007E415D     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+324    007E4160     0347 08                  ADD EAX, DWORD PTR DS:[EDI+0x8]
	$+327    007E4163     50                       PUSH EAX
	$+328    007E4164     56                       PUSH ESI
	$+329    007E4165     E8 26EC4A00              CALL PathOfEx.00C92D90
	$+32E    007E416A     8B45 EC                  MOV EAX, DWORD PTR SS:[EBP-0x14]
	$+331    007E416D     83C4 0C                  ADD ESP, 0xC
	$+334    007E4170     0147 08                  ADD DWORD PTR DS:[EDI+0x8], EAX
	$+337    007E4173     8DB3 98020000            LEA ESI, DWORD PTR DS:[EBX+0x298]
	$+33D    007E4179     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+33F    007E417B     8BCB                     MOV ECX, EBX
	$+341    007E417D     56                       PUSH ESI
	$+342    007E417E     FF50 5C                  CALL NEAR DWORD PTR DS:[EAX+0x5C]
	$+345    007E4181     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+348    007E4184     A9 00010000              TEST EAX, 0x100
	$+34D    007E4189     74 24                    JE SHORT PathOfEx.007E41AF
	$+34F    007E418B     6A 01                    PUSH 0x1
	$+351    007E418D     8BCF                     MOV ECX, EDI
	$+353    007E418F     E8 3CC0CBFF              CALL PathOfEx.004A01D0
	$+358    007E4194     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+35B    007E4197     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+35D    007E4199     8A1401                   MOV DL, BYTE PTR DS:[ECX+EAX]
	$+360    007E419C     8D41 01                  LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+363    007E419F     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+366    007E41A2     8BCB                     MOV ECX, EBX
	$+368    007E41A4     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+36A    007E41A6     8853 28                  MOV BYTE PTR DS:[EBX+0x28], DL
	$+36D    007E41A9     FF50 58                  CALL NEAR DWORD PTR DS:[EAX+0x58]
	$+370    007E41AC     8B45 E4                  MOV EAX, DWORD PTR SS:[EBP-0x1C]
	$+373    007E41AF     A9 00040000              TEST EAX, 0x400
	$+378    007E41B4     0F84 43020000            JE PathOfEx.007E43FD
	$+37E    007E41BA     6A 01                    PUSH 0x1
	$+380    007E41BC     8BCF                     MOV ECX, EDI
	$+382    007E41BE     E8 0DC0CBFF              CALL PathOfEx.004A01D0
	$+387    007E41C3     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+38A    007E41C6     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+38C    007E41C8     8A1401                   MOV DL, BYTE PTR DS:[ECX+EAX]
	$+38F    007E41CB     8D41 01                  LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+392    007E41CE     8855 EB                  MOV BYTE PTR SS:[EBP-0x15], DL
	$+395    007E41D1     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+398    007E41D4     84D2                     TEST DL, DL
	$+39A    007E41D6     74 67                    JE SHORT PathOfEx.007E423F
	$+39C    007E41D8     8D75 97                  LEA ESI, DWORD PTR SS:[EBP-0x69]
	$+39F    007E41DB     0FB6DA                   MOVZX EBX, DL
	$+3A2    007E41DE     66:90                    NOP
	$+3A4    007E41E0     6A 02                    PUSH 0x2
	$+3A6    007E41E2     8BCF                     MOV ECX, EDI
	$+3A8    007E41E4     E8 E7BFCBFF              CALL PathOfEx.004A01D0
	$+3AD    007E41E9     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+3B0    007E41EC     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+3B2    007E41EE     6A 01                    PUSH 0x1
	$+3B4    007E41F0     66:8B0401                MOV AX, WORD PTR DS:[ECX+EAX]
	$+3B8    007E41F4     66:8946 FD               MOV WORD PTR DS:[ESI-0x3], AX
	$+3BC    007E41F8     8D41 02                  LEA EAX, DWORD PTR DS:[ECX+0x2]
	$+3BF    007E41FB     8BCF                     MOV ECX, EDI
	$+3C1    007E41FD     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+3C4    007E4200     E8 CBBFCBFF              CALL PathOfEx.004A01D0
	$+3C9    007E4205     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+3CC    007E4208     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+3CE    007E420A     6A 01                    PUSH 0x1
	$+3D0    007E420C     0FB60401                 MOVZX EAX, BYTE PTR DS:[ECX+EAX]
	$+3D4    007E4210     8806                     MOV BYTE PTR DS:[ESI], AL
	$+3D6    007E4212     8D41 01                  LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+3D9    007E4215     8BCF                     MOV ECX, EDI
	$+3DB    007E4217     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+3DE    007E421A     E8 B1BFCBFF              CALL PathOfEx.004A01D0
	$+3E3    007E421F     8B4F 08                  MOV ECX, DWORD PTR DS:[EDI+0x8]
	$+3E6    007E4222     8D76 04                  LEA ESI, DWORD PTR DS:[ESI+0x4]
	$+3E9    007E4225     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$+3EB    007E4227     0FB60401                 MOVZX EAX, BYTE PTR DS:[ECX+EAX]
	$+3EF    007E422B     8846 FB                  MOV BYTE PTR DS:[ESI-0x5], AL
	$+3F2    007E422E     8D41 01                  LEA EAX, DWORD PTR DS:[ECX+0x1]
	$+3F5    007E4231     8947 08                  MOV DWORD PTR DS:[EDI+0x8], EAX
	$+3F8    007E4234     83EB 01                  SUB EBX, 0x1
	$+3FB    007E4237   ^ 75 A7                    JNZ SHORT PathOfEx.007E41E0
	$+3FD    007E4239     8B5D CC                  MOV EBX, DWORD PTR SS:[EBP-0x34]
	$+400    007E423C     8A55 EB                  MOV DL, BYTE PTR SS:[EBP-0x15]
	$+403    007E423F     8A83 D2020000            MOV AL, BYTE PTR DS:[EBX+0x2D2]
	$+409    007E4245     8D4D 94                  LEA ECX, DWORD PTR SS:[EBP-0x6C]
	$+40C    007E4248     0FB6FA                   MOVZX EDI, DL
	$+40F    007E424B     C645 0B 00               MOV BYTE PTR SS:[EBP+0xB], 0x0
	$+413    007E424F     897D D4                  MOV DWORD PTR SS:[EBP-0x2C], EDI
	$+416    007E4252     8845 F3                  MOV BYTE PTR SS:[EBP-0xD], AL
	$+419    007E4255     8D0CB9                   LEA ECX, DWORD PTR DS:[ECX+EDI*4]
	$+41C    007E4258     84C0                     TEST AL, AL
	$+41E    007E425A     0F84 8F000000            JE PathOfEx.007E42EF
	$+424    007E4260     8DB3 B6020000            LEA ESI, DWORD PTR DS:[EBX+0x2B6]
	$+42A    007E4266     0FB6F8                   MOVZX EDI, AL
	$+42D    007E4269     8A5D 0B                  MOV BL, BYTE PTR SS:[EBP+0xB]
	$+430    007E426C     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$+434    007E4270     8D45 94                  LEA EAX, DWORD PTR SS:[EBP-0x6C]
	$+437    007E4273     8BD0                     MOV EDX, EAX
	$+439    007E4275     3BD1                     CMP EDX, ECX
	$+43B    007E4277     0FB716                   MOVZX EDX, WORD PTR DS:[ESI]
	$+43E    007E427A     74 55                    JE SHORT PathOfEx.007E42D1
	$+440    007E427C     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$+444    007E4280     66:3910                  CMP WORD PTR DS:[EAX], DX
	$+447    007E4283     74 48                    JE SHORT PathOfEx.007E42CD
	$+449    007E4285     83C0 04                  ADD EAX, 0x4
	$+44C    007E4288     3BC1                     CMP EAX, ECX
	$+44E    007E428A   ^ 75 F4                    JNZ SHORT PathOfEx.007E4280
	$+450    007E428C     EB 43                    JMP SHORT PathOfEx.007E42D1
	$+452    007E428E     8DB3 98020000            LEA ESI, DWORD PTR DS:[EBX+0x298]
	$+458    007E4294     8D45 D0                  LEA EAX, DWORD PTR SS:[EBP-0x30]
	$+45B    007E4297     3BF0                     CMP ESI, EAX
	$+45D    007E4299     74 1F                    JE SHORT PathOfEx.007E42BA
	$+45F    007E429B     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+461    007E429D     85C0                     TEST EAX, EAX
	$+463    007E429F     74 13                    JE SHORT PathOfEx.007E42B4
	$+465    007E42A1     50                       PUSH EAX
	$+466    007E42A2     E8 2922F3FF              CALL PathOfEx.007164D0
	$+46B    007E42A7     8BC8                     MOV ECX, EAX
	$+46D    007E42A9     E8 B21DF3FF              CALL PathOfEx.00716060
	$+472    007E42AE     C706 00000000            MOV DWORD PTR DS:[ESI], 0x0
	$+478    007E42B4     C706 00000000            MOV DWORD PTR DS:[ESI], 0x0
	$+47E    007E42BA     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4], 0x0
	$+485    007E42C1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4], -0x1
	$+48C    007E42C8   ^ E9 ACFEFFFF              JMP PathOfEx.007E4179
	$+491    007E42CD     3BC1                     CMP EAX, ECX
	$+493    007E42CF     75 0A                    JNZ SHORT PathOfEx.007E42DB
	$+495    007E42D1     0FB6C3                   MOVZX EAX, BL
	$+498    007E42D4     FEC3                     INC BL
	$+49A    007E42D6     66:895445 B8             MOV WORD PTR SS:[EBP+EAX*2-0x48], DX
	$+49F    007E42DB     83C6 04                  ADD ESI, 0x4
	$+4A2    007E42DE     83EF 01                  SUB EDI, 0x1
	$+4A5    007E42E1   ^ 75 8D                    JNZ SHORT PathOfEx.007E4270
	$+4A7    007E42E3     8B7D D4                  MOV EDI, DWORD PTR SS:[EBP-0x2C]
	$+4AA    007E42E6     8A45 F3                  MOV AL, BYTE PTR SS:[EBP-0xD]
	$+4AD    007E42E9     885D 0B                  MOV BYTE PTR SS:[EBP+0xB], BL
	$+4B0    007E42EC     8B5D CC                  MOV EBX, DWORD PTR SS:[EBP-0x34]
	$+4B3    007E42EF     0FB6C0                   MOVZX EAX, AL
	$+4B6    007E42F2     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD], 0x0
	$+4BA    007E42F6     8D1485 B6020000          LEA EDX, DWORD PTR DS:[EAX*4+0x2B6]
	$+4C1    007E42FD     03D3                     ADD EDX, EBX
	$+4C3    007E42FF     807D EB 00               CMP BYTE PTR SS:[EBP-0x15], 0x0
	$+4C7    007E4303     76 4C                    JBE SHORT PathOfEx.007E4351
	$+4C9    007E4305     8D8B B6020000            LEA ECX, DWORD PTR DS:[EBX+0x2B6]
	$+4CF    007E430B     8A5D F3                  MOV BL, BYTE PTR SS:[EBP-0xD]
	$+4D2    007E430E     894D D4                  MOV DWORD PTR SS:[EBP-0x2C], ECX
	$+4D5    007E4311     8D75 94                  LEA ESI, DWORD PTR SS:[EBP-0x6C]
	$+4D8    007E4314     66:8B06                  MOV AX, WORD PTR DS:[ESI]
	$+4DB    007E4317     3BCA                     CMP ECX, EDX
	$+4DD    007E4319     74 17                    JE SHORT PathOfEx.007E4332
	$+4DF    007E431B     0F1F4400 00              NOP DWORD PTR DS:[EAX+EAX]
	$+4E4    007E4320     66:3901                  CMP WORD PTR DS:[ECX], AX
	$+4E7    007E4323     74 09                    JE SHORT PathOfEx.007E432E
	$+4E9    007E4325     83C1 04                  ADD ECX, 0x4
	$+4EC    007E4328     3BCA                     CMP ECX, EDX
	$+4EE    007E432A   ^ 75 F4                    JNZ SHORT PathOfEx.007E4320
	$+4F0    007E432C     EB 04                    JMP SHORT PathOfEx.007E4332
	$+4F2    007E432E     3BCA                     CMP ECX, EDX
	$+4F4    007E4330     75 0E                    JNZ SHORT PathOfEx.007E4340
	$+4F6    007E4332     8B06                     MOV EAX, DWORD PTR DS:[ESI]
	$+4F8    007E4334     0FB6CB                   MOVZX ECX, BL
	$+4FB    007E4337     FEC3                     INC BL
	$+4FD    007E4339     89848D 78FFFFFF          MOV DWORD PTR SS:[EBP+ECX*4-0x88], EAX
	$+504    007E4340     8B4D D4                  MOV ECX, DWORD PTR SS:[EBP-0x2C]
	$+507    007E4343     83C6 04                  ADD ESI, 0x4
	$+50A    007E4346     83EF 01                  SUB EDI, 0x1
	$+50D    007E4349   ^ 75 C9                    JNZ SHORT PathOfEx.007E4314
	$+50F    007E434B     885D F3                  MOV BYTE PTR SS:[EBP-0xD], BL
	$+512    007E434E     8B5D CC                  MOV EBX, DWORD PTR SS:[EBP-0x34]
	$+515    007E4351     8A45 0B                  MOV AL, BYTE PTR SS:[EBP+0xB]
	$+518    007E4354     84C0                     TEST AL, AL
	$+51A    007E4356     74 1B                    JE SHORT PathOfEx.007E4373
	$+51C    007E4358     8D75 B8                  LEA ESI, DWORD PTR SS:[EBP-0x48]
	$+51F    007E435B     0FB6F8                   MOVZX EDI, AL
	$+522    007E435E     66:90                    NOP
	$+524    007E4360     0FB706                   MOVZX EAX, WORD PTR DS:[ESI]
	$+527    007E4363     8BCB                     MOV ECX, EBX
	$+529    007E4365     50                       PUSH EAX
	$+52A    007E4366     E8 150F0000              CALL PathOfEx.007E5280
	$+52F    007E436B     8D76 02                  LEA ESI, DWORD PTR DS:[ESI+0x2]
	$+532    007E436E     83EF 01                  SUB EDI, 0x1
	$+535    007E4371   ^ 75 ED                    JNZ SHORT PathOfEx.007E4360
	$+537    007E4373     8A45 F3                  MOV AL, BYTE PTR SS:[EBP-0xD]
	$+53A    007E4376     84C0                     TEST AL, AL
	$+53C    007E4378     0F84 7F000000            JE PathOfEx.007E43FD
	$+542    007E437E     0FB6C0                   MOVZX EAX, AL
	$+545    007E4381     8DB5 7BFFFFFF            LEA ESI, DWORD PTR SS:[EBP-0x85]
	$+54B    007E4387     8945 08                  MOV DWORD PTR SS:[EBP+0x8], EAX
	$+54E    007E438A     66:0F1F4400 00           NOP WORD PTR DS:[EAX+EAX]
	$+554    007E4390     8A83 D2020000            MOV AL, BYTE PTR DS:[EBX+0x2D2]
	$+55A    007E4396     8A4E FF                  MOV CL, BYTE PTR DS:[ESI-0x1]
	$+55D    007E4399     8A16                     MOV DL, BYTE PTR DS:[ESI]
	$+55F    007E439B     0FB77E FD                MOVZX EDI, WORD PTR DS:[ESI-0x3]
	$+563    007E439F     884D D4                  MOV BYTE PTR SS:[EBP-0x2C], CL
	$+566    007E43A2     8855 C0                  MOV BYTE PTR SS:[EBP-0x40], DL
	$+569    007E43A5     3C 07                    CMP AL, 0x7
	$+56B    007E43A7     74 4B                    JE SHORT PathOfEx.007E43F4
	$+56D    007E43A9     FF75 D4                  PUSH DWORD PTR SS:[EBP-0x2C]
	$+570    007E43AC     0FB6C0                   MOVZX EAX, AL
	$+573    007E43AF     FF75 C0                  PUSH DWORD PTR SS:[EBP-0x40]
	$+576    007E43B2     57                       PUSH EDI
	$+577    007E43B3     66:89BC83 B6020000       MOV WORD PTR DS:[EBX+EAX*4+0x2B6], DI
	$+57F    007E43BB     0FB683 D2020000          MOVZX EAX, BYTE PTR DS:[EBX+0x2D2]
	$+586    007E43C2     889483 B9020000          MOV BYTE PTR DS:[EBX+EAX*4+0x2B9], DL
	$+58D    007E43C9     0FB683 D2020000          MOVZX EAX, BYTE PTR DS:[EBX+0x2D2]
	$+594    007E43D0     888C83 B8020000          MOV BYTE PTR DS:[EBX+EAX*4+0x2B8], CL
	$+59B    007E43D7     B8 00040000              MOV EAX, 0x400
	$+5A0    007E43DC     66:0983 B4020000         OR WORD PTR DS:[EBX+0x2B4], AX
	$+5A7    007E43E3     8BCB                     MOV ECX, EBX
	$+5A9    007E43E5     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+5AB    007E43E7     FE83 D2020000            INC BYTE PTR DS:[EBX+0x2D2]
	$+5B1    007E43ED     FF50 60                  CALL NEAR DWORD PTR DS:[EAX+0x60]
	$+5B4    007E43F0     C643 08 01               MOV BYTE PTR DS:[EBX+0x8], 0x1
	$+5B8    007E43F4     83C6 04                  ADD ESI, 0x4
	$+5BB    007E43F7     836D 08 01               SUB DWORD PTR SS:[EBP+0x8], 0x1
	$+5BF    007E43FB   ^ 75 93                    JNZ SHORT PathOfEx.007E4390
	$+5C1    007E43FD     8A45 E3                  MOV AL, BYTE PTR SS:[EBP-0x1D]
	$+5C4    007E4400     3843 48                  CMP BYTE PTR DS:[EBX+0x48], AL
	$+5C7    007E4403     74 07                    JE SHORT PathOfEx.007E440C
	$+5C9    007E4405     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+5CB    007E4407     8BCB                     MOV ECX, EBX
	$+5CD    007E4409     FF50 40                  CALL NEAR DWORD PTR DS:[EAX+0x40]
	$+5D0    007E440C     8A45 E2                  MOV AL, BYTE PTR SS:[EBP-0x1E]
	$+5D3    007E440F     3843 49                  CMP BYTE PTR DS:[EBX+0x49], AL
	$+5D6    007E4412     74 07                    JE SHORT PathOfEx.007E441B
	$+5D8    007E4414     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+5DA    007E4416     8BCB                     MOV ECX, EBX
	$+5DC    007E4418     FF50 44                  CALL NEAR DWORD PTR DS:[EAX+0x44]
	$+5DF    007E441B     8B7D C8                  MOV EDI, DWORD PTR SS:[EBP-0x38]
	$+5E2    007E441E     85FF                     TEST EDI, EDI
	$+5E4    007E4420     74 45                    JE SHORT PathOfEx.007E4467
	$+5E6    007E4422     A1 14DD0001              MOV EAX, DWORD PTR DS:[0x100DD14]
	$+5EB    007E4427     8B73 30                  MOV ESI, DWORD PTR DS:[EBX+0x30]
	$+5EE    007E442A     8B0D 10DD0001            MOV ECX, DWORD PTR DS:[0x100DD10]
	$+5F4    007E4430     03F7                     ADD ESI, EDI
	$+5F6    007E4432     2BC1                     SUB EAX, ECX
	$+5F8    007E4434     A9 FCFFFFFF              TEST EAX, -0x4
	$+5FD    007E4439     75 0B                    JNZ SHORT PathOfEx.007E4446
	$+5FF    007E443B     E8 A008F5FF              CALL PathOfEx.00734CE0
	$+604    007E4440     8B0D 10DD0001            MOV ECX, DWORD PTR DS:[0x100DD10]
	$+60A    007E4446     8B81 8C010000            MOV EAX, DWORD PTR DS:[ECX+0x18C]
	$+610    007E444C     3BF0                     CMP ESI, EAX
	$+612    007E444E     8BCB                     MOV ECX, EBX
	$+614    007E4450     0F42C6                   CMOVB EAX, ESI
	$+617    007E4453     50                       PUSH EAX
	$+618    007E4454     E8 27000000              CALL PathOfEx.007E4480
	$+61D    007E4459     01BB B0020000            ADD DWORD PTR DS:[EBX+0x2B0], EDI
	$+623    007E445F     66:838B B4020000 01      OR WORD PTR DS:[EBX+0x2B4], 0x1
	$+62B    007E4467     8B4D F4                  MOV ECX, DWORD PTR SS:[EBP-0xC]
	$+62E    007E446A     5F                       POP EDI
	$+62F    007E446B     5E                       POP ESI
	$+630    007E446C     5B                       POP EBX
	$+631    007E446D     64:890D 00000000         MOV DWORD PTR FS:[0], ECX
	$+638    007E4474     8BE5                     MOV ESP, EBP
	$+63A    007E4476     5D                       POP EBP
	$+63B    007E4477     C2 0400                  RETN 0x4
	$+63E    007E447A     CC                       INT3
	*/

	//gj3
	/*
	$-2DB    006E4BBB    /74 17                    JE SHORT PathOfEx.006E4BD4
	$-2D9    006E4BBD    |6A 01                    PUSH 0x1
	$-2D7    006E4BBF    |8BCE                     MOV ECX,ESI
	$-2D5    006E4BC1    |E8 3A70F5FF              CALL PathOfEx.0063BC00
	$-2D0    006E4BC6    |8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-2CD    006E4BC9    |8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-2CB    006E4BCB    |8A45 42                  MOV AL,BYTE PTR SS:[EBP+0x42]
	$-2C8    006E4BCE    |88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-2C5    006E4BD1    |FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-2C2    006E4BD4    \F685 E0010000 10         TEST BYTE PTR SS:[EBP+0x1E0],0x10
	$-2BB    006E4BDB     53                       PUSH EBX
	$-2BA    006E4BDC     0F84 85000000            JE PathOfEx.006E4C67
	$-2B4    006E4BE2     8DBD 38010000            LEA EDI,DWORD PTR SS:[EBP+0x138]
	$-2AE    006E4BE8     8D85 C8010000            LEA EAX,DWORD PTR SS:[EBP+0x1C8]
	$-2A8    006E4BEE     3BF8                     CMP EDI,EAX
	$-2A6    006E4BF0     74 75                    JE SHORT PathOfEx.006E4C67
	$-2A4    006E4BF2     BB C4FEFFFF              MOV EBX,-0x13C
	$-29F    006E4BF7     2BDD                     SUB EBX,EBP
	$-29D    006E4BF9     8BE8                     MOV EBP,EAX
	$-29B    006E4BFB     0F1F4400 00              NOP DWORD PTR DS:[EAX+EAX]
	$-296    006E4C00     8D43 04                  LEA EAX,DWORD PTR DS:[EBX+0x4]
	$-293    006E4C03     03C7                     ADD EAX,EDI
	$-291    006E4C05     C1F8 04                  SAR EAX,0x4
	$-28E    006E4C08     40                       INC EAX
	$-28D    006E4C09     83F8 09                  CMP EAX,0x9
	$-28A    006E4C0C     7D 4E                    JGE SHORT PathOfEx.006E4C5C
	$-288    006E4C0E     6A 04                    PUSH 0x4
	$-286    006E4C10     8BCE                     MOV ECX,ESI
	$-284    006E4C12     E8 E96FF5FF              CALL PathOfEx.0063BC00
	$-27F    006E4C17     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-27C    006E4C1A     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-27A    006E4C1C     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-278    006E4C1E     6A 04                    PUSH 0x4
	$-276    006E4C20     89040A                   MOV DWORD PTR DS:[EDX+ECX],EAX
	$-273    006E4C23     8BCE                     MOV ECX,ESI
	$-271    006E4C25     8346 08 04               ADD DWORD PTR DS:[ESI+0x8],0x4
	$-26D    006E4C29     E8 D26FF5FF              CALL PathOfEx.0063BC00
	$-268    006E4C2E     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-265    006E4C31     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-263    006E4C33     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-260    006E4C36     6A 08                    PUSH 0x8
	$-25E    006E4C38     89040A                   MOV DWORD PTR DS:[EDX+ECX],EAX
	$-25B    006E4C3B     8BCE                     MOV ECX,ESI
	$-259    006E4C3D     8346 08 04               ADD DWORD PTR DS:[ESI+0x8],0x4
	$-255    006E4C41     E8 BA6FF5FF              CALL PathOfEx.0063BC00
	$-250    006E4C46     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-24D    006E4C49     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-24B    006E4C4B     8B47 08                  MOV EAX,DWORD PTR DS:[EDI+0x8]
	$-248    006E4C4E     89040A                   MOV DWORD PTR DS:[EDX+ECX],EAX
	$-245    006E4C51     8B47 0C                  MOV EAX,DWORD PTR DS:[EDI+0xC]
	$-242    006E4C54     89440A 04                MOV DWORD PTR DS:[EDX+ECX+0x4],EAX
	$-23E    006E4C58     8346 08 08               ADD DWORD PTR DS:[ESI+0x8],0x8
	$-23A    006E4C5C     83C7 10                  ADD EDI,0x10
	$-237    006E4C5F     3BFD                     CMP EDI,EBP
	$-235    006E4C61   ^ 75 9D                    JNZ SHORT PathOfEx.006E4C00
	$-233    006E4C63     8B6C24 10                MOV EBP,DWORD PTR SS:[ESP+0x10]
	$-22F    006E4C67     F685 E0010000 20         TEST BYTE PTR SS:[EBP+0x1E0],0x20
	$-228    006E4C6E     0F84 95000000            JE PathOfEx.006E4D09
	$-222    006E4C74     8B85 CC010000            MOV EAX,DWORD PTR SS:[EBP+0x1CC]
	$-21C    006E4C7A     8BCE                     MOV ECX,ESI
	$-21A    006E4C7C     6A 02                    PUSH 0x2
	$-218    006E4C7E     85C0                     TEST EAX,EAX
	$-216    006E4C80     75 16                    JNZ SHORT PathOfEx.006E4C98
	$-214    006E4C82     E8 796FF5FF              CALL PathOfEx.0063BC00
	$-20F    006E4C87     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-20C    006E4C8A     33D2                     XOR EDX,EDX
	$-20A    006E4C8C     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-208    006E4C8E     66:891401                MOV WORD PTR DS:[ECX+EAX],DX
	$-204    006E4C92     8346 08 02               ADD DWORD PTR DS:[ESI+0x8],0x2
	$-200    006E4C96     EB 71                    JMP SHORT PathOfEx.006E4D09
	$-1FE    006E4C98     0FB778 24                MOVZX EDI,WORD PTR DS:[EAX+0x24]
	$-1FA    006E4C9C     E8 5F6FF5FF              CALL PathOfEx.0063BC00
	$-1F5    006E4CA1     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-1F2    006E4CA4     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-1F0    006E4CA6     6A 01                    PUSH 0x1
	$-1EE    006E4CA8     66:893C01                MOV WORD PTR DS:[ECX+EAX],DI
	$-1EA    006E4CAC     8BCE                     MOV ECX,ESI
	$-1E8    006E4CAE     8346 08 02               ADD DWORD PTR DS:[ESI+0x8],0x2
	$-1E4    006E4CB2     E8 496FF5FF              CALL PathOfEx.0063BC00
	$-1DF    006E4CB7     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-1DC    006E4CBA     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-1DA    006E4CBC     8A85 E2010000            MOV AL,BYTE PTR SS:[EBP+0x1E2]
	$-1D4    006E4CC2     6A 01                    PUSH 0x1
	$-1D2    006E4CC4     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-1CF    006E4CC7     8BCE                     MOV ECX,ESI
	$-1CD    006E4CC9     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-1CA    006E4CCC     8B9D D4010000            MOV EBX,DWORD PTR SS:[EBP+0x1D4]
	$-1C4    006E4CD2     2B9D D0010000            SUB EBX,DWORD PTR SS:[EBP+0x1D0]
	$-1BE    006E4CD8     E8 236FF5FF              CALL PathOfEx.0063BC00
	$-1B9    006E4CDD     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-1B6    006E4CE0     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-1B4    006E4CE2     53                       PUSH EBX
	$-1B3    006E4CE3     881C01                   MOV BYTE PTR DS:[ECX+EAX],BL
	$-1B0    006E4CE6     8BCE                     MOV ECX,ESI
	$-1AE    006E4CE8     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-1AB    006E4CEB     8BBD D0010000            MOV EDI,DWORD PTR SS:[EBP+0x1D0]
	$-1A5    006E4CF1     E8 0A6FF5FF              CALL PathOfEx.0063BC00
	$-1A0    006E4CF6     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-19D    006E4CF9     0306                     ADD EAX,DWORD PTR DS:[ESI]
	$-19B    006E4CFB     53                       PUSH EBX
	$-19A    006E4CFC     57                       PUSH EDI
	$-199    006E4CFD     50                       PUSH EAX
	$-198    006E4CFE     E8 4DE75F00              CALL PathOfEx.00CE3450
	$-193    006E4D03     83C4 0C                  ADD ESP,0xC
	$-190    006E4D06     015E 08                  ADD DWORD PTR DS:[ESI+0x8],EBX
	$-18D    006E4D09     F685 E0010000 40         TEST BYTE PTR SS:[EBP+0x1E0],0x40
	$-186    006E4D10     74 17                    JE SHORT PathOfEx.006E4D29
	$-184    006E4D12     8A5D 28                  MOV BL,BYTE PTR SS:[EBP+0x28]
	$-181    006E4D15     8BCE                     MOV ECX,ESI
	$-17F    006E4D17     6A 01                    PUSH 0x1
	$-17D    006E4D19     E8 E26EF5FF              CALL PathOfEx.0063BC00
	$-178    006E4D1E     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-175    006E4D21     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-173    006E4D23     881C01                   MOV BYTE PTR DS:[ECX+EAX],BL
	$-170    006E4D26     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-16D    006E4D29     F685 E0010000 80         TEST BYTE PTR SS:[EBP+0x1E0],0x80
	$-166    006E4D30     74 6A                    JE SHORT PathOfEx.006E4D9C
	$-164    006E4D32     6A 01                    PUSH 0x1
	$-162    006E4D34     8BCE                     MOV ECX,ESI
	$-160    006E4D36     E8 C56EF5FF              CALL PathOfEx.0063BC00
	$-15B    006E4D3B     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-158    006E4D3E     32DB                     XOR BL,BL
	$-156    006E4D40     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-154    006E4D42     8A85 E3010000            MOV AL,BYTE PTR SS:[EBP+0x1E3]
	$-14E    006E4D48     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-14B    006E4D4B     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-148    006E4D4E     389D E3010000            CMP BYTE PTR SS:[EBP+0x1E3],BL
	$-142    006E4D54     76 46                    JBE SHORT PathOfEx.006E4D9C
	$-140    006E4D56     6A 02                    PUSH 0x2
	$-13E    006E4D58     8BCE                     MOV ECX,ESI
	$-13C    006E4D5A     E8 A16EF5FF              CALL PathOfEx.0063BC00
	$-137    006E4D5F     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-134    006E4D62     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-132    006E4D64     0FB6FB                   MOVZX EDI,BL
	$-12F    006E4D67     6A 01                    PUSH 0x1
	$-12D    006E4D69     66:8B84BD E4010000       MOV AX,WORD PTR SS:[EBP+EDI*4+0x1E4]
	$-125    006E4D71     66:89040A                MOV WORD PTR DS:[EDX+ECX],AX
	$-121    006E4D75     8BCE                     MOV ECX,ESI
	$-11F    006E4D77     8346 08 02               ADD DWORD PTR DS:[ESI+0x8],0x2
	$-11B    006E4D7B     E8 806EF5FF              CALL PathOfEx.0063BC00
	$-116    006E4D80     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-113    006E4D83     FEC3                     INC BL
	$-111    006E4D85     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-10F    006E4D87     8A84BD E6010000          MOV AL,BYTE PTR SS:[EBP+EDI*4+0x1E6]
	$-108    006E4D8E     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-105    006E4D91     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-102    006E4D94     3A9D E3010000            CMP BL,BYTE PTR SS:[EBP+0x1E3]
	$-FC     006E4D9A   ^ 72 BA                    JB SHORT PathOfEx.006E4D56
	$-FA     006E4D9C     B8 00020000              MOV EAX,0x200
	$-F5     006E4DA1     5B                       POP EBX
	$-F4     006E4DA2     66:8585 E0010000         TEST WORD PTR SS:[EBP+0x1E0],AX
	$-ED     006E4DA9     74 30                    JE SHORT PathOfEx.006E4DDB
	$-EB     006E4DAB     6A 01                    PUSH 0x1
	$-E9     006E4DAD     8BCE                     MOV ECX,ESI
	$-E7     006E4DAF     E8 4C6EF5FF              CALL PathOfEx.0063BC00
	$-E2     006E4DB4     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-DF     006E4DB7     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-DD     006E4DB9     0FB645 44                MOVZX EAX,BYTE PTR SS:[EBP+0x44]
	$-D9     006E4DBD     6A 01                    PUSH 0x1
	$-D7     006E4DBF     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-D4     006E4DC2     8BCE                     MOV ECX,ESI
	$-D2     006E4DC4     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-CF     006E4DC7     E8 346EF5FF              CALL PathOfEx.0063BC00
	$-CA     006E4DCC     8B56 08                  MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-C7     006E4DCF     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$-C5     006E4DD1     0FB645 43                MOVZX EAX,BYTE PTR SS:[EBP+0x43]
	$-C1     006E4DD5     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-BE     006E4DD8     FF46 08                  INC DWORD PTR DS:[ESI+0x8]
	$-BB     006E4DDB     5F                       POP EDI
	$-BA     006E4DDC     5E                       POP ESI
	$-B9     006E4DDD     5D                       POP EBP
	$-B8     006E4DDE     59                       POP ECX
	$-B7     006E4DDF     C2 0400                  RETN 0x4
	$-B4     006E4DE2     CC                       INT3
	$-B3     006E4DE3     CC                       INT3
	$-B2     006E4DE4     CC                       INT3
	$-B1     006E4DE5     CC                       INT3
	$-B0     006E4DE6     CC                       INT3
	$-AF     006E4DE7     CC                       INT3
	$-AE     006E4DE8     CC                       INT3
	$-AD     006E4DE9     CC                       INT3
	$-AC     006E4DEA     CC                       INT3
	$-AB     006E4DEB     CC                       INT3
	$-AA     006E4DEC     CC                       INT3
	$-A9     006E4DED     CC                       INT3
	$-A8     006E4DEE     CC                       INT3
	$-A7     006E4DEF     CC                       INT3
	$-A6     006E4DF0     55                       PUSH EBP
	$-A5     006E4DF1     8BEC                     MOV EBP,ESP
	$-A3     006E4DF3     6A FF                    PUSH -0x1
	$-A1     006E4DF5     68 EA8CD900              PUSH PathOfEx.00D98CEA
	$-9C     006E4DFA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-96     006E4E00     50                       PUSH EAX
	$-95     006E4E01     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-8E     006E4E08     83EC 78                  SUB ESP,0x78
	$-8B     006E4E0B     53                       PUSH EBX
	$-8A     006E4E0C     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-87     006E4E0F     56                       PUSH ESI
	$-86     006E4E10     57                       PUSH EDI
	$-85     006E4E11     8BF9                     MOV EDI,ECX
	$-83     006E4E13     8BCB                     MOV ECX,EBX
	$-81     006E4E15     6A 02                    PUSH 0x2
	$-7F     006E4E17     897D D0                  MOV DWORD PTR SS:[EBP-0x30],EDI
	$-7C     006E4E1A     E8 B186C7FF              CALL PathOfEx.0035D4D0
	$-77     006E4E1F     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$-74     006E4E22     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-72     006E4E24     0FB71401                 MOVZX EDX,WORD PTR DS:[ECX+EAX]
	$-6E     006E4E28     8D41 02                  LEA EAX,DWORD PTR DS:[ECX+0x2]
	$-6B     006E4E2B     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$-68     006E4E2E     32C0                     XOR AL,AL
	$-66     006E4E30     8955 E8                  MOV DWORD PTR SS:[EBP-0x18],EDX
	$-63     006E4E33     F6C2 02                  TEST DL,0x2
	$-60     006E4E36     74 2F                    JE SHORT PathOfEx.006E4E67
	$-5E     006E4E38     68 C0000000              PUSH 0xC0
	$-59     006E4E3D     8BCB                     MOV ECX,EBX
	$-57     006E4E3F     E8 8C86C7FF              CALL PathOfEx.0035D4D0
	$-52     006E4E44     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-50     006E4E46     0343 08                  ADD EAX,DWORD PTR DS:[EBX+0x8]
	$-4D     006E4E49     68 C0000000              PUSH 0xC0
	$-48     006E4E4E     50                       PUSH EAX
	$-47     006E4E4F     8D47 4C                  LEA EAX,DWORD PTR DS:[EDI+0x4C]
	$-44     006E4E52     50                       PUSH EAX
	$-43     006E4E53     E8 F8E55F00              CALL PathOfEx.00CE3450
	$-3E     006E4E58     8B55 E8                  MOV EDX,DWORD PTR SS:[EBP-0x18]
	$-3B     006E4E5B     83C4 0C                  ADD ESP,0xC
	$-38     006E4E5E     8143 08 C0000000         ADD DWORD PTR DS:[EBX+0x8],0xC0
	$-31     006E4E65     B0 01                    MOV AL,0x1
	$-2F     006E4E67     F6C2 08                  TEST DL,0x8
	$-2C     006E4E6A     74 26                    JE SHORT PathOfEx.006E4E92
	$-2A     006E4E6C     6A 2C                    PUSH 0x2C
	$-28     006E4E6E     8BCB                     MOV ECX,EBX
	$-26     006E4E70     E8 5B86C7FF              CALL PathOfEx.0035D4D0
	$-21     006E4E75     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-1F     006E4E77     0343 08                  ADD EAX,DWORD PTR DS:[EBX+0x8]
	$-1C     006E4E7A     6A 2C                    PUSH 0x2C
	$-1A     006E4E7C     50                       PUSH EAX
	$-19     006E4E7D     8D87 0C010000            LEA EAX,DWORD PTR DS:[EDI+0x10C]
	$-13     006E4E83     50                       PUSH EAX
	$-12     006E4E84     E8 C7E55F00              CALL PathOfEx.00CE3450
	$-D      006E4E89     83C4 0C                  ADD ESP,0xC
	$-A      006E4E8C     8343 08 2C               ADD DWORD PTR DS:[EBX+0x8],0x2C
	$-6      006E4E90     EB 04                    JMP SHORT PathOfEx.006E4E96
	$-4      006E4E92     84C0                     TEST AL,AL
	$-2      006E4E94     74 20                    JE SHORT PathOfEx.006E4EB6
	$ ==>    006E4E96     8BB7 00020000            MOV ESI,DWORD PTR DS:[EDI+0x200]                       ; g_o_cur_quest_info_
	$+6      006E4E9C     3BB7 04020000            CMP ESI,DWORD PTR DS:[EDI+0x204]
	$+C      006E4EA2     74 12                    JE SHORT PathOfEx.006E4EB6
	$+E      006E4EA4     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+10     006E4EA6     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+12     006E4EA8     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]                            ; g_c_player_quest_info_vfn_
	$+15     006E4EAB     83C6 04                  ADD ESI,0x4
	$+18     006E4EAE     3BB7 04020000            CMP ESI,DWORD PTR DS:[EDI+0x204]
	$+1E     006E4EB4   ^ 75 EE                    JNZ SHORT PathOfEx.006E4EA4
	$+20     006E4EB6     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+23     006E4EB9     C745 C8 00000000         MOV DWORD PTR SS:[EBP-0x38],0x0
	$+2A     006E4EC0     F6C1 01                  TEST CL,0x1
	$+2D     006E4EC3     74 1D                    JE SHORT PathOfEx.006E4EE2
	$+2F     006E4EC5     6A 04                    PUSH 0x4
	$+31     006E4EC7     8BCB                     MOV ECX,EBX
	$+33     006E4EC9     E8 0286C7FF              CALL PathOfEx.0035D4D0
	$+38     006E4ECE     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+3B     006E4ED1     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+3D     006E4ED3     8B0401                   MOV EAX,DWORD PTR DS:[ECX+EAX]
	$+40     006E4ED6     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
	$+43     006E4ED9     8D41 04                  LEA EAX,DWORD PTR DS:[ECX+0x4]
	$+46     006E4EDC     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+49     006E4EDF     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+4C     006E4EE2     8A47 41                  MOV AL,BYTE PTR DS:[EDI+0x41]
	$+4F     006E4EE5     8845 E7                  MOV BYTE PTR SS:[EBP-0x19],AL
	$+52     006E4EE8     F6C1 04                  TEST CL,0x4
	$+55     006E4EEB     74 1A                    JE SHORT PathOfEx.006E4F07
	$+57     006E4EED     6A 01                    PUSH 0x1
	$+59     006E4EEF     8BCB                     MOV ECX,EBX
	$+5B     006E4EF1     E8 DA85C7FF              CALL PathOfEx.0035D4D0
	$+60     006E4EF6     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+62     006E4EF8     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+65     006E4EFB     8A0401                   MOV AL,BYTE PTR DS:[ECX+EAX]
	$+68     006E4EFE     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+6B     006E4F01     8847 41                  MOV BYTE PTR DS:[EDI+0x41],AL
	$+6E     006E4F04     FF43 08                  INC DWORD PTR DS:[EBX+0x8]
	$+71     006E4F07     8A47 42                  MOV AL,BYTE PTR DS:[EDI+0x42]
	$+74     006E4F0A     8845 E6                  MOV BYTE PTR SS:[EBP-0x1A],AL
	$+77     006E4F0D     F7C1 00010000            TEST ECX,0x100
	$+7D     006E4F13     74 1A                    JE SHORT PathOfEx.006E4F2F
	$+7F     006E4F15     6A 01                    PUSH 0x1
	$+81     006E4F17     8BCB                     MOV ECX,EBX
	$+83     006E4F19     E8 B285C7FF              CALL PathOfEx.0035D4D0
	$+88     006E4F1E     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+8A     006E4F20     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+8D     006E4F23     8A0401                   MOV AL,BYTE PTR DS:[ECX+EAX]
	$+90     006E4F26     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+93     006E4F29     8847 42                  MOV BYTE PTR DS:[EDI+0x42],AL
	$+96     006E4F2C     FF43 08                  INC DWORD PTR DS:[EBX+0x8]
	$+99     006E4F2F     F6C1 10                  TEST CL,0x10
	$+9C     006E4F32     0F84 41010000            JE PathOfEx.006E5079
	$+A2     006E4F38     8DB7 38010000            LEA ESI,DWORD PTR DS:[EDI+0x138]
	$+A8     006E4F3E     8D87 C8010000            LEA EAX,DWORD PTR DS:[EDI+0x1C8]
	$+AE     006E4F44     3BF0                     CMP ESI,EAX
	$+B0     006E4F46     0F84 2D010000            JE PathOfEx.006E5079
	$+B6     006E4F4C     83C6 04                  ADD ESI,0x4
	$+B9     006E4F4F     8D8F C8010000            LEA ECX,DWORD PTR DS:[EDI+0x1C8]
	$+BF     006E4F55     B8 C4FEFFFF              MOV EAX,-0x13C
	$+C4     006E4F5A     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$+C7     006E4F5D     2BC7                     SUB EAX,EDI
	$+C9     006E4F5F     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+CC     006E4F62     03C6                     ADD EAX,ESI
	$+CE     006E4F64     C1F8 04                  SAR EAX,0x4
	$+D1     006E4F67     40                       INC EAX
	$+D2     006E4F68     8945 D4                  MOV DWORD PTR SS:[EBP-0x2C],EAX
	$+D5     006E4F6B     83F8 09                  CMP EAX,0x9
	$+D8     006E4F6E     0F8D EB000000            JGE PathOfEx.006E505F
	$+DE     006E4F74     8B4E FC                  MOV ECX,DWORD PTR DS:[ESI-0x4]
	$+E1     006E4F77     8B1E                     MOV EBX,DWORD PTR DS:[ESI]
	$+E3     006E4F79     894D C4                  MOV DWORD PTR SS:[EBP-0x3C],ECX
	$+E6     006E4F7C     8BCF                     MOV ECX,EDI
	$+E8     006E4F7E     50                       PUSH EAX
	$+E9     006E4F7F     E8 FC71DAFF              CALL PathOfEx.0048C180
	$+EE     006E4F84     8945 CC                  MOV DWORD PTR SS:[EBP-0x34],EAX
	$+F1     006E4F87     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+F4     006E4F8A     8945 BC                  MOV DWORD PTR SS:[EBP-0x44],EAX
	$+F7     006E4F8D     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+FA     006E4F90     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+FD     006E4F93     8BCE                     MOV ECX,ESI
	$+FF     006E4F95     6A 04                    PUSH 0x4
	$+101    006E4F97     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+104    006E4F9A     E8 3185C7FF              CALL PathOfEx.0035D4D0
	$+109    006E4F9F     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+10B    006E4FA1     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+10E    006E4FA4     6A 04                    PUSH 0x4
	$+110    006E4FA6     8B0401                   MOV EAX,DWORD PTR DS:[ECX+EAX]
	$+113    006E4FA9     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+116    006E4FAC     8941 FC                  MOV DWORD PTR DS:[ECX-0x4],EAX
	$+119    006E4FAF     8BCE                     MOV ECX,ESI
	$+11B    006E4FB1     8346 08 04               ADD DWORD PTR DS:[ESI+0x8],0x4
	$+11F    006E4FB5     E8 1685C7FF              CALL PathOfEx.0035D4D0
	$+124    006E4FBA     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+126    006E4FBC     8B46 08                  MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+129    006E4FBF     6A 08                    PUSH 0x8
	$+12B    006E4FC1     8B0401                   MOV EAX,DWORD PTR DS:[ECX+EAX]
	$+12E    006E4FC4     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+131    006E4FC7     8901                     MOV DWORD PTR DS:[ECX],EAX
	$+133    006E4FC9     8BCE                     MOV ECX,ESI
	$+135    006E4FCB     8346 08 04               ADD DWORD PTR DS:[ESI+0x8],0x4
	$+139    006E4FCF     E8 FC84C7FF              CALL PathOfEx.0035D4D0
	$+13E    006E4FD4     8B4E 08                  MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+141    006E4FD7     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+143    006E4FD9     8B7408 04                MOV ESI,DWORD PTR DS:[EAX+ECX+0x4]
	$+147    006E4FDD     8B1408                   MOV EDX,DWORD PTR DS:[EAX+ECX]
	$+14A    006E4FE0     8D41 08                  LEA EAX,DWORD PTR DS:[ECX+0x8]
	$+14D    006E4FE3     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+150    006E4FE6     8941 08                  MOV DWORD PTR DS:[ECX+0x8],EAX
	$+153    006E4FE9     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+156    006E4FEC     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+158    006E4FEE     8970 08                  MOV DWORD PTR DS:[EAX+0x8],ESI
	$+15B    006E4FF1     8B75 D4                  MOV ESI,DWORD PTR SS:[EBP-0x2C]
	$+15E    006E4FF4     8950 04                  MOV DWORD PTR DS:[EAX+0x4],EDX
	$+161    006E4FF7     3BCB                     CMP ECX,EBX
	$+163    006E4FF9     74 09                    JE SHORT PathOfEx.006E5004
	$+165    006E4FFB     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+167    006E4FFD     51                       PUSH ECX
	$+168    006E4FFE     56                       PUSH ESI
	$+169    006E4FFF     8BCF                     MOV ECX,EDI
	$+16B    006E5001     FF50 48                  CALL DWORD PTR DS:[EAX+0x48]
	$+16E    006E5004     56                       PUSH ESI
	$+16F    006E5005     8BCF                     MOV ECX,EDI
	$+171    006E5007     E8 7471DAFF              CALL PathOfEx.0048C180
	$+176    006E500C     8945 D4                  MOV DWORD PTR SS:[EBP-0x2C],EAX
	$+179    006E500F     3B45 CC                  CMP EAX,DWORD PTR SS:[EBP-0x34]
	$+17C    006E5012     74 0C                    JE SHORT PathOfEx.006E5020
	$+17E    006E5014     8B17                     MOV EDX,DWORD PTR DS:[EDI]
	$+180    006E5016     8BCF                     MOV ECX,EDI
	$+182    006E5018     50                       PUSH EAX
	$+183    006E5019     56                       PUSH ESI
	$+184    006E501A     FF52 4C                  CALL DWORD PTR DS:[EDX+0x4C]
	$+187    006E501D     8B45 D4                  MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+18A    006E5020     8B5D EC                  MOV EBX,DWORD PTR SS:[EBP-0x14]
	$+18D    006E5023     8B4B FC                  MOV ECX,DWORD PTR DS:[EBX-0x4]
	$+190    006E5026     3B4D C4                  CMP ECX,DWORD PTR SS:[EBP-0x3C]
	$+193    006E5029     74 13                    JE SHORT PathOfEx.006E503E
	$+195    006E502B     3B45 CC                  CMP EAX,DWORD PTR SS:[EBP-0x34]
	$+198    006E502E     8B17                     MOV EDX,DWORD PTR DS:[EDI]
	$+19A    006E5030     0F95C0                   SETNE AL
	$+19D    006E5033     0FB6C0                   MOVZX EAX,AL
	$+1A0    006E5036     50                       PUSH EAX
	$+1A1    006E5037     51                       PUSH ECX
	$+1A2    006E5038     56                       PUSH ESI
	$+1A3    006E5039     8BCF                     MOV ECX,EDI
	$+1A5    006E503B     FF52 50                  CALL DWORD PTR DS:[EDX+0x50]
	$+1A8    006E503E     8B45 BC                  MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+1AB    006E5041     3943 04                  CMP DWORD PTR DS:[EBX+0x4],EAX
	$+1AE    006E5044     75 08                    JNZ SHORT PathOfEx.006E504E
	$+1B0    006E5046     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+1B3    006E5049     3943 08                  CMP DWORD PTR DS:[EBX+0x8],EAX
	$+1B6    006E504C     74 08                    JE SHORT PathOfEx.006E5056
	$+1B8    006E504E     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+1BA    006E5050     8BCF                     MOV ECX,EDI
	$+1BC    006E5052     56                       PUSH ESI
	$+1BD    006E5053     FF50 54                  CALL DWORD PTR DS:[EAX+0x54]
	$+1C0    006E5056     8B75 EC                  MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+1C3    006E5059     8D8F C8010000            LEA ECX,DWORD PTR DS:[EDI+0x1C8]
	$+1C9    006E505F     83C6 10                  ADD ESI,0x10
	$+1CC    006E5062     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$+1CF    006E5065     8D46 FC                  LEA EAX,DWORD PTR DS:[ESI-0x4]
	$+1D2    006E5068     3BC1                     CMP EAX,ECX
	$+1D4    006E506A     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+1D7    006E506D   ^ 0F85 EFFEFFFF            JNZ PathOfEx.006E4F62
	$+1DD    006E5073     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+1E0    006E5076     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+1E3    006E5079     F6C1 20                  TEST CL,0x20
	$+1E6    006E507C     0F84 58010000            JE PathOfEx.006E51DA
	$+1EC    006E5082     6A 02                    PUSH 0x2
	$+1EE    006E5084     8BCB                     MOV ECX,EBX
	$+1F0    006E5086     E8 4584C7FF              CALL PathOfEx.0035D4D0
	$+1F5    006E508B     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+1F8    006E508E     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+1FA    006E5090     0FB71401                 MOVZX EDX,WORD PTR DS:[ECX+EAX]
	$+1FE    006E5094     8D41 02                  LEA EAX,DWORD PTR DS:[ECX+0x2]
	$+201    006E5097     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+204    006E509A     66:85D2                  TEST DX,DX
	$+207    006E509D     0F84 1B020000            JE PathOfEx.006E52BE
	$+20D    006E50A3     83EC 10                  SUB ESP,0x10
	$+210    006E50A6     8BC2                     MOV EAX,EDX
	$+212    006E50A8     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+215    006E50AB     E8 A092F2FF              CALL PathOfEx.0060E350
	$+21A    006E50B0     8BC8                     MOV ECX,EAX
	$+21C    006E50B2     E8 D9160000              CALL PathOfEx.006E6790
	$+221    006E50B7     8BF0                     MOV ESI,EAX
	$+223    006E50B9     8975 E0                  MOV DWORD PTR SS:[EBP-0x20],ESI
	$+226    006E50BC     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+229    006E50BF     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+230    006E50C6     50                       PUSH EAX
	$+231    006E50C7     8D55 E0                  LEA EDX,DWORD PTR SS:[EBP-0x20]
	$+234    006E50CA     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+237    006E50CD     E8 EEDFF7FF              CALL PathOfEx.006630C0
	$+23C    006E50D2     8BD0                     MOV EDX,EAX
	$+23E    006E50D4     8D8F C8010000            LEA ECX,DWORD PTR DS:[EDI+0x1C8]
	$+244    006E50DA     83C4 04                  ADD ESP,0x4
	$+247    006E50DD     8955 DC                  MOV DWORD PTR SS:[EBP-0x24],EDX
	$+24A    006E50E0     3BCA                     CMP ECX,EDX
	$+24C    006E50E2     74 2C                    JE SHORT PathOfEx.006E5110
	$+24E    006E50E4     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+250    006E50E6     85C0                     TEST EAX,EAX
	$+252    006E50E8     74 1C                    JE SHORT PathOfEx.006E5106
	$+254    006E50EA     50                       PUSH EAX
	$+255    006E50EB     E8 6092F2FF              CALL PathOfEx.0060E350
	$+25A    006E50F0     8BC8                     MOV ECX,EAX
	$+25C    006E50F2     E8 298EF2FF              CALL PathOfEx.0060DF20
	$+261    006E50F7     8B55 DC                  MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+264    006E50FA     8D8F C8010000            LEA ECX,DWORD PTR DS:[EDI+0x1C8]
	$+26A    006E5100     C701 00000000            MOV DWORD PTR DS:[ECX],0x0
	$+270    006E5106     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$+272    006E5108     8901                     MOV DWORD PTR DS:[ECX],EAX
	$+274    006E510A     C702 00000000            MOV DWORD PTR DS:[EDX],0x0
	$+27A    006E5110     8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+27D    006E5113     8941 04                  MOV DWORD PTR DS:[ECX+0x4],EAX
	$+280    006E5116     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+284    006E511A     8B45 B8                  MOV EAX,DWORD PTR SS:[EBP-0x48]
	$+287    006E511D     85C0                     TEST EAX,EAX
	$+289    006E511F     74 14                    JE SHORT PathOfEx.006E5135
	$+28B    006E5121     50                       PUSH EAX
	$+28C    006E5122     E8 2992F2FF              CALL PathOfEx.0060E350
	$+291    006E5127     8BC8                     MOV ECX,EAX
	$+293    006E5129     E8 F28DF2FF              CALL PathOfEx.0060DF20
	$+298    006E512E     C745 B8 00000000         MOV DWORD PTR SS:[EBP-0x48],0x0
	$+29F    006E5135     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$+2A6    006E513C     85F6                     TEST ESI,ESI
	$+2A8    006E513E     74 0D                    JE SHORT PathOfEx.006E514D
	$+2AA    006E5140     56                       PUSH ESI
	$+2AB    006E5141     E8 0A92F2FF              CALL PathOfEx.0060E350
	$+2B0    006E5146     8BC8                     MOV ECX,EAX
	$+2B2    006E5148     E8 D38DF2FF              CALL PathOfEx.0060DF20
	$+2B7    006E514D     6A 01                    PUSH 0x1
	$+2B9    006E514F     8BCB                     MOV ECX,EBX
	$+2BB    006E5151     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2C2    006E5158     E8 7383C7FF              CALL PathOfEx.0035D4D0
	$+2C7    006E515D     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+2C9    006E515F     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+2CC    006E5162     6A 01                    PUSH 0x1
	$+2CE    006E5164     8A0401                   MOV AL,BYTE PTR DS:[ECX+EAX]
	$+2D1    006E5167     8BCB                     MOV ECX,EBX
	$+2D3    006E5169     8887 E2010000            MOV BYTE PTR DS:[EDI+0x1E2],AL
	$+2D9    006E516F     FF43 08                  INC DWORD PTR DS:[EBX+0x8]
	$+2DC    006E5172     E8 5983C7FF              CALL PathOfEx.0035D4D0
	$+2E1    006E5177     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+2E4    006E517A     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+2E6    006E517C     8A1C01                   MOV BL,BYTE PTR DS:[ECX+EAX]
	$+2E9    006E517F     8D41 01                  LEA EAX,DWORD PTR DS:[ECX+0x1]
	$+2EC    006E5182     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+2EF    006E5185     8941 08                  MOV DWORD PTR DS:[ECX+0x8],EAX
	$+2F2    006E5188     8D8F D0010000            LEA ECX,DWORD PTR DS:[EDI+0x1D0]
	$+2F8    006E518E     0FB6C3                   MOVZX EAX,BL
	$+2FB    006E5191     50                       PUSH EAX
	$+2FC    006E5192     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$+2FF    006E5195     E8 D630F0FF              CALL PathOfEx.005E8270
	$+304    006E519A     84DB                     TEST BL,BL
	$+306    006E519C     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+309    006E519F     74 28                    JE SHORT PathOfEx.006E51C9
	$+30B    006E51A1     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+30E    006E51A4     8BB7 D0010000            MOV ESI,DWORD PTR DS:[EDI+0x1D0]
	$+314    006E51AA     8BCB                     MOV ECX,EBX
	$+316    006E51AC     E8 1F83C7FF              CALL PathOfEx.0035D4D0
	$+31B    006E51B1     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+31D    006E51B3     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
	$+320    006E51B6     0343 08                  ADD EAX,DWORD PTR DS:[EBX+0x8]
	$+323    006E51B9     50                       PUSH EAX
	$+324    006E51BA     56                       PUSH ESI
	$+325    006E51BB     E8 90E25F00              CALL PathOfEx.00CE3450
	$+32A    006E51C0     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+32D    006E51C3     83C4 0C                  ADD ESP,0xC
	$+330    006E51C6     0143 08                  ADD DWORD PTR DS:[EBX+0x8],EAX
	$+333    006E51C9     8DB7 C8010000            LEA ESI,DWORD PTR DS:[EDI+0x1C8]
	$+339    006E51CF     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+33B    006E51D1     8BCF                     MOV ECX,EDI
	$+33D    006E51D3     56                       PUSH ESI
	$+33E    006E51D4     FF50 5C                  CALL DWORD PTR DS:[EAX+0x5C]
	$+341    006E51D7     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+344    006E51DA     F6C1 40                  TEST CL,0x40
	$+347    006E51DD     74 24                    JE SHORT PathOfEx.006E5203
	$+349    006E51DF     6A 01                    PUSH 0x1
	$+34B    006E51E1     8BCB                     MOV ECX,EBX
	$+34D    006E51E3     E8 E882C7FF              CALL PathOfEx.0035D4D0
	$+352    006E51E8     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+355    006E51EB     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+357    006E51ED     8A1401                   MOV DL,BYTE PTR DS:[ECX+EAX]
	$+35A    006E51F0     8D41 01                  LEA EAX,DWORD PTR DS:[ECX+0x1]
	$+35D    006E51F3     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+360    006E51F6     8BCF                     MOV ECX,EDI
	$+362    006E51F8     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+364    006E51FA     8857 28                  MOV BYTE PTR DS:[EDI+0x28],DL
	$+367    006E51FD     FF50 58                  CALL DWORD PTR DS:[EAX+0x58]
	$+36A    006E5200     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+36D    006E5203     84C9                     TEST CL,CL
	$+36F    006E5205     0F89 10020000            JNS PathOfEx.006E541B
	$+375    006E520B     6A 01                    PUSH 0x1
	$+377    006E520D     8BCB                     MOV ECX,EBX
	$+379    006E520F     E8 BC82C7FF              CALL PathOfEx.0035D4D0
	$+37E    006E5214     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+381    006E5217     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+383    006E5219     8A1401                   MOV DL,BYTE PTR DS:[ECX+EAX]
	$+386    006E521C     8D41 01                  LEA EAX,DWORD PTR DS:[ECX+0x1]
	$+389    006E521F     8855 F1                  MOV BYTE PTR SS:[EBP-0xF],DL
	$+38C    006E5222     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+38F    006E5225     84D2                     TEST DL,DL
	$+391    006E5227     74 4B                    JE SHORT PathOfEx.006E5274
	$+393    006E5229     8D75 9A                  LEA ESI,DWORD PTR SS:[EBP-0x66]
	$+396    006E522C     0FB6FA                   MOVZX EDI,DL
	$+399    006E522F     90                       NOP
	$+39A    006E5230     6A 02                    PUSH 0x2
	$+39C    006E5232     8BCB                     MOV ECX,EBX
	$+39E    006E5234     E8 9782C7FF              CALL PathOfEx.0035D4D0
	$+3A3    006E5239     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+3A6    006E523C     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+3A8    006E523E     6A 01                    PUSH 0x1
	$+3AA    006E5240     66:8B0401                MOV AX,WORD PTR DS:[ECX+EAX]
	$+3AE    006E5244     66:8946 FE               MOV WORD PTR DS:[ESI-0x2],AX
	$+3B2    006E5248     8D41 02                  LEA EAX,DWORD PTR DS:[ECX+0x2]
	$+3B5    006E524B     8BCB                     MOV ECX,EBX
	$+3B7    006E524D     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+3BA    006E5250     E8 7B82C7FF              CALL PathOfEx.0035D4D0
	$+3BF    006E5255     8B4B 08                  MOV ECX,DWORD PTR DS:[EBX+0x8]
	$+3C2    006E5258     8D76 04                  LEA ESI,DWORD PTR DS:[ESI+0x4]
	$+3C5    006E525B     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+3C7    006E525D     8A0401                   MOV AL,BYTE PTR DS:[ECX+EAX]
	$+3CA    006E5260     8846 FC                  MOV BYTE PTR DS:[ESI-0x4],AL
	$+3CD    006E5263     8D41 01                  LEA EAX,DWORD PTR DS:[ECX+0x1]
	$+3D0    006E5266     8943 08                  MOV DWORD PTR DS:[EBX+0x8],EAX
	$+3D3    006E5269     83EF 01                  SUB EDI,0x1
	$+3D6    006E526C   ^ 75 C2                    JNZ SHORT PathOfEx.006E5230
	$+3D8    006E526E     8B7D D0                  MOV EDI,DWORD PTR SS:[EBP-0x30]
	$+3DB    006E5271     8A55 F1                  MOV DL,BYTE PTR SS:[EBP-0xF]
	$+3DE    006E5274     0FB6C2                   MOVZX EAX,DL
	$+3E1    006E5277     8D4D 98                  LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+3E4    006E527A     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+3E7    006E527D     C645 F2 00               MOV BYTE PTR SS:[EBP-0xE],0x0
	$+3EB    006E5281     8D0C81                   LEA ECX,DWORD PTR DS:[ECX+EAX*4]
	$+3EE    006E5284     8A87 E3010000            MOV AL,BYTE PTR DS:[EDI+0x1E3]
	$+3F4    006E528A     8845 F3                  MOV BYTE PTR SS:[EBP-0xD],AL
	$+3F7    006E528D     84C0                     TEST AL,AL
	$+3F9    006E528F     0F84 8A000000            JE PathOfEx.006E531F
	$+3FF    006E5295     8A5D F2                  MOV BL,BYTE PTR SS:[EBP-0xE]
	$+402    006E5298     8DB7 E4010000            LEA ESI,DWORD PTR DS:[EDI+0x1E4]
	$+408    006E529E     0FB6F8                   MOVZX EDI,AL
	$+40B    006E52A1     8D45 98                  LEA EAX,DWORD PTR SS:[EBP-0x68]
	$+40E    006E52A4     8BD0                     MOV EDX,EAX
	$+410    006E52A6     3BD1                     CMP EDX,ECX
	$+412    006E52A8     0FB716                   MOVZX EDX,WORD PTR DS:[ESI]
	$+415    006E52AB     74 54                    JE SHORT PathOfEx.006E5301
	$+417    006E52AD     0F1F00                   NOP DWORD PTR DS:[EAX]
	$+41A    006E52B0     66:3910                  CMP WORD PTR DS:[EAX],DX
	$+41D    006E52B3     74 48                    JE SHORT PathOfEx.006E52FD
	$+41F    006E52B5     83C0 04                  ADD EAX,0x4
	$+422    006E52B8     3BC1                     CMP EAX,ECX
	$+424    006E52BA   ^ 75 F4                    JNZ SHORT PathOfEx.006E52B0
	$+426    006E52BC     EB 43                    JMP SHORT PathOfEx.006E5301
	$+428    006E52BE     8DB7 C8010000            LEA ESI,DWORD PTR DS:[EDI+0x1C8]
	$+42E    006E52C4     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+431    006E52C7     3BF0                     CMP ESI,EAX
	$+433    006E52C9     74 1F                    JE SHORT PathOfEx.006E52EA
	$+435    006E52CB     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+437    006E52CD     85C0                     TEST EAX,EAX
	$+439    006E52CF     74 13                    JE SHORT PathOfEx.006E52E4
	$+43B    006E52D1     50                       PUSH EAX
	$+43C    006E52D2     E8 7990F2FF              CALL PathOfEx.0060E350
	$+441    006E52D7     8BC8                     MOV ECX,EAX
	$+443    006E52D9     E8 428CF2FF              CALL PathOfEx.0060DF20
	$+448    006E52DE     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$+44E    006E52E4     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$+454    006E52EA     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4],0x0
	$+45B    006E52F1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+462    006E52F8   ^ E9 D2FEFFFF              JMP PathOfEx.006E51CF
	$+467    006E52FD     3BC1                     CMP EAX,ECX
	$+469    006E52FF     75 0A                    JNZ SHORT PathOfEx.006E530B
	$+46B    006E5301     0FB6C3                   MOVZX EAX,BL
	$+46E    006E5304     FEC3                     INC BL
	$+470    006E5306     66:895445 B4             MOV WORD PTR SS:[EBP+EAX*2-0x4C],DX
	$+475    006E530B     83C6 04                  ADD ESI,0x4
	$+478    006E530E     83EF 01                  SUB EDI,0x1
	$+47B    006E5311   ^ 75 8E                    JNZ SHORT PathOfEx.006E52A1
	$+47D    006E5313     8B7D D0                  MOV EDI,DWORD PTR SS:[EBP-0x30]
	$+480    006E5316     8A45 F3                  MOV AL,BYTE PTR SS:[EBP-0xD]
	$+483    006E5319     885D F2                  MOV BYTE PTR SS:[EBP-0xE],BL
	$+486    006E531C     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+489    006E531F     807D F1 00               CMP BYTE PTR SS:[EBP-0xF],0x0
	$+48D    006E5323     0FB6C0                   MOVZX EAX,AL
	$+490    006E5326     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD],0x0
	$+494    006E532A     8D50 79                  LEA EDX,DWORD PTR DS:[EAX+0x79]
	$+497    006E532D     8D1497                   LEA EDX,DWORD PTR DS:[EDI+EDX*4]
	$+49A    006E5330     76 52                    JBE SHORT PathOfEx.006E5384
	$+49C    006E5332     8A5D F3                  MOV BL,BYTE PTR SS:[EBP-0xD]
	$+49F    006E5335     8D8F E4010000            LEA ECX,DWORD PTR DS:[EDI+0x1E4]
	$+4A5    006E533B     8B7D DC                  MOV EDI,DWORD PTR SS:[EBP-0x24]
	$+4A8    006E533E     8D75 98                  LEA ESI,DWORD PTR SS:[EBP-0x68]
	$+4AB    006E5341     894D E0                  MOV DWORD PTR SS:[EBP-0x20],ECX
	$+4AE    006E5344     66:8B06                  MOV AX,WORD PTR DS:[ESI]
	$+4B1    006E5347     3BCA                     CMP ECX,EDX
	$+4B3    006E5349     74 17                    JE SHORT PathOfEx.006E5362
	$+4B5    006E534B     0F1F4400 00              NOP DWORD PTR DS:[EAX+EAX]
	$+4BA    006E5350     66:3901                  CMP WORD PTR DS:[ECX],AX
	$+4BD    006E5353     74 09                    JE SHORT PathOfEx.006E535E
	$+4BF    006E5355     83C1 04                  ADD ECX,0x4
	$+4C2    006E5358     3BCA                     CMP ECX,EDX
	$+4C4    006E535A   ^ 75 F4                    JNZ SHORT PathOfEx.006E5350
	$+4C6    006E535C     EB 04                    JMP SHORT PathOfEx.006E5362
	$+4C8    006E535E     3BCA                     CMP ECX,EDX
	$+4CA    006E5360     75 0E                    JNZ SHORT PathOfEx.006E5370
	$+4CC    006E5362     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+4CE    006E5364     0FB6CB                   MOVZX ECX,BL
	$+4D1    006E5367     FEC3                     INC BL
	$+4D3    006E5369     89848D 7CFFFFFF          MOV DWORD PTR SS:[EBP+ECX*4-0x84],EAX
	$+4DA    006E5370     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+4DD    006E5373     83C6 04                  ADD ESI,0x4
	$+4E0    006E5376     83EF 01                  SUB EDI,0x1
	$+4E3    006E5379   ^ 75 C9                    JNZ SHORT PathOfEx.006E5344
	$+4E5    006E537B     8B7D D0                  MOV EDI,DWORD PTR SS:[EBP-0x30]
	$+4E8    006E537E     885D F3                  MOV BYTE PTR SS:[EBP-0xD],BL
	$+4EB    006E5381     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+4EE    006E5384     8A45 F2                  MOV AL,BYTE PTR SS:[EBP-0xE]
	$+4F1    006E5387     84C0                     TEST AL,AL
	$+4F3    006E5389     74 1C                    JE SHORT PathOfEx.006E53A7
	$+4F5    006E538B     8D75 B4                  LEA ESI,DWORD PTR SS:[EBP-0x4C]
	$+4F8    006E538E     0FB6D8                   MOVZX EBX,AL
	$+4FB    006E5391     0FB706                   MOVZX EAX,WORD PTR DS:[ESI]
	$+4FE    006E5394     8BCF                     MOV ECX,EDI
	$+500    006E5396     50                       PUSH EAX
	$+501    006E5397     E8 14100000              CALL PathOfEx.006E63B0
	$+506    006E539C     8D76 02                  LEA ESI,DWORD PTR DS:[ESI+0x2]
	$+509    006E539F     83EB 01                  SUB EBX,0x1
	$+50C    006E53A2   ^ 75 ED                    JNZ SHORT PathOfEx.006E5391
	$+50E    006E53A4     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+511    006E53A7     8A45 F3                  MOV AL,BYTE PTR SS:[EBP-0xD]
	$+514    006E53AA     84C0                     TEST AL,AL
	$+516    006E53AC     74 6A                    JE SHORT PathOfEx.006E5418
	$+518    006E53AE     8DB5 7CFFFFFF            LEA ESI,DWORD PTR SS:[EBP-0x84]
	$+51E    006E53B4     0FB6D8                   MOVZX EBX,AL
	$+521    006E53B7     66:0F1F8400 00000000     NOP WORD PTR DS:[EAX+EAX]
	$+52A    006E53C0     8A87 E3010000            MOV AL,BYTE PTR DS:[EDI+0x1E3]
	$+530    006E53C6     8A4E 02                  MOV CL,BYTE PTR DS:[ESI+0x2]
	$+533    006E53C9     0FB716                   MOVZX EDX,WORD PTR DS:[ESI]
	$+536    006E53CC     884D DC                  MOV BYTE PTR SS:[EBP-0x24],CL
	$+539    006E53CF     3C 07                    CMP AL,0x7
	$+53B    006E53D1     74 3A                    JE SHORT PathOfEx.006E540D
	$+53D    006E53D3     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
	$+540    006E53D6     0FB6C0                   MOVZX EAX,AL
	$+543    006E53D9     52                       PUSH EDX
	$+544    006E53DA     66:899487 E4010000       MOV WORD PTR DS:[EDI+EAX*4+0x1E4],DX
	$+54C    006E53E2     0FB687 E3010000          MOVZX EAX,BYTE PTR DS:[EDI+0x1E3]
	$+553    006E53E9     888C87 E6010000          MOV BYTE PTR DS:[EDI+EAX*4+0x1E6],CL
	$+55A    006E53F0     B8 80000000              MOV EAX,0x80
	$+55F    006E53F5     66:0987 E0010000         OR WORD PTR DS:[EDI+0x1E0],AX
	$+566    006E53FC     8BCF                     MOV ECX,EDI
	$+568    006E53FE     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+56A    006E5400     FE87 E3010000            INC BYTE PTR DS:[EDI+0x1E3]
	$+570    006E5406     FF50 60                  CALL DWORD PTR DS:[EAX+0x60]
	$+573    006E5409     C647 08 01               MOV BYTE PTR DS:[EDI+0x8],0x1
	$+577    006E540D     83C6 04                  ADD ESI,0x4
	$+57A    006E5410     83EB 01                  SUB EBX,0x1
	$+57D    006E5413   ^ 75 AB                    JNZ SHORT PathOfEx.006E53C0
	$+57F    006E5415     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+582    006E5418     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+585    006E541B     F7C1 00020000            TEST ECX,0x200
	$+58B    006E5421     74 37                    JE SHORT PathOfEx.006E545A
	$+58D    006E5423     6A 01                    PUSH 0x1
	$+58F    006E5425     8BCB                     MOV ECX,EBX
	$+591    006E5427     E8 A480C7FF              CALL PathOfEx.0035D4D0
	$+596    006E542C     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+598    006E542E     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+59B    006E5431     6A 01                    PUSH 0x1
	$+59D    006E5433     0FB60401                 MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+5A1    006E5437     8BCB                     MOV ECX,EBX
	$+5A3    006E5439     8847 44                  MOV BYTE PTR DS:[EDI+0x44],AL
	$+5A6    006E543C     FF43 08                  INC DWORD PTR DS:[EBX+0x8]
	$+5A9    006E543F     E8 8C80C7FF              CALL PathOfEx.0035D4D0
	$+5AE    006E5444     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+5B0    006E5446     8B43 08                  MOV EAX,DWORD PTR DS:[EBX+0x8]
	$+5B3    006E5449     0FB60401                 MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+5B7    006E544D     8BCF                     MOV ECX,EDI
	$+5B9    006E544F     8847 43                  MOV BYTE PTR DS:[EDI+0x43],AL
	$+5BC    006E5452     FF43 08                  INC DWORD PTR DS:[EBX+0x8]
	$+5BF    006E5455     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+5C1    006E5457     FF50 68                  CALL DWORD PTR DS:[EAX+0x68]
	$+5C4    006E545A     8A45 E7                  MOV AL,BYTE PTR SS:[EBP-0x19]
	$+5C7    006E545D     3847 41                  CMP BYTE PTR DS:[EDI+0x41],AL
	$+5CA    006E5460     74 07                    JE SHORT PathOfEx.006E5469
	$+5CC    006E5462     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+5CE    006E5464     8BCF                     MOV ECX,EDI
	$+5D0    006E5466     FF50 40                  CALL DWORD PTR DS:[EAX+0x40]
	$+5D3    006E5469     8A45 E6                  MOV AL,BYTE PTR SS:[EBP-0x1A]
	$+5D6    006E546C     3847 42                  CMP BYTE PTR DS:[EDI+0x42],AL
	$+5D9    006E546F     74 07                    JE SHORT PathOfEx.006E5478
	$+5DB    006E5471     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+5DD    006E5473     8BCF                     MOV ECX,EDI
	$+5DF    006E5475     FF50 44                  CALL DWORD PTR DS:[EAX+0x44]
	$+5E2    006E5478     8B5D C8                  MOV EBX,DWORD PTR SS:[EBP-0x38]
	$+5E5    006E547B     85DB                     TEST EBX,EBX
	$+5E7    006E547D     74 45                    JE SHORT PathOfEx.006E54C4
	$+5E9    006E547F     A1 18AC0A01              MOV EAX,DWORD PTR DS:[0x10AAC18]
	$+5EE    006E5484     8B77 30                  MOV ESI,DWORD PTR DS:[EDI+0x30]
	$+5F1    006E5487     8B0D 14AC0A01            MOV ECX,DWORD PTR DS:[0x10AAC14]
	$+5F7    006E548D     03F3                     ADD ESI,EBX
	$+5F9    006E548F     2BC1                     SUB EAX,ECX
	$+5FB    006E5491     A9 FCFFFFFF              TEST EAX,0xFFFFFFFC
	$+600    006E5496     75 0B                    JNZ SHORT PathOfEx.006E54A3
	$+602    006E5498     E8 B369F4FF              CALL PathOfEx.0062BE50
	$+607    006E549D     8B0D 14AC0A01            MOV ECX,DWORD PTR DS:[0x10AAC14]
	$+60D    006E54A3     8B81 8C010000            MOV EAX,DWORD PTR DS:[ECX+0x18C]
	$+613    006E54A9     3BF0                     CMP ESI,EAX
	$+615    006E54AB     8BCF                     MOV ECX,EDI
	$+617    006E54AD     0F42C6                   CMOVB EAX,ESI
	$+61A    006E54B0     50                       PUSH EAX
	$+61B    006E54B1     E8 2A000000              CALL PathOfEx.006E54E0
	$+620    006E54B6     019F DC010000            ADD DWORD PTR DS:[EDI+0x1DC],EBX
	$+626    006E54BC     66:838F E0010000 01      OR WORD PTR DS:[EDI+0x1E0],0x1
	$+62E    006E54C4     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+631    006E54C7     5F                       POP EDI
	$+632    006E54C8     5E                       POP ESI
	$+633    006E54C9     5B                       POP EBX
	$+634    006E54CA     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+63B    006E54D1     8BE5                     MOV ESP,EBP
	$+63D    006E54D3     5D                       POP EBP
	$+63E    006E54D4     C2 0400                  RETN 0x4
	$+641    006E54D7     CC                       INT3
	$+642    006E54D8     CC                       INT3
	$+643    006E54D9     CC                       INT3
	$+644    006E54DA     CC                       INT3
	$+645    006E54DB     CC                       INT3
	$+646    006E54DC     CC                       INT3
	$+647    006E54DD     CC                       INT3
	$+648    006E54DE     CC                       INT3
	$+649    006E54DF     CC                       INT3
	$+64A    006E54E0     53                       PUSH EBX
	$+64B    006E54E1     56                       PUSH ESI
	$+64C    006E54E2     57                       PUSH EDI
	$+64D    006E54E3     8BF9                     MOV EDI,ECX
	$+64F    006E54E5     8B4C24 10                MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+653    006E54E9     8A47 40                  MOV AL,BYTE PTR DS:[EDI+0x40]
	$+656    006E54EC     8B5F 30                  MOV EBX,DWORD PTR DS:[EDI+0x30]
	$+659    006E54EF     894F 30                  MOV DWORD PTR DS:[EDI+0x30],ECX
	$+65C    006E54F2     884424 10                MOV BYTE PTR SS:[ESP+0x10],AL
	$+660    006E54F6     E8 E56EF4FF              CALL PathOfEx.0062C3E0
	$+665    006E54FB     8847 40                  MOV BYTE PTR DS:[EDI+0x40],AL
	$+668    006E54FE     8BB7 00020000            MOV ESI,DWORD PTR DS:[EDI+0x200]
	$+66E    006E5504     3BB7 04020000            CMP ESI,DWORD PTR DS:[EDI+0x204]
	$+674    006E550A     74 19                    JE SHORT PathOfEx.006E5525
	$+676    006E550C     0F1F40 00                NOP DWORD PTR DS:[EAX]
	$+67A    006E5510     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+67C    006E5512     53                       PUSH EBX
	$+67D    006E5513     FF77 30                  PUSH DWORD PTR DS:[EDI+0x30]
	$+680    006E5516     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+682    006E5518     FF10                     CALL DWORD PTR DS:[EAX]
	$+684    006E551A     83C6 04                  ADD ESI,0x4
	$+687    006E551D     3BB7 04020000            CMP ESI,DWORD PTR DS:[EDI+0x204]
	$+68D    006E5523   ^ 75 EB                    JNZ SHORT PathOfEx.006E5510
	$+68F    006E5525     8A4424 10                MOV AL,BYTE PTR SS:[ESP+0x10]
	$+693    006E5529     3A47 40                  CMP AL,BYTE PTR DS:[EDI+0x40]
	$+696    006E552C     74 07                    JE SHORT PathOfEx.006E5535
	$+698    006E552E     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+69A    006E5530     8BCF                     MOV ECX,EDI
	$+69C    006E5532     FF50 3C                  CALL DWORD PTR DS:[EAX+0x3C]
	$+69F    006E5535     C647 08 01               MOV BYTE PTR DS:[EDI+0x8],0x1
	$+6A3    006E5539     5F                       POP EDI
	$+6A4    006E553A     5E                       POP ESI
	$+6A5    006E553B     5B                       POP EBX
	$+6A6    006E553C     C2 0400                  RETN 0x4
	$+6A9    006E553F     CC                       INT3
	$+6AA    006E5540     83EC 0C                  SUB ESP,0xC
	$+6AD    006E5543     53                       PUSH EBX
	$+6AE    006E5544     8BD9                     MOV EBX,ECX
	$+6B0    006E5546     56                       PUSH ESI
	$+6B1    006E5547     57                       PUSH EDI
	$+6B2    006E5548     8B7B 04                  MOV EDI,DWORD PTR DS:[EBX+0x4]
	$+6B5    006E554B     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$+6B7    006E554D     85C9                     TEST ECX,ECX
	$+6B9    006E554F     74 05                    JE SHORT PathOfEx.006E5556
	$+6BB    006E5551     E8 7AA3F2FF              CALL PathOfEx.0060F8D0
	$+6C0    006E5556     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+6C2    006E5558     C74424 10 CC8DF500       MOV DWORD PTR SS:[ESP+0x10],PathOfEx.00F58DCC          ; ASCII "Actor"
	$+6CA    006E5560     8B70 30                  MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+6CD    006E5563     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$+6D1    006E5567     50                       PUSH EAX
	$+6D2    006E5568     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+6D6    006E556C     50                       PUSH EAX
	$+6D7    006E556D     8D4E 14                  LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+6DA    006E5570     E8 0BF6C3FF              CALL PathOfEx.00324B80
	$+6DF    006E5575     8B4424 14                MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+6E3    006E5579     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+6E6    006E557C     74 40                    JE SHORT PathOfEx.006E55BE
	$+6E8    006E557E     8B70 0C                  MOV ESI,DWORD PTR DS:[EAX+0xC]
	$+6EB    006E5581     83FE FF                  CMP ESI,-0x1
	$+6EE    006E5584     74 38                    JE SHORT PathOfEx.006E55BE
	$+6F0    006E5586     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+6F3    006E5589     8B34B0                   MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$+6F6    006E558C     85F6                     TEST ESI,ESI
	$+6F8    006E558E     74 2E                    JE SHORT PathOfEx.006E55BE
	$+6FA    006E5590     8B4B 04                  MOV ECX,DWORD PTR DS:[EBX+0x4]
	$+6FD    006E5593     E8 28A6CAFF              CALL PathOfEx.0038FBC0
	$+702    006E5598     8B4E 70                  MOV ECX,DWORD PTR DS:[ESI+0x70]
	$+705    006E559B     D1E9                     SHR ECX,1
	$+707    006E559D     F6C1 01                  TEST CL,0x1
	$+70A    006E55A0     75 1C                    JNZ SHORT PathOfEx.006E55BE
	$+70C    006E55A2     6A 00                    PUSH 0x0
	$+70E    006E55A4     83C0 10                  ADD EAX,0x10
	$+711    006E55A7     8BCE                     MOV ECX,ESI
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????3b??????????74??8b??8b??ff????83????3b", 2);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????3b??????????74??8b??8b??ff????83????3b", 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x150 && dw_tmp < 0x350);
	ofs << "pt_cdword	g_o_cur_quest_info_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-359    00619927     8BCE                      MOV ECX,ESI
	$-357    00619929     C686 C0080000 01          MOV BYTE PTR DS:[ESI+0x8C0],0x1
	$-350    00619930     C70424 0000803F           MOV DWORD PTR SS:[ESP],0x3F800000                  ; UNICODE ""
	$-349    00619937     FF50 2C                   CALL DWORD PTR DS:[EAX+0x2C]
	$-346    0061993A     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-344    0061993C     51                        PUSH ECX
	$-343    0061993D     8BCE                      MOV ECX,ESI
	$-341    0061993F     C70424 0000803F           MOV DWORD PTR SS:[ESP],0x3F800000                  ; UNICODE ""
	$-33A    00619946     FF50 30                   CALL DWORD PTR DS:[EAX+0x30]
	$-337    00619949     8B8B A0000000             MOV ECX,DWORD PTR DS:[EBX+0xA0]
	$-331    0061994F     56                        PUSH ESI
	$-330    00619950     57                        PUSH EDI
	$-32F    00619951     81C1 04070000             ADD ECX,0x704
	$-329    00619957     E8 149C3E00               CALL PathOfEx.00A03570
	$-324    0061995C     47                        INC EDI
	$-323    0061995D     EB 4A                     JMP SHORT PathOfEx.006199A9
	$-321    0061995F     80BE C0080000 00          CMP BYTE PTR DS:[ESI+0x8C0],0x0
	$-31A    00619966     74 25                     JE SHORT PathOfEx.0061998D
	$-318    00619968     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-316    0061996A     51                        PUSH ECX
	$-315    0061996B     8BCE                      MOV ECX,ESI
	$-313    0061996D     C686 C0080000 00          MOV BYTE PTR DS:[ESI+0x8C0],0x0
	$-30C    00619974     C70424 0000003F           MOV DWORD PTR SS:[ESP],0x3F000000
	$-305    0061997B     FF50 2C                   CALL DWORD PTR DS:[EAX+0x2C]
	$-302    0061997E     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-300    00619980     51                        PUSH ECX
	$-2FF    00619981     8BCE                      MOV ECX,ESI
	$-2FD    00619983     C70424 00000000           MOV DWORD PTR SS:[ESP],0x0
	$-2F6    0061998A     FF50 30                   CALL DWORD PTR DS:[EAX+0x30]
	$-2F3    0061998D     8B8B A0000000             MOV ECX,DWORD PTR DS:[EBX+0xA0]
	$-2ED    00619993     81C1 04070000             ADD ECX,0x704
	$-2E7    00619999     56                        PUSH ESI
	$-2E6    0061999A     8B41 08                   MOV EAX,DWORD PTR DS:[ECX+0x8]
	$-2E3    0061999D     2B41 04                   SUB EAX,DWORD PTR DS:[ECX+0x4]
	$-2E0    006199A0     C1F8 02                   SAR EAX,0x2
	$-2DD    006199A3     50                        PUSH EAX
	$-2DC    006199A4     E8 C79B3E00               CALL PathOfEx.00A03570
	$-2D7    006199A9     C645 FC 0F                MOV BYTE PTR SS:[EBP-0x4],0xF
	$-2D3    006199AD     8B45 C4                   MOV EAX,DWORD PTR SS:[EBP-0x3C]
	$-2D0    006199B0     85C0                      TEST EAX,EAX
	$-2CE    006199B2     74 14                     JE SHORT PathOfEx.006199C8
	$-2CC    006199B4     50                        PUSH EAX
	$-2CB    006199B5     E8 46E60F00               CALL PathOfEx.00718000
	$-2C6    006199BA     8BC8                      MOV ECX,EAX
	$-2C4    006199BC     E8 CFE10F00               CALL PathOfEx.00717B90
	$-2BF    006199C1     C745 C4 00000000          MOV DWORD PTR SS:[EBP-0x3C],0x0
	$-2B8    006199C8     8B75 E4                   MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-2B5    006199CB     8B45 E8                   MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-2B2    006199CE     83C6 08                   ADD ESI,0x8
	$-2AF    006199D1     40                        INC EAX
	$-2AE    006199D2     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$-2AA    006199D6     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$-2A7    006199D9     8975 E4                   MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-2A4    006199DC     3B45 DC                   CMP EAX,DWORD PTR SS:[EBP-0x24]
	$-2A1    006199DF   ^ 0F82 8BFEFFFF             JB PathOfEx.00619870
	$-29B    006199E5     8B93 A0000000             MOV EDX,DWORD PTR DS:[EBX+0xA0]
	$-295    006199EB     8B82 0C070000             MOV EAX,DWORD PTR DS:[EDX+0x70C]
	$-28F    006199F1     2B82 08070000             SUB EAX,DWORD PTR DS:[EDX+0x708]
	$-289    006199F7     C1F8 02                   SAR EAX,0x2
	$-286    006199FA     85C0                      TEST EAX,EAX
	$-284    006199FC     0F84 1D010000             JE PathOfEx.00619B1F
	$-27E    00619A02     8B82 0C070000             MOV EAX,DWORD PTR DS:[EDX+0x70C]
	$-278    00619A08     33FF                      XOR EDI,EDI
	$-276    00619A0A     2B82 08070000             SUB EAX,DWORD PTR DS:[EDX+0x708]
	$-270    00619A10     C1F8 02                   SAR EAX,0x2
	$-26D    00619A13     85C0                      TEST EAX,EAX
	$-26B    00619A15     74 6B                     JE SHORT PathOfEx.00619A82
	$-269    00619A17     8BB2 08070000             MOV ESI,DWORD PTR DS:[EDX+0x708]
	$-263    00619A1D     0F1F00                    NOP DWORD PTR DS:[EAX]
	$-260    00619A20     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$-25E    00619A22     0FB781 4C070000           MOVZX EAX,WORD PTR DS:[ECX+0x74C]
	$-257    00619A29     66:3B05 D8F00001          CMP AX,WORD PTR DS:[0x100F0D8]
	$-250    00619A30     75 0B                     JNZ SHORT PathOfEx.00619A3D
	$-24E    00619A32     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-24B    00619A35     3981 B4080000             CMP DWORD PTR DS:[ECX+0x8B4],EAX
	$-245    00619A3B     74 19                     JE SHORT PathOfEx.00619A56
	$-243    00619A3D     8B82 0C070000             MOV EAX,DWORD PTR DS:[EDX+0x70C]
	$-23D    00619A43     47                        INC EDI
	$-23C    00619A44     2B82 08070000             SUB EAX,DWORD PTR DS:[EDX+0x708]
	$-236    00619A4A     83C6 04                   ADD ESI,0x4
	$-233    00619A4D     C1F8 02                   SAR EAX,0x2
	$-230    00619A50     3BF8                      CMP EDI,EAX
	$-22E    00619A52   ^ 72 CC                     JB SHORT PathOfEx.00619A20
	$-22C    00619A54     EB 2C                     JMP SHORT PathOfEx.00619A82
	$-22A    00619A56     8B55 BC                   MOV EDX,DWORD PTR SS:[EBP-0x44]
	$-227    00619A59     8955 CC                   MOV DWORD PTR SS:[EBP-0x34],EDX
	$-224    00619A5C     85D2                      TEST EDX,EDX
	$-222    00619A5E     74 17                     JE SHORT PathOfEx.00619A77
	$-220    00619A60     8D4A 38                   LEA ECX,DWORD PTR DS:[EDX+0x38]
	$-21D    00619A63     B8 01000000               MOV EAX,0x1
	$-218    00619A68     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX],EAX
	$-214    00619A6C     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
	$-20F    00619A71     8942 34                   MOV DWORD PTR DS:[EDX+0x34],EAX
	$-20C    00619A74     8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-209    00619A77     8945 D0                   MOV DWORD PTR SS:[EBP-0x30],EAX
	$-206    00619A7A     85C0                      TEST EAX,EAX
	$-204    00619A7C     0F85 9D000000             JNZ PathOfEx.00619B1F
	$-1FE    00619A82     8BB3 A0000000             MOV ESI,DWORD PTR DS:[EBX+0xA0]
	$-1F8    00619A88     33D2                      XOR EDX,EDX
	$-1F6    00619A8A     8B86 0C070000             MOV EAX,DWORD PTR DS:[ESI+0x70C]
	$-1F0    00619A90     2B86 08070000             SUB EAX,DWORD PTR DS:[ESI+0x708]
	$-1EA    00619A96     C1F8 02                   SAR EAX,0x2
	$-1E7    00619A99     85C0                      TEST EAX,EAX
	$-1E5    00619A9B     0F84 7E000000             JE PathOfEx.00619B1F
	$-1DF    00619AA1     8B8E 08070000             MOV ECX,DWORD PTR DS:[ESI+0x708]
	$-1D9    00619AA7     66:0F1F8400 00000000      NOP WORD PTR DS:[EAX+EAX]
	$-1D0    00619AB0     8B39                      MOV EDI,DWORD PTR DS:[ECX]
	$-1CE    00619AB2     0FB787 4C070000           MOVZX EAX,WORD PTR DS:[EDI+0x74C]
	$-1C7    00619AB9     66:3B05 D8F00001          CMP AX,WORD PTR DS:[0x100F0D8]
	$-1C0    00619AC0     74 19                     JE SHORT PathOfEx.00619ADB
	$-1BE    00619AC2     8B86 0C070000             MOV EAX,DWORD PTR DS:[ESI+0x70C]
	$-1B8    00619AC8     42                        INC EDX
	$-1B7    00619AC9     2B86 08070000             SUB EAX,DWORD PTR DS:[ESI+0x708]
	$-1B1    00619ACF     83C1 04                   ADD ECX,0x4
	$-1AE    00619AD2     C1F8 02                   SAR EAX,0x2
	$-1AB    00619AD5     3BD0                      CMP EDX,EAX
	$-1A9    00619AD7   ^ 72 D7                     JB SHORT PathOfEx.00619AB0
	$-1A7    00619AD9     EB 44                     JMP SHORT PathOfEx.00619B1F
	$-1A5    00619ADB     81C7 B0080000             ADD EDI,0x8B0
	$-19F    00619AE1     8D45 CC                   LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-19C    00619AE4     3BC7                      CMP EAX,EDI
	$-19A    00619AE6     74 31                     JE SHORT PathOfEx.00619B19
	$-198    00619AE8     8B45 CC                   MOV EAX,DWORD PTR SS:[EBP-0x34]
	$-195    00619AEB     85C0                      TEST EAX,EAX
	$-193    00619AED     74 0D                     JE SHORT PathOfEx.00619AFC
	$-191    00619AEF     50                        PUSH EAX
	$-190    00619AF0     E8 0BE50F00               CALL PathOfEx.00718000
	$-18B    00619AF5     8BC8                      MOV ECX,EAX
	$-189    00619AF7     E8 94E00F00               CALL PathOfEx.00717B90
	$-184    00619AFC     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
	$-182    00619AFE     894D CC                   MOV DWORD PTR SS:[EBP-0x34],ECX
	$-17F    00619B01     85C9                      TEST ECX,ECX
	$-17D    00619B03     74 14                     JE SHORT PathOfEx.00619B19
	$-17B    00619B05     8D41 38                   LEA EAX,DWORD PTR DS:[ECX+0x38]
	$-178    00619B08     BA 01000000               MOV EDX,0x1
	$-173    00619B0D     F0:0FC110                 LOCK XADD DWORD PTR DS:[EAX],EDX
	$-16F    00619B11     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
	$-16A    00619B16     8941 34                   MOV DWORD PTR DS:[ECX+0x34],EAX
	$-167    00619B19     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-164    00619B1C     8945 D0                   MOV DWORD PTR SS:[EBP-0x30],EAX
	$-161    00619B1F     8B8B A8000000             MOV ECX,DWORD PTR DS:[EBX+0xA8]
	$-15B    00619B25     8B55 EC                   MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-158    00619B28     42                        INC EDX
	$-157    00619B29     8B81 A0090000             MOV EAX,DWORD PTR DS:[ECX+0x9A0]
	$-151    00619B2F     C781 B0090000 00000000    MOV DWORD PTR DS:[ECX+0x9B0],0x0
	$-147    00619B39     8991 B4090000             MOV DWORD PTR DS:[ECX+0x9B4],EDX
	$-141    00619B3F     85C0                      TEST EAX,EAX
	$-13F    00619B41     78 1E                     JS SHORT PathOfEx.00619B61
	$-13D    00619B43     3B91 A8090000             CMP EDX,DWORD PTR DS:[ECX+0x9A8]
	$-137    00619B49     7E 16                     JLE SHORT PathOfEx.00619B61
	$-135    00619B4B     8BB1 AC090000             MOV ESI,DWORD PTR DS:[ECX+0x9AC]
	$-12F    00619B51     03C6                      ADD EAX,ESI
	$-12D    00619B53     3BC2                      CMP EAX,EDX
	$-12B    00619B55     7E 14                     JLE SHORT PathOfEx.00619B6B
	$-129    00619B57     2BD6                      SUB EDX,ESI
	$-127    00619B59     8991 A0090000             MOV DWORD PTR DS:[ECX+0x9A0],EDX
	$-121    00619B5F     EB 0A                     JMP SHORT PathOfEx.00619B6B
	$-11F    00619B61     C781 A0090000 00000000    MOV DWORD PTR DS:[ECX+0x9A0],0x0
	$-115    00619B6B     E8 A08E3F00               CALL PathOfEx.00A12A10
	$-110    00619B70     8B8B A8000000             MOV ECX,DWORD PTR DS:[EBX+0xA8]
	$-10A    00619B76     8BB9 A0090000             MOV EDI,DWORD PTR DS:[ECX+0x9A0]
	$-104    00619B7C     8BB1 B0090000             MOV ESI,DWORD PTR DS:[ECX+0x9B0]
	$-FE     00619B82     C781 A8090000 07000000    MOV DWORD PTR DS:[ECX+0x9A8],0x7
	$-F4     00619B8C     C781 AC090000 07000000    MOV DWORD PTR DS:[ECX+0x9AC],0x7
	$-EA     00619B96     3BFE                      CMP EDI,ESI
	$-E8     00619B98     7C 21                     JL SHORT PathOfEx.00619BBB
	$-E6     00619B9A     8B91 B4090000             MOV EDX,DWORD PTR DS:[ECX+0x9B4]
	$-E0     00619BA0     8BC2                      MOV EAX,EDX
	$-DE     00619BA2     2BC6                      SUB EAX,ESI
	$-DC     00619BA4     83F8 07                   CMP EAX,0x7
	$-D9     00619BA7     7E 12                     JLE SHORT PathOfEx.00619BBB
	$-D7     00619BA9     8D47 07                   LEA EAX,DWORD PTR DS:[EDI+0x7]
	$-D4     00619BAC     3BC2                      CMP EAX,EDX
	$-D2     00619BAE     7E 11                     JLE SHORT PathOfEx.00619BC1
	$-D0     00619BB0     8D42 F9                   LEA EAX,DWORD PTR DS:[EDX-0x7]
	$-CD     00619BB3     8981 A0090000             MOV DWORD PTR DS:[ECX+0x9A0],EAX
	$-C7     00619BB9     EB 06                     JMP SHORT PathOfEx.00619BC1
	$-C5     00619BBB     89B1 A0090000             MOV DWORD PTR DS:[ECX+0x9A0],ESI
	$-BF     00619BC1     E8 4A8E3F00               CALL PathOfEx.00A12A10
	$-BA     00619BC6     8D45 CC                   LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-B7     00619BC9     50                        PUSH EAX
	$-B6     00619BCA     8D8B 50F7FFFF             LEA ECX,DWORD PTR DS:[EBX-0x8B0]
	$-B0     00619BD0     E8 8BF8FFFF               CALL PathOfEx.00619460
	$-AB     00619BD5     8B8B A0000000             MOV ECX,DWORD PTR DS:[EBX+0xA0]
	$-A5     00619BDB     5F                        POP EDI
	$-A4     00619BDC     8381 B4080000 FF          ADD DWORD PTR DS:[ECX+0x8B4],-0x1
	$-9D     00619BE3     75 0E                     JNZ SHORT PathOfEx.00619BF3
	$-9B     00619BE5     80B9 B8080000 00          CMP BYTE PTR DS:[ECX+0x8B8],0x0
	$-94     00619BEC     74 05                     JE SHORT PathOfEx.00619BF3
	$-92     00619BEE     E8 CDB3E9FF               CALL PathOfEx.004B4FC0
	$-8D     00619BF3     8B8B A0000000             MOV ECX,DWORD PTR DS:[EBX+0xA0]
	$-87     00619BF9     81C1 3C080000             ADD ECX,0x83C
	$-81     00619BFF     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-7F     00619C01     FF50 10                   CALL DWORD PTR DS:[EAX+0x10]
	$-7C     00619C04     8B43 8C                   MOV EAX,DWORD PTR DS:[EBX-0x74]
	$-79     00619C07     8D4B 8C                   LEA ECX,DWORD PTR DS:[EBX-0x74]
	$-76     00619C0A     FF50 10                   CALL DWORD PTR DS:[EAX+0x10]
	$-73     00619C0D     8B75 B0                   MOV ESI,DWORD PTR SS:[EBP-0x50]
	$-70     00619C10     85F6                      TEST ESI,ESI
	$-6E     00619C12     74 1F                     JE SHORT PathOfEx.00619C33
	$-6C     00619C14     8B55 B4                   MOV EDX,DWORD PTR SS:[EBP-0x4C]
	$-69     00619C17     51                        PUSH ECX
	$-68     00619C18     8BCE                      MOV ECX,ESI
	$-66     00619C1A     E8 318DE8FF               CALL PathOfEx.004A2950
	$-61     00619C1F     8B45 B8                   MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-5E     00619C22     2BC6                      SUB EAX,ESI
	$-5C     00619C24     C1F8 03                   SAR EAX,0x3
	$-59     00619C27     6A 08                     PUSH 0x8
	$-57     00619C29     50                        PUSH EAX
	$-56     00619C2A     56                        PUSH ESI
	$-55     00619C2B     E8 002CE5FF               CALL PathOfEx.0046C830
	$-50     00619C30     83C4 10                   ADD ESP,0x10
	$-4D     00619C33     C645 FC 10                MOV BYTE PTR SS:[EBP-0x4],0x10
	$-49     00619C37     8B45 CC                   MOV EAX,DWORD PTR SS:[EBP-0x34]
	$-46     00619C3A     5E                        POP ESI
	$-45     00619C3B     5B                        POP EBX
	$-44     00619C3C     85C0                      TEST EAX,EAX
	$-42     00619C3E     74 0D                     JE SHORT PathOfEx.00619C4D
	$-40     00619C40     50                        PUSH EAX
	$-3F     00619C41     E8 BAE30F00               CALL PathOfEx.00718000
	$-3A     00619C46     8BC8                      MOV ECX,EAX
	$-38     00619C48     E8 43DF0F00               CALL PathOfEx.00717B90
	$-33     00619C4D     C745 FC 11000000          MOV DWORD PTR SS:[EBP-0x4],0x11
	$-2C     00619C54     8B45 BC                   MOV EAX,DWORD PTR SS:[EBP-0x44]
	$-29     00619C57     85C0                      TEST EAX,EAX
	$-27     00619C59     74 0D                     JE SHORT PathOfEx.00619C68
	$-25     00619C5B     50                        PUSH EAX
	$-24     00619C5C     E8 9FE30F00               CALL PathOfEx.00718000
	$-1F     00619C61     8BC8                      MOV ECX,EAX
	$-1D     00619C63     E8 28DF0F00               CALL PathOfEx.00717B90
	$-18     00619C68     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-15     00619C6B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-E      00619C72     8BE5                      MOV ESP,EBP
	$-C      00619C74     5D                        POP EBP
	$-B      00619C75     C3                        RETN
	$-A      00619C76     CC                        INT3
	$-9      00619C77     CC                        INT3
	$-8      00619C78     CC                        INT3
	$-7      00619C79     CC                        INT3
	$-6      00619C7A     CC                        INT3
	$-5      00619C7B     CC                        INT3
	$-4      00619C7C     CC                        INT3
	$-3      00619C7D     CC                        INT3
	$-2      00619C7E     CC                        INT3
	$-1      00619C7F     CC                        INT3
	$ ==>    00619C80     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]                     ; p1=stCD_DlgWaypointQuestItem*,then,eax=p1
	$+4      00619C84     05 B0080000               ADD EAX,0x8B0                                      ; g_o_dlg_waypoint_quest_item_db_quest_
	$+9      00619C89     50                        PUSH EAX
	$+A      00619C8A     E8 D1F7FFFF               CALL PathOfEx.00619460
	$+F      00619C8F     C2 0800                   RETN 0x8
	$+12     00619C92     CC                        INT3
	$+13     00619C93     CC                        INT3
	$+14     00619C94     CC                        INT3
	$+15     00619C95     CC                        INT3
	$+16     00619C96     CC                        INT3
	$+17     00619C97     CC                        INT3
	$+18     00619C98     CC                        INT3
	$+19     00619C99     CC                        INT3
	$+1A     00619C9A     CC                        INT3
	$+1B     00619C9B     CC                        INT3
	$+1C     00619C9C     CC                        INT3
	$+1D     00619C9D     CC                        INT3
	$+1E     00619C9E     CC                        INT3
	$+1F     00619C9F     CC                        INT3
	$+20     00619CA0     8D81 44090000             LEA EAX,DWORD PTR DS:[ECX+0x944]
	$+26     00619CA6     0F57DB                    XORPS XMM3,XMM3
	$+29     00619CA9     8B4C24 04                 MOV ECX,DWORD PTR SS:[ESP+0x4]
	$+2D     00619CAD     0F57D2                    XORPS XMM2,XMM2
	$+30     00619CB0     50                        PUSH EAX
	$+31     00619CB1     E8 5ABD3E00               CALL PathOfEx.00A05A10
	$+36     00619CB6     C2 0400                   RETN 0x4
	$+39     00619CB9     CC                        INT3
	$+3A     00619CBA     CC                        INT3
	$+3B     00619CBB     CC                        INT3
	$+3C     00619CBC     CC                        INT3
	$+3D     00619CBD     CC                        INT3
	$+3E     00619CBE     CC                        INT3
	$+3F     00619CBF     CC                        INT3
	$+40     00619CC0     55                        PUSH EBP
	$+41     00619CC1     8BEC                      MOV EBP,ESP
	$+43     00619CC3     6A FF                     PUSH -0x1
	$+45     00619CC5     68 8E88D300               PUSH PathOfEx.00D3888E
	$+4A     00619CCA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+50     00619CD0     50                        PUSH EAX
	$+51     00619CD1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+58     00619CD8     83EC 10                   SUB ESP,0x10
	$+5B     00619CDB     53                        PUSH EBX
	$+5C     00619CDC     56                        PUSH ESI
	$+5D     00619CDD     57                        PUSH EDI
	$+5E     00619CDE     8BD9                      MOV EBX,ECX
	$+60     00619CE0     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+67     00619CE7     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+6A     00619CEA     8B73 04                   MOV ESI,DWORD PTR DS:[EBX+0x4]
	$+6D     00619CED     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+6F     00619CEF     8975 E8                   MOV DWORD PTR SS:[EBP-0x18],ESI
	$+72     00619CF2     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+75     00619CF5     890B                      MOV DWORD PTR DS:[EBX],ECX
	$+77     00619CF7     833E 00                   CMP DWORD PTR DS:[ESI],0x0
	$+7A     00619CFA     74 4E                     JE SHORT PathOfEx.00619D4A
	$+7C     00619CFC     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$+80     00619D00     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+82     00619D02     8BCE                      MOV ECX,ESI
	$+84     00619D04     FF10                      CALL DWORD PTR DS:[EAX]
	$+86     00619D06     0145 E8                   ADD DWORD PTR SS:[EBP-0x18],EAX
	$+89     00619D09     83EC 08                   SUB ESP,0x8
	$+8C     00619D0C     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8F     00619D0F     8BFC                      MOV EDI,ESP
	$+91     00619D11     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+93     00619D13     8B55 08                   MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+96     00619D16     85D2                      TEST EDX,EDX
	$+98     00619D18     74 14                     JE SHORT PathOfEx.00619D2E
	$+9A     00619D1A     8D4A 38                   LEA ECX,DWORD PTR DS:[EDX+0x38]
	$+9D     00619D1D     B8 01000000               MOV EAX,0x1
	$+A2     00619D22     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX],EAX
	$+A6     00619D26     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
	$+AB     00619D2B     8942 34                   MOV DWORD PTR DS:[EDX+0x34],EAX
	$+AE     00619D2E     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+B1     00619D31     8BCE                      MOV ECX,ESI
	$+B3     00619D33     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
	$+B6     00619D36     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+B8     00619D38     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$+BB     00619D3B     8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
	$+BE     00619D3E     85F6                      TEST ESI,ESI
	$+C0     00619D40     74 0A                     JE SHORT PathOfEx.00619D4C
	$+C2     00619D42     833E 00                   CMP DWORD PTR DS:[ESI],0x0
	$+C5     00619D45   ^ 75 B9                     JNZ SHORT PathOfEx.00619D00
	$+C7     00619D47     8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+CA     00619D4A     8903                      MOV DWORD PTR DS:[EBX],EAX
	$+CC     00619D4C     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+D3     00619D53     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+D6     00619D56     85C0                      TEST EAX,EAX
	$+D8     00619D58     74 11                     JE SHORT PathOfEx.00619D6B
	$+DA     00619D5A     50                        PUSH EAX
	$+DB     00619D5B     51                        PUSH ECX
	$+DC     00619D5C     E8 9FE20F00               CALL PathOfEx.00718000
	$+E1     00619D61     83C4 04                   ADD ESP,0x4
	$+E4     00619D64     8BC8                      MOV ECX,EAX
	$+E6     00619D66     E8 25DE0F00               CALL PathOfEx.00717B90
	$+EB     00619D6B     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+EE     00619D6E     5F                        POP EDI
	$+EF     00619D6F     5E                        POP ESI
	$+F0     00619D70     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+F7     00619D77     5B                        POP EBX
	$+F8     00619D78     8BE5                      MOV ESP,EBP
	$+FA     00619D7A     5D                        POP EBP
	$+FB     00619D7B     C2 0800                   RETN 0x8
	$+FE     00619D7E     CC                        INT3
	$+FF     00619D7F     CC                        INT3
	$+100    00619D80     83C1 04                   ADD ECX,0x4
	$+103    00619D83   ^ E9 58F5FFFF               JMP PathOfEx.006192E0
	$+108    00619D88     CC                        INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b44240405????????50e8????????c20800", 4 + 1);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x800 && dw_tmp < 0x9b0);
	ofs << "pt_cdword	g_o_dlg_waypoint_quest_item_db_quest_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-BA     006111FF     CC                        INT3
	$-B9     00611200     55                        PUSH EBP                                           ; 点击stCD_DlgWaypointQuestItem时会断到
	$-B8     00611201     8BEC                      MOV EBP,ESP
	$-B6     00611203     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-B0     00611209     6A FF                     PUSH -0x1
	$-AE     0061120B     68 5C1BD200               PUSH PathOfEx.00D21B5C                             ; ASCII "胳狒"
	$-A9     00611210     50                        PUSH EAX
	$-A8     00611211     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$-A1     00611218     83EC 08                   SUB ESP,0x8
	$-9E     0061121B     53                        PUSH EBX
	$-9D     0061121C     8B5D 08                   MOV EBX,DWORD PTR SS:[EBP+0x8]                     ; then,ebx=stCD_QuestPair*
	$-9A     0061121F     56                        PUSH ESI
	$-99     00611220     8BF1                      MOV ESI,ECX                                        ; ecx=stCD_DlgWaypointMap*
	$-97     00611222     57                        PUSH EDI
	$-96     00611223     8DBE CC080000             LEA EDI,DWORD PTR DS:[ESI+0x8CC]
	$-90     00611229     3BFB                      CMP EDI,EBX
	$-8E     0061122B     74 39                     JE SHORT PathOfEx.00611266
	$-8C     0061122D     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$-8A     0061122F     85C0                      TEST EAX,EAX                                       ; eax=stCD_DbQuest*
	$-88     00611231     74 17                     JE SHORT PathOfEx.0061124A
	$-86     00611233     50                        PUSH EAX
	$-85     00611234     51                        PUSH ECX
	$-84     00611235     E8 C66D1000               CALL PathOfEx.00718000
	$-7F     0061123A     83C4 04                   ADD ESP,0x4
	$-7C     0061123D     8BC8                      MOV ECX,EAX
	$-7A     0061123F     E8 4C691000               CALL PathOfEx.00717B90
	$-75     00611244     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
	$-6F     0061124A     8B13                      MOV EDX,DWORD PTR DS:[EBX]
	$-6D     0061124C     8917                      MOV DWORD PTR DS:[EDI],EDX
	$-6B     0061124E     85D2                      TEST EDX,EDX                                       ; edx=stCD_DbQuest*
	$-69     00611250     74 14                     JE SHORT PathOfEx.00611266
	$-67     00611252     8D4A 38                   LEA ECX,DWORD PTR DS:[EDX+0x38]
	$-64     00611255     B8 01000000               MOV EAX,0x1
	$-5F     0061125A     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX],EAX                   ; ++stCD_Db.ref_cnt_
	$-5B     0061125E     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
	$-56     00611263     8942 34                   MOV DWORD PTR DS:[EDX+0x34],EAX
	$-53     00611266     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-50     00611269     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
	$-4D     0061126C     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]                    ; then,eax=stCD_QuestStatesPair*
	$-4A     0061126F     837B 04 00                CMP DWORD PTR DS:[EBX+0x4],0x0
	$-46     00611273     75 36                     JNZ SHORT PathOfEx.006112AB                        ; 跳转
	$-44     00611275     8DBE D4080000             LEA EDI,DWORD PTR DS:[ESI+0x8D4]
	$-3E     0061127B     3BF8                      CMP EDI,EAX
	$-3C     0061127D     74 23                     JE SHORT PathOfEx.006112A2
	$-3A     0061127F     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$-38     00611281     85C0                      TEST EAX,EAX
	$-36     00611283     74 17                     JE SHORT PathOfEx.0061129C
	$-34     00611285     50                        PUSH EAX
	$-33     00611286     51                        PUSH ECX
	$-32     00611287     E8 746D1000               CALL PathOfEx.00718000
	$-2D     0061128C     83C4 04                   ADD ESP,0x4
	$-2A     0061128F     8BC8                      MOV ECX,EAX
	$-28     00611291     E8 FA681000               CALL PathOfEx.00717B90
	$-23     00611296     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
	$-1D     0061129C     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
	$-17     006112A2     C747 04 00000000          MOV DWORD PTR DS:[EDI+0x4],0x0
	$-10     006112A9     EB 72                     JMP SHORT PathOfEx.0061131D
	$-E      006112AB     FFB6 C8080000             PUSH DWORD PTR DS:[ESI+0x8C8]                      ; enCD_kMapDifficulty
	$-8      006112B1     8B8E C0080000             MOV ECX,DWORD PTR DS:[ESI+0x8C0]                   ; ecx=stCD_Player*
	$-2      006112B7     53                        PUSH EBX                                           ; ebx=stCD_QuestPair*
	$-1      006112B8     50                        PUSH EAX                                           ; eax=stCD_QuestStatesPair*
	$ ==>    006112B9     E8 D24F1D00               CALL PathOfEx.007E6290                             ; call tag_获得任务的任务状态
	$+5      006112BE     8BD8                      MOV EBX,EAX
	$+7      006112C0     8DBE D4080000             LEA EDI,DWORD PTR DS:[ESI+0x8D4]
	$+D      006112C6     3BFB                      CMP EDI,EBX
	$+F      006112C8     74 27                     JE SHORT PathOfEx.006112F1
	$+11     006112CA     8B07                      MOV EAX,DWORD PTR DS:[EDI]
	$+13     006112CC     85C0                      TEST EAX,EAX
	$+15     006112CE     74 17                     JE SHORT PathOfEx.006112E7
	$+17     006112D0     50                        PUSH EAX
	$+18     006112D1     51                        PUSH ECX
	$+19     006112D2     E8 296D1000               CALL PathOfEx.00718000
	$+1E     006112D7     83C4 04                   ADD ESP,0x4
	$+21     006112DA     8BC8                      MOV ECX,EAX
	$+23     006112DC     E8 AF681000               CALL PathOfEx.00717B90
	$+28     006112E1     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
	$+2E     006112E7     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+30     006112E9     8907                      MOV DWORD PTR DS:[EDI],EAX
	$+32     006112EB     C703 00000000             MOV DWORD PTR DS:[EBX],0x0
	$+38     006112F1     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+3B     006112F4     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
	$+3E     006112F7     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+45     006112FE     8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+48     00611301     85C0                      TEST EAX,EAX
	$+4A     00611303     74 18                     JE SHORT PathOfEx.0061131D
	$+4C     00611305     50                        PUSH EAX
	$+4D     00611306     51                        PUSH ECX
	$+4E     00611307     E8 F46C1000               CALL PathOfEx.00718000
	$+53     0061130C     83C4 04                   ADD ESP,0x4
	$+56     0061130F     8BC8                      MOV ECX,EAX
	$+58     00611311     E8 7A681000               CALL PathOfEx.00717B90
	$+5D     00611316     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$+64     0061131D     8BCE                      MOV ECX,ESI
	$+66     0061131F     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+6D     00611326     E8 550B0000               CALL PathOfEx.00611E80
	$+72     0061132B     8B8E E0090000             MOV ECX,DWORD PTR DS:[ESI+0x9E0]
	$+78     00611331     56                        PUSH ESI
	$+79     00611332     81C1 04070000             ADD ECX,0x704
	$+7F     00611338     E8 23370000               CALL PathOfEx.00614A60
	$+84     0061133D     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+87     00611340     5F                        POP EDI
	$+88     00611341     5E                        POP ESI
	$+89     00611342     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+90     00611349     5B                        POP EBX
	$+91     0061134A     8BE5                      MOV ESP,EBP
	$+93     0061134C     5D                        POP EBP
	$+94     0061134D     C2 0400                   RETN 0x4
	$+97     00611350     55                        PUSH EBP
	$+98     00611351     8BEC                      MOV EBP,ESP
	$+9A     00611353     83E4 F8                   AND ESP,0xFFFFFFF8
	$+9D     00611356     83EC 10                   SUB ESP,0x10
	$+A0     00611359     53                        PUSH EBX
	$+A1     0061135A     8BD9                      MOV EBX,ECX
	$+A3     0061135C     55                        PUSH EBP
	$+A4     0061135D     56                        PUSH ESI
	$+A5     0061135E     57                        PUSH EDI
	$+A6     0061135F     8B83 E0090000             MOV EAX,DWORD PTR DS:[EBX+0x9E0]
	$+AC     00611365     33FF                      XOR EDI,EDI
	$+AE     00611367     05 08070000               ADD EAX,0x708
	$+B3     0061136C     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+B6     0061136F     2B10                      SUB EDX,DWORD PTR DS:[EAX]
	$+B8     00611371     C1FA 02                   SAR EDX,0x2
	$+BB     00611374     85D2                      TEST EDX,EDX
	$+BD     00611376     0F84 8B000000             JE PathOfEx.00611407
	$+C3     0061137C     0F1F40 00                 NOP DWORD PTR DS:[EAX]
	$+C7     00611380     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+C9     00611382     8B8B C8080000             MOV ECX,DWORD PTR DS:[EBX+0x8C8]
	$+CF     00611388     8BAB C0080000             MOV EBP,DWORD PTR DS:[EBX+0x8C0]
	$+D5     0061138E     894C24 14                 MOV DWORD PTR SS:[ESP+0x14],ECX
	$+D9     00611392     8B34B8                    MOV ESI,DWORD PTR DS:[EAX+EDI*4]
	$+DC     00611395     8B8E F0080000             MOV ECX,DWORD PTR DS:[ESI+0x8F0]
	$+E2     0061139B     8379 60 01                CMP DWORD PTR DS:[ECX+0x60],0x1
	$+E6     0061139F     75 1B                     JNZ SHORT PathOfEx.006113BC
	$+E8     006113A1     51                        PUSH ECX
	$+E9     006113A2     E8 B9DAFFFF               CALL PathOfEx.0060EE60
	$+EE     006113A7     3D 95000000               CMP EAX,0x95
	$+F3     006113AC     75 0E                     JNZ SHORT PathOfEx.006113BC
	$+F5     006113AE     8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+F9     006113B2     83F8 01                   CMP EAX,0x1
	$+FC     006113B5     74 34                     JE SHORT PathOfEx.006113EB
	$+FE     006113B7     83F8 02                   CMP EAX,0x2
	$+101    006113BA     74 2F                     JE SHORT PathOfEx.006113EB
	$+103    006113BC     8BB6 F0080000             MOV ESI,DWORD PTR DS:[ESI+0x8F0]
	$+109    006113C2     896C24 18                 MOV DWORD PTR SS:[ESP+0x18],EBP
	$+10D    006113C6     8B4E 24                   MOV ECX,DWORD PTR DS:[ESI+0x24]
	$+110    006113C9     8B46 28                   MOV EAX,DWORD PTR DS:[ESI+0x28]
	$+113    006113CC     8D1488                    LEA EDX,DWORD PTR DS:[EAX+ECX*4]
	$+116    006113CF     8B4E 28                   MOV ECX,DWORD PTR DS:[ESI+0x28]
	$+119    006113D2     8D4424 14                 LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+11D    006113D6     894424 1C                 MOV DWORD PTR SS:[ESP+0x1C],EAX
	$+121    006113DA     8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
	$+125    006113DE     50                        PUSH EAX
	$+126    006113DF     E8 AC380000               CALL PathOfEx.00614C90
	$+12B    006113E4     83C4 04                   ADD ESP,0x4
	$+12E    006113E7     84C0                      TEST AL,AL
	$+130    006113E9     75 26                     JNZ SHORT PathOfEx.00611411
	$+132    006113EB     8B83 E0090000             MOV EAX,DWORD PTR DS:[EBX+0x9E0]
	$+138    006113F1     47                        INC EDI
	$+139    006113F2     05 08070000               ADD EAX,0x708
	$+13E    006113F7     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+141    006113FA     2B08                      SUB ECX,DWORD PTR DS:[EAX]
	$+143    006113FC     C1F9 02                   SAR ECX,0x2
	$+146    006113FF     3BF9                      CMP EDI,ECX
	$+148    00611401   ^ 0F82 79FFFFFF             JB PathOfEx.00611380
	$+14E    00611407     32C0                      XOR AL,AL
	$+150    00611409     5F                        POP EDI
	$+151    0061140A     5E                        POP ESI
	$+152    0061140B     5D                        POP EBP
	$+153    0061140C     5B                        POP EBX
	$+154    0061140D     8BE5                      MOV ESP,EBP
	$+156    0061140F     5D                        POP EBP
	$+157    00611410     C3                        RETN
	$+158    00611411     5F                        POP EDI
	$+159    00611412     B0 01                     MOV AL,0x1
	$+15B    00611414     5E                        POP ESI
	$+15C    00611415     5D                        POP EBP
	$+15D    00611416     5B                        POP EBX
	$+15E    00611417     8BE5                      MOV ESP,EBP
	$+160    00611419     5D                        POP EBP
	$+161    0061141A     C3                        RETN
	$+162    0061141B     CC                        INT3
	$+163    0061141C     CC                        INT3
	$+164    0061141D     CC                        INT3
	$+165    0061141E     CC                        INT3
	$+166    0061141F     CC                        INT3
	$+167    00611420     55                        PUSH EBP
	$+168    00611421     8BEC                      MOV EBP,ESP
	$+16A    00611423     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+170    00611429     6A FF                     PUSH -0x1
	$+172    0061142B     68 6E47D200               PUSH PathOfEx.00D2476E
	$+177    00611430     50                        PUSH EAX
	$+178    00611431     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+17F    00611438     83EC 08                   SUB ESP,0x8
	$+182    0061143B     53                        PUSH EBX
	$+183    0061143C     56                        PUSH ESI
	$+184    0061143D     57                        PUSH EDI
	$+185    0061143E     8BF9                      MOV EDI,ECX
	$+187    00611440     83BF BC080000 00          CMP DWORD PTR DS:[EDI+0x8BC],0x0
	$+18E    00611447     0F84 A8000000             JE PathOfEx.006114F5
	$+194    0061144D     8B87 E0090000             MOV EAX,DWORD PTR DS:[EDI+0x9E0]
	$+19A    00611453     33F6                      XOR ESI,ESI
	$+19C    00611455     05 08070000               ADD EAX,0x708
	$+1A1    0061145A     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+1A4    0061145D     2B08                      SUB ECX,DWORD PTR DS:[EAX]
	$+1A6    0061145F     C1F9 02                   SAR ECX,0x2
	$+1A9    00611462     85C9                      TEST ECX,ECX
	$+1AB    00611464     0F84 8B000000             JE PathOfEx.006114F5
	$+1B1    0061146A     66:0F1F4400 00            NOP WORD PTR DS:[EAX+EAX]
	$+1B7    00611470     8B00                      MOV EAX,DWORD PTR DS:[EAX]
	$+1B9    00611472     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+1BC    00611475     FFB7 C8080000             PUSH DWORD PTR DS:[EDI+0x8C8]
	$+1C2    0061147B     8B14B0                    MOV EDX,DWORD PTR DS:[EAX+ESI*4]
	$+1C5    0061147E     81C2 EC080000             ADD EDX,0x8EC
	$+1CB    00611484     E8 E7DAFFFF               CALL PathOfEx.0060EF70
	$+1D0    00611489     83C4 04                   ADD ESP,0x4
	$+1D3    0061148C     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1DA    00611493     8BC8                      MOV ECX,EAX
	$+1DC    00611495     8B97 BC080000             MOV EDX,DWORD PTR DS:[EDI+0x8BC]
	$+1E2    0061149B     81C2 483F0000             ADD EDX,0x3F48
	$+1E8    006114A1     E8 DA562000               CALL PathOfEx.00816B80
	$+1ED    006114A6     8AD8                      MOV BL,AL
	$+1EF    006114A8     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+1F6    006114AF     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+1F9    006114B2     85C9                      TEST ECX,ECX
	$+1FB    006114B4     74 18                     JE SHORT PathOfEx.006114CE
	$+1FD    006114B6     51                        PUSH ECX
	$+1FE    006114B7     51                        PUSH ECX
	$+1FF    006114B8     E8 436B1000               CALL PathOfEx.00718000
	$+204    006114BD     83C4 04                   ADD ESP,0x4
	$+207    006114C0     8BC8                      MOV ECX,EAX
	$+209    006114C2     E8 C9661000               CALL PathOfEx.00717B90
	$+20E    006114C7     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
	$+215    006114CE     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+21C    006114D5     84DB                      TEST BL,BL
	$+21E    006114D7     75 2F                     JNZ SHORT PathOfEx.00611508
	$+220    006114D9     8B87 E0090000             MOV EAX,DWORD PTR DS:[EDI+0x9E0]
	$+226    006114DF     46                        INC ESI
	$+227    006114E0     05 08070000               ADD EAX,0x708
	$+22C    006114E5     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+22F    006114E8     2B08                      SUB ECX,DWORD PTR DS:[EAX]
	$+231    006114EA     C1F9 02                   SAR ECX,0x2
	$+234    006114ED     3BF1                      CMP ESI,ECX
	$+236    006114EF   ^ 0F82 7BFFFFFF             JB PathOfEx.00611470
	$+23C    006114F5     32C0                      XOR AL,AL
	$+23E    006114F7     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+241    006114FA     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+248    00611501     5F                        POP EDI
	$+249    00611502     5E                        POP ESI
	$+24A    00611503     5B                        POP EBX
	$+24B    00611504     8BE5                      MOV ESP,EBP
	$+24D    00611506     5D                        POP EBP
	$+24E    00611507     C3                        RETN
	$+24F    00611508     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+252    0061150B     B0 01                     MOV AL,0x1
	$+254    0061150D     5F                        POP EDI
	$+255    0061150E     5E                        POP ESI
	$+256    0061150F     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+25D    00611516     5B                        POP EBX
	$+25E    00611517     8BE5                      MOV ESP,EBP
	$+260    00611519     5D                        POP EBP
	$+261    0061151A     C3                        RETN
	$+262    0061151B     CC                        INT3
	*/

	//gj3
	/*
	$-263    005025A0     8B8B 24090000            MOV ECX,DWORD PTR DS:[EBX+0x924]
	$-25D    005025A6     894D 08                  MOV DWORD PTR SS:[EBP+0x8],ECX
	$-25A    005025A9     894D E8                  MOV DWORD PTR SS:[EBP-0x18],ECX
	$-257    005025AC     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-250    005025B3     8B87 C0080000            MOV EAX,DWORD PTR DS:[EDI+0x8C0]
	$-24A    005025B9     C783 94080000 00000000   MOV DWORD PTR DS:[EBX+0x894],0x0
	$-240    005025C3     8983 98080000            MOV DWORD PTR DS:[EBX+0x898],EAX
	$-23A    005025C9     85C0                     TEST EAX,EAX
	$-238    005025CB     74 1A                    JE SHORT PathOfEx.005025E7
	$-236    005025CD     8B80 14410000            MOV EAX,DWORD PTR DS:[EAX+0x4114]
	$-230    005025D3     8B88 58010000            MOV ECX,DWORD PTR DS:[EAX+0x158]
	$-22A    005025D9     E8 827EE6FF              CALL PathOfEx.0036A460
	$-225    005025DE     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-222    005025E1     8983 94080000            MOV DWORD PTR DS:[EBX+0x894],EAX
	$-21C    005025E7     C683 2C090000 00         MOV BYTE PTR DS:[EBX+0x92C],0x0
	$-215    005025EE     C683 28090000 01         MOV BYTE PTR DS:[EBX+0x928],0x1
	$-20E    005025F5     8B97 C0080000            MOV EDX,DWORD PTR DS:[EDI+0x8C0]
	$-208    005025FB     85D2                     TEST EDX,EDX
	$-206    005025FD     0F84 8C000000            JE PathOfEx.0050268F
	$-200    00502603     83BF E8080000 00         CMP DWORD PTR DS:[EDI+0x8E8],0x0
	$-1F9    0050260A     0F85 7F000000            JNZ PathOfEx.0050268F
	$-1F3    00502610     8B41 18                  MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-1F0    00502613     8B49 14                  MOV ECX,DWORD PTR DS:[ECX+0x14]
	$-1ED    00502616     8B92 14410000            MOV EDX,DWORD PTR DS:[EDX+0x4114]
	$-1E7    0050261C     8D0CC8                   LEA ECX,DWORD PTR DS:[EAX+ECX*8]
	$-1E4    0050261F     3BC1                     CMP EAX,ECX
	$-1E2    00502621     74 6C                    JE SHORT PathOfEx.0050268F
	$-1E0    00502623     8B52 14                  MOV EDX,DWORD PTR DS:[EDX+0x14]
	$-1DD    00502626     3950 04                  CMP DWORD PTR DS:[EAX+0x4],EDX
	$-1DA    00502629     74 09                    JE SHORT PathOfEx.00502634
	$-1D8    0050262B     83C0 08                  ADD EAX,0x8
	$-1D5    0050262E     3BC1                     CMP EAX,ECX
	$-1D3    00502630   ^ 75 F4                    JNZ SHORT PathOfEx.00502626
	$-1D1    00502632     EB 5B                    JMP SHORT PathOfEx.0050268F
	$-1CF    00502634     3BC1                     CMP EAX,ECX
	$-1CD    00502636     74 57                    JE SHORT PathOfEx.0050268F
	$-1CB    00502638     8D97 E4080000            LEA EDX,DWORD PTR DS:[EDI+0x8E4]
	$-1C5    0050263E     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-1C2    00502641     3BD0                     CMP EDX,EAX
	$-1C0    00502643     74 3D                    JE SHORT PathOfEx.00502682
	$-1BE    00502645     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-1BC    00502647     85C0                     TEST EAX,EAX
	$-1BA    00502649     74 1D                    JE SHORT PathOfEx.00502668
	$-1B8    0050264B     50                       PUSH EAX
	$-1B7    0050264C     51                       PUSH ECX
	$-1B6    0050264D     E8 FEBC1200              CALL PathOfEx.0062E350
	$-1B1    00502652     83C4 04                  ADD ESP,0x4
	$-1AE    00502655     8BC8                     MOV ECX,EAX
	$-1AC    00502657     E8 C4B81200              CALL PathOfEx.0062DF20
	$-1A7    0050265C     8D97 E4080000            LEA EDX,DWORD PTR DS:[EDI+0x8E4]
	$-1A1    00502662     C702 00000000            MOV DWORD PTR DS:[EDX],0x0
	$-19B    00502668     8932                     MOV DWORD PTR DS:[EDX],ESI
	$-199    0050266A     85F6                     TEST ESI,ESI
	$-197    0050266C     74 14                    JE SHORT PathOfEx.00502682
	$-195    0050266E     8D4E 38                  LEA ECX,DWORD PTR DS:[ESI+0x38]
	$-192    00502671     B8 01000000              MOV EAX,0x1
	$-18D    00502676     F0:0FC101                LOCK XADD DWORD PTR DS:[ECX],EAX
	$-189    0050267A     A1 B0AA0701              MOV EAX,DWORD PTR DS:[0x107AAB0]
	$-184    0050267F     8946 34                  MOV DWORD PTR DS:[ESI+0x34],EAX
	$-181    00502682     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-17E    00502685     8942 04                  MOV DWORD PTR DS:[EDX+0x4],EAX
	$-17B    00502688     C683 2C090000 01         MOV BYTE PTR DS:[EBX+0x92C],0x1
	$-174    0050268F     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$-16D    00502696     85F6                     TEST ESI,ESI
	$-16B    00502698     74 18                    JE SHORT PathOfEx.005026B2
	$-169    0050269A     56                       PUSH ESI
	$-168    0050269B     51                       PUSH ECX
	$-167    0050269C     E8 AFBC1200              CALL PathOfEx.0062E350
	$-162    005026A1     83C4 04                  ADD ESP,0x4
	$-15F    005026A4     8BC8                     MOV ECX,EAX
	$-15D    005026A6     E8 75B81200              CALL PathOfEx.0062DF20
	$-158    005026AB     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-151    005026B2     8B55 EC                  MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-14E    005026B5     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-147    005026BC     42                       INC EDX
	$-146    005026BD     8B8F 080A0000            MOV ECX,DWORD PTR DS:[EDI+0xA08]
	$-140    005026C3     8955 EC                  MOV DWORD PTR SS:[EBP-0x14],EDX
	$-13D    005026C6     8B81 0C070000            MOV EAX,DWORD PTR DS:[ECX+0x70C]
	$-137    005026CC     2B81 08070000            SUB EAX,DWORD PTR DS:[ECX+0x708]
	$-131    005026D2     C1F8 02                  SAR EAX,0x2
	$-12E    005026D5     3BD0                     CMP EDX,EAX
	$-12C    005026D7   ^ 0F82 93FEFFFF            JB PathOfEx.00502570
	$-126    005026DD     8BCF                     MOV ECX,EDI
	$-124    005026DF     E8 8C2D0000              CALL PathOfEx.00505470
	$-11F    005026E4     8BCF                     MOV ECX,EDI
	$-11D    005026E6     E8 752B0000              CALL PathOfEx.00505260
	$-118    005026EB     8BB7 0C0A0000            MOV ESI,DWORD PTR DS:[EDI+0xA0C]
	$-112    005026F1     8B9F C4080000            MOV EBX,DWORD PTR DS:[EDI+0x8C4]
	$-10C    005026F7     8B8E B8080000            MOV ECX,DWORD PTR DS:[ESI+0x8B8]
	$-106    005026FD     85C9                     TEST ECX,ECX
	$-104    005026FF     74 0C                    JE SHORT PathOfEx.0050270D
	$-102    00502701     8D86 B4080000            LEA EAX,DWORD PTR DS:[ESI+0x8B4]
	$-FC     00502707     50                       PUSH EAX
	$-FB     00502708     E8 D3D4E7FF              CALL PathOfEx.0037FBE0
	$-F6     0050270D     899E B8080000            MOV DWORD PTR DS:[ESI+0x8B8],EBX
	$-F0     00502713     85DB                     TEST EBX,EBX
	$-EE     00502715     74 18                    JE SHORT PathOfEx.0050272F
	$-EC     00502717     8D86 B4080000            LEA EAX,DWORD PTR DS:[ESI+0x8B4]
	$-E6     0050271D     8945 08                  MOV DWORD PTR SS:[EBP+0x8],EAX
	$-E3     00502720     8D8B 00020000            LEA ECX,DWORD PTR DS:[EBX+0x200]
	$-DD     00502726     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-DA     00502729     50                       PUSH EAX
	$-D9     0050272A     E8 A1BEE7FF              CALL PathOfEx.0037E5D0
	$-D4     0050272F     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-D1     00502732     5F                       POP EDI
	$-D0     00502733     5E                       POP ESI
	$-CF     00502734     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-C8     0050273B     5B                       POP EBX
	$-C7     0050273C     8BE5                     MOV ESP,EBP
	$-C5     0050273E     5D                       POP EBP
	$-C4     0050273F     C2 0400                  RETN 0x4
	$-C1     00502742     CC                       INT3
	$-C0     00502743     CC                       INT3
	$-BF     00502744     CC                       INT3
	$-BE     00502745     CC                       INT3
	$-BD     00502746     CC                       INT3
	$-BC     00502747     CC                       INT3
	$-BB     00502748     CC                       INT3
	$-BA     00502749     CC                       INT3
	$-B9     0050274A     CC                       INT3
	$-B8     0050274B     CC                       INT3
	$-B7     0050274C     CC                       INT3
	$-B6     0050274D     CC                       INT3
	$-B5     0050274E     CC                       INT3
	$-B4     0050274F     CC                       INT3
	$-B3     00502750     55                       PUSH EBP
	$-B2     00502751     8BEC                     MOV EBP,ESP
	$-B0     00502753     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-AA     00502759     6A FF                    PUSH -0x1
	$-A8     0050275B     68 2CD9D700              PUSH PathOfEx.00D7D92C
	$-A3     00502760     50                       PUSH EAX
	$-A2     00502761     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-9B     00502768     83EC 08                  SUB ESP,0x8
	$-98     0050276B     53                       PUSH EBX
	$-97     0050276C     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-94     0050276F     56                       PUSH ESI
	$-93     00502770     57                       PUSH EDI
	$-92     00502771     8BF9                     MOV EDI,ECX
	$-90     00502773     8DB7 D4080000            LEA ESI,DWORD PTR DS:[EDI+0x8D4]
	$-8A     00502779     3BF3                     CMP ESI,EBX
	$-88     0050277B     74 39                    JE SHORT PathOfEx.005027B6
	$-86     0050277D     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-84     0050277F     85C0                     TEST EAX,EAX
	$-82     00502781     74 17                    JE SHORT PathOfEx.0050279A
	$-80     00502783     50                       PUSH EAX
	$-7F     00502784     51                       PUSH ECX
	$-7E     00502785     E8 C6BB1200              CALL PathOfEx.0062E350
	$-79     0050278A     83C4 04                  ADD ESP,0x4
	$-76     0050278D     8BC8                     MOV ECX,EAX
	$-74     0050278F     E8 8CB71200              CALL PathOfEx.0062DF20
	$-6F     00502794     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$-69     0050279A     8B13                     MOV EDX,DWORD PTR DS:[EBX]
	$-67     0050279C     8916                     MOV DWORD PTR DS:[ESI],EDX
	$-65     0050279E     85D2                     TEST EDX,EDX
	$-63     005027A0     74 14                    JE SHORT PathOfEx.005027B6
	$-61     005027A2     8D4A 38                  LEA ECX,DWORD PTR DS:[EDX+0x38]
	$-5E     005027A5     B8 01000000              MOV EAX,0x1
	$-59     005027AA     F0:0FC101                LOCK XADD DWORD PTR DS:[ECX],EAX
	$-55     005027AE     A1 B0AA0701              MOV EAX,DWORD PTR DS:[0x107AAB0]
	$-50     005027B3     8942 34                  MOV DWORD PTR DS:[EDX+0x34],EAX
	$-4D     005027B6     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-4A     005027B9     8946 04                  MOV DWORD PTR DS:[ESI+0x4],EAX
	$-47     005027BC     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-44     005027BF     837B 04 00               CMP DWORD PTR DS:[EBX+0x4],0x0
	$-40     005027C3     75 36                    JNZ SHORT PathOfEx.005027FB
	$-3E     005027C5     8DB7 DC080000            LEA ESI,DWORD PTR DS:[EDI+0x8DC]
	$-38     005027CB     3BF0                     CMP ESI,EAX
	$-36     005027CD     74 23                    JE SHORT PathOfEx.005027F2
	$-34     005027CF     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-32     005027D1     85C0                     TEST EAX,EAX
	$-30     005027D3     74 17                    JE SHORT PathOfEx.005027EC
	$-2E     005027D5     50                       PUSH EAX
	$-2D     005027D6     51                       PUSH ECX
	$-2C     005027D7     E8 74BB1200              CALL PathOfEx.0062E350
	$-27     005027DC     83C4 04                  ADD ESP,0x4
	$-24     005027DF     8BC8                     MOV ECX,EAX
	$-22     005027E1     E8 3AB71200              CALL PathOfEx.0062DF20
	$-1D     005027E6     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$-17     005027EC     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$-11     005027F2     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4],0x0
	$-A      005027F9     EB 6C                    JMP SHORT PathOfEx.00502867
	$-8      005027FB     8B8F C4080000            MOV ECX,DWORD PTR DS:[EDI+0x8C4]                   ; ecx=stCD_Player*
	$-2      00502801     53                       PUSH EBX                                           ; ebx=stCD_QuestPair*
	$-1      00502802     50                       PUSH EAX                                           ; eax=stCD_QuestStatesPair*
	$ ==>    00502803     E8 38302000              CALL PathOfEx.00705840                             ; call tag_获得任务的任务状态
	$+5      00502808     8BD8                     MOV EBX,EAX
	$+7      0050280A     8DB7 DC080000            LEA ESI,DWORD PTR DS:[EDI+0x8DC]
	$+D      00502810     3BF3                     CMP ESI,EBX
	$+F      00502812     74 27                    JE SHORT PathOfEx.0050283B
	$+11     00502814     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+13     00502816     85C0                     TEST EAX,EAX
	$+15     00502818     74 17                    JE SHORT PathOfEx.00502831
	$+17     0050281A     50                       PUSH EAX
	$+18     0050281B     51                       PUSH ECX
	$+19     0050281C     E8 2FBB1200              CALL PathOfEx.0062E350
	$+1E     00502821     83C4 04                  ADD ESP,0x4
	$+21     00502824     8BC8                     MOV ECX,EAX
	$+23     00502826     E8 F5B61200              CALL PathOfEx.0062DF20
	$+28     0050282B     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$+2E     00502831     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+30     00502833     8906                     MOV DWORD PTR DS:[ESI],EAX
	$+32     00502835     C703 00000000            MOV DWORD PTR DS:[EBX],0x0
	$+38     0050283B     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+3B     0050283E     8946 04                  MOV DWORD PTR DS:[ESI+0x4],EAX
	$+3E     00502841     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+45     00502848     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+48     0050284B     85C0                     TEST EAX,EAX
	$+4A     0050284D     74 18                    JE SHORT PathOfEx.00502867
	$+4C     0050284F     50                       PUSH EAX
	$+4D     00502850     51                       PUSH ECX
	$+4E     00502851     E8 FABA1200              CALL PathOfEx.0062E350
	$+53     00502856     83C4 04                  ADD ESP,0x4
	$+56     00502859     8BC8                     MOV ECX,EAX
	$+58     0050285B     E8 C0B61200              CALL PathOfEx.0062DF20
	$+5D     00502860     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$+64     00502867     8BCF                     MOV ECX,EDI
	$+66     00502869     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+6D     00502870     E8 5B0A0000              CALL PathOfEx.005032D0
	$+72     00502875     8B8F 080A0000            MOV ECX,DWORD PTR DS:[EDI+0xA08]
	$+78     0050287B     57                       PUSH EDI
	$+79     0050287C     81C1 04070000            ADD ECX,0x704
	$+7F     00502882     E8 A93A0000              CALL PathOfEx.00506330
	$+84     00502887     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+87     0050288A     5F                       POP EDI
	$+88     0050288B     5E                       POP ESI
	$+89     0050288C     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+90     00502893     5B                       POP EBX
	$+91     00502894     8BE5                     MOV ESP,EBP
	$+93     00502896     5D                       POP EBP
	$+94     00502897     C2 0400                  RETN 0x4
	$+97     0050289A     CC                       INT3
	$+98     0050289B     CC                       INT3
	$+99     0050289C     CC                       INT3
	$+9A     0050289D     CC                       INT3
	$+9B     0050289E     CC                       INT3
	$+9C     0050289F     CC                       INT3
	$+9D     005028A0     55                       PUSH EBP
	$+9E     005028A1     8BEC                     MOV EBP,ESP
	$+A0     005028A3     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+A6     005028A9     6A FF                    PUSH -0x1
	$+A8     005028AB     68 DE0BD800              PUSH PathOfEx.00D80BDE
	$+AD     005028B0     50                       PUSH EAX
	$+AE     005028B1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+B5     005028B8     83EC 08                  SUB ESP,0x8
	$+B8     005028BB     53                       PUSH EBX
	$+B9     005028BC     56                       PUSH ESI
	$+BA     005028BD     57                       PUSH EDI
	$+BB     005028BE     8BF9                     MOV EDI,ECX
	$+BD     005028C0     83BF C0080000 00         CMP DWORD PTR DS:[EDI+0x8C0],0x0
	$+C4     005028C7     0F84 92000000            JE PathOfEx.0050295F
	$+CA     005028CD     8B87 080A0000            MOV EAX,DWORD PTR DS:[EDI+0xA08]
	$+D0     005028D3     33F6                     XOR ESI,ESI
	$+D2     005028D5     05 08070000              ADD EAX,0x708
	$+D7     005028DA     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+DA     005028DD     2B08                     SUB ECX,DWORD PTR DS:[EAX]
	$+DC     005028DF     C1F9 02                  SAR ECX,0x2
	$+DF     005028E2     85C9                     TEST ECX,ECX
	$+E1     005028E4     74 79                    JE SHORT PathOfEx.0050295F
	$+E3     005028E6     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+E5     005028E8     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+E8     005028EB     51                       PUSH ECX
	$+E9     005028EC     8B04B0                   MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+EC     005028EF     8B88 24090000            MOV ECX,DWORD PTR DS:[EAX+0x924]
	$+F2     005028F5     E8 6658E8FF              CALL PathOfEx.00388160
	$+F7     005028FA     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+FE     00502901     8BC8                     MOV ECX,EAX
	$+100    00502903     8B97 C0080000            MOV EDX,DWORD PTR DS:[EDI+0x8C0]
	$+106    00502909     81C2 D0450000            ADD EDX,0x45D0
	$+10C    0050290F     E8 4C002300              CALL PathOfEx.00732960
	$+111    00502914     8AD8                     MOV BL,AL
	$+113    00502916     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+11A    0050291D     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+11D    00502920     85C9                     TEST ECX,ECX
	$+11F    00502922     74 18                    JE SHORT PathOfEx.0050293C
	$+121    00502924     51                       PUSH ECX
	$+122    00502925     51                       PUSH ECX
	$+123    00502926     E8 25BA1200              CALL PathOfEx.0062E350
	$+128    0050292B     83C4 04                  ADD ESP,0x4
	$+12B    0050292E     8BC8                     MOV ECX,EAX
	$+12D    00502930     E8 EBB51200              CALL PathOfEx.0062DF20
	$+132    00502935     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$+139    0050293C     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+140    00502943     84DB                     TEST BL,BL
	$+142    00502945     75 2B                    JNZ SHORT PathOfEx.00502972
	$+144    00502947     8B87 080A0000            MOV EAX,DWORD PTR DS:[EDI+0xA08]
	$+14A    0050294D     46                       INC ESI
	$+14B    0050294E     05 08070000              ADD EAX,0x708
	$+150    00502953     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+153    00502956     2B08                     SUB ECX,DWORD PTR DS:[EAX]
	$+155    00502958     C1F9 02                  SAR ECX,0x2
	$+158    0050295B     3BF1                     CMP ESI,ECX
	$+15A    0050295D   ^ 72 87                    JB SHORT PathOfEx.005028E6
	$+15C    0050295F     32C0                     XOR AL,AL
	$+15E    00502961     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+161    00502964     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+168    0050296B     5F                       POP EDI
	$+169    0050296C     5E                       POP ESI
	$+16A    0050296D     5B                       POP EBX
	$+16B    0050296E     8BE5                     MOV ESP,EBP
	$+16D    00502970     5D                       POP EBP
	$+16E    00502971     C3                       RETN
	$+16F    00502972     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+172    00502975     B0 01                    MOV AL,0x1
	$+174    00502977     5F                       POP EDI
	$+175    00502978     5E                       POP ESI
	$+176    00502979     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+17D    00502980     5B                       POP EBX
	$+17E    00502981     8BE5                     MOV ESP,EBP
	$+180    00502983     5D                       POP EBP
	$+181    00502984     C3                       RETN
	$+182    00502985     CC                       INT3
	$+183    00502986     CC                       INT3
	$+184    00502987     CC                       INT3
	$+185    00502988     CC                       INT3
	$+186    00502989     CC                       INT3
	$+187    0050298A     CC                       INT3
	$+188    0050298B     CC                       INT3
	$+189    0050298C     CC                       INT3
	$+18A    0050298D     CC                       INT3
	$+18B    0050298E     CC                       INT3
	$+18C    0050298F     CC                       INT3
	$+18D    00502990     55                       PUSH EBP
	$+18E    00502991     8BEC                     MOV EBP,ESP
	$+190    00502993     83E4 F8                  AND ESP,0xFFFFFFF8
	$+193    00502996     83EC 0C                  SUB ESP,0xC
	$+196    00502999     807D 08 00               CMP BYTE PTR SS:[EBP+0x8],0x0
	$+19A    0050299D     53                       PUSH EBX
	$+19B    0050299E     56                       PUSH ESI
	$+19C    0050299F     8BD9                     MOV EBX,ECX
	$+19E    005029A1     57                       PUSH EDI
	$+19F    005029A2     895C24 14                MOV DWORD PTR SS:[ESP+0x14],EBX
	$+1A3    005029A6     74 62                    JE SHORT PathOfEx.00502A0A
	$+1A5    005029A8     83BB C0080000 00         CMP DWORD PTR DS:[EBX+0x8C0],0x0
	$+1AC    005029AF     74 59                    JE SHORT PathOfEx.00502A0A
	$+1AE    005029B1     8B8B 080A0000            MOV ECX,DWORD PTR DS:[EBX+0xA08]
	$+1B4    005029B7     C683 F9090000 01         MOV BYTE PTR DS:[EBX+0x9F9],0x1
	$+1BB    005029BE     895C24 10                MOV DWORD PTR SS:[ESP+0x10],EBX
	$+1BF    005029C2     8B81 08070000            MOV EAX,DWORD PTR DS:[ECX+0x708]
	$+1C5    005029C8     3B81 0C070000            CMP EAX,DWORD PTR DS:[ECX+0x70C]
	$+1CB    005029CE     74 68                    JE SHORT PathOfEx.00502A38
	$+1CD    005029D0     8BF0                     MOV ESI,EAX
	$+1CF    005029D2     33D2                     XOR EDX,EDX
	$+1D1    005029D4     8B81 0C070000            MOV EAX,DWORD PTR DS:[ECX+0x70C]
	$+1D7    005029DA     33FF                     XOR EDI,EDI
	$+1D9    005029DC     8BC8                     MOV ECX,EAX
	$+1DB    005029DE     2BCE                     SUB ECX,ESI
	$+1DD    005029E0     83C1 03                  ADD ECX,0x3
	$+1E0    005029E3     C1E9 02                  SHR ECX,0x2
	$+1E3    005029E6     3BF0                     CMP ESI,EAX
	$+1E5    005029E8     0F47CA                   CMOVA ECX,EDX
	$+1E8    005029EB     85C9                     TEST ECX,ECX
	$+1EA    005029ED     74 49                    JE SHORT PathOfEx.00502A38
	$+1EC    005029EF     8BD9                     MOV EBX,ECX
	$+1EE    005029F1     FF36                     PUSH DWORD PTR DS:[ESI]
	$+1F0    005029F3     8D4C24 14                LEA ECX,DWORD PTR SS:[ESP+0x14]
	$+1F4    005029F7     E8 54000000              CALL PathOfEx.00502A50
	$+1F9    005029FC     47                       INC EDI
	$+1FA    005029FD     8D76 04                  LEA ESI,DWORD PTR DS:[ESI+0x4]
	$+1FD    00502A00     3BFB                     CMP EDI,EBX
	$+1FF    00502A02   ^ 75 ED                    JNZ SHORT PathOfEx.005029F1
	$+201    00502A04     8B5C24 14                MOV EBX,DWORD PTR SS:[ESP+0x14]
	$+205    00502A08     EB 2E                    JMP SHORT PathOfEx.00502A38
	$+207    00502A0A     8B8B 0C0A0000            MOV ECX,DWORD PTR DS:[EBX+0xA0C]
	$+20D    00502A10     6A 00                    PUSH 0x0
	$+20F    00502A12     C683 F9090000 00         MOV BYTE PTR DS:[EBX+0x9F9],0x0
	$+216    00502A19     E8 62AB4400              CALL PathOfEx.0094D580
	$+21B    00502A1E     8B8B 080A0000            MOV ECX,DWORD PTR DS:[EBX+0xA08]
	$+221    00502A24     C64424 14 00             MOV BYTE PTR SS:[ESP+0x14],0x0
	$+226    00502A29     81C1 04070000            ADD ECX,0x704
	$+22C    00502A2F     FF7424 14                PUSH DWORD PTR SS:[ESP+0x14]
	$+230    00502A33     E8 38380000              CALL PathOfEx.00506270
	$+235    00502A38     8BCB                     MOV ECX,EBX
	$+237    00502A3A     E8 312A0000              CALL PathOfEx.00505470
	$+23C    00502A3F     8BCB                     MOV ECX,EBX
	$+23E    00502A41     E8 1A280000              CALL PathOfEx.00505260
	$+243    00502A46     5F                       POP EDI
	$+244    00502A47     5E                       POP ESI
	$+245    00502A48     5B                       POP EBX
	$+246    00502A49     8BE5                     MOV ESP,EBP
	$+248    00502A4B     5D                       POP EBP
	$+249    00502A4C     C2 0400                  RETN 0x4
	$+24C    00502A4F     CC                       INT3
	$+24D    00502A50     55                       PUSH EBP
	$+24E    00502A51     8BEC                     MOV EBP,ESP
	$+250    00502A53     6A FF                    PUSH -0x1
	$+252    00502A55     68 DE51D800              PUSH PathOfEx.00D851DE
	$+257    00502A5A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+25D    00502A60     50                       PUSH EAX
	$+25E    00502A61     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+265    00502A68     83EC 0C                  SUB ESP,0xC
	$+268    00502A6B     53                       PUSH EBX
	$+269    00502A6C     56                       PUSH ESI
	$+26A    00502A6D     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+26D    00502A70     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+270    00502A73     57                       PUSH EDI
	$+271    00502A74     8BD9                     MOV EBX,ECX
	$+273    00502A76     50                       PUSH EAX
	$+274    00502A77     8B8E 24090000            MOV ECX,DWORD PTR DS:[ESI+0x924]
	$+27A    00502A7D     E8 DE56E8FF              CALL PathOfEx.00388160
	$+27F    00502A82     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+286    00502A89     8B7D EC                  MOV EDI,DWORD PTR SS:[EBP-0x14]
	$+289    00502A8C     85FF                     TEST EDI,EDI
	$+28B    00502A8E     74 6A                    JE SHORT PathOfEx.00502AFA
	$+28D    00502A90     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+28F    00502A92     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+292    00502A95     8B90 C0080000            MOV EDX,DWORD PTR DS:[EAX+0x8C0]
	$+298    00502A9B     81C2 D0450000            ADD EDX,0x45D0
	$+29E    00502AA1     E8 BAFE2200              CALL PathOfEx.00732960
	$+2A3    00502AA6     84C0                     TEST AL,AL
	$+2A5    00502AA8     74 35                    JE SHORT PathOfEx.00502ADF
	$+2A7    00502AAA     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$+2A9    00502AAC     8B47 1A                  MOV EAX,DWORD PTR DS:[EDI+0x1A]
	$+2AC    00502AAF     3B81 000A0000            CMP EAX,DWORD PTR DS:[ECX+0xA00]
	$+2B2    00502AB5     74 28                    JE SHORT PathOfEx.00502ADF
	$+2B4    00502AB7     8B81 C0080000            MOV EAX,DWORD PTR DS:[ECX+0x8C0]
	$+2BA    00502ABD     8D96 20090000            LEA EDX,DWORD PTR DS:[ESI+0x920]
	$+2C0    00502AC3     8B89 C4080000            MOV ECX,DWORD PTR DS:[ECX+0x8C4]
	$+2C6    00502AC9     05 D0450000              ADD EAX,0x45D0
	*/
	addr_tmp = scan_exe_.ScanCall("e8????????8b??8d??????????3b??74??8b??85??74??5051e8????????83????8b??e8????????c7??????????8b??89??c7??????????8b", 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_find_quest_state_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
