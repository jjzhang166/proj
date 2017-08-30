#pragma once
/*
@author zhp
@date 2017/4/9 21:39
@purpose for item asm record
*/

//tag_查找GrantedEffectsPerLevelPair
/*
$-2      007A9B1E     CC                        INT3
$-1      007A9B1F     CC                        INT3                                     ; g_c_find_GrantedEffectsPerLevelPair_
$ ==>    007A9B20     55                        PUSH EBP                                 ; tag_查找GrantedEffectsPerLevelPair
$+1      007A9B21     8BEC                      MOV EBP,ESP
$+3      007A9B23     6A FF                     PUSH -0x1
$+5      007A9B25     68 7F43D500               PUSH PathOfEx.00D5437F
$+A      007A9B2A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+10     007A9B30     50                        PUSH EAX
$+11     007A9B31     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+18     007A9B38     83EC 3C                   SUB ESP,0x3C
$+1B     007A9B3B     53                        PUSH EBX
$+1C     007A9B3C     56                        PUSH ESI
$+1D     007A9B3D     57                        PUSH EDI
$+1E     007A9B3E     8BF9                      MOV EDI,ECX
$+20     007A9B40     83EC 10                   SUB ESP,0x10
$+23     007A9B43     897D E4                   MOV DWORD PTR SS:[EBP-0x1C],EDI
$+26     007A9B46     8BDA                      MOV EBX,EDX
$+28     007A9B48     C745 E8 00000000          MOV DWORD PTR SS:[EBP-0x18],0x0
$+2F     007A9B4F     E8 ACE4F6FF               CALL PathOfEx.00718000
$+34     007A9B54     8BC8                      MOV ECX,EAX
$+36     007A9B56     E8 E5F7FCFF               CALL PathOfEx.00779340
$+3B     007A9B5B     8BF0                      MOV ESI,EAX
$+3D     007A9B5D     8975 EC                   MOV DWORD PTR SS:[EBP-0x14],ESI
$+40     007A9B60     C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
$+47     007A9B67     85F6                      TEST ESI,ESI
$+49     007A9B69     74 07                     JE SHORT PathOfEx.007A9B72
$+4B     007A9B6B     8BCE                      MOV ECX,ESI
$+4D     007A9B6D     E8 4EFAF6FF               CALL PathOfEx.007195C0
$+52     007A9B72     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
$+55     007A9B75     8BD3                      MOV EDX,EBX
$+57     007A9B77     50                        PUSH EAX
$+58     007A9B78     8D4D BC                   LEA ECX,DWORD PTR SS:[EBP-0x44]
$+5B     007A9B7B     E8 E015E2FF               CALL PathOfEx.005CB160
$+60     007A9B80     83C4 04                   ADD ESP,0x4
$+63     007A9B83     8B48 08                   MOV ECX,DWORD PTR DS:[EAX+0x8]
$+66     007A9B86     894D C8                   MOV DWORD PTR SS:[EBP-0x38],ECX
$+69     007A9B89     8B08                      MOV ECX,DWORD PTR DS:[EAX]
$+6B     007A9B8B     C700 00000000             MOV DWORD PTR DS:[EAX],0x0
$+71     007A9B91     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
$+74     007A9B94     894D CC                   MOV DWORD PTR SS:[EBP-0x34],ECX
$+77     007A9B97     8945 D0                   MOV DWORD PTR SS:[EBP-0x30],EAX
$+7A     007A9B9A     8D45 C8                   LEA EAX,DWORD PTR SS:[EBP-0x38]
$+7D     007A9B9D     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
$+81     007A9BA1     50                        PUSH EAX
$+82     007A9BA2     8D55 EC                   LEA EDX,DWORD PTR SS:[EBP-0x14]
$+85     007A9BA5     8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
$+88     007A9BA8     E8 13B5FDFF               CALL PathOfEx.007850C0
$+8D     007A9BAD     83C4 04                   ADD ESP,0x4
$+90     007A9BB0     8B18                      MOV EBX,DWORD PTR DS:[EAX]
$+92     007A9BB2     C700 00000000             MOV DWORD PTR DS:[EAX],0x0
$+98     007A9BB8     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
$+9B     007A9BBB     895D D4                   MOV DWORD PTR SS:[EBP-0x2C],EBX
$+9E     007A9BBE     8945 D8                   MOV DWORD PTR SS:[EBP-0x28],EAX
$+A1     007A9BC1     C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
$+A5     007A9BC5     8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
$+A8     007A9BC8     85C0                      TEST EAX,EAX
$+AA     007A9BCA     74 18                     JE SHORT PathOfEx.007A9BE4
$+AC     007A9BCC     50                        PUSH EAX
$+AD     007A9BCD     51                        PUSH ECX
$+AE     007A9BCE     E8 2DE4F6FF               CALL PathOfEx.00718000
$+B3     007A9BD3     83C4 04                   ADD ESP,0x4
$+B6     007A9BD6     8BC8                      MOV ECX,EAX
$+B8     007A9BD8     E8 B3DFF6FF               CALL PathOfEx.00717B90
$+BD     007A9BDD     C745 DC 00000000          MOV DWORD PTR SS:[EBP-0x24],0x0
$+C4     007A9BE4     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
$+C8     007A9BE8     8B45 CC                   MOV EAX,DWORD PTR SS:[EBP-0x34]
$+CB     007A9BEB     85C0                      TEST EAX,EAX
$+CD     007A9BED     74 11                     JE SHORT PathOfEx.007A9C00
$+CF     007A9BEF     50                        PUSH EAX
$+D0     007A9BF0     51                        PUSH ECX
$+D1     007A9BF1     E8 0AE4F6FF               CALL PathOfEx.00718000
$+D6     007A9BF6     83C4 04                   ADD ESP,0x4
$+D9     007A9BF9     8BC8                      MOV ECX,EAX
$+DB     007A9BFB     E8 90DFF6FF               CALL PathOfEx.00717B90
$+E0     007A9C00     C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
$+E4     007A9C04     8B45 BC                   MOV EAX,DWORD PTR SS:[EBP-0x44]
$+E7     007A9C07     85C0                      TEST EAX,EAX
$+E9     007A9C09     74 11                     JE SHORT PathOfEx.007A9C1C
$+EB     007A9C0B     50                        PUSH EAX
$+EC     007A9C0C     51                        PUSH ECX
$+ED     007A9C0D     E8 EEE3F6FF               CALL PathOfEx.00718000
$+F2     007A9C12     83C4 04                   ADD ESP,0x4
$+F5     007A9C15     8BC8                      MOV ECX,EAX
$+F7     007A9C17     E8 74DFF6FF               CALL PathOfEx.00717B90
$+FC     007A9C1C     8D55 D4                   LEA EDX,DWORD PTR SS:[EBP-0x2C]
$+FF     007A9C1F     8BCF                      MOV ECX,EDI
$+101    007A9C21     E8 5A000000               CALL PathOfEx.007A9C80
$+106    007A9C26     C745 E8 01000000          MOV DWORD PTR SS:[EBP-0x18],0x1
$+10D    007A9C2D     C645 FC 07                MOV BYTE PTR SS:[EBP-0x4],0x7
$+111    007A9C31     85DB                      TEST EBX,EBX
$+113    007A9C33     74 11                     JE SHORT PathOfEx.007A9C46
$+115    007A9C35     53                        PUSH EBX
$+116    007A9C36     51                        PUSH ECX
$+117    007A9C37     E8 C4E3F6FF               CALL PathOfEx.00718000
$+11C    007A9C3C     83C4 04                   ADD ESP,0x4
$+11F    007A9C3F     8BC8                      MOV ECX,EAX
$+121    007A9C41     E8 4ADFF6FF               CALL PathOfEx.00717B90
$+126    007A9C46     C745 FC 08000000          MOV DWORD PTR SS:[EBP-0x4],0x8
$+12D    007A9C4D     85F6                      TEST ESI,ESI
$+12F    007A9C4F     74 11                     JE SHORT PathOfEx.007A9C62
$+131    007A9C51     56                        PUSH ESI
$+132    007A9C52     51                        PUSH ECX
$+133    007A9C53     E8 A8E3F6FF               CALL PathOfEx.00718000
$+138    007A9C58     83C4 04                   ADD ESP,0x4
$+13B    007A9C5B     8BC8                      MOV ECX,EAX
$+13D    007A9C5D     E8 2EDFF6FF               CALL PathOfEx.00717B90
$+142    007A9C62     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
$+145    007A9C65     8BC7                      MOV EAX,EDI
$+147    007A9C67     5F                        POP EDI
$+148    007A9C68     5E                        POP ESI
$+149    007A9C69     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
$+150    007A9C70     5B                        POP EBX
$+151    007A9C71     8BE5                      MOV ESP,EBP
$+153    007A9C73     5D                        POP EBP
$+154    007A9C74     C3                        RETN
$+155    007A9C75     CC                        INT3
$+156    007A9C76     CC                        INT3
$+157    007A9C77     CC                        INT3
$+158    007A9C78     CC                        INT3
$+159    007A9C79     CC                        INT3
$+15A    007A9C7A     CC                        INT3
$+15B    007A9C7B     CC                        INT3
$+15C    007A9C7C     CC                        INT3
$+15D    007A9C7D     CC                        INT3
$+15E    007A9C7E     CC                        INT3
$+15F    007A9C7F     CC                        INT3
$+160    007A9C80     57                        PUSH EDI
$+161    007A9C81     8B3A                      MOV EDI,DWORD PTR DS:[EDX]
$+163    007A9C83     8939                      MOV DWORD PTR DS:[ECX],EDI
$+165    007A9C85     85FF                      TEST EDI,EDI
$+167    007A9C87     74 16                     JE SHORT PathOfEx.007A9C9F
$+169    007A9C89     56                        PUSH ESI
$+16A    007A9C8A     8D77 38                   LEA ESI,DWORD PTR DS:[EDI+0x38]
$+16D    007A9C8D     B8 01000000               MOV EAX,0x1
$+172    007A9C92     F0:0FC106                 LOCK XADD DWORD PTR DS:[ESI],EAX
$+176    007A9C96     A1 0C67FD00               MOV EAX,DWORD PTR DS:[0xFD670C]
$+17B    007A9C9B     8947 34                   MOV DWORD PTR DS:[EDI+0x34],EAX
$+17E    007A9C9E     5E                        POP ESI
$+17F    007A9C9F     8B42 04                   MOV EAX,DWORD PTR DS:[EDX+0x4]
$+182    007A9CA2     8941 04                   MOV DWORD PTR DS:[ECX+0x4],EAX
$+185    007A9CA5     8BC1                      MOV EAX,ECX
$+187    007A9CA7     C741 08 00000000          MOV DWORD PTR DS:[ECX+0x8],0x0
$+18E    007A9CAE     5F                        POP EDI
$+18F    007A9CAF     C3                        RETN
$+190    007A9CB0     55                        PUSH EBP
$+191    007A9CB1     8BEC                      MOV EBP,ESP
$+193    007A9CB3     6A FF                     PUSH -0x1
$+195    007A9CB5     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
$+19B    007A9CBB     68 CC43D500               PUSH PathOfEx.00D543CC
$+1A0    007A9CC0     50                        PUSH EAX
$+1A1    007A9CC1     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
$+1A8    007A9CC8     83EC 1C                   SUB ESP,0x1C
$+1AB    007A9CCB     53                        PUSH EBX
$+1AC    007A9CCC     56                        PUSH ESI
$+1AD    007A9CCD     57                        PUSH EDI
$+1AE    007A9CCE     6A 04                     PUSH 0x4
$+1B0    007A9CD0     8BFA                      MOV EDI,EDX
$+1B2    007A9CD2     8BD9                      MOV EBX,ECX
$+1B4    007A9CD4     E8 576ACFFF               CALL PathOfEx.004A0730
$+1B9    007A9CD9     8B43 08                   MOV EAX,DWORD PTR DS:[EBX+0x8]
$+1BC    007A9CDC     8BCB                      MOV ECX,EBX
$+1BE    007A9CDE     8B33                      MOV ESI,DWORD PTR DS:[EBX]
$+1C0    007A9CE0     6A 04                     PUSH 0x4
$+1C2    007A9CE2     8B0406                    MOV EAX,DWORD PTR DS:[ESI+EAX]
$+1C5    007A9CE5     8947 08                   MOV DWORD PTR DS:[EDI+0x8],EAX
$+1C8    007A9CE8     8343 08 04                ADD DWORD PTR DS:[EBX+0x8],0x4
$+1CC    007A9CEC     837F 08 00                CMP DWORD PTR DS:[EDI+0x8],0x0
$+1D0    007A9CF0     0F84 E3000000             JE PathOfEx.007A9DD9
$+1D6    007A9CF6     E8 356ACFFF               CALL PathOfEx.004A0730
$+1DB    007A9CFB     8B4B 08                   MOV ECX,DWORD PTR DS:[EBX+0x8]
$+1DE    007A9CFE     83EC 10                   SUB ESP,0x10
$+1E1    007A9D01     8B03                      MOV EAX,DWORD PTR DS:[EBX]
$+1E3    007A9D03     8B0401                    MOV EAX,DWORD PTR DS:[ECX+EAX]
$+1E6    007A9D06     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
$+1E9    007A9D09     8D41 04                   LEA EAX,DWORD PTR DS:[ECX+0x4]
$+1EC    007A9D0C     8943 08                   MOV DWORD PTR DS:[EBX+0x8],EAX
$+1EF    007A9D0F     E8 ECE2F6FF               CALL PathOfEx.00718000
$+1F4    007A9D14     8BC8                      MOV ECX,EAX
$+1F6    007A9D16     E8 C5F9E7FF               CALL PathOfEx.006296E0
$+1FB    007A9D1B     8BF0                      MOV ESI,EAX
$+1FD    007A9D1D     8975 EC                   MOV DWORD PTR SS:[EBP-0x14],ESI
$+200    007A9D20     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
$+207    007A9D27     85F6                      TEST ESI,ESI
$+209    007A9D29     74 07                     JE SHORT PathOfEx.007A9D32
$+20B    007A9D2B     8BCE                      MOV ECX,ESI
$+20D    007A9D2D     E8 8EF8F6FF               CALL PathOfEx.007195C0
$+212    007A9D32     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
$+215    007A9D35     8D55 EC                   LEA EDX,DWORD PTR SS:[EBP-0x14]
$+218    007A9D38     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
$+21B    007A9D3B     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+21E    007A9D3E     8D45 F0                   LEA EAX,DWORD PTR SS:[EBP-0x10]
$+221    007A9D41     50                        PUSH EAX
$+222    007A9D42     E8 590BF7FF               CALL PathOfEx.0071A8A0
$+227    007A9D47     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
$+22B    007A9D4B     8D4D D8                   LEA ECX,DWORD PTR SS:[EBP-0x28]
$+22E    007A9D4E     FF77 08                   PUSH DWORD PTR DS:[EDI+0x8]
$+231    007A9D51     8BD0                      MOV EDX,EAX
$+233    007A9D53     E8 E8FBFFFF               CALL PathOfEx.007A9940
$+238    007A9D58     8BC8                      MOV ECX,EAX
$+23A    007A9D5A     83C4 08                   ADD ESP,0x8
$+23D    007A9D5D     894D F0                   MOV DWORD PTR SS:[EBP-0x10],ECX
$+240    007A9D60     3BF9                      CMP EDI,ECX
$+242    007A9D62     74 26                     JE SHORT PathOfEx.007A9D8A
$+244    007A9D64     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+246    007A9D66     85C0                      TEST EAX,EAX
$+248    007A9D68     74 16                     JE SHORT PathOfEx.007A9D80
$+24A    007A9D6A     50                        PUSH EAX
$+24B    007A9D6B     E8 90E2F6FF               CALL PathOfEx.00718000
$+250    007A9D70     8BC8                      MOV ECX,EAX
$+252    007A9D72     E8 19DEF6FF               CALL PathOfEx.00717B90
$+257    007A9D77     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
$+25A    007A9D7A     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
$+260    007A9D80     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+262    007A9D82     8907                      MOV DWORD PTR DS:[EDI],EAX
$+264    007A9D84     C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+26A    007A9D8A     8B41 04                   MOV EAX,DWORD PTR DS:[ECX+0x4]
$+26D    007A9D8D     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
$+270    007A9D90     8B41 08                   MOV EAX,DWORD PTR DS:[ECX+0x8]
$+273    007A9D93     8947 08                   MOV DWORD PTR DS:[EDI+0x8],EAX
$+276    007A9D96     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
$+27A    007A9D9A     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
$+27D    007A9D9D     85C0                      TEST EAX,EAX
$+27F    007A9D9F     74 14                     JE SHORT PathOfEx.007A9DB5
$+281    007A9DA1     50                        PUSH EAX
$+282    007A9DA2     E8 59E2F6FF               CALL PathOfEx.00718000
$+287    007A9DA7     8BC8                      MOV ECX,EAX
$+289    007A9DA9     E8 E2DDF6FF               CALL PathOfEx.00717B90
$+28E    007A9DAE     C745 D8 00000000          MOV DWORD PTR SS:[EBP-0x28],0x0
$+295    007A9DB5     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
$+299    007A9DB9     8B45 E4                   MOV EAX,DWORD PTR SS:[EBP-0x1C]
$+29C    007A9DBC     85C0                      TEST EAX,EAX
$+29E    007A9DBE     74 0D                     JE SHORT PathOfEx.007A9DCD
$+2A0    007A9DC0     50                        PUSH EAX
$+2A1    007A9DC1     E8 3AE2F6FF               CALL PathOfEx.00718000
$+2A6    007A9DC6     8BC8                      MOV ECX,EAX
$+2A8    007A9DC8     E8 C3DDF6FF               CALL PathOfEx.00717B90
$+2AD    007A9DCD     C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4],0x4
$+2B4    007A9DD4     E9 D0000000               JMP PathOfEx.007A9EA9
$+2B9    007A9DD9     E8 5269CFFF               CALL PathOfEx.004A0730
$+2BE    007A9DDE     8B4B 08                   MOV ECX,DWORD PTR DS:[EBX+0x8]
$+2C1    007A9DE1     83EC 10                   SUB ESP,0x10
$+2C4    007A9DE4     8B03                      MOV EAX,DWORD PTR DS:[EBX]
$+2C6    007A9DE6     8B0401                    MOV EAX,DWORD PTR DS:[ECX+EAX]
$+2C9    007A9DE9     8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
$+2CC    007A9DEC     8D41 04                   LEA EAX,DWORD PTR DS:[ECX+0x4]
$+2CF    007A9DEF     8943 08                   MOV DWORD PTR DS:[EBX+0x8],EAX
$+2D2    007A9DF2     E8 09E2F6FF               CALL PathOfEx.00718000
$+2D7    007A9DF7     8BC8                      MOV ECX,EAX
$+2D9    007A9DF9     E8 42F5FCFF               CALL PathOfEx.00779340
$+2DE    007A9DFE     8BF0                      MOV ESI,EAX
$+2E0    007A9E00     8975 F0                   MOV DWORD PTR SS:[EBP-0x10],ESI
$+2E3    007A9E03     C745 FC 05000000          MOV DWORD PTR SS:[EBP-0x4],0x5
$+2EA    007A9E0A     85F6                      TEST ESI,ESI
$+2EC    007A9E0C     74 07                     JE SHORT PathOfEx.007A9E15
$+2EE    007A9E0E     8BCE                      MOV ECX,ESI
$+2F0    007A9E10     E8 ABF7F6FF               CALL PathOfEx.007195C0
$+2F5    007A9E15     FF75 EC                   PUSH DWORD PTR SS:[EBP-0x14]
$+2F8    007A9E18     8D55 F0                   LEA EDX,DWORD PTR SS:[EBP-0x10]
$+2FB    007A9E1B     8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
$+2FE    007A9E1E     E8 0D340000               CALL PathOfEx.007AD230
$+303    007A9E23     83C4 04                   ADD ESP,0x4
$+306    007A9E26     8D4D D8                   LEA ECX,DWORD PTR SS:[EBP-0x28]
$+309    007A9E29     8BD0                      MOV EDX,EAX
$+30B    007A9E2B     E8 50FEFFFF               CALL PathOfEx.007A9C80
$+310    007A9E30     8BC8                      MOV ECX,EAX
$+312    007A9E32     894D EC                   MOV DWORD PTR SS:[EBP-0x14],ECX
$+315    007A9E35     3BF9                      CMP EDI,ECX
$+317    007A9E37     74 26                     JE SHORT PathOfEx.007A9E5F
$+319    007A9E39     8B07                      MOV EAX,DWORD PTR DS:[EDI]
$+31B    007A9E3B     85C0                      TEST EAX,EAX
$+31D    007A9E3D     74 16                     JE SHORT PathOfEx.007A9E55
$+31F    007A9E3F     50                        PUSH EAX
$+320    007A9E40     E8 BBE1F6FF               CALL PathOfEx.00718000
$+325    007A9E45     8BC8                      MOV ECX,EAX
$+327    007A9E47     E8 44DDF6FF               CALL PathOfEx.00717B90
$+32C    007A9E4C     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
$+32F    007A9E4F     C707 00000000             MOV DWORD PTR DS:[EDI],0x0
$+335    007A9E55     8B01                      MOV EAX,DWORD PTR DS:[ECX]
$+337    007A9E57     8907                      MOV DWORD PTR DS:[EDI],EAX
$+339    007A9E59     C701 00000000             MOV DWORD PTR DS:[ECX],0x0
$+33F    007A9E5F     8B41 04                   MOV EAX,DWORD PTR DS:[ECX+0x4]
$+342    007A9E62     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
$+345    007A9E65     8B41 08                   MOV EAX,DWORD PTR DS:[ECX+0x8]
$+348    007A9E68     8947 08                   MOV DWORD PTR DS:[EDI+0x8],EAX
$+34B    007A9E6B     C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
$+34F    007A9E6F     8B45 D8                   MOV EAX,DWORD PTR SS:[EBP-0x28]
$+352    007A9E72     85C0                      TEST EAX,EAX
$+354    007A9E74     74 14                     JE SHORT PathOfEx.007A9E8A
*/

