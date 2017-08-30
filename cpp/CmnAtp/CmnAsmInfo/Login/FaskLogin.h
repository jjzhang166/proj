
//下面的汇编所属module->TCLS
/**/
//////////////////////////////////////////////////////////////////////////
/**/
//tag_添加所有快速登陆项帐号信息，并格式化

//c7??????????????????eb??8b??????????83????89??????????8b??????????3b????0f??????????66
/*
$-1      03EB6B1F        CC                         INT3
$ ==>    03EB6B20        55                         PUSH EBP                                          ; tag_添加所有快速登陆项帐号信息
$+1      03EB6B21        8BEC                       MOV EBP,ESP
$+3      03EB6B23        6A FF                      PUSH -0x1
$+5      03EB6B25        68 C9910004                PUSH TCLS.040091C9
$+A      03EB6B2A        64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+10     03EB6B30        50                         PUSH EAX
$+11     03EB6B31        64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+18     03EB6B38        B8 B4160000                MOV EAX,0x16B4
$+1D     03EB6B3D        E8 FE3A1100                CALL TCLS.03FCA640
$+22     03EB6B42        53                         PUSH EBX
$+23     03EB6B43        56                         PUSH ESI
$+24     03EB6B44        57                         PUSH EDI
$+25     03EB6B45        894D F0                    MOV DWORD PTR SS:[EBP-0x10],ECX
$+28     03EB6B48        8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+2B     03EB6B4B        8378 9C 01                 CMP DWORD PTR DS:[EAX-0x64],0x1
$+2F     03EB6B4F        74 06                      JE SHORT TCLS.03EB6B57
$+31     03EB6B51        FF15 609C1B04              CALL DWORD PTR DS:[<&KERNEL32.DebugBreak>]        ; kernel32.DebugBreak
$+37     03EB6B57        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+3A     03EB6B5A        81E9 88000000              SUB ECX,0x88
$+40     03EB6B60        51                         PUSH ECX
$+41     03EB6B61        8B55 F0                    MOV EDX,DWORD PTR SS:[EBP-0x10]
$+44     03EB6B64        83EA 64                    SUB EDX,0x64
$+47     03EB6B67        52                         PUSH EDX
$+48     03EB6B68        8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+4B     03EB6B6B        83E8 60                    SUB EAX,0x60
$+4E     03EB6B6E        50                         PUSH EAX
$+4F     03EB6B6F        8D4D E0                    LEA ECX,DWORD PTR SS:[EBP-0x20]
$+52     03EB6B72        E8 15F0F2FF                CALL TCLS.03DE5B8C
$+57     03EB6B77        C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+5E     03EB6B7E        C745 D8 00000000           MOV DWORD PTR SS:[EBP-0x28],0x0
$+65     03EB6B85        837D 0C 00                 CMP DWORD PTR SS:[EBP+0xC],0x0
$+69     03EB6B89        7C 12                      JL SHORT TCLS.03EB6B9D
$+6B     03EB6B8B        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+6E     03EB6B8E        83B9 34130000 00           CMP DWORD PTR DS:[ECX+0x1334],0x0
$+75     03EB6B95        74 06                      JE SHORT TCLS.03EB6B9D
$+77     03EB6B97        837D 10 00                 CMP DWORD PTR SS:[EBP+0x10],0x0
$+7B     03EB6B9B        75 2C                      JNZ SHORT TCLS.03EB6BC9
$+7D     03EB6B9D        8B55 0C                    MOV EDX,DWORD PTR SS:[EBP+0xC]
$+80     03EB6BA0        52                         PUSH EDX
$+81     03EB6BA1        68 4CF11504                PUSH TCLS.0415F14C                                ; [login][CTASLogin::OnUpdatePVAList]nCount = %d,return
$+86     03EB6BA6        8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+89     03EB6BA9        83C0 0D                    ADD EAX,0xD
$+8C     03EB6BAC        50                         PUSH EAX
$+8D     03EB6BAD        E8 1AF6F2FF                CALL TCLS.03DE61CC
$+92     03EB6BB2        83C4 0C                    ADD ESP,0xC
$+95     03EB6BB5        C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
$+9C     03EB6BBC        8D4D E0                    LEA ECX,DWORD PTR SS:[EBP-0x20]
$+9F     03EB6BBF        E8 44CEF2FF                CALL TCLS.03DE3A08
$+A4     03EB6BC4        E9 8B020000                JMP TCLS.03EB6E54
$+A9     03EB6BC9        837D 0C 14                 CMP DWORD PTR SS:[EBP+0xC],0x14
$+AD     03EB6BCD        7E 1F                      JLE SHORT TCLS.03EB6BEE
$+AF     03EB6BCF        8B4D 0C                    MOV ECX,DWORD PTR SS:[EBP+0xC]
$+B2     03EB6BD2        51                         PUSH ECX
$+B3     03EB6BD3        68 90F01504                PUSH TCLS.0415F090                                ; [login][CTASLogin::OnUpdatePVAList]nCount = %d, nCount = MAXNUM_HISTORYCOUNT;
$+B8     03EB6BD8        8B55 F0                    MOV EDX,DWORD PTR SS:[EBP-0x10]
$+BB     03EB6BDB        83C2 0D                    ADD EDX,0xD
$+BE     03EB6BDE        52                         PUSH EDX
$+BF     03EB6BDF        E8 E8F5F2FF                CALL TCLS.03DE61CC
$+C4     03EB6BE4        83C4 0C                    ADD ESP,0xC
$+C7     03EB6BE7        C745 0C 14000000           MOV DWORD PTR SS:[EBP+0xC],0x14
$+CE     03EB6BEE        8B45 0C                    MOV EAX,DWORD PTR SS:[EBP+0xC]
$+D1     03EB6BF1        50                         PUSH EAX
$+D2     03EB6BF2        68 20F01504                PUSH TCLS.0415F020                                ; [login][CTASLogin::OnUpdatePVAList]nCount = %d
$+D7     03EB6BF7        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+DA     03EB6BFA        83C1 0D                    ADD ECX,0xD
$+DD     03EB6BFD        51                         PUSH ECX
$+DE     03EB6BFE        E8 91FBF2FF                CALL TCLS.03DE6794
$+E3     03EB6C03        83C4 0C                    ADD ESP,0xC
$+E6     03EB6C06        C785 94EBFFFF 00000000     MOV DWORD PTR SS:[EBP-0x146C],0x0
$+F0     03EB6C10        B9 0D050000                MOV ECX,0x50D
$+F5     03EB6C15        33C0                       XOR EAX,EAX
$+F7     03EB6C17        8DBD 98EBFFFF              LEA EDI,DWORD PTR SS:[EBP-0x1468]
$+FD     03EB6C1D        F3:AB                      REP STOS DWORD PTR ES:[EDI]
$+FF     03EB6C1F        66:AB                      STOS WORD PTR ES:[EDI]
$+101    03EB6C21        C785 94EBFFFF 25020010     MOV DWORD PTR SS:[EBP-0x146C],0x10000225
$+10B    03EB6C2B        6A 32                      PUSH 0x32
$+10D    03EB6C2D        68 10F01504                PUSH TCLS.0415F010                                ; QUICKLOGINQQ
$+112    03EB6C32        8D95 98EBFFFF              LEA EDX,DWORD PTR SS:[EBP-0x1468]
$+118    03EB6C38        52                         PUSH EDX
$+119    03EB6C39        FF15 AC9E1B04              CALL DWORD PTR DS:[<&MSVCRT.strncpy>]             ; msvcrt.strncpy
$+11F    03EB6C3F        83C4 0C                    ADD ESP,0xC
$+122    03EB6C42        8B45 0C                    MOV EAX,DWORD PTR SS:[EBP+0xC]
$+125    03EB6C45        8985 CAEBFFFF              MOV DWORD PTR SS:[EBP-0x1436],EAX
$+12B    03EB6C4B        8B4D 10                    MOV ECX,DWORD PTR SS:[EBP+0x10]
$+12E    03EB6C4E        898D 90EBFFFF              MOV DWORD PTR SS:[EBP-0x1470],ECX
$+134    03EB6C54        33D2                       XOR EDX,EDX
$+136    03EB6C56        85D2                       TEST EDX,EDX
$+138    03EB6C58        74 05                      JE SHORT TCLS.03EB6C5F
$+13A    03EB6C5A        E9 2D010000                JMP TCLS.03EB6D8C
$+13F    03EB6C5F        C785 8CEBFFFF 00000000     MOV DWORD PTR SS:[EBP-0x1474],0x0                 ; 清空数量，下面遍历每个QQ项，最后的结果便是当前QQ的总数量
$+149    03EB6C69        EB 0F                      JMP SHORT TCLS.03EB6C7A
$+14B    03EB6C6B        8B85 8CEBFFFF              MOV EAX,DWORD PTR SS:[EBP-0x1474]
$+151    03EB6C71        83C0 01                    ADD EAX,0x1
$+154    03EB6C74        8985 8CEBFFFF              MOV DWORD PTR SS:[EBP-0x1474],EAX
$+15A    03EB6C7A        8B8D 8CEBFFFF              MOV ECX,DWORD PTR SS:[EBP-0x1474]
$+160    03EB6C80        3B4D 0C                    CMP ECX,DWORD PTR SS:[EBP+0xC]
$+163    03EB6C83        0F8D 03010000              JGE TCLS.03EB6D8C
$+169    03EB6C89        66:C785 84E9FFFF 0000      MOV WORD PTR SS:[EBP-0x167C],0x0
$+172    03EB6C92        B9 81000000                MOV ECX,0x81
$+177    03EB6C97        33C0                       XOR EAX,EAX
$+179    03EB6C99        8DBD 86E9FFFF              LEA EDI,DWORD PTR SS:[EBP-0x167A]
$+17F    03EB6C9F        F3:AB                      REP STOS DWORD PTR ES:[EDI]
$+181    03EB6CA1        66:AB                      STOS WORD PTR ES:[EDI]
$+183    03EB6CA3        8D95 84E9FFFF              LEA EDX,DWORD PTR SS:[EBP-0x167C]
$+189    03EB6CA9        52                         PUSH EDX
$+18A    03EB6CAA        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+18D    03EB6CAD        83C1 0C                    ADD ECX,0xC
$+190    03EB6CB0        E8 F5DAF2FF                CALL TCLS.03DE47AA
$+195    03EB6CB5        50                         PUSH EAX
$+196    03EB6CB6        8D85 84E9FFFF              LEA EAX,DWORD PTR SS:[EBP-0x167C]
$+19C    03EB6CBC        50                         PUSH EAX
$+19D    03EB6CBD        68 840C1504                PUSH TCLS.04150C84                                ; #amp;
$+1A2    03EB6CC2        68 800C1504                PUSH TCLS.04150C80                                ; &
$+1A7    03EB6CC7        8B4D 10                    MOV ECX,DWORD PTR SS:[EBP+0x10]
$+1AA    03EB6CCA        83C1 40                    ADD ECX,0x40
$+1AD    03EB6CCD        51                         PUSH ECX
$+1AE    03EB6CCE        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+1B1    03EB6CD1        83C1 0C                    ADD ECX,0xC
$+1B4    03EB6CD4        E8 9FB2F2FF                CALL TCLS.03DE1F78
$+1B9    03EB6CD9        8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+1BC    03EB6CDC        83C2 40                    ADD EDX,0x40
$+1BF    03EB6CDF        52                         PUSH EDX
$+1C0    03EB6CE0        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+1C3    03EB6CE3        83C1 0C                    ADD ECX,0xC
$+1C6    03EB6CE6        E8 F7C5F2FF                CALL TCLS.03DE32E2
$+1CB    03EB6CEB        50                         PUSH EAX
$+1CC    03EB6CEC        8B45 10                    MOV EAX,DWORD PTR SS:[EBP+0x10]
$+1CF    03EB6CEF        83C0 40                    ADD EAX,0x40
$+1D2    03EB6CF2        50                         PUSH EAX
$+1D3    03EB6CF3        68 700C1504                PUSH TCLS.04150C70                                ; &amp;
$+1D8    03EB6CF8        68 840C1504                PUSH TCLS.04150C84                                ; #amp;
$+1DD    03EB6CFD        8D8D 84E9FFFF              LEA ECX,DWORD PTR SS:[EBP-0x167C]
$+1E3    03EB6D03        51                         PUSH ECX
$+1E4    03EB6D04        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+1E7    03EB6D07        83C1 0C                    ADD ECX,0xC
$+1EA    03EB6D0A        E8 69B2F2FF                CALL TCLS.03DE1F78
$+1EF    03EB6D0F        8D95 84E9FFFF              LEA EDX,DWORD PTR SS:[EBP-0x167C]
$+1F5    03EB6D15        52                         PUSH EDX
$+1F6    03EB6D16        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+1F9    03EB6D19        83C1 0C                    ADD ECX,0xC
$+1FC    03EB6D1C        E8 89DAF2FF                CALL TCLS.03DE47AA
$+201    03EB6D21        50                         PUSH EAX
$+202    03EB6D22        8D85 84E9FFFF              LEA EAX,DWORD PTR SS:[EBP-0x167C]
$+208    03EB6D28        50                         PUSH EAX
$+209    03EB6D29        68 640C1504                PUSH TCLS.04150C64                                ; &lt;
$+20E    03EB6D2E        68 600C1504                PUSH TCLS.04150C60                                ; <
$+213    03EB6D33        8B4D 10                    MOV ECX,DWORD PTR SS:[EBP+0x10]
$+216    03EB6D36        83C1 40                    ADD ECX,0x40
$+219    03EB6D39        51                         PUSH ECX
$+21A    03EB6D3A        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+21D    03EB6D3D        83C1 0C                    ADD ECX,0xC
$+220    03EB6D40        E8 33B2F2FF                CALL TCLS.03DE1F78
$+225    03EB6D45        8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+228    03EB6D48        83C2 40                    ADD EDX,0x40
$+22B    03EB6D4B        52                         PUSH EDX
$+22C    03EB6D4C        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+22F    03EB6D4F        83C1 0C                    ADD ECX,0xC
$+232    03EB6D52        E8 8BC5F2FF                CALL TCLS.03DE32E2
$+237    03EB6D57        50                         PUSH EAX
$+238    03EB6D58        8B45 10                    MOV EAX,DWORD PTR SS:[EBP+0x10]
$+23B    03EB6D5B        83C0 40                    ADD EAX,0x40
$+23E    03EB6D5E        50                         PUSH EAX
$+23F    03EB6D5F        68 540C1504                PUSH TCLS.04150C54                                ; &gt;
$+244    03EB6D64        68 500C1504                PUSH TCLS.04150C50                                ; >
$+249    03EB6D69        8D8D 84E9FFFF              LEA ECX,DWORD PTR SS:[EBP-0x167C]
$+24F    03EB6D6F        51                         PUSH ECX
$+250    03EB6D70        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+253    03EB6D73        83C1 0C                    ADD ECX,0xC
$+256    03EB6D76        E8 FDB1F2FF                CALL TCLS.03DE1F78
$+25B    03EB6D7B        8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+25E    03EB6D7E        81C2 A0000000              ADD EDX,0xA0
$+264    03EB6D84        8955 10                    MOV DWORD PTR SS:[EBP+0x10],EDX
$+267    03EB6D87      ^ E9 DFFEFFFF                JMP TCLS.03EB6C6B
$+26C    03EB6D8C        33C0                       XOR EAX,EAX
$+26E    03EB6D8E        85C0                       TEST EAX,EAX
$+270    03EB6D90        74 05                      JE SHORT TCLS.03EB6D97
$+272    03EB6D92        E9 8C000000                JMP TCLS.03EB6E23
$+277    03EB6D97        C785 80E9FFFF 00000000     MOV DWORD PTR SS:[EBP-0x1680],0x0                 ; 清空数量，下面遍历每个QQ项，最后的结果便是当前QQ的总数量
$+281    03EB6DA1        EB 0F                      JMP SHORT TCLS.03EB6DB2
$+283    03EB6DA3        8B8D 80E9FFFF              MOV ECX,DWORD PTR SS:[EBP-0x1680]
$+289    03EB6DA9        83C1 01                    ADD ECX,0x1
$+28C    03EB6DAC        898D 80E9FFFF              MOV DWORD PTR SS:[EBP-0x1680],ECX
$+292    03EB6DB2        8B95 80E9FFFF              MOV EDX,DWORD PTR SS:[EBP-0x1680]
$+298    03EB6DB8        3B55 0C                    CMP EDX,DWORD PTR SS:[EBP+0xC]
$+29B    03EB6DBB        7D 66                      JGE SHORT TCLS.03EB6E23
$+29D    03EB6DBD        68 80000000                PUSH 0x80
$+2A2    03EB6DC2        8B85 80E9FFFF              MOV EAX,DWORD PTR SS:[EBP-0x1680]
$+2A8    03EB6DC8        C1E0 08                    SHL EAX,0x8
$+2AB    03EB6DCB        8D8C05 CEEBFFFF            LEA ECX,DWORD PTR SS:[EBP+EAX-0x1432]
$+2B2    03EB6DD2        51                         PUSH ECX
$+2B3    03EB6DD3        8B95 90EBFFFF              MOV EDX,DWORD PTR SS:[EBP-0x1470]
$+2B9    03EB6DD9        52                         PUSH EDX
$+2BA    03EB6DDA        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+2BD    03EB6DDD        83C1 0C                    ADD ECX,0xC
$+2C0    03EB6DE0        E8 A0D1F2FF                CALL TCLS.03DE3F85
$+2C5    03EB6DE5        68 80000000                PUSH 0x80
$+2CA    03EB6DEA        8B85 80E9FFFF              MOV EAX,DWORD PTR SS:[EBP-0x1680]
$+2D0    03EB6DF0        C1E0 08                    SHL EAX,0x8
$+2D3    03EB6DF3        8D8C05 4EECFFFF            LEA ECX,DWORD PTR SS:[EBP+EAX-0x13B2]
$+2DA    03EB6DFA        51                         PUSH ECX
$+2DB    03EB6DFB        8B95 90EBFFFF              MOV EDX,DWORD PTR SS:[EBP-0x1470]
$+2E1    03EB6E01        83C2 40                    ADD EDX,0x40
$+2E4    03EB6E04        52                         PUSH EDX
$+2E5    03EB6E05        8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$+2E8    03EB6E08        83C1 0C                    ADD ECX,0xC
$+2EB    03EB6E0B        E8 75D1F2FF                CALL TCLS.03DE3F85
$+2F0    03EB6E10        8B85 90EBFFFF              MOV EAX,DWORD PTR SS:[EBP-0x1470]
$+2F6    03EB6E16        05 A0000000                ADD EAX,0xA0
$+2FB    03EB6E1B        8985 90EBFFFF              MOV DWORD PTR SS:[EBP-0x1470],EAX
$+301    03EB6E21      ^ EB 80                      JMP SHORT TCLS.03EB6DA3
$+303    03EB6E23        68 3A140000                PUSH 0x143A
$+308    03EB6E28        8D8D 94EBFFFF              LEA ECX,DWORD PTR SS:[EBP-0x146C]
$+30E    03EB6E2E        51                         PUSH ECX
$+30F    03EB6E2F        8B55 F0                    MOV EDX,DWORD PTR SS:[EBP-0x10]
$+312    03EB6E32        8B8A 34130000              MOV ECX,DWORD PTR DS:[EDX+0x1334]
$+318    03EB6E38        8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+31B    03EB6E3B        8B90 34130000              MOV EDX,DWORD PTR DS:[EAX+0x1334]
$+321    03EB6E41        8B02                       MOV EAX,DWORD PTR DS:[EDX]
$+323    03EB6E43        FF10                       CALL DWORD PTR DS:[EAX]
$+325    03EB6E45        C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
$+32C    03EB6E4C        8D4D E0                    LEA ECX,DWORD PTR SS:[EBP-0x20]
$+32F    03EB6E4F        E8 B4CBF2FF                CALL TCLS.03DE3A08
$+334    03EB6E54        8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+337    03EB6E57        64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+33E    03EB6E5E        5F                         POP EDI
$+33F    03EB6E5F        5E                         POP ESI
$+340    03EB6E60        5B                         POP EBX
$+341    03EB6E61        8BE5                       MOV ESP,EBP
$+343    03EB6E63        5D                         POP EBP
$+344    03EB6E64        C2 0C00                    RETN 0xC
$+347    03EB6E67        CC                         INT3
*/
/**/
//////////////////////////////////////////////////////////////////////////
/**/
//下面的汇编没有所属模块，是分配在堆上的。
/**/
//tag_刷新所有快速登陆项

