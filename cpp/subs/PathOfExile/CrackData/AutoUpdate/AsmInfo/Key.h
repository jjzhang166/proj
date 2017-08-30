#pragma once
/*
@author zhp
@date 2017/2/23 3:54
@purpose for keyboard asm record
*/

//tag_按esc键会断到此处
//调用此函数的地方是ui事件处理的地方。
/*
$-4CF    0067CD75       CD 67                    INT 0x67										 ; 第二个参数是stCD_UiEventParam*，该结构体大小为0xc
$-4CD    0067CD77       00FB                     ADD BL, BH										 ; 第一个参数是子ui对象，但没用到
$-4CB    0067CD79       CC                       INT3                                            ; 很多关于按esc的功能都在这个函数里面调用的
$-4CA    0067CD7A       67:00CC                  ADD AH, CL                                      ; 比如关闭npc面板、传送点面板等
$-4C7    0067CD7D       CC                       INT3                                            ; 找相应的功能时，先断住相应的发包，然后堆栈往上回朔，
$-4C6    0067CD7E       CC                       INT3                                            ; 其顶层函数若是被这个函数调用的，
$-4C5    0067CD7F       CC                       INT3                                            ; 那么该顶层函数便是要找的功能函数了
$-4C4    0067CD80       55                       PUSH EBP                                        ; tag_按esc键会断到此处
$-4C3    0067CD81       8BEC                     MOV EBP, ESP									 ; ecx=stCD_InGameMainUi*
$-4C1    0067CD83       83E4 F8                  AND ESP, -0x8
$-4BE    0067CD86       83EC 0C                  SUB ESP, 0xC
$-4BB    0067CD89       53                       PUSH EBX
$-4BA    0067CD8A       56                       PUSH ESI
$-4B9    0067CD8B       57                       PUSH EDI
$-4B8    0067CD8C       8B7D 0C                  MOV EDI, DWORD PTR SS:[EBP+0xC]
$-4B5    0067CD8F       8BF1                     MOV ESI, ECX
$-4B3    0067CD91       803F 00                  CMP BYTE PTR DS:[EDI], 0x0
$-4B0    0067CD94       0F85 D5090000            JNZ PathOfEx.0067D76F
$-4AA    0067CD9A       8B47 04                  MOV EAX, DWORD PTR DS:[EDI+0x4]
$-4A7    0067CD9D       83F8 25                  CMP EAX, 0x25
$-4A4    0067CDA0       74 12                    JE SHORT PathOfEx.0067CDB4
$-4A2    0067CDA2       83F8 27                  CMP EAX, 0x27
$-49F    0067CDA5       0F85 BC020000            JNZ PathOfEx.0067D067
$-499    0067CDAB       83F8 25                  CMP EAX, 0x25
$-496    0067CDAE       0F85 57010000            JNZ PathOfEx.0067CF0B
$-490    0067CDB4       8B8E 580A0000            MOV ECX, DWORD PTR DS:[ESI+0xA58]
$-48A    0067CDBA       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-483    0067CDC1       74 17                    JE SHORT PathOfEx.0067CDDA
$-481    0067CDC3       8B89 D4080000            MOV ECX, DWORD PTR DS:[ECX+0x8D4]
$-47B    0067CDC9       E8 12B63A00              CALL PathOfEx.00A283E0
$-476    0067CDCE       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-473    0067CDD1       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-472    0067CDD2       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-471    0067CDD3       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-470    0067CDD4       8BE5                     MOV ESP, EBP
$-46E    0067CDD6       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-46D    0067CDD7       C2 0800                  RETN 0x8
$-46A    0067CDDA       8B8E 040A0000            MOV ECX, DWORD PTR DS:[ESI+0xA04]               ; PathOfEx.00740061
$-464    0067CDE0       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-45D    0067CDE7       74 17                    JE SHORT PathOfEx.0067CE00
$-45B    0067CDE9       8B89 74090000            MOV ECX, DWORD PTR DS:[ECX+0x974]
$-455    0067CDEF       E8 ECB53A00              CALL PathOfEx.00A283E0
$-450    0067CDF4       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-44D    0067CDF7       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-44C    0067CDF8       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-44B    0067CDF9       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-44A    0067CDFA       8BE5                     MOV ESP, EBP
$-448    0067CDFC       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-447    0067CDFD       C2 0800                  RETN 0x8
$-444    0067CE00       8B8E 080A0000            MOV ECX, DWORD PTR DS:[ESI+0xA08]               ; PathOfEx.006C005F
$-43E    0067CE06       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-437    0067CE0D       0F84 A8000000            JE PathOfEx.0067CEBB
$-431    0067CE13       8B91 70090000            MOV EDX, DWORD PTR DS:[ECX+0x970]
$-42B    0067CE19       85D2                     TEST EDX, EDX
$-429    0067CE1B       0F84 4B090000            JE PathOfEx.0067D76C
$-423    0067CE21       8B81 9C090000            MOV EAX, DWORD PTR DS:[ECX+0x99C]
$-41D    0067CE27       83E8 00                  SUB EAX, 0x0
$-41A    0067CE2A       74 7C                    JE SHORT PathOfEx.0067CEA8
$-418    0067CE2C       83E8 01                  SUB EAX, 0x1
$-415    0067CE2F       74 40                    JE SHORT PathOfEx.0067CE71
$-413    0067CE31       83E8 01                  SUB EAX, 0x1
$-410    0067CE34       0F85 32090000            JNZ PathOfEx.0067D76C
$-40A    0067CE3A       8B82 DC0A0000            MOV EAX, DWORD PTR DS:[EDX+0xADC]
$-404    0067CE40       3B81 88090000            CMP EAX, DWORD PTR DS:[ECX+0x988]
$-3FE    0067CE46       0F85 20090000            JNZ PathOfEx.0067D76C
$-3F8    0067CE4C       8B89 74090000            MOV ECX, DWORD PTR DS:[ECX+0x974]
$-3F2    0067CE52       85C9                     TEST ECX, ECX
$-3F0    0067CE54       0F84 12090000            JE PathOfEx.0067D76C
$-3EA    0067CE5A       8B89 3C090000            MOV ECX, DWORD PTR DS:[ECX+0x93C]
$-3E4    0067CE60       E8 7BB53A00              CALL PathOfEx.00A283E0
$-3DF    0067CE65       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-3DC    0067CE68       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-3DB    0067CE69       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-3DA    0067CE6A       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-3D9    0067CE6B       8BE5                     MOV ESP, EBP
$-3D7    0067CE6D       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-3D6    0067CE6E       C2 0800                  RETN 0x8
$-3D3    0067CE71       8B82 DC0A0000            MOV EAX, DWORD PTR DS:[EDX+0xADC]
$-3CD    0067CE77       3B81 90090000            CMP EAX, DWORD PTR DS:[ECX+0x990]
$-3C7    0067CE7D       0F85 E9080000            JNZ PathOfEx.0067D76C
$-3C1    0067CE83       8B89 84090000            MOV ECX, DWORD PTR DS:[ECX+0x984]
$-3BB    0067CE89       85C9                     TEST ECX, ECX
$-3B9    0067CE8B       0F84 DB080000            JE PathOfEx.0067D76C
$-3B3    0067CE91       8B89 38090000            MOV ECX, DWORD PTR DS:[ECX+0x938]
$-3AD    0067CE97       E8 44B53A00              CALL PathOfEx.00A283E0
$-3A8    0067CE9C       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-3A5    0067CE9F       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-3A4    0067CEA0       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-3A3    0067CEA1       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-3A2    0067CEA2       8BE5                     MOV ESP, EBP
$-3A0    0067CEA4       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-39F    0067CEA5       C2 0800                  RETN 0x8
$-39C    0067CEA8       8BCA                     MOV ECX, EDX
$-39A    0067CEAA       E8 31B53A00              CALL PathOfEx.00A283E0
$-395    0067CEAF       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-392    0067CEB2       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-391    0067CEB3       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-390    0067CEB4       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-38F    0067CEB5       8BE5                     MOV ESP, EBP
$-38D    0067CEB7       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-38C    0067CEB8       C2 0800                  RETN 0x8
$-389    0067CEBB       8B8E 0C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA0C]               ; PathOfEx.006E006F
$-383    0067CEC1       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-37C    0067CEC8     ^ 0F85 1BFFFFFF            JNZ PathOfEx.0067CDE9
$-376    0067CECE       8B8E 100A0000            MOV ECX, DWORD PTR DS:[ESI+0xA10]               ; PathOfEx.005F0067
$-370    0067CED4       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-369    0067CEDB     ^ 0F85 08FFFFFF            JNZ PathOfEx.0067CDE9
$-363    0067CEE1       8B8E F8090000            MOV ECX, DWORD PTR DS:[ESI+0x9F8]               ; PathOfEx.006D005F
$-35D    0067CEE7       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-356    0067CEEE       0F84 73010000            JE PathOfEx.0067D067
$-350    0067CEF4       8B89 70090000            MOV ECX, DWORD PTR DS:[ECX+0x970]
$-34A    0067CEFA       E8 E1B43A00              CALL PathOfEx.00A283E0
$-345    0067CEFF       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-342    0067CF02       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-341    0067CF03       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-340    0067CF04       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-33F    0067CF05       8BE5                     MOV ESP, EBP
$-33D    0067CF07       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-33C    0067CF08       C2 0800                  RETN 0x8
$-339    0067CF0B       83F8 27                  CMP EAX, 0x27
$-336    0067CF0E       0F85 58080000            JNZ PathOfEx.0067D76C
$-330    0067CF14       8B8E 580A0000            MOV ECX, DWORD PTR DS:[ESI+0xA58]
$-32A    0067CF1A       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-323    0067CF21       74 17                    JE SHORT PathOfEx.0067CF3A
$-321    0067CF23       8B89 D4080000            MOV ECX, DWORD PTR DS:[ECX+0x8D4]
$-31B    0067CF29       E8 22B43A00              CALL PathOfEx.00A28350
$-316    0067CF2E       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-313    0067CF31       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-312    0067CF32       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-311    0067CF33       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-310    0067CF34       8BE5                     MOV ESP, EBP
$-30E    0067CF36       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-30D    0067CF37       C2 0800                  RETN 0x8
$-30A    0067CF3A       8B8E 040A0000            MOV ECX, DWORD PTR DS:[ESI+0xA04]               ; PathOfEx.00740061
$-304    0067CF40       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-2FD    0067CF47       74 17                    JE SHORT PathOfEx.0067CF60
$-2FB    0067CF49       8B89 74090000            MOV ECX, DWORD PTR DS:[ECX+0x974]
$-2F5    0067CF4F       E8 FCB33A00              CALL PathOfEx.00A28350
$-2F0    0067CF54       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-2ED    0067CF57       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-2EC    0067CF58       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-2EB    0067CF59       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-2EA    0067CF5A       8BE5                     MOV ESP, EBP
$-2E8    0067CF5C       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-2E7    0067CF5D       C2 0800                  RETN 0x8
$-2E4    0067CF60       8B8E 080A0000            MOV ECX, DWORD PTR DS:[ESI+0xA08]               ; PathOfEx.006C005F
$-2DE    0067CF66       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-2D7    0067CF6D       0F84 A8000000            JE PathOfEx.0067D01B
$-2D1    0067CF73       8B91 70090000            MOV EDX, DWORD PTR DS:[ECX+0x970]
$-2CB    0067CF79       85D2                     TEST EDX, EDX
$-2C9    0067CF7B       0F84 EB070000            JE PathOfEx.0067D76C
$-2C3    0067CF81       8B81 9C090000            MOV EAX, DWORD PTR DS:[ECX+0x99C]
$-2BD    0067CF87       83E8 00                  SUB EAX, 0x0
$-2BA    0067CF8A       74 7C                    JE SHORT PathOfEx.0067D008
$-2B8    0067CF8C       83E8 01                  SUB EAX, 0x1
$-2B5    0067CF8F       74 40                    JE SHORT PathOfEx.0067CFD1
$-2B3    0067CF91       83E8 01                  SUB EAX, 0x1
$-2B0    0067CF94       0F85 D2070000            JNZ PathOfEx.0067D76C
$-2AA    0067CF9A       8B82 DC0A0000            MOV EAX, DWORD PTR DS:[EDX+0xADC]
$-2A4    0067CFA0       3B81 88090000            CMP EAX, DWORD PTR DS:[ECX+0x988]
$-29E    0067CFA6       0F85 C0070000            JNZ PathOfEx.0067D76C
$-298    0067CFAC       8B89 74090000            MOV ECX, DWORD PTR DS:[ECX+0x974]
$-292    0067CFB2       85C9                     TEST ECX, ECX
$-290    0067CFB4       0F84 B2070000            JE PathOfEx.0067D76C
$-28A    0067CFBA       8B89 3C090000            MOV ECX, DWORD PTR DS:[ECX+0x93C]
$-284    0067CFC0       E8 8BB33A00              CALL PathOfEx.00A28350
$-27F    0067CFC5       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-27C    0067CFC8       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-27B    0067CFC9       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-27A    0067CFCA       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-279    0067CFCB       8BE5                     MOV ESP, EBP
$-277    0067CFCD       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-276    0067CFCE       C2 0800                  RETN 0x8
$-273    0067CFD1       8B82 DC0A0000            MOV EAX, DWORD PTR DS:[EDX+0xADC]
$-26D    0067CFD7       3B81 90090000            CMP EAX, DWORD PTR DS:[ECX+0x990]
$-267    0067CFDD       0F85 89070000            JNZ PathOfEx.0067D76C
$-261    0067CFE3       8B89 84090000            MOV ECX, DWORD PTR DS:[ECX+0x984]
$-25B    0067CFE9       85C9                     TEST ECX, ECX
$-259    0067CFEB       0F84 7B070000            JE PathOfEx.0067D76C
$-253    0067CFF1       8B89 38090000            MOV ECX, DWORD PTR DS:[ECX+0x938]
$-24D    0067CFF7       E8 54B33A00              CALL PathOfEx.00A28350
$-248    0067CFFC       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-245    0067CFFF       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-244    0067D000       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-243    0067D001       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-242    0067D002       8BE5                     MOV ESP, EBP
$-240    0067D004       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-23F    0067D005       C2 0800                  RETN 0x8
$-23C    0067D008       8BCA                     MOV ECX, EDX
$-23A    0067D00A       E8 41B33A00              CALL PathOfEx.00A28350
$-235    0067D00F       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-232    0067D012       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-231    0067D013       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-230    0067D014       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-22F    0067D015       8BE5                     MOV ESP, EBP
$-22D    0067D017       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-22C    0067D018       C2 0800                  RETN 0x8
$-229    0067D01B       8B8E 0C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA0C]               ; PathOfEx.006E006F
$-223    0067D021       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-21C    0067D028     ^ 0F85 1BFFFFFF            JNZ PathOfEx.0067CF49
$-216    0067D02E       8B8E 100A0000            MOV ECX, DWORD PTR DS:[ESI+0xA10]               ; PathOfEx.005F0067
$-210    0067D034       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-209    0067D03B     ^ 0F85 08FFFFFF            JNZ PathOfEx.0067CF49
$-203    0067D041       8B8E F8090000            MOV ECX, DWORD PTR DS:[ESI+0x9F8]               ; PathOfEx.006D005F
$-1FD    0067D047       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-1F6    0067D04E       74 17                    JE SHORT PathOfEx.0067D067
$-1F4    0067D050       8B89 70090000            MOV ECX, DWORD PTR DS:[ECX+0x970]
$-1EE    0067D056       E8 F5B23A00              CALL PathOfEx.00A28350
$-1E9    0067D05B       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-1E6    0067D05E       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$-1E5    0067D05F       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$-1E4    0067D060       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$-1E3    0067D061       8BE5                     MOV ESP, EBP
$-1E1    0067D063       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$-1E0    0067D064       C2 0800                  RETN 0x8
$-1DD    0067D067       8B8E 680B0000            MOV ECX, DWORD PTR DS:[ESI+0xB68]
$-1D7    0067D06D       8D4424 10                LEA EAX, DWORD PTR SS:[ESP+0x10]
$-1D3    0067D071       50                       PUSH EAX
$-1D2    0067D072       6A 25                    PUSH 0x25
$-1D0    0067D074       E8 17241400              CALL PathOfEx.007BF490
$-1CB    0067D079       84C0                     TEST AL, AL
$-1C9    0067D07B       0F94C0                   SETE AL
$-1C6    0067D07E       84C0                     TEST AL, AL
$-1C4    0067D080       0F84 7E010000            JE PathOfEx.0067D204
$-1BE    0067D086       8B8E 680B0000            MOV ECX, DWORD PTR DS:[ESI+0xB68]
$-1B8    0067D08C       8D4424 10                LEA EAX, DWORD PTR SS:[ESP+0x10]
$-1B4    0067D090       50                       PUSH EAX
$-1B3    0067D091       6A 27                    PUSH 0x27
$-1B1    0067D093       E8 F8231400              CALL PathOfEx.007BF490
$-1AC    0067D098       84C0                     TEST AL, AL
$-1AA    0067D09A       0F94C0                   SETE AL
$-1A7    0067D09D       84C0                     TEST AL, AL
$-1A5    0067D09F       0F84 5F010000            JE PathOfEx.0067D204
$-19F    0067D0A5       8B8E 680B0000            MOV ECX, DWORD PTR DS:[ESI+0xB68]
$-199    0067D0AB       8D4424 10                LEA EAX, DWORD PTR SS:[ESP+0x10]
$-195    0067D0AF       50                       PUSH EAX
$-194    0067D0B0       6A 26                    PUSH 0x26
$-192    0067D0B2       E8 D9231400              CALL PathOfEx.007BF490
$-18D    0067D0B7       84C0                     TEST AL, AL
$-18B    0067D0B9       0F94C0                   SETE AL
$-188    0067D0BC       84C0                     TEST AL, AL
$-186    0067D0BE       0F84 40010000            JE PathOfEx.0067D204
$-180    0067D0C4       8B8E 680B0000            MOV ECX, DWORD PTR DS:[ESI+0xB68]
$-17A    0067D0CA       8D4424 10                LEA EAX, DWORD PTR SS:[ESP+0x10]
$-176    0067D0CE       50                       PUSH EAX
$-175    0067D0CF       6A 28                    PUSH 0x28
$-173    0067D0D1       E8 BA231400              CALL PathOfEx.007BF490
$-16E    0067D0D6       84C0                     TEST AL, AL
$-16C    0067D0D8       0F94C0                   SETE AL
$-169    0067D0DB       84C0                     TEST AL, AL
$-167    0067D0DD       0F84 21010000            JE PathOfEx.0067D204
$-161    0067D0E3       8B86 340A0000            MOV EAX, DWORD PTR DS:[ESI+0xA34]               ; PathOfEx.0069005F
$-15B    0067D0E9       8B88 34090000            MOV ECX, DWORD PTR DS:[EAX+0x934]
$-155    0067D0EF       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$-14E    0067D0F6       8B81 DC080000            MOV EAX, DWORD PTR DS:[ECX+0x8DC]
$-148    0067D0FC       894424 10                MOV DWORD PTR SS:[ESP+0x10], EAX
$-144    0067D100       8B81 E0080000            MOV EAX, DWORD PTR DS:[ECX+0x8E0]
$-13E    0067D106       894424 14                MOV DWORD PTR SS:[ESP+0x14], EAX
$-13A    0067D10A       0F84 AC000000            JE PathOfEx.0067D1BC
$-134    0067D110       8B86 A8090000            MOV EAX, DWORD PTR DS:[ESI+0x9A8]               ; PathOfEx.0073006F
$-12E    0067D116       80B8 380A0000 00         CMP BYTE PTR DS:[EAX+0xA38], 0x0
$-127    0067D11D       0F85 99000000            JNZ PathOfEx.0067D1BC
$-121    0067D123       8B86 580A0000            MOV EAX, DWORD PTR DS:[ESI+0xA58]
$-11B    0067D129       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-114    0067D130       0F85 86000000            JNZ PathOfEx.0067D1BC
$-10E    0067D136       8B86 040A0000            MOV EAX, DWORD PTR DS:[ESI+0xA04]               ; PathOfEx.00740061
$-108    0067D13C       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-101    0067D143       75 77                    JNZ SHORT PathOfEx.0067D1BC
$-FF     0067D145       8B86 0C0A0000            MOV EAX, DWORD PTR DS:[ESI+0xA0C]               ; PathOfEx.006E006F
$-F9     0067D14B       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-F2     0067D152       75 68                    JNZ SHORT PathOfEx.0067D1BC
$-F0     0067D154       8B86 100A0000            MOV EAX, DWORD PTR DS:[ESI+0xA10]               ; PathOfEx.005F0067
$-EA     0067D15A       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-E3     0067D161       75 59                    JNZ SHORT PathOfEx.0067D1BC
$-E1     0067D163       8B86 F8090000            MOV EAX, DWORD PTR DS:[ESI+0x9F8]               ; PathOfEx.006D005F
$-DB     0067D169       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-D4     0067D170       75 4A                    JNZ SHORT PathOfEx.0067D1BC
$-D2     0067D172       8B86 080A0000            MOV EAX, DWORD PTR DS:[ESI+0xA08]               ; PathOfEx.006C005F
$-CC     0067D178       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$-C5     0067D17F       75 3B                    JNZ SHORT PathOfEx.0067D1BC
$-C3     0067D181       8B47 04                  MOV EAX, DWORD PTR DS:[EDI+0x4]
$-C0     0067D184       83C0 DB                  ADD EAX, -0x25
$-BD     0067D187       83F8 03                  CMP EAX, 0x3
$-BA     0067D18A       77 30                    JA SHORT PathOfEx.0067D1BC
$-B8     0067D18C       FF2485 80D76700          JMP NEAR DWORD PTR DS:[EAX*4+0x67D780]          ; PathOfEx.0067D193
$-B1     0067D193       C74424 14 0080A243       MOV DWORD PTR SS:[ESP+0x14], 0x43A28000
$-A9     0067D19B       EB 1C                    JMP SHORT PathOfEx.0067D1B9
$-A7     0067D19D       C74424 14 0080A2C3       MOV DWORD PTR SS:[ESP+0x14], 0xC3A28000
$-9F     0067D1A5       EB 12                    JMP SHORT PathOfEx.0067D1B9
$-9D     0067D1A7       C74424 10 0080A243       MOV DWORD PTR SS:[ESP+0x10], 0x43A28000
$-95     0067D1AF       EB 08                    JMP SHORT PathOfEx.0067D1B9
$-93     0067D1B1       C74424 10 0080A2C3       MOV DWORD PTR SS:[ESP+0x10], 0xC3A28000
$-8B     0067D1B9       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$-88     0067D1BC       8B86 340A0000            MOV EAX, DWORD PTR DS:[ESI+0xA34]               ; PathOfEx.0069005F
$-82     0067D1C2       F3:0F104424 10           MOVSS XMM0, DWORD PTR SS:[ESP+0x10]
$-7C     0067D1C8       8B88 34090000            MOV ECX, DWORD PTR DS:[EAX+0x934]
$-76     0067D1CE       0F2E81 DC080000          UCOMISS XMM0, DWORD PTR DS:[ECX+0x8DC]
$-6F     0067D1D5       9F                       LAHF
$-6E     0067D1D6       F6C4 44                  TEST AH, 0x44
$-6B     0067D1D9       7A 1F                    JPE SHORT PathOfEx.0067D1FA
$-69     0067D1DB       F3:0F104424 14           MOVSS XMM0, DWORD PTR SS:[ESP+0x14]
$-63     0067D1E1       0F2E81 E0080000          UCOMISS XMM0, DWORD PTR DS:[ECX+0x8E0]
$-5C     0067D1E8       9F                       LAHF
$-5B     0067D1E9       F6C4 44                  TEST AH, 0x44
$-58     0067D1EC       7A 0C                    JPE SHORT PathOfEx.0067D1FA
$-56     0067D1EE       C781 D8080000 0000C03F   MOV DWORD PTR DS:[ECX+0x8D8], 0x3FC00000
$-4C     0067D1F8       EB 0A                    JMP SHORT PathOfEx.0067D204
$-4A     0067D1FA       8D4424 10                LEA EAX, DWORD PTR SS:[ESP+0x10]
$-46     0067D1FE       50                       PUSH EAX
$-45     0067D1FF       E8 7CACF3FF              CALL PathOfEx.005B7E80
$-40     0067D204       803F 00                  CMP BYTE PTR DS:[EDI], 0x0
$-3D     0067D207       0F85 62050000            JNZ PathOfEx.0067D76F
$-37     0067D20D       8B47 04                  MOV EAX, DWORD PTR DS:[EDI+0x4]
$-34     0067D210       83F8 0D                  CMP EAX, 0xD
$-31     0067D213       0F84 0D050000            JE PathOfEx.0067D726
$-2B     0067D219       83F8 1B                  CMP EAX, 0x1B
$-28     0067D21C       0F84 92010000            JE PathOfEx.0067D3B4
$-22     0067D222       83F8 20                  CMP EAX, 0x20
$-1F     0067D225       0F85 44050000            JNZ PathOfEx.0067D76F
$-19     0067D22B       807F 08 00               CMP BYTE PTR DS:[EDI+0x8], 0x0
$-15     0067D22F       0F85 3A050000            JNZ PathOfEx.0067D76F
$-F      0067D235       8BCE                     MOV ECX, ESI
$-D      0067D237       E8 A4290000              CALL PathOfEx.0067FBE0
$-8      0067D23C       84C0                     TEST AL, AL
$-6      0067D23E       74 15                    JE SHORT PathOfEx.0067D255
$-4      0067D240       6A 00                    PUSH 0x0
$-2      0067D242       8BCE                     MOV ECX, ESI
$ ==>    0067D244       E8 D7280000              CALL PathOfEx.0067FB20                          ; tag_关闭npc面板
$+5      0067D249       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+8      0067D24C       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+9      0067D24D       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+A      0067D24E       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+B      0067D24F       8BE5                     MOV ESP, EBP
$+D      0067D251       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+E      0067D252       C2 0800                  RETN 0x8
$+11     0067D255       8B8E 640A0000            MOV ECX, DWORD PTR DS:[ESI+0xA64]               ; PathOfEx.00610063
$+17     0067D25B       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+1E     0067D262       0F85 FD040000            JNZ PathOfEx.0067D765
$+24     0067D268       8B86 680A0000            MOV EAX, DWORD PTR DS:[ESI+0xA68]               ; PathOfEx.005F006C
$+2A     0067D26E       894424 10                MOV DWORD PTR SS:[ESP+0x10], EAX
$+2E     0067D272       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$+35     0067D279       74 3D                    JE SHORT PathOfEx.0067D2B8
$+37     0067D27B       8BC8                     MOV ECX, EAX
$+39     0067D27D       E8 EEE23600              CALL PathOfEx.009EB570
$+3E     0067D282       8B7424 10                MOV ESI, DWORD PTR SS:[ESP+0x10]
$+42     0067D286       8AD8                     MOV BL, AL
$+44     0067D288       6A 00                    PUSH 0x0
$+46     0067D28A       8BCE                     MOV ECX, ESI
$+48     0067D28C       E8 EFE13600              CALL PathOfEx.009EB480
$+4D     0067D291       84DB                     TEST BL, BL
$+4F     0067D293       0F84 D3040000            JE PathOfEx.0067D76C
$+55     0067D299       8B8E C0080000            MOV ECX, DWORD PTR DS:[ESI+0x8C0]
$+5B     0067D29F       85C9                     TEST ECX, ECX
$+5D     0067D2A1       0F84 C5040000            JE PathOfEx.0067D76C
$+63     0067D2A7       E8 F49D2500              CALL PathOfEx.008D70A0
$+68     0067D2AC       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+6B     0067D2AF       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+6C     0067D2B0       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+6D     0067D2B1       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+6E     0067D2B2       8BE5                     MOV ESP, EBP
$+70     0067D2B4       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+71     0067D2B5       C2 0800                  RETN 0x8
$+74     0067D2B8       8B8E 6C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA6C]               ; PathOfEx.006E0075
$+7A     0067D2BE       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+81     0067D2C5       0F85 9A040000            JNZ PathOfEx.0067D765
$+87     0067D2CB       8B8E 700A0000            MOV ECX, DWORD PTR DS:[ESI+0xA70]               ; PathOfEx.00710069
$+8D     0067D2D1       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+94     0067D2D8       0F85 87040000            JNZ PathOfEx.0067D765
$+9A     0067D2DE       8B8E 740A0000            MOV ECX, DWORD PTR DS:[ESI+0xA74]               ; PathOfEx.00650075
$+A0     0067D2E4       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+A7     0067D2EB       0F85 74040000            JNZ PathOfEx.0067D765
$+AD     0067D2F1       8B8E 780A0000            MOV ECX, DWORD PTR DS:[ESI+0xA78]               ; PathOfEx.006A005F
$+B3     0067D2F7       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+BA     0067D2FE       0F85 61040000            JNZ PathOfEx.0067D765
$+C0     0067D304       8B8E 980A0000            MOV ECX, DWORD PTR DS:[ESI+0xA98]               ; PathOfEx.00690072
$+C6     0067D30A       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+CD     0067D311       74 11                    JE SHORT PathOfEx.0067D324
$+CF     0067D313       E8 489BFCFF              CALL PathOfEx.00646E60
$+D4     0067D318       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+D7     0067D31B       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+D8     0067D31C       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+D9     0067D31D       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+DA     0067D31E       8BE5                     MOV ESP, EBP
$+DC     0067D320       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+DD     0067D321       C2 0800                  RETN 0x8
$+E0     0067D324       8B8E 7C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA7C]               ; PathOfEx.00770065
$+E6     0067D32A       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+ED     0067D331       0F85 2E040000            JNZ PathOfEx.0067D765
$+F3     0067D337       8B8E 800A0000            MOV ECX, DWORD PTR DS:[ESI+0xA80]               ; PathOfEx.006C0065
$+F9     0067D33D       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+100    0067D344       0F85 1B040000            JNZ PathOfEx.0067D765
$+106    0067D34A       8B8E 840A0000            MOV ECX, DWORD PTR DS:[ESI+0xA84]               ; PathOfEx.0064005F
$+10C    0067D350       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+113    0067D357       0F85 08040000            JNZ PathOfEx.0067D765
$+119    0067D35D       8B8E 880A0000            MOV ECX, DWORD PTR DS:[ESI+0xA88]               ; PathOfEx.0075006F
$+11F    0067D363       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+126    0067D36A       0F85 F5030000            JNZ PathOfEx.0067D765
$+12C    0067D370       8B8E 8C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA8C]               ; PathOfEx.006C0062
$+132    0067D376       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+139    0067D37D       0F85 E2030000            JNZ PathOfEx.0067D765
$+13F    0067D383       8B8E 900A0000            MOV ECX, DWORD PTR DS:[ESI+0xA90]               ; PathOfEx.005F0065
$+145    0067D389       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+14C    0067D390       0F85 CF030000            JNZ PathOfEx.0067D765
$+152    0067D396       8B8E B80A0000            MOV ECX, DWORD PTR DS:[ESI+0xAB8]               ; PathOfEx.00690072
$+158    0067D39C       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+15F    0067D3A3       0F85 BC030000            JNZ PathOfEx.0067D765
$+165    0067D3A9       8B8E 880B0000            MOV ECX, DWORD PTR DS:[ESI+0xB88]
$+16B    0067D3AF       E9 A8030000              JMP PathOfEx.0067D75C
$+170    0067D3B4       807F 08 00               CMP BYTE PTR DS:[EDI+0x8], 0x0
$+174    0067D3B8       0F85 B1030000            JNZ PathOfEx.0067D76F
$+17A    0067D3BE       8B8E 64090000            MOV ECX, DWORD PTR DS:[ESI+0x964]
$+180    0067D3C4       8B81 38090000            MOV EAX, DWORD PTR DS:[ECX+0x938]
$+186    0067D3CA       83F8 02                  CMP EAX, 0x2
$+189    0067D3CD       0F84 42030000            JE PathOfEx.0067D715
$+18F    0067D3D3       83F8 03                  CMP EAX, 0x3
$+192    0067D3D6       0F84 39030000            JE PathOfEx.0067D715
$+198    0067D3DC       83F8 04                  CMP EAX, 0x4
$+19B    0067D3DF       0F84 30030000            JE PathOfEx.0067D715
$+1A1    0067D3E5       83F8 05                  CMP EAX, 0x5
$+1A4    0067D3E8       0F84 27030000            JE PathOfEx.0067D715
$+1AA    0067D3EE       83F8 06                  CMP EAX, 0x6
$+1AD    0067D3F1       0F84 1E030000            JE PathOfEx.0067D715
$+1B3    0067D3F7       8B8E FC080000            MOV ECX, DWORD PTR DS:[ESI+0x8FC]
$+1B9    0067D3FD       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+1C0    0067D404       75 0F                    JNZ SHORT PathOfEx.0067D415
$+1C2    0067D406       8B8E 6C090000            MOV ECX, DWORD PTR DS:[ESI+0x96C]               ; PathOfEx.00610063
$+1C8    0067D40C       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+1CF    0067D413       74 10                    JE SHORT PathOfEx.0067D425
$+1D1    0067D415       6A 00                    PUSH 0x0
$+1D3    0067D417       E8 64E03600              CALL PathOfEx.009EB480
$+1D8    0067D41C       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+1D9    0067D41D       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+1DA    0067D41E       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+1DB    0067D41F       8BE5                     MOV ESP, EBP
$+1DD    0067D421       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+1DE    0067D422       C2 0800                  RETN 0x8
$+1E1    0067D425       8B9E B40A0000            MOV EBX, DWORD PTR DS:[ESI+0xAB4]               ; PathOfEx.0074005F
$+1E7    0067D42B       85DB                     TEST EBX, EBX
$+1E9    0067D42D       74 5F                    JE SHORT PathOfEx.0067D48E
$+1EB    0067D42F       8B86 A8090000            MOV EAX, DWORD PTR DS:[ESI+0x9A8]               ; PathOfEx.0073006F
$+1F1    0067D435       894424 10                MOV DWORD PTR SS:[ESP+0x10], EAX
$+1F5    0067D439       80B8 380A0000 00         CMP BYTE PTR DS:[EAX+0xA38], 0x0
$+1FC    0067D440       74 34                    JE SHORT PathOfEx.0067D476
$+1FE    0067D442       8DBE 04070000            LEA EDI, DWORD PTR DS:[ESI+0x704]
$+204    0067D448       50                       PUSH EAX
$+205    0067D449       8BCF                     MOV ECX, EDI
$+207    0067D44B       E8 7055E9FF              CALL PathOfEx.005129C0
$+20C    0067D450       53                       PUSH EBX
$+20D    0067D451       8BCF                     MOV ECX, EDI
$+20F    0067D453       8BF0                     MOV ESI, EAX
$+211    0067D455       E8 6655E9FF              CALL PathOfEx.005129C0
$+216    0067D45A       3BF0                     CMP ESI, EAX
$+218    0067D45C       76 18                    JBE SHORT PathOfEx.0067D476
$+21A    0067D45E       8B4C24 10                MOV ECX, DWORD PTR SS:[ESP+0x10]
$+21E    0067D462       E8 39D6F0FF              CALL PathOfEx.0058AAA0
$+223    0067D467       8B45 0C                  MOV EAX, DWORD PTR SS:[EBP+0xC]
$+226    0067D46A       C600 01                  MOV BYTE PTR DS:[EAX], 0x1
$+229    0067D46D       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+22A    0067D46E       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+22B    0067D46F       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+22C    0067D470       8BE5                     MOV ESP, EBP
$+22E    0067D472       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+22F    0067D473       C2 0800                  RETN 0x8
$+232    0067D476       6A 00                    PUSH 0x0
$+234    0067D478       8BCB                     MOV ECX, EBX
$+236    0067D47A       E8 01E03600              CALL PathOfEx.009EB480
$+23B    0067D47F       8B45 0C                  MOV EAX, DWORD PTR SS:[EBP+0xC]
$+23E    0067D482       C600 01                  MOV BYTE PTR DS:[EAX], 0x1
$+241    0067D485       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+242    0067D486       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+243    0067D487       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+244    0067D488       8BE5                     MOV ESP, EBP
$+246    0067D48A       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+247    0067D48B       C2 0800                  RETN 0x8
$+24A    0067D48E       8B8E E80A0000            MOV ECX, DWORD PTR DS:[ESI+0xAE8]               ; PathOfEx.005F006E
$+250    0067D494       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+257    0067D49B       74 1F                    JE SHORT PathOfEx.0067D4BC
$+259    0067D49D       8B89 14090000            MOV ECX, DWORD PTR DS:[ECX+0x914]
$+25F    0067D4A3       85C9                     TEST ECX, ECX
$+261    0067D4A5       0F84 CD020000            JE PathOfEx.0067D778
$+267    0067D4AB       8B01                     MOV EAX, DWORD PTR DS:[ECX]
$+269    0067D4AD       FF50 08                  CALL NEAR DWORD PTR DS:[EAX+0x8]
$+26C    0067D4B0       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+26F    0067D4B3       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+270    0067D4B4       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+271    0067D4B5       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+272    0067D4B6       8BE5                     MOV ESP, EBP
$+274    0067D4B8       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+275    0067D4B9       C2 0800                  RETN 0x8
$+278    0067D4BC       8B8E A8090000            MOV ECX, DWORD PTR DS:[ESI+0x9A8]               ; PathOfEx.0073006F
$+27E    0067D4C2       80B9 380A0000 00         CMP BYTE PTR DS:[ECX+0xA38], 0x0
$+285    0067D4C9       74 11                    JE SHORT PathOfEx.0067D4DC
$+287    0067D4CB       E8 D0D5F0FF              CALL PathOfEx.0058AAA0
$+28C    0067D4D0       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+28F    0067D4D3       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+290    0067D4D4       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+291    0067D4D5       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+292    0067D4D6       8BE5                     MOV ESP, EBP
$+294    0067D4D8       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+295    0067D4D9       C2 0800                  RETN 0x8
$+298    0067D4DC       8B8E 940A0000            MOV ECX, DWORD PTR DS:[ESI+0xA94]               ; PathOfEx.00740073
$+29E    0067D4E2       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+2A5    0067D4E9       0F85 76020000            JNZ PathOfEx.0067D765
$+2AB    0067D4EF       8B8E 8C090000            MOV ECX, DWORD PTR DS:[ESI+0x98C]               ; PathOfEx.0066005F
$+2B1    0067D4F5       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+2B8    0067D4FC       74 22                    JE SHORT PathOfEx.0067D520
$+2BA    0067D4FE       F3:0F1081 F4080000       MOVSS XMM0, DWORD PTR DS:[ECX+0x8F4]
$+2C2    0067D506       0F2F05 40C9F300          COMISS XMM0, DWORD PTR DS:[0xF3C940]
$+2C9    0067D50D       77 11                    JA SHORT PathOfEx.0067D520
$+2CB    0067D50F       E8 0C8BFDFF              CALL PathOfEx.00656020
$+2D0    0067D514       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+2D3    0067D517       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+2D4    0067D518       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+2D5    0067D519       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+2D6    0067D51A       8BE5                     MOV ESP, EBP
$+2D8    0067D51C       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+2D9    0067D51D       C2 0800                  RETN 0x8
$+2DC    0067D520       8B9E B80A0000            MOV EBX, DWORD PTR DS:[ESI+0xAB8]               ; PathOfEx.00690072
$+2E2    0067D526       8BCB                     MOV ECX, EBX
$+2E4    0067D528       E8 43E03600              CALL PathOfEx.009EB570
$+2E9    0067D52D       84C0                     TEST AL, AL
$+2EB    0067D52F       74 15                    JE SHORT PathOfEx.0067D546
$+2ED    0067D531       8BCB                     MOV ECX, EBX
$+2EF    0067D533       6A 00                    PUSH 0x0
$+2F1    0067D535       E8 46DF3600              CALL PathOfEx.009EB480
$+2F6    0067D53A       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+2F9    0067D53D       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+2FA    0067D53E       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+2FB    0067D53F       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+2FC    0067D540       8BE5                     MOV ESP, EBP
$+2FE    0067D542       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+2FF    0067D543       C2 0800                  RETN 0x8
$+302    0067D546       8B9E A80A0000            MOV EBX, DWORD PTR DS:[ESI+0xAA8]               ; PathOfEx.0063006E
$+308    0067D54C       8BCB                     MOV ECX, EBX
$+30A    0067D54E       E8 1DE03600              CALL PathOfEx.009EB570
$+30F    0067D553       84C0                     TEST AL, AL
$+311    0067D555     ^ 75 DA                    JNZ SHORT PathOfEx.0067D531
$+313    0067D557       8B9E AC0A0000            MOV EBX, DWORD PTR DS:[ESI+0xAAC]               ; PathOfEx.005F0065
$+319    0067D55D       8BCB                     MOV ECX, EBX
$+31B    0067D55F       E8 0CE03600              CALL PathOfEx.009EB570
$+320    0067D564       84C0                     TEST AL, AL
$+322    0067D566     ^ 75 C9                    JNZ SHORT PathOfEx.0067D531
$+324    0067D568       8B9E B00A0000            MOV EBX, DWORD PTR DS:[ESI+0xAB0]               ; PathOfEx.006F0074
$+32A    0067D56E       8BCB                     MOV ECX, EBX
$+32C    0067D570       E8 FBDF3600              CALL PathOfEx.009EB570
$+331    0067D575       84C0                     TEST AL, AL
$+333    0067D577     ^ 75 B8                    JNZ SHORT PathOfEx.0067D531
$+335    0067D579       8B86 5C0A0000            MOV EAX, DWORD PTR DS:[ESI+0xA5C]
$+33B    0067D57F       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$+342    0067D586       74 1B                    JE SHORT PathOfEx.0067D5A3
$+344    0067D588       8B8E BC080000            MOV ECX, DWORD PTR DS:[ESI+0x8BC]
$+34A    0067D58E       6A 00                    PUSH 0x0
$+34C    0067D590       6A 00                    PUSH 0x0
$+34E    0067D592       E8 69A12500              CALL PathOfEx.008D7700
$+353    0067D597       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+356    0067D59A       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+357    0067D59B       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+358    0067D59C       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+359    0067D59D       8BE5                     MOV ESP, EBP
$+35B    0067D59F       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+35C    0067D5A0       C2 0800                  RETN 0x8
$+35F    0067D5A3       8B8E 640A0000            MOV ECX, DWORD PTR DS:[ESI+0xA64]               ; PathOfEx.00610063
$+365    0067D5A9       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+36C    0067D5B0       0F85 AF010000            JNZ PathOfEx.0067D765
$+372    0067D5B6       8B86 680A0000            MOV EAX, DWORD PTR DS:[ESI+0xA68]               ; PathOfEx.005F006C
$+378    0067D5BC       894424 10                MOV DWORD PTR SS:[ESP+0x10], EAX
$+37C    0067D5C0       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$+383    0067D5C7     ^ 0F85 AEFCFFFF            JNZ PathOfEx.0067D27B
$+389    0067D5CD       8B8E 6C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA6C]               ; PathOfEx.006E0075
$+38F    0067D5D3       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+396    0067D5DA       0F85 85010000            JNZ PathOfEx.0067D765
$+39C    0067D5E0       8B8E 700A0000            MOV ECX, DWORD PTR DS:[ESI+0xA70]               ; PathOfEx.00710069
$+3A2    0067D5E6       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+3A9    0067D5ED       0F85 72010000            JNZ PathOfEx.0067D765
$+3AF    0067D5F3       8B8E 740A0000            MOV ECX, DWORD PTR DS:[ESI+0xA74]               ; PathOfEx.00650075
$+3B5    0067D5F9       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+3BC    0067D600       0F85 5F010000            JNZ PathOfEx.0067D765
$+3C2    0067D606       8B8E 780A0000            MOV ECX, DWORD PTR DS:[ESI+0xA78]               ; PathOfEx.006A005F
$+3C8    0067D60C       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+3CF    0067D613       0F85 4C010000            JNZ PathOfEx.0067D765
$+3D5    0067D619       8B8E 980A0000            MOV ECX, DWORD PTR DS:[ESI+0xA98]               ; PathOfEx.00690072
$+3DB    0067D61F       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+3E2    0067D626     ^ 0F85 E7FCFFFF            JNZ PathOfEx.0067D313
$+3E8    0067D62C       8B8E 7C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA7C]               ; PathOfEx.00770065
$+3EE    0067D632       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+3F5    0067D639       0F85 26010000            JNZ PathOfEx.0067D765
$+3FB    0067D63F       8B8E 800A0000            MOV ECX, DWORD PTR DS:[ESI+0xA80]               ; PathOfEx.006C0065
$+401    0067D645       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+408    0067D64C       0F85 13010000            JNZ PathOfEx.0067D765
$+40E    0067D652       8B8E 840A0000            MOV ECX, DWORD PTR DS:[ESI+0xA84]               ; PathOfEx.0064005F
$+414    0067D658       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+41B    0067D65F       0F85 00010000            JNZ PathOfEx.0067D765
$+421    0067D665       8B8E 880A0000            MOV ECX, DWORD PTR DS:[ESI+0xA88]               ; PathOfEx.0075006F
$+427    0067D66B       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+42E    0067D672       0F85 ED000000            JNZ PathOfEx.0067D765
$+434    0067D678       8B8E 8C0A0000            MOV ECX, DWORD PTR DS:[ESI+0xA8C]               ; PathOfEx.006C0062
$+43A    0067D67E       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+441    0067D685       0F85 DA000000            JNZ PathOfEx.0067D765
$+447    0067D68B       8B8E 900A0000            MOV ECX, DWORD PTR DS:[ESI+0xA90]               ; PathOfEx.005F0065
$+44D    0067D691       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+454    0067D698       0F85 C7000000            JNZ PathOfEx.0067D765
$+45A    0067D69E       83BE EC090000 00         CMP DWORD PTR DS:[ESI+0x9EC], 0x0
$+461    0067D6A5       75 5B                    JNZ SHORT PathOfEx.0067D702
$+463    0067D6A7       83BE F0090000 00         CMP DWORD PTR DS:[ESI+0x9F0], 0x0
$+46A    0067D6AE       75 52                    JNZ SHORT PathOfEx.0067D702
$+46C    0067D6B0       8B86 180A0000            MOV EAX, DWORD PTR DS:[ESI+0xA18]               ; PathOfEx.0067006E
$+472    0067D6B6       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$+479    0067D6BD       75 43                    JNZ SHORT PathOfEx.0067D702
$+47B    0067D6BF       8B86 1C0A0000            MOV EAX, DWORD PTR DS:[ESI+0xA1C]               ; PathOfEx.005F0065
$+481    0067D6C5       80B8 54070000 00         CMP BYTE PTR DS:[EAX+0x754], 0x0
$+488    0067D6CC       75 34                    JNZ SHORT PathOfEx.0067D702
$+48A    0067D6CE       8BCE                     MOV ECX, ESI
$+48C    0067D6D0       E8 0B250000              CALL PathOfEx.0067FBE0
$+491    0067D6D5       84C0                     TEST AL, AL
$+493    0067D6D7     ^ 0F85 63FBFFFF            JNZ PathOfEx.0067D240
$+499    0067D6DD       8B8E 880B0000            MOV ECX, DWORD PTR DS:[ESI+0xB88]
$+49F    0067D6E3       3881 54070000            CMP BYTE PTR DS:[ECX+0x754], AL
$+4A5    0067D6E9       75 7A                    JNZ SHORT PathOfEx.0067D765
$+4A7    0067D6EB       8B8E C4080000            MOV ECX, DWORD PTR DS:[ESI+0x8C4]                ; 存放的是stCD_SceneInGameState*,g_o_main_ui_get_in_game_state_
$+4AD    0067D6F1       E8 3A3F0500              CALL PathOfEx.006D1630                          ; tag_从InGameState置顶到EscapeState
$+4B2    0067D6F6       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+4B5    0067D6F9       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+4B6    0067D6FA       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+4B7    0067D6FB       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+4B8    0067D6FC       8BE5                     MOV ESP, EBP
$+4BA    0067D6FE       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+4BB    0067D6FF       C2 0800                  RETN 0x8
$+4BE    0067D702       8BCE                     MOV ECX, ESI
$+4C0    0067D704       E8 C70E0000              CALL PathOfEx.0067E5D0
$+4C5    0067D709       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+4C8    0067D70C       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+4C9    0067D70D       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+4CA    0067D70E       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+4CB    0067D70F       8BE5                     MOV ESP, EBP
$+4CD    0067D711       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+4CE    0067D712       C2 0800                  RETN 0x8
$+4D1    0067D715       E8 76B6F1FF              CALL PathOfEx.00598D90
$+4D6    0067D71A       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+4D9    0067D71D       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+4DA    0067D71E       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+4DB    0067D71F       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+4DC    0067D720       8BE5                     MOV ESP, EBP
$+4DE    0067D722       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+4DF    0067D723       C2 0800                  RETN 0x8
$+4E2    0067D726       807F 08 00               CMP BYTE PTR DS:[EDI+0x8], 0x0
$+4E6    0067D72A       75 43                    JNZ SHORT PathOfEx.0067D76F
$+4E8    0067D72C       8B9E C00A0000            MOV EBX, DWORD PTR DS:[ESI+0xAC0]               ; PathOfEx.00720065
$+4EE    0067D732       8BCB                     MOV ECX, EBX
$+4F0    0067D734       E8 37DE3600              CALL PathOfEx.009EB570
$+4F5    0067D739       84C0                     TEST AL, AL
$+4F7    0067D73B       74 19                    JE SHORT PathOfEx.0067D756
$+4F9    0067D73D       FFB3 D0080000            PUSH DWORD PTR DS:[EBX+0x8D0]
$+4FF    0067D743       8BCB                     MOV ECX, EBX
$+501    0067D745       E8 6626FCFF              CALL PathOfEx.0063FDB0
$+506    0067D74A       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+509    0067D74D       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+50A    0067D74E       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+50B    0067D74F       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+50C    0067D750       8BE5                     MOV ESP, EBP
$+50E    0067D752       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+50F    0067D753       C2 0800                  RETN 0x8
$+512    0067D756       8B8E B80A0000            MOV ECX, DWORD PTR DS:[ESI+0xAB8]               ; PathOfEx.00690072
$+518    0067D75C       80B9 54070000 00         CMP BYTE PTR DS:[ECX+0x754], 0x0
$+51F    0067D763       74 0A                    JE SHORT PathOfEx.0067D76F
$+521    0067D765       6A 00                    PUSH 0x0
$+523    0067D767       E8 14DD3600              CALL PathOfEx.009EB480
$+528    0067D76C       C607 01                  MOV BYTE PTR DS:[EDI], 0x1
$+52B    0067D76F       5F                       POP EDI                                         ; ntdll_1z.777AF8B1
$+52C    0067D770       5E                       POP ESI                                         ; ntdll_1z.777AF8B1
$+52D    0067D771       5B                       POP EBX                                         ; ntdll_1z.777AF8B1
$+52E    0067D772       8BE5                     MOV ESP, EBP
$+530    0067D774       5D                       POP EBP                                         ; ntdll_1z.777AF8B1
$+531    0067D775       C2 0800                  RETN 0x8
$+534    0067D778       E8 E0A15E00              CALL PathOfEx.00C6795D
$+539    0067D77D       0F1F00                   NOP DWORD PTR DS:[EAX]
$+53C    0067D780       A7                       CMPS DWORD PTR DS:[ESI], DWORD PTR ES:[EDI]
$+53D    0067D781       D167 00                  SHL DWORD PTR DS:[EDI], 1
$+540    0067D784       93                       XCHG EAX, EBX
$+541    0067D785       D167 00                  SHL DWORD PTR DS:[EDI], 1
$+544    0067D788       B1 D1                    MOV CL, 0xD1
$+546    0067D78A       67:009D D167             ADD BYTE PTR DS:[DI+0x67D1], BL
$+54B    0067D78F       0051 8B                  ADD BYTE PTR DS:[ECX-0x75], DL
$+54E    0067D792       44                       INC ESP
$+54F    0067D793       24 0C                    AND AL, 0xC
$+551    0067D795       8038 00                  CMP BYTE PTR DS:[EAX], 0x0
$+554    0067D798       75 41                    JNZ SHORT PathOfEx.0067D7DB
$+556    0067D79A       8B89 64090000            MOV ECX, DWORD PTR DS:[ECX+0x964]
$+55C    0067D7A0       85C9                     TEST ECX, ECX
$+55E    0067D7A2       74 12                    JE SHORT PathOfEx.0067D7B6
$+560    0067D7A4       83B9 38090000 04         CMP DWORD PTR DS:[ECX+0x938], 0x4
$+567    0067D7AB       74 2E                    JE SHORT PathOfEx.0067D7DB
$+569    0067D7AD       80B9 78090000 00         CMP BYTE PTR DS:[ECX+0x978], 0x0
$+570    0067D7B4       75 25                    JNZ SHORT PathOfEx.0067D7DB
$+572    0067D7B6       83B9 34090000 00         CMP DWORD PTR DS:[ECX+0x934], 0x0
$+579    0067D7BD       74 1C                    JE SHORT PathOfEx.0067D7DB
$+57B    0067D7BF       83B9 38090000 01         CMP DWORD PTR DS:[ECX+0x938], 0x1
$+582    0067D7C6       74 13                    JE SHORT PathOfEx.0067D7DB
$+584    0067D7C8       6A FF                    PUSH -0x1
$+586    0067D7CA       6A 00                    PUSH 0x0
$+588    0067D7CC       6A 00                    PUSH 0x0
$+58A    0067D7CE       6A 00                    PUSH 0x0
$+58C    0067D7D0       6A 00                    PUSH 0x0
$+58E    0067D7D2       6A 00                    PUSH 0x0
$+590    0067D7D4       6A 00                    PUSH 0x0
$+592    0067D7D6       E8 55B0F1FF              CALL PathOfEx.00598830
$+597    0067D7DB       59                       POP ECX                                         ; ntdll_1z.777AF8B1
$+598    0067D7DC       C2 0800                  RETN 0x8
$+59B    0067D7DF       CC                       INT3
*/
/**/
//tag_当选择了EscapeMenu中的某一项的时候会断到
/*
$-311    006C5D7F     CC                      INT3
$-310    006C5D80     55                      PUSH EBP                                             ; tag_创建EscapeMenuInner
$-30F    006C5D81     8BEC                    MOV EBP,ESP                                          ; ecx=stCD_EscapeState*
$-30D    006C5D83     6A FF                   PUSH -0x1
$-30B    006C5D85     68 381AD200             PUSH PathOfEx.00D21A38                               ; ASCII "胳唪"
$-306    006C5D8A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-300    006C5D90     50                      PUSH EAX
$-2FF    006C5D91     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-2F8    006C5D98     83EC 28                 SUB ESP,0x28
$-2F5    006C5D9B     56                      PUSH ESI
$-2F4    006C5D9C     57                      PUSH EDI
$-2F3    006C5D9D     6A 2C                   PUSH 0x2C
$-2F1    006C5D9F     8BF9                    MOV EDI,ECX
$-2EF    006C5DA1     C745 F0 07000000        MOV DWORD PTR SS:[EBP-0x10],0x7
$-2E8    006C5DA8     33C0                    XOR EAX,EAX
$-2E6    006C5DAA     C745 EC 00000000        MOV DWORD PTR SS:[EBP-0x14],0x0
$-2DF    006C5DB1     68 88A7EA00             PUSH PathOfEx.00EAA788                               ; UNICODE "Metadata/UI/InGameState/TencentEscapeMenu.ui"
$-2DA    006C5DB6     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
$-2D7    006C5DB9     66:8945 DC              MOV WORD PTR SS:[EBP-0x24],AX
$-2D3    006C5DBD     E8 EE7EDAFF             CALL PathOfEx.0046DCB0
$-2CE    006C5DC2     8D45 DC                 LEA EAX,DWORD PTR SS:[EBP-0x24]
$-2CB    006C5DC5     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$-2C4    006C5DCC     50                      PUSH EAX
$-2C3    006C5DCD     8D4F 20                 LEA ECX,DWORD PTR DS:[EDI+0x20]
$-2C0    006C5DD0     E8 1B183500             CALL PathOfEx.00A175F0
$-2BB    006C5DD5     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$-2B4    006C5DDC     8B45 F0                 MOV EAX,DWORD PTR SS:[EBP-0x10]
$-2B1    006C5DDF     83F8 08                 CMP EAX,0x8
$-2AE    006C5DE2     72 0F                   JB SHORT PathOfEx.006C5DF3
$-2AC    006C5DE4     6A 02                   PUSH 0x2
$-2AA    006C5DE6     40                      INC EAX
$-2A9    006C5DE7     50                      PUSH EAX
$-2A8    006C5DE8     FF75 DC                 PUSH DWORD PTR SS:[EBP-0x24]
$-2A5    006C5DEB     E8 406ADAFF             CALL PathOfEx.0046C830
$-2A0    006C5DF0     83C4 0C                 ADD ESP,0xC
$-29D    006C5DF3     8B8F C40A0000           MOV ECX,DWORD PTR DS:[EDI+0xAC4]
$-297    006C5DF9     6A 0B                   PUSH 0xB
$-295    006C5DFB     68 74A8EA00             PUSH PathOfEx.00EAA874                               ; UNICODE "escape_menu"
$-290    006C5E00     81C1 04070000           ADD ECX,0x704
$-28A    006C5E06     E8 55DD3300             CALL PathOfEx.00A03B60
$-285    006C5E0B     6A 11                   PUSH 0x11
$-283    006C5E0D     68 50A8EA00             PUSH PathOfEx.00EAA850                               ; UNICODE "escape_menu_inner"
$-27E    006C5E12     8987 3C0B0000           MOV DWORD PTR DS:[EDI+0xB3C],EAX
$-278    006C5E18     8D88 04070000           LEA ECX,DWORD PTR DS:[EAX+0x704]
$-272    006C5E1E     E8 3DDD3300             CALL PathOfEx.00A03B60
$-26D    006C5E23     6A 0D                   PUSH 0xD
$-26B    006C5E25     68 34A8EA00             PUSH PathOfEx.00EAA834                               ; UNICODE "exit_to_login"
$-266    006C5E2A     8DB0 04070000           LEA ESI,DWORD PTR DS:[EAX+0x704]
$-260    006C5E30     8BCE                    MOV ECX,ESI                                          ; esi=stCD_UiEscapeMenuInner*
$-25E    006C5E32     E8 29DD3300             CALL PathOfEx.00A03B60
$-259    006C5E37     6A 1B                   PUSH 0x1B
$-257    006C5E39     68 FCA7EA00             PUSH PathOfEx.00EAA7FC                               ; UNICODE "exit_to_character_selection"
$-252    006C5E3E     8BCE                    MOV ECX,ESI
$-250    006C5E40     8987 400B0000           MOV DWORD PTR DS:[EDI+0xB40],EAX
$-24A    006C5E46     E8 15DD3300             CALL PathOfEx.00A03B60
$-245    006C5E4B     6A 09                   PUSH 0x9
$-243    006C5E4D     68 D8A8EA00             PUSH PathOfEx.00EAA8D8                               ; UNICODE "exit_game"
$-23E    006C5E52     8BCE                    MOV ECX,ESI
$-23C    006C5E54     8987 440B0000           MOV DWORD PTR DS:[EDI+0xB44],EAX
$-236    006C5E5A     E8 01DD3300             CALL PathOfEx.00A03B60
$-231    006C5E5F     6A 07                   PUSH 0x7
$-22F    006C5E61     68 C8A8EA00             PUSH PathOfEx.00EAA8C8                               ; UNICODE "options"
$-22A    006C5E66     8BCE                    MOV ECX,ESI
$-228    006C5E68     8987 480B0000           MOV DWORD PTR DS:[EDI+0xB48],EAX
$-222    006C5E6E     E8 EDDC3300             CALL PathOfEx.00A03B60
$-21D    006C5E73     6A 0B                   PUSH 0xB
$-21B    006C5E75     68 B0A8EA00             PUSH PathOfEx.00EAA8B0                               ; UNICODE "resume_game"
$-216    006C5E7A     8BCE                    MOV ECX,ESI
$-214    006C5E7C     8987 4C0B0000           MOV DWORD PTR DS:[EDI+0xB4C],EAX
$-20E    006C5E82     E8 D9DC3300             CALL PathOfEx.00A03B60
$-209    006C5E87     6A 11                   PUSH 0x11
$-207    006C5E89     68 8CA8EA00             PUSH PathOfEx.00EAA88C                               ; UNICODE "option_shop_panel"
$-202    006C5E8E     8BCE                    MOV ECX,ESI
$-200    006C5E90     8987 500B0000           MOV DWORD PTR DS:[EDI+0xB50],EAX
$-1FA    006C5E96     E8 C5DC3300             CALL PathOfEx.00A03B60
$-1F5    006C5E9B     6A 08                   PUSH 0x8
$-1F3    006C5E9D     68 9CA9EA00             PUSH PathOfEx.00EAA99C                               ; UNICODE "tw_forum"
$-1EE    006C5EA2     8BCE                    MOV ECX,ESI
$-1EC    006C5EA4     8987 540B0000           MOV DWORD PTR DS:[EDI+0xB54],EAX
$-1E6    006C5EAA     E8 B1DC3300             CALL PathOfEx.00A03B60
$-1E1    006C5EAF     6A 0A                   PUSH 0xA
$-1DF    006C5EB1     68 84A9EA00             PUSH PathOfEx.00EAA984                               ; UNICODE "tw_website"
$-1DA    006C5EB6     8BCE                    MOV ECX,ESI
$-1D8    006C5EB8     8987 580B0000           MOV DWORD PTR DS:[EDI+0xB58],EAX
$-1D2    006C5EBE     E8 9DDC3300             CALL PathOfEx.00A03B60
$-1CD    006C5EC3     8B8F 400B0000           MOV ECX,DWORD PTR DS:[EDI+0xB40]
$-1C7    006C5EC9     8987 5C0B0000           MOV DWORD PTR DS:[EDI+0xB5C],EAX
$-1C1    006C5ECF     85C9                    TEST ECX,ECX
$-1BF    006C5ED1     74 27                   JE SHORT PathOfEx.006C5EFA
$-1BD    006C5ED3     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-1BA    006C5ED6     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-1B3    006C5EDD     50                      PUSH EAX
$-1B2    006C5EDE     81C1 B4080000           ADD ECX,0x8B4
$-1AC    006C5EE4     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-1A9    006C5EE7     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-1A2    006C5EEE     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-19B    006C5EF5     E8 C688DDFF             CALL PathOfEx.0049E7C0
$-196    006C5EFA     8B8F 440B0000           MOV ECX,DWORD PTR DS:[EDI+0xB44]
$-190    006C5F00     85C9                    TEST ECX,ECX
$-18E    006C5F02     74 27                   JE SHORT PathOfEx.006C5F2B
$-18C    006C5F04     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-189    006C5F07     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-182    006C5F0E     50                      PUSH EAX
$-181    006C5F0F     81C1 B4080000           ADD ECX,0x8B4
$-17B    006C5F15     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-178    006C5F18     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-171    006C5F1F     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-16A    006C5F26     E8 9588DDFF             CALL PathOfEx.0049E7C0
$-165    006C5F2B     8B8F 480B0000           MOV ECX,DWORD PTR DS:[EDI+0xB48]
$-15F    006C5F31     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-15C    006C5F34     50                      PUSH EAX
$-15B    006C5F35     81C1 B4080000           ADD ECX,0x8B4
$-155    006C5F3B     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-14E    006C5F42     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-14B    006C5F45     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-144    006C5F4C     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-13D    006C5F53     E8 6888DDFF             CALL PathOfEx.0049E7C0
$-138    006C5F58     8B8F 4C0B0000           MOV ECX,DWORD PTR DS:[EDI+0xB4C]
$-132    006C5F5E     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-12F    006C5F61     50                      PUSH EAX
$-12E    006C5F62     81C1 B4080000           ADD ECX,0x8B4
$-128    006C5F68     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-121    006C5F6F     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-11E    006C5F72     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-117    006C5F79     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-110    006C5F80     E8 3B88DDFF             CALL PathOfEx.0049E7C0
$-10B    006C5F85     8B8F 500B0000           MOV ECX,DWORD PTR DS:[EDI+0xB50]
$-105    006C5F8B     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-102    006C5F8E     50                      PUSH EAX
$-101    006C5F8F     81C1 B4080000           ADD ECX,0x8B4
$-FB     006C5F95     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-F4     006C5F9C     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-F1     006C5F9F     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-EA     006C5FA6     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-E3     006C5FAD     E8 0E88DDFF             CALL PathOfEx.0049E7C0
$-DE     006C5FB2     8B8F 540B0000           MOV ECX,DWORD PTR DS:[EDI+0xB54]
$-D8     006C5FB8     85C9                    TEST ECX,ECX
$-D6     006C5FBA     74 27                   JE SHORT PathOfEx.006C5FE3
$-D4     006C5FBC     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-D1     006C5FBF     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-CA     006C5FC6     50                      PUSH EAX
$-C9     006C5FC7     81C1 B4080000           ADD ECX,0x8B4
$-C3     006C5FCD     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-C0     006C5FD0     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-B9     006C5FD7     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-B2     006C5FDE     E8 DD87DDFF             CALL PathOfEx.0049E7C0
$-AD     006C5FE3     8B8F 580B0000           MOV ECX,DWORD PTR DS:[EDI+0xB58]
$-A7     006C5FE9     85C9                    TEST ECX,ECX
$-A5     006C5FEB     74 27                   JE SHORT PathOfEx.006C6014
$-A3     006C5FED     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-A0     006C5FF0     C745 E4 382EE700        MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00E72E38
$-99     006C5FF7     50                      PUSH EAX
$-98     006C5FF8     81C1 B4080000           ADD ECX,0x8B4
$-92     006C5FFE     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$-8F     006C6001     C745 EC 90606C00        MOV DWORD PTR SS:[EBP-0x14],PathOfEx.006C6090
$-88     006C6008     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-81     006C600F     E8 AC87DDFF             CALL PathOfEx.0049E7C0
$-7C     006C6014     8B8F 5C0B0000           MOV ECX,DWORD PTR DS:[EDI+0xB5C]
$-76     006C601A     85C9                    TEST ECX,ECX
$-74     006C601C     74 27                   JE SHORT PathOfEx.006C6045
$-72     006C601E     8D45 CC                 LEA EAX,DWORD PTR SS:[EBP-0x34]
$-6F     006C6021     C745 CC 382EE700        MOV DWORD PTR SS:[EBP-0x34],PathOfEx.00E72E38
$-68     006C6028     50                      PUSH EAX
$-67     006C6029     81C1 B4080000           ADD ECX,0x8B4
$-61     006C602F     897D D0                 MOV DWORD PTR SS:[EBP-0x30],EDI
$-5E     006C6032     C745 D4 90606C00        MOV DWORD PTR SS:[EBP-0x2C],PathOfEx.006C6090
$-57     006C6039     C745 D8 00000000        MOV DWORD PTR SS:[EBP-0x28],0x0
$-50     006C6040     E8 7B87DDFF             CALL PathOfEx.0049E7C0
$-4B     006C6045     8BB7 3C0B0000           MOV ESI,DWORD PTR DS:[EDI+0xB3C]
$-45     006C604B     80BE 54070000 01        CMP BYTE PTR DS:[ESI+0x754],0x1
$-3E     006C6052     74 24                   JE SHORT PathOfEx.006C6078
$-3C     006C6054     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$-3A     006C6056     8BCE                    MOV ECX,ESI
$-38     006C6058     C686 54070000 01        MOV BYTE PTR DS:[ESI+0x754],0x1
$-31     006C605F     FF90 B0000000           CALL DWORD PTR DS:[EAX+0xB0]
$-2B     006C6065     8B8E 5C070000           MOV ECX,DWORD PTR DS:[ESI+0x75C]
$-25     006C606B     85C9                    TEST ECX,ECX
$-23     006C606D     74 09                   JE SHORT PathOfEx.006C6078
$-21     006C606F     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-1F     006C6071     56                      PUSH ESI
$-1E     006C6072     FF90 D0000000           CALL DWORD PTR DS:[EAX+0xD0]
$-18     006C6078     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-15     006C607B     5F                      POP EDI
$-14     006C607C     5E                      POP ESI
$-13     006C607D     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-C      006C6084     8BE5                    MOV ESP,EBP
$-A      006C6086     5D                      POP EBP
$-9      006C6087     C3                      RETN
$-8      006C6088     CC                      INT3
$-7      006C6089     CC                      INT3
$-6      006C608A     CC                      INT3
$-5      006C608B     CC                      INT3
$-4      006C608C     CC                      INT3
$-3      006C608D     CC                      INT3
$-2      006C608E     CC                      INT3								;有1个参数,p1=子ui对象
$-1      006C608F     CC                      INT3								;子ui对象也就是按钮对象
$ ==>    006C6090     55                      PUSH EBP                                             ; tag_当选择了EscapeMenu中的某一项的时候会断到
$+1      006C6091     8BEC                    MOV EBP,ESP                                          ; ecx=stCD_SceneEscapeState*
$+3      006C6093     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+9      006C6099     6A FF                   PUSH -0x1
$+B      006C609B     68 C461D200             PUSH PathOfEx.00D261C4
$+10     006C60A0     50                      PUSH EAX
$+11     006C60A1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     006C60A8     83EC 10                 SUB ESP,0x10
$+1B     006C60AB     56                      PUSH ESI
$+1C     006C60AC     8BF1                    MOV ESI,ECX
$+1E     006C60AE     80BE 640B0000 00        CMP BYTE PTR DS:[ESI+0xB64],0x0
$+25     006C60B5     0F85 1A010000           JNZ PathOfEx.006C61D5
$+2B     006C60BB     53                      PUSH EBX
$+2C     006C60BC     57                      PUSH EDI
$+2D     006C60BD     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+30     006C60C0     E8 7B76DBFF             CALL PathOfEx.0047D740
$+35     006C60C5     51                      PUSH ECX
$+36     006C60C6     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
$+39     006C60C9     51                      PUSH ECX
$+3A     006C60CA     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$+3C     006C60CC     E8 BFE1DBFF             CALL PathOfEx.00484290
$+41     006C60D1     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+48     006C60D8     83CB FF                 OR EBX,0xFFFFFFFF
$+4B     006C60DB     8B7D E8                 MOV EDI,DWORD PTR SS:[EBP-0x18]
$+4E     006C60DE     85FF                    TEST EDI,EDI
$+50     006C60E0     74 1F                   JE SHORT PathOfEx.006C6101
$+52     006C60E2     8BC3                    MOV EAX,EBX
$+54     006C60E4     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+59     006C60E9     75 16                   JNZ SHORT PathOfEx.006C6101
$+5B     006C60EB     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+5D     006C60ED     8BCF                    MOV ECX,EDI
$+5F     006C60EF     FF10                    CALL DWORD PTR DS:[EAX]
$+61     006C60F1     8BC3                    MOV EAX,EBX
$+63     006C60F3     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$+68     006C60F8     75 07                   JNZ SHORT PathOfEx.006C6101
$+6A     006C60FA     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+6C     006C60FC     8BCF                    MOV ECX,EDI
$+6E     006C60FE     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+71     006C6101     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$+74     006C6104     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
$+78     006C6108     3B86 400B0000           CMP EAX,DWORD PTR DS:[ESI+0xB40]
$+7E     006C610E     75 10                   JNZ SHORT PathOfEx.006C6120
$+80     006C6110     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+83     006C6113     8BCE                    MOV ECX,ESI
$+85     006C6115     50                      PUSH EAX
$+86     006C6116     E8 D5060000             CALL PathOfEx.006C67F0
$+8B     006C611B     E9 87000000             JMP PathOfEx.006C61A7
$+90     006C6120     3B86 440B0000           CMP EAX,DWORD PTR DS:[ESI+0xB44]
$+96     006C6126     75 0D                   JNZ SHORT PathOfEx.006C6135
$+98     006C6128     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+9B     006C612B     8BCE                    MOV ECX,ESI
$+9D     006C612D     50                      PUSH EAX
$+9E     006C612E     E8 5D070000             CALL PathOfEx.006C6890
$+A3     006C6133     EB 72                   JMP SHORT PathOfEx.006C61A7
$+A5     006C6135     3B86 480B0000           CMP EAX,DWORD PTR DS:[ESI+0xB48]
$+AB     006C613B     75 0D                   JNZ SHORT PathOfEx.006C614A
$+AD     006C613D     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+B0     006C6140     8BCE                    MOV ECX,ESI
$+B2     006C6142     50                      PUSH EAX
$+B3     006C6143     E8 38080000             CALL PathOfEx.006C6980
$+B8     006C6148     EB 5D                   JMP SHORT PathOfEx.006C61A7
$+BA     006C614A     3B86 4C0B0000           CMP EAX,DWORD PTR DS:[ESI+0xB4C]
$+C0     006C6150     75 0D                   JNZ SHORT PathOfEx.006C615F
$+C2     006C6152     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+C5     006C6155     8BCE                    MOV ECX,ESI
$+C7     006C6157     50                      PUSH EAX
$+C8     006C6158     E8 43050000             CALL PathOfEx.006C66A0
$+CD     006C615D     EB 48                   JMP SHORT PathOfEx.006C61A7
$+CF     006C615F     3B86 500B0000           CMP EAX,DWORD PTR DS:[ESI+0xB50]
$+D5     006C6165     75 0A                   JNZ SHORT PathOfEx.006C6171
$+D7     006C6167     51                      PUSH ECX
$+D8     006C6168     8BCE                    MOV ECX,ESI
$+DA     006C616A     E8 A1040000             CALL PathOfEx.006C6610
$+DF     006C616F     EB 36                   JMP SHORT PathOfEx.006C61A7
$+E1     006C6171     3B86 540B0000           CMP EAX,DWORD PTR DS:[ESI+0xB54]
$+E7     006C6177     75 0D                   JNZ SHORT PathOfEx.006C6186
$+E9     006C6179     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+EC     006C617C     8BCE                    MOV ECX,ESI
$+EE     006C617E     50                      PUSH EAX
$+EF     006C617F     E8 BC050000             CALL PathOfEx.006C6740
$+F4     006C6184     EB 21                   JMP SHORT PathOfEx.006C61A7
$+F6     006C6186     3B86 580B0000           CMP EAX,DWORD PTR DS:[ESI+0xB58]
$+FC     006C618C     75 07                   JNZ SHORT PathOfEx.006C6195
$+FE     006C618E     B9 44A9EA00             MOV ECX,PathOfEx.00EAA944                            ; UNICODE "http://web.poe.garena.tw/forum"
$+103    006C6193     EB 0D                   JMP SHORT PathOfEx.006C61A2
$+105    006C6195     3B86 5C0B0000           CMP EAX,DWORD PTR DS:[ESI+0xB5C]
$+10B    006C619B     75 0A                   JNZ SHORT PathOfEx.006C61A7
$+10D    006C619D     B9 F0A8EA00             MOV ECX,PathOfEx.00EAA8F0                            ; UNICODE "http://forum.gamer.com.tw/A.php?bsn=18966"
$+112    006C61A2     E8 09060400             CALL PathOfEx.007067B0
$+117    006C61A7     C745 FC 02000000        MOV DWORD PTR SS:[EBP-0x4],0x2
$+11E    006C61AE     8B75 F0                 MOV ESI,DWORD PTR SS:[EBP-0x10]
$+121    006C61B1     85F6                    TEST ESI,ESI
$+123    006C61B3     74 1E                   JE SHORT PathOfEx.006C61D3
$+125    006C61B5     8BC3                    MOV EAX,EBX
$+127    006C61B7     F0:0FC146 04            LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+12C    006C61BC     75 15                   JNZ SHORT PathOfEx.006C61D3
$+12E    006C61BE     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+130    006C61C0     8BCE                    MOV ECX,ESI
$+132    006C61C2     FF10                    CALL DWORD PTR DS:[EAX]
$+134    006C61C4     F0:0FC15E 08            LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
$+139    006C61C9     4B                      DEC EBX
$+13A    006C61CA     75 07                   JNZ SHORT PathOfEx.006C61D3
$+13C    006C61CC     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+13E    006C61CE     8BCE                    MOV ECX,ESI
$+140    006C61D0     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+143    006C61D3     5F                      POP EDI
$+144    006C61D4     5B                      POP EBX
$+145    006C61D5     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+148    006C61D8     5E                      POP ESI
$+149    006C61D9     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+150    006C61E0     8BE5                    MOV ESP,EBP
$+152    006C61E2     5D                      POP EBP
$+153    006C61E3     C2 0400                 RETN 0x4
$+156    006C61E6     CC                      INT3
$+157    006C61E7     CC                      INT3
$+158    006C61E8     CC                      INT3
$+159    006C61E9     CC                      INT3
$+15A    006C61EA     CC                      INT3
$+15B    006C61EB     CC                      INT3
$+15C    006C61EC     CC                      INT3
$+15D    006C61ED     CC                      INT3
$+15E    006C61EE     CC                      INT3
$+15F    006C61EF     CC                      INT3
$+160    006C61F0     55                      PUSH EBP
$+161    006C61F1     8BEC                    MOV EBP,ESP
$+163    006C61F3     83E4 F8                 AND ESP,0xFFFFFFF8
$+166    006C61F6     56                      PUSH ESI
$+167    006C61F7     57                      PUSH EDI
$+168    006C61F8     8BF1                    MOV ESI,ECX
$+16A    006C61FA     E8 010A0000             CALL PathOfEx.006C6C00
$+16F    006C61FF     8BCE                    MOV ECX,ESI
$+171    006C6201     E8 2A12FEFF             CALL PathOfEx.006A7430
$+176    006C6206     8D7E 20                 LEA EDI,DWORD PTR DS:[ESI+0x20]
$+179    006C6209     8B75 10                 MOV ESI,DWORD PTR SS:[EBP+0x10]
$+17C    006C620C     01B7 140B0000           ADD DWORD PTR DS:[EDI+0xB14],ESI
$+182    006C6212     8BCF                    MOV ECX,EDI
$+184    006C6214     8B87 140B0000           MOV EAX,DWORD PTR DS:[EDI+0xB14]
$+18A    006C621A     6A 01                   PUSH 0x1
$+18C    006C621C     6A 00                   PUSH 0x0
$+18E    006C621E     68 00020000             PUSH 0x200
$+193    006C6223     66:0F6EC0               MOVD MM0,EAX
$+197    006C6227     F30FE6C0                CVTDQ2PD XMM0,XMM0
$+19B    006C622B     C1E8 1F                 SHR EAX,0x1F
$+19E    006C622E     F20F5804C5 40FCF300     ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
$+1A7    006C6237     660F5AC0                CVTPD2PS XMM0,XMM0
$+1AB    006C623B     F3:0F5905 68EBF300      MULSS XMM0,DWORD PTR DS:[0xF3EB68]
$+1B3    006C6243     F3:0F1187 180B0000      MOVSS DWORD PTR DS:[EDI+0xB18],XMM0
$+1BB    006C624B     E8 B01A3500             CALL PathOfEx.00A17D00
$+1C0    006C6250     FFB7 A40A0000           PUSH DWORD PTR DS:[EDI+0xAA4]
$+1C6    006C6256     F3:0F1097 180B0000      MOVSS XMM2,DWORD PTR DS:[EDI+0xB18]
$+1CE    006C625E     8BCF                    MOV ECX,EDI
$+1D0    006C6260     66:0F6EC6               MOVD MM0,ESI
$+1D4    006C6264     F30FE6C0                CVTDQ2PD XMM0,XMM0
$+1D8    006C6268     C1EE 1F                 SHR ESI,0x1F
$+1DB    006C626B     F20F5804F5 40FCF300     ADDSD XMM0,QWORD PTR DS:[ESI*8+0xF3FC40]
$+1E4    006C6274     660F5AD8                CVTPD2PS XMM3,XMM0
$+1E8    006C6278     F3:0F591D 68EBF300      MULSS XMM3,DWORD PTR DS:[0xF3EB68]
$+1F0    006C6280     E8 4B013500             CALL PathOfEx.00A163D0
$+1F5    006C6285     5F                      POP EDI
$+1F6    006C6286     5E                      POP ESI
$+1F7    006C6287     8BE5                    MOV ESP,EBP
$+1F9    006C6289     5D                      POP EBP
$+1FA    006C628A     C2 0C00                 RETN 0xC
$+1FD    006C628D     CC                      INT3
$+1FE    006C628E     CC                      INT3
$+1FF    006C628F     CC                      INT3
$+200    006C6290     8B5424 04               MOV EDX,DWORD PTR SS:[ESP+0x4]
$+204    006C6294     FF7424 08               PUSH DWORD PTR SS:[ESP+0x8]
$+208    006C6298     8991 600B0000           MOV DWORD PTR DS:[ECX+0xB60],EDX
$+20E    006C629E     8B89 C40A0000           MOV ECX,DWORD PTR DS:[ECX+0xAC4]
$+214    006C62A4     52                      PUSH EDX
$+215    006C62A5     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+217    006C62A7     FF90 88000000           CALL DWORD PTR DS:[EAX+0x88]
$+21D    006C62AD     33C9                    XOR ECX,ECX
$+21F    006C62AF     85C0                    TEST EAX,EAX
$+221    006C62B1     0F48C8                  CMOVS ECX,EAX
$+224    006C62B4     8BC1                    MOV EAX,ECX
$+226    006C62B6     C2 0800                 RETN 0x8
$+229    006C62B9     CC                      INT3
*/
/**/
//tag_从InGameState置顶到EscapeState
/*
$-311    006D2B2F     CC                      INT3
$-310    006D2B30     55                      PUSH EBP
$-30F    006D2B31     8BEC                    MOV EBP,ESP
$-30D    006D2B33     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-307    006D2B39     6A FF                   PUSH -0x1
$-305    006D2B3B     68 DA4BD400             PUSH PathOfEx.00D44BDA
$-300    006D2B40     50                      PUSH EAX
$-2FF    006D2B41     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-2F8    006D2B48     83EC 28                 SUB ESP,0x28
$-2F5    006D2B4B     57                      PUSH EDI
$-2F4    006D2B4C     8BF9                    MOV EDI,ECX
$-2F2    006D2B4E     8B87 3C010000           MOV EAX,DWORD PTR DS:[EDI+0x13C]
$-2EC    006D2B54     83B8 683F0000 00        CMP DWORD PTR DS:[EAX+0x3F68],0x0
$-2E5    006D2B5B     7E 13                   JLE SHORT PathOfEx.006D2B70
$-2E3    006D2B5D     32C0                    XOR AL,AL
$-2E1    006D2B5F     5F                      POP EDI
$-2E0    006D2B60     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-2DD    006D2B63     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-2D6    006D2B6A     8BE5                    MOV ESP,EBP
$-2D4    006D2B6C     5D                      POP EBP
$-2D3    006D2B6D     C2 0800                 RETN 0x8
$-2D0    006D2B70     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
$-2CD    006D2B73     33C9                    XOR ECX,ECX
$-2CB    006D2B75     C787 C81A0000 00000000  MOV DWORD PTR DS:[EDI+0x1AC8],0x0
$-2C1    006D2B7F     894D F0                 MOV DWORD PTR SS:[EBP-0x10],ECX
$-2BE    006D2B82     83F8 27                 CMP EAX,0x27
$-2BB    006D2B85     7D 13                   JGE SHORT PathOfEx.006D2B9A
$-2B9    006D2B87     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
$-2B6    006D2B8A     51                      PUSH ECX
$-2B5    006D2B8B     8B8F 34010000           MOV ECX,DWORD PTR DS:[EDI+0x134]
$-2AF    006D2B91     50                      PUSH EAX
$-2AE    006D2B92     E8 A9E20E00             CALL PathOfEx.007C0E40
$-2A9    006D2B97     8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
$-2A6    006D2B9A     53                      PUSH EBX
$-2A5    006D2B9B     56                      PUSH ESI
$-2A4    006D2B9C     E8 3FF23500             CALL PathOfEx.00A31DE0
$-29F    006D2BA1     8B8F 38010000           MOV ECX,DWORD PTR DS:[EDI+0x138]
$-299    006D2BA7     8BD0                    MOV EDX,EAX
$-297    006D2BA9     8B75 0C                 MOV ESI,DWORD PTR SS:[EBP+0xC]
$-294    006D2BAC     8955 F0                 MOV DWORD PTR SS:[EBP-0x10],EDX
$-291    006D2BAF     8B49 08                 MOV ECX,DWORD PTR DS:[ECX+0x8]
$-28E    006D2BB2     8A49 0C                 MOV CL,BYTE PTR DS:[ECX+0xC]
$-28B    006D2BB5     84C9                    TEST CL,CL
$-289    006D2BB7     74 4E                   JE SHORT PathOfEx.006D2C07
$-287    006D2BB9     8B0E                    MOV ECX,DWORD PTR DS:[ESI]
$-285    006D2BBB     85C9                    TEST ECX,ECX
$-283    006D2BBD     74 48                   JE SHORT PathOfEx.006D2C07
$-281    006D2BBF     66:8B41 02              MOV AX,WORD PTR DS:[ECX+0x2]
$-27D    006D2BC3     66:3B05 58F70001        CMP AX,WORD PTR DS:[0x100F758]
$-276    006D2BCA     74 3B                   JE SHORT PathOfEx.006D2C07
$-274    006D2BCC     83FA 01                 CMP EDX,0x1
$-271    006D2BCF     74 36                   JE SHORT PathOfEx.006D2C07
$-26F    006D2BD1     83FA 20                 CMP EDX,0x20
$-26C    006D2BD4     74 09                   JE SHORT PathOfEx.006D2BDF
$-26A    006D2BD6     83BF BC180000 00        CMP DWORD PTR DS:[EDI+0x18BC],0x0
$-263    006D2BDD     75 28                   JNZ SHORT PathOfEx.006D2C07
$-261    006D2BDF     68 6CB0EA00             PUSH PathOfEx.00EAB06C                          ; UNICODE "SkillTown"
$-25C    006D2BE4     8D4D CC                 LEA ECX,DWORD PTR SS:[EBP-0x34]
$-259    006D2BE7     E8 04B2D9FF             CALL PathOfEx.0046DDF0
$-254    006D2BEC     51                      PUSH ECX
$-253    006D2BED     8D45 CC                 LEA EAX,DWORD PTR SS:[EBP-0x34]
$-250    006D2BF0     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$-249    006D2BF7     50                      PUSH EAX
$-248    006D2BF8     8BCF                    MOV ECX,EDI
$-246    006D2BFA     E8 A1E7FFFF             CALL PathOfEx.006D13A0
$-241    006D2BFF     8D4D CC                 LEA ECX,DWORD PTR SS:[EBP-0x34]
$-23E    006D2C02     E8 39ABD9FF             CALL PathOfEx.0046D740
$-239    006D2C07     56                      PUSH ESI
$-238    006D2C08     8D55 08                 LEA EDX,DWORD PTR SS:[EBP+0x8]
$-235    006D2C0B     8D4D D8                 LEA ECX,DWORD PTR SS:[EBP-0x28]
$-232    006D2C0E     E8 9D310000             CALL PathOfEx.006D5DB0
$-22D    006D2C13     83C4 04                 ADD ESP,0x4
$-22A    006D2C16     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$-223    006D2C1D     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$-221    006D2C1F     894D E4                 MOV DWORD PTR SS:[EBP-0x1C],ECX
$-21E    006D2C22     8B48 08                 MOV ECX,DWORD PTR DS:[EAX+0x8]
$-21B    006D2C25     8B40 04                 MOV EAX,DWORD PTR DS:[EAX+0x4]
$-218    006D2C28     85C9                    TEST ECX,ECX
$-216    006D2C2A     74 04                   JE SHORT PathOfEx.006D2C30
$-214    006D2C2C     F0:FF41 08              LOCK INC DWORD PTR DS:[ECX+0x8]
$-210    006D2C30     894D EC                 MOV DWORD PTR SS:[EBP-0x14],ECX
$-20D    006D2C33     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
$-20A    006D2C36     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
$-207    006D2C39     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
$-203    006D2C3D     51                      PUSH ECX
$-202    006D2C3E     8D8F 64150000           LEA ECX,DWORD PTR DS:[EDI+0x1564]
$-1FC    006D2C44     E8 A7150000             CALL PathOfEx.006D41F0
$-1F7    006D2C49     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
$-1F3    006D2C4D     83CB FF                 OR EBX,0xFFFFFFFF
$-1F0    006D2C50     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$-1ED    006D2C53     85C9                    TEST ECX,ECX
$-1EB    006D2C55     74 0E                   JE SHORT PathOfEx.006D2C65
$-1E9    006D2C57     8BC3                    MOV EAX,EBX
$-1E7    006D2C59     F0:0FC141 08            LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
$-1E2    006D2C5E     75 05                   JNZ SHORT PathOfEx.006D2C65
$-1E0    006D2C60     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-1DE    006D2C62     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-1DB    006D2C65     C745 FC 05000000        MOV DWORD PTR SS:[EBP-0x4],0x5
$-1D4    006D2C6C     8B75 E0                 MOV ESI,DWORD PTR SS:[EBP-0x20]
$-1D1    006D2C6F     85F6                    TEST ESI,ESI
$-1CF    006D2C71     74 1E                   JE SHORT PathOfEx.006D2C91
$-1CD    006D2C73     8BC3                    MOV EAX,EBX
$-1CB    006D2C75     F0:0FC146 04            LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$-1C6    006D2C7A     75 15                   JNZ SHORT PathOfEx.006D2C91
$-1C4    006D2C7C     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$-1C2    006D2C7E     8BCE                    MOV ECX,ESI
$-1C0    006D2C80     FF10                    CALL DWORD PTR DS:[EAX]
$-1BE    006D2C82     F0:0FC15E 08            LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
$-1B9    006D2C87     4B                      DEC EBX
$-1B8    006D2C88     75 07                   JNZ SHORT PathOfEx.006D2C91
$-1B6    006D2C8A     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$-1B4    006D2C8C     8BCE                    MOV ECX,ESI
$-1B2    006D2C8E     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-1AF    006D2C91     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
$-1A8    006D2C98     837D F0 20              CMP DWORD PTR SS:[EBP-0x10],0x20
$-1A4    006D2C9C     74 18                   JE SHORT PathOfEx.006D2CB6
$-1A2    006D2C9E     8B8F D8180000           MOV ECX,DWORD PTR DS:[EDI+0x18D8]
$-19C    006D2CA4     8B87 040C0000           MOV EAX,DWORD PTR DS:[EDI+0xC04]
$-196    006D2CAA     3B81 400A0000           CMP EAX,DWORD PTR DS:[ECX+0xA40]
$-190    006D2CB0     74 04                   JE SHORT PathOfEx.006D2CB6
$-18E    006D2CB2     B3 01                   MOV BL,0x1
$-18C    006D2CB4     EB 02                   JMP SHORT PathOfEx.006D2CB8
$-18A    006D2CB6     32DB                    XOR BL,BL
$-188    006D2CB8     8BCF                    MOV ECX,EDI
$-186    006D2CBA     E8 61F2FFFF             CALL PathOfEx.006D1F20
$-181    006D2CBF     83F8 02                 CMP EAX,0x2
$-17E    006D2CC2     75 69                   JNZ SHORT PathOfEx.006D2D2D
$-17C    006D2CC4     8B8F 68150000           MOV ECX,DWORD PTR DS:[EDI+0x1568]
$-176    006D2CCA     8DB7 64150000           LEA ESI,DWORD PTR DS:[EDI+0x1564]
$-170    006D2CD0     2B0E                    SUB ECX,DWORD PTR DS:[ESI]
$-16E    006D2CD2     B8 ABAAAA2A             MOV EAX,0x2AAAAAAB                              ; UNICODE ""
$-169    006D2CD7     F7E9                    IMUL ECX
$-167    006D2CD9     D1FA                    SAR EDX,1
$-165    006D2CDB     8BC2                    MOV EAX,EDX
$-163    006D2CDD     C1E8 1F                 SHR EAX,0x1F
$-160    006D2CE0     03C2                    ADD EAX,EDX
$-15E    006D2CE2     83F8 01                 CMP EAX,0x1
$-15B    006D2CE5     76 51                   JBE SHORT PathOfEx.006D2D38
$-159    006D2CE7     8B87 68150000           MOV EAX,DWORD PTR DS:[EDI+0x1568]
$-153    006D2CED     8B48 F4                 MOV ECX,DWORD PTR DS:[EAX-0xC]
$-150    006D2CF0     3B48 E8                 CMP ECX,DWORD PTR DS:[EAX-0x18]
$-14D    006D2CF3     74 43                   JE SHORT PathOfEx.006D2D38
$-14B    006D2CF5     84DB                    TEST BL,BL
$-149    006D2CF7     74 1F                   JE SHORT PathOfEx.006D2D18
$-147    006D2CF9     8B87 D8180000           MOV EAX,DWORD PTR DS:[EDI+0x18D8]
$-141    006D2CFF     8BB0 400A0000           MOV ESI,DWORD PTR DS:[EAX+0xA40]
$-13B    006D2D05     A1 C88CFF00             MOV EAX,DWORD PTR DS:[0xFF8CC8]
$-136    006D2D0A     FF30                    PUSH DWORD PTR DS:[EAX]
$-134    006D2D0C     FF15 34E4D800           CALL DWORD PTR DS:[0xD8E434]
$-12E    006D2D12     89B7 040C0000           MOV DWORD PTR DS:[EDI+0xC04],ESI
$-128    006D2D18     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-125    006D2D1B     B0 01                   MOV AL,0x1
$-123    006D2D1D     5E                      POP ESI
$-122    006D2D1E     5B                      POP EBX
$-121    006D2D1F     5F                      POP EDI
$-120    006D2D20     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-119    006D2D27     8BE5                    MOV ESP,EBP
$-117    006D2D29     5D                      POP EBP
$-116    006D2D2A     C2 0800                 RETN 0x8
$-113    006D2D2D     83F8 04                 CMP EAX,0x4
$-110    006D2D30   ^ 74 C3                   JE SHORT PathOfEx.006D2CF5
$-10E    006D2D32     8DB7 64150000           LEA ESI,DWORD PTR DS:[EDI+0x1564]
$-108    006D2D38     8B87 68150000           MOV EAX,DWORD PTR DS:[EDI+0x1568]
$-102    006D2D3E     8BCE                    MOV ECX,ESI
$-100    006D2D40     83E8 0C                 SUB EAX,0xC
$-FD     006D2D43     50                      PUSH EAX
$-FC     006D2D44     8D45 08                 LEA EAX,DWORD PTR SS:[EBP+0x8]
$-F9     006D2D47     50                      PUSH EAX
$-F8     006D2D48     E8 03140000             CALL PathOfEx.006D4150
$-F3     006D2D4D   ^ EB C9                   JMP SHORT PathOfEx.006D2D18
$-F1     006D2D4F     CC                      INT3
$-F0     006D2D50     55                      PUSH EBP
$-EF     006D2D51     8BEC                    MOV EBP,ESP
$-ED     006D2D53     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$-E7     006D2D59     6A FF                   PUSH -0x1
$-E5     006D2D5B     68 3C1BD200             PUSH PathOfEx.00D21B3C
$-E0     006D2D60     50                      PUSH EAX
$-DF     006D2D61     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$-D8     006D2D68     83EC 20                 SUB ESP,0x20
$-D5     006D2D6B     56                      PUSH ESI
$-D4     006D2D6C     8BF1                    MOV ESI,ECX
$-D2     006D2D6E     8B86 64150000           MOV EAX,DWORD PTR DS:[ESI+0x1564]
$-CC     006D2D74     3B86 68150000           CMP EAX,DWORD PTR DS:[ESI+0x1568]
$-C6     006D2D7A     0F84 A7000000           JE PathOfEx.006D2E27
$-C0     006D2D80     FF75 EC                 PUSH DWORD PTR SS:[EBP-0x14]
$-BD     006D2D83     8B96 68150000           MOV EDX,DWORD PTR DS:[ESI+0x1568]
$-B7     006D2D89     51                      PUSH ECX
$-B6     006D2D8A     8BC8                    MOV ECX,EAX
$-B4     006D2D8C     E8 2F450000             CALL PathOfEx.006D72C0
$-AF     006D2D91     8B86 64150000           MOV EAX,DWORD PTR DS:[ESI+0x1564]
$-A9     006D2D97     83C4 08                 ADD ESP,0x8
$-A6     006D2D9A     8986 68150000           MOV DWORD PTR DS:[ESI+0x1568],EAX
$-A0     006D2DA0     8B8E 3C010000           MOV ECX,DWORD PTR DS:[ESI+0x13C]
$-9A     006D2DA6     85C9                    TEST ECX,ECX
$-98     006D2DA8     74 7D                   JE SHORT PathOfEx.006D2E27
$-96     006D2DAA     E8 61462000             CALL PathOfEx.008D7410
$-91     006D2DAF     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
$-8E     006D2DB2     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
$-87     006D2DB9     50                      PUSH EAX
$-86     006D2DBA     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
$-83     006D2DBD     C745 EC 00000000        MOV DWORD PTR SS:[EBP-0x14],0x0
$-7C     006D2DC4     50                      PUSH EAX
$-7B     006D2DC5     8D55 EC                 LEA EDX,DWORD PTR SS:[EBP-0x14]
$-78     006D2DC8     C745 E4 00000000        MOV DWORD PTR SS:[EBP-0x1C],0x0
$-71     006D2DCF     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$-6E     006D2DD2     C745 E8 00000000        MOV DWORD PTR SS:[EBP-0x18],0x0
$-67     006D2DD9     E8 72240000             CALL PathOfEx.006D5250
$-62     006D2DDE     83C4 08                 ADD ESP,0x8
$-5F     006D2DE1     8D8E 70150000           LEA ECX,DWORD PTR DS:[ESI+0x1570]
$-59     006D2DE7     50                      PUSH EAX
$-58     006D2DE8     E8 63270000             CALL PathOfEx.006D5550
$-53     006D2DED     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
$-4C     006D2DF4     83CE FF                 OR ESI,0xFFFFFFFF
$-49     006D2DF7     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
$-46     006D2DFA     85C9                    TEST ECX,ECX
$-44     006D2DFC     74 0E                   JE SHORT PathOfEx.006D2E0C
$-42     006D2DFE     8BC6                    MOV EAX,ESI
$-40     006D2E00     F0:0FC141 08            LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
$-3B     006D2E05     75 05                   JNZ SHORT PathOfEx.006D2E0C
$-39     006D2E07     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-37     006D2E09     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-34     006D2E0C     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$-2D     006D2E13     8B4D E8                 MOV ECX,DWORD PTR SS:[EBP-0x18]
$-2A     006D2E16     85C9                    TEST ECX,ECX
$-28     006D2E18     74 0D                   JE SHORT PathOfEx.006D2E27
$-26     006D2E1A     F0:0FC171 08            LOCK XADD DWORD PTR DS:[ECX+0x8],ESI
$-21     006D2E1F     4E                      DEC ESI
$-20     006D2E20     75 05                   JNZ SHORT PathOfEx.006D2E27
$-1E     006D2E22     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$-1C     006D2E24     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$-19     006D2E27     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$-16     006D2E2A     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$-F      006D2E31     5E                      POP ESI
$-E      006D2E32     8BE5                    MOV ESP,EBP
$-C      006D2E34     5D                      POP EBP
$-B      006D2E35     C3                      RETN
$-A      006D2E36     CC                      INT3
$-9      006D2E37     CC                      INT3
$-8      006D2E38     CC                      INT3
$-7      006D2E39     CC                      INT3
$-6      006D2E3A     CC                      INT3
$-5      006D2E3B     CC                      INT3
$-4      006D2E3C     CC                      INT3
$-3      006D2E3D     CC                      INT3
$-2      006D2E3E     CC                      INT3
$-1      006D2E3F     CC                      INT3						;此函数无参数
$ ==>    006D2E40     55                      PUSH EBP                                        ; tag_从InGameState置顶到EscapeState
$+1      006D2E41     8BEC                    MOV EBP,ESP
$+3      006D2E43     6A FF                   PUSH -0x1
$+5      006D2E45     68 0636D400             PUSH PathOfEx.00D43606
$+A      006D2E4A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+10     006D2E50     50                      PUSH EAX
$+11     006D2E51     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+18     006D2E58     83EC 18                 SUB ESP,0x18
$+1B     006D2E5B     53                      PUSH EBX
$+1C     006D2E5C     56                      PUSH ESI
$+1D     006D2E5D     57                      PUSH EDI
$+1E     006D2E5E     8BF1                    MOV ESI,ECX
$+20     006D2E60     E8 EBFEFFFF             CALL PathOfEx.006D2D50                          ; 啥都没干
$+25     006D2E65     6A 00                   PUSH 0x0
$+27     006D2E67     6A 00                   PUSH 0x0
$+29     006D2E69     8BCE                    MOV ECX,ESI
$+2B     006D2E6B     E8 D0D3FFFF             CALL PathOfEx.006D0240                          ; 啥都没干
$+30     006D2E70     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]                 ; ecx是8个字节的结构体
$+33     006D2E73     E8 C8A8DAFF             CALL PathOfEx.0047D740                          ; 填充该结构，成员是两个指针
$+38     006D2E78     51                      PUSH ECX
$+39     006D2E79     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+3C     006D2E7C     51                      PUSH ECX
$+3D     006D2E7D     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$+3F     006D2E7F     E8 4C2F0000             CALL PathOfEx.006D5DD0                          ; 做了些EscapeState之类的事
$+44     006D2E84     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+4B     006D2E8B     83CF FF                 OR EDI,0xFFFFFFFF
$+4E     006D2E8E     8B75 F0                 MOV ESI,DWORD PTR SS:[EBP-0x10]
$+51     006D2E91     85F6                    TEST ESI,ESI
$+53     006D2E93     74 1F                   JE SHORT PathOfEx.006D2EB4                      ; 跳过去
$+55     006D2E95     8BC7                    MOV EAX,EDI
$+57     006D2E97     F0:0FC146 04            LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+5C     006D2E9C     75 16                   JNZ SHORT PathOfEx.006D2EB4
$+5E     006D2E9E     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+60     006D2EA0     8BCE                    MOV ECX,ESI
$+62     006D2EA2     FF10                    CALL DWORD PTR DS:[EAX]
$+64     006D2EA4     8BC7                    MOV EAX,EDI
$+66     006D2EA6     F0:0FC146 08            LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+6B     006D2EAB     75 07                   JNZ SHORT PathOfEx.006D2EB4
$+6D     006D2EAD     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+6F     006D2EAF     8BCE                    MOV ECX,ESI
$+71     006D2EB1     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+74     006D2EB4     8B75 E8                 MOV ESI,DWORD PTR SS:[EBP-0x18]
$+77     006D2EB7     85F6                    TEST ESI,ESI
$+79     006D2EB9     74 04                   JE SHORT PathOfEx.006D2EBF
$+7B     006D2EBB     F0:FF46 04              LOCK INC DWORD PTR DS:[ESI+0x4]
$+7F     006D2EBF     8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+82     006D2EC2     8975 F0                 MOV DWORD PTR SS:[EBP-0x10],ESI
$+85     006D2EC5     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
$+88     006D2EC8     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
$+8B     006D2ECB     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
$+8F     006D2ECF     E8 6CA8DAFF             CALL PathOfEx.0047D740                          ; 啥都没干
$+94     006D2ED4     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
$+98     006D2ED8     8B18                    MOV EBX,DWORD PTR DS:[EAX]
$+9A     006D2EDA     8B43 04                 MOV EAX,DWORD PTR DS:[EBX+0x4]
$+9D     006D2EDD     8D4B 04                 LEA ECX,DWORD PTR DS:[EBX+0x4]
$+A0     006D2EE0     3B41 04                 CMP EAX,DWORD PTR DS:[ECX+0x4]
$+A3     006D2EE3     74 0E                   JE SHORT PathOfEx.006D2EF3
$+A5     006D2EE5     8B43 08                 MOV EAX,DWORD PTR DS:[EBX+0x8]
$+A8     006D2EE8     8B48 F8                 MOV ECX,DWORD PTR DS:[EAX-0x8]
$+AB     006D2EEB     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+AD     006D2EED     FF50 2C                 CALL DWORD PTR DS:[EAX+0x2C]                    ; 啥都没干
$+B0     006D2EF0     8D4B 04                 LEA ECX,DWORD PTR DS:[EBX+0x4]
$+B3     006D2EF3     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+B6     006D2EF6     50                      PUSH EAX
$+B7     006D2EF7     E8 945EFDFF             CALL PathOfEx.006A8D90                          ; 啥都没干
$+BC     006D2EFC     837B 1C 00              CMP DWORD PTR DS:[EBX+0x1C],0x0
$+C0     006D2F00     75 0E                   JNZ SHORT PathOfEx.006D2F10
$+C2     006D2F02     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
$+C5     006D2F05     50                      PUSH EAX
$+C6     006D2F06     8D4B 10                 LEA ECX,DWORD PTR DS:[EBX+0x10]
$+C9     006D2F09     E8 825EFDFF             CALL PathOfEx.006A8D90
$+CE     006D2F0E     EB 04                   JMP SHORT PathOfEx.006D2F14
$+D0     006D2F10     C643 20 01              MOV BYTE PTR DS:[EBX+0x20],0x1
$+D4     006D2F14     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
$+D7     006D2F17     8B01                    MOV EAX,DWORD PTR DS:[ECX]
$+D9     006D2F19     FF50 24                 CALL DWORD PTR DS:[EAX+0x24]                    ; stCD_EscapeState在里面创建了EscapeMenuInner
$+DC     006D2F1C     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
$+E0     006D2F20     8B5D E0                 MOV EBX,DWORD PTR SS:[EBP-0x20]
$+E3     006D2F23     85DB                    TEST EBX,EBX
$+E5     006D2F25     74 1F                   JE SHORT PathOfEx.006D2F46
$+E7     006D2F27     8BC7                    MOV EAX,EDI
$+E9     006D2F29     F0:0FC143 04            LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+EE     006D2F2E     75 16                   JNZ SHORT PathOfEx.006D2F46
$+F0     006D2F30     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+F2     006D2F32     8BCB                    MOV ECX,EBX
$+F4     006D2F34     FF10                    CALL DWORD PTR DS:[EAX]
$+F6     006D2F36     8BC7                    MOV EAX,EDI
$+F8     006D2F38     F0:0FC143 08            LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
$+FD     006D2F3D     75 07                   JNZ SHORT PathOfEx.006D2F46
$+FF     006D2F3F     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+101    006D2F41     8BCB                    MOV ECX,EBX
$+103    006D2F43     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+106    006D2F46     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
$+10A    006D2F4A     8B5D F0                 MOV EBX,DWORD PTR SS:[EBP-0x10]
$+10D    006D2F4D     85DB                    TEST EBX,EBX
$+10F    006D2F4F     74 1F                   JE SHORT PathOfEx.006D2F70
$+111    006D2F51     8BC7                    MOV EAX,EDI
$+113    006D2F53     F0:0FC143 04            LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+118    006D2F58     75 16                   JNZ SHORT PathOfEx.006D2F70
$+11A    006D2F5A     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+11C    006D2F5C     8BCB                    MOV ECX,EBX
$+11E    006D2F5E     FF10                    CALL DWORD PTR DS:[EAX]
$+120    006D2F60     8BC7                    MOV EAX,EDI
$+122    006D2F62     F0:0FC143 08            LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
$+127    006D2F67     75 07                   JNZ SHORT PathOfEx.006D2F70
$+129    006D2F69     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+12B    006D2F6B     8BCB                    MOV ECX,EBX
$+12D    006D2F6D     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+130    006D2F70     C745 FC 07000000        MOV DWORD PTR SS:[EBP-0x4],0x7
$+137    006D2F77     85F6                    TEST ESI,ESI
$+139    006D2F79     74 1E                   JE SHORT PathOfEx.006D2F99
$+13B    006D2F7B     8BC7                    MOV EAX,EDI
$+13D    006D2F7D     F0:0FC146 04            LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+142    006D2F82     75 15                   JNZ SHORT PathOfEx.006D2F99
$+144    006D2F84     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+146    006D2F86     8BCE                    MOV ECX,ESI
$+148    006D2F88     FF10                    CALL DWORD PTR DS:[EAX]
$+14A    006D2F8A     F0:0FC17E 08            LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
$+14F    006D2F8F     4F                      DEC EDI
$+150    006D2F90     75 07                   JNZ SHORT PathOfEx.006D2F99
$+152    006D2F92     8B06                    MOV EAX,DWORD PTR DS:[ESI]
$+154    006D2F94     8BCE                    MOV ECX,ESI
$+156    006D2F96     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+159    006D2F99     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+15C    006D2F9C     5F                      POP EDI
$+15D    006D2F9D     5E                      POP ESI
$+15E    006D2F9E     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+165    006D2FA5     5B                      POP EBX
$+166    006D2FA6     8BE5                    MOV ESP,EBP
$+168    006D2FA8     5D                      POP EBP
$+169    006D2FA9     C3                      RETN
$+16A    006D2FAA     CC                      INT3
$+16B    006D2FAB     CC                      INT3
$+16C    006D2FAC     CC                      INT3
$+16D    006D2FAD     CC                      INT3
$+16E    006D2FAE     CC                      INT3
$+16F    006D2FAF     CC                      INT3
$+170    006D2FB0     55                      PUSH EBP
$+171    006D2FB1     8BEC                    MOV EBP,ESP
$+173    006D2FB3     6A FF                   PUSH -0x1
$+175    006D2FB5     68 3A4CD400             PUSH PathOfEx.00D44C3A
$+17A    006D2FBA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
$+180    006D2FC0     50                      PUSH EAX
$+181    006D2FC1     64:8925 00000000        MOV DWORD PTR FS:[0],ESP
$+188    006D2FC8     83EC 30                 SUB ESP,0x30
$+18B    006D2FCB     83B9 60150000 00        CMP DWORD PTR DS:[ECX+0x1560],0x0
$+192    006D2FD2     894D EC                 MOV DWORD PTR SS:[EBP-0x14],ECX
$+195    006D2FD5     76 0F                   JBE SHORT PathOfEx.006D2FE6
$+197    006D2FD7     C781 60150000 00000000  MOV DWORD PTR DS:[ECX+0x1560],0x0
$+1A1    006D2FE1     E8 FA010000             CALL PathOfEx.006D31E0
$+1A6    006D2FE6     56                      PUSH ESI
$+1A7    006D2FE7     57                      PUSH EDI
$+1A8    006D2FE8     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+1AB    006D2FEB     E8 50A7DAFF             CALL PathOfEx.0047D740
$+1B0    006D2FF0     51                      PUSH ECX
$+1B1    006D2FF1     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
$+1B4    006D2FF4     51                      PUSH ECX
$+1B5    006D2FF5     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$+1B7    006D2FF7     E8 64E0FCFF             CALL PathOfEx.006A1060
$+1BC    006D2FFC     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
$+1C3    006D3003     83CE FF                 OR ESI,0xFFFFFFFF
$+1C6    006D3006     8B7D D8                 MOV EDI,DWORD PTR SS:[EBP-0x28]
$+1C9    006D3009     85FF                    TEST EDI,EDI
$+1CB    006D300B     74 1F                   JE SHORT PathOfEx.006D302C
$+1CD    006D300D     8BC6                    MOV EAX,ESI
$+1CF    006D300F     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+1D4    006D3014     75 16                   JNZ SHORT PathOfEx.006D302C
$+1D6    006D3016     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+1D8    006D3018     8BCF                    MOV ECX,EDI
$+1DA    006D301A     FF10                    CALL DWORD PTR DS:[EAX]
$+1DC    006D301C     8BC6                    MOV EAX,ESI
$+1DE    006D301E     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$+1E3    006D3023     75 07                   JNZ SHORT PathOfEx.006D302C
$+1E5    006D3025     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+1E7    006D3027     8BCF                    MOV ECX,EDI
$+1E9    006D3029     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+1EC    006D302C     8D4D CC                 LEA ECX,DWORD PTR SS:[EBP-0x34]
$+1EF    006D302F     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
$+1F3    006D3033     E8 08A7DAFF             CALL PathOfEx.0047D740
$+1F8    006D3038     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
$+1FB    006D303B     51                      PUSH ECX
$+1FC    006D303C     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$+1FE    006D303E     E8 9D4AFDFF             CALL PathOfEx.006A7AE0
$+203    006D3043     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
$+207    006D3047     8B7D D0                 MOV EDI,DWORD PTR SS:[EBP-0x30]
$+20A    006D304A     85FF                    TEST EDI,EDI
$+20C    006D304C     74 1F                   JE SHORT PathOfEx.006D306D
$+20E    006D304E     8BC6                    MOV EAX,ESI
$+210    006D3050     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+215    006D3055     75 16                   JNZ SHORT PathOfEx.006D306D
$+217    006D3057     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+219    006D3059     8BCF                    MOV ECX,EDI
$+21B    006D305B     FF10                    CALL DWORD PTR DS:[EAX]
$+21D    006D305D     8BC6                    MOV EAX,ESI
$+21F    006D305F     F0:0FC147 08            LOCK XADD DWORD PTR DS:[EDI+0x8],EAX
$+224    006D3064     75 07                   JNZ SHORT PathOfEx.006D306D
$+226    006D3066     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+228    006D3068     8BCF                    MOV ECX,EDI
$+22A    006D306A     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+22D    006D306D     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
$+230    006D3070     394D D4                 CMP DWORD PTR SS:[EBP-0x2C],ECX
$+233    006D3073     8B7D E0                 MOV EDI,DWORD PTR SS:[EBP-0x20]
$+236    006D3076     0F94C0                  SETE AL
$+239    006D3079     34 01                   XOR AL,0x1
$+23B    006D307B     53                      PUSH EBX
$+23C    006D307C     0F84 EC000000           JE PathOfEx.006D316E
$+242    006D3082     85FF                    TEST EDI,EDI
$+244    006D3084     74 04                   JE SHORT PathOfEx.006D308A
$+246    006D3086     F0:FF47 04              LOCK INC DWORD PTR DS:[EDI+0x4]
$+24A    006D308A     897D E8                 MOV DWORD PTR SS:[EBP-0x18],EDI
$+24D    006D308D     894D E4                 MOV DWORD PTR SS:[EBP-0x1C],ECX
$+250    006D3090     8D4D C4                 LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+253    006D3093     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
$+257    006D3097     E8 A4A6DAFF             CALL PathOfEx.0047D740
$+25C    006D309C     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+25F    006D309F     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
$+263    006D30A3     51                      PUSH ECX
$+264    006D30A4     8B08                    MOV ECX,DWORD PTR DS:[EAX]
$+266    006D30A6     E8 3544FDFF             CALL PathOfEx.006A74E0
$+26B    006D30AB     C645 FC 07              MOV BYTE PTR SS:[EBP-0x4],0x7
$+26F    006D30AF     8B5D C8                 MOV EBX,DWORD PTR SS:[EBP-0x38]
$+272    006D30B2     85DB                    TEST EBX,EBX
$+274    006D30B4     74 1F                   JE SHORT PathOfEx.006D30D5
$+276    006D30B6     8BC6                    MOV EAX,ESI
$+278    006D30B8     F0:0FC143 04            LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+27D    006D30BD     75 16                   JNZ SHORT PathOfEx.006D30D5
$+27F    006D30BF     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+281    006D30C1     8BCB                    MOV ECX,EBX
$+283    006D30C3     FF10                    CALL DWORD PTR DS:[EAX]
$+285    006D30C5     8BC6                    MOV EAX,ESI
$+287    006D30C7     F0:0FC143 08            LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
$+28C    006D30CC     75 07                   JNZ SHORT PathOfEx.006D30D5
$+28E    006D30CE     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+290    006D30D0     8BCB                    MOV ECX,EBX
$+292    006D30D2     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+295    006D30D5     C645 FC 08              MOV BYTE PTR SS:[EBP-0x4],0x8
$+299    006D30D9     8B5D E8                 MOV EBX,DWORD PTR SS:[EBP-0x18]
$+29C    006D30DC     85DB                    TEST EBX,EBX
$+29E    006D30DE     74 1F                   JE SHORT PathOfEx.006D30FF
$+2A0    006D30E0     8BC6                    MOV EAX,ESI
$+2A2    006D30E2     F0:0FC143 04            LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+2A7    006D30E7     75 16                   JNZ SHORT PathOfEx.006D30FF
$+2A9    006D30E9     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+2AB    006D30EB     8BCB                    MOV ECX,EBX
$+2AD    006D30ED     FF10                    CALL DWORD PTR DS:[EAX]
$+2AF    006D30EF     8BC6                    MOV EAX,ESI
$+2B1    006D30F1     F0:0FC143 08            LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
$+2B6    006D30F6     75 07                   JNZ SHORT PathOfEx.006D30FF
$+2B8    006D30F8     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+2BA    006D30FA     8BCB                    MOV ECX,EBX
$+2BC    006D30FC     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+2BF    006D30FF     FF75 08                 PUSH DWORD PTR SS:[EBP+0x8]
$+2C2    006D3102     C645 FC 02              MOV BYTE PTR SS:[EBP-0x4],0x2
$+2C6    006D3106     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
$+2C9    006D3109     E8 A271FDFF             CALL PathOfEx.006AA2B0
$+2CE    006D310E     8B55 EC                 MOV EDX,DWORD PTR SS:[EBP-0x14]
$+2D1    006D3111     8A8A BC170000           MOV CL,BYTE PTR DS:[EDX+0x17BC]
$+2D7    006D3117     C682 BC170000 00        MOV BYTE PTR DS:[EDX+0x17BC],0x0
$+2DE    006D311E     8B82 B0170000           MOV EAX,DWORD PTR DS:[EDX+0x17B0]
$+2E4    006D3124     8982 B4170000           MOV DWORD PTR DS:[EDX+0x17B4],EAX
$+2EA    006D312A     888A BC170000           MOV BYTE PTR DS:[EDX+0x17BC],CL
$+2F0    006D3130     84C9                    TEST CL,CL
$+2F2    006D3132     75 06                   JNZ SHORT PathOfEx.006D313A
$+2F4    006D3134     8982 B4170000           MOV DWORD PTR DS:[EDX+0x17B4],EAX
$+2FA    006D313A     8B82 901A0000           MOV EAX,DWORD PTR DS:[EDX+0x1A90]
$+300    006D3140     85C0                    TEST EAX,EAX
$+302    006D3142     74 2A                   JE SHORT PathOfEx.006D316E
$+304    006D3144     8078 29 00              CMP BYTE PTR DS:[EAX+0x29],0x0
$+308    006D3148     C740 34 00000000        MOV DWORD PTR DS:[EAX+0x34],0x0
$+30F    006D314F     C740 38 0000003F        MOV DWORD PTR DS:[EAX+0x38],0x3F000000
$+316    006D3156     C640 28 FF              MOV BYTE PTR DS:[EAX+0x28],0xFF
$+31A    006D315A     74 12                   JE SHORT PathOfEx.006D316E
$+31C    006D315C     8B48 1C                 MOV ECX,DWORD PTR DS:[EAX+0x1C]
$+31F    006D315F     85C9                    TEST ECX,ECX
$+321    006D3161     74 0B                   JE SHORT PathOfEx.006D316E
$+323    006D3163     83C0 24                 ADD EAX,0x24
$+326    006D3166     50                      PUSH EAX
$+327    006D3167     51                      PUSH ECX
$+328    006D3168     FF15 1C37FF00           CALL DWORD PTR DS:[0xFF371C]                    ; fmodstud.FMOD::Studio::EventInstance::getVolume
$+32E    006D316E     C645 FC 09              MOV BYTE PTR SS:[EBP-0x4],0x9
$+332    006D3172     8B5D D8                 MOV EBX,DWORD PTR SS:[EBP-0x28]
$+335    006D3175     85DB                    TEST EBX,EBX
$+337    006D3177     74 1F                   JE SHORT PathOfEx.006D3198
$+339    006D3179     8BC6                    MOV EAX,ESI
$+33B    006D317B     F0:0FC143 04            LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
$+340    006D3180     75 16                   JNZ SHORT PathOfEx.006D3198
$+342    006D3182     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+344    006D3184     8BCB                    MOV ECX,EBX
$+346    006D3186     FF10                    CALL DWORD PTR DS:[EAX]
$+348    006D3188     8BC6                    MOV EAX,ESI
$+34A    006D318A     F0:0FC143 08            LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
$+34F    006D318F     75 07                   JNZ SHORT PathOfEx.006D3198
$+351    006D3191     8B03                    MOV EAX,DWORD PTR DS:[EBX]
$+353    006D3193     8BCB                    MOV ECX,EBX
$+355    006D3195     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+358    006D3198     C745 FC 0A000000        MOV DWORD PTR SS:[EBP-0x4],0xA
$+35F    006D319F     5B                      POP EBX
$+360    006D31A0     85FF                    TEST EDI,EDI
$+362    006D31A2     74 1E                   JE SHORT PathOfEx.006D31C2
$+364    006D31A4     8BC6                    MOV EAX,ESI
$+366    006D31A6     F0:0FC147 04            LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
$+36B    006D31AB     75 15                   JNZ SHORT PathOfEx.006D31C2
$+36D    006D31AD     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+36F    006D31AF     8BCF                    MOV ECX,EDI
$+371    006D31B1     FF10                    CALL DWORD PTR DS:[EAX]
$+373    006D31B3     F0:0FC177 08            LOCK XADD DWORD PTR DS:[EDI+0x8],ESI
$+378    006D31B8     4E                      DEC ESI
$+379    006D31B9     75 07                   JNZ SHORT PathOfEx.006D31C2
$+37B    006D31BB     8B07                    MOV EAX,DWORD PTR DS:[EDI]
$+37D    006D31BD     8BCF                    MOV ECX,EDI
$+37F    006D31BF     FF50 04                 CALL DWORD PTR DS:[EAX+0x4]
$+382    006D31C2     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
$+385    006D31C5     5F                      POP EDI
$+386    006D31C6     5E                      POP ESI
$+387    006D31C7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
$+38E    006D31CE     8BE5                    MOV ESP,EBP
$+390    006D31D0     5D                      POP EBP
$+391    006D31D1     C2 0400                 RETN 0x4
$+394    006D31D4     CC                      INT3
*/
/**/
