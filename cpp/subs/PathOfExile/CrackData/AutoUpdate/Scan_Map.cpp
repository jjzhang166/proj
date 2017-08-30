#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Map(Cmn_FC_OFstream& ofs)
{
	boost::optional<pt_dword> op_dw_tmp;
	pt_dword dw_tmp;
	pt_pcvoid addr_tmp;
	/*
	$-6C     006D5F50       56                        PUSH ESI
	$-6B     006D5F51       57                        PUSH EDI
	$-6A     006D5F52       8B7C24 0C                 MOV EDI, DWORD PTR SS:[ESP+0xC]
	$-66     006D5F56       8DB1 7C010000             LEA ESI, DWORD PTR DS:[ECX+0x17C]
	$-60     006D5F5C       3BF7                      CMP ESI, EDI
	$-5E     006D5F5E       74 35                     JE SHORT PathOfEx.006D5F95
	$-5C     006D5F60       8B06                      MOV EAX, DWORD PTR DS:[ESI]                ; PathOfEx.00EA2F7C
	$-5A     006D5F62       85C0                      TEST EAX, EAX
	$-58     006D5F64       74 13                     JE SHORT PathOfEx.006D5F79
	$-56     006D5F66       50                        PUSH EAX
	$-55     006D5F67       E8 A4980300               CALL PathOfEx.0070F810
	$-50     006D5F6C       8BC8                      MOV ECX, EAX
	$-4E     006D5F6E       E8 2D940300               CALL PathOfEx.0070F3A0
	$-49     006D5F73       C706 00000000             MOV DWORD PTR DS:[ESI], 0x0
	$-43     006D5F79       8B17                      MOV EDX, DWORD PTR DS:[EDI]                ; PathOfEx.00EA2E64
	$-41     006D5F7B       8916                      MOV DWORD PTR DS:[ESI], EDX
	$-3F     006D5F7D       85D2                      TEST EDX, EDX
	$-3D     006D5F7F       74 14                     JE SHORT PathOfEx.006D5F95
	$-3B     006D5F81       8D4A 38                   LEA ECX, DWORD PTR DS:[EDX+0x38]
	$-38     006D5F84       B8 01000000               MOV EAX, 0x1
	$-33     006D5F89       F0:0FC101                 LOCK XADD DWORD PTR DS:[ECX], EAX
	$-2F     006D5F8D       A1 4CD7FC00               MOV EAX, DWORD PTR DS:[0xFCD74C]
	$-2A     006D5F92       8942 34                   MOV DWORD PTR DS:[EDX+0x34], EAX
	$-27     006D5F95       5F                        POP EDI                                    ; 38030044
	$-26     006D5F96       5E                        POP ESI                                    ; 38030044
	$-25     006D5F97       C2 0400                   RETN 0x4
	$-22     006D5F9A       CC                        INT3
	$-21     006D5F9B       CC                        INT3
	$-20     006D5F9C       CC                        INT3
	$-1F     006D5F9D       CC                        INT3
	$-1E     006D5F9E       CC                        INT3
	$-1D     006D5F9F       CC                        INT3
	$-1C     006D5FA0       8D81 D0020000             LEA EAX, DWORD PTR DS:[ECX+0x2D0]          ; 获得地图数据的偏移
	$-16     006D5FA6       C3                        RETN
	$-15     006D5FA7       CC                        INT3
	$-14     006D5FA8       CC                        INT3
	$-13     006D5FA9       CC                        INT3
	$-12     006D5FAA       CC                        INT3
	$-11     006D5FAB       CC                        INT3
	$-10     006D5FAC       CC                        INT3
	$-F      006D5FAD       CC                        INT3
	$-E      006D5FAE       CC                        INT3
	$-D      006D5FAF       CC                        INT3
	$-C      006D5FB0       8BD1                      MOV EDX, ECX
	$-A      006D5FB2       8B4C24 04                 MOV ECX, DWORD PTR SS:[ESP+0x4]
	$-6      006D5FB6       C702 F429E700             MOV DWORD PTR DS:[EDX], PathOfEx.00E729F4  ; ASCII "绣u"
	$ ==>    006D5FBC       C742 04 00000000          MOV DWORD PTR DS:[EDX+0x4], 0x0
	$+7      006D5FC3       C742 08 00000000          MOV DWORD PTR DS:[EDX+0x8], 0x0
	$+E      006D5FCA       C742 0C 00000000          MOV DWORD PTR DS:[EDX+0xC], 0x0
	$+15     006D5FD1       8B41 04                   MOV EAX, DWORD PTR DS:[ECX+0x4]
	$+18     006D5FD4       8942 04                   MOV DWORD PTR DS:[EDX+0x4], EAX
	$+1B     006D5FD7       8B41 08                   MOV EAX, DWORD PTR DS:[ECX+0x8]
	$+1E     006D5FDA       8942 08                   MOV DWORD PTR DS:[EDX+0x8], EAX
	$+21     006D5FDD       8B41 0C                   MOV EAX, DWORD PTR DS:[ECX+0xC]
	$+24     006D5FE0       8942 0C                   MOV DWORD PTR DS:[EDX+0xC], EAX
	$+27     006D5FE3       8BC2                      MOV EAX, EDX
	$+29     006D5FE5       C741 04 00000000          MOV DWORD PTR DS:[ECX+0x4], 0x0
	$+30     006D5FEC       C741 08 00000000          MOV DWORD PTR DS:[ECX+0x8], 0x0
	$+37     006D5FF3       C741 0C 00000000          MOV DWORD PTR DS:[ECX+0xC], 0x0
	$+3E     006D5FFA       C2 0400                   RETN 0x4

	//这个函数里面调用了上面的获得地图数据的函数
	$-27     007D2C70       55                        PUSH EBP
	$-26     007D2C71       8BEC                      MOV EBP, ESP
	$-24     007D2C73       83E4 F8                   AND ESP, -0x8
	$-21     007D2C76       8BD1                      MOV EDX, ECX
	$-1F     007D2C78       83EC 08                   SUB ESP, 0x8
	$-1C     007D2C7B       817A 18 FFFFFF7F          CMP DWORD PTR DS:[EDX+0x18], 0x7FFFFFFF
	$-15     007D2C82       74 07                     JE SHORT PathOfEx.007D2C8B
	$-13     007D2C84       D942 24                   FLD DWORD PTR DS:[EDX+0x24]
	$-10     007D2C87       8BE5                      MOV ESP, EBP
	$-E      007D2C89       5D                        POP EBP                                    ; 38030044
	$-D      007D2C8A       C3                        RETN
	$-C      007D2C8B       8B42 04                   MOV EAX, DWORD PTR DS:[EDX+0x4]
	$-9      007D2C8E       8B48 10                   MOV ECX, DWORD PTR DS:[EAX+0x10]
	$-6      007D2C91       8D42 1C                   LEA EAX, DWORD PTR DS:[EDX+0x1C]
	$-3      007D2C94       50                        PUSH EAX
	$-2      007D2C95       8B01                      MOV EAX, DWORD PTR DS:[ECX]                ; PathOfEx.00EA23C0
	$ ==>    007D2C97       FF50 0C                   CALL NEAR DWORD PTR DS:[EAX+0xC]           ; 调用获得地图数据
	$+3      007D2C9A       8BC8                      MOV ECX, EAX
	$+5      007D2C9C       E8 0F260000               CALL PathOfEx.007D52B0
	$+A      007D2CA1       D95C24 04                 FSTP DWORD PTR SS:[ESP+0x4]
	$+E      007D2CA5       F3:0F104424 04            MOVSS XMM0, DWORD PTR SS:[ESP+0x4]
	$+14     007D2CAB       0F5705 406BF300           XORPS XMM0, DQWORD PTR DS:[0xF36B40]
	$+1B     007D2CB2       F3:0F114424 04            MOVSS DWORD PTR SS:[ESP+0x4], XMM0
	$+21     007D2CB8       D94424 04                 FLD DWORD PTR SS:[ESP+0x4]
	$+25     007D2CBC       8BE5                      MOV ESP, EBP
	$+27     007D2CBE       5D                        POP EBP                                    ; 38030044
	$+28     007D2CBF       C3                        RETN
	$+29     007D2CC0       8BD1                      MOV EDX, ECX
	$+2B     007D2CC2       8B42 18                   MOV EAX, DWORD PTR DS:[EDX+0x18]
	$+2E     007D2CC5       3D FFFFFF7F               CMP EAX, 0x7FFFFFFF
	$+33     007D2CCA       75 16                     JNZ SHORT PathOfEx.007D2CE2
	$+35     007D2CCC       8B42 04                   MOV EAX, DWORD PTR DS:[EDX+0x4]
	$+38     007D2CCF       8B48 10                   MOV ECX, DWORD PTR DS:[EAX+0x10]
	$+3B     007D2CD2       8D42 10                   LEA EAX, DWORD PTR DS:[EDX+0x10]
	$+3E     007D2CD5       50                        PUSH EAX
	$+3F     007D2CD6       8B01                      MOV EAX, DWORD PTR DS:[ECX]                ; PathOfEx.00EA23C0
	$+41     007D2CD8       FF50 0C                   CALL NEAR DWORD PTR DS:[EAX+0xC]
	$+44     007D2CDB       8BC8                      MOV ECX, EAX
	$+46     007D2CDD       E8 4E240000               CALL PathOfEx.007D5130
	$+4B     007D2CE2       C3                        RETN
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("C7420400000000C7420800000000C7420C000000008B41048942048B41088942088B410C89420C8BC2C74104", -0x1a);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x100 && dw_tmp < 0x400);
	ofs << "pt_cdword	g_o_world2map_data_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-C0     00490760     53              PUSH EBX
	$-BF     00490761     8BD9            MOV EBX, ECX
	$-BD     00490763     B8 ABAAAA2A     MOV EAX, 0x2AAAAAAB
	$-B8     00490768     55              PUSH EBP
	$-B7     00490769     56              PUSH ESI
	$-B6     0049076A     57              PUSH EDI
	$-B5     0049076B     8B6B 08         MOV EBP, DWORD PTR DS:[EBX+0x8]
	$-B2     0049076E     8BD5            MOV EDX, EBP
	$-B0     00490770     8B73 04         MOV ESI, DWORD PTR DS:[EBX+0x4]
	$-AD     00490773     2BD6            SUB EDX, ESI
	$-AB     00490775     8B7C24 14       MOV EDI, DWORD PTR SS:[ESP+0x14]
	$-A7     00490779     F7EA            IMUL EDX
	$-A5     0049077B     C1FA 02         SAR EDX, 0x2
	$-A2     0049077E     8BC2            MOV EAX, EDX
	$-A0     00490780     C1E8 1F         SHR EAX, 0x1F
	$-9D     00490783     03C2            ADD EAX, EDX
	$-9B     00490785     3BC7            CMP EAX, EDI
	$-99     00490787     73 55           JNB SHORT PathOfEx.004907DE
	$-97     00490789     2B33            SUB ESI, DWORD PTR DS:[EBX]
	$-95     0049078B     B8 ABAAAA2A     MOV EAX, 0x2AAAAAAB
	$-90     00490790     F7EE            IMUL ESI
	$-8E     00490792     B8 AAAAAA0A     MOV EAX, 0xAAAAAAA
	$-89     00490797     C1FA 02         SAR EDX, 0x2
	$-86     0049079A     8BCA            MOV ECX, EDX
	$-84     0049079C     C1E9 1F         SHR ECX, 0x1F
	$-81     0049079F     03CA            ADD ECX, EDX
	$-7F     004907A1     2BC1            SUB EAX, ECX
	$-7D     004907A3     3BC7            CMP EAX, EDI
	$-7B     004907A5     72 3E           JB SHORT PathOfEx.004907E5
	$-79     004907A7     2B2B            SUB EBP, DWORD PTR DS:[EBX]
	$-77     004907A9     B8 ABAAAA2A     MOV EAX, 0x2AAAAAAB
	$-72     004907AE     F7ED            IMUL EBP
	$-70     004907B0     03F9            ADD EDI, ECX
	$-6E     004907B2     33ED            XOR EBP, EBP
	$-6C     004907B4     C1FA 02         SAR EDX, 0x2
	$-69     004907B7     8BF2            MOV ESI, EDX
	$-67     004907B9     C1EE 1F         SHR ESI, 0x1F
	$-64     004907BC     03F2            ADD ESI, EDX
	$-62     004907BE     BA AAAAAA0A     MOV EDX, 0xAAAAAAA
	$-5D     004907C3     8BC6            MOV EAX, ESI
	$-5B     004907C5     D1E8            SHR EAX, 1
	$-59     004907C7     2BD0            SUB EDX, EAX
	$-57     004907C9     3BD6            CMP EDX, ESI
	$-55     004907CB     8D0C30          LEA ECX, DWORD PTR DS:[EAX+ESI]
	$-52     004907CE     0F42CD          CMOVB ECX, EBP
	$-4F     004907D1     3BCF            CMP ECX, EDI
	$-4D     004907D3     0F42CF          CMOVB ECX, EDI
	$-4A     004907D6     51              PUSH ECX
	$-49     004907D7     8BCB            MOV ECX, EBX
	$-47     004907D9     E8 E2FEFFFF     CALL PathOfEx.004906C0
	$-42     004907DE     5F              POP EDI                                         ; PathOfEx.008349D0
	$-41     004907DF     5E              POP ESI                                         ; PathOfEx.008349D0
	$-40     004907E0     5D              POP EBP                                         ; PathOfEx.008349D0
	$-3F     004907E1     5B              POP EBX                                         ; PathOfEx.008349D0
	$-3E     004907E2     C2 0400         RETN 0x4
	$-3B     004907E5     68 3C1BE000     PUSH PathOfEx.00E01B3C                          ; ASCII "vector<T> too long"
	$-36     004907EA     E8 68FA7C00     CALL PathOfEx.00C60257
	$-31     004907EF     CC              INT3
	$-30     004907F0     8B5424 04       MOV EDX, DWORD PTR SS:[ESP+0x4]
	$-2C     004907F4     56              PUSH ESI
	$-2B     004907F5     8B32            MOV ESI, DWORD PTR DS:[EDX]
	$-29     004907F7     85F6            TEST ESI, ESI
	$-27     004907F9     78 1D           JS SHORT PathOfEx.00490818
	$-25     004907FB     8B52 04         MOV EDX, DWORD PTR DS:[EDX+0x4]
	$-22     004907FE     85D2            TEST EDX, EDX
	$-20     00490800     78 16           JS SHORT PathOfEx.00490818
	$-1E     00490802     6B41 34 17      IMUL EAX, DWORD PTR DS:[ECX+0x34], 0x17
	$-1A     00490806     3BF0            CMP ESI, EAX
	$-18     00490808     7D 0E           JGE SHORT PathOfEx.00490818
	$-16     0049080A     6B41 38 17      IMUL EAX, DWORD PTR DS:[ECX+0x38], 0x17
	$-12     0049080E     3BD0            CMP EDX, EAX
	$-10     00490810     7D 06           JGE SHORT PathOfEx.00490818
	$-E      00490812     B0 01           MOV AL, 0x1
	$-C      00490814     5E              POP ESI                                         ; PathOfEx.008349D0
	$-B      00490815     C2 0400         RETN 0x4
	$-8      00490818     32C0            XOR AL, AL
	$-6      0049081A     5E              POP ESI                                         ; PathOfEx.008349D0
	$-5      0049081B     C2 0400         RETN 0x4
	$-2      0049081E     CC              INT3
	$-1      0049081F     CC              INT3
	$ ==>    00490820     8B5424 04       MOV EDX, DWORD PTR SS:[ESP+0x4]                 ; 移动的时候会断住，计算地图点相关数据
	$+4      00490824     56              PUSH ESI
	$+5      00490825     8B32            MOV ESI, DWORD PTR DS:[EDX]                     ; edx=stCD_VecInt*
	$+7      00490827     85F6            TEST ESI, ESI                                   ; esi=目标坐标x
	$+9      00490829     78 55           JS SHORT PathOfEx.00490880                      ; 判断是否为负
	$+B      0049082B     8B52 04         MOV EDX, DWORD PTR DS:[EDX+0x4]
	$+E      0049082E     85D2            TEST EDX, EDX                                   ; edx=目标坐标y
	$+10     00490830     78 4E           JS SHORT PathOfEx.00490880
	$+12     00490832     6B41 34 17      IMUL EAX, DWORD PTR DS:[ECX+0x34], 0x17         ; shrink_size_x
	$+16     00490836     3BF0            CMP ESI, EAX
	$+18     00490838     7D 46           JGE SHORT PathOfEx.00490880                     ; 目标坐标x大于等于地图宽度，则返回0
	$+1A     0049083A     6B41 38 17      IMUL EAX, DWORD PTR DS:[ECX+0x38], 0x17         ; shrink_size_y
	$+1E     0049083E     3BD0            CMP EDX, EAX
	$+20     00490840     7D 3E           JGE SHORT PathOfEx.00490880
	$+22     00490842     57              PUSH EDI
	$+23     00490843     8B79 54         MOV EDI, DWORD PTR DS:[ECX+0x54]                ; 不知道是干嘛的
	$+26     00490846     8BC6            MOV EAX, ESI
	$+28     00490848     0FAFFA          IMUL EDI, EDX                                   ; edi=edi*edx
	$+2B     0049084B     99              CDQ                                             ; eax=目标坐标x
	$+2C     0049084C     2BC2            SUB EAX, EDX
	$+2E     0049084E     D1F8            SAR EAX, 1                                      ; eax=目标坐标x/2
	$+30     00490850     03F8            ADD EDI, EAX
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

	//2017/2/10 13:55
	$-C0     0049AB00     53                  PUSH EBX
	$-BF     0049AB01     8BD9                MOV EBX, ECX
	$-BD     0049AB03     B8 ABAAAA2A         MOV EAX, 0x2AAAAAAB
	$-B8     0049AB08     55                  PUSH EBP
	$-B7     0049AB09     56                  PUSH ESI
	$-B6     0049AB0A     57                  PUSH EDI
	$-B5     0049AB0B     8B6B 08             MOV EBP, DWORD PTR DS:[EBX+0x8]
	$-B2     0049AB0E     8BD5                MOV EDX, EBP
	$-B0     0049AB10     8B73 04             MOV ESI, DWORD PTR DS:[EBX+0x4]
	$-AD     0049AB13     2BD6                SUB EDX, ESI
	$-AB     0049AB15     8B7C24 14           MOV EDI, DWORD PTR SS:[ESP+0x14]
	$-A7     0049AB19     F7EA                IMUL EDX
	$-A5     0049AB1B     C1FA 02             SAR EDX, 0x2
	$-A2     0049AB1E     8BC2                MOV EAX, EDX
	$-A0     0049AB20     C1E8 1F             SHR EAX, 0x1F
	$-9D     0049AB23     03C2                ADD EAX, EDX
	$-9B     0049AB25     3BC7                CMP EAX, EDI
	$-99     0049AB27     73 54               JNB SHORT PathOfEx.0049AB7D
	$-97     0049AB29     2B33                SUB ESI, DWORD PTR DS:[EBX]
	$-95     0049AB2B     B8 ABAAAA2A         MOV EAX, 0x2AAAAAAB
	$-90     0049AB30     F7EE                IMUL ESI
	$-8E     0049AB32     B8 AAAAAA0A         MOV EAX, 0xAAAAAAA
	$-89     0049AB37     C1FA 02             SAR EDX, 0x2
	$-86     0049AB3A     8BCA                MOV ECX, EDX
	$-84     0049AB3C     C1E9 1F             SHR ECX, 0x1F
	$-81     0049AB3F     03CA                ADD ECX, EDX
	$-7F     0049AB41     2BC1                SUB EAX, ECX
	$-7D     0049AB43     3BC7                CMP EAX, EDI
	$-7B     0049AB45     72 3D               JB SHORT PathOfEx.0049AB84
	$-79     0049AB47     2B2B                SUB EBP, DWORD PTR DS:[EBX]
	$-77     0049AB49     B8 ABAAAA2A         MOV EAX, 0x2AAAAAAB
	$-72     0049AB4E     F7ED                IMUL EBP
	$-70     0049AB50     03F9                ADD EDI, ECX
	$-6E     0049AB52     33C9                XOR ECX, ECX
	$-6C     0049AB54     C1FA 02             SAR EDX, 0x2
	$-69     0049AB57     8BF2                MOV ESI, EDX
	$-67     0049AB59     C1EE 1F             SHR ESI, 0x1F
	$-64     0049AB5C     03F2                ADD ESI, EDX
	$-62     0049AB5E     BA AAAAAA0A         MOV EDX, 0xAAAAAAA
	$-5D     0049AB63     8BC6                MOV EAX, ESI
	$-5B     0049AB65     D1E8                SHR EAX, 1
	$-59     0049AB67     2BD0                SUB EDX, EAX
	$-57     0049AB69     03C6                ADD EAX, ESI
	$-55     0049AB6B     3BD6                CMP EDX, ESI
	$-53     0049AB6D     0F43C8              CMOVNB ECX, EAX
	$-50     0049AB70     3BCF                CMP ECX, EDI
	$-4E     0049AB72     0F43F9              CMOVNB EDI, ECX
	$-4B     0049AB75     8BCB                MOV ECX, EBX
	$-49     0049AB77     57                  PUSH EDI
	$-48     0049AB78     E8 D3FEFFFF         CALL PathOfEx.0049AA50
	$-43     0049AB7D     5F                  POP EDI                                      ; PathOfEx.0083962E
	$-42     0049AB7E     5E                  POP ESI                                      ; PathOfEx.0083962E
	$-41     0049AB7F     5D                  POP EBP                                      ; PathOfEx.0083962E
	$-40     0049AB80     5B                  POP EBX                                      ; PathOfEx.0083962E
	$-3F     0049AB81     C2 0400             RETN 0x4
	$-3C     0049AB84     68 DC8AE000         PUSH PathOfEx.00E08ADC                       ; ASCII "vector<T> too long"
	$-37     0049AB89     E8 0CCE7C00         CALL PathOfEx.00C6799A
	$-32     0049AB8E     CC                  INT3
	$-31     0049AB8F     CC                  INT3
	$-30     0049AB90     8B5424 04           MOV EDX, DWORD PTR SS:[ESP+0x4]
	$-2C     0049AB94     56                  PUSH ESI
	$-2B     0049AB95     8B32                MOV ESI, DWORD PTR DS:[EDX]                  ; PathOfEx.00EE8CDC
	$-29     0049AB97     85F6                TEST ESI, ESI
	$-27     0049AB99     78 1D               JS SHORT PathOfEx.0049ABB8
	$-25     0049AB9B     8B52 04             MOV EDX, DWORD PTR DS:[EDX+0x4]
	$-22     0049AB9E     85D2                TEST EDX, EDX
	$-20     0049ABA0     78 16               JS SHORT PathOfEx.0049ABB8
	$-1E     0049ABA2     6B41 34 17          IMUL EAX, DWORD PTR DS:[ECX+0x34], 0x17
	$-1A     0049ABA6     3BF0                CMP ESI, EAX
	$-18     0049ABA8     7D 0E               JGE SHORT PathOfEx.0049ABB8
	$-16     0049ABAA     6B41 38 17          IMUL EAX, DWORD PTR DS:[ECX+0x38], 0x17
	$-12     0049ABAE     3BD0                CMP EDX, EAX
	$-10     0049ABB0     7D 06               JGE SHORT PathOfEx.0049ABB8
	$-E      0049ABB2     B0 01               MOV AL, 0x1
	$-C      0049ABB4     5E                  POP ESI                                      ; PathOfEx.0083962E
	$-B      0049ABB5     C2 0400             RETN 0x4
	$-8      0049ABB8     32C0                XOR AL, AL
	$-6      0049ABBA     5E                  POP ESI                                      ; PathOfEx.0083962E
	$-5      0049ABBB     C2 0400             RETN 0x4
	$-2      0049ABBE     CC                  INT3
	$-1      0049ABBF     CC                  INT3
	$ ==>    0049ABC0     8B5424 04           MOV EDX, DWORD PTR SS:[ESP+0x4]              ; 移动的时候会断住，计算地图点相关数据
	$+4      0049ABC4     53                  PUSH EBX
	$+5      0049ABC5     8B1A                MOV EBX, DWORD PTR DS:[EDX]                  ; PathOfEx.00EE8CDC
	$+7      0049ABC7     85DB                TEST EBX, EBX
	$+9      0049ABC9     78 4D               JS SHORT PathOfEx.0049AC18
	$+B      0049ABCB     8B52 04             MOV EDX, DWORD PTR DS:[EDX+0x4]
	$+E      0049ABCE     85D2                TEST EDX, EDX
	$+10     0049ABD0     78 46               JS SHORT PathOfEx.0049AC18
	$+12     0049ABD2     6B41 34 17          IMUL EAX, DWORD PTR DS:[ECX+0x34], 0x17
	$+16     0049ABD6     3BD8                CMP EBX, EAX
	$+18     0049ABD8     7D 3E               JGE SHORT PathOfEx.0049AC18
	$+1A     0049ABDA     6B41 38 17          IMUL EAX, DWORD PTR DS:[ECX+0x38], 0x17
	$+1E     0049ABDE     3BD0                CMP EDX, EAX
	$+20     0049ABE0     7D 36               JGE SHORT PathOfEx.0049AC18
	$+22     0049ABE2     56                  PUSH ESI
	$+23     0049ABE3     8B71 54             MOV ESI, DWORD PTR DS:[ECX+0x54]
	$+26     0049ABE6     8BC3                MOV EAX, EBX
	$+28     0049ABE8     0FAFF2              IMUL ESI, EDX
	$+2B     0049ABEB     99                  CDQ
	$+2C     0049ABEC     2BC2                SUB EAX, EDX
	$+2E     0049ABEE     D1F8                SAR EAX, 1
	$+30     0049ABF0     03F0                ADD ESI, EAX
	$+32     0049ABF2     8B4424 10           MOV EAX, DWORD PTR SS:[ESP+0x10]             ; PathOfEx.00838D2E
	$+36     0049ABF6     8D0440              LEA EAX, DWORD PTR DS:[EAX+EAX*2]
	$+39     0049ABF9     8B4481 3C           MOV EAX, DWORD PTR DS:[ECX+EAX*4+0x3C]       ; PathOfEx.00D6D6D6
	$+3D     0049ABFD     F6C3 01             TEST BL, 0x1
	$+40     0049AC00     74 0C               JE SHORT PathOfEx.0049AC0E
	$+42     0049AC02     0FB60430            MOVZX EAX, BYTE PTR DS:[EAX+ESI]
	$+46     0049AC06     5E                  POP ESI                                      ; PathOfEx.0083962E
	$+47     0049AC07     C1E8 04             SHR EAX, 0x4
	$+4A     0049AC0A     5B                  POP EBX                                      ; PathOfEx.0083962E
	$+4B     0049AC0B     C2 0800             RETN 0x8
	$+4E     0049AC0E     8A0406              MOV AL, BYTE PTR DS:[ESI+EAX]
	$+51     0049AC11     5E                  POP ESI                                      ; PathOfEx.0083962E
	$+52     0049AC12     24 0F               AND AL, 0xF
	$+54     0049AC14     5B                  POP EBX                                      ; PathOfEx.0083962E
	$+55     0049AC15     C2 0800             RETN 0x8
	$+58     0049AC18     32C0                XOR AL, AL
	$+5A     0049AC1A     5B                  POP EBX                                      ; PathOfEx.0083962E
	$+5B     0049AC1B     C2 0800             RETN 0x8
	$+5E     0049AC1E     CC                  INT3
	$+5F     0049AC1F     CC                  INT3
	$+60     0049AC20     55                  PUSH EBP

	//2017/3/27 11:50
	$-C1     0049B05F      CC                    INT3
	$-C0     0049B060      53                    PUSH EBX
	$-BF     0049B061      8BD9                  MOV EBX,ECX
	$-BD     0049B063      B8 ABAAAA2A           MOV EAX,0x2AAAAAAB
	$-B8     0049B068      55                    PUSH EBP
	$-B7     0049B069      56                    PUSH ESI
	$-B6     0049B06A      57                    PUSH EDI
	$-B5     0049B06B      8B6B 08               MOV EBP,DWORD PTR DS:[EBX+0x8]
	$-B2     0049B06E      8BD5                  MOV EDX,EBP
	$-B0     0049B070      8B73 04               MOV ESI,DWORD PTR DS:[EBX+0x4]
	$-AD     0049B073      2BD6                  SUB EDX,ESI
	$-AB     0049B075      8B7C24 14             MOV EDI,DWORD PTR SS:[ESP+0x14]
	$-A7     0049B079      F7EA                  IMUL EDX
	$-A5     0049B07B      C1FA 02               SAR EDX,0x2
	$-A2     0049B07E      8BC2                  MOV EAX,EDX
	$-A0     0049B080      C1E8 1F               SHR EAX,0x1F
	$-9D     0049B083      03C2                  ADD EAX,EDX
	$-9B     0049B085      3BC7                  CMP EAX,EDI
	$-99     0049B087      73 54                 JNB SHORT PathOfEx.0049B0DD
	$-97     0049B089      2B33                  SUB ESI,DWORD PTR DS:[EBX]
	$-95     0049B08B      B8 ABAAAA2A           MOV EAX,0x2AAAAAAB
	$-90     0049B090      F7EE                  IMUL ESI
	$-8E     0049B092      B8 AAAAAA0A           MOV EAX,0xAAAAAAA                        ; UNICODE "eed"
	$-89     0049B097      C1FA 02               SAR EDX,0x2
	$-86     0049B09A      8BCA                  MOV ECX,EDX
	$-84     0049B09C      C1E9 1F               SHR ECX,0x1F
	$-81     0049B09F      03CA                  ADD ECX,EDX
	$-7F     0049B0A1      2BC1                  SUB EAX,ECX
	$-7D     0049B0A3      3BC7                  CMP EAX,EDI
	$-7B     0049B0A5      72 3D                 JB SHORT PathOfEx.0049B0E4
	$-79     0049B0A7      2B2B                  SUB EBP,DWORD PTR DS:[EBX]
	$-77     0049B0A9      B8 ABAAAA2A           MOV EAX,0x2AAAAAAB
	$-72     0049B0AE      F7ED                  IMUL EBP
	$-70     0049B0B0      03F9                  ADD EDI,ECX
	$-6E     0049B0B2      33C9                  XOR ECX,ECX
	$-6C     0049B0B4      C1FA 02               SAR EDX,0x2
	$-69     0049B0B7      8BF2                  MOV ESI,EDX
	$-67     0049B0B9      C1EE 1F               SHR ESI,0x1F
	$-64     0049B0BC      03F2                  ADD ESI,EDX
	$-62     0049B0BE      BA AAAAAA0A           MOV EDX,0xAAAAAAA                        ; UNICODE "eed"
	$-5D     0049B0C3      8BC6                  MOV EAX,ESI
	$-5B     0049B0C5      D1E8                  SHR EAX,1
	$-59     0049B0C7      2BD0                  SUB EDX,EAX
	$-57     0049B0C9      03C6                  ADD EAX,ESI
	$-55     0049B0CB      3BD6                  CMP EDX,ESI
	$-53     0049B0CD      0F43C8                CMOVNB ECX,EAX
	$-50     0049B0D0      3BCF                  CMP ECX,EDI
	$-4E     0049B0D2      0F43F9                CMOVNB EDI,ECX
	$-4B     0049B0D5      8BCB                  MOV ECX,EBX
	$-49     0049B0D7      57                    PUSH EDI
	$-48     0049B0D8      E8 D3FEFFFF           CALL PathOfEx.0049AFB0
	$-43     0049B0DD      5F                    POP EDI
	$-42     0049B0DE      5E                    POP ESI
	$-41     0049B0DF      5D                    POP EBP
	$-40     0049B0E0      5B                    POP EBX
	$-3F     0049B0E1      C2 0400               RETN 0x4
	$-3C     0049B0E4      68 BCAAE000           PUSH PathOfEx.00E0AABC                   ; ASCII "vector<T> too long"
	$-37     0049B0E9      E8 2CF37C00           CALL PathOfEx.00C6A41A
	$-32     0049B0EE      CC                    INT3
	$-31     0049B0EF      CC                    INT3
	$-30     0049B0F0      8B5424 04             MOV EDX,DWORD PTR SS:[ESP+0x4]
	$-2C     0049B0F4      56                    PUSH ESI
	$-2B     0049B0F5      8B32                  MOV ESI,DWORD PTR DS:[EDX]
	$-29     0049B0F7      85F6                  TEST ESI,ESI
	$-27     0049B0F9      78 1D                 JS SHORT PathOfEx.0049B118
	$-25     0049B0FB      8B52 04               MOV EDX,DWORD PTR DS:[EDX+0x4]
	$-22     0049B0FE      85D2                  TEST EDX,EDX
	$-20     0049B100      78 16                 JS SHORT PathOfEx.0049B118
	$-1E     0049B102      6B41 34 17            IMUL EAX,DWORD PTR DS:[ECX+0x34],0x17
	$-1A     0049B106      3BF0                  CMP ESI,EAX
	$-18     0049B108      7D 0E                 JGE SHORT PathOfEx.0049B118
	$-16     0049B10A      6B41 38 17            IMUL EAX,DWORD PTR DS:[ECX+0x38],0x17
	$-12     0049B10E      3BD0                  CMP EDX,EAX
	$-10     0049B110      7D 06                 JGE SHORT PathOfEx.0049B118
	$-E      0049B112      B0 01                 MOV AL,0x1
	$-C      0049B114      5E                    POP ESI
	$-B      0049B115      C2 0400               RETN 0x4
	$-8      0049B118      32C0                  XOR AL,AL
	$-6      0049B11A      5E                    POP ESI
	$-5      0049B11B      C2 0400               RETN 0x4
	$-2      0049B11E      CC                    INT3                                     ; tag_地图数据之系数_34
	$-1      0049B11F      CC                    INT3                                     ; 移动的时候会断住，计算地图点相关数据
	$ ==>    0049B120      8B5424 04             MOV EDX,DWORD PTR SS:[ESP+0x4]           ; g_c_get_map_data_byte_
	$+4      0049B124      53                    PUSH EBX
	$+5      0049B125      8B1A                  MOV EBX,DWORD PTR DS:[EDX]               ; p1=stCD_VecInt*
	$+7      0049B127      85DB                  TEST EBX,EBX
	$+9      0049B129      78 4D                 JS SHORT PathOfEx.0049B178               ; 判断x是否为负
	$+B      0049B12B      8B52 04               MOV EDX,DWORD PTR DS:[EDX+0x4]
	$+E      0049B12E      85D2                  TEST EDX,EDX
	$+10     0049B130      78 46                 JS SHORT PathOfEx.0049B178               ; 判断y是否为负
	$+12     0049B132      6B41 34 17            IMUL EAX,DWORD PTR DS:[ECX+0x34],0x17    ; shrink_size_x
	$+16     0049B136      3BD8                  CMP EBX,EAX                              ; eax=地图宽度
	$+18     0049B138      7D 3E                 JGE SHORT PathOfEx.0049B178
	$+1A     0049B13A      6B41 38 17            IMUL EAX,DWORD PTR DS:[ECX+0x38],0x17    ; shrink_size_y
	$+1E     0049B13E      3BD0                  CMP EDX,EAX                              ; eax=地图高度
	$+20     0049B140      7D 36                 JGE SHORT PathOfEx.0049B178
	$+22     0049B142      56                    PUSH ESI
	$+23     0049B143      8B71 54               MOV ESI,DWORD PTR DS:[ECX+0x54]
	$+26     0049B146      8BC3                  MOV EAX,EBX                              ; ebx=x坐标
	$+28     0049B148      0FAFF2                IMUL ESI,EDX                             ; *y坐标
	$+2B     0049B14B      99                    CDQ                                      ; x坐标扩展符号位
	$+2C     0049B14C      2BC2                  SUB EAX,EDX
	$+2E     0049B14E      D1F8                  SAR EAX,1                                ; 目标坐标x/2
	$+30     0049B150      03F0                  ADD ESI,EAX                              ; then,通过x和y计算得出一个索引值
	$+32     0049B152      8B4424 10             MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+36     0049B156      8D0440                LEA EAX,DWORD PTR DS:[EAX+EAX*2]
	$+39     0049B159      8B4481 3C             MOV EAX,DWORD PTR DS:[ECX+EAX*4+0x3C]
	$+3D     0049B15D      F6C3 01               TEST BL,0x1
	$+40     0049B160      74 0C                 JE SHORT PathOfEx.0049B16E
	$+42     0049B162      0FB60430              MOVZX EAX,BYTE PTR DS:[EAX+ESI]
	$+46     0049B166      5E                    POP ESI
	$+47     0049B167      C1E8 04               SHR EAX,0x4
	$+4A     0049B16A      5B                    POP EBX
	$+4B     0049B16B      C2 0800               RETN 0x8
	$+4E     0049B16E      8A0406                MOV AL,BYTE PTR DS:[ESI+EAX]
	$+51     0049B171      5E                    POP ESI
	$+52     0049B172      24 0F                 AND AL,0xF                               ; enCD_MapDataFlag.kMDF_MaxMask
	$+54     0049B174      5B                    POP EBX
	$+55     0049B175      C2 0800               RETN 0x8
	$+58     0049B178      32C0                  XOR AL,AL
	$+5A     0049B17A      5B                    POP EBX
	$+5B     0049B17B      C2 0800               RETN 0x8
	$+5E     0049B17E      CC                    INT3
	$+5F     0049B17F      CC                    INT3
	$+60     0049B180      55                    PUSH EBP
	$+61     0049B181      8BEC                  MOV EBP,ESP
	$+63     0049B183      83E4 C0               AND ESP,0xFFFFFFC0
	$+66     0049B186      F20F1045 1C           MOVSD XMM0,QWORD PTR SS:[EBP+0x1C]
	$+6B     0049B18B      83EC 08               SUB ESP,0x8
	$+6E     0049B18E      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+70     0049B190      F20F110424            MOVSD QWORD PTR SS:[ESP],XMM0
	$+75     0049B195      FF75 10               PUSH DWORD PTR SS:[EBP+0x10]
	$+78     0049B198      FF75 0C               PUSH DWORD PTR SS:[EBP+0xC]
	$+7B     0049B19B      FF75 18               PUSH DWORD PTR SS:[EBP+0x18]
	$+7E     0049B19E      FF75 14               PUSH DWORD PTR SS:[EBP+0x14]
	$+81     0049B1A1      FF75 08               PUSH DWORD PTR SS:[EBP+0x8]
	$+84     0049B1A4      FF50 14               CALL DWORD PTR DS:[EAX+0x14]
	$+87     0049B1A7      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8A     0049B1AA      8BE5                  MOV ESP,EBP
	$+8C     0049B1AC      5D                    POP EBP
	$+8D     0049B1AD      C2 1C00               RETN 0x1C
	$+90     0049B1B0      55                    PUSH EBP
	$+91     0049B1B1      8BEC                  MOV EBP,ESP
	$+93     0049B1B3      6A FF                 PUSH -0x1
	$+95     0049B1B5      68 9A3FD200           PUSH PathOfEx.00D23F9A
	$+9A     0049B1BA      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+A0     0049B1C0      50                    PUSH EAX
	$+A1     0049B1C1      64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$+A8     0049B1C8      83EC 30               SUB ESP,0x30
	$+AB     0049B1CB      53                    PUSH EBX
	$+AC     0049B1CC      56                    PUSH ESI
	$+AD     0049B1CD      8BF1                  MOV ESI,ECX
	$+AF     0049B1CF      F20F114D D0           MOVSD QWORD PTR SS:[EBP-0x30],XMM1
	$+B4     0049B1D4      57                    PUSH EDI
	$+B5     0049B1D5      33FF                  XOR EDI,EDI
	$+B7     0049B1D7      8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
	$+BA     0049B1DA      8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
	$+BD     0049B1DD      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+BF     0049B1DF      897D E8               MOV DWORD PTR SS:[EBP-0x18],EDI
	$+C2     0049B1E2      8975 E0               MOV DWORD PTR SS:[EBP-0x20],ESI
	$+C5     0049B1E5      8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+C8     0049B1E8      8B4C30 38             MOV ECX,DWORD PTR DS:[EAX+ESI+0x38]
	$+CC     0049B1EC      85C9                  TEST ECX,ECX
	$+CE     0049B1EE      74 05                 JE SHORT PathOfEx.0049B1F5
	$+D0     0049B1F0      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+D2     0049B1F2      FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+D5     0049B1F5      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+DC     0049B1FC      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+DE     0049B1FE      8B48 04               MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+E1     0049B201      837C31 0C 00          CMP DWORD PTR DS:[ECX+ESI+0xC],0x0
	$+E6     0049B206      75 11                 JNZ SHORT PathOfEx.0049B219
	$+E8     0049B208      8B4C31 3C             MOV ECX,DWORD PTR DS:[ECX+ESI+0x3C]
	$+EC     0049B20C      85C9                  TEST ECX,ECX
	$+EE     0049B20E      74 09                 JE SHORT PathOfEx.0049B219
	$+F0     0049B210      3BCE                  CMP ECX,ESI
	$+F2     0049B212      74 05                 JE SHORT PathOfEx.0049B219
	$+F4     0049B214      E8 57B6FDFF           CALL PathOfEx.00476870
	$+F9     0049B219      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+FB     0049B21B      8B48 04               MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+FE     0049B21E      03CE                  ADD ECX,ESI
	$+100    0049B220      8379 0C 00            CMP DWORD PTR DS:[ECX+0xC],0x0
	$+104    0049B224      0F94C0                SETE AL
	$+107    0049B227      8845 E4               MOV BYTE PTR SS:[EBP-0x1C],AL
	$+10A    0049B22A      C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$+111    0049B231      84C0                  TEST AL,AL
	$+113    0049B233      0F84 AC000000         JE PathOfEx.0049B2E5
	$+119    0049B239      8D45 D8               LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+11C    0049B23C      50                    PUSH EAX
	$+11D    0049B23D      E8 CEC5FDFF           CALL PathOfEx.00477810
	$+122    0049B242      50                    PUSH EAX
	$+123    0049B243      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+127    0049B247      E8 14C4FEFF           CALL PathOfEx.00487660
	$+12C    0049B24C      83C4 04               ADD ESP,0x4
	$+12F    0049B24F      8BF8                  MOV EDI,EAX
	$+131    0049B251      C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$+135    0049B255      8B4D DC               MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+138    0049B258      85C9                  TEST ECX,ECX
	$+13A    0049B25A      74 11                 JE SHORT PathOfEx.0049B26D
	$+13C    0049B25C      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+13E    0049B25E      FF52 08               CALL DWORD PTR DS:[EDX+0x8]
	$+141    0049B261      85C0                  TEST EAX,EAX
	$+143    0049B263      74 08                 JE SHORT PathOfEx.0049B26D
	$+145    0049B265      8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$+147    0049B267      8BC8                  MOV ECX,EAX
	$+149    0049B269      6A 01                 PUSH 0x1
	$+14B    0049B26B      FF12                  CALL DWORD PTR DS:[EDX]
	$+14D    0049B26D      F20F1045 D0           MOVSD XMM0,QWORD PTR SS:[EBP-0x30]
	$+152    0049B272      83EC 08               SUB ESP,0x8
	$+155    0049B275      C645 FC 04            MOV BYTE PTR SS:[EBP-0x4],0x4
	$+159    0049B279      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+15B    0049B27B      F20F110424            MOVSD QWORD PTR SS:[ESP],XMM0
	$+160    0049B280      8B48 04               MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+163    0049B283      03CE                  ADD ECX,ESI
	$+165    0049B285      C645 C8 00            MOV BYTE PTR SS:[EBP-0x38],0x0
	$+169    0049B289      FF71 38               PUSH DWORD PTR DS:[ECX+0x38]
	$+16C    0049B28C      0FB741 40             MOVZX EAX,WORD PTR DS:[ECX+0x40]
	$+170    0049B290      FF75 C8               PUSH DWORD PTR SS:[EBP-0x38]
	$+173    0049B293      50                    PUSH EAX
	$+174    0049B294      51                    PUSH ECX
	$+175    0049B295      8D45 C8               LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+178    0049B298      8BCF                  MOV ECX,EDI
	$+17A    0049B29A      50                    PUSH EAX
	$+17B    0049B29B      E8 E0FEFFFF           CALL PathOfEx.0049B180
	$+180    0049B2A0      33C9                  XOR ECX,ECX
	$+182    0049B2A2      BF 04000000           MOV EDI,0x4
	$+187    0049B2A7      3808                  CMP BYTE PTR DS:[EAX],CL
	$+189    0049B2A9      0F44F9                CMOVE EDI,ECX
	$+18C    0049B2AC      EB 30                 JMP SHORT PathOfEx.0049B2DE
	$+18E    0049B2AE      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+191    0049B2B1      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+193    0049B2B3      8B50 04               MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+196    0049B2B6      8B440A 0C             MOV EAX,DWORD PTR DS:[EDX+ECX+0xC]
	$+19A    0049B2BA      03D1                  ADD EDX,ECX
	$+19C    0049B2BC      83C8 04               OR EAX,0x4
	$+19F    0049B2BF      837A 38 00            CMP DWORD PTR DS:[EDX+0x38],0x0
	$+1A3    0049B2C3      75 03                 JNZ SHORT PathOfEx.0049B2C8
	$+1A5    0049B2C5      83C8 04               OR EAX,0x4
	$+1A8    0049B2C8      6A 01                 PUSH 0x1
	$+1AA    0049B2CA      50                    PUSH EAX
	$+1AB    0049B2CB      8BCA                  MOV ECX,EDX
	$+1AD    0049B2CD      E8 AECEFDFF           CALL PathOfEx.00478180
	$+1B2    0049B2D2      B8 D8B24900           MOV EAX,PathOfEx.0049B2D8
	$+1B7    0049B2D7      C3                    RETN
	$+1B8    0049B2D8      8B75 EC               MOV ESI,DWORD PTR SS:[EBP-0x14]
	$+1BB    0049B2DB      8B7D E8               MOV EDI,DWORD PTR SS:[EBP-0x18]
	$+1BE    0049B2DE      C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$+1C5    0049B2E5      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+1C7    0049B2E7      8B48 04               MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+1CA    0049B2EA      03CE                  ADD ECX,ESI
	$+1CC    0049B2EC      85FF                  TEST EDI,EDI
	$+1CE    0049B2EE      74 19                 JE SHORT PathOfEx.0049B309
	$+1D0    0049B2F0      8B51 0C               MOV EDX,DWORD PTR DS:[ECX+0xC]
	$+1D3    0049B2F3      0BD7                  OR EDX,EDI
	$+1D5    0049B2F5      8BC2                  MOV EAX,EDX
	$+1D7    0049B2F7      83C8 04               OR EAX,0x4
	$+1DA    0049B2FA      8379 38 00            CMP DWORD PTR DS:[ECX+0x38],0x0
	$+1DE    0049B2FE      6A 00                 PUSH 0x0
	$+1E0    0049B300      0F45C2                CMOVNE EAX,EDX
	$+1E3    0049B303      50                    PUSH EAX
	$+1E4    0049B304      E8 77CEFDFF           CALL PathOfEx.00478180
	$+1E9    0049B309      C745 FC 06000000      MOV DWORD PTR SS:[EBP-0x4],0x6
	$+1F0    0049B310      E8 F1FB7C00           CALL PathOfEx.00C6AF06
	$+1F5    0049B315      84C0                  TEST AL,AL
	$+1F7    0049B317      75 07                 JNZ SHORT PathOfEx.0049B320
	$+1F9    0049B319      8BCE                  MOV ECX,ESI
	$+1FB    0049B31B      E8 90B6FDFF           CALL PathOfEx.004769B0
	$+200    0049B320      C645 FC 07            MOV BYTE PTR SS:[EBP-0x4],0x7
	$+204    0049B324      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+206    0049B326      8B40 04               MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+209    0049B329      8B4C30 38             MOV ECX,DWORD PTR DS:[EAX+ESI+0x38]
	$+20D    0049B32D      85C9                  TEST ECX,ECX
	$+20F    0049B32F      74 05                 JE SHORT PathOfEx.0049B336
	$+211    0049B331      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+213    0049B333      FF50 08               CALL DWORD PTR DS:[EAX+0x8]
	$+216    0049B336      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+219    0049B339      8BC6                  MOV EAX,ESI
	$+21B    0049B33B      5F                    POP EDI
	$+21C    0049B33C      5E                    POP ESI
	$+21D    0049B33D      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+224    0049B344      5B                    POP EBX
	$+225    0049B345      8BE5                  MOV ESP,EBP
	$+227    0049B347      5D                    POP EBP
	$+228    0049B348      C3                    RETN
	$+229    0049B349      CC                    INT3

	//上面的函数才是真的得到地图数据的函数，下面这个函数只是调用了它。
	//ff??????ff??????e8????????8b??????0f????3b??76??b0??59c2????51ff??????ff??????ff
	$-1      00839620     51                  PUSH ECX
	$ ==>    00839621     FF7424 10           PUSH DWORD PTR SS:[ESP+0x10]                 ; PathOfEx.00838D2E
	$+4      00839625     FF7424 0C           PUSH DWORD PTR SS:[ESP+0xC]
	$+8      00839629     E8 9215C6FF         CALL PathOfEx.0049ABC0                       ; 调用得到目标坐标点的数据
	$+D      0083962E     8B5424 0C           MOV EDX, DWORD PTR SS:[ESP+0xC]
	$+11     00839632     0FB6C0              MOVZX EAX, AL
	$+14     00839635     3BD0                CMP EDX, EAX
	$+16     00839637     76 06               JBE SHORT PathOfEx.0083963F
	$+18     00839639     B0 01               MOV AL, 0x1
	$+1A     0083963B     59                  POP ECX                                      ; PathOfEx.0083962E
	$+1B     0083963C     C2 1800             RETN 0x18
	$+1E     0083963F     51                  PUSH ECX
	$+1F     00839640     FF7424 1C           PUSH DWORD PTR SS:[ESP+0x1C]
	$+23     00839644     FF7424 1C           PUSH DWORD PTR SS:[ESP+0x1C]
	$+27     00839648     FF7424 1C           PUSH DWORD PTR SS:[ESP+0x1C]
	$+2B     0083964C     52                  PUSH EDX
	$+2C     0083964D     FF7424 1C           PUSH DWORD PTR SS:[ESP+0x1C]
	$+30     00839651     E8 1A000000         CALL PathOfEx.00839670
	$+35     00839656     59                  POP ECX                                      ; PathOfEx.0083962E
	$+36     00839657     C2 1800             RETN 0x18
	$+39     0083965A     CC                  INT3
	$+3A     0083965B     CC                  INT3
	$+3B     0083965C     CC                  INT3
	$+3C     0083965D     CC                  INT3
	$+3D     0083965E     CC                  INT3
	$+3E     0083965F     CC                  INT3
	$+3F     00839660     B8 7C030101         MOV EAX, PathOfEx.0101037C
	$+44     00839665     C3                  RETN
	*/

	//gj3
	/*
	$-277    006DF81F     CC                       INT3
	$-276    006DF820     55                       PUSH EBP
	$-275    006DF821     8BEC                     MOV EBP,ESP
	$-273    006DF823     6A FF                    PUSH -0x1
	$-271    006DF825     68 D48AD900              PUSH PathOfEx.00D98AD4
	$-26C    006DF82A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-266    006DF830     50                       PUSH EAX
	$-265    006DF831     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-25E    006DF838     83EC 1C                  SUB ESP,0x1C
	$-25B    006DF83B     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$-258    006DF83E     53                       PUSH EBX
	$-257    006DF83F     56                       PUSH ESI
	$-256    006DF840     57                       PUSH EDI
	$-255    006DF841     8BF9                     MOV EDI,ECX
	$-253    006DF843     897D E0                  MOV DWORD PTR SS:[EBP-0x20],EDI
	$-250    006DF846     25 01000080              AND EAX,0x80000001
	$-24B    006DF84B     79 05                    JNS SHORT PathOfEx.006DF852
	$-249    006DF84D     48                       DEC EAX
	$-248    006DF84E     83C8 FE                  OR EAX,0xFFFFFFFE
	$-245    006DF851     40                       INC EAX
	$-244    006DF852     8B5D 10                  MOV EBX,DWORD PTR SS:[EBP+0x10]
	$-241    006DF855     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-23F    006DF857     83F8 01                  CMP EAX,0x1
	$-23C    006DF85A     75 28                    JNZ SHORT PathOfEx.006DF884
	$-23A    006DF85C     85C9                     TEST ECX,ECX
	$-238    006DF85E     74 05                    JE SHORT PathOfEx.006DF865
	$-236    006DF860     E8 6B00F3FF              CALL PathOfEx.0060F8D0
	$-231    006DF865     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-22F    006DF867     8B41 30                  MOV EAX,DWORD PTR DS:[ECX+0x30]
	$-22C    006DF86A     0FB670 2D                MOVZX ESI,BYTE PTR DS:[EAX+0x2D]
	$-228    006DF86E     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$-225    006DF871     85C9                     TEST ECX,ECX
	$-223    006DF873     74 05                    JE SHORT PathOfEx.006DF87A
	$-221    006DF875     E8 5600F3FF              CALL PathOfEx.0060F8D0
	$-21C    006DF87A     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-21A    006DF87C     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$-217    006DF87F     8A40 2C                  MOV AL,BYTE PTR DS:[EAX+0x2C]
	$-214    006DF882     EB 26                    JMP SHORT PathOfEx.006DF8AA
	$-212    006DF884     85C9                     TEST ECX,ECX
	$-210    006DF886     74 05                    JE SHORT PathOfEx.006DF88D
	$-20E    006DF888     E8 4300F3FF              CALL PathOfEx.0060F8D0
	$-209    006DF88D     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-207    006DF88F     8B41 30                  MOV EAX,DWORD PTR DS:[ECX+0x30]
	$-204    006DF892     0FB670 2C                MOVZX ESI,BYTE PTR DS:[EAX+0x2C]
	$-200    006DF896     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$-1FD    006DF899     85C9                     TEST ECX,ECX
	$-1FB    006DF89B     74 05                    JE SHORT PathOfEx.006DF8A2
	$-1F9    006DF89D     E8 2E00F3FF              CALL PathOfEx.0060F8D0
	$-1F4    006DF8A2     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-1F2    006DF8A4     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$-1EF    006DF8A7     8A40 2D                  MOV AL,BYTE PTR DS:[EAX+0x2D]
	$-1EC    006DF8AA     0FB6C0                   MOVZX EAX,AL
	$-1E9    006DF8AD     8BCF                     MOV ECX,EDI
	$-1E7    006DF8AF     50                       PUSH EAX
	$-1E6    006DF8B0     56                       PUSH ESI
	$-1E5    006DF8B1     FF75 0C                  PUSH DWORD PTR SS:[EBP+0xC]
	$-1E2    006DF8B4     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-1DF    006DF8B7     FF75 08                  PUSH DWORD PTR SS:[EBP+0x8]
	$-1DC    006DF8BA     E8 D1FEFFFF              CALL PathOfEx.006DF790
	$-1D7    006DF8BF     84C0                     TEST AL,AL
	$-1D5    006DF8C1     0F84 1B010000            JE PathOfEx.006DF9E2
	$-1CF    006DF8C7     807F 08 00               CMP BYTE PTR DS:[EDI+0x8],0x0
	$-1CB    006DF8CB     0F84 26010000            JE PathOfEx.006DF9F7
	$-1C5    006DF8D1     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-1C2    006DF8D4     33C0                     XOR EAX,EAX
	$-1C0    006DF8D6     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$-1BD    006DF8D9     85C9                     TEST ECX,ECX
	$-1BB    006DF8DB     0F8E 16010000            JLE PathOfEx.006DF9F7
	$-1B5    006DF8E1     33FF                     XOR EDI,EDI
	$-1B3    006DF8E3     897D D8                  MOV DWORD PTR SS:[EBP-0x28],EDI
	$-1B0    006DF8E6     85F6                     TEST ESI,ESI
	$-1AE    006DF8E8     0F8E CC000000            JLE PathOfEx.006DF9BA
	$-1A8    006DF8EE     8B75 0C                  MOV ESI,DWORD PTR SS:[EBP+0xC]
	$-1A5    006DF8F1     03F0                     ADD ESI,EAX
	$-1A3    006DF8F3     8975 E4                  MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-1A0    006DF8F6     EB 03                    JMP SHORT PathOfEx.006DF8FB
	$-19E    006DF8F8     8B75 E4                  MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-19B    006DF8FB     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-199    006DF8FD     85C9                     TEST ECX,ECX
	$-197    006DF8FF     74 05                    JE SHORT PathOfEx.006DF906
	$-195    006DF901     E8 CAFFF2FF              CALL PathOfEx.0060F8D0
	$-190    006DF906     FF75 14                  PUSH DWORD PTR SS:[EBP+0x14]
	$-18D    006DF909     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-18B    006DF90B     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$-188    006DF90E     51                       PUSH ECX
	$-187    006DF90F     8D4D F0                  LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-184    006DF912     51                       PUSH ECX
	$-183    006DF913     8B48 30                  MOV ECX,DWORD PTR DS:[EAX+0x30]
	$-180    006DF916     E8 35B80300              CALL PathOfEx.0071B150
	$-17B    006DF91B     8B55 E0                  MOV EDX,DWORD PTR SS:[EBP-0x20]
	$-178    006DF91E     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-175    006DF921     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-16E    006DF928     03C7                     ADD EAX,EDI
	$-16C    006DF92A     8B4A 34                  MOV ECX,DWORD PTR DS:[EDX+0x34]
	$-169    006DF92D     41                       INC ECX
	$-168    006DF92E     0FAFCE                   IMUL ECX,ESI
	$-165    006DF931     8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-162    006DF934     03C8                     ADD ECX,EAX
	$-160    006DF936     8D1CC9                   LEA EBX,DWORD PTR DS:[ECX+ECX*8]
	$-15D    006DF939     035A 28                  ADD EBX,DWORD PTR DS:[EDX+0x28]
	$-15A    006DF93C     807B 04 00               CMP BYTE PTR DS:[EBX+0x4],0x0
	$-156    006DF940     74 3A                    JE SHORT PathOfEx.006DF97C
	$-154    006DF942     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-152    006DF944     3BC6                     CMP EAX,ESI
	$-150    006DF946     74 34                    JE SHORT PathOfEx.006DF97C
	$-14E    006DF948     85C0                     TEST EAX,EAX
	$-14C    006DF94A     75 15                    JNZ SHORT PathOfEx.006DF961
	$-14A    006DF94C     85F6                     TEST ESI,ESI
	$-148    006DF94E     74 2C                    JE SHORT PathOfEx.006DF97C
	$-146    006DF950     8BCE                     MOV ECX,ESI
	$-144    006DF952     E8 79FFF2FF              CALL PathOfEx.0060F8D0
	$-13F    006DF957     8B46 30                  MOV EAX,DWORD PTR DS:[ESI+0x30]
	$-13C    006DF95A     8A40 23                  MOV AL,BYTE PTR DS:[EAX+0x23]
	$-139    006DF95D     84C0                     TEST AL,AL
	$-137    006DF95F     75 1B                    JNZ SHORT PathOfEx.006DF97C
	$-135    006DF961     85F6                     TEST ESI,ESI
	$-133    006DF963     75 65                    JNZ SHORT PathOfEx.006DF9CA
	$-131    006DF965     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-12F    006DF967     85C9                     TEST ECX,ECX
	$-12D    006DF969     74 11                    JE SHORT PathOfEx.006DF97C
	$-12B    006DF96B     E8 60FFF2FF              CALL PathOfEx.0060F8D0
	$-126    006DF970     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-124    006DF972     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$-121    006DF975     8A40 23                  MOV AL,BYTE PTR DS:[EAX+0x23]
	$-11E    006DF978     84C0                     TEST AL,AL
	$-11C    006DF97A     74 4E                    JE SHORT PathOfEx.006DF9CA
	$-11A    006DF97C     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$-113    006DF983     85F6                     TEST ESI,ESI
	$-111    006DF985     74 14                    JE SHORT PathOfEx.006DF99B
	$-10F    006DF987     56                       PUSH ESI
	$-10E    006DF988     E8 C3E9F2FF              CALL PathOfEx.0060E350
	$-109    006DF98D     8BC8                     MOV ECX,EAX
	$-107    006DF98F     E8 8CE5F2FF              CALL PathOfEx.0060DF20
	$-102    006DF994     C745 F0 00000000         MOV DWORD PTR SS:[EBP-0x10],0x0
	$-FB     006DF99B     8B75 EC                  MOV ESI,DWORD PTR SS:[EBP-0x14]
	$-F8     006DF99E     47                       INC EDI
	$-F7     006DF99F     8B5D 10                  MOV EBX,DWORD PTR SS:[EBP+0x10]
	$-F4     006DF9A2     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-ED     006DF9A9     897D D8                  MOV DWORD PTR SS:[EBP-0x28],EDI
	$-EA     006DF9AC     3BFE                     CMP EDI,ESI
	$-E8     006DF9AE   ^ 0F8C 44FFFFFF            JL PathOfEx.006DF8F8
	$-E2     006DF9B4     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-DF     006DF9B7     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-DC     006DF9BA     40                       INC EAX
	$-DB     006DF9BB     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$-D8     006DF9BE     3BC1                     CMP EAX,ECX
	$-D6     006DF9C0     7D 35                    JGE SHORT PathOfEx.006DF9F7
	$-D4     006DF9C2     8B5D 10                  MOV EBX,DWORD PTR SS:[EBP+0x10]
	$-D1     006DF9C5   ^ E9 17FFFFFF              JMP PathOfEx.006DF8E1
	$-CC     006DF9CA     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-C5     006DF9D1     85F6                     TEST ESI,ESI
	$-C3     006DF9D3     74 0D                    JE SHORT PathOfEx.006DF9E2
	$-C1     006DF9D5     56                       PUSH ESI
	$-C0     006DF9D6     E8 75E9F2FF              CALL PathOfEx.0060E350
	$-BB     006DF9DB     8BC8                     MOV ECX,EAX
	$-B9     006DF9DD     E8 3EE5F2FF              CALL PathOfEx.0060DF20
	$-B4     006DF9E2     5F                       POP EDI
	$-B3     006DF9E3     5E                       POP ESI
	$-B2     006DF9E4     32C0                     XOR AL,AL
	$-B0     006DF9E6     5B                       POP EBX
	$-AF     006DF9E7     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-AC     006DF9EA     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-A5     006DF9F1     8BE5                     MOV ESP,EBP
	$-A3     006DF9F3     5D                       POP EBP
	$-A2     006DF9F4     C2 1000                  RETN 0x10
	$-9F     006DF9F7     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-9C     006DF9FA     B0 01                    MOV AL,0x1
	$-9A     006DF9FC     5F                       POP EDI
	$-99     006DF9FD     5E                       POP ESI
	$-98     006DF9FE     5B                       POP EBX
	$-97     006DF9FF     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-90     006DFA06     8BE5                     MOV ESP,EBP
	$-8E     006DFA08     5D                       POP EBP
	$-8D     006DFA09     C2 1000                  RETN 0x10
	$-8A     006DFA0C     CC                       INT3
	$-89     006DFA0D     CC                       INT3
	$-88     006DFA0E     CC                       INT3
	$-87     006DFA0F     CC                       INT3
	$-86     006DFA10     53                       PUSH EBX
	$-85     006DFA11     56                       PUSH ESI
	$-84     006DFA12     8B7424 0C                MOV ESI,DWORD PTR SS:[ESP+0xC]
	$-80     006DFA16     8BD9                     MOV EBX,ECX
	$-7E     006DFA18     57                       PUSH EDI
	$-7D     006DFA19     8B7C24 14                MOV EDI,DWORD PTR SS:[ESP+0x14]
	$-79     006DFA1D     8D47 12                  LEA EAX,DWORD PTR DS:[EDI+0x12]
	$-76     006DFA20     50                       PUSH EAX
	$-75     006DFA21     FF76 04                  PUSH DWORD PTR DS:[ESI+0x4]
	$-72     006DFA24     FF36                     PUSH DWORD PTR DS:[ESI]
	$-70     006DFA26     E8 D5000000              CALL PathOfEx.006DFB00
	$-6B     006DFA2B     84C0                     TEST AL,AL
	$-69     006DFA2D     74 58                    JE SHORT PathOfEx.006DFA87
	$-67     006DFA2F     8D47 16                  LEA EAX,DWORD PTR DS:[EDI+0x16]
	$-64     006DFA32     8BCB                     MOV ECX,EBX
	$-62     006DFA34     50                       PUSH EAX
	$-61     006DFA35     0FB647 10                MOVZX EAX,BYTE PTR DS:[EDI+0x10]
	$-5D     006DFA39     FF76 04                  PUSH DWORD PTR DS:[ESI+0x4]
	$-5A     006DFA3C     0306                     ADD EAX,DWORD PTR DS:[ESI]
	$-58     006DFA3E     50                       PUSH EAX
	$-57     006DFA3F     E8 BC000000              CALL PathOfEx.006DFB00
	$-52     006DFA44     84C0                     TEST AL,AL
	$-50     006DFA46     74 3F                    JE SHORT PathOfEx.006DFA87
	$-4E     006DFA48     8D47 1A                  LEA EAX,DWORD PTR DS:[EDI+0x1A]
	$-4B     006DFA4B     8BCB                     MOV ECX,EBX
	$-49     006DFA4D     50                       PUSH EAX
	$-48     006DFA4E     0FB647 11                MOVZX EAX,BYTE PTR DS:[EDI+0x11]
	$-44     006DFA52     0346 04                  ADD EAX,DWORD PTR DS:[ESI+0x4]
	$-41     006DFA55     50                       PUSH EAX
	$-40     006DFA56     0FB647 10                MOVZX EAX,BYTE PTR DS:[EDI+0x10]
	$-3C     006DFA5A     0306                     ADD EAX,DWORD PTR DS:[ESI]
	$-3A     006DFA5C     50                       PUSH EAX
	$-39     006DFA5D     E8 9E000000              CALL PathOfEx.006DFB00
	$-34     006DFA62     84C0                     TEST AL,AL
	$-32     006DFA64     74 21                    JE SHORT PathOfEx.006DFA87
	$-30     006DFA66     8D47 1E                  LEA EAX,DWORD PTR DS:[EDI+0x1E]
	$-2D     006DFA69     8BCB                     MOV ECX,EBX
	$-2B     006DFA6B     50                       PUSH EAX
	$-2A     006DFA6C     0FB647 11                MOVZX EAX,BYTE PTR DS:[EDI+0x11]
	$-26     006DFA70     0346 04                  ADD EAX,DWORD PTR DS:[ESI+0x4]
	$-23     006DFA73     50                       PUSH EAX
	$-22     006DFA74     FF36                     PUSH DWORD PTR DS:[ESI]
	$-20     006DFA76     E8 85000000              CALL PathOfEx.006DFB00
	$-1B     006DFA7B     84C0                     TEST AL,AL
	$-19     006DFA7D     74 08                    JE SHORT PathOfEx.006DFA87
	$-17     006DFA7F     B0 01                    MOV AL,0x1
	$-15     006DFA81     5F                       POP EDI
	$-14     006DFA82     5E                       POP ESI
	$-13     006DFA83     5B                       POP EBX
	$-12     006DFA84     C2 0800                  RETN 0x8
	$-F      006DFA87     5F                       POP EDI
	$-E      006DFA88     5E                       POP ESI
	$-D      006DFA89     32C0                     XOR AL,AL
	$-B      006DFA8B     5B                       POP EBX
	$-A      006DFA8C     C2 0800                  RETN 0x8                                              ; 移动的时候会断住，计算地图点相关数据
	$-7      006DFA8F     CC                       INT3                                                  ; g_c_get_map_data_byte_
	$-6      006DFA90     8B5424 04                MOV EDX,DWORD PTR SS:[ESP+0x4]                        ; tag_地图数据之系数_34
	$-2      006DFA94     53                       PUSH EBX                                              ; p1=stCD_VecInt*
	$-1      006DFA95     56                       PUSH ESI
	$ ==>    006DFA96     8B1A                     MOV EBX,DWORD PTR DS:[EDX]
	$+2      006DFA98     85DB                     TEST EBX,EBX
	$+4      006DFA9A     78 57                    JS SHORT PathOfEx.006DFAF3
	$+6      006DFA9C     8B52 04                  MOV EDX,DWORD PTR DS:[EDX+0x4]
	$+9      006DFA9F     85D2                     TEST EDX,EDX
	$+B      006DFAA1     78 50                    JS SHORT PathOfEx.006DFAF3
	$+D      006DFAA3     6B41 34 17               IMUL EAX,DWORD PTR DS:[ECX+0x34],0x17
	$+11     006DFAA7     3BD8                     CMP EBX,EAX
	$+13     006DFAA9     7D 48                    JGE SHORT PathOfEx.006DFAF3
	$+15     006DFAAB     6B41 38 17               IMUL EAX,DWORD PTR DS:[ECX+0x38],0x17
	$+19     006DFAAF     3BD0                     CMP EDX,EAX
	$+1B     006DFAB1     7D 40                    JGE SHORT PathOfEx.006DFAF3
	$+1D     006DFAB3     8B4424 10                MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+21     006DFAB7     8D0440                   LEA EAX,DWORD PTR DS:[EAX+EAX*2]
	$+24     006DFABA     8D40 0F                  LEA EAX,DWORD PTR DS:[EAX+0xF]
	$+27     006DFABD     8D3481                   LEA ESI,DWORD PTR DS:[ECX+EAX*4]
	$+2A     006DFAC0     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+2C     006DFAC2     3B46 04                  CMP EAX,DWORD PTR DS:[ESI+0x4]
	$+2F     006DFAC5     74 2C                    JE SHORT PathOfEx.006DFAF3
	$+31     006DFAC7     8B49 54                  MOV ECX,DWORD PTR DS:[ECX+0x54]
	$+34     006DFACA     8BC3                     MOV EAX,EBX
	$+36     006DFACC     0FAFCA                   IMUL ECX,EDX
	$+39     006DFACF     99                       CDQ
	$+3A     006DFAD0     2BC2                     SUB EAX,EDX
	$+3C     006DFAD2     D1F8                     SAR EAX,1
	$+3E     006DFAD4     03C8                     ADD ECX,EAX
	$+40     006DFAD6     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+42     006DFAD8     F6C3 01                  TEST BL,0x1
	$+45     006DFADB     74 0C                    JE SHORT PathOfEx.006DFAE9
	$+47     006DFADD     0FB60401                 MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+4B     006DFAE1     5E                       POP ESI
	$+4C     006DFAE2     C1E8 04                  SHR EAX,0x4
	$+4F     006DFAE5     5B                       POP EBX
	$+50     006DFAE6     C2 0800                  RETN 0x8
	$+53     006DFAE9     8A0401                   MOV AL,BYTE PTR DS:[ECX+EAX]
	$+56     006DFAEC     5E                       POP ESI
	$+57     006DFAED     24 0F                    AND AL,0xF                                            ; enCD_MapDataFlag.kMDF_MaxMask
	$+59     006DFAEF     5B                       POP EBX
	$+5A     006DFAF0     C2 0800                  RETN 0x8
	$+5D     006DFAF3     5E                       POP ESI
	$+5E     006DFAF4     32C0                     XOR AL,AL
	$+60     006DFAF6     5B                       POP EBX
	$+61     006DFAF7     C2 0800                  RETN 0x8
	$+64     006DFAFA     CC                       INT3
	$+65     006DFAFB     CC                       INT3
	$+66     006DFAFC     CC                       INT3
	$+67     006DFAFD     CC                       INT3
	$+68     006DFAFE     CC                       INT3
	$+69     006DFAFF     CC                       INT3
	$+6A     006DFB00     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+6D     006DFB03     40                       INC EAX
	$+6E     006DFB04     0FAF4424 08              IMUL EAX,DWORD PTR SS:[ESP+0x8]
	$+73     006DFB09     56                       PUSH ESI
	$+74     006DFB0A     57                       PUSH EDI
	$+75     006DFB0B     034424 0C                ADD EAX,DWORD PTR SS:[ESP+0xC]
	$+79     006DFB0F     8D34C0                   LEA ESI,DWORD PTR DS:[EAX+EAX*8]
	$+7C     006DFB12     0371 28                  ADD ESI,DWORD PTR DS:[ECX+0x28]
	$+7F     006DFB15     807E 04 00               CMP BYTE PTR DS:[ESI+0x4],0x0
	$+83     006DFB19     74 45                    JE SHORT PathOfEx.006DFB60
	$+85     006DFB1B     8B7C24 14                MOV EDI,DWORD PTR SS:[ESP+0x14]
	$+89     006DFB1F     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+8B     006DFB21     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$+8D     006DFB23     3BC1                     CMP EAX,ECX
	$+8F     006DFB25     74 39                    JE SHORT PathOfEx.006DFB60
	$+91     006DFB27     85C0                     TEST EAX,EAX
	$+93     006DFB29     75 14                    JNZ SHORT PathOfEx.006DFB3F
	$+95     006DFB2B     85C9                     TEST ECX,ECX
	$+97     006DFB2D     74 31                    JE SHORT PathOfEx.006DFB60
	$+99     006DFB2F     E8 9CFDF2FF              CALL PathOfEx.0060F8D0
	$+9E     006DFB34     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$+A0     006DFB36     8B41 30                  MOV EAX,DWORD PTR DS:[ECX+0x30]
	$+A3     006DFB39     8078 23 00               CMP BYTE PTR DS:[EAX+0x23],0x0
	$+A7     006DFB3D     75 21                    JNZ SHORT PathOfEx.006DFB60
	$+A9     006DFB3F     85C9                     TEST ECX,ECX
	$+AB     006DFB41     75 16                    JNZ SHORT PathOfEx.006DFB59
	$+AD     006DFB43     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+AF     006DFB45     85C9                     TEST ECX,ECX
	$+B1     006DFB47     74 17                    JE SHORT PathOfEx.006DFB60
	$+B3     006DFB49     E8 82FDF2FF              CALL PathOfEx.0060F8D0
	$+B8     006DFB4E     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+BA     006DFB50     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$+BD     006DFB53     8078 23 00               CMP BYTE PTR DS:[EAX+0x23],0x0
	$+C1     006DFB57     75 07                    JNZ SHORT PathOfEx.006DFB60
	$+C3     006DFB59     32C0                     XOR AL,AL
	$+C5     006DFB5B     5F                       POP EDI
	$+C6     006DFB5C     5E                       POP ESI
	$+C7     006DFB5D     C2 0C00                  RETN 0xC
	$+CA     006DFB60     5F                       POP EDI
	$+CB     006DFB61     B0 01                    MOV AL,0x1
	$+CD     006DFB63     5E                       POP ESI
	$+CE     006DFB64     C2 0C00                  RETN 0xC
	$+D1     006DFB67     CC                       INT3
	$+D2     006DFB68     CC                       INT3
	$+D3     006DFB69     CC                       INT3
	$+D4     006DFB6A     CC                       INT3
	$+D5     006DFB6B     CC                       INT3
	$+D6     006DFB6C     CC                       INT3
	$+D7     006DFB6D     CC                       INT3
	$+D8     006DFB6E     CC                       INT3
	$+D9     006DFB6F     CC                       INT3
	$+DA     006DFB70     55                       PUSH EBP
	$+DB     006DFB71     8BEC                     MOV EBP,ESP
	$+DD     006DFB73     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+E3     006DFB79     6A FF                    PUSH -0x1
	$+E5     006DFB7B     68 F38AD900              PUSH PathOfEx.00D98AF3
	$+EA     006DFB80     50                       PUSH EAX
	$+EB     006DFB81     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+F2     006DFB88     81EC 0C020000            SUB ESP,0x20C
	$+F8     006DFB8E     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+FB     006DFB91     40                       INC EAX
	$+FC     006DFB92     53                       PUSH EBX
	$+FD     006DFB93     8B5D 0C                  MOV EBX,DWORD PTR SS:[EBP+0xC]
	$+100    006DFB96     0FAFC3                   IMUL EAX,EBX
	$+103    006DFB99     56                       PUSH ESI
	$+104    006DFB9A     57                       PUSH EDI
	$+105    006DFB9B     8B7D 10                  MOV EDI,DWORD PTR SS:[EBP+0x10]
	$+108    006DFB9E     0345 08                  ADD EAX,DWORD PTR SS:[EBP+0x8]
	$+10B    006DFBA1     8D34C0                   LEA ESI,DWORD PTR DS:[EAX+EAX*8]
	$+10E    006DFBA4     0371 28                  ADD ESI,DWORD PTR DS:[ECX+0x28]
	$+111    006DFBA7     8079 08 00               CMP BYTE PTR DS:[ECX+0x8],0x0
	$+115    006DFBAB     74 06                    JE SHORT PathOfEx.006DFBB3
	$+117    006DFBAD     807E 04 00               CMP BYTE PTR DS:[ESI+0x4],0x0
	$+11B    006DFBB1     75 0A                    JNZ SHORT PathOfEx.006DFBBD
	$+11D    006DFBB3     807E 06 00               CMP BYTE PTR DS:[ESI+0x6],0x0
	$+121    006DFBB7     0F84 C3000000            JE PathOfEx.006DFC80
	$+127    006DFBBD     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$+129    006DFBBF     390E                     CMP DWORD PTR DS:[ESI],ECX
	$+12B    006DFBC1     0F84 B9000000            JE PathOfEx.006DFC80
	$+131    006DFBC7     85C9                     TEST ECX,ECX
	$+133    006DFBC9     74 11                    JE SHORT PathOfEx.006DFBDC
	$+135    006DFBCB     E8 00FDF2FF              CALL PathOfEx.0060F8D0
	$+13A    006DFBD0     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+13C    006DFBD2     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$+13F    006DFBD5     8A40 23                  MOV AL,BYTE PTR DS:[EAX+0x23]
	$+142    006DFBD8     84C0                     TEST AL,AL
	$+144    006DFBDA     74 1F                    JE SHORT PathOfEx.006DFBFB
	$+146    006DFBDC     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+148    006DFBDE     85C9                     TEST ECX,ECX
	$+14A    006DFBE0     0F84 9A000000            JE PathOfEx.006DFC80
	$+150    006DFBE6     E8 E5FCF2FF              CALL PathOfEx.0060F8D0
	$+155    006DFBEB     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+157    006DFBED     8B40 30                  MOV EAX,DWORD PTR DS:[EAX+0x30]
	$+15A    006DFBF0     8A40 23                  MOV AL,BYTE PTR DS:[EAX+0x23]
	$+15D    006DFBF3     84C0                     TEST AL,AL
	$+15F    006DFBF5     0F85 85000000            JNZ PathOfEx.006DFC80
	$+165    006DFBFB     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+168    006DFBFE     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+16B    006DFC01     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+16E    006DFC04     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+171    006DFC07     50                       PUSH EAX
	$+172    006DFC08     895D EC                  MOV DWORD PTR SS:[EBP-0x14],EBX
	$+175    006DFC0B     E8 0041F2FF              CALL PathOfEx.00603D10
	$+17A    006DFC10     8945 08                  MOV DWORD PTR SS:[EBP+0x8],EAX
	$+17D    006DFC13     8D8D ECFDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x214]
	$+183    006DFC19     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+18A    006DFC20     E8 CBE9FFFF              CALL PathOfEx.006DE5F0
	$+18F    006DFC25     8BD8                     MOV EBX,EAX
	$+191    006DFC27     33C0                     XOR EAX,EAX
	$+193    006DFC29     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+197    006DFC2D     8D73 40                  LEA ESI,DWORD PTR DS:[EBX+0x40]
	$+19A    006DFC30     8D7B 50                  LEA EDI,DWORD PTR DS:[EBX+0x50]
	$+19D    006DFC33     85F6                     TEST ESI,ESI
	$+19F    006DFC35     8BD7                     MOV EDX,EDI
	$+1A1    006DFC37     0F44D0                   CMOVE EDX,EAX
	$+1A4    006DFC3A     68 E089F500              PUSH PathOfEx.00F589E0                                ; UNICODE "Mismatching ground types at "
	$+1A9    006DFC3F     52                       PUSH EDX
	$+1AA    006DFC40     E8 8BABC5FF              CALL PathOfEx.0033A7D0
	$+1AF    006DFC45     8B55 08                  MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+1B2    006DFC48     33C0                     XOR EAX,EAX
	$+1B4    006DFC4A     83C4 08                  ADD ESP,0x8
	$+1B7    006DFC4D     85F6                     TEST ESI,ESI
	$+1B9    006DFC4F     0F44F8                   CMOVE EDI,EAX
	$+1BC    006DFC52     8BCF                     MOV ECX,EDI
	$+1BE    006DFC54     E8 E7D7C5FF              CALL PathOfEx.0033D440
	$+1C3    006DFC59     53                       PUSH EBX
	$+1C4    006DFC5A     8D8D DCFEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x124]
	$+1CA    006DFC60     E8 5BCED1FF              CALL PathOfEx.003FCAC0
	$+1CF    006DFC65     68 402F0501              PUSH PathOfEx.01052F40
	$+1D4    006DFC6A     8D85 DCFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x124]
	$+1DA    006DFC70     C785 DCFEFFFF A48AF500   MOV DWORD PTR SS:[EBP-0x124],PathOfEx.00F58AA4
	$+1E4    006DFC7A     50                       PUSH EAX
	$+1E5    006DFC7B     E8 F3406000              CALL PathOfEx.00CE3D73
	$+1EA    006DFC80     833F 00                  CMP DWORD PTR DS:[EDI],0x0
	$+1ED    006DFC83     74 3D                    JE SHORT PathOfEx.006DFCC2
	$+1EF    006DFC85     3BF7                     CMP ESI,EDI
	$+1F1    006DFC87     74 39                    JE SHORT PathOfEx.006DFCC2
	$+1F3    006DFC89     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+1F5    006DFC8B     85C0                     TEST EAX,EAX
	$+1F7    006DFC8D     74 17                    JE SHORT PathOfEx.006DFCA6
	$+1F9    006DFC8F     50                       PUSH EAX
	$+1FA    006DFC90     51                       PUSH ECX
	$+1FB    006DFC91     E8 BAE6F2FF              CALL PathOfEx.0060E350
	$+200    006DFC96     83C4 04                  ADD ESP,0x4
	$+203    006DFC99     8BC8                     MOV ECX,EAX
	$+205    006DFC9B     E8 80E2F2FF              CALL PathOfEx.0060DF20
	$+20A    006DFCA0     C706 00000000            MOV DWORD PTR DS:[ESI],0x0
	$+210    006DFCA6     8B17                     MOV EDX,DWORD PTR DS:[EDI]
	$+212    006DFCA8     8916                     MOV DWORD PTR DS:[ESI],EDX
	$+214    006DFCAA     85D2                     TEST EDX,EDX
	$+216    006DFCAC     74 14                    JE SHORT PathOfEx.006DFCC2
	$+218    006DFCAE     8D4A 38                  LEA ECX,DWORD PTR DS:[EDX+0x38]
	$+21B    006DFCB1     B8 01000000              MOV EAX,0x1
	$+220    006DFCB6     F0:0FC101                LOCK XADD DWORD PTR DS:[ECX],EAX
	$+224    006DFCBA     A1 B0AA0501              MOV EAX,DWORD PTR DS:[0x105AAB0]
	$+229    006DFCBF     8942 34                  MOV DWORD PTR DS:[EDX+0x34],EAX
	$+22C    006DFCC2     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+22F    006DFCC5     5F                       POP EDI
	$+230    006DFCC6     C646 04 01               MOV BYTE PTR DS:[ESI+0x4],0x1
	$+234    006DFCCA     5E                       POP ESI
	$+235    006DFCCB     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+23C    006DFCD2     5B                       POP EBX
	$+23D    006DFCD3     8BE5                     MOV ESP,EBP
	$+23F    006DFCD5     5D                       POP EBP
	$+240    006DFCD6     C2 0C00                  RETN 0xC
	$+243    006DFCD9     CC                       INT3
	$+244    006DFCDA     CC                       INT3
	$+245    006DFCDB     CC                       INT3
	$+246    006DFCDC     CC                       INT3
	$+247    006DFCDD     CC                       INT3
	$+248    006DFCDE     CC                       INT3
	$+249    006DFCDF     CC                       INT3
	$+24A    006DFCE0     51                       PUSH ECX
	$+24B    006DFCE1     56                       PUSH ESI
	$+24C    006DFCE2     FF7424 0C                PUSH DWORD PTR SS:[ESP+0xC]
	$+250    006DFCE6     8BF1                     MOV ESI,ECX
	$+252    006DFCE8     897424 08                MOV DWORD PTR SS:[ESP+0x8],ESI
	$+256    006DFCEC     E8 CFCDD1FF              CALL PathOfEx.003FCAC0
	$+25B    006DFCF1     C706 A48AF500            MOV DWORD PTR DS:[ESI],PathOfEx.00F58AA4
	$+261    006DFCF7     8BC6                     MOV EAX,ESI
	$+263    006DFCF9     5E                       POP ESI
	$+264    006DFCFA     59                       POP ECX
	$+265    006DFCFB     C2 0400                  RETN 0x4
	$+268    006DFCFE     CC                       INT3
	$+269    006DFCFF     CC                       INT3
	$+26A    006DFD00     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+26D    006DFD03     40                       INC EAX
	$+26E    006DFD04     0FAF4424 0C              IMUL EAX,DWORD PTR SS:[ESP+0xC]
	$+273    006DFD09     56                       PUSH ESI
	$+274    006DFD0A     034424 0C                ADD EAX,DWORD PTR SS:[ESP+0xC]
	$+278    006DFD0E     8D14C0                   LEA EDX,DWORD PTR DS:[EAX+EAX*8]
	$+27B    006DFD11     8B41 28                  MOV EAX,DWORD PTR DS:[ECX+0x28]
	$+27E    006DFD14     8B3402                   MOV ESI,DWORD PTR DS:[EDX+EAX]
	$+281    006DFD17     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+285    006DFD1B     8930                     MOV DWORD PTR DS:[EAX],ESI
	$+287    006DFD1D     85F6                     TEST ESI,ESI
	$+289    006DFD1F     74 15                    JE SHORT PathOfEx.006DFD36
	$+28B    006DFD21     8D56 38                  LEA EDX,DWORD PTR DS:[ESI+0x38]
	$+28E    006DFD24     B9 01000000              MOV ECX,0x1
	$+293    006DFD29     F0:0FC10A                LOCK XADD DWORD PTR DS:[EDX],ECX
	$+297    006DFD2D     8B0D B0AA0501            MOV ECX,DWORD PTR DS:[0x105AAB0]
	$+29D    006DFD33     894E 34                  MOV DWORD PTR DS:[ESI+0x34],ECX
	$+2A0    006DFD36     5E                       POP ESI
	$+2A1    006DFD37     C2 0C00                  RETN 0xC
	$+2A4    006DFD3A     CC                       INT3
	$+2A5    006DFD3B     CC                       INT3
	$+2A6    006DFD3C     CC                       INT3
	$+2A7    006DFD3D     CC                       INT3
	$+2A8    006DFD3E     CC                       INT3
	$+2A9    006DFD3F     CC                       INT3
	$+2AA    006DFD40     55                       PUSH EBP
	$+2AB    006DFD41     8BEC                     MOV EBP,ESP
	$+2AD    006DFD43     83E4 F0                  AND ESP,0xFFFFFFF0
	$+2B0    006DFD46     83EC 54                  SUB ESP,0x54
	$+2B3    006DFD49     6B51 38 17               IMUL EDX,DWORD PTR DS:[ECX+0x38],0x17
	$+2B7    006DFD4D     53                       PUSH EBX
	$+2B8    006DFD4E     6B59 34 17               IMUL EBX,DWORD PTR DS:[ECX+0x34],0x17
	$+2BC    006DFD52     56                       PUSH ESI
	$+2BD    006DFD53     57                       PUSH EDI
	$+2BE    006DFD54     894C24 10                MOV DWORD PTR SS:[ESP+0x10],ECX
	$+2C2    006DFD58     8D71 3C                  LEA ESI,DWORD PTR DS:[ECX+0x3C]
	$+2C5    006DFD5B     C64424 0F 55             MOV BYTE PTR SS:[ESP+0xF],0x55
	$+2CA    006DFD60     BF 02000000              MOV EDI,0x2
	$+2CF    006DFD65     C74424 1C 1F000000       MOV DWORD PTR SS:[ESP+0x1C],0x1F
	$+2D7    006DFD6D     8D5B 01                  LEA EBX,DWORD PTR DS:[EBX+0x1]
	$+2DA    006DFD70     D1EB                     SHR EBX,1
	$+2DC    006DFD72     8959 54                  MOV DWORD PTR DS:[ECX+0x54],EBX
	$+2DF    006DFD75     0FAFDA                   IMUL EBX,EDX
	$+2E2    006DFD78     8D4424 0F                LEA EAX,DWORD PTR SS:[ESP+0xF]
	$+2E6    006DFD7C     8BCE                     MOV ECX,ESI
	$+2E8    006DFD7E     50                       PUSH EAX
	$+2E9    006DFD7F     53                       PUSH EBX
	$+2EA    006DFD80     E8 7B190000              CALL PathOfEx.006E1700
	$+2EF    006DFD85     83C6 0C                  ADD ESI,0xC
	$+2F2    006DFD88     83EF 01                  SUB EDI,0x1
	$+2F5    006DFD8B   ^ 75 EB                    JNZ SHORT PathOfEx.006DFD78
	$+2F7    006DFD8D     8B5424 10                MOV EDX,DWORD PTR SS:[ESP+0x10]
	$+2FB    006DFD91     8D5C24 1C                LEA EBX,DWORD PTR SS:[ESP+0x1C]
	$+2FF    006DFD95     33F6                     XOR ESI,ESI
	$+301    006DFD97     897C24 24                MOV DWORD PTR SS:[ESP+0x24],EDI
	$+305    006DFD9B     897424 20                MOV DWORD PTR SS:[ESP+0x20],ESI
	$+309    006DFD9F     897424 30                MOV DWORD PTR SS:[ESP+0x30],ESI
	$+30D    006DFDA3     8B4A 34                  MOV ECX,DWORD PTR DS:[EDX+0x34]
	$+310    006DFDA6     8B42 38                  MOV EAX,DWORD PTR DS:[EDX+0x38]
	$+313    006DFDA9     894C24 28                MOV DWORD PTR SS:[ESP+0x28],ECX
	$+317    006DFDAD     894424 2C                MOV DWORD PTR SS:[ESP+0x2C],EAX
	$+31B    006DFDB1     0F284424 20              MOVAPS XMM0,DQWORD PTR SS:[ESP+0x20]
	$+320    006DFDB6     894424 34                MOV DWORD PTR SS:[ESP+0x34],EAX
	$+324    006DFDBA     85C9                     TEST ECX,ECX
	$+326    006DFDBC     75 15                    JNZ SHORT PathOfEx.006DFDD3
	$+328    006DFDBE     894C24 30                MOV DWORD PTR SS:[ESP+0x30],ECX
	$+32C    006DFDC2     0F10C8                   MOVUPS XMM1,XMM0
	$+32F    006DFDC5     894424 34                MOV DWORD PTR SS:[ESP+0x34],EAX
	$+333    006DFDC9     894C24 50                MOV DWORD PTR SS:[ESP+0x50],ECX
	$+337    006DFDCD     894424 54                MOV DWORD PTR SS:[ESP+0x54],EAX
	$+33B    006DFDD1     EB 23                    JMP SHORT PathOfEx.006DFDF6
	$+33D    006DFDD3     85C0                     TEST EAX,EAX
	$+33F    006DFDD5     75 08                    JNZ SHORT PathOfEx.006DFDDF
	$+341    006DFDD7     897424 30                MOV DWORD PTR SS:[ESP+0x30],ESI
	$+345    006DFDDB     897C24 34                MOV DWORD PTR SS:[ESP+0x34],EDI
	$+349    006DFDDF     897424 50                MOV DWORD PTR SS:[ESP+0x50],ESI
	$+34D    006DFDE3     0F10C8                   MOVUPS XMM1,XMM0
	$+350    006DFDE6     897C24 54                MOV DWORD PTR SS:[ESP+0x54],EDI
	$+354    006DFDEA     85C0                     TEST EAX,EAX
	$+356    006DFDEC     75 08                    JNZ SHORT PathOfEx.006DFDF6
	$+358    006DFDEE     897424 50                MOV DWORD PTR SS:[ESP+0x50],ESI
	$+35C    006DFDF2     897C24 54                MOV DWORD PTR SS:[ESP+0x54],EDI
	$+360    006DFDF6     83EC 18                  SUB ESP,0x18
	$+363    006DFDF9     895424 28                MOV DWORD PTR SS:[ESP+0x28],EDX
	$+367    006DFDFD     8BC4                     MOV EAX,ESP
	$+369    006DFDFF     895C24 2C                MOV DWORD PTR SS:[ESP+0x2C],EBX
	$+36D    006DFE03     83EC 18                  SUB ESP,0x18
	$+370    006DFE06     8D4C24 40                LEA ECX,DWORD PTR SS:[ESP+0x40]
	$+374    006DFE0A     0F1100                   MOVUPS DQWORD PTR DS:[EAX],XMM0
	$+377    006DFE0D     F30F7E4424 60            MOVQ XMM0,QWORD PTR SS:[ESP+0x60]
	$+37D    006DFE13     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$+382    006DFE18     8BC4                     MOV EAX,ESP
	$+384    006DFE1A     F30F7E8424 80000000      MOVQ XMM0,QWORD PTR SS:[ESP+0x80]
	$+38D    006DFE23     0F1108                   MOVUPS DQWORD PTR DS:[EAX],XMM1
	$+390    006DFE26     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$+395    006DFE2B     E8 C0260000              CALL PathOfEx.006E24F0
	$+39A    006DFE30     83C4 30                  ADD ESP,0x30
	$+39D    006DFE33     5F                       POP EDI
	$+39E    006DFE34     5E                       POP ESI
	$+39F    006DFE35     5B                       POP EBX
	$+3A0    006DFE36     8BE5                     MOV ESP,EBP
	$+3A2    006DFE38     5D                       POP EBP
	$+3A3    006DFE39     C3                       RETN
	$+3A4    006DFE3A     CC                       INT3
	$+3A5    006DFE3B     CC                       INT3
	$+3A6    006DFE3C     CC                       INT3
	$+3A7    006DFE3D     CC                       INT3
	$+3A8    006DFE3E     CC                       INT3
	$+3A9    006DFE3F     CC                       INT3
	$+3AA    006DFE40     55                       PUSH EBP
	$+3AB    006DFE41     8BEC                     MOV EBP,ESP
	$+3AD    006DFE43     6A FF                    PUSH -0x1
	$+3AF    006DFE45     68 0E8BD900              PUSH PathOfEx.00D98B0E
	$+3B4    006DFE4A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+3BA    006DFE50     50                       PUSH EAX
	$+3BB    006DFE51     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+3C2    006DFE58     81EC AC000000            SUB ESP,0xAC
	$+3C8    006DFE5E     8BC1                     MOV EAX,ECX
	$+3CA    006DFE60     53                       PUSH EBX
	$+3CB    006DFE61     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+3CE    006DFE64     56                       PUSH ESI
	$+3CF    006DFE65     8B30                     MOV ESI,DWORD PTR DS:[EAX]
	$+3D1    006DFE67     57                       PUSH EDI
	$+3D2    006DFE68     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+3D5    006DFE6B     8D45 B8                  LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+3D8    006DFE6E     50                       PUSH EAX
	$+3D9    006DFE6F     8B7E 34                  MOV EDI,DWORD PTR DS:[ESI+0x34]
	$+3DC    006DFE72     0FAF7B 04                IMUL EDI,DWORD PTR DS:[EBX+0x4]
	$+3E0    006DFE76     8975 C0                  MOV DWORD PTR SS:[EBP-0x40],ESI
	$+3E3    006DFE79     033B                     ADD EDI,DWORD PTR DS:[EBX]
	$+3E5    006DFE7B     C1E7 05                  SHL EDI,0x5
	$+3E8    006DFE7E     037E 24                  ADD EDI,DWORD PTR DS:[ESI+0x24]
	$+3EB    006DFE81     8BCF                     MOV ECX,EDI
	$+3ED    006DFE83     E8 D8E8C7FF              CALL PathOfEx.0035E760
	$+3F2    006DFE88     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+3F9    006DFE8F     837D BC 00               CMP DWORD PTR SS:[EBP-0x44],0x0
	$+3FD    006DFE93     895D E4                  MOV DWORD PTR SS:[EBP-0x1C],EBX
	$+400    006DFE96     8975 E8                  MOV DWORD PTR SS:[EBP-0x18],ESI
	$+403    006DFE99     75 19                    JNZ SHORT PathOfEx.006DFEB4
	$+405    006DFE9B     6A 00                    PUSH 0x0
	$+407    006DFE9D     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+40A    006DFEA0     E8 AB030000              CALL PathOfEx.006E0250
	$+40F    006DFEA5     6A 01                    PUSH 0x1
	$+411    006DFEA7     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+414    006DFEAA     E8 A1030000              CALL PathOfEx.006E0250
	$+419    006DFEAF     E9 64030000              JMP PathOfEx.006E0218
	$+41E    006DFEB4     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+420    006DFEB6     8B73 04                  MOV ESI,DWORD PTR DS:[EBX+0x4]
	$+423    006DFEB9     33DB                     XOR EBX,EBX
	$+425    006DFEBB     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+428    006DFEBE     6BC0 17                  IMUL EAX,EAX,0x17
	$+42B    006DFEC1     6BCE 17                  IMUL ECX,ESI,0x17
	$+42E    006DFEC4     8945 A0                  MOV DWORD PTR SS:[EBP-0x60],EAX
	$+431    006DFEC7     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+434    006DFECA     894D A4                  MOV DWORD PTR SS:[EBP-0x5C],ECX
	$+437    006DFECD     8B08                     MOV ECX,DWORD PTR DS:[EAX]
	$+439    006DFECF     894D A8                  MOV DWORD PTR SS:[EBP-0x58],ECX
	$+43C    006DFED2     8B41 38                  MOV EAX,DWORD PTR DS:[ECX+0x38]
	$+43F    006DFED5     48                       DEC EAX
	$+440    006DFED6     3BF0                     CMP ESI,EAX
	$+442    006DFED8     8B41 34                  MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+445    006DFEDB     8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
	$+448    006DFEDE     8BCE                     MOV ECX,ESI
	$+44A    006DFEE0     0F95C3                   SETNE BL
	$+44D    006DFEE3     48                       DEC EAX
	$+44E    006DFEE4     33D2                     XOR EDX,EDX
	$+450    006DFEE6     3945 E0                  CMP DWORD PTR SS:[EBP-0x20],EAX
	$+453    006DFEE9     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+456    006DFEEC     0F95C2                   SETNE DL
	$+459    006DFEEF     F7D9                     NEG ECX
	$+45B    006DFEF1     8D1C9D 17000000          LEA EBX,DWORD PTR DS:[EBX*4+0x17]
	$+462    006DFEF8     1BC9                     SBB ECX,ECX
	$+464    006DFEFA     895D 9C                  MOV DWORD PTR SS:[EBP-0x64],EBX
	$+467    006DFEFD     83E1 FC                  AND ECX,0xFFFFFFFC
	$+46A    006DFF00     F7D8                     NEG EAX
	$+46C    006DFF02     8D1495 17000000          LEA EDX,DWORD PTR DS:[EDX*4+0x17]
	$+473    006DFF09     894D 94                  MOV DWORD PTR SS:[EBP-0x6C],ECX
	$+476    006DFF0C     1BC0                     SBB EAX,EAX
	$+478    006DFF0E     8955 98                  MOV DWORD PTR SS:[EBP-0x68],EDX
	$+47B    006DFF11     83E0 FC                  AND EAX,0xFFFFFFFC
	$+47E    006DFF14     8945 90                  MOV DWORD PTR SS:[EBP-0x70],EAX
	$+481    006DFF17     8B45 BC                  MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+484    006DFF1A     8A58 20                  MOV BL,BYTE PTR DS:[EAX+0x20]
	$+487    006DFF1D     885D 0B                  MOV BYTE PTR SS:[EBP+0xB],BL
	$+48A    006DFF20     F6C3 02                  TEST BL,0x2
	$+48D    006DFF23     0F84 2C010000            JE PathOfEx.006E0055
	$+493    006DFF29     F6C3 10                  TEST BL,0x10
	$+496    006DFF2C     74 19                    JE SHORT PathOfEx.006DFF47
	$+498    006DFF2E     6A 00                    PUSH 0x0
	$+49A    006DFF30     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+49D    006DFF33     E8 18030000              CALL PathOfEx.006E0250
	$+4A2    006DFF38     6A 01                    PUSH 0x1
	$+4A4    006DFF3A     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+4A7    006DFF3D     E8 0E030000              CALL PathOfEx.006E0250
	$+4AC    006DFF42     E9 D1020000              JMP PathOfEx.006E0218
	$+4B1    006DFF47     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+4B4    006DFF4A     8B55 A8                  MOV EDX,DWORD PTR SS:[EBP-0x58]
	$+4B7    006DFF4D     40                       INC EAX
	$+4B8    006DFF4E     0FAFF0                   IMUL ESI,EAX
	$+4BB    006DFF51     8B52 28                  MOV EDX,DWORD PTR DS:[EDX+0x28]
	$+4BE    006DFF54     0375 E0                  ADD ESI,DWORD PTR SS:[EBP-0x20]
	$+4C1    006DFF57     8D0C06                   LEA ECX,DWORD PTR DS:[ESI+EAX]
	$+4C4    006DFF5A     8D04CA                   LEA EAX,DWORD PTR DS:[EDX+ECX*8]
	$+4C7    006DFF5D     03C8                     ADD ECX,EAX
	$+4C9    006DFF5F     8D04F2                   LEA EAX,DWORD PTR DS:[EDX+ESI*8]
	$+4CC    006DFF62     0FB65406 10              MOVZX EDX,BYTE PTR DS:[ESI+EAX+0x10]
	$+4D1    006DFF67     0FB64406 07              MOVZX EAX,BYTE PTR DS:[ESI+EAX+0x7]
	$+4D6    006DFF6C     03D0                     ADD EDX,EAX
	$+4D8    006DFF6E     0FB641 10                MOVZX EAX,BYTE PTR DS:[ECX+0x10]
	$+4DC    006DFF72     03D0                     ADD EDX,EAX
	$+4DE    006DFF74     0FB641 07                MOVZX EAX,BYTE PTR DS:[ECX+0x7]
	$+4E2    006DFF78     03C2                     ADD EAX,EDX
	$+4E4    006DFF7A     F6C3 01                  TEST BL,0x1
	$+4E7    006DFF7D     0F85 B5000000            JNZ PathOfEx.006E0038
	$+4ED    006DFF83     83F8 04                  CMP EAX,0x4
	$+4F0    006DFF86     0F84 AC000000            JE PathOfEx.006E0038
	$+4F6    006DFF8C     8B4D BC                  MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+4F9    006DFF8F     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+4FC    006DFF92     50                       PUSH EAX
	$+4FD    006DFF93     E8 288C0300              CALL PathOfEx.00718BC0
	$+502    006DFF98     8B4D DC                  MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+505    006DFF9B     897D CC                  MOV DWORD PTR SS:[EBP-0x34],EDI
	$+508    006DFF9E     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+50B    006DFFA1     8D4D A0                  LEA ECX,DWORD PTR SS:[EBP-0x60]
	$+50E    006DFFA4     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
	$+511    006DFFA7     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+514    006DFFAA     8945 D0                  MOV DWORD PTR SS:[EBP-0x30],EAX
	$+517    006DFFAD     8D85 64FFFFFF            LEA EAX,DWORD PTR SS:[EBP-0x9C]
	$+51D    006DFFB3     894D C4                  MOV DWORD PTR SS:[EBP-0x3C],ECX
	$+520    006DFFB6     8D4D 90                  LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+523    006DFFB9     50                       PUSH EAX
	$+524    006DFFBA     E8 2134F2FF              CALL PathOfEx.006033E0
	$+529    006DFFBF     8D4D 90                  LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+52C    006DFFC2     0F1000                   MOVUPS XMM0,DQWORD PTR DS:[EAX]
	$+52F    006DFFC5     0F1185 7CFFFFFF          MOVUPS DQWORD PTR SS:[EBP-0x84],XMM0
	$+536    006DFFCC     F30F7E40 10              MOVQ XMM0,QWORD PTR DS:[EAX+0x10]
	$+53B    006DFFD1     8D85 4CFFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xB4]
	$+541    006DFFD7     50                       PUSH EAX
	$+542    006DFFD8     660FD645 E4              MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$+547    006DFFDD     E8 AE33F2FF              CALL PathOfEx.00603390
	$+54C    006DFFE2     0F1045 C4                MOVUPS XMM0,DQWORD PTR SS:[EBP-0x3C]
	$+550    006DFFE6     83EC 18                  SUB ESP,0x18
	$+553    006DFFE9     8BD0                     MOV EDX,EAX
	$+555    006DFFEB     8B45 C0                  MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+558    006DFFEE     8BCC                     MOV ECX,ESP
	$+55A    006DFFF0     0F1145 C4                MOVUPS DQWORD PTR SS:[EBP-0x3C],XMM0
	$+55E    006DFFF4     83EC 18                  SUB ESP,0x18
	$+561    006DFFF7     8945 D4                  MOV DWORD PTR SS:[EBP-0x2C],EAX
	$+564    006DFFFA     0F1085 7CFFFFFF          MOVUPS XMM0,DQWORD PTR SS:[EBP-0x84]
	$+56B    006E0001     8BC4                     MOV EAX,ESP
	$+56D    006E0003     0F1101                   MOVUPS DQWORD PTR DS:[ECX],XMM0
	$+570    006E0006     F30F7E45 E4              MOVQ XMM0,QWORD PTR SS:[EBP-0x1C]
	$+575    006E000B     660FD641 10              MOVQ QWORD PTR DS:[ECX+0x10],XMM0
	$+57A    006E0010     8D4D C4                  LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$+57D    006E0013     0F1002                   MOVUPS XMM0,DQWORD PTR DS:[EDX]
	$+580    006E0016     0F1100                   MOVUPS DQWORD PTR DS:[EAX],XMM0
	$+583    006E0019     F30F7E42 10              MOVQ XMM0,QWORD PTR DS:[EDX+0x10]
	$+588    006E001E     660FD640 10              MOVQ QWORD PTR DS:[EAX+0x10],XMM0
	$+58D    006E0023     E8 D8230000              CALL PathOfEx.006E2400
	$+592    006E0028     83C4 30                  ADD ESP,0x30
	$+595    006E002B     8D4D AC                  LEA ECX,DWORD PTR SS:[EBP-0x54]
	$+598    006E002E     E8 AD1FEBFF              CALL PathOfEx.00591FE0
	$+59D    006E0033     E9 E0010000              JMP PathOfEx.006E0218
	$+5A2    006E0038     F6C3 08                  TEST BL,0x8
	$+5A5    006E003B     75 09                    JNZ SHORT PathOfEx.006E0046
	$+5A7    006E003D     83F8 04                  CMP EAX,0x4
	$+5AA    006E0040     0F85 D2010000            JNZ PathOfEx.006E0218
	$+5B0    006E0046     6A 00                    PUSH 0x0
	$+5B2    006E0048     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+5B5    006E004B     E8 00020000              CALL PathOfEx.006E0250
	$+5BA    006E0050     E9 C3010000              JMP PathOfEx.006E0218
	$+5BF    006E0055     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+5C2    006E0058     8B55 A8                  MOV EDX,DWORD PTR SS:[EBP-0x58]
	$+5C5    006E005B     40                       INC EAX
	$+5C6    006E005C     0FAFF0                   IMUL ESI,EAX
	$+5C9    006E005F     8B52 28                  MOV EDX,DWORD PTR DS:[EDX+0x28]
	$+5CC    006E0062     0375 E0                  ADD ESI,DWORD PTR SS:[EBP-0x20]
	$+5CF    006E0065     8975 D8                  MOV DWORD PTR SS:[EBP-0x28],ESI
	$+5D2    006E0068     8B5D D8                  MOV EBX,DWORD PTR SS:[EBP-0x28]
	$+5D5    006E006B     8D0C06                   LEA ECX,DWORD PTR DS:[ESI+EAX]
	$+5D8    006E006E     8D34CA                   LEA ESI,DWORD PTR DS:[EDX+ECX*8]
	$+5DB    006E0071     03F1                     ADD ESI,ECX
	$+5DD    006E0073     8D04DA                   LEA EAX,DWORD PTR DS:[EDX+EBX*8]
	$+5E0    006E0076     0FB64C03 10              MOVZX ECX,BYTE PTR DS:[EBX+EAX+0x10]
	$+5E5    006E007B     0FB64403 07              MOVZX EAX,BYTE PTR DS:[EBX+EAX+0x7]
	$+5EA    006E0080     03C8                     ADD ECX,EAX
	$+5EC    006E0082     0FB646 10                MOVZX EAX,BYTE PTR DS:[ESI+0x10]
	$+5F0    006E0086     03C8                     ADD ECX,EAX
	$+5F2    006E0088     0FB646 07                MOVZX EAX,BYTE PTR DS:[ESI+0x7]
	$+5F6    006E008C     03C8                     ADD ECX,EAX
	$+5F8    006E008E     8A45 0B                  MOV AL,BYTE PTR SS:[EBP+0xB]
	$+5FB    006E0091     A8 01                    TEST AL,0x1
	$+5FD    006E0093     0F85 B5000000            JNZ PathOfEx.006E014E
	$+603    006E0099     83F9 04                  CMP ECX,0x4
	$+606    006E009C     0F84 AC000000            JE PathOfEx.006E014E
	$+60C    006E00A2     8B4D BC                  MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+60F    006E00A5     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+612    006E00A8     50                       PUSH EAX
	$+613    006E00A9     E8 128B0300              CALL PathOfEx.00718BC0
	$+618    006E00AE     8B4D DC                  MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+61B    006E00B1     897D CC                  MOV DWORD PTR SS:[EBP-0x34],EDI
	$+61E    006E00B4     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+621    006E00B7     8D4D A0                  LEA ECX,DWORD PTR SS:[EBP-0x60]
	$+624    006E00BA     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
	$+627    006E00BD     8D45 AC                  LEA EAX,DWORD PTR SS:[EBP-0x54]
	$+62A    006E00C0     8945 D0                  MOV DWORD PTR SS:[EBP-0x30],EAX
	$+62D    006E00C3     8D85 4CFFFFFF            LEA EAX,DWORD PTR SS:[EBP-0xB4]
	$+633    006E00C9     894D C4                  MOV DWORD PTR SS:[EBP-0x3C],ECX
	$+636    006E00CC     8D4D 90                  LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+639    006E00CF     50                       PUSH EAX
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("8b??85??78??8b????85??78??6b??????3b??7d??6b??????3b??7d??8b", -6);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x8b)
	{
		assert(false);
		return false;
	}
#else
	addr_tmp = scan_exe_.ScanAddr("8b??85??78??8b????85??78??6b??????3b??7d??6b??????3b??7d??8b", -6);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x8b)
	{
		assert(false);
		return false;
	}
#endif
	ofs << "pt_cdword	g_c_get_map_data_byte_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$ ==>    007FDBA0     8B4424 08                MOV EAX, DWORD PTR SS:[ESP+0x8]                 ; g_c_player_move_,第一个参数是目标坐标指针
	$+4      007FDBA4     53                       PUSH EBX                                        ; ntdll_1.RtlLeaveCriticalSection
	$+5      007FDBA5     56                       PUSH ESI
	$+6      007FDBA6     8BF1                     MOV ESI, ECX
	$+8      007FDBA8     57                       PUSH EDI
	$+9      007FDBA9     8B4E 10                  MOV ECX, DWORD PTR DS:[ESI+0x10]
	$+C      007FDBAC     8946 34                  MOV DWORD PTR DS:[ESI+0x34], EAX
	$+F      007FDBAF     8B4424 18                MOV EAX, DWORD PTR SS:[ESP+0x18]
	$+13     007FDBB3     8946 38                  MOV DWORD PTR DS:[ESI+0x38], EAX
	$+16     007FDBB6     8079 14 00               CMP BYTE PTR DS:[ECX+0x14], 0x0
	$+1A     007FDBBA     74 07                    JE SHORT PathOfEx.007FDBC3
	$+1C     007FDBBC     B8 01000000              MOV EAX, 0x1
	$+21     007FDBC1     EB 24                    JMP SHORT PathOfEx.007FDBE7
	$+23     007FDBC3     837C24 1C 05             CMP DWORD PTR SS:[ESP+0x1C], 0x5
	$+28     007FDBC8     8D5424 1C                LEA EDX, DWORD PTR SS:[ESP+0x1C]
	$+2C     007FDBCC     8D4424 14                LEA EAX, DWORD PTR SS:[ESP+0x14]
	$+30     007FDBD0     C74424 14 05000000       MOV DWORD PTR SS:[ESP+0x14], 0x5
	$+38     007FDBD8     0F46C2                   CMOVBE EAX, EDX
	$+3B     007FDBDB     BA 01000000              MOV EDX, 0x1
	$+40     007FDBE0     8B00                     MOV EAX, DWORD PTR DS:[EAX]
	$+42     007FDBE2     3BC2                     CMP EAX, EDX
	$+44     007FDBE4     0F42C2                   CMOVB EAX, EDX
	$+47     007FDBE7     8946 3C                  MOV DWORD PTR DS:[ESI+0x3C], EAX
	$+4A     007FDBEA     8A4424 20                MOV AL, BYTE PTR SS:[ESP+0x20]
	$+4E     007FDBEE     8846 26                  MOV BYTE PTR DS:[ESI+0x26], AL
	$+51     007FDBF1     8B4424 10                MOV EAX, DWORD PTR SS:[ESP+0x10]
	$+55     007FDBF5     8946 30                  MOV DWORD PTR DS:[ESI+0x30], EAX
	$+58     007FDBF8     8B4424 24                MOV EAX, DWORD PTR SS:[ESP+0x24]
	$+5C     007FDBFC     8946 40                  MOV DWORD PTR DS:[ESI+0x40], EAX
	$+5F     007FDBFF     8079 14 00               CMP BYTE PTR DS:[ECX+0x14], 0x0
	$+63     007FDC03     75 1B                    JNZ SHORT PathOfEx.007FDC20
	$+65     007FDC05     6A 01                    PUSH 0x1
	$+67     007FDC07     8BCE                     MOV ECX, ESI
	$+69     007FDC09     E8 82040000              CALL PathOfEx.007FE090
	$+6E     007FDC0E     0F57C0                   XORPS XMM0, XMM0
	$+71     007FDC11     D95C24 1C                FSTP DWORD PTR SS:[ESP+0x1C]
	$+75     007FDC15     0F2F4424 1C              COMISS XMM0, DWORD PTR SS:[ESP+0x1C]
	$+7A     007FDC1A     73 04                    JNB SHORT PathOfEx.007FDC20
	$+7C     007FDC1C     32DB                     XOR BL, BL
	$+7E     007FDC1E     EB 09                    JMP SHORT PathOfEx.007FDC29
	$+80     007FDC20     B3 01                    MOV BL, 0x1
	$+82     007FDC22     C746 34 00000000         MOV DWORD PTR DS:[ESI+0x34], 0x0
	$+89     007FDC29     8BCE                     MOV ECX, ESI
	$+8B     007FDC2B     E8 B0FEFFFF              CALL PathOfEx.007FDAE0
	$+90     007FDC30     84C0                     TEST AL, AL
	$+92     007FDC32     74 4D                    JE SHORT PathOfEx.007FDC81
	$+94     007FDC34     807E 24 00               CMP BYTE PTR DS:[ESI+0x24], 0x0
	$+98     007FDC38     74 0F                    JE SHORT PathOfEx.007FDC49
	$+9A     007FDC3A     8BCE                     MOV ECX, ESI
	$+9C     007FDC3C     E8 8F020000              CALL PathOfEx.007FDED0
	$+A1     007FDC41     B0 01                    MOV AL, 0x1
	$+A3     007FDC43     5F                       POP EDI                                         ; PathOfEx.008B81CD
	$+A4     007FDC44     5E                       POP ESI                                         ; PathOfEx.008B81CD
	$+A5     007FDC45     5B                       POP EBX                                         ; PathOfEx.008B81CD
	$+A6     007FDC46     C2 1800                  RETN 0x18
	$+A9     007FDC49     C746 30 00000000         MOV DWORD PTR DS:[ESI+0x30], 0x0
	$+B0     007FDC50     33FF                     XOR EDI, EDI
	$+B2     007FDC52     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+B5     007FDC55     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+B8     007FDC58     C1F8 02                  SAR EAX, 0x2
	$+BB     007FDC5B     85C0                     TEST EAX, EAX
	$+BD     007FDC5D     74 1A                    JE SHORT PathOfEx.007FDC79
	$+BF     007FDC5F     90                       NOP
	$+C0     007FDC60     8B46 14                  MOV EAX, DWORD PTR DS:[ESI+0x14]
	$+C3     007FDC63     8B0CB8                   MOV ECX, DWORD PTR DS:[EAX+EDI*4]
	$+C6     007FDC66     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+C8     007FDC68     FF50 08                  CALL NEAR DWORD PTR DS:[EAX+0x8]
	$+CB     007FDC6B     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+CE     007FDC6E     47                       INC EDI
	$+CF     007FDC6F     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+D2     007FDC72     C1F8 02                  SAR EAX, 0x2
	$+D5     007FDC75     3BF8                     CMP EDI, EAX
	$+D7     007FDC77   ^ 72 E7                    JB SHORT PathOfEx.007FDC60
	$+D9     007FDC79     B0 01                    MOV AL, 0x1
	$+DB     007FDC7B     5F                       POP EDI                                         ; PathOfEx.008B81CD
	$+DC     007FDC7C     5E                       POP ESI                                         ; PathOfEx.008B81CD
	$+DD     007FDC7D     5B                       POP EBX                                         ; PathOfEx.008B81CD
	$+DE     007FDC7E     C2 1800                  RETN 0x18
	$+E1     007FDC81     8BCE                     MOV ECX, ESI
	$+E3     007FDC83     84DB                     TEST BL, BL
	$+E5     007FDC85     74 0D                    JE SHORT PathOfEx.007FDC94
	$+E7     007FDC87     E8 44020000              CALL PathOfEx.007FDED0
	$+EC     007FDC8C     32C0                     XOR AL, AL
	$+EE     007FDC8E     5F                       POP EDI                                         ; PathOfEx.008B81CD
	$+EF     007FDC8F     5E                       POP ESI                                         ; PathOfEx.008B81CD
	$+F0     007FDC90     5B                       POP EBX                                         ; PathOfEx.008B81CD
	$+F1     007FDC91     C2 1800                  RETN 0x18
	$+F4     007FDC94     E8 B7000000              CALL PathOfEx.007FDD50                          ; 调用自动寻路的函数，里面有创建寻路点等东西
	$+F9     007FDC99     5F                       POP EDI                                         ; PathOfEx.008B81CD
	$+FA     007FDC9A     5E                       POP ESI                                         ; PathOfEx.008B81CD
	$+FB     007FDC9B     5B                       POP EBX                                         ; PathOfEx.008B81CD
	$+FC     007FDC9C     C2 1800                  RETN 0x18

	//这个函数由上面的函数最后的位置调用，函数里面有创建寻路点等东西
	$ ==>    007FDD50     83EC 08                  SUB ESP, 0x8                                    ; 寻路相关函数
	$+3      007FDD53     53                       PUSH EBX                                        ; ntdll_1.RtlLeaveCriticalSection
	$+4      007FDD54     56                       PUSH ESI
	$+5      007FDD55     8BF1                     MOV ESI, ECX
	$+7      007FDD57     C64424 0E 00             MOV BYTE PTR SS:[ESP+0xE], 0x0
	$+C      007FDD5C     57                       PUSH EDI
	$+D      007FDD5D     8D4424 12                LEA EAX, DWORD PTR SS:[ESP+0x12]
	$+11     007FDD61     50                       PUSH EAX
	$+12     007FDD62     8D9E 84040000            LEA EBX, DWORD PTR DS:[ESI+0x484]               ; 寻路点个数
	$+18     007FDD68     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C], 0x0
	$+22     007FDD72     53                       PUSH EBX                                        ; ntdll_1.RtlLeaveCriticalSection
	$+23     007FDD73     8D46 44                  LEA EAX, DWORD PTR DS:[ESI+0x44]                ; 寻路点坐标组
	$+26     007FDD76     50                       PUSH EAX
	$+27     007FDD77     FF76 30                  PUSH DWORD PTR DS:[ESI+0x30]                    ; PathOfEx.00DDC61C
	$+2A     007FDD7A     E8 01620300              CALL PathOfEx.00833F80                          ; 创建寻路点
	$+2F     007FDD7F     884424 13                MOV BYTE PTR SS:[ESP+0x13], AL
	$+33     007FDD83     84C0                     TEST AL, AL
	$+35     007FDD85     75 72                    JNZ SHORT PathOfEx.007FDDF9
	$+37     007FDD87     837E 3C 01               CMP DWORD PTR DS:[ESI+0x3C], 0x1
	$+3B     007FDD8B     76 6C                    JBE SHORT PathOfEx.007FDDF9
	$+3D     007FDD8D     8BCE                     MOV ECX, ESI
	$+3F     007FDD8F     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C], 0x0
	$+49     007FDD99     C746 3C 01000000         MOV DWORD PTR DS:[ESI+0x3C], 0x1
	$+50     007FDDA0     E8 3BFDFFFF              CALL PathOfEx.007FDAE0
	$+55     007FDDA5     84C0                     TEST AL, AL
	$+57     007FDDA7     74 50                    JE SHORT PathOfEx.007FDDF9
	$+59     007FDDA9     807E 24 00               CMP BYTE PTR DS:[ESI+0x24], 0x0
	$+5D     007FDDAD     74 10                    JE SHORT PathOfEx.007FDDBF
	$+5F     007FDDAF     8BCE                     MOV ECX, ESI
	$+61     007FDDB1     E8 1A010000              CALL PathOfEx.007FDED0
	$+66     007FDDB6     B0 01                    MOV AL, 0x1
	$+68     007FDDB8     5F                       POP EDI                                         ; 06FB94F8
	$+69     007FDDB9     5E                       POP ESI                                         ; 06FB94F8
	$+6A     007FDDBA     5B                       POP EBX                                         ; 06FB94F8
	$+6B     007FDDBB     83C4 08                  ADD ESP, 0x8
	$+6E     007FDDBE     C3                       RETN
	$+6F     007FDDBF     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+72     007FDDC2     33FF                     XOR EDI, EDI
	$+74     007FDDC4     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+77     007FDDC7     C1F8 02                  SAR EAX, 0x2
	$+7A     007FDDCA     85C0                     TEST EAX, EAX
	$+7C     007FDDCC     74 1B                    JE SHORT PathOfEx.007FDDE9
	$+7E     007FDDCE     66:90                    NOP
	$+80     007FDDD0     8B46 14                  MOV EAX, DWORD PTR DS:[ESI+0x14]
	$+83     007FDDD3     8B0CB8                   MOV ECX, DWORD PTR DS:[EAX+EDI*4]
	$+86     007FDDD6     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+88     007FDDD8     FF50 08                  CALL NEAR DWORD PTR DS:[EAX+0x8]
	$+8B     007FDDDB     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+8E     007FDDDE     47                       INC EDI
	$+8F     007FDDDF     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+92     007FDDE2     C1F8 02                  SAR EAX, 0x2
	$+95     007FDDE5     3BF8                     CMP EDI, EAX
	$+97     007FDDE7   ^ 72 E7                    JB SHORT PathOfEx.007FDDD0
	$+99     007FDDE9     C746 30 00000000         MOV DWORD PTR DS:[ESI+0x30], 0x0
	$+A0     007FDDF0     B0 01                    MOV AL, 0x1
	$+A2     007FDDF2     5F                       POP EDI                                         ; 06FB94F8
	$+A3     007FDDF3     5E                       POP ESI                                         ; 06FB94F8
	$+A4     007FDDF4     5B                       POP EBX                                         ; 06FB94F8
	$+A5     007FDDF5     83C4 08                  ADD ESP, 0x8
	$+A8     007FDDF8     C3                       RETN
	$+A9     007FDDF9     833B 00                  CMP DWORD PTR DS:[EBX], 0x0
	$+AC     007FDDFC     76 74                    JBE SHORT PathOfEx.007FDE72
	$+AE     007FDDFE     807C24 12 00             CMP BYTE PTR SS:[ESP+0x12], 0x0
	$+B3     007FDE03     74 0A                    JE SHORT PathOfEx.007FDE0F
	$+B5     007FDE05     8B46 04                  MOV EAX, DWORD PTR DS:[ESI+0x4]
	$+B8     007FDE08     8B40 20                  MOV EAX, DWORD PTR DS:[EAX+0x20]
	$+BB     007FDE0B     8348 2C 04               OR DWORD PTR DS:[EAX+0x2C], 0x4
	$+BF     007FDE0F     807E 24 00               CMP BYTE PTR DS:[ESI+0x24], 0x0
	$+C3     007FDE13     8B4E 30                  MOV ECX, DWORD PTR DS:[ESI+0x30]                ; PathOfEx.00DDC61C
	$+C6     007FDE16     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+C8     007FDE18     8946 28                  MOV DWORD PTR DS:[ESI+0x28], EAX
	$+CB     007FDE1B     8B41 04                  MOV EAX, DWORD PTR DS:[ECX+0x4]
	$+CE     007FDE1E     8946 2C                  MOV DWORD PTR DS:[ESI+0x2C], EAX
	$+D1     007FDE21     75 2B                    JNZ SHORT PathOfEx.007FDE4E
	$+D3     007FDE23     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+D6     007FDE26     33FF                     XOR EDI, EDI
	$+D8     007FDE28     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+DB     007FDE2B     C1F8 02                  SAR EAX, 0x2
	$+DE     007FDE2E     85C0                     TEST EAX, EAX
	$+E0     007FDE30     74 18                    JE SHORT PathOfEx.007FDE4A
	$+E2     007FDE32     8B46 14                  MOV EAX, DWORD PTR DS:[ESI+0x14]
	$+E5     007FDE35     8B0CB8                   MOV ECX, DWORD PTR DS:[EAX+EDI*4]
	$+E8     007FDE38     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+EA     007FDE3A     FF10                     CALL NEAR DWORD PTR DS:[EAX]
	$+EC     007FDE3C     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+EF     007FDE3F     47                       INC EDI
	$+F0     007FDE40     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+F3     007FDE43     C1F8 02                  SAR EAX, 0x2
	$+F6     007FDE46     3BF8                     CMP EDI, EAX
	$+F8     007FDE48   ^ 72 E8                    JB SHORT PathOfEx.007FDE32
	$+FA     007FDE4A     C646 24 01               MOV BYTE PTR DS:[ESI+0x24], 0x1                 ; Pathfinding写入移动状态
	$+FE     007FDE4E     8B03                     MOV EAX, DWORD PTR DS:[EBX]
	$+100    007FDE50     8BCE                     MOV ECX, ESI
	$+102    007FDE52     48                       DEC EAX
	$+103    007FDE53     C646 25 01               MOV BYTE PTR DS:[ESI+0x25], 0x1
	$+107    007FDE57     50                       PUSH EAX
	$+108    007FDE58     8986 88040000            MOV DWORD PTR DS:[ESI+0x488], EAX
	$+10E    007FDE5E     E8 3DFEFFFF              CALL PathOfEx.007FDCA0
	$+113    007FDE63     8A4424 13                MOV AL, BYTE PTR SS:[ESP+0x13]
	$+117    007FDE67     C646 25 00               MOV BYTE PTR DS:[ESI+0x25], 0x0
	$+11B    007FDE6B     5F                       POP EDI                                         ; PathOfEx.0069DC6D
	$+11C    007FDE6C     5E                       POP ESI                                         ; PathOfEx.0069DC6D
	$+11D    007FDE6D     5B                       POP EBX                                         ; PathOfEx.0069DC6D
	$+11E    007FDE6E     83C4 08                  ADD ESP, 0x8
	$+121    007FDE71     C3                       RETN
	$+122    007FDE72     807E 24 00               CMP BYTE PTR DS:[ESI+0x24], 0x0
	$+126    007FDE76     74 10                    JE SHORT PathOfEx.007FDE88
	$+128    007FDE78     8BCE                     MOV ECX, ESI
	$+12A    007FDE7A     E8 51000000              CALL PathOfEx.007FDED0
	$+12F    007FDE7F     32C0                     XOR AL, AL
	$+131    007FDE81     5F                       POP EDI                                         ; PathOfEx.0069DC6D
	$+132    007FDE82     5E                       POP ESI                                         ; PathOfEx.0069DC6D
	$+133    007FDE83     5B                       POP EBX                                         ; PathOfEx.0069DC6D
	$+134    007FDE84     83C4 08                  ADD ESP, 0x8
	$+137    007FDE87     C3                       RETN
	$+138    007FDE88     C746 30 00000000         MOV DWORD PTR DS:[ESI+0x30], 0x0
	$+13F    007FDE8F     33FF                     XOR EDI, EDI
	$+141    007FDE91     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+144    007FDE94     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+147    007FDE97     C1F8 02                  SAR EAX, 0x2
	$+14A    007FDE9A     85C0                     TEST EAX, EAX
	$+14C    007FDE9C     74 1B                    JE SHORT PathOfEx.007FDEB9
	$+14E    007FDE9E     66:90                    NOP
	$+150    007FDEA0     8B46 14                  MOV EAX, DWORD PTR DS:[ESI+0x14]
	$+153    007FDEA3     8B0CB8                   MOV ECX, DWORD PTR DS:[EAX+EDI*4]
	$+156    007FDEA6     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+158    007FDEA8     FF50 04                  CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+15B    007FDEAB     8B46 18                  MOV EAX, DWORD PTR DS:[ESI+0x18]
	$+15E    007FDEAE     47                       INC EDI
	$+15F    007FDEAF     2B46 14                  SUB EAX, DWORD PTR DS:[ESI+0x14]
	$+162    007FDEB2     C1F8 02                  SAR EAX, 0x2
	$+165    007FDEB5     3BF8                     CMP EDI, EAX
	$+167    007FDEB7   ^ 72 E7                    JB SHORT PathOfEx.007FDEA0
	$+169    007FDEB9     5F                       POP EDI                                         ; PathOfEx.0069DC6D
	$+16A    007FDEBA     5E                       POP ESI                                         ; PathOfEx.0069DC6D
	$+16B    007FDEBB     32C0                     XOR AL, AL
	$+16D    007FDEBD     5B                       POP EBX                                         ; 10CCAB64
	$+16E    007FDEBE     83C4 08                  ADD ESP, 0x8
	$+171    007FDEC1     C3                       RETN
	*/

	//gj3 2017/7/28 15:53
	/*
	$-F1     00693B7F     CC                       INT3
	$-F0     00693B80     B8 6807F200              MOV EAX,PathOfEx.00F20768                    ; ASCII "Pathfinding"
	$-EB     00693B85     C3                       RETN
	$-EA     00693B86     CC                       INT3
	$-E9     00693B87     CC                       INT3
	$-E8     00693B88     CC                       INT3
	$-E7     00693B89     CC                       INT3
	$-E6     00693B8A     CC                       INT3
	$-E5     00693B8B     CC                       INT3
	$-E4     00693B8C     CC                       INT3
	$-E3     00693B8D     CC                       INT3
	$-E2     00693B8E     CC                       INT3
	$-E1     00693B8F     CC                       INT3
	$-E0     00693B90     55                       PUSH EBP
	$-DF     00693B91     8BEC                     MOV EBP,ESP
	$-DD     00693B93     83E4 F8                  AND ESP,0xFFFFFFF8
	$-DA     00693B96     83EC 10                  SUB ESP,0x10
	$-D7     00693B99     53                       PUSH EBX
	$-D6     00693B9A     55                       PUSH EBP
	$-D5     00693B9B     56                       PUSH ESI
	$-D4     00693B9C     8BF1                     MOV ESI,ECX
	$-D2     00693B9E     57                       PUSH EDI
	$-D1     00693B9F     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-CE     00693BA2     8A8E 68040000            MOV CL,BYTE PTR DS:[ESI+0x468]
	$-C8     00693BA8     8B40 2C                  MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$-C5     00693BAB     8B58 10                  MOV EBX,DWORD PTR DS:[EAX+0x10]
	$-C2     00693BAE     8B68 14                  MOV EBP,DWORD PTR DS:[EAX+0x14]
	$-BF     00693BB1     895C24 18                MOV DWORD PTR SS:[ESP+0x18],EBX
	$-BB     00693BB5     896C24 1C                MOV DWORD PTR SS:[ESP+0x1C],EBP
	$-B7     00693BB9     84C9                     TEST CL,CL
	$-B5     00693BBB     79 37                    JNS SHORT PathOfEx.00693BF4
	$-B3     00693BBD     8B8E 8C040000            MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$-AD     00693BC3     2B69 04                  SUB EBP,DWORD PTR DS:[ECX+0x4]
	$-AA     00693BC6     2B19                     SUB EBX,DWORD PTR DS:[ECX]
	$-A8     00693BC8     8BC5                     MOV EAX,EBP
	$-A6     00693BCA     99                       CDQ
	$-A5     00693BCB     8BC8                     MOV ECX,EAX
	$-A3     00693BCD     8BC3                     MOV EAX,EBX
	$-A1     00693BCF     33CA                     XOR ECX,EDX
	$-9F     00693BD1     2BCA                     SUB ECX,EDX
	$-9D     00693BD3     99                       CDQ
	$-9C     00693BD4     33C2                     XOR EAX,EDX
	$-9A     00693BD6     2BC2                     SUB EAX,EDX
	$-98     00693BD8     03C8                     ADD ECX,EAX
	$-96     00693BDA     3B8E 5C040000            CMP ECX,DWORD PTR DS:[ESI+0x45C]
	$-90     00693BE0     77 08                    JA SHORT PathOfEx.00693BEA
	$-8E     00693BE2     3B8E 58040000            CMP ECX,DWORD PTR DS:[ESI+0x458]
	$-88     00693BE8     73 72                    JNB SHORT PathOfEx.00693C5C
	$-86     00693BEA     33C0                     XOR EAX,EAX
	$-84     00693BEC     5F                       POP EDI
	$-83     00693BED     5E                       POP ESI
	$-82     00693BEE     5D                       POP EBP
	$-81     00693BEF     5B                       POP EBX
	$-80     00693BF0     8BE5                     MOV ESP,EBP
	$-7E     00693BF2     5D                       POP EBP
	$-7D     00693BF3     C3                       RETN
	$-7C     00693BF4     8BBE 8C040000            MOV EDI,DWORD PTR DS:[ESI+0x48C]
	$-76     00693BFA     0FBEC9                   MOVSX ECX,CL
	$-73     00693BFD     51                       PUSH ECX
	$-72     00693BFE     FF70 28                  PUSH DWORD PTR DS:[EAX+0x28]
	$-6F     00693C01     8D4424 20                LEA EAX,DWORD PTR SS:[ESP+0x20]
	$-6B     00693C05     894C24 1C                MOV DWORD PTR SS:[ESP+0x1C],ECX
	$-67     00693C09     57                       PUSH EDI
	$-66     00693C0A     50                       PUSH EAX
	$-65     00693C0B     E8 D0CDEFFF              CALL PathOfEx.005909E0
	$-60     00693C10     8B8E 5C040000            MOV ECX,DWORD PTR DS:[ESI+0x45C]
	$-5A     00693C16     83C4 10                  ADD ESP,0x10
	$-57     00693C19     3BC1                     CMP EAX,ECX
	$-55     00693C1B   ^ 77 CD                    JA SHORT PathOfEx.00693BEA
	$-53     00693C1D     3B86 58040000            CMP EAX,DWORD PTR DS:[ESI+0x458]
	$-4D     00693C23   ^ 72 C5                    JB SHORT PathOfEx.00693BEA
	$-4B     00693C25     3B1F                     CMP EBX,DWORD PTR DS:[EDI]
	$-49     00693C27     75 05                    JNZ SHORT PathOfEx.00693C2E
	$-47     00693C29     3B6F 04                  CMP EBP,DWORD PTR DS:[EDI+0x4]
	$-44     00693C2C     74 2E                    JE SHORT PathOfEx.00693C5C
	$-42     00693C2E     394C24 14                CMP DWORD PTR SS:[ESP+0x14],ECX
	$-3E     00693C32     7D 28                    JGE SHORT PathOfEx.00693C5C
	$-3C     00693C34     FFB6 64040000            PUSH DWORD PTR DS:[ESI+0x464]
	$-36     00693C3A     8B4E 04                  MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-33     00693C3D     8D4424 1C                LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-2F     00693C41     FFB6 60040000            PUSH DWORD PTR DS:[ESI+0x460]
	$-29     00693C47     57                       PUSH EDI
	$-28     00693C48     8B49 1C                  MOV ECX,DWORD PTR DS:[ECX+0x1C]
	$-25     00693C4B     50                       PUSH EAX
	$-24     00693C4C     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-22     00693C4E     FF50 0C                  CALL DWORD PTR DS:[EAX+0xC]
	$-1F     00693C51     8BC8                     MOV ECX,EAX
	$-1D     00693C53     E8 B8C4FDFF              CALL PathOfEx.00670110
	$-18     00693C58     84C0                     TEST AL,AL
	$-16     00693C5A   ^ 75 8E                    JNZ SHORT PathOfEx.00693BEA
	$-14     00693C5C     5F                       POP EDI
	$-13     00693C5D     B8 01000000              MOV EAX,0x1
	$-E      00693C62     5E                       POP ESI
	$-D      00693C63     5D                       POP EBP
	$-C      00693C64     5B                       POP EBX
	$-B      00693C65     8BE5                     MOV ESP,EBP
	$-9      00693C67     5D                       POP EBP
	$-8      00693C68     C3                       RETN
	$-7      00693C69     CC                       INT3
	$-6      00693C6A     CC                       INT3
	$-5      00693C6B     CC                       INT3
	$-4      00693C6C     CC                       INT3
	$-3      00693C6D     CC                       INT3
	$-2      00693C6E     CC                       INT3
	$-1      00693C6F     CC                       INT3
	$ ==>    00693C70     8B4424 08                MOV EAX,DWORD PTR SS:[ESP+0x8]               ; g_c_player_move_,第一个参数是目标坐标指针
	$+4      00693C74     53                       PUSH EBX
	$+5      00693C75     56                       PUSH ESI
	$+6      00693C76     8BF1                     MOV ESI,ECX
	$+8      00693C78     57                       PUSH EDI
	$+9      00693C79     8B8E 6C040000            MOV ECX,DWORD PTR DS:[ESI+0x46C]
	$+F      00693C7F     8986 58040000            MOV DWORD PTR DS:[ESI+0x458],EAX
	$+15     00693C85     8B4424 18                MOV EAX,DWORD PTR SS:[ESP+0x18]
	$+19     00693C89     8986 5C040000            MOV DWORD PTR DS:[ESI+0x45C],EAX
	$+1F     00693C8F     8079 14 00               CMP BYTE PTR DS:[ECX+0x14],0x0
	$+23     00693C93     74 07                    JE SHORT PathOfEx.00693C9C
	$+25     00693C95     B8 01000000              MOV EAX,0x1
	$+2A     00693C9A     EB 24                    JMP SHORT PathOfEx.00693CC0
	$+2C     00693C9C     837C24 1C 05             CMP DWORD PTR SS:[ESP+0x1C],0x5
	$+31     00693CA1     8D5424 1C                LEA EDX,DWORD PTR SS:[ESP+0x1C]
	$+35     00693CA5     8D4424 14                LEA EAX,DWORD PTR SS:[ESP+0x14]
	$+39     00693CA9     C74424 14 05000000       MOV DWORD PTR SS:[ESP+0x14],0x5
	$+41     00693CB1     0F46C2                   CMOVBE EAX,EDX
	$+44     00693CB4     BA 01000000              MOV EDX,0x1
	$+49     00693CB9     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$+4B     00693CBB     3BC2                     CMP EAX,EDX
	$+4D     00693CBD     0F42C2                   CMOVB EAX,EDX
	$+50     00693CC0     8986 60040000            MOV DWORD PTR DS:[ESI+0x460],EAX
	$+56     00693CC6     8A4424 20                MOV AL,BYTE PTR SS:[ESP+0x20]
	$+5A     00693CCA     8886 68040000            MOV BYTE PTR DS:[ESI+0x468],AL
	$+60     00693CD0     8B4424 10                MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+64     00693CD4     8986 8C040000            MOV DWORD PTR DS:[ESI+0x48C],EAX
	$+6A     00693CDA     8B4424 24                MOV EAX,DWORD PTR SS:[ESP+0x24]
	$+6E     00693CDE     8986 64040000            MOV DWORD PTR DS:[ESI+0x464],EAX
	$+74     00693CE4     8079 14 00               CMP BYTE PTR DS:[ECX+0x14],0x0
	$+78     00693CE8     75 1B                    JNZ SHORT PathOfEx.00693D05
	$+7A     00693CEA     6A 01                    PUSH 0x1
	$+7C     00693CEC     8BCE                     MOV ECX,ESI
	$+7E     00693CEE     E8 4D050000              CALL PathOfEx.00694240
	$+83     00693CF3     0F57C0                   XORPS XMM0,XMM0
	$+86     00693CF6     D95C24 1C                FSTP DWORD PTR SS:[ESP+0x1C]
	$+8A     00693CFA     0F2F4424 1C              COMISS XMM0,DWORD PTR SS:[ESP+0x1C]
	$+8F     00693CFF     73 04                    JNB SHORT PathOfEx.00693D05
	$+91     00693D01     32DB                     XOR BL,BL
	$+93     00693D03     EB 0C                    JMP SHORT PathOfEx.00693D11
	$+95     00693D05     B3 01                    MOV BL,0x1
	$+97     00693D07     C786 58040000 00000000   MOV DWORD PTR DS:[ESI+0x458],0x0
	$+A1     00693D11     8BCE                     MOV ECX,ESI
	$+A3     00693D13     E8 78FEFFFF              CALL PathOfEx.00693B90
	$+A8     00693D18     84C0                     TEST AL,AL
	$+AA     00693D1A     74 61                    JE SHORT PathOfEx.00693D7D
	$+AC     00693D1C     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+B3     00693D23     74 0F                    JE SHORT PathOfEx.00693D34
	$+B5     00693D25     8BCE                     MOV ECX,ESI
	$+B7     00693D27     E8 04030000              CALL PathOfEx.00694030
	$+BC     00693D2C     B0 01                    MOV AL,0x1
	$+BE     00693D2E     5F                       POP EDI
	$+BF     00693D2F     5E                       POP ESI
	$+C0     00693D30     5B                       POP EBX
	$+C1     00693D31     C2 1800                  RETN 0x18
	$+C4     00693D34     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+CE     00693D3E     33FF                     XOR EDI,EDI
	$+D0     00693D40     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+D6     00693D46     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+DC     00693D4C     C1F8 02                  SAR EAX,0x2
	$+DF     00693D4F     85C0                     TEST EAX,EAX
	$+E1     00693D51     74 22                    JE SHORT PathOfEx.00693D75
	$+E3     00693D53     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+E9     00693D59     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+EC     00693D5C     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+EE     00693D5E     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]
	$+F1     00693D61     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+F7     00693D67     47                       INC EDI
	$+F8     00693D68     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+FE     00693D6E     C1F8 02                  SAR EAX,0x2
	$+101    00693D71     3BF8                     CMP EDI,EAX
	$+103    00693D73   ^ 72 DE                    JB SHORT PathOfEx.00693D53
	$+105    00693D75     B0 01                    MOV AL,0x1
	$+107    00693D77     5F                       POP EDI
	$+108    00693D78     5E                       POP ESI
	$+109    00693D79     5B                       POP EBX
	$+10A    00693D7A     C2 1800                  RETN 0x18
	$+10D    00693D7D     8BCE                     MOV ECX,ESI
	$+10F    00693D7F     84DB                     TEST BL,BL
	$+111    00693D81     74 0D                    JE SHORT PathOfEx.00693D90
	$+113    00693D83     E8 A8020000              CALL PathOfEx.00694030
	$+118    00693D88     32C0                     XOR AL,AL
	$+11A    00693D8A     5F                       POP EDI
	$+11B    00693D8B     5E                       POP ESI
	$+11C    00693D8C     5B                       POP EBX
	$+11D    00693D8D     C2 1800                  RETN 0x18
	$+120    00693D90     E8 BB000000              CALL PathOfEx.00693E50
	$+125    00693D95     5F                       POP EDI
	$+126    00693D96     5E                       POP ESI
	$+127    00693D97     5B                       POP EBX
	$+128    00693D98     C2 1800                  RETN 0x18
	$+12B    00693D9B     CC                       INT3
	$+12C    00693D9C     CC                       INT3
	$+12D    00693D9D     CC                       INT3
	$+12E    00693D9E     CC                       INT3
	$+12F    00693D9F     CC                       INT3
	$+130    00693DA0     83EC 0C                  SUB ESP,0xC
	$+133    00693DA3     56                       PUSH ESI
	$+134    00693DA4     6A 01                    PUSH 0x1
	$+136    00693DA6     8BF1                     MOV ESI,ECX
	$+138    00693DA8     E8 93040000              CALL PathOfEx.00694240
	$+13D    00693DAD     80BE 69040000 00         CMP BYTE PTR DS:[ESI+0x469],0x0
	$+144    00693DB4     D95C24 08                FSTP DWORD PTR SS:[ESP+0x8]
	$+148    00693DB8     F3:0F104424 08           MOVSS XMM0,DWORD PTR SS:[ESP+0x8]
	$+14E    00693DBE     6A 00                    PUSH 0x0
	$+150    00693DC0     74 34                    JE SHORT PathOfEx.00693DF6
	$+152    00693DC2     83EC 28                  SUB ESP,0x28
	$+155    00693DC5     8BC4                     MOV EAX,ESP
	$+157    00693DC7     C700 4035F200            MOV DWORD PTR DS:[EAX],PathOfEx.00F23540
	$+15D    00693DCD     8940 24                  MOV DWORD PTR DS:[EAX+0x24],EAX
	$+160    00693DD0     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+163    00693DD3     8B48 2C                  MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$+166    00693DD6     8B4424 40                MOV EAX,DWORD PTR SS:[ESP+0x40]
	$+16A    00693DDA     51                       PUSH ECX
	$+16B    00693DDB     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+170    00693DE0     8D04C5 0C000000          LEA EAX,DWORD PTR DS:[EAX*8+0xC]
	$+177    00693DE7     03C6                     ADD EAX,ESI
	$+179    00693DE9     50                       PUSH EAX
	$+17A    00693DEA     E8 C13EFDFF              CALL PathOfEx.00667CB0
	$+17F    00693DEF     5E                       POP ESI
	$+180    00693DF0     83C4 0C                  ADD ESP,0xC
	$+183    00693DF3     C2 0400                  RETN 0x4
	$+186    00693DF6     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+189    00693DF9     8B48 2C                  MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$+18C    00693DFC     8B41 0C                  MOV EAX,DWORD PTR DS:[ECX+0xC]
	$+18F    00693DFF     FF70 10                  PUSH DWORD PTR DS:[EAX+0x10]
	$+192    00693E02     83EC 28                  SUB ESP,0x28
	$+195    00693E05     8BC4                     MOV EAX,ESP
	$+197    00693E07     51                       PUSH ECX
	$+198    00693E08     C700 70FEF100            MOV DWORD PTR DS:[EAX],PathOfEx.00F1FE70
	$+19E    00693E0E     8940 24                  MOV DWORD PTR DS:[EAX+0x24],EAX
	$+1A1    00693E11     8B4424 48                MOV EAX,DWORD PTR SS:[ESP+0x48]
	$+1A5    00693E15     F3:0F110424              MOVSS DWORD PTR SS:[ESP],XMM0
	$+1AA    00693E1A     68 FFFFFF7F              PUSH 0x7FFFFFFF
	$+1AF    00693E1F     8D04C5 0C000000          LEA EAX,DWORD PTR DS:[EAX*8+0xC]
	$+1B6    00693E26     03C6                     ADD EAX,ESI
	$+1B8    00693E28     50                       PUSH EAX
	$+1B9    00693E29     E8 C23FFDFF              CALL PathOfEx.00667DF0
	$+1BE    00693E2E     5E                       POP ESI
	$+1BF    00693E2F     83C4 0C                  ADD ESP,0xC
	$+1C2    00693E32     C2 0400                  RETN 0x4
	$+1C5    00693E35     CC                       INT3
	$+1C6    00693E36     CC                       INT3
	$+1C7    00693E37     CC                       INT3
	$+1C8    00693E38     CC                       INT3
	$+1C9    00693E39     CC                       INT3
	$+1CA    00693E3A     CC                       INT3
	$+1CB    00693E3B     CC                       INT3
	$+1CC    00693E3C     CC                       INT3
	$+1CD    00693E3D     CC                       INT3
	$+1CE    00693E3E     CC                       INT3
	$+1CF    00693E3F     CC                       INT3
	$+1D0    00693E40     E9 EB010000              JMP PathOfEx.00694030
	$+1D5    00693E45     CC                       INT3
	$+1D6    00693E46     CC                       INT3
	$+1D7    00693E47     CC                       INT3
	$+1D8    00693E48     CC                       INT3
	$+1D9    00693E49     CC                       INT3
	$+1DA    00693E4A     CC                       INT3
	$+1DB    00693E4B     CC                       INT3
	$+1DC    00693E4C     CC                       INT3
	$+1DD    00693E4D     CC                       INT3
	$+1DE    00693E4E     CC                       INT3
	$+1DF    00693E4F     CC                       INT3
	$+1E0    00693E50     83EC 08                  SUB ESP,0x8
	$+1E3    00693E53     53                       PUSH EBX
	$+1E4    00693E54     56                       PUSH ESI
	$+1E5    00693E55     8BF1                     MOV ESI,ECX
	$+1E7    00693E57     C64424 0E 00             MOV BYTE PTR SS:[ESP+0xE],0x0
	$+1EC    00693E5C     57                       PUSH EDI
	$+1ED    00693E5D     8D4424 12                LEA EAX,DWORD PTR SS:[ESP+0x12]
	$+1F1    00693E61     50                       PUSH EAX
	$+1F2    00693E62     8D9E 54040000            LEA EBX,DWORD PTR DS:[ESI+0x454]
	$+1F8    00693E68     C786 94040000 00000000   MOV DWORD PTR DS:[ESI+0x494],0x0
	$+202    00693E72     53                       PUSH EBX
	$+203    00693E73     8D46 14                  LEA EAX,DWORD PTR DS:[ESI+0x14]
	$+206    00693E76     50                       PUSH EAX
	$+207    00693E77     FFB6 8C040000            PUSH DWORD PTR DS:[ESI+0x48C]
	$+20D    00693E7D     8D4E 10                  LEA ECX,DWORD PTR DS:[ESI+0x10]
	$+210    00693E80     E8 8B320300              CALL PathOfEx.006C7110
	$+215    00693E85     884424 13                MOV BYTE PTR SS:[ESP+0x13],AL
	$+219    00693E89     84C0                     TEST AL,AL
	$+21B    00693E8B     0F85 94000000            JNZ PathOfEx.00693F25
	$+221    00693E91     83BE 60040000 01         CMP DWORD PTR DS:[ESI+0x460],0x1
	$+228    00693E98     0F86 87000000            JBE PathOfEx.00693F25
	$+22E    00693E9E     8BCE                     MOV ECX,ESI
	$+230    00693EA0     C786 94040000 00000000   MOV DWORD PTR DS:[ESI+0x494],0x0
	$+23A    00693EAA     C786 60040000 01000000   MOV DWORD PTR DS:[ESI+0x460],0x1
	$+244    00693EB4     E8 D7FCFFFF              CALL PathOfEx.00693B90
	$+249    00693EB9     84C0                     TEST AL,AL
	$+24B    00693EBB     74 68                    JE SHORT PathOfEx.00693F25
	$+24D    00693EBD     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+254    00693EC4     74 10                    JE SHORT PathOfEx.00693ED6
	$+256    00693EC6     8BCE                     MOV ECX,ESI
	$+258    00693EC8     E8 63010000              CALL PathOfEx.00694030
	$+25D    00693ECD     B0 01                    MOV AL,0x1
	$+25F    00693ECF     5F                       POP EDI
	$+260    00693ED0     5E                       POP ESI
	$+261    00693ED1     5B                       POP EBX
	$+262    00693ED2     83C4 08                  ADD ESP,0x8
	$+265    00693ED5     C3                       RETN
	$+266    00693ED6     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+26C    00693EDC     33FF                     XOR EDI,EDI
	$+26E    00693EDE     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+274    00693EE4     C1F8 02                  SAR EAX,0x2
	$+277    00693EE7     85C0                     TEST EAX,EAX
	$+279    00693EE9     74 27                    JE SHORT PathOfEx.00693F12
	$+27B    00693EEB     0F1F4400 00              NOP DWORD PTR DS:[EAX+EAX]
	$+280    00693EF0     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+286    00693EF6     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+289    00693EF9     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+28B    00693EFB     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]
	$+28E    00693EFE     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+294    00693F04     47                       INC EDI
	$+295    00693F05     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+29B    00693F0B     C1F8 02                  SAR EAX,0x2
	$+29E    00693F0E     3BF8                     CMP EDI,EAX
	$+2A0    00693F10   ^ 72 DE                    JB SHORT PathOfEx.00693EF0
	$+2A2    00693F12     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+2AC    00693F1C     B0 01                    MOV AL,0x1
	$+2AE    00693F1E     5F                       POP EDI
	$+2AF    00693F1F     5E                       POP ESI
	$+2B0    00693F20     5B                       POP EBX
	$+2B1    00693F21     83C4 08                  ADD ESP,0x8
	$+2B4    00693F24     C3                       RETN
	$+2B5    00693F25     833B 00                  CMP DWORD PTR DS:[EBX],0x0
	$+2B8    00693F28     0F86 98000000            JBE PathOfEx.00693FC6
	$+2BE    00693F2E     807C24 12 00             CMP BYTE PTR SS:[ESP+0x12],0x0
	$+2C3    00693F33     74 0A                    JE SHORT PathOfEx.00693F3F
	$+2C5    00693F35     8B46 04                  MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+2C8    00693F38     8B40 2C                  MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+2CB    00693F3B     8348 2C 04               OR DWORD PTR DS:[EAX+0x2C],0x4
	$+2CF    00693F3F     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+2D6    00693F46     8B8E 8C040000            MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$+2DC    00693F4C     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+2DE    00693F4E     8986 84040000            MOV DWORD PTR DS:[ESI+0x484],EAX
	$+2E4    00693F54     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+2E7    00693F57     8986 88040000            MOV DWORD PTR DS:[ESI+0x488],EAX
	$+2ED    00693F5D     75 3D                    JNZ SHORT PathOfEx.00693F9C
	$+2EF    00693F5F     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+2F5    00693F65     33FF                     XOR EDI,EDI
	$+2F7    00693F67     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+2FD    00693F6D     C1F8 02                  SAR EAX,0x2
	$+300    00693F70     85C0                     TEST EAX,EAX
	$+302    00693F72     74 21                    JE SHORT PathOfEx.00693F95
	$+304    00693F74     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+30A    00693F7A     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+30D    00693F7D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+30F    00693F7F     FF10                     CALL DWORD PTR DS:[EAX]
	$+311    00693F81     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+317    00693F87     47                       INC EDI
	$+318    00693F88     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+31E    00693F8E     C1F8 02                  SAR EAX,0x2
	$+321    00693F91     3BF8                     CMP EDI,EAX
	$+323    00693F93   ^ 72 DF                    JB SHORT PathOfEx.00693F74
	$+325    00693F95     C686 80040000 01         MOV BYTE PTR DS:[ESI+0x480],0x1
	$+32C    00693F9C     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$+32E    00693F9E     8BCE                     MOV ECX,ESI
	$+330    00693FA0     48                       DEC EAX
	$+331    00693FA1     C686 81040000 01         MOV BYTE PTR DS:[ESI+0x481],0x1
	$+338    00693FA8     50                       PUSH EAX
	$+339    00693FA9     8986 90040000            MOV DWORD PTR DS:[ESI+0x490],EAX
	$+33F    00693FAF     E8 ECFDFFFF              CALL PathOfEx.00693DA0
	$+344    00693FB4     8A4424 13                MOV AL,BYTE PTR SS:[ESP+0x13]
	$+348    00693FB8     C686 81040000 00         MOV BYTE PTR DS:[ESI+0x481],0x0
	$+34F    00693FBF     5F                       POP EDI
	$+350    00693FC0     5E                       POP ESI
	$+351    00693FC1     5B                       POP EBX
	$+352    00693FC2     83C4 08                  ADD ESP,0x8
	$+355    00693FC5     C3                       RETN
	$+356    00693FC6     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+35D    00693FCD     74 10                    JE SHORT PathOfEx.00693FDF
	$+35F    00693FCF     8BCE                     MOV ECX,ESI
	$+361    00693FD1     E8 5A000000              CALL PathOfEx.00694030
	$+366    00693FD6     32C0                     XOR AL,AL
	$+368    00693FD8     5F                       POP EDI
	$+369    00693FD9     5E                       POP ESI
	$+36A    00693FDA     5B                       POP EBX
	$+36B    00693FDB     83C4 08                  ADD ESP,0x8
	$+36E    00693FDE     C3                       RETN
	$+36F    00693FDF     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+379    00693FE9     33FF                     XOR EDI,EDI
	$+37B    00693FEB     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+381    00693FF1     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+387    00693FF7     C1F8 02                  SAR EAX,0x2
	$+38A    00693FFA     85C0                     TEST EAX,EAX
	$+38C    00693FFC     74 24                    JE SHORT PathOfEx.00694022
	$+38E    00693FFE     66:90                    NOP
	$+390    00694000     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+396    00694006     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+399    00694009     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+39B    0069400B     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+39E    0069400E     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+3A4    00694014     47                       INC EDI
	$+3A5    00694015     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+3AB    0069401B     C1F8 02                  SAR EAX,0x2
	$+3AE    0069401E     3BF8                     CMP EDI,EAX
	$+3B0    00694020   ^ 72 DE                    JB SHORT PathOfEx.00694000
	$+3B2    00694022     5F                       POP EDI
	$+3B3    00694023     5E                       POP ESI
	$+3B4    00694024     32C0                     XOR AL,AL
	$+3B6    00694026     5B                       POP EBX
	$+3B7    00694027     83C4 08                  ADD ESP,0x8
	$+3BA    0069402A     C3                       RETN
	$+3BB    0069402B     CC                       INT3
	$+3BC    0069402C     CC                       INT3
	$+3BD    0069402D     CC                       INT3
	$+3BE    0069402E     CC                       INT3
	$+3BF    0069402F     CC                       INT3
	$+3C0    00694030     56                       PUSH ESI
	$+3C1    00694031     8BF1                     MOV ESI,ECX
	$+3C3    00694033     57                       PUSH EDI
	$+3C4    00694034     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+3CB    0069403B     0F84 B1000000            JE PathOfEx.006940F2
	$+3D1    00694041     8B4E 04                  MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+3D4    00694044     C686 81040000 01         MOV BYTE PTR DS:[ESI+0x481],0x1
	$+3DB    0069404B     8B49 2C                  MOV ECX,DWORD PTR DS:[ECX+0x2C]
	$+3DE    0069404E     E8 3D40FDFF              CALL PathOfEx.00668090
	$+3E3    00694053     8BCE                     MOV ECX,ESI
	$+3E5    00694055     C686 81040000 00         MOV BYTE PTR DS:[ESI+0x481],0x0
	$+3EC    0069405C     E8 2FFBFFFF              CALL PathOfEx.00693B90
	$+3F1    00694061     33FF                     XOR EDI,EDI
	$+3F3    00694063     C686 80040000 00         MOV BYTE PTR DS:[ESI+0x480],0x0
	$+3FA    0069406A     C786 54040000 00000000   MOV DWORD PTR DS:[ESI+0x454],0x0
	$+404    00694074     84C0                     TEST AL,AL
	$+406    00694076     C786 8C040000 00000000   MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+410    00694080     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+416    00694086     74 32                    JE SHORT PathOfEx.006940BA
	$+418    00694088     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+41E    0069408E     C1F8 02                  SAR EAX,0x2
	$+421    00694091     85C0                     TEST EAX,EAX
	$+423    00694093     74 5D                    JE SHORT PathOfEx.006940F2
	$+425    00694095     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+42B    0069409B     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+42E    0069409E     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+430    006940A0     FF50 08                  CALL DWORD PTR DS:[EAX+0x8]
	$+433    006940A3     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+439    006940A9     47                       INC EDI
	$+43A    006940AA     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+440    006940B0     C1F8 02                  SAR EAX,0x2
	$+443    006940B3     3BF8                     CMP EDI,EAX
	$+445    006940B5   ^ 72 DE                    JB SHORT PathOfEx.00694095
	$+447    006940B7     5F                       POP EDI
	$+448    006940B8     5E                       POP ESI
	$+449    006940B9     C3                       RETN
	$+44A    006940BA     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+450    006940C0     C1F8 02                  SAR EAX,0x2
	$+453    006940C3     85C0                     TEST EAX,EAX
	$+455    006940C5     74 2B                    JE SHORT PathOfEx.006940F2
	$+457    006940C7     66:0F1F8400 00000000     NOP WORD PTR DS:[EAX+EAX]
	$+460    006940D0     8B86 70040000            MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+466    006940D6     8B0CB8                   MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+469    006940D9     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+46B    006940DB     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+46E    006940DE     8B86 74040000            MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+474    006940E4     47                       INC EDI
	$+475    006940E5     2B86 70040000            SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+47B    006940EB     C1F8 02                  SAR EAX,0x2
	$+47E    006940EE     3BF8                     CMP EDI,EAX
	$+480    006940F0   ^ 72 DE                    JB SHORT PathOfEx.006940D0
	$+482    006940F2     5F                       POP EDI
	$+483    006940F3     5E                       POP ESI
	$+484    006940F4     C3                       RETN
	$+485    006940F5     CC                       INT3
	$+486    006940F6     CC                       INT3
	$+487    006940F7     CC                       INT3
	$+488    006940F8     CC                       INT3
	$+489    006940F9     CC                       INT3
	$+48A    006940FA     CC                       INT3
	$+48B    006940FB     CC                       INT3
	$+48C    006940FC     CC                       INT3
	$+48D    006940FD     CC                       INT3
	$+48E    006940FE     CC                       INT3
	$+48F    006940FF     CC                       INT3
	$+490    00694100     56                       PUSH ESI
	$+491    00694101     8BF1                     MOV ESI,ECX
	$+493    00694103     80BE 74040000 00         CMP BYTE PTR DS:[ESI+0x474],0x0
	$+49A    0069410A     74 3B                    JE SHORT PathOfEx.00694147
	$+49C    0069410C     57                       PUSH EDI
	$+49D    0069410D     8D4E F4                  LEA ECX,DWORD PTR DS:[ESI-0xC]
	$+4A0    00694110     E8 7BFAFFFF              CALL PathOfEx.00693B90
	$+4A5    00694115     8D4E F4                  LEA ECX,DWORD PTR DS:[ESI-0xC]
	$+4A8    00694118     84C0                     TEST AL,AL
	$+4AA    0069411A     74 0A                    JE SHORT PathOfEx.00694126
	$+4AC    0069411C     E8 0FFFFFFF              CALL PathOfEx.00694030
	$+4B1    00694121     5F                       POP EDI
	$+4B2    00694122     5E                       POP ESI
	$+4B3    00694123     C2 0400                  RETN 0x4
	$+4B6    00694126     FF8E 84040000            DEC DWORD PTR DS:[ESI+0x484]
	$+4BC    0069412C     8B86 84040000            MOV EAX,DWORD PTR DS:[ESI+0x484]
	$+4C2    00694132     85C0                     TEST EAX,EAX
	$+4C4    00694134     7E 0B                    JLE SHORT PathOfEx.00694141
	$+4C6    00694136     50                       PUSH EAX
	$+4C7    00694137     E8 64FCFFFF              CALL PathOfEx.00693DA0
	$+4CC    0069413C     5F                       POP EDI
	$+4CD    0069413D     5E                       POP ESI
	$+4CE    0069413E     C2 0400                  RETN 0x4
	$+4D1    00694141     E8 0AFDFFFF              CALL PathOfEx.00693E50
	$+4D6    00694146     5F                       POP EDI
	$+4D7    00694147     5E                       POP ESI
	$+4D8    00694148     C2 0400                  RETN 0x4
	$+4DB    0069414B     CC                       INT3
	$+4DC    0069414C     CC                       INT3
	$+4DD    0069414D     CC                       INT3
	$+4DE    0069414E     CC                       INT3
	$+4DF    0069414F     CC                       INT3
	$+4E0    00694150     83C1 F4                  ADD ECX,-0xC
	$+4E3    00694153     E8 D8FEFFFF              CALL PathOfEx.00694030
	$+4E8    00694158     C2 0400                  RETN 0x4
	$+4EB    0069415B     CC                       INT3
	$+4EC    0069415C     CC                       INT3
	$+4ED    0069415D     CC                       INT3
	$+4EE    0069415E     CC                       INT3
	$+4EF    0069415F     CC                       INT3
	$+4F0    00694160     80B9 75040000 00         CMP BYTE PTR DS:[ECX+0x475],0x0
	$+4F7    00694167     75 08                    JNZ SHORT PathOfEx.00694171
	$+4F9    00694169     83C1 F4                  ADD ECX,-0xC
	$+4FC    0069416C     E8 BFFEFFFF              CALL PathOfEx.00694030
	$+501    00694171     C2 0400                  RETN 0x4
	$+504    00694174     CC                       INT3
	$+505    00694175     CC                       INT3
	$+506    00694176     CC                       INT3
	$+507    00694177     CC                       INT3
	$+508    00694178     CC                       INT3
	$+509    00694179     CC                       INT3
	$+50A    0069417A     CC                       INT3
	$+50B    0069417B     CC                       INT3
	$+50C    0069417C     CC                       INT3
	$+50D    0069417D     CC                       INT3
	$+50E    0069417E     CC                       INT3
	$+50F    0069417F     CC                       INT3
	$+510    00694180     55                       PUSH EBP
	$+511    00694181     8BEC                     MOV EBP,ESP
	$+513    00694183     83E4 F8                  AND ESP,0xFFFFFFF8
	$+516    00694186     51                       PUSH ECX
	$+517    00694187     F3:0F1045 08             MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+51C    0069418C     56                       PUSH ESI
	$+51D    0069418D     8BF1                     MOV ESI,ECX
	$+51F    0069418F     80BE 80040000 00         CMP BYTE PTR DS:[ESI+0x480],0x0
	$+526    00694196     F3:0F5886 94040000       ADDSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+52E    0069419E     F3:0F1186 94040000       MOVSS DWORD PTR DS:[ESI+0x494],XMM0
	$+536    006941A6     0F84 82000000            JE PathOfEx.0069422E
	$+53C    006941AC     E8 DFF9FFFF              CALL PathOfEx.00693B90
	$+541    006941B1     84C0                     TEST AL,AL
	$+543    006941B3     74 0E                    JE SHORT PathOfEx.006941C3
	$+545    006941B5     8BCE                     MOV ECX,ESI
	$+547    006941B7     E8 74FEFFFF              CALL PathOfEx.00694030
	$+54C    006941BC     5E                       POP ESI
	$+54D    006941BD     8BE5                     MOV ESP,EBP
	$+54F    006941BF     5D                       POP EBP
	$+550    006941C0     C2 0400                  RETN 0x4
	$+553    006941C3     F3:0F1086 94040000       MOVSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+55B    006941CB     0F2F05 A8B1F700          COMISS XMM0,DWORD PTR DS:[0xF7B1A8]
	$+562    006941D2     72 1B                    JB SHORT PathOfEx.006941EF
	$+564    006941D4     8B8E 8C040000            MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$+56A    006941DA     8B86 84040000            MOV EAX,DWORD PTR DS:[ESI+0x484]
	$+570    006941E0     3B01                     CMP EAX,DWORD PTR DS:[ECX]
	$+572    006941E2     75 43                    JNZ SHORT PathOfEx.00694227
	$+574    006941E4     8B86 88040000            MOV EAX,DWORD PTR DS:[ESI+0x488]
	$+57A    006941EA     3B41 04                  CMP EAX,DWORD PTR DS:[ECX+0x4]
	$+57D    006941ED     75 38                    JNZ SHORT PathOfEx.00694227
	$+57F    006941EF     8B86 8C040000            MOV EAX,DWORD PTR DS:[ESI+0x48C]
	$+585    006941F5     8B96 84040000            MOV EDX,DWORD PTR DS:[ESI+0x484]
	$+58B    006941FB     8B8E 88040000            MOV ECX,DWORD PTR DS:[ESI+0x488]
	$+591    00694201     2B10                     SUB EDX,DWORD PTR DS:[EAX]
	$+593    00694203     2B48 04                  SUB ECX,DWORD PTR DS:[EAX+0x4]
	$+596    00694206     0F2F05 ACB0F700          COMISS XMM0,DWORD PTR DS:[0xF7B0AC]
	$+59D    0069420D     72 1F                    JB SHORT PathOfEx.0069422E
	$+59F    0069420F     0FAFC9                   IMUL ECX,ECX
	$+5A2    00694212     0FAFD2                   IMUL EDX,EDX
	$+5A5    00694215     03CA                     ADD ECX,EDX
	$+5A7    00694217     66:0F6EC1                MOVD MM0,ECX
	$+5AB    0069421B     0F5BC0                   CVTDQ2PS XMM0,XMM0
	$+5AE    0069421E     0F2F05 A0B7F700          COMISS XMM0,DWORD PTR DS:[0xF7B7A0]
	$+5B5    00694225     72 07                    JB SHORT PathOfEx.0069422E
	$+5B7    00694227     8BCE                     MOV ECX,ESI
	$+5B9    00694229     E8 22FCFFFF              CALL PathOfEx.00693E50
	$+5BE    0069422E     5E                       POP ESI
	$+5BF    0069422F     8BE5                     MOV ESP,EBP
	$+5C1    00694231     5D                       POP EBP
	$+5C2    00694232     C2 0400                  RETN 0x4
	$+5C5    00694235     CC                       INT3
	*/
#ifdef VER_GJ3_
	addr_tmp = scan_exe_.ScanAddr("8b??????53568b??578b??????????89??????????8b??????89??????????80??????74??b8", 0);
#else
	//addr_tmp = scan_exe_.ScanAddr("8b??????53568b??578b????89????8b??????89????80", 0);
addr_tmp = scan_exe_.ScanAddr("8b??????53568b??578b??????????89??????????8b??????89??????????80??????74??b8", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_player_move_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;

	/*
	$-148    006D2AA0     55                    PUSH EBP
	$-147    006D2AA1     8BEC                  MOV EBP, ESP
	$-145    006D2AA3     83E4 F8               AND ESP, -0x8
	$-142    006D2AA6     51                    PUSH ECX
	$-141    006D2AA7     803D B5FBFE00 00      CMP BYTE PTR DS:[0xFEFBB5], 0x0
	$-13A    006D2AAE     56                    PUSH ESI
	$-139    006D2AAF     8BF1                  MOV ESI, ECX
	$-137    006D2AB1     74 68                 JE SHORT PathOfEx.006D2B1B
	$-135    006D2AB3     6A 10                 PUSH 0x10
	$-133    006D2AB5     E8 A8FD5900           CALL PathOfEx.00C72862
	$-12E    006D2ABA     F3:0F1045 08          MOVSS XMM0, DWORD PTR SS:[EBP+0x8]
	$-129    006D2ABF     83C4 04               ADD ESP, 0x4
	$-126    006D2AC2     894424 04             MOV DWORD PTR SS:[ESP+0x4], EAX          ; PathOfEx.00EA23C0
	$-122    006D2AC6     8970 08               MOV DWORD PTR DS:[EAX+0x8], ESI
	$-11F    006D2AC9     8D48 04               LEA ECX, DWORD PTR DS:[EAX+0x4]
	$-11C    006D2ACC     F3:0F1140 0C          MOVSS DWORD PTR DS:[EAX+0xC], XMM0
	$-117    006D2AD1     C700 5C23EA00         MOV DWORD PTR DS:[EAX], PathOfEx.00EA235>
	$-111    006D2AD7     33C0                  XOR EAX, EAX                             ; PathOfEx.00EA23C0
	$-10F    006D2AD9     8701                  XCHG DWORD PTR DS:[ECX], EAX             ; PathOfEx.00EA23C0
	$-10D    006D2ADB     8BC1                  MOV EAX, ECX
	$-10B    006D2ADD     BA FC660001           MOV EDX, PathOfEx.010066FC
	$-106    006D2AE2     8702                  XCHG DWORD PTR DS:[EDX], EAX             ; PathOfEx.00EA23C0
	$-104    006D2AE4     8708                  XCHG DWORD PTR DS:[EAX], ECX
	$-102    006D2AE6     6A 10                 PUSH 0x10
	$-100    006D2AE8     E8 75FD5900           CALL PathOfEx.00C72862
	$-FB     006D2AED     F3:0F1045 08          MOVSS XMM0, DWORD PTR SS:[EBP+0x8]
	$-F6     006D2AF2     83C4 04               ADD ESP, 0x4
	$-F3     006D2AF5     894424 04             MOV DWORD PTR SS:[ESP+0x4], EAX          ; PathOfEx.00EA23C0
	$-EF     006D2AF9     8970 08               MOV DWORD PTR DS:[EAX+0x8], ESI
	$-EC     006D2AFC     8D48 04               LEA ECX, DWORD PTR DS:[EAX+0x4]
	$-E9     006D2AFF     F3:0F1140 0C          MOVSS DWORD PTR DS:[EAX+0xC], XMM0
	$-E4     006D2B04     C700 5823EA00         MOV DWORD PTR DS:[EAX], PathOfEx.00EA235>
	$-DE     006D2B0A     33C0                  XOR EAX, EAX                             ; PathOfEx.00EA23C0
	$-DC     006D2B0C     8701                  XCHG DWORD PTR DS:[ECX], EAX             ; PathOfEx.00EA23C0
	$-DA     006D2B0E     8BC1                  MOV EAX, ECX
	$-D8     006D2B10     BA 08670001           MOV EDX, PathOfEx.01006708
	$-D3     006D2B15     8702                  XCHG DWORD PTR DS:[EDX], EAX             ; PathOfEx.00EA23C0
	$-D1     006D2B17     8708                  XCHG DWORD PTR DS:[EAX], ECX
	$-CF     006D2B19     EB 1C                 JMP SHORT PathOfEx.006D2B37
	$-CD     006D2B1B     F3:0F1045 08          MOVSS XMM0, DWORD PTR SS:[EBP+0x8]
	$-C8     006D2B20     51                    PUSH ECX
	$-C7     006D2B21     F3:0F110424           MOVSS DWORD PTR SS:[ESP], XMM0
	$-C2     006D2B26     E8 F5551E00           CALL PathOfEx.008B8120
	$-BD     006D2B2B     F3:0F104D 08          MOVSS XMM1, DWORD PTR SS:[EBP+0x8]
	$-B8     006D2B30     8BCE                  MOV ECX, ESI
	$-B6     006D2B32     E8 A95A1E00           CALL PathOfEx.008B85E0
	$-B1     006D2B37     83BE C8010000 00      CMP DWORD PTR DS:[ESI+0x1C8], 0x0
	$-AA     006D2B3E     74 0F                 JE SHORT PathOfEx.006D2B4F
	$-A8     006D2B40     8B06                  MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00EA23C0
	$-A6     006D2B42     8BCE                  MOV ECX, ESI
	$-A4     006D2B44     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]         ; PathOfEx.006D5FA0
	$-A1     006D2B47     8B8E C8010000         MOV ECX, DWORD PTR DS:[ESI+0x1C8]
	$-9B     006D2B4D     EB 16                 JMP SHORT PathOfEx.006D2B65
	$-99     006D2B4F     83BE C4010000 00      CMP DWORD PTR DS:[ESI+0x1C4], 0x0
	$-92     006D2B56     74 1D                 JE SHORT PathOfEx.006D2B75
	$-90     006D2B58     8B06                  MOV EAX, DWORD PTR DS:[ESI]              ; PathOfEx.00EA23C0
	$-8E     006D2B5A     8BCE                  MOV ECX, ESI
	$-8C     006D2B5C     FF50 0C               CALL NEAR DWORD PTR DS:[EAX+0xC]         ; PathOfEx.006D5FA0
	$-89     006D2B5F     8B8E C4010000         MOV ECX, DWORD PTR DS:[ESI+0x1C4]
	$-83     006D2B65     8B49 20               MOV ECX, DWORD PTR DS:[ECX+0x20]
	$-80     006D2B68     8B10                  MOV EDX, DWORD PTR DS:[EAX]              ; PathOfEx.006D5100
	$-7E     006D2B6A     FF71 14               PUSH DWORD PTR DS:[ECX+0x14]
	$-7B     006D2B6D     FF71 10               PUSH DWORD PTR DS:[ECX+0x10]
	$-78     006D2B70     8BC8                  MOV ECX, EAX                             ; PathOfEx.00EA23C0
	$-76     006D2B72     FF52 3C               CALL NEAR DWORD PTR DS:[EDX+0x3C]        ; nvd3dum.64D0F8A1
	$-73     006D2B75     E8 A63B2200           CALL PathOfEx.008F6720
	$-6E     006D2B7A     F3:0F104D 08          MOVSS XMM1, DWORD PTR SS:[EBP+0x8]
	$-69     006D2B7F     8B08                  MOV ECX, DWORD PTR DS:[EAX]              ; PathOfEx.006D5100
	$-67     006D2B81     E8 7A322200           CALL PathOfEx.008F5E00
	$-62     006D2B86     5E                    POP ESI                                  ; 1CFE2140
	$-61     006D2B87     8BE5                  MOV ESP, EBP
	$-5F     006D2B89     5D                    POP EBP                                  ; 1CFE2140
	$-5E     006D2B8A     C2 0400               RETN 0x4
	$-5B     006D2B8D     CC                    INT3
	$-5A     006D2B8E     CC                    INT3
	$-59     006D2B8F     CC                    INT3
	$-58     006D2B90     51                    PUSH ECX
	$-57     006D2B91     803D B5FBFE00 00      CMP BYTE PTR DS:[0xFEFBB5], 0x0
	$-50     006D2B98     56                    PUSH ESI
	$-4F     006D2B99     8BF1                  MOV ESI, ECX
	$-4D     006D2B9B     74 40                 JE SHORT PathOfEx.006D2BDD
	$-4B     006D2B9D     6A 10                 PUSH 0x10
	$-49     006D2B9F     E8 BEFC5900           CALL PathOfEx.00C72862
	$-44     006D2BA4     F3:0F104424 10        MOVSS XMM0, DWORD PTR SS:[ESP+0x10]
	$-3E     006D2BAA     83C4 04               ADD ESP, 0x4
	$-3B     006D2BAD     894424 04             MOV DWORD PTR SS:[ESP+0x4], EAX          ; PathOfEx.00EA23C0
	$-37     006D2BB1     8970 08               MOV DWORD PTR DS:[EAX+0x8], ESI
	$-34     006D2BB4     8D48 04               LEA ECX, DWORD PTR DS:[EAX+0x4]
	$-31     006D2BB7     F3:0F1140 0C          MOVSS DWORD PTR DS:[EAX+0xC], XMM0
	$-2C     006D2BBC     C700 5423EA00         MOV DWORD PTR DS:[EAX], PathOfEx.00EA235>
	$-26     006D2BC2     33C0                  XOR EAX, EAX                             ; PathOfEx.00EA23C0
	$-24     006D2BC4     8701                  XCHG DWORD PTR DS:[ECX], EAX             ; PathOfEx.00EA23C0
	$-22     006D2BC6     8BC1                  MOV EAX, ECX
	$-20     006D2BC8     BA 14670001           MOV EDX, PathOfEx.01006714               ; ASCII "g"
	$-1B     006D2BCD     8702                  XCHG DWORD PTR DS:[EDX], EAX             ; PathOfEx.00EA23C0
	$-19     006D2BCF     8708                  XCHG DWORD PTR DS:[EAX], ECX
	$-17     006D2BD1     E8 4AFEFFFF           CALL PathOfEx.006D2A20
	$-12     006D2BD6     E8 65FBFFFF           CALL PathOfEx.006D2740
	$-D      006D2BDB     EB 1C                 JMP SHORT PathOfEx.006D2BF9
	$-B      006D2BDD     F3:0F104424 0C        MOVSS XMM0, DWORD PTR SS:[ESP+0xC]
	$-5      006D2BE3     8D4424 04             LEA EAX, DWORD PTR SS:[ESP+0x4]
	$-1      006D2BE7     50                    PUSH EAX                                 ; PathOfEx.00EA23C0
	$ ==>    006D2BE8     8D8E 28020000         LEA ECX, DWORD PTR DS:[ESI+0x228]        ; NPC、怪物、玩家、WorldItem等的二叉树
	$+6      006D2BEE     F3:0F114424 08        MOVSS DWORD PTR SS:[ESP+0x8], XMM0
	$+C      006D2BF4     E8 F7691E00           CALL PathOfEx.008B95F0					 ; 遍历二叉树的算法
	$+11     006D2BF9     F3:0F104C24 0C        MOVSS XMM1, DWORD PTR SS:[ESP+0xC]
	$+17     006D2BFF     8D8E D0020000         LEA ECX, DWORD PTR DS:[ESI+0x2D0]
	$+1D     006D2C05     E8 F6A70100           CALL PathOfEx.006ED400
	$+22     006D2C0A     5E                    POP ESI                                  ; 1CFE2140
	$+23     006D2C0B     59                    POP ECX                                  ; 1CFE2140
	$+24     006D2C0C     C2 0400               RETN 0x4
	$+27     006D2C0F     CC                    INT3
	$+28     006D2C10     51                    PUSH ECX
	$+29     006D2C11     56                    PUSH ESI
	$+2A     006D2C12     8BF1                  MOV ESI, ECX
	$+2C     006D2C14     57                    PUSH EDI
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8d??????????f3??????????e8????????f3??????????8d??????????e8", 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x150 && dw_tmp < 0x400);
	ofs << "pt_cdword	g_o_map_obj_btree_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
