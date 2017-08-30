#pragma once
/*
@author zhp
@date 2017/7/3 0:02
@purpose for anti check
*/

//call tag_获得机器码_cpuid
/*
$-20B    0086C39F    CC                        INT3
$-20A    0086C3A0    55                        PUSH EBP
$-209    0086C3A1    8BEC                      MOV EBP,ESP
$-207    0086C3A3    6A FF                     PUSH -0x1
$-205    0086C3A5    68 B68FC300               PUSH PathOfEx.00C38FB6
$-200    0086C3AA    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-1FA    0086C3B0    50                        PUSH EAX
$-1F9    0086C3B1    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-1F2    0086C3B8    83EC 08                   SUB ESP,0x8
$-1EF    0086C3BB    56                        PUSH ESI
$-1EE    0086C3BC    6A 0C                     PUSH 0xC
$-1EC    0086C3BE    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$-1E9    0086C3C1    B9 FC470901               MOV ECX,PathOfEx.010947FC
$-1E4    0086C3C6    50                        PUSH EAX
$-1E3    0086C3C7    E8 E4DCF3FF               CALL PathOfEx.007AA0B0
$-1DE    0086C3CC    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-1D7    0086C3D3    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
$-1D4    0086C3D6    8B30                      MOV ESI,DWORD PTR DS:[EAX]
$-1D2    0086C3D8    83FA 20                   CMP EDX,0x20
$-1CF    0086C3DB    72 0A                     JB SHORT PathOfEx.0086C3E7
$-1CD    0086C3DD    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
$-1C8    0086C3E2    E8 26323100               CALL PathOfEx.00B7F60D
$-1C3    0086C3E7    8BCA                      MOV ECX,EDX
$-1C1    0086C3E9    B8 01000000               MOV EAX,0x1
$-1BC    0086C3EE    83E1 1F                   AND ECX,0x1F
$-1B9    0086C3F1    C1EA 05                   SHR EDX,0x5
$-1B6    0086C3F4    D3E0                      SHL EAX,CL
$-1B4    0086C3F6    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1B1    0086C3F9    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
$-1AE    0086C3FC    5E                        POP ESI
$-1AD    0086C3FD    0F95C0                    SETNE AL
$-1AA    0086C400    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-1A3    0086C407    8BE5                      MOV ESP,EBP
$-1A1    0086C409    5D                        POP EBP
$-1A0    0086C40A    C3                        RETN
$-19F    0086C40B    CC                        INT3
$-19E    0086C40C    CC                        INT3
$-19D    0086C40D    CC                        INT3
$-19C    0086C40E    CC                        INT3
$-19B    0086C40F    CC                        INT3
$-19A    0086C410    55                        PUSH EBP
$-199    0086C411    8BEC                      MOV EBP,ESP
$-197    0086C413    6A FF                     PUSH -0x1
$-195    0086C415    68 B68FC300               PUSH PathOfEx.00C38FB6
$-190    0086C41A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-18A    0086C420    50                        PUSH EAX
$-189    0086C421    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-182    0086C428    83EC 08                   SUB ESP,0x8
$-17F    0086C42B    56                        PUSH ESI
$-17E    0086C42C    6A 1B                     PUSH 0x1B
$-17C    0086C42E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$-179    0086C431    B9 FC470901               MOV ECX,PathOfEx.010947FC
$-174    0086C436    50                        PUSH EAX
$-173    0086C437    E8 74DCF3FF               CALL PathOfEx.007AA0B0
$-16E    0086C43C    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-167    0086C443    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
$-164    0086C446    8B30                      MOV ESI,DWORD PTR DS:[EAX]
$-162    0086C448    83FA 20                   CMP EDX,0x20
$-15F    0086C44B    72 0A                     JB SHORT PathOfEx.0086C457
$-15D    0086C44D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
$-158    0086C452    E8 B6313100               CALL PathOfEx.00B7F60D
$-153    0086C457    8BCA                      MOV ECX,EDX
$-151    0086C459    B8 01000000               MOV EAX,0x1
$-14C    0086C45E    83E1 1F                   AND ECX,0x1F
$-149    0086C461    C1EA 05                   SHR EDX,0x5
$-146    0086C464    D3E0                      SHL EAX,CL
$-144    0086C466    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-141    0086C469    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
$-13E    0086C46C    5E                        POP ESI
$-13D    0086C46D    0F95C0                    SETNE AL
$-13A    0086C470    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-133    0086C477    8BE5                      MOV ESP,EBP
$-131    0086C479    5D                        POP EBP
$-130    0086C47A    C3                        RETN
$-12F    0086C47B    CC                        INT3
$-12E    0086C47C    CC                        INT3
$-12D    0086C47D    CC                        INT3
$-12C    0086C47E    CC                        INT3
$-12B    0086C47F    CC                        INT3
$-12A    0086C480    55                        PUSH EBP
$-129    0086C481    8BEC                      MOV EBP,ESP
$-127    0086C483    6A FF                     PUSH -0x1
$-125    0086C485    68 B68FC300               PUSH PathOfEx.00C38FB6
$-120    0086C48A    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-11A    0086C490    50                        PUSH EAX
$-119    0086C491    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-112    0086C498    83EC 08                   SUB ESP,0x8
$-10F    0086C49B    56                        PUSH ESI
$-10E    0086C49C    6A 1D                     PUSH 0x1D
$-10C    0086C49E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$-109    0086C4A1    B9 FC470901               MOV ECX,PathOfEx.010947FC
$-104    0086C4A6    50                        PUSH EAX
$-103    0086C4A7    E8 04DCF3FF               CALL PathOfEx.007AA0B0
$-FE     0086C4AC    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-F7     0086C4B3    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
$-F4     0086C4B6    8B30                      MOV ESI,DWORD PTR DS:[EAX]
$-F2     0086C4B8    83FA 20                   CMP EDX,0x20
$-EF     0086C4BB    72 0A                     JB SHORT PathOfEx.0086C4C7
$-ED     0086C4BD    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
$-E8     0086C4C2    E8 46313100               CALL PathOfEx.00B7F60D
$-E3     0086C4C7    8BCA                      MOV ECX,EDX
$-E1     0086C4C9    B8 01000000               MOV EAX,0x1
$-DC     0086C4CE    83E1 1F                   AND ECX,0x1F
$-D9     0086C4D1    C1EA 05                   SHR EDX,0x5
$-D6     0086C4D4    D3E0                      SHL EAX,CL
$-D4     0086C4D6    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-D1     0086C4D9    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
$-CE     0086C4DC    5E                        POP ESI
$-CD     0086C4DD    0F95C0                    SETNE AL
$-CA     0086C4E0    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-C3     0086C4E7    8BE5                      MOV ESP,EBP
$-C1     0086C4E9    5D                        POP EBP
$-C0     0086C4EA    C3                        RETN
$-BF     0086C4EB    CC                        INT3
$-BE     0086C4EC    CC                        INT3
$-BD     0086C4ED    CC                        INT3
$-BC     0086C4EE    CC                        INT3
$-BB     0086C4EF    CC                        INT3
$-BA     0086C4F0    55                        PUSH EBP
$-B9     0086C4F1    8BEC                      MOV EBP,ESP
$-B7     0086C4F3    6A FF                     PUSH -0x1
$-B5     0086C4F5    68 B68FC300               PUSH PathOfEx.00C38FB6
$-B0     0086C4FA    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-AA     0086C500    50                        PUSH EAX
$-A9     0086C501    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-A2     0086C508    83EC 08                   SUB ESP,0x8
$-9F     0086C50B    56                        PUSH ESI
$-9E     0086C50C    6A 05                     PUSH 0x5
$-9C     0086C50E    8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$-99     0086C511    B9 04480901               MOV ECX,PathOfEx.01094804
$-94     0086C516    50                        PUSH EAX
$-93     0086C517    E8 94DBF3FF               CALL PathOfEx.007AA0B0
$-8E     0086C51C    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-87     0086C523    8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
$-84     0086C526    8B30                      MOV ESI,DWORD PTR DS:[EAX]
$-82     0086C528    83FA 20                   CMP EDX,0x20
$-7F     0086C52B    72 0A                     JB SHORT PathOfEx.0086C537
$-7D     0086C52D    68 EC3BD900               PUSH PathOfEx.00D93BEC                          ; ASCII 69,"nvalid bitset<N> position"
$-78     0086C532    E8 D6303100               CALL PathOfEx.00B7F60D
$-73     0086C537    8BCA                      MOV ECX,EDX
$-71     0086C539    B8 01000000               MOV EAX,0x1
$-6C     0086C53E    83E1 1F                   AND ECX,0x1F
$-69     0086C541    C1EA 05                   SHR EDX,0x5
$-66     0086C544    D3E0                      SHL EAX,CL
$-64     0086C546    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-61     0086C549    850496                    TEST DWORD PTR DS:[ESI+EDX*4],EAX
$-5E     0086C54C    5E                        POP ESI
$-5D     0086C54D    0F95C0                    SETNE AL
$-5A     0086C550    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-53     0086C557    8BE5                      MOV ESP,EBP
$-51     0086C559    5D                        POP EBP
$-50     0086C55A    C3                        RETN
$-4F     0086C55B    CC                        INT3
$-4E     0086C55C    CC                        INT3
$-4D     0086C55D    CC                        INT3
$-4C     0086C55E    CC                        INT3
$-4B     0086C55F    CC                        INT3
$-4A     0086C560    55                        PUSH EBP
$-49     0086C561    8BEC                      MOV EBP,ESP
$-47     0086C563    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-41     0086C569    6A FF                     PUSH -0x1
$-3F     0086C56B    68 7C48C800               PUSH PathOfEx.00C8487C
$-3A     0086C570    50                        PUSH EAX
$-39     0086C571    64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-32     0086C578    64:A1 2C000000            MOV EAX,DWORD PTR FS:[0x2C]
$-2C     0086C57E    8B08                      MOV ECX,DWORD PTR DS:[EAX]
$-2A     0086C580    A1 B8470901               MOV EAX,DWORD PTR DS:[0x10947B8]
$-25     0086C585    3B81 28000000             CMP EAX,DWORD PTR DS:[ECX+0x28]
$-1F     0086C58B    7E 39                     JLE SHORT PathOfEx.0086C5C6
$-1D     0086C58D    68 B8470901               PUSH PathOfEx.010947B8
$-18     0086C592    E8 D2543200               CALL PathOfEx.00B91A69
$-13     0086C597    83C4 04                   ADD ESP,0x4
$-10     0086C59A    833D B8470901 FF          CMP DWORD PTR DS:[0x10947B8],-0x1
$-9      0086C5A1    75 23                     JNZ SHORT PathOfEx.0086C5C6
$-7      0086C5A3    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$ ==>    0086C5AA    E8 81F7FFFF               CALL PathOfEx.0086BD30                          ; call tag_获得机器码_cpuid
$+5      0086C5AF    68 5049CA00               PUSH PathOfEx.00CA4950
$+A      0086C5B4    E8 09533200               CALL PathOfEx.00B918C2
$+F      0086C5B9    68 B8470901               PUSH PathOfEx.010947B8
$+14     0086C5BE    E8 67543200               CALL PathOfEx.00B91A2A
$+19     0086C5C3    83C4 08                   ADD ESP,0x8
$+1C     0086C5C6    A1 2C480901               MOV EAX,DWORD PTR DS:[0x109482C]
$+21     0086C5CB    8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+24     0086C5CE    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+2B     0086C5D5    8BE5                      MOV ESP,EBP
$+2D     0086C5D7    5D                        POP EBP
$+2E     0086C5D8    C3                        RETN
$+2F     0086C5D9    CC                        INT3
$+30     0086C5DA    CC                        INT3
$+31     0086C5DB    CC                        INT3
$+32     0086C5DC    CC                        INT3
$+33     0086C5DD    CC                        INT3
$+34     0086C5DE    CC                        INT3
$+35     0086C5DF    CC                        INT3
$+36     0086C5E0    8B0D 20480901             MOV ECX,DWORD PTR DS:[0x1094820]
$+3C     0086C5E6    85C9                      TEST ECX,ECX
$+3E     0086C5E8    74 34                     JE SHORT PathOfEx.0086C61E
$+40     0086C5EA    A1 28480901               MOV EAX,DWORD PTR DS:[0x1094828]
$+45     0086C5EF    2BC1                      SUB EAX,ECX
$+47     0086C5F1    6A 10                     PUSH 0x10
$+49     0086C5F3    C1F8 04                   SAR EAX,0x4
$+4C     0086C5F6    50                        PUSH EAX
$+4D     0086C5F7    51                        PUSH ECX
$+4E     0086C5F8    E8 B3CAADFF               CALL PathOfEx.003490B0
$+53     0086C5FD    83C4 0C                   ADD ESP,0xC
$+56     0086C600    C705 20480901 00000000    MOV DWORD PTR DS:[0x1094820],0x0
$+60     0086C60A    C705 24480901 00000000    MOV DWORD PTR DS:[0x1094824],0x0
$+6A     0086C614    C705 28480901 00000000    MOV DWORD PTR DS:[0x1094828],0x0
$+74     0086C61E    8B0D 14480901             MOV ECX,DWORD PTR DS:[0x1094814]
$+7A     0086C624    85C9                      TEST ECX,ECX
$+7C     0086C626    74 34                     JE SHORT PathOfEx.0086C65C
$+7E     0086C628    A1 1C480901               MOV EAX,DWORD PTR DS:[0x109481C]
$+83     0086C62D    2BC1                      SUB EAX,ECX
$+85     0086C62F    6A 10                     PUSH 0x10
$+87     0086C631    C1F8 04                   SAR EAX,0x4
$+8A     0086C634    50                        PUSH EAX
$+8B     0086C635    51                        PUSH ECX
$+8C     0086C636    E8 75CAADFF               CALL PathOfEx.003490B0
$+91     0086C63B    83C4 0C                   ADD ESP,0xC
$+94     0086C63E    C705 14480901 00000000    MOV DWORD PTR DS:[0x1094814],0x0
$+9E     0086C648    C705 18480901 00000000    MOV DWORD PTR DS:[0x1094818],0x0
$+A8     0086C652    C705 1C480901 00000000    MOV DWORD PTR DS:[0x109481C],0x0
$+B2     0086C65C    A1 F8470901               MOV EAX,DWORD PTR DS:[0x10947F8]
$+B7     0086C661    83F8 10                   CMP EAX,0x10
$+BA     0086C664    72 12                     JB SHORT PathOfEx.0086C678
$+BC     0086C666    6A 01                     PUSH 0x1
$+BE     0086C668    40                        INC EAX
$+BF     0086C669    50                        PUSH EAX
$+C0     0086C66A    FF35 E4470901             PUSH DWORD PTR DS:[0x10947E4]
$+C6     0086C670    E8 3BCAADFF               CALL PathOfEx.003490B0
$+CB     0086C675    83C4 0C                   ADD ESP,0xC
$+CE     0086C678    A1 E0470901               MOV EAX,DWORD PTR DS:[0x10947E0]
$+D3     0086C67D    C705 F8470901 0F000000    MOV DWORD PTR DS:[0x10947F8],0xF
$+DD     0086C687    C705 F4470901 00000000    MOV DWORD PTR DS:[0x10947F4],0x0
$+E7     0086C691    C605 E4470901 00          MOV BYTE PTR DS:[0x10947E4],0x0
$+EE     0086C698    83F8 10                   CMP EAX,0x10
$+F1     0086C69B    72 12                     JB SHORT PathOfEx.0086C6AF
$+F3     0086C69D    6A 01                     PUSH 0x1
$+F5     0086C69F    40                        INC EAX
$+F6     0086C6A0    50                        PUSH EAX
$+F7     0086C6A1    FF35 CC470901             PUSH DWORD PTR DS:[0x10947CC]
$+FD     0086C6A7    E8 04CAADFF               CALL PathOfEx.003490B0
$+102    0086C6AC    83C4 0C                   ADD ESP,0xC
$+105    0086C6AF    C705 E0470901 0F000000    MOV DWORD PTR DS:[0x10947E0],0xF
$+10F    0086C6B9    C705 DC470901 00000000    MOV DWORD PTR DS:[0x10947DC],0x0
$+119    0086C6C3    C605 CC470901 00          MOV BYTE PTR DS:[0x10947CC],0x0
$+120    0086C6CA    C3                        RETN
$+121    0086C6CB    CC                        INT3
$+122    0086C6CC    CC                        INT3
$+123    0086C6CD    CC                        INT3
$+124    0086C6CE    CC                        INT3
$+125    0086C6CF    CC                        INT3
$+126    0086C6D0    CC                        INT3
$+127    0086C6D1    CC                        INT3
$+128    0086C6D2    CC                        INT3
$+129    0086C6D3    CC                        INT3
$+12A    0086C6D4    CC                        INT3
$+12B    0086C6D5    CC                        INT3
$+12C    0086C6D6    CC                        INT3
$+12D    0086C6D7    CC                        INT3
$+12E    0086C6D8    CC                        INT3
$+12F    0086C6D9    CC                        INT3
$+130    0086C6DA    CC                        INT3
$+131    0086C6DB    CC                        INT3
$+132    0086C6DC    CC                        INT3
$+133    0086C6DD    CC                        INT3
$+134    0086C6DE    CC                        INT3
$+135    0086C6DF    CC                        INT3
$+136    0086C6E0    CC                        INT3
$+137    0086C6E1    CC                        INT3
$+138    0086C6E2    CC                        INT3
$+139    0086C6E3    CC                        INT3
$+13A    0086C6E4    CC                        INT3
$+13B    0086C6E5    CC                        INT3
$+13C    0086C6E6    CC                        INT3
$+13D    0086C6E7    CC                        INT3
$+13E    0086C6E8    CC                        INT3
$+13F    0086C6E9    CC                        INT3
$+140    0086C6EA    CC                        INT3
$+141    0086C6EB    CC                        INT3
$+142    0086C6EC    CC                        INT3
$+143    0086C6ED    CC                        INT3
$+144    0086C6EE    CC                        INT3
$+145    0086C6EF    CC                        INT3
$+146    0086C6F0    81F9 01400980             CMP ECX,0x80094001
$+14C    0086C6F6    0F8F 19160000             JG PathOfEx.0086DD15
$+152    0086C6FC    0F84 0D160000             JE PathOfEx.0086DD0F
$+158    0086C702    81F9 B80B0780             CMP ECX,0x80070BB8
$+15E    0086C708    0F8F 4E0D0000             JG PathOfEx.0086D45C
$+164    0086C70E    0F84 F13A0000             JE PathOfEx.00870205
$+16A    0086C714    81F9 FE000780             CMP ECX,0x800700FE
$+170    0086C71A    0F8F 230D0000             JG PathOfEx.0086D443
$+176    0086C720    0F84 9E2B0000             JE PathOfEx.0086F2C4
$+17C    0086C726    81F9 FF000480             CMP ECX,0x800400FF
$+182    0086C72C    0F8F F0070000             JG PathOfEx.0086CF22
$+188    0086C732    0F84 E4070000             JE PathOfEx.0086CF1C
$+18E    0086C738    81F9 00010180             CMP ECX,0x80010100
$+194    0086C73E    0F8F F2010000             JG PathOfEx.0086C936
$+19A    0086C744    0F84 E6010000             JE PathOfEx.0086C930
$+1A0    0086C74A    81F9 01400080             CMP ECX,0x80004001
$+1A6    0086C750    7F 1A                     JG SHORT PathOfEx.0086C76C
$+1A8    0086C752    74 12                     JE SHORT PathOfEx.0086C766
$+1AA    0086C754    81F9 0A000080             CMP ECX,0x8000000A
$+1B0    0086C75A    0F85 34570000             JNZ PathOfEx.00871E94
$+1B6    0086C760    B8 443FE400               MOV EAX,PathOfEx.00E43F44                       ; UNICODE "E_PENDING"
$+1BB    0086C765    C3                        RETN
$+1BC    0086C766    B8 8440E400               MOV EAX,PathOfEx.00E44084                       ; UNICODE "E_NOTIMPL"
$+1C1    0086C76B    C3                        RETN
$+1C2    0086C76C    81F9 FFFF0080             CMP ECX,0x8000FFFF
$+1C8    0086C772    0F8F 36010000             JG PathOfEx.0086C8AE
$+1CE    0086C778    0F84 2A010000             JE PathOfEx.0086C8A8
$+1D4    0086C77E    81C1 FEBFFF7F             ADD ECX,0x7FFFBFFE
$+1DA    0086C784    83F9 31                   CMP ECX,0x31
$+1DD    0086C787    0F87 07570000             JA PathOfEx.00871E94
$+1E3    0086C78D    FF248D A01E8700           JMP DWORD PTR DS:[ECX*4+0x871EA0]
$+1EA    0086C794    B8 D040E400               MOV EAX,PathOfEx.00E440D0                       ; UNICODE "E_NOINTERFACE"
$+1EF    0086C799    C3                        RETN
$+1F0    0086C79A    B8 EC40E400               MOV EAX,PathOfEx.00E440EC                       ; UNICODE "E_POINTER"
$+1F5    0086C79F    C3                        RETN
$+1F6    0086C7A0    B8 1441E400               MOV EAX,PathOfEx.00E44114                       ; UNICODE "E_ABORT"
$+1FB    0086C7A5    C3                        RETN
$+1FC    0086C7A6    B8 2441E400               MOV EAX,PathOfEx.00E44124                       ; UNICODE "E_FAIL"
$+201    0086C7AB    C3                        RETN
$+202    0086C7AC    B8 583FE400               MOV EAX,PathOfEx.00E43F58                       ; UNICODE "CO_E_INIT_TLS"
$+207    0086C7B1    C3                        RETN
$+208    0086C7B2    B8 743FE400               MOV EAX,PathOfEx.00E43F74                       ; UNICODE "CO_E_INIT_SHARED_ALLOCATOR"
$+20D    0086C7B7    C3                        RETN
$+20E    0086C7B8    B8 AC3FE400               MOV EAX,PathOfEx.00E43FAC                       ; UNICODE "CO_E_INIT_MEMORY_ALLOCATOR"
$+213    0086C7BD    C3                        RETN
$+214    0086C7BE    B8 E43FE400               MOV EAX,PathOfEx.00E43FE4                       ; UNICODE "CO_E_INIT_CLASS_CACHE"
$+219    0086C7C3    C3                        RETN
$+21A    0086C7C4    B8 1040E400               MOV EAX,PathOfEx.00E44010                       ; UNICODE "CO_E_INIT_RPC_CHANNEL"
$+21F    0086C7C9    C3                        RETN
$+220    0086C7CA    B8 4040E400               MOV EAX,PathOfEx.00E44040                       ; UNICODE "CO_E_INIT_TLS_SET_CHANNEL_CONTROL"
$+225    0086C7CF    C3                        RETN
$+226    0086C7D0    B8 D442E400               MOV EAX,PathOfEx.00E442D4                       ; UNICODE "CO_E_INIT_TLS_CHANNEL_CONTROL"
$+22B    0086C7D5    C3                        RETN
$+22C    0086C7D6    B8 1043E400               MOV EAX,PathOfEx.00E44310                       ; UNICODE "CO_E_INIT_UNACCEPTED_USER_ALLOCATOR"
$+231    0086C7DB    C3                        RETN
$+232    0086C7DC    B8 5843E400               MOV EAX,PathOfEx.00E44358                       ; UNICODE "CO_E_INIT_SCM_MUTEX_EXISTS"
$+237    0086C7E1    C3                        RETN
$+238    0086C7E2    B8 9043E400               MOV EAX,PathOfEx.00E44390                       ; UNICODE "CO_E_INIT_SCM_FILE_MAPPING_EXISTS"
$+23D    0086C7E7    C3                        RETN
$+23E    0086C7E8    B8 D443E400               MOV EAX,PathOfEx.00E443D4                       ; UNICODE "CO_E_INIT_SCM_MAP_VIEW_OF_FILE"
$+243    0086C7ED    C3                        RETN
$+244    0086C7EE    B8 1444E400               MOV EAX,PathOfEx.00E44414                       ; UNICODE "CO_E_INIT_SCM_EXEC_FAILURE"
$+249    0086C7F3    C3                        RETN
$+24A    0086C7F4    B8 4C44E400               MOV EAX,PathOfEx.00E4444C                       ; UNICODE "CO_E_INIT_ONLY_SINGLE_THREADED"
$+24F    0086C7F9    C3                        RETN
$+250    0086C7FA    B8 8C44E400               MOV EAX,PathOfEx.00E4448C                       ; UNICODE "CO_E_CANT_REMOTE"
$+255    0086C7FF    C3                        RETN
$+256    0086C800    B8 3441E400               MOV EAX,PathOfEx.00E44134                       ; UNICODE "CO_E_BAD_SERVER_NAME"
$+25B    0086C805    C3                        RETN
$+25C    0086C806    B8 6041E400               MOV EAX,PathOfEx.00E44160                       ; UNICODE "CO_E_WRONG_SERVER_IDENTITY"
$+261    0086C80B    C3                        RETN
$+262    0086C80C    B8 9841E400               MOV EAX,PathOfEx.00E44198                       ; UNICODE "CO_E_OLE1DDE_DISABLED"
$+267    0086C811    C3                        RETN
$+268    0086C812    B8 C441E400               MOV EAX,PathOfEx.00E441C4                       ; UNICODE "CO_E_RUNAS_SYNTAX"
$+26D    0086C817    C3                        RETN
$+26E    0086C818    B8 E841E400               MOV EAX,PathOfEx.00E441E8                       ; UNICODE "CO_E_CREATEPROCESS_FAILURE"
$+273    0086C81D    C3                        RETN
$+274    0086C81E    B8 2042E400               MOV EAX,PathOfEx.00E44220                       ; UNICODE "CO_E_RUNAS_CREATEPROCESS_FAILURE"
$+279    0086C823    C3                        RETN
$+27A    0086C824    B8 6442E400               MOV EAX,PathOfEx.00E44264                       ; UNICODE "CO_E_RUNAS_LOGON_FAILURE"
$+27F    0086C829    C3                        RETN
$+280    0086C82A    B8 9842E400               MOV EAX,PathOfEx.00E44298                       ; UNICODE "CO_E_LAUNCH_PERMSSION_DENIED"
$+285    0086C82F    C3                        RETN
$+286    0086C830    B8 A845E400               MOV EAX,PathOfEx.00E445A8                       ; UNICODE "CO_E_START_SERVICE_FAILURE"
$+28B    0086C835    C3                        RETN
$+28C    0086C836    B8 E045E400               MOV EAX,PathOfEx.00E445E0                       ; UNICODE "CO_E_REMOTE_COMMUNICATION_FAILURE"
$+291    0086C83B    C3                        RETN
$+292    0086C83C    B8 2446E400               MOV EAX,PathOfEx.00E44624                       ; UNICODE "CO_E_SERVER_START_TIMEOUT"
$+297    0086C841    C3                        RETN
$+298    0086C842    B8 5846E400               MOV EAX,PathOfEx.00E44658                       ; UNICODE "CO_E_CLSREG_INCONSISTENT"
$+29D    0086C847    C3                        RETN
$+29E    0086C848    B8 8C46E400               MOV EAX,PathOfEx.00E4468C                       ; UNICODE "CO_E_IIDREG_INCONSISTENT"
$+2A3    0086C84D    C3                        RETN
$+2A4    0086C84E    B8 C046E400               MOV EAX,PathOfEx.00E446C0                       ; UNICODE "CO_E_NOT_SUPPORTED"
$+2A9    0086C853    C3                        RETN
$+2AA    0086C854    B8 E846E400               MOV EAX,PathOfEx.00E446E8                       ; UNICODE "CO_E_RELOAD_DLL"
$+2AF    0086C859    C3                        RETN
$+2B0    0086C85A    B8 0847E400               MOV EAX,PathOfEx.00E44708                       ; UNICODE "CO_E_MSI_ERROR"
$+2B5    0086C85F    C3                        RETN
$+2B6    0086C860    B8 40B6E200               MOV EAX,PathOfEx.00E2B640                       ; UNICODE "CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT"
$+2BB    0086C865    C3                        RETN
$+2BC    0086C866    B8 9CB6E200               MOV EAX,PathOfEx.00E2B69C                       ; UNICODE "CO_E_SERVER_PAUSED"
$+2C1    0086C86B    C3                        RETN
$+2C2    0086C86C    B8 C4B6E200               MOV EAX,PathOfEx.00E2B6C4                       ; UNICODE "CO_E_SERVER_NOT_PAUSED"
$+2C7    0086C871    C3                        RETN
$+2C8    0086C872    B8 F4B6E200               MOV EAX,PathOfEx.00E2B6F4                       ; UNICODE "CO_E_CLASS_DISABLED"
$+2CD    0086C877    C3                        RETN
$+2CE    0086C878    B8 24B4E200               MOV EAX,PathOfEx.00E2B424                       ; UNICODE "CO_E_CLRNOTAVAILABLE"
$+2D3    0086C87D    C3                        RETN
$+2D4    0086C87E    B8 50B4E200               MOV EAX,PathOfEx.00E2B450                       ; UNICODE "CO_E_ASYNC_WORK_REJECTED"
$+2D9    0086C883    C3                        RETN
$+2DA    0086C884    B8 84B4E200               MOV EAX,PathOfEx.00E2B484                       ; UNICODE "CO_E_SERVER_INIT_TIMEOUT"
$+2DF    0086C889    C3                        RETN
$+2E0    0086C88A    B8 B8B4E200               MOV EAX,PathOfEx.00E2B4B8                       ; UNICODE "CO_E_NO_SECCTX_IN_ACTIVATE"
$+2E5    0086C88F    C3                        RETN
$+2E6    0086C890    B8 F0B4E200               MOV EAX,PathOfEx.00E2B4F0                       ; UNICODE "CO_E_TRACKER_CONFIG"
$+2EB    0086C895    C3                        RETN
$+2EC    0086C896    B8 18B5E200               MOV EAX,PathOfEx.00E2B518                       ; UNICODE "CO_E_THREADPOOL_CONFIG"
$+2F1    0086C89B    C3                        RETN
$+2F2    0086C89C    B8 48B5E200               MOV EAX,PathOfEx.00E2B548                       ; UNICODE "CO_E_SXS_CONFIG"
$+2F7    0086C8A1    C3                        RETN
$+2F8    0086C8A2    B8 68B5E200               MOV EAX,PathOfEx.00E2B568                       ; UNICODE "CO_E_MALFORMED_SPN"
$+2FD    0086C8A7    C3                        RETN
$+2FE    0086C8A8    B8 083FE400               MOV EAX,PathOfEx.00E43F08                       ; UNICODE "E_UNEXPECTED"
$+303    0086C8AD    C3                        RETN
$+304    0086C8AE    81C1 FFFFFE7F             ADD ECX,0x7FFEFFFF
$+30A    0086C8B4    83F9 11                   CMP ECX,0x11
$+30D    0086C8B7    0F87 D7550000             JA PathOfEx.00871E94
$+313    0086C8BD    FF248D 681F8700           JMP DWORD PTR DS:[ECX*4+0x871F68]
$+31A    0086C8C4    B8 E46FE400               MOV EAX,PathOfEx.00E46FE4                       ; UNICODE "RPC_E_CALL_REJECTED"
$+31F    0086C8C9    C3                        RETN
$+320    0086C8CA    B8 0C70E400               MOV EAX,PathOfEx.00E4700C                       ; UNICODE "RPC_E_CALL_CANCELED"
$+325    0086C8CF    C3                        RETN
$+326    0086C8D0    B8 3470E400               MOV EAX,PathOfEx.00E47034                       ; UNICODE "RPC_E_CANTPOST_INSENDCALL"
$+32B    0086C8D5    C3                        RETN
$+32C    0086C8D6    B8 706DE400               MOV EAX,PathOfEx.00E46D70                       ; UNICODE "RPC_E_CANTCALLOUT_INASYNCCALL"
$+331    0086C8DB    C3                        RETN
$+332    0086C8DC    B8 B06DE400               MOV EAX,PathOfEx.00E46DB0                       ; UNICODE "RPC_E_CANTCALLOUT_INEXTERNALCALL"
$+337    0086C8E1    C3                        RETN
$+338    0086C8E2    B8 F46DE400               MOV EAX,PathOfEx.00E46DF4                       ; UNICODE "RPC_E_CONNECTION_TERMINATED"
$+33D    0086C8E7    C3                        RETN
$+33E    0086C8E8    B8 2C6EE400               MOV EAX,PathOfEx.00E46E2C                       ; UNICODE "RPC_E_SERVER_DIED"
$+343    0086C8ED    C3                        RETN
$+344    0086C8EE    B8 506EE400               MOV EAX,PathOfEx.00E46E50                       ; UNICODE "RPC_E_CLIENT_DIED"
$+349    0086C8F3    C3                        RETN
$+34A    0086C8F4    B8 746EE400               MOV EAX,PathOfEx.00E46E74                       ; UNICODE "RPC_E_INVALID_DATAPACKET"
$+34F    0086C8F9    C3                        RETN
$+350    0086C8FA    B8 A86EE400               MOV EAX,PathOfEx.00E46EA8                       ; UNICODE "RPC_E_CANTTRANSMIT_CALL"
$+355    0086C8FF    C3                        RETN
$+356    0086C900    B8 D86EE400               MOV EAX,PathOfEx.00E46ED8                       ; UNICODE "RPC_E_CLIENT_CANTMARSHAL_DATA"
$+35B    0086C905    C3                        RETN
$+35C    0086C906    B8 B071E400               MOV EAX,PathOfEx.00E471B0                       ; UNICODE "RPC_E_CLIENT_CANTUNMARSHAL_DATA"
$+361    0086C90B    C3                        RETN
$+362    0086C90C    B8 F071E400               MOV EAX,PathOfEx.00E471F0                       ; UNICODE "RPC_E_SERVER_CANTMARSHAL_DATA"
$+367    0086C911    C3                        RETN
$+368    0086C912    B8 3072E400               MOV EAX,PathOfEx.00E47230                       ; UNICODE "RPC_E_SERVER_CANTUNMARSHAL_DATA"
$+36D    0086C917    C3                        RETN
$+36E    0086C918    B8 7072E400               MOV EAX,PathOfEx.00E47270                       ; UNICODE "RPC_E_INVALID_DATA"
$+373    0086C91D    C3                        RETN
$+374    0086C91E    B8 9872E400               MOV EAX,PathOfEx.00E47298                       ; UNICODE "RPC_E_INVALID_PARAMETER"
$+379    0086C923    C3                        RETN
$+37A    0086C924    B8 C872E400               MOV EAX,PathOfEx.00E472C8                       ; UNICODE "RPC_E_CANTCALLOUT_AGAIN"
$+37F    0086C929    C3                        RETN
$+380    0086C92A    B8 F872E400               MOV EAX,PathOfEx.00E472F8                       ; UNICODE "RPC_E_SERVER_DIED_DNE"
$+385    0086C92F    C3                        RETN
$+386    0086C930    B8 2473E400               MOV EAX,PathOfEx.00E47324                       ; UNICODE "RPC_E_SYS_CALL_FAILED"
$+38B    0086C935    C3                        RETN
$+38C    0086C936    81F9 FFFF0180             CMP ECX,0x8001FFFF
$+392    0086C93C    0F8F 9C010000             JG PathOfEx.0086CADE
$+398    0086C942    0F84 90010000             JE PathOfEx.0086CAD8
$+39E    0086C948    81C1 FFFEFE7F             ADD ECX,0x7FFEFEFF
$+3A4    0086C94E    83F9 3F                   CMP ECX,0x3F
$+3A7    0086C951    0F87 3D550000             JA PathOfEx.00871E94
$+3AD    0086C957    FF248D B01F8700           JMP DWORD PTR DS:[ECX*4+0x871FB0]
$+3B4    0086C95E    B8 6870E400               MOV EAX,PathOfEx.00E47068                       ; UNICODE "RPC_E_OUT_OF_RESOURCES"
$+3B9    0086C963    C3                        RETN
$+3BA    0086C964    B8 9870E400               MOV EAX,PathOfEx.00E47098                       ; UNICODE "RPC_E_ATTEMPTED_MULTITHREAD"
$+3BF    0086C969    C3                        RETN
$+3C0    0086C96A    B8 D070E400               MOV EAX,PathOfEx.00E470D0                       ; UNICODE "RPC_E_NOT_REGISTERED"
$+3C5    0086C96F    C3                        RETN
$+3C6    0086C970    B8 FC70E400               MOV EAX,PathOfEx.00E470FC                       ; UNICODE "RPC_E_FAULT"
$+3CB    0086C975    C3                        RETN
$+3CC    0086C976    B8 1471E400               MOV EAX,PathOfEx.00E47114                       ; UNICODE "RPC_E_SERVERFAULT"
$+3D1    0086C97B    C3                        RETN
$+3D2    0086C97C    B8 3871E400               MOV EAX,PathOfEx.00E47138                       ; UNICODE "RPC_E_CHANGED_MODE"
$+3D7    0086C981    C3                        RETN
$+3D8    0086C982    B8 6071E400               MOV EAX,PathOfEx.00E47160                       ; UNICODE "RPC_E_INVALIDMETHOD"
$+3DD    0086C987    C3                        RETN
$+3DE    0086C988    B8 8871E400               MOV EAX,PathOfEx.00E47188                       ; UNICODE "RPC_E_DISCONNECTED"
$+3E3    0086C98D    C3                        RETN
$+3E4    0086C98E    B8 9874E400               MOV EAX,PathOfEx.00E47498                       ; UNICODE "RPC_E_RETRY"
$+3E9    0086C993    C3                        RETN
$+3EA    0086C994    B8 B074E400               MOV EAX,PathOfEx.00E474B0                       ; UNICODE "RPC_E_SERVERCALL_RETRYLATER"
$+3EF    0086C999    C3                        RETN
$+3F0    0086C99A    B8 E874E400               MOV EAX,PathOfEx.00E474E8                       ; UNICODE "RPC_E_SERVERCALL_REJECTED"
$+3F5    0086C99F    C3                        RETN
$+3F6    0086C9A0    B8 1C75E400               MOV EAX,PathOfEx.00E4751C                       ; UNICODE "RPC_E_INVALID_CALLDATA"
$+3FB    0086C9A5    C3                        RETN
$+3FC    0086C9A6    B8 5075E400               MOV EAX,PathOfEx.00E47550                       ; UNICODE "RPC_E_CANTCALLOUT_ININPUTSYNCCALL"
$+401    0086C9AB    C3                        RETN
$+402    0086C9AC    B8 9475E400               MOV EAX,PathOfEx.00E47594                       ; UNICODE "RPC_E_WRONG_THREAD"
$+407    0086C9B1    C3                        RETN
$+408    0086C9B2    B8 BC75E400               MOV EAX,PathOfEx.00E475BC                       ; UNICODE "RPC_E_THREAD_NOT_INIT"
$+40D    0086C9B7    C3                        RETN
$+40E    0086C9B8    B8 E875E400               MOV EAX,PathOfEx.00E475E8                       ; UNICODE "RPC_E_VERSION_MISMATCH"
$+413    0086C9BD    C3                        RETN
$+414    0086C9BE    B8 5073E400               MOV EAX,PathOfEx.00E47350                       ; UNICODE "RPC_E_INVALID_HEADER"
$+419    0086C9C3    C3                        RETN
$+41A    0086C9C4    B8 7C73E400               MOV EAX,PathOfEx.00E4737C                       ; UNICODE "RPC_E_INVALID_EXTENSION"
$+41F    0086C9C9    C3                        RETN
$+420    0086C9CA    B8 AC73E400               MOV EAX,PathOfEx.00E473AC                       ; UNICODE "RPC_E_INVALID_IPID"
$+425    0086C9CF    C3                        RETN
$+426    0086C9D0    B8 D473E400               MOV EAX,PathOfEx.00E473D4                       ; UNICODE "RPC_E_INVALID_OBJECT"
$+42B    0086C9D5    C3                        RETN
$+42C    0086C9D6    B8 0074E400               MOV EAX,PathOfEx.00E47400                       ; UNICODE "RPC_S_CALLPENDING"
$+431    0086C9DB    C3                        RETN
$+432    0086C9DC    B8 2474E400               MOV EAX,PathOfEx.00E47424                       ; UNICODE "RPC_S_WAITONTIMER"
$+437    0086C9E1    C3                        RETN
$+438    0086C9E2    B8 4874E400               MOV EAX,PathOfEx.00E47448                       ; UNICODE "RPC_E_CALL_COMPLETE"
$+43D    0086C9E7    C3                        RETN
$+43E    0086C9E8    B8 7074E400               MOV EAX,PathOfEx.00E47470                       ; UNICODE "RPC_E_UNSECURE_CALL"
$+443    0086C9ED    C3                        RETN
$+444    0086C9EE    B8 C877E400               MOV EAX,PathOfEx.00E477C8                       ; UNICODE "RPC_E_TOO_LATE"
$+449    0086C9F3    C3                        RETN
$+44A    0086C9F4    B8 E877E400               MOV EAX,PathOfEx.00E477E8                       ; UNICODE "RPC_E_NO_GOOD_SECURITY_PACKAGES"
$+44F    0086C9F9    C3                        RETN
$+450    0086C9FA    B8 2878E400               MOV EAX,PathOfEx.00E47828                       ; UNICODE "RPC_E_ACCESS_DENIED"
$+455    0086C9FF    C3                        RETN
$+456    0086CA00    B8 5078E400               MOV EAX,PathOfEx.00E47850                       ; UNICODE "RPC_E_REMOTE_DISABLED"
$+45B    0086CA05    C3                        RETN
$+45C    0086CA06    B8 7C78E400               MOV EAX,PathOfEx.00E4787C                       ; UNICODE "RPC_E_INVALID_OBJREF"
$+461    0086CA0B    C3                        RETN
$+462    0086CA0C    B8 A878E400               MOV EAX,PathOfEx.00E478A8                       ; UNICODE "RPC_E_NO_CONTEXT"
$+467    0086CA11    C3                        RETN
$+468    0086CA12    B8 CC78E400               MOV EAX,PathOfEx.00E478CC                       ; UNICODE "RPC_E_TIMEOUT"
$+46D    0086CA17    C3                        RETN
$+46E    0086CA18    B8 E878E400               MOV EAX,PathOfEx.00E478E8                       ; UNICODE "RPC_E_NO_SYNC"
$+473    0086CA1D    C3                        RETN
$+474    0086CA1E    B8 1876E400               MOV EAX,PathOfEx.00E47618                       ; UNICODE "RPC_E_FULLSIC_REQUIRED"
$+479    0086CA23    C3                        RETN
$+47A    0086CA24    B8 4876E400               MOV EAX,PathOfEx.00E47648                       ; UNICODE "RPC_E_INVALID_STD_NAME"
$+47F    0086CA29    C3                        RETN
$+480    0086CA2A    B8 7876E400               MOV EAX,PathOfEx.00E47678                       ; UNICODE "CO_E_FAILEDTOIMPERSONATE"
$+485    0086CA2F    C3                        RETN
$+486    0086CA30    B8 AC76E400               MOV EAX,PathOfEx.00E476AC                       ; UNICODE "CO_E_FAILEDTOGETSECCTX"
$+48B    0086CA35    C3                        RETN
*/
//tag_获得cpuid的包装函数

