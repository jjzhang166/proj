#pragma once
/*
@author zhp
@date 2017/2/23 2:49
@purpose for task asm record
*/

//tag_打开NPC等解除功能函数
/*
$ ==>    006CBFE0     55                          PUSH EBP                                        ; 解除功能call
$+1      006CBFE1     8BEC                        MOV EBP, ESP
$+3      006CBFE3     6A FF                       PUSH -0x1
$+5      006CBFE5     68 E721D400                 PUSH PathOfEx.00D421E7
$+A      006CBFEA     64:A1 00000000              MOV EAX, DWORD PTR FS:[0]
$+10     006CBFF0     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+11     006CBFF1     64:8925 00000000            MOV DWORD PTR FS:[0], ESP
$+18     006CBFF8     81EC 8C000000               SUB ESP, 0x8C
$+1E     006CBFFE     53                          PUSH EBX
$+1F     006CBFFF     56                          PUSH ESI
$+20     006CC000     57                          PUSH EDI
$+21     006CC001     8BF9                        MOV EDI, ECX
$+23     006CC003     33DB                        XOR EBX, EBX
$+25     006CC005     895D D8                     MOV DWORD PTR SS:[EBP-0x28], EBX
$+28     006CC008     399F 3C010000               CMP DWORD PTR DS:[EDI+0x13C], EBX
$+2E     006CC00E     74 28                       JE SHORT PathOfEx.006CC038
$+30     006CC010     399F D8180000               CMP DWORD PTR DS:[EDI+0x18D8], EBX
$+36     006CC016     74 20                       JE SHORT PathOfEx.006CC038
$+38     006CC018     8D4D E8                     LEA ECX, DWORD PTR SS:[EBP-0x18]
$+3B     006CC01B     E8 C016DBFF                 CALL PathOfEx.0047D6E0
$+40     006CC020     8D4D DC                     LEA ECX, DWORD PTR SS:[EBP-0x24]
$+43     006CC023     51                          PUSH ECX
$+44     006CC024     8B08                        MOV ECX, DWORD PTR DS:[EAX]
$+46     006CC026     E8 E5A3FDFF                 CALL PathOfEx.006A6410
$+4B     006CC02B     BB 03000000                 MOV EBX, 0x3
$+50     006CC030     C645 F3 00                  MOV BYTE PTR SS:[EBP-0xD], 0x0
$+54     006CC034     3938                        CMP DWORD PTR DS:[EAX], EDI
$+56     006CC036     74 04                       JE SHORT PathOfEx.006CC03C
$+58     006CC038     C645 F3 01                  MOV BYTE PTR SS:[EBP-0xD], 0x1
$+5C     006CC03C     83C8 FF                     OR EAX, -0x1
$+5F     006CC03F     F6C3 02                     TEST BL, 0x2
$+62     006CC042     74 3B                       JE SHORT PathOfEx.006CC07F
$+64     006CC044     83E3 FD                     AND EBX, -0x3
$+67     006CC047     C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4], 0x0
$+6E     006CC04E     8B75 E0                     MOV ESI, DWORD PTR SS:[EBP-0x20]
$+71     006CC051     85F6                        TEST ESI, ESI
$+73     006CC053     74 23                       JE SHORT PathOfEx.006CC078
$+75     006CC055     F0:0FC146 04                LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
$+7A     006CC05A     75 19                       JNZ SHORT PathOfEx.006CC075
$+7C     006CC05C     8B06                        MOV EAX, DWORD PTR DS:[ESI]                     ; PathOfEx.00E73FFC
$+7E     006CC05E     8BCE                        MOV ECX, ESI
$+80     006CC060     FF10                        CALL NEAR DWORD PTR DS:[EAX]
$+82     006CC062     83C9 FF                     OR ECX, -0x1
$+85     006CC065     8BC1                        MOV EAX, ECX
$+87     006CC067     F0:0FC146 08                LOCK XADD DWORD PTR DS:[ESI+0x8], EAX
$+8C     006CC06C     75 07                       JNZ SHORT PathOfEx.006CC075
$+8E     006CC06E     8B06                        MOV EAX, DWORD PTR DS:[ESI]                     ; PathOfEx.00E73FFC
$+90     006CC070     8BCE                        MOV ECX, ESI
$+92     006CC072     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
$+95     006CC075     83C8 FF                     OR EAX, -0x1
$+98     006CC078     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4], -0x1
$+9F     006CC07F     F6C3 01                     TEST BL, 0x1
$+A2     006CC082     74 37                       JE SHORT PathOfEx.006CC0BB
$+A4     006CC084     83E3 FE                     AND EBX, -0x2
$+A7     006CC087     C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4], 0x1
$+AE     006CC08E     8B75 EC                     MOV ESI, DWORD PTR SS:[EBP-0x14]
$+B1     006CC091     85F6                        TEST ESI, ESI
$+B3     006CC093     74 1F                       JE SHORT PathOfEx.006CC0B4
$+B5     006CC095     F0:0FC146 04                LOCK XADD DWORD PTR DS:[ESI+0x4], EAX
$+BA     006CC09A     75 18                       JNZ SHORT PathOfEx.006CC0B4
$+BC     006CC09C     8B06                        MOV EAX, DWORD PTR DS:[ESI]                     ; PathOfEx.00E73FFC
$+BE     006CC09E     8BCE                        MOV ECX, ESI
$+C0     006CC0A0     FF10                        CALL NEAR DWORD PTR DS:[EAX]
$+C2     006CC0A2     83C9 FF                     OR ECX, -0x1
$+C5     006CC0A5     F0:0FC14E 08                LOCK XADD DWORD PTR DS:[ESI+0x8], ECX
$+CA     006CC0AA     49                          DEC ECX
$+CB     006CC0AB     75 07                       JNZ SHORT PathOfEx.006CC0B4
$+CD     006CC0AD     8B06                        MOV EAX, DWORD PTR DS:[ESI]                     ; PathOfEx.00E73FFC
$+CF     006CC0AF     8BCE                        MOV ECX, ESI
$+D1     006CC0B1     FF50 04                     CALL NEAR DWORD PTR DS:[EAX+0x4]
$+D4     006CC0B4     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4], -0x1
$+DB     006CC0BB     807D F3 00                  CMP BYTE PTR SS:[EBP-0xD], 0x0
$+DF     006CC0BF     0F85 0F0B0000               JNZ PathOfEx.006CCBD4
$+E5     006CC0C5     8B75 08                     MOV ESI, DWORD PTR SS:[EBP+0x8]
$+E8     006CC0C8     83FE 22                     CMP ESI, 0x22
$+EB     006CC0CB     74 4D                       JE SHORT PathOfEx.006CC11A
$+ED     006CC0CD     85F6                        TEST ESI, ESI
$+EF     006CC0CF     74 49                       JE SHORT PathOfEx.006CC11A
$+F1     006CC0D1     83FE 01                     CMP ESI, 0x1
$+F4     006CC0D4     74 44                       JE SHORT PathOfEx.006CC11A
$+F6     006CC0D6     83FE 02                     CMP ESI, 0x2
$+F9     006CC0D9     74 3F                       JE SHORT PathOfEx.006CC11A
$+FB     006CC0DB     83FE 03                     CMP ESI, 0x3
$+FE     006CC0DE     74 3A                       JE SHORT PathOfEx.006CC11A
$+100    006CC0E0     83FE 04                     CMP ESI, 0x4
$+103    006CC0E3     74 35                       JE SHORT PathOfEx.006CC11A
$+105    006CC0E5     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+10B    006CC0EB     8B81 440A0000               MOV EAX, DWORD PTR DS:[ECX+0xA44]
$+111    006CC0F1     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+113    006CC0F3     74 0E                       JE SHORT PathOfEx.006CC103
$+115    006CC0F5     8A80 54070000               MOV AL, BYTE PTR DS:[EAX+0x754]
$+11B    006CC0FB     84C0                        TEST AL, AL
$+11D    006CC0FD     0F85 D10A0000               JNZ PathOfEx.006CCBD4
$+123    006CC103     8B89 240A0000               MOV ECX, DWORD PTR DS:[ECX+0xA24]
$+129    006CC109     85C9                        TEST ECX, ECX
$+12B    006CC10B     74 0D                       JE SHORT PathOfEx.006CC11A
$+12D    006CC10D     E8 BE3DF2FF                 CALL PathOfEx.005EFED0
$+132    006CC112     84C0                        TEST AL, AL
$+134    006CC114     0F85 BA0A0000               JNZ PathOfEx.006CCBD4
$+13A    006CC11A     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]
$+140    006CC120     8B88 240A0000               MOV ECX, DWORD PTR DS:[EAX+0xA24]
$+146    006CC126     85C9                        TEST ECX, ECX
$+148    006CC128     74 1E                       JE SHORT PathOfEx.006CC148
$+14A    006CC12A     8A81 54070000               MOV AL, BYTE PTR DS:[ECX+0x754]
$+150    006CC130     84C0                        TEST AL, AL
$+152    006CC132     74 14                       JE SHORT PathOfEx.006CC148
$+154    006CC134     8B81 A8090000               MOV EAX, DWORD PTR DS:[ECX+0x9A8]               ; PathOfEx.00610069
$+15A    006CC13A     8A80 56070000               MOV AL, BYTE PTR DS:[EAX+0x756]
$+160    006CC140     84C0                        TEST AL, AL
$+162    006CC142     74 04                       JE SHORT PathOfEx.006CC148
$+164    006CC144     B1 01                       MOV CL, 0x1
$+166    006CC146     EB 02                       JMP SHORT PathOfEx.006CC14A
$+168    006CC148     32C9                        XOR CL, CL
$+16A    006CC14A     8B55 14                     MOV EDX, DWORD PTR SS:[EBP+0x14]
$+16D    006CC14D     C602 01                     MOV BYTE PTR DS:[EDX], 0x1
$+170    006CC150     83FE 25                     CMP ESI, 0x25
$+173    006CC153     0F87 780A0000               JA PathOfEx.006CCBD1
$+179    006CC159     0FB686 44CC6C00             MOVZX EAX, BYTE PTR DS:[ESI+0x6CCC44]
$+180    006CC160     FF2485 E8CB6C00             JMP NEAR DWORD PTR DS:[EAX*4+0x6CCBE8]
$+187    006CC167     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+18B    006CC16B     0F84 630A0000               JE PathOfEx.006CCBD4
$+191    006CC171     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+195    006CC175     0F85 590A0000               JNZ PathOfEx.006CCBD4
$+19B    006CC17B     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+1A1    006CC181     56                          PUSH ESI
$+1A2    006CC182     8B89 F4080000               MOV ECX, DWORD PTR DS:[ECX+0x8F4]
$+1A8    006CC188     E8 93BBECFF                 CALL PathOfEx.00597D20
$+1AD    006CC18D     5F                          POP EDI                                         ; 0018E210
$+1AE    006CC18E     5E                          POP ESI                                         ; 0018E210
$+1AF    006CC18F     5B                          POP EBX                                         ; 0018E210
$+1B0    006CC190     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+1B3    006CC193     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+1BA    006CC19A     8BE5                        MOV ESP, EBP
$+1BC    006CC19C     5D                          POP EBP                                         ; 0018E210
$+1BD    006CC19D     C2 1000                     RETN 0x10
$+1C0    006CC1A0     8B87 64150000               MOV EAX, DWORD PTR DS:[EDI+0x1564]
$+1C6    006CC1A6     3B87 68150000               CMP EAX, DWORD PTR DS:[EDI+0x1568]
$+1CC    006CC1AC     75 16                       JNZ SHORT PathOfEx.006CC1C4
$+1CE    006CC1AE     C745 E8 00000000            MOV DWORD PTR SS:[EBP-0x18], 0x0
$+1D5    006CC1B5     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
$+1D8    006CC1B8     C745 EC 00000000            MOV DWORD PTR SS:[EBP-0x14], 0x0
$+1DF    006CC1BF     83CB 04                     OR EBX, 0x4
$+1E2    006CC1C2     EB 15                       JMP SHORT PathOfEx.006CC1D9
$+1E4    006CC1C4     8B87 68150000               MOV EAX, DWORD PTR DS:[EDI+0x1568]
$+1EA    006CC1CA     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
$+1ED    006CC1CD     51                          PUSH ECX
$+1EE    006CC1CE     8D48 F8                     LEA ECX, DWORD PTR DS:[EAX-0x8]
$+1F1    006CC1D1     E8 FA2BEBFF                 CALL PathOfEx.0057EDD0
$+1F6    006CC1D6     83CB 08                     OR EBX, 0x8
$+1F9    006CC1D9     8B08                        MOV ECX, DWORD PTR DS:[EAX]
$+1FB    006CC1DB     894D DC                     MOV DWORD PTR SS:[EBP-0x24], ECX
$+1FE    006CC1DE     8B48 04                     MOV ECX, DWORD PTR DS:[EAX+0x4]
$+201    006CC1E1     894D E0                     MOV DWORD PTR SS:[EBP-0x20], ECX
$+204    006CC1E4     C700 00000000               MOV DWORD PTR DS:[EAX], 0x0
$+20A    006CC1EA     C740 04 00000000            MOV DWORD PTR DS:[EAX+0x4], 0x0
$+211    006CC1F1     C745 FC 02000000            MOV DWORD PTR SS:[EBP-0x4], 0x2
$+218    006CC1F8     F6C3 08                     TEST BL, 0x8
$+21B    006CC1FB     74 0B                       JE SHORT PathOfEx.006CC208
$+21D    006CC1FD     8D4D D0                     LEA ECX, DWORD PTR SS:[EBP-0x30]
$+220    006CC200     83E3 F7                     AND EBX, -0x9
$+223    006CC203     E8 A8E7D9FF                 CALL PathOfEx.0046A9B0
$+228    006CC208     F6C3 04                     TEST BL, 0x4
$+22B    006CC20B     74 08                       JE SHORT PathOfEx.006CC215
$+22D    006CC20D     8D4D E8                     LEA ECX, DWORD PTR SS:[EBP-0x18]
$+230    006CC210     E8 9BE7D9FF                 CALL PathOfEx.0046A9B0
$+235    006CC215     8B8F 34010000               MOV ECX, DWORD PTR DS:[EDI+0x134]
$+23B    006CC21B     8D45 08                     LEA EAX, DWORD PTR SS:[EBP+0x8]
$+23E    006CC21E     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+23F    006CC21F     56                          PUSH ESI
$+240    006CC220     E8 0B320F00                 CALL PathOfEx.007BF430
$+245    006CC225     84C0                        TEST AL, AL
$+247    006CC227     74 6B                       JE SHORT PathOfEx.006CC294
$+249    006CC229     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+24D    006CC22D     0F84 8A030000               JE PathOfEx.006CC5BD
$+253    006CC233     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+257    006CC237     75 5B                       JNZ SHORT PathOfEx.006CC294
$+259    006CC239     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+25F    006CC23F     8D46 FB                     LEA EAX, DWORD PTR DS:[ESI-0x5]
$+262    006CC242     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+263    006CC243     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
$+266    006CC246     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+267    006CC247     E8 C4D7FAFF                 CALL PathOfEx.00679A10
$+26C    006CC24C     C645 FC 03                  MOV BYTE PTR SS:[EBP-0x4], 0x3
$+270    006CC250     8B4D 08                     MOV ECX, DWORD PTR SS:[EBP+0x8]
$+273    006CC253     E8 C8383600                 CALL PathOfEx.00A2FB20
$+278    006CC258     8BD0                        MOV EDX, EAX                                    ; PathOfEx.006CBFE0
$+27A    006CC25A     83FA 20                     CMP EDX, 0x20
$+27D    006CC25D     0F95C1                      SETNE CL
$+280    006CC260     84C9                        TEST CL, CL
$+282    006CC262     0F84 F1020000               JE PathOfEx.006CC559
$+288    006CC268     8B9F D8180000               MOV EBX, DWORD PTR DS:[EDI+0x18D8]
$+28E    006CC26E     8B8F 140C0000               MOV ECX, DWORD PTR DS:[EDI+0xC14]
$+294    006CC274     3B8B 400A0000               CMP ECX, DWORD PTR DS:[EBX+0xA40]
$+29A    006CC27A     74 33                       JE SHORT PathOfEx.006CC2AF
$+29C    006CC27C     51                          PUSH ECX
$+29D    006CC27D     8B8B 380A0000               MOV ECX, DWORD PTR DS:[EBX+0xA38]
$+2A3    006CC283     E8 48F03100                 CALL PathOfEx.009EB2D0
$+2A8    006CC288     84C0                        TEST AL, AL
$+2AA    006CC28A     75 23                       JNZ SHORT PathOfEx.006CC2AF
$+2AC    006CC28C     8D4D E8                     LEA ECX, DWORD PTR SS:[EBP-0x18]
$+2AF    006CC28F     E8 1CE7D9FF                 CALL PathOfEx.0046A9B0
$+2B4    006CC294     8D4D DC                     LEA ECX, DWORD PTR SS:[EBP-0x24]
$+2B7    006CC297     E8 14E7D9FF                 CALL PathOfEx.0046A9B0
$+2BC    006CC29C     5F                          POP EDI                                         ; 12D554D0
$+2BD    006CC29D     5E                          POP ESI                                         ; 12D554D0
$+2BE    006CC29E     5B                          POP EBX                                         ; 12D554D0
$+2BF    006CC29F     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+2C2    006CC2A2     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+2C9    006CC2A9     8BE5                        MOV ESP, EBP
$+2CB    006CC2AB     5D                          POP EBP                                         ; 017072CA
$+2CC    006CC2AC     C2 1000                     RETN 0x10
$+2CF    006CC2AF     8B9B 64090000               MOV EBX, DWORD PTR DS:[EBX+0x964]
$+2D5    006CC2B5     83FA 01                     CMP EDX, 0x1
$+2D8    006CC2B8     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+2DE    006CC2BE     0F94C2                      SETE DL
$+2E1    006CC2C1     8855 0F                     MOV BYTE PTR SS:[EBP+0xF], DL
$+2E4    006CC2C4     83BB 38090000 01            CMP DWORD PTR DS:[EBX+0x938], 0x1
$+2EB    006CC2CB     8B83 40090000               MOV EAX, DWORD PTR DS:[EBX+0x940]
$+2F1    006CC2D1     8945 D8                     MOV DWORD PTR SS:[EBP-0x28], EAX                ; PathOfEx.006CBFE0
$+2F4    006CC2D4     8B83 4C090000               MOV EAX, DWORD PTR DS:[EBX+0x94C]
$+2FA    006CC2DA     8945 E4                     MOV DWORD PTR SS:[EBP-0x1C], EAX                ; PathOfEx.006CBFE0
$+2FD    006CC2DD     8B81 943A0000               MOV EAX, DWORD PTR DS:[ECX+0x3A94]              ; PathOfEx.00740065
$+303    006CC2E3     8B40 08                     MOV EAX, DWORD PTR DS:[EAX+0x8]
$+306    006CC2E6     8A70 0C                     MOV DH, BYTE PTR DS:[EAX+0xC]
$+309    006CC2E9     8B81 943A0000               MOV EAX, DWORD PTR DS:[ECX+0x3A94]              ; PathOfEx.00740065
$+30F    006CC2EF     8875 17                     MOV BYTE PTR SS:[EBP+0x17], DH
$+312    006CC2F2     8B40 08                     MOV EAX, DWORD PTR DS:[EAX+0x8]
$+315    006CC2F5     8A88 DE000000               MOV CL, BYTE PTR DS:[EAX+0xDE]
$+31B    006CC2FB     8B45 E4                     MOV EAX, DWORD PTR SS:[EBP-0x1C]
$+31E    006CC2FE     884D F3                     MOV BYTE PTR SS:[EBP-0xD], CL
$+321    006CC301     75 1F                       JNZ SHORT PathOfEx.006CC322
$+323    006CC303     84D2                        TEST DL, DL
$+325    006CC305     74 1B                       JE SHORT PathOfEx.006CC322
$+327    006CC307     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+329    006CC309     74 17                       JE SHORT PathOfEx.006CC322
$+32B    006CC30B     84F6                        TEST DH, DH
$+32D    006CC30D     75 04                       JNZ SHORT PathOfEx.006CC313
$+32F    006CC30F     84C9                        TEST CL, CL
$+331    006CC311     74 09                       JE SHORT PathOfEx.006CC31C
$+333    006CC313     83BF BC180000 00            CMP DWORD PTR DS:[EDI+0x18BC], 0x0
$+33A    006CC31A     75 06                       JNZ SHORT PathOfEx.006CC322
$+33C    006CC31C     C645 13 01                  MOV BYTE PTR SS:[EBP+0x13], 0x1
$+340    006CC320     EB 04                       JMP SHORT PathOfEx.006CC326
$+342    006CC322     C645 13 00                  MOV BYTE PTR SS:[EBP+0x13], 0x0
$+346    006CC326     83BB 38090000 02            CMP DWORD PTR DS:[EBX+0x938], 0x2
$+34D    006CC32D     75 23                       JNZ SHORT PathOfEx.006CC352
$+34F    006CC32F     84D2                        TEST DL, DL
$+351    006CC331     74 1F                       JE SHORT PathOfEx.006CC352
$+353    006CC333     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+355    006CC335     74 1B                       JE SHORT PathOfEx.006CC352
$+357    006CC337     84F6                        TEST DH, DH
$+359    006CC339     75 17                       JNZ SHORT PathOfEx.006CC352
$+35B    006CC33B     84C9                        TEST CL, CL
$+35D    006CC33D     75 13                       JNZ SHORT PathOfEx.006CC352
$+35F    006CC33F     8B8F BC180000               MOV ECX, DWORD PTR DS:[EDI+0x18BC]
$+365    006CC345     E8 A69FFFFF                 CALL PathOfEx.006C62F0
$+36A    006CC34A     C645 0B 01                  MOV BYTE PTR SS:[EBP+0xB], 0x1
$+36E    006CC34E     84C0                        TEST AL, AL
$+370    006CC350     75 04                       JNZ SHORT PathOfEx.006CC356
$+372    006CC352     C645 0B 00                  MOV BYTE PTR SS:[EBP+0xB], 0x0
$+376    006CC356     83BB 38090000 02            CMP DWORD PTR DS:[EBX+0x938], 0x2
$+37D    006CC35D     75 2B                       JNZ SHORT PathOfEx.006CC38A
$+37F    006CC35F     807D 0F 00                  CMP BYTE PTR SS:[EBP+0xF], 0x0
$+383    006CC363     74 25                       JE SHORT PathOfEx.006CC38A
$+385    006CC365     837D E4 00                  CMP DWORD PTR SS:[EBP-0x1C], 0x0
$+389    006CC369     74 1F                       JE SHORT PathOfEx.006CC38A
$+38B    006CC36B     807D 17 00                  CMP BYTE PTR SS:[EBP+0x17], 0x0
$+38F    006CC36F     75 19                       JNZ SHORT PathOfEx.006CC38A
$+391    006CC371     807D F3 00                  CMP BYTE PTR SS:[EBP-0xD], 0x0
$+395    006CC375     75 13                       JNZ SHORT PathOfEx.006CC38A
$+397    006CC377     8B8F BC180000               MOV ECX, DWORD PTR DS:[EDI+0x18BC]
$+39D    006CC37D     E8 BE9FFFFF                 CALL PathOfEx.006C6340
$+3A2    006CC382     84C0                        TEST AL, AL
$+3A4    006CC384     74 04                       JE SHORT PathOfEx.006CC38A
$+3A6    006CC386     B4 01                       MOV AH, 0x1
$+3A8    006CC388     EB 02                       JMP SHORT PathOfEx.006CC38C
$+3AA    006CC38A     32E4                        XOR AH, AH
$+3AC    006CC38C     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+3B2    006CC392     8A55 0F                     MOV DL, BYTE PTR SS:[EBP+0xF]
$+3B5    006CC395     8A81 AC3E0000               MOV AL, BYTE PTR DS:[ECX+0x3EAC]
$+3BB    006CC39B     84C0                        TEST AL, AL
$+3BD    006CC39D     74 11                       JE SHORT PathOfEx.006CC3B0
$+3BF    006CC39F     83BB 38090000 05            CMP DWORD PTR DS:[EBX+0x938], 0x5
$+3C6    006CC3A6     75 08                       JNZ SHORT PathOfEx.006CC3B0
$+3C8    006CC3A8     84D2                        TEST DL, DL
$+3CA    006CC3AA     74 04                       JE SHORT PathOfEx.006CC3B0
$+3CC    006CC3AC     B0 01                       MOV AL, 0x1
$+3CE    006CC3AE     EB 02                       JMP SHORT PathOfEx.006CC3B2
$+3D0    006CC3B0     32C0                        XOR AL, AL
$+3D2    006CC3B2     807D 13 00                  CMP BYTE PTR SS:[EBP+0x13], 0x0
$+3D6    006CC3B6     74 0C                       JE SHORT PathOfEx.006CC3C4
$+3D8    006CC3B8     8BCF                        MOV ECX, EDI
$+3DA    006CC3BA     E8 310D0000                 CALL PathOfEx.006CD0F0
$+3DF    006CC3BF     E9 ED010000                 JMP PathOfEx.006CC5B1
$+3E4    006CC3C4     807D 0B 00                  CMP BYTE PTR SS:[EBP+0xB], 0x0
$+3E8    006CC3C8     0F85 ED000000               JNZ PathOfEx.006CC4BB
$+3EE    006CC3CE     84E4                        TEST AH, AH
$+3F0    006CC3D0     0F85 E5000000               JNZ PathOfEx.006CC4BB
$+3F6    006CC3D6     84C0                        TEST AL, AL
$+3F8    006CC3D8     74 4A                       JE SHORT PathOfEx.006CC424
$+3FA    006CC3DA     8BB7 D81A0000               MOV ESI, DWORD PTR DS:[EDI+0x1AD8]
$+400    006CC3E0     85F6                        TEST ESI, ESI
$+402    006CC3E2     74 25                       JE SHORT PathOfEx.006CC409
$+404    006CC3E4     8D87 9C180000               LEA EAX, DWORD PTR DS:[EDI+0x189C]
$+40A    006CC3EA     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+40B    006CC3EB     8D45 C8                     LEA EAX, DWORD PTR SS:[EBP-0x38]
$+40E    006CC3EE     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+40F    006CC3EF     E8 2C521200                 CALL PathOfEx.007F1620
$+414    006CC3F4     83C4 08                     ADD ESP, 0x8
$+417    006CC3F7     8BCE                        MOV ECX, ESI
$+419    006CC3F9     FF70 04                     PUSH DWORD PTR DS:[EAX+0x4]
$+41C    006CC3FC     FF30                        PUSH DWORD PTR DS:[EAX]
$+41E    006CC3FE     FF75 E4                     PUSH DWORD PTR SS:[EBP-0x1C]
$+421    006CC401     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
$+424    006CC404     E8 A721E0FF                 CALL PathOfEx.004CE5B0
$+429    006CC409     80BF D51A0000 00            CMP BYTE PTR DS:[EDI+0x1AD5], 0x0
$+430    006CC410     0F85 9B010000               JNZ PathOfEx.006CC5B1
$+436    006CC416     6A 01                       PUSH 0x1
$+438    006CC418     8BCF                        MOV ECX, EDI
$+43A    006CC41A     E8 613E0000                 CALL PathOfEx.006D0280
$+43F    006CC41F     E9 8D010000                 JMP PathOfEx.006CC5B1
$+444    006CC424     83B9 0C400000 00            CMP DWORD PTR DS:[ECX+0x400C], 0x0
$+44B    006CC42B     76 41                       JBE SHORT PathOfEx.006CC46E
$+44D    006CC42D     83B9 0C400000 01            CMP DWORD PTR DS:[ECX+0x400C], 0x1
$+454    006CC434     74 38                       JE SHORT PathOfEx.006CC46E
$+456    006CC436     8B87 D0180000               MOV EAX, DWORD PTR DS:[EDI+0x18D0]
$+45C    006CC43C     3B87 BC180000               CMP EAX, DWORD PTR DS:[EDI+0x18BC]
$+462    006CC442     74 36                       JE SHORT PathOfEx.006CC47A
$+464    006CC444     84D2                        TEST DL, DL
$+466    006CC446     75 19                       JNZ SHORT PathOfEx.006CC461
$+468    006CC448     8B4D E8                     MOV ECX, DWORD PTR SS:[EBP-0x18]
$+46B    006CC44B     85C9                        TEST ECX, ECX
$+46D    006CC44D     0F94C0                      SETE AL
$+470    006CC450     34 01                       XOR AL, 0x1
$+472    006CC452     74 26                       JE SHORT PathOfEx.006CC47A
$+474    006CC454     66:8B41 02                  MOV AX, WORD PTR DS:[ECX+0x2]
$+478    006CC458     66:3B05 0CD70001            CMP AX, WORD PTR DS:[0x100D70C]
$+47F    006CC45F     75 19                       JNZ SHORT PathOfEx.006CC47A
$+481    006CC461     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+487    006CC467     E8 E47CFBFF                 CALL PathOfEx.00684150
$+48C    006CC46C     EB 0C                       JMP SHORT PathOfEx.006CC47A
$+48E    006CC46E     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
$+491    006CC471     8BCF                        MOV ECX, EDI
$+493    006CC473     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+494    006CC474     56                          PUSH ESI
$+495    006CC475     E8 A64E0000                 CALL PathOfEx.006D1320
$+49A    006CC47A     8B87 D0180000               MOV EAX, DWORD PTR DS:[EDI+0x18D0]
$+4A0    006CC480     3B87 BC180000               CMP EAX, DWORD PTR DS:[EDI+0x18BC]
$+4A6    006CC486     0F84 25010000               JE PathOfEx.006CC5B1
$+4AC    006CC48C     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]
$+4B2    006CC492     8B88 64090000               MOV ECX, DWORD PTR DS:[EAX+0x964]
$+4B8    006CC498     8B81 38090000               MOV EAX, DWORD PTR DS:[ECX+0x938]
$+4BE    006CC49E     83F8 02                     CMP EAX, 0x2
$+4C1    006CC4A1     74 0E                       JE SHORT PathOfEx.006CC4B1
$+4C3    006CC4A3     83F8 03                     CMP EAX, 0x3
$+4C6    006CC4A6     74 09                       JE SHORT PathOfEx.006CC4B1
$+4C8    006CC4A8     83F8 04                     CMP EAX, 0x4
$+4CB    006CC4AB     0F85 00010000               JNZ PathOfEx.006CC5B1
$+4D1    006CC4B1     E8 DAC8ECFF                 CALL PathOfEx.00598D90
$+4D6    006CC4B6     E9 F6000000                 JMP PathOfEx.006CC5B1
$+4DB    006CC4BB     8A83 68090000               MOV AL, BYTE PTR DS:[EBX+0x968]
$+4E1    006CC4C1     84C0                        TEST AL, AL
$+4E3    006CC4C3     74 2C                       JE SHORT PathOfEx.006CC4F1
$+4E5    006CC4C5     8A83 78090000               MOV AL, BYTE PTR DS:[EBX+0x978]
$+4EB    006CC4CB     84C0                        TEST AL, AL
$+4ED    006CC4CD     74 22                       JE SHORT PathOfEx.006CC4F1
$+4EF    006CC4CF     FFB7 BC180000               PUSH DWORD PTR DS:[EDI+0x18BC]
$+4F5    006CC4D5     FFB3 5C090000               PUSH DWORD PTR DS:[EBX+0x95C]
$+4FB    006CC4DB     FFB3 58090000               PUSH DWORD PTR DS:[EBX+0x958]
$+501    006CC4E1     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
$+504    006CC4E4     FFB3 50090000               PUSH DWORD PTR DS:[EBX+0x950]
$+50A    006CC4EA     E8 919A2000                 CALL PathOfEx.008D5F80
$+50F    006CC4EF     EB 11                       JMP SHORT PathOfEx.006CC502
$+511    006CC4F1     FFB7 BC180000               PUSH DWORD PTR DS:[EDI+0x18BC]
$+517    006CC4F7     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
$+51A    006CC4FA     FF75 E4                     PUSH DWORD PTR SS:[EBP-0x1C]
$+51D    006CC4FD     E8 CE982000                 CALL PathOfEx.008D5DD0
$+522    006CC502     8D45 0C                     LEA EAX, DWORD PTR SS:[EBP+0xC]
$+525    006CC505     8BCB                        MOV ECX, EBX
$+527    006CC507     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+528    006CC508     E8 13C9ECFF                 CALL PathOfEx.00598E20
$+52D    006CC50D     8D4D 0C                     LEA ECX, DWORD PTR SS:[EBP+0xC]
$+530    006CC510     8B30                        MOV ESI, DWORD PTR DS:[EAX]
$+532    006CC512     E8 B905DAFF                 CALL PathOfEx.0046CAD0
$+537    006CC517     85F6                        TEST ESI, ESI
$+539    006CC519     0F84 92000000               JE PathOfEx.006CC5B1
$+53F    006CC51F     8D45 0C                     LEA EAX, DWORD PTR SS:[EBP+0xC]
$+542    006CC522     8BCB                        MOV ECX, EBX
$+544    006CC524     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+545    006CC525     E8 F6C8ECFF                 CALL PathOfEx.00598E20
$+54A    006CC52A     8BF0                        MOV ESI, EAX                                    ; PathOfEx.006CBFE0
$+54C    006CC52C     C645 FC 04                  MOV BYTE PTR SS:[EBP-0x4], 0x4
$+550    006CC530     E8 ABDD2200                 CALL PathOfEx.008FA2E0
$+555    006CC535     83EC 08                     SUB ESP, 0x8
$+558    006CC538     8B08                        MOV ECX, DWORD PTR DS:[EAX]
$+55A    006CC53A     C74424 04 0000803F          MOV DWORD PTR SS:[ESP+0x4], 0x3F800000
$+562    006CC542     C70424 0000803F             MOV DWORD PTR SS:[ESP], 0x3F800000
$+569    006CC549     56                          PUSH ESI
$+56A    006CC54A     E8 21CD2200                 CALL PathOfEx.008F9270
$+56F    006CC54F     8D4D 0C                     LEA ECX, DWORD PTR SS:[EBP+0xC]
$+572    006CC552     E8 7905DAFF                 CALL PathOfEx.0046CAD0
$+577    006CC557     EB 58                       JMP SHORT PathOfEx.006CC5B1
$+579    006CC559     8B87 3C010000               MOV EAX, DWORD PTR DS:[EDI+0x13C]
$+57F    006CC55F     83B8 0C400000 00            CMP DWORD PTR DS:[EAX+0x400C], 0x0
$+586    006CC566     76 3D                       JBE SHORT PathOfEx.006CC5A5
$+588    006CC568     83B8 0C400000 01            CMP DWORD PTR DS:[EAX+0x400C], 0x1
$+58F    006CC56F     74 34                       JE SHORT PathOfEx.006CC5A5
$+591    006CC571     8B87 D0180000               MOV EAX, DWORD PTR DS:[EDI+0x18D0]
$+597    006CC577     3B87 BC180000               CMP EAX, DWORD PTR DS:[EDI+0x18BC]
$+59D    006CC57D     74 32                       JE SHORT PathOfEx.006CC5B1
$+59F    006CC57F     8B4D E8                     MOV ECX, DWORD PTR SS:[EBP-0x18]
$+5A2    006CC582     85C9                        TEST ECX, ECX
$+5A4    006CC584     0F94C0                      SETE AL
$+5A7    006CC587     34 01                       XOR AL, 0x1
$+5A9    006CC589     74 26                       JE SHORT PathOfEx.006CC5B1
$+5AB    006CC58B     66:8B41 02                  MOV AX, WORD PTR DS:[ECX+0x2]
$+5AF    006CC58F     66:3B05 0CD70001            CMP AX, WORD PTR DS:[0x100D70C]
$+5B6    006CC596     75 19                       JNZ SHORT PathOfEx.006CC5B1
$+5B8    006CC598     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+5BE    006CC59E     E8 AD7BFBFF                 CALL PathOfEx.00684150
$+5C3    006CC5A3     EB 0C                       JMP SHORT PathOfEx.006CC5B1
$+5C5    006CC5A5     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
$+5C8    006CC5A8     8BCF                        MOV ECX, EDI
$+5CA    006CC5AA     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+5CB    006CC5AB     56                          PUSH ESI
$+5CC    006CC5AC     E8 6F4D0000                 CALL PathOfEx.006D1320
$+5D1    006CC5B1     8D4D E8                     LEA ECX, DWORD PTR SS:[EBP-0x18]
$+5D4    006CC5B4     C645 FC 02                  MOV BYTE PTR SS:[EBP-0x4], 0x2
$+5D8    006CC5B8     E8 F3E3D9FF                 CALL PathOfEx.0046A9B0
$+5DD    006CC5BD     8BCF                        MOV ECX, EDI
$+5DF    006CC5BF     E8 0C4A0000                 CALL PathOfEx.006D0FD0
$+5E4    006CC5C4     8D4D DC                     LEA ECX, DWORD PTR SS:[EBP-0x24]
$+5E7    006CC5C7     E8 E4E3D9FF                 CALL PathOfEx.0046A9B0
$+5EC    006CC5CC     5F                          POP EDI                                         ; 0018E210
$+5ED    006CC5CD     5E                          POP ESI                                         ; 0018E210
$+5EE    006CC5CE     5B                          POP EBX                                         ; 0018E210
$+5EF    006CC5CF     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+5F2    006CC5D2     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+5F9    006CC5D9     8BE5                        MOV ESP, EBP
$+5FB    006CC5DB     5D                          POP EBP                                         ; 0018E210
$+5FC    006CC5DC     C2 1000                     RETN 0x10
$+5FF    006CC5DF     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+603    006CC5E3     0F84 EB050000               JE PathOfEx.006CCBD4
$+609    006CC5E9     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+60D    006CC5ED     0F85 E1050000               JNZ PathOfEx.006CCBD4
$+613    006CC5F3     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]
$+619    006CC5F9     8B80 000A0000               MOV EAX, DWORD PTR DS:[EAX+0xA00]
$+61F    006CC5FF     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+621    006CC601     0F84 D8000000               JE PathOfEx.006CC6DF
$+627    006CC607     8A80 54070000               MOV AL, BYTE PTR DS:[EAX+0x754]
$+62D    006CC60D     84C0                        TEST AL, AL
$+62F    006CC60F     0F85 CA000000               JNZ PathOfEx.006CC6DF
$+635    006CC615     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+63B    006CC61B     8D45 E8                     LEA EAX, DWORD PTR SS:[EBP-0x18]
$+63E    006CC61E     6A 0E                       PUSH 0xE
$+640    006CC620     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+641    006CC621     81C1 EC3C0000               ADD ECX, 0x3CEC
$+647    006CC627     E8 24ECE3FF                 CALL PathOfEx.0050B250
$+64C    006CC62C     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+652    006CC632     8D45 DC                     LEA EAX, DWORD PTR SS:[EBP-0x24]
$+655    006CC635     6A 0F                       PUSH 0xF
$+657    006CC637     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+658    006CC638     81C1 EC3C0000               ADD ECX, 0x3CEC
$+65E    006CC63E     E8 0DECE3FF                 CALL PathOfEx.0050B250
$+663    006CC643     8B4D E8                     MOV ECX, DWORD PTR SS:[EBP-0x18]
$+666    006CC646     6A 00                       PUSH 0x0
$+668    006CC648     6A 00                       PUSH 0x0
$+66A    006CC64A     E8 E1250800                 CALL PathOfEx.0074EC30
$+66F    006CC64F     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+671    006CC651     75 12                       JNZ SHORT PathOfEx.006CC665
$+673    006CC653     8B4D DC                     MOV ECX, DWORD PTR SS:[EBP-0x24]
$+676    006CC656     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+677    006CC657     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+678    006CC658     E8 D3250800                 CALL PathOfEx.0074EC30
$+67D    006CC65D     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+67F    006CC65F     75 04                       JNZ SHORT PathOfEx.006CC665
$+681    006CC661     32DB                        XOR BL, BL
$+683    006CC663     EB 02                       JMP SHORT PathOfEx.006CC667
$+685    006CC665     B3 01                       MOV BL, 0x1
$+687    006CC667     8D4D DC                     LEA ECX, DWORD PTR SS:[EBP-0x24]
$+68A    006CC66A     E8 41E3D9FF                 CALL PathOfEx.0046A9B0
$+68F    006CC66F     8D4D E8                     LEA ECX, DWORD PTR SS:[EBP-0x18]
$+692    006CC672     E8 39E3D9FF                 CALL PathOfEx.0046A9B0
$+697    006CC677     84DB                        TEST BL, BL
$+699    006CC679     75 64                       JNZ SHORT PathOfEx.006CC6DF
$+69B    006CC67B     68 A0DCE600                 PUSH PathOfEx.00E6DCA0
$+6A0    006CC680     8D4D B0                     LEA ECX, DWORD PTR SS:[EBP-0x50]
$+6A3    006CC683     E8 2817DAFF                 CALL PathOfEx.0046DDB0
$+6A8    006CC688     BA 59000000                 MOV EDX, 0x59
$+6AD    006CC68D     C745 FC 05000000            MOV DWORD PTR SS:[EBP-0x4], 0x5
$+6B4    006CC694     8D4D 80                     LEA ECX, DWORD PTR SS:[EBP-0x80]
$+6B7    006CC697     E8 14F30400                 CALL PathOfEx.0071B9B0
$+6BC    006CC69C     6A 00                       PUSH 0x0
$+6BE    006CC69E     6A 00                       PUSH 0x0
$+6C0    006CC6A0     C645 FC 06                  MOV BYTE PTR SS:[EBP-0x4], 0x6
$+6C4    006CC6A4     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+6CA    006CC6AA     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+6CB    006CC6AB     8D45 B0                     LEA EAX, DWORD PTR SS:[EBP-0x50]
$+6CE    006CC6AE     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+6CF    006CC6AF     8B89 A8090000               MOV ECX, DWORD PTR DS:[ECX+0x9A8]               ; PathOfEx.00610069
$+6D5    006CC6B5     6A 00                       PUSH 0x0
$+6D7    006CC6B7     E8 94BEEBFF                 CALL PathOfEx.00588550
$+6DC    006CC6BC     8D4D 80                     LEA ECX, DWORD PTR SS:[EBP-0x80]
$+6DF    006CC6BF     E8 3C10DAFF                 CALL PathOfEx.0046D700
$+6E4    006CC6C4     8D4D B0                     LEA ECX, DWORD PTR SS:[EBP-0x50]
$+6E7    006CC6C7     E8 3410DAFF                 CALL PathOfEx.0046D700
$+6EC    006CC6CC     5F                          POP EDI                                         ; 0018E210
$+6ED    006CC6CD     5E                          POP ESI                                         ; 0018E210
$+6EE    006CC6CE     5B                          POP EBX                                         ; 0018E210
$+6EF    006CC6CF     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+6F2    006CC6D2     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+6F9    006CC6D9     8BE5                        MOV ESP, EBP
$+6FB    006CC6DB     5D                          POP EBP                                         ; 0018E210
$+6FC    006CC6DC     C2 1000                     RETN 0x10
$+6FF    006CC6DF     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+705    006CC6E5     E8 D6BD2000                 CALL PathOfEx.008D84C0
$+70A    006CC6EA     5F                          POP EDI                                         ; 0018E210
$+70B    006CC6EB     5E                          POP ESI                                         ; 0018E210
$+70C    006CC6EC     5B                          POP EBX                                         ; 0018E210
$+70D    006CC6ED     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+710    006CC6F0     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+717    006CC6F7     8BE5                        MOV ESP, EBP
$+719    006CC6F9     5D                          POP EBP                                         ; 0018E210
$+71A    006CC6FA     C2 1000                     RETN 0x10
$+71D    006CC6FD     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+721    006CC701     0F84 CD040000               JE PathOfEx.006CCBD4
$+727    006CC707     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+72B    006CC70B     0F85 C3040000               JNZ PathOfEx.006CCBD4
$+731    006CC711     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]
$+737    006CC717     8B88 68090000               MOV ECX, DWORD PTR DS:[EAX+0x968]
$+73D    006CC71D     FFB1 70090000               PUSH DWORD PTR DS:[ECX+0x970]
$+743    006CC723     E8 48B3F5FF                 CALL PathOfEx.00627A70
$+748    006CC728     5F                          POP EDI                                         ; 0018E210
$+749    006CC729     5E                          POP ESI                                         ; 0018E210
$+74A    006CC72A     5B                          POP EBX                                         ; 0018E210
$+74B    006CC72B     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+74E    006CC72E     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+755    006CC735     8BE5                        MOV ESP, EBP
$+757    006CC737     5D                          POP EBP
$+758    006CC738     C2 1000                     RETN 0x10
$+75B    006CC73B     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+75F    006CC73F     0F84 8F040000               JE PathOfEx.006CCBD4
$+765    006CC745     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+769    006CC749     0F85 85040000               JNZ PathOfEx.006CCBD4
$+76F    006CC74F     8BB7 34010000               MOV ESI, DWORD PTR DS:[EDI+0x134]
$+775    006CC755     8BCE                        MOV ECX, ESI
$+777    006CC757     E8 B41C0F00                 CALL PathOfEx.007BE410
$+77C    006CC75C     84C0                        TEST AL, AL
$+77E    006CC75E     8BCE                        MOV ECX, ESI
$+780    006CC760     0F94C0                      SETE AL
$+783    006CC763     0FB6C0                      MOVZX EAX, AL
$+786    006CC766     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+787    006CC767     E8 D41C0F00                 CALL PathOfEx.007BE440
$+78C    006CC76C     8B87 D8180000               MOV EAX, DWORD PTR DS:[EDI+0x18D8]
$+792    006CC772     8B80 240A0000               MOV EAX, DWORD PTR DS:[EAX+0xA24]
$+798    006CC778     85C0                        TEST EAX, EAX                                   ; PathOfEx.006CBFE0
$+79A    006CC77A     74 14                       JE SHORT PathOfEx.006CC790
$+79C    006CC77C     80B8 54070000 00            CMP BYTE PTR DS:[EAX+0x754], 0x0
$+7A3    006CC783     75 0B                       JNZ SHORT PathOfEx.006CC790
$+7A5    006CC785     8B8F 34010000               MOV ECX, DWORD PTR DS:[EDI+0x134]
$+7AB    006CC78B     E8 C03E0F00                 CALL PathOfEx.007C0650
$+7B0    006CC790     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+7B6    006CC796     E8 8522FBFF                 CALL PathOfEx.0067EA20
$+7BB    006CC79B     5F                          POP EDI
$+7BC    006CC79C     5E                          POP ESI
$+7BD    006CC79D     5B                          POP EBX
$+7BE    006CC79E     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+7C1    006CC7A1     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+7C8    006CC7A8     8BE5                        MOV ESP, EBP
$+7CA    006CC7AA     5D                          POP EBP
$+7CB    006CC7AB     C2 1000                     RETN 0x10
$+7CE    006CC7AE     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+7D2    006CC7B2     0F84 1C040000               JE PathOfEx.006CCBD4
$+7D8    006CC7B8     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+7DC    006CC7BC     0F85 12040000               JNZ PathOfEx.006CCBD4
$+7E2    006CC7C2     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+7E8    006CC7C8     E8 13D3FAFF                 CALL PathOfEx.00679AE0
$+7ED    006CC7CD     5F                          POP EDI
$+7EE    006CC7CE     5E                          POP ESI
$+7EF    006CC7CF     5B                          POP EBX
$+7F0    006CC7D0     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+7F3    006CC7D3     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+7FA    006CC7DA     8BE5                        MOV ESP, EBP
$+7FC    006CC7DC     5D                          POP EBP
$+7FD    006CC7DD     C2 1000                     RETN 0x10
$+800    006CC7E0     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+804    006CC7E4     0F84 EA030000               JE PathOfEx.006CCBD4
$+80A    006CC7EA     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+80E    006CC7EE     0F85 E0030000               JNZ PathOfEx.006CCBD4
$+814    006CC7F4     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+81A    006CC7FA     E8 31D2FAFF                 CALL PathOfEx.00679A30
$+81F    006CC7FF     5F                          POP EDI
$+820    006CC800     5E                          POP ESI
$+821    006CC801     5B                          POP EBX
$+822    006CC802     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+825    006CC805     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+82C    006CC80C     8BE5                        MOV ESP, EBP
$+82E    006CC80E     5D                          POP EBP
$+82F    006CC80F     C2 1000                     RETN 0x10
$+832    006CC812     8B97 D8180000               MOV EDX, DWORD PTR DS:[EDI+0x18D8]
$+838    006CC818     8B8A 240A0000               MOV ECX, DWORD PTR DS:[EDX+0xA24]
$+83E    006CC81E     85C9                        TEST ECX, ECX
$+840    006CC820     0F84 AE030000               JE PathOfEx.006CCBD4
$+846    006CC826     E8 A536F2FF                 CALL PathOfEx.005EFED0
$+84B    006CC82B     84C0                        TEST AL, AL
$+84D    006CC82D     0F85 A1030000               JNZ PathOfEx.006CCBD4
$+853    006CC833     3845 0C                     CMP BYTE PTR SS:[EBP+0xC], AL
$+856    006CC836     0F84 98030000               JE PathOfEx.006CCBD4
$+85C    006CC83C     3845 10                     CMP BYTE PTR SS:[EBP+0x10], AL
$+85F    006CC83F     0F85 8F030000               JNZ PathOfEx.006CCBD4
$+865    006CC845     51                          PUSH ECX
$+866    006CC846     8BCA                        MOV ECX, EDX
$+868    006CC848     E8 631BFBFF                 CALL PathOfEx.0067E3B0
$+86D    006CC84D     5F                          POP EDI
$+86E    006CC84E     5E                          POP ESI
$+86F    006CC84F     5B                          POP EBX
$+870    006CC850     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+873    006CC853     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+87A    006CC85A     8BE5                        MOV ESP, EBP
$+87C    006CC85C     5D                          POP EBP
$+87D    006CC85D     C2 1000                     RETN 0x10
$+880    006CC860     84C9                        TEST CL, CL
$+882    006CC862     0F85 6C030000               JNZ PathOfEx.006CCBD4
$+888    006CC868     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+88B    006CC86B     0F84 63030000               JE PathOfEx.006CCBD4
$+891    006CC871     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+894    006CC874     0F85 5A030000               JNZ PathOfEx.006CCBD4
$+89A    006CC87A     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+8A0    006CC880     FFB1 2C0A0000               PUSH DWORD PTR DS:[ECX+0xA2C]
$+8A6    006CC886     E8 251BFBFF                 CALL PathOfEx.0067E3B0
$+8AB    006CC88B     5F                          POP EDI
$+8AC    006CC88C     5E                          POP ESI
$+8AD    006CC88D     5B                          POP EBX
$+8AE    006CC88E     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+8B1    006CC891     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+8B8    006CC898     8BE5                        MOV ESP, EBP
$+8BA    006CC89A     5D                          POP EBP
$+8BB    006CC89B     C2 1000                     RETN 0x10
$+8BE    006CC89E     84C9                        TEST CL, CL
$+8C0    006CC8A0     0F85 2E030000               JNZ PathOfEx.006CCBD4
$+8C6    006CC8A6     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+8C9    006CC8A9     0F84 25030000               JE PathOfEx.006CCBD4
$+8CF    006CC8AF     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+8D2    006CC8B2     0F85 1C030000               JNZ PathOfEx.006CCBD4
$+8D8    006CC8B8     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+8DE    006CC8BE     FFB1 140A0000               PUSH DWORD PTR DS:[ECX+0xA14]                   ; PathOfEx.00E72B2C
$+8E4    006CC8C4     E8 E71AFBFF                 CALL PathOfEx.0067E3B0
$+8E9    006CC8C9     5F                          POP EDI
$+8EA    006CC8CA     5E                          POP ESI
$+8EB    006CC8CB     5B                          POP EBX
$+8EC    006CC8CC     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+8EF    006CC8CF     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+8F6    006CC8D6     8BE5                        MOV ESP, EBP
$+8F8    006CC8D8     5D                          POP EBP
$+8F9    006CC8D9     C2 1000                     RETN 0x10
$+8FC    006CC8DC     84C9                        TEST CL, CL
$+8FE    006CC8DE     0F85 F0020000               JNZ PathOfEx.006CCBD4
$+904    006CC8E4     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+907    006CC8E7     0F84 E7020000               JE PathOfEx.006CCBD4
$+90D    006CC8ED     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+910    006CC8F0     0F85 DE020000               JNZ PathOfEx.006CCBD4
$+916    006CC8F6     E8 D5493400                 CALL PathOfEx.00A112D0
$+91B    006CC8FB     84C0                        TEST AL, AL
$+91D    006CC8FD     0F85 D1020000               JNZ PathOfEx.006CCBD4
$+923    006CC903     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+929    006CC909     FFB1 200A0000               PUSH DWORD PTR DS:[ECX+0xA20]
$+92F    006CC90F     E8 9C1AFBFF                 CALL PathOfEx.0067E3B0
$+934    006CC914     5F                          POP EDI                                         ; 12E3C800
$+935    006CC915     5E                          POP ESI
$+936    006CC916     5B                          POP EBX
$+937    006CC917     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+93A    006CC91A     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+941    006CC921     8BE5                        MOV ESP, EBP
$+943    006CC923     5D                          POP EBP                                         ; 38A53A30
$+944    006CC924     C2 1000                     RETN 0x10
$+947    006CC927     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+94B    006CC92B     0F84 A3020000               JE PathOfEx.006CCBD4
$+951    006CC931     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+955    006CC935     0F85 99020000               JNZ PathOfEx.006CCBD4
$+95B    006CC93B     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+961    006CC941     FFB1 000A0000               PUSH DWORD PTR DS:[ECX+0xA00]
$+967    006CC947     E8 641AFBFF                 CALL PathOfEx.0067E3B0
$+96C    006CC94C     5F                          POP EDI                                         ; 12D554D0
$+96D    006CC94D     5E                          POP ESI                                         ; 12D554D0
$+96E    006CC94E     5B                          POP EBX                                         ; nvd3dum.67C26293
$+96F    006CC94F     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+972    006CC952     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+979    006CC959     8BE5                        MOV ESP, EBP
$+97B    006CC95B     5D                          POP EBP                                         ; 01707334
$+97C    006CC95C     C2 1000                     RETN 0x10
$+97F    006CC95F     8B87 3C010000               MOV EAX, DWORD PTR DS:[EDI+0x13C]
$+985    006CC965     80B8 693B0000 00            CMP BYTE PTR DS:[EAX+0x3B69], 0x0
$+98C    006CC96C     0F85 62020000               JNZ PathOfEx.006CCBD4
$+992    006CC972     84C9                        TEST CL, CL
$+994    006CC974     0F85 5A020000               JNZ PathOfEx.006CCBD4
$+99A    006CC97A     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+99D    006CC97D     0F84 51020000               JE PathOfEx.006CCBD4
$+9A3    006CC983     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+9A6    006CC986     0F85 48020000               JNZ PathOfEx.006CCBD4
$+9AC    006CC98C     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+9B2    006CC992     FFB1 300A0000               PUSH DWORD PTR DS:[ECX+0xA30]
$+9B8    006CC998     E8 131AFBFF                 CALL PathOfEx.0067E3B0
$+9BD    006CC99D     5F                          POP EDI                                         ; 0018E210
$+9BE    006CC99E     5E                          POP ESI                                         ; 0018E210
$+9BF    006CC99F     5B                          POP EBX                                         ; 0018E210
$+9C0    006CC9A0     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+9C3    006CC9A3     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+9CA    006CC9AA     8BE5                        MOV ESP, EBP
$+9CC    006CC9AC     5D                          POP EBP                                         ; 0018E210
$+9CD    006CC9AD     C2 1000                     RETN 0x10
$+9D0    006CC9B0     84C9                        TEST CL, CL
$+9D2    006CC9B2     0F85 1C020000               JNZ PathOfEx.006CCBD4
$+9D8    006CC9B8     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+9DB    006CC9BB     0F84 13020000               JE PathOfEx.006CCBD4
$+9E1    006CC9C1     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+9E4    006CC9C4     0F85 0A020000               JNZ PathOfEx.006CCBD4
$+9EA    006CC9CA     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+9F0    006CC9D0     E8 7B1AFBFF                 CALL PathOfEx.0067E450
$+9F5    006CC9D5     5F                          POP EDI                                         ; 0018E210
$+9F6    006CC9D6     5E                          POP ESI                                         ; 0018E210
$+9F7    006CC9D7     5B                          POP EBX                                         ; 0018E210
$+9F8    006CC9D8     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+9FB    006CC9DB     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+A02    006CC9E2     8BE5                        MOV ESP, EBP
$+A04    006CC9E4     5D                          POP EBP                                         ; 0018E210
$+A05    006CC9E5     C2 1000                     RETN 0x10
$+A08    006CC9E8     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+A0C    006CC9EC     0F84 E2010000               JE PathOfEx.006CCBD4
$+A12    006CC9F2     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+A16    006CC9F6     0F85 D8010000               JNZ PathOfEx.006CCBD4
$+A1C    006CC9FC     8B8F 3C010000               MOV ECX, DWORD PTR DS:[EDI+0x13C]
$+A22    006CCA02     8B81 743B0000               MOV EAX, DWORD PTR DS:[ECX+0x3B74]
$+A28    006CCA08     3B81 783B0000               CMP EAX, DWORD PTR DS:[ECX+0x3B78]
$+A2E    006CCA0E     0F84 C0010000               JE PathOfEx.006CCBD4
$+A34    006CCA14     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+A3A    006CCA1A     FFB1 280A0000               PUSH DWORD PTR DS:[ECX+0xA28]
$+A40    006CCA20     E8 8B19FBFF                 CALL PathOfEx.0067E3B0
$+A45    006CCA25     5F                          POP EDI                                         ; 0018E210
$+A46    006CCA26     5E                          POP ESI                                         ; 0018E210
$+A47    006CCA27     5B                          POP EBX                                         ; 0018E210
$+A48    006CCA28     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+A4B    006CCA2B     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+A52    006CCA32     8BE5                        MOV ESP, EBP
$+A54    006CCA34     5D                          POP EBP                                         ; 0018E210
$+A55    006CCA35     C2 1000                     RETN 0x10
$+A58    006CCA38     84C9                        TEST CL, CL
$+A5A    006CCA3A     0F85 94010000               JNZ PathOfEx.006CCBD4
$+A60    006CCA40     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+A63    006CCA43     0F84 8B010000               JE PathOfEx.006CCBD4
$+A69    006CCA49     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+A6C    006CCA4C     0F85 82010000               JNZ PathOfEx.006CCBD4
$+A72    006CCA52     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+A78    006CCA58     E8 B31AFBFF                 CALL PathOfEx.0067E510
$+A7D    006CCA5D     5F                          POP EDI                                         ; 0018E210
$+A7E    006CCA5E     5E                          POP ESI                                         ; 0018E210
$+A7F    006CCA5F     5B                          POP EBX                                         ; 0018E210
$+A80    006CCA60     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+A83    006CCA63     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+A8A    006CCA6A     8BE5                        MOV ESP, EBP
$+A8C    006CCA6C     5D                          POP EBP                                         ; 0018E210
$+A8D    006CCA6D     C2 1000                     RETN 0x10
$+A90    006CCA70     84C9                        TEST CL, CL
$+A92    006CCA72     0F85 5C010000               JNZ PathOfEx.006CCBD4
$+A98    006CCA78     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+A9B    006CCA7B     0F84 53010000               JE PathOfEx.006CCBD4
$+AA1    006CCA81     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+AA4    006CCA84     0F85 4A010000               JNZ PathOfEx.006CCBD4
$+AAA    006CCA8A     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+AB0    006CCA90     FFB1 F8090000               PUSH DWORD PTR DS:[ECX+0x9F8]
$+AB6    006CCA96     E8 1519FBFF                 CALL PathOfEx.0067E3B0
$+ABB    006CCA9B     5F                          POP EDI                                         ; 0018E210
$+ABC    006CCA9C     5E                          POP ESI                                         ; 0018E210
$+ABD    006CCA9D     5B                          POP EBX                                         ; 0018E210
$+ABE    006CCA9E     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+AC1    006CCAA1     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+AC8    006CCAA8     8BE5                        MOV ESP, EBP
$+ACA    006CCAAA     5D                          POP EBP                                         ; 0018E210
$+ACB    006CCAAB     C2 1000                     RETN 0x10
$+ACE    006CCAAE     84C9                        TEST CL, CL
$+AD0    006CCAB0     0F85 1E010000               JNZ PathOfEx.006CCBD4
$+AD6    006CCAB6     384D 0C                     CMP BYTE PTR SS:[EBP+0xC], CL
$+AD9    006CCAB9     0F84 15010000               JE PathOfEx.006CCBD4
$+ADF    006CCABF     384D 10                     CMP BYTE PTR SS:[EBP+0x10], CL
$+AE2    006CCAC2     0F85 0C010000               JNZ PathOfEx.006CCBD4
$+AE8    006CCAC8     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+AEE    006CCACE     6A 00                       PUSH 0x0
$+AF0    006CCAD0     6A 00                       PUSH 0x0
$+AF2    006CCAD2     6A 00                       PUSH 0x0
$+AF4    006CCAD4     E8 071DFBFF                 CALL PathOfEx.0067E7E0
$+AF9    006CCAD9     5F                          POP EDI                                         ; 0018E210
$+AFA    006CCADA     5E                          POP ESI                                         ; 0018E210
$+AFB    006CCADB     5B                          POP EBX                                         ; 0018E210
$+AFC    006CCADC     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+AFF    006CCADF     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+B06    006CCAE6     8BE5                        MOV ESP, EBP
$+B08    006CCAE8     5D                          POP EBP                                         ; 0018E210
$+B09    006CCAE9     C2 1000                     RETN 0x10
$+B0C    006CCAEC     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+B10    006CCAF0     0F84 DE000000               JE PathOfEx.006CCBD4
$+B16    006CCAF6     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+B1A    006CCAFA     0F85 D4000000               JNZ PathOfEx.006CCBD4
$+B20    006CCB00     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+B26    006CCB06     E8 65D0FAFF                 CALL PathOfEx.00679B70
$+B2B    006CCB0B     5F                          POP EDI                                         ; 0018E210
$+B2C    006CCB0C     5E                          POP ESI                                         ; 0018E210
$+B2D    006CCB0D     5B                          POP EBX                                         ; 0018E210
$+B2E    006CCB0E     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+B31    006CCB11     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+B38    006CCB18     8BE5                        MOV ESP, EBP
$+B3A    006CCB1A     5D                          POP EBP                                         ; 0018E210
$+B3B    006CCB1B     C2 1000                     RETN 0x10
$+B3E    006CCB1E     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+B42    006CCB22     0F84 AC000000               JE PathOfEx.006CCBD4
$+B48    006CCB28     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+B4C    006CCB2C     0F85 A2000000               JNZ PathOfEx.006CCBD4
$+B52    006CCB32     51                          PUSH ECX
$+B53    006CCB33     8B8F 001B0000               MOV ECX, DWORD PTR DS:[EDI+0x1B00]
$+B59    006CCB39     E8 528D2500                 CALL PathOfEx.00925890
$+B5E    006CCB3E     83C4 04                     ADD ESP, 0x4
$+B61    006CCB41     8D4D 98                     LEA ECX, DWORD PTR SS:[EBP-0x68]
$+B64    006CCB44     68 A0DCE600                 PUSH PathOfEx.00E6DCA0
$+B69    006CCB49     E8 6212DAFF                 CALL PathOfEx.0046DDB0
$+B6E    006CCB4E     BA 52030000                 MOV EDX, 0x352
$+B73    006CCB53     C745 FC 07000000            MOV DWORD PTR SS:[EBP-0x4], 0x7
$+B7A    006CCB5A     8D8D 68FFFFFF               LEA ECX, DWORD PTR SS:[EBP-0x98]
$+B80    006CCB60     E8 4BEE0400                 CALL PathOfEx.0071B9B0
$+B85    006CCB65     6A 00                       PUSH 0x0
$+B87    006CCB67     6A 00                       PUSH 0x0
$+B89    006CCB69     C645 FC 08                  MOV BYTE PTR SS:[EBP-0x4], 0x8
$+B8D    006CCB6D     8B8F D8180000               MOV ECX, DWORD PTR DS:[EDI+0x18D8]
$+B93    006CCB73     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+B94    006CCB74     8D45 98                     LEA EAX, DWORD PTR SS:[EBP-0x68]
$+B97    006CCB77     50                          PUSH EAX                                        ; PathOfEx.006CBFE0
$+B98    006CCB78     8B89 A8090000               MOV ECX, DWORD PTR DS:[ECX+0x9A8]               ; PathOfEx.00610069
$+B9E    006CCB7E     6A 00                       PUSH 0x0
$+BA0    006CCB80     E8 CBB9EBFF                 CALL PathOfEx.00588550
$+BA5    006CCB85     8D8D 68FFFFFF               LEA ECX, DWORD PTR SS:[EBP-0x98]
$+BAB    006CCB8B     E8 700BDAFF                 CALL PathOfEx.0046D700
$+BB0    006CCB90     8D4D 98                     LEA ECX, DWORD PTR SS:[EBP-0x68]
$+BB3    006CCB93     E8 680BDAFF                 CALL PathOfEx.0046D700
$+BB8    006CCB98     5F                          POP EDI                                         ; 0018E210
$+BB9    006CCB99     5E                          POP ESI                                         ; 0018E210
$+BBA    006CCB9A     5B                          POP EBX                                         ; 0018E210
$+BBB    006CCB9B     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+BBE    006CCB9E     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+BC5    006CCBA5     8BE5                        MOV ESP, EBP
$+BC7    006CCBA7     5D                          POP EBP                                         ; 0018E210
$+BC8    006CCBA8     C2 1000                     RETN 0x10
$+BCB    006CCBAB     807D 0C 00                  CMP BYTE PTR SS:[EBP+0xC], 0x0
$+BCF    006CCBAF     74 23                       JE SHORT PathOfEx.006CCBD4
$+BD1    006CCBB1     807D 10 00                  CMP BYTE PTR SS:[EBP+0x10], 0x0
$+BD5    006CCBB5     75 1D                       JNZ SHORT PathOfEx.006CCBD4
$+BD7    006CCBB7     8BCF                        MOV ECX, EDI
$+BD9    006CCBB9     E8 322F0000                 CALL PathOfEx.006CFAF0
$+BDE    006CCBBE     5F                          POP EDI                                         ; 0018E210
$+BDF    006CCBBF     5E                          POP ESI                                         ; 0018E210
$+BE0    006CCBC0     5B                          POP EBX                                         ; 0018E210
$+BE1    006CCBC1     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+BE4    006CCBC4     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+BEB    006CCBCB     8BE5                        MOV ESP, EBP
$+BED    006CCBCD     5D                          POP EBP                                         ; 0018E210
$+BEE    006CCBCE     C2 1000                     RETN 0x10
$+BF1    006CCBD1     C602 00                     MOV BYTE PTR DS:[EDX], 0x0
$+BF4    006CCBD4     8B4D F4                     MOV ECX, DWORD PTR SS:[EBP-0xC]
$+BF7    006CCBD7     5F                          POP EDI                                         ; 0018E210
$+BF8    006CCBD8     5E                          POP ESI                                         ; 0018E210
$+BF9    006CCBD9     5B                          POP EBX                                         ; 0018E210
$+BFA    006CCBDA     64:890D 00000000            MOV DWORD PTR FS:[0], ECX
$+C01    006CCBE1     8BE5                        MOV ESP, EBP
$+C03    006CCBE3     5D                          POP EBP                                         ; 0018E210
$+C04    006CCBE4     C2 1000                     RETN 0x10
$+C07    006CCBE7     90                          NOP
*/
/**/
//tag_获得任务的任务状态
/*
$-31     007E625F     CC                        INT3
$-30     007E6260     51                        PUSH ECX
$-2F     007E6261     56                        PUSH ESI
$-2E     007E6262     8BF1                      MOV ESI,ECX
$-2C     007E6264     E8 B7050000               CALL PathOfEx.007E6820
$-27     007E6269     8D46 34                   LEA EAX,DWORD PTR DS:[ESI+0x34]
$-24     007E626C     2BF0                      SUB ESI,EAX
$-22     007E626E     83C6 40                   ADD ESI,0x40
$-1F     007E6271     56                        PUSH ESI
$-1E     007E6272     50                        PUSH EAX
$-1D     007E6273     FF7424 14                 PUSH DWORD PTR SS:[ESP+0x14]
$-19     007E6277     E8 04EA4A00               CALL PathOfEx.00C94C80
$-14     007E627C     83C4 0C                   ADD ESP,0xC
$-11     007E627F     5E                        POP ESI
$-10     007E6280     59                        POP ECX
$-F      007E6281     C2 0400                   RETN 0x4
$-C      007E6284     CC                        INT3
$-B      007E6285     CC                        INT3
$-A      007E6286     CC                        INT3
$-9      007E6287     CC                        INT3
$-8      007E6288     CC                        INT3
$-7      007E6289     CC                        INT3
$-6      007E628A     CC                        INT3
$-5      007E628B     CC                        INT3
$-4      007E628C     CC                        INT3
$-3      007E628D     CC                        INT3
$-2      007E628E     CC                        INT3
$-1      007E628F     CC                        INT3
$ ==>    007E6290     55                        PUSH EBP                                           ; tag_获得任务的任务状态
$+1      007E6291     8BEC                      MOV EBP,ESP                                        ; ecx=stCD_Player*
$+3      007E6293     6A FF                     PUSH -0x1                                          ; p1=stCD_QuestStatesPair*,输出结果
$+5      007E6295     68 0981D500               PUSH PathOfEx.00D58109                             ; p2=const stCD_QuestPair*
$+A      007E629A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]                           ; p3=enCD_kMapDifficulty
$+10     007E62A0     50                        PUSH EAX                                           ; 成功则eax=p1
$+11     007E62A1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     007E62A8     83EC 54                   SUB ESP,0x54
$+1B     007E62AB     53                        PUSH EBX
$+1C     007E62AC     56                        PUSH ESI
$+1D     007E62AD     57                        PUSH EDI
$+1E     007E62AE     83EC 10                   SUB ESP,0x10
$+21     007E62B1     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18],0x0
$+28     007E62B8     8BF9                      MOV EDI,ECX
$+2A     007E62BA     897D D8                   MOV DWORD PTR SS:[EBP-0x28],EDI
$+2D     007E62BD     E8 3E1DF3FF               CALL PathOfEx.00718000
$+32     007E62C2     8BC8                      MOV ECX,EAX
$+34     007E62C4     E8 D723E3FF               CALL PathOfEx.006186A0
$+39     007E62C9     8BF0                      MOV ESI,EAX
$+3B     007E62CB     8975 E4                   MOV DWORD PTR SS:[EBP-0x1C],ESI
$+3E     007E62CE     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
$+45     007E62D5     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
$+48     007E62D8     C745 EC 00000000          MOV DWORD PTR SS:[EBP-0x14],0x0
$+4F     007E62DF     8B40 10                   MOV EAX,DWORD PTR DS:[EAX+0x10]
$+52     007E62E2     83C0 08                   ADD EAX,0x8
$+55     007E62E5     8945 DC                   MOV DWORD PTR SS:[EBP-0x24],EAX
$+58     007E62E8     8B55 0C                   MOV EDX,DWORD PTR SS:[EBP+0xC]
$+5B     007E62EB     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
$+5E     007E62EE     50                        PUSH EAX
$+5F     007E62EF     8D4D A4                   LEA ECX,DWORD PTR SS:[EBP-0x5C]
$+62     007E62F2     E8 694EDEFF               CALL PathOfEx.005CB160
$+67     007E62F7     83C4 04                   ADD ESP,0x4
$+6A     007E62FA     8B48 08                   MOV ECX,DWORD PTR DS:[EAX+0x8]
$+6D     007E62FD     894D B0                   MOV DWORD PTR SS:[EBP-0x50],ECX
$+70     007E6300     8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+72     007E6302     C700 00000000             MOV DWORD PTR DS:[EAX],0x0
$+78     007E6308     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
$+7B     007E630B     894D B4                   MOV DWORD PTR SS:[EBP-0x4C],ECX
$+7E     007E630E     8945 B8                   MOV DWORD PTR SS:[EBP-0x48],EAX
$+81     007E6311     8D45 B0                   LEA EAX,DWORD PTR SS:[EBP-0x50]
$+84     007E6314     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
$+88     007E6318     50                        PUSH EAX
$+89     007E6319     8D55 E4                   LEA EDX,DWORD PTR SS:[EBP-0x1C]
$+8C     007E631C     8D4D D0                   LEA ECX,DWORD PTR SS:[EBP-0x30]
$+8F     007E631F     E8 9CEDF9FF               CALL PathOfEx.007850C0
$+94     007E6324     83C4 04                   ADD ESP,0x4
$+97     007E6327     C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
$+9B     007E632B     8B45 B4                   MOV EAX,DWORD PTR SS:[EBP-0x4C]
$+9E     007E632E     85C0                      TEST EAX,EAX
$+A0     007E6330     74 18                     JE SHORT PathOfEx.007E634A
$+A2     007E6332     50                        PUSH EAX
$+A3     007E6333     51                        PUSH ECX
$+A4     007E6334     E8 C71CF3FF               CALL PathOfEx.00718000
$+A9     007E6339     83C4 04                   ADD ESP,0x4
$+AC     007E633C     8BC8                      MOV ECX,EAX
$+AE     007E633E     E8 4D18F3FF               CALL PathOfEx.00717B90
$+B3     007E6343     C745 B4 00000000          MOV DWORD PTR SS:[EBP-0x4C],0x0
$+BA     007E634A     C645 FC 08                MOV BYTE PTR SS:[EBP-0x4],0x8
$+BE     007E634E     8B45 A4                   MOV EAX,DWORD PTR SS:[EBP-0x5C]
$+C1     007E6351     85C0                      TEST EAX,EAX
$+C3     007E6353     74 18                     JE SHORT PathOfEx.007E636D
$+C5     007E6355     50                        PUSH EAX
$+C6     007E6356     51                        PUSH ECX
$+C7     007E6357     E8 A41CF3FF               CALL PathOfEx.00718000
$+CC     007E635C     83C4 04                   ADD ESP,0x4
$+CF     007E635F     8BC8                      MOV ECX,EAX
$+D1     007E6361     E8 2A18F3FF               CALL PathOfEx.00717B90
$+D6     007E6366     C745 A4 00000000          MOV DWORD PTR SS:[EBP-0x5C],0x0
$+DD     007E636D     C645 FC 07                MOV BYTE PTR SS:[EBP-0x4],0x7
$+E1     007E6371     8B5D D4                   MOV EBX,DWORD PTR SS:[EBP-0x2C]
$+E4     007E6374     85DB                      TEST EBX,EBX
$+E6     007E6376     0F84 29010000             JE PathOfEx.007E64A5
$+EC     007E637C     8B43 0C                   MOV EAX,DWORD PTR DS:[EBX+0xC]
$+EF     007E637F     8B4B 10                   MOV ECX,DWORD PTR DS:[EBX+0x10]
$+F2     007E6382     897D C4                   MOV DWORD PTR SS:[EBP-0x3C],EDI
$+F5     007E6385     8D1481                    LEA EDX,DWORD PTR DS:[ECX+EAX*4]
$+F8     007E6388     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]
$+FB     007E638B     8945 C8                   MOV DWORD PTR SS:[EBP-0x38],EAX
$+FE     007E638E     8D45 C4                   LEA EAX,DWORD PTR SS:[EBP-0x3C]
$+101    007E6391     50                        PUSH EAX
$+102    007E6392     E8 090D0000               CALL PathOfEx.007E70A0
$+107    007E6397     83C4 04                   ADD ESP,0x4
$+10A    007E639A     84C0                      TEST AL,AL
$+10C    007E639C     0F84 8C000000             JE PathOfEx.007E642E
$+112    007E63A2     8B43 14                   MOV EAX,DWORD PTR DS:[EBX+0x14]
$+115    007E63A5     8B4B 18                   MOV ECX,DWORD PTR DS:[EBX+0x18]
$+118    007E63A8     897D BC                   MOV DWORD PTR SS:[EBP-0x44],EDI
$+11B    007E63AB     8D1481                    LEA EDX,DWORD PTR DS:[ECX+EAX*4]
$+11E    007E63AE     8D45 10                   LEA EAX,DWORD PTR SS:[EBP+0x10]
$+121    007E63B1     8945 C0                   MOV DWORD PTR SS:[EBP-0x40],EAX
$+124    007E63B4     8D45 BC                   LEA EAX,DWORD PTR SS:[EBP-0x44]
$+127    007E63B7     50                        PUSH EAX
$+128    007E63B8     E8 330D0000               CALL PathOfEx.007E70F0
$+12D    007E63BD     83C4 04                   ADD ESP,0x4
$+130    007E63C0     84C0                      TEST AL,AL
$+132    007E63C2     74 6A                     JE SHORT PathOfEx.007E642E
$+134    007E63C4     8B4B 52                   MOV ECX,DWORD PTR DS:[EBX+0x52]
$+137    007E63C7     8D45 E0                   LEA EAX,DWORD PTR SS:[EBP-0x20]
$+13A    007E63CA     50                        PUSH EAX
$+13B    007E63CB     8B43 4E                   MOV EAX,DWORD PTR DS:[EBX+0x4E]
$+13E    007E63CE     897D E0                   MOV DWORD PTR SS:[EBP-0x20],EDI
$+141    007E63D1     8D1481                    LEA EDX,DWORD PTR DS:[ECX+EAX*4]
$+144    007E63D4     E8 670D0000               CALL PathOfEx.007E7140
$+149    007E63D9     83C4 04                   ADD ESP,0x4
$+14C    007E63DC     84C0                      TEST AL,AL
$+14E    007E63DE     74 4E                     JE SHORT PathOfEx.007E642E
$+150    007E63E0     8B4B 41                   MOV ECX,DWORD PTR DS:[EBX+0x41]
$+153    007E63E3     85C9                      TEST ECX,ECX
$+155    007E63E5     74 72                     JE SHORT PathOfEx.007E6459
$+157    007E63E7     8B43 45                   MOV EAX,DWORD PTR DS:[EBX+0x45]
$+15A    007E63EA     8D0CC8                    LEA ECX,DWORD PTR DS:[EAX+ECX*8]
$+15D    007E63ED     894D E0                   MOV DWORD PTR SS:[EBP-0x20],ECX
$+160    007E63F0     3BC1                      CMP EAX,ECX
$+162    007E63F2     74 34                     JE SHORT PathOfEx.007E6428
$+164    007E63F4     8B4D DC                   MOV ECX,DWORD PTR SS:[EBP-0x24]
$+167    007E63F7     83C0 04                   ADD EAX,0x4
$+16A    007E63FA     8B39                      MOV EDI,DWORD PTR DS:[ECX]
$+16C    007E63FC     0F1F40 00                 NOP DWORD PTR DS:[EAX]
$+170    007E6400     8B10                      MOV EDX,DWORD PTR DS:[EAX]
$+172    007E6402     3BFA                      CMP EDI,EDX
$+174    007E6404     74 53                     JE SHORT PathOfEx.007E6459
$+176    007E6406     8A8F 15010000             MOV CL,BYTE PTR DS:[EDI+0x115]
$+17C    007E640C     84C9                      TEST CL,CL
$+17E    007E640E     74 0A                     JE SHORT PathOfEx.007E641A
$+180    007E6410     8A8A 14010000             MOV CL,BYTE PTR DS:[EDX+0x114]
$+186    007E6416     84C9                      TEST CL,CL
$+188    007E6418     75 3F                     JNZ SHORT PathOfEx.007E6459
$+18A    007E641A     83C0 08                   ADD EAX,0x8
$+18D    007E641D     8D48 FC                   LEA ECX,DWORD PTR DS:[EAX-0x4]
$+190    007E6420     3B4D E0                   CMP ECX,DWORD PTR SS:[EBP-0x20]
$+193    007E6423   ^ 75 DB                     JNZ SHORT PathOfEx.007E6400
$+195    007E6425     8B7D D8                   MOV EDI,DWORD PTR SS:[EBP-0x28]
$+198    007E6428     C645 FC 0A                MOV BYTE PTR SS:[EBP-0x4],0xA
$+19C    007E642C     EB 04                     JMP SHORT PathOfEx.007E6432
$+19E    007E642E     C645 FC 0D                MOV BYTE PTR SS:[EBP-0x4],0xD
$+1A2    007E6432     8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
$+1A5    007E6435     85C0                      TEST EAX,EAX
$+1A7    007E6437     74 18                     JE SHORT PathOfEx.007E6451
$+1A9    007E6439     50                        PUSH EAX
$+1AA    007E643A     51                        PUSH ECX
$+1AB    007E643B     E8 C01BF3FF               CALL PathOfEx.00718000
$+1B0    007E6440     83C4 04                   ADD ESP,0x4
$+1B3    007E6443     8BC8                      MOV ECX,EAX
$+1B5    007E6445     E8 4617F3FF               CALL PathOfEx.00717B90
$+1BA    007E644A     C745 D0 00000000          MOV DWORD PTR SS:[EBP-0x30],0x0
$+1C1    007E6451     FF45 EC                   INC DWORD PTR SS:[EBP-0x14]
$+1C4    007E6454   ^ E9 8FFEFFFF               JMP PathOfEx.007E62E8
$+1C9    007E6459     8B7D 08                   MOV EDI,DWORD PTR SS:[EBP+0x8]
$+1CC    007E645C     8B55 D0                   MOV EDX,DWORD PTR SS:[EBP-0x30]
$+1CF    007E645F     8917                      MOV DWORD PTR DS:[EDI],EDX
$+1D1    007E6461     85D2                      TEST EDX,EDX
$+1D3    007E6463     74 14                     JE SHORT PathOfEx.007E6479
$+1D5    007E6465     8D4A 38                   LEA ECX,DWORD PTR DS:[EDX+0x38]
$+1D8    007E6468     B8 01000000               MOV EAX,0x1
$+1DD    007E646D     F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX],EAX
$+1E1    007E6471     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
$+1E6    007E6476     8942 34                   MOV DWORD PTR DS:[EDX+0x34],EAX
$+1E9    007E6479     895F 04                   MOV DWORD PTR DS:[EDI+0x4],EBX
$+1EC    007E647C     C745 E8 01000000          MOV DWORD PTR SS:[EBP-0x18],0x1
$+1F3    007E6483     C645 FC 0B                MOV BYTE PTR SS:[EBP-0x4],0xB
$+1F7    007E6487     85D2                      TEST EDX,EDX
$+1F9    007E6489     74 11                     JE SHORT PathOfEx.007E649C
$+1FB    007E648B     52                        PUSH EDX
$+1FC    007E648C     51                        PUSH ECX
$+1FD    007E648D     E8 6E1BF3FF               CALL PathOfEx.00718000
$+202    007E6492     83C4 04                   ADD ESP,0x4
$+205    007E6495     8BC8                      MOV ECX,EAX
$+207    007E6497     E8 F416F3FF               CALL PathOfEx.00717B90
$+20C    007E649C     C745 FC 0C000000          MOV DWORD PTR SS:[EBP-0x4],0xC
$+213    007E64A3     EB 3A                     JMP SHORT PathOfEx.007E64DF
$+215    007E64A5     C645 FC 09                MOV BYTE PTR SS:[EBP-0x4],0x9
$+219    007E64A9     8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
$+21C    007E64AC     85C0                      TEST EAX,EAX
$+21E    007E64AE     74 11                     JE SHORT PathOfEx.007E64C1
$+220    007E64B0     50                        PUSH EAX
$+221    007E64B1     51                        PUSH ECX
$+222    007E64B2     E8 491BF3FF               CALL PathOfEx.00718000
$+227    007E64B7     83C4 04                   ADD ESP,0x4
$+22A    007E64BA     8BC8                      MOV ECX,EAX
$+22C    007E64BC     E8 CF16F3FF               CALL PathOfEx.00717B90
$+231    007E64C1     8B7D 08                   MOV EDI,DWORD PTR SS:[EBP+0x8]
$+234    007E64C4     C745 E8 01000000          MOV DWORD PTR SS:[EBP-0x18],0x1
$+23B    007E64CB     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
$+241    007E64D1     C747 04 00000000          MOV DWORD PTR DS:[EDI+0x4],0x0
$+248    007E64D8     C745 FC 0E000000          MOV DWORD PTR SS:[EBP-0x4],0xE
$+24F    007E64DF     85F6                      TEST ESI,ESI
$+251    007E64E1     74 11                     JE SHORT PathOfEx.007E64F4
$+253    007E64E3     56                        PUSH ESI
$+254    007E64E4     51                        PUSH ECX
$+255    007E64E5     E8 161BF3FF               CALL PathOfEx.00718000
$+25A    007E64EA     83C4 04                   ADD ESP,0x4
$+25D    007E64ED     8BC8                      MOV ECX,EAX
$+25F    007E64EF     E8 9C16F3FF               CALL PathOfEx.00717B90
$+264    007E64F4     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+267    007E64F7     8BC7                      MOV EAX,EDI
$+269    007E64F9     5F                        POP EDI
$+26A    007E64FA     5E                        POP ESI
$+26B    007E64FB     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+272    007E6502     5B                        POP EBX
$+273    007E6503     8BE5                      MOV ESP,EBP
$+275    007E6505     5D                        POP EBP
$+276    007E6506     C2 0C00                   RETN 0xC
$+279    007E6509     CC                        INT3
*/
/**/
//////////////////////////////////////////////////////////////////////////
//*****************gj 3.0

