#pragma once
/*
@author zhp
@date 2017/2/9 17:49
@purpose for player asm record
*/

//tag_获取实体属性信息的偏移_30
//e8????????39????74??f3??????????????eb??f3??????????????f3
/*
$-67     007E1910     55                           PUSH EBP                                             ; 猜测这个函数是处理自动寻路的上层函数，因为它无参数
$-66     007E1911     8BEC                         MOV EBP, ESP
$-64     007E1913     6A FF                        PUSH -0x1
$-62     007E1915     68 02FFD100                  PUSH PathOfEx.00D1FF02
$-5D     007E191A     64:A1 00000000               MOV EAX, DWORD PTR FS:[0]
$-57     007E1920     50                           PUSH EAX
$-56     007E1921     64:8925 00000000             MOV DWORD PTR FS:[0], ESP
$-4F     007E1928     83EC 34                      SUB ESP, 0x34
$-4C     007E192B     53                           PUSH EBX
$-4B     007E192C     8BD9                         MOV EBX, ECX                                         ; ecx==stCD_Actor*，角色对象
$-49     007E192E     33C0                         XOR EAX, EAX
$-47     007E1930     56                           PUSH ESI
$-46     007E1931     57                           PUSH EDI
$-45     007E1932     895D E8                      MOV DWORD PTR SS:[EBP-0x18], EBX
$-42     007E1935     8B4B 30                      MOV ECX, DWORD PTR DS:[EBX+0x30]
$-3F     007E1938     8945 EC                      MOV DWORD PTR SS:[EBP-0x14], EAX
$-3C     007E193B     E8 90CF2F00                  CALL PathOfEx.00ADE8D0
$-37     007E1940     0FB7C0                       MOVZX EAX, AX
$-34     007E1943     8945 D8                      MOV DWORD PTR SS:[EBP-0x28], EAX
$-31     007E1946     66:3B05 3C730001             CMP AX, WORD PTR DS:[0x100733C]
$-2A     007E194D     75 4C                        JNZ SHORT PathOfEx.007E199B
$-28     007E194F     8B73 04                      MOV ESI, DWORD PTR DS:[EBX+0x4]                      ; 执行后esi==实体对象
$-25     007E1952     8B0E                         MOV ECX, DWORD PTR DS:[ESI]
$-23     007E1954     85C9                         TEST ECX, ECX
$-21     007E1956     74 05                        JE SHORT PathOfEx.007E195D
$-1F     007E1958     E8 43F4F2FF                  CALL PathOfEx.00710DA0
$-1A     007E195D     8B06                         MOV EAX, DWORD PTR DS:[ESI]
$-18     007E195F     C745 E4 8C23EE00             MOV DWORD PTR SS:[EBP-0x1C], PathOfEx.00EE238C       ; ASCII "Player"
$-11     007E1966     8B48 30                      MOV ECX, DWORD PTR DS:[EAX+0x30]                     ; g_so_get_entity_attri_info_
$-E      007E1969     8D45 E4                      LEA EAX, DWORD PTR SS:[EBP-0x1C]
$-B      007E196C     50                           PUSH EAX
$-A      007E196D     8D45 E0                      LEA EAX, DWORD PTR SS:[EBP-0x20]
$-7      007E1970     50                           PUSH EAX
$-6      007E1971     8B71 18                      MOV ESI, DWORD PTR DS:[ECX+0x18]
$-3      007E1974     83C1 14                      ADD ECX, 0x14                                        ; g_so_get_entity_attri_list_
$ ==>    007E1977     E8 B406C8FF                  CALL PathOfEx.00462030                               ; 获得实体属性信息（有属性名）
	$ ==>    00462030     53                           PUSH EBX                                             ; 根据属性名获得实体对象的属性信息
	$+1      00462031     56                           PUSH ESI                                             ; 有两个参数，第二个参数是属性名arcii字符串指针的指针
	$+2      00462032     57                           PUSH EDI                                             ; 第一个参数是结果，是个指针的引用，保存结果对象
	$+3      00462033     8B7C24 14                    MOV EDI, DWORD PTR SS:[ESP+0x14]                     ; edi=字符串指针的指针
	$+7      00462037     8BF1                         MOV ESI, ECX
	$+9      00462039     0FB607                       MOVZX EAX, BYTE PTR DS:[EDI]
	$+C      0046203C     35 C59D1C81                  XOR EAX, 0x811C9DC5
	$+11     00462041     69D0 93010001                IMUL EDX, EAX, PathOfEx.01000193
	$+17     00462047     0FB647 01                    MOVZX EAX, BYTE PTR DS:[EDI+0x1]
	$+1B     0046204B     33D0                         XOR EDX, EAX
	$+1D     0046204D     0FB647 02                    MOVZX EAX, BYTE PTR DS:[EDI+0x2]
	$+21     00462051     69D2 93010001                IMUL EDX, EDX, PathOfEx.01000193
	$+27     00462057     33D0                         XOR EDX, EAX
	$+29     00462059     0FB647 03                    MOVZX EAX, BYTE PTR DS:[EDI+0x3]
	$+2D     0046205D     69CA 93010001                IMUL ECX, EDX, PathOfEx.01000193
	$+33     00462063     8B56 0C                      MOV EDX, DWORD PTR DS:[ESI+0xC]                      ; 数组地址，存放的是stCD_EntityAttriInfoHolder数组
	$+36     00462066     33C8                         XOR ECX, EAX
	$+38     00462068     69C1 93010001                IMUL EAX, ECX, PathOfEx.01000193
	$+3E     0046206E     8B4E 18                      MOV ECX, DWORD PTR DS:[ESI+0x18]                     ; 执行后ecx=stCD_EntityAttriInfoHolder元素的个数
	$+41     00462071     8B76 04                      MOV ESI, DWORD PTR DS:[ESI+0x4]
	$+44     00462074     23C8                         AND ECX, EAX
	$+46     00462076     8B04CA                       MOV EAX, DWORD PTR DS:[EDX+ECX*8]                    ; 8=stCD_EntityAttriInfoHolder的结构体大小
	$+49     00462079     03C9                         ADD ECX, ECX
	$+4B     0046207B     8B1C8A                       MOV EBX, DWORD PTR DS:[EDX+ECX*4]
	$+4E     0046207E     8D148A                       LEA EDX, DWORD PTR DS:[EDX+ECX*4]
	$+51     00462081     3BDE                         CMP EBX, ESI
	$+53     00462083     75 04                        JNZ SHORT PathOfEx.00462089
	$+55     00462085     8BCE                         MOV ECX, ESI
	$+57     00462087     EB 05                        JMP SHORT PathOfEx.0046208E
	$+59     00462089     8B4A 04                      MOV ECX, DWORD PTR DS:[EDX+0x4]
	$+5C     0046208C     8B09                         MOV ECX, DWORD PTR DS:[ECX]
	$+5E     0046208E     3BC1                         CMP EAX, ECX
	$+60     00462090     74 21                        JE SHORT PathOfEx.004620B3
	$+62     00462092     8B48 08                      MOV ECX, DWORD PTR DS:[EAX+0x8]
	$+65     00462095     3B0F                         CMP ECX, DWORD PTR DS:[EDI]
	$+67     00462097     74 04                        JE SHORT PathOfEx.0046209D
	$+69     00462099     8B00                         MOV EAX, DWORD PTR DS:[EAX]
	$+6B     0046209B   ^ EB E4                        JMP SHORT PathOfEx.00462081
	$+6D     0046209D     8B0F                         MOV ECX, DWORD PTR DS:[EDI]
	$+6F     0046209F     3B48 08                      CMP ECX, DWORD PTR DS:[EAX+0x8]
	$+72     004620A2     8B4C24 10                    MOV ECX, DWORD PTR SS:[ESP+0x10]                     ; 执行后ecx=第一个参数的值，并返回，而结果就保存在此值表示的地址中
	$+76     004620A6     0F45C6                       CMOVNE EAX, ESI
	$+79     004620A9     5F                           POP EDI                                              ; 0018EC58
	$+7A     004620AA     5E                           POP ESI                                              ; 0018EC58
	$+7B     004620AB     8901                         MOV DWORD PTR DS:[ECX], EAX                          ; 结果保存在第一个参数中
	$+7D     004620AD     8BC1                         MOV EAX, ECX
	$+7F     004620AF     5B                           POP EBX                                              ; 0018EC58
	$+80     004620B0     C2 0800                      RETN 0x8
	$+83     004620B3     8B4424 10                    MOV EAX, DWORD PTR SS:[ESP+0x10]
	$+87     004620B7     5F                           POP EDI                                              ; 0018EC58
	$+88     004620B8     8930                         MOV DWORD PTR DS:[EAX], ESI
	$+8A     004620BA     5E                           POP ESI                                              ; 0018EC58
	$+8B     004620BB     5B                           POP EBX                                              ; 0018EC58
	$+8C     004620BC     C2 0800                      RETN 0x8
$+5      007E197C     3975 E0                      CMP DWORD PTR SS:[EBP-0x20], ESI
$+8      007E197F     74 0A                        JE SHORT PathOfEx.007E198B
$+A      007E1981     F3:0F1005 545AF300           MOVSS XMM0, DWORD PTR DS:[0xF35A54]
$+12     007E1989     EB 08                        JMP SHORT PathOfEx.007E1993
$+14     007E198B     F3:0F1005 B45AF300           MOVSS XMM0, DWORD PTR DS:[0xF35AB4]
$+1C     007E1993     F3:0F1183 A4000000           MOVSS DWORD PTR DS:[EBX+0xA4], XMM0
$+24     007E199B     8B43 24                      MOV EAX, DWORD PTR DS:[EBX+0x24]						; g_so_actor_get_actor_info_
$+27     007E199E     8B53 04                      MOV EDX, DWORD PTR DS:[EBX+0x4]                      ; 执行后edx==实体对象,==stCD_EntityMgr*
$+2A     007E19A1     8B7B 30                      MOV EDI, DWORD PTR DS:[EBX+0x30]
$+2D     007E19A4     8B48 50                      MOV ECX, DWORD PTR DS:[EAX+0x50]                     ; ecx==2,==kComponent_Pathfinding
$+30     007E19A7     8B42 04                      MOV EAX, DWORD PTR DS:[EDX+0x4]
$+33     007E19AA     8B77 1C                      MOV ESI, DWORD PTR DS:[EDI+0x1C]
$+36     007E19AD     8975 DC                      MOV DWORD PTR SS:[EBP-0x24], ESI
$+39     007E19B0     8B0C88                       MOV ECX, DWORD PTR DS:[EAX+ECX*4]
$+3C     007E19B3     894D E4                      MOV DWORD PTR SS:[EBP-0x1C], ECX                     ; ecx==pathfinding*
$+3F     007E19B6     85F6                         TEST ESI, ESI
$+41     007E19B8     74 09                        JE SHORT PathOfEx.007E19C3
$+43     007E19BA     8B46 14                      MOV EAX, DWORD PTR DS:[ESI+0x14]
$+46     007E19BD     8983 94000000                MOV DWORD PTR DS:[EBX+0x94], EAX
$+4C     007E19C3     F647 17 01                   TEST BYTE PTR DS:[EDI+0x17], 0x1
$+50     007E19C7     74 40                        JE SHORT PathOfEx.007E1A09
$+52     007E19C9     814B 70 00040000             OR DWORD PTR DS:[EBX+0x70], 0x400
$+59     007E19D0     8D71 28                      LEA ESI, DWORD PTR DS:[ECX+0x28]
$+5C     007E19D3     8B52 20                      MOV EDX, DWORD PTR DS:[EDX+0x20]
$+5F     007E19D6     6A 01                        PUSH 0x1
$+61     007E19D8     6A FF                        PUSH -0x1
$+63     007E19DA     6A 01                        PUSH 0x1
$+65     007E19DC     8B42 10                      MOV EAX, DWORD PTR DS:[EDX+0x10]
$+68     007E19DF     6A 00                        PUSH 0x0
$+6A     007E19E1     6A 00                        PUSH 0x0
$+6C     007E19E3     8906                         MOV DWORD PTR DS:[ESI], EAX
$+6E     007E19E5     8B42 14                      MOV EAX, DWORD PTR DS:[EDX+0x14]
$+71     007E19E8     56                           PUSH ESI
$+72     007E19E9     8946 04                      MOV DWORD PTR DS:[ESI+0x4], EAX
$+75     007E19EC     E8 AFC10100                  CALL PathOfEx.007FDBA0
$+7A     007E19F1     8163 70 FFFBFFFF             AND DWORD PTR DS:[EBX+0x70], -0x401
$+81     007E19F8     8B4D F4                      MOV ECX, DWORD PTR SS:[EBP-0xC]
$+84     007E19FB     64:890D 00000000             MOV DWORD PTR FS:[0], ECX
$+8B     007E1A02     5F                           POP EDI
$+8C     007E1A03     5E                           POP ESI
$+8D     007E1A04     5B                           POP EBX
$+8E     007E1A05     8BE5                         MOV ESP, EBP
$+90     007E1A07     5D                           POP EBP
$+91     007E1A08     C3                           RETN
$+92     007E1A09     814B 70 00040000             OR DWORD PTR DS:[EBX+0x70], 0x400
$+99     007E1A10     85F6                         TEST ESI, ESI
$+9B     007E1A12     0F84 87010000                JE PathOfEx.007E1B9F
$+A1     007E1A18     8B7F 08                      MOV EDI, DWORD PTR DS:[EDI+0x8]
$+A4     007E1A1B     83C7 04                      ADD EDI, 0x4
$+A7     007E1A1E     897D E0                      MOV DWORD PTR SS:[EBP-0x20], EDI
$+AA     007E1A21     837F 04 00                   CMP DWORD PTR DS:[EDI+0x4], 0x0
$+AE     007E1A25     74 6C                        JE SHORT PathOfEx.007E1A93
$+B0     007E1A27     8D45 C0                      LEA EAX, DWORD PTR SS:[EBP-0x40]
$+B3     007E1A2A     8BCF                         MOV ECX, EDI
$+B5     007E1A2C     50                           PUSH EAX
$+B6     007E1A2D     E8 BEBACDFF                  CALL PathOfEx.004BD4F0
$+BB     007E1A32     8BF0                         MOV ESI, EAX
$+BD     007E1A34     8BCF                         MOV ECX, EDI
$+BF     007E1A36     8D45 C8                      LEA EAX, DWORD PTR SS:[EBP-0x38]
$+C2     007E1A39     50                           PUSH EAX
$+C3     007E1A3A     E8 B1BACDFF                  CALL PathOfEx.004BD4F0
$+C8     007E1A3F     8B4D E0                      MOV ECX, DWORD PTR SS:[EBP-0x20]                     ; PathOfEx.00D38680
$+CB     007E1A42     8BF8                         MOV EDI, EAX
$+CD     007E1A44     8D45 D0                      LEA EAX, DWORD PTR SS:[EBP-0x30]
$+D0     007E1A47     50                           PUSH EAX
$+D1     007E1A48     E8 A3BACDFF                  CALL PathOfEx.004BD4F0
$+D6     007E1A4D     8B4F 04                      MOV ECX, DWORD PTR DS:[EDI+0x4]
$+D9     007E1A50     8B76 04                      MOV ESI, DWORD PTR DS:[ESI+0x4]
$+DC     007E1A53     8945 E0                      MOV DWORD PTR SS:[EBP-0x20], EAX
$+DF     007E1A56     B8 07000000                  MOV EAX, 0x7
$+E4     007E1A5B     8945 EC                      MOV DWORD PTR SS:[EBP-0x14], EAX
$+E7     007E1A5E     8B79 18                      MOV EDI, DWORD PTR DS:[ECX+0x18]
$+EA     007E1A61     8B56 14                      MOV EDX, DWORD PTR DS:[ESI+0x14]
$+ED     007E1A64     8B4E 18                      MOV ECX, DWORD PTR DS:[ESI+0x18]
$+F0     007E1A67     8D0C91                       LEA ECX, DWORD PTR DS:[ECX+EDX*4]
$+F3     007E1A6A     3BF9                         CMP EDI, ECX
$+F5     007E1A6C     74 0E                        JE SHORT PathOfEx.007E1A7C
$+F7     007E1A6E     66:90                        NOP
$+F9     007E1A70     833F 08                      CMP DWORD PTR DS:[EDI], 0x8
$+FC     007E1A73     74 07                        JE SHORT PathOfEx.007E1A7C
$+FE     007E1A75     83C7 04                      ADD EDI, 0x4
$+101    007E1A78     3BF9                         CMP EDI, ECX
$+103    007E1A7A   ^ 75 F4                        JNZ SHORT PathOfEx.007E1A70
$+105    007E1A7C     8B45 E0                      MOV EAX, DWORD PTR SS:[EBP-0x20]                     ; PathOfEx.00721B7F
$+108    007E1A7F     C645 F3 01                   MOV BYTE PTR SS:[EBP-0xD], 0x1
$+10C    007E1A83     8B40 04                      MOV EAX, DWORD PTR DS:[EAX+0x4]
$+10F    007E1A86     8B48 14                      MOV ECX, DWORD PTR DS:[EAX+0x14]
$+112    007E1A89     8B40 18                      MOV EAX, DWORD PTR DS:[EAX+0x18]
$+115    007E1A8C     8D0488                       LEA EAX, DWORD PTR DS:[EAX+ECX*4]
$+118    007E1A8F     3BF8                         CMP EDI, EAX
$+11A    007E1A91     75 04                        JNZ SHORT PathOfEx.007E1A97
$+11C    007E1A93     C645 F3 00                   MOV BYTE PTR SS:[EBP-0xD], 0x0
$+120    007E1A97     8B45 EC                      MOV EAX, DWORD PTR SS:[EBP-0x14]                     ; ntdll_1z.RtlLeaveCriticalSection
$+123    007E1A9A     A8 04                        TEST AL, 0x4
$+125    007E1A9C     74 36                        JE SHORT PathOfEx.007E1AD4
$+127    007E1A9E     83E0 FB                      AND EAX, -0x5
$+12A    007E1AA1     8945 EC                      MOV DWORD PTR SS:[EBP-0x14], EAX
$+12D    007E1AA4     C745 FC 00000000             MOV DWORD PTR SS:[EBP-0x4], 0x0
$+134    007E1AAB     8B4D D0                      MOV ECX, DWORD PTR SS:[EBP-0x30]
$+137    007E1AAE     85C9                         TEST ECX, ECX
$+139    007E1AB0     74 1B                        JE SHORT PathOfEx.007E1ACD
$+13B    007E1AB2     51                           PUSH ECX
$+13C    007E1AB3     51                           PUSH ECX
$+13D    007E1AB4     E8 57DDF2FF                  CALL PathOfEx.0070F810
$+142    007E1AB9     83C4 04                      ADD ESP, 0x4
$+145    007E1ABC     8BC8                         MOV ECX, EAX
$+147    007E1ABE     E8 DDD8F2FF                  CALL PathOfEx.0070F3A0
$+14C    007E1AC3     8B45 EC                      MOV EAX, DWORD PTR SS:[EBP-0x14]                     ; ntdll_1z.RtlLeaveCriticalSection
$+14F    007E1AC6     C745 D0 00000000             MOV DWORD PTR SS:[EBP-0x30], 0x0
$+156    007E1ACD     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$+15D    007E1AD4     A8 02                        TEST AL, 0x2
$+15F    007E1AD6     74 36                        JE SHORT PathOfEx.007E1B0E
$+161    007E1AD8     83E0 FD                      AND EAX, -0x3
$+164    007E1ADB     8945 EC                      MOV DWORD PTR SS:[EBP-0x14], EAX
$+167    007E1ADE     C745 FC 01000000             MOV DWORD PTR SS:[EBP-0x4], 0x1
$+16E    007E1AE5     8B4D C8                      MOV ECX, DWORD PTR SS:[EBP-0x38]
$+171    007E1AE8     85C9                         TEST ECX, ECX
$+173    007E1AEA     74 1B                        JE SHORT PathOfEx.007E1B07
$+175    007E1AEC     51                           PUSH ECX
$+176    007E1AED     51                           PUSH ECX
$+177    007E1AEE     E8 1DDDF2FF                  CALL PathOfEx.0070F810
$+17C    007E1AF3     83C4 04                      ADD ESP, 0x4
$+17F    007E1AF6     8BC8                         MOV ECX, EAX
$+181    007E1AF8     E8 A3D8F2FF                  CALL PathOfEx.0070F3A0
$+186    007E1AFD     8B45 EC                      MOV EAX, DWORD PTR SS:[EBP-0x14]                     ; ntdll_1z.RtlLeaveCriticalSection
$+189    007E1B00     C745 C8 00000000             MOV DWORD PTR SS:[EBP-0x38], 0x0
$+190    007E1B07     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$+197    007E1B0E     A8 01                        TEST AL, 0x1
$+199    007E1B10     74 26                        JE SHORT PathOfEx.007E1B38
$+19B    007E1B12     C745 FC 02000000             MOV DWORD PTR SS:[EBP-0x4], 0x2
$+1A2    007E1B19     8B45 C0                      MOV EAX, DWORD PTR SS:[EBP-0x40]
$+1A5    007E1B1C     85C0                         TEST EAX, EAX
$+1A7    007E1B1E     74 11                        JE SHORT PathOfEx.007E1B31
$+1A9    007E1B20     50                           PUSH EAX
$+1AA    007E1B21     51                           PUSH ECX
$+1AB    007E1B22     E8 E9DCF2FF                  CALL PathOfEx.0070F810
$+1B0    007E1B27     83C4 04                      ADD ESP, 0x4
$+1B3    007E1B2A     8BC8                         MOV ECX, EAX
$+1B5    007E1B2C     E8 6FD8F2FF                  CALL PathOfEx.0070F3A0
$+1BA    007E1B31     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4], -0x1
$+1C1    007E1B38     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD], 0x0
$+1C5    007E1B3C     8B45 DC                      MOV EAX, DWORD PTR SS:[EBP-0x24]
$+1C8    007E1B3F     8B40 20                      MOV EAX, DWORD PTR DS:[EAX+0x20]
$+1CB    007E1B42     8945 DC                      MOV DWORD PTR SS:[EBP-0x24], EAX
$+1CE    007E1B45     74 09                        JE SHORT PathOfEx.007E1B50
$+1D0    007E1B47     C745 EC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x14], -0x1
$+1D7    007E1B4E     EB 06                        JMP SHORT PathOfEx.007E1B56
$+1D9    007E1B50     8B40 28                      MOV EAX, DWORD PTR DS:[EAX+0x28]
$+1DC    007E1B53     8945 EC                      MOV DWORD PTR SS:[EBP-0x14], EAX
$+1DF    007E1B56     8B4B 30                      MOV ECX, DWORD PTR DS:[EBX+0x30]
$+1E2    007E1B59     8B01                         MOV EAX, DWORD PTR DS:[ECX]
$+1E4    007E1B5B     FF50 28                      CALL NEAR DWORD PTR DS:[EAX+0x28]
$+1E7    007E1B5E     8B4B 30                      MOV ECX, DWORD PTR DS:[EBX+0x30]
$+1EA    007E1B61     8BF8                         MOV EDI, EAX
$+1EC    007E1B63     8B11                         MOV EDX, DWORD PTR DS:[ECX]
$+1EE    007E1B65     FF52 24                      CALL NEAR DWORD PTR DS:[EDX+0x24]
$+1F1    007E1B68     8B75 E8                      MOV ESI, DWORD PTR SS:[EBP-0x18]
$+1F4    007E1B6B     8BD8                         MOV EBX, EAX
$+1F6    007E1B6D     8B76 30                      MOV ESI, DWORD PTR DS:[ESI+0x30]
$+1F9    007E1B70     8BCE                         MOV ECX, ESI
$+1FB    007E1B72     8B16                         MOV EDX, DWORD PTR DS:[ESI]
$+1FD    007E1B74     FF52 30                      CALL NEAR DWORD PTR DS:[EDX+0x30]
$+200    007E1B77     8B16                         MOV EDX, DWORD PTR DS:[ESI]
$+202    007E1B79     8BCE                         MOV ECX, ESI
$+204    007E1B7B     50                           PUSH EAX
$+205    007E1B7C     FF75 EC                      PUSH DWORD PTR SS:[EBP-0x14]                         ; ntdll_1z.RtlLeaveCriticalSection
$+208    007E1B7F     FF52 2C                      CALL NEAR DWORD PTR DS:[EDX+0x2C]
$+20B    007E1B82     B9 01000000                  MOV ECX, 0x1
$+210    007E1B87     3BF9                         CMP EDI, ECX
$+212    007E1B89     50                           PUSH EAX
$+213    007E1B8A     8B45 DC                      MOV EAX, DWORD PTR SS:[EBP-0x24]
$+216    007E1B8D     0F42F9                       CMOVB EDI, ECX
$+219    007E1B90     4F                           DEC EDI
$+21A    007E1B91     3BD9                         CMP EBX, ECX
$+21C    007E1B93     57                           PUSH EDI
$+21D    007E1B94     0F42D9                       CMOVB EBX, ECX
$+220    007E1B97     4B                           DEC EBX
$+221    007E1B98     83C0 10                      ADD EAX, 0x10
$+224    007E1B9B     53                           PUSH EBX
$+225    007E1B9C     50                           PUSH EAX
$+226    007E1B9D     EB 5F                        JMP SHORT PathOfEx.007E1BFE
$+228    007E1B9F     8BCF                         MOV ECX, EDI
$+22A    007E1BA1     8B01                         MOV EAX, DWORD PTR DS:[ECX]
$+22C    007E1BA3     FF50 28                      CALL NEAR DWORD PTR DS:[EAX+0x28]
$+22F    007E1BA6     8B4B 30                      MOV ECX, DWORD PTR DS:[EBX+0x30]
$+232    007E1BA9     8BF8                         MOV EDI, EAX
$+234    007E1BAB     8B11                         MOV EDX, DWORD PTR DS:[ECX]
$+236    007E1BAD     FF52 24                      CALL NEAR DWORD PTR DS:[EDX+0x24]
$+239    007E1BB0     8B75 E8                      MOV ESI, DWORD PTR SS:[EBP-0x18]
$+23C    007E1BB3     8B5B 30                      MOV EBX, DWORD PTR DS:[EBX+0x30]
$+23F    007E1BB6     8945 DC                      MOV DWORD PTR SS:[EBP-0x24], EAX
$+242    007E1BB9     83C3 20                      ADD EBX, 0x20
$+245    007E1BBC     8B76 30                      MOV ESI, DWORD PTR DS:[ESI+0x30]
$+248    007E1BBF     8BCE                         MOV ECX, ESI
$+24A    007E1BC1     8B16                         MOV EDX, DWORD PTR DS:[ESI]
$+24C    007E1BC3     FF52 30                      CALL NEAR DWORD PTR DS:[EDX+0x30]
$+24F    007E1BC6     50                           PUSH EAX
$+250    007E1BC7     8B06                         MOV EAX, DWORD PTR DS:[ESI]
$+252    007E1BC9     8BCE                         MOV ECX, ESI
$+254    007E1BCB     8B40 34                      MOV EAX, DWORD PTR DS:[EAX+0x34]
$+257    007E1BCE     FFD0                         CALL NEAR EAX
$+259    007E1BD0     33D2                         XOR EDX, EDX
$+25B    007E1BD2     8BCE                         MOV ECX, ESI
$+25D    007E1BD4     84C0                         TEST AL, AL
$+25F    007E1BD6     8B06                         MOV EAX, DWORD PTR DS:[ESI]
$+261    007E1BD8     0F95C2                       SETNE DL
$+264    007E1BDB     8D1455 FFFFFFFF              LEA EDX, DWORD PTR DS:[EDX*2-0x1]
$+26B    007E1BE2     52                           PUSH EDX
$+26C    007E1BE3     FF50 2C                      CALL NEAR DWORD PTR DS:[EAX+0x2C]
$+26F    007E1BE6     B9 01000000                  MOV ECX, 0x1
$+274    007E1BEB     3BF9                         CMP EDI, ECX
$+276    007E1BED     50                           PUSH EAX
$+277    007E1BEE     8B45 DC                      MOV EAX, DWORD PTR SS:[EBP-0x24]
$+27A    007E1BF1     0F42F9                       CMOVB EDI, ECX
$+27D    007E1BF4     4F                           DEC EDI
$+27E    007E1BF5     3BC1                         CMP EAX, ECX
$+280    007E1BF7     57                           PUSH EDI
$+281    007E1BF8     0F42C1                       CMOVB EAX, ECX
$+284    007E1BFB     48                           DEC EAX
$+285    007E1BFC     50                           PUSH EAX
$+286    007E1BFD     53                           PUSH EBX
$+287    007E1BFE     8B4D E4                      MOV ECX, DWORD PTR SS:[EBP-0x1C]
$+28A    007E1C01     E8 9ABF0100                  CALL PathOfEx.007FDBA0                               ; call自动寻路函数
$+28F    007E1C06     8B5D E8                      MOV EBX, DWORD PTR SS:[EBP-0x18]
$+292    007E1C09     8163 70 FFFBFFFF             AND DWORD PTR DS:[EBX+0x70], -0x401
$+299    007E1C10     84C0                         TEST AL, AL
$+29B    007E1C12     75 1C                        JNZ SHORT PathOfEx.007E1C30
$+29D    007E1C14     8B73 58                      MOV ESI, DWORD PTR DS:[EBX+0x58]
$+2A0    007E1C17     3B73 5C                      CMP ESI, DWORD PTR DS:[EBX+0x5C]
$+2A3    007E1C1A     74 14                        JE SHORT PathOfEx.007E1C30
$+2A5    007E1C1C     8B7D D8                      MOV EDI, DWORD PTR SS:[EBP-0x28]
$+2A8    007E1C1F     90                           NOP
$+2A9    007E1C20     8B0E                         MOV ECX, DWORD PTR DS:[ESI]
$+2AB    007E1C22     57                           PUSH EDI
$+2AC    007E1C23     8B01                         MOV EAX, DWORD PTR DS:[ECX]
$+2AE    007E1C25     FF50 14                      CALL NEAR DWORD PTR DS:[EAX+0x14]
$+2B1    007E1C28     83C6 04                      ADD ESI, 0x4
$+2B4    007E1C2B     3B73 5C                      CMP ESI, DWORD PTR DS:[EBX+0x5C]
$+2B7    007E1C2E   ^ 75 F0                        JNZ SHORT PathOfEx.007E1C20
$+2B9    007E1C30     8B4D F4                      MOV ECX, DWORD PTR SS:[EBP-0xC]
$+2BC    007E1C33     5F                           POP EDI                                              ; 0018EC38
$+2BD    007E1C34     5E                           POP ESI                                              ; 0018EC38
$+2BE    007E1C35     64:890D 00000000             MOV DWORD PTR FS:[0], ECX
$+2C5    007E1C3C     5B                           POP EBX                                              ; 0018EC38
$+2C6    007E1C3D     8BE5                         MOV ESP, EBP
$+2C8    007E1C3F     5D                           POP EBP                                              ; 0018EC38
$+2C9    007E1C40     C3                           RETN
*/
/**/