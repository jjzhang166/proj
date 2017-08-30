#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Item(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	pt_dword dw_tmp;
	/*
	//背包相关，选择角色进入游戏期间会断住
	$-5      008D90CF     CC                     INT3
	$-4      008D90D0     8B4424 04              MOV EAX, DWORD PTR SS:[ESP+0x4]          ; 选择角色进入游戏期间会断住
	$ ==>    008D90D4     83EC 14                SUB ESP, 0x14
	$+3      008D90D7     83E8 00                SUB EAX, 0x0
	$+6      008D90DA     74 22                  JE SHORT PathOfEx.008D90FE
	$+8      008D90DC     83E8 01                SUB EAX, 0x1
	$+B      008D90DF     74 11                  JE SHORT PathOfEx.008D90F2
	$+D      008D90E1     83E8 01                SUB EAX, 0x1
	$+10     008D90E4     75 24                  JNZ SHORT PathOfEx.008D910A
	$+12     008D90E6     8D81 043E0000          LEA EAX, DWORD PTR DS:[ECX+0x3E04]
	$+18     008D90EC     83C4 14                ADD ESP, 0x14
	$+1B     008D90EF     C2 0400                RETN 0x4
	$+1E     008D90F2     8D81 783D0000          LEA EAX, DWORD PTR DS:[ECX+0x3D78]
	$+24     008D90F8     83C4 14                ADD ESP, 0x14
	$+27     008D90FB     C2 0400                RETN 0x4
	$+2A     008D90FE     8D81 EC3C0000          LEA EAX, DWORD PTR DS:[ECX+0x3CEC]       ; 获取背包对象的偏移
	$+30     008D9104     83C4 14                ADD ESP, 0x14
	$+33     008D9107     C2 0400                RETN 0x4
	$+36     008D910A     8D4424 0C              LEA EAX, DWORD PTR SS:[ESP+0xC]
	$+3A     008D910E     C74424 08 401FDE00     MOV DWORD PTR SS:[ESP+0x8], PathOfEx.00D>
	$+42     008D9116     50                     PUSH EAX                                 ; PathOfEx.00F2EA00
	$+43     008D9117     8D4424 04              LEA EAX, DWORD PTR SS:[ESP+0x4]
	$+47     008D911B     C74424 04 E874EF00     MOV DWORD PTR SS:[ESP+0x4], PathOfEx.00E>; ASCII "Invalid inventory set specified"
	$+4F     008D9123     0F57C0                 XORPS XMM0, XMM0
	$+52     008D9126     C64424 08 01           MOV BYTE PTR SS:[ESP+0x8], 0x1
	$+57     008D912B     50                     PUSH EAX                                 ; PathOfEx.00F2EA00
	$+58     008D912C     660FD64424 14          MOVQ QWORD PTR SS:[ESP+0x14], XMM0
	$+5E     008D9132     E8 AD943B00            CALL PathOfEx.00C925E4
	$+63     008D9137     83C4 08                ADD ESP, 0x8
	$+66     008D913A     C74424 08 981FDE00     MOV DWORD PTR SS:[ESP+0x8], PathOfEx.00D>
	$+6E     008D9142     8D4424 08              LEA EAX, DWORD PTR SS:[ESP+0x8]
	$+72     008D9146     68 48C7FC00            PUSH PathOfEx.00FCC748
	$+77     008D914B     50                     PUSH EAX                                 ; PathOfEx.00F2EA00
	$+78     008D914C     E8 62A53B00            CALL PathOfEx.00C936B3
	$+7D     008D9151     CC                     INT3


	//这个函数非常大，选择角色进入游戏期间会断住，这个函数内部会调用上面的函数。
	$-299    008CFA79     8B03                          MOV EAX, DWORD PTR DS:[EBX]              ; PathOfEx.00EF8DA8
	$-297    008CFA7B     E9 7D060000                   JMP PathOfEx.008D00FD
	$-292    008CFA80     51                            PUSH ECX
	$-291    008CFA81     8D45 6C                       LEA EAX, DWORD PTR SS:[EBP+0x6C]
	$-28E    008CFA84     8BCE                          MOV ECX, ESI
	$-28C    008CFA86     50                            PUSH EAX
	$-28B    008CFA87     E8 94A60000                   CALL PathOfEx.008DA120
	$-286    008CFA8C     56                            PUSH ESI
	$-285    008CFA8D     8D8F 10190000                 LEA ECX, DWORD PTR DS:[EDI+0x1910]
	$-27F    008CFA93     E8 98C3C2FF                   CALL PathOfEx.004FBE30
	$-27A    008CFA98     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-277    008CFA9B     E9 49060000                   JMP PathOfEx.008D00E9
	$-272    008CFAA0     8B43 08                       MOV EAX, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$-26F    008CFAA3     3B87 643C0000                 CMP EAX, DWORD PTR DS:[EDI+0x3C64]
	$-269    008CFAA9     0F85 37060000                 JNZ PathOfEx.008D00E6
	$-263    008CFAAF     8D8F 901C0000                 LEA ECX, DWORD PTR DS:[EDI+0x1C90]
	$-25D    008CFAB5     C787 643C0000 00000000        MOV DWORD PTR DS:[EDI+0x3C64], 0x0
	$-253    008CFABF     E8 EC3ABEFF                   CALL PathOfEx.004B35B0
	$-24E    008CFAC4     C745 FC FFFFFFFF              MOV DWORD PTR SS:[EBP-0x4], -0x1
	$-247    008CFACB     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-244    008CFACE     85C9                          TEST ECX, ECX
	$-242    008CFAD0     0F84 2B060000                 JE PathOfEx.008D0101
	$-23C    008CFAD6     C745 FC 60000000              MOV DWORD PTR SS:[EBP-0x4], 0x60
	$-235    008CFADD     E9 19060000                   JMP PathOfEx.008D00FB
	$-230    008CFAE2     8B8F 903A0000                 MOV ECX, DWORD PTR DS:[EDI+0x3A90]
	$-22A    008CFAE8     8D43 08                       LEA EAX, DWORD PTR DS:[EBX+0x8]
	$-227    008CFAEB     50                            PUSH EAX
	$-226    008CFAEC     E8 8FCEFEFF                   CALL PathOfEx.008BC980
	$-221    008CFAF1     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-21E    008CFAF4     E9 F0050000                   JMP PathOfEx.008D00E9
	$-219    008CFAF9     C787 08400000 02000000        MOV DWORD PTR DS:[EDI+0x4008], 0x2
	$-20F    008CFB03     8D8F 100A0000                 LEA ECX, DWORD PTR DS:[EDI+0xA10]
	$-209    008CFB09     FF73 08                       PUSH DWORD PTR DS:[EBX+0x8]              ; PathOfEx.006E0000
	$-206    008CFB0C     E8 1FC3C2FF                   CALL PathOfEx.004FBE30
	$-201    008CFB11     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-1FE    008CFB14     E9 D0050000                   JMP PathOfEx.008D00E9
	$-1F9    008CFB19     C787 08400000 02000000        MOV DWORD PTR DS:[EDI+0x4008], 0x2
	$-1EF    008CFB23     8B43 14                       MOV EAX, DWORD PTR DS:[EBX+0x14]
	$-1EC    008CFB26     85C0                          TEST EAX, EAX
	$-1EA    008CFB28     74 2C                         JE SHORT PathOfEx.008CFB56
	$-1E8    008CFB2A     8B8F 903A0000                 MOV ECX, DWORD PTR DS:[EDI+0x3A90]
	$-1E2    008CFB30     8B73 08                       MOV ESI, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$-1DF    008CFB33     50                            PUSH EAX
	$-1DE    008CFB34     8B01                          MOV EAX, DWORD PTR DS:[ECX]              ; PathOfEx.00EF770C
	$-1DC    008CFB36     FF50 14                       CALL NEAR DWORD PTR DS:[EAX+0x14]
	$-1D9    008CFB39     8BC8                          MOV ECX, EAX
	$-1D7    008CFB3B     E8 30ECF0FF                   CALL PathOfEx.007DE770
	$-1D2    008CFB40     50                            PUSH EAX
	$-1D1    008CFB41     FF36                          PUSH DWORD PTR DS:[ESI]
	$-1CF    008CFB43     8D8F 100C0000                 LEA ECX, DWORD PTR DS:[EDI+0xC10]
	$-1C9    008CFB49     E8 523DC8FF                   CALL PathOfEx.005538A0
	$-1C4    008CFB4E     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-1C1    008CFB51     E9 93050000                   JMP PathOfEx.008D00E9
	$-1BC    008CFB56     8D43 08                       LEA EAX, DWORD PTR DS:[EBX+0x8]
	$-1B9    008CFB59     50                            PUSH EAX
	$-1B8    008CFB5A     8D8F 900A0000                 LEA ECX, DWORD PTR DS:[EDI+0xA90]
	$-1B2    008CFB60     E8 CBC2C2FF                   CALL PathOfEx.004FBE30
	$-1AD    008CFB65     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-1AA    008CFB68     E9 7C050000                   JMP PathOfEx.008D00E9
	$-1A5    008CFB6D     8B43 08                       MOV EAX, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$-1A2    008CFB70     83EC 10                       SUB ESP, 0x10
	$-19F    008CFB73     8945 6C                       MOV DWORD PTR SS:[EBP+0x6C], EAX
	$-19C    008CFB76     E8 5569E4FF                   CALL PathOfEx.007164D0
	$-197    008CFB7B     8BC8                          MOV ECX, EAX
	$-195    008CFB7D     E8 7EC6CCFF                   CALL PathOfEx.0059C200
	$-190    008CFB82     8945 68                       MOV DWORD PTR SS:[EBP+0x68], EAX
	$-18D    008CFB85     8D45 6C                       LEA EAX, DWORD PTR SS:[EBP+0x6C]
	$-18A    008CFB88     C645 FC 61                    MOV BYTE PTR SS:[EBP-0x4], 0x61
	$-186    008CFB8C     50                            PUSH EAX
	$-185    008CFB8D     8D55 68                       LEA EDX, DWORD PTR SS:[EBP+0x68]
	$-182    008CFB90     8D4D 5C                       LEA ECX, DWORD PTR SS:[EBP+0x5C]
	$-17F    008CFB93     E8 48B9E6FF                   CALL PathOfEx.0073B4E0
	$-17A    008CFB98     83C4 04                       ADD ESP, 0x4
	$-177    008CFB9B     8D4D 68                       LEA ECX, DWORD PTR SS:[EBP+0x68]
	$-174    008CFB9E     C645 FC 63                    MOV BYTE PTR SS:[EBP-0x4], 0x63
	$-170    008CFBA2     E8 29CFB9FF                   CALL PathOfEx.0046CAD0
	$-16B    008CFBA7     8D43 10                       LEA EAX, DWORD PTR DS:[EBX+0x10]
	$-168    008CFBAA     50                            PUSH EAX
	$-167    008CFBAB     8D45 5C                       LEA EAX, DWORD PTR SS:[EBP+0x5C]
	$-164    008CFBAE     50                            PUSH EAX
	$-163    008CFBAF     8D8F 90130000                 LEA ECX, DWORD PTR DS:[EDI+0x1390]
	$-15D    008CFBB5     E8 E63CC8FF                   CALL PathOfEx.005538A0
	$-158    008CFBBA     8D4D 5C                       LEA ECX, DWORD PTR SS:[EBP+0x5C]
	$-155    008CFBBD     E8 9E00BAFF                   CALL PathOfEx.0046FC60
	$-150    008CFBC2     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-14D    008CFBC5     E9 1F050000                   JMP PathOfEx.008D00E9
	$-148    008CFBCA     80BF 673B0000 00              CMP BYTE PTR DS:[EDI+0x3B67], 0x0
	$-141    008CFBD1     74 10                         JE SHORT PathOfEx.008CFBE3
	$-13F    008CFBD3     C745 FC 64000000              MOV DWORD PTR SS:[EBP-0x4], 0x64
	$-138    008CFBDA     8BCB                          MOV ECX, EBX
	$-136    008CFBDC     8B03                          MOV EAX, DWORD PTR DS:[EBX]              ; PathOfEx.00EF8DA8
	$-134    008CFBDE     E9 1A050000                   JMP PathOfEx.008D00FD
	$-12F    008CFBE3     83C3 08                       ADD EBX, 0x8
	$-12C    008CFBE6     8D8F CC3E0000                 LEA ECX, DWORD PTR DS:[EDI+0x3ECC]
	$-126    008CFBEC     53                            PUSH EBX
	$-125    008CFBED     E8 8ECED8FF                   CALL PathOfEx.0065CA80
	$-120    008CFBF2     83BF D43E0000 00              CMP DWORD PTR DS:[EDI+0x3ED4], 0x0
	$-119    008CFBF9     75 0A                         JNZ SHORT PathOfEx.008CFC05
	$-117    008CFBFB     C787 08400000 02000000        MOV DWORD PTR DS:[EDI+0x4008], 0x2
	$-10D    008CFC05     53                            PUSH EBX
	$-10C    008CFC06     8D8F 10210000                 LEA ECX, DWORD PTR DS:[EDI+0x2110]
	$-106    008CFC0C     E8 1FC2C2FF                   CALL PathOfEx.004FBE30
	$-101    008CFC11     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-FE     008CFC14     E9 D0040000                   JMP PathOfEx.008D00E9
	$-F9     008CFC19     0FB643 0C                     MOVZX EAX, BYTE PTR DS:[EBX+0xC]
	$-F5     008CFC1D     8D8F 90210000                 LEA ECX, DWORD PTR DS:[EDI+0x2190]
	$-EF     008CFC23     50                            PUSH EAX
	$-EE     008CFC24     FF73 08                       PUSH DWORD PTR DS:[EBX+0x8]              ; PathOfEx.006E0000
	$-EB     008CFC27     E8 E437CBFF                   CALL PathOfEx.00583410
	$-E6     008CFC2C     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-E3     008CFC2F     E9 B5040000                   JMP PathOfEx.008D00E9
	$-DE     008CFC34     8D43 08                       LEA EAX, DWORD PTR DS:[EBX+0x8]
	$-DB     008CFC37     50                            PUSH EAX
	$-DA     008CFC38     8D8F 9C3B0000                 LEA ECX, DWORD PTR DS:[EDI+0x3B9C]
	$-D4     008CFC3E     E8 9DA40000                   CALL PathOfEx.008DA0E0
	$-CF     008CFC43     8D43 14                       LEA EAX, DWORD PTR DS:[EBX+0x14]
	$-CC     008CFC46     50                            PUSH EAX
	$-CB     008CFC47     8D8F 8C3E0000                 LEA ECX, DWORD PTR DS:[EDI+0x3E8C]
	$-C5     008CFC4D     E8 9E71BBFF                   CALL PathOfEx.00486DF0
	$-C0     008CFC52     8A43 30                       MOV AL, BYTE PTR DS:[EBX+0x30]
	$-BD     008CFC55     8D4F FC                       LEA ECX, DWORD PTR DS:[EDI-0x4]
	$-BA     008CFC58     24 01                         AND AL, 0x1
	$-B8     008CFC5A     8887 A43E0000                 MOV BYTE PTR DS:[EDI+0x3EA4], AL
	$-B2     008CFC60     8B43 30                       MOV EAX, DWORD PTR DS:[EBX+0x30]
	$-AF     008CFC63     D1E8                          SHR EAX, 1
	$-AD     008CFC65     24 01                         AND AL, 0x1
	$-AB     008CFC67     8887 A53E0000                 MOV BYTE PTR DS:[EDI+0x3EA5], AL
	$-A5     008CFC6D     8B43 30                       MOV EAX, DWORD PTR DS:[EBX+0x30]
	$-A2     008CFC70     C1E8 02                       SHR EAX, 0x2
	$-9F     008CFC73     24 01                         AND AL, 0x1
	$-9D     008CFC75     8887 A83E0000                 MOV BYTE PTR DS:[EDI+0x3EA8], AL
	$-97     008CFC7B     8B43 30                       MOV EAX, DWORD PTR DS:[EBX+0x30]
	$-94     008CFC7E     C1E8 03                       SHR EAX, 0x3
	$-91     008CFC81     24 01                         AND AL, 0x1
	$-8F     008CFC83     8887 A93E0000                 MOV BYTE PTR DS:[EDI+0x3EA9], AL
	$-89     008CFC89     8B43 30                       MOV EAX, DWORD PTR DS:[EBX+0x30]
	$-86     008CFC8C     C1E8 04                       SHR EAX, 0x4
	$-83     008CFC8F     24 01                         AND AL, 0x1
	$-81     008CFC91     8887 AA3E0000                 MOV BYTE PTR DS:[EDI+0x3EAA], AL
	$-7B     008CFC97     8B43 30                       MOV EAX, DWORD PTR DS:[EBX+0x30]
	$-78     008CFC9A     C1E8 05                       SHR EAX, 0x5
	$-75     008CFC9D     24 01                         AND AL, 0x1
	$-73     008CFC9F     8887 AB3E0000                 MOV BYTE PTR DS:[EDI+0x3EAB], AL
	$-6D     008CFCA5     FF73 2C                       PUSH DWORD PTR DS:[EBX+0x2C]
	$-6A     008CFCA8     E8 031C0000                   CALL PathOfEx.008D18B0
	$-65     008CFCAD     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-62     008CFCB0     E9 34040000                   JMP PathOfEx.008D00E9
	$-5D     008CFCB5     8B87 903A0000                 MOV EAX, DWORD PTR DS:[EDI+0x3A90]
	$-57     008CFCBB     8B4B 08                       MOV ECX, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$-54     008CFCBE     8988 D4010000                 MOV DWORD PTR DS:[EAX+0x1D4], ECX
	$-4E     008CFCC4     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-4B     008CFCC7     E9 1D040000                   JMP PathOfEx.008D00E9
	$-46     008CFCCC     807B 09 00                    CMP BYTE PTR DS:[EBX+0x9], 0x0
	$-42     008CFCD0     8A43 08                       MOV AL, BYTE PTR DS:[EBX+0x8]
	$-3F     008CFCD3     A2 006CFF00                   MOV BYTE PTR DS:[0xFF6C00], AL
	$-3A     008CFCD8     0F84 08040000                 JE PathOfEx.008D00E6
	$-34     008CFCDE     50                            PUSH EAX
	$-33     008CFCDF     8D8F 90310000                 LEA ECX, DWORD PTR DS:[EDI+0x3190]
	$-2D     008CFCE5     E8 0637BEFF                   CALL PathOfEx.004B33F0
	$-28     008CFCEA     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-25     008CFCED     E9 F7030000                   JMP PathOfEx.008D00E9
	$-20     008CFCF2     8D43 08                       LEA EAX, DWORD PTR DS:[EBX+0x8]
	$-1D     008CFCF5     50                            PUSH EAX
	$-1C     008CFCF6     8D8F 10220000                 LEA ECX, DWORD PTR DS:[EDI+0x2210]
	$-16     008CFCFC     E8 2FC1C2FF                   CALL PathOfEx.004FBE30
	$-11     008CFD01     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$-E      008CFD04     E9 E0030000                   JMP PathOfEx.008D00E9
	$-9      008CFD09     0FB643 08                     MOVZX EAX, BYTE PTR DS:[EBX+0x8]
	$-5      008CFD0D     8D4F FC                       LEA ECX, DWORD PTR DS:[EDI-0x4]
	$-2      008CFD10     53                            PUSH EBX
	$-1      008CFD11     50                            PUSH EAX
	$ ==>    008CFD12     E8 B9930000                   CALL PathOfEx.008D90D0                   ; 调用上面的这个函数
	$+5      008CFD17     8BC8                          MOV ECX, EAX
	$+7      008CFD19     E8 2270F2FF                   CALL PathOfEx.007F6D40
	$+C      008CFD1E     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+F      008CFD21     E9 C3030000                   JMP PathOfEx.008D00E9
	$+14     008CFD26     0FB643 08                     MOVZX EAX, BYTE PTR DS:[EBX+0x8]
	$+18     008CFD2A     8D4F FC                       LEA ECX, DWORD PTR DS:[EDI-0x4]
	$+1B     008CFD2D     53                            PUSH EBX
	$+1C     008CFD2E     50                            PUSH EAX
	$+1D     008CFD2F     E8 9C930000                   CALL PathOfEx.008D90D0                   ; 调用上面的这个函数
	$+22     008CFD34     8BC8                          MOV ECX, EAX
	$+24     008CFD36     E8 656FF2FF                   CALL PathOfEx.007F6CA0					 ; tag_访问背包信息的偏移
	$+29     008CFD3B     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+2C     008CFD3E     E9 A6030000                   JMP PathOfEx.008D00E9
	$+31     008CFD43     0FB643 08                     MOVZX EAX, BYTE PTR DS:[EBX+0x8]
	$+35     008CFD47     8D4F FC                       LEA ECX, DWORD PTR DS:[EDI-0x4]
	$+38     008CFD4A     53                            PUSH EBX
	$+39     008CFD4B     50                            PUSH EAX
	$+3A     008CFD4C     E8 7F930000                   CALL PathOfEx.008D90D0
	$+3F     008CFD51     8BC8                          MOV ECX, EAX
	$+41     008CFD53     E8 D870F2FF                   CALL PathOfEx.007F6E30
	$+46     008CFD58     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+49     008CFD5B     E9 89030000                   JMP PathOfEx.008D00E9
	$+4E     008CFD60     8A43 1C                       MOV AL, BYTE PTR DS:[EBX+0x1C]
	$+51     008CFD63     8D8F E83C0000                 LEA ECX, DWORD PTR DS:[EDI+0x3CE8]
	$+57     008CFD69     8887 A73E0000                 MOV BYTE PTR DS:[EDI+0x3EA7], AL
	$+5D     008CFD6F     8D45 5C                       LEA EAX, DWORD PTR SS:[EBP+0x5C]
	$+60     008CFD72     FFB7 4C3D0000                 PUSH DWORD PTR DS:[EDI+0x3D4C]
	$+66     008CFD78     50                            PUSH EAX
	$+67     008CFD79     E8 9271F2FF                   CALL PathOfEx.007F6F10
	$+6C     008CFD7E     C645 FC 65                    MOV BYTE PTR SS:[EBP-0x4], 0x65
	$+70     008CFD82     80BF 8D3B0000 00              CMP BYTE PTR DS:[EDI+0x3B8D], 0x0
	$+77     008CFD89     8B75 5C                       MOV ESI, DWORD PTR SS:[EBP+0x5C]         ; PathOfEx.008F2975
	$+7A     008CFD8C     75 1C                         JNZ SHORT PathOfEx.008CFDAA
	$+7C     008CFD8E     8D87 C03A0000                 LEA EAX, DWORD PTR DS:[EDI+0x3AC0]
	$+82     008CFD94     8945 6C                       MOV DWORD PTR SS:[EBP+0x6C], EAX
	$+85     008CFD97     8D4E 34                       LEA ECX, DWORD PTR DS:[ESI+0x34]
	$+88     008CFD9A     8D45 6C                       LEA EAX, DWORD PTR SS:[EBP+0x6C]
	$+8B     008CFD9D     50                            PUSH EAX
	$+8C     008CFD9E     E8 FD14BDFF                   CALL PathOfEx.004A12A0
	$+91     008CFDA3     C687 8D3B0000 01              MOV BYTE PTR DS:[EDI+0x3B8D], 0x1
	$+98     008CFDAA     8B43 0C                       MOV EAX, DWORD PTR DS:[EBX+0xC]
	$+9B     008CFDAD     8D8F C03A0000                 LEA ECX, DWORD PTR DS:[EDI+0x3AC0]
	$+A1     008CFDB3     8945 6C                       MOV DWORD PTR SS:[EBP+0x6C], EAX
	$+A4     008CFDB6     8B43 08                       MOV EAX, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$+A7     008CFDB9     8945 54                       MOV DWORD PTR SS:[EBP+0x54], EAX
	$+AA     008CFDBC     0FB743 18                     MOVZX EAX, WORD PTR DS:[EBX+0x18]
	$+AE     008CFDC0     56                            PUSH ESI
	$+AF     008CFDC1     50                            PUSH EAX
	$+B0     008CFDC2     0FB743 16                     MOVZX EAX, WORD PTR DS:[EBX+0x16]
	$+B4     008CFDC6     50                            PUSH EAX
	$+B5     008CFDC7     0FB743 14                     MOVZX EAX, WORD PTR DS:[EBX+0x14]
	$+B9     008CFDCB     50                            PUSH EAX
	$+BA     008CFDCC     0FB643 1B                     MOVZX EAX, BYTE PTR DS:[EBX+0x1B]
	$+BE     008CFDD0     50                            PUSH EAX
	$+BF     008CFDD1     0FB643 1A                     MOVZX EAX, BYTE PTR DS:[EBX+0x1A]
	$+C3     008CFDD5     50                            PUSH EAX
	$+C4     008CFDD6     8D45 6C                       LEA EAX, DWORD PTR SS:[EBP+0x6C]
	$+C7     008CFDD9     50                            PUSH EAX
	$+C8     008CFDDA     8D45 54                       LEA EAX, DWORD PTR SS:[EBP+0x54]
	$+CB     008CFDDD     50                            PUSH EAX
	$+CC     008CFDDE     E8 7D6EF3FF                   CALL PathOfEx.00806C60
	$+D1     008CFDE3     8D4D 5C                       LEA ECX, DWORD PTR SS:[EBP+0x5C]
	$+D4     008CFDE6     E8 C5ABB9FF                   CALL PathOfEx.0046A9B0
	$+D9     008CFDEB     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+DC     008CFDEE     E9 F6020000                   JMP PathOfEx.008D00E9
	$+E1     008CFDF3     8B53 08                       MOV EDX, DWORD PTR DS:[EBX+0x8]          ; PathOfEx.006E0000
	$+E4     008CFDF6     8D87 10150000                 LEA EAX, DWORD PTR DS:[EDI+0x1510]
	$+EA     008CFDFC     50                            PUSH EAX
	$+EB     008CFDFD     8D87 90150000                 LEA EAX, DWORD PTR DS:[EDI+0x1590]
	$+F1     008CFE03     50                            PUSH EAX
	$+F2     008CFE04     8D8F E83C0000                 LEA ECX, DWORD PTR DS:[EDI+0x3CE8]
	$+F8     008CFE0A     E8 C1BF0000                   CALL PathOfEx.008DBDD0
	$+FD     008CFE0F     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+100    008CFE12     83C4 08                       ADD ESP, 0x8
	$+103    008CFE15     E9 CF020000                   JMP PathOfEx.008D00E9
	$+108    008CFE1A     807B 10 00                    CMP BYTE PTR DS:[EBX+0x10], 0x0
	$+10C    008CFE1E     0FB743 08                     MOVZX EAX, WORD PTR DS:[EBX+0x8]
	$+110    008CFE22     8D14C0                        LEA EDX, DWORD PTR DS:[EAX+EAX*8]
	$+113    008CFE25     8B43 0C                       MOV EAX, DWORD PTR DS:[EBX+0xC]
	$+116    008CFE28     74 22                         JE SHORT PathOfEx.008CFE4C
	$+118    008CFE2A     8B8F A83B0000                 MOV ECX, DWORD PTR DS:[EDI+0x3BA8]
	$+11E    008CFE30     8944D1 08                     MOV DWORD PTR DS:[ECX+EDX*8+0x8], EAX
	$+122    008CFE34     8D8F 10230000                 LEA ECX, DWORD PTR DS:[EDI+0x2310]
	$+128    008CFE3A     0FB743 08                     MOVZX EAX, WORD PTR DS:[EBX+0x8]
	$+12C    008CFE3E     50                            PUSH EAX
	$+12D    008CFE3F     E8 AC35BEFF                   CALL PathOfEx.004B33F0
	$+132    008CFE44     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+135    008CFE47     E9 9D020000                   JMP PathOfEx.008D00E9
	$+13A    008CFE4C     8B8F 9C3B0000                 MOV ECX, DWORD PTR DS:[EDI+0x3B9C]
	$+140    008CFE52     8944D1 08                     MOV DWORD PTR DS:[ECX+EDX*8+0x8], EAX
	$+144    008CFE56     8D8F 90220000                 LEA ECX, DWORD PTR DS:[EDI+0x2290]
	$+14A    008CFE5C     0FB743 08                     MOVZX EAX, WORD PTR DS:[EBX+0x8]
	$+14E    008CFE60     50                            PUSH EAX
	$+14F    008CFE61     E8 8A35BEFF                   CALL PathOfEx.004B33F0
	$+154    008CFE66     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+157    008CFE69     E9 7B020000                   JMP PathOfEx.008D00E9
	$+15C    008CFE6E     8B43 38                       MOV EAX, DWORD PTR DS:[EBX+0x38]
	$+15F    008CFE71     C1E8 02                       SHR EAX, 0x2
	$+162    008CFE74     24 01                         AND AL, 0x1
	$+164    008CFE76     74 78                         JE SHORT PathOfEx.008CFEF0
	$+166    008CFE78     83C3 08                       ADD EBX, 0x8
	$+169    008CFE7B     81C7 C43B0000                 ADD EDI, 0x3BC4
	$+16F    008CFE81     53                            PUSH EBX
	$+170    008CFE82     8BCF                          MOV ECX, EDI
	$+172    008CFE84     895D 54                       MOV DWORD PTR SS:[EBP+0x54], EBX
	$+175    008CFE87     897D 6C                       MOV DWORD PTR SS:[EBP+0x6C], EDI
	$+178    008CFE8A     E8 3108BAFF                   CALL PathOfEx.004706C0
	$+17D    008CFE8F     8B3F                          MOV EDI, DWORD PTR DS:[EDI]              ; PathOfEx.00EF7710
	$+17F    008CFE91     8BF0                          MOV ESI, EAX
	$+181    008CFE93     3BF7                          CMP ESI, EDI
	$+183    008CFE95     74 0E                         JE SHORT PathOfEx.008CFEA5
	$+185    008CFE97     8D46 10                       LEA EAX, DWORD PTR DS:[ESI+0x10]
	$+188    008CFE9A     50                            PUSH EAX
	$+189    008CFE9B     53                            PUSH EBX
	$+18A    008CFE9C     E8 BF07BAFF                   CALL PathOfEx.00470660
	$+18F    008CFEA1     84C0                          TEST AL, AL
	$+191    008CFEA3     74 02                         JE SHORT PathOfEx.008CFEA7
	$+193    008CFEA5     8BF7                          MOV ESI, EDI
	$+195    008CFEA7     8B4D 6C                       MOV ECX, DWORD PTR SS:[EBP+0x6C]
	$+198    008CFEAA     8D45 64                       LEA EAX, DWORD PTR SS:[EBP+0x64]
	$+19B    008CFEAD     56                            PUSH ESI
	$+19C    008CFEAE     50                            PUSH EAX
	$+19D    008CFEAF     E8 5CDBD2FF                   CALL PathOfEx.005FDA10
	$+1A2    008CFEB4     8B5D 48                       MOV EBX, DWORD PTR SS:[EBP+0x48]
	$+1A5    008CFEB7     8D45 6C                       LEA EAX, DWORD PTR SS:[EBP+0x6C]
	$+1A8    008CFEBA     8B7D 54                       MOV EDI, DWORD PTR SS:[EBP+0x54]
	$+1AB    008CFEBD     57                            PUSH EDI
	$+1AC    008CFEBE     50                            PUSH EAX
	$+1AD    008CFEBF     8D8B CC3B0000                 LEA ECX, DWORD PTR DS:[EBX+0x3BCC]
	$+1B3    008CFEC5     E8 C6F2B9FF                   CALL PathOfEx.0046F190
	$+1B8    008CFECA     FF75 6C                       PUSH DWORD PTR SS:[EBP+0x6C]
	$+1BB    008CFECD     8D45 64                       LEA EAX, DWORD PTR SS:[EBP+0x64]
	$+1BE    008CFED0     50                            PUSH EAX
	$+1BF    008CFED1     8D8B CC3B0000                 LEA ECX, DWORD PTR DS:[EBX+0x3BCC]
	$+1C5    008CFED7     E8 E41DDEFF                   CALL PathOfEx.006B1CC0
	$+1CA    008CFEDC     57                            PUSH EDI
	$+1CB    008CFEDD     8D8B 101F0000                 LEA ECX, DWORD PTR DS:[EBX+0x1F10]
	$+1D1    008CFEE3     E8 48BFC2FF                   CALL PathOfEx.004FBE30
	$+1D6    008CFEE8     8B4D 7C                       MOV ECX, DWORD PTR SS:[EBP+0x7C]
	$+1D9    008CFEEB     E9 F9010000                   JMP PathOfEx.008D00E9
	$+1DE    008CFEF0     8B4D 48                       MOV ECX, DWORD PTR SS:[EBP+0x48]
	$+1E1    008CFEF3     8D7B 08                       LEA EDI, DWORD PTR DS:[EBX+0x8]
	$+1E4    008CFEF6     57                            PUSH EDI
	$+1E5    008CFEF7     8D45 5C                       LEA EAX, DWORD PTR SS:[EBP+0x5C]
	$+1E8    008CFEFA     50                            PUSH EAX
	$+1E9    008CFEFB     8D89 C43B0000                 LEA ECX, DWORD PTR DS:[ECX+0x3BC4]
	$+1EF    008CFF01     E8 DACF0000                   CALL PathOfEx.008DCEE0
	$+1F4    008CFF06     8A43 38                       MOV AL, BYTE PTR DS:[EBX+0x38]
	$+1F7    008CFF09     8B75 5C                       MOV ESI, DWORD PTR SS:[EBP+0x5C]         ; PathOfEx.008F2975
	$+1FA    008CFF0C     24 01                         AND AL, 0x1
	$+1FC    008CFF0E     8B4B 38                       MOV ECX, DWORD PTR DS:[EBX+0x38]
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("83EC1483E800742283E801741183E801", 0x2a + 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x2000 && dw_tmp < 0x5000);
	ofs << "pt_cdword	g_o_logic_get_bag_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//搜 tag_访问背包信息的偏移 会找到调用这个函数的地方。选择角色进入游戏期间会断住。
	/*
	$-36     007F6C9F     CC                            INT3
	$-35     007F6CA0     83EC 14                       SUB ESP, 0x14
	$-32     007F6CA3     53                            PUSH EBX
	$-31     007F6CA4     8B5C24 1C                     MOV EBX, DWORD PTR SS:[ESP+0x1C]
	$-2D     007F6CA8     8D4424 1C                     LEA EAX, DWORD PTR SS:[ESP+0x1C]
	$-29     007F6CAC     56                            PUSH ESI
	$-28     007F6CAD     8BB1 84000000                 MOV ESI, DWORD PTR DS:[ECX+0x84]
	$-22     007F6CB3     8BD6                          MOV EDX, ESI
	$-20     007F6CB5     57                            PUSH EDI
	$-1F     007F6CB6     8B7B 0C                       MOV EDI, DWORD PTR DS:[EBX+0xC]
	$-1C     007F6CB9     51                            PUSH ECX
	$-1B     007F6CBA     8B89 80000000                 MOV ECX, DWORD PTR DS:[ECX+0x80]         ; 背包的偏移
	$-15     007F6CC0     50                            PUSH EAX
	$-14     007F6CC1     897C24 2C                     MOV DWORD PTR SS:[ESP+0x2C], EDI
	$-10     007F6CC5     E8 36190000                   CALL PathOfEx.007F8600
	$-B      007F6CCA     83C4 08                       ADD ESP, 0x8
	$-8      007F6CCD     3BC6                          CMP EAX, ESI
	$-6      007F6CCF     74 04                         JE SHORT PathOfEx.007F6CD5
	$-4      007F6CD1     3B38                          CMP EDI, DWORD PTR DS:[EAX]
	$-2      007F6CD3     73 02                         JNB SHORT PathOfEx.007F6CD7
	$ ==>    007F6CD5     8BC6                          MOV EAX, ESI
	$+2      007F6CD7     3BC6                          CMP EAX, ESI
	$+4      007F6CD9     74 15                         JE SHORT PathOfEx.007F6CF0
	$+6      007F6CDB     5F                            POP EDI
	$+7      007F6CDC     5E                            POP ESI
	$+8      007F6CDD     8D4B 10                       LEA ECX, DWORD PTR DS:[EBX+0x10]
	$+B      007F6CE0     5B                            POP EBX
	$+C      007F6CE1     83C4 14                       ADD ESP, 0x14
	$+F      007F6CE4     894C24 04                     MOV DWORD PTR SS:[ESP+0x4], ECX
	$+13     007F6CE8     8B48 04                       MOV ECX, DWORD PTR DS:[EAX+0x4]
	$+16     007F6CEB   ^ E9 5083F5FF                   JMP PathOfEx.0074F040
	$+1B     007F6CF0     8D4424 18                     LEA EAX, DWORD PTR SS:[ESP+0x18]
	$+1F     007F6CF4     C74424 14 401FDE00            MOV DWORD PTR SS:[ESP+0x14], PathOfEx.00>
	$+27     007F6CFC     50                            PUSH EAX
	$+28     007F6CFD     8D4424 10                     LEA EAX, DWORD PTR SS:[ESP+0x10]
	$+2C     007F6D01     C74424 10 88B0EE00            MOV DWORD PTR SS:[ESP+0x10], PathOfEx.00>; ASCII "Got an inventory set change for an inventory that isn't in the set"
	$+34     007F6D09     0F57C0                        XORPS XMM0, XMM0
	$+37     007F6D0C     C64424 14 01                  MOV BYTE PTR SS:[ESP+0x14], 0x1
	$+3C     007F6D11     50                            PUSH EAX
	$+3D     007F6D12     660FD64424 20                 MOVQ QWORD PTR SS:[ESP+0x20], XMM0
	$+43     007F6D18     E8 C7B84900                   CALL PathOfEx.00C925E4
	$+48     007F6D1D     83C4 08                       ADD ESP, 0x8
	$+4B     007F6D20     C74424 14 981FDE00            MOV DWORD PTR SS:[ESP+0x14], PathOfEx.00>
	$+53     007F6D28     68 48C7FC00                   PUSH PathOfEx.00FCC748
	$+58     007F6D2D     8D4424 18                     LEA EAX, DWORD PTR SS:[ESP+0x18]
	$+5C     007F6D31     50                            PUSH EAX
	$+5D     007F6D32     E8 7CC94900                   CALL PathOfEx.00C936B3
	$+62     007F6D37     CC                            INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8BC63BC674155F5E8D4B105B83C414", -0x1b + 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x50 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_bag_item_cont_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-66     0076E7F0     83EC 28                     SUB ESP, 0x28
	$-63     0076E7F3     53                          PUSH EBX
	$-62     0076E7F4     55                          PUSH EBP
	$-61     0076E7F5     56                          PUSH ESI
	$-60     0076E7F6     57                          PUSH EDI
	$-5F     0076E7F7     8BF9                        MOV EDI, ECX
	$-5D     0076E7F9     897C24 1C                   MOV DWORD PTR SS:[ESP+0x1C], EDI
	$-59     0076E7FD     8B0F                        MOV ECX, DWORD PTR DS:[EDI]
	$-57     0076E7FF     85C9                        TEST ECX, ECX
	$-55     0076E801     74 05                       JE SHORT PathOfEx.0076E808
	$-53     0076E803     E8 9892FAFF                 CALL PathOfEx.00717AA0
	$-4E     0076E808     8B07                        MOV EAX, DWORD PTR DS:[EDI]
	$-4C     0076E80A     C74424 14 2455ED00          MOV DWORD PTR SS:[ESP+0x14], PathOfEx.00>; ASCII "Mods"
	$-44     0076E812     8B70 30                     MOV ESI, DWORD PTR DS:[EAX+0x30]
	$-41     0076E815     8D4424 14                   LEA EAX, DWORD PTR SS:[ESP+0x14]
	$-3D     0076E819     50                          PUSH EAX
	$-3C     0076E81A     8D4424 14                   LEA EAX, DWORD PTR SS:[ESP+0x14]
	$-38     0076E81E     50                          PUSH EAX
	$-37     0076E81F     8D4E 14                     LEA ECX, DWORD PTR DS:[ESI+0x14]
	$-34     0076E822     E8 D9D7CFFF                 CALL PathOfEx.0046C000
	$-2F     0076E827     8B4424 10                   MOV EAX, DWORD PTR SS:[ESP+0x10]
	$-2B     0076E82B     3B46 18                     CMP EAX, DWORD PTR DS:[ESI+0x18]
	$-28     0076E82E     0F84 6E020000               JE PathOfEx.0076EAA2
	$-22     0076E834     8B48 0C                     MOV ECX, DWORD PTR DS:[EAX+0xC]
	$-1F     0076E837     83F9 FF                     CMP ECX, -0x1
	$-1C     0076E83A     0F84 62020000               JE PathOfEx.0076EAA2
	$-16     0076E840     8B47 04                     MOV EAX, DWORD PTR DS:[EDI+0x4]
	$-13     0076E843     8B1C88                      MOV EBX, DWORD PTR DS:[EAX+ECX*4]
	$-10     0076E846     895C24 28                   MOV DWORD PTR SS:[ESP+0x28], EBX
	$-C      0076E84A     85DB                        TEST EBX, EBX
	$-A      0076E84C     0F84 50020000               JE PathOfEx.0076EAA2
	$-4      0076E852     8B6C24 3C                   MOV EBP, DWORD PTR SS:[ESP+0x3C]
	$ ==>    0076E856     39AB 1C010000               CMP DWORD PTR DS:[EBX+0x11C], EBP        ; Mods的角色等级
	$+6      0076E85C     0F87 40020000               JA PathOfEx.0076EAA2
	$+C      0076E862     80BB 21010000 00            CMP BYTE PTR DS:[EBX+0x121], 0x0
	$+13     0076E869     75 07                       JNZ SHORT PathOfEx.0076E872
	$+15     0076E86B     8BCB                        MOV ECX, EBX
	$+17     0076E86D     E8 0E82FDFF                 CALL PathOfEx.00746A80
	$+1C     0076E872     807B 54 00                  CMP BYTE PTR DS:[EBX+0x54], 0x0
	$+20     0076E876     0F84 26020000               JE PathOfEx.0076EAA2
	$+26     0076E87C     80BB 14010000 00            CMP BYTE PTR DS:[EBX+0x114], 0x0
	$+2D     0076E883     0F85 19020000               JNZ PathOfEx.0076EAA2
	$+33     0076E889     8BCF                        MOV ECX, EDI
	$+35     0076E88B     E8 A0B5DBFF                 CALL PathOfEx.00529E30
	$+3A     0076E890     85C0                        TEST EAX, EAX
	$+3C     0076E892     74 25                       JE SHORT PathOfEx.0076E8B9
	$+3E     0076E894     8D48 0C                     LEA ECX, DWORD PTR DS:[EAX+0xC]
	$+41     0076E897     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$+43     0076E899     68 5A040000                 PUSH 0x45A
	$+48     0076E89E     FF10                        CALL NEAR DWORD PTR DS:[EAX]
	$+4A     0076E8A0     85C0                        TEST EAX, EAX
	$+4C     0076E8A2     74 15                       JE SHORT PathOfEx.0076E8B9
	$+4E     0076E8A4     8B4C24 40                   MOV ECX, DWORD PTR SS:[ESP+0x40]
	$+52     0076E8A8     68 C5020000                 PUSH 0x2C5
	$+57     0076E8AD     8B01                        MOV EAX, DWORD PTR DS:[ECX]
	$+59     0076E8AF     FF10                        CALL NEAR DWORD PTR DS:[EAX]
	$+5B     0076E8B1     85C0                        TEST EAX, EAX
	$+5D     0076E8B3     0F85 E9010000               JNZ PathOfEx.0076EAA2
	$+63     0076E8B9     8BCF                        MOV ECX, EDI
	$+65     0076E8BB     E8 B079DCFF                 CALL PathOfEx.00536270
	$+6A     0076E8C0     894424 18                   MOV DWORD PTR SS:[ESP+0x18], EAX
	$+6E     0076E8C4     85C0                        TEST EAX, EAX
	$+70     0076E8C6     75 0C                       JNZ SHORT PathOfEx.0076E8D4
	$+72     0076E8C8     B0 01                       MOV AL, 0x1
	$+74     0076E8CA     5F                          POP EDI                                  ; 486979A8
	$+75     0076E8CB     5E                          POP ESI                                  ; 486979A8
	$+76     0076E8CC     5D                          POP EBP                                  ; 486979A8
	$+77     0076E8CD     5B                          POP EBX                                  ; 486979A8
	$+78     0076E8CE     83C4 28                     ADD ESP, 0x28
	$+7B     0076E8D1     C2 0C00                     RETN 0xC
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("39??????????0f??????????80????????????75??8b??e8", 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x80 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_entity_attr_mods_player_lvl_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-307    006CE92F     CC                     INT3
	$-306    006CE930     55                     PUSH EBP                                        ; 丢掉物品的时候会断到，函数内会判断是否可丢之类的
	$-305    006CE931     8BEC                   MOV EBP,ESP
	$-303    006CE933     6A FF                  PUSH -0x1
	$-301    006CE935     68 A449D400            PUSH PathOfEx.00D449A4
	$-2FC    006CE93A     64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-2F6    006CE940     50                     PUSH EAX
	$-2F5    006CE941     64:8925 00000000       MOV DWORD PTR FS:[0],ESP
	$-2EE    006CE948     83EC 74                SUB ESP,0x74
	$-2EB    006CE94B     53                     PUSH EBX
	$-2EA    006CE94C     8BD9                   MOV EBX,ECX
	$-2E8    006CE94E     8D45 C8                LEA EAX,DWORD PTR SS:[EBP-0x38]
	$-2E5    006CE951     56                     PUSH ESI
	$-2E4    006CE952     57                     PUSH EDI
	$-2E3    006CE953     6A 0C                  PUSH 0xC
	$-2E1    006CE955     8B8B 3C010000          MOV ECX,DWORD PTR DS:[EBX+0x13C]
	$-2DB    006CE95B     50                     PUSH EAX
	$-2DA    006CE95C     81C1 EC3C0000          ADD ECX,0x3CEC
	$-2D4    006CE962     895D E4                MOV DWORD PTR SS:[EBP-0x1C],EBX
	$-2D1    006CE965     E8 76DEE3FF            CALL PathOfEx.0050C7E0
	$-2CC    006CE96A     C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4],0x0
	$-2C5    006CE971     8B75 C8                MOV ESI,DWORD PTR SS:[EBP-0x38]
	$-2C2    006CE974     837E 0C 00             CMP DWORD PTR DS:[ESI+0xC],0x0
	$-2BE    006CE978     0F86 85050000          JBE PathOfEx.006CEF03
	$-2B8    006CE97E     837E 10 00             CMP DWORD PTR DS:[ESI+0x10],0x0
	$-2B4    006CE982     0F86 7B050000          JBE PathOfEx.006CEF03
	$-2AE    006CE988     8B46 20                MOV EAX,DWORD PTR DS:[ESI+0x20]
	$-2AB    006CE98B     8B00                   MOV EAX,DWORD PTR DS:[EAX]
	$-2A9    006CE98D     85C0                   TEST EAX,EAX
	$-2A7    006CE98F     0F84 6E050000          JE PathOfEx.006CEF03
	$-2A1    006CE995     8B50 14                MOV EDX,DWORD PTR DS:[EAX+0x14]
	$-29E    006CE998     85D2                   TEST EDX,EDX
	$-29C    006CE99A     0F84 63050000          JE PathOfEx.006CEF03
	$-296    006CE9A0     8B83 3C010000          MOV EAX,DWORD PTR DS:[EBX+0x13C]
	$-290    006CE9A6     8BB8 943A0000          MOV EDI,DWORD PTR DS:[EAX+0x3A94]
	$-28A    006CE9AC     8B47 04                MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-287    006CE9AF     8945 D0                MOV DWORD PTR SS:[EBP-0x30],EAX
	$-284    006CE9B2     85C0                   TEST EAX,EAX
	$-282    006CE9B4     74 17                  JE SHORT PathOfEx.006CE9CD
	$-280    006CE9B6     8D48 38                LEA ECX,DWORD PTR DS:[EAX+0x38]
	$-27D    006CE9B9     B8 01000000            MOV EAX,0x1
	$-278    006CE9BE     F0:0FC101              LOCK XADD DWORD PTR DS:[ECX],EAX
	$-274    006CE9C2     8B4D D0                MOV ECX,DWORD PTR SS:[EBP-0x30]
	$-271    006CE9C5     A1 0C67FD00            MOV EAX,DWORD PTR DS:[0xFD670C]
	$-26C    006CE9CA     8941 34                MOV DWORD PTR DS:[ECX+0x34],EAX
	$-269    006CE9CD     8B47 08                MOV EAX,DWORD PTR DS:[EDI+0x8]
	$-266    006CE9D0     8945 D8                MOV DWORD PTR SS:[EBP-0x28],EAX
	$-263    006CE9D3     8945 D4                MOV DWORD PTR SS:[EBP-0x2C],EAX
	$-260    006CE9D6     C645 FC 01             MOV BYTE PTR SS:[EBP-0x4],0x1
	$-25C    006CE9DA     8A80 DE000000          MOV AL,BYTE PTR DS:[EAX+0xDE]
	$-256    006CE9E0     84C0                   TEST AL,AL
	$-254    006CE9E2     74 1A                  JE SHORT PathOfEx.006CE9FE
	$-252    006CE9E4     8B83 D8180000          MOV EAX,DWORD PTR DS:[EBX+0x18D8]
	$-24C    006CE9EA     8B80 7C0A0000          MOV EAX,DWORD PTR DS:[EAX+0xA7C]
	$-246    006CE9F0     8A80 54070000          MOV AL,BYTE PTR DS:[EAX+0x754]
	$-240    006CE9F6     84C0                   TEST AL,AL
	$-23E    006CE9F8     0F85 ED040000          JNZ PathOfEx.006CEEEB
	$-238    006CE9FE     8D45 E0                LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-235    006CEA01     8955 E0                MOV DWORD PTR SS:[EBP-0x20],EDX
	$-232    006CEA04     50                     PUSH EAX
	$-231    006CEA05     8D45 E8                LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-22E    006CEA08     50                     PUSH EAX
	$-22D    006CEA09     8D4E 2C                LEA ECX,DWORD PTR DS:[ESI+0x2C]
	$-22A    006CEA0C     E8 BFF5EDFF            CALL PathOfEx.005ADFD0
	$-225    006CEA11     8B45 E8                MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-222    006CEA14     3B46 2C                CMP EAX,DWORD PTR DS:[ESI+0x2C]
	$-21F    006CEA17     0F84 CE040000          JE PathOfEx.006CEEEB
	$-219    006CEA1D     8B40 14                MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-216    006CEA20     8B38                   MOV EDI,DWORD PTR DS:[EAX]
	$-214    006CEA22     897D E8                MOV DWORD PTR SS:[EBP-0x18],EDI
	$-211    006CEA25     85FF                   TEST EDI,EDI
	$-20F    006CEA27     0F84 BE040000          JE PathOfEx.006CEEEB
	$-209    006CEA2D     8BCF                   MOV ECX,EDI
	$-207    006CEA2F     C645 F3 00             MOV BYTE PTR SS:[EBP-0xD],0x0
	$-203    006CEA33     E8 78ABDFFF            CALL PathOfEx.004C95B0
	$-1FE    006CEA38     8BD8                   MOV EBX,EAX
	$-1FC    006CEA3A     895D E0                MOV DWORD PTR SS:[EBP-0x20],EBX
	$-1F9    006CEA3D     85DB                   TEST EBX,EBX
	$-1F7    006CEA3F     74 28                  JE SHORT PathOfEx.006CEA69
	$-1F5    006CEA41     8BCB                   MOV ECX,EBX
	$-1F3    006CEA43     33F6                   XOR ESI,ESI
	$-1F1    006CEA45     E8 E66B0800            CALL PathOfEx.00755630
	$-1EC    006CEA4A     85C0                   TEST EAX,EAX
	$-1EA    006CEA4C     74 1B                  JE SHORT PathOfEx.006CEA69
	$-1E8    006CEA4E     8D4B 24                LEA ECX,DWORD PTR DS:[EBX+0x24]
	$-1E5    006CEA51     83FE 06                CMP ESI,0x6
	$-1E2    006CEA54     73 05                  JNB SHORT PathOfEx.006CEA5B
	$-1E0    006CEA56     8339 00                CMP DWORD PTR DS:[ECX],0x0
	$-1DD    006CEA59     75 0A                  JNZ SHORT PathOfEx.006CEA65
	$-1DB    006CEA5B     46                     INC ESI
	$-1DA    006CEA5C     83C1 04                ADD ECX,0x4
	$-1D7    006CEA5F     3BF0                   CMP ESI,EAX
	$-1D5    006CEA61   ^ 72 EE                  JB SHORT PathOfEx.006CEA51
	$-1D3    006CEA63     EB 04                  JMP SHORT PathOfEx.006CEA69
	$-1D1    006CEA65     C645 F3 01             MOV BYTE PTR SS:[EBP-0xD],0x1
	$-1CD    006CEA69     8B0F                   MOV ECX,DWORD PTR DS:[EDI]
	$-1CB    006CEA6B     85C9                   TEST ECX,ECX
	$-1C9    006CEA6D     74 05                  JE SHORT PathOfEx.006CEA74
	$-1C7    006CEA6F     E8 4CAB0400            CALL PathOfEx.007195C0
	$-1C2    006CEA74     8B3F                   MOV EDI,DWORD PTR DS:[EDI]
	$-1C0    006CEA76     8D45 EC                LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-1BD    006CEA79     50                     PUSH EAX
	$-1BC    006CEA7A     8D45 DC                LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-1B9    006CEA7D     C745 EC 5830EA00       MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EA3058   ; ASCII "Base"
	$-1B2    006CEA84     50                     PUSH EAX
	$-1B1    006CEA85     8B77 30                MOV ESI,DWORD PTR DS:[EDI+0x30]
	$-1AE    006CEA88     8D4E 14                LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-1AB    006CEA8B     E8 B0D5D9FF            CALL PathOfEx.0046C040
	$-1A6    006CEA90     8B45 DC                MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-1A3    006CEA93     3B46 18                CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-1A0    006CEA96     74 13                  JE SHORT PathOfEx.006CEAAB
	$-19E    006CEA98     8B48 0C                MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-19B    006CEA9B     83F9 FF                CMP ECX,-0x1
	$-198    006CEA9E     74 0B                  JE SHORT PathOfEx.006CEAAB
	$-196    006CEAA0     8B75 E8                MOV ESI,DWORD PTR SS:[EBP-0x18]
	$-193    006CEAA3     8B46 04                MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-190    006CEAA6     8B0C88                 MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$-18D    006CEAA9     EB 05                  JMP SHORT PathOfEx.006CEAB0
	$-18B    006CEAAB     8B75 E8                MOV ESI,DWORD PTR SS:[EBP-0x18]
	$-188    006CEAAE     33C9                   XOR ECX,ECX
	$-186    006CEAB0     8B41 50                MOV EAX,DWORD PTR DS:[ECX+0x50]
	$-183    006CEAB3     3B41 54                CMP EAX,DWORD PTR DS:[ECX+0x54]
	$-180    006CEAB6     0F95C7                 SETNE BH
	$-17D    006CEAB9     85FF                   TEST EDI,EDI
	$-17B    006CEABB     74 07                  JE SHORT PathOfEx.006CEAC4
	$-179    006CEABD     8BCF                   MOV ECX,EDI
	$-177    006CEABF     E8 FCAA0400            CALL PathOfEx.007195C0
	$-172    006CEAC4     8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-170    006CEAC6     C745 DC 5830EA00       MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EA3058   ; ASCII "Base"
	$-169    006CEACD     8B70 30                MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-166    006CEAD0     8D45 DC                LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-163    006CEAD3     50                     PUSH EAX
	$-162    006CEAD4     8D45 EC                LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-15F    006CEAD7     50                     PUSH EAX
	$-15E    006CEAD8     8D4E 14                LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-15B    006CEADB     E8 60D5D9FF            CALL PathOfEx.0046C040
	$-156    006CEAE0     8B45 EC                MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-153    006CEAE3     3B46 18                CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-150    006CEAE6     74 13                  JE SHORT PathOfEx.006CEAFB
	$-14E    006CEAE8     8B70 0C                MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-14B    006CEAEB     83FE FF                CMP ESI,-0x1
	$-148    006CEAEE     74 0B                  JE SHORT PathOfEx.006CEAFB
	$-146    006CEAF0     8B45 E8                MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-143    006CEAF3     8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-140    006CEAF6     8B34B0                 MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-13D    006CEAF9     EB 02                  JMP SHORT PathOfEx.006CEAFD
	$-13B    006CEAFB     33F6                   XOR ESI,ESI
	$-139    006CEAFD     8B46 54                MOV EAX,DWORD PTR DS:[ESI+0x54]
	$-136    006CEB00     8B56 50                MOV EDX,DWORD PTR DS:[ESI+0x50]
	$-133    006CEB03     3BD0                   CMP EDX,EAX
	$-131    006CEB05     74 11                  JE SHORT PathOfEx.006CEB18
	$-12F    006CEB07     8B4A 04                MOV ECX,DWORD PTR DS:[EDX+0x4]
	$-12C    006CEB0A     8A49 38                MOV CL,BYTE PTR DS:[ECX+0x38]
	$-129    006CEB0D     84C9                   TEST CL,CL
	$-127    006CEB0F     74 11                  JE SHORT PathOfEx.006CEB22
	$-125    006CEB11     83C2 10                ADD EDX,0x10
	$-122    006CEB14     3BD0                   CMP EDX,EAX
	$-120    006CEB16   ^ 75 EF                  JNZ SHORT PathOfEx.006CEB07
	$-11E    006CEB18     837E 14 00             CMP DWORD PTR DS:[ESI+0x14],0x0
	$-11A    006CEB1C     75 04                  JNZ SHORT PathOfEx.006CEB22
	$-118    006CEB1E     32DB                   XOR BL,BL
	$-116    006CEB20     EB 02                  JMP SHORT PathOfEx.006CEB24
	$-114    006CEB22     B3 01                  MOV BL,0x1
	$-112    006CEB24     8B4D E0                MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-10F    006CEB27     85C9                   TEST ECX,ECX
	$-10D    006CEB29     0F84 BD000000          JE PathOfEx.006CEBEC
	$-107    006CEB2F     84FF                   TEST BH,BH
	$-105    006CEB31     74 08                  JE SHORT PathOfEx.006CEB3B
	$-103    006CEB33     84DB                   TEST BL,BL
	$-101    006CEB35     0F85 B1000000          JNZ PathOfEx.006CEBEC
	$-FB     006CEB3B     8B41 40                MOV EAX,DWORD PTR DS:[ECX+0x40]
	$-F8     006CEB3E     33FF                   XOR EDI,EDI
	$-F6     006CEB40     8B49 3C                MOV ECX,DWORD PTR DS:[ECX+0x3C]
	$-F3     006CEB43     33D2                   XOR EDX,EDX
	$-F1     006CEB45     8BF0                   MOV ESI,EAX
	$-EF     006CEB47     897D EC                MOV DWORD PTR SS:[EBP-0x14],EDI
	$-EC     006CEB4A     2BF1                   SUB ESI,ECX
	$-EA     006CEB4C     3BC8                   CMP ECX,EAX
	$-E8     006CEB4E     0F47F2                 CMOVA ESI,EDX
	$-E5     006CEB51     85F6                   TEST ESI,ESI
	$-E3     006CEB53     0F84 93000000          JE PathOfEx.006CEBEC
	$-DD     006CEB59     0F1F80 00000000        NOP DWORD PTR DS:[EAX]
	$-D6     006CEB60     0FB6040A               MOVZX EAX,BYTE PTR DS:[EDX+ECX]
	$-D2     006CEB64     42                     INC EDX
	$-D1     006CEB65     0145 EC                ADD DWORD PTR SS:[EBP-0x14],EAX
	$-CE     006CEB68     3BD6                   CMP EDX,ESI
	$-CC     006CEB6A   ^ 75 F4                  JNZ SHORT PathOfEx.006CEB60
	$-CA     006CEB6C     397D EC                CMP DWORD PTR SS:[EBP-0x14],EDI
	$-C7     006CEB6F     76 7B                  JBE SHORT PathOfEx.006CEBEC
	$-C5     006CEB71     8B45 E0                MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-C2     006CEB74     8B4D EC                MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-BF     006CEB77     83C0 24                ADD EAX,0x24
	$-BC     006CEB7A     8945 E0                MOV DWORD PTR SS:[EBP-0x20],EAX
	$-B9     006CEB7D     0F1F00                 NOP DWORD PTR DS:[EAX]
	$-B6     006CEB80     83FF 06                CMP EDI,0x6
	$-B3     006CEB83     73 5C                  JNB SHORT PathOfEx.006CEBE1
	$-B1     006CEB85     8B30                   MOV ESI,DWORD PTR DS:[EAX]
	$-AF     006CEB87     85F6                   TEST ESI,ESI
	$-AD     006CEB89     74 56                  JE SHORT PathOfEx.006CEBE1
	$-AB     006CEB8B     84FF                   TEST BH,BH
	$-A9     006CEB8D     75 10                  JNZ SHORT PathOfEx.006CEB9F
	$-A7     006CEB8F     8BCE                   MOV ECX,ESI
	$-A5     006CEB91     E8 9AB6DEFF            CALL PathOfEx.004BA230
	$-A0     006CEB96     8B50 50                MOV EDX,DWORD PTR DS:[EAX+0x50]
	$-9D     006CEB99     3B50 54                CMP EDX,DWORD PTR DS:[EAX+0x54]
	$-9A     006CEB9C     0F95C7                 SETNE BH
	$-97     006CEB9F     84DB                   TEST BL,BL
	$-95     006CEBA1     75 30                  JNZ SHORT PathOfEx.006CEBD3
	$-93     006CEBA3     8BCE                   MOV ECX,ESI
	$-91     006CEBA5     E8 86B6DEFF            CALL PathOfEx.004BA230
	$-8C     006CEBAA     8BF0                   MOV ESI,EAX
	$-8A     006CEBAC     8B4E 54                MOV ECX,DWORD PTR DS:[ESI+0x54]
	$-87     006CEBAF     8B56 50                MOV EDX,DWORD PTR DS:[ESI+0x50]
	$-84     006CEBB2     3BD1                   CMP EDX,ECX
	$-82     006CEBB4     74 11                  JE SHORT PathOfEx.006CEBC7
	$-80     006CEBB6     8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-7D     006CEBB9     8A40 38                MOV AL,BYTE PTR DS:[EAX+0x38]
	$-7A     006CEBBC     84C0                   TEST AL,AL
	$-78     006CEBBE     74 11                  JE SHORT PathOfEx.006CEBD1
	$-76     006CEBC0     83C2 10                ADD EDX,0x10
	$-73     006CEBC3     3BD1                   CMP EDX,ECX
	$-71     006CEBC5   ^ 75 EF                  JNZ SHORT PathOfEx.006CEBB6
	$-6F     006CEBC7     837E 14 00             CMP DWORD PTR DS:[ESI+0x14],0x0
	$-6B     006CEBCB     75 04                  JNZ SHORT PathOfEx.006CEBD1
	$-69     006CEBCD     32DB                   XOR BL,BL
	$-67     006CEBCF     EB 02                  JMP SHORT PathOfEx.006CEBD3
	$-65     006CEBD1     B3 01                  MOV BL,0x1
	$-63     006CEBD3     84FF                   TEST BH,BH
	$-61     006CEBD5     74 04                  JE SHORT PathOfEx.006CEBDB
	$-5F     006CEBD7     84DB                   TEST BL,BL
	$-5D     006CEBD9     75 11                  JNZ SHORT PathOfEx.006CEBEC
	$-5B     006CEBDB     8B4D EC                MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-58     006CEBDE     8B45 E0                MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-55     006CEBE1     47                     INC EDI
	$-54     006CEBE2     83C0 04                ADD EAX,0x4
	$-51     006CEBE5     8945 E0                MOV DWORD PTR SS:[EBP-0x20],EAX
	$-4E     006CEBE8     3BF9                   CMP EDI,ECX
	$-4C     006CEBEA   ^ 72 94                  JB SHORT PathOfEx.006CEB80
	$-4A     006CEBEC     8B75 E8                MOV ESI,DWORD PTR SS:[EBP-0x18]
	$-47     006CEBEF     8B0E                   MOV ECX,DWORD PTR DS:[ESI]
	$-45     006CEBF1     85C9                   TEST ECX,ECX
	$-43     006CEBF3     74 05                  JE SHORT PathOfEx.006CEBFA
	$-41     006CEBF5     E8 C6A90400            CALL PathOfEx.007195C0
	$-3C     006CEBFA     8B3E                   MOV EDI,DWORD PTR DS:[ESI]                      ; esi=stCD_EntityItemBased*
	$-3A     006CEBFC     8D45 DC                LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-37     006CEBFF     50                     PUSH EAX
	$-36     006CEC00     8D45 E0                LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-33     006CEC03     C745 DC 5830EA00       MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EA3058   ; ASCII "Base"
	$-2C     006CEC0A     50                     PUSH EAX
	$-2B     006CEC0B     8B77 30                MOV ESI,DWORD PTR DS:[EDI+0x30]                 ; 执行后esi=stCD_DbTblEntity*
	$-28     006CEC0E     8D4E 14                LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-25     006CEC11     E8 2AD4D9FF            CALL PathOfEx.0046C040                          ; 查找属性索引
	$-20     006CEC16     8B45 E0                MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-1D     006CEC19     3B46 18                CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-1A     006CEC1C     74 13                  JE SHORT PathOfEx.006CEC31
	$-18     006CEC1E     8B48 0C                MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-15     006CEC21     83F9 FF                CMP ECX,-0x1                                    ; ecx=base属性的索引
	$-12     006CEC24     74 0B                  JE SHORT PathOfEx.006CEC31
	$-10     006CEC26     8B45 E8                MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-D      006CEC29     8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-A      006CEC2C     8B0488                 MOV EAX,DWORD PTR DS:[EAX+ECX*4]                ; 执行后eax=stCD_EntityAttrItemBase*
	$-7      006CEC2F     EB 02                  JMP SHORT PathOfEx.006CEC33
	$-5      006CEC31     33C0                   XOR EAX,EAX
	$-3      006CEC33     8B55 D8                MOV EDX,DWORD PTR SS:[EBP-0x28]                 ; 执行后edx=stCD_TblItemWorldArea*
	$ ==>    006CEC36     8A88 82000000          MOV CL,BYTE PTR DS:[EAX+0x82]                   ; g_o_item_base_is_bind_
	$+6      006CEC3C     8A42 0C                MOV AL,BYTE PTR DS:[EDX+0xC]                    ; stCD_TblItemWorldArea.is_town_
	$+9      006CEC3F     84C0                   TEST AL,AL
	$+B      006CEC41     0F85 C3000000          JNZ PathOfEx.006CED0A
	$+11     006CEC47     8A82 DE000000          MOV AL,BYTE PTR DS:[EDX+0xDE]
	$+17     006CEC4D     84C0                   TEST AL,AL
	$+19     006CEC4F     0F85 B5000000          JNZ PathOfEx.006CED0A
	$+1F     006CEC55     84C9                   TEST CL,CL
	$+21     006CEC57     0F85 AD000000          JNZ PathOfEx.006CED0A                           ; 绑定物品此处会断到
	$+27     006CEC5D     8B75 E4                MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+2A     006CEC60     84DB                   TEST BL,BL
	$+2C     006CEC62     74 40                  JE SHORT PathOfEx.006CECA4                      ; 此处跳转到丢掉物品
	$+2E     006CEC64     6A 00                  PUSH 0x0
	$+30     006CEC66     83EC 2C                SUB ESP,0x2C
	$+33     006CEC69     8BC4                   MOV EAX,ESP
	$+35     006CEC6B     C74424 28 00006144     MOV DWORD PTR SS:[ESP+0x28],0x44610000
	$+3D     006CEC73     83EC 28                SUB ESP,0x28
	$+40     006CEC76     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+47     006CEC7D     8BC4                   MOV EAX,ESP
	$+49     006CEC7F     83EC 10                SUB ESP,0x10
	$+4C     006CEC82     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+53     006CEC89     8B86 D8180000          MOV EAX,DWORD PTR DS:[ESI+0x18D8]
	$+59     006CEC8F     68 4C030000            PUSH 0x34C
	$+5E     006CEC94     8B88 C00A0000          MOV ECX,DWORD PTR DS:[EAX+0xAC0]
	$+64     006CEC9A     E8 414BE0FF            CALL PathOfEx.004D37E0
	$+69     006CEC9F     E9 47020000            JMP PathOfEx.006CEEEB
	$+6E     006CECA4     807D F3 00             CMP BYTE PTR SS:[EBP-0xD],0x0
	$+72     006CECA8     74 50                  JE SHORT PathOfEx.006CECFA                      ; 此处跳转到丢掉物品
	$+74     006CECAA     83EC 28                SUB ESP,0x28
	$+77     006CECAD     8BC4                   MOV EAX,ESP
	$+79     006CECAF     83EC 28                SUB ESP,0x28
	$+7C     006CECB2     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+83     006CECB9     8BC4                   MOV EAX,ESP
	$+85     006CECBB     6A 00                  PUSH 0x0
	$+87     006CECBD     83EC 08                SUB ESP,0x8
	$+8A     006CECC0     C700 9CAFEA00          MOV DWORD PTR DS:[EAX],PathOfEx.00EAAF9C
	$+90     006CECC6     8970 04                MOV DWORD PTR DS:[EAX+0x4],ESI
	$+93     006CECC9     C74424 04 00006144     MOV DWORD PTR SS:[ESP+0x4],0x44610000
	$+9B     006CECD1     68 51030000            PUSH 0x351
	$+A0     006CECD6     8940 24                MOV DWORD PTR DS:[EAX+0x24],EAX
	$+A3     006CECD9     8B86 D8180000          MOV EAX,DWORD PTR DS:[ESI+0x18D8]
	$+A9     006CECDF     68 4F030000            PUSH 0x34F
	$+AE     006CECE4     51                     PUSH ECX
	$+AF     006CECE5     68 4E030000            PUSH 0x34E
	$+B4     006CECEA     8B88 C00A0000          MOV ECX,DWORD PTR DS:[EAX+0xAC0]
	$+BA     006CECF0     E8 6BCEEDFF            CALL PathOfEx.005ABB60
	$+BF     006CECF5     E9 F1010000            JMP PathOfEx.006CEEEB
	$+C4     006CECFA     8B8E 3C010000          MOV ECX,DWORD PTR DS:[ESI+0x13C]
	$+CA     006CED00     E8 6B832000            CALL PathOfEx.008D7070                          ; 发包上层
	$+CF     006CED05     E9 E1010000            JMP PathOfEx.006CEEEB
	$+D4     006CED0A     84FF                   TEST BH,BH
	$+D6     006CED0C     74 43                  JE SHORT PathOfEx.006CED51
	$+D8     006CED0E     8B75 E4                MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+DB     006CED11     6A 00                  PUSH 0x0
	$+DD     006CED13     83EC 2C                SUB ESP,0x2C
	$+E0     006CED16     8BC4                   MOV EAX,ESP
	$+E2     006CED18     C74424 28 00006144     MOV DWORD PTR SS:[ESP+0x28],0x44610000
	$+EA     006CED20     83EC 28                SUB ESP,0x28
	$+ED     006CED23     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+F4     006CED2A     8BC4                   MOV EAX,ESP
	$+F6     006CED2C     83EC 10                SUB ESP,0x10
	$+F9     006CED2F     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+100    006CED36     8B86 D8180000          MOV EAX,DWORD PTR DS:[ESI+0x18D8]
	$+106    006CED3C     68 4A030000            PUSH 0x34A
	$+10B    006CED41     8B88 C00A0000          MOV ECX,DWORD PTR DS:[EAX+0xAC0]
	$+111    006CED47     E8 944AE0FF            CALL PathOfEx.004D37E0
	$+116    006CED4C     E9 9A010000            JMP PathOfEx.006CEEEB
	$+11B    006CED51     85FF                   TEST EDI,EDI
	$+11D    006CED53     74 07                  JE SHORT PathOfEx.006CED5C
	$+11F    006CED55     8BCF                   MOV ECX,EDI
	$+121    006CED57     E8 64A80400            CALL PathOfEx.007195C0
	$+126    006CED5C     8B5D E8                MOV EBX,DWORD PTR SS:[EBP-0x18]
	$+129    006CED5F     C745 D8 F47EED00       MOV DWORD PTR SS:[EBP-0x28],PathOfEx.00ED7EF4   ; ASCII "Quest"
	$+130    006CED66     8B03                   MOV EAX,DWORD PTR DS:[EBX]
	$+132    006CED68     8B70 30                MOV ESI,DWORD PTR DS:[EAX+0x30]
	$+135    006CED6B     8D45 D8                LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+138    006CED6E     50                     PUSH EAX
	$+139    006CED6F     8D45 DC                LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+13C    006CED72     50                     PUSH EAX
	$+13D    006CED73     8D4E 14                LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+140    006CED76     E8 C5D2D9FF            CALL PathOfEx.0046C040
	$+145    006CED7B     8B45 DC                MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+148    006CED7E     3B46 18                CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+14B    006CED81     74 4A                  JE SHORT PathOfEx.006CEDCD
	$+14D    006CED83     8B48 0C                MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+150    006CED86     83F9 FF                CMP ECX,-0x1
	$+153    006CED89     74 42                  JE SHORT PathOfEx.006CEDCD
	$+155    006CED8B     8B43 04                MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+158    006CED8E     833C88 00              CMP DWORD PTR DS:[EAX+ECX*4],0x0
	$+15C    006CED92     74 39                  JE SHORT PathOfEx.006CEDCD
	$+15E    006CED94     6A 09                  PUSH 0x9
	$+160    006CED96     33C0                   XOR EAX,EAX
	$+162    006CED98     C745 C4 07000000       MOV DWORD PTR SS:[EBP-0x3C],0x7
	$+169    006CED9F     68 3CC6E800            PUSH PathOfEx.00E8C63C                          ; UNICODE "QuestItem"
	$+16E    006CEDA4     8D4D B0                LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+171    006CEDA7     C745 C0 00000000       MOV DWORD PTR SS:[EBP-0x40],0x0
	$+178    006CEDAE     66:8945 B0             MOV WORD PTR SS:[EBP-0x50],AX
	$+17C    006CEDB2     E8 F9EED9FF            CALL PathOfEx.0046DCB0
	$+181    006CEDB7     51                     PUSH ECX
	$+182    006CEDB8     8B4D E4                MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+185    006CEDBB     8D45 B0                LEA EAX,DWORD PTR SS:[EBP-0x50]
	$+188    006CEDBE     50                     PUSH EAX
	$+189    006CEDBF     C645 FC 02             MOV BYTE PTR SS:[EBP-0x4],0x2
	$+18D    006CEDC3     E8 D8250000            CALL PathOfEx.006D13A0
	$+192    006CEDC8     E9 07010000            JMP PathOfEx.006CEED4
	$+197    006CEDCD     33C0                   XOR EAX,EAX
	$+199    006CEDCF     C745 C4 07000000       MOV DWORD PTR SS:[EBP-0x3C],0x7
	$+1A0    006CEDD6     C745 C0 00000000       MOV DWORD PTR SS:[EBP-0x40],0x0
	$+1A7    006CEDDD     66:8945 B0             MOV WORD PTR SS:[EBP-0x50],AX
	$+1AB    006CEDE1     C645 FC 03             MOV BYTE PTR SS:[EBP-0x4],0x3
	$+1AF    006CEDE5     3845 F3                CMP BYTE PTR SS:[EBP-0xD],AL
	$+1B2    006CEDE8     74 64                  JE SHORT PathOfEx.006CEE4E
	$+1B4    006CEDEA     BA 4D030000            MOV EDX,0x34D
	$+1B9    006CEDEF     8D4D 98                LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1BC    006CEDF2     E8 D9E60400            CALL PathOfEx.0071D4D0
	$+1C1    006CEDF7     50                     PUSH EAX
	$+1C2    006CEDF8     8D4D B0                LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+1C5    006CEDFB     E8 9085DBFF            CALL PathOfEx.00487390
	$+1CA    006CEE00     8D4D 98                LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1CD    006CEE03     E8 38E9D9FF            CALL PathOfEx.0046D740
	$+1D2    006CEE08     837D C0 00             CMP DWORD PTR SS:[EBP-0x40],0x0
	$+1D6    006CEE0C     74 40                  JE SHORT PathOfEx.006CEE4E
	$+1D8    006CEE0E     8D45 B0                LEA EAX,DWORD PTR SS:[EBP-0x50]
	$+1DB    006CEE11     BA 7CC7E700            MOV EDX,PathOfEx.00E7C77C                       ; UNICODE "<red>{"
	$+1E0    006CEE16     50                     PUSH EAX
	$+1E1    006CEE17     8D4D 80                LEA ECX,DWORD PTR SS:[EBP-0x80]
	$+1E4    006CEE1A     E8 A137DFFF            CALL PathOfEx.004C25C0
	$+1E9    006CEE1F     68 9C4AE700            PUSH PathOfEx.00E74A9C                          ; UNICODE "}"
	$+1EE    006CEE24     8BD0                   MOV EDX,EAX
	$+1F0    006CEE26     C645 FC 04             MOV BYTE PTR SS:[EBP-0x4],0x4
	$+1F4    006CEE2A     8D4D 98                LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1F7    006CEE2D     E8 EEE3DAFF            CALL PathOfEx.0047D220
	$+1FC    006CEE32     83C4 08                ADD ESP,0x8
	$+1FF    006CEE35     8D4D B0                LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+202    006CEE38     50                     PUSH EAX
	$+203    006CEE39     E8 5285DBFF            CALL PathOfEx.00487390
	$+208    006CEE3E     8D4D 98                LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+20B    006CEE41     E8 FAE8D9FF            CALL PathOfEx.0046D740
	$+210    006CEE46     8D4D 80                LEA ECX,DWORD PTR SS:[EBP-0x80]
	$+213    006CEE49     E8 F2E8D9FF            CALL PathOfEx.0046D740
	$+218    006CEE4E     6A 00                  PUSH 0x0
	$+21A    006CEE50     83EC 2C                SUB ESP,0x2C
	$+21D    006CEE53     8BC4                   MOV EAX,ESP
	$+21F    006CEE55     8965 D8                MOV DWORD PTR SS:[EBP-0x28],ESP
	$+222    006CEE58     C74424 28 00006144     MOV DWORD PTR SS:[ESP+0x28],0x44610000
	$+22A    006CEE60     C740 24 00000000       MOV DWORD PTR DS:[EAX+0x24],0x0
	$+231    006CEE67     8B75 E4                MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+234    006CEE6A     83EC 28                SUB ESP,0x28
	$+237    006CEE6D     8BC4                   MOV EAX,ESP
	$+239    006CEE6F     8965 DC                MOV DWORD PTR SS:[EBP-0x24],ESP
	$+23C    006CEE72     C700 B4AFEA00          MOV DWORD PTR DS:[EAX],PathOfEx.00EAAFB4
	$+242    006CEE78     8970 04                MOV DWORD PTR DS:[EAX+0x4],ESI
	$+245    006CEE7B     8940 24                MOV DWORD PTR DS:[EAX+0x24],EAX
	$+248    006CEE7E     6A 00                  PUSH 0x0
	$+24A    006CEE80     68 51030000            PUSH 0x351
	$+24F    006CEE85     68 50030000            PUSH 0x350
	$+254    006CEE8A     83EC 18                SUB ESP,0x18
	$+257    006CEE8D     C645 FC 06             MOV BYTE PTR SS:[EBP-0x4],0x6
	$+25B    006CEE91     8BCC                   MOV ECX,ESP
	$+25D    006CEE93     C741 14 07000000       MOV DWORD PTR DS:[ECX+0x14],0x7
	$+264    006CEE9A     C741 10 00000000       MOV DWORD PTR DS:[ECX+0x10],0x0
	$+26B    006CEEA1     8379 14 08             CMP DWORD PTR DS:[ECX+0x14],0x8
	$+26F    006CEEA5     72 04                  JB SHORT PathOfEx.006CEEAB
	$+271    006CEEA7     8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$+273    006CEEA9     EB 02                  JMP SHORT PathOfEx.006CEEAD
	$+275    006CEEAB     8BC1                   MOV EAX,ECX
	$+277    006CEEAD     33D2                   XOR EDX,EDX
	$+279    006CEEAF     6A FF                  PUSH -0x1
	$+27B    006CEEB1     66:8910                MOV WORD PTR DS:[EAX],DX
	$+27E    006CEEB4     8D45 B0                LEA EAX,DWORD PTR SS:[EBP-0x50]
	$+281    006CEEB7     52                     PUSH EDX
	$+282    006CEEB8     50                     PUSH EAX
	$+283    006CEEB9     E8 C2E9D9FF            CALL PathOfEx.0046D880
	$+288    006CEEBE     8B86 D8180000          MOV EAX,DWORD PTR DS:[ESI+0x18D8]
	$+28E    006CEEC4     8B88 C00A0000          MOV ECX,DWORD PTR DS:[EAX+0xAC0]
	$+294    006CEECA     51                     PUSH ECX
	$+295    006CEECB     C645 FC 03             MOV BYTE PTR SS:[EBP-0x4],0x3
	$+299    006CEECF     E8 0C6A0000            CALL PathOfEx.006D58E0
	$+29E    006CEED4     8B45 C4                MOV EAX,DWORD PTR SS:[EBP-0x3C]
	$+2A1    006CEED7     83F8 08                CMP EAX,0x8
	$+2A4    006CEEDA     72 0F                  JB SHORT PathOfEx.006CEEEB
	$+2A6    006CEEDC     6A 02                  PUSH 0x2
	$+2A8    006CEEDE     40                     INC EAX
	$+2A9    006CEEDF     50                     PUSH EAX
	$+2AA    006CEEE0     FF75 B0                PUSH DWORD PTR SS:[EBP-0x50]
	$+2AD    006CEEE3     E8 48D9D9FF            CALL PathOfEx.0046C830
	$+2B2    006CEEE8     83C4 0C                ADD ESP,0xC
	$+2B5    006CEEEB     C645 FC 07             MOV BYTE PTR SS:[EBP-0x4],0x7
	$+2B9    006CEEEF     8B45 D0                MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+2BC    006CEEF2     85C0                   TEST EAX,EAX
	$+2BE    006CEEF4     74 0D                  JE SHORT PathOfEx.006CEF03
	$+2C0    006CEEF6     50                     PUSH EAX
	$+2C1    006CEEF7     E8 04910400            CALL PathOfEx.00718000
	$+2C6    006CEEFC     8BC8                   MOV ECX,EAX
	$+2C8    006CEEFE     E8 8D8C0400            CALL PathOfEx.00717B90
	$+2CD    006CEF03     C745 FC 08000000       MOV DWORD PTR SS:[EBP-0x4],0x8
	$+2D4    006CEF0A     8B75 CC                MOV ESI,DWORD PTR SS:[EBP-0x34]
	$+2D7    006CEF0D     85F6                   TEST ESI,ESI
	$+2D9    006CEF0F     74 21                  JE SHORT PathOfEx.006CEF32
	$+2DB    006CEF11     83CF FF                OR EDI,0xFFFFFFFF
	$+2DE    006CEF14     8BC7                   MOV EAX,EDI
	$+2E0    006CEF16     F0:0FC146 04           LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+2E5    006CEF1B     75 15                  JNZ SHORT PathOfEx.006CEF32
	$+2E7    006CEF1D     8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+2E9    006CEF1F     8BCE                   MOV ECX,ESI
	$+2EB    006CEF21     FF10                   CALL DWORD PTR DS:[EAX]
	$+2ED    006CEF23     F0:0FC17E 08           LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+2F2    006CEF28     4F                     DEC EDI
	$+2F3    006CEF29     75 07                  JNZ SHORT PathOfEx.006CEF32
	$+2F5    006CEF2B     8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+2F7    006CEF2D     8BCE                   MOV ECX,ESI
	$+2F9    006CEF2F     FF50 04                CALL DWORD PTR DS:[EAX+0x4]
	$+2FC    006CEF32     8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2FF    006CEF35     5F                     POP EDI
	$+300    006CEF36     5E                     POP ESI
	$+301    006CEF37     64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$+308    006CEF3E     5B                     POP EBX
	$+309    006CEF3F     8BE5                   MOV ESP,EBP
	$+30B    006CEF41     5D                     POP EBP
	$+30C    006CEF42     C3                     RETN
	$+30D    006CEF43     CC                     INT3
	*/

	//gj3
	/*
	$-196    005BA7DF     CC                       INT3
	$-195    005BA7E0     56                       PUSH ESI
	$-194    005BA7E1     8BF1                     MOV ESI,ECX
	$-192    005BA7E3     8B86 A81A0000            MOV EAX,DWORD PTR DS:[ESI+0x1AA8]
	$-18C    005BA7E9     85C0                     TEST EAX,EAX
	$-18A    005BA7EB     74 41                    JE SHORT PathOfEx.005BA82E
	$-188    005BA7ED     8B48 10                  MOV ECX,DWORD PTR DS:[EAX+0x10]
	$-185    005BA7F0     85C9                     TEST ECX,ECX
	$-183    005BA7F2     74 3A                    JE SHORT PathOfEx.005BA82E
	$-181    005BA7F4     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-17E    005BA7F7     85C0                     TEST EAX,EAX
	$-17C    005BA7F9     74 33                    JE SHORT PathOfEx.005BA82E
	$-17A    005BA7FB     8B89 14410000            MOV ECX,DWORD PTR DS:[ECX+0x4114]
	$-174    005BA801     50                       PUSH EAX
	$-173    005BA802     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-171    005BA804     FF50 14                  CALL DWORD PTR DS:[EAX+0x14]
	$-16E    005BA807     8BC8                     MOV ECX,EAX
	$-16C    005BA809     E8 92001200              CALL PathOfEx.006DA8A0
	$-167    005BA80E     85C0                     TEST EAX,EAX
	$-165    005BA810     74 1C                    JE SHORT PathOfEx.005BA82E
	$-163    005BA812     8B4C24 0C                MOV ECX,DWORD PTR SS:[ESP+0xC]
	$-15F    005BA816     85C9                     TEST ECX,ECX
	$-15D    005BA818     74 14                    JE SHORT PathOfEx.005BA82E
	$-15B    005BA81A     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-159    005BA81C     FF50 18                  CALL DWORD PTR DS:[EAX+0x18]
	$-156    005BA81F     85C0                     TEST EAX,EAX
	$-154    005BA821     74 0B                    JE SHORT PathOfEx.005BA82E
	$-152    005BA823     8B8E A81A0000            MOV ECX,DWORD PTR DS:[ESI+0x1AA8]
	$-14C    005BA829     E8 924FDDFF              CALL PathOfEx.0038F7C0
	$-147    005BA82E     5E                       POP ESI
	$-146    005BA82F     C2 0800                  RETN 0x8
	$-143    005BA832     CC                       INT3
	$-142    005BA833     CC                       INT3
	$-141    005BA834     CC                       INT3
	$-140    005BA835     CC                       INT3
	$-13F    005BA836     CC                       INT3
	$-13E    005BA837     CC                       INT3
	$-13D    005BA838     CC                       INT3
	$-13C    005BA839     CC                       INT3
	$-13B    005BA83A     CC                       INT3
	$-13A    005BA83B     CC                       INT3
	$-139    005BA83C     CC                       INT3
	$-138    005BA83D     CC                       INT3
	$-137    005BA83E     CC                       INT3
	$-136    005BA83F     CC                       INT3
	$-135    005BA840     55                       PUSH EBP
	$-134    005BA841     8BEC                     MOV EBP,ESP
	$-132    005BA843     6A FF                    PUSH -0x1
	$-130    005BA845     68 1C3FD800              PUSH PathOfEx.00D83F1C
	$-12B    005BA84A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-125    005BA850     50                       PUSH EAX
	$-124    005BA851     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-11D    005BA858     83EC 74                  SUB ESP,0x74
	$-11A    005BA85B     53                       PUSH EBX
	$-119    005BA85C     56                       PUSH ESI
	$-118    005BA85D     57                       PUSH EDI
	$-117    005BA85E     8BF9                     MOV EDI,ECX
	$-115    005BA860     8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-112    005BA863     6A 0C                    PUSH 0xC
	$-110    005BA865     50                       PUSH EAX
	$-10F    005BA866     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$-109    005BA86C     81C1 6C430000            ADD ECX,0x436C
	$-103    005BA872     E8 A922E1FF              CALL PathOfEx.003CCB20
	$-FE     005BA877     8B75 E0                  MOV ESI,DWORD PTR SS:[EBP-0x20]
	$-FB     005BA87A     837E 0C 00               CMP DWORD PTR DS:[ESI+0xC],0x0
	$-F7     005BA87E     0F86 C4030000            JBE PathOfEx.005BAC48
	$-F1     005BA884     837E 10 00               CMP DWORD PTR DS:[ESI+0x10],0x0
	$-ED     005BA888     0F86 BA030000            JBE PathOfEx.005BAC48
	$-E7     005BA88E     8B46 20                  MOV EAX,DWORD PTR DS:[ESI+0x20]
	$-E4     005BA891     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-E2     005BA893     85C0                     TEST EAX,EAX
	$-E0     005BA895     0F84 AD030000            JE PathOfEx.005BAC48
	$-DA     005BA89B     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-D7     005BA89E     85C0                     TEST EAX,EAX
	$-D5     005BA8A0     0F84 A2030000            JE PathOfEx.005BAC48
	$-CF     005BA8A6     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-CC     005BA8A9     8D4E 2C                  LEA ECX,DWORD PTR DS:[ESI+0x2C]
	$-C9     005BA8AC     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-C6     005BA8AF     50                       PUSH EAX
	$-C5     005BA8B0     8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-C2     005BA8B3     50                       PUSH EAX
	$-C1     005BA8B4     E8 E796EBFF              CALL PathOfEx.00473FA0
	$-BC     005BA8B9     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-B9     005BA8BC     3B46 2C                  CMP EAX,DWORD PTR DS:[ESI+0x2C]
	$-B6     005BA8BF     75 04                    JNZ SHORT PathOfEx.005BA8C5
	$-B4     005BA8C1     33C0                     XOR EAX,EAX
	$-B2     005BA8C3     EB 05                    JMP SHORT PathOfEx.005BA8CA
	$-B0     005BA8C5     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-AD     005BA8C8     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-AB     005BA8CA     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-A8     005BA8CD     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-A1     005BA8D4     8B5D E4                  MOV EBX,DWORD PTR SS:[EBP-0x1C]
	$-9E     005BA8D7     85DB                     TEST EBX,EBX
	$-9C     005BA8D9     74 21                    JE SHORT PathOfEx.005BA8FC
	$-9A     005BA8DB     83CE FF                  OR ESI,0xFFFFFFFF
	$-97     005BA8DE     8BC6                     MOV EAX,ESI
	$-95     005BA8E0     F0:0FC143 04             LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
	$-90     005BA8E5     75 15                    JNZ SHORT PathOfEx.005BA8FC
	$-8E     005BA8E7     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-8C     005BA8E9     8BCB                     MOV ECX,EBX
	$-8A     005BA8EB     FF10                     CALL DWORD PTR DS:[EAX]
	$-88     005BA8ED     F0:0FC173 08             LOCK XADD DWORD PTR DS:[EBX+0x8],ESI
	$-83     005BA8F2     4E                       DEC ESI
	$-82     005BA8F3     75 07                    JNZ SHORT PathOfEx.005BA8FC
	$-80     005BA8F5     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-7E     005BA8F7     8BCB                     MOV ECX,EBX
	$-7C     005BA8F9     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$-79     005BA8FC     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-72     005BA903     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$-6C     005BA909     8B80 14410000            MOV EAX,DWORD PTR DS:[EAX+0x4114]
	$-66     005BA90F     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-63     005BA912     8B40 14                  MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-60     005BA915     8A80 D0000000            MOV AL,BYTE PTR DS:[EAX+0xD0]
	$-5A     005BA91B     84C0                     TEST AL,AL
	$-58     005BA91D     74 1A                    JE SHORT PathOfEx.005BA939
	$-56     005BA91F     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$-50     005BA925     8B80 D80A0000            MOV EAX,DWORD PTR DS:[EAX+0xAD8]
	$-4A     005BA92B     8A80 54070000            MOV AL,BYTE PTR DS:[EAX+0x754]
	$-44     005BA931     84C0                     TEST AL,AL
	$-42     005BA933     0F85 3E030000            JNZ PathOfEx.005BAC77
	$-3C     005BA939     8B5D EC                  MOV EBX,DWORD PTR SS:[EBP-0x14]
	$-39     005BA93C     85DB                     TEST EBX,EBX
	$-37     005BA93E     0F84 33030000            JE PathOfEx.005BAC77
	$-31     005BA944     8BCB                     MOV ECX,EBX
	$-2F     005BA946     E8 75F3DCFF              CALL PathOfEx.00389CC0
	$-2A     005BA94B     85C0                     TEST EAX,EAX
	$-28     005BA94D     74 0F                    JE SHORT PathOfEx.005BA95E
	$-26     005BA94F     8BC8                     MOV ECX,EAX
	$-24     005BA951     E8 2AF40800              CALL PathOfEx.00649D80
	$-1F     005BA956     C645 F3 01               MOV BYTE PTR SS:[EBP-0xD],0x1
	$-1B     005BA95A     84C0                     TEST AL,AL
	$-19     005BA95C     75 04                    JNZ SHORT PathOfEx.005BA962
	$-17     005BA95E     C645 F3 00               MOV BYTE PTR SS:[EBP-0xD],0x0
	$-13     005BA962     8BCB                     MOV ECX,EBX
	$-11     005BA964     E8 D7ABDBFF              CALL PathOfEx.00375540
	$-C      005BA969     8BF0                     MOV ESI,EAX
	$-A      005BA96B     8BCE                     MOV ECX,ESI
	$-8      005BA96D     E8 EE320800              CALL PathOfEx.0063DC60
	$-3      005BA972     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$ ==>    005BA975     8AA6 82000000            MOV AH,BYTE PTR DS:[ESI+0x82]                         ; g_o_item_base_is_bind_
	$+6      005BA97B     8B51 14                  MOV EDX,DWORD PTR DS:[ECX+0x14]
	$+9      005BA97E     8A4A 0C                  MOV CL,BYTE PTR DS:[EDX+0xC]                          ; stCD_TblItemWorldArea.is_town_
	$+C      005BA981     84C9                     TEST CL,CL
	$+E      005BA983     0F85 EE000000            JNZ PathOfEx.005BAA77
	$+14     005BA989     8A8A D0000000            MOV CL,BYTE PTR DS:[EDX+0xD0]
	$+1A     005BA98F     84C9                     TEST CL,CL
	$+1C     005BA991     0F85 E0000000            JNZ PathOfEx.005BAA77
	$+22     005BA997     84E4                     TEST AH,AH
	$+24     005BA999     0F85 D8000000            JNZ PathOfEx.005BAA77
	$+2A     005BA99F     84C0                     TEST AL,AL
	$+2C     005BA9A1     74 50                    JE SHORT PathOfEx.005BA9F3
	$+2E     005BA9A3     6A 00                    PUSH 0x0
	$+30     005BA9A5     6A 00                    PUSH 0x0
	$+32     005BA9A7     83EC 2C                  SUB ESP,0x2C
	$+35     005BA9AA     8BC4                     MOV EAX,ESP
	$+37     005BA9AC     C74424 28 00006144       MOV DWORD PTR SS:[ESP+0x28],0x44610000                ; ASCII "饨*"
	$+3F     005BA9B4     83EC 28                  SUB ESP,0x28
	$+42     005BA9B7     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+49     005BA9BE     8BC4                     MOV EAX,ESP
	$+4B     005BA9C0     83EC 10                  SUB ESP,0x10
	$+4E     005BA9C3     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+55     005BA9CA     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$+5B     005BA9D0     68 55030000              PUSH 0x355
	$+60     005BA9D5     8B88 200B0000            MOV ECX,DWORD PTR DS:[EAX+0xB20]
	$+66     005BA9DB     E8 E0A7DDFF              CALL PathOfEx.003951C0
	$+6B     005BA9E0     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+6E     005BA9E3     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+75     005BA9EA     5F                       POP EDI
	$+76     005BA9EB     5E                       POP ESI
	$+77     005BA9EC     5B                       POP EBX
	$+78     005BA9ED     8BE5                     MOV ESP,EBP
	$+7A     005BA9EF     5D                       POP EBP
	$+7B     005BA9F0     C2 0400                  RETN 0x4
	$+7E     005BA9F3     807D F3 00               CMP BYTE PTR SS:[EBP-0xD],0x0
	$+82     005BA9F7     74 60                    JE SHORT PathOfEx.005BAA59
	$+84     005BA9F9     83EC 28                  SUB ESP,0x28
	$+87     005BA9FC     8BC4                     MOV EAX,ESP
	$+89     005BA9FE     83EC 28                  SUB ESP,0x28
	$+8C     005BAA01     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+93     005BAA08     8BC4                     MOV EAX,ESP
	$+95     005BAA0A     6A 00                    PUSH 0x0
	$+97     005BAA0C     6A 00                    PUSH 0x0
	$+99     005BAA0E     83EC 08                  SUB ESP,0x8
	$+9C     005BAA11     C700 2CEEF000            MOV DWORD PTR DS:[EAX],PathOfEx.00F0EE2C
	$+A2     005BAA17     8978 04                  MOV DWORD PTR DS:[EAX+0x4],EDI
	$+A5     005BAA1A     8940 24                  MOV DWORD PTR DS:[EAX+0x24],EAX
	$+A8     005BAA1D     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$+AE     005BAA23     C74424 04 00006144       MOV DWORD PTR SS:[ESP+0x4],0x44610000                 ; ASCII "饨*"
	$+B6     005BAA2B     68 5A030000              PUSH 0x35A
	$+BB     005BAA30     68 58030000              PUSH 0x358
	$+C0     005BAA35     51                       PUSH ECX
	$+C1     005BAA36     8B88 200B0000            MOV ECX,DWORD PTR DS:[EAX+0xB20]
	$+C7     005BAA3C     68 57030000              PUSH 0x357
	$+CC     005BAA41     E8 1A61EBFF              CALL PathOfEx.00470B60
	$+D1     005BAA46     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+D4     005BAA49     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+DB     005BAA50     5F                       POP EDI
	$+DC     005BAA51     5E                       POP ESI
	$+DD     005BAA52     5B                       POP EBX
	$+DE     005BAA53     8BE5                     MOV ESP,EBP
	$+E0     005BAA55     5D                       POP EBP
	$+E1     005BAA56     C2 0400                  RETN 0x4
	$+E4     005BAA59     8B8F 3C010000            MOV ECX,DWORD PTR DS:[EDI+0x13C]
	$+EA     005BAA5F     E8 4C3F2200              CALL PathOfEx.007DE9B0
	$+EF     005BAA64     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+F2     005BAA67     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+F9     005BAA6E     5F                       POP EDI
	$+FA     005BAA6F     5E                       POP ESI
	$+FB     005BAA70     5B                       POP EBX
	$+FC     005BAA71     8BE5                     MOV ESP,EBP
	$+FE     005BAA73     5D                       POP EBP
	$+FF     005BAA74     C2 0400                  RETN 0x4
	$+102    005BAA77     84C0                     TEST AL,AL
	$+104    005BAA79     74 50                    JE SHORT PathOfEx.005BAACB
	$+106    005BAA7B     6A 00                    PUSH 0x0
	$+108    005BAA7D     6A 00                    PUSH 0x0
	$+10A    005BAA7F     83EC 2C                  SUB ESP,0x2C
	$+10D    005BAA82     8BC4                     MOV EAX,ESP
	$+10F    005BAA84     C74424 28 00006144       MOV DWORD PTR SS:[ESP+0x28],0x44610000                ; ASCII "饨*"
	$+117    005BAA8C     83EC 28                  SUB ESP,0x28
	$+11A    005BAA8F     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+121    005BAA96     8BC4                     MOV EAX,ESP
	$+123    005BAA98     83EC 10                  SUB ESP,0x10
	$+126    005BAA9B     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+12D    005BAAA2     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$+133    005BAAA8     68 53030000              PUSH 0x353
	$+138    005BAAAD     8B88 200B0000            MOV ECX,DWORD PTR DS:[EAX+0xB20]
	$+13E    005BAAB3     E8 08A7DDFF              CALL PathOfEx.003951C0
	$+143    005BAAB8     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+146    005BAABB     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+14D    005BAAC2     5F                       POP EDI
	$+14E    005BAAC3     5E                       POP ESI
	$+14F    005BAAC4     5B                       POP EBX
	$+150    005BAAC5     8BE5                     MOV ESP,EBP
	$+152    005BAAC7     5D                       POP EBP
	$+153    005BAAC8     C2 0400                  RETN 0x4
	$+156    005BAACB     8BCB                     MOV ECX,EBX
	$+158    005BAACD     E8 5E51DCFF              CALL PathOfEx.0037FC30
	$+15D    005BAAD2     84C0                     TEST AL,AL
	$+15F    005BAAD4     74 5B                    JE SHORT PathOfEx.005BAB31
	$+161    005BAAD6     8B87 3C010000            MOV EAX,DWORD PTR DS:[EDI+0x13C]
	$+167    005BAADC     8B80 14410000            MOV EAX,DWORD PTR DS:[EAX+0x4114]
	$+16D    005BAAE2     8B88 58010000            MOV ECX,DWORD PTR DS:[EAX+0x158]
	$+173    005BAAE8     E8 73F9D8FF              CALL PathOfEx.0034A460
	$+178    005BAAED     F680 A5000000 80         TEST BYTE PTR DS:[EAX+0xA5],0x80
	$+17F    005BAAF4     75 3B                    JNZ SHORT PathOfEx.005BAB31
	$+181    005BAAF6     68 E4F3EE00              PUSH PathOfEx.00EEF3E4                                ; UNICODE "QuestItem"
	$+186    005BAAFB     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+189    005BAAFE     E8 4DBED6FF              CALL PathOfEx.00326950
	$+18E    005BAB03     51                       PUSH ECX
	$+18F    005BAB04     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+192    005BAB07     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$+199    005BAB0E     50                       PUSH EAX
	$+19A    005BAB0F     8BCF                     MOV ECX,EDI
	$+19C    005BAB11     E8 BA240000              CALL PathOfEx.005BCFD0
	$+1A1    005BAB16     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+1A4    005BAB19     E8 82B7D6FF              CALL PathOfEx.003262A0
	$+1A9    005BAB1E     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1AC    005BAB21     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+1B3    005BAB28     5F                       POP EDI
	$+1B4    005BAB29     5E                       POP ESI
	$+1B5    005BAB2A     5B                       POP EBX
	$+1B6    005BAB2B     8BE5                     MOV ESP,EBP
	$+1B8    005BAB2D     5D                       POP EBP
	$+1B9    005BAB2E     C2 0400                  RETN 0x4
	$+1BC    005BAB31     33C0                     XOR EAX,EAX
	$+1BE    005BAB33     C745 DC 07000000         MOV DWORD PTR SS:[EBP-0x24],0x7
	$+1C5    005BAB3A     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
	$+1CC    005BAB41     66:8945 C8               MOV WORD PTR SS:[EBP-0x38],AX
	$+1D0    005BAB45     C745 FC 03000000         MOV DWORD PTR SS:[EBP-0x4],0x3
	$+1D7    005BAB4C     3845 F3                  CMP BYTE PTR SS:[EBP-0xD],AL
	$+1DA    005BAB4F     74 57                    JE SHORT PathOfEx.005BABA8
	$+1DC    005BAB51     BA 56030000              MOV EDX,0x356
	$+1E1    005BAB56     8D4D 80                  LEA ECX,DWORD PTR SS:[EBP-0x80]
	$+1E4    005BAB59     E8 E26D0500              CALL PathOfEx.00611940
	$+1E9    005BAB5E     50                       PUSH EAX
	$+1EA    005BAB5F     8D45 98                  LEA EAX,DWORD PTR SS:[EBP-0x68]
	$+1ED    005BAB62     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+1F1    005BAB66     68 78F7ED00              PUSH PathOfEx.00EDF778                                ; UNICODE "<red>{"
	$+1F6    005BAB6B     50                       PUSH EAX
	$+1F7    005BAB6C     E8 2FA9DBFF              CALL PathOfEx.003754A0
	$+1FC    005BAB71     68 807BED00              PUSH PathOfEx.00ED7B80                                ; UNICODE "}"
	$+201    005BAB76     50                       PUSH EAX
	$+202    005BAB77     8D45 B0                  LEA EAX,DWORD PTR SS:[EBP-0x50]
	$+205    005BAB7A     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
	$+209    005BAB7E     50                       PUSH EAX
	$+20A    005BAB7F     E8 4C3BD7FF              CALL PathOfEx.0032E6D0
	$+20F    005BAB84     83C4 18                  ADD ESP,0x18
	$+212    005BAB87     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+215    005BAB8A     50                       PUSH EAX
	$+216    005BAB8B     E8 B011D8FF              CALL PathOfEx.0033BD40
	$+21B    005BAB90     8D4D B0                  LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+21E    005BAB93     E8 08B7D6FF              CALL PathOfEx.003262A0
	$+223    005BAB98     8D4D 98                  LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+226    005BAB9B     E8 00B7D6FF              CALL PathOfEx.003262A0
	$+22B    005BABA0     8D4D 80                  LEA ECX,DWORD PTR SS:[EBP-0x80]
	$+22E    005BABA3     E8 F8B6D6FF              CALL PathOfEx.003262A0
	$+233    005BABA8     6A 00                    PUSH 0x0
	$+235    005BABAA     6A 00                    PUSH 0x0
	$+237    005BABAC     83EC 2C                  SUB ESP,0x2C
	$+23A    005BABAF     8BC4                     MOV EAX,ESP
	$+23C    005BABB1     8965 E8                  MOV DWORD PTR SS:[EBP-0x18],ESP
	$+23F    005BABB4     C74424 28 00006144       MOV DWORD PTR SS:[ESP+0x28],0x44610000                ; ASCII "饨*"
	$+247    005BABBC     C740 24 00000000         MOV DWORD PTR DS:[EAX+0x24],0x0
	$+24E    005BABC3     83EC 28                  SUB ESP,0x28
	$+251    005BABC6     8BC4                     MOV EAX,ESP
	$+253    005BABC8     8965 EC                  MOV DWORD PTR SS:[EBP-0x14],ESP
	$+256    005BABCB     C700 14EEF000            MOV DWORD PTR DS:[EAX],PathOfEx.00F0EE14
	$+25C    005BABD1     8978 04                  MOV DWORD PTR DS:[EAX+0x4],EDI
	$+25F    005BABD4     8940 24                  MOV DWORD PTR DS:[EAX+0x24],EAX
	$+262    005BABD7     6A 00                    PUSH 0x0
	$+264    005BABD9     68 5A030000              PUSH 0x35A
	$+269    005BABDE     68 59030000              PUSH 0x359
	$+26E    005BABE3     83EC 18                  SUB ESP,0x18
	$+271    005BABE6     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
	$+275    005BABEA     8BCC                     MOV ECX,ESP
	$+277    005BABEC     C741 14 07000000         MOV DWORD PTR DS:[ECX+0x14],0x7
	$+27E    005BABF3     C741 10 00000000         MOV DWORD PTR DS:[ECX+0x10],0x0
	$+285    005BABFA     8379 14 08               CMP DWORD PTR DS:[ECX+0x14],0x8
	$+289    005BABFE     72 04                    JB SHORT PathOfEx.005BAC04
	$+28B    005BAC00     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+28D    005BAC02     EB 02                    JMP SHORT PathOfEx.005BAC06
	$+28F    005BAC04     8BC1                     MOV EAX,ECX
	$+291    005BAC06     33D2                     XOR EDX,EDX
	$+293    005BAC08     6A FF                    PUSH -0x1
	$+295    005BAC0A     66:8910                  MOV WORD PTR DS:[EAX],DX
	$+298    005BAC0D     8D45 C8                  LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+29B    005BAC10     52                       PUSH EDX
	$+29C    005BAC11     50                       PUSH EAX
	$+29D    005BAC12     E8 C9B7D6FF              CALL PathOfEx.003263E0
	$+2A2    005BAC17     8B87 64190000            MOV EAX,DWORD PTR DS:[EDI+0x1964]
	$+2A8    005BAC1D     8B88 200B0000            MOV ECX,DWORD PTR DS:[EAX+0xB20]
	$+2AE    005BAC23     51                       PUSH ECX
	$+2AF    005BAC24     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2B3    005BAC28     E8 036E0000              CALL PathOfEx.005C1A30
	$+2B8    005BAC2D     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+2BB    005BAC30     E8 6BB6D6FF              CALL PathOfEx.003262A0
	$+2C0    005BAC35     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2C3    005BAC38     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+2CA    005BAC3F     5F                       POP EDI
	$+2CB    005BAC40     5E                       POP ESI
	$+2CC    005BAC41     5B                       POP EBX
	$+2CD    005BAC42     8BE5                     MOV ESP,EBP
	$+2CF    005BAC44     5D                       POP EBP
	$+2D0    005BAC45     C2 0400                  RETN 0x4
	$+2D3    005BAC48     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2DA    005BAC4F     8B7D E4                  MOV EDI,DWORD PTR SS:[EBP-0x1C]
	$+2DD    005BAC52     85FF                     TEST EDI,EDI
	$+2DF    005BAC54     74 21                    JE SHORT PathOfEx.005BAC77
	$+2E1    005BAC56     83CE FF                  OR ESI,0xFFFFFFFF
	$+2E4    005BAC59     8BC6                     MOV EAX,ESI
	$+2E6    005BAC5B     F0:0FC147 04             LOCK XADD DWORD PTR DS:[EDI+0x4],EAX
	$+2EB    005BAC60     75 15                    JNZ SHORT PathOfEx.005BAC77
	$+2ED    005BAC62     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+2EF    005BAC64     8BCF                     MOV ECX,EDI
	$+2F1    005BAC66     FF10                     CALL DWORD PTR DS:[EAX]
	$+2F3    005BAC68     F0:0FC177 08             LOCK XADD DWORD PTR DS:[EDI+0x8],ESI
	$+2F8    005BAC6D     4E                       DEC ESI
	$+2F9    005BAC6E     75 07                    JNZ SHORT PathOfEx.005BAC77
	$+2FB    005BAC70     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+2FD    005BAC72     8BCF                     MOV ECX,EDI
	$+2FF    005BAC74     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+302    005BAC77     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+305    005BAC7A     5F                       POP EDI
	$+306    005BAC7B     5E                       POP ESI
	$+307    005BAC7C     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+30E    005BAC83     5B                       POP EBX
	$+30F    005BAC84     8BE5                     MOV ESP,EBP
	$+311    005BAC86     5D                       POP EBP
	$+312    005BAC87     C2 0400                  RETN 0x4
	$+315    005BAC8A     CC                       INT3
	$+316    005BAC8B     CC                       INT3
	$+317    005BAC8C     CC                       INT3
	$+318    005BAC8D     CC                       INT3
	$+319    005BAC8E     CC                       INT3
	$+31A    005BAC8F     CC                       INT3
	$+31B    005BAC90     51                       PUSH ECX
	$+31C    005BAC91     807C24 08 00             CMP BYTE PTR SS:[ESP+0x8],0x0
	$+321    005BAC96     56                       PUSH ESI
	$+322    005BAC97     8BF1                     MOV ESI,ECX
	$+324    005BAC99     75 1B                    JNZ SHORT PathOfEx.005BACB6
	$+326    005BAC9B     8B8E 34010000            MOV ECX,DWORD PTR DS:[ESI+0x134]
	$+32C    005BACA1     E8 8A4A1000              CALL PathOfEx.006BF730
	$+331    005BACA6     85C0                     TEST EAX,EAX
	$+333    005BACA8     74 0C                    JE SHORT PathOfEx.005BACB6
	$+335    005BACAA     C686 60190000 00         MOV BYTE PTR DS:[ESI+0x1960],0x0
	$+33C    005BACB1     5E                       POP ESI
	$+33D    005BACB2     59                       POP ECX
	$+33E    005BACB3     C2 0400                  RETN 0x4
	$+341    005BACB6     C686 60190000 01         MOV BYTE PTR DS:[ESI+0x1960],0x1
	$+348    005BACBD     5E                       POP ESI
	$+349    005BACBE     59                       POP ECX
	$+34A    005BACBF     C2 0400                  RETN 0x4
	$+34D    005BACC2     CC                       INT3
	$+34E    005BACC3     CC                       INT3
	$+34F    005BACC4     CC                       INT3
	$+350    005BACC5     CC                       INT3
	$+351    005BACC6     CC                       INT3
	$+352    005BACC7     CC                       INT3
	$+353    005BACC8     CC                       INT3
	$+354    005BACC9     CC                       INT3
	$+355    005BACCA     CC                       INT3
	$+356    005BACCB     CC                       INT3
	$+357    005BACCC     CC                       INT3
	$+358    005BACCD     CC                       INT3
	$+359    005BACCE     CC                       INT3
	$+35A    005BACCF     CC                       INT3
	$+35B    005BACD0     56                       PUSH ESI
	$+35C    005BACD1     8BB1 64190000            MOV ESI,DWORD PTR DS:[ECX+0x1964]
	$+362    005BACD7     8B8E 380B0000            MOV ECX,DWORD PTR DS:[ESI+0xB38]
	$+368    005BACDD     E8 8E293700              CALL PathOfEx.0092D670
	$+36D    005BACE2     807C24 0C 00             CMP BYTE PTR SS:[ESP+0xC],0x0
	$+372    005BACE7     74 23                    JE SHORT PathOfEx.005BAD0C
	$+374    005BACE9     84C0                     TEST AL,AL
	$+376    005BACEB     74 3B                    JE SHORT PathOfEx.005BAD28
	$+378    005BACED     8B86 380B0000            MOV EAX,DWORD PTR DS:[ESI+0xB38]
	$+37E    005BACF3     33C9                     XOR ECX,ECX
	$+380    005BACF5     51                       PUSH ECX
	$+381    005BACF6     66:8988 4C070000         MOV WORD PTR DS:[EAX+0x74C],CX
	$+388    005BACFD     8B8E 380B0000            MOV ECX,DWORD PTR DS:[ESI+0xB38]
	$+38E    005BAD03     E8 78283700              CALL PathOfEx.0092D580
	$+393    005BAD08     5E                       POP ESI
	$+394    005BAD09     C2 1800                  RETN 0x18
	$+397    005BAD0C     84C0                     TEST AL,AL
	$+399    005BAD0E     75 18                    JNZ SHORT PathOfEx.005BAD28
	$+39B    005BAD10     83EC 08                  SUB ESP,0x8
	$+39E    005BAD13     8BCE                     MOV ECX,ESI
	$+3A0    005BAD15     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$+3A4    005BAD19     FF7424 1C                PUSH DWORD PTR SS:[ESP+0x1C]
	$+3A8    005BAD1D     FF7424 18                PUSH DWORD PTR SS:[ESP+0x18]
	$+3AC    005BAD21     6A 01                    PUSH 0x1
	$+3AE    005BAD23     E8 28E4FAFF              CALL PathOfEx.00569150
	$+3B3    005BAD28     5E                       POP ESI
	$+3B4    005BAD29     C2 1800                  RETN 0x18
	$+3B7    005BAD2C     CC                       INT3
	$+3B8    005BAD2D     CC                       INT3
	$+3B9    005BAD2E     CC                       INT3
	$+3BA    005BAD2F     CC                       INT3
	$+3BB    005BAD30     8B4424 04                MOV EAX,DWORD PTR SS:[ESP+0x4]
	$+3BF    005BAD34     8B91 14010000            MOV EDX,DWORD PTR DS:[ECX+0x114]
	$+3C5    005BAD3A     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+3C8    005BAD3D     3B82 58010000            CMP EAX,DWORD PTR DS:[EDX+0x158]
	$+3CE    005BAD43     75 23                    JNZ SHORT PathOfEx.005BAD68
	$+3D0    005BAD45     83B9 581A0000 00         CMP DWORD PTR DS:[ECX+0x1A58],0x0
	$+3D7    005BAD4C     C781 540C0000 00000000   MOV DWORD PTR DS:[ECX+0xC54],0x0
	$+3E1    005BAD56     74 10                    JE SHORT PathOfEx.005BAD68
	$+3E3    005BAD58     C74424 04 00000000       MOV DWORD PTR SS:[ESP+0x4],0x0
	$+3EB    005BAD60     83C1 DC                  ADD ECX,-0x24
	$+3EE    005BAD63     E9 E84A0000              JMP PathOfEx.005BF850
	$+3F3    005BAD68     C2 0400                  RETN 0x4
	$+3F6    005BAD6B     CC                       INT3
	$+3F7    005BAD6C     CC                       INT3
	$+3F8    005BAD6D     CC                       INT3
	$+3F9    005BAD6E     CC                       INT3
	$+3FA    005BAD6F     CC                       INT3
	$+3FB    005BAD70     55                       PUSH EBP
	$+3FC    005BAD71     8BEC                     MOV EBP,ESP
	$+3FE    005BAD73     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+404    005BAD79     6A FF                    PUSH -0x1
	$+406    005BAD7B     68 783FD800              PUSH PathOfEx.00D83F78
	$+40B    005BAD80     50                       PUSH EAX
	$+40C    005BAD81     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+413    005BAD88     83EC 30                  SUB ESP,0x30
	$+416    005BAD8B     807D 10 00               CMP BYTE PTR SS:[EBP+0x10],0x0
	$+41A    005BAD8F     56                       PUSH ESI
	$+41B    005BAD90     8BF1                     MOV ESI,ECX
	$+41D    005BAD92     0F85 52030000            JNZ PathOfEx.005BB0EA
	$+423    005BAD98     8B86 18010000            MOV EAX,DWORD PTR DS:[ESI+0x118]
	$+429    005BAD9E     85C0                     TEST EAX,EAX
	$+42B    005BADA0     0F84 44030000            JE PathOfEx.005BB0EA
	$+431    005BADA6     83B8 B8460000 00         CMP DWORD PTR DS:[EAX+0x46B8],0x0
	$+438    005BADAD     0F87 37030000            JA PathOfEx.005BB0EA
	$+43E    005BADB3     8B8E 14010000            MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+444    005BADB9     83B9 58010000 00         CMP DWORD PTR DS:[ECX+0x158],0x0
	$+44B    005BADC0     0F84 24030000            JE PathOfEx.005BB0EA
	$+451    005BADC6     57                       PUSH EDI
	$+452    005BADC7     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+455    005BADCA     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+458    005BADCD     3B81 58010000            CMP EAX,DWORD PTR DS:[ECX+0x158]
	$+45E    005BADD3     75 58                    JNZ SHORT PathOfEx.005BAE2D
	$+460    005BADD5     8B8E 40190000            MOV ECX,DWORD PTR DS:[ESI+0x1940]
	$+466    005BADDB     85C9                     TEST ECX,ECX
	$+468    005BADDD     74 0B                    JE SHORT PathOfEx.005BADEA
	$+46A    005BADDF     8B89 8C090000            MOV ECX,DWORD PTR DS:[ECX+0x98C]
	$+470    005BADE5     E8 5636EAFF              CALL PathOfEx.0045E440
	$+475    005BADEA     8D8E 3C1A0000            LEA ECX,DWORD PTR DS:[ESI+0x1A3C]
	$+47B    005BADF0     E8 3B38DAFF              CALL PathOfEx.0035E630
	$+480    005BADF5     83BE 540C0000 02         CMP DWORD PTR DS:[ESI+0xC54],0x2
	$+487    005BADFC     74 0A                    JE SHORT PathOfEx.005BAE08
	$+489    005BADFE     C786 540C0000 01000000   MOV DWORD PTR DS:[ESI+0xC54],0x1
	$+493    005BAE08     8B8E 40190000            MOV ECX,DWORD PTR DS:[ESI+0x1940]
	$+499    005BAE0E     E8 4D90FAFF              CALL PathOfEx.00563E60
	$+49E    005BAE13     8D4E DC                  LEA ECX,DWORD PTR DS:[ESI-0x24]
	$+4A1    005BAE16     E8 B5390000              CALL PathOfEx.005BE7D0
	$+4A6    005BAE1B     5F                       POP EDI
	$+4A7    005BAE1C     5E                       POP ESI
	$+4A8    005BAE1D     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+4AB    005BAE20     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+4B2    005BAE27     8BE5                     MOV ESP,EBP
	$+4B4    005BAE29     5D                       POP EBP
	$+4B5    005BAE2A     C2 0C00                  RETN 0xC
	$+4B8    005BAE2D     8B81 58010000            MOV EAX,DWORD PTR DS:[ECX+0x158]
	$+4BE    005BAE33     8BC8                     MOV ECX,EAX
	$+4C0    005BAE35     53                       PUSH EBX
	$+4C1    005BAE36     8945 10                  MOV DWORD PTR SS:[EBP+0x10],EAX
	$+4C4    005BAE39     E8 729BD6FF              CALL PathOfEx.003249B0
	$+4C9    005BAE3E     8BD8                     MOV EBX,EAX
	$+4CB    005BAE40     85DB                     TEST EBX,EBX
	$+4CD    005BAE42     0F84 A0020000            JE PathOfEx.005BB0E8
	$+4D3    005BAE48     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$+4D6    005BAE4B     8B48 2C                  MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$+4D9    005BAE4E     8B45 10                  MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+4DC    005BAE51     8B40 2C                  MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+4DF    005BAE54     8945 10                  MOV DWORD PTR SS:[EBP+0x10],EAX
	$+4E2    005BAE57     8B50 14                  MOV EDX,DWORD PTR DS:[EAX+0x14]
	$+4E5    005BAE5A     8B40 10                  MOV EAX,DWORD PTR DS:[EAX+0x10]
	$+4E8    005BAE5D     2B51 14                  SUB EDX,DWORD PTR DS:[ECX+0x14]
	$+4EB    005BAE60     2B41 10                  SUB EAX,DWORD PTR DS:[ECX+0x10]
	$+4EE    005BAE63     0FAFD2                   IMUL EDX,EDX
	$+4F1    005BAE66     0FAFC0                   IMUL EAX,EAX
	$+4F4    005BAE69     03D0                     ADD EDX,EAX
	$+4F6    005BAE6B     81FA A9330000            CMP EDX,0x33A9
	$+4FC    005BAE71     0F87 71020000            JA PathOfEx.005BB0E8
	$+502    005BAE77     8B4F 04                  MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+505    005BAE7A     8B45 10                  MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+508    005BAE7D     8B51 2C                  MOV EDX,DWORD PTR DS:[ECX+0x2C]
	$+50B    005BAE80     8A80 F8000000            MOV AL,BYTE PTR DS:[EAX+0xF8]
	$+511    005BAE86     3282 F8000000            XOR AL,BYTE PTR DS:[EDX+0xF8]
	$+517    005BAE8C     A8 7F                    TEST AL,0x7F
	$+519    005BAE8E     0F84 54020000            JE PathOfEx.005BB0E8
	$+51F    005BAE94     E8 6738DAFF              CALL PathOfEx.0035E700
	$+524    005BAE99     85C0                     TEST EAX,EAX
	$+526    005BAE9B     74 29                    JE SHORT PathOfEx.005BAEC6
	$+528    005BAE9D     8B48 64                  MOV ECX,DWORD PTR DS:[EAX+0x64]
	$+52B    005BAEA0     8B50 60                  MOV EDX,DWORD PTR DS:[EAX+0x60]
	$+52E    005BAEA3     3BD1                     CMP EDX,ECX
	$+530    005BAEA5     74 1F                    JE SHORT PathOfEx.005BAEC6
	$+532    005BAEA7     0FB73D 5CA50A01          MOVZX EDI,WORD PTR DS:[0x10AA55C]
	$+539    005BAEAE     66:90                    NOP
	$+53B    005BAEB0     8B42 10                  MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+53E    005BAEB3     3978 04                  CMP DWORD PTR DS:[EAX+0x4],EDI
	$+541    005BAEB6     0F84 2C020000            JE PathOfEx.005BB0E8
	$+547    005BAEBC     83C2 14                  ADD EDX,0x14
	$+54A    005BAEBF     3BD1                     CMP EDX,ECX
	$+54C    005BAEC1   ^ 75 ED                    JNZ SHORT PathOfEx.005BAEB0
	$+54E    005BAEC3     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+551    005BAEC6     8B86 18010000            MOV EAX,DWORD PTR DS:[ESI+0x118]
	$+557    005BAECC     83B8 88420000 00         CMP DWORD PTR DS:[EAX+0x4288],0x0
	$+55E    005BAED3     0F85 AD000000            JNZ PathOfEx.005BAF86
	$+564    005BAED9     8B4F 04                  MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+567    005BAEDC     E8 2F86FFFF              CALL PathOfEx.005B3510
	$+56C    005BAEE1     84C0                     TEST AL,AL
	$+56E    005BAEE3     0F84 9D000000            JE PathOfEx.005BAF86
	$+574    005BAEE9     68 18ECF000              PUSH PathOfEx.00F0EC18                                ; UNICODE "KillRare"
	$+579    005BAEEE     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+57C    005BAEF1     E8 5ABAD6FF              CALL PathOfEx.00326950
	$+581    005BAEF6     51                       PUSH ECX
	$+582    005BAEF7     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+585    005BAEFA     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+58C    005BAF01     50                       PUSH EAX
	$+58D    005BAF02     8D4E DC                  LEA ECX,DWORD PTR DS:[ESI-0x24]
	$+590    005BAF05     E8 C6200000              CALL PathOfEx.005BCFD0
	$+595    005BAF0A     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+598    005BAF0D     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+59F    005BAF14     E8 87B3D6FF              CALL PathOfEx.003262A0
	$+5A4    005BAF19     8B4B 10                  MOV ECX,DWORD PTR DS:[EBX+0x10]
	$+5A7    005BAF1C     68 9B0B0000              PUSH 0xB9B
	$+5AC    005BAF21     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+5AE    005BAF23     FF10                     CALL DWORD PTR DS:[EAX]
	$+5B0    005BAF25     85C0                     TEST EAX,EAX
	$+5B2    005BAF27     74 5D                    JE SHORT PathOfEx.005BAF86
	$+5B4    005BAF29     68 90EAF000              PUSH PathOfEx.00F0EA90                                ; UNICODE "Audio/Sound Effects/Misc/WhisperingIce/Whisper_$(#"
	$+5B9    005BAF2E     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+5BC    005BAF31     E8 1ABAD6FF              CALL PathOfEx.00326950
	$+5C1    005BAF36     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+5C4    005BAF39     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+5CB    005BAF40     50                       PUSH EAX
	$+5CC    005BAF41     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+5CF    005BAF44     E8 17622700              CALL PathOfEx.00831160
	$+5D4    005BAF49     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+5D7    005BAF4C     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$+5DB    005BAF50     E8 4BB3D6FF              CALL PathOfEx.003262A0
	$+5E0    005BAF55     E8 A6862500              CALL PathOfEx.00813600
	$+5E5    005BAF5A     83EC 08                  SUB ESP,0x8
	$+5E8    005BAF5D     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+5EB    005BAF60     C74424 04 0000803F       MOV DWORD PTR SS:[ESP+0x4],0x3F800000
	$+5F3    005BAF68     C70424 0000803F          MOV DWORD PTR SS:[ESP],0x3F800000
	$+5FA    005BAF6F     51                       PUSH ECX
	$+5FB    005BAF70     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+5FD    005BAF72     E8 B9752500              CALL PathOfEx.00812530
	$+602    005BAF77     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+605    005BAF7A     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+60C    005BAF81     E8 DA6ED7FF              CALL PathOfEx.00331E60
	$+611    005BAF86     8B4B 10                  MOV ECX,DWORD PTR DS:[EBX+0x10]
	$+614    005BAF89     68 21090000              PUSH 0x921
	$+619    005BAF8E     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+61B    005BAF90     FF10                     CALL DWORD PTR DS:[EAX]
	$+61D    005BAF92     85C0                     TEST EAX,EAX
	$+61F    005BAF94     74 60                    JE SHORT PathOfEx.005BAFF6
	$+621    005BAF96     8B4F 04                  MOV ECX,DWORD PTR DS:[EDI+0x4]
	$+624    005BAF99     E8 62F2DAFF              CALL PathOfEx.0036A200
	$+629    005BAF9E     84C0                     TEST AL,AL
	$+62B    005BAFA0     74 16                    JE SHORT PathOfEx.005BAFB8
	$+62D    005BAFA2     68 04EBF000              PUSH PathOfEx.00F0EB04                                ; UNICODE "EventJackKillPlayer"
	$+632    005BAFA7     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+635    005BAFAA     E8 A1B9D6FF              CALL PathOfEx.00326950
	$+63A    005BAFAF     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
	$+641    005BAFB6     EB 22                    JMP SHORT PathOfEx.005BAFDA
	$+643    005BAFB8     6A 32                    PUSH 0x32
	$+645    005BAFBA     E8 61FA0300              CALL PathOfEx.005FAA20
	$+64A    005BAFBF     83C4 04                  ADD ESP,0x4
	$+64D    005BAFC2     85C0                     TEST EAX,EAX
	$+64F    005BAFC4     75 30                    JNZ SHORT PathOfEx.005BAFF6
	$+651    005BAFC6     68 2CEBF000              PUSH PathOfEx.00F0EB2C                                ; UNICODE "EventJackKillMonster"
	$+656    005BAFCB     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+659    005BAFCE     E8 7DB9D6FF              CALL PathOfEx.00326950
	$+65E    005BAFD3     C745 FC 05000000         MOV DWORD PTR SS:[EBP-0x4],0x5
	$+665    005BAFDA     51                       PUSH ECX
	$+666    005BAFDB     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+669    005BAFDE     50                       PUSH EAX
	$+66A    005BAFDF     8D4E DC                  LEA ECX,DWORD PTR DS:[ESI-0x24]
	$+66D    005BAFE2     E8 E91F0000              CALL PathOfEx.005BCFD0
	$+672    005BAFE7     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+675    005BAFEA     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+67C    005BAFF1     E8 AAB2D6FF              CALL PathOfEx.003262A0
	$+681    005BAFF6     8B4B 10                  MOV ECX,DWORD PTR DS:[EBX+0x10]
	$+684    005BAFF9     68 22090000              PUSH 0x922
	$+689    005BAFFE     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+68B    005BB000     FF10                     CALL DWORD PTR DS:[EAX]
	$+68D    005BB002     85C0                     TEST EAX,EAX
	$+68F    005BB004     74 6E                    JE SHORT PathOfEx.005BB074
	$+691    005BB006     68 C8000000              PUSH 0xC8
	$+696    005BB00B     E8 10FA0300              CALL PathOfEx.005FAA20
	$+69B    005BB010     83C4 04                  ADD ESP,0x4
	$+69E    005BB013     85C0                     TEST EAX,EAX
	$+6A0    005BB015     75 5D                    JNZ SHORT PathOfEx.005BB074
	$+6A2    005BB017     68 58EBF000              PUSH PathOfEx.00F0EB58                                ; UNICODE "Audio/Sound Effects/Misc/WolfUnique/Wolf_$(#).oggS"
	$+6A7    005BB01C     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+6AA    005BB01F     E8 2CB9D6FF              CALL PathOfEx.00326950
	$+6AF    005BB024     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+6B2    005BB027     C745 FC 06000000         MOV DWORD PTR SS:[EBP-0x4],0x6
	$+6B9    005BB02E     50                       PUSH EAX
	$+6BA    005BB02F     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+6BD    005BB032     E8 29612700              CALL PathOfEx.00831160
	$+6C2    005BB037     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+6C5    005BB03A     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+6C9    005BB03E     E8 5DB2D6FF              CALL PathOfEx.003262A0
	$+6CE    005BB043     E8 B8852500              CALL PathOfEx.00813600
	$+6D3    005BB048     83EC 08                  SUB ESP,0x8
	$+6D6    005BB04B     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+6D9    005BB04E     C74424 04 0000803F       MOV DWORD PTR SS:[ESP+0x4],0x3F800000
	$+6E1    005BB056     C70424 0000803F          MOV DWORD PTR SS:[ESP],0x3F800000
	$+6E8    005BB05D     51                       PUSH ECX
	$+6E9    005BB05E     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+6EB    005BB060     E8 CB742500              CALL PathOfEx.00812530
	$+6F0    005BB065     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+6F3    005BB068     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+6FA    005BB06F     E8 EC6DD7FF              CALL PathOfEx.00331E60
	$+6FF    005BB074     8B4B 10                  MOV ECX,DWORD PTR DS:[EBX+0x10]
	$+702    005BB077     68 AD110000              PUSH 0x11AD
	$+707    005BB07C     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+709    005BB07E     FF10                     CALL DWORD PTR DS:[EAX]
	$+70B    005BB080     85C0                     TEST EAX,EAX
	$+70D    005BB082     74 64                    JE SHORT PathOfEx.005BB0E8
	$+70F    005BB084     6A 0A                    PUSH 0xA
	$+711    005BB086     E8 95F90300              CALL PathOfEx.005FAA20
	$+716    005BB08B     83C4 04                  ADD ESP,0x4
	$+719    005BB08E     85C0                     TEST EAX,EAX
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8a??????????8b????8a????84??0f??????????8a??????????84", 2);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8a??????????8b????8a????84??0f??????????8a??????????84", 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x50 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_item_base_is_bind_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-4E1    0072A944     C745 FC 0E000000            MOV DWORD PTR SS:[EBP-0x4],0xE
	$-4DA    0072A94B     EB 3E                       JMP SHORT PathOfEx.0072A98B
	$-4D8    0072A94D     6A FF                       PUSH -0x1
	$-4D6    0072A94F     6A 00                       PUSH 0x0
	$-4D4    0072A951     50                          PUSH EAX
	$-4D3    0072A952     8BCB                        MOV ECX,EBX
	$-4D1    0072A954     E8 27F0D4FF                 CALL PathOfEx.00479980
	$-4CC    0072A959     8B4D F0                     MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-4C9    0072A95C     8379 10 00                  CMP DWORD PTR DS:[ECX+0x10],0x0
	$-4C5    0072A960     74 17                       JE SHORT PathOfEx.0072A979
	$-4C3    0072A962     8B41 18                     MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-4C0    0072A965     3B41 1C                     CMP EAX,DWORD PTR DS:[ECX+0x1C]
	$-4BD    0072A968     74 0F                       JE SHORT PathOfEx.0072A979
	$-4BB    0072A96A     68 B8B1E700                 PUSH PathOfEx.00E7B1B8                                ; :
	$-4B6    0072A96F     8BCB                        MOV ECX,EBX
	$-4B4    0072A971     E8 2ADED5FF                 CALL PathOfEx.004887A0
	$-4AF    0072A976     8B4D F0                     MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-4AC    0072A979     8D51 18                     LEA EDX,DWORD PTR DS:[ECX+0x18]
	$-4A9    0072A97C     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-4A6    0072A97F     E8 8C89FFFF                 CALL PathOfEx.00723310
	$-4A1    0072A984     C745 FC 0F000000            MOV DWORD PTR SS:[EBP-0x4],0xF
	$-49A    0072A98B     6A FF                       PUSH -0x1
	$-498    0072A98D     6A 00                       PUSH 0x0
	$-496    0072A98F     50                          PUSH EAX
	$-495    0072A990     8BCB                        MOV ECX,EBX
	$-493    0072A992     E8 E9EFD4FF                 CALL PathOfEx.00479980
	$-48E    0072A997     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-487    0072A99E     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-484    0072A9A1     83F8 08                     CMP EAX,0x8
	$-481    0072A9A4     72 0F                       JB SHORT PathOfEx.0072A9B5
	$-47F    0072A9A6     6A 02                       PUSH 0x2
	$-47D    0072A9A8     40                          INC EAX
	$-47C    0072A9A9     50                          PUSH EAX
	$-47B    0072A9AA     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$-478    0072A9AD     E8 7E1ED4FF                 CALL PathOfEx.0046C830
	$-473    0072A9B2     83C4 0C                     ADD ESP,0xC
	$-470    0072A9B5     6A 02                       PUSH 0x2
	$-46E    0072A9B7     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$-469    0072A9BC     8BCB                        MOV ECX,EBX
	$-467    0072A9BE     E8 4D27D5FF                 CALL PathOfEx.0047D110
	$-462    0072A9C3     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-45F    0072A9C6     83C6 48                     ADD ESI,0x48
	$-45C    0072A9C9     83C0 48                     ADD EAX,0x48
	$-459    0072A9CC   ^ E9 14FFFFFF                 JMP PathOfEx.0072A8E5
	$-454    0072A9D1     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$-451    0072A9D4     8BB7 C4000000               MOV ESI,DWORD PTR DS:[EDI+0xC4]
	$-44B    0072A9DA     8BBF C8000000               MOV EDI,DWORD PTR DS:[EDI+0xC8]
	$-445    0072A9E0     3BF7                        CMP ESI,EDI
	$-443    0072A9E2     74 48                       JE SHORT PathOfEx.0072AA2C
	$-441    0072A9E4     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$-43C    0072A9E9     8BD6                        MOV EDX,ESI
	$-43A    0072A9EB     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-437    0072A9EE     E8 CD49D9FF                 CALL PathOfEx.004BF3C0
	$-432    0072A9F3     83C4 04                     ADD ESP,0x4
	$-42F    0072A9F6     6A FF                       PUSH -0x1
	$-42D    0072A9F8     6A 00                       PUSH 0x0
	$-42B    0072A9FA     50                          PUSH EAX
	$-42A    0072A9FB     8BCB                        MOV ECX,EBX
	$-428    0072A9FD     C745 FC 10000000            MOV DWORD PTR SS:[EBP-0x4],0x10
	$-421    0072AA04     E8 77EFD4FF                 CALL PathOfEx.00479980
	$-41C    0072AA09     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-415    0072AA10     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-412    0072AA13     83F8 08                     CMP EAX,0x8
	$-40F    0072AA16     72 0F                       JB SHORT PathOfEx.0072AA27
	$-40D    0072AA18     40                          INC EAX
	$-40C    0072AA19     6A 02                       PUSH 0x2
	$-40A    0072AA1B     50                          PUSH EAX
	$-409    0072AA1C     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$-406    0072AA1F     E8 0C1ED4FF                 CALL PathOfEx.0046C830
	$-401    0072AA24     83C4 0C                     ADD ESP,0xC
	$-3FE    0072AA27     83C6 18                     ADD ESI,0x18
	$-3FB    0072AA2A   ^ EB B4                       JMP SHORT PathOfEx.0072A9E0
	$-3F9    0072AA2C     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$-3F6    0072AA2F     8B47 40                     MOV EAX,DWORD PTR DS:[EDI+0x40]
	$-3F3    0072AA32     3B47 44                     CMP EAX,DWORD PTR DS:[EDI+0x44]
	$-3F0    0072AA35     0F84 C6010000               JE PathOfEx.0072AC01
	$-3EA    0072AA3B     6A 0A                       PUSH 0xA
	$-3E8    0072AA3D     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$-3E3    0072AA42     8BCB                        MOV ECX,EBX
	$-3E1    0072AA44     E8 C726D5FF                 CALL PathOfEx.0047D110
	$-3DC    0072AA49     BA 35030000                 MOV EDX,0x335
	$-3D7    0072AA4E     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-3D4    0072AA51     E8 2A4CFFFF                 CALL PathOfEx.0071F680
	$-3CF    0072AA56     68 CCADE700                 PUSH PathOfEx.00E7ADCC                                ; :
	$-3CA    0072AA5B     8BD0                        MOV EDX,EAX
	$-3C8    0072AA5D     C745 FC 11000000            MOV DWORD PTR SS:[EBP-0x4],0x11
	$-3C1    0072AA64     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-3BE    0072AA67     E8 B427D5FF                 CALL PathOfEx.0047D220
	$-3B9    0072AA6C     83C4 04                     ADD ESP,0x4
	$-3B6    0072AA6F     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$-3B1    0072AA74     8BD0                        MOV EDX,EAX
	$-3AF    0072AA76     C645 FC 12                  MOV BYTE PTR SS:[EBP-0x4],0x12
	$-3AB    0072AA7A     8D4D 98                     LEA ECX,DWORD PTR SS:[EBP-0x68]
	$-3A8    0072AA7D     E8 9E27D5FF                 CALL PathOfEx.0047D220
	$-3A3    0072AA82     83C4 04                     ADD ESP,0x4
	$-3A0    0072AA85     6A FF                       PUSH -0x1
	$-39E    0072AA87     6A 00                       PUSH 0x0
	$-39C    0072AA89     50                          PUSH EAX
	$-39B    0072AA8A     8BCB                        MOV ECX,EBX
	$-399    0072AA8C     C645 FC 13                  MOV BYTE PTR SS:[EBP-0x4],0x13
	$-395    0072AA90     E8 EBEED4FF                 CALL PathOfEx.00479980
	$-390    0072AA95     8B45 AC                     MOV EAX,DWORD PTR SS:[EBP-0x54]
	$-38D    0072AA98     83F8 08                     CMP EAX,0x8
	$-38A    0072AA9B     72 0F                       JB SHORT PathOfEx.0072AAAC
	$-388    0072AA9D     40                          INC EAX
	$-387    0072AA9E     6A 02                       PUSH 0x2
	$-385    0072AAA0     50                          PUSH EAX
	$-384    0072AAA1     FF75 98                     PUSH DWORD PTR SS:[EBP-0x68]
	$-381    0072AAA4     E8 871DD4FF                 CALL PathOfEx.0046C830
	$-37C    0072AAA9     83C4 0C                     ADD ESP,0xC
	$-379    0072AAAC     33C0                        XOR EAX,EAX
	$-377    0072AAAE     C745 AC 07000000            MOV DWORD PTR SS:[EBP-0x54],0x7
	$-370    0072AAB5     66:8945 98                  MOV WORD PTR SS:[EBP-0x68],AX
	$-36C    0072AAB9     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-369    0072AABC     C745 A8 00000000            MOV DWORD PTR SS:[EBP-0x58],0x0
	$-362    0072AAC3     83F8 08                     CMP EAX,0x8
	$-35F    0072AAC6     72 0F                       JB SHORT PathOfEx.0072AAD7
	$-35D    0072AAC8     40                          INC EAX
	$-35C    0072AAC9     6A 02                       PUSH 0x2
	$-35A    0072AACB     50                          PUSH EAX
	$-359    0072AACC     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$-356    0072AACF     E8 5C1DD4FF                 CALL PathOfEx.0046C830
	$-351    0072AAD4     83C4 0C                     ADD ESP,0xC
	$-34E    0072AAD7     33C0                        XOR EAX,EAX
	$-34C    0072AAD9     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-345    0072AAE0     66:8945 BC                  MOV WORD PTR SS:[EBP-0x44],AX
	$-341    0072AAE4     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-33E    0072AAE7     C745 D0 07000000            MOV DWORD PTR SS:[EBP-0x30],0x7
	$-337    0072AAEE     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34],0x0
	$-330    0072AAF5     83F8 08                     CMP EAX,0x8
	$-32D    0072AAF8     72 0F                       JB SHORT PathOfEx.0072AB09
	$-32B    0072AAFA     40                          INC EAX
	$-32A    0072AAFB     6A 02                       PUSH 0x2
	$-328    0072AAFD     50                          PUSH EAX
	$-327    0072AAFE     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$-324    0072AB01     E8 2A1DD4FF                 CALL PathOfEx.0046C830
	$-31F    0072AB06     83C4 0C                     ADD ESP,0xC
	$-31C    0072AB09     8B77 40                     MOV ESI,DWORD PTR DS:[EDI+0x40]
	$-319    0072AB0C     8B7F 44                     MOV EDI,DWORD PTR DS:[EDI+0x44]
	$-316    0072AB0F     8D46 08                     LEA EAX,DWORD PTR DS:[ESI+0x8]
	$-313    0072AB12     8945 F0                     MOV DWORD PTR SS:[EBP-0x10],EAX
	$-310    0072AB15     3BF7                        CMP ESI,EDI
	$-30E    0072AB17     0F84 E1000000               JE PathOfEx.0072ABFE
	$-308    0072AB1D     833E 03                     CMP DWORD PTR DS:[ESI],0x3
	$-305    0072AB20     75 58                       JNZ SHORT PathOfEx.0072AB7A
	$-303    0072AB22     83EC 18                     SUB ESP,0x18
	$-300    0072AB25     8BCC                        MOV ECX,ESP
	$-2FE    0072AB27     C741 10 00000000            MOV DWORD PTR DS:[ECX+0x10],0x0
	$-2F7    0072AB2E     C741 14 00000000            MOV DWORD PTR DS:[ECX+0x14],0x0
	$-2F0    0072AB35     C741 14 07000000            MOV DWORD PTR DS:[ECX+0x14],0x7
	$-2E9    0072AB3C     8379 14 08                  CMP DWORD PTR DS:[ECX+0x14],0x8
	$-2E5    0072AB40     C741 10 00000000            MOV DWORD PTR DS:[ECX+0x10],0x0
	$-2DE    0072AB47     72 04                       JB SHORT PathOfEx.0072AB4D
	$-2DC    0072AB49     8B11                        MOV EDX,DWORD PTR DS:[ECX]
	$-2DA    0072AB4B     EB 02                       JMP SHORT PathOfEx.0072AB4F
	$-2D8    0072AB4D     8BD1                        MOV EDX,ECX
	$-2D6    0072AB4F     33C0                        XOR EAX,EAX
	$-2D4    0072AB51     6A FF                       PUSH -0x1
	$-2D2    0072AB53     50                          PUSH EAX
	$-2D1    0072AB54     66:8902                     MOV WORD PTR DS:[EDX],AX
	$-2CE    0072AB57     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-2CB    0072AB5A     50                          PUSH EAX
	$-2CA    0072AB5B     E8 202DD4FF                 CALL PathOfEx.0046D880
	$-2C5    0072AB60     8B55 F0                     MOV EDX,DWORD PTR SS:[EBP-0x10]
	$-2C2    0072AB63     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-2BF    0072AB66     8D52 18                     LEA EDX,DWORD PTR DS:[EDX+0x18]
	$-2BC    0072AB69     E8 B288FFFF                 CALL PathOfEx.00723420
	$-2B7    0072AB6E     83C4 18                     ADD ESP,0x18
	$-2B4    0072AB71     C745 FC 14000000            MOV DWORD PTR SS:[EBP-0x4],0x14
	$-2AD    0072AB78     EB 3E                       JMP SHORT PathOfEx.0072ABB8
	$-2AB    0072AB7A     6A FF                       PUSH -0x1
	$-2A9    0072AB7C     6A 00                       PUSH 0x0
	$-2A7    0072AB7E     50                          PUSH EAX
	$-2A6    0072AB7F     8BCB                        MOV ECX,EBX
	$-2A4    0072AB81     E8 FAEDD4FF                 CALL PathOfEx.00479980
	$-29F    0072AB86     8B4D F0                     MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-29C    0072AB89     8379 10 00                  CMP DWORD PTR DS:[ECX+0x10],0x0
	$-298    0072AB8D     74 17                       JE SHORT PathOfEx.0072ABA6
	$-296    0072AB8F     8B41 18                     MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-293    0072AB92     3B41 1C                     CMP EAX,DWORD PTR DS:[ECX+0x1C]
	$-290    0072AB95     74 0F                       JE SHORT PathOfEx.0072ABA6
	$-28E    0072AB97     68 B8B1E700                 PUSH PathOfEx.00E7B1B8                                ; :
	$-289    0072AB9C     8BCB                        MOV ECX,EBX
	$-287    0072AB9E     E8 FDDBD5FF                 CALL PathOfEx.004887A0
	$-282    0072ABA3     8B4D F0                     MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-27F    0072ABA6     8D51 18                     LEA EDX,DWORD PTR DS:[ECX+0x18]
	$-27C    0072ABA9     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-279    0072ABAC     E8 5F87FFFF                 CALL PathOfEx.00723310
	$-274    0072ABB1     C745 FC 15000000            MOV DWORD PTR SS:[EBP-0x4],0x15
	$-26D    0072ABB8     6A FF                       PUSH -0x1
	$-26B    0072ABBA     6A 00                       PUSH 0x0
	$-269    0072ABBC     50                          PUSH EAX
	$-268    0072ABBD     8BCB                        MOV ECX,EBX
	$-266    0072ABBF     E8 BCEDD4FF                 CALL PathOfEx.00479980
	$-261    0072ABC4     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-25A    0072ABCB     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-257    0072ABCE     83F8 08                     CMP EAX,0x8
	$-254    0072ABD1     72 0F                       JB SHORT PathOfEx.0072ABE2
	$-252    0072ABD3     6A 02                       PUSH 0x2
	$-250    0072ABD5     40                          INC EAX
	$-24F    0072ABD6     50                          PUSH EAX
	$-24E    0072ABD7     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$-24B    0072ABDA     E8 511CD4FF                 CALL PathOfEx.0046C830
	$-246    0072ABDF     83C4 0C                     ADD ESP,0xC
	$-243    0072ABE2     6A 02                       PUSH 0x2
	$-241    0072ABE4     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$-23C    0072ABE9     8BCB                        MOV ECX,EBX
	$-23A    0072ABEB     E8 2025D5FF                 CALL PathOfEx.0047D110
	$-235    0072ABF0     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-232    0072ABF3     83C6 48                     ADD ESI,0x48
	$-22F    0072ABF6     83C0 48                     ADD EAX,0x48
	$-22C    0072ABF9   ^ E9 14FFFFFF                 JMP PathOfEx.0072AB12
	$-227    0072ABFE     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$-224    0072AC01     83BF E0010000 00            CMP DWORD PTR DS:[EDI+0x1E0],0x0
	$-21D    0072AC08     0F84 FC000000               JE PathOfEx.0072AD0A
	$-217    0072AC0E     6A 0A                       PUSH 0xA
	$-215    0072AC10     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$-210    0072AC15     8BCB                        MOV ECX,EBX
	$-20E    0072AC17     C745 B4 58524742            MOV DWORD PTR SS:[EBP-0x4C],0x42475258
	$-207    0072AC1E     C645 B8 57                  MOV BYTE PTR SS:[EBP-0x48],0x57
	$-203    0072AC22     E8 E924D5FF                 CALL PathOfEx.0047D110
	$-1FE    0072AC27     BA B0070000                 MOV EDX,0x7B0
	$-1F9    0072AC2C     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-1F6    0072AC2F     E8 4C4AFFFF                 CALL PathOfEx.0071F680
	$-1F1    0072AC34     68 B8B1E700                 PUSH PathOfEx.00E7B1B8                                ; :
	$-1EC    0072AC39     8BD0                        MOV EDX,EAX
	$-1EA    0072AC3B     C745 FC 16000000            MOV DWORD PTR SS:[EBP-0x4],0x16
	$-1E3    0072AC42     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-1E0    0072AC45     E8 D625D5FF                 CALL PathOfEx.0047D220
	$-1DB    0072AC4A     83C4 04                     ADD ESP,0x4
	$-1D8    0072AC4D     6A FF                       PUSH -0x1
	$-1D6    0072AC4F     6A 00                       PUSH 0x0
	$-1D4    0072AC51     50                          PUSH EAX
	$-1D3    0072AC52     8BCB                        MOV ECX,EBX
	$-1D1    0072AC54     C645 FC 17                  MOV BYTE PTR SS:[EBP-0x4],0x17
	$-1CD    0072AC58     E8 23EDD4FF                 CALL PathOfEx.00479980
	$-1C8    0072AC5D     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-1C5    0072AC60     83F8 08                     CMP EAX,0x8
	$-1C2    0072AC63     72 0F                       JB SHORT PathOfEx.0072AC74
	$-1C0    0072AC65     40                          INC EAX
	$-1BF    0072AC66     6A 02                       PUSH 0x2
	$-1BD    0072AC68     50                          PUSH EAX
	$-1BC    0072AC69     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$-1B9    0072AC6C     E8 BF1BD4FF                 CALL PathOfEx.0046C830
	$-1B4    0072AC71     83C4 0C                     ADD ESP,0xC
	$-1B1    0072AC74     33C0                        XOR EAX,EAX
	$-1AF    0072AC76     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-1A8    0072AC7D     66:8945 BC                  MOV WORD PTR SS:[EBP-0x44],AX
	$-1A4    0072AC81     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-1A1    0072AC84     C745 D0 07000000            MOV DWORD PTR SS:[EBP-0x30],0x7
	$-19A    0072AC8B     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34],0x0
	$-193    0072AC92     83F8 08                     CMP EAX,0x8
	$-190    0072AC95     72 0F                       JB SHORT PathOfEx.0072ACA6
	$-18E    0072AC97     40                          INC EAX
	$-18D    0072AC98     6A 02                       PUSH 0x2
	$-18B    0072AC9A     50                          PUSH EAX
	$-18A    0072AC9B     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$-187    0072AC9E     E8 8D1BD4FF                 CALL PathOfEx.0046C830
	$-182    0072ACA3     83C4 0C                     ADD ESP,0xC
	$-17F    0072ACA6     83BF E0010000 00            CMP DWORD PTR DS:[EDI+0x1E0],0x0
	$-178    0072ACAD     C745 F0 00000000            MOV DWORD PTR SS:[EBP-0x10],0x0
	$-171    0072ACB4     76 46                       JBE SHORT PathOfEx.0072ACFC
	$-16F    0072ACB6     8DB7 B4010000               LEA ESI,DWORD PTR DS:[EDI+0x1B4]
	$-169    0072ACBC     0F1F40 00                   NOP DWORD PTR DS:[EAX]
	$-165    0072ACC0     8B46 FC                     MOV EAX,DWORD PTR DS:[ESI-0x4]
	$-162    0072ACC3     8BCB                        MOV ECX,EBX
	$-160    0072ACC5     66:0FBE4405 B4              MOVSX AX,BYTE PTR SS:[EBP+EAX-0x4C]
	$-15A    0072ACCB     0FB7C0                      MOVZX EAX,AX
	$-157    0072ACCE     50                          PUSH EAX
	$-156    0072ACCF     6A 01                       PUSH 0x1
	$-154    0072ACD1     E8 7AADD4FF                 CALL PathOfEx.00475A50
	$-14F    0072ACD6     803E 00                     CMP BYTE PTR DS:[ESI],0x0
	$-14C    0072ACD9     8BCB                        MOV ECX,EBX
	$-14A    0072ACDB     74 04                       JE SHORT PathOfEx.0072ACE1
	$-148    0072ACDD     6A 2D                       PUSH 0x2D
	$-146    0072ACDF     EB 02                       JMP SHORT PathOfEx.0072ACE3
	$-144    0072ACE1     6A 20                       PUSH 0x20
	$-142    0072ACE3     6A 01                       PUSH 0x1
	$-140    0072ACE5     E8 66ADD4FF                 CALL PathOfEx.00475A50
	$-13B    0072ACEA     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-138    0072ACED     83C6 08                     ADD ESI,0x8
	$-135    0072ACF0     40                          INC EAX
	$-134    0072ACF1     8945 F0                     MOV DWORD PTR SS:[EBP-0x10],EAX
	$-131    0072ACF4     3B87 E0010000               CMP EAX,DWORD PTR DS:[EDI+0x1E0]
	$-12B    0072ACFA   ^ 72 C4                       JB SHORT PathOfEx.0072ACC0
	$-129    0072ACFC     6A 02                       PUSH 0x2
	$-127    0072ACFE     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$-122    0072AD03     8BCB                        MOV ECX,EBX
	$-120    0072AD05     E8 0624D5FF                 CALL PathOfEx.0047D110
	$-11B    0072AD0A     83BF E4010000 00            CMP DWORD PTR DS:[EDI+0x1E4],0x0
	$-114    0072AD11     0F84 95010000               JE PathOfEx.0072AEAC
	$-10E    0072AD17     6A 0A                       PUSH 0xA
	$-10C    0072AD19     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$-107    0072AD1E     8BCB                        MOV ECX,EBX
	$-105    0072AD20     E8 EB23D5FF                 CALL PathOfEx.0047D110
	$-100    0072AD25     BA B1070000                 MOV EDX,0x7B1
	$-FB     0072AD2A     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-F8     0072AD2D     E8 4E49FFFF                 CALL PathOfEx.0071F680
	$-F3     0072AD32     68 B8B1E700                 PUSH PathOfEx.00E7B1B8                                ; :
	$-EE     0072AD37     8BD0                        MOV EDX,EAX
	$-EC     0072AD39     C745 FC 18000000            MOV DWORD PTR SS:[EBP-0x4],0x18
	$-E5     0072AD40     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-E2     0072AD43     E8 D824D5FF                 CALL PathOfEx.0047D220
	$-DD     0072AD48     83C4 04                     ADD ESP,0x4
	$-DA     0072AD4B     6A FF                       PUSH -0x1
	$-D8     0072AD4D     6A 00                       PUSH 0x0
	$-D6     0072AD4F     50                          PUSH EAX
	$-D5     0072AD50     8BCB                        MOV ECX,EBX
	$-D3     0072AD52     C645 FC 19                  MOV BYTE PTR SS:[EBP-0x4],0x19
	$-CF     0072AD56     E8 25ECD4FF                 CALL PathOfEx.00479980
	$-CA     0072AD5B     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-C7     0072AD5E     83F8 08                     CMP EAX,0x8
	$-C4     0072AD61     72 0F                       JB SHORT PathOfEx.0072AD72
	$-C2     0072AD63     40                          INC EAX
	$-C1     0072AD64     6A 02                       PUSH 0x2
	$-BF     0072AD66     50                          PUSH EAX
	$-BE     0072AD67     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$-BB     0072AD6A     E8 C11AD4FF                 CALL PathOfEx.0046C830
	$-B6     0072AD6F     83C4 0C                     ADD ESP,0xC
	$-B3     0072AD72     33C0                        XOR EAX,EAX
	$-B1     0072AD74     C745 D0 07000000            MOV DWORD PTR SS:[EBP-0x30],0x7
	$-AA     0072AD7B     66:8945 BC                  MOV WORD PTR SS:[EBP-0x44],AX
	$-A6     0072AD7F     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-A3     0072AD82     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34],0x0
	$-9C     0072AD89     83F8 08                     CMP EAX,0x8
	$-99     0072AD8C     72 0F                       JB SHORT PathOfEx.0072AD9D
	$-97     0072AD8E     40                          INC EAX
	$-96     0072AD8F     6A 02                       PUSH 0x2
	$-94     0072AD91     50                          PUSH EAX
	$-93     0072AD92     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$-90     0072AD95     E8 961AD4FF                 CALL PathOfEx.0046C830
	$-8B     0072AD9A     83C4 0C                     ADD ESP,0xC
	$-88     0072AD9D     C785 E8FEFFFF F41AEB00      MOV DWORD PTR SS:[EBP-0x118],PathOfEx.00EB1AF4
	$-7E     0072ADA7     C785 F8FEFFFF A81AEB00      MOV DWORD PTR SS:[EBP-0x108],PathOfEx.00EB1AA8
	$-74     0072ADB1     C785 50FFFFFF EC84DE00      MOV DWORD PTR SS:[EBP-0xB0],PathOfEx.00DE84EC
	$-6A     0072ADBB     51                          PUSH ECX
	$-69     0072ADBC     8D85 00FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0x100]
	$-63     0072ADC2     C745 FC 1A000000            MOV DWORD PTR SS:[EBP-0x4],0x1A
	$-5C     0072ADC9     BE 01000000                 MOV ESI,0x1
	$-57     0072ADCE     8D8D E8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x118]
	$-51     0072ADD4     50                          PUSH EAX
	$-50     0072ADD5     8975 B0                     MOV DWORD PTR SS:[EBP-0x50],ESI
	$-4D     0072ADD8     E8 D3D6D4FF                 CALL PathOfEx.004784B0
	$-48     0072ADDD     C745 FC 1B000000            MOV DWORD PTR SS:[EBP-0x4],0x1B
	$-41     0072ADE4     8B85 E8FEFFFF               MOV EAX,DWORD PTR SS:[EBP-0x118]
	$-3B     0072ADEA     6A 03                       PUSH 0x3
	$-39     0072ADEC     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-36     0072ADEF     C78405 E8FEFFFF 6C2CE700    MOV DWORD PTR SS:[EBP+EAX-0x118],PathOfEx.00E72C6C
	$-2B     0072ADFA     8B85 E8FEFFFF               MOV EAX,DWORD PTR SS:[EBP-0x118]
	$-25     0072AE00     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-22     0072AE03     8D41 98                     LEA EAX,DWORD PTR DS:[ECX-0x68]
	$-1F     0072AE06     89840D E4FEFFFF             MOV DWORD PTR SS:[EBP+ECX-0x11C],EAX
	$-18     0072AE0D     8D8D 00FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0x100]
	$-12     0072AE13     E8 78DCD4FF                 CALL PathOfEx.00478A90
	$-D      0072AE18     C745 FC 1C000000            MOV DWORD PTR SS:[EBP-0x4],0x1C
	$-6      0072AE1F     8D8D F8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x108]                      ; edi=stCD_ItemTipsParams*
	$ ==>    0072AE25     FFB7 E4010000               PUSH DWORD PTR DS:[EDI+0x1E4]                         ; 物品等级,g_o_item_tips_param_item_lvl_
	$+6      0072AE2B     E8 805CD5FF                 CALL PathOfEx.00480AB0
	$+B      0072AE30     8D45 D8                     LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+E      0072AE33     50                          PUSH EAX
	$+F      0072AE34     8D8D E8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x118]
	$+15     0072AE3A     E8 B1D5D5FF                 CALL PathOfEx.004883F0
	$+1A     0072AE3F     6A FF                       PUSH -0x1
	$+1C     0072AE41     6A 00                       PUSH 0x0
	$+1E     0072AE43     50                          PUSH EAX
	$+1F     0072AE44     8BCB                        MOV ECX,EBX
	$+21     0072AE46     C645 FC 1D                  MOV BYTE PTR SS:[EBP-0x4],0x1D
	$+25     0072AE4A     E8 31EBD4FF                 CALL PathOfEx.00479980
	$+2A     0072AE4F     C645 FC 1C                  MOV BYTE PTR SS:[EBP-0x4],0x1C
	$+2E     0072AE53     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+31     0072AE56     83F8 08                     CMP EAX,0x8
	$+34     0072AE59     72 0F                       JB SHORT PathOfEx.0072AE6A
	$+36     0072AE5B     40                          INC EAX
	$+37     0072AE5C     6A 02                       PUSH 0x2
	$+39     0072AE5E     50                          PUSH EAX
	$+3A     0072AE5F     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+3D     0072AE62     E8 C919D4FF                 CALL PathOfEx.0046C830
	$+42     0072AE67     83C4 0C                     ADD ESP,0xC
	$+45     0072AE6A     6A 02                       PUSH 0x2
	$+47     0072AE6C     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+4C     0072AE71     8BCB                        MOV ECX,EBX
	$+4E     0072AE73     E8 9822D5FF                 CALL PathOfEx.0047D110
	$+53     0072AE78     8D8D 50FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xB0]
	$+59     0072AE7E     E8 EDBED4FF                 CALL PathOfEx.00476D70
	$+5E     0072AE83     8D85 50FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0xB0]
	$+64     0072AE89     C745 FC 1E000000            MOV DWORD PTR SS:[EBP-0x4],0x1E
	$+6B     0072AE90     50                          PUSH EAX
	$+6C     0072AE91     C785 50FFFFFF 6484DE00      MOV DWORD PTR SS:[EBP-0xB0],PathOfEx.00DE8464
	$+76     0072AE9B     E8 64195400                 CALL PathOfEx.00C6C804
	$+7B     0072AEA0     83C4 04                     ADD ESP,0x4
	$+7E     0072AEA3     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+85     0072AEAA     EB 03                       JMP SHORT PathOfEx.0072AEAF
	$+87     0072AEAC     8B75 B0                     MOV ESI,DWORD PTR SS:[EBP-0x50]
	$+8A     0072AEAF     83BF E8010000 00            CMP DWORD PTR DS:[EDI+0x1E8],0x0
	$+91     0072AEB6     0F84 91010000               JE PathOfEx.0072B04D
	$+97     0072AEBC     6A 0A                       PUSH 0xA
	$+99     0072AEBE     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+9E     0072AEC3     8BCB                        MOV ECX,EBX
	$+A0     0072AEC5     E8 4622D5FF                 CALL PathOfEx.0047D110
	$+A5     0072AECA     BA B2070000                 MOV EDX,0x7B2
	$+AA     0072AECF     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+AD     0072AED2     E8 A947FFFF                 CALL PathOfEx.0071F680
	$+B2     0072AED7     68 B8B1E700                 PUSH PathOfEx.00E7B1B8                                ; :
	$+B7     0072AEDC     8BD0                        MOV EDX,EAX
	$+B9     0072AEDE     C745 FC 1F000000            MOV DWORD PTR SS:[EBP-0x4],0x1F
	$+C0     0072AEE5     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+C3     0072AEE8     E8 3323D5FF                 CALL PathOfEx.0047D220
	$+C8     0072AEED     83C4 04                     ADD ESP,0x4
	$+CB     0072AEF0     6A FF                       PUSH -0x1
	$+CD     0072AEF2     6A 00                       PUSH 0x0
	$+CF     0072AEF4     50                          PUSH EAX
	$+D0     0072AEF5     8BCB                        MOV ECX,EBX
	$+D2     0072AEF7     C645 FC 20                  MOV BYTE PTR SS:[EBP-0x4],0x20
	$+D6     0072AEFB     E8 80EAD4FF                 CALL PathOfEx.00479980
	$+DB     0072AF00     8B45 D0                     MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+DE     0072AF03     83F8 08                     CMP EAX,0x8
	$+E1     0072AF06     72 0F                       JB SHORT PathOfEx.0072AF17
	$+E3     0072AF08     40                          INC EAX
	$+E4     0072AF09     6A 02                       PUSH 0x2
	$+E6     0072AF0B     50                          PUSH EAX
	$+E7     0072AF0C     FF75 BC                     PUSH DWORD PTR SS:[EBP-0x44]
	$+EA     0072AF0F     E8 1C19D4FF                 CALL PathOfEx.0046C830
	$+EF     0072AF14     83C4 0C                     ADD ESP,0xC
	$+F2     0072AF17     33C0                        XOR EAX,EAX
	$+F4     0072AF19     C745 D0 07000000            MOV DWORD PTR SS:[EBP-0x30],0x7
	$+FB     0072AF20     66:8945 BC                  MOV WORD PTR SS:[EBP-0x44],AX
	$+FF     0072AF24     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+102    0072AF27     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34],0x0
	$+109    0072AF2E     83F8 08                     CMP EAX,0x8
	$+10C    0072AF31     72 0F                       JB SHORT PathOfEx.0072AF42
	$+10E    0072AF33     40                          INC EAX
	$+10F    0072AF34     6A 02                       PUSH 0x2
	$+111    0072AF36     50                          PUSH EAX
	$+112    0072AF37     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+115    0072AF3A     E8 F118D4FF                 CALL PathOfEx.0046C830
	$+11A    0072AF3F     83C4 0C                     ADD ESP,0xC
	$+11D    0072AF42     C785 E8FEFFFF F41AEB00      MOV DWORD PTR SS:[EBP-0x118],PathOfEx.00EB1AF4
	$+127    0072AF4C     C785 F8FEFFFF A81AEB00      MOV DWORD PTR SS:[EBP-0x108],PathOfEx.00EB1AA8
	$+131    0072AF56     C785 50FFFFFF EC84DE00      MOV DWORD PTR SS:[EBP-0xB0],PathOfEx.00DE84EC
	$+13B    0072AF60     51                          PUSH ECX
	$+13C    0072AF61     8D85 00FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0x100]
	$+142    0072AF67     C745 FC 21000000            MOV DWORD PTR SS:[EBP-0x4],0x21
	$+149    0072AF6E     83CE 02                     OR ESI,0x2
	$+14C    0072AF71     8D8D E8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x118]
	$+152    0072AF77     50                          PUSH EAX
	$+153    0072AF78     8975 B0                     MOV DWORD PTR SS:[EBP-0x50],ESI
	$+156    0072AF7B     E8 30D5D4FF                 CALL PathOfEx.004784B0
	$+15B    0072AF80     C745 FC 22000000            MOV DWORD PTR SS:[EBP-0x4],0x22
	$+162    0072AF87     8B85 E8FEFFFF               MOV EAX,DWORD PTR SS:[EBP-0x118]
	$+168    0072AF8D     6A 03                       PUSH 0x3
	$+16A    0072AF8F     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+16D    0072AF92     C78405 E8FEFFFF 6C2CE700    MOV DWORD PTR SS:[EBP+EAX-0x118],PathOfEx.00E72C6C
	$+178    0072AF9D     8B85 E8FEFFFF               MOV EAX,DWORD PTR SS:[EBP-0x118]
	$+17E    0072AFA3     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+181    0072AFA6     8D41 98                     LEA EAX,DWORD PTR DS:[ECX-0x68]
	$+184    0072AFA9     89840D E4FEFFFF             MOV DWORD PTR SS:[EBP+ECX-0x11C],EAX
	$+18B    0072AFB0     8D8D 00FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0x100]
	$+191    0072AFB6     E8 D5DAD4FF                 CALL PathOfEx.00478A90
	$+196    0072AFBB     C745 FC 23000000            MOV DWORD PTR SS:[EBP-0x4],0x23
	$+19D    0072AFC2     8D8D F8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x108]
	$+1A3    0072AFC8     FFB7 E8010000               PUSH DWORD PTR DS:[EDI+0x1E8]
	$+1A9    0072AFCE     E8 DD5AD5FF                 CALL PathOfEx.00480AB0
	$+1AE    0072AFD3     8D45 D8                     LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+1B1    0072AFD6     50                          PUSH EAX
	$+1B2    0072AFD7     8D8D E8FEFFFF               LEA ECX,DWORD PTR SS:[EBP-0x118]
	$+1B8    0072AFDD     E8 0ED4D5FF                 CALL PathOfEx.004883F0
	$+1BD    0072AFE2     6A FF                       PUSH -0x1
	$+1BF    0072AFE4     6A 00                       PUSH 0x0
	$+1C1    0072AFE6     50                          PUSH EAX
	$+1C2    0072AFE7     8BCB                        MOV ECX,EBX
	$+1C4    0072AFE9     C645 FC 24                  MOV BYTE PTR SS:[EBP-0x4],0x24
	$+1C8    0072AFED     E8 8EE9D4FF                 CALL PathOfEx.00479980
	$+1CD    0072AFF2     C645 FC 23                  MOV BYTE PTR SS:[EBP-0x4],0x23
	$+1D1    0072AFF6     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+1D4    0072AFF9     83F8 08                     CMP EAX,0x8
	$+1D7    0072AFFC     72 0F                       JB SHORT PathOfEx.0072B00D
	$+1D9    0072AFFE     40                          INC EAX
	$+1DA    0072AFFF     6A 02                       PUSH 0x2
	$+1DC    0072B001     50                          PUSH EAX
	$+1DD    0072B002     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+1E0    0072B005     E8 2618D4FF                 CALL PathOfEx.0046C830
	$+1E5    0072B00A     83C4 0C                     ADD ESP,0xC
	$+1E8    0072B00D     6A 02                       PUSH 0x2
	$+1EA    0072B00F     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+1EF    0072B014     8BCB                        MOV ECX,EBX
	$+1F1    0072B016     E8 F520D5FF                 CALL PathOfEx.0047D110
	$+1F6    0072B01B     8D8D 50FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xB0]
	$+1FC    0072B021     E8 4ABDD4FF                 CALL PathOfEx.00476D70
	$+201    0072B026     8D85 50FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0xB0]
	$+207    0072B02C     C745 FC 25000000            MOV DWORD PTR SS:[EBP-0x4],0x25
	$+20E    0072B033     50                          PUSH EAX
	$+20F    0072B034     C785 50FFFFFF 6484DE00      MOV DWORD PTR SS:[EBP-0xB0],PathOfEx.00DE8464
	$+219    0072B03E     E8 C1175400                 CALL PathOfEx.00C6C804
	$+21E    0072B043     83C4 04                     ADD ESP,0x4
	$+221    0072B046     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+228    0072B04D     83BF 7C010000 00            CMP DWORD PTR DS:[EDI+0x17C],0x0
	$+22F    0072B054     74 55                       JE SHORT PathOfEx.0072B0AB
	$+231    0072B056     6A 0A                       PUSH 0xA
	$+233    0072B058     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+238    0072B05D     8BCB                        MOV ECX,EBX
	$+23A    0072B05F     E8 AC20D5FF                 CALL PathOfEx.0047D110
	$+23F    0072B064     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+244    0072B069     8D97 6C010000               LEA EDX,DWORD PTR DS:[EDI+0x16C]
	$+24A    0072B06F     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+24D    0072B072     E8 4943D9FF                 CALL PathOfEx.004BF3C0
	$+252    0072B077     83C4 04                     ADD ESP,0x4
	$+255    0072B07A     6A FF                       PUSH -0x1
	$+257    0072B07C     6A 00                       PUSH 0x0
	$+259    0072B07E     50                          PUSH EAX
	$+25A    0072B07F     8BCB                        MOV ECX,EBX
	$+25C    0072B081     C745 FC 26000000            MOV DWORD PTR SS:[EBP-0x4],0x26
	$+263    0072B088     E8 F3E8D4FF                 CALL PathOfEx.00479980
	$+268    0072B08D     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+26F    0072B094     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+272    0072B097     83F8 08                     CMP EAX,0x8
	$+275    0072B09A     72 0F                       JB SHORT PathOfEx.0072B0AB
	$+277    0072B09C     40                          INC EAX
	$+278    0072B09D     6A 02                       PUSH 0x2
	$+27A    0072B09F     50                          PUSH EAX
	$+27B    0072B0A0     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+27E    0072B0A3     E8 8817D4FF                 CALL PathOfEx.0046C830
	$+283    0072B0A8     83C4 0C                     ADD ESP,0xC
	$+286    0072B0AB     8B47 64                     MOV EAX,DWORD PTR DS:[EDI+0x64]
	$+289    0072B0AE     3B47 68                     CMP EAX,DWORD PTR DS:[EDI+0x68]
	$+28C    0072B0B1     74 0E                       JE SHORT PathOfEx.0072B0C1
	$+28E    0072B0B3     6A 0A                       PUSH 0xA
	$+290    0072B0B5     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+295    0072B0BA     8BCB                        MOV ECX,EBX
	$+297    0072B0BC     E8 4F20D5FF                 CALL PathOfEx.0047D110
	$+29C    0072B0C1     8B77 64                     MOV ESI,DWORD PTR DS:[EDI+0x64]
	$+29F    0072B0C4     8B7F 68                     MOV EDI,DWORD PTR DS:[EDI+0x68]
	$+2A2    0072B0C7     3BF7                        CMP ESI,EDI
	$+2A4    0072B0C9     74 48                       JE SHORT PathOfEx.0072B113
	$+2A6    0072B0CB     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+2AB    0072B0D0     8BD6                        MOV EDX,ESI
	$+2AD    0072B0D2     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+2B0    0072B0D5     E8 E642D9FF                 CALL PathOfEx.004BF3C0
	$+2B5    0072B0DA     83C4 04                     ADD ESP,0x4
	$+2B8    0072B0DD     6A FF                       PUSH -0x1
	$+2BA    0072B0DF     6A 00                       PUSH 0x0
	$+2BC    0072B0E1     50                          PUSH EAX
	$+2BD    0072B0E2     8BCB                        MOV ECX,EBX
	$+2BF    0072B0E4     C745 FC 27000000            MOV DWORD PTR SS:[EBP-0x4],0x27
	$+2C6    0072B0EB     E8 90E8D4FF                 CALL PathOfEx.00479980
	$+2CB    0072B0F0     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2D2    0072B0F7     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+2D5    0072B0FA     83F8 08                     CMP EAX,0x8
	$+2D8    0072B0FD     72 0F                       JB SHORT PathOfEx.0072B10E
	$+2DA    0072B0FF     40                          INC EAX
	$+2DB    0072B100     6A 02                       PUSH 0x2
	$+2DD    0072B102     50                          PUSH EAX
	$+2DE    0072B103     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+2E1    0072B106     E8 2517D4FF                 CALL PathOfEx.0046C830
	$+2E6    0072B10B     83C4 0C                     ADD ESP,0xC
	$+2E9    0072B10E     83C6 18                     ADD ESI,0x18
	$+2EC    0072B111   ^ EB B4                       JMP SHORT PathOfEx.0072B0C7
	$+2EE    0072B113     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+2F1    0072B116     8B47 7C                     MOV EAX,DWORD PTR DS:[EDI+0x7C]
	$+2F4    0072B119     3B87 80000000               CMP EAX,DWORD PTR DS:[EDI+0x80]
	$+2FA    0072B11F     74 0E                       JE SHORT PathOfEx.0072B12F
	$+2FC    0072B121     6A 0A                       PUSH 0xA
	$+2FE    0072B123     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+303    0072B128     8BCB                        MOV ECX,EBX
	$+305    0072B12A     E8 E11FD5FF                 CALL PathOfEx.0047D110
	$+30A    0072B12F     8B77 7C                     MOV ESI,DWORD PTR DS:[EDI+0x7C]
	$+30D    0072B132     8BBF 80000000               MOV EDI,DWORD PTR DS:[EDI+0x80]
	$+313    0072B138     3BF7                        CMP ESI,EDI
	$+315    0072B13A     74 48                       JE SHORT PathOfEx.0072B184
	$+317    0072B13C     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+31C    0072B141     8BD6                        MOV EDX,ESI
	$+31E    0072B143     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+321    0072B146     E8 7542D9FF                 CALL PathOfEx.004BF3C0
	$+326    0072B14B     83C4 04                     ADD ESP,0x4
	$+329    0072B14E     6A FF                       PUSH -0x1
	$+32B    0072B150     6A 00                       PUSH 0x0
	$+32D    0072B152     50                          PUSH EAX
	$+32E    0072B153     8BCB                        MOV ECX,EBX
	$+330    0072B155     C745 FC 28000000            MOV DWORD PTR SS:[EBP-0x4],0x28
	$+337    0072B15C     E8 1FE8D4FF                 CALL PathOfEx.00479980
	$+33C    0072B161     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+343    0072B168     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+346    0072B16B     83F8 08                     CMP EAX,0x8
	$+349    0072B16E     72 0F                       JB SHORT PathOfEx.0072B17F
	$+34B    0072B170     40                          INC EAX
	$+34C    0072B171     6A 02                       PUSH 0x2
	$+34E    0072B173     50                          PUSH EAX
	$+34F    0072B174     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+352    0072B177     E8 B416D4FF                 CALL PathOfEx.0046C830
	$+357    0072B17C     83C4 0C                     ADD ESP,0xC
	$+35A    0072B17F     83C6 18                     ADD ESI,0x18
	$+35D    0072B182   ^ EB B4                       JMP SHORT PathOfEx.0072B138
	$+35F    0072B184     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+362    0072B187     80BF EC010000 00            CMP BYTE PTR DS:[EDI+0x1EC],0x0
	$+369    0072B18E     75 0E                       JNZ SHORT PathOfEx.0072B19E
	$+36B    0072B190     8B87 94000000               MOV EAX,DWORD PTR DS:[EDI+0x94]
	$+371    0072B196     3B87 98000000               CMP EAX,DWORD PTR DS:[EDI+0x98]
	$+377    0072B19C     74 0E                       JE SHORT PathOfEx.0072B1AC
	$+379    0072B19E     6A 0A                       PUSH 0xA
	$+37B    0072B1A0     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+380    0072B1A5     8BCB                        MOV ECX,EBX
	$+382    0072B1A7     E8 641FD5FF                 CALL PathOfEx.0047D110
	$+387    0072B1AC     80BF EC010000 00            CMP BYTE PTR DS:[EDI+0x1EC],0x0
	$+38E    0072B1B3     74 51                       JE SHORT PathOfEx.0072B206
	$+390    0072B1B5     BA 14010000                 MOV EDX,0x114
	$+395    0072B1BA     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+398    0072B1BD     E8 BE44FFFF                 CALL PathOfEx.0071F680
	$+39D    0072B1C2     6A FF                       PUSH -0x1
	$+39F    0072B1C4     6A 00                       PUSH 0x0
	$+3A1    0072B1C6     50                          PUSH EAX
	$+3A2    0072B1C7     8BCB                        MOV ECX,EBX
	$+3A4    0072B1C9     C745 FC 29000000            MOV DWORD PTR SS:[EBP-0x4],0x29
	$+3AB    0072B1D0     E8 ABE7D4FF                 CALL PathOfEx.00479980
	$+3B0    0072B1D5     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+3B7    0072B1DC     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+3BA    0072B1DF     83F8 08                     CMP EAX,0x8
	$+3BD    0072B1E2     72 0F                       JB SHORT PathOfEx.0072B1F3
	$+3BF    0072B1E4     40                          INC EAX
	$+3C0    0072B1E5     6A 02                       PUSH 0x2
	$+3C2    0072B1E7     50                          PUSH EAX
	$+3C3    0072B1E8     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+3C6    0072B1EB     E8 4016D4FF                 CALL PathOfEx.0046C830
	$+3CB    0072B1F0     83C4 0C                     ADD ESP,0xC
	$+3CE    0072B1F3     6A 02                       PUSH 0x2
	$+3D0    0072B1F5     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+3D5    0072B1FA     8BCB                        MOV ECX,EBX
	$+3D7    0072B1FC     E8 0F1FD5FF                 CALL PathOfEx.0047D110
	$+3DC    0072B201     E9 B9000000                 JMP PathOfEx.0072B2BF
	$+3E1    0072B206     8BB7 94000000               MOV ESI,DWORD PTR DS:[EDI+0x94]
	$+3E7    0072B20C     8BBF 98000000               MOV EDI,DWORD PTR DS:[EDI+0x98]
	$+3ED    0072B212     3BF7                        CMP ESI,EDI
	$+3EF    0072B214     74 48                       JE SHORT PathOfEx.0072B25E
	$+3F1    0072B216     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+3F6    0072B21B     8BD6                        MOV EDX,ESI
	$+3F8    0072B21D     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+3FB    0072B220     E8 9B41D9FF                 CALL PathOfEx.004BF3C0
	$+400    0072B225     83C4 04                     ADD ESP,0x4
	$+403    0072B228     6A FF                       PUSH -0x1
	$+405    0072B22A     6A 00                       PUSH 0x0
	$+407    0072B22C     50                          PUSH EAX
	$+408    0072B22D     8BCB                        MOV ECX,EBX
	$+40A    0072B22F     C745 FC 2A000000            MOV DWORD PTR SS:[EBP-0x4],0x2A
	$+411    0072B236     E8 45E7D4FF                 CALL PathOfEx.00479980
	$+416    0072B23B     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+41D    0072B242     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+420    0072B245     83F8 08                     CMP EAX,0x8
	$+423    0072B248     72 0F                       JB SHORT PathOfEx.0072B259
	$+425    0072B24A     40                          INC EAX
	$+426    0072B24B     6A 02                       PUSH 0x2
	$+428    0072B24D     50                          PUSH EAX
	$+429    0072B24E     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+42C    0072B251     E8 DA15D4FF                 CALL PathOfEx.0046C830
	$+431    0072B256     83C4 0C                     ADD ESP,0xC
	$+434    0072B259     83C6 18                     ADD ESI,0x18
	$+437    0072B25C   ^ EB B4                       JMP SHORT PathOfEx.0072B212
	$+439    0072B25E     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+43C    0072B261     8BB7 AC000000               MOV ESI,DWORD PTR DS:[EDI+0xAC]
	$+442    0072B267     8BBF B0000000               MOV EDI,DWORD PTR DS:[EDI+0xB0]
	$+448    0072B26D     0F1F00                      NOP DWORD PTR DS:[EAX]
	$+44B    0072B270     3BF7                        CMP ESI,EDI
	$+44D    0072B272     74 48                       JE SHORT PathOfEx.0072B2BC
	$+44F    0072B274     68 102AEB00                 PUSH PathOfEx.00EB2A10                                ; \r\n
	$+454    0072B279     8BD6                        MOV EDX,ESI
	$+456    0072B27B     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+459    0072B27E     E8 3D41D9FF                 CALL PathOfEx.004BF3C0
	$+45E    0072B283     83C4 04                     ADD ESP,0x4
	$+461    0072B286     6A FF                       PUSH -0x1
	$+463    0072B288     6A 00                       PUSH 0x0
	$+465    0072B28A     50                          PUSH EAX
	$+466    0072B28B     8BCB                        MOV ECX,EBX
	$+468    0072B28D     C745 FC 2B000000            MOV DWORD PTR SS:[EBP-0x4],0x2B
	$+46F    0072B294     E8 E7E6D4FF                 CALL PathOfEx.00479980
	$+474    0072B299     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+47B    0072B2A0     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+47E    0072B2A3     83F8 08                     CMP EAX,0x8
	$+481    0072B2A6     72 0F                       JB SHORT PathOfEx.0072B2B7
	$+483    0072B2A8     40                          INC EAX
	$+484    0072B2A9     6A 02                       PUSH 0x2
	$+486    0072B2AB     50                          PUSH EAX
	$+487    0072B2AC     FF75 D8                     PUSH DWORD PTR SS:[EBP-0x28]
	$+48A    0072B2AF     E8 7C15D4FF                 CALL PathOfEx.0046C830
	$+48F    0072B2B4     83C4 0C                     ADD ESP,0xC
	$+492    0072B2B7     83C6 18                     ADD ESI,0x18
	$+495    0072B2BA   ^ EB B4                       JMP SHORT PathOfEx.0072B270
	$+497    0072B2BC     8B7D D4                     MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+49A    0072B2BF     8B47 58                     MOV EAX,DWORD PTR DS:[EDI+0x58]
	$+49D    0072B2C2     3B47 5C                     CMP EAX,DWORD PTR DS:[EDI+0x5C]
	$+4A0    0072B2C5     0F84 A1010000               JE PathOfEx.0072B46C
	$+4A6    0072B2CB     6A 0A                       PUSH 0xA
	$+4A8    0072B2CD     68 182AEB00                 PUSH PathOfEx.00EB2A18                                ; --------\r\n
	$+4AD    0072B2D2     8BCB                        MOV ECX,EBX
	$+4AF    0072B2D4     E8 371ED5FF                 CALL PathOfEx.0047D110
	$+4B4    0072B2D9     8B47 58                     MOV EAX,DWORD PTR DS:[EDI+0x58]
	$+4B7    0072B2DC     3B47 5C                     CMP EAX,DWORD PTR DS:[EDI+0x5C]
	$+4BA    0072B2DF     0F84 87010000               JE PathOfEx.0072B46C
	$+4C0    0072B2E5     BA B3070000                 MOV EDX,0x7B3
	$+4C5    0072B2EA     8D4D D8                     LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+4C8    0072B2ED     E8 8E43FFFF                 CALL PathOfEx.0071F680
	$+4CD    0072B2F2     68 CCADE700                 PUSH PathOfEx.00E7ADCC                                ; :
	$+4D2    0072B2F7     8BD0                        MOV EDX,EAX
	$+4D4    0072B2F9     C745 FC 2C000000            MOV DWORD PTR SS:[EBP-0x4],0x2C
	$+4DB    0072B300     8D4D BC                     LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+4DE    0072B303     E8 181FD5FF                 CALL PathOfEx.0047D220
	$+4E3    0072B308     83C4 04                     ADD ESP,0x4
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("ff??????????e8????????8d????508d??????????e8????????6a??6a??50", 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x1a0 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_item_tips_param_item_lvl_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