//tag_根据ID判断任务状态
/*
$-F1     006E678F     CC                       INT3
$-F0     006E6790     55                       PUSH EBP
$-EF     006E6791     8BEC                     MOV EBP,ESP
$-ED     006E6793     6A FF                    PUSH -0x1
$-EB     006E6795     68 59D6D500              PUSH PathOfEx.00D5D659
$-E6     006E679A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$-E0     006E67A0     50                       PUSH EAX
$-DF     006E67A1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$-D8     006E67A8     83EC 18                  SUB ESP,0x18
$-D5     006E67AB     53                       PUSH EBX
$-D4     006E67AC     8BD9                     MOV EBX,ECX
$-D2     006E67AE     56                       PUSH ESI
$-D1     006E67AF     57                       PUSH EDI
$-D0     006E67B0     8D7B 30                  LEA EDI,DWORD PTR DS:[EBX+0x30]
$-CD     006E67B3     57                       PUSH EDI
$-CC     006E67B4     897D E0                  MOV DWORD PTR SS:[EBP-0x20],EDI
$-C9     006E67B7     FF15 4071DD00            CALL DWORD PTR DS:[<&KERNEL32.EnterCriticalSection>]   ; ntdll_1.RtlEnterCriticalSection
$-C3     006E67BD     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
$-C0     006E67C0     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$-B9     006E67C7     50                       PUSH EAX
$-B8     006E67C8     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
$-B5     006E67CB     C745 F0 B48CF500         MOV DWORD PTR SS:[EBP-0x10],PathOfEx.00F58CB4          ; UNICODE "Data/Hideouts.dat"
$-AE     006E67D2     50                       PUSH EAX
$-AD     006E67D3     8BCB                     MOV ECX,EBX
$-AB     006E67D5     E8 867FF2FF              CALL PathOfEx.0060E760
$-A6     006E67DA     8B75 E8                  MOV ESI,DWORD PTR SS:[EBP-0x18]
$-A3     006E67DD     3B73 04                  CMP ESI,DWORD PTR DS:[EBX+0x4]
$-A0     006E67E0     74 11                    JE SHORT PathOfEx.006E67F3
$-9E     006E67E2     FF76 0C                  PUSH DWORD PTR DS:[ESI+0xC]
$-9B     006E67E5     8BCB                     MOV ECX,EBX
$-99     006E67E7     E8 8478F2FF              CALL PathOfEx.0060E070
$-94     006E67EC     8B76 0C                  MOV ESI,DWORD PTR DS:[ESI+0xC]
$-91     006E67EF     85F6                     TEST ESI,ESI
$-8F     006E67F1     75 4C                    JNZ SHORT PathOfEx.006E683F
$-8D     006E67F3     6A 60                    PUSH 0x60
$-8B     006E67F5     E8 EB5C6000              CALL PathOfEx.00CEC4E5
$-86     006E67FA     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
$-83     006E67FD     83C4 04                  ADD ESP,0x4
$-80     006E6800     8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
$-7D     006E6803     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$-79     006E6807     85C0                     TEST EAX,EAX
$-77     006E6809     74 0E                    JE SHORT PathOfEx.006E6819
$-75     006E680B     83EC 10                  SUB ESP,0x10
$-72     006E680E     8BC8                     MOV ECX,EAX
$-70     006E6810     E8 AB010000              CALL PathOfEx.006E69C0
$-6B     006E6815     8BF0                     MOV ESI,EAX
$-69     006E6817     EB 02                    JMP SHORT PathOfEx.006E681B
$-67     006E6819     33F6                     XOR ESI,ESI
$-65     006E681B     8D46 14                  LEA EAX,DWORD PTR DS:[ESI+0x14]
$-62     006E681E     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
$-5E     006E6822     8378 14 08               CMP DWORD PTR DS:[EAX+0x14],0x8
$-5A     006E6826     72 02                    JB SHORT PathOfEx.006E682A
$-58     006E6828     8B00                     MOV EAX,DWORD PTR DS:[EAX]
$-56     006E682A     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
$-53     006E682D     8BCB                     MOV ECX,EBX
$-51     006E682F     8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-4E     006E6832     8975 E8                  MOV DWORD PTR SS:[EBP-0x18],ESI
$-4B     006E6835     50                       PUSH EAX
$-4A     006E6836     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
$-47     006E6839     50                       PUSH EAX
$-46     006E683A     E8 6184F2FF              CALL PathOfEx.0060ECA0
$-41     006E683F     8D56 38                  LEA EDX,DWORD PTR DS:[ESI+0x38]
$-3E     006E6842     B9 01000000              MOV ECX,0x1
$-39     006E6847     F0:0FC10A                LOCK XADD DWORD PTR DS:[EDX],ECX
$-35     006E684B     8B0D B0AA0501            MOV ECX,DWORD PTR DS:[0x105AAB0]
$-2F     006E6851     894E 34                  MOV DWORD PTR DS:[ESI+0x34],ECX
$-2C     006E6854     85FF                     TEST EDI,EDI
$-2A     006E6856     74 07                    JE SHORT PathOfEx.006E685F
$-28     006E6858     57                       PUSH EDI
$-27     006E6859     FF15 4471DD00            CALL DWORD PTR DS:[<&KERNEL32.LeaveCriticalSection>]   ; ntdll_1.RtlLeaveCriticalSection
$-21     006E685F     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-1E     006E6862     8BC6                     MOV EAX,ESI
$-1C     006E6864     5F                       POP EDI
$-1B     006E6865     5E                       POP ESI
$-1A     006E6866     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-13     006E686D     5B                       POP EBX
$-12     006E686E     8BE5                     MOV ESP,EBP
$-10     006E6870     5D                       POP EBP
$-F      006E6871     C2 1000                  RETN 0x10
$-C      006E6874     CC                       INT3
$-B      006E6875     CC                       INT3
$-A      006E6876     CC                       INT3
$-9      006E6877     CC                       INT3
$-8      006E6878     CC                       INT3
$-7      006E6879     CC                       INT3
$-6      006E687A     CC                       INT3
$-5      006E687B     CC                       INT3
$-4      006E687C     CC                       INT3
$-3      006E687D     CC                       INT3
$-2      006E687E     CC                       INT3
$-1      006E687F     CC                       INT3
$ ==>    006E6880     53                       PUSH EBX                                               ; tag_根据ID判断任务状态
$+1      006E6881     56                       PUSH ESI
$+2      006E6882     57                       PUSH EDI
$+3      006E6883     8BFA                     MOV EDI,EDX
$+5      006E6885     8BC1                     MOV EAX,ECX
$+7      006E6887     3BC7                     CMP EAX,EDI
$+9      006E6889     74 2E                    JE SHORT PathOfEx.006E68B9
$+B      006E688B     8B5C24 10                MOV EBX,DWORD PTR SS:[ESP+0x10]
$+F      006E688F     90                       NOP
$+10     006E6890     8B30                     MOV ESI,DWORD PTR DS:[EAX]
$+12     006E6892     81FE E9050000            CMP ESI,0x5E9
$+18     006E6898     74 25                    JE SHORT PathOfEx.006E68BF
$+1A     006E689A     4E                       DEC ESI
$+1B     006E689B     BA 01000000              MOV EDX,0x1
$+20     006E68A0     8BCE                     MOV ECX,ESI
$+22     006E68A2     C1EE 03                  SHR ESI,0x3
$+25     006E68A5     83E1 07                  AND ECX,0x7
$+28     006E68A8     D3E2                     SHL EDX,CL
$+2A     006E68AA     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
$+2C     006E68AC     84540E 4C                TEST BYTE PTR DS:[ESI+ECX+0x4C],DL
$+30     006E68B0     74 0D                    JE SHORT PathOfEx.006E68BF
$+32     006E68B2     83C0 04                  ADD EAX,0x4
$+35     006E68B5     3BC7                     CMP EAX,EDI
$+37     006E68B7   ^ 75 D7                    JNZ SHORT PathOfEx.006E6890
$+39     006E68B9     5F                       POP EDI
$+3A     006E68BA     5E                       POP ESI
$+3B     006E68BB     B0 01                    MOV AL,0x1
$+3D     006E68BD     5B                       POP EBX
$+3E     006E68BE     C3                       RETN
$+3F     006E68BF     5F                       POP EDI
$+40     006E68C0     5E                       POP ESI
$+41     006E68C1     32C0                     XOR AL,AL
$+43     006E68C3     5B                       POP EBX
$+44     006E68C4     C3                       RETN
$+45     006E68C5     CC                       INT3
$+46     006E68C6     CC                       INT3
$+47     006E68C7     CC                       INT3
$+48     006E68C8     CC                       INT3
$+49     006E68C9     CC                       INT3
$+4A     006E68CA     CC                       INT3
$+4B     006E68CB     CC                       INT3
$+4C     006E68CC     CC                       INT3
$+4D     006E68CD     CC                       INT3
$+4E     006E68CE     CC                       INT3
$+4F     006E68CF     CC                       INT3
$+50     006E68D0     53                       PUSH EBX
$+51     006E68D1     56                       PUSH ESI
$+52     006E68D2     57                       PUSH EDI
$+53     006E68D3     8BFA                     MOV EDI,EDX
$+55     006E68D5     8BC1                     MOV EAX,ECX
$+57     006E68D7     3BC7                     CMP EAX,EDI
$+59     006E68D9     74 2E                    JE SHORT PathOfEx.006E6909
$+5B     006E68DB     8B5C24 10                MOV EBX,DWORD PTR SS:[ESP+0x10]
$+5F     006E68DF     90                       NOP
$+60     006E68E0     8B30                     MOV ESI,DWORD PTR DS:[EAX]
$+62     006E68E2     81FE E9050000            CMP ESI,0x5E9
$+68     006E68E8     74 18                    JE SHORT PathOfEx.006E6902
$+6A     006E68EA     4E                       DEC ESI
$+6B     006E68EB     BA 01000000              MOV EDX,0x1
$+70     006E68F0     8BCE                     MOV ECX,ESI
$+72     006E68F2     C1EE 03                  SHR ESI,0x3
$+75     006E68F5     83E1 07                  AND ECX,0x7
$+78     006E68F8     D3E2                     SHL EDX,CL
$+7A     006E68FA     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
$+7C     006E68FC     84540E 4C                TEST BYTE PTR DS:[ESI+ECX+0x4C],DL
$+80     006E6900     75 0D                    JNZ SHORT PathOfEx.006E690F
$+82     006E6902     83C0 04                  ADD EAX,0x4
$+85     006E6905     3BC7                     CMP EAX,EDI
$+87     006E6907   ^ 75 D7                    JNZ SHORT PathOfEx.006E68E0
$+89     006E6909     5F                       POP EDI
$+8A     006E690A     5E                       POP ESI
$+8B     006E690B     B0 01                    MOV AL,0x1
$+8D     006E690D     5B                       POP EBX
$+8E     006E690E     C3                       RETN
$+8F     006E690F     5F                       POP EDI
$+90     006E6910     5E                       POP ESI
$+91     006E6911     32C0                     XOR AL,AL
$+93     006E6913     5B                       POP EBX
$+94     006E6914     C3                       RETN
$+95     006E6915     CC                       INT3
$+96     006E6916     CC                       INT3
$+97     006E6917     CC                       INT3
$+98     006E6918     CC                       INT3
$+99     006E6919     CC                       INT3
$+9A     006E691A     CC                       INT3
$+9B     006E691B     CC                       INT3
$+9C     006E691C     CC                       INT3
$+9D     006E691D     CC                       INT3
$+9E     006E691E     CC                       INT3
$+9F     006E691F     CC                       INT3
$+A0     006E6920     53                       PUSH EBX
$+A1     006E6921     56                       PUSH ESI
$+A2     006E6922     57                       PUSH EDI
$+A3     006E6923     8BFA                     MOV EDI,EDX
$+A5     006E6925     8BC1                     MOV EAX,ECX
$+A7     006E6927     3BC7                     CMP EAX,EDI
$+A9     006E6929     74 31                    JE SHORT PathOfEx.006E695C
$+AB     006E692B     8B5C24 10                MOV EBX,DWORD PTR SS:[ESP+0x10]
$+AF     006E692F     90                       NOP
$+B0     006E6930     8B30                     MOV ESI,DWORD PTR DS:[EAX]
$+B2     006E6932     81FE 61010000            CMP ESI,0x161
$+B8     006E6938     74 28                    JE SHORT PathOfEx.006E6962
$+BA     006E693A     4E                       DEC ESI
$+BB     006E693B     BA 01000000              MOV EDX,0x1
$+C0     006E6940     8BCE                     MOV ECX,ESI
$+C2     006E6942     C1EE 03                  SHR ESI,0x3
$+C5     006E6945     83E1 07                  AND ECX,0x7
$+C8     006E6948     D3E2                     SHL EDX,CL
$+CA     006E694A     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
$+CC     006E694C     84940E 0C010000          TEST BYTE PTR DS:[ESI+ECX+0x10C],DL
$+D3     006E6953     74 0D                    JE SHORT PathOfEx.006E6962
$+D5     006E6955     83C0 04                  ADD EAX,0x4
$+D8     006E6958     3BC7                     CMP EAX,EDI
$+DA     006E695A   ^ 75 D4                    JNZ SHORT PathOfEx.006E6930
$+DC     006E695C     5F                       POP EDI
$+DD     006E695D     5E                       POP ESI
$+DE     006E695E     B0 01                    MOV AL,0x1
$+E0     006E6960     5B                       POP EBX
$+E1     006E6961     C3                       RETN
$+E2     006E6962     5F                       POP EDI
$+E3     006E6963     5E                       POP ESI
$+E4     006E6964     32C0                     XOR AL,AL
$+E6     006E6966     5B                       POP EBX
$+E7     006E6967     C3                       RETN
$+E8     006E6968     CC                       INT3
$+E9     006E6969     CC                       INT3
$+EA     006E696A     CC                       INT3
$+EB     006E696B     CC                       INT3
$+EC     006E696C     CC                       INT3
$+ED     006E696D     CC                       INT3
$+EE     006E696E     CC                       INT3
$+EF     006E696F     CC                       INT3
$+F0     006E6970     53                       PUSH EBX
$+F1     006E6971     56                       PUSH ESI
$+F2     006E6972     57                       PUSH EDI
$+F3     006E6973     8BFA                     MOV EDI,EDX
$+F5     006E6975     8BC1                     MOV EAX,ECX
$+F7     006E6977     3BC7                     CMP EAX,EDI
$+F9     006E6979     74 31                    JE SHORT PathOfEx.006E69AC
$+FB     006E697B     8B5C24 10                MOV EBX,DWORD PTR SS:[ESP+0x10]
$+FF     006E697F     90                       NOP
$+100    006E6980     8B30                     MOV ESI,DWORD PTR DS:[EAX]
$+102    006E6982     81FE 61010000            CMP ESI,0x161
$+108    006E6988     74 1B                    JE SHORT PathOfEx.006E69A5
$+10A    006E698A     4E                       DEC ESI
$+10B    006E698B     BA 01000000              MOV EDX,0x1
$+110    006E6990     8BCE                     MOV ECX,ESI
$+112    006E6992     C1EE 03                  SHR ESI,0x3
$+115    006E6995     83E1 07                  AND ECX,0x7
$+118    006E6998     D3E2                     SHL EDX,CL
$+11A    006E699A     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
$+11C    006E699C     84940E 0C010000          TEST BYTE PTR DS:[ESI+ECX+0x10C],DL
$+123    006E69A3     75 0D                    JNZ SHORT PathOfEx.006E69B2
$+125    006E69A5     83C0 04                  ADD EAX,0x4
$+128    006E69A8     3BC7                     CMP EAX,EDI
$+12A    006E69AA   ^ 75 D4                    JNZ SHORT PathOfEx.006E6980
$+12C    006E69AC     5F                       POP EDI
$+12D    006E69AD     5E                       POP ESI
$+12E    006E69AE     B0 01                    MOV AL,0x1
$+130    006E69B0     5B                       POP EBX
$+131    006E69B1     C3                       RETN
$+132    006E69B2     5F                       POP EDI
$+133    006E69B3     5E                       POP ESI
$+134    006E69B4     32C0                     XOR AL,AL
$+136    006E69B6     5B                       POP EBX
$+137    006E69B7     C3                       RETN
$+138    006E69B8     CC                       INT3
$+139    006E69B9     CC                       INT3
$+13A    006E69BA     CC                       INT3
$+13B    006E69BB     CC                       INT3
$+13C    006E69BC     CC                       INT3
$+13D    006E69BD     CC                       INT3
$+13E    006E69BE     CC                       INT3
$+13F    006E69BF     CC                       INT3
$+140    006E69C0     55                       PUSH EBP
$+141    006E69C1     8BEC                     MOV EBP,ESP
$+143    006E69C3     6A FF                    PUSH -0x1
$+145    006E69C5     68 E1E9D500              PUSH PathOfEx.00D5E9E1
$+14A    006E69CA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+150    006E69D0     50                       PUSH EAX
$+151    006E69D1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+158    006E69D8     83EC 10                  SUB ESP,0x10
$+15B    006E69DB     56                       PUSH ESI
$+15C    006E69DC     6A 01                    PUSH 0x1
$+15E    006E69DE     6A 04                    PUSH 0x4
$+160    006E69E0     8BF1                     MOV ESI,ECX
$+162    006E69E2     6A 78                    PUSH 0x78
$+164    006E69E4     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
$+167    006E69E7     6A 1C                    PUSH 0x1C
$+169    006E69E9     C745 EC 5EA50A01         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.010AA55E
$+170    006E69F0     E8 F05A6000              CALL PathOfEx.00CEC4E5
$+175    006E69F5     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
$+178    006E69F8     83C4 04                  ADD ESP,0x4
$+17B    006E69FB     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
$+17E    006E69FE     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+185    006E6A05     85C0                     TEST EAX,EAX
$+187    006E6A07     74 19                    JE SHORT PathOfEx.006E6A22
$+189    006E6A09     8D4D F0                  LEA ECX,DWORD PTR SS:[EBP-0x10]
$+18C    006E6A0C     C745 F0 5EA50A01         MOV DWORD PTR SS:[EBP-0x10],PathOfEx.010AA55E
$+193    006E6A13     51                       PUSH ECX
$+194    006E6A14     68 B48CF500              PUSH PathOfEx.00F58CB4                                 ; UNICODE "Data/Hideouts.dat"
$+199    006E6A19     8BC8                     MOV ECX,EAX
$+19B    006E6A1B     E8 C004C4FF              CALL PathOfEx.00326EE0
$+1A0    006E6A20     EB 02                    JMP SHORT PathOfEx.006E6A24
$+1A2    006E6A22     33C0                     XOR EAX,EAX
$+1A4    006E6A24     50                       PUSH EAX
$+1A5    006E6A25     68 B0353200              PUSH PathOfEx.003235B0
$+1AA    006E6A2A     68 B0353200              PUSH PathOfEx.003235B0
$+1AF    006E6A2F     68 F0603200              PUSH PathOfEx.003260F0
$+1B4    006E6A34     68 106B6E00              PUSH PathOfEx.006E6B10
$+1B9    006E6A39     68 806A6E00              PUSH PathOfEx.006E6A80
$+1BE    006E6A3E     83EC 1C                  SUB ESP,0x1C
$+1C1    006E6A41     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+1C8    006E6A48     8BCC                     MOV ECX,ESP
$+1CA    006E6A4A     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
$+1CD    006E6A4D     50                       PUSH EAX
$+1CE    006E6A4E     68 B48CF500              PUSH PathOfEx.00F58CB4                                 ; UNICODE "Data/Hideouts.dat"
$+1D3    006E6A53     E8 8804C4FF              CALL PathOfEx.00326EE0
$+1D8    006E6A58     8BCE                     MOV ECX,ESI
$+1DA    006E6A5A     E8 0187F2FF              CALL PathOfEx.0060F160
$+1DF    006E6A5F     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+1E2    006E6A62     8BC6                     MOV EAX,ESI
$+1E4    006E6A64     C706 0C8DF500            MOV DWORD PTR DS:[ESI],PathOfEx.00F58D0C
$+1EA    006E6A6A     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+1F1    006E6A71     5E                       POP ESI
$+1F2    006E6A72     8BE5                     MOV ESP,EBP
$+1F4    006E6A74     5D                       POP EBP
$+1F5    006E6A75     C2 1000                  RETN 0x10
$+1F8    006E6A78     CC                       INT3
*/
//tag_根据ID判断任务状态

