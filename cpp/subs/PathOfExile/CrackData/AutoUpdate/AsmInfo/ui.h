#pragma once
/*
@author zhp
@date 2017/2/26 19:41
@purpose for ui asm record
*/

//tag_Ui事件遍历算法。tag_响应ui事件
/*
$-B4     0049121F     CC                   INT3
$-B3     00491220     55                   PUSH EBP
$-B2     00491221     8BEC                 MOV EBP, ESP
$-B0     00491223     6A FF                PUSH -0x1
$-AE     00491225     68 6813D200          PUSH PathOfEx.00D21368
$-A9     0049122A     64:A1 00000000       MOV EAX, DWORD PTR FS:[0]
$-A3     00491230     50                   PUSH EAX                                 ; PathOfEx.00612420
$-A2     00491231     64:8925 00000000     MOV DWORD PTR FS:[0], ESP
$-9B     00491238     83EC 0C              SUB ESP, 0xC
$-98     0049123B     56                   PUSH ESI
$-97     0049123C     57                   PUSH EDI
$-96     0049123D     8BF9                 MOV EDI, ECX
$-94     0049123F     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4], 0x0
$-8D     00491246     8D4D E8              LEA ECX, DWORD PTR SS:[EBP-0x18]
$-8A     00491249     8B77 04              MOV ESI, DWORD PTR DS:[EDI+0x4]
$-87     0049124C     8B07                 MOV EAX, DWORD PTR DS:[EDI]
$-85     0049124E     8975 E8              MOV DWORD PTR SS:[EBP-0x18], ESI
$-82     00491251     8945 EC              MOV DWORD PTR SS:[EBP-0x14], EAX         ; PathOfEx.00612420
$-7F     00491254     890F                 MOV DWORD PTR DS:[EDI], ECX
$-7D     00491256     833E 00              CMP DWORD PTR DS:[ESI], 0x0
$-7A     00491259     74 48                JE SHORT PathOfEx.004912A3
$-78     0049125B     0F1F4400 00          NOP DWORD PTR DS:[EAX+EAX]
$-73     00491260     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
$-71     00491262     8BCE                 MOV ECX, ESI
$-6F     00491264     FF10                 CALL NEAR DWORD PTR DS:[EAX]
$-6D     00491266     0145 E8              ADD DWORD PTR SS:[EBP-0x18], EAX         ; PathOfEx.00612420
$-6A     00491269     83EC 18              SUB ESP, 0x18
$-67     0049126C     8BCC                 MOV ECX, ESP
$-65     0049126E     33C0                 XOR EAX, EAX                             ; PathOfEx.00612420
$-63     00491270     6A FF                PUSH -0x1
$-61     00491272     C741 14 07000000     MOV DWORD PTR DS:[ECX+0x14], 0x7
$-5A     00491279     C741 10 00000000     MOV DWORD PTR DS:[ECX+0x10], 0x0
$-53     00491280     50                   PUSH EAX                                 ; PathOfEx.00612420
$-52     00491281     66:8901              MOV WORD PTR DS:[ECX], AX
$-4F     00491284     8D45 08              LEA EAX, DWORD PTR SS:[EBP+0x8]
$-4C     00491287     50                   PUSH EAX                                 ; PathOfEx.00612420
$-4B     00491288     E8 B3C5FDFF          CALL PathOfEx.0046D840
$-46     0049128D     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
$-44     0049128F     8BCE                 MOV ECX, ESI
$-42     00491291     FF50 08              CALL NEAR DWORD PTR DS:[EAX+0x8]         ; crypt32.74C98500
$-3F     00491294     8B75 E8              MOV ESI, DWORD PTR SS:[EBP-0x18]
$-3C     00491297     85F6                 TEST ESI, ESI
$-3A     00491299     74 0A                JE SHORT PathOfEx.004912A5
$-38     0049129B     833E 00              CMP DWORD PTR DS:[ESI], 0x0
$-35     0049129E   ^ 75 C0                JNZ SHORT PathOfEx.00491260
$-33     004912A0     8B45 EC              MOV EAX, DWORD PTR SS:[EBP-0x14]
$-30     004912A3     8907                 MOV DWORD PTR DS:[EDI], EAX              ; PathOfEx.00612420
$-2E     004912A5     8B45 1C              MOV EAX, DWORD PTR SS:[EBP+0x1C]
$-2B     004912A8     83F8 08              CMP EAX, 0x8
$-28     004912AB     72 0F                JB SHORT PathOfEx.004912BC
$-26     004912AD     6A 02                PUSH 0x2
$-24     004912AF     40                   INC EAX                                  ; PathOfEx.00612420
$-23     004912B0     50                   PUSH EAX                                 ; PathOfEx.00612420
$-22     004912B1     FF75 08              PUSH DWORD PTR SS:[EBP+0x8]
$-1F     004912B4     E8 37B5FDFF          CALL PathOfEx.0046C7F0
$-1A     004912B9     83C4 0C              ADD ESP, 0xC
$-17     004912BC     8B4D F4              MOV ECX, DWORD PTR SS:[EBP-0xC]
$-14     004912BF     5F                   POP EDI                                  ; PathOfEx.00491313
$-13     004912C0     64:890D 00000000     MOV DWORD PTR FS:[0], ECX
$-C      004912C7     5E                   POP ESI                                  ; PathOfEx.00491313
$-B      004912C8     8BE5                 MOV ESP, EBP
$-9      004912CA     5D                   POP EBP                                  ; PathOfEx.00491313
$-8      004912CB     C2 1800              RETN 0x18
$-5      004912CE     CC                   INT3
$-4      004912CF     CC                   INT3
$-3      004912D0     83EC 0C              SUB ESP, 0xC								; tag_Ui事件遍历算法。tag_响应ui事件
$ ==>    004912D3     53                   PUSH EBX
$+1      004912D4     55                   PUSH EBP
$+2      004912D5     56                   PUSH ESI
$+3      004912D6     57                   PUSH EDI
$+4      004912D7     8BF9                 MOV EDI, ECX                             ; ecx=stCD_UiEventMgr*
$+6      004912D9     8D4C24 10            LEA ECX, DWORD PTR SS:[ESP+0x10]
$+A      004912DD     8B77 04              MOV ESI, DWORD PTR DS:[EDI+0x4]          ; 对象偏移4的地方存放着第一个stCD_UiEventBase*
$+D      004912E0     8B07                 MOV EAX, DWORD PTR DS:[EDI]
$+F      004912E2     897424 10            MOV DWORD PTR SS:[ESP+0x10], ESI
$+13     004912E6     894424 14            MOV DWORD PTR SS:[ESP+0x14], EAX         ; PathOfEx.00612420
$+17     004912EA     890F                 MOV DWORD PTR DS:[EDI], ECX              ; ecx=堆栈上的地址,把该地址写入到对象偏移0的地方
$+19     004912EC     833E 00              CMP DWORD PTR DS:[ESI], 0x0
$+1C     004912EF     74 33                JE SHORT PathOfEx.00491324
$+1E     004912F1     8B5C24 24            MOV EBX, DWORD PTR SS:[ESP+0x24]
$+22     004912F5     8B6C24 20            MOV EBP, DWORD PTR SS:[ESP+0x20]
$+26     004912F9     0F1F80 00000000      NOP DWORD PTR DS:[EAX]
$+2D     00491300     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
$+2F     00491302     8BCE                 MOV ECX, ESI
$+31     00491304     FF10                 CALL NEAR DWORD PTR DS:[EAX]             ; 返回下一个对象的偏移
$+33     00491306     014424 10            ADD DWORD PTR SS:[ESP+0x10], EAX         ; 地址加上偏移
$+37     0049130A     8BCE                 MOV ECX, ESI
$+39     0049130C     8B06                 MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00E72B2C
$+3B     0049130E     53                   PUSH EBX
$+3C     0049130F     55                   PUSH EBP									; ebp=第一个参数，ui对象
$+3D     00491310     FF50 08              CALL NEAR DWORD PTR DS:[EAX+0x8]         ; 响应ui事件
$+40     00491313     8B7424 10            MOV ESI, DWORD PTR SS:[ESP+0x10]         ; 读取下一个对象的地址
$+44     00491317     85F6                 TEST ESI, ESI
$+46     00491319     74 0B                JE SHORT PathOfEx.00491326
$+48     0049131B     833E 00              CMP DWORD PTR DS:[ESI], 0x0
$+4B     0049131E   ^ 75 E0                JNZ SHORT PathOfEx.00491300
$+4D     00491320     8B4424 14            MOV EAX, DWORD PTR SS:[ESP+0x14]
$+51     00491324     8907                 MOV DWORD PTR DS:[EDI], EAX              ; PathOfEx.00612420
$+53     00491326     5F                   POP EDI                                  ; PathOfEx.00491313
$+54     00491327     5E                   POP ESI                                  ; PathOfEx.00491313
$+55     00491328     5D                   POP EBP                                  ; PathOfEx.00491313
$+56     00491329     5B                   POP EBX                                  ; PathOfEx.00491313
$+57     0049132A     83C4 0C              ADD ESP, 0xC
$+5A     0049132D     C2 0800              RETN 0x8
$+5D     00491330     55                   PUSH EBP
$+5E     00491331     8BEC                 MOV EBP, ESP
$+60     00491333     6A FF                PUSH -0x1
$+62     00491335     68 8013D200          PUSH PathOfEx.00D21380
$+67     0049133A     64:A1 00000000       MOV EAX, DWORD PTR FS:[0]
$+6D     00491340     50                   PUSH EAX                                 ; PathOfEx.00612420
$+6E     00491341     64:8925 00000000     MOV DWORD PTR FS:[0], ESP
$+75     00491348     83EC 0C              SUB ESP, 0xC
$+78     0049134B     53                   PUSH EBX
$+79     0049134C     56                   PUSH ESI
$+7A     0049134D     8BF1                 MOV ESI, ECX
$+7C     0049134F     B8 ABAAAA2A          MOV EAX, 0x2AAAAAAB
$+81     00491354     57                   PUSH EDI
$+82     00491355     8B7D 08              MOV EDI, DWORD PTR SS:[EBP+0x8]
$+85     00491358     8965 F0              MOV DWORD PTR SS:[EBP-0x10], ESP
$+88     0049135B     C706 00000000        MOV DWORD PTR DS:[ESI], 0x0
$+8E     00491361     C746 04 00000000     MOV DWORD PTR DS:[ESI+0x4], 0x0
$+95     00491368     C746 08 00000000     MOV DWORD PTR DS:[ESI+0x8], 0x0
$+9C     0049136F     8B57 04              MOV EDX, DWORD PTR DS:[EDI+0x4]
$+9F     00491372     2B17                 SUB EDX, DWORD PTR DS:[EDI]
$+A1     00491374     F7EA                 IMUL EDX
$+A3     00491376     8975 EC              MOV DWORD PTR SS:[EBP-0x14], ESI
$+A6     00491379     C1FA 02              SAR EDX, 0x2
$+A9     0049137C     8BC2                 MOV EAX, EDX
$+AB     0049137E     C1E8 1F              SHR EAX, 0x1F
$+AE     00491381     03C2                 ADD EAX, EDX
$+B0     00491383     50                   PUSH EAX                                 ; PathOfEx.00612420
$+B1     00491384     E8 A7FBFDFF          CALL PathOfEx.00470F30
$+B6     00491389     84C0                 TEST AL, AL
$+B8     0049138B     74 22                JE SHORT PathOfEx.004913AF
$+BA     0049138D     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4], 0x0
$+C1     00491394     8B57 04              MOV EDX, DWORD PTR DS:[EDI+0x4]
$+C4     00491397     51                   PUSH ECX
$+C5     00491398     C645 08 00           MOV BYTE PTR SS:[EBP+0x8], 0x0
$+C9     0049139C     FF75 08              PUSH DWORD PTR SS:[EBP+0x8]
$+CC     0049139F     51                   PUSH ECX
$+CD     004913A0     FF36                 PUSH DWORD PTR DS:[ESI]                  ; PathOfEx.00E72B2C
$+CF     004913A2     8B0F                 MOV ECX, DWORD PTR DS:[EDI]
$+D1     004913A4     E8 0745FEFF          CALL PathOfEx.004758B0
$+D6     004913A9     83C4 10              ADD ESP, 0x10
$+D9     004913AC     8946 04              MOV DWORD PTR DS:[ESI+0x4], EAX          ; PathOfEx.00612420
$+DC     004913AF     8B4D F4              MOV ECX, DWORD PTR SS:[EBP-0xC]
$+DF     004913B2     8BC6                 MOV EAX, ESI
$+E1     004913B4     5F                   POP EDI                                  ; PathOfEx.00491313
$+E2     004913B5     5E                   POP ESI                                  ; PathOfEx.00491313
$+E3     004913B6     64:890D 00000000     MOV DWORD PTR FS:[0], ECX
$+EA     004913BD     5B                   POP EBX                                  ; PathOfEx.00491313
$+EB     004913BE     8BE5                 MOV ESP, EBP
$+ED     004913C0     5D                   POP EBP                                  ; PathOfEx.00491313
$+EE     004913C1     C2 0400              RETN 0x4
$+F1     004913C4     8B4D EC              MOV ECX, DWORD PTR SS:[EBP-0x14]
$+F4     004913C7     E8 0487FEFF          CALL PathOfEx.00479AD0
$+F9     004913CC     6A 00                PUSH 0x0
$+FB     004913CE     6A 00                PUSH 0x0
$+FD     004913D0     E8 DE228000          CALL PathOfEx.00C936B3
$+102    004913D5     CC                   INT3
*/
/**/
//tag_状态处理ui逻辑的函数
/*
$-7      00A17909      CC                         INT3
$-6      00A1790A      CC                         INT3
$-5      00A1790B      CC                         INT3
$-4      00A1790C      CC                         INT3
$-3      00A1790D      CC                         INT3
$-2      00A1790E      CC                         INT3                                     ; tag_状态处理ui逻辑的函数
$-1      00A1790F      CC                         INT3                                     ; 这个函数是State的ui处理逻辑的
$ ==>    00A17910      55                         PUSH EBP                                 ; ecx=stCD_StateUiLogic*
$+1      00A17911      8BEC                       MOV EBP,ESP
$+3      00A17913      83E4 F8                    AND ESP,0xFFFFFFF8
$+6      00A17916      51                         PUSH ECX
$+7      00A17917      53                         PUSH EBX
$+8      00A17918      56                         PUSH ESI
$+9      00A17919      8BF1                       MOV ESI,ECX
$+B      00A1791B      B9 10901301                MOV ECX,PathOfEx.01139010
$+10     00A17920      57                         PUSH EDI
$+11     00A17921      E8 1A6FFEFF                CALL PathOfEx.009FE840
$+16     00A17926      85C0                       TEST EAX,EAX
$+18     00A17928      0F85 E6000000              JNZ PathOfEx.00A17A14
$+1E     00A1792E      8B86 B40A0000              MOV EAX,DWORD PTR DS:[ESI+0xAB4]
$+24     00A17934      85C0                       TEST EAX,EAX
$+26     00A17936      74 07                      JE SHORT PathOfEx.00A1793F
$+28     00A17938      C680 54070000 00           MOV BYTE PTR DS:[EAX+0x754],0x0
$+2F     00A1793F      8B86 AC0A0000              MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+35     00A17945      32DB                       XOR BL,BL
$+37     00A17947      85C0                       TEST EAX,EAX
$+39     00A17949      74 0D                      JE SHORT PathOfEx.00A17958
$+3B     00A1794B      8A98 54070000              MOV BL,BYTE PTR DS:[EAX+0x754]
$+41     00A17951      C680 54070000 00           MOV BYTE PTR DS:[EAX+0x754],0x0
$+48     00A17958      FFB6 A40A0000              PUSH DWORD PTR DS:[ESI+0xAA4]            ; 偏移aa4,g_so_ui_logic_top_ui_
$+4E     00A1795E      8BCE                       MOV ECX,ESI							   ; 下面这个函数只被此函数调用
$+50     00A17960      E8 9BE6FFFF                CALL PathOfEx.00A16000                   ; ui逻辑的顶层函数，不会被其他ui调用
	$-2      00A15FFE     CC                         INT3
	$-1      00A15FFF     CC                         INT3                                          ; ui逻辑的顶层函数，不会被其他ui调用
	$ ==>    00A16000     83EC 10                    SUB ESP,0x10
	$+3      00A16003     A1 04901301                MOV EAX,DWORD PTR DS:[0x1139004]
	$+8      00A16008     8D1424                     LEA EDX,DWORD PTR SS:[ESP]
	$+B      00A1600B     F3:0F100D 2C5FFE00         MOVSS XMM1,DWORD PTR DS:[0xFE5F2C]
	$+13     00A16013     F3:0F1015 305FFE00         MOVSS XMM2,DWORD PTR DS:[0xFE5F30]
	$+1B     00A1601B     F3:0F110C24                MOVSS DWORD PTR SS:[ESP],XMM1                 ; 0，可能是左顶点x
	$+20     00A16020     66:0F6EC0                  MOVD MM0,EAX
	$+24     00A16024     F30FE6C0                   CVTDQ2PD XMM0,XMM0
	$+28     00A16028     C1E8 1F                    SHR EAX,0x1F
	$+2B     00A1602B     F3:0F115424 04             MOVSS DWORD PTR SS:[ESP+0x4],XMM2             ; 0，可能是左顶点y
	$+31     00A16031     F20F5804C5 40FCF300        ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
	$+3A     00A1603A     A1 08901301                MOV EAX,DWORD PTR DS:[0x1139008]
	$+3F     00A1603F     660F5AC0                   CVTPD2PS XMM0,XMM0
	$+43     00A16043     F3:0F58C1                  ADDSS XMM0,XMM1
	$+47     00A16047     F3:0F114424 08             MOVSS DWORD PTR SS:[ESP+0x8],XMM0             ; 分辨率宽度，右底点x，比如1024
	$+4D     00A1604D     66:0F6EC0                  MOVD MM0,EAX
	$+51     00A16051     F30FE6C0                   CVTDQ2PD XMM0,XMM0
	$+55     00A16055     C1E8 1F                    SHR EAX,0x1F
	$+58     00A16058     F20F5804C5 40FCF300        ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
	$+61     00A16061     8B4424 14                  MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+65     00A16065     FFB0 34080000              PUSH DWORD PTR DS:[EAX+0x834]
	$+6B     00A1606B     F3:0F1098 28080000         MOVSS XMM3,DWORD PTR DS:[EAX+0x828]
	$+73     00A16073     660F5AC0                   CVTPD2PS XMM0,XMM0
	$+77     00A16077     83EC 08                    SUB ESP,0x8
	$+7A     00A1607A     F3:0F58C2                  ADDSS XMM0,XMM2
	$+7E     00A1607E     F3:0F114424 18             MOVSS DWORD PTR SS:[ESP+0x18],XMM0            ; 分辨率高度，右底点y，比如768
	$+84     00A16084     F3:0F1080 30080000         MOVSS XMM0,DWORD PTR DS:[EAX+0x830]
	$+8C     00A1608C     F3:0F114424 04             MOVSS DWORD PTR SS:[ESP+0x4],XMM0
	$+92     00A16092     F3:0F1080 2C080000         MOVSS XMM0,DWORD PTR DS:[EAX+0x82C]
	$+9A     00A1609A     F3:0F110424                MOVSS DWORD PTR SS:[ESP],XMM0
	$+9F     00A1609F     52                         PUSH EDX                                      ; edx=分辨率信息
	$+A0     00A160A0     50                         PUSH EAX                                      ; eax=stCD_StateUiLogic.main_ui_
	$+A1     00A160A1     E8 0A000000                CALL PathOfEx.00A160B0                        ; tag_根据位置信息处理ui对象
	$+A6     00A160A6     83C4 10                    ADD ESP,0x10
	$+A9     00A160A9     C2 0400                    RETN 0x4
	$+AC     00A160AC     CC                         INT3
	$+AD     00A160AD     CC                         INT3
	$+AE     00A160AE     CC                         INT3
	$+AF     00A160AF     CC                         INT3                                          ; tag_根据位置信息处理ui对象
	$+B0     00A160B0     55                         PUSH EBP                                      ; 接收两个参数,p1是ui对象,p2是stRectFloat*
	$+B1     00A160B1     8BEC                       MOV EBP,ESP
	$+B3     00A160B3     83E4 F0                    AND ESP,0xFFFFFFF0
	$+B6     00A160B6     83EC 68                    SUB ESP,0x68
	$+B9     00A160B9     56                         PUSH ESI
	$+BA     00A160BA     57                         PUSH EDI
	$+BB     00A160BB     8B7D 08                    MOV EDI,DWORD PTR SS:[EBP+0x8]                ; edi=ui对象
$+55     00A17965      8B86 AC0A0000              MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+5B     00A1796B      85C0                       TEST EAX,EAX
$+5D     00A1796D      74 1D                      JE SHORT PathOfEx.00A1798C
$+5F     00A1796F      8898 54070000              MOV BYTE PTR DS:[EAX+0x754],BL
$+65     00A17975      8B86 AC0A0000              MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+6B     00A1797B      80B8 54070000 00           CMP BYTE PTR DS:[EAX+0x754],0x0
$+72     00A17982      74 08                      JE SHORT PathOfEx.00A1798C
$+74     00A17984      50                         PUSH EAX
$+75     00A17985      8BCE                       MOV ECX,ESI
$+77     00A17987      E8 74E6FFFF                CALL PathOfEx.00A16000
$+7C     00A1798C      8BBE C80A0000              MOV EDI,DWORD PTR DS:[ESI+0xAC8]
$+82     00A17992      85FF                       TEST EDI,EDI
$+84     00A17994      74 24                      JE SHORT PathOfEx.00A179BA
$+86     00A17996      80BF 54070000 00           CMP BYTE PTR DS:[EDI+0x754],0x0
$+8D     00A1799D      74 1B                      JE SHORT PathOfEx.00A179BA
$+8F     00A1799F      8B8F 5C070000              MOV ECX,DWORD PTR DS:[EDI+0x75C]
$+95     00A179A5      85C9                       TEST ECX,ECX
$+97     00A179A7      74 11                      JE SHORT PathOfEx.00A179BA
$+99     00A179A9      E8 3256FDFF                CALL PathOfEx.009ECFE0
$+9E     00A179AE      84C0                       TEST AL,AL
$+A0     00A179B0      74 08                      JE SHORT PathOfEx.00A179BA
$+A2     00A179B2      57                         PUSH EDI
$+A3     00A179B3      8BCE                       MOV ECX,ESI
$+A5     00A179B5      E8 46E6FFFF                CALL PathOfEx.00A16000
$+AA     00A179BA      8B86 B40A0000              MOV EAX,DWORD PTR DS:[ESI+0xAB4]
$+B0     00A179C0      85C0                       TEST EAX,EAX
$+B2     00A179C2      74 14                      JE SHORT PathOfEx.00A179D8
$+B4     00A179C4      C680 54070000 01           MOV BYTE PTR DS:[EAX+0x754],0x1
$+BB     00A179CB      8BCE                       MOV ECX,ESI
$+BD     00A179CD      FFB6 B40A0000              PUSH DWORD PTR DS:[ESI+0xAB4]
$+C3     00A179D3      E8 28E6FFFF                CALL PathOfEx.00A16000
$+C8     00A179D8      8BCE                       MOV ECX,ESI
$+CA     00A179DA      E8 81E9FFFF                CALL PathOfEx.00A16360
$+CF     00A179DF      B9 10901301                MOV ECX,PathOfEx.01139010
$+D4     00A179E4      E8 4784FEFF                CALL PathOfEx.009FFE30
$+D9     00A179E9      803D 68901301 00           CMP BYTE PTR DS:[0x1139068],0x0
$+E0     00A179F0      75 22                      JNZ SHORT PathOfEx.00A17A14
$+E2     00A179F2      A1 1C901301                MOV EAX,DWORD PTR DS:[0x113901C]
$+E7     00A179F7      C705 64901301 00000000     MOV DWORD PTR DS:[0x1139064],0x0
$+F1     00A17A01      85C0                       TEST EAX,EAX
$+F3     00A17A03      74 0F                      JE SHORT PathOfEx.00A17A14
$+F5     00A17A05      8378 08 00                 CMP DWORD PTR DS:[EAX+0x8],0x0
$+F9     00A17A09      75 09                      JNZ SHORT PathOfEx.00A17A14
$+FB     00A17A0B      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$+FE     00A17A0E      50                         PUSH EAX
$+FF     00A17A0F      8B08                       MOV ECX,DWORD PTR DS:[EAX]
$+101    00A17A11      FF51 30                    CALL DWORD PTR DS:[ECX+0x30]
$+104    00A17A14      5F                         POP EDI
$+105    00A17A15      5E                         POP ESI
$+106    00A17A16      5B                         POP EBX
$+107    00A17A17      8BE5                       MOV ESP,EBP
$+109    00A17A19      5D                         POP EBP
$+10A    00A17A1A      C2 0400                    RETN 0x4
$+10D    00A17A1D      CC                         INT3
$+10E    00A17A1E      CC                         INT3
$+10F    00A17A1F      CC                         INT3
$+110    00A17A20      55                         PUSH EBP
$+111    00A17A21      8BEC                       MOV EBP,ESP
$+113    00A17A23      83E4 F8                    AND ESP,0xFFFFFFF8
$+116    00A17A26      83EC 3C                    SUB ESP,0x3C
$+119    00A17A29      53                         PUSH EBX
$+11A    00A17A2A      8B5D 0C                    MOV EBX,DWORD PTR SS:[EBP+0xC]
$+11D    00A17A2D      56                         PUSH ESI
$+11E    00A17A2E      57                         PUSH EDI
$+11F    00A17A2F      8BF9                       MOV EDI,ECX
$+121    00A17A31      32C9                       XOR CL,CL
$+123    00A17A33      884C24 0F                  MOV BYTE PTR SS:[ESP+0xF],CL
$+127    00A17A37      81FB 05010000              CMP EBX,0x105
$+12D    00A17A3D      0F87 29010000              JA PathOfEx.00A17B6C
$+133    00A17A43      81FB 04010000              CMP EBX,0x104
$+139    00A17A49      73 1D                      JNB SHORT PathOfEx.00A17A68
$+13B    00A17A4B      83FB 1C                    CMP EBX,0x1C
$+13E    00A17A4E      74 31                      JE SHORT PathOfEx.00A17A81
$+140    00A17A50      81FB FF000000              CMP EBX,0xFF
$+146    00A17A56      0F86 8B020000              JBE PathOfEx.00A17CE7
$+14C    00A17A5C      81FB 02010000              CMP EBX,0x102
$+152    00A17A62      0F87 7F020000              JA PathOfEx.00A17CE7
$+158    00A17A68      FF75 14                    PUSH DWORD PTR SS:[EBP+0x14]
$+15B    00A17A6B      8BCF                       MOV ECX,EDI
$+15D    00A17A6D      FF75 10                    PUSH DWORD PTR SS:[EBP+0x10]
$+160    00A17A70      53                         PUSH EBX
$+161    00A17A71      E8 DA040000                CALL PathOfEx.00A17F50
$+166    00A17A76      8AC8                       MOV CL,AL
$+168    00A17A78      5F                         POP EDI
$+169    00A17A79      5E                         POP ESI
$+16A    00A17A7A      5B                         POP EBX
$+16B    00A17A7B      8BE5                       MOV ESP,EBP
$+16D    00A17A7D      5D                         POP EBP
$+16E    00A17A7E      C2 1000                    RETN 0x10
$+171    00A17A81      837D 10 00                 CMP DWORD PTR SS:[EBP+0x10],0x0
$+175    00A17A85      74 3B                      JE SHORT PathOfEx.00A17AC2
$+177    00A17A87      8B87 B00A0000              MOV EAX,DWORD PTR DS:[EDI+0xAB0]
$+17D    00A17A8D      85C0                       TEST EAX,EAX
$+17F    00A17A8F      74 12                      JE SHORT PathOfEx.00A17AA3
$+181    00A17A91      50                         PUSH EAX
$+182    00A17A92      8BCF                       MOV ECX,EDI
$+184    00A17A94      E8 97FCFFFF                CALL PathOfEx.00A17730
$+189    00A17A99      C787 B00A0000 00000000     MOV DWORD PTR DS:[EDI+0xAB0],0x0
$+193    00A17AA3      6A 01                      PUSH 0x1
$+195    00A17AA5      6A 00                      PUSH 0x0
$+197    00A17AA7      68 00020000                PUSH 0x200
$+19C    00A17AAC      8BCF                       MOV ECX,EDI
$+19E    00A17AAE      E8 4D020000                CALL PathOfEx.00A17D00
$+1A3    00A17AB3      8A4C24 0F                  MOV CL,BYTE PTR SS:[ESP+0xF]
$+1A7    00A17AB7      8AC1                       MOV AL,CL
$+1A9    00A17AB9      5F                         POP EDI
$+1AA    00A17ABA      5E                         POP ESI
$+1AB    00A17ABB      5B                         POP EBX
$+1AC    00A17ABC      8BE5                       MOV ESP,EBP
$+1AE    00A17ABE      5D                         POP EBP
$+1AF    00A17ABF      C2 1000                    RETN 0x10
$+1B2    00A17AC2      8BB7 B80A0000              MOV ESI,DWORD PTR DS:[EDI+0xAB8]
$+1B8    00A17AC8      85F6                       TEST ESI,ESI
$+1BA    00A17ACA      74 6E                      JE SHORT PathOfEx.00A17B3A
$+1BC    00A17ACC      80BE 56070000 00           CMP BYTE PTR DS:[ESI+0x756],0x0
$+1C3    00A17AD3      74 65                      JE SHORT PathOfEx.00A17B3A
$+1C5    00A17AD5      66:0F6E87 D80A0000         MOVD MM0,DWORD PTR DS:[EDI+0xAD8]
$+1CD    00A17ADD      8D4424 18                  LEA EAX,DWORD PTR SS:[ESP+0x18]
$+1D1    00A17AE1      66:0F6E97 D80A0000         MOVD MM2,DWORD PTR DS:[EDI+0xAD8]
$+1D9    00A17AE9      8BCE                       MOV ECX,ESI
$+1DB    00A17AEB      66:0F6E9F DC0A0000         MOVD MM3,DWORD PTR DS:[EDI+0xADC]
$+1E3    00A17AF3      0F5BC0                     CVTDQ2PS XMM0,XMM0
$+1E6    00A17AF6      50                         PUSH EAX
$+1E7    00A17AF7      0F5BD2                     CVTDQ2PS XMM2,XMM2
$+1EA    00A17AFA      F3:0F114424 14             MOVSS DWORD PTR SS:[ESP+0x14],XMM0
$+1F0    00A17B00      66:0F6E87 DC0A0000         MOVD MM0,DWORD PTR DS:[EDI+0xADC]
$+1F8    00A17B08      0F5BC0                     CVTDQ2PS XMM0,XMM0
$+1FB    00A17B0B      0F5BDB                     CVTDQ2PS XMM3,XMM3
$+1FE    00A17B0E      F3:0F114424 18             MOVSS DWORD PTR SS:[ESP+0x18],XMM0
$+204    00A17B14      E8 37B4ABFF                CALL PathOfEx.004D2F50
$+209    00A17B19      83EC 0C                    SUB ESP,0xC
$+20C    00A17B1C      8D4424 1C                  LEA EAX,DWORD PTR SS:[ESP+0x1C]
$+210    00A17B20      8D4C24 34                  LEA ECX,DWORD PTR SS:[ESP+0x34]
$+214    00A17B24      50                         PUSH EAX
$+215    00A17B25      8D4424 28                  LEA EAX,DWORD PTR SS:[ESP+0x28]
$+219    00A17B29      50                         PUSH EAX
$+21A    00A17B2A      E8 B1F0C4FF                CALL PathOfEx.00666BE0
$+21F    00A17B2F      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+221    00A17B31      51                         PUSH ECX
$+222    00A17B32      8BCE                       MOV ECX,ESI
$+224    00A17B34      FF90 E8000000              CALL DWORD PTR DS:[EAX+0xE8]
$+22A    00A17B3A      8B87 AC0A0000              MOV EAX,DWORD PTR DS:[EDI+0xAAC]
$+230    00A17B40      8BCF                       MOV ECX,EDI
$+232    00A17B42      6A 00                      PUSH 0x0
$+234    00A17B44      8987 B00A0000              MOV DWORD PTR DS:[EDI+0xAB0],EAX
$+23A    00A17B4A      E8 E1FBFFFF                CALL PathOfEx.00A17730
$+23F    00A17B4F      8BCF                       MOV ECX,EDI
$+241    00A17B51      E8 CAF8FFFF                CALL PathOfEx.00A17420
$+246    00A17B56      8BCF                       MOV ECX,EDI
$+248    00A17B58      E8 D3E0FFFF                CALL PathOfEx.00A15C30
$+24D    00A17B5D      8A4C24 0F                  MOV CL,BYTE PTR SS:[ESP+0xF]
$+251    00A17B61      8AC1                       MOV AL,CL
$+253    00A17B63      5F                         POP EDI
$+254    00A17B64      5E                         POP ESI
$+255    00A17B65      5B                         POP EBX
$+256    00A17B66      8BE5                       MOV ESP,EBP
$+258    00A17B68      5D                         POP EBP
$+259    00A17B69      C2 1000                    RETN 0x10
*/
/**/
//tag_开始调用StateUiLogic去处理UI逻辑
/*
$-12A    006A815F     CC                      INT3
$-129    006A8160     55                      PUSH EBP							;tag_开始调用StateUiLogic去处理UI逻辑
$-128    006A8161     8BEC                    MOV EBP,ESP
$-126    006A8163     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-120    006A8169     6A FF                   PUSH -0x1
$-11E    006A816B     68 D223D400             PUSH PathOfEx.00D423D2
$-119    006A8170     50                      PUSH EAX
$-118    006A8171     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-111    006A8178     83EC 08                 SUB ESP,0x8
$-10E    006A817B     53                      PUSH EBX
$-10D    006A817C     8BD9                    MOV EBX,ECX
$-10B    006A817E     8B43 10                 MOV EAX,DWORD PTR DS:[EBX+0x10]
$-108    006A8181     8D53 10                 LEA EDX,DWORD PTR DS:[EBX+0x10]
$-105    006A8184     3B42 04                 CMP EAX,DWORD PTR DS:[EDX+0x4]
$-102    006A8187     0F84 7A010000           JE PathOfEx.006A8307
$-FC     006A818D     FF43 1C                 INC DWORD PTR DS:[EBX+0x1C]
$-F9     006A8190     56                      PUSH ESI
$-F8     006A8191     8B73 14                 MOV ESI,DWORD PTR DS:[EBX+0x14]
$-F5     006A8194     83EE 08                 SUB ESI,0x8
$-F2     006A8197     57                      PUSH EDI
$-F1     006A8198     3B32                    CMP ESI,DWORD PTR DS:[EDX]
$-EF     006A819A     0F84 97000000           JE PathOfEx.006A8237
$-E9     006A81A0     8B7E 04                 MOV EDI,DWORD PTR DS:[ESI+0x4]
$-E6     006A81A3     8B0E                    MOV ECX,DWORD PTR DS:[ESI]
$-E4     006A81A5     85FF                    TEST EDI,EDI
$-E2     006A81A7     74 04                   JE SHORT PathOfEx.006A81AD
$-E0     006A81A9     F0:FF47 04              LOCK INC DWORD PTR DS:[EDI+0x4]
$-DC     006A81AD     85C9                    TEST ECX,ECX
$-DA     006A81AF     0F94C0                  SETE AL
$-D7     006A81B2     84C0                    TEST AL,AL
$-D5     006A81B4     74 09                   JE SHORT PathOfEx.006A81BF
$-D3     006A81B6     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$-CC     006A81BD     EB 0D                   JMP SHORT PathOfEx.006A81CC
$-CA     006A81BF     8079 05 00              CMP BYTE PTR DS:[ECX+0x5],0x0
$-C6     006A81C3     75 3F                   JNZ SHORT PathOfEx.006A8204
$-C4     006A81C5     C745 FC 03000000        MOV DWORD PTR SS:[EBP-0x4],0x3
$-BD     006A81CC     85FF                    TEST EDI,EDI
$-BB     006A81CE     74 24                   JE SHORT PathOfEx.006A81F4
$-B9     006A81D0     83C8 FF                 OR EAX,0xFFFFFFFF
$-B6     006A81D3     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$-B1     006A81D8     75 1A                   JNZ SHORT PathOfEx.006A81F4
$-AF     006A81DA     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$-AD     006A81DC     8BCF                    MOV ECX,EDI
$-AB     006A81DE     FF10                    CALL DWORD PTR DS:[EAX]
$-A9     006A81E0     83C8 FF                 OR EAX,0xFFFFFFFF
$-A6     006A81E3     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$-A1     006A81E8     75 07                   JNZ SHORT PathOfEx.006A81F1
$-9F     006A81EA     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$-9D     006A81EC     8BCF                    MOV ECX,EDI
$-9B     006A81EE     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-98     006A81F1     8D53 10                 LEA EDX,DWORD PTR DS:[EBX+0x10]
$-95     006A81F4     83EE 08                 SUB ESI,0x8
$-92     006A81F7     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$-8B     006A81FE     3B32                    CMP ESI,DWORD PTR DS:[EDX]
$-89     006A8200   ^ 75 9E                   JNZ SHORT PathOfEx.006A81A0
$-87     006A8202     EB 33                   JMP SHORT PathOfEx.006A8237
$-85     006A8204     C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4],0x2
$-7E     006A820B     85FF                    TEST EDI,EDI
$-7C     006A820D     74 21                   JE SHORT PathOfEx.006A8230
$-7A     006A820F     83C8 FF                 OR EAX,0xFFFFFFFF
$-77     006A8212     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$-72     006A8217     75 17                   JNZ SHORT PathOfEx.006A8230
$-70     006A8219     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$-6E     006A821B     8BCF                    MOV ECX,EDI
$-6C     006A821D     FF10                    CALL DWORD PTR DS:[EAX]
$-6A     006A821F     83C8 FF                 OR EAX,0xFFFFFFFF
$-67     006A8222     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$-62     006A8227     75 07                   JNZ SHORT PathOfEx.006A8230
$-60     006A8229     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$-5E     006A822B     8BCF                    MOV ECX,EDI
$-5C     006A822D     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-59     006A8230     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$-52     006A8237     3B73 14                 CMP ESI,DWORD PTR DS:[EBX+0x14]
$-4F     006A823A     0F84 8B000000           JE PathOfEx.006A82CB
$-49     006A8240     8B7E 04                 MOV EDI,DWORD PTR DS:[ESI+0x4]
$-46     006A8243     8B0E                    MOV ECX,DWORD PTR DS:[ESI]
$-44     006A8245     85FF                    TEST EDI,EDI
$-42     006A8247     74 04                   JE SHORT PathOfEx.006A824D
$-40     006A8249     F0:FF47 04              LOCK INC DWORD PTR DS:[EDI+0x4]
$-3C     006A824D     897D F0                 MOV DWORD PTR SS:[EBP-0x10],EDI
$-39     006A8250     894D EC                 MOV DWORD PTR SS:[EBP-0x14],ECX
$-36     006A8253     85C9                    TEST ECX,ECX
$-34     006A8255     C745 FC 05000000        MOV DWORD PTR SS:[EBP-0x4],0x5
$-2D     006A825C     0F94C0                  SETE AL
$-2A     006A825F     84C0                    TEST AL,AL
$-28     006A8261     74 09                   JE SHORT PathOfEx.006A826C
$-26     006A8263     C745 FC 06000000        MOV DWORD PTR SS:[EBP-0x4],0x6
$-1F     006A826A     EB 27                   JMP SHORT PathOfEx.006A8293
$-1D     006A826C     F20F1045 14             MOVSD XMM0,QWORD PTR SS:[EBP+0x14]
$-18     006A8271     83EC 10                 SUB ESP,0x10
$-15     006A8274     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-13     006A8276     F20F114424 08           MOVSD QWORD PTR SS:[ESP+0x8],XMM0
$-D      006A827C     F20F1045 0C             MOVSD XMM0,QWORD PTR SS:[EBP+0xC]
$-8      006A8281     F20F110424              MOVSD QWORD PTR SS:[ESP],XMM0                        ; ecx=State对象，比如InGameState,EscapeState,PreInGameState等
$-3      006A8286     FF75 08                 PUSH DWORD PTR SS:[EBP+0x8]                          ; 总共有5个参数，不过下面的函数都没有用到
$ ==>    006A8289     FF50 08                 CALL DWORD PTR DS:[EAX+0x8]                          ; 处理顶层ui逻辑
	$-A      006CADA6     8BE5                    MOV ESP,EBP
	$-8      006CADA8     5D                      POP EBP
	$-7      006CADA9     C2 1400                 RETN 0x14
	$-4      006CADAC     CC                      INT3
	$-3      006CADAD     CC                      INT3
	$-2      006CADAE     CC                      INT3
	$-1      006CADAF     CC                      INT3
	$ ==>    006CADB0     80B9 E8180000 00        CMP BYTE PTR DS:[ECX+0x18E8],0x0
	$+7      006CADB7     74 1E                   JE SHORT PathOfEx.006CADD7
	$+9      006CADB9     83B9 38010000 00        CMP DWORD PTR DS:[ECX+0x138],0x0
	$+10     006CADC0     74 15                   JE SHORT PathOfEx.006CADD7
	$+12     006CADC2     83B9 3C010000 00        CMP DWORD PTR DS:[ECX+0x13C],0x0
	$+19     006CADC9     74 0C                   JE SHORT PathOfEx.006CADD7
	$+1B     006CADCB     51                      PUSH ECX                                             ; 下面的指令执行后，ecx=stCD_StateUiLogic*
	$+1C     006CADCC     81C1 5C010000           ADD ECX,0x15C                                        ; InGameStateMainUi偏移15c,g_o_in_game_state2ui_mgr_
	$+22     006CADD2     E8 39CB3400             CALL PathOfEx.00A17910                               ; call tag_状态处理ui逻辑的函数
		$-2      00A1790E     CC                      INT3                                                 ; tag_状态处理ui逻辑的函数
		$-1      00A1790F     CC                      INT3                                                 ; 这个函数是State的ui处理逻辑的
		$ ==>    00A17910     55                      PUSH EBP                                             ; ecx=stCD_StateUiLogic*
		$+1      00A17911     8BEC                    MOV EBP,ESP
		$+3      00A17913     83E4 F8                 AND ESP,0xFFFFFFF8
		$+6      00A17916     51                      PUSH ECX
		$+7      00A17917     53                      PUSH EBX
		$+8      00A17918     56                      PUSH ESI
		$+9      00A17919     8BF1                    MOV ESI,ECX
		$+B      00A1791B     B9 10901301             MOV ECX,PathOfEx.01139010
		$+10     00A17920     57                      PUSH EDI
		$+11     00A17921     E8 1A6FFEFF             CALL PathOfEx.009FE840
		$+16     00A17926     85C0                    TEST EAX,EAX
		$+18     00A17928     0F85 E6000000           JNZ PathOfEx.00A17A14
		$+1E     00A1792E     8B86 B40A0000           MOV EAX,DWORD PTR DS:[ESI+0xAB4]
		$+24     00A17934     85C0                    TEST EAX,EAX
		$+26     00A17936     74 07                   JE SHORT PathOfEx.00A1793F
		$+28     00A17938     C680 54070000 00        MOV BYTE PTR DS:[EAX+0x754],0x0
		$+2F     00A1793F     8B86 AC0A0000           MOV EAX,DWORD PTR DS:[ESI+0xAAC]
		$+35     00A17945     32DB                    XOR BL,BL
		$+37     00A17947     85C0                    TEST EAX,EAX
		$+39     00A17949     74 0D                   JE SHORT PathOfEx.00A17958
		$+3B     00A1794B     8A98 54070000           MOV BL,BYTE PTR DS:[EAX+0x754]
		$+41     00A17951     C680 54070000 00        MOV BYTE PTR DS:[EAX+0x754],0x0
		$+48     00A17958     FFB6 A40A0000           PUSH DWORD PTR DS:[ESI+0xAA4]                        ; 偏移aa4,g_so_ui_logic_main_ui_
		$+4E     00A1795E     8BCE                    MOV ECX,ESI                                          ; 下面这个函数只被此函数调用
		$+50     00A17960     E8 9BE6FFFF             CALL PathOfEx.00A16000                               ; ui逻辑的顶层函数，不会被其他ui调用
	$+27     006CADD7     C2 1400                 RETN 0x14
	$+2A     006CADDA     CC                      INT3
	$+2B     006CADDB     CC                      INT3
	$+2C     006CADDC     CC                      INT3
	$+2D     006CADDD     CC                      INT3
	$+2E     006CADDE     CC                      INT3
	$+2F     006CADDF     CC                      INT3
	$+30     006CADE0     56                      PUSH ESI
	$+31     006CADE1     8BF1                    MOV ESI,ECX
$+3      006A828C     C745 FC 07000000        MOV DWORD PTR SS:[EBP-0x4],0x7
$+A      006A8293     85FF                    TEST EDI,EDI
$+C      006A8295     74 21                   JE SHORT PathOfEx.006A82B8
$+E      006A8297     83C8 FF                 OR EAX,0xFFFFFFFF
$+11     006A829A     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+16     006A829F     75 17                   JNZ SHORT PathOfEx.006A82B8
$+18     006A82A1     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+1A     006A82A3     8BCF                    MOV ECX,EDI
$+1C     006A82A5     FF10                    CALL DWORD PTR DS:[EAX]
$+1E     006A82A7     83C8 FF                 OR EAX,0xFFFFFFFF
$+21     006A82AA     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$+26     006A82AF     75 07                   JNZ SHORT PathOfEx.006A82B8
$+28     006A82B1     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+2A     006A82B3     8BCF                    MOV ECX,EDI
$+2C     006A82B5     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+2F     006A82B8     83C6 08                 ADD ESI,0x8
$+32     006A82BB     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$+39     006A82C2     3B73 14                 CMP ESI,DWORD PTR DS:[EBX+0x14]
$+3C     006A82C5   ^ 0F85 75FFFFFF           JNZ PathOfEx.006A8240
$+42     006A82CB     8343 1C FF              ADD DWORD PTR DS:[EBX+0x1C],-0x1
$+46     006A82CF     75 34                   JNZ SHORT PathOfEx.006A8305
$+48     006A82D1     807B 20 00              CMP BYTE PTR DS:[EBX+0x20],0x0
$+4C     006A82D5     74 2E                   JE SHORT PathOfEx.006A8305
$+4E     006A82D7     8B43 08                 MOV EAX,DWORD PTR DS:[EBX+0x8]
$+51     006A82DA     8D4B 10                 LEA ECX,DWORD PTR DS:[EBX+0x10]
$+54     006A82DD     2B43 04                 SUB EAX,DWORD PTR DS:[EBX+0x4]
$+57     006A82E0     C1F8 03                 SAR EAX,0x3
$+5A     006A82E3     50                      PUSH EAX
$+5B     006A82E4     E8 170B0000             CALL PathOfEx.006A8E00
$+60     006A82E9     8B53 08                 MOV EDX,DWORD PTR DS:[EBX+0x8]
$+63     006A82EC     8B4B 04                 MOV ECX,DWORD PTR DS:[EBX+0x4]
$+66     006A82EF     C645 08 00              MOV BYTE PTR SS:[EBP+0x8],0x0
$+6A     006A82F3     FF75 08                 PUSH DWORD PTR SS:[EBP+0x8]
$+6D     006A82F6     FF73 10                 PUSH DWORD PTR DS:[EBX+0x10]
$+70     006A82F9     E8 62180000             CALL PathOfEx.006A9B60
$+75     006A82FE     83C4 08                 ADD ESP,0x8
$+78     006A8301     C643 20 00              MOV BYTE PTR DS:[EBX+0x20],0x0
$+7C     006A8305     5F                      POP EDI
$+7D     006A8306     5E                      POP ESI
$+7E     006A8307     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+81     006A830A     5B                      POP EBX
$+82     006A830B     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+89     006A8312     8BE5                    MOV ESP,EBP
$+8B     006A8314     5D                      POP EBP
$+8C     006A8315     C2 1400                 RETN 0x14
$+8F     006A8318     CC                      INT3
*/
/**/
//call tag_状态计时处理ui逻辑的函数
/*
$-97     006CA93D     8B40 08                  MOV EAX,DWORD PTR DS:[EAX+0x8]
$-94     006CA940     83B8 88000000 04         CMP DWORD PTR DS:[EAX+0x88],0x4
$-8D     006CA947     74 6C                    JE SHORT PathOfEx.006CA9B5
$-8B     006CA949     8B87 38010000            MOV EAX,DWORD PTR DS:[EDI+0x138]
$-85     006CA94F     8B40 08                  MOV EAX,DWORD PTR DS:[EAX+0x8]
$-82     006CA952     83B8 88000000 03         CMP DWORD PTR DS:[EAX+0x88],0x3
$-7B     006CA959     74 5A                    JE SHORT PathOfEx.006CA9B5
$-79     006CA95B     6A 00                    PUSH 0x0
$-77     006CA95D     EB 4D                    JMP SHORT PathOfEx.006CA9AC
$-75     006CA95F     83F8 02                  CMP EAX,0x2
$-72     006CA962     75 51                    JNZ SHORT PathOfEx.006CA9B5
$-70     006CA964     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
$-6A     006CA96A     8A80 6B3B0000            MOV AL,BYTE PTR DS:[EAX+0x3B6B]
$-64     006CA970     84C0                     TEST AL,AL
$-62     006CA972     75 41                    JNZ SHORT PathOfEx.006CA9B5
$-60     006CA974     3887 D61A0000            CMP BYTE PTR DS:[EDI+0x1AD6],AL
$-5A     006CA97A     75 39                    JNZ SHORT PathOfEx.006CA9B5
$-58     006CA97C     8B8F AC1A0000            MOV ECX,DWORD PTR DS:[EDI+0x1AAC]
$-52     006CA982     85C9                     TEST ECX,ECX
$-50     006CA984     74 2F                    JE SHORT PathOfEx.006CA9B5
$-4E     006CA986     E8 8514DAFF              CALL PathOfEx.0046BE10
$-49     006CA98B     8378 30 00               CMP DWORD PTR DS:[EAX+0x30],0x0
$-45     006CA98F     7F 24                    JG SHORT PathOfEx.006CA9B5
$-43     006CA991     8B8F AC1A0000            MOV ECX,DWORD PTR DS:[EDI+0x1AAC]
$-3D     006CA997     E8 5413DAFF              CALL PathOfEx.0046BCF0
$-38     006CA99C     8B48 2C                  MOV ECX,DWORD PTR DS:[EAX+0x2C]
$-35     006CA99F     E8 0C14DAFF              CALL PathOfEx.0046BDB0
$-30     006CA9A4     8378 20 FF               CMP DWORD PTR DS:[EAX+0x20],-0x1
$-2C     006CA9A8     75 0B                    JNZ SHORT PathOfEx.006CA9B5
$-2A     006CA9AA     6A 01                    PUSH 0x1
$-28     006CA9AC     6A 01                    PUSH 0x1
$-26     006CA9AE     8BCF                     MOV ECX,EDI
$-24     006CA9B0     E8 3B090000              CALL PathOfEx.006CB2F0
$-1F     006CA9B5     83BF D81A0000 00         CMP DWORD PTR DS:[EDI+0x1AD8],0x0
$-18     006CA9BC     74 0B                    JE SHORT PathOfEx.006CA9C9
$-16     006CA9BE     8B8F D81A0000            MOV ECX,DWORD PTR DS:[EDI+0x1AD8]
$-10     006CA9C4     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$-E      006CA9C6     FF50 10                  CALL DWORD PTR DS:[EAX+0x10]
$-B      006CA9C9     51                       PUSH ECX
$-A      006CA9CA     53                       PUSH EBX
$-9      006CA9CB     83EC 08                  SUB ESP,0x8                                      ; edi=InGameState*
$-6      006CA9CE     8D8F 5C010000            LEA ECX,DWORD PTR DS:[EDI+0x15C]                 ; ecx=stCD_StateUiLogic*
$ ==>    006CA9D4     E8 A7CE3400              CALL PathOfEx.00A17880                           ; call tag_状态计时处理ui逻辑的函数
$+5      006CA9D9     8B87 D01A0000            MOV EAX,DWORD PTR DS:[EDI+0x1AD0]
$+B      006CA9DF     85C0                     TEST EAX,EAX
$+D      006CA9E1     74 18                    JE SHORT PathOfEx.006CA9FB
$+F      006CA9E3     3BC3                     CMP EAX,EBX
$+11     006CA9E5     76 0A                    JBE SHORT PathOfEx.006CA9F1
$+13     006CA9E7     2BC3                     SUB EAX,EBX
$+15     006CA9E9     8987 D01A0000            MOV DWORD PTR DS:[EDI+0x1AD0],EAX
$+1B     006CA9EF     EB 0A                    JMP SHORT PathOfEx.006CA9FB
$+1D     006CA9F1     C787 D01A0000 00000000   MOV DWORD PTR DS:[EDI+0x1AD0],0x0
$+27     006CA9FB     8B87 A01A0000            MOV EAX,DWORD PTR DS:[EDI+0x1AA0]
$+2D     006CAA01     8DB7 A01A0000            LEA ESI,DWORD PTR DS:[EDI+0x1AA0]
$+33     006CAA07     50                       PUSH EAX
$+34     006CAA08     FF30                     PUSH DWORD PTR DS:[EAX]
$+36     006CAA0A     E8 B1BA0000              CALL PathOfEx.006D64C0
$+3B     006CAA0F     83C4 08                  ADD ESP,0x8
$+3E     006CAA12     C745 FC 23000000         MOV DWORD PTR SS:[EBP-0x4],0x23
$+45     006CAA19     8BCE                     MOV ECX,ESI
$+47     006CAA1B     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+49     006CAA1D     FF70 04                  PUSH DWORD PTR DS:[EAX+0x4]
$+4C     006CAA20     E8 7B990000              CALL PathOfEx.006D43A0
$+51     006CAA25     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+53     006CAA27     8BCF                     MOV ECX,EDI
$+55     006CAA29     8940 04                  MOV DWORD PTR DS:[EAX+0x4],EAX
$+58     006CAA2C     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+5A     006CAA2E     8900                     MOV DWORD PTR DS:[EAX],EAX
$+5C     006CAA30     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+5E     006CAA32     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+65     006CAA39     8940 08                  MOV DWORD PTR DS:[EAX+0x8],EAX
$+68     006CAA3C     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4],0x0
$+6F     006CAA43     E8 987D0000              CALL PathOfEx.006D27E0
$+74     006CAA48     8B8F D8180000            MOV ECX,DWORD PTR DS:[EDI+0x18D8]
$+7A     006CAA4E     8B87 140C0000            MOV EAX,DWORD PTR DS:[EDI+0xC14]
$+80     006CAA54     3B81 400A0000            CMP EAX,DWORD PTR DS:[ECX+0xA40]
$+86     006CAA5A     75 14                    JNZ SHORT PathOfEx.006CAA70
$+88     006CAA5C     8B89 380A0000            MOV ECX,DWORD PTR DS:[ECX+0xA38]
$+8E     006CAA62     50                       PUSH EAX
$+8F     006CAA63     E8 D8223200              CALL PathOfEx.009ECD40
$+94     006CAA68     84C0                     TEST AL,AL
$+96     006CAA6A     75 04                    JNZ SHORT PathOfEx.006CAA70
$+98     006CAA6C     B3 01                    MOV BL,0x1
$+9A     006CAA6E     EB 02                    JMP SHORT PathOfEx.006CAA72
$+9C     006CAA70     32DB                     XOR BL,BL
$+9E     006CAA72     8D4D 50                  LEA ECX,DWORD PTR SS:[EBP+0x50]
$+A1     006CAA75     E8 C62CDBFF              CALL PathOfEx.0047D740
$+A6     006CAA7A     8D4D 60                  LEA ECX,DWORD PTR SS:[EBP+0x60]
$+A9     006CAA7D     51                       PUSH ECX
$+AA     006CAA7E     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+AC     006CAA80     E8 6BD0FDFF              CALL PathOfEx.006A7AF0
$+B1     006CAA85     3938                     CMP DWORD PTR DS:[EAX],EDI
$+B3     006CAA87     75 08                    JNZ SHORT PathOfEx.006CAA91
$+B5     006CAA89     84DB                     TEST BL,BL
$+B7     006CAA8B     74 04                    JE SHORT PathOfEx.006CAA91
$+B9     006CAA8D     B3 01                    MOV BL,0x1
$+BB     006CAA8F     EB 02                    JMP SHORT PathOfEx.006CAA93
$+BD     006CAA91     32DB                     XOR BL,BL
$+BF     006CAA93     8D4D 60                  LEA ECX,DWORD PTR SS:[EBP+0x60]
$+C2     006CAA96     E8 55FFD9FF              CALL PathOfEx.0046A9F0
$+C7     006CAA9B     8D4D 50                  LEA ECX,DWORD PTR SS:[EBP+0x50]
$+CA     006CAA9E     E8 4DFFD9FF              CALL PathOfEx.0046A9F0
$+CF     006CAAA3     84DB                     TEST BL,BL
$+D1     006CAAA5     74 07                    JE SHORT PathOfEx.006CAAAE
$+D3     006CAAA7     8BCF                     MOV ECX,EDI
$+D5     006CAAA9     E8 42600000              CALL PathOfEx.006D0AF0
$+DA     006CAAAE     8B75 74                  MOV ESI,DWORD PTR SS:[EBP+0x74]
$+DD     006CAAB1     8B8F 40150000            MOV ECX,DWORD PTR DS:[EDI+0x1540]
$+E3     006CAAB7     8B87 44150000            MOV EAX,DWORD PTR DS:[EDI+0x1544]
*/
/**/
//tag_状态计时处理ui逻辑的函数
/*
$-2      00A1787E     CC                       INT3
$-1      00A1787F     CC                       INT3                                             ; 这个函数类似于 tag_状态处理ui逻辑的函数
$ ==>    00A17880     56                       PUSH ESI                                         ; tag_状态计时处理ui逻辑的函数
$+1      00A17881     8B7424 10                MOV ESI,DWORD PTR SS:[ESP+0x10]                  ; ecx=stCD_StateUiLogic*
$+5      00A17885     57                       PUSH EDI                                         ; 这个函数每隔几百毫秒会进来一次，可能跟加速有关
$+6      00A17886     8BF9                     MOV EDI,ECX
$+8      00A17888     6A 01                    PUSH 0x1
$+A      00A1788A     6A 00                    PUSH 0x0
$+C      00A1788C     68 00020000              PUSH 0x200
$+11     00A17891     01B7 140B0000            ADD DWORD PTR DS:[EDI+0xB14],ESI
$+17     00A17897     8B87 140B0000            MOV EAX,DWORD PTR DS:[EDI+0xB14]
$+1D     00A1789D     66:0F6EC0                MOVD MM0,EAX
$+21     00A178A1     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+25     00A178A5     C1E8 1F                  SHR EAX,0x1F
$+28     00A178A8     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+31     00A178B1     660F5AC0                 CVTPD2PS XMM0,XMM0
$+35     00A178B5     F3:0F5905 28ECF300       MULSS XMM0,DWORD PTR DS:[0xF3EC28]
$+3D     00A178BD     F3:0F1187 180B0000       MOVSS DWORD PTR DS:[EDI+0xB18],XMM0
$+45     00A178C5     E8 36040000              CALL PathOfEx.00A17D00
$+4A     00A178CA     FFB7 A40A0000            PUSH DWORD PTR DS:[EDI+0xAA4]                    ; stCD_StateUiLogic.top_ui_
$+50     00A178D0     F3:0F1097 180B0000       MOVSS XMM2,DWORD PTR DS:[EDI+0xB18]
$+58     00A178D8     8BCF                     MOV ECX,EDI                                      ; ecx=stCD_StateUiLogic*
$+5A     00A178DA     66:0F6EC6                MOVD MM0,ESI
$+5E     00A178DE     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+62     00A178E2     C1EE 1F                  SHR ESI,0x1F
$+65     00A178E5     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+6E     00A178EE     660F5AD8                 CVTPD2PS XMM3,XMM0
$+72     00A178F2     F3:0F591D 28ECF300       MULSS XMM3,DWORD PTR DS:[0xF3EC28]               ; 可能跟加速有关
$+7A     00A178FA     E8 D1EAFFFF              CALL PathOfEx.00A163D0			;这个函数就是处理具体的ui逻辑了，子ui也会调用这个函数
$+7F     00A178FF     5F                       POP EDI
$+80     00A17900     5E                       POP ESI
$+81     00A17901     C2 1000                  RETN 0x10
$+84     00A17904     CC                       INT3
$+85     00A17905     CC                       INT3
$+86     00A17906     CC                       INT3
$+87     00A17907     CC                       INT3
$+88     00A17908     CC                       INT3
$+89     00A17909     CC                       INT3
$+8A     00A1790A     CC                       INT3
$+8B     00A1790B     CC                       INT3
$+8C     00A1790C     CC                       INT3
$+8D     00A1790D     CC                       INT3
$+8E     00A1790E     CC                       INT3                                             ; tag_状态处理ui逻辑的函数
$+8F     00A1790F     CC                       INT3                                             ; 这个函数是State的ui处理逻辑的
$+90     00A17910     55                       PUSH EBP                                         ; ecx=stCD_StateUiLogic*
$+91     00A17911     8BEC                     MOV EBP,ESP
$+93     00A17913     83E4 F8                  AND ESP,0xFFFFFFF8
$+96     00A17916     51                       PUSH ECX
$+97     00A17917     53                       PUSH EBX
$+98     00A17918     56                       PUSH ESI
$+99     00A17919     8BF1                     MOV ESI,ECX
$+9B     00A1791B     B9 30901301              MOV ECX,PathOfEx.01139030
$+A0     00A17920     57                       PUSH EDI
$+A1     00A17921     E8 1A6FFEFF              CALL PathOfEx.009FE840
$+A6     00A17926     85C0                     TEST EAX,EAX
$+A8     00A17928     0F85 E6000000            JNZ PathOfEx.00A17A14
$+AE     00A1792E     8B86 B40A0000            MOV EAX,DWORD PTR DS:[ESI+0xAB4]
$+B4     00A17934     85C0                     TEST EAX,EAX
$+B6     00A17936     74 07                    JE SHORT PathOfEx.00A1793F
$+B8     00A17938     C680 54070000 00         MOV BYTE PTR DS:[EAX+0x754],0x0
$+BF     00A1793F     8B86 AC0A0000            MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+C5     00A17945     32DB                     XOR BL,BL
$+C7     00A17947     85C0                     TEST EAX,EAX
$+C9     00A17949     74 0D                    JE SHORT PathOfEx.00A17958
$+CB     00A1794B     8A98 54070000            MOV BL,BYTE PTR DS:[EAX+0x754]
$+D1     00A17951     C680 54070000 00         MOV BYTE PTR DS:[EAX+0x754],0x0
$+D8     00A17958     FFB6 A40A0000            PUSH DWORD PTR DS:[ESI+0xAA4]                    ; 偏移aa4,g_so_ui_logic_main_ui_
$+DE     00A1795E     8BCE                     MOV ECX,ESI                                      ; 下面这个函数只被此函数调用
$+E0     00A17960     E8 9BE6FFFF              CALL PathOfEx.00A16000                           ; ui逻辑的顶层函数，不会被其他ui调用
$+E5     00A17965     8B86 AC0A0000            MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+EB     00A1796B     85C0                     TEST EAX,EAX
$+ED     00A1796D     74 1D                    JE SHORT PathOfEx.00A1798C
$+EF     00A1796F     8898 54070000            MOV BYTE PTR DS:[EAX+0x754],BL
$+F5     00A17975     8B86 AC0A0000            MOV EAX,DWORD PTR DS:[ESI+0xAAC]
$+FB     00A1797B     80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754],0x0
$+102    00A17982     74 08                    JE SHORT PathOfEx.00A1798C
$+104    00A17984     50                       PUSH EAX
$+105    00A17985     8BCE                     MOV ECX,ESI
$+107    00A17987     E8 74E6FFFF              CALL PathOfEx.00A16000
$+10C    00A1798C     8BBE C80A0000            MOV EDI,DWORD PTR DS:[ESI+0xAC8]
$+112    00A17992     85FF                     TEST EDI,EDI
$+114    00A17994     74 24                    JE SHORT PathOfEx.00A179BA
$+116    00A17996     80BF 54070000 00         CMP BYTE PTR DS:[EDI+0x754],0x0
$+11D    00A1799D     74 1B                    JE SHORT PathOfEx.00A179BA
$+11F    00A1799F     8B8F 5C070000            MOV ECX,DWORD PTR DS:[EDI+0x75C]
$+125    00A179A5     85C9                     TEST ECX,ECX
$+127    00A179A7     74 11                    JE SHORT PathOfEx.00A179BA
$+129    00A179A9     E8 3256FDFF              CALL PathOfEx.009ECFE0
$+12E    00A179AE     84C0                     TEST AL,AL
$+130    00A179B0     74 08                    JE SHORT PathOfEx.00A179BA
$+132    00A179B2     57                       PUSH EDI
$+133    00A179B3     8BCE                     MOV ECX,ESI
$+135    00A179B5     E8 46E6FFFF              CALL PathOfEx.00A16000
$+13A    00A179BA     8B86 B40A0000            MOV EAX,DWORD PTR DS:[ESI+0xAB4]
$+140    00A179C0     85C0                     TEST EAX,EAX
$+142    00A179C2     74 14                    JE SHORT PathOfEx.00A179D8
$+144    00A179C4     C680 54070000 01         MOV BYTE PTR DS:[EAX+0x754],0x1
$+14B    00A179CB     8BCE                     MOV ECX,ESI
$+14D    00A179CD     FFB6 B40A0000            PUSH DWORD PTR DS:[ESI+0xAB4]
$+153    00A179D3     E8 28E6FFFF              CALL PathOfEx.00A16000
$+158    00A179D8     8BCE                     MOV ECX,ESI
$+15A    00A179DA     E8 81E9FFFF              CALL PathOfEx.00A16360
$+15F    00A179DF     B9 30901301              MOV ECX,PathOfEx.01139030
$+164    00A179E4     E8 4784FEFF              CALL PathOfEx.009FFE30
$+169    00A179E9     803D 88901301 00         CMP BYTE PTR DS:[0x1139088],0x0
*/
/**/
//tag_EscapeState处理UiLogic
/*
$-3F     00674591    /74 21                   JE SHORT PathOfEx.006745B4
$-3D     00674593    |83CF FF                 OR EDI,0xFFFFFFFF
$-3A     00674596    |8BC7                    MOV EAX,EDI
$-38     00674598    |F0:0FC146 04            LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$-33     0067459D    |75 15                   JNZ SHORT PathOfEx.006745B4
$-31     0067459F    |8B06                    MOV EAX,DWORD PTR DS:[ESI]
$-2F     006745A1    |8BCE                    MOV ECX,ESI
$-2D     006745A3    |FF10                    CALL DWORD PTR DS:[EAX]
$-2B     006745A5    |F0:0FC17E 08            LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$-26     006745AA    |4F                      DEC EDI
$-25     006745AB    |75 07                   JNZ SHORT PathOfEx.006745B4
$-23     006745AD    |8B06                    MOV EAX,DWORD PTR DS:[ESI]
$-21     006745AF    |8BCE                    MOV ECX,ESI
$-1F     006745B1    |FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-1C     006745B4    \8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-19     006745B7     5F                      POP EDI
$-18     006745B8     5E                      POP ESI
$-17     006745B9     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-10     006745C0     8BE5                    MOV ESP,EBP
$-E      006745C2     5D                      POP EBP
$-D      006745C3     C2 0C00                 RETN 0xC
$-A      006745C6     CC                      INT3
$-9      006745C7     CC                      INT3
$-8      006745C8     CC                      INT3
$-7      006745C9     CC                      INT3
$-6      006745CA     CC                      INT3
$-5      006745CB     CC                      INT3
$-4      006745CC     CC                      INT3
$-3      006745CD     CC                      INT3
$-2      006745CE     CC                      INT3
$-1      006745CF     CC                      INT3
$ ==>    006745D0     51                      PUSH ECX                                             ; tag_EscapeState处理UiLogic
$+1      006745D1     83C1 20                 ADD ECX,0x20                                         ; g_so_escape_state_get_ui_logic_
$+4      006745D4     E8 37333A00             CALL PathOfEx.00A17910                               ; call tag_状态处理ui逻辑的函数
$+9      006745D9     C2 1400                 RETN 0x14
$+C      006745DC     CC                      INT3
$+D      006745DD     CC                      INT3
$+E      006745DE     CC                      INT3
$+F      006745DF     CC                      INT3
$+10     006745E0     8B5424 04               MOV EDX,DWORD PTR SS:[ESP+0x4]
$+14     006745E4     FF7424 08               PUSH DWORD PTR SS:[ESP+0x8]
$+18     006745E8     8991 440B0000           MOV DWORD PTR DS:[ECX+0xB44],EDX
$+1E     006745EE     8B89 C40A0000           MOV ECX,DWORD PTR DS:[ECX+0xAC4]
$+24     006745F4     52                      PUSH EDX
$+25     006745F5     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+27     006745F7     FF90 88000000           CALL DWORD PTR DS:[EAX+0x88]
$+2D     006745FD     33C9                    XOR ECX,ECX
$+2F     006745FF     85C0                    TEST EAX,EAX
$+31     00674601     0F48C8                  CMOVS ECX,EAX
$+34     00674604     8BC1                    MOV EAX,ECX
$+36     00674606     C2 0800                 RETN 0x8
$+39     00674609     CC                      INT3
*/
/**/
//tag_函数内进行循环添加Npc面板的MenuItem
/*
$-1      005C755F     CC                          INT3
$ ==>    005C7560     55                          PUSH EBP                                          ; tag_函数内进行循环添加Npc面板的MenuItem
$+1      005C7561     8BEC                        MOV EBP,ESP
$+3      005C7563     6A FF                       PUSH -0x1
$+5      005C7565     68 0146D300                 PUSH PathOfEx.00D34601
$+A      005C756A     64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
$+10     005C7570     50                          PUSH EAX
$+11     005C7571     64:8925 00000000            MOV DWORD PTR FS:[0],ESP
$+18     005C7578     81EC A4000000               SUB ESP,0xA4
$+1E     005C757E     53                          PUSH EBX
$+1F     005C757F     8BD9                        MOV EBX,ECX
$+21     005C7581     C745 AC 00000000            MOV DWORD PTR SS:[EBP-0x54],0x0
$+28     005C7588     56                          PUSH ESI
$+29     005C7589     57                          PUSH EDI
$+2A     005C758A     8B83 D0080000               MOV EAX,DWORD PTR DS:[EBX+0x8D0]
$+30     005C7590     C680 F40B0000 01            MOV BYTE PTR DS:[EAX+0xBF4],0x1
$+37     005C7597     8B83 D4080000               MOV EAX,DWORD PTR DS:[EBX+0x8D4]
$+3D     005C759D     85C0                        TEST EAX,EAX
$+3F     005C759F     74 07                       JE SHORT PathOfEx.005C75A8
$+41     005C75A1     C680 F40B0000 01            MOV BYTE PTR DS:[EAX+0xBF4],0x1
$+48     005C75A8     E8 531E0000                 CALL PathOfEx.005C9400
$+4D     005C75AD     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10],0x0
$+51     005C75B1     8B7D 08                     MOV EDI,DWORD PTR SS:[EBP+0x8]
$+54     005C75B4     8B75 0C                     MOV ESI,DWORD PTR SS:[EBP+0xC]
$+57     005C75B7     74 1F                       JE SHORT PathOfEx.005C75D8
$+59     005C75B9     85FF                        TEST EDI,EDI
$+5B     005C75BB     74 1B                       JE SHORT PathOfEx.005C75D8
$+5D     005C75BD     3BBB B8080000               CMP EDI,DWORD PTR DS:[EBX+0x8B8]
$+63     005C75C3     74 13                       JE SHORT PathOfEx.005C75D8
$+65     005C75C5     837E 04 00                  CMP DWORD PTR DS:[ESI+0x4],0x0
$+69     005C75C9     75 0D                       JNZ SHORT PathOfEx.005C75D8
$+6B     005C75CB     83BB C8080000 00            CMP DWORD PTR DS:[EBX+0x8C8],0x0
$+72     005C75D2     C645 E7 01                  MOV BYTE PTR SS:[EBP-0x19],0x1
$+76     005C75D6     75 04                       JNZ SHORT PathOfEx.005C75DC
$+78     005C75D8     C645 E7 00                  MOV BYTE PTR SS:[EBP-0x19],0x0
$+7C     005C75DC     8BCB                        MOV ECX,EBX
$+7E     005C75DE     E8 EDFDFFFF                 CALL PathOfEx.005C73D0
$+83     005C75E3     83BB C8080000 00            CMP DWORD PTR DS:[EBX+0x8C8],0x0
$+8A     005C75EA     0F84 840C0000               JE PathOfEx.005C8274
$+90     005C75F0     85FF                        TEST EDI,EDI
$+92     005C75F2     0F84 7C0C0000               JE PathOfEx.005C8274
$+98     005C75F8     8D8B B8080000               LEA ECX,DWORD PTR DS:[EBX+0x8B8]
$+9E     005C75FE     57                          PUSH EDI
$+9F     005C75FF     E8 FCE4FFFF                 CALL PathOfEx.005C5B00
$+A4     005C7604     56                          PUSH ESI
$+A5     005C7605     8D8B 18090000               LEA ECX,DWORD PTR DS:[EBX+0x918]
$+AB     005C760B     E8 50A6F0FF                 CALL PathOfEx.004D1C60
$+B0     005C7610     8BCF                        MOV ECX,EDI
$+B2     005C7612     E8 4975EDFF                 CALL PathOfEx.0049EB60
$+B7     005C7617     8BF0                        MOV ESI,EAX
$+B9     005C7619     8D45 E8                     LEA EAX,DWORD PTR SS:[EBP-0x18]
$+BC     005C761C     50                          PUSH EAX
$+BD     005C761D     8BCE                        MOV ECX,ESI
$+BF     005C761F     E8 0C44F0FF                 CALL PathOfEx.004CBA30
$+C4     005C7624     8D4D B0                     LEA ECX,DWORD PTR SS:[EBP-0x50]
$+C7     005C7627     51                          PUSH ECX
$+C8     005C7628     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
$+CB     005C762B     E8 E047EEFF                 CALL PathOfEx.004ABE10
$+D0     005C7630     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
$+D3     005C7633     8945 E0                     MOV DWORD PTR SS:[EBP-0x20],EAX
$+D6     005C7636     C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
$+DD     005C763D     8B45 B0                     MOV EAX,DWORD PTR SS:[EBP-0x50]
$+E0     005C7640     85C0                        TEST EAX,EAX
$+E2     005C7642     74 18                       JE SHORT PathOfEx.005C765C
$+E4     005C7644     50                          PUSH EAX
$+E5     005C7645     51                          PUSH ECX
$+E6     005C7646     E8 B5091500                 CALL PathOfEx.00718000
$+EB     005C764B     83C4 04                     ADD ESP,0x4
$+EE     005C764E     8BC8                        MOV ECX,EAX
$+F0     005C7650     E8 3B051500                 CALL PathOfEx.00717B90
$+F5     005C7655     C745 B0 00000000            MOV DWORD PTR SS:[EBP-0x50],0x0
$+FC     005C765C     C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4],0x1
$+103    005C7663     8B45 E8                     MOV EAX,DWORD PTR SS:[EBP-0x18]
$+106    005C7666     85C0                        TEST EAX,EAX
$+108    005C7668     74 11                       JE SHORT PathOfEx.005C767B
$+10A    005C766A     50                          PUSH EAX
$+10B    005C766B     51                          PUSH ECX
$+10C    005C766C     E8 8F091500                 CALL PathOfEx.00718000
$+111    005C7671     83C4 04                     ADD ESP,0x4
$+114    005C7674     8BC8                        MOV ECX,EAX
$+116    005C7676     E8 15051500                 CALL PathOfEx.00717B90
$+11B    005C767B     837D E0 00                  CMP DWORD PTR SS:[EBP-0x20],0x0
$+11F    005C767F     74 29                       JE SHORT PathOfEx.005C76AA
$+121    005C7681     8B8B C8080000               MOV ECX,DWORD PTR DS:[EBX+0x8C8]
$+127    005C7687     8B81 943A0000               MOV EAX,DWORD PTR DS:[ECX+0x3A94]
$+12D    005C768D     8B40 08                     MOV EAX,DWORD PTR DS:[EAX+0x8]
$+130    005C7690     8A80 DE000000               MOV AL,BYTE PTR DS:[EAX+0xDE]
$+136    005C7696     84C0                        TEST AL,AL
$+138    005C7698     74 0A                       JE SHORT PathOfEx.005C76A4
$+13A    005C769A     8A81 AC3E0000               MOV AL,BYTE PTR DS:[ECX+0x3EAC]
$+140    005C76A0     84C0                        TEST AL,AL
$+142    005C76A2     74 06                       JE SHORT PathOfEx.005C76AA
$+144    005C76A4     C645 13 01                  MOV BYTE PTR SS:[EBP+0x13],0x1
$+148    005C76A8     EB 04                       JMP SHORT PathOfEx.005C76AE
$+14A    005C76AA     C645 13 00                  MOV BYTE PTR SS:[EBP+0x13],0x0
$+14E    005C76AE     8D45 E8                     LEA EAX,DWORD PTR SS:[EBP-0x18]
$+151    005C76B1     8BCE                        MOV ECX,ESI
$+153    005C76B3     50                          PUSH EAX
$+154    005C76B4     E8 7743F0FF                 CALL PathOfEx.004CBA30
$+159    005C76B9     C745 FC 02000000            MOV DWORD PTR SS:[EBP-0x4],0x2
$+160    005C76C0     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+163    005C76C3     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
$+166    005C76C6     FF30                        PUSH DWORD PTR DS:[EAX]
$+168    005C76C8     E8 2367EAFF                 CALL PathOfEx.0046DDF0
$+16D    005C76CD     BA 2C36E900                 MOV EDX,PathOfEx.00E9362C                         ; UNICODE "/Fish"
$+172    005C76D2     8BC8                        MOV ECX,EAX
$+174    005C76D4     E8 97350000                 CALL PathOfEx.005CAC70
$+179    005C76D9     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+17C    005C76DC     8845 0B                     MOV BYTE PTR SS:[EBP+0xB],AL
$+17F    005C76DF     E8 5C60EAFF                 CALL PathOfEx.0046D740
$+184    005C76E4     C745 FC 03000000            MOV DWORD PTR SS:[EBP-0x4],0x3
$+18B    005C76EB     8B45 E8                     MOV EAX,DWORD PTR SS:[EBP-0x18]
$+18E    005C76EE     85C0                        TEST EAX,EAX
$+190    005C76F0     74 11                       JE SHORT PathOfEx.005C7703
$+192    005C76F2     50                          PUSH EAX
$+193    005C76F3     51                          PUSH ECX
$+194    005C76F4     E8 07091500                 CALL PathOfEx.00718000
$+199    005C76F9     83C4 04                     ADD ESP,0x4
$+19C    005C76FC     8BC8                        MOV ECX,EAX
$+19E    005C76FE     E8 8D041500                 CALL PathOfEx.00717B90
$+1A3    005C7703     807D E7 00                  CMP BYTE PTR SS:[EBP-0x19],0x0
$+1A7    005C7707     74 3F                       JE SHORT PathOfEx.005C7748
$+1A9    005C7709     8D45 E8                     LEA EAX,DWORD PTR SS:[EBP-0x18]
$+1AC    005C770C     8BCE                        MOV ECX,ESI
$+1AE    005C770E     50                          PUSH EAX
$+1AF    005C770F     E8 1C43F0FF                 CALL PathOfEx.004CBA30
$+1B4    005C7714     C745 FC 04000000            MOV DWORD PTR SS:[EBP-0x4],0x4
$+1BB    005C771B     8BCB                        MOV ECX,EBX
$+1BD    005C771D     FFB3 B8080000               PUSH DWORD PTR DS:[EBX+0x8B8]
$+1C3    005C7723     50                          PUSH EAX
$+1C4    005C7724     E8 47240000                 CALL PathOfEx.005C9B70
$+1C9    005C7729     C745 FC 05000000            MOV DWORD PTR SS:[EBP-0x4],0x5
$+1D0    005C7730     8B45 E8                     MOV EAX,DWORD PTR SS:[EBP-0x18]
$+1D3    005C7733     85C0                        TEST EAX,EAX
$+1D5    005C7735     74 11                       JE SHORT PathOfEx.005C7748
$+1D7    005C7737     50                          PUSH EAX
$+1D8    005C7738     51                          PUSH ECX
$+1D9    005C7739     E8 C2081500                 CALL PathOfEx.00718000
$+1DE    005C773E     83C4 04                     ADD ESP,0x4
$+1E1    005C7741     8BC8                        MOV ECX,EAX
$+1E3    005C7743     E8 48041500                 CALL PathOfEx.00717B90
$+1E8    005C7748     8D45 E8                     LEA EAX,DWORD PTR SS:[EBP-0x18]
$+1EB    005C774B     8BCE                        MOV ECX,ESI
$+1ED    005C774D     50                          PUSH EAX
$+1EE    005C774E     E8 DD42F0FF                 CALL PathOfEx.004CBA30
$+1F3    005C7753     C745 FC 06000000            MOV DWORD PTR SS:[EBP-0x4],0x6
$+1FA    005C775A     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+1FD    005C775D     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
$+200    005C7760     FF70 04                     PUSH DWORD PTR DS:[EAX+0x4]
$+203    005C7763     E8 8866EAFF                 CALL PathOfEx.0046DDF0
$+208    005C7768     C645 FC 07                  MOV BYTE PTR SS:[EBP-0x4],0x7
$+20C    005C776C     8D55 B8                     LEA EDX,DWORD PTR SS:[EBP-0x48]
$+20F    005C776F     8B8B D8080000               MOV ECX,DWORD PTR DS:[EBX+0x8D8]
$+215    005C7775     81C1 B0080000               ADD ECX,0x8B0
$+21B    005C777B     52                          PUSH EDX
$+21C    005C777C     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+21E    005C777E     FF50 04                     CALL DWORD PTR DS:[EAX+0x4]
$+221    005C7781     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+224    005C7784     E8 B75FEAFF                 CALL PathOfEx.0046D740
$+229    005C7789     C745 FC 08000000            MOV DWORD PTR SS:[EBP-0x4],0x8
$+230    005C7790     8B45 E8                     MOV EAX,DWORD PTR SS:[EBP-0x18]
$+233    005C7793     85C0                        TEST EAX,EAX
$+235    005C7795     74 11                       JE SHORT PathOfEx.005C77A8
$+237    005C7797     50                          PUSH EAX
$+238    005C7798     51                          PUSH ECX
$+239    005C7799     E8 62081500                 CALL PathOfEx.00718000
$+23E    005C779E     83C4 04                     ADD ESP,0x4
$+241    005C77A1     8BC8                        MOV ECX,EAX
$+243    005C77A3     E8 E8031500                 CALL PathOfEx.00717B90
$+248    005C77A8     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
$+24F    005C77AF     8B8B D8080000               MOV ECX,DWORD PTR DS:[EBX+0x8D8]
$+255    005C77B5     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+257    005C77B7     8B40 40                     MOV EAX,DWORD PTR DS:[EAX+0x40]
$+25A    005C77BA     FFD0                        CALL EAX
$+25C    005C77BC     D80D ACECF300               FMUL DWORD PTR DS:[0xF3ECAC]
$+262    005C77C2     8B8B D8080000               MOV ECX,DWORD PTR DS:[EBX+0x8D8]
$+268    005C77C8     D82D FCEEF300               FSUBR DWORD PTR DS:[0xF3EEFC]
$+26E    005C77CE     F3:0F1081 64070000          MOVSS XMM0,DWORD PTR DS:[ECX+0x764]
$+276    005C77D6     D95D D0                     FSTP DWORD PTR SS:[EBP-0x30]
$+279    005C77D9     F3:0F104D D0                MOVSS XMM1,DWORD PTR SS:[EBP-0x30]
$+27E    005C77DE     0F2EC1                      UCOMISS XMM0,XMM1
$+281    005C77E1     9F                          LAHF
$+282    005C77E2     F6C4 44                     TEST AH,0x44
$+285    005C77E5     7B 10                       JPO SHORT PathOfEx.005C77F7
$+287    005C77E7     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+289    005C77E9     F3:0F1189 64070000          MOVSS DWORD PTR DS:[ECX+0x764],XMM1
$+291    005C77F1     FF90 08010000               CALL DWORD PTR DS:[EAX+0x108]
$+297    005C77F7     C745 90 00000000            MOV DWORD PTR SS:[EBP-0x70],0x0
$+29E    005C77FE     C745 94 00000000            MOV DWORD PTR SS:[EBP-0x6C],0x0
$+2A5    005C7805     C745 FC 09000000            MOV DWORD PTR SS:[EBP-0x4],0x9
$+2AC    005C780C     8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+2AF    005C780F     8379 04 00                  CMP DWORD PTR DS:[ECX+0x4],0x0
$+2B3    005C7813     0F84 49030000               JE PathOfEx.005C7B62
$+2B9    005C7819     8B8B C0080000               MOV ECX,DWORD PTR DS:[EBX+0x8C0]
$+2BF    005C781F     8B81 D8180000               MOV EAX,DWORD PTR DS:[ECX+0x18D8]
$+2C5    005C7825     83B8 B8090000 00            CMP DWORD PTR DS:[EAX+0x9B8],0x0
$+2CC    005C782C     74 0B                       JE SHORT PathOfEx.005C7839
$+2CE    005C782E     8B88 B8090000               MOV ECX,DWORD PTR DS:[EAX+0x9B8]
$+2D4    005C7834     E8 2768FAFF                 CALL PathOfEx.0056E060
$+2D9    005C7839     807D 13 00                  CMP BYTE PTR SS:[EBP+0x13],0x0
$+2DD    005C783D     75 0B                       JNZ SHORT PathOfEx.005C784A
$+2DF    005C783F     807D 0B 00                  CMP BYTE PTR SS:[EBP+0xB],0x0
$+2E3    005C7843     BA 009EE800                 MOV EDX,PathOfEx.00E89E00                         ; UNICODE "Art/2DArt/UIImages/InGame/NPCSpeechBar"
$+2E8    005C7848     74 05                       JE SHORT PathOfEx.005C784F
$+2EA    005C784A     BA D035E900                 MOV EDX,PathOfEx.00E935D0                         ; UNICODE "Art/2DArt/UIImages/InGame/NPCMasterSpeechBar"
$+2EF    005C784F     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+2F2    005C7852     E8 899C4300                 CALL PathOfEx.00A014E0
$+2F7    005C7857     50                          PUSH EAX
$+2F8    005C7858     8D4D 90                     LEA ECX,DWORD PTR SS:[EBP-0x70]
$+2FB    005C785B     E8 D0C1EEFF                 CALL PathOfEx.004B3A30
$+300    005C7860     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+303    005C7863     E8 980EECFF                 CALL PathOfEx.00488700
$+308    005C7868     F3:0F1005 8CF2F300          MOVSS XMM0,DWORD PTR DS:[0xF3F28C]
$+310    005C7870     8BCB                        MOV ECX,EBX
$+312    005C7872     F3:0F100D 68EFF300          MOVSS XMM1,DWORD PTR DS:[0xF3EF68]
$+31A    005C787A     F3:0F1145 DC                MOVSS DWORD PTR SS:[EBP-0x24],XMM0
$+31F    005C787F     E8 7C0C0000                 CALL PathOfEx.005C8500
$+324    005C7884     8B8B C8080000               MOV ECX,DWORD PTR DS:[EBX+0x8C8]
$+32A    005C788A     8983 D4080000               MOV DWORD PTR DS:[EBX+0x8D4],EAX
$+330    005C7890     8D45 0B                     LEA EAX,DWORD PTR SS:[EBP+0xB]
$+333    005C7893     50                          PUSH EAX
$+334    005C7894     E8 37293100                 CALL PathOfEx.008DA1D0
$+339    005C7899     8B55 0C                     MOV EDX,DWORD PTR SS:[EBP+0xC]
$+33C    005C789C     8D4D A8                     LEA ECX,DWORD PTR SS:[EBP-0x58]
$+33F    005C789F     0FB600                      MOVZX EAX,BYTE PTR DS:[EAX]
$+342    005C78A2     83E0 07                     AND EAX,0x7
$+345    005C78A5     50                          PUSH EAX
$+346    005C78A6     E8 F5DFFFFF                 CALL PathOfEx.005C58A0
$+34B    005C78AB     83C4 04                     ADD ESP,0x4
$+34E    005C78AE     C645 FC 0A                  MOV BYTE PTR SS:[EBP-0x4],0xA
$+352    005C78B2     837D AC 00                  CMP DWORD PTR SS:[EBP-0x54],0x0
$+356    005C78B6     0F84 67010000               JE PathOfEx.005C7A23
$+35C    005C78BC     8B8B FC080000               MOV ECX,DWORD PTR DS:[EBX+0x8FC]
$+362    005C78C2     8DB3 FC080000               LEA ESI,DWORD PTR DS:[EBX+0x8FC]
$+368    005C78C8     85C9                        TEST ECX,ECX
$+36A    005C78CA     74 0C                       JE SHORT PathOfEx.005C78D8
$+36C    005C78CC     8D83 B4080000               LEA EAX,DWORD PTR DS:[EBX+0x8B4]
$+372    005C78D2     50                          PUSH EAX
$+373    005C78D3     E8 D8BD3500                 CALL PathOfEx.009236B0
$+378    005C78D8     8BCE                        MOV ECX,ESI
$+37A    005C78DA     E8 819FEDFF                 CALL PathOfEx.004A1860
$+37F    005C78DF     8B45 AC                     MOV EAX,DWORD PTR SS:[EBP-0x54]
$+382    005C78E2     8B48 10                     MOV ECX,DWORD PTR DS:[EAX+0x10]
$+385    005C78E5     85C9                        TEST ECX,ECX
$+387    005C78E7     0F84 31010000               JE PathOfEx.005C7A1E
$+38D    005C78ED     66:8339 00                  CMP WORD PTR DS:[ECX],0x0
$+391    005C78F1     0F84 27010000               JE PathOfEx.005C7A1E
$+397    005C78F7     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
$+39A    005C78FA     8B50 04                     MOV EDX,DWORD PTR DS:[EAX+0x4]
$+39D    005C78FD     8A42 55                     MOV AL,BYTE PTR DS:[EDX+0x55]
$+3A0    005C7900     84C0                        TEST AL,AL
$+3A2    005C7902     75 7F                       JNZ SHORT PathOfEx.005C7983
$+3A4    005C7904     8A42 7E                     MOV AL,BYTE PTR DS:[EDX+0x7E]
$+3A7    005C7907     84C0                        TEST AL,AL
$+3A9    005C7909     75 78                       JNZ SHORT PathOfEx.005C7983
$+3AB    005C790B     57                          PUSH EDI
$+3AC    005C790C     8D85 7CFFFFFF               LEA EAX,DWORD PTR SS:[EBP-0x84]
$+3B2    005C7912     8BCB                        MOV ECX,EBX
$+3B4    005C7914     50                          PUSH EAX
$+3B5    005C7915     E8 86210000                 CALL PathOfEx.005C9AA0
$+3BA    005C791A     8B45 AC                     MOV EAX,DWORD PTR SS:[EBP-0x54]
$+3BD    005C791D     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+3C0    005C7920     C745 E0 02000000            MOV DWORD PTR SS:[EBP-0x20],0x2
$+3C7    005C7927     C645 13 00                  MOV BYTE PTR SS:[EBP+0x13],0x0
$+3CB    005C792B     FF70 10                     PUSH DWORD PTR DS:[EAX+0x10]
$+3CE    005C792E     E8 BD64EAFF                 CALL PathOfEx.0046DDF0
$+3D3    005C7933     51                          PUSH ECX
$+3D4    005C7934     6A 2F                       PUSH 0x2F
$+3D6    005C7936     8D55 B8                     LEA EDX,DWORD PTR SS:[EBP-0x48]
$+3D9    005C7939     C645 FC 0D                  MOV BYTE PTR SS:[EBP-0x4],0xD
$+3DD    005C793D     8D8D 54FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+3E3    005C7943     E8 B8151F00                 CALL PathOfEx.007B8F00
$+3E8    005C7948     8D4D 98                     LEA ECX,DWORD PTR SS:[EBP-0x68]
$+3EB    005C794B     C645 FC 0E                  MOV BYTE PTR SS:[EBP-0x4],0xE
$+3EF    005C794F     51                          PUSH ECX
$+3F0    005C7950     8D8D 7CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0x84]
$+3F6    005C7956     C745 98 00000000            MOV DWORD PTR SS:[EBP-0x68],0x0
$+3FD    005C795D     51                          PUSH ECX
$+3FE    005C795E     8D4D E0                     LEA ECX,DWORD PTR SS:[EBP-0x20]
$+401    005C7961     C745 9C 00000000            MOV DWORD PTR SS:[EBP-0x64],0x0
$+408    005C7968     51                          PUSH ECX
$+409    005C7969     8D4D 13                     LEA ECX,DWORD PTR SS:[EBP+0x13]
$+40C    005C796C     C745 A0 00000000            MOV DWORD PTR SS:[EBP-0x60],0x0
$+413    005C7973     51                          PUSH ECX
$+414    005C7974     8BD0                        MOV EDX,EAX
$+416    005C7976     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+419    005C7979     E8 F2330000                 CALL PathOfEx.005CAD70
$+41E    005C797E     83C4 18                     ADD ESP,0x18
$+421    005C7981     EB 42                       JMP SHORT PathOfEx.005C79C5
$+423    005C7983     51                          PUSH ECX
$+424    005C7984     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+427    005C7987     C745 E0 02000000            MOV DWORD PTR SS:[EBP-0x20],0x2
$+42E    005C798E     C645 13 00                  MOV BYTE PTR SS:[EBP+0x13],0x0
$+432    005C7992     E8 5964EAFF                 CALL PathOfEx.0046DDF0
$+437    005C7997     51                          PUSH ECX
$+438    005C7998     6A 2F                       PUSH 0x2F
$+43A    005C799A     8D55 B8                     LEA EDX,DWORD PTR SS:[EBP-0x48]
$+43D    005C799D     C645 FC 0B                  MOV BYTE PTR SS:[EBP-0x4],0xB
$+441    005C79A1     8D8D 54FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+447    005C79A7     E8 54151F00                 CALL PathOfEx.007B8F00
$+44C    005C79AC     8D4D E0                     LEA ECX,DWORD PTR SS:[EBP-0x20]
$+44F    005C79AF     C645 FC 0C                  MOV BYTE PTR SS:[EBP-0x4],0xC
$+453    005C79B3     51                          PUSH ECX
$+454    005C79B4     8D4D 13                     LEA ECX,DWORD PTR SS:[EBP+0x13]
$+457    005C79B7     8BD0                        MOV EDX,EAX
$+459    005C79B9     51                          PUSH ECX
$+45A    005C79BA     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+45D    005C79BD     E8 FE320000                 CALL PathOfEx.005CACC0
$+462    005C79C2     83C4 10                     ADD ESP,0x10
$+465    005C79C5     50                          PUSH EAX
$+466    005C79C6     8BCE                        MOV ECX,ESI
$+468    005C79C8     E8 033CEAFF                 CALL PathOfEx.0046B5D0
$+46D    005C79CD     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+470    005C79D0     E8 1B30EAFF                 CALL PathOfEx.0046A9F0
$+475    005C79D5     8D8D 54FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+47B    005C79DB     E8 605DEAFF                 CALL PathOfEx.0046D740
$+480    005C79E0     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+483    005C79E3     C645 FC 0A                  MOV BYTE PTR SS:[EBP-0x4],0xA
$+487    005C79E7     E8 545DEAFF                 CALL PathOfEx.0046D740
$+48C    005C79EC     8B0E                        MOV ECX,DWORD PTR DS:[ESI]
$+48E    005C79EE     8D83 B4080000               LEA EAX,DWORD PTR DS:[EBX+0x8B4]
$+494    005C79F4     50                          PUSH EAX
$+495    005C79F5     E8 56BC3500                 CALL PathOfEx.00923650
$+49A    005C79FA     8B06                        MOV EAX,DWORD PTR DS:[ESI]
$+49C    005C79FC     6A 00                       PUSH 0x0
$+49E    005C79FE     C740 3C 0000003F            MOV DWORD PTR DS:[EAX+0x3C],0x3F000000
$+4A5    005C7A05     8B0E                        MOV ECX,DWORD PTR DS:[ESI]
$+4A7    005C7A07     E8 E4B83500                 CALL PathOfEx.009232F0
$+4AC    005C7A0C     8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
$+4AF    005C7A0F     8B41 04                     MOV EAX,DWORD PTR DS:[ECX+0x4]
$+4B2    005C7A12     8B40 08                     MOV EAX,DWORD PTR DS:[EAX+0x8]
$+4B5    005C7A15     8983 08090000               MOV DWORD PTR DS:[EBX+0x908],EAX
$+4BB    005C7A1B     8B45 AC                     MOV EAX,DWORD PTR SS:[EBP-0x54]
$+4BE    005C7A1E     8B78 0C                     MOV EDI,DWORD PTR DS:[EAX+0xC]
$+4C1    005C7A21     EB 05                       JMP SHORT PathOfEx.005C7A28
$+4C3    005C7A23     BF 4CFCE600                 MOV EDI,PathOfEx.00E6FC4C
$+4C8    005C7A28     8BCF                        MOV ECX,EDI
$+4CA    005C7A2A     C745 10 20000000            MOV DWORD PTR SS:[EBP+0x10],0x20
$+4D1    005C7A31     8D51 02                     LEA EDX,DWORD PTR DS:[ECX+0x2]
$+4D4    005C7A34     66:8B01                     MOV AX,WORD PTR DS:[ECX]
$+4D7    005C7A37     83C1 02                     ADD ECX,0x2
$+4DA    005C7A3A     66:85C0                     TEST AX,AX
$+4DD    005C7A3D   ^ 75 F5                       JNZ SHORT PathOfEx.005C7A34
$+4DF    005C7A3F     2BCA                        SUB ECX,EDX
$+4E1    005C7A41     8D45 10                     LEA EAX,DWORD PTR SS:[EBP+0x10]
$+4E4    005C7A44     D1F9                        SAR ECX,1
$+4E6    005C7A46     50                          PUSH EAX
$+4E7    005C7A47     8D144F                      LEA EDX,DWORD PTR DS:[EDI+ECX*2]
$+4EA    005C7A4A     8BCF                        MOV ECX,EDI
$+4EC    005C7A4C     E8 FFD9FAFF                 CALL PathOfEx.00575450
$+4F1    005C7A51     40                          INC EAX
$+4F2    005C7A52     F3:0F100D 8CEEF300          MOVSS XMM1,DWORD PTR DS:[0xF3EE8C]
$+4FA    005C7A5A     83C4 04                     ADD ESP,0x4
$+4FD    005C7A5D     C745 10 00000041            MOV DWORD PTR SS:[EBP+0x10],0x41000000
$+504    005C7A64     8D75 10                     LEA ESI,DWORD PTR SS:[EBP+0x10]
$+507    005C7A67     C745 D4 00000000            MOV DWORD PTR SS:[EBP-0x2C],0x0
$+50E    005C7A6E     C745 D8 00000000            MOV DWORD PTR SS:[EBP-0x28],0x0
$+515    005C7A75     66:0F6EC0                   MOVD MM0,EAX
$+519    005C7A79     F30FE6C0                    CVTDQ2PD XMM0,XMM0
$+51D    005C7A7D     C1E8 1F                     SHR EAX,0x1F
$+520    005C7A80     F20F5804C5 40FCF300         ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
$+529    005C7A89     8D45 0C                     LEA EAX,DWORD PTR SS:[EBP+0xC]
$+52C    005C7A8C     660F5AC0                    CVTPD2PS XMM0,XMM0
$+530    005C7A90     F3:0F5905 70ECF300          MULSS XMM0,DWORD PTR DS:[0xF3EC70]
$+538    005C7A98     0F2FC8                      COMISS XMM1,XMM0
$+53B    005C7A9B     F3:0F1145 0C                MOVSS DWORD PTR SS:[EBP+0xC],XMM0
$+540    005C7AA0     0F46F0                      CMOVBE ESI,EAX
$+543    005C7AA3     57                          PUSH EDI
$+544    005C7AA4     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+547    005C7AA7     C645 FC 0F                  MOV BYTE PTR SS:[EBP-0x4],0xF
$+54B    005C7AAB     E8 4063EAFF                 CALL PathOfEx.0046DDF0
$+550    005C7AB0     8D45 B8                     LEA EAX,DWORD PTR SS:[EBP-0x48]
$+553    005C7AB3     C645 FC 10                  MOV BYTE PTR SS:[EBP-0x4],0x10
$+557    005C7AB7     F3:0F101E                   MOVSS XMM3,DWORD PTR DS:[ESI]
$+55B    005C7ABB     8BCB                        MOV ECX,EBX
$+55D    005C7ABD     50                          PUSH EAX
$+55E    005C7ABE     FFB3 D4080000               PUSH DWORD PTR DS:[EBX+0x8D4]
$+564    005C7AC4     E8 97110000                 CALL PathOfEx.005C8C60
$+569    005C7AC9     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+56C    005C7ACC     C645 FC 0F                  MOV BYTE PTR SS:[EBP-0x4],0xF
$+570    005C7AD0     E8 6B5CEAFF                 CALL PathOfEx.0046D740
$+575    005C7AD5     FFB3 D4080000               PUSH DWORD PTR DS:[EBX+0x8D4]
$+57B    005C7ADB     E8 30170000                 CALL PathOfEx.005C9210
$+580    005C7AE0     C745 B0 00000000            MOV DWORD PTR SS:[EBP-0x50],0x0
$+587    005C7AE7     C745 B4 00000000            MOV DWORD PTR SS:[EBP-0x4C],0x0
$+58E    005C7AEE     BA AA000000                 MOV EDX,0xAA
$+593    005C7AF3     C645 FC 11                  MOV BYTE PTR SS:[EBP-0x4],0x11
$+597    005C7AF7     8D8D 54FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+59D    005C7AFD     E8 CE591500                 CALL PathOfEx.0071D4D0
$+5A2    005C7B02     6A 00                       PUSH 0x0
$+5A4    005C7B04     8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+5A7    005C7B07     C645 FC 12                  MOV BYTE PTR SS:[EBP-0x4],0x12
$+5AB    005C7B0B     F3:0F1015 8CF2F300          MOVSS XMM2,DWORD PTR DS:[0xF3F28C]
$+5B3    005C7B13     51                          PUSH ECX
$+5B4    005C7B14     6A 39                       PUSH 0x39
$+5B6    005C7B16     6A 00                       PUSH 0x0
$+5B8    005C7B18     8D4D B0                     LEA ECX,DWORD PTR SS:[EBP-0x50]
$+5BB    005C7B1B     51                          PUSH ECX
$+5BC    005C7B1C     50                          PUSH EAX
$+5BD    005C7B1D     FFB3 D4080000               PUSH DWORD PTR DS:[EBX+0x8D4]
$+5C3    005C7B23     8BCB                        MOV ECX,EBX
$+5C5    005C7B25     E8 B60B0000                 CALL PathOfEx.005C86E0
$+5CA    005C7B2A     8D8D 54FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+5D0    005C7B30     E8 0B5CEAFF                 CALL PathOfEx.0046D740
$+5D5    005C7B35     8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+5D8    005C7B38     E8 C30BECFF                 CALL PathOfEx.00488700
$+5DD    005C7B3D     C645 FC 14                  MOV BYTE PTR SS:[EBP-0x4],0x14
$+5E1    005C7B41     8B45 A8                     MOV EAX,DWORD PTR SS:[EBP-0x58]
$+5E4    005C7B44     85C0                        TEST EAX,EAX
$+5E6    005C7B46     74 11                       JE SHORT PathOfEx.005C7B59
$+5E8    005C7B48     50                          PUSH EAX
$+5E9    005C7B49     51                          PUSH ECX
$+5EA    005C7B4A     E8 B1041500                 CALL PathOfEx.00718000
$+5EF    005C7B4F     83C4 04                     ADD ESP,0x4
$+5F2    005C7B52     8BC8                        MOV ECX,EAX
$+5F4    005C7B54     E8 37001500                 CALL PathOfEx.00717B90
$+5F9    005C7B59     C645 FC 09                  MOV BYTE PTR SS:[EBP-0x4],0x9
$+5FD    005C7B5D     E9 D8050000                 JMP PathOfEx.005C813A
$+602    005C7B62     807D 13 00                  CMP BYTE PTR SS:[EBP+0x13],0x0
$+606    005C7B66     75 56                       JNZ SHORT PathOfEx.005C7BBE
$+608    005C7B68     807D 0B 00                  CMP BYTE PTR SS:[EBP+0xB],0x0
$+60C    005C7B6C     75 50                       JNZ SHORT PathOfEx.005C7BBE
$+60E    005C7B6E     837D E0 00                  CMP DWORD PTR SS:[EBP-0x20],0x0
$+612    005C7B72     74 16                       JE SHORT PathOfEx.005C7B8A
$+614    005C7B74     BA 1037E900                 MOV EDX,PathOfEx.00E93710                         ; UNICODE "Art/2DArt/UIImages/InGame/NPCMenuBarWide"
$+619    005C7B79     8D4D 88                     LEA ECX,DWORD PTR SS:[EBP-0x78]
$+61C    005C7B7C     E8 5F994300                 CALL PathOfEx.00A014E0
$+621    005C7B81     8BD0                        MOV EDX,EAX
$+623    005C7B83     B9 01000000                 MOV ECX,0x1
$+628    005C7B88     EB 14                       JMP SHORT PathOfEx.005C7B9E
$+62A    005C7B8A     BA 4036E900                 MOV EDX,PathOfEx.00E93640                         ; UNICODE "Art/2DArt/UIImages/InGame/NPCMenuBar"
$+62F    005C7B8F     8D4D 80                     LEA ECX,DWORD PTR SS:[EBP-0x80]
$+632    005C7B92     E8 49994300                 CALL PathOfEx.00A014E0
$+637    005C7B97     8BD0                        MOV EDX,EAX
$+639    005C7B99     B9 02000000                 MOV ECX,0x2
$+63E    005C7B9E     8B02                        MOV EAX,DWORD PTR DS:[EDX]
$+640    005C7BA0     8945 D4                     MOV DWORD PTR SS:[EBP-0x2C],EAX
$+643    005C7BA3     8B42 04                     MOV EAX,DWORD PTR DS:[EDX+0x4]
$+646    005C7BA6     8945 D8                     MOV DWORD PTR SS:[EBP-0x28],EAX
$+649    005C7BA9     C702 00000000               MOV DWORD PTR DS:[EDX],0x0
$+64F    005C7BAF     C742 04 00000000            MOV DWORD PTR DS:[EDX+0x4],0x0
$+656    005C7BB6     8D45 D4                     LEA EAX,DWORD PTR SS:[EBP-0x2C]
$+659    005C7BB9     83C9 04                     OR ECX,0x4
$+65C    005C7BBC     EB 12                       JMP SHORT PathOfEx.005C7BD0
$+65E    005C7BBE     BA B836E900                 MOV EDX,PathOfEx.00E936B8                         ; UNICODE "Art/2DArt/UIImages/InGame/NPCMasterMenuBar"
$+663    005C7BC3     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+666    005C7BC6     E8 15994300                 CALL PathOfEx.00A014E0
$+66B    005C7BCB     B9 08000000                 MOV ECX,0x8
$+670    005C7BD0     894D 0C                     MOV DWORD PTR SS:[EBP+0xC],ECX
$+673    005C7BD3     8D4D 90                     LEA ECX,DWORD PTR SS:[EBP-0x70]
$+676    005C7BD6     50                          PUSH EAX
$+677    005C7BD7     E8 54BEEEFF                 CALL PathOfEx.004B3A30
$+67C    005C7BDC     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
$+67F    005C7BDF     A8 08                       TEST AL,0x8
$+681    005C7BE1     74 11                       JE SHORT PathOfEx.005C7BF4
$+683    005C7BE3     83E0 F7                     AND EAX,0xFFFFFFF7
$+686    005C7BE6     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+689    005C7BE9     8945 0C                     MOV DWORD PTR SS:[EBP+0xC],EAX
$+68C    005C7BEC     E8 0F0BECFF                 CALL PathOfEx.00488700
$+691    005C7BF1     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
$+694    005C7BF4     A8 04                       TEST AL,0x4
$+696    005C7BF6     74 11                       JE SHORT PathOfEx.005C7C09
$+698    005C7BF8     83E0 FB                     AND EAX,0xFFFFFFFB
$+69B    005C7BFB     8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+69E    005C7BFE     8945 0C                     MOV DWORD PTR SS:[EBP+0xC],EAX
$+6A1    005C7C01     E8 FA0AECFF                 CALL PathOfEx.00488700
$+6A6    005C7C06     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
$+6A9    005C7C09     A8 02                       TEST AL,0x2
$+6AB    005C7C0B     74 11                       JE SHORT PathOfEx.005C7C1E
$+6AD    005C7C0D     83E0 FD                     AND EAX,0xFFFFFFFD
$+6B0    005C7C10     8D4D 80                     LEA ECX,DWORD PTR SS:[EBP-0x80]
$+6B3    005C7C13     8945 0C                     MOV DWORD PTR SS:[EBP+0xC],EAX
$+6B6    005C7C16     E8 E50AECFF                 CALL PathOfEx.00488700
$+6BB    005C7C1B     8B45 0C                     MOV EAX,DWORD PTR SS:[EBP+0xC]
$+6BE    005C7C1E     C745 FC 09000000            MOV DWORD PTR SS:[EBP-0x4],0x9
$+6C5    005C7C25     A8 01                       TEST AL,0x1
$+6C7    005C7C27     74 08                       JE SHORT PathOfEx.005C7C31
$+6C9    005C7C29     8D4D 88                     LEA ECX,DWORD PTR SS:[EBP-0x78]
$+6CC    005C7C2C     E8 CF0AECFF                 CALL PathOfEx.00488700
$+6D1    005C7C31     8A45 13                     MOV AL,BYTE PTR SS:[EBP+0x13]
$+6D4    005C7C34     8A4D 0B                     MOV CL,BYTE PTR SS:[EBP+0xB]
$+6D7    005C7C37     84C0                        TEST AL,AL
$+6D9    005C7C39     75 13                       JNZ SHORT PathOfEx.005C7C4E
$+6DB    005C7C3B     84C9                        TEST CL,CL
$+6DD    005C7C3D     75 0F                       JNZ SHORT PathOfEx.005C7C4E
$+6DF    005C7C3F     F3:0F1005 88F1F300          MOVSS XMM0,DWORD PTR DS:[0xF3F188]
$+6E7    005C7C47     F3:0F1145 DC                MOVSS DWORD PTR SS:[EBP-0x24],XMM0
$+6EC    005C7C4C     EB 3B                       JMP SHORT PathOfEx.005C7C89
$+6EE    005C7C4E     F3:0F1005 14F2F300          MOVSS XMM0,DWORD PTR DS:[0xF3F214]
$+6F6    005C7C56     F3:0F1145 DC                MOVSS DWORD PTR SS:[EBP-0x24],XMM0
$+6FB    005C7C5B     84C0                        TEST AL,AL
$+6FD    005C7C5D     74 09                       JE SHORT PathOfEx.005C7C68
$+6FF    005C7C5F     8B46 0C                     MOV EAX,DWORD PTR DS:[ESI+0xC]
$+702    005C7C62     8378 14 00                  CMP DWORD PTR DS:[EAX+0x14],0x0
$+706    005C7C66     75 04                       JNZ SHORT PathOfEx.005C7C6C
$+708    005C7C68     84C9                        TEST CL,CL
$+70A    005C7C6A     74 1D                       JE SHORT PathOfEx.005C7C89
$+70C    005C7C6C     FFB3 0C090000               PUSH DWORD PTR DS:[EBX+0x90C]                     ; ebx是个ui
$+712    005C7C72     56                          PUSH ESI
$+713    005C7C73     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+719    005C7C79     E8 F2160000                 CALL PathOfEx.005C9370
$+71E    005C7C7E     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+724    005C7C84     E8 87150000                 CALL PathOfEx.005C9210
$+729    005C7C89     FFB3 0C090000               PUSH DWORD PTR DS:[EBX+0x90C]                     ; 偏移90c
$+72F    005C7C8F     8D45 98                     LEA EAX,DWORD PTR SS:[EBP-0x68]
$+732    005C7C92     8BCE                        MOV ECX,ESI                                       ; esi=stCD_EntityAttrNpc*
$+734    005C7C94     50                          PUSH EAX                                          ; eax=stCD_NpcPanelTalkInfoVector*
$+735    005C7C95     E8 76D72200                 CALL PathOfEx.007F5410                            ; 填充stCD_NpcPanelTalkInfoVector
$+73A    005C7C9A     C645 FC 18                  MOV BYTE PTR SS:[EBP-0x4],0x18
$+73E    005C7C9E     32C9                        XOR CL,CL
$+740    005C7CA0     8B7D 9C                     MOV EDI,DWORD PTR SS:[EBP-0x64]
$+743    005C7CA3     BE 01000000                 MOV ESI,0x1
$+748    005C7CA8     884D 13                     MOV BYTE PTR SS:[EBP+0x13],CL
$+74B    005C7CAB     8975 08                     MOV DWORD PTR SS:[EBP+0x8],ESI
$+74E    005C7CAE     66:90                       NOP
$+750    005C7CB0     8B45 98                     MOV EAX,DWORD PTR SS:[EBP-0x68]
$+753    005C7CB3     3BC7                        CMP EAX,EDI                                       ; eax=stCD_NpcPanelTalkInfoVector.begin
$+755    005C7CB5     0F84 6C030000               JE PathOfEx.005C8027
$+75B    005C7CBB     8D78 04                     LEA EDI,DWORD PTR DS:[EAX+0x4]
$+75E    005C7CBE     66:90                       NOP
$+760    005C7CC0     8B0F                        MOV ECX,DWORD PTR DS:[EDI]                        ; 循环开始，ecx=stCD_TblItemNPCTalk*
$+762    005C7CC2     8B47 04                     MOV EAX,DWORD PTR DS:[EDI+0x4]
$+765    005C7CC5     8945 E0                     MOV DWORD PTR SS:[EBP-0x20],EAX
$+768    005C7CC8     3971 30                     CMP DWORD PTR DS:[ECX+0x30],ESI
$+76B    005C7CCB     0F85 41030000               JNZ PathOfEx.005C8012
$+771    005C7CD1     8D85 74FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0x8C]
$+777    005C7CD7     C645 13 01                  MOV BYTE PTR SS:[EBP+0x13],0x1
$+77B    005C7CDB     50                          PUSH EAX
$+77C    005C7CDC     C645 0C 00                  MOV BYTE PTR SS:[EBP+0xC],0x0
$+780    005C7CE0     E8 AB3F0000                 CALL PathOfEx.005CBC90
$+785    005C7CE5     8B70 04                     MOV ESI,DWORD PTR DS:[EAX+0x4]
$+788    005C7CE8     C645 FC 19                  MOV BYTE PTR SS:[EBP-0x4],0x19
$+78C    005C7CEC     8B85 74FFFFFF               MOV EAX,DWORD PTR SS:[EBP-0x8C]
$+792    005C7CF2     85C0                        TEST EAX,EAX
$+794    005C7CF4     74 1B                       JE SHORT PathOfEx.005C7D11
$+796    005C7CF6     50                          PUSH EAX
$+797    005C7CF7     51                          PUSH ECX
$+798    005C7CF8     E8 03031500                 CALL PathOfEx.00718000
$+79D    005C7CFD     83C4 04                     ADD ESP,0x4
$+7A0    005C7D00     8BC8                        MOV ECX,EAX
$+7A2    005C7D02     E8 89FE1400                 CALL PathOfEx.00717B90
$+7A7    005C7D07     C785 74FFFFFF 00000000      MOV DWORD PTR SS:[EBP-0x8C],0x0
$+7B1    005C7D11     C645 FC 18                  MOV BYTE PTR SS:[EBP-0x4],0x18
$+7B5    005C7D15     85F6                        TEST ESI,ESI
$+7B7    005C7D17     74 06                       JE SHORT PathOfEx.005C7D1F
$+7B9    005C7D19     C645 0C 00                  MOV BYTE PTR SS:[EBP+0xC],0x0
$+7BD    005C7D1D     EB 73                       JMP SHORT PathOfEx.005C7D92
$+7BF    005C7D1F     8B07                        MOV EAX,DWORD PTR DS:[EDI]
$+7C1    005C7D21     8378 30 03                  CMP DWORD PTR DS:[EAX+0x30],0x3
$+7C5    005C7D25     75 15                       JNZ SHORT PathOfEx.005C7D3C
$+7C7    005C7D27     8B8B 0C090000               MOV ECX,DWORD PTR DS:[EBX+0x90C]
$+7CD    005C7D2D     6A 08                       PUSH 0x8
$+7CF    005C7D2F     E8 ACEB2100                 CALL PathOfEx.007E68E0
$+7D4    005C7D34     84C0                        TEST AL,AL
$+7D6    005C7D36     0F9445 0C                   SETE BYTE PTR SS:[EBP+0xC]
$+7DA    005C7D3A     EB 56                       JMP SHORT PathOfEx.005C7D92
$+7DC    005C7D3C     8B93 0C090000               MOV EDX,DWORD PTR DS:[EBX+0x90C]
$+7E2    005C7D42     8B40 3C                     MOV EAX,DWORD PTR DS:[EAX+0x3C]
$+7E5    005C7D45     8B4A 44                     MOV ECX,DWORD PTR DS:[EDX+0x44]
$+7E8    005C7D48     3D E1030000                 CMP EAX,0x3E1
$+7ED    005C7D4D     74 13                       JE SHORT PathOfEx.005C7D62
$+7EF    005C7D4F     48                          DEC EAX
$+7F0    005C7D50     C1E1 07                     SHL ECX,0x7
$+7F3    005C7D53     83C2 D0                     ADD EDX,-0x30
$+7F6    005C7D56     50                          PUSH EAX
$+7F7    005C7D57     03CA                        ADD ECX,EDX
$+7F9    005C7D59     E8 22FAF2FF                 CALL PathOfEx.004F7780
$+7FE    005C7D5E     84C0                        TEST AL,AL
$+800    005C7D60     75 2C                       JNZ SHORT PathOfEx.005C7D8E                       ; 跳过去
$+802    005C7D62     8B07                        MOV EAX,DWORD PTR DS:[EDI]
$+804    005C7D64     8B70 66                     MOV ESI,DWORD PTR DS:[EAX+0x66]
$+807    005C7D67     81FE 61010000               CMP ESI,0x161
$+80D    005C7D6D     74 23                       JE SHORT PathOfEx.005C7D92
$+80F    005C7D6F     8B83 0C090000               MOV EAX,DWORD PTR DS:[EBX+0x90C]
$+815    005C7D75     4E                          DEC ESI
$+816    005C7D76     8BCE                        MOV ECX,ESI
$+818    005C7D78     BA 01000000                 MOV EDX,0x1
$+81D    005C7D7D     83E1 07                     AND ECX,0x7
$+820    005C7D80     C1EE 03                     SHR ESI,0x3
$+823    005C7D83     D3E2                        SHL EDX,CL
$+825    005C7D85     849406 D0010000             TEST BYTE PTR DS:[ESI+EAX+0x1D0],DL
$+82C    005C7D8C     74 04                       JE SHORT PathOfEx.005C7D92
$+82E    005C7D8E     C645 0C 01                  MOV BYTE PTR SS:[EBP+0xC],0x1
$+832    005C7D92     8B07                        MOV EAX,DWORD PTR DS:[EDI]
$+834    005C7D94     8378 7F 01                  CMP DWORD PTR DS:[EAX+0x7F],0x1
$+838    005C7D98     0F85 ED010000               JNZ PathOfEx.005C7F8B                             ; 在这跳转
$+83E    005C7D9E     51                          PUSH ECX
$+83F    005C7D9F     8D4D 88                     LEA ECX,DWORD PTR SS:[EBP-0x78]
$+842    005C7DA2     51                          PUSH ECX
$+843    005C7DA3     8BC8                        MOV ECX,EAX
$+845    005C7DA5     E8 A63F0000                 CALL PathOfEx.005CBD50
$+84A    005C7DAA     8D8D 6CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0x94]
$+850    005C7DB0     51                          PUSH ECX
$+851    005C7DB1     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
$+854    005C7DB4     E8 27400000                 CALL PathOfEx.005CBDE0
$+859    005C7DB9     C645 FC 1B                  MOV BYTE PTR SS:[EBP-0x4],0x1B
$+85D    005C7DBD     8B8B 0C090000               MOV ECX,DWORD PTR DS:[EBX+0x90C]
$+863    005C7DC3     50                          PUSH EAX
$+864    005C7DC4     E8 37EC2100                 CALL PathOfEx.007E6A00
$+869    005C7DC9     8945 A4                     MOV DWORD PTR SS:[EBP-0x5C],EAX
$+86C    005C7DCC     C645 FC 1C                  MOV BYTE PTR SS:[EBP-0x4],0x1C
$+870    005C7DD0     8B8D 6CFFFFFF               MOV ECX,DWORD PTR SS:[EBP-0x94]
$+876    005C7DD6     85C9                        TEST ECX,ECX
$+878    005C7DD8     74 1B                       JE SHORT PathOfEx.005C7DF5
$+87A    005C7DDA     51                          PUSH ECX
$+87B    005C7DDB     51                          PUSH ECX
$+87C    005C7DDC     E8 1F021500                 CALL PathOfEx.00718000
$+881    005C7DE1     83C4 04                     ADD ESP,0x4
$+884    005C7DE4     8BC8                        MOV ECX,EAX
$+886    005C7DE6     E8 A5FD1400                 CALL PathOfEx.00717B90
$+88B    005C7DEB     C785 6CFFFFFF 00000000      MOV DWORD PTR SS:[EBP-0x94],0x0
$+895    005C7DF5     C645 FC 1D                  MOV BYTE PTR SS:[EBP-0x4],0x1D
$+899    005C7DF9     8B45 88                     MOV EAX,DWORD PTR SS:[EBP-0x78]
$+89C    005C7DFC     85C0                        TEST EAX,EAX
$+89E    005C7DFE     74 18                       JE SHORT PathOfEx.005C7E18
$+8A0    005C7E00     50                          PUSH EAX
$+8A1    005C7E01     51                          PUSH ECX
$+8A2    005C7E02     E8 F9011500                 CALL PathOfEx.00718000
$+8A7    005C7E07     83C4 04                     ADD ESP,0x4
$+8AA    005C7E0A     8BC8                        MOV ECX,EAX
$+8AC    005C7E0C     E8 7FFD1400                 CALL PathOfEx.00717B90
$+8B1    005C7E11     C745 88 00000000            MOV DWORD PTR SS:[EBP-0x78],0x0
$+8B8    005C7E18     51                          PUSH ECX
$+8B9    005C7E19     8D45 D4                     LEA EAX,DWORD PTR SS:[EBP-0x2C]
$+8BC    005C7E1C     C645 FC 18                  MOV BYTE PTR SS:[EBP-0x4],0x18
$+8C0    005C7E20     8B0F                        MOV ECX,DWORD PTR DS:[EDI]
$+8C2    005C7E22     50                          PUSH EAX
$+8C3    005C7E23     E8 283F0000                 CALL PathOfEx.005CBD50
$+8C8    005C7E28     8D45 D0                     LEA EAX,DWORD PTR SS:[EBP-0x30]
$+8CB    005C7E2B     50                          PUSH EAX
$+8CC    005C7E2C     8D4D D4                     LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+8CF    005C7E2F     E8 8C35EDFF                 CALL PathOfEx.0049B3C0
$+8D4    005C7E34     C645 FC 1F                  MOV BYTE PTR SS:[EBP-0x4],0x1F
$+8D8    005C7E38     8B75 D0                     MOV ESI,DWORD PTR SS:[EBP-0x30]
$+8DB    005C7E3B     85F6                        TEST ESI,ESI
$+8DD    005C7E3D     74 0E                       JE SHORT PathOfEx.005C7E4D
$+8DF    005C7E3F     8BCE                        MOV ECX,ESI
$+8E1    005C7E41     E8 7A171500                 CALL PathOfEx.007195C0
$+8E6    005C7E46     8BCE                        MOV ECX,ESI
$+8E8    005C7E48     E8 73171500                 CALL PathOfEx.007195C0
$+8ED    005C7E4D     8B46 30                     MOV EAX,DWORD PTR DS:[ESI+0x30]
$+8F0    005C7E50     8B4D D8                     MOV ECX,DWORD PTR SS:[EBP-0x28]
$+8F3    005C7E53     2B48 04                     SUB ECX,DWORD PTR DS:[EAX+0x4]
$+8F6    005C7E56     B8 09CB3D8D                 MOV EAX,0x8D3DCB09
$+8FB    005C7E5B     F7E9                        IMUL ECX
$+8FD    005C7E5D     03D1                        ADD EDX,ECX
$+8FF    005C7E5F     8B4D A4                     MOV ECX,DWORD PTR SS:[EBP-0x5C]
$+902    005C7E62     C1FA 05                     SAR EDX,0x5
$+905    005C7E65     49                          DEC ECX
$+906    005C7E66     8BC2                        MOV EAX,EDX
$+908    005C7E68     C1E8 1F                     SHR EAX,0x1F
$+90B    005C7E6B     03C2                        ADD EAX,EDX
$+90D    005C7E6D     8D55 D0                     LEA EDX,DWORD PTR SS:[EBP-0x30]
$+910    005C7E70     03C1                        ADD EAX,ECX
$+912    005C7E72     8D4D 80                     LEA ECX,DWORD PTR SS:[EBP-0x80]
$+915    005C7E75     50                          PUSH EAX
$+916    005C7E76     E8 C5B41900                 CALL PathOfEx.00763340
$+91B    005C7E7B     83C4 04                     ADD ESP,0x4
$+91E    005C7E7E     8B08                        MOV ECX,DWORD PTR DS:[EAX]
$+920    005C7E80     C700 00000000               MOV DWORD PTR DS:[EAX],0x0
$+926    005C7E86     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
$+929    005C7E89     894D AC                     MOV DWORD PTR SS:[EBP-0x54],ECX
$+92C    005C7E8C     894D E8                     MOV DWORD PTR SS:[EBP-0x18],ECX
$+92F    005C7E8F     8945 A4                     MOV DWORD PTR SS:[EBP-0x5C],EAX
$+932    005C7E92     8945 EC                     MOV DWORD PTR SS:[EBP-0x14],EAX
$+935    005C7E95     C645 FC 21                  MOV BYTE PTR SS:[EBP-0x4],0x21
$+939    005C7E99     8B4D 80                     MOV ECX,DWORD PTR SS:[EBP-0x80]
$+93C    005C7E9C     85C9                        TEST ECX,ECX
$+93E    005C7E9E     74 1B                       JE SHORT PathOfEx.005C7EBB
$+940    005C7EA0     51                          PUSH ECX
$+941    005C7EA1     51                          PUSH ECX
$+942    005C7EA2     E8 59011500                 CALL PathOfEx.00718000
$+947    005C7EA7     83C4 04                     ADD ESP,0x4
$+94A    005C7EAA     8BC8                        MOV ECX,EAX
$+94C    005C7EAC     E8 DFFC1400                 CALL PathOfEx.00717B90
$+951    005C7EB1     8B45 A4                     MOV EAX,DWORD PTR SS:[EBP-0x5C]
$+954    005C7EB4     C745 80 00000000            MOV DWORD PTR SS:[EBP-0x80],0x0
$+95B    005C7EBB     C645 FC 20                  MOV BYTE PTR SS:[EBP-0x4],0x20
$+95F    005C7EBF     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+962    005C7EC2     85C0                        TEST EAX,EAX
$+964    005C7EC4     74 0E                       JE SHORT PathOfEx.005C7ED4
$+966    005C7EC6     FF70 0C                     PUSH DWORD PTR DS:[EAX+0xC]
$+969    005C7EC9     E8 225FEAFF                 CALL PathOfEx.0046DDF0
$+96E    005C7ECE     C645 FC 22                  MOV BYTE PTR SS:[EBP-0x4],0x22
$+972    005C7ED2     EB 0E                       JMP SHORT PathOfEx.005C7EE2
$+974    005C7ED4     8B07                        MOV EAX,DWORD PTR DS:[EDI]
$+976    005C7ED6     FF70 0C                     PUSH DWORD PTR DS:[EAX+0xC]
$+979    005C7ED9     E8 125FEAFF                 CALL PathOfEx.0046DDF0
$+97E    005C7EDE     C645 FC 23                  MOV BYTE PTR SS:[EBP-0x4],0x23
$+982    005C7EE2     F3:0F1055 DC                MOVSS XMM2,DWORD PTR SS:[EBP-0x24]
$+987    005C7EE7     8D47 FC                     LEA EAX,DWORD PTR DS:[EDI-0x4]
$+98A    005C7EEA     6A 00                       PUSH 0x0
$+98C    005C7EEC     68 18921301                 PUSH PathOfEx.01139218
$+991    005C7EF1     FF75 E0                     PUSH DWORD PTR SS:[EBP-0x20]
$+994    005C7EF4     8BCB                        MOV ECX,EBX
$+996    005C7EF6     FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
$+999    005C7EF9     50                          PUSH EAX
$+99A    005C7EFA     8D45 B8                     LEA EAX,DWORD PTR SS:[EBP-0x48]
$+99D    005C7EFD     50                          PUSH EAX
$+99E    005C7EFE     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+9A4    005C7F04     E8 D7070000                 CALL PathOfEx.005C86E0
$+9A9    005C7F09     8B45 CC                     MOV EAX,DWORD PTR SS:[EBP-0x34]
$+9AC    005C7F0C     83F8 08                     CMP EAX,0x8
$+9AF    005C7F0F     72 0F                       JB SHORT PathOfEx.005C7F20
$+9B1    005C7F11     6A 02                       PUSH 0x2
$+9B3    005C7F13     40                          INC EAX
$+9B4    005C7F14     50                          PUSH EAX
$+9B5    005C7F15     FF75 B8                     PUSH DWORD PTR SS:[EBP-0x48]
$+9B8    005C7F18     E8 1349EAFF                 CALL PathOfEx.0046C830
$+9BD    005C7F1D     83C4 0C                     ADD ESP,0xC
$+9C0    005C7F20     C645 FC 24                  MOV BYTE PTR SS:[EBP-0x4],0x24
$+9C4    005C7F24     8B45 AC                     MOV EAX,DWORD PTR SS:[EBP-0x54]
$+9C7    005C7F27     85C0                        TEST EAX,EAX
$+9C9    005C7F29     74 18                       JE SHORT PathOfEx.005C7F43
$+9CB    005C7F2B     50                          PUSH EAX
$+9CC    005C7F2C     51                          PUSH ECX
$+9CD    005C7F2D     E8 CE001500                 CALL PathOfEx.00718000
$+9D2    005C7F32     83C4 04                     ADD ESP,0x4
$+9D5    005C7F35     8BC8                        MOV ECX,EAX
$+9D7    005C7F37     E8 54FC1400                 CALL PathOfEx.00717B90
$+9DC    005C7F3C     C745 E8 00000000            MOV DWORD PTR SS:[EBP-0x18],0x0
$+9E3    005C7F43     56                          PUSH ESI
$+9E4    005C7F44     51                          PUSH ECX
$+9E5    005C7F45     C645 FC 25                  MOV BYTE PTR SS:[EBP-0x4],0x25
$+9E9    005C7F49     E8 B2001500                 CALL PathOfEx.00718000
$+9EE    005C7F4E     83C4 04                     ADD ESP,0x4
$+9F1    005C7F51     8BC8                        MOV ECX,EAX
$+9F3    005C7F53     E8 38FC1400                 CALL PathOfEx.00717B90
$+9F8    005C7F58     C745 D0 00000000            MOV DWORD PTR SS:[EBP-0x30],0x0
$+9FF    005C7F5F     C645 FC 26                  MOV BYTE PTR SS:[EBP-0x4],0x26
$+A03    005C7F63     8B45 D4                     MOV EAX,DWORD PTR SS:[EBP-0x2C]
$+A06    005C7F66     85C0                        TEST EAX,EAX
$+A08    005C7F68     74 18                       JE SHORT PathOfEx.005C7F82
$+A0A    005C7F6A     50                          PUSH EAX
$+A0B    005C7F6B     51                          PUSH ECX
$+A0C    005C7F6C     E8 8F001500                 CALL PathOfEx.00718000
$+A11    005C7F71     83C4 04                     ADD ESP,0x4
$+A14    005C7F74     8BC8                        MOV ECX,EAX
$+A16    005C7F76     E8 15FC1400                 CALL PathOfEx.00717B90
$+A1B    005C7F7B     C745 D4 00000000            MOV DWORD PTR SS:[EBP-0x2C],0x0
$+A22    005C7F82     C645 FC 18                  MOV BYTE PTR SS:[EBP-0x4],0x18
$+A26    005C7F86     E9 84000000                 JMP PathOfEx.005C800F
$+A2B    005C7F8B     8B50 0C                     MOV EDX,DWORD PTR DS:[EAX+0xC]                    ; eax=stCD_TblItemNPCTalk*
$+A2E    005C7F8E     33C0                        XOR EAX,EAX
$+A30    005C7F90     C745 CC 07000000            MOV DWORD PTR SS:[EBP-0x34],0x7
$+A37    005C7F97     C745 C8 00000000            MOV DWORD PTR SS:[EBP-0x38],0x0
$+A3E    005C7F9E     66:8945 B8                  MOV WORD PTR SS:[EBP-0x48],AX
$+A42    005C7FA2     66:3902                     CMP WORD PTR DS:[EDX],AX
$+A45    005C7FA5     75 04                       JNZ SHORT PathOfEx.005C7FAB
$+A47    005C7FA7     33C9                        XOR ECX,ECX
$+A49    005C7FA9     EB 14                       JMP SHORT PathOfEx.005C7FBF
$+A4B    005C7FAB     8BCA                        MOV ECX,EDX
$+A4D    005C7FAD     8D71 02                     LEA ESI,DWORD PTR DS:[ECX+0x2]                    ; 计算menu item的名字
$+A50    005C7FB0     66:8B01                     MOV AX,WORD PTR DS:[ECX]
$+A53    005C7FB3     83C1 02                     ADD ECX,0x2
$+A56    005C7FB6     66:85C0                     TEST AX,AX
$+A59    005C7FB9   ^ 75 F5                       JNZ SHORT PathOfEx.005C7FB0
$+A5B    005C7FBB     2BCE                        SUB ECX,ESI
$+A5D    005C7FBD     D1F9                        SAR ECX,1
$+A5F    005C7FBF     51                          PUSH ECX
$+A60    005C7FC0     52                          PUSH EDX
$+A61    005C7FC1     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+A64    005C7FC4     E8 E75CEAFF                 CALL PathOfEx.0046DCB0                            ; 名字字符串拷贝
$+A69    005C7FC9     F3:0F1055 DC                MOVSS XMM2,DWORD PTR SS:[EBP-0x24]
$+A6E    005C7FCE     8D47 FC                     LEA EAX,DWORD PTR DS:[EDI-0x4]
$+A71    005C7FD1     6A 00                       PUSH 0x0
$+A73    005C7FD3     68 18921301                 PUSH PathOfEx.01139218
$+A78    005C7FD8     C645 FC 27                  MOV BYTE PTR SS:[EBP-0x4],0x27
$+A7C    005C7FDC     8BCB                        MOV ECX,EBX
$+A7E    005C7FDE     FF75 E0                     PUSH DWORD PTR SS:[EBP-0x20]
$+A81    005C7FE1     FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
$+A84    005C7FE4     50                          PUSH EAX
$+A85    005C7FE5     8D45 B8                     LEA EAX,DWORD PTR SS:[EBP-0x48]
$+A88    005C7FE8     50                          PUSH EAX                                          ; eax=std::wstring*,menu_item_name
$+A89    005C7FE9     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+A8F    005C7FEF     E8 EC060000                 CALL PathOfEx.005C86E0                            ; tag_NpcPanel添加MenuItem
$+A94    005C7FF4     C645 FC 18                  MOV BYTE PTR SS:[EBP-0x4],0x18
$+A98    005C7FF8     8B45 CC                     MOV EAX,DWORD PTR SS:[EBP-0x34]
$+A9B    005C7FFB     83F8 08                     CMP EAX,0x8
$+A9E    005C7FFE     72 0F                       JB SHORT PathOfEx.005C800F
$+AA0    005C8000     40                          INC EAX
$+AA1    005C8001     6A 02                       PUSH 0x2
$+AA3    005C8003     50                          PUSH EAX
$+AA4    005C8004     FF75 B8                     PUSH DWORD PTR SS:[EBP-0x48]
$+AA7    005C8007     E8 2448EAFF                 CALL PathOfEx.0046C830
$+AAC    005C800C     83C4 0C                     ADD ESP,0xC
$+AAF    005C800F     8B75 08                     MOV ESI,DWORD PTR SS:[EBP+0x8]
$+AB2    005C8012     83C7 0C                     ADD EDI,0xC
$+AB5    005C8015     8D47 FC                     LEA EAX,DWORD PTR DS:[EDI-0x4]
$+AB8    005C8018     3B45 9C                     CMP EAX,DWORD PTR SS:[EBP-0x64]
$+ABB    005C801B   ^ 0F85 9FFCFFFF               JNZ PathOfEx.005C7CC0                             ; 循环结束
$+AC1    005C8021     8A4D 13                     MOV CL,BYTE PTR SS:[EBP+0x13]
$+AC4    005C8024     8B7D 9C                     MOV EDI,DWORD PTR SS:[EBP-0x64]
$+AC7    005C8027     84C9                        TEST CL,CL
$+AC9    005C8029     74 49                       JE SHORT PathOfEx.005C8074
$+ACB    005C802B     83FE 08                     CMP ESI,0x8
$+ACE    005C802E     74 44                       JE SHORT PathOfEx.005C8074
$+AD0    005C8030     83FE 09                     CMP ESI,0x9
$+AD3    005C8033     74 3F                       JE SHORT PathOfEx.005C8074
$+AD5    005C8035     83FE 01                     CMP ESI,0x1
$+AD8    005C8038     74 3A                       JE SHORT PathOfEx.005C8074
$+ADA    005C803A     83FE 0B                     CMP ESI,0xB
$+ADD    005C803D     75 1B                       JNZ SHORT PathOfEx.005C805A
$+ADF    005C803F     8B83 C8080000               MOV EAX,DWORD PTR DS:[EBX+0x8C8]
$+AE5    005C8045     8B80 943A0000               MOV EAX,DWORD PTR DS:[EAX+0x3A94]
$+AEB    005C804B     8B40 08                     MOV EAX,DWORD PTR DS:[EAX+0x8]
$+AEE    005C804E     8A80 DE000000               MOV AL,BYTE PTR DS:[EAX+0xDE]
$+AF4    005C8054     84C0                        TEST AL,AL
$+AF6    005C8056     75 1C                       JNZ SHORT PathOfEx.005C8074
$+AF8    005C8058     EB 0A                       JMP SHORT PathOfEx.005C8064
$+AFA    005C805A     83FE 0D                     CMP ESI,0xD
$+AFD    005C805D     74 15                       JE SHORT PathOfEx.005C8074
$+AFF    005C805F     83FE 0E                     CMP ESI,0xE
$+B02    005C8062     74 10                       JE SHORT PathOfEx.005C8074
$+B04    005C8064     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+B0A    005C806A     E8 A1110000                 CALL PathOfEx.005C9210
$+B0F    005C806F     32C9                        XOR CL,CL
$+B11    005C8071     884D 13                     MOV BYTE PTR SS:[EBP+0x13],CL
$+B14    005C8074     46                          INC ESI
$+B15    005C8075     8975 08                     MOV DWORD PTR SS:[EBP+0x8],ESI
$+B18    005C8078     83FE 17                     CMP ESI,0x17
$+B1B    005C807B   ^ 0F82 2FFCFFFF               JB PathOfEx.005C7CB0
$+B21    005C8081     84C9                        TEST CL,CL
$+B23    005C8083     74 0B                       JE SHORT PathOfEx.005C8090
$+B25    005C8085     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+B2B    005C808B     E8 80110000                 CALL PathOfEx.005C9210
$+B30    005C8090     C745 B0 00000000            MOV DWORD PTR SS:[EBP-0x50],0x0
$+B37    005C8097     C745 B4 00000000            MOV DWORD PTR SS:[EBP-0x4C],0x0
$+B3E    005C809E     C745 E8 00000000            MOV DWORD PTR SS:[EBP-0x18],0x0
$+B45    005C80A5     C745 EC 00000000            MOV DWORD PTR SS:[EBP-0x14],0x0
$+B4C    005C80AC     BA AB000000                 MOV EDX,0xAB
$+B51    005C80B1     C645 FC 29                  MOV BYTE PTR SS:[EBP-0x4],0x29
$+B55    005C80B5     8D4D B8                     LEA ECX,DWORD PTR SS:[EBP-0x48]
$+B58    005C80B8     E8 13541500                 CALL PathOfEx.0071D4D0
$+B5D    005C80BD     F3:0F1055 DC                MOVSS XMM2,DWORD PTR SS:[EBP-0x24]
$+B62    005C80C2     8D4D B0                     LEA ECX,DWORD PTR SS:[EBP-0x50]
$+B65    005C80C5     6A 01                       PUSH 0x1
$+B67    005C80C7     51                          PUSH ECX
$+B68    005C80C8     6A 39                       PUSH 0x39
$+B6A    005C80CA     6A 00                       PUSH 0x0
$+B6C    005C80CC     8D4D E8                     LEA ECX,DWORD PTR SS:[EBP-0x18]
$+B6F    005C80CF     C645 FC 2A                  MOV BYTE PTR SS:[EBP-0x4],0x2A
$+B73    005C80D3     51                          PUSH ECX
$+B74    005C80D4     50                          PUSH EAX
$+B75    005C80D5     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+B7B    005C80DB     8BCB                        MOV ECX,EBX
$+B7D    005C80DD     E8 FE050000                 CALL PathOfEx.005C86E0
$+B82    005C80E2     8B45 CC                     MOV EAX,DWORD PTR SS:[EBP-0x34]
$+B85    005C80E5     83F8 08                     CMP EAX,0x8
$+B88    005C80E8     72 0F                       JB SHORT PathOfEx.005C80F9
$+B8A    005C80EA     40                          INC EAX
$+B8B    005C80EB     6A 02                       PUSH 0x2
$+B8D    005C80ED     50                          PUSH EAX
$+B8E    005C80EE     FF75 B8                     PUSH DWORD PTR SS:[EBP-0x48]
$+B91    005C80F1     E8 3A47EAFF                 CALL PathOfEx.0046C830
$+B96    005C80F6     83C4 0C                     ADD ESP,0xC
$+B99    005C80F9     8D4D B0                     LEA ECX,DWORD PTR SS:[EBP-0x50]
$+B9C    005C80FC     E8 FF05ECFF                 CALL PathOfEx.00488700
$+BA1    005C8101     C645 FC 09                  MOV BYTE PTR SS:[EBP-0x4],0x9
$+BA5    005C8105     8B75 98                     MOV ESI,DWORD PTR SS:[EBP-0x68]
$+BA8    005C8108     85F6                        TEST ESI,ESI
$+BAA    005C810A     74 2E                       JE SHORT PathOfEx.005C813A
$+BAC    005C810C     51                          PUSH ECX
$+BAD    005C810D     8BD7                        MOV EDX,EDI
$+BAF    005C810F     8BCE                        MOV ECX,ESI
$+BB1    005C8111     E8 6A19EEFF                 CALL PathOfEx.004A9A80
$+BB6    005C8116     8B4D A0                     MOV ECX,DWORD PTR SS:[EBP-0x60]
$+BB9    005C8119     B8 ABAAAA2A                 MOV EAX,0x2AAAAAAB
$+BBE    005C811E     2BCE                        SUB ECX,ESI
$+BC0    005C8120     83C4 04                     ADD ESP,0x4
$+BC3    005C8123     F7E9                        IMUL ECX
$+BC5    005C8125     6A 0C                       PUSH 0xC
$+BC7    005C8127     D1FA                        SAR EDX,1
$+BC9    005C8129     8BC2                        MOV EAX,EDX
$+BCB    005C812B     C1E8 1F                     SHR EAX,0x1F
$+BCE    005C812E     03C2                        ADD EAX,EDX
$+BD0    005C8130     50                          PUSH EAX
$+BD1    005C8131     56                          PUSH ESI
$+BD2    005C8132     E8 F946EAFF                 CALL PathOfEx.0046C830
$+BD7    005C8137     83C4 0C                     ADD ESP,0xC
$+BDA    005C813A     8B8B CC080000               MOV ECX,DWORD PTR DS:[EBX+0x8CC]
$+BE0    005C8140     8D55 90                     LEA EDX,DWORD PTR SS:[EBP-0x70]
$+BE3    005C8143     81C1 40080000               ADD ECX,0x840
$+BE9    005C8149     52                          PUSH EDX
$+BEA    005C814A     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+BEC    005C814C     FF50 20                     CALL DWORD PTR DS:[EAX+0x20]
$+BEF    005C814F     8B8B CC080000               MOV ECX,DWORD PTR DS:[EBX+0x8CC]
$+BF5    005C8155     81C1 40080000               ADD ECX,0x840
$+BFB    005C815B     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+BFD    005C815D     FF50 24                     CALL DWORD PTR DS:[EAX+0x24]
$+C00    005C8160     8B30                        MOV ESI,DWORD PTR DS:[EAX]
$+C02    005C8162     85F6                        TEST ESI,ESI
$+C04    005C8164     74 15                       JE SHORT PathOfEx.005C817B
$+C06    005C8166     80BE 80000000 00            CMP BYTE PTR DS:[ESI+0x80],0x0
$+C0D    005C816D     75 07                       JNZ SHORT PathOfEx.005C8176
$+C0F    005C816F     8BCE                        MOV ECX,ESI
$+C11    005C8171     E8 2AD24300                 CALL PathOfEx.00A053A0
$+C16    005C8176     83C6 60                     ADD ESI,0x60
$+C19    005C8179     EB 05                       JMP SHORT PathOfEx.005C8180
$+C1B    005C817B     BE 34391501                 MOV ESI,PathOfEx.01153934
$+C20    005C8180     8B06                        MOV EAX,DWORD PTR DS:[ESI]
$+C22    005C8182     8D55 E8                     LEA EDX,DWORD PTR SS:[EBP-0x18]
$+C25    005C8185     8B8B CC080000               MOV ECX,DWORD PTR DS:[EBX+0x8CC]
$+C2B    005C818B     52                          PUSH EDX
$+C2C    005C818C     66:0F6EC0                   MOVD MM0,EAX
$+C30    005C8190     F30FE6C0                    CVTDQ2PD XMM0,XMM0
$+C34    005C8194     C1E8 1F                     SHR EAX,0x1F
$+C37    005C8197     F20F5804C5 40FCF300         ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
$+C40    005C81A0     660F5AC0                    CVTPD2PS XMM0,XMM0
$+C44    005C81A4     F3:0F1145 E8                MOVSS DWORD PTR SS:[EBP-0x18],XMM0
$+C49    005C81A9     8B46 04                     MOV EAX,DWORD PTR DS:[ESI+0x4]
$+C4C    005C81AC     66:0F6EC0                   MOVD MM0,EAX
$+C50    005C81B0     F30FE6C0                    CVTDQ2PD XMM0,XMM0
$+C54    005C81B4     C1E8 1F                     SHR EAX,0x1F
$+C57    005C81B7     F20F5804C5 40FCF300         ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
$+C60    005C81C0     660F5AC0                    CVTPD2PS XMM0,XMM0
$+C64    005C81C4     F3:0F1145 EC                MOVSS DWORD PTR SS:[EBP-0x14],XMM0
$+C69    005C81C9     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+C6B    005C81CB     FF50 50                     CALL DWORD PTR DS:[EAX+0x50]
$+C6E    005C81CE     8B03                        MOV EAX,DWORD PTR DS:[EBX]
$+C70    005C81D0     F3:0F1045 DC                MOVSS XMM0,DWORD PTR SS:[EBP-0x24]
$+C75    005C81D5     51                          PUSH ECX
$+C76    005C81D6     8BCB                        MOV ECX,EBX
$+C78    005C81D8     F3:0F110424                 MOVSS DWORD PTR SS:[ESP],XMM0
$+C7D    005C81DD     FF50 48                     CALL DWORD PTR DS:[EAX+0x48]
$+C80    005C81E0     8B83 D0080000               MOV EAX,DWORD PTR DS:[EBX+0x8D0]
$+C86    005C81E6     6A 01                       PUSH 0x1
$+C88    005C81E8     6A 01                       PUSH 0x1
$+C8A    005C81EA     8B88 F80B0000               MOV ECX,DWORD PTR DS:[EAX+0xBF8]
$+C90    005C81F0     C680 F40B0000 00            MOV BYTE PTR DS:[EAX+0xBF4],0x0
$+C97    005C81F7     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+C99    005C81F9     FF90 38010000               CALL DWORD PTR DS:[EAX+0x138]
$+C9F    005C81FF     8B8B D0080000               MOV ECX,DWORD PTR DS:[EBX+0x8D0]
$+CA5    005C8205     6A 01                       PUSH 0x1
$+CA7    005C8207     6A 01                       PUSH 0x1
$+CA9    005C8209     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+CAB    005C820B     FF90 38010000               CALL DWORD PTR DS:[EAX+0x138]
$+CB1    005C8211     8B8B D4080000               MOV ECX,DWORD PTR DS:[EBX+0x8D4]
$+CB7    005C8217     85C9                        TEST ECX,ECX
$+CB9    005C8219     74 44                       JE SHORT PathOfEx.005C825F
$+CBB    005C821B     C681 F40B0000 00            MOV BYTE PTR DS:[ECX+0xBF4],0x0
$+CC2    005C8222     8B89 F80B0000               MOV ECX,DWORD PTR DS:[ECX+0xBF8]
$+CC8    005C8228     6A 01                       PUSH 0x1
$+CCA    005C822A     6A 01                       PUSH 0x1
$+CCC    005C822C     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+CCE    005C822E     FF90 38010000               CALL DWORD PTR DS:[EAX+0x138]
$+CD4    005C8234     8B8B D4080000               MOV ECX,DWORD PTR DS:[EBX+0x8D4]
$+CDA    005C823A     6A 01                       PUSH 0x1
$+CDC    005C823C     6A 00                       PUSH 0x0
$+CDE    005C823E     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+CE0    005C8240     FF90 38010000               CALL DWORD PTR DS:[EAX+0x138]
$+CE6    005C8246     8B8B D4080000               MOV ECX,DWORD PTR DS:[EBX+0x8D4]
$+CEC    005C824C     81C1 3C080000               ADD ECX,0x83C
$+CF2    005C8252     8B01                        MOV EAX,DWORD PTR DS:[ECX]
$+CF4    005C8254     FF50 10                     CALL DWORD PTR DS:[EAX+0x10]
$+CF7    005C8257     FFB3 D4080000               PUSH DWORD PTR DS:[EBX+0x8D4]
$+CFD    005C825D     EB 06                       JMP SHORT PathOfEx.005C8265
$+CFF    005C825F     FFB3 D0080000               PUSH DWORD PTR DS:[EBX+0x8D0]
$+D05    005C8265     8BCB                        MOV ECX,EBX
$+D07    005C8267     E8 24000000                 CALL PathOfEx.005C8290
$+D0C    005C826C     8D4D 90                     LEA ECX,DWORD PTR SS:[EBP-0x70]
$+D0F    005C826F     E8 8C04ECFF                 CALL PathOfEx.00488700
$+D14    005C8274     8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
$+D17    005C8277     5F                          POP EDI
$+D18    005C8278     5E                          POP ESI
$+D19    005C8279     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
$+D20    005C8280     5B                          POP EBX
$+D21    005C8281     8BE5                        MOV ESP,EBP
$+D23    005C8283     5D                          POP EBP
$+D24    005C8284     C2 0C00                     RETN 0xC
$+D27    005C8287     CC                          INT3
*/
/**/
//tag_点击没物品的格子时左右键都会断到_有物品时不会断到
/*
$-1      005A448F      CC                         INT3
$ ==>    005A4490      56                         PUSH ESI                                             ; tag_点击没物品的格子时左右键都会断到_有物品时不会断到
$+1      005A4491      57                         PUSH EDI                                             ; ecx=ui对象
$+2      005A4492      8B7C24 0C                  MOV EDI,DWORD PTR SS:[ESP+0xC]                       ; 执行后edi=临时变量，也就是stCD_StateUiLogic.tmp_ui_event_mgr_
$+6      005A4496      8BF1                       MOV ESI,ECX
$+8      005A4498      57                         PUSH EDI
$+9      005A4499      56                         PUSH ESI
$+A      005A449A      8D8E 84020000              LEA ECX,DWORD PTR DS:[ESI+0x284]
$+10     005A44A0      E8 9BD3EEFF                CALL PathOfEx.00491840
$+15     005A44A5      83BE E8070000 00           CMP DWORD PTR DS:[ESI+0x7E8],0x0
$+1C     005A44AC      74 34                      JE SHORT PathOfEx.005A44E2
$+1E     005A44AE      8B86 EC070000              MOV EAX,DWORD PTR DS:[ESI+0x7EC]
$+24     005A44B4      8547 14                    TEST DWORD PTR DS:[EDI+0x14],EAX
$+27     005A44B7      74 29                      JE SHORT PathOfEx.005A44E2
$+29     005A44B9      8B8E 58070000              MOV ECX,DWORD PTR DS:[ESI+0x758]
$+2F     005A44BF      85C9                       TEST ECX,ECX
$+31     005A44C1      74 1F                      JE SHORT PathOfEx.005A44E2
$+33     005A44C3      6A 00                      PUSH 0x0
$+35     005A44C5      E8 66324700                CALL PathOfEx.00A17730
$+3A     005A44CA      51                         PUSH ECX
$+3B     005A44CB      8B8E 58070000              MOV ECX,DWORD PTR DS:[ESI+0x758]
$+41     005A44D1      8D47 0C                    LEA EAX,DWORD PTR DS:[EDI+0xC]
$+44     005A44D4      6A 01                      PUSH 0x1
$+46     005A44D6      50                         PUSH EAX
$+47     005A44D7      FFB6 E8070000              PUSH DWORD PTR DS:[ESI+0x7E8]
$+4D     005A44DD      E8 6E2F4700                CALL PathOfEx.00A17450
$+52     005A44E2      57                         PUSH EDI
$+53     005A44E3      56                         PUSH ESI
$+54     005A44E4      8BCE                       MOV ECX,ESI
$+56     005A44E6      E8 15020000                CALL PathOfEx.005A4700                               ; tag_点击格子会断到_包括左键和右键_不管有没有物品
$+5B     005A44EB      5F                         POP EDI
$+5C     005A44EC      5E                         POP ESI
$+5D     005A44ED      C2 0400                    RETN 0x4
$+60     005A44F0      8B4424 08                  MOV EAX,DWORD PTR SS:[ESP+0x8]
$+64     005A44F4      C681 8C090000 01           MOV BYTE PTR DS:[ECX+0x98C],0x1
$+6B     005A44FB      C600 01                    MOV BYTE PTR DS:[EAX],0x1
$+6E     005A44FE      C2 0800                    RETN 0x8
$+71     005A4501      CC                         INT3
*/
/**/
//tag_点击格子会断到_包括左键和右键_不管有没有物品
/*
$-1      005A46FF      CC                         INT3
$ ==>    005A4700      51                         PUSH ECX                                             ; tag_点击格子会断到_包括左键和右键_不管有没有物品
$+1      005A4701      56                         PUSH ESI                                             ; ecx=ui对象,p1=子ui对象,p2是事件参数,p2是个临时变量，=stCD_StateUiLogic.tmp_ui_event_mgr_
$+2      005A4702      8B7424 10                  MOV ESI,DWORD PTR SS:[ESP+0x10]
$+6      005A4706      57                         PUSH EDI
$+7      005A4707      8BF9                       MOV EDI,ECX
$+9      005A4709      803E 00                    CMP BYTE PTR DS:[ESI],0x0
$+C      005A470C      75 54                      JNZ SHORT PathOfEx.005A4762                          ; 点击有物品的格子时，跳过
$+E      005A470E      837E 14 01                 CMP DWORD PTR DS:[ESI+0x14],0x1
$+12     005A4712      C606 01                    MOV BYTE PTR DS:[ESI],0x1
$+15     005A4715      74 31                      JE SHORT PathOfEx.005A4748                           ; 点击没物品且鼠标未依附过物品的格子时，跳过
$+17     005A4717      8B8F 64090000              MOV ECX,DWORD PTR DS:[EDI+0x964]
$+1D     005A471D      83B9 34090000 00           CMP DWORD PTR DS:[ECX+0x934],0x0
$+24     005A4724      74 1C                      JE SHORT PathOfEx.005A4742
$+26     005A4726      83B9 38090000 01           CMP DWORD PTR DS:[ECX+0x938],0x1
$+2D     005A472D      74 13                      JE SHORT PathOfEx.005A4742
$+2F     005A472F      6A FF                      PUSH -0x1
$+31     005A4731      6A 00                      PUSH 0x0
$+33     005A4733      6A 00                      PUSH 0x0
$+35     005A4735      6A 00                      PUSH 0x0
$+37     005A4737      6A 00                      PUSH 0x0
$+39     005A4739      6A 00                      PUSH 0x0
$+3B     005A473B      6A 00                      PUSH 0x0
$+3D     005A473D      E8 8E56FFFF                CALL PathOfEx.00599DD0
$+42     005A4742      837E 14 01                 CMP DWORD PTR DS:[ESI+0x14],0x1
$+46     005A4746      75 1A                      JNZ SHORT PathOfEx.005A4762
$+48     005A4748      8B87 64090000              MOV EAX,DWORD PTR DS:[EDI+0x964]                     ; 执行后eax=ui对象
$+4E     005A474E      85C0                       TEST EAX,EAX
$+50     005A4750      74 10                      JE SHORT PathOfEx.005A4762
$+52     005A4752      83B8 38090000 00           CMP DWORD PTR DS:[EAX+0x938],0x0
$+59     005A4759      74 07                      JE SHORT PathOfEx.005A4762
$+5B     005A475B      8BCF                       MOV ECX,EDI
$+5D     005A475D      E8 0E000000                CALL PathOfEx.005A4770                               ; tag_点击格子之放下物品
$+62     005A4762      5F                         POP EDI
$+63     005A4763      5E                         POP ESI
$+64     005A4764      59                         POP ECX
$+65     005A4765      C2 0800                    RETN 0x8
$+68     005A4768      CC                         INT3
*/
/**/
//tag_鼠标点击会断到_鼠标左键或右键都能断到
/*
$-1      009EE94F      CC                         INT3
$ ==>    009EE950      56                         PUSH ESI                                             ; tag_鼠标点击会断到_鼠标左键或右键都能断到
$+1      009EE951      57                         PUSH EDI                                             ; ecx=ui对象
$+2      009EE952      8B7C24 0C                  MOV EDI,DWORD PTR SS:[ESP+0xC]
$+6      009EE956      8BF1                       MOV ESI,ECX
$+8      009EE958      57                         PUSH EDI
$+9      009EE959      56                         PUSH ESI
$+A      009EE95A      8D8E 84020000              LEA ECX,DWORD PTR DS:[ESI+0x284]
$+10     009EE960      E8 DB2EAAFF                CALL PathOfEx.00491840
$+15     009EE965      83BE E8070000 00           CMP DWORD PTR DS:[ESI+0x7E8],0x0
$+1C     009EE96C      74 34                      JE SHORT PathOfEx.009EE9A2
$+1E     009EE96E      8B86 EC070000              MOV EAX,DWORD PTR DS:[ESI+0x7EC]
$+24     009EE974      8547 14                    TEST DWORD PTR DS:[EDI+0x14],EAX
$+27     009EE977      74 29                      JE SHORT PathOfEx.009EE9A2
$+29     009EE979      8B8E 58070000              MOV ECX,DWORD PTR DS:[ESI+0x758]
$+2F     009EE97F      85C9                       TEST ECX,ECX
$+31     009EE981      74 1F                      JE SHORT PathOfEx.009EE9A2
$+33     009EE983      6A 00                      PUSH 0x0
$+35     009EE985      E8 A68D0200                CALL PathOfEx.00A17730
$+3A     009EE98A      51                         PUSH ECX
$+3B     009EE98B      8B8E 58070000              MOV ECX,DWORD PTR DS:[ESI+0x758]
$+41     009EE991      8D47 0C                    LEA EAX,DWORD PTR DS:[EDI+0xC]
$+44     009EE994      6A 01                      PUSH 0x1
$+46     009EE996      50                         PUSH EAX
$+47     009EE997      FFB6 E8070000              PUSH DWORD PTR DS:[ESI+0x7E8]
$+4D     009EE99D      E8 AE8A0200                CALL PathOfEx.00A17450
$+52     009EE9A2      5F                         POP EDI
$+53     009EE9A3      5E                         POP ESI
$+54     009EE9A4      C2 0400                    RETN 0x4
$+57     009EE9A7      CC                         INT3
*/
//tag_点击右键_左键物品会断到
/*
$-2      005A615E     CC                         INT3
$-1      005A615F     CC                         INT3                                             ; 这是个响应事件处理函数
$ ==>    005A6160     55                         PUSH EBP                                         ; tag_点击右键_左键物品会断到
$+1      005A6161     8BEC                       MOV EBP,ESP						;ecx=stCD_UiGridPanel*
$+3      005A6163     83E4 F8                    AND ESP,0xFFFFFFF8				;p1=stCD_UiItemGrid*
$+6      005A6166     83EC 1C                    SUB ESP,0x1C
$+9      005A6169     53                         PUSH EBX
$+A      005A616A     56                         PUSH ESI
$+B      005A616B     8B75 0C                    MOV ESI,DWORD PTR SS:[EBP+0xC]
$+E      005A616E     57                         PUSH EDI
$+F      005A616F     8BF9                       MOV EDI,ECX
$+11     005A6171     837E 14 01                 CMP DWORD PTR DS:[ESI+0x14],0x1
$+15     005A6175     75 05                      JNZ SHORT PathOfEx.005A617C
$+17     005A6177     E8 A4020000                CALL PathOfEx.005A6420
$+1C     005A617C     8B87 58070000              MOV EAX,DWORD PTR DS:[EDI+0x758]
$+22     005A6182     8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$+25     005A6185     894424 10                  MOV DWORD PTR SS:[ESP+0x10],EAX
$+29     005A6189     3B98 A80A0000              CMP EBX,DWORD PTR DS:[EAX+0xAA8]
$+2F     005A618F     75 2A                      JNZ SHORT PathOfEx.005A61BB
$+31     005A6191     FF15 60E4D800              CALL DWORD PTR DS:[0xD8E460]
$+37     005A6197     8B4424 10                  MOV EAX,DWORD PTR SS:[ESP+0x10]
$+3B     005A619B     8B88 A80A0000              MOV ECX,DWORD PTR DS:[EAX+0xAA8]
$+41     005A61A1     85C9                       TEST ECX,ECX
$+43     005A61A3     74 0C                      JE SHORT PathOfEx.005A61B1
$+45     005A61A5     8B01                       MOV EAX,DWORD PTR DS:[ECX]
$+47     005A61A7     FF90 04010000              CALL DWORD PTR DS:[EAX+0x104]
$+4D     005A61AD     8B4424 10                  MOV EAX,DWORD PTR SS:[ESP+0x10]
$+51     005A61B1     C780 A80A0000 00000000     MOV DWORD PTR DS:[EAX+0xAA8],0x0
$+5B     005A61BB     803E 00                    CMP BYTE PTR DS:[ESI],0x0
$+5E     005A61BE     0F85 48020000              JNZ PathOfEx.005A640C
$+64     005A61C4     8B87 64090000              MOV EAX,DWORD PTR DS:[EDI+0x964]
$+6A     005A61CA     8B80 38090000              MOV EAX,DWORD PTR DS:[EAX+0x938]
$+70     005A61D0     85C0                       TEST EAX,EAX
$+72     005A61D2     74 09                      JE SHORT PathOfEx.005A61DD
$+74     005A61D4     83F8 06                    CMP EAX,0x6
$+77     005A61D7     0F85 2F020000              JNZ PathOfEx.005A640C
$+7D     005A61DD     C606 01                    MOV BYTE PTR DS:[ESI],0x1
$+80     005A61E0     8D4C24 18                  LEA ECX,DWORD PTR SS:[ESP+0x18]
$+84     005A61E4     8B03                       MOV EAX,DWORD PTR DS:[EBX]
$+86     005A61E6     51                         PUSH ECX
$+87     005A61E7     8BCB                       MOV ECX,EBX
$+89     005A61E9     FF50 44                    CALL DWORD PTR DS:[EAX+0x44]
$+8C     005A61EC     8BF0                       MOV ESI,EAX
$+8E     005A61EE     8BCB                       MOV ECX,EBX
$+90     005A61F0     8D4424 10                  LEA EAX,DWORD PTR SS:[ESP+0x10]
$+94     005A61F4     50                         PUSH EAX
$+95     005A61F5     E8 56714400                CALL PathOfEx.009ED350
$+9A     005A61FA     F3:0F1016                  MOVSS XMM2,DWORD PTR DS:[ESI]
$+9E     005A61FE     8BCB                       MOV ECX,EBX
$+A0     005A6200     F3:0F105E 04               MOVSS XMM3,DWORD PTR DS:[ESI+0x4]
$+A5     005A6205     F3:0F1020                  MOVSS XMM4,DWORD PTR DS:[EAX]
$+A9     005A6209     F3:0F1068 04               MOVSS XMM5,DWORD PTR DS:[EAX+0x4]
$+AE     005A620E     F3:0F58D4                  ADDSS XMM2,XMM4
$+B2     005A6212     8D4424 10                  LEA EAX,DWORD PTR SS:[ESP+0x10]
$+B6     005A6216     50                         PUSH EAX
$+B7     005A6217     F3:0F58DD                  ADDSS XMM3,XMM5
$+BB     005A621B     E8 F034F1FF                CALL PathOfEx.004B9710
$+C0     005A6220     F3:0F107424 10             MOVSS XMM6,DWORD PTR SS:[ESP+0x10]
$+C6     005A6226     8B4D 0C                    MOV ECX,DWORD PTR SS:[EBP+0xC]
$+C9     005A6229     0F28C6                     MOVAPS XMM0,XMM6
$+CC     005A622C     F3:0F59C4                  MULSS XMM0,XMM4
$+D0     005A6230     F3:0F106424 14             MOVSS XMM4,DWORD PTR SS:[ESP+0x14]
$+D6     005A6236     F3:0F1049 10               MOVSS XMM1,DWORD PTR DS:[ECX+0x10]
$+DB     005A623B     0F28FC                     MOVAPS XMM7,XMM4
$+DE     005A623E     F3:0F59F2                  MULSS XMM6,XMM2
$+E2     005A6242     F3:0F1051 0C               MOVSS XMM2,DWORD PTR DS:[ECX+0xC]
$+E7     005A6247     0F2FD0                     COMISS XMM2,XMM0
$+EA     005A624A     F3:0F59FD                  MULSS XMM7,XMM5
$+EE     005A624E     F3:0F59E3                  MULSS XMM4,XMM3
$+F2     005A6252     0F82 B4010000              JB PathOfEx.005A640C
$+F8     005A6258     0F2FCF                     COMISS XMM1,XMM7
$+FB     005A625B     0F82 AB010000              JB PathOfEx.005A640C
$+101    005A6261     0F2FF2                     COMISS XMM6,XMM2
$+104    005A6264     0F86 A2010000              JBE PathOfEx.005A640C
$+10A    005A626A     0F2FE1                     COMISS XMM4,XMM1
$+10D    005A626D     0F86 99010000              JBE PathOfEx.005A640C
$+113    005A6273     80BF 26080000 00           CMP BYTE PTR DS:[EDI+0x826],0x0
$+11A    005A627A     0F84 92000000              JE PathOfEx.005A6312
$+120    005A6280     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+122    005A6282     8D4C24 18                  LEA ECX,DWORD PTR SS:[ESP+0x18]
$+126    005A6286     51                         PUSH ECX
$+127    005A6287     8BCF                       MOV ECX,EDI
$+129    005A6289     FF50 44                    CALL DWORD PTR DS:[EAX+0x44]
$+12C    005A628C     8BF0                       MOV ESI,EAX
$+12E    005A628E     8BCF                       MOV ECX,EDI
$+130    005A6290     8D4424 10                  LEA EAX,DWORD PTR SS:[ESP+0x10]
$+134    005A6294     50                         PUSH EAX
$+135    005A6295     E8 B6704400                CALL PathOfEx.009ED350
$+13A    005A629A     F3:0F105E 04               MOVSS XMM3,DWORD PTR DS:[ESI+0x4]
$+13F    005A629F     8BCF                       MOV ECX,EDI
$+141    005A62A1     F3:0F1020                  MOVSS XMM4,DWORD PTR DS:[EAX]
$+145    005A62A5     F3:0F1068 04               MOVSS XMM5,DWORD PTR DS:[EAX+0x4]
$+14A    005A62AA     0F28D4                     MOVAPS XMM2,XMM4
$+14D    005A62AD     F3:0F5816                  ADDSS XMM2,DWORD PTR DS:[ESI]
$+151    005A62B1     8D4424 10                  LEA EAX,DWORD PTR SS:[ESP+0x10]
$+155    005A62B5     50                         PUSH EAX
$+156    005A62B6     F3:0F58DD                  ADDSS XMM3,XMM5
$+15A    005A62BA     E8 5134F1FF                CALL PathOfEx.004B9710
$+15F    005A62BF     F3:0F107424 10             MOVSS XMM6,DWORD PTR SS:[ESP+0x10]
$+165    005A62C5     8B4D 0C                    MOV ECX,DWORD PTR SS:[EBP+0xC]
$+168    005A62C8     0F28C6                     MOVAPS XMM0,XMM6
$+16B    005A62CB     F3:0F59C4                  MULSS XMM0,XMM4
$+16F    005A62CF     F3:0F106424 14             MOVSS XMM4,DWORD PTR SS:[ESP+0x14]
$+175    005A62D5     F3:0F1049 10               MOVSS XMM1,DWORD PTR DS:[ECX+0x10]
$+17A    005A62DA     0F28FC                     MOVAPS XMM7,XMM4
$+17D    005A62DD     F3:0F59F2                  MULSS XMM6,XMM2
$+181    005A62E1     F3:0F1051 0C               MOVSS XMM2,DWORD PTR DS:[ECX+0xC]
$+186    005A62E6     0F2FD0                     COMISS XMM2,XMM0
$+189    005A62E9     F3:0F59FD                  MULSS XMM7,XMM5
$+18D    005A62ED     F3:0F59E3                  MULSS XMM4,XMM3
$+191    005A62F1     0F82 15010000              JB PathOfEx.005A640C
$+197    005A62F7     0F2FCF                     COMISS XMM1,XMM7
$+19A    005A62FA     0F82 0C010000              JB PathOfEx.005A640C
$+1A0    005A6300     0F2FF2                     COMISS XMM6,XMM2
$+1A3    005A6303     0F86 03010000              JBE PathOfEx.005A640C
$+1A9    005A6309     0F2FE1                     COMISS XMM4,XMM1
$+1AC    005A630C     0F86 FA000000              JBE PathOfEx.005A640C
$+1B2    005A6312     8B97 5C070000              MOV EDX,DWORD PTR DS:[EDI+0x75C]
$+1B8    005A6318     895424 10                  MOV DWORD PTR SS:[ESP+0x10],EDX
$+1BC    005A631C     80BA 26080000 00           CMP BYTE PTR DS:[EDX+0x826],0x0
$+1C3    005A6323     0F84 86000000              JE PathOfEx.005A63AF
$+1C9    005A6329     8B02                       MOV EAX,DWORD PTR DS:[EDX]
$+1CB    005A632B     8D4C24 18                  LEA ECX,DWORD PTR SS:[ESP+0x18]
$+1CF    005A632F     51                         PUSH ECX
$+1D0    005A6330     8BCA                       MOV ECX,EDX
$+1D2    005A6332     FF50 44                    CALL DWORD PTR DS:[EAX+0x44]
$+1D5    005A6335     8B4C24 10                  MOV ECX,DWORD PTR SS:[ESP+0x10]
$+1D9    005A6339     8BF0                       MOV ESI,EAX
$+1DB    005A633B     8D4424 20                  LEA EAX,DWORD PTR SS:[ESP+0x20]
$+1DF    005A633F     50                         PUSH EAX
$+1E0    005A6340     E8 0B704400                CALL PathOfEx.009ED350
$+1E5    005A6345     F3:0F105E 04               MOVSS XMM3,DWORD PTR DS:[ESI+0x4]
$+1EA    005A634A     8B4C24 10                  MOV ECX,DWORD PTR SS:[ESP+0x10]
$+1EE    005A634E     F3:0F1020                  MOVSS XMM4,DWORD PTR DS:[EAX]
$+1F2    005A6352     F3:0F1068 04               MOVSS XMM5,DWORD PTR DS:[EAX+0x4]
$+1F7    005A6357     0F28D4                     MOVAPS XMM2,XMM4
$+1FA    005A635A     F3:0F5816                  ADDSS XMM2,DWORD PTR DS:[ESI]
$+1FE    005A635E     8D4424 18                  LEA EAX,DWORD PTR SS:[ESP+0x18]
$+202    005A6362     50                         PUSH EAX
$+203    005A6363     F3:0F58DD                  ADDSS XMM3,XMM5
$+207    005A6367     E8 A433F1FF                CALL PathOfEx.004B9710
$+20C    005A636C     F3:0F107424 18             MOVSS XMM6,DWORD PTR SS:[ESP+0x18]
$+212    005A6372     8B4D 0C                    MOV ECX,DWORD PTR SS:[EBP+0xC]
$+215    005A6375     0F28C6                     MOVAPS XMM0,XMM6
$+218    005A6378     F3:0F59C4                  MULSS XMM0,XMM4
$+21C    005A637C     F3:0F106424 1C             MOVSS XMM4,DWORD PTR SS:[ESP+0x1C]
$+222    005A6382     F3:0F1049 10               MOVSS XMM1,DWORD PTR DS:[ECX+0x10]
$+227    005A6387     0F28FC                     MOVAPS XMM7,XMM4
$+22A    005A638A     F3:0F59F2                  MULSS XMM6,XMM2
$+22E    005A638E     F3:0F1051 0C               MOVSS XMM2,DWORD PTR DS:[ECX+0xC]
$+233    005A6393     0F2FD0                     COMISS XMM2,XMM0
$+236    005A6396     F3:0F59FD                  MULSS XMM7,XMM5
$+23A    005A639A     F3:0F59E3                  MULSS XMM4,XMM3
$+23E    005A639E     72 6C                      JB SHORT PathOfEx.005A640C
$+240    005A63A0     0F2FCF                     COMISS XMM1,XMM7
$+243    005A63A3     72 67                      JB SHORT PathOfEx.005A640C
$+245    005A63A5     0F2FF2                     COMISS XMM6,XMM2
$+248    005A63A8     76 62                      JBE SHORT PathOfEx.005A640C
$+24A    005A63AA     0F2FE1                     COMISS XMM4,XMM1
$+24D    005A63AD     76 5D                      JBE SHORT PathOfEx.005A640C
$+24F    005A63AF     8B83 5C0A0000              MOV EAX,DWORD PTR DS:[EBX+0xA5C]
$+255    005A63B5     8078 10 00                 CMP BYTE PTR DS:[EAX+0x10],0x0
$+259    005A63B9     75 51                      JNZ SHORT PathOfEx.005A640C
$+25B    005A63BB     8B87 64090000              MOV EAX,DWORD PTR DS:[EDI+0x964]
$+261    005A63C1     83B8 38090000 06           CMP DWORD PTR DS:[EAX+0x938],0x6
$+268    005A63C8     75 1C                      JNZ SHORT PathOfEx.005A63E6
$+26A    005A63CA     FF71 14                    PUSH DWORD PTR DS:[ECX+0x14]
$+26D    005A63CD     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+26F    005A63CF     8BCF                       MOV ECX,EDI
$+271    005A63D1     FFB3 580A0000              PUSH DWORD PTR DS:[EBX+0xA58]
$+277    005A63D7     FF90 68010000              CALL DWORD PTR DS:[EAX+0x168]
$+27D    005A63DD     5F                         POP EDI
$+27E    005A63DE     5E                         POP ESI
$+27F    005A63DF     5B                         POP EBX
$+280    005A63E0     8BE5                       MOV ESP,EBP
$+282    005A63E2     5D                         POP EBP
$+283    005A63E3     C2 0800                    RETN 0x8
$+286    005A63E6     8B41 14                    MOV EAX,DWORD PTR DS:[ECX+0x14]
$+289    005A63E9     83E8 01                    SUB EAX,0x1
$+28C    005A63EC     74 16                      JE SHORT PathOfEx.005A6404
$+28E    005A63EE     83E8 01                    SUB EAX,0x1
$+291    005A63F1     75 19                      JNZ SHORT PathOfEx.005A640C
$+293    005A63F3     53                         PUSH EBX
$+294    005A63F4     8BCF                       MOV ECX,EDI
$+296    005A63F6     E8 C5ECFFFF                CALL PathOfEx.005A50C0                           ; tag_鼠标右键点击物品会断到
$+29B    005A63FB     5F                         POP EDI
$+29C    005A63FC     5E                         POP ESI
$+29D    005A63FD     5B                         POP EBX
$+29E    005A63FE     8BE5                       MOV ESP,EBP
$+2A0    005A6400     5D                         POP EBP
$+2A1    005A6401     C2 0800                    RETN 0x8
$+2A4    005A6404     53                         PUSH EBX
$+2A5    005A6405     8BCF                       MOV ECX,EDI
$+2A7    005A6407     E8 34ECFFFF                CALL PathOfEx.005A5040			;call tag_左键点击stCD_UiItemGrid会断到
$+2AC    005A640C     5F                         POP EDI
$+2AD    005A640D     5E                         POP ESI
$+2AE    005A640E     5B                         POP EBX
$+2AF    005A640F     8BE5                       MOV ESP,EBP
$+2B1    005A6411     5D                         POP EBP
$+2B2    005A6412     C2 0800                    RETN 0x8
$+2B5    005A6415     CC                         INT3
*/
//tag_鼠标右键点击物品会断到
/*
$-1      005A50BF     CC                         INT3
$ ==>    005A50C0     83EC 10                    SUB ESP,0x10                                     ; tag_鼠标右键点击物品会断到
$+3      005A50C3     53                         PUSH EBX                                         ; ecx和参数都是ui对象,p1应该是物品格子ui对象
$+4      005A50C4     55                         PUSH EBP
$+5      005A50C5     8B6C24 1C                  MOV EBP,DWORD PTR SS:[ESP+0x1C]
$+9      005A50C9     56                         PUSH ESI
$+A      005A50CA     57                         PUSH EDI
$+B      005A50CB     8BF9                       MOV EDI,ECX
$+D      005A50CD     8B9D 580A0000              MOV EBX,DWORD PTR SS:[EBP+0xA58]
$+13     005A50D3     6A 02                      PUSH 0x2
$+15     005A50D5     53                         PUSH EBX
$+16     005A50D6     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+18     005A50D8     FF90 68010000              CALL DWORD PTR DS:[EAX+0x168]
$+1E     005A50DE     8BB5 5C0A0000              MOV ESI,DWORD PTR SS:[EBP+0xA5C]                 ; 执行后esi=stCD_EntityItemBased*
$+24     005A50E4     897424 18                  MOV DWORD PTR SS:[ESP+0x18],ESI
$+28     005A50E8     8B0E                       MOV ECX,DWORD PTR DS:[ESI]
$+2A     005A50EA     85C9                       TEST ECX,ECX
$+2C     005A50EC     74 05                      JE SHORT PathOfEx.005A50F3
$+2E     005A50EE     E8 CD441700                CALL PathOfEx.007195C0
$+33     005A50F3     8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+35     005A50F5     C74424 24 308EED00         MOV DWORD PTR SS:[ESP+0x24],PathOfEx.00ED8E30    ; Usable
$+3D     005A50FD     8B70 30                    MOV ESI,DWORD PTR DS:[EAX+0x30]                  ; 执行后esi=stCD_DbTblBase*
$+40     005A5100     8D4424 24                  LEA EAX,DWORD PTR SS:[ESP+0x24]
$+44     005A5104     50                         PUSH EAX
$+45     005A5105     8D4424 18                  LEA EAX,DWORD PTR SS:[ESP+0x18]
$+49     005A5109     50                         PUSH EAX
$+4A     005A510A     8D4E 14                    LEA ECX,DWORD PTR DS:[ESI+0x14]                  ; 执行后ecx=&stCD_DbTblEntity.attr_idx_
$+4D     005A510D     E8 2E6FECFF                CALL PathOfEx.0046C040                           ; FindAttrIdxInfo
$+52     005A5112     8B4424 14                  MOV EAX,DWORD PTR SS:[ESP+0x14]
$+56     005A5116     3B46 18                    CMP EAX,DWORD PTR DS:[ESI+0x18]
$+59     005A5119     0F84 ED000000              JE PathOfEx.005A520C
$+5F     005A511F     8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]                   ; 执行后ecx=属性索引
$+62     005A5122     83F9 FF                    CMP ECX,-0x1
$+65     005A5125     0F84 E1000000              JE PathOfEx.005A520C
$+6B     005A512B     8B4424 18                  MOV EAX,DWORD PTR SS:[ESP+0x18]
$+6F     005A512F     8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$+72     005A5132     8B0488                     MOV EAX,DWORD PTR DS:[EAX+ECX*4]                 ; 执行后eax=stCD_EntityAttrUsable*
$+75     005A5135     85C0                       TEST EAX,EAX
$+77     005A5137     0F84 CF000000              JE PathOfEx.005A520C
$+7D     005A513D     8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]                   ; 执行后eax=stCD_EntityAttrDbUsable*
$+80     005A5140     8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]                   ; 执行后，eax=enCD_UsableType
$+83     005A5143     48                         DEC EAX
$+84     005A5144     83F8 05                    CMP EAX,0x5
$+87     005A5147     0F87 BF000000              JA PathOfEx.005A520C
$+8D     005A514D     FF2485 18525A00            JMP DWORD PTR DS:[EAX*4+0x5A5218]
$+94     005A5154     F3:0F100D 3CEDF300         MOVSS XMM1,DWORD PTR DS:[0xF3ED3C]
$+9C     005A515C     F3:0F5E8F 60090000         DIVSS XMM1,DWORD PTR DS:[EDI+0x960]
$+A4     005A5164     8B85 60070000              MOV EAX,DWORD PTR SS:[EBP+0x760]
$+AA     005A516A     894424 18                  MOV DWORD PTR SS:[ESP+0x18],EAX
$+AE     005A516E     8B85 64070000              MOV EAX,DWORD PTR SS:[EBP+0x764]
$+B4     005A5174     894424 1C                  MOV DWORD PTR SS:[ESP+0x1C],EAX
$+B8     005A5178     0F28C1                     MOVAPS XMM0,XMM1
$+BB     005A517B     F3:0F594C24 1C             MULSS XMM1,DWORD PTR SS:[ESP+0x1C]
$+C1     005A5181     F3:0F594424 18             MULSS XMM0,DWORD PTR SS:[ESP+0x18]
$+C7     005A5187     F3:0F114C24 24             MOVSS DWORD PTR SS:[ESP+0x24],XMM1
$+CD     005A518D     E8 9E786D00                CALL PathOfEx.00C7CA30
$+D2     005A5192     F3:0F104424 24             MOVSS XMM0,DWORD PTR SS:[ESP+0x24]
$+D8     005A5198     894424 18                  MOV DWORD PTR SS:[ESP+0x18],EAX
$+DC     005A519C     E8 8F786D00                CALL PathOfEx.00C7CA30
$+E1     005A51A1     894424 1C                  MOV DWORD PTR SS:[ESP+0x1C],EAX
$+E5     005A51A5     8D4C24 18                  LEA ECX,DWORD PTR SS:[ESP+0x18]
$+E9     005A51A9     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+EB     005A51AB     51                         PUSH ECX
$+EC     005A51AC     53                         PUSH EBX
$+ED     005A51AD     8BCF                       MOV ECX,EDI
$+EF     005A51AF     FF90 90010000              CALL DWORD PTR DS:[EAX+0x190]
$+F5     005A51B5     5F                         POP EDI
$+F6     005A51B6     5E                         POP ESI
$+F7     005A51B7     5D                         POP EBP
$+F8     005A51B8     5B                         POP EBX
$+F9     005A51B9     83C4 10                    ADD ESP,0x10
$+FC     005A51BC     C2 0400                    RETN 0x4
$+FF     005A51BF     8B87 4C090000              MOV EAX,DWORD PTR DS:[EDI+0x94C]
$+105    005A51C5     80B8 AC3E0000 00           CMP BYTE PTR DS:[EAX+0x3EAC],0x0
$+10C    005A51CC     74 0F                      JE SHORT PathOfEx.005A51DD
$+10E    005A51CE     8B8D 5C0A0000              MOV ECX,DWORD PTR SS:[EBP+0xA5C]
$+114    005A51D4     E8 C7BDFFFF                CALL PathOfEx.005A0FA0
$+119    005A51D9     84C0                       TEST AL,AL
$+11B    005A51DB     75 24                      JNZ SHORT PathOfEx.005A5201
$+11D    005A51DD     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+11F    005A51DF     8BCF                       MOV ECX,EDI
$+121    005A51E1     53                         PUSH EBX
$+122    005A51E2     FF90 78010000              CALL DWORD PTR DS:[EAX+0x178]                    ; 使用物品上层2
$+128    005A51E8     5F                         POP EDI
$+129    005A51E9     5E                         POP ESI
$+12A    005A51EA     5D                         POP EBP
$+12B    005A51EB     5B                         POP EBX
$+12C    005A51EC     83C4 10                    ADD ESP,0x10
$+12F    005A51EF     C2 0400                    RETN 0x4
$+132    005A51F2     8B87 4C090000              MOV EAX,DWORD PTR DS:[EDI+0x94C]
$+138    005A51F8     80B8 AC3E0000 00           CMP BYTE PTR DS:[EAX+0x3EAC],0x0
$+13F    005A51FF     74 0B                      JE SHORT PathOfEx.005A520C
$+141    005A5201     8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+143    005A5203     8BCF                       MOV ECX,EDI
$+145    005A5205     53                         PUSH EBX
$+146    005A5206     FF90 A4010000              CALL DWORD PTR DS:[EAX+0x1A4]
$+14C    005A520C     5F                         POP EDI
$+14D    005A520D     5E                         POP ESI
$+14E    005A520E     5D                         POP EBP
$+14F    005A520F     5B                         POP EBX
$+150    005A5210     83C4 10                    ADD ESP,0x10
$+153    005A5213     C2 0400                    RETN 0x4
$+156    005A5216     66:90                      NOP
*/
//tag_UI对象处理操作命令
/*
$-D1     004D1CDF      CC                         INT3
$-D0     004D1CE0      56                         PUSH ESI
$-CF     004D1CE1      8BF1                       MOV ESI,ECX
$-CD     004D1CE3      8B0E                       MOV ECX,DWORD PTR DS:[ESI]
$-CB     004D1CE5      85C9                       TEST ECX,ECX
$-C9     004D1CE7      74 33                      JE SHORT PathOfEx.004D1D1C
$-C7     004D1CE9      8B56 04                    MOV EDX,DWORD PTR DS:[ESI+0x4]
$-C4     004D1CEC      51                         PUSH ECX
$-C3     004D1CED      E8 5E0CFDFF                CALL PathOfEx.004A2950
$-BE     004D1CF2      8B0E                       MOV ECX,DWORD PTR DS:[ESI]
$-BC     004D1CF4      8B46 08                    MOV EAX,DWORD PTR DS:[ESI+0x8]
$-B9     004D1CF7      2BC1                       SUB EAX,ECX
$-B7     004D1CF9      6A 08                      PUSH 0x8
$-B5     004D1CFB      C1F8 03                    SAR EAX,0x3
$-B2     004D1CFE      50                         PUSH EAX
$-B1     004D1CFF      51                         PUSH ECX
$-B0     004D1D00      E8 2BABF9FF                CALL PathOfEx.0046C830
$-AB     004D1D05      C706 00000000              MOV DWORD PTR DS:[ESI],0x0
$-A5     004D1D0B      83C4 10                    ADD ESP,0x10
$-A2     004D1D0E      C746 04 00000000           MOV DWORD PTR DS:[ESI+0x4],0x0
$-9B     004D1D15      C746 08 00000000           MOV DWORD PTR DS:[ESI+0x8],0x0
$-94     004D1D1C      5E                         POP ESI
$-93     004D1D1D      C3                         RETN
$-92     004D1D1E      CC                         INT3
$-91     004D1D1F      CC                         INT3
$-90     004D1D20      56                         PUSH ESI
$-8F     004D1D21      8BF1                       MOV ESI,ECX
$-8D     004D1D23      57                         PUSH EDI
$-8C     004D1D24      8B7C24 0C                  MOV EDI,DWORD PTR SS:[ESP+0xC]
$-88     004D1D28      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$-85     004D1D2B      3BF8                       CMP EDI,EAX
$-83     004D1D2D      73 3E                      JNB SHORT PathOfEx.004D1D6D
$-81     004D1D2F      8B0E                       MOV ECX,DWORD PTR DS:[ESI]
$-7F     004D1D31      3BCF                       CMP ECX,EDI
$-7D     004D1D33      77 38                      JA SHORT PathOfEx.004D1D6D
$-7B     004D1D35      2BF9                       SUB EDI,ECX
$-79     004D1D37      C1FF 03                    SAR EDI,0x3
$-76     004D1D3A      3B46 08                    CMP EAX,DWORD PTR DS:[ESI+0x8]
$-73     004D1D3D      75 09                      JNZ SHORT PathOfEx.004D1D48
$-71     004D1D3F      6A 01                      PUSH 0x1
$-6F     004D1D41      8BCE                       MOV ECX,ESI
$-6D     004D1D43      E8 08010000                CALL PathOfEx.004D1E50
$-68     004D1D48      8B56 04                    MOV EDX,DWORD PTR DS:[ESI+0x4]
$-65     004D1D4B      85D2                       TEST EDX,EDX
$-63     004D1D4D      74 43                      JE SHORT PathOfEx.004D1D92
$-61     004D1D4F      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$-5F     004D1D51      8D0CF8                     LEA ECX,DWORD PTR DS:[EAX+EDI*8]
$-5C     004D1D54      8B01                       MOV EAX,DWORD PTR DS:[ECX]
$-5A     004D1D56      8902                       MOV DWORD PTR DS:[EDX],EAX
$-58     004D1D58      C701 00000000              MOV DWORD PTR DS:[ECX],0x0
$-52     004D1D5E      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$-4F     004D1D61      8942 04                    MOV DWORD PTR DS:[EDX+0x4],EAX
$-4C     004D1D64      8346 04 08                 ADD DWORD PTR DS:[ESI+0x4],0x8
$-48     004D1D68      5F                         POP EDI
$-47     004D1D69      5E                         POP ESI
$-46     004D1D6A      C2 0400                    RETN 0x4
$-43     004D1D6D      3B46 08                    CMP EAX,DWORD PTR DS:[ESI+0x8]
$-40     004D1D70      75 09                      JNZ SHORT PathOfEx.004D1D7B
$-3E     004D1D72      6A 01                      PUSH 0x1
$-3C     004D1D74      8BCE                       MOV ECX,ESI
$-3A     004D1D76      E8 D5000000                CALL PathOfEx.004D1E50
$-35     004D1D7B      8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
$-32     004D1D7E      85C9                       TEST ECX,ECX
$-30     004D1D80      74 10                      JE SHORT PathOfEx.004D1D92
$-2E     004D1D82      8B07                       MOV EAX,DWORD PTR DS:[EDI]
$-2C     004D1D84      8901                       MOV DWORD PTR DS:[ECX],EAX
$-2A     004D1D86      C707 00000000              MOV DWORD PTR DS:[EDI],0x0
$-24     004D1D8C      8B47 04                    MOV EAX,DWORD PTR DS:[EDI+0x4]
$-21     004D1D8F      8941 04                    MOV DWORD PTR DS:[ECX+0x4],EAX
$-1E     004D1D92      8346 04 08                 ADD DWORD PTR DS:[ESI+0x4],0x8
$-1A     004D1D96      5F                         POP EDI
$-19     004D1D97      5E                         POP ESI
$-18     004D1D98      C2 0400                    RETN 0x4
$-15     004D1D9B      CC                         INT3
$-14     004D1D9C      CC                         INT3
$-13     004D1D9D      CC                         INT3
$-12     004D1D9E      CC                         INT3
$-11     004D1D9F      CC                         INT3
$-10     004D1DA0      8B4424 04                  MOV EAX,DWORD PTR SS:[ESP+0x4]
$-C      004D1DA4      8B09                       MOV ECX,DWORD PTR DS:[ECX]
$-A      004D1DA6      83E9 04                    SUB ECX,0x4
$-7      004D1DA9      8908                       MOV DWORD PTR DS:[EAX],ECX
$-5      004D1DAB      C2 0800                    RETN 0x8
$-2      004D1DAE      CC                         INT3
$-1      004D1DAF      CC                         INT3
$ ==>    004D1DB0      55                         PUSH EBP                                              ; tag_UI对象处理操作命令
$+1      004D1DB1      8BEC                       MOV EBP,ESP                                           ; 3个参数，都是宽字符串
$+3      004D1DB3      6A FF                      PUSH -0x1                                             ; p1有时候="18"
$+5      004D1DB5      68 8720D200                PUSH PathOfEx.00D22087                                ; p2有时候="ACTION_KEYS"
$+A      004D1DBA      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]                              ; p3有时候="highlight"
$+10     004D1DC0      50                         PUSH EAX
$+11     004D1DC1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+18     004D1DC8      83EC 08                    SUB ESP,0x8
$+1B     004D1DCB      56                         PUSH ESI
$+1C     004D1DCC      57                         PUSH EDI
$+1D     004D1DCD      8BF9                       MOV EDI,ECX
$+1F     004D1DCF      897D EC                    MOV DWORD PTR SS:[EBP-0x14],EDI
$+22     004D1DD2      C745 F0 00000000           MOV DWORD PTR SS:[EBP-0x10],0x0
$+29     004D1DD9      C707 ECE8EA00              MOV DWORD PTR DS:[EDI],PathOfEx.00EAE8EC
$+2F     004D1DDF      C747 10 98E7EA00           MOV DWORD PTR DS:[EDI+0x10],PathOfEx.00EAE798
$+36     004D1DE6      C747 68 4C54DE00           MOV DWORD PTR DS:[EDI+0x68],PathOfEx.00DE544C
$+3D     004D1DED      51                         PUSH ECX
$+3E     004D1DEE      8D77 18                    LEA ESI,DWORD PTR DS:[EDI+0x18]
$+41     004D1DF1      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+48     004D1DF8      56                         PUSH ESI
$+49     004D1DF9      C745 F0 01000000           MOV DWORD PTR SS:[EBP-0x10],0x1
$+50     004D1E00      E8 AB66FAFF                CALL PathOfEx.004784B0
$+55     004D1E05      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+5C     004D1E0C      8BCE                       MOV ECX,ESI
$+5E     004D1E0E      8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+60     004D1E10      6A 03                      PUSH 0x3
$+62     004D1E12      FF75 08                    PUSH DWORD PTR SS:[EBP+0x8]
$+65     004D1E15      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$+68     004D1E18      C70407 64FCE600            MOV DWORD PTR DS:[EDI+EAX],PathOfEx.00E6FC64
$+6F     004D1E1F      8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+71     004D1E21      8B50 04                    MOV EDX,DWORD PTR DS:[EAX+0x4]
$+74     004D1E24      8D42 98                    LEA EAX,DWORD PTR DS:[EDX-0x68]
$+77     004D1E27      89443A FC                  MOV DWORD PTR DS:[EDX+EDI-0x4],EAX
$+7B     004D1E2B      E8 80000000                CALL PathOfEx.004D1EB0
$+80     004D1E30      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+83     004D1E33      8BC7                       MOV EAX,EDI
$+85     004D1E35      5F                         POP EDI                                               ; 44549D28
$+86     004D1E36      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+8D     004D1E3D      5E                         POP ESI
$+8E     004D1E3E      8BE5                       MOV ESP,EBP
$+90     004D1E40      5D                         POP EBP
$+91     004D1E41      C2 0C00                    RETN 0xC
$+94     004D1E44      CC                         INT3
$+95     004D1E45      CC                         INT3
$+96     004D1E46      CC                         INT3
$+97     004D1E47      CC                         INT3
$+98     004D1E48      CC                         INT3
$+99     004D1E49      CC                         INT3
$+9A     004D1E4A      CC                         INT3
$+9B     004D1E4B      CC                         INT3
$+9C     004D1E4C      CC                         INT3
$+9D     004D1E4D      CC                         INT3
$+9E     004D1E4E      CC                         INT3
$+9F     004D1E4F      CC                         INT3
$+A0     004D1E50      56                         PUSH ESI
$+A1     004D1E51      8BF1                       MOV ESI,ECX
$+A3     004D1E53      8B4C24 08                  MOV ECX,DWORD PTR SS:[ESP+0x8]
$+A7     004D1E57      57                         PUSH EDI
$+A8     004D1E58      8B7E 08                    MOV EDI,DWORD PTR DS:[ESI+0x8]
$+AB     004D1E5B      8BC7                       MOV EAX,EDI
$+AD     004D1E5D      8B56 04                    MOV EDX,DWORD PTR DS:[ESI+0x4]
$+B0     004D1E60      2BC2                       SUB EAX,EDX
$+B2     004D1E62      C1F8 03                    SAR EAX,0x3
$+B5     004D1E65      3BC1                       CMP EAX,ECX
$+B7     004D1E67      73 37                      JNB SHORT PathOfEx.004D1EA0
$+B9     004D1E69      2B16                       SUB EDX,DWORD PTR DS:[ESI]
$+BB     004D1E6B      53                         PUSH EBX
$+BC     004D1E6C      BB FFFFFF1F                MOV EBX,0x1FFFFFFF
$+C1     004D1E71      C1FA 03                    SAR EDX,0x3
$+C4     004D1E74      8BC3                       MOV EAX,EBX
$+C6     004D1E76      2BC2                       SUB EAX,EDX
$+C8     004D1E78      3BC1                       CMP EAX,ECX
$+CA     004D1E7A      72 29                      JB SHORT PathOfEx.004D1EA5
$+CC     004D1E7C      2B3E                       SUB EDI,DWORD PTR DS:[ESI]
$+CE     004D1E7E      03D1                       ADD EDX,ECX
$+D0     004D1E80      C1FF 03                    SAR EDI,0x3
$+D3     004D1E83      33C9                       XOR ECX,ECX
$+D5     004D1E85      8BC7                       MOV EAX,EDI
$+D7     004D1E87      D1E8                       SHR EAX,1
$+D9     004D1E89      2BD8                       SUB EBX,EAX
$+DB     004D1E8B      03C7                       ADD EAX,EDI
$+DD     004D1E8D      3BDF                       CMP EBX,EDI
$+DF     004D1E8F      0F43C8                     CMOVNB ECX,EAX
$+E2     004D1E92      3BCA                       CMP ECX,EDX
$+E4     004D1E94      0F43D1                     CMOVNB EDX,ECX
$+E7     004D1E97      8BCE                       MOV ECX,ESI
$+E9     004D1E99      52                         PUSH EDX
$+EA     004D1E9A      E8 D1000000                CALL PathOfEx.004D1F70
$+EF     004D1E9F      5B                         POP EBX
$+F0     004D1EA0      5F                         POP EDI
$+F1     004D1EA1      5E                         POP ESI
$+F2     004D1EA2      C2 0400                    RETN 0x4
$+F5     004D1EA5      68 BCAAE000                PUSH PathOfEx.00E0AABC                                ; ASCII "vector<T> too long"
$+FA     004D1EAA      E8 6B857900                CALL PathOfEx.00C6A41A
$+FF     004D1EAF      CC                         INT3
$+100    004D1EB0      55                         PUSH EBP
$+101    004D1EB1      8BEC                       MOV EBP,ESP
$+103    004D1EB3      6A FF                      PUSH -0x1
$+105    004D1EB5      68 4E20D200                PUSH PathOfEx.00D2204E                                ; ASCII "冈牯"
$+10A    004D1EBA      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+110    004D1EC0      50                         PUSH EAX
$+111    004D1EC1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+118    004D1EC8      51                         PUSH ECX
$+119    004D1EC9      53                         PUSH EBX
$+11A    004D1ECA      56                         PUSH ESI
$+11B    004D1ECB      57                         PUSH EDI
$+11C    004D1ECC      8BF9                       MOV EDI,ECX
$+11E    004D1ECE      897D F0                    MOV DWORD PTR SS:[EBP-0x10],EDI
$+121    004D1ED1      6A 08                      PUSH 0x8
$+123    004D1ED3      C707 CC53DE00              MOV DWORD PTR DS:[EDI],PathOfEx.00DE53CC
$+129    004D1ED9      E8 38A87A00                CALL PathOfEx.00C7C716
$+12E    004D1EDE      8BF0                       MOV ESI,EAX
$+130    004D1EE0      6A 01                      PUSH 0x1
$+132    004D1EE2      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+139    004D1EE9      E8 6F887900                CALL PathOfEx.00C6A75D
$+13E    004D1EEE      83C4 08                    ADD ESP,0x8
$+141    004D1EF1      8946 04                    MOV DWORD PTR DS:[ESI+0x4],EAX
$+144    004D1EF4      8BCF                       MOV ECX,EDI
$+146    004D1EF6      8977 34                    MOV DWORD PTR DS:[EDI+0x34],ESI
$+149    004D1EF9      E8 226AFAFF                CALL PathOfEx.00478920
$+14E    004D1EFE      8B5D 0C                    MOV EBX,DWORD PTR SS:[EBP+0xC]
$+151    004D1F01      8BC3                       MOV EAX,EBX
$+153    004D1F03      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+15A    004D1F0A      C707 68FCE600              MOV DWORD PTR DS:[EDI],PathOfEx.00E6FC68
$+160    004D1F10      8D0C9D 00000000            LEA ECX,DWORD PTR DS:[EBX*4]
$+167    004D1F17      F7D1                       NOT ECX
$+169    004D1F19      83E1 04                    AND ECX,0x4
$+16C    004D1F1C      8BD1                       MOV EDX,ECX
$+16E    004D1F1E      83CA 02                    OR EDX,0x2
$+171    004D1F21      24 02                      AND AL,0x2
$+173    004D1F23      8BC3                       MOV EAX,EBX
$+175    004D1F25      0F45D1                     CMOVNE EDX,ECX
$+178    004D1F28      8BCA                       MOV ECX,EDX
$+17A    004D1F2A      83C9 08                    OR ECX,0x8
$+17D    004D1F2D      24 08                      AND AL,0x8
$+17F    004D1F2F      8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
$+182    004D1F32      0F44CA                     CMOVE ECX,EDX
$+185    004D1F35      8BD1                       MOV EDX,ECX
$+187    004D1F37      83CA 10                    OR EDX,0x10
$+18A    004D1F3A      80E3 04                    AND BL,0x4
$+18D    004D1F3D      0F44D1                     CMOVE EDX,ECX
$+190    004D1F40      8378 14 08                 CMP DWORD PTR DS:[EAX+0x14],0x8
$+194    004D1F44      8B48 10                    MOV ECX,DWORD PTR DS:[EAX+0x10]
$+197    004D1F47      72 02                      JB SHORT PathOfEx.004D1F4B
$+199    004D1F49      8B00                       MOV EAX,DWORD PTR DS:[EAX]
$+19B    004D1F4B      52                         PUSH EDX
$+19C    004D1F4C      51                         PUSH ECX
$+19D    004D1F4D      50                         PUSH EAX
$+19E    004D1F4E      8BCF                       MOV ECX,EDI
$+1A0    004D1F50      E8 8B6AFAFF                CALL PathOfEx.004789E0
$+1A5    004D1F55      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1A8    004D1F58      8BC7                       MOV EAX,EDI
$+1AA    004D1F5A      5F                         POP EDI
$+1AB    004D1F5B      5E                         POP ESI
$+1AC    004D1F5C      5B                         POP EBX
$+1AD    004D1F5D      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+1B4    004D1F64      8BE5                       MOV ESP,EBP
$+1B6    004D1F66      5D                         POP EBP
$+1B7    004D1F67      C2 0800                    RETN 0x8
$+1BA    004D1F6A      CC                         INT3
*/
/**/
//tag_得到手上拿着的物品，即拿起的物品
/*
$-31     0059A32F     CC                           INT3
$-30     0059A330     51                           PUSH ECX
$-2F     0059A331     83B9 34090000 00             CMP DWORD PTR DS:[ECX+0x934],0x0
$-28     0059A338     74 1C                        JE SHORT PathOfEx.0059A356
$-26     0059A33A     83B9 38090000 01             CMP DWORD PTR DS:[ECX+0x938],0x1
$-1F     0059A341     74 13                        JE SHORT PathOfEx.0059A356
$-1D     0059A343     6A FF                        PUSH -0x1
$-1B     0059A345     6A 00                        PUSH 0x0
$-19     0059A347     6A 00                        PUSH 0x0
$-17     0059A349     6A 00                        PUSH 0x0
$-15     0059A34B     6A 00                        PUSH 0x0
$-13     0059A34D     6A 00                        PUSH 0x0
$-11     0059A34F     6A 00                        PUSH 0x0
$-F      0059A351     E8 7AFAFFFF                  CALL PathOfEx.00599DD0
$-A      0059A356     59                           POP ECX
$-9      0059A357     C3                           RETN
$-8      0059A358     CC                           INT3
$-7      0059A359     CC                           INT3
$-6      0059A35A     CC                           INT3
$-5      0059A35B     CC                           INT3
$-4      0059A35C     CC                           INT3
$-3      0059A35D     CC                           INT3
$-2      0059A35E     CC                           INT3
$-1      0059A35F     CC                           INT3                                 ; g_c_ui_mouse_attach_item_get_item_
$ ==>    0059A360     83EC 08                      SUB ESP,0x8                          ; tag_得到手上拿着的物品，即拿起的物品
$+3      0059A363     83B9 38090000 00             CMP DWORD PTR DS:[ECX+0x938],0x0     ; 返回stCD_EntityItemBased*
$+A      0059A36A     56                           PUSH ESI                             ; ecx=stCD_UiMouseAttachItem*
$+B      0059A36B     74 3D                        JE SHORT PathOfEx.0059A3AA
$+D      0059A36D     8BB1 44090000                MOV ESI,DWORD PTR DS:[ECX+0x944]
$+13     0059A373     85F6                         TEST ESI,ESI
$+15     0059A375     74 33                        JE SHORT PathOfEx.0059A3AA
$+17     0059A377     8B81 4C090000                MOV EAX,DWORD PTR DS:[ECX+0x94C]
$+1D     0059A37D     85C0                         TEST EAX,EAX
$+1F     0059A37F     74 29                        JE SHORT PathOfEx.0059A3AA
$+21     0059A381     894424 04                    MOV DWORD PTR SS:[ESP+0x4],EAX
$+25     0059A385     8D4E 2C                      LEA ECX,DWORD PTR DS:[ESI+0x2C]
$+28     0059A388     8D4424 04                    LEA EAX,DWORD PTR SS:[ESP+0x4]
$+2C     0059A38C     50                           PUSH EAX
$+2D     0059A38D     8D4424 0C                    LEA EAX,DWORD PTR SS:[ESP+0xC]
$+31     0059A391     50                           PUSH EAX
$+32     0059A392     E8 393C0100                  CALL PathOfEx.005ADFD0
$+37     0059A397     8B4424 08                    MOV EAX,DWORD PTR SS:[ESP+0x8]
$+3B     0059A39B     3B46 2C                      CMP EAX,DWORD PTR DS:[ESI+0x2C]
$+3E     0059A39E     74 0A                        JE SHORT PathOfEx.0059A3AA
$+40     0059A3A0     8B40 14                      MOV EAX,DWORD PTR DS:[EAX+0x14]
$+43     0059A3A3     5E                           POP ESI
$+44     0059A3A4     8B00                         MOV EAX,DWORD PTR DS:[EAX]
$+46     0059A3A6     83C4 08                      ADD ESP,0x8
$+49     0059A3A9     C3                           RETN
$+4A     0059A3AA     33C0                         XOR EAX,EAX
$+4C     0059A3AC     5E                           POP ESI
$+4D     0059A3AD     83C4 08                      ADD ESP,0x8
$+50     0059A3B0     C3                           RETN
$+51     0059A3B1     CC                           INT3
$+52     0059A3B2     CC                           INT3
$+53     0059A3B3     CC                           INT3
$+54     0059A3B4     CC                           INT3
$+55     0059A3B5     CC                           INT3
$+56     0059A3B6     CC                           INT3
$+57     0059A3B7     CC                           INT3
$+58     0059A3B8     CC                           INT3
$+59     0059A3B9     CC                           INT3
$+5A     0059A3BA     CC                           INT3
$+5B     0059A3BB     CC                           INT3
$+5C     0059A3BC     CC                           INT3
$+5D     0059A3BD     CC                           INT3
$+5E     0059A3BE     CC                           INT3
$+5F     0059A3BF     CC                           INT3
$+60     0059A3C0     8B4424 04                    MOV EAX,DWORD PTR SS:[ESP+0x4]
$+64     0059A3C4     56                           PUSH ESI
$+65     0059A3C5     8BB1 54090000                MOV ESI,DWORD PTR DS:[ECX+0x954]
$+6B     0059A3CB     8930                         MOV DWORD PTR DS:[EAX],ESI
$+6D     0059A3CD     85F6                         TEST ESI,ESI
$+6F     0059A3CF     74 15                        JE SHORT PathOfEx.0059A3E6
$+71     0059A3D1     8D56 38                      LEA EDX,DWORD PTR DS:[ESI+0x38]
$+74     0059A3D4     B9 01000000                  MOV ECX,0x1
$+79     0059A3D9     F0:0FC10A                    LOCK XADD DWORD PTR DS:[EDX],ECX
$+7D     0059A3DD     8B0D 0C67FD00                MOV ECX,DWORD PTR DS:[0xFD670C]
$+83     0059A3E3     894E 34                      MOV DWORD PTR DS:[ESI+0x34],ECX
$+86     0059A3E6     5E                           POP ESI
$+87     0059A3E7     C2 0400                      RETN 0x4
$+8A     0059A3EA     CC                           INT3
*/
/**/
//tag_判断物品是否可以放到inventory
/*
$-32     007F8A5E     C1F8 04                      SAR EAX,0x4
$-2F     007F8A61     50                           PUSH EAX
$-2E     007F8A62     51                           PUSH ECX
$-2D     007F8A63     E8 C83DC7FF                  CALL PathOfEx.0046C830
$-28     007F8A68     C786 80000000 00000000       MOV DWORD PTR DS:[ESI+0x80],0x0
$-1E     007F8A72     83C4 14                      ADD ESP,0x14
$-1B     007F8A75     C786 84000000 00000000       MOV DWORD PTR DS:[ESI+0x84],0x0
$-11     007F8A7F     C786 88000000 00000000       MOV DWORD PTR DS:[ESI+0x88],0x0
$-7      007F8A89     5E                           POP ESI
$-6      007F8A8A     59                           POP ECX
$-5      007F8A8B     C3                           RETN
$-4      007F8A8C     CC                           INT3
$-3      007F8A8D     CC                           INT3
$-2      007F8A8E     CC                           INT3
$-1      007F8A8F     CC                           INT3                                 ; 7个参数,g_c_can_put_it_to_the_inventory_
$ ==>    007F8A90     55                           PUSH EBP                             ; tag_判断物品是否可以放到inventory，返回bool
$+1      007F8A91     8BEC                         MOV EBP,ESP                          ; p1=stCD_EntityItemBased*
$+3      007F8A93     6A FF                        PUSH -0x1                            ; p2=dst_inventory_type
$+5      007F8A95     68 D892D500                  PUSH PathOfEx.00D592D8               ; p3=dst_pos_x
$+A      007F8A9A     64:A1 00000000               MOV EAX,DWORD PTR FS:[0]             ; p4=dst_pos_y
$+10     007F8AA0     50                           PUSH EAX                             ; p5=int*，只是初始化为-1，一直没用到
$+11     007F8AA1     64:8925 00000000             MOV DWORD PTR FS:[0],ESP             ; p6=int*,一直初始化为0，没有用到
$+18     007F8AA8     83EC 2C                      SUB ESP,0x2C                         ; p7=dword，也一直没有用到
$+1B     007F8AAB     8B45 18                      MOV EAX,DWORD PTR SS:[EBP+0x18]      ; ecx=stCD_GridPanelInventoryInfo*
$+1E     007F8AAE     53                           PUSH EBX
$+1F     007F8AAF     8BD9                         MOV EBX,ECX
$+21     007F8AB1     8B4D 1C                      MOV ECX,DWORD PTR SS:[EBP+0x1C]
$+24     007F8AB4     56                           PUSH ESI
$+25     007F8AB5     C700 FFFFFFFF                MOV DWORD PTR DS:[EAX],-0x1
$+2B     007F8ABB     8B45 0C                      MOV EAX,DWORD PTR SS:[EBP+0xC]
$+2E     007F8ABE     57                           PUSH EDI
$+2F     007F8ABF     C701 00000000                MOV DWORD PTR DS:[ECX],0x0
$+35     007F8AC5     8BBB 84000000                MOV EDI,DWORD PTR DS:[EBX+0x84]
$+3B     007F8ACB     8BD7                         MOV EDX,EDI
$+3D     007F8ACD     8945 E0                      MOV DWORD PTR SS:[EBP-0x20],EAX
$+40     007F8AD0     8D45 E0                      LEA EAX,DWORD PTR SS:[EBP-0x20]
$+43     007F8AD3     51                           PUSH ECX
$+44     007F8AD4     8B8B 80000000                MOV ECX,DWORD PTR DS:[EBX+0x80]
$+4A     007F8ADA     50                           PUSH EAX
$+4B     007F8ADB     895D E8                      MOV DWORD PTR SS:[EBP-0x18],EBX
$+4E     007F8ADE     E8 DD150000                  CALL PathOfEx.007FA0C0
$+53     007F8AE3     8BF0                         MOV ESI,EAX
$+55     007F8AE5     83C4 08                      ADD ESP,0x8
$+58     007F8AE8     3BF7                         CMP ESI,EDI
$+5A     007F8AEA     74 07                        JE SHORT PathOfEx.007F8AF3
$+5C     007F8AEC     8B45 0C                      MOV EAX,DWORD PTR SS:[EBP+0xC]
$+5F     007F8AEF     3B06                         CMP EAX,DWORD PTR DS:[ESI]
$+61     007F8AF1     73 02                        JNB SHORT PathOfEx.007F8AF5
$+63     007F8AF3     8BF7                         MOV ESI,EDI
$+65     007F8AF5     3BF7                         CMP ESI,EDI
$+67     007F8AF7     0F84 4D070000                JE PathOfEx.007F924A
$+6D     007F8AFD     807E 0C 00                   CMP BYTE PTR DS:[ESI+0xC],0x0
$+71     007F8B01     0F85 43070000                JNZ PathOfEx.007F924A
$+77     007F8B07     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+7A     007F8B0A     8338 0B                      CMP DWORD PTR DS:[EAX],0xB
$+7D     007F8B0D     0F84 37070000                JE PathOfEx.007F924A
$+83     007F8B13     8B7D 08                      MOV EDI,DWORD PTR SS:[EBP+0x8]       ; 执行后edi=stCD_EntityItemBased*
$+86     007F8B16     8BCF                         MOV ECX,EDI
$+88     007F8B18     E8 1317CCFF                  CALL PathOfEx.004BA230               ; 返回stCD_EntityAttrItemBase*
$+8D     007F8B1D     8BC8                         MOV ECX,EAX
$+8F     007F8B1F     8945 EC                      MOV DWORD PTR SS:[EBP-0x14],EAX
$+92     007F8B22     E8 592EF5FF                  CALL PathOfEx.0074B980
$+97     007F8B27     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+9A     007F8B2A     8B5D 10                      MOV EBX,DWORD PTR SS:[EBP+0x10]
$+9D     007F8B2D     FF75 1C                      PUSH DWORD PTR SS:[EBP+0x1C]
$+A0     007F8B30     8945 E4                      MOV DWORD PTR SS:[EBP-0x1C],EAX
$+A3     007F8B33     8B49 08                      MOV ECX,DWORD PTR DS:[ECX+0x8]
$+A6     007F8B36     8B45 14                      MOV EAX,DWORD PTR SS:[EBP+0x14]
$+A9     007F8B39     8945 CC                      MOV DWORD PTR SS:[EBP-0x34],EAX
$+AC     007F8B3C     895D C8                      MOV DWORD PTR SS:[EBP-0x38],EBX
$+AF     007F8B3F     8B51 0C                      MOV EDX,DWORD PTR DS:[ECX+0xC]
$+B2     007F8B42     8B49 08                      MOV ECX,DWORD PTR DS:[ECX+0x8]
$+B5     007F8B45     03D0                         ADD EDX,EAX
$+B7     007F8B47     03CB                         ADD ECX,EBX
$+B9     007F8B49     8955 D4                      MOV DWORD PTR SS:[EBP-0x2C],EDX
$+BC     007F8B4C     8D45 C8                      LEA EAX,DWORD PTR SS:[EBP-0x38]
$+BF     007F8B4F     894D D0                      MOV DWORD PTR SS:[EBP-0x30],ECX
$+C2     007F8B52     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+C5     007F8B55     50                           PUSH EAX
$+C6     007F8B56     E8 B57CF5FF                  CALL PathOfEx.00750810
$+CB     007F8B5B     8B5D E8                      MOV EBX,DWORD PTR SS:[EBP-0x18]
$+CE     007F8B5E     8845 F3                      MOV BYTE PTR SS:[EBP-0xD],AL
$+D1     007F8B61     84C0                         TEST AL,AL
$+D3     007F8B63     74 14                        JE SHORT PathOfEx.007F8B79
$+D5     007F8B65     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+D8     007F8B68     8338 00                      CMP DWORD PTR DS:[EAX],0x0
$+DB     007F8B6B     0F84 D9060000                JE PathOfEx.007F924A
$+E1     007F8B71     8B45 18                      MOV EAX,DWORD PTR SS:[EBP+0x18]
$+E4     007F8B74     8B4D 0C                      MOV ECX,DWORD PTR SS:[EBP+0xC]
$+E7     007F8B77     8908                         MOV DWORD PTR DS:[EAX],ECX
$+E9     007F8B79     8BCF                         MOV ECX,EDI
$+EB     007F8B7B     E8 00CDEDFF                  CALL PathOfEx.006D5880
$+F0     007F8B80     85C0                         TEST EAX,EAX
$+F2     007F8B82     74 2C                        JE SHORT PathOfEx.007F8BB0
$+F4     007F8B84     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+F7     007F8B87     8079 0A 00                   CMP BYTE PTR DS:[ECX+0xA],0x0
$+FB     007F8B8B     75 0C                        JNZ SHORT PathOfEx.007F8B99
$+FD     007F8B8D     837B 04 00                   CMP DWORD PTR DS:[EBX+0x4],0x0
$+101    007F8B91     74 06                        JE SHORT PathOfEx.007F8B99
$+103    007F8B93     8379 04 1B                   CMP DWORD PTR DS:[ECX+0x4],0x1B
$+107    007F8B97     75 17                        JNZ SHORT PathOfEx.007F8BB0
$+109    007F8B99     807D 20 00                   CMP BYTE PTR SS:[EBP+0x20],0x0
$+10D    007F8B9D     0F84 A7060000                JE PathOfEx.007F924A
$+113    007F8BA3     8B40 08                      MOV EAX,DWORD PTR DS:[EAX+0x8]
$+116    007F8BA6     8078 24 00                   CMP BYTE PTR DS:[EAX+0x24],0x0
$+11A    007F8BAA     0F84 9A060000                JE PathOfEx.007F924A
$+120    007F8BB0     8B7D EC                      MOV EDI,DWORD PTR SS:[EBP-0x14]
$+123    007F8BB3     8BCF                         MOV ECX,EDI
$+125    007F8BB5     E8 C62DF5FF                  CALL PathOfEx.0074B980
$+12A    007F8BBA     83F8 2C                      CMP EAX,0x2C
$+12D    007F8BBD     74 0C                        JE SHORT PathOfEx.007F8BCB
$+12F    007F8BBF     8BCF                         MOV ECX,EDI
$+131    007F8BC1     E8 BA2DF5FF                  CALL PathOfEx.0074B980
$+136    007F8BC6     83F8 2D                      CMP EAX,0x2D
$+139    007F8BC9     75 17                        JNZ SHORT PathOfEx.007F8BE2
$+13B    007F8BCB     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+13E    007F8BCE     8078 0A 00                   CMP BYTE PTR DS:[EAX+0xA],0x0
$+142    007F8BD2     0F85 72060000                JNZ PathOfEx.007F924A
$+148    007F8BD8     837B 04 00                   CMP DWORD PTR DS:[EBX+0x4],0x0
$+14C    007F8BDC     0F84 68060000                JE PathOfEx.007F924A
$+152    007F8BE2     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+155    007F8BE5     8338 11                      CMP DWORD PTR DS:[EAX],0x11
$+158    007F8BE8     0F85 58010000                JNZ PathOfEx.007F8D46
$+15E    007F8BEE     837D 14 00                   CMP DWORD PTR SS:[EBP+0x14],0x0
$+162    007F8BF2     0F85 52060000                JNZ PathOfEx.007F924A
$+168    007F8BF8     8B45 10                      MOV EAX,DWORD PTR SS:[EBP+0x10]
$+16B    007F8BFB     83EC 10                      SUB ESP,0x10
$+16E    007F8BFE     8945 DC                      MOV DWORD PTR SS:[EBP-0x24],EAX
$+171    007F8C01     E8 FAF3F1FF                  CALL PathOfEx.00718000
$+176    007F8C06     8BC8                         MOV ECX,EAX
$+178    007F8C08     E8 E372D5FF                  CALL PathOfEx.0054FEF0
$+17D    007F8C0D     8945 E0                      MOV DWORD PTR SS:[EBP-0x20],EAX
$+180    007F8C10     8D45 DC                      LEA EAX,DWORD PTR SS:[EBP-0x24]
$+183    007F8C13     C745 FC 00000000             MOV DWORD PTR SS:[EBP-0x4],0x0
$+18A    007F8C1A     50                           PUSH EAX
$+18B    007F8C1B     8D55 E0                      LEA EDX,DWORD PTR SS:[EBP-0x20]
$+18E    007F8C1E     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+191    007F8C21     E8 6ADDF5FF                  CALL PathOfEx.00756990
$+196    007F8C26     83C4 04                      ADD ESP,0x4
$+199    007F8C29     8D4D E0                      LEA ECX,DWORD PTR SS:[EBP-0x20]
$+19C    007F8C2C     C645 FC 02                   MOV BYTE PTR SS:[EBP-0x4],0x2
$+1A0    007F8C30     E8 DB3EC7FF                  CALL PathOfEx.0046CB10
$+1A5    007F8C35     8B5D D4                      MOV EBX,DWORD PTR SS:[EBP-0x2C]
$+1A8    007F8C38     85DB                         TEST EBX,EBX
$+1AA    007F8C3A     74 4D                        JE SHORT PathOfEx.007F8C89
$+1AC    007F8C3C     8BCF                         MOV ECX,EDI
$+1AE    007F8C3E     E8 3D2DF5FF                  CALL PathOfEx.0074B980
$+1B3    007F8C43     83F8 2B                      CMP EAX,0x2B
$+1B6    007F8C46     74 41                        JE SHORT PathOfEx.007F8C89
$+1B8    007F8C48     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+1BB    007F8C4B     8BCB                         MOV ECX,EBX
$+1BD    007F8C4D     50                           PUSH EAX
$+1BE    007F8C4E     E8 EDF3D6FF                  CALL PathOfEx.00568040
$+1C3    007F8C53     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+1C6    007F8C56     8B78 04                      MOV EDI,DWORD PTR DS:[EAX+0x4]
$+1C9    007F8C59     E8 4270C7FF                  CALL PathOfEx.0046FCA0
$+1CE    007F8C5E     85FF                         TEST EDI,EDI
$+1D0    007F8C60     74 34                        JE SHORT PathOfEx.007F8C96
$+1D2    007F8C62     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+1D5    007F8C65     8BCB                         MOV ECX,EBX
$+1D7    007F8C67     50                           PUSH EAX
$+1D8    007F8C68     E8 D3F3D6FF                  CALL PathOfEx.00568040
$+1DD    007F8C6D     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+1E0    007F8C70     8B78 04                      MOV EDI,DWORD PTR DS:[EAX+0x4]
$+1E3    007F8C73     8B49 08                      MOV ECX,DWORD PTR DS:[ECX+0x8]
$+1E6    007F8C76     8B59 4C                      MOV EBX,DWORD PTR DS:[ECX+0x4C]
$+1E9    007F8C79     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+1EC    007F8C7C     E8 1F70C7FF                  CALL PathOfEx.0046FCA0
$+1F1    007F8C81     3BDF                         CMP EBX,EDI
$+1F3    007F8C83     0F84 AB000000                JE PathOfEx.007F8D34
$+1F9    007F8C89     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+1FC    007F8C8C     E8 0F70C7FF                  CALL PathOfEx.0046FCA0
$+201    007F8C91     E9 B4050000                  JMP PathOfEx.007F924A
$+206    007F8C96     8B03                         MOV EAX,DWORD PTR DS:[EBX]
$+208    007F8C98     B9 807DED00                  MOV ECX,PathOfEx.00ED7D80            ; UNICODE "CraftingSlot"
$+20D    007F8C9D     0F1F00                       NOP DWORD PTR DS:[EAX]
$+210    007F8CA0     66:8B10                      MOV DX,WORD PTR DS:[EAX]
$+213    007F8CA3     66:3B11                      CMP DX,WORD PTR DS:[ECX]
$+216    007F8CA6     75 1E                        JNZ SHORT PathOfEx.007F8CC6
$+218    007F8CA8     66:85D2                      TEST DX,DX
$+21B    007F8CAB     74 15                        JE SHORT PathOfEx.007F8CC2
$+21D    007F8CAD     66:8B50 02                   MOV DX,WORD PTR DS:[EAX+0x2]
$+221    007F8CB1     66:3B51 02                   CMP DX,WORD PTR DS:[ECX+0x2]
$+225    007F8CB5     75 0F                        JNZ SHORT PathOfEx.007F8CC6
$+227    007F8CB7     83C0 04                      ADD EAX,0x4
$+22A    007F8CBA     83C1 04                      ADD ECX,0x4
$+22D    007F8CBD     66:85D2                      TEST DX,DX
$+230    007F8CC0   ^ 75 DE                        JNZ SHORT PathOfEx.007F8CA0
$+232    007F8CC2     33C0                         XOR EAX,EAX
$+234    007F8CC4     EB 05                        JMP SHORT PathOfEx.007F8CCB
$+236    007F8CC6     1BC0                         SBB EAX,EAX
$+238    007F8CC8     83C8 01                      OR EAX,0x1
$+23B    007F8CCB     85C0                         TEST EAX,EAX
$+23D    007F8CCD     75 0C                        JNZ SHORT PathOfEx.007F8CDB
$+23F    007F8CCF     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+242    007F8CD2     E8 B971CDFF                  CALL PathOfEx.004CFE90
$+247    007F8CD7     84C0                         TEST AL,AL
$+249    007F8CD9   ^ EB A8                        JMP SHORT PathOfEx.007F8C83
$+24B    007F8CDB     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+24E    007F8CDE     8B00                         MOV EAX,DWORD PTR DS:[EAX]
$+250    007F8CE0     85C0                         TEST EAX,EAX
$+252    007F8CE2     74 40                        JE SHORT PathOfEx.007F8D24
$+254    007F8CE4     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+257    007F8CE7     50                           PUSH EAX
$+258    007F8CE8     E8 F37AF5FF                  CALL PathOfEx.007507E0
$+25D    007F8CED     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+260    007F8CF0     8BD8                         MOV EBX,EAX
$+262    007F8CF2     8D45 E0                      LEA EAX,DWORD PTR SS:[EBP-0x20]
$+265    007F8CF5     50                           PUSH EAX
$+266    007F8CF6     E8 C526CAFF                  CALL PathOfEx.0049B3C0
$+26B    007F8CFB     8BF8                         MOV EDI,EAX
$+26D    007F8CFD     8BCB                         MOV ECX,EBX
$+26F    007F8CFF     8D45 DC                      LEA EAX,DWORD PTR SS:[EBP-0x24]
$+272    007F8D02     50                           PUSH EAX
$+273    007F8D03     E8 B826CAFF                  CALL PathOfEx.0049B3C0
$+278    007F8D08     8B3F                         MOV EDI,DWORD PTR DS:[EDI]
$+27A    007F8D0A     8D4D DC                      LEA ECX,DWORD PTR SS:[EBP-0x24]
$+27D    007F8D0D     8B18                         MOV EBX,DWORD PTR DS:[EAX]
$+27F    007F8D0F     E8 FC3DC7FF                  CALL PathOfEx.0046CB10
$+284    007F8D14     8D4D E0                      LEA ECX,DWORD PTR SS:[EBP-0x20]
$+287    007F8D17     E8 F43DC7FF                  CALL PathOfEx.0046CB10
$+28C    007F8D1C     3BDF                         CMP EBX,EDI
$+28E    007F8D1E   ^ 0F85 65FFFFFF                JNZ PathOfEx.007F8C89
$+294    007F8D24     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+297    007F8D27     E8 6471CDFF                  CALL PathOfEx.004CFE90
$+29C    007F8D2C     84C0                         TEST AL,AL
$+29E    007F8D2E   ^ 0F84 55FFFFFF                JE PathOfEx.007F8C89
$+2A4    007F8D34     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+2A7    007F8D37     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+2AE    007F8D3E     E8 5D6FC7FF                  CALL PathOfEx.0046FCA0
$+2B3    007F8D43     8B7D EC                      MOV EDI,DWORD PTR SS:[EBP-0x14]
$+2B6    007F8D46     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+2B9    007F8D49     8338 13                      CMP DWORD PTR DS:[EAX],0x13
$+2BC    007F8D4C     0F85 AF000000                JNZ PathOfEx.007F8E01
$+2C2    007F8D52     837D 14 00                   CMP DWORD PTR SS:[EBP+0x14],0x0
$+2C6    007F8D56     0F85 EE040000                JNZ PathOfEx.007F924A
$+2CC    007F8D5C     83EC 10                      SUB ESP,0x10
$+2CF    007F8D5F     E8 9CF2F1FF                  CALL PathOfEx.00718000
$+2D4    007F8D64     8BC8                         MOV ECX,EAX
$+2D6    007F8D66     E8 35A7D5FF                  CALL PathOfEx.005534A0
$+2DB    007F8D6B     8945 E0                      MOV DWORD PTR SS:[EBP-0x20],EAX
$+2DE    007F8D6E     C745 FC 03000000             MOV DWORD PTR SS:[EBP-0x4],0x3
$+2E5    007F8D75     8D55 E0                      LEA EDX,DWORD PTR SS:[EBP-0x20]
$+2E8    007F8D78     FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
$+2EB    007F8D7B     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+2EE    007F8D7E     E8 8DFFF6FF                  CALL PathOfEx.00768D10
$+2F3    007F8D83     83C4 04                      ADD ESP,0x4
$+2F6    007F8D86     8D4D E0                      LEA ECX,DWORD PTR SS:[EBP-0x20]
$+2F9    007F8D89     C645 FC 05                   MOV BYTE PTR SS:[EBP-0x4],0x5
$+2FD    007F8D8D     E8 7E3DC7FF                  CALL PathOfEx.0046CB10
$+302    007F8D92     8B5D D4                      MOV EBX,DWORD PTR SS:[EBP-0x2C]
$+305    007F8D95     85DB                         TEST EBX,EBX
$+307    007F8D97     0F84 16010000                JE PathOfEx.007F8EB3
$+30D    007F8D9D     8BCF                         MOV ECX,EDI
$+30F    007F8D9F     E8 DC2BF5FF                  CALL PathOfEx.0074B980
$+314    007F8DA4     83F8 2B                      CMP EAX,0x2B
$+317    007F8DA7     0F85 06010000                JNZ PathOfEx.007F8EB3
$+31D    007F8DAD     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+320    007F8DB0     8BCB                         MOV ECX,EBX
$+322    007F8DB2     50                           PUSH EAX
$+323    007F8DB3     E8 688FCAFF                  CALL PathOfEx.004A1D20
$+328    007F8DB8     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+32B    007F8DBB     8B78 04                      MOV EDI,DWORD PTR DS:[EAX+0x4]
$+32E    007F8DBE     E8 DD6EC7FF                  CALL PathOfEx.0046FCA0
$+333    007F8DC3     85FF                         TEST EDI,EDI
$+335    007F8DC5     0F84 E8000000                JE PathOfEx.007F8EB3
$+33B    007F8DCB     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+33E    007F8DCE     8BCB                         MOV ECX,EBX
$+340    007F8DD0     50                           PUSH EAX
$+341    007F8DD1     E8 4A8FCAFF                  CALL PathOfEx.004A1D20
$+346    007F8DD6     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+349    007F8DD9     8B58 04                      MOV EBX,DWORD PTR DS:[EAX+0x4]
$+34C    007F8DDC     8B41 08                      MOV EAX,DWORD PTR DS:[ECX+0x8]
$+34F    007F8DDF     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+352    007F8DE2     8B78 4C                      MOV EDI,DWORD PTR DS:[EAX+0x4C]
$+355    007F8DE5     E8 B66EC7FF                  CALL PathOfEx.0046FCA0
$+35A    007F8DEA     3BDF                         CMP EBX,EDI
$+35C    007F8DEC     0F85 C1000000                JNZ PathOfEx.007F8EB3
$+362    007F8DF2     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+365    007F8DF5     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+36C    007F8DFC     E8 9F6EC7FF                  CALL PathOfEx.0046FCA0
$+371    007F8E01     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+374    007F8E04     8338 14                      CMP DWORD PTR DS:[EAX],0x14
$+377    007F8E07     0F85 66010000                JNZ PathOfEx.007F8F73
$+37D    007F8E0D     837D 14 00                   CMP DWORD PTR SS:[EBP+0x14],0x0
$+381    007F8E11     0F85 33040000                JNZ PathOfEx.007F924A
$+387    007F8E17     83EC 10                      SUB ESP,0x10
$+38A    007F8E1A     E8 E1F1F1FF                  CALL PathOfEx.00718000
$+38F    007F8E1F     8BC8                         MOV ECX,EAX
$+391    007F8E21     E8 4ABCD5FF                  CALL PathOfEx.00554A70
$+396    007F8E26     8945 14                      MOV DWORD PTR SS:[EBP+0x14],EAX
$+399    007F8E29     C745 FC 06000000             MOV DWORD PTR SS:[EBP-0x4],0x6
$+3A0    007F8E30     8D55 14                      LEA EDX,DWORD PTR SS:[EBP+0x14]
$+3A3    007F8E33     FF75 10                      PUSH DWORD PTR SS:[EBP+0x10]
$+3A6    007F8E36     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+3A9    007F8E39     E8 C206F7FF                  CALL PathOfEx.00769500
$+3AE    007F8E3E     83C4 04                      ADD ESP,0x4
$+3B1    007F8E41     8D4D 14                      LEA ECX,DWORD PTR SS:[EBP+0x14]
$+3B4    007F8E44     C645 FC 08                   MOV BYTE PTR SS:[EBP-0x4],0x8
$+3B8    007F8E48     E8 C33CC7FF                  CALL PathOfEx.0046CB10
$+3BD    007F8E4D     8B7D DC                      MOV EDI,DWORD PTR SS:[EBP-0x24]
$+3C0    007F8E50     85FF                         TEST EDI,EDI
$+3C2    007F8E52     74 52                        JE SHORT PathOfEx.007F8EA6
$+3C4    007F8E54     8B5D EC                      MOV EBX,DWORD PTR SS:[EBP-0x14]
$+3C7    007F8E57     8BCB                         MOV ECX,EBX
$+3C9    007F8E59     E8 222BF5FF                  CALL PathOfEx.0074B980
$+3CE    007F8E5E     83F8 2B                      CMP EAX,0x2B
$+3D1    007F8E61     74 43                        JE SHORT PathOfEx.007F8EA6
$+3D3    007F8E63     8D45 D0                      LEA EAX,DWORD PTR SS:[EBP-0x30]
$+3D6    007F8E66     8BCF                         MOV ECX,EDI
$+3D8    007F8E68     50                           PUSH EAX
$+3D9    007F8E69     E8 D2F1D6FF                  CALL PathOfEx.00568040
$+3DE    007F8E6E     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+3E1    007F8E71     8B78 04                      MOV EDI,DWORD PTR DS:[EAX+0x4]
$+3E4    007F8E74     E8 276EC7FF                  CALL PathOfEx.0046FCA0
$+3E9    007F8E79     85FF                         TEST EDI,EDI
$+3EB    007F8E7B     74 43                        JE SHORT PathOfEx.007F8EC0
$+3ED    007F8E7D     8B4D DC                      MOV ECX,DWORD PTR SS:[EBP-0x24]
$+3F0    007F8E80     8D45 D0                      LEA EAX,DWORD PTR SS:[EBP-0x30]
$+3F3    007F8E83     50                           PUSH EAX
$+3F4    007F8E84     E8 B7F1D6FF                  CALL PathOfEx.00568040
$+3F9    007F8E89     8B4B 08                      MOV ECX,DWORD PTR DS:[EBX+0x8]
$+3FC    007F8E8C     8B40 04                      MOV EAX,DWORD PTR DS:[EAX+0x4]
$+3FF    007F8E8F     8B49 4C                      MOV ECX,DWORD PTR DS:[ECX+0x4C]
$+402    007F8E92     8B38                         MOV EDI,DWORD PTR DS:[EAX]
$+404    007F8E94     8B19                         MOV EBX,DWORD PTR DS:[ECX]
$+406    007F8E96     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+409    007F8E99     E8 026EC7FF                  CALL PathOfEx.0046FCA0
$+40E    007F8E9E     3BDF                         CMP EBX,EDI
$+410    007F8EA0     0F84 BE000000                JE PathOfEx.007F8F64
$+416    007F8EA6     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+419    007F8EA9     E8 F26DC7FF                  CALL PathOfEx.0046FCA0
$+41E    007F8EAE     E9 97030000                  JMP PathOfEx.007F924A
$+423    007F8EB3     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+426    007F8EB6     E8 E56DC7FF                  CALL PathOfEx.0046FCA0
$+42B    007F8EBB     E9 8A030000                  JMP PathOfEx.007F924A
$+430    007F8EC0     8B45 DC                      MOV EAX,DWORD PTR SS:[EBP-0x24]
$+433    007F8EC3     B9 807DED00                  MOV ECX,PathOfEx.00ED7D80            ; UNICODE "CraftingSlot"
$+438    007F8EC8     8B00                         MOV EAX,DWORD PTR DS:[EAX]
$+43A    007F8ECA     66:0F1F4400 00               NOP WORD PTR DS:[EAX+EAX]
$+440    007F8ED0     66:8B10                      MOV DX,WORD PTR DS:[EAX]
$+443    007F8ED3     66:3B11                      CMP DX,WORD PTR DS:[ECX]
$+446    007F8ED6     75 1E                        JNZ SHORT PathOfEx.007F8EF6
$+448    007F8ED8     66:85D2                      TEST DX,DX
$+44B    007F8EDB     74 15                        JE SHORT PathOfEx.007F8EF2
$+44D    007F8EDD     66:8B50 02                   MOV DX,WORD PTR DS:[EAX+0x2]
$+451    007F8EE1     66:3B51 02                   CMP DX,WORD PTR DS:[ECX+0x2]
$+455    007F8EE5     75 0F                        JNZ SHORT PathOfEx.007F8EF6
$+457    007F8EE7     83C0 04                      ADD EAX,0x4
$+45A    007F8EEA     83C1 04                      ADD ECX,0x4
$+45D    007F8EED     66:85D2                      TEST DX,DX
$+460    007F8EF0   ^ 75 DE                        JNZ SHORT PathOfEx.007F8ED0
$+462    007F8EF2     33C0                         XOR EAX,EAX
$+464    007F8EF4     EB 05                        JMP SHORT PathOfEx.007F8EFB
$+466    007F8EF6     1BC0                         SBB EAX,EAX
$+468    007F8EF8     83C8 01                      OR EAX,0x1
$+46B    007F8EFB     85C0                         TEST EAX,EAX
$+46D    007F8EFD     75 0C                        JNZ SHORT PathOfEx.007F8F0B
$+46F    007F8EFF     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+472    007F8F02     E8 896FCDFF                  CALL PathOfEx.004CFE90
$+477    007F8F07     84C0                         TEST AL,AL
$+479    007F8F09   ^ EB 95                        JMP SHORT PathOfEx.007F8EA0
$+47B    007F8F0B     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+47E    007F8F0E     8B00                         MOV EAX,DWORD PTR DS:[EAX]
$+480    007F8F10     85C0                         TEST EAX,EAX
$+482    007F8F12     74 40                        JE SHORT PathOfEx.007F8F54
$+484    007F8F14     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+487    007F8F17     50                           PUSH EAX
$+488    007F8F18     E8 C378F5FF                  CALL PathOfEx.007507E0
$+48D    007F8F1D     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+490    007F8F20     8BD8                         MOV EBX,EAX
$+492    007F8F22     8D45 10                      LEA EAX,DWORD PTR SS:[EBP+0x10]
$+495    007F8F25     50                           PUSH EAX
$+496    007F8F26     E8 9524CAFF                  CALL PathOfEx.0049B3C0
$+49B    007F8F2B     8BF8                         MOV EDI,EAX
$+49D    007F8F2D     8BCB                         MOV ECX,EBX
$+49F    007F8F2F     8D45 14                      LEA EAX,DWORD PTR SS:[EBP+0x14]
$+4A2    007F8F32     50                           PUSH EAX
$+4A3    007F8F33     E8 8824CAFF                  CALL PathOfEx.0049B3C0
$+4A8    007F8F38     8B3F                         MOV EDI,DWORD PTR DS:[EDI]
$+4AA    007F8F3A     8D4D 14                      LEA ECX,DWORD PTR SS:[EBP+0x14]
$+4AD    007F8F3D     8B18                         MOV EBX,DWORD PTR DS:[EAX]
$+4AF    007F8F3F     E8 CC3BC7FF                  CALL PathOfEx.0046CB10
$+4B4    007F8F44     8D4D 10                      LEA ECX,DWORD PTR SS:[EBP+0x10]
$+4B7    007F8F47     E8 C43BC7FF                  CALL PathOfEx.0046CB10
$+4BC    007F8F4C     3BDF                         CMP EBX,EDI
$+4BE    007F8F4E   ^ 0F85 52FFFFFF                JNZ PathOfEx.007F8EA6
$+4C4    007F8F54     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+4C7    007F8F57     E8 346FCDFF                  CALL PathOfEx.004CFE90
$+4CC    007F8F5C     84C0                         TEST AL,AL
$+4CE    007F8F5E   ^ 0F84 42FFFFFF                JE PathOfEx.007F8EA6
$+4D4    007F8F64     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+4D7    007F8F67     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+4DE    007F8F6E     E8 2D6DC7FF                  CALL PathOfEx.0046FCA0
$+4E3    007F8F73     8B5D EC                      MOV EBX,DWORD PTR SS:[EBP-0x14]
$+4E6    007F8F76     8BCB                         MOV ECX,EBX
$+4E8    007F8F78     E8 C328F5FF                  CALL PathOfEx.0074B840
$+4ED    007F8F7D     8B7D E8                      MOV EDI,DWORD PTR SS:[EBP-0x18]
$+4F0    007F8F80     84C0                         TEST AL,AL
$+4F2    007F8F82     74 19                        JE SHORT PathOfEx.007F8F9D
$+4F4    007F8F84     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+4F7    007F8F87     8078 0A 00                   CMP BYTE PTR DS:[EAX+0xA],0x0
$+4FB    007F8F8B     75 06                        JNZ SHORT PathOfEx.007F8F93
$+4FD    007F8F8D     837F 04 00                   CMP DWORD PTR DS:[EDI+0x4],0x0
$+501    007F8F91     75 0A                        JNZ SHORT PathOfEx.007F8F9D
$+503    007F8F93     807D 20 00                   CMP BYTE PTR SS:[EBP+0x20],0x0
$+507    007F8F97     0F84 AD020000                JE PathOfEx.007F924A
$+50D    007F8F9D     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+510    007F8FA0     8338 0D                      CMP DWORD PTR DS:[EAX],0xD
$+513    007F8FA3     75 0A                        JNZ SHORT PathOfEx.007F8FAF
$+515    007F8FA5     837D E4 2A                   CMP DWORD PTR SS:[EBP-0x1C],0x2A
$+519    007F8FA9     0F85 9B020000                JNZ PathOfEx.007F924A
$+51F    007F8FAF     8378 04 1C                   CMP DWORD PTR DS:[EAX+0x4],0x1C
$+523    007F8FB3     75 40                        JNZ SHORT PathOfEx.007F8FF5
$+525    007F8FB5     8BCB                         MOV ECX,EBX
$+527    007F8FB7     E8 E427F5FF                  CALL PathOfEx.0074B7A0
$+52C    007F8FBC     84C0                         TEST AL,AL
$+52E    007F8FBE     0F85 86020000                JNZ PathOfEx.007F924A
$+534    007F8FC4     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+537    007F8FC7     E8 94FACBFF                  CALL PathOfEx.004B8A60
$+53C    007F8FCC     85C0                         TEST EAX,EAX
$+53E    007F8FCE     0F84 76020000                JE PathOfEx.007F924A
$+544    007F8FD4     8BC8                         MOV ECX,EAX
$+546    007F8FD6     E8 85F5F4FF                  CALL PathOfEx.00748560
$+54B    007F8FDB     5F                           POP EDI
$+54C    007F8FDC     5E                           POP ESI
$+54D    007F8FDD     5B                           POP EBX
$+54E    007F8FDE     8378 40 02                   CMP DWORD PTR DS:[EAX+0x40],0x2
$+552    007F8FE2     0F94C0                       SETE AL
$+555    007F8FE5     8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+558    007F8FE8     64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+55F    007F8FEF     8BE5                         MOV ESP,EBP
$+561    007F8FF1     5D                           POP EBP
$+562    007F8FF2     C2 1C00                      RETN 0x1C
$+565    007F8FF5     8338 12                      CMP DWORD PTR DS:[EAX],0x12
$+568    007F8FF8     8B5D 08                      MOV EBX,DWORD PTR SS:[EBP+0x8]
$+56B    007F8FFB     75 6E                        JNZ SHORT PathOfEx.007F906B
$+56D    007F8FFD     8BCB                         MOV ECX,EBX
$+56F    007F8FFF     E8 8C6ECDFF                  CALL PathOfEx.004CFE90
$+574    007F9004     84C0                         TEST AL,AL
$+576    007F9006     0F84 3E020000                JE PathOfEx.007F924A
$+57C    007F900C     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+57F    007F900F     E8 6C29F5FF                  CALL PathOfEx.0074B980
$+584    007F9014     83F8 2B                      CMP EAX,0x2B
$+587    007F9017     0F84 2D020000                JE PathOfEx.007F924A
$+58D    007F901D     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+590    007F9020     E8 BB28F5FF                  CALL PathOfEx.0074B8E0
$+595    007F9025     84C0                         TEST AL,AL
$+597    007F9027     0F85 1D020000                JNZ PathOfEx.007F924A
$+59D    007F902D     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+5A0    007F9030     E8 0B28F5FF                  CALL PathOfEx.0074B840
$+5A5    007F9035     84C0                         TEST AL,AL
$+5A7    007F9037     0F85 0D020000                JNZ PathOfEx.007F924A
$+5AD    007F903D     8B4D EC                      MOV ECX,DWORD PTR SS:[EBP-0x14]
$+5B0    007F9040     E8 5B27F5FF                  CALL PathOfEx.0074B7A0
$+5B5    007F9045     84C0                         TEST AL,AL
$+5B7    007F9047     0F85 FD010000                JNZ PathOfEx.007F924A
$+5BD    007F904D     8BCB                         MOV ECX,EBX
$+5BF    007F904F     E8 CC4CCCFF                  CALL PathOfEx.004BDD20
$+5C4    007F9054     84C0                         TEST AL,AL
$+5C6    007F9056     0F85 EE010000                JNZ PathOfEx.007F924A
$+5CC    007F905C     8BCB                         MOV ECX,EBX
$+5CE    007F905E     E8 5D0B0000                  CALL PathOfEx.007F9BC0
$+5D3    007F9063     84C0                         TEST AL,AL
$+5D5    007F9065     0F85 DF010000                JNZ PathOfEx.007F924A
$+5DB    007F906B     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+5DE    007F906E     8378 04 1D                   CMP DWORD PTR DS:[EAX+0x4],0x1D
$+5E2    007F9072     75 60                        JNZ SHORT PathOfEx.007F90D4
$+5E4    007F9074     83EC 10                      SUB ESP,0x10
$+5E7    007F9077     E8 84EFF1FF                  CALL PathOfEx.00718000
$+5EC    007F907C     8BC8                         MOV ECX,EAX
$+5EE    007F907E     E8 8D30DBFF                  CALL PathOfEx.005AC110
$+5F3    007F9083     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+5F6    007F9086     8B45 EC                      MOV EAX,DWORD PTR SS:[EBP-0x14]
$+5F9    007F9089     8D55 20                      LEA EDX,DWORD PTR SS:[EBP+0x20]
$+5FC    007F908C     C745 FC 09000000             MOV DWORD PTR SS:[EBP-0x4],0x9
$+603    007F9093     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+606    007F9096     8B40 08                      MOV EAX,DWORD PTR DS:[EAX+0x8]
$+609    007F9099     83C0 48                      ADD EAX,0x48
$+60C    007F909C     50                           PUSH EAX
$+60D    007F909D     E8 1ED1F5FF                  CALL PathOfEx.007561C0
$+612    007F90A2     83C4 04                      ADD ESP,0x4
$+615    007F90A5     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+618    007F90A8     837D D4 00                   CMP DWORD PTR SS:[EBP-0x2C],0x0
$+61C    007F90AC     75 12                        JNZ SHORT PathOfEx.007F90C0
$+61E    007F90AE     E8 ED6BC7FF                  CALL PathOfEx.0046FCA0
$+623    007F90B3     8D4D 20                      LEA ECX,DWORD PTR SS:[EBP+0x20]
$+626    007F90B6     E8 553AC7FF                  CALL PathOfEx.0046CB10
$+62B    007F90BB     E9 8A010000                  JMP PathOfEx.007F924A
$+630    007F90C0     E8 DB6BC7FF                  CALL PathOfEx.0046FCA0
$+635    007F90C5     8D4D 20                      LEA ECX,DWORD PTR SS:[EBP+0x20]
$+638    007F90C8     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+63F    007F90CF     E8 3C3AC7FF                  CALL PathOfEx.0046CB10
$+644    007F90D4     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+647    007F90D7     8078 09 00                   CMP BYTE PTR DS:[EAX+0x9],0x0
$+64B    007F90DB     0F84 1C040000                JE PathOfEx.007F94FD
$+651    007F90E1     8B5D E8                      MOV EBX,DWORD PTR SS:[EBP-0x18]
$+654    007F90E4     837B 04 00                   CMP DWORD PTR DS:[EBX+0x4],0x0
$+658    007F90E8     0F84 5C010000                JE PathOfEx.007F924A
$+65E    007F90EE     8B7D 08                      MOV EDI,DWORD PTR SS:[EBP+0x8]
$+661    007F90F1     8BCF                         MOV ECX,EDI
$+663    007F90F3     E8 68F9CBFF                  CALL PathOfEx.004B8A60
$+668    007F90F8     85C0                         TEST EAX,EAX
$+66A    007F90FA     0F84 4A010000                JE PathOfEx.007F924A
$+670    007F9100     8B40 14                      MOV EAX,DWORD PTR DS:[EAX+0x14]
$+673    007F9103     8B48 24                      MOV ECX,DWORD PTR DS:[EAX+0x24]
$+676    007F9106     83F9 02                      CMP ECX,0x2
$+679    007F9109     0F85 5A020000                JNZ PathOfEx.007F9369
$+67F    007F910F     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+682    007F9112     3948 04                      CMP DWORD PTR DS:[EAX+0x4],ECX
$+685    007F9115     0F85 44010000                JNZ PathOfEx.007F925F
$+68B    007F911B     6A 03                        PUSH 0x3
$+68D    007F911D     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+690    007F9120     8BCB                         MOV ECX,EBX
$+692    007F9122     50                           PUSH EAX
$+693    007F9123     E8 B836D1FF                  CALL PathOfEx.0050C7E0
$+698    007F9128     6A 00                        PUSH 0x0
$+69A    007F912A     C745 FC 0A000000             MOV DWORD PTR SS:[EBP-0x4],0xA
$+6A1    007F9131     8B4D D8                      MOV ECX,DWORD PTR SS:[EBP-0x28]
$+6A4    007F9134     6A 00                        PUSH 0x0
$+6A6    007F9136     E8 7576F5FF                  CALL PathOfEx.007507B0
$+6AB    007F913B     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+6AE    007F913E     85C0                         TEST EAX,EAX
$+6B0    007F9140     0F84 91000000                JE PathOfEx.007F91D7
$+6B6    007F9146     8BCF                         MOV ECX,EDI
$+6B8    007F9148     E8 D3D9DAFF                  CALL PathOfEx.005A6B20
$+6BD    007F914D     8945 14                      MOV DWORD PTR SS:[EBP+0x14],EAX
$+6C0    007F9150     8D48 08                      LEA ECX,DWORD PTR DS:[EAX+0x8]
$+6C3    007F9153     E8 986DF7FF                  CALL PathOfEx.0076FEF0
$+6C8    007F9158     8B7D 20                      MOV EDI,DWORD PTR SS:[EBP+0x20]
$+6CB    007F915B     8B4D D8                      MOV ECX,DWORD PTR SS:[EBP-0x28]
$+6CE    007F915E     57                           PUSH EDI
$+6CF    007F915F     84C0                         TEST AL,AL
$+6D1    007F9161     0F84 84000000                JE PathOfEx.007F91EB
$+6D7    007F9167     E8 7476F5FF                  CALL PathOfEx.007507E0
$+6DC    007F916C     8BC8                         MOV ECX,EAX
$+6DE    007F916E     E8 BD10CCFF                  CALL PathOfEx.004BA230
$+6E3    007F9173     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+6E6    007F9176     8B45 14                      MOV EAX,DWORD PTR SS:[EBP+0x14]
$+6E9    007F9179     8BC8                         MOV ECX,EAX
$+6EB    007F917B     E8 4068F7FF                  CALL PathOfEx.0076F9C0
$+6F0    007F9180     84C0                         TEST AL,AL
$+6F2    007F9182     74 40                        JE SHORT PathOfEx.007F91C4
$+6F4    007F9184     8B4D 20                      MOV ECX,DWORD PTR SS:[EBP+0x20]
$+6F7    007F9187     E8 F427F5FF                  CALL PathOfEx.0074B980
$+6FC    007F918C     83F8 15                      CMP EAX,0x15
$+6FF    007F918F     75 33                        JNZ SHORT PathOfEx.007F91C4
$+701    007F9191     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+705    007F9195     74 40                        JE SHORT PathOfEx.007F91D7
$+707    007F9197     8B45 18                      MOV EAX,DWORD PTR SS:[EBP+0x18]
$+70A    007F919A     8B4D 0C                      MOV ECX,DWORD PTR SS:[EBP+0xC]
$+70D    007F919D     6A 00                        PUSH 0x0
$+70F    007F919F     6A 00                        PUSH 0x0
$+711    007F91A1     8908                         MOV DWORD PTR DS:[EAX],ECX
$+713    007F91A3     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+716    007F91A6     E8 0576F5FF                  CALL PathOfEx.007507B0
$+71B    007F91AB     8B4D 1C                      MOV ECX,DWORD PTR SS:[EBP+0x1C]
$+71E    007F91AE     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+725    007F91B5     8901                         MOV DWORD PTR DS:[ECX],EAX
$+727    007F91B7     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+72A    007F91BA     E8 3118C7FF                  CALL PathOfEx.0046A9F0
$+72F    007F91BF     E9 B6020000                  JMP PathOfEx.007F947A
$+734    007F91C4     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+738    007F91C8     75 78                        JNZ SHORT PathOfEx.007F9242
$+73A    007F91CA     8B43 14                      MOV EAX,DWORD PTR DS:[EBX+0x14]
$+73D    007F91CD     8B4D 18                      MOV ECX,DWORD PTR SS:[EBP+0x18]
$+740    007F91D0     8901                         MOV DWORD PTR DS:[ECX],EAX
$+742    007F91D2     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+745    007F91D5     8938                         MOV DWORD PTR DS:[EAX],EDI
$+747    007F91D7     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+74A    007F91DA     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+751    007F91E1     E8 0A18C7FF                  CALL PathOfEx.0046A9F0
$+756    007F91E6     E9 8F020000                  JMP PathOfEx.007F947A
$+75B    007F91EB     E8 F075F5FF                  CALL PathOfEx.007507E0
$+760    007F91F0     8BC8                         MOV ECX,EAX
$+762    007F91F2     E8 29D9DAFF                  CALL PathOfEx.005A6B20
$+767    007F91F7     85C0                         TEST EAX,EAX
$+769    007F91F9     74 21                        JE SHORT PathOfEx.007F921C
$+76B    007F91FB     8B40 10                      MOV EAX,DWORD PTR DS:[EAX+0x10]
$+76E    007F91FE     33C9                         XOR ECX,ECX
$+770    007F9200     8B10                         MOV EDX,DWORD PTR DS:[EAX]
$+772    007F9202     8B45 14                      MOV EAX,DWORD PTR SS:[EBP+0x14]
$+775    007F9205     8B40 10                      MOV EAX,DWORD PTR DS:[EAX+0x10]
$+778    007F9208     8338 06                      CMP DWORD PTR DS:[EAX],0x6
$+77B    007F920B     0F94C1                       SETE CL
$+77E    007F920E     33C0                         XOR EAX,EAX
$+780    007F9210     83FA 06                      CMP EDX,0x6
$+783    007F9213     0F94C0                       SETE AL
$+786    007F9216     3BC8                         CMP ECX,EAX
$+788    007F9218   ^ 74 BD                        JE SHORT PathOfEx.007F91D7
$+78A    007F921A     EB 26                        JMP SHORT PathOfEx.007F9242
$+78C    007F921C     8B4D D8                      MOV ECX,DWORD PTR SS:[EBP-0x28]
$+78F    007F921F     57                           PUSH EDI
$+790    007F9220     E8 BB75F5FF                  CALL PathOfEx.007507E0
$+795    007F9225     8BC8                         MOV ECX,EAX
$+797    007F9227     E8 0410CCFF                  CALL PathOfEx.004BA230
$+79C    007F922C     85C0                         TEST EAX,EAX
$+79E    007F922E   ^ 74 A7                        JE SHORT PathOfEx.007F91D7
$+7A0    007F9230     8BC8                         MOV ECX,EAX
$+7A2    007F9232     E8 4927F5FF                  CALL PathOfEx.0074B980
$+7A7    007F9237     83F8 15                      CMP EAX,0x15
$+7AA    007F923A   ^ 75 9B                        JNZ SHORT PathOfEx.007F91D7
$+7AC    007F923C     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+7B0    007F9240   ^ 74 88                        JE SHORT PathOfEx.007F91CA
$+7B2    007F9242     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+7B5    007F9245     E8 A617C7FF                  CALL PathOfEx.0046A9F0
$+7BA    007F924A     5F                           POP EDI
$+7BB    007F924B     5E                           POP ESI
$+7BC    007F924C     32C0                         XOR AL,AL
$+7BE    007F924E     5B                           POP EBX
$+7BF    007F924F     8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+7C2    007F9252     64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+7C9    007F9259     8BE5                         MOV ESP,EBP
$+7CB    007F925B     5D                           POP EBP
$+7CC    007F925C     C2 1C00                      RETN 0x1C
$+7CF    007F925F     8378 04 03                   CMP DWORD PTR DS:[EAX+0x4],0x3
$+7D3    007F9263     0F85 06020000                JNZ PathOfEx.007F946F
$+7D9    007F9269     6A 02                        PUSH 0x2
$+7DB    007F926B     8D45 D8                      LEA EAX,DWORD PTR SS:[EBP-0x28]
$+7DE    007F926E     8BCB                         MOV ECX,EBX
$+7E0    007F9270     50                           PUSH EAX
$+7E1    007F9271     E8 6A35D1FF                  CALL PathOfEx.0050C7E0
$+7E6    007F9276     6A 00                        PUSH 0x0
$+7E8    007F9278     C745 FC 0B000000             MOV DWORD PTR SS:[EBP-0x4],0xB
$+7EF    007F927F     8B4D D8                      MOV ECX,DWORD PTR SS:[EBP-0x28]
$+7F2    007F9282     6A 00                        PUSH 0x0
$+7F4    007F9284     E8 2775F5FF                  CALL PathOfEx.007507B0
$+7F9    007F9289     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+7FC    007F928C     85C0                         TEST EAX,EAX
$+7FE    007F928E   ^ 0F84 43FFFFFF                JE PathOfEx.007F91D7
$+804    007F9294     8BCF                         MOV ECX,EDI
$+806    007F9296     E8 85D8DAFF                  CALL PathOfEx.005A6B20
$+80B    007F929B     8945 14                      MOV DWORD PTR SS:[EBP+0x14],EAX
$+80E    007F929E     8D48 08                      LEA ECX,DWORD PTR DS:[EAX+0x8]
$+811    007F92A1     E8 4A6CF7FF                  CALL PathOfEx.0076FEF0
$+816    007F92A6     84C0                         TEST AL,AL
$+818    007F92A8     74 4C                        JE SHORT PathOfEx.007F92F6
$+81A    007F92AA     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+81E    007F92AE     74 3B                        JE SHORT PathOfEx.007F92EB
$+820    007F92B0     8BCF                         MOV ECX,EDI
$+822    007F92B2     E8 69D8DAFF                  CALL PathOfEx.005A6B20
$+827    007F92B7     8BC8                         MOV ECX,EAX
$+829    007F92B9     E8 0267F7FF                  CALL PathOfEx.0076F9C0
$+82E    007F92BE     84C0                         TEST AL,AL
$+830    007F92C0   ^ 74 80                        JE SHORT PathOfEx.007F9242
$+832    007F92C2     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+835    007F92C5     6A 00                        PUSH 0x0
$+837    007F92C7     6A 00                        PUSH 0x0
$+839    007F92C9     E8 E274F5FF                  CALL PathOfEx.007507B0
$+83E    007F92CE     50                           PUSH EAX
$+83F    007F92CF     E8 0C75F5FF                  CALL PathOfEx.007507E0
$+844    007F92D4     8BC8                         MOV ECX,EAX
$+846    007F92D6     E8 550FCCFF                  CALL PathOfEx.004BA230
$+84B    007F92DB     8BC8                         MOV ECX,EAX
$+84D    007F92DD     E8 9E26F5FF                  CALL PathOfEx.0074B980
$+852    007F92E2     83F8 15                      CMP EAX,0x15
$+855    007F92E5   ^ 0F85 57FFFFFF                JNZ PathOfEx.007F9242
$+85B    007F92EB     8B43 10                      MOV EAX,DWORD PTR DS:[EBX+0x10]
$+85E    007F92EE     8B7D 20                      MOV EDI,DWORD PTR SS:[EBP+0x20]
$+861    007F92F1   ^ E9 D7FEFFFF                  JMP PathOfEx.007F91CD
$+866    007F92F6     8B7D 20                      MOV EDI,DWORD PTR SS:[EBP+0x20]
$+869    007F92F9     8B4D D8                      MOV ECX,DWORD PTR SS:[EBP-0x28]
$+86C    007F92FC     57                           PUSH EDI
$+86D    007F92FD     E8 DE74F5FF                  CALL PathOfEx.007507E0
$+872    007F9302     8BC8                         MOV ECX,EAX
$+874    007F9304     E8 17D8DAFF                  CALL PathOfEx.005A6B20
$+879    007F9309     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+87C    007F930C     8D48 08                      LEA ECX,DWORD PTR DS:[EAX+0x8]
$+87F    007F930F     E8 DC6BF7FF                  CALL PathOfEx.0076FEF0
$+884    007F9314     84C0                         TEST AL,AL
$+886    007F9316     74 17                        JE SHORT PathOfEx.007F932F
$+888    007F9318     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+88C    007F931C   ^ 0F85 20FFFFFF                JNZ PathOfEx.007F9242
$+892    007F9322     8B4D 18                      MOV ECX,DWORD PTR SS:[EBP+0x18]
$+895    007F9325     8B43 10                      MOV EAX,DWORD PTR DS:[EBX+0x10]
$+898    007F9328     8901                         MOV DWORD PTR DS:[ECX],EAX
$+89A    007F932A     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+89D    007F932D     8938                         MOV DWORD PTR DS:[EAX],EDI
$+89F    007F932F     8B45 20                      MOV EAX,DWORD PTR SS:[EBP+0x20]
$+8A2    007F9332     33C9                         XOR ECX,ECX
$+8A4    007F9334     8B40 10                      MOV EAX,DWORD PTR DS:[EAX+0x10]
$+8A7    007F9337     8B10                         MOV EDX,DWORD PTR DS:[EAX]
$+8A9    007F9339     8B45 14                      MOV EAX,DWORD PTR SS:[EBP+0x14]
$+8AC    007F933C     8B40 10                      MOV EAX,DWORD PTR DS:[EAX+0x10]
$+8AF    007F933F     8338 06                      CMP DWORD PTR DS:[EAX],0x6
$+8B2    007F9342     0F94C1                       SETE CL
$+8B5    007F9345     33C0                         XOR EAX,EAX
$+8B7    007F9347     83FA 06                      CMP EDX,0x6
$+8BA    007F934A     0F94C0                       SETE AL
$+8BD    007F934D     3BC8                         CMP ECX,EAX
$+8BF    007F934F   ^ 0F85 EDFEFFFF                JNZ PathOfEx.007F9242
$+8C5    007F9355     8D4D D8                      LEA ECX,DWORD PTR SS:[EBP-0x28]
$+8C8    007F9358     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+8CF    007F935F     E8 8C16C7FF                  CALL PathOfEx.0046A9F0
$+8D4    007F9364     E9 11010000                  JMP PathOfEx.007F947A
$+8D9    007F9369     83F9 03                      CMP ECX,0x3
$+8DC    007F936C     0F85 FD000000                JNZ PathOfEx.007F946F
$+8E2    007F9372     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+8E5    007F9375     3948 04                      CMP DWORD PTR DS:[EAX+0x4],ECX
$+8E8    007F9378     75 7B                        JNZ SHORT PathOfEx.007F93F5
$+8EA    007F937A     6A 02                        PUSH 0x2
$+8EC    007F937C     8D45 D0                      LEA EAX,DWORD PTR SS:[EBP-0x30]
$+8EF    007F937F     8BCB                         MOV ECX,EBX
$+8F1    007F9381     50                           PUSH EAX
$+8F2    007F9382     E8 5934D1FF                  CALL PathOfEx.0050C7E0
$+8F7    007F9387     6A 00                        PUSH 0x0
$+8F9    007F9389     C745 FC 0C000000             MOV DWORD PTR SS:[EBP-0x4],0xC
$+900    007F9390     8B4D D0                      MOV ECX,DWORD PTR SS:[EBP-0x30]
$+903    007F9393     6A 00                        PUSH 0x0
$+905    007F9395     E8 1674F5FF                  CALL PathOfEx.007507B0
$+90A    007F939A     8BF8                         MOV EDI,EAX
$+90C    007F939C     85FF                         TEST EDI,EDI
$+90E    007F939E     0F84 BA000000                JE PathOfEx.007F945E
$+914    007F93A4     57                           PUSH EDI
$+915    007F93A5     E8 3674F5FF                  CALL PathOfEx.007507E0
$+91A    007F93AA     8BC8                         MOV ECX,EAX
$+91C    007F93AC     E8 6FD7DAFF                  CALL PathOfEx.005A6B20
$+921    007F93B1     8945 20                      MOV DWORD PTR SS:[EBP+0x20],EAX
$+924    007F93B4     8D48 08                      LEA ECX,DWORD PTR DS:[EAX+0x8]
$+927    007F93B7     E8 346BF7FF                  CALL PathOfEx.0076FEF0
$+92C    007F93BC     84C0                         TEST AL,AL
$+92E    007F93BE     74 21                        JE SHORT PathOfEx.007F93E1
$+930    007F93C0     8B4D 20                      MOV ECX,DWORD PTR SS:[EBP+0x20]
$+933    007F93C3     E8 F865F7FF                  CALL PathOfEx.0076F9C0
$+938    007F93C8     84C0                         TEST AL,AL
$+93A    007F93CA     74 0A                        JE SHORT PathOfEx.007F93D6
$+93C    007F93CC     837D E4 15                   CMP DWORD PTR SS:[EBP-0x1C],0x15
$+940    007F93D0     0F84 88000000                JE PathOfEx.007F945E
$+946    007F93D6     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+94A    007F93DA     75 11                        JNZ SHORT PathOfEx.007F93ED
$+94C    007F93DC     8B43 10                      MOV EAX,DWORD PTR DS:[EBX+0x10]
$+94F    007F93DF     EB 73                        JMP SHORT PathOfEx.007F9454
$+951    007F93E1     837D E4 15                   CMP DWORD PTR SS:[EBP-0x1C],0x15
$+955    007F93E5     75 77                        JNZ SHORT PathOfEx.007F945E
$+957    007F93E7     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+95B    007F93EB   ^ 74 EF                        JE SHORT PathOfEx.007F93DC
$+95D    007F93ED     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+960    007F93F0   ^ E9 50FEFFFF                  JMP PathOfEx.007F9245
$+965    007F93F5     8378 04 02                   CMP DWORD PTR DS:[EAX+0x4],0x2
$+969    007F93F9     75 74                        JNZ SHORT PathOfEx.007F946F
$+96B    007F93FB     6A 03                        PUSH 0x3
$+96D    007F93FD     8D45 D0                      LEA EAX,DWORD PTR SS:[EBP-0x30]
$+970    007F9400     8BCB                         MOV ECX,EBX
$+972    007F9402     50                           PUSH EAX
$+973    007F9403     E8 D833D1FF                  CALL PathOfEx.0050C7E0
$+978    007F9408     6A 00                        PUSH 0x0
$+97A    007F940A     C745 FC 0D000000             MOV DWORD PTR SS:[EBP-0x4],0xD
$+981    007F9411     8B4D D0                      MOV ECX,DWORD PTR SS:[EBP-0x30]
$+984    007F9414     6A 00                        PUSH 0x0
$+986    007F9416     E8 9573F5FF                  CALL PathOfEx.007507B0
$+98B    007F941B     8BF8                         MOV EDI,EAX
$+98D    007F941D     85FF                         TEST EDI,EDI
$+98F    007F941F     74 3D                        JE SHORT PathOfEx.007F945E
$+991    007F9421     807D F3 00                   CMP BYTE PTR SS:[EBP-0xD],0x0
$+995    007F9425     74 2A                        JE SHORT PathOfEx.007F9451
$+997    007F9427     837D E4 15                   CMP DWORD PTR SS:[EBP-0x1C],0x15
$+99B    007F942B   ^ 75 C0                        JNZ SHORT PathOfEx.007F93ED
$+99D    007F942D     8B4E 04                      MOV ECX,DWORD PTR DS:[ESI+0x4]
$+9A0    007F9430     6A 00                        PUSH 0x0
$+9A2    007F9432     6A 00                        PUSH 0x0
$+9A4    007F9434     E8 7773F5FF                  CALL PathOfEx.007507B0
$+9A9    007F9439     50                           PUSH EAX
$+9AA    007F943A     E8 A173F5FF                  CALL PathOfEx.007507E0
$+9AF    007F943F     8BC8                         MOV ECX,EAX
$+9B1    007F9441     E8 DAD6DAFF                  CALL PathOfEx.005A6B20
$+9B6    007F9446     8BC8                         MOV ECX,EAX
$+9B8    007F9448     E8 7365F7FF                  CALL PathOfEx.0076F9C0
$+9BD    007F944D     84C0                         TEST AL,AL
$+9BF    007F944F   ^ 74 9C                        JE SHORT PathOfEx.007F93ED
$+9C1    007F9451     8B43 14                      MOV EAX,DWORD PTR DS:[EBX+0x14]
$+9C4    007F9454     8B4D 18                      MOV ECX,DWORD PTR SS:[EBP+0x18]
$+9C7    007F9457     8901                         MOV DWORD PTR DS:[ECX],EAX
$+9C9    007F9459     8B45 1C                      MOV EAX,DWORD PTR SS:[EBP+0x1C]
$+9CC    007F945C     8938                         MOV DWORD PTR DS:[EAX],EDI
$+9CE    007F945E     8D4D D0                      LEA ECX,DWORD PTR SS:[EBP-0x30]
$+9D1    007F9461     C745 FC FFFFFFFF             MOV DWORD PTR SS:[EBP-0x4],-0x1
$+9D8    007F9468     E8 8315C7FF                  CALL PathOfEx.0046A9F0
$+9DD    007F946D     EB 0B                        JMP SHORT PathOfEx.007F947A
$+9DF    007F946F     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+9E2    007F9472     3908                         CMP DWORD PTR DS:[EAX],ECX
$+9E4    007F9474   ^ 0F85 D0FDFFFF                JNZ PathOfEx.007F924A
$+9EA    007F947A     8B4B 04                      MOV ECX,DWORD PTR DS:[EBX+0x4]
$+9ED    007F947D     E8 4E92C9FF                  CALL PathOfEx.004926D0
$+9F2    007F9482     8B4B 04                      MOV ECX,DWORD PTR DS:[EBX+0x4]
$+9F5    007F9485     8BF8                         MOV EDI,EAX
$+9F7    007F9487     E8 E429C7FF                  CALL PathOfEx.0046BE70
$+9FC    007F948C     8BD8                         MOV EBX,EAX
$+9FE    007F948E     51                           PUSH ECX
$+9FF    007F948F     8B4D 08                      MOV ECX,DWORD PTR SS:[EBP+0x8]
$+A02    007F9492     FF73 10                      PUSH DWORD PTR DS:[EBX+0x10]
$+A05    007F9495     FF77 40                      PUSH DWORD PTR DS:[EDI+0x40]
$+A08    007F9498     E8 C36EF7FF                  CALL PathOfEx.00770360
$+A0D    007F949D     84C0                         TEST AL,AL
$+A0F    007F949F   ^ 0F84 A5FDFFFF                JE PathOfEx.007F924A
$+A15    007F94A5     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+A18    007F94A8     8378 04 06                   CMP DWORD PTR DS:[EAX+0x4],0x6
$+A1C    007F94AC     75 14                        JNZ SHORT PathOfEx.007F94C2
$+A1E    007F94AE     8B4B 10                      MOV ECX,DWORD PTR DS:[EBX+0x10]
$+A21    007F94B1     68 26050000                  PUSH 0x526
$+A26    007F94B6     8B01                         MOV EAX,DWORD PTR DS:[ECX]
$+A28    007F94B8     FF10                         CALL DWORD PTR DS:[EAX]
$+A2A    007F94BA     85C0                         TEST EAX,EAX
$+A2C    007F94BC   ^ 0F85 88FDFFFF                JNZ PathOfEx.007F924A
$+A32    007F94C2     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+A35    007F94C5     8378 04 07                   CMP DWORD PTR DS:[EAX+0x4],0x7
$+A39    007F94C9     75 14                        JNZ SHORT PathOfEx.007F94DF
$+A3B    007F94CB     8B4B 10                      MOV ECX,DWORD PTR DS:[EBX+0x10]
$+A3E    007F94CE     68 27050000                  PUSH 0x527
$+A43    007F94D3     8B01                         MOV EAX,DWORD PTR DS:[ECX]
$+A45    007F94D5     FF10                         CALL DWORD PTR DS:[EAX]
$+A47    007F94D7     85C0                         TEST EAX,EAX
$+A49    007F94D9   ^ 0F85 6BFDFFFF                JNZ PathOfEx.007F924A
$+A4F    007F94DF     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+A52    007F94E2     8378 04 01                   CMP DWORD PTR DS:[EAX+0x4],0x1
$+A56    007F94E6     75 72                        JNZ SHORT PathOfEx.007F955A
$+A58    007F94E8     8B4B 10                      MOV ECX,DWORD PTR DS:[EBX+0x10]
$+A5B    007F94EB     68 46050000                  PUSH 0x546
$+A60    007F94F0     8B01                         MOV EAX,DWORD PTR DS:[ECX]
$+A62    007F94F2     FF10                         CALL DWORD PTR DS:[EAX]
$+A64    007F94F4     85C0                         TEST EAX,EAX
$+A66    007F94F6     74 62                        JE SHORT PathOfEx.007F955A
$+A68    007F94F8   ^ E9 4DFDFFFF                  JMP PathOfEx.007F924A
$+A6D    007F94FD     8338 0C                      CMP DWORD PTR DS:[EAX],0xC
$+A70    007F9500     75 28                        JNZ SHORT PathOfEx.007F952A
$+A72    007F9502     8BCB                         MOV ECX,EBX
$+A74    007F9504     E8 77D6DAFF                  CALL PathOfEx.005A6B80
$+A79    007F9509     84C0                         TEST AL,AL
$+A7B    007F950B     75 1D                        JNZ SHORT PathOfEx.007F952A
$+A7D    007F950D     8BCB                         MOV ECX,EBX
$+A7F    007F950F     E8 4CF5CBFF                  CALL PathOfEx.004B8A60
$+A84    007F9514     85C0                         TEST EAX,EAX
$+A86    007F9516   ^ 0F84 2EFDFFFF                JE PathOfEx.007F924A
$+A8C    007F951C     8B40 14                      MOV EAX,DWORD PTR DS:[EAX+0x14]
$+A8F    007F951F     8378 24 0C                   CMP DWORD PTR DS:[EAX+0x24],0xC
$+A93    007F9523     74 35                        JE SHORT PathOfEx.007F955A
$+A95    007F9525   ^ E9 20FDFFFF                  JMP PathOfEx.007F924A
$+A9A    007F952A     8B46 04                      MOV EAX,DWORD PTR DS:[ESI+0x4]
$+A9D    007F952D     8338 0D                      CMP DWORD PTR DS:[EAX],0xD
$+AA0    007F9530     75 28                        JNZ SHORT PathOfEx.007F955A
$+AA2    007F9532     8B4F 04                      MOV ECX,DWORD PTR DS:[EDI+0x4]
$+AA5    007F9535     E8 9691C9FF                  CALL PathOfEx.004926D0
$+AAA    007F953A     8B4F 04                      MOV ECX,DWORD PTR DS:[EDI+0x4]
$+AAD    007F953D     8BF0                         MOV ESI,EAX
$+AAF    007F953F     E8 2C29C7FF                  CALL PathOfEx.0046BE70
$+AB4    007F9544     51                           PUSH ECX
$+AB5    007F9545     8BCB                         MOV ECX,EBX
$+AB7    007F9547     FF70 10                      PUSH DWORD PTR DS:[EAX+0x10]
$+ABA    007F954A     FF76 40                      PUSH DWORD PTR DS:[ESI+0x40]
$+ABD    007F954D     E8 0E6EF7FF                  CALL PathOfEx.00770360
$+AC2    007F9552     84C0                         TEST AL,AL
$+AC4    007F9554   ^ 0F84 F0FCFFFF                JE PathOfEx.007F924A
$+ACA    007F955A     8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+ACD    007F955D     B0 01                        MOV AL,0x1
$+ACF    007F955F     5F                           POP EDI
$+AD0    007F9560     5E                           POP ESI
$+AD1    007F9561     5B                           POP EBX
$+AD2    007F9562     64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+AD9    007F9569     8BE5                         MOV ESP,EBP
$+ADB    007F956B     5D                           POP EBP
$+ADC    007F956C     C2 1C00                      RETN 0x1C
$+ADF    007F956F     CC                           INT3
*/
/**/
//tag_判断是否需要显示物品tips并显示之
/*
$-1      005B0B8F     CC                   INT3
$ ==>    005B0B90     56                   PUSH ESI                                 ; tag_判断是否需要显示物品tips并显示之
$+1      005B0B91     8BF1                 MOV ESI,ECX								; ecx是ui对象，一直会断到
$+3      005B0B93     80BE A9080000 01     CMP BYTE PTR DS:[ESI+0x8A9],0x1
$+A      005B0B9A     74 1B                JE SHORT PathOfEx.005B0BB7
$+C      005B0B9C     80BE AA080000 01     CMP BYTE PTR DS:[ESI+0x8AA],0x1
$+13     005B0BA3     74 12                JE SHORT PathOfEx.005B0BB7
$+15     005B0BA5     80BE AB080000 01     CMP BYTE PTR DS:[ESI+0x8AB],0x1
$+1C     005B0BAC     74 09                JE SHORT PathOfEx.005B0BB7
$+1E     005B0BAE     80BE AC080000 01     CMP BYTE PTR DS:[ESI+0x8AC],0x1
$+25     005B0BB5     75 0F                JNZ SHORT PathOfEx.005B0BC6
$+27     005B0BB7     8B86 3C080000        MOV EAX,DWORD PTR DS:[ESI+0x83C]
$+2D     005B0BBD     8D8E 3C080000        LEA ECX,DWORD PTR DS:[ESI+0x83C]
$+33     005B0BC3     FF50 10              CALL DWORD PTR DS:[EAX+0x10]
$+36     005B0BC6     80BE F4080000 00     CMP BYTE PTR DS:[ESI+0x8F4],0x0
$+3D     005B0BCD     74 2D                JE SHORT PathOfEx.005B0BFC
$+3F     005B0BCF     F686 C0080000 10     TEST BYTE PTR DS:[ESI+0x8C0],0x10
$+46     005B0BD6     75 0B                JNZ SHORT PathOfEx.005B0BE3
$+48     005B0BD8     8BCE                 MOV ECX,ESI
$+4A     005B0BDA     E8 01C44300          CALL PathOfEx.009ECFE0
$+4F     005B0BDF     84C0                 TEST AL,AL
$+51     005B0BE1     74 19                JE SHORT PathOfEx.005B0BFC
$+53     005B0BE3     8BCE                 MOV ECX,ESI
$+55     005B0BE5     C686 F4080000 00     MOV BYTE PTR DS:[ESI+0x8F4],0x0
$+5C     005B0BEC     E8 2FFCFFFF          CALL PathOfEx.005B0820                   ; call tag_无参创建物品tips
$+61     005B0BF1     8B16                 MOV EDX,DWORD PTR DS:[ESI]
$+63     005B0BF3     8BCE                 MOV ECX,ESI
$+65     005B0BF5     50                   PUSH EAX
$+66     005B0BF6     FF92 5C010000        CALL DWORD PTR DS:[EDX+0x15C]            ; call tag_根据stCD_ItemTipsParams来显示tips
$+6C     005B0BFC     5E                   POP ESI
$+6D     005B0BFD     C2 0800              RETN 0x8
$+70     005B0C00     56                   PUSH ESI
$+71     005B0C01     FF7424 08            PUSH DWORD PTR SS:[ESP+0x8]
$+75     005B0C05     8BF1                 MOV ESI,ECX
$+77     005B0C07     E8 24DC4300          CALL PathOfEx.009EE830
$+7C     005B0C0C     F686 C0080000 04     TEST BYTE PTR DS:[ESI+0x8C0],0x4
$+83     005B0C13     74 2A                JE SHORT PathOfEx.005B0C3F
$+85     005B0C15     80BE F4080000 00     CMP BYTE PTR DS:[ESI+0x8F4],0x0
$+8C     005B0C1C     75 21                JNZ SHORT PathOfEx.005B0C3F
$+8E     005B0C1E     8D86 D8080000        LEA EAX,DWORD PTR DS:[ESI+0x8D8]
$+94     005B0C24     C686 F4080000 01     MOV BYTE PTR DS:[ESI+0x8F4],0x1
$+9B     005B0C2B     8378 14 08           CMP DWORD PTR DS:[EAX+0x14],0x8
$+9F     005B0C2F     C740 10 00000000     MOV DWORD PTR DS:[EAX+0x10],0x0
$+A6     005B0C36     72 02                JB SHORT PathOfEx.005B0C3A
$+A8     005B0C38     8B00                 MOV EAX,DWORD PTR DS:[EAX]
$+AA     005B0C3A     33C9                 XOR ECX,ECX
$+AC     005B0C3C     66:8908              MOV WORD PTR DS:[EAX],CX
$+AF     005B0C3F     5E                   POP ESI
$+B0     005B0C40     C2 0400              RETN 0x4
$+B3     005B0C43     CC                   INT3
*/
/**/
//tag_无参创建物品tips
/*
$-1      005B081F     CC                   INT3
$ ==>    005B0820     83EC 08              SUB ESP,0x8                              ; tag_无参创建物品tips
$+3      005B0823     53                   PUSH EBX
$+4      005B0824     8BD9                 MOV EBX,ECX
$+6      005B0826     56                   PUSH ESI
$+7      005B0827     57                   PUSH EDI
$+8      005B0828     F683 C0080000 04     TEST BYTE PTR DS:[EBX+0x8C0],0x4
$+F      005B082F     75 0D                JNZ SHORT PathOfEx.005B083E
$+11     005B0831     8B83 F0080000        MOV EAX,DWORD PTR DS:[EBX+0x8F0]
$+17     005B0837     5F                   POP EDI
$+18     005B0838     5E                   POP ESI
$+19     005B0839     5B                   POP EBX
$+1A     005B083A     83C4 08              ADD ESP,0x8
$+1D     005B083D     C3                   RETN
$+1E     005B083E     83BB CC080000 00     CMP DWORD PTR DS:[EBX+0x8CC],0x0
$+25     005B0845     74 47                JE SHORT PathOfEx.005B088E
$+27     005B0847     8BBB C8080000        MOV EDI,DWORD PTR DS:[EBX+0x8C8]
$+2D     005B084D     8BCF                 MOV ECX,EDI
$+2F     005B084F     8B77 40              MOV ESI,DWORD PTR DS:[EDI+0x40]
$+32     005B0852     E8 C95F2300          CALL PathOfEx.007E6820
$+37     005B0857     8B83 C0080000        MOV EAX,DWORD PTR DS:[EBX+0x8C0]
$+3D     005B085D     B9 20FA0001          MOV ECX,PathOfEx.0100FA20
$+42     005B0862     FFB3 D4080000        PUSH DWORD PTR DS:[EBX+0x8D4]
$+48     005B0868     D1E8                 SHR EAX,1
$+4A     005B086A     24 01                AND AL,0x1
$+4C     005B086C     0FB6C0               MOVZX EAX,AL
$+4F     005B086F     50                   PUSH EAX
$+50     005B0870     56                   PUSH ESI
$+51     005B0871     8D47 34              LEA EAX,DWORD PTR DS:[EDI+0x34]
$+54     005B0874     50                   PUSH EAX
$+55     005B0875     6A 00                PUSH 0x0
$+57     005B0877     FFB3 CC080000        PUSH DWORD PTR DS:[EBX+0x8CC]
$+5D     005B087D     E8 1E121700          CALL PathOfEx.00721AA0                   ; call tag_索引物品属性并创建tips
$+62     005B0882     B8 20FA0001          MOV EAX,PathOfEx.0100FA20
$+67     005B0887     5F                   POP EDI
$+68     005B0888     5E                   POP ESI
$+69     005B0889     5B                   POP EBX
$+6A     005B088A     83C4 08              ADD ESP,0x8
$+6D     005B088D     C3                   RETN
$+6E     005B088E     8B83 D0080000        MOV EAX,DWORD PTR DS:[EBX+0x8D0]
$+74     005B0894     8B48 04              MOV ECX,DWORD PTR DS:[EAX+0x4]
$+77     005B0897     E8 34FDEBFF          CALL PathOfEx.004705D0
$+7C     005B089C     85C0                 TEST EAX,EAX
$+7E     005B089E     74 0D                JE SHORT PathOfEx.005B08AD
$+80     005B08A0     8378 6C 08           CMP DWORD PTR DS:[EAX+0x6C],0x8
$+84     005B08A4     8D48 58              LEA ECX,DWORD PTR DS:[EAX+0x58]
$+87     005B08A7     72 09                JB SHORT PathOfEx.005B08B2
$+89     005B08A9     8B09                 MOV ECX,DWORD PTR DS:[ECX]
$+8B     005B08AB     EB 05                JMP SHORT PathOfEx.005B08B2
$+8D     005B08AD     B9 B4FCE600          MOV ECX,PathOfEx.00E6FCB4
$+92     005B08B2     8B93 D0080000        MOV EDX,DWORD PTR DS:[EBX+0x8D0]
$+98     005B08B8     8B82 10010000        MOV EAX,DWORD PTR DS:[EDX+0x110]
$+9E     005B08BE     85C0                 TEST EAX,EAX
$+A0     005B08C0     74 05                JE SHORT PathOfEx.005B08C7
$+A2     005B08C2     8B70 08              MOV ESI,DWORD PTR DS:[EAX+0x8]
$+A5     005B08C5     EB 02                JMP SHORT PathOfEx.005B08C9
$+A7     005B08C7     33F6                 XOR ESI,ESI
$+A9     005B08C9     8A82 20010000        MOV AL,BYTE PTR DS:[EDX+0x120]
$+AF     005B08CF     83C2 10              ADD EDX,0x10
$+B2     005B08D2     884424 10            MOV BYTE PTR SS:[ESP+0x10],AL
$+B6     005B08D6     56                   PUSH ESI
$+B7     005B08D7     FF7424 14            PUSH DWORD PTR SS:[ESP+0x14]
$+BB     005B08DB     E8 E0F9FFFF          CALL PathOfEx.005B02C0
$+C0     005B08E0     83C4 08              ADD ESP,0x8
$+C3     005B08E3     5F                   POP EDI
$+C4     005B08E4     5E                   POP ESI
$+C5     005B08E5     5B                   POP EBX
$+C6     005B08E6     83C4 08              ADD ESP,0x8
$+C9     005B08E9     C3                   RETN
$+CA     005B08EA     CC                   INT3
*/
/**/
//tag_索引物品属性并创建tips

//tag_创建物品tips
/*
$-1      00721A9F     CC                    INT3
$ ==>    00721AA0     83EC 54               SUB ESP,0x54                                     ; tag_索引物品属性并创建tips
$+3      00721AA3     53                    PUSH EBX				;ecx=stCD_ItemTipsParams*
$+4      00721AA4     55                    PUSH EBP				;p1=stCD_EntityItemBased*
$+5      00721AA5     56                    PUSH ESI				;p2=0,p4=3f,p5=0,p6=0
$+6      00721AA6     57                    PUSH EDI
$+7      00721AA7     8B7C24 68             MOV EDI,DWORD PTR SS:[ESP+0x68]                  ; 执行后edi=stCD_EntityItemBased*
$+B      00721AAB     894C24 60             MOV DWORD PTR SS:[ESP+0x60],ECX
$+F      00721AAF     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
$+11     00721AB1     85C9                  TEST ECX,ECX
$+13     00721AB3     74 05                 JE SHORT PathOfEx.00721ABA
$+15     00721AB5     E8 067BFFFF           CALL PathOfEx.007195C0
$+1A     00721ABA     8B07                  MOV EAX,DWORD PTR DS:[EDI]                       ; 执行后eax=stCD_DbEntity*
$+1C     00721ABC     C74424 68 4077ED00    MOV DWORD PTR SS:[ESP+0x68],PathOfEx.00ED7740    ; ASCII "Mods"
$+24     00721AC4     8B70 30               MOV ESI,DWORD PTR DS:[EAX+0x30]                  ; 执行后esi=stCD_DbTblEntity*
$+27     00721AC7     8D4424 68             LEA EAX,DWORD PTR SS:[ESP+0x68]
$+2B     00721ACB     50                    PUSH EAX
$+2C     00721ACC     8D4424 58             LEA EAX,DWORD PTR SS:[ESP+0x58]
$+30     00721AD0     50                    PUSH EAX
$+31     00721AD1     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]                  ; 查找属性索引信息
$+34     00721AD4     E8 67A5D4FF           CALL PathOfEx.0046C040                           ; 得到stCD_DbTblEntity::AttrInfoCont::iterator
$+39     00721AD9     8B4424 54             MOV EAX,DWORD PTR SS:[ESP+0x54]
$+3D     00721ADD     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+40     00721AE0     74 55                 JE SHORT PathOfEx.00721B37
$+42     00721AE2     8B70 0C               MOV ESI,DWORD PTR DS:[EAX+0xC]
$+45     00721AE5     83FE FF               CMP ESI,-0x1
$+48     00721AE8     74 4D                 JE SHORT PathOfEx.00721B37
$+4A     00721AEA     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+4D     00721AED     8B34B0                MOV ESI,DWORD PTR DS:[EAX+ESI*4]
$+50     00721AF0     85F6                  TEST ESI,ESI                                     ; esi=stCD_EntityAttrMods*
$+52     00721AF2     74 43                 JE SHORT PathOfEx.00721B37
$+54     00721AF4     80BE 21010000 00      CMP BYTE PTR DS:[ESI+0x121],0x0
$+5B     00721AFB     0FB696 14010000       MOVZX EDX,BYTE PTR DS:[ESI+0x114]
$+62     00721B02     895424 68             MOV DWORD PTR SS:[ESP+0x68],EDX
$+66     00721B06     0FB696 15010000       MOVZX EDX,BYTE PTR DS:[ESI+0x115]
$+6D     00721B0D     895424 10             MOV DWORD PTR SS:[ESP+0x10],EDX
$+71     00721B11     8B96 18010000         MOV EDX,DWORD PTR DS:[ESI+0x118]
$+77     00721B17     895424 14             MOV DWORD PTR SS:[ESP+0x14],EDX
$+7B     00721B1B     8B96 1C010000         MOV EDX,DWORD PTR DS:[ESI+0x11C]
$+81     00721B21     895424 18             MOV DWORD PTR SS:[ESP+0x18],EDX
$+85     00721B25     75 07                 JNZ SHORT PathOfEx.00721B2E
$+87     00721B27     8BCE                  MOV ECX,ESI
$+89     00721B29     E8 526A0200           CALL PathOfEx.00748580
$+8E     00721B2E     8D46 18               LEA EAX,DWORD PTR DS:[ESI+0x18]
$+91     00721B31     894424 54             MOV DWORD PTR SS:[ESP+0x54],EAX                  ; eax=stCD_ItemTipsParams*
$+95     00721B35     EB 28                 JMP SHORT PathOfEx.00721B5F
$+97     00721B37     C74424 68 00000000    MOV DWORD PTR SS:[ESP+0x68],0x0
$+9F     00721B3F     C74424 10 00000000    MOV DWORD PTR SS:[ESP+0x10],0x0
$+A7     00721B47     C74424 14 00000000    MOV DWORD PTR SS:[ESP+0x14],0x0
$+AF     00721B4F     C74424 18 00000000    MOV DWORD PTR SS:[ESP+0x18],0x0
$+B7     00721B57     C74424 54 00000000    MOV DWORD PTR SS:[ESP+0x54],0x0
$+BF     00721B5F     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
$+C1     00721B61     85C9                  TEST ECX,ECX
$+C3     00721B63     74 05                 JE SHORT PathOfEx.00721B6A
$+C5     00721B65     E8 567AFFFF           CALL PathOfEx.007195C0
$+CA     00721B6A     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+CC     00721B6C     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+D0     00721B70     50                    PUSH EAX
$+D1     00721B71     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+D5     00721B75     C74424 54 6C80ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED806C    ; ASCII "Prophecy"
$+DD     00721B7D     50                    PUSH EAX
$+DE     00721B7E     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+E1     00721B81     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+E4     00721B84     E8 B7A4D4FF           CALL PathOfEx.0046C040
$+E9     00721B89     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+ED     00721B8D     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+F0     00721B90     74 14                 JE SHORT PathOfEx.00721BA6
$+F2     00721B92     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+F5     00721B95     83F9 FF               CMP ECX,-0x1
$+F8     00721B98     74 0C                 JE SHORT PathOfEx.00721BA6
$+FA     00721B9A     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+FD     00721B9D     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+100    00721BA0     894424 1C             MOV DWORD PTR SS:[ESP+0x1C],EAX
$+104    00721BA4     EB 08                 JMP SHORT PathOfEx.00721BAE
$+106    00721BA6     C74424 1C 00000000    MOV DWORD PTR SS:[ESP+0x1C],0x0
$+10E    00721BAE     85DB                  TEST EBX,EBX
$+110    00721BB0     74 07                 JE SHORT PathOfEx.00721BB9
$+112    00721BB2     8BCB                  MOV ECX,EBX
$+114    00721BB4     E8 077AFFFF           CALL PathOfEx.007195C0
$+119    00721BB9     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+11B    00721BBB     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+11F    00721BBF     50                    PUSH EAX
$+120    00721BC0     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+124    00721BC4     C74424 54 34A7ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00EDA734    ; ASCII "Imprint"
$+12C    00721BCC     50                    PUSH EAX
$+12D    00721BCD     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+130    00721BD0     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+133    00721BD3     E8 68A4D4FF           CALL PathOfEx.0046C040
$+138    00721BD8     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+13C    00721BDC     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+13F    00721BDF     74 14                 JE SHORT PathOfEx.00721BF5
$+141    00721BE1     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+144    00721BE4     83F9 FF               CMP ECX,-0x1
$+147    00721BE7     74 0C                 JE SHORT PathOfEx.00721BF5
$+149    00721BE9     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+14C    00721BEC     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+14F    00721BEF     894424 20             MOV DWORD PTR SS:[ESP+0x20],EAX
$+153    00721BF3     EB 08                 JMP SHORT PathOfEx.00721BFD
$+155    00721BF5     C74424 20 00000000    MOV DWORD PTR SS:[ESP+0x20],0x0
$+15D    00721BFD     85DB                  TEST EBX,EBX
$+15F    00721BFF     74 07                 JE SHORT PathOfEx.00721C08
$+161    00721C01     8BCB                  MOV ECX,EBX
$+163    00721C03     E8 B879FFFF           CALL PathOfEx.007195C0
$+168    00721C08     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+16A    00721C0A     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+16E    00721C0E     50                    PUSH EAX
$+16F    00721C0F     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+173    00721C13     C74424 54 2090ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED9020    ; ASCII "Flask"
$+17B    00721C1B     50                    PUSH EAX
$+17C    00721C1C     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+17F    00721C1F     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+182    00721C22     E8 19A4D4FF           CALL PathOfEx.0046C040
$+187    00721C27     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+18B    00721C2B     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+18E    00721C2E     74 14                 JE SHORT PathOfEx.00721C44
$+190    00721C30     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+193    00721C33     83F9 FF               CMP ECX,-0x1
$+196    00721C36     74 0C                 JE SHORT PathOfEx.00721C44
$+198    00721C38     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+19B    00721C3B     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+19E    00721C3E     894424 24             MOV DWORD PTR SS:[ESP+0x24],EAX
$+1A2    00721C42     EB 08                 JMP SHORT PathOfEx.00721C4C
$+1A4    00721C44     C74424 24 00000000    MOV DWORD PTR SS:[ESP+0x24],0x0
$+1AC    00721C4C     85DB                  TEST EBX,EBX
$+1AE    00721C4E     74 07                 JE SHORT PathOfEx.00721C57
$+1B0    00721C50     8BCB                  MOV ECX,EBX
$+1B2    00721C52     E8 6979FFFF           CALL PathOfEx.007195C0
$+1B7    00721C57     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+1B9    00721C59     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+1BD    00721C5D     50                    PUSH EAX
$+1BE    00721C5E     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+1C2    00721C62     C74424 54 B48EED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED8EB4    ; ASCII "Map"
$+1CA    00721C6A     50                    PUSH EAX
$+1CB    00721C6B     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+1CE    00721C6E     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+1D1    00721C71     E8 CAA3D4FF           CALL PathOfEx.0046C040
$+1D6    00721C76     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+1DA    00721C7A     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+1DD    00721C7D     74 14                 JE SHORT PathOfEx.00721C93
$+1DF    00721C7F     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+1E2    00721C82     83F9 FF               CMP ECX,-0x1
$+1E5    00721C85     74 0C                 JE SHORT PathOfEx.00721C93
$+1E7    00721C87     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+1EA    00721C8A     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+1ED    00721C8D     894424 28             MOV DWORD PTR SS:[ESP+0x28],EAX
$+1F1    00721C91     EB 08                 JMP SHORT PathOfEx.00721C9B
$+1F3    00721C93     C74424 28 00000000    MOV DWORD PTR SS:[ESP+0x28],0x0
$+1FB    00721C9B     85DB                  TEST EBX,EBX
$+1FD    00721C9D     74 07                 JE SHORT PathOfEx.00721CA6
$+1FF    00721C9F     8BCB                  MOV ECX,EBX
$+201    00721CA1     E8 1A79FFFF           CALL PathOfEx.007195C0
$+206    00721CA6     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+208    00721CA8     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+20C    00721CAC     50                    PUSH EAX
$+20D    00721CAD     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+211    00721CB1     C74424 54 F0A6ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00EDA6F0    ; ASCII "Shield"
$+219    00721CB9     50                    PUSH EAX
$+21A    00721CBA     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+21D    00721CBD     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+220    00721CC0     E8 7BA3D4FF           CALL PathOfEx.0046C040
$+225    00721CC5     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+229    00721CC9     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+22C    00721CCC     74 14                 JE SHORT PathOfEx.00721CE2
$+22E    00721CCE     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+231    00721CD1     83F9 FF               CMP ECX,-0x1
$+234    00721CD4     74 0C                 JE SHORT PathOfEx.00721CE2
$+236    00721CD6     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+239    00721CD9     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+23C    00721CDC     894424 2C             MOV DWORD PTR SS:[ESP+0x2C],EAX
$+240    00721CE0     EB 08                 JMP SHORT PathOfEx.00721CEA
$+242    00721CE2     C74424 2C 00000000    MOV DWORD PTR SS:[ESP+0x2C],0x0
$+24A    00721CEA     85DB                  TEST EBX,EBX
$+24C    00721CEC     74 07                 JE SHORT PathOfEx.00721CF5
$+24E    00721CEE     8BCB                  MOV ECX,EBX
$+250    00721CF0     E8 CB78FFFF           CALL PathOfEx.007195C0
$+255    00721CF5     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+257    00721CF7     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+25B    00721CFB     50                    PUSH EAX
$+25C    00721CFC     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+260    00721D00     C74424 54 2477ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED7724    ; ASCII "LocalStats"
$+268    00721D08     50                    PUSH EAX
$+269    00721D09     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+26C    00721D0C     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+26F    00721D0F     E8 2CA3D4FF           CALL PathOfEx.0046C040
$+274    00721D14     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+278    00721D18     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+27B    00721D1B     74 14                 JE SHORT PathOfEx.00721D31
$+27D    00721D1D     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+280    00721D20     83F9 FF               CMP ECX,-0x1
$+283    00721D23     74 0C                 JE SHORT PathOfEx.00721D31
$+285    00721D25     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+288    00721D28     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+28B    00721D2B     894424 30             MOV DWORD PTR SS:[ESP+0x30],EAX
$+28F    00721D2F     EB 08                 JMP SHORT PathOfEx.00721D39
$+291    00721D31     C74424 30 00000000    MOV DWORD PTR SS:[ESP+0x30],0x0
$+299    00721D39     85DB                  TEST EBX,EBX
$+29B    00721D3B     74 07                 JE SHORT PathOfEx.00721D44
$+29D    00721D3D     8BCB                  MOV ECX,EBX
$+29F    00721D3F     E8 7C78FFFF           CALL PathOfEx.007195C0
$+2A4    00721D44     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+2A6    00721D46     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+2AA    00721D4A     50                    PUSH EAX
$+2AB    00721D4B     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+2AF    00721D4F     C74424 54 F47EED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED7EF4    ; ASCII "Quest"
$+2B7    00721D57     50                    PUSH EAX
$+2B8    00721D58     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+2BB    00721D5B     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+2BE    00721D5E     E8 DDA2D4FF           CALL PathOfEx.0046C040
$+2C3    00721D63     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+2C7    00721D67     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+2CA    00721D6A     74 14                 JE SHORT PathOfEx.00721D80
$+2CC    00721D6C     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+2CF    00721D6F     83F9 FF               CMP ECX,-0x1
$+2D2    00721D72     74 0C                 JE SHORT PathOfEx.00721D80
$+2D4    00721D74     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+2D7    00721D77     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+2DA    00721D7A     894424 34             MOV DWORD PTR SS:[ESP+0x34],EAX
$+2DE    00721D7E     EB 08                 JMP SHORT PathOfEx.00721D88
$+2E0    00721D80     C74424 34 00000000    MOV DWORD PTR SS:[ESP+0x34],0x0
$+2E8    00721D88     85DB                  TEST EBX,EBX
$+2EA    00721D8A     74 07                 JE SHORT PathOfEx.00721D93
$+2EC    00721D8C     8BCB                  MOV ECX,EBX
$+2EE    00721D8E     E8 2D78FFFF           CALL PathOfEx.007195C0
$+2F3    00721D93     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+2F5    00721D95     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+2F9    00721D99     50                    PUSH EAX
$+2FA    00721D9A     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+2FE    00721D9E     C74424 54 AC81ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED81AC    ; ASCII "Stack"
$+306    00721DA6     50                    PUSH EAX
$+307    00721DA7     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+30A    00721DAA     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+30D    00721DAD     E8 8EA2D4FF           CALL PathOfEx.0046C040
$+312    00721DB2     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+316    00721DB6     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+319    00721DB9     74 14                 JE SHORT PathOfEx.00721DCF
$+31B    00721DBB     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+31E    00721DBE     83F9 FF               CMP ECX,-0x1
$+321    00721DC1     74 0C                 JE SHORT PathOfEx.00721DCF
$+323    00721DC3     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+326    00721DC6     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+329    00721DC9     894424 38             MOV DWORD PTR SS:[ESP+0x38],EAX
$+32D    00721DCD     EB 08                 JMP SHORT PathOfEx.00721DD7
$+32F    00721DCF     C74424 38 00000000    MOV DWORD PTR SS:[ESP+0x38],0x0
$+337    00721DD7     85DB                  TEST EBX,EBX
$+339    00721DD9     74 07                 JE SHORT PathOfEx.00721DE2
$+33B    00721DDB     8BCB                  MOV ECX,EBX
$+33D    00721DDD     E8 DE77FFFF           CALL PathOfEx.007195C0
$+342    00721DE2     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+344    00721DE4     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+348    00721DE8     50                    PUSH EAX
$+349    00721DE9     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+34D    00721DED     C74424 54 3489ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED8934    ; ASCII "AttributeRequirements"
$+355    00721DF5     50                    PUSH EAX
$+356    00721DF6     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+359    00721DF9     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+35C    00721DFC     E8 3FA2D4FF           CALL PathOfEx.0046C040
$+361    00721E01     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+365    00721E05     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+368    00721E08     74 14                 JE SHORT PathOfEx.00721E1E
$+36A    00721E0A     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+36D    00721E0D     83F9 FF               CMP ECX,-0x1
$+370    00721E10     74 0C                 JE SHORT PathOfEx.00721E1E
$+372    00721E12     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+375    00721E15     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+378    00721E18     894424 3C             MOV DWORD PTR SS:[ESP+0x3C],EAX
$+37C    00721E1C     EB 08                 JMP SHORT PathOfEx.00721E26
$+37E    00721E1E     C74424 3C 00000000    MOV DWORD PTR SS:[ESP+0x3C],0x0
$+386    00721E26     85DB                  TEST EBX,EBX
$+388    00721E28     74 07                 JE SHORT PathOfEx.00721E31
$+38A    00721E2A     8BCB                  MOV ECX,EBX
$+38C    00721E2C     E8 8F77FFFF           CALL PathOfEx.007195C0
$+391    00721E31     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+393    00721E33     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+397    00721E37     50                    PUSH EAX
$+398    00721E38     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+39C    00721E3C     C74424 54 C07DED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED7DC0    ; ASCII "SkillGem"
$+3A4    00721E44     50                    PUSH EAX
$+3A5    00721E45     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+3A8    00721E48     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+3AB    00721E4B     E8 F0A1D4FF           CALL PathOfEx.0046C040
$+3B0    00721E50     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+3B4    00721E54     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+3B7    00721E57     74 14                 JE SHORT PathOfEx.00721E6D
$+3B9    00721E59     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+3BC    00721E5C     83F9 FF               CMP ECX,-0x1
$+3BF    00721E5F     74 0C                 JE SHORT PathOfEx.00721E6D
$+3C1    00721E61     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+3C4    00721E64     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+3C7    00721E67     894424 40             MOV DWORD PTR SS:[ESP+0x40],EAX
$+3CB    00721E6B     EB 08                 JMP SHORT PathOfEx.00721E75
$+3CD    00721E6D     C74424 40 00000000    MOV DWORD PTR SS:[ESP+0x40],0x0
$+3D5    00721E75     85DB                  TEST EBX,EBX
$+3D7    00721E77     74 07                 JE SHORT PathOfEx.00721E80
$+3D9    00721E79     8BCB                  MOV ECX,EBX
$+3DB    00721E7B     E8 4077FFFF           CALL PathOfEx.007195C0
$+3E0    00721E80     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+3E2    00721E82     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+3E6    00721E86     50                    PUSH EAX
$+3E7    00721E87     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+3EB    00721E8B     C74424 54 EC7BED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED7BEC    ; ASCII "Quality"
$+3F3    00721E93     50                    PUSH EAX
$+3F4    00721E94     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+3F7    00721E97     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+3FA    00721E9A     E8 A1A1D4FF           CALL PathOfEx.0046C040
$+3FF    00721E9F     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+403    00721EA3     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+406    00721EA6     74 14                 JE SHORT PathOfEx.00721EBC
$+408    00721EA8     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+40B    00721EAB     83F9 FF               CMP ECX,-0x1
$+40E    00721EAE     74 0C                 JE SHORT PathOfEx.00721EBC
$+410    00721EB0     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+413    00721EB3     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+416    00721EB6     894424 44             MOV DWORD PTR SS:[ESP+0x44],EAX
$+41A    00721EBA     EB 08                 JMP SHORT PathOfEx.00721EC4
$+41C    00721EBC     C74424 44 00000000    MOV DWORD PTR SS:[ESP+0x44],0x0
$+424    00721EC4     85DB                  TEST EBX,EBX
$+426    00721EC6     74 07                 JE SHORT PathOfEx.00721ECF
$+428    00721EC8     8BCB                  MOV ECX,EBX
$+42A    00721ECA     E8 F176FFFF           CALL PathOfEx.007195C0
$+42F    00721ECF     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+431    00721ED1     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+435    00721ED5     50                    PUSH EAX
$+436    00721ED6     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+43A    00721EDA     C74424 54 D88CED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED8CD8    ; ASCII "Charges"
$+442    00721EE2     50                    PUSH EAX
$+443    00721EE3     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+446    00721EE6     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+449    00721EE9     E8 52A1D4FF           CALL PathOfEx.0046C040
$+44E    00721EEE     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+452    00721EF2     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+455    00721EF5     74 14                 JE SHORT PathOfEx.00721F0B
$+457    00721EF7     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+45A    00721EFA     83F9 FF               CMP ECX,-0x1
$+45D    00721EFD     74 0C                 JE SHORT PathOfEx.00721F0B
$+45F    00721EFF     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+462    00721F02     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+465    00721F05     894424 48             MOV DWORD PTR SS:[ESP+0x48],EAX
$+469    00721F09     EB 08                 JMP SHORT PathOfEx.00721F13
$+46B    00721F0B     C74424 48 00000000    MOV DWORD PTR SS:[ESP+0x48],0x0
$+473    00721F13     85DB                  TEST EBX,EBX
$+475    00721F15     74 07                 JE SHORT PathOfEx.00721F1E
$+477    00721F17     8BCB                  MOV ECX,EBX
$+479    00721F19     E8 A276FFFF           CALL PathOfEx.007195C0
$+47E    00721F1E     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+480    00721F20     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+484    00721F24     50                    PUSH EAX
$+485    00721F25     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+489    00721F29     C74424 54 D090ED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED90D0    ; ASCII "Armour"
$+491    00721F31     50                    PUSH EAX
$+492    00721F32     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+495    00721F35     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+498    00721F38     E8 03A1D4FF           CALL PathOfEx.0046C040
$+49D    00721F3D     8B4424 4C             MOV EAX,DWORD PTR SS:[ESP+0x4C]
$+4A1    00721F41     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+4A4    00721F44     74 14                 JE SHORT PathOfEx.00721F5A
$+4A6    00721F46     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+4A9    00721F49     83F9 FF               CMP ECX,-0x1
$+4AC    00721F4C     74 0C                 JE SHORT PathOfEx.00721F5A
$+4AE    00721F4E     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+4B1    00721F51     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+4B4    00721F54     894424 4C             MOV DWORD PTR SS:[ESP+0x4C],EAX
$+4B8    00721F58     EB 08                 JMP SHORT PathOfEx.00721F62
$+4BA    00721F5A     C74424 4C 00000000    MOV DWORD PTR SS:[ESP+0x4C],0x0
$+4C2    00721F62     85DB                  TEST EBX,EBX
$+4C4    00721F64     74 07                 JE SHORT PathOfEx.00721F6D
$+4C6    00721F66     8BCB                  MOV ECX,EBX
$+4C8    00721F68     E8 5376FFFF           CALL PathOfEx.007195C0
$+4CD    00721F6D     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+4CF    00721F6F     8D4424 50             LEA EAX,DWORD PTR SS:[ESP+0x50]
$+4D3    00721F73     50                    PUSH EAX
$+4D4    00721F74     8D4424 5C             LEA EAX,DWORD PTR SS:[ESP+0x5C]
$+4D8    00721F78     C74424 54 388EED00    MOV DWORD PTR SS:[ESP+0x54],PathOfEx.00ED8E38    ; ASCII "Weapon"
$+4E0    00721F80     50                    PUSH EAX
$+4E1    00721F81     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+4E4    00721F84     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+4E7    00721F87     E8 B4A0D4FF           CALL PathOfEx.0046C040
$+4EC    00721F8C     8B4424 58             MOV EAX,DWORD PTR SS:[ESP+0x58]
$+4F0    00721F90     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+4F3    00721F93     74 14                 JE SHORT PathOfEx.00721FA9
$+4F5    00721F95     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+4F8    00721F98     83F9 FF               CMP ECX,-0x1
$+4FB    00721F9B     74 0C                 JE SHORT PathOfEx.00721FA9
$+4FD    00721F9D     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+500    00721FA0     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+503    00721FA3     894424 50             MOV DWORD PTR SS:[ESP+0x50],EAX
$+507    00721FA7     EB 08                 JMP SHORT PathOfEx.00721FB1
$+509    00721FA9     C74424 50 00000000    MOV DWORD PTR SS:[ESP+0x50],0x0
$+511    00721FB1     85DB                  TEST EBX,EBX
$+513    00721FB3     74 07                 JE SHORT PathOfEx.00721FBC
$+515    00721FB5     8BCB                  MOV ECX,EBX
$+517    00721FB7     E8 0476FFFF           CALL PathOfEx.007195C0
$+51C    00721FBC     8B1F                  MOV EBX,DWORD PTR DS:[EDI]
$+51E    00721FBE     8D4424 58             LEA EAX,DWORD PTR SS:[ESP+0x58]
$+522    00721FC2     50                    PUSH EAX
$+523    00721FC3     8D4424 60             LEA EAX,DWORD PTR SS:[ESP+0x60]
$+527    00721FC7     C74424 5C DC80ED00    MOV DWORD PTR SS:[ESP+0x5C],PathOfEx.00ED80DC    ; ASCII "Sockets"
$+52F    00721FCF     50                    PUSH EAX
$+530    00721FD0     8B73 30               MOV ESI,DWORD PTR DS:[EBX+0x30]
$+533    00721FD3     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+536    00721FD6     E8 65A0D4FF           CALL PathOfEx.0046C040
$+53B    00721FDB     8B4424 5C             MOV EAX,DWORD PTR SS:[ESP+0x5C]
$+53F    00721FDF     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+542    00721FE2     74 10                 JE SHORT PathOfEx.00721FF4
$+544    00721FE4     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+547    00721FE7     83F9 FF               CMP ECX,-0x1
$+54A    00721FEA     74 08                 JE SHORT PathOfEx.00721FF4
$+54C    00721FEC     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+54F    00721FEF     8B2C88                MOV EBP,DWORD PTR DS:[EAX+ECX*4]
$+552    00721FF2     EB 02                 JMP SHORT PathOfEx.00721FF6
$+554    00721FF4     33ED                  XOR EBP,EBP
$+556    00721FF6     85DB                  TEST EBX,EBX
$+558    00721FF8     74 07                 JE SHORT PathOfEx.00722001
$+55A    00721FFA     8BCB                  MOV ECX,EBX
$+55C    00721FFC     E8 BF75FFFF           CALL PathOfEx.007195C0
$+561    00722001     8B07                  MOV EAX,DWORD PTR DS:[EDI]
$+563    00722003     C74424 5C 5830EA00    MOV DWORD PTR SS:[ESP+0x5C],PathOfEx.00EA3058    ; ASCII "Base"
$+56B    0072200B     8B70 30               MOV ESI,DWORD PTR DS:[EAX+0x30]
$+56E    0072200E     8D4424 5C             LEA EAX,DWORD PTR SS:[ESP+0x5C]
$+572    00722012     50                    PUSH EAX
$+573    00722013     8D4424 5C             LEA EAX,DWORD PTR SS:[ESP+0x5C]
$+577    00722017     50                    PUSH EAX
$+578    00722018     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
$+57B    0072201B     E8 20A0D4FF           CALL PathOfEx.0046C040
$+580    00722020     8B4424 58             MOV EAX,DWORD PTR SS:[ESP+0x58]
$+584    00722024     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
$+587    00722027     74 10                 JE SHORT PathOfEx.00722039
$+589    00722029     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
$+58C    0072202C     83F9 FF               CMP ECX,-0x1
$+58F    0072202F     74 08                 JE SHORT PathOfEx.00722039
$+591    00722031     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
$+594    00722034     8B0488                MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+597    00722037     EB 02                 JMP SHORT PathOfEx.0072203B
$+599    00722039     33C0                  XOR EAX,EAX
$+59B    0072203B     68 B4FCE600           PUSH PathOfEx.00E6FCB4                           ; 在此下断，看哪个是需要关注的属性地址，并设访问断点来找字段
$+5A0    00722040     6A 00                 PUSH 0x0
$+5A2    00722042     FFB424 84000000       PUSH DWORD PTR SS:[ESP+0x84]
$+5A9    00722049     8B4C24 6C             MOV ECX,DWORD PTR SS:[ESP+0x6C]
$+5AD    0072204D     FFB424 84000000       PUSH DWORD PTR SS:[ESP+0x84]
$+5B4    00722054     FF7424 78             PUSH DWORD PTR SS:[ESP+0x78]
$+5B8    00722058     FF7424 24             PUSH DWORD PTR SS:[ESP+0x24]
$+5BC    0072205C     FF7424 2C             PUSH DWORD PTR SS:[ESP+0x2C]
$+5C0    00722060     FF7424 34             PUSH DWORD PTR SS:[ESP+0x34]
$+5C4    00722064     FFB424 94000000       PUSH DWORD PTR SS:[ESP+0x94]
$+5CB    0072206B     FFB424 94000000       PUSH DWORD PTR SS:[ESP+0x94]                     ; Usable
$+5D2    00722072     FFB424 94000000       PUSH DWORD PTR SS:[ESP+0x94]
$+5D9    00722079     FF7424 48             PUSH DWORD PTR SS:[ESP+0x48]
$+5DD    0072207D     FF7424 50             PUSH DWORD PTR SS:[ESP+0x50]
$+5E1    00722081     FF7424 58             PUSH DWORD PTR SS:[ESP+0x58]                     ; Flask
$+5E5    00722085     FF7424 60             PUSH DWORD PTR SS:[ESP+0x60]
$+5E9    00722089     FF7424 68             PUSH DWORD PTR SS:[ESP+0x68]                     ; Shield
$+5ED    0072208D     FF7424 70             PUSH DWORD PTR SS:[ESP+0x70]                     ; LocalStats
$+5F1    00722091     FF7424 78             PUSH DWORD PTR SS:[ESP+0x78]
$+5F5    00722095     FFB424 80000000       PUSH DWORD PTR SS:[ESP+0x80]
$+5FC    0072209C     FFB424 88000000       PUSH DWORD PTR SS:[ESP+0x88]
$+603    007220A3     FFB424 90000000       PUSH DWORD PTR SS:[ESP+0x90]
$+60A    007220AA     FFB424 98000000       PUSH DWORD PTR SS:[ESP+0x98]                     ; Quality
$+611    007220B1     FFB424 A0000000       PUSH DWORD PTR SS:[ESP+0xA0]                     ; Charges
$+618    007220B8     FFB424 A8000000       PUSH DWORD PTR SS:[ESP+0xA8]
$+61F    007220BF     FFB424 B0000000       PUSH DWORD PTR SS:[ESP+0xB0]
$+626    007220C6     55                    PUSH EBP
$+627    007220C7     50                    PUSH EAX                                         ; Base
$+628    007220C8     FFB424 C0000000       PUSH DWORD PTR SS:[ESP+0xC0]
$+62F    007220CF     68 B4FCE600           PUSH PathOfEx.00E6FCB4
$+634    007220D4     E8 17000000           CALL PathOfEx.007220F0                           ; call tag_创建物品tips
$+639    007220D9     5F                    POP EDI
$+63A    007220DA     5E                    POP ESI
$+63B    007220DB     5D                    POP EBP
$+63C    007220DC     5B                    POP EBX
$+63D    007220DD     83C4 54               ADD ESP,0x54
$+640    007220E0     C2 1800               RETN 0x18
$+643    007220E3     CC                    INT3
$+644    007220E4     CC                    INT3
$+645    007220E5     CC                    INT3
$+646    007220E6     CC                    INT3
$+647    007220E7     CC                    INT3
$+648    007220E8     CC                    INT3
$+649    007220E9     CC                    INT3
$+64A    007220EA     CC                    INT3
$+64B    007220EB     CC                    INT3
$+64C    007220EC     CC                    INT3
$+64D    007220ED     CC                    INT3                                             ; 每个ui对象只会创建一次,函数非常大
$+64E    007220EE     CC                    INT3                                             ; 参数非常多(29个参数)。参数大多数都是物品的属性地址
$+64F    007220EF     CC                    INT3                                             ; 需要显示什么信息，参数中就有什么属性地址
$+650    007220F0     55                    PUSH EBP                                         ; tag_创建物品tips
$+651    007220F1     8BEC                  MOV EBP,ESP                                      ; ecx=stCD_ItemTipsParams*
$+653    007220F3     6A FF                 PUSH -0x1
$+655    007220F5     68 E79DD400           PUSH PathOfEx.00D49DE7
$+65A    007220FA     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
$+660    00722100     50                    PUSH EAX
$+661    00722101     B8 7C1B0000           MOV EAX,0x1B7C
$+666    00722106     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
$+66D    0072210D     E8 6EB25500           CALL PathOfEx.00C7D380
$+672    00722112     53                    PUSH EBX
$+673    00722113     56                    PUSH ESI
$+674    00722114     57                    PUSH EDI
$+675    00722115     FF75 50               PUSH DWORD PTR SS:[EBP+0x50]
$+678    00722118     8BD9                  MOV EBX,ECX
$+67A    0072211A     33F6                  XOR ESI,ESI
$+67C    0072211C     895D C8               MOV DWORD PTR SS:[EBP-0x38],EBX
$+67F    0072211F     8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
$+682    00722122     8975 E8               MOV DWORD PTR SS:[EBP-0x18],ESI
$+685    00722125     E8 265E0000           CALL PathOfEx.00727F50
$+68A    0072212A     8B75 10               MOV ESI,DWORD PTR SS:[EBP+0x10]
$+68D    0072212D     85F6                  TEST ESI,ESI
$+68F    0072212F     74 09                 JE SHORT PathOfEx.0072213A
$+691    00722131     8B46 5C               MOV EAX,DWORD PTR DS:[ESI+0x5C]
$+694    00722134     8983 F4010000         MOV DWORD PTR DS:[EBX+0x1F4],EAX
$+69A    0072213A     8B55 08               MOV EDX,DWORD PTR SS:[EBP+0x8]
$+69D    0072213D     85D2                  TEST EDX,EDX
$+69F    0072213F     74 2A                 JE SHORT PathOfEx.0072216B
$+6A1    00722141     66:833A 00            CMP WORD PTR DS:[EDX],0x0
$+6A5    00722145     74 24                 JE SHORT PathOfEx.0072216B
$+6A7    00722147     8BCA                  MOV ECX,EDX
$+6A9    00722149     8D79 02               LEA EDI,DWORD PTR DS:[ECX+0x2]
$+6AC    0072214C     0F1F40 00             NOP DWORD PTR DS:[EAX]
$+6B0    00722150     66:8B01               MOV AX,WORD PTR DS:[ECX]
$+6B3    00722153     83C1 02               ADD ECX,0x2
$+6B6    00722156     66:85C0               TEST AX,AX
$+6B9    00722159   ^ 75 F5                 JNZ SHORT PathOfEx.00722150
$+6BB    0072215B     2BCF                  SUB ECX,EDI
$+6BD    0072215D     D1F9                  SAR ECX,1
$+6BF    0072215F     51                    PUSH ECX
$+6C0    00722160     52                    PUSH EDX
$+6C1    00722161     8D4B 18               LEA ECX,DWORD PTR DS:[EBX+0x18]
$+6C4    00722164     E8 47BBD4FF           CALL PathOfEx.0046DCB0
$+6C9    00722169     EB 1B                 JMP SHORT PathOfEx.00722186
$+6CB    0072216B     85F6                  TEST ESI,ESI
$+6CD    0072216D     74 17                 JE SHORT PathOfEx.00722186
$+6CF    0072216F     8B46 08               MOV EAX,DWORD PTR DS:[ESI+0x8]
$+6D2    00722172     8D4B 18               LEA ECX,DWORD PTR DS:[EBX+0x18]
$+6D5    00722175     83C0 10               ADD EAX,0x10
$+6D8    00722178     3BC8                  CMP ECX,EAX
$+6DA    0072217A     74 0A                 JE SHORT PathOfEx.00722186
$+6DC    0072217C     6A FF                 PUSH -0x1
$+6DE    0072217E     6A 00                 PUSH 0x0
$+6E0    00722180     50                    PUSH EAX
$+6E1    00722181     E8 FAB6D4FF           CALL PathOfEx.0046D880
$+6E6    00722186     8B7D 28               MOV EDI,DWORD PTR SS:[EBP+0x28]
$+6E9    00722189     B8 05000000           MOV EAX,0x5
$+6EE    0072218E     85FF                  TEST EDI,EDI
$+6F0    00722190     B9 04000000           MOV ECX,0x4
$+6F5    00722195     0F45C1                CMOVNE EAX,ECX
$+6F8    00722198     8943 30               MOV DWORD PTR DS:[EBX+0x30],EAX
$+6FB    0072219B     85F6                  TEST ESI,ESI
$+6FD    0072219D     74 5A                 JE SHORT PathOfEx.007221F9
$+6FF    0072219F     8B46 08               MOV EAX,DWORD PTR DS:[ESI+0x8]
$+702    007221A2     8D8D A4FEFFFF         LEA ECX,DWORD PTR SS:[EBP-0x15C]
$+708    007221A8     51                    PUSH ECX
$+709    007221A9     8B48 4C               MOV ECX,DWORD PTR DS:[EAX+0x4C]
$+70C    007221AC     E8 8F5EE4FF           CALL PathOfEx.00568040
$+711    007221B1     50                    PUSH EAX
$+712    007221B2     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
$+719    007221B9     E8 32C50600           CALL PathOfEx.0078E6F0
$+71E    007221BE     83C4 04               ADD ESP,0x4
$+721    007221C1     8945 08               MOV DWORD PTR SS:[EBP+0x8],EAX
$+724    007221C4     C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
$+72B    007221CB     8B8D A4FEFFFF         MOV ECX,DWORD PTR SS:[EBP-0x15C]
$+731    007221D1     85C9                  TEST ECX,ECX
$+733    007221D3     74 1B                 JE SHORT PathOfEx.007221F0
$+735    007221D5     51                    PUSH ECX
$+736    007221D6     51                    PUSH ECX
$+737    007221D7     E8 245EFFFF           CALL PathOfEx.00718000
$+73C    007221DC     83C4 04               ADD ESP,0x4
$+73F    007221DF     8BC8                  MOV ECX,EAX
$+741    007221E1     E8 AA59FFFF           CALL PathOfEx.00717B90
$+746    007221E6     C785 A4FEFFFF 0000000>MOV DWORD PTR SS:[EBP-0x15C],0x0
$+750    007221F0     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
$+757    007221F7     EB 07                 JMP SHORT PathOfEx.00722200
$+759    007221F9     C745 08 30000000      MOV DWORD PTR SS:[EBP+0x8],0x30
$+760    00722200     8B55 0C               MOV EDX,DWORD PTR SS:[EBP+0xC]
$+763    00722203     85D2                  TEST EDX,EDX
$+765    00722205     0F84 B6000000         JE PathOfEx.007222C1
$+76B    0072220B     8B42 40               MOV EAX,DWORD PTR DS:[EDX+0x40]
$+76E    0072220E     85C0                  TEST EAX,EAX
$+770    00722210     75 05                 JNZ SHORT PathOfEx.00722217
$+772    00722212     8943 30               MOV DWORD PTR DS:[EBX+0x30],EAX
$+775    00722215     EB 1C                 JMP SHORT PathOfEx.00722233
$+777    00722217     83F8 01               CMP EAX,0x1
$+77A    0072221A     75 05                 JNZ SHORT PathOfEx.00722221
$+77C    0072221C     8943 30               MOV DWORD PTR DS:[EBX+0x30],EAX
$+77F    0072221F     EB 12                 JMP SHORT PathOfEx.00722233
$+781    00722221     83F8 02               CMP EAX,0x2
$+784    00722224     75 05                 JNZ SHORT PathOfEx.0072222B
$+786    00722226     8943 30               MOV DWORD PTR DS:[EBX+0x30],EAX
$+789    00722229     EB 08                 JMP SHORT PathOfEx.00722233
$+78B    0072222B     83F8 03               CMP EAX,0x3
$+78E    0072222E     75 03                 JNZ SHORT PathOfEx.00722233
$+790    00722230     8943 30               MOV DWORD PTR DS:[EBX+0x30],EAX
$+793    00722233     8A45 64               MOV AL,BYTE PTR SS:[EBP+0x64]
$+796    00722236     8883 ED010000         MOV BYTE PTR DS:[EBX+0x1ED],AL
$+79C    0072223C     8A45 68               MOV AL,BYTE PTR SS:[EBP+0x68]
$+79F    0072223F     8883 F0010000         MOV BYTE PTR DS:[EBX+0x1F0],AL
$+7A5    00722245     8A4A 3C               MOV CL,BYTE PTR DS:[EDX+0x3C]                    ; edx=stCD_ModsInfo*
$+7A8    00722248     84C9                  TEST CL,CL                                       ; edx+3c=stCD_EntityAttrMods.is_identified_
$+7AA    0072224A     0F94C0                SETE AL
$+7AD    0072224D     8883 EC010000         MOV BYTE PTR DS:[EBX+0x1EC],AL                   ; 置1，表示显示 未鉴定
$+7B3    00722253     84C9                  TEST CL,CL
$+7B5    00722255     74 5C                 JE SHORT PathOfEx.007222B3
$+7B7    00722257     8B02                  MOV EAX,DWORD PTR DS:[EDX]
$+7B9    00722259     3B42 04               CMP EAX,DWORD PTR DS:[EDX+0x4]
$+7BC    0072225C     74 24                 JE SHORT PathOfEx.00722282
$+7BE    0072225E     8D85 1CF5FFFF         LEA EAX,DWORD PTR SS:[EBP-0xAE4]
$+7C4    00722264     8BCA                  MOV ECX,EDX
$+7C6    00722266     50                    PUSH EAX
$+7C7    00722267     E8 94CB0200           CALL PathOfEx.0074EE00
$+7CC    0072226C     50                    PUSH EAX
$+7CD    0072226D     8BCB                  MOV ECX,EBX
$+7CF    0072226F     E8 1C51D6FF           CALL PathOfEx.00487390
$+7D4    00722274     8D8D 1CF5FFFF         LEA ECX,DWORD PTR SS:[EBP-0xAE4]
$+7DA    0072227A     E8 C1B4D4FF           CALL PathOfEx.0046D740
$+7DF    0072227F     8B55 0C               MOV EDX,DWORD PTR SS:[EBP+0xC]
$+7E2    00722282     837B 28 00            CMP DWORD PTR DS:[EBX+0x28],0x0
$+7E6    00722286     74 2B                 JE SHORT PathOfEx.007222B3
$+7E8    00722288     837A 40 01            CMP DWORD PTR DS:[EDX+0x40],0x1
$+7EC    0072228C     75 25                 JNZ SHORT PathOfEx.007222B3
$+7EE    0072228E     8D73 18               LEA ESI,DWORD PTR DS:[EBX+0x18]
$+7F1    00722291     8BCA                  MOV ECX,EDX
$+7F3    00722293     56                    PUSH ESI
$+7F4    00722294     8D85 9CF9FFFF         LEA EAX,DWORD PTR SS:[EBP-0x664]
$+7FA    0072229A     50                    PUSH EAX
$+7FB    0072229B     E8 90C10200           CALL PathOfEx.0074E430
$+800    007222A0     50                    PUSH EAX
$+801    007222A1     8BCE                  MOV ECX,ESI
$+803    007222A3     E8 E850D6FF           CALL PathOfEx.00487390
$+808    007222A8     8D8D 9CF9FFFF         LEA ECX,DWORD PTR SS:[EBP-0x664]
$+80E    007222AE     E8 8DB4D4FF           CALL PathOfEx.0046D740
$+813    007222B3     8B45 60               MOV EAX,DWORD PTR SS:[EBP+0x60]
$+816    007222B6     8983 E4010000         MOV DWORD PTR DS:[EBX+0x1E4],EAX
$+81C    007222BC     E9 F2000000           JMP PathOfEx.007223B3
$+821    007222C1     8B75 4C               MOV ESI,DWORD PTR SS:[EBP+0x4C]
$+824    007222C4     85F6                  TEST ESI,ESI
$+826    007222C6     0F84 E7000000         JE PathOfEx.007223B3
$+82C    007222CC     8D85 BCFEFFFF         LEA EAX,DWORD PTR SS:[EBP-0x144]
$+832    007222D2     8BCE                  MOV ECX,ESI
$+834    007222D4     50                    PUSH EAX
$+835    007222D5     E8 568AD8FF           CALL PathOfEx.004AAD30
$+83A    007222DA     C745 FC 02000000      MOV DWORD PTR SS:[EBP-0x4],0x2
$+841    007222E1     8378 04 00            CMP DWORD PTR DS:[EAX+0x4],0x0
$+845    007222E5     74 2D                 JE SHORT PathOfEx.00722314
$+847    007222E7     8D85 9CFEFFFF         LEA EAX,DWORD PTR SS:[EBP-0x164]
$+84D    007222ED     8BCE                  MOV ECX,ESI
$+84F    007222EF     50                    PUSH EAX
$+850    007222F0     E8 3B8AD8FF           CALL PathOfEx.004AAD30
$+855    007222F5     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
$+859    007222F9     BE 01000000           MOV ESI,0x1
$+85E    007222FE     8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
$+861    00722301     8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
$+864    00722304     8975 E8               MOV DWORD PTR SS:[EBP-0x18],ESI
$+867    00722307     8B50 0C               MOV EDX,DWORD PTR DS:[EAX+0xC]
$+86A    0072230A     66:833A 00            CMP WORD PTR DS:[EDX],0x0
$+86E    0072230E     75 09                 JNZ SHORT PathOfEx.00722319
$+870    00722310     33C9                  XOR ECX,ECX
$+872    00722312     EB 1B                 JMP SHORT PathOfEx.0072232F
$+874    00722314     BA 6050E700           MOV EDX,PathOfEx.00E75060                        ; UNICODE "Invalid Prophecy"
$+879    00722319     8BCA                  MOV ECX,EDX
$+87B    0072231B     8D71 02               LEA ESI,DWORD PTR DS:[ECX+0x2]
$+87E    0072231E     66:90                 NOP
$+880    00722320     66:8B01               MOV AX,WORD PTR DS:[ECX]
$+883    00722323     83C1 02               ADD ECX,0x2
$+886    00722326     66:85C0               TEST AX,AX
$+889    00722329   ^ 75 F5                 JNZ SHORT PathOfEx.00722320
$+88B    0072232B     2BCE                  SUB ECX,ESI
$+88D    0072232D     D1F9                  SAR ECX,1
$+88F    0072232F     51                    PUSH ECX
$+890    00722330     52                    PUSH EDX
$+891    00722331     8D4B 18               LEA ECX,DWORD PTR DS:[EBX+0x18]
$+894    00722334     E8 77B9D4FF           CALL PathOfEx.0046DCB0
$+899    00722339     C745 FC 02000000      MOV DWORD PTR SS:[EBP-0x4],0x2
$+8A0    00722340     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
$+8A3    00722343     A8 01                 TEST AL,0x1
$+8A5    00722345     74 32                 JE SHORT PathOfEx.00722379
$+8A7    00722347     83E0 FE               AND EAX,0xFFFFFFFE
$+8AA    0072234A     8945 EC               MOV DWORD PTR SS:[EBP-0x14],EAX
$+8AD    0072234D     8945 E8               MOV DWORD PTR SS:[EBP-0x18],EAX
$+8B0    00722350     C645 FC 04            MOV BYTE PTR SS:[EBP-0x4],0x4
$+8B4    00722354     8B85 9CFEFFFF         MOV EAX,DWORD PTR SS:[EBP-0x164]
$+8BA    0072235A     85C0                  TEST EAX,EAX
$+8BC    0072235C     74 1B                 JE SHORT PathOfEx.00722379
$+8BE    0072235E     50                    PUSH EAX
$+8BF    0072235F     51                    PUSH ECX
$+8C0    00722360     E8 9B5CFFFF           CALL PathOfEx.00718000
*/
/**/
//tag_根据stCD_ItemTipsParams来显示tips
/*
$-1      004D3F3F     CC                       INT3
$ ==>    004D3F40     55                       PUSH EBP                                         ; tag_根据stCD_ItemTipsParams来显示tips
$+1      004D3F41     8D6C24 94                LEA EBP,DWORD PTR SS:[ESP-0x6C]
$+5      004D3F45     83EC 6C                  SUB ESP,0x6C
$+8      004D3F48     6A FF                    PUSH -0x1
$+A      004D3F4A     68 456BD200              PUSH PathOfEx.00D26B45
$+F      004D3F4F     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+15     004D3F55     50                       PUSH EAX
$+16     004D3F56     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+1D     004D3F5D     81EC 78040000            SUB ESP,0x478
$+23     004D3F63     53                       PUSH EBX
$+24     004D3F64     56                       PUSH ESI
$+25     004D3F65     57                       PUSH EDI
$+26     004D3F66     894D 58                  MOV DWORD PTR SS:[EBP+0x58],ECX
$+29     004D3F69     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+2E     004D3F6E     68 947DE700              PUSH PathOfEx.00E77D94                           ; UNICODE "LargeNormalSC"
$+33     004D3F73     C745 00 00000000         MOV DWORD PTR SS:[EBP],0x0
$+3A     004D3F7A     E8 A107FEFF              CALL PathOfEx.004B4720
$+3F     004D3F7F     68 947DE700              PUSH PathOfEx.00E77D94                           ; UNICODE "LargeNormalSC"
$+44     004D3F84     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+49     004D3F89     8B70 18                  MOV ESI,DWORD PTR DS:[EAX+0x18]
$+4C     004D3F8C     E8 8F07FEFF              CALL PathOfEx.004B4720
$+51     004D3F91     C785 B4FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x14C],0x0
$+5B     004D3F9B     33C9                     XOR ECX,ECX
$+5D     004D3F9D     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+64     004D3FA4     6A FF                    PUSH -0x1
$+66     004D3FA6     51                       PUSH ECX
$+67     004D3FA7     66:898D B8FEFFFF         MOV WORD PTR SS:[EBP-0x148],CX
$+6E     004D3FAE     8D8D B8FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x148]
$+74     004D3FB4     50                       PUSH EAX
$+75     004D3FB5     C785 CCFEFFFF 07000000   MOV DWORD PTR SS:[EBP-0x134],0x7
$+7F     004D3FBF     C785 C8FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x138],0x0
$+89     004D3FC9     E8 B298F9FF              CALL PathOfEx.0046D880
$+8E     004D3FCE     66:0F6EC6                MOVD MM0,ESI
$+92     004D3FD2     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+96     004D3FD6     C1EE 1F                  SHR ESI,0x1F
$+99     004D3FD9     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+9D     004D3FDD     C685 D0FEFFFF 00         MOV BYTE PTR SS:[EBP-0x130],0x0
$+A4     004D3FE4     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+AD     004D3FED     660F5AC0                 CVTPD2PS XMM0,XMM0
$+B1     004D3FF1     E8 BA8D7A00              CALL PathOfEx.00C7CDB0
$+B6     004D3FF6     8985 D4FEFFFF            MOV DWORD PTR SS:[EBP-0x12C],EAX
$+BC     004D3FFC     C785 DCFEFFFF 0000803F   MOV DWORD PTR SS:[EBP-0x124],0x3F800000
$+C6     004D4006     C685 E0FEFFFF 00         MOV BYTE PTR SS:[EBP-0x120],0x0
$+CD     004D400D     66:0F6EC0                MOVD MM0,EAX
$+D1     004D4011     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+D5     004D4015     C1E8 1F                  SHR EAX,0x1F
$+D8     004D4018     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+E1     004D4021     660F5AC0                 CVTPD2PS XMM0,XMM0
$+E5     004D4025     E8 868D7A00              CALL PathOfEx.00C7CDB0
$+EA     004D402A     83F8 0B                  CMP EAX,0xB
$+ED     004D402D     8985 D8FEFFFF            MOV DWORD PTR SS:[EBP-0x128],EAX
$+F3     004D4033     BF 50E2E000              MOV EDI,PathOfEx.00E0E250
$+F8     004D4038     8D95 D8FEFFFF            LEA EDX,DWORD PTR SS:[EBP-0x128]
$+FE     004D403E     8BCF                     MOV ECX,EDI
$+100    004D4040     0F43CA                   CMOVNB ECX,EDX
$+103    004D4043     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+105    004D4045     8D8D B4FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x14C]
$+10B    004D404B     8985 D8FEFFFF            MOV DWORD PTR SS:[EBP-0x128],EAX
$+111    004D4051     E8 3A5E5300              CALL PathOfEx.00A09E90
$+116    004D4056     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+11B    004D405B     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+120    004D4060     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$+127    004D4067     E8 B406FEFF              CALL PathOfEx.004B4720
$+12C    004D406C     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+131    004D4071     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+136    004D4076     8B70 18                  MOV ESI,DWORD PTR DS:[EAX+0x18]
$+139    004D4079     E8 A206FEFF              CALL PathOfEx.004B4720
$+13E    004D407E     C785 74FFFFFF 00000000   MOV DWORD PTR SS:[EBP-0x8C],0x0
$+148    004D4088     33C9                     XOR ECX,ECX
$+14A    004D408A     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
$+14E    004D408E     6A FF                    PUSH -0x1
$+150    004D4090     51                       PUSH ECX
$+151    004D4091     66:898D 78FFFFFF         MOV WORD PTR SS:[EBP-0x88],CX
$+158    004D4098     8D8D 78FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0x88]
$+15E    004D409E     50                       PUSH EAX
$+15F    004D409F     C745 8C 07000000         MOV DWORD PTR SS:[EBP-0x74],0x7
$+166    004D40A6     C745 88 00000000         MOV DWORD PTR SS:[EBP-0x78],0x0
$+16D    004D40AD     E8 CE97F9FF              CALL PathOfEx.0046D880
$+172    004D40B2     66:0F6EC6                MOVD MM0,ESI
$+176    004D40B6     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+17A    004D40BA     C1EE 1F                  SHR ESI,0x1F
$+17D    004D40BD     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
$+181    004D40C1     C645 90 00               MOV BYTE PTR SS:[EBP-0x70],0x0
$+185    004D40C5     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+18E    004D40CE     660F5AC0                 CVTPD2PS XMM0,XMM0
$+192    004D40D2     E8 D98C7A00              CALL PathOfEx.00C7CDB0
$+197    004D40D7     8945 94                  MOV DWORD PTR SS:[EBP-0x6C],EAX
$+19A    004D40DA     C745 9C 0000803F         MOV DWORD PTR SS:[EBP-0x64],0x3F800000
$+1A1    004D40E1     C645 A0 00               MOV BYTE PTR SS:[EBP-0x60],0x0
$+1A5    004D40E5     66:0F6EC0                MOVD MM0,EAX
$+1A9    004D40E9     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+1AD    004D40ED     C1E8 1F                  SHR EAX,0x1F
$+1B0    004D40F0     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+1B9    004D40F9     660F5AC0                 CVTPD2PS XMM0,XMM0
$+1BD    004D40FD     E8 AE8C7A00              CALL PathOfEx.00C7CDB0
$+1C2    004D4102     83F8 0B                  CMP EAX,0xB
$+1C5    004D4105     8945 98                  MOV DWORD PTR SS:[EBP-0x68],EAX
$+1C8    004D4108     8D55 98                  LEA EDX,DWORD PTR SS:[EBP-0x68]
$+1CB    004D410B     8BCF                     MOV ECX,EDI
$+1CD    004D410D     0F43CA                   CMOVNB ECX,EDX
$+1D0    004D4110     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+1D2    004D4112     8D8D 74FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0x8C]
$+1D8    004D4118     8945 98                  MOV DWORD PTR SS:[EBP-0x68],EAX
$+1DB    004D411B     E8 705D5300              CALL PathOfEx.00A09E90
$+1E0    004D4120     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+1E5    004D4125     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+1EA    004D412A     E8 F105FEFF              CALL PathOfEx.004B4720
$+1EF    004D412F     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+1F4    004D4134     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+1F9    004D4139     8B70 18                  MOV ESI,DWORD PTR DS:[EAX+0x18]
$+1FC    004D413C     E8 DF05FEFF              CALL PathOfEx.004B4720
$+201    004D4141     C785 E4FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x11C],0x0
$+20B    004D414B     33C9                     XOR ECX,ECX
$+20D    004D414D     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
$+211    004D4151     6A FF                    PUSH -0x1
$+213    004D4153     51                       PUSH ECX
$+214    004D4154     66:898D E8FEFFFF         MOV WORD PTR SS:[EBP-0x118],CX
$+21B    004D415B     8D8D E8FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x118]
$+221    004D4161     50                       PUSH EAX
$+222    004D4162     C785 FCFEFFFF 07000000   MOV DWORD PTR SS:[EBP-0x104],0x7
$+22C    004D416C     C785 F8FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x108],0x0
$+236    004D4176     E8 0597F9FF              CALL PathOfEx.0046D880
$+23B    004D417B     66:0F6EC6                MOVD MM0,ESI
$+23F    004D417F     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+243    004D4183     C1EE 1F                  SHR ESI,0x1F
$+246    004D4186     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
$+24A    004D418A     C685 00FFFFFF 04         MOV BYTE PTR SS:[EBP-0x100],0x4
$+251    004D4191     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+25A    004D419A     660F5AC0                 CVTPD2PS XMM0,XMM0
$+25E    004D419E     E8 0D8C7A00              CALL PathOfEx.00C7CDB0
$+263    004D41A3     8985 04FFFFFF            MOV DWORD PTR SS:[EBP-0xFC],EAX
$+269    004D41A9     C785 0CFFFFFF 0000803F   MOV DWORD PTR SS:[EBP-0xF4],0x3F800000
$+273    004D41B3     C685 10FFFFFF 00         MOV BYTE PTR SS:[EBP-0xF0],0x0
$+27A    004D41BA     66:0F6EC0                MOVD MM0,EAX
$+27E    004D41BE     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+282    004D41C2     C1E8 1F                  SHR EAX,0x1F
$+285    004D41C5     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+28E    004D41CE     660F5AC0                 CVTPD2PS XMM0,XMM0
$+292    004D41D2     E8 D98B7A00              CALL PathOfEx.00C7CDB0
$+297    004D41D7     83F8 0B                  CMP EAX,0xB
$+29A    004D41DA     8985 08FFFFFF            MOV DWORD PTR SS:[EBP-0xF8],EAX
$+2A0    004D41E0     8D95 08FFFFFF            LEA EDX,DWORD PTR SS:[EBP-0xF8]
$+2A6    004D41E6     8BCF                     MOV ECX,EDI
$+2A8    004D41E8     0F43CA                   CMOVNB ECX,EDX
$+2AB    004D41EB     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+2AD    004D41ED     8D8D E4FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x11C]
$+2B3    004D41F3     8985 08FFFFFF            MOV DWORD PTR SS:[EBP-0xF8],EAX
$+2B9    004D41F9     E8 925C5300              CALL PathOfEx.00A09E90
$+2BE    004D41FE     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+2C3    004D4203     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+2C8    004D4208     E8 1305FEFF              CALL PathOfEx.004B4720
$+2CD    004D420D     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+2D2    004D4212     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+2D7    004D4217     8B70 18                  MOV ESI,DWORD PTR DS:[EAX+0x18]
$+2DA    004D421A     E8 0105FEFF              CALL PathOfEx.004B4720
$+2DF    004D421F     C785 44FFFFFF 00000000   MOV DWORD PTR SS:[EBP-0xBC],0x0
$+2E9    004D4229     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
$+2ED    004D422D     33C9                     XOR ECX,ECX
$+2EF    004D422F     C785 5CFFFFFF 07000000   MOV DWORD PTR SS:[EBP-0xA4],0x7
$+2F9    004D4239     C785 58FFFFFF 00000000   MOV DWORD PTR SS:[EBP-0xA8],0x0
$+303    004D4243     66:898D 48FFFFFF         MOV WORD PTR SS:[EBP-0xB8],CX
$+30A    004D424A     6A FF                    PUSH -0x1
$+30C    004D424C     51                       PUSH ECX
$+30D    004D424D     50                       PUSH EAX
$+30E    004D424E     8D8D 48FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xB8]
$+314    004D4254     E8 2796F9FF              CALL PathOfEx.0046D880
$+319    004D4259     66:0F6EC6                MOVD MM0,ESI
$+31D    004D425D     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+321    004D4261     C1EE 1F                  SHR ESI,0x1F
$+324    004D4264     C645 FC 0A               MOV BYTE PTR SS:[EBP-0x4],0xA
$+328    004D4268     C685 60FFFFFF 04         MOV BYTE PTR SS:[EBP-0xA0],0x4
$+32F    004D426F     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+338    004D4278     660F5AC0                 CVTPD2PS XMM0,XMM0
$+33C    004D427C     E8 2F8B7A00              CALL PathOfEx.00C7CDB0
$+341    004D4281     8985 64FFFFFF            MOV DWORD PTR SS:[EBP-0x9C],EAX
$+347    004D4287     C785 6CFFFFFF 0000803F   MOV DWORD PTR SS:[EBP-0x94],0x3F800000
$+351    004D4291     C685 70FFFFFF 00         MOV BYTE PTR SS:[EBP-0x90],0x0
$+358    004D4298     66:0F6EC0                MOVD MM0,EAX
$+35C    004D429C     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+360    004D42A0     C1E8 1F                  SHR EAX,0x1F
$+363    004D42A3     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+36C    004D42AC     660F5AC0                 CVTPD2PS XMM0,XMM0
$+370    004D42B0     E8 FB8A7A00              CALL PathOfEx.00C7CDB0
$+375    004D42B5     83F8 0B                  CMP EAX,0xB
$+378    004D42B8     8985 68FFFFFF            MOV DWORD PTR SS:[EBP-0x98],EAX
$+37E    004D42BE     8D95 68FFFFFF            LEA EDX,DWORD PTR SS:[EBP-0x98]
$+384    004D42C4     8BCF                     MOV ECX,EDI
$+386    004D42C6     0F43CA                   CMOVNB ECX,EDX
$+389    004D42C9     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+38B    004D42CB     8D8D 44FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xBC]
$+391    004D42D1     8985 68FFFFFF            MOV DWORD PTR SS:[EBP-0x98],EAX
$+397    004D42D7     E8 B45B5300              CALL PathOfEx.00A09E90
$+39C    004D42DC     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+3A1    004D42E1     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+3A6    004D42E6     E8 3504FEFF              CALL PathOfEx.004B4720
$+3AB    004D42EB     68 FC70E700              PUSH PathOfEx.00E770FC                           ; UNICODE "NormalSC"
$+3B0    004D42F0     B9 048F1301              MOV ECX,PathOfEx.01138F04
$+3B5    004D42F5     8B70 18                  MOV ESI,DWORD PTR DS:[EAX+0x18]
$+3B8    004D42F8     E8 2304FEFF              CALL PathOfEx.004B4720
$+3BD    004D42FD     C785 14FFFFFF 00000000   MOV DWORD PTR SS:[EBP-0xEC],0x0
$+3C7    004D4307     33C9                     XOR ECX,ECX
$+3C9    004D4309     C645 FC 0C               MOV BYTE PTR SS:[EBP-0x4],0xC
$+3CD    004D430D     6A FF                    PUSH -0x1
$+3CF    004D430F     51                       PUSH ECX
$+3D0    004D4310     66:898D 18FFFFFF         MOV WORD PTR SS:[EBP-0xE8],CX
$+3D7    004D4317     8D8D 18FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xE8]
$+3DD    004D431D     50                       PUSH EAX
$+3DE    004D431E     C785 2CFFFFFF 07000000   MOV DWORD PTR SS:[EBP-0xD4],0x7
$+3E8    004D4328     C785 28FFFFFF 00000000   MOV DWORD PTR SS:[EBP-0xD8],0x0
$+3F2    004D4332     E8 4995F9FF              CALL PathOfEx.0046D880
$+3F7    004D4337     66:0F6EC6                MOVD MM0,ESI
$+3FB    004D433B     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+3FF    004D433F     C1EE 1F                  SHR ESI,0x1F
$+402    004D4342     C645 FC 0D               MOV BYTE PTR SS:[EBP-0x4],0xD
$+406    004D4346     C685 30FFFFFF 00         MOV BYTE PTR SS:[EBP-0xD0],0x0
$+40D    004D434D     F20F5804F5 00FDF300      ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FD00]
$+416    004D4356     660F5AC0                 CVTPD2PS XMM0,XMM0
$+41A    004D435A     E8 518A7A00              CALL PathOfEx.00C7CDB0
$+41F    004D435F     8985 34FFFFFF            MOV DWORD PTR SS:[EBP-0xCC],EAX
$+425    004D4365     C785 3CFFFFFF 0000803F   MOV DWORD PTR SS:[EBP-0xC4],0x3F800000
$+42F    004D436F     C685 40FFFFFF 00         MOV BYTE PTR SS:[EBP-0xC0],0x0
$+436    004D4376     66:0F6EC0                MOVD MM0,EAX
$+43A    004D437A     F30FE6C0                 CVTDQ2PD XMM0,XMM0
$+43E    004D437E     C1E8 1F                  SHR EAX,0x1F
$+441    004D4381     F20F5804C5 00FDF300      ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$+44A    004D438A     660F5AC0                 CVTPD2PS XMM0,XMM0
$+44E    004D438E     E8 1D8A7A00              CALL PathOfEx.00C7CDB0
$+453    004D4393     83F8 0B                  CMP EAX,0xB
$+456    004D4396     8985 38FFFFFF            MOV DWORD PTR SS:[EBP-0xC8],EAX
$+45C    004D439C     8D8D 38FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xC8]
$+462    004D43A2     0F43F9                   CMOVNB EDI,ECX
$+465    004D43A5     8D8D 14FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xEC]
$+46B    004D43AB     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+46D    004D43AD     8985 38FFFFFF            MOV DWORD PTR SS:[EBP-0xC8],EAX
$+473    004D43B3     E8 D85A5300              CALL PathOfEx.00A09E90
$+478    004D43B8     C785 8CFEFFFF CC5FF300   MOV DWORD PTR SS:[EBP-0x174],PathOfEx.00F35FCC
$+482    004D43C2     8D85 8CFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x174]
$+488    004D43C8     8985 B0FEFFFF            MOV DWORD PTR SS:[EBP-0x150],EAX
$+48E    004D43CE     8B7D 58                  MOV EDI,DWORD PTR SS:[EBP+0x58]
$+491    004D43D1     51                       PUSH ECX
$+492    004D43D2     50                       PUSH EAX
$+493    004D43D3     C645 FC 0F               MOV BYTE PTR SS:[EBP-0x4],0xF
$+497    004D43D7     8D8F 04070000            LEA ECX,DWORD PTR DS:[EDI+0x704]
$+49D    004D43DD     E8 DEF65200              CALL PathOfEx.00A03AC0
$+4A2    004D43E2     C645 FC 10               MOV BYTE PTR SS:[EBP-0x4],0x10
$+4A6    004D43E6     8B8D B0FEFFFF            MOV ECX,DWORD PTR SS:[EBP-0x150]
$+4AC    004D43EC     85C9                     TEST ECX,ECX
$+4AE    004D43EE     74 14                    JE SHORT PathOfEx.004D4404
$+4B0    004D43F0     8B11                     MOV EDX,DWORD PTR DS:[ECX]
$+4B2    004D43F2     8D85 8CFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x174]
$+4B8    004D43F8     3BC8                     CMP ECX,EAX
$+4BA    004D43FA     0F95C0                   SETNE AL
$+4BD    004D43FD     0FB6C0                   MOVZX EAX,AL
$+4C0    004D4400     50                       PUSH EAX
$+4C1    004D4401     FF52 10                  CALL DWORD PTR DS:[EDX+0x10]
$+4C4    004D4404     C645 FC 0E               MOV BYTE PTR SS:[EBP-0x4],0xE
$+4C8    004D4408     8B5D 74                  MOV EBX,DWORD PTR SS:[EBP+0x74]                  ; 执行后ebx=stCD_ItemTipsParams*
$+4CB    004D440B     8B53 30                  MOV EDX,DWORD PTR DS:[EBX+0x30]
$+4CE    004D440E     83FA 02                  CMP EDX,0x2
$+4D1    004D4411     74 05                    JE SHORT PathOfEx.004D4418
$+4D3    004D4413     83FA 03                  CMP EDX,0x3
$+4D6    004D4416     75 0A                    JNZ SHORT PathOfEx.004D4422
$+4D8    004D4418     837B 10 00               CMP DWORD PTR DS:[EBX+0x10],0x0
$+4DC    004D441C     C645 68 01               MOV BYTE PTR SS:[EBP+0x68],0x1
$+4E0    004D4420     74 04                    JE SHORT PathOfEx.004D4426
$+4E2    004D4422     C645 68 00               MOV BYTE PTR SS:[EBP+0x68],0x0
$+4E6    004D4426     51                       PUSH ECX
$+4E7    004D4427     FF75 68                  PUSH DWORD PTR SS:[EBP+0x68]
$+4EA    004D442A     8D4D 48                  LEA ECX,DWORD PTR SS:[EBP+0x48]
$+4ED    004D442D     E8 CE520000              CALL PathOfEx.004D9700
$+4F2    004D4432     83C4 08                  ADD ESP,0x8
$+4F5    004D4435     F3:0F1040 04             MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
$+4FA    004D443A     8B48 08                  MOV ECX,DWORD PTR DS:[EAX+0x8]
$+4FD    004D443D     F3:0F1145 1C             MOVSS DWORD PTR SS:[EBP+0x1C],XMM0
$+502    004D4442     F3:0F1000                MOVSS XMM0,DWORD PTR DS:[EAX]
$+506    004D4446     8B87 0C070000            MOV EAX,DWORD PTR DS:[EDI+0x70C]
$+50C    004D444C     2B87 08070000            SUB EAX,DWORD PTR DS:[EDI+0x708]
$+512    004D4452     51                       PUSH ECX
$+513    004D4453     C1F8 02                  SAR EAX,0x2
*/
/**/
//call tag_添加一行物品tips字符串
/*
$-20E    004D5AAD     85C9                      TEST ECX,ECX
$-20C    004D5AAF     74 09                     JE SHORT PathOfEx.004D5ABA
$-20A    004D5AB1     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-208    004D5AB3     56                        PUSH ESI
$-207    004D5AB4     FF90 CC000000             CALL DWORD PTR DS:[EAX+0xCC]
$-201    004D5ABA     8B8F 10090000             MOV ECX,DWORD PTR DS:[EDI+0x910]
$-1FB    004D5AC0     81C1 B0080000             ADD ECX,0x8B0
$-1F5    004D5AC6     6A 01                     PUSH 0x1
$-1F3    004D5AC8     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-1F1    004D5ACA     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
$-1EE    004D5ACD     8B8F 10090000             MOV ECX,DWORD PTR DS:[EDI+0x910]
$-1E8    004D5AD3     83B9 68070000 05          CMP DWORD PTR DS:[ECX+0x768],0x5
$-1E1    004D5ADA     F3:0F1091 6C070000        MOVSS XMM2,DWORD PTR DS:[ECX+0x76C]
$-1D9    004D5AE2     75 09                     JNZ SHORT PathOfEx.004D5AED
$-1D7    004D5AE4     0F2ED2                    UCOMISS XMM2,XMM2
$-1D4    004D5AE7     9F                        LAHF
$-1D3    004D5AE8     F6C4 44                   TEST AH,0x44
$-1D0    004D5AEB     7B 11                     JPO SHORT PathOfEx.004D5AFE
$-1CE    004D5AED     6A 05                     PUSH 0x5
$-1CC    004D5AEF     C781 68070000 05000000    MOV DWORD PTR DS:[ECX+0x768],0x5
$-1C2    004D5AF9     E8 A2AB5100               CALL PathOfEx.009F06A0
$-1BD    004D5AFE     803D D6CDFF00 00          CMP BYTE PTR DS:[0xFFCDD6],0x0
$-1B6    004D5B05     75 05                     JNZ SHORT PathOfEx.004D5B0C
$-1B4    004D5B07     E8 B4B25300               CALL PathOfEx.00A10DC0
$-1AF    004D5B0C     BA 12000000               MOV EDX,0x12
$-1AA    004D5B11     B9 24ADE700               MOV ECX,PathOfEx.00E7AD24                       ; augmented
$-1A5    004D5B16     E8 45442300               CALL PathOfEx.00709F60
$-1A0    004D5B1B     8945 68                   MOV DWORD PTR SS:[EBP+0x68],EAX
$-19D    004D5B1E     8D45 68                   LEA EAX,DWORD PTR SS:[EBP+0x68]
$-19A    004D5B21     50                        PUSH EAX
$-199    004D5B22     8D45 64                   LEA EAX,DWORD PTR SS:[EBP+0x64]
$-196    004D5B25     50                        PUSH EAX
$-195    004D5B26     E8 65C05300               CALL PathOfEx.00A11B90
$-190    004D5B2B     8B45 64                   MOV EAX,DWORD PTR SS:[EBP+0x64]
$-18D    004D5B2E     3B05 60D21301             CMP EAX,DWORD PTR DS:[0x113D260]
$-187    004D5B34     75 04                     JNZ SHORT PathOfEx.004D5B3A
$-185    004D5B36     33D2                      XOR EDX,EDX
$-183    004D5B38     EB 03                     JMP SHORT PathOfEx.004D5B3D
$-181    004D5B3A     8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
$-17E    004D5B3D     8B8F 10090000             MOV ECX,DWORD PTR DS:[EDI+0x910]
$-178    004D5B43     81C1 40080000             ADD ECX,0x840
$-172    004D5B49     52                        PUSH EDX
$-171    004D5B4A     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-16F    004D5B4C     FF50 0C                   CALL DWORD PTR DS:[EAX+0xC]
$-16C    004D5B4F     833D 80DE0001 08          CMP DWORD PTR DS:[0x100DE80],0x8
$-165    004D5B56     B8 6CDE0001               MOV EAX,PathOfEx.0100DE6C
$-160    004D5B5B     C705 7CDE0001 00000000    MOV DWORD PTR DS:[0x100DE7C],0x0
$-156    004D5B65     0F4305 6CDE0001           CMOVNB EAX,DWORD PTR DS:[0x100DE6C]
$-14F    004D5B6C     33C9                      XOR ECX,ECX
$-14D    004D5B6E     66:8908                   MOV WORD PTR DS:[EAX],CX
$-14A    004D5B71     8B83 94000000             MOV EAX,DWORD PTR DS:[EBX+0x94]
$-144    004D5B77     3B83 98000000             CMP EAX,DWORD PTR DS:[EBX+0x98]
$-13E    004D5B7D     74 7A                     JE SHORT PathOfEx.004D5BF9
$-13C    004D5B7F     390D 7CDE0001             CMP DWORD PTR DS:[0x100DE7C],ECX
$-136    004D5B85     74 11                     JE SHORT PathOfEx.004D5B98
$-134    004D5B87     6A 01                     PUSH 0x1
$-132    004D5B89     68 9483E700               PUSH PathOfEx.00E78394                          ; \n
$-12D    004D5B8E     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-128    004D5B93     E8 7875FAFF               CALL PathOfEx.0047D110
$-123    004D5B98     8B8B 98000000             MOV ECX,DWORD PTR DS:[EBX+0x98]
$-11D    004D5B9E     B8 ABAAAA2A               MOV EAX,0x2AAAAAAB
$-118    004D5BA3     2B8B 94000000             SUB ECX,DWORD PTR DS:[EBX+0x94]
$-112    004D5BA9     BE 00000000               MOV ESI,0x0
$-10D    004D5BAE     F7E9                      IMUL ECX
$-10B    004D5BB0     C1FA 02                   SAR EDX,0x2
$-108    004D5BB3     8BC2                      MOV EAX,EDX
$-106    004D5BB5     C1E8 1F                   SHR EAX,0x1F
$-103    004D5BB8     03C2                      ADD EAX,EDX
$-101    004D5BBA     8945 64                   MOV DWORD PTR SS:[EBP+0x64],EAX
$-FE     004D5BBD     74 3A                     JE SHORT PathOfEx.004D5BF9
$-FC     004D5BBF     8975 68                   MOV DWORD PTR SS:[EBP+0x68],ESI
$-F9     004D5BC2     85F6                      TEST ESI,ESI
$-F7     004D5BC4     74 11                     JE SHORT PathOfEx.004D5BD7
$-F5     004D5BC6     6A 01                     PUSH 0x1
$-F3     004D5BC8     68 9483E700               PUSH PathOfEx.00E78394                          ; \n
$-EE     004D5BCD     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-E9     004D5BD2     E8 3975FAFF               CALL PathOfEx.0047D110
$-E4     004D5BD7     8B83 94000000             MOV EAX,DWORD PTR DS:[EBX+0x94]
$-DE     004D5BDD     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-D9     004D5BE2     0345 68                   ADD EAX,DWORD PTR SS:[EBP+0x68]
$-D6     004D5BE5     6A FF                     PUSH -0x1
$-D4     004D5BE7     6A 00                     PUSH 0x0
$-D2     004D5BE9     50                        PUSH EAX
$-D1     004D5BEA     E8 913DFAFF               CALL PathOfEx.00479980                          ; 添加一行蓝色属性
$-CC     004D5BEF     8345 68 18                ADD DWORD PTR SS:[EBP+0x68],0x18
$-C8     004D5BF3     46                        INC ESI
$-C7     004D5BF4     3B75 64                   CMP ESI,DWORD PTR SS:[EBP+0x64]
$-C4     004D5BF7   ^ 72 C9                     JB SHORT PathOfEx.004D5BC2
$-C2     004D5BF9     8B83 AC000000             MOV EAX,DWORD PTR DS:[EBX+0xAC]
$-BC     004D5BFF     3B83 B0000000             CMP EAX,DWORD PTR DS:[EBX+0xB0]
$-B6     004D5C05     0F84 9D000000             JE PathOfEx.004D5CA8
$-B0     004D5C0B     833D 7CDE0001 00          CMP DWORD PTR DS:[0x100DE7C],0x0
$-A9     004D5C12     74 11                     JE SHORT PathOfEx.004D5C25
$-A7     004D5C14     6A 01                     PUSH 0x1
$-A5     004D5C16     68 9483E700               PUSH PathOfEx.00E78394                          ; \n
$-A0     004D5C1B     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-9B     004D5C20     E8 EB74FAFF               CALL PathOfEx.0047D110
$-96     004D5C25     6A 0A                     PUSH 0xA
$-94     004D5C27     68 A0ADE700               PUSH PathOfEx.00E7ADA0                          ; <crafted>{
$-8F     004D5C2C     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-8A     004D5C31     E8 DA74FAFF               CALL PathOfEx.0047D110
$-85     004D5C36     8B8B B0000000             MOV ECX,DWORD PTR DS:[EBX+0xB0]
$-7F     004D5C3C     B8 ABAAAA2A               MOV EAX,0x2AAAAAAB
$-7A     004D5C41     2B8B AC000000             SUB ECX,DWORD PTR DS:[EBX+0xAC]
$-74     004D5C47     BE 00000000               MOV ESI,0x0
$-6F     004D5C4C     F7E9                      IMUL ECX
$-6D     004D5C4E     C1FA 02                   SAR EDX,0x2
$-6A     004D5C51     8BC2                      MOV EAX,EDX
$-68     004D5C53     C1E8 1F                   SHR EAX,0x1F
$-65     004D5C56     03C2                      ADD EAX,EDX
$-63     004D5C58     8945 64                   MOV DWORD PTR SS:[EBP+0x64],EAX
$-60     004D5C5B     74 3A                     JE SHORT PathOfEx.004D5C97
$-5E     004D5C5D     8975 68                   MOV DWORD PTR SS:[EBP+0x68],ESI
$-5B     004D5C60     85F6                      TEST ESI,ESI
$-59     004D5C62     74 11                     JE SHORT PathOfEx.004D5C75
$-57     004D5C64     6A 01                     PUSH 0x1
$-55     004D5C66     68 9483E700               PUSH PathOfEx.00E78394                          ; \n
$-50     004D5C6B     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-4B     004D5C70     E8 9B74FAFF               CALL PathOfEx.0047D110
$-46     004D5C75     8B83 AC000000             MOV EAX,DWORD PTR DS:[EBX+0xAC]
$-40     004D5C7B     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-3B     004D5C80     0345 68                   ADD EAX,DWORD PTR SS:[EBP+0x68]
$-38     004D5C83     6A FF                     PUSH -0x1
$-36     004D5C85     6A 00                     PUSH 0x0
$-34     004D5C87     50                        PUSH EAX
$-33     004D5C88     E8 F33CFAFF               CALL PathOfEx.00479980
$-2E     004D5C8D     8345 68 18                ADD DWORD PTR SS:[EBP+0x68],0x18
$-2A     004D5C91     46                        INC ESI
$-29     004D5C92     3B75 64                   CMP ESI,DWORD PTR SS:[EBP+0x64]
$-26     004D5C95   ^ 72 C9                     JB SHORT PathOfEx.004D5C60
$-24     004D5C97     6A 01                     PUSH 0x1
$-22     004D5C99     68 CC7AE700               PUSH PathOfEx.00E77ACC                          ; }
$-1D     004D5C9E     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$-18     004D5CA3     E8 6874FAFF               CALL PathOfEx.0047D110
$-13     004D5CA8     8B8F 10090000             MOV ECX,DWORD PTR DS:[EDI+0x910]
$-D      004D5CAE     81C1 B0080000             ADD ECX,0x8B0
$-7      004D5CB4     68 6CDE0001               PUSH PathOfEx.0100DE6C                          ; push const std::wstring*，这是全局缓冲区字符串对象
$-2      004D5CB9     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$ ==>    004D5CBB     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]                     ; call tag_添加一行物品tips字符串
$+3      004D5CBE     8B8F 10090000             MOV ECX,DWORD PTR DS:[EDI+0x910]
$+9      004D5CC4     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+B      004D5CC6     8B40 3C                   MOV EAX,DWORD PTR DS:[EAX+0x3C]
$+E      004D5CC9     FFD0                      CALL EAX
$+10     004D5CCB     D95D 68                   FSTP DWORD PTR SS:[EBP+0x68]
$+13     004D5CCE     F3:0F1045 68              MOVSS XMM0,DWORD PTR SS:[EBP+0x68]
$+18     004D5CD3     8D45 68                   LEA EAX,DWORD PTR SS:[EBP+0x68]
$+1B     004D5CD6     0F2F45 5C                 COMISS XMM0,DWORD PTR SS:[EBP+0x5C]
$+1F     004D5CDA     8D4D 5C                   LEA ECX,DWORD PTR SS:[EBP+0x5C]
$+22     004D5CDD     0F46C1                    CMOVBE EAX,ECX
$+25     004D5CE0     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+29     004D5CE4     8B83 A0000000             MOV EAX,DWORD PTR DS:[EBX+0xA0]
$+2F     004D5CEA     F3:0F1145 5C              MOVSS DWORD PTR SS:[EBP+0x5C],XMM0
$+34     004D5CEF     3B83 A4000000             CMP EAX,DWORD PTR DS:[EBX+0xA4]
$+3A     004D5CF5     75 12                     JNZ SHORT PathOfEx.004D5D09
$+3C     004D5CF7     8B83 B8000000             MOV EAX,DWORD PTR DS:[EBX+0xB8]
$+42     004D5CFD     3B83 BC000000             CMP EAX,DWORD PTR DS:[EBX+0xBC]
$+48     004D5D03     0F84 D6020000             JE PathOfEx.004D5FDF
$+4E     004D5D09     68 8C0B0000               PUSH 0xB8C
$+53     004D5D0E     E8 128D7A00               CALL PathOfEx.00C7EA25
$+58     004D5D13     83C4 04                   ADD ESP,0x4
$+5B     004D5D16     8945 50                   MOV DWORD PTR SS:[EBP+0x50],EAX
$+5E     004D5D19     8BC8                      MOV ECX,EAX
$+60     004D5D1B     C645 FC 2E                MOV BYTE PTR SS:[EBP-0x4],0x2E
$+64     004D5D1F     E8 EC665300               CALL PathOfEx.00A0C410
$+69     004D5D24     C645 FC 0E                MOV BYTE PTR SS:[EBP-0x4],0xE
$+6D     004D5D28     8BD0                      MOV EDX,EAX
$+6F     004D5D2A     8B8F F8080000             MOV ECX,DWORD PTR DS:[EDI+0x8F8]
$+75     004D5D30     81C1 04070000             ADD ECX,0x704
$+7B     004D5D36     8997 14090000             MOV DWORD PTR DS:[EDI+0x914],EDX
$+81     004D5D3C     52                        PUSH EDX
$+82     004D5D3D     8B41 08                   MOV EAX,DWORD PTR DS:[ECX+0x8]
$+85     004D5D40     2B41 04                   SUB EAX,DWORD PTR DS:[ECX+0x4]
$+88     004D5D43     C1F8 02                   SAR EAX,0x2
$+8B     004D5D46     50                        PUSH EAX
$+8C     004D5D47     E8 94FA5200               CALL PathOfEx.00A057E0
$+91     004D5D4C     8B87 14090000             MOV EAX,DWORD PTR DS:[EDI+0x914]
$+97     004D5D52     F3:0F1005 9C26F400        MOVSS XMM0,DWORD PTR DS:[0xF4269C]
$+9F     004D5D5A     C780 800B0000 00000000    MOV DWORD PTR DS:[EAX+0xB80],0x0
$+A9     004D5D64     66:C780 A9080000 0101     MOV WORD PTR DS:[EAX+0x8A9],0x101
$+B2     004D5D6D     8BB7 14090000             MOV ESI,DWORD PTR DS:[EDI+0x914]
$+B8     004D5D73     0F2E86 54080000           UCOMISS XMM0,DWORD PTR DS:[ESI+0x854]
$+BF     004D5D7A     9F                        LAHF
$+C0     004D5D7B     F6C4 44                   TEST AH,0x44
$+C3     004D5D7E     7B 4E                     JPO SHORT PathOfEx.004D5DCE
$+C5     004D5D80     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+C7     004D5D82     8BCE                      MOV ECX,ESI
$+C9     004D5D84     C786 54080000 00809844    MOV DWORD PTR DS:[ESI+0x854],0x44988000
$+D3     004D5D8E     FF90 24010000             CALL DWORD PTR DS:[EAX+0x124]
$+D9     004D5D94     F3:0F108E 54080000        MOVSS XMM1,DWORD PTR DS:[ESI+0x854]
$+E1     004D5D9C     F3:0F1086 44080000        MOVSS XMM0,DWORD PTR DS:[ESI+0x844]
$+E9     004D5DA4     0F2FC1                    COMISS XMM0,XMM1
$+EC     004D5DA7     76 25                     JBE SHORT PathOfEx.004D5DCE
$+EE     004D5DA9     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+F0     004D5DAB     8BCE                      MOV ECX,ESI
$+F2     004D5DAD     F3:0F118E 44080000        MOVSS DWORD PTR DS:[ESI+0x844],XMM1
$+FA     004D5DB5     FF90 B8000000             CALL DWORD PTR DS:[EAX+0xB8]
$+100    004D5DBB     8B8E 5C070000             MOV ECX,DWORD PTR DS:[ESI+0x75C]
$+106    004D5DC1     85C9                      TEST ECX,ECX
$+108    004D5DC3     74 09                     JE SHORT PathOfEx.004D5DCE
$+10A    004D5DC5     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+10C    004D5DC7     56                        PUSH ESI
$+10D    004D5DC8     FF90 CC000000             CALL DWORD PTR DS:[EAX+0xCC]
$+113    004D5DCE     8B8F 14090000             MOV ECX,DWORD PTR DS:[EDI+0x914]
$+119    004D5DD4     81C1 B0080000             ADD ECX,0x8B0
$+11F    004D5DDA     6A 01                     PUSH 0x1
$+121    004D5DDC     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+123    004D5DDE     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
$+126    004D5DE1     8B8F 14090000             MOV ECX,DWORD PTR DS:[EDI+0x914]
$+12C    004D5DE7     83B9 68070000 05          CMP DWORD PTR DS:[ECX+0x768],0x5
$+133    004D5DEE     F3:0F1091 6C070000        MOVSS XMM2,DWORD PTR DS:[ECX+0x76C]
$+13B    004D5DF6     75 09                     JNZ SHORT PathOfEx.004D5E01
$+13D    004D5DF8     0F2ED2                    UCOMISS XMM2,XMM2
$+140    004D5DFB     9F                        LAHF
$+141    004D5DFC     F6C4 44                   TEST AH,0x44
$+144    004D5DFF     7B 11                     JPO SHORT PathOfEx.004D5E12
$+146    004D5E01     6A 05                     PUSH 0x5
$+148    004D5E03     C781 68070000 05000000    MOV DWORD PTR DS:[ECX+0x768],0x5
$+152    004D5E0D     E8 8EA85100               CALL PathOfEx.009F06A0
$+157    004D5E12     803D D6CDFF00 00          CMP BYTE PTR DS:[0xFFCDD6],0x0
$+15E    004D5E19     75 05                     JNZ SHORT PathOfEx.004D5E20
$+160    004D5E1B     E8 A0AF5300               CALL PathOfEx.00A10DC0
$+165    004D5E20     BA 12000000               MOV EDX,0x12
$+16A    004D5E25     B9 24ADE700               MOV ECX,PathOfEx.00E7AD24                       ; augmented
$+16F    004D5E2A     E8 31412300               CALL PathOfEx.00709F60
$+174    004D5E2F     8945 68                   MOV DWORD PTR SS:[EBP+0x68],EAX
$+177    004D5E32     8D45 68                   LEA EAX,DWORD PTR SS:[EBP+0x68]
$+17A    004D5E35     50                        PUSH EAX
$+17B    004D5E36     8D45 64                   LEA EAX,DWORD PTR SS:[EBP+0x64]
$+17E    004D5E39     50                        PUSH EAX
$+17F    004D5E3A     E8 51BD5300               CALL PathOfEx.00A11B90
$+184    004D5E3F     8B45 64                   MOV EAX,DWORD PTR SS:[EBP+0x64]
$+187    004D5E42     3B05 60D21301             CMP EAX,DWORD PTR DS:[0x113D260]
$+18D    004D5E48     75 04                     JNZ SHORT PathOfEx.004D5E4E
$+18F    004D5E4A     33D2                      XOR EDX,EDX
$+191    004D5E4C     EB 03                     JMP SHORT PathOfEx.004D5E51
$+193    004D5E4E     8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
$+196    004D5E51     8B8F 14090000             MOV ECX,DWORD PTR DS:[EDI+0x914]
$+19C    004D5E57     81C1 40080000             ADD ECX,0x840
$+1A2    004D5E5D     52                        PUSH EDX
$+1A3    004D5E5E     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+1A5    004D5E60     FF50 0C                   CALL DWORD PTR DS:[EAX+0xC]
$+1A8    004D5E63     8B8F 14090000             MOV ECX,DWORD PTR DS:[EDI+0x914]
$+1AE    004D5E69     6A 00                     PUSH 0x0
$+1B0    004D5E6B     E8 00935100               CALL PathOfEx.009EF170
$+1B5    004D5E70     833D 80DE0001 08          CMP DWORD PTR DS:[0x100DE80],0x8
$+1BC    004D5E77     B8 6CDE0001               MOV EAX,PathOfEx.0100DE6C
$+1C1    004D5E7C     C705 7CDE0001 00000000    MOV DWORD PTR DS:[0x100DE7C],0x0
$+1CB    004D5E86     0F4305 6CDE0001           CMOVNB EAX,DWORD PTR DS:[0x100DE6C]
$+1D2    004D5E8D     33C9                      XOR ECX,ECX
$+1D4    004D5E8F     66:8908                   MOV WORD PTR DS:[EAX],CX
$+1D7    004D5E92     8B83 A0000000             MOV EAX,DWORD PTR DS:[EBX+0xA0]
$+1DD    004D5E98     3B83 A4000000             CMP EAX,DWORD PTR DS:[EBX+0xA4]
$+1E3    004D5E9E     74 7A                     JE SHORT PathOfEx.004D5F1A
$+1E5    004D5EA0     390D 7CDE0001             CMP DWORD PTR DS:[0x100DE7C],ECX
$+1EB    004D5EA6     74 11                     JE SHORT PathOfEx.004D5EB9
$+1ED    004D5EA8     6A 01                     PUSH 0x1
$+1EF    004D5EAA     68 9483E700               PUSH PathOfEx.00E78394                          ; \n
$+1F4    004D5EAF     B9 6CDE0001               MOV ECX,PathOfEx.0100DE6C
$+1F9    004D5EB4     E8 5772FAFF               CALL PathOfEx.0047D110
$+1FE    004D5EB9     8B8B A4000000             MOV ECX,DWORD PTR DS:[EBX+0xA4]
$+204    004D5EBF     B8 ABAAAA2A               MOV EAX,0x2AAAAAAB
$+209    004D5EC4     2B8B A0000000             SUB ECX,DWORD PTR DS:[EBX+0xA0]
*/
//tag_添加一行物品tips字符串
/*
$-91     00A0CFAF     CC                        INT3
$-90     00A0CFB0     51                        PUSH ECX
$-8F     00A0CFB1     56                        PUSH ESI
$-8E     00A0CFB2     8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
$-8A     00A0CFB6     33C0                      XOR EAX,EAX
$-88     00A0CFB8     6A FF                     PUSH -0x1
$-86     00A0CFBA     81C1 18020000             ADD ECX,0x218
$-80     00A0CFC0     C74424 08 00000000        MOV DWORD PTR SS:[ESP+0x8],0x0
$-78     00A0CFC8     50                        PUSH EAX
$-77     00A0CFC9     51                        PUSH ECX
$-76     00A0CFCA     C746 14 07000000          MOV DWORD PTR DS:[ESI+0x14],0x7
$-6F     00A0CFD1     8BCE                      MOV ECX,ESI
$-6D     00A0CFD3     C746 10 00000000          MOV DWORD PTR DS:[ESI+0x10],0x0
$-66     00A0CFDA     66:8906                   MOV WORD PTR DS:[ESI],AX
$-63     00A0CFDD     E8 9E08A6FF               CALL PathOfEx.0046D880
$-5E     00A0CFE2     8BC6                      MOV EAX,ESI
$-5C     00A0CFE4     5E                        POP ESI
$-5B     00A0CFE5     59                        POP ECX
$-5A     00A0CFE6     C2 0400                   RETN 0x4
$-57     00A0CFE9     CC                        INT3
$-56     00A0CFEA     CC                        INT3
$-55     00A0CFEB     CC                        INT3
$-54     00A0CFEC     CC                        INT3
$-53     00A0CFED     CC                        INT3
$-52     00A0CFEE     CC                        INT3
$-51     00A0CFEF     CC                        INT3
$-50     00A0CFF0     51                        PUSH ECX
$-4F     00A0CFF1     80B9 200B0000 00          CMP BYTE PTR DS:[ECX+0xB20],0x0
$-48     00A0CFF8     56                        PUSH ESI
$-47     00A0CFF9     8DB1 C00A0000             LEA ESI,DWORD PTR DS:[ECX+0xAC0]
$-41     00A0CFFF     C74424 04 00000000        MOV DWORD PTR SS:[ESP+0x4],0x0
$-39     00A0D007     74 07                     JE SHORT PathOfEx.00A0D010
$-37     00A0D009     8BCE                      MOV ECX,ESI
$-35     00A0D00B     E8 90DD0000               CALL PathOfEx.00A1ADA0
$-30     00A0D010     8D4E 2C                   LEA ECX,DWORD PTR DS:[ESI+0x2C]
$-2D     00A0D013     33C0                      XOR EAX,EAX
$-2B     00A0D015     8B7424 0C                 MOV ESI,DWORD PTR SS:[ESP+0xC]
$-27     00A0D019     6A FF                     PUSH -0x1
$-25     00A0D01B     50                        PUSH EAX
$-24     00A0D01C     51                        PUSH ECX
$-23     00A0D01D     C746 14 07000000          MOV DWORD PTR DS:[ESI+0x14],0x7
$-1C     00A0D024     8BCE                      MOV ECX,ESI
$-1A     00A0D026     C746 10 00000000          MOV DWORD PTR DS:[ESI+0x10],0x0
$-13     00A0D02D     66:8906                   MOV WORD PTR DS:[ESI],AX
$-10     00A0D030     E8 4B08A6FF               CALL PathOfEx.0046D880
$-B      00A0D035     8BC6                      MOV EAX,ESI
$-9      00A0D037     5E                        POP ESI
$-8      00A0D038     59                        POP ECX
$-7      00A0D039     C2 0400                   RETN 0x4
$-4      00A0D03C     CC                        INT3
$-3      00A0D03D     CC                        INT3
$-2      00A0D03E     CC                        INT3
$-1      00A0D03F     CC                        INT3
$ ==>    00A0D040     56                        PUSH ESI                                        ; tag_添加一行物品tips字符串
$+1      00A0D041     FF7424 08                 PUSH DWORD PTR SS:[ESP+0x8]                     ; p1=const std::wstring*
$+5      00A0D045     8DB1 50F7FFFF             LEA ESI,DWORD PTR DS:[ECX-0x8B0]
$+B      00A0D04B     8D8E C00A0000             LEA ECX,DWORD PTR DS:[ESI+0xAC0]
$+11     00A0D051     E8 9ADB0000               CALL PathOfEx.00A1ABF0
$+16     00A0D056     84C0                      TEST AL,AL
$+18     00A0D058     74 4B                     JE SHORT PathOfEx.00A0D0A5
$+1A     00A0D05A     6A 01                     PUSH 0x1
$+1C     00A0D05C     8BCE                      MOV ECX,ESI
$+1E     00A0D05E     E8 BD0E0000               CALL PathOfEx.00A0DF20
$+23     00A0D063     80BE 7C0B0000 00          CMP BYTE PTR DS:[ESI+0xB7C],0x0
$+2A     00A0D06A     C686 6C0B0000 01          MOV BYTE PTR DS:[ESI+0xB6C],0x1
$+31     00A0D071     75 28                     JNZ SHORT PathOfEx.00A0D09B
$+33     00A0D073     0FB686 A5080000           MOVZX EAX,BYTE PTR DS:[ESI+0x8A5]
$+3A     00A0D07A     8BCE                      MOV ECX,ESI
$+3C     00A0D07C     8B16                      MOV EDX,DWORD PTR DS:[ESI]
$+3E     00A0D07E     50                        PUSH EAX
$+3F     00A0D07F     0FB686 A4080000           MOVZX EAX,BYTE PTR DS:[ESI+0x8A4]
$+46     00A0D086     50                        PUSH EAX
$+47     00A0D087     C686 7C0B0000 01          MOV BYTE PTR DS:[ESI+0xB7C],0x1
$+4E     00A0D08E     FF92 38010000             CALL DWORD PTR DS:[EDX+0x138]
$+54     00A0D094     C686 7C0B0000 00          MOV BYTE PTR DS:[ESI+0xB7C],0x0
$+5B     00A0D09B     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+5D     00A0D09D     8BCE                      MOV ECX,ESI
$+5F     00A0D09F     FF90 84010000             CALL DWORD PTR DS:[EAX+0x184]
$+65     00A0D0A5     5E                        POP ESI
$+66     00A0D0A6     C2 0400                   RETN 0x4
$+69     00A0D0A9     CC                        INT3
$+6A     00A0D0AA     CC                        INT3
$+6B     00A0D0AB     CC                        INT3
$+6C     00A0D0AC     CC                        INT3
$+6D     00A0D0AD     CC                        INT3
$+6E     00A0D0AE     CC                        INT3
$+6F     00A0D0AF     CC                        INT3
$+70     00A0D0B0     56                        PUSH ESI
$+71     00A0D0B1     FF7424 08                 PUSH DWORD PTR SS:[ESP+0x8]
$+75     00A0D0B5     8BF1                      MOV ESI,ECX
$+77     00A0D0B7     8D8E C00A0000             LEA ECX,DWORD PTR DS:[ESI+0xAC0]
$+7D     00A0D0BD     E8 2EDB0000               CALL PathOfEx.00A1ABF0
$+82     00A0D0C2     84C0                      TEST AL,AL
$+84     00A0D0C4     74 52                     JE SHORT PathOfEx.00A0D118
$+86     00A0D0C6     6A 01                     PUSH 0x1
$+88     00A0D0C8     8BCE                      MOV ECX,ESI
$+8A     00A0D0CA     E8 510E0000               CALL PathOfEx.00A0DF20
$+8F     00A0D0CF     80BE 7C0B0000 00          CMP BYTE PTR DS:[ESI+0xB7C],0x0
$+96     00A0D0D6     C686 6C0B0000 01          MOV BYTE PTR DS:[ESI+0xB6C],0x1
$+9D     00A0D0DD     75 28                     JNZ SHORT PathOfEx.00A0D107
$+9F     00A0D0DF     0FB686 A5080000           MOVZX EAX,BYTE PTR DS:[ESI+0x8A5]
$+A6     00A0D0E6     8BCE                      MOV ECX,ESI
$+A8     00A0D0E8     8B16                      MOV EDX,DWORD PTR DS:[ESI]
$+AA     00A0D0EA     50                        PUSH EAX
$+AB     00A0D0EB     0FB686 A4080000           MOVZX EAX,BYTE PTR DS:[ESI+0x8A4]
$+B2     00A0D0F2     50                        PUSH EAX
$+B3     00A0D0F3     C686 7C0B0000 01          MOV BYTE PTR DS:[ESI+0xB7C],0x1
$+BA     00A0D0FA     FF92 38010000             CALL DWORD PTR DS:[EDX+0x138]
$+C0     00A0D100     C686 7C0B0000 00          MOV BYTE PTR DS:[ESI+0xB7C],0x0
$+C7     00A0D107     807C24 0C 00              CMP BYTE PTR SS:[ESP+0xC],0x0
$+CC     00A0D10C     75 0A                     JNZ SHORT PathOfEx.00A0D118
$+CE     00A0D10E     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+D0     00A0D110     8BCE                      MOV ECX,ESI
$+D2     00A0D112     FF90 84010000             CALL DWORD PTR DS:[EAX+0x184]
$+D8     00A0D118     5E                        POP ESI
$+D9     00A0D119     C2 0800                   RETN 0x8
$+DC     00A0D11C     CC                        INT3
$+DD     00A0D11D     CC                        INT3
$+DE     00A0D11E     CC                        INT3
$+DF     00A0D11F     CC                        INT3
$+E0     00A0D120     55                        PUSH EBP
$+E1     00A0D121     8BEC                      MOV EBP,ESP
$+E3     00A0D123     6A FF                     PUSH -0x1
$+E5     00A0D125     68 783CD200               PUSH PathOfEx.00D23C78
$+EA     00A0D12A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+F0     00A0D130     50                        PUSH EAX
$+F1     00A0D131     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+F8     00A0D138     83EC 18                   SUB ESP,0x18
$+FB     00A0D13B     56                        PUSH ESI
$+FC     00A0D13C     33C0                      XOR EAX,EAX
$+FE     00A0D13E     C745 F0 07000000          MOV DWORD PTR SS:[EBP-0x10],0x7
$+105    00A0D145     50                        PUSH EAX
$+106    00A0D146     8BF1                      MOV ESI,ECX
$+108    00A0D148     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
$+10F    00A0D14F     68 BC2CE700               PUSH PathOfEx.00E72CBC
$+114    00A0D154     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$+117    00A0D157     66:8945 DC                MOV WORD PTR SS:[EBP-0x24],AX
$+11B    00A0D15B     E8 500BA6FF               CALL PathOfEx.0046DCB0
$+120    00A0D160     8D8E B0080000             LEA ECX,DWORD PTR DS:[ESI+0x8B0]
$+126    00A0D166     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+12D    00A0D16D     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+12F    00A0D16F     8D55 DC                   LEA EDX,DWORD PTR SS:[EBP-0x24]
$+132    00A0D172     52                        PUSH EDX
$+133    00A0D173     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+136    00A0D176     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
$+139    00A0D179     5E                        POP ESI
$+13A    00A0D17A     83F8 08                   CMP EAX,0x8
$+13D    00A0D17D     72 0F                     JB SHORT PathOfEx.00A0D18E
$+13F    00A0D17F     6A 02                     PUSH 0x2
$+141    00A0D181     40                        INC EAX
$+142    00A0D182     50                        PUSH EAX
$+143    00A0D183     FF75 DC                   PUSH DWORD PTR SS:[EBP-0x24]
$+146    00A0D186     E8 A5F6A5FF               CALL PathOfEx.0046C830
$+14B    00A0D18B     83C4 0C                   ADD ESP,0xC
$+14E    00A0D18E     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+151    00A0D191     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+158    00A0D198     8BE5                      MOV ESP,EBP
$+15A    00A0D19A     5D                        POP EBP
$+15B    00A0D19B     C3                        RETN
$+15C    00A0D19C     CC                        INT3
*/
/**/
//tag_物品tips生成格式化与颜色化字符串
/*
$-11C    00C97B94     C3                        RETN
$-11B    00C97B95     0FBA25 0098FD00 01        BT DWORD PTR DS:[0xFD9800],0x1
$-113    00C97B9D     0F83 B2000000             JNB PathOfEx.00C97C55
$-10D    00C97BA3     66:0F6EC0                 MOVD MM0,EAX
$-109    00C97BA7     66:0F70C0 00              PSHUFW MM0,MM0,0x0
$-104    00C97BAC     03CF                      ADD ECX,EDI
$-102    00C97BAE     0F1107                    MOVUPS DQWORD PTR DS:[EDI],XMM0
$-FF     00C97BB1     83C7 10                   ADD EDI,0x10
$-FC     00C97BB4     83E7 F0                   AND EDI,0xFFFFFFF0
$-F9     00C97BB7     2BCF                      SUB ECX,EDI
$-F7     00C97BB9     81F9 80000000             CMP ECX,0x80
$-F1     00C97BBF     7E 4C                     JLE SHORT PathOfEx.00C97C0D
$-EF     00C97BC1     8DA424 00000000           LEA ESP,DWORD PTR SS:[ESP]
$-E8     00C97BC8     8DA424 00000000           LEA ESP,DWORD PTR SS:[ESP]
$-E1     00C97BCF     90                        NOP
$-E0     00C97BD0     66:0F7F07                 MOVQ QWORD PTR DS:[EDI],MM0
$-DC     00C97BD4     66:0F7F47 10              MOVQ QWORD PTR DS:[EDI+0x10],MM0
$-D7     00C97BD9     66:0F7F47 20              MOVQ QWORD PTR DS:[EDI+0x20],MM0
$-D2     00C97BDE     66:0F7F47 30              MOVQ QWORD PTR DS:[EDI+0x30],MM0
$-CD     00C97BE3     66:0F7F47 40              MOVQ QWORD PTR DS:[EDI+0x40],MM0
$-C8     00C97BE8     66:0F7F47 50              MOVQ QWORD PTR DS:[EDI+0x50],MM0
$-C3     00C97BED     66:0F7F47 60              MOVQ QWORD PTR DS:[EDI+0x60],MM0
$-BE     00C97BF2     66:0F7F47 70              MOVQ QWORD PTR DS:[EDI+0x70],MM0
$-B9     00C97BF7     8DBF 80000000             LEA EDI,DWORD PTR DS:[EDI+0x80]
$-B3     00C97BFD     81E9 80000000             SUB ECX,0x80
$-AD     00C97C03     F7C1 00FFFFFF             TEST ECX,0xFFFFFF00
$-A7     00C97C09   ^ 75 C5                     JNZ SHORT PathOfEx.00C97BD0
$-A5     00C97C0B     EB 13                     JMP SHORT PathOfEx.00C97C20
$-A3     00C97C0D     0FBA25 0098FD00 01        BT DWORD PTR DS:[0xFD9800],0x1
$-9B     00C97C15     73 3E                     JNB SHORT PathOfEx.00C97C55
$-99     00C97C17     66:0F6EC0                 MOVD MM0,EAX
$-95     00C97C1B     66:0F70C0 00              PSHUFW MM0,MM0,0x0
$-90     00C97C20     83F9 20                   CMP ECX,0x20
$-8D     00C97C23     72 1C                     JB SHORT PathOfEx.00C97C41
$-8B     00C97C25     F30F7F07                  MOVDQU DQWORD PTR DS:[EDI],XMM0
$-87     00C97C29     F30F7F47 10               MOVDQU DQWORD PTR DS:[EDI+0x10],XMM0
$-82     00C97C2E     83C7 20                   ADD EDI,0x20
$-7F     00C97C31     83E9 20                   SUB ECX,0x20
$-7C     00C97C34     83F9 20                   CMP ECX,0x20
$-79     00C97C37   ^ 73 EC                     JNB SHORT PathOfEx.00C97C25
$-77     00C97C39     F7C1 1F000000             TEST ECX,0x1F
$-71     00C97C3F     74 62                     JE SHORT PathOfEx.00C97CA3
$-6F     00C97C41     8D7C39 E0                 LEA EDI,DWORD PTR DS:[ECX+EDI-0x20]
$-6B     00C97C45     F30F7F07                  MOVDQU DQWORD PTR DS:[EDI],XMM0
$-67     00C97C49     F30F7F47 10               MOVDQU DQWORD PTR DS:[EDI+0x10],XMM0
$-62     00C97C4E     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
$-5E     00C97C52     8BFA                      MOV EDI,EDX
$-5C     00C97C54     C3                        RETN
$-5B     00C97C55     F7C1 03000000             TEST ECX,0x3
$-55     00C97C5B     74 0E                     JE SHORT PathOfEx.00C97C6B
$-53     00C97C5D     8807                      MOV BYTE PTR DS:[EDI],AL
$-51     00C97C5F     47                        INC EDI
$-50     00C97C60     83E9 01                   SUB ECX,0x1
$-4D     00C97C63     F7C1 03000000             TEST ECX,0x3
$-47     00C97C69   ^ 75 F2                     JNZ SHORT PathOfEx.00C97C5D
$-45     00C97C6B     F7C1 04000000             TEST ECX,0x4
$-3F     00C97C71     74 08                     JE SHORT PathOfEx.00C97C7B
$-3D     00C97C73     8907                      MOV DWORD PTR DS:[EDI],EAX
$-3B     00C97C75     83C7 04                   ADD EDI,0x4
$-38     00C97C78     83E9 04                   SUB ECX,0x4
$-35     00C97C7B     F7C1 F8FFFFFF             TEST ECX,0xFFFFFFF8
$-2F     00C97C81     74 20                     JE SHORT PathOfEx.00C97CA3
$-2D     00C97C83     8DA424 00000000           LEA ESP,DWORD PTR SS:[ESP]
$-26     00C97C8A     8D9B 00000000             LEA EBX,DWORD PTR DS:[EBX]
$-20     00C97C90     8907                      MOV DWORD PTR DS:[EDI],EAX
$-1E     00C97C92     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
$-1B     00C97C95     83C7 08                   ADD EDI,0x8
$-18     00C97C98     83E9 08                   SUB ECX,0x8
$-15     00C97C9B     F7C1 F8FFFFFF             TEST ECX,0xFFFFFFF8
$-F      00C97CA1   ^ 75 ED                     JNZ SHORT PathOfEx.00C97C90
$-D      00C97CA3     8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
$-9      00C97CA7     8BFA                      MOV EDI,EDX
$-7      00C97CA9     C3                        RETN
$-6      00C97CAA     CC                        INT3
$-5      00C97CAB     CC                        INT3
$-4      00C97CAC     CC                        INT3
$-3      00C97CAD     CC                        INT3
$-2      00C97CAE     CC                        INT3
$-1      00C97CAF     CC                        INT3
$ ==>    00C97CB0     57                        PUSH EDI                                           ; tag_物品tips生成格式化与颜色化字符串
$+1      00C97CB1     56                        PUSH ESI                                           ; 需求等级 <valuedefault>{49}, <valuedefault>{134} 智慧.
$+2      00C97CB2     8B7424 10                 MOV ESI,DWORD PTR SS:[ESP+0x10]                    ; p2=字符串copy源
$+6      00C97CB6     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]                    ; p3=字符串长度
$+A      00C97CBA     8B7C24 0C                 MOV EDI,DWORD PTR SS:[ESP+0xC]                     ; p1=目标字符串缓冲区
$+E      00C97CBE     8BC1                      MOV EAX,ECX
$+10     00C97CC0     8BD1                      MOV EDX,ECX
$+12     00C97CC2     03C6                      ADD EAX,ESI
$+14     00C97CC4     3BFE                      CMP EDI,ESI
$+16     00C97CC6     76 08                     JBE SHORT PathOfEx.00C97CD0
$+18     00C97CC8     3BF8                      CMP EDI,EAX
$+1A     00C97CCA     0F82 94020000             JB PathOfEx.00C97F64
$+20     00C97CD0     83F9 20                   CMP ECX,0x20
$+23     00C97CD3     0F82 D2040000             JB PathOfEx.00C981AB
$+29     00C97CD9     81F9 80000000             CMP ECX,0x80
$+2F     00C97CDF     73 13                     JNB SHORT PathOfEx.00C97CF4
$+31     00C97CE1     0FBA25 0098FD00 01        BT DWORD PTR DS:[0xFD9800],0x1
$+39     00C97CE9     0F82 8E040000             JB PathOfEx.00C9817D
$+3F     00C97CEF     E9 E3010000               JMP PathOfEx.00C97ED7
$+44     00C97CF4     0FBA25 3CB1FF00 01        BT DWORD PTR DS:[0xFFB13C],0x1
$+4C     00C97CFC     73 09                     JNB SHORT PathOfEx.00C97D07
$+4E     00C97CFE     F3:A4                     REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
$+50     00C97D00     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
$+54     00C97D04     5E                        POP ESI
$+55     00C97D05     5F                        POP EDI
$+56     00C97D06     C3                        RETN
$+57     00C97D07     8BC7                      MOV EAX,EDI
$+59     00C97D09     33C6                      XOR EAX,ESI
$+5B     00C97D0B     A9 0F000000               TEST EAX,0xF
$+60     00C97D10     75 0E                     JNZ SHORT PathOfEx.00C97D20
$+62     00C97D12     0FBA25 0098FD00 01        BT DWORD PTR DS:[0xFD9800],0x1
$+6A     00C97D1A     0F82 E0030000             JB PathOfEx.00C98100
$+70     00C97D20     0FBA25 3CB1FF00 00        BT DWORD PTR DS:[0xFFB13C],0x0
$+78     00C97D28     0F83 A9010000             JNB PathOfEx.00C97ED7
$+7E     00C97D2E     F7C7 03000000             TEST EDI,0x3
$+84     00C97D34     0F85 9D010000             JNZ PathOfEx.00C97ED7
$+8A     00C97D3A     F7C6 03000000             TEST ESI,0x3
$+90     00C97D40     0F85 AC010000             JNZ PathOfEx.00C97EF2
$+96     00C97D46     0FBAE7 02                 BT EDI,0x2
$+9A     00C97D4A     73 0D                     JNB SHORT PathOfEx.00C97D59
$+9C     00C97D4C     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+9E     00C97D4E     83E9 04                   SUB ECX,0x4
$+A1     00C97D51     8D76 04                   LEA ESI,DWORD PTR DS:[ESI+0x4]
$+A4     00C97D54     8907                      MOV DWORD PTR DS:[EDI],EAX
$+A6     00C97D56     8D7F 04                   LEA EDI,DWORD PTR DS:[EDI+0x4]
$+A9     00C97D59     0FBAE7 03                 BT EDI,0x3
$+AD     00C97D5D     73 11                     JNB SHORT PathOfEx.00C97D70
$+AF     00C97D5F     F30F7E0E                  MOVQ XMM1,QWORD PTR DS:[ESI]
$+B3     00C97D63     83E9 08                   SUB ECX,0x8
$+B6     00C97D66     8D76 08                   LEA ESI,DWORD PTR DS:[ESI+0x8]
$+B9     00C97D69     660FD60F                  MOVQ QWORD PTR DS:[EDI],XMM1
$+BD     00C97D6D     8D7F 08                   LEA EDI,DWORD PTR DS:[EDI+0x8]
$+C0     00C97D70     F7C6 07000000             TEST ESI,0x7
$+C6     00C97D76     74 65                     JE SHORT PathOfEx.00C97DDD
$+C8     00C97D78     0FBAE6 03                 BT ESI,0x3
$+CC     00C97D7C     0F83 B4000000             JNB PathOfEx.00C97E36
$+D2     00C97D82     66:0F6F4E F4              MOVQ MM1,QWORD PTR DS:[ESI-0xC]
$+D7     00C97D87     8D76 F4                   LEA ESI,DWORD PTR DS:[ESI-0xC]
$+DA     00C97D8A     8BFF                      MOV EDI,EDI
$+DC     00C97D8C     66:0F6F5E 10              MOVQ MM3,QWORD PTR DS:[ESI+0x10]
$+E1     00C97D91     83E9 30                   SUB ECX,0x30
$+E4     00C97D94     66:0F6F46 20              MOVQ MM0,QWORD PTR DS:[ESI+0x20]
$+E9     00C97D99     66:0F6F6E 30              MOVQ MM5,QWORD PTR DS:[ESI+0x30]
$+EE     00C97D9E     8D76 30                   LEA ESI,DWORD PTR DS:[ESI+0x30]
$+F1     00C97DA1     83F9 30                   CMP ECX,0x30
$+F4     00C97DA4     66:0F6FD3                 MOVQ MM2,MM3
$+F8     00C97DA8     660F3A0FD9 0C             PALIGNR XMM3,XMM1,0xC
$+FE     00C97DAE     66:0F7F1F                 MOVQ QWORD PTR DS:[EDI],MM3
$+102    00C97DB2     66:0F6FE0                 MOVQ MM4,MM0
$+106    00C97DB6     660F3A0FC2 0C             PALIGNR XMM0,XMM2,0xC
$+10C    00C97DBC     66:0F7F47 10              MOVQ QWORD PTR DS:[EDI+0x10],MM0
$+111    00C97DC1     66:0F6FCD                 MOVQ MM1,MM5
$+115    00C97DC5     660F3A0FEC 0C             PALIGNR XMM5,XMM4,0xC
$+11B    00C97DCB     66:0F7F6F 20              MOVQ QWORD PTR DS:[EDI+0x20],MM5
$+120    00C97DD0     8D7F 30                   LEA EDI,DWORD PTR DS:[EDI+0x30]
$+123    00C97DD3   ^ 7D B7                     JGE SHORT PathOfEx.00C97D8C
$+125    00C97DD5     8D76 0C                   LEA ESI,DWORD PTR DS:[ESI+0xC]
$+128    00C97DD8     E9 AF000000               JMP PathOfEx.00C97E8C
$+12D    00C97DDD     66:0F6F4E F8              MOVQ MM1,QWORD PTR DS:[ESI-0x8]
$+132    00C97DE2     8D76 F8                   LEA ESI,DWORD PTR DS:[ESI-0x8]
$+135    00C97DE5     8D49 00                   LEA ECX,DWORD PTR DS:[ECX]
$+138    00C97DE8     66:0F6F5E 10              MOVQ MM3,QWORD PTR DS:[ESI+0x10]
$+13D    00C97DED     83E9 30                   SUB ECX,0x30
$+140    00C97DF0     66:0F6F46 20              MOVQ MM0,QWORD PTR DS:[ESI+0x20]
$+145    00C97DF5     66:0F6F6E 30              MOVQ MM5,QWORD PTR DS:[ESI+0x30]
$+14A    00C97DFA     8D76 30                   LEA ESI,DWORD PTR DS:[ESI+0x30]
$+14D    00C97DFD     83F9 30                   CMP ECX,0x30
$+150    00C97E00     66:0F6FD3                 MOVQ MM2,MM3
$+154    00C97E04     660F3A0FD9 08             PALIGNR XMM3,XMM1,0x8
$+15A    00C97E0A     66:0F7F1F                 MOVQ QWORD PTR DS:[EDI],MM3
$+15E    00C97E0E     66:0F6FE0                 MOVQ MM4,MM0
$+162    00C97E12     660F3A0FC2 08             PALIGNR XMM0,XMM2,0x8
$+168    00C97E18     66:0F7F47 10              MOVQ QWORD PTR DS:[EDI+0x10],MM0
$+16D    00C97E1D     66:0F6FCD                 MOVQ MM1,MM5
$+171    00C97E21     660F3A0FEC 08             PALIGNR XMM5,XMM4,0x8
$+177    00C97E27     66:0F7F6F 20              MOVQ QWORD PTR DS:[EDI+0x20],MM5
$+17C    00C97E2C     8D7F 30                   LEA EDI,DWORD PTR DS:[EDI+0x30]
$+17F    00C97E2F   ^ 7D B7                     JGE SHORT PathOfEx.00C97DE8
$+181    00C97E31     8D76 08                   LEA ESI,DWORD PTR DS:[ESI+0x8]
$+184    00C97E34     EB 56                     JMP SHORT PathOfEx.00C97E8C
$+186    00C97E36     66:0F6F4E FC              MOVQ MM1,QWORD PTR DS:[ESI-0x4]
$+18B    00C97E3B     8D76 FC                   LEA ESI,DWORD PTR DS:[ESI-0x4]
$+18E    00C97E3E     8BFF                      MOV EDI,EDI
$+190    00C97E40     66:0F6F5E 10              MOVQ MM3,QWORD PTR DS:[ESI+0x10]
$+195    00C97E45     83E9 30                   SUB ECX,0x30
$+198    00C97E48     66:0F6F46 20              MOVQ MM0,QWORD PTR DS:[ESI+0x20]
$+19D    00C97E4D     66:0F6F6E 30              MOVQ MM5,QWORD PTR DS:[ESI+0x30]
$+1A2    00C97E52     8D76 30                   LEA ESI,DWORD PTR DS:[ESI+0x30]
$+1A5    00C97E55     83F9 30                   CMP ECX,0x30
$+1A8    00C97E58     66:0F6FD3                 MOVQ MM2,MM3
$+1AC    00C97E5C     660F3A0FD9 04             PALIGNR XMM3,XMM1,0x4
$+1B2    00C97E62     66:0F7F1F                 MOVQ QWORD PTR DS:[EDI],MM3
$+1B6    00C97E66     66:0F6FE0                 MOVQ MM4,MM0
$+1BA    00C97E6A     660F3A0FC2 04             PALIGNR XMM0,XMM2,0x4
$+1C0    00C97E70     66:0F7F47 10              MOVQ QWORD PTR DS:[EDI+0x10],MM0
$+1C5    00C97E75     66:0F6FCD                 MOVQ MM1,MM5
$+1C9    00C97E79     660F3A0FEC 04             PALIGNR XMM5,XMM4,0x4
$+1CF    00C97E7F     66:0F7F6F 20              MOVQ QWORD PTR DS:[EDI+0x20],MM5
$+1D4    00C97E84     8D7F 30                   LEA EDI,DWORD PTR DS:[EDI+0x30]
$+1D7    00C97E87   ^ 7D B7                     JGE SHORT PathOfEx.00C97E40
$+1D9    00C97E89     8D76 04                   LEA ESI,DWORD PTR DS:[ESI+0x4]
$+1DC    00C97E8C     83F9 10                   CMP ECX,0x10
$+1DF    00C97E8F     7C 13                     JL SHORT PathOfEx.00C97EA4
$+1E1    00C97E91     F30F6F0E                  MOVDQU XMM1,DQWORD PTR DS:[ESI]
$+1E5    00C97E95     83E9 10                   SUB ECX,0x10
$+1E8    00C97E98     8D76 10                   LEA ESI,DWORD PTR DS:[ESI+0x10]
$+1EB    00C97E9B     66:0F7F0F                 MOVQ QWORD PTR DS:[EDI],MM1
$+1EF    00C97E9F     8D7F 10                   LEA EDI,DWORD PTR DS:[EDI+0x10]
$+1F2    00C97EA2   ^ EB E8                     JMP SHORT PathOfEx.00C97E8C
$+1F4    00C97EA4     0FBAE1 02                 BT ECX,0x2
$+1F8    00C97EA8     73 0D                     JNB SHORT PathOfEx.00C97EB7
$+1FA    00C97EAA     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+1FC    00C97EAC     83E9 04                   SUB ECX,0x4
$+1FF    00C97EAF     8D76 04                   LEA ESI,DWORD PTR DS:[ESI+0x4]
$+202    00C97EB2     8907                      MOV DWORD PTR DS:[EDI],EAX
$+204    00C97EB4     8D7F 04                   LEA EDI,DWORD PTR DS:[EDI+0x4]
$+207    00C97EB7     0FBAE1 03                 BT ECX,0x3
$+20B    00C97EBB     73 11                     JNB SHORT PathOfEx.00C97ECE
$+20D    00C97EBD     F30F7E0E                  MOVQ XMM1,QWORD PTR DS:[ESI]
$+211    00C97EC1     83E9 08                   SUB ECX,0x8
$+214    00C97EC4     8D76 08                   LEA ESI,DWORD PTR DS:[ESI+0x8]
$+217    00C97EC7     660FD60F                  MOVQ QWORD PTR DS:[EDI],XMM1
$+21B    00C97ECB     8D7F 08                   LEA EDI,DWORD PTR DS:[EDI+0x8]
$+21E    00C97ECE     8B048D 147FC900           MOV EAX,DWORD PTR DS:[ECX*4+0xC97F14]
$+225    00C97ED5     FFE0                      JMP EAX
$+227    00C97ED7     F7C7 03000000             TEST EDI,0x3
$+22D    00C97EDD     74 13                     JE SHORT PathOfEx.00C97EF2
$+22F    00C97EDF     8A06                      MOV AL,BYTE PTR DS:[ESI]
$+231    00C97EE1     8807                      MOV BYTE PTR DS:[EDI],AL
$+233    00C97EE3     49                        DEC ECX
$+234    00C97EE4     83C6 01                   ADD ESI,0x1
$+237    00C97EE7     83C7 01                   ADD EDI,0x1
$+23A    00C97EEA     F7C7 03000000             TEST EDI,0x3
$+240    00C97EF0   ^ 75 ED                     JNZ SHORT PathOfEx.00C97EDF
$+242    00C97EF2     8BD1                      MOV EDX,ECX
$+244    00C97EF4     83F9 20                   CMP ECX,0x20
$+247    00C97EF7     0F82 AE020000             JB PathOfEx.00C981AB
$+24D    00C97EFD     C1E9 02                   SHR ECX,0x2
$+250    00C97F00     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
$+252    00C97F02     83E2 03                   AND EDX,0x3
$+255    00C97F05     FF2495 147FC900           JMP DWORD PTR DS:[EDX*4+0xC97F14]
$+25C    00C97F0C     FF248D 247FC900           JMP DWORD PTR DS:[ECX*4+0xC97F24]
$+263    00C97F13     90                        NOP
$+264    00C97F14     24 7F                     AND AL,0x7F
$+266    00C97F16     C9                        LEAVE
$+267    00C97F17     002C7F                    ADD BYTE PTR DS:[EDI+EDI*2],CH
$+26A    00C97F1A     C9                        LEAVE
$+26B    00C97F1B     0038                      ADD BYTE PTR DS:[EAX],BH
$+26D    00C97F1D   ^ 7F C9                     JG SHORT PathOfEx.00C97EE8
$+26F    00C97F1F     004C7F C9                 ADD BYTE PTR DS:[EDI+EDI*2-0x37],CL
$+273    00C97F23     008B 44240C5E             ADD BYTE PTR DS:[EBX+0x5E0C2444],CL
$+279    00C97F29     5F                        POP EDI
$+27A    00C97F2A     C3                        RETN
$+27B    00C97F2B     90                        NOP
$+27C    00C97F2C     8A06                      MOV AL,BYTE PTR DS:[ESI]
$+27E    00C97F2E     8807                      MOV BYTE PTR DS:[EDI],AL
$+280    00C97F30     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
$+284    00C97F34     5E                        POP ESI
$+285    00C97F35     5F                        POP EDI
$+286    00C97F36     C3                        RETN
$+287    00C97F37     90                        NOP
$+288    00C97F38     8A06                      MOV AL,BYTE PTR DS:[ESI]
$+28A    00C97F3A     8807                      MOV BYTE PTR DS:[EDI],AL
$+28C    00C97F3C     8A46 01                   MOV AL,BYTE PTR DS:[ESI+0x1]
$+28F    00C97F3F     8847 01                   MOV BYTE PTR DS:[EDI+0x1],AL
$+292    00C97F42     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
$+296    00C97F46     5E                        POP ESI
$+297    00C97F47     5F                        POP EDI
$+298    00C97F48     C3                        RETN
$+299    00C97F49     8D49 00                   LEA ECX,DWORD PTR DS:[ECX]
$+29C    00C97F4C     8A06                      MOV AL,BYTE PTR DS:[ESI]
$+29E    00C97F4E     8807                      MOV BYTE PTR DS:[EDI],AL
$+2A0    00C97F50     8A46 01                   MOV AL,BYTE PTR DS:[ESI+0x1]
$+2A3    00C97F53     8847 01                   MOV BYTE PTR DS:[EDI+0x1],AL
$+2A6    00C97F56     8A46 02                   MOV AL,BYTE PTR DS:[ESI+0x2]
$+2A9    00C97F59     8847 02                   MOV BYTE PTR DS:[EDI+0x2],AL
$+2AC    00C97F5C     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
$+2B0    00C97F60     5E                        POP ESI
$+2B1    00C97F61     5F                        POP EDI
$+2B2    00C97F62     C3                        RETN
$+2B3    00C97F63     90                        NOP
$+2B4    00C97F64     8D3431                    LEA ESI,DWORD PTR DS:[ECX+ESI]
$+2B7    00C97F67     8D3C39                    LEA EDI,DWORD PTR DS:[ECX+EDI]
$+2BA    00C97F6A     83F9 20                   CMP ECX,0x20
$+2BD    00C97F6D     0F82 51010000             JB PathOfEx.00C980C4
$+2C3    00C97F73     0FBA25 0098FD00 01        BT DWORD PTR DS:[0xFD9800],0x1
$+2CB    00C97F7B     0F82 94000000             JB PathOfEx.00C98015
$+2D1    00C97F81     F7C7 03000000             TEST EDI,0x3
$+2D7    00C97F87     74 14                     JE SHORT PathOfEx.00C97F9D
$+2D9    00C97F89     8BD7                      MOV EDX,EDI
$+2DB    00C97F8B     83E2 03                   AND EDX,0x3
$+2DE    00C97F8E     2BCA                      SUB ECX,EDX
$+2E0    00C97F90     8A46 FF                   MOV AL,BYTE PTR DS:[ESI-0x1]
$+2E3    00C97F93     8847 FF                   MOV BYTE PTR DS:[EDI-0x1],AL
$+2E6    00C97F96     4E                        DEC ESI
$+2E7    00C97F97     4F                        DEC EDI
$+2E8    00C97F98     83EA 01                   SUB EDX,0x1
$+2EB    00C97F9B   ^ 75 F3                     JNZ SHORT PathOfEx.00C97F90
$+2ED    00C97F9D     83F9 20                   CMP ECX,0x20
$+2F0    00C97FA0     0F82 1E010000             JB PathOfEx.00C980C4
$+2F6    00C97FA6     8BD1                      MOV EDX,ECX
$+2F8    00C97FA8     C1E9 02                   SHR ECX,0x2
$+2FB    00C97FAB     83E2 03                   AND EDX,0x3
$+2FE    00C97FAE     83EE 04                   SUB ESI,0x4
$+301    00C97FB1     83EF 04                   SUB EDI,0x4
$+304    00C97FB4     FD                        STD
$+305    00C97FB5     F3:A5                     REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
$+307    00C97FB7     FC                        CLD
$+308    00C97FB8     FF2495 C07FC900           JMP DWORD PTR DS:[EDX*4+0xC97FC0]
$+30F    00C97FBF     90                        NOP
$+310    00C97FC0     D07F C9                   SAR BYTE PTR DS:[EDI-0x37],1
$+313    00C97FC3     00D8                      ADD AL,BL
$+315    00C97FC5   ^ 7F C9                     JG SHORT PathOfEx.00C97F90
$+317    00C97FC7     00E8                      ADD AL,CH
$+319    00C97FC9   ^ 7F C9                     JG SHORT PathOfEx.00C97F94
$+31B    00C97FCB     00FC                      ADD AH,BH
$+31D    00C97FCD   ^ 7F C9                     JG SHORT PathOfEx.00C97F98
$+31F    00C97FCF     008B 44240C5E             ADD BYTE PTR DS:[EBX+0x5E0C2444],CL
$+325    00C97FD5     5F                        POP EDI
$+326    00C97FD6     C3                        RETN
$+327    00C97FD7     90                        NOP
*/
//tag_生成物品属性格式化字符串
/*
$-121    005B0ACF     CC                        INT3
$-120    005B0AD0     55                        PUSH EBP
$-11F    005B0AD1     8BEC                      MOV EBP,ESP
$-11D    005B0AD3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-117    005B0AD9     6A FF                     PUSH -0x1
$-115    005B0ADB     68 0039D200               PUSH PathOfEx.00D23900
$-110    005B0AE0     50                        PUSH EAX
$-10F    005B0AE1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-108    005B0AE8     53                        PUSH EBX
$-107    005B0AE9     56                        PUSH ESI
$-106    005B0AEA     8BF1                      MOV ESI,ECX
$-104    005B0AEC     57                        PUSH EDI
$-103    005B0AED     8DBE B0080000             LEA EDI,DWORD PTR DS:[ESI+0x8B0]
$-FD     005B0AF3     C786 B4080000 24A1E700    MOV DWORD PTR DS:[ESI+0x8B4],PathOfEx.00E7A124
$-F3     005B0AFD     8D9E B4080000             LEA EBX,DWORD PTR DS:[ESI+0x8B4]
$-ED     005B0B03     C707 C0B1E700             MOV DWORD PTR DS:[EDI],PathOfEx.00E7B1C0         ; ASCII "廓F"
$-E7     005B0B09     F686 C0080000 04          TEST BYTE PTR DS:[ESI+0x8C0],0x4
$-E0     005B0B10     C706 B437E900             MOV DWORD PTR DS:[ESI],PathOfEx.00E937B4
$-DA     005B0B16     C786 3C080000 B497F300    MOV DWORD PTR DS:[ESI+0x83C],PathOfEx.00F397B4
$-D0     005B0B20     C786 40080000 6CDCE900    MOV DWORD PTR DS:[ESI+0x840],PathOfEx.00E9DC6C
$-C6     005B0B2A     74 44                     JE SHORT PathOfEx.005B0B70
$-C4     005B0B2C     80BE BC080000 00          CMP BYTE PTR DS:[ESI+0x8BC],0x0
$-BD     005B0B33     74 19                     JE SHORT PathOfEx.005B0B4E
$-BB     005B0B35     8B8E B8080000             MOV ECX,DWORD PTR DS:[ESI+0x8B8]
$-B5     005B0B3B     6A 00                     PUSH 0x0
$-B3     005B0B3D     68 00105B00               PUSH PathOfEx.005B1000
$-AE     005B0B42     56                        PUSH ESI
$-AD     005B0B43     81C1 94150000             ADD ECX,0x1594
$-A7     005B0B49     E8 52D5EEFF               CALL PathOfEx.0049E0A0
$-A2     005B0B4E     8B8E C8080000             MOV ECX,DWORD PTR DS:[ESI+0x8C8]
$-9C     005B0B54     57                        PUSH EDI
$-9B     005B0B55     E8 9626EFFF               CALL PathOfEx.004A31F0
$-96     005B0B5A     8B86 C8080000             MOV EAX,DWORD PTR DS:[ESI+0x8C8]
$-90     005B0B60     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
$-8D     005B0B63     E8 08B3EBFF               CALL PathOfEx.0046BE70
$-88     005B0B68     53                        PUSH EBX
$-87     005B0B69     8BC8                      MOV ECX,EAX
$-85     005B0B6B     8B10                      MOV EDX,DWORD PTR DS:[EAX]
$-83     005B0B6D     FF52 38                   CALL DWORD PTR DS:[EDX+0x38]
$-80     005B0B70     8BBE F0080000             MOV EDI,DWORD PTR DS:[ESI+0x8F0]
$-7A     005B0B76     85FF                      TEST EDI,EDI
$-78     005B0B78     74 15                     JE SHORT PathOfEx.005B0B8F
$-76     005B0B7A     8BCF                      MOV ECX,EDI
$-74     005B0B7C     E8 0FE0FFFF               CALL PathOfEx.005AEB90
$-6F     005B0B81     68 F0020000               PUSH 0x2F0
$-6A     005B0B86     57                        PUSH EDI
$-69     005B0B87     E8 8BDE6C00               CALL PathOfEx.00C7EA17
$-64     005B0B8C     83C4 08                   ADD ESP,0x8
$-61     005B0B8F     8B86 EC080000             MOV EAX,DWORD PTR DS:[ESI+0x8EC]
$-5B     005B0B95     8DBE D8080000             LEA EDI,DWORD PTR DS:[ESI+0x8D8]
$-55     005B0B9B     83F8 08                   CMP EAX,0x8
$-52     005B0B9E     72 0E                     JB SHORT PathOfEx.005B0BAE
$-50     005B0BA0     6A 02                     PUSH 0x2
$-4E     005B0BA2     40                        INC EAX
$-4D     005B0BA3     50                        PUSH EAX
$-4C     005B0BA4     FF37                      PUSH DWORD PTR DS:[EDI]
$-4A     005B0BA6     E8 85BCEBFF               CALL PathOfEx.0046C830
$-45     005B0BAB     83C4 0C                   ADD ESP,0xC
$-42     005B0BAE     C747 14 07000000          MOV DWORD PTR DS:[EDI+0x14],0x7
$-3B     005B0BB5     837F 14 08                CMP DWORD PTR DS:[EDI+0x14],0x8
$-37     005B0BB9     C747 10 00000000          MOV DWORD PTR DS:[EDI+0x10],0x0
$-30     005B0BC0     72 02                     JB SHORT PathOfEx.005B0BC4
$-2E     005B0BC2     8B3F                      MOV EDI,DWORD PTR DS:[EDI]
$-2C     005B0BC4     33C0                      XOR EAX,EAX
$-2A     005B0BC6     8BCE                      MOV ECX,ESI
$-28     005B0BC8     66:8907                   MOV WORD PTR DS:[EDI],AX
$-25     005B0BCB     E8 501CF0FF               CALL PathOfEx.004B2820
$-20     005B0BD0     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1D     005B0BD3     5F                        POP EDI
$-1C     005B0BD4     5E                        POP ESI
$-1B     005B0BD5     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-14     005B0BDC     5B                        POP EBX
$-13     005B0BDD     8BE5                      MOV ESP,EBP
$-11     005B0BDF     5D                        POP EBP
$-10     005B0BE0     C3                        RETN
$-F      005B0BE1     CC                        INT3
$-E      005B0BE2     CC                        INT3
$-D      005B0BE3     CC                        INT3
$-C      005B0BE4     CC                        INT3
$-B      005B0BE5     CC                        INT3
$-A      005B0BE6     CC                        INT3
$-9      005B0BE7     CC                        INT3
$-8      005B0BE8     CC                        INT3
$-7      005B0BE9     CC                        INT3
$-6      005B0BEA     CC                        INT3
$-5      005B0BEB     CC                        INT3
$-4      005B0BEC     CC                        INT3
$-3      005B0BED     CC                        INT3
$-2      005B0BEE     CC                        INT3
$-1      005B0BEF     CC                        INT3
$ ==>    005B0BF0     55                        PUSH EBP                                         ; tag_生成物品属性格式化字符串
$+1      005B0BF1     8BEC                      MOV EBP,ESP                                      ; 返回const std::wstring*
$+3      005B0BF3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]                         ; ecx=stCD_UiItemTips*
$+9      005B0BF9     6A FF                     PUSH -0x1
$+B      005B0BFB     68 5055D300               PUSH PathOfEx.00D35550
$+10     005B0C00     50                        PUSH EAX
$+11     005B0C01     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     005B0C08     83EC 4C                   SUB ESP,0x4C
$+1B     005B0C0B     56                        PUSH ESI
$+1C     005B0C0C     57                        PUSH EDI
$+1D     005B0C0D     8BF9                      MOV EDI,ECX
$+1F     005B0C0F     83BF E8080000 00          CMP DWORD PTR DS:[EDI+0x8E8],0x0
$+26     005B0C16     8DB7 D8080000             LEA ESI,DWORD PTR DS:[EDI+0x8D8]                 ; stCD_UiItemTips.tmp_format_attr_str_
$+2C     005B0C1C     75 77                     JNZ SHORT PathOfEx.005B0C95
$+2E     005B0C1E     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18],0x0
$+35     005B0C25     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
$+3C     005B0C2C     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
$+43     005B0C33     8D4D A8                   LEA ECX,DWORD PTR SS:[EBP-0x58]
$+46     005B0C36     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+4D     005B0C3D     E8 DEE64500               CALL PathOfEx.00A0F320
$+52     005B0C42     8BCF                      MOV ECX,EDI
$+54     005B0C44     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+58     005B0C48     E8 83000000               CALL PathOfEx.005B0CD0
$+5D     005B0C4D     8BC8                      MOV ECX,EAX										;eax=stCD_ItemTipsParams*
$+5F     005B0C4F     E8 AC981700               CALL PathOfEx.0072A500
$+64     005B0C54     50                        PUSH EAX
$+65     005B0C55     56                        PUSH ESI
$+66     005B0C56     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
$+69     005B0C59     50                        PUSH EAX
$+6A     005B0C5A     8D4D A8                   LEA ECX,DWORD PTR SS:[EBP-0x58]
$+6D     005B0C5D     E8 EE40F0FF               CALL PathOfEx.004B4D50
$+72     005B0C62     8B45 C0                   MOV EAX,DWORD PTR SS:[EBP-0x40]
$+75     005B0C65     83F8 08                   CMP EAX,0x8
$+78     005B0C68     72 0F                     JB SHORT PathOfEx.005B0C79
$+7A     005B0C6A     6A 02                     PUSH 0x2
$+7C     005B0C6C     40                        INC EAX
$+7D     005B0C6D     50                        PUSH EAX
$+7E     005B0C6E     FF75 AC                   PUSH DWORD PTR SS:[EBP-0x54]
$+81     005B0C71     E8 BABBEBFF               CALL PathOfEx.0046C830
$+86     005B0C76     83C4 0C                   ADD ESP,0xC
$+89     005B0C79     33C0                      XOR EAX,EAX
$+8B     005B0C7B     C745 C0 07000000          MOV DWORD PTR SS:[EBP-0x40],0x7
$+92     005B0C82     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
$+95     005B0C85     C745 BC 00000000          MOV DWORD PTR SS:[EBP-0x44],0x0
$+9C     005B0C8C     66:8945 AC                MOV WORD PTR SS:[EBP-0x54],AX
$+A0     005B0C90     E8 BBCCF0FF               CALL PathOfEx.004BD950
$+A5     005B0C95     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+A8     005B0C98     8BC6                      MOV EAX,ESI
$+AA     005B0C9A     5F                        POP EDI
$+AB     005B0C9B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+B2     005B0CA2     5E                        POP ESI
$+B3     005B0CA3     8BE5                      MOV ESP,EBP
$+B5     005B0CA5     5D                        POP EBP
$+B6     005B0CA6     C3                        RETN
$+B7     005B0CA7     CC                        INT3
$+B8     005B0CA8     CC                        INT3
$+B9     005B0CA9     CC                        INT3
$+BA     005B0CAA     CC                        INT3
$+BB     005B0CAB     CC                        INT3
$+BC     005B0CAC     CC                        INT3
$+BD     005B0CAD     CC                        INT3
$+BE     005B0CAE     CC                        INT3
$+BF     005B0CAF     CC                        INT3
$+C0     005B0CB0     51                        PUSH ECX
$+C1     005B0CB1     E8 3AFFFFFF               CALL PathOfEx.005B0BF0
$+C6     005B0CB6     8378 14 08                CMP DWORD PTR DS:[EAX+0x14],0x8
$+CA     005B0CBA     8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
$+CD     005B0CBD     72 02                     JB SHORT PathOfEx.005B0CC1
$+CF     005B0CBF     8B00                      MOV EAX,DWORD PTR DS:[EAX]
$+D1     005B0CC1     8BC8                      MOV ECX,EAX
$+D3     005B0CC3     E8 F8441600               CALL PathOfEx.007151C0
$+D8     005B0CC8     59                        POP ECX
$+D9     005B0CC9     C3                        RETN
$+DA     005B0CCA     CC                        INT3
$+DB     005B0CCB     CC                        INT3
$+DC     005B0CCC     CC                        INT3
$+DD     005B0CCD     CC                        INT3
$+DE     005B0CCE     CC                        INT3
$+DF     005B0CCF     CC                        INT3                                             ; g_c_ui_create_item_tips_
$+E0     005B0CD0     83EC 08                   SUB ESP,0x8                                      ; tag_ui创建物品tips时会断到,tag_无参创建物品tips
$+E3     005B0CD3     53                        PUSH EBX                                         ; ecx=stCD_UiItemTips*
$+E4     005B0CD4     8BD9                      MOV EBX,ECX
$+E6     005B0CD6     56                        PUSH ESI
$+E7     005B0CD7     57                        PUSH EDI
$+E8     005B0CD8     F683 C0080000 04          TEST BYTE PTR DS:[EBX+0x8C0],0x4
$+EF     005B0CDF     75 0D                     JNZ SHORT PathOfEx.005B0CEE
$+F1     005B0CE1     8B83 F0080000             MOV EAX,DWORD PTR DS:[EBX+0x8F0]
$+F7     005B0CE7     5F                        POP EDI
$+F8     005B0CE8     5E                        POP ESI
$+F9     005B0CE9     5B                        POP EBX
$+FA     005B0CEA     83C4 08                   ADD ESP,0x8
$+FD     005B0CED     C3                        RETN
$+FE     005B0CEE     83BB CC080000 00          CMP DWORD PTR DS:[EBX+0x8CC],0x0                 ; g_o_items_tips2item_
$+105    005B0CF5     74 47                     JE SHORT PathOfEx.005B0D3E
$+107    005B0CF7     8BBB C8080000             MOV EDI,DWORD PTR DS:[EBX+0x8C8]
$+10D    005B0CFD     8BCF                      MOV ECX,EDI
$+10F    005B0CFF     8B77 40                   MOV ESI,DWORD PTR DS:[EDI+0x40]
$+112    005B0D02     E8 E97C2300               CALL PathOfEx.007E89F0
$+117    005B0D07     8B83 C0080000             MOV EAX,DWORD PTR DS:[EBX+0x8C0]
$+11D    005B0D0D     B9 F03A0101               MOV ECX,PathOfEx.01013AF0                        ; g_a_item_tips_params_
$+122    005B0D12     FFB3 D4080000             PUSH DWORD PTR DS:[EBX+0x8D4]                    ; push 0
$+128    005B0D18     D1E8                      SHR EAX,1
$+12A    005B0D1A     24 01                     AND AL,0x1
$+12C    005B0D1C     0FB6C0                    MOVZX EAX,AL
$+12F    005B0D1F     50                        PUSH EAX                                         ; push 0
$+130    005B0D20     56                        PUSH ESI                                         ; push 3f
$+131    005B0D21     8D47 34                   LEA EAX,DWORD PTR DS:[EDI+0x34]
$+134    005B0D24     50                        PUSH EAX
$+135    005B0D25     6A 00                     PUSH 0x0
$+137    005B0D27     FFB3 CC080000             PUSH DWORD PTR DS:[EBX+0x8CC]                    ; push stCD_EntityItemBased*,g_o_items_tips2item_
$+13D    005B0D2D     E8 1E2F1700               CALL PathOfEx.00723C50                           ; call tag_索引物品属性并创建tips
$+142    005B0D32     B8 F03A0101               MOV EAX,PathOfEx.01013AF0                        ; g_a_item_tips_params_
$+147    005B0D37     5F                        POP EDI
$+148    005B0D38     5E                        POP ESI
$+149    005B0D39     5B                        POP EBX
$+14A    005B0D3A     83C4 08                   ADD ESP,0x8
$+14D    005B0D3D     C3                        RETN
$+14E    005B0D3E     8B83 D0080000             MOV EAX,DWORD PTR DS:[EBX+0x8D0]
$+154    005B0D44     8B48 04                   MOV ECX,DWORD PTR DS:[EAX+0x4]
$+157    005B0D47     E8 84F8EBFF               CALL PathOfEx.004705D0
$+15C    005B0D4C     85C0                      TEST EAX,EAX
$+15E    005B0D4E     74 0D                     JE SHORT PathOfEx.005B0D5D
$+160    005B0D50     8378 6C 08                CMP DWORD PTR DS:[EAX+0x6C],0x8
$+164    005B0D54     8D48 58                   LEA ECX,DWORD PTR DS:[EAX+0x58]
$+167    005B0D57     72 09                     JB SHORT PathOfEx.005B0D62
$+169    005B0D59     8B09                      MOV ECX,DWORD PTR DS:[ECX]
$+16B    005B0D5B     EB 05                     JMP SHORT PathOfEx.005B0D62
$+16D    005B0D5D     B9 BC2CE700               MOV ECX,PathOfEx.00E72CBC
$+172    005B0D62     8B93 D0080000             MOV EDX,DWORD PTR DS:[EBX+0x8D0]
$+178    005B0D68     8B82 10010000             MOV EAX,DWORD PTR DS:[EDX+0x110]
$+17E    005B0D6E     85C0                      TEST EAX,EAX
$+180    005B0D70     74 05                     JE SHORT PathOfEx.005B0D77
$+182    005B0D72     8B70 08                   MOV ESI,DWORD PTR DS:[EAX+0x8]
$+185    005B0D75     EB 02                     JMP SHORT PathOfEx.005B0D79
$+187    005B0D77     33F6                      XOR ESI,ESI
$+189    005B0D79     8A82 20010000             MOV AL,BYTE PTR DS:[EDX+0x120]
$+18F    005B0D7F     83C2 10                   ADD EDX,0x10
$+192    005B0D82     884424 10                 MOV BYTE PTR SS:[ESP+0x10],AL
$+196    005B0D86     56                        PUSH ESI
$+197    005B0D87     FF7424 14                 PUSH DWORD PTR SS:[ESP+0x14]
$+19B    005B0D8B     E8 E0F9FFFF               CALL PathOfEx.005B0770
$+1A0    005B0D90     83C4 08                   ADD ESP,0x8
$+1A3    005B0D93     5F                        POP EDI
$+1A4    005B0D94     5E                        POP ESI
$+1A5    005B0D95     5B                        POP EBX
$+1A6    005B0D96     83C4 08                   ADD ESP,0x8
$+1A9    005B0D99     C3                        RETN
$+1AA    005B0D9A     CC                        INT3
*/
//tag_查找ClientString并返回std::wstring
/*
$-A1     0071D42F     CC                       INT3
$-A0     0071D430     83EC 0C                  SUB ESP,0xC
$-9D     0071D433     56                       PUSH ESI
$-9C     0071D434     8BF1                     MOV ESI,ECX
$-9A     0071D436     0FB7D2                   MOVZX EDX,DX
$-97     0071D439     57                       PUSH EDI
$-96     0071D43A     897424 0C                MOV DWORD PTR SS:[ESP+0xC],ESI
$-92     0071D43E     C74424 10 00000000       MOV DWORD PTR SS:[ESP+0x10],0x0
$-8A     0071D446     E8 45050000              CALL PathOfEx.0071D990
$-85     0071D44B     8BD0                     MOV EDX,EAX
$-83     0071D44D     C746 14 07000000         MOV DWORD PTR DS:[ESI+0x14],0x7
$-7C     0071D454     33C0                     XOR EAX,EAX
$-7A     0071D456     C746 10 00000000         MOV DWORD PTR DS:[ESI+0x10],0x0
$-73     0071D45D     66:8906                  MOV WORD PTR DS:[ESI],AX
$-70     0071D460     66:3902                  CMP WORD PTR DS:[EDX],AX
$-6D     0071D463     75 13                    JNZ SHORT PathOfEx.0071D478
$-6B     0071D465     33C9                     XOR ECX,ECX
$-69     0071D467     51                       PUSH ECX
$-68     0071D468     52                       PUSH EDX
$-67     0071D469     8BCE                     MOV ECX,ESI
$-65     0071D46B     E8 4008D5FF              CALL PathOfEx.0046DCB0
$-60     0071D470     8BC6                     MOV EAX,ESI
$-5E     0071D472     5F                       POP EDI
$-5D     0071D473     5E                       POP ESI
$-5C     0071D474     83C4 0C                  ADD ESP,0xC
$-59     0071D477     C3                       RETN
$-58     0071D478     8BCA                     MOV ECX,EDX
$-56     0071D47A     8D79 02                  LEA EDI,DWORD PTR DS:[ECX+0x2]
$-53     0071D47D     0F1F00                   NOP DWORD PTR DS:[EAX]
$-50     0071D480     66:8B01                  MOV AX,WORD PTR DS:[ECX]
$-4D     0071D483     83C1 02                  ADD ECX,0x2
$-4A     0071D486     66:85C0                  TEST AX,AX
$-47     0071D489   ^ 75 F5                    JNZ SHORT PathOfEx.0071D480
$-45     0071D48B     2BCF                     SUB ECX,EDI
$-43     0071D48D     D1F9                     SAR ECX,1
$-41     0071D48F     51                       PUSH ECX
$-40     0071D490     52                       PUSH EDX
$-3F     0071D491     8BCE                     MOV ECX,ESI
$-3D     0071D493     E8 1808D5FF              CALL PathOfEx.0046DCB0
$-38     0071D498     5F                       POP EDI
$-37     0071D499     8BC6                     MOV EAX,ESI
$-35     0071D49B     5E                       POP ESI
$-34     0071D49C     83C4 0C                  ADD ESP,0xC
$-31     0071D49F     C3                       RETN
$-30     0071D4A0     83EC 0C                  SUB ESP,0xC
$-2D     0071D4A3     56                       PUSH ESI
$-2C     0071D4A4     FF7424 14                PUSH DWORD PTR SS:[ESP+0x14]
$-28     0071D4A8     0FB7C2                   MOVZX EAX,DX
$-25     0071D4AB     8BF1                     MOV ESI,ECX
$-23     0071D4AD     50                       PUSH EAX
$-22     0071D4AE     897424 10                MOV DWORD PTR SS:[ESP+0x10],ESI
$-1E     0071D4B2     C74424 14 00000000       MOV DWORD PTR SS:[ESP+0x14],0x0
$-16     0071D4BA     E8 D1050000              CALL PathOfEx.0071DA90
$-11     0071D4BF     83C4 08                  ADD ESP,0x8
$-E      0071D4C2     8BC6                     MOV EAX,ESI
$-C      0071D4C4     5E                       POP ESI
$-B      0071D4C5     83C4 0C                  ADD ESP,0xC
$-8      0071D4C8     C3                       RETN
$-7      0071D4C9     CC                       INT3
$-6      0071D4CA     CC                       INT3
$-5      0071D4CB     CC                       INT3
$-4      0071D4CC     CC                       INT3
$-3      0071D4CD     CC                       INT3
$-2      0071D4CE     CC                       INT3
$-1      0071D4CF     CC                       INT3
$ ==>    0071D4D0     83EC 0C                  SUB ESP,0xC                                      ; tag_查找ClientString并返回std::wstring
$+3      0071D4D3     56                       PUSH ESI                                         ; 参数edx=stCD_TblItemClientStrings的数组索引
$+4      0071D4D4     8BF1                     MOV ESI,ECX
$+6      0071D4D6     C74424 0C 00000000       MOV DWORD PTR SS:[ESP+0xC],0x0
$+E      0071D4DE     57                       PUSH EDI
$+F      0071D4DF     B9 3018E700              MOV ECX,PathOfEx.00E71830                        ; UNICODE "Data/ClientStrings.dat"
$+14     0071D4E4     897424 0C                MOV DWORD PTR SS:[ESP+0xC],ESI
$+18     0071D4E8     E8 F30F0000              CALL PathOfEx.0071E4E0                           ; tag_查找ClientString中文字符串
$+1D     0071D4ED     8BD0                     MOV EDX,EAX
$+1F     0071D4EF     C746 14 07000000         MOV DWORD PTR DS:[ESI+0x14],0x7
$+26     0071D4F6     33C0                     XOR EAX,EAX
$+28     0071D4F8     C746 10 00000000         MOV DWORD PTR DS:[ESI+0x10],0x0
$+2F     0071D4FF     66:8906                  MOV WORD PTR DS:[ESI],AX
$+32     0071D502     66:3902                  CMP WORD PTR DS:[EDX],AX
$+35     0071D505     75 13                    JNZ SHORT PathOfEx.0071D51A
$+37     0071D507     33C9                     XOR ECX,ECX
$+39     0071D509     51                       PUSH ECX
$+3A     0071D50A     52                       PUSH EDX
$+3B     0071D50B     8BCE                     MOV ECX,ESI
$+3D     0071D50D     E8 9E07D5FF              CALL PathOfEx.0046DCB0
$+42     0071D512     8BC6                     MOV EAX,ESI
$+44     0071D514     5F                       POP EDI
$+45     0071D515     5E                       POP ESI
$+46     0071D516     83C4 0C                  ADD ESP,0xC
$+49     0071D519     C3                       RETN
$+4A     0071D51A     8BCA                     MOV ECX,EDX
$+4C     0071D51C     8D79 02                  LEA EDI,DWORD PTR DS:[ECX+0x2]
$+4F     0071D51F     90                       NOP
$+50     0071D520     66:8B01                  MOV AX,WORD PTR DS:[ECX]
$+53     0071D523     83C1 02                  ADD ECX,0x2
$+56     0071D526     66:85C0                  TEST AX,AX
$+59     0071D529   ^ 75 F5                    JNZ SHORT PathOfEx.0071D520
$+5B     0071D52B     2BCF                     SUB ECX,EDI
$+5D     0071D52D     D1F9                     SAR ECX,1
$+5F     0071D52F     51                       PUSH ECX
$+60     0071D530     52                       PUSH EDX
$+61     0071D531     8BCE                     MOV ECX,ESI
$+63     0071D533     E8 7807D5FF              CALL PathOfEx.0046DCB0
$+68     0071D538     5F                       POP EDI
$+69     0071D539     8BC6                     MOV EAX,ESI
$+6B     0071D53B     5E                       POP ESI
$+6C     0071D53C     83C4 0C                  ADD ESP,0xC
$+6F     0071D53F     C3                       RETN
$+70     0071D540     55                       PUSH EBP
$+71     0071D541     8BEC                     MOV EBP,ESP
$+73     0071D543     83E4 F8                  AND ESP,0xFFFFFFF8
$+76     0071D546     83EC 0C                  SUB ESP,0xC
$+79     0071D549     56                       PUSH ESI
$+7A     0071D54A     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]
$+7D     0071D54D     8BF1                     MOV ESI,ECX
$+7F     0071D54F     C74424 10 00000000       MOV DWORD PTR SS:[ESP+0x10],0x0
$+87     0071D557     52                       PUSH EDX
$+88     0071D558     897424 10                MOV DWORD PTR SS:[ESP+0x10],ESI
$+8C     0071D55C     E8 7F100000              CALL PathOfEx.0071E5E0
$+91     0071D561     83C4 08                  ADD ESP,0x8
$+94     0071D564     8BC6                     MOV EAX,ESI
$+96     0071D566     5E                       POP ESI
$+97     0071D567     8BE5                     MOV ESP,EBP
$+99     0071D569     5D                       POP EBP
$+9A     0071D56A     C3                       RETN
$+9B     0071D56B     CC                       INT3
$+9C     0071D56C     CC                       INT3
$+9D     0071D56D     CC                       INT3
$+9E     0071D56E     CC                       INT3
$+9F     0071D56F     CC                       INT3
$+A0     0071D570     55                       PUSH EBP
$+A1     0071D571     8BEC                     MOV EBP,ESP
$+A3     0071D573     6A FF                    PUSH -0x1
$+A5     0071D575     68 5C89D400              PUSH PathOfEx.00D4895C
$+AA     0071D57A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+B0     0071D580     50                       PUSH EAX
$+B1     0071D581     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+B8     0071D588     83EC 48                  SUB ESP,0x48
$+BB     0071D58B     56                       PUSH ESI
$+BC     0071D58C     57                       PUSH EDI
$+BD     0071D58D     83EC 10                  SUB ESP,0x10
$+C0     0071D590     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
$+C7     0071D597     8BF9                     MOV EDI,ECX
$+C9     0071D599     897D EC                  MOV DWORD PTR SS:[EBP-0x14],EDI
$+CC     0071D59C     E8 5FAAFFFF              CALL PathOfEx.00718000
$+D1     0071D5A1     8BC8                     MOV ECX,EAX
$+D3     0071D5A3     E8 2882D6FF              CALL PathOfEx.004857D0
$+D8     0071D5A8     8BF0                     MOV ESI,EAX
$+DA     0071D5AA     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
$+DD     0071D5AD     6A 00                    PUSH 0x0
$+DF     0071D5AF     8D55 EC                  LEA EDX,DWORD PTR SS:[EBP-0x14]
$+E2     0071D5B2     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+E9     0071D5B9     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
$+EC     0071D5BC     E8 DF070100              CALL PathOfEx.0072DDA0
$+F1     0071D5C1     83C4 04                  ADD ESP,0x4
$+F4     0071D5C4     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$+F7     0071D5C7     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+FB     0071D5CB     51                       PUSH ECX
$+FC     0071D5CC     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
$+FF     0071D5CF     E8 FC8EDCFF              CALL PathOfEx.004E64D0
$+104    0071D5D4     68 88F5EA00              PUSH PathOfEx.00EAF588                           ; UNICODE ":Data/ClientStrings.dat"
$+109    0071D5D9     8BD0                     MOV EDX,EAX
$+10B    0071D5DB     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+10F    0071D5DF     8D4D B0                  LEA ECX,DWORD PTR SS:[EBP-0x50]
$+112    0071D5E2     E8 39FCD5FF              CALL PathOfEx.0047D220
$+117    0071D5E7     83C4 04                  ADD ESP,0x4
$+11A    0071D5EA     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
$+11D    0071D5ED     83F8 08                  CMP EAX,0x8
$+120    0071D5F0     72 0F                    JB SHORT PathOfEx.0071D601
$+122    0071D5F2     40                       INC EAX
$+123    0071D5F3     6A 02                    PUSH 0x2
$+125    0071D5F5     50                       PUSH EAX
$+126    0071D5F6     FF75 C8                  PUSH DWORD PTR SS:[EBP-0x38]
$+129    0071D5F9     E8 32F2D4FF              CALL PathOfEx.0046C830
$+12E    0071D5FE     83C4 0C                  ADD ESP,0xC
$+131    0071D601     33C0                     XOR EAX,EAX
$+133    0071D603     C745 DC 07000000         MOV DWORD PTR SS:[EBP-0x24],0x7
$+13A    0071D60A     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
$+141    0071D611     66:8945 C8               MOV WORD PTR SS:[EBP-0x38],AX
$+145    0071D615     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
$+149    0071D619     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
$+14C    0071D61C     85C0                     TEST EAX,EAX
$+14E    0071D61E     74 18                    JE SHORT PathOfEx.0071D638
$+150    0071D620     50                       PUSH EAX
$+151    0071D621     51                       PUSH ECX
$+152    0071D622     E8 D9A9FFFF              CALL PathOfEx.00718000
$+157    0071D627     83C4 04                  ADD ESP,0x4
$+15A    0071D62A     8BC8                     MOV ECX,EAX
$+15C    0071D62C     E8 5FA5FFFF              CALL PathOfEx.00717B90
$+161    0071D631     C745 E0 00000000         MOV DWORD PTR SS:[EBP-0x20],0x0
$+168    0071D638     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
$+16C    0071D63C     85F6                     TEST ESI,ESI
$+16E    0071D63E     74 11                    JE SHORT PathOfEx.0071D651
$+170    0071D640     56                       PUSH ESI
$+171    0071D641     51                       PUSH ECX
$+172    0071D642     E8 B9A9FFFF              CALL PathOfEx.00718000
$+177    0071D647     83C4 04                  ADD ESP,0x4
$+17A    0071D64A     8BC8                     MOV ECX,EAX
$+17C    0071D64C     E8 3FA5FFFF              CALL PathOfEx.00717B90
$+181    0071D651     8B55 08                  MOV EDX,DWORD PTR SS:[EBP+0x8]
$+184    0071D654     8D4D B0                  LEA ECX,DWORD PTR SS:[EBP-0x50]
$+187    0071D657     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
$+18B    0071D65B     837D C4 08               CMP DWORD PTR SS:[EBP-0x3C],0x8
$+18F    0071D65F     0F434D B0                CMOVNB ECX,DWORD PTR SS:[EBP-0x50]
$+193    0071D663     E8 780E0000              CALL PathOfEx.0071E4E0
$+198    0071D668     8BD0                     MOV EDX,EAX
$+19A    0071D66A     C747 14 07000000         MOV DWORD PTR DS:[EDI+0x14],0x7
$+1A1    0071D671     33C0                     XOR EAX,EAX
$+1A3    0071D673     C747 10 00000000         MOV DWORD PTR DS:[EDI+0x10],0x0
$+1AA    0071D67A     66:8907                  MOV WORD PTR DS:[EDI],AX
$+1AD    0071D67D     66:3902                  CMP WORD PTR DS:[EDX],AX
$+1B0    0071D680     75 04                    JNZ SHORT PathOfEx.0071D686
$+1B2    0071D682     33C9                     XOR ECX,ECX
$+1B4    0071D684     EB 19                    JMP SHORT PathOfEx.0071D69F
$+1B6    0071D686     8BCA                     MOV ECX,EDX
$+1B8    0071D688     8D71 02                  LEA ESI,DWORD PTR DS:[ECX+0x2]
$+1BB    0071D68B     0F1F4400 00              NOP DWORD PTR DS:[EAX+EAX]
$+1C0    0071D690     66:8B01                  MOV AX,WORD PTR DS:[ECX]
$+1C3    0071D693     83C1 02                  ADD ECX,0x2
$+1C6    0071D696     66:85C0                  TEST AX,AX
$+1C9    0071D699   ^ 75 F5                    JNZ SHORT PathOfEx.0071D690
$+1CB    0071D69B     2BCE                     SUB ECX,ESI
$+1CD    0071D69D     D1F9                     SAR ECX,1
$+1CF    0071D69F     51                       PUSH ECX
$+1D0    0071D6A0     52                       PUSH EDX
$+1D1    0071D6A1     8BCF                     MOV ECX,EDI
$+1D3    0071D6A3     E8 0806D5FF              CALL PathOfEx.0046DCB0
$+1D8    0071D6A8     8B45 C4                  MOV EAX,DWORD PTR SS:[EBP-0x3C]
$+1DB    0071D6AB     83F8 08                  CMP EAX,0x8
$+1DE    0071D6AE     72 0F                    JB SHORT PathOfEx.0071D6BF
$+1E0    0071D6B0     6A 02                    PUSH 0x2
$+1E2    0071D6B2     40                       INC EAX
$+1E3    0071D6B3     50                       PUSH EAX
$+1E4    0071D6B4     FF75 B0                  PUSH DWORD PTR SS:[EBP-0x50]
$+1E7    0071D6B7     E8 74F1D4FF              CALL PathOfEx.0046C830
$+1EC    0071D6BC     83C4 0C                  ADD ESP,0xC
$+1EF    0071D6BF     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1F2    0071D6C2     8BC7                     MOV EAX,EDI
$+1F4    0071D6C4     5F                       POP EDI
$+1F5    0071D6C5     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+1FC    0071D6CC     5E                       POP ESI
$+1FD    0071D6CD     8BE5                     MOV ESP,EBP
$+1FF    0071D6CF     5D                       POP EBP
$+200    0071D6D0     C3                       RETN
$+201    0071D6D1     CC                       INT3
*/
//tag_查找ClientString中文字符串
/*
$-281    0071E25F     8B7D E4                  MOV EDI,DWORD PTR SS:[EBP-0x1C]
$-27E    0071E262     8B35 C8E0D800            MOV ESI,DWORD PTR DS:[0xD8E0C8]                  ; kernel32.GetTickCount
$-278    0071E268     47                       INC EDI
$-277    0071E269     8B55 0C                  MOV EDX,DWORD PTR SS:[EBP+0xC]
$-274    0071E26C   ^ E9 F7FAFFFF              JMP PathOfEx.0071DD68
$-26F    0071E271     8B42 0C                  MOV EAX,DWORD PTR DS:[EDX+0xC]
$-26C    0071E274     894D E4                  MOV DWORD PTR SS:[EBP-0x1C],ECX
$-269    0071E277     833CC8 00                CMP DWORD PTR DS:[EAX+ECX*8],0x0
$-265    0071E27B     8B54C8 04                MOV EDX,DWORD PTR DS:[EAX+ECX*8+0x4]
$-261    0071E27F     0F84 A3000000            JE PathOfEx.0071E328
$-25B    0071E285     83EC 08                  SUB ESP,0x8
$-258    0071E288     8D8D 40FCFFFF            LEA ECX,DWORD PTR SS:[EBP-0x3C0]
$-252    0071E28E     E8 FDB4D5FF              CALL PathOfEx.00479790
$-24D    0071E293     8D85 50FCFFFF            LEA EAX,DWORD PTR SS:[EBP-0x3B0]
$-247    0071E299     C645 FC 26               MOV BYTE PTR SS:[EBP-0x4],0x26
$-243    0071E29D     68 F0F4EA00              PUSH PathOfEx.00EAF4F0                           ; UNICODE "Received StatDescription ClientString or BackendEr"
$-23E    0071E2A2     50                       PUSH EAX
$-23D    0071E2A3     E8 68A1D6FF              CALL PathOfEx.00488410
$-238    0071E2A8     83C4 08                  ADD ESP,0x8
$-235    0071E2AB     8D45 A8                  LEA EAX,DWORD PTR SS:[EBP-0x58]
$-232    0071E2AE     8D8D 40FCFFFF            LEA ECX,DWORD PTR SS:[EBP-0x3C0]
$-22C    0071E2B4     50                       PUSH EAX
$-22B    0071E2B5     E8 36A1D6FF              CALL PathOfEx.004883F0
$-226    0071E2BA     8BF8                     MOV EDI,EAX
$-224    0071E2BC     C645 FC 27               MOV BYTE PTR SS:[EBP-0x4],0x27
$-220    0071E2C0     FFD6                     CALL ESI
$-21E    0071E2C2     8BF0                     MOV ESI,EAX
$-21C    0071E2C4     8D85 60FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xA0]
$-216    0071E2CA     50                       PUSH EAX
$-215    0071E2CB     E8 F7CA5800              CALL PathOfEx.00CAADC7
$-210    0071E2D0     64:8B0D 2C000000         MOV ECX,DWORD PTR FS:[0x2C]
$-209    0071E2D7     83C4 04                  ADD ESP,0x4
$-206    0071E2DA     8B09                     MOV ECX,DWORD PTR DS:[ECX]
$-204    0071E2DC     57                       PUSH EDI
$-203    0071E2DD     83EC 0C                  SUB ESP,0xC
$-200    0071E2E0     8B49 18                  MOV ECX,DWORD PTR DS:[ECX+0x18]
$-1FD    0071E2E3     6A 02                    PUSH 0x2
$-1FB    0071E2E5     56                       PUSH ESI
$-1FA    0071E2E6     FFB5 64FFFFFF            PUSH DWORD PTR SS:[EBP-0x9C]
$-1F4    0071E2EC     FFB5 60FFFFFF            PUSH DWORD PTR SS:[EBP-0xA0]
$-1EE    0071E2F2     68 8B000000              PUSH 0x8B
$-1E9    0071E2F7     E8 949DD6FF              CALL PathOfEx.00488090
$-1E4    0071E2FC     8D4D A8                  LEA ECX,DWORD PTR SS:[EBP-0x58]
$-1E1    0071E2FF     E8 3CF4D4FF              CALL PathOfEx.0046D740
$-1DC    0071E304     8D8D A8FCFFFF            LEA ECX,DWORD PTR SS:[EBP-0x358]
$-1D6    0071E30A     E8 618AD5FF              CALL PathOfEx.00476D70
$-1D1    0071E30F     C645 FC 28               MOV BYTE PTR SS:[EBP-0x4],0x28
$-1CD    0071E313     8D85 A8FCFFFF            LEA EAX,DWORD PTR SS:[EBP-0x358]
$-1C7    0071E319     C785 A8FCFFFF C453DE00   MOV DWORD PTR SS:[EBP-0x358],PathOfEx.00DE53C4
$-1BD    0071E323   ^ E9 1FFFFFFF              JMP PathOfEx.0071E247
$-1B8    0071E328     52                       PUSH EDX
$-1B7    0071E329     8B57 04                  MOV EDX,DWORD PTR DS:[EDI+0x4]
$-1B4    0071E32C     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$-1B1    0071E32F     E8 7C4A0500              CALL PathOfEx.00772DB0
$-1AC    0071E334     83C4 04                  ADD ESP,0x4
$-1A9    0071E337     C645 FC 29               MOV BYTE PTR SS:[EBP-0x4],0x29
$-1A5    0071E33B     8D55 C8                  LEA EDX,DWORD PTR SS:[EBP-0x38]
$-1A2    0071E33E     8D8D 98FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x168]
$-19C    0071E344     E8 C76EE5FF              CALL PathOfEx.00575210
$-197    0071E349     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$-194    0071E34C     C645 FC 0B               MOV BYTE PTR SS:[EBP-0x4],0xB
$-190    0071E350     E8 EBF3D4FF              CALL PathOfEx.0046D740
$-18B    0071E355     8B55 0C                  MOV EDX,DWORD PTR SS:[EBP+0xC]
$-188    0071E358     8B7D E4                  MOV EDI,DWORD PTR SS:[EBP-0x1C]
$-185    0071E35B     47                       INC EDI
$-184    0071E35C   ^ E9 07FAFFFF              JMP PathOfEx.0071DD68
$-17F    0071E361     8B75 E0                  MOV ESI,DWORD PTR SS:[EBP-0x20]
$-17C    0071E364     8D8D 98FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x168]
$-176    0071E36A     56                       PUSH ESI
$-175    0071E36B     E8 9057E5FF              CALL PathOfEx.00573B00
$-170    0071E370     8D8D 98FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x168]
$-16A    0071E376     E8 2529E5FF              CALL PathOfEx.00570CA0
$-165    0071E37B     C645 FC 2A               MOV BYTE PTR SS:[EBP-0x4],0x2A
$-161    0071E37F     8B7D C4                  MOV EDI,DWORD PTR SS:[EBP-0x3C]
$-15E    0071E382     85FF                     TEST EDI,EDI
$-15C    0071E384     74 0D                    JE SHORT PathOfEx.0071E393
$-15A    0071E386     E8 759CFFFF              CALL PathOfEx.00718000
$-155    0071E38B     57                       PUSH EDI
$-154    0071E38C     8BC8                     MOV ECX,EAX
$-152    0071E38E     E8 FD97FFFF              CALL PathOfEx.00717B90
$-14D    0071E393     C745 FC 2B000000         MOV DWORD PTR SS:[EBP-0x4],0x2B
$-146    0071E39A     85DB                     TEST EBX,EBX
$-144    0071E39C   ^ 0F84 4DF9FFFF            JE PathOfEx.0071DCEF
$-13E    0071E3A2     E8 599CFFFF              CALL PathOfEx.00718000
$-139    0071E3A7     53                       PUSH EBX
$-138    0071E3A8     8BC8                     MOV ECX,EAX
$-136    0071E3AA     E8 E197FFFF              CALL PathOfEx.00717B90
$-131    0071E3AF     8BC6                     MOV EAX,ESI
$-12F    0071E3B1     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-12C    0071E3B4     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-125    0071E3BB     5F                       POP EDI
$-124    0071E3BC     5E                       POP ESI
$-123    0071E3BD     5B                       POP EBX
$-122    0071E3BE     8BE5                     MOV ESP,EBP
$-120    0071E3C0     5D                       POP EBP
$-11F    0071E3C1     C3                       RETN
$-11E    0071E3C2     8B8D 5CFFFFFF            MOV ECX,DWORD PTR SS:[EBP-0xA4]
$-118    0071E3C8     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$-116    0071E3CA     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$-113    0071E3CD     50                       PUSH EAX
$-112    0071E3CE     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$-10F    0071E3D1     E8 EAE6D4FF              CALL PathOfEx.0046CAC0
$-10A    0071E3D6     8D55 C8                  LEA EDX,DWORD PTR SS:[EBP-0x38]
$-107    0071E3D9     C645 FC 2D               MOV BYTE PTR SS:[EBP-0x4],0x2D
$-103    0071E3DD     8D8D A0FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x260]
$-FD     0071E3E3     E8 08A0FEFF              CALL PathOfEx.007083F0
$-F8     0071E3E8     50                       PUSH EAX
$-F7     0071E3E9     BA 18F6EA00              MOV EDX,PathOfEx.00EAF618                        ; UNICODE "[STRING FORMAT ERROR: "
$-F2     0071E3EE     C645 FC 2E               MOV BYTE PTR SS:[EBP-0x4],0x2E
$-EE     0071E3F2     8D8D B8FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x248]
$-E8     0071E3F8     E8 B3A5D9FF              CALL PathOfEx.004B89B0
$-E3     0071E3FD     83C4 04                  ADD ESP,0x4
$-E0     0071E400     68 F4F5EA00              PUSH PathOfEx.00EAF5F4                           ; UNICODE "  ORIGINAL TEXT: "
$-DB     0071E405     8BD0                     MOV EDX,EAX
$-D9     0071E407     C645 FC 2F               MOV BYTE PTR SS:[EBP-0x4],0x2F
$-D5     0071E40B     8D8D D0FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x230]
$-CF     0071E411     E8 0AEED5FF              CALL PathOfEx.0047D220
$-CA     0071E416     83C4 04                  ADD ESP,0x4
$-C7     0071E419     C645 FC 30               MOV BYTE PTR SS:[EBP-0x4],0x30
$-C3     0071E41D     8BD0                     MOV EDX,EAX
$-C1     0071E41F     8B4D 9C                  MOV ECX,DWORD PTR SS:[EBP-0x64]
$-BE     0071E422     FF71 04                  PUSH DWORD PTR DS:[ECX+0x4]
$-BB     0071E425     8D4D A8                  LEA ECX,DWORD PTR SS:[EBP-0x58]
$-B8     0071E428     E8 F3EDD5FF              CALL PathOfEx.0047D220
$-B3     0071E42D     83C4 04                  ADD ESP,0x4
$-B0     0071E430     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
$-AD     0071E433     8BD0                     MOV EDX,EAX
$-AB     0071E435     68 984AE700              PUSH PathOfEx.00E74A98                           ; UNICODE "]"
$-A6     0071E43A     C645 FC 31               MOV BYTE PTR SS:[EBP-0x4],0x31
$-A2     0071E43E     E8 DDEDD5FF              CALL PathOfEx.0047D220
$-9D     0071E443     83C4 04                  ADD ESP,0x4
$-9A     0071E446     8D4D A8                  LEA ECX,DWORD PTR SS:[EBP-0x58]
$-97     0071E449     E8 F2F2D4FF              CALL PathOfEx.0046D740
$-92     0071E44E     8D8D D0FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x230]
$-8C     0071E454     E8 E7F2D4FF              CALL PathOfEx.0046D740
$-87     0071E459     8D8D B8FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x248]
$-81     0071E45F     E8 DCF2D4FF              CALL PathOfEx.0046D740
$-7C     0071E464     8D8D A0FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x260]
$-76     0071E46A     E8 D1F2D4FF              CALL PathOfEx.0046D740
$-71     0071E46F     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$-6E     0071E472     E8 29DFD4FF              CALL PathOfEx.0046C3A0
$-69     0071E477     B8 7DE47100              MOV EAX,PathOfEx.0071E47D
$-64     0071E47C     C3                       RETN
$-63     0071E47D     8D4D 98                  LEA ECX,DWORD PTR SS:[EBP-0x68]
$-60     0071E480     E8 1B18D5FF              CALL PathOfEx.0046FCA0
$-5B     0071E485     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
$-58     0071E488     E8 83E6D4FF              CALL PathOfEx.0046CB10
$-53     0071E48D     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-50     0071E490     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
$-4D     0071E493     5F                       POP EDI
$-4C     0071E494     5E                       POP ESI
$-4B     0071E495     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-44     0071E49C     5B                       POP EBX
$-43     0071E49D     8BE5                     MOV ESP,EBP
$-41     0071E49F     5D                       POP EBP
$-40     0071E4A0     C3                       RETN
$-3F     0071E4A1     0F1F00                   NOP DWORD PTR DS:[EAX]
$-3C     0071E4A4     94                       XCHG EAX,ESP
$-3B     0071E4A5     DD71 00                  FSAVE (108-BYTE) PTR DS:[ECX]
$-38     0071E4A8     AA                       STOS BYTE PTR ES:[EDI]
$-37     0071E4A9     DD71 00                  FSAVE (108-BYTE) PTR DS:[ECX]
$-34     0071E4AC     07                       POP ES
$-33     0071E4AD     DE71 00                  FIDIV WORD PTR DS:[ECX]
$-30     0071E4B0     5D                       POP EBP
$-2F     0071E4B1     DE71 00                  FIDIV WORD PTR DS:[ECX]
$-2C     0071E4B4     8BDE                     MOV EBX,ESI
$-2A     0071E4B6     71 00                    JNO SHORT PathOfEx.0071E4B8
$-28     0071E4B8     AF                       SCAS DWORD PTR ES:[EDI]
$-27     0071E4B9     DE71 00                  FIDIV WORD PTR DS:[ECX]
$-24     0071E4BC     D3DE                     RCR ESI,CL
$-22     0071E4BE     71 00                    JNO SHORT PathOfEx.0071E4C0
$-20     0071E4C0     F8                       CLC
$-1F     0071E4C1     DE71 00                  FIDIV WORD PTR DS:[ECX]
$-1C     0071E4C4     BE DF7100E5              MOV ESI,0xE50071DF
$-17     0071E4C9     DF71 00                  FBSTP TBYTE PTR DS:[ECX]
$-14     0071E4CC     65:E0 71                 LOOPNZ SHORT 0071E540
$-11     0071E4CF     009A DF7100FC            ADD BYTE PTR DS:[EDX+0xFC0071DF],BL
$-B      0071E4D5     E0 71                    LOOPDNE SHORT PathOfEx.0071E548
$-9      0071E4D7     0085 E17100CC            ADD BYTE PTR SS:[EBP+0xCC0071E1],AL
$-3      0071E4DD     CC                       INT3
$-2      0071E4DE     CC                       INT3
$-1      0071E4DF     CC                       INT3
$ ==>    0071E4E0     55                       PUSH EBP                                         ; tag_查找ClientString中文字符串
$+1      0071E4E1     8BEC                     MOV EBP,ESP                                      ; 参数是edx，数组索引
$+3      0071E4E3     6A FF                    PUSH -0x1
$+5      0071E4E5     68 1A18D200              PUSH PathOfEx.00D2181A                           ; ASCII "隔荇"
$+A      0071E4EA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+10     0071E4F0     50                       PUSH EAX
$+11     0071E4F1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+18     0071E4F8     83EC 10                  SUB ESP,0x10
$+1B     0071E4FB     53                       PUSH EBX
$+1C     0071E4FC     56                       PUSH ESI
$+1D     0071E4FD     57                       PUSH EDI
$+1E     0071E4FE     8BFA                     MOV EDI,EDX                                      ; edx=stCD_DbClientStrings的数组索引
$+20     0071E500     894D F0                  MOV DWORD PTR SS:[EBP-0x10],ECX
$+23     0071E503     85FF                     TEST EDI,EDI
$+25     0071E505     75 16                    JNZ SHORT PathOfEx.0071E51D
$+27     0071E507     B8 B8F5EA00              MOV EAX,PathOfEx.00EAF5B8                        ; UNICODE "Success"
$+2C     0071E50C     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+2F     0071E50F     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+36     0071E516     5F                       POP EDI
$+37     0071E517     5E                       POP ESI
$+38     0071E518     5B                       POP EBX
$+39     0071E519     8BE5                     MOV ESP,EBP
$+3B     0071E51B     5D                       POP EBP
$+3C     0071E51C     C3                       RETN
$+3D     0071E51D     83EC 0C                  SUB ESP,0xC
$+40     0071E520     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
$+43     0071E523     50                       PUSH EAX
$+44     0071E524     E8 D79AFFFF              CALL PathOfEx.00718000
$+49     0071E529     8BC8                     MOV ECX,EAX
$+4B     0071E52B     E8 500C0000              CALL PathOfEx.0071F180                           ; 查找并返回stCD_DbClientStrings*
$+50     0071E530     8BF0                     MOV ESI,EAX
$+52     0071E532     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
$+55     0071E535     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+5C     0071E53C     85F6                     TEST ESI,ESI
$+5E     0071E53E     74 07                    JE SHORT PathOfEx.0071E547
$+60     0071E540     8BCE                     MOV ECX,ESI
$+62     0071E542     E8 79B0FFFF              CALL PathOfEx.007195C0
$+67     0071E547     8D47 FF                  LEA EAX,DWORD PTR DS:[EDI-0x1]
$+6A     0071E54A     50                       PUSH EAX                                         ; eax=数组索引，也就是stCD_TblItemClientStrings*的索引
$+6B     0071E54B     8D55 EC                  LEA EDX,DWORD PTR SS:[EBP-0x14]
$+6E     0071E54E     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]                  ; 执行后ecx=stCD_ClientStringPair*
$+71     0071E551     E8 7A780100              CALL PathOfEx.00735DD0                           ; 查找并返回stCD_ClientStringPair*
$+76     0071E556     83C4 04                  ADD ESP,0x4
$+79     0071E559     8B18                     MOV EBX,DWORD PTR DS:[EAX]
$+7B     0071E55B     C700 00000000            MOV DWORD PTR DS:[EAX],0x0
$+81     0071E561     8B78 04                  MOV EDI,DWORD PTR DS:[EAX+0x4]
$+84     0071E564     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+88     0071E568     8B45 E4                  MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+8B     0071E56B     85C0                     TEST EAX,EAX
$+8D     0071E56D     74 11                    JE SHORT PathOfEx.0071E580
$+8F     0071E56F     50                       PUSH EAX
$+90     0071E570     51                       PUSH ECX
$+91     0071E571     E8 8A9AFFFF              CALL PathOfEx.00718000
$+96     0071E576     83C4 04                  ADD ESP,0x4
$+99     0071E579     8BC8                     MOV ECX,EAX
$+9B     0071E57B     E8 1096FFFF              CALL PathOfEx.00717B90
$+A0     0071E580     85FF                     TEST EDI,EDI                                     ; edi=stCD_TblItemClientStrings*
$+A2     0071E582     75 07                    JNZ SHORT PathOfEx.0071E58B
$+A4     0071E584     BF C8F5EA00              MOV EDI,PathOfEx.00EAF5C8                        ; UNICODE "bad-error-code"
$+A9     0071E589     EB 03                    JMP SHORT PathOfEx.0071E58E
$+AB     0071E58B     8B7F 04                  MOV EDI,DWORD PTR DS:[EDI+0x4]                   ; stCD_TblItemClientStrings.ch_str_
$+AE     0071E58E     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+B2     0071E592     85DB                     TEST EBX,EBX
$+B4     0071E594     74 11                    JE SHORT PathOfEx.0071E5A7
$+B6     0071E596     53                       PUSH EBX
$+B7     0071E597     51                       PUSH ECX
$+B8     0071E598     E8 639AFFFF              CALL PathOfEx.00718000                           ; 啥都没干
$+BD     0071E59D     83C4 04                  ADD ESP,0x4
$+C0     0071E5A0     8BC8                     MOV ECX,EAX
$+C2     0071E5A2     E8 E995FFFF              CALL PathOfEx.00717B90
$+C7     0071E5A7     C745 FC 03000000         MOV DWORD PTR SS:[EBP-0x4],0x3
$+CE     0071E5AE     85F6                     TEST ESI,ESI
$+D0     0071E5B0     74 11                    JE SHORT PathOfEx.0071E5C3
$+D2     0071E5B2     56                       PUSH ESI
$+D3     0071E5B3     51                       PUSH ECX
$+D4     0071E5B4     E8 479AFFFF              CALL PathOfEx.00718000                           ; 啥都没干
$+D9     0071E5B9     83C4 04                  ADD ESP,0x4
$+DC     0071E5BC     8BC8                     MOV ECX,EAX
$+DE     0071E5BE     E8 CD95FFFF              CALL PathOfEx.00717B90
$+E3     0071E5C3     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+E6     0071E5C6     8BC7                     MOV EAX,EDI
$+E8     0071E5C8     5F                       POP EDI
$+E9     0071E5C9     5E                       POP ESI
$+EA     0071E5CA     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+F1     0071E5D1     5B                       POP EBX
$+F2     0071E5D2     8BE5                     MOV ESP,EBP
$+F4     0071E5D4     5D                       POP EBP
$+F5     0071E5D5     C3                       RETN
$+F6     0071E5D6     CC                       INT3
$+F7     0071E5D7     CC                       INT3
$+F8     0071E5D8     CC                       INT3
$+F9     0071E5D9     CC                       INT3
$+FA     0071E5DA     CC                       INT3
$+FB     0071E5DB     CC                       INT3
$+FC     0071E5DC     CC                       INT3
$+FD     0071E5DD     CC                       INT3
$+FE     0071E5DE     CC                       INT3
$+FF     0071E5DF     CC                       INT3
$+100    0071E5E0     55                       PUSH EBP
$+101    0071E5E1     8BEC                     MOV EBP,ESP
$+103    0071E5E3     6A FF                    PUSH -0x1
$+105    0071E5E5     68 1A8DD400              PUSH PathOfEx.00D48D1A
$+10A    0071E5EA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+110    0071E5F0     50                       PUSH EAX
$+111    0071E5F1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+118    0071E5F8     81EC B8030000            SUB ESP,0x3B8
$+11E    0071E5FE     53                       PUSH EBX
$+11F    0071E5FF     56                       PUSH ESI
$+120    0071E600     57                       PUSH EDI
$+121    0071E601     C745 A0 00000000         MOV DWORD PTR SS:[EBP-0x60],0x0
$+128    0071E608     8BD9                     MOV EBX,ECX
$+12A    0071E60A     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
$+12D    0071E60D     8965 F0                  MOV DWORD PTR SS:[EBP-0x10],ESP
$+130    0071E610     895D E0                  MOV DWORD PTR SS:[EBP-0x20],EBX
$+133    0071E613     C745 A4 3018E700         MOV DWORD PTR SS:[EBP-0x5C],PathOfEx.00E71830    ; UNICODE "Data/ClientStrings.dat"
$+13A    0071E61A     85FF                     TEST EDI,EDI
$+13C    0071E61C     75 1D                    JNZ SHORT PathOfEx.0071E63B
$+13E    0071E61E     68 B4FCE600              PUSH PathOfEx.00E6FCB4
$+143    0071E623     E8 C8F7D4FF              CALL PathOfEx.0046DDF0
$+148    0071E628     8BC3                     MOV EAX,EBX
$+14A    0071E62A     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+14D    0071E62D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+154    0071E634     5F                       POP EDI
$+155    0071E635     5E                       POP ESI
$+156    0071E636     5B                       POP EBX
$+157    0071E637     8BE5                     MOV ESP,EBP
$+159    0071E639     5D                       POP EBP
$+15A    0071E63A     C3                       RETN
$+15B    0071E63B     83EC 0C                  SUB ESP,0xC
$+15E    0071E63E     8D45 A4                  LEA EAX,DWORD PTR SS:[EBP-0x5C]
$+161    0071E641     50                       PUSH EAX
$+162    0071E642     E8 B999FFFF              CALL PathOfEx.00718000
$+167    0071E647     8BC8                     MOV ECX,EAX
$+169    0071E649     E8 320B0000              CALL PathOfEx.0071F180
$+16E    0071E64E     8BD8                     MOV EBX,EAX
$+170    0071E650     895D C0                  MOV DWORD PTR SS:[EBP-0x40],EBX
$+173    0071E653     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+17A    0071E65A     85DB                     TEST EBX,EBX
$+17C    0071E65C     74 07                    JE SHORT PathOfEx.0071E665
$+17E    0071E65E     8BCB                     MOV ECX,EBX
$+180    0071E660     E8 5BAFFFFF              CALL PathOfEx.007195C0
$+185    0071E665     8D47 FF                  LEA EAX,DWORD PTR DS:[EDI-0x1]
$+188    0071E668     50                       PUSH EAX
$+189    0071E669     8D55 C0                  LEA EDX,DWORD PTR SS:[EBP-0x40]
$+18C    0071E66C     8D4D 90                  LEA ECX,DWORD PTR SS:[EBP-0x70]
$+18F    0071E66F     E8 5C770100              CALL PathOfEx.00735DD0
$+194    0071E674     83C4 04                  ADD ESP,0x4
$+197    0071E677     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+199    0071E679     C700 00000000            MOV DWORD PTR DS:[EAX],0x0
$+19F    0071E67F     8B70 04                  MOV ESI,DWORD PTR DS:[EAX+0x4]
$+1A2    0071E682     894D C4                  MOV DWORD PTR SS:[EBP-0x3C],ECX
$+1A5    0071E685     894D 98                  MOV DWORD PTR SS:[EBP-0x68],ECX
$+1A8    0071E688     8975 9C                  MOV DWORD PTR SS:[EBP-0x64],ESI
$+1AB    0071E68B     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+1AF    0071E68F     8B45 90                  MOV EAX,DWORD PTR SS:[EBP-0x70]
$+1B2    0071E692     85C0                     TEST EAX,EAX
$+1B4    0071E694     74 18                    JE SHORT PathOfEx.0071E6AE
$+1B6    0071E696     50                       PUSH EAX
$+1B7    0071E697     51                       PUSH ECX
$+1B8    0071E698     E8 6399FFFF              CALL PathOfEx.00718000
$+1BD    0071E69D     83C4 04                  ADD ESP,0x4
$+1C0    0071E6A0     8BC8                     MOV ECX,EAX
$+1C2    0071E6A2     E8 E994FFFF              CALL PathOfEx.00717B90
$+1C7    0071E6A7     C745 90 00000000         MOV DWORD PTR SS:[EBP-0x70],0x0
$+1CE    0071E6AE     85F6                     TEST ESI,ESI
$+1D0    0071E6B0     0F85 9C010000            JNZ PathOfEx.0071E852
$+1D6    0071E6B6     C785 E8FDFFFF ECE8EA00   MOV DWORD PTR SS:[EBP-0x218],PathOfEx.00EAE8EC
$+1E0    0071E6C0     C785 F8FDFFFF 98E7EA00   MOV DWORD PTR SS:[EBP-0x208],PathOfEx.00EAE798
$+1EA    0071E6CA     C785 50FEFFFF 4C54DE00   MOV DWORD PTR SS:[EBP-0x1B0],PathOfEx.00DE544C
$+1F4    0071E6D4     51                       PUSH ECX
$+1F5    0071E6D5     8D85 00FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x200]
$+1FB    0071E6DB     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
$+1FF    0071E6DF     50                       PUSH EAX
$+200    0071E6E0     8D8D E8FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x218]
$+206    0071E6E6     C745 A0 02000000         MOV DWORD PTR SS:[EBP-0x60],0x2
$+20D    0071E6ED     E8 BE9DD5FF              CALL PathOfEx.004784B0
$+212    0071E6F2     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
$+219    0071E6F9     8B85 E8FDFFFF            MOV EAX,DWORD PTR SS:[EBP-0x218]
$+21F    0071E6FF     6A 03                    PUSH 0x3
$+221    0071E701     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+224    0071E704     C78405 E8FDFFFF 64FCE600 MOV DWORD PTR SS:[EBP+EAX-0x218],PathOfEx.00E6FC>
$+22F    0071E70F     8B85 E8FDFFFF            MOV EAX,DWORD PTR SS:[EBP-0x218]
$+235    0071E715     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
$+238    0071E718     8D41 98                  LEA EAX,DWORD PTR DS:[ECX-0x68]
$+23B    0071E71B     89840D E4FDFFFF          MOV DWORD PTR SS:[EBP+ECX-0x21C],EAX
$+242    0071E722     8D8D 00FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x200]
$+248    0071E728     E8 63A3D5FF              CALL PathOfEx.00478A90
$+24D    0071E72D     57                       PUSH EDI
$+24E    0071E72E     51                       PUSH ECX
$+24F    0071E72F     68 E8F5EA00              PUSH PathOfEx.00EAF5E8                           ; UNICODE " row "
$+254    0071E734     51                       PUSH ECX
$+255    0071E735     68 3018E700              PUSH PathOfEx.00E71830                           ; UNICODE "Data/ClientStrings.dat"
$+25A    0071E73A     51                       PUSH ECX
$+25B    0071E73B     8D85 F8FDFFFF            LEA EAX,DWORD PTR SS:[EBP-0x208]
$+261    0071E741     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
$+265    0071E745     68 18F4EA00              PUSH PathOfEx.00EAF418                           ; UNICODE "Trying to parse unknown string "
$+26A    0071E74A     50                       PUSH EAX
$+26B    0071E74B     E8 C09CD6FF              CALL PathOfEx.00488410
$+270    0071E750     83C4 0C                  ADD ESP,0xC
$+273    0071E753     50                       PUSH EAX
$+274    0071E754     E8 B79CD6FF              CALL PathOfEx.00488410
$+279    0071E759     83C4 0C                  ADD ESP,0xC
$+27C    0071E75C     50                       PUSH EAX
$+27D    0071E75D     E8 AE9CD6FF              CALL PathOfEx.00488410
$+282    0071E762     83C4 0C                  ADD ESP,0xC
$+285    0071E765     8BC8                     MOV ECX,EAX
$+287    0071E767     E8 4423D6FF              CALL PathOfEx.00480AB0
$+28C    0071E76C     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
$+28F    0071E76F     50                       PUSH EAX
$+290    0071E770     8D8D E8FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x218]
$+296    0071E776     E8 759CD6FF              CALL PathOfEx.004883F0
$+29B    0071E77B     8BF0                     MOV ESI,EAX
$+29D    0071E77D     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
$+2A1    0071E781     FF15 C8E0D800            CALL DWORD PTR DS:[0xD8E0C8]                     ; kernel32.GetTickCount
$+2A7    0071E787     8BF8                     MOV EDI,EAX
$+2A9    0071E789     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
$+2AC    0071E78C     50                       PUSH EAX
$+2AD    0071E78D     E8 35C65800              CALL PathOfEx.00CAADC7
$+2B2    0071E792     64:8B0D 2C000000         MOV ECX,DWORD PTR FS:[0x2C]
$+2B9    0071E799     83C4 04                  ADD ESP,0x4
$+2BC    0071E79C     8B09                     MOV ECX,DWORD PTR DS:[ECX]
$+2BE    0071E79E     56                       PUSH ESI
$+2BF    0071E79F     83EC 0C                  SUB ESP,0xC
$+2C2    0071E7A2     8B49 18                  MOV ECX,DWORD PTR DS:[ECX+0x18]
$+2C5    0071E7A5     6A 02                    PUSH 0x2
$+2C7    0071E7A7     57                       PUSH EDI
$+2C8    0071E7A8     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
$+2CB    0071E7AB     FF75 E8                  PUSH DWORD PTR SS:[EBP-0x18]
$+2CE    0071E7AE     6A 3F                    PUSH 0x3F
$+2D0    0071E7B0     E8 DB98D6FF              CALL PathOfEx.00488090
$+2D5    0071E7B5     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
$+2D8    0071E7B8     83F8 08                  CMP EAX,0x8
$+2DB    0071E7BB     72 0F                    JB SHORT PathOfEx.0071E7CC
$+2DD    0071E7BD     40                       INC EAX
$+2DE    0071E7BE     6A 02                    PUSH 0x2
$+2E0    0071E7C0     50                       PUSH EAX
$+2E1    0071E7C1     FF75 C8                  PUSH DWORD PTR SS:[EBP-0x38]
$+2E4    0071E7C4     E8 67E0D4FF              CALL PathOfEx.0046C830
$+2E9    0071E7C9     83C4 0C                  ADD ESP,0xC
$+2EC    0071E7CC     8D8D 50FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1B0]
$+2F2    0071E7D2     E8 9985D5FF              CALL PathOfEx.00476D70
$+2F7    0071E7D7     8D85 50FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1B0]
$+2FD    0071E7DD     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
$+301    0071E7E1     50                       PUSH EAX
$+302    0071E7E2     C785 50FEFFFF C453DE00   MOV DWORD PTR SS:[EBP-0x1B0],PathOfEx.00DE53C4
$+30C    0071E7EC     E8 00BD5400              CALL PathOfEx.00C6A4F1
$+311    0071E7F1     8B75 E0                  MOV ESI,DWORD PTR SS:[EBP-0x20]
$+314    0071E7F4     83C4 04                  ADD ESP,0x4
$+317    0071E7F7     8BCE                     MOV ECX,ESI
$+319    0071E7F9     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+31D    0071E7FD     68 B4FCE600              PUSH PathOfEx.00E6FCB4
$+322    0071E802     E8 E9F5D4FF              CALL PathOfEx.0046DDF0
$+327    0071E807     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
$+32B    0071E80B     8B7D C4                  MOV EDI,DWORD PTR SS:[EBP-0x3C]
$+32E    0071E80E     85FF                     TEST EDI,EDI
$+330    0071E810     74 11                    JE SHORT PathOfEx.0071E823
$+332    0071E812     57                       PUSH EDI
$+333    0071E813     51                       PUSH ECX
$+334    0071E814     E8 E797FFFF              CALL PathOfEx.00718000
$+339    0071E819     83C4 04                  ADD ESP,0x4
$+33C    0071E81C     8BC8                     MOV ECX,EAX
$+33E    0071E81E     E8 6D93FFFF              CALL PathOfEx.00717B90
$+343    0071E823     C745 FC 09000000         MOV DWORD PTR SS:[EBP-0x4],0x9
$+34A    0071E82A     85DB                     TEST EBX,EBX
$+34C    0071E82C     74 11                    JE SHORT PathOfEx.0071E83F
$+34E    0071E82E     53                       PUSH EBX
$+34F    0071E82F     51                       PUSH ECX
$+350    0071E830     E8 CB97FFFF              CALL PathOfEx.00718000
$+355    0071E835     83C4 04                  ADD ESP,0x4
$+358    0071E838     8BC8                     MOV ECX,EAX
$+35A    0071E83A     E8 5193FFFF              CALL PathOfEx.00717B90
$+35F    0071E83F     8BC6                     MOV EAX,ESI
$+361    0071E841     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+364    0071E844     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+36B    0071E84B     5F                       POP EDI
$+36C    0071E84C     5E                       POP ESI
$+36D    0071E84D     5B                       POP EBX
$+36E    0071E84E     8BE5                     MOV ESP,EBP
$+370    0071E850     5D                       POP EBP
$+371    0071E851     C3                       RETN
*/
/**/
//tag_关闭新手引导
/*
$-1D8    006330C8     E8 13E43C00               CALL PathOfEx.00A014E0
$-1D3    006330CD     C645 FC 0C                MOV BYTE PTR SS:[EBP-0x4],0xC
$-1CF    006330D1     8B8B C0080000             MOV ECX,DWORD PTR DS:[EBX+0x8C0]
$-1C9    006330D7     81C1 40080000             ADD ECX,0x840
$-1C3    006330DD     50                        PUSH EAX
$-1C2    006330DE     8B11                      MOV EDX,DWORD PTR DS:[ECX]
$-1C0    006330E0     FF52 20                   CALL DWORD PTR DS:[EDX+0x20]
$-1BD    006330E3     8D4D CC                   LEA ECX,DWORD PTR SS:[EBP-0x34]
$-1BA    006330E6     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
$-1B6    006330EA     E8 1156E5FF               CALL PathOfEx.00488700
$-1B1    006330EF     8B8B C0080000             MOV ECX,DWORD PTR DS:[EBX+0x8C0]
$-1AB    006330F5     81C1 40080000             ADD ECX,0x840
$-1A5    006330FB     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-1A3    006330FD     FF50 24                   CALL DWORD PTR DS:[EAX+0x24]
$-1A0    00633100     8B30                      MOV ESI,DWORD PTR DS:[EAX]
$-19E    00633102     85F6                      TEST ESI,ESI
$-19C    00633104     74 15                     JE SHORT PathOfEx.0063311B
$-19A    00633106     80BE 80000000 00          CMP BYTE PTR DS:[ESI+0x80],0x0
$-193    0063310D     75 07                     JNZ SHORT PathOfEx.00633116
$-191    0063310F     8BCE                      MOV ECX,ESI
$-18F    00633111     E8 8A223D00               CALL PathOfEx.00A053A0
$-18A    00633116     83C6 60                   ADD ESI,0x60
$-187    00633119     EB 05                     JMP SHORT PathOfEx.00633120
$-185    0063311B     BE 54391501               MOV ESI,PathOfEx.01153954
$-180    00633120     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$-17E    00633122     8D55 CC                   LEA EDX,DWORD PTR SS:[EBP-0x34]
$-17B    00633125     8B8B C0080000             MOV ECX,DWORD PTR DS:[EBX+0x8C0]
$-175    0063312B     52                        PUSH EDX
$-174    0063312C     66:0F6EC0                 MOVD MM0,EAX
$-170    00633130     F30FE6C0                  CVTDQ2PD XMM0,XMM0
$-16C    00633134     C1E8 1F                   SHR EAX,0x1F
$-169    00633137     F20F5804C5 00FDF300       ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$-160    00633140     660F5AC0                  CVTPD2PS XMM0,XMM0
$-15C    00633144     F3:0F1145 CC              MOVSS DWORD PTR SS:[EBP-0x34],XMM0
$-157    00633149     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
$-154    0063314C     66:0F6EC0                 MOVD MM0,EAX
$-150    00633150     F30FE6C0                  CVTDQ2PD XMM0,XMM0
$-14C    00633154     C1E8 1F                   SHR EAX,0x1F
$-149    00633157     F20F5804C5 00FDF300       ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FD00]
$-140    00633160     660F5AC0                  CVTPD2PS XMM0,XMM0
$-13C    00633164     F3:0F1145 D0              MOVSS DWORD PTR SS:[EBP-0x30],XMM0
$-137    00633169     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-135    0063316B     FF50 50                   CALL DWORD PTR DS:[EAX+0x50]
$-132    0063316E     68 B0080000               PUSH 0x8B0
$-12D    00633173     E8 9E956400               CALL PathOfEx.00C7C716
$-128    00633178     83C4 04                   ADD ESP,0x4
$-125    0063317B     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
$-122    0063317E     8BC8                      MOV ECX,EAX
$-120    00633180     E8 4BE43B00               CALL PathOfEx.009F15D0
$-11B    00633185     50                        PUSH EAX
$-11A    00633186     8983 DC080000             MOV DWORD PTR DS:[EBX+0x8DC],EAX
$-114    0063318C     8DB3 04070000             LEA ESI,DWORD PTR DS:[EBX+0x704]
$-10E    00633192     8B46 08                   MOV EAX,DWORD PTR DS:[ESI+0x8]
$-10B    00633195     8BCE                      MOV ECX,ESI
$-109    00633197     2B46 04                   SUB EAX,DWORD PTR DS:[ESI+0x4]
$-106    0063319A     C1F8 02                   SAR EAX,0x2
$-103    0063319D     50                        PUSH EAX
$-102    0063319E     E8 CD033D00               CALL PathOfEx.00A03570
$-FD     006331A3     8B8B DC080000             MOV ECX,DWORD PTR DS:[EBX+0x8DC]
$-F7     006331A9     83B9 68070000 05          CMP DWORD PTR DS:[ECX+0x768],0x5
$-F0     006331B0     F3:0F1091 6C070000        MOVSS XMM2,DWORD PTR DS:[ECX+0x76C]
$-E8     006331B8     75 09                     JNZ SHORT PathOfEx.006331C3
$-E6     006331BA     0F2ED2                    UCOMISS XMM2,XMM2
$-E3     006331BD     9F                        LAHF
$-E2     006331BE     F6C4 44                   TEST AH,0x44
$-DF     006331C1     7B 11                     JPO SHORT PathOfEx.006331D4
$-DD     006331C3     6A 05                     PUSH 0x5
$-DB     006331C5     C781 68070000 05000000    MOV DWORD PTR DS:[ECX+0x768],0x5
$-D1     006331CF     E8 4CB23B00               CALL PathOfEx.009EE420
$-CC     006331D4     8B8B DC080000             MOV ECX,DWORD PTR DS:[EBX+0x8DC]
$-C6     006331DA     6A 00                     PUSH 0x0
$-C4     006331DC     E8 0F9D3B00               CALL PathOfEx.009ECEF0
$-BF     006331E1     BA B8CEE900               MOV EDX,PathOfEx.00E9CEB8                       ; UNICODE "Art/2DArt/UIImages/Misc/TooltipPointer"
$-BA     006331E6     8D4D CC                   LEA ECX,DWORD PTR SS:[EBP-0x34]
$-B7     006331E9     E8 F2E23C00               CALL PathOfEx.00A014E0
$-B2     006331EE     50                        PUSH EAX
$-B1     006331EF     8D8B D0080000             LEA ECX,DWORD PTR DS:[EBX+0x8D0]
$-AB     006331F5     E8 3608E8FF               CALL PathOfEx.004B3A30
$-A6     006331FA     8D4D CC                   LEA ECX,DWORD PTR SS:[EBP-0x34]
$-A3     006331FD     E8 FE54E5FF               CALL PathOfEx.00488700
$-9E     00633202     8B8B B0080000             MOV ECX,DWORD PTR DS:[EBX+0x8B0]
$-98     00633208     8381 B4080000 FF          ADD DWORD PTR DS:[ECX+0x8B4],-0x1
$-91     0063320F     75 0E                     JNZ SHORT PathOfEx.0063321F
$-8F     00633211     80B9 B8080000 00          CMP BYTE PTR DS:[ECX+0x8B8],0x0
$-88     00633218     74 05                     JE SHORT PathOfEx.0063321F
$-86     0063321A     E8 A11DE8FF               CALL PathOfEx.004B4FC0
$-81     0063321F     8B8B B0080000             MOV ECX,DWORD PTR DS:[EBX+0x8B0]
$-7B     00633225     81C1 3C080000             ADD ECX,0x83C
$-75     0063322B     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-73     0063322D     FF50 10                   CALL DWORD PTR DS:[EAX+0x10]
$-70     00633230     68 CC080000               PUSH 0x8CC
$-6B     00633235     E8 DC946400               CALL PathOfEx.00C7C716
$-66     0063323A     83C4 04                   ADD ESP,0x4
$-63     0063323D     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
$-60     00633240     8BC8                      MOV ECX,EAX
$-5E     00633242     E8 495F4000               CALL PathOfEx.00A39190
$-59     00633247     50                        PUSH EAX
$-58     00633248     8983 CC080000             MOV DWORD PTR DS:[EBX+0x8CC],EAX
$-52     0063324E     8BCE                      MOV ECX,ESI
$-50     00633250     8B46 08                   MOV EAX,DWORD PTR DS:[ESI+0x8]
$-4D     00633253     2B46 04                   SUB EAX,DWORD PTR DS:[ESI+0x4]
$-4A     00633256     C1F8 02                   SAR EAX,0x2
$-47     00633259     50                        PUSH EAX
$-46     0063325A     E8 11033D00               CALL PathOfEx.00A03570
$-41     0063325F     8B8B CC080000             MOV ECX,DWORD PTR DS:[EBX+0x8CC]
$-3B     00633265     8D45 CC                   LEA EAX,DWORD PTR SS:[EBP-0x34]
$-38     00633268     50                        PUSH EAX
$-37     00633269     81C1 3C080000             ADD ECX,0x83C
$-31     0063326F     C745 CC 08CFE900          MOV DWORD PTR SS:[EBP-0x34],PathOfEx.00E9CF08
$-2A     00633276     895D D0                   MOV DWORD PTR SS:[EBP-0x30],EBX
$-27     00633279     E8 C2100000               CALL PathOfEx.00634340
$-22     0063327E     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1F     00633281     8BC3                      MOV EAX,EBX
$-1D     00633283     5F                        POP EDI
$-1C     00633284     5E                        POP ESI
$-1B     00633285     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-14     0063328C     5B                        POP EBX
$-13     0063328D     8BE5                      MOV ESP,EBP
$-11     0063328F     5D                        POP EBP
$-10     00633290     C3                        RETN
$-F      00633291     CC                        INT3
$-E      00633292     CC                        INT3
$-D      00633293     CC                        INT3
$-C      00633294     CC                        INT3
$-B      00633295     CC                        INT3
$-A      00633296     CC                        INT3
$-9      00633297     CC                        INT3
$-8      00633298     CC                        INT3
$-7      00633299     CC                        INT3
$-6      0063329A     CC                        INT3
$-5      0063329B     CC                        INT3
$-4      0063329C     CC                        INT3
$-3      0063329D     CC                        INT3
$-2      0063329E     CC                        INT3
$-1      0063329F     CC                        INT3
$ ==>    006332A0     55                        PUSH EBP                                        ; tag_关闭新手引导
$+1      006332A1     8BEC                      MOV EBP,ESP
$+3      006332A3     6A FF                     PUSH -0x1
$+5      006332A5     68 8CB1D300               PUSH PathOfEx.00D3B18C                          ; ASCII "歌啮"
$+A      006332AA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+10     006332B0     50                        PUSH EAX
$+11     006332B1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     006332B8     83EC 10                   SUB ESP,0x10
$+1B     006332BB     56                        PUSH ESI
$+1C     006332BC     8BF1                      MOV ESI,ECX
$+1E     006332BE     57                        PUSH EDI
$+1F     006332BF     6A 00                     PUSH 0x0
$+21     006332C1     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+23     006332C3     E8 289C3B00               CALL PathOfEx.009ECEF0                          ; call tag_隐藏Ui
$+28     006332C8     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+2A     006332CA     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+2D     006332CD     50                        PUSH EAX
$+2E     006332CE     81C1 EC080000             ADD ECX,0x8EC
$+34     006332D4     E8 77D0F4FF               CALL PathOfEx.00580350
$+39     006332D9     8BF0                      MOV ESI,EAX
$+3B     006332DB     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
$+3E     006332DE     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+45     006332E5     E8 76A1E4FF               CALL PathOfEx.0047D460
$+4A     006332EA     6A 01                     PUSH 0x1
$+4C     006332EC     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+50     006332F0     8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+52     006332F2     56                        PUSH ESI
$+53     006332F3     E8 E8EE0600               CALL PathOfEx.006A21E0
$+58     006332F8     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
$+5C     006332FC     83CF FF                   OR EDI,0xFFFFFFFF
$+5F     006332FF     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
$+62     00633302     85F6                      TEST ESI,ESI
$+64     00633304     74 1F                     JE SHORT PathOfEx.00633325
$+66     00633306     8BC7                      MOV EAX,EDI
$+68     00633308     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+6D     0063330D     75 16                     JNZ SHORT PathOfEx.00633325
$+6F     0063330F     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+71     00633311     8BCE                      MOV ECX,ESI
$+73     00633313     FF10                      CALL DWORD PTR DS:[EAX]
$+75     00633315     8BC7                      MOV EAX,EDI
$+77     00633317     F0:0FC146 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+7C     0063331C     75 07                     JNZ SHORT PathOfEx.00633325
$+7E     0063331E     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+80     00633320     8BCE                      MOV ECX,ESI
$+82     00633322     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+85     00633325     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4],0x3
$+8C     0063332C     8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
$+8F     0063332F     85F6                      TEST ESI,ESI
$+91     00633331     74 1E                     JE SHORT PathOfEx.00633351
$+93     00633333     8BC7                      MOV EAX,EDI
$+95     00633335     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+9A     0063333A     75 15                     JNZ SHORT PathOfEx.00633351
$+9C     0063333C     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+9E     0063333E     8BCE                      MOV ECX,ESI
$+A0     00633340     FF10                      CALL DWORD PTR DS:[EAX]
$+A2     00633342     F0:0FC17E 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+A7     00633347     4F                        DEC EDI
$+A8     00633348     75 07                     JNZ SHORT PathOfEx.00633351
$+AA     0063334A     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+AC     0063334C     8BCE                      MOV ECX,ESI
$+AE     0063334E     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+B1     00633351     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+B4     00633354     5F                        POP EDI
$+B5     00633355     5E                        POP ESI
$+B6     00633356     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+BD     0063335D     8BE5                      MOV ESP,EBP
$+BF     0063335F     5D                        POP EBP
$+C0     00633360     C2 0400                   RETN 0x4
$+C3     00633363     CC                        INT3
$+C4     00633364     CC                        INT3
$+C5     00633365     CC                        INT3
$+C6     00633366     CC                        INT3
$+C7     00633367     CC                        INT3
$+C8     00633368     CC                        INT3
$+C9     00633369     CC                        INT3
$+CA     0063336A     CC                        INT3
$+CB     0063336B     CC                        INT3
$+CC     0063336C     CC                        INT3
$+CD     0063336D     CC                        INT3
$+CE     0063336E     CC                        INT3
$+CF     0063336F     CC                        INT3
$+D0     00633370     55                        PUSH EBP
$+D1     00633371     8BEC                      MOV EBP,ESP
$+D3     00633373     6A FF                     PUSH -0x1
$+D5     00633375     68 8CB1D300               PUSH PathOfEx.00D3B18C                          ; ASCII "歌啮"
$+DA     0063337A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+E0     00633380     50                        PUSH EAX
$+E1     00633381     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+E8     00633388     83EC 10                   SUB ESP,0x10
$+EB     0063338B     56                        PUSH ESI
$+EC     0063338C     8BF1                      MOV ESI,ECX
$+EE     0063338E     57                        PUSH EDI
$+EF     0063338F     6A 00                     PUSH 0x0
$+F1     00633391     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+F3     00633393     E8 589B3B00               CALL PathOfEx.009ECEF0
$+F8     00633398     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+FA     0063339A     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+FD     0063339D     50                        PUSH EAX
$+FE     0063339E     81C1 EC080000             ADD ECX,0x8EC
$+104    006333A4     E8 A7CFF4FF               CALL PathOfEx.00580350
$+109    006333A9     8BF0                      MOV ESI,EAX
$+10B    006333AB     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
$+10E    006333AE     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+115    006333B5     E8 A6A0E4FF               CALL PathOfEx.0047D460
$+11A    006333BA     6A 02                     PUSH 0x2
$+11C    006333BC     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+120    006333C0     8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+122    006333C2     56                        PUSH ESI
$+123    006333C3     E8 18EE0600               CALL PathOfEx.006A21E0
$+128    006333C8     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
$+12C    006333CC     83CF FF                   OR EDI,0xFFFFFFFF
$+12F    006333CF     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
$+132    006333D2     85F6                      TEST ESI,ESI
$+134    006333D4     74 1F                     JE SHORT PathOfEx.006333F5
$+136    006333D6     8BC7                      MOV EAX,EDI
$+138    006333D8     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+13D    006333DD     75 16                     JNZ SHORT PathOfEx.006333F5
$+13F    006333DF     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+141    006333E1     8BCE                      MOV ECX,ESI
$+143    006333E3     FF10                      CALL DWORD PTR DS:[EAX]
$+145    006333E5     8BC7                      MOV EAX,EDI
$+147    006333E7     F0:0FC146 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+14C    006333EC     75 07                     JNZ SHORT PathOfEx.006333F5
$+14E    006333EE     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+150    006333F0     8BCE                      MOV ECX,ESI
$+152    006333F2     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+155    006333F5     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4],0x3
$+15C    006333FC     8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
$+15F    006333FF     85F6                      TEST ESI,ESI
$+161    00633401     74 1E                     JE SHORT PathOfEx.00633421
$+163    00633403     8BC7                      MOV EAX,EDI
$+165    00633405     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+16A    0063340A     75 15                     JNZ SHORT PathOfEx.00633421
$+16C    0063340C     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+16E    0063340E     8BCE                      MOV ECX,ESI
$+170    00633410     FF10                      CALL DWORD PTR DS:[EAX]
$+172    00633412     F0:0FC17E 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+177    00633417     4F                        DEC EDI
$+178    00633418     75 07                     JNZ SHORT PathOfEx.00633421
$+17A    0063341A     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+17C    0063341C     8BCE                      MOV ECX,ESI
$+17E    0063341E     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+181    00633421     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+184    00633424     5F                        POP EDI
$+185    00633425     5E                        POP ESI
$+186    00633426     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+18D    0063342D     8BE5                      MOV ESP,EBP
$+18F    0063342F     5D                        POP EBP
$+190    00633430     C3                        RETN
$+191    00633431     CC                        INT3
$+192    00633432     CC                        INT3
*/
//tag_点击新手引导并暂时关闭它
/*
$-FD     00633C23    /74 3C                     JE SHORT PathOfEx.00633C61
$-FB     00633C25    |8B86 0C070000             MOV EAX,DWORD PTR DS:[ESI+0x70C]
$-F5     00633C2B    |8D8E 08070000             LEA ECX,DWORD PTR DS:[ESI+0x708]
$-EF     00633C31    |2B01                      SUB EAX,DWORD PTR DS:[ECX]
$-ED     00633C33    |C1F8 02                   SAR EAX,0x2
$-EA     00633C36    |83F8 02                   CMP EAX,0x2
$-E7     00633C39    |72 26                     JB SHORT PathOfEx.00633C61
$-E5     00633C3B    |57                        PUSH EDI
$-E4     00633C3C    |51                        PUSH ECX
$-E3     00633C3D    |E8 8EFB3C00               CALL PathOfEx.00A037D0
$-DE     00633C42    |83BE 20070000 00          CMP DWORD PTR DS:[ESI+0x720],0x0
$-D7     00633C49    |75 0F                     JNZ SHORT PathOfEx.00633C5A
$-D5     00633C4B    |57                        PUSH EDI
$-D4     00633C4C    |8D86 14070000             LEA EAX,DWORD PTR DS:[ESI+0x714]
$-CE     00633C52    |50                        PUSH EAX
$-CD     00633C53    |E8 78FB3C00               CALL PathOfEx.00A037D0
$-C8     00633C58    |EB 07                     JMP SHORT PathOfEx.00633C61
$-C6     00633C5A    |C686 24070000 01          MOV BYTE PTR DS:[ESI+0x724],0x1
$-BF     00633C61    \80BF 54070000 01          CMP BYTE PTR DS:[EDI+0x754],0x1
$-B8     00633C68     74 24                     JE SHORT PathOfEx.00633C8E
$-B6     00633C6A     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-B4     00633C6C     8BCF                      MOV ECX,EDI
$-B2     00633C6E     C687 54070000 01          MOV BYTE PTR DS:[EDI+0x754],0x1
$-AB     00633C75     FF90 B0000000             CALL DWORD PTR DS:[EAX+0xB0]
$-A5     00633C7B     8B8F 5C070000             MOV ECX,DWORD PTR DS:[EDI+0x75C]
$-9F     00633C81     85C9                      TEST ECX,ECX
$-9D     00633C83     74 09                     JE SHORT PathOfEx.00633C8E
$-9B     00633C85     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$-99     00633C87     57                        PUSH EDI
$-98     00633C88     FF90 D0000000             CALL DWORD PTR DS:[EAX+0xD0]
$-92     00633C8E     F3:0F104C24 24            MOVSS XMM1,DWORD PTR SS:[ESP+0x24]
$-8C     00633C94     0F57C0                    XORPS XMM0,XMM0
$-89     00633C97     0F2EC8                    UCOMISS XMM1,XMM0
$-86     00633C9A     9F                        LAHF
$-85     00633C9B     F6C4 44                   TEST AH,0x44
$-82     00633C9E     7B 41                     JPO SHORT PathOfEx.00633CE1
$-80     00633CA0     8B8F CC080000             MOV ECX,DWORD PTR DS:[EDI+0x8CC]
$-7A     00633CA6     8B81 58070000             MOV EAX,DWORD PTR DS:[ECX+0x758]
$-74     00633CAC     F3:0F1189 BC080000        MOVSS DWORD PTR DS:[ECX+0x8BC],XMM1
$-6C     00633CB4     C781 C0080000 01000000    MOV DWORD PTR DS:[ECX+0x8C0],0x1
$-62     00633CBE     F3:0F1080 180B0000        MOVSS XMM0,DWORD PTR DS:[EAX+0xB18]
$-5A     00633CC6     F3:0F58C1                 ADDSS XMM0,XMM1
$-56     00633CCA     C681 C4080000 00          MOV BYTE PTR DS:[ECX+0x8C4],0x0
$-4F     00633CD1     F3:0F1181 C8080000        MOVSS DWORD PTR DS:[ECX+0x8C8],XMM0
$-47     00633CD9     5F                        POP EDI
$-46     00633CDA     5E                        POP ESI
$-45     00633CDB     8BE5                      MOV ESP,EBP
$-43     00633CDD     5D                        POP EBP
$-42     00633CDE     C2 1C00                   RETN 0x1C
$-3F     00633CE1     8B87 CC080000             MOV EAX,DWORD PTR DS:[EDI+0x8CC]
$-39     00633CE7     5F                        POP EDI
$-38     00633CE8     5E                        POP ESI
$-37     00633CE9     C780 C8080000 00000000    MOV DWORD PTR DS:[EAX+0x8C8],0x0
$-2D     00633CF3     C680 C4080000 01          MOV BYTE PTR DS:[EAX+0x8C4],0x1
$-26     00633CFA     8BE5                      MOV ESP,EBP
$-24     00633CFC     5D                        POP EBP
$-23     00633CFD     C2 1C00                   RETN 0x1C
$-20     00633D00     78 3A                     JS SHORT PathOfEx.00633D3C
$-1E     00633D02     6300                      ARPL WORD PTR DS:[EAX],AX
$-1C     00633D04     78 3A                     JS SHORT PathOfEx.00633D40
$-1A     00633D06     6300                      ARPL WORD PTR DS:[EAX],AX
$-18     00633D08     FD                        STD
$-17     00633D09     3A63 00                   CMP AH,BYTE PTR DS:[EBX]
$-14     00633D0C     7E 3B                     JLE SHORT PathOfEx.00633D49
$-12     00633D0E     6300                      ARPL WORD PTR DS:[EAX],AX
$-10     00633D10     7E 3B                     JLE SHORT PathOfEx.00633D4D
$-E      00633D12     6300                      ARPL WORD PTR DS:[EAX],AX
$-C      00633D14     78 3A                     JS SHORT PathOfEx.00633D50
$-A      00633D16     6300                      ARPL WORD PTR DS:[EAX],AX
$-8      00633D18     78 3A                     JS SHORT PathOfEx.00633D54
$-6      00633D1A     6300                      ARPL WORD PTR DS:[EAX],AX
$-4      00633D1C     CC                        INT3
$-3      00633D1D     CC                        INT3
$-2      00633D1E     CC                        INT3
$-1      00633D1F     CC                        INT3
$ ==>    00633D20     55                        PUSH EBP                                        ; tag_点击新手引导并暂时关闭它
$+1      00633D21     8BEC                      MOV EBP,ESP
$+3      00633D23     6A FF                     PUSH -0x1
$+5      00633D25     68 8CB1D300               PUSH PathOfEx.00D3B18C                          ; ASCII "歌啮"
$+A      00633D2A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+10     00633D30     50                        PUSH EAX
$+11     00633D31     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     00633D38     83EC 10                   SUB ESP,0x10
$+1B     00633D3B     56                        PUSH ESI
$+1C     00633D3C     57                        PUSH EDI
$+1D     00633D3D     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
$+20     00633D40     8BF1                      MOV ESI,ECX
$+22     00633D42     56                        PUSH ESI
$+23     00633D43     8D8E 04020000             LEA ECX,DWORD PTR DS:[ESI+0x204]
$+29     00633D49     E8 F2DAE5FF               CALL PathOfEx.00491840
$+2E     00633D4E     6A 00                     PUSH 0x0
$+30     00633D50     8BCE                      MOV ECX,ESI
$+32     00633D52     E8 99913B00               CALL PathOfEx.009ECEF0                          ; call tag_隐藏Ui
$+37     00633D57     8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+3A     00633D5A     50                        PUSH EAX
$+3B     00633D5B     8D8E EC080000             LEA ECX,DWORD PTR DS:[ESI+0x8EC]
$+41     00633D61     E8 EAC5F4FF               CALL PathOfEx.00580350
$+46     00633D66     8BF0                      MOV ESI,EAX
$+48     00633D68     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
$+4B     00633D6B     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+52     00633D72     E8 E996E4FF               CALL PathOfEx.0047D460
$+57     00633D77     6A 00                     PUSH 0x0
$+59     00633D79     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+5D     00633D7D     8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+5F     00633D7F     56                        PUSH ESI
$+60     00633D80     E8 5BE40600               CALL PathOfEx.006A21E0
$+65     00633D85     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
$+69     00633D89     83CF FF                   OR EDI,0xFFFFFFFF
$+6C     00633D8C     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
$+6F     00633D8F     85F6                      TEST ESI,ESI
$+71     00633D91     74 1F                     JE SHORT PathOfEx.00633DB2
$+73     00633D93     8BC7                      MOV EAX,EDI
$+75     00633D95     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+7A     00633D9A     75 16                     JNZ SHORT PathOfEx.00633DB2
$+7C     00633D9C     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+7E     00633D9E     8BCE                      MOV ECX,ESI
$+80     00633DA0     FF10                      CALL DWORD PTR DS:[EAX]
$+82     00633DA2     8BC7                      MOV EAX,EDI
$+84     00633DA4     F0:0FC146 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+89     00633DA9     75 07                     JNZ SHORT PathOfEx.00633DB2
$+8B     00633DAB     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+8D     00633DAD     8BCE                      MOV ECX,ESI
$+8F     00633DAF     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+92     00633DB2     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4],0x3
$+99     00633DB9     8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
$+9C     00633DBC     85F6                      TEST ESI,ESI
$+9E     00633DBE     74 1E                     JE SHORT PathOfEx.00633DDE
$+A0     00633DC0     8BC7                      MOV EAX,EDI
$+A2     00633DC2     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+A7     00633DC7     75 15                     JNZ SHORT PathOfEx.00633DDE
$+A9     00633DC9     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+AB     00633DCB     8BCE                      MOV ECX,ESI
$+AD     00633DCD     FF10                      CALL DWORD PTR DS:[EAX]
$+AF     00633DCF     F0:0FC17E 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+B4     00633DD4     4F                        DEC EDI
$+B5     00633DD5     75 07                     JNZ SHORT PathOfEx.00633DDE
$+B7     00633DD7     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+B9     00633DD9     8BCE                      MOV ECX,ESI
$+BB     00633DDB     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+BE     00633DDE     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+C1     00633DE1     5F                        POP EDI
$+C2     00633DE2     5E                        POP ESI
$+C3     00633DE3     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+CA     00633DEA     8BE5                      MOV ESP,EBP
$+CC     00633DEC     5D                        POP EBP
$+CD     00633DED     C2 0400                   RETN 0x4
$+D0     00633DF0     55                        PUSH EBP
$+D1     00633DF1     8BEC                      MOV EBP,ESP
$+D3     00633DF3     83E4 F8                   AND ESP,0xFFFFFFF8
$+D6     00633DF6     83EC 10                   SUB ESP,0x10
$+D9     00633DF9     56                        PUSH ESI
$+DA     00633DFA     57                        PUSH EDI
$+DB     00633DFB     8BF9                      MOV EDI,ECX
$+DD     00633DFD     80BF E8080000 00          CMP BYTE PTR DS:[EDI+0x8E8],0x0
$+E4     00633E04     0F84 84020000             JE PathOfEx.0063408E
$+EA     00633E0A     F3:0F1087 E0080000        MOVSS XMM0,DWORD PTR DS:[EDI+0x8E0]
$+F2     00633E12     0F2E05 445FFE00           UCOMISS XMM0,DWORD PTR DS:[0xFE5F44]
$+F9     00633E19     9F                        LAHF
$+FA     00633E1A     F6C4 44                   TEST AH,0x44
$+FD     00633E1D     7A 19                     JPE SHORT PathOfEx.00633E38
$+FF     00633E1F     F3:0F1087 E4080000        MOVSS XMM0,DWORD PTR DS:[EDI+0x8E4]
$+107    00633E27     0F2E05 485FFE00           UCOMISS XMM0,DWORD PTR DS:[0xFE5F48]
$+10E    00633E2E     9F                        LAHF
$+10F    00633E2F     F6C4 44                   TEST AH,0x44
$+112    00633E32     0F8B 56020000             JPO PathOfEx.0063408E
$+118    00633E38     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
$+11C    00633E3C     50                        PUSH EAX
$+11D    00633E3D     E8 7E020000               CALL PathOfEx.006340C0
$+122    00633E42     A1 ECF6E600               MOV EAX,DWORD PTR DS:[0xE6F6EC]
$+127    00633E47     894424 10                 MOV DWORD PTR SS:[ESP+0x10],EAX
$+12B    00633E4B     A1 F0F6E600               MOV EAX,DWORD PTR DS:[0xE6F6F0]
$+130    00633E50     894424 14                 MOV DWORD PTR SS:[ESP+0x14],EAX
$+134    00633E54     8B87 D8080000             MOV EAX,DWORD PTR DS:[EDI+0x8D8]
$+13A    00633E5A     83F8 06                   CMP EAX,0x6
$+13D    00633E5D     0F87 2B020000             JA PathOfEx.0063408E
$+143    00633E63     FF2485 98406300           JMP DWORD PTR DS:[EAX*4+0x634098]
$+14A    00633E6A     A1 F4F6E600               MOV EAX,DWORD PTR DS:[0xE6F6F4]
$+14F    00633E6F     894424 10                 MOV DWORD PTR SS:[ESP+0x10],EAX
$+153    00633E73     A1 F8F6E600               MOV EAX,DWORD PTR DS:[0xE6F6F8]
$+158    00633E78     894424 14                 MOV DWORD PTR SS:[ESP+0x14],EAX
$+15C    00633E7C     66:8B4424 0C              MOV AX,WORD PTR SS:[ESP+0xC]
$+161    00633E81     84C0                      TEST AL,AL
$+163    00633E83     0F84 AD000000             JE PathOfEx.00633F36
$+169    00633E89     84E4                      TEST AH,AH
$+16B    00633E8B     74 34                     JE SHORT PathOfEx.00633EC1
$+16D    00633E8D     F3:0F101D C0F0F300        MOVSS XMM3,DWORD PTR DS:[0xF3F0C0]
$+175    00633E95     F3:0F105424 10            MOVSS XMM2,DWORD PTR SS:[ESP+0x10]
$+17B    00633E9B     F3:0F5C5C24 14            SUBSS XMM3,DWORD PTR SS:[ESP+0x14]
$+181    00633EA1     0F5715 C0FDF300           XORPS XMM2,DQWORD PTR DS:[0xF3FDC0]
$+188    00633EA8     6A 00                     PUSH 0x0
$+18A    00633EAA     6A 00                     PUSH 0x0
$+18C    00633EAC     68 64391501               PUSH PathOfEx.01153964
$+191    00633EB1     83EC 14                   SUB ESP,0x14
$+194    00633EB4     C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
$+19C    00633EBC     E9 9F010000               JMP PathOfEx.00634060
$+1A1    00633EC1     8DB7 D0080000             LEA ESI,DWORD PTR DS:[EDI+0x8D0]
$+1A7    00633EC7     8BCE                      MOV ECX,ESI
$+1A9    00633EC9     E8 A20DE8FF               CALL PathOfEx.004B4C70
$+1AE    00633ECE     F3:0F109F 48080000        MOVSS XMM3,DWORD PTR DS:[EDI+0x848]
$+1B6    00633ED6     F3:0F105424 10            MOVSS XMM2,DWORD PTR SS:[ESP+0x10]
$+1BC    00633EDC     0F5715 C0FDF300           XORPS XMM2,DQWORD PTR DS:[0xF3FDC0]
$+1C3    00633EE3     F3:0F1048 04              MOVSS XMM1,DWORD PTR DS:[EAX+0x4]
$+1C8    00633EE8     F3:0F5C4C24 14            SUBSS XMM1,DWORD PTR SS:[ESP+0x14]
$+1CE    00633EEE     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
$+1D1    00633EF1     6A 01                     PUSH 0x1
$+1D3    00633EF3     6A 00                     PUSH 0x0
$+1D5    00633EF5     68 64391501               PUSH PathOfEx.01153964
$+1DA    00633EFA     83EC 14                   SUB ESP,0x14
$+1DD    00633EFD     F3:0F5CD9                 SUBSS XMM3,XMM1
$+1E1    00633F01     C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
$+1E9    00633F09     C74424 0C 00000000        MOV DWORD PTR SS:[ESP+0xC],0x0
$+1F1    00633F11     C74424 08 00000000        MOV DWORD PTR SS:[ESP+0x8],0x0
$+1F9    00633F19     C74424 04 0000803F        MOV DWORD PTR SS:[ESP+0x4],0x3F800000
$+201    00633F21     C70424 0000803F           MOV DWORD PTR SS:[ESP],0x3F800000
$+208    00633F28     56                        PUSH ESI
$+209    00633F29     E8 821E3D00               CALL PathOfEx.00A05DB0
$+20E    00633F2E     5F                        POP EDI
$+20F    00633F2F     5E                        POP ESI
$+210    00633F30     8BE5                      MOV ESP,EBP
$+212    00633F32     5D                        POP EBP
$+213    00633F33     C2 0400                   RETN 0x4
$+216    00633F36     8DB7 D0080000             LEA ESI,DWORD PTR DS:[EDI+0x8D0]
$+21C    00633F3C     8BCE                      MOV ECX,ESI
$+21E    00633F3E     84E4                      TEST AH,AH
$+220    00633F40     74 6D                     JE SHORT PathOfEx.00633FAF
$+222    00633F42     E8 290DE8FF               CALL PathOfEx.004B4C70
$+227    00633F47     F3:0F101D C0F0F300        MOVSS XMM3,DWORD PTR DS:[0xF3F0C0]
$+22F    00633F4F     F3:0F5C5C24 14            SUBSS XMM3,DWORD PTR SS:[ESP+0x14]
$+235    00633F55     6A 00                     PUSH 0x0
$+237    00633F57     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+23B    00633F5B     F3:0F5C4424 14            SUBSS XMM0,DWORD PTR SS:[ESP+0x14]
$+241    00633F61     F3:0F1097 44080000        MOVSS XMM2,DWORD PTR DS:[EDI+0x844]
$+249    00633F69     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
$+24C    00633F6C     6A 01                     PUSH 0x1
$+24E    00633F6E     68 64391501               PUSH PathOfEx.01153964
$+253    00633F73     83EC 14                   SUB ESP,0x14
$+256    00633F76     F3:0F5CD0                 SUBSS XMM2,XMM0
$+25A    00633F7A     C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
$+262    00633F82     C74424 0C 00000000        MOV DWORD PTR SS:[ESP+0xC],0x0
$+26A    00633F8A     C74424 08 00000000        MOV DWORD PTR SS:[ESP+0x8],0x0
$+272    00633F92     C74424 04 0000803F        MOV DWORD PTR SS:[ESP+0x4],0x3F800000
$+27A    00633F9A     C70424 0000803F           MOV DWORD PTR SS:[ESP],0x3F800000
$+281    00633FA1     56                        PUSH ESI
$+282    00633FA2     E8 091E3D00               CALL PathOfEx.00A05DB0
$+287    00633FA7     5F                        POP EDI
$+288    00633FA8     5E                        POP ESI
$+289    00633FA9     8BE5                      MOV ESP,EBP
$+28B    00633FAB     5D                        POP EBP
$+28C    00633FAC     C2 0400                   RETN 0x4
$+28F    00633FAF     E8 BC0CE8FF               CALL PathOfEx.004B4C70
$+294    00633FB4     8BCE                      MOV ECX,ESI
$+296    00633FB6     F3:0F1040 04              MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
$+29B    00633FBB     F3:0F5C4424 14            SUBSS XMM0,DWORD PTR SS:[ESP+0x14]
$+2A1    00633FC1     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
$+2A7    00633FC7     E8 A40CE8FF               CALL PathOfEx.004B4C70
$+2AC    00633FCC     F3:0F109F 48080000        MOVSS XMM3,DWORD PTR DS:[EDI+0x848]
$+2B4    00633FD4     F3:0F5C5C24 0C            SUBSS XMM3,DWORD PTR SS:[ESP+0xC]
$+2BA    00633FDA     6A 01                     PUSH 0x1
$+2BC    00633FDC   ^ E9 76FFFFFF               JMP PathOfEx.00633F57
$+2C1    00633FE1     F3:0F1005 F0F6E600        MOVSS XMM0,DWORD PTR DS:[0xE6F6F0]
$+2C9    00633FE9     F3:0F100D ECF6E600        MOVSS XMM1,DWORD PTR DS:[0xE6F6EC]
$+2D1    00633FF1     F3:0F59C0                 MULSS XMM0,XMM0
$+2D5    00633FF5     F3:0F59C9                 MULSS XMM1,XMM1
$+2D9    00633FF9     F3:0F58C1                 ADDSS XMM0,XMM1
$+2DD    00633FFD     E8 BEC7E6FF               CALL PathOfEx.004A07C0
$+2E2    00634002     F3:0F109F 48080000        MOVSS XMM3,DWORD PTR DS:[EDI+0x848]
$+2EA    0063400A     0F28C8                    MOVAPS XMM1,XMM0
$+2ED    0063400D     F3:0F5C1D C0F0F300        SUBSS XMM3,DWORD PTR DS:[0xF3F0C0]
$+2F5    00634015     6A 00                     PUSH 0x0
$+2F7    00634017     6A 00                     PUSH 0x0
$+2F9    00634019     68 64391501               PUSH PathOfEx.01153964
$+2FE    0063401E     83EC 14                   SUB ESP,0x14
$+301    00634021     807C24 2C 00              CMP BYTE PTR SS:[ESP+0x2C],0x0
$+306    00634026     F3:0F591D 6CEDF300        MULSS XMM3,DWORD PTR DS:[0xF3ED6C]
$+30E    0063402E     F3:0F581D C0F0F300        ADDSS XMM3,DWORD PTR DS:[0xF3F0C0]
$+316    00634036     74 14                     JE SHORT PathOfEx.0063404C
$+318    00634038     0F570D C0FDF300           XORPS XMM1,DQWORD PTR DS:[0xF3FDC0]
$+31F    0063403F     C74424 10 E0EDAF40        MOV DWORD PTR SS:[ESP+0x10],0x40AFEDE0
$+327    00634047     0F28D1                    MOVAPS XMM2,XMM1
$+32A    0063404A     EB 14                     JMP SHORT PathOfEx.00634060
$+32C    0063404C     F3:0F1097 44080000        MOVSS XMM2,DWORD PTR DS:[EDI+0x844]
$+334    00634054     C74424 10 E4CB1640        MOV DWORD PTR SS:[ESP+0x10],0x4016CBE4
$+33C    0063405C     F3:0F58D1                 ADDSS XMM2,XMM1
$+340    00634060     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
$+343    00634063     8D87 D0080000             LEA EAX,DWORD PTR DS:[EDI+0x8D0]
$+349    00634069     C74424 0C 00000000        MOV DWORD PTR SS:[ESP+0xC],0x0
$+351    00634071     C74424 08 00000000        MOV DWORD PTR SS:[ESP+0x8],0x0
$+359    00634079     C74424 04 0000803F        MOV DWORD PTR SS:[ESP+0x4],0x3F800000
$+361    00634081     C70424 0000803F           MOV DWORD PTR SS:[ESP],0x3F800000
$+368    00634088     50                        PUSH EAX
$+369    00634089     E8 221D3D00               CALL PathOfEx.00A05DB0
$+36E    0063408E     5F                        POP EDI
$+36F    0063408F     5E                        POP ESI
$+370    00634090     8BE5                      MOV ESP,EBP
$+372    00634092     5D                        POP EBP
$+373    00634093     C2 0400                   RETN 0x4
$+376    00634096     66:90                     NOP
*/
/**/
//tag_显示或隐藏Ui
/*
$-181    009ECD6F     CC                   INT3
$-180    009ECD70     55                   PUSH EBP
$-17F    009ECD71     8D6C24 D4            LEA EBP,DWORD PTR SS:[ESP-0x2C]
$-17B    009ECD75     83EC 2C              SUB ESP,0x2C
$-178    009ECD78     6A FF                PUSH -0x1
$-176    009ECD7A     68 5324D700          PUSH PathOfEx.00D72453
$-171    009ECD7F     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
$-16B    009ECD85     50                   PUSH EAX
$-16A    009ECD86     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
$-163    009ECD8D     81EC FC000000        SUB ESP,0xFC
$-15D    009ECD93     56                   PUSH ESI
$-15C    009ECD94     57                   PUSH EDI
$-15B    009ECD95     FF75 34              PUSH DWORD PTR SS:[EBP+0x34]
$-158    009ECD98     8BF9                 MOV EDI,ECX
$-156    009ECD9A     8D8D F8FEFFFF        LEA ECX,DWORD PTR SS:[EBP-0x108]
$-150    009ECDA0     E8 2BD70400          CALL PathOfEx.00A3A4D0
$-14B    009ECDA5     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
$-144    009ECDAC     80BD F8FEFFFF 00     CMP BYTE PTR SS:[EBP-0x108],0x0
$-13D    009ECDB3     75 3F                JNZ SHORT PathOfEx.009ECDF4
$-13B    009ECDB5     8D45 24              LEA EAX,DWORD PTR SS:[EBP+0x24]
$-138    009ECDB8     C745 20 103FDE00     MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00DE3F10
$-131    009ECDBF     50                   PUSH EAX
$-130    009ECDC0     8D45 18              LEA EAX,DWORD PTR SS:[EBP+0x18]
$-12D    009ECDC3     C745 18 EC56F300     MOV DWORD PTR SS:[EBP+0x18],PathOfEx.00F356EC    ; ASCII "Failed to initialise scanner."
$-126    009ECDCA     0F57C0               XORPS XMM0,XMM0
$-123    009ECDCD     C645 1C 01           MOV BYTE PTR SS:[EBP+0x1C],0x1
$-11F    009ECDD1     50                   PUSH EAX
$-11E    009ECDD2     660FD645 24          MOVQ QWORD PTR SS:[EBP+0x24],XMM0
$-119    009ECDD7     E8 18842A00          CALL PathOfEx.00C951F4
$-114    009ECDDC     83C4 08              ADD ESP,0x8
$-111    009ECDDF     C745 20 683FDE00     MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00DE3F68
$-10A    009ECDE6     8D45 20              LEA EAX,DWORD PTR SS:[EBP+0x20]
$-107    009ECDE9     68 B8EDFC00          PUSH PathOfEx.00FCEDB8
$-102    009ECDEE     50                   PUSH EAX
$-101    009ECDEF     E8 CF942A00          CALL PathOfEx.00C962C3
$-FC     009ECDF4     6A 00                PUSH 0x0
$-FA     009ECDF6     8D85 F8FEFFFF        LEA EAX,DWORD PTR SS:[EBP-0x108]
$-F4     009ECDFC     50                   PUSH EAX
$-F3     009ECDFD     57                   PUSH EDI
$-F2     009ECDFE     8D4D 9C              LEA ECX,DWORD PTR SS:[EBP-0x64]
$-EF     009ECE01     E8 AA920500          CALL PathOfEx.00A460B0
$-EA     009ECE06     8D4D 9C              LEA ECX,DWORD PTR SS:[EBP-0x64]
$-E7     009ECE09     C645 FC 01           MOV BYTE PTR SS:[EBP-0x4],0x1
$-E3     009ECE0D     E8 CE900500          CALL PathOfEx.00A45EE0
$-DE     009ECE12     8B75 CC              MOV ESI,DWORD PTR SS:[EBP-0x34]
$-DB     009ECE15     833E 00              CMP DWORD PTR DS:[ESI],0x0
$-D8     009ECE18     0F8E A4000000        JLE PathOfEx.009ECEC2
$-D2     009ECE1E     8D8F 04070000        LEA ECX,DWORD PTR DS:[EDI+0x704]
$-CC     009ECE24     E8 B76B0100          CALL PathOfEx.00A039E0
$-C7     009ECE29     33C0                 XOR EAX,EAX
$-C5     009ECE2B     C745 14 07000000     MOV DWORD PTR SS:[EBP+0x14],0x7
$-BE     009ECE32     C745 10 00000000     MOV DWORD PTR SS:[EBP+0x10],0x0
$-B7     009ECE39     66:8945 00           MOV WORD PTR SS:[EBP],AX
$-B3     009ECE3D     C645 FC 02           MOV BYTE PTR SS:[EBP-0x4],0x2
$-AF     009ECE41     8D45 00              LEA EAX,DWORD PTR SS:[EBP]
$-AC     009ECE44     8B56 08              MOV EDX,DWORD PTR DS:[ESI+0x8]
$-A9     009ECE47     8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
$-A6     009ECE4A     8945 34              MOV DWORD PTR SS:[EBP+0x34],EAX
$-A3     009ECE4D     8D45 34              LEA EAX,DWORD PTR SS:[EBP+0x34]
$-A0     009ECE50     50                   PUSH EAX
$-9F     009ECE51     E8 7A380000          CALL PathOfEx.009F06D0
$-9A     009ECE56     8B56 14              MOV EDX,DWORD PTR DS:[ESI+0x14]
$-97     009ECE59     8D45 00              LEA EAX,DWORD PTR SS:[EBP]
$-94     009ECE5C     8B4E 10              MOV ECX,DWORD PTR DS:[ESI+0x10]
$-91     009ECE5F     8945 34              MOV DWORD PTR SS:[EBP+0x34],EAX
$-8E     009ECE62     8D45 34              LEA EAX,DWORD PTR SS:[EBP+0x34]
$-8B     009ECE65     50                   PUSH EAX
$-8A     009ECE66     E8 65380000          CALL PathOfEx.009F06D0
$-85     009ECE6B     8D45 00              LEA EAX,DWORD PTR SS:[EBP]
$-82     009ECE6E     50                   PUSH EAX
$-81     009ECE6F     8D45 84              LEA EAX,DWORD PTR SS:[EBP-0x7C]
$-7E     009ECE72     50                   PUSH EAX
$-7D     009ECE73     E8 E8B4D1FF          CALL PathOfEx.00708360
$-78     009ECE78     83C4 10              ADD ESP,0x10
$-75     009ECE7B     C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
$-71     009ECE7F     8378 14 10           CMP DWORD PTR DS:[EAX+0x14],0x10
$-6D     009ECE83     72 02                JB SHORT PathOfEx.009ECE87
$-6B     009ECE85     8B00                 MOV EAX,DWORD PTR DS:[EAX]
$-69     009ECE87     8945 18              MOV DWORD PTR SS:[EBP+0x18],EAX
$-66     009ECE8A     0F57C0               XORPS XMM0,XMM0
$-63     009ECE8D     8D45 24              LEA EAX,DWORD PTR SS:[EBP+0x24]
$-60     009ECE90     C745 20 103FDE00     MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00DE3F10
$-59     009ECE97     50                   PUSH EAX
$-58     009ECE98     8D45 18              LEA EAX,DWORD PTR SS:[EBP+0x18]
$-55     009ECE9B     660FD645 24          MOVQ QWORD PTR SS:[EBP+0x24],XMM0
$-50     009ECEA0     50                   PUSH EAX
$-4F     009ECEA1     C645 1C 01           MOV BYTE PTR SS:[EBP+0x1C],0x1
$-4B     009ECEA5     E8 4A832A00          CALL PathOfEx.00C951F4
$-46     009ECEAA     83C4 08              ADD ESP,0x8
$-43     009ECEAD     C745 20 683FDE00     MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00DE3F68
$-3C     009ECEB4     8D45 20              LEA EAX,DWORD PTR SS:[EBP+0x20]
$-39     009ECEB7     68 B8EDFC00          PUSH PathOfEx.00FCEDB8
$-34     009ECEBC     50                   PUSH EAX
$-33     009ECEBD     E8 01942A00          CALL PathOfEx.00C962C3
$-2E     009ECEC2     8D4D 9C              LEA ECX,DWORD PTR SS:[EBP-0x64]
$-2B     009ECEC5     E8 E6930500          CALL PathOfEx.00A462B0
$-26     009ECECA     8D8D F8FEFFFF        LEA ECX,DWORD PTR SS:[EBP-0x108]
$-20     009ECED0     E8 1BD70400          CALL PathOfEx.00A3A5F0
$-1B     009ECED5     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$-18     009ECED8     5F                   POP EDI
$-17     009ECED9     5E                   POP ESI
$-16     009ECEDA     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$-F      009ECEE1     8D65 2C              LEA ESP,DWORD PTR SS:[EBP+0x2C]
$-C      009ECEE4     5D                   POP EBP
$-B      009ECEE5     C2 0400              RETN 0x4
$-8      009ECEE8     CC                   INT3
$-7      009ECEE9     CC                   INT3
$-6      009ECEEA     CC                   INT3
$-5      009ECEEB     CC                   INT3
$-4      009ECEEC     CC                   INT3
$-3      009ECEED     CC                   INT3
$-2      009ECEEE     CC                   INT3
$-1      009ECEEF     CC                   INT3
$ ==>    009ECEF0     51                   PUSH ECX                                         ; tag_显示或隐藏Ui
$+1      009ECEF1     8A4424 08            MOV AL,BYTE PTR SS:[ESP+0x8]                     ; ecx=stCD_UiObjBase*
$+5      009ECEF5     56                   PUSH ESI                                         ; p1=bool,true=显示,false=隐藏
$+6      009ECEF6     8BF1                 MOV ESI,ECX
$+8      009ECEF8     3886 54070000        CMP BYTE PTR DS:[ESI+0x754],AL
$+E      009ECEFE     0F84 CB000000        JE PathOfEx.009ECFCF
$+14     009ECF04     8886 54070000        MOV BYTE PTR DS:[ESI+0x754],AL
$+1A     009ECF0A     84C0                 TEST AL,AL
$+1C     009ECF0C     0F85 A0000000        JNZ PathOfEx.009ECFB2
$+22     009ECF12     8B96 58070000        MOV EDX,DWORD PTR DS:[ESI+0x758]
$+28     009ECF18     85D2                 TEST EDX,EDX
$+2A     009ECF1A     0F84 92000000        JE PathOfEx.009ECFB2
$+30     009ECF20     8B82 AC0A0000        MOV EAX,DWORD PTR DS:[EDX+0xAAC]
$+36     009ECF26     3BF0                 CMP ESI,EAX
$+38     009ECF28     74 0A                JE SHORT PathOfEx.009ECF34
$+3A     009ECF2A     50                   PUSH EAX
$+3B     009ECF2B     E8 10FEFFFF          CALL PathOfEx.009ECD40
$+40     009ECF30     84C0                 TEST AL,AL
$+42     009ECF32     74 09                JE SHORT PathOfEx.009ECF3D
$+44     009ECF34     6A 00                PUSH 0x0
$+46     009ECF36     8BCA                 MOV ECX,EDX
$+48     009ECF38     E8 F3A70200          CALL PathOfEx.00A17730
$+4D     009ECF3D     8B96 58070000        MOV EDX,DWORD PTR DS:[ESI+0x758]
$+53     009ECF43     8B82 A80A0000        MOV EAX,DWORD PTR DS:[EDX+0xAA8]
$+59     009ECF49     3BF0                 CMP ESI,EAX
$+5B     009ECF4B     74 0C                JE SHORT PathOfEx.009ECF59
$+5D     009ECF4D     50                   PUSH EAX
$+5E     009ECF4E     8BCE                 MOV ECX,ESI
$+60     009ECF50     E8 EBFDFFFF          CALL PathOfEx.009ECD40
$+65     009ECF55     84C0                 TEST AL,AL
$+67     009ECF57     74 07                JE SHORT PathOfEx.009ECF60
$+69     009ECF59     8BCA                 MOV ECX,EDX
$+6B     009ECF5B     E8 D08C0200          CALL PathOfEx.00A15C30
$+70     009ECF60     8B96 58070000        MOV EDX,DWORD PTR DS:[ESI+0x758]
$+76     009ECF66     8B82 C80A0000        MOV EAX,DWORD PTR DS:[EDX+0xAC8]
$+7C     009ECF6C     85C0                 TEST EAX,EAX
$+7E     009ECF6E     74 1D                JE SHORT PathOfEx.009ECF8D
$+80     009ECF70     8B80 5C070000        MOV EAX,DWORD PTR DS:[EAX+0x75C]
$+86     009ECF76     3BF0                 CMP ESI,EAX
$+88     009ECF78     74 0C                JE SHORT PathOfEx.009ECF86
$+8A     009ECF7A     50                   PUSH EAX
$+8B     009ECF7B     8BCE                 MOV ECX,ESI
$+8D     009ECF7D     E8 BEFDFFFF          CALL PathOfEx.009ECD40
$+92     009ECF82     84C0                 TEST AL,AL
$+94     009ECF84     74 07                JE SHORT PathOfEx.009ECF8D
$+96     009ECF86     8BCA                 MOV ECX,EDX
$+98     009ECF88     E8 63A50200          CALL PathOfEx.00A174F0
$+9D     009ECF8D     8B86 58070000        MOV EAX,DWORD PTR DS:[ESI+0x758]
$+A3     009ECF93     8B90 C40A0000        MOV EDX,DWORD PTR DS:[EAX+0xAC4]
$+A9     009ECF99     3BF2                 CMP ESI,EDX
$+AB     009ECF9B     74 0C                JE SHORT PathOfEx.009ECFA9
$+AD     009ECF9D     52                   PUSH EDX
$+AE     009ECF9E     8BCE                 MOV ECX,ESI
$+B0     009ECFA0     E8 9BFDFFFF          CALL PathOfEx.009ECD40
$+B5     009ECFA5     84C0                 TEST AL,AL
$+B7     009ECFA7     74 09                JE SHORT PathOfEx.009ECFB2
$+B9     009ECFA9     6A 00                PUSH 0x0
$+BB     009ECFAB     8BCA                 MOV ECX,EDX
$+BD     009ECFAD     E8 BE110000          CALL PathOfEx.009EE170
$+C2     009ECFB2     8B06                 MOV EAX,DWORD PTR DS:[ESI]
$+C4     009ECFB4     8BCE                 MOV ECX,ESI
$+C6     009ECFB6     FF90 B0000000        CALL DWORD PTR DS:[EAX+0xB0]
$+CC     009ECFBC     8B8E 5C070000        MOV ECX,DWORD PTR DS:[ESI+0x75C]
$+D2     009ECFC2     85C9                 TEST ECX,ECX
$+D4     009ECFC4     74 09                JE SHORT PathOfEx.009ECFCF
$+D6     009ECFC6     8B01                 MOV EAX,DWORD PTR DS:[ECX]
$+D8     009ECFC8     56                   PUSH ESI
$+D9     009ECFC9     FF90 D0000000        CALL DWORD PTR DS:[EAX+0xD0]
$+DF     009ECFCF     5E                   POP ESI
$+E0     009ECFD0     59                   POP ECX
$+E1     009ECFD1     C2 0400              RETN 0x4
$+E4     009ECFD4     CC                   INT3
$+E5     009ECFD5     CC                   INT3
$+E6     009ECFD6     CC                   INT3
$+E7     009ECFD7     CC                   INT3
$+E8     009ECFD8     CC                   INT3
$+E9     009ECFD9     CC                   INT3
$+EA     009ECFDA     CC                   INT3
$+EB     009ECFDB     CC                   INT3
$+EC     009ECFDC     CC                   INT3
$+ED     009ECFDD     CC                   INT3
$+EE     009ECFDE     CC                   INT3
$+EF     009ECFDF     CC                   INT3
$+F0     009ECFE0     80B9 54070000 00     CMP BYTE PTR DS:[ECX+0x754],0x0
$+F7     009ECFE7     74 46                JE SHORT PathOfEx.009ED02F
$+F9     009ECFE9     8B91 58070000        MOV EDX,DWORD PTR DS:[ECX+0x758]
$+FF     009ECFEF     85D2                 TEST EDX,EDX
$+101    009ECFF1     74 3C                JE SHORT PathOfEx.009ED02F
$+103    009ECFF3     56                   PUSH ESI
$+104    009ECFF4     80B9 54070000 00     CMP BYTE PTR DS:[ECX+0x754],0x0
$+10B    009ECFFB     8BF1                 MOV ESI,ECX
$+10D    009ECFFD     74 2C                JE SHORT PathOfEx.009ED02B
$+10F    009ECFFF     80B9 E5070000 00     CMP BYTE PTR DS:[ECX+0x7E5],0x0
$+116    009ED006     74 0E                JE SHORT PathOfEx.009ED016
$+118    009ED008     8B81 5C070000        MOV EAX,DWORD PTR DS:[ECX+0x75C]
$+11E    009ED00E     3B82 C40A0000        CMP EAX,DWORD PTR DS:[EDX+0xAC4]
$+124    009ED014     75 15                JNZ SHORT PathOfEx.009ED02B
$+126    009ED016     8B89 5C070000        MOV ECX,DWORD PTR DS:[ECX+0x75C]
$+12C    009ED01C     85C9                 TEST ECX,ECX
$+12E    009ED01E   ^ 75 D4                JNZ SHORT PathOfEx.009ECFF4
$+130    009ED020     3BB2 A40A0000        CMP ESI,DWORD PTR DS:[EDX+0xAA4]
$+136    009ED026     5E                   POP ESI
$+137    009ED027     0F94C0               SETE AL
$+13A    009ED02A     C3                   RETN
$+13B    009ED02B     32C0                 XOR AL,AL
$+13D    009ED02D     5E                   POP ESI
$+13E    009ED02E     C3                   RETN
$+13F    009ED02F     32C0                 XOR AL,AL
$+141    009ED031     C3                   RETN
$+142    009ED032     CC                   INT3
$+143    009ED033     CC                   INT3
$+144    009ED034     CC                   INT3
$+145    009ED035     CC                   INT3
$+146    009ED036     CC                   INT3
$+147    009ED037     CC                   INT3
$+148    009ED038     CC                   INT3
$+149    009ED039     CC                   INT3
$+14A    009ED03A     CC                   INT3
$+14B    009ED03B     CC                   INT3
$+14C    009ED03C     CC                   INT3
$+14D    009ED03D     CC                   INT3
$+14E    009ED03E     CC                   INT3
$+14F    009ED03F     CC                   INT3
$+150    009ED040     8A4424 04            MOV AL,BYTE PTR SS:[ESP+0x4]
$+154    009ED044     56                   PUSH ESI
$+155    009ED045     8BF1                 MOV ESI,ECX
$+157    009ED047     3886 56070000        CMP BYTE PTR DS:[ESI+0x756],AL
$+15D    009ED04D     0F84 83000000        JE PathOfEx.009ED0D6
$+163    009ED053     8886 56070000        MOV BYTE PTR DS:[ESI+0x756],AL
$+169    009ED059     84C0                 TEST AL,AL
$+16B    009ED05B     75 6F                JNZ SHORT PathOfEx.009ED0CC
$+16D    009ED05D     8B96 58070000        MOV EDX,DWORD PTR DS:[ESI+0x758]
$+173    009ED063     85D2                 TEST EDX,EDX
$+175    009ED065     74 65                JE SHORT PathOfEx.009ED0CC
$+177    009ED067     8B82 AC0A0000        MOV EAX,DWORD PTR DS:[EDX+0xAAC]
$+17D    009ED06D     3BF0                 CMP ESI,EAX
$+17F    009ED06F     74 0A                JE SHORT PathOfEx.009ED07B
$+181    009ED071     50                   PUSH EAX
$+182    009ED072     E8 C9FCFFFF          CALL PathOfEx.009ECD40
$+187    009ED077     84C0                 TEST AL,AL
$+189    009ED079     74 09                JE SHORT PathOfEx.009ED084
$+18B    009ED07B     6A 00                PUSH 0x0
$+18D    009ED07D     8BCA                 MOV ECX,EDX
$+18F    009ED07F     E8 ACA60200          CALL PathOfEx.00A17730
$+194    009ED084     8B96 58070000        MOV EDX,DWORD PTR DS:[ESI+0x758]
$+19A    009ED08A     8B82 A80A0000        MOV EAX,DWORD PTR DS:[EDX+0xAA8]
$+1A0    009ED090     3BF0                 CMP ESI,EAX
$+1A2    009ED092     74 0C                JE SHORT PathOfEx.009ED0A0
$+1A4    009ED094     50                   PUSH EAX
$+1A5    009ED095     8BCE                 MOV ECX,ESI
$+1A7    009ED097     E8 A4FCFFFF          CALL PathOfEx.009ECD40
$+1AC    009ED09C     84C0                 TEST AL,AL
$+1AE    009ED09E     74 07                JE SHORT PathOfEx.009ED0A7
$+1B0    009ED0A0     8BCA                 MOV ECX,EDX
$+1B2    009ED0A2     E8 898B0200          CALL PathOfEx.00A15C30
$+1B7    009ED0A7     8B86 58070000        MOV EAX,DWORD PTR DS:[ESI+0x758]
$+1BD    009ED0AD     8B90 C40A0000        MOV EDX,DWORD PTR DS:[EAX+0xAC4]
$+1C3    009ED0B3     3BF2                 CMP ESI,EDX
$+1C5    009ED0B5     74 0C                JE SHORT PathOfEx.009ED0C3
$+1C7    009ED0B7     52                   PUSH EDX
$+1C8    009ED0B8     8BCE                 MOV ECX,ESI
$+1CA    009ED0BA     E8 81FCFFFF          CALL PathOfEx.009ECD40
$+1CF    009ED0BF     84C0                 TEST AL,AL
$+1D1    009ED0C1     74 09                JE SHORT PathOfEx.009ED0CC
$+1D3    009ED0C3     6A 00                PUSH 0x0
$+1D5    009ED0C5     8BCA                 MOV ECX,EDX
$+1D7    009ED0C7     E8 A4100000          CALL PathOfEx.009EE170
$+1DC    009ED0CC     8B06                 MOV EAX,DWORD PTR DS:[ESI]
$+1DE    009ED0CE     8BCE                 MOV ECX,ESI
$+1E0    009ED0D0     FF90 9C000000        CALL DWORD PTR DS:[EAX+0x9C]
$+1E6    009ED0D6     5E                   POP ESI
$+1E7    009ED0D7     C2 0400              RETN 0x4
$+1EA    009ED0DA     CC                   INT3
*/
/**/
//tag_是否可以升级技能宝石
/*
$-D1     004C8D0F      CC                        INT3
$-D0     004C8D10      55                        PUSH EBP
$-CF     004C8D11      8BEC                      MOV EBP,ESP
$-CD     004C8D13      6A FF                     PUSH -0x1
$-CB     004C8D15      68 485FD200               PUSH PathOfEx.00D25F48
$-C6     004C8D1A      64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-C0     004C8D20      50                        PUSH EAX
$-BF     004C8D21      64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-B8     004C8D28      51                        PUSH ECX
$-B7     004C8D29      53                        PUSH EBX
$-B6     004C8D2A      57                        PUSH EDI
$-B5     004C8D2B      8BF9                      MOV EDI,ECX
$-B3     004C8D2D      897D F0                   MOV DWORD PTR SS:[EBP-0x10],EDI
$-B0     004C8D30      E8 9B885200               CALL PathOfEx.009F15D0
$-AB     004C8D35      C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-A4     004C8D3C      8B5D 0C                   MOV EBX,DWORD PTR SS:[EBP+0xC]
$-A1     004C8D3F      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
$-9E     004C8D42      C707 8C61E700             MOV DWORD PTR DS:[EDI],PathOfEx.00E7618C
$-98     004C8D48      C787 3C080000 88F7E800    MOV DWORD PTR DS:[EDI+0x83C],PathOfEx.00E8F788
$-8E     004C8D52      C787 40080000 2880E900    MOV DWORD PTR DS:[EDI+0x840],PathOfEx.00E98028
$-84     004C8D5C      8987 B0080000             MOV DWORD PTR DS:[EDI+0x8B0],EAX
$-7E     004C8D62      899F B4080000             MOV DWORD PTR DS:[EDI+0x8B4],EBX
$-78     004C8D68      8B0B                      MOV ECX,DWORD PTR DS:[EBX]
$-76     004C8D6A      85C9                      TEST ECX,ECX
$-74     004C8D6C      74 05                     JE SHORT PathOfEx.004C8D73
$-72     004C8D6E      E8 4D082500               CALL PathOfEx.007195C0
$-6D     004C8D73      8B03                      MOV EAX,DWORD PTR DS:[EBX]
$-6B     004C8D75      56                        PUSH ESI
$-6A     004C8D76      C745 08 C07DED00          MOV DWORD PTR SS:[EBP+0x8],PathOfEx.00ED7DC0      ; ASCII "SkillGem"
$-63     004C8D7D      8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
$-60     004C8D80      8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
$-5D     004C8D83      50                        PUSH EAX
$-5C     004C8D84      8D45 0C                   LEA EAX,DWORD PTR SS:[EBP+0xC]
$-59     004C8D87      50                        PUSH EAX
$-58     004C8D88      8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$-55     004C8D8B      E8 B032FAFF               CALL PathOfEx.0046C040
$-50     004C8D90      8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
$-4D     004C8D93      3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$-4A     004C8D96      5E                        POP ESI
$-49     004C8D97      74 10                     JE SHORT PathOfEx.004C8DA9
$-47     004C8D99      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$-44     004C8D9C      83F9 FF                   CMP ECX,-0x1
$-41     004C8D9F      74 08                     JE SHORT PathOfEx.004C8DA9
$-3F     004C8DA1      8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
$-3C     004C8DA4      8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$-39     004C8DA7      EB 02                     JMP SHORT PathOfEx.004C8DAB
$-37     004C8DA9      33C0                      XOR EAX,EAX
$-35     004C8DAB      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-32     004C8DAE      8987 B8080000             MOV DWORD PTR DS:[EDI+0x8B8],EAX
$-2C     004C8DB4      8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
$-29     004C8DB7      8987 BC080000             MOV DWORD PTR DS:[EDI+0x8BC],EAX
$-23     004C8DBD      8B45 14                   MOV EAX,DWORD PTR SS:[EBP+0x14]
$-20     004C8DC0      8987 C0080000             MOV DWORD PTR DS:[EDI+0x8C0],EAX
$-1A     004C8DC6      8B45 18                   MOV EAX,DWORD PTR SS:[EBP+0x18]
$-17     004C8DC9      8987 C4080000             MOV DWORD PTR DS:[EDI+0x8C4],EAX
$-11     004C8DCF      8BC7                      MOV EAX,EDI
$-F      004C8DD1      5F                        POP EDI
$-E      004C8DD2      5B                        POP EBX
$-D      004C8DD3      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-6      004C8DDA      8BE5                      MOV ESP,EBP
$-4      004C8DDC      5D                        POP EBP
$-3      004C8DDD      C2 1400                   RETN 0x14
$ ==>    004C8DE0      55                        PUSH EBP                                          ; tag_是否可以升级技能宝石,g_c_can_lvl_up_skill_gem_
$+1      004C8DE1      8BEC                      MOV EBP,ESP                                       ; ecx=stCD_UiLvlUpSkillGemItem*
$+3      004C8DE3      6A FF                     PUSH -0x1
$+5      004C8DE5      68 5C1BD200               PUSH PathOfEx.00D21B5C                            ; ASCII "胳狒"
$+A      004C8DEA      64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+10     004C8DF0      50                        PUSH EAX
$+11     004C8DF1      64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     004C8DF8      83EC 2C                   SUB ESP,0x2C
$+1B     004C8DFB      8BC1                      MOV EAX,ECX
$+1D     004C8DFD      8945 D8                   MOV DWORD PTR SS:[EBP-0x28],EAX
$+20     004C8E00      53                        PUSH EBX
$+21     004C8E01      56                        PUSH ESI
$+22     004C8E02      8B80 B0080000             MOV EAX,DWORD PTR DS:[EAX+0x8B0]
$+28     004C8E08      57                        PUSH EDI
$+29     004C8E09      8B80 943A0000             MOV EAX,DWORD PTR DS:[EAX+0x3A94]
$+2F     004C8E0F      8BB8 C4010000             MOV EDI,DWORD PTR DS:[EAX+0x1C4]
$+35     004C8E15      8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$+37     004C8E17      85C9                      TEST ECX,ECX
$+39     004C8E19      74 05                     JE SHORT PathOfEx.004C8E20
$+3B     004C8E1B      E8 A0072500               CALL PathOfEx.007195C0
$+40     004C8E20      8B1F                      MOV EBX,DWORD PTR DS:[EDI]
$+42     004C8E22      8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+45     004C8E25      50                        PUSH EAX
$+46     004C8E26      8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
$+49     004C8E29      C745 E4 6CB4EE00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EEB46C     ; ASCII "Player"
$+50     004C8E30      50                        PUSH EAX
$+51     004C8E31      8B73 30                   MOV ESI,DWORD PTR DS:[EBX+0x30]
$+54     004C8E34      8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$+57     004C8E37      E8 0432FAFF               CALL PathOfEx.0046C040
$+5C     004C8E3C      8B45 E8                   MOV EAX,DWORD PTR SS:[EBP-0x18]
$+5F     004C8E3F      3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$+62     004C8E42      74 10                     JE SHORT PathOfEx.004C8E54
$+64     004C8E44      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+67     004C8E47      83F9 FF                   CMP ECX,-0x1
$+6A     004C8E4A      74 08                     JE SHORT PathOfEx.004C8E54
$+6C     004C8E4C      8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+6F     004C8E4F      8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+72     004C8E52      EB 02                     JMP SHORT PathOfEx.004C8E56
$+74     004C8E54      33C0                      XOR EAX,EAX
$+76     004C8E56      8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
$+79     004C8E59      85DB                      TEST EBX,EBX
$+7B     004C8E5B      74 07                     JE SHORT PathOfEx.004C8E64
$+7D     004C8E5D      8BCB                      MOV ECX,EBX
$+7F     004C8E5F      E8 5C072500               CALL PathOfEx.007195C0
$+84     004C8E64      8B1F                      MOV EBX,DWORD PTR DS:[EDI]
$+86     004C8E66      8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+89     004C8E69      50                        PUSH EAX
$+8A     004C8E6A      8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
$+8D     004C8E6D      C745 E4 7CB5EE00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EEB57C     ; ASCII "Actor"
$+94     004C8E74      50                        PUSH EAX
$+95     004C8E75      8B73 30                   MOV ESI,DWORD PTR DS:[EBX+0x30]
$+98     004C8E78      8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$+9B     004C8E7B      E8 C031FAFF               CALL PathOfEx.0046C040
$+A0     004C8E80      8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
$+A3     004C8E83      3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$+A6     004C8E86      74 13                     JE SHORT PathOfEx.004C8E9B
$+A8     004C8E88      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+AB     004C8E8B      83F9 FF                   CMP ECX,-0x1
$+AE     004C8E8E      74 0B                     JE SHORT PathOfEx.004C8E9B
$+B0     004C8E90      8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+B3     004C8E93      8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+B6     004C8E96      8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
$+B9     004C8E99      EB 07                     JMP SHORT PathOfEx.004C8EA2
$+BB     004C8E9B      C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
$+C2     004C8EA2      85DB                      TEST EBX,EBX
$+C4     004C8EA4      74 07                     JE SHORT PathOfEx.004C8EAD
$+C6     004C8EA6      8BCB                      MOV ECX,EBX
$+C8     004C8EA8      E8 13072500               CALL PathOfEx.007195C0
$+CD     004C8EAD      8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+CF     004C8EAF      C745 E4 A8AAEE00          MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EEAAA8     ; ASCII "Stats"
$+D6     004C8EB6      8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
$+D9     004C8EB9      8D45 E4                   LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+DC     004C8EBC      50                        PUSH EAX
$+DD     004C8EBD      8D45 E0                   LEA EAX,DWORD PTR SS:[EBP-0x20]
$+E0     004C8EC0      50                        PUSH EAX
$+E1     004C8EC1      8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$+E4     004C8EC4      E8 7731FAFF               CALL PathOfEx.0046C040
$+E9     004C8EC9      8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
$+EC     004C8ECC      3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$+EF     004C8ECF      74 13                     JE SHORT PathOfEx.004C8EE4
$+F1     004C8ED1      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+F4     004C8ED4      83F9 FF                   CMP ECX,-0x1
$+F7     004C8ED7      74 0B                     JE SHORT PathOfEx.004C8EE4
$+F9     004C8ED9      8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+FC     004C8EDC      8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+FF     004C8EDF      8945 E4                   MOV DWORD PTR SS:[EBP-0x1C],EAX
$+102    004C8EE2      EB 07                     JMP SHORT PathOfEx.004C8EEB
$+104    004C8EE4      C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C],0x0
$+10B    004C8EEB      8B5D D8                   MOV EBX,DWORD PTR SS:[EBP-0x28]
$+10E    004C8EEE      8BBB B4080000             MOV EDI,DWORD PTR DS:[EBX+0x8B4]
$+114    004C8EF4      8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$+116    004C8EF6      85C9                      TEST ECX,ECX
$+118    004C8EF8      74 05                     JE SHORT PathOfEx.004C8EFF
$+11A    004C8EFA      E8 C1062500               CALL PathOfEx.007195C0
$+11F    004C8EFF      8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+121    004C8F01      C745 D8 C07DED00          MOV DWORD PTR SS:[EBP-0x28],PathOfEx.00ED7DC0     ; ASCII "SkillGem"
$+128    004C8F08      8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
$+12B    004C8F0B      8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
$+12E    004C8F0E      50                        PUSH EAX
$+12F    004C8F0F      8D45 E0                   LEA EAX,DWORD PTR SS:[EBP-0x20]
$+132    004C8F12      50                        PUSH EAX
$+133    004C8F13      8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$+136    004C8F16      E8 2531FAFF               CALL PathOfEx.0046C040
$+13B    004C8F1B      8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
$+13E    004C8F1E      3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$+141    004C8F21      74 10                     JE SHORT PathOfEx.004C8F33
$+143    004C8F23      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+146    004C8F26      83F9 FF                   CMP ECX,-0x1
$+149    004C8F29      74 08                     JE SHORT PathOfEx.004C8F33
$+14B    004C8F2B      8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+14E    004C8F2E      8B3C88                    MOV EDI,DWORD PTR DS:[EAX+ECX*4]
$+151    004C8F31      EB 02                     JMP SHORT PathOfEx.004C8F35
$+153    004C8F33      33FF                      XOR EDI,EDI
$+155    004C8F35      8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$+158    004C8F38      8B49 30                   MOV ECX,DWORD PTR DS:[ECX+0x30]
$+15B    004C8F3B      85C9                      TEST ECX,ECX
$+15D    004C8F3D      0F84 C3000000             JE PathOfEx.004C9006
$+163    004C8F43      8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$+166    004C8F46      50                        PUSH EAX
$+167    004C8F47      E8 64E9FFFF               CALL PathOfEx.004C78B0
$+16C    004C8F4C      8B75 EC                   MOV ESI,DWORD PTR SS:[EBP-0x14]
$+16F    004C8F4F      85F6                      TEST ESI,ESI
$+171    004C8F51      0F95C0                    SETNE AL
$+174    004C8F54      84C0                      TEST AL,AL
$+176    004C8F56      74 78                     JE SHORT PathOfEx.004C8FD0
$+178    004C8F58      8B47 10                   MOV EAX,DWORD PTR DS:[EDI+0x10]
$+17B    004C8F5B      8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$+17E    004C8F5E      51                        PUSH ECX
$+17F    004C8F5F      8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+182    004C8F62      E8 39130000               CALL PathOfEx.004CA2A0
$+187    004C8F67      837E 08 00                CMP DWORD PTR DS:[ESI+0x8],0x0
$+18B    004C8F6B      74 44                     JE SHORT PathOfEx.004C8FB1
$+18D    004C8F6D      8D45 D4                   LEA EAX,DWORD PTR SS:[EBP-0x2C]
$+190    004C8F70      50                        PUSH EAX
$+191    004C8F71      8D4E 04                   LEA ECX,DWORD PTR DS:[ESI+0x4]
$+194    004C8F74      E8 C7E8FFFF               CALL PathOfEx.004C7840
$+199    004C8F79      8B45 E0                   MOV EAX,DWORD PTR SS:[EBP-0x20]
$+19C    004C8F7C      8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+19F    004C8F7F      3B45 D8                   CMP EAX,DWORD PTR SS:[EBP-0x28]
$+1A2    004C8F82      75 28                     JNZ SHORT PathOfEx.004C8FAC
$+1A4    004C8F84      E8 176DFAFF               CALL PathOfEx.0046FCA0
$+1A9    004C8F89      8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$+1AC    004C8F8C      E8 0F6DFAFF               CALL PathOfEx.0046FCA0
$+1B1    004C8F91      8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
$+1B4    004C8F94      E8 571AFAFF               CALL PathOfEx.0046A9F0
$+1B9    004C8F99      32C0                      XOR AL,AL
$+1BB    004C8F9B      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1BE    004C8F9E      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+1C5    004C8FA5      5F                        POP EDI
$+1C6    004C8FA6      5E                        POP ESI
$+1C7    004C8FA7      5B                        POP EBX
$+1C8    004C8FA8      8BE5                      MOV ESP,EBP
$+1CA    004C8FAA      5D                        POP EBP
$+1CB    004C8FAB      C3                        RETN
$+1CC    004C8FAC      E8 EF6CFAFF               CALL PathOfEx.0046FCA0
$+1D1    004C8FB1      C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+1D8    004C8FB8      8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
$+1DB    004C8FBB      85C0                      TEST EAX,EAX
$+1DD    004C8FBD      74 11                     JE SHORT PathOfEx.004C8FD0
$+1DF    004C8FBF      50                        PUSH EAX
$+1E0    004C8FC0      51                        PUSH ECX
$+1E1    004C8FC1      E8 3AF02400               CALL PathOfEx.00718000
$+1E6    004C8FC6      83C4 04                   ADD ESP,0x4
$+1E9    004C8FC9      8BC8                      MOV ECX,EAX
$+1EB    004C8FCB      E8 C0EB2400               CALL PathOfEx.00717B90
$+1F0    004C8FD0      C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
$+1F7    004C8FD7      8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
$+1FA    004C8FDA      85F6                      TEST ESI,ESI
$+1FC    004C8FDC      74 21                     JE SHORT PathOfEx.004C8FFF
$+1FE    004C8FDE      83CF FF                   OR EDI,0xFFFFFFFF
$+201    004C8FE1      8BC7                      MOV EAX,EDI
$+203    004C8FE3      F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+208    004C8FE8      75 15                     JNZ SHORT PathOfEx.004C8FFF
$+20A    004C8FEA      8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+20C    004C8FEC      8BCE                      MOV ECX,ESI
$+20E    004C8FEE      FF10                      CALL DWORD PTR DS:[EAX]
$+210    004C8FF0      F0:0FC17E 08              LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+215    004C8FF5      4F                        DEC EDI
$+216    004C8FF6      75 07                     JNZ SHORT PathOfEx.004C8FFF
$+218    004C8FF8      8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+21A    004C8FFA      8BCE                      MOV ECX,ESI
$+21C    004C8FFC      FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
$+21F    004C8FFF      C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
$+226    004C9006      8B75 E8                   MOV ESI,DWORD PTR SS:[EBP-0x18]
$+229    004C9009      8BCE                      MOV ECX,ESI
$+22B    004C900B      E8 10D83100               CALL PathOfEx.007E6820
$+230    004C9010      8D4E 34                   LEA ECX,DWORD PTR DS:[ESI+0x34]
$+233    004C9013      8BC6                      MOV EAX,ESI
$+235    004C9015      2BC1                      SUB EAX,ECX
$+237    004C9017      83C0 40                   ADD EAX,0x40
$+23A    004C901A      50                        PUSH EAX
$+23B    004C901B      51                        PUSH ECX
$+23C    004C901C      8D45 C8                   LEA EAX,DWORD PTR SS:[EBP-0x38]
$+23F    004C901F      50                        PUSH EAX
$+240    004C9020      E8 5BBC7C00               CALL PathOfEx.00C94C80
$+245    004C9025      8B4D E4                   MOV ECX,DWORD PTR SS:[EBP-0x1C]
$+248    004C9028      83C4 0C                   ADD ESP,0xC
$+24B    004C902B      8B49 10                   MOV ECX,DWORD PTR DS:[ECX+0x10]
$+24E    004C902E      68 28050000               PUSH 0x528
$+253    004C9033      8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+255    004C9035      FF10                      CALL DWORD PTR DS:[EAX]
$+257    004C9037      8B56 40                   MOV EDX,DWORD PTR DS:[ESI+0x40]
$+25A    004C903A      8B8B B4080000             MOV ECX,DWORD PTR DS:[EBX+0x8B4]
$+260    004C9040      50                        PUSH EAX
$+261    004C9041      8D45 C8                   LEA EAX,DWORD PTR SS:[EBP-0x38]
$+264    004C9044      50                        PUSH EAX
$+265    004C9045      E8 56CD2800               CALL PathOfEx.00755DA0
$+26A    004C904A      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+26D    004C904D      83C4 08                   ADD ESP,0x8
$+270    004C9050      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+277    004C9057      5F                        POP EDI
$+278    004C9058      5E                        POP ESI
$+279    004C9059      5B                        POP EBX
$+27A    004C905A      8BE5                      MOV ESP,EBP
$+27C    004C905C      5D                        POP EBP
$+27D    004C905D      C3                        RETN
$+27E    004C905E      CC                        INT3
*/
/**/
//tag_返回当前选择的角色名
/*
$-71     005DCBAF      E8 6CFDFFFF                  CALL PathOfEx.005DC920
$-6C     005DCBB4      8D4D DC                      LEA ECX,DWORD PTR SS:[EBP-0x24]
$-69     005DCBB7      E8 34DEE8FF                  CALL PathOfEx.0046A9F0
$-64     005DCBBC      8D4D E4                      LEA ECX,DWORD PTR SS:[EBP-0x1C]
$-61     005DCBBF      E8 2CDEE8FF                  CALL PathOfEx.0046A9F0
$-5C     005DCBC4      EB 41                        JMP SHORT PathOfEx.005DCC07
$-5A     005DCBC6      3957 54                      CMP DWORD PTR DS:[EDI+0x54],EDX
$-57     005DCBC9      7C 24                        JL SHORT PathOfEx.005DCBEF
$-55     005DCBCB      7F 05                        JG SHORT PathOfEx.005DCBD2
$-53     005DCBCD      3947 50                      CMP DWORD PTR DS:[EDI+0x50],EAX
$-50     005DCBD0      76 1D                        JBE SHORT PathOfEx.005DCBEF
$-4E     005DCBD2      8B75 08                      MOV ESI,DWORD PTR SS:[EBP+0x8]
$-4B     005DCBD5      8BCB                         MOV ECX,EBX
$-49     005DCBD7      56                           PUSH ESI
$-48     005DCBD8      E8 43FDFFFF                  CALL PathOfEx.005DC920
$-43     005DCBDD      8D4D DC                      LEA ECX,DWORD PTR SS:[EBP-0x24]
$-40     005DCBE0      E8 0BDEE8FF                  CALL PathOfEx.0046A9F0
$-3B     005DCBE5      8D4D E4                      LEA ECX,DWORD PTR SS:[EBP-0x1C]
$-38     005DCBE8      E8 03DEE8FF                  CALL PathOfEx.0046A9F0
$-33     005DCBED      EB 18                        JMP SHORT PathOfEx.005DCC07
$-31     005DCBEF      8B75 08                      MOV ESI,DWORD PTR SS:[EBP+0x8]
$-2E     005DCBF2      8BCF                         MOV ECX,EDI
$-2C     005DCBF4      56                           PUSH ESI
$-2B     005DCBF5      E8 563CF3FF                  CALL PathOfEx.00510850
$-26     005DCBFA    ^ EB B8                        JMP SHORT PathOfEx.005DCBB4
$-24     005DCBFC      8B75 08                      MOV ESI,DWORD PTR SS:[EBP+0x8]
$-21     005DCBFF      8BCB                         MOV ECX,EBX
$-1F     005DCC01      56                           PUSH ESI
$-1E     005DCC02      E8 19FDFFFF                  CALL PathOfEx.005DC920
$-19     005DCC07      8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$-16     005DCC0A      8BC6                         MOV EAX,ESI
$-14     005DCC0C      5F                           POP EDI
$-13     005DCC0D      5E                           POP ESI
$-12     005DCC0E      64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$-B      005DCC15      5B                           POP EBX
$-A      005DCC16      8BE5                         MOV ESP,EBP
$-8      005DCC18      5D                           POP EBP
$-7      005DCC19      C2 0400                      RETN 0x4
$-4      005DCC1C      CC                           INT3
$-3      005DCC1D      CC                           INT3
$-2      005DCC1E      CC                           INT3
$-1      005DCC1F      CC                           INT3
$ ==>    005DCC20      55                           PUSH EBP                                         ; tag_返回当前选择的角色名,std::wstring
$+1      005DCC21      8BEC                         MOV EBP,ESP                                      ; ecx=stCD_UiSelectRoleTop*
$+3      005DCC23      6A FF                        PUSH -0x1
$+5      005DCC25      68 BD5AD300                  PUSH PathOfEx.00D35ABD
$+A      005DCC2A      64:A1 00000000               MOV EAX,DWORD PTR FS:[0]
$+10     005DCC30      50                           PUSH EAX
$+11     005DCC31      64:8925 00000000             MOV DWORD PTR FS:[0],ESP
$+18     005DCC38      83EC 14                      SUB ESP,0x14
$+1B     005DCC3B      53                           PUSH EBX
$+1C     005DCC3C      56                           PUSH ESI
$+1D     005DCC3D      57                           PUSH EDI
$+1E     005DCC3E      8D45 E0                      LEA EAX,DWORD PTR SS:[EBP-0x20]
$+21     005DCC41      C745 E8 00000000             MOV DWORD PTR SS:[EBP-0x18],0x0
$+28     005DCC48      8B89 8C0D0000                MOV ECX,DWORD PTR DS:[ECX+0xD8C]                 ; g_o_role_list_
$+2E     005DCC4E      50                           PUSH EAX
$+2F     005DCC4F      E8 3C64F3FF                  CALL PathOfEx.00513090                           ; call tag_得到当前选择的角色
$+34     005DCC54      C745 FC 01000000             MOV DWORD PTR SS:[EBP-0x4],0x1
$+3B     005DCC5B      8B4D E0                      MOV ECX,DWORD PTR SS:[EBP-0x20]
$+3E     005DCC5E      85C9                         TEST ECX,ECX
$+40     005DCC60      0F94C0                       SETE AL
$+43     005DCC63      34 01                        XOR AL,0x1
$+45     005DCC65      74 60                        JE SHORT PathOfEx.005DCCC7
$+47     005DCC67      8B09                         MOV ECX,DWORD PTR DS:[ECX]
$+49     005DCC69      8A81 54070000                MOV AL,BYTE PTR DS:[ECX+0x754]
$+4F     005DCC6F      84C0                         TEST AL,AL
$+51     005DCC71      74 54                        JE SHORT PathOfEx.005DCCC7
$+53     005DCC73      8B5D 08                      MOV EBX,DWORD PTR SS:[EBP+0x8]
$+56     005DCC76      53                           PUSH EBX
$+57     005DCC77      E8 44BDFFFF                  CALL PathOfEx.005D89C0                           ; call tag_得到角色列表项的角色名
$+5C     005DCC7C      C745 E8 01000000             MOV DWORD PTR SS:[EBP-0x18],0x1
$+63     005DCC83      C745 FC 02000000             MOV DWORD PTR SS:[EBP-0x4],0x2
$+6A     005DCC8A      8B75 E4                      MOV ESI,DWORD PTR SS:[EBP-0x1C]
$+6D     005DCC8D      85F6                         TEST ESI,ESI
$+6F     005DCC8F      74 21                        JE SHORT PathOfEx.005DCCB2
$+71     005DCC91      83CF FF                      OR EDI,0xFFFFFFFF
$+74     005DCC94      8BC7                         MOV EAX,EDI
$+76     005DCC96      F0:0FC146 04                 LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+7B     005DCC9B      75 15                        JNZ SHORT PathOfEx.005DCCB2
$+7D     005DCC9D      8B06                         MOV EAX,DWORD PTR DS:[ESI]
$+7F     005DCC9F      8BCE                         MOV ECX,ESI
$+81     005DCCA1      FF10                         CALL DWORD PTR DS:[EAX]
$+83     005DCCA3      F0:0FC17E 08                 LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+88     005DCCA8      4F                           DEC EDI
$+89     005DCCA9      75 07                        JNZ SHORT PathOfEx.005DCCB2
$+8B     005DCCAB      8B16                         MOV EDX,DWORD PTR DS:[ESI]
$+8D     005DCCAD      8BCE                         MOV ECX,ESI
$+8F     005DCCAF      FF52 04                      CALL DWORD PTR DS:[EDX+0x4]
$+92     005DCCB2      8BC3                         MOV EAX,EBX
$+94     005DCCB4      8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+97     005DCCB7      64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+9E     005DCCBE      5F                           POP EDI
$+9F     005DCCBF      5E                           POP ESI
$+A0     005DCCC0      5B                           POP EBX
$+A1     005DCCC1      8BE5                         MOV ESP,EBP
$+A3     005DCCC3      5D                           POP EBP
$+A4     005DCCC4      C2 0400                      RETN 0x4
$+A7     005DCCC7      8B75 08                      MOV ESI,DWORD PTR SS:[EBP+0x8]
$+AA     005DCCCA      C746 10 00000000             MOV DWORD PTR DS:[ESI+0x10],0x0
$+B1     005DCCD1      C746 14 00000000             MOV DWORD PTR DS:[ESI+0x14],0x0
$+B8     005DCCD8      C746 14 07000000             MOV DWORD PTR DS:[ESI+0x14],0x7
$+BF     005DCCDF      837E 14 08                   CMP DWORD PTR DS:[ESI+0x14],0x8
$+C3     005DCCE3      C746 10 00000000             MOV DWORD PTR DS:[ESI+0x10],0x0
$+CA     005DCCEA      72 04                        JB SHORT PathOfEx.005DCCF0
$+CC     005DCCEC      8B06                         MOV EAX,DWORD PTR DS:[ESI]
$+CE     005DCCEE      EB 02                        JMP SHORT PathOfEx.005DCCF2
$+D0     005DCCF0      8BC6                         MOV EAX,ESI
$+D2     005DCCF2      33C9                         XOR ECX,ECX
$+D4     005DCCF4      51                           PUSH ECX
$+D5     005DCCF5      66:8908                      MOV WORD PTR DS:[EAX],CX
$+D8     005DCCF8      8BCE                         MOV ECX,ESI
$+DA     005DCCFA      68 B4FCE600                  PUSH PathOfEx.00E6FCB4
$+DF     005DCCFF      E8 AC0FE9FF                  CALL PathOfEx.0046DCB0
$+E4     005DCD04      C745 E8 01000000             MOV DWORD PTR SS:[EBP-0x18],0x1
$+EB     005DCD0B      C745 FC 03000000             MOV DWORD PTR SS:[EBP-0x4],0x3
$+F2     005DCD12      8B5D E4                      MOV EBX,DWORD PTR SS:[EBP-0x1C]
$+F5     005DCD15      85DB                         TEST EBX,EBX
$+F7     005DCD17      74 21                        JE SHORT PathOfEx.005DCD3A
$+F9     005DCD19      83CF FF                      OR EDI,0xFFFFFFFF
$+FC     005DCD1C      8BC7                         MOV EAX,EDI
$+FE     005DCD1E      F0:0FC143 04                 LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+103    005DCD23      75 15                        JNZ SHORT PathOfEx.005DCD3A
$+105    005DCD25      8B03                         MOV EAX,DWORD PTR DS:[EBX]
$+107    005DCD27      8BCB                         MOV ECX,EBX
$+109    005DCD29      FF10                         CALL DWORD PTR DS:[EAX]
$+10B    005DCD2B      F0:0FC17B 08                 LOCK XADD DWORD PTR DS:[EBX+0x8],EDI
$+110    005DCD30      4F                           DEC EDI
$+111    005DCD31      75 07                        JNZ SHORT PathOfEx.005DCD3A
$+113    005DCD33      8B13                         MOV EDX,DWORD PTR DS:[EBX]
$+115    005DCD35      8BCB                         MOV ECX,EBX
$+117    005DCD37      FF52 04                      CALL DWORD PTR DS:[EDX+0x4]
$+11A    005DCD3A      8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+11D    005DCD3D      8BC6                         MOV EAX,ESI
$+11F    005DCD3F      5F                           POP EDI
$+120    005DCD40      5E                           POP ESI
$+121    005DCD41      64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+128    005DCD48      5B                           POP EBX
$+129    005DCD49      8BE5                         MOV ESP,EBP
$+12B    005DCD4B      5D                           POP EBP
$+12C    005DCD4C      C2 0400                      RETN 0x4
$+12F    005DCD4F      CC                           INT3
$+130    005DCD50      55                           PUSH EBP
$+131    005DCD51      8BEC                         MOV EBP,ESP
$+133    005DCD53      6A FF                        PUSH -0x1
$+135    005DCD55      68 D616D200                  PUSH PathOfEx.00D216D6                           ; ASCII "咐隰"
$+13A    005DCD5A      64:A1 00000000               MOV EAX,DWORD PTR FS:[0]
$+140    005DCD60      50                           PUSH EAX
$+141    005DCD61      64:8925 00000000             MOV DWORD PTR FS:[0],ESP
$+148    005DCD68      83EC 10                      SUB ESP,0x10
$+14B    005DCD6B      8B89 8C0D0000                MOV ECX,DWORD PTR DS:[ECX+0xD8C]
$+151    005DCD71      8D45 E8                      LEA EAX,DWORD PTR SS:[EBP-0x18]
$+154    005DCD74      53                           PUSH EBX
$+155    005DCD75      56                           PUSH ESI
$+156    005DCD76      57                           PUSH EDI
$+157    005DCD77      50                           PUSH EAX
$+158    005DCD78      E8 1363F3FF                  CALL PathOfEx.00513090
$+15D    005DCD7D      8B4D E8                      MOV ECX,DWORD PTR SS:[EBP-0x18]
$+160    005DCD80      85C9                         TEST ECX,ECX
$+162    005DCD82      0F94C0                       SETE AL
$+165    005DCD85      34 01                        XOR AL,0x1
$+167    005DCD87      74 18                        JE SHORT PathOfEx.005DCDA1
$+169    005DCD89      8B09                         MOV ECX,DWORD PTR DS:[ECX]
$+16B    005DCD8B      8A81 54070000                MOV AL,BYTE PTR DS:[ECX+0x754]
$+171    005DCD91      84C0                         TEST AL,AL
$+173    005DCD93      74 0C                        JE SHORT PathOfEx.005DCDA1
$+175    005DCD95      0FB6B1 04090000              MOVZX ESI,BYTE PTR DS:[ECX+0x904]
$+17C    005DCD9C      83E6 07                      AND ESI,0x7
$+17F    005DCD9F      EB 05                        JMP SHORT PathOfEx.005DCDA6
$+181    005DCDA1      BE 07000000                  MOV ESI,0x7
$+186    005DCDA6      C745 FC 00000000             MOV DWORD PTR SS:[EBP-0x4],0x0
$+18D    005DCDAD      8B7D EC                      MOV EDI,DWORD PTR SS:[EBP-0x14]
$+190    005DCDB0      85FF                         TEST EDI,EDI
$+192    005DCDB2      74 21                        JE SHORT PathOfEx.005DCDD5
$+194    005DCDB4      83CB FF                      OR EBX,0xFFFFFFFF
$+197    005DCDB7      8BC3                         MOV EAX,EBX
$+199    005DCDB9      F0:0FC147 04                 LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+19E    005DCDBE      75 15                        JNZ SHORT PathOfEx.005DCDD5
$+1A0    005DCDC0      8B07                         MOV EAX,DWORD PTR DS:[EDI]
$+1A2    005DCDC2      8BCF                         MOV ECX,EDI
$+1A4    005DCDC4      FF10                         CALL DWORD PTR DS:[EAX]
$+1A6    005DCDC6      F0:0FC15F 08                 LOCK XADD DWORD PTR DS:[EDI+0x8],EBX
$+1AB    005DCDCB      4B                           DEC EBX
$+1AC    005DCDCC      75 07                        JNZ SHORT PathOfEx.005DCDD5
$+1AE    005DCDCE      8B17                         MOV EDX,DWORD PTR DS:[EDI]
$+1B0    005DCDD0      8BCF                         MOV ECX,EDI
$+1B2    005DCDD2      FF52 04                      CALL DWORD PTR DS:[EDX+0x4]
$+1B5    005DCDD5      8B4D F4                      MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1B8    005DCDD8      8BC6                         MOV EAX,ESI
$+1BA    005DCDDA      5F                           POP EDI
$+1BB    005DCDDB      5E                           POP ESI
$+1BC    005DCDDC      64:890D 00000000             MOV DWORD PTR FS:[0],ECX
$+1C3    005DCDE3      5B                           POP EBX
$+1C4    005DCDE4      8BE5                         MOV ESP,EBP
$+1C6    005DCDE6      5D                           POP EBP
$+1C7    005DCDE7      C3                           RETN
$+1C8    005DCDE8      CC                           INT3
*/
//tag_判断是否可以前往藏身处
/*
$-41     0061448F     CC                   INT3
$-40     00614490     57                   PUSH EDI
$-3F     00614491     8BF9                 MOV EDI,ECX
$-3D     00614493     8B87 58070000        MOV EAX,DWORD PTR DS:[EDI+0x758]
$-37     00614499     3BB8 C40A0000        CMP EDI,DWORD PTR DS:[EAX+0xAC4]
$-31     0061449F     75 07                JNZ SHORT PathOfEx.006144A8
$-2F     006144A1     6A 00                PUSH 0x0
$-2D     006144A3     E8 48BF3D00          CALL PathOfEx.009F03F0
$-28     006144A8     FF7424 08            PUSH DWORD PTR SS:[ESP+0x8]
$-24     006144AC     8D8F 04040000        LEA ECX,DWORD PTR DS:[EDI+0x404]
$-1E     006144B2     57                   PUSH EDI
$-1D     006144B3     E8 48D3E7FF          CALL PathOfEx.00491800
$-18     006144B8     8B07                 MOV EAX,DWORD PTR DS:[EDI]
$-16     006144BA     8BCF                 MOV ECX,EDI
$-14     006144BC     FF7424 08            PUSH DWORD PTR SS:[ESP+0x8]
$-10     006144C0     FF90 E0000000        CALL DWORD PTR DS:[EAX+0xE0]
$-A      006144C6     5F                   POP EDI
$-9      006144C7     C2 0400              RETN 0x4
$-6      006144CA     CC                   INT3
$-5      006144CB     CC                   INT3
$-4      006144CC     CC                   INT3
$-3      006144CD     CC                   INT3
$-2      006144CE     CC                   INT3
$-1      006144CF     CC                   INT3
$ ==>    006144D0     55                   PUSH EBP                                        ; tag_判断是否可以前往藏身处
$+1      006144D1     8BEC                 MOV EBP,ESP                                     ; ecx=stCD_DlgWaypointMap*
$+3      006144D3     6A FF                PUSH -0x1
$+5      006144D5     68 F638D200          PUSH PathOfEx.00D238F6
$+A      006144DA     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
$+10     006144E0     50                   PUSH EAX
$+11     006144E1     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
$+18     006144E8     83EC 0C              SUB ESP,0xC
$+1B     006144EB     53                   PUSH EBX
$+1C     006144EC     56                   PUSH ESI
$+1D     006144ED     8BF1                 MOV ESI,ECX
$+1F     006144EF     33D2                 XOR EDX,EDX
$+21     006144F1     8955 F0              MOV DWORD PTR SS:[EBP-0x10],EDX
$+24     006144F4     3896 D0090000        CMP BYTE PTR DS:[ESI+0x9D0],DL
$+2A     006144FA     74 32                JE SHORT PathOfEx.0061452E
$+2C     006144FC     8B8E C0080000        MOV ECX,DWORD PTR DS:[ESI+0x8C0]
$+32     00614502     85C9                 TEST ECX,ECX
$+34     00614504     74 28                JE SHORT PathOfEx.0061452E
$+36     00614506     8D45 EC              LEA EAX,DWORD PTR SS:[EBP-0x14]
$+39     00614509     50                   PUSH EAX
$+3A     0061450A     E8 71AEFFFF          CALL PathOfEx.0060F380
$+3F     0061450F     BA 01000000          MOV EDX,0x1
$+44     00614514     8378 04 00           CMP DWORD PTR DS:[EAX+0x4],0x0
$+48     00614518     74 14                JE SHORT PathOfEx.0061452E
$+4A     0061451A     8B86 BC080000        MOV EAX,DWORD PTR DS:[ESI+0x8BC]
$+50     00614520     8A80 AC3E0000        MOV AL,BYTE PTR DS:[EAX+0x3EAC]
$+56     00614526     84C0                 TEST AL,AL
$+58     00614528     75 04                JNZ SHORT PathOfEx.0061452E
$+5A     0061452A     8ADA                 MOV BL,DL
$+5C     0061452C     EB 02                JMP SHORT PathOfEx.00614530
$+5E     0061452E     32DB                 XOR BL,BL
$+60     00614530     F6C2 01              TEST DL,0x1
$+63     00614533     74 1F                JE SHORT PathOfEx.00614554
$+65     00614535     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
$+6C     0061453C     8B45 EC              MOV EAX,DWORD PTR SS:[EBP-0x14]
$+6F     0061453F     85C0                 TEST EAX,EAX
$+71     00614541     74 11                JE SHORT PathOfEx.00614554
$+73     00614543     50                   PUSH EAX
$+74     00614544     51                   PUSH ECX
$+75     00614545     E8 665C1000          CALL PathOfEx.0071A1B0
$+7A     0061454A     83C4 04              ADD ESP,0x4
$+7D     0061454D     8BC8                 MOV ECX,EAX
$+7F     0061454F     E8 EC571000          CALL PathOfEx.00719D40
$+84     00614554     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$+87     00614557     8AC3                 MOV AL,BL
$+89     00614559     5E                   POP ESI
$+8A     0061455A     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+91     00614561     5B                   POP EBX
$+92     00614562     8BE5                 MOV ESP,EBP
$+94     00614564     5D                   POP EBP
$+95     00614565     C3                   RETN
$+96     00614566     CC                   INT3
$+97     00614567     CC                   INT3
$+98     00614568     CC                   INT3
$+99     00614569     CC                   INT3
$+9A     0061456A     CC                   INT3
$+9B     0061456B     CC                   INT3
$+9C     0061456C     CC                   INT3
$+9D     0061456D     CC                   INT3
$+9E     0061456E     CC                   INT3
$+9F     0061456F     CC                   INT3
$+A0     00614570     55                   PUSH EBP
$+A1     00614571     8BEC                 MOV EBP,ESP
$+A3     00614573     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
$+A9     00614579     6A FF                PUSH -0x1
$+AB     0061457B     68 FEB0D300          PUSH PathOfEx.00D3B0FE
$+B0     00614580     50                   PUSH EAX
$+B1     00614581     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
$+B8     00614588     83EC 38              SUB ESP,0x38
$+BB     0061458B     53                   PUSH EBX
$+BC     0061458C     56                   PUSH ESI
$+BD     0061458D     8BF1                 MOV ESI,ECX
$+BF     0061458F     57                   PUSH EDI
$+C0     00614590     83BE BC080000 00     CMP DWORD PTR DS:[ESI+0x8BC],0x0
$+C7     00614597     0F84 0B020000        JE PathOfEx.006147A8
$+CD     0061459D     E8 2EFFFFFF          CALL PathOfEx.006144D0
$+D2     006145A2     84C0                 TEST AL,AL
$+D4     006145A4     74 1B                JE SHORT PathOfEx.006145C1
$+D6     006145A6     8B86 58070000        MOV EAX,DWORD PTR DS:[ESI+0x758]
$+DC     006145AC     8B80 B80A0000        MOV EAX,DWORD PTR DS:[EAX+0xAB8]
$+E2     006145B2     3986 F4090000        CMP DWORD PTR DS:[ESI+0x9F4],EAX
$+E8     006145B8     74 07                JE SHORT PathOfEx.006145C1
$+EA     006145BA     B8 01000000          MOV EAX,0x1
$+EF     006145BF     EB 02                JMP SHORT PathOfEx.006145C3
$+F1     006145C1     33C0                 XOR EAX,EAX
$+F3     006145C3     8B8E F4090000        MOV ECX,DWORD PTR DS:[ESI+0x9F4]
$+F9     006145C9     50                   PUSH EAX
$+FA     006145CA     E8 D1723F00          CALL PathOfEx.00A0B8A0
$+FF     006145CF     6A 3A                PUSH 0x3A
$+101    006145D1     33C0                 XOR EAX,EAX
$+103    006145D3     C745 E8 07000000     MOV DWORD PTR SS:[EBP-0x18],0x7
$+10A    006145DA     68 40C4E900          PUSH PathOfEx.00E9C440                          ; Art/2DArt/UIImages/InGame/WorldPanelHideoutWaypointPinIcon
$+10F    006145DF     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+112    006145E2     C745 E4 00000000     MOV DWORD PTR SS:[EBP-0x1C],0x0
$+119    006145E9     66:8945 D4           MOV WORD PTR SS:[EBP-0x2C],AX
$+11D    006145ED     E8 BE96E5FF          CALL PathOfEx.0046DCB0
$+122    006145F2     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
$+129    006145F9     8D55 D4              LEA EDX,DWORD PTR SS:[EBP-0x2C]
$+12C    006145FC     8B8E F4090000        MOV ECX,DWORD PTR DS:[ESI+0x9F4]
$+132    00614602     81C1 B0080000        ADD ECX,0x8B0
$+138    00614608     52                   PUSH EDX
$+139    00614609     6A 01                PUSH 0x1
$+13B    0061460B     8B01                 MOV EAX,DWORD PTR DS:[ECX]
$+13D    0061460D     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
$+140    00614610     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
$+147    00614617     8B45 E8              MOV EAX,DWORD PTR SS:[EBP-0x18]
$+14A    0061461A     83F8 08              CMP EAX,0x8
$+14D    0061461D     72 0F                JB SHORT PathOfEx.0061462E
$+14F    0061461F     40                   INC EAX
$+150    00614620     6A 02                PUSH 0x2
$+152    00614622     50                   PUSH EAX
$+153    00614623     FF75 D4              PUSH DWORD PTR SS:[EBP-0x2C]
$+156    00614626     E8 0582E5FF          CALL PathOfEx.0046C830
$+15B    0061462B     83C4 0C              ADD ESP,0xC
$+15E    0061462E     8B86 BC080000        MOV EAX,DWORD PTR DS:[ESI+0x8BC]
$+164    00614634     8B96 F4090000        MOV EDX,DWORD PTR DS:[ESI+0x9F4]
$+16A    0061463A     8A80 AC3E0000        MOV AL,BYTE PTR DS:[EAX+0x3EAC]
$+170    00614640     8845 F0              MOV BYTE PTR SS:[EBP-0x10],AL
$+173    00614643     8B82 08070000        MOV EAX,DWORD PTR DS:[EDX+0x708]
$+179    00614649     3B82 0C070000        CMP EAX,DWORD PTR DS:[EDX+0x70C]
$+17F    0061464F     75 04                JNZ SHORT PathOfEx.00614655
$+181    00614651     33C9                 XOR ECX,ECX
$+183    00614653     EB 3A                JMP SHORT PathOfEx.0061468F
$+185    00614655     8B82 0C070000        MOV EAX,DWORD PTR DS:[EDX+0x70C]
$+18B    0061465B     8B92 08070000        MOV EDX,DWORD PTR DS:[EDX+0x708]
$+191    00614661     3BD0                 CMP EDX,EAX
$+193    00614663     74 24                JE SHORT PathOfEx.00614689
$+195    00614665     66:8B3D 9C310101     MOV DI,WORD PTR DS:[0x101319C]
$+19C    0061466C     0F1F40 00            NOP DWORD PTR DS:[EAX]
$+1A0    00614670     8B0A                 MOV ECX,DWORD PTR DS:[EDX]
$+1A2    00614672     0FB789 4C070000      MOVZX ECX,WORD PTR DS:[ECX+0x74C]
$+1A9    00614679     66:3BCF              CMP CX,DI
$+1AC    0061467C     74 07                JE SHORT PathOfEx.00614685
$+1AE    0061467E     83C2 04              ADD EDX,0x4
$+1B1    00614681     3BD0                 CMP EDX,EAX
$+1B3    00614683   ^ 75 EB                JNZ SHORT PathOfEx.00614670
$+1B5    00614685     3BD0                 CMP EDX,EAX
$+1B7    00614687     75 04                JNZ SHORT PathOfEx.0061468D
$+1B9    00614689     33C9                 XOR ECX,ECX
$+1BB    0061468B     EB 02                JMP SHORT PathOfEx.0061468F
$+1BD    0061468D     8B0A                 MOV ECX,DWORD PTR DS:[EDX]
$+1BF    0061468F     FF75 F0              PUSH DWORD PTR SS:[EBP-0x10]
$+1C2    00614692     E8 D9AA3D00          CALL PathOfEx.009EF170
$+1C7    00614697     8B86 F4090000        MOV EAX,DWORD PTR DS:[ESI+0x9F4]
$+1CD    0061469D     8B8E C0080000        MOV ECX,DWORD PTR DS:[ESI+0x8C0]
$+1D3    006146A3     8BB8 84070000        MOV EDI,DWORD PTR DS:[EAX+0x784]
$+1D9    006146A9     8D45 EC              LEA EAX,DWORD PTR SS:[EBP-0x14]
$+1DC    006146AC     50                   PUSH EAX
$+1DD    006146AD     E8 CEACFFFF          CALL PathOfEx.0060F380
$+1E2    006146B2     8B58 04              MOV EBX,DWORD PTR DS:[EAX+0x4]
$+1E5    006146B5     C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
$+1EC    006146BC     8B45 EC              MOV EAX,DWORD PTR SS:[EBP-0x14]
$+1EF    006146BF     85C0                 TEST EAX,EAX
$+1F1    006146C1     74 11                JE SHORT PathOfEx.006146D4
$+1F3    006146C3     50                   PUSH EAX
$+1F4    006146C4     51                   PUSH ECX
$+1F5    006146C5     E8 E65A1000          CALL PathOfEx.0071A1B0
$+1FA    006146CA     83C4 04              ADD ESP,0x4
$+1FD    006146CD     8BC8                 MOV ECX,EAX
$+1FF    006146CF     E8 6C561000          CALL PathOfEx.00719D40
$+204    006146D4     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
$+20B    006146DB     85DB                 TEST EBX,EBX
$+20D    006146DD     0F84 9D000000        JE PathOfEx.00614780
$+213    006146E3     80BE D0090000 00     CMP BYTE PTR DS:[ESI+0x9D0],0x0
$+21A    006146EA     74 6F                JE SHORT PathOfEx.0061475B
$+21C    006146EC     8B86 BC080000        MOV EAX,DWORD PTR DS:[ESI+0x8BC]
$+222    006146F2     8A80 AC3E0000        MOV AL,BYTE PTR DS:[EAX+0x3EAC]
$+228    006146F8     84C0                 TEST AL,AL
$+22A    006146FA     75 5F                JNZ SHORT PathOfEx.0061475B
$+22C    006146FC     BA FC050000          MOV EDX,0x5FC
$+231    00614701     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+234    00614704     E8 77AF1000          CALL PathOfEx.0071F680
$+239    00614709     8D8F B0080000        LEA ECX,DWORD PTR DS:[EDI+0x8B0]
$+23F    0061470F     C745 FC 02000000     MOV DWORD PTR SS:[EBP-0x4],0x2
$+246    00614716     8B11                 MOV EDX,DWORD PTR DS:[ECX]
$+248    00614718     50                   PUSH EAX
$+249    00614719     FF52 04              CALL DWORD PTR DS:[EDX+0x4]
$+24C    0061471C     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+24F    0061471F     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
$+256    00614726     E8 1590E5FF          CALL PathOfEx.0046D740
$+25B    0061472B     68 A0C5E900          PUSH PathOfEx.00E9C5A0                          ; Art/2DArt/UIImages/InGame/WorldPanelHideoutWaypointHighlightedPinIcon
$+260    00614730     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+263    00614733     E8 B896E5FF          CALL PathOfEx.0046DDF0
$+268    00614738     C745 FC 03000000     MOV DWORD PTR SS:[EBP-0x4],0x3
$+26F    0061473F     8D55 D4              LEA EDX,DWORD PTR SS:[EBP-0x2C]
$+272    00614742     8B8E F4090000        MOV ECX,DWORD PTR DS:[ESI+0x9F4]
$+278    00614748     81C1 B0080000        ADD ECX,0x8B0
$+27E    0061474E     52                   PUSH EDX
$+27F    0061474F     6A 01                PUSH 0x1
$+281    00614751     8B01                 MOV EAX,DWORD PTR DS:[ECX]
$+283    00614753     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
$+286    00614756     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+289    00614759     EB 48                JMP SHORT PathOfEx.006147A3
$+28B    0061475B     BA D5050000          MOV EDX,0x5D5
$+290    00614760     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+293    00614763     E8 18AF1000          CALL PathOfEx.0071F680
$+298    00614768     8D8F B0080000        LEA ECX,DWORD PTR DS:[EDI+0x8B0]
$+29E    0061476E     C745 FC 04000000     MOV DWORD PTR SS:[EBP-0x4],0x4
$+2A5    00614775     8B11                 MOV EDX,DWORD PTR DS:[ECX]
$+2A7    00614777     50                   PUSH EAX
$+2A8    00614778     FF52 04              CALL DWORD PTR DS:[EDX+0x4]
$+2AB    0061477B     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+2AE    0061477E     EB 23                JMP SHORT PathOfEx.006147A3
$+2B0    00614780     BA D5080000          MOV EDX,0x8D5
$+2B5    00614785     8D4D BC              LEA ECX,DWORD PTR SS:[EBP-0x44]
$+2B8    00614788     E8 F3AE1000          CALL PathOfEx.0071F680
$+2BD    0061478D     8D8F B0080000        LEA ECX,DWORD PTR DS:[EDI+0x8B0]
$+2C3    00614793     C745 FC 05000000     MOV DWORD PTR SS:[EBP-0x4],0x5
$+2CA    0061479A     8B11                 MOV EDX,DWORD PTR DS:[ECX]
$+2CC    0061479C     50                   PUSH EAX
$+2CD    0061479D     FF52 04              CALL DWORD PTR DS:[EDX+0x4]
$+2D0    006147A0     8D4D BC              LEA ECX,DWORD PTR SS:[EBP-0x44]
$+2D3    006147A3     E8 988FE5FF          CALL PathOfEx.0046D740
$+2D8    006147A8     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$+2DB    006147AB     5F                   POP EDI
$+2DC    006147AC     5E                   POP ESI
$+2DD    006147AD     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+2E4    006147B4     5B                   POP EBX
$+2E5    006147B5     8BE5                 MOV ESP,EBP
$+2E7    006147B7     5D                   POP EBP
$+2E8    006147B8     C3                   RETN
$+2E9    006147B9     CC                   INT3
*/

//call 选择要创建的角色模型,stCD_Vtbl_CharacterCreationInfo::kSelectCharacter

//call tag_选择要创建角色的职业
/*
$-188    007475BF      CC                   INT3
$-187    007475C0      55                   PUSH EBP                                 ; g_c_select_character_
$-186    007475C1      8BEC                 MOV EBP,ESP
$-184    007475C3      6A FF                PUSH -0x1
$-182    007475C5      68 5B45F600          PUSH PathOfEx.00F6455B
$-17D    007475CA      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
$-177    007475D0      50                   PUSH EAX
$-176    007475D1      64:8925 00000000     MOV DWORD PTR FS:[0],ESP
$-16F    007475D8      83EC 60              SUB ESP,0x60
$-16C    007475DB      C745 F0 00000000     MOV DWORD PTR SS:[EBP-0x10],0x0
$-165    007475E2      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
$-162    007475E5      53                   PUSH EBX
$-161    007475E6      56                   PUSH ESI
$-160    007475E7      57                   PUSH EDI
$-15F    007475E8      8038 00              CMP BYTE PTR DS:[EAX],0x0
$-15C    007475EB      8BF9                 MOV EDI,ECX
$-15A    007475ED      0F85 8E020000        JNZ PathOfEx.00747881
$-154    007475F3      8378 14 01           CMP DWORD PTR DS:[EAX+0x14],0x1
$-150    007475F7      C600 01              MOV BYTE PTR DS:[EAX],0x1
$-14D    007475FA      0F85 81020000        JNZ PathOfEx.00747881
$-147    00747600      BA 94030000          MOV EDX,0x394
$-142    00747605      8D4D 98              LEA ECX,DWORD PTR SS:[EBP-0x68]
$-13F    00747608      E8 53490700          CALL PathOfEx.007BBF60
$-13A    0074760D      8BF0                 MOV ESI,EAX
$-138    0074760F      C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
$-131    00747616      8D45 C8              LEA EAX,DWORD PTR SS:[EBP-0x38]
$-12E    00747619      8B4F 64              MOV ECX,DWORD PTR DS:[EDI+0x64]
$-12B    0074761C      81C1 D4080000        ADD ECX,0x8D4
$-125    00747622      50                   PUSH EAX
$-124    00747623      8B11                 MOV EDX,DWORD PTR DS:[ECX]
$-122    00747625      FF12                 CALL DWORD PTR DS:[EDX]
$-120    00747627      8BD6                 MOV EDX,ESI
$-11E    00747629      8BC8                 MOV ECX,EAX
$-11C    0074762B      E8 A092D3FF          CALL PathOfEx.004808D0
$-117    00747630      8D4D C8              LEA ECX,DWORD PTR SS:[EBP-0x38]
$-114    00747633      8AD8                 MOV BL,AL
$-112    00747635      E8 F66DD3FF          CALL PathOfEx.0047E430
$-10D    0074763A      8D4D 98              LEA ECX,DWORD PTR SS:[EBP-0x68]
$-10A    0074763D      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
$-103    00747644      E8 E76DD3FF          CALL PathOfEx.0047E430
$-FE     00747649      84DB                 TEST BL,BL
$-FC     0074764B      74 2E                JE SHORT PathOfEx.0074767B
$-FA     0074764D      68 30D90B01          PUSH PathOfEx.010BD930
$-F5     00747652      8D4D C8              LEA ECX,DWORD PTR SS:[EBP-0x38]
$-F2     00747655      E8 4674D3FF          CALL PathOfEx.0047EAA0
$-ED     0074765A      8D45 C8              LEA EAX,DWORD PTR SS:[EBP-0x38]
$-EA     0074765D      C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
$-E3     00747664      50                   PUSH EAX
$-E2     00747665      8BCF                 MOV ECX,EDI
$-E0     00747667      E8 74E8FFFF          CALL PathOfEx.00745EE0
$-DB     0074766C      8D4D C8              LEA ECX,DWORD PTR SS:[EBP-0x38]
$-D8     0074766F      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
$-D1     00747676      E8 B56DD3FF          CALL PathOfEx.0047E430
$-CC     0074767B      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
$-C9     0074767E      E8 0D03D4FF          CALL PathOfEx.00487990
$-C4     00747683      51                   PUSH ECX
$-C3     00747684      8D4D E0              LEA ECX,DWORD PTR SS:[EBP-0x20]
$-C0     00747687      51                   PUSH ECX
$-BF     00747688      8B08                 MOV ECX,DWORD PTR DS:[EAX]
$-BD     0074768A      E8 E174D4FF          CALL PathOfEx.0048EB70
$-B8     0074768F      C745 FC 03000000     MOV DWORD PTR SS:[EBP-0x4],0x3
$-B1     00747696      83CB FF              OR EBX,0xFFFFFFFF
$-AE     00747699      8B75 EC              MOV ESI,DWORD PTR SS:[EBP-0x14]
$-AB     0074769C      85F6                 TEST ESI,ESI
$-A9     0074769E      74 1F                JE SHORT PathOfEx.007476BF
$-A7     007476A0      8BC3                 MOV EAX,EBX
$-A5     007476A2      F0:0FC146 04         LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$-A0     007476A7      75 16                JNZ SHORT PathOfEx.007476BF
$-9E     007476A9      8B06                 MOV EAX,DWORD PTR DS:[ESI]
$-9C     007476AB      8BCE                 MOV ECX,ESI
$-9A     007476AD      FF10                 CALL DWORD PTR DS:[EAX]
$-98     007476AF      8BC3                 MOV EAX,EBX
$-96     007476B1      F0:0FC146 08         LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$-91     007476B6      75 07                JNZ SHORT PathOfEx.007476BF
$-8F     007476B8      8B06                 MOV EAX,DWORD PTR DS:[ESI]
$-8D     007476BA      8BCE                 MOV ECX,ESI
$-8B     007476BC      FF50 04              CALL DWORD PTR DS:[EAX+0x4]
$-88     007476BF      C645 FC 02           MOV BYTE PTR SS:[EBP-0x4],0x2
$-84     007476C3      8B4D E0              MOV ECX,DWORD PTR SS:[EBP-0x20]
$-81     007476C6      8B49 38              MOV ECX,DWORD PTR DS:[ECX+0x38]
$-7E     007476C9      8B01                 MOV EAX,DWORD PTR DS:[ECX]
$-7C     007476CB      FF50 68              CALL DWORD PTR DS:[EAX+0x68]
$-79     007476CE      8945 0C              MOV DWORD PTR SS:[EBP+0xC],EAX
$-76     007476D1      83F8 07              CMP EAX,0x7
$-73     007476D4      0F84 7B010000        JE PathOfEx.00747855
$-6D     007476DA      50                   PUSH EAX
$-6C     007476DB      8BCF                 MOV ECX,EDI
$-6A     007476DD      E8 BE050000          CALL PathOfEx.00747CA0                   ; call tag_选择要创建角色的职业
$-65     007476E2      8BCF                 MOV ECX,EDI
$-63     007476E4      E8 F7130000          CALL PathOfEx.00748AE0
$-5E     007476E9      8B57 58              MOV EDX,DWORD PTR DS:[EDI+0x58]          ; stCD_SceneCreateCharacterState.name_edit_
$-5B     007476EC      85D2                 TEST EDX,EDX
$-59     007476EE      74 10                JE SHORT PathOfEx.00747700
$-57     007476F0      8B8A 70070000        MOV ECX,DWORD PTR DS:[EDX+0x770]
$-51     007476F6      85C9                 TEST ECX,ECX
$-4F     007476F8      74 06                JE SHORT PathOfEx.00747700
$-4D     007476FA      52                   PUSH EDX
$-4C     007476FB      E8 B0243B00          CALL PathOfEx.00AF9BB0
$-47     00747700      837F 44 00           CMP DWORD PTR DS:[EDI+0x44],0x0
$-43     00747704      8D77 44              LEA ESI,DWORD PTR DS:[EDI+0x44]
$-40     00747707      74 33                JE SHORT PathOfEx.0074773C
$-3E     00747709      8B06                 MOV EAX,DWORD PTR DS:[ESI]
$-3C     0074770B      F3:0F1040 3C         MOVSS XMM0,DWORD PTR DS:[EAX+0x3C]
$-37     00747710      0F2F05 A4091B01      COMISS XMM0,DWORD PTR DS:[0x11B09A4]
$-30     00747717      76 0B                JBE SHORT PathOfEx.00747724
$-2E     00747719      6A 00                PUSH 0x0
$-2C     0074771B      8BCE                 MOV ECX,ESI
$-2A     0074771D      E8 3E0BE8FF          CALL PathOfEx.005C8260
$-25     00747722      EB 18                JMP SHORT PathOfEx.0074773C
$-23     00747724      F3:0F100D F80C1B01   MOVSS XMM1,DWORD PTR DS:[0x11B0CF8]
$-1B     0074772C      8BC8                 MOV ECX,EAX
$-19     0074772E      E8 ADF32900          CALL PathOfEx.009E6AE0
$-14     00747733      56                   PUSH ESI
$-13     00747734      8D4F 48              LEA ECX,DWORD PTR DS:[EDI+0x48]
$-10     00747737      E8 74160000          CALL PathOfEx.00748DB0
$-B      0074773C      8B45 E0              MOV EAX,DWORD PTR SS:[EBP-0x20]          ; then,eax=stCD_ScenePreGameState*
$-8      0074773F      FF75 0C              PUSH DWORD PTR SS:[EBP+0xC]
$-5      00747742      8B48 38              MOV ECX,DWORD PTR DS:[EAX+0x38]          ; g_so_character_creation_info_
$-2      00747745      8B01                 MOV EAX,DWORD PTR DS:[ECX]               ; ecx=stCD_CharacterCreationInfo*
$ ==>    00747747      FF50 3C              CALL DWORD PTR DS:[EAX+0x3C]             ; call 选择要创建的角色模型,stCD_Vtbl_CharacterCreationInfo::kSelectCharacter
$+3      0074774A      83EC 10              SUB ESP,0x10
$+6      0074774D      E8 9E110700          CALL PathOfEx.007B88F0
$+B      00747752      8BC8                 MOV ECX,EAX
$+D      00747754      E8 0704EEFF          CALL PathOfEx.00627B60
$+12     00747759      8BF0                 MOV ESI,EAX
$+14     0074775B      8975 F0              MOV DWORD PTR SS:[EBP-0x10],ESI
$+17     0074775E      C645 FC 04           MOV BYTE PTR SS:[EBP-0x4],0x4
$+1B     00747762      8D55 F0              LEA EDX,DWORD PTR SS:[EBP-0x10]
$+1E     00747765      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
$+21     00747768      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
$+24     0074776B      8D0440               LEA EAX,DWORD PTR DS:[EAX+EAX*2]
$+27     0074776E      8D04C5 683D2B01      LEA EAX,DWORD PTR DS:[EAX*8+0x12B3D68]
$+2E     00747775      50                   PUSH EAX
$+2F     00747776      E8 15640800          CALL PathOfEx.007CDB90
$+34     0074777B      83C4 04              ADD ESP,0x4
$+37     0074777E      C645 FC 07           MOV BYTE PTR SS:[EBP-0x4],0x7
$+3B     00747782      85F6                 TEST ESI,ESI
$+3D     00747784      74 11                JE SHORT PathOfEx.00747797
$+3F     00747786      56                   PUSH ESI
$+40     00747787      51                   PUSH ECX
$+41     00747788      E8 63110700          CALL PathOfEx.007B88F0
$+46     0074778D      83C4 04              ADD ESP,0x4
$+49     00747790      8BC8                 MOV ECX,EAX
$+4B     00747792      E8 B90C0700          CALL PathOfEx.007B8450
$+50     00747797      C645 FC 06           MOV BYTE PTR SS:[EBP-0x4],0x6
$+54     0074779B      8B45 EC              MOV EAX,DWORD PTR SS:[EBP-0x14]
$+57     0074779E      8B40 64              MOV EAX,DWORD PTR DS:[EAX+0x64]
$+5A     007477A1      66:8338 00           CMP WORD PTR DS:[EAX],0x0
$+5E     007477A5      74 7E                JE SHORT PathOfEx.00747825
$+60     007477A7      68 A8000000          PUSH 0xA8
$+65     007477AC      E8 08DB7500          CALL PathOfEx.00EA52B9
$+6A     007477B1      8BF0                 MOV ESI,EAX
$+6C     007477B3      83C4 04              ADD ESP,0x4
$+6F     007477B6      8975 0C              MOV DWORD PTR SS:[EBP+0xC],ESI
$+72     007477B9      C645 FC 08           MOV BYTE PTR SS:[EBP-0x4],0x8
$+76     007477BD      8D4D B0              LEA ECX,DWORD PTR SS:[EBP-0x50]
$+79     007477C0      8B45 EC              MOV EAX,DWORD PTR SS:[EBP-0x14]
$+7C     007477C3      FF70 64              PUSH DWORD PTR DS:[EAX+0x64]
$+7F     007477C6      E8 D572D3FF          CALL PathOfEx.0047EAA0
$+84     007477CB      83EC 08              SUB ESP,0x8
$+87     007477CE      C645 FC 09           MOV BYTE PTR SS:[EBP-0x4],0x9
$+8B     007477D2      0F57C0               XORPS XMM0,XMM0
$+8E     007477D5      C745 F0 01000000     MOV DWORD PTR SS:[EBP-0x10],0x1
$+95     007477DC      8D45 B0              LEA EAX,DWORD PTR SS:[EBP-0x50]
$+98     007477DF      8BCE                 MOV ECX,ESI
$+9A     007477E1      F20F110424           MOVSD QWORD PTR SS:[ESP],XMM0
$+9F     007477E6      6A 02                PUSH 0x2
$+A1     007477E8      6A 00                PUSH 0x0
$+A3     007477EA      50                   PUSH EAX
$+A4     007477EB      E8 80E22900          CALL PathOfEx.009E5A70
$+A9     007477F0      50                   PUSH EAX
$+AA     007477F1      8D4F 44              LEA ECX,DWORD PTR DS:[EDI+0x44]
$+AD     007477F4      E8 670AE8FF          CALL PathOfEx.005C8260
$+B2     007477F9      8D4D B0              LEA ECX,DWORD PTR SS:[EBP-0x50]
$+B5     007477FC      C745 FC 06000000     MOV DWORD PTR SS:[EBP-0x4],0x6
$+BC     00747803      E8 286CD3FF          CALL PathOfEx.0047E430
$+C1     00747808      8B4F 44              MOV ECX,DWORD PTR DS:[EDI+0x44]
$+C4     0074780B      8D47 20              LEA EAX,DWORD PTR DS:[EDI+0x20]
$+C7     0074780E      50                   PUSH EAX
$+C8     0074780F      E8 FCF12900          CALL PathOfEx.009E6A10
$+CD     00747814      8B4F 44              MOV ECX,DWORD PTR DS:[EDI+0x44]
$+D0     00747817      6A 00                PUSH 0x0
$+D2     00747819      C741 3C 00000040     MOV DWORD PTR DS:[ECX+0x3C],0x40000000   ; ASCII "thumb_r"
$+D9     00747820      E8 4BEE2900          CALL PathOfEx.009E6670
$+DE     00747825      8B4F 40              MOV ECX,DWORD PTR DS:[EDI+0x40]
$+E1     00747828      85C9                 TEST ECX,ECX
$+E3     0074782A      74 0D                JE SHORT PathOfEx.00747839
$+E5     0074782C      F3:0F100D F80C1B01   MOVSS XMM1,DWORD PTR DS:[0x11B0CF8]
$+ED     00747834      E8 A7F22900          CALL PathOfEx.009E6AE0
$+F2     00747839      C645 FC 0B           MOV BYTE PTR SS:[EBP-0x4],0xB
$+F6     0074783D      8B45 E8              MOV EAX,DWORD PTR SS:[EBP-0x18]
$+F9     00747840      85C0                 TEST EAX,EAX
$+FB     00747842      74 11                JE SHORT PathOfEx.00747855
$+FD     00747844      50                   PUSH EAX
$+FE     00747845      51                   PUSH ECX
$+FF     00747846      E8 A5100700          CALL PathOfEx.007B88F0
$+104    0074784B      83C4 04              ADD ESP,0x4
$+107    0074784E      8BC8                 MOV ECX,EAX
$+109    00747850      E8 FB0B0700          CALL PathOfEx.007B8450
$+10E    00747855      C745 FC 0C000000     MOV DWORD PTR SS:[EBP-0x4],0xC
$+115    0074785C      8B75 E4              MOV ESI,DWORD PTR SS:[EBP-0x1C]
$+118    0074785F      85F6                 TEST ESI,ESI
$+11A    00747861      74 1E                JE SHORT PathOfEx.00747881
$+11C    00747863      8BC3                 MOV EAX,EBX
$+11E    00747865      F0:0FC146 04         LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+123    0074786A      75 15                JNZ SHORT PathOfEx.00747881
$+125    0074786C      8B06                 MOV EAX,DWORD PTR DS:[ESI]
$+127    0074786E      8BCE                 MOV ECX,ESI
$+129    00747870      FF10                 CALL DWORD PTR DS:[EAX]
$+12B    00747872      F0:0FC15E 08         LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
$+130    00747877      4B                   DEC EBX
$+131    00747878      75 07                JNZ SHORT PathOfEx.00747881
$+133    0074787A      8B06                 MOV EAX,DWORD PTR DS:[ESI]
$+135    0074787C      8BCE                 MOV ECX,ESI
$+137    0074787E      FF50 04              CALL DWORD PTR DS:[EAX+0x4]
$+13A    00747881      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]           ; ret
$+13D    00747884      5F                   POP EDI
$+13E    00747885      5E                   POP ESI
$+13F    00747886      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+146    0074788D      5B                   POP EBX
$+147    0074788E      8BE5                 MOV ESP,EBP
$+149    00747890      5D                   POP EBP
$+14A    00747891      C2 0800              RETN 0x8
$+14D    00747894      CC                   INT3
$+14E    00747895      CC                   INT3
$+14F    00747896      CC                   INT3
$+150    00747897      CC                   INT3
$+151    00747898      CC                   INT3
$+152    00747899      CC                   INT3
$+153    0074789A      CC                   INT3
$+154    0074789B      CC                   INT3
$+155    0074789C      CC                   INT3
$+156    0074789D      CC                   INT3
$+157    0074789E      CC                   INT3
$+158    0074789F      CC                   INT3
$+159    007478A0      55                   PUSH EBP                                 ; g_c_press_create_role_buttons_
$+15A    007478A1      8BEC                 MOV EBP,ESP
$+15C    007478A3      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
$+162    007478A9      6A FF                PUSH -0x1
$+164    007478AB      68 EE46F400          PUSH PathOfEx.00F446EE
$+169    007478B0      50                   PUSH EAX
$+16A    007478B1      8B45 08              MOV EAX,DWORD PTR SS:[EBP+0x8]
$+16D    007478B4      64:8925 00000000     MOV DWORD PTR FS:[0],ESP
$+174    007478BB      83EC 10              SUB ESP,0x10
$+177    007478BE      56                   PUSH ESI
$+178    007478BF      8BF1                 MOV ESI,ECX
$+17A    007478C1      57                   PUSH EDI
$+17B    007478C2      3B86 8C000000        CMP EAX,DWORD PTR DS:[ESI+0x8C]
$+181    007478C8      75 17                JNZ SHORT PathOfEx.007478E1
$+183    007478CA      E8 B1F5FFFF          CALL PathOfEx.00746E80
$+188    007478CF      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$+18B    007478D2      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+192    007478D9      5F                   POP EDI
$+193    007478DA      5E                   POP ESI
$+194    007478DB      8BE5                 MOV ESP,EBP
$+196    007478DD      5D                   POP EBP
$+197    007478DE      C2 0400              RETN 0x4
$+19A    007478E1      3986 88000000        CMP DWORD PTR DS:[ESI+0x88],EAX
$+1A0    007478E7      75 17                JNZ SHORT PathOfEx.00747900
$+1A2    007478E9      E8 F2EFFFFF          CALL PathOfEx.007468E0
$+1A7    007478EE      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1AA    007478F1      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+1B1    007478F8      5F                   POP EDI
$+1B2    007478F9      5E                   POP ESI
$+1B3    007478FA      8BE5                 MOV ESP,EBP
$+1B5    007478FC      5D                   POP EBP
$+1B6    007478FD      C2 0400              RETN 0x4
$+1B9    00747900      3986 84000000        CMP DWORD PTR DS:[ESI+0x84],EAX
$+1BF    00747906      75 2E                JNZ SHORT PathOfEx.00747936
$+1C1    00747908      8B4E 54              MOV ECX,DWORD PTR DS:[ESI+0x54]
$+1C4    0074790B      E8 705A3900          CALL PathOfEx.00ADD380
$+1C9    00747910      84C0                 TEST AL,AL
$+1CB    00747912      74 22                JE SHORT PathOfEx.00747936
$+1CD    00747914      6A 07                PUSH 0x7
$+1CF    00747916      8BCE                 MOV ECX,ESI
$+1D1    00747918      E8 83030000          CALL PathOfEx.00747CA0
$+1D6    0074791D      8BCE                 MOV ECX,ESI
$+1D8    0074791F      E8 EC0A0000          CALL PathOfEx.00748410
$+1DD    00747924      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1E0    00747927      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
$+1E7    0074792E      5F                   POP EDI
$+1E8    0074792F      5E                   POP ESI
$+1E9    00747930      8BE5                 MOV ESP,EBP
$+1EB    00747932      5D                   POP EBP
$+1EC    00747933      C2 0400              RETN 0x4
$+1EF    00747936      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
$+1F2    00747939      E8 5200D4FF          CALL PathOfEx.00487990
$+1F7    0074793E      6A 01                PUSH 0x1
*/