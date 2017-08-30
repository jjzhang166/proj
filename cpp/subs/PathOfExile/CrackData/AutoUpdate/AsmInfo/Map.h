#pragma once
/*
@author zhp
@date 2017/2/6 15:21
@purpose for map asm record
*/

//tag_地图数据之系数_34
//8BC60FAFFA992BC2D1F803F881E601000080
/*
$ ==>    00490820     8B5424 04       MOV EDX, DWORD PTR SS:[ESP+0x4]                 ; 移动的时候会断住，计算地图点相关数据
$+4      00490824     56              PUSH ESI
$+5      00490825     8B32            MOV ESI, DWORD PTR DS:[EDX]                     ; edx=stCD_VecInt*
$+7      00490827     85F6            TEST ESI, ESI                                   ; esi=目标坐标x
$+9      00490829     78 55           JS SHORT PathOfEx.00490880                      ; 判断x是否为负
$+B      0049082B     8B52 04         MOV EDX, DWORD PTR DS:[EDX+0x4]
$+E      0049082E     85D2            TEST EDX, EDX                                   ; edx=目标坐标y
$+10     00490830     78 4E           JS SHORT PathOfEx.00490880					  ; 判断y是否为负
$+12     00490832     6B41 34 17      IMUL EAX, DWORD PTR DS:[ECX+0x34], 0x17         ; shrink_size_x
$+16     00490836     3BF0            CMP ESI, EAX									  ; eax=地图宽度
$+18     00490838     7D 46           JGE SHORT PathOfEx.00490880                     ; 目标坐标x大于等于地图宽度，则返回0
$+1A     0049083A     6B41 38 17      IMUL EAX, DWORD PTR DS:[ECX+0x38], 0x17         ; shrink_size_y
$+1E     0049083E     3BD0            CMP EDX, EAX									  ; eax=地图高度
$+20     00490840     7D 3E           JGE SHORT PathOfEx.00490880
$+22     00490842     57              PUSH EDI
$+23     00490843     8B79 54         MOV EDI, DWORD PTR DS:[ECX+0x54]                ; 不知道是干嘛的
$+26     00490846     8BC6            MOV EAX, ESI
$+28     00490848     0FAFFA          IMUL EDI, EDX                                   ; edi=edi*y坐标
$+2B     0049084B     99              CDQ                                             ; eax=目标坐标x,x坐标扩展符号位
$+2C     0049084C     2BC2            SUB EAX, EDX
$+2E     0049084E     D1F8            SAR EAX, 1                                      ; eax=目标坐标x/2
$+30     00490850     03F8            ADD EDI, EAX									  ; then,通过x和y计算得出一个索引值
$+32     00490852     81E6 01000080   AND ESI, 0x80000001
$+38     00490858     79 05           JNS SHORT PathOfEx.0049085F
$+3A     0049085A     4E              DEC ESI
$+3B     0049085B     83CE FE         OR ESI, -0x2
$+3E     0049085E     46              INC ESI
$+3F     0049085F     8B4424 10       MOV EAX, DWORD PTR SS:[ESP+0x10]                ; 第二个参数,第一个参数是stCD_VecInt*目标坐标指针
$+43     00490863     8D0440          LEA EAX, DWORD PTR DS:[EAX+EAX*2]
$+46     00490866     8B4481 3C       MOV EAX, DWORD PTR DS:[ECX+EAX*4+0x3C]          ; 0x3c存放的是地图点数据
$+4A     0049086A     0FB60438        MOVZX EAX, BYTE PTR DS:[EAX+EDI]                ; 获取目标坐标的地图字节数据
$+4E     0049086E     74 08           JE SHORT PathOfEx.00490878
$+50     00490870     5F              POP EDI                                         ; PathOfEx.008349D0
$+51     00490871     C1E8 04         SHR EAX, 0x4
$+54     00490874     5E              POP ESI                                         ; PathOfEx.008349D0
$+55     00490875     C2 0800         RETN 0x8
$+58     00490878     5F              POP EDI                                         ; PathOfEx.008349D0
$+59     00490879     83E0 0F         AND EAX, 0xF
$+5C     0049087C     5E              POP ESI                                         ; PathOfEx.008349D0
$+5D     0049087D     C2 0800         RETN 0x8
$+60     00490880     32C0            XOR AL, AL                                      ; 返回0
$+62     00490882     5E              POP ESI                                         ; PathOfEx.008349D0
$+63     00490883     C2 0800         RETN 0x8
*/
/**/
//tag_有关于WorldArea结构大小的信息
/*$
==>    007427C0     83EC 0C               SUB ESP, 0xC                             ; 只有一个参数，数组索引
$+3      007427C3     53                    PUSH EBX
$+4      007427C4     56                    PUSH ESI
$+5      007427C5     57                    PUSH EDI
$+6      007427C6     8BFA                  MOV EDI, EDX
$+8      007427C8     C74424 14 00000000    MOV DWORD PTR SS:[ESP+0x14], 0x0
$+10     007427D0     8BF1                  MOV ESI, ECX
$+12     007427D2     897424 10             MOV DWORD PTR SS:[ESP+0x10], ESI         ; 这个函数查找stCD_WorldAreaPairT，esi保存该指针
$+16     007427D6     8B0F                  MOV ECX, DWORD PTR DS:[EDI]
$+18     007427D8     85C9                  TEST ECX, ECX                            ; ecx=world area db
$+1A     007427DA     74 05                 JE SHORT PathOfEx.007427E1
$+1C     007427DC     E8 BF52FDFF           CALL PathOfEx.00717AA0
$+21     007427E1     8B0F                  MOV ECX, DWORD PTR DS:[EDI]
$+23     007427E3     8B5C24 1C             MOV EBX, DWORD PTR SS:[ESP+0x1C]
$+27     007427E7     8B41 30               MOV EAX, DWORD PTR DS:[ECX+0x30]
$+2A     007427EA     3B58 14               CMP EBX, DWORD PTR DS:[EAX+0x14]
$+2D     007427ED     72 16                 JB SHORT PathOfEx.00742805
$+2F     007427EF     C706 00000000         MOV DWORD PTR DS:[ESI], 0x0
$+35     007427F5     8BC6                  MOV EAX, ESI
$+37     007427F7     C746 04 00000000      MOV DWORD PTR DS:[ESI+0x4], 0x0
$+3E     007427FE     5F                    POP EDI
$+3F     007427FF     5E                    POP ESI
$+40     00742800     5B                    POP EBX
$+41     00742801     83C4 0C               ADD ESP, 0xC
$+44     00742804     C3                    RETN
$+45     00742805     85C9                  TEST ECX, ECX
$+47     00742807     74 05                 JE SHORT PathOfEx.0074280E
$+49     00742809     E8 9252FDFF           CALL PathOfEx.00717AA0
$+4E     0074280E     8B3F                  MOV EDI, DWORD PTR DS:[EDI]
$+50     00742810     8D145B                LEA EDX, DWORD PTR DS:[EBX+EBX*2]        ; ebx是tbl item的索引，从0开始
$+53     00742813     C1E2 07               SHL EDX, 0x7                             ; 执行完，当ebx=0时，edx=stCD_TblItemWorldArea的大小
$+56     00742816     8B47 30               MOV EAX, DWORD PTR DS:[EDI+0x30]
$+59     00742819     0350 04               ADD EDX, DWORD PTR DS:[EAX+0x4]
$+5C     0074281C     893E                  MOV DWORD PTR DS:[ESI], EDI
$+5E     0074281E     85FF                  TEST EDI, EDI
$+60     00742820     74 14                 JE SHORT PathOfEx.00742836
$+62     00742822     8D4F 38               LEA ECX, DWORD PTR DS:[EDI+0x38]
$+65     00742825     B8 01000000           MOV EAX, 0x1
$+6A     0074282A     F0:0FC101             LOCK XADD DWORD PTR DS:[ECX], EAX
$+6E     0074282E     A1 0C47FD00           MOV EAX, DWORD PTR DS:[0xFD470C]
$+73     00742833     8947 34               MOV DWORD PTR DS:[EDI+0x34], EAX
$+76     00742836     5F                    POP EDI
$+77     00742837     8956 04               MOV DWORD PTR DS:[ESI+0x4], EDX
$+7A     0074283A     8BC6                  MOV EAX, ESI
$+7C     0074283C     5E                    POP ESI
$+7D     0074283D     5B                    POP EBX
$+7E     0074283E     83C4 0C               ADD ESP, 0xC
$+81     00742841     C3                    RETN
*/
/**/
//call tag_复制目标区域的地图数据到vector中
//tag_打开门会断到_改写相应地图数据为可移动的
/*
$-2F3    0088633F     CC                      INT3
$-2F2    00886340     51                      PUSH ECX
$-2F1    00886341     56                      PUSH ESI
$-2F0    00886342     6A 3C                   PUSH 0x3C
$-2EE    00886344     8BF1                    MOV ESI,ECX
$-2EC    00886346     E8 CB633F00             CALL PathOfEx.00C7C716
$-2E7    0088634B     8B5424 10               MOV EDX,DWORD PTR SS:[ESP+0x10]
$-2E3    0088634F     83C4 04                 ADD ESP,0x4
$-2E0    00886352     894424 04               MOV DWORD PTR SS:[ESP+0x4],EAX
$-2DC    00886356     8950 04                 MOV DWORD PTR DS:[EAX+0x4],EDX
$-2D9    00886359     C640 08 00              MOV BYTE PTR DS:[EAX+0x8],0x0
$-2D5    0088635D     C740 0C DCABDD00        MOV DWORD PTR DS:[EAX+0xC],PathOfEx.00DDABDC
$-2CE    00886364     C740 10 307AEA00        MOV DWORD PTR DS:[EAX+0x10],PathOfEx.00EA7A30     ; ASCII "廓F"
$-2C7    0088636B     8970 14                 MOV DWORD PTR DS:[EAX+0x14],ESI
$-2C4    0088636E     C700 2071EF00           MOV DWORD PTR DS:[EAX],PathOfEx.00EF7120          ; 构造TriggerableBlockage属性
$-2BE    00886374     C740 0C 5C71EF00        MOV DWORD PTR DS:[EAX+0xC],PathOfEx.00EF715C
$-2B7    0088637B     C740 10 7071EF00        MOV DWORD PTR DS:[EAX+0x10],PathOfEx.00EF7170     ; ASCII "廓F"
$-2B0    00886382     C740 18 00000000        MOV DWORD PTR DS:[EAX+0x18],0x0
$-2A9    00886389     C640 1C 00              MOV BYTE PTR DS:[EAX+0x1C],0x0
$-2A5    0088638D     C740 20 00000000        MOV DWORD PTR DS:[EAX+0x20],0x0
$-29E    00886394     C740 24 00000000        MOV DWORD PTR DS:[EAX+0x24],0x0
$-297    0088639B     C740 28 00000000        MOV DWORD PTR DS:[EAX+0x28],0x0
$-290    008863A2     5E                      POP ESI
$-28F    008863A3     59                      POP ECX
$-28E    008863A4     C2 0400                 RETN 0x4
$-28B    008863A7     CC                      INT3
$-28A    008863A8     CC                      INT3
$-289    008863A9     CC                      INT3
$-288    008863AA     CC                      INT3
$-287    008863AB     CC                      INT3
$-286    008863AC     CC                      INT3
$-285    008863AD     CC                      INT3
$-284    008863AE     CC                      INT3
$-283    008863AF     CC                      INT3
$-282    008863B0     56                      PUSH ESI
$-281    008863B1     57                      PUSH EDI
$-280    008863B2     8B7C24 0C               MOV EDI,DWORD PTR SS:[ESP+0xC]
$-27C    008863B6     8BF1                    MOV ESI,ECX
$-27A    008863B8     BA EC70EF00             MOV EDX,PathOfEx.00EF70EC                         ; ASCII "start_blocked"
$-275    008863BD     8BCF                    MOV ECX,EDI
$-273    008863BF     E8 9CDDC1FF             CALL PathOfEx.004A4160
$-26E    008863C4     84C0                    TEST AL,AL
$-26C    008863C6     74 0E                   JE SHORT PathOfEx.008863D6
$-26A    008863C8     8A4424 10               MOV AL,BYTE PTR SS:[ESP+0x10]
$-266    008863CC     8846 08                 MOV BYTE PTR DS:[ESI+0x8],AL
$-263    008863CF     B0 01                   MOV AL,0x1
$-261    008863D1     5F                      POP EDI
$-260    008863D2     5E                      POP ESI
$-25F    008863D3     C2 0800                 RETN 0x8
$-25C    008863D6     BA DC70EF00             MOV EDX,PathOfEx.00EF70DC                         ; ASCII "save_as_diff"
$-257    008863DB     8BCF                    MOV ECX,EDI
$-255    008863DD     E8 7EDDC1FF             CALL PathOfEx.004A4160
$-250    008863E2     84C0                    TEST AL,AL
$-24E    008863E4     74 0E                   JE SHORT PathOfEx.008863F4
$-24C    008863E6     8A4424 10               MOV AL,BYTE PTR SS:[ESP+0x10]
$-248    008863EA     8846 09                 MOV BYTE PTR DS:[ESI+0x9],AL
$-245    008863ED     B0 01                   MOV AL,0x1
$-243    008863EF     5F                      POP EDI
$-242    008863F0     5E                      POP ESI
$-241    008863F1     C2 0800                 RETN 0x8
$-23E    008863F4     BA 1071EF00             MOV EDX,PathOfEx.00EF7110                         ; ASCII "disable_save"
$-239    008863F9     8BCF                    MOV ECX,EDI
$-237    008863FB     E8 60DDC1FF             CALL PathOfEx.004A4160
$-232    00886400     84C0                    TEST AL,AL
$-230    00886402     74 0E                   JE SHORT PathOfEx.00886412
$-22E    00886404     8A4424 10               MOV AL,BYTE PTR SS:[ESP+0x10]
$-22A    00886408     8846 0A                 MOV BYTE PTR DS:[ESI+0xA],AL
$-227    0088640B     B0 01                   MOV AL,0x1
$-225    0088640D     5F                      POP EDI
$-224    0088640E     5E                      POP ESI
$-223    0088640F     C2 0800                 RETN 0x8
$-220    00886412     5F                      POP EDI
$-21F    00886413     32C0                    XOR AL,AL
$-21D    00886415     5E                      POP ESI
$-21C    00886416     C2 0800                 RETN 0x8
$-219    00886419     CC                      INT3
$-218    0088641A     CC                      INT3
$-217    0088641B     CC                      INT3
$-216    0088641C     CC                      INT3
$-215    0088641D     CC                      INT3
$-214    0088641E     CC                      INT3
$-213    0088641F     CC                      INT3
$-212    00886420     51                      PUSH ECX
$-211    00886421     53                      PUSH EBX
$-210    00886422     56                      PUSH ESI
$-20F    00886423     57                      PUSH EDI
$-20E    00886424     8BF9                    MOV EDI,ECX
$-20C    00886426     8BDA                    MOV EBX,EDX
$-20A    00886428     33F6                    XOR ESI,ESI
$-208    0088642A     895C24 0C               MOV DWORD PTR SS:[ESP+0xC],EBX
$-204    0088642E     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$-201    00886431     2B07                    SUB EAX,DWORD PTR DS:[EDI]
$-1FF    00886433     74 45                   JE SHORT PathOfEx.0088647A
$-1FD    00886435     55                      PUSH EBP
$-1FC    00886436     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$-1FA    00886438     8B0B                    MOV ECX,DWORD PTR DS:[EBX]
$-1F8    0088643A     8D2C30                  LEA EBP,DWORD PTR DS:[EAX+ESI]
$-1F5    0088643D     8A45 00                 MOV AL,BYTE PTR SS:[EBP]
$-1F2    00886440     8A0C0E                  MOV CL,BYTE PTR DS:[ESI+ECX]
$-1EF    00886443     8AD0                    MOV DL,AL
$-1ED    00886445     8AD9                    MOV BL,CL
$-1EB    00886447     80E2 F0                 AND DL,0xF0
$-1E8    0088644A     80E3 F0                 AND BL,0xF0
$-1E5    0088644D     3AD3                    CMP DL,BL
$-1E3    0088644F     72 04                   JB SHORT PathOfEx.00886455
$-1E1    00886451     2AD3                    SUB DL,BL
$-1DF    00886453     EB 02                   JMP SHORT PathOfEx.00886457
$-1DD    00886455     32D2                    XOR DL,DL
$-1DB    00886457     24 0F                   AND AL,0xF
$-1D9    00886459     80E1 0F                 AND CL,0xF
$-1D6    0088645C     3AC1                    CMP AL,CL
$-1D4    0088645E     72 04                   JB SHORT PathOfEx.00886464
$-1D2    00886460     2AC1                    SUB AL,CL
$-1D0    00886462     EB 02                   JMP SHORT PathOfEx.00886466
$-1CE    00886464     32C0                    XOR AL,AL
$-1CC    00886466     8B5C24 10               MOV EBX,DWORD PTR SS:[ESP+0x10]
$-1C8    0088646A     0AC2                    OR AL,DL
$-1C6    0088646C     8845 00                 MOV BYTE PTR SS:[EBP],AL
$-1C3    0088646F     46                      INC ESI
$-1C2    00886470     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$-1BF    00886473     2B07                    SUB EAX,DWORD PTR DS:[EDI]
$-1BD    00886475     3BF0                    CMP ESI,EAX
$-1BB    00886477   ^ 72 BD                   JB SHORT PathOfEx.00886436
$-1B9    00886479     5D                      POP EBP
$-1B8    0088647A     5F                      POP EDI
$-1B7    0088647B     5E                      POP ESI
$-1B6    0088647C     5B                      POP EBX
$-1B5    0088647D     59                      POP ECX
$-1B4    0088647E     C3                      RETN
$-1B3    0088647F     CC                      INT3
$-1B2    00886480     51                      PUSH ECX
$-1B1    00886481     55                      PUSH EBP
$-1B0    00886482     57                      PUSH EDI
$-1AF    00886483     8BF9                    MOV EDI,ECX
$-1AD    00886485     895424 08               MOV DWORD PTR SS:[ESP+0x8],EDX
$-1A9    00886489     33ED                    XOR EBP,EBP
$-1A7    0088648B     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$-1A4    0088648E     2B07                    SUB EAX,DWORD PTR DS:[EDI]
$-1A2    00886490     74 50                   JE SHORT PathOfEx.008864E2
$-1A0    00886492     53                      PUSH EBX
$-19F    00886493     56                      PUSH ESI
$-19E    00886494     8B02                    MOV EAX,DWORD PTR DS:[EDX]
$-19C    00886496     8B37                    MOV ESI,DWORD PTR DS:[EDI]
$-19A    00886498     8A1428                  MOV DL,BYTE PTR DS:[EAX+EBP]
$-197    0088649B     8A0C2E                  MOV CL,BYTE PTR DS:[ESI+EBP]
$-194    0088649E     8ADA                    MOV BL,DL
$-192    008864A0     8AC1                    MOV AL,CL
$-190    008864A2     80E2 0F                 AND DL,0xF
$-18D    008864A5     24 F0                   AND AL,0xF0
$-18B    008864A7     80E1 0F                 AND CL,0xF
$-188    008864AA     02D1                    ADD DL,CL
$-186    008864AC     80E3 F0                 AND BL,0xF0
$-183    008864AF     02D8                    ADD BL,AL
$-181    008864B1     0FB6CA                  MOVZX ECX,DL
$-17E    008864B4     80FA 05                 CMP DL,0x5
$-17B    008864B7     B8 05000000             MOV EAX,0x5
$-176    008864BC     BA 50000000             MOV EDX,0x50
$-171    008864C1     0F47C8                  CMOVA ECX,EAX
$-16E    008864C4     80FB 50                 CMP BL,0x50
$-16B    008864C7     0FB6C3                  MOVZX EAX,BL
$-168    008864CA     0F47C2                  CMOVA EAX,EDX
$-165    008864CD     8B5424 10               MOV EDX,DWORD PTR SS:[ESP+0x10]
$-161    008864D1     0AC8                    OR CL,AL
$-15F    008864D3     880C2E                  MOV BYTE PTR DS:[ESI+EBP],CL
$-15C    008864D6     45                      INC EBP
$-15B    008864D7     8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$-158    008864DA     2B07                    SUB EAX,DWORD PTR DS:[EDI]
$-156    008864DC     3BE8                    CMP EBP,EAX
$-154    008864DE   ^ 72 B4                   JB SHORT PathOfEx.00886494
$-152    008864E0     5E                      POP ESI
$-151    008864E1     5B                      POP EBX
$-150    008864E2     5F                      POP EDI
$-14F    008864E3     5D                      POP EBP
$-14E    008864E4     59                      POP ECX
$-14D    008864E5     C3                      RETN
$-14C    008864E6     CC                      INT3
$-14B    008864E7     CC                      INT3
$-14A    008864E8     CC                      INT3
$-149    008864E9     CC                      INT3
$-148    008864EA     CC                      INT3
$-147    008864EB     CC                      INT3
$-146    008864EC     CC                      INT3
$-145    008864ED     CC                      INT3
$-144    008864EE     CC                      INT3
$-143    008864EF     CC                      INT3
$-142    008864F0     55                      PUSH EBP
$-141    008864F1     8BEC                    MOV EBP,ESP
$-13F    008864F3     6A FF                   PUSH -0x1
$-13D    008864F5     68 B85AD400             PUSH PathOfEx.00D45AB8
$-138    008864FA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-132    00886500     50                      PUSH EAX
$-131    00886501     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-12A    00886508     83EC 40                 SUB ESP,0x40
$-127    0088650B     53                      PUSH EBX
$-126    0088650C     8BD9                    MOV EBX,ECX
$-124    0088650E     C745 E0 00000000        MOV DWORD PTR SS:[EBP-0x20],0x0
$-11D    00886515     56                      PUSH ESI
$-11C    00886516     57                      PUSH EDI
$-11B    00886517     895D EC                 MOV DWORD PTR SS:[EBP-0x14],EBX
$-118    0088651A     807B 18 00              CMP BYTE PTR DS:[EBX+0x18],0x0
$-114    0088651E     0F85 4A020000           JNZ PathOfEx.0088676E
$-10E    00886524     8B4B 04                 MOV ECX,DWORD PTR DS:[EBX+0x4]
$-10B    00886527     E8 C457BEFF             CALL PathOfEx.0046BCF0
$-106    0088652C     8B48 2C                 MOV ECX,DWORD PTR DS:[EAX+0x2C]
$-103    0088652F     85C9                    TEST ECX,ECX
$-101    00886531     0F84 37020000           JE PathOfEx.0088676E
$-FB     00886537     E8 0487F5FF             CALL PathOfEx.007DEC40
$-F6     0088653C     8B4B 04                 MOV ECX,DWORD PTR DS:[EBX+0x4]
$-F3     0088653F     8945 E4                 MOV DWORD PTR SS:[EBP-0x1C],EAX
$-F0     00886542     8B79 20                 MOV EDI,DWORD PTR DS:[ECX+0x20]
$-ED     00886545     8B49 10                 MOV ECX,DWORD PTR DS:[ECX+0x10]
$-EA     00886548     8B11                    MOV EDX,DWORD PTR DS:[ECX]
$-E8     0088654A     FF52 0C                 CALL DWORD PTR DS:[EDX+0xC]
$-E5     0088654D     837F 10 00              CMP DWORD PTR DS:[EDI+0x10],0x0
$-E1     00886551     8BF0                    MOV ESI,EAX
$-DF     00886553     0F8C 15020000           JL PathOfEx.0088676E
$-D9     00886559     8B57 14                 MOV EDX,DWORD PTR DS:[EDI+0x14]
$-D6     0088655C     85D2                    TEST EDX,EDX
$-D4     0088655E     0F88 0A020000           JS PathOfEx.0088676E
$-CE     00886564     6B46 34 17              IMUL EAX,DWORD PTR DS:[ESI+0x34],0x17
$-CA     00886568     3947 10                 CMP DWORD PTR DS:[EDI+0x10],EAX
$-C7     0088656B     0F8D FD010000           JGE PathOfEx.0088676E
$-C1     00886571     6B46 38 17              IMUL EAX,DWORD PTR DS:[ESI+0x38],0x17
$-BD     00886575     3BD0                    CMP EDX,EAX
$-BB     00886577     0F8D F1010000           JGE PathOfEx.0088676E
$-B5     0088657D     8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$-B2     00886580     8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$-AF     00886583     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-AD     00886585     FF50 0C                 CALL DWORD PTR DS:[EAX+0xC]
$-AA     00886588     8B4B 14                 MOV ECX,DWORD PTR DS:[EBX+0x14]
$-A7     0088658B     8945 F0                 MOV DWORD PTR SS:[EBP-0x10],EAX
$-A4     0088658E     8079 0A 00              CMP BYTE PTR DS:[ECX+0xA],0x0
$-A0     00886592     0F85 B2000000           JNZ PathOfEx.0088664A
$-9A     00886598     8B4F 2C                 MOV ECX,DWORD PTR DS:[EDI+0x2C]
$-97     0088659B     C1E9 04                 SHR ECX,0x4
$-94     0088659E     80E1 01                 AND CL,0x1
$-91     008865A1     884D E8                 MOV BYTE PTR SS:[EBP-0x18],CL
$-8E     008865A4     8BCF                    MOV ECX,EDI
$-8C     008865A6     E8 058AE6FF             CALL PathOfEx.006EEFB0
$-87     008865AB     51                      PUSH ECX
$-86     008865AC     F3:0F110424             MOVSS DWORD PTR SS:[ESP],XMM0
$-81     008865B1     FF75 E8                 PUSH DWORD PTR SS:[EBP-0x18]
$-7E     008865B4     E8 E751C1FF             CALL PathOfEx.0049B7A0
$-79     008865B9     8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
$-76     008865BC     8D45 C4                 LEA EAX,DWORD PTR SS:[EBP-0x3C]
$-73     008865BF     50                      PUSH EAX
$-72     008865C0     0F28D0                  MOVAPS XMM2,XMM0
$-6F     008865C3     E8 C830FBFF             CALL PathOfEx.00839690
$-6A     008865C8     8D73 2C                 LEA ESI,DWORD PTR DS:[EBX+0x2C]
$-67     008865CB     8BCE                    MOV ECX,ESI
$-65     008865CD     0F1000                  MOVUPS XMM0,DQWORD PTR DS:[EAX]
$-62     008865D0     C745 C4 00000000        MOV DWORD PTR SS:[EBP-0x3C],0x0
$-5B     008865D7     C745 C8 00000000        MOV DWORD PTR SS:[EBP-0x38],0x0
$-54     008865DE     0F1106                  MOVUPS DQWORD PTR DS:[ESI],XMM0
$-51     008865E1     8306 FB                 ADD DWORD PTR DS:[ESI],-0x5
$-4E     008865E4     8346 04 FB              ADD DWORD PTR DS:[ESI+0x4],-0x5
$-4A     008865E8     8343 34 05              ADD DWORD PTR DS:[EBX+0x34],0x5
$-46     008865EC     8343 38 05              ADD DWORD PTR DS:[EBX+0x38],0x5
$-42     008865F0     8B47 10                 MOV EAX,DWORD PTR DS:[EDI+0x10]
$-3F     008865F3     0106                    ADD DWORD PTR DS:[ESI],EAX
$-3D     008865F5     8B47 14                 MOV EAX,DWORD PTR DS:[EDI+0x14]
$-3A     008865F8     0146 04                 ADD DWORD PTR DS:[ESI+0x4],EAX
$-37     008865FB     8B47 10                 MOV EAX,DWORD PTR DS:[EDI+0x10]
$-34     008865FE     0146 08                 ADD DWORD PTR DS:[ESI+0x8],EAX
$-31     00886601     8B47 14                 MOV EAX,DWORD PTR DS:[EDI+0x14]
$-2E     00886604     8B7D F0                 MOV EDI,DWORD PTR SS:[EBP-0x10]
$-2B     00886607     0146 0C                 ADD DWORD PTR DS:[ESI+0xC],EAX
$-28     0088660A     6B47 34 17              IMUL EAX,DWORD PTR DS:[EDI+0x34],0x17
$-24     0088660E     8945 CC                 MOV DWORD PTR SS:[EBP-0x34],EAX
$-21     00886611     6B47 38 17              IMUL EAX,DWORD PTR DS:[EDI+0x38],0x17
$-1D     00886615     8945 D0                 MOV DWORD PTR SS:[EBP-0x30],EAX
$-1A     00886618     8D45 C4                 LEA EAX,DWORD PTR SS:[EBP-0x3C]
$-17     0088661B     50                      PUSH EAX
$-16     0088661C     8D45 B4                 LEA EAX,DWORD PTR SS:[EBP-0x4C]
$-13     0088661F     50                      PUSH EAX
$-12     00886620     E8 EB08E8FF             CALL PathOfEx.00706F10
$-D      00886625     56                      PUSH ESI                                          ; esi=stCD_EntityAttrTriggerableBlockage.dst_rect_
$-C      00886626     8BCF                    MOV ECX,EDI                                       ; edi=stCD_MapData*
$-A      00886628     0F1000                  MOVUPS XMM0,DQWORD PTR DS:[EAX]
$-7      0088662B     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
$-4      0088662E     50                      PUSH EAX                                          ; eax=stCD_MapDataVector*
$-3      0088662F     0F1106                  MOVUPS DQWORD PTR DS:[ESI],XMM0
$ ==>    00886632     E8 D973F5FF             CALL PathOfEx.007DDA10                            ; call tag_复制目标区域的地图数据到vector中
$+5      00886637     50                      PUSH EAX
$+6      00886638     8D4B 20                 LEA ECX,DWORD PTR DS:[EBX+0x20]
$+9      0088663B     E8 5079E5FF             CALL PathOfEx.006DDF90
$+E      00886640     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
$+11     00886643     E8 E87AE3FF             CALL PathOfEx.006BE130
$+16     00886648     EB 03                   JMP SHORT PathOfEx.0088664D
$+18     0088664A     8B7D F0                 MOV EDI,DWORD PTR SS:[EBP-0x10]
$+1B     0088664D     FF73 04                 PUSH DWORD PTR DS:[EBX+0x4]
$+1E     00886650     8BCF                    MOV ECX,EDI
$+20     00886652     E8 8970F5FF             CALL PathOfEx.007DD6E0
$+25     00886657     8B43 14                 MOV EAX,DWORD PTR DS:[EBX+0x14]
$+28     0088665A     8078 09 00              CMP BYTE PTR DS:[EAX+0x9],0x0
$+2C     0088665E     74 28                   JE SHORT PathOfEx.00886688
$+2E     00886660     8078 0A 00              CMP BYTE PTR DS:[EAX+0xA],0x0
$+32     00886664     75 22                   JNZ SHORT PathOfEx.00886688
$+34     00886666     8D43 2C                 LEA EAX,DWORD PTR DS:[EBX+0x2C]
$+37     00886669     8BCF                    MOV ECX,EDI
$+39     0088666B     50                      PUSH EAX
$+3A     0088666C     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
$+3D     0088666F     50                      PUSH EAX
$+3E     00886670     E8 9B73F5FF             CALL PathOfEx.007DDA10
$+43     00886675     8D4B 20                 LEA ECX,DWORD PTR DS:[EBX+0x20]
$+46     00886678     8D55 C8                 LEA EDX,DWORD PTR SS:[EBP-0x38]
$+49     0088667B     E8 A0FDFFFF             CALL PathOfEx.00886420
$+4E     00886680     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
$+51     00886683     E8 A87AE3FF             CALL PathOfEx.006BE130
$+56     00886688     C745 D4 00000000        MOV DWORD PTR SS:[EBP-0x2C],0x0
$+5D     0088668F     C745 D8 00000000        MOV DWORD PTR SS:[EBP-0x28],0x0
$+64     00886696     C745 DC 00000000        MOV DWORD PTR SS:[EBP-0x24],0x0
$+6B     0088669D     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+72     008866A4     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+75     008866A7     83EC 28                 SUB ESP,0x28
$+78     008866AA     8BC4                    MOV EAX,ESP
$+7A     008866AC     83EC 10                 SUB ESP,0x10
$+7D     008866AF     C700 8871EF00           MOV DWORD PTR DS:[EAX],PathOfEx.00EF7188
$+83     008866B5     8978 04                 MOV DWORD PTR DS:[EAX+0x4],EDI
$+86     008866B8     8948 08                 MOV DWORD PTR DS:[EAX+0x8],ECX
$+89     008866BB     8940 24                 MOV DWORD PTR DS:[EAX+0x24],EAX
$+8C     008866BE     B8 C94216B2             MOV EAX,0xB21642C9
$+91     008866C3     8B4B 34                 MOV ECX,DWORD PTR DS:[EBX+0x34]
$+94     008866C6     8B73 38                 MOV ESI,DWORD PTR DS:[EBX+0x38]
$+97     008866C9     49                      DEC ECX
$+98     008866CA     F7E9                    IMUL ECX
$+9A     008866CC     4E                      DEC ESI
$+9B     008866CD     B8 C94216B2             MOV EAX,0xB21642C9
$+A0     008866D2     03D1                    ADD EDX,ECX
$+A2     008866D4     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$+A5     008866D7     C1FA 04                 SAR EDX,0x4
$+A8     008866DA     8BFA                    MOV EDI,EDX
$+AA     008866DC     C1EF 1F                 SHR EDI,0x1F
$+AD     008866DF     8B49 2C                 MOV ECX,DWORD PTR DS:[ECX+0x2C]
$+B0     008866E2     03FA                    ADD EDI,EDX
$+B2     008866E4     F7EE                    IMUL ESI
$+B4     008866E6     B8 C94216B2             MOV EAX,0xB21642C9
$+B9     008866EB     47                      INC EDI
$+BA     008866EC     03D6                    ADD EDX,ESI
$+BC     008866EE     897D CC                 MOV DWORD PTR SS:[EBP-0x34],EDI
$+BF     008866F1     C1FA 04                 SAR EDX,0x4
$+C2     008866F4     8BDA                    MOV EBX,EDX
$+C4     008866F6     42                      INC EDX
$+C5     008866F7     C1EB 1F                 SHR EBX,0x1F
$+C8     008866FA     03DA                    ADD EBX,EDX
$+CA     008866FC     F7E9                    IMUL ECX
$+CC     008866FE     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+CF     00886701     03D1                    ADD EDX,ECX
$+D1     00886703     895D D0                 MOV DWORD PTR SS:[EBP-0x30],EBX
$+D4     00886706     C1FA 04                 SAR EDX,0x4
$+D7     00886709     8BF2                    MOV ESI,EDX
$+D9     0088670B     8B48 30                 MOV ECX,DWORD PTR DS:[EAX+0x30]
$+DC     0088670E     B8 C94216B2             MOV EAX,0xB21642C9
$+E1     00886713     C1EE 1F                 SHR ESI,0x1F
$+E4     00886716     03F2                    ADD ESI,EDX
$+E6     00886718     F7E9                    IMUL ECX
$+E8     0088671A     8975 C4                 MOV DWORD PTR SS:[EBP-0x3C],ESI
$+EB     0088671D     03D1                    ADD EDX,ECX
$+ED     0088671F     8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$+F0     00886722     C1FA 04                 SAR EDX,0x4
$+F3     00886725     8BC2                    MOV EAX,EDX
$+F5     00886727     C1E8 1F                 SHR EAX,0x1F
$+F8     0088672A     03C2                    ADD EAX,EDX
$+FA     0088672C     8945 C8                 MOV DWORD PTR SS:[EBP-0x38],EAX
$+FD     0088672F     8BC4                    MOV EAX,ESP
$+FF     00886731     0F1045 C4               MOVUPS XMM0,DQWORD PTR SS:[EBP-0x3C]
$+103    00886735     0F1100                  MOVUPS DQWORD PTR DS:[EAX],XMM0
$+106    00886738     E8 F35CF5FF             CALL PathOfEx.007DC430
$+10B    0088673D     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+10E    00886740     8B4D 08                 MOV ECX,DWORD PTR SS:[EBP+0x8]
$+111    00886743     C640 18 01              MOV BYTE PTR DS:[EAX+0x18],0x1
$+115    00886747     8D45 D4                 LEA EAX,DWORD PTR SS:[EBP-0x2C]
$+118    0088674A     50                      PUSH EAX
$+119    0088674B     E8 F0ABF5FF             CALL PathOfEx.007E1340
$+11E    00886750     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+121    00886753     E8 083FBFFF             CALL PathOfEx.0047A660
$+126    00886758     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+129    0088675B     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+12C    0088675E     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+133    00886765     5F                      POP EDI
$+134    00886766     5E                      POP ESI
$+135    00886767     5B                      POP EBX
$+136    00886768     8BE5                    MOV ESP,EBP
$+138    0088676A     5D                      POP EBP
$+139    0088676B     C2 0400                 RETN 0x4
$+13C    0088676E     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+13F    00886771     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+142    00886774     5F                      POP EDI
$+143    00886775     5E                      POP ESI
$+144    00886776     C740 08 00000000        MOV DWORD PTR DS:[EAX+0x8],0x0
$+14B    0088677D     C740 04 00000000        MOV DWORD PTR DS:[EAX+0x4],0x0
$+152    00886784     C700 00000000           MOV DWORD PTR DS:[EAX],0x0
$+158    0088678A     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+15F    00886791     5B                      POP EBX
$+160    00886792     8BE5                    MOV ESP,EBP
$+162    00886794     5D                      POP EBP
$+163    00886795     C2 0400                 RETN 0x4
$+166    00886798     CC                      INT3
$+167    00886799     CC                      INT3
$+168    0088679A     CC                      INT3
$+169    0088679B     CC                      INT3
$+16A    0088679C     CC                      INT3
$+16B    0088679D     CC                      INT3
$+16C    0088679E     CC                      INT3
$+16D    0088679F     CC                      INT3
$+16E    008867A0     83EC 0C                 SUB ESP,0xC                                       ; tag_打开门会断到_改写相应地图数据为可移动的
$+171    008867A3     56                      PUSH ESI                                          ; ecx=stCD_EntityAttrTriggerableBlockage*
$+172    008867A4     8BF1                    MOV ESI,ECX
$+174    008867A6     807E 18 00              CMP BYTE PTR DS:[ESI+0x18],0x0
$+178    008867AA     74 5D                   JE SHORT PathOfEx.00886809
$+17A    008867AC     8B46 14                 MOV EAX,DWORD PTR DS:[ESI+0x14]
$+17D    008867AF     8078 0A 00              CMP BYTE PTR DS:[EAX+0xA],0x0
$+181    008867B3     75 54                   JNZ SHORT PathOfEx.00886809
$+183    008867B5     8078 09 00              CMP BYTE PTR DS:[EAX+0x9],0x0
$+187    008867B9     74 30                   JE SHORT PathOfEx.008867EB
$+189    008867BB     8B46 04                 MOV EAX,DWORD PTR DS:[ESI+0x4]
$+18C    008867BE     8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$+18F    008867C1     8D46 2C                 LEA EAX,DWORD PTR DS:[ESI+0x2C]
$+192    008867C4     50                      PUSH EAX
$+193    008867C5     8D4424 08               LEA EAX,DWORD PTR SS:[ESP+0x8]
$+197    008867C9     50                      PUSH EAX
$+198    008867CA     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+19A    008867CC     FF50 0C                 CALL DWORD PTR DS:[EAX+0xC]
$+19D    008867CF     8BC8                    MOV ECX,EAX
$+19F    008867D1     E8 3A72F5FF             CALL PathOfEx.007DDA10
$+1A4    008867D6     8D4E 20                 LEA ECX,DWORD PTR DS:[ESI+0x20]
$+1A7    008867D9     8D5424 04               LEA EDX,DWORD PTR SS:[ESP+0x4]
$+1AB    008867DD     E8 9EFCFFFF             CALL PathOfEx.00886480
$+1B0    008867E2     8D4C24 04               LEA ECX,DWORD PTR SS:[ESP+0x4]
$+1B4    008867E6     E8 4579E3FF             CALL PathOfEx.006BE130
$+1B9    008867EB     8B46 04                 MOV EAX,DWORD PTR DS:[ESI+0x4]                    ; then,eax=实体对象
$+1BC    008867EE     8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]                   ; then,ecx=stCD_World*
$+1BF    008867F1     8D46 20                 LEA EAX,DWORD PTR DS:[ESI+0x20]
$+1C2    008867F4     50                      PUSH EAX                                          ; 地图数据,vector
$+1C3    008867F5     8D46 2C                 LEA EAX,DWORD PTR DS:[ESI+0x2C]
$+1C6    008867F8     50                      PUSH EAX                                          ; 要改写的目标区域
$+1C7    008867F9     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+1C9    008867FB     FF50 0C                 CALL DWORD PTR DS:[EAX+0xC]                       ; 得到stCD_MapData*
$+1CC    008867FE     8BC8                    MOV ECX,EAX
$+1CE    00886800     E8 FB73F5FF             CALL PathOfEx.007DDC00
$+1D3    00886805     C646 18 00              MOV BYTE PTR DS:[ESI+0x18],0x0                    ; stCD_EntityAttrTriggerableBlockage.is_blocked_
$+1D7    00886809     5E                      POP ESI
$+1D8    0088680A     83C4 0C                 ADD ESP,0xC
$+1DB    0088680D     C3                      RETN
$+1DC    0088680E     CC                      INT3
$+1DD    0088680F     CC                      INT3
$+1DE    00886810     55                      PUSH EBP
$+1DF    00886811     8BEC                    MOV EBP,ESP
$+1E1    00886813     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+1E7    00886819     6A FF                   PUSH -0x1
$+1E9    0088681B     68 2617D200             PUSH PathOfEx.00D21726
$+1EE    00886820     50                      PUSH EAX
$+1EF    00886821     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+1F6    00886828     56                      PUSH ESI
$+1F7    00886829     8BF1                    MOV ESI,ECX
$+1F9    0088682B     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+200    00886832     C706 2071EF00           MOV DWORD PTR DS:[ESI],PathOfEx.00EF7120          ; 构造TriggerableBlockage属性
$+206    00886838     C746 0C 5C71EF00        MOV DWORD PTR DS:[ESI+0xC],PathOfEx.00EF715C
$+20D    0088683F     C746 10 7071EF00        MOV DWORD PTR DS:[ESI+0x10],PathOfEx.00EF7170     ; ASCII "廓F"
$+214    00886846     E8 55FFFFFF             CALL PathOfEx.008867A0
$+219    0088684B     8B4E 20                 MOV ECX,DWORD PTR DS:[ESI+0x20]
$+21C    0088684E     85C9                    TEST ECX,ECX
$+21E    00886850     74 26                   JE SHORT PathOfEx.00886878
$+220    00886852     8B46 28                 MOV EAX,DWORD PTR DS:[ESI+0x28]
$+223    00886855     6A 01                   PUSH 0x1
$+225    00886857     2BC1                    SUB EAX,ECX
$+227    00886859     50                      PUSH EAX
$+228    0088685A     51                      PUSH ECX
$+229    0088685B     E8 D05FBEFF             CALL PathOfEx.0046C830
$+22E    00886860     C746 20 00000000        MOV DWORD PTR DS:[ESI+0x20],0x0
$+235    00886867     83C4 0C                 ADD ESP,0xC
$+238    0088686A     C746 24 00000000        MOV DWORD PTR DS:[ESI+0x24],0x0
$+23F    00886871     C746 28 00000000        MOV DWORD PTR DS:[ESI+0x28],0x0
$+246    00886878     F645 08 01              TEST BYTE PTR SS:[EBP+0x8],0x1
$+24A    0088687C     C706 7C32E700           MOV DWORD PTR DS:[ESI],PathOfEx.00E7327C
$+250    00886882     74 0B                   JE SHORT PathOfEx.0088688F
$+252    00886884     6A 3C                   PUSH 0x3C
$+254    00886886     56                      PUSH ESI
$+255    00886887     E8 7C5E3F00             CALL PathOfEx.00C7C708
$+25A    0088688C     83C4 08                 ADD ESP,0x8
$+25D    0088688F     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+260    00886892     8BC6                    MOV EAX,ESI
$+262    00886894     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+269    0088689B     5E                      POP ESI
$+26A    0088689C     8BE5                    MOV ESP,EBP
$+26C    0088689E     5D                      POP EBP
$+26D    0088689F     C2 0400                 RETN 0x4
$+270    008868A2     CC                      INT3
*/
/**/
//tag_复制目标区域的地图数据到vector中
/*
$-1      007DDA0F     CC                      INT3
$ ==>    007DDA10     55                      PUSH EBP                                          ; tag_复制目标区域的地图数据到vector中
$+1      007DDA11     8BEC                    MOV EBP,ESP
$+3      007DDA13     6A FF                   PUSH -0x1
$+5      007DDA15     68 C879D500             PUSH PathOfEx.00D579C8
$+A      007DDA1A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+10     007DDA20     50                      PUSH EAX
$+11     007DDA21     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     007DDA28     83EC 20                 SUB ESP,0x20
$+1B     007DDA2B     53                      PUSH EBX
$+1C     007DDA2C     8B5D 0C                 MOV EBX,DWORD PTR SS:[EBP+0xC]                    ; then,ebx=p2,=stRectInt*
$+1F     007DDA2F     894D F0                 MOV DWORD PTR SS:[EBP-0x10],ECX
$+22     007DDA32     56                      PUSH ESI
$+23     007DDA33     57                      PUSH EDI
$+24     007DDA34     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+26     007DDA36     99                      CDQ
$+27     007DDA37     2BC2                    SUB EAX,EDX
$+29     007DDA39     C745 E0 00000000        MOV DWORD PTR SS:[EBP-0x20],0x0
$+30     007DDA40     8BC8                    MOV ECX,EAX
$+32     007DDA42     C745 D4 00000000        MOV DWORD PTR SS:[EBP-0x2C],0x0
$+39     007DDA49     8B43 08                 MOV EAX,DWORD PTR DS:[EBX+0x8]
$+3C     007DDA4C     40                      INC EAX
$+3D     007DDA4D     D1F9                    SAR ECX,1
$+3F     007DDA4F     99                      CDQ
$+40     007DDA50     2BC2                    SUB EAX,EDX
$+42     007DDA52     894D E8                 MOV DWORD PTR SS:[EBP-0x18],ECX
$+45     007DDA55     D1F8                    SAR EAX,1
$+47     007DDA57     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+4A     007DDA5A     C745 D8 00000000        MOV DWORD PTR SS:[EBP-0x28],0x0
$+51     007DDA61     C745 DC 00000000        MOV DWORD PTR SS:[EBP-0x24],0x0
$+58     007DDA68     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$+5F     007DDA6F     2BC1                    SUB EAX,ECX
$+61     007DDA71     8B53 0C                 MOV EDX,DWORD PTR DS:[EBX+0xC]
$+64     007DDA74     2B53 04                 SUB EDX,DWORD PTR DS:[EBX+0x4]
$+67     007DDA77     0FAFD0                  IMUL EDX,EAX
$+6A     007DDA7A     03D2                    ADD EDX,EDX
$+6C     007DDA7C     74 09                   JE SHORT PathOfEx.007DDA87
$+6E     007DDA7E     52                      PUSH EDX
$+6F     007DDA7F     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+72     007DDA82     E8 49F6F1FF             CALL PathOfEx.006FD0D0                            ; 构造stCD_MapDataVector
$+77     007DDA87     8B55 F0                 MOV EDX,DWORD PTR SS:[EBP-0x10]
$+7A     007DDA8A     B9 02000000             MOV ECX,0x2
$+7F     007DDA8F     8B43 0C                 MOV EAX,DWORD PTR DS:[EBX+0xC]
$+82     007DDA92     894D 0C                 MOV DWORD PTR SS:[EBP+0xC],ECX
$+85     007DDA95     8D7A 3C                 LEA EDI,DWORD PTR DS:[EDX+0x3C]
$+88     007DDA98     8B73 04                 MOV ESI,DWORD PTR DS:[EBX+0x4]                    ; 外层循环开始
$+8B     007DDA9B     3BF0                    CMP ESI,EAX
$+8D     007DDA9D     74 33                   JE SHORT PathOfEx.007DDAD2
$+8F     007DDA9F     90                      NOP
$+90     007DDAA0     8B42 54                 MOV EAX,DWORD PTR DS:[EDX+0x54]                   ; 内层循环开始
$+93     007DDAA3     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$+96     007DDAA6     0FAFC6                  IMUL EAX,ESI
$+99     007DDAA9     C645 E4 00              MOV BYTE PTR SS:[EBP-0x1C],0x0
$+9D     007DDAAD     FF75 E4                 PUSH DWORD PTR SS:[EBP-0x1C]
$+A0     007DDAB0     0307                    ADD EAX,DWORD PTR DS:[EDI]
$+A2     007DDAB2     03C8                    ADD ECX,EAX
$+A4     007DDAB4     0345 E8                 ADD EAX,DWORD PTR SS:[EBP-0x18]
$+A7     007DDAB7     51                      PUSH ECX
$+A8     007DDAB8     50                      PUSH EAX
$+A9     007DDAB9     FF75 D8                 PUSH DWORD PTR SS:[EBP-0x28]
$+AC     007DDABC     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+AF     007DDABF     E8 4C1B0000             CALL PathOfEx.007DF610                            ; 把目标区域的地图数据copy到vector中
$+B4     007DDAC4     8B43 0C                 MOV EAX,DWORD PTR DS:[EBX+0xC]
$+B7     007DDAC7     46                      INC ESI
$+B8     007DDAC8     8B55 F0                 MOV EDX,DWORD PTR SS:[EBP-0x10]
$+BB     007DDACB     3BF0                    CMP ESI,EAX
$+BD     007DDACD   ^ 75 D1                   JNZ SHORT PathOfEx.007DDAA0                       ; 内层循环结束
$+BF     007DDACF     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
$+C2     007DDAD2     83C7 0C                 ADD EDI,0xC
$+C5     007DDAD5     83E9 01                 SUB ECX,0x1
$+C8     007DDAD8     894D 0C                 MOV DWORD PTR SS:[EBP+0xC],ECX
$+CB     007DDADB   ^ 75 BB                   JNZ SHORT PathOfEx.007DDA98                       ; 外层循环结束
$+CD     007DDADD     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]                    ; then,eax=p1,=stCD_MapDataVector*
$+D0     007DDAE0     8B4D D4                 MOV ECX,DWORD PTR SS:[EBP-0x2C]
$+D3     007DDAE3     5F                      POP EDI
$+D4     007DDAE4     5E                      POP ESI
$+D5     007DDAE5     8908                    MOV DWORD PTR DS:[EAX],ECX
$+D7     007DDAE7     8B4D D8                 MOV ECX,DWORD PTR SS:[EBP-0x28]
$+DA     007DDAEA     8948 04                 MOV DWORD PTR DS:[EAX+0x4],ECX
$+DD     007DDAED     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
$+E0     007DDAF0     8948 08                 MOV DWORD PTR DS:[EAX+0x8],ECX
$+E3     007DDAF3     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+E6     007DDAF6     5B                      POP EBX
$+E7     007DDAF7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+EE     007DDAFE     8BE5                    MOV ESP,EBP
$+F0     007DDB00     5D                      POP EBP
$+F1     007DDB01     C2 0800                 RETN 0x8
$+F4     007DDB04     CC                      INT3
*/
/**/
//tag_切换地图中的房间会断到
/*
$-55     006DCD1B     E8 E0B20300              CALL PathOfEx.00718000
$-50     006DCD20     83C4 04                  ADD ESP,0x4
$-4D     006DCD23     8BC8                     MOV ECX,EAX
$-4B     006DCD25     E8 66AE0300              CALL PathOfEx.00717B90
$-46     006DCD2A     8D4E 04                  LEA ECX,DWORD PTR DS:[ESI+0x4]
$-43     006DCD2D     33C0                     XOR EAX,EAX
$-41     006DCD2F     8701                     XCHG DWORD PTR DS:[ECX],EAX
$-3F     006DCD31     8BC1                     MOV EAX,ECX
$-3D     006DCD33     BA 84010101              MOV EDX,PathOfEx.01010184
$-38     006DCD38     8702                     XCHG DWORD PTR DS:[EDX],EAX
$-36     006DCD3A     8708                     XCHG DWORD PTR DS:[EAX],ECX
$-34     006DCD3C     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$-2D     006DCD43     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
$-2A     006DCD46     85C0                     TEST EAX,EAX
$-28     006DCD48     74 11                    JE SHORT PathOfEx.006DCD5B
$-26     006DCD4A     50                       PUSH EAX
$-25     006DCD4B     51                       PUSH ECX
$-24     006DCD4C     E8 AFB20300              CALL PathOfEx.00718000
$-1F     006DCD51     83C4 04                  ADD ESP,0x4
$-1C     006DCD54     8BC8                     MOV ECX,EAX
$-1A     006DCD56     E8 35AE0300              CALL PathOfEx.00717B90
$-15     006DCD5B     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-12     006DCD5E     5F                       POP EDI
$-11     006DCD5F     5E                       POP ESI
$-10     006DCD60     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-9      006DCD67     5B                       POP EBX
$-8      006DCD68     8BE5                     MOV ESP,EBP
$-6      006DCD6A     5D                       POP EBP
$-5      006DCD6B     C2 0400                  RETN 0x4
$-2      006DCD6E     CC                       INT3
$-1      006DCD6F     CC                       INT3
$ ==>    006DCD70     55                       PUSH EBP                                 ; tag_切换地图中的房间会断到,包括设置游戏环境
$+1      006DCD71     8BEC                     MOV EBP,ESP                              ; ecx=stCD_World*
$+3      006DCD73     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+9      006DCD79     6A FF                    PUSH -0x1
$+B      006DCD7B     F3:0F1045 14             MOVSS XMM0,DWORD PTR SS:[EBP+0x14]
$+10     006DCD80     68 5452D400              PUSH PathOfEx.00D45254                   ; ASCII "讣崂"
$+15     006DCD85     50                       PUSH EAX
$+16     006DCD86     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+1D     006DCD8D     83EC 08                  SUB ESP,0x8
$+20     006DCD90     53                       PUSH EBX
$+21     006DCD91     8B5D 18                  MOV EBX,DWORD PTR SS:[EBP+0x18]
$+24     006DCD94     56                       PUSH ESI
$+25     006DCD95     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
$+28     006DCD98     57                       PUSH EDI
$+29     006DCD99     FF75 1C                  PUSH DWORD PTR SS:[EBP+0x1C]
$+2C     006DCD9C     8BF9                     MOV EDI,ECX
$+2E     006DCD9E     53                       PUSH EBX
$+2F     006DCD9F     51                       PUSH ECX
$+30     006DCDA0     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
$+35     006DCDA5     FF75 10                  PUSH DWORD PTR SS:[EBP+0x10]
$+38     006DCDA8     F3:0F1045 0C             MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
$+3D     006DCDAD     51                       PUSH ECX
$+3E     006DCDAE     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
$+43     006DCDB3     56                       PUSH ESI
$+44     006DCDB4     E8 37161E00              CALL PathOfEx.008BE3F0
$+49     006DCDB9     8B87 C4010000            MOV EAX,DWORD PTR DS:[EDI+0x1C4]
$+4F     006DCDBF     85C0                     TEST EAX,EAX
$+51     006DCDC1     0F84 44010000            JE PathOfEx.006DCF0B
$+57     006DCDC7     3B70 14                  CMP ESI,DWORD PTR DS:[EAX+0x14]
$+5A     006DCDCA     0F85 3B010000            JNZ PathOfEx.006DCF0B
$+60     006DCDD0     B8 FFFF0000              MOV EAX,0xFFFF
$+65     006DCDD5     66:3BD8                  CMP BX,AX
$+68     006DCDD8     0F84 FB000000            JE PathOfEx.006DCED9
$+6E     006DCDDE     83EC 10                  SUB ESP,0x10
$+71     006DCDE1     E8 1AB20300              CALL PathOfEx.00718000
$+76     006DCDE6     8BC8                     MOV ECX,EAX
$+78     006DCDE8     E8 5368DEFF              CALL PathOfEx.004C3640
$+7D     006DCDED     8BF0                     MOV ESI,EAX
$+7F     006DCDEF     8975 18                  MOV DWORD PTR SS:[EBP+0x18],ESI
$+82     006DCDF2     0FB7C3                   MOVZX EAX,BX
$+85     006DCDF5     8D55 18                  LEA EDX,DWORD PTR SS:[EBP+0x18]
$+88     006DCDF8     50                       PUSH EAX
$+89     006DCDF9     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
$+8C     006DCDFC     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+93     006DCE03     E8 18760700              CALL PathOfEx.00754420                   ; 查找stCD_TblItemEnvironmentsPair
$+98     006DCE08     83C4 04                  ADD ESP,0x4
$+9B     006DCE0B     8D8F 30040000            LEA ECX,DWORD PTR DS:[EDI+0x430]         ; stCD_World.tmp_env_
$+A1     006DCE11     50                       PUSH EAX
$+A2     006DCE12     E8 692FD9FF              CALL PathOfEx.0046FD80
$+A7     006DCE17     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+AB     006DCE1B     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
$+AE     006DCE1E     85C0                     TEST EAX,EAX
$+B0     006DCE20     74 14                    JE SHORT PathOfEx.006DCE36
$+B2     006DCE22     50                       PUSH EAX
$+B3     006DCE23     E8 D8B10300              CALL PathOfEx.00718000
$+B8     006DCE28     8BC8                     MOV ECX,EAX
$+BA     006DCE2A     E8 61AD0300              CALL PathOfEx.00717B90
$+BF     006DCE2F     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
$+C6     006DCE36     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$+CD     006DCE3D     85F6                     TEST ESI,ESI
$+CF     006DCE3F     74 0D                    JE SHORT PathOfEx.006DCE4E
$+D1     006DCE41     56                       PUSH ESI
$+D2     006DCE42     E8 B9B10300              CALL PathOfEx.00718000
$+D7     006DCE47     8BC8                     MOV ECX,EAX
$+D9     006DCE49     E8 42AD0300              CALL PathOfEx.00717B90
$+DE     006DCE4E     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+E5     006DCE55     8BB7 34040000            MOV ESI,DWORD PTR DS:[EDI+0x434]
$+EB     006DCE5B     85F6                     TEST ESI,ESI
$+ED     006DCE5D     74 7A                    JE SHORT PathOfEx.006DCED9
$+EF     006DCE5F     8B76 04                  MOV ESI,DWORD PTR DS:[ESI+0x4]
$+F2     006DCE62     8BCF                     MOV ECX,EDI
$+F4     006DCE64     E8 77010000              CALL PathOfEx.006DCFE0
$+F9     006DCE69     84C0                     TEST AL,AL
$+FB     006DCE6B     74 10                    JE SHORT PathOfEx.006DCE7D
$+FD     006DCE6D     8B87 34040000            MOV EAX,DWORD PTR DS:[EDI+0x434]
$+103    006DCE73     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
$+106    006DCE76     66:8338 00               CMP WORD PTR DS:[EAX],0x0
$+10A    006DCE7A     0F45F0                   CMOVNE ESI,EAX
$+10D    006DCE7D     8B9F 3C040000            MOV EBX,DWORD PTR DS:[EDI+0x43C]
$+113    006DCE83     85DB                     TEST EBX,EBX
$+115    006DCE85     74 20                    JE SHORT PathOfEx.006DCEA7
$+117    006DCE87     56                       PUSH ESI
$+118    006DCE88     8BCB                     MOV ECX,EBX
$+11A    006DCE8A     E8 4141D9FF              CALL PathOfEx.00470FD0
$+11F    006DCE8F     85C0                     TEST EAX,EAX
$+121    006DCE91     74 14                    JE SHORT PathOfEx.006DCEA7
$+123    006DCE93     F3:0F104D 0C             MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
$+128    006DCE98     8BCB                     MOV ECX,EBX
$+12A    006DCE9A     F3:0F590D C8EDF300       MULSS XMM1,DWORD PTR DS:[0xF3EDC8]
$+132    006DCEA2     E8 89682400              CALL PathOfEx.00923730
$+137    006DCEA7     8BB7 40040000            MOV ESI,DWORD PTR DS:[EDI+0x440]
$+13D    006DCEAD     85F6                     TEST ESI,ESI
$+13F    006DCEAF     74 28                    JE SHORT PathOfEx.006DCED9
$+141    006DCEB1     8B87 34040000            MOV EAX,DWORD PTR DS:[EDI+0x434]
$+147    006DCEB7     8BCE                     MOV ECX,ESI
$+149    006DCEB9     FF70 24                  PUSH DWORD PTR DS:[EAX+0x24]
$+14C    006DCEBC     E8 0F41D9FF              CALL PathOfEx.00470FD0
$+151    006DCEC1     85C0                     TEST EAX,EAX
$+153    006DCEC3     74 14                    JE SHORT PathOfEx.006DCED9
$+155    006DCEC5     F3:0F104D 0C             MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
$+15A    006DCECA     8BCE                     MOV ECX,ESI
$+15C    006DCECC     F3:0F590D C8EDF300       MULSS XMM1,DWORD PTR DS:[0xF3EDC8]
$+164    006DCED4     E8 57682400              CALL PathOfEx.00923730
$+169    006DCED9     807D 1C 00               CMP BYTE PTR SS:[EBP+0x1C],0x0
$+16D    006DCEDD     74 2C                    JE SHORT PathOfEx.006DCF0B
$+16F    006DCEDF     8B87 38040000            MOV EAX,DWORD PTR DS:[EDI+0x438]
$+175    006DCEE5     F3:0F1045 0C             MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
$+17A    006DCEEA     F3:0F5905 C8EDF300       MULSS XMM0,DWORD PTR DS:[0xF3EDC8]
$+182    006DCEF2     C680 D0030000 00         MOV BYTE PTR DS:[EAX+0x3D0],0x0
$+189    006DCEF9     C780 C8030000 00000000   MOV DWORD PTR DS:[EAX+0x3C8],0x0
$+193    006DCF03     F3:0F1180 C4030000       MOVSS DWORD PTR DS:[EAX+0x3C4],XMM0
$+19B    006DCF0B     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+19E    006DCF0E     5F                       POP EDI
$+19F    006DCF0F     5E                       POP ESI
$+1A0    006DCF10     5B                       POP EBX
$+1A1    006DCF11     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+1A8    006DCF18     8BE5                     MOV ESP,EBP
$+1AA    006DCF1A     5D                       POP EBP
$+1AB    006DCF1B     C2 1800                  RETN 0x18
$+1AE    006DCF1E     CC                       INT3
$+1AF    006DCF1F     CC                       INT3
$+1B0    006DCF20     55                       PUSH EBP
$+1B1    006DCF21     8BEC                     MOV EBP,ESP
$+1B3    006DCF23     83E4 F8                  AND ESP,0xFFFFFFF8
$+1B6    006DCF26     83EC 08                  SUB ESP,0x8
$+1B9    006DCF29     53                       PUSH EBX
$+1BA    006DCF2A     8BD9                     MOV EBX,ECX
$+1BC    006DCF2C     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
$+1BF    006DCF2F     57                       PUSH EDI
$+1C0    006DCF30     8B93 C4010000            MOV EDX,DWORD PTR DS:[EBX+0x1C4]
$+1C6    006DCF36     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+1C8    006DCF38     3B42 14                  CMP EAX,DWORD PTR DS:[EDX+0x14]
$+1CB    006DCF3B     0F85 8C000000            JNZ PathOfEx.006DCFCD
$+1D1    006DCF41     83BB 34040000 00         CMP DWORD PTR DS:[EBX+0x434],0x0
$+1D8    006DCF48     74 51                    JE SHORT PathOfEx.006DCF9B
$+1DA    006DCF4A     8BCB                     MOV ECX,EBX
$+1DC    006DCF4C     E8 8F000000              CALL PathOfEx.006DCFE0
$+1E1    006DCF51     83EC 08                  SUB ESP,0x8
$+1E4    006DCF54     884424 14                MOV BYTE PTR SS:[ESP+0x14],AL
$+1E8    006DCF58     8BCB                     MOV ECX,EBX
$+1EA    006DCF5A     8DBB 30040000            LEA EDI,DWORD PTR DS:[EBX+0x430]
$+1F0    006DCF60     C74424 04 00000000       MOV DWORD PTR SS:[ESP+0x4],0x0
$+1F8    006DCF68     E8 E3000000              CALL PathOfEx.006DD050
$+1FD    006DCF6D     83C4 04                  ADD ESP,0x4
$+200    006DCF70     0FB6C0                   MOVZX EAX,AL
$+203    006DCF73     50                       PUSH EAX
$+204    006DCF74     FF7424 14                PUSH DWORD PTR SS:[ESP+0x14]
$+208    006DCF78     57                       PUSH EDI
$+209    006DCF79     E8 02952200              CALL PathOfEx.00906480
$+20E    006DCF7E     8BC8                     MOV ECX,EAX
$+210    006DCF80     E8 9B952200              CALL PathOfEx.00906520
$+215    006DCF85     FF7424 0C                PUSH DWORD PTR SS:[ESP+0xC]
$+219    006DCF89     8BCB                     MOV ECX,EBX
$+21B    006DCF8B     57                       PUSH EDI
$+21C    006DCF8C     E8 AFF1FFFF              CALL PathOfEx.006DC140
$+221    006DCF91     8BCF                     MOV ECX,EDI
$+223    006DCF93     E8 B865E9FF              CALL PathOfEx.00573550
$+228    006DCF98     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
$+22B    006DCF9B     8079 1A 00               CMP BYTE PTR DS:[ECX+0x1A],0x0
$+22F    006DCF9F     74 2C                    JE SHORT PathOfEx.006DCFCD
$+231    006DCFA1     F3:0F1041 0C             MOVSS XMM0,DWORD PTR DS:[ECX+0xC]
$+236    006DCFA6     8B83 38040000            MOV EAX,DWORD PTR DS:[EBX+0x438]
$+23C    006DCFAC     F3:0F5905 C8EDF300       MULSS XMM0,DWORD PTR DS:[0xF3EDC8]
$+244    006DCFB4     C680 D0030000 01         MOV BYTE PTR DS:[EAX+0x3D0],0x1
$+24B    006DCFBB     F3:0F1180 C4030000       MOVSS DWORD PTR DS:[EAX+0x3C4],XMM0
$+253    006DCFC3     C780 C8030000 00000000   MOV DWORD PTR DS:[EAX+0x3C8],0x0
$+25D    006DCFCD     5F                       POP EDI
$+25E    006DCFCE     5B                       POP EBX
$+25F    006DCFCF     8BE5                     MOV ESP,EBP
$+261    006DCFD1     5D                       POP EBP
$+262    006DCFD2     C2 0400                  RETN 0x4
$+265    006DCFD5     CC                       INT3
*/
/**/
//tag_通过Meata地形数据结构查找TblItemEnvironmentsPair
/*
$-71     0052D9FF          CC                  INT3
$-70     0052DA00          55                  PUSH EBP
$-6F     0052DA01          8BEC                MOV EBP,ESP
$-6D     0052DA03          64:A1 00000000      MOV EAX,DWORD PTR FS:[0]
$-67     0052DA09          6A FF               PUSH -0x1
$-65     0052DA0B          68 2617D200         PUSH PathOfEx.00D21726
$-60     0052DA10          50                  PUSH EAX
$-5F     0052DA11          64:8925 00000000    MOV DWORD PTR FS:[0],ESP
$-58     0052DA18          56                  PUSH ESI
$-57     0052DA19          8BF1                MOV ESI,ECX
$-55     0052DA1B          C745 FC 00000000    MOV DWORD PTR SS:[EBP-0x4],0x0
$-4E     0052DA22          C706 D010E800       MOV DWORD PTR DS:[ESI],PathOfEx.00E810D0
$-48     0052DA28          E8 23BB1E00         CALL PathOfEx.00719550
$-43     0052DA2D          84C0                TEST AL,AL
$-41     0052DA2F          74 0C               JE SHORT PathOfEx.0052DA3D
$-3F     0052DA31          8B4E 30             MOV ECX,DWORD PTR DS:[ESI+0x30]
$-3C     0052DA34          85C9                TEST ECX,ECX
$-3A     0052DA36          74 05               JE SHORT PathOfEx.0052DA3D
$-38     0052DA38          E8 138B2300         CALL PathOfEx.00766550
$-33     0052DA3D          8BCE                MOV ECX,ESI
$-31     0052DA3F          E8 ACB51E00         CALL PathOfEx.00718FF0
$-2C     0052DA44          F645 08 01          TEST BYTE PTR SS:[EBP+0x8],0x1
$-28     0052DA48          74 0B               JE SHORT PathOfEx.0052DA55
$-26     0052DA4A          6A 60               PUSH 0x60
$-24     0052DA4C          56                  PUSH ESI
$-23     0052DA4D          E8 B6EC7400         CALL PathOfEx.00C7C708
$-1E     0052DA52          83C4 08             ADD ESP,0x8
$-1B     0052DA55          8B4D F4             MOV ECX,DWORD PTR SS:[EBP-0xC]
$-18     0052DA58          8BC6                MOV EAX,ESI
$-16     0052DA5A          64:890D 00000000    MOV DWORD PTR FS:[0],ECX
$-F      0052DA61          5E                  POP ESI
$-E      0052DA62          8BE5                MOV ESP,EBP
$-C      0052DA64          5D                  POP EBP
$-B      0052DA65          C2 0400             RETN 0x4
$-8      0052DA68          CC                  INT3
$-7      0052DA69          CC                  INT3
$-6      0052DA6A          CC                  INT3
$-5      0052DA6B          CC                  INT3
$-4      0052DA6C          CC                  INT3
$-3      0052DA6D          CC                  INT3
$-2      0052DA6E          CC                  INT3
$-1      0052DA6F          CC                  INT3
$ ==>    0052DA70          8B4424 04           MOV EAX,DWORD PTR SS:[ESP+0x4]                 ; tag_通过Meata地形数据结构查找TblItemEnvironmentsPair
$+4      0052DA74          56                  PUSH ESI
$+5      0052DA75          57                  PUSH EDI
$+6      0052DA76          8BF9                MOV EDI,ECX
$+8      0052DA78          8B77 14             MOV ESI,DWORD PTR DS:[EDI+0x14]
$+B      0052DA7B          8930                MOV DWORD PTR DS:[EAX],ESI
$+D      0052DA7D          85F6                TEST ESI,ESI
$+F      0052DA7F          74 15               JE SHORT PathOfEx.0052DA96
$+11     0052DA81          8D56 38             LEA EDX,DWORD PTR DS:[ESI+0x38]
$+14     0052DA84          B9 01000000         MOV ECX,0x1
$+19     0052DA89          F0:0FC10A           LOCK XADD DWORD PTR DS:[EDX],ECX
$+1D     0052DA8D          8B0D 0C67FD00       MOV ECX,DWORD PTR DS:[0xFD670C]
$+23     0052DA93          894E 34             MOV DWORD PTR DS:[ESI+0x34],ECX
$+26     0052DA96          8B4F 18             MOV ECX,DWORD PTR DS:[EDI+0x18]
$+29     0052DA99          5F                  POP EDI
$+2A     0052DA9A          8948 04             MOV DWORD PTR DS:[EAX+0x4],ECX		;hook此处，g_h_modify_game_env_id_at_change_map_
$+2D     0052DA9D          5E                  POP ESI
$+2E     0052DA9E          C2 0400             RETN 0x4
$+31     0052DAA1          CC                  INT3
$+32     0052DAA2          CC                  INT3
$+33     0052DAA3          CC                  INT3
$+34     0052DAA4          CC                  INT3
$+35     0052DAA5          CC                  INT3
$+36     0052DAA6          CC                  INT3
$+37     0052DAA7          CC                  INT3
$+38     0052DAA8          CC                  INT3
$+39     0052DAA9          CC                  INT3
$+3A     0052DAAA          CC                  INT3
$+3B     0052DAAB          CC                  INT3
$+3C     0052DAAC          CC                  INT3
$+3D     0052DAAD          CC                  INT3
$+3E     0052DAAE          CC                  INT3
$+3F     0052DAAF          CC                  INT3
$+40     0052DAB0          8B4424 04           MOV EAX,DWORD PTR SS:[ESP+0x4]
$+44     0052DAB4          56                  PUSH ESI
$+45     0052DAB5          57                  PUSH EDI
$+46     0052DAB6          8BF9                MOV EDI,ECX
$+48     0052DAB8          8B77 37             MOV ESI,DWORD PTR DS:[EDI+0x37]
$+4B     0052DABB          8930                MOV DWORD PTR DS:[EAX],ESI
$+4D     0052DABD          85F6                TEST ESI,ESI
$+4F     0052DABF          74 15               JE SHORT PathOfEx.0052DAD6
$+51     0052DAC1          8D56 38             LEA EDX,DWORD PTR DS:[ESI+0x38]
$+54     0052DAC4          B9 01000000         MOV ECX,0x1
$+59     0052DAC9          F0:0FC10A           LOCK XADD DWORD PTR DS:[EDX],ECX
$+5D     0052DACD          8B0D 0C67FD00       MOV ECX,DWORD PTR DS:[0xFD670C]
$+63     0052DAD3          894E 34             MOV DWORD PTR DS:[ESI+0x34],ECX
$+66     0052DAD6          8B4F 3B             MOV ECX,DWORD PTR DS:[EDI+0x3B]
$+69     0052DAD9          5F                  POP EDI
$+6A     0052DADA          8948 04             MOV DWORD PTR DS:[EAX+0x4],ECX
$+6D     0052DADD          5E                  POP ESI
$+6E     0052DADE          C2 0400             RETN 0x4
$+71     0052DAE1          CC                  INT3
$+72     0052DAE2          CC                  INT3
$+73     0052DAE3          CC                  INT3
$+74     0052DAE4          CC                  INT3
$+75     0052DAE5          CC                  INT3
$+76     0052DAE6          CC                  INT3
$+77     0052DAE7          CC                  INT3
$+78     0052DAE8          CC                  INT3
$+79     0052DAE9          CC                  INT3
$+7A     0052DAEA          CC                  INT3
$+7B     0052DAEB          CC                  INT3
$+7C     0052DAEC          CC                  INT3
$+7D     0052DAED          CC                  INT3
$+7E     0052DAEE          CC                  INT3
$+7F     0052DAEF          CC                  INT3
$+80     0052DAF0          55                  PUSH EBP
$+81     0052DAF1          8BEC                MOV EBP,ESP
$+83     0052DAF3          83E4 F8             AND ESP,0xFFFFFFF8
$+86     0052DAF6          F3:0F1015 48F4F300  MOVSS XMM2,DWORD PTR DS:[0xF3F448]
$+8E     0052DAFE          0F28E0              MOVAPS XMM4,XMM0
$+91     0052DB01          F3:0F100D 28F4F300  MOVSS XMM1,DWORD PTR DS:[0xF3F428]
$+99     0052DB09          0F54D4              ANDPS XMM2,XMM4
$+9C     0052DB0C          0F57C2              XORPS XMM0,XMM2
$+9F     0052DB0F          F3:0F101D FCEDF300  MOVSS XMM3,DWORD PTR DS:[0xF3EDFC]
$+A7     0052DB17          F3:0FC2C1 01        CMPLTSS XMM0,XMM1
$+AC     0052DB1C          0F54C8              ANDPS XMM1,XMM0
$+AF     0052DB1F          0F28C4              MOVAPS XMM0,XMM4
$+B2     0052DB22          0F56CA              ORPS XMM1,XMM2
$+B5     0052DB25          F3:0F58C1           ADDSS XMM0,XMM1
$+B9     0052DB29          F3:0F5CC1           SUBSS XMM0,XMM1
$+BD     0052DB2D          0F28C8              MOVAPS XMM1,XMM0
$+C0     0052DB30          F3:0F5CCC           SUBSS XMM1,XMM4
$+C4     0052DB34          F3:0FC2CA 06        CMPNLESS XMM1,XMM2
$+C9     0052DB39          0F54CB              ANDPS XMM1,XMM3
$+CC     0052DB3C          F3:0F5CC1           SUBSS XMM0,XMM1
$+D0     0052DB40          8BE5                MOV ESP,EBP
$+D2     0052DB42          5D                  POP EBP
$+D3     0052DB43          C3                  RETN
$+D4     0052DB44          CC                  INT3
*/
/**/