//tag_根据ID判断任务状态up
/*
$-105    006F643F     CC                       INT3
$-104    006F6440     55                       PUSH EBP
$-103    006F6441     8BEC                     MOV EBP,ESP
$-101    006F6443     6A FF                    PUSH -0x1
$-FF     006F6445     68 A6D4D500              PUSH PathOfEx.00D5D4A6
$-FA     006F644A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$-F4     006F6450     50                       PUSH EAX
$-F3     006F6451     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$-EC     006F6458     83EC 08                  SUB ESP,0x8
$-E9     006F645B     53                       PUSH EBX
$-E8     006F645C     56                       PUSH ESI
$-E7     006F645D     57                       PUSH EDI
$-E6     006F645E     8BF2                     MOV ESI,EDX
$-E4     006F6460     8BF9                     MOV EDI,ECX
$-E2     006F6462     33C9                     XOR ECX,ECX
$-E0     006F6464     894D F0                  MOV DWORD PTR SS:[EBP-0x10],ECX
$-DD     006F6467     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
$-DA     006F646A     3948 14                  CMP DWORD PTR DS:[EAX+0x14],ECX
$-D7     006F646D     74 21                    JE SHORT PathOfEx.006F6490
$-D5     006F646F     8B40 0C                  MOV EAX,DWORD PTR DS:[EAX+0xC]
$-D2     006F6472     85C0                     TEST EAX,EAX
$-D0     006F6474     74 1A                    JE SHORT PathOfEx.006F6490
$-CE     006F6476     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
$-CB     006F6479     51                       PUSH ECX
$-CA     006F647A     8BC8                     MOV ECX,EAX
$-C8     006F647C     E8 2F2CC7FF              CALL PathOfEx.003690B0
$-C3     006F6481     B9 01000000              MOV ECX,0x1
$-BE     006F6486     8378 04 00               CMP DWORD PTR DS:[EAX+0x4],0x0
$-BA     006F648A     74 04                    JE SHORT PathOfEx.006F6490
$-B8     006F648C     32DB                     XOR BL,BL
$-B6     006F648E     EB 02                    JMP SHORT PathOfEx.006F6492
$-B4     006F6490     B3 01                    MOV BL,0x1
$-B2     006F6492     F6C1 01                  TEST CL,0x1
$-AF     006F6495     74 2D                    JE SHORT PathOfEx.006F64C4
$-AD     006F6497     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$-A6     006F649E     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
$-A3     006F64A1     85C0                     TEST EAX,EAX
$-A1     006F64A3     74 18                    JE SHORT PathOfEx.006F64BD
$-9F     006F64A5     50                       PUSH EAX
$-9E     006F64A6     51                       PUSH ECX
$-9D     006F64A7     E8 A47EF1FF              CALL PathOfEx.0060E350
$-98     006F64AC     83C4 04                  ADD ESP,0x4
$-95     006F64AF     8BC8                     MOV ECX,EAX
$-93     006F64B1     E8 6A7AF1FF              CALL PathOfEx.0060DF20
$-8E     006F64B6     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
$-87     006F64BD     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$-80     006F64C4     84DB                     TEST BL,BL
$-7E     006F64C6     74 13                    JE SHORT PathOfEx.006F64DB
$-7C     006F64C8     33C0                     XOR EAX,EAX
$-7A     006F64CA     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-77     006F64CD     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-70     006F64D4     5F                       POP EDI
$-6F     006F64D5     5E                       POP ESI
$-6E     006F64D6     5B                       POP EBX
$-6D     006F64D7     8BE5                     MOV ESP,EBP
$-6B     006F64D9     5D                       POP EBP
$-6A     006F64DA     C3                       RETN
$-69     006F64DB     8BCE                     MOV ECX,ESI
$-67     006F64DD     E8 7EFDFFFF              CALL PathOfEx.006F6260
$-62     006F64E2     50                       PUSH EAX
$-61     006F64E3     8BCF                     MOV ECX,EDI
$-5F     006F64E5     E8 965CD9FF              CALL PathOfEx.0048C180
$-5A     006F64EA     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$-57     006F64ED     5F                       POP EDI
$-56     006F64EE     5E                       POP ESI
$-55     006F64EF     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$-4E     006F64F6     5B                       POP EBX
$-4D     006F64F7     8BE5                     MOV ESP,EBP
$-4B     006F64F9     5D                       POP EBP
$-4A     006F64FA     C3                       RETN
$-49     006F64FB     CC                       INT3
$-48     006F64FC     CC                       INT3
$-47     006F64FD     CC                       INT3
$-46     006F64FE     CC                       INT3
$-45     006F64FF     CC                       INT3
$-44     006F6500     55                       PUSH EBP
$-43     006F6501     8BEC                     MOV EBP,ESP
$-41     006F6503     6A FF                    PUSH -0x1
$-3F     006F6505     68 F29DD900              PUSH PathOfEx.00D99DF2
$-3A     006F650A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$-34     006F6510     50                       PUSH EAX
$-33     006F6511     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$-2C     006F6518     83EC 40                  SUB ESP,0x40
$-29     006F651B     33C0                     XOR EAX,EAX
$-27     006F651D     894D E8                  MOV DWORD PTR SS:[EBP-0x18],ECX
$-24     006F6520     8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
$-21     006F6523     53                       PUSH EBX
$-20     006F6524     8B5D 0C                  MOV EBX,DWORD PTR SS:[EBP+0xC]
$-1D     006F6527     56                       PUSH ESI
$-1C     006F6528     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
$-19     006F652B     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
$-16     006F652E     8D45 0C                  LEA EAX,DWORD PTR SS:[EBP+0xC]
$-13     006F6531     57                       PUSH EDI
$-12     006F6532     50                       PUSH EAX
$-11     006F6533     8B76 04                  MOV ESI,DWORD PTR DS:[ESI+0x4]
$-E      006F6536     895D 0C                  MOV DWORD PTR SS:[EBP+0xC],EBX
$-B      006F6539     8B46 14                  MOV EAX,DWORD PTR DS:[ESI+0x14]
$-8      006F653C     8BC8                     MOV ECX,EAX
$-6      006F653E     8B56 10                  MOV EDX,DWORD PTR DS:[ESI+0x10]
$-3      006F6541     8D1490                   LEA EDX,DWORD PTR DS:[EAX+EDX*4]
$ ==>    006F6544     E8 3703FFFF              CALL PathOfEx.006E6880                                 ; call tag_根据ID判断任务状态
$+5      006F6549     83C4 04                  ADD ESP,0x4
$+8      006F654C     84C0                     TEST AL,AL
$+A      006F654E     0F84 EF030000            JE PathOfEx.006F6943
$+10     006F6554     8B56 18                  MOV EDX,DWORD PTR DS:[ESI+0x18]
$+13     006F6557     8D45 0C                  LEA EAX,DWORD PTR SS:[EBP+0xC]
$+16     006F655A     50                       PUSH EAX
$+17     006F655B     8B46 1C                  MOV EAX,DWORD PTR DS:[ESI+0x1C]
$+1A     006F655E     8BC8                     MOV ECX,EAX
$+1C     006F6560     895D 0C                  MOV DWORD PTR SS:[EBP+0xC],EBX
$+1F     006F6563     8D1490                   LEA EDX,DWORD PTR DS:[EAX+EDX*4]
$+22     006F6566     E8 6503FFFF              CALL PathOfEx.006E68D0
$+27     006F656B     83C4 04                  ADD ESP,0x4
$+2A     006F656E     84C0                     TEST AL,AL
$+2C     006F6570     0F84 CD030000            JE PathOfEx.006F6943
$+32     006F6576     8B56 56                  MOV EDX,DWORD PTR DS:[ESI+0x56]
$+35     006F6579     8D45 0C                  LEA EAX,DWORD PTR SS:[EBP+0xC]
$+38     006F657C     50                       PUSH EAX
$+39     006F657D     8B46 5A                  MOV EAX,DWORD PTR DS:[ESI+0x5A]
$+3C     006F6580     8BC8                     MOV ECX,EAX
$+3E     006F6582     895D 0C                  MOV DWORD PTR SS:[EBP+0xC],EBX
$+41     006F6585     8D1490                   LEA EDX,DWORD PTR DS:[EAX+EDX*4]
$+44     006F6588     E8 9303FFFF              CALL PathOfEx.006E6920
$+49     006F658D     83C4 04                  ADD ESP,0x4
$+4C     006F6590     84C0                     TEST AL,AL
$+4E     006F6592     0F84 AB030000            JE PathOfEx.006F6943
$+54     006F6598     8B56 6A                  MOV EDX,DWORD PTR DS:[ESI+0x6A]
$+57     006F659B     8D45 0C                  LEA EAX,DWORD PTR SS:[EBP+0xC]
$+5A     006F659E     50                       PUSH EAX
$+5B     006F659F     8B46 6E                  MOV EAX,DWORD PTR DS:[ESI+0x6E]
$+5E     006F65A2     8BC8                     MOV ECX,EAX
$+60     006F65A4     895D 0C                  MOV DWORD PTR SS:[EBP+0xC],EBX
$+63     006F65A7     8D1490                   LEA EDX,DWORD PTR DS:[EAX+EDX*4]
$+66     006F65AA     E8 C103FFFF              CALL PathOfEx.006E6970
$+6B     006F65AF     83C4 04                  ADD ESP,0x4
$+6E     006F65B2     84C0                     TEST AL,AL
$+70     006F65B4     0F84 89030000            JE PathOfEx.006F6943
$+76     006F65BA     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
$+79     006F65BD     8BCE                     MOV ECX,ESI
$+7B     006F65BF     50                       PUSH EAX
$+7C     006F65C0     E8 3BF9FFFF              CALL PathOfEx.006F5F00
$+81     006F65C5     8B70 04                  MOV ESI,DWORD PTR DS:[EAX+0x4]
$+84     006F65C8     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+8B     006F65CF     8B45 B8                  MOV EAX,DWORD PTR SS:[EBP-0x48]
$+8E     006F65D2     85C0                     TEST EAX,EAX
$+90     006F65D4     74 11                    JE SHORT PathOfEx.006F65E7
$+92     006F65D6     50                       PUSH EAX
$+93     006F65D7     51                       PUSH ECX
$+94     006F65D8     E8 737DF1FF              CALL PathOfEx.0060E350
$+99     006F65DD     83C4 04                  ADD ESP,0x4
$+9C     006F65E0     8BC8                     MOV ECX,EAX
$+9E     006F65E2     E8 3979F1FF              CALL PathOfEx.0060DF20
$+A3     006F65E7     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+AA     006F65EE     85F6                     TEST ESI,ESI
$+AC     006F65F0     0F84 D1000000            JE PathOfEx.006F66C7
$+B2     006F65F6     8B4D 14                  MOV ECX,DWORD PTR SS:[EBP+0x14]
$+B5     006F65F9     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
$+B8     006F65FC     C645 0F 00               MOV BYTE PTR SS:[EBP+0xF],0x0
$+BC     006F6600     FF71 08                  PUSH DWORD PTR DS:[ECX+0x8]
$+BF     006F6603     50                       PUSH EAX
$+C0     006F6604     E8 97310000              CALL PathOfEx.006F97A0
$+C5     006F6609     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
$+CC     006F6610     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
$+CF     006F6613     8B70 2C                  MOV ESI,DWORD PTR DS:[EAX+0x2C]
$+D2     006F6616     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+D4     006F6618     8945 14                  MOV DWORD PTR SS:[EBP+0x14],EAX
$+D7     006F661B     3BC6                     CMP EAX,ESI
$+D9     006F661D     0F84 BB010000            JE PathOfEx.006F67DE
$+DF     006F6623     807D 0F 00               CMP BYTE PTR SS:[EBP+0xF],0x0
$+E3     006F6627     75 68                    JNZ SHORT PathOfEx.006F6691
$+E5     006F6629     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
$+E8     006F662C     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+EA     006F662E     E8 0DEFC7FF              CALL PathOfEx.00375540
$+EF     006F6633     8BF8                     MOV EDI,EAX
$+F1     006F6635     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
$+F4     006F6638     50                       PUSH EAX
$+F5     006F6639     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
$+F8     006F663C     8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
$+FB     006F663F     E8 BCF8FFFF              CALL PathOfEx.006F5F00
$+100    006F6644     8B4F 08                  MOV ECX,DWORD PTR DS:[EDI+0x8]
$+103    006F6647     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+106    006F664A     3B41 4C                  CMP EAX,DWORD PTR DS:[ECX+0x4C]
$+109    006F664D     0F9445 0F                SETE BYTE PTR SS:[EBP+0xF]
$+10D    006F6651     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
$+111    006F6655     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
$+114    006F6658     85C0                     TEST EAX,EAX
$+116    006F665A     74 18                    JE SHORT PathOfEx.006F6674
$+118    006F665C     50                       PUSH EAX
$+119    006F665D     51                       PUSH ECX
$+11A    006F665E     E8 ED7CF1FF              CALL PathOfEx.0060E350
$+11F    006F6663     83C4 04                  ADD ESP,0x4
$+122    006F6666     8BC8                     MOV ECX,EAX
$+124    006F6668     E8 B378F1FF              CALL PathOfEx.0060DF20
$+129    006F666D     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
$+130    006F6674     8D4D 14                  LEA ECX,DWORD PTR SS:[EBP+0x14]
$+133    006F6677     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
$+137    006F667B     E8 2004C4FF              CALL PathOfEx.00336AA0
$+13C    006F6680     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
$+13F    006F6683     3BC6                     CMP EAX,ESI
$+141    006F6685   ^ 75 9C                    JNZ SHORT PathOfEx.006F6623
$+143    006F6687     807D 0F 00               CMP BYTE PTR SS:[EBP+0xF],0x0
$+147    006F668B     0F84 4D010000            JE PathOfEx.006F67DE
$+14D    006F6691     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
$+154    006F6698     8B7D F0                  MOV EDI,DWORD PTR SS:[EBP-0x10]
$+157    006F669B     85FF                     TEST EDI,EDI
$+159    006F669D     74 21                    JE SHORT PathOfEx.006F66C0
$+15B    006F669F     83CE FF                  OR ESI,0xFFFFFFFF
$+15E    006F66A2     8BC6                     MOV EAX,ESI
$+160    006F66A4     F0:0FC147 04             LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+165    006F66A9     75 15                    JNZ SHORT PathOfEx.006F66C0
$+167    006F66AB     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+169    006F66AD     8BCF                     MOV ECX,EDI
$+16B    006F66AF     FF10                     CALL DWORD PTR DS:[EAX]
$+16D    006F66B1     F0:0FC177 08             LOCK XADD DWORD PTR DS:[EDI+0x8],ESI
$+172    006F66B6     4E                       DEC ESI
$+173    006F66B7     75 07                    JNZ SHORT PathOfEx.006F66C0
$+175    006F66B9     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+177    006F66BB     8BCF                     MOV ECX,EDI
$+179    006F66BD     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+17C    006F66C0     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+183    006F66C7     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
$+186    006F66CA     8B4D 10                  MOV ECX,DWORD PTR SS:[EBP+0x10]
$+189    006F66CD     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
$+18C    006F66D0     3B48 62                  CMP ECX,DWORD PTR DS:[EAX+0x62]
$+18F    006F66D3     0F82 6A020000            JB PathOfEx.006F6943
$+195    006F66D9     3B48 66                  CMP ECX,DWORD PTR DS:[EAX+0x66]
$+198    006F66DC     0F87 61020000            JA PathOfEx.006F6943
$+19E    006F66E2     8B50 7B                  MOV EDX,DWORD PTR DS:[EAX+0x7B]
$+1A1    006F66E5     8B48 77                  MOV ECX,DWORD PTR DS:[EAX+0x77]
$+1A4    006F66E8     8955 14                  MOV DWORD PTR SS:[EBP+0x14],EDX
$+1A7    006F66EB     8D04CA                   LEA EAX,DWORD PTR DS:[EDX+ECX*8]
$+1AA    006F66EE     3BD0                     CMP EDX,EAX
$+1AC    006F66F0     0F84 EA010000            JE PathOfEx.006F68E0
$+1B2    006F66F6     8B3A                     MOV EDI,DWORD PTR DS:[EDX]
$+1B4    006F66F8     897D B8                  MOV DWORD PTR SS:[EBP-0x48],EDI
$+1B7    006F66FB     85FF                     TEST EDI,EDI
$+1B9    006F66FD     74 14                    JE SHORT PathOfEx.006F6713
$+1BB    006F66FF     8D4F 38                  LEA ECX,DWORD PTR DS:[EDI+0x38]
$+1BE    006F6702     B8 01000000              MOV EAX,0x1
$+1C3    006F6707     F0:0FC101                LOCK XADD DWORD PTR DS:[ECX],EAX
$+1C7    006F670B     A1 B0AA0501              MOV EAX,DWORD PTR DS:[0x105AAB0]
$+1CC    006F6710     8947 34                  MOV DWORD PTR DS:[EDI+0x34],EAX
$+1CF    006F6713     8B72 04                  MOV ESI,DWORD PTR DS:[EDX+0x4]
$+1D2    006F6716     8975 BC                  MOV DWORD PTR SS:[EBP-0x44],ESI
$+1D5    006F6719     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
$+1D8    006F671C     C745 FC 05000000         MOV DWORD PTR SS:[EBP-0x4],0x5
$+1DF    006F6723     50                       PUSH EAX
$+1E0    006F6724     8BCE                     MOV ECX,ESI
$+1E2    006F6726     E8 65BED9FF              CALL PathOfEx.00492590
$+1E7    006F672B     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+1EA    006F672E     8945 0C                  MOV DWORD PTR SS:[EBP+0xC],EAX
$+1ED    006F6731     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
$+1F1    006F6735     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
$+1F4    006F6738     85C0                     TEST EAX,EAX
$+1F6    006F673A     74 18                    JE SHORT PathOfEx.006F6754
$+1F8    006F673C     50                       PUSH EAX
$+1F9    006F673D     51                       PUSH ECX
$+1FA    006F673E     E8 0D7CF1FF              CALL PathOfEx.0060E350
$+1FF    006F6743     83C4 04                  ADD ESP,0x4
$+202    006F6746     8BC8                     MOV ECX,EAX
$+204    006F6748     E8 D377F1FF              CALL PathOfEx.0060DF20
$+209    006F674D     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
$+210    006F6754     837D 0C 00               CMP DWORD PTR SS:[EBP+0xC],0x0
$+214    006F6758     0F84 38010000            JE PathOfEx.006F6896
$+21A    006F675E     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
$+21D    006F6761     8BCE                     MOV ECX,ESI
$+21F    006F6763     50                       PUSH EAX
$+220    006F6764     E8 27BED9FF              CALL PathOfEx.00492590
$+225    006F6769     8B4E 28                  MOV ECX,DWORD PTR DS:[ESI+0x28]
$+228    006F676C     894D 0C                  MOV DWORD PTR SS:[EBP+0xC],ECX
$+22B    006F676F     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+22E    006F6772     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+231    006F6775     48                       DEC EAX
$+232    006F6776     8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
$+235    006F6779     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
$+239    006F677D     8B45 D0                  MOV EAX,DWORD PTR SS:[EBP-0x30]
$+23C    006F6780     85C0                     TEST EAX,EAX
$+23E    006F6782     74 1B                    JE SHORT PathOfEx.006F679F
$+240    006F6784     50                       PUSH EAX
$+241    006F6785     51                       PUSH ECX
$+242    006F6786     E8 C57BF1FF              CALL PathOfEx.0060E350
$+247    006F678B     83C4 04                  ADD ESP,0x4
$+24A    006F678E     8BC8                     MOV ECX,EAX
$+24C    006F6790     E8 8B77F1FF              CALL PathOfEx.0060DF20
$+251    006F6795     8B4D 0C                  MOV ECX,DWORD PTR SS:[EBP+0xC]
$+254    006F6798     C745 D0 00000000         MOV DWORD PTR SS:[EBP-0x30],0x0
$+25B    006F679F     3B4D F0                  CMP ECX,DWORD PTR SS:[EBP-0x10]
$+25E    006F67A2     8BCE                     MOV ECX,ESI
$+260    006F67A4     0F85 A5000000            JNZ PathOfEx.006F684F
$+266    006F67AA     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
$+269    006F67AD     50                       PUSH EAX
$+26A    006F67AE     E8 DDBDD9FF              CALL PathOfEx.00492590
$+26F    006F67B3     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
$+272    006F67B6     8B50 18                  MOV EDX,DWORD PTR DS:[EAX+0x18]
$+275    006F67B9     81FA E9050000            CMP EDX,0x5E9
$+27B    006F67BF     74 5D                    JE SHORT PathOfEx.006F681E
$+27D    006F67C1     4A                       DEC EDX
$+27E    006F67C2     B8 01000000              MOV EAX,0x1
$+283    006F67C7     8BCA                     MOV ECX,EDX
$+285    006F67C9     C1EA 03                  SHR EDX,0x3
$+288    006F67CC     83E1 07                  AND ECX,0x7
$+28B    006F67CF     D3E0                     SHL EAX,CL
$+28D    006F67D1     84441A 4C                TEST BYTE PTR DS:[EDX+EBX+0x4C],AL
$+291    006F67D5     74 47                    JE SHORT PathOfEx.006F681E
$+293    006F67D7     B8 01000000              MOV EAX,0x1
$+298    006F67DC     EB 42                    JMP SHORT PathOfEx.006F6820
$+29A    006F67DE     C745 FC 03000000         MOV DWORD PTR SS:[EBP-0x4],0x3
$+2A1    006F67E5     8B7D F0                  MOV EDI,DWORD PTR SS:[EBP-0x10]
$+2A4    006F67E8     85FF                     TEST EDI,EDI
$+2A6    006F67EA     0F84 53010000            JE PathOfEx.006F6943
$+2AC    006F67F0     83CE FF                  OR ESI,0xFFFFFFFF
$+2AF    006F67F3     8BC6                     MOV EAX,ESI
$+2B1    006F67F5     F0:0FC147 04             LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+2B6    006F67FA     0F85 43010000            JNZ PathOfEx.006F6943
$+2BC    006F6800     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+2BE    006F6802     8BCF                     MOV ECX,EDI
$+2C0    006F6804     FF10                     CALL DWORD PTR DS:[EAX]
$+2C2    006F6806     F0:0FC177 08             LOCK XADD DWORD PTR DS:[EDI+0x8],ESI
$+2C7    006F680B     4E                       DEC ESI
$+2C8    006F680C     0F85 31010000            JNZ PathOfEx.006F6943
$+2CE    006F6812     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+2D0    006F6814     8BCF                     MOV ECX,EDI
$+2D2    006F6816     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+2D5    006F6819     E9 25010000              JMP PathOfEx.006F6943
$+2DA    006F681E     33C0                     XOR EAX,EAX
$+2DC    006F6820     84C0                     TEST AL,AL
$+2DE    006F6822     0F9445 0F                SETE BYTE PTR SS:[EBP+0xF]
$+2E2    006F6826     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
$+2E6    006F682A     8B45 C8                  MOV EAX,DWORD PTR SS:[EBP-0x38]
$+2E9    006F682D     85C0                     TEST EAX,EAX
$+2EB    006F682F     74 18                    JE SHORT PathOfEx.006F6849
$+2ED    006F6831     50                       PUSH EAX
$+2EE    006F6832     51                       PUSH ECX
$+2EF    006F6833     E8 187BF1FF              CALL PathOfEx.0060E350
$+2F4    006F6838     83C4 04                  ADD ESP,0x4
$+2F7    006F683B     8BC8                     MOV ECX,EAX
$+2F9    006F683D     E8 DE76F1FF              CALL PathOfEx.0060DF20
$+2FE    006F6842     C745 C8 00000000         MOV DWORD PTR SS:[EBP-0x38],0x0
$+305    006F6849     807D 0F 00               CMP BYTE PTR SS:[EBP+0xF],0x0
$+309    006F684D     EB 45                    JMP SHORT PathOfEx.006F6894
$+30B    006F684F     8D45 C0                  LEA EAX,DWORD PTR SS:[EBP-0x40]
$+30E    006F6852     50                       PUSH EAX
$+30F    006F6853     E8 38BDD9FF              CALL PathOfEx.00492590
$+314    006F6858     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
$+318    006F685C     8BCB                     MOV ECX,EBX
$+31A    006F685E     8B76 28                  MOV ESI,DWORD PTR DS:[ESI+0x28]
$+31D    006F6861     50                       PUSH EAX
$+31E    006F6862     E8 09F8FEFF              CALL PathOfEx.006E6070
$+323    006F6867     3BC6                     CMP EAX,ESI
$+325    006F6869     0F9E45 0F                SETLE BYTE PTR SS:[EBP+0xF]
$+329    006F686D     C645 FC 0A               MOV BYTE PTR SS:[EBP-0x4],0xA
$+32D    006F6871     8B45 C0                  MOV EAX,DWORD PTR SS:[EBP-0x40]
$+330    006F6874     85C0                     TEST EAX,EAX
$+332    006F6876     74 18                    JE SHORT PathOfEx.006F6890
$+334    006F6878     50                       PUSH EAX
$+335    006F6879     51                       PUSH ECX
$+336    006F687A     E8 D17AF1FF              CALL PathOfEx.0060E350
$+33B    006F687F     83C4 04                  ADD ESP,0x4
$+33E    006F6882     8BC8                     MOV ECX,EAX
$+340    006F6884     E8 9776F1FF              CALL PathOfEx.0060DF20
$+345    006F6889     C745 C0 00000000         MOV DWORD PTR SS:[EBP-0x40],0x0
$+34C    006F6890     807D 0F 00               CMP BYTE PTR SS:[EBP+0xF],0x0
$+350    006F6894     75 71                    JNZ SHORT PathOfEx.006F6907
$+352    006F6896     C745 FC 0C000000         MOV DWORD PTR SS:[EBP-0x4],0xC
$+359    006F689D     85FF                     TEST EDI,EDI
$+35B    006F689F     74 18                    JE SHORT PathOfEx.006F68B9
$+35D    006F68A1     57                       PUSH EDI
$+35E    006F68A2     51                       PUSH ECX
$+35F    006F68A3     E8 A87AF1FF              CALL PathOfEx.0060E350
$+364    006F68A8     83C4 04                  ADD ESP,0x4
$+367    006F68AB     8BC8                     MOV ECX,EAX
$+369    006F68AD     E8 6E76F1FF              CALL PathOfEx.0060DF20
$+36E    006F68B2     C745 B8 00000000         MOV DWORD PTR SS:[EBP-0x48],0x0
$+375    006F68B9     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+37C    006F68C0     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
$+37F    006F68C3     8B55 14                  MOV EDX,DWORD PTR SS:[EBP+0x14]
$+382    006F68C6     83C2 08                  ADD EDX,0x8
$+385    006F68C9     8955 14                  MOV DWORD PTR SS:[EBP+0x14],EDX
$+388    006F68CC     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
$+38B    006F68CF     8B48 77                  MOV ECX,DWORD PTR DS:[EAX+0x77]
$+38E    006F68D2     8B40 7B                  MOV EAX,DWORD PTR DS:[EAX+0x7B]
$+391    006F68D5     8D04C8                   LEA EAX,DWORD PTR DS:[EAX+ECX*8]
$+394    006F68D8     3BD0                     CMP EDX,EAX
$+396    006F68DA   ^ 0F85 16FEFFFF            JNZ PathOfEx.006F66F6
$+39C    006F68E0     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
$+39F    006F68E3     8B40 1C                  MOV EAX,DWORD PTR DS:[EAX+0x1C]
$+3A2    006F68E6     8A48 21                  MOV CL,BYTE PTR DS:[EAX+0x21]
$+3A5    006F68E9     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
$+3A8    006F68EC     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
$+3AB    006F68EF     48                       DEC EAX
$+3AC    006F68F0     83F8 15                  CMP EAX,0x15
$+3AF    006F68F3     0F87 B8050000            JA PathOfEx.006F6EB1
$+3B5    006F68F9     0FB680 006F6F00          MOVZX EAX,BYTE PTR DS:[EAX+0x6F6F00]
$+3BC    006F6900     FF2485 CC6E6F00          JMP DWORD PTR DS:[EAX*4+0x6F6ECC]
$+3C3    006F6907     C745 FC 0B000000         MOV DWORD PTR SS:[EBP-0x4],0xB
$+3CA    006F690E     85FF                     TEST EDI,EDI
$+3CC    006F6910     74 31                    JE SHORT PathOfEx.006F6943
$+3CE    006F6912     57                       PUSH EDI
$+3CF    006F6913     51                       PUSH ECX
$+3D0    006F6914     E8 377AF1FF              CALL PathOfEx.0060E350
$+3D5    006F6919     83C4 04                  ADD ESP,0x4
$+3D8    006F691C     8BC8                     MOV ECX,EAX
$+3DA    006F691E     E8 FD75F1FF              CALL PathOfEx.0060DF20
$+3DF    006F6923     EB 1E                    JMP SHORT PathOfEx.006F6943
$+3E1    006F6925     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
$+3E4    006F6928     8BCB                     MOV ECX,EBX
$+3E6    006F692A     50                       PUSH EAX
$+3E7    006F692B     E8 C08DDEFF              CALL PathOfEx.004DF6F0
$+3EC    006F6930     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
$+3EF    006F6933     8B70 04                  MOV ESI,DWORD PTR DS:[EAX+0x4]
$+3F2    006F6936     E8 25B5C3FF              CALL PathOfEx.00331E60
$+3F7    006F693B     85F6                     TEST ESI,ESI
$+3F9    006F693D     0F85 6E050000            JNZ PathOfEx.006F6EB1
$+3FF    006F6943     33C0                     XOR EAX,EAX
$+401    006F6945     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+404    006F6948     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+40B    006F694F     5F                       POP EDI
$+40C    006F6950     5E                       POP ESI
$+40D    006F6951     5B                       POP EBX
$+40E    006F6952     8BE5                     MOV ESP,EBP
$+410    006F6954     5D                       POP EBP
$+411    006F6955     C2 1000                  RETN 0x10
$+414    006F6958     8B75 E8                  MOV ESI,DWORD PTR SS:[EBP-0x18]
$+417    006F695B     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
$+41A    006F695E     51                       PUSH ECX
$+41B    006F695F     8B46 0C                  MOV EAX,DWORD PTR DS:[ESI+0xC]
$+41E    006F6962     8B48 0C                  MOV ECX,DWORD PTR DS:[EAX+0xC]
$+421    006F6965     E8 4627C7FF              CALL PathOfEx.003690B0
$+426    006F696A     8BF8                     MOV EDI,EAX
$+428    006F696C     C745 FC 0D000000         MOV DWORD PTR SS:[EBP-0x4],0xD
$+42F    006F6973     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
$+432    006F6976     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
$+435    006F6979     50                       PUSH EAX
$+436    006F697A     8B49 0C                  MOV ECX,DWORD PTR DS:[ECX+0xC]
$+439    006F697D     E8 2E27C7FF              CALL PathOfEx.003690B0
$+43E    006F6982     8D4D 08                  LEA ECX,DWORD PTR SS:[EBP+0x8]
$+441    006F6985     51                       PUSH ECX
$+442    006F6986     8BC8                     MOV ECX,EAX
$+444    006F6988     E8 F34EC6FF              CALL PathOfEx.0035B880
$+449    006F698D     8BC8                     MOV ECX,EAX
$+44B    006F698F     C645 FC 0F               MOV BYTE PTR SS:[EBP-0x4],0xF
$+44F    006F6993     E8 8829C5FF              CALL PathOfEx.00349320
$+454    006F6998     8B7F 04                  MOV EDI,DWORD PTR DS:[EDI+0x4]
$+457    006F699B     8D4D 08                  LEA ECX,DWORD PTR SS:[EBP+0x8]
$+45A    006F699E     8B70 04                  MOV ESI,DWORD PTR DS:[EAX+0x4]
$+45D    006F69A1     E8 CAECC2FF              CALL PathOfEx.00325670
$+462    006F69A6     8D4D B8                  LEA ECX,DWORD PTR SS:[EBP-0x48]
$+465    006F69A9     E8 B2B4C3FF              CALL PathOfEx.00331E60
$+46A    006F69AE     8D4D C0                  LEA ECX,DWORD PTR SS:[EBP-0x40]
$+46D    006F69B1     E8 AAB4C3FF              CALL PathOfEx.00331E60
$+472    006F69B6     2BFE                     SUB EDI,ESI
$+474    006F69B8     B8 03B57EA5              MOV EAX,0xA57EB503
$+479    006F69BD     F7EF                     IMUL EDI
$+47B    006F69BF     8BCB                     MOV ECX,EBX
$+47D    006F69C1     03D7                     ADD EDX,EDI
$+47F    006F69C3     C1FA 06                  SAR EDX,0x6
$+482    006F69C6     8BC2                     MOV EAX,EDX
$+484    006F69C8     42                       INC EDX
$+485    006F69C9     C1E8 1F                  SHR EAX,0x1F
$+488    006F69CC     03C2                     ADD EAX,EDX
$+48A    006F69CE     50                       PUSH EAX
$+48B    006F69CF     E8 7CF5FEFF              CALL PathOfEx.006E5F50
$+490    006F69D4     84C0                     TEST AL,AL
$+492    006F69D6     0F84 A5000000            JE PathOfEx.006F6A81
$+498    006F69DC     0FB683 A5000000          MOVZX EAX,BYTE PTR DS:[EBX+0xA5]
$+49F    006F69E3     A8 40                    TEST AL,0x40
$+4A1    006F69E5     74 18                    JE SHORT PathOfEx.006F69FF
$+4A3    006F69E7     B8 13060000              MOV EAX,0x613
$+4A8    006F69EC     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+4AB    006F69EF     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+4B2    006F69F6     5F                       POP EDI
$+4B3    006F69F7     5E                       POP ESI
$+4B4    006F69F8     5B                       POP EBX
$+4B5    006F69F9     8BE5                     MOV ESP,EBP
$+4B7    006F69FB     5D                       POP EBP
$+4B8    006F69FC     C2 1000                  RETN 0x10
$+4BB    006F69FF     84C0                     TEST AL,AL
$+4BD    006F6A01     0F89 AA040000            JNS PathOfEx.006F6EB1
$+4C3    006F6A07     B8 43060000              MOV EAX,0x643
$+4C8    006F6A0C     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+4CB    006F6A0F     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+4D2    006F6A16     5F                       POP EDI
$+4D3    006F6A17     5E                       POP ESI
$+4D4    006F6A18     5B                       POP EBX
$+4D5    006F6A19     8BE5                     MOV ESP,EBP
$+4D7    006F6A1B     5D                       POP EBP
$+4D8    006F6A1C     C2 1000                  RETN 0x10
$+4DB    006F6A1F     84C9                     TEST CL,CL
$+4DD    006F6A21   ^ 0F85 1CFFFFFF            JNZ PathOfEx.006F6943
$+4E3    006F6A27     6A 08                    PUSH 0x8
$+4E5    006F6A29     8BCB                     MOV ECX,EBX
$+4E7    006F6A2B     E8 20F5FEFF              CALL PathOfEx.006E5F50
$+4EC    006F6A30     84C0                     TEST AL,AL
$+4EE    006F6A32   ^ 0F85 0BFFFFFF            JNZ PathOfEx.006F6943
$+4F4    006F6A38     0FB683 29010000          MOVZX EAX,BYTE PTR DS:[EBX+0x129]
$+4FB    006F6A3F     A8 40                    TEST AL,0x40
$+4FD    006F6A41   ^ 0F84 FCFEFFFF            JE PathOfEx.006F6943
$+503    006F6A47     A8 20                    TEST AL,0x20
$+505    006F6A49     0F84 62040000            JE PathOfEx.006F6EB1
$+50B    006F6A4F   ^ E9 EFFEFFFF              JMP PathOfEx.006F6943
$+510    006F6A54     84C9                     TEST CL,CL
$+512    006F6A56   ^ 0F85 E7FEFFFF            JNZ PathOfEx.006F6943
$+518    006F6A5C     F683 A5000000 80         TEST BYTE PTR DS:[EBX+0xA5],0x80
$+51F    006F6A63   ^ E9 D5FEFFFF              JMP PathOfEx.006F693D
$+524    006F6A68     84C9                     TEST CL,CL
$+526    006F6A6A   ^ 0F85 D3FEFFFF            JNZ PathOfEx.006F6943
$+52C    006F6A70     6A 08                    PUSH 0x8
$+52E    006F6A72     8BCB                     MOV ECX,EBX
$+530    006F6A74     E8 D7F4FEFF              CALL PathOfEx.006E5F50
$+535    006F6A79     84C0                     TEST AL,AL
$+537    006F6A7B     0F85 30040000            JNZ PathOfEx.006F6EB1
$+53D    006F6A81     B8 12060000              MOV EAX,0x612
$+542    006F6A86     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+545    006F6A89     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+54C    006F6A90     5F                       POP EDI
$+54D    006F6A91     5E                       POP ESI
$+54E    006F6A92     5B                       POP EBX
$+54F    006F6A93     8BE5                     MOV ESP,EBP
$+551    006F6A95     5D                       POP EBP
$+552    006F6A96     C2 1000                  RETN 0x10
$+555    006F6A99     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
$+558    006F6A9C     8BCB                     MOV ECX,EBX
$+55A    006F6A9E     50                       PUSH EAX
$+55B    006F6A9F     E8 4C8CDEFF              CALL PathOfEx.004DF6F0
$+560    006F6AA4     8378 04 00               CMP DWORD PTR DS:[EAX+0x4],0x0
$+564    006F6AA8     74 59                    JE SHORT PathOfEx.006F6B03
$+566    006F6AAA     8B83 D4010000            MOV EAX,DWORD PTR DS:[EBX+0x1D4]
$+56C    006F6AB0     33F6                     XOR ESI,ESI
$+56E    006F6AB2     8B8B D0010000            MOV ECX,DWORD PTR DS:[EBX+0x1D0]
$+574    006F6AB8     8BF8                     MOV EDI,EAX
$+576    006F6ABA     2BF9                     SUB EDI,ECX
$+578    006F6ABC     33DB                     XOR EBX,EBX
$+57A    006F6ABE     33D2                     XOR EDX,EDX
$+57C    006F6AC0     3BC8                     CMP ECX,EAX
$+57E    006F6AC2     0F47FB                   CMOVA EDI,EBX
$+581    006F6AC5     85FF                     TEST EDI,EDI
$+583    006F6AC7     74 18                    JE SHORT PathOfEx.006F6AE1
$+585    006F6AC9     0F1F80 00000000          NOP DWORD PTR DS:[EAX]
$+58C    006F6AD0     8A01                     MOV AL,BYTE PTR DS:[ECX]
$+58E    006F6AD2     3C 08                    CMP AL,0x8
$+590    006F6AD4     74 05                    JE SHORT PathOfEx.006F6ADB
$+592    006F6AD6     84C0                     TEST AL,AL
$+594    006F6AD8     74 01                    JE SHORT PathOfEx.006F6ADB
$+596    006F6ADA     46                       INC ESI
$+597    006F6ADB     42                       INC EDX
$+598    006F6ADC     41                       INC ECX
$+599    006F6ADD     3BD7                     CMP EDX,EDI
$+59B    006F6ADF   ^ 75 EF                    JNZ SHORT PathOfEx.006F6AD0
$+59D    006F6AE1     8B45 10                  MOV EAX,DWORD PTR SS:[EBP+0x10]
$+5A0    006F6AE4     83F8 07                  CMP EAX,0x7
$+5A3    006F6AE7     72 07                    JB SHORT PathOfEx.006F6AF0
$+5A5    006F6AE9     B8 07000000              MOV EAX,0x7
$+5AA    006F6AEE     EB 0B                    JMP SHORT PathOfEx.006F6AFB
$+5AC    006F6AF0     83F8 05                  CMP EAX,0x5
*/