//8b??????????3b??8b????73??e8????????8b????8b????8b??89????8b????89????83????0f??????????8b??????????83????ff????5168
/*
$-D4     091FF24B      CC                         INT3
$-D3     091FF24C      0000                       ADD BYTE PTR DS:[EAX],AL
$-D1     091FF24E      0000                       ADD BYTE PTR DS:[EAX],AL
$-CF     091FF250      1C F1                      SBB AL,0xF1
$-CD     091FF252      1F                         POP DS
$-CC     091FF253      09EC                       OR ESP,EBP
$-CA     091FF255      4F                         DEC EDI
$-C9     091FF256      2009                       AND BYTE PTR DS:[ECX],CL
$-C7     091FF258      0000                       ADD BYTE PTR DS:[EAX],AL
$-C5     091FF25A      0000                       ADD BYTE PTR DS:[EAX],AL
$-C3     091FF25C      08F3                       OR BL,DH
$-C1     091FF25E      1F                         POP DS
$-C0     091FF25F      0900                       OR DWORD PTR DS:[EAX],EAX
$-BE     091FF261      0000                       ADD BYTE PTR DS:[EAX],AL
$-BC     091FF263      55                         PUSH EBP
$-BB     091FF264      8BEC                       MOV EBP,ESP
$-B9     091FF266      83EC 28                    SUB ESP,0x28
$-B6     091FF269      895D FC                    MOV DWORD PTR SS:[EBP-0x4],EBX
$-B3     091FF26C      8975 EC                    MOV DWORD PTR SS:[EBP-0x14],ESI
$-B0     091FF26F      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$-AD     091FF272      8B45 10                    MOV EAX,DWORD PTR SS:[EBP+0x10]
$-AA     091FF275      8D55 F0                    LEA EDX,DWORD PTR SS:[EBP-0x10]
$-A7     091FF278      8B35 50B05608              MOV ESI,DWORD PTR DS:[0x856B050]
$-A1     091FF27E      894D F4                    MOV DWORD PTR SS:[EBP-0xC],ECX
$-9E     091FF281      8975 F0                    MOV DWORD PTR SS:[EBP-0x10],ESI
$-9B     091FF284      8915 50B05608              MOV DWORD PTR DS:[0x856B050],EDX
$-95     091FF28A      8B35 40B05608              MOV ESI,DWORD PTR DS:[0x856B040]
$-8F     091FF290      3BD6                       CMP EDX,ESI
$-8D     091FF292      8B75 EC                    MOV ESI,DWORD PTR SS:[EBP-0x14]
$-8A     091FF295      73 08                      JNB SHORT 091FF29F
$-88     091FF297      E8 544D55FE                CALL 07753FF0
$-83     091FF29C      8B45 10                    MOV EAX,DWORD PTR SS:[EBP+0x10]
$-80     091FF29F      8B08                       MOV ECX,DWORD PTR DS:[EAX]
$-7E     091FF2A1      8B50 04                    MOV EDX,DWORD PTR DS:[EAX+0x4]
$-7B     091FF2A4      B8 04000000                MOV EAX,0x4
$-76     091FF2A9      8955 EC                    MOV DWORD PTR SS:[EBP-0x14],EDX
$-73     091FF2AC      8B91 9C000000              MOV EDX,DWORD PTR DS:[ECX+0x9C]
$-6D     091FF2B2      85D2                       TEST EDX,EDX
$-6B     091FF2B4      74 30                      JE SHORT 091FF2E6
$-69     091FF2B6      8B45 EC                    MOV EAX,DWORD PTR SS:[EBP-0x14]
$-66     091FF2B9      85D2                       TEST EDX,EDX
$-64     091FF2BB      74 36                      JE SHORT 091FF2F3
$-62     091FF2BD      8B5A 08                    MOV EBX,DWORD PTR DS:[EDX+0x8]
$-5F     091FF2C0      8B8B E4000000              MOV ECX,DWORD PTR DS:[EBX+0xE4]
$-59     091FF2C6      8B5D FC                    MOV EBX,DWORD PTR SS:[EBP-0x4]
$-56     091FF2C9      8955 E8                    MOV DWORD PTR SS:[EBP-0x18],EDX
$-53     091FF2CC      8D55 E8                    LEA EDX,DWORD PTR SS:[EBP-0x18]
$-50     091FF2CF      8945 EC                    MOV DWORD PTR SS:[EBP-0x14],EAX
$-4D     091FF2D2      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$-4A     091FF2D5      83EC 04                    SUB ESP,0x4
$-47     091FF2D8      52                         PUSH EDX
$-46     091FF2D9      6A 01                      PUSH 0x1
$-44     091FF2DB      51                         PUSH ECX
$-43     091FF2DC      FFD0                       CALL EAX
$-41     091FF2DE      83C4 10                    ADD ESP,0x10
$-3E     091FF2E1      B8 04000000                MOV EAX,0x4
$-39     091FF2E6      8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
$-36     091FF2E9      890D 50B05608              MOV DWORD PTR DS:[0x856B050],ECX
$-30     091FF2EF      8BE5                       MOV ESP,EBP
$-2E     091FF2F1      5D                         POP EBP
$-2D     091FF2F2      C3                         RETN
$-2C     091FF2F3      8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$-29     091FF2F6      83EC 0C                    SUB ESP,0xC
$-26     091FF2F9      53                         PUSH EBX
$-25     091FF2FA      E8 81DA5AFE                CALL 077ACD80
$-20     091FF2FF      83C4 10                    ADD ESP,0x10
$-1D     091FF302      CC                         INT3
$-1C     091FF303      CC                         INT3
$-1B     091FF304      CC                         INT3
$-1A     091FF305      CC                         INT3
$-19     091FF306      CC                         INT3
$-18     091FF307      CC                         INT3
$-17     091FF308      0000                       ADD BYTE PTR DS:[EAX],AL
$-15     091FF30A      0000                       ADD BYTE PTR DS:[EAX],AL
$-13     091FF30C      4C                         DEC ESP
$-12     091FF30D      F2:1F                      REPNE POP DS
$-10     091FF30F      09EC                       OR ESP,EBP
$-E      091FF311      4F                         DEC EDI
$-D      091FF312      2009                       AND BYTE PTR DS:[ECX],CL
$-B      091FF314      0000                       ADD BYTE PTR DS:[EAX],AL
$-9      091FF316      0000                       ADD BYTE PTR DS:[EAX],AL
$-7      091FF318      64:F5                      CMC
$-5      091FF31A      1F                         POP DS
$-4      091FF31B      0900                       OR DWORD PTR DS:[EAX],EAX
$-2      091FF31D      0000                       ADD BYTE PTR DS:[EAX],AL
$ ==>    091FF31F      55                         PUSH EBP                                      ; tag_刷新所有快速登陆项
$+1      091FF320      8BEC                       MOV EBP,ESP                                   ; 如果ret掉，则既不会清空，也不会重新遍历生成快速登陆项了
$+3      091FF322      83EC 68                    SUB ESP,0x68
$+6      091FF325      895D CC                    MOV DWORD PTR SS:[EBP-0x34],EBX
$+9      091FF328      8975 BC                    MOV DWORD PTR SS:[EBP-0x44],ESI
$+C      091FF32B      897D B8                    MOV DWORD PTR SS:[EBP-0x48],EDI
$+F      091FF32E      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$+12     091FF331      8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+15     091FF334      8D45 C0                    LEA EAX,DWORD PTR SS:[EBP-0x40]
$+18     091FF337      8B3D 50B05608              MOV EDI,DWORD PTR DS:[0x856B050]
$+1E     091FF33D      894D C4                    MOV DWORD PTR SS:[EBP-0x3C],ECX
$+21     091FF340      897D C0                    MOV DWORD PTR SS:[EBP-0x40],EDI
$+24     091FF343      8905 50B05608              MOV DWORD PTR DS:[0x856B050],EAX
$+2A     091FF349      8B3D 40B05608              MOV EDI,DWORD PTR DS:[0x856B040]
$+30     091FF34F      3BC7                       CMP EAX,EDI
$+32     091FF351      8B7D B8                    MOV EDI,DWORD PTR SS:[EBP-0x48]
$+35     091FF354      73 0B                      JNB SHORT 091FF361
$+37     091FF356      E8 954C55FE                CALL 07753FF0
$+3C     091FF35B      8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+3F     091FF35E      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$+42     091FF361      8B02                       MOV EAX,DWORD PTR DS:[EDX]
$+44     091FF363      8945 B4                    MOV DWORD PTR SS:[EBP-0x4C],EAX
$+47     091FF366      8B42 04                    MOV EAX,DWORD PTR DS:[EDX+0x4]
$+4A     091FF369      8945 B8                    MOV DWORD PTR SS:[EBP-0x48],EAX
$+4D     091FF36C      83F8 04                    CMP EAX,0x4
$+50     091FF36F      0F82 D8010000              JB 091FF54D
$+56     091FF375      8B05 6826FA06              MOV EAX,DWORD PTR DS:[0x6FA2668]
$+5C     091FF37B      83EC 04                    SUB ESP,0x4
$+5F     091FF37E      FF75 B8                    PUSH DWORD PTR SS:[EBP-0x48]
$+62     091FF381      51                         PUSH ECX
$+63     091FF382      68 5826FA06                PUSH 0x6FA2658
$+68     091FF387      FFD0                       CALL EAX
$+6A     091FF389      83C4 10                    ADD ESP,0x10
$+6D     091FF38C      83EC 0C                    SUB ESP,0xC
$+70     091FF38F      50                         PUSH EAX
$+71     091FF390      E8 8BE154FE                CALL 0774D520
$+76     091FF395      83C4 10                    ADD ESP,0x10
$+79     091FF398      8B55 B4                    MOV EDX,DWORD PTR SS:[EBP-0x4C]
$+7C     091FF39B      8942 54                    MOV DWORD PTR DS:[EDX+0x54],EAX               ; 此处改写数量,有几个QQ，数量便是几
$+7F     091FF39E      8B82 D4000000              MOV EAX,DWORD PTR DS:[EDX+0xD4]
$+85     091FF3A4      8945 B0                    MOV DWORD PTR SS:[EBP-0x50],EAX
$+88     091FF3A7      85C0                       TEST EAX,EAX
$+8A     091FF3A9      74 42                      JE SHORT 091FF3ED
$+8C     091FF3AB      8BC2                       MOV EAX,EDX
$+8E     091FF3AD      8B55 B0                    MOV EDX,DWORD PTR SS:[EBP-0x50]
$+91     091FF3B0      8B70 54                    MOV ESI,DWORD PTR DS:[EAX+0x54]
$+94     091FF3B3      83FE 00                    CMP ESI,0x0
$+97     091FF3B6      0F9FC0                     SETG AL
$+9A     091FF3B9      0FB6C0                     MOVZX EAX,AL
$+9D     091FF3BC      85D2                       TEST EDX,EDX
$+9F     091FF3BE      0F84 89010000              JE 091FF54D
$+A5     091FF3C4      8BC0                       MOV EAX,EAX
$+A7     091FF3C6      8B72 08                    MOV ESI,DWORD PTR DS:[EDX+0x8]
$+AA     091FF3C9      8B8E 34010000              MOV ECX,DWORD PTR DS:[ESI+0x134]
$+B0     091FF3CF      8B75 BC                    MOV ESI,DWORD PTR SS:[EBP-0x44]
$+B3     091FF3D2      8955 A8                    MOV DWORD PTR SS:[EBP-0x58],EDX
$+B6     091FF3D5      8D55 A8                    LEA EDX,DWORD PTR SS:[EBP-0x58]
$+B9     091FF3D8      8945 AC                    MOV DWORD PTR SS:[EBP-0x54],EAX
$+BC     091FF3DB      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+BF     091FF3DE      83EC 04                    SUB ESP,0x4
$+C2     091FF3E1      52                         PUSH EDX
$+C3     091FF3E2      6A 01                      PUSH 0x1
$+C5     091FF3E4      51                         PUSH ECX
$+C6     091FF3E5      FFD0                       CALL EAX
$+C8     091FF3E7      83C4 10                    ADD ESP,0x10
$+CB     091FF3EA      8B55 B4                    MOV EDX,DWORD PTR SS:[EBP-0x4C]
$+CE     091FF3ED      8B4A 54                    MOV ECX,DWORD PTR DS:[EDX+0x54]
$+D1     091FF3F0      83F9 00                    CMP ECX,0x0
$+D4     091FF3F3      0F9EC0                     SETLE AL
$+D7     091FF3F6      0FB6C0                     MOVZX EAX,AL
$+DA     091FF3F9      8945 E8                    MOV DWORD PTR SS:[EBP-0x18],EAX
$+DD     091FF3FC      B8 04000000                MOV EAX,0x4
$+E2     091FF401      83F9 00                    CMP ECX,0x0
$+E5     091FF404      7F 0F                      JG SHORT 091FF415
$+E7     091FF406      8B4A 5C                    MOV ECX,DWORD PTR DS:[EDX+0x5C]
$+EA     091FF409      83F9 02                    CMP ECX,0x2
$+ED     091FF40C      0F94C9                     SETE CL
$+F0     091FF40F      0FB6C9                     MOVZX ECX,CL
$+F3     091FF412      894D E8                    MOV DWORD PTR SS:[EBP-0x18],ECX
$+F6     091FF415      8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
$+F9     091FF418      85C9                       TEST ECX,ECX
$+FB     091FF41A      0F84 8A000000              JE 091FF4AA
$+101    091FF420      8B45 B4                    MOV EAX,DWORD PTR SS:[EBP-0x4C]
$+104    091FF423      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+107    091FF426      8B8A A0030000              MOV ECX,DWORD PTR DS:[EDX+0x3A0]
$+10D    091FF42C      8D55 A8                    LEA EDX,DWORD PTR SS:[EBP-0x58]
$+110    091FF42F      8945 A8                    MOV DWORD PTR SS:[EBP-0x58],EAX
$+113    091FF432      C745 AC 01000000           MOV DWORD PTR SS:[EBP-0x54],0x1
$+11A    091FF439      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+11D    091FF43C      83EC 04                    SUB ESP,0x4
$+120    091FF43F      52                         PUSH EDX
$+121    091FF440      6A 01                      PUSH 0x1
$+123    091FF442      51                         PUSH ECX
$+124    091FF443      FFD0                       CALL EAX
$+126    091FF445      83C4 10                    ADD ESP,0x10
$+129    091FF448      8B55 B4                    MOV EDX,DWORD PTR SS:[EBP-0x4C]
$+12C    091FF44B      8B4A 6C                    MOV ECX,DWORD PTR DS:[EDX+0x6C]
$+12F    091FF44E      85C9                       TEST ECX,ECX
$+131    091FF450      0F94C0                     SETE AL
$+134    091FF453      0FB6C0                     MOVZX EAX,AL
$+137    091FF456      8945 E8                    MOV DWORD PTR SS:[EBP-0x18],EAX
$+13A    091FF459      B8 04000000                MOV EAX,0x4
$+13F    091FF45E      85C9                       TEST ECX,ECX
$+141    091FF460      75 0E                      JNZ SHORT 091FF470
$+143    091FF462      8B4A 78                    MOV ECX,DWORD PTR DS:[EDX+0x78]
$+146    091FF465      85C9                       TEST ECX,ECX
$+148    091FF467      0F94C9                     SETE CL
$+14B    091FF46A      0FB6C9                     MOVZX ECX,CL
$+14E    091FF46D      894D E8                    MOV DWORD PTR SS:[EBP-0x18],ECX
$+151    091FF470      8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
$+154    091FF473      85C9                       TEST ECX,ECX
$+156    091FF475      74 33                      JE SHORT 091FF4AA
$+158    091FF477      8BCA                       MOV ECX,EDX
$+15A    091FF479      8B81 94000000              MOV EAX,DWORD PTR DS:[ECX+0x94]
$+160    091FF47F      85C0                       TEST EAX,EAX
$+162    091FF481      0F84 C6000000              JE 091FF54D
$+168    091FF487      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+16B    091FF48A      8B4A 7C                    MOV ECX,DWORD PTR DS:[EDX+0x7C]
$+16E    091FF48D      8D55 BC                    LEA EDX,DWORD PTR SS:[EBP-0x44]
$+171    091FF490      8945 BC                    MOV DWORD PTR SS:[EBP-0x44],EAX
$+174    091FF493      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+177    091FF496      83EC 04                    SUB ESP,0x4
$+17A    091FF499      52                         PUSH EDX
$+17B    091FF49A      6A 00                      PUSH 0x0
$+17D    091FF49C      51                         PUSH ECX
$+17E    091FF49D      FFD0                       CALL EAX
$+180    091FF49F      83C4 10                    ADD ESP,0x10
$+183    091FF4A2      8B55 B4                    MOV EDX,DWORD PTR SS:[EBP-0x4C]
$+186    091FF4A5      B8 04000000                MOV EAX,0x4
$+18B    091FF4AA      8B8A D0000000              MOV ECX,DWORD PTR DS:[EDX+0xD0]
$+191    091FF4B0      894D BC                    MOV DWORD PTR SS:[EBP-0x44],ECX
$+194    091FF4B3      85C9                       TEST ECX,ECX
$+196    091FF4B5      0F84 85000000              JE 091FF540
$+19C    091FF4BB      8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$+19F    091FF4BE      83EC 0C                    SUB ESP,0xC
$+1A2    091FF4C1      FF75 B8                    PUSH DWORD PTR SS:[EBP-0x48]
$+1A5    091FF4C4      E8 D7C154FE                CALL 0774B6A0
$+1AA    091FF4C9      83C4 10                    ADD ESP,0x10
$+1AD    091FF4CC      8B55 BC                    MOV EDX,DWORD PTR SS:[EBP-0x44]
$+1B0    091FF4CF      8B4D B8                    MOV ECX,DWORD PTR SS:[EBP-0x48]
$+1B3    091FF4D2      85C0                       TEST EAX,EAX
$+1B5    091FF4D4      74 28                      JE SHORT 091FF4FE
$+1B7    091FF4D6      8B05 8026FA06              MOV EAX,DWORD PTR DS:[0x6FA2680]
$+1BD    091FF4DC      83EC 04                    SUB ESP,0x4
$+1C0    091FF4DF      51                         PUSH ECX
$+1C1    091FF4E0      53                         PUSH EBX
$+1C2    091FF4E1      68 7026FA06                PUSH 0x6FA2670
$+1C7    091FF4E6      FFD0                       CALL EAX
$+1C9    091FF4E8      83C4 10                    ADD ESP,0x10
$+1CC    091FF4EB      8B55 BC                    MOV EDX,DWORD PTR SS:[EBP-0x44]
$+1CF    091FF4EE      BB 01000000                MOV EBX,0x1
$+1D4    091FF4F3      83F8 04                    CMP EAX,0x4
$+1D7    091FF4F6      0F45D8                     CMOVNE EBX,EAX
$+1DA    091FF4F9      895D F0                    MOV DWORD PTR SS:[EBP-0x10],EBX
$+1DD    091FF4FC      EB 12                      JMP SHORT 091FF510
$+1DF    091FF4FE      8BDD                       MOV EBX,EBP
$+1E1    091FF500      81EB 59CD794C              SUB EBX,0x4C79CD59
$+1E7    091FF506      C783 49CD794C 01000000     MOV DWORD PTR DS:[EBX+0x4C79CD49],0x1
$+1F1    091FF510      85D2                       TEST EDX,EDX
$+1F3    091FF512      74 39                      JE SHORT 091FF54D
$+1F5    091FF514      8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+1F8    091FF517      8B5A 08                    MOV EBX,DWORD PTR DS:[EDX+0x8]
$+1FB    091FF51A      8B8B D4000000              MOV ECX,DWORD PTR DS:[EBX+0xD4]
$+201    091FF520      8B5D CC                    MOV EBX,DWORD PTR SS:[EBP-0x34]
$+204    091FF523      8955 B8                    MOV DWORD PTR SS:[EBP-0x48],EDX
$+207    091FF526      8D55 B8                    LEA EDX,DWORD PTR SS:[EBP-0x48]
$+20A    091FF529      8945 BC                    MOV DWORD PTR SS:[EBP-0x44],EAX
$+20D    091FF52C      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+210    091FF52F      83EC 04                    SUB ESP,0x4
$+213    091FF532      52                         PUSH EDX
$+214    091FF533      6A 01                      PUSH 0x1
$+216    091FF535      51                         PUSH ECX
$+217    091FF536      FFD0                       CALL EAX
$+219    091FF538      83C4 10                    ADD ESP,0x10
$+21C    091FF53B      B8 04000000                MOV EAX,0x4
$+221    091FF540      8B4D C0                    MOV ECX,DWORD PTR SS:[EBP-0x40]
$+224    091FF543      890D 50B05608              MOV DWORD PTR DS:[0x856B050],ECX
$+22A    091FF549      8BE5                       MOV ESP,EBP
$+22C    091FF54B      5D                         POP EBP
$+22D    091FF54C      C3                         RETN
$+22E    091FF54D      8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$+231    091FF550      83EC 0C                    SUB ESP,0xC
$+234    091FF553      53                         PUSH EBX
$+235    091FF554      E8 27D85AFE                CALL 077ACD80
$+23A    091FF559      83C4 10                    ADD ESP,0x10
$+23D    091FF55C      CC                         INT3
*/
//tag_重新遍历生成QQ快速登陆项。执行到该函数时，快速登陆的面板数据已被清空了，该函数负责重新遍历生成快速登陆项。