//call tag_获得cpuid的包装函数
/*
$-191    012ABD2F    CC                        INT3
$-190    012ABD30    51                        PUSH ECX
$-18F    012ABD31    53                        PUSH EBX
$-18E    012ABD32    55                        PUSH EBP
$-18D    012ABD33    56                        PUSH ESI
$-18C    012ABD34    57                        PUSH EDI
$-18B    012ABD35    8D4C24 13                 LEA ECX,DWORD PTR SS:[ESP+0x13]
$-187    012ABD39    E8 32FDFFFF               CALL PathOfEx.012ABA70
$-182    012ABD3E    8B5C24 1C                 MOV EBX,DWORD PTR SS:[ESP+0x1C]
$-17E    012ABD42    33F6                      XOR ESI,ESI
$-17C    012ABD44    8B6C24 18                 MOV EBP,DWORD PTR SS:[ESP+0x18]
$-178    012ABD48    8B38                      MOV EDI,DWORD PTR DS:[EAX]
$-176    012ABD4A    0FB707                    MOVZX EAX,WORD PTR DS:[EDI]
$-173    012ABD4D    3BC3                      CMP EAX,EBX
$-171    012ABD4F    73 1D                     JNB SHORT PathOfEx.012ABD6E
$-16F    012ABD51    8BCF                      MOV ECX,EDI
$-16D    012ABD53    0FB701                    MOVZX EAX,WORD PTR DS:[ECX]
$-16A    012ABD56    8BCD                      MOV ECX,EBP
$-168    012ABD58    50                        PUSH EAX
$-167    012ABD59    E8 9235FEFF               CALL PathOfEx.0128F2F0
$-162    012ABD5E    85C0                      TEST EAX,EAX
$-160    012ABD60    74 27                     JE SHORT PathOfEx.012ABD89
$-15E    012ABD62    46                        INC ESI
$-15D    012ABD63    0FB70477                  MOVZX EAX,WORD PTR DS:[EDI+ESI*2]
$-159    012ABD67    8D0C77                    LEA ECX,DWORD PTR DS:[EDI+ESI*2]
$-156    012ABD6A    3BC3                      CMP EAX,EBX
$-154    012ABD6C  ^ 72 E5                     JB SHORT PathOfEx.012ABD53
$-152    012ABD6E    0FB70477                  MOVZX EAX,WORD PTR DS:[EDI+ESI*2]
$-14E    012ABD72    3BD8                      CMP EBX,EAX
$-14C    012ABD74    75 1B                     JNZ SHORT PathOfEx.012ABD91
$-14A    012ABD76    53                        PUSH EBX
$-149    012ABD77    8BCD                      MOV ECX,EBP
$-147    012ABD79    E8 7235FEFF               CALL PathOfEx.0128F2F0
$-142    012ABD7E    5F                        POP EDI
$-141    012ABD7F    F7D8                      NEG EAX
$-13F    012ABD81    5E                        POP ESI
$-13E    012ABD82    1BC0                      SBB EAX,EAX
$-13C    012ABD84    5D                        POP EBP
$-13B    012ABD85    40                        INC EAX
$-13A    012ABD86    5B                        POP EBX
$-139    012ABD87    59                        POP ECX
$-138    012ABD88    C3                        RETN
$-137    012ABD89    5F                        POP EDI
$-136    012ABD8A    5E                        POP ESI
$-135    012ABD8B    5D                        POP EBP
$-134    012ABD8C    B0 01                     MOV AL,0x1
$-132    012ABD8E    5B                        POP EBX
$-131    012ABD8F    59                        POP ECX
$-130    012ABD90    C3                        RETN
$-12F    012ABD91    5F                        POP EDI
$-12E    012ABD92    5E                        POP ESI
$-12D    012ABD93    5D                        POP EBP
$-12C    012ABD94    32C0                      XOR AL,AL
$-12A    012ABD96    5B                        POP EBX
$-129    012ABD97    59                        POP ECX
$-128    012ABD98    C3                        RETN
$-127    012ABD99    CC                        INT3
$-126    012ABD9A    CC                        INT3
$-125    012ABD9B    CC                        INT3
$-124    012ABD9C    CC                        INT3
$-123    012ABD9D    CC                        INT3
$-122    012ABD9E    CC                        INT3
$-121    012ABD9F    CC                        INT3
$-120    012ABDA0    56                        PUSH ESI
$-11F    012ABDA1    8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
$-11B    012ABDA5    56                        PUSH ESI
$-11A    012ABDA6    E8 25E3FFFF               CALL PathOfEx.012AA0D0
$-115    012ABDAB    83C4 04                   ADD ESP,0x4
$-112    012ABDAE    84C0                      TEST AL,AL
$-110    012ABDB0    74 17                     JE SHORT PathOfEx.012ABDC9
$-10E    012ABDB2    6A 01                     PUSH 0x1
$-10C    012ABDB4    56                        PUSH ESI
$-10B    012ABDB5    FF7424 10                 PUSH DWORD PTR SS:[ESP+0x10]
$-107    012ABDB9    E8 D2F9FFFF               CALL PathOfEx.012AB790
$-102    012ABDBE    83C4 0C                   ADD ESP,0xC
$-FF     012ABDC1    84C0                      TEST AL,AL
$-FD     012ABDC3    74 04                     JE SHORT PathOfEx.012ABDC9
$-FB     012ABDC5    B0 01                     MOV AL,0x1
$-F9     012ABDC7    EB 02                     JMP SHORT PathOfEx.012ABDCB
$-F7     012ABDC9    32C0                      XOR AL,AL
$-F5     012ABDCB    837C24 10 01              CMP DWORD PTR SS:[ESP+0x10],0x1
$-F0     012ABDD0    72 1D                     JB SHORT PathOfEx.012ABDEF
$-EE     012ABDD2    84C0                      TEST AL,AL
$-EC     012ABDD4    74 17                     JE SHORT PathOfEx.012ABDED
$-EA     012ABDD6    6A 0A                     PUSH 0xA
$-E8     012ABDD8    56                        PUSH ESI
$-E7     012ABDD9    FF7424 10                 PUSH DWORD PTR SS:[ESP+0x10]
$-E3     012ABDDD    E8 AEF9FFFF               CALL PathOfEx.012AB790
$-DE     012ABDE2    83C4 0C                   ADD ESP,0xC
$-DB     012ABDE5    84C0                      TEST AL,AL
$-D9     012ABDE7    74 04                     JE SHORT PathOfEx.012ABDED
$-D7     012ABDE9    B0 01                     MOV AL,0x1
$-D5     012ABDEB    5E                        POP ESI
$-D4     012ABDEC    C3                        RETN
$-D3     012ABDED    32C0                      XOR AL,AL
$-D1     012ABDEF    5E                        POP ESI
$-D0     012ABDF0    C3                        RETN
$-CF     012ABDF1    CC                        INT3
$-CE     012ABDF2    CC                        INT3
$-CD     012ABDF3    CC                        INT3
$-CC     012ABDF4    CC                        INT3
$-CB     012ABDF5    CC                        INT3
$-CA     012ABDF6    CC                        INT3
$-C9     012ABDF7    CC                        INT3
$-C8     012ABDF8    CC                        INT3
$-C7     012ABDF9    CC                        INT3
$-C6     012ABDFA    CC                        INT3
$-C5     012ABDFB    CC                        INT3
$-C4     012ABDFC    CC                        INT3
$-C3     012ABDFD    CC                        INT3
$-C2     012ABDFE    CC                        INT3
$-C1     012ABDFF    CC                        INT3
$-C0     012ABE00    55                        PUSH EBP                                                      ; tag_获得cpuid的包装函数
$-BF     012ABE01    8BEC                      MOV EBP,ESP
$-BD     012ABE03    6A FE                     PUSH -0x2
$-BB     012ABE05    68 C8225901               PUSH PathOfEx.015922C8
$-B6     012ABE0A    68 40D22401               PUSH PathOfEx.0124D240
$-B1     012ABE0F    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-AB     012ABE15    50                        PUSH EAX
$-AA     012ABE16    83EC 08                   SUB ESP,0x8
$-A7     012ABE19    53                        PUSH EBX
$-A6     012ABE1A    56                        PUSH ESI
$-A5     012ABE1B    57                        PUSH EDI
$-A4     012ABE1C    A1 28985901               MOV EAX,DWORD PTR DS:[0x1599828]
$-9F     012ABE21    3145 F8                   XOR DWORD PTR SS:[EBP-0x8],EAX
$-9C     012ABE24    33C5                      XOR EAX,EBP
$-9A     012ABE26    50                        PUSH EAX
$-99     012ABE27    8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
$-96     012ABE2A    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
$-90     012ABE30    8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
$-8D     012ABE33    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-86     012ABE3A    8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
$-83     012ABE3D    B9 00000000               MOV ECX,0x0
$-7E     012ABE42    0FA2                      CPUID                                                         ; g_h_cpuid8_
$-7C     012ABE44    8B7D 0C                   MOV EDI,DWORD PTR SS:[EBP+0xC]
$-79     012ABE47    8907                      MOV DWORD PTR DS:[EDI],EAX
$-77     012ABE49    895F 04                   MOV DWORD PTR DS:[EDI+0x4],EBX
$-74     012ABE4C    894F 08                   MOV DWORD PTR DS:[EDI+0x8],ECX
$-71     012ABE4F    8957 0C                   MOV DWORD PTR DS:[EDI+0xC],EDX
$-6E     012ABE52    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
$-67     012ABE59    837D 08 00                CMP DWORD PTR SS:[EBP+0x8],0x0
$-63     012ABE5D    75 1C                     JNZ SHORT PathOfEx.012ABE7B
$-61     012ABE5F    33C0                      XOR EAX,EAX
$-5F     012ABE61    8B4D 0C                   MOV ECX,DWORD PTR SS:[EBP+0xC]
$-5C     012ABE64    3901                      CMP DWORD PTR DS:[ECX],EAX
$-5A     012ABE66    0F95C0                    SETNE AL
$-57     012ABE69    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$-54     012ABE6C    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-4D     012ABE73    59                        POP ECX
$-4C     012ABE74    5F                        POP EDI
$-4B     012ABE75    5E                        POP ESI
$-4A     012ABE76    5B                        POP EBX
$-49     012ABE77    8BE5                      MOV ESP,EBP
$-47     012ABE79    5D                        POP EBP
$-46     012ABE7A    C3                        RETN
$-45     012ABE7B    B0 01                     MOV AL,0x1
$-43     012ABE7D    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$-40     012ABE80    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-39     012ABE87    59                        POP ECX
$-38     012ABE88    5F                        POP EDI
$-37     012ABE89    5E                        POP ESI
$-36     012ABE8A    5B                        POP EBX
$-35     012ABE8B    8BE5                      MOV ESP,EBP
$-33     012ABE8D    5D                        POP EBP
$-32     012ABE8E    C3                        RETN
$-31     012ABE8F    B8 01000000               MOV EAX,0x1
$-2C     012ABE94    C3                        RETN
$-2B     012ABE95    8B65 E8                   MOV ESP,DWORD PTR SS:[EBP-0x18]
$-28     012ABE98    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
$-21     012ABE9F    32C0                      XOR AL,AL
$-1F     012ABEA1    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$-1C     012ABEA4    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-15     012ABEAB    59                        POP ECX
$-14     012ABEAC    5F                        POP EDI
$-13     012ABEAD    5E                        POP ESI
$-12     012ABEAE    5B                        POP EBX
$-11     012ABEAF    8BE5                      MOV ESP,EBP
$-F      012ABEB1    5D                        POP EBP
$-E      012ABEB2    C3                        RETN
$-D      012ABEB3    CC                        INT3
$-C      012ABEB4    CC                        INT3
$-B      012ABEB5    CC                        INT3
$-A      012ABEB6    CC                        INT3
$-9      012ABEB7    CC                        INT3
$-8      012ABEB8    CC                        INT3
$-7      012ABEB9    CC                        INT3
$-6      012ABEBA    CC                        INT3
$-5      012ABEBB    CC                        INT3
$-4      012ABEBC    CC                        INT3
$-3      012ABEBD    CC                        INT3
$-2      012ABEBE    CC                        INT3
$-1      012ABEBF    CC                        INT3
$ ==>    012ABEC0    83EC 40                   SUB ESP,0x40
$+3      012ABEC3    8D0424                    LEA EAX,DWORD PTR SS:[ESP]
$+6      012ABEC6    50                        PUSH EAX
$+7      012ABEC7    6A 00                     PUSH 0x0
$+9      012ABEC9    E8 32FFFFFF               CALL PathOfEx.012ABE00                                        ; call tag_获得cpuid的包装函数
$+E      012ABECE    83C4 08                   ADD ESP,0x8
$+11     012ABED1    84C0                      TEST AL,AL
$+13     012ABED3    0F84 7F010000             JE PathOfEx.012AC058
$+19     012ABED9    8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
$+1D     012ABEDD    50                        PUSH EAX
$+1E     012ABEDE    6A 01                     PUSH 0x1
$+20     012ABEE0    E8 1BFFFFFF               CALL PathOfEx.012ABE00                                        ; call tag_获得cpuid的包装函数
$+25     012ABEE5    83C4 08                   ADD ESP,0x8
$+28     012ABEE8    84C0                      TEST AL,AL
$+2A     012ABEEA    0F84 68010000             JE PathOfEx.012AC058
$+30     012ABEF0    8B4C24 1C                 MOV ECX,DWORD PTR SS:[ESP+0x1C]
$+34     012ABEF4    8BC1                      MOV EAX,ECX
$+36     012ABEF6    C1E8 17                   SHR EAX,0x17
$+39     012ABEF9    24 01                     AND AL,0x1
$+3B     012ABEFB    A2 65DE5B01               MOV BYTE PTR DS:[0x15BDE65],AL
$+40     012ABF00    F7C1 00000004             TEST ECX,0x4000000
$+46     012ABF06    74 10                     JE SHORT PathOfEx.012ABF18
$+48     012ABF08    E8 93010000               CALL PathOfEx.012AC0A0
$+4D     012ABF0D    8B4C24 1C                 MOV ECX,DWORD PTR SS:[ESP+0x1C]
$+51     012ABF11    A2 67DE5B01               MOV BYTE PTR DS:[0x15BDE67],AL
$+56     012ABF16    EB 05                     JMP SHORT PathOfEx.012ABF1D
$+58     012ABF18    A0 67DE5B01               MOV AL,BYTE PTR DS:[0x15BDE67]
$+5D     012ABF1D    8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
$+61     012ABF21    84C0                      TEST AL,AL
$+63     012ABF23    74 0F                     JE SHORT PathOfEx.012ABF34
$+65     012ABF25    C605 68DE5B01 01          MOV BYTE PTR DS:[0x15BDE68],0x1
$+6C     012ABF2C    F7C2 00020000             TEST EDX,0x200
$+72     012ABF32    75 07                     JNZ SHORT PathOfEx.012ABF3B
$+74     012ABF34    C605 68DE5B01 00          MOV BYTE PTR DS:[0x15BDE68],0x0
$+7B     012ABF3B    84C0                      TEST AL,AL
$+7D     012ABF3D    74 0F                     JE SHORT PathOfEx.012ABF4E
$+7F     012ABF3F    C605 69DE5B01 01          MOV BYTE PTR DS:[0x15BDE69],0x1
$+86     012ABF46    F7C2 00000002             TEST EDX,0x2000000
$+8C     012ABF4C    75 07                     JNZ SHORT PathOfEx.012ABF55
$+8E     012ABF4E    C605 69DE5B01 00          MOV BYTE PTR DS:[0x15BDE69],0x0
$+95     012ABF55    84C0                      TEST AL,AL
$+97     012ABF57    74 0C                     JE SHORT PathOfEx.012ABF65
$+99     012ABF59    C605 6ADE5B01 01          MOV BYTE PTR DS:[0x15BDE6A],0x1
$+A0     012ABF60    F6C2 02                   TEST DL,0x2
$+A3     012ABF63    75 07                     JNZ SHORT PathOfEx.012ABF6C
$+A5     012ABF65    C605 6ADE5B01 00          MOV BYTE PTR DS:[0x15BDE6A],0x0
$+AC     012ABF6C    F7C1 00000002             TEST ECX,0x2000000
$+B2     012ABF72    74 09                     JE SHORT PathOfEx.012ABF7D
$+B4     012ABF74    C605 66DE5B01 01          MOV BYTE PTR DS:[0x15BDE66],0x1
$+BB     012ABF7B    EB 40                     JMP SHORT PathOfEx.012ABFBD
$+BD     012ABF7D    8D4424 20                 LEA EAX,DWORD PTR SS:[ESP+0x20]
$+C1     012ABF81    50                        PUSH EAX
$+C2     012ABF82    68 00000080               PUSH 0x80000000
$+C7     012ABF87    E8 74FEFFFF               CALL PathOfEx.012ABE00                                        ; call tag_获得cpuid的包装函数
$+CC     012ABF8C    83C4 08                   ADD ESP,0x8
$+CF     012ABF8F    817C24 20 01000080        CMP DWORD PTR SS:[ESP+0x20],0x80000001
$+D7     012ABF97    72 20                     JB SHORT PathOfEx.012ABFB9
$+D9     012ABF99    8D4424 20                 LEA EAX,DWORD PTR SS:[ESP+0x20]
$+DD     012ABF9D    50                        PUSH EAX
$+DE     012ABF9E    68 01000080               PUSH 0x80000001
$+E3     012ABFA3    E8 58FEFFFF               CALL PathOfEx.012ABE00                                        ; call  tag_获得cpuid的包装函数
$+E8     012ABFA8    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+EC     012ABFAC    83C4 08                   ADD ESP,0x8
$+EF     012ABFAF    C1E8 16                   SHR EAX,0x16
$+F2     012ABFB2    24 01                     AND AL,0x1
$+F4     012ABFB4    A2 66DE5B01               MOV BYTE PTR DS:[0x15BDE66],AL
$+F9     012ABFB9    8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
$+FD     012ABFBD    8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
$+101    012ABFC1    3D 47656E75               CMP EAX,0x756E6547
$+106    012ABFC6    0F85 90000000             JNZ PathOfEx.012AC05C
$+10C    012ABFCC    817C24 08 6E74656C        CMP DWORD PTR SS:[ESP+0x8],0x6C65746E
$+114    012ABFD4    75 65                     JNZ SHORT PathOfEx.012AC03B
$+116    012ABFD6    817C24 0C 696E6549        CMP DWORD PTR SS:[ESP+0xC],0x49656E69
$+11E    012ABFDE    75 5B                     JNZ SHORT PathOfEx.012AC03B
$+120    012ABFE0    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
$+124    012ABFE4    25 000F0000               AND EAX,0xF00
$+129    012ABFE9    3D 000F0000               CMP EAX,0xF00
$+12E    012ABFEE    8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
$+132    012ABFF2    0F9405 6BDE5B01           SETE BYTE PTR DS:[0x15BDE6B]
$+139    012ABFF9    C1E8 08                   SHR EAX,0x8
$+13C    012ABFFC    C1EA 1E                   SHR EDX,0x1E
$+13F    012ABFFF    0FB6C8                    MOVZX ECX,AL
$+142    012AC002    80E2 01                   AND DL,0x1
$+145    012AC005    C1E1 03                   SHL ECX,0x3
$+148    012AC008    833C24 07                 CMP DWORD PTR SS:[ESP],0x7
$+14C    012AC00C    890D 50A25901             MOV DWORD PTR DS:[0x159A250],ECX
$+152    012AC012    8815 6CDE5B01             MOV BYTE PTR DS:[0x15BDE6C],DL
$+158    012AC018    72 27                     JB SHORT PathOfEx.012AC041
$+15A    012AC01A    8D4424 30                 LEA EAX,DWORD PTR SS:[ESP+0x30]
$+15E    012AC01E    50                        PUSH EAX
$+15F    012AC01F    6A 07                     PUSH 0x7
$+161    012AC021    E8 DAFDFFFF               CALL PathOfEx.012ABE00                                        ; call tag_获得cpuid的包装函数
$+166    012AC026    83C4 08                   ADD ESP,0x8
$+169    012AC029    84C0                      TEST AL,AL
$+16B    012AC02B    74 0E                     JE SHORT PathOfEx.012AC03B
$+16D    012AC02D    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+171    012AC031    C1E8 12                   SHR EAX,0x12
$+174    012AC034    24 01                     AND AL,0x1
$+176    012AC036    A2 6DDE5B01               MOV BYTE PTR DS:[0x15BDE6D],AL
$+17B    012AC03B    8B0D 50A25901             MOV ECX,DWORD PTR DS:[0x159A250]
$+181    012AC041    85C9                      TEST ECX,ECX
$+183    012AC043    B8 20000000               MOV EAX,0x20
$+188    012AC048    0F44C8                    CMOVE ECX,EAX
$+18B    012AC04B    890D 50A25901             MOV DWORD PTR DS:[0x159A250],ECX
$+191    012AC051    C605 64DE5B01 01          MOV BYTE PTR DS:[0x15BDE64],0x1
$+198    012AC058    83C4 40                   ADD ESP,0x40
$+19B    012AC05B    C3                        RETN
$+19C    012AC05C    3D 41757468               CMP EAX,0x68747541
$+1A1    012AC061  ^ 75 D8                     JNZ SHORT PathOfEx.012AC03B
$+1A3    012AC063    817C24 08 656E7469        CMP DWORD PTR SS:[ESP+0x8],0x69746E65
$+1AB    012AC06B  ^ 75 CE                     JNZ SHORT PathOfEx.012AC03B
$+1AD    012AC06D    817C24 0C 63414D44        CMP DWORD PTR SS:[ESP+0xC],0x444D4163
$+1B5    012AC075  ^ 75 C4                     JNZ SHORT PathOfEx.012AC03B
$+1B7    012AC077    8D0424                    LEA EAX,DWORD PTR SS:[ESP]
$+1BA    012AC07A    50                        PUSH EAX
$+1BB    012AC07B    68 05000080               PUSH 0x80000005
$+1C0    012AC080    E8 7BFDFFFF               CALL PathOfEx.012ABE00                                        ; call tag_获得cpuid的包装函数
$+1C5    012AC085    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
$+1C9    012AC089    83C4 08                   ADD ESP,0x8
$+1CC    012AC08C    0FB6C8                    MOVZX ECX,AL
$+1CF    012AC08F    C1E8 1E                   SHR EAX,0x1E
$+1D2    012AC092    24 01                     AND AL,0x1
$+1D4    012AC094    A2 6CDE5B01               MOV BYTE PTR DS:[0x15BDE6C],AL
$+1D9    012AC099  ^ EB A6                     JMP SHORT PathOfEx.012AC041
$+1DB    012AC09B    CC                        INT3
$+1DC    012AC09C    CC                        INT3
$+1DD    012AC09D    CC                        INT3
$+1DE    012AC09E    CC                        INT3
$+1DF    012AC09F    CC                        INT3
$+1E0    012AC0A0    55                        PUSH EBP
$+1E1    012AC0A1    8BEC                      MOV EBP,ESP
$+1E3    012AC0A3    6A FE                     PUSH -0x2
$+1E5    012AC0A5    68 E8225901               PUSH PathOfEx.015922E8
$+1EA    012AC0AA    68 40D22401               PUSH PathOfEx.0124D240
$+1EF    012AC0AF    64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+1F5    012AC0B5    50                        PUSH EAX
$+1F6    012AC0B6    83EC 08                   SUB ESP,0x8
$+1F9    012AC0B9    53                        PUSH EBX
$+1FA    012AC0BA    56                        PUSH ESI
$+1FB    012AC0BB    57                        PUSH EDI
$+1FC    012AC0BC    A1 28985901               MOV EAX,DWORD PTR DS:[0x1599828]
$+201    012AC0C1    3145 F8                   XOR DWORD PTR SS:[EBP-0x8],EAX
$+204    012AC0C4    33C5                      XOR EAX,EBP
$+206    012AC0C6    50                        PUSH EAX
$+207    012AC0C7    8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
$+20A    012AC0CA    64:A3 00000000            MOV DWORD PTR FS:[0],EAX
$+210    012AC0D0    8965 E8                   MOV DWORD PTR SS:[EBP-0x18],ESP
$+213    012AC0D3    C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+21A    012AC0DA    66:0FEBC0                 POR MM0,MM0
$+21E    012AC0DE    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
$+225    012AC0E5    B0 01                     MOV AL,0x1
$+227    012AC0E7    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$+22A    012AC0EA    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+231    012AC0F1    59                        POP ECX
$+232    012AC0F2    5F                        POP EDI
$+233    012AC0F3    5E                        POP ESI
$+234    012AC0F4    5B                        POP EBX
$+235    012AC0F5    8BE5                      MOV ESP,EBP
$+237    012AC0F7    5D                        POP EBP
$+238    012AC0F8    C3                        RETN
$+239    012AC0F9    B8 01000000               MOV EAX,0x1
$+23E    012AC0FE    C3                        RETN
$+23F    012AC0FF    8B65 E8                   MOV ESP,DWORD PTR SS:[EBP-0x18]
$+242    012AC102    C745 FC FEFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x2
$+249    012AC109    32C0                      XOR AL,AL
$+24B    012AC10B    8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$+24E    012AC10E    64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+255    012AC115    59                        POP ECX
$+256    012AC116    5F                        POP EDI
$+257    012AC117    5E                        POP ESI
$+258    012AC118    5B                        POP EBX
$+259    012AC119    8BE5                      MOV ESP,EBP
$+25B    012AC11B    5D                        POP EBP
$+25C    012AC11C    C3                        RETN
$+25D    012AC11D    CC                        INT3
$+25E    012AC11E    CC                        INT3
$+25F    012AC11F    CC                        INT3
$+260    012AC120    56                        PUSH ESI
$+261    012AC121    8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
$+265    012AC125    3B7424 14                 CMP ESI,DWORD PTR SS:[ESP+0x14]
$+269    012AC129    74 32                     JE SHORT PathOfEx.012AC15D
$+26B    012AC12B    57                        PUSH EDI
$+26C    012AC12C    8B7C24 1C                 MOV EDI,DWORD PTR SS:[ESP+0x1C]
$+270    012AC130    8B4C24 0C                 MOV ECX,DWORD PTR SS:[ESP+0xC]
$+274    012AC134    8BC6                      MOV EAX,ESI
$+276    012AC136    C1E8 02                   SHR EAX,0x2
$+279    012AC139    83E6 03                   AND ESI,0x3
$+27C    012AC13C    8B51 08                   MOV EDX,DWORD PTR DS:[ECX+0x8]
$+27F    012AC13F    8B49 04                   MOV ECX,DWORD PTR DS:[ECX+0x4]
$+282    012AC142    4A                        DEC EDX
$+283    012AC143    23D0                      AND EDX,EAX
$+285    012AC145    8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+287    012AC147    8B0C91                    MOV ECX,DWORD PTR DS:[ECX+EDX*4]
$+28A    012AC14A    8904B1                    MOV DWORD PTR DS:[ECX+ESI*4],EAX
$+28D    012AC14D    8B7424 10                 MOV ESI,DWORD PTR SS:[ESP+0x10]
$+291    012AC151    46                        INC ESI
$+292    012AC152    897424 10                 MOV DWORD PTR SS:[ESP+0x10],ESI
$+296    012AC156    3B7424 18                 CMP ESI,DWORD PTR SS:[ESP+0x18]
$+29A    012AC15A  ^ 75 D4                     JNZ SHORT PathOfEx.012AC130
$+29C    012AC15C    5F                        POP EDI
$+29D    012AC15D    5E                        POP ESI
$+29E    012AC15E    C3                        RETN
$+29F    012AC15F    CC                        INT3
$+2A0    012AC160    56                        PUSH ESI
$+2A1    012AC161    8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
$+2A5    012AC165    3B7424 14                 CMP ESI,DWORD PTR SS:[ESP+0x14]
$+2A9    012AC169    74 38                     JE SHORT PathOfEx.012AC1A3
$+2AB    012AC16B    57                        PUSH EDI
$+2AC    012AC16C    8B7C24 1C                 MOV EDI,DWORD PTR SS:[ESP+0x1C]
$+2B0    012AC170    8B4C24 0C                 MOV ECX,DWORD PTR SS:[ESP+0xC]
$+2B4    012AC174    8BC6                      MOV EAX,ESI
$+2B6    012AC176    D1E8                      SHR EAX,1
$+2B8    012AC178    83E6 01                   AND ESI,0x1
$+2BB    012AC17B    8B51 08                   MOV EDX,DWORD PTR DS:[ECX+0x8]
$+2BE    012AC17E    4A                        DEC EDX
$+2BF    012AC17F    23D0                      AND EDX,EAX
$+2C1    012AC181    8B41 04                   MOV EAX,DWORD PTR DS:[ECX+0x4]
$+2C4    012AC184    8B0C90                    MOV ECX,DWORD PTR DS:[EAX+EDX*4]
$+2C7    012AC187    8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+2C9    012AC189    8904F1                    MOV DWORD PTR DS:[ECX+ESI*8],EAX
$+2CC    012AC18C    8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+2CF    012AC18F    8944F1 04                 MOV DWORD PTR DS:[ECX+ESI*8+0x4],EAX
$+2D3    012AC193    8B7424 10                 MOV ESI,DWORD PTR SS:[ESP+0x10]
$+2D7    012AC197    46                        INC ESI
$+2D8    012AC198    897424 10                 MOV DWORD PTR SS:[ESP+0x10],ESI
$+2DC    012AC19C    3B7424 18                 CMP ESI,DWORD PTR SS:[ESP+0x18]
$+2E0    012AC1A0  ^ 75 CE                     JNZ SHORT PathOfEx.012AC170
$+2E2    012AC1A2    5F                        POP EDI
$+2E3    012AC1A3    5E                        POP ESI
$+2E4    012AC1A4    C3                        RETN
$+2E5    012AC1A5    CC                        INT3
$+2E6    012AC1A6    CC                        INT3
$+2E7    012AC1A7    CC                        INT3
$+2E8    012AC1A8    CC                        INT3
$+2E9    012AC1A9    CC                        INT3
$+2EA    012AC1AA    CC                        INT3
$+2EB    012AC1AB    CC                        INT3
$+2EC    012AC1AC    CC                        INT3
$+2ED    012AC1AD    CC                        INT3
$+2EE    012AC1AE    CC                        INT3
$+2EF    012AC1AF    CC                        INT3
$+2F0    012AC1B0    51                        PUSH ECX
$+2F1    012AC1B1    FF7424 08                 PUSH DWORD PTR SS:[ESP+0x8]
$+2F5    012AC1B5    83EC 0C                   SUB ESP,0xC
$+2F8    012AC1B8    8BCC                      MOV ECX,ESP
$+2FA    012AC1BA    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+300    012AC1C0    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+307    012AC1C7    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+30B    012AC1CB    85C0                      TEST EAX,EAX
$+30D    012AC1CD    74 0A                     JE SHORT PathOfEx.012AC1D9
$+30F    012AC1CF    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+311    012AC1D1    85C0                      TEST EAX,EAX
$+313    012AC1D3    74 04                     JE SHORT PathOfEx.012AC1D9
$+315    012AC1D5    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+317    012AC1D7    8901                      MOV DWORD PTR DS:[ECX],EAX
$+319    012AC1D9    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+31D    012AC1DD    83EC 0C                   SUB ESP,0xC
$+320    012AC1E0    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+323    012AC1E3    8BCC                      MOV ECX,ESP
$+325    012AC1E5    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+32B    012AC1EB    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+332    012AC1F2    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+336    012AC1F6    85C0                      TEST EAX,EAX
$+338    012AC1F8    74 0A                     JE SHORT PathOfEx.012AC204
$+33A    012AC1FA    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+33C    012AC1FC    85C0                      TEST EAX,EAX
$+33E    012AC1FE    74 04                     JE SHORT PathOfEx.012AC204
$+340    012AC200    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+342    012AC202    8901                      MOV DWORD PTR DS:[ECX],EAX
$+344    012AC204    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+348    012AC208    83EC 0C                   SUB ESP,0xC
$+34B    012AC20B    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+34E    012AC20E    8BCC                      MOV ECX,ESP
$+350    012AC210    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+356    012AC216    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+35D    012AC21D    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+361    012AC221    85C0                      TEST EAX,EAX
$+363    012AC223    74 0A                     JE SHORT PathOfEx.012AC22F
$+365    012AC225    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+367    012AC227    85C0                      TEST EAX,EAX
$+369    012AC229    74 04                     JE SHORT PathOfEx.012AC22F
$+36B    012AC22B    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+36D    012AC22D    8901                      MOV DWORD PTR DS:[ECX],EAX
$+36F    012AC22F    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+373    012AC233    FF7424 30                 PUSH DWORD PTR SS:[ESP+0x30]
$+377    012AC237    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+37A    012AC23A    E8 11000000               CALL PathOfEx.012AC250
$+37F    012AC23F    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+383    012AC243    83C4 30                   ADD ESP,0x30
$+386    012AC246    C3                        RETN
$+387    012AC247    CC                        INT3
$+388    012AC248    CC                        INT3
$+389    012AC249    CC                        INT3
$+38A    012AC24A    CC                        INT3
$+38B    012AC24B    CC                        INT3
$+38C    012AC24C    CC                        INT3
$+38D    012AC24D    CC                        INT3
$+38E    012AC24E    CC                        INT3
$+38F    012AC24F    CC                        INT3
$+390    012AC250    8B5424 10                 MOV EDX,DWORD PTR SS:[ESP+0x10]
$+394    012AC254    56                        PUSH ESI
$+395    012AC255    8B7424 2C                 MOV ESI,DWORD PTR SS:[ESP+0x2C]
$+399    012AC259    3B5424 20                 CMP EDX,DWORD PTR SS:[ESP+0x20]
$+39D    012AC25D    74 62                     JE SHORT PathOfEx.012AC2C1
$+39F    012AC25F    57                        PUSH EDI
$+3A0    012AC260    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
$+3A4    012AC264    85C0                      TEST EAX,EAX
$+3A6    012AC266    75 04                     JNZ SHORT PathOfEx.012AC26C
$+3A8    012AC268    33FF                      XOR EDI,EDI
$+3AA    012AC26A    EB 02                     JMP SHORT PathOfEx.012AC26E
$+3AC    012AC26C    8B38                      MOV EDI,DWORD PTR DS:[EAX]
$+3AE    012AC26E    8B47 08                   MOV EAX,DWORD PTR DS:[EDI+0x8]
$+3B1    012AC271    8BCA                      MOV ECX,EDX
$+3B3    012AC273    48                        DEC EAX
$+3B4    012AC274    C1E9 02                   SHR ECX,0x2
$+3B7    012AC277    23C8                      AND ECX,EAX
$+3B9    012AC279    83E2 03                   AND EDX,0x3
$+3BC    012AC27C    8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+3BF    012AC27F    8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+3C2    012AC282    8B4C24 28                 MOV ECX,DWORD PTR SS:[ESP+0x28]
$+3C6    012AC286    8D3C90                    LEA EDI,DWORD PTR DS:[EAX+EDX*4]
$+3C9    012AC289    85C9                      TEST ECX,ECX
$+3CB    012AC28B    74 02                     JE SHORT PathOfEx.012AC28F
$+3CD    012AC28D    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+3CF    012AC28F    8B51 08                   MOV EDX,DWORD PTR DS:[ECX+0x8]
$+3D2    012AC292    8BC6                      MOV EAX,ESI
$+3D4    012AC294    8B49 04                   MOV ECX,DWORD PTR DS:[ECX+0x4]
$+3D7    012AC297    4A                        DEC EDX
$+3D8    012AC298    C1E8 02                   SHR EAX,0x2
$+3DB    012AC29B    83E6 03                   AND ESI,0x3
$+3DE    012AC29E    23D0                      AND EDX,EAX
$+3E0    012AC2A0    8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+3E2    012AC2A2    8B0C91                    MOV ECX,DWORD PTR DS:[ECX+EDX*4]
$+3E5    012AC2A5    8904B1                    MOV DWORD PTR DS:[ECX+ESI*4],EAX
$+3E8    012AC2A8    8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
$+3EC    012AC2AC    8B7424 30                 MOV ESI,DWORD PTR SS:[ESP+0x30]
$+3F0    012AC2B0    42                        INC EDX
$+3F1    012AC2B1    46                        INC ESI
$+3F2    012AC2B2    895424 18                 MOV DWORD PTR SS:[ESP+0x18],EDX
$+3F6    012AC2B6    897424 30                 MOV DWORD PTR SS:[ESP+0x30],ESI
$+3FA    012AC2BA    3B5424 24                 CMP EDX,DWORD PTR SS:[ESP+0x24]
$+3FE    012AC2BE  ^ 75 A0                     JNZ SHORT PathOfEx.012AC260
$+400    012AC2C0    5F                        POP EDI
$+401    012AC2C1    8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
$+405    012AC2C5    8B4C24 24                 MOV ECX,DWORD PTR SS:[ESP+0x24]
$+409    012AC2C9    C700 00000000             MOV DWORD PTR DS:[EAX],0x0
$+40F    012AC2CF    C740 04 00000000          MOV DWORD PTR DS:[EAX+0x4],0x0
$+416    012AC2D6    8970 08                   MOV DWORD PTR DS:[EAX+0x8],ESI
$+419    012AC2D9    85C9                      TEST ECX,ECX
$+41B    012AC2DB    74 0A                     JE SHORT PathOfEx.012AC2E7
$+41D    012AC2DD    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+41F    012AC2DF    85C9                      TEST ECX,ECX
$+421    012AC2E1    74 04                     JE SHORT PathOfEx.012AC2E7
$+423    012AC2E3    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+425    012AC2E5    8908                      MOV DWORD PTR DS:[EAX],ECX
$+427    012AC2E7    5E                        POP ESI
$+428    012AC2E8    C3                        RETN
$+429    012AC2E9    CC                        INT3
$+42A    012AC2EA    CC                        INT3
$+42B    012AC2EB    CC                        INT3
$+42C    012AC2EC    CC                        INT3
$+42D    012AC2ED    CC                        INT3
$+42E    012AC2EE    CC                        INT3
$+42F    012AC2EF    CC                        INT3
$+430    012AC2F0    51                        PUSH ECX
$+431    012AC2F1    FF7424 08                 PUSH DWORD PTR SS:[ESP+0x8]
$+435    012AC2F5    83EC 0C                   SUB ESP,0xC
$+438    012AC2F8    8BCC                      MOV ECX,ESP
$+43A    012AC2FA    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+440    012AC300    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+447    012AC307    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+44B    012AC30B    85C0                      TEST EAX,EAX
$+44D    012AC30D    74 0A                     JE SHORT PathOfEx.012AC319
$+44F    012AC30F    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+451    012AC311    85C0                      TEST EAX,EAX
$+453    012AC313    74 04                     JE SHORT PathOfEx.012AC319
$+455    012AC315    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+457    012AC317    8901                      MOV DWORD PTR DS:[ECX],EAX
$+459    012AC319    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+45D    012AC31D    83EC 0C                   SUB ESP,0xC
$+460    012AC320    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+463    012AC323    8BCC                      MOV ECX,ESP
$+465    012AC325    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+46B    012AC32B    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+472    012AC332    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+476    012AC336    85C0                      TEST EAX,EAX
$+478    012AC338    74 0A                     JE SHORT PathOfEx.012AC344
$+47A    012AC33A    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+47C    012AC33C    85C0                      TEST EAX,EAX
$+47E    012AC33E    74 04                     JE SHORT PathOfEx.012AC344
$+480    012AC340    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+482    012AC342    8901                      MOV DWORD PTR DS:[ECX],EAX
$+484    012AC344    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+488    012AC348    83EC 0C                   SUB ESP,0xC
$+48B    012AC34B    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+48E    012AC34E    8BCC                      MOV ECX,ESP
$+490    012AC350    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+496    012AC356    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+49D    012AC35D    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+4A1    012AC361    85C0                      TEST EAX,EAX
$+4A3    012AC363    74 0A                     JE SHORT PathOfEx.012AC36F
$+4A5    012AC365    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+4A7    012AC367    85C0                      TEST EAX,EAX
$+4A9    012AC369    74 04                     JE SHORT PathOfEx.012AC36F
$+4AB    012AC36B    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+4AD    012AC36D    8901                      MOV DWORD PTR DS:[ECX],EAX
$+4AF    012AC36F    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+4B3    012AC373    FF7424 30                 PUSH DWORD PTR SS:[ESP+0x30]
$+4B7    012AC377    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+4BA    012AC37A    E8 11000000               CALL PathOfEx.012AC390
$+4BF    012AC37F    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+4C3    012AC383    83C4 30                   ADD ESP,0x30
$+4C6    012AC386    C3                        RETN
$+4C7    012AC387    CC                        INT3
$+4C8    012AC388    CC                        INT3
$+4C9    012AC389    CC                        INT3
$+4CA    012AC38A    CC                        INT3
$+4CB    012AC38B    CC                        INT3
$+4CC    012AC38C    CC                        INT3
$+4CD    012AC38D    CC                        INT3
$+4CE    012AC38E    CC                        INT3
$+4CF    012AC38F    CC                        INT3
$+4D0    012AC390    8B5424 10                 MOV EDX,DWORD PTR SS:[ESP+0x10]
$+4D4    012AC394    56                        PUSH ESI
$+4D5    012AC395    8B7424 2C                 MOV ESI,DWORD PTR SS:[ESP+0x2C]
$+4D9    012AC399    3B5424 20                 CMP EDX,DWORD PTR SS:[ESP+0x20]
$+4DD    012AC39D    74 67                     JE SHORT PathOfEx.012AC406
$+4DF    012AC39F    57                        PUSH EDI
$+4E0    012AC3A0    8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
$+4E4    012AC3A4    85C0                      TEST EAX,EAX
$+4E6    012AC3A6    75 04                     JNZ SHORT PathOfEx.012AC3AC
$+4E8    012AC3A8    33FF                      XOR EDI,EDI
$+4EA    012AC3AA    EB 02                     JMP SHORT PathOfEx.012AC3AE
$+4EC    012AC3AC    8B38                      MOV EDI,DWORD PTR DS:[EAX]
$+4EE    012AC3AE    8B47 08                   MOV EAX,DWORD PTR DS:[EDI+0x8]
$+4F1    012AC3B1    8BCA                      MOV ECX,EDX
$+4F3    012AC3B3    48                        DEC EAX
$+4F4    012AC3B4    D1E9                      SHR ECX,1
$+4F6    012AC3B6    23C8                      AND ECX,EAX
$+4F8    012AC3B8    83E2 01                   AND EDX,0x1
$+4FB    012AC3BB    8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+4FE    012AC3BE    8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+501    012AC3C1    8D3CD0                    LEA EDI,DWORD PTR DS:[EAX+EDX*8]
$+504    012AC3C4    8B5424 28                 MOV EDX,DWORD PTR SS:[ESP+0x28]
$+508    012AC3C8    85D2                      TEST EDX,EDX
$+50A    012AC3CA    74 02                     JE SHORT PathOfEx.012AC3CE
$+50C    012AC3CC    8B12                      MOV EDX,DWORD PTR DS:[EDX]
$+50E    012AC3CE    8B4A 08                   MOV ECX,DWORD PTR DS:[EDX+0x8]
$+511    012AC3D1    8BC6                      MOV EAX,ESI
$+513    012AC3D3    D1E8                      SHR EAX,1
$+515    012AC3D5    49                        DEC ECX
$+516    012AC3D6    23C8                      AND ECX,EAX
$+518    012AC3D8    83E6 01                   AND ESI,0x1
$+51B    012AC3DB    8B42 04                   MOV EAX,DWORD PTR DS:[EDX+0x4]
$+51E    012AC3DE    8B0C88                    MOV ECX,DWORD PTR DS:[EAX+ECX*4]
$+521    012AC3E1    8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+523    012AC3E3    8904F1                    MOV DWORD PTR DS:[ECX+ESI*8],EAX
$+526    012AC3E6    8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+529    012AC3E9    8944F1 04                 MOV DWORD PTR DS:[ECX+ESI*8+0x4],EAX
$+52D    012AC3ED    8B5424 18                 MOV EDX,DWORD PTR SS:[ESP+0x18]
$+531    012AC3F1    8B7424 30                 MOV ESI,DWORD PTR SS:[ESP+0x30]
$+535    012AC3F5    42                        INC EDX
$+536    012AC3F6    46                        INC ESI
$+537    012AC3F7    895424 18                 MOV DWORD PTR SS:[ESP+0x18],EDX
$+53B    012AC3FB    897424 30                 MOV DWORD PTR SS:[ESP+0x30],ESI
$+53F    012AC3FF    3B5424 24                 CMP EDX,DWORD PTR SS:[ESP+0x24]
$+543    012AC403  ^ 75 9B                     JNZ SHORT PathOfEx.012AC3A0
$+545    012AC405    5F                        POP EDI
$+546    012AC406    8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
$+54A    012AC40A    8B4C24 24                 MOV ECX,DWORD PTR SS:[ESP+0x24]
$+54E    012AC40E    C700 00000000             MOV DWORD PTR DS:[EAX],0x0
$+554    012AC414    C740 04 00000000          MOV DWORD PTR DS:[EAX+0x4],0x0
$+55B    012AC41B    8970 08                   MOV DWORD PTR DS:[EAX+0x8],ESI
$+55E    012AC41E    85C9                      TEST ECX,ECX
$+560    012AC420    74 0A                     JE SHORT PathOfEx.012AC42C
$+562    012AC422    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+564    012AC424    85C9                      TEST ECX,ECX
$+566    012AC426    74 04                     JE SHORT PathOfEx.012AC42C
$+568    012AC428    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+56A    012AC42A    8908                      MOV DWORD PTR DS:[EAX],ECX
$+56C    012AC42C    5E                        POP ESI
$+56D    012AC42D    C3                        RETN
$+56E    012AC42E    CC                        INT3
$+56F    012AC42F    CC                        INT3
$+570    012AC430    51                        PUSH ECX
$+571    012AC431    FF7424 08                 PUSH DWORD PTR SS:[ESP+0x8]
$+575    012AC435    83EC 0C                   SUB ESP,0xC
$+578    012AC438    8BCC                      MOV ECX,ESP
$+57A    012AC43A    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+580    012AC440    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+587    012AC447    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+58B    012AC44B    85C0                      TEST EAX,EAX
$+58D    012AC44D    74 0A                     JE SHORT PathOfEx.012AC459
$+58F    012AC44F    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+591    012AC451    85C0                      TEST EAX,EAX
$+593    012AC453    74 04                     JE SHORT PathOfEx.012AC459
$+595    012AC455    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+597    012AC457    8901                      MOV DWORD PTR DS:[ECX],EAX
$+599    012AC459    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+59D    012AC45D    83EC 0C                   SUB ESP,0xC
$+5A0    012AC460    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+5A3    012AC463    8BCC                      MOV ECX,ESP
$+5A5    012AC465    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+5AB    012AC46B    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+5B2    012AC472    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+5B6    012AC476    85C0                      TEST EAX,EAX
$+5B8    012AC478    74 0A                     JE SHORT PathOfEx.012AC484
$+5BA    012AC47A    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+5BC    012AC47C    85C0                      TEST EAX,EAX
$+5BE    012AC47E    74 04                     JE SHORT PathOfEx.012AC484
$+5C0    012AC480    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+5C2    012AC482    8901                      MOV DWORD PTR DS:[ECX],EAX
$+5C4    012AC484    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+5C8    012AC488    83EC 0C                   SUB ESP,0xC
$+5CB    012AC48B    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+5CE    012AC48E    8BCC                      MOV ECX,ESP
$+5D0    012AC490    C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+5D6    012AC496    C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4],0x0
$+5DD    012AC49D    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+5E1    012AC4A1    85C0                      TEST EAX,EAX
$+5E3    012AC4A3    74 0A                     JE SHORT PathOfEx.012AC4AF
$+5E5    012AC4A5    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+5E7    012AC4A7    85C0                      TEST EAX,EAX
$+5E9    012AC4A9    74 04                     JE SHORT PathOfEx.012AC4AF
$+5EB    012AC4AB    8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+5ED    012AC4AD    8901                      MOV DWORD PTR DS:[ECX],EAX
$+5EF    012AC4AF    8B4424 3C                 MOV EAX,DWORD PTR SS:[ESP+0x3C]
$+5F3    012AC4B3    FF7424 30                 PUSH DWORD PTR SS:[ESP+0x30]
$+5F7    012AC4B7    8941 08                   MOV DWORD PTR DS:[ECX+0x8],EAX
$+5FA    012AC4BA    E8 11000000               CALL PathOfEx.012AC4D0
$+5FF    012AC4BF    8B4424 34                 MOV EAX,DWORD PTR SS:[ESP+0x34]
$+603    012AC4C3    83C4 30                   ADD ESP,0x30
$+606    012AC4C6    C3                        RETN
$+607    012AC4C7    CC                        INT3
$+608    012AC4C8    CC                        INT3
$+609    012AC4C9    CC                        INT3
$+60A    012AC4CA    CC                        INT3
$+60B    012AC4CB    CC                        INT3
$+60C    012AC4CC    CC                        INT3
$+60D    012AC4CD    CC                        INT3
$+60E    012AC4CE    CC                        INT3
$+60F    012AC4CF    CC                        INT3
$+610    012AC4D0    8B5424 1C                 MOV EDX,DWORD PTR SS:[ESP+0x1C]
$+614    012AC4D4    395424 10                 CMP DWORD PTR SS:[ESP+0x10],EDX
$+618    012AC4D8    74 68                     JE SHORT PathOfEx.012AC542
$+61A    012AC4DA    56                        PUSH ESI
$+61B    012AC4DB    57                        PUSH EDI
$+61C    012AC4DC    0F1F40 00                 NOP DWORD PTR DS:[EAX]
$+620    012AC4E0    8B4424 1C                 MOV EAX,DWORD PTR SS:[ESP+0x1C]
$+624    012AC4E4    4A                        DEC EDX
$+625    012AC4E5    895424 24                 MOV DWORD PTR SS:[ESP+0x24],EDX
$+629    012AC4E9    85C0                      TEST EAX,EAX
$+62B    012AC4EB    75 04                     JNZ SHORT PathOfEx.012AC4F1
$+62D    012AC4ED    33F6                      XOR ESI,ESI
$+62F    012AC4EF    EB 02                     JMP SHORT PathOfEx.012AC4F3
$+631    012AC4F1    8B30                      MOV ESI,DWORD PTR DS:[EAX]
$+633    012AC4F3    8B46 08                   MOV EAX,DWORD PTR DS:[ESI+0x8]
$+636    012AC4F6    8BCA                      MOV ECX,EDX
$+638    012AC4F8    48                        DEC EAX
$+639    012AC4F9    C1E9 02                   SHR ECX,0x2
$+63C    012AC4FC    23C8                      AND ECX,EAX
$+63E    012AC4FE    83E2 03                   AND EDX,0x3
$+641    012AC501    8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
$+644    012AC504    8B7424 30                 MOV ESI,DWORD PTR SS:[ESP+0x30]
$+648    012AC508    4E                        DEC ESI
$+649    012AC509    8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+64C    012AC50C    8B4C24 28                 MOV ECX,DWORD PTR SS:[ESP+0x28]
$+650    012AC510    897424 30                 MOV DWORD PTR SS:[ESP+0x30],ESI
$+654    012AC514    8D3C90                    LEA EDI,DWORD PTR DS:[EAX+EDX*4]
$+657    012AC517    85C9                      TEST ECX,ECX
$+659    012AC519    74 02                     JE SHORT PathOfEx.012AC51D
$+65B    012AC51B    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+65D    012AC51D    8B41 08                   MOV EAX,DWORD PTR DS:[ECX+0x8]
$+660    012AC520    8BD6                      MOV EDX,ESI
$+662    012AC522    8B49 04                   MOV ECX,DWORD PTR DS:[ECX+0x4]
$+665    012AC525    48                        DEC EAX
$+666    012AC526    C1EA 02                   SHR EDX,0x2
$+669    012AC529    83E6 03                   AND ESI,0x3
$+66C    012AC52C    23D0                      AND EDX,EAX
$+66E    012AC52E    8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+670    012AC530    8B0C91                    MOV ECX,DWORD PTR DS:[ECX+EDX*4]
$+673    012AC533    8904B1                    MOV DWORD PTR DS:[ECX+ESI*4],EAX
$+676    012AC536    8B5424 24                 MOV EDX,DWORD PTR SS:[ESP+0x24]
$+67A    012AC53A    395424 18                 CMP DWORD PTR SS:[ESP+0x18],EDX
$+67E    012AC53E  ^ 75 A0                     JNZ SHORT PathOfEx.012AC4E0
$+680    012AC540    5F                        POP EDI
$+681    012AC541    5E                        POP ESI
$+682    012AC542    8B5424 04                 MOV EDX,DWORD PTR SS:[ESP+0x4]
$+686    012AC546    8B4424 20                 MOV EAX,DWORD PTR SS:[ESP+0x20]
$+68A    012AC54A    C702 00000000             MOV DWORD PTR DS:[EDX],0x0
$+690    012AC550    C742 04 00000000          MOV DWORD PTR DS:[EDX+0x4],0x0
$+697    012AC557    85C0                      TEST EAX,EAX
$+699    012AC559    74 14                     JE SHORT PathOfEx.012AC56F
$+69B    012AC55B    8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+69D    012AC55D    85C9                      TEST ECX,ECX
$+69F    012AC55F    74 0E                     JE SHORT PathOfEx.012AC56F
$+6A1    012AC561    8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+6A3    012AC563    8BC2                      MOV EAX,EDX
$+6A5    012AC565    890A                      MOV DWORD PTR DS:[EDX],ECX
$+6A7    012AC567    8B4C24 28                 MOV ECX,DWORD PTR SS:[ESP+0x28]
$+6AB    012AC56B    894A 08                   MOV DWORD PTR DS:[EDX+0x8],ECX
$+6AE    012AC56E    C3                        RETN
$+6AF    012AC56F    8B4424 28                 MOV EAX,DWORD PTR SS:[ESP+0x28]
$+6B3    012AC573    8942 08                   MOV DWORD PTR DS:[EDX+0x8],EAX
$+6B6    012AC576    8BC2                      MOV EAX,EDX
$+6B8    012AC578    C3                        RETN
$+6B9    012AC579    CC                        INT3
*/