#pragma once
/*
@author zhp
@date 2017/2/13 0:26
@purpose for skill asm record
*/

//tag_点击天赋界面中的天赋会断下
/*
$ ==>    00524510     55                        PUSH EBP                                          ; 点击天赋界面中的天赋会断下
$+1      00524511     8BEC                      MOV EBP, ESP
$+3      00524513     6A FF                     PUSH -0x1
$+5      00524515     68 0593D200               PUSH PathOfEx.00D29305
$+A      0052451A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
$+10     00524520     50                        PUSH EAX
$+11     00524521     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
$+18     00524528     81EC D0000000             SUB ESP, 0xD0
$+1E     0052452E     56                        PUSH ESI
$+1F     0052452F     8BF1                      MOV ESI, ECX
$+21     00524531     C745 E4 00000000          MOV DWORD PTR SS:[EBP-0x1C], 0x0
$+28     00524538     8B8E 080C0000             MOV ECX, DWORD PTR DS:[ESI+0xC08]                 ; PathOfEx.00EA0AE0
$+2E     0052453E     8975 F0                   MOV DWORD PTR SS:[EBP-0x10], ESI
$+31     00524541     57                        PUSH EDI
$+32     00524542     8DBE 080C0000             LEA EDI, DWORD PTR DS:[ESI+0xC08]
$+38     00524548     897D C8                   MOV DWORD PTR SS:[EBP-0x38], EDI
$+3B     0052454B     85C9                      TEST ECX, ECX
$+3D     0052454D     74 0B                     JE SHORT PathOfEx.0052455A
$+3F     0052454F     FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
$+42     00524552     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
$+45     00524555     E8 B6FFFFFF               CALL PathOfEx.00524510
$+4A     0052455A     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]                   ; 天赋遍历地址
$+4D     0052455D     398E 60090000             CMP DWORD PTR DS:[ESI+0x960], ECX
$+53     00524563     74 71                     JE SHORT PathOfEx.005245D6
$+55     00524565     85C9                      TEST ECX, ECX
$+57     00524567     74 4D                     JE SHORT PathOfEx.005245B6
$+59     00524569     8D45 A4                   LEA EAX, DWORD PTR SS:[EBP-0x5C]
$+5C     0052456C     50                        PUSH EAX
$+5D     0052456D     E8 3E3F2E00               CALL PathOfEx.008084B0                            ; tag_后悔点数运算
$+62     00524572     50                        PUSH EAX
$+63     00524573     8D8E 60090000             LEA ECX, DWORD PTR DS:[ESI+0x960]
$+69     00524579     E8 1270F4FF               CALL PathOfEx.0046B590
$+6E     0052457E     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
$+75     00524585     8B4D A8                   MOV ECX, DWORD PTR SS:[EBP-0x58]                  ; PathOfEx.005D554E
$+78     00524588     894D E0                   MOV DWORD PTR SS:[EBP-0x20], ECX
$+7B     0052458B     85C9                      TEST ECX, ECX
$+7D     0052458D     74 20                     JE SHORT PathOfEx.005245AF
$+7F     0052458F     83C8 FF                   OR EAX, -0x1
$+82     00524592     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
$+87     00524597     75 16                     JNZ SHORT PathOfEx.005245AF
$+89     00524599     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+8B     0052459B     FF10                      CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00EEC4CC
$+8D     0052459D     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
$+90     005245A0     83C8 FF                   OR EAX, -0x1
$+93     005245A3     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$+98     005245A8     75 05                     JNZ SHORT PathOfEx.005245AF
$+9A     005245AA     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+9C     005245AC     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+9F     005245AF     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+A6     005245B6     8B86 68090000             MOV EAX, DWORD PTR DS:[ESI+0x968]
$+AC     005245BC     3B86 6C090000             CMP EAX, DWORD PTR DS:[ESI+0x96C]
$+B2     005245C2     74 12                     JE SHORT PathOfEx.005245D6
$+B4     005245C4     8BCE                      MOV ECX, ESI
$+B6     005245C6     E8 252E0000               CALL PathOfEx.005273F0
$+BB     005245CB     8D8E B8080000             LEA ECX, DWORD PTR DS:[ESI+0x8B8]
$+C1     005245D1     E8 DAEFF8FF               CALL PathOfEx.004B35B0
$+C6     005245D6     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+C9     005245D9     8B42 04                   MOV EAX, DWORD PTR DS:[EDX+0x4]
$+CC     005245DC     85C0                      TEST EAX, EAX
$+CE     005245DE     74 30                     JE SHORT PathOfEx.00524610
$+D0     005245E0     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+D3     005245E3     8D8E 700B0000             LEA ECX, DWORD PTR DS:[ESI+0xB70]
$+D9     005245E9     8945 E0                   MOV DWORD PTR SS:[EBP-0x20], EAX
$+DC     005245EC     8D45 E0                   LEA EAX, DWORD PTR SS:[EBP-0x20]
$+DF     005245EF     50                        PUSH EAX
$+E0     005245F0     8D45 C0                   LEA EAX, DWORD PTR SS:[EBP-0x40]
$+E3     005245F3     50                        PUSH EAX
$+E4     005245F4     E8 077AF4FF               CALL PathOfEx.0046C000
$+E9     005245F9     8B86 740B0000             MOV EAX, DWORD PTR DS:[ESI+0xB74]
$+EF     005245FF     3945 C0                   CMP DWORD PTR SS:[EBP-0x40], EAX
$+F2     00524602     0F84 0E080000             JE PathOfEx.00524E16
$+F8     00524608     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+FB     0052460B     E9 DE000000               JMP PathOfEx.005246EE
$+100    00524610     8BBE 680B0000             MOV EDI, DWORD PTR DS:[ESI+0xB68]                 ; PathOfEx.00F2FCD8
$+106    00524616     8B3F                      MOV EDI, DWORD PTR DS:[EDI]                       ; PathOfEx.00E7DE64
$+108    00524618     897D D4                   MOV DWORD PTR SS:[EBP-0x2C], EDI
$+10B    0052461B     3BBE 680B0000             CMP EDI, DWORD PTR DS:[ESI+0xB68]                 ; PathOfEx.00F2FCD8
$+111    00524621     0F84 C4000000             JE PathOfEx.005246EB
$+117    00524627     66:0F1F8400 00000000      NOP WORD PTR DS:[EAX+EAX]
$+120    00524630     8B47 10                   MOV EAX, DWORD PTR DS:[EDI+0x10]
$+123    00524633     8B4F 18                   MOV ECX, DWORD PTR DS:[EDI+0x18]
$+126    00524636     F3:0F1010                 MOVSS XMM2, DWORD PTR DS:[EAX]
$+12A    0052463A     F3:0F1081 60070000        MOVSS XMM0, DWORD PTR DS:[ECX+0x760]
$+132    00524642     F3:0F1048 04              MOVSS XMM1, DWORD PTR DS:[EAX+0x4]
$+137    00524647     0F2EC2                    UCOMISS XMM0, XMM2
$+13A    0052464A     9F                        LAHF
$+13B    0052464B     F6C4 44                   TEST AH, 0x44
$+13E    0052464E     7A 11                     JPE SHORT PathOfEx.00524661
$+140    00524650     F3:0F1081 64070000        MOVSS XMM0, DWORD PTR DS:[ECX+0x764]
$+148    00524658     0F2EC1                    UCOMISS XMM0, XMM1
$+14B    0052465B     9F                        LAHF
$+14C    0052465C     F6C4 44                   TEST AH, 0x44
$+14F    0052465F     7B 24                     JPO SHORT PathOfEx.00524685
$+151    00524661     83B9 68070000 00          CMP DWORD PTR DS:[ECX+0x768], 0x0
$+158    00524668     75 08                     JNZ SHORT PathOfEx.00524672
$+15A    0052466A     F3:0F1191 60070000        MOVSS DWORD PTR DS:[ECX+0x760], XMM2
$+162    00524672     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+164    00524674     F3:0F1189 64070000        MOVSS DWORD PTR DS:[ECX+0x764], XMM1
$+16C    0052467C     FF90 08010000             CALL NEAR DWORD PTR DS:[EAX+0x108]
$+172    00524682     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+175    00524685     8B77 18                   MOV ESI, DWORD PTR DS:[EDI+0x18]
$+178    00524688     8B7F 18                   MOV EDI, DWORD PTR DS:[EDI+0x18]
$+17B    0052468B     8BB6 08070000             MOV ESI, DWORD PTR DS:[ESI+0x708]
$+181    00524691     8BBF 0C070000             MOV EDI, DWORD PTR DS:[EDI+0x70C]
$+187    00524697     3BF7                      CMP ESI, EDI
$+189    00524699     74 33                     JE SHORT PathOfEx.005246CE
$+18B    0052469B     8B4A 04                   MOV ECX, DWORD PTR DS:[EDX+0x4]
$+18E    0052469E     66:90                     NOP
$+190    005246A0     8B16                      MOV EDX, DWORD PTR DS:[ESI]
$+192    005246A2     85C9                      TEST ECX, ECX
$+194    005246A4     74 0B                     JE SHORT PathOfEx.005246B1
$+196    005246A6     8B82 B0080000             MOV EAX, DWORD PTR DS:[EDX+0x8B0]
$+19C    005246AC     3B48 08                   CMP ECX, DWORD PTR DS:[EAX+0x8]
$+19F    005246AF     75 16                     JNZ SHORT PathOfEx.005246C7
$+1A1    005246B1     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
$+1A4    005246B4     52                        PUSH EDX
$+1A5    005246B5     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]                       ; 传入天赋地址
$+1A8    005246B8     E8 93F9FFFF               CALL PathOfEx.00524050                            ; tag_计算天赋剩余点数的上层函数
$+1AD    005246BD     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+1B0    005246C0     8B48 04                   MOV ECX, DWORD PTR DS:[EAX+0x4]
$+1B3    005246C3     85C9                      TEST ECX, ECX
$+1B5    005246C5     75 07                     JNZ SHORT PathOfEx.005246CE
$+1B7    005246C7     83C6 04                   ADD ESI, 0x4
$+1BA    005246CA     3BF7                      CMP ESI, EDI
$+1BC    005246CC   ^ 75 D2                     JNZ SHORT PathOfEx.005246A0
$+1BE    005246CE     8D4D D4                   LEA ECX, DWORD PTR SS:[EBP-0x2C]
$+1C1    005246D1     E8 AA07F5FF               CALL PathOfEx.00474E80
$+1C6    005246D6     8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-0x10]
$+1C9    005246D9     8B7D D4                   MOV EDI, DWORD PTR SS:[EBP-0x2C]                  ; nvd3dum.67D07432
$+1CC    005246DC     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+1CF    005246DF     3BBE 680B0000             CMP EDI, DWORD PTR DS:[ESI+0xB68]                 ; PathOfEx.00F2FCD8
$+1D5    005246E5   ^ 0F85 45FFFFFF             JNZ PathOfEx.00524630
$+1DB    005246EB     8B7D C8                   MOV EDI, DWORD PTR SS:[EBP-0x38]                  ; nvd3dum.67CFFE00
$+1DE    005246EE     837A 04 00                CMP DWORD PTR DS:[EDX+0x4], 0x0
$+1E2    005246F2     0F85 1E070000             JNZ PathOfEx.00524E16
$+1E8    005246F8     8BCE                      MOV ECX, ESI
$+1EA    005246FA     E8 C1E6FFFF               CALL PathOfEx.00522DC0
$+1EF    005246FF     8BCE                      MOV ECX, ESI
$+1F1    00524701     E8 3A270000               CALL PathOfEx.00526E40
$+1F6    00524706     8B86 38090000             MOV EAX, DWORD PTR DS:[ESI+0x938]
$+1FC    0052470C     85C0                      TEST EAX, EAX
$+1FE    0052470E     74 0A                     JE SHORT PathOfEx.0052471A
$+200    00524710     05 C43A0000               ADD EAX, 0x3AC4
$+205    00524715     8945 B8                   MOV DWORD PTR SS:[EBP-0x48], EAX
$+208    00524718     EB 07                     JMP SHORT PathOfEx.00524721
$+20A    0052471A     C745 B8 00000000          MOV DWORD PTR SS:[EBP-0x48], 0x0
$+211    00524721     8D45 08                   LEA EAX, DWORD PTR SS:[EBP+0x8]
$+214    00524724     8985 7CFFFFFF             MOV DWORD PTR SS:[EBP-0x84], EAX
$+21A    0052472A     8D45 B8                   LEA EAX, DWORD PTR SS:[EBP-0x48]
$+21D    0052472D     8945 80                   MOV DWORD PTR SS:[EBP-0x80], EAX
$+220    00524730     8D86 5C0B0000             LEA EAX, DWORD PTR DS:[ESI+0xB5C]
$+226    00524736     8945 BC                   MOV DWORD PTR SS:[EBP-0x44], EAX
$+229    00524739     8B86 5C0B0000             MOV EAX, DWORD PTR DS:[ESI+0xB5C]
$+22F    0052473F     3B86 600B0000             CMP EAX, DWORD PTR DS:[ESI+0xB60]
$+235    00524745     74 77                     JE SHORT PathOfEx.005247BE
$+237    00524747     33C9                      XOR ECX, ECX
$+239    00524749     894D 0C                   MOV DWORD PTR SS:[EBP+0xC], ECX
$+23C    0052474C     8B8E 600B0000             MOV ECX, DWORD PTR DS:[ESI+0xB60]
$+242    00524752     2BC8                      SUB ECX, EAX
$+244    00524754     B8 67666666               MOV EAX, 0x66666667
$+249    00524759     F7E9                      IMUL ECX
$+24B    0052475B     C1FA 05                   SAR EDX, 0x5
$+24E    0052475E     8BC2                      MOV EAX, EDX
$+250    00524760     C1E8 1F                   SHR EAX, 0x1F
$+253    00524763     03C2                      ADD EAX, EDX
$+255    00524765     0F84 AB060000             JE PathOfEx.00524E16
$+25B    0052476B     33FF                      XOR EDI, EDI
$+25D    0052476D     0F1F00                    NOP DWORD PTR DS:[EAX]
$+260    00524770     8B86 5C0B0000             MOV EAX, DWORD PTR DS:[ESI+0xB5C]
$+266    00524776     8D8D 7CFFFFFF             LEA ECX, DWORD PTR SS:[EBP-0x84]
$+26C    0052477C     03C7                      ADD EAX, EDI
$+26E    0052477E     50                        PUSH EAX
$+26F    0052477F     E8 AC060000               CALL PathOfEx.00524E30
$+274    00524784     8B8E 600B0000             MOV ECX, DWORD PTR DS:[ESI+0xB60]
$+27A    0052478A     B8 67666666               MOV EAX, 0x66666667
$+27F    0052478F     2B8E 5C0B0000             SUB ECX, DWORD PTR DS:[ESI+0xB5C]
$+285    00524795     83C7 50                   ADD EDI, 0x50
$+288    00524798     FF45 0C                   INC DWORD PTR SS:[EBP+0xC]
$+28B    0052479B     F7E9                      IMUL ECX
$+28D    0052479D     C1FA 05                   SAR EDX, 0x5
$+290    005247A0     8BC2                      MOV EAX, EDX
$+292    005247A2     C1E8 1F                   SHR EAX, 0x1F
$+295    005247A5     03C2                      ADD EAX, EDX
$+297    005247A7     3945 0C                   CMP DWORD PTR SS:[EBP+0xC], EAX
$+29A    005247AA   ^ 72 C4                     JB SHORT PathOfEx.00524770
$+29C    005247AC     5F                        POP EDI                                           ; 0018E6FC
$+29D    005247AD     5E                        POP ESI                                           ; 0018E6FC
$+29E    005247AE     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+2A1    005247B1     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+2A8    005247B8     8BE5                      MOV ESP, EBP
$+2AA    005247BA     5D                        POP EBP                                           ; 0018E6FC
$+2AB    005247BB     C2 0800                   RETN 0x8
$+2AE    005247BE     8B8E 600B0000             MOV ECX, DWORD PTR DS:[ESI+0xB60]
$+2B4    005247C4     894D E0                   MOV DWORD PTR SS:[EBP-0x20], ECX
$+2B7    005247C7     8945 0C                   MOV DWORD PTR SS:[EBP+0xC], EAX
$+2BA    005247CA     3BC1                      CMP EAX, ECX
$+2BC    005247CC     74 1B                     JE SHORT PathOfEx.005247E9
$+2BE    005247CE     8BF8                      MOV EDI, EAX
$+2C0    005247D0     8BF1                      MOV ESI, ECX
$+2C2    005247D2     8BCF                      MOV ECX, EDI
$+2C4    005247D4     E8 F7060000               CALL PathOfEx.00524ED0
$+2C9    005247D9     83C7 50                   ADD EDI, 0x50
$+2CC    005247DC     3BFE                      CMP EDI, ESI
$+2CE    005247DE   ^ 75 F2                     JNZ SHORT PathOfEx.005247D2
$+2D0    005247E0     8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-0x10]
$+2D3    005247E3     8DBE 080C0000             LEA EDI, DWORD PTR DS:[ESI+0xC08]
$+2D9    005247E9     8B86 5C0B0000             MOV EAX, DWORD PTR DS:[ESI+0xB5C]
$+2DF    005247EF     8D4D A4                   LEA ECX, DWORD PTR SS:[EBP-0x5C]
$+2E2    005247F2     8986 600B0000             MOV DWORD PTR DS:[ESI+0xB60], EAX
$+2E8    005247F8     8B06                      MOV EAX, DWORD PTR DS:[ESI]
$+2EA    005247FA     51                        PUSH ECX
$+2EB    005247FB     8BCE                      MOV ECX, ESI
$+2ED    005247FD     8B40 44                   MOV EAX, DWORD PTR DS:[EAX+0x44]
$+2F0    00524800     FFD0                      CALL NEAR EAX
$+2F2    00524802     33C9                      XOR ECX, ECX
$+2F4    00524804     894D E0                   MOV DWORD PTR SS:[EBP-0x20], ECX
$+2F7    00524807     F3:0F1008                 MOVSS XMM1, DWORD PTR DS:[EAX]
$+2FB    0052480B     F3:0F590D E4CAF300        MULSS XMM1, DWORD PTR DS:[0xF3CAE4]
$+303    00524813     F3:0F114D B4              MOVSS DWORD PTR SS:[EBP-0x4C], XMM1
$+308    00524818     F3:0F1048 04              MOVSS XMM1, DWORD PTR DS:[EAX+0x4]
$+30D    0052481D     8B86 4C090000             MOV EAX, DWORD PTR DS:[ESI+0x94C]
$+313    00524823     2B86 48090000             SUB EAX, DWORD PTR DS:[ESI+0x948]
$+319    00524829     F3:0F590D E4CAF300        MULSS XMM1, DWORD PTR DS:[0xF3CAE4]
$+321    00524831     C1F8 03                   SAR EAX, 0x3
$+324    00524834     F3:0F114D B0              MOVSS DWORD PTR SS:[EBP-0x50], XMM1
$+329    00524839     85C0                      TEST EAX, EAX
$+32B    0052483B     0F84 D5050000             JE PathOfEx.00524E16
$+331    00524841     8B86 48090000             MOV EAX, DWORD PTR DS:[ESI+0x948]
$+337    00524847     8BB6 3C090000             MOV ESI, DWORD PTR DS:[ESI+0x93C]
$+33D    0052484D     8D04C8                    LEA EAX, DWORD PTR DS:[EAX+ECX*8]
$+340    00524850     50                        PUSH EAX
$+341    00524851     8945 D8                   MOV DWORD PTR SS:[EBP-0x28], EAX
$+344    00524854     8D4E 0C                   LEA ECX, DWORD PTR DS:[ESI+0xC]
$+347    00524857     8D45 A0                   LEA EAX, DWORD PTR SS:[EBP-0x60]
$+34A    0052485A     50                        PUSH EAX
$+34B    0052485B     E8 70EBFCFF               CALL PathOfEx.004F33D0
$+350    00524860     8B45 A0                   MOV EAX, DWORD PTR SS:[EBP-0x60]
$+353    00524863     3B46 10                   CMP EAX, DWORD PTR DS:[ESI+0x10]
$+356    00524866     8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-0x10]
$+359    00524869     0F84 86050000             JE PathOfEx.00524DF5
$+35F    0052486F     8B50 10                   MOV EDX, DWORD PTR DS:[EAX+0x10]
$+362    00524872     8955 AC                   MOV DWORD PTR SS:[EBP-0x54], EDX
$+365    00524875     85D2                      TEST EDX, EDX
$+367    00524877     0F84 78050000             JE PathOfEx.00524DF5
$+36D    0052487D     8D86 980B0000             LEA EAX, DWORD PTR DS:[ESI+0xB98]
$+373    00524883     8945 98                   MOV DWORD PTR SS:[EBP-0x68], EAX
$+376    00524886     3BC7                      CMP EAX, EDI
$+378    00524888     74 16                     JE SHORT PathOfEx.005248A0
$+37A    0052488A     8B4A 08                   MOV ECX, DWORD PTR DS:[EDX+0x8]
$+37D    0052488D     0F1F00                    NOP DWORD PTR DS:[EAX]
$+380    00524890     3B48 04                   CMP ECX, DWORD PTR DS:[EAX+0x4]
$+383    00524893     0F84 5C050000             JE PathOfEx.00524DF5
$+389    00524899     83C0 10                   ADD EAX, 0x10
$+38C    0052489C     3BC7                      CMP EAX, EDI
$+38E    0052489E   ^ 75 F0                     JNZ SHORT PathOfEx.00524890
$+390    005248A0     837A 18 00                CMP DWORD PTR DS:[EDX+0x18], 0x0
$+394    005248A4     0F84 4B050000             JE PathOfEx.00524DF5
$+39A    005248AA     8B42 18                   MOV EAX, DWORD PTR DS:[EDX+0x18]
$+39D    005248AD     F3:0F1045 B4              MOVSS XMM0, DWORD PTR SS:[EBP-0x4C]
$+3A2    005248B2     8B4A 08                   MOV ECX, DWORD PTR DS:[EDX+0x8]
$+3A5    005248B5     F3:0F1048 04              MOVSS XMM1, DWORD PTR DS:[EAX+0x4]
$+3AA    005248BA     F3:0F5800                 ADDSS XMM0, DWORD PTR DS:[EAX]
$+3AE    005248BE     F3:0F584D B0              ADDSS XMM1, DWORD PTR SS:[EBP-0x50]
$+3B3    005248C3     F3:0F1145 C0              MOVSS DWORD PTR SS:[EBP-0x40], XMM0
$+3B8    005248C8     F3:0F114D 94              MOVSS DWORD PTR SS:[EBP-0x6C], XMM1
$+3BD    005248CD     F3:0F1145 84              MOVSS DWORD PTR SS:[EBP-0x7C], XMM0
$+3C2    005248D2     F3:0F114D 88              MOVSS DWORD PTR SS:[EBP-0x78], XMM1
$+3C7    005248D7     85C9                      TEST ECX, ECX
$+3C9    005248D9     74 21                     JE SHORT PathOfEx.005248FC
$+3CB    005248DB     8D85 74FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0x8C]
$+3D1    005248E1     50                        PUSH EAX
$+3D2    005248E2     E8 F9B1FFFF               CALL PathOfEx.0051FAE0
$+3D7    005248E7     8B4D E4                   MOV ECX, DWORD PTR SS:[EBP-0x1C]
$+3DA    005248EA     83C9 01                   OR ECX, 0x1
$+3DD    005248ED     894D E4                   MOV DWORD PTR SS:[EBP-0x1C], ECX
$+3E0    005248F0     8378 04 00                CMP DWORD PTR DS:[EAX+0x4], 0x0
$+3E4    005248F4     74 09                     JE SHORT PathOfEx.005248FF
$+3E6    005248F6     C645 EF 01                MOV BYTE PTR SS:[EBP-0x11], 0x1
$+3EA    005248FA     EB 07                     JMP SHORT PathOfEx.00524903
$+3EC    005248FC     8B4D E4                   MOV ECX, DWORD PTR SS:[EBP-0x1C]
$+3EF    005248FF     C645 EF 00                MOV BYTE PTR SS:[EBP-0x11], 0x0
$+3F3    00524903     F6C1 01                   TEST CL, 0x1
$+3F6    00524906     74 35                     JE SHORT PathOfEx.0052493D
$+3F8    00524908     83E1 FE                   AND ECX, -0x2
$+3FB    0052490B     894D E4                   MOV DWORD PTR SS:[EBP-0x1C], ECX
$+3FE    0052490E     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4], 0x1
$+405    00524915     8B85 74FFFFFF             MOV EAX, DWORD PTR SS:[EBP-0x8C]
$+40B    0052491B     85C0                      TEST EAX, EAX
$+40D    0052491D     74 17                     JE SHORT PathOfEx.00524936
$+40F    0052491F     50                        PUSH EAX
$+410    00524920     E8 AB1B1F00               CALL PathOfEx.007164D0
$+415    00524925     8BC8                      MOV ECX, EAX
$+417    00524927     E8 34171F00               CALL PathOfEx.00716060
$+41C    0052492C     C785 74FFFFFF 00000000    MOV DWORD PTR SS:[EBP-0x8C], 0x0
$+426    00524936     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+42D    0052493D     8BB6 3C090000             MOV ESI, DWORD PTR DS:[ESI+0x93C]
$+433    00524943     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
$+436    00524946     50                        PUSH EAX
$+437    00524947     8D45 9C                   LEA EAX, DWORD PTR SS:[EBP-0x64]
$+43A    0052494A     50                        PUSH EAX
$+43B    0052494B     8D4E 0C                   LEA ECX, DWORD PTR DS:[ESI+0xC]
$+43E    0052494E     E8 7DEAFCFF               CALL PathOfEx.004F33D0
$+443    00524953     8B45 9C                   MOV EAX, DWORD PTR SS:[EBP-0x64]
$+446    00524956     3B46 10                   CMP EAX, DWORD PTR DS:[ESI+0x10]
$+449    00524959     75 04                     JNZ SHORT PathOfEx.0052495F
$+44B    0052495B     33FF                      XOR EDI, EDI
$+44D    0052495D     EB 03                     JMP SHORT PathOfEx.00524962
$+44F    0052495F     8B78 10                   MOV EDI, DWORD PTR DS:[EAX+0x10]
$+452    00524962     8B77 30                   MOV ESI, DWORD PTR DS:[EDI+0x30]
$+455    00524965     8B7F 34                   MOV EDI, DWORD PTR DS:[EDI+0x34]
$+458    00524968     897D 8C                   MOV DWORD PTR SS:[EBP-0x74], EDI
$+45B    0052496B     3BF7                      CMP ESI, EDI
$+45D    0052496D     0F84 7F040000             JE PathOfEx.00524DF2
$+463    00524973     8B06                      MOV EAX, DWORD PTR DS:[ESI]
$+465    00524975     8B48 04                   MOV ECX, DWORD PTR DS:[EAX+0x4]
$+468    00524978     8B45 98                   MOV EAX, DWORD PTR SS:[EBP-0x68]
$+46B    0052497B     894D D4                   MOV DWORD PTR SS:[EBP-0x2C], ECX
$+46E    0052497E     3B45 C8                   CMP EAX, DWORD PTR SS:[EBP-0x38]                  ; nvd3dum.67CFFE00
$+471    00524981     74 14                     JE SHORT PathOfEx.00524997
$+473    00524983     8B51 08                   MOV EDX, DWORD PTR DS:[ECX+0x8]
$+476    00524986     3B50 04                   CMP EDX, DWORD PTR DS:[EAX+0x4]
$+479    00524989     0F84 58040000             JE PathOfEx.00524DE7
$+47F    0052498F     83C0 10                   ADD EAX, 0x10
$+482    00524992     3B45 C8                   CMP EAX, DWORD PTR SS:[EBP-0x38]                  ; nvd3dum.67CFFE00
$+485    00524995   ^ 75 EF                     JNZ SHORT PathOfEx.00524986
$+487    00524997     8379 18 00                CMP DWORD PTR DS:[ECX+0x18], 0x0
$+48B    0052499B     8D41 04                   LEA EAX, DWORD PTR DS:[ECX+0x4]
$+48E    0052499E     8945 DC                   MOV DWORD PTR SS:[EBP-0x24], EAX
$+491    005249A1     0F84 40040000             JE PathOfEx.00524DE7
$+497    005249A7     8B49 08                   MOV ECX, DWORD PTR DS:[ECX+0x8]
$+49A    005249AA     85C9                      TEST ECX, ECX
$+49C    005249AC     74 1E                     JE SHORT PathOfEx.005249CC
$+49E    005249AE     8D45 A4                   LEA EAX, DWORD PTR SS:[EBP-0x5C]
$+4A1    005249B1     50                        PUSH EAX
$+4A2    005249B2     E8 29B1FFFF               CALL PathOfEx.0051FAE0
$+4A7    005249B7     8B4D E4                   MOV ECX, DWORD PTR SS:[EBP-0x1C]
$+4AA    005249BA     83C9 02                   OR ECX, 0x2
$+4AD    005249BD     894D E4                   MOV DWORD PTR SS:[EBP-0x1C], ECX
$+4B0    005249C0     8378 04 00                CMP DWORD PTR DS:[EAX+0x4], 0x0
$+4B4    005249C4     74 09                     JE SHORT PathOfEx.005249CF
$+4B6    005249C6     C645 0F 01                MOV BYTE PTR SS:[EBP+0xF], 0x1
$+4BA    005249CA     EB 07                     JMP SHORT PathOfEx.005249D3
$+4BC    005249CC     8B4D E4                   MOV ECX, DWORD PTR SS:[EBP-0x1C]
$+4BF    005249CF     C645 0F 00                MOV BYTE PTR SS:[EBP+0xF], 0x0
$+4C3    005249D3     F6C1 02                   TEST CL, 0x2
$+4C6    005249D6     74 2F                     JE SHORT PathOfEx.00524A07
$+4C8    005249D8     83E1 FD                   AND ECX, -0x3
$+4CB    005249DB     894D E4                   MOV DWORD PTR SS:[EBP-0x1C], ECX
$+4CE    005249DE     C745 FC 02000000          MOV DWORD PTR SS:[EBP-0x4], 0x2
$+4D5    005249E5     8B45 A4                   MOV EAX, DWORD PTR SS:[EBP-0x5C]
$+4D8    005249E8     85C0                      TEST EAX, EAX
$+4DA    005249EA     74 14                     JE SHORT PathOfEx.00524A00
$+4DC    005249EC     50                        PUSH EAX
$+4DD    005249ED     E8 DE1A1F00               CALL PathOfEx.007164D0
$+4E2    005249F2     8BC8                      MOV ECX, EAX
$+4E4    005249F4     E8 67161F00               CALL PathOfEx.00716060
$+4E9    005249F9     C745 A4 00000000          MOV DWORD PTR SS:[EBP-0x5C], 0x0
$+4F0    00524A00     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+4F7    00524A07     807D EF 00                CMP BYTE PTR SS:[EBP-0x11], 0x0
$+4FB    00524A0B     74 0C                     JE SHORT PathOfEx.00524A19
$+4FD    00524A0D     807D 0F 00                CMP BYTE PTR SS:[EBP+0xF], 0x0
$+501    00524A11     0F84 D0030000             JE PathOfEx.00524DE7
$+507    00524A17     EB 0A                     JMP SHORT PathOfEx.00524A23
$+509    00524A19     807D 0F 00                CMP BYTE PTR SS:[EBP+0xF], 0x0
$+50D    00524A1D     0F85 C4030000             JNZ PathOfEx.00524DE7
$+513    00524A23     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
$+516    00524A26     8B48 04                   MOV ECX, DWORD PTR DS:[EAX+0x4]
$+519    00524A29     8A41 71                   MOV AL, BYTE PTR DS:[ECX+0x71]
$+51C    00524A2C     84C0                      TEST AL, AL
$+51E    00524A2E     74 11                     JE SHORT PathOfEx.00524A41
$+520    00524A30     8B45 DC                   MOV EAX, DWORD PTR SS:[EBP-0x24]
$+523    00524A33     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+526    00524A36     8A40 72                   MOV AL, BYTE PTR DS:[EAX+0x72]
$+529    00524A39     84C0                      TEST AL, AL
$+52B    00524A3B     0F85 A6030000             JNZ PathOfEx.00524DE7
$+531    00524A41     8B45 DC                   MOV EAX, DWORD PTR SS:[EBP-0x24]
$+534    00524A44     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+537    00524A47     8A40 71                   MOV AL, BYTE PTR DS:[EAX+0x71]
$+53A    00524A4A     84C0                      TEST AL, AL
$+53C    00524A4C     74 0B                     JE SHORT PathOfEx.00524A59
$+53E    00524A4E     8A41 72                   MOV AL, BYTE PTR DS:[ECX+0x72]
$+541    00524A51     84C0                      TEST AL, AL
$+543    00524A53     0F85 8E030000             JNZ PathOfEx.00524DE7
$+549    00524A59     8B4D D4                   MOV ECX, DWORD PTR SS:[EBP-0x2C]
$+54C    00524A5C     F3:0F1045 B4              MOVSS XMM0, DWORD PTR SS:[EBP-0x4C]
$+551    00524A61     6A 50                     PUSH 0x50
$+553    00524A63     6A 00                     PUSH 0x0
$+555    00524A65     8B41 18                   MOV EAX, DWORD PTR DS:[ECX+0x18]
$+558    00524A68     F3:0F5800                 ADDSS XMM0, DWORD PTR DS:[EAX]
$+55C    00524A6C     F3:0F1145 C4              MOVSS DWORD PTR SS:[EBP-0x3C], XMM0
$+561    00524A71     F3:0F1040 04              MOVSS XMM0, DWORD PTR DS:[EAX+0x4]
$+566    00524A76     8D85 24FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xDC]
$+56C    00524A7C     F3:0F5845 B0              ADDSS XMM0, DWORD PTR SS:[EBP-0x50]
$+571    00524A81     50                        PUSH EAX
$+572    00524A82     F3:0F1145 90              MOVSS DWORD PTR SS:[EBP-0x70], XMM0
$+577    00524A87     E8 A4E17600               CALL PathOfEx.00C92C30
$+57C    00524A8C     0F57C0                    XORPS XMM0, XMM0
$+57F    00524A8F     C785 6CFFFFFF 00000000    MOV DWORD PTR SS:[EBP-0x94], 0x0
$+589    00524A99     83C4 0C                   ADD ESP, 0xC
$+58C    00524A9C     C785 70FFFFFF 00000000    MOV DWORD PTR SS:[EBP-0x90], 0x0
$+596    00524AA6     0F1185 24FFFFFF           MOVUPS DQWORD PTR SS:[EBP-0xDC], XMM0
$+59D    00524AAD     0F1185 34FFFFFF           MOVUPS DQWORD PTR SS:[EBP-0xCC], XMM0
$+5A4    00524AB4     0F1185 5CFFFFFF           MOVUPS DQWORD PTR SS:[EBP-0xA4], XMM0
$+5AB    00524ABB     8B4D BC                   MOV ECX, DWORD PTR SS:[EBP-0x44]                  ; PathOfEx.00E6DB8A
$+5AE    00524ABE     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4], 0x3
$+5B5    00524AC5     8B41 04                   MOV EAX, DWORD PTR DS:[ECX+0x4]
$+5B8    00524AC8     3B41 08                   CMP EAX, DWORD PTR DS:[ECX+0x8]
$+5BB    00524ACB     75 09                     JNZ SHORT PathOfEx.00524AD6
$+5BD    00524ACD     51                        PUSH ECX
$+5BE    00524ACE     E8 9D560000               CALL PathOfEx.0052A170
$+5C3    00524AD3     8B4D BC                   MOV ECX, DWORD PTR SS:[EBP-0x44]                  ; PathOfEx.00E6DB8A
$+5C6    00524AD6     8B41 04                   MOV EAX, DWORD PTR DS:[ECX+0x4]
$+5C9    00524AD9     85C0                      TEST EAX, EAX
$+5CB    00524ADB     74 11                     JE SHORT PathOfEx.00524AEE
$+5CD    00524ADD     8D8D 24FFFFFF             LEA ECX, DWORD PTR SS:[EBP-0xDC]
$+5D3    00524AE3     51                        PUSH ECX
$+5D4    00524AE4     8BC8                      MOV ECX, EAX
$+5D6    00524AE6     E8 E5700000               CALL PathOfEx.0052BBD0
$+5DB    00524AEB     8B4D BC                   MOV ECX, DWORD PTR SS:[EBP-0x44]                  ; PathOfEx.00E6DB8A
$+5DE    00524AEE     8341 04 50                ADD DWORD PTR DS:[ECX+0x4], 0x50
$+5E2    00524AF2     8D8D 24FFFFFF             LEA ECX, DWORD PTR SS:[EBP-0xDC]
$+5E8    00524AF8     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+5EF    00524AFF     E8 CC030000               CALL PathOfEx.00524ED0
$+5F4    00524B04     8B45 F0                   MOV EAX, DWORD PTR SS:[EBP-0x10]
$+5F7    00524B07     8B90 600B0000             MOV EDX, DWORD PTR DS:[EAX+0xB60]                 ; PathOfEx.006E006F
$+5FD    00524B0D     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]                  ; PathOfEx.009EB91B
$+600    00524B10     83C2 B0                   ADD EDX, -0x50
$+603    00524B13     8955 0C                   MOV DWORD PTR SS:[EBP+0xC], EDX
$+606    00524B16     3BD0                      CMP EDX, EAX
$+608    00524B18     74 44                     JE SHORT PathOfEx.00524B5E
$+60A    00524B1A     8B02                      MOV EAX, DWORD PTR DS:[EDX]
$+60C    00524B1C     85C0                      TEST EAX, EAX
$+60E    00524B1E     74 16                     JE SHORT PathOfEx.00524B36
$+610    00524B20     50                        PUSH EAX
$+611    00524B21     E8 AA191F00               CALL PathOfEx.007164D0
$+616    00524B26     8BC8                      MOV ECX, EAX
$+618    00524B28     E8 33151F00               CALL PathOfEx.00716060
$+61D    00524B2D     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+620    00524B30     C702 00000000             MOV DWORD PTR DS:[EDX], 0x0
$+626    00524B36     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
$+629    00524B39     8B00                      MOV EAX, DWORD PTR DS:[EAX]                       ; PathOfEx.00EEC4CC
$+62B    00524B3B     8945 E8                   MOV DWORD PTR SS:[EBP-0x18], EAX
$+62E    00524B3E     8902                      MOV DWORD PTR DS:[EDX], EAX
$+630    00524B40     85C0                      TEST EAX, EAX
$+632    00524B42     74 17                     JE SHORT PathOfEx.00524B5B
$+634    00524B44     8D48 38                   LEA ECX, DWORD PTR DS:[EAX+0x38]
$+637    00524B47     B8 01000000               MOV EAX, 0x1
$+63C    00524B4C     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX], EAX
$+640    00524B50     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
$+643    00524B53     A1 0C47FD00               MOV EAX, DWORD PTR DS:[0xFD470C]
$+648    00524B58     8941 34                   MOV DWORD PTR DS:[ECX+0x34], EAX
$+64B    00524B5B     8B45 D8                   MOV EAX, DWORD PTR SS:[EBP-0x28]
$+64E    00524B5E     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+651    00524B61     8B4D DC                   MOV ECX, DWORD PTR SS:[EBP-0x24]
$+654    00524B64     8942 04                   MOV DWORD PTR DS:[EDX+0x4], EAX
$+657    00524B67     8D42 08                   LEA EAX, DWORD PTR DS:[EDX+0x8]
$+65A    00524B6A     8945 D0                   MOV DWORD PTR SS:[EBP-0x30], EAX
$+65D    00524B6D     3BC1                      CMP EAX, ECX
$+65F    00524B6F     74 4C                     JE SHORT PathOfEx.00524BBD
$+661    00524B71     8B10                      MOV EDX, DWORD PTR DS:[EAX]                       ; PathOfEx.00EEC4CC
$+663    00524B73     85D2                      TEST EDX, EDX
$+665    00524B75     8955 E8                   MOV DWORD PTR SS:[EBP-0x18], EDX
$+668    00524B78     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+66B    00524B7B     74 1E                     JE SHORT PathOfEx.00524B9B
$+66D    00524B7D     FF75 E8                   PUSH DWORD PTR SS:[EBP-0x18]
$+670    00524B80     E8 4B191F00               CALL PathOfEx.007164D0
$+675    00524B85     8BC8                      MOV ECX, EAX
$+677    00524B87     E8 D4141F00               CALL PathOfEx.00716060
$+67C    00524B8C     8B45 D0                   MOV EAX, DWORD PTR SS:[EBP-0x30]
$+67F    00524B8F     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+682    00524B92     8B4D DC                   MOV ECX, DWORD PTR SS:[EBP-0x24]
$+685    00524B95     C700 00000000             MOV DWORD PTR DS:[EAX], 0x0
$+68B    00524B9B     8B09                      MOV ECX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+68D    00524B9D     894D D0                   MOV DWORD PTR SS:[EBP-0x30], ECX
$+690    00524BA0     8908                      MOV DWORD PTR DS:[EAX], ECX
$+692    00524BA2     85C9                      TEST ECX, ECX
$+694    00524BA4     74 17                     JE SHORT PathOfEx.00524BBD
$+696    00524BA6     83C1 38                   ADD ECX, 0x38
$+699    00524BA9     B8 01000000               MOV EAX, 0x1
$+69E    00524BAE     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX], EAX
$+6A2    00524BB2     8B4D D0                   MOV ECX, DWORD PTR SS:[EBP-0x30]
$+6A5    00524BB5     A1 0C47FD00               MOV EAX, DWORD PTR DS:[0xFD470C]
$+6AA    00524BBA     8941 34                   MOV DWORD PTR DS:[ECX+0x34], EAX
$+6AD    00524BBD     8B45 DC                   MOV EAX, DWORD PTR SS:[EBP-0x24]                  ; nvd3dum.67E6DCF9
$+6B0    00524BC0     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+6B3    00524BC3     8942 0C                   MOV DWORD PTR DS:[EDX+0xC], EAX
$+6B6    00524BC6     8B4D AC                   MOV ECX, DWORD PTR SS:[EBP-0x54]
$+6B9    00524BC9     8B41 1C                   MOV EAX, DWORD PTR DS:[ECX+0x1C]
$+6BC    00524BCC     8B49 18                   MOV ECX, DWORD PTR DS:[ECX+0x18]
$+6BF    00524BCF     894D D0                   MOV DWORD PTR SS:[EBP-0x30], ECX
$+6C2    00524BD2     85C0                      TEST EAX, EAX
$+6C4    00524BD4     74 04                     JE SHORT PathOfEx.00524BDA
$+6C6    00524BD6     F0:FF40 04                LOCK INC DWORD PTR DS:[EAX+0x4]
$+6CA    00524BDA     8B4A 14                   MOV ECX, DWORD PTR DS:[EDX+0x14]
$+6CD    00524BDD     8942 14                   MOV DWORD PTR DS:[EDX+0x14], EAX
$+6D0    00524BE0     8B45 D0                   MOV EAX, DWORD PTR SS:[EBP-0x30]
$+6D3    00524BE3     894D E8                   MOV DWORD PTR SS:[EBP-0x18], ECX
$+6D6    00524BE6     8942 10                   MOV DWORD PTR DS:[EDX+0x10], EAX
$+6D9    00524BE9     C745 FC 05000000          MOV DWORD PTR SS:[EBP-0x4], 0x5
$+6E0    00524BF0     85C9                      TEST ECX, ECX
$+6E2    00524BF2     74 23                     JE SHORT PathOfEx.00524C17
$+6E4    00524BF4     83C8 FF                   OR EAX, -0x1
$+6E7    00524BF7     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
$+6EC    00524BFC     75 19                     JNZ SHORT PathOfEx.00524C17
$+6EE    00524BFE     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+6F0    00524C00     FF10                      CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00EEC4CC
$+6F2    00524C02     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
$+6F5    00524C05     83C8 FF                   OR EAX, -0x1
$+6F8    00524C08     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$+6FD    00524C0D     75 05                     JNZ SHORT PathOfEx.00524C14
$+6FF    00524C0F     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+701    00524C11     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+704    00524C14     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+707    00524C17     8B45 D4                   MOV EAX, DWORD PTR SS:[EBP-0x2C]
$+70A    00524C1A     8B48 1C                   MOV ECX, DWORD PTR DS:[EAX+0x1C]
$+70D    00524C1D     8B40 18                   MOV EAX, DWORD PTR DS:[EAX+0x18]
$+710    00524C20     894D D0                   MOV DWORD PTR SS:[EBP-0x30], ECX
$+713    00524C23     8945 E8                   MOV DWORD PTR SS:[EBP-0x18], EAX
$+716    00524C26     85C9                      TEST ECX, ECX
$+718    00524C28     74 04                     JE SHORT PathOfEx.00524C2E
$+71A    00524C2A     F0:FF41 04                LOCK INC DWORD PTR DS:[ECX+0x4]
$+71E    00524C2E     8B4A 1C                   MOV ECX, DWORD PTR DS:[EDX+0x1C]
$+721    00524C31     8B45 D0                   MOV EAX, DWORD PTR SS:[EBP-0x30]
$+724    00524C34     8942 1C                   MOV DWORD PTR DS:[EDX+0x1C], EAX
$+727    00524C37     8B45 E8                   MOV EAX, DWORD PTR SS:[EBP-0x18]
$+72A    00524C3A     894D DC                   MOV DWORD PTR SS:[EBP-0x24], ECX
$+72D    00524C3D     8942 18                   MOV DWORD PTR DS:[EDX+0x18], EAX
$+730    00524C40     C745 FC 07000000          MOV DWORD PTR SS:[EBP-0x4], 0x7
$+737    00524C47     85C9                      TEST ECX, ECX
$+739    00524C49     74 23                     JE SHORT PathOfEx.00524C6E
$+73B    00524C4B     83C8 FF                   OR EAX, -0x1
$+73E    00524C4E     F0:0FC141 04              LOCK XADD DWORD PTR DS:[ECX+0x4], EAX
$+743    00524C53     75 19                     JNZ SHORT PathOfEx.00524C6E
$+745    00524C55     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+747    00524C57     FF10                      CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00EEC4CC
$+749    00524C59     8B4D DC                   MOV ECX, DWORD PTR SS:[EBP-0x24]
$+74C    00524C5C     83C8 FF                   OR EAX, -0x1
$+74F    00524C5F     F0:0FC141 08              LOCK XADD DWORD PTR DS:[ECX+0x8], EAX
$+754    00524C64     75 05                     JNZ SHORT PathOfEx.00524C6B
$+756    00524C66     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00E7DE64
$+758    00524C68     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+75B    00524C6B     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+75E    00524C6E     8B7D AC                   MOV EDI, DWORD PTR SS:[EBP-0x54]
$+761    00524C71     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+768    00524C78     8B4D D4                   MOV ECX, DWORD PTR SS:[EBP-0x2C]                  ; nvd3dum.67D07432
$+76B    00524C7B     8B47 10                   MOV EAX, DWORD PTR DS:[EDI+0x10]
$+76E    00524C7E     8942 28                   MOV DWORD PTR DS:[EDX+0x28], EAX
$+771    00524C81     8B41 10                   MOV EAX, DWORD PTR DS:[ECX+0x10]
$+774    00524C84     8942 2C                   MOV DWORD PTR DS:[EDX+0x2C], EAX
$+777    00524C87     8B47 14                   MOV EAX, DWORD PTR DS:[EDI+0x14]
$+77A    00524C8A     8942 30                   MOV DWORD PTR DS:[EDX+0x30], EAX
$+77D    00524C8D     8B41 14                   MOV EAX, DWORD PTR DS:[ECX+0x14]
$+780    00524C90     8B4A 28                   MOV ECX, DWORD PTR DS:[EDX+0x28]
$+783    00524C93     8942 34                   MOV DWORD PTR DS:[EDX+0x34], EAX
$+786    00524C96     8B55 F0                   MOV EDX, DWORD PTR SS:[EBP-0x10]
$+789    00524C99     8B82 3C090000             MOV EAX, DWORD PTR DS:[EDX+0x93C]                 ; PathOfEx.00640065
$+78F    00524C9F     8B10                      MOV EDX, DWORD PTR DS:[EAX]                       ; PathOfEx.00EEC4CC
$+791    00524CA1     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+794    00524CA4     8B148A                    MOV EDX, DWORD PTR DS:[EDX+ECX*4]
$+797    00524CA7     8B48 30                   MOV ECX, DWORD PTR DS:[EAX+0x30]
$+79A    00524CAA     E8 51B22E00               CALL PathOfEx.0080FF00
$+79F    00524CAF     0F5705 30DBF300           XORPS XMM0, DQWORD PTR DS:[0xF3DB30]
$+7A6    00524CB6     F3:0F1145 D0              MOVSS DWORD PTR SS:[EBP-0x30], XMM0
$+7AB    00524CBB     E8 6065F7FF               CALL PathOfEx.0049B220
$+7B0    00524CC0     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+7B3    00524CC3     8B7D F0                   MOV EDI, DWORD PTR SS:[EBP-0x10]
$+7B6    00524CC6     F3:0F104D C0              MOVSS XMM1, DWORD PTR SS:[EBP-0x40]
$+7BB    00524CCB     8B4A 28                   MOV ECX, DWORD PTR DS:[EDX+0x28]
$+7BE    00524CCE     8B87 BC090000             MOV EAX, DWORD PTR DS:[EDI+0x9BC]
$+7C4    00524CD4     F3:0F590488               MULSS XMM0, DWORD PTR DS:[EAX+ECX*4]
$+7C9    00524CD9     F3:0F5CC8                 SUBSS XMM1, XMM0
$+7CD    00524CDD     F3:0F1045 D0              MOVSS XMM0, DWORD PTR SS:[EBP-0x30]
$+7D2    00524CE2     F3:0F114D E8              MOVSS DWORD PTR SS:[EBP-0x18], XMM1
$+7D7    00524CE7     E8 4465F7FF               CALL PathOfEx.0049B230
$+7DC    00524CEC     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+7DF    00524CEF     8B87 BC090000             MOV EAX, DWORD PTR DS:[EDI+0x9BC]
$+7E5    00524CF5     F3:0F104D 94              MOVSS XMM1, DWORD PTR SS:[EBP-0x6C]
$+7EA    00524CFA     8B4A 28                   MOV ECX, DWORD PTR DS:[EDX+0x28]
$+7ED    00524CFD     F3:0F590488               MULSS XMM0, DWORD PTR DS:[EAX+ECX*4]
$+7F2    00524D02     8B4A 2C                   MOV ECX, DWORD PTR DS:[EDX+0x2C]
$+7F5    00524D05     F3:0F5CC8                 SUBSS XMM1, XMM0
$+7F9    00524D09     F3:0F1045 E8              MOVSS XMM0, DWORD PTR SS:[EBP-0x18]
$+7FE    00524D0E     F3:0F1142 38              MOVSS DWORD PTR DS:[EDX+0x38], XMM0
$+803    00524D13     F3:0F114A 3C              MOVSS DWORD PTR DS:[EDX+0x3C], XMM1
$+808    00524D18     8BD7                      MOV EDX, EDI
$+80A    00524D1A     8B82 3C090000             MOV EAX, DWORD PTR DS:[EDX+0x93C]                 ; PathOfEx.00640065
$+810    00524D20     8B10                      MOV EDX, DWORD PTR DS:[EAX]                       ; PathOfEx.00EEC4CC
$+812    00524D22     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+815    00524D25     8B148A                    MOV EDX, DWORD PTR DS:[EDX+ECX*4]
$+818    00524D28     8B48 34                   MOV ECX, DWORD PTR DS:[EAX+0x34]
$+81B    00524D2B     E8 D0B12E00               CALL PathOfEx.0080FF00
$+820    00524D30     0F5705 30DBF300           XORPS XMM0, DQWORD PTR DS:[0xF3DB30]
$+827    00524D37     F3:0F1145 D0              MOVSS DWORD PTR SS:[EBP-0x30], XMM0
$+82C    00524D3C     E8 DF64F7FF               CALL PathOfEx.0049B220
$+831    00524D41     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+834    00524D44     8B87 BC090000             MOV EAX, DWORD PTR DS:[EDI+0x9BC]
$+83A    00524D4A     F3:0F104D C4              MOVSS XMM1, DWORD PTR SS:[EBP-0x3C]
$+83F    00524D4F     8B4A 2C                   MOV ECX, DWORD PTR DS:[EDX+0x2C]
$+842    00524D52     F3:0F590488               MULSS XMM0, DWORD PTR DS:[EAX+ECX*4]
$+847    00524D57     F3:0F5CC8                 SUBSS XMM1, XMM0
$+84B    00524D5B     F3:0F1045 D0              MOVSS XMM0, DWORD PTR SS:[EBP-0x30]
$+850    00524D60     F3:0F114D C4              MOVSS DWORD PTR SS:[EBP-0x3C], XMM1
$+855    00524D65     E8 C664F7FF               CALL PathOfEx.0049B230
$+85A    00524D6A     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+85D    00524D6D     8B87 BC090000             MOV EAX, DWORD PTR DS:[EDI+0x9BC]
$+863    00524D73     F3:0F104D 90              MOVSS XMM1, DWORD PTR SS:[EBP-0x70]
$+868    00524D78     52                        PUSH EDX
$+869    00524D79     8B4A 2C                   MOV ECX, DWORD PTR DS:[EDX+0x2C]
$+86C    00524D7C     F3:0F590488               MULSS XMM0, DWORD PTR DS:[EAX+ECX*4]
$+871    00524D81     8D8D 7CFFFFFF             LEA ECX, DWORD PTR SS:[EBP-0x84]
$+877    00524D87     8B45 84                   MOV EAX, DWORD PTR SS:[EBP-0x7C]
$+87A    00524D8A     8942 48                   MOV DWORD PTR DS:[EDX+0x48], EAX
$+87D    00524D8D     8B45 88                   MOV EAX, DWORD PTR SS:[EBP-0x78]
$+880    00524D90     F3:0F5CC8                 SUBSS XMM1, XMM0
$+884    00524D94     8942 4C                   MOV DWORD PTR DS:[EDX+0x4C], EAX
$+887    00524D97     F3:0F1045 C4              MOVSS XMM0, DWORD PTR SS:[EBP-0x3C]
$+88C    00524D9C     F3:0F1142 40              MOVSS DWORD PTR DS:[EDX+0x40], XMM0
$+891    00524DA1     F3:0F114A 44              MOVSS DWORD PTR DS:[EDX+0x44], XMM1
$+896    00524DA6     E8 85000000               CALL PathOfEx.00524E30
$+89B    00524DAB     8BCF                      MOV ECX, EDI
$+89D    00524DAD     8B7D 8C                   MOV EDI, DWORD PTR SS:[EBP-0x74]                  ; nvd3dum.67CFF6CF
$+8A0    00524DB0     8B81 1C0C0000             MOV EAX, DWORD PTR DS:[ECX+0xC1C]
$+8A6    00524DB6     83E8 02                   SUB EAX, 0x2
$+8A9    00524DB9     74 22                     JE SHORT PathOfEx.00524DDD
$+8AB    00524DBB     83E8 01                   SUB EAX, 0x1
$+8AE    00524DBE     74 11                     JE SHORT PathOfEx.00524DD1
$+8B0    00524DC0     83E8 01                   SUB EAX, 0x1
$+8B3    00524DC3     75 22                     JNZ SHORT PathOfEx.00524DE7
$+8B5    00524DC5     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+8B8    00524DC8     C740 20 00010001          MOV DWORD PTR DS:[EAX+0x20], PathOfEx.01000100
$+8BF    00524DCF     EB 16                     JMP SHORT PathOfEx.00524DE7
$+8C1    00524DD1     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+8C4    00524DD4     C740 20 01000100          MOV DWORD PTR DS:[EAX+0x20], 0x10001
$+8CB    00524DDB     EB 0A                     JMP SHORT PathOfEx.00524DE7
$+8CD    00524DDD     8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+0xC]
$+8D0    00524DE0     C740 20 00000000          MOV DWORD PTR DS:[EAX+0x20], 0x0
$+8D7    00524DE7     83C6 04                   ADD ESI, 0x4
$+8DA    00524DEA     3BF7                      CMP ESI, EDI
$+8DC    00524DEC   ^ 0F85 81FBFFFF             JNZ PathOfEx.00524973
$+8E2    00524DF2     8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-0x10]
$+8E5    00524DF5     8B86 4C090000             MOV EAX, DWORD PTR DS:[ESI+0x94C]
$+8EB    00524DFB     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
$+8EE    00524DFE     2B86 48090000             SUB EAX, DWORD PTR DS:[ESI+0x948]
$+8F4    00524E04     41                        INC ECX
$+8F5    00524E05     8B7D C8                   MOV EDI, DWORD PTR SS:[EBP-0x38]                  ; nvd3dum.67CFFE00
$+8F8    00524E08     C1F8 03                   SAR EAX, 0x3
$+8FB    00524E0B     894D E0                   MOV DWORD PTR SS:[EBP-0x20], ECX
$+8FE    00524E0E     3BC8                      CMP ECX, EAX
$+900    00524E10   ^ 0F82 2BFAFFFF             JB PathOfEx.00524841
$+906    00524E16     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+909    00524E19     5F                        POP EDI                                           ; 0018E6FC
$+90A    00524E1A     5E                        POP ESI                                           ; 0018E6FC
$+90B    00524E1B     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+912    00524E22     8BE5                      MOV ESP, EBP
$+914    00524E24     5D                        POP EBP                                           ; 0018E6FC
$+915    00524E25     C2 0800                   RETN 0x8
$+918    00524E28     CC                        INT3
*/
/**/
//tag_后悔点数运算
/*
$ ==>    008084B0     55                        PUSH EBP                                          ; tag_后悔点数运算
$+1      008084B1     8BEC                      MOV EBP, ESP
$+3      008084B3     6A FF                     PUSH -0x1
$+5      008084B5     68 AD7CD500               PUSH PathOfEx.00D57CAD                            ; ASCII "给澍"
$+A      008084BA     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
$+10     008084C0     50                        PUSH EAX
$+11     008084C1     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
$+18     008084C8     83EC 14                   SUB ESP, 0x14
$+1B     008084CB     56                        PUSH ESI
$+1C     008084CC     8BF1                      MOV ESI, ECX
$+1E     008084CE     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18], 0x0
$+25     008084D5     57                        PUSH EDI
$+26     008084D6     8D4D E0                   LEA ECX, DWORD PTR SS:[EBP-0x20]
$+29     008084D9     8D56 04                   LEA EDX, DWORD PTR DS:[ESI+0x4]
$+2C     008084DC     E8 CF520000               CALL PathOfEx.0080D7B0
$+31     008084E1     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4], 0x1
$+38     008084E8     8D46 08                   LEA EAX, DWORD PTR DS:[ESI+0x8]
$+3B     008084EB     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
$+3E     008084EE     50                        PUSH EAX
$+3F     008084EF     8D49 08                   LEA ECX, DWORD PTR DS:[ECX+0x8]
$+42     008084F2     E8 09500000               CALL PathOfEx.0080D500
$+47     008084F7     8B4D E0                   MOV ECX, DWORD PTR SS:[EBP-0x20]
$+4A     008084FA     8D46 14                   LEA EAX, DWORD PTR DS:[ESI+0x14]
$+4D     008084FD     50                        PUSH EAX
$+4E     008084FE     8D49 14                   LEA ECX, DWORD PTR DS:[ECX+0x14]
$+51     00808501     E8 FA4F0000               CALL PathOfEx.0080D500
$+56     00808506     8B55 E0                   MOV EDX, DWORD PTR SS:[EBP-0x20]
$+59     00808509     8D46 20                   LEA EAX, DWORD PTR DS:[ESI+0x20]
$+5C     0080850C     8D4A 20                   LEA ECX, DWORD PTR DS:[EDX+0x20]
$+5F     0080850F     3BC8                      CMP ECX, EAX
$+61     00808511     74 09                     JE SHORT PathOfEx.0080851C
$+63     00808513     50                        PUSH EAX
$+64     00808514     E8 A7510000               CALL PathOfEx.0080D6C0
$+69     00808519     8B55 E0                   MOV EDX, DWORD PTR SS:[EBP-0x20]
$+6C     0080851C     8A46 40                   MOV AL, BYTE PTR DS:[ESI+0x40]
$+6F     0080851F     8842 40                   MOV BYTE PTR DS:[EDX+0x40], AL
$+72     00808522     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+75     00808525     8B4E 44                   MOV ECX, DWORD PTR DS:[ESI+0x44]
$+78     00808528     8948 44                   MOV DWORD PTR DS:[EAX+0x44], ECX
$+7B     0080852B     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+7E     0080852E     8B4E 48                   MOV ECX, DWORD PTR DS:[ESI+0x48]
$+81     00808531     8948 48                   MOV DWORD PTR DS:[EAX+0x48], ECX                  ; 后悔点数偏移
$+84     00808534     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+87     00808537     8B4E 4C                   MOV ECX, DWORD PTR DS:[ESI+0x4C]
$+8A     0080853A     8948 4C                   MOV DWORD PTR DS:[EAX+0x4C], ECX
$+8D     0080853D     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+90     00808540     8B4E 50                   MOV ECX, DWORD PTR DS:[ESI+0x50]
$+93     00808543     8948 50                   MOV DWORD PTR DS:[EAX+0x50], ECX
$+96     00808546     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+99     00808549     8B4E 54                   MOV ECX, DWORD PTR DS:[ESI+0x54]
$+9C     0080854C     8948 54                   MOV DWORD PTR DS:[EAX+0x54], ECX
$+9F     0080854F     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+A2     00808552     8B4E 58                   MOV ECX, DWORD PTR DS:[ESI+0x58]                  ; PathOfEx.00F2FCD8
$+A5     00808555     8948 58                   MOV DWORD PTR DS:[EAX+0x58], ECX
$+A8     00808558     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+AB     0080855B     8B4E 5C                   MOV ECX, DWORD PTR DS:[ESI+0x5C]
$+AE     0080855E     8948 5C                   MOV DWORD PTR DS:[EAX+0x5C], ECX
$+B1     00808561     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+B4     00808564     8B4E 60                   MOV ECX, DWORD PTR DS:[ESI+0x60]
$+B7     00808567     8948 60                   MOV DWORD PTR DS:[EAX+0x60], ECX
$+BA     0080856A     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4], 0x2
$+BE     0080856E     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
$+C1     00808571     8B75 E4                   MOV ESI, DWORD PTR SS:[EBP-0x1C]
$+C4     00808574     8B45 E0                   MOV EAX, DWORD PTR SS:[EBP-0x20]
$+C7     00808577     8975 F0                   MOV DWORD PTR SS:[EBP-0x10], ESI
$+CA     0080857A     C707 00000000             MOV DWORD PTR DS:[EDI], 0x0
$+D0     00808580     C747 04 00000000          MOV DWORD PTR DS:[EDI+0x4], 0x0
$+D7     00808587     8945 EC                   MOV DWORD PTR SS:[EBP-0x14], EAX
$+DA     0080858A     85F6                      TEST ESI, ESI
$+DC     0080858C     74 07                     JE SHORT PathOfEx.00808595
$+DE     0080858E     F0:FF46 04                LOCK INC DWORD PTR DS:[ESI+0x4]
$+E2     00808592     8B75 E4                   MOV ESI, DWORD PTR SS:[EBP-0x1C]
$+E5     00808595     53                        PUSH EBX
$+E6     00808596     8B5F 04                   MOV EBX, DWORD PTR DS:[EDI+0x4]
$+E9     00808599     83C8 FF                   OR EAX, -0x1
$+EC     0080859C     85DB                      TEST EBX, EBX
$+EE     0080859E     74 26                     JE SHORT PathOfEx.008085C6
$+F0     008085A0     F0:0FC143 04              LOCK XADD DWORD PTR DS:[EBX+0x4], EAX
$+F5     008085A5     75 19                     JNZ SHORT PathOfEx.008085C0
$+F7     008085A7     8B03                      MOV EAX, DWORD PTR DS:[EBX]                       ; PathOfEx.00E80934
$+F9     008085A9     8BCB                      MOV ECX, EBX
$+FB     008085AB     FF10                      CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00EEC4CC
$+FD     008085AD     83C9 FF                   OR ECX, -0x1
$+100    008085B0     8BC1                      MOV EAX, ECX
$+102    008085B2     F0:0FC143 08              LOCK XADD DWORD PTR DS:[EBX+0x8], EAX
$+107    008085B7     75 07                     JNZ SHORT PathOfEx.008085C0
$+109    008085B9     8B03                      MOV EAX, DWORD PTR DS:[EBX]                       ; PathOfEx.00E80934
$+10B    008085BB     8BCB                      MOV ECX, EBX
$+10D    008085BD     FF50 04                   CALL NEAR DWORD PTR DS:[EAX+0x4]
$+110    008085C0     8B75 E4                   MOV ESI, DWORD PTR SS:[EBP-0x1C]
$+113    008085C3     83C8 FF                   OR EAX, -0x1
$+116    008085C6     8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-0x10]
$+119    008085C9     894F 04                   MOV DWORD PTR DS:[EDI+0x4], ECX
$+11C    008085CC     8B4D EC                   MOV ECX, DWORD PTR SS:[EBP-0x14]
$+11F    008085CF     890F                      MOV DWORD PTR DS:[EDI], ECX
$+121    008085D1     C745 E8 01000000          MOV DWORD PTR SS:[EBP-0x18], 0x1
$+128    008085D8     C745 FC 03000000          MOV DWORD PTR SS:[EBP-0x4], 0x3
$+12F    008085DF     5B                        POP EBX
$+130    008085E0     85F6                      TEST ESI, ESI
$+132    008085E2     74 1F                     JE SHORT PathOfEx.00808603
$+134    008085E4     F0:0FC146 04              LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
$+139    008085E9     75 18                     JNZ SHORT PathOfEx.00808603
$+13B    008085EB     8B06                      MOV EAX, DWORD PTR DS:[ESI]
$+13D    008085ED     8BCE                      MOV ECX, ESI
$+13F    008085EF     FF10                      CALL NEAR DWORD PTR DS:[EAX]                      ; PathOfEx.00EEC4CC
$+141    008085F1     83C9 FF                   OR ECX, -0x1
$+144    008085F4     F0:0FC14E 08              LOCK XADD DWORD PTR DS:[ESI+0x8], ECX
$+149    008085F9     49                        DEC ECX
$+14A    008085FA     75 07                     JNZ SHORT PathOfEx.00808603
$+14C    008085FC     8B16                      MOV EDX, DWORD PTR DS:[ESI]
$+14E    008085FE     8BCE                      MOV ECX, ESI
$+150    00808600     FF52 04                   CALL NEAR DWORD PTR DS:[EDX+0x4]
$+153    00808603     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+156    00808606     8BC7                      MOV EAX, EDI
$+158    00808608     5F                        POP EDI                                           ; 18F7EBD8
$+159    00808609     5E                        POP ESI                                           ; 18F7EBD8
$+15A    0080860A     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+161    00808611     8BE5                      MOV ESP, EBP
$+163    00808613     5D                        POP EBP                                           ; 18F7EBD8
$+164    00808614     C2 0400                   RETN 0x4
*/
/**/
//tag_计算天赋剩余点数的上层函数
/*
$ ==>    00524050     55                        PUSH EBP                                          ; tag_计算天赋剩余点数的上层函数
$+1      00524051     8BEC                      MOV EBP, ESP
$+3      00524053     6A FF                     PUSH -0x1
$+5      00524055     68 16F2D100               PUSH PathOfEx.00D1F216
$+A      0052405A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
$+10     00524060     50                        PUSH EAX
$+11     00524061     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
$+18     00524068     83EC 08                   SUB ESP, 0x8
$+1B     0052406B     53                        PUSH EBX                                          ; PathOfEx.00E6DDE8
$+1C     0052406C     56                        PUSH ESI
$+1D     0052406D     8B75 0C                   MOV ESI, DWORD PTR SS:[EBP+0xC]
$+20     00524070     32C0                      XOR AL, AL
$+22     00524072     57                        PUSH EDI
$+23     00524073     8B7D 08                   MOV EDI, DWORD PTR SS:[EBP+0x8]
$+26     00524076     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10], 0x0
$+2D     0052407D     89BE A4090000             MOV DWORD PTR DS:[ESI+0x9A4], EDI
$+33     00524083     C686 C9090000 01          MOV BYTE PTR DS:[ESI+0x9C9], 0x1
$+3A     0052408A     85FF                      TEST EDI, EDI
$+3C     0052408C     0F84 2B010000             JE PathOfEx.005241BD
$+42     00524092     8B81 38090000             MOV EAX, DWORD PTR DS:[ECX+0x938]
$+48     00524098     85C0                      TEST EAX, EAX
$+4A     0052409A     74 1F                     JE SHORT PathOfEx.005240BB
$+4C     0052409C     8B80 943A0000             MOV EAX, DWORD PTR DS:[EAX+0x3A94]
$+52     005240A2     8B88 C4010000             MOV ECX, DWORD PTR DS:[EAX+0x1C4]
$+58     005240A8     E8 B3E0F6FF               CALL PathOfEx.00492160
$+5D     005240AD     8A40 28                   MOV AL, BYTE PTR DS:[EAX+0x28]
$+60     005240B0     0FB6D8                    MOVZX EBX, AL
$+63     005240B3     83E3 07                   AND EBX, 0x7
$+66     005240B6     895D 08                   MOV DWORD PTR SS:[EBP+0x8], EBX                   ; PathOfEx.00E6DDE8
$+69     005240B9     EB 07                     JMP SHORT PathOfEx.005240C2
$+6B     005240BB     C745 08 07000000          MOV DWORD PTR SS:[EBP+0x8], 0x7
$+72     005240C2     8B86 B0080000             MOV EAX, DWORD PTR DS:[ESI+0x8B0]
$+78     005240C8     8BCF                      MOV ECX, EDI
$+7A     005240CA     83C0 04                   ADD EAX, 0x4
$+7D     005240CD     50                        PUSH EAX
$+7E     005240CE     E8 6D382E00               CALL PathOfEx.00807940                            ; tag_计算剩余天赋点
$+83     005240D3     84C0                      TEST AL, AL
$+85     005240D5     74 41                     JE SHORT PathOfEx.00524118
$+87     005240D7     8B4F 0C                   MOV ECX, DWORD PTR DS:[EDI+0xC]
$+8A     005240DA     2B4F 08                   SUB ECX, DWORD PTR DS:[EDI+0x8]
$+8D     005240DD     8B47 60                   MOV EAX, DWORD PTR DS:[EDI+0x60]
$+90     005240E0     8B5F 58                   MOV EBX, DWORD PTR DS:[EDI+0x58]
$+93     005240E3     2BC3                      SUB EAX, EBX                                      ; PathOfEx.00E6DDE8
$+95     005240E5     D1F9                      SAR ECX, 1                                        ; 获取天赋数量
$+97     005240E7     03C1                      ADD EAX, ECX
$+99     005240E9     75 12                     JNZ SHORT PathOfEx.005240FD
$+9B     005240EB     8B96 B0080000             MOV EDX, DWORD PTR DS:[ESI+0x8B0]
$+A1     005240F1     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]
$+A4     005240F4     E8 97BAFFFF               CALL PathOfEx.0051FB90
$+A9     005240F9     84C0                      TEST AL, AL
$+AB     005240FB     75 14                     JNZ SHORT PathOfEx.00524111
$+AD     005240FD     2B5F 60                   SUB EBX, DWORD PTR DS:[EDI+0x60]
$+B0     00524100     75 16                     JNZ SHORT PathOfEx.00524118
$+B2     00524102     8B8E B0080000             MOV ECX, DWORD PTR DS:[ESI+0x8B0]
$+B8     00524108     E8 93BBFFFF               CALL PathOfEx.0051FCA0
$+BD     0052410D     84C0                      TEST AL, AL
$+BF     0052410F     74 07                     JE SHORT PathOfEx.00524118
$+C1     00524111     B0 01                     MOV AL, 0x1
$+C3     00524113     E9 A5000000               JMP PathOfEx.005241BD
$+C8     00524118     8B8E B0080000             MOV ECX, DWORD PTR DS:[ESI+0x8B0]
$+CE     0052411E     E8 EDBAFFFF               CALL PathOfEx.0051FC10
$+D3     00524123     8945 0C                   MOV DWORD PTR SS:[EBP+0xC], EAX
$+D6     00524126     85C0                      TEST EAX, EAX
$+D8     00524128     74 21                     JE SHORT PathOfEx.0052414B
$+DA     0052412A     51                        PUSH ECX
$+DB     0052412B     8D4D EC                   LEA ECX, DWORD PTR SS:[EBP-0x14]
$+DE     0052412E     51                        PUSH ECX
$+DF     0052412F     8B48 08                   MOV ECX, DWORD PTR DS:[EAX+0x8]
$+E2     00524132     E8 39860000               CALL PathOfEx.0052C770
$+E7     00524137     8B4D 08                   MOV ECX, DWORD PTR SS:[EBP+0x8]
$+EA     0052413A     BA 01000000               MOV EDX, 0x1
$+EF     0052413F     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+F2     00524142     3948 2C                   CMP DWORD PTR DS:[EAX+0x2C], ECX
$+F5     00524145     74 07                     JE SHORT PathOfEx.0052414E
$+F7     00524147     8ADA                      MOV BL, DL
$+F9     00524149     EB 05                     JMP SHORT PathOfEx.00524150
$+FB     0052414B     8B55 F0                   MOV EDX, DWORD PTR SS:[EBP-0x10]
$+FE     0052414E     32DB                      XOR BL, BL
$+100    00524150     F6C2 01                   TEST DL, 0x1
$+103    00524153     74 2D                     JE SHORT PathOfEx.00524182
$+105    00524155     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
$+10C    0052415C     8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-0x14]
$+10F    0052415F     85C0                      TEST EAX, EAX
$+111    00524161     74 18                     JE SHORT PathOfEx.0052417B
$+113    00524163     50                        PUSH EAX
$+114    00524164     51                        PUSH ECX
$+115    00524165     E8 66231F00               CALL PathOfEx.007164D0
$+11A    0052416A     83C4 04                   ADD ESP, 0x4
$+11D    0052416D     8BC8                      MOV ECX, EAX
$+11F    0052416F     E8 EC1E1F00               CALL PathOfEx.00716060
$+124    00524174     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14], 0x0
$+12B    0052417B     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4], -0x1
$+132    00524182     84DB                      TEST BL, BL
$+134    00524184     74 13                     JE SHORT PathOfEx.00524199
$+136    00524186     8B55 0C                   MOV EDX, DWORD PTR SS:[EBP+0xC]
$+139    00524189     8BCF                      MOV ECX, EDI
$+13B    0052418B     E8 80BBFFFF               CALL PathOfEx.0051FD10
$+140    00524190     85C0                      TEST EAX, EAX
$+142    00524192     0F94C0                    SETE AL
$+145    00524195     84C0                      TEST AL, AL
$+147    00524197     75 24                     JNZ SHORT PathOfEx.005241BD
$+149    00524199     8B8E B0080000             MOV ECX, DWORD PTR DS:[ESI+0x8B0]
$+14F    0052419F     8B41 08                   MOV EAX, DWORD PTR DS:[ECX+0x8]                   ; PathOfEx.00650074
$+152    005241A2     8078 72 00                CMP BYTE PTR DS:[EAX+0x72], 0x0
$+156    005241A6     74 13                     JE SHORT PathOfEx.005241BB
$+158    005241A8     8D41 04                   LEA EAX, DWORD PTR DS:[ECX+0x4]
$+15B    005241AB     8BCF                      MOV ECX, EDI
$+15D    005241AD     50                        PUSH EAX
$+15E    005241AE     E8 8D372E00               CALL PathOfEx.00807940                            ; tag_计算剩余天赋点
$+163    005241B3     84C0                      TEST AL, AL
$+165    005241B5   ^ 0F85 56FFFFFF             JNZ PathOfEx.00524111
$+16B    005241BB     32C0                      XOR AL, AL
$+16D    005241BD     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+170    005241C0     5F                        POP EDI                                           ; 3C1B6B9C
$+171    005241C1     8886 89090000             MOV BYTE PTR DS:[ESI+0x989], AL
$+177    005241C7     5E                        POP ESI                                           ; 34B44558
$+178    005241C8     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+17F    005241CF     5B                        POP EBX                                           ; 0C1A5DA4
$+180    005241D0     8BE5                      MOV ESP, EBP
$+182    005241D2     5D                        POP EBP                                           ; 0C1A5DA4
$+183    005241D3     C2 0800                   RETN 0x8
*/
/**/
//tag_计算剩余天赋点
/*
$ ==>    00807940     55                        PUSH EBP                                          ; tag_计算剩余天赋点
$+1      00807941     8BEC                      MOV EBP, ESP
$+3      00807943     6A FF                     PUSH -0x1
$+5      00807945     68 197CD500               PUSH PathOfEx.00D57C19
$+A      0080794A     64:A1 00000000            MOV EAX, DWORD PTR FS:[0]
$+10     00807950     50                        PUSH EAX
$+11     00807951     64:8925 00000000          MOV DWORD PTR FS:[0], ESP
$+18     00807958     81EC A4000000             SUB ESP, 0xA4
$+1E     0080795E     53                        PUSH EBX                                          ; PathOfEx.00E6DDE8
$+1F     0080795F     56                        PUSH ESI
$+20     00807960     8B75 08                   MOV ESI, DWORD PTR SS:[EBP+0x8]
$+23     00807963     8D45 A0                   LEA EAX, DWORD PTR SS:[EBP-0x60]
$+26     00807966     57                        PUSH EDI
$+27     00807967     8BD9                      MOV EBX, ECX
$+29     00807969     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18], 0x0
$+30     00807970     50                        PUSH EAX
$+31     00807971     8B4E 04                   MOV ECX, DWORD PTR DS:[ESI+0x4]
$+34     00807974     895D E4                   MOV DWORD PTR SS:[EBP-0x1C], EBX                  ; PathOfEx.00E6DDE8
$+37     00807977     E8 6481D1FF               CALL PathOfEx.0051FAE0
$+3C     0080797C     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4], 0x0
$+43     00807983     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
$+46     00807986     8B7D A4                   MOV EDI, DWORD PTR SS:[EBP-0x5C]
$+49     00807989     8A40 72                   MOV AL, BYTE PTR DS:[EAX+0x72]
$+4C     0080798C     84C0                      TEST AL, AL
$+4E     0080798E     75 3B                     JNZ SHORT PathOfEx.008079CB
$+50     00807990     85FF                      TEST EDI, EDI
$+52     00807992     75 1E                     JNZ SHORT PathOfEx.008079B2
$+54     00807994     8B43 5C                   MOV EAX, DWORD PTR DS:[EBX+0x5C]                  ; PathOfEx.006F0074
$+57     00807997     0343 58                   ADD EAX, DWORD PTR DS:[EBX+0x58]                  ; PathOfEx.00750074
$+5A     0080799A     0343 50                   ADD EAX, DWORD PTR DS:[EBX+0x50]                  ; PathOfEx.006F0064
$+5D     0080799D     8B4B 0C                   MOV ECX, DWORD PTR DS:[EBX+0xC]                   ; PathOfEx.00740053
$+60     008079A0     0343 4C                   ADD EAX, DWORD PTR DS:[EBX+0x4C]                  ; PathOfEx.006E0069
$+63     008079A3     2B4B 08                   SUB ECX, DWORD PTR DS:[EBX+0x8]                   ; PathOfEx.0065006D
$+66     008079A6     0343 44                   ADD EAX, DWORD PTR DS:[EBX+0x44]                  ; PathOfEx.006F0074
$+69     008079A9     D1F9                      SAR ECX, 1
$+6B     008079AB     2BC1                      SUB EAX, ECX
$+6D     008079AD     83E8 01                   SUB EAX, 0x1                                      ; 计算剩余天赋点
$+70     008079B0     EB 13                     JMP SHORT PathOfEx.008079C5
$+72     008079B2     807B 40 40                CMP BYTE PTR DS:[EBX+0x40], 0x40
$+76     008079B6     0F82 9C030000             JB PathOfEx.00807D58
$+7C     008079BC     8B43 60                   MOV EAX, DWORD PTR DS:[EBX+0x60]                  ; PathOfEx.00690072
$+7F     008079BF     2B43 58                   SUB EAX, DWORD PTR DS:[EBX+0x58]                  ; PathOfEx.00750074
$+82     008079C2     0343 54                   ADD EAX, DWORD PTR DS:[EBX+0x54]
$+85     008079C5     0F84 8D030000             JE PathOfEx.00807D58
$+8B     008079CB     56                        PUSH ESI
$+8C     008079CC     8BCB                      MOV ECX, EBX                                      ; PathOfEx.00E6DDE8
$+8E     008079CE     E8 CD030000               CALL PathOfEx.00807DA0
$+93     008079D3     84C0                      TEST AL, AL
$+95     008079D5     0F85 7D030000             JNZ PathOfEx.00807D58
$+9B     008079DB     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
$+9E     008079DE     8378 28 00                CMP DWORD PTR DS:[EAX+0x28], 0x0
$+A2     008079E2     0F87 70030000             JA PathOfEx.00807D58
$+A8     008079E8     8A40 64                   MOV AL, BYTE PTR DS:[EAX+0x64]
$+AB     008079EB     84C0                      TEST AL, AL
$+AD     008079ED     0F85 65030000             JNZ PathOfEx.00807D58
$+B3     008079F3     85FF                      TEST EDI, EDI
$+B5     008079F5     74 33                     JE SHORT PathOfEx.00807A2A
$+B7     008079F7     8D45 B4                   LEA EAX, DWORD PTR SS:[EBP-0x4C]
$+BA     008079FA     8BCF                      MOV ECX, EDI
$+BC     008079FC     50                        PUSH EAX
$+BD     008079FD     E8 BEC7CEFF               CALL PathOfEx.004F41C0
$+C2     00807A02     0FB673 40                 MOVZX ESI, BYTE PTR DS:[EBX+0x40]
$+C6     00807A06     BA 01000000               MOV EDX, 0x1
$+CB     00807A0B     8955 E8                   MOV DWORD PTR SS:[EBP-0x18], EDX                  ; PathOfEx.00E6DDE8
$+CE     00807A0E     8B40 04                   MOV EAX, DWORD PTR DS:[EAX+0x4]
$+D1     00807A11     8B48 2C                   MOV ECX, DWORD PTR DS:[EAX+0x2C]
$+D4     00807A14     8BC6                      MOV EAX, ESI
$+D6     00807A16     83E0 07                   AND EAX, 0x7
$+D9     00807A19     3BC8                      CMP ECX, EAX
$+DB     00807A1B     75 08                     JNZ SHORT PathOfEx.00807A25
$+DD     00807A1D     C1EE 06                   SHR ESI, 0x6
$+E0     00807A20     3977 04                   CMP DWORD PTR DS:[EDI+0x4], ESI
$+E3     00807A23     74 08                     JE SHORT PathOfEx.00807A2D
$+E5     00807A25     8855 EF                   MOV BYTE PTR SS:[EBP-0x11], DL
$+E8     00807A28     EB 07                     JMP SHORT PathOfEx.00807A31
$+EA     00807A2A     8B55 E8                   MOV EDX, DWORD PTR SS:[EBP-0x18]
$+ED     00807A2D     C645 EF 00                MOV BYTE PTR SS:[EBP-0x11], 0x0
$+F1     00807A31     F6C2 01                   TEST DL, 0x1
$+F4     00807A34     74 0E                     JE SHORT PathOfEx.00807A44
$+F6     00807A36     83E2 FE                   AND EDX, -0x2
$+F9     00807A39     8D4D B4                   LEA ECX, DWORD PTR SS:[EBP-0x4C]
$+FC     00807A3C     8955 E8                   MOV DWORD PTR SS:[EBP-0x18], EDX                  ; PathOfEx.00E6DDE8
$+FF     00807A3F     E8 1C82C6FF               CALL PathOfEx.0046FC60
$+104    00807A44     807D EF 00                CMP BYTE PTR SS:[EBP-0x11], 0x0
$+108    00807A48     0F85 0A030000             JNZ PathOfEx.00807D58
$+10E    00807A4E     8B75 08                   MOV ESI, DWORD PTR SS:[EBP+0x8]
$+111    00807A51     8D4D DC                   LEA ECX, DWORD PTR SS:[EBP-0x24]
$+114    00807A54     51                        PUSH ECX
$+115    00807A55     8D4D D4                   LEA ECX, DWORD PTR SS:[EBP-0x2C]
$+118    00807A58     51                        PUSH ECX
$+119    00807A59     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
$+11C    00807A5C     8D4B 20                   LEA ECX, DWORD PTR DS:[EBX+0x20]
$+11F    00807A5F     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+122    00807A62     8945 DC                   MOV DWORD PTR SS:[EBP-0x24], EAX
$+125    00807A65     E8 9645C6FF               CALL PathOfEx.0046C000
$+12A    00807A6A     8B45 D4                   MOV EAX, DWORD PTR SS:[EBP-0x2C]
$+12D    00807A6D     3B43 24                   CMP EAX, DWORD PTR DS:[EBX+0x24]
$+130    00807A70     74 17                     JE SHORT PathOfEx.00807A89
$+132    00807A72     8D48 0C                   LEA ECX, DWORD PTR DS:[EAX+0xC]
$+135    00807A75     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00650044
$+137    00807A77     68 4A0B0000               PUSH 0xB4A
$+13C    00807A7C     FF10                      CALL NEAR DWORD PTR DS:[EAX]
$+13E    00807A7E     85C0                      TEST EAX, EAX
$+140    00807A80     74 07                     JE SHORT PathOfEx.00807A89
$+142    00807A82     B3 01                     MOV BL, 0x1
$+144    00807A84     E9 D1020000               JMP PathOfEx.00807D5A
$+149    00807A89     8B46 04                   MOV EAX, DWORD PTR DS:[ESI+0x4]
$+14C    00807A8C     8D4B 04                   LEA ECX, DWORD PTR DS:[EBX+0x4]
$+14F    00807A8F     FF70 20                   PUSH DWORD PTR DS:[EAX+0x20]
$+152    00807A92     51                        PUSH ECX
$+153    00807A93     E8 6897C8FF               CALL PathOfEx.00491200
$+158    00807A98     83C4 04                   ADD ESP, 0x4
$+15B    00807A9B     8BC8                      MOV ECX, EAX
$+15D    00807A9D     E8 2E7F0000               CALL PathOfEx.0080F9D0
$+162    00807AA2     8BF0                      MOV ESI, EAX
$+164    00807AA4     C645 D0 00                MOV BYTE PTR SS:[EBP-0x30], 0x0
$+168    00807AA8     8975 C8                   MOV DWORD PTR SS:[EBP-0x38], ESI
$+16B    00807AAB     8B7E 24                   MOV EDI, DWORD PTR DS:[ESI+0x24]
$+16E    00807AAE     897D CC                   MOV DWORD PTR SS:[EBP-0x34], EDI
$+171    00807AB1     3B7E 28                   CMP EDI, DWORD PTR DS:[ESI+0x28]
$+174    00807AB4     75 0A                     JNZ SHORT PathOfEx.00807AC0
$+176    00807AB6     8B7E 30                   MOV EDI, DWORD PTR DS:[ESI+0x30]
$+179    00807AB9     C645 D0 01                MOV BYTE PTR SS:[EBP-0x30], 0x1
$+17D    00807ABD     897D CC                   MOV DWORD PTR SS:[EBP-0x34], EDI
$+180    00807AC0     8B46 34                   MOV EAX, DWORD PTR DS:[ESI+0x34]
$+183    00807AC3     8D4D C8                   LEA ECX, DWORD PTR SS:[EBP-0x38]
$+186    00807AC6     8945 AC                   MOV DWORD PTR SS:[EBP-0x54], EAX
$+189    00807AC9     8D45 A8                   LEA EAX, DWORD PTR SS:[EBP-0x58]
$+18C    00807ACC     50                        PUSH EAX
$+18D    00807ACD     8975 A8                   MOV DWORD PTR SS:[EBP-0x58], ESI
$+190    00807AD0     C645 B0 01                MOV BYTE PTR SS:[EBP-0x50], 0x1
$+194    00807AD4     E8 674DD2FF               CALL PathOfEx.0052C840
$+199    00807AD9     84C0                      TEST AL, AL
$+19B    00807ADB     0F84 77020000             JE PathOfEx.00807D58
$+1A1    00807AE1     807D D0 00                CMP BYTE PTR SS:[EBP-0x30], 0x0
$+1A5    00807AE5     8B07                      MOV EAX, DWORD PTR DS:[EDI]
$+1A7    00807AE7     74 03                     JE SHORT PathOfEx.00807AEC
$+1A9    00807AE9     83C0 04                   ADD EAX, 0x4
$+1AC    00807AEC     8B30                      MOV ESI, DWORD PTR DS:[EAX]
$+1AE    00807AEE     8975 DC                   MOV DWORD PTR SS:[EBP-0x24], ESI
$+1B1    00807AF1     837E 08 00                CMP DWORD PTR DS:[ESI+0x8], 0x0
$+1B5    00807AF5     0F84 A0020000             JE PathOfEx.00807D9B
$+1BB    00807AFB     8D46 04                   LEA EAX, DWORD PTR DS:[ESI+0x4]
$+1BE    00807AFE     8BCB                      MOV ECX, EBX                                      ; PathOfEx.00E6DDE8
$+1C0    00807B00     50                        PUSH EAX
$+1C1    00807B01     E8 5A080000               CALL PathOfEx.00808360
$+1C6    00807B06     84C0                      TEST AL, AL
$+1C8    00807B08     0F85 8D020000             JNZ PathOfEx.00807D9B
$+1CE    00807B0E     8D46 04                   LEA EAX, DWORD PTR DS:[ESI+0x4]
$+1D1    00807B11     8BCB                      MOV ECX, EBX                                      ; PathOfEx.00E6DDE8
$+1D3    00807B13     50                        PUSH EAX
$+1D4    00807B14     E8 97080000               CALL PathOfEx.008083B0
$+1D9    00807B19     84C0                      TEST AL, AL
$+1DB    00807B1B     0F85 7A020000             JNZ PathOfEx.00807D9B
$+1E1    00807B21     8B46 08                   MOV EAX, DWORD PTR DS:[ESI+0x8]
$+1E4    00807B24     8D4D D4                   LEA ECX, DWORD PTR SS:[EBP-0x2C]
$+1E7    00807B27     FF73 0C                   PUSH DWORD PTR DS:[EBX+0xC]                       ; PathOfEx.00740053
$+1EA    00807B2A     FF73 08                   PUSH DWORD PTR DS:[EBX+0x8]                       ; PathOfEx.0065006D
$+1ED    00807B2D     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+1F0    00807B30     0FB7C0                    MOVZX EAX, AX
$+1F3    00807B33     8945 D4                   MOV DWORD PTR SS:[EBP-0x2C], EAX
$+1F6    00807B36     E8 455C0000               CALL PathOfEx.0080D780
$+1FB    00807B3B     83C4 08                   ADD ESP, 0x8
$+1FE    00807B3E     8845 E3                   MOV BYTE PTR SS:[EBP-0x1D], AL
$+201    00807B41     8D45 90                   LEA EAX, DWORD PTR SS:[EBP-0x70]
$+204    00807B44     50                        PUSH EAX
$+205    00807B45     8B45 08                   MOV EAX, DWORD PTR SS:[EBP+0x8]
$+208    00807B48     8B48 04                   MOV ECX, DWORD PTR DS:[EAX+0x4]
$+20B    00807B4B     E8 907FD1FF               CALL PathOfEx.0051FAE0
$+210    00807B50     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
$+213    00807B53     83C9 02                   OR ECX, 0x2
$+216    00807B56     894D E8                   MOV DWORD PTR SS:[EBP-0x18], ECX
$+219    00807B59     8378 04 00                CMP DWORD PTR DS:[EAX+0x4], 0x0
$+21D    00807B5D     74 1C                     JE SHORT PathOfEx.00807B7B
$+21F    00807B5F     8B4E 08                   MOV ECX, DWORD PTR DS:[ESI+0x8]
$+222    00807B62     8D45 98                   LEA EAX, DWORD PTR SS:[EBP-0x68]
$+225    00807B65     50                        PUSH EAX
$+226    00807B66     E8 757FD1FF               CALL PathOfEx.0051FAE0
$+22B    00807B6B     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
$+22E    00807B6E     83C9 04                   OR ECX, 0x4
$+231    00807B71     C645 EF 01                MOV BYTE PTR SS:[EBP-0x11], 0x1
$+235    00807B75     8378 04 00                CMP DWORD PTR DS:[EAX+0x4], 0x0
$+239    00807B79     74 04                     JE SHORT PathOfEx.00807B7F
$+23B    00807B7B     C645 EF 00                MOV BYTE PTR SS:[EBP-0x11], 0x0
$+23F    00807B7F     F6C1 04                   TEST CL, 0x4
$+242    00807B82     74 11                     JE SHORT PathOfEx.00807B95
$+244    00807B84     83E1 FB                   AND ECX, -0x5
$+247    00807B87     894D E8                   MOV DWORD PTR SS:[EBP-0x18], ECX
$+24A    00807B8A     8D4D 98                   LEA ECX, DWORD PTR SS:[EBP-0x68]
$+24D    00807B8D     E8 CE80C6FF               CALL PathOfEx.0046FC60
$+252    00807B92     8B4D E8                   MOV ECX, DWORD PTR SS:[EBP-0x18]
$+255    00807B95     83E1 FD                   AND ECX, -0x3
$+258    00807B98     894D E8                   MOV DWORD PTR SS:[EBP-0x18], ECX
$+25B    00807B9B     8D4D 90                   LEA ECX, DWORD PTR SS:[EBP-0x70]
$+25E    00807B9E     E8 BD80C6FF               CALL PathOfEx.0046FC60
$+263    00807BA3     807D EF 00                CMP BYTE PTR SS:[EBP-0x11], 0x0
$+267    00807BA7     0F85 79010000             JNZ PathOfEx.00807D26
$+26D    00807BAD     807D E3 00                CMP BYTE PTR SS:[EBP-0x1D], 0x0
$+271    00807BB1     0F84 6F010000             JE PathOfEx.00807D26
$+277    00807BB7     8B46 08                   MOV EAX, DWORD PTR DS:[ESI+0x8]
$+27A    00807BBA     8A40 71                   MOV AL, BYTE PTR DS:[EAX+0x71]
$+27D    00807BBD     84C0                      TEST AL, AL
$+27F    00807BBF     74 5F                     JE SHORT PathOfEx.00807C20
$+281    00807BC1     8B45 C8                   MOV EAX, DWORD PTR SS:[EBP-0x38]
$+284    00807BC4     8B5D DC                   MOV EBX, DWORD PTR SS:[EBP-0x24]
$+287    00807BC7     8B76 3C                   MOV ESI, DWORD PTR DS:[ESI+0x3C]
$+28A    00807BCA     8B48 08                   MOV ECX, DWORD PTR DS:[EAX+0x8]
$+28D    00807BCD     8B5B 40                   MOV EBX, DWORD PTR DS:[EBX+0x40]                  ; PathOfEx.005F0065
$+290    00807BD0     894D D8                   MOV DWORD PTR SS:[EBP-0x28], ECX
$+293    00807BD3     8A41 72                   MOV AL, BYTE PTR DS:[ECX+0x72]
$+296    00807BD6     84C0                      TEST AL, AL
$+298    00807BD8     0F84 10010000             JE PathOfEx.00807CEE
$+29E    00807BDE     3BF3                      CMP ESI, EBX                                      ; PathOfEx.00E6DDE8
$+2A0    00807BE0     74 38                     JE SHORT PathOfEx.00807C1A
$+2A2    00807BE2     8B06                      MOV EAX, DWORD PTR DS:[ESI]
$+2A4    00807BE4     8B40 08                   MOV EAX, DWORD PTR DS:[EAX+0x8]
$+2A7    00807BE7     3BC1                      CMP EAX, ECX
$+2A9    00807BE9     74 28                     JE SHORT PathOfEx.00807C13
$+2AB    00807BEB     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+2AE    00807BEE     8D4D D4                   LEA ECX, DWORD PTR SS:[EBP-0x2C]
$+2B1    00807BF1     0FB7C0                    MOVZX EAX, AX
$+2B4    00807BF4     8945 D4                   MOV DWORD PTR SS:[EBP-0x2C], EAX
$+2B7    00807BF7     8B45 E4                   MOV EAX, DWORD PTR SS:[EBP-0x1C]
$+2BA    00807BFA     FF70 0C                   PUSH DWORD PTR DS:[EAX+0xC]
$+2BD    00807BFD     FF70 08                   PUSH DWORD PTR DS:[EAX+0x8]
$+2C0    00807C00     E8 7B5B0000               CALL PathOfEx.0080D780
$+2C5    00807C05     83C4 08                   ADD ESP, 0x8
$+2C8    00807C08     84C0                      TEST AL, AL
$+2CA    00807C0A     0F85 13010000             JNZ PathOfEx.00807D23
$+2D0    00807C10     8B4D D8                   MOV ECX, DWORD PTR SS:[EBP-0x28]
$+2D3    00807C13     83C6 04                   ADD ESI, 0x4
$+2D6    00807C16     3BF3                      CMP ESI, EBX                                      ; PathOfEx.00E6DDE8
$+2D8    00807C18   ^ 75 C8                     JNZ SHORT PathOfEx.00807BE2
$+2DA    00807C1A     8B5D E4                   MOV EBX, DWORD PTR SS:[EBP-0x1C]
$+2DD    00807C1D     8B75 DC                   MOV ESI, DWORD PTR SS:[EBP-0x24]
$+2E0    00807C20     8B46 08                   MOV EAX, DWORD PTR DS:[ESI+0x8]
$+2E3    00807C23     8D4B 20                   LEA ECX, DWORD PTR DS:[EBX+0x20]
$+2E6    00807C26     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+2E9    00807C29     8945 D8                   MOV DWORD PTR SS:[EBP-0x28], EAX
$+2EC    00807C2C     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
$+2EF    00807C2F     50                        PUSH EAX
$+2F0    00807C30     8D45 B8                   LEA EAX, DWORD PTR SS:[EBP-0x48]
$+2F3    00807C33     50                        PUSH EAX
$+2F4    00807C34     E8 C743C6FF               CALL PathOfEx.0046C000
$+2F9    00807C39     8B45 B8                   MOV EAX, DWORD PTR SS:[EBP-0x48]
$+2FC    00807C3C     3B43 24                   CMP EAX, DWORD PTR DS:[EBX+0x24]
$+2FF    00807C3F     0F84 56010000             JE PathOfEx.00807D9B
$+305    00807C45     8D48 0C                   LEA ECX, DWORD PTR DS:[EAX+0xC]
$+308    00807C48     8B01                      MOV EAX, DWORD PTR DS:[ECX]                       ; PathOfEx.00650044
$+30A    00807C4A     68 4A0B0000               PUSH 0xB4A
$+30F    00807C4F     FF10                      CALL NEAR DWORD PTR DS:[EAX]
$+311    00807C51     85C0                      TEST EAX, EAX
$+313    00807C53     0F84 42010000             JE PathOfEx.00807D9B
$+319    00807C59     8D4D 80                   LEA ECX, DWORD PTR SS:[EBP-0x80]
$+31C    00807C5C     E8 8F86CDFF               CALL PathOfEx.004E02F0
$+321    00807C61     C745 BC 00000000          MOV DWORD PTR SS:[EBP-0x44], 0x0
$+328    00807C68     C745 C0 00000000          MOV DWORD PTR SS:[EBP-0x40], 0x0
$+32F    00807C6F     C745 C4 00000000          MOV DWORD PTR SS:[EBP-0x3C], 0x0
$+336    00807C76     8D85 54FFFFFF             LEA EAX, DWORD PTR SS:[EBP-0xAC]
$+33C    00807C7C     C785 54FFFFFF 74C4EE00    MOV DWORD PTR SS:[EBP-0xAC], PathOfEx.00EEC474
$+346    00807C86     899D 58FFFFFF             MOV DWORD PTR SS:[EBP-0xA8], EBX                  ; PathOfEx.00E6DDE8
$+34C    00807C8C     8985 78FFFFFF             MOV DWORD PTR SS:[EBP-0x88], EAX
$+352    00807C92     50                        PUSH EAX
$+353    00807C93     56                        PUSH ESI
$+354    00807C94     8D45 BC                   LEA EAX, DWORD PTR SS:[EBP-0x44]
$+357    00807C97     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4], 0x3
$+35B    00807C9B     50                        PUSH EAX
$+35C    00807C9C     8D45 80                   LEA EAX, DWORD PTR SS:[EBP-0x80]
$+35F    00807C9F     50                        PUSH EAX
$+360    00807CA0     8D4B 04                   LEA ECX, DWORD PTR DS:[EBX+0x4]
$+363    00807CA3     E8 5895C8FF               CALL PathOfEx.00491200
$+368    00807CA8     8BC8                      MOV ECX, EAX
$+36A    00807CAA     E8 B17D0000               CALL PathOfEx.0080FA60
$+36F    00807CAF     8D8D 54FFFFFF             LEA ECX, DWORD PTR SS:[EBP-0xAC]
$+375    00807CB5     E8 B683C6FF               CALL PathOfEx.00470070
$+37A    00807CBA     8B55 C0                   MOV EDX, DWORD PTR SS:[EBP-0x40]                  ; PathOfEx.009EDF6A
$+37D    00807CBD     8D45 D8                   LEA EAX, DWORD PTR SS:[EBP-0x28]
$+380    00807CC0     8B4D BC                   MOV ECX, DWORD PTR SS:[EBP-0x44]
$+383    00807CC3     50                        PUSH EAX
$+384    00807CC4     895D D8                   MOV DWORD PTR SS:[EBP-0x28], EBX                  ; PathOfEx.00E6DDE8
$+387    00807CC7     E8 D4600000               CALL PathOfEx.0080DDA0
$+38C    00807CCC     83C4 04                   ADD ESP, 0x4
$+38F    00807CCF     8D4D BC                   LEA ECX, DWORD PTR SS:[EBP-0x44]
$+392    00807CD2     3B45 C0                   CMP EAX, DWORD PTR SS:[EBP-0x40]
$+395    00807CD5     0F85 B3000000             JNZ PathOfEx.00807D8E
$+39B    00807CDB     E8 3029C7FF               CALL PathOfEx.0047A610
$+3A0    00807CE0     8D4D 80                   LEA ECX, DWORD PTR SS:[EBP-0x80]
$+3A3    00807CE3     C645 FC 00                MOV BYTE PTR SS:[EBP-0x4], 0x0
$+3A7    00807CE7     E8 2429C7FF               CALL PathOfEx.0047A610
$+3AC    00807CEC     EB 38                     JMP SHORT PathOfEx.00807D26
$+3AE    00807CEE     3BF3                      CMP ESI, EBX                                      ; PathOfEx.00E6DDE8
$+3B0    00807CF0     74 31                     JE SHORT PathOfEx.00807D23
$+3B2    00807CF2     8B06                      MOV EAX, DWORD PTR DS:[ESI]
$+3B4    00807CF4     8D4D D8                   LEA ECX, DWORD PTR SS:[EBP-0x28]
$+3B7    00807CF7     8B40 08                   MOV EAX, DWORD PTR DS:[EAX+0x8]
$+3BA    00807CFA     8B40 20                   MOV EAX, DWORD PTR DS:[EAX+0x20]
$+3BD    00807CFD     0FB7C0                    MOVZX EAX, AX
$+3C0    00807D00     8945 D8                   MOV DWORD PTR SS:[EBP-0x28], EAX
$+3C3    00807D03     8B45 E4                   MOV EAX, DWORD PTR SS:[EBP-0x1C]
$+3C6    00807D06     FF70 0C                   PUSH DWORD PTR DS:[EAX+0xC]
$+3C9    00807D09     FF70 08                   PUSH DWORD PTR DS:[EAX+0x8]
$+3CC    00807D0C     E8 6F5A0000               CALL PathOfEx.0080D780
$+3D1    00807D11     83C4 08                   ADD ESP, 0x8
$+3D4    00807D14     84C0                      TEST AL, AL
$+3D6    00807D16   ^ 0F85 FEFEFFFF             JNZ PathOfEx.00807C1A
$+3DC    00807D1C     83C6 04                   ADD ESI, 0x4
$+3DF    00807D1F     3BF3                      CMP ESI, EBX                                      ; PathOfEx.00E6DDE8
$+3E1    00807D21   ^ 75 CF                     JNZ SHORT PathOfEx.00807CF2
$+3E3    00807D23     8B5D E4                   MOV EBX, DWORD PTR SS:[EBP-0x1C]
$+3E6    00807D26     83C7 04                   ADD EDI, 0x4
$+3E9    00807D29     807D D0 00                CMP BYTE PTR SS:[EBP-0x30], 0x0
$+3ED    00807D2D     897D CC                   MOV DWORD PTR SS:[EBP-0x34], EDI
$+3F0    00807D30     75 12                     JNZ SHORT PathOfEx.00807D44
$+3F2    00807D32     8B75 C8                   MOV ESI, DWORD PTR SS:[EBP-0x38]
$+3F5    00807D35     3B7E 28                   CMP EDI, DWORD PTR DS:[ESI+0x28]
$+3F8    00807D38     75 0A                     JNZ SHORT PathOfEx.00807D44
$+3FA    00807D3A     8B7E 30                   MOV EDI, DWORD PTR DS:[ESI+0x30]
$+3FD    00807D3D     897D CC                   MOV DWORD PTR SS:[EBP-0x34], EDI
$+400    00807D40     C645 D0 01                MOV BYTE PTR SS:[EBP-0x30], 0x1
$+404    00807D44     8D45 A8                   LEA EAX, DWORD PTR SS:[EBP-0x58]
$+407    00807D47     50                        PUSH EAX
$+408    00807D48     8D4D C8                   LEA ECX, DWORD PTR SS:[EBP-0x38]
$+40B    00807D4B     E8 F04AD2FF               CALL PathOfEx.0052C840
$+410    00807D50     84C0                      TEST AL, AL
$+412    00807D52   ^ 0F85 89FDFFFF             JNZ PathOfEx.00807AE1
$+418    00807D58     32DB                      XOR BL, BL
$+41A    00807D5A     C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4], 0x4
$+421    00807D61     8B45 A0                   MOV EAX, DWORD PTR SS:[EBP-0x60]
$+424    00807D64     85C0                      TEST EAX, EAX
$+426    00807D66     74 11                     JE SHORT PathOfEx.00807D79
$+428    00807D68     50                        PUSH EAX
$+429    00807D69     51                        PUSH ECX
$+42A    00807D6A     E8 61E7F0FF               CALL PathOfEx.007164D0
$+42F    00807D6F     83C4 04                   ADD ESP, 0x4
$+432    00807D72     8BC8                      MOV ECX, EAX
$+434    00807D74     E8 E7E2F0FF               CALL PathOfEx.00716060
$+439    00807D79     8B4D F4                   MOV ECX, DWORD PTR SS:[EBP-0xC]
$+43C    00807D7C     8AC3                      MOV AL, BL
$+43E    00807D7E     5F                        POP EDI                                           ; 0DE511F0
$+43F    00807D7F     5E                        POP ESI                                           ; 0DE511F0
$+440    00807D80     64:890D 00000000          MOV DWORD PTR FS:[0], ECX
$+447    00807D87     5B                        POP EBX                                           ; 0DE511F0
$+448    00807D88     8BE5                      MOV ESP, EBP
$+44A    00807D8A     5D                        POP EBP                                           ; 0DE511F0
$+44B    00807D8B     C2 0400                   RETN 0x4
$+44E    00807D8E     E8 7D28C7FF               CALL PathOfEx.0047A610
$+453    00807D93     8D4D 80                   LEA ECX, DWORD PTR SS:[EBP-0x80]
$+456    00807D96     E8 7528C7FF               CALL PathOfEx.0047A610
$+45B    00807D9B     B3 01                     MOV BL, 0x1
$+45D    00807D9D   ^ EB BB                     JMP SHORT PathOfEx.00807D5A
$+45F    00807D9F     CC                        INT3
$+460    00807DA0     51                        PUSH ECX
$+461    00807DA1     53                        PUSH EBX                                          ; PathOfEx.00E6DDE8
$+462    00807DA2     55                        PUSH EBP
$+463    00807DA3     8B6C24 10                 MOV EBP, DWORD PTR SS:[ESP+0x10]
$+467    00807DA7     57                        PUSH EDI
$+468    00807DA8     8BF9                      MOV EDI, ECX
$+46A    00807DAA     8B5D 04                   MOV EBX, DWORD PTR SS:[EBP+0x4]
$+46D    00807DAD     85DB                      TEST EBX, EBX                                     ; PathOfEx.00E6DDE8
$+46F    00807DAF     75 09                     JNZ SHORT PathOfEx.00807DBA
$+471    00807DB1     32C0                      XOR AL, AL
$+473    00807DB3     5F                        POP EDI
$+474    00807DB4     5D                        POP EBP
$+475    00807DB5     5B                        POP EBX                                           ; PathOfEx.01137098
$+476    00807DB6     59                        POP ECX                                           ; 0DDB2360
$+477    00807DB7     C2 0400                   RETN 0x4
$+47A    00807DBA     0FB743 20                 MOVZX EAX, WORD PTR DS:[EBX+0x20]
$+47E    00807DBE     8B57 0C                   MOV EDX, DWORD PTR DS:[EDI+0xC]
$+481    00807DC1     894424 14                 MOV DWORD PTR SS:[ESP+0x14], EAX
$+485    00807DC5     8D4424 14                 LEA EAX, DWORD PTR SS:[ESP+0x14]
$+489    00807DC9     51                        PUSH ECX
$+48A    00807DCA     8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+0x8]
$+48D    00807DCD     50                        PUSH EAX
$+48E    00807DCE     E8 4D660000               CALL PathOfEx.0080E420
$+493    00807DD3     8BC8                      MOV ECX, EAX
$+495    00807DD5     83C4 08                   ADD ESP, 0x8
$+498    00807DD8     3B4F 0C                   CMP ECX, DWORD PTR DS:[EDI+0xC]
$+49B    00807DDB     74 09                     JE SHORT PathOfEx.00807DE6
$+49D    00807DDD     0FB743 20                 MOVZX EAX, WORD PTR DS:[EBX+0x20]
$+4A1    00807DE1     66:3B01                   CMP AX, WORD PTR DS:[ECX]
$+4A4    00807DE4     73 39                     JNB SHORT PathOfEx.00807E1F
$+4A6    00807DE6     8B4B 28                   MOV ECX, DWORD PTR DS:[EBX+0x28]                  ; PathOfEx.006F0063
$+4A9    00807DE9     8B43 2C                   MOV EAX, DWORD PTR DS:[EBX+0x2C]                  ; PathOfEx.0066006E
$+4AC    00807DEC     897C24 14                 MOV DWORD PTR SS:[ESP+0x14], EDI
$+4B0    00807DF0     8D14C8                    LEA EDX, DWORD PTR DS:[EAX+ECX*8]
$+4B3    00807DF3     8B4B 2C                   MOV ECX, DWORD PTR DS:[EBX+0x2C]                  ; PathOfEx.0066006E
$+4B6    00807DF6     8D4424 14                 LEA EAX, DWORD PTR SS:[ESP+0x14]
$+4BA    00807DFA     50                        PUSH EAX
$+4BB    00807DFB     E8 00600000               CALL PathOfEx.0080DE00
$+4C0    00807E00     8B4D 04                   MOV ECX, DWORD PTR SS:[EBP+0x4]
$+4C3    00807E03     83C4 04                   ADD ESP, 0x4
$+4C6    00807E06     8B51 28                   MOV EDX, DWORD PTR DS:[ECX+0x28]
$+4C9    00807E09     8B49 2C                   MOV ECX, DWORD PTR DS:[ECX+0x2C]
$+4CC    00807E0C     8D0CD1                    LEA ECX, DWORD PTR DS:[ECX+EDX*8]
$+4CF    00807E0F     3BC1                      CMP EAX, ECX
$+4D1    00807E11     75 0C                     JNZ SHORT PathOfEx.00807E1F
$+4D3    00807E13     55                        PUSH EBP
$+4D4    00807E14     8BCF                      MOV ECX, EDI
$+4D6    00807E16     E8 95050000               CALL PathOfEx.008083B0
$+4DB    00807E1B     84C0                      TEST AL, AL
$+4DD    00807E1D   ^ 74 92                     JE SHORT PathOfEx.00807DB1
$+4DF    00807E1F     5F                        POP EDI                                           ; 01AA4000
$+4E0    00807E20     5D                        POP EBP                                           ; PathOfEx.00F5C28F
$+4E1    00807E21     B0 01                     MOV AL, 0x1
$+4E3    00807E23     5B                        POP EBX                                           ; PathOfEx.00F5C28F
$+4E4    00807E24     59                        POP ECX
$+4E5    00807E25     C2 0400                   RETN 0x4
$+4E8    00807E28     CC                        INT3
*/
/**/
//tag_已学天赋点遍历运算
/*
$-1      00CEE6AF     CC                        INT3
$ ==>    00CEE6B0     8B4C24 08                 MOV ECX, DWORD PTR SS:[ESP+0x8]                   ; tag_已学天赋点遍历运算
$+4      00CEE6B4     56                        PUSH ESI
$+5      00CEE6B5     8B7424 08                 MOV ESI, DWORD PTR SS:[ESP+0x8]
$+9      00CEE6B9     2BCE                      SUB ECX, ESI
$+B      00CEE6BB     D1F9                      SAR ECX, 1
$+D      00CEE6BD     85C9                      TEST ECX, ECX
$+F      00CEE6BF     7E 34                     JLE SHORT PathOfEx.00CEE6F5
$+11     00CEE6C1     8B4424 10                 MOV EAX, DWORD PTR SS:[ESP+0x10]
$+15     00CEE6C5     57                        PUSH EDI
$+16     00CEE6C6     0FB738                    MOVZX EDI, WORD PTR DS:[EAX]
$+19     00CEE6C9     0F1F80 00000000           NOP DWORD PTR DS:[EAX]
$+20     00CEE6D0     8BC1                      MOV EAX, ECX
$+22     00CEE6D2     99                        CDQ
$+23     00CEE6D3     2BC2                      SUB EAX, EDX                                      ; PathOfEx.00E6DDE8
$+25     00CEE6D5     8BD0                      MOV EDX, EAX
$+27     00CEE6D7     D1FA                      SAR EDX, 1
$+29     00CEE6D9     66:393C56                 CMP WORD PTR DS:[ESI+EDX*2], DI
$+2D     00CEE6DD     8D0456                    LEA EAX, DWORD PTR DS:[ESI+EDX*2]
$+30     00CEE6E0     73 0C                     JNB SHORT PathOfEx.00CEE6EE
$+32     00CEE6E2     8D70 02                   LEA ESI, DWORD PTR DS:[EAX+0x2]
$+35     00CEE6E5     83C8 FF                   OR EAX, -0x1
$+38     00CEE6E8     2BC2                      SUB EAX, EDX                                      ; PathOfEx.00E6DDE8
$+3A     00CEE6EA     03C8                      ADD ECX, EAX
$+3C     00CEE6EC     EB 02                     JMP SHORT PathOfEx.00CEE6F0
$+3E     00CEE6EE     8BCA                      MOV ECX, EDX                                      ; PathOfEx.00E6DDE8
$+40     00CEE6F0     85C9                      TEST ECX, ECX
$+42     00CEE6F2   ^ 7F DC                     JG SHORT PathOfEx.00CEE6D0
$+44     00CEE6F4     5F                        POP EDI
$+45     00CEE6F5     8BC6                      MOV EAX, ESI
$+47     00CEE6F7     5E                        POP ESI
$+48     00CEE6F8     C3                        RETN
$+49     00CEE6F9     CC                        INT3
*/
/**/
//tag_stCD_MySkillMgrArray偏移_28
/*
$-9A     008C5E0F      CC                   INT3
$-99     008C5E10      83EC 10              SUB ESP, 0x10
$-96     008C5E13      53                   PUSH EBX
$-95     008C5E14      8B5C24 18            MOV EBX, DWORD PTR SS:[ESP+0x18]
$-91     008C5E18      55                   PUSH EBP
$-90     008C5E19      56                   PUSH ESI
$-8F     008C5E1A      57                   PUSH EDI
$-8E     008C5E1B      8BE9                 MOV EBP, ECX
$-8C     008C5E1D      8BCB                 MOV ECX, EBX
$-8A     008C5E1F      6A 01                PUSH 0x1
$-88     008C5E21      896C24 20            MOV DWORD PTR SS:[ESP+0x20], EBP
$-84     008C5E25      E8 A6A3BDFF          CALL PathOfEx.004A01D0
$-7F     008C5E2A      8B53 08              MOV EDX, DWORD PTR DS:[EBX+0x8]
$-7C     008C5E2D      8BCB                 MOV ECX, EBX
$-7A     008C5E2F      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$-78     008C5E31      6A 01                PUSH 0x1
$-76     008C5E33      8A0402               MOV AL, BYTE PTR DS:[EDX+EAX]
$-73     008C5E36      884424 28            MOV BYTE PTR SS:[ESP+0x28], AL
$-6F     008C5E3A      884424 18            MOV BYTE PTR SS:[ESP+0x18], AL
$-6B     008C5E3E      8D42 01              LEA EAX, DWORD PTR DS:[EDX+0x1]
$-68     008C5E41      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$-65     008C5E44      E8 87A3BDFF          CALL PathOfEx.004A01D0
$-60     008C5E49      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$-5D     008C5E4C      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$-5B     008C5E4E      8A1401               MOV DL, BYTE PTR DS:[ECX+EAX]
$-58     008C5E51      8D41 01              LEA EAX, DWORD PTR DS:[ECX+0x1]
$-55     008C5E54      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$-52     008C5E57      8A4424 24            MOV AL, BYTE PTR SS:[ESP+0x24]
$-4E     008C5E5B      885424 13            MOV BYTE PTR SS:[ESP+0x13], DL
$-4A     008C5E5F      885424 18            MOV BYTE PTR SS:[ESP+0x18], DL
$-46     008C5E63      84C0                 TEST AL, AL
$-44     008C5E65      75 04                JNZ SHORT PathOfEx.008C5E6B
$-42     008C5E67      84D2                 TEST DL, DL
$-40     008C5E69      74 56                JE SHORT PathOfEx.008C5EC1
$-3E     008C5E6B      8B75 28              MOV ESI, DWORD PTR SS:[EBP+0x28]
$-3B     008C5E6E      3B75 2C              CMP ESI, DWORD PTR SS:[EBP+0x2C]                  ; PathOfEx.00441BAE
$-38     008C5E71      74 13                JE SHORT PathOfEx.008C5E86
$-36     008C5E73      8B0E                 MOV ECX, DWORD PTR DS:[ESI]
$-34     008C5E75      55                   PUSH EBP
$-33     008C5E76      8B01                 MOV EAX, DWORD PTR DS:[ECX]
$-31     008C5E78      FF10                 CALL NEAR DWORD PTR DS:[EAX]
$-2F     008C5E7A      83C6 04              ADD ESI, 0x4
$-2C     008C5E7D      3B75 2C              CMP ESI, DWORD PTR SS:[EBP+0x2C]                  ; PathOfEx.00441BAE
$-29     008C5E80    ^ 75 F1                JNZ SHORT PathOfEx.008C5E73
$-27     008C5E82      8A4424 24            MOV AL, BYTE PTR SS:[ESP+0x24]
$-23     008C5E86      84C0                 TEST AL, AL
$-21     008C5E88      74 0C                JE SHORT PathOfEx.008C5E96
$-1F     008C5E8A      FF7424 14            PUSH DWORD PTR SS:[ESP+0x14]
$-1B     008C5E8E      8BCD                 MOV ECX, EBP
$-19     008C5E90      53                   PUSH EBX
$-18     008C5E91      E8 2AF8FFFF          CALL PathOfEx.008C56C0
$-13     008C5E96      807C24 13 00         CMP BYTE PTR SS:[ESP+0x13], 0x0
$-E      008C5E9B      74 0C                JE SHORT PathOfEx.008C5EA9
$-C      008C5E9D      FF7424 18            PUSH DWORD PTR SS:[ESP+0x18]
$-8      008C5EA1      8BCD                 MOV ECX, EBP
$-6      008C5EA3      53                   PUSH EBX
$-5      008C5EA4      E8 07FEFFFF          CALL PathOfEx.008C5CB0
$ ==>    008C5EA9      8B75 28              MOV ESI, DWORD PTR SS:[EBP+0x28]                  ; g_so_my_skill_mgr_array_get_skill_mgr_
$+3      008C5EAC      3B75 2C              CMP ESI, DWORD PTR SS:[EBP+0x2C]                  ; ebp=stCD_MySkillMgrArray*
$+6      008C5EAF      74 10                JE SHORT PathOfEx.008C5EC1                        ; ebp=this
$+8      008C5EB1      8B0E                 MOV ECX, DWORD PTR DS:[ESI]
$+A      008C5EB3      55                   PUSH EBP
$+B      008C5EB4      8B01                 MOV EAX, DWORD PTR DS:[ECX]
$+D      008C5EB6      FF50 04              CALL NEAR DWORD PTR DS:[EAX+0x4]
$+10     008C5EB9      83C6 04              ADD ESI, 0x4
$+13     008C5EBC      3B75 2C              CMP ESI, DWORD PTR SS:[EBP+0x2C]                  ; PathOfEx.00441BAE
$+16     008C5EBF    ^ 75 F0                JNZ SHORT PathOfEx.008C5EB1
$+18     008C5EC1      6A 01                PUSH 0x1
$+1A     008C5EC3      8BCB                 MOV ECX, EBX
$+1C     008C5EC5      E8 06A3BDFF          CALL PathOfEx.004A01D0
$+21     008C5ECA      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$+24     008C5ECD      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$+26     008C5ECF      8A1401               MOV DL, BYTE PTR DS:[ECX+EAX]
$+29     008C5ED2      8D41 01              LEA EAX, DWORD PTR DS:[ECX+0x1]
$+2C     008C5ED5      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$+2F     008C5ED8      0FB6C2               MOVZX EAX, DL
$+32     008C5EDB      894424 24            MOV DWORD PTR SS:[ESP+0x24], EAX
$+36     008C5EDF      84D2                 TEST DL, DL
$+38     008C5EE1      0F84 88000000        JE PathOfEx.008C5F6F
$+3E     008C5EE7      6A 02                PUSH 0x2
$+40     008C5EE9      8BCB                 MOV ECX, EBX
$+42     008C5EEB      E8 E0A2BDFF          CALL PathOfEx.004A01D0
$+47     008C5EF0      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$+4A     008C5EF3      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$+4C     008C5EF5      51                   PUSH ECX
$+4D     008C5EF6      0FB73C01             MOVZX EDI, WORD PTR DS:[ECX+EAX]
$+51     008C5EFA      8D41 02              LEA EAX, DWORD PTR DS:[ECX+0x2]
$+54     008C5EFD      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$+57     008C5F00      8BC7                 MOV EAX, EDI
$+59     008C5F02      8B75 14              MOV ESI, DWORD PTR SS:[EBP+0x14]
$+5C     008C5F05      8BD6                 MOV EDX, ESI
$+5E     008C5F07      8B4D 10              MOV ECX, DWORD PTR SS:[EBP+0x10]
$+61     008C5F0A      894424 1C            MOV DWORD PTR SS:[ESP+0x1C], EAX
$+65     008C5F0E      8D4424 1C            LEA EAX, DWORD PTR SS:[ESP+0x1C]
$+69     008C5F12      50                   PUSH EAX
$+6A     008C5F13      E8 98C7F4FF          CALL PathOfEx.008126B0
$+6F     008C5F18      83C4 08              ADD ESP, 0x8
$+72     008C5F1B      3BC6                 CMP EAX, ESI
$+74     008C5F1D      74 07                JE SHORT PathOfEx.008C5F26
$+76     008C5F1F      8B08                 MOV ECX, DWORD PTR DS:[EAX]
$+78     008C5F21      66:3B39              CMP DI, WORD PTR DS:[ECX]
$+7B     008C5F24      73 02                JNB SHORT PathOfEx.008C5F28
$+7D     008C5F26      8BC6                 MOV EAX, ESI
$+7F     008C5F28      8B00                 MOV EAX, DWORD PTR DS:[EAX]
$+81     008C5F2A      8BCB                 MOV ECX, EBX
$+83     008C5F2C      8B40 40              MOV EAX, DWORD PTR DS:[EAX+0x40]
$+86     008C5F2F      894424 18            MOV DWORD PTR SS:[ESP+0x18], EAX
$+8A     008C5F33      E8 4851E9FF          CALL PathOfEx.0075B080
$+8F     008C5F38      8BF8                 MOV EDI, EAX
$+91     008C5F3A      85FF                 TEST EDI, EDI
$+93     008C5F3C      74 26                JE SHORT PathOfEx.008C5F64
$+95     008C5F3E      8B6C24 18            MOV EBP, DWORD PTR SS:[ESP+0x18]
$+99     008C5F42      8BCB                 MOV ECX, EBX
$+9B     008C5F44      E8 3751E9FF          CALL PathOfEx.0075B080
$+A0     008C5F49      8BCB                 MOV ECX, EBX
$+A2     008C5F4B      8BF0                 MOV ESI, EAX
$+A4     008C5F4D      E8 AE52E9FF          CALL PathOfEx.0075B200
$+A9     008C5F52      50                   PUSH EAX
$+AA     008C5F53      56                   PUSH ESI
$+AB     008C5F54      8BCD                 MOV ECX, EBP
$+AD     008C5F56      E8 85090200          CALL PathOfEx.008E68E0
$+B2     008C5F5B      83EF 01              SUB EDI, 0x1
$+B5     008C5F5E    ^ 75 E2                JNZ SHORT PathOfEx.008C5F42
$+B7     008C5F60      8B6C24 1C            MOV EBP, DWORD PTR SS:[ESP+0x1C]
$+BB     008C5F64      836C24 24 01         SUB DWORD PTR SS:[ESP+0x24], 0x1
$+C0     008C5F69    ^ 0F85 78FFFFFF        JNZ PathOfEx.008C5EE7
$+C6     008C5F6F      6A 01                PUSH 0x1
$+C8     008C5F71      8BCB                 MOV ECX, EBX
$+CA     008C5F73      E8 58A2BDFF          CALL PathOfEx.004A01D0
$+CF     008C5F78      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$+D2     008C5F7B      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$+D4     008C5F7D      8A1401               MOV DL, BYTE PTR DS:[ECX+EAX]
$+D7     008C5F80      8D41 01              LEA EAX, DWORD PTR DS:[ECX+0x1]
$+DA     008C5F83      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$+DD     008C5F86      0FB6C2               MOVZX EAX, DL
$+E0     008C5F89      894424 24            MOV DWORD PTR SS:[ESP+0x24], EAX
$+E4     008C5F8D      84D2                 TEST DL, DL
$+E6     008C5F8F      74 73                JE SHORT PathOfEx.008C6004
$+E8     008C5F91      6A 02                PUSH 0x2
$+EA     008C5F93      8BCB                 MOV ECX, EBX
$+EC     008C5F95      E8 36A2BDFF          CALL PathOfEx.004A01D0
$+F1     008C5F9A      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$+F4     008C5F9D      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$+F6     008C5F9F      51                   PUSH ECX
$+F7     008C5FA0      0FB71401             MOVZX EDX, WORD PTR DS:[ECX+EAX]
$+FB     008C5FA4      8D41 02              LEA EAX, DWORD PTR DS:[ECX+0x2]
$+FE     008C5FA7      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$+101    008C5FAA      8B7D 14              MOV EDI, DWORD PTR SS:[EBP+0x14]
$+104    008C5FAD      8B4D 10              MOV ECX, DWORD PTR SS:[EBP+0x10]
$+107    008C5FB0      0FB7C2               MOVZX EAX, DX
$+10A    008C5FB3      894424 20            MOV DWORD PTR SS:[ESP+0x20], EAX
$+10E    008C5FB7      8D4424 20            LEA EAX, DWORD PTR SS:[ESP+0x20]
$+112    008C5FBB      895424 1C            MOV DWORD PTR SS:[ESP+0x1C], EDX
$+116    008C5FBF      8BD7                 MOV EDX, EDI
$+118    008C5FC1      50                   PUSH EAX
$+119    008C5FC2      E8 E9C6F4FF          CALL PathOfEx.008126B0
$+11E    008C5FC7      8BF0                 MOV ESI, EAX
$+120    008C5FC9      83C4 08              ADD ESP, 0x8
$+123    008C5FCC      3BF7                 CMP ESI, EDI
$+125    008C5FCE      74 0B                JE SHORT PathOfEx.008C5FDB
$+127    008C5FD0      8B06                 MOV EAX, DWORD PTR DS:[ESI]
$+129    008C5FD2      8B4C24 18            MOV ECX, DWORD PTR SS:[ESP+0x18]
$+12D    008C5FD6      66:3B08              CMP CX, WORD PTR DS:[EAX]
$+130    008C5FD9      73 02                JNB SHORT PathOfEx.008C5FDD
$+132    008C5FDB      8BF7                 MOV ESI, EDI
$+134    008C5FDD      6A 04                PUSH 0x4
$+136    008C5FDF      8BCB                 MOV ECX, EBX
$+138    008C5FE1      E8 EAA1BDFF          CALL PathOfEx.004A01D0
$+13D    008C5FE6      836C24 24 01         SUB DWORD PTR SS:[ESP+0x24], 0x1
$+142    008C5FEB      8B4B 08              MOV ECX, DWORD PTR DS:[EBX+0x8]
$+145    008C5FEE      8B03                 MOV EAX, DWORD PTR DS:[EBX]
$+147    008C5FF0      8B1401               MOV EDX, DWORD PTR DS:[ECX+EAX]
$+14A    008C5FF3      8D41 04              LEA EAX, DWORD PTR DS:[ECX+0x4]
$+14D    008C5FF6      8943 08              MOV DWORD PTR DS:[EBX+0x8], EAX
$+150    008C5FF9      8B06                 MOV EAX, DWORD PTR DS:[ESI]
$+152    008C5FFB      8950 3C              MOV DWORD PTR DS:[EAX+0x3C], EDX
$+155    008C5FFE      C640 50 01           MOV BYTE PTR DS:[EAX+0x50], 0x1
$+159    008C6002    ^ 75 8D                JNZ SHORT PathOfEx.008C5F91
$+15B    008C6004      5F                   POP EDI
$+15C    008C6005      5E                   POP ESI
$+15D    008C6006      5D                   POP EBP
$+15E    008C6007      5B                   POP EBX
$+15F    008C6008      83C4 10              ADD ESP, 0x10
$+162    008C600B      C2 0400              RETN 0x4
$+165    008C600E      CC                   INT3
*/
/**/
//tag_点亮该技能

//call tag_是否可点亮该技能

//tag_判断是否满足需求

//call tag_点亮该技能
/*
$-321    0080892F      CC                         INT3
$-320    00808930      55                         PUSH EBP                                                 ; tag_点亮该技能,成功返回true
$-31F    00808931      8BEC                       MOV EBP,ESP
$-31D    00808933      6A FF                      PUSH -0x1
$-31B    00808935      68 20A3D500                PUSH PathOfEx.00D5A320
$-316    0080893A      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$-310    00808940      50                         PUSH EAX
$-30F    00808941      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$-308    00808948      81EC 90010000              SUB ESP,0x190
$-302    0080894E      56                         PUSH ESI
$-301    0080894F      57                         PUSH EDI
$-300    00808950      C745 E8 00000000           MOV DWORD PTR SS:[EBP-0x18],0x0
$-2F9    00808957      8BF1                       MOV ESI,ECX
$-2F7    00808959      8B7D 08                    MOV EDI,DWORD PTR SS:[EBP+0x8]
$-2F4    0080895C      57                         PUSH EDI
$-2F3    0080895D      E8 AE0A0000                CALL PathOfEx.00809410                                   ; call tag_是否可点亮该技能
$-2EE    00808962      84C0                       TEST AL,AL
$-2EC    00808964      0F85 52010000              JNZ PathOfEx.00808ABC
$-2E6    0080896A      C785 14FFFFFF ECE8EA00     MOV DWORD PTR SS:[EBP-0xEC],PathOfEx.00EAE8EC
$-2DC    00808974      C785 24FFFFFF 98E7EA00     MOV DWORD PTR SS:[EBP-0xDC],PathOfEx.00EAE798
$-2D2    0080897E      C785 7CFFFFFF 4C54DE00     MOV DWORD PTR SS:[EBP-0x84],PathOfEx.00DE544C
$-2C8    00808988      51                         PUSH ECX
$-2C7    00808989      8D85 2CFFFFFF              LEA EAX,DWORD PTR SS:[EBP-0xD4]
$-2C1    0080898F      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$-2BA    00808996      50                         PUSH EAX
$-2B9    00808997      8D8D 14FFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xEC]
$-2B3    0080899D      C745 E8 01000000           MOV DWORD PTR SS:[EBP-0x18],0x1
$-2AC    008089A4      E8 07FBC6FF                CALL PathOfEx.004784B0
$-2A7    008089A9      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$-2A0    008089B0      8B85 14FFFFFF              MOV EAX,DWORD PTR SS:[EBP-0xEC]
$-29A    008089B6      6A 03                      PUSH 0x3
$-298    008089B8      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$-295    008089BB      C78405 14FFFFFF 64FCE600   MOV DWORD PTR SS:[EBP+EAX-0xEC],PathOfEx.00E6FC64
$-28A    008089C6      8B85 14FFFFFF              MOV EAX,DWORD PTR SS:[EBP-0xEC]
$-284    008089CC      8B48 04                    MOV ECX,DWORD PTR DS:[EAX+0x4]
$-281    008089CF      8D41 98                    LEA EAX,DWORD PTR DS:[ECX-0x68]
$-27E    008089D2      89840D 10FFFFFF            MOV DWORD PTR SS:[EBP+ECX-0xF0],EAX
$-277    008089D9      8D8D 2CFFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xD4]
$-271    008089DF      E8 AC00C7FF                CALL PathOfEx.00478A90
$-26C    008089E4      C745 FC 02000000           MOV DWORD PTR SS:[EBP-0x4],0x2
$-265    008089EB      8B47 04                    MOV EAX,DWORD PTR DS:[EDI+0x4]
$-262    008089EE      68 D4E5EE00                PUSH PathOfEx.00EEE5D4                                   ; ASCII ", but CanAllocate check failed."
$-25D    008089F3      51                         PUSH ECX
$-25C    008089F4      FF30                       PUSH DWORD PTR DS:[EAX]
$-25A    008089F6      8D85 24FFFFFF              LEA EAX,DWORD PTR SS:[EBP-0xDC]
$-254    008089FC      51                         PUSH ECX
$-253    008089FD      68 98E5EE00                PUSH PathOfEx.00EEE598                                   ; ASCII "Trying to allocate passive skill point named: "
$-24E    00808A02      50                         PUSH EAX
$-24D    00808A03      E8 387FD2FF                CALL PathOfEx.00530940
$-248    00808A08      83C4 0C                    ADD ESP,0xC
$-245    00808A0B      50                         PUSH EAX
$-244    00808A0C      E8 FFF9C7FF                CALL PathOfEx.00488410
$-23F    00808A11      83C4 0C                    ADD ESP,0xC
$-23C    00808A14      50                         PUSH EAX
$-23B    00808A15      E8 267FD2FF                CALL PathOfEx.00530940
$-236    00808A1A      83C4 08                    ADD ESP,0x8
$-233    00808A1D      8D45 C4                    LEA EAX,DWORD PTR SS:[EBP-0x3C]
$-230    00808A20      8D8D 14FFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xEC]
$-22A    00808A26      50                         PUSH EAX
$-229    00808A27      E8 C4F9C7FF                CALL PathOfEx.004883F0
$-224    00808A2C      8BF0                       MOV ESI,EAX
$-222    00808A2E      C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4],0x3
$-21E    00808A32      FF15 C8E0D800              CALL DWORD PTR DS:[0xD8E0C8]                             ; kernel32.GetTickCount
$-218    00808A38      8BF8                       MOV EDI,EAX
$-216    00808A3A      8D45 E4                    LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-213    00808A3D      50                         PUSH EAX
$-212    00808A3E      E8 84234A00                CALL PathOfEx.00CAADC7
$-20D    00808A43      64:8B0D 2C000000           MOV ECX,DWORD PTR FS:[0x2C]
$-206    00808A4A      83C4 04                    ADD ESP,0x4
$-203    00808A4D      8B09                       MOV ECX,DWORD PTR DS:[ECX]
$-201    00808A4F      56                         PUSH ESI
$-200    00808A50      83EC 0C                    SUB ESP,0xC
$-1FD    00808A53      8B49 18                    MOV ECX,DWORD PTR DS:[ECX+0x18]
$-1FA    00808A56      6A 00                      PUSH 0x0
$-1F8    00808A58      57                         PUSH EDI
$-1F7    00808A59      FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
$-1F4    00808A5C      FF75 E4                    PUSH DWORD PTR SS:[EBP-0x1C]
$-1F1    00808A5F      6A 71                      PUSH 0x71
$-1EF    00808A61      E8 2AF6C7FF                CALL PathOfEx.00488090
$-1EA    00808A66      8B45 D8                    MOV EAX,DWORD PTR SS:[EBP-0x28]
$-1E7    00808A69      83F8 08                    CMP EAX,0x8
$-1E4    00808A6C      72 0F                      JB SHORT PathOfEx.00808A7D
$-1E2    00808A6E      40                         INC EAX
$-1E1    00808A6F      6A 02                      PUSH 0x2
$-1DF    00808A71      50                         PUSH EAX
$-1DE    00808A72      FF75 C4                    PUSH DWORD PTR SS:[EBP-0x3C]
$-1DB    00808A75      E8 B63DC6FF                CALL PathOfEx.0046C830
$-1D6    00808A7A      83C4 0C                    ADD ESP,0xC
$-1D3    00808A7D      8D8D 7CFFFFFF              LEA ECX,DWORD PTR SS:[EBP-0x84]
$-1CD    00808A83      E8 E8E2C6FF                CALL PathOfEx.00476D70
$-1C8    00808A88      8D85 7CFFFFFF              LEA EAX,DWORD PTR SS:[EBP-0x84]
$-1C2    00808A8E      C745 FC 04000000           MOV DWORD PTR SS:[EBP-0x4],0x4
$-1BB    00808A95      50                         PUSH EAX
$-1BA    00808A96      C785 7CFFFFFF C453DE00     MOV DWORD PTR SS:[EBP-0x84],PathOfEx.00DE53C4
$-1B0    00808AA0      E8 4C1A4600                CALL PathOfEx.00C6A4F1
$-1AB    00808AA5      83C4 04                    ADD ESP,0x4
$-1A8    00808AA8      32C0                       XOR AL,AL
$-1A6    00808AAA      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1A3    00808AAD      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$-19C    00808AB4      5F                         POP EDI
$-19B    00808AB5      5E                         POP ESI
$-19A    00808AB6      8BE5                       MOV ESP,EBP
$-198    00808AB8      5D                         POP EBP
$-197    00808AB9      C2 0400                    RETN 0x4
$-194    00808ABC      8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
$-191    00808ABF      85C9                       TEST ECX,ECX
$-18F    00808AC1      74 05                      JE SHORT PathOfEx.00808AC8
$-18D    00808AC3      E8 F80AF1FF                CALL PathOfEx.007195C0
$-188    00808AC8      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$-185    00808ACB      57                         PUSH EDI
$-184    00808ACC      8B48 30                    MOV ECX,DWORD PTR DS:[EAX+0x30]
$-181    00808ACF      8D45 08                    LEA EAX,DWORD PTR SS:[EBP+0x8]
$-17E    00808AD2      50                         PUSH EAX
$-17D    00808AD3      83C1 0C                    ADD ECX,0xC
$-17A    00808AD6      E8 85BECEFF                CALL PathOfEx.004F4960
$-175    00808ADB      57                         PUSH EDI
$-174    00808ADC      51                         PUSH ECX
$-173    00808ADD      8BCE                       MOV ECX,ESI
$-171    00808ADF      E8 FC100000                CALL PathOfEx.00809BE0
$-16C    00808AE4      C785 64FEFFFF ECE8EA00     MOV DWORD PTR SS:[EBP-0x19C],PathOfEx.00EAE8EC
$-162    00808AEE      C785 74FEFFFF 98E7EA00     MOV DWORD PTR SS:[EBP-0x18C],PathOfEx.00EAE798
$-158    00808AF8      C785 CCFEFFFF 4C54DE00     MOV DWORD PTR SS:[EBP-0x134],PathOfEx.00DE544C
$-14E    00808B02      51                         PUSH ECX
$-14D    00808B03      8D85 7CFEFFFF              LEA EAX,DWORD PTR SS:[EBP-0x184]
$-147    00808B09      C745 FC 05000000           MOV DWORD PTR SS:[EBP-0x4],0x5
$-140    00808B10      50                         PUSH EAX
$-13F    00808B11      8D8D 64FEFFFF              LEA ECX,DWORD PTR SS:[EBP-0x19C]
$-139    00808B17      C745 E8 02000000           MOV DWORD PTR SS:[EBP-0x18],0x2
$-132    00808B1E      E8 8DF9C6FF                CALL PathOfEx.004784B0
$-12D    00808B23      C745 FC 06000000           MOV DWORD PTR SS:[EBP-0x4],0x6
$-126    00808B2A      8B85 64FEFFFF              MOV EAX,DWORD PTR SS:[EBP-0x19C]
$-120    00808B30      6A 03                      PUSH 0x3
$-11E    00808B32      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$-11B    00808B35      C78405 64FEFFFF 64FCE600   MOV DWORD PTR SS:[EBP+EAX-0x19C],PathOfEx.00E6FC64
$-110    00808B40      8B85 64FEFFFF              MOV EAX,DWORD PTR SS:[EBP-0x19C]
$-10A    00808B46      8B48 04                    MOV ECX,DWORD PTR DS:[EAX+0x4]
$-107    00808B49      8D41 98                    LEA EAX,DWORD PTR DS:[ECX-0x68]
$-104    00808B4C      89840D 60FEFFFF            MOV DWORD PTR SS:[EBP+ECX-0x1A0],EAX
$-FD     00808B53      8D8D 7CFEFFFF              LEA ECX,DWORD PTR SS:[EBP-0x184]
$-F7     00808B59      E8 32FFC6FF                CALL PathOfEx.00478A90
$-F2     00808B5E      C745 FC 07000000           MOV DWORD PTR SS:[EBP-0x4],0x7
$-EB     00808B65      8B47 04                    MOV EAX,DWORD PTR DS:[EDI+0x4]
$-E8     00808B68      FF70 24                    PUSH DWORD PTR DS:[EAX+0x24]
$-E5     00808B6B      51                         PUSH ECX
$-E4     00808B6C      68 C8E5EE00                PUSH PathOfEx.00EEE5C8                                   ; ASCII ", name: "
$-DF     00808B71      51                         PUSH ECX
$-DE     00808B72      FF30                       PUSH DWORD PTR DS:[EAX]
$-DC     00808B74      8D85 74FEFFFF              LEA EAX,DWORD PTR SS:[EBP-0x18C]
$-D6     00808B7A      51                         PUSH ECX
$-D5     00808B7B      68 44E6EE00                PUSH PathOfEx.00EEE644                                   ; ASCII "Successfully allocated passive skill id: "
$-D0     00808B80      50                         PUSH EAX
$-CF     00808B81      E8 BA7DD2FF                CALL PathOfEx.00530940
$-CA     00808B86      83C4 0C                    ADD ESP,0xC
$-C7     00808B89      50                         PUSH EAX
$-C6     00808B8A      E8 81F8C7FF                CALL PathOfEx.00488410
$-C1     00808B8F      83C4 0C                    ADD ESP,0xC
$-BE     00808B92      50                         PUSH EAX
$-BD     00808B93      E8 A87DD2FF                CALL PathOfEx.00530940
$-B8     00808B98      83C4 0C                    ADD ESP,0xC
$-B5     00808B9B      50                         PUSH EAX
$-B4     00808B9C      E8 6FF8C7FF                CALL PathOfEx.00488410
$-AF     00808BA1      83C4 08                    ADD ESP,0x8
$-AC     00808BA4      8D45 C4                    LEA EAX,DWORD PTR SS:[EBP-0x3C]
$-A9     00808BA7      8D8D 64FEFFFF              LEA ECX,DWORD PTR SS:[EBP-0x19C]
$-A3     00808BAD      50                         PUSH EAX
$-A2     00808BAE      E8 3DF8C7FF                CALL PathOfEx.004883F0
$-9D     00808BB3      8BF0                       MOV ESI,EAX
$-9B     00808BB5      C645 FC 08                 MOV BYTE PTR SS:[EBP-0x4],0x8
$-97     00808BB9      FF15 C8E0D800              CALL DWORD PTR DS:[0xD8E0C8]                             ; kernel32.GetTickCount
$-91     00808BBF      8BF8                       MOV EDI,EAX
$-8F     00808BC1      8D45 DC                    LEA EAX,DWORD PTR SS:[EBP-0x24]
$-8C     00808BC4      50                         PUSH EAX
$-8B     00808BC5      E8 FD214A00                CALL PathOfEx.00CAADC7
$-86     00808BCA      64:8B0D 2C000000           MOV ECX,DWORD PTR FS:[0x2C]
$-7F     00808BD1      83C4 04                    ADD ESP,0x4
$-7C     00808BD4      8B09                       MOV ECX,DWORD PTR DS:[ECX]
$-7A     00808BD6      56                         PUSH ESI
$-79     00808BD7      83EC 0C                    SUB ESP,0xC
$-76     00808BDA      8B49 18                    MOV ECX,DWORD PTR DS:[ECX+0x18]
$-73     00808BDD      6A 00                      PUSH 0x0
$-71     00808BDF      57                         PUSH EDI
$-70     00808BE0      FF75 E0                    PUSH DWORD PTR SS:[EBP-0x20]
$-6D     00808BE3      FF75 DC                    PUSH DWORD PTR SS:[EBP-0x24]
$-6A     00808BE6      6A 7D                      PUSH 0x7D
$-68     00808BE8      E8 A3F4C7FF                CALL PathOfEx.00488090
$-63     00808BED      8B45 D8                    MOV EAX,DWORD PTR SS:[EBP-0x28]
$-60     00808BF0      83F8 08                    CMP EAX,0x8
$-5D     00808BF3      72 0F                      JB SHORT PathOfEx.00808C04
$-5B     00808BF5      40                         INC EAX
$-5A     00808BF6      6A 02                      PUSH 0x2
$-58     00808BF8      50                         PUSH EAX
$-57     00808BF9      FF75 C4                    PUSH DWORD PTR SS:[EBP-0x3C]
$-54     00808BFC      E8 2F3CC6FF                CALL PathOfEx.0046C830
$-4F     00808C01      83C4 0C                    ADD ESP,0xC
$-4C     00808C04      8D8D CCFEFFFF              LEA ECX,DWORD PTR SS:[EBP-0x134]
$-46     00808C0A      E8 61E1C6FF                CALL PathOfEx.00476D70
$-41     00808C0F      8D85 CCFEFFFF              LEA EAX,DWORD PTR SS:[EBP-0x134]
$-3B     00808C15      C745 FC 09000000           MOV DWORD PTR SS:[EBP-0x4],0x9
$-34     00808C1C      50                         PUSH EAX
$-33     00808C1D      C785 CCFEFFFF C453DE00     MOV DWORD PTR SS:[EBP-0x134],PathOfEx.00DE53C4
$-29     00808C27      E8 C5184600                CALL PathOfEx.00C6A4F1
$-24     00808C2C      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$-21     00808C2F      83C4 04                    ADD ESP,0x4
$-1E     00808C32      B0 01                      MOV AL,0x1
$-1C     00808C34      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$-15     00808C3B      5F                         POP EDI
$-14     00808C3C      5E                         POP ESI
$-13     00808C3D      8BE5                       MOV ESP,EBP
$-11     00808C3F      5D                         POP EBP
$-10     00808C40      C2 0400                    RETN 0x4
$-D      00808C43      CC                         INT3
$-C      00808C44      CC                         INT3
$-B      00808C45      CC                         INT3
$-A      00808C46      CC                         INT3
$-9      00808C47      CC                         INT3
$-8      00808C48      CC                         INT3
$-7      00808C49      CC                         INT3
$-6      00808C4A      CC                         INT3
$-5      00808C4B      CC                         INT3
$-4      00808C4C      CC                         INT3
$-3      00808C4D      CC                         INT3
$-2      00808C4E      CC                         INT3
$-1      00808C4F      CC                         INT3
$ ==>    00808C50      55                         PUSH EBP                                                 ; tag_判断是否满足需求
$+1      00808C51      8BEC                       MOV EBP,ESP                                              ; ecx=stCD_PassiveSkillGraphMgr*
$+3      00808C53      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]                                 ; p1=skill_id
$+9      00808C59      6A FF                      PUSH -0x1
$+B      00808C5B      68 5E8BD200                PUSH PathOfEx.00D28B5E
$+10     00808C60      50                         PUSH EAX
$+11     00808C61      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+18     00808C68      83EC 10                    SUB ESP,0x10
$+1B     00808C6B      53                         PUSH EBX
$+1C     00808C6C      56                         PUSH ESI
$+1D     00808C6D      8BF1                       MOV ESI,ECX
$+1F     00808C6F      8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]                           ; then,ecx=stCD_DbPassiveSkillGraph*
$+22     00808C72      85C9                       TEST ECX,ECX
$+24     00808C74      74 05                      JE SHORT PathOfEx.00808C7B
$+26     00808C76      E8 4509F1FF                CALL PathOfEx.007195C0                                   ; nothing
$+2B     00808C7B      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+2E     00808C7E      8D4D E8                    LEA ECX,DWORD PTR SS:[EBP-0x18]
$+31     00808C81      FF75 08                    PUSH DWORD PTR SS:[EBP+0x8]                              ; skill_id
$+34     00808C84      51                         PUSH ECX                                                 ; ecx=stCD_TblItemPassiveSkillPair*
$+35     00808C85      8B48 30                    MOV ECX,DWORD PTR DS:[EAX+0x30]                          ; then,ecx=stCD_DbTblPassiveSkillGraph*
$+38     00808C88      E8 A3870000                CALL PathOfEx.00811430                                   ; 查找返回stCD_TblItemPassiveSkillPair
$+3D     00808C8D      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+44     00808C94      837D EC 00                 CMP DWORD PTR SS:[EBP-0x14],0x0
$+48     00808C98      75 04                      JNZ SHORT PathOfEx.00808C9E
$+4A     00808C9A      32DB                       XOR BL,BL
$+4C     00808C9C      EB 0D                      JMP SHORT PathOfEx.00808CAB
$+4E     00808C9E      8D45 E8                    LEA EAX,DWORD PTR SS:[EBP-0x18]
$+51     00808CA1      8BCE                       MOV ECX,ESI
$+53     00808CA3      50                         PUSH EAX                                                 ; eax=stCD_TblItemPassiveSkillPair*
$+54     00808CA4      E8 87FCFFFF                CALL PathOfEx.00808930                                   ; call tag_点亮该技能,成功返回true
$+59     00808CA9      8AD8                       MOV BL,AL
$+5B     00808CAB      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+62     00808CB2      8B45 E8                    MOV EAX,DWORD PTR SS:[EBP-0x18]
$+65     00808CB5      85C0                       TEST EAX,EAX
$+67     00808CB7      74 11                      JE SHORT PathOfEx.00808CCA
$+69     00808CB9      50                         PUSH EAX
$+6A     00808CBA      51                         PUSH ECX
$+6B     00808CBB      E8 40F3F0FF                CALL PathOfEx.00718000
$+70     00808CC0      83C4 04                    ADD ESP,0x4
$+73     00808CC3      8BC8                       MOV ECX,EAX
$+75     00808CC5      E8 C6EEF0FF                CALL PathOfEx.00717B90
$+7A     00808CCA      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+7D     00808CCD      8AC3                       MOV AL,BL
$+7F     00808CCF      5E                         POP ESI
$+80     00808CD0      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+87     00808CD7      5B                         POP EBX
$+88     00808CD8      8BE5                       MOV ESP,EBP
$+8A     00808CDA      5D                         POP EBP
$+8B     00808CDB      C2 0400                    RETN 0x4
$+8E     00808CDE      CC                         INT3
$+8F     00808CDF      CC                         INT3
$+90     00808CE0      55                         PUSH EBP
$+91     00808CE1      8BEC                       MOV EBP,ESP
$+93     00808CE3      6A FF                      PUSH -0x1
$+95     00808CE5      68 6DA3D500                PUSH PathOfEx.00D5A36D
$+9A     00808CEA      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+A0     00808CF0      50                         PUSH EAX
$+A1     00808CF1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+A8     00808CF8      81EC D8000000              SUB ESP,0xD8
$+AE     00808CFE      56                         PUSH ESI
$+AF     00808CFF      57                         PUSH EDI
$+B0     00808D00      8B7D 08                    MOV EDI,DWORD PTR SS:[EBP+0x8]
$+B3     00808D03      8BF1                       MOV ESI,ECX
$+B5     00808D05      57                         PUSH EDI
$+B6     00808D06      C745 EC 00000000           MOV DWORD PTR SS:[EBP-0x14],0x0
$+BD     00808D0D      E8 CE5D0000                CALL PathOfEx.0080EAE0
$+C2     00808D12      84C0                       TEST AL,AL
$+C4     00808D14      75 12                      JNZ SHORT PathOfEx.00808D28
$+C6     00808D16      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+C9     00808D19      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+D0     00808D20      5F                         POP EDI
$+D1     00808D21      5E                         POP ESI
$+D2     00808D22      8BE5                       MOV ESP,EBP
$+D4     00808D24      5D                         POP EBP
$+D5     00808D25      C2 0400                    RETN 0x4
$+D8     00808D28      57                         PUSH EDI
$+D9     00808D29      E8 425F0000                CALL PathOfEx.0080EC70
$+DE     00808D2E      2946 48                    SUB DWORD PTR DS:[ESI+0x48],EAX
$+E1     00808D31      8BCE                       MOV ECX,ESI
$+E3     00808D33      57                         PUSH EDI
$+E4     00808D34      E8 C70F0000                CALL PathOfEx.00809D00
$+E9     00808D39      C785 1CFFFFFF ECE8EA00     MOV DWORD PTR SS:[EBP-0xE4],PathOfEx.00EAE8EC
$+F3     00808D43      C785 2CFFFFFF 98E7EA00     MOV DWORD PTR SS:[EBP-0xD4],PathOfEx.00EAE798
$+FD     00808D4D      C745 84 4C54DE00           MOV DWORD PTR SS:[EBP-0x7C],PathOfEx.00DE544C
$+104    00808D54      51                         PUSH ECX
$+105    00808D55      8D85 34FFFFFF              LEA EAX,DWORD PTR SS:[EBP-0xCC]
$+10B    00808D5B      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+112    00808D62      50                         PUSH EAX
$+113    00808D63      8D8D 1CFFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xE4]
$+119    00808D69      C745 EC 01000000           MOV DWORD PTR SS:[EBP-0x14],0x1
$+120    00808D70      E8 3BF7C6FF                CALL PathOfEx.004784B0
$+125    00808D75      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+12C    00808D7C      8B85 1CFFFFFF              MOV EAX,DWORD PTR SS:[EBP-0xE4]
$+132    00808D82      6A 03                      PUSH 0x3
$+134    00808D84      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$+137    00808D87      C78405 1CFFFFFF 64FCE600   MOV DWORD PTR SS:[EBP+EAX-0xE4],PathOfEx.00E6FC64
$+142    00808D92      8B85 1CFFFFFF              MOV EAX,DWORD PTR SS:[EBP-0xE4]
$+148    00808D98      8B48 04                    MOV ECX,DWORD PTR DS:[EAX+0x4]
$+14B    00808D9B      8D41 98                    LEA EAX,DWORD PTR DS:[ECX-0x68]
$+14E    00808D9E      89840D 18FFFFFF            MOV DWORD PTR SS:[EBP+ECX-0xE8],EAX
$+155    00808DA5      8D8D 34FFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xCC]
$+15B    00808DAB      E8 E0FCC6FF                CALL PathOfEx.00478A90
$+160    00808DB0      C745 FC 02000000           MOV DWORD PTR SS:[EBP-0x4],0x2
$+167    00808DB7      8B47 04                    MOV EAX,DWORD PTR DS:[EDI+0x4]
$+16A    00808DBA      FF70 24                    PUSH DWORD PTR DS:[EAX+0x24]
$+16D    00808DBD      51                         PUSH ECX
$+16E    00808DBE      68 C8E5EE00                PUSH PathOfEx.00EEE5C8                                   ; ASCII ", name: "
$+173    00808DC3      51                         PUSH ECX
$+174    00808DC4      FF30                       PUSH DWORD PTR DS:[EAX]
$+176    00808DC6      8D85 2CFFFFFF              LEA EAX,DWORD PTR SS:[EBP-0xD4]
$+17C    00808DCC      51                         PUSH ECX
$+17D    00808DCD      68 70E6EE00                PUSH PathOfEx.00EEE670                                   ; ASCII "Successfully unallocated passive skill id: "
$+182    00808DD2      50                         PUSH EAX
$+183    00808DD3      E8 687BD2FF                CALL PathOfEx.00530940
$+188    00808DD8      83C4 0C                    ADD ESP,0xC
$+18B    00808DDB      50                         PUSH EAX
$+18C    00808DDC      E8 2FF6C7FF                CALL PathOfEx.00488410
$+191    00808DE1      83C4 0C                    ADD ESP,0xC
$+194    00808DE4      50                         PUSH EAX
$+195    00808DE5      E8 567BD2FF                CALL PathOfEx.00530940
$+19A    00808DEA      83C4 0C                    ADD ESP,0xC
$+19D    00808DED      50                         PUSH EAX
$+19E    00808DEE      E8 1DF6C7FF                CALL PathOfEx.00488410
$+1A3    00808DF3      83C4 08                    ADD ESP,0x8
$+1A6    00808DF6      8D45 CC                    LEA EAX,DWORD PTR SS:[EBP-0x34]
$+1A9    00808DF9      8D8D 1CFFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xE4]
$+1AF    00808DFF      50                         PUSH EAX
$+1B0    00808E00      E8 EBF5C7FF                CALL PathOfEx.004883F0
$+1B5    00808E05      8BF0                       MOV ESI,EAX
$+1B7    00808E07      C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4],0x3
$+1BB    00808E0B      FF15 C8E0D800              CALL DWORD PTR DS:[0xD8E0C8]                             ; kernel32.GetTickCount
$+1C1    00808E11      8BF8                       MOV EDI,EAX
$+1C3    00808E13      8D45 E4                    LEA EAX,DWORD PTR SS:[EBP-0x1C]
$+1C6    00808E16      50                         PUSH EAX
$+1C7    00808E17      E8 AB1F4A00                CALL PathOfEx.00CAADC7
$+1CC    00808E1C      64:8B0D 2C000000           MOV ECX,DWORD PTR FS:[0x2C]
$+1D3    00808E23      83C4 04                    ADD ESP,0x4
$+1D6    00808E26      8B09                       MOV ECX,DWORD PTR DS:[ECX]
$+1D8    00808E28      56                         PUSH ESI
$+1D9    00808E29      83EC 0C                    SUB ESP,0xC
$+1DC    00808E2C      8B49 18                    MOV ECX,DWORD PTR DS:[ECX+0x18]
$+1DF    00808E2F      6A 00                      PUSH 0x0
$+1E1    00808E31      57                         PUSH EDI
$+1E2    00808E32      FF75 E8                    PUSH DWORD PTR SS:[EBP-0x18]
$+1E5    00808E35      FF75 E4                    PUSH DWORD PTR SS:[EBP-0x1C]
$+1E8    00808E38      68 94000000                PUSH 0x94
$+1ED    00808E3D      E8 4EF2C7FF                CALL PathOfEx.00488090
$+1F2    00808E42      8B45 E0                    MOV EAX,DWORD PTR SS:[EBP-0x20]
$+1F5    00808E45      83F8 08                    CMP EAX,0x8
$+1F8    00808E48      72 0F                      JB SHORT PathOfEx.00808E59
$+1FA    00808E4A      40                         INC EAX
$+1FB    00808E4B      6A 02                      PUSH 0x2
$+1FD    00808E4D      50                         PUSH EAX
$+1FE    00808E4E      FF75 CC                    PUSH DWORD PTR SS:[EBP-0x34]
$+201    00808E51      E8 DA39C6FF                CALL PathOfEx.0046C830
$+206    00808E56      83C4 0C                    ADD ESP,0xC
$+209    00808E59      8D4D 84                    LEA ECX,DWORD PTR SS:[EBP-0x7C]
$+20C    00808E5C      E8 0FDFC6FF                CALL PathOfEx.00476D70
$+211    00808E61      8D45 84                    LEA EAX,DWORD PTR SS:[EBP-0x7C]
$+214    00808E64      C745 FC 04000000           MOV DWORD PTR SS:[EBP-0x4],0x4
$+21B    00808E6B      50                         PUSH EAX
$+21C    00808E6C      C745 84 C453DE00           MOV DWORD PTR SS:[EBP-0x7C],PathOfEx.00DE53C4
$+223    00808E73      E8 79164600                CALL PathOfEx.00C6A4F1
$+228    00808E78      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+22B    00808E7B      83C4 04                    ADD ESP,0x4
$+22E    00808E7E      B0 01                      MOV AL,0x1
$+230    00808E80      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+237    00808E87      5F                         POP EDI
$+238    00808E88      5E                         POP ESI
$+239    00808E89      8BE5                       MOV ESP,EBP
$+23B    00808E8B      5D                         POP EBP
$+23C    00808E8C      C2 0400                    RETN 0x4
$+23F    00808E8F      CC                         INT3
$+240    00808E90      55                         PUSH EBP
$+241    00808E91      8BEC                       MOV EBP,ESP
$+243    00808E93      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+249    00808E99      6A FF                      PUSH -0x1
$+24B    00808E9B      68 5E8BD200                PUSH PathOfEx.00D28B5E
$+250    00808EA0      50                         PUSH EAX
$+251    00808EA1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+258    00808EA8      83EC 10                    SUB ESP,0x10
$+25B    00808EAB      53                         PUSH EBX
$+25C    00808EAC      56                         PUSH ESI
$+25D    00808EAD      8BF1                       MOV ESI,ECX
$+25F    00808EAF      8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
$+262    00808EB2      85C9                       TEST ECX,ECX
$+264    00808EB4      74 05                      JE SHORT PathOfEx.00808EBB
$+266    00808EB6      E8 0507F1FF                CALL PathOfEx.007195C0
$+26B    00808EBB      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+26E    00808EBE      8D4D E8                    LEA ECX,DWORD PTR SS:[EBP-0x18]
$+271    00808EC1      FF75 08                    PUSH DWORD PTR SS:[EBP+0x8]
$+274    00808EC4      51                         PUSH ECX
$+275    00808EC5      8B48 30                    MOV ECX,DWORD PTR DS:[EAX+0x30]
$+278    00808EC8      E8 63850000                CALL PathOfEx.00811430
$+27D    00808ECD      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+284    00808ED4      837D EC 00                 CMP DWORD PTR SS:[EBP-0x14],0x0
$+288    00808ED8      75 04                      JNZ SHORT PathOfEx.00808EDE
$+28A    00808EDA      32DB                       XOR BL,BL
$+28C    00808EDC      EB 0D                      JMP SHORT PathOfEx.00808EEB
$+28E    00808EDE      8D45 E8                    LEA EAX,DWORD PTR SS:[EBP-0x18]
$+291    00808EE1      8BCE                       MOV ECX,ESI
$+293    00808EE3      50                         PUSH EAX
$+294    00808EE4      E8 F7FDFFFF                CALL PathOfEx.00808CE0
$+299    00808EE9      8AD8                       MOV BL,AL
$+29B    00808EEB      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+2A2    00808EF2      8B45 E8                    MOV EAX,DWORD PTR SS:[EBP-0x18]
$+2A5    00808EF5      85C0                       TEST EAX,EAX
$+2A7    00808EF7      74 11                      JE SHORT PathOfEx.00808F0A
$+2A9    00808EF9      50                         PUSH EAX
$+2AA    00808EFA      51                         PUSH ECX
$+2AB    00808EFB      E8 00F1F0FF                CALL PathOfEx.00718000
$+2B0    00808F00      83C4 04                    ADD ESP,0x4
$+2B3    00808F03      8BC8                       MOV ECX,EAX
$+2B5    00808F05      E8 86ECF0FF                CALL PathOfEx.00717B90
$+2BA    00808F0A      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+2BD    00808F0D      8AC3                       MOV AL,BL
$+2BF    00808F0F      5E                         POP ESI
$+2C0    00808F10      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+2C7    00808F17      5B                         POP EBX
$+2C8    00808F18      8BE5                       MOV ESP,EBP
$+2CA    00808F1A      5D                         POP EBP
$+2CB    00808F1B      C2 0400                    RETN 0x4
$+2CE    00808F1E      CC                         INT3
$+2CF    00808F1F      CC                         INT3
$+2D0    00808F20      56                         PUSH ESI
$+2D1    00808F21      8B7424 08                  MOV ESI,DWORD PTR SS:[ESP+0x8]
$+2D5    00808F25      8B16                       MOV EDX,DWORD PTR DS:[ESI]
$+2D7    00808F27      8911                       MOV DWORD PTR DS:[ECX],EDX
$+2D9    00808F29      C741 04 00000000           MOV DWORD PTR DS:[ECX+0x4],0x0
$+2E0    00808F30      C641 08 00                 MOV BYTE PTR DS:[ECX+0x8],0x0
$+2E4    00808F34      8B42 24                    MOV EAX,DWORD PTR DS:[EDX+0x24]
$+2E7    00808F37      8941 04                    MOV DWORD PTR DS:[ECX+0x4],EAX
$+2EA    00808F3A      3B42 28                    CMP EAX,DWORD PTR DS:[EDX+0x28]
$+2ED    00808F3D      75 0A                      JNZ SHORT PathOfEx.00808F49
$+2EF    00808F3F      C641 08 01                 MOV BYTE PTR DS:[ECX+0x8],0x1
$+2F3    00808F43      8B42 30                    MOV EAX,DWORD PTR DS:[EDX+0x30]
$+2F6    00808F46      8941 04                    MOV DWORD PTR DS:[ECX+0x4],EAX
$+2F9    00808F49      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+2FB    00808F4B      8941 0C                    MOV DWORD PTR DS:[ECX+0xC],EAX
$+2FE    00808F4E      C741 10 00000000           MOV DWORD PTR DS:[ECX+0x10],0x0
$+305    00808F55      C641 14 01                 MOV BYTE PTR DS:[ECX+0x14],0x1
$+309    00808F59      8B40 34                    MOV EAX,DWORD PTR DS:[EAX+0x34]
$+30C    00808F5C      8941 10                    MOV DWORD PTR DS:[ECX+0x10],EAX
$+30F    00808F5F      8BC1                       MOV EAX,ECX
$+311    00808F61      5E                         POP ESI
$+312    00808F62      C2 0400                    RETN 0x4
$+315    00808F65      CC                         INT3
*/
//tag_是否可点亮该技能
/*
$-1      0080940F      CC                         INT3
$ ==>    00809410      55                         PUSH EBP                                                 ; tag_是否可点亮该技能
$+1      00809411      8BEC                       MOV EBP,ESP                                              ; ecx=stCD_PassiveSkillGraphMgr*
$+3      00809413      6A FF                      PUSH -0x1                                                ; p1=stCD_TblItemPassiveSkillPair*
$+5      00809415      68 F9A3D500                PUSH PathOfEx.00D5A3F9
$+A      0080941A      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+10     00809420      50                         PUSH EAX
$+11     00809421      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+18     00809428      81EC A4000000              SUB ESP,0xA4
$+1E     0080942E      53                         PUSH EBX
$+1F     0080942F      56                         PUSH ESI
$+20     00809430      8B75 08                    MOV ESI,DWORD PTR SS:[EBP+0x8]
$+23     00809433      8D45 A0                    LEA EAX,DWORD PTR SS:[EBP-0x60]
$+26     00809436      57                         PUSH EDI
$+27     00809437      8BD9                       MOV EBX,ECX
$+29     00809439      C745 E8 00000000           MOV DWORD PTR SS:[EBP-0x18],0x0
$+30     00809440      50                         PUSH EAX
$+31     00809441      8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
$+34     00809444      895D E4                    MOV DWORD PTR SS:[EBP-0x1C],EBX                          ; store this
$+37     00809447      E8 147CD1FF                CALL PathOfEx.00521060                                   ; 查找stCD_UknTblItemPair*
$+3C     0080944C      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+43     00809453      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]                           ; then,eax=stCD_TblItemPassiveSkill*
$+46     00809456      8B7D A4                    MOV EDI,DWORD PTR SS:[EBP-0x5C]
$+49     00809459      8A40 72                    MOV AL,BYTE PTR DS:[EAX+0x72]
$+4C     0080945C      84C0                       TEST AL,AL
$+4E     0080945E      75 3B                      JNZ SHORT PathOfEx.0080949B
$+50     00809460      85FF                       TEST EDI,EDI
$+52     00809462      75 1E                      JNZ SHORT PathOfEx.00809482
$+54     00809464      8B43 5C                    MOV EAX,DWORD PTR DS:[EBX+0x5C]
$+57     00809467      0343 58                    ADD EAX,DWORD PTR DS:[EBX+0x58]
$+5A     0080946A      0343 50                    ADD EAX,DWORD PTR DS:[EBX+0x50]
$+5D     0080946D      8B4B 0C                    MOV ECX,DWORD PTR DS:[EBX+0xC]
$+60     00809470      0343 4C                    ADD EAX,DWORD PTR DS:[EBX+0x4C]
$+63     00809473      2B4B 08                    SUB ECX,DWORD PTR DS:[EBX+0x8]
$+66     00809476      0343 44                    ADD EAX,DWORD PTR DS:[EBX+0x44]
$+69     00809479      D1F9                       SAR ECX,1
$+6B     0080947B      2BC1                       SUB EAX,ECX
$+6D     0080947D      83E8 01                    SUB EAX,0x1
$+70     00809480      EB 13                      JMP SHORT PathOfEx.00809495                              ; 此时eax=剩余天赋点数
$+72     00809482      807B 40 40                 CMP BYTE PTR DS:[EBX+0x40],0x40
$+76     00809486      0F82 9C030000              JB PathOfEx.00809828
$+7C     0080948C      8B43 60                    MOV EAX,DWORD PTR DS:[EBX+0x60]
$+7F     0080948F      2B43 58                    SUB EAX,DWORD PTR DS:[EBX+0x58]
$+82     00809492      0343 54                    ADD EAX,DWORD PTR DS:[EBX+0x54]
$+85     00809495      0F84 8D030000              JE PathOfEx.00809828                                     ; 如果eax=0，则表示没有天赋点数了，则跳转
$+8B     0080949B      56                         PUSH ESI
$+8C     0080949C      8BCB                       MOV ECX,EBX
$+8E     0080949E      E8 CD030000                CALL PathOfEx.00809870                                   ; call tag_判断是否已点亮该天赋，是否已学过该技能
$+93     008094A3      84C0                       TEST AL,AL
$+95     008094A5      0F85 7D030000              JNZ PathOfEx.00809828                                    ; 已学过该技能则跳转
$+9B     008094AB      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+9E     008094AE      8378 28 00                 CMP DWORD PTR DS:[EAX+0x28],0x0
$+A2     008094B2      0F87 70030000              JA PathOfEx.00809828
$+A8     008094B8      8A40 64                    MOV AL,BYTE PTR DS:[EAX+0x64]
$+AB     008094BB      84C0                       TEST AL,AL
$+AD     008094BD      0F85 65030000              JNZ PathOfEx.00809828
$+B3     008094C3      85FF                       TEST EDI,EDI
$+B5     008094C5      74 33                      JE SHORT PathOfEx.008094FA                               ; 通常会跳转
$+B7     008094C7      8D45 B4                    LEA EAX,DWORD PTR SS:[EBP-0x4C]
$+BA     008094CA      8BCF                       MOV ECX,EDI
$+BC     008094CC      50                         PUSH EAX
$+BD     008094CD      E8 7EC2CEFF                CALL PathOfEx.004F5750
$+C2     008094D2      0FB673 40                  MOVZX ESI,BYTE PTR DS:[EBX+0x40]
$+C6     008094D6      BA 01000000                MOV EDX,0x1
$+CB     008094DB      8955 E8                    MOV DWORD PTR SS:[EBP-0x18],EDX
$+CE     008094DE      8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
$+D1     008094E1      8B48 2C                    MOV ECX,DWORD PTR DS:[EAX+0x2C]
$+D4     008094E4      8BC6                       MOV EAX,ESI
$+D6     008094E6      83E0 07                    AND EAX,0x7
$+D9     008094E9      3BC8                       CMP ECX,EAX
$+DB     008094EB      75 08                      JNZ SHORT PathOfEx.008094F5
$+DD     008094ED      C1EE 06                    SHR ESI,0x6
$+E0     008094F0      3977 04                    CMP DWORD PTR DS:[EDI+0x4],ESI
$+E3     008094F3      74 08                      JE SHORT PathOfEx.008094FD
$+E5     008094F5      8855 EF                    MOV BYTE PTR SS:[EBP-0x11],DL
$+E8     008094F8      EB 07                      JMP SHORT PathOfEx.00809501
$+EA     008094FA      8B55 E8                    MOV EDX,DWORD PTR SS:[EBP-0x18]
$+ED     008094FD      C645 EF 00                 MOV BYTE PTR SS:[EBP-0x11],0x0
$+F1     00809501      F6C2 01                    TEST DL,0x1
$+F4     00809504      74 0E                      JE SHORT PathOfEx.00809514                               ; 跳转
$+F6     00809506      83E2 FE                    AND EDX,0xFFFFFFFE
$+F9     00809509      8D4D B4                    LEA ECX,DWORD PTR SS:[EBP-0x4C]
$+FC     0080950C      8955 E8                    MOV DWORD PTR SS:[EBP-0x18],EDX
$+FF     0080950F      E8 8C67C6FF                CALL PathOfEx.0046FCA0
$+104    00809514      807D EF 00                 CMP BYTE PTR SS:[EBP-0x11],0x0
$+108    00809518      0F85 0A030000              JNZ PathOfEx.00809828
$+10E    0080951E      8B75 08                    MOV ESI,DWORD PTR SS:[EBP+0x8]
$+111    00809521      8D4D DC                    LEA ECX,DWORD PTR SS:[EBP-0x24]
$+114    00809524      51                         PUSH ECX                                                 ; ecx=skill_id*
$+115    00809525      8D4D D4                    LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+118    00809528      51                         PUSH ECX
$+119    00809529      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+11C    0080952C      8D4B 20                    LEA ECX,DWORD PTR DS:[EBX+0x20]
$+11F    0080952F      8B40 20                    MOV EAX,DWORD PTR DS:[EAX+0x20]
$+122    00809532      8945 DC                    MOV DWORD PTR SS:[EBP-0x24],EAX
$+125    00809535      E8 062BC6FF                CALL PathOfEx.0046C040                                   ; 查找hash
$+12A    0080953A      8B45 D4                    MOV EAX,DWORD PTR SS:[EBP-0x2C]
$+12D    0080953D      3B43 24                    CMP EAX,DWORD PTR DS:[EBX+0x24]
$+130    00809540      74 17                      JE SHORT PathOfEx.00809559                               ; 一般都会跳转，一般都不在这个hash中
$+132    00809542      8D48 0C                    LEA ECX,DWORD PTR DS:[EAX+0xC]
$+135    00809545      8B01                       MOV EAX,DWORD PTR DS:[ECX]
$+137    00809547      68 4A0B0000                PUSH 0xB4A
$+13C    0080954C      FF10                       CALL DWORD PTR DS:[EAX]
$+13E    0080954E      85C0                       TEST EAX,EAX
$+140    00809550      74 07                      JE SHORT PathOfEx.00809559
$+142    00809552      B3 01                      MOV BL,0x1
$+144    00809554      E9 D1020000                JMP PathOfEx.0080982A
$+149    00809559      8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
$+14C    0080955C      8D4B 04                    LEA ECX,DWORD PTR DS:[EBX+0x4]
$+14F    0080955F      FF70 20                    PUSH DWORD PTR DS:[EAX+0x20]                             ; skill_id
$+152    00809562      51                         PUSH ECX                                                 ; ecx=stCD_DbPassiveSkillGraph*&
$+153    00809563      E8 0882C8FF                CALL PathOfEx.00491770                                   ; 得到stCD_DbTblPassiveSkillGraph*
$+158    00809568      83C4 04                    ADD ESP,0x4
$+15B    0080956B      8BC8                       MOV ECX,EAX
$+15D    0080956D      E8 2E7F0000                CALL PathOfEx.008114A0                                   ; call tag_根据被动技能查找stCD_PassiveSkillVertex
$+162    00809572      8BF0                       MOV ESI,EAX
$+164    00809574      C645 D0 00                 MOV BYTE PTR SS:[EBP-0x30],0x0
$+168    00809578      8975 C8                    MOV DWORD PTR SS:[EBP-0x38],ESI
$+16B    0080957B      8B7E 24                    MOV EDI,DWORD PTR DS:[ESI+0x24]
$+16E    0080957E      897D CC                    MOV DWORD PTR SS:[EBP-0x34],EDI
$+171    00809581      3B7E 28                    CMP EDI,DWORD PTR DS:[ESI+0x28]
$+174    00809584      75 0A                      JNZ SHORT PathOfEx.00809590
$+176    00809586      8B7E 30                    MOV EDI,DWORD PTR DS:[ESI+0x30]
$+179    00809589      C645 D0 01                 MOV BYTE PTR SS:[EBP-0x30],0x1
$+17D    0080958D      897D CC                    MOV DWORD PTR SS:[EBP-0x34],EDI
$+180    00809590      8B46 34                    MOV EAX,DWORD PTR DS:[ESI+0x34]
$+183    00809593      8D4D C8                    LEA ECX,DWORD PTR SS:[EBP-0x38]
$+186    00809596      8945 AC                    MOV DWORD PTR SS:[EBP-0x54],EAX
$+189    00809599      8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
$+18C    0080959C      50                         PUSH EAX
$+18D    0080959D      8975 A8                    MOV DWORD PTR SS:[EBP-0x58],ESI
$+190    008095A0      C645 B0 01                 MOV BYTE PTR SS:[EBP-0x50],0x1
$+194    008095A4      E8 0748D2FF                CALL PathOfEx.0052DDB0
$+199    008095A9      84C0                       TEST AL,AL
$+19B    008095AB      0F84 77020000              JE PathOfEx.00809828
$+1A1    008095B1      807D D0 00                 CMP BYTE PTR SS:[EBP-0x30],0x0                           ; 循环开始
$+1A5    008095B5      8B07                       MOV EAX,DWORD PTR DS:[EDI]
$+1A7    008095B7      74 03                      JE SHORT PathOfEx.008095BC
$+1A9    008095B9      83C0 04                    ADD EAX,0x4
$+1AC    008095BC      8B30                       MOV ESI,DWORD PTR DS:[EAX]
$+1AE    008095BE      8975 DC                    MOV DWORD PTR SS:[EBP-0x24],ESI
$+1B1    008095C1      837E 08 00                 CMP DWORD PTR DS:[ESI+0x8],0x0
$+1B5    008095C5      0F84 A0020000              JE PathOfEx.0080986B
$+1BB    008095CB      8D46 04                    LEA EAX,DWORD PTR DS:[ESI+0x4]
$+1BE    008095CE      8BCB                       MOV ECX,EBX
$+1C0    008095D0      50                         PUSH EAX
$+1C1    008095D1      E8 5A080000                CALL PathOfEx.00809E30
$+1C6    008095D6      84C0                       TEST AL,AL
$+1C8    008095D8      0F85 8D020000              JNZ PathOfEx.0080986B
$+1CE    008095DE      8D46 04                    LEA EAX,DWORD PTR DS:[ESI+0x4]
$+1D1    008095E1      8BCB                       MOV ECX,EBX
$+1D3    008095E3      50                         PUSH EAX
$+1D4    008095E4      E8 97080000                CALL PathOfEx.00809E80
$+1D9    008095E9      84C0                       TEST AL,AL
$+1DB    008095EB      0F85 7A020000              JNZ PathOfEx.0080986B
$+1E1    008095F1      8B46 08                    MOV EAX,DWORD PTR DS:[ESI+0x8]
$+1E4    008095F4      8D4D D4                    LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+1E7    008095F7      FF73 0C                    PUSH DWORD PTR DS:[EBX+0xC]
$+1EA    008095FA      FF73 08                    PUSH DWORD PTR DS:[EBX+0x8]
$+1ED    008095FD      8B40 20                    MOV EAX,DWORD PTR DS:[EAX+0x20]
$+1F0    00809600      0FB7C0                     MOVZX EAX,AX
$+1F3    00809603      8945 D4                    MOV DWORD PTR SS:[EBP-0x2C],EAX
$+1F6    00809606      E8 455C0000                CALL PathOfEx.0080F250
$+1FB    0080960B      83C4 08                    ADD ESP,0x8
$+1FE    0080960E      8845 E3                    MOV BYTE PTR SS:[EBP-0x1D],AL
$+201    00809611      8D45 90                    LEA EAX,DWORD PTR SS:[EBP-0x70]
$+204    00809614      50                         PUSH EAX
$+205    00809615      8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
$+208    00809618      8B48 04                    MOV ECX,DWORD PTR DS:[EAX+0x4]
$+20B    0080961B      E8 407AD1FF                CALL PathOfEx.00521060
$+210    00809620      8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
$+213    00809623      83C9 02                    OR ECX,0x2
$+216    00809626      894D E8                    MOV DWORD PTR SS:[EBP-0x18],ECX
$+219    00809629      8378 04 00                 CMP DWORD PTR DS:[EAX+0x4],0x0
$+21D    0080962D      74 1C                      JE SHORT PathOfEx.0080964B
$+21F    0080962F      8B4E 08                    MOV ECX,DWORD PTR DS:[ESI+0x8]
$+222    00809632      8D45 98                    LEA EAX,DWORD PTR SS:[EBP-0x68]
$+225    00809635      50                         PUSH EAX
$+226    00809636      E8 257AD1FF                CALL PathOfEx.00521060
$+22B    0080963B      8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
$+22E    0080963E      83C9 04                    OR ECX,0x4
$+231    00809641      C645 EF 01                 MOV BYTE PTR SS:[EBP-0x11],0x1
$+235    00809645      8378 04 00                 CMP DWORD PTR DS:[EAX+0x4],0x0
$+239    00809649      74 04                      JE SHORT PathOfEx.0080964F
$+23B    0080964B      C645 EF 00                 MOV BYTE PTR SS:[EBP-0x11],0x0
$+23F    0080964F      F6C1 04                    TEST CL,0x4
$+242    00809652      74 11                      JE SHORT PathOfEx.00809665
$+244    00809654      83E1 FB                    AND ECX,0xFFFFFFFB
$+247    00809657      894D E8                    MOV DWORD PTR SS:[EBP-0x18],ECX
$+24A    0080965A      8D4D 98                    LEA ECX,DWORD PTR SS:[EBP-0x68]
$+24D    0080965D      E8 3E66C6FF                CALL PathOfEx.0046FCA0
$+252    00809662      8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
$+255    00809665      83E1 FD                    AND ECX,0xFFFFFFFD
$+258    00809668      894D E8                    MOV DWORD PTR SS:[EBP-0x18],ECX
$+25B    0080966B      8D4D 90                    LEA ECX,DWORD PTR SS:[EBP-0x70]
$+25E    0080966E      E8 2D66C6FF                CALL PathOfEx.0046FCA0
$+263    00809673      807D EF 00                 CMP BYTE PTR SS:[EBP-0x11],0x0
$+267    00809677      0F85 79010000              JNZ PathOfEx.008097F6
$+26D    0080967D      807D E3 00                 CMP BYTE PTR SS:[EBP-0x1D],0x0
$+271    00809681      0F84 6F010000              JE PathOfEx.008097F6
$+277    00809687      8B46 08                    MOV EAX,DWORD PTR DS:[ESI+0x8]
$+27A    0080968A      8A40 71                    MOV AL,BYTE PTR DS:[EAX+0x71]
$+27D    0080968D      84C0                       TEST AL,AL
$+27F    0080968F      74 5F                      JE SHORT PathOfEx.008096F0
$+281    00809691      8B45 C8                    MOV EAX,DWORD PTR SS:[EBP-0x38]
$+284    00809694      8B5D DC                    MOV EBX,DWORD PTR SS:[EBP-0x24]
$+287    00809697      8B76 3C                    MOV ESI,DWORD PTR DS:[ESI+0x3C]
$+28A    0080969A      8B48 08                    MOV ECX,DWORD PTR DS:[EAX+0x8]
$+28D    0080969D      8B5B 40                    MOV EBX,DWORD PTR DS:[EBX+0x40]
$+290    008096A0      894D D8                    MOV DWORD PTR SS:[EBP-0x28],ECX
$+293    008096A3      8A41 72                    MOV AL,BYTE PTR DS:[ECX+0x72]
$+296    008096A6      84C0                       TEST AL,AL
$+298    008096A8      0F84 10010000              JE PathOfEx.008097BE
$+29E    008096AE      3BF3                       CMP ESI,EBX
$+2A0    008096B0      74 38                      JE SHORT PathOfEx.008096EA
$+2A2    008096B2      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+2A4    008096B4      8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]
$+2A7    008096B7      3BC1                       CMP EAX,ECX
$+2A9    008096B9      74 28                      JE SHORT PathOfEx.008096E3
$+2AB    008096BB      8B40 20                    MOV EAX,DWORD PTR DS:[EAX+0x20]
$+2AE    008096BE      8D4D D4                    LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+2B1    008096C1      0FB7C0                     MOVZX EAX,AX
$+2B4    008096C4      8945 D4                    MOV DWORD PTR SS:[EBP-0x2C],EAX
$+2B7    008096C7      8B45 E4                    MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+2BA    008096CA      FF70 0C                    PUSH DWORD PTR DS:[EAX+0xC]
$+2BD    008096CD      FF70 08                    PUSH DWORD PTR DS:[EAX+0x8]
$+2C0    008096D0      E8 7B5B0000                CALL PathOfEx.0080F250
$+2C5    008096D5      83C4 08                    ADD ESP,0x8
$+2C8    008096D8      84C0                       TEST AL,AL
$+2CA    008096DA      0F85 13010000              JNZ PathOfEx.008097F3
$+2D0    008096E0      8B4D D8                    MOV ECX,DWORD PTR SS:[EBP-0x28]
$+2D3    008096E3      83C6 04                    ADD ESI,0x4
$+2D6    008096E6      3BF3                       CMP ESI,EBX
$+2D8    008096E8    ^ 75 C8                      JNZ SHORT PathOfEx.008096B2
$+2DA    008096EA      8B5D E4                    MOV EBX,DWORD PTR SS:[EBP-0x1C]
$+2DD    008096ED      8B75 DC                    MOV ESI,DWORD PTR SS:[EBP-0x24]
$+2E0    008096F0      8B46 08                    MOV EAX,DWORD PTR DS:[ESI+0x8]
$+2E3    008096F3      8D4B 20                    LEA ECX,DWORD PTR DS:[EBX+0x20]
$+2E6    008096F6      8B40 20                    MOV EAX,DWORD PTR DS:[EAX+0x20]
$+2E9    008096F9      8945 D8                    MOV DWORD PTR SS:[EBP-0x28],EAX
$+2EC    008096FC      8D45 D8                    LEA EAX,DWORD PTR SS:[EBP-0x28]
$+2EF    008096FF      50                         PUSH EAX
$+2F0    00809700      8D45 B8                    LEA EAX,DWORD PTR SS:[EBP-0x48]
$+2F3    00809703      50                         PUSH EAX
$+2F4    00809704      E8 3729C6FF                CALL PathOfEx.0046C040
$+2F9    00809709      8B45 B8                    MOV EAX,DWORD PTR SS:[EBP-0x48]
$+2FC    0080970C      3B43 24                    CMP EAX,DWORD PTR DS:[EBX+0x24]
$+2FF    0080970F      0F84 56010000              JE PathOfEx.0080986B
$+305    00809715      8D48 0C                    LEA ECX,DWORD PTR DS:[EAX+0xC]
$+308    00809718      8B01                       MOV EAX,DWORD PTR DS:[ECX]
$+30A    0080971A      68 4A0B0000                PUSH 0xB4A
$+30F    0080971F      FF10                       CALL DWORD PTR DS:[EAX]
$+311    00809721      85C0                       TEST EAX,EAX
$+313    00809723      0F84 42010000              JE PathOfEx.0080986B
$+319    00809729      8D4D 80                    LEA ECX,DWORD PTR SS:[EBP-0x80]
$+31C    0080972C      E8 5F71CDFF                CALL PathOfEx.004E0890
$+321    00809731      C745 BC 00000000           MOV DWORD PTR SS:[EBP-0x44],0x0
$+328    00809738      C745 C0 00000000           MOV DWORD PTR SS:[EBP-0x40],0x0
$+32F    0080973F      C745 C4 00000000           MOV DWORD PTR SS:[EBP-0x3C],0x0
$+336    00809746      8D85 54FFFFFF              LEA EAX,DWORD PTR SS:[EBP-0xAC]
$+33C    0080974C      C785 54FFFFFF C4E6EE00     MOV DWORD PTR SS:[EBP-0xAC],PathOfEx.00EEE6C4
$+346    00809756      899D 58FFFFFF              MOV DWORD PTR SS:[EBP-0xA8],EBX
$+34C    0080975C      8985 78FFFFFF              MOV DWORD PTR SS:[EBP-0x88],EAX
$+352    00809762      50                         PUSH EAX
$+353    00809763      56                         PUSH ESI
$+354    00809764      8D45 BC                    LEA EAX,DWORD PTR SS:[EBP-0x44]
$+357    00809767      C645 FC 03                 MOV BYTE PTR SS:[EBP-0x4],0x3
$+35B    0080976B      50                         PUSH EAX
$+35C    0080976C      8D45 80                    LEA EAX,DWORD PTR SS:[EBP-0x80]
$+35F    0080976F      50                         PUSH EAX
$+360    00809770      8D4B 04                    LEA ECX,DWORD PTR DS:[EBX+0x4]
$+363    00809773      E8 F87FC8FF                CALL PathOfEx.00491770
$+368    00809778      8BC8                       MOV ECX,EAX
$+36A    0080977A      E8 B17D0000                CALL PathOfEx.00811530
$+36F    0080977F      8D8D 54FFFFFF              LEA ECX,DWORD PTR SS:[EBP-0xAC]
$+375    00809785      E8 2669C6FF                CALL PathOfEx.004700B0
$+37A    0080978A      8B55 C0                    MOV EDX,DWORD PTR SS:[EBP-0x40]
$+37D    0080978D      8D45 D8                    LEA EAX,DWORD PTR SS:[EBP-0x28]
$+380    00809790      8B4D BC                    MOV ECX,DWORD PTR SS:[EBP-0x44]
$+383    00809793      50                         PUSH EAX
$+384    00809794      895D D8                    MOV DWORD PTR SS:[EBP-0x28],EBX
$+387    00809797      E8 D4600000                CALL PathOfEx.0080F870
$+38C    0080979C      83C4 04                    ADD ESP,0x4
$+38F    0080979F      8D4D BC                    LEA ECX,DWORD PTR SS:[EBP-0x44]
$+392    008097A2      3B45 C0                    CMP EAX,DWORD PTR SS:[EBP-0x40]
$+395    008097A5      0F85 B3000000              JNZ PathOfEx.0080985E
$+39B    008097AB      E8 B00EC7FF                CALL PathOfEx.0047A660
$+3A0    008097B0      8D4D 80                    LEA ECX,DWORD PTR SS:[EBP-0x80]
$+3A3    008097B3      C645 FC 00                 MOV BYTE PTR SS:[EBP-0x4],0x0
$+3A7    008097B7      E8 A40EC7FF                CALL PathOfEx.0047A660
$+3AC    008097BC      EB 38                      JMP SHORT PathOfEx.008097F6
$+3AE    008097BE      3BF3                       CMP ESI,EBX
$+3B0    008097C0      74 31                      JE SHORT PathOfEx.008097F3
$+3B2    008097C2      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+3B4    008097C4      8D4D D8                    LEA ECX,DWORD PTR SS:[EBP-0x28]
$+3B7    008097C7      8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]
$+3BA    008097CA      8B40 20                    MOV EAX,DWORD PTR DS:[EAX+0x20]
$+3BD    008097CD      0FB7C0                     MOVZX EAX,AX
$+3C0    008097D0      8945 D8                    MOV DWORD PTR SS:[EBP-0x28],EAX
$+3C3    008097D3      8B45 E4                    MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+3C6    008097D6      FF70 0C                    PUSH DWORD PTR DS:[EAX+0xC]
$+3C9    008097D9      FF70 08                    PUSH DWORD PTR DS:[EAX+0x8]
$+3CC    008097DC      E8 6F5A0000                CALL PathOfEx.0080F250
$+3D1    008097E1      83C4 08                    ADD ESP,0x8
$+3D4    008097E4      84C0                       TEST AL,AL
$+3D6    008097E6    ^ 0F85 FEFEFFFF              JNZ PathOfEx.008096EA
$+3DC    008097EC      83C6 04                    ADD ESI,0x4
$+3DF    008097EF      3BF3                       CMP ESI,EBX
$+3E1    008097F1    ^ 75 CF                      JNZ SHORT PathOfEx.008097C2
$+3E3    008097F3      8B5D E4                    MOV EBX,DWORD PTR SS:[EBP-0x1C]
$+3E6    008097F6      83C7 04                    ADD EDI,0x4
$+3E9    008097F9      807D D0 00                 CMP BYTE PTR SS:[EBP-0x30],0x0
$+3ED    008097FD      897D CC                    MOV DWORD PTR SS:[EBP-0x34],EDI
$+3F0    00809800      75 12                      JNZ SHORT PathOfEx.00809814
$+3F2    00809802      8B75 C8                    MOV ESI,DWORD PTR SS:[EBP-0x38]
$+3F5    00809805      3B7E 28                    CMP EDI,DWORD PTR DS:[ESI+0x28]
$+3F8    00809808      75 0A                      JNZ SHORT PathOfEx.00809814
$+3FA    0080980A      8B7E 30                    MOV EDI,DWORD PTR DS:[ESI+0x30]
$+3FD    0080980D      897D CC                    MOV DWORD PTR SS:[EBP-0x34],EDI
$+400    00809810      C645 D0 01                 MOV BYTE PTR SS:[EBP-0x30],0x1
$+404    00809814      8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
$+407    00809817      50                         PUSH EAX
$+408    00809818      8D4D C8                    LEA ECX,DWORD PTR SS:[EBP-0x38]
$+40B    0080981B      E8 9045D2FF                CALL PathOfEx.0052DDB0
$+410    00809820      84C0                       TEST AL,AL
$+412    00809822    ^ 0F85 89FDFFFF              JNZ PathOfEx.008095B1                                    ; 循环结束
$+418    00809828      32DB                       XOR BL,BL
$+41A    0080982A      C745 FC 04000000           MOV DWORD PTR SS:[EBP-0x4],0x4
$+421    00809831      8B45 A0                    MOV EAX,DWORD PTR SS:[EBP-0x60]
$+424    00809834      85C0                       TEST EAX,EAX
$+426    00809836      74 11                      JE SHORT PathOfEx.00809849
$+428    00809838      50                         PUSH EAX
$+429    00809839      51                         PUSH ECX
$+42A    0080983A      E8 C1E7F0FF                CALL PathOfEx.00718000
$+42F    0080983F      83C4 04                    ADD ESP,0x4
$+432    00809842      8BC8                       MOV ECX,EAX
$+434    00809844      E8 47E3F0FF                CALL PathOfEx.00717B90
$+439    00809849      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+43C    0080984C      8AC3                       MOV AL,BL
$+43E    0080984E      5F                         POP EDI
$+43F    0080984F      5E                         POP ESI
$+440    00809850      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+447    00809857      5B                         POP EBX
$+448    00809858      8BE5                       MOV ESP,EBP
$+44A    0080985A      5D                         POP EBP
$+44B    0080985B      C2 0400                    RETN 0x4
$+44E    0080985E      E8 FD0DC7FF                CALL PathOfEx.0047A660
$+453    00809863      8D4D 80                    LEA ECX,DWORD PTR SS:[EBP-0x80]
$+456    00809866      E8 F50DC7FF                CALL PathOfEx.0047A660
$+45B    0080986B      B3 01                      MOV BL,0x1
$+45D    0080986D    ^ EB BB                      JMP SHORT PathOfEx.0080982A
$+45F    0080986F      CC                         INT3
$+460    00809870      51                         PUSH ECX                                                 ; tag_判断是否已点亮该天赋
$+461    00809871      53                         PUSH EBX
$+462    00809872      55                         PUSH EBP
$+463    00809873      8B6C24 10                  MOV EBP,DWORD PTR SS:[ESP+0x10]
$+467    00809877      57                         PUSH EDI
$+468    00809878      8BF9                       MOV EDI,ECX
$+46A    0080987A      8B5D 04                    MOV EBX,DWORD PTR SS:[EBP+0x4]
$+46D    0080987D      85DB                       TEST EBX,EBX
$+46F    0080987F      75 09                      JNZ SHORT PathOfEx.0080988A
$+471    00809881      32C0                       XOR AL,AL
$+473    00809883      5F                         POP EDI
$+474    00809884      5D                         POP EBP
$+475    00809885      5B                         POP EBX
$+476    00809886      59                         POP ECX
$+477    00809887      C2 0400                    RETN 0x4
$+47A    0080988A      0FB743 20                  MOVZX EAX,WORD PTR DS:[EBX+0x20]
$+47E    0080988E      8B57 0C                    MOV EDX,DWORD PTR DS:[EDI+0xC]
$+481    00809891      894424 14                  MOV DWORD PTR SS:[ESP+0x14],EAX
$+485    00809895      8D4424 14                  LEA EAX,DWORD PTR SS:[ESP+0x14]
$+489    00809899      51                         PUSH ECX                                                 ; ecx=stCD_PassiveSkillGraphMgr*
$+48A    0080989A      8B4F 08                    MOV ECX,DWORD PTR DS:[EDI+0x8]                           ; then,ecx=stCD_PassiveSkillGraphMgr.learned_skill_id_cont_
$+48D    0080989D      50                         PUSH EAX                                                 ; eax=skill_id*
$+48E    0080989E      E8 4D660000                CALL PathOfEx.0080FEF0
$+493    008098A3      8BC8                       MOV ECX,EAX
$+495    008098A5      83C4 08                    ADD ESP,0x8
$+498    008098A8      3B4F 0C                    CMP ECX,DWORD PTR DS:[EDI+0xC]
$+49B    008098AB      74 09                      JE SHORT PathOfEx.008098B6
$+49D    008098AD      0FB743 20                  MOVZX EAX,WORD PTR DS:[EBX+0x20]
$+4A1    008098B1      66:3B01                    CMP AX,WORD PTR DS:[ECX]
$+4A4    008098B4      73 39                      JNB SHORT PathOfEx.008098EF
$+4A6    008098B6      8B4B 28                    MOV ECX,DWORD PTR DS:[EBX+0x28]
$+4A9    008098B9      8B43 2C                    MOV EAX,DWORD PTR DS:[EBX+0x2C]
$+4AC    008098BC      897C24 14                  MOV DWORD PTR SS:[ESP+0x14],EDI
$+4B0    008098C0      8D14C8                     LEA EDX,DWORD PTR DS:[EAX+ECX*8]
$+4B3    008098C3      8B4B 2C                    MOV ECX,DWORD PTR DS:[EBX+0x2C]
$+4B6    008098C6      8D4424 14                  LEA EAX,DWORD PTR SS:[ESP+0x14]
$+4BA    008098CA      50                         PUSH EAX
$+4BB    008098CB      E8 00600000                CALL PathOfEx.0080F8D0
$+4C0    008098D0      8B4D 04                    MOV ECX,DWORD PTR SS:[EBP+0x4]
$+4C3    008098D3      83C4 04                    ADD ESP,0x4
$+4C6    008098D6      8B51 28                    MOV EDX,DWORD PTR DS:[ECX+0x28]
$+4C9    008098D9      8B49 2C                    MOV ECX,DWORD PTR DS:[ECX+0x2C]
$+4CC    008098DC      8D0CD1                     LEA ECX,DWORD PTR DS:[ECX+EDX*8]
$+4CF    008098DF      3BC1                       CMP EAX,ECX
$+4D1    008098E1      75 0C                      JNZ SHORT PathOfEx.008098EF
$+4D3    008098E3      55                         PUSH EBP
$+4D4    008098E4      8BCF                       MOV ECX,EDI
$+4D6    008098E6      E8 95050000                CALL PathOfEx.00809E80
$+4DB    008098EB      84C0                       TEST AL,AL
$+4DD    008098ED    ^ 74 92                      JE SHORT PathOfEx.00809881
$+4DF    008098EF      5F                         POP EDI
$+4E0    008098F0      5D                         POP EBP
$+4E1    008098F1      B0 01                      MOV AL,0x1
$+4E3    008098F3      5B                         POP EBX
$+4E4    008098F4      59                         POP ECX
$+4E5    008098F5      C2 0400                    RETN 0x4
$+4E8    008098F8      CC                         INT3
$+4E9    008098F9      CC                         INT3
$+4EA    008098FA      CC                         INT3
$+4EB    008098FB      CC                         INT3
$+4EC    008098FC      CC                         INT3
$+4ED    008098FD      CC                         INT3
$+4EE    008098FE      CC                         INT3
$+4EF    008098FF      CC                         INT3
$+4F0    00809900      55                         PUSH EBP
$+4F1    00809901      8BEC                       MOV EBP,ESP
$+4F3    00809903      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+4F9    00809909      6A FF                      PUSH -0x1
$+4FB    0080990B      68 56A4D500                PUSH PathOfEx.00D5A456
$+500    00809910      50                         PUSH EAX
$+501    00809911      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+508    00809918      83EC 60                    SUB ESP,0x60
$+50B    0080991B      53                         PUSH EBX
$+50C    0080991C      56                         PUSH ESI
$+50D    0080991D      57                         PUSH EDI
$+50E    0080991E      8B7D 08                    MOV EDI,DWORD PTR SS:[EBP+0x8]
$+511    00809921      8BD9                       MOV EBX,ECX
$+513    00809923      57                         PUSH EDI
$+514    00809924      E8 47FFFFFF                CALL PathOfEx.00809870
$+519    00809929      84C0                       TEST AL,AL
$+51B    0080992B      75 13                      JNZ SHORT PathOfEx.00809940
$+51D    0080992D      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+520    00809930      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+527    00809937      5F                         POP EDI
$+528    00809938      5E                         POP ESI
$+529    00809939      5B                         POP EBX
$+52A    0080993A      8BE5                       MOV ESP,EBP
$+52C    0080993C      5D                         POP EBP
$+52D    0080993D      C2 0800                    RETN 0x8
$+530    00809940      8B4B 04                    MOV ECX,DWORD PTR DS:[EBX+0x4]
$+533    00809943      85C9                       TEST ECX,ECX
$+535    00809945      74 05                      JE SHORT PathOfEx.0080994C
$+537    00809947      E8 74FCF0FF                CALL PathOfEx.007195C0
$+53C    0080994C      8B43 04                    MOV EAX,DWORD PTR DS:[EBX+0x4]
$+53F    0080994F      57                         PUSH EDI
$+540    00809950      8B70 30                    MOV ESI,DWORD PTR DS:[EAX+0x30]
$+543    00809953      8D45 F0                    LEA EAX,DWORD PTR SS:[EBP-0x10]
$+546    00809956      50                         PUSH EAX
$+547    00809957      8D4E 0C                    LEA ECX,DWORD PTR DS:[ESI+0xC]
$+54A    0080995A      E8 01B0CEFF                CALL PathOfEx.004F4960
$+54F    0080995F      8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
$+552    00809962      3B46 10                    CMP EAX,DWORD PTR DS:[ESI+0x10]
$+555    00809965      75 04                      JNZ SHORT PathOfEx.0080996B
$+557    00809967      33C0                       XOR EAX,EAX
$+559    00809969      EB 03                      JMP SHORT PathOfEx.0080996E
$+55B    0080996B      8B40 10                    MOV EAX,DWORD PTR DS:[EAX+0x10]
$+55E    0080996E      8945 08                    MOV DWORD PTR SS:[EBP+0x8],EAX
$+561    00809971      8D4D C0                    LEA ECX,DWORD PTR SS:[EBP-0x40]
$+564    00809974      8D45 F0                    LEA EAX,DWORD PTR SS:[EBP-0x10]
$+567    00809977      C745 F0 00000000           MOV DWORD PTR SS:[EBP-0x10],0x0
$+56E    0080997E      50                         PUSH EAX
$+56F    0080997F      6A 00                      PUSH 0x0
$+571    00809981      C745 C0 00000000           MOV DWORD PTR SS:[EBP-0x40],0x0
$+578    00809988      C745 C4 00000000           MOV DWORD PTR SS:[EBP-0x3C],0x0
$+57F    0080998F      C745 C8 00000000           MOV DWORD PTR SS:[EBP-0x38],0x0
$+586    00809996      E8 F56FCDFF                CALL PathOfEx.004E0990
$+58B    0080999B      C745 CC 00000000           MOV DWORD PTR SS:[EBP-0x34],0x0
$+592    008099A2      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+599    008099A9      C745 D0 00000000           MOV DWORD PTR SS:[EBP-0x30],0x0
$+5A0    008099B0      C745 D4 00000000           MOV DWORD PTR SS:[EBP-0x2C],0x0
$+5A7    008099B7      C745 D8 00000000           MOV DWORD PTR SS:[EBP-0x28],0x0
$+5AE    008099BE      8B45 0C                    MOV EAX,DWORD PTR SS:[EBP+0xC]
$+5B1    008099C1      8945 DC                    MOV DWORD PTR SS:[EBP-0x24],EAX
$+5B4    008099C4      8D45 08                    LEA EAX,DWORD PTR SS:[EBP+0x8]
$+5B7    008099C7      8945 E0                    MOV DWORD PTR SS:[EBP-0x20],EAX
$+5BA    008099CA      8D45 98                    LEA EAX,DWORD PTR SS:[EBP-0x68]
$+5BD    008099CD      F30F7E45 DC                MOVQ XMM0,QWORD PTR SS:[EBP-0x24]
$+5C2    008099D2      C745 98 ACE6EE00           MOV DWORD PTR SS:[EBP-0x68],PathOfEx.00EEE6AC
$+5C9    008099D9      660FD645 9C                MOVQ QWORD PTR SS:[EBP-0x64],XMM0
$+5CE    008099DE      895D A4                    MOV DWORD PTR SS:[EBP-0x5C],EBX
*/
//tag_根据被动技能查找stCD_PassiveSkillVertex
/*
$-1      0081149F      CC                         INT3
$ ==>    008114A0      55                         PUSH EBP                                                 ; tag_根据被动技能查找stCD_PassiveSkillVertex
$+1      008114A1      8BEC                       MOV EBP,ESP
$+3      008114A3      6A FF                      PUSH -0x1
$+5      008114A5      68 5E8BD200                PUSH PathOfEx.00D28B5E
$+A      008114AA      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+10     008114B0      50                         PUSH EAX
$+11     008114B1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+18     008114B8      83EC 0C                    SUB ESP,0xC
$+1B     008114BB      56                         PUSH ESI
$+1C     008114BC      FF75 08                    PUSH DWORD PTR SS:[EBP+0x8]                              ; push skill_id
$+1F     008114BF      8D45 E8                    LEA EAX,DWORD PTR SS:[EBP-0x18]
$+22     008114C2      8BF1                       MOV ESI,ECX
$+24     008114C4      50                         PUSH EAX
$+25     008114C5      E8 66FFFFFF                CALL PathOfEx.00811430                                   ; 查找返回stCD_TblItemPassiveSkillPair
$+2A     008114CA      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+31     008114D1      837D EC 00                 CMP DWORD PTR SS:[EBP-0x14],0x0
$+35     008114D5      74 1D                      JE SHORT PathOfEx.008114F4
$+37     008114D7      8D45 E8                    LEA EAX,DWORD PTR SS:[EBP-0x18]
$+3A     008114DA      50                         PUSH EAX                                                 ; eax=const stCD_TblItemPassiveSkillPair*
$+3B     008114DB      8D45 08                    LEA EAX,DWORD PTR SS:[EBP+0x8]
$+3E     008114DE      50                         PUSH EAX                                                 ; eax=stCD_PassiveSkillVertexHashMap::iterator*
$+3F     008114DF      8D4E 0C                    LEA ECX,DWORD PTR DS:[ESI+0xC]
$+42     008114E2      E8 7934CEFF                CALL PathOfEx.004F4960                                   ; call tag_查找天赋连接结点
$+47     008114E7      8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
$+4A     008114EA      3B46 10                    CMP EAX,DWORD PTR DS:[ESI+0x10]
$+4D     008114ED      74 05                      JE SHORT PathOfEx.008114F4
$+4F     008114EF      8B70 10                    MOV ESI,DWORD PTR DS:[EAX+0x10]
$+52     008114F2      EB 02                      JMP SHORT PathOfEx.008114F6
$+54     008114F4      33F6                       XOR ESI,ESI
$+56     008114F6      C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
$+5D     008114FD      8B45 E8                    MOV EAX,DWORD PTR SS:[EBP-0x18]
$+60     00811500      85C0                       TEST EAX,EAX
$+62     00811502      74 11                      JE SHORT PathOfEx.00811515
$+64     00811504      50                         PUSH EAX
$+65     00811505      51                         PUSH ECX
$+66     00811506      E8 F56AF0FF                CALL PathOfEx.00718000                                   ; nothing
$+6B     0081150B      83C4 04                    ADD ESP,0x4
$+6E     0081150E      8BC8                       MOV ECX,EAX
$+70     00811510      E8 7B66F0FF                CALL PathOfEx.00717B90                                   ; nothing
$+75     00811515      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+78     00811518      8BC6                       MOV EAX,ESI
$+7A     0081151A      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+81     00811521      5E                         POP ESI
$+82     00811522      8BE5                       MOV ESP,EBP
$+84     00811524      5D                         POP EBP
$+85     00811525      C2 0400                    RETN 0x4
$+88     00811528      CC                         INT3
$+89     00811529      CC                         INT3
$+8A     0081152A      CC                         INT3
$+8B     0081152B      CC                         INT3
$+8C     0081152C      CC                         INT3
$+8D     0081152D      CC                         INT3
$+8E     0081152E      CC                         INT3
$+8F     0081152F      CC                         INT3
$+90     00811530      55                         PUSH EBP
$+91     00811531      8BEC                       MOV EBP,ESP
$+93     00811533      6A FF                      PUSH -0x1
$+95     00811535      68 24A9D500                PUSH PathOfEx.00D5A924
$+9A     0081153A      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+A0     00811540      50                         PUSH EAX
$+A1     00811541      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+A8     00811548      83EC 30                    SUB ESP,0x30
$+AB     0081154B      53                         PUSH EBX
$+AC     0081154C      33DB                       XOR EBX,EBX
$+AE     0081154E      895D E4                    MOV DWORD PTR SS:[EBP-0x1C],EBX
$+B1     00811551      56                         PUSH ESI
$+B2     00811552      57                         PUSH EDI
$+B3     00811553      895D C8                    MOV DWORD PTR SS:[EBP-0x38],EBX
$+B6     00811556      895D CC                    MOV DWORD PTR SS:[EBP-0x34],EBX
$+B9     00811559      895D D0                    MOV DWORD PTR SS:[EBP-0x30],EBX
$+BC     0081155C      53                         PUSH EBX
$+BD     0081155D      895D FC                    MOV DWORD PTR SS:[EBP-0x4],EBX
$+C0     00811560      FF71 14                    PUSH DWORD PTR DS:[ECX+0x14]
$+C3     00811563      8B4D 08                    MOV ECX,DWORD PTR SS:[EBP+0x8]
$+C6     00811566      E8 25FBCCFF                CALL PathOfEx.004E1090
$+CB     0081156B      8D45 10                    LEA EAX,DWORD PTR SS:[EBP+0x10]
$+CE     0081156E      50                         PUSH EAX
$+CF     0081156F      8D4D C8                    LEA ECX,DWORD PTR SS:[EBP-0x38]
$+D2     00811572      E8 8902C9FF                CALL PathOfEx.004A1800
$+D7     00811577      8B7D C8                    MOV EDI,DWORD PTR SS:[EBP-0x38]
$+DA     0081157A      8B75 CC                    MOV ESI,DWORD PTR SS:[EBP-0x34]
$+DD     0081157D      3BFE                       CMP EDI,ESI
$+DF     0081157F      0F94C0                     SETE AL
$+E2     00811582      84C0                       TEST AL,AL
$+E4     00811584      0F85 AC000000              JNZ PathOfEx.00811636
$+EA     0081158A      66:0F1F4400 00             NOP WORD PTR DS:[EAX+EAX]
$+F0     00811590      8B46 FC                    MOV EAX,DWORD PTR DS:[ESI-0x4]
$+F3     00811593      83C6 FC                    ADD ESI,-0x4
$+F6     00811596      8945 EC                    MOV DWORD PTR SS:[EBP-0x14],EAX
$+F9     00811599      8975 CC                    MOV DWORD PTR SS:[EBP-0x34],ESI
$+FC     0081159C      8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]
$+FF     0081159F      8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
$+102    008115A2      8B10                       MOV EDX,DWORD PTR DS:[EAX]
$+104    008115A4      8BC1                       MOV EAX,ECX
$+106    008115A6      C1E8 05                    SHR EAX,0x5
$+109    008115A9      83E1 1F                    AND ECX,0x1F
$+10C    008115AC      8D1482                     LEA EDX,DWORD PTR DS:[EDX+EAX*4]
$+10F    008115AF      B8 01000000                MOV EAX,0x1
$+114    008115B4      D3E0                       SHL EAX,CL
$+116    008115B6      8502                       TEST DWORD PTR DS:[EDX],EAX
$+118    008115B8      75 74                      JNZ SHORT PathOfEx.0081162E
$+11A    008115BA      8B06                       MOV EAX,DWORD PTR DS:[ESI]
$+11C    008115BC      8945 10                    MOV DWORD PTR SS:[EBP+0x10],EAX
*/
//tag_查找天赋连接结点
/*
$-1      004F495F      CC                         INT3
$ ==>    004F4960      51                         PUSH ECX                                                 ; tag_查找天赋连接结点
$+1      004F4961      53                         PUSH EBX                                                 ; ecx=stCD_PassiveSkillVertexHashMap*
$+2      004F4962      8B5C24 10                  MOV EBX,DWORD PTR SS:[ESP+0x10]                          ; p1=stCD_PassiveSkillVertexHashMap::iterator*
$+6      004F4966      55                         PUSH EBP                                                 ; p2=const stCD_TblItemPassiveSkillPair*
$+7      004F4967      56                         PUSH ESI
$+8      004F4968      57                         PUSH EDI
$+9      004F4969      53                         PUSH EBX
$+A      004F496A      8BF9                       MOV EDI,ECX
$+C      004F496C      E8 5F010000                CALL PathOfEx.004F4AD0
$+11     004F4971      8B57 18                    MOV EDX,DWORD PTR DS:[EDI+0x18]
$+14     004F4974      8B77 0C                    MOV ESI,DWORD PTR DS:[EDI+0xC]
$+17     004F4977      23D0                       AND EDX,EAX
$+19     004F4979      8B7F 04                    MOV EDI,DWORD PTR DS:[EDI+0x4]
$+1C     004F497C      8B04D6                     MOV EAX,DWORD PTR DS:[ESI+EDX*8]
$+1F     004F497F      03D2                       ADD EDX,EDX
$+21     004F4981      8B2C96                     MOV EBP,DWORD PTR DS:[ESI+EDX*4]
$+24     004F4984      8D1496                     LEA EDX,DWORD PTR DS:[ESI+EDX*4]
$+27     004F4987      3BEF                       CMP EBP,EDI
$+29     004F4989      75 04                      JNZ SHORT PathOfEx.004F498F
$+2B     004F498B      8BCF                       MOV ECX,EDI
$+2D     004F498D      EB 05                      JMP SHORT PathOfEx.004F4994
$+2F     004F498F      8B4A 04                    MOV ECX,DWORD PTR DS:[EDX+0x4]
$+32     004F4992      8B09                       MOV ECX,DWORD PTR DS:[ECX]
$+34     004F4994      3BC1                       CMP EAX,ECX
$+36     004F4996      74 25                      JE SHORT PathOfEx.004F49BD
$+38     004F4998      8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]
$+3B     004F499B      3B4B 04                    CMP ECX,DWORD PTR DS:[EBX+0x4]
$+3E     004F499E      74 04                      JE SHORT PathOfEx.004F49A4
$+40     004F49A0      8B00                       MOV EAX,DWORD PTR DS:[EAX]
$+42     004F49A2    ^ EB E3                      JMP SHORT PathOfEx.004F4987
$+44     004F49A4      8B4B 04                    MOV ECX,DWORD PTR DS:[EBX+0x4]
$+47     004F49A7      3B48 0C                    CMP ECX,DWORD PTR DS:[EAX+0xC]
$+4A     004F49AA      8B4C24 18                  MOV ECX,DWORD PTR SS:[ESP+0x18]
$+4E     004F49AE      0F45C7                     CMOVNE EAX,EDI
$+51     004F49B1      8901                       MOV DWORD PTR DS:[ECX],EAX
$+53     004F49B3      8BC1                       MOV EAX,ECX
$+55     004F49B5      5F                         POP EDI
$+56     004F49B6      5E                         POP ESI
$+57     004F49B7      5D                         POP EBP
$+58     004F49B8      5B                         POP EBX
$+59     004F49B9      59                         POP ECX
$+5A     004F49BA      C2 0800                    RETN 0x8
$+5D     004F49BD      8B4424 18                  MOV EAX,DWORD PTR SS:[ESP+0x18]
$+61     004F49C1      8938                       MOV DWORD PTR DS:[EAX],EDI
$+63     004F49C3      5F                         POP EDI
$+64     004F49C4      5E                         POP ESI
$+65     004F49C5      5D                         POP EBP
$+66     004F49C6      5B                         POP EBX
$+67     004F49C7      59                         POP ECX
$+68     004F49C8      C2 0800                    RETN 0x8
$+6B     004F49CB      CC                         INT3
$+6C     004F49CC      CC                         INT3
$+6D     004F49CD      CC                         INT3
$+6E     004F49CE      CC                         INT3
$+6F     004F49CF      CC                         INT3
$+70     004F49D0      55                         PUSH EBP
$+71     004F49D1      8BEC                       MOV EBP,ESP
$+73     004F49D3      64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
$+79     004F49D9      6A FF                      PUSH -0x1
$+7B     004F49DB      68 2617D200                PUSH PathOfEx.00D21726
$+80     004F49E0      50                         PUSH EAX
$+81     004F49E1      64:8925 00000000           MOV DWORD PTR FS:[0],ESP
$+88     004F49E8      56                         PUSH ESI
$+89     004F49E9      8BF1                       MOV ESI,ECX
$+8B     004F49EB      C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
$+92     004F49F2      C706 74AFE700              MOV DWORD PTR DS:[ESI],PathOfEx.00E7AF74
$+98     004F49F8      E8 534B2200                CALL PathOfEx.00719550
$+9D     004F49FD      84C0                       TEST AL,AL
$+9F     004F49FF      74 0C                      JE SHORT PathOfEx.004F4A0D
$+A1     004F4A01      8B4E 30                    MOV ECX,DWORD PTR DS:[ESI+0x30]
$+A4     004F4A04      85C9                       TEST ECX,ECX
$+A6     004F4A06      74 05                      JE SHORT PathOfEx.004F4A0D
$+A8     004F4A08      E8 A3472600                CALL PathOfEx.007591B0
$+AD     004F4A0D      8BCE                       MOV ECX,ESI
$+AF     004F4A0F      E8 DC452200                CALL PathOfEx.00718FF0
$+B4     004F4A14      F645 08 01                 TEST BYTE PTR SS:[EBP+0x8],0x1
$+B8     004F4A18      74 0B                      JE SHORT PathOfEx.004F4A25
$+BA     004F4A1A      6A 60                      PUSH 0x60
$+BC     004F4A1C      56                         PUSH ESI
$+BD     004F4A1D      E8 E67C7800                CALL PathOfEx.00C7C708
$+C2     004F4A22      83C4 08                    ADD ESP,0x8
$+C5     004F4A25      8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
$+C8     004F4A28      8BC6                       MOV EAX,ESI
$+CA     004F4A2A      64:890D 00000000           MOV DWORD PTR FS:[0],ECX
$+D1     004F4A31      5E                         POP ESI
$+D2     004F4A32      8BE5                       MOV ESP,EBP
$+D4     004F4A34      5D                         POP EBP
$+D5     004F4A35      C2 0400                    RETN 0x4
$+D8     004F4A38      CC                         INT3
*/
//tag_生成技能tips信息到vector
/*
$-1      0075F38F     CC                       INT3
$ ==>    0075F390     55                       PUSH EBP                                          ; tag_生成技能tips信息到vector
$+1      0075F391     8BEC                     MOV EBP,ESP                                       ; ecx=stCD_SkillAttrHashMap*
$+3      0075F393     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]                          ; p1=stCD_SkillAttrObj*
$+9      0075F399     6A FF                    PUSH -0x1                                         ; p2=const stCD_SkillAttrNameCont*,=可显示属性集
$+B      0075F39B     68 B4EAD400              PUSH PathOfEx.00D4EAB4                            ; p3=stCD_SkillTipsRowCont*，输出结果集
$+10     0075F3A0     50                       PUSH EAX
$+11     0075F3A1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+18     0075F3A8     83EC 08                  SUB ESP,0x8
$+1B     0075F3AB     53                       PUSH EBX
$+1C     0075F3AC     8BD9                     MOV EBX,ECX
$+1E     0075F3AE     56                       PUSH ESI
$+1F     0075F3AF     57                       PUSH EDI
$+20     0075F3B0     8B73 04                  MOV ESI,DWORD PTR DS:[EBX+0x4]
$+23     0075F3B3     8B36                     MOV ESI,DWORD PTR DS:[ESI]                        ; then,esi=stCD_SkillAttrSmartPtr*
$+25     0075F3B5     3B73 04                  CMP ESI,DWORD PTR DS:[EBX+0x4]
$+28     0075F3B8     74 76                    JE SHORT PathOfEx.0075F430
$+2A     0075F3BA     66:0F1F4400 00           NOP WORD PTR DS:[EAX+EAX]
$+30     0075F3C0     8B4D 0C                  MOV ECX,DWORD PTR SS:[EBP+0xC]                    ; 循环开始,then,ecx=stCD_SkillAttrNameCont*
$+33     0075F3C3     8D46 08                  LEA EAX,DWORD PTR DS:[ESI+0x8]
$+36     0075F3C6     50                       PUSH EAX                                          ; push const std::wstring*,属性名
$+37     0075F3C7     E8 84850200              CALL PathOfEx.00787950                            ; call tag_判断技能属性是否属于可显示属性
$+3C     0075F3CC     84C0                     TEST AL,AL
$+3E     0075F3CE     74 59                    JE SHORT PathOfEx.0075F429
$+40     0075F3D0     8B7E 24                  MOV EDI,DWORD PTR DS:[ESI+0x24]
$+43     0075F3D3     8B4E 20                  MOV ECX,DWORD PTR DS:[ESI+0x20]
$+46     0075F3D6     85FF                     TEST EDI,EDI
$+48     0075F3D8     74 04                    JE SHORT PathOfEx.0075F3DE
$+4A     0075F3DA     F0:FF47 04               LOCK INC DWORD PTR DS:[EDI+0x4]
$+4E     0075F3DE     897D F0                  MOV DWORD PTR SS:[EBP-0x10],EDI
$+51     0075F3E1     894D EC                  MOV DWORD PTR SS:[EBP-0x14],ECX
$+54     0075F3E4     FF75 10                  PUSH DWORD PTR SS:[EBP+0x10]                      ; push stCD_SkillTipsRowCont*
$+57     0075F3E7     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
$+5E     0075F3EE     8B01                     MOV EAX,DWORD PTR DS:[ECX]                        ; ecx=stCD_SkillAttr*
$+60     0075F3F0     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]                       ; push stCD_SkillAttrObj*
$+63     0075F3F3     FF50 10                  CALL DWORD PTR DS:[EAX+0x10]                      ; call tag_添加一条技能属性信息到stCD_SkillTipsRowCont中
$+66     0075F3F6     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$+6D     0075F3FD     85FF                     TEST EDI,EDI
$+6F     0075F3FF     74 21                    JE SHORT PathOfEx.0075F422
$+71     0075F401     83C8 FF                  OR EAX,0xFFFFFFFF
$+74     0075F404     F0:0FC147 04             LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+79     0075F409     75 17                    JNZ SHORT PathOfEx.0075F422
$+7B     0075F40B     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+7D     0075F40D     8BCF                     MOV ECX,EDI
$+7F     0075F40F     FF10                     CALL DWORD PTR DS:[EAX]
$+81     0075F411     83C8 FF                  OR EAX,0xFFFFFFFF
$+84     0075F414     F0:0FC147 08             LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$+89     0075F419     75 07                    JNZ SHORT PathOfEx.0075F422
$+8B     0075F41B     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+8D     0075F41D     8BCF                     MOV ECX,EDI
$+8F     0075F41F     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+92     0075F422     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+99     0075F429     8B36                     MOV ESI,DWORD PTR DS:[ESI]
$+9B     0075F42B     3B73 04                  CMP ESI,DWORD PTR DS:[EBX+0x4]
$+9E     0075F42E   ^ 75 90                    JNZ SHORT PathOfEx.0075F3C0                       ; 循环结束
$+A0     0075F430     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+A3     0075F433     5F                       POP EDI
$+A4     0075F434     5E                       POP ESI
$+A5     0075F435     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+AC     0075F43C     5B                       POP EBX
$+AD     0075F43D     8BE5                     MOV ESP,EBP
$+AF     0075F43F     5D                       POP EBP
$+B0     0075F440     C2 0C00                  RETN 0xC
$+B3     0075F443     CC                       INT3
*/
//tag_判断技能属性是否属于可显示属性
/*
$-1      0078794F     CC                       INT3
$ ==>    00787950     53                       PUSH EBX                                          ; tag_判断技能属性是否属于可显示属性
$+1      00787951     56                       PUSH ESI                                          ; ecx=stCD_SkillAttrNameCont*
$+2      00787952     FF7424 0C                PUSH DWORD PTR SS:[ESP+0xC]                       ; p1=const std::wstring*,技能属性名
$+6      00787956     8BD9                     MOV EBX,ECX                                       ; 返回bool
$+8      00787958     FF7424 10                PUSH DWORD PTR SS:[ESP+0x10]
$+C      0078795C     8B53 04                  MOV EDX,DWORD PTR DS:[EBX+0x4]
$+F      0078795F     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
$+11     00787961     E8 DAB0E0FF              CALL PathOfEx.00592A40
$+16     00787966     83C4 08                  ADD ESP,0x8
$+19     00787969     3B43 04                  CMP EAX,DWORD PTR DS:[EBX+0x4]
$+1C     0078796C     74 07                    JE SHORT PathOfEx.00787975                        ; 不属性可显示属性则跳转
$+1E     0078796E     5E                       POP ESI
$+1F     0078796F     B0 01                    MOV AL,0x1
$+21     00787971     5B                       POP EBX
$+22     00787972     C2 0400                  RETN 0x4
$+25     00787975     55                       PUSH EBP
$+26     00787976     57                       PUSH EDI
$+27     00787977     8B7B 0C                  MOV EDI,DWORD PTR DS:[EBX+0xC]
$+2A     0078797A     3B7B 10                  CMP EDI,DWORD PTR DS:[EBX+0x10]
$+2D     0078797D     74 26                    JE SHORT PathOfEx.007879A5
$+2F     0078797F     8B6C24 14                MOV EBP,DWORD PTR SS:[ESP+0x14]
$+33     00787983     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
$+35     00787985     55                       PUSH EBP
$+36     00787986     FF7424 18                PUSH DWORD PTR SS:[ESP+0x18]
$+3A     0078798A     8B71 04                  MOV ESI,DWORD PTR DS:[ECX+0x4]
$+3D     0078798D     8BD6                     MOV EDX,ESI
$+3F     0078798F     8B09                     MOV ECX,DWORD PTR DS:[ECX]
$+41     00787991     E8 AAB0E0FF              CALL PathOfEx.00592A40
$+46     00787996     83C4 08                  ADD ESP,0x8
$+49     00787999     3BC6                     CMP EAX,ESI
$+4B     0078799B     75 11                    JNZ SHORT PathOfEx.007879AE
$+4D     0078799D     83C7 04                  ADD EDI,0x4
$+50     007879A0     3B7B 10                  CMP EDI,DWORD PTR DS:[EBX+0x10]
$+53     007879A3   ^ 75 DE                    JNZ SHORT PathOfEx.00787983
$+55     007879A5     5F                       POP EDI
$+56     007879A6     5D                       POP EBP
$+57     007879A7     5E                       POP ESI
$+58     007879A8     32C0                     XOR AL,AL
$+5A     007879AA     5B                       POP EBX
$+5B     007879AB     C2 0400                  RETN 0x4
$+5E     007879AE     5F                       POP EDI
$+5F     007879AF     5D                       POP EBP
$+60     007879B0     5E                       POP ESI
$+61     007879B1     B0 01                    MOV AL,0x1
$+63     007879B3     5B                       POP EBX
$+64     007879B4     C2 0400                  RETN 0x4
$+67     007879B7     CC                       INT3
*/
//tag_添加每一行技能tips
/*
$-1      0062706F     CC                       INT3
$ ==>    00627070     55                       PUSH EBP                                          ; tag_添加每一行技能tips
$+1      00627071     8BEC                     MOV EBP,ESP                                       ; p1=const std::wstring*
$+3      00627073     6A FF                    PUSH -0x1
$+5      00627075     68 424ED200              PUSH PathOfEx.00D24E42
$+A      0062707A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+10     00627080     50                       PUSH EAX
$+11     00627081     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+18     00627088     51                       PUSH ECX
$+19     00627089     53                       PUSH EBX
$+1A     0062708A     56                       PUSH ESI
$+1B     0062708B     57                       PUSH EDI
$+1C     0062708C     68 8C0B0000              PUSH 0xB8C
$+21     00627091     8BF1                     MOV ESI,ECX
$+23     00627093     E8 7E566500              CALL PathOfEx.00C7C716
$+28     00627098     83C4 04                  ADD ESP,0x4
$+2B     0062709B     8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
$+2E     0062709E     8BC8                     MOV ECX,EAX
$+30     006270A0     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+37     006270A7     E8 F4303E00              CALL PathOfEx.00A0A1A0
$+3C     006270AC     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+43     006270B3     8BD8                     MOV EBX,EAX
$+45     006270B5     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
$+47     006270B7     53                       PUSH EBX
$+48     006270B8     8B89 F0080000            MOV ECX,DWORD PTR DS:[ECX+0x8F0]
$+4E     006270BE     81C1 04070000            ADD ECX,0x704
$+54     006270C4     8B51 08                  MOV EDX,DWORD PTR DS:[ECX+0x8]
$+57     006270C7     2B51 04                  SUB EDX,DWORD PTR DS:[ECX+0x4]
$+5A     006270CA     C1FA 02                  SAR EDX,0x2
$+5D     006270CD     52                       PUSH EDX
$+5E     006270CE     E8 9DC43D00              CALL PathOfEx.00A03570
$+63     006270D3     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
$+65     006270D5     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+67     006270D7     8B40 3C                  MOV EAX,DWORD PTR DS:[EAX+0x3C]
$+6A     006270DA     FFD0                     CALL EAX
$+6C     006270DC     D905 74EFF300            FLD DWORD PTR DS:[0xF3EF74]
$+72     006270E2     8BCB                     MOV ECX,EBX
$+74     006270E4     DCE9                     FSUB ST(1),ST
$+76     006270E6     DEE9                     FSUBP ST(1),ST
$+78     006270E8     D95D F0                  FSTP DWORD PTR SS:[EBP-0x10]
$+7B     006270EB     F3:0F104D F0             MOVSS XMM1,DWORD PTR SS:[EBP-0x10]
$+80     006270F0     E8 DBC8E9FF              CALL PathOfEx.004C39D0
$+85     006270F5     8B83 B0080000            MOV EAX,DWORD PTR DS:[EBX+0x8B0]
$+8B     006270FB     8D8B B0080000            LEA ECX,DWORD PTR DS:[EBX+0x8B0]
$+91     00627101     6A 04                    PUSH 0x4
$+93     00627103     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]
$+96     00627106     8B03                     MOV EAX,DWORD PTR DS:[EBX]
$+98     00627108     8BCB                     MOV ECX,EBX
$+9A     0062710A     FF76 04                  PUSH DWORD PTR DS:[ESI+0x4]
$+9D     0062710D     FF50 78                  CALL DWORD PTR DS:[EAX+0x78]
$+A0     00627110     8BBB 40080000            MOV EDI,DWORD PTR DS:[EBX+0x840]
$+A6     00627116     B9 001DE900              MOV ECX,PathOfEx.00E91D00                         ; magicitem
$+AB     0062711B     E8 40683E00              CALL PathOfEx.00A0D960
$+B0     00627120     50                       PUSH EAX
$+B1     00627121     8D8B 40080000            LEA ECX,DWORD PTR DS:[EBX+0x840]
$+B7     00627127     FF57 0C                  CALL DWORD PTR DS:[EDI+0xC]
$+BA     0062712A     83BB 68070000 05         CMP DWORD PTR DS:[EBX+0x768],0x5
$+C1     00627131     F3:0F1093 6C070000       MOVSS XMM2,DWORD PTR DS:[EBX+0x76C]
$+C9     00627139     75 09                    JNZ SHORT PathOfEx.00627144
$+CB     0062713B     0F2ED2                   UCOMISS XMM2,XMM2
$+CE     0062713E     9F                       LAHF
$+CF     0062713F     F6C4 44                  TEST AH,0x44
$+D2     00627142     7B 13                    JPO SHORT PathOfEx.00627157
$+D4     00627144     6A 05                    PUSH 0x5
$+D6     00627146     8BCB                     MOV ECX,EBX
$+D8     00627148     C783 68070000 05000000   MOV DWORD PTR DS:[EBX+0x768],0x5
$+E2     00627152     E8 C9723C00              CALL PathOfEx.009EE420
$+E7     00627157     8B83 B0080000            MOV EAX,DWORD PTR DS:[EBX+0x8B0]
$+ED     0062715D     8D8B B0080000            LEA ECX,DWORD PTR DS:[EBX+0x8B0]
$+F3     00627163     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]
$+F6     00627166     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]                       ; call tag_添加字符串到技能tips中
$+F9     00627169     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+FC     0062716C     5F                       POP EDI
$+FD     0062716D     5E                       POP ESI
$+FE     0062716E     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+105    00627175     5B                       POP EBX
$+106    00627176     8BE5                     MOV ESP,EBP
$+108    00627178     5D                       POP EBP
$+109    00627179     C2 0400                  RETN 0x4
$+10C    0062717C     CC                       INT3
*/
//tag_添加一条技能属性信息到stCD_SkillTipsRowCont中
/*
$-1      0077CE5F     CC                       INT3
$ ==>    0077CE60     55                       PUSH EBP                                          ; tag_添加一条技能属性信息到stCD_SkillTipsRowCont中
$+1      0077CE61     8BEC                     MOV EBP,ESP                                       ; ecx=stCD_SkillAttr*
$+3      0077CE63     6A FF                    PUSH -0x1                                         ; p1=const stCD_SkillAttrObj*
$+5      0077CE65     68 580ED500              PUSH PathOfEx.00D50E58                            ; p2=stCD_SkillTipsRowCont*
$+A      0077CE6A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+10     0077CE70     50                       PUSH EAX
$+11     0077CE71     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+18     0077CE78     83EC 6C                  SUB ESP,0x6C
$+1B     0077CE7B     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]                       ; push const stCD_SkillAttrObj*
$+1E     0077CE7E     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
$+21     0077CE81     50                       PUSH EAX                                          ; push std::wstring*
$+22     0077CE82     E8 89160000              CALL PathOfEx.0077E510                            ; call tag_计算技能属性数值并得到tips字符串
$+27     0077CE87     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+2E     0077CE8E     837D C8 00               CMP DWORD PTR SS:[EBP-0x38],0x0
$+32     0077CE92     74 52                    JE SHORT PathOfEx.0077CEE6
$+34     0077CE94     33C0                     XOR EAX,EAX
$+36     0077CE96     C745 F0 07000000         MOV DWORD PTR SS:[EBP-0x10],0x7
$+3D     0077CE9D     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
$+44     0077CEA4     66:8945 DC               MOV WORD PTR SS:[EBP-0x24],AX
$+48     0077CEA8     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
$+4B     0077CEAB     50                       PUSH EAX
$+4C     0077CEAC     8D55 B8                  LEA EDX,DWORD PTR SS:[EBP-0x48]
$+4F     0077CEAF     8D4D 88                  LEA ECX,DWORD PTR SS:[EBP-0x78]
$+52     0077CEB2     E8 19420000              CALL PathOfEx.007810D0
$+57     0077CEB7     83C4 04                  ADD ESP,0x4
$+5A     0077CEBA     8B4D 0C                  MOV ECX,DWORD PTR SS:[EBP+0xC]
$+5D     0077CEBD     50                       PUSH EAX
$+5E     0077CEBE     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+62     0077CEC2     E8 49420000              CALL PathOfEx.00781110
$+67     0077CEC7     8D4D 88                  LEA ECX,DWORD PTR SS:[EBP-0x78]
$+6A     0077CECA     E8 412FD0FF              CALL PathOfEx.0047FE10
$+6F     0077CECF     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
$+72     0077CED2     83F8 08                  CMP EAX,0x8
$+75     0077CED5     72 0F                    JB SHORT PathOfEx.0077CEE6
$+77     0077CED7     6A 02                    PUSH 0x2
$+79     0077CED9     40                       INC EAX
$+7A     0077CEDA     50                       PUSH EAX
$+7B     0077CEDB     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
$+7E     0077CEDE     E8 4DF9CEFF              CALL PathOfEx.0046C830
$+83     0077CEE3     83C4 0C                  ADD ESP,0xC
$+86     0077CEE6     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
$+89     0077CEE9     E8 12000000              CALL PathOfEx.0077CF00
$+8E     0077CEEE     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+91     0077CEF1     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+98     0077CEF8     8BE5                     MOV ESP,EBP
$+9A     0077CEFA     5D                       POP EBP
$+9B     0077CEFB     C2 0800                  RETN 0x8
$+9E     0077CEFE     CC                       INT3
*/
//tag_计算技能属性数值并得到tips字符串
/*
$-1      0077E50F     CC                       INT3
$ ==>    0077E510     55                       PUSH EBP                                          ; tag_计算技能属性数值并得到tips字符串
$+1      0077E511     8D6C24 98                LEA EBP,DWORD PTR SS:[ESP-0x68]                   ; 返回std::wstring*
$+5      0077E515     83EC 68                  SUB ESP,0x68                                      ; ecx=stCD_SkillAttr*
$+8      0077E518     6A FF                    PUSH -0x1                                         ; p1=std::wstring*
$+A      0077E51A     68 F90FD500              PUSH PathOfEx.00D50FF9                            ; p2=const stCD_SkillAttrObj*
$+F      0077E51F     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+15     0077E525     50                       PUSH EAX
$+16     0077E526     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+1D     0077E52D     81EC 40020000            SUB ESP,0x240
$+23     0077E533     8BC1                     MOV EAX,ECX
$+25     0077E535     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
$+2C     0077E53C     53                       PUSH EBX
$+2D     0077E53D     56                       PUSH ESI
$+2E     0077E53E     57                       PUSH EDI
$+2F     0077E53F     8B78 18                  MOV EDI,DWORD PTR DS:[EAX+0x18]
$+32     0077E542     2B78 14                  SUB EDI,DWORD PTR DS:[EAX+0x14]
$+35     0077E545     C1FF 02                  SAR EDI,0x2
$+38     0077E548     8945 2C                  MOV DWORD PTR SS:[EBP+0x2C],EAX                   ; 保存this指针
$+3B     0077E54B     897D 58                  MOV DWORD PTR SS:[EBP+0x58],EDI                   ; 保存数量
$+3E     0077E54E     C645 60 00               MOV BYTE PTR SS:[EBP+0x60],0x0
$+42     0077E552     8D34BD 00000000          LEA ESI,DWORD PTR DS:[EDI*4]
$+49     0077E559     8BC6                     MOV EAX,ESI
$+4B     0077E55B     E8 60F04F00              CALL PathOfEx.00C7D5C0
$+50     0077E560     8BDC                     MOV EBX,ESP
$+52     0077E562     895D 64                  MOV DWORD PTR SS:[EBP+0x64],EBX
$+55     0077E565     8BC3                     MOV EAX,EBX
$+57     0077E567     8D0C1E                   LEA ECX,DWORD PTR DS:[ESI+EBX]
$+5A     0077E56A     3BD9                     CMP EBX,ECX
$+5C     0077E56C     74 13                    JE SHORT PathOfEx.0077E581
$+5E     0077E56E     66:90                    NOP
$+60     0077E570     85C0                     TEST EAX,EAX
$+62     0077E572     74 06                    JE SHORT PathOfEx.0077E57A
$+64     0077E574     C700 00000000            MOV DWORD PTR DS:[EAX],0x0
$+6A     0077E57A     83C0 04                  ADD EAX,0x4
$+6D     0077E57D     3BC1                     CMP EAX,ECX
$+6F     0077E57F   ^ 75 EF                    JNZ SHORT PathOfEx.0077E570
$+71     0077E581     33C0                     XOR EAX,EAX
$+73     0077E583     8945 54                  MOV DWORD PTR SS:[EBP+0x54],EAX
$+76     0077E586     85FF                     TEST EDI,EDI
$+78     0077E588     74 46                    JE SHORT PathOfEx.0077E5D0
$+7A     0077E58A     66:0F1F4400 00           NOP WORD PTR DS:[EAX+EAX]
$+80     0077E590     8B4D 74                  MOV ECX,DWORD PTR SS:[EBP+0x74]                   ; then,ecx=stCD_SkillAttrObj*，循环计算属性数值
$+83     0077E593     8D3485 00000000          LEA ESI,DWORD PTR DS:[EAX*4]
$+8A     0077E59A     8B45 2C                  MOV EAX,DWORD PTR SS:[EBP+0x2C]                   ; then,eax=this,=stCD_SkillAttr*
$+8D     0077E59D     8B11                     MOV EDX,DWORD PTR DS:[ECX]
$+8F     0077E59F     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
$+92     0077E5A2     FF3430                   PUSH DWORD PTR DS:[EAX+ESI]
$+95     0077E5A5     FF12                     CALL DWORD PTR DS:[EDX]                           ; call tag_得到技能属性数值
$+97     0077E5A7     8B4D 60                  MOV ECX,DWORD PTR SS:[EBP+0x60]
$+9A     0077E5AA     85C0                     TEST EAX,EAX
$+9C     0077E5AC     89041E                   MOV DWORD PTR DS:[ESI+EBX],EAX
$+9F     0077E5AF     B8 01000000              MOV EAX,0x1
$+A4     0077E5B4     0FB6C9                   MOVZX ECX,CL
$+A7     0077E5B7     0F45C8                   CMOVNE ECX,EAX
$+AA     0077E5BA     8B45 54                  MOV EAX,DWORD PTR SS:[EBP+0x54]
$+AD     0077E5BD     40                       INC EAX
$+AE     0077E5BE     894D 60                  MOV DWORD PTR SS:[EBP+0x60],ECX
$+B1     0077E5C1     8945 54                  MOV DWORD PTR SS:[EBP+0x54],EAX
$+B4     0077E5C4     3BC7                     CMP EAX,EDI
$+B6     0077E5C6   ^ 72 C8                    JB SHORT PathOfEx.0077E590                        ; 循环结束
$+B8     0077E5C8     84C9                     TEST CL,CL
$+BA     0077E5CA     0F85 A8000000            JNZ PathOfEx.0077E678
$+C0     0077E5D0     8B45 2C                  MOV EAX,DWORD PTR SS:[EBP+0x2C]
$+C3     0077E5D3     8078 20 00               CMP BYTE PTR DS:[EAX+0x20],0x0
$+C7     0077E5D7     0F85 9E000000            JNZ PathOfEx.0077E67B
$+CD     0077E5DD     8B75 70                  MOV ESI,DWORD PTR SS:[EBP+0x70]
$+D0     0077E5E0     8D55 30                  LEA EDX,DWORD PTR SS:[EBP+0x30]
$+D3     0077E5E3     33C0                     XOR EAX,EAX
$+D5     0077E5E5     C745 48 00000000         MOV DWORD PTR SS:[EBP+0x48],0x0
$+DC     0077E5EC     66:8945 30               MOV WORD PTR SS:[EBP+0x30],AX
$+E0     0077E5F0     8BCE                     MOV ECX,ESI
$+E2     0077E5F2     8D45 48                  LEA EAX,DWORD PTR SS:[EBP+0x48]
$+E5     0077E5F5     C745 4C 00000000         MOV DWORD PTR SS:[EBP+0x4C],0x0
$+EC     0077E5FC     50                       PUSH EAX
$+ED     0077E5FD     C745 50 00000000         MOV DWORD PTR SS:[EBP+0x50],0x0
$+F4     0077E604     C745 44 07000000         MOV DWORD PTR SS:[EBP+0x44],0x7
$+FB     0077E60B     C745 40 00000000         MOV DWORD PTR SS:[EBP+0x40],0x0
$+102    0077E612     E8 092C0000              CALL PathOfEx.00781220
$+107    0077E617     8B45 44                  MOV EAX,DWORD PTR SS:[EBP+0x44]
$+10A    0077E61A     83C4 04                  ADD ESP,0x4
$+10D    0077E61D     83F8 08                  CMP EAX,0x8
$+110    0077E620     72 0F                    JB SHORT PathOfEx.0077E631
$+112    0077E622     6A 02                    PUSH 0x2
$+114    0077E624     40                       INC EAX
$+115    0077E625     50                       PUSH EAX
$+116    0077E626     FF75 30                  PUSH DWORD PTR SS:[EBP+0x30]
$+119    0077E629     E8 02E2CEFF              CALL PathOfEx.0046C830
$+11E    0077E62E     83C4 0C                  ADD ESP,0xC
$+121    0077E631     8B55 48                  MOV EDX,DWORD PTR SS:[EBP+0x48]
$+124    0077E634     33C0                     XOR EAX,EAX
$+126    0077E636     66:8945 30               MOV WORD PTR SS:[EBP+0x30],AX
$+12A    0077E63A     8945 40                  MOV DWORD PTR SS:[EBP+0x40],EAX
$+12D    0077E63D     C745 44 07000000         MOV DWORD PTR SS:[EBP+0x44],0x7
$+134    0077E644     85D2                     TEST EDX,EDX
$+136    0077E646     74 14                    JE SHORT PathOfEx.0077E65C
$+138    0077E648     8B4D 50                  MOV ECX,DWORD PTR SS:[EBP+0x50]
$+13B    0077E64B     2BCA                     SUB ECX,EDX
$+13D    0077E64D     6A 04                    PUSH 0x4
$+13F    0077E64F     C1F9 02                  SAR ECX,0x2
$+142    0077E652     51                       PUSH ECX
$+143    0077E653     52                       PUSH EDX
$+144    0077E654     E8 D7E1CEFF              CALL PathOfEx.0046C830
$+149    0077E659     83C4 0C                  ADD ESP,0xC
$+14C    0077E65C     8BC6                     MOV EAX,ESI
$+14E    0077E65E     8DA5 A8FDFFFF            LEA ESP,DWORD PTR SS:[EBP-0x258]
$+154    0077E664     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+157    0077E667     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+15E    0077E66E     5F                       POP EDI
$+15F    0077E66F     5E                       POP ESI
$+160    0077E670     5B                       POP EBX
$+161    0077E671     8D65 68                  LEA ESP,DWORD PTR SS:[EBP+0x68]
$+164    0077E674     5D                       POP EBP
$+165    0077E675     C2 0800                  RETN 0x8
$+168    0077E678     8B45 2C                  MOV EAX,DWORD PTR SS:[EBP+0x2C]
$+16B    0077E67B     8B70 08                  MOV ESI,DWORD PTR DS:[EAX+0x8]
$+16E    0077E67E     8B40 0C                  MOV EAX,DWORD PTR DS:[EAX+0xC]
$+171    0077E681     8945 74                  MOV DWORD PTR SS:[EBP+0x74],EAX
$+174    0077E684     3BF0                     CMP ESI,EAX
$+176    0077E686   ^ 0F84 51FFFFFF            JE PathOfEx.0077E5DD
$+17C    0077E68C     33C9                     XOR ECX,ECX
$+17E    0077E68E     B2 01                    MOV DL,0x1
$+180    0077E690     85FF                     TEST EDI,EDI
$+182    0077E692     74 4B                    JE SHORT PathOfEx.0077E6DF
$+184    0077E694     8B45 64                  MOV EAX,DWORD PTR SS:[EBP+0x64]
$+187    0077E697     84D2                     TEST DL,DL
$+189    0077E699     74 2E                    JE SHORT PathOfEx.0077E6C9
$+18B    0077E69B     8B1E                     MOV EBX,DWORD PTR DS:[ESI]
$+18D    0077E69D     8B3C88                   MOV EDI,DWORD PTR DS:[EAX+ECX*4]
$+190    0077E6A0     8B14CB                   MOV EDX,DWORD PTR DS:[EBX+ECX*8]
$+193    0077E6A3     81FA FFFFFF7F            CMP EDX,0x7FFFFFFF
$+199    0077E6A9     74 04                    JE SHORT PathOfEx.0077E6AF
$+19B    0077E6AB     3BFA                     CMP EDI,EDX
$+19D    0077E6AD     7C 17                    JL SHORT PathOfEx.0077E6C6
$+19F    0077E6AF     8B54CB 04                MOV EDX,DWORD PTR DS:[EBX+ECX*8+0x4]
$+1A3    0077E6B3     81FA FFFFFF7F            CMP EDX,0x7FFFFFFF
$+1A9    0077E6B9     74 04                    JE SHORT PathOfEx.0077E6BF
$+1AB    0077E6BB     3BFA                     CMP EDI,EDX
$+1AD    0077E6BD     7F 07                    JG SHORT PathOfEx.0077E6C6
$+1AF    0077E6BF     8B7D 58                  MOV EDI,DWORD PTR SS:[EBP+0x58]
$+1B2    0077E6C2     B2 01                    MOV DL,0x1
$+1B4    0077E6C4     EB 05                    JMP SHORT PathOfEx.0077E6CB
$+1B6    0077E6C6     8B7D 58                  MOV EDI,DWORD PTR SS:[EBP+0x58]
$+1B9    0077E6C9     32D2                     XOR DL,DL
$+1BB    0077E6CB     41                       INC ECX
$+1BC    0077E6CC     3BCF                     CMP ECX,EDI
$+1BE    0077E6CE   ^ 72 C7                    JB SHORT PathOfEx.0077E697
$+1C0    0077E6D0     8B45 74                  MOV EAX,DWORD PTR SS:[EBP+0x74]
$+1C3    0077E6D3     84D2                     TEST DL,DL
$+1C5    0077E6D5     75 08                    JNZ SHORT PathOfEx.0077E6DF
$+1C7    0077E6D7     81C6 C4000000            ADD ESI,0xC4
$+1CD    0077E6DD   ^ EB A5                    JMP SHORT PathOfEx.0077E684
$+1CF    0077E6DF     6A 19                    PUSH 0x19
$+1D1    0077E6E1     33C0                     XOR EAX,EAX
$+1D3    0077E6E3     C745 F0 07000000         MOV DWORD PTR SS:[EBP-0x10],0x7
$+1DA    0077E6EA     68 D08FED00              PUSH PathOfEx.00ED8FD0                            ; OldLeechModsEffectiveness
$+1DF    0077E6EF     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
$+1E2    0077E6F2     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
$+1E9    0077E6F9     66:8945 DC               MOV WORD PTR SS:[EBP-0x24],AX
$+1ED    0077E6FD     E8 AEF5CEFF              CALL PathOfEx.0046DCB0
$+1F2    0077E702     83EC 10                  SUB ESP,0x10
$+1F5    0077E705     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+1FC    0077E70C     E8 EF98F9FF              CALL PathOfEx.00718000
$+201    0077E711     8BC8                     MOV ECX,EAX
$+203    0077E713     E8 6879FFFF              CALL PathOfEx.00776080
$+208    0077E718     8BF8                     MOV EDI,EAX
$+20A    0077E71A     897D 74                  MOV DWORD PTR SS:[EBP+0x74],EDI
$+20D    0077E71D     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
$+210    0077E720     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+214    0077E724     50                       PUSH EAX
$+215    0077E725     8D55 74                  LEA EDX,DWORD PTR SS:[EBP+0x74]
$+218    0077E728     8D4D 5C                  LEA ECX,DWORD PTR SS:[EBP+0x5C]
$+21B    0077E72B     E8 005BFCFF              CALL PathOfEx.00744230
$+220    0077E730     83C4 04                  ADD ESP,0x4
$+223    0077E733     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+226    0077E736     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+229    0077E739     8945 54                  MOV DWORD PTR SS:[EBP+0x54],EAX
$+22C    0077E73C     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+230    0077E740     8B45 5C                  MOV EAX,DWORD PTR SS:[EBP+0x5C]
$+233    0077E743     85C0                     TEST EAX,EAX
$+235    0077E745     74 14                    JE SHORT PathOfEx.0077E75B
$+237    0077E747     50                       PUSH EAX
$+238    0077E748     E8 B398F9FF              CALL PathOfEx.00718000
$+23D    0077E74D     8BC8                     MOV ECX,EAX
$+23F    0077E74F     E8 3C94F9FF              CALL PathOfEx.00717B90
$+244    0077E754     C745 5C 00000000         MOV DWORD PTR SS:[EBP+0x5C],0x0
$+24B    0077E75B     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
$+24F    0077E75F     85FF                     TEST EDI,EDI
$+251    0077E761     74 0D                    JE SHORT PathOfEx.0077E770
$+253    0077E763     57                       PUSH EDI
$+254    0077E764     E8 9798F9FF              CALL PathOfEx.00718000
$+259    0077E769     8BC8                     MOV ECX,EAX
$+25B    0077E76B     E8 2094F9FF              CALL PathOfEx.00717B90
$+260    0077E770     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+267    0077E777     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
$+26A    0077E77A     83F8 08                  CMP EAX,0x8
$+26D    0077E77D     72 0F                    JB SHORT PathOfEx.0077E78E
$+26F    0077E77F     40                       INC EAX
$+270    0077E780     6A 02                    PUSH 0x2
$+272    0077E782     50                       PUSH EAX
$+273    0077E783     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
$+276    0077E786     E8 A5E0CEFF              CALL PathOfEx.0046C830
$+27B    0077E78B     83C4 0C                  ADD ESP,0xC
$+27E    0077E78E     8D46 1C                  LEA EAX,DWORD PTR DS:[ESI+0x1C]
$+281    0077E791     50                       PUSH EAX
$+282    0077E792     8D8D 3CFFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xC4]
$+288    0077E798     E8 23260000              CALL PathOfEx.00780DC0
$+28D    0077E79D     33C9                     XOR ECX,ECX
$+28F    0077E79F     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
$+296    0077E7A6     894D 74                  MOV DWORD PTR SS:[EBP+0x74],ECX
$+299    0077E7A9     394D 58                  CMP DWORD PTR SS:[EBP+0x58],ECX
$+29C    0077E7AC     0F86 AB060000            JBE PathOfEx.0077EE5D
$+2A2    0077E7B2     F3:0F100D 38F0F300       MOVSS XMM1,DWORD PTR DS:[0xF3F038]
$+2AA    0077E7BA     8D79 0A                  LEA EDI,DWORD PTR DS:[ECX+0xA]
$+2AD    0077E7BD     F3:0F1025 E8EDF300       MOVSS XMM4,DWORD PTR DS:[0xF3EDE8]
$+2B5    0077E7C5     F3:0F102D B0F3F300       MOVSS XMM5,DWORD PTR DS:[0xF3F3B0]
$+2BD    0077E7CD     F3:0F101D 5CEFF300       MOVSS XMM3,DWORD PTR DS:[0xF3EF5C]
$+2C5    0077E7D5     F3:0F1015 B8F0F300       MOVSS XMM2,DWORD PTR DS:[0xF3F0B8]
$+2CD    0077E7DD     8B5D 64                  MOV EBX,DWORD PTR SS:[EBP+0x64]
$+2D0    0077E7E0     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
$+2D3    0077E7E3     8B0488                   MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+2D6    0077E7E6     48                       DEC EAX
$+2D7    0077E7E7     83F8 16                  CMP EAX,0x16
$+2DA    0077E7EA     0F87 58060000            JA PathOfEx.0077EE48
$+2E0    0077E7F0     FF2485 C4EE7700          JMP DWORD PTR DS:[EAX*4+0x77EEC4]
$+2E7    0077E7F7     8BD3                     MOV EDX,EBX
$+2E9    0077E7F9     8D8D 3CFFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xC4]
$+2EF    0077E7FF     E8 9C69DFFF              CALL PathOfEx.005751A0
$+2F4    0077E804     E9 17060000              JMP PathOfEx.0077EE20
$+2F9    0077E809     8B03                     MOV EAX,DWORD PTR DS:[EBX]
$+2FB    0077E80B     8D55 64                  LEA EDX,DWORD PTR SS:[EBP+0x64]
$+2FE    0077E80E     F7D8                     NEG EAX
$+300    0077E810     8D8D 3CFFFFFF            LEA ECX,DWORD PTR SS:[EBP-0xC4]
$+306    0077E816     8945 64                  MOV DWORD PTR SS:[EBP+0x64],EAX
$+309    0077E819     E8 8269DFFF              CALL PathOfEx.005751A0
$+30E    0077E81E     E9 FD050000              JMP PathOfEx.0077EE20
$+313    0077E823     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+317    0077E827     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+31A    0077E82A     F3:0F5EC1                DIVSS XMM0,XMM1
$+31E    0077E82E     F3:0F2CC0                CVTTSS2SI EAX,XMM0
$+322    0077E832     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+327    0077E837     66:0F6EC8                MOVD MM1,EAX
$+32B    0077E83B     0F5BC9                   CVTDQ2PS XMM1,XMM1
$+32E    0077E83E     F3:0F5CC1                SUBSS XMM0,XMM1
$+332    0077E842     E8 191FD3FF              CALL PathOfEx.004B0760
$+337    0077E847     F3:0F100D BCECF300       MOVSS XMM1,DWORD PTR DS:[0xF3ECBC]
$+33F    0077E84F     0F2FC8                   COMISS XMM1,XMM0
$+342    0077E852     77 09                    JA SHORT PathOfEx.0077E85D
$+344    0077E854     0F2FC4                   COMISS XMM0,XMM4
$+347    0077E857     73 04                    JNB SHORT PathOfEx.0077E85D
$+349    0077E859     33C9                     XOR ECX,ECX
$+34B    0077E85B     EB 05                    JMP SHORT PathOfEx.0077E862
$+34D    0077E85D     B9 01000000              MOV ECX,0x1
$+352    0077E862     33C0                     XOR EAX,EAX
$+354    0077E864     84C9                     TEST CL,CL
$+356    0077E866     6A 00                    PUSH 0x0
$+358    0077E868     0F94C0                   SETE AL
$+35B    0077E86B     50                       PUSH EAX
$+35C    0077E86C     8D85 54FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1AC]
$+362    0077E872     50                       PUSH EAX
$+363    0077E873     E8 09CE4E00              CALL PathOfEx.00C6B681
$+368    0077E878     8BF8                     MOV EDI,EAX
$+36A    0077E87A     8D85 D4FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x12C]
$+370    0077E880     E9 60050000              JMP PathOfEx.0077EDE5
$+375    0077E885     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+379    0077E889     8D85 E4FDFFFF            LEA EAX,DWORD PTR SS:[EBP-0x21C]
$+37F    0077E88F     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+382    0077E892     6A 00                    PUSH 0x0
$+384    0077E894     6A 00                    PUSH 0x0
$+386    0077E896     50                       PUSH EAX
$+387    0077E897     F3:0F5EC1                DIVSS XMM0,XMM1
$+38B    0077E89B     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+390    0077E8A0     E8 DCCD4E00              CALL PathOfEx.00C6B681
$+395    0077E8A5     8BF8                     MOV EDI,EAX
$+397    0077E8A7     8D85 CCFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x134]
$+39D    0077E8AD     E9 33050000              JMP PathOfEx.0077EDE5
$+3A2    0077E8B2     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+3A6    0077E8B6     8D85 D4FDFFFF            LEA EAX,DWORD PTR SS:[EBP-0x22C]
$+3AC    0077E8BC     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+3AF    0077E8BF     6A 00                    PUSH 0x0
$+3B1    0077E8C1     6A 01                    PUSH 0x1
$+3B3    0077E8C3     50                       PUSH EAX
$+3B4    0077E8C4     F3:0F5EC1                DIVSS XMM0,XMM1
$+3B8    0077E8C8     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+3BD    0077E8CD     E8 AFCD4E00              CALL PathOfEx.00C6B681
$+3C2    0077E8D2     8BF8                     MOV EDI,EAX
$+3C4    0077E8D4     8D85 C4FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x13C]
$+3CA    0077E8DA     E9 06050000              JMP PathOfEx.0077EDE5
$+3CF    0077E8DF     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+3D3    0077E8E3     8D85 C4FDFFFF            LEA EAX,DWORD PTR SS:[EBP-0x23C]
$+3D9    0077E8E9     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+3DC    0077E8EC     6A 00                    PUSH 0x0
$+3DE    0077E8EE     6A 02                    PUSH 0x2
$+3E0    0077E8F0     50                       PUSH EAX
$+3E1    0077E8F1     F3:0F5EC1                DIVSS XMM0,XMM1
$+3E5    0077E8F5     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+3EA    0077E8FA     E8 82CD4E00              CALL PathOfEx.00C6B681
$+3EF    0077E8FF     8BF8                     MOV EDI,EAX
$+3F1    0077E901     8D85 BCFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x144]
$+3F7    0077E907     E9 D9040000              JMP PathOfEx.0077EDE5
$+3FC    0077E90C     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+400    0077E910     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+403    0077E913     F3:0F5EC5                DIVSS XMM0,XMM5
$+407    0077E917     F3:0F2CC0                CVTTSS2SI EAX,XMM0
$+40B    0077E91B     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+410    0077E920     66:0F6EC8                MOVD MM1,EAX
$+414    0077E924     0F5BC9                   CVTDQ2PS XMM1,XMM1
$+417    0077E927     F3:0F5CC1                SUBSS XMM0,XMM1
$+41B    0077E92B     E8 301ED3FF              CALL PathOfEx.004B0760
$+420    0077E930     F3:0F100D BCECF300       MOVSS XMM1,DWORD PTR DS:[0xF3ECBC]
$+428    0077E938     0F2FC8                   COMISS XMM1,XMM0
$+42B    0077E93B     77 09                    JA SHORT PathOfEx.0077E946
$+42D    0077E93D     0F2FC4                   COMISS XMM0,XMM4
$+430    0077E940     73 04                    JNB SHORT PathOfEx.0077E946
$+432    0077E942     33C9                     XOR ECX,ECX
$+434    0077E944     EB 05                    JMP SHORT PathOfEx.0077E94B
$+436    0077E946     B9 01000000              MOV ECX,0x1
$+43B    0077E94B     33C0                     XOR EAX,EAX
$+43D    0077E94D     84C9                     TEST CL,CL
$+43F    0077E94F     6A 00                    PUSH 0x0
$+441    0077E951     0F94C0                   SETE AL
$+444    0077E954     50                       PUSH EAX
$+445    0077E955     8D85 A4FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x15C]
$+44B    0077E95B     50                       PUSH EAX
$+44C    0077E95C     E8 20CD4E00              CALL PathOfEx.00C6B681
$+451    0077E961     8BF8                     MOV EDI,EAX
$+453    0077E963     8D85 B4FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x14C]
$+459    0077E969     E9 77040000              JMP PathOfEx.0077EDE5
$+45E    0077E96E     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+462    0077E972     8D85 94FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x16C]
$+468    0077E978     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+46B    0077E97B     6A 00                    PUSH 0x0
$+46D    0077E97D     6A 00                    PUSH 0x0
$+46F    0077E97F     50                       PUSH EAX
$+470    0077E980     F3:0F5EC5                DIVSS XMM0,XMM5
$+474    0077E984     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+479    0077E989     E8 F3CC4E00              CALL PathOfEx.00C6B681
$+47E    0077E98E     8BF8                     MOV EDI,EAX
$+480    0077E990     8D85 34FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xCC]
$+486    0077E996     E9 4A040000              JMP PathOfEx.0077EDE5
$+48B    0077E99B     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+48F    0077E99F     8D85 84FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x17C]
$+495    0077E9A5     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+498    0077E9A8     6A 00                    PUSH 0x0
$+49A    0077E9AA     6A 01                    PUSH 0x1
$+49C    0077E9AC     50                       PUSH EAX
$+49D    0077E9AD     F3:0F5EC5                DIVSS XMM0,XMM5
$+4A1    0077E9B1     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+4A6    0077E9B6     E8 C6CC4E00              CALL PathOfEx.00C6B681
$+4AB    0077E9BB     8BF8                     MOV EDI,EAX
$+4AD    0077E9BD     8D85 2CFFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xD4]
$+4B3    0077E9C3     E9 1D040000              JMP PathOfEx.0077EDE5
$+4B8    0077E9C8     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+4BC    0077E9CC     8D85 74FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x18C]
$+4C2    0077E9D2     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+4C5    0077E9D5     6A 00                    PUSH 0x0
$+4C7    0077E9D7     6A 02                    PUSH 0x2
$+4C9    0077E9D9     50                       PUSH EAX
$+4CA    0077E9DA     F3:0F5EC5                DIVSS XMM0,XMM5
$+4CE    0077E9DE     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+4D3    0077E9E3     E8 99CC4E00              CALL PathOfEx.00C6B681
$+4D8    0077E9E8     8BF8                     MOV EDI,EAX
$+4DA    0077E9EA     8D85 24FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xDC]
$+4E0    0077E9F0     E9 F0030000              JMP PathOfEx.0077EDE5
$+4E5    0077E9F5     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+4E9    0077E9F9     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+4EC    0077E9FC     F3:0F5EC3                DIVSS XMM0,XMM3
$+4F0    0077EA00     F3:0F2CC0                CVTTSS2SI EAX,XMM0
$+4F4    0077EA04     F3:0F1145 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM0
$+4F9    0077EA09     66:0F6EC8                MOVD MM1,EAX
$+4FD    0077EA0D     0F5BC9                   CVTDQ2PS XMM1,XMM1
$+500    0077EA10     F3:0F5CC1                SUBSS XMM0,XMM1
$+504    0077EA14     E8 471DD3FF              CALL PathOfEx.004B0760
$+509    0077EA19     F3:0F100D BCECF300       MOVSS XMM1,DWORD PTR DS:[0xF3ECBC]
$+511    0077EA21     0F2FC8                   COMISS XMM1,XMM0
$+514    0077EA24     77 09                    JA SHORT PathOfEx.0077EA2F
$+516    0077EA26     0F2FC4                   COMISS XMM0,XMM4
$+519    0077EA29     73 04                    JNB SHORT PathOfEx.0077EA2F
$+51B    0077EA2B     33C9                     XOR ECX,ECX
$+51D    0077EA2D     EB 05                    JMP SHORT PathOfEx.0077EA34
$+51F    0077EA2F     B9 01000000              MOV ECX,0x1
$+524    0077EA34     33C0                     XOR EAX,EAX
$+526    0077EA36     84C9                     TEST CL,CL
$+528    0077EA38     6A 00                    PUSH 0x0
$+52A    0077EA3A     0F94C0                   SETE AL
$+52D    0077EA3D     50                       PUSH EAX
$+52E    0077EA3E     8D85 64FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x19C]
$+534    0077EA44     50                       PUSH EAX
$+535    0077EA45     E8 37CC4E00              CALL PathOfEx.00C6B681
$+53A    0077EA4A     8BF8                     MOV EDI,EAX
$+53C    0077EA4C     8D85 1CFFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xE4]
$+542    0077EA52     E9 8E030000              JMP PathOfEx.0077EDE5
$+547    0077EA57     66:0F6E0B                MOVD MM1,DWORD PTR DS:[EBX]
$+54B    0077EA5B     0F5BC9                   CVTDQ2PS XMM1,XMM1
$+54E    0077EA5E     F3:0F5ECA                DIVSS XMM1,XMM2
$+552    0077EA62     0F28C1                   MOVAPS XMM0,XMM1
$+555    0077EA65     F3:0F114D 64             MOVSS DWORD PTR SS:[EBP+0x64],XMM1
$+55A    0077EA6A     F3:0F59C2                MULSS XMM0,XMM2
$+55E    0077EA6E     F3:0F2CC0                CVTTSS2SI EAX,XMM0
$+562    0077EA72     99                       CDQ
$+563    0077EA73     F7FF                     IDIV EDI
$+565    0077EA75     85D2                     TEST EDX,EDX
$+567    0077EA77     75 26                    JNZ SHORT PathOfEx.0077EA9F
$+569    0077EA79     F3:0F59CB                MULSS XMM1,XMM3
$+56D    0077EA7D     F3:0F2CC1                CVTTSS2SI EAX,XMM1
$+571    0077EA81     99                       CDQ
$+572    0077EA82     F7FF                     IDIV EDI
$+574    0077EA84     85D2                     TEST EDX,EDX
$+576    0077EA86     75 10                    JNZ SHORT PathOfEx.0077EA98
$+578    0077EA88     0F57C0                   XORPS XMM0,XMM0
$+57B    0077EA8B     66:0F1345 5C             MOVLPS QWORD PTR SS:[EBP+0x5C],XMM0
$+580    0077EA90     8B45 60                  MOV EAX,DWORD PTR SS:[EBP+0x60]
$+583    0077EA93     8B4D 5C                  MOV ECX,DWORD PTR SS:[EBP+0x5C]
$+586    0077EA96     EB 0E                    JMP SHORT PathOfEx.0077EAA6
$+588    0077EA98     B9 01000000              MOV ECX,0x1
$+58D    0077EA9D     EB 05                    JMP SHORT PathOfEx.0077EAA4
$+58F    0077EA9F     B9 02000000              MOV ECX,0x2
$+594    0077EAA4     33C0                     XOR EAX,EAX
$+596    0077EAA6     50                       PUSH EAX
$+597    0077EAA7     51                       PUSH ECX
$+598    0077EAA8     8D85 04FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1FC]
$+59E    0077EAAE     50                       PUSH EAX
$+59F    0077EAAF     E8 CDCB4E00              CALL PathOfEx.00C6B681
$+5A4    0077EAB4     8BF8                     MOV EDI,EAX
$+5A6    0077EAB6     8D85 14FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xEC]
$+5AC    0077EABC     E9 24030000              JMP PathOfEx.0077EDE5
$+5B1    0077EAC1     66:0F6E03                MOVD MM0,DWORD PTR DS:[EBX]
$+5B5    0077EAC5     8D85 44FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1BC]
$+5BB    0077EACB     0F5BC0                   CVTDQ2PS XMM0,XMM0
$+5BE    0077EACE     6A 00                    PUSH 0x0
*/
//tag_得到技能属性数值
/*
$-21     00624B4F     CC                       INT3
$-20     00624B50     56                       PUSH ESI
$-1F     00624B51     8BF1                     MOV ESI,ECX
$-1D     00624B53     8B4C24 08                MOV ECX,DWORD PTR SS:[ESP+0x8]
$-19     00624B57     68 78BFE800              PUSH PathOfEx.00E8BF78                            ; ACTION_KEYS
$-14     00624B5C     E8 6FC4E4FF              CALL PathOfEx.00470FD0
$-F      00624B61     85C0                     TEST EAX,EAX
$-D      00624B63     75 07                    JNZ SHORT PathOfEx.00624B6C
$-B      00624B65     8BCE                     MOV ECX,ESI
$-9      00624B67     E8 04FDFFFF              CALL PathOfEx.00624870
$-4      00624B6C     5E                       POP ESI
$-3      00624B6D     C2 0800                  RETN 0x8
$ ==>    00624B70     55                       PUSH EBP                                          ; tag_得到技能属性数值
$+1      00624B71     8BEC                     MOV EBP,ESP                                       ; ecx=stCD_SkillAttrObj*
$+3      00624B73     6A FF                    PUSH -0x1
$+5      00624B75     68 D616D200              PUSH PathOfEx.00D216D6                            ; 咐隰
$+A      00624B7A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+10     00624B80     50                       PUSH EAX
$+11     00624B81     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+18     00624B88     83EC 08                  SUB ESP,0x8
$+1B     00624B8B     53                       PUSH EBX
$+1C     00624B8C     56                       PUSH ESI
$+1D     00624B8D     57                       PUSH EDI
$+1E     00624B8E     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
$+21     00624B91     8BF1                     MOV ESI,ECX
$+23     00624B93     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
$+26     00624B96     8D47 FF                  LEA EAX,DWORD PTR DS:[EDI-0x1]
$+29     00624B99     50                       PUSH EAX
$+2A     00624B9A     8D56 08                  LEA EDX,DWORD PTR DS:[ESI+0x8]
$+2D     00624B9D     E8 7E1A1600              CALL PathOfEx.00786620
$+32     00624BA2     83C4 04                  ADD ESP,0x4
$+35     00624BA5     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+38     00624BA8     8A58 11                  MOV BL,BYTE PTR DS:[EAX+0x11]
$+3B     00624BAB     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+42     00624BB2     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
$+45     00624BB5     85C0                     TEST EAX,EAX
$+47     00624BB7     74 18                    JE SHORT PathOfEx.00624BD1
$+49     00624BB9     50                       PUSH EAX
$+4A     00624BBA     51                       PUSH ECX
$+4B     00624BBB     E8 40340F00              CALL PathOfEx.00718000
$+50     00624BC0     83C4 04                  ADD ESP,0x4
$+53     00624BC3     8BC8                     MOV ECX,EAX
$+55     00624BC5     E8 C62F0F00              CALL PathOfEx.00717B90
$+5A     00624BCA     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
$+61     00624BD1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+68     00624BD8     8B4E 04                  MOV ECX,DWORD PTR DS:[ESI+0x4]
$+6B     00624BDB     57                       PUSH EDI
$+6C     00624BDC     84DB                     TEST BL,BL
$+6E     00624BDE     74 17                    JE SHORT PathOfEx.00624BF7
$+70     00624BE0     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+72     00624BE2     FF10                     CALL DWORD PTR DS:[EAX]
$+74     00624BE4     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+77     00624BE7     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+7E     00624BEE     5F                       POP EDI
$+7F     00624BEF     5E                       POP ESI
$+80     00624BF0     5B                       POP EBX
$+81     00624BF1     8BE5                     MOV ESP,EBP
$+83     00624BF3     5D                       POP EBP
$+84     00624BF4     C2 0400                  RETN 0x4
$+87     00624BF7     E8 64392C00              CALL PathOfEx.008E8560
$+8C     00624BFC     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+8F     00624BFF     5F                       POP EDI
$+90     00624C00     5E                       POP ESI
$+91     00624C01     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+98     00624C08     5B                       POP EBX
$+99     00624C09     8BE5                     MOV ESP,EBP
$+9B     00624C0B     5D                       POP EBP
$+9C     00624C0C     C2 0400                  RETN 0x4
$+9F     00624C0F     CC                       INT3
$+A0     00624C10     55                       PUSH EBP
$+A1     00624C11     8BEC                     MOV EBP,ESP
$+A3     00624C13     6A FF                    PUSH -0x1
$+A5     00624C15     68 86A2D300              PUSH PathOfEx.00D3A286                            ; 袱饿
$+AA     00624C1A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+B0     00624C20     50                       PUSH EAX
$+B1     00624C21     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+B8     00624C28     51                       PUSH ECX
$+B9     00624C29     56                       PUSH ESI
$+BA     00624C2A     57                       PUSH EDI
$+BB     00624C2B     8BF9                     MOV EDI,ECX
$+BD     00624C2D     83EC 28                  SUB ESP,0x28
$+C0     00624C30     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+C7     00624C37     8BF4                     MOV ESI,ESP
$+C9     00624C39     8975 F0                  MOV DWORD PTR SS:[EBP-0x10],ESI
$+CC     00624C3C     C746 24 00000000         MOV DWORD PTR DS:[ESI+0x24],0x0
$+D3     00624C43     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+D7     00624C47     8B4D 2C                  MOV ECX,DWORD PTR SS:[EBP+0x2C]
$+DA     00624C4A     85C9                     TEST ECX,ECX
$+DC     00624C4C     74 08                    JE SHORT PathOfEx.00624C56
$+DE     00624C4E     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+E0     00624C50     56                       PUSH ESI
$+E1     00624C51     FF10                     CALL DWORD PTR DS:[EAX]
$+E3     00624C53     8946 24                  MOV DWORD PTR DS:[ESI+0x24],EAX
$+E6     00624C56     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
$+EA     00624C5A     8B4F 04                  MOV ECX,DWORD PTR DS:[EDI+0x4]
$+ED     00624C5D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
$+EF     00624C5F     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+F2     00624C62     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$+F9     00624C69     8B4D 2C                  MOV ECX,DWORD PTR SS:[EBP+0x2C]
$+FC     00624C6C     85C9                     TEST ECX,ECX
$+FE     00624C6E     74 11                    JE SHORT PathOfEx.00624C81
$+100    00624C70     8B11                     MOV EDX,DWORD PTR DS:[ECX]
$+102    00624C72     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
$+105    00624C75     3BC8                     CMP ECX,EAX
$+107    00624C77     0F95C0                   SETNE AL
$+10A    00624C7A     0FB6C0                   MOVZX EAX,AL
$+10D    00624C7D     50                       PUSH EAX
$+10E    00624C7E     FF52 10                  CALL DWORD PTR DS:[EDX+0x10]
$+111    00624C81     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+114    00624C84     5F                       POP EDI
$+115    00624C85     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+11C    00624C8C     5E                       POP ESI
$+11D    00624C8D     8BE5                     MOV ESP,EBP
$+11F    00624C8F     5D                       POP EBP
$+120    00624C90     C2 2800                  RETN 0x28
$+123    00624C93     CC                       INT3
*/
//tag_添加字符串到技能tips中
/*
$-51     00A0AD7F     CC                       INT3
$-50     00A0AD80     51                       PUSH ECX
$-4F     00A0AD81     80B9 200B0000 00         CMP BYTE PTR DS:[ECX+0xB20],0x0
$-48     00A0AD88     56                       PUSH ESI
$-47     00A0AD89     8DB1 C00A0000            LEA ESI,DWORD PTR DS:[ECX+0xAC0]
$-41     00A0AD8F     C74424 04 00000000       MOV DWORD PTR SS:[ESP+0x4],0x0
$-39     00A0AD97     74 07                    JE SHORT PathOfEx.00A0ADA0
$-37     00A0AD99     8BCE                     MOV ECX,ESI
$-35     00A0AD9B     E8 A0DD0000              CALL PathOfEx.00A18B40
$-30     00A0ADA0     8D4E 2C                  LEA ECX,DWORD PTR DS:[ESI+0x2C]
$-2D     00A0ADA3     33C0                     XOR EAX,EAX
$-2B     00A0ADA5     8B7424 0C                MOV ESI,DWORD PTR SS:[ESP+0xC]
$-27     00A0ADA9     6A FF                    PUSH -0x1
$-25     00A0ADAB     50                       PUSH EAX
$-24     00A0ADAC     51                       PUSH ECX
$-23     00A0ADAD     C746 14 07000000         MOV DWORD PTR DS:[ESI+0x14],0x7
$-1C     00A0ADB4     8BCE                     MOV ECX,ESI
$-1A     00A0ADB6     C746 10 00000000         MOV DWORD PTR DS:[ESI+0x10],0x0
$-13     00A0ADBD     66:8906                  MOV WORD PTR DS:[ESI],AX
$-10     00A0ADC0     E8 BB2AA6FF              CALL PathOfEx.0046D880
$-B      00A0ADC5     8BC6                     MOV EAX,ESI
$-9      00A0ADC7     5E                       POP ESI
$-8      00A0ADC8     59                       POP ECX
$-7      00A0ADC9     C2 0400                  RETN 0x4
$-4      00A0ADCC     CC                       INT3
$-3      00A0ADCD     CC                       INT3
$-2      00A0ADCE     CC                       INT3
$-1      00A0ADCF     CC                       INT3
$ ==>    00A0ADD0     56                       PUSH ESI                                          ; tag_添加字符串到技能tips中
$+1      00A0ADD1     FF7424 08                PUSH DWORD PTR SS:[ESP+0x8]                       ; p1=const std::wstring*
$+5      00A0ADD5     8DB1 50F7FFFF            LEA ESI,DWORD PTR DS:[ECX-0x8B0]
$+B      00A0ADDB     8D8E C00A0000            LEA ECX,DWORD PTR DS:[ESI+0xAC0]
$+11     00A0ADE1     E8 AADB0000              CALL PathOfEx.00A18990
$+16     00A0ADE6     84C0                     TEST AL,AL
$+18     00A0ADE8     74 4B                    JE SHORT PathOfEx.00A0AE35
$+1A     00A0ADEA     6A 01                    PUSH 0x1
$+1C     00A0ADEC     8BCE                     MOV ECX,ESI
$+1E     00A0ADEE     E8 BD0E0000              CALL PathOfEx.00A0BCB0
$+23     00A0ADF3     80BE 7C0B0000 00         CMP BYTE PTR DS:[ESI+0xB7C],0x0
$+2A     00A0ADFA     C686 6C0B0000 01         MOV BYTE PTR DS:[ESI+0xB6C],0x1
$+31     00A0AE01     75 28                    JNZ SHORT PathOfEx.00A0AE2B
$+33     00A0AE03     0FB686 A5080000          MOVZX EAX,BYTE PTR DS:[ESI+0x8A5]
$+3A     00A0AE0A     8BCE                     MOV ECX,ESI
$+3C     00A0AE0C     8B16                     MOV EDX,DWORD PTR DS:[ESI]
$+3E     00A0AE0E     50                       PUSH EAX
$+3F     00A0AE0F     0FB686 A4080000          MOVZX EAX,BYTE PTR DS:[ESI+0x8A4]
$+46     00A0AE16     50                       PUSH EAX
$+47     00A0AE17     C686 7C0B0000 01         MOV BYTE PTR DS:[ESI+0xB7C],0x1
$+4E     00A0AE1E     FF92 38010000            CALL DWORD PTR DS:[EDX+0x138]
$+54     00A0AE24     C686 7C0B0000 00         MOV BYTE PTR DS:[ESI+0xB7C],0x0
$+5B     00A0AE2B     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+5D     00A0AE2D     8BCE                     MOV ECX,ESI
$+5F     00A0AE2F     FF90 84010000            CALL DWORD PTR DS:[EAX+0x184]
$+65     00A0AE35     5E                       POP ESI
$+66     00A0AE36     C2 0400                  RETN 0x4
$+69     00A0AE39     CC                       INT3
$+6A     00A0AE3A     CC                       INT3
$+6B     00A0AE3B     CC                       INT3
$+6C     00A0AE3C     CC                       INT3
$+6D     00A0AE3D     CC                       INT3
$+6E     00A0AE3E     CC                       INT3
$+6F     00A0AE3F     CC                       INT3
$+70     00A0AE40     56                       PUSH ESI
$+71     00A0AE41     FF7424 08                PUSH DWORD PTR SS:[ESP+0x8]
$+75     00A0AE45     8BF1                     MOV ESI,ECX
$+77     00A0AE47     8D8E C00A0000            LEA ECX,DWORD PTR DS:[ESI+0xAC0]
$+7D     00A0AE4D     E8 3EDB0000              CALL PathOfEx.00A18990
$+82     00A0AE52     84C0                     TEST AL,AL
$+84     00A0AE54     74 52                    JE SHORT PathOfEx.00A0AEA8
$+86     00A0AE56     6A 01                    PUSH 0x1
$+88     00A0AE58     8BCE                     MOV ECX,ESI
$+8A     00A0AE5A     E8 510E0000              CALL PathOfEx.00A0BCB0
$+8F     00A0AE5F     80BE 7C0B0000 00         CMP BYTE PTR DS:[ESI+0xB7C],0x0
$+96     00A0AE66     C686 6C0B0000 01         MOV BYTE PTR DS:[ESI+0xB6C],0x1
$+9D     00A0AE6D     75 28                    JNZ SHORT PathOfEx.00A0AE97
$+9F     00A0AE6F     0FB686 A5080000          MOVZX EAX,BYTE PTR DS:[ESI+0x8A5]
$+A6     00A0AE76     8BCE                     MOV ECX,ESI
$+A8     00A0AE78     8B16                     MOV EDX,DWORD PTR DS:[ESI]
$+AA     00A0AE7A     50                       PUSH EAX
$+AB     00A0AE7B     0FB686 A4080000          MOVZX EAX,BYTE PTR DS:[ESI+0x8A4]
$+B2     00A0AE82     50                       PUSH EAX
$+B3     00A0AE83     C686 7C0B0000 01         MOV BYTE PTR DS:[ESI+0xB7C],0x1
$+BA     00A0AE8A     FF92 38010000            CALL DWORD PTR DS:[EDX+0x138]
$+C0     00A0AE90     C686 7C0B0000 00         MOV BYTE PTR DS:[ESI+0xB7C],0x0
$+C7     00A0AE97     807C24 0C 00             CMP BYTE PTR SS:[ESP+0xC],0x0
$+CC     00A0AE9C     75 0A                    JNZ SHORT PathOfEx.00A0AEA8
$+CE     00A0AE9E     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+D0     00A0AEA0     8BCE                     MOV ECX,ESI
$+D2     00A0AEA2     FF90 84010000            CALL DWORD PTR DS:[EAX+0x184]
$+D8     00A0AEA8     5E                       POP ESI
$+D9     00A0AEA9     C2 0800                  RETN 0x8
$+DC     00A0AEAC     CC                       INT3
*/
//tag_actor带技能次数使用技能
/*
$-D1     007A6A0F     CC                        INT3
$-D0     007A6A10     55                        PUSH EBP
$-CF     007A6A11     8BEC                      MOV EBP,ESP
$-CD     007A6A13     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$-C7     007A6A19     6A FF                     PUSH -0x1
$-C5     007A6A1B     68 78EFD500               PUSH PathOfEx.00D5EF78
$-C0     007A6A20     50                        PUSH EAX
$-BF     007A6A21     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$-B8     007A6A28     83EC 08                   SUB ESP,0x8
$-B5     007A6A2B     8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
$-B2     007A6A2E     53                        PUSH EBX
$-B1     007A6A2F     56                        PUSH ESI
$-B0     007A6A30     57                        PUSH EDI
$-AF     007A6A31     8BF9                      MOV EDI,ECX
$-AD     007A6A33     8B77 04                   MOV ESI,DWORD PTR DS:[EDI+0x4]
$-AA     007A6A36     3B46 14                   CMP EAX,DWORD PTR DS:[ESI+0x14]
$-A7     007A6A39     74 0A                     JE SHORT PathOfEx.007A6A45
$-A5     007A6A3B     50                        PUSH EAX
$-A4     007A6A3C     8BCE                      MOV ECX,ESI
$-A2     007A6A3E     E8 CDEDFEFF               CALL PathOfEx.00795810
$-9D     007A6A43     8BF0                      MOV ESI,EAX
$-9B     007A6A45     85F6                      TEST ESI,ESI
$-99     007A6A47     75 15                     JNZ SHORT PathOfEx.007A6A5E
$-97     007A6A49     32C0                      XOR AL,AL
$-95     007A6A4B     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-92     007A6A4E     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-8B     007A6A55     5F                        POP EDI
$-8A     007A6A56     5E                        POP ESI
$-89     007A6A57     5B                        POP EBX
$-88     007A6A58     8BE5                      MOV ESP,EBP
$-86     007A6A5A     5D                        POP EBP
$-85     007A6A5B     C2 1000                   RETN 0x10
$-82     007A6A5E     8D47 38                   LEA EAX,DWORD PTR DS:[EDI+0x38]
$-7F     007A6A61     8BCF                      MOV ECX,EDI
$-7D     007A6A63     50                        PUSH EAX
$-7C     007A6A64     FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
$-79     007A6A67     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]
$-76     007A6A6A     50                        PUSH EAX
$-75     007A6A6B     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-73     007A6A6D     FF50 54                   CALL DWORD PTR DS:[EAX+0x54]
$-70     007A6A70     8BC8                      MOV ECX,EAX
$-6E     007A6A72     E8 C9AC0200               CALL PathOfEx.007D1740
$-69     007A6A77     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$-62     007A6A7E     8B5D 10                   MOV EBX,DWORD PTR SS:[EBP+0x10]
$-5F     007A6A81     85DB                      TEST EBX,EBX
$-5D     007A6A83   ^ 74 C4                     JE SHORT PathOfEx.007A6A49
$-5B     007A6A85     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
$-58     007A6A88     8BCB                      MOV ECX,EBX
$-56     007A6A8A     E8 71293400               CALL PathOfEx.00AE9400
$-51     007A6A8F     56                        PUSH ESI
$-50     007A6A90     8BCB                      MOV ECX,EBX
$-4E     007A6A92     E8 89293400               CALL PathOfEx.00AE9420
$-49     007A6A97     8B4E 20                   MOV ECX,DWORD PTR DS:[ESI+0x20]
$-46     007A6A9A     C745 EC 00000080          MOV DWORD PTR SS:[EBP-0x14],0x80000000
$-3F     007A6AA1     C745 F0 00000080          MOV DWORD PTR SS:[EBP-0x10],0x80000000
$-38     007A6AA8     C745 10 00000000          MOV DWORD PTR SS:[EBP+0x10],0x0
$-31     007A6AAF     8B41 10                   MOV EAX,DWORD PTR DS:[ECX+0x10]
$-2E     007A6AB2     8943 20                   MOV DWORD PTR DS:[EBX+0x20],EAX
$-2B     007A6AB5     8B41 14                   MOV EAX,DWORD PTR DS:[ECX+0x14]
$-28     007A6AB8     8BCF                      MOV ECX,EDI
$-26     007A6ABA     8943 24                   MOV DWORD PTR DS:[EBX+0x24],EAX
$-23     007A6ABD     8A45 14                   MOV AL,BYTE PTR SS:[EBP+0x14]
$-20     007A6AC0     8843 17                   MOV BYTE PTR DS:[EBX+0x17],AL
$-1D     007A6AC3     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$-1A     007A6AC6     50                        PUSH EAX
$-19     007A6AC7     53                        PUSH EBX
$-18     007A6AC8     E8 B3FDFFFF               CALL PathOfEx.007A6880
$-13     007A6ACD     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$-10     007A6AD0     5F                        POP EDI
$-F      007A6AD1     5E                        POP ESI
$-E      007A6AD2     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$-7      007A6AD9     5B                        POP EBX
$-6      007A6ADA     8BE5                      MOV ESP,EBP
$-4      007A6ADC     5D                        POP EBP
$-3      007A6ADD     C2 1000                   RETN 0x10
$ ==>    007A6AE0     55                        PUSH EBP                                          ; tag_actor带技能次数使用技能
$+1      007A6AE1     8BEC                      MOV EBP,ESP                                       ; p1=use_cnt
$+3      007A6AE3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]                          ; p2=skill_id
$+9      007A6AE9     6A FF                     PUSH -0x1                                         ; p3=const stCD_VecInt*,=dst_pos
$+B      007A6AEB     68 78EFD500               PUSH PathOfEx.00D5EF78                            ; p4=kTTSF_Normal,=8
$+10     007A6AF0     50                        PUSH EAX                                          ; ecx=stCD_Actor*
$+11     007A6AF1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     007A6AF8     83EC 08                   SUB ESP,0x8
$+1B     007A6AFB     53                        PUSH EBX
$+1C     007A6AFC     56                        PUSH ESI
$+1D     007A6AFD     57                        PUSH EDI
$+1E     007A6AFE     8BF9                      MOV EDI,ECX
$+20     007A6B00     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]                    ; then,eax=stCD_EntityPosBased*
$+23     007A6B03     8B48 10                   MOV ECX,DWORD PTR DS:[EAX+0x10]                   ; then,ecx=stCD_World*
$+26     007A6B06     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+28     007A6B08     FF50 0C                   CALL DWORD PTR DS:[EAX+0xC]                       ; call GetMapData,then,eax=stCD_MapData*
$+2B     007A6B0B     8B5D 10                   MOV EBX,DWORD PTR SS:[EBP+0x10]                   ; then,ebx=p3
$+2E     007A6B0E     8BD0                      MOV EDX,EAX
$+30     007A6B10     8B33                      MOV ESI,DWORD PTR DS:[EBX]
$+32     007A6B12     85F6                      TEST ESI,ESI
$+34     007A6B14     0F88 91000000             JS PathOfEx.007A6BAB
$+3A     007A6B1A     8B4B 04                   MOV ECX,DWORD PTR DS:[EBX+0x4]
$+3D     007A6B1D     85C9                      TEST ECX,ECX
$+3F     007A6B1F     0F88 86000000             JS PathOfEx.007A6BAB
$+45     007A6B25     6B42 34 17                IMUL EAX,DWORD PTR DS:[EDX+0x34],0x17
$+49     007A6B29     3BF0                      CMP ESI,EAX
$+4B     007A6B2B     7D 7E                     JGE SHORT PathOfEx.007A6BAB
$+4D     007A6B2D     6B42 38 17                IMUL EAX,DWORD PTR DS:[EDX+0x38],0x17
$+51     007A6B31     3BC8                      CMP ECX,EAX
$+53     007A6B33     7D 76                     JGE SHORT PathOfEx.007A6BAB
$+55     007A6B35     8D47 38                   LEA EAX,DWORD PTR DS:[EDI+0x38]
$+58     007A6B38     8BCF                      MOV ECX,EDI
$+5A     007A6B3A     50                        PUSH EAX
$+5B     007A6B3B     FF75 0C                   PUSH DWORD PTR SS:[EBP+0xC]
$+5E     007A6B3E     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]                   ; 返回的技能对象保存在此处
$+61     007A6B41     50                        PUSH EAX
$+62     007A6B42     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+64     007A6B44     FF50 54                   CALL DWORD PTR DS:[EAX+0x54]
$+67     007A6B47     8BC8                      MOV ECX,EAX
$+69     007A6B49     E8 F2AB0200               CALL PathOfEx.007D1740
$+6E     007A6B4E     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+75     007A6B55     8B75 10                   MOV ESI,DWORD PTR SS:[EBP+0x10]                   ; then,esi=技能对象
$+78     007A6B58     85F6                      TEST ESI,ESI                                      ; esi=stCD_SkillObj*
$+7A     007A6B5A     74 4F                     JE SHORT PathOfEx.007A6BAB
$+7C     007A6B5C     FF75 08                   PUSH DWORD PTR SS:[EBP+0x8]
$+7F     007A6B5F     8BCE                      MOV ECX,ESI
$+81     007A6B61     E8 9A283400               CALL PathOfEx.00AE9400
$+86     007A6B66     8B03                      MOV EAX,DWORD PTR DS:[EBX]                        ; then,eax=dst_pos.x
$+88     007A6B68     8BCF                      MOV ECX,EDI
$+8A     007A6B6A     8946 20                   MOV DWORD PTR DS:[ESI+0x20],EAX
$+8D     007A6B6D     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]                    ; then,eax=dst_pos.y
$+90     007A6B70     8946 24                   MOV DWORD PTR DS:[ESI+0x24],EAX
$+93     007A6B73     8A45 14                   MOV AL,BYTE PTR SS:[EBP+0x14]
$+96     007A6B76     8846 17                   MOV BYTE PTR DS:[ESI+0x17],AL                     ; g_so_skill_obj_ukn_bool_
$+99     007A6B79     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$+9C     007A6B7C     50                        PUSH EAX
$+9D     007A6B7D     56                        PUSH ESI
$+9E     007A6B7E     C745 EC 00000080          MOV DWORD PTR SS:[EBP-0x14],0x80000000
$+A5     007A6B85     C745 F0 00000080          MOV DWORD PTR SS:[EBP-0x10],0x80000000
$+AC     007A6B8C     C745 10 00000000          MOV DWORD PTR SS:[EBP+0x10],0x0
$+B3     007A6B93     E8 E8FCFFFF               CALL PathOfEx.007A6880
$+B8     007A6B98     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+BB     007A6B9B     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+C2     007A6BA2     5F                        POP EDI
$+C3     007A6BA3     5E                        POP ESI
$+C4     007A6BA4     5B                        POP EBX
$+C5     007A6BA5     8BE5                      MOV ESP,EBP
$+C7     007A6BA7     5D                        POP EBP
$+C8     007A6BA8     C2 1000                   RETN 0x10
$+CB     007A6BAB     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+CE     007A6BAE     32C0                      XOR AL,AL
$+D0     007A6BB0     5F                        POP EDI
$+D1     007A6BB1     5E                        POP ESI
$+D2     007A6BB2     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+D9     007A6BB9     5B                        POP EBX
$+DA     007A6BBA     8BE5                      MOV ESP,EBP
$+DC     007A6BBC     5D                        POP EBP
$+DD     007A6BBD     C2 1000                   RETN 0x10
$+E0     007A6BC0     55                        PUSH EBP
$+E1     007A6BC1     8BEC                      MOV EBP,ESP
$+E3     007A6BC3     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+E9     007A6BC9     6A FF                     PUSH -0x1
$+EB     007A6BCB     68 E094D200               PUSH PathOfEx.00D294E0
$+F0     007A6BD0     50                        PUSH EAX
$+F1     007A6BD1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+F8     007A6BD8     56                        PUSH ESI
$+F9     007A6BD9     8BF1                      MOV ESI,ECX
$+FB     007A6BDB     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+FD     007A6BDD     FF88 84000000             DEC DWORD PTR DS:[EAX+0x84]
$+103    007A6BE3     8B16                      MOV EDX,DWORD PTR DS:[ESI]
$+105    007A6BE5     83BA 84000000 00          CMP DWORD PTR DS:[EDX+0x84],0x0
$+10C    007A6BEC     75 3F                     JNZ SHORT PathOfEx.007A6C2D
$+10E    007A6BEE     8B4A 70                   MOV ECX,DWORD PTR DS:[EDX+0x70]
$+111    007A6BF1     8BC1                      MOV EAX,ECX
$+113    007A6BF3     C1E8 02                   SHR EAX,0x2
$+116    007A6BF6     A8 01                     TEST AL,0x1
$+118    007A6BF8     74 20                     JE SHORT PathOfEx.007A6C1A
$+11A    007A6BFA     8362 70 FB                AND DWORD PTR DS:[EDX+0x70],0xFFFFFFFB
$+11E    007A6BFE     8B06                      MOV EAX,DWORD PTR DS:[ESI]
$+120    007A6C00     8360 70 F7                AND DWORD PTR DS:[EAX+0x70],0xFFFFFFF7
$+124    007A6C04     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+126    007A6C06     E8 A5080000               CALL PathOfEx.007A74B0
$+12B    007A6C0B     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+12E    007A6C0E     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+135    007A6C15     5E                        POP ESI
$+136    007A6C16     8BE5                      MOV ESP,EBP
$+138    007A6C18     5D                        POP EBP
$+139    007A6C19     C3                        RETN
$+13A    007A6C1A     C1E9 03                   SHR ECX,0x3
$+13D    007A6C1D     F6C1 01                   TEST CL,0x1
$+140    007A6C20     74 0B                     JE SHORT PathOfEx.007A6C2D
$+142    007A6C22     8362 70 F7                AND DWORD PTR DS:[EDX+0x70],0xFFFFFFF7
$+146    007A6C26     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
$+148    007A6C28     E8 53090000               CALL PathOfEx.007A7580
$+14D    007A6C2D     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+150    007A6C30     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+157    007A6C37     5E                        POP ESI
$+158    007A6C38     8BE5                      MOV ESP,EBP
$+15A    007A6C3A     5D                        POP EBP
$+15B    007A6C3B     C3                        RETN
$+15C    007A6C3C     CC                        INT3
*/
//tag_InGameState不断地写入鼠标坐标
/*
$-1B1    0068DBCF     CC                        INT3
$-1B0    0068DBD0     55                        PUSH EBP
$-1AF    0068DBD1     8BEC                      MOV EBP,ESP
$-1AD    0068DBD3     83E4 F8                   AND ESP,0xFFFFFFF8
$-1AA    0068DBD6     83EC 28                   SUB ESP,0x28
$-1A7    0068DBD9     53                        PUSH EBX
$-1A6    0068DBDA     8BD9                      MOV EBX,ECX
$-1A4    0068DBDC     55                        PUSH EBP
$-1A3    0068DBDD     56                        PUSH ESI
$-1A2    0068DBDE     57                        PUSH EDI
$-1A1    0068DBDF     8B83 3C010000             MOV EAX,DWORD PTR DS:[EBX+0x13C]
$-19B    0068DBE5     8B80 94380000             MOV EAX,DWORD PTR DS:[EAX+0x3894]
$-195    0068DBEB     8BB8 44010000             MOV EDI,DWORD PTR DS:[EAX+0x144]
$-18F    0068DBF1     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$-18D    0068DBF3     85C9                      TEST ECX,ECX
$-18B    0068DBF5     74 05                     JE SHORT PathOfEx.0068DBFC
$-189    0068DBF7     E8 14890400               CALL PathOfEx.006D6510
$-184    0068DBFC     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$-182    0068DBFE     C74424 10 789CEF00        MOV DWORD PTR SS:[ESP+0x10],PathOfEx.00EF9C78     ; ASCII "Player"
$-17A    0068DC06     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
$-177    0068DC09     8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
$-173    0068DC0D     50                        PUSH EAX
$-172    0068DC0E     8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
$-16E    0068DC12     50                        PUSH EAX
$-16D    0068DC13     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$-16A    0068DC16     E8 A5ACDAFF               CALL PathOfEx.004388C0
$-165    0068DC1B     8B4424 14                 MOV EAX,DWORD PTR SS:[ESP+0x14]
$-161    0068DC1F     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$-15E    0068DC22     74 10                     JE SHORT PathOfEx.0068DC34
$-15C    0068DC24     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$-159    0068DC27     83F9 FF                   CMP ECX,-0x1
$-156    0068DC2A     74 08                     JE SHORT PathOfEx.0068DC34
$-154    0068DC2C     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$-151    0068DC2F     8B0488                    MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$-14E    0068DC32     EB 02                     JMP SHORT PathOfEx.0068DC36
$-14C    0068DC34     33C0                      XOR EAX,EAX
$-14A    0068DC36     83C0 10                   ADD EAX,0x10
$-147    0068DC39     8D8B E81A0000             LEA ECX,DWORD PTR DS:[EBX+0x1AE8]
$-141    0068DC3F     50                        PUSH EAX
$-140    0068DC40     894424 18                 MOV DWORD PTR SS:[ESP+0x18],EAX
$-13C    0068DC44     E8 87F3DAFF               CALL PathOfEx.0043CFD0
$-137    0068DC49     8BBB E81A0000             MOV EDI,DWORD PTR DS:[EBX+0x1AE8]
$-131    0068DC4F     8BF0                      MOV ESI,EAX
$-12F    0068DC51     3BF7                      CMP ESI,EDI
$-12D    0068DC53     74 11                     JE SHORT PathOfEx.0068DC66
$-12B    0068DC55     8D46 10                   LEA EAX,DWORD PTR DS:[ESI+0x10]
$-128    0068DC58     50                        PUSH EAX
$-127    0068DC59     FF7424 18                 PUSH DWORD PTR SS:[ESP+0x18]
$-123    0068DC5D     E8 0EF3DAFF               CALL PathOfEx.0043CF70
$-11E    0068DC62     84C0                      TEST AL,AL
$-11C    0068DC64     74 02                     JE SHORT PathOfEx.0068DC68
$-11A    0068DC66     8BF7                      MOV ESI,EDI
$-118    0068DC68     3BF7                      CMP ESI,EDI
$-116    0068DC6A     74 68                     JE SHORT PathOfEx.0068DCD4
$-114    0068DC6C     8D4424 18                 LEA EAX,DWORD PTR SS:[ESP+0x18]
$-110    0068DC70     C74424 18 00000000        MOV DWORD PTR SS:[ESP+0x18],0x0
$-108    0068DC78     0F57C0                    XORPS XMM0,XMM0
$-105    0068DC7B     C74424 1C 00000000        MOV DWORD PTR SS:[ESP+0x1C],0x0
$-FD     0068DC83     50                        PUSH EAX
$-FC     0068DC84     8D4E 28                   LEA ECX,DWORD PTR DS:[ESI+0x28]
$-F9     0068DC87     C74424 24 00000000        MOV DWORD PTR SS:[ESP+0x24],0x0
$-F1     0068DC8F     66:0F134424 2C            MOVLPS QWORD PTR SS:[ESP+0x2C],XMM0
$-EB     0068DC95     C74424 34 00000000        MOV DWORD PTR SS:[ESP+0x34],0x0
$-E3     0068DC9D     E8 3E08F2FF               CALL PathOfEx.005AE4E0
$-DE     0068DCA2     8B4424 28                 MOV EAX,DWORD PTR SS:[ESP+0x28]
$-DA     0068DCA6     8B4C24 18                 MOV ECX,DWORD PTR SS:[ESP+0x18]
$-D6     0068DCAA     8946 38                   MOV DWORD PTR DS:[ESI+0x38],EAX
$-D3     0068DCAD     8B4424 2C                 MOV EAX,DWORD PTR SS:[ESP+0x2C]
$-CF     0068DCB1     8946 3C                   MOV DWORD PTR DS:[ESI+0x3C],EAX
$-CC     0068DCB4     8B4424 30                 MOV EAX,DWORD PTR SS:[ESP+0x30]
$-C8     0068DCB8     8946 40                   MOV DWORD PTR DS:[ESI+0x40],EAX
$-C5     0068DCBB     85C9                      TEST ECX,ECX
$-C3     0068DCBD     74 15                     JE SHORT PathOfEx.0068DCD4
$-C1     0068DCBF     8B4424 20                 MOV EAX,DWORD PTR SS:[ESP+0x20]
$-BD     0068DCC3     2BC1                      SUB EAX,ECX
$-BB     0068DCC5     6A 08                     PUSH 0x8
$-B9     0068DCC7     C1F8 03                   SAR EAX,0x3
$-B6     0068DCCA     50                        PUSH EAX
$-B5     0068DCCB     51                        PUSH ECX
$-B4     0068DCCC     E8 DFB3DAFF               CALL PathOfEx.004390B0
$-AF     0068DCD1     83C4 0C                   ADD ESP,0xC
$-AC     0068DCD4     8B8B E0180000             MOV ECX,DWORD PTR DS:[EBX+0x18E0]
$-A6     0068DCDA     8B89 FC080000             MOV ECX,DWORD PTR DS:[ECX+0x8FC]
$-A0     0068DCE0     E8 EB72ECFF               CALL PathOfEx.00554FD0
$-9B     0068DCE5     5F                        POP EDI
$-9A     0068DCE6     5E                        POP ESI
$-99     0068DCE7     5D                        POP EBP
$-98     0068DCE8     5B                        POP EBX
$-97     0068DCE9     8BE5                      MOV ESP,EBP
$-95     0068DCEB     5D                        POP EBP
$-94     0068DCEC     C3                        RETN
$-93     0068DCED     CC                        INT3
$-92     0068DCEE     CC                        INT3
$-91     0068DCEF     CC                        INT3
$-90     0068DCF0     83EC 08                   SUB ESP,0x8
$-8D     0068DCF3     56                        PUSH ESI
$-8C     0068DCF4     8BF1                      MOV ESI,ECX
$-8A     0068DCF6     83BE 38010000 00          CMP DWORD PTR DS:[ESI+0x138],0x0
$-83     0068DCFD     74 75                     JE SHORT PathOfEx.0068DD74
$-81     0068DCFF     8B86 3C010000             MOV EAX,DWORD PTR DS:[ESI+0x13C]
$-7B     0068DD05     8B80 94380000             MOV EAX,DWORD PTR DS:[EAX+0x3894]
$-75     0068DD0B     8B88 44010000             MOV ECX,DWORD PTR DS:[EAX+0x144]
$-6F     0068DD11     E8 1A23DDFF               CALL PathOfEx.00460030
$-6A     0068DD16     83C0 10                   ADD EAX,0x10
$-67     0068DD19     8D8E E81A0000             LEA ECX,DWORD PTR DS:[ESI+0x1AE8]
$-61     0068DD1F     50                        PUSH EAX
$-60     0068DD20     8D4424 08                 LEA EAX,DWORD PTR SS:[ESP+0x8]
$-5C     0068DD24     50                        PUSH EAX
$-5B     0068DD25     E8 56770000               CALL PathOfEx.00695480
$-56     0068DD2A     807C24 10 00              CMP BYTE PTR SS:[ESP+0x10],0x0
$-51     0068DD2F     8B7424 04                 MOV ESI,DWORD PTR SS:[ESP+0x4]
$-4D     0068DD33     74 2F                     JE SHORT PathOfEx.0068DD64
$-4B     0068DD35     807C24 14 00              CMP BYTE PTR SS:[ESP+0x14],0x0
$-46     0068DD3A     75 38                     JNZ SHORT PathOfEx.0068DD74
$-44     0068DD3C     837E 40 00                CMP DWORD PTR DS:[ESI+0x40],0x0
$-40     0068DD40     76 32                     JBE SHORT PathOfEx.0068DD74
$-3E     0068DD42     FF15 D870D900             CALL DWORD PTR DS:[<&KERNEL32.GetTickCount>]      ; kernel32.GetTickCount
$-38     0068DD48     2B46 40                   SUB EAX,DWORD PTR DS:[ESI+0x40]
$-35     0068DD4B     8B4E 28                   MOV ECX,DWORD PTR DS:[ESI+0x28]
$-32     0068DD4E     3B4E 2C                   CMP ECX,DWORD PTR DS:[ESI+0x2C]
$-2F     0068DD51     74 21                     JE SHORT PathOfEx.0068DD74
$-2D     0068DD53     0101                      ADD DWORD PTR DS:[ECX],EAX
$-2B     0068DD55     83C1 08                   ADD ECX,0x8
$-28     0068DD58     3B4E 2C                   CMP ECX,DWORD PTR DS:[ESI+0x2C]
$-25     0068DD5B   ^ 75 F6                     JNZ SHORT PathOfEx.0068DD53
$-23     0068DD5D     5E                        POP ESI
$-22     0068DD5E     83C4 08                   ADD ESP,0x8
$-1F     0068DD61     C2 0800                   RETN 0x8
$-1C     0068DD64     807C24 14 00              CMP BYTE PTR SS:[ESP+0x14],0x0
$-17     0068DD69     74 09                     JE SHORT PathOfEx.0068DD74
$-15     0068DD6B     FF15 D870D900             CALL DWORD PTR DS:[<&KERNEL32.GetTickCount>]      ; kernel32.GetTickCount
$-F      0068DD71     8946 40                   MOV DWORD PTR DS:[ESI+0x40],EAX
$-C      0068DD74     5E                        POP ESI
$-B      0068DD75     83C4 08                   ADD ESP,0x8
$-8      0068DD78     C2 0800                   RETN 0x8
$-5      0068DD7B     CC                        INT3
$-4      0068DD7C     CC                        INT3
$-3      0068DD7D     CC                        INT3
$-2      0068DD7E     CC                        INT3
$-1      0068DD7F     CC                        INT3
$ ==>    0068DD80     55                        PUSH EBP                                          ; tag_InGameState不断地写入鼠标坐标
$+1      0068DD81     8BEC                      MOV EBP,ESP                                       ; ecx=stCD_SceneInGameState*
$+3      0068DD83     83E4 F0                   AND ESP,0xFFFFFFF0
$+6      0068DD86     81EC C4000000             SUB ESP,0xC4
$+C      0068DD8C     53                        PUSH EBX
$+D      0068DD8D     56                        PUSH ESI
$+E      0068DD8E     57                        PUSH EDI
$+F      0068DD8F     8BF9                      MOV EDI,ECX
$+11     0068DD91     897C24 10                 MOV DWORD PTR SS:[ESP+0x10],EDI
$+15     0068DD95     8B87 340C0000             MOV EAX,DWORD PTR DS:[EDI+0xC34]
$+1B     0068DD9B     8987 9C180000             MOV DWORD PTR DS:[EDI+0x189C],EAX
$+21     0068DDA1     8B87 380C0000             MOV EAX,DWORD PTR DS:[EDI+0xC38]
$+27     0068DDA7     8987 A0180000             MOV DWORD PTR DS:[EDI+0x18A0],EAX
$+2D     0068DDAD     8B87 38010000             MOV EAX,DWORD PTR DS:[EDI+0x138]
$+33     0068DDB3     85C0                      TEST EAX,EAX
$+35     0068DDB5     0F84 E1050000             JE PathOfEx.0068E39C
$+3B     0068DDBB     83B8 44010000 00          CMP DWORD PTR DS:[EAX+0x144],0x0
$+42     0068DDC2     0F84 D4050000             JE PathOfEx.0068E39C
$+48     0068DDC8     FFB7 A0180000             PUSH DWORD PTR DS:[EDI+0x18A0]
$+4E     0068DDCE     8B87 A0150000             MOV EAX,DWORD PTR DS:[EDI+0x15A0]
$+54     0068DDD4     8D8F A0150000             LEA ECX,DWORD PTR DS:[EDI+0x15A0]
$+5A     0068DDDA     FFB7 9C180000             PUSH DWORD PTR DS:[EDI+0x189C]
$+60     0068DDE0     8D9424 B8000000           LEA EDX,DWORD PTR SS:[ESP+0xB8]
$+67     0068DDE7     52                        PUSH EDX
$+68     0068DDE8     8D9F B8180000             LEA EBX,DWORD PTR DS:[EDI+0x18B8]
$+6E     0068DDEE     8DB7 AC180000             LEA ESI,DWORD PTR DS:[EDI+0x18AC]
$+74     0068DDF4     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
$+77     0068DDF7     8BD0                      MOV EDX,EAX
$+79     0068DDF9     8B0A                      MOV ECX,DWORD PTR DS:[EDX]
$+7B     0068DDFB     890E                      MOV DWORD PTR DS:[ESI],ECX
$+7D     0068DDFD     8B4A 04                   MOV ECX,DWORD PTR DS:[EDX+0x4]
$+80     0068DE00     894E 04                   MOV DWORD PTR DS:[ESI+0x4],ECX
$+83     0068DE03     8B4A 08                   MOV ECX,DWORD PTR DS:[EDX+0x8]
$+86     0068DE06     894E 08                   MOV DWORD PTR DS:[ESI+0x8],ECX
$+89     0068DE09     8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
$+8C     0068DE0C     8B8F 38010000             MOV ECX,DWORD PTR DS:[EDI+0x138]
$+92     0068DE12     8903                      MOV DWORD PTR DS:[EBX],EAX
$+94     0068DE14     8B42 10                   MOV EAX,DWORD PTR DS:[EDX+0x10]
$+97     0068DE17     8943 04                   MOV DWORD PTR DS:[EBX+0x4],EAX
$+9A     0068DE1A     8B42 14                   MOV EAX,DWORD PTR DS:[EDX+0x14]
$+9D     0068DE1D     8943 08                   MOV DWORD PTR DS:[EBX+0x8],EAX
$+A0     0068DE20     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+A2     0068DE22     FF50 0C                   CALL DWORD PTR DS:[EAX+0xC]
$+A5     0068DE25     8B8F 38010000             MOV ECX,DWORD PTR DS:[EDI+0x138]
$+AB     0068DE2B     894424 14                 MOV DWORD PTR SS:[ESP+0x14],EAX
$+AF     0068DE2F     8B89 44010000             MOV ECX,DWORD PTR DS:[ECX+0x144]
$+B5     0068DE35     8B51 20                   MOV EDX,DWORD PTR DS:[ECX+0x20]
$+B8     0068DE38     8B4A 1C                   MOV ECX,DWORD PTR DS:[EDX+0x1C]
$+BB     0068DE3B     894C24 24                 MOV DWORD PTR SS:[ESP+0x24],ECX
$+BF     0068DE3F     8B4A 20                   MOV ECX,DWORD PTR DS:[EDX+0x20]
$+C2     0068DE42     894C24 28                 MOV DWORD PTR SS:[ESP+0x28],ECX
$+C6     0068DE46     8D4C24 24                 LEA ECX,DWORD PTR SS:[ESP+0x24]
$+CA     0068DE4A     51                        PUSH ECX
$+CB     0068DE4B     8BC8                      MOV ECX,EAX
$+CD     0068DE4D     E8 0ECB1000               CALL PathOfEx.0079A960
$+D2     0068DE52     F3:0F1003                 MOVSS XMM0,DWORD PTR DS:[EBX]
$+D6     0068DE56     0F28E0                    MOVAPS XMM4,XMM0
$+D9     0068DE59     F3:0F1043 04              MOVSS XMM0,DWORD PTR DS:[EBX+0x4]
$+DE     0068DE5E     0F28C8                    MOVAPS XMM1,XMM0
$+E1     0068DE61     F3:0F1043 08              MOVSS XMM0,DWORD PTR DS:[EBX+0x8]
$+E6     0068DE66     0F14E1                    UNPCKLPS XMM4,XMM1
$+E9     0068DE69     0F28C8                    MOVAPS XMM1,XMM0
$+EC     0068DE6C     F3:0F1087 B4180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18B4]
$+F4     0068DE74     0F5705 C017F500           XORPS XMM0,DQWORD PTR DS:[0xF517C0]
$+FB     0068DE7B     D95C24 20                 FSTP DWORD PTR SS:[ESP+0x20]
$+FF     0068DE7F     F3:0F5C4424 20            SUBSS XMM0,DWORD PTR SS:[ESP+0x20]
$+105    0068DE85     0F16E1                    MOVLHPS XMM4,XMM1
$+108    0068DE88     F3:0F5E87 C0180000        DIVSS XMM0,DWORD PTR DS:[EDI+0x18C0]
$+110    0068DE90     0FC6C0 00                 SHUFPS XMM0,XMM0,0x0
$+114    0068DE94     0F59E0                    MULPS XMM4,XMM0
$+117    0068DE97     F3:0F1006                 MOVSS XMM0,DWORD PTR DS:[ESI]
$+11B    0068DE9B     0F28E8                    MOVAPS XMM5,XMM0
$+11E    0068DE9E     F3:0F1046 04              MOVSS XMM0,DWORD PTR DS:[ESI+0x4]
$+123    0068DEA3     0F28C8                    MOVAPS XMM1,XMM0
$+126    0068DEA6     F3:0F1046 08              MOVSS XMM0,DWORD PTR DS:[ESI+0x8]
$+12B    0068DEAB     0F14E9                    UNPCKLPS XMM5,XMM1
$+12E    0068DEAE     0F28D4                    MOVAPS XMM2,XMM4
$+131    0068DEB1     0F28C8                    MOVAPS XMM1,XMM0
$+134    0068DEB4     0FC6D4 55                 SHUFPS XMM2,XMM4,0x55
$+138    0068DEB8     0F16E9                    MOVLHPS XMM5,XMM1
$+13B    0068DEBB     0F57C0                    XORPS XMM0,XMM0
$+13E    0068DEBE     0F57C9                    XORPS XMM1,XMM1
$+141    0068DEC1     F3:0F10C2                 MOVSS XMM0,XMM2
$+145    0068DEC5     F3:0F10CC                 MOVSS XMM1,XMM4
$+149    0068DEC9     0F28DC                    MOVAPS XMM3,XMM4
$+14C    0068DECC     0F14C8                    UNPCKLPS XMM1,XMM0
$+14F    0068DECF     0F57C0                    XORPS XMM0,XMM0
$+152    0068DED2     0FC6DC AA                 SHUFPS XMM3,XMM4,0xAA
$+156    0068DED6     F3:0F10C3                 MOVSS XMM0,XMM3
$+15A    0068DEDA     0F16C8                    MOVLHPS XMM1,XMM0
$+15D    0068DEDD     0F58E9                    ADDPS XMM5,XMM1
$+160    0068DEE0     0F28E5                    MOVAPS XMM4,XMM5
$+163    0068DEE3     0F29AC24 A0000000         MOVAPS DQWORD PTR SS:[ESP+0xA0],XMM5
$+16B    0068DEEB     0FC6E5 55                 SHUFPS XMM4,XMM5,0x55
$+16F    0068DEEF     0F28C5                    MOVAPS XMM0,XMM5
$+172    0068DEF2     0F29A424 B0000000         MOVAPS DQWORD PTR SS:[ESP+0xB0],XMM4
$+17A    0068DEFA     0FC6C5 AA                 SHUFPS XMM0,XMM5,0xAA
$+17E    0068DEFE     F3:0F11AF A4180000        MOVSS DWORD PTR DS:[EDI+0x18A4],XMM5
$+186    0068DF06     F3:0F11A7 A8180000        MOVSS DWORD PTR DS:[EDI+0x18A8],XMM4
$+18E    0068DF0E     F3:0F101D 840AF500        MOVSS XMM3,DWORD PTR DS:[0xF50A84]
$+196    0068DF16     0F28C8                    MOVAPS XMM1,XMM0
$+199    0068DF19     F3:0F5C05 500CF500        SUBSS XMM0,DWORD PTR DS:[0xF50C50]
$+1A1    0068DF21     F3:0F58CB                 ADDSS XMM1,XMM3
$+1A5    0068DF25     33FF                      XOR EDI,EDI
$+1A7    0068DF27     33C0                      XOR EAX,EAX
$+1A9    0068DF29     894424 20                 MOV DWORD PTR SS:[ESP+0x20],EAX
$+1AD    0068DF2D     0F298424 90000000         MOVAPS DQWORD PTR SS:[ESP+0x90],XMM0
$+1B5    0068DF35     F3:0F114C24 1C            MOVSS DWORD PTR SS:[ESP+0x1C],XMM1
$+1BB    0068DF3B     F3:0F114C24 18            MOVSS DWORD PTR SS:[ESP+0x18],XMM1
$+1C1    0068DF41     8BD8                      MOV EBX,EAX
$+1C3    0068DF43     3BC7                      CMP EAX,EDI
$+1C5    0068DF45     0F8F 95030000             JG PathOfEx.0068E2E0
$+1CB    0068DF4B     0F1F4400 00               NOP DWORD PTR DS:[EAX+EAX]
$+1D0    0068DF50     8BF0                      MOV ESI,EAX
$+1D2    0068DF52     3BF0                      CMP ESI,EAX
$+1D4    0068DF54     74 10                     JE SHORT PathOfEx.0068DF66
$+1D6    0068DF56     3BF7                      CMP ESI,EDI
$+1D8    0068DF58     74 0C                     JE SHORT PathOfEx.0068DF66
$+1DA    0068DF5A     3BD8                      CMP EBX,EAX
$+1DC    0068DF5C     74 08                     JE SHORT PathOfEx.0068DF66
$+1DE    0068DF5E     3BDF                      CMP EBX,EDI
$+1E0    0068DF60     0F85 68030000             JNZ PathOfEx.0068E2CE
$+1E6    0068DF66     66:0F6EC6                 MOVD MM0,ESI
$+1EA    0068DF6A     8D4424 48                 LEA EAX,DWORD PTR SS:[ESP+0x48]
$+1EE    0068DF6E     0F5BC0                    CVTDQ2PS XMM0,XMM0
$+1F1    0068DF71     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
$+1F5    0068DF75     50                        PUSH EAX
$+1F6    0068DF76     66:0F6ED3                 MOVD MM2,EBX
$+1FA    0068DF7A     0F5BD2                    CVTDQ2PS XMM2,XMM2
$+1FD    0068DF7D     F3:0F59C3                 MULSS XMM0,XMM3
$+201    0068DF81     F3:0F59D3                 MULSS XMM2,XMM3
$+205    0068DF85     F3:0F58C0                 ADDSS XMM0,XMM0
$+209    0068DF89     F3:0F58D2                 ADDSS XMM2,XMM2
$+20D    0068DF8D     F3:0F58C5                 ADDSS XMM0,XMM5
$+211    0068DF91     F3:0F58D4                 ADDSS XMM2,XMM4
$+215    0068DF95     0F28C8                    MOVAPS XMM1,XMM0
$+218    0068DF98     F3:0F114424 30            MOVSS DWORD PTR SS:[ESP+0x30],XMM0
$+21E    0068DF9E     F3:0F5CCB                 SUBSS XMM1,XMM3
$+222    0068DFA2     0F28C2                    MOVAPS XMM0,XMM2
$+225    0068DFA5     F3:0F115424 34            MOVSS DWORD PTR SS:[ESP+0x34],XMM2
$+22B    0068DFAB     F3:0F5CC3                 SUBSS XMM0,XMM3
$+22F    0068DFAF     F3:0F114C24 28            MOVSS DWORD PTR SS:[ESP+0x28],XMM1
$+235    0068DFB5     F3:0F114C24 64            MOVSS DWORD PTR SS:[ESP+0x64],XMM1
$+23B    0068DFBB     F3:0F114424 10            MOVSS DWORD PTR SS:[ESP+0x10],XMM0
$+241    0068DFC1     F3:0F114424 68            MOVSS DWORD PTR SS:[ESP+0x68],XMM0
$+247    0068DFC7     F3:0F114C24 4C            MOVSS DWORD PTR SS:[ESP+0x4C],XMM1
$+24D    0068DFCD     F3:0F114424 50            MOVSS DWORD PTR SS:[ESP+0x50],XMM0
$+253    0068DFD3     E8 88C91000               CALL PathOfEx.0079A960
$+258    0068DFD8     F3:0F104C24 1C            MOVSS XMM1,DWORD PTR SS:[ESP+0x1C]
$+25E    0068DFDE     8D4424 38                 LEA EAX,DWORD PTR SS:[ESP+0x38]
$+262    0068DFE2     D95C24 38                 FSTP DWORD PTR SS:[ESP+0x38]
$+266    0068DFE6     F3:0F104424 38            MOVSS XMM0,DWORD PTR SS:[ESP+0x38]
$+26C    0068DFEC     8D4C24 18                 LEA ECX,DWORD PTR SS:[ESP+0x18]
$+270    0068DFF0     0F5705 C017F500           XORPS XMM0,DQWORD PTR DS:[0xF517C0]
$+277    0068DFF7     0F2FC8                    COMISS XMM1,XMM0
$+27A    0068DFFA     0F288C24 90000000         MOVAPS XMM1,DQWORD PTR SS:[ESP+0x90]
$+282    0068E002     F3:0F114424 38            MOVSS DWORD PTR SS:[ESP+0x38],XMM0
$+288    0068E008     0F46C1                    CMOVBE EAX,ECX
$+28B    0068E00B     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+28F    0068E00F     0F2FC1                    COMISS XMM0,XMM1
$+292    0068E012     77 06                     JA SHORT PathOfEx.0068E01A
$+294    0068E014     F3:0F10C1                 MOVSS XMM0,XMM1
$+298    0068E018     EB 04                     JMP SHORT PathOfEx.0068E01E
$+29A    0068E01A     F3:0F10C0                 MOVSS XMM0,XMM0
$+29E    0068E01E     8D4424 50                 LEA EAX,DWORD PTR SS:[ESP+0x50]
$+2A2    0068E022     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
$+2A6    0068E026     50                        PUSH EAX
$+2A7    0068E027     F3:0F104C24 30            MOVSS XMM1,DWORD PTR SS:[ESP+0x30]
$+2AD    0068E02D     F3:0F580D 840AF500        ADDSS XMM1,DWORD PTR DS:[0xF50A84]
$+2B5    0068E035     F3:0F114424 6C            MOVSS DWORD PTR SS:[ESP+0x6C],XMM0
$+2BB    0068E03B     F3:0F104424 10            MOVSS XMM0,DWORD PTR SS:[ESP+0x10]
$+2C1    0068E041     F3:0F114424 74            MOVSS DWORD PTR SS:[ESP+0x74],XMM0
$+2C7    0068E047     F3:0F114424 58            MOVSS DWORD PTR SS:[ESP+0x58],XMM0
$+2CD    0068E04D     F3:0F114C24 30            MOVSS DWORD PTR SS:[ESP+0x30],XMM1
$+2D3    0068E053     F3:0F114C24 70            MOVSS DWORD PTR SS:[ESP+0x70],XMM1
$+2D9    0068E059     F3:0F114C24 54            MOVSS DWORD PTR SS:[ESP+0x54],XMM1
$+2DF    0068E05F     E8 FCC81000               CALL PathOfEx.0079A960
$+2E4    0068E064     F3:0F104C24 1C            MOVSS XMM1,DWORD PTR SS:[ESP+0x1C]
$+2EA    0068E06A     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
$+2EE    0068E06E     D95C24 0C                 FSTP DWORD PTR SS:[ESP+0xC]
$+2F2    0068E072     F3:0F104424 0C            MOVSS XMM0,DWORD PTR SS:[ESP+0xC]
$+2F8    0068E078     8D4C24 18                 LEA ECX,DWORD PTR SS:[ESP+0x18]
$+2FC    0068E07C     0F5705 C017F500           XORPS XMM0,DQWORD PTR DS:[0xF517C0]
$+303    0068E083     0F2FC8                    COMISS XMM1,XMM0
$+306    0068E086     0F288C24 90000000         MOVAPS XMM1,DQWORD PTR SS:[ESP+0x90]
$+30E    0068E08E     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
$+314    0068E094     0F46C1                    CMOVBE EAX,ECX
$+317    0068E097     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+31B    0068E09B     0F2FC1                    COMISS XMM0,XMM1
$+31E    0068E09E     77 06                     JA SHORT PathOfEx.0068E0A6
$+320    0068E0A0     F3:0F10C1                 MOVSS XMM0,XMM1
$+324    0068E0A4     EB 04                     JMP SHORT PathOfEx.0068E0AA
$+326    0068E0A6     F3:0F10C0                 MOVSS XMM0,XMM0
$+32A    0068E0AA     8D4424 58                 LEA EAX,DWORD PTR SS:[ESP+0x58]
$+32E    0068E0AE     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
$+332    0068E0B2     50                        PUSH EAX
$+333    0068E0B3     F3:0F104C24 34            MOVSS XMM1,DWORD PTR SS:[ESP+0x34]
$+339    0068E0B9     F3:0F580D 840AF500        ADDSS XMM1,DWORD PTR DS:[0xF50A84]
$+341    0068E0C1     F3:0F114424 78            MOVSS DWORD PTR SS:[ESP+0x78],XMM0
$+347    0068E0C7     F3:0F104424 30            MOVSS XMM0,DWORD PTR SS:[ESP+0x30]
$+34D    0068E0CD     F3:0F114424 7C            MOVSS DWORD PTR SS:[ESP+0x7C],XMM0
$+353    0068E0D3     F3:0F114424 5C            MOVSS DWORD PTR SS:[ESP+0x5C],XMM0
$+359    0068E0D9     F3:0F114C24 34            MOVSS DWORD PTR SS:[ESP+0x34],XMM1
$+35F    0068E0DF     F3:0F118C24 80000000      MOVSS DWORD PTR SS:[ESP+0x80],XMM1
$+368    0068E0E8     F3:0F114C24 60            MOVSS DWORD PTR SS:[ESP+0x60],XMM1
$+36E    0068E0EE     E8 6DC81000               CALL PathOfEx.0079A960
$+373    0068E0F3     D95C24 0C                 FSTP DWORD PTR SS:[ESP+0xC]
$+377    0068E0F7     F3:0F104424 0C            MOVSS XMM0,DWORD PTR SS:[ESP+0xC]
$+37D    0068E0FD     0F5705 C017F500           XORPS XMM0,DQWORD PTR DS:[0xF517C0]
$+384    0068E104     F3:0F104C24 1C            MOVSS XMM1,DWORD PTR SS:[ESP+0x1C]
$+38A    0068E10A     8D4424 0C                 LEA EAX,DWORD PTR SS:[ESP+0xC]
$+38E    0068E10E     0F2FC8                    COMISS XMM1,XMM0
$+391    0068E111     8D4C24 18                 LEA ECX,DWORD PTR SS:[ESP+0x18]
$+395    0068E115     0F288C24 90000000         MOVAPS XMM1,DQWORD PTR SS:[ESP+0x90]
$+39D    0068E11D     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
$+3A3    0068E123     0F46C1                    CMOVBE EAX,ECX
$+3A6    0068E126     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+3AA    0068E12A     0F2FC1                    COMISS XMM0,XMM1
$+3AD    0068E12D     77 06                     JA SHORT PathOfEx.0068E135
$+3AF    0068E12F     F3:0F10C1                 MOVSS XMM0,XMM1
$+3B3    0068E133     EB 04                     JMP SHORT PathOfEx.0068E139
$+3B5    0068E135     F3:0F10C0                 MOVSS XMM0,XMM0
$+3B9    0068E139     8D4424 3C                 LEA EAX,DWORD PTR SS:[ESP+0x3C]
$+3BD    0068E13D     8B4C24 14                 MOV ECX,DWORD PTR SS:[ESP+0x14]
$+3C1    0068E141     50                        PUSH EAX
$+3C2    0068E142     F3:0F104C24 34            MOVSS XMM1,DWORD PTR SS:[ESP+0x34]
$+3C8    0068E148     F3:0F118424 84000000      MOVSS DWORD PTR SS:[ESP+0x84],XMM0
$+3D1    0068E151     F3:0F104424 28            MOVSS XMM0,DWORD PTR SS:[ESP+0x28]
$+3D7    0068E157     F3:0F118424 88000000      MOVSS DWORD PTR SS:[ESP+0x88],XMM0
$+3E0    0068E160     F3:0F118C24 8C000000      MOVSS DWORD PTR SS:[ESP+0x8C],XMM1
$+3E9    0068E169     F3:0F114424 40            MOVSS DWORD PTR SS:[ESP+0x40],XMM0
$+3EF    0068E16F     F3:0F114C24 44            MOVSS DWORD PTR SS:[ESP+0x44],XMM1
$+3F5    0068E175     E8 E6C71000               CALL PathOfEx.0079A960
$+3FA    0068E17A     F3:0F104C24 1C            MOVSS XMM1,DWORD PTR SS:[ESP+0x1C]
$+400    0068E180     8D4424 24                 LEA EAX,DWORD PTR SS:[ESP+0x24]
$+404    0068E184     D95C24 24                 FSTP DWORD PTR SS:[ESP+0x24]
$+408    0068E188     F3:0F104424 24            MOVSS XMM0,DWORD PTR SS:[ESP+0x24]
$+40E    0068E18E     8D4C24 18                 LEA ECX,DWORD PTR SS:[ESP+0x18]
$+412    0068E192     0F5705 C017F500           XORPS XMM0,DQWORD PTR DS:[0xF517C0]
$+419    0068E199     0F2FC8                    COMISS XMM1,XMM0
$+41C    0068E19C     0F288C24 90000000         MOVAPS XMM1,DQWORD PTR SS:[ESP+0x90]
$+424    0068E1A4     F3:0F114424 24            MOVSS DWORD PTR SS:[ESP+0x24],XMM0
$+42A    0068E1AA     0F46C1                    CMOVBE EAX,ECX
$+42D    0068E1AD     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+431    0068E1B1     0F2FC1                    COMISS XMM0,XMM1
$+434    0068E1B4     77 06                     JA SHORT PathOfEx.0068E1BC
$+436    0068E1B6     F3:0F10C1                 MOVSS XMM0,XMM1
$+43A    0068E1BA     EB 04                     JMP SHORT PathOfEx.0068E1C0
$+43C    0068E1BC     F3:0F10C0                 MOVSS XMM0,XMM0
$+440    0068E1C0     8B4424 10                 MOV EAX,DWORD PTR SS:[ESP+0x10]
$+444    0068E1C4     8D4C24 34                 LEA ECX,DWORD PTR SS:[ESP+0x34]
$+448    0068E1C8     05 B8180000               ADD EAX,0x18B8
$+44D    0068E1CD     8BD0                      MOV EDX,EAX
$+44F    0068E1CF     51                        PUSH ECX
$+450    0068E1D0     8D4C24 7C                 LEA ECX,DWORD PTR SS:[ESP+0x7C]
$+454    0068E1D4     51                        PUSH ECX
$+455    0068E1D5     F3:0F118424 94000000      MOVSS DWORD PTR SS:[ESP+0x94],XMM0
$+45E    0068E1DE     8D4C24 74                 LEA ECX,DWORD PTR SS:[ESP+0x74]
$+462    0068E1E2     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
$+466    0068E1E6     0F28D8                    MOVAPS XMM3,XMM0
$+469    0068E1E9     F3:0F1040 04              MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
$+46E    0068E1EE     0F28C8                    MOVAPS XMM1,XMM0
$+471    0068E1F1     F3:0F1040 08              MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
$+476    0068E1F6     0F14D9                    UNPCKLPS XMM3,XMM1
$+479    0068E1F9     0F28C8                    MOVAPS XMM1,XMM0
$+47C    0068E1FC     0F16D9                    MOVLHPS XMM3,XMM1
$+47F    0068E1FF     0F28D3                    MOVAPS XMM2,XMM3
$+482    0068E202     0F59D3                    MULPS XMM2,XMM3
$+485    0068E205     51                        PUSH ECX
$+486    0068E206     8D4C24 6C                 LEA ECX,DWORD PTR SS:[ESP+0x6C]
$+48A    0068E20A     51                        PUSH ECX
$+48B    0068E20B     8B4C24 20                 MOV ECX,DWORD PTR SS:[ESP+0x20]
$+48F    0068E20F     0F28CA                    MOVAPS XMM1,XMM2
$+492    0068E212     0FC6CA 99                 SHUFPS XMM1,XMM2,0x99
$+496    0068E216     F3:0F58D1                 ADDSS XMM2,XMM1
$+49A    0068E21A     0F28C1                    MOVAPS XMM0,XMM1
$+49D    0068E21D     0FC6C1 55                 SHUFPS XMM0,XMM1,0x55
$+4A1    0068E221     8D89 AC180000             LEA ECX,DWORD PTR DS:[ECX+0x18AC]
$+4A7    0068E227     F3:0F58D0                 ADDSS XMM2,XMM0
$+4AB    0068E22B     0F57C0                    XORPS XMM0,XMM0
$+4AE    0068E22E     0FC6D2 00                 SHUFPS XMM2,XMM2,0x0
$+4B2    0068E232     0F51CA                    SQRTPS XMM1,XMM2
$+4B5    0068E235     0FC215 F043E800 04        CMPNEQPS XMM2,DQWORD PTR DS:[0xE843F0]
$+4BD    0068E23D     0F5ED9                    DIVPS XMM3,XMM1
$+4C0    0068E240     0FC2C8 04                 CMPNEQPS XMM1,XMM0
$+4C4    0068E244     0F54D9                    ANDPS XMM3,XMM1
$+4C7    0068E247     0F54DA                    ANDPS XMM3,XMM2
$+4CA    0068E24A     0F5515 0044E800           ANDNPS XMM2,DQWORD PTR DS:[0xE84400]
$+4D1    0068E251     0F56DA                    ORPS XMM3,XMM2
$+4D4    0068E254     0F28CB                    MOVAPS XMM1,XMM3
$+4D7    0068E257     F3:0F1118                 MOVSS DWORD PTR DS:[EAX],XMM3
$+4DB    0068E25B     0F28C3                    MOVAPS XMM0,XMM3
$+4DE    0068E25E     0FC6CB 55                 SHUFPS XMM1,XMM3,0x55
$+4E2    0068E262     0FC6C3 AA                 SHUFPS XMM0,XMM3,0xAA
$+4E6    0068E266     F3:0F1148 04              MOVSS DWORD PTR DS:[EAX+0x4],XMM1
$+4EB    0068E26B     F3:0F1140 08              MOVSS DWORD PTR DS:[EAX+0x8],XMM0
$+4F0    0068E270     E8 EB70FFFF               CALL PathOfEx.00685360
$+4F5    0068E275     83C4 10                   ADD ESP,0x10
$+4F8    0068E278     84C0                      TEST AL,AL
$+4FA    0068E27A     75 7C                     JNZ SHORT PathOfEx.0068E2F8
$+4FC    0068E27C     8B4C24 10                 MOV ECX,DWORD PTR SS:[ESP+0x10]
$+500    0068E280     8D4424 34                 LEA EAX,DWORD PTR SS:[ESP+0x34]
$+504    0068E284     50                        PUSH EAX
$+505    0068E285     8D8424 88000000           LEA EAX,DWORD PTR SS:[ESP+0x88]
$+50C    0068E28C     50                        PUSH EAX
$+50D    0068E28D     8D8424 80000000           LEA EAX,DWORD PTR SS:[ESP+0x80]
$+514    0068E294     50                        PUSH EAX
$+515    0068E295     8D4424 6C                 LEA EAX,DWORD PTR SS:[ESP+0x6C]
$+519    0068E299     8D91 B8180000             LEA EDX,DWORD PTR DS:[ECX+0x18B8]
$+51F    0068E29F     50                        PUSH EAX
$+520    0068E2A0     8D89 AC180000             LEA ECX,DWORD PTR DS:[ECX+0x18AC]
$+526    0068E2A6     E8 B570FFFF               CALL PathOfEx.00685360
$+52B    0068E2AB     83C4 10                   ADD ESP,0x10
$+52E    0068E2AE     84C0                      TEST AL,AL
$+530    0068E2B0     75 46                     JNZ SHORT PathOfEx.0068E2F8
$+532    0068E2B2     F3:0F101D 840AF500        MOVSS XMM3,DWORD PTR DS:[0xF50A84]
$+53A    0068E2BA     0F28AC24 A0000000         MOVAPS XMM5,DQWORD PTR SS:[ESP+0xA0]
$+542    0068E2C2     0F28A424 B0000000         MOVAPS XMM4,DQWORD PTR SS:[ESP+0xB0]
$+54A    0068E2CA     8B4424 20                 MOV EAX,DWORD PTR SS:[ESP+0x20]
$+54E    0068E2CE     46                        INC ESI
$+54F    0068E2CF     3BF7                      CMP ESI,EDI
$+551    0068E2D1   ^ 0F8E 7BFCFFFF             JLE PathOfEx.0068DF52
$+557    0068E2D7     43                        INC EBX
$+558    0068E2D8     3BDF                      CMP EBX,EDI
$+55A    0068E2DA   ^ 0F8E 70FCFFFF             JLE PathOfEx.0068DF50
$+560    0068E2E0     48                        DEC EAX
$+561    0068E2E1     47                        INC EDI
$+562    0068E2E2     894424 20                 MOV DWORD PTR SS:[ESP+0x20],EAX
$+566    0068E2E6     83F8 FD                   CMP EAX,-0x3
$+569    0068E2E9   ^ 0F8F 52FCFFFF             JG PathOfEx.0068DF41
$+56F    0068E2EF     8B7C24 10                 MOV EDI,DWORD PTR SS:[ESP+0x10]
$+573    0068E2F3     E9 B8000000               JMP PathOfEx.0068E3B0
$+578    0068E2F8     8B7C24 10                 MOV EDI,DWORD PTR SS:[ESP+0x10]
$+57C    0068E2FC     F3:0F1087 B8180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18B8]
$+584    0068E304     0F28E8                    MOVAPS XMM5,XMM0
$+587    0068E307     F3:0F1087 BC180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18BC]
$+58F    0068E30F     0F28C8                    MOVAPS XMM1,XMM0
$+592    0068E312     F3:0F1087 C0180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18C0]
$+59A    0068E31A     0F14E9                    UNPCKLPS XMM5,XMM1
$+59D    0068E31D     0F28C8                    MOVAPS XMM1,XMM0
$+5A0    0068E320     F3:0F104424 34            MOVSS XMM0,DWORD PTR SS:[ESP+0x34]
$+5A6    0068E326     0F16E9                    MOVLHPS XMM5,XMM1
$+5A9    0068E329     0FC6C0 00                 SHUFPS XMM0,XMM0,0x0
$+5AD    0068E32D     0F59E8                    MULPS XMM5,XMM0
$+5B0    0068E330     F3:0F1087 AC180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18AC]
$+5B8    0068E338     0F28E0                    MOVAPS XMM4,XMM0
$+5BB    0068E33B     F3:0F1087 B0180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18B0]
$+5C3    0068E343     0F28C8                    MOVAPS XMM1,XMM0
$+5C6    0068E346     F3:0F1087 B4180000        MOVSS XMM0,DWORD PTR DS:[EDI+0x18B4]
$+5CE    0068E34E     0F14E1                    UNPCKLPS XMM4,XMM1
$+5D1    0068E351     0F28D5                    MOVAPS XMM2,XMM5
$+5D4    0068E354     0F28C8                    MOVAPS XMM1,XMM0
$+5D7    0068E357     0FC6D5 55                 SHUFPS XMM2,XMM5,0x55
$+5DB    0068E35B     0F16E1                    MOVLHPS XMM4,XMM1
$+5DE    0068E35E     0F57C0                    XORPS XMM0,XMM0
$+5E1    0068E361     0F57C9                    XORPS XMM1,XMM1
$+5E4    0068E364     F3:0F10C2                 MOVSS XMM0,XMM2
$+5E8    0068E368     F3:0F10CD                 MOVSS XMM1,XMM5
$+5EC    0068E36C     0F28DD                    MOVAPS XMM3,XMM5
$+5EF    0068E36F     0F14C8                    UNPCKLPS XMM1,XMM0
$+5F2    0068E372     0F57C0                    XORPS XMM0,XMM0
$+5F5    0068E375     0FC6DD AA                 SHUFPS XMM3,XMM5,0xAA
$+5F9    0068E379     F3:0F10C3                 MOVSS XMM0,XMM3
$+5FD    0068E37D     0F16C8                    MOVLHPS XMM1,XMM0
$+600    0068E380     0F58E1                    ADDPS XMM4,XMM1
$+603    0068E383     0F28CC                    MOVAPS XMM1,XMM4
$+606    0068E386     F3:0F11A7 A4180000        MOVSS DWORD PTR DS:[EDI+0x18A4],XMM4
$+60E    0068E38E     0FC6CC 55                 SHUFPS XMM1,XMM4,0x55
$+612    0068E392     F3:0F118F A8180000        MOVSS DWORD PTR DS:[EDI+0x18A8],XMM1
$+61A    0068E39A     EB 14                     JMP SHORT PathOfEx.0068E3B0
$+61C    0068E39C     C787 A4180000 00000000    MOV DWORD PTR DS:[EDI+0x18A4],0x0
$+626    0068E3A6     C787 A8180000 00000000    MOV DWORD PTR DS:[EDI+0x18A8],0x0
$+630    0068E3B0     8B8F 3C010000             MOV ECX,DWORD PTR DS:[EDI+0x13C]                  ; then,ecx=stCD_GameLogic*
$+636    0068E3B6     8B87 A4180000             MOV EAX,DWORD PTR DS:[EDI+0x18A4]
$+63C    0068E3BC     8981 AC3A0000             MOV DWORD PTR DS:[ECX+0x3AAC],EAX                 ; 写入浮点数坐标x
$+642    0068E3C2     8B87 A8180000             MOV EAX,DWORD PTR DS:[EDI+0x18A8]
$+648    0068E3C8     5F                        POP EDI
$+649    0068E3C9     5E                        POP ESI
$+64A    0068E3CA     8981 B03A0000             MOV DWORD PTR DS:[ECX+0x3AB0],EAX                 ; 写入浮点数坐标y
$+650    0068E3D0     5B                        POP EBX
$+651    0068E3D1     8BE5                      MOV ESP,EBP
$+653    0068E3D3     5D                        POP EBP
$+654    0068E3D4     C3                        RETN
$+655    0068E3D5     CC                        INT3
$+656    0068E3D6     CC                        INT3
$+657    0068E3D7     CC                        INT3
$+658    0068E3D8     CC                        INT3
$+659    0068E3D9     CC                        INT3
$+65A    0068E3DA     CC                        INT3
$+65B    0068E3DB     CC                        INT3
$+65C    0068E3DC     CC                        INT3
$+65D    0068E3DD     CC                        INT3
$+65E    0068E3DE     CC                        INT3
$+65F    0068E3DF     CC                        INT3
$+660    0068E3E0     55                        PUSH EBP
$+661    0068E3E1     8BEC                      MOV EBP,ESP
$+663    0068E3E3     83E4 F8                   AND ESP,0xFFFFFFF8
$+666    0068E3E6     51                        PUSH ECX
$+667    0068E3E7     53                        PUSH EBX
$+668    0068E3E8     56                        PUSH ESI
$+669    0068E3E9     8BF1                      MOV ESI,ECX
$+66B    0068E3EB     57                        PUSH EDI
$+66C    0068E3EC     8B7D 08                   MOV EDI,DWORD PTR SS:[EBP+0x8]
$+66F    0068E3EF     3BBE C4180000             CMP EDI,DWORD PTR DS:[ESI+0x18C4]
$+675    0068E3F5     74 52                     JE SHORT PathOfEx.0068E449
$+677    0068E3F7     807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
$+67B    0068E3FB     74 09                     JE SHORT PathOfEx.0068E406
$+67D    0068E3FD     E8 4E6EFFFF               CALL PathOfEx.00685250
$+682    0068E402     84C0                      TEST AL,AL
$+684    0068E404     75 43                     JNZ SHORT PathOfEx.0068E449
$+686    0068E406     8B9E C4180000             MOV EBX,DWORD PTR DS:[ESI+0x18C4]
$+68C    0068E40C     85FF                      TEST EDI,EDI
$+68E    0068E40E     74 1F                     JE SHORT PathOfEx.0068E42F
$+690    0068E410     8BCE                      MOV ECX,ESI
$+692    0068E412     E8 19030000               CALL PathOfEx.0068E730
$+697    0068E417     84C0                      TEST AL,AL
$+699    0068E419     74 14                     JE SHORT PathOfEx.0068E42F
$+69B    0068E41B     57                        PUSH EDI
$+69C    0068E41C     8BCE                      MOV ECX,ESI
$+69E    0068E41E     E8 CD070000               CALL PathOfEx.0068EBF0
$+6A3    0068E423     84C0                      TEST AL,AL
$+6A5    0068E425     74 12                     JE SHORT PathOfEx.0068E439
$+6A7    0068E427     89BE C4180000             MOV DWORD PTR DS:[ESI+0x18C4],EDI
$+6AD    0068E42D     EB 0A                     JMP SHORT PathOfEx.0068E439
$+6AF    0068E42F     C786 C4180000 00000000    MOV DWORD PTR DS:[ESI+0x18C4],0x0
$+6B9    0068E439     399E C4180000             CMP DWORD PTR DS:[ESI+0x18C4],EBX
$+6BF    0068E43F     74 08                     JE SHORT PathOfEx.0068E449
$+6C1    0068E441     53                        PUSH EBX
$+6C2    0068E442     8BCE                      MOV ECX,ESI
$+6C4    0068E444     E8 370F0000               CALL PathOfEx.0068F380
$+6C9    0068E449     5F                        POP EDI
$+6CA    0068E44A     5E                        POP ESI
$+6CB    0068E44B     5B                        POP EBX
$+6CC    0068E44C     8BE5                      MOV ESP,EBP
$+6CE    0068E44E     5D                        POP EBP
$+6CF    0068E44F     C2 0800                   RETN 0x8
$+6D2    0068E452     CC                        INT3
$+6D3    0068E453     CC                        INT3
$+6D4    0068E454     CC                        INT3
$+6D5    0068E455     CC                        INT3
$+6D6    0068E456     CC                        INT3
$+6D7    0068E457     CC                        INT3
$+6D8    0068E458     CC                        INT3
$+6D9    0068E459     CC                        INT3
$+6DA    0068E45A     CC                        INT3
$+6DB    0068E45B     CC                        INT3
$+6DC    0068E45C     CC                        INT3
$+6DD    0068E45D     CC                        INT3
$+6DE    0068E45E     CC                        INT3
$+6DF    0068E45F     CC                        INT3
$+6E0    0068E460     83EC 08                   SUB ESP,0x8
$+6E3    0068E463     56                        PUSH ESI
$+6E4    0068E464     57                        PUSH EDI
$+6E5    0068E465     8B7C24 14                 MOV EDI,DWORD PTR SS:[ESP+0x14]
$+6E9    0068E469     8B0F                      MOV ECX,DWORD PTR DS:[EDI]
$+6EB    0068E46B     85C9                      TEST ECX,ECX
$+6ED    0068E46D     74 05                     JE SHORT PathOfEx.0068E474
$+6EF    0068E46F     E8 9C800400               CALL PathOfEx.006D6510
$+6F4    0068E474     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+6F6    0068E476     C74424 14 6451E800        MOV DWORD PTR SS:[ESP+0x14],PathOfEx.00E85164
$+6FE    0068E47E     8B70 30                   MOV ESI,DWORD PTR DS:[EAX+0x30]
$+701    0068E481     8D4424 14                 LEA EAX,DWORD PTR SS:[ESP+0x14]
$+705    0068E485     50                        PUSH EAX
$+706    0068E486     8D4424 10                 LEA EAX,DWORD PTR SS:[ESP+0x10]
$+70A    0068E48A     50                        PUSH EAX
$+70B    0068E48B     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
$+70E    0068E48E     E8 2DA4DAFF               CALL PathOfEx.004388C0
$+713    0068E493     8B4424 0C                 MOV EAX,DWORD PTR SS:[ESP+0xC]
$+717    0068E497     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
$+71A    0068E49A     74 25                     JE SHORT PathOfEx.0068E4C1
$+71C    0068E49C     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
$+71F    0068E49F     83F9 FF                   CMP ECX,-0x1
$+722    0068E4A2     74 1D                     JE SHORT PathOfEx.0068E4C1
$+724    0068E4A4     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+727    0068E4A7     8B0C88                    MOV ECX,DWORD PTR DS:[EAX+ECX*4]
$+72A    0068E4AA     85C9                      TEST ECX,ECX
$+72C    0068E4AC     74 13                     JE SHORT PathOfEx.0068E4C1
$+72E    0068E4AE     E8 ED5A1000               CALL PathOfEx.00793FA0
$+733    0068E4B3     84C0                      TEST AL,AL
$+735    0068E4B5     74 0A                     JE SHORT PathOfEx.0068E4C1
$+737    0068E4B7     B0 01                     MOV AL,0x1
$+739    0068E4B9     5F                        POP EDI
$+73A    0068E4BA     5E                        POP ESI
$+73B    0068E4BB     83C4 08                   ADD ESP,0x8
$+73E    0068E4BE     C2 0400                   RETN 0x4
$+741    0068E4C1     5F                        POP EDI
$+742    0068E4C2     32C0                      XOR AL,AL
$+744    0068E4C4     5E                        POP ESI
$+745    0068E4C5     83C4 08                   ADD ESP,0x8
$+748    0068E4C8     C2 0400                   RETN 0x4
$+74B    0068E4CB     CC                        INT3
*/
//tag_运行一次技能
/*
$-151    01A565AF     CC                    INT3
$-150    01A565B0     55                    PUSH EBP
$-14F    01A565B1     8BEC                  MOV EBP,ESP
$-14D    01A565B3     6A FF                 PUSH -0x1
$-14B    01A565B5     68 F0F8D201           PUSH PathOfEx.01D2F8F0
$-146    01A565BA     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
$-140    01A565C0     50                    PUSH EAX
$-13F    01A565C1     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
$-138    01A565C8     83EC 14               SUB ESP,0x14
$-135    01A565CB     53                    PUSH EBX
$-134    01A565CC     8BD9                  MOV EBX,ECX
$-132    01A565CE     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
$-12B    01A565D5     56                    PUSH ESI
$-12A    01A565D6     57                    PUSH EDI
$-129    01A565D7     B0 01                 MOV AL,0x1
$-127    01A565D9     8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
$-124    01A565DC     8B0B                  MOV ECX,DWORD PTR DS:[EBX]
$-122    01A565DE     8BF9                  MOV EDI,ECX
$-120    01A565E0     895D E0               MOV DWORD PTR SS:[EBP-0x20],EBX
$-11D    01A565E3     894D E4               MOV DWORD PTR SS:[EBP-0x1C],ECX
$-11A    01A565E6     8845 E8               MOV BYTE PTR SS:[EBP-0x18],AL
$-117    01A565E9     8B71 04               MOV ESI,DWORD PTR DS:[ECX+0x4]
$-114    01A565EC     807E 0D 00            CMP BYTE PTR DS:[ESI+0xD],0x0
$-110    01A565F0     75 75                 JNZ SHORT PathOfEx.01A56667
$-10E    01A565F2     8B45 10               MOV EAX,DWORD PTR SS:[EBP+0x10]
$-10B    01A565F5     8B58 10               MOV EBX,DWORD PTR DS:[EAX+0x10]
$-108    01A565F8     EB 09                 JMP SHORT PathOfEx.01A56603
$-106    01A565FA     66:0F1F4400 00        NOP WORD PTR DS:[EAX+EAX]
$-100    01A56600     8B45 10               MOV EAX,DWORD PTR SS:[EBP+0x10]
$-FD     01A56603     837E 24 08            CMP DWORD PTR DS:[ESI+0x24],0x8
$-F9     01A56607     8D56 10               LEA EDX,DWORD PTR DS:[ESI+0x10]
$-F6     01A5660A     8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
$-F3     01A5660D     8D7A 10               LEA EDI,DWORD PTR DS:[EDX+0x10]
$-F0     01A56610     72 02                 JB SHORT PathOfEx.01A56614
$-EE     01A56612     8B12                  MOV EDX,DWORD PTR DS:[EDX]
$-EC     01A56614     8378 14 08            CMP DWORD PTR DS:[EAX+0x14],0x8
$-E8     01A56618     72 04                 JB SHORT PathOfEx.01A5661E
$-E6     01A5661A     8B08                  MOV ECX,DWORD PTR DS:[EAX]
$-E4     01A5661C     EB 02                 JMP SHORT PathOfEx.01A56620
$-E2     01A5661E     8BC8                  MOV ECX,EAX
$-E0     01A56620     8B3F                  MOV EDI,DWORD PTR DS:[EDI]
$-DE     01A56622     3BDF                  CMP EBX,EDI
$-DC     01A56624     8BC7                  MOV EAX,EDI
$-DA     01A56626     0F42C3                CMOVB EAX,EBX
$-D7     01A56629     85C0                  TEST EAX,EAX
$-D5     01A5662B     74 0D                 JE SHORT PathOfEx.01A5663A
$-D3     01A5662D     50                    PUSH EAX
$-D2     01A5662E     E8 3D5E99FF           CALL PathOfEx.013EC470
$-CD     01A56633     83C4 04               ADD ESP,0x4
$-CA     01A56636     85C0                  TEST EAX,EAX
$-C8     01A56638     75 0D                 JNZ SHORT PathOfEx.01A56647
$-C6     01A5663A     3BFB                  CMP EDI,EBX
$-C4     01A5663C     76 05                 JBE SHORT PathOfEx.01A56643
$-C2     01A5663E     83C8 FF               OR EAX,0xFFFFFFFF
$-BF     01A56641     EB 04                 JMP SHORT PathOfEx.01A56647
$-BD     01A56643     1BC0                  SBB EAX,EAX
$-BB     01A56645     F7D8                  NEG EAX
$-B9     01A56647     C1E8 1F               SHR EAX,0x1F
$-B6     01A5664A     8845 E8               MOV BYTE PTR SS:[EBP-0x18],AL
$-B3     01A5664D     84C0                  TEST AL,AL
$-B1     01A5664F     74 04                 JE SHORT PathOfEx.01A56655
$-AF     01A56651     8B36                  MOV ESI,DWORD PTR DS:[ESI]
$-AD     01A56653     EB 03                 JMP SHORT PathOfEx.01A56658
$-AB     01A56655     8B76 08               MOV ESI,DWORD PTR DS:[ESI+0x8]
$-A8     01A56658     807E 0D 00            CMP BYTE PTR DS:[ESI+0xD],0x0
$-A4     01A5665C   ^ 74 A2                 JE SHORT PathOfEx.01A56600
$-A2     01A5665E     8B7D EC               MOV EDI,DWORD PTR SS:[EBP-0x14]
$-9F     01A56661     8B4D E4               MOV ECX,DWORD PTR SS:[EBP-0x1C]
$-9C     01A56664     8B5D E0               MOV EBX,DWORD PTR SS:[EBP-0x20]
$-99     01A56667     8BF7                  MOV ESI,EDI
$-97     01A56669     8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
$-94     01A5666C     84C0                  TEST AL,AL
$-92     01A5666E     74 3F                 JE SHORT PathOfEx.01A566AF
$-90     01A56670     3B39                  CMP EDI,DWORD PTR DS:[ECX]
$-8E     01A56672     75 30                 JNZ SHORT PathOfEx.01A566A4
$-8C     01A56674     FF75 14               PUSH DWORD PTR SS:[EBP+0x14]
$-89     01A56677     51                    PUSH ECX
$-88     01A56678     57                    PUSH EDI
$-87     01A56679     6A 01                 PUSH 0x1
$-85     01A5667B     8D45 10               LEA EAX,DWORD PTR SS:[EBP+0x10]
$-82     01A5667E     8BCB                  MOV ECX,EBX
$-80     01A56680     50                    PUSH EAX
$-7F     01A56681     E8 1AFDFFFF           CALL PathOfEx.01A563A0
$-7A     01A56686     8B08                  MOV ECX,DWORD PTR DS:[EAX]
$-78     01A56688     8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
$-75     01A5668B     8908                  MOV DWORD PTR DS:[EAX],ECX
$-73     01A5668D     C640 04 01            MOV BYTE PTR DS:[EAX+0x4],0x1
$-6F     01A56691     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
$-6C     01A56694     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
$-65     01A5669B     5F                    POP EDI
$-64     01A5669C     5E                    POP ESI
$-63     01A5669D     5B                    POP EBX
$-62     01A5669E     8BE5                  MOV ESP,EBP
$-60     01A566A0     5D                    POP EBP
$-5F     01A566A1     C2 1000               RETN 0x10
$-5C     01A566A4     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
$-59     01A566A7     E8 84B099FF           CALL PathOfEx.013F1730
$-54     01A566AC     8B75 EC               MOV ESI,DWORD PTR SS:[EBP-0x14]
$-51     01A566AF     FF75 10               PUSH DWORD PTR SS:[EBP+0x10]
$-4E     01A566B2     8D46 10               LEA EAX,DWORD PTR DS:[ESI+0x10]
$-4B     01A566B5     50                    PUSH EAX
$-4A     01A566B6     E8 B56899FF           CALL PathOfEx.013ECF70
$-45     01A566BB     FF75 14               PUSH DWORD PTR SS:[EBP+0x14]
$-42     01A566BE     84C0                  TEST AL,AL
$-40     01A566C0     74 18                 JE SHORT PathOfEx.01A566DA
$-3E     01A566C2     51                    PUSH ECX
$-3D     01A566C3     57                    PUSH EDI
$-3C     01A566C4     FF75 E8               PUSH DWORD PTR SS:[EBP-0x18]
$-39     01A566C7   ^ EB B2                 JMP SHORT PathOfEx.01A5667B
$-37     01A566C9     FF75 14               PUSH DWORD PTR SS:[EBP+0x14]
$-34     01A566CC     E8 7FFCFFFF           CALL PathOfEx.01A56350
$-2F     01A566D1     6A 00                 PUSH 0x0
$-2D     01A566D3     6A 00                 PUSH 0x0
$-2B     01A566D5     E8 79541F00           CALL PathOfEx.01C4BB53
$-26     01A566DA     E8 71FCFFFF           CALL PathOfEx.01A56350
$-21     01A566DF     8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
$-1E     01A566E2     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1B     01A566E5     5F                    POP EDI
$-1A     01A566E6     8930                  MOV DWORD PTR DS:[EAX],ESI
$-18     01A566E8     5E                    POP ESI
$-17     01A566E9     C640 04 00            MOV BYTE PTR DS:[EAX+0x4],0x0
$-13     01A566ED     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
$-C      01A566F4     5B                    POP EBX
$-B      01A566F5     8BE5                  MOV ESP,EBP
$-9      01A566F7     5D                    POP EBP
$-8      01A566F8     C2 1000               RETN 0x10
$-5      01A566FB     CC                    INT3
$-4      01A566FC     CC                    INT3
$-3      01A566FD     CC                    INT3
$-2      01A566FE     CC                    INT3
$-1      01A566FF     CC                    INT3
$ ==>    01A56700     51                    PUSH ECX                                        ; tag_运行一次技能
$+1      01A56701     56                    PUSH ESI
$+2      01A56702     0FB771 0C             MOVZX ESI,WORD PTR DS:[ECX+0xC]                 ; then,esi=skill_id
$+6      01A56706     66:85F6               TEST SI,SI
$+9      01A56709     74 43                 JE SHORT PathOfEx.01A5674E
$+B      01A5670B     8B5424 0C             MOV EDX,DWORD PTR SS:[ESP+0xC]
$+F      01A5670F     F742 70 02020000      TEST DWORD PTR DS:[EDX+0x70],0x202
$+16     01A56716     75 36                 JNZ SHORT PathOfEx.01A5674E
$+18     01A56718     0F57C0                XORPS XMM0,XMM0
$+1B     01A5671B     0F2F82 A8000000       COMISS XMM0,DWORD PTR DS:[EDX+0xA8]
$+22     01A56722     72 2A                 JB SHORT PathOfEx.01A5674E
$+24     01A56724     8339 00               CMP DWORD PTR DS:[ECX],0x0
$+27     01A56727     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
$+2B     01A5672B     50                    PUSH EAX
$+2C     01A5672C     0FB641 0E             MOVZX EAX,BYTE PTR DS:[ECX+0xE]
$+30     01A56730     50                    PUSH EAX
$+31     01A56731     74 0F                 JE SHORT PathOfEx.01A56742
$+33     01A56733     FF31                  PUSH DWORD PTR DS:[ECX]
$+35     01A56735     8BCA                  MOV ECX,EDX
$+37     01A56737     56                    PUSH ESI
$+38     01A56738     E8 3302D0FF           CALL PathOfEx.01756970                          ; 运行一次技能
$+3D     01A5673D     5E                    POP ESI
$+3E     01A5673E     59                    POP ECX
$+3F     01A5673F     C2 0400               RETN 0x4
$+42     01A56742     8D41 04               LEA EAX,DWORD PTR DS:[ECX+0x4]
$+45     01A56745     8BCA                  MOV ECX,EDX
$+47     01A56747     50                    PUSH EAX
$+48     01A56748     56                    PUSH ESI
$+49     01A56749     E8 7202D0FF           CALL PathOfEx.017569C0
$+4E     01A5674E     5E                    POP ESI
$+4F     01A5674F     59                    POP ECX
$+50     01A56750     C2 0400               RETN 0x4
$+53     01A56753     CC                    INT3
$+54     01A56754     CC                    INT3
$+55     01A56755     CC                    INT3
$+56     01A56756     CC                    INT3
$+57     01A56757     CC                    INT3
$+58     01A56758     CC                    INT3
$+59     01A56759     CC                    INT3
$+5A     01A5675A     CC                    INT3
$+5B     01A5675B     CC                    INT3
$+5C     01A5675C     CC                    INT3
$+5D     01A5675D     CC                    INT3
$+5E     01A5675E     CC                    INT3
$+5F     01A5675F     CC                    INT3
$+60     01A56760     51                    PUSH ECX
$+61     01A56761     53                    PUSH EBX
$+62     01A56762     56                    PUSH ESI
$+63     01A56763     57                    PUSH EDI
$+64     01A56764     8BF9                  MOV EDI,ECX
$+66     01A56766     0FB747 04             MOVZX EAX,WORD PTR DS:[EDI+0x4]
$+6A     01A5676A     50                    PUSH EAX
$+6B     01A5676B     FF15 B875D401         CALL DWORD PTR DS:[<&WS2_32.#9>]                ; ws2_32.ntohs
$+71     01A56771     8B5C24 14             MOV EBX,DWORD PTR SS:[ESP+0x14]
$+75     01A56775     8BCB                  MOV ECX,EBX
$+77     01A56777     0FB7C0                MOVZX EAX,AX
$+7A     01A5677A     894424 0C             MOV DWORD PTR SS:[ESP+0xC],EAX
$+7E     01A5677E     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
$+82     01A56782     6A 02                 PUSH 0x2
$+84     01A56784     50                    PUSH EAX
$+85     01A56785     E8 36C2CAFF           CALL PathOfEx.017029C0
$+8A     01A5678A     8B73 58               MOV ESI,DWORD PTR DS:[EBX+0x58]
$+8D     01A5678D     8BCE                  MOV ECX,ESI
$+8F     01A5678F     FF77 08               PUSH DWORD PTR DS:[EDI+0x8]
$+92     01A56792     E8 E9010000           CALL PathOfEx.01A56980
$+97     01A56797     50                    PUSH EAX
$+98     01A56798     8BCE                  MOV ECX,ESI
$+9A     01A5679A     E8 71020000           CALL PathOfEx.01A56A10
$+9F     01A5679F     50                    PUSH EAX
$+A0     01A567A0     FF15 5475D401         CALL DWORD PTR DS:[<&WS2_32.#8>]                ; ws2_32.ntohl
$+A6     01A567A6     894424 14             MOV DWORD PTR SS:[ESP+0x14],EAX
$+AA     01A567AA     8BCB                  MOV ECX,EBX
$+AC     01A567AC     6A 04                 PUSH 0x4
$+AE     01A567AE     8D4424 18             LEA EAX,DWORD PTR SS:[ESP+0x18]
$+B2     01A567B2     50                    PUSH EAX
$+B3     01A567B3     E8 08C2CAFF           CALL PathOfEx.017029C0
$+B8     01A567B8     5F                    POP EDI
$+B9     01A567B9     5E                    POP ESI
$+BA     01A567BA     5B                    POP EBX
$+BB     01A567BB     59                    POP ECX
$+BC     01A567BC     C2 0400               RETN 0x4
$+BF     01A567BF     CC                    INT3
$+C0     01A567C0     51                    PUSH ECX
$+C1     01A567C1     53                    PUSH EBX
$+C2     01A567C2     56                    PUSH ESI
$+C3     01A567C3     57                    PUSH EDI
$+C4     01A567C4     8BF9                  MOV EDI,ECX
$+C6     01A567C6     0FB747 04             MOVZX EAX,WORD PTR DS:[EDI+0x4]
$+CA     01A567CA     50                    PUSH EAX
$+CB     01A567CB     FF15 B875D401         CALL DWORD PTR DS:[<&WS2_32.#9>]                ; ws2_32.ntohs
$+D1     01A567D1     8B5C24 14             MOV EBX,DWORD PTR SS:[ESP+0x14]
$+D5     01A567D5     8BCB                  MOV ECX,EBX
$+D7     01A567D7     0FB7C0                MOVZX EAX,AX
$+DA     01A567DA     894424 0C             MOV DWORD PTR SS:[ESP+0xC],EAX
$+DE     01A567DE     8D4424 0C             LEA EAX,DWORD PTR SS:[ESP+0xC]
$+E2     01A567E2     6A 02                 PUSH 0x2
$+E4     01A567E4     50                    PUSH EAX
$+E5     01A567E5     E8 D6C1CAFF           CALL PathOfEx.017029C0
$+EA     01A567EA     8B73 58               MOV ESI,DWORD PTR DS:[EBX+0x58]
$+ED     01A567ED     8BCE                  MOV ECX,ESI
$+EF     01A567EF     FF77 08               PUSH DWORD PTR DS:[EDI+0x8]
$+F2     01A567F2     E8 A9020000           CALL PathOfEx.01A56AA0
$+F7     01A567F7     50                    PUSH EAX
$+F8     01A567F8     8BCE                  MOV ECX,ESI
$+FA     01A567FA     E8 31030000           CALL PathOfEx.01A56B30
$+FF     01A567FF     50                    PUSH EAX
$+100    01A56800     FF15 5475D401         CALL DWORD PTR DS:[<&WS2_32.#8>]                ; ws2_32.ntohl
$+106    01A56806     894424 14             MOV DWORD PTR SS:[ESP+0x14],EAX
$+10A    01A5680A     8BCB                  MOV ECX,EBX
$+10C    01A5680C     6A 04                 PUSH 0x4
$+10E    01A5680E     8D4424 18             LEA EAX,DWORD PTR SS:[ESP+0x18]
$+112    01A56812     50                    PUSH EAX
$+113    01A56813     E8 A8C1CAFF           CALL PathOfEx.017029C0
$+118    01A56818     5F                    POP EDI
$+119    01A56819     5E                    POP ESI
$+11A    01A5681A     5B                    POP EBX
$+11B    01A5681B     59                    POP ECX
$+11C    01A5681C     C2 0400               RETN 0x4
$+11F    01A5681F     CC                    INT3
$+120    01A56820     56                    PUSH ESI
$+121    01A56821     57                    PUSH EDI
$+122    01A56822     8B7C24 0C             MOV EDI,DWORD PTR SS:[ESP+0xC]
$+126    01A56826     8D71 08               LEA ESI,DWORD PTR DS:[ECX+0x8]
$+129    01A56829     6A 04                 PUSH 0x4
$+12B    01A5682B     56                    PUSH ESI
$+12C    01A5682C     8BCF                  MOV ECX,EDI
$+12E    01A5682E     E8 7DC0CAFF           CALL PathOfEx.017028B0
$+133    01A56833     FF36                  PUSH DWORD PTR DS:[ESI]
$+135    01A56835     FF15 5875D401         CALL DWORD PTR DS:[<&WS2_32.#14>]               ; ws2_32.ntohl
$+13B    01A5683B     8906                  MOV DWORD PTR DS:[ESI],EAX
$+13D    01A5683D     80BF B2000000 00      CMP BYTE PTR DS:[EDI+0xB2],0x0
$+144    01A56844     75 12                 JNZ SHORT PathOfEx.01A56858
$+146    01A56846     8B4F 58               MOV ECX,DWORD PTR DS:[EDI+0x58]
$+149    01A56849     56                    PUSH ESI
$+14A    01A5684A     E8 71030000           CALL PathOfEx.01A56BC0
$+14F    01A5684F     8B4F 58               MOV ECX,DWORD PTR DS:[EDI+0x58]
$+152    01A56852     56                    PUSH ESI
$+153    01A56853     E8 08040000           CALL PathOfEx.01A56C60
$+158    01A56858     5F                    POP EDI
$+159    01A56859     5E                    POP ESI
$+15A    01A5685A     C2 0400               RETN 0x4
$+15D    01A5685D     CC                    INT3
*/