//c7??????????????????8b????89????8b??????????85??0f??????????8b??81??????????c7??????????????????e9????????8b??????????85??74??83
/*
$-15     09203343      CC                         INT3
$-14     09203344      0000                       ADD BYTE PTR DS:[EAX],AL
$-12     09203346      0000                       ADD BYTE PTR DS:[EAX],AL
$-10     09203348      A0 322009EC                MOV AL,BYTE PTR DS:[0xEC092032]
$-B      0920334D      4F                         DEC EDI
$-A      0920334E      2009                       AND BYTE PTR DS:[ECX],CL
$-8      09203350      0000                       ADD BYTE PTR DS:[EAX],AL
$-6      09203352      0000                       ADD BYTE PTR DS:[EAX],AL
$-4      09203354      1C 39                      SBB AL,0x39
$-2      09203356      2009                       AND BYTE PTR DS:[ECX],CL
$ ==>    09203358      55                         PUSH EBP                                      ; tag_重新遍历生成QQ快速登陆项
$+1      09203359      8BEC                       MOV EBP,ESP                                   ; 有3个参数，p2一直=0
$+3      0920335B      81EC B8000000              SUB ESP,0xB8                                  ; p3是个堆栈指针，是指针的指针
$+9      09203361      895D 8C                    MOV DWORD PTR SS:[EBP-0x74],EBX
$+C      09203364      8975 88                    MOV DWORD PTR SS:[EBP-0x78],ESI
$+F      09203367      897D 80                    MOV DWORD PTR SS:[EBP-0x80],EDI
$+12     0920336A      8B55 08                    MOV EDX,DWORD PTR SS:[EBP+0x8]
$+15     0920336D      8995 74FFFFFF              MOV DWORD PTR SS:[EBP-0x8C],EDX
$+1B     09203373      8B4D 10                    MOV ECX,DWORD PTR SS:[EBP+0x10]
$+1E     09203376      8D45 90                    LEA EAX,DWORD PTR SS:[EBP-0x70]
$+21     09203379      BB 20B05608                MOV EBX,0x856B020
$+26     0920337E      899D 78FFFFFF              MOV DWORD PTR SS:[EBP-0x88],EBX
$+2C     09203384      8B1D 50B05608              MOV EBX,DWORD PTR DS:[0x856B050]
$+32     0920338A      8955 94                    MOV DWORD PTR SS:[EBP-0x6C],EDX
$+35     0920338D      895D 90                    MOV DWORD PTR SS:[EBP-0x70],EBX
$+38     09203390      8905 50B05608              MOV DWORD PTR DS:[0x856B050],EAX
$+3E     09203396      8B1D 40B05608              MOV EBX,DWORD PTR DS:[0x856B040]
$+44     0920339C      3BC3                       CMP EAX,EBX
$+46     0920339E      8B5D 8C                    MOV EBX,DWORD PTR SS:[EBP-0x74]
$+49     092033A1      73 13                      JNB SHORT 092033B6
$+4B     092033A3      8D09                       LEA ECX,DWORD PTR DS:[ECX]
$+4D     092033A5      8B8D 74FFFFFF              MOV ECX,DWORD PTR SS:[EBP-0x8C]
$+53     092033AB      E8 400C55FE                CALL 07753FF0
$+58     092033B0      8B55 08                    MOV EDX,DWORD PTR SS:[EBP+0x8]
$+5B     092033B3      8B4D 10                    MOV ECX,DWORD PTR SS:[EBP+0x10]
$+5E     092033B6      B8 04000000                MOV EAX,0x4
$+63     092033BB      8985 7CFFFFFF              MOV DWORD PTR SS:[EBP-0x84],EAX
$+69     092033C1      8B01                       MOV EAX,DWORD PTR DS:[ECX]
$+6B     092033C3      8985 68FFFFFF              MOV DWORD PTR SS:[EBP-0x98],EAX
$+71     092033C9      8945 A0                    MOV DWORD PTR SS:[EBP-0x60],EAX
$+74     092033CC      8B4A 0C                    MOV ECX,DWORD PTR DS:[EDX+0xC]
$+77     092033CF      894D 84                    MOV DWORD PTR SS:[EBP-0x7C],ECX
$+7A     092033D2      8B51 08                    MOV EDX,DWORD PTR DS:[ECX+0x8]
$+7D     092033D5      8995 6CFFFFFF              MOV DWORD PTR SS:[EBP-0x94],EDX
$+83     092033DB      8B4A 14                    MOV ECX,DWORD PTR DS:[EDX+0x14]
$+86     092033DE      898D 70FFFFFF              MOV DWORD PTR SS:[EBP-0x90],ECX
$+8C     092033E4      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+8F     092033E7      8B8A D0000000              MOV ECX,DWORD PTR DS:[EDX+0xD0]
$+95     092033ED      8D95 64FFFFFF              LEA EDX,DWORD PTR SS:[EBP-0x9C]
$+9B     092033F3      8985 64FFFFFF              MOV DWORD PTR SS:[EBP-0x9C],EAX
$+A1     092033F9      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+A4     092033FC      83EC 04                    SUB ESP,0x4
$+A7     092033FF      52                         PUSH EDX
$+A8     09203400      6A 00                      PUSH 0x0
$+AA     09203402      51                         PUSH ECX
$+AB     09203403      FFD0                       CALL EAX
$+AD     09203405      83C4 10                    ADD ESP,0x10
$+B0     09203408      8B8D 70FFFFFF              MOV ECX,DWORD PTR SS:[EBP-0x90]
$+B6     0920340E      8945 9C                    MOV DWORD PTR SS:[EBP-0x64],EAX
$+B9     09203411      8D49 40                    LEA ECX,DWORD PTR DS:[ECX+0x40]
$+BC     09203414      8D55 90                    LEA EDX,DWORD PTR SS:[EBP-0x70]
$+BF     09203417      E8 E42759FE                CALL 07795C00
$+C4     0920341C      8BD0                       MOV EDX,EAX
$+C6     0920341E      8B45 9C                    MOV EAX,DWORD PTR SS:[EBP-0x64]
$+C9     09203421      8B4A 6C                    MOV ECX,DWORD PTR DS:[EDX+0x6C]
$+CC     09203424      8D49 01                    LEA ECX,DWORD PTR DS:[ECX+0x1]
$+CF     09203427      83EC 04                    SUB ESP,0x4
$+D2     0920342A      51                         PUSH ECX
$+D3     0920342B      50                         PUSH EAX
$+D4     0920342C      FFB5 74FFFFFF              PUSH DWORD PTR SS:[EBP-0x8C]
$+DA     09203432      E8 893459FE                CALL 077968C0
$+DF     09203437      83C4 10                    ADD ESP,0x10
$+E2     0920343A      8B95 68FFFFFF              MOV EDX,DWORD PTR SS:[EBP-0x98]
$+E8     09203440      8BC8                       MOV ECX,EAX
$+EA     09203442      B8 04000000                MOV EAX,0x4
$+EF     09203447      83E1 F8                    AND ECX,0xFFFFFFF8
$+F2     0920344A      894D 9C                    MOV DWORD PTR SS:[EBP-0x64],ECX
$+F5     0920344D      894D A8                    MOV DWORD PTR SS:[EBP-0x58],ECX
$+F8     09203450      85C9                       TEST ECX,ECX
$+FA     09203452      74 38                      JE SHORT 0920348C
$+FC     09203454      8B45 9C                    MOV EAX,DWORD PTR SS:[EBP-0x64]
$+FF     09203457      85C0                       TEST EAX,EAX
$+101    09203459      0F84 91040000              JE 092038F0
$+107    0920345F      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+10A    09203462      8B4A 50                    MOV ECX,DWORD PTR DS:[EDX+0x50]
$+10D    09203465      8D55 9C                    LEA EDX,DWORD PTR SS:[EBP-0x64]
$+110    09203468      8945 9C                    MOV DWORD PTR SS:[EBP-0x64],EAX
$+113    0920346B      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+116    0920346E      83EC 04                    SUB ESP,0x4
$+119    09203471      52                         PUSH EDX
$+11A    09203472      6A 00                      PUSH 0x0
$+11C    09203474      51                         PUSH ECX
$+11D    09203475      FFD0                       CALL EAX
$+11F    09203477      83C4 10                    ADD ESP,0x10
$+122    0920347A      8B95 68FFFFFF              MOV EDX,DWORD PTR SS:[EBP-0x98]
$+128    09203480      8BC8                       MOV ECX,EAX
$+12A    09203482      B8 04000000                MOV EAX,0x4
$+12F    09203487      894D E0                    MOV DWORD PTR SS:[EBP-0x20],ECX
$+132    0920348A      EB 12                      JMP SHORT 0920349E
$+134    0920348C      8BCD                       MOV ECX,EBP
$+136    0920348E      81E9 13B2E512              SUB ECX,0x12E5B213
$+13C    09203494      C781 F3B1E512 00000000     MOV DWORD PTR DS:[ECX+0x12E5B1F3],0x0
$+146    0920349E      8B4D E0                    MOV ECX,DWORD PTR SS:[EBP-0x20]
$+149    092034A1      894A 78                    MOV DWORD PTR DS:[EDX+0x78],ECX               ; 此处改写数量,有几个QQ，数量便是几
$+14C    092034A4      8B8A 94000000              MOV ECX,DWORD PTR DS:[EDX+0x94]
$+152    092034AA      85C9                       TEST ECX,ECX
$+154    092034AC      0F84 31040000              JE 092038E3
$+15A    092034B2      8BDD                       MOV EBX,EBP
$+15C    092034B4      81EB 13B2E512              SUB EBX,0x12E5B213
$+162    092034BA      C783 D3B1E512 00000000     MOV DWORD PTR DS:[EBX+0x12E5B1D3],0x0
$+16C    092034C4      E9 CB000000                JMP 09203594                                  ; 开始循环添加每个快速登陆项
$+171    092034C9      8B1D 48B05608              MOV EBX,DWORD PTR DS:[0x856B048]              ; 循环开始
$+177    092034CF      85DB                       TEST EBX,EBX
$+179    092034D1      74 0B                      JE SHORT 092034DE                             ; 跳转
$+17B    092034D3      83EC 0C                    SUB ESP,0xC
$+17E    092034D6      E8 24040000                CALL 092038FF                                 ; 不会执行到
$+183    092034DB      83C4 10                    ADD ESP,0x10
$+186    092034DE      8B75 A0                    MOV ESI,DWORD PTR SS:[EBP-0x60]
$+189    092034E1      89B5 68FFFFFF              MOV DWORD PTR SS:[EBP-0x98],ESI
$+18F    092034E7      8B9E A0000000              MOV EBX,DWORD PTR DS:[ESI+0xA0]
$+195    092034ED      8D5B 01                    LEA EBX,DWORD PTR DS:[EBX+0x1]
$+198    092034F0      8D75 9C                    LEA ESI,DWORD PTR SS:[EBP-0x64]
$+19B    092034F3      C745 9C 01000000           MOV DWORD PTR SS:[EBP-0x64],0x1
$+1A2    092034FA      56                         PUSH ESI
$+1A3    092034FB      6A 00                      PUSH 0x0
$+1A5    092034FD      53                         PUSH EBX
$+1A6    092034FE      FFB5 74FFFFFF              PUSH DWORD PTR SS:[EBP-0x8C]
$+1AC    09203504      E8 97CB5AFE                CALL 077B00A0                                 ; call tag_添加一个快速登陆项
$+1B1    09203509      83C4 10                    ADD ESP,0x10
$+1B4    0920350C      8BD8                       MOV EBX,EAX
$+1B6    0920350E      8D2424                     LEA ESP,DWORD PTR SS:[ESP]
$+1B9    09203511      83EC 04                    SUB ESP,0x4
$+1BC    09203514      68 C0CD4C08                PUSH 0x84CCDC0
$+1C1    09203519      53                         PUSH EBX
$+1C2    0920351A      FFB5 74FFFFFF              PUSH DWORD PTR SS:[EBP-0x8C]
$+1C8    09203520      E8 5B3159FE                CALL 07796680									; 这个没鸟用，ret掉也照样会添加成功快速登陆项
$+1CD    09203525      83C4 10                    ADD ESP,0x10
$+1D0    09203528      83E3 F8                    AND EBX,0xFFFFFFF8
$+1D3    0920352B      895D 9C                    MOV DWORD PTR SS:[EBP-0x64],EBX
$+1D6    0920352E      8B4D A8                    MOV ECX,DWORD PTR SS:[EBP-0x58]
$+1D9    09203531      8B5D C0                    MOV EBX,DWORD PTR SS:[EBP-0x40]
$+1DC    09203534      85C9                       TEST ECX,ECX
$+1DE    09203536      0F84 B4030000              JE 092038F0
$+1E4    0920353C      83EC 0C                    SUB ESP,0xC
$+1E7    0920353F      53                         PUSH EBX
$+1E8    09203540      E8 1B0559FE                CALL 07793A60			;这个函数只是进行了一些判断，并没有改写任何东西。会返回eax,是个指针
$+1ED    09203545      83C4 0C                    ADD ESP,0xC
$+1F0    09203548      8B95 68FFFFFF              MOV EDX,DWORD PTR SS:[EBP-0x98]
$+1F6    0920354E      8B4D 9C                    MOV ECX,DWORD PTR SS:[EBP-0x64]
$+1F9    09203551      8BF0                       MOV ESI,EAX
$+1FB    09203553      B8 01000000                MOV EAX,0x1
$+200    09203558      83FE 04                    CMP ESI,0x4
$+203    0920355B      0F45C6                     CMOVNE EAX,ESI
$+206    0920355E      8B7A 08                    MOV EDI,DWORD PTR DS:[EDX+0x8]
$+209    09203561      8BB7 0C010000              MOV ESI,DWORD PTR DS:[EDI+0x10C]
$+20F    09203567      8DBD 58FFFFFF              LEA EDI,DWORD PTR SS:[EBP-0xA8]
$+215    0920356D      8995 58FFFFFF              MOV DWORD PTR SS:[EBP-0xA8],EDX
$+21B    09203573      898D 5CFFFFFF              MOV DWORD PTR SS:[EBP-0xA4],ECX
$+221    09203579      8985 60FFFFFF              MOV DWORD PTR SS:[EBP-0xA0],EAX
$+227    0920357F      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+22A    09203582      83EC 04                    SUB ESP,0x4
$+22D    09203585      57                         PUSH EDI
$+22E    09203586      6A 02                      PUSH 0x2
$+230    09203588      56                         PUSH ESI
$+231    09203589      FFD0                       CALL EAX
$+233    0920358B      83C4 10                    ADD ESP,0x10
$+236    0920358E      8D5B 01                    LEA EBX,DWORD PTR DS:[EBX+0x1]
$+239    09203591      895D C0                    MOV DWORD PTR SS:[EBP-0x40],EBX
$+23C    09203594      8B5D C0                    MOV EBX,DWORD PTR SS:[EBP-0x40]
$+23F    09203597      8B7D A0                    MOV EDI,DWORD PTR SS:[EBP-0x60]
$+242    0920359A      897D 9C                    MOV DWORD PTR SS:[EBP-0x64],EDI
$+245    0920359D      8B77 78                    MOV ESI,DWORD PTR DS:[EDI+0x78]
$+248    092035A0      3BDE                       CMP EBX,ESI
$+24A    092035A2    ^ 0F8C 21FFFFFF              JL 092034C9                                   ; 循环结束
$+250    092035A8      8BBD 6CFFFFFF              MOV EDI,DWORD PTR SS:[EBP-0x94]
$+256    092035AE      8B75 88                    MOV ESI,DWORD PTR SS:[EBP-0x78]
$+259    092035B1      8B5D 84                    MOV EBX,DWORD PTR SS:[EBP-0x7C]
$+25C    092035B4      8B55 9C                    MOV EDX,DWORD PTR SS:[EBP-0x64]
$+25F    092035B7      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$+262    092035BA      8B85 70FFFFFF              MOV EAX,DWORD PTR SS:[EBP-0x90]
$+268    092035C0      8B5D 8C                    MOV EBX,DWORD PTR SS:[EBP-0x74]
$+26B    092035C3      8B7D 80                    MOV EDI,DWORD PTR SS:[EBP-0x80]
$+26E    092035C6      B8 04000000                MOV EAX,0x4
$+273    092035CB      8B8A 8C000000              MOV ECX,DWORD PTR DS:[EDX+0x8C]
$+279    092035D1      894D 80                    MOV DWORD PTR SS:[EBP-0x80],ECX
$+27C    092035D4      85C9                       TEST ECX,ECX
$+27E    092035D6      0F84 07030000              JE 092038E3
$+284    092035DC      8B45 80                    MOV EAX,DWORD PTR SS:[EBP-0x80]
$+287    092035DF      85C0                       TEST EAX,EAX
$+289    092035E1      0F84 09030000              JE 092038F0
$+28F    092035E7      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+292    092035EA      8B8A 24010000              MOV ECX,DWORD PTR DS:[EDX+0x124]
$+298    092035F0      8D55 80                    LEA EDX,DWORD PTR SS:[EBP-0x80]
$+29B    092035F3      8945 80                    MOV DWORD PTR SS:[EBP-0x80],EAX
$+29E    092035F6      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+2A1    092035F9      83EC 04                    SUB ESP,0x4
$+2A4    092035FC      52                         PUSH EDX
$+2A5    092035FD      6A 00                      PUSH 0x0
$+2A7    092035FF      51                         PUSH ECX
$+2A8    09203600      FFD0                       CALL EAX
$+2AA    09203602      83C4 10                    ADD ESP,0x10
$+2AD    09203605      8B55 84                    MOV EDX,DWORD PTR SS:[EBP-0x7C]
$+2B0    09203608      8B45 9C                    MOV EAX,DWORD PTR SS:[EBP-0x64]
$+2B3    0920360B      DD9D 78FFFFFF              FSTP QWORD PTR SS:[EBP-0x88]
$+2B9    09203611      8B88 98000000              MOV ECX,DWORD PTR DS:[EAX+0x98]
$+2BF    09203617      8B42 24                    MOV EAX,DWORD PTR DS:[EDX+0x24]
$+2C2    0920361A      83E0 F8                    AND EAX,0xFFFFFFF8
$+2C5    0920361D      8B50 18                    MOV EDX,DWORD PTR DS:[EAX+0x18]
$+2C8    09203620      E8 1B1856FE                CALL 07764E40
$+2CD    09203625      8B55 84                    MOV EDX,DWORD PTR SS:[EBP-0x7C]
$+2D0    09203628      8BC8                       MOV ECX,EAX
$+2D2    0920362A      8B45 9C                    MOV EAX,DWORD PTR SS:[EBP-0x64]
$+2D5    0920362D      85C9                       TEST ECX,ECX
$+2D7    0920362F      0F84 33010000              JE 09203768
$+2DD    09203635      8B75 9C                    MOV ESI,DWORD PTR SS:[EBP-0x64]
$+2E0    09203638      8B9E 8C000000              MOV EBX,DWORD PTR DS:[ESI+0x8C]
$+2E6    0920363E      8B86 90000000              MOV EAX,DWORD PTR DS:[ESI+0x90]
$+2EC    09203644      85C0                       TEST EAX,EAX
$+2EE    09203646      0F84 A4020000              JE 092038F0
$+2F4    0920364C      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+2F7    0920364F      8B8A 0C010000              MOV ECX,DWORD PTR DS:[EDX+0x10C]
$+2FD    09203655      8D55 84                    LEA EDX,DWORD PTR SS:[EBP-0x7C]
$+300    09203658      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+303    0920365B      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+306    0920365E      83EC 04                    SUB ESP,0x4
$+309    09203661      52                         PUSH EDX
$+30A    09203662      6A 00                      PUSH 0x0
$+30C    09203664      51                         PUSH ECX
$+30D    09203665      FFD0                       CALL EAX
$+30F    09203667      83C4 10                    ADD ESP,0x10
$+312    0920366A      DD9D 70FFFFFF              FSTP QWORD PTR SS:[EBP-0x90]
$+318    09203670      8B86 94000000              MOV EAX,DWORD PTR DS:[ESI+0x94]
$+31E    09203676      85C0                       TEST EAX,EAX
$+320    09203678      0F84 72020000              JE 092038F0
$+326    0920367E      8B48 08                    MOV ECX,DWORD PTR DS:[EAX+0x8]
$+329    09203681      8BB1 0C010000              MOV ESI,DWORD PTR DS:[ECX+0x10C]
$+32F    09203687      8D4D 84                    LEA ECX,DWORD PTR SS:[EBP-0x7C]
$+332    0920368A      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+335    0920368D      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+338    09203690      83EC 04                    SUB ESP,0x4
$+33B    09203693      51                         PUSH ECX
$+33C    09203694      6A 00                      PUSH 0x0
$+33E    09203696      56                         PUSH ESI
$+33F    09203697      FFD0                       CALL EAX
$+341    09203699      83C4 10                    ADD ESP,0x10
$+344    0920369C      F30F7E85 70FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x90]
$+34C    092036A4      DD9D 68FFFFFF              FSTP QWORD PTR SS:[EBP-0x98]
$+352    092036AA      F30F7E8D 68FFFFFF          MOVQ XMM1,QWORD PTR SS:[EBP-0x98]
$+35A    092036B2      85DB                       TEST EBX,EBX
$+35C    092036B4      0F84 36020000              JE 092038F0
$+362    092036BA      F20F2CF1                   CVTTSD2SI ESI,XMM1
$+366    092036BE      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+369    092036C1      81FE 00000080              CMP ESI,0x80000000
$+36F    092036C7      75 22                      JNZ SHORT 092036EB
$+371    092036C9      83EC 08                    SUB ESP,0x8
$+374    092036CC      83EC 08                    SUB ESP,0x8
$+377    092036CF      DD85 68FFFFFF              FLD QWORD PTR SS:[EBP-0x98]
$+37D    092036D5      DD1C24                     FSTP QWORD PTR SS:[ESP]
$+380    092036D8      E8 137954FE                CALL 0774AFF0
$+385    092036DD      83C4 10                    ADD ESP,0x10
$+388    092036E0      F30F7E85 70FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x90]
$+390    092036E8      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+393    092036EB      8B45 84                    MOV EAX,DWORD PTR SS:[EBP-0x7C]
$+396    092036EE      8945 80                    MOV DWORD PTR SS:[EBP-0x80],EAX
$+399    092036F1      F20F2CF0                   CVTTSD2SI ESI,XMM0
$+39D    092036F5      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+3A0    092036F8      81FE 00000080              CMP ESI,0x80000000
$+3A6    092036FE      75 1F                      JNZ SHORT 0920371F
$+3A8    09203700      83EC 08                    SUB ESP,0x8
$+3AB    09203703      83EC 08                    SUB ESP,0x8
$+3AE    09203706      DD85 70FFFFFF              FLD QWORD PTR SS:[EBP-0x90]
$+3B4    0920370C      DD1C24                     FSTP QWORD PTR SS:[ESP]
$+3B7    0920370F      E8 DC7854FE                CALL 0774AFF0
$+3BC    09203714      83C4 10                    ADD ESP,0x10
$+3BF    09203717      8BF0                       MOV ESI,EAX
$+3C1    09203719      8B45 80                    MOV EAX,DWORD PTR SS:[EBP-0x80]
$+3C4    0920371C      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+3C7    0920371F      8B55 84                    MOV EDX,DWORD PTR SS:[EBP-0x7C]
$+3CA    09203722      8B73 08                    MOV ESI,DWORD PTR DS:[EBX+0x8]
$+3CD    09203725      8B8E 00010000              MOV ECX,DWORD PTR DS:[ESI+0x100]
$+3D3    0920372B      8B75 88                    MOV ESI,DWORD PTR SS:[EBP-0x78]
$+3D6    0920372E      899D 68FFFFFF              MOV DWORD PTR SS:[EBP-0x98],EBX
$+3DC    09203734      8B5D 8C                    MOV EBX,DWORD PTR SS:[EBP-0x74]
$+3DF    09203737      8995 6CFFFFFF              MOV DWORD PTR SS:[EBP-0x94],EDX
$+3E5    0920373D      8D95 68FFFFFF              LEA EDX,DWORD PTR SS:[EBP-0x98]
$+3EB    09203743      8985 70FFFFFF              MOV DWORD PTR SS:[EBP-0x90],EAX
$+3F1    09203749      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+3F4    0920374C      83EC 04                    SUB ESP,0x4
$+3F7    0920374F      52                         PUSH EDX
$+3F8    09203750      6A 02                      PUSH 0x2
$+3FA    09203752      51                         PUSH ECX
$+3FB    09203753      FFD0                       CALL EAX
$+3FD    09203755      83C4 10                    ADD ESP,0x10
$+400    09203758      F30F7E85 78FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x88]
$+408    09203760      8B4D 9C                    MOV ECX,DWORD PTR SS:[EBP-0x64]
$+40B    09203763      E9 49010000                JMP 092038B1
$+410    09203768      8B88 98000000              MOV ECX,DWORD PTR DS:[EAX+0x98]
$+416    0920376E      8B42 24                    MOV EAX,DWORD PTR DS:[EDX+0x24]
$+419    09203771      83E0 F8                    AND EAX,0xFFFFFFF8
$+41C    09203774      8B50 1C                    MOV EDX,DWORD PTR DS:[EAX+0x1C]
$+41F    09203777      E8 C41656FE                CALL 07764E40
$+424    0920377C      F30F7E85 78FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x88]
$+42C    09203784      8B4D 9C                    MOV ECX,DWORD PTR SS:[EBP-0x64]
$+42F    09203787      85C0                       TEST EAX,EAX
$+431    09203789      0F84 22010000              JE 092038B1
$+437    0920378F      8B75 9C                    MOV ESI,DWORD PTR SS:[EBP-0x64]
$+43A    09203792      8B9E 8C000000              MOV EBX,DWORD PTR DS:[ESI+0x8C]
$+440    09203798      8B86 90000000              MOV EAX,DWORD PTR DS:[ESI+0x90]
$+446    0920379E      85C0                       TEST EAX,EAX
$+448    092037A0      0F84 4A010000              JE 092038F0
$+44E    092037A6      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+451    092037A9      8B8A 04010000              MOV ECX,DWORD PTR DS:[EDX+0x104]
$+457    092037AF      8D55 84                    LEA EDX,DWORD PTR SS:[EBP-0x7C]
$+45A    092037B2      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+45D    092037B5      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+460    092037B8      83EC 04                    SUB ESP,0x4
$+463    092037BB      52                         PUSH EDX
$+464    092037BC      6A 00                      PUSH 0x0
$+466    092037BE      51                         PUSH ECX
$+467    092037BF      FFD0                       CALL EAX
$+469    092037C1      83C4 10                    ADD ESP,0x10
$+46C    092037C4      DD9D 70FFFFFF              FSTP QWORD PTR SS:[EBP-0x90]
$+472    092037CA      8B86 94000000              MOV EAX,DWORD PTR DS:[ESI+0x94]
$+478    092037D0      85C0                       TEST EAX,EAX
$+47A    092037D2      0F84 18010000              JE 092038F0
$+480    092037D8      8B48 08                    MOV ECX,DWORD PTR DS:[EAX+0x8]
$+483    092037DB      8BB1 04010000              MOV ESI,DWORD PTR DS:[ECX+0x104]
$+489    092037E1      8D4D 84                    LEA ECX,DWORD PTR SS:[EBP-0x7C]
$+48C    092037E4      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+48F    092037E7      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+492    092037EA      83EC 04                    SUB ESP,0x4
$+495    092037ED      51                         PUSH ECX
$+496    092037EE      6A 00                      PUSH 0x0
$+498    092037F0      56                         PUSH ESI
$+499    092037F1      FFD0                       CALL EAX
$+49B    092037F3      83C4 10                    ADD ESP,0x10
$+49E    092037F6      F30F7E85 70FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x90]
$+4A6    092037FE      DD9D 68FFFFFF              FSTP QWORD PTR SS:[EBP-0x98]
$+4AC    09203804      F30F7E8D 68FFFFFF          MOVQ XMM1,QWORD PTR SS:[EBP-0x98]
$+4B4    0920380C      85DB                       TEST EBX,EBX
$+4B6    0920380E      0F84 DC000000              JE 092038F0
$+4BC    09203814      F20F2CF1                   CVTTSD2SI ESI,XMM1
$+4C0    09203818      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+4C3    0920381B      81FE 00000080              CMP ESI,0x80000000
$+4C9    09203821      75 22                      JNZ SHORT 09203845
$+4CB    09203823      83EC 08                    SUB ESP,0x8
$+4CE    09203826      83EC 08                    SUB ESP,0x8
$+4D1    09203829      DD85 68FFFFFF              FLD QWORD PTR SS:[EBP-0x98]
$+4D7    0920382F      DD1C24                     FSTP QWORD PTR SS:[ESP]
$+4DA    09203832      E8 B97754FE                CALL 0774AFF0
$+4DF    09203837      83C4 10                    ADD ESP,0x10
$+4E2    0920383A      F30F7E85 70FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x90]
$+4EA    09203842      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+4ED    09203845      8B45 84                    MOV EAX,DWORD PTR SS:[EBP-0x7C]
$+4F0    09203848      8945 80                    MOV DWORD PTR SS:[EBP-0x80],EAX
$+4F3    0920384B      F20F2CF0                   CVTTSD2SI ESI,XMM0
$+4F7    0920384F      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+4FA    09203852      81FE 00000080              CMP ESI,0x80000000
$+500    09203858      75 1F                      JNZ SHORT 09203879
$+502    0920385A      83EC 08                    SUB ESP,0x8
$+505    0920385D      83EC 08                    SUB ESP,0x8
$+508    09203860      DD85 70FFFFFF              FLD QWORD PTR SS:[EBP-0x90]
$+50E    09203866      DD1C24                     FSTP QWORD PTR SS:[ESP]
$+511    09203869      E8 827754FE                CALL 0774AFF0
$+516    0920386E      83C4 10                    ADD ESP,0x10
$+519    09203871      8BF0                       MOV ESI,EAX
$+51B    09203873      8B45 80                    MOV EAX,DWORD PTR SS:[EBP-0x80]
$+51E    09203876      8975 84                    MOV DWORD PTR SS:[EBP-0x7C],ESI
$+521    09203879      8B55 84                    MOV EDX,DWORD PTR SS:[EBP-0x7C]
$+524    0920387C      8B73 08                    MOV ESI,DWORD PTR DS:[EBX+0x8]
$+527    0920387F      8B8E 00010000              MOV ECX,DWORD PTR DS:[ESI+0x100]
$+52D    09203885      8B75 88                    MOV ESI,DWORD PTR SS:[EBP-0x78]
$+530    09203888      895D 80                    MOV DWORD PTR SS:[EBP-0x80],EBX
$+533    0920388B      8B5D 8C                    MOV EBX,DWORD PTR SS:[EBP-0x74]
$+536    0920388E      8955 84                    MOV DWORD PTR SS:[EBP-0x7C],EDX
$+539    09203891      8D55 80                    LEA EDX,DWORD PTR SS:[EBP-0x80]
$+53C    09203894      8945 88                    MOV DWORD PTR SS:[EBP-0x78],EAX
$+53F    09203897      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+542    0920389A      83EC 04                    SUB ESP,0x4
$+545    0920389D      52                         PUSH EDX
$+546    0920389E      6A 02                      PUSH 0x2
$+548    092038A0      51                         PUSH ECX
$+549    092038A1      FFD0                       CALL EAX
$+54B    092038A3      83C4 10                    ADD ESP,0x10
$+54E    092038A6      F30F7E85 78FFFFFF          MOVQ XMM0,QWORD PTR SS:[EBP-0x88]
$+556    092038AE      8B4D 9C                    MOV ECX,DWORD PTR SS:[EBP-0x64]
$+559    092038B1      8B81 8C000000              MOV EAX,DWORD PTR DS:[ECX+0x8C]
$+55F    092038B7      85C0                       TEST EAX,EAX
$+561    092038B9      74 35                      JE SHORT 092038F0
$+563    092038BB      8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
$+566    092038BE      8B8A 28010000              MOV ECX,DWORD PTR DS:[EDX+0x128]
$+56C    092038C4      8945 84                    MOV DWORD PTR SS:[EBP-0x7C],EAX
$+56F    092038C7      660FD645 88                MOVQ QWORD PTR SS:[EBP-0x78],XMM0
$+574    092038CC      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+577    092038CF      8D55 84                    LEA EDX,DWORD PTR SS:[EBP-0x7C]
$+57A    092038D2      83EC 04                    SUB ESP,0x4
$+57D    092038D5      52                         PUSH EDX
$+57E    092038D6      6A 01                      PUSH 0x1
$+580    092038D8      51                         PUSH ECX
$+581    092038D9      FFD0                       CALL EAX
$+583    092038DB      83C4 10                    ADD ESP,0x10
$+586    092038DE      B8 04000000                MOV EAX,0x4
$+58B    092038E3      8B4D 90                    MOV ECX,DWORD PTR SS:[EBP-0x70]
$+58E    092038E6      890D 50B05608              MOV DWORD PTR DS:[0x856B050],ECX
$+594    092038EC      8BE5                       MOV ESP,EBP
$+596    092038EE      5D                         POP EBP
$+597    092038EF      C3                         RETN
$+598    092038F0      8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$+59B    092038F3      83EC 0C                    SUB ESP,0xC
$+59E    092038F6      53                         PUSH EBX
$+59F    092038F7      E8 84945AFE                CALL 077ACD80
$+5A4    092038FC      83C4 10                    ADD ESP,0x10
$+5A7    092038FF      60                         PUSHAD
$+5A8    09203900      8B5D 08                    MOV EBX,DWORD PTR SS:[EBP+0x8]
$+5AB    09203903      83EC 0C                    SUB ESP,0xC
$+5AE    09203906      53                         PUSH EBX
$+5AF    09203907      E8 A4BE54FE                CALL 0774F7B0
$+5B4    0920390C      83C4 10                    ADD ESP,0x10
$+5B7    0920390F      61                         POPAD
$+5B8    09203910      FF2424                     JMP DWORD PTR SS:[ESP]
$+5BB    09203913      CC                         INT3
*/
/**/
//tag_添加一个快速登陆项
/*
$-1      077B009F      CC                         INT3
$ ==>    077B00A0      8B4C24 08                  MOV ECX,DWORD PTR SS:[ESP+0x8]                ; tag_添加一个快速登陆项
$+4      077B00A4      8BC1                       MOV EAX,ECX
$+6      077B00A6      24 07                      AND AL,0x7
$+8      077B00A8      3C 01                      CMP AL,0x1
$+A      077B00AA      75 1A                      JNZ SHORT 077B00C6
$+C      077B00AC      83F9 04                    CMP ECX,0x4
$+F      077B00AF      72 15                      JB SHORT 077B00C6
$+11     077B00B1      8B4424 10                  MOV EAX,DWORD PTR SS:[ESP+0x10]
$+15     077B00B5      83E1 F8                    AND ECX,0xFFFFFFF8
$+18     077B00B8      8B11                       MOV EDX,DWORD PTR DS:[ECX]
$+1A     077B00BA      8B52 58                    MOV EDX,DWORD PTR DS:[EDX+0x58]
$+1D     077B00BD      50                         PUSH EAX
$+1E     077B00BE      8B4424 10                  MOV EAX,DWORD PTR SS:[ESP+0x10]
$+22     077B00C2      50                         PUSH EAX
$+23     077B00C3      FFD2                       CALL EDX
$+25     077B00C5      C3                         RETN
$+26     077B00C6      8B4424 04                  MOV EAX,DWORD PTR SS:[ESP+0x4]
$+2A     077B00CA      8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]
$+2D     077B00CD      8B51 04                    MOV EDX,DWORD PTR DS:[ECX+0x4]
$+30     077B00D0      8B4A 04                    MOV ECX,DWORD PTR DS:[EDX+0x4]
$+33     077B00D3      68 EF030000                PUSH 0x3EF
$+38     077B00D8      E8 C3EAFDFF                CALL 0778EBA0
$+3D     077B00DD      33C0                       XOR EAX,EAX
$+3F     077B00DF      C3                         RETN
$+40     077B00E0      8B4424 04                  MOV EAX,DWORD PTR SS:[ESP+0x4]
$+44     077B00E4      8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]
$+47     077B00E7      8B51 04                    MOV EDX,DWORD PTR DS:[ECX+0x4]
$+4A     077B00EA      8B42 04                    MOV EAX,DWORD PTR DS:[EDX+0x4]
$+4D     077B00ED      8B48 20                    MOV ECX,DWORD PTR DS:[EAX+0x20]
$+50     077B00F0      BA 09000000                MOV EDX,0x9
$+55     077B00F5      E8 46A3F9FF                CALL 0774A440
$+5A     077B00FA      8B4C24 08                  MOV ECX,DWORD PTR SS:[ESP+0x8]
$+5E     077B00FE      8B5424 0C                  MOV EDX,DWORD PTR SS:[ESP+0xC]
$+62     077B0102      51                         PUSH ECX
$+63     077B0103      52                         PUSH EDX
$+64     077B0104      8BC8                       MOV ECX,EAX
$+66     077B0106      E8 E511FEFF                CALL 077912F0
$+6B     077B010B      C3                         RETN
$+6C     077B010C      CC                         INT3
$+6D     077B010D      CC                         INT3
$+6E     077B010E      CC                         INT3
$+6F     077B010F      CC                         INT3
$+70     077B0110      8B4424 08                  MOV EAX,DWORD PTR SS:[ESP+0x8]
$+74     077B0114      56                         PUSH ESI
$+75     077B0115      8B7424 10                  MOV ESI,DWORD PTR SS:[ESP+0x10]
$+79     077B0119      85F6                       TEST ESI,ESI
$+7B     077B011B      74 3E                      JE SHORT 077B015B
$+7D     077B011D      83F8 04                    CMP EAX,0x4
$+80     077B0120      73 04                      JNB SHORT 077B0126
$+82     077B0122      33D2                       XOR EDX,EDX
$+84     077B0124      EB 05                      JMP SHORT 077B012B
$+86     077B0126      8BD0                       MOV EDX,EAX
$+88     077B0128      83E2 07                    AND EDX,0x7
$+8B     077B012B      8A8E 80000000              MOV CL,BYTE PTR DS:[ESI+0x80]
$+91     077B0131      57                         PUSH EDI
$+92     077B0132      BF 01000000                MOV EDI,0x1
$+97     077B0137      D3E7                       SHL EDI,CL
$+99     077B0139      853C95 8C11E107            TEST DWORD PTR DS:[EDX*4+0x7E1118C],EDI
$+A0     077B0140      5F                         POP EDI
$+A1     077B0141      75 18                      JNZ SHORT 077B015B
$+A3     077B0143      8B4C24 08                  MOV ECX,DWORD PTR SS:[ESP+0x8]
$+A7     077B0147      8B51 0C                    MOV EDX,DWORD PTR DS:[ECX+0xC]
$+AA     077B014A      8B4A 04                    MOV ECX,DWORD PTR DS:[EDX+0x4]
$+AD     077B014D      8B49 04                    MOV ECX,DWORD PTR DS:[ECX+0x4]
$+B0     077B0150      56                         PUSH ESI
$+B1     077B0151      50                         PUSH EAX
$+B2     077B0152      51                         PUSH ECX
$+B3     077B0153      E8 B85AFEFF                CALL 07795C10
$+B8     077B0158      83C4 0C                    ADD ESP,0xC
$+BB     077B015B      5E                         POP ESI
$+BC     077B015C      C3                         RETN
$+BD     077B015D      CC                         INT3
$+BE     077B015E      CC                         INT3
$+BF     077B015F      CC                         INT3
$+C0     077B0160      55                         PUSH EBP
$+C1     077B0161      8BEC                       MOV EBP,ESP
$+C3     077B0163      8B45 0C                    MOV EAX,DWORD PTR SS:[EBP+0xC]
$+C6     077B0166      8BC8                       MOV ECX,EAX
$+C8     077B0168      80E1 07                    AND CL,0x7
$+CB     077B016B      80F9 01                    CMP CL,0x1
$+CE     077B016E      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$+D1     077B0171      75 2F                      JNZ SHORT 077B01A2
$+D3     077B0173      8B50 07                    MOV EDX,DWORD PTR DS:[EAX+0x7]
$+D6     077B0176      3B11                       CMP EDX,DWORD PTR DS:[ECX]
$+D8     077B0178      75 28                      JNZ SHORT 077B01A2
$+DA     077B017A      8B55 14                    MOV EDX,DWORD PTR SS:[EBP+0x14]
$+DD     077B017D      8902                       MOV DWORD PTR DS:[EDX],EAX
$+DF     077B017F      8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
$+E2     077B0182      51                         PUSH ECX
$+E3     077B0183      8BCC                       MOV ECX,ESP
$+E5     077B0185      83E1 04                    AND ECX,0x4
$+E8     077B0188      2BE1                       SUB ESP,ECX
$+EA     077B018A      51                         PUSH ECX
$+EB     077B018B      8B48 08                    MOV ECX,DWORD PTR DS:[EAX+0x8]
$+EE     077B018E      52                         PUSH EDX
$+EF     077B018F      8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+F2     077B0192      52                         PUSH EDX
$+F3     077B0193      50                         PUSH EAX
$+F4     077B0194      8B41 08                    MOV EAX,DWORD PTR DS:[ECX+0x8]
$+F7     077B0197      FFD0                       CALL EAX
$+F9     077B0199      83C4 0C                    ADD ESP,0xC
$+FC     077B019C      59                         POP ECX
$+FD     077B019D      03E1                       ADD ESP,ECX
$+FF     077B019F      59                         POP ECX
$+100    077B01A0      5D                         POP EBP
$+101    077B01A1      C3                         RETN
$+102    077B01A2      8B55 18                    MOV EDX,DWORD PTR SS:[EBP+0x18]
$+105    077B01A5      52                         PUSH EDX
$+106    077B01A6      8B55 14                    MOV EDX,DWORD PTR SS:[EBP+0x14]
$+109    077B01A9      52                         PUSH EDX
$+10A    077B01AA      8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
$+10D    077B01AD      52                         PUSH EDX
$+10E    077B01AE      50                         PUSH EAX
$+10F    077B01AF      51                         PUSH ECX
$+110    077B01B0      E8 7BC9FFFF                CALL 077ACB30
$+115    077B01B5      83C4 14                    ADD ESP,0x14
$+118    077B01B8      5D                         POP EBP
$+119    077B01B9      C3                         RETN
$+11A    077B01BA      CC                         INT3
*/
//tag_左键点击快速登陆项的时候会断到
/*
$-16     091AD757        CC                      INT3
$-15     091AD758        0000                    ADD BYTE PTR DS:[EAX],AL
$-13     091AD75A        0000                    ADD BYTE PTR DS:[EAX],AL
$-11     091AD75C        2C D6                   SUB AL,0xD6
$-F      091AD75E        1A09                    SBB CL,BYTE PTR DS:[ECX]
$-D      091AD760        EC                      IN AL,DX
$-C      091AD761        4F                      DEC EDI
$-B      091AD762        1B09                    SBB ECX,DWORD PTR DS:[ECX]
$-9      091AD764        0000                    ADD BYTE PTR DS:[EAX],AL
$-7      091AD766        0000                    ADD BYTE PTR DS:[EAX],AL
$-5      091AD768        6C                      INS BYTE PTR ES:[EDI],DX
$-4      091AD769        DB1A                    FISTP DWORD PTR DS:[EDX]
$-2      091AD76B        0900                    OR DWORD PTR DS:[EAX],EAX
$ ==>    091AD76D        55                      PUSH EBP                                 ; tag_左键点击快速登陆项的时候会断到
$+1      091AD76E        8BEC                    MOV EBP,ESP
$+3      091AD770        83EC 78                 SUB ESP,0x78
$+6      091AD773        895D BC                 MOV DWORD PTR SS:[EBP-0x44],EBX
$+9      091AD776        8975 A8                 MOV DWORD PTR SS:[EBP-0x58],ESI
$+C      091AD779        897D A4                 MOV DWORD PTR SS:[EBP-0x5C],EDI
$+F      091AD77C        8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+12     091AD77F        8B5D 10                 MOV EBX,DWORD PTR SS:[EBP+0x10]
$+15     091AD782        8D7D B0                 LEA EDI,DWORD PTR SS:[EBP-0x50]
$+18     091AD785        8B0D 50B05108           MOV ECX,DWORD PTR DS:[0x851B050]
$+1E     091AD78B        8945 B4                 MOV DWORD PTR SS:[EBP-0x4C],EAX
$+21     091AD78E        894D B0                 MOV DWORD PTR SS:[EBP-0x50],ECX
$+24     091AD791        893D 50B05108           MOV DWORD PTR DS:[0x851B050],EDI
$+2A     091AD797        8B0D 40B05108           MOV ECX,DWORD PTR DS:[0x851B040]
$+30     091AD79D        3BF9                    CMP EDI,ECX
$+32     091AD79F        73 0A                   JNB SHORT 091AD7AB
$+34     091AD7A1        8BC8                    MOV ECX,EAX
$+36     091AD7A3        E8 486853FE             CALL 076E3FF0
$+3B     091AD7A8        8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+3E     091AD7AB        8B13                    MOV EDX,DWORD PTR DS:[EBX]
$+40     091AD7AD        8955 A0                 MOV DWORD PTR SS:[EBP-0x60],EDX
$+43     091AD7B0        8B7B 04                 MOV EDI,DWORD PTR DS:[EBX+0x4]
$+46     091AD7B3        8B58 0C                 MOV EBX,DWORD PTR DS:[EAX+0xC]
$+49     091AD7B6        8B43 08                 MOV EAX,DWORD PTR DS:[EBX+0x8]
$+4C     091AD7B9        8B58 14                 MOV EBX,DWORD PTR DS:[EAX+0x14]
$+4F     091AD7BC        895D AC                 MOV DWORD PTR SS:[EBP-0x54],EBX
$+52     091AD7BF        8B82 84000000           MOV EAX,DWORD PTR DS:[EDX+0x84]
$+58     091AD7C5        8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
$+5B     091AD7C8        85C0                    TEST EAX,EAX
$+5D     091AD7CA        74 0E                   JE SHORT 091AD7DA
$+5F     091AD7CC        8B42 7C                 MOV EAX,DWORD PTR DS:[EDX+0x7C]          ; 访问当前选择的快速登陆项的索引
$+62     091AD7CF        3BC7                    CMP EAX,EDI
$+64     091AD7D1        0F94C0                  SETE AL
$+67     091AD7D4        0FB6C0                  MOVZX EAX,AL
$+6A     091AD7D7        8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
$+6D     091AD7DA        8B45 E0                 MOV EAX,DWORD PTR SS:[EBP-0x20]
$+70     091AD7DD        85C0                    TEST EAX,EAX
$+72     091AD7DF        0F84 AC000000           JE 091AD891
$+78     091AD7E5        8B7D A4                 MOV EDI,DWORD PTR SS:[EBP-0x5C]
$+7B     091AD7E8        8D5B 14                 LEA EBX,DWORD PTR DS:[EBX+0x14]
$+7E     091AD7EB        895D 98                 MOV DWORD PTR SS:[EBP-0x68],EBX
$+81     091AD7EE        8D55 B0                 LEA EDX,DWORD PTR SS:[EBP-0x50]
$+84     091AD7F1        8B4D 98                 MOV ECX,DWORD PTR SS:[EBP-0x68]
$+87     091AD7F4        E8 078457FE             CALL 07725C00
$+8C     091AD7F9        8B4D 98                 MOV ECX,DWORD PTR SS:[EBP-0x68]
$+8F     091AD7FC        8945 9C                 MOV DWORD PTR SS:[EBP-0x64],EAX
$+92     091AD7FF        8D55 B0                 LEA EDX,DWORD PTR SS:[EBP-0x50]
$+95     091AD802        E8 F98357FE             CALL 07725C00
$+9A     091AD807        8BD8                    MOV EBX,EAX
$+9C     091AD809        8B45 9C                 MOV EAX,DWORD PTR SS:[EBP-0x64]
$+9F     091AD80C        8B4B 10                 MOV ECX,DWORD PTR DS:[EBX+0x10]
$+A2     091AD80F        85C9                    TEST ECX,ECX
$+A4     091AD811        0F84 27030000           JE 091ADB3E
$+AA     091AD817        8B59 2C                 MOV EBX,DWORD PTR DS:[ECX+0x2C]
$+AD     091AD81A        895D 9C                 MOV DWORD PTR SS:[EBP-0x64],EBX
$+B0     091AD81D        8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$+B3     091AD820        85C9                    TEST ECX,ECX
$+B5     091AD822        0F84 16030000           JE 091ADB3E
$+BB     091AD828        8B59 08                 MOV EBX,DWORD PTR DS:[ECX+0x8]
$+BE     091AD82B        8B53 10                 MOV EDX,DWORD PTR DS:[EBX+0x10]
$+C1     091AD82E        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+C4     091AD831        8B42 18                 MOV EAX,DWORD PTR DS:[EDX+0x18]
$+C7     091AD834        FFD0                    CALL EAX
$+C9     091AD836        8945 94                 MOV DWORD PTR SS:[EBP-0x6C],EAX
$+CC     091AD839        8BD0                    MOV EDX,EAX
$+CE     091AD83B        8B45 9C                 MOV EAX,DWORD PTR SS:[EBP-0x64]
$+D1     091AD83E        8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
$+D4     091AD841        8955 98                 MOV DWORD PTR SS:[EBP-0x68],EDX
$+D7     091AD844        8945 9C                 MOV DWORD PTR SS:[EBP-0x64],EAX
$+DA     091AD847        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+DD     091AD84A        83EC 04                 SUB ESP,0x4
$+E0     091AD84D        51                      PUSH ECX
$+E1     091AD84E        6A 01                   PUSH 0x1
$+E3     091AD850        53                      PUSH EBX
$+E4     091AD851        FFD0                    CALL EAX
$+E6     091AD853        83C4 10                 ADD ESP,0x10
$+E9     091AD856        8B55 A0                 MOV EDX,DWORD PTR SS:[EBP-0x60]
$+EC     091AD859        8B45 94                 MOV EAX,DWORD PTR SS:[EBP-0x6C]
$+EF     091AD85C        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+F2     091AD85F        8BFF                    MOV EDI,EDI
$+F4     091AD861        8B4B 58                 MOV ECX,DWORD PTR DS:[EBX+0x58]
$+F7     091AD864        8B5D BC                 MOV EBX,DWORD PTR SS:[EBP-0x44]
$+FA     091AD867        8955 98                 MOV DWORD PTR SS:[EBP-0x68],EDX
$+FD     091AD86A        8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
$+100    091AD86D        8945 9C                 MOV DWORD PTR SS:[EBP-0x64],EAX
$+103    091AD870        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+106    091AD873        83EC 04                 SUB ESP,0x4
$+109    091AD876        52                      PUSH EDX
$+10A    091AD877        6A 01                   PUSH 0x1
$+10C    091AD879        51                      PUSH ECX
$+10D    091AD87A        FFD0                    CALL EAX
$+10F    091AD87C        83C4 10                 ADD ESP,0x10
$+112    091AD87F        B8 04000000             MOV EAX,0x4
$+117    091AD884        8B4D B0                 MOV ECX,DWORD PTR SS:[EBP-0x50]
$+11A    091AD887        890D 50B05108           MOV DWORD PTR DS:[0x851B050],ECX
$+120    091AD88D        8BE5                    MOV ESP,EBP
$+122    091AD88F        5D                      POP EBP
$+123    091AD890        C3                      RETN
$+124    091AD891        8B42 7C                 MOV EAX,DWORD PTR DS:[EDX+0x7C]
$+127    091AD894        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+12A    091AD897        8B8B 10010000           MOV ECX,DWORD PTR DS:[EBX+0x110]
$+130    091AD89D        8B5D AC                 MOV EBX,DWORD PTR SS:[EBP-0x54]
$+133    091AD8A0        8955 98                 MOV DWORD PTR SS:[EBP-0x68],EDX
$+136    091AD8A3        8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
$+139    091AD8A6        8945 9C                 MOV DWORD PTR SS:[EBP-0x64],EAX
$+13C    091AD8A9        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+13F    091AD8AC        83EC 04                 SUB ESP,0x4
$+142    091AD8AF        52                      PUSH EDX
$+143    091AD8B0        6A 01                   PUSH 0x1
$+145    091AD8B2        51                      PUSH ECX
$+146    091AD8B3        FFD0                    CALL EAX
$+148    091AD8B5        83C4 10                 ADD ESP,0x10
$+14B    091AD8B8        8B55 A0                 MOV EDX,DWORD PTR SS:[EBP-0x60]
$+14E    091AD8BB        85C0                    TEST EAX,EAX
$+150    091AD8BD        74 30                   JE SHORT 091AD8EF
$+152    091AD8BF        85C0                    TEST EAX,EAX
$+154    091AD8C1        0F84 77020000           JE 091ADB3E
$+15A    091AD8C7        8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
$+15D    091AD8CA        8B8A 40010000           MOV ECX,DWORD PTR DS:[EDX+0x140]
$+163    091AD8D0        8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
$+166    091AD8D3        8945 98                 MOV DWORD PTR SS:[EBP-0x68],EAX
$+169    091AD8D6        C745 9C 00000000        MOV DWORD PTR SS:[EBP-0x64],0x0
$+170    091AD8DD        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+173    091AD8E0        83EC 04                 SUB ESP,0x4
$+176    091AD8E3        52                      PUSH EDX
$+177    091AD8E4        6A 01                   PUSH 0x1
$+179    091AD8E6        51                      PUSH ECX
$+17A    091AD8E7        FFD0                    CALL EAX
$+17C    091AD8E9        83C4 10                 ADD ESP,0x10
$+17F    091AD8EC        8B55 A0                 MOV EDX,DWORD PTR SS:[EBP-0x60]
$+182    091AD8EF        897A 7C                 MOV DWORD PTR DS:[EDX+0x7C],EDI
$+185    091AD8F2        8B42 7C                 MOV EAX,DWORD PTR DS:[EDX+0x7C]
$+188    091AD8F5        8B7A 08                 MOV EDI,DWORD PTR DS:[EDX+0x8]
$+18B    091AD8F8        8B8F 10010000           MOV ECX,DWORD PTR DS:[EDI+0x110]
$+191    091AD8FE        8B7D A4                 MOV EDI,DWORD PTR SS:[EBP-0x5C]
$+194    091AD901        8955 98                 MOV DWORD PTR SS:[EBP-0x68],EDX
$+197    091AD904        8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
$+19A    091AD907        8945 9C                 MOV DWORD PTR SS:[EBP-0x64],EAX
$+19D    091AD90A        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+1A0    091AD90D        83EC 04                 SUB ESP,0x4
$+1A3    091AD910        52                      PUSH EDX
$+1A4    091AD911        6A 01                   PUSH 0x1
$+1A6    091AD913        51                      PUSH ECX
$+1A7    091AD914        FFD0                    CALL EAX
$+1A9    091AD916        83C4 10                 ADD ESP,0x10
$+1AC    091AD919        8945 A4                 MOV DWORD PTR SS:[EBP-0x5C],EAX
$+1AF    091AD91C        8BC8                    MOV ECX,EAX
$+1B1    091AD91E        8B45 A0                 MOV EAX,DWORD PTR SS:[EBP-0x60]
$+1B4    091AD921        85C9                    TEST ECX,ECX
$+1B6    091AD923        0F84 64010000           JE 091ADA8D
$+1BC    091AD929        8B45 A4                 MOV EAX,DWORD PTR SS:[EBP-0x5C]
$+1BF    091AD92C        85C0                    TEST EAX,EAX
$+1C1    091AD92E        0F84 0A020000           JE 091ADB3E
$+1C7    091AD934        8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
$+1CA    091AD937        8B8A 40010000           MOV ECX,DWORD PTR DS:[EDX+0x140]
$+1D0    091AD93D        8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
$+1D3    091AD940        8945 98                 MOV DWORD PTR SS:[EBP-0x68],EAX
$+1D6    091AD943        C745 9C 01000000        MOV DWORD PTR SS:[EBP-0x64],0x1
$+1DD    091AD94A        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+1E0    091AD94D        83EC 04                 SUB ESP,0x4
$+1E3    091AD950        52                      PUSH EDX
$+1E4    091AD951        6A 01                   PUSH 0x1
$+1E6    091AD953        51                      PUSH ECX
$+1E7    091AD954        FFD0                    CALL EAX
$+1E9    091AD956        83C4 10                 ADD ESP,0x10
$+1EC    091AD959        8B55 A0                 MOV EDX,DWORD PTR SS:[EBP-0x60]
$+1EF    091AD95C        8B4D A4                 MOV ECX,DWORD PTR SS:[EBP-0x5C]
$+1F2    091AD95F        8B82 8C000000           MOV EAX,DWORD PTR DS:[EDX+0x8C]
$+1F8    091AD965        85C0                    TEST EAX,EAX
$+1FA    091AD967        0F94D2                  SETE DL
$+1FD    091AD96A        0FB6D2                  MOVZX EDX,DL
$+200    091AD96D        85D2                    TEST EDX,EDX
$+202    091AD96F        0F94D2                  SETE DL
$+205    091AD972        0FB6D2                  MOVZX EDX,DL
$+208    091AD975        8955 E0                 MOV DWORD PTR SS:[EBP-0x20],EDX
$+20B    091AD978        85C0                    TEST EAX,EAX
$+20D    091AD97A        0F84 B0000000           JE 091ADA30
$+213    091AD980        8B75 A0                 MOV ESI,DWORD PTR SS:[EBP-0x60]
$+216    091AD983        8B51 20                 MOV EDX,DWORD PTR DS:[ECX+0x20]
$+219    091AD986        8B8E 80000000           MOV ECX,DWORD PTR DS:[ESI+0x80]
$+21F    091AD98C        8B75 A8                 MOV ESI,DWORD PTR SS:[EBP-0x58]
$+222    091AD98F        66:0F57C9               XORPS XMM1,XMM1
$+226    091AD993        F20F2ACA                CVTSI2SD XMM1,EDX
$+22A    091AD997        66:0F57C0               XORPS XMM0,XMM0
$+22E    091AD99B        F20F2AC1                CVTSI2SD XMM0,ECX
$+232    091AD99F        F20F59C8                MULSD XMM1,XMM0
$+236    091AD9A3        660FD64D 90             MOVQ QWORD PTR SS:[EBP-0x70],XMM1
$+23B    091AD9A8        85C0                    TEST EAX,EAX
$+23D    091AD9AA        0F84 8E010000           JE 091ADB3E
$+243    091AD9B0        8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
$+246    091AD9B3        8B8A 24010000           MOV ECX,DWORD PTR DS:[EDX+0x124]
$+24C    091AD9B9        8D55 A8                 LEA EDX,DWORD PTR SS:[EBP-0x58]
$+24F    091AD9BC        8945 A8                 MOV DWORD PTR SS:[EBP-0x58],EAX
$+252    091AD9BF        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
*/
//tag_左键双击快速登陆项
/*
$-8      091CE12F        00E4                    ADD AH,AH
$-6      091CE131        E2 1C                   LOOPD SHORT 091CE14F
$-4      091CE133        0900                    OR DWORD PTR DS:[EAX],EAX
$-2      091CE135        0000                    ADD BYTE PTR DS:[EAX],AL
$ ==>    091CE137        55                      PUSH EBP                                 ; tag_左键双击快速登陆项
$+1      091CE138        8BEC                    MOV EBP,ESP
$+3      091CE13A        83EC 28                 SUB ESP,0x28
$+6      091CE13D        895D FC                 MOV DWORD PTR SS:[EBP-0x4],EBX
$+9      091CE140        8975 EC                 MOV DWORD PTR SS:[EBP-0x14],ESI
$+C      091CE143        897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$+F      091CE146        8B7D 08                 MOV EDI,DWORD PTR SS:[EBP+0x8]
$+12     091CE149        8B5D 10                 MOV EBX,DWORD PTR SS:[EBP+0x10]
$+15     091CE14C        8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
$+18     091CE14F        8B0D 50B05108           MOV ECX,DWORD PTR DS:[0x851B050]
$+1E     091CE155        897D F4                 MOV DWORD PTR SS:[EBP-0xC],EDI
$+21     091CE158        894D F0                 MOV DWORD PTR SS:[EBP-0x10],ECX
$+24     091CE15B        8D09                    LEA ECX,DWORD PTR DS:[ECX]
$+26     091CE15D        8905 50B05108           MOV DWORD PTR DS:[0x851B050],EAX
$+2C     091CE163        8B0D 40B05108           MOV ECX,DWORD PTR DS:[0x851B040]
$+32     091CE169        3BC1                    CMP EAX,ECX
$+34     091CE16B        73 07                   JNB SHORT 091CE174                       ; 跳转
$+36     091CE16D        8BCF                    MOV ECX,EDI
$+38     091CE16F        E8 7C5E51FE             CALL 076E3FF0
$+3D     091CE174        8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+3F     091CE176        8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
$+42     091CE179        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+45     091CE17C        8B5F 0C                 MOV EBX,DWORD PTR DS:[EDI+0xC]
$+48     091CE17F        8B7B 08                 MOV EDI,DWORD PTR DS:[EBX+0x8]
$+4B     091CE182        8B5F 14                 MOV EBX,DWORD PTR DS:[EDI+0x14]
$+4E     091CE185        85C0                    TEST EAX,EAX
$+50     091CE187        0F84 2C010000           JE 091CE2B9
$+56     091CE18D        8B48 08                 MOV ECX,DWORD PTR DS:[EAX+0x8]
$+59     091CE190        8B79 70                 MOV EDI,DWORD PTR DS:[ECX+0x70]
$+5C     091CE193        8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
$+5F     091CE196        8945 DC                 MOV DWORD PTR SS:[EBP-0x24],EAX
$+62     091CE199        8B47 04                 MOV EAX,DWORD PTR DS:[EDI+0x4]
$+65     091CE19C        83EC 04                 SUB ESP,0x4
$+68     091CE19F        51                      PUSH ECX
$+69     091CE1A0        6A 00                   PUSH 0x0
$+6B     091CE1A2        57                      PUSH EDI
$+6C     091CE1A3        FFD0                    CALL EAX
$+6E     091CE1A5        83C4 10                 ADD ESP,0x10
$+71     091CE1A8        8945 E4                 MOV DWORD PTR SS:[EBP-0x1C],EAX
$+74     091CE1AB        8D8B B8000000           LEA ECX,DWORD PTR DS:[EBX+0xB8]
$+7A     091CE1B1        8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
$+7D     091CE1B4        E8 477A55FE             CALL 07725C00
$+82     091CE1B9        8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
$+85     091CE1BC        8BF8                    MOV EDI,EAX
$+87     091CE1BE        8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+8A     091CE1C1        8B57 10                 MOV EDX,DWORD PTR DS:[EDI+0x10]
$+8D     091CE1C4        8B7D E8                 MOV EDI,DWORD PTR SS:[EBP-0x18]
$+90     091CE1C7        8D52 01                 LEA EDX,DWORD PTR DS:[EDX+0x1]
$+93     091CE1CA        83EC 04                 SUB ESP,0x4
$+96     091CE1CD        52                      PUSH EDX
$+97     091CE1CE        51                      PUSH ECX
$+98     091CE1CF        50                      PUSH EAX
$+99     091CE1D0        E8 EB8655FE             CALL 077268C0
$+9E     091CE1D5        83C4 10                 ADD ESP,0x10
$+A1     091CE1D8        8B55 E0                 MOV EDX,DWORD PTR SS:[EBP-0x20]
$+A4     091CE1DB        83E0 F8                 AND EAX,0xFFFFFFF8
$+A7     091CE1DE        8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+AA     091CE1E1        85C0                    TEST EAX,EAX
$+AC     091CE1E3        74 32                   JE SHORT 091CE217
$+AE     091CE1E5        8B75 E8                 MOV ESI,DWORD PTR SS:[EBP-0x18]
$+B1     091CE1E8        85F6                    TEST ESI,ESI
$+B3     091CE1EA        0F84 C9000000           JE 091CE2B9
$+B9     091CE1F0        8B46 20                 MOV EAX,DWORD PTR DS:[ESI+0x20]
$+BC     091CE1F3        8B72 08                 MOV ESI,DWORD PTR DS:[EDX+0x8]
$+BF     091CE1F6        8B8E 1C010000           MOV ECX,DWORD PTR DS:[ESI+0x11C]
$+C5     091CE1FC        8B75 EC                 MOV ESI,DWORD PTR SS:[EBP-0x14]
$+C8     091CE1FF        8955 E8                 MOV DWORD PTR SS:[EBP-0x18],EDX
$+CB     091CE202        8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
$+CE     091CE205        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+D1     091CE208        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+D4     091CE20B        83EC 04                 SUB ESP,0x4
$+D7     091CE20E        52                      PUSH EDX
$+D8     091CE20F        6A 01                   PUSH 0x1
$+DA     091CE211        51                      PUSH ECX
$+DB     091CE212        FFD0                    CALL EAX                                 ; call tag_左键点击快速登陆项的时候会断到
$+DD     091CE214        83C4 10                 ADD ESP,0x10
$+E0     091CE217        8D5B 14                 LEA EBX,DWORD PTR DS:[EBX+0x14]
$+E3     091CE21A        895D E8                 MOV DWORD PTR SS:[EBP-0x18],EBX
$+E6     091CE21D        8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
$+E9     091CE220        8B4D E8                 MOV ECX,DWORD PTR SS:[EBP-0x18]
$+EC     091CE223        E8 D87955FE             CALL 07725C00                            ; 啥都没干
$+F1     091CE228        8B4D E8                 MOV ECX,DWORD PTR SS:[EBP-0x18]
$+F4     091CE22B        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+F7     091CE22E        8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
$+FA     091CE231        E8 CA7955FE             CALL 07725C00                            ; 啥都没干
$+FF     091CE236        8BD8                    MOV EBX,EAX
$+101    091CE238        8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+104    091CE23B        8B4B 10                 MOV ECX,DWORD PTR DS:[EBX+0x10]
$+107    091CE23E        85C9                    TEST ECX,ECX
$+109    091CE240        74 77                   JE SHORT 091CE2B9
$+10B    091CE242        8B99 88000000           MOV EBX,DWORD PTR DS:[ECX+0x88]
$+111    091CE248        895D EC                 MOV DWORD PTR SS:[EBP-0x14],EBX
$+114    091CE24B        8B48 10                 MOV ECX,DWORD PTR DS:[EAX+0x10]
$+117    091CE24E        85C9                    TEST ECX,ECX
$+119    091CE250        74 67                   JE SHORT 091CE2B9
$+11B    091CE252        8B59 08                 MOV EBX,DWORD PTR DS:[ECX+0x8]
$+11E    091CE255        8B53 10                 MOV EDX,DWORD PTR DS:[EBX+0x10]
$+121    091CE258        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+124    091CE25B        8B42 18                 MOV EAX,DWORD PTR DS:[EDX+0x18]
$+127    091CE25E        FFD0                    CALL EAX
$+129    091CE260        8945 E4                 MOV DWORD PTR SS:[EBP-0x1C],EAX
$+12C    091CE263        8BD0                    MOV EDX,EAX
$+12E    091CE265        8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+131    091CE268        8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
$+134    091CE26B        8955 E8                 MOV DWORD PTR SS:[EBP-0x18],EDX
$+137    091CE26E        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+13A    091CE271        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+13D    091CE274        83EC 04                 SUB ESP,0x4
$+140    091CE277        51                      PUSH ECX
$+141    091CE278        6A 01                   PUSH 0x1
$+143    091CE27A        53                      PUSH EBX
$+144    091CE27B        FFD0                    CALL EAX
$+146    091CE27D        83C4 10                 ADD ESP,0x10
$+149    091CE280        8B55 E0                 MOV EDX,DWORD PTR SS:[EBP-0x20]
$+14C    091CE283        8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+14F    091CE286        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+152    091CE289        8B4B 58                 MOV ECX,DWORD PTR DS:[EBX+0x58]
$+155    091CE28C        8B5D FC                 MOV EBX,DWORD PTR SS:[EBP-0x4]
$+158    091CE28F        8955 E8                 MOV DWORD PTR SS:[EBP-0x18],EDX
$+15B    091CE292        8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
$+15E    091CE295        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+161    091CE298        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+164    091CE29B        83EC 04                 SUB ESP,0x4
$+167    091CE29E        52                      PUSH EDX
$+168    091CE29F        6A 01                   PUSH 0x1
$+16A    091CE2A1        51                      PUSH ECX
$+16B    091CE2A2        FFD0                    CALL EAX                                 ; call tag_鼠标移动到快速登陆项的时候断到
$+16D    091CE2A4        83C4 10                 ADD ESP,0x10
$+170    091CE2A7        B8 04000000             MOV EAX,0x4
$+175    091CE2AC        8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$+178    091CE2AF        890D 50B05108           MOV DWORD PTR DS:[0x851B050],ECX
$+17E    091CE2B5        8BE5                    MOV ESP,EBP
$+180    091CE2B7        5D                      POP EBP
$+181    091CE2B8        C3                      RETN
$+182    091CE2B9        8B5D 08                 MOV EBX,DWORD PTR SS:[EBP+0x8]
$+185    091CE2BC        83EC 0C                 SUB ESP,0xC
$+188    091CE2BF        53                      PUSH EBX
$+189    091CE2C0        E8 BBEA56FE             CALL 0773CD80
$+18E    091CE2C5        83C4 10                 ADD ESP,0x10
$+191    091CE2C8        CC                      INT3
*/
/**/
//tag_鼠标移动到快速登陆项的时候断到
/*
$-1D     0912E5F3        53                      PUSH EBX
$-1C     0912E5F4        E8 F71560FE             CALL 0772FBF0
$-17     0912E5F9        83C4 0C                 ADD ESP,0xC
$-14     0912E5FC        0000                    ADD BYTE PTR DS:[EAX],AL
$-12     0912E5FE        0000                    ADD BYTE PTR DS:[EAX],AL
$-10     0912E600        84E5                    TEST CH,AH
$-E      0912E602        1209                    ADC CL,BYTE PTR DS:[ECX]
$-C      0912E604        EC                      IN AL,DX
$-B      0912E605        4F                      DEC EDI
$-A      0912E606        1309                    ADC ECX,DWORD PTR DS:[ECX]
$-8      0912E608        0000                    ADD BYTE PTR DS:[EAX],AL
$-6      0912E60A        0000                    ADD BYTE PTR DS:[EAX],AL
$-4      0912E60C        24 E7                   AND AL,0xE7
$-2      0912E60E        1209                    ADC CL,BYTE PTR DS:[ECX]
$ ==>    0912E610        55                      PUSH EBP                                 ; tag_鼠标移动到快速登陆项的时候断到
$+1      0912E611        8BEC                    MOV EBP,ESP
$+3      0912E613        83EC 28                 SUB ESP,0x28
$+6      0912E616        895D FC                 MOV DWORD PTR SS:[EBP-0x4],EBX
$+9      0912E619        8B4D 08                 MOV ECX,DWORD PTR SS:[EBP+0x8]
$+C      0912E61C        8B5D 10                 MOV EBX,DWORD PTR SS:[EBP+0x10]
$+F      0912E61F        8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
$+12     0912E622        8B15 50B05108           MOV EDX,DWORD PTR DS:[0x851B050]
$+18     0912E628        894D F4                 MOV DWORD PTR SS:[EBP-0xC],ECX
$+1B     0912E62B        8955 F0                 MOV DWORD PTR SS:[EBP-0x10],EDX
$+1E     0912E62E        8905 50B05108           MOV DWORD PTR DS:[0x851B050],EAX
$+24     0912E634        8B15 40B05108           MOV EDX,DWORD PTR DS:[0x851B040]
$+2A     0912E63A        3BC2                    CMP EAX,EDX
$+2C     0912E63C        73 05                   JNB SHORT 0912E643
$+2E     0912E63E        E8 AD595BFE             CALL 076E3FF0
$+33     0912E643        8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+35     0912E645        8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$+38     0912E648        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+3B     0912E64B        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+3E     0912E64E        85C0                    TEST EAX,EAX
$+40     0912E650        0F84 B0000000           JE 0912E706
$+46     0912E656        8B48 08                 MOV ECX,DWORD PTR DS:[EAX+0x8]
$+49     0912E659        894D E4                 MOV DWORD PTR SS:[EBP-0x1C],ECX
$+4C     0912E65C        8B59 70                 MOV EBX,DWORD PTR DS:[ECX+0x70]
$+4F     0912E65F        8D4D E0                 LEA ECX,DWORD PTR SS:[EBP-0x20]
$+52     0912E662        8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
$+55     0912E665        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+58     0912E668        83EC 04                 SUB ESP,0x4
$+5B     0912E66B        51                      PUSH ECX
$+5C     0912E66C        6A 00                   PUSH 0x0
$+5E     0912E66E        53                      PUSH EBX
$+5F     0912E66F        FFD0                    CALL EAX
$+61     0912E671        83C4 10                 ADD ESP,0x10
$+64     0912E674        83EC 0C                 SUB ESP,0xC
$+67     0912E677        50                      PUSH EAX
$+68     0912E678        E8 23D05AFE             CALL 076DB6A0
$+6D     0912E67D        83C4 10                 ADD ESP,0x10
$+70     0912E680        8B55 E8                 MOV EDX,DWORD PTR SS:[EBP-0x18]
$+73     0912E683        8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
$+76     0912E686        8BD8                    MOV EBX,EAX
$+78     0912E688        8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
$+7B     0912E68B        85DB                    TEST EBX,EBX
$+7D     0912E68D        74 49                   JE SHORT 0912E6D8
$+7F     0912E68F        8B59 50                 MOV EBX,DWORD PTR DS:[ECX+0x50]
$+82     0912E692        8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
$+85     0912E695        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+88     0912E698        8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+8B     0912E69B        83EC 04                 SUB ESP,0x4
$+8E     0912E69E        51                      PUSH ECX
$+8F     0912E69F        6A 00                   PUSH 0x0
$+91     0912E6A1        53                      PUSH EBX
$+92     0912E6A2        FFD0                    CALL EAX
$+94     0912E6A4        83C4 10                 ADD ESP,0x10
$+97     0912E6A7        8B55 E8                 MOV EDX,DWORD PTR SS:[EBP-0x18]
$+9A     0912E6AA        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+9D     0912E6AD        8B4B 64                 MOV ECX,DWORD PTR DS:[EBX+0x64]
$+A0     0912E6B0        8B5D FC                 MOV EBX,DWORD PTR SS:[EBP-0x4]
$+A3     0912E6B3        8955 E8                 MOV DWORD PTR SS:[EBP-0x18],EDX
$+A6     0912E6B6        8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
$+A9     0912E6B9        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+AC     0912E6BC        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+AF     0912E6BF        83EC 04                 SUB ESP,0x4
$+B2     0912E6C2        52                      PUSH EDX
$+B3     0912E6C3        6A 01                   PUSH 0x1
$+B5     0912E6C5        51                      PUSH ECX
$+B6     0912E6C6        FFD0                    CALL EAX
$+B8     0912E6C8        83C4 10                 ADD ESP,0x10
$+BB     0912E6CB        8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$+BE     0912E6CE        890D 50B05108           MOV DWORD PTR DS:[0x851B050],ECX
$+C4     0912E6D4        8BE5                    MOV ESP,EBP
$+C6     0912E6D6        5D                      POP EBP
$+C7     0912E6D7        C3                      RETN
$+C8     0912E6D8        8B5A 08                 MOV EBX,DWORD PTR DS:[EDX+0x8]
$+CB     0912E6DB        8B4B 64                 MOV ECX,DWORD PTR DS:[EBX+0x64]
$+CE     0912E6DE        8B5D FC                 MOV EBX,DWORD PTR SS:[EBP-0x4]
$+D1     0912E6E1        8955 E8                 MOV DWORD PTR SS:[EBP-0x18],EDX
$+D4     0912E6E4        8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
$+D7     0912E6E7        8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+DA     0912E6EA        8B41 04                 MOV EAX,DWORD PTR DS:[ECX+0x4]
$+DD     0912E6ED        83EC 04                 SUB ESP,0x4
$+E0     0912E6F0        52                      PUSH EDX
$+E1     0912E6F1        6A 01                   PUSH 0x1
$+E3     0912E6F3        51                      PUSH ECX
$+E4     0912E6F4        FFD0                    CALL EAX
$+E6     0912E6F6        83C4 10                 ADD ESP,0x10
$+E9     0912E6F9        8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$+EC     0912E6FC        890D 50B05108           MOV DWORD PTR DS:[0x851B050],ECX
$+F2     0912E702        8BE5                    MOV ESP,EBP
$+F4     0912E704        5D                      POP EBP
$+F5     0912E705        C3                      RETN
$+F6     0912E706        8B5D 08                 MOV EBX,DWORD PTR SS:[EBP+0x8]
$+F9     0912E709        83EC 0C                 SUB ESP,0xC
$+FC     0912E70C        53                      PUSH EBX
$+FD     0912E70D        E8 6EE660FE             CALL 0773CD80
$+102    0912E712        83C4 10                 ADD ESP,0x10
$+105    0912E715        CC                      INT3
*/