//tag_有物品没物品都会断到

//call tag_放下物品到目标

//tag_放下物品到目标

//stCD_Vtbl_UiGridPanel.kPress_PutItemDown
/*
$-1F1    0125137F     CC                       INT3
$-1F0    01251380     55                       PUSH EBP
$-1EF    01251381     8BEC                     MOV EBP,ESP
$-1ED    01251383     83E4 F8                  AND ESP,0xFFFFFFF8
$-1EA    01251386     83EC 14                  SUB ESP,0x14
$-1E7    01251389     53                       PUSH EBX
$-1E6    0125138A     56                       PUSH ESI
$-1E5    0125138B     57                       PUSH EDI
$-1E4    0125138C     8BF9                     MOV EDI,ECX
$-1E2    0125138E     8D4C24 10                LEA ECX,DWORD PTR SS:[ESP+0x10]
$-1DE    01251392     51                       PUSH ECX
$-1DD    01251393     8BCF                     MOV ECX,EDI
$-1DB    01251395     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$-1D9    01251397     FF50 44                  CALL DWORD PTR DS:[EAX+0x44]
$-1D6    0125139A     8BF0                     MOV ESI,EAX
$-1D4    0125139C     8BCF                     MOV ECX,EDI
$-1D2    0125139E     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
$-1CE    012513A2     50                       PUSH EAX
$-1CD    012513A3     E8 688B4500              CALL PathOfEx.016A9F10
$-1C8    012513A8     F3:0F1016                MOVSS XMM2,DWORD PTR DS:[ESI]
$-1C4    012513AC     8BCF                     MOV ECX,EDI
$-1C2    012513AE     F3:0F105E 04             MOVSS XMM3,DWORD PTR DS:[ESI+0x4]
$-1BD    012513B3     F3:0F1020                MOVSS XMM4,DWORD PTR DS:[EAX]
$-1B9    012513B7     F3:0F1068 04             MOVSS XMM5,DWORD PTR DS:[EAX+0x4]
$-1B4    012513BC     F3:0F58D4                ADDSS XMM2,XMM4
$-1B0    012513C0     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
$-1AC    012513C4     50                       PUSH EAX
$-1AB    012513C5     F3:0F58DD                ADDSS XMM3,XMM5
$-1A7    012513C9     E8 92B5F2FF              CALL PathOfEx.0117C960
$-1A2    012513CE     F3:0F107424 10           MOVSS XMM6,DWORD PTR SS:[ESP+0x10]
$-19C    012513D4     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
$-199    012513D7     0F28C6                   MOVAPS XMM0,XMM6
$-196    012513DA     F3:0F59C4                MULSS XMM0,XMM4
$-192    012513DE     F3:0F106424 14           MOVSS XMM4,DWORD PTR SS:[ESP+0x14]
$-18C    012513E4     F3:0F1048 10             MOVSS XMM1,DWORD PTR DS:[EAX+0x10]
$-187    012513E9     0F28FC                   MOVAPS XMM7,XMM4
$-184    012513EC     F3:0F59F2                MULSS XMM6,XMM2
$-180    012513F0     F3:0F1050 0C             MOVSS XMM2,DWORD PTR DS:[EAX+0xC]
$-17B    012513F5     0F2FD0                   COMISS XMM2,XMM0
$-178    012513F8     F3:0F59FD                MULSS XMM7,XMM5
$-174    012513FC     F3:0F59E3                MULSS XMM4,XMM3
$-170    01251400     0F82 B1000000            JB PathOfEx.012514B7
$-16A    01251406     0F2FCF                   COMISS XMM1,XMM7
$-167    01251409     0F82 A8000000            JB PathOfEx.012514B7
$-161    0125140F     0F2FF2                   COMISS XMM6,XMM2
$-15E    01251412     0F86 9F000000            JBE PathOfEx.012514B7
$-158    01251418     0F2FE1                   COMISS XMM4,XMM1
$-155    0125141B     0F86 96000000            JBE PathOfEx.012514B7
$-14F    01251421     8B9F 5C070000            MOV EBX,DWORD PTR DS:[EDI+0x75C]
$-149    01251427     80BB 26080000 00         CMP BYTE PTR DS:[EBX+0x826],0x0
$-142    0125142E     0F84 AE000000            JE PathOfEx.012514E2
$-13C    01251434     8B03                     MOV EAX,DWORD PTR DS:[EBX]
$-13A    01251436     8D4C24 18                LEA ECX,DWORD PTR SS:[ESP+0x18]
$-136    0125143A     51                       PUSH ECX
$-135    0125143B     8BCB                     MOV ECX,EBX
$-133    0125143D     FF50 44                  CALL DWORD PTR DS:[EAX+0x44]
$-130    01251440     8BF0                     MOV ESI,EAX
$-12E    01251442     8BCB                     MOV ECX,EBX
$-12C    01251444     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
$-128    01251448     50                       PUSH EAX
$-127    01251449     E8 C28A4500              CALL PathOfEx.016A9F10
$-122    0125144E     F3:0F1016                MOVSS XMM2,DWORD PTR DS:[ESI]
$-11E    01251452     8BCB                     MOV ECX,EBX
$-11C    01251454     F3:0F105E 04             MOVSS XMM3,DWORD PTR DS:[ESI+0x4]
$-117    01251459     F3:0F1020                MOVSS XMM4,DWORD PTR DS:[EAX]
$-113    0125145D     F3:0F1068 04             MOVSS XMM5,DWORD PTR DS:[EAX+0x4]
$-10E    01251462     F3:0F58D4                ADDSS XMM2,XMM4
$-10A    01251466     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
$-106    0125146A     50                       PUSH EAX
$-105    0125146B     F3:0F58DD                ADDSS XMM3,XMM5
$-101    0125146F     E8 ECB4F2FF              CALL PathOfEx.0117C960
$-FC     01251474     F3:0F107424 10           MOVSS XMM6,DWORD PTR SS:[ESP+0x10]
$-F6     0125147A     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
$-F3     0125147D     0F28C6                   MOVAPS XMM0,XMM6
$-F0     01251480     F3:0F59C4                MULSS XMM0,XMM4
$-EC     01251484     F3:0F106424 14           MOVSS XMM4,DWORD PTR SS:[ESP+0x14]
$-E6     0125148A     F3:0F1048 10             MOVSS XMM1,DWORD PTR DS:[EAX+0x10]
$-E1     0125148F     0F28FC                   MOVAPS XMM7,XMM4
$-DE     01251492     F3:0F59F2                MULSS XMM6,XMM2
$-DA     01251496     F3:0F1050 0C             MOVSS XMM2,DWORD PTR DS:[EAX+0xC]
$-D5     0125149B     0F2FD0                   COMISS XMM2,XMM0
$-D2     0125149E     F3:0F59FD                MULSS XMM7,XMM5
$-CE     012514A2     F3:0F59E3                MULSS XMM4,XMM3
$-CA     012514A6     72 0F                    JB SHORT PathOfEx.012514B7
$-C8     012514A8     0F2FCF                   COMISS XMM1,XMM7
$-C5     012514AB     72 0A                    JB SHORT PathOfEx.012514B7
$-C3     012514AD     0F2FF2                   COMISS XMM6,XMM2
$-C0     012514B0     76 05                    JBE SHORT PathOfEx.012514B7
$-BE     012514B2     0F2FE1                   COMISS XMM4,XMM1
$-BB     012514B5     77 2B                    JA SHORT PathOfEx.012514E2
$-B9     012514B7     C687 8C090000 00         MOV BYTE PTR DS:[EDI+0x98C],0x0
$-B2     012514BE     C787 68090000 00000000   MOV DWORD PTR DS:[EDI+0x968],0x0
$-A8     012514C8     C787 70090000 00000000   MOV DWORD PTR DS:[EDI+0x970],0x0
$-9E     012514D2     C787 6C090000 00000000   MOV DWORD PTR DS:[EDI+0x96C],0x0
$-94     012514DC     8B7D 0C                  MOV EDI,DWORD PTR SS:[EBP+0xC]
$-91     012514DF     C607 01                  MOV BYTE PTR DS:[EDI],0x1
$-8E     012514E2     5F                       POP EDI
$-8D     012514E3     5E                       POP ESI
$-8C     012514E4     5B                       POP EBX
$-8B     012514E5     8BE5                     MOV ESP,EBP
$-89     012514E7     5D                       POP EBP
$-88     012514E8     C2 0800                  RETN 0x8
$-85     012514EB     CC                       INT3
$-84     012514EC     CC                       INT3
$-83     012514ED     CC                       INT3
$-82     012514EE     CC                       INT3
$-81     012514EF     CC                       INT3
$-80     012514F0     51                       PUSH ECX
$-7F     012514F1     8B4424 0C                MOV EAX,DWORD PTR SS:[ESP+0xC]
$-7B     012514F5     56                       PUSH ESI
$-7A     012514F6     8BF1                     MOV ESI,ECX
$-78     012514F8     57                       PUSH EDI
$-77     012514F9     C600 01                  MOV BYTE PTR DS:[EAX],0x1
$-74     012514FC     8BBE 080A0000            MOV EDI,DWORD PTR DS:[ESI+0xA08]
$-6E     01251502     85FF                     TEST EDI,EDI
$-6C     01251504     74 62                    JE SHORT PathOfEx.01251568
$-6A     01251506     F3:0F1048 0C             MOVSS XMM1,DWORD PTR DS:[EAX+0xC]
$-65     0125150B     F3:0F1040 10             MOVSS XMM0,DWORD PTR DS:[EAX+0x10]
$-60     01251510     F3:0F5C86 140A0000       SUBSS XMM0,DWORD PTR DS:[ESI+0xA14]
$-58     01251518     F3:0F5C8E 100A0000       SUBSS XMM1,DWORD PTR DS:[ESI+0xA10]
$-50     01251520     F3:0F59C0                MULSS XMM0,XMM0
$-4C     01251524     F3:0F59C9                MULSS XMM1,XMM1
$-48     01251528     F3:0F58C1                ADDSS XMM0,XMM1
$-44     0125152C     E8 FFCBF0FF              CALL PathOfEx.0115E130
$-3F     01251531     0F2F86 180A0000          COMISS XMM0,DWORD PTR DS:[ESI+0xA18]
$-38     01251538     76 2E                    JBE SHORT PathOfEx.01251568
$-36     0125153A     E8 511D0000              CALL PathOfEx.01253290
$-31     0125153F     8B8E 50090000            MOV ECX,DWORD PTR DS:[ESI+0x950]
$-2B     01251545     85C9                     TEST ECX,ECX
$-29     01251547     74 1F                    JE SHORT PathOfEx.01251568
$-27     01251549     85FF                     TEST EDI,EDI
$-25     0125154B     74 1B                    JE SHORT PathOfEx.01251568
$-23     0125154D     57                       PUSH EDI
$-22     0125154E     E8 1DD81A00              CALL PathOfEx.013FED70
$-1D     01251553     85C0                     TEST EAX,EAX
$-1B     01251555     74 11                    JE SHORT PathOfEx.01251568
$-19     01251557     8078 10 00               CMP BYTE PTR DS:[EAX+0x10],0x0
$-15     0125155B     75 0B                    JNZ SHORT PathOfEx.01251568
$-13     0125155D     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$-11     0125155F     8BCE                     MOV ECX,ESI
$-F      01251561     57                       PUSH EDI
$-E      01251562     FF90 64010000            CALL DWORD PTR DS:[EAX+0x164]
$-8      01251568     5F                       POP EDI
$-7      01251569     5E                       POP ESI
$-6      0125156A     59                       POP ECX
$-5      0125156B     C2 0800                  RETN 0x8
$-2      0125156E     CC                       INT3
$-1      0125156F     CC                       INT3
$ ==>    01251570     51                       PUSH ECX                                 ; tag_有物品没物品都会断到
$+1      01251571     56                       PUSH ESI
$+2      01251572     8B7424 10                MOV ESI,DWORD PTR SS:[ESP+0x10]
$+6      01251576     57                       PUSH EDI
$+7      01251577     8BF9                     MOV EDI,ECX
$+9      01251579     803E 00                  CMP BYTE PTR DS:[ESI],0x0
$+C      0125157C     75 54                    JNZ SHORT PathOfEx.012515D2
$+E      0125157E     837E 14 01               CMP DWORD PTR DS:[ESI+0x14],0x1
$+12     01251582     C606 01                  MOV BYTE PTR DS:[ESI],0x1
$+15     01251585     74 31                    JE SHORT PathOfEx.012515B8
$+17     01251587     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+1D     0125158D     83B9 34090000 00         CMP DWORD PTR DS:[ECX+0x934],0x0
$+24     01251594     74 1C                    JE SHORT PathOfEx.012515B2
$+26     01251596     83B9 38090000 01         CMP DWORD PTR DS:[ECX+0x938],0x1
$+2D     0125159D     74 13                    JE SHORT PathOfEx.012515B2
$+2F     0125159F     6A FF                    PUSH -0x1
$+31     012515A1     6A 00                    PUSH 0x0
$+33     012515A3     6A 00                    PUSH 0x0
$+35     012515A5     6A 00                    PUSH 0x0
$+37     012515A7     6A 00                    PUSH 0x0
$+39     012515A9     6A 00                    PUSH 0x0
$+3B     012515AB     6A 00                    PUSH 0x0
$+3D     012515AD     E8 DE59FFFF              CALL PathOfEx.01246F90
$+42     012515B2     837E 14 01               CMP DWORD PTR DS:[ESI+0x14],0x1
$+46     012515B6     75 1A                    JNZ SHORT PathOfEx.012515D2
$+48     012515B8     8B87 64090000            MOV EAX,DWORD PTR DS:[EDI+0x964]
$+4E     012515BE     85C0                     TEST EAX,EAX
$+50     012515C0     74 10                    JE SHORT PathOfEx.012515D2
$+52     012515C2     83B8 38090000 00         CMP DWORD PTR DS:[EAX+0x938],0x0
$+59     012515C9     74 07                    JE SHORT PathOfEx.012515D2
$+5B     012515CB     8BCF                     MOV ECX,EDI
$+5D     012515CD     E8 0E000000              CALL PathOfEx.012515E0                   ; call tag_放下物品到目标
$+62     012515D2     5F                       POP EDI
$+63     012515D3     5E                       POP ESI
$+64     012515D4     59                       POP ECX
$+65     012515D5     C2 0800                  RETN 0x8
$+68     012515D8     CC                       INT3
$+69     012515D9     CC                       INT3
$+6A     012515DA     CC                       INT3
$+6B     012515DB     CC                       INT3
$+6C     012515DC     CC                       INT3
$+6D     012515DD     CC                       INT3
$+6E     012515DE     CC                       INT3
$+6F     012515DF     CC                       INT3
$+70     012515E0     55                       PUSH EBP                                 ; tag_放下物品到目标
$+71     012515E1     8BEC                     MOV EBP,ESP
$+73     012515E3     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
$+79     012515E9     6A FF                    PUSH -0x1
$+7B     012515EB     68 9B96A201              PUSH PathOfEx.01A2969B
$+80     012515F0     50                       PUSH EAX
$+81     012515F1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
$+88     012515F8     81EC 80000000            SUB ESP,0x80
$+8E     012515FE     53                       PUSH EBX
$+8F     012515FF     56                       PUSH ESI
$+90     01251600     57                       PUSH EDI
$+91     01251601     8BF9                     MOV EDI,ECX
$+93     01251603     83CB FF                  OR EBX,0xFFFFFFFF
$+96     01251606     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+9C     0125160C     83B9 38090000 04         CMP DWORD PTR DS:[ECX+0x938],0x4
$+A3     01251613     0F85 80010000            JNZ PathOfEx.01251799
$+A9     01251619     8BB7 50090000            MOV ESI,DWORD PTR DS:[EDI+0x950]
$+AF     0125161F     8B97 980A0000            MOV EDX,DWORD PTR DS:[EDI+0xA98]
$+B5     01251625     8B87 9C0A0000            MOV EAX,DWORD PTR DS:[EDI+0xA9C]
$+BB     0125162B     0397 7C090000            ADD EDX,DWORD PTR DS:[EDI+0x97C]
$+C1     01251631     0387 80090000            ADD EAX,DWORD PTR DS:[EDI+0x980]
$+C7     01251637     8B4E 0C                  MOV ECX,DWORD PTR DS:[ESI+0xC]
$+CA     0125163A     3BD1                     CMP EDX,ECX
$+CC     0125163C     0F83 E9010000            JNB PathOfEx.0125182B
$+D2     01251642     3B46 10                  CMP EAX,DWORD PTR DS:[ESI+0x10]
$+D5     01251645     0F83 E0010000            JNB PathOfEx.0125182B
$+DB     0125164B     0FAFC8                   IMUL ECX,EAX
$+DE     0125164E     8B46 20                  MOV EAX,DWORD PTR DS:[ESI+0x20]
$+E1     01251651     03CA                     ADD ECX,EDX
$+E3     01251653     8B0488                   MOV EAX,DWORD PTR DS:[EAX+ECX*4]
$+E6     01251656     85C0                     TEST EAX,EAX
$+E8     01251658     0F84 CD010000            JE PathOfEx.0125182B
$+EE     0125165E     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
$+F1     01251661     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
$+F4     01251664     85C0                     TEST EAX,EAX
$+F6     01251666     0F84 BF010000            JE PathOfEx.0125182B
$+FC     0125166C     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
$+FF     0125166F     8D4E 2C                  LEA ECX,DWORD PTR DS:[ESI+0x2C]
$+102    01251672     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
$+105    01251675     50                       PUSH EAX
$+106    01251676     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
$+109    01251679     50                       PUSH EAX
$+10A    0125167A     E8 61990000              CALL PathOfEx.0125AFE0
$+10F    0125167F     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
$+112    01251682     3B46 2C                  CMP EAX,DWORD PTR DS:[ESI+0x2C]
$+115    01251685     75 04                    JNZ SHORT PathOfEx.0125168B
$+117    01251687     33C9                     XOR ECX,ECX
$+119    01251689     EB 05                    JMP SHORT PathOfEx.01251690
$+11B    0125168B     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
$+11E    0125168E     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+120    01251690     E8 EBBDF2FF              CALL PathOfEx.0117D480
$+125    01251695     8BC8                     MOV ECX,EAX
$+127    01251697     E8 54871A00              CALL PathOfEx.013F9DF0
$+12C    0125169C     84C0                     TEST AL,AL
$+12E    0125169E     74 19                    JE SHORT PathOfEx.012516B9
$+130    012516A0     FF75 E4                  PUSH DWORD PTR SS:[EBP-0x1C]
$+133    012516A3     8B8F 4C090000            MOV ECX,DWORD PTR DS:[EDI+0x94C]
$+139    012516A9     FFB7 BC080000            PUSH DWORD PTR DS:[EDI+0x8BC]
$+13F    012516AF     E8 4C663300              CALL PathOfEx.01587D00
$+144    012516B4     E9 72010000              JMP PathOfEx.0125182B
$+149    012516B9     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+14C    012516BC     E8 5F89EEFF              CALL PathOfEx.0113A020
$+151    012516C1     51                       PUSH ECX
$+152    012516C2     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
$+155    012516C5     51                       PUSH ECX
$+156    012516C6     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+158    012516C8     E8 E3FFEEFF              CALL PathOfEx.011416B0
$+15D    012516CD     8B00                     MOV EAX,DWORD PTR DS:[EAX]
$+15F    012516CF     8B80 E0180000            MOV EAX,DWORD PTR DS:[EAX+0x18E0]
$+165    012516D5     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
$+168    012516D8     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
$+16F    012516DF     8B75 C0                  MOV ESI,DWORD PTR SS:[EBP-0x40]
$+172    012516E2     85F6                     TEST ESI,ESI
$+174    012516E4     74 1F                    JE SHORT PathOfEx.01251705
$+176    012516E6     8BC3                     MOV EAX,EBX
$+178    012516E8     F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+17D    012516ED     75 16                    JNZ SHORT PathOfEx.01251705
$+17F    012516EF     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+181    012516F1     8BCE                     MOV ECX,ESI
$+183    012516F3     FF10                     CALL DWORD PTR DS:[EAX]
$+185    012516F5     8BC3                     MOV EAX,EBX
$+187    012516F7     F0:0FC146 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+18C    012516FC     75 07                    JNZ SHORT PathOfEx.01251705
$+18E    012516FE     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+190    01251700     8BCE                     MOV ECX,ESI
$+192    01251702     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+195    01251705     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
$+19C    0125170C     8B75 C8                  MOV ESI,DWORD PTR SS:[EBP-0x38]
$+19F    0125170F     85F6                     TEST ESI,ESI
$+1A1    01251711     74 1F                    JE SHORT PathOfEx.01251732
$+1A3    01251713     8BC3                     MOV EAX,EBX
$+1A5    01251715     F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+1AA    0125171A     75 16                    JNZ SHORT PathOfEx.01251732
$+1AC    0125171C     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+1AE    0125171E     8BCE                     MOV ECX,ESI
$+1B0    01251720     FF10                     CALL DWORD PTR DS:[EAX]
$+1B2    01251722     8BC3                     MOV EAX,EBX
$+1B4    01251724     F0:0FC146 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+1B9    01251729     75 07                    JNZ SHORT PathOfEx.01251732
$+1BB    0125172B     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+1BD    0125172D     8BCE                     MOV ECX,ESI
$+1BF    0125172F     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+1C2    01251732     68 D40DB701              PUSH PathOfEx.01B70DD4
$+1C7    01251737     8D4D A4                  LEA ECX,DWORD PTR SS:[EBP-0x5C]
$+1CA    0125173A     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+1D1    01251741     E8 4A8FEDFF              CALL PathOfEx.0112A690
$+1D6    01251746     BA 27040000              MOV EDX,0x427
$+1DB    0125174B     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
$+1E2    01251752     8D8D 74FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0x8C]
$+1E8    01251758     E8 C38C1700              CALL PathOfEx.013CA420
$+1ED    0125175D     8B4D E4                  MOV ECX,DWORD PTR SS:[EBP-0x1C]
$+1F0    01251760     6A 00                    PUSH 0x0
$+1F2    01251762     6A 00                    PUSH 0x0
$+1F4    01251764     50                       PUSH EAX
$+1F5    01251765     8D45 A4                  LEA EAX,DWORD PTR SS:[EBP-0x5C]
$+1F8    01251768     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
$+1FC    0125176C     8B89 AC090000            MOV ECX,DWORD PTR DS:[ECX+0x9AC]
$+202    01251772     50                       PUSH EAX
$+203    01251773     6A 01                    PUSH 0x1
$+205    01251775     E8 8653FEFF              CALL PathOfEx.01236B00
$+20A    0125177A     8D8D 74FFFFFF            LEA ECX,DWORD PTR SS:[EBP-0x8C]
$+210    01251780     E8 5B88EDFF              CALL PathOfEx.01129FE0
$+215    01251785     8D4D A4                  LEA ECX,DWORD PTR SS:[EBP-0x5C]
$+218    01251788     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+21F    0125178F     E8 4C88EDFF              CALL PathOfEx.01129FE0
$+224    01251794     E9 92000000              JMP PathOfEx.0125182B
$+229    01251799     8B87 6C090000            MOV EAX,DWORD PTR DS:[EDI+0x96C]
$+22F    0125179F     83F8 01                  CMP EAX,0x1
$+232    012517A2     75 62                    JNZ SHORT PathOfEx.01251806
$+234    012517A4     83B9 38090000 03         CMP DWORD PTR DS:[ECX+0x938],0x3
$+23B    012517AB     75 37                    JNZ SHORT PathOfEx.012517E4
$+23D    012517AD     83B9 70090000 FF         CMP DWORD PTR DS:[ECX+0x970],-0x1
$+244    012517B4     74 2E                    JE SHORT PathOfEx.012517E4
$+246    012517B6     8B87 74090000            MOV EAX,DWORD PTR DS:[EDI+0x974]
$+24C    012517BC     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
$+24F    012517BF     8B87 78090000            MOV EAX,DWORD PTR DS:[EDI+0x978]
$+255    012517C5     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
$+258    012517C8     FFB1 70090000            PUSH DWORD PTR DS:[ECX+0x970]
$+25E    012517CE     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+260    012517D0     FFB1 4C090000            PUSH DWORD PTR DS:[ECX+0x94C]
$+266    012517D6     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+269    012517D9     51                       PUSH ECX
$+26A    012517DA     8BCF                     MOV ECX,EDI
$+26C    012517DC     FF90 80010000            CALL DWORD PTR DS:[EAX+0x180]
$+272    012517E2     EB 47                    JMP SHORT PathOfEx.0125182B
$+274    012517E4     8B87 74090000            MOV EAX,DWORD PTR DS:[EDI+0x974]
$+27A    012517EA     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+27D    012517ED     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
$+280    012517F0     8B87 78090000            MOV EAX,DWORD PTR DS:[EDI+0x978]
$+286    012517F6     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
$+289    012517F9     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+28B    012517FB     51                       PUSH ECX
$+28C    012517FC     8BCF                     MOV ECX,EDI                              ; then,ecx=stCD_UiGridPanel*
$+28E    012517FE     FF90 7C010000            CALL DWORD PTR DS:[EAX+0x17C]            ; stCD_Vtbl_UiGridPanel.kPress_PutItemDown
$+294    01251804     EB 25                    JMP SHORT PathOfEx.0125182B
$+296    01251806     83F8 02                  CMP EAX,0x2
$+299    01251809     75 58                    JNZ SHORT PathOfEx.01251863
$+29B    0125180B     8B87 74090000            MOV EAX,DWORD PTR DS:[EDI+0x974]
$+2A1    01251811     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+2A4    01251814     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
$+2A7    01251817     8B87 78090000            MOV EAX,DWORD PTR DS:[EDI+0x978]
$+2AD    0125181D     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
$+2B0    01251820     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+2B2    01251822     51                       PUSH ECX
$+2B3    01251823     8BCF                     MOV ECX,EDI
$+2B5    01251825     FF90 8C010000            CALL DWORD PTR DS:[EAX+0x18C]
$+2BB    0125182B     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+2C1    01251831     83B9 34090000 00         CMP DWORD PTR DS:[ECX+0x934],0x0
$+2C8    01251838     0F84 AB020000            JE PathOfEx.01251AE9
$+2CE    0125183E     83B9 38090000 01         CMP DWORD PTR DS:[ECX+0x938],0x1
$+2D5    01251845     0F84 9E020000            JE PathOfEx.01251AE9
$+2DB    0125184B     6A FF                    PUSH -0x1
$+2DD    0125184D     6A 00                    PUSH 0x0
$+2DF    0125184F     6A 00                    PUSH 0x0
$+2E1    01251851     6A 00                    PUSH 0x0
$+2E3    01251853     6A 00                    PUSH 0x0
$+2E5    01251855     6A 00                    PUSH 0x0
$+2E7    01251857     6A 00                    PUSH 0x0
$+2E9    01251859     E8 3257FFFF              CALL PathOfEx.01246F90
$+2EE    0125185E     E9 86020000              JMP PathOfEx.01251AE9
$+2F3    01251863     83F8 03                  CMP EAX,0x3
$+2F6    01251866     75 5D                    JNZ SHORT PathOfEx.012518C5
$+2F8    01251868     8B97 68090000            MOV EDX,DWORD PTR DS:[EDI+0x968]
$+2FE    0125186E     85D2                     TEST EDX,EDX
$+300    01251870     0F84 73020000            JE PathOfEx.01251AE9
$+306    01251876     E8 65C5FFFF              CALL PathOfEx.0124DDE0
$+30B    0125187B     84C0                     TEST AL,AL
$+30D    0125187D     74 31                    JE SHORT PathOfEx.012518B0
$+30F    0125187F     8B87 74090000            MOV EAX,DWORD PTR DS:[EDI+0x974]
$+315    01251885     8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
$+318    01251888     8B87 78090000            MOV EAX,DWORD PTR DS:[EDI+0x978]
$+31E    0125188E     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
$+321    01251891     FFB1 70090000            PUSH DWORD PTR DS:[ECX+0x970]
$+327    01251897     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+329    01251899     FFB1 4C090000            PUSH DWORD PTR DS:[ECX+0x94C]
$+32F    0125189F     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+332    012518A2     51                       PUSH ECX
$+333    012518A3     8BCF                     MOV ECX,EDI
$+335    012518A5     FF90 80010000            CALL DWORD PTR DS:[EAX+0x180]
$+33B    012518AB     E9 2E020000              JMP PathOfEx.01251ADE
$+340    012518B0     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+342    012518B2     8BCF                     MOV ECX,EDI
$+344    012518B4     FFB2 580A0000            PUSH DWORD PTR DS:[EDX+0xA58]
$+34A    012518BA     FF90 88010000            CALL DWORD PTR DS:[EAX+0x188]
$+350    012518C0     E9 19020000              JMP PathOfEx.01251ADE
$+355    012518C5     83F8 04                  CMP EAX,0x4
$+358    012518C8     0F85 E1010000            JNZ PathOfEx.01251AAF
$+35E    012518CE     8B81 4C090000            MOV EAX,DWORD PTR DS:[ECX+0x94C]
$+364    012518D4     8BB1 40090000            MOV ESI,DWORD PTR DS:[ECX+0x940]
$+36A    012518DA     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
$+36D    012518DD     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
$+370    012518E0     50                       PUSH EAX
$+371    012518E1     8975 E4                  MOV DWORD PTR SS:[EBP-0x1C],ESI
$+374    012518E4     E8 975CFFFF              CALL PathOfEx.01247580
$+379    012518E9     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
$+380    012518F0     8B87 64090000            MOV EAX,DWORD PTR DS:[EDI+0x964]
$+386    012518F6     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
$+389    012518F9     8A80 78090000            MOV AL,BYTE PTR DS:[EAX+0x978]
$+38F    012518FF     8845 F3                  MOV BYTE PTR SS:[EBP-0xD],AL
$+392    01251902     3935 9CDFCF01            CMP DWORD PTR DS:[0x1CFDF9C],ESI
$+398    01251908     75 08                    JNZ SHORT PathOfEx.01251912
$+39A    0125190A     390D A0DFCF01            CMP DWORD PTR DS:[0x1CFDFA0],ECX
$+3A0    01251910     74 16                    JE SHORT PathOfEx.01251928
$+3A2    01251912     A1 E4ACD101              MOV EAX,DWORD PTR DS:[0x1D1ACE4]
$+3A7    01251917     A3 E8ACD101              MOV DWORD PTR DS:[0x1D1ACE8],EAX
$+3AC    0125191C     8935 9CDFCF01            MOV DWORD PTR DS:[0x1CFDF9C],ESI
$+3B2    01251922     890D A0DFCF01            MOV DWORD PTR DS:[0x1CFDFA0],ECX
$+3B8    01251928     8B87 9C0A0000            MOV EAX,DWORD PTR DS:[EDI+0xA9C]
$+3BE    0125192E     0387 80090000            ADD EAX,DWORD PTR DS:[EDI+0x980]
$+3C4    01251934     8B8F 50090000            MOV ECX,DWORD PTR DS:[EDI+0x950]
$+3CA    0125193A     50                       PUSH EAX
$+3CB    0125193B     8B87 980A0000            MOV EAX,DWORD PTR DS:[EDI+0xA98]
$+3D1    01251941     0387 7C090000            ADD EAX,DWORD PTR DS:[EDI+0x97C]
$+3D7    01251947     50                       PUSH EAX
$+3D8    01251948     E8 F3D31A00              CALL PathOfEx.013FED40
$+3DD    0125194D     837D E4 00               CMP DWORD PTR SS:[EBP-0x1C],0x0
$+3E1    01251951     8BF0                     MOV ESI,EAX
$+3E3    01251953     0F84 34010000            JE PathOfEx.01251A8D
$+3E9    01251959     85F6                     TEST ESI,ESI
$+3EB    0125195B     0F84 2C010000            JE PathOfEx.01251A8D
$+3F1    01251961     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+3F7    01251967     8B81 40090000            MOV EAX,DWORD PTR DS:[ECX+0x940]
$+3FD    0125196D     8945 CC                  MOV DWORD PTR SS:[EBP-0x34],EAX
$+400    01251970     8B81 4C090000            MOV EAX,DWORD PTR DS:[ECX+0x94C]
$+406    01251976     8945 D0                  MOV DWORD PTR SS:[EBP-0x30],EAX
$+409    01251979     8B81 50090000            MOV EAX,DWORD PTR DS:[ECX+0x950]
$+40F    0125197F     8945 D4                  MOV DWORD PTR SS:[EBP-0x2C],EAX
$+412    01251982     8B81 58090000            MOV EAX,DWORD PTR DS:[ECX+0x958]
$+418    01251988     8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
$+41B    0125198B     8B81 5C090000            MOV EAX,DWORD PTR DS:[ECX+0x95C]
$+421    01251991     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
$+424    01251994     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
$+427    01251997     50                       PUSH EAX
$+428    01251998     E8 E35BFFFF              CALL PathOfEx.01247580
$+42D    0125199D     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
$+431    012519A1     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+437    012519A7     8B97 BC080000            MOV EDX,DWORD PTR DS:[EDI+0x8BC]
$+43D    012519AD     8A81 79090000            MOV AL,BYTE PTR DS:[ECX+0x979]
$+443    012519B3     8845 E0                  MOV BYTE PTR SS:[EBP-0x20],AL
$+446    012519B6     8A81 7A090000            MOV AL,BYTE PTR DS:[ECX+0x97A]
$+44C    012519BC     8845 E4                  MOV BYTE PTR SS:[EBP-0x1C],AL
$+44F    012519BF     8A81 78090000            MOV AL,BYTE PTR DS:[ECX+0x978]
$+455    012519C5     8B0D E4ACD101            MOV ECX,DWORD PTR DS:[0x1D1ACE4]
$+45B    012519CB     8845 EC                  MOV BYTE PTR SS:[EBP-0x14],AL
$+45E    012519CE     A1 E8ACD101              MOV EAX,DWORD PTR DS:[0x1D1ACE8]
$+463    012519D3     8955 BC                  MOV DWORD PTR SS:[EBP-0x44],EDX
$+466    012519D6     8975 C0                  MOV DWORD PTR SS:[EBP-0x40],ESI
$+469    012519D9     3BC8                     CMP ECX,EAX
$+46B    012519DB     74 13                    JE SHORT PathOfEx.012519F0
$+46D    012519DD     0F1F00                   NOP DWORD PTR DS:[EAX]
$+470    012519E0     3911                     CMP DWORD PTR DS:[ECX],EDX
$+472    012519E2     75 05                    JNZ SHORT PathOfEx.012519E9
$+474    012519E4     3971 04                  CMP DWORD PTR DS:[ECX+0x4],ESI
$+477    012519E7     74 6D                    JE SHORT PathOfEx.01251A56
$+479    012519E9     83C1 08                  ADD ECX,0x8
$+47C    012519EC     3BC8                     CMP ECX,EAX
$+47E    012519EE   ^ 75 F0                    JNZ SHORT PathOfEx.012519E0
$+480    012519F0     8D45 BC                  LEA EAX,DWORD PTR SS:[EBP-0x44]
$+483    012519F3     50                       PUSH EAX
$+484    012519F4     E8 071C0000              CALL PathOfEx.01253600
$+489    012519F9     8B87 64090000            MOV EAX,DWORD PTR DS:[EDI+0x964]
$+48F    012519FF     33D2                     XOR EDX,EDX
$+491    01251A01     8A80 69090000            MOV AL,BYTE PTR DS:[EAX+0x969]
$+497    01251A07     84C0                     TEST AL,AL
$+499    01251A09     A1 E8ACD101              MOV EAX,DWORD PTR DS:[0x1D1ACE8]
$+49E    01251A0E     8BC8                     MOV ECX,EAX
$+4A0    01251A10     0F95C2                   SETNE DL
$+4A3    01251A13     2B0D E4ACD101            SUB ECX,DWORD PTR DS:[0x1D1ACE4]
$+4A9    01251A19     42                       INC EDX
$+4AA    01251A1A     C1F9 03                  SAR ECX,0x3
$+4AD    01251A1D     3BCA                     CMP ECX,EDX
$+4AF    01251A1F     75 35                    JNZ SHORT PathOfEx.01251A56
$+4B1    01251A21     FF75 EC                  PUSH DWORD PTR SS:[EBP-0x14]
$+4B4    01251A24     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+4B6    01251A26     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
$+4B9    01251A29     51                       PUSH ECX
$+4BA    01251A2A     68 E4ACD101              PUSH PathOfEx.01D1ACE4
$+4BF    01251A2F     FF75 E4                  PUSH DWORD PTR SS:[EBP-0x1C]
$+4C2    01251A32     8BCF                     MOV ECX,EDI
$+4C4    01251A34     FF75 E0                  PUSH DWORD PTR SS:[EBP-0x20]
$+4C7    01251A37     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
$+4CA    01251A3A     FF75 D8                  PUSH DWORD PTR SS:[EBP-0x28]
$+4CD    01251A3D     FF75 D4                  PUSH DWORD PTR SS:[EBP-0x2C]
$+4D0    01251A40     FF75 D0                  PUSH DWORD PTR SS:[EBP-0x30]
$+4D3    01251A43     FF75 CC                  PUSH DWORD PTR SS:[EBP-0x34]
$+4D6    01251A46     FF90 94010000            CALL DWORD PTR DS:[EAX+0x194]
$+4DC    01251A4C     A1 E4ACD101              MOV EAX,DWORD PTR DS:[0x1D1ACE4]
$+4E1    01251A51     A3 E8ACD101              MOV DWORD PTR DS:[0x1D1ACE8],EAX
$+4E6    01251A56     3905 E4ACD101            CMP DWORD PTR DS:[0x1D1ACE4],EAX
$+4EC    01251A5C     0F94C0                   SETE AL
$+4EF    01251A5F     84C0                     TEST AL,AL
$+4F1    01251A61     74 11                    JE SHORT PathOfEx.01251A74
$+4F3    01251A63     807D EC 00               CMP BYTE PTR SS:[EBP-0x14],0x0
$+4F7    01251A67     75 0B                    JNZ SHORT PathOfEx.01251A74
$+4F9    01251A69     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+4FF    01251A6F     E8 7C5AFFFF              CALL PathOfEx.012474F0
$+504    01251A74     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
$+507    01251A77     E8 3479EDFF              CALL PathOfEx.011293B0
$+50C    01251A7C     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$+50F    01251A7F     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+516    01251A86     E8 2579EDFF              CALL PathOfEx.011293B0
$+51B    01251A8B     EB 5C                    JMP SHORT PathOfEx.01251AE9
$+51D    01251A8D     807D F3 00               CMP BYTE PTR SS:[EBP-0xD],0x0
$+521    01251A91     75 0B                    JNZ SHORT PathOfEx.01251A9E
$+523    01251A93     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+529    01251A99     E8 525AFFFF              CALL PathOfEx.012474F0
$+52E    01251A9E     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
$+531    01251AA1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
$+538    01251AA8     E8 0379EDFF              CALL PathOfEx.011293B0
$+53D    01251AAD     EB 3A                    JMP SHORT PathOfEx.01251AE9
$+53F    01251AAF     85C0                     TEST EAX,EAX
$+541    01251AB1     75 36                    JNZ SHORT PathOfEx.01251AE9
$+543    01251AB3     8B87 50090000            MOV EAX,DWORD PTR DS:[EDI+0x950]
$+549    01251AB9     8338 14                  CMP DWORD PTR DS:[EAX],0x14
$+54C    01251ABC     75 2B                    JNZ SHORT PathOfEx.01251AE9
$+54E    01251ABE     8B87 74090000            MOV EAX,DWORD PTR DS:[EDI+0x974]
$+554    01251AC4     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
$+557    01251AC7     8945 BC                  MOV DWORD PTR SS:[EBP-0x44],EAX
$+55A    01251ACA     8B87 78090000            MOV EAX,DWORD PTR DS:[EDI+0x978]
$+560    01251AD0     8945 C0                  MOV DWORD PTR SS:[EBP-0x40],EAX
$+563    01251AD3     8B07                     MOV EAX,DWORD PTR DS:[EDI]
$+565    01251AD5     51                       PUSH ECX
$+566    01251AD6     8BCF                     MOV ECX,EDI
$+568    01251AD8     FF90 7C010000            CALL DWORD PTR DS:[EAX+0x17C]
$+56E    01251ADE     8B8F 64090000            MOV ECX,DWORD PTR DS:[EDI+0x964]
$+574    01251AE4     E8 075AFFFF              CALL PathOfEx.012474F0
$+579    01251AE9     8B87 64090000            MOV EAX,DWORD PTR DS:[EDI+0x964]
$+57F    01251AEF     C787 68090000 00000000   MOV DWORD PTR DS:[EDI+0x968],0x0
$+589    01251AF9     C787 6C090000 00000000   MOV DWORD PTR DS:[EDI+0x96C],0x0
$+593    01251B03     83B8 38090000 00         CMP DWORD PTR DS:[EAX+0x938],0x0
$+59A    01251B0A     74 4D                    JE SHORT PathOfEx.01251B59
$+59C    01251B0C     8BB0 44090000            MOV ESI,DWORD PTR DS:[EAX+0x944]
$+5A2    01251B12     85F6                     TEST ESI,ESI
$+5A4    01251B14     74 43                    JE SHORT PathOfEx.01251B59
$+5A6    01251B16     8B80 4C090000            MOV EAX,DWORD PTR DS:[EAX+0x94C]
$+5AC    01251B1C     85C0                     TEST EAX,EAX
$+5AE    01251B1E     74 39                    JE SHORT PathOfEx.01251B59
$+5B0    01251B20     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
$+5B3    01251B23     8D4E 2C                  LEA ECX,DWORD PTR DS:[ESI+0x2C]
$+5B6    01251B26     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
$+5B9    01251B29     50                       PUSH EAX
$+5BA    01251B2A     8D45 CC                  LEA EAX,DWORD PTR SS:[EBP-0x34]
$+5BD    01251B2D     50                       PUSH EAX
$+5BE    01251B2E     E8 AD940000              CALL PathOfEx.0125AFE0
$+5C3    01251B33     8B45 CC                  MOV EAX,DWORD PTR SS:[EBP-0x34]
$+5C6    01251B36     3B46 2C                  CMP EAX,DWORD PTR DS:[ESI+0x2C]
$+5C9    01251B39     75 04                    JNZ SHORT PathOfEx.01251B3F
$+5CB    01251B3B     33F6                     XOR ESI,ESI
$+5CD    01251B3D     EB 05                    JMP SHORT PathOfEx.01251B44
$+5CF    01251B3F     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
$+5D2    01251B42     8B30                     MOV ESI,DWORD PTR DS:[EAX]
$+5D4    01251B44     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
$+5D7    01251B47     85F6                     TEST ESI,ESI
$+5D9    01251B49     74 13                    JE SHORT PathOfEx.01251B5E
$+5DB    01251B4B     8BCE                     MOV ECX,ESI
$+5DD    01251B4D     E8 9E51F2FF              CALL PathOfEx.01176CF0
$+5E2    01251B52     8BC8                     MOV ECX,EAX
$+5E4    01251B54     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
$+5E7    01251B57     EB 0A                    JMP SHORT PathOfEx.01251B63
$+5E9    01251B59     33F6                     XOR ESI,ESI
$+5EB    01251B5B     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
$+5EE    01251B5E     33C9                     XOR ECX,ECX
$+5F0    01251B60     894D E8                  MOV DWORD PTR SS:[EBP-0x18],ECX
$+5F3    01251B63     8B87 50090000            MOV EAX,DWORD PTR DS:[EDI+0x950]
$+5F9    01251B69     8078 09 00               CMP BYTE PTR DS:[EAX+0x9],0x0
$+5FD    01251B6D     0F84 25010000            JE PathOfEx.01251C98
$+603    01251B73     85C9                     TEST ECX,ECX
$+605    01251B75     0F84 1D010000            JE PathOfEx.01251C98
$+60B    01251B7B     8BCE                     MOV ECX,ESI
$+60D    01251B7D     E8 0E1E0000              CALL PathOfEx.01253990
$+612    01251B82     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
$+615    01251B85     8B97 50090000            MOV EDX,DWORD PTR DS:[EDI+0x950]
$+61B    01251B8B     8B49 14                  MOV ECX,DWORD PTR DS:[ECX+0x14]
$+61E    01251B8E     8B49 24                  MOV ECX,DWORD PTR DS:[ECX+0x24]
$+621    01251B91     390A                     CMP DWORD PTR DS:[EDX],ECX
$+623    01251B93     74 16                    JE SHORT PathOfEx.01251BAB
$+625    01251B95     85C0                     TEST EAX,EAX
$+627    01251B97     0F84 FB000000            JE PathOfEx.01251C98
$+62D    01251B9D     833A 03                  CMP DWORD PTR DS:[EDX],0x3
$+630    01251BA0     74 09                    JE SHORT PathOfEx.01251BAB
$+632    01251BA2     833A 02                  CMP DWORD PTR DS:[EDX],0x2
$+635    01251BA5     0F85 ED000000            JNZ PathOfEx.01251C98
$+63B    01251BAB     8D4D BC                  LEA ECX,DWORD PTR SS:[EBP-0x44]
$+63E    01251BAE     E8 6D84EEFF              CALL PathOfEx.0113A020
$+643    01251BB3     51                       PUSH ECX
$+644    01251BB4     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
$+647    01251BB7     51                       PUSH ECX
$+648    01251BB8     8B08                     MOV ECX,DWORD PTR DS:[EAX]
$+64A    01251BBA     E8 F1FAEEFF              CALL PathOfEx.011416B0
$+64F    01251BBF     C745 FC 07000000         MOV DWORD PTR SS:[EBP-0x4],0x7
$+656    01251BC6     8B75 C0                  MOV ESI,DWORD PTR SS:[EBP-0x40]
$+659    01251BC9     85F6                     TEST ESI,ESI
$+65B    01251BCB     74 1F                    JE SHORT PathOfEx.01251BEC
$+65D    01251BCD     8BC3                     MOV EAX,EBX
$+65F    01251BCF     F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+664    01251BD4     75 16                    JNZ SHORT PathOfEx.01251BEC
$+666    01251BD6     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+668    01251BD8     8BCE                     MOV ECX,ESI
$+66A    01251BDA     FF10                     CALL DWORD PTR DS:[EAX]
$+66C    01251BDC     8BC3                     MOV EAX,EBX
$+66E    01251BDE     F0:0FC146 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
$+673    01251BE3     75 07                    JNZ SHORT PathOfEx.01251BEC
$+675    01251BE5     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+677    01251BE7     8BCE                     MOV ECX,ESI
$+679    01251BE9     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+67C    01251BEC     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
$+680    01251BF0     8B87 4C090000            MOV EAX,DWORD PTR DS:[EDI+0x94C]
$+686    01251BF6     8B80 94380000            MOV EAX,DWORD PTR DS:[EAX+0x3894]
$+68C    01251BFC     8B88 44010000            MOV ECX,DWORD PTR DS:[EAX+0x144]
$+692    01251C02     E8 29E4EFFF              CALL PathOfEx.01150030
$+697    01251C07     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
$+69A    01251C0A     8BD0                     MOV EDX,EAX
$+69C    01251C0C     E8 7FC3FFFF              CALL PathOfEx.0124DF90
$+6A1    01251C11     83E8 00                  SUB EAX,0x0
$+6A4    01251C14     74 30                    JE SHORT PathOfEx.01251C46
$+6A6    01251C16     83E8 01                  SUB EAX,0x1
$+6A9    01251C19     74 18                    JE SHORT PathOfEx.01251C33
$+6AB    01251C1B     83E8 01                  SUB EAX,0x1
$+6AE    01251C1E     75 4C                    JNZ SHORT PathOfEx.01251C6C
$+6B0    01251C20     68 FCE5B801              PUSH PathOfEx.01B8E5FC                   ; UNICODE "LowINT"
$+6B5    01251C25     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
$+6B8    01251C28     E8 638AEDFF              CALL PathOfEx.0112A690
$+6BD    01251C2D     C645 FC 0A               MOV BYTE PTR SS:[EBP-0x4],0xA
$+6C1    01251C31     EB 24                    JMP SHORT PathOfEx.01251C57
$+6C3    01251C33     68 DCE5B801              PUSH PathOfEx.01B8E5DC                   ; UNICODE "LowDEX"
$+6C8    01251C38     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
$+6CB    01251C3B     E8 508AEDFF              CALL PathOfEx.0112A690
$+6D0    01251C40     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
$+6D4    01251C44     EB 11                    JMP SHORT PathOfEx.01251C57
$+6D6    01251C46     68 ECE5B801              PUSH PathOfEx.01B8E5EC                   ; UNICODE "LowSTR"
$+6DB    01251C4B     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
$+6DE    01251C4E     E8 3D8AEDFF              CALL PathOfEx.0112A690
$+6E3    01251C53     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
$+6E7    01251C57     51                       PUSH ECX
$+6E8    01251C58     8B4D C4                  MOV ECX,DWORD PTR SS:[EBP-0x3C]
$+6EB    01251C5B     8D45 8C                  LEA EAX,DWORD PTR SS:[EBP-0x74]
$+6EE    01251C5E     50                       PUSH EAX
$+6EF    01251C5F     E8 DCD81200              CALL PathOfEx.0137F540
$+6F4    01251C64     8D4D 8C                  LEA ECX,DWORD PTR SS:[EBP-0x74]
$+6F7    01251C67     E8 7483EDFF              CALL PathOfEx.01129FE0
$+6FC    01251C6C     C745 FC 0B000000         MOV DWORD PTR SS:[EBP-0x4],0xB
$+703    01251C73     8B75 C8                  MOV ESI,DWORD PTR SS:[EBP-0x38]
$+706    01251C76     85F6                     TEST ESI,ESI
$+708    01251C78     74 1E                    JE SHORT PathOfEx.01251C98
$+70A    01251C7A     8BC3                     MOV EAX,EBX
$+70C    01251C7C     F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
$+711    01251C81     75 15                    JNZ SHORT PathOfEx.01251C98
$+713    01251C83     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+715    01251C85     8BCE                     MOV ECX,ESI
$+717    01251C87     FF10                     CALL DWORD PTR DS:[EAX]
$+719    01251C89     F0:0FC15E 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
$+71E    01251C8E     4B                       DEC EBX
$+71F    01251C8F     75 07                    JNZ SHORT PathOfEx.01251C98
$+721    01251C91     8B06                     MOV EAX,DWORD PTR DS:[ESI]
$+723    01251C93     8BCE                     MOV ECX,ESI
$+725    01251C95     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
$+728    01251C98     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
$+72B    01251C9B     5F                       POP EDI
$+72C    01251C9C     5E                       POP ESI
$+72D    01251C9D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
$+734    01251CA4     5B                       POP EBX
$+735    01251CA5     8BE5                     MOV ESP,EBP
$+737    01251CA7     5D                       POP EBP
$+738    01251CA8     C3                       RETN
$+739    01251CA9     CC                       INT3
*/